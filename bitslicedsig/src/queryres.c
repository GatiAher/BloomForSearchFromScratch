/**
 * A structure to store the results of a query.
 * 
 * A partial implementation of a linked list is used to store query results
 * because an unknown number of
 */

#include "queryres.h"

queryres_t *queryres_create()
{
    queryres_t *qr = malloc(sizeof(queryres_t));
    qr->head = NULL;
    qr->len = 0;
}

void queryres_free(queryres_t *qr)
{
    //create a link
    res_t *ptr = qr->head;

    while (ptr != NULL)
    {
        res_t *temp_ptr = ptr->next;
        free(ptr);
        ptr = temp_ptr;
    }
}

void queryres_add(queryres_t *qr, u_int32_t data)
{
    res_t *link = (res_t *)malloc(sizeof(res_t));
    link->data = data;
    link->next = qr->head;

    qr->head = link;
    qr->len += 1;
}

void queryres_print(queryres_t *qr)
{
    if (qr != NULL)
    {
        res_t *ptr = qr->head;
        printf("\n[ ");

        while (ptr != NULL)
        {
            printf("%d, ", ptr->data);
            ptr = ptr->next;
        }
        printf(" ] len=%d \n", qr->len);
    }
}