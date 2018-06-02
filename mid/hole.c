#include "hole.h"
#include <driver/pin.h>
#include <driver/port.h>
#include <driver/cn.h>

static void hole_event(void*);
static port_t ha, hb, hc;

static const hole_t hole_table[8] = {
    HOLE_ERROR, //000
    HOLE_A, //001
    HOLE_B, //010
    HOLE_AB, //011
    HOLE_C, //100
    HOLE_CA, //101
    HOLE_BC, //110
    HOLE_ERROR, //111
};

static hole_t last;

void hole_init() {
    //IOの初期化
    pin_din(PIN_HA);
    change_pull_up(PIN_HA, true);
    pin_din(PIN_HB);
    change_pull_up(PIN_HB, true);
    pin_din(PIN_HC);
    change_pull_up(PIN_HC, true);

    //管理領域の初期化
    ha = pin_cast_port(PIN_HA);
    hb = pin_cast_port(PIN_HB);
    hc = pin_cast_port(PIN_HC);
    last = hole_sense();

    //割り込みの有効化
    change_assign(PIN_HA, true);
    change_assign(PIN_HB, true);
    change_assign(PIN_HC, true);
    change_event(hole_event, NULL);
}

hole_t hole_sense() {
    //テーブルを引く
    uint16_t id = port_read(hc) << 2 | port_read(hb) | port_read(ha); //0~7は自明
    return hole_table[id];
}

int hole_diff(hole_t now, hole_t last) {
    if (now < HOLE_END && last < HOLE_END) {
        int d = ((int) now - (int) last + 6) % 6; //0~5
        if (d < 3) {
            return d;
        } else if (d > 3) {
            return d - 6;
        }
    }
    return 0;

}

static void hole_event(void* obj) {

}