Example Description

This example describes how to use proximity sensor to detect distance

Requirement Components:
    extend board

work with arduino extended board, which has proximity sensor

When the proximity sensor is in PS mode (detect distance), if the object is close to the sensor, a near message will print out. Otherwise a far message will print out.
  
Connect 
  - I2C0 SDA (PA_19) to extended board's SDA 
  - I2C0 SCL (PA_22) to extended board's SCL 
  
