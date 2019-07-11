Example Description

This example describes how to use SPI stream read/write by mbed api.


The SPI Interface provides a "Serial Peripheral Interface" Master.

This interface can be used for communication with SPI slave devices,
such as FLASH memory, LCD screens and other modules or integrated circuits.


In this example, we use config SPI_IS_AS_MASTER to decide if device is master or slave.
    If SPI_IS_AS_MASTER is 1, then device is master.
    If SPI_IS_AS_MASTER is 0, then device is slave.

The option SPI_DMA_DEMO provides a demostration in the SPI DMA mode.
    If SPI_DMA_DEMO is 1, then the device operates in DMA mode.
    If SPI_DMA_DEMO is 0, then the device operates in interrupt mode.

We connect wires as below:
    master's MOSI (PB_3) connect to slave's MOSI (PB_3)
    master's MISO (PB_2) connect to slave's MISO (PB_2)
    master's SCLK (PB_1) connect to slave's SCLK (PB_1)
    master's CS   (PB_0) connect to slave's CS   (PB_0)
	master's GPIO_SYNC_PIN (PA_5) connect to slave's GPIO_SYNC_PIN (PA_5)

This example shows master sends data to slave.
We bootup slave first, and then bootup master.
Then log will presents that master sending data to slave.
To ensure the order is correct, we use a GPIO pin to notify the master that the slave device is ready to write or read data.

Note:spi_idx should be asigned first in the initialization process,We use MBED_SPI1 for Master and MBED_SPI0 for Slave