#ifndef __TEST_NGX_LIST_H__
#define __TEST_NGX_LIST_H__

#include "test_ngx_mempool.h"

typedef struct test_ngx_list_part_s  test_ngx_list_part_t;

struct test_ngx_list_part_s {
    void             *elts;
    unsigned int     nelts;
    test_ngx_list_part_t  *next;
};

typedef struct {
    test_ngx_list_part_t  *last;
    test_ngx_list_part_t   part;
    size_t            size;
    unsigned int     nalloc;
    test_ngx_pool_t       *pool;
} test_ngx_list_t;

test_ngx_list_t *test_ngx_list_create(test_ngx_pool_t *pool, unsigned int n, size_t size);
void *test_ngx_list_push(test_ngx_list_t *list);

static inline int
test_ngx_list_init(test_ngx_list_t *list, test_ngx_pool_t *pool, unsigned int n, size_t size)
{
    list->part.elts = test_ngx_palloc(pool, n * size);
    if (list->part.elts == NULL) {
        return -1;
    }

    list->part.nelts = 0;
    list->part.next = NULL;
    list->last = &list->part;
    list->size = size;
    list->nalloc = n;
    list->pool = pool;

    return 0;
}


#endif
