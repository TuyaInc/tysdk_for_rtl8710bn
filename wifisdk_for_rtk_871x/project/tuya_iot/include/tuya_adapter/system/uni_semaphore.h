/*
uni_semaphore.h
Copyright(C),2018-2020, 涂鸦科技 www.tuya.comm
*/

#ifndef _UNI_SEMAPHORE_H
#define _UNI_SEMAPHORE_H

#include "tuya_cloud_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* tuya sdk definition of semaphore */
typedef PVOID_T SEM_HANDLE;

/***********************************************************
*  Function: CreateAndInitSemaphore
*  Desc:     create and init a semaphore
*  Input:    semCnt: the init value of a semaphore
*  Input:    sem_max: the max value
*  Output:   pHandle: the created semaphore context
*  Return:   OPRT_OK: success  Other: fail
***********************************************************/
OPERATE_RET CreateAndInitSemaphore(OUT SEM_HANDLE *pHandle, IN CONST UINT_T semCnt, IN CONST UINT_T sem_max);

/***********************************************************
*  Function: WaitSemaphore
*  Desc:     wait a semaphore
*  Input:    semHandle: the semaphore context
*  Return:   OPRT_OK: success  Other: fail
***********************************************************/
OPERATE_RET WaitSemaphore(IN CONST SEM_HANDLE semHandle);

/***********************************************************
*  Function: PostSemaphore
*  Desc:     post a semaphore
*  Input:    semHandle: the semaphore context
*  Return:   OPRT_OK: success  Other: fail
***********************************************************/
OPERATE_RET PostSemaphore(IN CONST SEM_HANDLE semHandle);

/***********************************************************
*  Function: ReleaseSemaphore
*  Desc:     release a semaphore
*  Input:    semHandle: the semaphore context
*  Return:   OPRT_OK: success  Other: fail
***********************************************************/
OPERATE_RET ReleaseSemaphore(IN CONST SEM_HANDLE semHandle);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

