/**
 * Test that Bloom filter implementation works as expected.
 * 
 * Author: Gati Aher
 * Date: April 30, 2021
 */

#include "test_bloom_main.h"

extern int errno;
extern char *optarg;
extern int opterr, optind;

int main(int argc, char *argv[])
{
    int opt;

    /* set default args */
    char *def_in_test = "bloom/test/in_test.txt";
    char *def_out_test = "bloom/test/out_test.txt";
    options_t options = {0, NULL, NULL, 60, 3};

    // Initialize opterr to 0 to disable getopt from emiting a ?.
    opterr = 0;

    while ((opt = getopt(argc, argv, OPTSTR)) != EOF)
        switch (opt)
        {
        case 'a':
            printf("fadd_to_bloom: %s\n", optarg);
            if (!(options.fadd_to_bloom = fopen(optarg, "r")))
            {
                perror(ERR_FOPEN_ADD_TO_BLOOM);
                exit(EXIT_FAILURE);
                /* NOTREACHED */
            }
            break;

        case 'c':
            printf("fcheck_in_bloom: %s\n", optarg);
            if (!(options.fcheck_in_bloom = fopen(optarg, "r")))
            {
                perror(ERR_FOPEN_CHECK_IN_BLOOM);
                exit(EXIT_FAILURE);
                /* NOTREACHED */
            }
            break;

        case 'm':
            options.m = atoi(optarg);
            break;

        case 'k':
            options.k = atoi(optarg);
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

    /* open default files */
    if (options.fadd_to_bloom == NULL)
    {
        printf("fadd_to_bloom: %s\n", def_in_test);
        if (!(options.fadd_to_bloom = fopen(def_in_test, "r")))
        {
            perror(ERR_FOPEN_ADD_TO_BLOOM);
            exit(EXIT_FAILURE);
            /* NOTREACHED */
        }
    }
    if (options.fcheck_in_bloom == NULL)
    {
        printf("fcheck_in_bloom: %s\n", def_out_test);
        if (!(options.fcheck_in_bloom = fopen(def_out_test, "r")))
        {
            perror(ERR_FOPEN_CHECK_IN_BLOOM);
            exit(EXIT_FAILURE);
            /* NOTREACHED */
        }
    }

    if (test_bloom(&options) != EXIT_SUCCESS)
    {
        perror(ERR_RUN_BLOOM);
        exit(EXIT_FAILURE);
        /* NOTREACHED */
    }

    fclose(options.fadd_to_bloom);
    fclose(options.fcheck_in_bloom);

    return EXIT_SUCCESS;
}

void usage(char *progname)
{
    fprintf(stderr, USAGE_FMT, progname ? progname : DEFAULT_PROGNAME);
    exit(EXIT_FAILURE);
    /* NOTREACHED */
}
