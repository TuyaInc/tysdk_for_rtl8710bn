/***********************************************************
*  File: tuya_gpio.h
*  Author: nzy
*  Date: 20171115
***********************************************************/
#ifndef __TUYA_GPIO_H
    #define __TUYA_GPIO_H

    #include "tuya_cloud_types.h"
    #include "gpio_irq_api.h"

#ifdef __cplusplus
	extern "C" {
#endif

#ifdef  __TUYA_GPIO_GLOBALS
    #define __TUYA_GPIO_EXT
#else
    #define __TUYA_GPIO_EXT extern
#endif

/***********************************************************
*************************micro define***********************
***********************************************************/
typedef enum {
    TY_GPIOA_0 = 0,
    TY_GPIOA_1,
    TY_GPIOA_2,
    TY_GPIOA_3,
    TY_GPIOA_4,
    TY_GPIOA_5,
    TY_GPIOA_6,
    TY_GPIOA_7,
    TY_GPIOA_8,
    TY_GPIOA_9,
    TY_GPIOA_10,
    TY_GPIOA_11,
    TY_GPIOA_12,
    TY_GPIOA_13,
    TY_GPIOA_14,
    TY_GPIOA_15,
    TY_GPIOA_16,
    TY_GPIOA_17,
    TY_GPIOA_18,
    TY_GPIOA_19,
    TY_GPIOA_20,
    TY_GPIOA_21,
    TY_GPIOA_22,
    TY_GPIOA_23,
    TY_GPIOA_24,
    TY_GPIOA_25,
    TY_GPIOA_26,
    TY_GPIOA_27,
    TY_GPIOA_28,
    TY_GPIOA_29,
    TY_GPIOA_30,
    TY_GPIOA_31,
    TY_GPIOB_0 = 32,
    TY_GPIOB_1,
    TY_GPIOB_2,
    TY_GPIOB_3,
    TY_GPIOB_4,
    TY_GPIOB_5,
    TY_GPIOB_6,
    TY_GPIOB_7,
    TY_GPIOB_8,
}TY_GPIO_PORT_E;

typedef enum {
    TY_GPIO_PULLUP,                    /*!< Pad pull up            */
    TY_GPIO_PULLDOWN,                  /*!< Pad pull down          */
    TY_GPIO_PULLUP_PULLDOWN,           /*!< Pad pull up + pull down*/
    TY_GPIO_OPENDRAIN,                 /*!< Pad open drain         */
    TY_GPIO_FLOATING,                  /*!< Pad floating           */
} TY_GPIO_MODE_E;

/***********************************************************
*************************variable define********************
***********************************************************/

/***********************************************************
*************************function define********************
***********************************************************/
/***********************************************************
*  Function: tuya_gpio_inout_set
*  Input: port
*         in->TRUE:in
*             FALSE:out
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__TUYA_GPIO_EXT \
OPERATE_RET tuya_gpio_inout_set(IN CONST TY_GPIO_PORT_E port,IN CONST BOOL_T in);
/***********************************************************
*  Function: tuya_gpio_inout_set_select
*  Input: port
*          in->TRUE:in
*              FALSE:out
          high->TRUE
               FALSE
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__TUYA_GPIO_EXT \
OPERATE_RET tuya_gpio_inout_set_select(IN CONST TY_GPIO_PORT_E port,IN CONST BOOL_T in,IN CONST BOOL_T high);


/***********************************************************
*  Function: tuya_gpio_mode_set
*  Input: port
*         in->  TY_GPIO_PULLUP,
*               TY_GPIO_PULLDOWN,
*               TY_GPIO_PULLUP_PULLDOWN,
*               TY_GPIO_OPENDRAIN,
*               TY_GPIO_FLOATING
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__TUYA_GPIO_EXT \
OPERATE_RET tuya_gpio_mode_set(IN CONST TY_GPIO_PORT_E port, IN CONST TY_GPIO_MODE_E mode);

/***********************************************************
*  Function: tuya_gpio_read
*  Input: port
*         in->TRUE:in
*             FALSE:out
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__TUYA_GPIO_EXT \
INT_T tuya_gpio_read(IN CONST TY_GPIO_PORT_E port);

/***********************************************************
*  Function: tuya_gpio_write
*  Input: port
*         high->TRUE:high
*               FALSE:low
*  Output: none
*  Return: OPERATE_RET
***********************************************************/
__TUYA_GPIO_EXT \
OPERATE_RET tuya_gpio_write(IN CONST TY_GPIO_PORT_E port,IN CONST BOOL_T high);
__TUYA_GPIO_EXT \
OPERATE_RET tuya_gpio_irq_init(IN CONST TY_GPIO_PORT_E port,IN CONST gpio_irq_handler gpio_irq_handler_cb, IN CONST INT_T trig_type, UINT_T id);

#ifdef __cplusplus
}
#endif
#endif

