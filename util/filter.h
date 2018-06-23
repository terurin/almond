//作成　terurin
//用途　デジタルフィルタ

#ifndef __UTIL_FILTER_HEADER_GUARD__
#define __UTIL_FILTER_HEADER_GUARD__

#include <stddef.h>
#include <stdint.h>

extern uint32_t filter_dot(const uint16_t* a,const uint16_t* b,size_t size);
extern uint16_t filter_fir(const uint16_t* data,const uint16_t* filter,size_t size);




#endif