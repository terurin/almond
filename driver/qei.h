/* 作成　terurin
 * 用途　QEIエンコーダーとの接続
 */

#pragma once
#ifndef DRIVER_QEI_HEADER_GUARD
#define DRIVER_QEI_HEADER_GUARD

#include <stdint.h>
#include <stdbool.h>
void qei_init();
int16_t qei_read();
int16_t qei_load();
bool qei_direction();//現在の進行方向
#endif