/**
 * Create or modify a Bloom filter
 * 
 * Load a Bloom filter from a file or create a new one. Add new terms to it and save it.
 * 
 * Author: Gati Aher
 */

#ifndef BF_EDITOR_H
#define BF_EDITOR_H

#include "bf_editor_main.h"
#include "../src/bloom.h"

/* Create and save a Bloom filter */
int bf_editor(options_t *options);

#endif