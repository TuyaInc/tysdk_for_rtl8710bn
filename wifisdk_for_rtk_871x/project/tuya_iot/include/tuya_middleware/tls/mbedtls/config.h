/**
 * \file config.h
 *
 * \brief Configuration options (set of defines)
 *
 *  This set of compile-time options may be used to enable
 *  or disable features selectively, and reduce the global
 *  memory footprint.
 */
/*
 *  Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  This file is part of mbed TLS (https://tls.mbed.org)
 */

#ifndef MBEDTLS_CONFIG_H
#define MBEDTLS_CONFIG_H

#include "tuya_cloud_types.h"

#if defined(TLS_MODE) && (TLS_MODE==TLS_DISABLE)
#include "tuya_tls_config_disable.h"
#endif

#if defined(TLS_MODE) && (TLS_MODE==TLS_TUYA_PSK_ONLY)
#include "tuya_tls_config_psk_only.h"
#endif

#if defined(TLS_MODE) && (TLS_MODE==TLS_TUYA_ECC_PSK)
#include "tuya_tls_config_ecc_psk.h"
#endif

#if defined(TLS_MODE) && (TLS_MODE==TLS_TUYA_ECC_ONLY)
#include "tuya_tls_config_ecc_only.h"
#endif

#if defined(TLS_MODE) && (TLS_MODE==TLS_TUYA_ECC_ONLY_NOSTRIP)
#include "tuya_tls_config_no_strip.h"
#endif

#include "check_config.h"

#endif /* MBEDTLS_CONFIG_H */
