#include "uart.h"
#include <p33Fxxxx.h>
#include <util/ring.h>
//領域設定用定数
#define TX_BUFFER_SIZE_LOG2 (6)
#define RX_BUFFER_SIZE_LOG2 (6)
#define TX_BUFFER_SIZE ((1UL)<<TX_BUFFER_SIZE_LOG2)
#define RX_BUFFER_SIZE ((1UL)<<RX_BUFFER_SIZE_LOG2)

static uint8_t tx_buf [TX_BUFFER_SIZE];
static ring2_t tx_ring;
static uint8_t rx_buf [RX_BUFFER_SIZE];
static ring2_t rx_ring;

void uart_init(){
    ring2_init(&tx_ring,tx_buf,TX_BUFFER_SIZE_LOG2);
    ring2_init(&rx_ring,rx_buf,RX_BUFFER_SIZE_LOG2);
}