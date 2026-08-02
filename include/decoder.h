#ifndef DECODER_H
#define DECODER_H

#include "alu.h"
#include "register.h"
#include <stdint.h>

enum {
    DIRECTION_REG_SOURCE = 0,
    DIRECTION_REG_DEST,
};

typedef enum {
    ADDRESSING_MM = 0,
    ADDRESSING_MMBD = 1,
    ADDRESSING_MMWD = 2,
    ADDRESSING_RM = 3,
} AddressingMode;


typedef struct opcode {
    const char* const mnemonic;
    uint8_t modrm;
    RegisterAccessType oprandWidth;
    uint8_t direction;
    uint8_t immed;
    uint8_t isGroup;
    uint8_t hasImpliedOperand;
    uint8_t impliedOperand;
    AluOperator operation;
} Opcode;

struct ModRM {
    AddressingMode mode; 
    uint8_t reg;
    uint8_t regMem;
};

Opcode* decoder_loopup_opcode(uint8_t opcode);

#endif

