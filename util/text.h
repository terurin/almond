#pragma once
#include <stdint.h>

char text_hex_dight(uint8_t x);
const char* text_hex16(char* buf,uint16_t x);//制約　sizeof(buf)>5
