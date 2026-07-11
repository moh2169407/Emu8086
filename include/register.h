#ifndef REGISTER_H
#define REGISTER_H

#include <stdint.h>

typedef struct registers Register;

typedef enum {
    CARRY_FLAG = 0,
    PARITY_FLAG,
    AC_FLAG,
    ZERO_FLAG,
    SIGN_FLAG,
    OVERFLOW_FLAG,
    INTERRUPT_ENABLE_FLAG,
    DIRECTION_FLAG,
    TRAP_FLAG
} StatusFlags;

typedef enum {
    AX,
    AH,
    AL,
    BX,
    BH,
    BL,
    CX,
    CH,
    CL,
    DX,
    DH,
    DL
}RegisterType;


extern Register* register_init_regs(void);

extern void register_free_regs(Register* regs);

extern int16_t registers_read_instruction_reg(Register* regs);

extern void registers_write_instruction_reg(Register* regs, int16_t ir);

extern void register_reinit_on_reset(Register* regs);

extern void registers_clear_all_status_flags(Register* regs);

extern void registers_clear_status_flag(Register* regs, StatusFlags flag);

extern void registers_set_status_flag(Register* regs, StatusFlags flag);

extern int16_t registers_read_data_reg(Register* regs, RegisterType type);

extern void registers_write_data_reg(Register* regs, RegisterType type, int16_t data);

/* TODO
 * Status register functions*/

#endif
