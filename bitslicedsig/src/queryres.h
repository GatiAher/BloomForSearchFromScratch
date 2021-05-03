/**
 * A structure to store the results of a query.
 * 
 * A partial implementation of a linked list is used to store query results
 * because an unknown number of results can be returned.
 * 
 * Author: Gati Aher
 */

#ifndef QUERYRES_H
#define QUERYRES_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct res_t res_t;
struct res_t
{
    u_int32_t data;
    res_t *next;
};

typedef struct
{
    res_t *head;
    u_int32_t len;
} queryres_t;

/** Creates a new structure to store query results
 * 
 * Returns a pointer to the structure.
 */
queryres_t *queryres_create();

/* Frees a query result */
void queryres_free(queryres_t *qr);

/* Adds a result to query result */
void queryres_add(queryres_t *qr, u_int32_t data);

/* Print query result */
void queryres_print(queryres_t *qr);

#endif