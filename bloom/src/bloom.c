/** Implementation of Bloom filter for 32-bit system.
 * 
 * Author: Gati Aher
*/

#include "bloom.h"

#define WORD_POW 5
#define WORD_SIZE 32

/**
 * Returns a mod b
 * 
 * b must be a power of 2 
 */
u_int32_t mod_pow_2(u_int32_t a, u_int32_t b)
{
    // if b is 2^n
    // a mod b = last n digits of a
    return a & (b - 1);
}

/** Returns power of 2 larger than n
 * 
 * Operates in log(log(n)) bit shifts 
 */
u_int32_t get_next_pow_2(u_int32_t n)
{
    n--;
    // Divide by 2^k for consecutive doublings of k up to 32, and then OR the results.
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    n++;
    // The result is a number of 1 bits equal to the number of bits in the original number, plus 1.
    // That's the next highest power of 2.
    return n;
}

/**Count bits set in number
 * 
 * source: https://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetParallel
 * 
 * Return count of set bits
 */
u_int32_t count_set_bits(u_int32_t n)
{
    u_int32_t c; // store the total here

    // divide and conquer
    c = n - ((n >> 1) & 0x55555555);
    c = ((c >> 2) & 0x33333333) + (c & 0x33333333);
    c = ((c >> 4) + c) & 0x0F0F0F0F;
    c = ((c >> 8) + c) & 0x00FF00FF;
    c = ((c >> 16) + c) & 0x0000FFFF;
    return c;
}

bloom_t *bloom_create(u_int32_t m, u_int32_t k)
{
    u_int32_t i = 0;

    bloom_t *filter = malloc(sizeof(bloom_t));

    u_int32_t num_bits = get_next_pow_2(m);
    filter->m = get_next_pow_2(num_bits);
    filter->array = calloc(num_bits >> WORD_POW, sizeof(u_int32_t));

    filter->k = k;
    filter->hash_seeds = (u_int32_t *)malloc(k * sizeof(u_int32_t));
    for (i = 0; i < k; i++)
        filter->hash_seeds[i] = (rand() % (WORD_SIZE + 1));

    filter->n = 0;

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
    u_int32_t i;
    u_int32_t hash;
    u_int32_t *bits = filter->array;

    for (i = 0; i < filter->k; i++)
    {
        // hash the string to generate a hash
        hash = murmurhash(item, (u_int32_t)strlen(item), filter->hash_seeds[i]);
        // decide what bit of array to set by taking hash % number of bits
        hash = mod_pow_2(hash, filter->m);
        // set the bit in array by setting specific bit of specific word
        bits[hash >> WORD_POW] |= 1 << mod_pow_2(hash, WORD_SIZE);
    }
    filter->n += 1;
}

bool bloom_lookup(bloom_t *filter, const char *item)
{
    u_int32_t i;
    u_int32_t hash;
    u_int32_t *bits = filter->array;

    for (i = 0; i < filter->k; i++)
    {
        // hash the string to generate a hash
        hash = murmurhash(item, (u_int32_t)strlen(item), filter->hash_seeds[i]);
        // decide what bit of array to set by taking hash % number of bits
        hash = mod_pow_2(hash, filter->m);
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

u_int32_t bloom_save(bloom_t *filter, const char *filename)
{
    FILE *outfile;
    outfile = fopen(filename, "w");
    if (outfile == NULL)
    {
        perror(ERR_FOPEN_SAVE_BLOOM_TO);
        return 1;
    }

    u_int32_t status_code = 0;

    // m
    if (fwrite(&filter->m, sizeof(u_int32_t), 1, outfile) == 0)
        status_code = 1;

    // k
    if (fwrite(&filter->k, sizeof(u_int32_t), 1, outfile) == 0)
        status_code = 1;

    // n
    if (fwrite(&filter->n, sizeof(u_int32_t), 1, outfile) == 0)
        status_code = 1;

    // hash_seeds
    if (fwrite(filter->hash_seeds, sizeof(u_int32_t), filter->k, outfile) == 0)
        status_code = 1;

    // array
    if (fwrite(filter->array, sizeof(u_int32_t), filter->m >> WORD_POW, outfile) == 0)
        status_code = 1;

    fclose(outfile);
    return status_code;
}

bloom_t *bloom_load(const char *filename)
{
    FILE *infile;

    infile = fopen(filename, "r");
    if (infile == NULL)
    {
        perror(ERR_FOPEN_LOAD_BLOOM_FROM);
        exit(1);
    }

    u_int32_t m, k, n;

    fread(&m, sizeof(u_int32_t), 1, infile);
    fread(&k, sizeof(u_int32_t), 1, infile);
    fread(&n, sizeof(u_int32_t), 1, infile);

    bloom_t *filter = bloom_create(m, k);
    filter->n = n;

    fread(filter->hash_seeds, sizeof(u_int32_t), filter->k, infile);
    fread(filter->array, sizeof(u_int32_t), filter->m >> WORD_POW, infile);

    fclose(infile);
    return filter;
}

void bloom_print(bloom_t *filter)
{
    printf("\n---------------------------------\n");
    printf("Bloom filter\n");
    printf("\nm = %d, k = %d", filter->m, filter->k);
    u_int32_t i;
    printf("\nhash_seeds = ");
    for (i = 0; i < filter->k; i++)
        printf("%d ", filter->hash_seeds[i]);

    printf("\narray = ");
    for (i = 0; i < 10; i++)
        printf("%x ", filter->array[i]);
    printf("... [output truncated]\n");

    u_int32_t csb = 0;
    for (i = 0; i < filter->m >> WORD_POW; i++)
    {
        csb += count_set_bits(filter->array[i]);
    }
    printf("%d/%d bits set\n", csb, filter->m);
    printf("%d items added\n", filter->n);
    printf("---------------------------------\n");
}
