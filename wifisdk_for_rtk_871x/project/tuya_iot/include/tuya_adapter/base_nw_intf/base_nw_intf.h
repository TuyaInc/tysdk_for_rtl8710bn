/***********************************************************
*  File: base_nw_intf.h
*  Author: nzy
*  Date: 20170220
***********************************************************/
#ifndef _BASE_NW_INTF_H
#define _BASE_NW_INTF_H
#ifdef __cplusplus
	extern "C" {
#endif

#include "tuya_cloud_types.h"
#include "uni_network.h"
#include "base_hwl.h"

#ifdef  __BASE_NW_INTF_GLOBALS
    #define __BASE_NW_INTF_EXT
#else
    #define __BASE_NW_INTF_EXT extern
#endif

/***********************************************************
*************************macro define***********************
***********************************************************/

/***********************************************************
*************************variable define********************
***********************************************************/

/***********************************************************
*************************function define********************
***********************************************************/
/***********************************************************
*  Function: bnw_get_ip 
*  Input: none
*  Output: ip
*  Return: OPERATE_RET
***********************************************************/
__BASE_NW_INTF_EXT \
OPERATE_RET bnw_get_ip(OUT NW_IP_S *ip);

/***********************************************************
*  Function: bnw_station_conn 
*  Input: none
*  Output: none
*  Return: BOOL_T
***********************************************************/
__BASE_NW_INTF_EXT \
BOOL_T bnw_station_conn(VOID);

/***********************************************************
*  Function: bnw_set_station_connect
*  Input: ssid passwd
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__BASE_NW_INTF_EXT \
OPERATE_RET bnw_set_station_connect(IN CONST CHAR_T *ssid,IN CONST CHAR_T *passwd);

/***********************************************************
*  Function: bnw_need_wifi_cfg
*  Input: none
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__BASE_NW_INTF_EXT \
BOOL_T bnw_need_wifi_cfg(VOID);

/***********************************************************
*  Function: bnw_station_get_conn_ap_rssi
*  Input: none
*  Output: rssi
*  Return: OPERATE_RET
*  Note: 仅有线返回99,无线+有线/无线，返回实际无线信号
***********************************************************/
__BASE_NW_INTF_EXT \
OPERATE_RET bnw_station_get_conn_ap_rssi(OUT SCHAR_T *rssi);

/***********************************************************
*  Function: bnw_get_mac
*  Input: none
*  Output: mac
*  Return: OPERATE_RET
***********************************************************/
__BASE_NW_INTF_EXT \
OPERATE_RET bnw_get_mac(OUT NW_MAC_S *mac);

/***********************************************************
*  Function: bnw_set_mac
*  Input: mac
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__BASE_NW_INTF_EXT \
OPERATE_RET bnw_set_mac(IN CONST NW_MAC_S *mac);

#ifdef __cplusplus
}
#endif
#endif

