/***********************************************************
*  File: tuya_gpio_test.c
*  Author: lql
*  Date: 20180502
***********************************************************/

#include "adapter_platform.h"
#include "tuya_gpio.h"
#include "uni_log.h"
#include "sys_api.h"
#include "gpio_test.h"



typedef struct
{
    INT_T   ionum;
    TY_GPIO_PORT_E iopin[8];
}CTRL_GROUP;

typedef struct
{
    INT_T group_num;
    CTRL_GROUP *group;
}GPIO_TEST_TABLE;

STATIC CTRL_GROUP groups_wr1[] = 
{
    // group 0
    {
        //
        .ionum  = 2,
        //  TY_GPIOA_19,
        .iopin = {TY_GPIOA_15, TY_GPIOA_22}
    },

     // group 1
    {
        // 继电器
        .ionum  = 2,
        // 按钮
        .iopin = {TY_GPIOA_14, TY_GPIOA_29}
    },

     // group 2
    {
        // 继电器
        .ionum  = 2,
        // 按钮
        .iopin = {TY_GPIOA_0, TY_GPIOA_5}
    },

     // group 3
    {
        // 继电器
        .ionum  = 2,
        // 按钮
        .iopin = {TY_GPIOA_19, TY_GPIOA_30}
    },

};

STATIC CTRL_GROUP groups_wr2[] =   //wr6
{
    // group 0
    {
        //
        .ionum  = 3,
        .iopin = {TY_GPIOA_0,TY_GPIOA_5, TY_GPIOA_15}
    },

     // group 1
    {
        // 继电器
        .ionum  = 0,
        // 按钮
        .iopin = {}
    },

     // group 2
    {
        // 继电器
        .ionum  = 2,
        // 按钮
        .iopin = {TY_GPIOA_14, TY_GPIOA_29}
    },

     // group 3
    {
        // 继电器
        .ionum  = 2,
        // 按钮
        .iopin = {TY_GPIOA_12, TY_GPIOA_30}
    },

};

STATIC CTRL_GROUP groups_wr3[] =   //wr5
{
    // group 0
    {
        //
        .ionum  = 3,
        //  TY_GPIOA_19,
        .iopin = {TY_GPIOA_15, TY_GPIOA_19, TY_GPIOA_22}
    },

     // group 1
    {
        // 继电器
        .ionum  = 2,
        // 按钮
        .iopin = {TY_GPIOA_14, TY_GPIOA_29}
    },

     // group 2
    {
        // 继电器
        .ionum  = 2,
        // 按钮
        .iopin = {TY_GPIOA_0, TY_GPIOA_5}
    },

     // group 3
    {
        // 继电器
        .ionum  = 2,
        // 按钮
        .iopin = {TY_GPIOA_12, TY_GPIOA_30}
    },

};

STATIC CTRL_GROUP groups_wr4[] = 
{
    // group 0
    {
        //
        .ionum  = 3,
        //  TY_GPIOA_19,
        .iopin = {TY_GPIOA_12, TY_GPIOA_15, TY_GPIOA_22}
    },

     // group 1
    {
        // 继电器
        .ionum  = 2,
        // 按钮
        .iopin = {TY_GPIOA_14, TY_GPIOA_29}
    },

     // group 2
    {
        // 继电器
        .ionum  = 2,
        // 按钮
        .iopin = {TY_GPIOA_0, TY_GPIOA_5}
    },

     // group 3
    {
        // 继电器
        .ionum  = 2,
        // 按钮
        .iopin = {TY_GPIOA_19, TY_GPIOA_30}
    },

};

STATIC CTRL_GROUP groups_wr7[] = //WF_R710_RWD1
{
    // group 0
    {
        //
        .ionum  = 0,
        //  TY_GPIOA_19,
        .iopin = {}
    },

     // group 1
    {
        // 继电器
        .ionum  = 2,
        // 按钮
        .iopin = {TY_GPIOA_14, TY_GPIOA_29}
    },

     // group 2
    {
        // 继电器
        .ionum  = 2,
        // 按钮
        .iopin = {TY_GPIOA_5, TY_GPIOA_15}
    },

     // group 3
    {
        // 继电器
        .ionum  = 2,
        // 按钮
        .iopin = {TY_GPIOA_12, TY_GPIOA_30}
    },

};
 
STATIC CTRL_GROUP groups_wr8[] = //TYAUX_F
{
    // group 0
    {
        //
        .ionum  = 3,
        //  
        .iopin = {TY_GPIOA_19,TY_GPIOA_22,TY_GPIOA_29}
    },

     // group 1
    {
        // 继电器
        .ionum  = 0,
        // 按钮
        .iopin = {}
    },

     // group 2
    {
        // 继电器
        .ionum  = 2,
        // 按钮
        .iopin = {TY_GPIOA_0, TY_GPIOA_5}
    },

     // group 3
    {
        // 继电器
        .ionum  = 2,
        // 按钮
        .iopin = {TY_GPIOA_12, TY_GPIOA_30}
    },

};

STATIC CTRL_GROUP groups_wr9[] = //WR2L
{
    // group 0
    {
        //
        .ionum  = 3,
        //  
        .iopin = {TY_GPIOA_5,TY_GPIOA_12,TY_GPIOA_15}
    },

     // group 1
    {
        // 继电器
        .ionum  = 0,
        // 按钮
        .iopin = {}
    },

     // group 2
    {
        // 继电器
        .ionum  = 2,
        // 按钮
        .iopin = {TY_GPIOA_14, TY_GPIOA_19}
    },

     // group 3
    {
        // 继电器
        .ionum  = 0,
        // 按钮
        .iopin = {}
    },

};

