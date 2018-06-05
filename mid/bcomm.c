#include "bcomm.h"
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <util/bits.h>

static bcomm_getc_t bcget;
static bcomm_putc_t bcput;
static bcomm_rdy_t bcrdy;
//内部バッファ
#define BUFFER_SIZE 8
static uint16_t word;
static bool word_comp;

struct bcomm_cmd{
    bool is_write;//書き込み?
    bool is_stream;//対象のアドレスを変化させるか?
    uint8_t start;//仮想ワードアドレス(6bit)
    uint8_t size;//ワード長(6bit)
};
typedef struct bcomm_cmd bcomm_cmd_t;

static inline bcomm_cmd_t command_decoder(uint16_t w){
     const bcomm_cmd_t cmd={
         .is_write = bits_read(w,15),
         .is_stream = bits_read(w,14),
         .start =bits_read_width(w,6,6),
         .size =bits_read_width(w,6,0)
     };
     return cmd;
}

void bcomm_init() {
    //管理領域の初期化
    bcrdy=NULL;
    bcget=NULL;
    bcput=NULL;
    word=0;
    word_comp=false;
}

void bcomm_assign(bcomm_rdy_t rdy, bcomm_getc_t get, bcomm_putc_t put) {
    bcrdy=rdy;
    bcget=get;
    bcput=put;
}

static void word_decoder(uint16_t);


void bcomm_process(){
    while (bcrdy()){
       word=(word<<8)+(uint16_t)bcget();
       //1Word文の情報が完成した?
       word_comp=!word_comp;
       if (!word_comp)word_decoder(word);
    } 
}

static void word_decoder(uint16_t w){
    
    
    
    
    
}
