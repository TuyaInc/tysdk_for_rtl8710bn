Example Description

This example describes how to use SPI read/write by mbed api.


The SPI Interface provides a "Serial Peripheral Interface" Master.

This interface can be used for communication with SPI slave devices,
such as FLASH memory, LCD screens and other modules or integrated circuits.

In this example, it use 2 sets of SPI. One is master, the other is slave.
By default it use SPI0 as slave, and use SPI1 as master.
So we connect them as below:
    Connect SPI0_MOSI (PA_4) to SPI1_MOSI (PA_23)
    Connect SPI0_MISO (PA_3) to SPI1_MISO (PA_22)
    Connect SPI0_SCLK (PA_1) to SPI1_SCLK (PA_18)
    Connect SPI0_CS   (PA_2) to SPI1_CS   (PA_19)

After boot up, the master will send data to slave and shows result on LOG_OUT.

Note:spi_idx should be asigned first in the initialization process,We use MBED_SPI1 for Master and MBED_SPI0 for Slave