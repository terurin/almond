// 作成者　terurin
// 用途　ホールセンサーの状態を取得する

#ifndef MID_HOLE_HEADER_GUARD
#define MID_HOLE_HEADER_GUARD

enum hole{
    HOLE_A=0,
    HOLE_AB,
    HOLE_B,
    HOLE_BC,
    HOLE_C,
    HOLE_CA,
    HOLE_END,//終端処理用
    HOLE_ERROR
};
typedef enum hole hole_t;

void hole_init();
hole_t hole_sense();
int hole_diff(hole_t now,hole_t last);//進行度を得る


#endif