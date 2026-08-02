#include <stdio.h>
#include <string.h>
#include "cpu_internal.h"
#include "decoder.h"
#include "register.h"

#define CASES_SIZE 11

const char* passedEmoji = "\xE2\x9c\x85";
const char* failedEmoji = "\xE2\x9D\x8C";

int testing_eu_register_ax_lower(void);
int testing_eu_register_ax_uppper(void);
int testing_eu_register_ax(void);
int testing_eu_register_bx_lower(void);
int testing_eu_register_bx_uppper(void);
int testing_eu_register_bx(void);
int testing_eu_register_cx_setting_lower(void);
int testing_eu_register_cx_setting_upper(void);
int testing_eu_register_cx(void);
int testing_eu_decoder(void);
int testing_mod_splicer(void);

static int (*cases[CASES_SIZE])(void) = {
    testing_eu_register_ax_lower,
    testing_eu_register_ax_uppper,
    testing_eu_register_ax,
    testing_eu_register_bx_lower,
    testing_eu_register_bx_uppper,
    testing_eu_register_bx,
    testing_eu_register_cx_setting_lower,
    testing_eu_register_cx_setting_upper,
    testing_eu_register_cx,
    testing_eu_decoder,
    testing_mod_splicer,
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


int testing_eu_register_ax_lower(void) {
    uint8_t result;
    int passed = 1;
    int expected = 0x64;
    EU* eu = cpu_eu_init();

    cpu_eu_set_data_reg(eu, REG_AX, 0xA864, REGISTER_WORD_ACCESS);

    result = cpu_eu_get_data_reg(eu, REG_AL, REGISTER_WORD_ACCESS);

    fprintf(stdout, "TESTING AX REGISTER LOWER Modes ");

    if (eu == NULL || result != expected) {
        passed = 0; 
    }

    print_emoji_message(passed, expected, result);
    cpu_eu_free(eu);
    return passed;
}


int testing_eu_register_ax_uppper(void) {
    uint8_t result;
    int passed = 1;
    int expected = 0xA8;
    EU* eu = cpu_eu_init();

    cpu_eu_set_data_reg(eu, REG_AX, 0xA864, REGISTER_WORD_ACCESS);

    result =  cpu_eu_get_data_reg(eu, REG_AH, REGISTER_BYTE_ACCESS);

    fprintf(stdout, "TESTING AX REGISTER UPPER Modes ");

    if (eu == NULL || result != expected) {
        passed = 0; 
    }

    print_emoji_message(passed, expected, result);
    cpu_eu_free(eu);
    return passed;
}

int testing_eu_register_ax(void) {
    uint16_t result;
    int passed = 1;
    int expected = 0xA864;
    EU* eu = cpu_eu_init();

    cpu_eu_set_data_reg(eu, REG_AX, 0xA864, REGISTER_WORD_ACCESS);

    result = cpu_eu_get_data_reg(eu, REG_AX, REGISTER_WORD_ACCESS);

    fprintf(stdout, "TESTING AX REGISTER ");

    if (eu == NULL || result != expected) {
        passed = 0; 
    }

    print_emoji_message(passed, expected, result);
    cpu_eu_free(eu);
    return passed;
}

int testing_eu_register_bx_lower(void) {
    uint8_t result;
    int passed = 1;
    int expected = 0xF2;
    EU* eu = cpu_eu_init();

    cpu_eu_set_data_reg(eu, REG_BX, 0xB9F2, REGISTER_WORD_ACCESS);

    result = cpu_eu_get_data_reg(eu, REG_BL, REGISTER_WORD_ACCESS);

    fprintf(stdout, "TESTING AX REGISTER LOWER Modes ");

    if (eu == NULL || result != expected) {
        passed = 0; 
    }

    print_emoji_message(passed, expected, result);
    cpu_eu_free(eu);
    return passed;
}


int testing_eu_register_bx_uppper(void) {
    uint8_t result;
    int passed = 1;
    int expected = 0xB9;
    EU* eu = cpu_eu_init();
    cpu_eu_set_data_reg(eu, REG_BX, 0xB9F2, REGISTER_WORD_ACCESS);

    result = cpu_eu_get_data_reg(eu, REG_BH, REGISTER_BYTE_ACCESS);

    fprintf(stdout, "TESTING BX REGISTER UPPER Modes ");

    if (eu == NULL || result != expected) {
        passed = 0; 
    }

    print_emoji_message(passed, expected, result);
    cpu_eu_free(eu);
    return passed;
}

int testing_eu_register_bx(void) {
    uint16_t result;
    int passed = 1;
    int expected = 0xB9F2;
    EU* eu = cpu_eu_init(); 

    cpu_eu_set_data_reg(eu, REG_BX, 0xB9F2, REGISTER_WORD_ACCESS);

    result = cpu_eu_get_data_reg(eu, REG_BX, REGISTER_WORD_ACCESS);

    fprintf(stdout, "TESTING BX REGISTER ");

    if (eu == NULL || result != expected) {
        passed = 0; 
    }

    print_emoji_message(passed, expected, result);
    cpu_eu_free(eu);
    return passed;
}

int testing_eu_register_cx_setting_lower(void) {
    uint8_t result;
    int passed = 1;
    int expected = 0x9F;
    EU* eu = cpu_eu_init(); 

    cpu_eu_set_data_reg(eu, REG_CL, 0x9F, REGISTER_BYTE_ACCESS);

    result = cpu_eu_get_data_reg(eu, REG_CX, REGISTER_WORD_ACCESS);

    fprintf(stdout, "TESTING CX REGISTER SETTING LOWER ");

    if (eu == NULL || result != expected) {
        passed = 0; 
    }

    print_emoji_message(passed, expected, result);
    cpu_eu_free(eu);
    return passed;
}

int testing_eu_register_cx_setting_upper(void) {
    uint16_t result;
    int passed = 1;
    int expected = 0x5A00;
    EU* eu = cpu_eu_init(); 

    cpu_eu_set_data_reg(eu, REG_CH, 0x5A, REGISTER_BYTE_ACCESS);

    result = cpu_eu_get_data_reg(eu, REG_CX, REGISTER_WORD_ACCESS);

    fprintf(stdout, "TESTING CX REGISTER SETTING UPPER ");

    if (eu == NULL || result != expected) {
        passed = 0; 
    }

    print_emoji_message(passed, expected, result);
    cpu_eu_free(eu);
    return passed;
}

int testing_eu_register_cx(void) {
    uint16_t result;
    int passed = 1;
    int expected = 0x5A9F;
    EU* eu = cpu_eu_init(); 

    cpu_eu_set_data_reg(eu, REG_CL, 0x9F, REGISTER_BYTE_ACCESS);
    cpu_eu_set_data_reg(eu, REG_CH, 0x5A, REGISTER_BYTE_ACCESS);

    result = cpu_eu_get_data_reg(eu, REG_CX, REGISTER_WORD_ACCESS);

    fprintf(stdout, "TESTING CX REGISTER ");

    if (eu == NULL || result != expected) {
        passed = 0; 
    }

    print_emoji_message(passed, expected, result);
    cpu_eu_free(eu);
    return passed;
}

int testing_eu_decoder(void) {
    char* result;
    int passed = 1;
    char* expected = "add";
    EU* eu = cpu_eu_init(); 

    result = (char*)cpu_eu_decode_opcode(0x0000)->mnemonic;

    fprintf(stdout, "TESTING EU DECODER ");

    if (eu == NULL || strcmp(result, expected)) {
        passed = 0; 
    }

    print_emoji_message(passed, 0, 0);

    cpu_eu_free(eu);
    return passed;
}

int testing_mod_splicer(void) {
    struct ModRM result;
    int passed = 1;
    struct ModRM expected = {.mode = ADDRESSING_RM, .reg = 0 , .regMem = 4};
    EU* eu = cpu_eu_init(); 
    result = cpu_eu_decode_modrm(0xC4);

    fprintf(stdout, "TESTING MOD SPLICER ");

    if (eu == NULL || memcmp(&result, &expected, sizeof(result))){
        passed = 0; 
    }

    print_emoji_message(passed, 0, 0);
    cpu_eu_free(eu);
    return passed;
}


