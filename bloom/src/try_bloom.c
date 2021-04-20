#include "try_bloom.h"

/* Run bloom filter */
int try_bloom(options_t *options)
{

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

    if (options->verbose)
        printf("\n ------ \n Create bloom filter with %d bits and %d hash functions \n", options->num_bits, options->num_hash);
    bloom_t *filter = bloom_create(options->num_bits, options->num_hash);

    /* assumes no word exceeds length of 1023 */
    const int bufferLength = 1023;
    char buffer[bufferLength];
    int count_TP = 0, count_TN = 0, count_FP = 0, count_FN = 0, add_warnings = 0;
    bool isIn;

    if (options->verbose)
        puts("\n ------ \n Adding words to bloom filter \n");
    while (fgets(buffer, bufferLength, options->add_to_bloom))
    {
        isIn = bloom_add_with_warning(filter, buffer);
        if (isIn)
        {
            add_warnings++;
            if (options->verbose)
                printf("WARNING: all bits have already been set for %s", buffer);
        }
        else
        {
            if (options->verbose)
                printf("added %s", buffer);
        }
    }

    if (options->verbose)
        puts("\n ------ \n Looking up words that are in bloom filter \n");
    rewind(options->add_to_bloom);
    while (fgets(buffer, bufferLength, options->add_to_bloom))
    {
        isIn = bloom_lookup(filter, buffer);
        if (isIn)
        {
            count_TP++;
            if (options->verbose)
                printf("might be in: %s", buffer);
        }
        else
        {
            count_FN++;
            if (options->verbose)
                printf("definitely not in %s", buffer);
        }
    }

    if (options->verbose)
        puts("\n ------ \n Looking up words that are not in bloom filter \n");
    while (fgets(buffer, bufferLength, options->check_in_bloom))
    {
        isIn = bloom_lookup(filter, buffer);
        if (isIn)
        {
            count_FP++;
            if (options->verbose)
                printf("might be in: %s", buffer);
        }
        else
        {
            count_TN++;
            if (options->verbose)
                printf("definitely not in %s", buffer);
        }
    }

    printf("\n ------ \n RESULTS: \n BAD_ADDs %d \n TP %d \n FP %d \n FN %d \n TN %d \n", add_warnings, count_TP, count_FP, count_FN, count_TN);

    bloom_free(filter);

    return EXIT_SUCCESS;
}