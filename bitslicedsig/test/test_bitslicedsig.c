/**
 * Test that Bit-Sliced Block Signature implementation works as expected.
 * 
 * Author: Gati Aher
 * Date: April 30, 2021
 */

#include "test_bitslicedsig.h"
/* Run bloom filter */
int test_bitslicedsig(options_t *options)
{

    if (!options)
    {
        errno = EINVAL;
        return EXIT_FAILURE;
    }

    if (!options->prefix_path_of_corpus_to_add || !options->fquery)
    {
        errno = EINVAL;
        return EXIT_FAILURE;
    }

    if (!options->m || !options->k | !options->d)
    {
        errno = EINVAL;
        return EXIT_FAILURE;
    }

    if (options->verbose)
        printf("\n ------ \n Create Bit-Sliced Signature with %d signature bits, %d hash functions, and can store atleast of %d documents \n", options->m, options->k, options->d);
    bitslicedsig_t *bss = bitslicedsig_create(5, 64, 5);
    if (options->verbose)
        bitslicesig_print(bss);

    /* Add all the documents */
    uint32_t i;
    for (i = 0; i < options->d; i++)
    {
        char pathcopy[1000];
        char numstr[1000];
        sprintf(numstr, "%d", i);
        memset(pathcopy, '\0', sizeof(pathcopy));
        strcpy(pathcopy, options->prefix_path_of_corpus_to_add);
        strcat(pathcopy, numstr);
        strcat(pathcopy, ".txt");
        bitslicedsig_add_doc(bss, i, pathcopy);
        printf("Added doc %d: %s\n", i, pathcopy);
        if (options->verbose)
        {
            bitslicesig_print(bss);
        }
    }

    /* query */
    if (options->verbose)
    {
        const u_int32_t bufferLength = 1023; // assumes no term exceeds length of 1023
        char buffer[bufferLength];
        char *token;
        char *rest;

        printf("query: ");
        while (fgets(buffer, bufferLength, options->fquery))
        {
            rest = buffer;
            while ((token = strtok_r(rest, " !\"#$%%&()*+,-./:;<=>?@[\\]^_`{|}~", &rest)))
            {
                if (token[strlen(token) - 1] == '\n')
                    token[strlen(token) - 1] = '\0';
                printf("%s ", token);
            }
        }
        printf("\n");
        rewind(options->fquery);
    }

    bitslicedsig_query(bss, options->fquery);
    bitslicedsig_free(bss);

    return EXIT_SUCCESS;
}