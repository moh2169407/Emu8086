#include <stdint.h>
#include <stdio.h>
#include "alu.h"

#define CASES_SIZE 1

const char* passedEmoji = "\xE2\x9c\x85";
const char* failedEmoji = "\xE2\x9D\x8C";


int testing_alu_add(void);

static int (*cases[CASES_SIZE])(void) = {
    testing_alu_add,
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


int main(void) {
    int passed = 0;
    for (int i = 0; i < CASES_SIZE; i++) {
        fprintf(stdout, "Running Test Case %d...\n", i + 1);
        passed += cases[i]();
    }
    fprintf(stdout, "Test Cases: %d/%d passed\n", passed, CASES_SIZE);
    return 0;
}

int testing_alu_add(void) {
    uint16_t result;
    int passed = 1;
    int expected = 5456;

    Alu* alu = alu_init();
    
    uint16_t input = 2319;
    uint16_t input2 = 3137;
    AluIn in = {.operator = ALU_ADD, .temp1 = &input, .temp2 = &input2};
    
    result = alu[in.operator](in).val;

    fprintf(stdout, "TESTING ALU ADD OPERATIONS ");

    if (alu == NULL || result != expected) {
        passed = 0; 
    }

    print_emoji_message(passed, expected, result);
    alu_free(alu);
    return passed;
}
