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

#define TEST_BUF_SIZE       1024
#define SCLK_FREQ           1000000
#define TEST_LOOP           100
#define SPI_GPIO_CS PA_5

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
#define SPI0_MOSI  PA_4
#define SPI0_MISO  PA_3
#define SPI0_SCLK  PA_1
#define SPI0_CS    PA_2



extern void wait_ms(u32);

char TestBuf[TEST_BUF_SIZE];
volatile int TrDone;
gpio_t spi_cs;


void master_tr_done_callback(void *pdata, SpiIrq event)
{
    TrDone = 1;
}

void master_cs_tr_done_callback(void *pdata, SpiIrq event)
{
    /*Disable CS by pulling gpio to high*/
    gpio_write(&spi_cs, 1);
    DBG_8195A("SPI Master CS High==>\r\n");
}


spi_t spi_master;

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void main(void)
{
    int Counter = 0;
    int i;
    

    gpio_init(&spi_cs, SPI_GPIO_CS);
    gpio_write(&spi_cs, 1);//Initialize GPIO Pin to high 
    gpio_dir(&spi_cs, PIN_OUTPUT);    // Direction: Output
    gpio_mode(&spi_cs, PullNone);     // No pull   

    spi_master.spi_idx=MBED_SPI1;
    spi_init(&spi_master, SPI0_MOSI, SPI0_MISO, SPI0_SCLK, SPI0_CS);
    spi_frequency(&spi_master, SCLK_FREQ);
    spi_format(&spi_master, 8, (SPI_SCLK_IDLE_LOW|SPI_SCLK_TOGGLE_START) , 0);
    DBG_8195A("Test Start\n");

    for(i = 0; i < TEST_BUF_SIZE;i++)
        TestBuf[i] = i;
    while (Counter < TEST_LOOP) {
        DBG_8195A("======= Test Loop %d =======\r\n", Counter);

        spi_irq_hook(&spi_master, (spi_irq_handler)master_tr_done_callback, (uint32_t)&spi_master);
        spi_bus_tx_done_irq_hook(&spi_master, (spi_irq_handler)master_cs_tr_done_callback, (uint32_t)&spi_master);
        DBG_8195A("SPI Master Write Test==>\r\n");
        TrDone = 0;
        /*Enable CS by pulling gpio to low*/
        gpio_write(&spi_cs, 0);
        spi_master_write_stream(&spi_master, TestBuf, TEST_BUF_SIZE);

        i=0;
        DBG_8195A("SPI Master Wait Write Done...\r\n");
        while(TrDone == 0) {
            wait_ms(10);
            i++;
        }
        DBG_8195A("SPI Master Write Done!!\r\n");
        wait_ms(10000);
        Counter++;
    }
    spi_free(&spi_master);
    DBG_8195A("SPI Master Test Done<==\r\n");

    DBG_8195A("SPI Demo finished.\n");
    for(;;);
}
