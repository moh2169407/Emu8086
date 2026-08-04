#include "register.h"

#include <assert.h>
#include <stdatomic.h>
#include <stdint.h>
#include "svec.h"
// #include "bitset.h"
//

#define BYTE_TO_BINARY_4(val)  ((val) >> 3) & 1, ((val) >> 2) & 1, ((val) >> 1) & 1, (val) & 1


struct dataregister {
    union {
        uint16_t registers[4];
        uint8_t split[8];
    }; 
    struct {
        uint16_t sp;
        uint16_t bp;
        uint16_t si;
        uint16_t di;
    };
};

struct segmentregister { 
    uint16_t cs;
    uint16_t ds;
    uint16_t ss;
    uint16_t es;
};

SegmentRegister* register_init_seg_regs(void) {
    SegmentRegister* segReg = xmalloc(sizeof(*segReg));
    memset(segReg, 0, sizeof(*segReg));
    return segReg;
}

void register_free_seg_regs(SegmentRegister* segReg) {
    assert(segReg != NULL);
    free(segReg);
}

uint16_t registers_read_seg_reg(SegmentRegister* regs, SegmentType type) {
    int16_t val;

    switch (type) {
    case CS:
        val = regs->cs;
    break;
    case DS:
        val = regs->ds;
    break;
    case SS:
        val = regs->ss;
    break;
    case ES:
        val = regs->es;
      break;

      default:
        val = 0x0000;
    }

    return val;
}

void registers_write_seg_reg(SegmentRegister* regs, SegmentType type, uint16_t data) {
    switch (type) {
    case CS:
        regs->cs = data;
    break;
    case DS:
        regs->ds = data;
    break;
    case SS:
        regs->ss = data;
    break;
    case ES:
        regs->es = data;
      break;
    }
}

 DataRegister* register_init_data_regs(void) {
     DataRegister* dataReg = xmalloc(sizeof(*dataReg));
     memset(dataReg, 0, sizeof(*dataReg));
     return dataReg;
 }

void register_free_data_regs(DataRegister* dataReg) {
    assert(dataReg != NULL);
    free(dataReg);
}

// Array            // Split
// [ 0 ] = AX   -- [0] AL [1] AH 
// [ 1 ] = BX   -- [2] BL [3] BH
// [ 2 ] = CX   -- [4] CL [5] CH
// [ 3 ] = DX   -- [6] DL [7] DH

uint16_t registers_read_data_regs(DataRegister* regs, uint8_t idx, RegisterAccessType access) {
    if (access == REGISTER_BYTE_ACCESS) {
        switch (idx) {
            case REG_AL:
                return regs->split[0];
            case REG_CL:
                return regs->split[4];
            case REG_DL:
                return regs->split[6];
            case REG_BL:
                return regs->split[2];
            case REG_AH:
                return regs->split[1];
            case REG_CH:
                return regs->split[5];
            case REG_DH:
                return regs->split[7];
            case REG_BH:
                return regs->split[3];
        }
    }
    else if (access == REGISTER_WORD_ACCESS) {
        switch (idx) {
            case REG_AX:
                return regs->registers[0];
            case REG_CX:
                return regs->registers[2];
            case REG_DX:
                return regs->registers[3];
            case REG_BX:
                return regs->registers[1];
            case REG_SP:
                return regs->sp;
            case REG_BP:
                return regs->bp;
            case REG_SI:
                return regs->si;
            case REG_DI:
                return regs->di;
        } 
    }
    return 0x0000;
}

void registers_write_data_regs(DataRegister* regs, uint8_t idx, RegisterAccessType access, void* data) {
    if (access == REGISTER_BYTE_ACCESS) {
        switch (idx) {
            case REG_AL:
                regs->split[0] = *(uint8_t*)data;
            break;
            case REG_CL:
                 regs->split[4] = *(uint8_t*)data;
            break;
            case REG_DL:
                 regs->split[6] = *(uint8_t*)data;
            break;
            case REG_BL:
                 regs->split[2] = *(uint8_t*)data;
            break;
            case REG_AH:
                 regs->split[1] = *(uint8_t*)data;
            break;
            case REG_CH:
                 regs->split[5] = *(uint8_t*)data;
            break;
            case REG_DH:
                 regs->split[7] = *(uint8_t*)data;
            break;
            case REG_BH:
                 regs->split[3] = *(uint8_t*)data;
            break;
        }
    }
    else if (access == REGISTER_WORD_ACCESS) {
        switch (idx) {
            case REG_AX:
                 regs->registers[0] = *(uint16_t*)data;
            break;
            case REG_CX:
                 regs->registers[2] = *(uint16_t*)data;
            break;
            case REG_DX:
                 regs->registers[3] = *(uint16_t*)data;
            break;
            case REG_BX:
                 regs->registers[1] = *(uint16_t*)data;
            break;
            case REG_SP:
                 regs->sp = *(uint16_t*)data;
            break;
            case REG_BP:
                 regs->bp = *(uint16_t*)data;
            break;
            case REG_SI:
                 regs->si = *(uint16_t*)data;
            break;
            case REG_DI:
                 regs->di = *(uint16_t*)data;
            break;
        } 
    }
}


void register_print_debugging_info(DataRegister* dReg) {
    #ifdef PRINT_DEBUGGING
    fprintf(stdout, "------ AX=%d -------\n", dReg->registers[0]);
    fprintf(stdout, "--- AL=0x%02X --- AH=0x%02X\n", dReg->split[0], dReg->split[1]);


    fprintf(stdout, "------ BX=%d -------\n", dReg->registers[1]);
    fprintf(stdout, "--- BL=0x%02X --- BH=0x%02X\n", dReg->split[2], dReg->split[3]);


    fprintf(stdout, "------ CX=%d -------\n", dReg->registers[2]);
    fprintf(stdout, "--- CL=0x%02X --- CH=0x%02X\n", dReg->split[4], dReg->split[5]);


    fprintf(stdout, "------ DX=%d -------\n", dReg->registers[3]);
    fprintf(stdout, "--- DL=0x%02X --- DH=0x%02X\n", dReg->split[6], dReg->split[7]);


    fprintf(stdout, "------ SP=%d -------\n", dReg->sp);
    fprintf(stdout, "------ BP=%d -------\n", dReg->bp);
    fprintf(stdout, "------ SI=%d -------\n", dReg->si);
    fprintf(stdout, "------ DI=%d -------\n", dReg->di);
    #endif
}

