/**
 * Interactive bit-sliced signature.
 * 
 * Author: Gati Aher
 * Date: April 30, 2021
 */

#include "bss_play.h"

/* for coloring text */
#define PRINTC(c, f, s) printf("\033[%dm" f "\033[0m", 30 + c, s)

int bss_play(options_t *options)
{
    if (!options)
    {
        errno = EINVAL;
        return EXIT_FAILURE;
    }

    if (!options->loc_load_bss)
    {
        errno = ENOENT;
        return EXIT_FAILURE;
    }

    if (!options->fread_input_from)
    {
        errno = ENOENT;
        return EXIT_FAILURE;
    }

    if (!options->output_format_string)
    {
        errno = ENOENT;
        return EXIT_FAILURE;
    }

    if (options->verbose)
        printf("\nLoad bitslicedsig from file %s\n", options->loc_load_bss);

    /* load bit-sliced signature */
    bitslicedsig_t *bss = bitslicedsig_load(options->loc_load_bss);

    if (options->verbose)
        bitslicedsig_print(bss);

    queryres_t *qr = bitslicedsig_query(bss, options->fread_input_from);

    printf("\n%d Documents matching query: \n", qr->len);
    res_t *ptr = qr->head;
    while (ptr != NULL)
    {
        printf(options->output_format_string, ptr->data);
        ptr = ptr->next;
    }
    queryres_free(qr);

    return EXIT_SUCCESS;
}