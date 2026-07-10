/*
 * Name: Mohamud Mohamud
 * Description: 
 *
 */
#ifndef INSTRQUEUE_H
#define INSTRQUEUE_H

#include <stdint.h>

typedef struct instrqueue InstrQueue;

extern InstrQueue* iqueue_init(void);                              /* Initialize queue internal memory */

extern void iqueue_free(InstrQueue* iqueue);                       /* Frees and cleans up queue internal memory */ 

extern int8_t iqueue_get_size(InstrQueue* iqueue);                 /* Returns the current size of the queue */

extern void iqueue_enqueue(InstrQueue* iqueue, uint16_t value);      /* Adds two bytes to back of the queue*/ 
                                                                    /* Silently doesn't do anything if the queue is already full */

extern int8_t iqueue_dequeue(InstrQueue* iqueue);                  /* Gets the first and returns the first byte from the queue */
    
extern void iqueue_reset(InstrQueue* queue);                       /* Reset queue and size back to zero */

extern int8_t iqueue_is_full(InstrQueue* queue);                    /* Returns a one if the queue is full */

#endif
