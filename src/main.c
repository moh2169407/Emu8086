#include "cpu.h"
#include "cpu_internal.h"
#include "register.h"


#include <stdio.h>


int main(void) {
    /*
     * Manually loading add ax, bx
     *
     */ 
    // 000 000 - Opcode
    // 1 - Destination 
    // 1 - Word
    // 0000 0011

    // Mod 11 Register Mode
    // 000 - AX
    // 011 - BX
    // 0000 0011 1100 0011
    // 0x03C3
    //

    CPU* cpu = cpu_init(CPU_MINIMUM_MODE);

    cpu_biu_enqueue_iq(cpu->biu, 0x03);
    cpu_biu_enqueue_iq(cpu->biu, 0xC3);

    cpu_eu_write16(cpu->eu, REG_AX, 10);
    cpu_eu_write16(cpu->eu, REG_BX, 54);

    cpu_eu_print_debugging(cpu->eu);

    cpu_exei(cpu);


    // add 50

    cpu_eu_print_debugging(cpu->eu);
    // 0000 0100 0011 0010
    // 0x0432

    cpu_biu_enqueue_iq(cpu->biu, 0x04);
    cpu_biu_enqueue_iq(cpu->biu, 0x32);
    // cpu_eu_set_data_reg(cpu->eu, REG_AX, 04, REGISTER_WORD_ACCESS);
    // cpu_eu_set_data_reg(cpu->eu, REG_BX, 32, REGISTER_WORD_ACCESS);
    //
    //
    // cpu_exei(cpu);

    cpu_eu_print_debugging(cpu->eu);
    cpu_free(cpu);
}

