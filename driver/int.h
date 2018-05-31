/* 作成者　terurin
 * 用途　INT割り込み
 */

#pragma once
#ifndef DRIVER_INT_HEADER_GUARD
#define DRIVER_INT_HEADER_GUARD
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

//通常割り込み
enum int_name{
    INT_ID_0=0,
    INT_ID_1,
    INT_ID_2,
    INT_ID_END //終端処理用
};
typedef enum int_name int_id;
enum int_mode{
    INT_MODE_RASING,
    INT_MODE_FALLING,
};
typedef enum int_mode int_mode_t;
typedef void(*int_handle_t)(int_id,void*);
void int_init();
//INT割り込みを設定する
//制約　int_initを実行すること
void int_config(int_id,int_mode_t,uint8_t priority);
//INT割り込みを設定する
//制約　int_configを実行すること
void int_event(int_id,int_handle_t,void*);
#endif