#include "cpu.h"

#include <assert.h>
#include <stdint.h>

#include "alu.h"
#include "decoder.h"
#include "register.h"
#include "svec.h"
#include "cpu_internal.h"

typedef enum {
    INSTRU_DECODING_OPCODE,
    INSTR_EXPECTING_BYTE,
    INSTR_MODRM,
    INSTR_EXEC,
    INSTR_FINISHED,
} InstruState;

// struct cpu {
//     EU* eu;
//     BIU* biu;
//
//     uint8_t intr_line_status;
//     uint8_t min_mode;
//     uint8_t io_line_status; 
//
//     SegmentType segment;
// };

struct CpuContext {
    CPU* cpu;
    struct ImmedBuf buf;
    struct ModRM mod;
    Opcode* opcode;
};

void set_instru_prefix_flags(CPU*cpu, uint8_t byte);
static void source_modrn_operands(struct CpuContext* context, uint16_t* temp1, uint16_t* temp2);
static void source_immed_operands(struct CpuContext* context, uint16_t* temp1, uint16_t* temp2);

CPU* cpu_init(uint8_t min_mode) {
    CPU* cpu = xmalloc(sizeof(*cpu));
    cpu->minMode = min_mode;

    cpu->biu = cpu_biu_init();
    cpu->eu = cpu_eu_init();

    cpu->intrLineStatus = 0;
    // cpu->io_line_status = 0;

    return cpu;
}

void cpu_free(CPU* cpu) {
    assert(cpu != NULL);

    cpu_biu_free(cpu->biu);
    cpu_eu_free(cpu->eu);

    free(cpu);
}

void cpu_exei(CPU* cpu) {
    int byte;

    InstruState state = INSTRU_DECODING_OPCODE; 
    struct CpuContext context = {0};
    context.cpu = cpu;

    int i = 0;

    uint16_t operand1, operand2 = 0;
    uint16_t result;

    do {
        if (state == INSTR_EXEC) {
            if (context.opcode->modrm) {
                source_modrn_operands(&context, &operand1, &operand2);
            } 

            else if (context.opcode->hasImpliedOperand && context.opcode->immed) {
                source_immed_operands(&context, &operand1, &operand2);
            }

            AluIn op = {
                .temp1 = &operand1,
                .temp2 = &operand2,
                .operator = context.opcode->operation,
                .width = context.opcode->oprandWidth,
                .type = 0,
            };

            result = cpu_eu_execute(cpu->eu, op);
    #ifdef PRINT_DEBUGGING
            printf("Operation: %s\n", context.opcode->mnemonic);
            printf("Operand1: %d\nOperands2: %d\n", operand1, operand2);
            printf("RESULT: %d\n", result);
    #endif
    

            if (context.opcode->direction == DIRECTION_REG_DEST) {
                if (context.opcode->oprandWidth == REGISTER_WORD_ACCESS) {
                    cpu_eu_write16(cpu->eu, context.mod.reg, result);
                }
                else {
                    cpu_eu_write8(cpu->eu, context.mod.reg, result);
                }
            }
            else {
                if (context.opcode->modrm && context.mod.mode != ADDRESSING_RM) {
                    // TODO 
                }
                else {
                    if (context.opcode->oprandWidth == REGISTER_WORD_ACCESS) {
                        cpu_eu_write16(cpu->eu, context.mod.regMem, result);
                    }
                    else {
                        cpu_eu_write8(cpu->eu, context.mod.regMem, result);
                    }
                }
            }

            state = INSTR_FINISHED;
            continue;
        }

        byte = cpu_biu_dequeue_iq(cpu->biu);
        if (state == INSTRU_DECODING_OPCODE) {
            if (cpu_eu_is_instru_prefix(byte)) {
                set_instru_prefix_flags(cpu, byte);
                continue;
            }

            context.opcode = cpu_eu_decode_opcode(byte);
            if (context.opcode->immed) {
                state = INSTR_EXPECTING_BYTE;
                i += context.opcode->oprandWidth;
            }
            if (context.opcode->modrm) {
                state = INSTR_MODRM;
            }
            else {
                state = INSTR_EXEC;
            }
            if (!cpu->segmentPrefixSet) {
                cpu->segment = context.opcode->defaultSegment; 
            }
            continue; 
        }

        if (state == INSTR_EXPECTING_BYTE) {
            assert(context.buf.byteIdx < 4 && context.buf.byteIdx + i < 4);
            context.buf.byteArr[context.buf.byteIdx++] = byte;
            state = (--i) ? INSTR_EXPECTING_BYTE : INSTR_EXEC;
        }


        if (state == INSTR_MODRM) {
            context.mod = cpu_eu_decode_modrm(byte);
            if (context.mod.mode == ADDRESSING_MMBD) {
                i += 1;
                context.buf.immedOffset = 1;
                state = INSTR_EXPECTING_BYTE;
            }
            else if (context.mod.mode == ADDRESSING_MMWD) {
                i += 2;
                context.buf.immedOffset = 2;
                state = INSTR_EXPECTING_BYTE;
            }
            else {state = INSTR_EXEC;}
            continue;
        }

    } while (state != INSTR_FINISHED);
}

