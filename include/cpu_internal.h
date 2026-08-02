#ifndef CPU_INTERNAL_H
#define CPU_INTERNAL_H

#include "alu.h"
#include "decoder.h"
#include <stdint.h>

typedef struct biu BIU;
typedef struct eu EU;
typedef struct opcode Opcode;

struct ImmedBuf {
    uint8_t byteArr[4];
    uint8_t byteIdx;

    uint8_t immedOffset;
};

extern BIU* cpu_biu_init(void);

extern void cpu_biu_free(BIU* biu);

extern uint8_t cpu_biu_dequeue_iq(BIU* biu);

extern void cpu_biu_enqueue_iq(BIU* biu, uint16_t byte);

extern uint8_t cpu_biu_iq_is_full(BIU* biu);

extern void cpu_biu_iq_clear(BIU* biu);

extern uint8_t cpu_biu_extern_r8(BIU* biu, uint32_t address);

extern uint16_t cpu_biu_extern_r16(BIU* biu, uint32_t address);

extern void cpu_biu_extern_w8(BIU* biu, uint32_t address, uint8_t data);

extern void cpu_biu_extern_w16(BIU* biu, uint32_t address, uint16_t data);

extern uint16_t cpu_biu_get_ir(BIU* biu);

extern void cpu_biu_set_ir(BIU* biu, uint16_t data);

extern uint16_t cpu_biu_get_seg_reg(BIU* biu, uint8_t idx);

extern void cpu_biu_set_seg_reg(BIU* biu, uint8_t idx, uint16_t data);

extern uint32_t cpu_biu_calculate_physical_address(BIU* biu, uint8_t idx, uint16_t offset);

extern EU* cpu_eu_init(void);

extern void cpu_eu_free(EU* eu);

extern uint16_t cpu_eu_get_data_reg(EU* eu, uint8_t idx, uint8_t access);

extern void cpu_eu_set_data_reg(EU* eu, uint8_t idx, uint16_t data, uint8_t access);

extern Opcode* cpu_eu_decode_opcode(uint8_t opcode);

extern uint8_t cpu_eu_is_instru_prefix(uint8_t byte);

extern struct ModRM cpu_eu_decode_modrm(uint8_t byte);

uint16_t cpu_eu_calculate_effective_address(EU* eu, AddressingMode mode, uint8_t op, struct ImmedBuf* buf);

uint16_t cpu_eu_execute(EU* eu, AluIn op);

void cpu_eu_print_debugging(EU* eu);

#endif
