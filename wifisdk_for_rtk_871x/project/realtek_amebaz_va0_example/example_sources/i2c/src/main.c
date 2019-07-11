/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */

#include "PinNames.h"
#include "basic_types.h"
#include "diag.h"

#include "i2c_api.h"
#include "pinmap.h"

/*I2C pin location:
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

//I2C1_SEL S1
#define MBED_I2C_MTR_SDA    PA_23
#define MBED_I2C_MTR_SCL    PA_18

//I2C0_SEL S1
#define MBED_I2C_SLV_SDA    PA_19
#define MBED_I2C_SLV_SCL    PA_22

#define MBED_I2C_SLAVE_ADDR0    0xAA
#define MBED_I2C_BUS_CLK        100000  //hz

#define I2C_DATA_LENGTH         32
uint8_t i2cdatasrc[I2C_DATA_LENGTH];
uint8_t i2cdatadst[I2C_DATA_LENGTH];
SemaphoreHandle_t  I2CRxSema;  // RX begin

i2c_t   i2cmaster;
i2c_t   i2cslave;

void slave_tx_thread(void)
{
	while (1) {
		xSemaphoreTake(I2CRxSema, portMAX_DELAY);
		i2c_slave_write(&i2cslave,&i2cdatasrc[0],I2C_DATA_LENGTH);
		DBG_8195A("slave write done\r\n");
	}

	vTaskDelete(NULL);
}

void i2c_demo(void)
{
	int     send_times = 0;
	int     i2clocalcnt;

	int     result = 0;

	i2c_init(&i2cmaster, MBED_I2C_MTR_SDA ,MBED_I2C_MTR_SCL);
	i2c_frequency(&i2cmaster,MBED_I2C_BUS_CLK);

	i2c_init(&i2cslave, MBED_I2C_SLV_SDA ,MBED_I2C_SLV_SCL);
	i2c_frequency(&i2cslave,MBED_I2C_BUS_CLK);
	i2c_slave_address(&i2cslave, 0, MBED_I2C_SLAVE_ADDR0, 0xFF);
	i2c_slave_mode(&i2cslave, 1);

    for (i2clocalcnt=0; i2clocalcnt < I2C_DATA_LENGTH; i2clocalcnt++){
        i2cdatasrc[i2clocalcnt] = i2clocalcnt+1;
    }

	for (send_times = 0; send_times < 10; send_times++) {
		i2c_write(&i2cmaster, MBED_I2C_SLAVE_ADDR0, &i2cdatasrc[0], I2C_DATA_LENGTH, 1);

		DBG_8195A("master write...\n");

		if (i2c_slave_receive(&i2cslave) == 3) {
			i2c_slave_read(&i2cslave, &i2cdatadst[0], I2C_DATA_LENGTH);
		}

		DBG_8195A("show slave received data\n");
		for (i2clocalcnt = 0; i2clocalcnt < I2C_DATA_LENGTH; i2clocalcnt+=2) {
			DBG_8195A("i2c data: %02x \t %02x\n",i2cdatadst[i2clocalcnt],i2cdatadst[i2clocalcnt+1]);
		}

		// verify result
		result = 1;
		for (i2clocalcnt = 0; i2clocalcnt < I2C_DATA_LENGTH; i2clocalcnt++) {
			if (i2cdatasrc[i2clocalcnt] != i2cdatadst[i2clocalcnt]) {
				result = 0;
				break;
			}
		}
		DBG_8195A("\r\nResult is %s\r\n", (result) ? "success" : "fail");
	}

	// Create semaphore for i2c slave RX 
	I2CRxSema = xSemaphoreCreateBinary();
	if(xTaskCreate((TaskFunction_t)slave_tx_thread, ((const char*)"slave_tx_thread"), (256>>2), (void *const)&i2cslave, tskIDLE_PRIORITY + 1, NULL) != pdPASS) {
		DBG_8195A("xTaskCreate(slave_tx_thread) failed\r\n");
	}

	for (send_times = 0; send_times < 10; send_times++) {
		
		DBG_8195A("master read...\n");
		
		xSemaphoreGive(I2CRxSema);
		i2c_read(&i2cmaster, MBED_I2C_SLAVE_ADDR0, &i2cdatadst[0], I2C_DATA_LENGTH, 1);
		
		DBG_8195A("show master received data\n");
		for (i2clocalcnt = 0; i2clocalcnt < I2C_DATA_LENGTH; i2clocalcnt+=2) {
			DBG_8195A("i2c data: %02x \t %02x\n",i2cdatadst[i2clocalcnt],i2cdatadst[i2clocalcnt+1]);
		}

		// verify result
		result = 1;
		for (i2clocalcnt = 0; i2clocalcnt < I2C_DATA_LENGTH; i2clocalcnt++) {
			if (i2cdatasrc[i2clocalcnt] != i2cdatadst[i2clocalcnt]) {
				result = 0;
				break;
			}
		}
		DBG_8195A("\r\nResult is %s\r\n", (result) ? "success" : "fail");

	}
	while(1){;}
}

void main(void)
{
	// create demo Task
	if(xTaskCreate( (TaskFunction_t)i2c_demo, "I2C DEMO", (2048/4), (void *)&i2cmaster, (tskIDLE_PRIORITY + 1), NULL)!= pdPASS) {
		DBG_8195A("Cannot create I2C demo task\n\r");
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

