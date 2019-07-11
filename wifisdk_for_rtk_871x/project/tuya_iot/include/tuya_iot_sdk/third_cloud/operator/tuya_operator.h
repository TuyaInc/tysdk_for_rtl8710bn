#ifndef _TUYA_OPERATOR_H_
#define _TUYA_OPERATOR_H_

#include "tuya_cloud_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#if defined(TUYA_OPERATOR_TYPE) && ((TUYA_OPERATOR_TYPE==TUYA_OPERATOR_CMCC)||(TUYA_OPERATOR_TYPE==TUYA_OPERATOR_CMCC_ANDLINK) )
OPERATE_RET tuya_operator_cmcc_reg_params(IN CONST CHAR_T *p_appkey, IN CONST CHAR_T *p_aeskey,
                                          IN CHAR_T *p_deviceType,IN CHAR_T *p_productToken, UINT_T localport);
#endif

typedef struct {              /*   CTCC     CMCC     */
    CHAR_T *fwver;            /*    Y                */ //
    CHAR_T *device_info;      /*             Y       */ //备用(不上报)
    CHAR_T *app_info;         /*             Y       */ //应用信息(App集合，每条日志用\r\n分隔，每条日志包含 App名称、包名、App使用时长(毫秒)、App打开次数 并由’|’分隔 )
    CHAR_T *rom;              /*             Y       */ //rom信息
    CHAR_T *ram;              /*             Y       */ //ram信息
    CHAR_T *cpu;              /*             Y       */ //cpu信息
    CHAR_T *sys_ver;          /*             Y       */ //操作系统版本号
    CHAR_T *soft_ver;         /*             Y       */ //固件版本号
    CHAR_T *soft_name;        /*             Y       */ //固件名称
    CHAR_T *volte;            /*             Y       */ //volte开关状态
    CHAR_T *nettype;          /*             Y       */ //当前网络类型
    CHAR_T *account;          /*             Y       */ //宽带账号
    CHAR_T *p_num;            /*             Y       */ //本机号码
    CHAR_T *location;         /*             Y       */ //位置信息(包含经度，维度，定位模式，以逗号‘,’分隔)
}TY_OPERATOR_INFO_S;

OPERATE_RET tuya_operator_upload_manual(IN CONST TY_OPERATOR_INFO_S *p_info);

OPERATE_RET tuya_operator_upload(VOID);


#ifdef __cplusplus
}
#endif

#endif
