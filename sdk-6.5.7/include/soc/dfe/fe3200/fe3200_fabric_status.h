/*
 * $Id: fe3200_fabric_status.h,v 1.4 Broadcom SDK $
 *
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * FE3200 FABRIC STATUS H
 */
 
#ifndef _SOC_FE3200_FABRIC_STATUS_H_
#define _SOC_FE3200_FABRIC_STATUS_H_

#include <soc/dcmn/fabric.h>
#include <soc/dfe/cmn/dfe_defs.h>
#include <soc/error.h>

soc_error_t soc_fe3200_fabric_link_status_clear(int unit, soc_port_t link);
soc_error_t soc_fe3200_fabric_link_status_get(int unit, soc_port_t link_id, uint32 *link_status, uint32 *errored_token_count);

#endif

