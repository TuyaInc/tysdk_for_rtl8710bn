Example Description

This example describes how to use GPIO Port read/write by mbed api.

Requirement Components:
    8 LEDs  
	dupont line

Pin name A0 map to GPIOA_0:	
   -For 32pin test, the available Gpio port are A0, A5, A12, A18, A19, A22, A23, and A14,A15 are also available when turning off the JTAG function.
   -GPIO ports as output pins connect to the LED long leg, and the LED short leg connect to GND.
   
In this example:
When test Gpio port output, the LEDs is on when corresponding Gpio pins output 1, the LEDs is off when corresponding Gpio pins output 0; 
When test Gpio port input, current selected Gpio pins input value is different with original value, trace tool print current Gpio pins value.


