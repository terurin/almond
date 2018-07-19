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
static char rx_buf[RX_BUFFER_SIZE];
static char tx_buf[TX_BUFFER_SIZE];
static ring2_t tx_ring;
static ring2_t rx_ring;


void uart_init() {
    //initialize memory
    ring2_init(&tx_ring, tx_buf, TX_BUFFER_SIZE_LOG2);
    ring2_init(&rx_ring, rx_buf, RX_BUFFER_SIZE_LOG2);

    //assign uart's pins
    pin_dout(PIN_RX);
    pin_set_ppso(PIN_RX, PPSO_U1TX);
    pin_din(PIN_TX);
    pin_set_ppsi(PIN_TX, PPSI_U1RX);

    //how to send
    U1MODEbits.PDSEL = 0b00; //8bit non party
    U1MODEbits.STSEL = false; //stop bit length is one  
    U1BRG = BRG;
    U1STAbits.UTXEN = true;//use transmit pins
    U1STAbits.URXISEL= 0b00;//interrupt at 1 data
    //interrupt sequence
    IFS0bits.U1TXIF = false;
    IFS0bits.U1RXIF = false;
    IEC0bits.U1RXIE = true;
    IEC0bits.U1TXIE = false;
    IPC3bits.U1TXIP = UART_TX_PRI;
    IPC2bits.U1RXIP = UART_RX_PRI;
    //begin uart module
    U1MODEbits.UEN = 0b00; //module activate on using tx and rx pins
    U1MODEbits.UARTEN = true; //transmit pin activate

}

bool uart_empty(){
    return ring2_empty(&rx_ring);
}

char uart_putc(char c) {
    while (U1STAbits.UTXBF);
    putchar(c);
}

char uart_getc() {
    while (uart_empty());
    return ring2_getc(&rx_ring);
}

void __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt(void) {
    volatile char c;
    while (U1STAbits.URXDA){
        //check error
        if (U1STAbits.OERR){
            U1STAbits.OERR=false;
        }else{
            c=U1RXREG;
            ring2_putc(&rx_ring,c);
        }
    }
    IFS0bits.U1RXIF = 0;
}
