/**
 * Interactive Bloom filter.
 * 
 * Load a Bloom filter and use it to evaluate a query.
 * 
 * Author: Gati Aher
 */

#include "bf_play.h"

/* for coloring text */
#define PRINTC(c, f, s) printf("\033[%dm" f "\033[0m", 30 + c, s)

int bf_play(options_t *options)
{
    if (!options)
    {
        errno = EINVAL;
        return EXIT_FAILURE;
    }

    if (!options->loc_load_bloom)
    {
        errno = ENOENT;
        return EXIT_FAILURE;
    }

    /* load Bloom filter */
    bloom_t *filter = bloom_load(options->loc_load_bloom);

    if (options->verbose)
        bloom_print(filter);

    /* read each term in input */
    const uint32_t bufferLength = 1023; // assumes no term exceeds length of 1023
    char buffer[bufferLength];
    char *token;
    char *rest;

    bool isIn;
    while (fgets(buffer, bufferLength, options->fread_input_from))
    {
        rest = buffer;
        while ((token = strtok_r(rest, " !\"#$%%&()*+,-./:;<=>?@[\\]^_`{|}~", &rest)))
        {
            if (token[strlen(token) - 1] == '\n')
                token[strlen(token) - 1] = '\0';
            isIn = bloom_lookup(filter, token);
            if (isIn)
            {
                if (options->mode == (M_DEF | M_IN))
                    PRINTC(4, "%s ", token);
                else if (options->mode == (M_DEF))
                    printf("%s ", token);
                else if (options->mode == (M_SEL | M_IN))
                    printf("%s\n", token);
            }
            else
            {
                if (options->mode == (M_DEF))
                    PRINTC(1, "%s ", token);
                else if (options->mode == (M_DEF | M_IN))
                    printf("%s ", token);
                else if (options->mode == (M_SEL))
                    printf("%s\n", token);
            }
        }
        printf("\n");
    }

    return EXIT_SUCCESS;
}