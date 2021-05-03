#ifndef BLOOM_H
#define BLOOM_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>

#include "../../deps/murmurhash/murmurhash.h"

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

/** Creates a new bit-sliced block signature matrix
 * 
 * Creates a m*n bit matrix where m is number of signature bits and n is number of documents.
 * 
 * A bit matrix created with large number of signature bits or large number of hash functions
 * has fewer collisions (fewer false positives). However, large number of signature bits
 * takes up space, and the greater number of hash functions leads to slower insertion and 
 * lookup performance time. 
 * 
 * m: number of bits to be used by each document signatures
 * k: number of hash functions
 * min_doc_capacity: bit matrix's lower bound on document storing capacity
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
 * query: file pointer with string to check for in bit-sliced signature
 * 
 * Returns list of document indicies that match the query
 */
void bitslicedsig_query(bitslicedsig_t *bitslicedsig, FILE *fquery);

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
void bitslicesig_print(bitslicedsig_t *bitslicedsig);

#endif