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

#define SPI_IS_AS_MASTER 1

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

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void main(void)
{
    int TestingTimes = 10;
    int Counter      = 0;
    int TestData     = 0;

#if SPI_IS_AS_MASTER
    spi_t spi_master;

    spi_master.spi_idx=MBED_SPI1;
    spi_init(&spi_master, SPI0_MOSI, SPI0_MISO, SPI0_SCLK, SPI0_CS);

    DBG_8195A("--------------------------------------------------------\n");
    for(Counter = 0, TestData=0xFF; Counter < TestingTimes; Counter++) {
        spi_master_write(&spi_master, TestData);
        DBG_8195A("Master write: %02X\n", TestData);
        TestData--;
    }
    spi_free(&spi_master);

#else
    spi_t spi_slave;

    spi_slave.spi_idx=MBED_SPI0;
    spi_init(&spi_slave,  SPI0_MOSI, SPI0_MISO, SPI0_SCLK, SPI0_CS);
    DBG_8195A("--------------------------------------------------------\n");
    for(Counter = 0, TestData=0xFF; Counter < TestingTimes; Counter++) {
        DBG_8195A(ANSI_COLOR_CYAN"Slave  read : %02X\n"ANSI_COLOR_RESET,
                spi_slave_read(&spi_slave));
        TestData--;
    }
    spi_free(&spi_slave);
#endif

    DBG_8195A("SPI Demo finished.\n");
    for(;;);
}
