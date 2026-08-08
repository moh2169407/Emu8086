#include "cpu.h"
#include "cpu_internal.h"
#include "register.h"


#include <stdio.h>


int main(void) {
    // /*
    //  * Manually loading add ax, bx
    //  *
    //  */ 
    // // 000 000 - Opcode
    // // 1 - Destination 
    // // 1 - Word
    // // 0000 0011
    //
    // // Mod 11 Register Mode
    // // 000 - AX
    // // 011 - BX
    // // 0000 0011 1100 0011
    // // 0x03C3
    // //
    //
    // CPU* cpu = cpu_init(CPU_MINIMUM_MODE);
    //
    // cpu_biu_enqueue_iq(cpu->biu, 0x03);
    // cpu_biu_enqueue_iq(cpu->biu, 0xC3);
    //
    // cpu_eu_write16(cpu->eu, REG_AX, 10);
    // cpu_eu_write16(cpu->eu, REG_BX, 54);
    //
    // cpu_eu_print_debugging(cpu->eu);
    //
    // cpu_exei(cpu);
    // cpu_eu_print_debugging(cpu->eu);
    //
    // // Manually loading add ah, al
    // // At this point, the register should contain
    // // ah = 0, al = 64(0x40)
    // // 000 000 - Opcode Add
    // // 1 - Direction Reg is destination
    // // 0 - Byte width
    // // 11 - Register Mod
    // // 100 - AH
    // // 000 - AL
    // // 0000 0010 1110 0000
    // // 0x02E0
    //
    //
    // cpu_biu_enqueue_iq(cpu->biu, 0x02);
    // cpu_biu_enqueue_iq(cpu->biu, 0xE0);
    //
    // cpu_exei(cpu);
    //
    // cpu_eu_print_debugging(cpu->eu);
    //
    // // Manually loading add bx, ax
    // // At this point, bx should be 0 and ax=16448
    // // 000 000 - Opcode (Add)
    // // 1 - Destination is Reg
    // // 1 - Word width
    // // 11 - Register Mode
    // // 011  BX
    // // 000 AX
    // //
    // // 0000 0011 1101 1000
    // // 0x03D8
    //
    // cpu_biu_enqueue_iq(cpu->biu, 0x03);
    // cpu_biu_enqueue_iq(cpu->biu, 0xD8);
    //
    // cpu_exei(cpu);
    //
    // cpu_eu_print_debugging(cpu->eu);
    //
    //
    // cpu_free(cpu);
    // return 0;
    // Tests Work till this point
    
    CPU* cpu = cpu_init(CPU_MINIMUM_MODE);
    
    // Manually loading add 5
    // testing immediate, and implied registers
    //
    // 000 001 - addi 
    // 0 - Direction(Doesnt matter, it goes to implied register)
    // 0 - Width(Byte width)
    // 5 - Imed 
    //
    // 0000 0100 0000 0101
    // 0x0405


    cpu_biu_enqueue_iq(cpu->biu, 0x05);
    cpu_biu_enqueue_iq(cpu->biu, 0x05);
    cpu_biu_enqueue_iq(cpu->biu, 0x05);

    cpu_eu_write16(cpu->eu, REG_AX, 10);

    cpu_eu_print_debugging(cpu->eu);

    cpu_exei(cpu);

    cpu_eu_print_debugging(cpu->eu);

    cpu_free(cpu);
    return 0;
}

