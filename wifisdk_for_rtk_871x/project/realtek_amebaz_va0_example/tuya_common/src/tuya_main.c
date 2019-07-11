/***********************************************************
*  File: tuya_main.c
*  Author: nzy
*  Date: 20171012
***********************************************************/
#include "adapter_platform.h"
#include "tuya_iot_wifi_api.h"
#include "flash_api.h"
#include <device_lock.h>
#include "rtl8710b_ota.h"
#include "mf_test.h"
#include "tuya_uart.h"
#include "tuya_gpio.h"
#include "gw_intf.h"
#include "wf_basic_intf.h"



/***********************************************************
*************************micro define***********************
***********************************************************/
#define TEST_SSID "tuya_mdev_test1"
#define WF_SSID_LEN 32
/***********************************************************
*************************variable define********************
***********************************************************/

typedef enum {
    UGS_RECV_HEADER = 0,
    UGS_RECV_OTA_HD,
    UGS_SEARCH_SIGN,
    UGS_RECV_SIGN,
    UGS_RECV_IMG_DATA,
    // UGS_RECV_RDP_DATA,
    UGS_FINISH
}UG_STAT_E;

typedef struct {
    UG_STAT_E stat;
    update_ota_target_hdr hdr;
    u32 ota_index;

    u32 image_cnt;
    u8 signature[8];
    update_dw_info DownloadInfo[2];
    u32 cur_image_cnt[2];

    flash_t flash;
    u32 new_addr;
    u32 recv_data_cnt;
}UG_PROC_S;

STATIC UG_PROC_S *ug_proc = NULL;
extern VOID app_init(VOID);
typedef VOID (*APP_PROD_CB)(BOOL_T flag, CHAR_T rssi);//lql
STATIC APP_PROD_CB app_prod_test = NULL;//lql
STATIC GW_WF_CFG_MTHD_SEL gwcm_mode = GWCM_OLD;//lql
STATIC CHAR_T prod_ssid_name[WF_SSID_LEN + 1] = TEST_SSID;


/***********************************************************
*************************function define********************
***********************************************************/
extern VOID pre_device_init(VOID);
extern OPERATE_RET device_init(VOID);
extern BOOL_T gpio_test(VOID);
extern VOID mf_user_callback(VOID);
extern VOID tuya_cnt_rst_judge(VOID);

extern void sys_jtag_off(void);
extern TY_GPIO_PORT_E swith_ctl_port;
STATIC VOID __gw_ug_inform_cb(INOUT BOOL_T *handled, IN CONST FW_UG_S *fw);
STATIC OPERATE_RET __gw_upgrage_process_cb(IN CONST FW_UG_S *fw, IN CONST UINT total_len,IN CONST UINT offset,\
                                                      IN CONST BYTE_T *data,IN CONST UINT len,OUT UINT *remain_len, IN PVOID pri_data);
STATIC VOID __gw_upgrade_notify_cb(IN CONST FW_UG_S *fw, IN CONST INT_T download_result, IN PVOID pri_data);

STATIC OPERATE_RET __mf_gw_upgrade_notify_cb(VOID);
STATIC VOID __mf_gw_ug_inform_cb(UINT_T file_size, UINT_T file_crc);

STATIC VOID __tuya_mf_uart_init(UINT_T baud,UINT_T bufsz)
{
    ty_uart_init(TY_UART0,baud,TYWL_8B,TYP_NONE,TYS_STOPBIT1,bufsz,TRUE);
}
STATIC VOID __tuya_mf_uart_free(VOID)
{
    ty_uart_free(TY_UART0);
}

STATIC VOID __tuya_mf_send(IN CONST BYTE_T *data,IN CONST UINT len)
{
    ty_uart_send_data(TY_UART0,data,len);
}

