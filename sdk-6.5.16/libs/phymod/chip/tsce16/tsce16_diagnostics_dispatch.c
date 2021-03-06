/*
 *         
 * 
 * 
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2019 Broadcom Inc. All rights reserved.
 *         
 *     
 * DO NOT EDIT THIS FILE!
 */

#include <phymod/phymod.h>
#include <phymod/phymod_system.h>
#include <phymod/phymod_diagnostics.h>
#include <phymod/phymod_diagnostics_dispatch.h>

#ifdef PHYMOD_TSCE16_SUPPORT

#include <phymod/chip/tsce16_diagnostics.h>

#include <phymod/chip/merlin16_diagnostics.h>

__phymod_diagnostics__dispatch__t__ phymod_diagnostics_tsce16_diagnostics_driver = {

    NULL, /* phymod_phy_rx_slicer_position_set */
    NULL, /* phymod_phy_rx_slicer_position_get */
    NULL, /* phymod_phy_rx_slicer_position_max_get */
    tsce16_phy_prbs_config_set,
    tsce16_phy_prbs_config_get,
    tsce16_phy_prbs_enable_set,
    tsce16_phy_prbs_enable_get,
    tsce16_phy_prbs_status_get,
    NULL, /* phymod_phy_pattern_config_set */
    NULL, /* phymod_phy_pattern_config_get */
    NULL, /* phymod_phy_pattern_enable_set */
    NULL, /* phymod_phy_pattern_enable_get */
    NULL, /* phymod_core_diagnostics_get */
    NULL, /* phymod_phy_diagnostics_get */
    tsce16_phy_pmd_info_dump,
    tsce16_phy_pcs_info_dump,
    merlin16_phy_eyescan_run,
    NULL, /* phymod_phy_link_mon_enable_set */
    NULL, /* phymod_phy_link_mon_enable_get */
    NULL, /* phymod_phy_link_mon_status_get */
    NULL, /* phymod_phy_fec_correctable_counter_get */
    NULL, /* phymod_phy_fec_uncorrectable_counter_get */
    NULL, /* phymod_phy_stat_get */
    NULL, /* phymod_phy_fec_cl91_correctable_counter_get */
    NULL, /* phymod_phy_fec_cl91_uncorrectable_counter_get */
    NULL, /* phymod_phy_PAM4_tx_pattern_enable_set */
    NULL, /* phymod_phy_PAM4_tx_pattern_enable_get */
    NULL, /* phymod_phy_ber_proj */
    NULL, /* phymod_phy_fast_ber_proj_get */
    NULL, /* phymod_phy_rsfec_symbol_error_counter_get */
};

#endif /* PHYMOD_TSCE16_SUPPORT */
