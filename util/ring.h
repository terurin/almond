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
typedef struct ring2* ring2_ptr;
void ring2_init(ring2_ptr obj,uint8_t*mem,uint16_t size_log2);

//property
static inline uint16_t ring2_used(const ring2_ptr obj){
    return obj!=NULL?obj->used:0;
}

static inline uint16_t ring2_free(const ring2_ptr obj){
    return obj!=NULL? obj->size-obj->used:0;
}

static inline bool ring2_full(const ring2_ptr obj){
    return ring2_free(obj)==0;
}

static inline bool ring2_empty(const ring2_ptr obj){
    return obj!=NULL?!obj->used:true;
}

static inline uint8_t ring2_in(const ring2_ptr obj){
    return obj!=NULL?obj->buf[obj->in]:0;
}

static inline uint8_t ring2_out(const ring2_ptr obj){
    return obj!=NULL?obj->buf[obj->out]:0;
}

static inline uint8_t ring2_at(const ring2_ptr obj,uint16_t pos){
     return obj!=NULL?obj->buf[obj->in+pos]:0;
}

uint16_t ring2_find(const ring2_ptr obj,char c);
static inline bool ring2_exist(const ring2_ptr obj,char c){
    return ring2_find(obj,c)<ring2_used(obj);
}

static inline bool ring2_writable(const ring2_ptr ring,size_t size){
    return ring2_free(ring)>=size;
}

static inline bool ring2_readable(const ring2_ptr ring,size_t size){
    return ring2_used(ring)>=size;
}

//書き込み
char ring2_putc(ring2_ptr obj,char);
const uint8_t* ring2_write(ring2_ptr obj,const uint8_t* ,size_t);
const char* ring2_puts(ring2_ptr obj,const char* str);
const char* ring2_putl(ring2_ptr obj,const char* str);
int ring2_putw(ring2_ptr obj,int);//intは基本的にword長になっているはず...

//読み込み
char ring2_getc(ring2_ptr obj);
uint8_t* ring2_read(ring2_ptr obj,uint8_t* ,size_t);
char* ring2_gets(ring2_ptr obj,char *mem,size_t);//改行文字があるなら読み込み
int ring2_getw(ring2_ptr ring);//intは基本的にword長になっているはず...
#endif