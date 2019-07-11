Example Description

This example describes how to use UART to communicate with PC.

Required Components:
    USBtoTTL adapter

Connect to PC
 - Connect Ground: connect to GND pin via USBtoTTL adapter
 - Use UART0
	GPIOA_18 as UART0_RX connect to TX of USBtoTTL adapter
	GPIOA_23 as UART0_TX connect to RX of USBtoTTL adapter

Open Super terminal or teraterm and 
set baud rate to 38400, 1 stopbit, no parity, no flow contorl.

This example shows:
1. The RX data ready interrupt service routine is used to receive 8 characters from the PC, and then loopback them to the PC.
