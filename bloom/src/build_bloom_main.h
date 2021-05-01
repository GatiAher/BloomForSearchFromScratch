/**
 * Create and save a Bloom Filter.
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

#define OPTSTR "va:o:m:k:h"
#define USAGE_FMT "%s [-v] [-a (optional) file_words_to_add_to_bloom] [-o file_save_bloom_to] [-m number_of_bits] [-k number_of_hash_functions] [-h]\n"
#define ERR_FOPEN_ADD_TO_BLOOM "Error while opening the file: fopen(file_words_to_add_to_bloom, r)"
#define ERR_FOPEN_SAVE_BLOOM_TO "Error while opening the file: fopen(file_save_bloom_to, r)"
#define ERR_RUN_BLOOM "run_build_bloom failed"
#define DEFAULT_PROGNAME "build_bloom"

typedef struct
{
    int verbose;
    char *add_to_bloom;
    char *save_bloom;
    uint32_t num_bits;
    uint32_t num_hash;
} options_t;

#include "build_bloom.h"

/* print program details */
void usage(char *progname);

#endif