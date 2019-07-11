/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2014 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */

#include "device.h"
#include "main.h"
#include "spi_api.h"
#include "spi_ex_api.h"
#include "gpio_api.h"

#define SPI_IS_AS_MASTER    1
#define TEST_BUF_SIZE       512
#define SCLK_FREQ           4000000
#define SPI_DMA_DEMO        0
#define GPIO_SYNC_PIN PA_5

/*SPIx pin location:

   S0: PA_4  (MOSI)
       PA_3  (MISO)
       PA_1  (SCLK)
       PA_2  (CS)
              
   S1: PA_23  (MOSI)
       PA_22  (MISO)
   	   PA_18  (SCLK)
       PA_19  (CS)
         
   S2: PB_3  (MOSI)
       PB_2  (MISO)
       PB_1  (SCLK)  
       PB_0  (CS)      
   */
// SPI0
#define SPI0_MOSI  PB_3
#define SPI0_MISO  PB_2
#define SPI0_SCLK  PB_1
#define SPI0_CS    PB_0

extern void wait_ms(u32);

char TxBuf[TEST_BUF_SIZE];
char RxBuf[TEST_BUF_SIZE];

volatile int MasterRxDone;
volatile int SlaveRxDone;
gpio_t GPIO_Syc;

void master_trx_done_callback(void *pdata, SpiIrq event)
{
    switch(event){
        case SpiRxIrq:
            DBG_8195A("Master RX done!\n");
            MasterRxDone = 1;
            break;
        case SpiTxIrq:
            DBG_8195A("Master TX done!\n");
            break;
        default:
            DBG_8195A("unknown interrput evnent!\n");
    }
}

void slave_trx_done_callback(void *pdata, SpiIrq event)
{
    switch(event){
        case SpiRxIrq:
            DBG_8195A("Slave RX done!\n");
            SlaveRxDone = 1;
            break;
        case SpiTxIrq:
            DBG_8195A("Slave TX done!\n");
            break;
        default:
            DBG_8195A("unknown interrput evnent!\n");
    }
}

void dump_data(const u8 *start, u32 size, char * strHeader)
{
    int row, column, index, index2, max;
    u8 *buf, *line;

    if(!start ||(size==0))
            return;

    line = (u8*)start;

    /*
    16 bytes per line
    */
    if (strHeader)
       DBG_8195A ("%s", strHeader);

    column = size % 16;
    row = (size / 16) + 1;
    for (index = 0; index < row; index++, line += 16) 
    {
        buf = (u8*)line;

        max = (index == row - 1) ? column : 16;
        if ( max==0 ) break; /* If we need not dump this line, break it. */

        DBG_8195A ("\n[%08x] ", line);
        
        //Hex
        for (index2 = 0; index2 < max; index2++)
        {
            if (index2 == 8)
            DBG_8195A ("  ");
            DBG_8195A ("%02x ", (u8) buf[index2]);
        }

        if (max != 16)
        {
            if (max < 8)
                DBG_8195A ("  ");
            for (index2 = 16 - max; index2 > 0; index2--)
                DBG_8195A ("   ");
        }

    }

    DBG_8195A ("\n");
    return;
}

#if SPI_IS_AS_MASTER
spi_t spi_master;
#else
spi_t spi_slave;
#endif
/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void main(void)
{
    int i;
    gpio_init(&GPIO_Syc, GPIO_SYNC_PIN);
    gpio_write(&GPIO_Syc, 0);//Initialize GPIO Pin to low 
#if SPI_IS_AS_MASTER 
    gpio_dir(&GPIO_Syc, PIN_INPUT);    // Direction: Master Input
#else
    gpio_dir(&GPIO_Syc, PIN_OUTPUT);    // Direction: Slave Output
#endif
    gpio_mode(&GPIO_Syc, PullNone);     // No pull   

#if SPI_IS_AS_MASTER
    spi_master.spi_idx=MBED_SPI1;
    spi_init(&spi_master, SPI0_MOSI, SPI0_MISO, SPI0_SCLK, SPI0_CS);
    spi_frequency(&spi_master, SCLK_FREQ);
    spi_format(&spi_master, 8, (SPI_SCLK_IDLE_LOW|SPI_SCLK_TOGGLE_MIDDLE) , 0);
    // wait Slave ready
   MasterRxDone = 0;
    wait_ms(1000);
    _memset(TxBuf, 0, TEST_BUF_SIZE);
    _memset(RxBuf, 0, TEST_BUF_SIZE);

    for (i=0;i<TEST_BUF_SIZE;i++) {
        TxBuf[i] = i;
    }

    spi_irq_hook(&spi_master, (spi_irq_handler)master_trx_done_callback, (uint32_t)&spi_master);
    spi_flush_rx_fifo(&spi_master);
    
    while(gpio_read(&GPIO_Syc) == 0);
#if SPI_DMA_DEMO
    spi_master_write_read_stream_dma(&spi_master, TxBuf, RxBuf, TEST_BUF_SIZE);
#else
    spi_master_write_read_stream(&spi_master, TxBuf, RxBuf, TEST_BUF_SIZE);
#endif
       
    while(MasterRxDone == 0) {
        wait_ms(1000);
    }
    dump_data(RxBuf, TEST_BUF_SIZE, "SPI Master Read Data:");
    
    spi_free(&spi_master);

#else
    spi_slave.spi_idx=MBED_SPI0;
    spi_init(&spi_slave, SPI0_MOSI, SPI0_MISO, SPI0_SCLK, SPI0_CS);
    spi_format(&spi_slave, 8, (SPI_SCLK_IDLE_LOW|SPI_SCLK_TOGGLE_MIDDLE) , 1);

    while (spi_busy(&spi_slave)) {
        DBG_8195A("Wait SPI Bus Ready...\r\n");
        wait_ms(1000);
    }
    SlaveRxDone = 0;

    _memset(TxBuf, 0, TEST_BUF_SIZE);
    _memset(RxBuf, 0, TEST_BUF_SIZE);
     for (i=0;i<TEST_BUF_SIZE;i++) {
        TxBuf[i] = ~i;
    }   
    spi_irq_hook(&spi_slave, (spi_irq_handler)slave_trx_done_callback, (uint32_t)&spi_slave);
    spi_flush_rx_fifo(&spi_slave);
#if SPI_DMA_DEMO
    spi_slave_read_stream_dma(&spi_slave, RxBuf, TEST_BUF_SIZE);
    spi_slave_write_stream_dma(&spi_slave, TxBuf, TEST_BUF_SIZE);
#else
    spi_slave_read_stream(&spi_slave, RxBuf, TEST_BUF_SIZE);
    spi_slave_write_stream(&spi_slave, TxBuf, TEST_BUF_SIZE);
#endif
    gpio_write(&GPIO_Syc, 1);

    i=0;
    DBG_8195A("SPI Slave Wait Read Done...\r\n");
    while(SlaveRxDone == 0) {
        wait_ms(100);
        i++;
        if (i>150) {
            DBG_8195A("SPI Slave Wait Timeout\r\n");
            break;
        }
    }
    
    gpio_write(&GPIO_Syc, 0);//Initialize GPIO Pin to low 
    dump_data(RxBuf, TEST_BUF_SIZE, "SPI Slave Read Data:");
 
    spi_free(&spi_slave);
#endif

    DBG_8195A("SPI Demo finished.\n");
    for(;;);
}

