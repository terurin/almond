#pragma once
#ifndef UTIL_RING_HEADER_GUARD
#define UTIL_RING_HEADER_GUARD

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

//大きさを2^nにすることで高速化したリングバッファ
struct ring2 { 
    uint8_t *buf;
    uint16_t size;
    uint16_t in,out,used;
    uint16_t mask;
    uint16_t tag;
};
typedef struct ring2 ring2_t;

void ring2_init(ring2_t* obj,uint8_t*mem,uint16_t size_log2);

//property
static inline uint16_t ring2_used(const ring2_t* obj){
    return obj!=NULL?obj->used:0;
}

static inline uint16_t ring2_free(const ring2_t* obj){
    return obj!=NULL? obj->size-obj->used:0;
}

static inline bool ring2_full(const ring2_t* obj){
    return ring2_free(obj)==0;
}

static inline bool ring2_empty(const ring2_t* obj){
    return ring2_used(obj)==0;
}

static inline uint8_t ring2_in(const ring2_t *obj){
    return obj!=NULL?obj->buf[obj->in]:0;
}

static inline uint8_t ring2_out(const ring2_t *obj){
    return obj!=NULL?obj->buf[obj->out]:0;
}

static inline uint8_t ring2_at(const ring2_t *obj,uint16_t pos){
     return obj!=NULL?obj->buf[obj->in+pos]:0;
}

uint16_t ring2_find(const ring2_t *obj,char c);

//method
static inline void  ring2_clean(ring2_t *obj){
    if (obj!=NULL)obj->used = obj->in=obj->out=0;
}

//書き込み
void ring2_putc(ring2_t* obj,uint8_t);
const uint8_t* ring2_write(ring2_t* obj,const uint8_t* ,size_t);

static inline const char* ring2_puts(ring2_t* obj,const char* str){
    return (const char*)ring2_write(obj,(const uint8_t*)str,strlen(str));
}

uint8_t ring2_getc(ring2_t* obj);
uint8_t* ring2_read(ring2_t* obj,uint8_t* ,size_t);
char* ring2_gets(ring2_t* obj,char *mem,size_t);//改行文字があるなら読み込み



#endif