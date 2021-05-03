/**
 * Interactive bit-sliced signature.
 * 
 * Load a bit-sliced signature and use it to evaluate a query.
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

#define OPTSTR "vf:i:s:h"
#define USAGE_FMT "%s [-v] [-f file_load_bitslicedsig_from] [-i file_read_input_from] [-s output_format_string] [-h]\n \
\tDefault: load bit-sliced signature from `bss_saved.txt`, read query from stdin or file, output matching documents with format specified by output_format_string, default: Doc %%d"
#define ERR_FOPEN_READ_INPUT_FROM "Error while opening the file: fopen(file_read_input_from, r)"
#define ERR_RUN_BSS "bss_play failed"
#define DEFAULT_PROGNAME "bss_play"

typedef struct
{
    int verbose;
    FILE *fread_input_from;
    char *loc_load_bss;
    char *output_format_string;
} options_t;

#include "bss_play.h"

/* print program details */
void usage(char *progname);

#endif