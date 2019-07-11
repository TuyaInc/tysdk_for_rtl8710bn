/***********************************************************
*  File: log_seq.h
*  Author: nzy
*  Date: 20190422
***********************************************************/
#ifndef __LOG_SEQ_H
    #define __LOG_SEQ_H

    #include "tuya_cloud_types.h"
    #include "uni_mutex.h"

#ifdef __cplusplus
	extern "C" {
#endif

#ifdef  _LOG_SEQ_GLOBALS
    #define __LOG_SEQ_EXT
#else
    #define __LOG_SEQ_EXT extern
#endif

/***********************************************************
*************************micro define***********************
***********************************************************/
typedef BYTE_T LS_DATA_TP_T; // log sequence data type
#define LDT_NULL 0
#define LDT_DEC 1
#define LDT_HEX 2
#define LDT_TIMESTAMP 3

typedef union {
    INT_T dec_data;
    UINT_T hex_data;
    TIME_T tm_data;
}LOG_DA_TP_U;

typedef struct { // log sequence data structure
    BYTE_T seq_num; // log sequence num
    LS_DATA_TP_T ldt;
    LOG_DA_TP_U data;
}LS_DATA_S;

typedef BYTE_T LS_STAT_T;
#define LSS_INIT 0
#define LSS_RECORDING 1
#define LSS_LOCKED 2 // L0G_SEQ_S set locked status when record the error log sequence
    
// Calling when record the error log sequence
struct s_log_seq;
typedef VOID (*LOG_SEQ_ERROR_PROC_CB)(IN struct s_log_seq *ls,IN CONST UINT_T timestamp,IN CONST BYTE_T err_repeat);

typedef struct {
    BYTE_T err_repeat; // the same error count
    LS_DATA_S last_data; // if(last_data == (current error data record)) then err_repeat++
}LOG_SEQ_ERR_MAG_S;
    
#define LOG_SEQ_NAME_LEN 15
#define LOG_DEF_SIZE 10
    
// log_name + status array
typedef struct s_log_seq {
    CHAR_T name[LOG_SEQ_NAME_LEN +1];
    TIME_T timestamp; // error log sequence timestamp
    MUTEX_HANDLE mutex;
    LOG_SEQ_ERROR_PROC_CB lsep_cb;
    LOG_SEQ_ERR_MAG_S lse_mag;
    LS_STAT_T ls_status;
    BYTE_T db_index; // data buf index.
                     // if(ls_status != LSS_LOCKED) then db_index is the next index of log sequence data buf
                     // if(ls_status == LSS_LOCKED) then db_index is the error log sequence
    BYTE_T db_count; // log sequence data buf count
    LS_DATA_S data_buf[LOG_DEF_SIZE]; // log sequence data buf(circle buf)
}L0G_SEQ_S;

// log sequence record format of string
// ec is error repeat count
// "name(15bytes)" " | " "mm-dd hh:mm:ss" " | " "erc:%d" " | " "[""%d" " | " "%3d:0x%x" " | " "%3d:%d""]"  
// for example:"eazymode | 02-01 10:58:20 | ec:20 | [1 | 2:0x12345678 | 3:1234 | 4:12-19 12:38:59]"
#define LS_DELIMITER " | "
#define LSR_STR_MAX LOG_SEQ_NAME_LEN + sizeof(LS_DELIMITER) + \
                    sizeof("mm-dd hh:mm:ss") + sizeof(LS_DELIMITER) + \
                    sizeof("ec:xxx") + sizeof(LS_DELIMITER) + \
                    sizeof("[]") + LOG_DEF_SIZE*(sizeof("xxx:yyyyyyyyyy")) + 20

#define RESET_LOG_SEQ(ls) reset_log_seq((ls))

#define INSERT_LOG_SEQ_NULL(ls,seq) { \
    LS_DATA_S ls_data = {0}; \
    ls_data.seq_num = seq; \
    ls_data.ldt = LDT_NULL; \
    insert_log_seq(ls,&ls_data); \
}

#define INSERT_LOG_SEQ_DEC(ls,seq,dec) { \
    LS_DATA_S ls_data = {0}; \
    ls_data.seq_num = seq; \
    ls_data.ldt = LDT_DEC; \
    ls_data.data.dec_data = dec; \
    insert_log_seq(ls,&ls_data); \
}

