#ifndef BLOOM_H
#define BLOOM_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>

#include "../deps/murmurhash/murmurhash.h"

typedef struct
{
    int num_bits;
    u_int8_t *array;
    int num_hash_funcs;
    u_int32_t *hash_seeds;
} bloom_t;

/** Creates a new bloom filter
 * 
 * filter created with large size or large number of hash functions
 * has fewer collisions (fewer false positives), but takes up more space,
 * and greater number of hash functions leads to slower insertion and 
 * lookup performance time. 
 * 
 * num_bits: number of bits allocated for bit array, should be a power of 2
 * num_hash_funcs: number of hash functions
 */
bloom_t *bloom_create(int num_bits, int num_hash_funcs);

/** Frees a bloom filter 
 * 
 * filter: bloom filter
 */
void bloom_free(bloom_t *filter);

/** Adds an item to the bloom filter
 * 
 * filter: bloom filter
 * item: string to add to bloom filter
 */
void bloom_add(bloom_t *filter, const char *item);

/** Tests if an item is in the bloom filter
 * 
 * filter: bloom filter
 * item: string to check for in bloom filter
 * 
 * Returns false if the item has definitely not been added to the bloom filter
 * Returns true if the item might have been added to the bloom filter
 */
bool bloom_lookup(bloom_t *filter, const char *item);

/** Adds an item to the bloom filter
 * 
 * filter: bloom filter
 * item: string to add to bloom filter
 * 
 * Returns true if all the hash bits for item have been previously set.
 * Otherwise returns false.
 */
bool bloom_add_with_warning(bloom_t *filter, const char *item);

#endif