#include <stdio.h>
#include <string.h>

#include "test_ngx_array.h"

typedef struct{
    char str[15];
    int num;
}test_array_t;

int main(){
    int i=0; 
    test_ngx_pool_t* pool_g;
    test_ngx_array_t* arr_g;
    test_array_t arr1,arr2,arr3,arr4;
    test_array_t *tmp_arr;
    
    //init memory pool
    pool_g=test_ngx_create_pool(1024);
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
    
    //init ngx_array
    arr_g=test_ngx_array_create(pool_g,10,sizeof(test_array_t));
    tmp_arr=test_ngx_array_push(arr_g);
    memcpy(tmp_arr,&arr1,sizeof(test_array_t));

    //push element to array
    tmp_arr=test_ngx_array_push_n(arr_g,2);
    memcpy(&tmp_arr[0],&arr2,sizeof(test_array_t));
    memcpy(&tmp_arr[1],&arr3,sizeof(test_array_t));

    tmp_arr=test_ngx_array_push(arr_g);
    memcpy(tmp_arr,&arr4,sizeof(test_array_t));

    //traverse array and print data
    for(i=0;i<arr_g->nelts;i++){
        printf("str:%s\n",((test_array_t*)arr_g->elts)[i].str);
        printf("num:%d\n",((test_array_t*)arr_g->elts)[i].num);
    }

    //deinit
    test_ngx_array_destroy(arr_g);
    test_ngx_destroy_pool(pool_g);

    return 0;
}
