/**
 * Create and save a Bloom Filter.
 * 
 * Author: Gati Aher
 * Date: April 30, 2021
 */

#include "run_build_bloom.h"

void print_bloom(bloom_t *filter)
{
    printf("\n---------------------------------\n");
    printf("Bloom filter\n");
    printf("\n m = %d, k = %d", filter->m, filter->k);
    printf("\n hash_seeds = ");
    uint32_t i;
    for (i = 0; i < filter->k; i++)
        printf("%d ", filter->hash_seeds[i]);
    printf("\n array = ");
    for (i = 0; i < 10; i++)
        printf("%x ", filter->array[i]);
    printf("... [output truncated]");
    printf("\n---------------------------------\n");
}

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
        print_bloom(filter);

    /* add terms to Bloom filter */
    const uint32_t bufferLength = 1023; // assumes no term exceeds length of 1023
    char buffer[bufferLength];
    FILE *f_add = fopen(options->add_to_bloom, "r");
    if (!f_add)
    {
        perror("could not open file of words to add to Bloom\n");
        exit(EXIT_FAILURE);
        /* NOTREACHED */
    }
    while (fgets(buffer, bufferLength, f_add))
    {
        bloom_add(filter, buffer);
    }
    fclose(f_add);

    if (options->verbose)
        print_bloom(filter);

    /* save Bloom filter */
    uint32_t save_status = bloom_save(filter, options->save_bloom);
    if (save_status == 0)
        printf("Sucessfully saved Bloom filter to %s!\n", options->save_bloom);
    else
        perror("Error saving Bloom filter!\n");

    bloom_free(filter);

    return EXIT_SUCCESS;
}