This example demonstrates how to use mbed-CoAP C library to build and parse a CoAP message.

In the example, a confirmable GET request is send to test server "coap.me" to retrieve the resource under path "/hello". The expected return is an ACK message with payload "world".

Note:
Company Firewall may block CoAP message. You can use copper (https://addons.mozilla.org/en-US/firefox/addon/copper-270430/) to test the server's reachability.