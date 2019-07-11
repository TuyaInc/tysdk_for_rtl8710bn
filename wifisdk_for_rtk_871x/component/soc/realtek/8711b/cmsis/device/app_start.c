/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */

#include "ameba_soc.h"
#include "build_info.h"

#if (defined(CONFIG_POST_SIM))
void Simulation_Init(void);
#endif

#if defined(CONFIG_WIFI_NORMAL) && defined(CONFIG_NETWORK)
extern void init_rom_wlan_ram_map(void);
extern VOID wlan_network(VOID);
#endif

#ifdef CONFIG_MBED_ENABLED
extern void __libc_fini_array (void);
extern void __libc_init_array (void);
extern  void SVC_Handler (void);
extern  void PendSV_Handler (void);
extern  void SysTick_Handler (void);

void APP_StartMbed(void)
{
	InterruptForOSInit((VOID*)SVC_Handler,
		(VOID*)PendSV_Handler,
		(VOID*)SysTick_Handler);
	__asm (
		"ldr   r0, =SystemInit\n"
		"blx   r0\n"
		"ldr   r0, =_start\n"
		"bx    r0\n"
	);

	for(;;);

}
#endif

void APP_InitTrace(void)
{
	u32 debug[4];

#if (defined(CONFIG_POST_SIM) || defined(CONFIG_CP))
	return;
#endif

	debug[LEVEL_ERROR] = BIT(MODULE_BOOT);
	debug[LEVEL_WARN]  = 0x0;
	debug[LEVEL_INFO]  = BIT(MODULE_BOOT);
	debug[LEVEL_TRACE] = 0x0;

#ifdef CONFIG_DEBUG_ERR_MSG
	debug[LEVEL_ERROR] = 0xFFFFFFFF;
#endif
#ifdef CONFIG_DEBUG_WARN_MSG
	debug[LEVEL_WARN] = 0xFFFFFFFF;
#endif
#ifdef CONFIG_DEBUG_INFO_MSG
	debug[LEVEL_INFO] = 0xFFFFFFFF;
#endif

	LOG_MASK(debug);

	DBG_PRINTF(MODULE_BOOT, LEVEL_INFO, "APP_InitTrace: %x:%x:%x:%x\n",debug[0], debug[1], debug[2], debug[3]);
	DBG_PRINTF(MODULE_BOOT, LEVEL_ERROR, "APP_InitTrace: %x:%x:%x:%x\n",debug[0], debug[1], debug[2], debug[3]);

}

//default main
_WEAK void main(void)
{
#if CONFIG_SOC_PS_MODULE	
	pmu_sysactive_timer_init();
#endif

#if (defined(CONFIG_POST_SIM))
	Simulation_Init();
#endif

	APP_InitTrace();
	
	PMAP_Init();

#ifndef CONFIG_WITHOUT_MONITOR
	ReRegisterPlatformLogUart();
#endif

#ifdef CONFIG_USB_DONGLE_NIC_EN
	if(xTaskCreate(USOC_Dongle_InitThread, ((const char*)"usb_dongle"), 1024, NULL, tskIDLE_PRIORITY + 3 + PRIORITIE_OFFSET, NULL) != pdPASS)
		DBG_8195A("\n\r%s xTaskCreate(usb_dongle_thread) failed", __FUNCTION__);
	goto end;
#endif

#if defined(CONFIG_WIFI_NORMAL) && defined(CONFIG_NETWORK)
#if (!defined(CONFIG_POST_SIM) && !defined(CONFIG_RTL_SIM) && !defined(CONFIG_FT) && !defined(CONFIG_CP))
	wlan_network();
#endif
#else

#endif  // end of else of "#if defined(CONFIG_WIFI_NORMAL) && defined(CONFIG_NETWORK)"

#if CONFIG_INIC_EN
#if SUPPORT_LOG_SERVICE
	
	log_service_init();
#endif

	inic_interface_init();
#endif

end:
	//3 4)Enable Schedule
#if defined(CONFIG_KERNEL) && !TASK_SCHEDULER_DISABLED
#ifdef PLATFORM_FREERTOS
	vTaskStartScheduler();
#endif
#else
	RtlConsolTaskRom(NULL);
#endif
}

// The Main App entry point
void APP_Start(void)
{
#if CONFIG_SOC_PS_MODULE
	SOCPS_InitSYSIRQ();
#endif

#ifdef CONFIG_KERNEL
#ifdef PLATFORM_FREERTOS
	InterruptForOSInit((VOID*)vPortSVCHandler,
		(VOID*)xPortPendSVHandler,
		(VOID*)xPortSysTickHandler);
#endif
#endif

#ifdef CONFIG_MBED_ENABLED
	APP_StartMbed();
#else

#if 0//def CONFIG_APP_DEMO
#ifdef PLATFORM_FREERTOS
	xTaskCreate( (TaskFunction_t)main, "MAIN_APP__TASK", (2048 /4), (void *)NULL, (tskIDLE_PRIORITY + 1), NULL);
	vTaskStartScheduler();
#endif
#else
#if defined ( __ICCARM__ )
	__iar_cstart_call_ctors(NULL);
#endif
	// force SP align to 8 byte not 4 byte (initial SP is 4 byte align)
	__asm( 
		"mov r0, sp\n"
		"bic r0, r0, #7\n" 
		"mov sp, r0\n"
	);

	main();
#endif // end of #if CONFIG_APP_DEMO
#endif  // end of else of "#ifdef CONFIG_MBED_ENABLED"
}
