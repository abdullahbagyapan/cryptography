#include <stdint.h>

/* Any nonzero start state will work. */
static uint16_t state = 0xF00D;


uint16_t lfsr_fib(void)
{   

    uint16_t lfsr = state;

    /* taps: 16 14 13 11; feedback polynomial: x^16 + x^14 + x^13 + x^11 + 1 */
    uint16_t bit = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5)) & 1u;
    lfsr = (lfsr >> 1) | (bit << 15);

    state = lfsr;

    return lfsr;
}