STATIC UINT __tuya_mf_recv(OUT BYTE_T *buf,IN CONST UINT len)
{
    return ty_uart_read_data(TY_UART0,buf,len);
}
STATIC BOOL_T scan_test_ssid(VOID)
{
    //if(FALSE == get_new_prod_mode()) {
        //return false;
    //}

    OPERATE_RET op_ret = OPRT_OK;
    //special for GWCM_OLD_PROD.......only do prodtesting when in smartlink or ap mode
	//if(gwcm_mode == GWCM_OLD_PROD ) {
        op_ret = wd_gw_wsm_read(&(get_gw_cntl()->gw_wsm));
        if(get_gw_cntl()->gw_wsm.nc_tp >= GWNS_TY_SMARTCFG) {
            return false;
        }
	//}

    wf_wk_mode_set(WWM_STATION);
    AP_IF_S *ap = NULL;
    BOOL_T flag = TRUE;
    PR_NOTICE("current product ssid name:%s", prod_ssid_name);
	op_ret = wf_assign_ap_scan(prod_ssid_name, &ap);//lql
    wf_station_disconnect();
    if(OPRT_OK != op_ret) {
        PR_NOTICE("wf_assign_ap_scan failed(%d)",op_ret);
        return FALSE;
    }
    //check if has authorized
    op_ret = wd_gw_base_if_read(&(get_gw_cntl()->gw_base));
    if(OPRT_OK != op_ret) {
        PR_DEBUG("read flash err");
        flag = FALSE;
    }
    // gateway base info verify
    #if TY_SECURITY_CHIP
    if(!get_gw_cntl()->gw_base.has_auth) {
    #else
    if(0 == get_gw_cntl()->gw_base.auth_key[0] || \
       0 == get_gw_cntl()->gw_base.uuid[0]) {
    #endif
        PR_DEBUG("please write uuid and auth_key first");
        flag = FALSE;
    }

    if(app_prod_test) {
        app_prod_test(flag, ap->rssi);
    }
    return TRUE;
}

void app_cfg_set(IN CONST GW_WF_CFG_MTHD_SEL mthd, APP_PROD_CB callback)
{
    app_prod_test = callback;
    gwcm_mode = mthd;
}

#if 0
#include "aes_inf.h"

STATIC CHAR_T s_ecb_enc_key_arr[16] = {0};
STATIC CHAR_T s_ecb_dec_key_arr[16] = {0};
STATIC CHAR_T s_cbc_enc_key_arr[16] = {0};
STATIC CHAR_T s_cbc_dec_key_arr[16] = {0};
STATIC INT_T s_crypto_mode = -1; //ecb_enc:0 ecb_dec:1 cbc_enc:2 cbc_dec:3

STATIC OPERATE_RET AES128_ECB_ENC_BUF_realtek(IN BYTE_T *input, IN CONST BYTE_T *key, OUT BYTE_T *output)
{
    UINT_T input_len = 16;
    UINT_T key_len = 16;
    int ret = 0;

    #ifdef __ICCARM__
    #pragma data_alignment = 4
    #elif defined (__GNUC__)
    __attribute__ ((aligned (4)))
    #endif
    STATIC BYTE_T tmp_key[16] = {0};//入参必须4字节对齐
    if( (s_crypto_mode != 0) || (memcmp(s_ecb_enc_key_arr, key, key_len) != 0) ) {
        memcpy(tmp_key, key, key_len);
        ret = rtl_crypto_aes_ecb_init(tmp_key, key_len);
        if ( ret != 0 ) {
            PR_ERR("AES rtl_crypto_aes_ecb_init failed tmp_key:%s 0x%x key_len:%u ret:%d",
                   tmp_key, (UINT_T)tmp_key, key_len, ret);
            return ret;
        }
        s_crypto_mode = 0;
        memcpy(s_ecb_enc_key_arr, key, key_len);
    }

    ret = rtl_crypto_aes_ecb_encrypt(input, input_len, NULL, 0, output);
    if ( ret != 0 ) {
        PR_ERR("AES rtl_crypto_aes_ecb_encrypt failed input:0x%x inputlen:%u output:0x%x ret:%d",
               (UINT_T)input, input_len, (UINT_T)output, ret);
        return ret;
    }
    return OPRT_OK;
}

STATIC OPERATE_RET AES128_ECB_DEC_BUF_realtek(IN BYTE_T *input,IN CONST BYTE_T *key,OUT BYTE_T *output)
{
    UINT_T input_len = 16;
    UINT_T key_len = 16;
    int ret = 0;

    #ifdef __ICCARM__
    #pragma data_alignment = 4
    #elif defined (__GNUC__)
    __attribute__ ((aligned (4)))
    #endif
    STATIC BYTE_T tmp_key[16] = {0};//入参必须4字节对齐
    if( (s_crypto_mode != 1) || (memcmp(s_ecb_dec_key_arr, key, key_len) != 0) ) {
        memcpy(tmp_key, key, key_len);
        ret = rtl_crypto_aes_ecb_init(tmp_key, key_len);
        if ( ret != 0 ) {
            PR_ERR("AES rtl_crypto_aes_ecb_init failed tmp_key:%s 0x%x key_len:%u ret:%d",
                   tmp_key, (UINT_T)tmp_key, key_len, ret);
            return ret;
        }
        s_crypto_mode = 1;
        memcpy(s_ecb_dec_key_arr, key, key_len);
    }

    ret = rtl_crypto_aes_ecb_decrypt(input, input_len, NULL, 0, output);
    if ( ret != 0 ) {
        PR_ERR("AES rtl_crypto_aes_ecb_decrypt failed input:0x%x inputlen:%u output:0x%x ret:%d",
               (UINT_T)input, input_len, (UINT_T)output, ret);
        return ret;
    }
    return OPRT_OK;
}

STATIC OPERATE_RET AES128_CBC_ENC_BUF_realtek(OUT BYTE_T *output,IN BYTE_T *input,IN UINT_T length,
                                              IN CONST BYTE_T *key,IN CONST BYTE_T *iv)
{
    UINT_T key_len = 16;
    UINT_T iv_len = 16;
    int ret = 0;

    #ifdef __ICCARM__
    #pragma data_alignment = 4
    #elif defined (__GNUC__)
    __attribute__ ((aligned (4)))
    #endif
    STATIC BYTE_T tmp_key[16] = {0};//入参必须4字节对齐
    if( (s_crypto_mode != 2) || (memcmp(s_cbc_enc_key_arr, key, key_len) != 0) ) {
        memcpy(tmp_key, key, key_len);
        ret = rtl_crypto_aes_cbc_init(tmp_key, key_len);
        if ( ret != 0 )
        {
            PR_ERR("AES rtl_crypto_aes_cbc_init failed tmp_key:%s 0x%x key_len:%u ret:%d",
                   tmp_key, (UINT_T)tmp_key, key_len, ret);
            return ret;
        }
        s_crypto_mode = 2;
        memcpy(s_cbc_enc_key_arr, key, key_len);
    }

    #ifdef __ICCARM__
    #pragma data_alignment = 4
    #elif defined (__GNUC__)
    __attribute__ ((aligned (4)))
    #endif
    STATIC BYTE_T tmp_iv[16] = {0};//入参必须4字节对齐
    memcpy(tmp_iv, iv, iv_len);
    ret = rtl_crypto_aes_cbc_encrypt(input, length, tmp_iv, iv_len, output);
    if ( ret != 0 ) {
        PR_ERR("AES rtl_crypto_aes_cbc_encrypt failed input:0x%x output:0x%x iv:0x%x ivlen:%u length:%u ret:%d",
               (UINT_T)input, (UINT_T)output, (UINT_T)tmp_iv, iv_len, length, ret);
        return ret;
    }
    return OPRT_OK;
}

STATIC OPERATE_RET AES128_CBC_DEC_BUF_realtek(OUT BYTE_T *output,IN BYTE_T *input,IN UINT_T length,
                                              IN CONST BYTE_T *key,IN CONST BYTE_T *iv)
{
    UINT_T key_len = 16;
    UINT_T iv_len = 16;
    int ret = 0;

    #ifdef __ICCARM__
    #pragma data_alignment = 4
    #elif defined (__GNUC__)
    __attribute__ ((aligned (4)))
    #endif
    STATIC BYTE_T tmp_key[16] = {0};//入参必须4字节对齐
    if( (s_crypto_mode != 3) || (memcmp(s_cbc_dec_key_arr, key, key_len) != 0) ) {
        memcpy(tmp_key, key, key_len);
        ret = rtl_crypto_aes_cbc_init(tmp_key, key_len);
        if ( ret != 0 ) {
            PR_ERR("AES rtl_crypto_aes_cbc_init failed tmp_key:%s 0x%x key_len:%u ret:%d",
                   tmp_key, (UINT_T)tmp_key, key_len, ret);
            return ret;
        }
        s_crypto_mode = 3;
        memcpy(s_cbc_dec_key_arr, key, key_len);
    }

    #ifdef __ICCARM__
    #pragma data_alignment = 4
    #elif defined (__GNUC__)
    __attribute__ ((aligned (4)))
    #endif
    STATIC BYTE_T tmp_iv[16] = {0};//入参必须4字节对齐
    memcpy(tmp_iv, iv, iv_len);
    ret = rtl_crypto_aes_cbc_decrypt(input, length, tmp_iv, iv_len, output);
    if ( ret != 0 ) {
        PR_ERR("AES rtl_crypto_aes_cbc_decrypt failed input:0x%x output:0x%x iv:0x%x ivlen:%u length:%u ret:%d",
               (UINT_T)input, (UINT_T)output, (UINT_T)tmp_iv, iv_len, length, ret);
        return ret;
    }
    return OPRT_OK;
}


#endif

/***********************************************************
*  Function: user_main
*  Input: none
*  Output: none
*  Return: none
***********************************************************/
void user_main(void)
{
    OPERATE_RET op_ret = OPRT_OK;
    
    TY_INIT_PARAMS_S init_param = {0};
    init_param.init_db = FALSE;
    op_ret = tuya_iot_init_params(NULL, &init_param);
    if(OPRT_OK != op_ret) {
        PR_ERR("tuya_iot_init err:%d",op_ret);
        return;
    }
    sys_jtag_off();//lql

#if TY_SECURITY_CHIP
    SEChip_I2C_Init(); //加密芯片 I2C 接口初始化
#endif

#if 0
    AES_METHOD_REG_S aes_regs =
    {
        AES128_ECB_ENC_BUF_realtek,
        AES128_ECB_DEC_BUF_realtek,
        AES128_CBC_ENC_BUF_realtek,
        AES128_CBC_DEC_BUF_realtek
    };
    aes_method_register(&aes_regs);
#endif
    pre_device_init();
    tuya_cnt_rst_judge();
    tuya_iot_kv_flash_init(NULL);
    extern void wait_wifi_semaphore(void);
    wait_wifi_semaphore();
#if 1
    // to add prodect test code
    mf_reg_gw_ug_cb(__mf_gw_ug_inform_cb, __gw_upgrage_process_cb, __mf_gw_upgrade_notify_cb);
    MF_IMPORT_INTF_S intf = {
        __tuya_mf_uart_init,
        __tuya_mf_uart_free,
        __tuya_mf_send,
        __tuya_mf_recv,
        gpio_test,
        mf_user_callback
    };
    op_ret = mf_init(&intf,APP_BIN_NAME,USER_SW_VER,TRUE);
    if(OPRT_OK != op_ret) {
        PR_ERR("mf_init err:%d",op_ret);
        return;
    }
    ty_uart_free(TY_UART0);
    PR_NOTICE("mf_init succ");
#endif
    // register gw upgrade inform callback
    iot_register_pre_gw_ug_cb(__gw_ug_inform_cb);

// for debug
#if 0
    WF_GW_PROD_INFO_S wf_prod_info = {
        "tuyad6db61930456075e","eAr5qH46Dw9HcpISLk5GpD4z1q3nvrfp",NULL,NULL
    };
    op_ret = tuya_iot_set_wf_gw_prod_info(&wf_prod_info);
    if(OPRT_OK != op_ret) {
        PR_ERR("tuya_iot_set_wf_gw_prod_info err:%d",op_ret);
        return;
    }
#endif

#if 0
    DEBUG_GW_PROD_INFO_S debug_prod_info = {
        TRUE, NULL, NULL
    };
    op_ret = tuya_iot_set_wf_gw_debug_info(&debug_prod_info);
    if(OPRT_OK != op_ret) {
        PR_ERR("tuya_iot_set_wf_gw_prod_info err:%d",op_ret);
        return;
    }
#endif

    app_init();
    PR_DEBUG("gwcm_mode %d",gwcm_mode);
    if(gwcm_mode != GWCM_OLD) {
        PR_DEBUG("low_power select");
        if(true == scan_test_ssid()) {
            PR_DEBUG("prodtest");
            return;
        }
        PR_DEBUG("no tuya_mdev_test1!");
        op_ret = device_init();
        if(OPRT_OK != op_ret) {
            PR_ERR("device_init error:%d",op_ret);
            return;
        }
    }else {
        PR_DEBUG("device_init in");
        op_ret = device_init();
        if(OPRT_OK != op_ret) {
            PR_ERR("device_init err:%d",op_ret);
            return;
        }
    }
}
// mf gateway upgrade start
VOID __mf_gw_ug_inform_cb(UINT_T file_size, UINT_T file_crc)
{
    ug_proc = Malloc(SIZEOF(UG_PROC_S));
    if(NULL == ug_proc) {
        PR_ERR("malloc err");
        return;
    }
    memset(ug_proc,0,SIZEOF(UG_PROC_S));

    if (ota_get_cur_index() == OTA_INDEX_1) {
        ug_proc->ota_index = OTA_INDEX_2;
        PR_DEBUG("OTA2 address space will be upgraded\n");
    } else {
        ug_proc->ota_index = OTA_INDEX_1;
        PR_DEBUG("OTA1 address space will be upgraded\n");
    }
}

// gateway upgrade start
STATIC VOID __gw_ug_inform_cb(INOUT BOOL_T *handled, IN CONST FW_UG_S *fw)
{
    PR_DEBUG("Rev Pre_GW Upgrade Info");
    PR_DEBUG("fw->tp:%d", fw->tp);
    PR_DEBUG("fw->fw_url:%s", fw->fw_url);
    PR_DEBUG("fw->fw_hmac:%s", fw->fw_hmac);
    PR_DEBUG("fw->sw_ver:%s", fw->sw_ver);
    PR_DEBUG("fw->file_size:%d", fw->file_size);

    if(fw->tp != DEV_NM_ATH_SNGL)
    {
        *handled = FALSE;
        return;
    }
    *handled = TRUE;

    ug_proc = Malloc(SIZEOF(UG_PROC_S));
    if(NULL == ug_proc) {
        PR_ERR("malloc err");
        return;
    }
    memset(ug_proc,0,SIZEOF(UG_PROC_S));
    OPERATE_RET op_ret = OPRT_OK;
    op_ret = tuya_iot_upgrade_gw(fw,__gw_upgrage_process_cb,__gw_upgrade_notify_cb,NULL);
    if(OPRT_OK != op_ret) {
        PR_ERR("tuya_iot_upgrade_gw err:%d",op_ret);
        return;
    }

    memset(ug_proc,0,SIZEOF(UG_PROC_S));
    if (ota_get_cur_index() == OTA_INDEX_1) {
        ug_proc->ota_index = OTA_INDEX_2;
        PR_DEBUG("OTA2 address space will be upgraded\n");
    } else {
        ug_proc->ota_index = OTA_INDEX_1;
        PR_DEBUG("OTA1 address space will be upgraded\n");
    }
}
// mf gateway upgrade result notify
OPERATE_RET __mf_gw_upgrade_notify_cb(VOID)
{
    // verify
    u32 ret = 0;
    ret = verify_ota_checksum(ug_proc->new_addr,ug_proc->DownloadInfo[0].ImageLen, \
                              ug_proc->signature,&ug_proc->hdr);
    if(1 != ret) {
        PR_ERR("verify_ota_checksum err");

        device_mutex_lock(RT_DEV_LOCK_FLASH);
        flash_erase_sector(&ug_proc->flash, ug_proc->new_addr - SPI_FLASH_BASE);
        device_mutex_unlock(RT_DEV_LOCK_FLASH);
        return OPRT_COM_ERROR;
    }

    if(!change_ota_signature(ug_proc->new_addr, ug_proc->signature, ug_proc->ota_index)) {
        PR_ERR("change signature failed\n");
        return OPRT_COM_ERROR;
    }

    PR_NOTICE("the gateway upgrade success");
    return OPRT_OK;
}

// gateway upgrade result notify
STATIC VOID __gw_upgrade_notify_cb(IN CONST FW_UG_S *fw, IN CONST INT_T download_result, IN PVOID pri_data)
{
    if(OPRT_OK == download_result) { // update success
        // verify
        u32 ret = 0;
        ret = verify_ota_checksum(ug_proc->new_addr,ug_proc->DownloadInfo[0].ImageLen, \
                                  ug_proc->signature,&ug_proc->hdr);
        if(1 != ret) {
            PR_ERR("verify_ota_checksum err");
            #if 0
            device_mutex_lock(RT_DEV_LOCK_FLASH);
            flash_erase_sector(&ug_proc->flash, ug_proc->new_addr - SPI_FLASH_BASE);
            device_mutex_unlock(RT_DEV_LOCK_FLASH);
            #endif
            return;
        }

        if(!change_ota_signature(ug_proc->new_addr, ug_proc->signature, ug_proc->ota_index)) {
            PR_ERR("change signature failed\n");
            return;
        }

        PR_NOTICE("the gateway upgrade success");
        ota_platform_reset();
        return;
    }else {
        PR_ERR("the gateway upgrade failed");
    }
}

// gateway upgrade process
STATIC OPERATE_RET __gw_upgrage_process_cb(IN CONST FW_UG_S *fw, IN CONST UINT total_len,IN CONST UINT offset,\
                                                      IN CONST BYTE_T *data,IN CONST UINT len,OUT UINT *remain_len, IN PVOID pri_data)
{
    STATIC UINT ota_hd_len = 0;

    switch(ug_proc->stat) {
        case UGS_RECV_HEADER: {
            if(len < SIZEOF(update_file_hdr)) {
                *remain_len = len;
                break;
            }

            memcpy(&ug_proc->hdr.FileHdr,data,SIZEOF(update_file_hdr));
            ug_proc->stat = UGS_RECV_OTA_HD;
            ota_hd_len = (ug_proc->hdr.FileHdr.HdrNum) * SIZEOF(update_file_img_hdr);
            *remain_len = len;

            PR_DEBUG("ota_hd_len:%d",ota_hd_len);
        }
        //break;

        case UGS_RECV_OTA_HD: {
            if(len < ota_hd_len+SIZEOF(update_file_hdr)) {
                *remain_len = len;
                break;
            }

            char * pImgId = NULL;
            if(OTA_INDEX_1 == ug_proc->ota_index) {
                pImgId = "OTA1";
            }else {
                pImgId = "OTA2";
            }

            u32 ret = 0;
            ret = get_ota_tartget_header(data,(ota_hd_len+SIZEOF(update_file_hdr)), \
                                         &(ug_proc->hdr), pImgId);
            if(0 == ret) {
                PR_ERR("get_ota_tartget_header err");
                return OPRT_COM_ERROR;
            }

            // get new image addr and check new address validity
            if(!get_ota_address(ug_proc->ota_index, &ug_proc->new_addr, &(ug_proc->hdr))) {
                PR_ERR("get_ota_address err");
                return OPRT_COM_ERROR;
            }

            u32 new_img_len = ug_proc->hdr.FileImgHdr.ImgLen;

            PR_NOTICE("ug_proc->new_addr:0x%x",ug_proc->new_addr);
            PR_NOTICE("new_img_len:%d",new_img_len);
            
            #if OPERATING_SYSTEM == SYSTEM_REALTEK8710_1M
            if(new_img_len > (468*1024 -32)) {//32 Bytes resvd for header 468K
                PR_ERR("image length is too big");
                return OPRT_COM_ERROR;
            }
            #endif

            #if OPERATING_SYSTEM == SYSTEM_REALTEK8710_2M
            if(new_img_len > (788*1024 - 32)) {//32 Bytes resvd for header 960K
                PR_ERR("image length is too big");
                return OPRT_COM_ERROR;
            }
            #endif
            
            erase_ota_target_flash(ug_proc->new_addr, new_img_len);
            if(ug_proc->hdr.RdpStatus == ENABLE) {
                device_mutex_lock(RT_DEV_LOCK_FLASH);
                flash_erase_sector(&ug_proc->flash, RDP_FLASH_ADDR - SPI_FLASH_BASE);
                device_mutex_unlock(RT_DEV_LOCK_FLASH);
            }

            // 暂时仅支持image的升级
            if(ug_proc->hdr.RdpStatus == ENABLE) {
                ug_proc->image_cnt = 2;
                if(ug_proc->hdr.FileImgHdr.Offset < ug_proc->hdr.FileRdpHdr.Offset) {
                    ug_proc->DownloadInfo[0].ImgId = OTA_IMAG;
                    /* get OTA image and Write New Image to flash, skip the signature,
                    not write signature first for power down protection*/
                    ug_proc->DownloadInfo[0].FlashAddr = ug_proc->new_addr -SPI_FLASH_BASE + 8;
                    ug_proc->DownloadInfo[0].ImageLen = ug_proc->hdr.FileImgHdr.ImgLen - 8;/*skip the signature*/
                    ug_proc->DownloadInfo[0].ImgOffset = ug_proc->hdr.FileImgHdr.Offset;
                    #if 0
                    ug_proc->DownloadInfo[1].ImgId = RDP_IMAG;
                    ug_proc->DownloadInfo[1].FlashAddr = RDP_FLASH_ADDR - SPI_FLASH_BASE;
                    ug_proc->DownloadInfo[1].ImageLen = ug_proc->hdr.FileRdpHdr.ImgLen;
                    ug_proc->DownloadInfo[1].ImgOffset = ug_proc->hdr.FileRdpHdr.Offset;
                    #endif
                } else {
                    #if 0
                    ug_proc->DownloadInfo[0].ImgId = RDP_IMAG;
                    ug_proc->DownloadInfo[0].FlashAddr = RDP_FLASH_ADDR - SPI_FLASH_BASE;
                    ug_proc->DownloadInfo[0].ImageLen = ug_proc->hdr.FileRdpHdr.ImgLen;
                    ug_proc->DownloadInfo[0].ImgOffset = ug_proc->hdr.FileRdpHdr.Offset;
                    ug_proc->DownloadInfo[1].ImgId = OTA_IMAG;
                    /* get OTA image and Write New Image to flash, skip the signature,
                    not write signature first for power down protection*/
                    ug_proc->DownloadInfo[1].FlashAddr = ug_proc->new_addr -SPI_FLASH_BASE + 8;
                    ug_proc->DownloadInfo[1].ImageLen = ug_proc->hdr.FileImgHdr.ImgLen - 8;/*skip the signature*/
                    ug_proc->DownloadInfo[1].ImgOffset = ug_proc->hdr.FileImgHdr.Offset;
                    #else
                    ug_proc->DownloadInfo[0].ImgId = OTA_IMAG;
                    /* get OTA image and Write New Image to flash, skip the signature,
                    not write signature first for power down protection*/
                    ug_proc->DownloadInfo[0].FlashAddr = ug_proc->new_addr -SPI_FLASH_BASE + 8;
                    ug_proc->DownloadInfo[0].ImageLen = ug_proc->hdr.FileImgHdr.ImgLen - 8;/*skip the signature*/
                    ug_proc->DownloadInfo[0].ImgOffset = ug_proc->hdr.FileImgHdr.Offset;
                    #endif
                }
            }else {
                ug_proc->image_cnt = 1;
                ug_proc->DownloadInfo[0].ImgId = OTA_IMAG;
                /* get OTA image and Write New Image to flash, skip the signature,
                not write signature first for power down protection*/
                ug_proc->DownloadInfo[0].FlashAddr = ug_proc->new_addr -SPI_FLASH_BASE + 8;
                ug_proc->DownloadInfo[0].ImageLen = ug_proc->hdr.FileImgHdr.ImgLen - 8;/*skip the signature*/
                ug_proc->DownloadInfo[0].ImgOffset = ug_proc->hdr.FileImgHdr.Offset;
            }
            PR_NOTICE("FlashAddr = 0x%x", ug_proc->DownloadInfo[0].FlashAddr);
            PR_NOTICE("ImageLen = %d", ug_proc->DownloadInfo[0].ImageLen);
            PR_NOTICE("ImgOffset = %d", ug_proc->DownloadInfo[0].ImgOffset);
            PR_NOTICE("OTA Image Address = 0x%x", ug_proc->new_addr);
            if(ug_proc->hdr.RdpStatus == ENABLE) {
                PR_NOTICE("RDP Image Address = %x", RDP_FLASH_ADDR);
            }

            ug_proc->recv_data_cnt = ota_hd_len + sizeof(update_file_hdr); // (ug_proc->hdr.FileHdr.HdrNum * ug_proc->hdr.FileImgHdr.ImgHdrLen)
            *remain_len = len - (ota_hd_len+SIZEOF(update_file_hdr));
            ug_proc->stat = UGS_SEARCH_SIGN;
        }
        break;

        case UGS_SEARCH_SIGN: {
            if(ug_proc->recv_data_cnt + len < ug_proc->DownloadInfo[0].ImgOffset) {
                ug_proc->recv_data_cnt += len;
                *remain_len = 0;
                break;
            }
            PR_NOTICE("ug_proc->recv_data_cnt:%d",ug_proc->recv_data_cnt);
            PR_NOTICE("len:%d",len);

            INT_T offset = (ug_proc->DownloadInfo[0].ImgOffset - ug_proc->recv_data_cnt);
            *remain_len = len - offset;
            if(*remain_len < SIZEOF(ug_proc->signature)) {
                ug_proc->recv_data_cnt += offset;
                ug_proc->stat = UGS_RECV_SIGN;
            }else {
                mempcpy(ug_proc->signature,data+offset,SIZEOF(ug_proc->signature));
                *remain_len -= SIZEOF(ug_proc->signature);
                ug_proc->stat = UGS_RECV_IMG_DATA;
            }
        }
        break;

        case UGS_RECV_SIGN: {
            if(len < SIZEOF(ug_proc->signature)) { // 8 is signature
                *remain_len = len;
                break;
            }

            mempcpy(ug_proc->signature,data,SIZEOF(ug_proc->signature));

            #if 0
            INT_T i = 0;
            PR_DEBUG_RAW("ug_proc->signature:");
            for(i = 0;i < SIZEOF(ug_proc->signature);i++) {
                PR_DEBUG_RAW("%02x",ug_proc->signature[i]);
            }
            PR_DEBUG_RAW("\n");
            #endif

            ug_proc->recv_data_cnt += 8;
            *remain_len = len-8;
            ug_proc->stat = UGS_RECV_IMG_DATA;
        }
        break;

        case UGS_RECV_IMG_DATA: {
            #define RT_IMG_WR_UNIT 1024
            if(ug_proc->cur_image_cnt[0] >= ug_proc->DownloadInfo[0].ImageLen) {
                ug_proc->stat = UGS_FINISH;
                *remain_len = len;
                break;
            }

            if((ug_proc->cur_image_cnt[0] + len < ug_proc->DownloadInfo[0].ImageLen) && \
               (len < RT_IMG_WR_UNIT)) {
                *remain_len = len;
                break;
            }

            u32 write_len = 0;
            if(ug_proc->cur_image_cnt[0] + len < ug_proc->DownloadInfo[0].ImageLen) {
                write_len = RT_IMG_WR_UNIT;
            }else {
                write_len = (ug_proc->DownloadInfo[0].ImageLen - ug_proc->cur_image_cnt[0]);
            }
            device_mutex_lock(RT_DEV_LOCK_FLASH);
            #if 0
            {
                PR_DEBUG_RAW("wirite data:");
                INT_T i = 0;
                for(i = 0;i < 16;i++) {
                    PR_DEBUG_RAW("%02x ",data[i]);
                }
                PR_DEBUG_RAW("\n");
            }
            #endif
            if(flash_stream_write(&ug_proc->flash, ug_proc->DownloadInfo[0].FlashAddr + ug_proc->cur_image_cnt[0], \
                                  write_len, data) < 0) {
                device_mutex_unlock(RT_DEV_LOCK_FLASH);
                PR_ERR("[%s] Write sector failed");
                return OPRT_WR_FLASH_ERROR;
            }
            device_mutex_unlock(RT_DEV_LOCK_FLASH);
            ug_proc->cur_image_cnt[0] += write_len;
            ug_proc->recv_data_cnt += write_len;
            *remain_len = len - write_len;

            if(ug_proc->cur_image_cnt[0] >= ug_proc->DownloadInfo[0].ImageLen) {
                ug_proc->stat = UGS_FINISH;
                break;
            }
        }
        break;

        case UGS_FINISH: {
            *remain_len = 0;
        }
        break;
    }
    return OPRT_OK;
}


void set_prod_ssid(CHAR_T *ssid)
{
    if (strlen(ssid) > WF_SSID_LEN) {
        PR_ERR("ssid len over max value 32");
        return;
    }
    memset(prod_ssid_name, 0, sizeof(prod_ssid_name));
    strncpy(prod_ssid_name, ssid, WF_SSID_LEN);
    return;
}








