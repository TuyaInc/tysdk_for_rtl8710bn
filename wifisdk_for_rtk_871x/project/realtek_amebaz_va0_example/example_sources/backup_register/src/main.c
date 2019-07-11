/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */

#include "device.h"
#include "analogin_api.h"
#include <sys_api.h>

#define TEST_BIT	BIT(0)

void backup_register(void)
{
	u32 value_bk0 = BKUP_Read(BKUP_REG0);
	u32 value_bk3 = BKUP_Read(BKUP_REG3);

	if(value_bk0 & BIT_CPU_RESET_HAPPEN){		//system reboot
		if(value_bk3 & TEST_BIT){
			DBG_8195A("Backup Register function test ok.\n");
			BKUP_Clear(BKUP_REG3, TEST_BIT);
		}
	}
	else{
		BKUP_Set(BKUP_REG3, TEST_BIT);
		
		DBG_8195A("\nRebooting ...\n");
		
		NVIC_SystemReset();
	}

	vTaskDelete(NULL);
}

void main(void)
{
	if(xTaskCreate( (TaskFunction_t)backup_register, "BACKUP REG DEMO", (2048/4), NULL, (tskIDLE_PRIORITY + 1), NULL)!= pdPASS) {
			DBG_8195A("Cannot create Backup register demo task\n\r");
	}

	vTaskStartScheduler();

	while(1);
}



