#include <stdio.h>
#include <stdlib.h>
#include "bitset.h"

#define CASES_SIZE 6

const char* passedEmoji = "\xE2\x9c\x85";
const char* failedEmoji = "\xE2\x9D\x8C";

int test_initialization_and_bytes_size(void);
int test_bytes_size_to_zero(void);
int test_setting_bits_and_getting_set_bit(void);
int test_setting_first_and_last_bit(void);
int test_setting_bits_and_check_int_val(void);
int test_bitset_to_int_array(void);

static int (*cases[CASES_SIZE])(void) = {
     test_initialization_and_bytes_size,
     test_bytes_size_to_zero,
     test_setting_bits_and_getting_set_bit,
     test_setting_first_and_last_bit,
     test_setting_bits_and_check_int_val,
     test_bitset_to_int_array,
};


static void print_emoji_message(int passed, int expected, int result) {
    const char* messageEmoji;
    messageEmoji = passed ? passedEmoji : failedEmoji; 
    fprintf(stdout, "%s\n", messageEmoji);
    if (passed == 0) {
        fprintf(stdout, "Expected Value %d\n", expected);
        fprintf(stdout, "Resultant Value %d\n", result);
    }
}


int main (void) {
    int passed = 0;
    for (int i = 0; i < CASES_SIZE; i++) {
        fprintf(stdout, "Running Test Case %d...\n", i + 1);
        passed += cases[i]();
    }
    fprintf(stdout, "Test Cases: %d/%d passed\n", passed, CASES_SIZE);
    return 0;
}

int test_initialization_and_bytes_size(void) {
    int result;
    int passed = 1;
    int expected = 31;
    BitSet* set = bitset_init_bset(4);

    fprintf(stdout, "TESTING INITIALIZATION PROCESS AND BYTE SIZE ");
    result = bitset_get_capacity_bits(set); 

    if (set == NULL || result != expected) {
        passed = 0; 
    }
    print_emoji_message(passed, expected, result);

    bitset_free_bset(set);
    return passed;
}

int test_bytes_size_to_zero(void) {
    int expected = 7;
    int passed = 1;
    int result;
    BitSet* set = bitset_init_bset(0);

    fprintf(stdout, "TESTING SETTING BYTES SIZE TO ZERO ");
    result = bitset_get_capacity_bits(set);
    if (set == NULL || result != expected) {
        passed = 0;    
    }
    print_emoji_message(passed, expected, result);
    bitset_free_bset(set);
    return passed;
}

int test_setting_bits_and_getting_set_bit(void) {
    int expected = 8;
    int passed = 1;
    int result;
    BitSet* set = bitset_init_bset(1);
    
    bitset_set_all_bit(set);
    result = bitset_get_size_bits(set);

    fprintf(stdout, "TESTING SETTING ALL BITS AND GETTING SIZE ");
    if (set == NULL || result != expected) {
        passed = 0;    
    }
    print_emoji_message(passed, expected, result);
    bitset_free_bset(set);
    return passed;
}

int test_setting_first_and_last_bit(void) {
    int expected = 2;
    int passed = 1;
    int result;
    BitSet* set = bitset_init_bset(1);

    bitset_set_bit(set, 7);
    bitset_set_bit(set, 0);

    result = bitset_get_size_bits(set);

    fprintf(stdout, "TESTING SETTING FIRST AND LAST BIT ");
    if (set == NULL || result != expected) {
        passed = 0;    
    }
    print_emoji_message(passed, expected, result);
    bitset_free_bset(set);
    return passed;
}


int test_setting_bits_and_check_int_val(void) {
    int expected = 85;
    int passed = 1;
    int result;
    BitSet* set = bitset_init_bset(1);

    bitset_set_bit(set, 0);
    bitset_set_bit(set, 2);
    bitset_set_bit(set, 4);
    bitset_set_bit(set, 6);

    uint8_t size;
    uint8_t* arr = bitset_to_int_array(set, &size);
    result = *arr;

    if (size != 1 || *arr != result) {
        passed = 0;
    }

    fprintf(stdout, "TESTING BITS AND TESTING RETURN VALUE ");
    print_emoji_message(passed, expected, result);

    free(arr);
    bitset_free_bset(set);
    return passed;
}

int test_bitset_to_int_array(void) {
    int expected = 128;
    int passed = 1;
    int result;
    BitSet* set = bitset_init_bset(3);

    bitset_set_bit(set, 7);
    bitset_set_bit(set, 15);
    bitset_set_bit(set, 23);

    
    uint8_t size;
    uint8_t* arr = bitset_to_int_array(set, &size);

    if (size != 3) {
        passed = 0;
    }

    for (uint8_t i = 0; i < size; i++) {
        result = arr[i];
        if (result != expected) {
            passed = 0;
            break;
        }
    }

    fprintf(stdout, "TESTING BITSET TO INT ARRAY ");
    print_emoji_message(passed, expected, result);

    free(arr);
    bitset_free_bset(set);
    return passed;

}



