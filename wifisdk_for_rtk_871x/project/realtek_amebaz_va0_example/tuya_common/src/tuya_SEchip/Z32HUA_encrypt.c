
/***********************************************************
*  File: Z32HUA_encrypt.c
*  Author: xl
*  Date: 20180806
***********************************************************/
#ifndef _TUYA_I2C_ENCRYPT_
#define _TUYA_I2C_ENCRYPT_

#include "tuya_cloud_types.h"
#include "tuya_cloud_error_code.h"
#include "uni_time_queue.h"
#include "uni_log.h"
#include "uni_mutex.h"
#include "tuya_gpio.h"
#include "i2c_api.h"
#include "bpoi.h"
#include "Z32HUA_encrypt.h"
#include "Z32HUA_des.h"
#include "aes_inf.h"

/***********************************************************
*************************micro define***********************
***********************************************************/
/*I2C pin location: 引脚选择
* I2C0:
*	  - S0:  PA_1(SCL)/PA_4(SDA).
*	  - S1:  PA_22(SCL)/PA_19(SDA).
*	  - S2:  PA_29(SCL)/PA_30(SDA).
*
* I2C1:
*	  - S0:  PA_3(SCL)/PA_2(SDA). 
*	  - S1:  PA_18(SCL)/PA_23(SDA).
*	  - S2:  PA_28(SCL)/PA_27(SDA).
*/

//I2C引脚选择 || 从机地址 || 频率定义
//I2C1_SEL S1 
#if 1
#define Z32HUA_I2C_MTR_SDA    		PA_19		
#define Z32HUA_I2C_MTR_SCL    		PA_22
#else 
#define Z32HUA_I2C_MTR_SDA    		PA_23
#define Z32HUA_I2C_MTR_SCL    		PA_18
#endif
#define Z32HUA_I2C_SLAVE_ADDR0    	0x50		//Z32HUA芯片   I2C从机地址
#define Z32HUA_I2C_BUS_CLK        	100000  	//100Khz 	 时钟频率		
/****************************************/


/***********************************************************
*************************variable define********************
***********************************************************/
i2c_t 	i2cEncrypt;					//I2C接口	
STATIC APDU_RESP  apduResp;			//接收数据结构体
STATIC bpoi_ctrl  sBpoiCtrl;		//BPOI协议结构体

#if 0
//子密钥信息（举例）
STATIC SEKeyInfo_s SEKeyInfo =
{
	.SEkey = {0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7,0xB0, 0xB1, 0xB2, 0xB3,\
		0xB4, 0xB5, 0xB6, 0xB7, 0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7},
	
	.produceDate = {0x20, 0x18, 0x09, 0x22},	
	
	.expireDate = {0x20, 0x99, 0x09, 0x22},

	.factoryCode = {0x54, 0x7C, 0x55, 0x7C, 0x59, 0x7C, 0x41, 0x7C},
	
	.algorithmCode = 0x01
};	
#endif
/***********************************************************
*************************function declare********************
***********************************************************/
STATIC VOID i2cEncpyt_Reset(VOID);
VOID MacCalculate(UINT8_T* key, UINT8_T* initValue, UINT8_T* inData, UINT8_T inLen, UINT8_T* mac);
VOID ThreeDes_CBC_Encode(IN UINT8_T* key, IN UINT8_T* initValue, INOUT UINT8_T* inOutData, INOUT UINT8_T* inOutLen);

/***********************************************************
*************************function define********************
***********************************************************/
/***********************************************************
*  Function: __tuya_i2c_write
*  Input:	pvInBuffer  - 发送缓冲区
*			ui32NumBytes  - 待发送字节数
*  Output:	none
*  Return:	remain bytes
***********************************************************/
STATIC INT_T __tuya_i2c_write(IN VOID *pvInBuffer, IN UINT_T ui32NumBytes)
{
	UINT_T len;
	UCHAR_T* ptr = (UCHAR_T*)pvInBuffer;
	INT_T slaveDevExist;
	
#if 0
	slaveDevExist = i2c_write_timeout(&i2cEncrypt, Z32HUA_I2C_SLAVE_ADDR0, (const UCHAR_T *)ptr, 0, 1, 5);//测试从设备是否存在
	if(slaveDevExist < 0) {
		PR_ERR("slave dev not exist");	//从设备不存在
		return BPOI_ERR_NAK;
	}
#endif	


#if 1
	PR_DEBUG_RAW("===> ");
	for(UCHAR_T i = 0; i < ui32NumBytes; i++) {
		PR_DEBUG_RAW("%02x ", ptr[i]);	
	}
	PR_DEBUG_RAW("\r\n");
#endif	

	len = i2c_write_timeout(&i2cEncrypt, Z32HUA_I2C_SLAVE_ADDR0, (const UCHAR_T *)ptr, (INT_T)ui32NumBytes, 1, 5);		
	if(len < ui32NumBytes) {
		PR_ERR("i2c tx err sent %d / %d", len , ui32NumBytes);			//发送出错
		//i2cEncpyt_Reset(); //复位加密芯片
		return BPOI_ERR_NAK;	
	}
	else {
		return BPOI_ERR_NONE;
	}
}

