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

/*
 * OFFSET:   value of measuring at 0.000v, value(0.000v)
 * GAIN_DIV: value(1.000v)-value(0.000v) or value(2.000v)-value(1.000v) or value(3.000v)-value(2.000v)
 *
 * MSB 12bit of value is valid, need to truncate LSB 4bit (0xABCD -> 0xABC). OFFSET and GAIN_DIV are truncated values.
 */

/* Vbat channel */
#define OFFSET		0x496							
#define GAIN_DIV		0xBA
#define AD2MV(ad,offset,gain) (((ad >> 4) -offset) * 1000 / gain)	

void adc_delay(void)
{
    	int i;
	for(i=0;i<1600000;i++)
		asm(" nop");
}

void adc_vbat_en(void)
{
	uint16_t adc_read = 0;
	int32_t voltage;
	analogin_t   adc_vbat;

	analogin_init(&adc_vbat, AD_2);

	DBG_8195A("ADC:offset = 0x%x, gain = 0x%x\n", OFFSET, GAIN_DIV);

	for (;;){
		adc_read = analogin_read_u16(&adc_vbat);

		voltage = AD2MV(adc_read, OFFSET, GAIN_DIV);

		DBG_8195A("ADC_Vbat: 0x%x = %d mv\n", adc_read, voltage); 
		adc_delay();
	}
	analogin_deinit(&adc_vbat);

	vTaskDelete(NULL);
}

VOID main (VOID)
{
	if(xTaskCreate( (TaskFunction_t)adc_vbat_en, "ADC VBAT DEMO", (2048/4), NULL, (tskIDLE_PRIORITY + 1), NULL)!= pdPASS) {
			DBG_8195A("Cannot create ADC Vbat demo task\n\r");
	}

	vTaskStartScheduler();

	while(1);
}


