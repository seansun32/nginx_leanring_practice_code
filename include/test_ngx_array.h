#ifndef __TEST_NGX_ARRAY_H__
#define __TEST_NGX_ARRAY_H__

#include "test_ngx_mempool.h"

typedef struct {
    void            *elts;
    unsigned int    nelts;
    size_t          size;
    unsigned int    nalloc;
    test_ngx_pool_t      *pool;
} test_ngx_array_t;


test_ngx_array_t *test_ngx_array_create(test_ngx_pool_t *p, unsigned int n, size_t size);
void test_ngx_array_destroy(test_ngx_array_t *a);
void *test_ngx_array_push(test_ngx_array_t *a);
void *test_ngx_array_push_n(test_ngx_array_t *a, unsigned int n);


static inline int 
test_ngx_array_init(test_ngx_array_t *array, test_ngx_pool_t *pool, unsigned int n, size_t size)
{
    /*
     * set "array->nelts" before "array->elts", otherwise MSVC thinks
     * that "array->nelts" may be used without having been initialized
     */

    array->nelts = 0;
    array->size = size;
    array->nalloc = n;
    array->pool = pool;

    array->elts = test_ngx_palloc(pool, n * size);
    if (array->elts == NULL) {
        return -1;
    }

    return 0;
}

#endif
