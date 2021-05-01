/**
 * Interactive Bloom Filter.
 * 
 * Author: Gati Aher
 * Date: April 30, 2021
 */

#include "ibf_main.h"

extern int errno;
extern char *optarg;
extern int opterr, optind;

int main(int argc, char *argv[])
{
    int opt;
    options_t options = {0, "save_bloom_spelling_words.txt", 0, 0};

    // Initialize opterr to 0 to disable getopt from emiting a ?.
    opterr = 0;

    while ((opt = getopt(argc, argv, OPTSTR)) != EOF)
        switch (opt)
        {
        case 's':
            options.source = optarg;
            break;

        case 'f':
            options.filter_mode = 1;
            break;

        case 'i':
            options.in_mode = 1;
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

    if (process_input(&options) != EXIT_SUCCESS)
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
