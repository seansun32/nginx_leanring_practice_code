#include <stdio.h>

#include "test_ngx_array.h"

test_ngx_array_t *
test_ngx_array_create(test_ngx_pool_t *p, unsigned int n, size_t size)
{
    test_ngx_array_t *a;

    a = test_ngx_palloc(p, sizeof(test_ngx_array_t));
    if (a == NULL) {
        return NULL;
    }

    if (test_ngx_array_init(a, p, n, size) != 0) {
        return NULL;
    }

    return a;
}


void
test_ngx_array_destroy(test_ngx_array_t *a)
{
    test_ngx_pool_t  *p;

    p = a->pool;

    if ((unsigned char*) a->elts + a->size * a->nalloc == p->d.last) {
        p->d.last -= a->size * a->nalloc;
    }

    if ((unsigned char*) a + sizeof(test_ngx_array_t) == p->d.last) {
        p->d.last = (unsigned char*) a;
    }
}

void *
test_ngx_array_push(test_ngx_array_t *a)
{
    void        *elt, *new;
    size_t       size;
    test_ngx_pool_t  *p;

    if (a->nelts == a->nalloc) {

        /* the array is full */

        size = a->size * a->nalloc;

        p = a->pool;

        if ((unsigned char*) a->elts + size == p->d.last
            && p->d.last + a->size <= p->d.end)
        {
            /*
             * the array allocation is the last in the pool
             * and there is space for new allocation
             */

            p->d.last += a->size;
            a->nalloc++;

        } else {
            /* allocate a new array */

            new = test_ngx_palloc(p, 2 * size);
            if (new == NULL) {
                return NULL;
            }

            memcpy(new, a->elts, size);
            a->elts = new;
            a->nalloc *= 2;
        }
    }

    elt = (unsigned char*) a->elts + a->size * a->nelts;
    a->nelts++;

    return elt;
}

void *
test_ngx_array_push_n(test_ngx_array_t *a, unsigned int n)
{
    void        *elt, *new;
    size_t       size;
    unsigned int nalloc;
    test_ngx_pool_t  *p;

    size = n * a->size;

    if (a->nelts + n > a->nalloc) {

        /* the array is full */

        p = a->pool;

        if ((unsigned char*) a->elts + a->size * a->nalloc == p->d.last
            && p->d.last + size <= p->d.end)
        {
            /*
             * the array allocation is the last in the pool
             * and there is space for new allocation
             */

            p->d.last += size;
            a->nalloc += n;

        } else {
            /* allocate a new array */

            nalloc = 2 * ((n >= a->nalloc) ? n : a->nalloc);

            new = test_ngx_palloc(p, nalloc * a->size);
            if (new == NULL) {
                return NULL;
            }

            memcpy(new, a->elts, a->nelts * a->size);
            a->elts = new;
            a->nalloc = nalloc;
        }
    }

    elt = (unsigned char*) a->elts + a->size * a->nelts;
    a->nelts += n;

    return elt;
}




