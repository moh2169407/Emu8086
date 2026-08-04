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

typedef enum {
    REP_NONE = 0,
    REP_EQ,
    REP_NEQ,
} RepeartPrefix;

struct cpu {
    EU* eu;
    BIU* biu;

    uint8_t intrLineStatus;
    uint8_t minMode;

    RepeartPrefix repP;
    uint8_t lockStatus;

    SegmentType segment;
    uint8_t segmentPrefixSet;
};

CPU* cpu_init(uint8_t min_mode);

void cpu_free(CPU* cpu);

void cpu_set_intr_line(CPU* cpu);

uint8_t cpu_is_intr_line_set(CPU* cpu);

void cpu_exei(CPU* cpu);


#endif
