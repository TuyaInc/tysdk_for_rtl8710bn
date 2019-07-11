/** ----------------------------------------------------------------------------
 *         Nationz Technology Software Support  -  NATIONZ  -
 * -----------------------------------------------------------------------------
 * Copyright (c) 2013 - 2018, Nationz Corporation  All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaiimer below.
 * 
 * - Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the disclaimer below in the documentation and/or
 * other materials provided with the distribution. 
 * 
 * Nationz's name may not be used to endorse or promote products derived from
 * this software without specific prior written permission. 
 * 
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY NATIONZ "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * -----------------------------------------------------------------------------
 */

/** ****************************************************************************
 * @copyright                  Nationz Co.,Ltd 
 *               Copyright (c) 2013 - 2018 All Rights Reserved 
 *******************************************************************************
 * @file     type.h
 * @author   nationz
 * @date     2017-6-14 09:03:00
 * @version  v1.0
 * @brief    bpoi Header File
 ******************************************************************************/

#ifndef __TYPE_H__
#define __TYPE_H__

typedef signed       char int8_t;
typedef signed short int int16_t;
typedef signed       int int32_t;

typedef unsigned       char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned       int uint32_t;

#ifndef __cplusplus /* In C++, 'bool', 'true' and 'false' and keywords */
  #define bool _Bool
  #define true 1
  #define false 0
#else
  #ifdef __GNUC__
    /* GNU C++ supports direct inclusion of stdbool.h to provide C99
       compatibility by defining _Bool */
    #define _Bool bool
  #endif
#endif
		
#endif /*__TYPE_H__*/

/* end of type.h */
