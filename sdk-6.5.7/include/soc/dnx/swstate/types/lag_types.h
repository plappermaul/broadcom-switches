/* $Id: $
 * 
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 * 
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated.
 * Edits to this file will be lost when it is regenerated.
 */

#ifndef _SOC_LAG_TYPES_H_
#define _SOC_LAG_TYPES_H_

#ifdef BCM_DNX_SUPPORT
#include <soc/dnx/swstate/sw_state_defs.h>

#include <soc/dnx/legacy/ARAD/arad_chip_defines.h>

/*** Definitions ***/
#define JER2_ARAD_NOF_LAG_GROUPS_MAX 1024


/*** Typedefs ***/

typedef struct JER2_ARAD_LAGS_INFO_s {
    uint8 in_use[JER2_ARAD_NOF_LAG_GROUPS_MAX];
    uint32 local_to_reassembly_context[JER2_ARAD_NOF_LOCAL_PORTS_MAX];
} JER2_ARAD_LAGS_INFO;


#endif /* BCM_DNX_SUPPORT*/ 

#endif /* _SOC_LAG_TYPES_H_ */
