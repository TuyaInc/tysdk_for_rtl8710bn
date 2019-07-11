/***********************************************************
*  File: wf_basic_intf.h 
*  Author: nzy
*  Date: 20170220
***********************************************************/
#ifndef _WF_BASIC_INTF_H
#define _WF_BASIC_INTF_H
#ifdef __cplusplus
	extern "C" {
#endif

#include "tuya_cloud_types.h"
#include "uni_network.h"
#include "wifi_hwl.h"

#ifdef  __WF_BASIC_INTF_GLOBALS
    #define __WF_BASIC_INTF_EXT
#else
    #define __WF_BASIC_INTF_EXT extern
#endif

/***********************************************************
*************************macro define***********************
***********************************************************/

/*!
\brief WIFI芯片探测本地AP信息结构体
\struct MIMO_IF_S
*/
typedef enum
{
    MIMO_TYPE_NORMAL = 0,
    MIMO_TYPE_HT40,
    MIMO_TYPE_2X2,
    MIMO_TYPE_LDPC,

    MIMO_TYPE_NUM,
}MIMO_TYPE_E;

typedef struct
{
    SCHAR_T rssi;               /*!< MIMO包信号 */
    MIMO_TYPE_E type;           /*!< MIMO包类型 */
    USHORT_T len;               /*!< MIMO包长度 */
    BYTE_T channel;             /*!< MIMO包信道 */
    BYTE_T mcs;
} MIMO_IF_S;

/*!
\brief 802.11 frame info
\enum WLAN_FRM_TP_E
*/
typedef enum {
    WFT_BEACON = 0x80,      // Beacon
    WFT_DATA = 0x08,        // Data
    WFT_QOS_DATA = 0x88,    // QOS Data
    WFT_MIMO_DATA = 0xff,   // MIMO Data
}WLAN_FRM_TP_E;

#pragma pack(1)
typedef struct {
    BYTE_T frame_ctrl_flags; // Frame Control flags
    USHORT_T duration; // Duration
    BYTE_T dest[6]; // Destination MAC Address
    BYTE_T src[6]; // Source MAC Address
    BYTE_T bssid[6]; // BSSID MAC Address
    USHORT_T seq_frag_num; // Sequence and Fragmentation number
    BYTE_T timestamp[8]; // Time stamp
    USHORT_T beacon_interval; // Beacon Interval
    USHORT_T cap_info; // Capability Information
    BYTE_T ssid_element_id; // SSID Element ID
    BYTE_T ssid_len; // SSID Length
    CHAR_T ssid[0]; // SSID
}WLAN_BECAON_IF_S;

#define TO_FROM_DS_MASK 0x03
#define TFD_IBSS 0x00 // da+sa+bssid
#define TFD_TO_AP 0x01 // bssid+sa+da
#define TFD_FROM_AP 0x02 // ds+bssid+sa
#define TFD_WDS 0x03 // ra+ta+da

typedef BYTE_T BC_DA_CHAN_T;
#define BC_TO_AP 0
#define BC_FROM_AP 1
#define BC_CHAN_NUM 2

typedef struct {
    BYTE_T addr1[6];
    BYTE_T addr2[6];
    BYTE_T addr3[6];
}WLAN_COM_ADDR_S;

typedef struct {
    BYTE_T bssid[6];
    BYTE_T src[6];
    BYTE_T dst[6];
}WLAN_TO_AP_ADDR_S;

typedef struct {
    BYTE_T dst[6];
    BYTE_T bssid[6];
    BYTE_T src[6];
}WLAN_FROM_AP_ADDR_S;

typedef union {
    WLAN_COM_ADDR_S com;
    WLAN_TO_AP_ADDR_S to_ap;
    WLAN_FROM_AP_ADDR_S from_ap;
}WLAN_ADDR_U;

typedef struct {
    BYTE_T frame_ctrl_flags; // Frame Control flags
    USHORT_T duration; // Duration
    WLAN_ADDR_U addr; // address
    USHORT_T seq_frag_num; // Sequence and Fragmentation number
    USHORT_T qos_ctrl; // QoS Control bits
}WLAN_DATA_IF_S;

/*!
\brief WLAN Frame info
\struct WLAN_FRAME_S
*/
typedef struct {
    BYTE_T frame_type; // WLAN Frame type
    union {
        WLAN_BECAON_IF_S beacon_info; // WLAN Beacon info
        WLAN_DATA_IF_S data_info; // WLAN Data info
        MIMO_IF_S mimo_info; // mimo info
    } frame_data;
}WLAN_FRAME_S,*P_WLAN_FRAME_S;
#pragma pack()

/***********************************************************
*************************variable define********************
***********************************************************/

/***********************************************************
*************************function define********************
***********************************************************/

/***********************************************************
*  Function: wf_all_ap_scan
*  Input: none
*  Output: ap_ary->scan ap info array
*          num->scan ap nums
*  Return: OPERATE_RET
***********************************************************/
__WF_BASIC_INTF_EXT \
OPERATE_RET wf_all_ap_scan(OUT AP_IF_S **ap_ary,OUT UINT_T *num);

/***********************************************************
*  Function: wf_assign_ap_scan
*  Input: ssid->assign ap ssid
*  Output: ap->ap info
*  Return: OPERATE_RET
***********************************************************/
__WF_BASIC_INTF_EXT \
OPERATE_RET wf_assign_ap_scan(IN CONST CHAR_T *ssid,OUT AP_IF_S **ap);

/***********************************************************
*  Function: wf_release_ap
*  Input: ap
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__WF_BASIC_INTF_EXT \
OPERATE_RET wf_release_ap(IN AP_IF_S *ap);

/***********************************************************
*  Function: wf_set_cur_channel
*  Input: chan
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__WF_BASIC_INTF_EXT \
OPERATE_RET wf_set_cur_channel(IN CONST BYTE_T chan);

/***********************************************************
*  Function: wf_get_cur_channel
*  Input: none
*  Output: chan
*  Return: OPERATE_RET
***********************************************************/
__WF_BASIC_INTF_EXT \
OPERATE_RET wf_get_cur_channel(OUT BYTE_T *chan);

/***********************************************************
*  Function: wf_sniffer_set
*  Input: en->TRUE/FALSE
*         cb->sniffer callback
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__WF_BASIC_INTF_EXT \
OPERATE_RET wf_sniffer_set(IN CONST BOOL_T en,IN CONST SNIFFER_CALLBACK cb);

/***********************************************************
*  Function: wf_get_ip
*  Input: wf->WF_IF_E
*  Output: ip
*  Return: OPERATE_RET
***********************************************************/
__WF_BASIC_INTF_EXT \
OPERATE_RET wf_get_ip(IN CONST WF_IF_E wf,OUT NW_IP_S *ip);

/***********************************************************
*  Function: wf_get_mac
*  Input: wf->WF_IF_E
*  Output: mac
*  Return: OPERATE_RET
***********************************************************/
__WF_BASIC_INTF_EXT \
OPERATE_RET wf_get_mac(IN CONST WF_IF_E wf,OUT NW_MAC_S *mac);

/***********************************************************
*  Function: wf_set_mac
*  Input: wf->WF_IF_E 
*         mac
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__WF_BASIC_INTF_EXT \
OPERATE_RET wf_set_mac(IN CONST WF_IF_E wf,IN CONST NW_MAC_S *mac);

/***********************************************************
*  Function: wf_wk_mode_set
*  Input: mode->WF_WK_MD_E
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__WF_BASIC_INTF_EXT \
OPERATE_RET wf_wk_mode_set(IN CONST WF_WK_MD_E mode);

/***********************************************************
*  Function: wf_wk_mode_get
*  Input: none
*  Output: mode
*  Return: OPERATE_RET
***********************************************************/
__WF_BASIC_INTF_EXT \
OPERATE_RET wf_wk_mode_get(OUT WF_WK_MD_E *mode);


#if defined(ENABLE_AP_FAST_CONNECT) && (ENABLE_AP_FAST_CONNECT==1)
/***********************************************************
*  Function: wf_get_connected_ap_info
*  Input: none
*  Output: fast_ap_infoone
*  Return: OPERATE_RET
***********************************************************/
__WF_BASIC_INTF_EXT \
OPERATE_RET wf_get_connected_ap_info(OUT FAST_WF_CONNECTED_AP_INFO_S *fast_ap_info);

/***********************************************************
*  Function: wf_fast_station_connect
*  Input: fast_ap_info
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__WF_BASIC_INTF_EXT \
OPERATE_RET wf_fast_station_connect(IN FAST_WF_CONNECTED_AP_INFO_S *fast_ap_info);
#endif

/***********************************************************
*  Function: wf_station_connect
*  Input: ssid
*         passwd
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__WF_BASIC_INTF_EXT \
OPERATE_RET wf_station_connect(IN CONST CHAR_T *ssid,IN CONST CHAR_T *passwd);

/***********************************************************
*  Function: wf_station_disconnect
*  Input: none
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__WF_BASIC_INTF_EXT \
OPERATE_RET wf_station_disconnect(VOID);

/***********************************************************
*  Function: wf_station_get_conn_ap_rssi
*  Input: none
*  Output: rssi
*  Return: OPERATE_RET
***********************************************************/
__WF_BASIC_INTF_EXT \
OPERATE_RET wf_station_get_conn_ap_rssi(OUT SCHAR_T *rssi);

/***********************************************************
*  Function: wf_station_stat_get
*  Input: none
*  Output: stat
*  Return: OPERATE_RET
***********************************************************/
__WF_BASIC_INTF_EXT \
OPERATE_RET wf_station_stat_get(OUT WF_STATION_STAT_E *stat);

/***********************************************************
*  Function: wf_ap_start
*  Input: cfg
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__WF_BASIC_INTF_EXT \
OPERATE_RET wf_ap_start(IN CONST WF_AP_CFG_IF_S *cfg);

/***********************************************************
*  Function: wf_ap_stop
*  Input: none
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__WF_BASIC_INTF_EXT \
OPERATE_RET wf_ap_stop(VOID);

__WF_BASIC_INTF_EXT \
OPERATE_RET wf_set_country_code(IN CONST CHAR_T *p_country_code);

__WF_BASIC_INTF_EXT \
OPERATE_RET wf_lowpower_enable(VOID);

__WF_BASIC_INTF_EXT \
OPERATE_RET wf_lowpower_disable(VOID);



#ifdef __cplusplus
}
#endif
#endif

