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
#include "serial_api.h"

/*UART pin location:

   UART0: PA_4  (TX)
              PA_1  (RX)
              
   UART0: PA_23  (TX)
              PA_18  (RX)

   UART1: PA_26  (TX)
              PA_25  (RX)    
   */
#define UART_TX    PA_23
#define UART_RX    PA_18

static serial_t	sobj_g;
volatile char rc=0;

void uart_send_string(serial_t *sobj, char *pstr)
{
	unsigned int i=0;

	while (*(pstr+i) != 0) {
		serial_putc(sobj, *(pstr+i));
		i++;
	}
}

void uart_irq(uint32_t id, SerialIrq event)
{
	serial_t    *sobj = (void*)id;

	if(event == RxIrq) {		
		while (serial_readable(sobj)) {
			rc = serial_getc(sobj);
			serial_putc(sobj, rc);
		}
	}

	if(event == TxIrq && rc!=0){
		uart_send_string(sobj, "\r\n8195a$");
		rc = 0;
	}
}

void psm_sleep_uart(void)
{
	uart_config[0].LOW_POWER_RX_ENABLE = ENABLE;
	
	// mbed uart test
	serial_init(&sobj_g,UART_TX,UART_RX);
	serial_baud(&sobj_g,38400);
	serial_format(&sobj_g, 8, ParityNone, 1);

	uart_send_string(&sobj_g, "UART IRQ API Demo...\r\n");
	uart_send_string(&sobj_g, "Enter sleep!!\n");
	uart_send_string(&sobj_g, "\r\n8195a$");
	serial_irq_handler(&sobj_g, uart_irq, (uint32_t)&sobj_g);
	serial_irq_set(&sobj_g, RxIrq, 1);
	serial_irq_set(&sobj_g, TxIrq, 1);

	pmu_tickless_debug(ENABLE); /* open tickless log */
	pmu_sysactive_timer_init();
	pmu_set_sysactive_time(PMU_LOGUART_DEVICE, 5000);
	pmu_release_wakelock(PMU_OS);

	vTaskDelete(NULL);
}

void main(void)
{
	// create demo Task
	if(xTaskCreate( (TaskFunction_t)psm_sleep_uart, "UART WAKEUP DEMO", (2048/4), (void *)&sobj_g, (tskIDLE_PRIORITY + 1), NULL)!= pdPASS) {
		DBG_8195A("Cannot create uart wakeup demo task\n\r");
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
