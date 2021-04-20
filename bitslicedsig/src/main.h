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
#define OPTSTR "va:c:b:n:h"
#define USAGE_FMT "%s [-v] [-a file_words_to_add_to_bloom] [-c file_words_to_check_in_bloom] [-b number_of_bits] [-n number_of_hash_functions] [-h]\n"
#define ERR_FOPEN_ADD_TO_BLOOM "Error while opening the file: fopen(file_words_to_add_to_bloom, r)"
#define ERR_FOPEN_CHECK_IN_BLOOM "Error while opening the file: fopen(file_words_to_check_in_bloom, r)"
#define ERR_NUM_SIG_BITS_NOT_POW_TWO "Error num_sig_bits must be a power of two"
#define ERR_RUN_BLOOM "Run_bloom failed"
#define ERR_TEMPLATE "error template"
#define DEFAULT_PROGNAME "bloom"

/* typedefs */
typedef struct
{
    int verbose;
    FILE *add_to_bloom;
    FILE *check_in_bloom;
    int num_sig_bits;
    int num_hash;
} options_t;

#include "try_bitslicedsig.h"

/* function prototypes */
void usage(char *progname);

#endif