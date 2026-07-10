#include <stdio.h>

union example {
    int16_t full;
    int8_t half[1];
};

int main(void) {
    union example e;
    e.full = 100;
    printf("Full value: %d\n", e.full);
    printf("Half Index 1 value: %hhd\n", e.half[1]);
    printf("Half Index 2 value: %hhd\n", e.half[0]);
    return 0;
}

