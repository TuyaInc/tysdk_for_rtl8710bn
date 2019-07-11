Example Description

It basically verifies hardware encryption/decryption functions and show results on the LOG_OUT.

1. Plug in macro USB to DUT.
2. use Arduino board to test, and it will show at console
3. check if the console result is right.

The expected result is in the serial terminal which has logs as below:
sleep 10 sec. to wait for UART console
CRYPTO API Demo...
MD5 test
MD5 test result is correct, ret=0
  MD5 test #1:  MD5 ret=0
passed
  MD5 test #2:  MD5 ret=0
passed
  MD5 test #3:  MD5 ret=0
passed
  MD5 test #4:  MD5 ret=0
passed
  MD5 test #5:  MD5 ret=0
passed
  MD5 test #6:  MD5 ret=0
passed
  MD5 test #7:  MD5 ret=0
passed
  MD5 test #8:  MD5 ret=0
passed
  MD5 test #9:  MD5 ret=0
passed
  MD5 test #10:  MD5 ret=0
passed
  MD5 test #11:  MD5 ret=0
passed
  MD5 test #12:  MD5 ret=0
passed
  MD5 test #13:  MD5 ret=0
passed
  MD5 test #14:  MD5 ret=0
passed
  MD5 test #15:  MD5 ret=0
passed
  MD5 test #16:  MD5 ret=0
passed
AES CBC test
AES CBC encrypt result success
AES CBC decrypt result success



