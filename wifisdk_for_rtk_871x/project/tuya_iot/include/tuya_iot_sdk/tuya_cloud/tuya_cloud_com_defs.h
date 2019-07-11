/*
tuya_cloud_com_defs.h
Copyright(C),2018-2020, 涂鸦科技 www.tuya.comm
*/

#ifndef TUYA_CLOUD_COM_DEFS_H
#define TUYA_CLOUD_COM_DEFS_H

#include "tuya_cloud_types.h"

#ifdef __cplusplus
extern "C" {
#endif

// tuya sdk constants definitions
#define ENCRYPT_KEY_LEN         16      // max string length of storage encrypt
#define GW_UUID_LEN             25      // max string length of UUID
#define AUTH_KEY_LEN            32      // max string length of AUTH_KEY
#define PRODUCT_KEY_LEN         16      // max string length of PRODUCT_KEY
#define SW_VER_LEN              10      // max string length of VERSION

#define GW_ID_LEN               25      // max string length of GW_ID
#define DEV_ID_LEN              25      // max string length of DEV_ID
#define SCHEMA_ID_LEN           16      // max string length of SCHEMA
#define BIND_TOKEN_LEN          16      // max string length of BIND_TOKEN
#define TOKEN_LEN               8       // max string length of TOKEN
#define REGION_LEN              2       // max string length of REGIN IN TOKEN
#define REGIST_KEY_LEN          4       // max string length of REGIST_KEY IN TOKEN
#define HTTP_URL_LMT            128     // max string length of URL
#define MQ_URL_LMT              128     // max string length of MQTT_URL
#define TIME_ZONE_LEN           10      // max string length of TIME_ZONE
#define SUMMER_TM_ZONE_LEN      256     // max string length of SUMMER_TIME_ZONE
#define COUNTRY_CODE_LEN        8       // max string length of COUNTRY_CODE
#define COUNTRY_CODE_LEN        8       // max string length of COUNTRY_CODE
#define IPV4_LEN                15      // max string length of IPV4_LEN 255.255.255.255

#define SEC_KEY_LEN             16      // max string length of SECURITY_KEY
#define LOCAL_KEY_LEN           16      // max string length of LOCAL_KEY

#define WXAPPID_LEN             32      // max string length of wechat_appid
#define WXUUID_LEN              32      // max string length of wechat_uuid

#define GRP_ID_LEN              5       // max string length of group id range:1-65535
#define SCENE_ID_LEN            3       // max string length of scene id range:1-255

#if defined(ENABLE_SIGMESH) && (ENABLE_SIGMESH==1)
#define SIGMESH_NET_KEY_LEN 32
#define SIGMESH_APP_KEY_LEN 32
#define SIGMESH_DEV_KEY_LEN 32
#define SIGMESH_DEV_MAC_LEN 32
#endif

#define LC_GW_SLEEP_HB_LMT_S 12*3600 // 12h
//group and scene error code
#define M_SCE_SUCCESS 0        //成功
#define M_SCE_ERR_EXCEED 1     //空间不足
#define M_SCE_ERR_TIMEOUT 2    //通讯超时
#define M_SCE_ERR_PARAM 3      //参数超出范围
#define M_SCE_WRITE_FILE 4     //写数据库文件错误
#define M_SCE_ERR_OTHER 5      //其他错误

// Product Function Specification
typedef BYTE_T GW_ABI;
#define GW_VIRTUAL 0x00 // product doesn't have subdevices functions,it only has device function. For example: SOC or MCU
#define GW_ENTITY_ATTH_DEV 0x01 // Product has subdevices functions and it also has device function.
#define GW_ENTITY 0x02 // Product only has subdevices functions.

// Product Network Connection
typedef BYTE_T GW_NW_STAT_T;
#define GNS_UNVALID 0    // product is offline in LAN and WAN
#define GNS_LAN_VALID 1  // product is online in LAN but offline in WAN
#define GNS_WAN_VALID 2  // product is online in LAN and WAN

// Product Extend State
typedef BYTE_T GW_EXT_STAT_T;
#define EXT_UNAUTH 0
#define EXT_PROD_TEST 1
#define EXT_NORMAL_S 2
#define EXT_GW_UPGRD 3
#define EXT_DEV_UPGRD 4
#define EXT_DEV_ADD 5

// Subdevice Type IN Local Namespace
typedef BYTE_T DEV_TYPE_T;
#define DEV_NM_ATH_SNGL 0       // netlink module attach single device,(devid == gwid)
#define DEV_NM_NOT_ATH_SNGL 1   // netlink module not attach single device,(devid == gwid)
#define DEV_ZB_SNGL 2           // zigbee single device
#define DEV_INFRARED_SNGL 3     // infrared single device
#define DEV_RF433_SNGL 4        // rf433 single device
#define DEV_BLE_SNGL 5          // ble single device

// sub-device detail type
typedef UINT_T USER_DEV_DTL_DEF_T; // user detial type define

typedef BYTE_T GW_PERMIT_DEV_TP_T;
#define GP_DEV_ZB DEV_ZB_SNGL
#define GP_DEV_RF433 DEV_RF433_SNGL
#define GP_DEV_BLE DEV_BLE_SNGL
#define GP_DEV_INFRARED DEV_INFRARED_SNGL

typedef struct {
    BYTE_T encrypt_key[ENCRYPT_KEY_LEN];
    BOOL_T init_db;
}TY_INIT_PARAMS_S;

typedef struct {
    GW_PERMIT_DEV_TP_T tp;
    CHAR_T ver[SW_VER_LEN+1]; // xx.xx.xx
}GW_ATTACH_ATTR_T;

#define GW_ATTACH_ATTR_LMT 4
typedef struct {
    CHAR_T id[DEV_ID_LEN+1];
    CHAR_T sw_ver[SW_VER_LEN+1];
    CHAR_T schema_id[SCHEMA_ID_LEN+1];
    CHAR_T product_key[PRODUCT_KEY_LEN+1];
    CHAR_T firmware_key[PRODUCT_KEY_LEN+1];
    BOOL_T is_oem;
#if defined(ENABLE_SIGMESH) && (ENABLE_SIGMESH==1)    
    CHAR_T sigmesh_dev_key[SIGMESH_DEV_KEY_LEN+1];
    CHAR_T sigmesh_mac[SIGMESH_DEV_MAC_LEN+1];
#endif
    USER_DEV_DTL_DEF_T uddd; // user detial type define
    DEV_TYPE_T tp;
    BOOL_T bind;
    BOOL_T sync;
#if defined(ENABLE_SIGMESH) && (ENABLE_SIGMESH==1)
    BOOL_T sigmesh_sync;
#endif
    BYTE_T attr_num;
    GW_ATTACH_ATTR_T attr[GW_ATTACH_ATTR_LMT];
    BOOL_T reset_flag;
}DEV_DESC_IF_S;

// dp type
typedef BYTE_T DP_TYPE_E;
#define T_OBJ           0           // dp is value,str,enum,bool,bitmap
#define T_RAW           1           // raw type
#define T_FILE          2           // file type

// dp mode
typedef BYTE_T DP_MODE_E;
#define M_RW            0          // cloud/app can read and write
#define M_WR            1          // cloud/app can only write to device
#define M_RO            2          // cloud/app can only read from device

// dp schema type
typedef BYTE_T DP_PROP_TP_E;
#define PROP_BOOL 0
#define PROP_VALUE 1
#define PROP_STR 2
#define PROP_ENUM 3
#define PROP_BITMAP 4

typedef BYTE_T UP_MAP_TYPE_E;
#define UP_CLEANER_MAP   0
#define UP_CLEANER_PATH  1
#define UP_CLEANER_MAP_PATH 2

// dp prop
typedef struct {
    INT_T min;
    INT_T max;
    SHORT_T step;
    USHORT_T scale;
    INT_T value;
}DP_PROP_VAL_S;

typedef struct {
    INT_T cnt;
    CHAR_T **pp_enum;
    INT_T value;
}DP_PROP_ENUM_S;

typedef struct {
    INT_T max_len;
    CHAR_T *value;
}DP_PROP_STR_S;

typedef struct {
    BOOL_T value;
}DP_BOOL_S;

typedef struct {
    UINT_T max_len;
    UINT_T value;
}DP_PROP_BITMAP;

typedef union {
    DP_PROP_VAL_S prop_value;
    DP_PROP_ENUM_S prop_enum;
    DP_PROP_STR_S prop_str;
    DP_BOOL_S prop_bool;
    DP_PROP_BITMAP prop_bitmap;
}DP_PROP_VALUE_U;

typedef BYTE_T DP_TRIG_T_E;
#define TRIG_PULSE      0   // upload when value changes
#define TRIG_DIRECT     1   // upload without value change check

/* tuya sdk dp positive upload policy */
typedef BYTE_T DP_PSV_E;
#define PSV_FALSE 0    /* disabled */
#define PSV_TRUE 1     /* app hasn't query yet */
#define PSV_F_ONCE 2   /* app has queryed, shoulb report */

// dp statistics type
typedef BYTE_T DP_STAT_TP_T;
#define DST_NONE 0 // no need statistics
#define DST_INC 1 // dp statistics increase
#define DST_TOTAL 2 // dp statistics total

typedef struct {
    BYTE_T dp_id;
    DP_MODE_E mode;
    DP_PSV_E passive;  /* true false once */
    DP_TYPE_E type;   /* obj raw file */
    DP_PROP_TP_E prop_tp;
    DP_TRIG_T_E trig_t;
    DP_STAT_TP_T stat; /* none total increase */
}DP_DESC_IF_S;

/* tuya sdk dp cmd type */
typedef BYTE_T DP_CMD_TYPE_E;
#define DP_CMD_LAN      0       // cmd from LAN
#define DP_CMD_MQ       1       // cmd from MQTT
#define DP_CMD_TIMER    2       // cmd from Local Timer
#define DP_CMD_SCENE_LINKAGE 3  // cmd from scene linkage
#define DP_CMD_RELIABLE_TRANSFER 4 // cmd from reliable transfer


typedef BYTE_T DP_TRANS_TYPE_T;
#define DTT_SCT_UNC     0       // unicast
#define DTT_SCT_BNC     1       // boardcast
#define DTT_SCT_MNC     2       // multicast
#define DTT_SCT_SCENE   3       // scene

/* tuya sdk dp value union */
typedef union {
    INT_T dp_value;             // valid when dp type is value
    UINT_T dp_enum;             // valid when dp type is enum
    CHAR_T *dp_str;             // valid when dp type is str
    BOOL_T dp_bool;             // valid when dp type is bool
    UINT_T dp_bitmap;           // valid when dp type is bitmap
}TY_OBJ_DP_VALUE_U;

/* tuya sdk obj dp manage info */
typedef struct {
    BYTE_T dpid;                // dp id
    DP_PROP_TP_E type;          // dp type
    TY_OBJ_DP_VALUE_U value;    // dp value
    UINT_T time_stamp;          // dp happen time. if 0, mean now
}TY_OBJ_DP_S;

typedef struct {
    DP_CMD_TYPE_E cmd_tp;
    DP_TRANS_TYPE_T dtt_tp;
    CHAR_T *cid; // if(NULL == cid) then then the cid represents gwid
    CHAR_T *mb_id;
    UINT_T dps_cnt;
    TY_OBJ_DP_S dps[0];
}TY_RECV_OBJ_DP_S;

typedef struct {
    DP_CMD_TYPE_E cmd_tp;
    DP_TRANS_TYPE_T dtt_tp;
    CHAR_T *cid; // if(NULL == cid) then then the cid represents gwid
    BYTE_T dpid;
    CHAR_T *mb_id;
    UINT_T len;
    BYTE_T data[0];
}TY_RECV_RAW_DP_S;

typedef struct {
    CHAR_T *cid; // if(NULL == cid) then then the cid represents gwid
    UINT_T cnt; // dpid cnt if(0 == cnt) then query all object dp
    BYTE_T dpid[0]; // dpid
}TY_DP_QUERY_S;

#define FW_URL_LEN      255       // max length of firmware download url
#define FW_MD5_LEN      32        // max length of firmware md5
#define FW_HMAC_LEN     64        // max length of firmware hmac

/* tuya sdk ota firmware info */
typedef struct {
    DEV_TYPE_T tp;      // firmware type
    CHAR_T fw_url[FW_URL_LEN+1];  // firmware download url
    CHAR_T sw_ver[SW_VER_LEN+1];  // firmware version
    UINT_T file_size;             // firmware size in BYTE
    CHAR_T fw_hmac[FW_HMAC_LEN+1];  // firmware hmac
}FW_UG_S;

#if defined(ENABLE_SIGMESH) && (ENABLE_SIGMESH==1)
typedef struct {
    CHAR_T net_key[SIGMESH_NET_KEY_LEN+1];
    CHAR_T app_key[SIGMESH_APP_KEY_LEN+1];
}TY_SIGMESH_NET_INFO_S;

typedef struct {
    INT_T nodeId_num;
    USHORT_T *node_list;
}TY_SIGMESH_NODE_LIST_S;
#endif

/***********************************************************
*  callback function: DEV_HEARTBEAT_SEND_CB
*  Desc:    send heartbeat to subdevice callback
*  Input:   dev_id: subdevice id
***********************************************************/
typedef VOID (*DEV_HEARTBEAT_SEND_CB)(IN CONST CHAR_T *dev_id);

/* tuya sdk gateway status info */
typedef BYTE_T GW_STATUS_E;
#define GW_RESET            0 // gw reset
#define GW_ACTIVED          1 // gw actived
#define GW_FIRST_START      2 // start tuya-sdk in the first time.
#define GW_NORMAL           3 // tuya-sdk is actived and started

/***********************************************************
*  callback function: GW_STATUS_CHANGED_CB
*  Desc:    gw status changed callback
*  Input:   status: current status
***********************************************************/
typedef VOID (*GW_STATUS_CHANGED_CB)(IN CONST GW_STATUS_E status);

/***********************************************************
*  callback function: DEV_OBJ_DP_CMD_CB
*  Desc:    obj dp info cmd callback
*  Input:   dp: obj dp info
***********************************************************/
typedef VOID (*DEV_OBJ_DP_CMD_CB)(IN CONST TY_RECV_OBJ_DP_S *dp);

/***********************************************************
*  callback function: DEV_RAW_DP_CMD_CB
*  Desc:    raw dp info cmd callback
*  Input:   dp: raw dp info
***********************************************************/
typedef VOID (*DEV_RAW_DP_CMD_CB)(IN CONST TY_RECV_RAW_DP_S *dp);

/***********************************************************
*  callback function: DEV_DP_QUERY_CB
*  Desc:    dp info query callback
*  Input:   dp_qry: query info
***********************************************************/
typedef VOID (*DEV_DP_QUERY_CB)(IN CONST TY_DP_QUERY_S *dp_qry);

/***********************************************************
*  callback function: GW_UG_INFORM_CB
*  Desc:    gateway ota firmware available nofity callback.
*  Input:   fw: firmware info
***********************************************************/
typedef VOID (*GW_UG_INFORM_CB)(IN CONST FW_UG_S *fw);


/* tuya sdk gateway reset type */
typedef enum {
    GW_LOCAL_RESET_FACTORY = 0,
    GW_REMOTE_UNACTIVE,
    GW_LOCAL_UNACTIVE,
    GW_REMOTE_RESET_FACTORY,
    GW_RESET_DATA_FACTORY, //need clear local data when active
}GW_RESET_TYPE_E;
/***********************************************************
*  callback function: GW_RESET_IFM_CB
*  Desc:    gateway restart callback
***********************************************************/
typedef VOID (*GW_RESET_IFM_CB)(GW_RESET_TYPE_E type);

/***********************************************************
*  callback function: DEV_UG_INFORM_CB
*  Desc:    subdevice ota firmware available nofity callback.
*  Input:   dev_id: subdevice id
*  Input:   fw: firmware info
***********************************************************/
typedef VOID (*DEV_UG_INFORM_CB)(IN CONST CHAR_T *dev_id,IN CONST FW_UG_S *fw);

typedef enum {
    DEV_REMOTE_RESET_FACTORY,
    DEV_RESET_DATA_FACTORY, //need clear local data when bind
}DEV_RESET_TYPE_E;
/***********************************************************
*  callback function: DEV_RESET_IFM_CB
*  Desc:    subdevice reset callback
***********************************************************/
typedef VOID (*DEV_RESET_IFM_CB)(IN CONST CHAR_T *dev_id,IN DEV_RESET_TYPE_E type);

/***********************************************************
*  callback function: GET_FILE_DATA_CB
*  Desc:    firmware download content process callback.
*  Input:   fw: firmware info
*  Input:   total_len: firmware total size
*  Input:   offset: offset of this download package
*  Input:   data && len: this download package
*  Output:  remain_len: the size left to process in next cb.
*  Input:   pri_data: private data
*  Return:  OPRT_OK: success  Other: fail
***********************************************************/
typedef OPERATE_RET (*GET_FILE_DATA_CB)(IN CONST FW_UG_S *fw, IN CONST UINT_T total_len,IN CONST UINT_T offset,\
                                                IN CONST BYTE_T *data,IN CONST UINT_T len,OUT UINT_T *remain_len, IN PVOID_T pri_data);

/***********************************************************
*  callback function: UPGRADE_NOTIFY_CB
*  Desc:    firmware download finish result callback.
*  Input:   fw: firmware info
*  Input:   download_result: 0 means download succes. other means fail
*  Input:   pri_data: private data
***********************************************************/
typedef VOID (*UPGRADE_NOTIFY_CB)(IN CONST FW_UG_S *fw, IN CONST INT_T download_result, IN PVOID_T pri_data);

/* tuya-sdk gateway callback functions */
typedef struct {
    GW_STATUS_CHANGED_CB gw_status_cb;
    GW_UG_INFORM_CB gw_ug_cb;
    GW_RESET_IFM_CB gw_reset_cb;
    DEV_OBJ_DP_CMD_CB dev_obj_dp_cb;
    DEV_RAW_DP_CMD_CB dev_raw_dp_cb;
    DEV_DP_QUERY_CB dev_dp_query_cb;
    DEV_UG_INFORM_CB dev_ug_cb;
    DEV_RESET_IFM_CB dev_reset_cb;
}TY_IOT_CBS_S;

/***********************************************************
*  callback function: GW_PERMIT_ADD_DEV_CB
*  Desc:    permit to add subdevices callback
*  Input:   tp      subdevice type
*  Input:   permit  is permit to add subdevices
*  Output:  timeout timeout
*  Return:  BOOL_T  if function process success, return true, if function process fails,return false.
***********************************************************/
typedef BOOL_T (*GW_PERMIT_ADD_DEV_CB)(IN CONST GW_PERMIT_DEV_TP_T tp,IN CONST BOOL_T permit,IN CONST UINT_T timeout);

/***********************************************************
*  callback function: GW_DEV_DEL_CB
*  Desc:    when a subdevice is deleted,use this callback to notify user
*  Input:   dev_id  subdevice_id
***********************************************************/
typedef VOID (*GW_DEV_DEL_CB)(IN CONST CHAR_T *dev_id);

/* tuya-sdk group process type */
typedef enum {
    GRP_ADD = 0, // add a group
    GRP_DEL      // delete a group
}GRP_ACTION_E;

/***********************************************************
*  callback function: GW_DEV_GRP_INFM_CB
*  Desc:    the group process command callback
*  Input:   action  group action type
*  Input:   dev_id  subdevice_id
*  Input:   grp_id  group_id
*  Return:  OPRT_OK:bind success. other values:bind failure
***********************************************************/
typedef OPERATE_RET (*GW_DEV_GRP_INFM_CB)(IN CONST GRP_ACTION_E action,IN CONST CHAR_T *dev_id,IN CONST CHAR_T *grp_id);

/* tuya-sdk scene process type */
typedef enum {
    SCE_ADD = 0, // add a scene
    SCE_DEL,     // delete a scene
    SCE_EXEC     // execute a scene
}SCE_ACTION_E;

/***********************************************************
*  callback function: GW_DEV_SCENE_INFM_CB
*  Desc:    the scene process command callback
*  Input:   action  scene action type
*  Input:   dev_id  subdevice_id
*  Input:   grp_id  group_id
*  Input:   sce_id  scene_id
*  Return:  OPRT_OK:bind success. other values:bind failure
***********************************************************/
typedef OPERATE_RET (*GW_DEV_SCENE_INFM_CB)(IN CONST SCE_ACTION_E action,IN CONST CHAR_T *dev_id,\
                                                 IN CONST CHAR_T *grp_id,IN CONST CHAR_T *sce_id);

/***********************************************************
*  callback function: GW_BIND_DEV_INFORM_CB
*  Desc:    the subdevice bind result callback
*  Input:   dev_id: subdevice_id
*  Input:   op_ret  OPRT_OK:bind success. other values:bind failure
***********************************************************/
typedef VOID (*GW_BIND_DEV_INFORM_CB)(IN CONST CHAR_T *dev_id, IN CONST OPERATE_RET op_ret);

#if defined(ENABLE_SIGMESH) && (ENABLE_SIGMESH==1)
typedef VOID (*GW_DEV_SIGMESH_TOPO_UPDAET_CB)(IN CONST CHAR_T *dev_id);
#endif

/* tuya-sdk gateway subdevices management callback functions */
typedef struct {
    GW_PERMIT_ADD_DEV_CB gw_add_dev_cb;
    GW_DEV_DEL_CB gw_del_cb;
    GW_DEV_GRP_INFM_CB gw_dev_grp_cb;
    GW_DEV_SCENE_INFM_CB gw_dev_scene_cb;
    GW_BIND_DEV_INFORM_CB gw_ifm_cb;
#if defined(ENABLE_SIGMESH) && (ENABLE_SIGMESH==1)
    GW_DEV_SIGMESH_TOPO_UPDAET_CB gw_dev_sigmesh_topo_update_cb;
#endif
}TY_IOT_GW_CBS_S;

// mqtt media data interface
typedef BYTE_T TRAN_STEP_T;
#define TS_START 0
#define TS_TRANSFER 1
#define TS_END 2

#pragma pack(1)
typedef struct {
    USHORT_T id;
    UINT_T posix;
    TRAN_STEP_T step;
    UINT_T offset;
    USHORT_T len;
    BYTE_T data[0];
}FLOW_BODY_ST;
#pragma pack()


typedef struct {
    CHAR_T region[REGION_LEN + 1];
    CHAR_T time_zone[TIME_ZONE_LEN+1];
}TY_IOT_REGION_INFO_S;


typedef enum {
    CFG_DEVICE = 1,
    CFG_PRODUCT = 2,
    CFG_INVALID,
}TY_CUSTOM_CFG_E;


typedef struct {
    CHAR_T country_code[COUNTRY_CODE_LEN + 1];
    CHAR_T ip[IPV4_LEN + 1];
}TY_LOCATION_INFO_S;




#ifdef __cplusplus
}
#endif

#endif
