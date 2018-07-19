#include "uart.h"
#include <p33Fxxxx.h>
#include <util/ring.h>
#include "clock.h"
#include "port.h"
#include "config.h"
#include <stdio.h>
//領域設定用定数
#define TX_BUFFER_SIZE_LOG2 (6)
#define RX_BUFFER_SIZE_LOG2 (6)
#define TX_BUFFER_SIZE  (1UL<<TX_BUFFER_SIZE_LOG2)
#define TX_BUFFER_LIMIT (0.30) //送信する必要があるとする割合
#define RX_BUFFER_SIZE ((1UL)<<RX_BUFFER_SIZE_LOG2)
#define BAUD (115200) //転送速度
#define HIGH_SPEED (0) //高速伝送の有無
#if HIGH_SPEED
    #define BRG (FCY / (4UL * BAUD) - 1)
#else
    #define BRG (FCY /(16UL * BAUD) - 1)
#endif

//キュー実装
static volatile uint16_t rused, rin, rout;
static volatile char rbuf[RX_BUFFER_SIZE];

static volatile uint16_t tused, tin, tout;
static volatile char tbuf[TX_BUFFER_SIZE];

static inline void pin_assign();//接続

void uart_init() {
    pin_assign();
    
    U1MODEbits.PDSEL= 0b00;//8bit non party
    U1MODEbits.STSEL=false;//stop bit length is one  
    U1BRG=BRG;
    U1STAbits.UTXEN=true;
    //interrupt sequence
    IFS0bits.U1TXIF=false;
    IEC0bits.U1RXIE=false;
    IPC2bits.U1RXIP=UART_RX_PRI;

    
    U1MODEbits.UEN=0b00;//module activate on using tx and rx pins
    U1MODEbits.UARTEN=true;//transmit pin activate
    







}

static inline void pin_assign(){
    pin_dout(PIN_RX);
    pin_set_ppso(PIN_RX, PPSO_U1TX);
    pin_din(PIN_TX);
    pin_set_ppsi(PIN_TX, PPSI_U1RX);  
}



char uart_putc(char c){
    while (U1STAbits.UTXBF);
    putchar(c);
}

