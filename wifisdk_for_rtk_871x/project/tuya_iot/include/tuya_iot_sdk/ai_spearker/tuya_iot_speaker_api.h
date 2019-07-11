/*!
\file tuya_iot_speaker_api.h
Copyright(C),2017, 涂鸦科技 www.tuya.comm
*/

#ifndef _TUYA_IOT_SPEAKER_API_H
#define _TUYA_IOT_SPEAKER_API_H


#include "tuya_cloud_types.h"
#include "tuya_cloud_com_defs.h"
#include "uni_log.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TY_MEDIA_SESSION_ID_MAX_LEN  64

typedef enum {
    TY_TYPE_MEDIA = 0,
    TY_TYPE_TTS = 1,
    TY_TYPE_INVALD
} TY_MEDIA_TYPE_E;

typedef enum {
    TY_SPEECH = 0,
    TY_WECHAT = 1,
    TY_TARGET_INVALD
}TY_MEDIA_TARGET_T;

typedef enum {
    TY_WAV = 0,
    TY_MP3 = 1,
    TY_SPEEX = 2,
    TY_AAC = 3,
    TY_AMR = 4,
    TY_M4A = 5,
    TY_PCM = 6,             //for speaker stream data play
    TY_INVALD
}TY_MEDIA_ENCODE_T;

typedef enum {
    TY_HTTP_GET,
    TY_HTTP_POST,
    TY_HTTP_PUT,
} TY_HTTP_METHOD_T;

typedef enum {
    TY_TASK_NORMAL = 0,     //music story ...
    TY_TASK_CLOCK = 1,
    TY_TASK_ALERT = 2,
    TY_TASK_INVALD
    
}TY_MEDIA_TASK_TYPE_T;

typedef enum {
    TY_THING_CONFIG_START = 0,     
    TY_THING_CONFIG_STOP = 1,
    TY_THING_CONFIG_INVALD
    
}TY_THING_CONFIG_MODE_T;

typedef enum {
    TY_NICK_NAME_SET = 0,     
    TY_NICK_NAME_DEL = 1,
    TY_NICK_NAME_INVALD
    
}TY_NICK_NAME_MODE_T;

/* 本地mic拾音和上传API */
typedef PVOID_T MEDIA_UPLOAD_CTX;

OPERATE_RET tuya_speaker_upload_media_start(OUT MEDIA_UPLOAD_CTX *p_ctx, IN CONST TY_MEDIA_ENCODE_T voice_encode, IN CONST TY_MEDIA_TARGET_T target, IN CONST CHAR_T *p_session_id);
OPERATE_RET tuya_speaker_upload_media_send(IN MEDIA_UPLOAD_CTX ctx, IN CONST BYTE_T *p_buf, IN CONST UINT_T buf_len);
OPERATE_RET tuya_speaker_upload_media_stop(IN MEDIA_UPLOAD_CTX ctx, BOOL_T force_stop);

/* 云端/本地播放API */
typedef PVOID_T TY_PLAY_CTX;
/* 播放 云端/本地 音视频事件回调 */
typedef enum
{
    MEDIA_INIT = 0,
    MEDIA_START = 1,
    MEDIA_PAUSE = 2,
    MEDIA_RESUME = 3,
    MEDIA_READ_FINISH = 4,
    MEDIA_PLAY_FINISH = 5,
    MEDIA_INTERRUPT_FINISH = 6,
    MEDIA_ERR = 7,
}MEDIA_PLAY_EVENT_E;
typedef VOID (*TY_MEDIA_PLAY_EVENT_CB)(IN TY_PLAY_CTX ctx, IN CONST MEDIA_PLAY_EVENT_E event, IN PVOID_T private_data);

/* 通用播放信息 */
typedef struct
{
    TY_MEDIA_ENCODE_T   decode_type;
    UINT_T              length;   //单位 字节
    UINT_T              duration; //单位 秒
    TY_MEDIA_TYPE_E     type;
}TY_COMM_MEDIA_S;

