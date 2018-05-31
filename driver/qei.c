#include "qei.h"
#include <p33Fxxxx.h>
#include "pin.h"
#include "port.h"

void qei_init(){
    //定数
    const QEI1CONBITS con1 ={
        .CNTERR=false,
        .QEISIDL=false,//at debuging,running
        .QEIM=0b111,//QEI x4 Mode
        .SWPAB=false,//non-swap
        .PCDOUT=false,//non-use direction output
        .TQGATE=false,//timer gate disenable
    };
    const DFLT1CONBITS dflt = {
       .IMV=0b00,//at reset, qea|qeb = 0b00
       .CEID=true,//interrupt of count error disable
       .QEOUT=true,//digital filter is active
       .QECK =0b11,//digital filter size is 16
    };
    
    //IO設定
    port_din(PIN_QEI_X);
    port_ppsi(PIN_QEI_X,PPSI_QEIA);
    port_din(PIN_QEI_Y);
    port_ppsi(PIN_QEI_Y,PPSI_QEIB);
    //内部レジスタ設定
    QEI1CONbits=con1;
    DFLT1CONbits=dflt;
    POSCNT=0;
    MAXCNT=0xffff;//not use
}

int16_t qei_read(){
    return (int16_t)POSCNT;
}

int16_t qei_load(){
    int16_t result = (int16_t)POSCNT;
    POSCNT=0;
    return result;
}

bool qei_direction(){
    return QEI1CONbits.UPDN;
}