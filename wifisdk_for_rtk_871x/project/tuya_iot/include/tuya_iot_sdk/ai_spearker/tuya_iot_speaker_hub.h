/*!
\file tuya_iot_speaker_internal.h
Copyright(C),2017, 涂鸦科技 www.tuya.comm
*/

#ifndef _TUYA_IOT_SPEAKER_INTERNAL_H
#define _TUYA_IOT_SPEAKER_INTERNAL_H


#include "tuya_cloud_types.h"
#include "tuya_iot_speaker_api.h"
#include "uni_thread.h"
#include "uni_queue.h"
#include "tuya_iot_internal_api.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef PACKED
#define PACKED __attribute__((packed))
#endif

typedef enum {
    STAT_INIT = 0,
    STAT_START = 1,
    STAT_PLAYING = 2,
    STAT_PAUSE = 3,
    STAT_PAUSE_FINISH = 4,
    STAT_RESUME = 5,
    STAT_STOP = 6,
    STAT_FINISH = 7,
} TY_PLAY_STAT_E;

typedef enum {
    TY_MEDIA_SRC_LOCAL = 0,
    TY_MEDIA_SRC_CLOUD = 1
} TY_MEDIA_SRC_E;

typedef struct {
    TY_CLOUD_MEDIA_S desc;
    IOT_RAW_HTTP_S raw_http;
} TY_CLOUD_MEDIA_INFO_S;

typedef struct {
    TY_LOCAL_MEDIA_S desc;
    TY_SPEAKER_GET_LOCAL_MEDIA_CB get_cb;
} TY_LOCAL_MEDIA_INFO_S;

typedef union {
    TY_CLOUD_MEDIA_INFO_S cloud_media;
    TY_LOCAL_MEDIA_INFO_S local_media;
} TY_MEDIA_SRC_INFO_U;

typedef struct {
    TY_MEDIA_SRC_E      src_type;
    TY_MEDIA_SRC_INFO_U src_info;
} TY_MEDIA_INFO_S;


#define WRITE_EXIT_BUF_ACK 0x01
#define READ_EXIT_BUF_ACK 0x02

typedef enum {
    BUF_WRITE_INIT = 100,
    BUF_WRITE_START = 101,
    BUF_WRITE_PLAYING = 102,
    BUF_WRITE_PAUSE = 103,
    BUF_WRITE_PAUSE_FINISH = 104,
    BUF_WRITE_RESUME = 105,
    BUF_WRITE_STOP = 106,
} TY_BUF_WRITE_STAT_E;

struct struct_play_ctl;
typedef struct
{
    THRD_HANDLE work_thread;
    TY_BUF_WRITE_STAT_E write_state;

    PVOID_T hal_ctx;
    TY_SPEAKER_HAL_WRITE_CB write_cb;
    TY_SPEAKER_HAL_CMD_CB hal_cmd_cb;
    struct struct_play_ctl *p_play_ctl;

}SPEAKER_BUF_CTX_S;

typedef struct struct_play_ctl
{
    P_QUEUE_CLASS buf_queue;
    TY_MEDIA_INFO_S media_info;
    THRD_HANDLE play_thread;
    UINT_T curr_offset;

    SPEAKER_BUF_CTX_S write_buf_ctx;

    TY_MEDIA_PLAY_EVENT_CB event_cb;

    PVOID_T private_data;
    TY_COMM_MEDIA_S com_type;
    CHAR_T debug_name[5];

    MUTEX_HANDLE mutex;
    UINT_T thread_status;
    BOOL_T enabled;
    TY_PLAY_STAT_E curr_state;


}TY_PLAY_CTL_S;


UINT_T iot_speaker_get_enable_cnt(VOID);

OPERATE_RET iot_speaker_buf_init(OUT SPEAKER_BUF_CTX_S *p_buf_ctx, IN CONST UINT_T max_buf_size,\
                                   PVOID_T hal_ctx,TY_SPEAKER_HAL_WRITE_CB write_cb, TY_SPEAKER_HAL_CMD_CB hal_cmd_cb,\
                                   IN TY_PLAY_CTL_S *p_play_ctl);
INT_T iot_speaker_buf_get_avail_len(IN TY_PLAY_CTL_S *p_play_ctl);
OPERATE_RET iot_speaker_buf_write(IN TY_PLAY_CTL_S *p_play_ctl, IN CONST BYTE_T *p_buf, IN CONST UINT_T buf_size);
OPERATE_RET iot_speaker_buf_hal_cmd(IN SPEAKER_BUF_CTX_S *p_buf_ctx, TY_HAL_CMD_E cmd, INT_T *p_hal_out);
OPERATE_RET iot_speaker_buf_set_state(IN SPEAKER_BUF_CTX_S *p_buf_ctx,IN CONST TY_BUF_WRITE_STAT_E new_state);



#ifdef __cplusplus
}
#endif

#endif  /*_TUYA_IOT_SPEAKER_INTERNAL_H*/

