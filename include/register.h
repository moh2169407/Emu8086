#ifndef REGISTER_H
#define REGISTER_H

#include <stdint.h>
#include <stdio.h>

#define REG_AL 0x0
#define REG_CL 0x1
#define REG_DL 0x2
#define REG_BL 0x3
#define REG_AH 0x4
#define REG_CH 0x5
#define REG_DH 0x6
#define REG_BH 0x7

#define REG_AX 0x0
#define REG_CX 0x1
#define REG_DX 0x2
#define REG_BX 0x3
#define REG_SP 0x4
#define REG_BP 0x5
#define REG_SI 0x6
#define REG_DI 0x7

typedef struct segmentregister SegmentRegister;

typedef struct dataregister DataRegister;

typedef enum {
    REGISTER_BYTE_ACCESS = 0,
    REGISTER_WORD_ACCESS = 1,
} RegisterAccessType;

// typedef enum {
//     CARRY_FLAG = 0,
//     PARITY_FLAG,
//     AC_FLAG,
//     ZERO_FLAG,
//     SIGN_FLAG,
//     OVERFLOW_FLAG,
//     INTERRUPT_ENABLE_FLAG,
//     DIRECTION_FLAG,
//     TRAP_FLAG
// } StatusFlags;

typedef enum {
    CS,
    DS,
    SS,
    ES,
} SegmentType;


extern SegmentRegister* register_init_seg_regs(void);

extern void register_free_seg_regs(SegmentRegister* segReg);

extern uint16_t registers_read_seg_reg(SegmentRegister* regs, SegmentType type);

extern void registers_write_seg_reg(SegmentRegister* regs, SegmentType type, uint16_t data);

extern DataRegister* register_init_data_regs(void);

extern void register_free_data_regs(DataRegister* segReg);

uint16_t registers_read_data_regs(DataRegister* regs, uint8_t idx, RegisterAccessType access);

void registers_write_data_regs(DataRegister* regs, uint8_t idx, RegisterAccessType access, void* data);


void register_print_debugging_info(DataRegister* dReg);

// extern void register_free_regs(Register* regs);
//
// extern int16_t registers_read_instruction_reg(Register* regs);
//
// extern void registers_write_instruction_reg(Register* regs, int16_t ir);
//
// extern void register_reinit_on_reset(Register* regs);
//
// extern void registers_clear_all_status_flags(Register* regs);
//
// extern void registers_clear_status_flag(Register* regs, StatusFlags flag);
//
// extern void registers_set_status_flag(Register* regs, StatusFlags flag);
//
// extern int16_t registers_read_data_reg(Register* regs, RegisterType type);
//
// extern void registers_write_data_reg(Register* regs, RegisterType type, int16_t data);
//
// /* TODO
//  * Status register functions*/
//
#endif
