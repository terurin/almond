#include "cn.h"
#include <p33Fxxxx.h>
#include <util/bits.h>
//状態変化割り込み
static change_handle_t callback_handle = NULL;
static void* callback_object = NULL;

void change_init() {
    //管理領域
    callback_handle = NULL;
    callback_object = NULL;
    //プルアップ、割り込みを無効化
    CNPU1=CNPU2=0;
    CNEN1=CNEN2=0;
    //割り込みを設定する
    IFS1bits.CNIF = false;
    IEC1bits.CNIE = false;
    IPC4bits.CNIP = 6; //割り込み優先度
}

void change_pull_up(pin_t pin, bool sw) {
    if (!pin_has_change(pin))return;
    change_id num = pin_cast_change(pin);
    volatile uint16_t * target = !(num & 0x10) ? &CNPU1 : &CNPU2;
    bits_write_reg(target, num & 0xf, sw);
}

void change_assign(pin_t pin, bool sw) {
    if (!pin_has_change(pin))return;
    uint16_t num = pin_cast_change(pin);
    volatile uint16_t * target = !(num & 0x10) ? &CNEN1 : &CNEN2;
    bits_write_reg(target, num & 0xf, sw);
}

void change_event(change_handle_t hwnd, void* obj) {
    callback_handle = hwnd;
    callback_object = obj;
    IFS1bits.CNIF = false;
    IEC1bits.CNIE = (hwnd != NULL);
}

void __attribute__((interrupt, no_auto_psv)) _CNInterrupt() {
    if (callback_handle != NULL) {
        callback_handle(callback_object);
    }
    //割り込み解除
    uint16_t dummy;
    dummy = PORTA;
    dummy = PORTB;
    dummy = PORTC;
    IFS1bits.CNIF = false;
}