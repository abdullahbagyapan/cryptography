#ifndef CHACHA20_H
#define CHACHA20_H

#include <stdint.h>
#include <stddef.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * ChaCha20 stream cipher
 *
 * key   : 256-bit key  (32 bytes)
 * nonce : 64-bit nonce (8 bytes)
 *
 * Encrypts/decrypts `len` bytes:
 *   out[i] = in[i] XOR keystream[i]
 */
void chacha20_xor(uint8_t *out,
                 const uint8_t *in,
                 size_t len,
                 const uint8_t key[32],
                 const uint8_t nonce[8]);

/*
 * ChaCha20 core block function (20 rounds)
 *
 * out = ChaCha20(in)
 * Both arrays are 16 × uint32_t (64 bytes)
 */
void chacha20_block(uint32_t out[16],
                   const uint32_t in[16]);

#ifdef __cplusplus
}
#endif

#endif /* CHACHA20_H */
