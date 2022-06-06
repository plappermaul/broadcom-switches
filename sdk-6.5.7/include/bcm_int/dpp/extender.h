/*
 * $Id: extender.h,v 1.45 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * File:        extender.h
 * Purpose:     Helper function declarations for the extender module BCM APIs.
 *
 */

#ifndef  INCLUDE_EXTENDER_H
#define  INCLUDE_EXTENDER_H

#include <bcm/extender.h>
#include <soc/dpp/drv.h>

/*************
 * INCLUDES  *
 *************/
/* { */

/* } */
/*************
 * DEFINES   *
 *************/
/* { */

/* } */
/*************
 * MACROS    *
 *************/
/* { */


#define _BCM_DPP_EXTENDER_IS_ENABLED(_unit) (SOC_DPP_CONFIG(_unit)->extender.port_extender_init_status >= soc_dpp_extender_init_status_enabled)

#define _BCM_DPP_EXTENDER_IS_INIT(_unit) (SOC_DPP_CONFIG(_unit)->extender.port_extender_init_status >= soc_dpp_extender_init_status_init)

#define _BCM_DPP_EXTENDER_INIT_CHECK(_unit) \
    if (!_BCM_DPP_EXTENDER_IS_INIT(_unit)) { \
        BCMDNX_ERR_EXIT_MSG(BCM_E_CONFIG, (_BSL_BCM_MSG("Port extender module is not initialized.")));\
    }

/* } */
/*************
 * TYPE DEFS *
 *************/
/* { */


/* } */
/*************
 * GLOBALS   *
 *************/
/* { */

/* } */
/*************
 * FUNCTIONS *
 *************/
/* { */


/* } */

#endif /* INCLUDE_EXTENDER_H */

