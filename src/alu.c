#include "alu.h"
#include "bitset.h"
#include "decoder.h"
#include "svec.h"

 
static AluOut alu_add(AluIn in);

Alu* alu_init(void) {
    Alu* alu = xmalloc(sizeof(alu) * ALU_SIZE);

    alu[0] = alu_add;

    return alu; 
}

void alu_free(Alu* alu) {
    free(alu);
}

AluIn* alu_init_op(AluOperator operator, uint16_t* temp1, uint16_t* temp2, uint8_t width) {
    AluIn* op = xmalloc(sizeof(*op));

    op->operator = operator;
    op->temp1 = temp1;
    op->temp2 = temp2;
    op->width = width;

    return op;
}

void alu_free_op(AluIn* in) {
    free(in);
}

static AluOut alu_add(AluIn in) {
        AluOut out;
        uint16_t temp1 = *in.temp1;
        uint16_t temp2 = *in.temp2;

        if (in.width == OPERAND_WORD_DATA) {
            temp1 = (uint8_t) temp1;
            temp2 = (uint8_t) temp2;
        }

        out.val = temp1 + temp2;

        return out;
}
