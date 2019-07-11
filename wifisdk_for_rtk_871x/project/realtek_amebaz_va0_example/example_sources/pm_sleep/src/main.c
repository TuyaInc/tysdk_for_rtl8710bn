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
#define GPIO_WAKE_PIN		PA_5

volatile int led_ctrl = 0;
gpio_t gpio_led;

volatile int put_to_sleep = 0;

void gpio_demo_irq_handler (uint32_t id, gpio_irq_event event)
{
    gpio_t *gpio_led;

    gpio_led = (gpio_t *)id;
    led_ctrl = 0;
    gpio_write(gpio_led, led_ctrl);
    put_to_sleep = 1;
}

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void psm_sleep(void)
{
    gpio_irq_t gpio_btn;
    gpio_t gpio_wake;
    //int IsDramOn = 1;

    DBG_INFO_MSG_OFF(_DBG_GPIO_);

    // Init LED control pin
    gpio_init(&gpio_led, GPIO_LED_PIN);
    gpio_dir(&gpio_led, PIN_OUTPUT);    // Direction: Output
    gpio_mode(&gpio_led, PullNone);     // No pull

    // Initial Push Button pin as interrupt source
    gpio_irq_init(&gpio_btn, GPIO_PUSHBT_PIN, gpio_demo_irq_handler, (uint32_t)(&gpio_led));
    gpio_mode((gpio_t*)&gpio_btn, PullUp); 	// Pull up
    gpio_irq_set(&gpio_btn, IRQ_FALL, 1);
    gpio_irq_enable(&gpio_btn);

    gpio_init(&gpio_wake, GPIO_WAKE_PIN);
    gpio_dir(&gpio_wake, PIN_INPUT);    // Direction: Input
    gpio_mode(&gpio_wake, PullUp);     // Pull up

    led_ctrl = 1;
    gpio_write(&gpio_led, led_ctrl);
    DBG_8195A("Push button to enter sleep\r\n");
    //system will hang when it tries to suspend SDRAM for 8711AF
    //if ( sys_is_sdram_power_on() == 0 ) {
    //    IsDramOn = 0;
    //}

    put_to_sleep = 0;
    while(1) {
        if (put_to_sleep) {
            DBG_8195A("Sleep 8s or give failing edge at PA_5 to resume system...\r\n");
            //sys_log_uart_off();
            gpio_irq_disable(&gpio_btn);

            sleep_ex(SLEEP_WAKEUP_BY_GPIO_INT | SLEEP_WAKEUP_BY_STIMER, 8000); // sleep_ex can't be put in irq handler
            //sys_log_uart_on();
            DBG_8195A("System resume\r\n");

            gpio_irq_enable(&gpio_btn);
            put_to_sleep = 0;
            led_ctrl = 1;
            gpio_write(&gpio_led, led_ctrl);
        }
    }
	vTaskDelete(NULL);
}

void main(void)
{
	// create demo Task
	if(xTaskCreate( (TaskFunction_t)psm_sleep, "UART WAKEUP DEMO", (2048/4), (void *)&gpio_led, (tskIDLE_PRIORITY + 1), NULL)!= pdPASS) {
		DBG_8195A("Cannot create uart wakeup demo task\n\r");
		goto end_demo;
	}

#if defined(CONFIG_KERNEL) && !TASK_SCHEDULER_DISABLED
	#ifdef PLATFORM_FREERTOS
	vTaskStartScheduler();
	#endif
#else
	#error !!!Need FREERTOS!!!
#endif

end_demo:	
	while(1);
}


