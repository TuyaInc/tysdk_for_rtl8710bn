Example Description

This example describes how to use DHT11/DHT22/DHT21 temperature and humidity sensor.
Since DHT require micorseconds level gpio operation, we use gpio register to perform gpio read.

Requirement Components:
    DHT11/DHT22/DHT21

DHT series may have 3 pin or 4 pin product.

3 pin DHT has pin layout:
	┌─┬┬┬┬┬┬─ GND
	│ ├┼┼┼┼┤
	├─┼┼┼┼┼┼─ 3.3V
	│ ├┼┼┼┼┤
	└─┴┴┴┴┴┴─ DATA

4 pin DHT has pin layout:

	┌─┬┬┬┬┬┐ 
	│ ├┼┼┼┼┼─ GND
	├┬┼┼┼┼┼┼─ N/A
	├┴┼┼┼┼┼┼─ DATA
	│ ├┼┼┼┼┼─ 3.3V
	└─┴┴┴┴┴┘

All we need is 3.3V, GND, and DATA (connect to PA_5).
DATA has default level high.

To get data, it has 3 stage:
(1) Turn DHT from power saving to high speed mode:
    Ameba toggle low on DATA pin
(2) Wait DHT ready:
    DHT toggle low on DATA pin
(3) Repeatly get 40 bits of data:
	If level high has shorter length than level low, then it's bit 0.
	If level high has longer length than level high, then it's bit 1.

             _____          _____________
	________/     \________/             \_________
             bit 0          bit 1

