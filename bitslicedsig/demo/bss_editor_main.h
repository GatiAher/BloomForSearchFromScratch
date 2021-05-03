/**
 * Create or modify a bit-sliced signature
 * 
 * Load a bit-sliced signature from a file or create a new one. Add new documents to it and save it.
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

#define OPTSTR "vf:i:o:m:k:d:h"
#define USAGE_FMT "%s [-v] [-f file_load_bitslicedsig_from] [-i file_read_input_from] [-o file_save_bitslicedsig_to] [-m number_of_bits] [-k number_of_hash_functions] [-d minimum_bound_on_document_capacity] [-h]\n \
\tDefault: create or load bitslicedsig, read list of documents to add either from stdin or from file, save bitslicedsig to `bss_saved.txt`, m = 60, k = 3 d = 40 \n"
#define ERR_FOPEN_READ_INPUT_FROM "Error while opening the file: fopen(file_read_input_from, r)"
#define ERR_RUN_BSS "bss_editor failed"
#define DEFAULT_PROGNAME "bss_editor"

typedef struct
{
    int verbose;
    FILE *fread_input_from;
    char *loc_load_bss;
    char *loc_save_bss;
    uint32_t m;
    uint32_t k;
    uint32_t d;
} options_t;

#include "bss_editor.h"

/* print program details */
void usage(char *progname);

#endif