#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "test_ngx_list.h"

typedef struct{
    char str[15];
    int num;
}test_list_t;

int main(){
    int i=0;
 
    test_ngx_list_part_t* part;
    test_list_t* tmp_list;
    test_list_t arr1,arr2,arr3,arr4;
    test_ngx_pool_t* pool_g;
    test_ngx_list_t* list_g;

    pool_g=test_ngx_create_pool(1024);

    //init memory pool
    if(!pool_g){
        printf("fail to create pool\n");
        return -1;
    }

    //init test array
    strcpy(arr1.str,"i'm str1");
    arr1.num=1;

    strcpy(arr2.str,"i'm str2");
    arr2.num=2;

    strcpy(arr3.str,"i'm str3");
    arr3.num=3;

    strcpy(arr4.str,"i'm str4");
    arr4.num=4;
    
    list_g=test_ngx_list_create(pool_g,2,sizeof(test_list_t));

    tmp_list=test_ngx_list_push(list_g);
    memcpy(tmp_list,&arr1,sizeof(test_list_t));

    tmp_list=test_ngx_list_push(list_g);
    memcpy(tmp_list,&arr2,sizeof(test_list_t));

    tmp_list=test_ngx_list_push(list_g);
    memcpy(tmp_list,&arr3,sizeof(test_list_t));

    tmp_list=test_ngx_list_push(list_g);
    memcpy(tmp_list,&arr4,sizeof(test_list_t));

    part=&list_g->part;
    tmp_list=part->elts;
    
    for(i=0;;i++){
        if(i>=part->nelts){
            if(part->next==NULL){
                break; 
            } 
            part=part->next;
            tmp_list=part->elts;
            i=0;
        }
        printf("str:%s\n",tmp_list[i].str);
        printf("num:%d\n",tmp_list[i].num);
    }
    test_ngx_destroy_pool(pool_g);
    
    return 0;
}
