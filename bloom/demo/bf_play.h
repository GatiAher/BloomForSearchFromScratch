/**
 * Interactive Bloom filter.
 * 
 * Load a Bloom filter and use it to evaluate a query.
 * 
 * Author: Gati Aher
 */

#ifndef BF_PLAY_H
#define BF_PLAY_H

#include "bf_play_main.h"
#include "../src/bloom.h"

/* Use a Bloom filter to process input

print output to standard output.
 */
int bf_play(options_t *options);

#endif