/***********************************************************
*  File: device.h 
*  Author: XY
*  Date: 20190410
***********************************************************/
#ifndef _CD_TIMER_IF_H
    #define _CD_TIMER_IF_H
	#include "tuya_device.h"
	#include "adapter_platform.h"
    #include "tuya_cloud_types.h"
	#include "sys_timer.h"
    
#ifdef __cplusplus
	extern "C" {
#endif

/*ch_idx      通道索引(0 – n-1)
*remain_sec  倒计时剩余时间
*/
typedef VOID (*CdTimeCB)(IN UCHAR_T  ch_idx, IN UINT_T remain_sec);

typedef PVOID CD_TIME_HANDLE; 

/***********************************************************
*  Function: cd_time_init
*  Input: handle	定时器句柄地址
*		  ch_num    通道数量（1 - n） 
*		  cycle_sec  回调周期（单位秒），推荐值30
*		  fun_cb     周期回调函数
*  Output: 
*  Return: 返回值：OPRT_OK 成功， 其它 失败
***********************************************************/
OPERATE_RET cd_time_init(INOUT CD_TIME_HANDLE* handle, IN UCHAR_T ch_num, IN UCHAR_T cycle_sec, IN CONST CdTimeCB fun_cb);


/***********************************************************
*  Function: cd_time_init 注销倒计时组件
*  Input: handle	定时器句柄地址
*  Output: 
*  Return: 
***********************************************************/
VOID cd_time_deinit(INOUT CD_TIME_HANDLE* handle);


/***********************************************************
*  Function: cd_time_start 开启通道N定时器
*  Input: handle	定时器句柄
*		  ch_idx      通道索引 0-(N-1)
*		  cd_time     倒计时时间
*  Output: 
*  Return: 返回值：OPRT_OK 成功， 其它 失败
***********************************************************/
OPERATE_RET cd_time_start(IN CONST CD_TIME_HANDLE handle, IN UCHAR_T ch_idx, IN UINT_T cd_time);


/***********************************************************
*  Function: cd_time_stop 停止倒计时
*  Input: 	handle	定时器句柄 
*			ch_idx  通道索引 0-(N-1)
*  Output: 
*  Return: 返回值：OPRT_OK 成功， 其它 失败
***********************************************************/
OPERATE_RET  cd_time_stop(IN CONST CD_TIME_HANDLE handle, IN UCHAR_T ch_idx);


/***********************************************************
*  Function: cd_time_all_stop 停止所有倒计时
*  Input: 	handle	定时器句柄 
*  Output: 
*  Return: 返回值：OPRT_OK 成功， 其它 失败
***********************************************************/
OPERATE_RET  cd_time_all_stop(IN CONST CD_TIME_HANDLE handle);


/***********************************************************
*  Function: cd_time_get_remain_sec 获取倒计剩余时间
*  Input: 	 ch_idx      通道索引 0-(N-1)
*  Output: 
*  Return: 倒计时剩余时间，单位秒
***********************************************************/
UINT_T	cd_time_get_remain_sec(IN CONST CD_TIME_HANDLE handle, IN UCHAR_T ch_idx);


#ifdef __cplusplus
}
#endif
#endif

