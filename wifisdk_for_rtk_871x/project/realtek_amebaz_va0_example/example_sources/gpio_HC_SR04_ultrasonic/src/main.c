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
#include "gpio_irq_api.h"   // mbed
#include "gpio_irq_ex_api.h"
#include "diag.h"
#include "main.h"

#define HC_SR04_TRIG        PA_3
#define HC_SR04_ECHO        PA_1

/* Speed of sound is around 340 m/s
 * So 1cm taks 1000000 / 340 * 100 = 29 us
 */
#define TIME_COST_OF_ONE_CM 29  // unit is microsecond

gpio_t gpio_trig;
gpio_irq_t gpio_irq_echo;
int current_level = IRQ_HIGH;

uint32_t timestamp_ping = 0;
uint32_t timestamp_pong = 0;

void gpio_demo_irq_handler (uint32_t id, gpio_irq_event event)
{
    uint32_t echo_time;
    uint32_t dist_cm;
    uint32_t dist_mm;

    // Disable level irq because the irq will keep triggered when it keeps in same level.
    gpio_irq_disable(&gpio_irq_echo);

    if (current_level == IRQ_LOW )
    {
        timestamp_pong = us_ticker_read();

        echo_time = (timestamp_pong - timestamp_ping);
        dist_cm = echo_time / ( TIME_COST_OF_ONE_CM * 2 );
        dist_mm = ( echo_time % ( TIME_COST_OF_ONE_CM * 2 ) * 10 ) / ( TIME_COST_OF_ONE_CM * 2 );

        DiagPrintf("%d.%d cm\r\n", dist_cm, dist_mm);

        // Change to listen to high level event
        current_level = IRQ_HIGH;
        gpio_irq_set(&gpio_irq_echo, current_level, 1);
        gpio_irq_enable(&gpio_irq_echo);
    }
    else if (current_level == IRQ_HIGH)
    {
        timestamp_ping = us_ticker_read();

        // Change to listen to low level event
        current_level = IRQ_LOW;
        gpio_irq_set(&gpio_irq_echo, current_level, 1);
        gpio_irq_enable(&gpio_irq_echo);
    }
}

void trigger_high(gpio_t *gpio, uint32_t us)
{
    gpio_write(gpio, 1);
    HalDelayUs(us);
    gpio_write(gpio, 0);
}

void main(void)
{
    // Initial HC-SR04 Trigger pin
    gpio_init(&gpio_trig, HC_SR04_TRIG);
    gpio_dir(&gpio_trig, PIN_OUTPUT);    // Direction: Output
    gpio_mode(&gpio_trig, PullNone);     // No pull
    gpio_write(&gpio_trig, 0);

    // Initial HC-SR04 Echo pin
    gpio_irq_init(&gpio_irq_echo, HC_SR04_ECHO, gpio_demo_irq_handler, NULL);
    gpio_irq_set(&gpio_irq_echo, current_level, 1);
    gpio_irq_enable(&gpio_irq_echo);

    while(1) {
        // trigger event by sending High level signal for 10us
        trigger_high(&gpio_trig, 10);

        // delay 2s for next calculation
        HalDelayUs(2 * 1000 * 1000);
    }
}

