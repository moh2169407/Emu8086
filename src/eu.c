#include "alu.h"
#include "cpu_internal.h"

#include <assert.h>
#include <string.h>

#include "register.h"
#include "svec.h"
#include "decoder.h"
#include "bitset.h"

#define MASK_MOD(byte) (byte & 0xC0) >> 6
#define MASK_REG(byte) (byte & 0x38) >> 3
#define MASK_REGM(byte) (byte & 0x07) 

static uint16_t cpu_eu_memMode(EU* eu, uint8_t idx, struct ImmedBuf* buf);
static uint16_t cpu_eu_memModeB(EU* eu, uint8_t idx, struct ImmedBuf* buf);
static uint16_t cpu_eu_memModeW(EU* eu, uint8_t idx, struct ImmedBuf* buf);

typedef uint16_t (*AddressingTarget)(EU* eu, uint8_t idx, struct ImmedBuf* buf);
    
static const AddressingTarget addressingMode[3] = {cpu_eu_memMode, cpu_eu_memModeB, cpu_eu_memModeW};

struct eu {
    DataRegister* dataReg;
    Alu* alu;
    BitSet* flags; 
};

EU* cpu_eu_init(void) {
    EU* eu = xmalloc(sizeof(*eu));
    eu->dataReg = register_init_data_regs();    
    eu->alu = alu_init();
    eu->flags = bitset_init_bset(1);
   
    return eu;
}

void cpu_eu_free(EU* eu) {
    assert(eu != NULL);
    register_free_data_regs(eu->dataReg);
    alu_free(eu->alu);
    bitset_free_bset(eu->flags);

    free(eu);
}

uint8_t cpu_eu_read8(EU* eu, uint8_t idx) {
    return registers_read_data_regs(eu->dataReg, idx, REGISTER_BYTE_ACCESS);
}
extern uint16_t cpu_eu_read16(EU* eu, uint8_t idx) {
    return registers_read_data_regs(eu->dataReg, idx, REGISTER_WORD_ACCESS);
}

void cpu_eu_write8(EU* eu, uint8_t idx, uint16_t data) {
    registers_write_data_regs(eu->dataReg, idx, REGISTER_BYTE_ACCESS, &data);
}

extern void cpu_eu_write16(EU* eu, uint8_t idx, uint16_t data) {
    registers_write_data_regs(eu->dataReg, idx, REGISTER_WORD_ACCESS, &data);
}

Opcode* cpu_eu_decode_opcode(uint8_t opcode) {
    return decoder_loopup_opcode(opcode);
}

struct ModRM cpu_eu_decode_modrm(uint8_t byte) {
    return (struct ModRM){
      .mode = MASK_MOD(byte),
      .reg = MASK_REG(byte),
      .regMem = MASK_REGM(byte)
      };
}

uint8_t cpu_eu_is_instru_prefix(uint8_t byte) {
    switch (byte) {
        case PREFIX_REPE:
        case PREFIX_REPNE:
        case PREFIX_SEGMENT_OVERRIDE_CS:
        case PREFIX_SEGMENT_OVERRIDE_DS:
        case PREFIX_SEGMENT_OVERRIDE_SS:
        case PREFIX_SEGEMENT_OVERRIDE_ES:
        case PREFIX_LOCK:;
            return TRUE;
        break;
    }
    return FALSE;
}

uint16_t cpu_eu_calculate_effective_address(EU* eu, AddressingMode mode, uint8_t op, struct ImmedBuf* buf) {
    return addressingMode[mode](eu, op, buf);      
}


