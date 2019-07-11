/***********************************************************
*  File: Z32HUA_encrypt.h
*  Author: nzy
*  Date: 20180810
***********************************************************/
#ifndef _Z32HUA_ENCRYPT_H
#define _Z32HUA_ENCRYPT_H

#include "tuya_cloud_types.h"
/***********************************************************
*************************micro define***********************
***********************************************************/

//APDU 应用协议数据单元 ------
//请求格式
typedef struct 
{
   	UCHAR_T		cmd[4]; 			//指令   CLA + INS + P1 + P2
   	UCHAR_T		Lc; 				//指令长度
   	UCHAR_T		DataIn[256]; 		//指令参数
    UCHAR_T		Le; 				//期望响应长度
} APDU_SEND;

//响应格式
typedef struct 
{
	UINT_T		LenOut;				//响应长度
	UCHAR_T 	DataOut[256];		//回应数据
	USHORT		SW;					//回应状态字
} APDU_RESP;
//------

//安全模块子密钥信息
typedef struct 
{
   	UCHAR_T		SEkey[24]; 			//子密钥
   	UCHAR_T		produceDate[4]; 	//生产日期			YYYYMMDD
   	UCHAR_T		expireDate[4]; 		//有效日期			YYYYMMDD
    UCHAR_T		factoryCode[8]; 	//涂鸦厂商代码
    UCHAR_T		algorithmCode; 		//算法标识  
} SEKeyInfo_s;

//初始化认证信息
typedef struct 
{
   	UCHAR_T		randomA[4]; 		//随机数A
   	UCHAR_T		chipID[16]; 		//加密芯片ID		16字节
   	UCHAR_T		mac1[8]; 			//安全认证码1
} InternalAuthInfo_s;

//下行认证信息
typedef struct 
{
   	UCHAR_T		internalAuthResult; //上行认证结果		0x05 - 认证成功 	0xA5 - 认证失败
   	UCHAR_T		randomB[4]; 		//随机数B
   	UCHAR_T		mac2[8]; 			//安全认证码1
} ExternalAuthInfo_s;

//上行认证结果
#define INTERNAL_AUTH_SUCCESS	0x05	//认证成功		
#define INTERNAL_AUTH_FAILED	0xA5	//认证失败



//SE APDU应答状态字
#define SE_OK			0x9000		//命令执行成功

//SE命令处理状态
#define SE_CMD_OK		0			//命令处理成功
#define SE_MALLOC_ERR	-10			//内存分配错误
#define SE_CMD_ERR		-11			//命令执行错误


/***********************************************************
*************************   extern   ***********************
***********************************************************/
extern VOID SEChip_I2C_Init(VOID);
extern INT_T SE_SelectDirByName(VOID);
extern INT_T SE_InitAuthenticate(	OUT UCHAR_T *outAuthInfo);
extern INT_T SE_ExternalAuthenticate(IN UCHAR_T *inputData);
extern INT_T SE_WriteAuthData(IN UCHAR_T *inputData, IN UCHAR_T inputLen);
extern INT_T SE_ReadAuthData(OUT UCHAR_T *outData, OUT UCHAR_T *outLen);

extern VOID SE_EncyptDemoTest(VOID);


#endif

