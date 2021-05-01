/**
 * Test that murmurhash dependency works as expected.
 * 
 * Author: Gati Aher
 * Date: April 30, 2021
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "../../deps/murmurhash/murmurhash.h"

int main(void)
{
    u_int32_t seed = 0;
    const char *key = "gati";
    u_int32_t hash = murmurhash(key, (u_int32_t)strlen(key), seed); // 0xbcce539
    assert(hash == 0xbcce539);
    printf("expect hash: bcce539 \n");
    printf("given  hash: %x \n", hash);
    return 0;
}