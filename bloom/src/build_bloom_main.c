/**
 * Create and save a Bloom Filter.
 * 
 * Author: Gati Aher
 * Date: April 30, 2021
 */

#include "build_bloom_main.h"

extern int errno;
extern char *optarg;
extern int opterr, optind;

int main(int argc, char *argv[])
{
    int opt;
    options_t options = {0, "/usr/share/dict/words", "save_bloom_spelling_words.txt", 1472280, 10};

    // Initialize opterr to 0 to disable getopt from emiting a ?.
    opterr = 0;

    while ((opt = getopt(argc, argv, OPTSTR)) != EOF)
        switch (opt)
        {
        case 'a':
            options.add_to_bloom = optarg;
            break;

        case 'o':
            options.save_bloom = optarg;
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

    if (run_build_bloom(&options) != EXIT_SUCCESS)
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
