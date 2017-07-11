#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "test_ngx_list.h"

test_ngx_list_t *
test_ngx_list_create(test_ngx_pool_t *pool, unsigned int n, size_t size)
{
    test_ngx_list_t  *list;

    list = test_ngx_palloc(pool, sizeof(test_ngx_list_t));
    if (list == NULL) {
        return NULL;
    }

    if (test_ngx_list_init(list, pool, n, size) != 0) {
        return NULL;
    }

    return list;
}

void *
test_ngx_list_push(test_ngx_list_t *l)
{
    void             *elt;
    test_ngx_list_part_t  *last;

    last = l->last;

    if (last->nelts == l->nalloc) {

        /* the last part is full, allocate a new list part */

        last = test_ngx_palloc(l->pool, sizeof(test_ngx_list_part_t));
        if (last == NULL) {
            return NULL;
        }

        last->elts = test_ngx_palloc(l->pool, l->nalloc * l->size);
        if (last->elts == NULL) {
            return NULL;
        }

        last->nelts = 0;
        last->next = NULL;

        l->last->next = last;
        l->last = last;
    }

    elt = (char *) last->elts + l->size * last->nelts;
    last->nelts++;

    return elt;
}
