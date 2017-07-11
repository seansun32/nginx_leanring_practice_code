#ifndef __TEST_NGX_MEMPOOL_H__
#define __TEST_NGX_MEMPOOL_H__


#define TEST_NGX_POOL_ALIGNMENT         16
#define test_ngx_align_ptr(p, a)                                                   \
    (unsigned char *) (((uintptr_t) (p) + ((uintptr_t) a - 1)) & ~((uintptr_t) a - 1))

#define TEST_NGX_MAX_ALLOC_FROM_POOL         (4096-1)


typedef struct test_ngx_pool_s          test_ngx_pool_t;
typedef struct test_ngx_pool_data_s     test_ngx_pool_data_t;
typedef struct test_ngx_pool_large_s    test_ngx_pool_large_t;
typedef struct test_ngx_pool_cleanup_s  test_ngx_pool_cleanup_t;


typedef void (*test_ngx_pool_cleanup_pt)(void *data);

struct test_ngx_pool_data_s{
    unsigned char*      last;
    unsigned char*      end;
    test_ngx_pool_t*    next;
    unsigned int        failed;
};

struct test_ngx_pool_s{
    test_ngx_pool_data_t        d;
    size_t                      max;
    test_ngx_pool_t*            current;
    test_ngx_pool_large_t*      large;
    test_ngx_pool_cleanup_t*    cleanup;
};

struct test_ngx_pool_large_s{
    test_ngx_pool_large_t*  next;
    void*                   alloc;
};

struct test_ngx_pool_cleanup_s{
    test_ngx_pool_cleanup_pt        handler;
    void*                       data;
    test_ngx_pool_cleanup_t*    next;
};


typedef struct{
    int             fd;
    unsigned char   *name;
}test_ngx_pool_cleanup_file_t;

typedef struct{
    test_ngx_pool_t* pool;
    void* data;
}test_ngx_data_t;




test_ngx_pool_t* 
test_ngx_create_pool(size_t size);

void 
test_ngx_destroy_pool(test_ngx_pool_t* pool);

static void*
test_ngx_palloc_block(test_ngx_pool_t* pool,size_t size);

static void*
test_ngx_palloc_small(test_ngx_pool_t* pool,size_t size);

static void*
test_ngx_palloc_large(test_ngx_pool_t* pool,size_t size);

void*
test_ngx_palloc(test_ngx_pool_t* pool,size_t size);

int 
test_ngx_pfree(test_ngx_pool_t* pool, void* p);

void
test_ngx_pool_cleanup_file(void* data);

void
test_ngx_pool_run_cleanup_file(test_ngx_pool_t *p,int fd);

test_ngx_pool_cleanup_t*
test_ngx_pool_cleanup_add(test_ngx_pool_t* p, size_t size);


#endif
