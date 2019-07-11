/***********************************************************
*  File: iot_httpc.h
*  Author: nzy
*  Date: 20150527
***********************************************************/
#ifndef _IOT_HTTPC_H
#define _IOT_HTTPC_H

#include "tuya_cloud_types.h"
#include "ty_cJSON.h"
#include "gw_intf.h"

#ifdef __cplusplus
    extern "C" {
#endif


OPERATE_RET httpc_common_post_no_remalloc(IN CONST CHAR_T *api_name, IN CONST CHAR_T *api_ver, IN CONST CHAR_T *uuid, IN CONST CHAR_T *devid, IN CHAR_T *post_data, IN UINT_T data_total_len,IN CONST CHAR_T *p_head_other, OUT ty_cJSON **pp_result);
OPERATE_RET httpc_common_post(IN CONST CHAR_T *api_name, IN CONST CHAR_T *api_ver, IN CONST CHAR_T *uuid, IN CONST CHAR_T *devid, IN CHAR_T *post_data, IN CONST CHAR_T *p_head_other, OUT ty_cJSON **pp_result);

OPERATE_RET http_gw_get_timer_cnt(IN CONST INT_T lastFetchTime, OUT ty_cJSON **result);
OPERATE_RET http_gw_get_timer_content(IN CONST INT_T offset, IN CONST INT_T limit, OUT ty_cJSON **result);

OPERATE_RET http_dev_chk_upgd_info(IN CONST CHAR_T *dev_id,IN CONST CLOUD_DEV_TP_DEF_T tp,OUT ty_cJSON **result);

OPERATE_RET http_dev_update_upgd_stat_v41(IN CONST CHAR_T *dev_id,IN CONST CLOUD_DEV_TP_DEF_T tp,IN CONST BYTE_T status);

OPERATE_RET http_device_bind(IN CONST CHAR_T *dev_id, IN CONST CHAR_T *product_key, IN CONST CHAR_T *firmware_key,\
                                 IN CONST CHAR_T *sw_ver, IN CONST BOOL_T online, IN CONST CLOUD_DEV_TP_DEF_T type, \
                                 IN CONST GW_ATTACH_ATTR_T *attr, IN CONST UINT_T attr_num, IN CONST BOOL_T is_oem,\
                                 OUT ty_cJSON **result);

OPERATE_RET http_device_unbind_v41(IN CONST CHAR_T *dev_id);
OPERATE_RET http_device_reset_v10(IN CONST CHAR_T *dev_id);


OPERATE_RET http_device_update_versions_v41(IN CONST CHAR_T *dev_id, IN CONST CHAR_T *ver, IN CONST CLOUD_DEV_TP_DEF_T tp, \
                                            IN CONST GW_ATTACH_ATTR_T *attr, IN CONST UINT_T attr_num);

OPERATE_RET http_gw_chk_upgd_info(IN CONST CLOUD_DEV_TP_DEF_T tp,OUT ty_cJSON **result);

OPERATE_RET http_gw_update_upgrade_status_v41(IN CONST CLOUD_DEV_TP_DEF_T tp,IN CONST BYTE_T status);

OPERATE_RET http_gw_reset(VOID);
//OPERATE_RET http_gw_check_exist(VOID);
OPERATE_RET http_gw_get_cloud_url(VOID);

OPERATE_RET http_gw_register_V41(OUT ty_cJSON **result);
#if defined(ENABLE_SIGMESH) && (ENABLE_SIGMESH==1)
OPERATE_RET http_get_dev_sigmesh_info_v10(IN CONST CHAR_T *sub_id, OUT ty_cJSON **result);
OPERATE_RET http_get_dev_sigmesh_free_node_list_v10(IN CONST INT_T node_num, OUT ty_cJSON **result);
OPERATE_RET http_dev_sigmesh_join_v10(IN CONST CHAR_T *node_id, IN CONST CHAR_T *product_key, IN CONST CHAR_T *firmware_key, IN CONST CHAR_T *uuid, \
                                      IN CONST CHAR_T *sw_ver, IN CONST CHAR_T *mac, IN CONST CHAR_T *dev_key, IN CONST BOOL_T online, IN CONST CLOUD_DEV_TP_DEF_T type,\
                                      IN CONST GW_ATTACH_ATTR_T *attr, IN CONST UINT_T attr_num, IN CONST BOOL_T is_oem, OUT ty_cJSON **result);
#endif

OPERATE_RET http_gw_active_v41(OUT ty_cJSON **result);

OPERATE_RET http_gw_update_version_v41(VOID);

typedef OPERATE_RET (*HTTP_GW_GET_FILE_DATA_CB)(IN PVOID_T priv_data, IN CONST UINT_T total_len,IN CONST UINT_T offset,\
                                            IN CONST BYTE_T *data,IN CONST UINT_T len,OUT UINT_T *remain_len);
OPERATE_RET http_gw_download_file(IN CONST CHAR_T *url,IN CONST UINT_T mlk_buf_len,\
                                  IN CONST HTTP_GW_GET_FILE_DATA_CB gfd_cb, IN PVOID_T priv_data, \
                                  IN CONST UINT_T total_len, IN CONST BYTE_T *p_key, IN CONST UINT_T key_len, INOUT BYTE_T file_hmac[32]);

OPERATE_RET wechat_gw_set_cloud_url(VOID);

OPERATE_RET http_put_rst_log(VOID);
OPERATE_RET http_get_self_fm_ug_info(IN CONST CHAR_T *sub_id, OUT ty_cJSON **result);
OPERATE_RET http_dynamic_cfg_get(OUT ty_cJSON **result);
OPERATE_RET http_dynamic_cfg_ack(IN CONST CHAR_T *ackid);
#if defined(ENABLE_SIGMESH) && (ENABLE_SIGMESH==1)
OPERATE_RET http_get_dev_list_v30(OUT ty_cJSON **result);
#else
OPERATE_RET http_get_dev_list_v20(OUT ty_cJSON **result);
#endif
OPERATE_RET http_device_update_skill(IN CONST CHAR_T *dev_id, IN CONST CHAR_T *skill);


#if defined(ENABLE_CLOUD_OPERATION) && (ENABLE_CLOUD_OPERATION==1)
OPERATE_RET httpc_domain_certs_get(OUT ty_cJSON **result, IN CONST CHAR_T *url_msg);
OPERATE_RET httpc_cloud_storage_cfg_get(OUT ty_cJSON **result, IN CONST BOOL_T withvideostorage);
OPERATE_RET httpc_cloud_storage_common_cfg_get(OUT ty_cJSON **result);
OPERATE_RET httpc_cloud_upload_complete(IN CONST UINT_T time, IN CONST CHAR_T *p_filetype, IN CONST CHAR_T *p_extend, IN CONST CHAR_T *p_bucket, IN CONST CHAR_T *p_file);
#endif

#if defined(ENABLE_LOCAL_LINKAGE) && (ENABLE_LOCAL_LINKAGE==1)
OPERATE_RET http_gw_get_linkage_content(IN CONST INT_T offset, IN CONST INT_T limit, OUT ty_cJSON **result);
#endif

OPERATE_RET http_upload_timer_log(IN CHAR_T *timer_info);

OPERATE_RET http_put_cloud_config(IN CONST CHAR_T *data);

/*
0: Normal Status
1: unactived but registered
2: unregistered
*/
OPERATE_RET http_gw_get_delete_status(INT_T *p_status);

OPERATE_RET http_secure_verify_v20(IN CONST CHAR_T *chipId, IN CONST CHAR_T *secureCode1, IN CONST CHAR_T *randomA, \
                 IN CONST CHAR_T *token,  IN CONST CHAR_T *region, OUT ty_cJSON **result);

OPERATE_RET http_gw_custom_cfg_get(IN CONST INT_T biztype, OUT ty_cJSON **result);

OPERATE_RET http_gw_location_info_get(OUT ty_cJSON **result);


OPERATE_RET http_dns_by_tuya(IN CONST CHAR_T *p_host_arr[], IN CONST UINT_T arr_size, OUT ty_cJSON **result);

typedef PVOID_T RAW_HTTP_S;
OPERATE_RET httpc_raw_read_resp_header(INOUT RAW_HTTP_S p_raw_http, OUT INT_T *p_status_code, OUT BOOL_T *p_chunked, OUT UINT_T *p_content_len);
INT_T httpc_raw_read_content(IN RAW_HTTP_S p_raw_http, IN BYTE_T *buf, IN CONST UINT_T buf_len);
INT_T httpc_raw_write_content(IN RAW_HTTP_S p_raw_http, IN CONST BYTE_T *buf, IN CONST UINT_T buf_len, BOOL_T chunked);
OPERATE_RET httpc_raw_close(IN RAW_HTTP_S p_raw_http);
OPERATE_RET httpc_raw_post(IN CONST CHAR_T *p_url, IN CONST UINT_T data_total_len, INOUT RAW_HTTP_S *pp_raw_http);
OPERATE_RET httpc_raw_get(IN CONST CHAR_T *p_url, INOUT RAW_HTTP_S *pp_raw_http, IN UINT_T offset, IN UINT_T total_len);
OPERATE_RET httpc_raw_put(IN CONST CHAR_T *p_url, IN CONST UINT_T data_total_len, INOUT RAW_HTTP_S *pp_raw_http);


#ifdef __cplusplus
}
#endif
#endif

