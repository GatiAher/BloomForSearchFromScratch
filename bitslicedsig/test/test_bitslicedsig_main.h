/**
 * Test that Bit-Sliced Block Signature implementation works as expected.
 * 
 * Author: Gati Aher
 * Date: April 30, 2021
 */

#ifndef MAIN_H
#define MAIN_H

/* includes */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libgen.h>
#include <errno.h>
#include <string.h>
#include <getopt.h>
#include <stdint.h>

/* defines */
#define OPTSTR "va:c:m:k:d"
#define USAGE_FMT "%s [-v] [-a prefix_path_of_corpus_to_add] [-c file_query_to_check] [-m number_of_bits] [-k number_of_hash_functions] [-d number_of_documents] [-h]\n \
\tDefault: add docs from `bitslicedsig/test/corpus/doc*.txt`, check query from `bitslicedsig/test/query.txt`, m = 60, k = 3, d = 3"
#define ERR_FOPEN_ADD_TO_BITSLICEDSIG "Error while opening the file: fopen(prefix_path_of_corpus_to_add/file, r)"
#define ERR_FOPEN_CHECK_IN_BITSLICEDSIG "Error while opening the file: fopen(file_query_to_check, r)"
#define ERR_RUN_BITSLICEDSIG "test_bitslicedsig failed"
#define DEFAULT_PROGNAME "test_bitslicedsig"

/* typedefs */
typedef struct
{
    int verbose;
    char *prefix_path_of_corpus_to_add;
    FILE *fquery;
    u_int32_t m;
    u_int32_t k;
    u_int32_t d;
} options_t;

#include "test_bitslicedsig.h"

/* function prototypes */
void usage(char *progname);

#endif