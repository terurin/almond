// 作成者　terurin
// 用途　ホールセンサーの状態を取得する

#ifndef MID_HOLE_HEADER_GUARD
#define MID_HOLE_HEADER_GUARD
#include <util/dsp_type.h>
enum hole{
    HOLE_A=0,
    HOLE_AB,
    HOLE_B,
    HOLE_BC,
    HOLE_C,
    HOLE_CA,
    HOLE_END,//終端処理用
    HOLE_ERROR=HOLE_END
};
typedef enum hole hole_t;

void hole_init();

int hole_sense_raw();
hole_t hole_sense();
int hole_diff(hole_t now,hole_t last);//進行度を得る

q0708_t hole_rad();//ホールセンサーの角度を得る。
q0708_t hole_speed();//ホールセンサーの回転速度を得る。(低速時は安定しない) 単位は[rad/us]
#endif