/***********************************************************
*  Function: __tuya_i2c_read
*  Input:	none
*  Output:	pvOutBuffer  - 接收缓冲区
*			ui32NumBytes  - 待发送字节数
*  Return:	remain bytes
***********************************************************/
STATIC INT_T __tuya_i2c_read(OUT VOID *pvOutBuffer, OUT UINT_T ui32NumBytes)
{
	UINT_T len;
	UCHAR_T* ptr = (UCHAR_T*)pvOutBuffer;

	len = i2c_read(&i2cEncrypt, Z32HUA_I2C_SLAVE_ADDR0, ptr, (INT_T)ui32NumBytes, 1);
	if(len < ui32NumBytes) {
		PR_ERR("i2c Rx err！rcv %d / %d", len , ui32NumBytes);			//接收出错
		//i2cEncpyt_Reset();
		return BPOI_ERR_NAK;	
	}
	
#if 1
	PR_DEBUG_RAW("<=== ");
	for(UCHAR_T i = 0; i < ui32NumBytes; i++) {	
		PR_DEBUG_RAW("%02x ", ptr[i]);
	}
	PR_DEBUG_RAW("\r\n");
#endif

	return BPOI_ERR_NONE;
}

/***********************************************************
*  Function: __tuya_delay_ms
*  Input:	ui32MilliSeconds - 延时时长 单位:ms
*  Output:	none
*  Return:	none
***********************************************************/
STATIC VOID __tuya_delay_ms(UINT_T ui32MilliSeconds)
{
	SystemSleep(ui32MilliSeconds);
	return;
}

/***********************************************************
* Function Name: void init_bpoi_parameter(bpoi_ctrl **psBpoiCtrl)
* Description:   BPOI协议 参数初始化
* Input:         bpoi_ctrl **psBpoiCtrl
* Output:        none
* Return:        none
***********************************************************/
VOID init_bpoi_parameter(bpoi_ctrl **psBpoiCtrl)
{
	sBpoiCtrl.i2c_write =  __tuya_i2c_write;
	sBpoiCtrl.i2c_read = __tuya_i2c_read;
	sBpoiCtrl.delay_ms = __tuya_delay_ms;
	sBpoiCtrl.ui16I2cTxMaxSize = 255;
	sBpoiCtrl.ui16I2cRxMaxSize = 255;
	sBpoiCtrl.ui8RetryCnt = 3;
	sBpoiCtrl.ui8ExtendTime = 10;
	sBpoiCtrl.ui8NakTimeout = 50;
	sBpoiCtrl.ui8OpInterval = 5;
	sBpoiCtrl.ui8CRInterval = 5;
	sBpoiCtrl.ui8RstTime = 5;	
	
	*psBpoiCtrl = &sBpoiCtrl;
	return;
}

/***********************************************************
* Function Name: SE_bpoi_cmd_process
* Description:   加密芯片命令处理接口函数
* Input:         cmdBuf - 命令地址
*				 cmdLen - 命令长度
* Output:        none
* Return:        result ( 0 -OK,  < 0 - failed )
***********************************************************/
STATIC INT_T SE_bpoi_cmd_process(IN UCHAR_T *cmdBuf, IN INT_T cmdLen)
{
	INT_T ret;
	UCHAR_T *ptr;
	
	sBpoiCtrl.ui8CmdType =  BPOI_IND_APDU;								//命令类型：apdu命令
	memset((UCHAR_T *)&apduResp, 0, sizeof(apduResp.DataOut));			//接收缓冲区清0

	ret = bpoi_apdu_cmd_process(cmdBuf, cmdLen, apduResp.DataOut, &apduResp.LenOut, &sBpoiCtrl);		
	if(ret == 0) {	
		ptr = apduResp.DataOut + apduResp.LenOut - 2;
		apduResp.SW = (((USHORT_T) ptr[0]) << 8 ) +  ((USHORT_T)ptr[1]);
	
		PR_DEBUG_RAW("apduResp.SW = 0x%04x! apduResp.LenOut = %d \r\n", apduResp.SW, apduResp.LenOut);
		for(UCHAR_T i = 0; i < apduResp.LenOut; i++) {	
			PR_DEBUG_RAW("%02x ", apduResp.DataOut[i]);
		}
		PR_DEBUG_RAW("\r\n");
	}
	return ret;
}


/***********************************************************
* Function Name: SE_RST_cmd_process
* Description:   加密芯片复位命令(软复位)
* Input:         none
* Output:        none
* Return:        result ( 0 -OK,  < 0 - failed )
***********************************************************/
STATIC VOID SE_RST_cmd(VOID)
{
	INT_T ret;
	
	sBpoiCtrl.ui8CmdType =  BPOI_IND_RST;								//命令类型：RST命令
	memset((UCHAR_T *)&apduResp, 0, sizeof(apduResp.DataOut));			//接收缓冲区清0

	ret = bpoi_apdu_cmd_process(NULL, 0, apduResp.DataOut, &apduResp.LenOut, &sBpoiCtrl);		
	PR_DEBUG_RAW(" ret = %d\r\n", ret);
	
	SystemSleep(100);		
	return;
}

