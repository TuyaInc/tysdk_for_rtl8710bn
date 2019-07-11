/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */

//#include "device.h"
//#include <sys_api.h>

#include "ameba_soc.h"
#include "main.h"

#define PWM_PERIOD	40000000/32768

void log_uart_rx_as_pwm(void)
{
	int pwm_chan = 4;
	RTIM_TimeBaseInitTypeDef TIM_InitStruct_temp;
	TIM_CCInitTypeDef TIM_CCInitStruct;

	RTIM_TimeBaseStructInit(&TIM_InitStruct_temp);
	TIM_InitStruct_temp.TIM_Idx = 5;
	TIM_InitStruct_temp.TIM_Period = PWM_PERIOD;
	RTIM_TimeBaseInit(TIM5, &TIM_InitStruct_temp, TIMER5_IRQ, NULL, 0);

	RTIM_CCStructInit(&TIM_CCInitStruct);
	RTIM_CCxInit(TIM5, &TIM_CCInitStruct, pwm_chan);
	RTIM_CCRxSet(TIM5, PWM_PERIOD / 2 + 1, pwm_chan);
	RTIM_CCxCmd(TIM5, pwm_chan, TIM_CCx_Enable);

	Pinmux_Config(_PA_29, PINMUX_FUNCTION_PWM);
	RTIM_Cmd(TIM5, ENABLE);

	vTaskDelete(NULL);
}

void main(void)
{
	if(xTaskCreate( (TaskFunction_t)log_uart_rx_as_pwm, "Log_uart_rx_as_pwm DEMO", (2048/4), NULL, (tskIDLE_PRIORITY + 1), NULL)!= pdPASS) {
			DBG_8195A("Cannot create log_uart_rx_as_pwm demo task\n\r");
	}

	vTaskStartScheduler();

	while(1);
}



