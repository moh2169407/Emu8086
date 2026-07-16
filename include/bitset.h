#ifndef BITSET_H
#define BITSET_H

#include <stdint.h>

typedef struct bitset BitSet;

extern BitSet* bitset_init_bset(uint8_t bytesSize);     /* Initialize internal bitset with size bytesSize, range {1, 255} 0 default to 1 */

extern void bitset_free_bset(BitSet* bset);             /* Free underlining memory */

extern int16_t bitset_get_capacity_bits(BitSet* bset);  /* Returns the size of internal bit */

extern int8_t bitset_get_capacity_bytes(BitSet* bset); /* Returns the size of internal bytes */

extern int16_t bitset_get_size_bits(BitSet* bset);      /* Returns the amount of set bit */

extern void bitset_set_bit_pos(BitSet* bSet, uint32_t pos); /* Sets a bit at a certain position, quietly exit on error */

extern void bitset_set_all_bit(BitSet* bSet);   /* Sets all the bits in the bitset */

extern void bitset_reset_bit_pos(BitSet* bSet, uint32_t pos);   /* Sets a bit at pos to zero */

extern void bitset_reset_all_bit(BitSet* bSet);     /* Set all bits to zero */

extern int8_t bitset_get_bit_pos(BitSet* bSet, uint32_t pos);   /* Return whether a bit is set or 0, return -1 on error */

extern uint8_t* bitset_to_int_array(BitSet* bSet, uint8_t* retSize);    /* Return a array of integers that user need to free */
                                                                        /* retSize is value is set to the returning array size */

#endif
