/**
 * Interactive bit-sliced signature.
 * 
 * Load a bit-sliced signature and use it to evaluate a query.
 * 
 * Author: Gati Aher
 */

#ifndef BSS_PLAY_H
#define BSS_PLAY_H

#include "bss_play_main.h"
#include "../src/bitslicedsig.h"

/* Use a bit-sliced signature to process input

print output to standard output.
 */
int bss_play(options_t *options);

#endif