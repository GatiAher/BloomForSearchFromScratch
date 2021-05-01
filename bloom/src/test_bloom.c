/**
 * Test that Bloom filter implementation works as expected.
 * 
 * Author: Gati Aher
 * Date: April 30, 2021
 */

#include "test_bloom.h"

void test_bloom_lookup(options_t *options, bloom_t *filter)
{
    // assumes no word exceeds length of 1023 */
    const int bufferLength = 1023;
    char buffer[bufferLength];

    int count_TP = 0, count_TN = 0, count_FP = 0, count_FN = 0;
    bool isIn;

    /* Check Positive Words */
    if (options->verbose)
        puts("\nLook up words that are really not in Bloom filter\n");
    rewind(options->add_to_bloom);
    while (fgets(buffer, bufferLength, options->add_to_bloom))
    {
        isIn = bloom_lookup(filter, buffer);
        if (isIn)
        {
            count_TP++;
            if (options->verbose)
                printf(">> might be in: %s", buffer);
        }
        else
        {
            count_FN++;
            if (options->verbose)
                printf("!! definitely not in: %s", buffer);
        }
    }

    /* Check Negative Words */
    if (options->verbose)
        puts("\n\nLook up words that are really in Bloom filter\n");
    rewind(options->check_in_bloom);
    while (fgets(buffer, bufferLength, options->check_in_bloom))
    {
        isIn = bloom_lookup(filter, buffer);
        if (isIn)
        {
            count_FP++;
            if (options->verbose)
                printf("!! might be in: %s", buffer);
        }
        else
        {
            count_TN++;
            if (options->verbose)
                printf(">> definitely not in: %s", buffer);
        }
    }

    /* Summary */
    printf("\n**results of lookup test: TP %d | FP %d | FN %d | TN %d \n", count_TP, count_FP, count_FN, count_TN);
}

int test_bloom(options_t *options)
{
    uint32_t add_warnings = 0;
    bool isIn;

    if (!options)
    {
        errno = EINVAL;
        return EXIT_FAILURE;
    }

    if (!options->add_to_bloom || !options->check_in_bloom)
    {
        errno = ENOENT;
        return EXIT_FAILURE;
    }

    if (!options->num_bits || !options->num_hash)
    {
        errno = EINVAL;
        return EXIT_FAILURE;
    }

    printf("\n---\nCreate Bloom filter with %d bits and %d hash functions\n", options->num_bits, options->num_hash);
    bloom_t *filter = bloom_create(options->num_bits, options->num_hash);
    if (options->verbose)
        bloom_print(filter);

    printf("\n---\nAdd words to Bloom filter\n");
    const uint32_t bufferLength = 1023; // assumes no term exceeds length of 1023
    char buffer[bufferLength];
    while (fgets(buffer, bufferLength, options->add_to_bloom))
    {
        isIn = bloom_add_with_warning(filter, buffer);
        if (isIn)
        {
            add_warnings++;
            if (options->verbose)
                printf("!! WARNING: all bits have already been set for %s", buffer);
        }
        else
        {
            if (options->verbose)
                printf(">> added %s", buffer);
        }
    }

    printf("\n*number of add warnings: %d\n", add_warnings);
    if (options->verbose)
        bloom_print(filter);

    test_bloom_lookup(options, filter);

    if (options->verbose)
        printf("\n---\nSave Bloom filter");
    char test_save_bloom[] = "test_save_bloom.txt";
    uint32_t save_status = bloom_save(filter, test_save_bloom);
    if (save_status == 0)
        printf("\nSucessfully saved Bloom filter to %s!", test_save_bloom);
    else
        printf("\nError saving Bloom filter to %s!", test_save_bloom);

    if (options->verbose)
        printf("\n---\nFree Bloom filter");
    bloom_free(filter);

    printf("\n---\nLoad Bloom Filter from %s\n", test_save_bloom);
    bloom_t *load_filter = bloom_load(test_save_bloom);
    bloom_print(load_filter);
    test_bloom_lookup(options, load_filter);

    if (options->verbose)
        printf("\n---\nFree loaded Bloom filter\n");
    bloom_free(load_filter);

    return EXIT_SUCCESS;
}