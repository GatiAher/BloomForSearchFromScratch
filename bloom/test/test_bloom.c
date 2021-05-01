/**
 * Test that Bloom filter implementation works as expected.
 * 
 * Author: Gati Aher
 * Date: April 30, 2021
 */

#include "test_bloom.h"

void process_stream(test_results_t *test_res, bloom_t *filter, void (*operate)(test_results_t *, bloom_t *, char *), FILE *stream)
{
    const uint32_t bufferLength = 1023; // assumes no term exceeds length of 1023
    char buffer[bufferLength];
    char *token;
    char *rest;

    /* check that stream is not a NULL pointer */
    if (!stream)
    {
        perror("Error: could not open file\n");
        exit(EXIT_FAILURE);
        /* NOTREACHED */
    }
    rewind(stream);

    while (fgets(buffer, bufferLength, stream))
    {
        rest = buffer;
        while ((token = strtok_r(rest, " .,?", &rest)))
        {
            if (token[strlen(token) - 1] == '\n')
                token[strlen(token) - 1] = '\0';
            operate(test_res, filter, token);
        }
    }
}

void operate_test_add_with_warning(test_results_t *test_res, bloom_t *filter, char *token)
{
    if (test_res->verbose)
        printf("test: bloom_add_with_warning\n");
    bool isIn;
    isIn = bloom_add_with_warning(filter, token);
    if (isIn)
    {
        test_res->count_add_warnings++;
        if (test_res->verbose)
            printf("!! WARNING: all bits have already been set for %s\n", token);
    }
    else
    {
        if (test_res->verbose)
            printf(">> added %s\n", token);
    }
}

void operate_test_lookup_real_positives(test_results_t *test_res, bloom_t *filter, char *token)
{
    if (test_res->verbose)
        printf("test: bloom_lookup, real positives\n");
    bool isIn;
    isIn = bloom_lookup(filter, token);
    if (isIn)
    {
        test_res->count_TP++;
        if (test_res->verbose)
            printf(">> TP: %s\n", token);
    }
    else
    {
        test_res->count_FN++;
        if (test_res->verbose)
            printf("!! FN: %s\n", token);
    }
}

void operate_test_lookup_real_negatives(test_results_t *test_res, bloom_t *filter, char *token)
{
    if (test_res->verbose)
        printf("test: bloom_lookup, real negatives\n");
    bool isIn;
    isIn = bloom_lookup(filter, token);
    if (isIn)
    {
        test_res->count_FP++;
        if (test_res->verbose)
            printf("!! FP: %s\n", token);
    }
    else
    {
        test_res->count_TN++;
        if (test_res->verbose)
            printf(">> TN: %s\n", token);
    }
}

int test_bloom(options_t *options)
{
    if (!options)
    {
        errno = EINVAL;
        return EXIT_FAILURE;
    }

    if (!options->fadd_to_bloom || !options->fcheck_in_bloom)
    {
        errno = ENOENT;
        return EXIT_FAILURE;
    }

    if (!options->m || !options->k)
    {
        errno = EINVAL;
        return EXIT_FAILURE;
    }

    test_results_t test_res = {options->verbose, 0, 0, 0, 0, 0};

    if (options->verbose)
        printf("\n---\nCreate Bloom filter with %d bits and %d hash functions\n", options->m, options->k);
    bloom_t *filter = bloom_create(options->m, options->k);
    if (options->verbose)
        bloom_print(filter);

    process_stream(&test_res, filter, operate_test_add_with_warning, options->fadd_to_bloom);
    if (options->verbose)
        bloom_print(filter);

    process_stream(&test_res, filter, operate_test_lookup_real_positives, options->fadd_to_bloom);
    process_stream(&test_res, filter, operate_test_lookup_real_negatives, options->fcheck_in_bloom);

    char test_save_bloom[] = "test_save_bloom.txt";

    if (options->verbose)
        printf("\n---\nSave Bloom filter\n");
    uint32_t save_status = bloom_save(filter, test_save_bloom);
    if (save_status == 0)
        printf("\nSucessfully saved Bloom filter to %s!", test_save_bloom);
    else
        printf("\nError saving Bloom filter to %s!", test_save_bloom);

    if (options->verbose)
        printf("\n---\nFree Bloom filter");
    bloom_free(filter);

    test_results_t test_res_load = {options->verbose, 0, 0, 0, 0, 0};

    printf("\n---\nLoad Bloom Filter from %s\n", test_save_bloom);
    bloom_t *load_filter = bloom_load(test_save_bloom);
    bloom_print(load_filter);

    process_stream(&test_res_load, load_filter, operate_test_lookup_real_positives, options->fadd_to_bloom);
    process_stream(&test_res_load, load_filter, operate_test_lookup_real_negatives, options->fcheck_in_bloom);

    if (options->verbose)
        printf("\n---\nFree loaded Bloom filter\n");
    bloom_free(load_filter);

    /* print results of test */
    printf("Bloom filter (orig): TP %d | TN %d | FP %d | FN %d | add warnings %d \n", test_res.count_TP, test_res.count_TN, test_res.count_FP, test_res.count_FN, test_res.count_add_warnings);
    printf("Bloom filter (load): TP %d | TN %d | FP %d | FN %d | \n", test_res_load.count_TP, test_res_load.count_TN, test_res_load.count_FP, test_res_load.count_FN);

    return EXIT_SUCCESS;
}