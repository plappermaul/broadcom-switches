/***********************************************************************************
 *                                                                                 *
 * Copyright: (c) 2019 Broadcom.                                                   *
 * Broadcom Proprietary and Confidential. All rights reserved.                     *
 */

/*********************************************************************************
 *********************************************************************************
 *  File Name  :  merlin7_pcieg3_select_defns.h
 *  Created On :  29 Sep 2015
 *  Created By :  Brent Roberts
 *  Description:  Select header files for IP-specific definitions
 *  Revision   :  
 */

 /** @file
 * Select IP files to include for API
 */

#ifndef MERLIN7_PCIEG3_API_SELECT_DEFNS_H
#define MERLIN7_PCIEG3_API_SELECT_DEFNS_H

#include "merlin7_pcieg3_ipconfig.h"
#include "merlin7_pcieg3_field_access.h"

#   include "merlin7_api_uc_common.h"

/****************************************************************************
 * @name Register Access Macro Inclusions
 *
 * All cores provide access to hardware control/status registers.
 */
/**@{*/

#if   defined(MERLIN7_EXP8)
#include "merlin7_tsc_fields.h"
#else
/**
 * This build includes register access macros for the MERLIN7_PCIEG3 core.
 */
#include "merlin7_pcieg3_fields.h"
#endif

/**@}*/


/****************************************************************************
 * @name RAM Access Macro Inclusions
 *
 * Some cores also provide access to firmware control/status RAM variables.
 */
/**@{*/

/**
 * This build includes macros to access Merlin7 microcode RAM variables.
 */
#include "merlin7_api_uc_vars_rdwr_defns.h"
/**@}*/

#endif
