#include "chacha20.h"


#define ROTL(a,b) \
    (((a) << (b)) | ((a) >> (32 - (b))))


#define QR(a, b, c, d) \
	( ((a += b), (d ^= a), d = ROTL(d, 16)), \
      ((c += d), (b ^= c), b = ROTL(b, 12)), \
      ((a += b), (d ^= a), d = ROTL(d,  8)), \
      ((c += d), (b ^= c), b = ROTL(b,  7)))


#define ROUNDS 20


static inline uint32_t CONCAT32_LE(const uint8_t *p)
{
    return ((uint32_t)p[0])       |
           ((uint32_t)p[1] <<  8) |
           ((uint32_t)p[2] << 16) |
           ((uint32_t)p[3] << 24);
}


static inline void STORE32_LE(uint8_t *p, uint32_t v)
{
    p[0] = v;
    p[1] = v >> 8;
    p[2] = v >> 16;
    p[3] = v >> 24;
}


void salsa20_xor(uint8_t *out,
                 const uint8_t *in,
                 size_t len,
                 const uint8_t key[32],
                 const uint8_t nonce[8])
{
    static const uint8_t sigma[16] = "expand 32-byte k";

    uint32_t state[16], block[16];
    uint8_t keystream[64];
    uint64_t counter = 0;

    // Constant
    state[ 0] = CONCAT32_LE(sigma + 0);
    state[ 1] = CONCAT32_LE(sigma + 4);
    state[ 2] = CONCAT32_LE(sigma + 8);
    state[ 3] = CONCAT32_LE(sigma +12);

    // Key
    for (int i = 0; i < 8; i++) {
        state[4 + i] = CONCAT32_LE(key + 4*i);
    }

    // Nonce
    state[14] = CONCAT32_LE(nonce + 0);
    state[15] = CONCAT32_LE(nonce + 4);

    while (len) {
        state[12] = (uint32_t)counter;
        state[13] = (uint32_t)(counter >> 32);

        chacha20_block(block, state);

        for (int i = 0; i < 16; i++)
            STORE32_LE(keystream + 4*i, block[i]);

        size_t n = len < 64 ? len : 64;
        for (size_t i = 0; i < n; i++)
            out[i] = in[i] ^ keystream[i];

        out += n;
        in  += n;
        len -= n;
        counter++;
    }
}


void chacha20_block(uint32_t out[16], const uint32_t in[16])
{
    int i;
	uint32_t x[16];

    memcpy(x, in, sizeof(x));

	// 10 loops × 2 rounds/loop = 20 rounds
	for (i = 0; i < ROUNDS; i += 2) {
		
        // Odd round
		QR(x[ 0], x[ 4], x[ 8], x[12]); // column 1
		QR(x[ 1], x[ 5], x[ 9], x[13]); // column 2
		QR(x[ 2], x[ 6], x[10], x[14]); // column 3
		QR(x[ 3], x[ 7], x[11], x[15]); // column 4
		
        // Even round
		QR(x[ 0], x[ 5], x[10], x[15]); // diagonal 1 (main diagonal)
		QR(x[ 1], x[ 6], x[11], x[12]); // diagonal 2
		QR(x[ 2], x[ 7], x[ 8], x[13]); // diagonal 3
		QR(x[ 3], x[ 4], x[ 9], x[14]); // diagonal 4
	}

	for (i = 0; i < 16; ++i) {
		out[i] = x[i] + in[i]; 
    }

    return;
}