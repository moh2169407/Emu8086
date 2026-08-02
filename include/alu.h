#ifndef ALU_H
#define ALU_H

#include "bitset.h"
#include <stdint.h>

#define ALU_SIZE 1

typedef enum {
    ALU_ADD = 0,
} AluOperator;

typedef enum {
    ALU_UNARY_OPERATOR,
    ALU_BINARY_OPERATOR,
}AluType;

struct BitSet;

typedef struct {
    uint16_t* temp1;
    uint16_t* temp2;
    AluType type;

    AluOperator operator;
    uint8_t width;
}AluIn;

typedef struct {
    uint16_t val;
}AluOut;


typedef AluOut (*Alu)(AluIn op);

extern Alu* alu_init(void);

extern void alu_free(Alu* alu);

#endif
