/**
 * Create or modify a bit-sliced signature
 * 
 * Load a bit-sliced signature from a file or create a new one. Add new documents to it and save it.
 * 
 * Author: Gati Aher
 * Date: April 30, 2021
 */

#ifndef BSS_EDITOR_H
#define BSS_EDITOR_H

#include "bss_editor_main.h"
#include "../src/bitslicedsig.h"

/* Create and save a Bloom filter */
int bss_editor(options_t *options);

#endif