/***********************************************************
* Function Name: SE_SelectDirByName
* Description:   选择应用/目录
* Input:         none
* Output:        none
* Return:        result ( 0 -OK,  < 0 - failed )
***********************************************************/
UCHAR_T* TuyaAppletName = "nation.tuya";	//应用名
INT_T SE_SelectDirByName(VOID)
{
	INT_T ret;
	UCHAR_T command[20];

	PR_DEBUG("SE_SelectDirByName!");	
	memcpy(command, "\x00\xA4\x04\x00", 4);				//CLA + INS + P1 + P2
	command[4]= 0x0C;									//Le = 0x08
	snprintf(command + 5, strlen(TuyaAppletName) + 1, TuyaAppletName);
	
	ret = SE_bpoi_cmd_process(command, 5 + strlen(TuyaAppletName));
	if((ret == 0) && (apduResp.SW == SE_OK )) { 	
		PR_DEBUG("SE_SelectDirByName OK! \r\n");
	}
	else {
		PR_ERR("SE_SelectDirByName err ret=%d SW=0x%04x\r\n", ret, apduResp.SW);
		ret = SE_CMD_ERR;
	}
	return ret;
}

/***********************************************************
* Function Name: SE_WriteSEChipID
* Description:   写入加密芯片ID
* Input:         none
* Output:        chipID - 16bytes芯片ID
* Return:        result ( 0 -OK,  < 0 - failed )
***********************************************************/
INT_T SE_WriteSEChipID(IN UCHAR_T *chipID)
{
	INT_T ret;
	UCHAR_T command[32];

	PR_DEBUG("SE_WriteSEChipID!");	
	memcpy(command, "\x80\xF7\x00\x00", 4);				//CLA + INS + P1 + P2
	command[4]= 0x10;									//Lc = 0x10
	memcpy(command + 5, chipID, 0x10);
	
	ret = SE_bpoi_cmd_process(command, 5 + 0x10);
	if((ret == 0) && (apduResp.SW == SE_OK )) { 	
		memcpy(chipID, apduResp.DataOut, apduResp.LenOut - 2);
		PR_DEBUG("SE_WriteSEChipID OK! \r\n");
	}
	else {
		PR_ERR("SE_WriteSEChipID err ret=%d SW=0x%04x\r\n", ret, apduResp.SW);
		ret = SE_CMD_ERR;
	}
	return ret;
}


/***********************************************************
* Function Name: SE_GetSEChipID
* Description:   获取加密芯片ID
* Input:         none
* Output:        chipID - 16bytes芯片ID
* Return:        result ( 0 -OK,  < 0 - failed )
***********************************************************/
INT_T SE_GetSEChipID(OUT UCHAR_T *chipID)
{
	INT_T ret;
	UCHAR_T command[5];

	PR_DEBUG("SE_GetSEChipID!");	
	memcpy(command, "\x80\xF8\x00\x00", 4);				//CLA + INS + P1 + P2
	command[4]= 0x10;									//Le = 0x10
	
	ret = SE_bpoi_cmd_process(command, 5);
	if((ret == 0) && (apduResp.SW == SE_OK )) { 	
		memcpy(chipID, apduResp.DataOut, apduResp.LenOut - 2);
		PR_DEBUG("SE_GetSEChipID OK! \r\n");
	}
	else {
		PR_ERR("SE_GetSEChipID err ret=%d SW=0x%04x\r\n", ret, apduResp.SW);
		ret = SE_CMD_ERR;
	}
	return ret;
}

/***********************************************************
* Function Name: SE_GetChallenge
* Description:   获取随机数
* Input:         none
* Output:        random - 4bytes真随机数
* Return:        result ( 0 -OK,  < 0 - failed )
***********************************************************/
STATIC INT_T SE_GetChallenge(OUT UCHAR_T *random)
{
	INT_T ret;
	UCHAR_T command[5];
	
	PR_DEBUG("SE_GetChallenge!");	
	memcpy(command, "\x80\x84\x00\x00", 4);				//CLA + INS + P1 + P2
	command[4]= 0x04;									//Le = 0x04
	ret = SE_bpoi_cmd_process(command, 5);

	if((ret == 0) && (apduResp.SW == SE_OK )) {
		memcpy(random, apduResp.DataOut, apduResp.LenOut - 2);
		PR_DEBUG("SE_GetChallenge OK! \r\n");
	}
	else {
		PR_ERR("SE_GetChallenge err ret=%d	SW=0x%04x\r\n", ret, apduResp.SW);
		ret = SE_CMD_ERR;
	}	
	return ret;
}

