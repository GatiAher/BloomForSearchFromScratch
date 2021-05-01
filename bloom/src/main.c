/**
 * Bloom Filter From Scratch
 * 
 * Author: Gati Aher
 */

/* includes */
#include "main.h"
#include "test_bloom.h"

/* external declarations */
extern int errno;
extern char *optarg;
extern int opterr, optind;

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

        case 'm':
            options.num_bits = atoi(optarg);
            break;

        case 'k':
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

    if (test_bloom(&options) != EXIT_SUCCESS)
    {
        perror(ERR_RUN_BLOOM);
        exit(EXIT_FAILURE);
        /* NOTREACHED */
    }

    return EXIT_SUCCESS;
}

void usage(char *progname)
{
    fprintf(stderr, USAGE_FMT, progname ? progname : DEFAULT_PROGNAME);
    exit(EXIT_FAILURE);
    /* NOTREACHED */
}
