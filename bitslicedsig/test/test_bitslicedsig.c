/**
 * Test that Bit-Sliced Block Signature implementation works as expected.
 * 
 * Author: Gati Aher
 * Date: April 30, 2021
 */

#include "test_bitslicedsig.h"

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

    /* test creation */
    if (options->verbose)
        printf("\n ------ \n Create Bit-Sliced Signature with %d signature bits, %d hash functions, and can store atleast of %d documents \n", options->m, options->k, options->d);
    bitslicedsig_t *bss = bitslicedsig_create(options->m, options->k, options->d);
    if (options->verbose)
        bitslicedsig_print(bss);

    /* test add */
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
            bitslicedsig_print(bss);
        }
    }

    /* get query for logging */
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

    /* test lookup */
    queryres_t *qr = bitslicedsig_query(bss, options->fquery);
    printf("\nDocuments matching query: ");
    queryres_print(qr);
    queryres_free(qr);

    /* test save */
    char test_save_bss[] = "test_save_bitslicedsig.txt";
    if (options->verbose)
        printf("\n---\nSave Bit-Sliced Signature \n");
    uint32_t save_status = bitslicedsig_save(bss, test_save_bss);
    if (save_status == 0)
        printf("\nSucessfully saved Bit-Sliced Signature to %s!", test_save_bss);
    else
        printf("\nError saving Bit-Sliced Signature to %s!", test_save_bss);

    /* test free */
    if (options->verbose)
        printf("\n---\nFree Bit-Sliced Signature");
    bitslicedsig_free(bss);

    /* test load */
    printf("\n---\nLoad Bit-Sliced Signature from %s\n", test_save_bss);
    bitslicedsig_t *load_bss = bitslicedsig_load(test_save_bss);
    bitslicedsig_print(load_bss);

    rewind(options->fquery);
    queryres_t *qr_load = bitslicedsig_query(load_bss, options->fquery);
    printf("\nDocuments matching query: ");
    queryres_print(qr_load);
    queryres_free(qr_load);

    if (options->verbose)
        printf("\n---\nFree loaded Bit-Sliced Signature\n");
    bitslicedsig_free(load_bss);

    return EXIT_SUCCESS;
}