#include <stdint.h>


/* S-box */
static const uint8_t S[16] = {
    0xC, 0x5, 0x6, 0xB,
    0x9, 0x0, 0xA, 0xD,
    0x3, 0xE, 0xF, 0x8,
    0x4, 0x7, 0x1, 0x2
};


/* ================= BIT UTILITIES ================= */

static uint64_t sbox_layer(uint64_t state) {
    uint64_t out = 0;
    for (int i = 0; i < 16; i++) {
        uint8_t nibble = (state >> (4 * i)) & 0xF;
        out |= ((uint64_t)S[nibble]) << (4 * i);
    }
    return out;
}

static uint64_t p_layer(uint64_t state) {
    uint64_t out = 0;
    for (int i = 0; i < 64; i++) {
        uint64_t bit = (state >> i) & 1;
        int pos = (i == 63) ? 63 : (16 * i) % 63;
        out |= bit << pos;
    }
    return out;
}

/* ================= KEY SCHEDULE ================= */

/* key is stored in two variables:
   k_high: top 64 bits
   k_low : bottom 16 bits */

static void present_key_update(uint64_t *k_high, uint16_t *k_low, uint8_t round) {
    /* Rotate left by 61 */
    uint64_t new_high = (*k_high << 61) | ((uint64_t)(*k_low) << 45) | (*k_high >> 3);
    uint16_t new_low  = (*k_high >> 19) & 0xFFFF;

    *k_high = new_high;
    *k_low  = new_low;

    /* Apply S-box to MS nibble */
    uint8_t ms_nibble = (*k_high >> 60) & 0xF;
    *k_high &= ~(0xFULL << 60);
    *k_high |= ((uint64_t)S[ms_nibble]) << 60;

    /* XOR round counter into bits 19..15 */
    *k_high ^= (uint64_t)(round & 0x1F) << 15;
}

/* ================= ENCRYPTION ================= */

uint64_t present_encrypt(uint64_t plaintext, uint64_t key_high, uint16_t key_low) {
    uint64_t state = plaintext;

    for (uint8_t round = 1; round <= 31; round++) {
        /* Add round key */
        state ^= key_high;

        /* S-box layer */
        state = sbox_layer(state);

        /* Permutation layer */
        state = p_layer(state);

        /* Key update */
        present_key_update(&key_high, &key_low, round);
    }

    /* Final round key */
    state ^= key_high;

    return state;
}