/***********************************************************
* Function Name: SE_InitAuthenticate
* Description:   初始化认证命令
* Input:         none
* Output:        outAuthInfo - 初始化认证产生的数据
* Return:        result ( 0 -OK,  < 0 - failed )
***********************************************************/
INT_T SE_InitAuthenticate(	OUT UCHAR_T *outAuthInfo)
{
	INT_T ret;
	UCHAR_T command[5];

	PR_DEBUG("SE_InitAuthenticate!");	
	memcpy(command, "\x80\x88\x00\x00", 4);				//CLA + INS + P1 + P2
	command[4]= 0x1C;									//Le = 0x1C
	ret = SE_bpoi_cmd_process(command, 5);

	if((ret == 0) && (apduResp.SW == SE_OK )) {
		memcpy(outAuthInfo, apduResp.DataOut, apduResp.LenOut - 2);
		PR_DEBUG("SE InitAuthenticate OK! \r\n");
	}
	else {
		PR_ERR("SE InitAuthenticate err ret=%d	SW=0x%04x\r\n", ret, apduResp.SW);
		ret = SE_CMD_ERR;
	}
	return ret;
}

/***********************************************************
* Function Name: SE_ExternalAuthenticate
* Description:   下行认证命令
* Input:         inputData - 输入数据
* Output:        apduResp.DataOut 
* Return:        result ( 0 -OK,  < 0 - failed )
***********************************************************/
INT_T SE_ExternalAuthenticate(IN UCHAR_T *inputData)
{
	INT_T ret;
	UCHAR_T command[32];

	PR_DEBUG("SE_ExternalAuthenticate!");	
	memcpy(command, "\x84\x82\x00\x00", 4);				//CLA + INS + P1 + P2
	command[4]= 0x0D;									//Lc = 0x0D
	memcpy(command + 5, inputData, command[4]); 		//DataIn
	ret = SE_bpoi_cmd_process(command, command[4] + 5);

	if((ret == 0) && (apduResp.SW == SE_OK )) { 	
		PR_DEBUG("SE_ExternalAuthenticate OK! \r\n");
	}
	else {
		PR_ERR("SE_ExternalAuthenticate err ret=%d	SW=0x%04x\r\n", ret, apduResp.SW);
		ret = SE_CMD_ERR;
	}
	return ret;
}

/***********************************************************
* Function Name: SE_InsertKey
* Description:   下载安全模块子密钥
* Input:         inKeyInfo - 输入数据
* Output:        apduResp.DataOut 
* Return:        result ( 0 -OK,  < 0 - failed )
***********************************************************/
INT_T SE_InsertKey(IN SEKeyInfo_s* inKeyInfo)
{
	INT_T ret;
	UCHAR_T command[64];
	
	PR_DEBUG("SE_InsertKey!");	
	memcpy(command, "\x80\xB6\x00\x00", 4);			//CLA + INS + P1 + P2
	command[4]= sizeof(SEKeyInfo_s);				//Lc = 0x29
	memcpy(command + 5, (UCHAR_T* )inKeyInfo, command[4]); 	//DataIn
	
	ret = SE_bpoi_cmd_process(command, command[4] + 5);
	if((ret == 0) && (apduResp.SW == SE_OK )) { 	
		PR_DEBUG("SE_InsertKey OK! \r\n");
	}
	else {
		PR_ERR("SE_InsertKey err ret=%d	SW=0x%04x\r\n", ret, apduResp.SW);
		ret = SE_CMD_ERR;
	}
	return ret;
}

/***********************************************************
* Function Name: SE_VerifyKey
* Description:   验证安全模块子密钥
* Input:         inOutData - 输入数据
* Output:        inOutData - 加密数据
* Return:        result ( 0 -OK,  < 0 - failed )
***********************************************************/
INT_T SE_VerifyKey(INOUT UCHAR_T *inOutData)
{
	INT_T ret;
	UCHAR_T command[16];
	
	PR_DEBUG("SE_VerifyKey!");	
	memcpy(command, "\x80\xB5\x00\x00", 4);			//CLA + INS + P1 + P2
	command[4]= 0x08;								//Lc = 0x08
	memcpy(command + 5, inOutData, command[4]); 	//DataIn
	
	ret = SE_bpoi_cmd_process(command, command[4] + 5);
	if((ret == 0) && (apduResp.SW == SE_OK )) { 		
		memcpy(inOutData, apduResp.DataOut, apduResp.LenOut - 2);
		PR_DEBUG("SE_VerifyKey OK! \r\n");
	}
	else {
		PR_ERR("SE_VerifyKey err ret=%d	SW=0x%04x\r\n", ret, apduResp.SW);
		ret = SE_CMD_ERR;
	}	
	return ret;
}

