/**
 * Create or modify a Bloom filter
 * 
 * Load a Bloom filter from a file or create a new one. Add new terms to it and save it.
 * 
 * Author: Gati Aher
 * Date: April 30, 2021
 */

#include "bf_editor.h"

int bf_editor(options_t *options)
{
    if (!options)
    {
        errno = EINVAL;
        return EXIT_FAILURE;
    }

    if (!options->loc_save_bloom)
    {
        errno = ENOENT;
        return EXIT_FAILURE;
    }

    if (!options->loc_load_bloom && (!options->m || !options->k))
    {
        errno = EINVAL;
        return EXIT_FAILURE;
    }

    bloom_t *filter;

    if (options->loc_load_bloom)
    {
        /* load Bloom filter */
        filter = bloom_load(options->loc_load_bloom);
    }
    else
    {
        /* create Bloom filter */
        filter = bloom_create(options->m, options->k);
    }

    if (options->verbose)
        bloom_print(filter);

    /* add terms to Bloom filter */
    const uint32_t bufferLength = 1023; // assumes no term exceeds length of 1023
    char buffer[bufferLength];
    char *token;
    char *rest;

    while (fgets(buffer, bufferLength, options->fread_input_from))
    {
        rest = buffer;
        while ((token = strtok_r(rest, " .,?", &rest)))
        {
            if (token[strlen(token) - 1] == '\n')
                token[strlen(token) - 1] = '\0';
            bloom_add(filter, token);
        }
    }

    if (options->verbose)
        bloom_print(filter);

    /* save Bloom filter */
    uint32_t save_status = bloom_save(filter, options->loc_save_bloom);
    if (save_status == 0)
        printf("Sucessfully saved Bloom filter to %s!\n", options->loc_save_bloom);
    else
        perror("Error saving Bloom filter!\n");

    bloom_free(filter);

    return EXIT_SUCCESS;
}