/**
 * Interactive Bloom Filter.
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

#define OPTSTR "vs:fi"
#define USAGE_FMT "%s [-v] [-s (optional) file_source_bloom_from] [-f filter_mode] [-i in_mode] [-h]"
#define ERR_FOPEN_ADD_TO_BLOOM "Error while opening the file: fopen(file_words_to_add_to_bloom, r)"
#define ERR_FOPEN_SAVE_BLOOM_TO "Error while opening the file: fopen(file_save_bloom_to, r)"
#define ERR_RUN_BLOOM "process_input failed"
#define DEFAULT_PROGNAME "build_bloom"

typedef struct
{
    int verbose;
    char *source;
    int filter_mode;
    int in_mode;
} options_t;

#include "ibf.h"

/* print program details */
void usage(char *progname);

#endif