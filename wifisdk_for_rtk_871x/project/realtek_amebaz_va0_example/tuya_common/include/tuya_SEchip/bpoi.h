/** ----------------------------------------------------------------------------
 *         Nationz Technology Software Support  -  NATIONZ  -
 * -----------------------------------------------------------------------------
 * Copyright (c) 2013 - 2018, Nationz Corporation  All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaiimer below.
 * 
 * - Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the disclaimer below in the documentation and/or
 * other materials provided with the distribution. 
 * 
 * Nationz's name may not be used to endorse or promote products derived from
 * this software without specific prior written permission. 
 * 
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY NATIONZ "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * -----------------------------------------------------------------------------
 */

/** ****************************************************************************
 * @copyright                  Nationz Co.,Ltd 
 *               Copyright (c) 2013 - 2018 All Rights Reserved 
 *******************************************************************************
 * @file     bpoi.h
 * @author   nationz
 * @date     2017-6-14 09:03:00
 * @version  v1.0
 * @brief    bpoi Header File
 ******************************************************************************/
#ifndef __BPOI_H__
#define __BPOI_H__

#include "tuya_cloud_types.h"	//#include "type.h"


//*****************************************************************************
//
//Macro definition for function controll
//
//*****************************************************************************
#define MALLOC_EN                    1

//*****************************************************************************
//
//Macro definition
//
//*****************************************************************************
#define NULL                         0 

#define SE_I2C_MAX_SIZE_B            255

//#define BPOI_I2C_RETRY_CNT           5
//#define BPOI_MAX_VLD_SIZE            (SE_I2C_MAX_SIZE_B - 3)
//#define BPOI_OP_INTERVAL             5
//#define BPOI_CR_INTERVAL             5
//#define BPOI_S_BLK_TIME              10
//#define BPOI_NAK_INTERVAL            50
//#define BPOI_NAK_TIMEOUT             50
//#define  BPOI_RST_HOLD_TIME          5

#define BPOI_STS_BLK_LEN             3
#define BPOI_RACK_BLK_LEN            3
  
#define BPOI_SND_WR_CMD              0
#define BPOI_RCV_RD_STS              1
                                     
#define BPOI_I_0_BLK                 0        
#define BPOI_I_1_BLK                 1
#define BPOI_I_ATR_BLK               2
#define BPOI_RACK_L_BLK              3
#define BPOI_RACK_D_BLK              4
#define BPOI_RST_BLK                 5

#define BPOI_IND_ATR                 0
#define BPOI_IND_APDU                1
#define BPOI_IND_RST                 2
   
#define BPOI_ERR_NONE                0
#define BPOI_ERR_NAK                 -1
#define BPOI_CMD_PHASE_ERR           -2
#define BPOI_CMD_RERTRY_ERR          -3
#define BPOI_CMD_EXEC_ERR            -4
#define BPOI_MEM_FAILURE             -5
#define BPOI_CMD_TYPE_ERR            -6
#define BPOI_RST_FAILURE             -7

//*****************************************************************************
//
//  BPOI Frame Format
//
//  ------------------------------------
//  | Field:  | PCB | LEN | DATA | XOR |
//  ------------------------------------
//  | Length: |  1  |  1  | LEN  |  1  |
//  ------------------------------------
//
//*****************************************************************************


//*****************************************************************************
//
// Protocol control byte description
//
//*****************************************************************************
#define BPOI_I_BLOCK       (0 << 6)
#define BPOI_I_LINK_BLOCK  (1 << 5)
#define BPOI_I_LAST_BLOCK  (0 << 5)
#define BPOI_I_ATR_BLOCK   (1 << 4)
#define BPOI_RST_BLOCK     (0x1F)

#define IS_I_BLOCK(n)      (((n >> 6) & 0x03) == 0x00)
#define IS_LINK_BLOCK(n)   (((n >> 5) & 0x01) == 0x01)
#define IS_ATR_BLOCK(n)    (((n >> 4) & 0x03) == 0x01)     

#define BPOI_L_BLOCK       (1 << 6)
#define IS_L_BLOCK(n)      (((n >> 6) & 0x03) == 0x01)

#define BPOI_R_BLOCK       (2 << 6)
#define IS_R_BLOCK(n)      (((n >> 6) & 0x03) == 0x02)
#define BPOI_R_ACK          0
#define BPOI_R_NACK         1
#define IS_R_ACK(n)        ((n & 0x01) == 0x00)

#define BPOI_S_BLOCK       (3 << 6)
#define IS_S_BLOCK(n)      (((n >> 6) & 0x03) == 0x03)     
   
//*****************************************************************************
//
//struct definition
//
//*****************************************************************************
typedef struct
{
    //I2C_WRTIE callback
    int32_t (*i2c_write)(void *pvInBuffer, uint32_t ui32NumBytes);

    //I2C_READ callback
    int32_t (*i2c_read)(void *pvOutBuffer, uint32_t ui32NumBytes);
	
    //delay_ms callback
    void (*delay_ms)(uint32_t ui32MilliSeconds);

    //indicating the max size per i2c send 
    uint16_t ui16I2cTxMaxSize;
 
    //indicating the max size per i2c receive 
    uint16_t ui16I2cRxMaxSize;
    
    //indicating the intput cmd type whither is apdu cmd or atr 
    uint8_t ui8CmdType;

    //the retry cnt for fail i2c operation 
    uint8_t ui8RetryCnt;
	
    //the extend time of S-BLOCK
    uint8_t ui8ExtendTime;
	
    //the timeout of the nak for i2c, , the uint is ms
    uint8_t ui8NakTimeout;
	
    //the time of the i2c operation, the uint is ms
    uint8_t ui8OpInterval;

    //the interval between bpoi cmd and rsp
    uint8_t ui8CRInterval;
	
    //the hold time after sending reset block
    uint8_t ui8RstTime;
}bpoi_ctrl;

//*****************************************************************************
//
//variable statemnt
//
//*****************************************************************************
extern bpoi_ctrl *g_psBpoiCtrl;

//*****************************************************************************
//
// External function statement for invoke
//
//*****************************************************************************
extern void init_bpoi_parameter(bpoi_ctrl **psBpoiCtrl);

//*****************************************************************************
//
// External function statement
//
//*****************************************************************************
void bpoi_init(void);
int32_t bpoi_apdu_cmd_process(void *pvInput, \
                                uint32_t ui32InputLen, \
                                void *pvOutput, \
                                uint32_t *pui32RcvLen, \
                                bpoi_ctrl *psBpoiCtrl);

#endif/*__BPOI_H__*/
