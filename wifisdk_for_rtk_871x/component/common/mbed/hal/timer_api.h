/** mbed Microcontroller Library
  ******************************************************************************
  * @file    timer_api.h
  * @author 
  * @version V1.0.0
  * @brief   This file provides following mbed I2C API
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2006-2013 ARM Limited
  *
  * Licensed under the Apache License, Version 2.0 (the "License");
  * you may not use this file except in compliance with the License.
  * You may obtain a copy of the License at
  *
  *     http://www.apache.org/licenses/LICENSE-2.0
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  ****************************************************************************** 
  */
#ifndef MBED_EXT_TIMER_API_EXT_H
#define MBED_EXT_TIMER_API_EXT_H

#include "device.h"

#ifdef __cplusplus
extern "C" {
#endif


/** @addtogroup timer       TIMER
 *  @ingroup    hal
 *  @brief      timer functions
 *  @{
 */

///@name Ameba Common 
///@{
typedef struct gtimer_s gtimer_t;
typedef void (*gtimer_irq_handler)(uint32_t id);


/**
  * @brief  Initializes the timer, including clock/function/timer registers.
  * @param  obj: gtimer object defined in application software.
  * @param  tid: the timer struct ID defined in gtimer structure.
  * @retval none  
  */
void gtimer_init (gtimer_t *obj, uint32_t tid);

/**
  * @brief  Deinitializes the timer, including clock/function/timer registers.
  * @param  obj: gtimer object defined in application software.
  * @retval none  
  */
void gtimer_deinit (gtimer_t *obj);

/**
  * @brief  Read current timer tick in Gtimer clock(0~32768).
  * @param  obj: gtimer object defined in application software.
  * @retval 32 bit tick time  
  */
uint32_t gtimer_read_tick (gtimer_t *obj);

/**
  * @brief  Read current timer tick in microsecond.
  * @param  obj: gtimer object defined in application software.
  * @retval 64 bit tick time  
  */
uint64_t gtimer_read_us (gtimer_t *obj);

/**
  * @brief  Reload timer
  * @param  obj: gtimer object defined in application software.
  * @param  duration_us: the time in microsecond of gtimer to reload.
  * @retval none  
  */
void gtimer_reload (gtimer_t *obj, uint32_t duration_us);
/**
  * @brief  Start the timer
  * @param  obj: gtimer object defined in application software.
  * @retval none  
  */
void gtimer_start (gtimer_t *obj);
/**
  * @brief  Set up a shout timer, including clock/function/timer registers.
  * @param  obj: gtimer object defined in application software.
  * @param  duration_us: the period in microsecond of gtimer.
  * @param  handler: The Pointer to the function that program runs into when timer is up.
  * @param  hid: the timer struct ID defined in gtimer structure.
  * @retval none  
  */
void gtimer_start_one_shout (gtimer_t *obj, uint32_t duration_us, void* handler, uint32_t hid);
/**
  * @brief  Set up a periodic timer, including clock/function/timer registers.
  * @param  obj: gtimer object defined in application software.
  * @param  duration_us: the period in microsecond of gtimer.
  * @param  handler: The Pointer to function that program runs into when timer is up.
  * @param  hid: the timer struct ID defined in gtimer structure.
  * @retval none  
  */
void gtimer_start_periodical (gtimer_t *obj, uint32_t duration_us, void* handler, uint32_t hid);

/**
  * @brief  Stop the timer, including clock/function/timer registers.
  * @param  obj: gtimer object defined in application software.
  * @retval none  
  */
void gtimer_stop (gtimer_t *obj);


#if CONFIG_PLATFORM_8195A
///@name Ameba Only 
///@{


enum { 
    TIMER0 = 2,    
    TIMER1 = 3,   
    TIMER2 = 4,    
    TIMER3 = 5,   
    TIMER4 = 0,    

    GTIMER_MAX = 5    
};

///@}
#endif //CONFIG_PLATFORM_8195A



#if CONFIG_PLATFORM_8711B
///@name AmebaZ Only 
///@{

enum { 
    TIMER0 = 0,   
    TIMER1 = 1,    
    TIMER2 = 2,    
    TIMER3 = 3,   

    GTIMER_MAX = 4    
};

///@}
#endif //CONFIG_PLATFORM_8711B


#ifdef __cplusplus
}
#endif

#endif/* MBED_TIMER_API_H */
