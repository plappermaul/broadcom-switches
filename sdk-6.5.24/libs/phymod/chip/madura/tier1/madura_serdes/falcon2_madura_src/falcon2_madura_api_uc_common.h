/**********************************************************************************
 **********************************************************************************
 *                                                                                *
 *  Revision      :  $Id: falcon_api_uc_common.h 1340 2016-02-04 22:50:01Z kirand $  *
 *                                                                                *
 *  Description   :  Defines and Enumerations required by Falcon ucode            *
 *                                                                                *
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2021 Broadcom Inc. All rights reserved.                                                          *
 *  No portions of this material may be reproduced in any form without            *
 *  the written permission of:                                                    *
 *      Broadcom Corporation                                                      *
 *      5300 California Avenue                                                    *
 *      Irvine, CA  92617                                                         *
 *                                                                                *
 *  All information contained in this document is Broadcom Corporation            *
 *  company private proprietary, and trade secret.                                *
 */

/** @file falcon_api_uc_common.h
 * Defines and Enumerations shared by Falcon IP Specific API and Microcode
 */

#ifndef FALCON_API_UC_COMMON_H
#define FALCON_API_UC_COMMON_H

/* Add Falcon specific items below this */

/** Translate between a VCO frequency in MHz and the vco_rate that is **\
*** found in the Core Config Variable Structure using the formula:    ***
***                                                                   ***
***     vco_rate = (frequency_in_ghz * 16.0) - 224.0                  ***
***                                                                   ***
*** Both functions round to the nearest resulting value.  This        ***
*** provides the highest accuracy possible, and ensures that:         ***
***                                                                   ***
***     vco_rate == MHZ_TO_VCO_RATE(VCO_RATE_TO_MHZ(vco_rate))        ***
***                                                                   ***
*** In the microcode, this should only be called with a numeric       ***
*** literal parameter.                                                ***
\**                                                                   **/
#define MHZ_TO_VCO_RATE(mhz) ((uint8_t)(((((uint16_t)(mhz) * 2) + 62) / 125) - 224))
#define VCO_RATE_TO_MHZ(vco_rate) (((((uint16_t)(vco_rate) + 224) * 125) + 1) >> 1)

/* Please note that when adding entries here you should update the #defines in the falcon2_madura_common.h */

/** OSR_MODES Enum */
enum falcon2_madura_osr_mode_enum {
  FALCON2_MADURA_OSX1    = 0,
  FALCON2_MADURA_OSX2    = 1,
  FALCON2_MADURA_OSX4    = 2,
  FALCON2_MADURA_OSX8    = 5,
  FALCON2_MADURA_OSX16P5   = 8,
  FALCON2_MADURA_OSX16     = 9,
  FALCON2_MADURA_OSX20P625 = 12,
  FALCON2_MADURA_OSX32     = 13
};

/** CDR mode Enum **/
enum falcon2_madura_cdr_mode_enum {
  FALCON2_MADURA_CDR_MODE_OS_ALL_EDGES         = 0,
  FALCON2_MADURA_CDR_MODE_OS_PATTERN           = 1,
  FALCON2_MADURA_CDR_MODE_OS_PATTERN_ENHANCED  = 2,
  FALCON2_MADURA_CDR_MODE_BR_PATTERN           = 3
};

/** Lane User Control Clause93/72 Force Value **/
enum falcon2_madura_cl93n72_frc_val_enum {
  FALCON2_MADURA_CL93N72_FORCE_OS  = 0,
  FALCON2_MADURA_CL93N72_FORCE_BR  = 1
};


#endif
