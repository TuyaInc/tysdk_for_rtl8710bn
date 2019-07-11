Example Description

This example describes how to use SPI read/write by mbed api.


The SPI Interface provides a "Serial Peripheral Interface" Master.

This interface can be used for communication with SPI slave devices,
such as FLASH memory, LCD screens and other modules or integrated circuits.


In this example, we use config SPI_IS_AS_MASTER to decide if device is master or slave.
    If SPI_IS_AS_MASTER is 1, then device is master.
    If SPI_IS_AS_MASTER is 0, then device is slave.

We connect wires as below:
    master's MOSI (PA_4) connect to slave's MOSI (PA_4)
    master's MISO (PA_3) connect to slave's MISO (PA_3)
    master's SCLK (PA_1) connect to slave's SCLK (PA_1)
    master's CS   (PA_2) connect to slave's CS   (PA_2)

This example shows master sends data to slave.
We bootup slave first, and then bootup master.
Then log will presents that master sending data to slave.

Note:spi_idx should be asigned first in the initialization process,We use MBED_SPI1 for Master and MBED_SPI0 for Slave