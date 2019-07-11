Example Description

This example describes how to use Bor2 Brown-Out Reset.

Requirement Components:
    a USB to TTL Adapter

Operating process: 
 - Remove R43 on the demo board
 - Give 3.3V at pin that near the chip of J34 by power supply.
 - Boot up device, and you will see the log"Supply 2.6V-3.0V voltage!!!"
 - Change 3.3V to 2.6V-3.0V to trigger Bor2 Interrupt,and will call the registered "bor_intr_Handler"
 - Recover voltage to 3.3V


Note: 
 - Never give 3.3V at the other pin of J34


