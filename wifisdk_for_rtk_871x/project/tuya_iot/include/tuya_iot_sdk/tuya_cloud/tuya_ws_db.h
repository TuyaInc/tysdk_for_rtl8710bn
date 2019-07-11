/***********************************************************
*  File: tuya_ws_db.h
*  Author: nzy
*  Date: 20150601
***********************************************************/
#ifndef _TUYA_WS_DB_H
#define _TUYA_WS_DB_H

#include "tuya_cloud_types.h"
#include "gw_intf.h"

#ifdef __cplusplus
    extern "C" {
#endif

#ifdef  __TUYA_WS_DB_GLOBALS
    #define __TUYA_WS_DB_EXT
#else
    #define __TUYA_WS_DB_EXT extern
#endif


/***********************************************************
*  Function: ws_db_init
*  Input:
*  Output:
*  Return: OPERATE_RET
***********************************************************/
__TUYA_WS_DB_EXT \
OPERATE_RET ws_db_init(IN CONST CHAR_T *p_attr, IN CONST BYTE_T *p_encrypt_key, IN CONST UINT_T encrypt_key_len);

/***********************************************************
*  Function: ws_db_uninit
*  Input: none
*  Output: none
*  Return: none
***********************************************************/
__TUYA_WS_DB_EXT \
VOID ws_db_uninit(VOID) ;

/***********************************************************
*  Function: wd_common_write
*  Input:key value len
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__TUYA_WS_DB_EXT \
OPERATE_RET wd_common_write(IN CONST CHAR_T *key, IN CONST BYTE_T *value, IN CONST UINT_T len);

/***********************************************************
*  Function: wd_common_read
*  Input: key
*  Output: value p_len
*  Return: OPERATE_RET
***********************************************************/
__TUYA_WS_DB_EXT \
OPERATE_RET wd_common_read(IN CONST CHAR_T *key, OUT BYTE_T **value, OUT UINT_T *p_len);

/***********************************************************
*  Function: wd_common_free_data
*  Input: data
*  Output:
*  Return: OPERATE_RET
***********************************************************/
__TUYA_WS_DB_EXT \
OPERATE_RET wd_common_free_data(IN BYTE_T *data);

/***********************************************************
*  Function: wd_common_delete
*  Input: key
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__TUYA_WS_DB_EXT \
OPERATE_RET wd_common_delete(IN CONST CHAR_T *key);

/***********************************************************
*  Function: wd_common_fuzzy_delete
*  Input: key
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__TUYA_WS_DB_EXT \
OPERATE_RET wd_common_fuzzy_delete(IN CONST CHAR_T *key);

/***********************************************************
*  Function: wd_gw_base_if_write
*  Input: bi
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__TUYA_WS_DB_EXT \
OPERATE_RET wd_gw_base_if_write(IN GW_BASE_IF_S *bi);

/***********************************************************
*  Function: wd_gw_base_if_read
*  Input: none
*  Output: bi
*  Return: OPERATE_RET
***********************************************************/
__TUYA_WS_DB_EXT \
OPERATE_RET wd_gw_base_if_read(OUT GW_BASE_IF_S *bi);

/***********************************************************
*  Function: wd_gw_wsm_write
*  Input: wsm
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__TUYA_WS_DB_EXT \
OPERATE_RET wd_gw_wsm_write(IN GW_WORK_STAT_MAG_S *wsm);

/***********************************************************
*  Function: wd_gw_wsm_read
*  Input: none
*  Output: wsm
*  Return: OPERATE_RET
***********************************************************/
__TUYA_WS_DB_EXT \
OPERATE_RET wd_gw_wsm_read(OUT GW_WORK_STAT_MAG_S *wsm);

/***********************************************************
*  Function: wd_gw_desc_if_write
*  Input: di
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__TUYA_WS_DB_EXT \
OPERATE_RET wd_gw_desc_if_write(IN GW_DESC_IF_S *di);

/***********************************************************
*  Function: wd_gw_desc_if_read
*  Input: none
*  Output: di
*  Return: OPERATE_RET
***********************************************************/
__TUYA_WS_DB_EXT \
OPERATE_RET wd_gw_desc_if_read(OUT GW_DESC_IF_S *di);

/***********************************************************
*  Function: wd_gw_actv_if_write
*  Input: ai
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__TUYA_WS_DB_EXT \
OPERATE_RET wd_gw_actv_if_write(IN GW_ACTV_IF_S *ai);

/***********************************************************
*  Function: wd_gw_actv_if_read
*  Input: none
*  Output: ai
*  Return: OPERATE_RET
***********************************************************/
__TUYA_WS_DB_EXT \
OPERATE_RET wd_gw_actv_if_read(OUT GW_ACTV_IF_S *ai);

/***********************************************************
*  Function: wd_dev_if_write
*  Input: ddi
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__TUYA_WS_DB_EXT \
OPERATE_RET wd_dev_if_write(IN DEV_DESC_IF_S *ddi);

/***********************************************************
*  Function: wd_dev_if_delete_all
*  Input: none
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__TUYA_WS_DB_EXT \
OPERATE_RET wd_dev_if_delete_all(VOID);

__TUYA_WS_DB_EXT \
OPERATE_RET wd_dev_if_delete(IN CONST CHAR_T *p_dev_id);



/***********************************************************
*  Function: wd_dev_if_read
*  Input: index->from 0
*  Output: index->the find index +1 ddi
*  Return: OPERATE_RET
***********************************************************/
__TUYA_WS_DB_EXT \
OPERATE_RET wd_dev_if_read(INOUT UINT_T *index,OUT DEV_DESC_IF_S *ddi);

/***********************************************************
*  Function: wd_dev_schema_write
*  Input: s_id schema
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__TUYA_WS_DB_EXT \
OPERATE_RET wd_dev_schema_write(IN CONST CHAR_T *s_id,IN CONST CHAR_T *schema);

/***********************************************************
*  Function: wd_dev_schema_read
*  Input: s_id
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__TUYA_WS_DB_EXT \
OPERATE_RET wd_dev_schema_read(IN CONST CHAR_T *s_id,OUT CHAR_T **schema);


/***********************************************************
*  Function: wd_dev_schema_read
*  Input: s_id
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__TUYA_WS_DB_EXT \
OPERATE_RET wd_dev_schema_delete(IN CONST CHAR_T *s_id);

/***********************************************************
*  Function: ws_db_user_param_write
*  Input: data len
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__TUYA_WS_DB_EXT \
OPERATE_RET ws_db_user_param_write(IN CONST BYTE_T *data,IN CONST UINT_T len);

/***********************************************************
*  Function: ws_db_user_param_read
*  Input: buf len
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__TUYA_WS_DB_EXT \
OPERATE_RET ws_db_user_param_read(OUT BYTE_T **buf,OUT UINT_T *len);

#if defined(ENABLE_AP_FAST_CONNECT) && (ENABLE_AP_FAST_CONNECT==1)
/***********************************************************
*  Function: ws_db_connect_ap_info_write
*  Input: connect_ap_info
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__TUYA_WS_DB_EXT \
OPERATE_RET ws_db_connect_ap_info_write(IN FAST_WF_CONNECTED_AP_INFO_S *connect_ap_info);


/***********************************************************
*  Function: ws_db_connect_ap_info_read
*  Input: none
*  Output: connect_ap_info
*  Return: OPERATE_RET
***********************************************************/
__TUYA_WS_DB_EXT \
OPERATE_RET ws_db_connect_ap_info_read(OUT FAST_WF_CONNECTED_AP_INFO_S *connect_ap_info);
#endif


#if defined(ENABLE_SUBDEVICE) && (ENABLE_SUBDEVICE==1)
/***********************************************************
*  Function: wd_dev_grp_if_write
*  Input: dev_id
*         grp_if
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__TUYA_WS_DB_EXT \
OPERATE_RET wd_dev_grp_if_write(IN CONST CHAR_T *dev_id,IN CONST GRP_MAG_S *grp);

/***********************************************************
*  Function: wd_dev_grp_if_read
*  Input: dev_id
*  Output: grp
*  Return: OPERATE_RET
***********************************************************/
__TUYA_WS_DB_EXT \
OPERATE_RET wd_dev_grp_if_read(IN CONST CHAR_T *dev_id,OUT GRP_MAG_S *grp);

/***********************************************************
*  Function: wd_dev_grp_if_delete
*  Input: dev_id
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__TUYA_WS_DB_EXT \
OPERATE_RET wd_dev_grp_if_delete(IN CONST CHAR_T *dev_id);

/***********************************************************
*  Function: wd_dev_grp_delete_all
*  Input: none
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__TUYA_WS_DB_EXT \
OPERATE_RET wd_dev_grp_delete_all(VOID);

/***********************************************************
*  Function: wd_dev_scene_if_write
*  Input: dev_id scene
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__TUYA_WS_DB_EXT \
OPERATE_RET wd_dev_scene_if_write(IN CONST CHAR_T *dev_id,IN CONST SCENE_MAG_S *scene);

/***********************************************************
*  Function: wd_dev_scene_if_read
*  Input: dev_id
*  Output: scene
*  Return: OPERATE_RET
***********************************************************/
__TUYA_WS_DB_EXT \
OPERATE_RET wd_dev_scene_if_read(IN CONST CHAR_T *dev_id,OUT SCENE_MAG_S *scene);

/***********************************************************
*  Function: wd_dev_scene_if_delete
*  Input: dev_id
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__TUYA_WS_DB_EXT \
OPERATE_RET wd_dev_scene_if_delete(IN CONST CHAR_T *dev_id);

/***********************************************************
*  Function: wd_dev_sce_delete_all
*  Input: none
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__TUYA_WS_DB_EXT \
OPERATE_RET wd_dev_sce_delete_all(VOID);

OPERATE_RET wd_encrypt_if_write(IN CONST CHAR_T *dev_id, BYTE_T *p_key);
OPERATE_RET wd_encrypt_if_read(IN CONST CHAR_T *dev_id, BYTE_T *p_key);
OPERATE_RET wd_encrypt_if_delete(IN CONST CHAR_T *dev_id);
OPERATE_RET wd_encrypt_delete_all(VOID);

#endif

typedef BYTE_T VAR_TP_T;
#define VT_CHAR 0
#define VT_BYTE 1
#define VT_SHORT 2
#define VT_USHORT 3
#define VT_INT 4
#define VT_BOOL 5
#define VT_STRING 6
#define VT_RAW 7

typedef struct {
    CHAR_T *key;
    VAR_TP_T vt;
    VOID *val;
    USHORT_T len;
}TY_DB_RW_S;


OPERATE_RET wd_utils_serialize(IN CONST TY_DB_RW_S *rw,IN CONST UINT_T rw_cnt,\
                               OUT CHAR_T **out,OUT UINT_T *out_len);
OPERATE_RET wd_utils_free_outbuf(IN CHAR_T *out_buf);
OPERATE_RET wd_utils_deserialize(IN CONST CHAR_T *in,INOUT TY_DB_RW_S *rw,IN CONST UINT_T rw_cnt);


OPERATE_RET wd_utils_serialize_save(IN CONST CHAR_T *key, IN CONST TY_DB_RW_S *rw,IN CONST UINT_T rw_cnt);
OPERATE_RET wd_utils_serialize_restore(IN CONST CHAR_T *key,INOUT TY_DB_RW_S *rw,IN CONST UINT_T rw_cnt);


#ifdef __cplusplus
}
#endif
#endif

