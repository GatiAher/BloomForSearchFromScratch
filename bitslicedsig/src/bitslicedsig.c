/* Implementation of bit-sliced signature */

#include "bitslicedsig.h"

#define WORD_POW 5
#define WORD_SIZE 32
#define BUFF_SIZE 128

/* Helper Functions */

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

bitslicedsig_t *bitslicedsig_create(u_int32_t m, u_int32_t k, u_int32_t min_doc_capacity)
{
    u_int32_t i;
    bitslicedsig_t *bitslicedsig = malloc(sizeof(bitslicedsig_t));

    bitslicedsig->added_d = 0;
    bitslicedsig->num_blocks = (min_doc_capacity + WORD_SIZE - 1) / WORD_SIZE;

    bitslicedsig->m = get_next_pow_2(m);
    bitslicedsig->bit_matrix = (u_int32_t **)malloc(bitslicedsig->m * sizeof(u_int32_t *));
    for (i = 0; i < bitslicedsig->m; i++)
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
        u_int32_t i;
        for (i = 0; i < bitslicedsig->m; i++)
            free((bitslicedsig->bit_matrix)[i]);
        free(bitslicedsig->bit_matrix);
        free(bitslicedsig->hash_seeds);
        free(bitslicedsig);
    }
}

void bitslicedsig_add_doc(bitslicedsig_t *bitslicedsig, u_int32_t index, char *filename)
{

    FILE *f;
    f = fopen(filename, "r");
    if (f == NULL)
    {
        perror(filename);
        return;
    }

    u_int32_t h;
    u_int32_t hash;

    u_int32_t blockIndex = index >> WORD_POW;
    u_int32_t docWord = 1ULL << mod_pow_2(index, WORD_SIZE);

    const u_int32_t bufferLength = 1023; // assumes no term exceeds length of 1023
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

queryres_t *bitslicedsig_query(bitslicedsig_t *bitslicedsig, FILE *fquery)
{
    /* check that stream is not a NULL pointer */
    if (!fquery)
    {
        perror("Error: could not open file\n");
        exit(EXIT_FAILURE);
        /* NOTREACHED */
    }

    u_int32_t h;
    u_int32_t hash;

    /* flag which row indicies show up in query */
    u_int32_t num_query_blocks = bitslicedsig->m >> WORD_POW;
    u_int32_t querysig[num_query_blocks];
    memset(querysig, 0, num_query_blocks * sizeof(u_int32_t));

    const u_int32_t bufferLength = 1023; // assumes no term exceeds length of 1023
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
                querysig[hash >> WORD_POW] |= (1ULL << mod_pow_2(hash, WORD_SIZE));
            }
        }
    }

    u_int32_t b, r, c;
    bool isSet;

    queryres_t *qr = queryres_create();

    /* find intersecting documents */
    u_int32_t word_mask;
    for (b = 0; b < bitslicedsig->num_blocks; b++)
    {
        word_mask = -1;
        for (r = 0; r < bitslicedsig->m; r++)
        {
            isSet = querysig[r >> WORD_POW] & (1ULL << mod_pow_2(r, WORD_SIZE));
            if (isSet)
            {
                word_mask &= bitslicedsig->bit_matrix[r][b];
            }

            if (word_mask == 0)
            {
                // early termination of loop if no documents match
                continue;
            }
        }

        /* decode word_mask */
        for (c = 0; c < WORD_SIZE; c++)
        {
            isSet = word_mask & (1ULL << c);
            if (isSet)
                queryres_add(qr, c);
        }
    }

    return qr;
}

