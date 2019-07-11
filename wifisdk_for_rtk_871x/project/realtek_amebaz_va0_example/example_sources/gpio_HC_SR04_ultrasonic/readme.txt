Example Description

This example describes how to use HC-SR04 ultrasonic.

Requirement Components:
    a HC-SR04 ultrasonic

HC-SR04 has 4 pins:
    VCC:  connect to 5V
    TRIG: connect to PA_3
    ECHO: connect to PA_1 (with level converter from 5V to 3.3V)
    GND:  Connect to GND

HC-SR04 use ultrasonic to raging distance.
We send a pulse HIGH on TRIG pin for more than 10us, 
then HC-SR04 return a pulse HIGH on ECHO pin which correspond distance.
The speed of sound wave is 340 m/s, which means it takes 29us for 1cm.
Thus the distance of result is:
    distance (in cm)
    = time (in us) / (29 * 2)


