/***********************************************************************************
 *                                                                                 *
 * Copyright: (c) 2019 Broadcom.                                                   *
 * Broadcom Proprietary and Confidential. All rights reserved.                     *
 */

/**************************************************************************************
 **************************************************************************************
 *  File Name     :  merlin7_tsc_pwr_mgt.c                                             *
 *  Created On    :  04 Nov 2015                                                      *
 *  Created By    :  Brent Roberts                                                    *
 *  Description   :  Power management APIs for Serdes IPs                             *
 *  Revision      :     *
 */

/** @file merlin7_tsc_pwr_mgt.c
 * Implementation of power management functions
 */

#include "merlin7_tsc_pwr_mgt.h"
#include "merlin7_tsc_common.h"
#include "merlin7_tsc_functions.h"
#include "merlin7_tsc_internal.h"
#include "merlin7_tsc_internal_error.h"
#include "merlin7_tsc_select_defns.h"
#include "merlin7_tsc_config.h"

#ifndef SMALL_FOOTPRINT
/***************************/
/*  Configure Serdes IDDQ  */
/***************************/

err_code_t merlin7_tsc_core_config_for_iddq(srds_access_t *sa__) {
    INIT_SRDS_ERR_CODE
    UNUSED(sa__);
    EFUN(wrc_ams_pll_pwrdn(0x1));
    return (ERR_CODE_NONE);
}


err_code_t merlin7_tsc_lane_config_for_iddq(srds_access_t *sa__) {
    INIT_SRDS_ERR_CODE

    /* Use frc/frc_val to force all RX and TX clk_vld signals to 0 */
    EFUN(wr_pmd_rx_clk_vld_frc_val(0x0));
    EFUN(wr_pmd_rx_clk_vld_frc(0x1));
    EFUN(wr_pmd_tx_clk_vld_frc_val(0x0));
    EFUN(wr_pmd_tx_clk_vld_frc(0x1));

    /* Use frc/frc_val to force all pmd_rx_lock signals to 0 */
    EFUN(wr_rx_dsc_lock_frc_val(0x0));
    EFUN(wr_rx_dsc_lock_frc(0x1));

    /* Switch all the lane clocks to comclk by writing to RX/TX comclk_sel registers */
    EFUN(wr_ln_rx_s_comclk_sel(0x1));
    EFUN(wr_ln_tx_s_comclk_sel(0x1));

    /* Assert all the AFE pwrdn/reset pins using frc/frc_val to make sure AFE is in lowest possible power mode */
    EFUN(wr_afe_tx_pwrdn_frc_val(0x1));
    EFUN(wr_afe_tx_pwrdn_frc(0x1));
    EFUN(wr_afe_rx_pwrdn_frc_val(0x1));
    EFUN(wr_afe_rx_pwrdn_frc(0x1));
    EFUN(wr_afe_tx_reset_frc_val(0x1));
    EFUN(wr_afe_tx_reset_frc(0x1));
    EFUN(wr_afe_rx_reset_frc_val(0x1));
    EFUN(wr_afe_rx_reset_frc(0x1));

    /* Set pmd_iddq pin to enable IDDQ */
    return (ERR_CODE_NONE);
}
/****************************************************/
/*  Serdes Powerdown, ClockGate and Deep_Powerdown  */
/****************************************************/

#endif /* !SMALL_FOOTPRINT */
#if !defined(SMALL_FOOTPRINT) 
err_code_t merlin7_tsc_core_pwrdn(srds_access_t *sa__, enum srds_core_pwrdn_mode_enum mode) {
    INIT_SRDS_ERR_CODE

    switch(mode) {
    case PWR_ON:
        EFUN(merlin7_tsc_INTERNAL_core_clkgate(sa__, 0));
        EFUN(wrc_ams_pll_pwrdn(0x0));
        EFUN(wrc_afe_s_pll_pwrdn(0x0));
        EFUN(USR_DELAY_NS(500)); /* wait >50 comclk cycles  */
        break;
    case PWRDN:
          EFUN(merlin7_tsc_core_dp_reset(sa__, 1));
          EFUN(USR_DELAY_NS(500)); /* wait >50 comclk cycles  */
          EFUN(wrc_afe_s_pll_pwrdn(0x1));
          EFUN(USR_DELAY_NS(500)); /* wait >50 comclk cycles  */
          EFUN(wrc_ams_pll_pwrdn(0x1));
        break;
    case PWROFF_DEEP: /* PWROFF_DEEP is identical to PWRDN_DEEP for core pwrdn. FALL THROUGH */
    case PWRDN_DEEP:
          EFUN(merlin7_tsc_core_pwrdn(sa__, PWRDN));

        break;
    default:
        EFUN(merlin7_tsc_error(sa__, ERR_CODE_BAD_PTR_OR_INVALID_INPUT));
        break;
    }
    return ERR_CODE_NONE;
}
#endif /* !SMALL_FOOTPRINT || BLACKHAWK7 */
#ifndef SMALL_FOOTPRINT

