
/**
 * `murmurhash.h' - murmurhash
 *
 * copyright (c) 2014 joseph werle <joseph.werle@gmail.com>
 */

#ifndef MURMURHASH_H
#define MURMURHASH_H 1

#define MURMURHASH_VERSION "0.0.3"

/**
 * Returns a murmur hash of `key' based on `seed'
 * using the MurmurHash3 algorithm
 */

u_int32_t
murmurhash (const char *, u_int32_t, u_int32_t);

#endif
