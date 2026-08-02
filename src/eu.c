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
    
AddressingTarget addressingMode[3] = {cpu_eu_memMode, cpu_eu_memModeB, cpu_eu_memModeW};

struct eu {
    DataRegister* dataReg;
    Alu* alu;
    BitSet* flags; 
};

EU* cpu_eu_init(void) {
    EU* eu = xmalloc(sizeof(*eu));
    eu->dataReg = register_init_data_regs();    
    eu->alu = alu_init();
   
    return eu;
}

void cpu_eu_free(EU* eu) {
    assert(eu != NULL);
    register_free_data_regs(eu->dataReg);
    free(eu);
}


uint16_t cpu_eu_get_data_reg(EU* eu, uint8_t idx, uint8_t access) {
    return registers_read_data_regs(eu->dataReg, idx, access);
}

void cpu_eu_set_data_reg(EU* eu, uint8_t idx, uint16_t data, uint8_t access) {
    registers_write_data_regs(eu->dataReg, idx, access, (void*) &data);
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
        case 0xF3:
        case 0xF2:
        case 0x2E:
        case 0x3E:
        case 0x36:
        case 0x26:
        case 0xF0:
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
            temp1 = cpu_eu_get_data_reg(eu, REG_BX, REGISTER_WORD_ACCESS);
            temp2 = cpu_eu_get_data_reg(eu, REG_SI, REGISTER_WORD_ACCESS);
            return temp1 + temp2;
        case 0x01:
            temp1 = cpu_eu_get_data_reg(eu, REG_BX, REGISTER_WORD_ACCESS);
            temp2 = cpu_eu_get_data_reg(eu, REG_DI, REGISTER_WORD_ACCESS);
            return temp1 + temp2;
        case 0x02:
            temp1 = cpu_eu_get_data_reg(eu, REG_BP, REGISTER_WORD_ACCESS);
            temp2 = cpu_eu_get_data_reg(eu, REG_SI, REGISTER_WORD_ACCESS);
            return temp1 + temp2;
        case 0x03:
            temp1 = cpu_eu_get_data_reg(eu, REG_BP, REGISTER_WORD_ACCESS);
            temp2 = cpu_eu_get_data_reg(eu, REG_DI, REGISTER_WORD_ACCESS);
            return temp1 + temp2;
        case 0x04:
            temp1 = cpu_eu_get_data_reg(eu, REG_SI, REGISTER_WORD_ACCESS);
            return temp1;
        case 0x05:
            temp1 = cpu_eu_get_data_reg(eu, REG_DI, REGISTER_WORD_ACCESS);
            return temp1;
        case 0x06:
            return (int16_t) buf->byteArr[0];
        case 0x07:
            temp1 = cpu_eu_get_data_reg(eu, REG_BX, REGISTER_WORD_ACCESS);
            return temp1;
    }
    return 0x00;
}
static uint16_t cpu_eu_memModeB(EU* eu, uint8_t idx, struct ImmedBuf* buf) {
    uint16_t temp1, temp2, temp3;
    switch (idx) {
        case 0x00:
            temp1 = cpu_eu_get_data_reg(eu, REG_BX, REGISTER_WORD_ACCESS);
            temp2 = cpu_eu_get_data_reg(eu, REG_SI, REGISTER_WORD_ACCESS);
            temp3 =  (uint8_t) buf->byteArr[0];
            return temp1 + temp2 + temp3;
        case 0x01:
            temp1 = cpu_eu_get_data_reg(eu, REG_BX, REGISTER_WORD_ACCESS);
            temp2 = cpu_eu_get_data_reg(eu, REG_DI, REGISTER_WORD_ACCESS);
            temp3 =  (uint8_t) buf->byteArr[0];
            return temp1 + temp2 + temp3;
        case 0x02:
            temp1 = cpu_eu_get_data_reg(eu, REG_BP, REGISTER_WORD_ACCESS);
            temp2 = cpu_eu_get_data_reg(eu, REG_SI, REGISTER_WORD_ACCESS);
            temp3 =  (uint8_t) buf->byteArr[0];
            return temp1 + temp2 + temp3;
        case 0x03:
            temp1 = cpu_eu_get_data_reg(eu, REG_BP, REGISTER_WORD_ACCESS);
            temp2 = cpu_eu_get_data_reg(eu, REG_DI, REGISTER_WORD_ACCESS);
            temp3 =  (uint8_t) buf->byteArr[0];
            return temp1 + temp2 + temp3;
        case 0x04:
            temp1 = cpu_eu_get_data_reg(eu, REG_SI, REGISTER_WORD_ACCESS);
            temp3 =  (uint8_t) buf->byteArr[0];
            return temp1 + temp3;
        case 0x05:
            temp1 = cpu_eu_get_data_reg(eu, REG_DI, REGISTER_WORD_ACCESS);
            temp3 =  (uint8_t) buf->byteArr[0];
            return temp1 + temp3;
        case 0x06:
            temp1 = cpu_eu_get_data_reg(eu, REG_BP, REGISTER_WORD_ACCESS);
            temp3 =  (uint8_t) buf->byteArr[0];
            return temp1 + temp3;
        case 0x07:
            temp1 = cpu_eu_get_data_reg(eu, REG_BX, REGISTER_WORD_ACCESS);
            temp3 =  (uint8_t) buf->byteArr[0];
            return temp1 + temp3;
    }
    return 0x00;
}
static uint16_t cpu_eu_memModeW(EU* eu, uint8_t idx, struct ImmedBuf* buf) {
    uint16_t temp1, temp2, temp3;
    switch (idx) {
        case 0x00:
            temp1 = cpu_eu_get_data_reg(eu, REG_BX, REGISTER_WORD_ACCESS);
            temp2 = cpu_eu_get_data_reg(eu, REG_SI, REGISTER_WORD_ACCESS);
            temp3 =  (uint16_t) buf->byteArr[0];
            return temp1 + temp2 + temp3;
        case 0x01:
            temp1 = cpu_eu_get_data_reg(eu, REG_BX, REGISTER_WORD_ACCESS);
            temp2 = cpu_eu_get_data_reg(eu, REG_DI, REGISTER_WORD_ACCESS);
            temp3 =  (uint16_t) buf->byteArr[0];
            return temp1 + temp2 + temp3;
        case 0x02:
            temp1 = cpu_eu_get_data_reg(eu, REG_BP, REGISTER_WORD_ACCESS);
            temp2 = cpu_eu_get_data_reg(eu, REG_SI, REGISTER_WORD_ACCESS);
            temp3 =  (uint16_t) buf->byteArr[0];
            return temp1 + temp2 + temp3;
        case 0x03:
            temp1 = cpu_eu_get_data_reg(eu, REG_BP, REGISTER_WORD_ACCESS);
            temp2 = cpu_eu_get_data_reg(eu, REG_DI, REGISTER_WORD_ACCESS);
            temp3 =  (uint16_t) buf->byteArr[0];
            return temp1 + temp2 + temp3;
        case 0x04:
            temp1 = cpu_eu_get_data_reg(eu, REG_SI, REGISTER_WORD_ACCESS);
            temp3 =  (uint16_t) buf->byteArr[0];
            return temp1 + temp3;
        case 0x05:
            temp1 = cpu_eu_get_data_reg(eu, REG_DI, REGISTER_WORD_ACCESS);
            temp3 =  (uint16_t) buf->byteArr[0];
            return temp1 + temp3;
        case 0x06:
            temp1 = cpu_eu_get_data_reg(eu, REG_BP, REGISTER_WORD_ACCESS);
            temp3 =  (uint16_t) buf->byteArr[0];
            return temp1 + temp3;
        case 0x07:
            temp1 = cpu_eu_get_data_reg(eu, REG_BX, REGISTER_WORD_ACCESS);
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
    

