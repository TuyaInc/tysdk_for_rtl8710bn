Example Description

This example describes how to use ADC normal channel.

Prcedure:
	1.Plug in macro USB to DUT.
	2.Run main code for the test.
	3.Make connection via DuPont Line  as specified in the above figure:
		1)	AD0 GND: Make a DuPont Line to A19 and ground.
		2)	AD0 3.3V: Make a DuPont Line to A19 and 3.3V.
		3)	AD2 GND: Make a DuPont Line to A20 and ground.
		4)	AD2 3.3V: Make a DuPont Line to A20 and 3.3V.
	4.Record the all ADC output values.
		AD0:4131 = -5 mv, AD2:8096 = 1209 mv
		AD0:4132 = -5 mv, AD2:8090 = 1209 mv
		AD0:4133 = -5 mv, AD2:8094 = 1209 mv
		AD0:4133 = -5 mv, AD2:8094 = 1209 mv

Results:
	1.	ADC0 GND: 
		1) AD0 voltage value approaches 0
		2) AD1 voltage value  is around  184mv
		3) AD2 voltage value  is around  1200mv
	2.	ADC0 3.3v: 
		1) AD0 voltage value approaches 3300mv
		2) AD1 voltage value is around  184mv
		3) AD2 voltage value is around  1200mv
	3.	ADC1 GND: 
		1) AD0 voltage value is around  1200mv
		2) AD1 voltage value is around  184mv
		3) AD2 voltage value approaches 0
	4.	ADC1 3.3v: 
		1) AD0 voltage value is around  1200mv
		2) AD1 voltage value is around  184mv
		3) AD2 voltage value approaches 3300mv


NOTE: 
	1. For 8710BN EVB, AD0 and AD1 are available. AD2 is not avaliable.
	   For 8711BN EVB, AD0 and AD2 are available. AD1 is not available.
	   For 8711BG EVB, AD0 and AD1 and AD2 are all avaliable.
	   
	2. AD1(VBAT) calibration parameters(gain & offset) are different with normal channel need modify OFFSET and GAIN_DI Vcalibration to get correct voltage value.



