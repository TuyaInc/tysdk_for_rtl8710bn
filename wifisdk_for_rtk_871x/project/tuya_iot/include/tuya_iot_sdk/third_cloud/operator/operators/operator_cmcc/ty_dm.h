#ifndef _TY_DM_H
#define _TY_DM_H

#include "tuya_operator.h"

#ifdef __cplusplus
    extern "C" {
#endif

OPERATE_RET ty_dm_coap_init(IN CHAR_T *cmei, IN TY_OPERATOR_INFO_S *p_info,
                            IN CHAR_T *p_appkey, IN CHAR_T *p_aeskey,
                            IN UINT_T local_coap_port,
                            IN CHAR_T *p_deviceType,IN CHAR_T *p_productToken);

#ifdef __cplusplus
}
#endif
#endif

