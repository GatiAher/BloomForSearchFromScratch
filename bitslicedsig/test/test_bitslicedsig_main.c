/**
 * Test that Bit-Sliced Block Signature implementation works as expected.
 * 
 * Author: Gati Aher
 * Date: April 30, 2021
 */

/* includes */
#include "test_bitslicedsig_main.h"

/* external declarations */
extern int errno;
extern char *optarg;
extern int opterr, optind;

int main(int argc, char *argv[])
{
    int opt;
    char *def_prefix_path_to_corpus = "bitslicedsig/test/corpus/doc";
    char *def_path_to_query = "bitslicedsig/test/query.txt";
    options_t options = {0, def_prefix_path_to_corpus, NULL, 60, 3, 3};

    // Initialize opterr to 0 to disable getopt from emiting a ?.
    opterr = 0;

    while ((opt = getopt(argc, argv, OPTSTR)) != EOF)
        switch (opt)
        {
        case 'a':
            options.prefix_path_of_corpus_to_add = optarg;
            break;

        case 'c':
        {
            if (!(options.fquery = fopen(optarg, "r")))
            {
                perror(ERR_FOPEN_CHECK_IN_BITSLICEDSIG);
                exit(EXIT_FAILURE);
                /* NOTREACHED */
            }
            break;
        }

        case 'm':
            options.m = atoi(optarg);
            break;

        case 'k':
            options.k = atoi(optarg);
            break;

        case 'd':
            options.d = atoi(optarg);
            break;

        case 'v':
            options.verbose = 1;
            break;

        case 'h':
        default:
            usage(basename(argv[0]));
            /* NOTREACHED */
            break;
        }

    /* default query file */
    if (options.fquery == NULL)
    {
        if (!(options.fquery = fopen(def_path_to_query, "r")))
        {
            perror(ERR_FOPEN_CHECK_IN_BITSLICEDSIG);
            exit(EXIT_FAILURE);
            /* NOTREACHED */
        }
    }

    if (options.verbose)
        printf("Program: %s\n", DEFAULT_PROGNAME);

    if (test_bitslicedsig(&options) != EXIT_SUCCESS)
    {
        perror(ERR_RUN_BITSLICEDSIG);
        exit(EXIT_FAILURE);
        /* NOTREACHED */
    }

    fclose(options.fquery);

    return EXIT_SUCCESS;
}

void usage(char *progname)
{
    fprintf(stderr, USAGE_FMT, progname ? progname : DEFAULT_PROGNAME);
    exit(EXIT_FAILURE);
    /* NOTREACHED */
}
