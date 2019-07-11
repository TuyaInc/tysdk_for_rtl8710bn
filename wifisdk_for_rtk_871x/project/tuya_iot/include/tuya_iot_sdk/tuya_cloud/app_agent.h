/***********************************************************
*  File: app_agent.h
*  Author: nzy
*  Date: 20150618
***********************************************************/
#ifndef _APP_AGENT_H
#define _APP_AGENT_H

#include "tuya_cloud_types.h"

#ifdef __cplusplus
    extern "C" {
#endif

#ifdef  __APP_AGENT_GLOBALS
    #define __APP_AGENT_EXT
#else
    #define __APP_AGENT_EXT extern
#endif

//group test
#define FRM_GRP_OPER_ENGR 0xd0
#define FRM_GRP_CMD 0xd1
typedef OPERATE_RET (*AP_CFG_FINISH_CB)(IN CONST CHAR_T *ssid,IN CONST CHAR_T *passwd, IN CONST CHAR_T *token);

__APP_AGENT_EXT \
OPERATE_RET lan_pro_cntl_init(BOOL_T wechat);

__APP_AGENT_EXT \
OPERATE_RET lan_pro_cntl_exit(VOID);

__APP_AGENT_EXT \
OPERATE_RET lan_ap_nw_cfg_start(AP_CFG_FINISH_CB finish_cb);

__APP_AGENT_EXT \
VOID lan_ap_nw_cfg_stop(VOID);

__APP_AGENT_EXT \
OPERATE_RET lan_dp_sata_report(IN CONST VOID *data,IN CONST UINT_T len);

__APP_AGENT_EXT \
OPERATE_RET lan_data_report_cb(IN CONST UINT_T fr_type,IN CONST UINT_T ret_code, \
                                     IN CONST BYTE_T *data,IN CONST UINT_T len);

__APP_AGENT_EXT \
INT_T lan_pro_cntl_get_valid_connect_cnt(VOID);

#ifdef __cplusplus
}
#endif
#endif

