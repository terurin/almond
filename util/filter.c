#include "filter.h"

uint32_t filter_dot(const uint16_t* a,const uint16_t* b,size_t size){
    const uint16_t* end=a+size;
    uint32_t sum;
    for (sum=0;a!=end;a++,b++){
        sum+=((uint32_t)*a)*((uint32_t)(*b));
    }
    return sum;
}

uint16_t filter_fir(const uint16_t* data,const uint16_t* filter,size_t size){
    const uint16_t *end=data+size;
    uint32_t sum;
    uint16_t total;
    
    for (sum=0,total=0;data!=end;data++,filter++){
        sum+=((uint32_t)*data)*((uint32_t)(*filter));
        total+=*filter;
    }
    
    return sum/total;
}



