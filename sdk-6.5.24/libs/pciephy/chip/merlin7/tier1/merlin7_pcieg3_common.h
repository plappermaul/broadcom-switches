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

/** @file merlin7_pcieg3_common.h
 * Defines and Enumerations shared across MER7/BHK7/OSP7 APIs but NOT uCode
 */
#ifdef __cplusplus
extern "C" {
#endif

#ifndef MERLIN7_PCIEG3_API_COMMON_H
#define MERLIN7_PCIEG3_API_COMMON_H

#include "merlin7_pcieg3_ipconfig.h"

/** Macro to determine sign of a value */
#define sign(x) ((x>=0) ? 1 : -1)

#define UCODE_MAX_SIZE  (33*1024)     /*  33K CODE RAM */

/*
 * IP-Specific Iteration Bounds
 */
# define DUAL_PLL_NUM_PLLS 1

#endif
#ifdef __cplusplus
}
#endif
