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


extern Register* register_init_regs(void);

extern void register_free_regs(Register* regs);

extern int16_t registers_get_instruction_reg(Register* regs);

extern void registers_set_instruction_reg(Register* regs, int16_t ir);

extern void register_reinit_on_reset(Register* regs);

extern void registers_clear_all_status_flags(Register* regs);

extern void registers_clear_status_flag(Register* regs, StatusFlags flag);

extern void registers_set_status_flag(Register* regs, StatusFlags flag);









#endif