err_code_t merlin7_tsc_lane_pwrdn(srds_access_t *sa__, enum srds_core_pwrdn_mode_enum mode) {
    INIT_SRDS_ERR_CODE

    switch(mode) {
    case PWR_ON:
        EFUN(merlin7_tsc_INTERNAL_lane_clkgate(sa__, 0));
        EFUN(merlin7_tsc_lane_pwrdn(sa__, PWR_ON_RX));
        EFUN(merlin7_tsc_lane_pwrdn(sa__, PWR_ON_TX));
        /* setup the correct Merlin7 TSC DCC enable settings when powering up */
        EFUN(merlin7_tsc_apply_dcc_en_cfg(sa__));
        break;
    case PWR_ON_TX:
        EFUN(wr_ln_tx_s_pwrdn(0x0));
        break;
    case PWR_ON_RX:
        EFUN(wr_ln_rx_s_pwrdn(0x0));
        break;
    case PWRDN:
        EFUN(merlin7_tsc_lane_pwrdn(sa__, PWRDN_TX));
        EFUN(merlin7_tsc_lane_pwrdn(sa__, PWRDN_RX));
        break;
    case PWROFF_DEEP: /* PWROFF_DEEP is identical to PWRDN_DEEP for lane pwrdn. FALL THROUGH */
    case PWRDN_DEEP:
        EFUN(merlin7_tsc_lane_pwrdn(sa__, PWRDN_TX_DEEP));
        EFUN(merlin7_tsc_lane_pwrdn(sa__, PWRDN_RX_DEEP));
        EFUN(merlin7_tsc_INTERNAL_lane_clkgate(sa__, 1));
        break;
    case PWRDN_TX:
        EFUN(wr_tx_ln_dp_s_rstb(0x0));
        EFUN(wr_ln_tx_s_pwrdn(0x1));
        break;
    case PWRDN_RX:
        EFUN(wr_rx_ln_dp_s_rstb(0x0));
        EFUN(wr_ln_rx_s_pwrdn(0x1));
        break;
    case PWRDN_TX_DEEP:
        EFUN(wr_tx_ln_dp_s_rstb(0x0));
        EFUN(wr_ln_tx_s_pwrdn(0x1));
        break;
    case PWRDN_RX_DEEP:
        EFUN(wr_rx_ln_dp_s_rstb(0x0));
        EFUN(wr_ln_rx_s_pwrdn(0x1));
        break;
    default:
        return(merlin7_tsc_error(sa__, ERR_CODE_BAD_PTR_OR_INVALID_INPUT));
    }
  return (ERR_CODE_NONE);
}
err_code_t merlin7_tsc_pwrdn_all(srds_access_t *sa__, enum srds_core_pwrdn_mode_enum mode) {
    INIT_SRDS_ERR_CODE
    uint8_t lane_orig = merlin7_tsc_get_lane(sa__);
    uint8_t lane;
    uint8_t lanes_per_core;

    if(mode != PWRDN_DEEP && mode != PWRDN && mode != PWROFF_DEEP) {
        EFUN(merlin7_tsc_error(sa__, ERR_CODE_BAD_PTR_OR_INVALID_INPUT));
    }

    ESTM(lanes_per_core = rdc_revid_multiplicity());
    for (lane = 0; lane < lanes_per_core; ++lane) {
        EFUN(merlin7_tsc_set_lane(sa__,   lane));
        EFUN(merlin7_tsc_lane_pwrdn(sa__, mode));
    }
    EFUN(merlin7_tsc_set_lane(sa__, lane_orig));
        EFUN(merlin7_tsc_core_pwrdn(sa__, mode));
    return ERR_CODE_NONE;
}

#endif /* !SMALL_FOOTPRINT */
