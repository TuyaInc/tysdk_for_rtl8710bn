/*
uni_mutex.h
Copyright(C),2018-2020, 涂鸦科技 www.tuya.comm
*/

#ifndef _UNI_MUTEX_H
#define _UNI_MUTEX_H

#include "tuya_cloud_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* tuya sdk definition of mutex context */
typedef PVOID_T MUTEX_HANDLE;

/***********************************************************
*  Function: CreateMutexAndInit
*  Desc:     create and init a mutex
*  Output:   pMutexHandle: the created mutex context
*  Return:   OPRT_OK: success  Other: fail
***********************************************************/
OPERATE_RET CreateMutexAndInit(OUT MUTEX_HANDLE *pMutexHandle);

/***********************************************************
*  Function: MutexLock
*  Desc:     lock a mutex
*  Input:    mutexHandle: the mutex context
*  Return:   OPRT_OK: success  Other: fail
***********************************************************/
OPERATE_RET MutexLock(IN CONST MUTEX_HANDLE mutexHandle);

/***********************************************************
*  Function: MutexUnLock
*  Desc:     unlock a mutex
*  Input:    mutexHandle: the mutex context
*  Return:   OPRT_OK: success  Other: fail
***********************************************************/
OPERATE_RET MutexUnLock(IN CONST MUTEX_HANDLE mutexHandle);

/***********************************************************
*  Function: ReleaseMutex
*  Desc:     release a mutex
*  Input:    mutexHandle: the mutex context
*  Return:   OPRT_OK: success  Other: fail
***********************************************************/
OPERATE_RET ReleaseMutex(IN CONST MUTEX_HANDLE mutexHandle);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
