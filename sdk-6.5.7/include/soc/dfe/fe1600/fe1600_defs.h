/*
 * $Id: fe1600_defs.h,v 1.10 Broadcom SDK $
 *
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * FE1600 DEFS H
 */
 
#ifndef _SOC_FE1600_FABRIC_DEFS_H_
#define _SOC_FE1600_FABRIC_DEFS_H_

#include <soc/dfe/cmn/dfe_defs.h>
#include <soc/dcmn/error.h>
#include <soc/dfe/fe1600/fe1600_interrupts.h>
#include <soc/dfe/fe1600/fe1600_config_defs.h>


#define SOC_FE1600_REDUCED_NOF_LINKS 64

#define SOC_FE1600_REDUCED_NOF_INSTANCES_MAC 16

#define SOC_FE1600_REDUCED_NOF_INSTANCES_MAC_FSRD 4

#define SOC_FE1600_REDUCED_NOF_INSTANCES_DCH 2

#define SOC_FE1600_REDUCED_NOF_INSTANCES_DCL 2

#define SOC_FE1600_REDUCED_BLK_NOF_INSTANCES_BRDC_FMACH 0


#define SOC_FE1600_ONLY(unit)         assert(SOC_IS_FE1600(unit))


#endif /*_SOC_FE1600_FABRIC_DEFS_H_*/

