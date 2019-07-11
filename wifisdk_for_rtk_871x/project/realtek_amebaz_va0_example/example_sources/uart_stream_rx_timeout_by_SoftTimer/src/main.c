/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */
 
#include "device.h"
#include "serial_api.h"
#include "serial_ex_api.h"
#include "task.h"
#include "timers.h"

/*UART pin location:     
   UART0: 
   PA_23  (TX)
   PA_18  (RX)
   */
#define UART_TX    PA_23    //UART0  TX
#define UART_RX    PA_18   //UART0  RX

#define SRX_BUF_SZ 100
#define UART_TIMEOUT_MS   5000    //ms

char rx_buf[SRX_BUF_SZ]={0};
volatile uint32_t rx_bytes=0;
TimerHandle_t UartTimerHandle;
SemaphoreHandle_t  UartRxSema;
SemaphoreHandle_t  UartTxSema;


void uart_send_string_done(uint32_t id)
{
//    serial_t    *sobj = (void*)id;
    signed portBASE_TYPE xHigherPriorityTaskWoken;
    
    xSemaphoreGiveFromISR(UartTxSema, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void uart_recv_string_done(uint32_t id)
{
    serial_t    *sobj = (void*)id;
    signed portBASE_TYPE xHigherPriorityTaskWoken;
    
    xTimerStopFromISR( UartTimerHandle, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    rx_bytes = sobj->rx_len;
    xSemaphoreGiveFromISR(UartRxSema, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void uart_send_string(serial_t *sobj, char *pstr)
{
    int32_t ret=0;

    xSemaphoreTake(UartTxSema, portMAX_DELAY);

    ret = serial_send_stream(sobj, pstr, _strlen(pstr));
    if (ret != 0) {
        DBG_8195A("%s Error(%d)\n", __FUNCTION__, ret);
        xSemaphoreGive( UartTxSema );
    }
}

void
UartTimeoutCallbck (
    xTimerHandle pxTimer
)
{
    serial_t    *psobj;

    psobj = (serial_t*) pvTimerGetTimerID( pxTimer );

    rx_bytes = serial_recv_stream_abort(psobj);
    xSemaphoreGive( UartRxSema );
}

int uart_test_demo(void *param)
{
    serial_t    sobj;
    int ret;

    serial_init(&sobj,UART_TX,UART_RX);
    serial_baud(&sobj,115200);
    serial_format(&sobj, 8, ParityNone, 1);

    serial_send_comp_handler(&sobj, (void*)uart_send_string_done, (uint32_t) &sobj);
    serial_recv_comp_handler(&sobj, (void*)uart_recv_string_done, (uint32_t) &sobj);

    // Create a Software Timer to wait UART RX done
    UartTimerHandle = xTimerCreate ((const char*)"UART_WAIT", (UART_TIMEOUT_MS/portTICK_RATE_MS),
            0, (void *) &sobj, UartTimeoutCallbck);
    // Create semaphore for UART RX done(received espected bytes or timeout)
    UartRxSema = xSemaphoreCreateBinary();

    // Create semaphore for UART TX done
    UartTxSema = xSemaphoreCreateBinary();
    xSemaphoreGive( UartTxSema );    // Ready to TX

    while (1) {
        rx_bytes = 0;
#if 0
        ret = serial_recv_stream(&sobj, rx_buf, 10);    // Interrupt mode
#else
        ret = serial_recv_stream_dma(&sobj, rx_buf, 10);    // DMA mode
#endif
        xTimerStart(UartTimerHandle, 0);
        xSemaphoreTake(UartRxSema, portMAX_DELAY);

        if (rx_bytes > 0) {
            rx_buf[rx_bytes] = 0x00; // end of string
            uart_send_string(&sobj, rx_buf);
        }
    }

    return 0;
}

void main(void)
{
	// create demo Task
	if(xTaskCreate( (TaskFunction_t)uart_test_demo, "uart test demo", (2048/2), (void *)NULL, (tskIDLE_PRIORITY + 1), NULL)!= pdPASS) {
		DBG_8195A("Cannot create uart test demo task\n\r");
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


