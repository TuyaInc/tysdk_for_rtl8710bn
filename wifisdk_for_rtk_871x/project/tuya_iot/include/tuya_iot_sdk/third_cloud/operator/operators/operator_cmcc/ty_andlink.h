#ifndef __TY_ANDLINK_H
#define __TY_ANDLINK_H

#include "adapter_platform.h"
#include "tuya_cloud_error_code.h"
#include "tuya_operator.h"
#include "tuya_coap_helper.h"


#ifdef __cplusplus
    extern "C" {
#endif


CHAR_T *ty_andlink_get_devId(VOID);
OPERATE_RET ty_andlink_init_coap_res_cb(IN TY_COAP_SERVER_HANLDER handler);
OPERATE_RET ty_andlink_init(IN CHAR_T *p_firmwareVersion, IN CHAR_T *p_softwareVersion, IN CHAR_T *p_deviceType, IN CHAR_T *p_productToken);
OPERATE_RET ty_andlink_reset(VOID);


#ifdef __cplusplus
}
#endif
#endif
