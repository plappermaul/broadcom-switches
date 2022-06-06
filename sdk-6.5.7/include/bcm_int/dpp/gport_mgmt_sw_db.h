/*
 * $Id: gport_mgmt_sw_db.h,v 1.2 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * File:        diag_alloc.h
 * Purpose:     
 */

#ifndef   INCLUDE_GPORT_MGMT_SW_DB_H
#define   INCLUDE_GPORT_MGMT_SW_DB_H

#include <bcm_int/dpp/gport_mgmt.h>


int
_bcm_dpp_sw_db_hash_vlan_print(int unit, _BCM_GPORT_PHY_PORT_INFO *data);

int
_bcm_dpp_lif_ac_match_print(int unit, int lif);

int
_bcm_dpp_lif_mpls_match_print(int unit, int lif);

#endif



