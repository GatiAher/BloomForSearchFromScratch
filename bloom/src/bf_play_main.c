/**
 * Interactive Bloom Filter.
 * 
 * Author: Gati Aher
 * Date: April 30, 2021
 */

#include "bf_play_main.h"

extern int errno;
extern char *optarg;
extern int opterr, optind;

int main(int argc, char *argv[])
{
    int opt;
    options_t options = {0, stdin, "bf_saved.txt", 0, 0};

    // Initialize opterr to 0 to disable getopt from emiting a ?.
    opterr = 0;

    while ((opt = getopt(argc, argv, OPTSTR)) != EOF)
        switch (opt)
        {

        case 'i':
        {
            if (!(options.fread_input_from = fopen(optarg, "r")))
            {
                perror(ERR_FOPEN_READ_INPUT_FROM);
                exit(EXIT_FAILURE);
                /* NOTREACHED */
            }
            dup2(fileno(options.fread_input_from), STDIN_FILENO);
            break;
        }

        case 'f':
            options.loc_load_bloom = optarg;
            break;

        case 's':
            options.mode_display_selected = 1;
            break;

        case 'x':
            options.mode_select_in = 1;
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

    if (bf_play(&options) != EXIT_SUCCESS)
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
