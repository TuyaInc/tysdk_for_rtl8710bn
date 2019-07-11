/***********************************************************
*  File: gw_intf.h
*  Author: nzy
*  Date: 20150822
***********************************************************/
#ifndef _GW_INTF_H
#define _GW_INTF_H

#include "tuya_cloud_types.h"
#include "adapter_platform.h"
#if defined(WIFI_GW) && (WIFI_GW==1)
#include "wl_cfg_com.h"
#endif

#include "uni_msg_queue.h"
#include "tuya_cloud_com_defs.h"
#include "tuya_cloud_wifi_defs.h"
#include "tuya_cloud_base_defs.h"
#include "tuya_iot_internal_api.h"

#include "hashmap.h"

#ifdef __cplusplus
    extern "C" {
#endif

#ifdef  __GW_INTF_GLOBALS
    #define __GW_INTF_EXT
#else
    #define __GW_INTF_EXT extern
#endif

/***********************************************************
*************************micro define***********************
***********************************************************/

#if TY_SECURITY_CHIP
typedef struct
{
    CHAR_T chipId1[32 + 1];				//芯片ID
    CHAR_T securityCode1[16 + 1];		//安全认证码1
    CHAR_T randomA[8 + 1];				//随机数A
    CHAR_T getTempKey[32];				//临时通信密钥
    CHAR_T cbc_iv[16];					//CBC加密初始密钥
}SECURITY_CHIP_S;

// gw BASE information
typedef struct {
    CHAR_T uuid[GW_UUID_LEN+1];
    CHAR_T auth_key[AUTH_KEY_LEN+1];
    BOOL_T is_security_chip_ok; 		//是否从加密芯片获得授权数据 即uuid和auth_key
}GW_AUTH_INFO_S;

#endif


// gateway network attribute decribe
typedef struct {
    GW_ABI abi;

    // only (tp == GNT_WIFI_NEED_CFG) valid
    #if defined(WIFI_GW) && (WIFI_GW==1)
    GW_WF_CFG_MTHD_SEL mthd;
    BOOL_T ap_start;
    GW_WF_START_MODE wifi_start_mode;
    #endif
}GW_CFG_S;

// gw BASE information
typedef struct {
#if !TY_SECURITY_CHIP
    CHAR_T uuid[GW_UUID_LEN+1];
    CHAR_T auth_key[AUTH_KEY_LEN+1];
#else
    BOOL_T has_auth;
#endif
    #if defined(WIFI_GW) && (WIFI_GW==1)
    CHAR_T ap_ssid[WIFI_SSID_LEN+1]; // ap ssid info
    CHAR_T ap_passwd[WIFI_PASSWD_LEN+1]; // ap passwd info
    CHAR_T country_code[8]; //  country code
    #endif
    BOOL_T prod_test; // production test
}GW_BASE_IF_S;

#if defined(WIFI_GW) && (WIFI_GW==1)
typedef BYTE_T GW_WF_MD_T;
#define GWM_NORMAL 0
#define GWM_SPECIAL_SMT_CFG 1
#define GWM_SPECIAL_AP_CFG 2
#endif

// gw register status
typedef BYTE_T GW_WORK_STAT_T;
#define UNREGISTERED 0 // unregistered
#define REGISTERED 1 // registered & activate start
#define ACTIVATED 2 // already active

typedef struct {
    // wifi connect info
    #if defined(WIFI_GW) && (WIFI_GW==1)
    GW_WF_NWC_STAT_T nc_tp; // valid when gateway is wireless
    GW_WF_MD_T md;
    CHAR_T ssid[WIFI_SSID_LEN+1];
    CHAR_T passwd[WIFI_PASSWD_LEN+1];
    BYTE_T random; // used to save airkiss random
    #endif

    GW_WORK_STAT_T stat;
    CHAR_T token[TOKEN_LEN+1]; // recvive from app
    CHAR_T region[REGION_LEN + 1]; // recvive from app
    CHAR_T regist_key[REGIST_KEY_LEN + 1]; // recvive from app,now useless
}GW_WORK_STAT_MAG_S;


typedef struct {
    GW_ABI abi; // ability
    CHAR_T id[GW_ID_LEN+1]; // virtual id
    CHAR_T sw_ver[SW_VER_LEN+1]; // xx.xx
    CHAR_T bs_ver[SW_VER_LEN+1]; // base version xx.xx
    CHAR_T prtl_ver[SW_VER_LEN+1]; // protocol version
    CHAR_T lan_prtl_ver[SW_VER_LEN+1]; // lan protocol version
    CHAR_T cad_ver[SW_VER_LEN+1]; // cad version
    CHAR_T cd_ver[SW_VER_LEN+1]; // cd version
    CHAR_T product_key[PRODUCT_KEY_LEN+1];
    CHAR_T firmware_key[PRODUCT_KEY_LEN+1]; // firmware-key

    // networklink module single device struct
    CHAR_T dev_sw_ver[SW_VER_LEN+1];
    CHAR_T schema_id[SCHEMA_ID_LEN+1];
    DEV_TYPE_T tp; // only (DEV_NM_ATH_SNGL or DEV_NM_NOT_ATH_SNGL)

    // GW attach attribute
    BYTE_T attr_num;
    GW_ATTACH_ATTR_T attr[GW_ATTACH_ATTR_LMT];

    BOOL_T sync;
}GW_DESC_IF_S;

typedef struct {
    CHAR_T key[SEC_KEY_LEN+1];
    CHAR_T local_key[LOCAL_KEY_LEN+1];
    CHAR_T http_url[HTTP_URL_LMT+1];
    CHAR_T https_url[HTTP_URL_LMT+1];
    CHAR_T httpsPSK_url[HTTP_URL_LMT+1];
    CHAR_T mq_meida_url[MQ_URL_LMT+1];
    CHAR_T mq_all_url[MQ_URL_LMT+1];/* TLS-ECC TLS-PSK NO-TLS Media */
    CHAR_T mq_tls_url[MQ_URL_LMT+1];
    CHAR_T mq_proto_url[MQ_URL_LMT+1];
    CHAR_T ai_speech_url[HTTP_URL_LMT+1];

    CHAR_T time_zone[TIME_ZONE_LEN+1];
    CHAR_T summer_time_zone[SUMMER_TM_ZONE_LEN+1];

    // wechat compatible
    CHAR_T wx_app_id[WXAPPID_LEN+1];
    // wechat compatible
    CHAR_T wx_uuid[WXUUID_LEN+1];
#if defined(ENABLE_SIGMESH) && (ENABLE_SIGMESH==1)
    CHAR_T net_key[SIGMESH_NET_KEY_LEN+1];
    CHAR_T app_key[SIGMESH_APP_KEY_LEN+1];
#endif
}GW_ACTV_IF_S;

/*
types in tuya cloud firmware type
TUYA云端对于嵌入式固件类型的定义,类型和值参考TUYA后台 固件版本管理-添加固件
*/
typedef BYTE_T CLOUD_DEV_TP_DEF_T;
#define CDT_NETLINK_MOD 0
#define CDT_BLUETOOTH_MOD 1
#define CDT_ZIGBEE_MOD 3
#define CDT_INFRARED_MOD 4
#define CDT_RF433_MOD 5
#define CDT_MCU_MOD 9


typedef BYTE_T DP_PV_STAT_E;
#define INVALID 0 // 数据无效
#define VALID_LC 1 // 本地有效数据
#define VALID_ULING 2 // 数据云端上传中 add by nzy 20150808
#define VALID_CLOUD 3 // 本地有效数据与服务端一致

typedef struct {
    USHORT_T cir_time; // circle time unit:second
    USHORT_T cir_rept_item_lmt; // admitting report to cloud items in circle(time == time_lmt)
    USHORT_T cir_rept_items; // report items in one circle
    TIME_S fir_rept_timestamp; // the first report success timestamp.
}DP_REPT_CNTL_S;

#if defined(RELIABLE_TRANSFER) && (RELIABLE_TRANSFER==1)
typedef struct {
    TIME_S cur_set_time;
    UINT_T cur_set_rept_num;
}DP_CUR_SET_REPT_CNTL_S;
#endif

typedef struct {
    DP_DESC_IF_S dp_desc;
    DP_PROP_VALUE_U prop;   /* 存储dp的值 */
    DP_PV_STAT_E pv_stat;  /* 当前缓存状态 */
    DP_REPT_CNTL_S rept_cntl;
#if defined(RELIABLE_TRANSFER) && (RELIABLE_TRANSFER==1)
    DP_PROP_VALUE_U cur_set;//当前下发的值
    DP_CUR_SET_REPT_CNTL_S cur_set_rept;
    BYTE_T is_set_flag;//是否要检查当前下发的值与实际状态是否一样
#endif
}DP_CNTL_S;

typedef BYTE_T DEV_OL_STAT_T;
#define OFFLINE FALSE
#define ONLINE TRUE

typedef BYTE_T DEV_OL_STAT_MAG_T;
#define DOSM_VLD_LC 0
#define DOSM_VLD_ULING 1
#define DOSM_VLD_CLD 2

#define GRP_LMT 56 // group limit
#define SCENE_LMT 56 // scene limit

#define LOG_PHAT_MAX_LEN 64

typedef struct {
    CHAR_T gid[GRP_ID_LEN+1];
}GRP_REC_S;

typedef struct {
    CHAR_T s_id[SCENE_ID_LEN+1];
    CHAR_T g_id[GRP_ID_LEN+1];
}SCENE_REC_S;

typedef struct {
    GRP_REC_S g_rec[GRP_LMT];
    BYTE_T cnt;
}GRP_MAG_S;

typedef struct {
    SCENE_REC_S s_rec[SCENE_LMT];
    BYTE_T cnt;
}SCENE_MAG_S;

// device active attribute
typedef struct {
    BOOL_T attach_dp_if; // is attach dp info
    BOOL_T preprocess; // 指示该设备是否预处理
    DEV_OL_STAT_T stat; // device online status
    DEV_OL_STAT_MAG_T dosm; // device online status manage
    TIME_S last_ol_time; // last online time,using for Heartbeat system trusteeship mode
    TIME_S hb_timeout_time; // if(hb_timeout_time == 0xffffffff) then device online forever
    TIME_S hb_send_time; // heartbeat send time
    UINT_T retry_send_times; //resend times
    UINT_T max_resend_times; // max send times
    // grpou info
    GRP_MAG_S *grp;

    // scene info
    SCENE_MAG_S *scene;
}DEV_ACTV_ATTR_S;

#define RETRY_BIND_LMT 3
#define RETRY_SYNC_LMT 3
#if defined(ENABLE_SIGMESH) && (ENABLE_SIGMESH==1)
#define RETRY_SIGMESH_LMT 3
#endif

typedef struct dev_cntl_n_s {
    struct dev_cntl_n_s *next;
    DEV_DESC_IF_S dev_if;
    DEV_ACTV_ATTR_S attr;
    BYTE_T retry_bind_times;
    BYTE_T retry_sync_times;
    BYTE_T dev_delete;
#if defined(RELIABLE_TRANSFER) && (RELIABLE_TRANSFER==1)
    BYTE_T reliable_transfer_flag;//1表示要可靠性检测
#endif
#if defined(ENABLE_SIGMESH) && (ENABLE_SIGMESH==1)
    BYTE_T retry_sigmesh_snyc_times;
#endif
    BYTE_T dp_num;
    DP_CNTL_S dp[0];
}DEV_CNTL_N_S;

typedef struct
{
    BOOL_T upload_upgrade_percent;
    GET_FILE_DATA_CB get_file_cb;
    UPGRADE_NOTIFY_CB upgrd_nofity_cb;
    PVOID_T pri_data;

    THRD_HANDLE upgrade_thrd;
    THRD_PARAM_S thrd_param;

    FW_UG_S fw;
    CHAR_T dev_id[DEV_ID_LEN+1];
    UINT_T download_buf_size;

    TIMER_ID mqc_upd_progess_timer;
    INT_T download_percent;
}UPGRADE_MGR_S;

typedef struct {
    FW_UG_S ug;
    CLOUD_DEV_TP_DEF_T tp;        //云端固件类型
    CHAR_T dev_id[DEV_ID_LEN+1]; // if gateway upgrade then dev_id[0] == 0
}GW_UG_MAG_S;

typedef BYTE_T TI_UPGRD_STAT_S;
#define TUS_RD 1
#define TUS_UPGRDING 2
#define TUS_UPGRD_FINI 3
#define TUS_UPGRD_EXEC 4

typedef struct {
    // gateway manage
    MUTEX_HANDLE gw_mutex;
    CHAR_T *serial_no; // gw serial num

    GW_WF_CFG_MTHD_SEL mthd;
    BOOL_T ap_start;
    GW_WF_START_MODE wifi_start_mode;

    GW_EXT_STAT_T ext_stat;

    BOOL_T is_oem; // is oem?
    BOOL_T is_init; // has inited?

#if TY_SECURITY_CHIP
    GW_AUTH_INFO_S secu_auth_info;
#endif

    GW_BASE_IF_S gw_base;
    GW_WORK_STAT_MAG_S gw_wsm;
    GW_DESC_IF_S gw_if;
    GW_ACTV_IF_S gw_actv;

    // device manage
    MUTEX_HANDLE dev_mutex;
    UINT_T dev_num;
    DEV_CNTL_N_S *dev;
    map_t dev_maps;

    /****** gw attribute ******/
    // nofity callback
    TY_IOT_CBS_S cbs;
    TY_IOT_GW_CBS_S gw_cbs;
    TY_IOT_APP_CBS_S app_cbs;
    /* upgrade manage */
    UPGRADE_MGR_S *upgrade;

    // timer message
    TM_MSG_S *tmm_get_url_cfg;
    #if defined(ENABLE_AP_FAST_CONNECT) && (ENABLE_AP_FAST_CONNECT==1)
    TM_MSG_S *tmm_update_connect_ap_info;
    #endif
    TM_MSG_S *tmm_gw_register;
    TM_MSG_S *tmm_gw_actv;
    TM_MSG_S *tmm_gw_if_sync;
    TM_MSG_S *tmm_dev_bind;
    TM_MSG_S *tmm_dev_if_sync;
#if defined(ENABLE_SIGMESH) && (ENABLE_SIGMESH==1)
    TM_MSG_S *tmm_dev_sigmesh_sync;
#endif

    TM_MSG_S *tmm_force_upd_log;
    TM_MSG_S *tmm_rst_log;//firmware rst log
    TM_MSG_S *tmm_gw_fw_ug;//firmware upgrade on self
    TM_MSG_S *tmm_dev_fw_ug;
    TM_MSG_S *tmm_gw_ugcfg;
    TM_MSG_S *tmm_get_dev_list;
    TM_MSG_S *tmm_dev_delete;

    #if defined(WIFI_GW) && (WIFI_GW==1)
    // use for GWCM_LOW_POWER or GWCM_SPCL_MODE
    TM_MSG_S *tmm_short; // 10s
    TM_MSG_S *tmm_long; // 3*60s
    TM_MSG_S *tmm_gw_monitor; // to monitor whether gw is connect to router lql
    TM_MSG_S *tmm_gw_security_verify; // to monitor whether gw is connect to router lql

    // message
    MSG_ID mid_gw_nw_lowpower; // network set low power
    MSG_ID mid_gw_nw_smt_cfg; // network set smartconfig
    MSG_ID mid_gw_nw_smt_cfg_stop; // network stop smartconfig
    MSG_ID mid_gw_nw_ap_cfg; // network set ap config
    MSG_ID mid_gw_nw_ap_cfg_stop; // network stop ap config
    MSG_ID mid_wf_reset;
    #endif
    MSG_ID mid_gw_net_conn_set; // network connect set
    MSG_ID mid_gw_actived; // gw actived

    MSG_ID mid_get_upgrd_if;
    MSG_ID mid_gw_upload_log;
    MSG_ID mid_gw_unregister;
    MSG_ID mid_gw_unactive;

#if defined(ENABLE_SUBDEVICE) && (ENABLE_SUBDEVICE==1)
    MSG_ID mid_dev_unregister;
#endif
    // MSG_ID mid_dev_unbind;

    TIMER_ID nw_stat_moni_tm;
    #if defined(WIFI_GW) && (WIFI_GW==1)
    GET_WF_NW_STAT_CB get_wf_nw_stat_cb;
    #else
    GET_NW_STAT_CB get_nw_stat_cb;
    #endif
}GW_CNTL_S;

typedef BYTE_T GW_RESET_TP_T;
#define GRT_LOCAL 0
#define GRT_REMOTE 1

typedef struct {
    GW_RESET_TP_T gw_rst_tp;
}GW_RESET_S;

typedef struct {
    GW_RESET_TP_T tp;
    CHAR_T dev_id[DEV_ID_LEN+1];
}DEV_RESET_S;

#if defined(ENABLE_ENGINEER_TO_NORMAL) && (ENABLE_ENGINEER_TO_NORMAL==1)
#define ENGINEER_STAT   "engineer_stat"
typedef enum {
    ENGINEER_STAT_MIN = 0,
    ENGINEER_NEED_SYNC,
    ENGINEER_ALREADY_SYNC,
    ENGINEER_ALREADY_UPLOAD,    //upload cloud config
    ENGINEER_STAT_MAX,

} ENGINEER_STAT_E;              //engineer state
#endif

/***********************************************************
*************************variable define********************
***********************************************************/


/***********************************************************
*************************function define********************
***********************************************************/
/***********************************************************
*  Function: __gw_cntl_init
*  Input: cfg->gw cfg
*         product_key->gw product key
*         sw_ver->gw version
*.........dev_sw_ver->device version
*.........tp->device type (DEV_NM_ATH_SNGL,DEV_NM_NOT_ATH_SNGL)
*         attr->gw attach attribute array when (cfg->abi >= GW_ENTITY_ATTH_DEV) valid
*         attr_num->gw attach attribute number when (cfg->abi >= GW_ENTITY_ATTH_DEV) valid
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__GW_INTF_EXT \
OPERATE_RET gw_cntl_init(IN CONST GW_CFG_S *cfg,IN CONST CHAR_T *product_key,IN CONST CHAR_T *firmware_key,\
                               IN CONST CHAR_T *sw_ver,IN CONST CHAR_T *dev_sw_ver,\
                               IN CONST DEV_TYPE_T tp,IN CONST GW_ATTACH_ATTR_T *attr,\
                               IN CONST UINT_T attr_num);

#if defined(WIFI_GW) && (WIFI_GW==1)

/***********************************************************
*  Function: gw_wifi_modify_and_conn
*  Input: ssid passwd
*  Output:
*  Return:
*  Note: use at the gateway is active,modify the wifi info
***********************************************************/
__GW_INTF_EXT \
OPERATE_RET gw_wifi_modify_and_conn(IN CONST CHAR_T *ssid,IN CONST CHAR_T *passwd);
#endif

/***********************************************************
*  Function: gw_unregister
*  Input: rst_t
*  Output:
*  Return:
***********************************************************/
__GW_INTF_EXT \
OPERATE_RET gw_unregister(IN CONST GW_RESET_S *rst_t);

/***********************************************************
*  Function: gw_unactive
*  Input: rst_t
*  Output:
*  Return:
***********************************************************/
__GW_INTF_EXT \
OPERATE_RET gw_unactive(IN CONST GW_RESET_S *rst_t);

/***********************************************************
*  Function: gw_register_cbs
*  Input: cbs
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__GW_INTF_EXT \
VOID gw_register_cbs(IN CONST TY_IOT_CBS_S *cbs);

/***********************************************************
*  Function: gw_register_gw_cbs
*  Input: cbs
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__GW_INTF_EXT \
VOID gw_register_gw_cbs(IN CONST TY_IOT_GW_CBS_S *gw_cbs);

/***********************************************************
*  Function: get_gw_dev_cntl
*  Input:
*  Output:
*  Return: GW_CNTL_S
***********************************************************/
__GW_INTF_EXT \
DEV_CNTL_N_S *get_gw_dev_cntl(VOID);

/***********************************************************
*  Function: get_gw_nw_status
*  Input:
*  Output:
*  Return: GW_NW_STAT_T
***********************************************************/
__GW_INTF_EXT \
GW_NW_STAT_T get_gw_nw_status(VOID);

#if defined(WIFI_GW) && (WIFI_GW==1)
/***********************************************************
*  Function: set_user_def_ap_if
*  Input: ssid passwd
*  Output:
*  Return: OPERATE_RET
*  Note: Must call on the front of the program
***********************************************************/
__GW_INTF_EXT \
OPERATE_RET set_user_def_ap_if(IN CONST CHAR_T *ssid,IN CONST CHAR_T *passwd);
#endif

__GW_INTF_EXT \
VOID gw_register_app_cbs(IN CONST TY_IOT_APP_CBS_S *app_cbs);

/***********************************************************
*  Function: set_gw_ext_stat
*  Input: ext_stat
*  Output:
*  Return:
***********************************************************/
__GW_INTF_EXT \
VOID set_gw_ext_stat(IN CONST GW_EXT_STAT_T ext_stat);

/***********************************************************
*  Function: get_gw_ext_stat
*  Input:
*  Output:
*  Return: GW_EXT_STAT_T
***********************************************************/
__GW_INTF_EXT \
GW_EXT_STAT_T get_gw_ext_stat(VOID);

/***********************************************************
*  Function: gw_lc_bind_device_pk
*  Input:
*  Output:
*  Return:
*  Note: use the prodect key to register
***********************************************************/
__GW_INTF_EXT \
OPERATE_RET gw_lc_bind_device_pk(IN CONST DEV_DESC_IF_S *dev_if,IN CONST BOOL_T attach_dp_if);

/***********************************************************
*  Function: gw_lc_unbind_device
*  Input:
*  Output:
*  Return:
*  Note:
***********************************************************/
__GW_INTF_EXT \
OPERATE_RET gw_lc_unbind_device(IN CONST CHAR_T *id);

/***********************************************************
*  Function: get_dev_cntl_unlock
*  Input:
*  Output:
*  Return:
*  Note:
***********************************************************/
__GW_INTF_EXT \
DEV_CNTL_N_S *get_dev_cntl_unlock(IN CONST CHAR_T *id);

/***********************************************************
*  Function: get_dev_cntl
*  Input:
*  Output:
*  Return:
*  Note:
***********************************************************/
__GW_INTF_EXT \
DEV_CNTL_N_S *get_dev_cntl(IN CONST CHAR_T *id);

/***********************************************************
*  Function: get_dev_dp_cntl
*  Input:dev_cntl
*        dpid
*  Output:
*  Return:
*  Note:
***********************************************************/
__GW_INTF_EXT \
DP_CNTL_S *get_dev_dp_cntl(IN CONST CHAR_T *id,IN CONST BYTE_T dpid);

/***********************************************************
*  Function: get_gw_cntl
*  Input:
*  Output:
*  Return: GW_CNTL_S
***********************************************************/
__GW_INTF_EXT \
GW_CNTL_S *get_gw_cntl(VOID);

/***********************************************************
*  Function: gw_start_upgrade
*  Input:
*
*        over_time: over time of upgrade,in Seconds
*  Output:
*  Return: OPERATE_RET
***********************************************************/
__GW_INTF_EXT \
OPERATE_RET gw_start_upgrade(IN CONST CHAR_T *devid,\
                             IN CONST FW_UG_S *fw,\
                             IN CONST GET_FILE_DATA_CB get_file_cb,\
                             IN PVOID_T pri_data,\
                             IN CONST UPGRADE_NOTIFY_CB upgrd_nofity_cb, \
                             IN CONST BOOL_T upload_upgrade_percent, \
                             IN CONST UINT_T download_buf_size
                             );
__GW_INTF_EXT \
OPERATE_RET gw_update_version(IN GW_PERMIT_DEV_TP_T type, IN CONST CHAR_T *ver);

/***********************************************************
*  Function: gw_user_token_bind
*  Input: token->user token
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__GW_INTF_EXT \
OPERATE_RET gw_user_token_bind(IN CONST CHAR_T *token);

/***********************************************************
*  Function: gw_get_ip
*  Input: none
*  Output: p_ip
*  Return: OPERATE_RET
***********************************************************/
__GW_INTF_EXT \
OPERATE_RET gw_get_ip(NW_IP_S *p_ip);

/***********************************************************
*  Function: gw_get_mac
*  Input: none
*  Output: p_ip
*  Return: OPERATE_RET
***********************************************************/
__GW_INTF_EXT \
OPERATE_RET gw_get_mac(NW_MAC_S *p_mac);

/***********************************************************
*  Function: gw_get_rssi
*  Input: none
*  Output: p_rssi if(*p_rssi == 99) then do't support get rssi
*  Return: OPERATE_RET
***********************************************************/
__GW_INTF_EXT \
OPERATE_RET gw_get_rssi(SCHAR_T *p_rssi);

#if defined(WIFI_GW) && (WIFI_GW==1)
/***********************************************************
*  Function: get_wf_gw_nw_status
*  Input:
*  Output:nw_stat
*  Return:
***********************************************************/
__GW_INTF_EXT \
OPERATE_RET get_wf_gw_nw_status(OUT GW_WIFI_NW_STAT_E *nw_stat);
#else
/***********************************************************
*  Function: get_base_gw_nw_status
*  Input:
*  Output:
*  Return:
***********************************************************/
__GW_INTF_EXT \
OPERATE_RET get_base_gw_nw_status(OUT GW_BASE_NW_STAT_T *nw_stat);
#endif

__GW_INTF_EXT \
VOID gw_register_pre_gw_ug_cb(IN PRE_GW_UG_INFORM_CB pre_gw_cb);


#if defined(WIFI_GW) && (WIFI_GW==1)
/***********************************************************
*  Function: gw_wifi_user_cfg
*  Input: ssid passwd token
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__GW_INTF_EXT \
OPERATE_RET gw_wifi_user_cfg(IN CONST CHAR_T *ssid,IN CONST CHAR_T *passwd,IN CONST CHAR_T *token);
#endif

/***********************************************************
*  Function: get_cloud_dev_tpye
*  Input: local_tp
*  Output: none
*  Return: CLOUD_DEV_TP_DEF_T
***********************************************************/
__GW_INTF_EXT \
CLOUD_DEV_TP_DEF_T get_cloud_dev_tpye(IN CONST DEV_TYPE_T local_tp);

/***********************************************************
*  Function: get_local_dev_tpye
*  Input: cloud_tp
*  Output: none
*  Return: DEV_TYPE_T
***********************************************************/
__GW_INTF_EXT \
DEV_TYPE_T get_local_dev_tpye(IN CONST CLOUD_DEV_TP_DEF_T cloud_tp);

/***********************************************************
*  Function: gw_bind_device
*  Input: tp uddd id pk ver
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
#if defined(ENABLE_SIGMESH) && (ENABLE_SIGMESH==1)
__GW_INTF_EXT \
OPERATE_RET gw_bind_device(IN CONST GW_PERMIT_DEV_TP_T tp,IN CONST USER_DEV_DTL_DEF_T uddd,\
                           IN CONST CHAR_T *id,IN CONST CHAR_T *pk,IN CONST CHAR_T *firmware_key, IN CONST CHAR_T *ver, \
                           IN CONST GW_ATTACH_ATTR_T *attr,IN CONST UINT_T attr_num, IN CONST BOOL_T oem, \
                           IN CONST CHAR_T *mac, IN CONST CHAR_T *uuid, IN CONST CHAR_T *dev_key);
#else
__GW_INTF_EXT \
OPERATE_RET gw_bind_device(IN CONST GW_PERMIT_DEV_TP_T tp,IN CONST USER_DEV_DTL_DEF_T uddd,\
                           IN CONST CHAR_T *id,IN CONST CHAR_T *pk, IN CONST CHAR_T *firmware_key, IN CONST CHAR_T *ver, \
                           IN CONST GW_ATTACH_ATTR_T *attr,IN CONST UINT_T attr_num, IN CONST BOOL_T oem);
#endif
/***********************************************************
*  Function: gw_subdevice_update
*  Input: id ver
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__GW_INTF_EXT \
OPERATE_RET gw_subdevice_update(IN CONST CHAR_T *id,IN CONST CHAR_T *ver, IN CONST GW_ATTACH_ATTR_T *attr,IN CONST UINT_T attr_num, IN CONST BOOL_T is_force);

/***********************************************************
*  Function: gw_bind_device_inner
*  Input: dev_cntl schema
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__GW_INTF_EXT \
OPERATE_RET gw_bind_device_inner(IN CONST DEV_CNTL_N_S *dev_cntl,IN CONST CHAR_T *schema);

/***********************************************************
*  Function: gw_unbind_device_inner
*  Input: dev_cntl
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__GW_INTF_EXT \
OPERATE_RET gw_unbind_device_inner(IN CONST DEV_CNTL_N_S *dev_cntl);

/***********************************************************
*  Function: gw_unbind_device
*  Input: dev_cntl
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__GW_INTF_EXT \
OPERATE_RET gw_unbind_device(IN CONST DEV_CNTL_N_S *dev_cntl);

__GW_INTF_EXT \
VOID gw_dev_lock(VOID);

__GW_INTF_EXT \
VOID gw_dev_unlock(VOID);

__GW_INTF_EXT \
OPERATE_RET dev_unregister(IN CONST DEV_RESET_S *rst_t);

/***********************************************************
*  Function: gw_dev_traversal
*  Input: iterator->if(*iterator == NULL) then get the first device
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__GW_INTF_EXT \
DEV_DESC_IF_S *gw_dev_traversal_unlock(BOOL_T include_gw, INOUT VOID **iterator);

__GW_INTF_EXT \
VOID gw_oem_set(IN CONST BOOL_T oem);

__GW_INTF_EXT \
BOOL_T is_gw_oem(VOID);

__GW_INTF_EXT \
OPERATE_RET dev_update_grp_info(IN CONST GRP_ACTION_E action,IN CONST CHAR_T *dev_id, \
                                      IN CONST CHAR_T *grp_id);

__GW_INTF_EXT \
OPERATE_RET dev_update_scene_info(IN CONST SCE_ACTION_E action,IN CONST CHAR_T *dev_id, \
                                  IN CONST CHAR_T *grp_id, IN CONST CHAR_T *sce_id);

__GW_INTF_EXT \
OPERATE_RET gw_upload_log(CHAR_T *cData, UINT_T iLen);

__GW_INTF_EXT \
OPERATE_RET gw_reset_device(IN CONST DEV_CNTL_N_S *dev_cntl);

__GW_INTF_EXT \
OPERATE_RET gw_update_versions(VOID);

#if defined(WIFI_GW) && (WIFI_GW==1)
__GW_INTF_EXT \
OPERATE_RET gw_fast_unactive(IN CONST GW_WF_CFG_MTHD_SEL mthd,IN CONST GW_WF_START_MODE wifi_start_mode);

__GW_INTF_EXT \
OPERATE_RET gw_fast_get_nc_type(GW_WF_NWC_FAST_STAT_T *nc_type);
#endif
#if defined(ENABLE_SIGMESH) && (ENABLE_SIGMESH==1)
__GW_INTF_EXT \
OPERATE_RET get_gw_sigmesh_net_info(TY_SIGMESH_NET_INFO_S *sigmesh_net_info);

__GW_INTF_EXT \
OPERATE_RET get_dev_sigmesh_free_node_list(IN CONST INT_T node_num, INOUT TY_SIGMESH_NODE_LIST_S *node_alloc_list);

#endif

#ifdef __cplusplus
}
#endif
#endif

