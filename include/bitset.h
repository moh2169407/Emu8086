#ifndef BITSET_H
#define BITSET_H

#include <stdint.h>

typedef struct bitset BitSet;

extern BitSet* bitset_init_bset(int byteSize);

extern void bitset_free_bset(BitSet* bset);

extern int8_t bitset_get_count(BitSet* bset);

extern int8_t bitset_get_size(BitSet* bset);

extern void bitset_set_bit(uint32_t pos);

extern void bitset_set_all_bit(void);

extern void bitset_reset_bit(int pos);

extern void bitset_reset_all_bit(int pos);

#endif
