#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include "cpu_internal.h"
#include "register.h"

typedef struct cpu CPU;

enum {
    CPU_MINIMUM_MODE = 0,
    CPU_MAXIMUM_MODE,
};


struct cpu {
    EU* eu;
    BIU* biu;

    uint8_t intr_line_status;
    uint8_t min_mode;
    uint8_t io_line_status; 

    SegmentType segment;
};

CPU* cpu_init(uint8_t min_mode);

void cpu_free(CPU* cpu);

void cpu_set_intr_line(CPU* cpu);

uint8_t cpu_is_intr_line_set(CPU* cpu);

void cpu_exei(CPU* cpu);


#endif
