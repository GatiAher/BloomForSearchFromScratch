/**
 * Interactive bit-sliced signature.
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

#define OPTSTR "vf:i:h"
#define USAGE_FMT "%s [-v] [-f file_load_bitslicedsig_from] [-i file_read_input_from] [-h]\n \
\tDefault: load bit-sliced signature from `bss_saved.txt`, read input from stdin, m = 60, k = 3, -s OFF (print all text), -x OFF (color out-of-set terms red)"
#define ERR_FOPEN_READ_INPUT_FROM "Error while opening the file: fopen(file_read_input_from, r)"
#define ERR_RUN_BSS "bss_play failed"
#define DEFAULT_PROGNAME "bss_play"

typedef struct
{
    int verbose;
    FILE *fread_input_from;
    char *loc_load_bss;
} options_t;

#include "bss_play.h"

/* print program details */
void usage(char *progname);

#endif