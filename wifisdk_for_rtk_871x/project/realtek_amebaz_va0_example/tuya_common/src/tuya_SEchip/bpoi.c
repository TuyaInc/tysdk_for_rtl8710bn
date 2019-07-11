/** ****************************************************************************
 * @copyright                  Nationz Co.,Ltd 
 *               Copyright (c) 2013 - 2018 All Rights Reserved 
 *******************************************************************************
 * @file     bpoi.c
 * @author   nationz
 * @date     2017-9-20 09:00:00
 * @version  v1.0
 * @brief    bpoi File
 * @defgroup 
 * @{
 ******************************************************************************/
#include <string.h>
#include <stdlib.h>
#include "bpoi.h"

#include "tuya_cloud_types.h"	//#include "type.h"
#include "mem_pool.h"


bpoi_ctrl *g_psBpoiCtrl = NULL;

/*******************************************************************************
* Function Name: uint8_t lrc(uint8_t *pbuff, int16_t len)
* Description:   calculate the lrc
* Input:         uint8_t *pbuff
*                int16_t len
* Output:        none
* Return:        uint8_t lrc: the lrc is calculated
*******************************************************************************/
static uint8_t lrc_calculate(uint8_t *pui8Buff, uint16_t ui16Len)
{
    uint8_t ui8Lrc = 0; 
    while(ui16Len--)
    {
        ui8Lrc ^= *pui8Buff++;
    }
	
    return ui8Lrc;
}

/*******************************************************************************
* Function Name: void bpoi_init(void)
* Description:   init bpoi parameter
* Input:         none
* Output:        none
* Return:        none
*******************************************************************************/
void bpoi_init(void)
{
    init_bpoi_parameter(&g_psBpoiCtrl);
}

/*******************************************************************************
* Function Name: int32_t bpoi_send_block(uint8_t *pui8TxBuff, \
*                                           uint32_t ui32TxLen, \
*                                           uint8_t *pui8RxBuff, \
*                                           uint32_t ui32RxLen, \
*                                           uint8_t ui8BlkType, \
*                                           bpoi_ctrl *psBpoiCtrl)
* Description:   send one BLOCK  
* Input:         uint8_t *pui8TxBuff
*                uint32_t ui32TxLen
*                uint8_t *pui8RxBuff
*                uint32_t ui32RxLen
*                uint8_t ui8BlkType
*                bpoi_ctrl *psBpoiCtrl
* Output:        uint8_t *pui8RxBuff
* Return:        int32_t i32Ret
*******************************************************************************/
static int32_t bpoi_send_block(uint8_t *pui8TxBuff, \
                                uint32_t ui32TxLen, \
                                uint8_t *pui8RxBuff, \
                                uint32_t ui32RxLen, \
                                uint8_t ui8BlkType, \
                                bpoi_ctrl *psBpoiCtrl)
{
    int32_t i32Ret = BPOI_ERR_NONE;
    uint8_t ui8Retry = 0;
    uint8_t ui8Stage = BPOI_SND_WR_CMD;

    while(1) 
    {
        if(ui8Stage == BPOI_SND_WR_CMD)
        {
            if (BPOI_ERR_NONE == psBpoiCtrl->i2c_write(pui8TxBuff, ui32TxLen)) 
            {
                if(ui8BlkType == BPOI_RST_BLK)
                {
                    psBpoiCtrl->delay_ms(psBpoiCtrl->ui8RstTime);
                    return BPOI_ERR_NONE;
                }
				
                //ui8Retry = 0;
                ui8Stage = BPOI_RCV_RD_STS;
                psBpoiCtrl->delay_ms(psBpoiCtrl->ui8OpInterval);
            } 
            else 
            {				
                ui8Retry++;
                psBpoiCtrl->delay_ms(psBpoiCtrl->ui8NakTimeout);
            }
        }
        else if(ui8Stage == BPOI_RCV_RD_STS)
        {			
            if(BPOI_ERR_NONE != psBpoiCtrl->i2c_read(pui8RxBuff, ui32RxLen)) 
            {
                ui8Retry++;
                ui8Stage = BPOI_SND_WR_CMD;
                psBpoiCtrl->delay_ms(psBpoiCtrl->ui8NakTimeout);
            } 
            else 
            {
                psBpoiCtrl->delay_ms(psBpoiCtrl->ui8OpInterval);
							
                if(pui8RxBuff[ui32RxLen - 1] == lrc_calculate(pui8RxBuff, (ui32RxLen - 1)))
                {
                    /** L Block */
                    if(IS_L_BLOCK(pui8RxBuff[0]))
                    {
                        if((ui8BlkType == BPOI_I_0_BLK) || (ui8BlkType == BPOI_I_ATR_BLK) || (ui8BlkType == BPOI_RACK_L_BLK))
                        {
                            break;
                        }
                        else
                        {
                            ui8Retry++;
                        }
                    }
                    /** I Block */
                    else if(IS_I_BLOCK(pui8RxBuff[0])) 
                    {
                        if(ui8BlkType == BPOI_RACK_D_BLK)
                        {
                            break;
                        }
                        else
                        {
                            ui8Retry++;
                        }									
                    }
                    /** S Block */
                    else if(IS_S_BLOCK(pui8RxBuff[0])) 
                    {
                        //ui8Retry = 0;
                        psBpoiCtrl->delay_ms(psBpoiCtrl->ui8ExtendTime);
                    }		
                    /** R Block */
                    else if(IS_R_BLOCK(pui8RxBuff[0])) 
                    {
                        if(!IS_R_ACK(pui8RxBuff[0]))
                        {
                            ui8Retry++;
                            ui8Stage = BPOI_SND_WR_CMD;
                        }
                        else
                        {
                            if(ui8BlkType == BPOI_I_1_BLK)
                            {
                                break;
                            }
                            else
                            {
                                ui8Retry++;
                            }
                        }
                    }	
                    else
                    {
                        ui8Retry++;
                    }	
                }
                else
                {
                    ui8Retry++;
                }
            }
        }
        else
        {
            i32Ret = BPOI_CMD_PHASE_ERR;
            break;
        }
				
        if(ui8Retry >= (psBpoiCtrl->ui8RetryCnt)*3)
        {
            i32Ret = BPOI_CMD_RERTRY_ERR;
            break;
        }
    }

    return i32Ret;
}

