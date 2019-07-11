/***********************************************************
*  File: tuya_rtc.h
*  Author: lql
*  Date: 20180926502
***********************************************************/
#include "rtc_api.h"
#include "wait_api.h"


extern VOID ty_rtc_init(VOID);
extern VOID ty_set_rtc_time(time_t write_time);
extern time_t ty_get_rtc_time(VOID);


