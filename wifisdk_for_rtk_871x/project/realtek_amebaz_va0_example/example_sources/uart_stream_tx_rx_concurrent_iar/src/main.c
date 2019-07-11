/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2015 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */

#include "device.h"
#include "serial_api.h"
#include "serial_ex_api.h"
#include "task.h"

/*UART pin location:     
   UART0: 
   PA_23  (TX)
   PA_18  (RX)
   */
#define UART_TX    PA_23		//UART0  TX
#define UART_RX    PA_18	//UART0  RX 

#define TX_DMA_MODE		1       /*1: DMA mode; 0: interrupt mode*/
#define RX_DMA_MODE		1       /*1: DMA mode; 0: interrupt mode*/


#define BUF_SZ					2048
#define UART_TIMEOUT_MS			5000    //ms

#define TASK_STACK_SIZE         2048
#define TASK_PRIORITY           (tskIDLE_PRIORITY + 1)

char tx_buf[BUF_SZ]={0};
char rx_buf[BUF_SZ]={0};

volatile uint32_t rx_bytes=0;
volatile uint32_t tx_bytes=0;

SemaphoreHandle_t  UartHWSema;  // Uart HW resource
SemaphoreHandle_t  UartRxSema;
SemaphoreHandle_t  UartTxSema;

void uart_send_string_done(uint32_t id)
{
    serial_t    *sobj = (void*)id;
    signed portBASE_TYPE xHigherPriorityTaskWoken;
    
    xSemaphoreGiveFromISR(UartTxSema, &xHigherPriorityTaskWoken);
}

void uart_recv_string_done(uint32_t id)
{
    serial_t    *sobj = (void*)id;
    signed portBASE_TYPE xHigherPriorityTaskWoken;
    
    rx_bytes = sobj->rx_len;
    xSemaphoreGiveFromISR(UartRxSema, &xHigherPriorityTaskWoken);
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

void uart_demo_tx(void *param)
{
    serial_t *psobj=param;  // UART object
    int i;
    int tx_byte_timeout;
    int32_t ret;
    unsigned int loop_cnt=0;
    unsigned int tx_size = 100;
    
    // Initial TX buffer
    //memset(tx_buf, 0xFF, BUF_SZ);
	memset(tx_buf, 0x55, BUF_SZ);
	tx_buf[0] = tx_size & 0xFF;
	tx_buf[1] = (tx_size >> 8) & 0xFF;
    tx_buf[tx_size + 2] = 0;   // end of string
    
	tx_size += 2;
	
	xSemaphoreGive( UartTxSema );    // Ready to TX
    while (1) {
        // Wait TX Rady (TX Done)
        if( xSemaphoreTake( UartTxSema, ( TickType_t ) UART_TIMEOUT_MS/portTICK_RATE_MS ) != pdTRUE ) {
            xSemaphoreTake(UartHWSema, portMAX_DELAY);  // get the semaphore before access the HW
            tx_byte_timeout = serial_send_stream_abort(psobj);
            xSemaphoreGive( UartHWSema );   // return the semaphore after access the HW

			DBG_8195A("send timeout!! %d bytes sent\r\n", tx_byte_timeout);			

            xSemaphoreGive( UartTxSema );   // Ready to TX
        } else {
            xSemaphoreTake(UartHWSema, portMAX_DELAY);  // get the semaphore before access the HW
#if TX_DMA_MODE        
            ret = serial_send_stream_dma(psobj, tx_buf, tx_size);
#else
            ret = serial_send_stream(psobj, tx_buf, tx_size);
#endif
            xSemaphoreGive( UartHWSema );   // return the semaphore after access the HW
			vTaskDelay(3000);
			if (ret != 0) {
                DBG_8195A("uart_tx_thread: send error %d\r\n", ret);
                xSemaphoreGive( UartTxSema );   // Ready to TX
            }
        }
    }
}

void uart_demo_rx(void *param)
{
	serial_t	*psobj=param;  // UART object
    
    int ret;
	int rx_len;

    while (1) {
		memset(rx_buf, 0, BUF_SZ);
        rx_bytes = 0;
		
		xSemaphoreTake(UartHWSema, portMAX_DELAY);  // get the semaphore before access the HW		
#if RX_DMA_MODE       
        ret = serial_recv_stream_dma(psobj, rx_buf, BUF_SZ);    // DMA mode
#else        
        ret = serial_recv_stream(psobj, rx_buf, BUF_SZ);    // Interrupt mode
#endif
		xSemaphoreGive( UartHWSema );   // return the semaphore after access the HW
		
        if( xSemaphoreTake( UartRxSema, ( TickType_t ) UART_TIMEOUT_MS/portTICK_RATE_MS ) != pdTRUE ) {
			xSemaphoreTake(UartHWSema, portMAX_DELAY);  // get the semaphore before access the HW
            rx_bytes = serial_recv_stream_abort(psobj);
			xSemaphoreGive( UartHWSema );   // return the semaphore after access the HW
			printf("recv timeout!!! %d bytes recvied\n\r", rx_bytes);
        }
		
		rx_len = (rx_buf[1]<<8)|(rx_buf[0]);
		if(rx_bytes && (rx_len != (rx_bytes-2))){
			printf("RX length not match %d != %d\n\r", rx_len, rx_bytes-2);
			
			int i,j;
			for(i=0;i<1+rx_bytes/10;i++){
				for(j=0;j<10;j++)
					printf(" %x", rx_buf[i*10+j]&0xFF);
				printf("\n\r");
			}
		}
    }
}

void uart_test_demo(void *param)
{
	serial_t    sobj;
    serial_init(&sobj,UART_TX,UART_RX);
    serial_baud(&sobj,38400);
    serial_format(&sobj, 8, ParityNone, 1);

    serial_send_comp_handler(&sobj, (void*)uart_send_string_done, (uint32_t) &sobj);
    serial_recv_comp_handler(&sobj, (void*)uart_recv_string_done, (uint32_t) &sobj);

	// Hardware resource lock
	UartHWSema = xSemaphoreCreateBinary();
    xSemaphoreGive( UartHWSema );
	
    // Create semaphore for UART RX done(received espected bytes or timeout)
    UartRxSema = xSemaphoreCreateBinary();

    // Create semaphore for UART TX done
    UartTxSema = xSemaphoreCreateBinary();
	
	// create demo Task
	if(xTaskCreate( (TaskFunction_t)uart_demo_rx, "UART RX DEMO", (TASK_STACK_SIZE/4), (void *)&sobj, TASK_PRIORITY, NULL)!= pdPASS) {
		DBG_8195A("Cannot create rx demo task\n\r");
		goto end_demo;
	}
	if(xTaskCreate( (TaskFunction_t)uart_demo_tx, "UART TX DEMO", (TASK_STACK_SIZE/4), (void *)&sobj, TASK_PRIORITY, NULL)!= pdPASS) {
		DBG_8195A("Cannot create tx demo task\n\r");
		goto end_demo;
	}

end_demo:

	vTaskDelete(NULL);

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

