/***********************************************************
*  File: sdk_version.h
*  Author:
*  Date:
***********************************************************/
#ifndef _SDK_VERSION_H
#define _SDK_VERSION_H

#include "tuya_iot_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
IOT_BV(BS_VER) 30.00 < - > IOT_SDK 4.1.0
IOT_BV(BS_VER) 30.01 < - > IOT_SDK 4.1.1 < - > RTL8170 2.0.0
IOT_BV(BS_VER) 30.02 < - > IOT_SDK 4.1.2
IOT_BV(BS_VER) 30.03 < - > IOT_SDK 4.1.3


*/

#define BS_VER "30.04"
#define PT_VER "2.2"
#if defined(ENABLE_LAN_ENCRYPTION) && (ENABLE_LAN_ENCRYPTION==1)
#define LAN_PRO_VER "3.3"   //必须3.3版本及以上才支持局域网加密功能
#else
#define LAN_PRO_VER "3.2"   //3.3以下不支持局域网加密
#endif
#define CAD_VER "1.0.2" //1.0.0:启用CID机制,固件升级启用4.1; 1.0.1:APP优化流程; 1.0.2:增加mbid，用于群组控制
#define CD_VER "1.0.0" //固件支持mqtt消息确认

#if defined(WIFI_GW) && (WIFI_GW==1)
#define DEV_ATTR_THINGCONFIG        (1 << 0)
#else
#define DEV_ATTR_THINGCONFIG        (0)
#endif

#define DEV_ATTR_LOCAL_DEV_DP_QUERY           (1<<1)

#if defined(ENABLE_LOCAL_LINKAGE) && (ENABLE_LOCAL_LINKAGE==1)
#define DEV_ATTR_LOCAL_SCENE_LINKAGE      (1<<2)
#else
#define DEV_ATTR_LOCAL_SCENE_LINKAGE      (0)
#endif


//设备属性,采用按位与方式
#define DEV_ATTRIBUTE               (DEV_ATTR_THINGCONFIG | DEV_ATTR_LOCAL_DEV_DP_QUERY | DEV_ATTR_LOCAL_SCENE_LINKAGE)

#define INT2STR(NUM) #NUM
#define I2S(R) INT2STR(R)

#define SDK_INFO_1 "< TUYA IOT SDK V:"IOT_SDK_VER" BS:"BS_VER"_PT:"PT_VER"_LAN:"LAN_PRO_VER"_CAD:"CAD_VER"_CD:"CD_VER" >"
#define SDK_INFO_2 "< tuya_iot_lib BUILD AT:"BUILD_DATE"_"BUILD_TIME" BY "GIT_USER" AT "TARGET_PLATFORM" >"
#define SDK_IOT_ATTR "IOT DEFS <"\
" WIFI_GW:"I2S(WIFI_GW)\
" DEBUG:"I2S(TUYA_IOT_DEBUG)\
" KV_FILE:"I2S(KV_FILE)\
" SHUTDOWN_MODE:"I2S(SHUTDOWN_MODE)\
" LITTLE_END:"I2S(LITTLE_END)\
" TLS_MODE:"I2S(TLS_MODE)\
" ENABLE_LOCAL_LINKAGE:"I2S(ENABLE_LOCAL_LINKAGE)\
" ENABLE_CLOUD_OPERATION:"I2S(ENABLE_CLOUD_OPERATION)\
" ENABLE_SUBDEVICE:"I2S(ENABLE_SUBDEVICE)\
" ENABLE_ENGINEER_TO_NORMAL:"I2S(ENABLE_ENGINEER_TO_NORMAL)\
" OPERATING_SYSTEM:"I2S(OPERATING_SYSTEM)\
" ENABLE_SYS_RPC:"I2S(ENABLE_SYS_RPC)\
" TY_SECURITY_CHIP:"I2S(TY_SECURITY_CHIP)\
" RELIABLE_TRANSFER:"I2S(RELIABLE_TRANSFER)\
" ENABLE_LAN_ENCRYPTION:"I2S(ENABLE_LAN_ENCRYPTION)\
" ENABLE_SIGMESH:"I2S(ENABLE_SIGMESH)\
" >"


#if defined(ENABLE_IPC) && (ENABLE_IPC==1)
#define IPC_SDK_INFO "<TUYA IPC SDK V:"IPC_SDK_VER" >"
#define SDK_IPC_ATTR "IPC DEFS <"\
" ENABLE_ECHO_SHOW:"I2S(ENABLE_ECHO_SHOW)\
" ENABLE_CHROMECAST:"I2S(ENABLE_CHROMECAST)\
" ENABLE_CLOUD_STORAGE:"I2S(ENABLE_CLOUD_STORAGE)\
" >"

#define SDK_INFO IPC_SDK_INFO"\r\n"SDK_INFO_1"\r\n"SDK_IPC_ATTR"'\r\n"SDK_INFO_2"\r\n"
#else
#define SDK_INFO SDK_INFO_1"\r\n"SDK_INFO_2"\r\n"SDK_IOT_ATTR"\r\n"
#endif


#ifdef __cplusplus
}
#endif
#endif /*_SDK_VERSION_H*/