/*******************************************************************************
* Function Name: int32_t bpoi_apdu_cmd_process(void *pvInput, \
*                                               uint32_t ui32InputLen, \
*                                               void *pvOutput, \
*                                               uint32_t *pui32RcvLen, \
*                                               bpoi_ctrl *psBpoiCtrl)
* Description:   send apdu cmd and receive rsp data
* Input:         void *pvInput,         
*                uint32_t ui32InputLen
*                void *pvOutput
*                uint32_t *pui32RcvLen
*                bpoi_ctrl *psBpoiCtrl
* Output:        uint32_t *pui32RcvLen
* Return:        int32_t i32Ret
*******************************************************************************/
int32_t bpoi_apdu_cmd_process(void *pvInput, \
                                uint32_t ui32InputLen, \
                                void *pvOutput, \
                                uint32_t *pui32RcvLen, \
                                bpoi_ctrl *psBpoiCtrl)
{
    int32_t i32Ret = BPOI_ERR_NONE;
    uint32_t ui32DataLen;
#ifdef MALLOC_EN 
    uint8_t *pui8TxBuff = (uint8_t *)Malloc(psBpoiCtrl->ui16I2cTxMaxSize);
    uint8_t *pui8RxBuff = (uint8_t *)Malloc(psBpoiCtrl->ui16I2cRxMaxSize);
#else
    uint8_t ui8SndBuff[psBpoiCtrl->ui16I2cTxMaxSize];
    uint8_t ui8RcvBuff[psBpoiCtrl->ui16I2cRxMaxSize];
#endif
    uint8_t ui8BlkType;
    uint8_t *pui8SndBuff = pvInput;
    uint8_t *pui8RcvBuff = pvOutput;

#ifdef MALLOC_EN   
    //send apdu cmd
    if(pui8TxBuff == NULL)
    {
        return BPOI_MEM_FAILURE;
    }
		
    if(pui8RxBuff == NULL)
    {
        Free(pui8TxBuff);
        return BPOI_MEM_FAILURE;
    }
    
    while(1)
    {
        if(psBpoiCtrl->ui8CmdType == BPOI_IND_APDU)
        {
            if(ui32InputLen > (psBpoiCtrl->ui16I2cTxMaxSize - 3))
            {
                ui32DataLen = psBpoiCtrl->ui16I2cTxMaxSize - 3;
                //set block Type to I-BLOCK with link
                ui8BlkType = BPOI_I_1_BLK;
                //set PCB byte
                pui8TxBuff[0] = BPOI_I_BLOCK | BPOI_I_LINK_BLOCK;
            }
            else
            {
                ui32DataLen = ui32InputLen;
                //set block Type to I-BLOCK with no link
                ui8BlkType = BPOI_I_0_BLK;
                //set PCB byte
                pui8TxBuff[0] = BPOI_I_BLOCK | BPOI_I_LAST_BLOCK;
            }
				    //set LEN byte
            pui8TxBuff[1] = ui32DataLen;
            //copy valid data
            memcpy((pui8TxBuff + 2), pui8SndBuff, ui32DataLen);
            //set crc byte
            pui8TxBuff[ui32DataLen + 2] = lrc_calculate(pui8TxBuff, ui32DataLen + 2);
        }
        else if(psBpoiCtrl->ui8CmdType == BPOI_IND_ATR)
        {
            ui32DataLen = 0;
            //set block Type to ATR-BLOCK
            ui8BlkType = BPOI_I_ATR_BLK;
            pui8TxBuff[0] = BPOI_I_ATR_BLOCK;
            pui8TxBuff[1] = 0;
            pui8TxBuff[2] = BPOI_I_ATR_BLOCK;
        }
        else if(psBpoiCtrl->ui8CmdType == BPOI_IND_RST)
        {
            ui32DataLen = 0;
            //set block Type to RST-BLOCK
            ui8BlkType = BPOI_RST_BLK;
            pui8TxBuff[0] = BPOI_RST_BLOCK;
            pui8TxBuff[1] = 0;
            pui8TxBuff[2] = BPOI_RST_BLOCK;
        }
        else
        {
            Free(pui8TxBuff);
            Free(pui8RxBuff);
            return BPOI_CMD_TYPE_ERR;
        }
				
        i32Ret = bpoi_send_block(pui8TxBuff, (ui32DataLen + 3), pui8RxBuff, BPOI_STS_BLK_LEN, ui8BlkType, psBpoiCtrl);			
        if(i32Ret == BPOI_ERR_NONE)
        {
            if(ui8BlkType == BPOI_RST_BLK)
            {
                Free(pui8TxBuff);
                Free(pui8RxBuff);
                return BPOI_ERR_NONE;
            }	
		
            if(IS_L_BLOCK(pui8RxBuff[0]))
            {
                ui32DataLen = pui8RxBuff[1];
                break;
            }
						
            ui32InputLen -= ui32DataLen;
            pui8SndBuff += ui32DataLen;
        }
        else
        {
            Free(pui8TxBuff);
            Free(pui8RxBuff);
            return i32Ret;
        }
    }

    psBpoiCtrl->delay_ms(psBpoiCtrl->ui8CRInterval);
		
    //receive rsp data
    *pui32RcvLen = 0;
    ui8BlkType = BPOI_RACK_D_BLK;
    //set PCB type RACK
    pui8TxBuff[0] = BPOI_R_BLOCK | BPOI_R_ACK;
    //set LEN to 0 
    pui8TxBuff[1] = 0;
    //set crc byte
    pui8TxBuff[2] = pui8TxBuff[0];
    while(1)
    {		
        //send RACK BLOCK 
        i32Ret = bpoi_send_block(pui8TxBuff, BPOI_RACK_BLK_LEN, pui8RxBuff, (ui32DataLen + 3), ui8BlkType, psBpoiCtrl);			
        if(i32Ret == BPOI_ERR_NONE)
        {
            if(IS_I_BLOCK(pui8RxBuff[0]) && (ui8BlkType == BPOI_RACK_D_BLK))
            { 
                memcpy(pui8RcvBuff, (pui8RxBuff + 2), pui8RxBuff[1]);
                pui8RcvBuff += ui32DataLen;
                *pui32RcvLen += ui32DataLen;
							
                if(IS_LINK_BLOCK(pui8RxBuff[0]))
                {
                    ui8BlkType = BPOI_RACK_L_BLK;
                    ui32DataLen = 0;
                }
                else
                {
                    break;
                }
            }
            else if(IS_L_BLOCK(pui8RxBuff[0]) && (ui8BlkType == BPOI_RACK_L_BLK))
            {
                ui8BlkType = BPOI_RACK_D_BLK;
                ui32DataLen = pui8RxBuff[1];
            }
        }
        else
        {
            Free(pui8TxBuff);
            Free(pui8RxBuff);
            return i32Ret;
        }			
    }

    Free(pui8TxBuff);
    Free(pui8RxBuff);
    return i32Ret;
#else   
    //send apdu cmd	
    while(1)
    {
        if(psBpoiCtrl->ui8CmdType == BPOI_IND_APDU)
        {
            if(ui32InputLen > (psBpoiCtrl->ui16I2cTxMaxSize - 3))
            {
                ui32DataLen = psBpoiCtrl->ui16I2cTxMaxSize - 3;
                //set block Type to I-BLOCK with link
                ui8BlkType = BPOI_I_1_BLK;
                //set PCB byte
                ui8SndBuff[0] = BPOI_I_BLOCK | BPOI_I_LINK_BLOCK;
            }
            else
            {
                ui32DataLen = ui32InputLen;
                //set block Type to I-BLOCK with no link
                ui8BlkType = BPOI_I_0_BLK;
                //set PCB byte
                ui8SndBuff[0] = BPOI_I_BLOCK | BPOI_I_LAST_BLOCK;
            }
				    //set LEN byte
            ui8SndBuff[1] = ui32DataLen;
            //copy valid data
            memcpy((ui8SndBuff + 2), pui8SndBuff, ui32DataLen);
            //set crc byte
            ui8SndBuff[ui32DataLen + 2] = lrc_calculate(ui8SndBuff, ui32DataLen + 2);
        }
        else if(psBpoiCtrl->ui8CmdType == BPOI_IND_ATR)
        {
            ui32DataLen = 0;
            //set block Type to ATR-BLOCK
            ui8BlkType = BPOI_I_ATR_BLK;
            ui8SndBuff[0] = BPOI_I_ATR_BLOCK;
            ui8SndBuff[1] = 0;
            ui8SndBuff[2] = BPOI_I_ATR_BLOCK;
        }
        else if(psBpoiCtrl->ui8CmdType == BPOI_IND_RST)
        {
            ui32DataLen = 0;
            //set block Type to RST-BLOCK
            ui8BlkType = BPOI_RST_BLK;
            ui8SndBuff[0] = BPOI_RST_BLOCK;
            ui8SndBuff[1] = 0;
            ui8SndBuff[2] = BPOI_RST_BLOCK;
        }
        else
        {
            return BPOI_CMD_TYPE_ERR;
        }
				
        i32Ret = bpoi_send_block(ui8SndBuff, (ui32DataLen + 3), ui8RcvBuff, BPOI_STS_BLK_LEN, ui8BlkType, psBpoiCtrl);			
        if(i32Ret == BPOI_ERR_NONE)
        {		
            if(ui8BlkType == BPOI_RST_BLK)
            {
                return BPOI_ERR_NONE;
            }	
			
            if(IS_L_BLOCK(ui8RcvBuff[0]))
            {
                ui32DataLen = ui8RcvBuff[1];
                break;
            }
						
            ui32InputLen -= ui32DataLen;
            pui8SndBuff += ui32DataLen;
        }
        else
        {
            return i32Ret;
        }
    }

    psBpoiCtrl->delay_ms(psBpoiCtrl->ui8CRInterval);
		
    //receive rsp data
    *pui32RcvLen = 0;
    ui8BlkType = BPOI_RACK_D_BLK;
    //set PCB type RACK
    ui8SndBuff[0] = BPOI_R_BLOCK | BPOI_R_ACK;
    //set LEN to 0 
    ui8SndBuff[1] = 0;
    //set crc byte
    ui8SndBuff[2] = ui8SndBuff[0];
    while(1)
    {		
        //send RACK BLOCK 
        i32Ret = bpoi_send_block(ui8SndBuff, BPOI_RACK_BLK_LEN, ui8RcvBuff, (ui32DataLen + 3), ui8BlkType, psBpoiCtrl);			
        if(i32Ret == BPOI_ERR_NONE)
        {
            if(IS_I_BLOCK(ui8RcvBuff[0]) && (ui8BlkType == BPOI_RACK_D_BLK))
            { 
                memcpy(pui8RcvBuff, (ui8RcvBuff + 2), ui8RcvBuff[1]);
                pui8RcvBuff += ui32DataLen;
                *pui32RcvLen += ui32DataLen;
							
                if(IS_LINK_BLOCK(ui8RcvBuff[0]))
                {
                    ui8BlkType = BPOI_RACK_L_BLK;
                    ui32DataLen = 0;
                }
                else
                {
                    break;
                }
            }
            else if(IS_L_BLOCK(ui8RcvBuff[0]) && (ui8BlkType == BPOI_RACK_L_BLK))
            {
                ui8BlkType = BPOI_RACK_D_BLK;
                ui32DataLen = ui8RcvBuff[1];
            }
        }
        else
        {
            return i32Ret;
        }			
    }
		
    return i32Ret;
#endif
}   
