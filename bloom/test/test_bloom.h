/**
 * Test that Bloom filter implementation works as expected.
 * 
 * Author: Gati Aher
 * Date: April 30, 2021
 */

#ifndef TEST_BLOOM_H
#define TEST_BLOOM_H

#include "test_bloom_main.h"
#include "../src/bloom.h"

typedef struct
{
    uint32_t verbose;
    uint32_t count_add_warnings;
    uint32_t count_TP;
    uint32_t count_TN;
    uint32_t count_FP;
    uint32_t count_FN;
} test_results_t;

/* Run Bloom filter functionality test*/
int test_bloom(options_t *options);

#endif