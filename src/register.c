#include "register.h"
#include "svec.h"
#include "bitset.h"

typedef struct {
    union {
        int16_t registers[4];
        int8_t split[8];
    }; 
} DataRegister;


typedef struct {
    uint16_t cs;
    uint16_t ds;
    uint16_t ss;
    uint16_t es;
} SegmentRegister;


struct registers {
    DataRegister d_regs;
    SegmentRegister s_regs;
    BitSet ss_regs;
    int16_t ir;
};

Register* register_init_regs(void) {
    Register* regs = xmalloc(sizeof(*regs));
    memset(regs, 0, sizeof(*regs));

    return regs;
}

void register_free_regs(Register* regs) {
    free(regs);
}

int16_t registers_read_instruction_reg(Register* regs) {
    return regs->ir;
}

void registers_write_instruction_reg(Register* regs, int16_t ir) {
    regs->ir = ir;
}

void register_reinit_on_reset(Register* regs) {
    /* TODO 
     *  Sets default values in registers after resetting */
}

void registers_clear_all_status_flags(Register* regs) {
    regs->ss_regs;
}




