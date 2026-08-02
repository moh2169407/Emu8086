#include "cpu_internal.h"

#include "register.h"
#include "svec.h"
#include "instr_queue.h"


struct biu {
    InstrQueue* iq;
    SegmentRegister* segReg;
    // TODO IO subsystem
    uint16_t ir;
};

BIU* cpu_biu_init(void) {
    BIU* biu = xmalloc(sizeof(*biu));
    biu->iq = iqueue_init();
    
    biu->segReg = register_init_seg_regs();

    /*
     * TODO 
     * need to implement an io subsystem
     */
    return biu;    
}


void cpu_biu_free(BIU* biu) {
    if (biu == NULL) {
        return;
    }

    iqueue_free(biu->iq);
    register_free_seg_regs(biu->segReg);

    free(biu);
}

uint8_t cpu_biu_dequeue_iq(BIU* biu) {
    return iqueue_dequeue(biu->iq);
}

void cpu_biu_enqueue_iq(BIU* biu, uint16_t byte) {
    iqueue_enqueue(biu->iq, byte);
}

uint8_t cpu_biu_iq_is_full(BIU* biu) {
    return iqueue_is_full(biu->iq);
}

void cpu_biu_iq_clear(BIU* biu) {
    iqueue_reset(biu->iq); 
}

uint16_t cpu_biu_get_ir(BIU* biu) {
    return biu->ir;
}

void cpu_biu_set_ir(BIU* biu, uint16_t data) {
    biu->ir = data;
}

uint16_t cpu_biu_get_seg_reg(BIU* biu, uint8_t idx) {
    return registers_read_seg_reg(biu->segReg, idx);
}

void cpu_biu_set_seg_reg(BIU* biu, uint8_t idx, uint16_t data) {
    registers_write_seg_reg(biu->segReg, idx, data);
}

uint32_t cpu_biu_calculate_physical_address(BIU *biu, uint8_t idx, uint16_t offset) {
    uint16_t base = cpu_biu_get_seg_reg(biu, idx);
    return (base << 4) + offset;
}

uint16_t cpu_biu_extern_r16(BIU* biu, uint32_t address) {
    // TODO 
    cpu_biu_calculate_physical_address(biu, 0, address);
    return 0;
}
