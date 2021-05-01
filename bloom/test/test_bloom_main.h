/**
 * Test that Bloom filter implementation works as expected.
 * 
 * Author: Gati Aher
 * Date: April 30, 2021
 */

#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libgen.h>
#include <errno.h>
#include <string.h>
#include <getopt.h>
#include <stdint.h>

#define OPTSTR "va:c:m:k:h"
#define USAGE_FMT "%s [-v] [-a file_words_to_add_to_bloom] [-c file_words_to_check_in_bloom] [-m number_of_bits] [-k number_of_hash_functions] [-h]\n \
\tDefault: add words from `bloom/test/in_test.txt`, check words from `bloom/test/out_test.txt`, m = 60, k = 3"
#define ERR_FOPEN_ADD_TO_BLOOM "Error while opening the file: fopen(file_words_to_add_to_bloom, r)"
#define ERR_FOPEN_CHECK_IN_BLOOM "Error while opening the file: fopen(file_words_to_check_in_bloom, r)"
#define ERR_RUN_BLOOM "test_bloom failed"
#define DEFAULT_PROGNAME "test_bloom"

typedef struct
{
    int verbose;
    FILE *fadd_to_bloom;
    FILE *fcheck_in_bloom;
    uint32_t m;
    uint32_t k;
} options_t;

#include "test_bloom.h"

/* print program details */
void usage(char *progname);

#endif