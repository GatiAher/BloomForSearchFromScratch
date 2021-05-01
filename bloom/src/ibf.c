/**
 * Interactive Bloom Filter.
 * 
 * Author: Gati Aher
 * Date: April 30, 2021
 */

#include "ibf.h"

int process_input(options_t *options)
{
    if (!options)
    {
        errno = EINVAL;
        return EXIT_FAILURE;
    }

    if (!options->source)
    {
        errno = ENOENT;
        return EXIT_FAILURE;
    }

    /* load Bloom filter */
    bloom_t *filter = bloom_load(options->source);

    if (options->verbose)
        bloom_print(filter);

    /* read each term in input */
    const uint32_t bufferLength = 1023; // assumes no term exceeds length of 1023
    char buffer[bufferLength];
    char *token;
    char *rest;
    bool isIn;

    while (fgets(buffer, bufferLength, stdin))
    {
        rest = buffer;
        while ((token = strtok_r(rest, " .,?", &rest)))
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
    }

    return EXIT_SUCCESS;
}