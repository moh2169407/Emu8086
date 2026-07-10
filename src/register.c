#include "register.h"

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

typedef struct {
    /* Didn't implement yet */   
} StatusRegister;


struct registers {
    DataRegister d_regs;
    SegmentRegister s_regs;
    StatusRegister ss_regs;
    int16_t ir;
};

