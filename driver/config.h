// 作成者　terurin
// 用途　まとめて管理したいパラメータを記述する

#ifndef DRIRVER_CONFIG_HEADER_GUARD
#define DRIRVER_CONFIG_HEADER_GUARD

/* 技術ノート
 *　設定可能な割り込み優先度は0~7まで
 */


//SPI 割り込み優先度
#define SPI_TRANCEMIT_PRI (4) 
#define SPI_ERROR_PRI (5) //使っていない

//UART 割り込み優先度
#define UART_TX_PRI (2) 
#define UART_RX_PRI (3)

//PWM 割り込み優先度
#define PWM_PRI (7)

//状態変化割り込み優先度
#define CN_PRI (6)

//INT 割り込み優先度

//TIMER 割り込み優先度
#define TMR23_PRI (1)


#endif