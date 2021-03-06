/**
 * Interactive Bloom filter.
 * 
 * Load a Bloom filter and use it to evaluate a query.
 * 
 * Author: Gati Aher
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

#define OPTSTR "vf:i:sxh"
#define USAGE_FMT "%s [-v] [-f file_load_bloom_from] [-i file_read_input_from] [-s mode_display_selected] [-x mode_select_in] [-h]\n \
\tDefault: load Bloom filter from `bf_saved.txt`, read input from stdin, m = 60, k = 3, -s OFF (print all text), -x OFF (color out-of-set terms red) \n"
#define ERR_FOPEN_READ_INPUT_FROM "Error while opening the file: fopen(file_read_input_from, r)"
#define ERR_RUN_BLOOM "bf_play failed"
#define DEFAULT_PROGNAME "bf_play"

enum modes
{
    M_DEF = 0,
    M_SEL = 1,
    M_IN = 2
};

typedef struct
{
    int verbose;
    FILE *fread_input_from;
    char *loc_load_bloom;
    int mode;
} options_t;

#include "bf_play.h"

/* print program details */
void usage(char *progname);

#endif