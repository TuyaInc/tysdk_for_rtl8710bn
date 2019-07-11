#include <FreeRTOS.h>
#include <task.h>
#include <platform/platform_stdlib.h>
#include <basic_types.h>
#include <platform_opts.h>
#include <dct/dct.h>

#if CONFIG_EXAMPLE_DCT

#define DCT_BEGIN_ADDR			0x100000	/*!< DCT begin address of flash, ex: 0x100000 = 1M */
#define	MODULE_NUM				6			/*!< max number of module */
#define VARIABLE_NAME_SIZE		32			/*!< max size of the variable name */
#define VARIABLE_VALUE_SIZE		32			/*!< max size of the variable value */

static char example_dct_module[] = "dct_test_module";
static char example_dct_variable0[] = "variable0";
static char example_dct_variable1[] = "variable1";
static char example_dct_value0[] = "value0";
static char example_dct_value1[] = "value1";

void example_dct_thread(void* param){
	int32_t			ret = -1;
	dct_handle_t	dct_handle;
	char			value[16];
	
	// format DCT, use for the first time
	ret = dct_format(DCT_BEGIN_ADDR, MODULE_NUM, VARIABLE_NAME_SIZE, VARIABLE_VALUE_SIZE, 1);
		
	// initial DCT
	ret = dct_init(DCT_BEGIN_ADDR);
	
	// register module
	ret = dct_register_module(example_dct_module);
	
	// open module
	ret = dct_open_module(&dct_handle, example_dct_module);
	
	if(ret == DCT_SUCCESS){
		// set test variable 0
		ret = dct_set_variable(&dct_handle, example_dct_variable0, example_dct_value0);
		
		// set test variable 1
		ret = dct_set_variable(&dct_handle, example_dct_variable1, example_dct_value1);
		
		// get value of test variable 0
		memset(value, 0, sizeof(value));
		ret = dct_get_variable(&dct_handle, example_dct_variable0, value, sizeof(value));
		if(ret == DCT_SUCCESS)
			printf("%s: %s\n", example_dct_variable0, value);
			
		// get value of test variable 1
		memset(value, 0, sizeof(value));
		ret = dct_get_variable(&dct_handle, example_dct_variable1, value, sizeof(value));
		if(ret == DCT_SUCCESS)
			printf("%s: %s\n", example_dct_variable1, value);
			
		// delete test variable 0
		ret = dct_delete_variable(&dct_handle, example_dct_variable0);
		
		// get value of test variable 0
		memset(value, 0, sizeof(value));
		ret = dct_get_variable(&dct_handle, example_dct_variable0, value, sizeof(value));
		if(ret == DCT_ERR_NOT_FIND)
			printf("Delete %s success.\n", example_dct_variable0);
		
		// get variable remaining amount
		ret = dct_remain_variable(&dct_handle);
		if(ret > 0)
			printf("Remaining variable amount:%d\n", ret);
			
		// close module
		ret = dct_close_module(&dct_handle);
	}
	
	vTaskDelete(NULL);
}


void example_dct(void)
{
	if(xTaskCreate(example_dct_thread, ((const char*)"example_dct_thread"), 1024, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS)
		printf("\n\r%s xTaskCreate(example_dct_thread) failed", __FUNCTION__);
}

#endif // #if CONFIG_DCT
