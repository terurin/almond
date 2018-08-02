/* 用途　drv8335の制御
 * 作者　terurin
 */

#ifndef __MID_DRV_HEADER_GUARD__
#define __MID_DRV_HEADER_GUARD__

enum drv_status{
    DRV_STATUS_NONE,
    DRV_STATUS_RUN,
    DRV_STATUS_OTW,
    DRV_STATE_FAULT
};
typedef enum drv_status drv_status_t;

void drv_init();
void drv_free();
void drv_halt();


drv_status_t drv_info();









#endif