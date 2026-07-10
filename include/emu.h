#ifndef CORE_H
#define CORE_H

#include <stdint.h>

typedef struct emu8086 Emu8086;

typedef enum {
    MAXIMUM_MODE = 0,       /* The processor is running in multiprocessor or coprocessor mode */
    MINIMUM_MODE,           /* The processor is running in single processor mode */
} EmuMode;

typedef enum {
    EMU_HALT = 0,
    EMU_RUNNING,
    EMU_RESET,
} EmuStatus;

extern Emu8086* emu_init(EmuMode mode);         /* Initialize Emu8086 structure */
extern void emu_free(Emu8086* emu);            /* Frees Emu8086 structure*/

extern EmuMode emu_get_running_mode(Emu8086* emu);

extern int8_t emu_hard_reset(Emu8086* emu);    /**/

extern int8_t emu_run_main_loop(Emu8086* emu);     /* Starts and while(1) state machine loop */

extern int8_t emu_post_test(Emu8086* emu);         /* Executes the Power-On Self Test to check system hardware */

#endif
