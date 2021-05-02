#include "bitslicedsig.h"

#define WORD_POW 5
#define WORD_SIZE 32
#define BUFF_SIZE 128

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

bitslicedsig_t *bitslicedsig_create(int max_doc_capacity, int m, int k)
{
    int i;
    bitslicedsig_t *bitslicedsig = malloc(sizeof(bitslicedsig_t));

    bitslicedsig->added_d = 0;
    bitslicedsig->num_blocks = (max_doc_capacity + WORD_SIZE - 1) / WORD_SIZE;

    bitslicedsig->m = m;
    bitslicedsig->bit_matrix = (u_int32_t **)malloc(m * sizeof(u_int32_t *));
    for (i = 0; i < m; i++)
        bitslicedsig->bit_matrix[i] = calloc(bitslicedsig->num_blocks, sizeof(u_int32_t));

    bitslicedsig->k = k;
    bitslicedsig->hash_seeds = calloc(k, sizeof(u_int32_t));
    for (i = 0; i < k; i++)
        bitslicedsig->hash_seeds[i] = (rand() % (WORD_SIZE + 1));

    return bitslicedsig;
}

void bitslicedsig_free(bitslicedsig_t *bitslicedsig)
{
    if (bitslicedsig)
    {
        int i;
        for (i = 0; i < bitslicedsig->m; i++)
            free((bitslicedsig->bit_matrix)[i]);
        free(bitslicedsig->bit_matrix);
        free(bitslicedsig->hash_seeds);
        free(bitslicedsig);
    }
}

void bitslicedsig_add_doc(bitslicedsig_t *bitslicedsig, int index, char *filename)
{

    FILE *f;
    f = fopen(filename, "r");
    if (f == NULL)
    {
        perror(filename);
        return;
    }

    int h;
    u_int32_t hash;

    int blockIndex = index >> WORD_POW;
    u_int32_t docWord = 1ULL << mod_pow_2(index, WORD_SIZE);

    const int bufferLength = 1023; // assumes no term exceeds length of 1023
    char buffer[bufferLength];
    char *token;
    char *rest;

    while (fgets(buffer, bufferLength, f))
    {
        rest = buffer;
        while ((token = strtok_r(rest, " !\"#$%%&()*+,-./:;<=>?@[\\]^_`{|}~", &rest)))
        {
            if (token[strlen(token) - 1] == '\n')
                token[strlen(token) - 1] = '\0';
            /* hash each term with each hash function */
            for (h = 0; h < bitslicedsig->k; h++)
            {
                // hash the token to generate a hash
                hash = murmurhash(token, (u_int32_t)strlen(token), bitslicedsig->hash_seeds[h]);
                // decide what row to set by taking hash % number of bits
                hash = mod_pow_2(hash, bitslicedsig->m);
                // set the bit in array by changing specific bit of specific word
                bitslicedsig->bit_matrix[hash][blockIndex] |= docWord;
            }
        }
    }

    fclose(f);
    bitslicedsig->added_d++;
}

void bitslicedsig_query(bitslicedsig_t *bitslicedsig, FILE *fquery)
{
    /* check that stream is not a NULL pointer */
    if (!fquery)
    {
        perror("Error: could not open file\n");
        exit(EXIT_FAILURE);
        /* NOTREACHED */
    }

    int h;
    u_int32_t hash;

    /* flag which row indicies show up in query */
    int num_query_blocks = (bitslicedsig->m + WORD_SIZE - 1) / WORD_SIZE;
    u_int32_t *querysig = calloc(num_query_blocks, sizeof(u_int32_t));

    const int bufferLength = 1023; // assumes no term exceeds length of 1023
    char buffer[bufferLength];
    char *token;
    char *rest;

    while (fgets(buffer, bufferLength, fquery))
    {
        rest = buffer;
        while ((token = strtok_r(rest, " !\"#$%%&()*+,-./:;<=>?@[\\]^_`{|}~", &rest)))
        {
            if (token[strlen(token) - 1] == '\n')
                token[strlen(token) - 1] = '\0';
            /* hash each term with each hash function */
            for (h = 0; h < bitslicedsig->k; h++)
            {
                // hash the token to generate a hash
                hash = murmurhash(token, (u_int32_t)strlen(token), bitslicedsig->hash_seeds[h]);
                // decide what row to set by taking hash % number of bits
                hash = mod_pow_2(hash, bitslicedsig->m);
                // set the bit in array by changing specific bit of specific word
                querysig[hash >> WORD_POW] |= 1ULL << hash;
            }
        }
    }

    int b, r, c;
    bool isSet;

    /* find intersecting documents */
    u_int32_t word_mask;
    for (b = 0; b < bitslicedsig->num_blocks; b++)
    {
        word_mask = -1;
        for (r = 0; r < bitslicedsig->m; r++)
        {
            isSet = querysig[r >> WORD_POW] & (1ULL << mod_pow_2(r, bitslicedsig->m));
            if (isSet)
            {
                word_mask &= bitslicedsig->bit_matrix[r][b];
            }
        }

        /* decode word_mask */
        for (c = 0; c < WORD_SIZE; c++)
        {
            isSet = word_mask & (1ULL << c);
            if (isSet)
                printf("doc %d \n", c);
        }
    }
}

void bitslicesig_print(bitslicedsig_t *bitslicedsig)
{
    u_int32_t i, j;
    printf("\n---------------------------------\n");
    printf("Bit-Sliced Block Signature\n");
    printf("\nm = %d, k = %d, num_blocks = %d", bitslicedsig->m, bitslicedsig->k, bitslicedsig->num_blocks);
    printf("\n%d/%d docs added", bitslicedsig->added_d, bitslicedsig->num_blocks * WORD_SIZE);

    printf("\nhash_seeds = ");
    for (i = 0; i < bitslicedsig->k; i++)
        printf("%d ", bitslicedsig->hash_seeds[i]);

    printf("\narray = ");
    for (i = 0; i < bitslicedsig->m; i++)
    {
        for (j = 0; j < bitslicedsig->num_blocks; j++)
        {
            printf("%x ", bitslicedsig->bit_matrix[i][j]);
        }
    }
    printf("\n---------------------------------\n");
}
