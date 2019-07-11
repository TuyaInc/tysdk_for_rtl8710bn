#include "FreeRTOS.h"
#include "task.h"
#include "diag.h"
#include "main.h"
#include "freertos_pmu.h"
#include "hal_platform.h"

#define TUYA_CONSOLE 0
extern void console_init(void);

static void app_init_thread(void *param)
{
 
    HAL_WRITE32(0x40000020, 0, 0x16c08195);


    extern void user_main(void); // user entry
    user_main();

    /* Kill init thread after all init tasks done */
    vTaskDelete(NULL);
}
void wait_wifi_semaphore(void)
{
    #include "semphr.h" 
    extern xSemaphoreHandle sync_sem; // define in wlan_network.c by nzy 
    
    // waiting to intial sync
    if(sync_sem) {
        xSemaphoreTake(sync_sem, portMAX_DELAY);
        vSemaphoreDelete(sync_sem);
        sync_sem = NULL;
    }
}
static void app_init_entry(void)
{
    if(xTaskCreate(app_init_thread, ((const char*)"app_init"), 1024*3, NULL, tskIDLE_PRIORITY + 3 + PRIORITIE_OFFSET, NULL) != pdPASS)
        printf("\n\r%s xTaskCreate(init_thread) failed", __FUNCTION__);
}

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void main(void)
{
	if ( rtl_cryptoEngine_init() != 0 ) {
		DiagPrintf("crypto engine init failed\r\n");
	}

	/* Initialize log uart and at command service */
    #if (TUYA_CONSOLE) || (MP)
	//console_init();	
	ReRegisterPlatformLogUart();
    #endif

	/* pre-processor of application example */
	pre_example_entry();

	/* wlan intialization */
#if defined(CONFIG_WIFI_NORMAL) && defined(CONFIG_NETWORK)
	wlan_network();
#endif


    //set tickless sleep type to SLEEP_CG lql 
	pmu_set_sleep_type(SLEEP_CG);
	pmu_sysactive_timer_init();
	//pmu_set_sysactive_time(5000);

    #if !MP
	/* application init */
	app_init_entry();
    #endif

	/*Enable Schedule, Start Kernel*/
#if defined(CONFIG_KERNEL) && !TASK_SCHEDULER_DISABLED
	#ifdef PLATFORM_FREERTOS
	vTaskStartScheduler();
	#endif
#else
    #if (TUYA_CONSOLE) || (MP)
	RtlConsolTaskRom(NULL);
	#endif
#endif
}