void set_instru_prefix_flags(CPU*cpu, uint8_t byte) {
    switch (byte) {
        case PREFIX_SEGEMENT_OVERRIDE_ES:
            cpu->segment = ES; 
            cpu->segmentPrefixSet = TRUE;
        break;
        case PREFIX_SEGMENT_OVERRIDE_CS:
            cpu->segment = CS;
            cpu->segmentPrefixSet = TRUE;
        break;
        case PREFIX_SEGMENT_OVERRIDE_DS:
            cpu->segment = DS;
            cpu->segmentPrefixSet = TRUE;
        break;
        case PREFIX_SEGMENT_OVERRIDE_SS:
            cpu->segment = SS;
            cpu->segmentPrefixSet = TRUE;
        break;
        case PREFIX_LOCK:
            cpu->lockStatus = TRUE;
        break;
        case PREFIX_REPE:
            cpu->repP = REP_EQ;
        break;
        case PREFIX_REPNE:
            cpu->repP = REP_NEQ;
        break;
    }
}

static void source_modrn_operands(struct CpuContext* context, uint16_t* temp1, uint16_t* temp2) {
    uint32_t physicalAddress;
    uint16_t effectiveAddress;
    if (context->opcode->oprandWidth == REGISTER_WORD_ACCESS) {
        *temp1 = cpu_eu_read16(context->cpu->eu, context->mod.reg);
    }
    else {
        *temp1 = cpu_eu_read8(context->cpu->eu, context->mod.reg);
    }
    if (context->mod.mode == ADDRESSING_RM) {
        if (context->opcode->oprandWidth == REGISTER_WORD_ACCESS) {
            *temp2 = cpu_eu_read16(context->cpu->eu, context->mod.regMem);
        }
        else {
            *temp2 = cpu_eu_read8(context->cpu->eu, context->mod.regMem);
        }
    }
    else {
        effectiveAddress = cpu_eu_calculate_effective_address(context->cpu->eu, context->mod.mode, context->mod.regMem, &context->buf);
        physicalAddress = cpu_biu_calculate_physical_address(context->cpu->biu, context->cpu->segment, effectiveAddress);
        *temp2 = cpu_biu_extern_r16(context->cpu->biu, physicalAddress);
    }
}

static void source_immed_operands(struct CpuContext* context, uint16_t* temp1, uint16_t* temp2) {
    if (context->opcode->oprandWidth == REGISTER_WORD_ACCESS) {
        *temp1 = cpu_eu_read16(context->cpu->eu, context->mod.reg);
    }
    else {
        *temp1 = cpu_eu_read8(context->cpu->eu, context->mod.reg);
    }
    if (context->opcode->oprandWidth) {
        *temp2 = (uint16_t) context->buf.byteArr[context->buf.immedOffset];
    }
    else {
        *temp2 = (uint8_t) context->buf.byteArr[context->buf.immedOffset];
    }
}
