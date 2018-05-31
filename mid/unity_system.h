/* 作成 terurin
 * 用途　通信の補助及び操作の一般化
 */

#pragma once
#ifndef MID_UNITY_SYSTEM_HEADER_GUARD
#define MID_UNITY_SYSTEM_HEADER_GUARD

#include <stdint.h>
void us_init();

typedef uint8_t us_id;
typedef void (*us_write_handle_t)(void*,uint16_t);
typedef uint16_t (*us_read_handle_t)(void*);
//操作の登録
void us_assign(us_id,us_write_handle_t,us_read_handle_t,void *object);//読み込み書き込み用関数を定義
void us_assign_simple(us_id,uint16_t*);//単に読み書きをする場合
void us_assign_simple_readonly(us_id,uint16_t*);//単に読み込む場合
void us_assign_simple_writeonly(us_id,uint16_t*);//単に書き込む場合
void us_unsign(us_id);//削除
//読み込みと書き込み
void us_write(us_id,uint16_t);
uint16_t us_read(us_id);



#endif