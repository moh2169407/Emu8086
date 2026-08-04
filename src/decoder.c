#include "decoder.h"
#include "alu.h"
#include "register.h"

#include <svec.h>

#define MATRIX_WIDTH_SIZE (0x0F)
#define MATRIX_LENGTH_SIZE (0x0F)

#define MASK_TOP_NIBBLE(byte) (byte & 0xF0) >> 4
#define MASK_BOTTOM_NIBBLE(byte) (byte & 0x0F)


static Opcode opMatrix[MATRIX_WIDTH_SIZE][MATRIX_LENGTH_SIZE] = {
    [0x0][0x0] = (Opcode) {
        .mnemonic = "add",
        .oprandWidth = REGISTER_BYTE_ACCESS,
        .direction = DIRECTION_REG_SOURCE,
        .modrm = TRUE,
        .operation = ALU_ADD,
    },
    [0x0][0x1] = (Opcode) {
        .mnemonic = "add",
        .oprandWidth = REGISTER_WORD_ACCESS,
        .direction = DIRECTION_REG_SOURCE,
        .modrm = TRUE,
        .operation = ALU_ADD,
    },
    [0x0][0x2] = (Opcode) {
        .mnemonic = "add",
        .oprandWidth = REGISTER_BYTE_ACCESS,
        .direction = DIRECTION_REG_DEST,
        .modrm = TRUE,
        .operation = ALU_ADD,
    },
    [0x0][0x3] = (Opcode) {
        .mnemonic = "add",
        .oprandWidth = REGISTER_WORD_ACCESS,
        .direction = DIRECTION_REG_DEST,
        .modrm = TRUE,
        .operation = ALU_ADD,
    },
    [0x00][0x04] = (Opcode) {
        .mnemonic = "add",
        .oprandWidth = REGISTER_BYTE_ACCESS,
        .direction = DIRECTION_REG_DEST,
        .immed = TRUE,
        .operation = ALU_ADD,
        .hasImpliedOperand = TRUE,
        .impliedOperand = REG_AL,
    },
    [0x00][0x05] = (Opcode) {
        .mnemonic = "add",
        .oprandWidth = REGISTER_WORD_ACCESS,
        .direction = DIRECTION_REG_DEST,
        .immed = TRUE,
        .operation = ALU_ADD,
        .hasImpliedOperand = TRUE,
        .impliedOperand = REG_AX,
    }
};

Opcode* decoder_loopup_opcode(uint8_t opcode) {
    uint8_t high = MASK_TOP_NIBBLE(opcode);
    uint8_t low = MASK_BOTTOM_NIBBLE(opcode);
    return &opMatrix[high][low];
}
