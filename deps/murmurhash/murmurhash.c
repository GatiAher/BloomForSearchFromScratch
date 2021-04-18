
/**
 * `murmurhash.h' - murmurhash
 *
 * copyright (c) 2014 joseph werle <joseph.werle@gmail.com>
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "murmurhash.h"

u_int32_t
murmurhash(const char *key, u_int32_t len, u_int32_t seed)
{
  u_int32_t c1 = 0xcc9e2d51;
  u_int32_t c2 = 0x1b873593;
  u_int32_t r1 = 15;
  u_int32_t r2 = 13;
  u_int32_t m = 5;
  u_int32_t n = 0xe6546b64;
  u_int32_t h = 0;
  u_int32_t k = 0;
  u_int8_t *d = (u_int8_t *)key; // 32 bit extract from `key'
  const u_int32_t *chunks = NULL;
  const u_int8_t *tail = NULL; // tail - last 8 bytes
  int i = 0;
  int l = len / 4; // chunk length

  h = seed;

  chunks = (const u_int32_t *)(d + l * 4); // body
  tail = (const u_int8_t *)(d + l * 4);    // last 8 byte chunk of `key'

  // for each 4 byte chunk of `key'
  for (i = -l; i != 0; ++i)
  {
    // next 4 byte chunk of `key'
    k = chunks[i];

    // encode next 4 byte chunk of `key'
    k *= c1;
    k = (k << r1) | (k >> (32 - r1));
    k *= c2;

    // append to hash
    h ^= k;
    h = (h << r2) | (h >> (32 - r2));
    h = h * m + n;
  }

  k = 0;

  // remainder
  switch (len & 3)
  { // `len % 4'
  case 3:
    k ^= (tail[2] << 16);
  case 2:
    k ^= (tail[1] << 8);

  case 1:
    k ^= tail[0];
    k *= c1;
    k = (k << r1) | (k >> (32 - r1));
    k *= c2;
    h ^= k;
  }

  h ^= len;

  h ^= (h >> 16);
  h *= 0x85ebca6b;
  h ^= (h >> 13);
  h *= 0xc2b2ae35;
  h ^= (h >> 16);

  return h;
}
