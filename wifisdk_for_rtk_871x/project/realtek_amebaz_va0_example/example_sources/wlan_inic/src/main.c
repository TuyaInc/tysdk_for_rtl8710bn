#include "FreeRTOS.h"
#include "task.h"
#include "diag.h"
#include "main.h"
#include "freertos_pmu.h"

extern void console_init(void);
extern void inic_interface_init(void);
extern void rtw_wowlan_init(void);

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void main(void)
{
 	console_init();	
	
#if CONFIG_INIC_EN

	inic_interface_init();

#if CONFIG_WOWLAN_SERVICE
	rtw_wowlan_init();
#endif
		
	// By default tickless is disabled because WAKELOCK_OS is locked.
	// Release this wakelock to enable tickless
#if defined(configUSE_WAKELOCK_PMU) && (configUSE_WAKELOCK_PMU == 1)
	pmu_acquire_wakelock(PMU_SDIO_DEVICE);
	pmu_release_wakelock(PMU_OS);        
#endif
		
#endif //endof #if CONFIG_INIC_EN
        
    //3 3)Enable Schedule, Start Kernel
#if defined(CONFIG_KERNEL) && !TASK_SCHEDULER_DISABLED
	#ifdef PLATFORM_FREERTOS
	vTaskStartScheduler();
	#endif
#else
	RtlConsolTaskRom(NULL);
#endif
}
