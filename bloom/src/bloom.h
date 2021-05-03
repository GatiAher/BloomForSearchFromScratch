/** Implementation of Bloom filter for 32-bit system.
 * 
 * Author: Gati Aher
*/

#ifndef BLOOM_H
#define BLOOM_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>

#include "../../deps/murmurhash/murmurhash.h"

#define ERR_FOPEN_LOAD_BLOOM_FROM "Error while opening the file: fopen(file_load_bloom_from, r)"
#define ERR_FOPEN_SAVE_BLOOM_TO "Error while opening the file: fopen(file_save_bloom_to, w)"

typedef struct
{
    u_int32_t m;
    u_int32_t *array;
    u_int32_t k;
    u_int32_t *hash_seeds;
    u_int32_t n;
} bloom_t;

/** Creates a new Bloom filter
 * 
 * The number of signature bits (m) and number of hash functions (k) control the Bloom filter false positive rate. 
 * 
 * The value of m configures how many bits will be allocated for the bit array. 
 * Ideally, when all the items are added, the bit array should be approximately half full.
 * Worst case is if the bit array is completely full (all 1s), as it will say that every query may be in the filter (false positives).
 * For performance reasons, the bit array rounds m to the nearest largest power of 2.
 * 
 * The value of k configures how my hash functions will be used in the Bloom filter hashing scheme.
 * A higher number of hash functions means that there is a higher signal to noise ratio,
 * where signal is the probability that if the term is a member of set given that all the term's probes are present in the Bloom filter,
 * and noise is the probability of a false positive. Rarer terms have a higher likelihood of being a false positive, so increasing k
 * improves the signal to noise ratio and allows for better retrival of rate terms. However, a larger k leads more hash operations, and
 * therefore slower insertion and lookup time. The seeds of the hash function are generated with a random number generator.
 * 
 * m: (integer) number of bits allocated for bit array
 * k: (integer) number of hash functions
 */
bloom_t *bloom_create(u_int32_t m, u_int32_t num_hash_funcs);

/** Frees a Bloom filter 
 * 
 * filter: Bloom filter
 */
void bloom_free(bloom_t *filter);

/** Add an item to the Bloom filter
 * 
 * Adds item to Bloom filter and increments n.
 * 
 * filter: Bloom filter
 * item: string to add to Bloom filter
 */
void bloom_add(bloom_t *filter, const char *item);

/** Check if an item is in the Bloom filter
 * 
 * filter: Bloom filter
 * item: string to check for in Bloom filter
 * 
 * Returns false if the item has definitely not been added to the Bloom filter
 * Returns true if the item might have been added to the Bloom filter
 */
bool bloom_lookup(bloom_t *filter, const char *item);

/** Add an item to the Bloom filter
 * 
 * filter: Bloom filter
 * item: string to add to Bloom filter
 * 
 * Returns true if all the hash bits for item have been previously set.
 * Otherwise returns false.
 */
bool bloom_add_with_warning(bloom_t *filter, const char *item);

/** Save a Bloom filter to file
 * 
 * filter: Bloom filter
 * filename: valid filename 
 * 
 * Returns status code 0 if successful, 1 if unsuccessful
 */
u_int32_t bloom_save(bloom_t *filter, const char *filename);

/** Load a Bloom filter from file
 * 
 * filename: valid filename 
 * Returns a Bloom filter saved by bloom_save
 */
bloom_t *bloom_load(const char *filename);

/** Print a Bloom filter
 * 
 * filter: Bloom filter
 */
void bloom_print(bloom_t *filter);

#endif