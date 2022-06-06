/*
 * $Id: fe3200_multicast.h,v 1.3 Broadcom SDK $
 *
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * FE3200 MULTICAST H
 */
 
#ifndef _SOC_FE3200_MULTICAST_H_
#define _SOC_FE3200_MULTICAST_H_

#include <soc/dfe/cmn/dfe_defs.h>
#include <soc/error.h>

soc_error_t soc_fe3200_multicast_mode_get(int unit, soc_dfe_multicast_table_mode_t* multicast_mode);
soc_error_t soc_fe3200_multicast_table_size_get(int unit, uint32* mc_table_size);
soc_error_t soc_fe3200_multicast_table_entry_size_get(int unit, uint32* entry_size);

#endif

