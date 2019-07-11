/***********************************************************
*  File: adapter_platform.h
*  Author: nzy
*  Date: 20170921
***********************************************************/
#ifndef _ADAPTER_PLATFORM_H
#define _ADAPTER_PLATFORM_H

#include <string.h>
#include <errno.h>
#include <stdio.h>

#include "tuya_cloud_types.h"
#include "uni_system.h"
#include "uni_thread.h"
#include "uni_mutex.h"
#include "uni_network.h"
#include "uni_semaphore.h"
#include "uni_output.h"
#include "mem_pool.h"
#include "uni_log.h"
#include "uni_storge.h"
#include "ty_cJSON.h"
#include "uni_time.h"
#if defined(WIFI_GW) && (WIFI_GW==1)
#include "wf_basic_intf.h"
#else
#include "base_nw_intf.h"
#endif

#if defined(TY_RTC) && (TY_RTC==1)
#include "uni_rtc.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _ADAPTER_PLATFORM_GLOBAL
    #define _ADAPTER_PLATFORM_EXT
#else
    #define _ADAPTER_PLATFORM_EXT extern
#endif

/***********************************************************
*************************micro define***********************
***********************************************************/

/***********************************************************
*************************variable define********************
***********************************************************/

/***********************************************************
*************************function define********************
***********************************************************/
/***********************************************************
*  Function: adapter_platform_init
*  Input: none
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
_ADAPTER_PLATFORM_EXT \
OPERATE_RET adapter_platform_init(UNW_QUERY_CUSTOM_DNS_CB dns_query_cb);

#ifdef __cplusplus
}
#endif


#endif /* _ADAPTER_PLATFORM_H */