#define INSERT_LOG_SEQ_HEX(ls,seq,hex) { \
    LS_DATA_S ls_data = {0}; \
    ls_data.seq_num = seq; \
    ls_data.ldt = LDT_HEX; \
    ls_data.data.hex_data = hex; \
    insert_log_seq(ls,&ls_data); \
}

#define INSERT_LOG_SEQ_TM(ls,seq,tm) { \
    LS_DATA_S ls_data = {0}; \
    ls_data.seq_num = seq; \
    ls_data.ldt = LDT_TIMESTAMP; \
    ls_data.data.tm_data = tm; \
    insert_log_seq(ls,&ls_data); \
}

#define INSERT_ERROR_LOG_SEQ_NULL(ls,seq) { \
    LS_DATA_S ls_data = {0}; \
    ls_data.seq_num = seq; \
    ls_data.ldt = LDT_NULL; \
    insert_error_log_seq(ls,&ls_data); \
}

#define INSERT_ERROR_LOG_SEQ_DEC(ls,seq,dec) { \
    LS_DATA_S ls_data = {0}; \
    ls_data.seq_num = seq; \
    ls_data.ldt = LDT_DEC; \
    ls_data.data.dec_data = dec; \
    insert_error_log_seq(ls,&ls_data); \
}

#define INSERT_ERROR_LOG_SEQ_HEX(ls,seq,hex) { \
    LS_DATA_S ls_data = {0}; \
    ls_data.seq_num = seq; \
    ls_data.ldt = LDT_HEX; \
    ls_data.data.hex_data = hex; \
    insert_error_log_seq(ls,&ls_data); \
}

#define INSERT_ERROR_LOG_SEQ_TM(ls,seq,tm) { \
    LS_DATA_S ls_data = {0}; \
    ls_data.seq_num = seq; \
    ls_data.ldt = LDT_TIMESTAMP; \
    ls_data.data.tm_data = tm; \
    insert_error_log_seq(ls,&ls_data); \
}

/***********************************************************
*************************variable define********************
***********************************************************/

/***********************************************************
*************************function define********************
***********************************************************/
/***********************************************************
*  Function: create_log_seq
*  Desc:     create and init a "log sequence" 
*  Input:    ls_name log sequence name
*  Input:    lsep_cb 
*  Output:   pMutexHandle: the created mutex context
*  Return:   OPRT_OK: success  Other: fail
***********************************************************/
__LOG_SEQ_EXT \
OPERATE_RET create_log_seq(IN CONST CHAR_T *ls_name,IN CONST LOG_SEQ_ERROR_PROC_CB lsep_cb,\
                           OUT L0G_SEQ_S **log_seq);

/***********************************************************
*  Function: reset_log_seq
*  Desc:     reset "log sequence" 
*  Input:    log_seq
*  Output:   none
*  Return:   OPRT_OK: success  Other: fail
***********************************************************/
__LOG_SEQ_EXT \
OPERATE_RET reset_log_seq(INOUT L0G_SEQ_S *log_seq);

/***********************************************************
*  Function: insert_log_seq
*  Desc:     insert log sequence 
*  Input:    log_seq ls_data
*  Output:   none
*  Return:   OPRT_OK: success  Other: fail
***********************************************************/
__LOG_SEQ_EXT \
OPERATE_RET insert_log_seq(INOUT L0G_SEQ_S *log_seq,IN CONST LS_DATA_S *ls_data);

/***********************************************************
*  Function: insert_error_log_seq
*  Desc:     insert error log sequence 
*  Input:    log_seq ls_data
*  Output:   none
*  Return:   OPRT_OK: success  Other: fail
***********************************************************/
__LOG_SEQ_EXT \
OPERATE_RET insert_error_log_seq(INOUT L0G_SEQ_S *log_seq,IN CONST LS_DATA_S *ls_data);

/***********************************************************
*  Function: make_log_seq_string
*  Desc:     make log seq string 
*  Input:    log_seq
*  Output:   ls_str
*  Return:   OPRT_OK: success  Other: fail
***********************************************************/
__LOG_SEQ_EXT \
OPERATE_RET make_log_seq_string(INOUT L0G_SEQ_S *log_seq,OUT CHAR_T **ls_str);

#ifdef __cplusplus
}
#endif
#endif

