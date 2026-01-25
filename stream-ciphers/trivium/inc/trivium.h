#ifndef TRIVIUM_H
#define TRIVIUM_H


#include <stdint.h>
#include <string.h>


/* Trivium parameters */
#define     POSITION_OF_IV  93

#define     SIZE_OF_STREAM  288


void trivium_initialize(const uint8_t key[10], const uint8_t iv[10]);

uint8_t trivium_rotate(void);

void trivium_keystream(uint8_t *out, size_t len);

void trivium_encrypt(const uint8_t *in, uint8_t *out, size_t len);

#endif /* TRIVIUM_H */
