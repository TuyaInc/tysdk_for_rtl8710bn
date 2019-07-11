/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */

#include "device.h"
#include "timer_api.h"
#include "serial_api.h"
#include "serial_ex_api.h"
#include "task.h"

/*UART pin location:     
   UART0: 
   PA_23  (TX)
   PA_18  (RX)
   */
#define UART_TX    PA_23	//UART0  TX
#define UART_RX    PA_18	//UART0  RX

#define SRX_BUF_SZ 100
#define UART_TIMEOUT_MS   5000    //ms

char rx_buf[SRX_BUF_SZ]={0};
volatile uint32_t tx_busy=0;
volatile uint32_t rx_done=0;
volatile uint32_t rx_bytes=0;
gtimer_t uart_timer;

void uart_send_string_done(uint32_t id)
{
    serial_t    *sobj = (void*)id;
    tx_busy = 0;
}

void uart_recv_string_done(uint32_t id)
{
    serial_t    *sobj = (void*)id;
    gtimer_stop(&uart_timer);
    rx_bytes = sobj->rx_len;
    rx_done = 1;
}

void uart_send_string(serial_t *sobj, char *pstr)
{
    int32_t ret=0;

    if (tx_busy) {
        return;
    }
    
    tx_busy = 1;
    ret = serial_send_stream(sobj, pstr, _strlen(pstr));
    if (ret != 0) {
        DBG_8195A("%s Error(%d)\n", __FUNCTION__, ret);        
        tx_busy = 0;
    }
}

void
UartTimeoutCallbck (
    uint32_t id
)
{
    serial_t    *psobj;

    psobj = (serial_t *)id;

    rx_bytes = serial_recv_stream_abort(psobj);
    rx_done = 1;
}

void main(void)
{
    serial_t    sobj;
    int ret;

    /*uart0 used*/
    sobj.uart_idx = 0;

    serial_init(&sobj,UART_TX,UART_RX);
    serial_baud(&sobj,115200);
    serial_format(&sobj, 8, ParityNone, 1);

    serial_send_comp_handler(&sobj, (void*)uart_send_string_done, (uint32_t) &sobj);
    serial_recv_comp_handler(&sobj, (void*)uart_recv_string_done, (uint32_t) &sobj);

    // Initial a timer to wait UART RX done
    gtimer_init(&uart_timer, TIMER3);

    while (1) {
        // expect to receive maximum 100 bytes with timeout 5000ms
        rx_bytes = 0;
        rx_done = 0;
#if 0        
        ret = serial_recv_stream(&sobj, rx_buf, 10);    // Interrupt mode
#else
        ret = serial_recv_stream_dma(&sobj, rx_buf, 10);    // DMA mode
#endif
        gtimer_start_one_shout(&uart_timer, (UART_TIMEOUT_MS*1000), (void*)UartTimeoutCallbck, (uint32_t)&sobj);
        while (!rx_done);
        
        if (rx_bytes > 0) {
            rx_buf[rx_bytes] = 0x00; // end of string
            uart_send_string(&sobj, rx_buf);            
        }
    }
}


