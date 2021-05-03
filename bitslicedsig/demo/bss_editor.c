/**
 * Create or modify a bit-sliced signature
 * 
 * Load a bit-sliced signature from a file or create a new one. Add new documents to it and save it.
 * 
 * Author: Gati Aher
 */

#include "bss_editor.h"

int bss_editor(options_t *options)
{
    if (!options)
    {
        errno = EINVAL;
        return EXIT_FAILURE;
    }

    if (!options->loc_save_bss)
    {
        errno = ENOENT;
        return EXIT_FAILURE;
    }

    if (!options->loc_load_bss && (!options->m || !options->k || !options->d))
    {
        errno = EINVAL;
        return EXIT_FAILURE;
    }

    bitslicedsig_t *bss;

    if (options->loc_load_bss)
    {
        /* load bit-sliced bit signature */
        bss = bitslicedsig_load(options->loc_load_bss);
    }
    else
    {
        /* create bit-sliced signature */
        bss = bitslicedsig_create(options->m, options->k, options->d);
    }

    if (options->verbose)
        bitslicedsig_print(bss);

    /* add terms to bit-sliced signature */
    const uint32_t bufferLength = 1023; // assumes no term exceeds length of 1023
    char buffer[bufferLength];
    int idx;
    char filename[bufferLength];

    while (fgets(buffer, bufferLength, options->fread_input_from))
    {
        if (buffer[strlen(buffer) - 1] == '\n')
            buffer[strlen(buffer) - 1] = '\0';
        sscanf(buffer, "%d %s", &idx, filename);
        printf("Adding document %d %s\n", idx, filename);
        bitslicedsig_add_doc(bss, idx, filename);
    }

    if (options->verbose)
        bitslicedsig_print(bss);

    /* save bit-sliced signature */
    uint32_t save_status = bitslicedsig_save(bss, options->loc_save_bss);
    if (save_status == 0)
        printf("Sucessfully saved bit-sliced signature to %s!\n", options->loc_save_bss);
    else
        perror("Error saving bit-sliced signature!\n");

    bitslicedsig_free(bss);

    return EXIT_SUCCESS;
}