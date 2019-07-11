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
#include "sleep_ex_api.h"
#include "diag.h"
#include "main.h"

#define GPIO_LED_PIN			PA_0
#define GPIO_PUSHBT_PIN		PA_22
#define GPIO_WAKE_PIN		PA_5

void gpio_pull_control()
{
	/* please use pmap_func to config sleep gpio pull control */
}

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void main(void)
{
    gpio_t gpio_led, gpio_btn, gpio_wake;
    int old_btn_state, new_btn_state;

    DBG_INFO_MSG_OFF(_DBG_GPIO_);

    // Init LED control pin
    gpio_init(&gpio_led, GPIO_LED_PIN);
    gpio_dir(&gpio_led, PIN_OUTPUT);    // Direction: Output
    gpio_mode(&gpio_led, PullNone);     // No pull

    // Initial Push Button pin
    gpio_init(&gpio_btn, GPIO_PUSHBT_PIN);
    gpio_dir(&gpio_btn, PIN_INPUT);     // Direction: Input
    gpio_mode(&gpio_btn, PullUp);

    // Initial Wake pin
    gpio_init(&gpio_wake, GPIO_WAKE_PIN);
    gpio_dir(&gpio_wake, PIN_INPUT);     // Direction: Input
    gpio_mode(&gpio_wake, PullUp);

    old_btn_state = new_btn_state = 0;
    gpio_write(&gpio_led, 1);

    DiagPrintf("Push button to enter deepstandby...\r\n");
    while(1){
        new_btn_state = gpio_read(&gpio_btn);

        if (old_btn_state == 1 && new_btn_state == 0) {
            gpio_write(&gpio_led, 0);

            DiagPrintf("Sleep 8s... (Or wakeup by giving failing edge at PA_5)\r\n");
            //turn off log uart to avoid warning in gpio_pull_control()
            //sys_log_uart_off();
            // Please note that the pull control is different in different board
            // This example is a sample code for RTL Ameba Dev Board
            gpio_pull_control();
            standby_wakeup_event_add(STANDBY_WAKEUP_BY_STIMER, 0);
            standby_wakeup_event_add(STANDBY_WAKEUP_BY_GPIO, WAKEUP_BY_GPIO_WAKEUP1_LOW);
            deepstandby_ex(8000);

            DiagPrintf("This line should not be printed\r\n");
        }
        old_btn_state = new_btn_state;
    }
}

