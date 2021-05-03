/**
 * Create or modify a Bloom filter
 * 
 * Load a Bloom filter from a file or create a new one. Add new terms to it and save it.
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

#define OPTSTR "vf:i:o:m:k:h"
#define USAGE_FMT "%s [-v] [-f file_load_bloom_from] [-i file_read_input_from] [-o file_save_bloom_to] [-m number_of_bits] [-k number_of_hash_functions] [-h]\n \
\tDefault: create new Bloom filter, read input from stdin, save Bloom filter to `bf_saved.txt`, m = 60, k = 3"
#define ERR_FOPEN_READ_INPUT_FROM "Error while opening the file: fopen(file_read_input_from, r)"
#define ERR_RUN_BLOOM "bf_editor failed"
#define DEFAULT_PROGNAME "bf_editor"

typedef struct
{
    int verbose;
    FILE *fread_input_from;
    char *loc_load_bloom;
    char *loc_save_bloom;
    uint32_t m;
    uint32_t k;
} options_t;

#include "bf_editor.h"

/* print program details */
void usage(char *progname);

#endif