STATIC CTRL_GROUP groups_wr10[] = //WR1E
{
    // group 0
    {
        //
        .ionum  = 2,
        //  
        .iopin = {TY_GPIOA_15,TY_GPIOA_22}
    },

     // group 1
    {
        // 继电器
        .ionum  = 2,
        // 按钮
        .iopin = {TY_GPIOA_14,TY_GPIOA_29}
    },

     // group 2
    {
        // 继电器
        .ionum  = 2,
        // 按钮
        .iopin = {TY_GPIOA_5,TY_GPIOA_19}
    },

     // group 3
    {
        // 继电器
        .ionum  = 2,
        // 按钮
        .iopin = {TY_GPIOA_12, TY_GPIOA_30}
    },

};

STATIC CTRL_GROUP groups_wr11[] = //WR2E
{
    // group 0
    {
        //
        .ionum  = 3,
        //  
        .iopin = {TY_GPIOA_5,TY_GPIOA_12,TY_GPIOA_19}
    },

     // group 1
    {
        // 继电器
        .ionum  = 2,
        // 按钮
        .iopin = {TY_GPIOA_14,TY_GPIOA_15}
    },

     // group 2
    {
        // 继电器
        .ionum  = 0,
        // 按钮
        .iopin = {}
    },

     // group 3
    {
        // 继电器
        .ionum  = 0,
        // 按钮
        .iopin = {}
    },

};

STATIC CTRL_GROUP groups_wr12[] = //RLC2VE
{
    // group 0
    {
        //
        .ionum  = 3,
        //  
        .iopin = {TY_GPIOA_5,TY_GPIOA_15,TY_GPIOA_22}
    },

     // group 1
    {
        // 继电器
        .ionum  = 2,
        // 按钮
        .iopin = {TY_GPIOA_14,TY_GPIOA_29}
    },

     // group 2
    {
        // 继电器
        .ionum  = 0,
        // 按钮
        .iopin = {}
    },

     // group 3
    {
        // 继电器
        .ionum  = 2,
        // 按钮
        .iopin = {TY_GPIOA_12,TY_GPIOA_30}
    },

};







STATIC GPIO_TEST_TABLE gpio_test_table = {
    .group_num = 4,
    .group = NULL
};

BOOL_T gpio_test_cb(BOARD_TYPE type)
{
	if((type < RTL_BOARD_WR1) || (type >= RTL_BOARD_MAX)) {
		PR_ERR("board type is out of range[%d]",type);
		return FALSE;
	}
	
	if(type == RTL_BOARD_WR1) {
		gpio_test_table.group = groups_wr1;
	}else if((type == RTL_BOARD_WR2) || (type == RTL_BOARD_WR6)) {
		gpio_test_table.group = groups_wr2;
	}else if((type == RTL_BOARD_WR3) || (type == RTL_BOARD_WR5)) {
		gpio_test_table.group = groups_wr3;
	}else if(type == RTL_BOARD_WR4) {
		gpio_test_table.group = groups_wr4;
	}else if(type == RTL_BOARD_WR7) {
		gpio_test_table.group = groups_wr7;
	}else if(type == RTL_BOARD_WR8) {
		gpio_test_table.group = groups_wr8;
	}else if(type == RTL_BOARD_WR9) {
		gpio_test_table.group = groups_wr9;
	}else if(type == RTL_BOARD_WR10) {
		gpio_test_table.group = groups_wr10;
	}else if(type == RTL_BOARD_WR11) {
		gpio_test_table.group = groups_wr11;
	}else if(type == RTL_BOARD_WR12) {
		gpio_test_table.group = groups_wr12;
	}
	
	sys_log_uart_off();
    INT_T idx,i,j;
    for(idx = 0; idx < gpio_test_table.group_num; idx++) {
        for(i = 0; i < gpio_test_table.group[idx].ionum; i++) {
            //set io direction
            for(j = 0; j < gpio_test_table.group[idx].ionum; j++) {
                if(i== j) {
                    tuya_gpio_inout_set(gpio_test_table.group[idx].iopin[j],FALSE);
                }else {
                    tuya_gpio_inout_set(gpio_test_table.group[idx].iopin[j],TRUE);
                }
            }
            // write 1
            tuya_gpio_write(gpio_test_table.group[idx].iopin[i],TRUE);
            for(j = 0; j < gpio_test_table.group[idx].ionum; j++) {
                if(i!= j) {
                    if(tuya_gpio_read(gpio_test_table.group[idx].iopin[j]) != 1) {
                        PR_ERR("gpio test err_high i = %d,j = %d",i,j);
                        return FALSE;
                    }
                }
            }

            // write 0
            tuya_gpio_write(gpio_test_table.group[idx].iopin[i],FALSE);
            for(j = 0; j < gpio_test_table.group[idx].ionum; j++) {
                if(i!= j) {
                    if(tuya_gpio_read(gpio_test_table.group[idx].iopin[j]) != 0) {
                        PR_ERR("gpio test err_lowi = %d,j = %d",i,j);
                        return FALSE;
                    }
                }
            }
            
        }
    }
    sys_log_uart_on();
	return TRUE;
	
	
}

BOOL_T gpio_test_all(VOID)
{

    return  gpio_test_cb(RTL_BOARD_WR1) || gpio_test_cb(RTL_BOARD_WR2)|| gpio_test_cb(RTL_BOARD_WR3)|| gpio_test_cb(RTL_BOARD_WR4)|| gpio_test_cb(RTL_BOARD_WR7) || \
            gpio_test_cb(RTL_BOARD_WR8) || gpio_test_cb(RTL_BOARD_WR9)|| gpio_test_cb(RTL_BOARD_WR10)|| gpio_test_cb(RTL_BOARD_WR11)|| gpio_test_cb(RTL_BOARD_WR12);
}
