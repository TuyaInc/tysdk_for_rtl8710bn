Example Description

This example describes how to use SPI master to transmit & receive data concurrently by mbed api.

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

SPI_IS_AS_MASTER is 1
The device operates in the master mode.
The SPI master transmits 512 bytes data in increacing order to the slave.
In the meantime, we expect the master to receive data sent by the SPI slave via spi_master_write_read_stream(spi_master_write_read_stream_dma).

SPI_IS_AS_MASTER is 0
The device operates in the slave mode.
To differentiate data from the master or the slave, we assign data sent from the slave in decreasing order.
We call the spi_slave_read_stream (or spi_slave_read_stream_dma) first to get the slave device ready to receive,
then call the spi_slave_write_stream(or spi_slave_write_stream_dma) that the slave would push data into its fifo.

The slave device should be enabled first prior to the master device.
As soon as the master sends the clock to the slave, the slave then transmits the data from its fifo to the master and receive data from the master simultaneously.
To make sure the order is right, we use a GPIO pin to signal the master that the slave device is ready.

Note:spi_idx should be asigned first in the initialization process,We use MBED_SPI1 for Master and MBED_SPI0 for Slave

