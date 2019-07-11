/*
uni_system.h
Copyright(C),2018-2020, 涂鸦科技 www.tuya.comm
*/

#ifndef _UNI_SYSTEM_H
#define _UNI_SYSTEM_H

#include "tuya_cloud_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _UNI_SYSTEM_GLOBAL
    #define _UNI_SYSTEM_EXT
#else
    #define _UNI_SYSTEM_EXT extern
#endif



#if OPERATING_SYSTEM == SYSTEM_LINUX
typedef UINT64_T SYS_TICK_T;
#else
typedef UINT_T SYS_TICK_T;
#endif



/***********************************************************
*  Function: GetSystemTickCount
*  Desc:     get the system running clock ticket
*  Return:   the clock ticket
***********************************************************/
_UNI_SYSTEM_EXT \
SYS_TICK_T GetSystemTickCount(VOID);

/***********************************************************
*  Function: GetTickRateMs
*  Desc:     get the time is ms of a system ticket
*  Return:   the time is ms of a system ticket
***********************************************************/
_UNI_SYSTEM_EXT \
UINT_T GetTickRateMs(VOID);

/***********************************************************
*  Function: SystemSleep
*  Desc:     tuya sdk system sleep function
*  Input:    sleep time is ms
***********************************************************/
_UNI_SYSTEM_EXT \
VOID SystemSleep(IN CONST TIME_MS msTime);

/***********************************************************
*  Function: SystemIsrStatus
*  Desc:     check the system is in interrupt context
*  Return:   the system in interrupt context
***********************************************************/
_UNI_SYSTEM_EXT \
BOOL_T SystemIsrStatus(VOID);

/***********************************************************
*  Function: SystemReset
*  Desc:     reboot the system
***********************************************************/
_UNI_SYSTEM_EXT \
VOID SystemReset(VOID);

/***********************************************************
*  Function: SysMalloc
*  Desc:    the malloc implementation in current system
*  Input & Return: refer to std malloc
***********************************************************/
_UNI_SYSTEM_EXT \
VOID *SysMalloc(IN CONST DWORD_T reqSize);

/***********************************************************
*  Function: SysFree
*  Desc:    the free implementation in current system
*  Input: refer to std free
***********************************************************/
_UNI_SYSTEM_EXT \
VOID SysFree(IN PVOID_T pReqMem);

/***********************************************************
*  Function: uni_watchdog_init_and_start
*  Input: timeval
*  Output: none
*  Return: VOID *
***********************************************************/
VOID uni_watchdog_init_and_start(INT_T timeval);

/***********************************************************
*  Function: uni_watchdog_refresh
*  Input: none
*  Output: none
*  Return: VOID *
***********************************************************/
VOID uni_watchdog_refresh(VOID);

/***********************************************************
*  Function: uni_watchdog_stop
*  Input: none
*  Output: none
*  Return: VOID *
***********************************************************/
VOID uni_watchdog_stop(VOID);


/***********************************************************
*  Function: SysGetHeapSize
*  Desc:     get system heap size / get system free memory
*  Return:   <0: don't support  >=0: current heap size/free memory
***********************************************************/
_UNI_SYSTEM_EXT \
INT_T SysGetHeapSize(VOID);

/***********************************************************
*  Function: GetSerialNo
*  Desc:     get hardware serial num
*  Return:   hardware serial num
***********************************************************/
_UNI_SYSTEM_EXT \
CHAR_T *GetSerialNo(VOID);

/***********************************************************
*  Function: system_get_rst_info
*  Desc:     get hardware reboot reason
*  Return:   hardware reboot reason
***********************************************************/
_UNI_SYSTEM_EXT \
CHAR_T *system_get_rst_info(VOID);


/***********************************************************
*  Function: tuya_random
*  Input: none
*  Output: none
*  Return: random data in INT
***********************************************************/
_UNI_SYSTEM_EXT \
UINT_T tuya_random(VOID);

/***********************************************************
*  Function: tuya_get_random_data
*  Input: dst size
*  Output: none
*  Return: VOID
***********************************************************/
_UNI_SYSTEM_EXT \
OPERATE_RET tuya_get_random_data(UCHAR_T* dst, INT_T size, UCHAR_T range);

/***********************************************************
*  Function: tuya_set_lp_mode
*  Input: lp_enable
*  Output: none
*  Return: VOID
***********************************************************/
_UNI_SYSTEM_EXT \
VOID tuya_set_lp_mode(BOOL_T lp_enable);


/***********************************************************
*  Function: tuya_get_lp_mode
*  Input: none
*  Output: none
*  Return: BOOL_T
***********************************************************/
_UNI_SYSTEM_EXT \
BOOL_T tuya_get_lp_mode(VOID);

/**
 * @brief 判断本次重启原因是否为电源重启，仅用于计数重置模式。
 *        若模块无法判断重启原因，且需要计数重置功能，直接返回true;
 *        若模块无法判断重启原因，且不需要计数重置功能，直接返回false;
 * 
 * @return true 本次重启原因是电源重启
 * @return false 本次重启原因不是电源重启
 */
BOOL_T system_rst_reason_poweron(VOID);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

