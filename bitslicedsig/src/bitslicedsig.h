/** 
 * Implement a bit-sliced signature to perform keyword search for finding matching documents in a corpus 
 * 
 * Author: Gati Aher
 * */

#ifndef BITSLICEDSIG_H
#define BITSLICEDSIG_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>

#include "../../deps/murmurhash/murmurhash.h"
#include "queryres.h"

#define ERR_FOPEN_LOAD_BITSLICEDSIG_FROM "Error while opening the file: fopen(file_load_bitslicedsig_from, r)"
#define ERR_FOPEN_SAVE_BITSLICEDSIG_TO "Error while opening the file: fopen(file_save_bitslicesig_bloom_to, r)"

typedef struct
{
    u_int32_t m;
    u_int32_t **bit_matrix;
    u_int32_t k;
    u_int32_t *hash_seeds;
    u_int32_t num_blocks;
    u_int32_t added_d;
} bitslicedsig_t;

/** Creates a new bit-sliced signature matrix
 * 
 * Creates a m*n bit matrix where m is number of signature bits and d is number of documents.
 * The documents are stored in a bit-sliced configuration, so that each document is represented by a column of bits,
 * so each machine word (32 bits) stores the presence of a hash in 32 documents.
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
 * m: number of bits to be used by each document signatures
 * k: number of hash functions
 * min_doc_capacity: bit matrix's lower bound on document storing capacity.
 */
bitslicedsig_t *bitslicedsig_create(u_int32_t m, u_int32_t k, u_int32_t min_doc_capacity);

/** Frees a bit-sliced signature 
 * 
 * bitslicedsig: bit-sliced signature
 */
void bitslicedsig_free(bitslicedsig_t *bitslicedsig);

/** Adds a document's signature to bit-sliced signature matrix
 * 
 * bitslicedsig: bit-sliced signature
 * index: index associated with document
 * filename: path to file with document
 */
void bitslicedsig_add_doc(bitslicedsig_t *bitslicedsig, u_int32_t index, char *filename);

/** Queries if an item is in the bit-sliced signature
 * 
 * bitslicedsig: bit-sliced signature
 * fquery: file pointer with string to check for in bit-sliced signature
 * 
 * Returns query result as pointer to linked list of document indicies that match the query
 */
queryres_t *bitslicedsig_query(bitslicedsig_t *bitslicedsig, FILE *fquery);

/** Save a bitslicedsig: bit-sliced signature to file
 * 
 * bitslicedsig: bit-sliced signature
 * filename: valid filename 
 * 
 * Returns status code 0 if successful, 1 if unsuccessful
 */
u_int32_t bitslicedsig_save(bitslicedsig_t *filter, const char *filename);

/** Load a Bloom filter from file
 * 
 * bitslicedsig: bit-sliced signature
 * Returns a bit-sliced signature saved by bitslicedsig_save
 */
bitslicedsig_t *bitslicedsig_load(const char *filename);

/* Print bitslicesig bit matrix */
void bitslicedsig_print(bitslicedsig_t *bitslicedsig);

#endif