/* 云端播放信息 */
typedef enum {
    TTS_KEEP_SESSION,
    TTS_NO_KEEP_SESSION,
}TY_CLOUD_FOLLOW_ACTION_E;

typedef struct
{
    UINT_T              id;
    TY_COMM_MEDIA_S     comm;
    CHAR_T              *p_url;
    TY_CLOUD_FOLLOW_ACTION_E followAction;
    CHAR_T              session_id[TY_MEDIA_SESSION_ID_MAX_LEN + 1];
    TY_HTTP_METHOD_T    http_method;
    CHAR_T              *p_req_body;
    TY_MEDIA_TASK_TYPE_T task_type;
    UINT_T              callback_val;
}TY_CLOUD_MEDIA_S;
/* 云端音视频资源获取回调 */
typedef VOID (*TY_SPEAKER_REV_CLOUD_MEDIA_CB)(IN TY_CLOUD_MEDIA_S **pp_media_arr, IN CONST UINT_T arr_size);
typedef VOID (*TY_SPEAKER_SYNC_AUDIO_CB)(VOID);
typedef VOID (*TY_SPEAKER_THING_CONFIG_CB)(IN TY_THING_CONFIG_MODE_T mode, IN CONST CHAR_T *token, IN CONST UINT_T timeout);
typedef VOID (*TY_SPEAKER_NICK_NAME_CB)(TY_NICK_NAME_MODE_T mode, IN CONST CHAR_T *nickname, IN CONST CHAR_T *pinyin);

/* 释放回调提供的数组资源 */
VOID tuya_speaker_free_cloud_media_arr(IN TY_CLOUD_MEDIA_S **pp_media_arr, IN CONST UINT_T arr_size);
VOID tuya_speaker_free_cloud_media(IN TY_CLOUD_MEDIA_S *p_media);
/* 快速打印函数 */
VOID tuya_speaker_print_cloud_media_arr(IN CONST TY_CLOUD_MEDIA_S **pp_media_arr, IN CONST UINT_T arr_size);


/* 本地音视频播放信息 */
typedef struct
{
    TY_COMM_MEDIA_S     comm;    
    CHAR_T              *p_path; 
}TY_LOCAL_MEDIA_S;
/* 播放本地音视频时请求本地音乐数据回调，返回为实际写入的数据量 */
typedef INT_T (*TY_SPEAKER_GET_LOCAL_MEDIA_CB)(IN CONST TY_PLAY_CTX ctx, IN CONST UINT_T offset, \
                                                 INOUT BYTE_T *p_buf, IN CONST UINT_T buf_len, IN PVOID_T private_data);

/* 请求播放云端音视频 */
OPERATE_RET tuya_speaker_req_play_cloud_media(IN CONST TY_CLOUD_MEDIA_S *p_desc, OUT TY_PLAY_CTX *p_ctx, \
                                              IN CONST TY_MEDIA_PLAY_EVENT_CB event_cb, \
                                              IN CONST PVOID_T private_data, CHAR_T debug_name[5]);
/* 请求播放本地音视频 */
OPERATE_RET tuya_speaker_req_play_local_media(IN CONST TY_LOCAL_MEDIA_S *p_desc, OUT TY_PLAY_CTX *p_ctx, \
                                              TY_SPEAKER_GET_LOCAL_MEDIA_CB get_cb, TY_MEDIA_PLAY_EVENT_CB event_cb,\
                                              PVOID_T private_data, CHAR_T debug_name[5]);
/* 开始播放 */
OPERATE_RET tuya_speaker_media_start(IN CONST TY_PLAY_CTX ctx);
/* 暂停播放 */
OPERATE_RET tuya_speaker_media_pause(IN CONST TY_PLAY_CTX ctx, BOOL_T hal_force_stop);
/* 继续播放 */
OPERATE_RET tuya_speaker_media_resume(IN CONST TY_PLAY_CTX ctx);
/* 停止播放 */
OPERATE_RET tuya_speaker_media_stop(IN CONST TY_PLAY_CTX ctx, BOOL_T hal_force_stop);
/* seek播放 */
OPERATE_RET tuya_speaker_media_seek(IN CONST TY_PLAY_CTX ctx, IN CONST UINT_T offset, BOOL_T hal_force_stop);
/* 获取播放的进度 */
OPERATE_RET tuya_speaker_media_get_offset(IN CONST TY_PLAY_CTX ctx, INOUT UINT_T *p_offset, INOUT UINT_T *p_totallen);


