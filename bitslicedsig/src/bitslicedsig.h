#ifndef BLOOM_H
#define BLOOM_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>

#include "../../deps/murmurhash/murmurhash.h"

typedef struct
{
    int num_blocks;
    int num_sig_bits;
    u_int32_t **bit_matrix;
    int num_hash_func;
    u_int32_t *hash_seeds;
} bitslicedsig_t;

/** Creates a new bit-sliced signature matrix
 * 
 * Creates a m*n bit matrix where m is number of signature bits and n is number of documents.
 * 
 * A bit matrix created with large number of signature bits or large number of hash functions
 * has fewer collisions (fewer false positives). However, large number of signature bits
 * takes up space, and the greater number of hash functions leads to slower insertion and 
 * lookup performance time. 
 * 
 * max_doc_capacity: maximum number of documents to be stored in table
 * num_sig_bits: number of bits to be used by each document signatures
 * num_hash_funcs: number of hash functions
 */
bitslicedsig_t *bitslicedsig_create(int max_doc_capacity, int num_sig_bits, int num_hash_funcs);

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
void bitslicedsig_add_doc(bitslicedsig_t *bitslicedsig, int index, char *filename);

/** Queries if an item is in the bit-sliced signature
 * 
 * bitslicedsig: bit-sliced signature
 * query: file pointer with string to check for in bit-sliced signature
 * 
 * Returns list of document indicies that match the query
 */
void bitslicedsig_query(bitslicedsig_t *bitslicedsig, FILE *fquery);

/* Print bitslicesig bit matrix */
void bitslicesig_print(bitslicedsig_t *bitslicedsig);

#endif