/**
 * Interactive Bloom Filter.
 * 
 * Author: Gati Aher
 * Date: April 30, 2021
 */

#include "bf_play.h"

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
                printf("in-%s ", token);
            }
            else
            {
                printf("out-%s ", token);
            }
        }
        printf("\n");
    }

    return EXIT_SUCCESS;
}