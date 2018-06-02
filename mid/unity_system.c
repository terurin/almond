#include "unity_system.h"
#include <stddef.h>
#define RECORD_SIZE 16 //内部で管理する領域の大きさ
struct us_record{
    us_write_handle_t write;
    us_read_handle_t read;
    void * object;
};
typedef struct us_record us_record_t;
typedef struct us_record* us_record_ptr;
us_record_t records[RECORD_SIZE];

void us_init(){
    us_record_ptr it,last;
    for(it=records,last=records+RECORD_SIZE;it!=last;it++){
        it->object=NULL;
        it->write=NULL;
        it->read=NULL;
    }
}

void us_assign(us_id id,us_write_handle_t write,us_read_handle_t read,void *object){
    if (id<RECORD_SIZE){
        us_record_ptr it = records+id;
        it->write = write;
        it->read = read;
        it->object = object;
    }
}

static void simple_write(void* object,uint16_t value){
    *(uint16_t*)object=value;
}

static uint16_t simple_read(void* object){
    return *(uint16_t*)object;
}

void us_assign_simple(us_id id,uint16_t* object){
    us_assign(id,simple_write,simple_read,object);
}

void us_assign_simple_readonly(us_id id ,uint16_t*object){
    us_assign(id,NULL,simple_read,object);
}
void us_assign_simple_writeonly(us_id id,uint16_t* object){
    us_assign(id,NULL,simple_read,object);
}

void us_unsign(us_id id){
    us_assign(id,NULL,NULL,NULL);
}

void us_write(us_id id,uint16_t value){
    us_record_ptr it;
    if (id<RECORD_SIZE){
        it = records+id;
        if (it->write!=NULL){
            it->write(it->object,value);
        }
    }   
}

uint16_t us_read(us_id id){
    const uint16_t error = 0xFFFF;
    us_record_ptr it;
    if (id<RECORD_SIZE){
        it = records+id;
        if (it->write!=NULL){
            return it->read(it->object);
        }
    }
    return error;
}
