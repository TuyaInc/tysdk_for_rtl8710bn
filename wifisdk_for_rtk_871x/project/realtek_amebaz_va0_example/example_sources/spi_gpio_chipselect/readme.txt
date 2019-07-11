Example Description

This example describes how to use another GPIO to replace CS line of SPI master.
This example provides the master code,users may choose SPI slave devices to communicate with.

The SPI Interface provides a "Serial Peripheral Interface" Master.

This interface can be used for communication with SPI slave devices,
such as FLASH memory, LCD screens and other modules or integrated circuits.

We connect wires as below:
    master's MOSI (PA_4) connect to slave's MOSI 
    master's MISO (PA_3) connect to slave's MISO 
    master's SCLK (PA_1) connect to slave's SCLK 
    master's SPI_GPIO_CS (PA_5) connect to slave's CS   

This example shows how users can use GPIO to replace SPI's default slave select pin for SPI master.
Users are allowed to select another availabe GPIO pin as a slave select line by defining the value of SPI_GPIO_CS
Before any master opeartion that requires the slave select to pull low, users should call the function gpio_write(&spi_cs, 0); 
as demonstrated in example code. Slave select line then pulls high by calling the function gpio_write(&spi_cs, 1);
in the interrupt function master_cs_tr_done_callback to indicate the operation is done.

Note:spi_idx should be asigned first in the initialization process,We use MBED_SPI1 for Master and MBED_SPI0 for Slave