/***********************************************************
* Function Name: SE_WriteAuthData
* Description:   写入授权数据（下载授权数据到安全芯片的flash内保存）
* Input:         inputData - 输入数据
* 		         inputLen - 输入数据长度
* Output:        apduResp.DataOut 
* Return:        result ( 0 -OK,  < 0 - failed )
***********************************************************/
INT_T SE_WriteAuthData(IN UCHAR_T *inputData, IN UCHAR_T inputLen)
{
	INT_T ret;
	UCHAR_T *cmdBuf;

	cmdBuf = (UCHAR_T *)Malloc(inputLen + 10);
	if(cmdBuf == NULL) {
		ret = SE_MALLOC_ERR;
		PR_ERR("SE malloc err");
		return ret;
	}

	PR_DEBUG("SE_WriteAuthData!");	
	memcpy(cmdBuf, "\x80\xB7\x00\x00", 4);			//CLA + INS + P1 + P2
	cmdBuf[4]= inputLen;							//Lc = data length
	memcpy(cmdBuf + 5, inputData, inputLen); 		//DataIn
	cmdBuf[inputLen + 5]= 0;						//Le = 0
	
	ret = SE_bpoi_cmd_process(cmdBuf, inputLen + 6);
	if((ret == 0) && (apduResp.SW == SE_OK )) { 	
		PR_DEBUG("SE_WriteAuthData OK! \r\n");
	}
	else {
		PR_ERR("SE_WriteAuthData err ret=%d	SW=0x%04x\r\n", ret, apduResp.SW);
		ret = SE_CMD_ERR;
	}
	
	Free(cmdBuf);
	return ret;
}

/***********************************************************
* Function Name: SE_ReadAuthData
* Description:   读取授权数据
* Input:         none
* Output:        outData - 读出数据; outLen - 读出长度地址
* Return:        result ( 0 -OK,  < 0 - failed )
***********************************************************/
INT_T SE_ReadAuthData(OUT UCHAR_T *outData, OUT UCHAR_T *outLen)
{
	INT_T ret;
	UCHAR_T command[5];

	PR_DEBUG("SE_ReadAuthData!");
	memcpy(command, "\x80\xB2\x00\x00", 4);			//CLA + INS + P1 + P2
	command[4]= 0;									//Le = 0
	
	ret = SE_bpoi_cmd_process(command, 5);
	if((ret == 0) && (apduResp.SW == SE_OK )) { 	
		memcpy(outData, apduResp.DataOut, apduResp.LenOut - 2);	
		*outLen = apduResp.LenOut - 2;				//获得长度，去掉最后2字节状态字
		PR_DEBUG("SE_ReadAuthData OK! \r\n");
	}
	else {
		PR_ERR("SE_ReadAuthData err ret=%d SW=0x%04x\r\n", ret, apduResp.SW);
		ret = SE_CMD_ERR;
	}

	return ret;
}

/***********************************************************
* Function Name: SE_CloseAuth
* Description:   权限封闭
* Input:         none
* Output:        none 
* Return:        result ( 0 -OK,  < 0 - failed )
***********************************************************/
STATIC INT_T SE_CloseAuth(VOID)
{
	INT_T ret;
	UCHAR_T command[5];

	PR_DEBUG("SE_CloseAuth!");	
	memcpy(command, "\x80\xB4\x00\x00", 4);			//CLA + INS + P1 + P2
	
	ret = SE_bpoi_cmd_process(command, 4);
	if((ret == 0) && (apduResp.SW == SE_OK )) { 	
		PR_DEBUG("SE_CloseAuth OK! \r\n");
	}
	else {
		PR_ERR("err ret=%d SW=0x%04x\r\n", ret, apduResp.SW);
		ret = SE_CMD_ERR;
	}
	return ret;
}

/***********************************************************
*  Function: i2cEncpyt_Reset 自身主I2C复位并初始化
*  Input: 	none
*  Output: 	none
*  Return:  none
***********************************************************/
STATIC VOID i2cEncpyt_Reset(VOID)
{
	PR_DEBUG("I2C reset! ");

	//i2c reset 	
	i2c_reset(&i2cEncrypt);
	SystemSleep(10);
#if 0	
	tuya_gpio_inout_set(Z32HUA_I2C_MTR_SDA, FALSE);		//SDA设置输出，并上拉
	tuya_gpio_inout_set(Z32HUA_I2C_MTR_SCL, FALSE);
	tuya_gpio_write(Z32HUA_I2C_MTR_SDA, TRUE);			//SDA设置输出高

	for(UCHAR_T i = 0; i < 9; i++) {
		tuya_gpio_write(Z32HUA_I2C_MTR_SCL, FALSE);		//SCL设置输出低
		SystemSleep(1);
		tuya_gpio_write(Z32HUA_I2C_MTR_SCL, TRUE);		//SCL设置输出高
	}

	GPIO_DeInit(Z32HUA_I2C_MTR_SDA);
	GPIO_DeInit(Z32HUA_I2C_MTR_SCL);
#endif

	//i2c as Master 
	i2c_init(&i2cEncrypt, Z32HUA_I2C_MTR_SDA , Z32HUA_I2C_MTR_SCL);
	i2c_frequency(&i2cEncrypt, Z32HUA_I2C_BUS_CLK); 
	
	i2c_slave_address(&i2cEncrypt, 0, Z32HUA_I2C_SLAVE_ADDR0, 0);
	SystemSleep(10);
	return;
}

