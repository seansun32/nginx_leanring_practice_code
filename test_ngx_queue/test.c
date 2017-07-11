#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

#include "test_ngx_queue.h"



int main(){
    test_ngx_queue_t queue_g;
    test_ngx_queue_t *q, *prev, *next, tail; 

    int i=0;
    test_ngx_queue_node_t *tmp_node=NULL;

    //init queue
    test_ngx_queue_init(&queue_g);

    
    //set value to queue node, and add node to queue
    for(i=0;i<10;i++){
        tmp_node=calloc(1,sizeof(test_ngx_queue_node_t));
        tmp_node->val=i;
        
        if(i%2==0){
            test_ngx_queue_insert_head(&queue_g,&tmp_node->queue);
        }else{
            test_ngx_queue_insert_tail(&queue_g,&tmp_node->queue); 
        }
        tmp_node=NULL;
    }

    q=test_ngx_queue_head(&queue_g);
    if(q==test_ngx_queue_last(&queue_g)){
        printf("no node\n");
        return -1;
    }


    //traverse queue, delete node with value 5
    for(q=test_ngx_queue_head(&queue_g);q!=test_ngx_queue_sentinel(&queue_g);q=next){
        //prev=test_ngx_queue_prev(q);
        next=test_ngx_queue_next(q);
        tmp_node=test_ngx_queue_data(q,test_ngx_queue_node_t,queue);
        if(tmp_node->val==5){
            test_ngx_queue_remove(&tmp_node->queue);
            free(tmp_node);
        }
    }

    
    //traverse queue, print value of each node
    for(q=test_ngx_queue_head(&queue_g);q!=test_ngx_queue_sentinel(&queue_g);q=next){
        next=test_ngx_queue_next(q);
        tmp_node=test_ngx_queue_data(q,test_ngx_queue_node_t,queue);
        printf("node value:%d\n",tmp_node->val);
    }


    //split queue at value=4
    for(q=test_ngx_queue_head(&queue_g);q!=test_ngx_queue_sentinel(&queue_g);q=next){
        next=test_ngx_queue_next(q);
        tmp_node=test_ngx_queue_data(q,test_ngx_queue_node_t,queue);
        if(tmp_node->val==4){
            test_ngx_queue_split(&queue_g,&tmp_node->queue,&tail);//tail is the new sentinel node for new splited list 
            break;
        }
    }
    for(q=test_ngx_queue_head(&queue_g);q!=test_ngx_queue_sentinel(&queue_g);q=next){
        next=test_ngx_queue_next(q);
        tmp_node=test_ngx_queue_data(q,test_ngx_queue_node_t,queue);
        printf("split-1 node value:%d\n",tmp_node->val); 
    }
    for(q=test_ngx_queue_head(&tail);q!=test_ngx_queue_sentinel(&tail);q=next){
        next=test_ngx_queue_next(q);
        tmp_node=test_ngx_queue_data(q,test_ngx_queue_node_t,queue);
        printf("split-2 node value:%d\n",tmp_node->val); 
    }


    //add queue of these two split queue
    test_ngx_queue_add(&queue_g,&tail);
    for(q=test_ngx_queue_head(&queue_g);q!=test_ngx_queue_sentinel(&queue_g);q=next){
        next=test_ngx_queue_next(q);
        tmp_node=test_ngx_queue_data(q,test_ngx_queue_node_t,queue);
        printf("after add node value:%d\n",tmp_node->val); 
    }
     

    //destroy queue
    for(q=test_ngx_queue_head(&queue_g);q!=test_ngx_queue_sentinel(&queue_g);q=next){
        next=test_ngx_queue_next(q);
        tmp_node=test_ngx_queue_data(q,test_ngx_queue_node_t,queue);
        test_ngx_queue_remove(&tmp_node->queue);
        free(tmp_node);
    }
    if(test_ngx_queue_empty(&queue_g)){
        printf("queue is empty now\n"); 
    }else{
        printf("queue is not empty\n"); 
    }

    return 0;

}
