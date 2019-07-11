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
//#include "main.h"

#define TX_DMA_MODE   1    // 0: interrupt mode, 1: DMA mode
#define RX_DMA_MODE   1    // 0: interrupt mode, 1: DMA mode

/*UART pin location:     
   UART0: 
   PA_23  (TX)
   PA_18  (RX)
   */
#define UART_TX    PA_23	//UART0  TX
#define UART_RX    PA_18	//UART0  RX

#define UART_TIMEOUT_MS     3000    // 3 sec

#define SRX_BUF_SZ      1536
#define STX_BUF_SZ      1536

#define UART_STACK_SZ       2048


char tx_buf[STX_BUF_SZ]={0};
char rx_buf[SRX_BUF_SZ]={0};
volatile uint32_t tx_bytes=0;
volatile uint32_t rx_bytes=0;

SemaphoreHandle_t  UartHWSema;  // Uart HW resource
SemaphoreHandle_t  UartRxSema;  // RX done
SemaphoreHandle_t  UartTxSema;  // TX ready, also use as TX done

void uart_send_done(uint32_t id)
{
    serial_t *sobj = (void*)id;
    signed portBASE_TYPE xHigherPriorityTaskWoken;
    
    tx_bytes += sobj->tx_len;
    xSemaphoreGiveFromISR(UartTxSema, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void uart_recv_done(uint32_t id)
{
    serial_t *sobj = (void*)id;
    signed portBASE_TYPE xHigherPriorityTaskWoken;
    
    rx_bytes += sobj->rx_len;
    xSemaphoreGiveFromISR(UartRxSema, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void uart_tx_thread(void *param)
{
    serial_t *psobj=param;  // UART object
    int i;
    int tx_byte_timeout;
    int32_t ret;
    unsigned int loop_cnt=0;
    unsigned int tx_size;
    
    // Initial TX buffer
    for (i=0;i<STX_BUF_SZ;i++) {
        tx_buf[i] = 0x30 + (i%10);
    }
    tx_buf[STX_BUF_SZ-1] = 0;   // end of string
    
    while (1) {
        loop_cnt++;
        tx_size = (loop_cnt % STX_BUF_SZ) + 1;
        // Wait TX Rady (TX Done)
        if( xSemaphoreTake( UartTxSema, ( TickType_t ) UART_TIMEOUT_MS/portTICK_RATE_MS ) != pdTRUE ) {
            DBG_8195A("%s: send timeout!!\r\n");
            xSemaphoreTake(UartHWSema, portMAX_DELAY);  // get the semaphore before access the HW
            tx_byte_timeout = serial_send_stream_abort(psobj);
            xSemaphoreGive( UartHWSema );   // return the semaphore after access the HW
            if (tx_byte_timeout > 0) {
                tx_bytes += tx_byte_timeout;
            }

            xSemaphoreGive( UartTxSema );   // Ready to TX
        } else {
            xSemaphoreTake(UartHWSema, portMAX_DELAY);  // get the semaphore before access the HW
#if TX_DMA_MODE        
            ret = serial_send_stream_dma(psobj, tx_buf, tx_size);
#else
            ret = serial_send_stream(psobj, tx_buf, tx_size);
#endif
            xSemaphoreGive( UartHWSema );   // return the semaphore after access the HW
            if (ret != 0) {
                DBG_8195A("uart_tx_thread: send error %d\r\n", ret);
                xSemaphoreGive( UartTxSema );   // Ready to TX
            }
        }
        
    }

    vTaskDelete(NULL);
    
}

void uart_rx_thread(void *param)
{
    serial_t *psobj=param;  // UART object
    int rx_byte_timeout;
    int ret;
    unsigned int loop_cnt=0;
    unsigned int rx_size;
    
    
    while (1) {
        loop_cnt++;
        rx_size = (loop_cnt % SRX_BUF_SZ) + 1;
        xSemaphoreTake(UartHWSema, portMAX_DELAY);  // get the semaphore before access the HW
#if RX_DMA_MODE        
        ret = serial_recv_stream_dma(psobj, rx_buf, rx_size);
#else
        ret = serial_recv_stream(psobj, rx_buf, rx_size);
#endif
        xSemaphoreGive( UartHWSema );   // return the semaphore after access the HW
        if (ret == 0) {
            // Wait RX done
            if( xSemaphoreTake( UartRxSema, ( TickType_t ) UART_TIMEOUT_MS/portTICK_RATE_MS ) != pdTRUE ) {
                xSemaphoreTake(UartHWSema, portMAX_DELAY);  // get the semaphore before access the HW
                rx_byte_timeout = serial_recv_stream_abort(psobj);
                xSemaphoreGive( UartHWSema );   // return the semaphore after access the HW
                if (rx_byte_timeout > 0) {
                    rx_bytes += rx_byte_timeout;
                }
				}
        } else {
            DBG_8195A("uart_rx_thread: receive error %d\r\n", ret);
        }
    }

    vTaskDelete(NULL);
    
}

void uart_test_demo(void *param)
{
    serial_t sobj;  // UART object
    int ret;

    serial_init(&sobj,UART_TX,UART_RX);
    serial_baud(&sobj,38400);
    serial_format(&sobj, 8, ParityNone, 1);

    serial_send_comp_handler(&sobj, (void*)uart_send_done, (uint32_t) &sobj);
    serial_recv_comp_handler(&sobj, (void*)uart_recv_done, (uint32_t) &sobj);

    // Create semaphore for UART HW resource
    UartHWSema = xSemaphoreCreateBinary();
    xSemaphoreGive( UartHWSema );

    // Create semaphore for UART RX done(received espected bytes or timeout)
    UartRxSema = xSemaphoreCreateBinary();

    // Create semaphore for UART TX done
    UartTxSema = xSemaphoreCreateBinary();
    xSemaphoreGive( UartTxSema );    // Ready to TX

    if(xTaskCreate(uart_rx_thread, ((const char*)"uart_recv_thread"), (UART_STACK_SZ>>2), &sobj, tskIDLE_PRIORITY + 2, NULL) != pdPASS) {
        DBG_8195A("xTaskCreate(uart_rx_thread) failed\r\n");
    }
    
    if(xTaskCreate(uart_tx_thread, ((const char*)"uart_tx_thread"), (UART_STACK_SZ>>2), &sobj, tskIDLE_PRIORITY + 2, NULL) != pdPASS) {
        DBG_8195A("xTaskCreate(uart_tx_thread) failed\r\n");
    }

    while (1) {
        DBG_8195A("tx_bytes:%d rx_bytes:%d\r\n", tx_bytes, rx_bytes);
        vTaskDelay(1000 / portTICK_RATE_MS);    
    }
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