/***********************************************************
*  Function: Aes192CbcDecodeTestDemo AES192-CBC 解密测试DEMO
*  Input: 	none
*  Output: 	none
*  Return:  none
***********************************************************/
VOID Aes192CbcDecodeTestDemo(VOID)
{
#if 0

	UCHAR_T	*AES_OUT;
	UCHAR_T	AES_OUT_LEN;
	OPERATE_RET ret;

	// AES user input data
	u8	 AES_PLAIN[64] = 
	{
		0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xAA,0xBB,0xCC,0xDD,0xEE,0xFF,
		0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xAA,0xBB,0xCC,0xDD,0xEE,0xFF,
		0x89,0xF8,0x65,0x22,0x51,0x3C,0x05,0x3D,0x4A,0xB3,0xBE,0xFD,0x8D,0x9E,0x19,0xD0,
		0x89,0xF8,0x65,0x22,0x51,0x3C,0x05,0x3D,0x4A,0xB3,0xBE,0xFD,0x8D,0x9E,0x19,0xD0
	};

	UCHAR_T	AES_Key[32]   = 
	{
		0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
		0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F
	};
	UCHAR_T	AES_IV[16]	  = 
	{
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
	};
		

	PR_DEBUG_RAW("Aes192CbcDecodeTestDemo");
	ret = aes192_cbc_decode(AES_PLAIN, 64, AES_Key, AES_IV, &AES_OUT, &AES_OUT_LEN);
	PR_DEBUG_RAW("ret =  %d:", ret);
	PR_DEBUG_RAW("\r\n");
	PR_DEBUG_RAW("decode result %d:", AES_OUT_LEN);
	for(UCHAR_T i = 0; i < AES_OUT_LEN; i++) {	
		PR_DEBUG_RAW("%02x", AES_OUT[i]);
	}
	PR_DEBUG_RAW("\r\n");	
	Free(AES_OUT);

	PR_DEBUG_RAW("AES_PLAIN  %d:" );
	for(UCHAR_T i = 0; i < 64; i++) {	
		PR_DEBUG_RAW("%02x", AES_PLAIN[i]);
	}
	PR_DEBUG_RAW("\r\n");		
#endif

	return;

}

/***********************************************************
*  Function: Aes192CbcEncodeTestDemo AES192-CBC 加密测试DEMO
*  Input: 	none
*  Output: 	none
*  Return:  none
***********************************************************/
VOID Aes192CbcEncodeTestDemo(VOID)
{
#if 0

	UCHAR_T	*AES_OUT;
	UCHAR_T	AES_OUT_LEN;
	OPERATE_RET ret;

	// AES user input data
	UCHAR_T	 AES_PLAIN[64] = 
	{
		0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xAA,0xBB,0xCC,0xDD,0xEE,0xFF,
		0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xAA,0xBB,0xCC,0xDD,0xEE,0xFF,
		0x89,0xF8,0x65,0x22,0x51,0x3C,0x05,0x3D,0x4A,0xB3,0xBE,0xFD,0x8D,0x9E,0x19,0xD0,
		0x89,0xF8,0x65,0x22,0x51,0x3C,0x05,0x3D,0x4A,0xB3,0xBE,0xFD,0x8D,0x9E,0x19,0xD0
	};
	UCHAR_T	AES_Key[32]   = 
	{
		0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
		0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F
	};
	UCHAR_T	AES_IV[16]	  = 
	{
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
	};


	PR_DEBUG_RAW("Aes192CbcEncodeTestDemo");
	aes192_cbc_encode(AES_PLAIN, 64, AES_Key, AES_IV, &AES_OUT, &AES_OUT_LEN);	
	PR_DEBUG_RAW("ret =  %d:  ", ret);
	PR_DEBUG_RAW("encode result %d:", AES_OUT_LEN);
	for(UCHAR_T i = 0; i < AES_OUT_LEN; i++) {	
		PR_DEBUG_RAW("%02x", AES_OUT[i]);
	}
	PR_DEBUG_RAW("\r\n");	
	Free(AES_OUT);

	PR_DEBUG_RAW("AES_PLAIN  %d:" );		//发现源数据 在加密后被修改掉了。xl 20181013
	for(UCHAR_T i = 0; i < 64; i++) {	
		PR_DEBUG_RAW("%02x", AES_PLAIN[i]);
	}
	PR_DEBUG_RAW("\r\n");	
#endif

	return;
}

/***********************************************************
*  Function: SEChip_I2C_Init 安全芯片 I2C初始化
*  Input: 	none
*  Output: 	none
*  Return:  none
***********************************************************/
VOID SEChip_I2C_Init(VOID)
{
	//i2c as Master 
	i2c_init(&i2cEncrypt, Z32HUA_I2C_MTR_SDA , Z32HUA_I2C_MTR_SCL);
	i2c_frequency(&i2cEncrypt, Z32HUA_I2C_BUS_CLK); 
	SystemSleep(5);

	bpoi_init();		//BPOI协议参数初始化
	PR_NOTICE("SE i2c init ok");
	return;
}

