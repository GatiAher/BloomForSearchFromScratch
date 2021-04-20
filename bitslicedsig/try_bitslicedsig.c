#include "try_bitslicedsig.h"

/* Run bloom filter */
int try_bitslicedsig(options_t *options)
{

    if (!options)
    {
        errno = EINVAL;
        return EXIT_FAILURE;
    }

    // if (!options->add_to_bloom || !options->check_in_bloom)
    // {
    //     errno = ENOENT;
    //     return EXIT_FAILURE;
    // }

    // if (!options->num_sig_bits || !options->num_hash)
    // {
    //     errno = EINVAL;
    //     return EXIT_FAILURE;
    // }

    // if (options->verbose)
    //     printf("\n ------ \n Create bloom filter with %d bits and %d hash functions \n", options->num_sig_bits, options->num_hash);

    bitslicedsig_t *bss = bitslicedsig_create(5, 64, 5);
    bitslicesig_print(bss);
    bitslicedsig_add_doc(bss, 0, "bitslicedsig_data/doc0.txt");
    printf("Add doc 1: \n");
    bitslicesig_print(bss);
    bitslicedsig_add_doc(bss, 1, "bitslicedsig_data/doc1.txt");
    printf("Add doc 2: \n");
    bitslicesig_print(bss);
    bitslicedsig_add_doc(bss, 2, "bitslicedsig_data/doc2.txt");
    printf("Add doc 3: \n");
    bitslicesig_print(bss);
    bitslicedsig_query(bss, "skiff in the Gulf Stream");
    bitslicedsig_free(bss);

    // /* assumes no word exceeds length of 1023 */
    // const int bufferLength = 1023;
    // char buffer[bufferLength];
    // int count_TP = 0, count_TN = 0, count_FP = 0, count_FN = 0, add_warnings = 0;
    // bool isIn;

    return EXIT_SUCCESS;
}