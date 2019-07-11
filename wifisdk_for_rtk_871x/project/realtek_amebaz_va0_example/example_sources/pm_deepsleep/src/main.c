/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2015 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */

#include "device.h"
#include "gpio_api.h"   // mbed
#include "gpio_irq_api.h"   // mbed
#include "sleep_ex_api.h"
#include "sys_api.h"
#include "diag.h"
#include "main.h"

#define GPIO_LED_PIN			PA_0
#define GPIO_PUSHBT_PIN		PA_22

// deep sleep can only be waked up by A33 AND GPIO(PA_18, PA_5, PA_22, PA_23)
#define GPIO_WAKE_PIN		PA_5

void gpio_pull_control()
{
	/* please use pmap_func to config sleep gpio pull control */
}

void gpio_demo_irq_handler (uint32_t id, gpio_irq_event event)
{
    gpio_t *gpio_led;
    gpio_led = (gpio_t *)id;

    DBG_8195A("Enter deep sleep...Wait 10s or give 3.3V at PA_5 to wakeup system.\r\n\r\n");

    // turn off led
    gpio_write(gpio_led, 0);

    // turn off log uart
    //sys_log_uart_off();

    // initialize wakeup pin at PA_5
    gpio_t gpio_wake;
    gpio_init(&gpio_wake, GPIO_WAKE_PIN);
    gpio_dir(&gpio_wake, PIN_INPUT);
    gpio_mode(&gpio_wake, PullDown);

    // Please note that the pull control is different in different board
    // This example is a sample code for RTL Ameba Dev Board
    gpio_pull_control();

    // enter deep sleep
    deepsleep_ex(DSLEEP_WAKEUP_BY_GPIO | DSLEEP_WAKEUP_BY_TIMER, 10000);
}

void main(void)
{
    gpio_t gpio_led;
    gpio_irq_t gpio_btn;

    // Init LED control pin
    gpio_init(&gpio_led, GPIO_LED_PIN);
    gpio_dir(&gpio_led, PIN_OUTPUT);    // Direction: Output
    gpio_mode(&gpio_led, PullNone);     // No pull

    // Initial Push Button pin as interrupt source
    gpio_irq_init(&gpio_btn, GPIO_PUSHBT_PIN, gpio_demo_irq_handler, (uint32_t)(&gpio_led));
    gpio_mode((gpio_t*)&gpio_btn, PullUp);     // Pull up
    gpio_irq_set(&gpio_btn, IRQ_FALL, 1);   // Falling Edge Trigger
    gpio_irq_enable(&gpio_btn);

    // led on means system is in run mode
    gpio_write(&gpio_led, 1);
    DBG_8195A("\r\nPush button at PA_22 to enter deep sleep\r\n");

    while(1);
}
