/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */
#include "device.h"
#include "gpio_api.h"   // mbed
#include "main.h"

#define DHT11 11
#define DHT22 22
#define DHT21 21

// define your DHT type
#define DHTTYPE DHT11

#define DHT_DATA_PIN PA_5

extern u8 GPIO_EXT_PORT_TBL[];
#define GPIO_DIRECT_READ(port,pin) ((GPIO->EXT_PORT[port]>> pin) & 0x01)

uint32_t expect_pulse(uint8_t port, uint8_t pin, uint32_t expect_level, uint32_t max_cycle) {
    uint32_t cycle = 1;
    while(expect_level == GPIO_DIRECT_READ(port, pin)) {
        if (cycle++ >= max_cycle) {
            return 0;
        }
    }
    return cycle;
}

void main(void)
{
    int i;
    uint32_t reg_value;
    uint32_t cycles[80];
    uint32_t low_cycles, high_cycles;

    uint8_t data[5];
    float   humidity = 0;
    float   temperature = 0;

    gpio_t gpio_dht;
    uint8_t port_num;
    uint8_t pin_num;

    gpio_init(&gpio_dht, DHT_DATA_PIN);
    gpio_dir(&gpio_dht, PIN_INPUT);
    gpio_mode(&gpio_dht, PullUp);

    port_num =  PORT_NUM(DHT_DATA_PIN);
    pin_num = PIN_NUM(DHT_DATA_PIN);

    while(1)
    {
        HalDelayUs(2 * 1000 * 1000);

        data[0] = data[1] = data[2] = data[3] = data[4] = 0;

        gpio_dir(&gpio_dht, PIN_OUTPUT);
        gpio_write(&gpio_dht, 1);
        HalDelayUs(250 * 1000);

        // toggle down to turn DHT from power saving mode to high speed mode
        gpio_write(&gpio_dht, 0);
        HalDelayUs(20 * 1000);
        gpio_write(&gpio_dht, 1);
        HalDelayUs(40);

        gpio_dir(&gpio_dht, PIN_INPUT);
        gpio_mode(&gpio_dht, PullNone);

        // wait DHT toggle down to ready
        if (expect_pulse(port_num, pin_num, 0, 1000) == 0) {
            rtl_printf("Timeout waiting for start signal low pulse.\r\n");
            continue;
        }
        if (expect_pulse(port_num, pin_num, 1, 1000) == 0) {
            rtl_printf("Timeout waiting for start signal high pulse.\r\n");
            continue;
        }

        for (i=0; i<80; i+=2) {
            cycles[i]   = expect_pulse(port_num, pin_num, 0, 1000);
            cycles[i+1] = expect_pulse(port_num, pin_num, 1, 1000);
        }

        for (i=0; i<40; i++) {
            low_cycles = cycles[2*i];
            high_cycles = cycles[2*i+1];
            if (low_cycles == 0 || high_cycles == 0) {
                break;
            }
            data[i/8] <<= 1;
            if (high_cycles > low_cycles) {
                data[i/8] |= 1;
            }
        }

        if (i != 40) {
            rtl_printf("Timeout waiting for pulse.\r\n");
            continue;
        }

        if ( ((data[0] + data[1] + data[2] + data[3]) & 0xFF) != data[4] ) {
            rtl_printf("Checksum failure!\r\n");
            continue;
        }

        switch(DHTTYPE) {
            case DHT11:
                humidity = data[0];
                temperature = data[2];
                break;
            case DHT22:
            case DHT21:
                humidity = data[0];
                humidity *= 256;
                humidity += data[1];
                humidity *= 0.1;
                temperature = data[2] & 0x7F;
                temperature *= 256;
                temperature += data[3];
                temperature *= 0.1;
                if (data[2] & 0x80) {
                    temperature *= -1;
                }
                break;
                
        }
        rtl_printf("Humidity: %.2f %%\t Temperature: %.2f *C\r\n", humidity, temperature);
    }
}