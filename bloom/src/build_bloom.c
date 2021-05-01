/**
 * Create and save a Bloom Filter.
 * 
 * Author: Gati Aher
 * Date: April 30, 2021
 */

#include "build_bloom.h"

int run_build_bloom(options_t *options)
{
    if (!options)
    {
        errno = EINVAL;
        return EXIT_FAILURE;
    }

    if (!options->add_to_bloom || !options->save_bloom)
    {
        errno = ENOENT;
        return EXIT_FAILURE;
    }

    if (!options->num_bits || !options->num_hash)
    {
        errno = EINVAL;
        return EXIT_FAILURE;
    }

    /* create Bloom filter */
    bloom_t *filter = bloom_create(options->num_bits, options->num_hash);
    if (options->verbose)
        bloom_print(filter);

    /* add terms to Bloom filter */
    const uint32_t bufferLength = 1023; // assumes no term exceeds length of 1023
    char buffer[bufferLength];
    char *token;
    char *rest;

    FILE *f_add = fopen(options->add_to_bloom, "r");
    if (!f_add)
    {
        perror("could not open file of words to add to Bloom\n");
        exit(EXIT_FAILURE);
        /* NOTREACHED */
    }

    while (fgets(buffer, bufferLength, f_add))
    {
        rest = buffer;
        while ((token = strtok_r(rest, " .,?", &rest)))
        {
            if (token[strlen(token)-1] == '\n')
                token[strlen(token)-1] = '\0';
            bloom_add(filter, token);
        }
    }
    fclose(f_add);

    if (options->verbose)
        bloom_print(filter);

    /* save Bloom filter */
    uint32_t save_status = bloom_save(filter, options->save_bloom);
    if (save_status == 0)
        printf("Sucessfully saved Bloom filter to %s!\n", options->save_bloom);
    else
        perror("Error saving Bloom filter!\n");

    bloom_free(filter);

    return EXIT_SUCCESS;
}