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
#define TX_BUFFER_LIMIT_RATE (0.5) //送信する必要があるとする割合
#define TX_BUFFER_SIZE  (1UL<<TX_BUFFER_SIZE_LOG2)
#define RX_BUFFER_SIZE  (1UL<<RX_BUFFER_SIZE_LOG2)

#define BAUD (115200) //転送速度
#define HIGH_SPEED (0) //高速伝送の有無
#if HIGH_SPEED
#define BRG (FCY / (4UL * BAUD) - 1)
#else
#define BRG (FCY /(16UL * BAUD) - 1)
#endif

//キュー実装
static uint8_t rx_buf[RX_BUFFER_SIZE];
static uint8_t tx_buf[TX_BUFFER_SIZE];
static ring2_t tx_ring;
static ring2_t rx_ring;
//定数
static size_t tx_limit = (size_t) ((float) TX_BUFFER_SIZE*TX_BUFFER_LIMIT_RATE);
//動作状況
static bool volatile busy = false;

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
    //U1STAbits.UTXEN = true; //use transmit pins
    U1STAbits.URXISEL = 0b00; //interrupt at 1 data
    U1STAbits.UTXISEL1 = false, U1STAbits.UTXISEL0 = true; //shift register and FIFO empty
    //interrupt sequence
    IFS0bits.U1TXIF = false;
    IFS0bits.U1RXIF = false;
    IEC0bits.U1RXIE = true;
    IEC0bits.U1TXIE = true;
    IPC3bits.U1TXIP = UART_TX_PRI;
    IPC2bits.U1RXIP = UART_RX_PRI;
    //begin uart module
    U1MODEbits.UEN = 0b00; //module activate on using tx and rx pins
    U1MODEbits.UARTEN = true; //transmit pin activate

}

bool uart_empty() {
    return ring2_empty(&rx_ring);
}

char uart_putc(char c) {
    IEC0bits.U1TXIE = false;
    ring2_putc(&tx_ring, c);
    if (ring2_used(&tx_ring) > tx_limit)uart_flush();
    IEC0bits.U1TXIE = true;
    return c;
}

const uint8_t* uart_write(const uint8_t* byte, size_t size) {
    IEC0bits.U1TXIE = false;
    ring2_write(&tx_ring, byte, size);
    if (ring2_used(&tx_ring) > tx_limit)uart_flush();
    IEC0bits.U1TXIE = true;
    return byte;
}

const char* uart_puts(const char* str) {
    IEC0bits.U1TXIE = false;
    ring2_puts(&tx_ring, str);
    if (ring2_used(&tx_ring) > tx_limit)uart_flush();
    IEC0bits.U1TXIE = true;
    return str;
}

const char* uart_putl(const char* str) {
    IEC0bits.U1TXIE = false;
    ring2_putl(&tx_ring, str);
    if (ring2_used(&tx_ring) > tx_limit)uart_flush();
    IEC0bits.U1TXIE = true;
    return str;
}

char uart_getc() {
    char c;
    IEC0bits.U1RXIE = false;
    c = !ring2_empty(&rx_ring) ? ring2_getc(&rx_ring) : 0;
    IEC0bits.U1RXIE = true;
    return c;
}

uint8_t* uart_read(uint8_t* byte, size_t size) {
    uint8_t *result;
    IEC0bits.U1RXIE = false;
    result = ring2_read(&rx_ring, byte, size);
    IEC0bits.U1RXIE = true;
    return result;
}

char* uart_gets(char* str, size_t size) {
    char *result;
    IEC0bits.U1RXIE = false;
    result = ring2_gets(&rx_ring, str, size);
    IEC0bits.U1RXIE = true;
    return result;
}

void uart_flush() {
    if (!busy) {
        volatile char c;
        U1STAbits.UTXEN = true; //use transmit pins
        while ((!U1STAbits.UTXBF)&&(ring2_used(&tx_ring) > 0)) {
            c = ring2_getc(&tx_ring);
            U1TXREG = c;
        }
        busy = true;
    }
}

void __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt(void) {
    volatile char c;
    while (U1STAbits.URXDA) {
        //check error
        if (U1STAbits.OERR) {
            U1STAbits.OERR = false;
        } else {
            c = U1RXREG;
            ring2_putc(&rx_ring, c);
        }
    }
    IFS0bits.U1RXIF = false;
}

void __attribute__((interrupt, no_auto_psv)) _U1TXInterrupt(void) {
    volatile char c;
    while ((!U1STAbits.UTXBF)&&(ring2_used(&tx_ring) > 0)) {
        c = ring2_getc(&tx_ring);
        U1TXREG = c;
    }
    busy = ring2_used(&tx_ring) > 0;
    IFS0bits.U1TXIF = false;
}
