/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2015 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */

#include "device.h"
#include "gpio_api.h"   // mbed
#include "gpio_irq_api.h"   // mbed
#include "sleep_ex_api.h"
#include "sys_api.h"
#include "diag.h"
#include "main.h"

void bor_intr_Handler(void)
{
	DBG_8195A("bor_intr_handler!!!\n");
}

void bor2_test(void)
{
	// mbed BOR2 test
	
	BOR2_ModeSet(BOR2_INTR);
	BOR2_INTRegister((void *)bor_intr_Handler);
	BOR2_INTCmd(ENABLE);
	
	DBG_8195A("Supply 2.6V-3.0V voltage!!!\n");
	
	vTaskDelete(NULL);
}

void main(void)
{
	// create demo Task
	if(xTaskCreate( (TaskFunction_t)bor2_test, "BOR2 DEMO", (2048/4), NULL, (tskIDLE_PRIORITY + 1), NULL)!= pdPASS) {
		DBG_8195A("Cannot create bor2 demo task\n\r");
		goto end_demo;
	}

#if defined(CONFIG_KERNEL) && !TASK_SCHEDULER_DISABLED
	#ifdef PLATFORM_FREERTOS
	vTaskStartScheduler();
	#endif
#else
	#error !!!Need FREERTOS!!!
#endif

end_demo:	
	while(1);
}