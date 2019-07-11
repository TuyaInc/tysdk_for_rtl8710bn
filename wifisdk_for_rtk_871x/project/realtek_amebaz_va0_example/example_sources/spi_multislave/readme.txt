Example Description

This example describes how to use SPI stream read/write for multi-slave by mbed api.

The SPI Interface provides a "Serial Peripheral Interface" Master.

This interface can be used for communication with SPI slave devices,
such as FLASH memory, LCD screens and other modules or integrated circuits.

In this example, we use config SPI_IS_AS_MASTER to decide if device is master or slave.
    If SPI_IS_AS_MASTER is 1, then device is master.
    If SPI_IS_AS_MASTER is 0, then device is slave.

We connect wires as below:
    master's MOSI (PA_4) connect to slave1's MOSI (PA_4) & slave2's MOSI (PA_4)
    master's MISO (PA_3) connect to slave1's MISO (PA_3) & slave2's MOSI (PA_3)
    master's SCLK (PA_1) connect to slave1's SCLK (PA_1) & slave2's MOSI (PA_1)
    master's SPI_GPIO_CS0   (PA_27) connect to slave1's CS  (PA_2) 
	master's SPI_GPIO_CS1   (PA_28) connect to slave2's CS  (PA_2) 

This example shows master sends data to multiple slaves.
User can configure several slave selected lines of the master 
In our demo code, one slave (CS0) would receive data in decreasing order in a loop.
The other slave (CS1) which is not selected by master receives nothing and generates rx timeout at the same time.
In the next loop, the second slave(CS_1) selected by the master receives data in increasing order while the first slave(CS0) receives nothing and generates rx timeout.

Note:spi_idx should be asigned first in the initialization process,We use MBED_SPI1 for Master and MBED_SPI0 for Slave