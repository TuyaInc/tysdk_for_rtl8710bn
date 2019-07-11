Example Description

This example describes how to use sleep api.

Requirement Components:
    a LED
    a push button
    a wakeup pin

Pin name PA_0, PA_5 and PA_22 map to GPIOA_0, GPIOA_5 and GPIOA_22:
 - PA_5 as input to wakeup the system
 - PA_0 as output, connect a LED to this pin and ground.
 - PA_22 as input with internal pull-high, connect a push button to this pin and ground.

In this example, LED is turned on after device initialize.
User push the button to turn off LED and trigger device enter sleep mode for 8s.
If user give failing edge at PA_5 before sleep timeout, the system will resume.
LED is turned on again after system resume without restart PC.

It can be easily measure power consumption in normal mode and sleep mode before/after push the putton.
