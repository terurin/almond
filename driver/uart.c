#include "uart.h"
#include <p33Fxxxx.h>
#include <util/ring.h>
#include "port.h"
//領域設定用定数
#define TX_BUFFER_SIZE_LOG2 (6)
#define RX_BUFFER_SIZE_LOG2 (6)
#define TX_BUFFER_SIZE  (1UL<<TX_BUFFER_SIZE_LOG2)
#define TX_BUFFER_LIMIT (0.30) //送信する必要があるとする割合
#define RX_BUFFER_SIZE ((1UL)<<RX_BUFFER_SIZE_LOG2)
#define FCY (40000000)
#define BAUD (115200) //転送速度
#define HIGH_SPEED (false) //高速伝送の有無

static uint8_t tx_buf [TX_BUFFER_SIZE];
static ring2_t tx_ring;
static const uint16_t tx_limit;
static uint8_t rx_buf [RX_BUFFER_SIZE];
static ring2_t rx_ring;
//割り込みの優先度(0~7)
static const uint16_t tx_ip = 3;
static const uint16_t rx_ip = 2;

//送信する必要があるか?
static inline bool sendable() {
    return ring2_used(&tx_ring)<(TX_BUFFER_SIZE * TX_BUFFER_LIMIT);
}
static void flush();

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

#if HIGH_SPEED == true
    const uint16_t brg = FCY / (16 * BAUD) - 1;
#else
    const uint16_t brg = FCY / (4 * BAUD) - 1;
#endif
    
    //管理領域初期化
    ring2_init(&tx_ring, tx_buf, TX_BUFFER_SIZE_LOG2);
    ring2_init(&rx_ring, rx_buf, RX_BUFFER_SIZE_LOG2);

    //接続
    pin_dout(PIN_TX);
    ppso_assign(PIN_TX, PPSO_U1TX);
    pin_din(PIN_RX);
    ppsi_assign(PIN_RX, PPSI_U1RX);

    //Uartの各種設定
    U1MODEbits.UARTEN = false; //一応、モジュールの電源を切る。
    U1STAbits = status;
    U1MODEbits = mode;
    U1BRG = brg;
    
    //割り込み
    IEC0bits.U1TXIE = IEC0bits.U1RXIE =false;//割り込み無効化
    IFS0bits.U1TXIF = IFS0bits.U1RXIF =false;//割り込みフラグ解除
    IPC3bits.U1TXIP =tx_ip,IPC2bits.U1RXIP = rx_ip;//割り込み優先度設定
    IEC0bits.U1TXIE =false, IEC0bits.U1RXIE =true;//割り込み有効化
}

char uart_putc(char c){
    char result=ring2_putc(&tx_ring,c);
    if (sendable())flush();
    return result;
}

const uint8_t* uart_write(const uint8_t* byte,size_t size){
    const uint8_t* result=ring2_write(&tx_ring,byte,size);
    if (sendable())flush();
    return result;
}

const char* uart_puts(const char* str){
    const char* result=ring2_puts(&tx_ring,str);
    if (sendable())flush();
    return result;
}

bool uart_empty(){
    return ring2_empty(&rx_ring);
}

char uart_getc(){
    return ring2_getc(&rx_ring);
}

uint8_t* uart_read(uint8_t* byte,size_t size){
    return ring2_read(&rx_ring,byte,size);
}

char* uart_gets(char* str,size_t size){
    return ring2_gets(&rx_ring,str,size);
}

void uart_flush(){
    flush();
}

static void flush(){
    bool necessary = (!IEC0bits.U1TXIE)&& //Hardware FIFO
            (!ring2_empty(&tx_ring));//Software FIFO
    if (necessary){
        while (!U1STAbits.UTXBF&&!ring2_empty(&tx_ring)){
            U1TXREG=ring2_getc(&tx_ring);
        }
        IEC0bits.U1TXIE=true;
    }
}

void _ISR _U1RXInterrupt(){
    do {
        if (!ring2_full(&rx_ring)){
            ring2_putc(&rx_ring,U1RXREG);
        }
    }while (U1STAbits.URXDA);
    IFS0bits.U1RXIF=false;
}

void _ISR _U1TXInterrupt(){
    while (!ring2_empty(&tx_ring)&& U1STAbits.UTXBF == false) {
        U1TXREG=ring2_getc(&tx_ring);
    }

    IEC0bits.U1TXIE = !ring2_empty(&tx_ring);
    IFS0bits.U1TXIF = false;
}
