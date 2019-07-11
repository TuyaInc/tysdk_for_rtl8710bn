#ifndef _CLOUD_OPERATION_H_
#define _CLOUD_OPERATION_H_

#include "tuya_cloud_types.h"
#include "tuya_cloud_error_code.h"

#define VALUE_LEN       128
#define TOKEN_LEN_MAX   2048
#define FILE_NAME_LEN   64
#define URL_MAX_LEN 256
#define CA_TYPE_CLOUD_STROAGE 1
#define CA_TYPE_CLOUD_NOTIFICATION 2
#define CA_TYPE_ECHO_SHOW 3
#define CA_TYPE_CHROMECAST 4
#define CA_TYPE_PRESET 5

/*
结构体长度不能变动，否则使用时strncpy会报错
*/
typedef struct {
    CHAR_T provider[VALUE_LEN];
    CHAR_T ak[VALUE_LEN];
    CHAR_T sk[VALUE_LEN];
    CHAR_T expiration[VALUE_LEN];
    CHAR_T bucket[VALUE_LEN];
    CHAR_T endpoint[VALUE_LEN];
    CHAR_T region[VALUE_LEN];
    CHAR_T content_type[VALUE_LEN];

    CHAR_T media_path[VALUE_LEN];
    CHAR_T log_path[VALUE_LEN];
    CHAR_T detect_path[VALUE_LEN];
    CHAR_T common_path[VALUE_LEN];

    CHAR_T object[VALUE_LEN*2];
    CHAR_T token[TOKEN_LEN_MAX];
    CHAR_T store_mode[VALUE_LEN];
    UINT_T position;
    INT_T  post_mode;
}CLOUD_CONFIG_S;

typedef enum{
    UPLOAD_PATH_MEDIA = 0,
    UPLOAD_PATH_LOG,
    UPLOAD_PATH_DETECT,
    UPLOAD_PATH_COMMON,
    UPLOAD_PATH_PRESET,
    UPLOAD_PATH_MAX,
}CLOUD_UPLOAD_PATH_E;

OPERATE_RET cloud_operation_init(VOID);

OPERATE_RET cloud_upload_file_complete(IN CONST CHAR_T *p_filetype, IN CONST CHAR_T *p_extend,
                                       IN CONST CHAR_T *p_bucket, IN CONST CHAR_T *p_file);
OPERATE_RET cloud_get_common_storage_config(INOUT CLOUD_CONFIG_S *config);
OPERATE_RET cloud_get_storage_config(INOUT CLOUD_CONFIG_S *config, IN CONST BOOL_T withvideostorage);
OPERATE_RET cloud_require_new_ca(IN CONST CHAR_T *p_url,IN UINT_T ca_type);
OPERATE_RET cloud_put_data_to_file(IN CLOUD_CONFIG_S* config,
                                          IN CHAR_T *method,
                                          IN CLOUD_UPLOAD_PATH_E path,
                                          IN CHAR_T *file,
                                          IN CHAR_T* content_type,
                                          IN CHAR_T *data,
                                          IN INT_T len);

/* 返回content 长度，0无效 */
typedef UINT_T (*CLOUD_GET_CONTENT_LEN_CB)(VOID *pri_data);

/* 读取前处理
 * 返回: 成功->0 失败->非0
 * 输出: *p_malloc_buffer_size: 希望底层申请的读取buffer大小
 */
typedef INT_T (*CLOUD_BEFORE_READ_CONTENT_CB)(VOID *pri_data, OUT UINT_T *p_malloc_buffer_size);

/* 返回: 真实写入的字节数
 * 输入: p_buffer && buf_size :可供写入的buffer
 */
typedef INT_T (*CLOUD_READ_CONTENT_CB)(BYTE_T* p_buffer, INT_T buf_size, VOID *pri_data);

/* 读取后处理
 * 返回: 成功->0 失败->非0
 */
typedef INT_T (*CLOUD_AFTER_READ_CONTENT_CB)(VOID *pri_data);

typedef struct {
    CLOUD_GET_CONTENT_LEN_CB get_content_len_cb;
    CLOUD_BEFORE_READ_CONTENT_CB before_read_cb;
    CLOUD_READ_CONTENT_CB read_content_cb;
    CLOUD_AFTER_READ_CONTENT_CB after_read_cb;
    VOID *pri_data;
}CLOUD_CUSTOM_CONTENT_CTX_S;

OPERATE_RET cloud_put_custom_data(IN CLOUD_CONFIG_S* config,
                                  IN CHAR_T *method,
                                  IN CLOUD_UPLOAD_PATH_E path,
                                  IN CHAR_T *file,
                                  IN CHAR_T* content_type,
                                  IN CLOUD_CUSTOM_CONTENT_CTX_S *p_ctx);


OPERATE_RET post_file_to_oss(IN CLOUD_CONFIG_S* config,
                    IN CHAR_T *method,
                    IN CLOUD_UPLOAD_PATH_E path,
                    IN CHAR_T *cFileLocalname,
                    CHAR_T *cFileUploadname,
                    IN CHAR_T* content_type,
                    OUT CHAR_T* logSuf);

#endif

