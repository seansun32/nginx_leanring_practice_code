#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>

#include "test_ngx_mempool.h"

int main(){
    test_ngx_pool_t* pool;
    test_ngx_data_t tmp_test_data1,tmp_test_data2;
    int fd=-1;
    test_ngx_pool_cleanup_t* tmp_cleanup=NULL;
    test_ngx_pool_cleanup_file_t* tmp_cf=NULL;

    //fist create pool
    pool=test_ngx_create_pool(256);
    if(!pool){
        printf("fail to create\n");
        return -1;
    }
    tmp_test_data1.pool=pool;
    tmp_test_data2.pool=pool;

    //test alloc memory
    tmp_test_data1.data=test_ngx_palloc(tmp_test_data1.pool,20);
    if(!tmp_test_data1.data){
        printf("error\n");
        return -1;
    }
    tmp_test_data2.data=test_ngx_palloc(tmp_test_data2.pool,30); 
    strcpy(tmp_test_data1.data,"this is test data1"); 
    strcpy(tmp_test_data2.data,"this is test data2");

    printf("%s\n",tmp_test_data1.data);
    printf("%s\n",tmp_test_data2.data);

    //test ngx_pfree
    test_ngx_pfree(tmp_test_data2.pool,tmp_test_data2.data);
    tmp_test_data2.data=NULL;

    //test cleanup
    fd=open("/tmp/test_ngx.txt",O_RDWR); 
    if(fd<0){
        printf("fail to open file\n"); 
        test_ngx_destroy_pool(pool);
        
        return -1;
    }
    printf("success open file\n");
    
    tmp_cleanup=test_ngx_pool_cleanup_add(tmp_test_data1.pool,sizeof(test_ngx_pool_cleanup_file_t));
    if(tmp_cleanup){
        printf("success create cleanup\n");
        tmp_cleanup->handler=test_ngx_pool_cleanup_file;
        tmp_cf=tmp_cleanup->data;
        tmp_cf->fd=fd;
        test_ngx_pool_run_cleanup_file(tmp_test_data1.pool,fd);
        if(tmp_cleanup->handler){
            printf("fail to clean file\n"); 
        }else{
            printf("success clean file\n"); 
        }
    }
    
    test_ngx_destroy_pool(pool);

    return 0;
}
