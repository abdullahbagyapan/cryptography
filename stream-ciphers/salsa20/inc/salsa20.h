#ifndef SALSA20_H
#define SALSA20_H

#include <stdint.h>
#include <stddef.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Salsa20 stream cipher
 *
 * key   : 256-bit key  (32 bytes)
 * nonce : 64-bit nonce (8 bytes)
 *
 * Encrypts/decrypts `len` bytes:
 *   out[i] = in[i] XOR keystream[i]
 */
void salsa20_xor(uint8_t *out,
                 const uint8_t *in,
                 size_t len,
                 const uint8_t key[32],
                 const uint8_t nonce[8]);

/*
 * Salsa20 core block function (20 rounds)
 *
 * out = Salsa20(in)
 * Both arrays are 16 × uint32_t (64 bytes)
 */
void salsa20_block(uint32_t out[16],
                   const uint32_t in[16]);

#ifdef __cplusplus
}
#endif

#endif /* SALSA20_H */
