#ifndef __TEST_NGX_QUEUE_H__
#define __TEST_NGX_QUEUE_H__

typedef struct test_ngx_queue_s  test_ngx_queue_t;

struct test_ngx_queue_s {
    test_ngx_queue_t  *prev;
    test_ngx_queue_t  *next;
};


#define test_ngx_queue_init(q)                                                     \
    (q)->prev = q;                                                            \
    (q)->next = q


#define test_ngx_queue_empty(h)                                                    \
    (h == (h)->prev)


#define test_ngx_queue_insert_head(h, x)                                           \
    (x)->next = (h)->next;                                                    \
    (x)->next->prev = x;                                                      \
    (x)->prev = h;                                                            \
    (h)->next = x


#define ngx_queue_insert_after   ngx_queue_insert_head


#define test_ngx_queue_insert_tail(h, x)                                           \
    (x)->prev = (h)->prev;                                                    \
    (x)->prev->next = x;                                                      \
    (x)->next = h;                                                            \
    (h)->prev = x


#define test_ngx_queue_head(h)                                                     \
    (h)->next


#define test_ngx_queue_last(h)                                                     \
    (h)->prev


#define test_ngx_queue_sentinel(h)                                                 \
    (h)


#define test_ngx_queue_next(q)                                                     \
    (q)->next


#define test_ngx_queue_prev(q)                                                     \
    (q)->prev


#define test_ngx_queue_remove(x)                                                   \
    (x)->next->prev = (x)->prev;                                              \
    (x)->prev->next = (x)->next


#define test_ngx_queue_split(h, q, n)                                              \
    (n)->prev = (h)->prev;                                                    \
    (n)->prev->next = n;                                                      \
    (n)->next = q;                                                            \
    (h)->prev = (q)->prev;                                                    \
    (h)->prev->next = h;                                                      \
    (q)->prev = n;


#define test_ngx_queue_add(h, n)                                                   \
    (h)->prev->next = (n)->next;                                              \
    (n)->next->prev = (h)->prev;                                              \
    (h)->prev = (n)->prev;                                                    \
    (h)->prev->next = h;


#define test_ngx_queue_data(q, type, link)                                         \
    (type *) ((unsigned char *) q - offsetof(type, link))


typedef struct{
    int val;
    test_ngx_queue_t queue;
}test_ngx_queue_node_t;


#endif
