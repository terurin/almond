/* 作成　terurin
 * 用途　clockの設定 
 */
#pragma once
#ifndef DRIVER_CLOCK_HEADER_GUARD
#define DRIVER_CLOCK_HEADER_GUARD
#include <stdint.h>

//制約　周辺機器の初期化の中で最初に実行すること
void clock_init();
uint32_t clock_fcy();//実行速度



#endif