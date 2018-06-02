#include "spi.h"
#include <p33Fxxxx.h>
#include <stdbool.h>
#include "pin.h"
#include "port.h"
#include "config.h"
#include <util/ring.h>
static const uint16_t error_code = 0xffff;

#define TX_SIZE_LOG2 (4)
#define RX_SIZE_LOG2 (4)

uint8_t tx_buf[1u<<TX_SIZE_LOG2];
uint8_t rx_buf[1u<RX_SIZE_LOG2];

ring2_t tx_ring;
ring2_t rx_ring;

void spi_init(){
    //定数を設定する
    const SPI1STATBITS sta = {
        .SPIEN =false, // 
        .SPISIDL =false, // sleep at idle
        .SPIROV=false, //overflow?
        .SPITBF =false ,//is full of transmit buffer?
        .SPIRBF =false // is full of receive buffer?
    };
    
    const SPI1CON1BITS con1 ={
      .DISSCK=false,  //use sck
      .DISSDO=false, //use sdo
      .MODE16=true, //use 16bit mode
      .SMP=false, //for slave mode (at slave mode,be false)
      .CKE = true, //fetch when clock fall down
      .SSEN =true, //use ss pin
      .CKP = true, //idel level is High
      .MSTEN=false ,//slave mode
    };
    
    const SPI1CON2BITS con2 ={
        .FRMEN =false //use frame mode
        //.SPIFSD =true,//direction of frame, for slave
    };

    //管理領域を設定する  
    ring2_init(&tx_ring,tx_buf,TX_SIZE_LOG2);
    ring2_init(&rx_ring,rx_buf,RX_SIZE_LOG2);
    
    //IOを接続する
    pin_din(PIN_CS);
    pin_set_ppsi(PIN_CS,PPSI_SS);
    pin_din(PIN_MCLK);
    pin_set_ppsi(PIN_MCLK,PPSI_SCK);
    pin_din(PIN_MOSI);
    pin_set_ppsi(PIN_MOSI,PPSI_SDI);
    pin_dout(PIN_MISO);    
    pin_set_ppso(PIN_MISO,PPSO_SDO1);
    
    //SPIモジュールを設定する
    SPI1STATbits=sta;
    SPI1CON1bits=con1;
    SPI1CON2bits=con2;
    
    //割り込み設定
    IFS0bits.SPI1EIF=false;//interrupt of spi error
    IFS0bits.SPI1IF=false;
    IPC2bits.SPI1EIP=SPI_ERROR_PRI;
    IPC2bits.SPI1EIP=SPI_TRANCEMIT_PRI;
    IEC0bits.SPI1EIE=true;
    IEC0bits.SPI1IE=false;//not use
    
    //SPIモジュールを起動する
    SPI1STATbits.SPIEN=true;
    
}

uint16_t spi_putw(uint16_t w){
    return ring2_putw(&tx_ring,w);
}

bool spi_full(){
    return ring2_full(&tx_ring);
}

uint16_t spi_getw(){
    return ring2_getw(&rx_ring);
}
bool spi_empty(){
    return ring2_used(&rx_ring)<sizeof(uint16_t);
}

//異常発生時(今は使っていない)
void __attribute__((interrupt, auto_psv)) _SPI1ErrInterrupt(){
    IFS0bits.SPI1EIF=false;
}

//通常転送時
void __attribute__((interrupt, auto_psv)) _SPI1Interrupt(){
    //read and write
    ring2_putw(&rx_ring,SPI1BUF);
    SPI1BUF= ring2_getw(&rx_ring);
    //clear flag
    IFS0bits.SPI1IF=false;
}