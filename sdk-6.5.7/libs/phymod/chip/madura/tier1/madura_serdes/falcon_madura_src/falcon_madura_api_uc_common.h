/**********************************************************************************
 **********************************************************************************
 *                                                                                *
 *  Revision      :  $Id: falcon_api_uc_common.h 661 2014-08-14 23:20:49Z kirand $  *
 *                                                                                *
 *  Description   :  Defines and Enumerations required by Falcon ucode            *
 *                                                                                *
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$                                                          *
 *  No portions of this material may be reproduced in any form without            *
 *  the written permission of:                                                    *
 *      Broadcom Corporation                                                      *
 *      5300 California Avenue                                                    *
 *      Irvine, CA  92617                                                         *
 *                                                                                *
 *  All information contained in this document is Broadcom Corporation            *
 *  company private proprietary, and trade secret.                                *
 *                                                                                *
 **********************************************************************************
 **********************************************************************************/

/** @file falcon_api_uc_common.h
 * Defines and Enumerations shared by Falcon IP Specific API and Microcode
 */

#ifndef FALCON_API_UC_COMMON_H
#define FALCON_API_UC_COMMON_H

/* Add Falcon specific items below this */ 

/* Please note that when adding entries here you should update the #defines in the falcon_furia_common.h */

/** OSR_MODES Enum */
enum falcon_furia_sesto_osr_mode_enum {
  FALCON_FURIA_OSX1    = 0,
  FALCON_FURIA_OSX2    = 1,
  FALCON_FURIA_OSX4    = 2,
  FALCON_FURIA_OSX16P5 = 8,
  FALCON_FURIA_OSX20P625 = 12
};

/** CDR mode Enum **/
enum falcon_furia_sesto_cdr_mode_enum {
  FALCON_FURIA_CDR_MODE_OS_ALL_EDGES         = 0,
  FALCON_FURIA_CDR_MODE_OS_PATTERN           = 1,
  FALCON_FURIA_CDR_MODE_OS_PATTERN_ENHANCED  = 2,  
  FALCON_FURIA_CDR_MODE_BR_PATTERN           = 3
};

/** Lane User Control Clause93/72 Force Value **/
enum falcon_furia_sesto_cl93n72_frc_val_enum {
  FALCON_FURIA_CL93N72_FORCE_OS  = 0,
  FALCON_FURIA_CL93N72_FORCE_BR  = 1  
};


#endif
