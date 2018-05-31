#include "int.h"
#include <p33Fxxxx.h>
#include <util/bits.h>
//INT割り込みについて

typedef struct int_pair {
    int_handle_t handle;
    void *object;
} int_pair_t;
static int_pair_t int_table[INT_ID_END];

#define _INT0IF IFS0bits.INT0IF
#define _INT1IF IFS1bits.INT1IF
#define _INT2IF IFS1bits.INT2IF

//割り込みのレジスタをリニア化する(有効?)
static inline void int_ie(int_id id, bool flag) {
    switch (id) {
        case INT_ID_0:
            IEC0bits.INT0IE = flag;
            break;
        case INT_ID_1:
            IEC1bits.INT1IE = flag;
            break;
        case INT_ID_2:
            IEC1bits.INT2IE = flag;
            break;
        default:
            break;
    }
}
//割り込みのレジスタをリニア化する(フラグ)
static inline void int_if(int_id id, bool flag) {
    switch (id) {
        case INT_ID_0:
            IFS0bits.INT0IF = flag;
            break;
        case INT_ID_1:
            IFS1bits.INT1IF = flag;
            break;
        case INT_ID_2:
            IFS1bits.INT2IF = flag;
            break;
        default:
            break;
    }
}
//割り込みのレジスタをリニア化する(優先度)
static inline void int_pri(int_id id, uint8_t pri) {
    switch (id) {
        case INT_ID_0:
            IPC0bits.INT0IP=pri;
            break;
        case INT_ID_1:
            IPC5bits.INT1IP=pri;
            break;
        case INT_ID_2:
            IPC7bits.INT2IP=pri;
            break;
        default:
            break;
    }
}

void int_init() {
    int_id idx;
    for (idx = INT_ID_0; idx < INT_ID_END; idx++) {
        int_table[idx].handle = NULL;
        int_table[idx].object = NULL;
        int_ie(idx,false);
        int_if(idx,false);
    }

}

void int_config(int_id id, int_mode_t mode, uint8_t priority) {
    //エッジを設定する。
    bits_write_reg(&INTCON2, 1 << id, mode == INT_MODE_FALLING);
    //割り込み優先度を設定する。
    int_pri(id,0x7&priority);
}

void int_event(int_id id,int_handle_t handle,void* object){
   //割り込み無効化
    int_ie(id,false);
    //割り込み処理先を記述する
    int_table[id].handle=handle;
    int_table[id].object=object;
    //割り込み有効化
    int_if(id,false);
    int_ie(id,handle!=NULL);
}

void __attribute__((interrupt, no_auto_psv)) _INT0Interrupt(){
    const int_id id = INT_ID_0;
    int_pair_t* p=&int_table[id];
    if (p->handle!=NULL){
        p->handle(id,p->object);
    }
    _INT0IF=false;
}

void __attribute__((interrupt, no_auto_psv)) _INT1Interrupt(){
    const int_id id = INT_ID_1;
    int_pair_t* p=&int_table[id];
    if (p->handle!=NULL){
        p->handle(id,p->object);
    }
    _INT0IF=false;
}

void __attribute__((interrupt, no_auto_psv)) _INT2Interrupt(){
    const int_id id = INT_ID_2;
    int_pair_t* p=&int_table[id];
    if (p->handle!=NULL){
        p->handle(id,p->object);
    }
    _INT0IF=false;
}