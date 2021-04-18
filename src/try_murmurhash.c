#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../deps/murmurhash/murmurhash.h"

int main(void)
{
    u_int32_t seed = 0;
    const char *key = "kinkajou";
    u_int32_t hash = murmurhash(key, (u_int32_t)strlen(key), seed); // 0xb6d99cf8
    printf("hash: %x \n", hash);
    return 0;
}