static uint16_t cpu_eu_memMode(EU* eu, uint8_t idx, struct ImmedBuf* buf) {
    uint16_t temp1, temp2;
    switch (idx) {
        case 0x00:
            temp1 = cpu_eu_read16(eu, REG_BX);
            temp2 = cpu_eu_read16(eu, REG_SI);
            return temp1 + temp2;
        case 0x01:
            temp1 = cpu_eu_read16(eu, REG_BX);
            temp2 = cpu_eu_read16(eu, REG_DI);
            return temp1 + temp2;
        case 0x02:
            temp1 = cpu_eu_read16(eu, REG_BP);
            temp2 = cpu_eu_read16(eu, REG_SI);
            return temp1 + temp2;
        case 0x03:
            temp1 = cpu_eu_read16(eu, REG_BP);
            temp2 = cpu_eu_read16(eu, REG_DI);
            return temp1 + temp2;
        case 0x04:
            temp1 = cpu_eu_read16(eu, REG_SI);
            return temp1;
        case 0x05:
            temp1 = cpu_eu_read16(eu, REG_DI);
            return temp1;
        case 0x06:
            return (uint16_t) buf->byteArr[0];
        case 0x07:
            temp1 = cpu_eu_read16(eu, REG_BX);
            return temp1;
    }
    return 0x00;
}
static uint16_t cpu_eu_memModeB(EU* eu, uint8_t idx, struct ImmedBuf* buf) {
    uint16_t temp1, temp2, temp3;
    switch (idx) {
        case 0x00:
            temp1 = cpu_eu_read16(eu, REG_BX);
            temp2 = cpu_eu_read16(eu, REG_SI);
            temp3 =  (uint8_t) buf->byteArr[0];
            return temp1 + temp2 + temp3;
        case 0x01:
            temp1 = cpu_eu_read16(eu, REG_BX);
            temp2 = cpu_eu_read16(eu, REG_DI);
            temp3 =  (uint8_t) buf->byteArr[0];
            return temp1 + temp2 + temp3;
        case 0x02:
            temp1 = cpu_eu_read16(eu, REG_BP);
            temp2 = cpu_eu_read16(eu, REG_SI);
            temp3 =  (uint8_t) buf->byteArr[0];
            return temp1 + temp2 + temp3;
        case 0x03:
            temp1 = cpu_eu_read16(eu, REG_BP);
            temp2 = cpu_eu_read16(eu, REG_DI);
            temp3 =  (uint8_t) buf->byteArr[0];
            return temp1 + temp2 + temp3;
        case 0x04:
            temp1 = cpu_eu_read16(eu, REG_SI);
            temp3 =  (uint8_t) buf->byteArr[0];
            return temp1 + temp3;
        case 0x05:
            temp1 = cpu_eu_read16(eu, REG_DI);
            temp3 =  (uint8_t) buf->byteArr[0];
            return temp1 + temp3;
        case 0x06:
            temp1 = cpu_eu_read16(eu, REG_BP);
            temp3 =  (uint8_t) buf->byteArr[0];
            return temp1 + temp3;
        case 0x07:
            temp1 = cpu_eu_read16(eu, REG_BX);
            temp3 =  (uint8_t) buf->byteArr[0];
            return temp1 + temp3;
    }
    return 0x00;
}
static uint16_t cpu_eu_memModeW(EU* eu, uint8_t idx, struct ImmedBuf* buf) {
    uint16_t temp1, temp2, temp3;
    switch (idx) {
        case 0x00:
            temp1 = cpu_eu_read16(eu, REG_BX);
            temp2 = cpu_eu_read16(eu, REG_SI);
            temp3 =  (uint16_t) buf->byteArr[0];
            return temp1 + temp2 + temp3;
        case 0x01:
            temp1 = cpu_eu_read16(eu, REG_BX);
            temp2 = cpu_eu_read16(eu, REG_DI);
            temp3 =  (uint16_t) buf->byteArr[0];
            return temp1 + temp2 + temp3;
        case 0x02:
            temp1 = cpu_eu_read16(eu, REG_BP);
            temp2 = cpu_eu_read16(eu, REG_SI);
            temp3 =  (uint16_t) buf->byteArr[0];
            return temp1 + temp2 + temp3;
        case 0x03:
            temp1 = cpu_eu_read16(eu, REG_BP);
            temp2 = cpu_eu_read16(eu, REG_DI);
            temp3 =  (uint16_t) buf->byteArr[0];
            return temp1 + temp2 + temp3;
        case 0x04:
            temp1 = cpu_eu_read16(eu, REG_SI);
            temp3 =  (uint16_t) buf->byteArr[0];
            return temp1 + temp3;
        case 0x05:
            temp1 = cpu_eu_read16(eu, REG_DI);
            temp3 =  (uint16_t) buf->byteArr[0];
            return temp1 + temp3;
        case 0x06:
            temp1 = cpu_eu_read16(eu, REG_BP);
            temp3 =  (uint16_t) buf->byteArr[0];
            return temp1 + temp3;
        case 0x07:
            temp1 = cpu_eu_read16(eu, REG_BX);
            temp3 =  (uint16_t) buf->byteArr[0];
            return temp1 + temp3;
    }
    return 0x00;
}

uint16_t cpu_eu_execute(EU* eu, AluIn op) {
    return eu->alu[op.operator](op).val;
}

void cpu_eu_print_debugging(EU* eu) {
    register_print_debugging_info(eu->dataReg); 
}
    

