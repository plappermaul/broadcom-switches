/********************************************************************************
 ********************************************************************************
 *                                                                              *
 *  Revision      :   *
 *                                                                              *
 *  Description   :  Defines and Enumerations required by Serdes APIs           *
 *                                                                              *
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2022 Broadcom Inc. All rights reserved.                                                        *
 *  No portions of this material may be reproduced in any form without          *
 *  the written permission of:                                                  *
 *      Broadcom Corporation                                                    *
 *      5300 California Avenue                                                  *
 *      Irvine, CA  92617                                                       *
 *                                                                              *
 *  All information contained in this document is Broadcom Corporation          *
 *  company private proprietary, and trade secret.                              *
 *                                                                              *
 ********************************************************************************
 ********************************************************************************/

/** @file merlin16_common.h
 * Defines and Enumerations shared across Merlin16/Falcon16 APIs BUT NOT MICROCODE
 */

#ifndef MERLIN16_API_COMMON_H
#define MERLIN16_API_COMMON_H

#include "merlin16_ipconfig.h"

/** Macro to determine sign of a value */
#define sign(x) ((x>=0) ? 1 : -1)

#define UCODE_MAX_SIZE  (64*1024 + 7168)    /* additional code that could be stored in data ram */

/*
 * IP-Specific Iteration Bounds
 */

#endif