typedef OPERATE_RET (*TY_SPEAKER_HAL_WRITE_CB)(PVOID_T hal_ctx, BYTE_T *p_buf, UINT_T buf_len, UINT_T *p_remain, BOOL_T finish, \
                                               TY_COMM_MEDIA_S *p_com_type, IN CONST TY_PLAY_CTX ctx, PVOID_T private_data);

typedef enum {
    STATE_IDLE,
    STATE_PLAY,
}TY_HAL_STATE_E;

typedef enum {
    CMD_HAL_FINISH, /* sdk判断当前没有播放任务，建议HAL可以释放部分资源 */
    CMD_HAL_START, /* sdk判断有播放任务执行(start/resume)，有可能会多次通知 */

    CMD_FORCE_STOP, /* sdk需要HAL层立刻停止当前播放(hal_force_stop=TRUE) */
    CMD_STATUS_QUERY, /* sdk查询HAL层状态, 状态返回定义 *p_hal_out = TY_HAL_STATE_E */
    CMD_PLAY_OFFSET_QUERY, /* sdk从HAL层查询总体播放进度，结果放到*p_hal_out */
    
}TY_HAL_CMD_E;
typedef OPERATE_RET (*TY_SPEAKER_HAL_CMD_CB)(PVOID_T hal_ctx, IN CONST TY_PLAY_CTX ctx, PVOID_T private_data, TY_HAL_CMD_E cmd, INT_T *p_hal_out);


/*
VOID iot_register_extra_mqt_cb(VOID)
{
    iot_speaker_mqtt_register( your_custom_cb );
}
*/
typedef struct {
    TY_SPEAKER_REV_CLOUD_MEDIA_CB rev_media_cb;
    TY_SPEAKER_SYNC_AUDIO_CB sync_audio_cb;
    TY_SPEAKER_THING_CONFIG_CB thing_config_cb;
    TY_SPEAKER_NICK_NAME_CB nick_name_cb;
    
}TY_SPEAKER_MQTT_CBS_S;

OPERATE_RET tuya_speaker_mqtt_register(TY_SPEAKER_MQTT_CBS_S *p_mqtt_cbs);

OPERATE_RET tuya_speaker_mqtt_sync_audio_progress(IN CONST TY_PLAY_CTX ctx);
OPERATE_RET tuya_speaker_mqtt_goto_new_audio(IN CONST TY_PLAY_CTX ctx, BOOL_T is_next);
OPERATE_RET tuya_speaker_mqtt_goto_current();
OPERATE_RET tuya_speaker_mqtt_collect_audio(IN CONST TY_PLAY_CTX ctx);
OPERATE_RET tuya_speaker_mqtt_request_bell(IN CONST CHAR_T *p_bell_data);
OPERATE_RET tuya_speaker_mqtt_report_nick_name(IN CONST TY_NICK_NAME_MODE_T mode, 
                                                          IN CONST CHAR_T *nickname, 
                                                          IN CONST CHAR_T *pinyin, 
                                                          IN CONST BOOL_T set_result);
OPERATE_RET tuya_speaker_mqtt_report_thing_config_stop();
OPERATE_RET tuya_speaker_mqtt_report_complete_tts(IN CONST UINT_T callback_val);

OPERATE_RET tuya_speaker_init(PVOID_T hal_ctx,TY_SPEAKER_HAL_WRITE_CB write_cb, TY_SPEAKER_HAL_CMD_CB hal_cmd_cb, \
                              UINT_T max_buffer_size, UINT_T max_media_cnt, UINT_T each_read_len);

#ifdef __cplusplus
}
#endif

#endif  /*_TUYA_IOT_SPEAKER_API_H*/