#if 0
/***********************************************************
*  Function: SE_EncyptDemoTest 
*  Input: 	none
*  Output: 	none
*  Return:  none
***********************************************************/
VOID SE_EncyptDemoTest(VOID)
{

	INT_T ret;
	UCHAR_T id[16];
	UCHAR_T random[4];
	UCHAR_T iv[16];
	UCHAR_T encyptData[8] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
	InternalAuthInfo_s internalAuthInfo;
	ExternalAuthInfo_s externalAuthInfo;
	static BOOL_T wirteAuthDataOnce = FALSE;
	UCHAR_T input_data[64];
	
	SEChip_I2C_Init(); //加密芯片 I2C 接口初始化
	PR_NOTICE("i2cEncyptDemoTest ETNER");
	
	while(1)
	{
		
#if 0
		Aes192CbcEncodeTestDemo();
		SystemSleep(1000);
		Aes192CbcDecodeTestDemo();
#endif

		//选择应用
		ret = SE_SelectDirByName();
		SystemSleep(5000);
		
		
#if 1
		//写入模块子密钥		
		ret = SE_InsertKey(&SEKeyInfo);	
		SystemSleep(5000);
	
		//验证密钥
		ret = SE_VerifyKey(encyptData);
		SystemSleep(5000);

		//封闭权限		
		ret = SE_CloseAuth();
		SystemSleep(5000);
#endif


#if 1
		//获取芯片ID
		ret = SE_GetSEChipID(id);
		SystemSleep(5000);

		//获取随机数
		ret = SE_GetChallenge(random);	
		SystemSleep(5000);
		
		//初始化认证
		ret = SE_InitAuthenticate((UCHAR_T *)&internalAuthInfo);
		SystemSleep(5000);
		
		//下行外部认证 
		memcpy(input_data, "\x05\x11\x12\x13\x14\xEB\x10\xF9\x51\x49\x7C\xD3\xDE", 13);		//随机数取： 0x11, 0x12, 0x13, 0x14
		ret = SE_ExternalAuthenticate(input_data);
		SystemSleep(5000);		
#endif


#if 1
		//计算临时密钥
		UCHAR_T keyTemp[24];
		UCHAR_T keyTempLen;
		memcpy(keyTemp, internalAuthInfo.randomA, 4);	//随机数A
		memcpy(keyTemp + 4, "\x11\x12\x13\x14", 4);		//随机数B	
		memcpy(keyTemp + 8, SEKeyInfo.factoryCode, 8);	//涂鸦厂商代码
		keyTemp[16] = 	SEKeyInfo.algorithmCode;		//算法标识
		keyTempLen = 17;
		memset(iv, 0, 16);
		ThreeDes_CBC_Encode(SEKeyInfo.SEkey, iv, keyTemp, &keyTempLen);
		PR_DEBUG_RAW("temp key len=%d is:", keyTempLen);
		for(UCHAR_T i = 0; i < keyTempLen; i++) {	
			PR_DEBUG_RAW("%02x", keyTemp[i]);
		}
		PR_DEBUG_RAW("\r\n");			
			
		//AES加密授权数据
		UCHAR_T* output_data;
		UCHAR_T output_data_len;
		memset(input_data, 0x22, 64);	//64个0x22
		memset(iv, 0, 16);
		aes192_cbc_encode(input_data, 64, keyTemp, iv, &output_data, &output_data_len);		//AES-192加密
		PR_DEBUG_RAW("AES192 encode len=%d data:", output_data_len);
		for(UCHAR_T i = 0; i < output_data_len; i++) {	
			PR_DEBUG_RAW("%02x", output_data[i]);
		}
		PR_DEBUG_RAW("\r\n");		
		
		//写入授权数据
		if(wirteAuthDataOnce == FALSE) {
			wirteAuthDataOnce = TRUE;				//只写一次	
			SE_WriteAuthData(output_data, 64);
		}
		Free(output_data);
		SystemSleep(5000);
#endif		



#if 1
		//读出授权数据
		UCHAR_T* decode_data;
		UCHAR_T decode_data_len;
		memset(input_data, 0, 64);	//读出缓冲区 清0
		ret = SE_ReadAuthData(input_data, &decode_data_len);
		if(ret == 0) {		
			aes192_cbc_decode(input_data, decode_data_len, keyTemp, iv, &decode_data, &decode_data_len); //AES192-CBC解密
			PR_DEBUG_RAW("AES192 decode len=%d data:", decode_data_len);
			for(UCHAR_T i = 0; i < decode_data_len; i++) {	
				PR_DEBUG_RAW("%02x", decode_data[i]);
			}
			PR_DEBUG_RAW("\r\n");		
		}
		Free(decode_data);
		SystemSleep(5000);	
#endif


	}

	PR_NOTICE("i2cEncyptDemoTest EXIT");
	return;
}