u_int32_t bitslicedsig_save(bitslicedsig_t *bitslicedsig, const char *filename)
{
    FILE *outfile;
    outfile = fopen(filename, "w");
    if (outfile == NULL)
    {
        perror(ERR_FOPEN_SAVE_BITSLICEDSIG_TO);
        return 1;
    }

    u_int32_t status_code = 0;

    // m
    if (fwrite(&bitslicedsig->m, sizeof(u_int32_t), 1, outfile) == 0)
        status_code = 1;

    // k
    if (fwrite(&bitslicedsig->k, sizeof(u_int32_t), 1, outfile) == 0)
        status_code = 1;

    // num_blocks
    if (fwrite(&bitslicedsig->num_blocks, sizeof(u_int32_t), 1, outfile) == 0)
        status_code = 1;

    // added_d
    if (fwrite(&bitslicedsig->added_d, sizeof(u_int32_t), 1, outfile) == 0)
        status_code = 1;

    // hash_seeds
    if (fwrite(bitslicedsig->hash_seeds, sizeof(u_int32_t), bitslicedsig->k, outfile) == 0)
        status_code = 1;

    // bit_matrix
    u_int32_t i = 0;
    for (i = 0; i < bitslicedsig->m; i++)
    {
        if (fwrite(bitslicedsig->bit_matrix[i], sizeof(u_int32_t), bitslicedsig->num_blocks, outfile) == 0)
            status_code = 1;
    }

    fclose(outfile);
    return status_code;
}

bitslicedsig_t *bitslicedsig_load(const char *filename)
{
    FILE *infile;

    infile = fopen(filename, "r");
    if (infile == NULL)
    {
        perror(ERR_FOPEN_LOAD_BITSLICEDSIG_FROM);
        exit(1);
    }

    u_int32_t m, k, num_blocks, added_d;

    fread(&m, sizeof(u_int32_t), 1, infile);
    fread(&k, sizeof(u_int32_t), 1, infile);
    fread(&num_blocks, sizeof(u_int32_t), 1, infile);
    fread(&added_d, sizeof(u_int32_t), 1, infile);

    bitslicedsig_t *bitslicedsig = bitslicedsig_create(m, k, num_blocks * WORD_SIZE);
    bitslicedsig->added_d = added_d;

    fread(bitslicedsig->hash_seeds, sizeof(u_int32_t), bitslicedsig->k, infile);

    // bit_matrix
    u_int32_t i = 0;
    for (i = 0; i < bitslicedsig->m; i++)
    {
        fread(bitslicedsig->bit_matrix[i], sizeof(u_int32_t), bitslicedsig->num_blocks, infile);
    }

    fclose(infile);
    return bitslicedsig;
}

void bitslicedsig_print(bitslicedsig_t *bitslicedsig)
{
    u_int32_t i, j, d, docWord;
    u_int32_t colsums[bitslicedsig->num_blocks * WORD_SIZE];
    memset(colsums, 0, bitslicedsig->num_blocks * WORD_SIZE * sizeof(u_int32_t));

    printf("\n---------------------------------\n");
    printf("Bit-Sliced Block Signature\n");
    printf("\nm = %d, k = %d, num_blocks = %d", bitslicedsig->m, bitslicedsig->k, bitslicedsig->num_blocks);
    printf("\n%d/%d docs added", bitslicedsig->added_d, bitslicedsig->num_blocks * WORD_SIZE);

    printf("\nhash_seeds = ");
    for (i = 0; i < bitslicedsig->k; i++)
        printf("%d ", bitslicedsig->hash_seeds[i]);

    printf("\narray = \n");
    for (i = 0; i < bitslicedsig->m && i < 10; i++)
    {
        for (j = 0; j < bitslicedsig->num_blocks && j < 10; j++)
        {
            printf("%d\t", bitslicedsig->bit_matrix[i][j]);
        }
        printf("\n");
    }
    printf("... [output truncated]");

    printf("\ncolsums = ");
    for (i = 0; i < bitslicedsig->m; i++)
    {
        for (j = 0; j < bitslicedsig->num_blocks; j++)
        {
            for (d = 0; d < WORD_SIZE; d++)
            {
                docWord = 1ULL << d;
                if (bitslicedsig->bit_matrix[i][j] & docWord)
                    colsums[(j * WORD_SIZE) + d] += 1;
            }
        }
    }
    for (i = 0; i < bitslicedsig->num_blocks * WORD_SIZE; i++)
        printf("%d ", colsums[i]);

    printf("\n\npercent filled = ");
    double m = bitslicedsig->m;
    for (i = 0; i < bitslicedsig->num_blocks * WORD_SIZE; i++)
        printf("%d=%.2f%% ", i, colsums[i] / m);

    printf("\n---------------------------------\n");
}
