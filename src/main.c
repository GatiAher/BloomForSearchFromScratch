/**
 * Bloom Filter From Scratch
 * 
 * Author: Gati Aher
 */

/* includes */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libgen.h>
#include <errno.h>
#include <string.h>
#include <getopt.h>
#include <stdint.h>
#include "bloom.h"

/* defines */
#define OPTSTR "va:c:b:n:h"
#define USAGE_FMT "%s [-v] [-a file_words_to_add_to_bloom] [-c file_words_to_check_in_bloom] [-b number_of_bits] [-n number_of_hash_functions] [-h]\n"
/* XXX write meaningful error messages */
#define ERR_FOPEN_ADD_TO_BLOOM "Error while opening the file: fopen(file_words_to_add_to_bloom, r)"
#define ERR_FOPEN_CHECK_IN_BLOOM "Error while opening the file: fopen(file_words_to_check_in_bloom, r)"
#define ERR_NUM_BITS_NOT_POW_TWO "Error num_bits must be a power of two"
#define ERR_RUN_BLOOM "Run_bloom failed"
#define ERR_TEMPLATE "error template"
#define DEFAULT_PROGNAME "bloom"

/* external declarations */
extern int errno;
extern char *optarg;
extern int opterr, optind;

/* typedefs */

typedef struct
{
    int verbose;
    FILE *add_to_bloom;
    FILE *check_in_bloom;
    int num_bits;
    int num_hash;
} options_t;

/* global variable declarations */

/* function prototypes */
void usage(char *progname)
{
    fprintf(stderr, USAGE_FMT, progname ? progname : DEFAULT_PROGNAME);
    exit(EXIT_FAILURE);
    /* NOTREACHED */
}

/* Run bloom filter */
int run_bloom(options_t *options)
{

    if (!options)
    {
        errno = EINVAL;
        return EXIT_FAILURE;
    }

    if (!options->add_to_bloom || !options->check_in_bloom)
    {
        errno = ENOENT;
        return EXIT_FAILURE;
    }

    if (!options->num_bits || !options->num_hash)
    {
        errno = EINVAL;
        return EXIT_FAILURE;
    }

    if (options->verbose)
        printf("\n ------ \n Create bloom filter with %d bits and %d hash functions \n", options->num_bits, options->num_hash);
    bloom_t *filter = bloom_create(options->num_bits, options->num_hash);

    /* assumes no word exceeds length of 1023 */
    const int bufferLength = 1023;
    char buffer[bufferLength];
    int count_TP = 0, count_TN = 0, count_FP = 0, count_FN = 0, add_warnings = 0;
    bool isIn;

    if (options->verbose)
        puts("\n ------ \n Adding words to bloom filter \n");
    while (fgets(buffer, bufferLength, options->add_to_bloom))
    {
        isIn = bloom_add_with_warning(filter, buffer);
        if (isIn)
        {
            add_warnings++;
            if (options->verbose)
                printf("WARNING: all bits have already been set for %s", buffer);
        }
        else
        {
            if (options->verbose)
                printf("added %s", buffer);
        }
    }

    if (options->verbose)
        puts("\n ------ \n Looking up words that are in bloom filter \n");
    rewind(options->add_to_bloom);
    while (fgets(buffer, bufferLength, options->add_to_bloom))
    {
        isIn = bloom_lookup(filter, buffer);
        if (isIn)
        {
            count_TP++;
            if (options->verbose)
                printf("might be in: %s", buffer);
        }
        else
        {
            count_FN++;
            if (options->verbose)
                printf("definitely not in %s", buffer);
        }
    }

    if (options->verbose)
        puts("\n ------ \n Looking up words that are not in bloom filter \n");
    while (fgets(buffer, bufferLength, options->check_in_bloom))
    {
        isIn = bloom_lookup(filter, buffer);
        if (isIn)
        {
            count_FP++;
            if (options->verbose)
                printf("might be in: %s", buffer);
        }
        else
        {
            count_TN++;
            if (options->verbose)
                printf("definitely not in %s", buffer);
        }
    }

    printf("\n ------ \n RESULTS: BAD_ADDs %d | TP %d | FP %d | FN %d | TN %d \n", add_warnings, count_TP, count_FP, count_FN, count_TN);

    bloom_free(filter);

    return EXIT_SUCCESS;
}

int main(int argc, char *argv[])
{
    int opt;
    options_t options = {0, NULL, NULL, 0, 0};

    // Initialize opterr to 0 to disable getopt from emiting a ?.
    opterr = 0;

    while ((opt = getopt(argc, argv, OPTSTR)) != EOF)
        switch (opt)
        {
        case 'a':
            if (!(options.add_to_bloom = fopen(optarg, "r")))
            {
                perror(ERR_FOPEN_ADD_TO_BLOOM);
                exit(EXIT_FAILURE);
                /* NOTREACHED */
            }
            break;

        case 'c':
            if (!(options.check_in_bloom = fopen(optarg, "r")))
            {
                perror(ERR_FOPEN_CHECK_IN_BLOOM);
                exit(EXIT_FAILURE);
                /* NOTREACHED */
            }
            break;

        case 'b':
            options.num_bits = atoi(optarg);
            if (!options.num_bits | (options.num_bits & (options.num_bits - 1)))
            {
                errno = EINVAL;
                perror(ERR_NUM_BITS_NOT_POW_TWO);
                exit(EXIT_FAILURE);
                /* NOTREACHED */
            }
            break;

        case 'n':
            options.num_hash = atoi(optarg);
            break;

        case 'v':
            options.verbose += 1;
            break;

        case 'h':
        default:
            usage(basename(argv[0]));
            /* NOTREACHED */
            break;
        }

    
    if (run_bloom(&options) != EXIT_SUCCESS)
    {
        perror(ERR_RUN_BLOOM);
        exit(EXIT_FAILURE);
        /* NOTREACHED */
    }

    return EXIT_SUCCESS;
}
