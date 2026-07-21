#include "bitset.h"
#include "svec.h"

#include <stdint.h>
#include <assert.h>
#include <string.h>


struct bitset {
    uint8_t* set;
    uint16_t capacity;
};

BitSet* bitset_init_bset(uint8_t bytesSize) {
    BitSet* bSet = xmalloc(sizeof(*bSet));
   
    if (bytesSize == 0) {
        bytesSize = 1;
    }
    bSet->set = xmalloc(sizeof(*bSet->set) * bytesSize);
    memset(bSet->set, 0, sizeof(*bSet->set) * bytesSize);

    bSet->capacity = ((sizeof(*bSet->set) * bytesSize) * 8 ) - 1;

    return bSet;
}

void bitset_free_bset(BitSet* bSet) {
    if (bSet == NULL) {
        return;
    }
    free(bSet->set);
    free(bSet);
}

int16_t bitset_get_capacity_bits(BitSet* bSet) {
    assert(bSet != NULL);
    return bSet->capacity;
}

int8_t bitset_get_capacity_bytes(BitSet* bSet) {
    assert(bSet != NULL);
    return (bSet->capacity + 1) / 8;
}

int16_t bitset_get_size_bits(BitSet* bSet) {
    assert(bSet != NULL);
    int16_t resulted = 0;

    for (uint32_t i = 0; i < bSet->capacity + 1; i++) {
        resulted += (*bSet->set >> i) & 1;
    }
    return resulted;
}

void bitset_set_bit_pos(BitSet* bSet, uint32_t pos) {
    assert(bSet != NULL);
    if (pos > bSet->capacity) {
        return; 
    }
    int idx = pos / 8;
    int x = pos % 8;

    bSet->set[idx] |= (1 << x);
}

void bitset_set_all_bit(BitSet* bSet) {
    assert(bSet != NULL);
    uint32_t byteSize = bitset_get_capacity_bytes(bSet);
    for (uint32_t i = 0; i < byteSize; i++) {
        bSet->set[i] = 255;
    }
}

void bitset_reset_bit(BitSet* bSet, uint32_t pos) {
    assert(bSet != NULL);
    if (pos > bSet->capacity) {
        return; 
    }

    int idx = pos / 8;
    int x = pos % 8;
    bSet->set[idx] &= (0 << x);
}

void bitset_reset_all_bit(BitSet* bSet) {
    assert(bSet != NULL);
    uint32_t byteSize = bitset_get_capacity_bytes(bSet);
    for (uint32_t i = 0; i < byteSize; i++) {
        bSet->set[i] = 0;
    }
}


int8_t bitset_get_bit(BitSet* bSet, uint32_t pos) {
    assert(bSet != NULL);
    if (pos > bSet->capacity) {
        return 0; 
    }
    int idx = pos / 8;
    int x = pos % 8;

    return (bSet->set[idx] >> x) & 1;
}

uint8_t* bitset_to_int_array(BitSet* bSet, uint8_t* retSize) {
    assert(bSet != NULL);
    uint8_t bytesSize = (bSet->capacity + 1) / 8;
    uint8_t* retArr = xmalloc(bytesSize);

    memcpy(retArr, bSet->set, bytesSize);
    *retSize = bytesSize;
    return retArr;
    
}


