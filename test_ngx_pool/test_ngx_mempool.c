#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>

#include "test_ngx_mempool.h"


/*
 *     create pool
 */

test_ngx_pool_t* 
test_ngx_create_pool(size_t size){
    int err;
    test_ngx_pool_t* p;
    err=posix_memalign((void**)&p,TEST_NGX_POOL_ALIGNMENT,size);
    if(err){
        printf("faild to alloc memory\n");
        p=NULL;
    }
    
    p->d.last=(unsigned char*)p+sizeof(test_ngx_pool_t);
    p->d.end=(unsigned char*)p+size;
    p->d.next=NULL;
    p->d.failed=0;

    size=size-sizeof(test_ngx_pool_t);
    p->max = (size < TEST_NGX_MAX_ALLOC_FROM_POOL) ? size : TEST_NGX_MAX_ALLOC_FROM_POOL;

    p->current=p;
    p->large=NULL;
    p->cleanup=NULL;
    

    return p;
}

/*
 *      destroy pool
 */

void 
test_ngx_destroy_pool(test_ngx_pool_t* pool){
    test_ngx_pool_t         *p, *n;
    test_ngx_pool_large_t   *l;
    test_ngx_pool_cleanup_t *c;

    for(c=pool->cleanup;c;c=c->next){
        if(c->handler){
            c->handler(c->data); 
        } 
    }

    for(l=pool->large;l;l=l->next){
        if(l->alloc){
            free(l->alloc); 
            l->alloc=NULL;
        }
    }

    for(p=pool,n=pool->d.next;;p=n,n=n->d.next){
        free(p);
        p=NULL;
        if(n==NULL){
            break; 
        }
    }
}


/*
 *      alloc memory
 */
static void*
test_ngx_palloc_block(test_ngx_pool_t* pool,size_t size){
    unsigned char       *m;
    size_t              psize;
    test_ngx_pool_t     *p,*new;
    
    psize=(size_t)(pool->d.end-(unsigned char*)pool);

    posix_memalign((void**)&m,TEST_NGX_POOL_ALIGNMENT,psize);
    if(m==NULL)
        return NULL;

    new=(test_ngx_pool_t*)m;
    new->d.end=m+psize;
    new->d.next=NULL;
    new->d.failed=0;

    m += sizeof(test_ngx_pool_data_t);
    m = test_ngx_align_ptr(m,TEST_NGX_POOL_ALIGNMENT);
    new->d.last=m+size;

    for(p=pool->current;p->d.next;p=p->d.next){
        if(p->d.failed++>4){
            pool->current=p->d.next; 
        } 
    }
    p->d.next=new;

    return m;
}

static void*
test_ngx_palloc_small(test_ngx_pool_t* pool,size_t size){
    unsigned char*      m;
    test_ngx_pool_t*    p;

    p=pool->current;

    do{
        m=p->d.last;
        m=test_ngx_align_ptr(m,TEST_NGX_POOL_ALIGNMENT);
        if((size_t)(p->d.end-m)>=size){
            p->d.last=m+size;

            return m;
        }
        p=p->d.next;
         
    }while(p);

    return test_ngx_palloc_block(pool,size);
}

static void*
test_ngx_palloc_large(test_ngx_pool_t* pool,size_t size){
    void*                   p=NULL;
    int                     n=0;
    test_ngx_pool_large_t*  large=NULL;
    
    p=calloc(1,size);
    if(p==NULL){
        return NULL; 
    }

    n=0;

    for(large=pool->large;large;large=large->next){
        if(large->alloc==NULL){
            large->alloc=p;
            return p;
        }
        if(n++>3){
            break; 
        }
    }
    //create new node
    large=test_ngx_palloc_small(pool,size);
    large->alloc=p;
    large->next=pool->large;
    pool->large=large;

    return p;
}

void*
test_ngx_palloc(test_ngx_pool_t* pool,size_t size){
    return test_ngx_palloc_large(pool,size);
}


/*
 *      free a certeain large memory node
 */
int 
test_ngx_pfree(test_ngx_pool_t* pool, void* p){
    test_ngx_pool_large_t   *l;
    for(l=pool->large;l;l=l->next){
        if(p==l->alloc){
            free(l->alloc); 
            l->alloc=NULL;
            return 0;
        } 
    }
    return -1;
}


/*
 *      cleanup outer source
 */
void
test_ngx_pool_cleanup_file(void* data){
    test_ngx_pool_cleanup_file_t    *c=data;
    close(c->fd);
}

void
test_ngx_pool_run_cleanup_file(test_ngx_pool_t *p,int fd){
    test_ngx_pool_cleanup_t         *c;
    test_ngx_pool_cleanup_file_t    *cf;

    for(c=p->cleanup;c;c=c->next){
        if(c->handler==test_ngx_pool_cleanup_file){
            cf=c->data;
            if(cf->fd==fd){
                c->handler(cf); 
                c->handler=NULL;
                return;
            }
        } 
    }
}


test_ngx_pool_cleanup_t*
test_ngx_pool_cleanup_add(test_ngx_pool_t* p, size_t size){
    test_ngx_pool_cleanup_t     *c;
    c=test_ngx_palloc(p,sizeof(test_ngx_pool_cleanup_t));
    if(c==NULL)
        return NULL;

    if(size){
        c->data=test_ngx_palloc(p,size);
        if(c->data==NULL)
            return NULL;
    }else{
        c->data=NULL; 
    }
    c->handler=NULL;
    c->next=p->cleanup;
    p->cleanup=c;

    return c;
}

