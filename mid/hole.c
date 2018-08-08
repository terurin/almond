#include "hole.h"
#include <math.h>
#include <driver/pin.h>
#include <driver/port.h>
#include <driver/timer.h>
#include <driver/cn.h>
#include <util/qmath.h>

static port_t ha, hb, hc;
#define M_PI (3.14159265359)
const static q0708_t rad_per_hole=QCAST(M_PI/3,8);
const static q1516_t rad_per_hole_32=QCAST(M_PI/3,16);
static q0708_t speed=0;


static void hole_event();
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

int hole_sense_raw(){
    bool c = port_read(hc);
    bool b = port_read(hb);
    bool a = port_read(ha);
    
    return  c<< 2 | b <<1| a; //0~7は自明
}

hole_t hole_sense() {
    //テーブルを引く
    uint16_t id = port_read(hc) << 2 | port_read(hb)<<1 | port_read(ha); //0~7は自明
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

static void hole_event(){
    static hole_t hole_last=HOLE_END;
    static tick_t tick_last=0;
    //データを取り込む
    tick_t tick=timer23_tick();
    hole_t hole = hole_sense();
    //速度を計算
    q1516_t delta = rad_per_hole_32*hole_diff(hole,hole_last);
    usec_t usec = tick_cast_usec(tick_diff(tick,tick_last));
    speed = delta/usec;
    //更新
    hole_last=hole;
    tick_last=tick;
}

q0708_t hole_rad(){
    return hole_sense()*rad_per_hole;
}

q0708_t hole_speed(){
    return speed;
}