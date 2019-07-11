Example Description

This example describes how to use tickless suspend resume api.

Requirement Components:
    a USB to TTL Adapter

Pin name PA_18 and PA_23 map to GPIOA_18 and GPIOA_23:
 - PA_18 as UART0_RX connect to TX of USB to TTL adapter.
 - PA_23 as UART0_TX connect to RX of USB to TTL adapter.
 - GND connect to GND of USB to TTL adapter.

Open the Trace Tool and set baud rate to ¡°115200¡±,Data Bits ¡°8¡±, no parity, ¡± 1¡± stop bit for mbed device
Open the Trace Tool and set baud rate to ¡°38400¡±, Data Bits ¡°8¡±, no parity, ¡± 1¡± stop bit for Usb to TTL Adapter.

Operating process: 
 - Boot up device, and wait around 5 seconds, device will enter sleep mode, and the registered suspend function will called automatically.
 - Then you can press the ¡°Enter¡± key or type characters to wakeup system , the registered resume function will called automatically. and the characters will be shown in the Trace Tool for Usb to TTL Adapter.
 - Then the registered late_resume function will called automatically.
 - After 5 seconds system will enter sleep again



