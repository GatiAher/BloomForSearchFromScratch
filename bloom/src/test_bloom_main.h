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
#define USAGE_FMT "%s [-v] [-a file_words_to_add_to_bloom] [-c file_words_to_check_in_bloom] [-m number_of_bits] [-k number_of_hash_functions] [-h]\n"
#define ERR_FOPEN_ADD_TO_BLOOM "Error while opening the file: fopen(file_words_to_add_to_bloom, r)"
#define ERR_FOPEN_CHECK_IN_BLOOM "Error while opening the file: fopen(file_words_to_check_in_bloom, r)"
#define ERR_RUN_BLOOM "test_bloom failed"
#define DEFAULT_PROGNAME "bloom"

typedef struct
{
    int verbose;
    FILE *add_to_bloom;
    FILE *check_in_bloom;
    uint32_t num_bits;
    uint32_t num_hash;
} options_t;

#include "test_bloom.h"

/* print program details */
void usage(char *progname);

#endif