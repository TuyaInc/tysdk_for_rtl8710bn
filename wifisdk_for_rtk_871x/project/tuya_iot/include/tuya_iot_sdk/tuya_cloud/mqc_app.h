/***********************************************************
*  File: mqc_app.h
*  Author: nzy
*  Date: 20170411
***********************************************************/
#ifndef _MQC_APP_H
#define _MQC_APP_H

#include "tuya_cloud_types.h"
#include "mqtt_client.h"
#include "gw_intf.h"
#include "uni_rfc.h"

#ifdef __cplusplus
    extern "C" {
#endif

#ifdef  __MQC_APP_GLOBALS
    #define __MQC_APP_EXT
#else
    #define __MQC_APP_EXT extern
#endif

// mqtt protocol
#define PRO_DATA_PUSH 4   /* dev -> cloud push dp data */
#define PRO_CMD 5         /* cloud -> dev send dp data */
#define PRO_GW_RESET 11   /* cloud -> dev reset dev */
#define PRO_TIMER_UG_INF 13   /* cloud -> dev update timer */
#define PRO_UPGD_REQ 15       /* cloud -> dev update dev/gw */
#define PRO_UPGE_PUSH 16      /* dev -> cloud update upgrade percent */
#define PRO_DEV_LINE_STAT_UPDATE 25 /* dev -> sub device online status update */
#define PRO_CMD_ACK  26    /* dev -> cloud  dev send ackId to cloud */
#define PRO_MQ_EXT_CFG_INF 27 /* cloud -> dev runtime cfg update */
#define PRO_MQ_QUERY_DP  31  /* cloud -> dev query dp stat */

#define PRO_IOT_DA_REQ 22   /* cloud -> dev send data req */
#define PRO_IOT_DA_RESP 23  /* dev -> cloud send data resp */
#define PRO_DEV_UNBIND 8 /* cloud -> dev */
// #define PRO_DEV_LINE_STAT_UPDATE 25 /* dev->cloud */
#define PRO_UG_SUMMER_TABLE 41 // ug sumer timer table
#define PRO_GW_UPLOAD_LOG  45 /* dev -> cloud, upload log*/

#define PRO_GW_LINKAGE_UPDATE  49 /* cloud -> dev 场景更新推送 */

#define PRO_MQ_THINGCONFIG  51  /* 设备免密配网 */

#if defined(ENABLE_SIGMESH) && (ENABLE_SIGMESH==1)
#define PRO_GW_SIGMESH_TOPO_UPDATE  54 /* cloud -> dev sigmesh topo update */
#endif
#define PRO_MQ_EN_GW_ADD_DEV_REQ 200 // gw enable add sub device request
#define PRT_MQ_EN_GW_ADD_DEV_RESP 201 // gw enable add sub device respond

#define PRT_DEV_LC_GROUP_OPER 202 /* cloud -> dev */
#define PRT_DEV_LC_GROUP_OPER_RESP 203  /* dev -> cloud */
#define PRO_DEV_LC_SENCE_OPER 204 /* cloud -> dev */
#define PRT_DEV_LC_SENCE_OPER_RESP 205 /* dev -> cloud */
#define PRO_DEV_LC_SENCE_EXEC 206 /* cloud -> dev */




//VOID mqc_app_enable_media(VOID);

__MQC_APP_EXT \
OPERATE_RET mqc_app_init(BOOL_T enable_time_check);

__MQC_APP_EXT \
BOOL_T get_mqc_conn_stat(VOID);

__MQC_APP_EXT \
OPERATE_RET mqc_very_pcmcd_mcd(IN CONST UINT_T cmd_from,IN CONST UINT_T seq);

__MQC_APP_EXT \
OPERATE_RET mqc_dp_report_async(RFC_DATA_S *rfc_da, IN CONST UINT_T to_lmt, IN CONST MQ_PUB_ASYNC_IFM_CB cb,IN VOID *prv_data);

__MQC_APP_EXT \
OPERATE_RET mqc_obj_dp_query(RFC_DATA_S *rfc_da);

__MQC_APP_EXT \
OPERATE_RET mqc_upgd_progress_rept_v41(IN CONST UINT_T percent,IN CONST CHAR_T *devid, IN CONST CLOUD_DEV_TP_DEF_T tp);

__MQC_APP_EXT \
OPERATE_RET mqc_prot_data_rept_seq(IN CONST UINT_T pro,IN CONST CHAR_T *data,IN CONST BYTE_T qos,\
                                              IN CONST UINT_T to_lmt,IN CONST MQ_PUB_ASYNC_IFM_CB cb,IN VOID *prv_data);

__MQC_APP_EXT \
OPERATE_RET mqc_get_wakeup_data(INOUT BYTE_T *wakeup_data_arr, INOUT UINT_T *p_len);

__MQC_APP_EXT \
OPERATE_RET mqc_get_heartbeat_data(INOUT BYTE_T *heartbeat_data_arr, INOUT UINT_T *p_len);

__MQC_APP_EXT \
OPERATE_RET mqc_book_wakeup_topic(VOID);

__MQC_APP_EXT \
INT_T mqc_get_alive_time_s(VOID);

__MQC_APP_EXT \
INT_T mqc_get_mqtt_socket_fd(VOID);

__MQC_APP_EXT \
OPERATE_RET mqc_send_custom_mqtt_msg(IN CONST UINT_T protocol, IN CONST BYTE_T *p_data);

typedef OPERATE_RET (*mqc_protocol_handler_cb)(IN ty_cJSON *root_json);
OPERATE_RET mqc_app_register_cb(UINT_T mq_pro, mqc_protocol_handler_cb handler);

__MQC_APP_EXT \
OPERATE_RET mqc_log_notify(IN CONST CHAR_T *uuid, IN CONST CHAR_T *bucket, \
                                IN CONST CHAR_T *log_path, IN CONST CHAR_T *logName, CHAR_T* logSuf);

__MQC_APP_EXT \
OPERATE_RET mqc_media_pub_async(IN CONST FLOW_BODY_ST *dt_body,
                                IN CONST UINT_T to_lmt, IN CONST MQ_PUB_ASYNC_IFM_CB cb,IN VOID *prv_data);
__MQC_APP_EXT \
OPERATE_RET mqc_media_pub_sync(IN CONST FLOW_BODY_ST *dt_body,IN CONST UINT_T timeout);

__MQC_APP_EXT \
OPERATE_RET mqc_media_pub_data_sync(IN CONST USHORT_T map_id,IN CONST UINT_T offset,IN CONST BYTE_T *pbuffer, IN CONST USHORT_T buf_len);

__MQC_APP_EXT \
OPERATE_RET mqc_custom_pub_async(IN CONST BYTE_T *data, IN CONST INT_T len, IN CONST CHAR_T *topic,
                                        IN CONST BYTE_T qos, IN CONST UINT_T timeout, IN CONST MQ_PUB_ASYNC_IFM_CB cb,
                                        IN VOID *prv_data);
__MQC_APP_EXT \
OPERATE_RET mqc_custom_pub_sync(IN CONST BYTE_T *data, IN CONST INT_T len, IN CONST CHAR_T *topic,
                                        IN CONST BYTE_T qos, IN CONST UINT_T timeout);

#ifdef __cplusplus
}
#endif
#endif

