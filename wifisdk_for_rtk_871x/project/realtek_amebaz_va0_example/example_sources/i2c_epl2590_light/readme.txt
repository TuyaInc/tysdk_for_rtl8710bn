Example Description

This example describes how to use proximity sensor to detect lightness

Requirement Components:
    extend board

work with arduino extended board, which has proximity sensor

when the proximity sensor is in ALS mode (detect lightness), it will keep polling lightness output.

Connect 
  - I2C0 SDA (PA_19) to extended board's SDA 
  - I2C0 SCL (PA_22) to extended board's SCL 
  