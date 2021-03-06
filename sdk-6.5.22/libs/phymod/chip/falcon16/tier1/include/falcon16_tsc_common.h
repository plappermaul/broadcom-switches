/***********************************************************************************
 *                                                                                 *
 * Copyright: (c) 2019 Broadcom.                                                   *
 * Broadcom Proprietary and Confidential. All rights reserved.                     *
 */

/********************************************************************************
 ********************************************************************************
 *                                                                              *
 *  Revision      :   *
 *                                                                              *
 *  Description   :  Defines and Enumerations required by Serdes APIs           *
 */

/** @file falcon16_tsc_common.h
 * Defines and Enumerations shared across M16/F16/BHK16 APIs BUT NOT MICROCODE
 */

#ifndef FALCON16_TSC_API_COMMON_H
#define FALCON16_TSC_API_COMMON_H

#include "falcon16_tsc_ipconfig.h"

/** Macro to determine sign of a value */
#define sign(x) ((x>=0) ? 1 : -1)

#define UCODE_MAX_SIZE (84*1024)

/*
 * IP-Specific Iteration Bounds
 */

#endif