/***********************************************************
*  Function: CalcXOR 2段数据异或计算
*  Input: 	dst - 数据目的地址
*    		src - 数据源地址
*    		len - 输入数据
*  Output: 	dst - 结果存放地址
*  Return:  none
***********************************************************/
VOID CalcXOR( INOUT UINT8_T* dst, IN UINT8_T* src, IN UINT8_T len)
{
	UINT8_T i;
	for( i = 0; i < len; i++ )
	{
		dst[i] ^= src[i];
	}
	return;
}

/***********************************************************
*  Function: MacCalculate 加密芯片安全认证码计算
*  Input: 	key - 24字节密钥
*    		initValue - IV值  	(8字节初始向量)
*    		inData - 输入数据
*     		inLen - 输入数据长度
*  Output: 	mac - 输出8字节MAC值
*  Return:  none
***********************************************************/
VOID MacCalculate(IN UINT8_T* key, IN UINT8_T* initValue, IN UINT8_T* inData,  IN UINT8_T inLen, OUT UINT8_T* mac)
{
	UINT8_T pakgeNum;		//输入数据以8字节为一包，不足一包用PADDING补齐
	UINT8_T total_len;		//输入数据总长
	UINT8_T i;
	UINT8_T j;
	UINT8_T DesInput[8];
	UCHAR_T *FillData;

	FillData = (UCHAR_T *)Malloc(256);			//输入数据最长256字节
	if(FillData == NULL) {
		PR_ERR("MacCalculate malloc err");
		return;
	}
	
	total_len = inLen;
	memcpy(FillData, inData, total_len);
	pakgeNum = total_len/8;
	if((total_len % 8) != 0 ) {
		memcpy(FillData + total_len, "\x80\x00\x00\x00\x00\x00\x00\x00", 8);//PADDING
		pakgeNum++;
	}

	PR_DEBUG_RAW("pakgeNum = %d, FillData:", pakgeNum);
	total_len = 8*pakgeNum;
	for( i = 0; i < total_len; i++) {	
		PR_DEBUG_RAW("%02x", FillData[i]);
	}
	PR_DEBUG_RAW("\r\n");
	
	memcpy(DesInput, initValue, 8);
	for(i = 0; i < pakgeNum; i++) 
	{
		CalcXOR(DesInput, FillData + 8*i,  8);	//异或值
		DES(DesInput, key, 1);					//KEY1加密
		DES(DesInput, key + 8, 0);				//KEY2解密
		DES(DesInput, key + 16, 1); 			//KEY3
	}	
	memcpy(mac, DesInput,8);					//获得8字节MAC
	
	Free(FillData);
	return;
} 


/***********************************************************
*  Function: ThreeDes_CBC_Encode 3DES-CBC加密
*  Input: 	key - 24字节密钥
*    		initValue - IV值  	(8字节初始向量)
*    		inOutData - 输入数据地址
*     		inOutLen -  输入数据长度地址 （长度要为 8的倍数）
*  Output: 	inOutData - 输出加密数据        inOutLen - 输出数据长度
*  Return:  none
***********************************************************/
VOID ThreeDes_CBC_Encode(IN UINT8_T* key, IN UINT8_T* initValue, INOUT UINT8_T* inOutData, INOUT UINT8_T* inOutLen)
{
	UINT8_T pakgeNum;		//输入数据以8字节为一包，不足一包用PADDING补齐
	UINT8_T total_len;		//输入数据总长
	UINT8_T i;
	UINT8_T j;
	UINT8_T DesInput[8];
	UCHAR_T *FillData;

	FillData = (UCHAR_T *)Malloc(256);		//输入数据最长256字节
	if(FillData == NULL) {
		PR_ERR("3DES malloc err");
		return;
	}
	
	total_len = *inOutLen;
	memcpy(FillData, inOutData, total_len);
	pakgeNum = total_len/8;
	if((total_len % 8) != 0 ) {
		memcpy(FillData + total_len, "\x80\x00\x00\x00\x00\x00\x00\x00", 8);//PADDING
		pakgeNum++;
	}

	PR_DEBUG_RAW("pakgeNum = %d, FillData:", pakgeNum);
	total_len = 8*pakgeNum;
	for( i = 0; i < total_len; i++) {	
		PR_DEBUG_RAW("%02x", FillData[i]);
	}
	PR_DEBUG_RAW("\r\n");
	
	memcpy(DesInput, initValue, 8);
	for(i = 0; i < pakgeNum; i++) 
	{
		CalcXOR(DesInput, FillData + 8*i,  8);	//异或值
		DES(DesInput, key, 1);					//KEY1加密
		DES(DesInput, key + 8, 0);				//KEY2解密
		DES(DesInput, key + 16, 1); 			//KEY3加密
		memcpy(inOutData + 8*i, DesInput, 8);	//加密结果
	}	
	*inOutLen = total_len;		//把加密长度返回	

	Free(FillData);
	return;
} 

#endif

#endif

/***********************************************************
*************************end of file************************
***********************************************************/

