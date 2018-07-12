#include "uart.h"
#include <p33Fxxxx.h>
#include <util/ring.h>
#include "clock.h"
#include "port.h"
#include "config.h"
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

void uart_init() {
    //各種定数
    const U1MODEBITS mode = {
        .UARTEN = true, //module enable
        .USIDL = false, //at idel,active
        .IREN = false, //IrDA is disable
        .RTSMD = false, //RTS is simplex
        .UEN = 0b00, //Tx,Rx activate, CTS ,RTS not use
        .WAKE = false, //not wake up by fall down
        .LPBACK = false, //not loop-back
        .ABAUD = false, //auto rate defect disable
        .URXINV = false, //When Idel level is '1'
        .BRGH = HIGH_SPEED, //High Speed Mode
        .PDSEL = 0b00, //8bit , non-parity
        .STSEL = false// Stop bit width is 1 bit
    };

    const U1STABITS status = {
        .UTXISEL1 = 1, .UTXISEL0 = 0, //at tx-fifo & shift register is empty happen,interrupt
        .UTXINV = false, //not invert
        .UTXEN = true, // Tx pin enable
        .URXISEL = 0b10, //3 byte received happen interrupt
        .ADDEN = false, //Address detect is disable
    };
    
    //接続
    pin_dout(PIN_RX);
    pin_set_ppso(PIN_RX, PPSO_U1TX);
    pin_din(PIN_TX);
    pin_set_ppsi(PIN_TX, PPSI_U1RX);

    //Uartの各種設定
    U1MODEbits.UARTEN = false; //一応、モジュールの電源を切る。
    U1STAbits = status;
    U1MODEbits = mode;
    U1BRG = BRG;
    //割り込み
    IEC0bits.U1TXIE = IEC0bits.U1RXIE =false;//割り込み無効化
    IFS0bits.U1TXIF = IFS0bits.U1RXIF =false;//割り込みフラグ解除
    IPC3bits.U1TXIP =UART_TX_PRI;IPC2bits.U1RXIP = UART_RX_PRI;//割り込み優先度設定
    IEC0bits.U1TXIE =false, IEC0bits.U1RXIE =true;//割り込み有効化
}

char uart_putc(char c){
    /*char result=ring2_putc(&tx_ring,c);
    if (sendable())flush();*/
    while (U1STAbits.UTXBF);
    U1TXREG =c; 
    return c ;
}