#include "text.h"

char text_hex_dight(uint8_t x){
    if (x<0x0A)return x+'0';
    else if(x<0x10)return x-10+'A';
    else return 'X';
}

const char* text_hex16(char* buf,uint16_t x){
    int i;
    for ( i=0;i<4;i++){
        buf[i]=text_hex_dight(x>>12);
        x<<=4;
    }
    buf[i]='\0';
    return buf;
}