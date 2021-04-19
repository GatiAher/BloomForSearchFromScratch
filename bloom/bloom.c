#include "bloom.h"

#define WORD_POW 3
#define WORD_SIZE 8

/* Helper Functions */

/* print an array in hexadecimal */
void print_array_hex(int size, u_int8_t *array)
{
    for (int i = 0; i < size; i++)
    {
        int x = array[i];
        printf("%x ", x);
    }
}

/**
 * Returns a mod b
 * 
 * b must be a power of 2 
 */
int mod_pow_2(int a, int b)
{
    // if b is 2^n
    // a mod b = last n digits of a
    return a & (b - 1);
}

bloom_t *bloom_create(int num_bits, int num_hash_funcs)
{
    int i = 0;
    bloom_t *filter = malloc(sizeof(bloom_t));
    filter->num_bits = num_bits;
    filter->array = malloc(num_bits >> WORD_POW);
    filter->num_hash_funcs = num_hash_funcs;
    filter->hash_seeds = (u_int32_t *)malloc(num_hash_funcs * sizeof(u_int32_t));
    for (i = 0; i < num_hash_funcs; i++)
        filter->hash_seeds[i] = i;

    return filter;
}

void bloom_free(bloom_t *filter)
{
    if (filter)
    {
        free(filter->array);
        free(filter->hash_seeds);
        free(filter);
    }
}

void bloom_add(bloom_t *filter, const char *item)
{
    int i;
    u_int32_t hash;
    u_int8_t *bits = filter->array;

    for (i = 0; i < filter->num_hash_funcs; i++)
    {
        // hash the string to generate a hash
        hash = murmurhash(item, (u_int32_t)strlen(item), filter->hash_seeds[i]);
        // decide what bit of array to set by taking hash % number of bits
        hash = mod_pow_2(hash, filter->num_bits);
        // set the bit in array by changing specific bit of specific word
        // equivalent to bits[hash / 8] |= 1 << hash % 8;
        bits[hash >> WORD_POW] |= 1 << mod_pow_2(hash, WORD_SIZE);
    }
}

bool bloom_lookup(bloom_t *filter, const char *item)
{
    int i;
    u_int32_t hash;
    u_int8_t *bits = filter->array;

    for (i = 0; i < filter->num_hash_funcs; i++)
    {
        // hash the string to generate a hash
        hash = murmurhash(item, (u_int32_t)strlen(item), filter->hash_seeds[i]);
        // decide what bit of array to set by taking hash % number of bits
        hash = mod_pow_2(hash, filter->num_bits);
        // check the bit in array by indexing specific bit of specific word
        if (!(bits[hash >> WORD_POW] & 1 << mod_pow_2(hash, WORD_SIZE)))
        {
            return false;
        }
    }
    return true;
}

bool bloom_add_with_warning(bloom_t *filter, const char *item)
{
    if (bloom_lookup(filter, item))
    {
        return true;
    }
    bloom_add(filter, item);
    return false;
}
