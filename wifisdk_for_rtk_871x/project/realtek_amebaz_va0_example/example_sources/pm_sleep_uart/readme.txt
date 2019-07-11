Example Description

This example describes how to use sleep uart api.

Requirement Components:
    a USB to TTL Adapter

Pin name PA_18 and PA_23 map to GPIOA_18 and GPIOA_23:
 - PA_18 as UART0_RX connect to TX of USB to TTL adapter.
 - PA_23 as UART0_TX connect to RX of USB to TTL adapter.
 - GND connect to GND of USB to TTL adapter.

Open the Trace Tool and set baud rate to ¡°115200¡±,Data Bits ¡°8¡±, no parity, ¡± 1¡± stop bit for mbed device
Open the Trace Tool and set baud rate to ¡°38400¡±, Data Bits ¡°8¡±, no parity, ¡± 1¡± stop bit for Usb to TTL Adapter.

Boot up device, and wait around 5 seconds, device will enter sleep mode.
Then you can press the ¡°Enter¡± key or type characters to wakeup system and the characters will be shown in the Trace Tool for Usb to TTL Adapter.

Note: user need to ENABLE the LOW_POWER_RX_ENABLE in array uart_config[] in rtl8710b_intfcfg.c to enable uart low power rx function
      and ON the wakeup event "BIT_SYSON_WEVT_UART0_MSK" in array sleep_wevent_config[] in rtl8710b_sleepcfg.c to enable uart wakeup event before make.