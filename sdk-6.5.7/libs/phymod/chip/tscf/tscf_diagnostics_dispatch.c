/*
 *         
 * $Id: phymod.xml,v 1.1.2.5 Broadcom SDK $
 * 
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *         
 *     
 * DO NOT EDIT THIS FILE!
 *
 */

#include <phymod/phymod.h>
#include <phymod/phymod_diagnostics.h>
#include <phymod/phymod_diagnostics_dispatch.h>

#ifdef PHYMOD_TSCF_SUPPORT

#include <phymod/chip/tscf_diagnostics.h>

#include <phymod/chip/falcon_diagnostics.h>

__phymod_diagnostics__dispatch__t__ phymod_diagnostics_tscf_diagnostics_driver = {

    tscf_phy_rx_slicer_position_set,
    tscf_phy_rx_slicer_position_get,
    tscf_phy_rx_slicer_position_max_get,
    tscf_phy_prbs_config_set,
    tscf_phy_prbs_config_get,
    tscf_phy_prbs_enable_set,
    tscf_phy_prbs_enable_get,
    tscf_phy_prbs_status_get,
    tscf_phy_pattern_config_set,
    tscf_phy_pattern_config_get,
    tscf_phy_pattern_enable_set,
    tscf_phy_pattern_enable_get,
    tscf_core_diagnostics_get,
    falcon_phy_diagnostics_get,
    tscf_phy_pmd_info_dump,
    tscf_phy_pcs_info_dump,
    falcon_phy_eyescan_run,
    NULL, /* phymod_phy_link_mon_enable_set */
    NULL, /* phymod_phy_link_mon_enable_get */
    NULL, /* phymod_phy_link_mon_status_get */
    tscf_phy_fec_correctable_counter_get,
    tscf_phy_fec_uncorrectable_counter_get,
};

#endif /* PHYMOD_TSCF_SUPPORT */
