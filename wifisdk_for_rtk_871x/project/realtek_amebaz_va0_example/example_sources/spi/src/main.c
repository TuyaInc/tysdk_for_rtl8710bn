/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2014 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */

#include "device.h"
#include "diag.h"
#include "main.h"
#include "spi_api.h"

#define FakeMbedAPI  1

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

// SPI1 (S2)
#define SPI1_MOSI  PA_23
#define SPI1_MISO  PA_22
#define SPI1_SCLK  PA_18
#define SPI1_CS    PA_19

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
spi_t spi_master;
spi_t spi_slave;

void main(void)
{
#if FakeMbedAPI

    /* SPI0 is as Slave */
    //SPI0_IS_AS_SLAVE = 1;
	spi_master.spi_idx=MBED_SPI1;
    spi_init(&spi_master, SPI1_MOSI, SPI1_MISO, SPI1_SCLK, SPI1_CS);
	spi_format(&spi_master, 8, 0, 0);
	spi_frequency(&spi_master, 200000);
	spi_slave.spi_idx=MBED_SPI0;
    spi_init(&spi_slave,  SPI0_MOSI, SPI0_MISO, SPI0_SCLK, SPI0_CS);
	spi_format(&spi_slave, 8, 0, 1);

    int TestingTimes = 10;
    int Counter      = 0;
    int TestData     = 0;
    int ReadData     = 0;

    int result = 1;

    /**
     * Master read/write, Slave read/write
     */
    DBG_8195A("--------------------------------------------------------\n");
    for(Counter = 0, TestData=0x01; Counter < TestingTimes; Counter++)
    {
        ReadData = spi_master_write(&spi_master, TestData);
        DBG_8195A("Master write: %02X, read: %02X\n", TestData, ReadData);
        if (TestData - 1 != ReadData) {
            result = 0;
        }

        TestData++;

        spi_slave_write(&spi_slave, TestData);
        ReadData = spi_slave_read(&spi_slave);
        DBG_8195A(ANSI_COLOR_CYAN"Slave  write: %02X, read: %02X\n"ANSI_COLOR_RESET, TestData, ReadData);
        if (TestData - 1 != ReadData) {
            result = 0;
        }

        TestData++;
    }

    /**
     * Master write, Slave read
     */
    DBG_8195A("--------------------------------------------------------\n");
    for(Counter = 0, TestData=0xFF; Counter < TestingTimes; Counter++)
    {
        spi_master_write(&spi_master, TestData);
        ReadData = spi_slave_read(&spi_slave);
        DBG_8195A("Master write: %02X\n", TestData);
        DBG_8195A(ANSI_COLOR_CYAN"Slave  read : %02X\n"ANSI_COLOR_RESET, ReadData);
        if (TestData != ReadData) {
            result = 0;
        }

        TestData--;
    }

    spi_free(&spi_master);
    spi_free(&spi_slave);

    DBG_8195A("SPI Demo finished.\n");

	DBG_8195A("\r\nResult is %s\r\n", (result) ? "success" : "fail");
    for(;;);

#else  // mbed SPI API emulation

#endif

}

