#include "trivium.h"


static uint8_t stream[SIZE_OF_STREAM];


void trivium_initialize(const uint8_t key[10], const uint8_t iv[10])
{
    uint16_t i;

    /* Clear state */
    memset(stream, 0x00, sizeof(stream));

    /* Load key into s[0..79] */
    for (int i = 0; i < 80; i++) {
        stream[i] = (key[i / 8] >> (i % 8)) & 1;
    }
    
    /* Load IV into s[93..172] */
    for (int i = 0; i < 80; i++) {
        stream[POSITION_OF_IV + i] = (iv[i / 8] >> (i % 8)) & 1;
    }

    /* Set s[285..287] = 111 */
    stream[285] = 0x01;
    stream[286] = 0x01;
    stream[287] = 0x01;

    
    /* Warm-up Phase: 1152 clocks */
    for(i = 0; i < 4 * SIZE_OF_STREAM; i++) {
        trivium_rotate();
    }

    return;
}

uint8_t trivium_rotate()
{

    uint8_t t1, t2, t3;
    uint8_t z;

    t1 = stream[65] ^ stream[92];
    t2 = stream[161] ^ stream[176];
    t3 = stream[242] ^ stream[287];

    z = (t1 ^ t2 ^ t3);

    t1 ^= (stream[90] & stream[91]) ^ stream[170];
    t2 ^= (stream[174] & stream[175]) ^ stream[263];
    t3 ^= (stream[285] & stream[286]) ^ stream[68];

    // shift stream to right
    uint16_t i;
    for(i = SIZE_OF_STREAM - 1; i > 0; i--) {
        stream[i] = stream[i - 1];
    }

    stream[177] = t2;
    stream[93] = t1;
    stream[0] = t3;

    return z;
}

void trivium_keystream(uint8_t *out, size_t len)
{
    for (size_t i = 0; i < len; i++) {
        uint8_t byte = 0;
        for (int b = 0; b < 8; b++)
            byte |= trivium_rotate() << b;
        out[i] = byte;
    }

    return;
}

void trivium_encrypt(const uint8_t *in, uint8_t *out, size_t len)
{
    uint8_t keystream;

    for (size_t i = 0; i < len; i++) {
        trivium_keystream(&keystream, sizeof(keystream));
        out[i] = in[i] ^ keystream;
    }

    return;
}