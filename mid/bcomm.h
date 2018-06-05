/* 作成者　terurin
 * 用途　バイナリを用いた通信
 */

#pragma once
#ifndef MID_BCOMM_HEADER_GUARD
#define MID_BCOMM_HEADER_GUARD

#include <stdbool.h>
typedef bool (*bcomm_rdy_t)();//データが存在する
typedef char(*bcomm_getc_t)();
typedef char(*bcomm_putc_t)(char);

void bcomm_init();
//入出力先を設定する
void bcomm_assign(bcomm_rdy_t,bcomm_getc_t,bcomm_putc_t);
//定期的に実行する関数
void bcomm_process();


#endif