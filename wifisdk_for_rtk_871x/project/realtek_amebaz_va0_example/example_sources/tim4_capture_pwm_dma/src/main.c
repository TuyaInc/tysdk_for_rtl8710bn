/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */

#include "ameba_soc.h"
#include "main.h"

#define PWM_PERIOD	9

GDMA_InitTypeDef GdmaInitStruct;

u32 PWM_pattern[20] = {
	0x3000001,
	0x3000002,
	0x3000003,
	0x3000004,
	0x3000005,
	0x3000006,
	0x3000007,
	0x3000008,
	0x3000009,
	0x3000009,
	
	0x3000009,
	0x3000009,
	0x3000008,
	0x3000007,
	0x3000006,
	0x3000005,
	0x3000004,
	0x3000003,
	0x3000002,
	0x3000001,
};

u32 rx_data[20] = {0};
u32 capture_pattern[16] = {
	0x100018f,
	0x1000257,
	0x100031f,
	0x10003e7,
	0x10004af,
	0x1000577,
	0x100063f,
	0x1000707,
	0x1002647,
	0x100063f,
	0x1000577,
	0x10004af,
	0x10003e7,
	0x100031f,
	0x1000257,
	0x100018f,
};

void tim5_gen_pwm_dma()
{
	int pwm_chan = 0;
	RTIM_TimeBaseInitTypeDef TIM_InitStruct_temp;
	TIM_CCInitTypeDef TIM_CCInitStruct;
	GDMA_InitTypeDef TIMGdmaInitStruct;

	RTIM_TimeBaseStructInit(&TIM_InitStruct_temp);
	TIM_InitStruct_temp.TIM_Idx = 5;
	TIM_InitStruct_temp.TIM_Prescaler = 199;
	TIM_InitStruct_temp.TIM_Period = PWM_PERIOD;
	RTIM_TimeBaseInit(TIM5, &TIM_InitStruct_temp, TIMER5_IRQ, NULL, 0);

	RTIM_CCStructInit(&TIM_CCInitStruct);
	RTIM_CCxInit(TIM5, &TIM_CCInitStruct, pwm_chan);
	RTIM_CCRxSet(TIM5, 1, pwm_chan);
	RTIM_CCxCmd(TIM5, pwm_chan, TIM_CCx_Enable);

	Pinmux_Config(_PA_23, PINMUX_FUNCTION_PWM);
	RTIM_Cmd(TIM5, ENABLE);

	/* enable pwm channel DMA*/
	InterruptDis(TIMER5_IRQ);
	RTIM_DMACmd(TIM5, TIM_DMA_CCx[pwm_chan], ENABLE);
	RTIM_TXGDMA_Init(pwm_chan, &TIMGdmaInitStruct, NULL, NULL, (u8*)PWM_pattern, 20*4);
}

void tim4_capture_dma_ISR(u32 data)
{
	GDMA_InitTypeDef *GdmaInitStruct = (GDMA_InitTypeDef *)data;
	GDMA_ClearINT(GdmaInitStruct->GDMA_Index, GdmaInitStruct->GDMA_ChNum);
        
        DBG_8195A("dma done\n");

	u32 i = 0;
	for(; i < 20; i++){
		if(rx_data[i] == capture_pattern[0])
			break;
	}

	if(i != 20){
		u32 j = 0;
		for(;(i < 20) && (j < 16); i++, j++){
			if(rx_data[i] != capture_pattern[j]){
				DBG_8195A("Fail\n");
				return;
			}
		}
		if(i == 20 || j == 16)
			DBG_8195A("Success\n");
	}else{
		DBG_8195A("Fail\n");
	}
}

void tim4_capture_pulse_width_dma()
{
	RTIM_TimeBaseInitTypeDef  TIM_InitStruct_temp;
	TIM_CCInitTypeDef TIM_CCInitStruct;

	RTIM_TimeBaseStructInit(&TIM_InitStruct_temp);
	TIM_InitStruct_temp.TIM_Idx = 4;	
	RTIM_TimeBaseInit(TIM4, &TIM_InitStruct_temp, TIMx_irq[4], NULL, 0);
	
	RTIM_CCStructInit(&TIM_CCInitStruct);
	TIM_CCInitStruct.TIM_ICPulseMode = TIM_CCMode_PulseWidth;
	RTIM_CCxInit(TIM4, &TIM_CCInitStruct, 0);
	
	RTIM_CCxCmd(TIM4, 0, TIM_CCx_Enable);
	RTIM_Cmd(TIM4, ENABLE);
	Pinmux_Config(_PA_18, PINMUX_FUNCTION_TIMINPUT);
	PAD_PullCtrl(_PA_18, GPIO_PuPd_UP);

	/* enable capture dma*/
	RTIM_DMACmd(TIM4, TIM_DMA_CC0, ENABLE);
	RTIM_RXGDMA_Init(4, TIM_Channel_0, &GdmaInitStruct, &GdmaInitStruct, (IRQ_FUN)tim4_capture_dma_ISR, (u8*)rx_data, 20 * 4);
	
}

void tim4_capture_pwm_dma(void)
{
	/* generate breathing light pwm wave*/
	tim5_gen_pwm_dma();

	/* capture pwm pulse width and dma to memory*/
	tim4_capture_pulse_width_dma();

	vTaskDelete(NULL);
}

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void main(void)
{
	if(xTaskCreate( (TaskFunction_t)tim4_capture_pwm_dma, "TIM4 CAPTURE DMA DEMO", (2048/4), NULL, (tskIDLE_PRIORITY + 1), NULL)!= pdPASS) {
			DBG_8195A("Cannot create tim4 capture dma demo task\n\r");
	}

	vTaskStartScheduler();

	while(1);
}


