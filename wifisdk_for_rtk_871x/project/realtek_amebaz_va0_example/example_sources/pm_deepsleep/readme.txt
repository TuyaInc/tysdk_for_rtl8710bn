Example Description

This example describes how to use deep sleep api.

Requirement Components:
    a LED
    a push button
    a wakeup pin

Pin name PA_0, PA_5 and PA_22 map to GPIOA_0, GPIOA_5 and GPIOA_22:
 - PA_5 as input to wakeup the system
 - PA_0 as output, connect a LED to this pin and ground.
 - PA_22 as input with internal pull-high, connect a push button to this pin and ground.

In this example, LED is turned on after device initialize.
User push the button to turn off LED and trigger device enter deep sleep mode for 10s.
If user give 3.3V at PA_5 before sleep timeout, the system will resume.
LED is turned on again after device initialize.

It can be easily measure power consumption in normal mode and deep sleep mode before/after push the putton.

NOTE:Before trigger device enter deep sleep mode, should connect PA_5 to GND to avoid incorrectly waking up by floating