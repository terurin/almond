/* 作成者　terurin
 * 用途　状態変化通知ピンの設定
 */
#pragma once
#ifndef DRIVER_CN_HAEDER_GUARD
#define DRIVER_CN_HAEDER_GUARD

#include "pin.h"
//状態変化割り込みについて

//プルアップについて
//割り込み登録
typedef void(*change_handle_t) (void*);
void change_init();
void change_pull_up(pin_t, bool);//true...抵抗を有効化
void change_assign(pin_t, bool);
void change_event(change_handle_t, void*);





#endif
