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
#define SCLK_FREQ          (2000000)
#define TEST_LOOP           100
#define _SUCCEED 1
#define _FAIL  0

spi_t spi_master;
spi_t spi_slave;

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

// SPI0 (S0)
#define SPI0_MOSI  PA_4
#define SPI0_MISO  PA_3
#define SPI0_SCLK  PA_1
#define SPI0_CS    PA_2

// SPI1 (S1)
#define SPI1_MOSI  PA_23
#define SPI1_MISO  PA_22
#define SPI1_SCLK  PA_18
#define SPI1_CS  PA_19 

extern void wait_ms(u32);

u8 Master_TxBuf[TEST_BUF_SIZE];
u8 Master_RxBuf[TEST_BUF_SIZE];
u8 Slave_TxBuf[TEST_BUF_SIZE];
u8 Slave_RxBuf[TEST_BUF_SIZE];

volatile int MasterTrDone;
volatile int SlaveRxDone;
gpio_t spi_cs;


void master_tr_done_callback(void *pdata, SpiIrq event)
{
	MasterTrDone = 1;
}

void master_cs_tr_done_callback(void *pdata, SpiIrq event)
{
	/*Disable CS by pulling gpio to high*/
	gpio_write(&spi_cs, 1);
}

void slave_trx_done_callback(void *pdata, SpiIrq event)
{
	switch(event){
		case SpiRxIrq:
			SlaveRxDone = 1;
			break;
		case SpiTxIrq:
			break;
		default:
			DBG_8195A("unknown interrput evnent!\n");
    }
}

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void main(void)
{
	int Counter = 0;
	int i=0;
	u8 TestResult=_SUCCEED;
    
	spi_master.spi_idx=MBED_SPI1;
	spi_init(&spi_master, SPI1_MOSI, SPI1_MISO, SPI1_SCLK, SPI1_CS);
	spi_frequency(&spi_master, SCLK_FREQ);
	spi_format(&spi_master, 8, (SPI_SCLK_IDLE_LOW|SPI_SCLK_TOGGLE_START) , 0);

	gpio_init(&spi_cs, SPI1_CS);      //Change Master CS to GPIO,which is controlled by SW
	gpio_write(&spi_cs, 1);           //Initialize GPIO Pin to high 
	gpio_dir(&spi_cs, PIN_OUTPUT);    // Direction: Output
	gpio_mode(&spi_cs, PullNone);     // No pull   

	spi_slave.spi_idx=MBED_SPI0;
	spi_init(&spi_slave, SPI0_MOSI, SPI0_MISO, SPI0_SCLK, SPI0_CS);
	spi_format(&spi_slave, 8, (SPI_SCLK_IDLE_LOW|SPI_SCLK_TOGGLE_START) , 1);

	DBG_8195A("Test Start\n");
	for(i = 0; i < TEST_BUF_SIZE;i++)
		Master_TxBuf[i] = (u8)i;

	for(i = 0; i < TEST_BUF_SIZE;i++)
		Slave_TxBuf[i] =(u8)~i;
	
	while (Counter < TEST_LOOP) {
		DBG_8195A("======= Test Loop %d =======\r\n", Counter);
		_memset(Master_RxBuf, 0, TEST_BUF_SIZE);
		 _memset(Slave_RxBuf, 0, TEST_BUF_SIZE);
		
		spi_irq_hook(&spi_slave, (spi_irq_handler)slave_trx_done_callback, (uint32_t)&spi_slave);
		spi_slave_read_stream_dma(&spi_slave, Slave_RxBuf, TEST_BUF_SIZE);
		spi_slave_write_stream_dma(&spi_slave, Slave_TxBuf, TEST_BUF_SIZE);
		
		spi_irq_hook(&spi_master, (spi_irq_handler)master_tr_done_callback, (uint32_t)&spi_master);
		spi_bus_tx_done_irq_hook(&spi_master, (spi_irq_handler)master_cs_tr_done_callback, (uint32_t)&spi_master);
		
		DBG_8195A("SPI Master Write Begin...\r\n");
		MasterTrDone = 0;
		SlaveRxDone=0;
		/*Enable CS by pulling gpio to low*/
		gpio_write(&spi_cs, 0);
		spi_master_write_read_stream(&spi_master, Master_TxBuf, Master_RxBuf,TEST_BUF_SIZE);

		i=0;
		DBG_8195A("SPI Master Wait Rx Done...\r\n");
		while((MasterTrDone == 0) ||( SlaveRxDone==0)) {
			wait_ms(10);
			i++;
		}
		DBG_8195A("SPI Slave Rx Done!!\r\n");

		for(i=0;i<TEST_BUF_SIZE;i++){
			if((Master_RxBuf[i]!=(u8)~i) ||(Slave_RxBuf[i]!=(u8)i)){
				TestResult=_FAIL;
				break;
			}
		}

		if(i==TEST_BUF_SIZE){
			DBG_8195A("Loop:%d Test Succeed!\r\n",Counter);
		}else{
			DBG_8195A("Loop:%d Test Fail!\r\n",Counter);
			goto END;
		}				
				
#if 0
		for(i=0;i<TEST_BUF_SIZE;i++)
			DBG_8195A("SPI Master Rx:0x%x,Slave Rx:0x%x,\r\n",(u8)Master_RxBuf[i],(u8)Slave_RxBuf[i]);
#endif
		wait_ms(1000);
		Counter++;
	}
END:
	spi_free(&spi_master);
	spi_free(&spi_slave);

	DBG_8195A("SPI Demo finished.\n");
	for(;;);
}
