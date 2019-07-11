/***********************************************************
*  File: mf_test.h
*  Author: nzy
*  Date: 20171106
***********************************************************/
#ifndef __MF_TEST_H
    #define __MF_TEST_H

    #include "tuya_cloud_types.h"
    #include "tuya_cloud_com_defs.h"

#ifdef __cplusplus
	extern "C" {
#endif

#ifdef  __MF_TEST_GLOBALS
    #define __MF_TEST_MODULE_EXT
#else
    #define __MF_TEST_MODULE_EXT extern
#endif

/***********************************************************
*************************micro define***********************
***********************************************************/
// in parm:baud
// data bits:8bit parity:none stop bits:1
typedef VOID (*MF_UART_INIT_CB)(UINT_T baud,UINT_T bufsz);

// in parm:send data buf
// in parm:send data buf len
typedef VOID (*MF_UART_SEND_CB)(IN CONST BYTE_T *data,IN CONST UINT_T len);

// in parm:len->data buf len
// out parm:buf->read data buf 
// return:read data len
typedef UINT_T (*MF_UART_RECV_CB)(OUT BYTE_T *buf,IN CONST UINT_T len);

// in gpio test
typedef BOOL_T (*MF_GPIO_TEST_CB)(VOID);

// uart free
typedef VOID (*MF_UART_FREE_CB)(VOID);

// callback for user
typedef VOID (*MF_USER_CALLBACK)(VOID);


typedef struct {
    MF_UART_INIT_CB uart_init;
    MF_UART_FREE_CB uart_free;
    MF_UART_SEND_CB uart_send;
    MF_UART_RECV_CB uart_recv;
    MF_GPIO_TEST_CB gpio_test;
    MF_USER_CALLBACK user_callback;
}MF_IMPORT_INTF_S;

typedef VOID (*MF_GW_UG_INFORM_CB)(UINT_T file_size, UINT_T file_crc);
typedef OPERATE_RET (*MF_UPGRADE_NOTIFY_CB)(VOID);

/***********************************************************
*************************variable define********************
***********************************************************/

/***********************************************************
*************************function define********************
***********************************************************/
/***********************************************************
*  Function: mf_init
*  Input: intf
*  Output: none 
*  Return: OPERATE_RET
***********************************************************/
__MF_TEST_MODULE_EXT \
OPERATE_RET mf_init(IN CONST MF_IMPORT_INTF_S *intf,\
                       IN CONST CHAR_T *file_name,IN CONST CHAR_T *file_ver,IN BOOL_T wrMacEn);


/***********************************************************
*  Function: mf_reg_gw_ug_cb
*  Input: mf_upgrd_inform mf_upgrd_get_file mf_upgrd_nofity
*  Output: none 
*  Return: OPERATE_RET
***********************************************************/
__MF_TEST_MODULE_EXT \
VOID mf_reg_gw_ug_cb(IN CONST MF_GW_UG_INFORM_CB mf_upgrd_inform,\
                       IN CONST GET_FILE_DATA_CB mf_upgrd_get_file,IN CONST MF_UPGRADE_NOTIFY_CB mf_upgrd_nofity);


#ifdef __cplusplus
}
#endif
#endif

