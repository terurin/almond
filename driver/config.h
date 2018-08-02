// 作成者　terurin
// 用途　まとめて管理したいパラメータを記述する

#ifndef DRIRVER_CONFIG_HEADER_GUARD
#define DRIRVER_CONFIG_HEADER_GUARD

/* 技術ノート
 *　設定可能な割り込み優先度は0~7まで
 */

//SPI 割り込み優先度
#define SPI_TRANCEMIT_PRI (3) 
#define SPI_ERROR_PRI (3) //使っていない

//UART 割り込み優先度
#define UART_TX_PRI (2) 
#define UART_RX_PRI (2)

//PWM 割り込み優先度
#define PWM_PRI (6)

//状態変化割り込み優先度
#define CN_PRI (1)

//INT 割り込み優先度

//TIMER 割り込み優先度
#define TMR23_PRI (2)

//ADC 割り込み優先度
#define ADC_PRI (3)

//DRV8332 異常通知
#define INT_OTW (7)
#define INT_FAULT (7)

//Clock設定

#define FOSC (80000000UL)
#define FCY (FOSC/2)

#define EXTERNAL_CLOCK (1) //外部クロックを使うか? 0/1

#endif