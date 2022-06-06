/******************************************************************************
 ******************************************************************************
 *  Revision      :  $Id: srds_api_enum.h 1355 2016-02-12 23:21:46Z cvazquez $ *
 *                                                                            *
 *  Description   :  Enum types used by Serdes API functions                  *
 *                                                                            *
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$                                                      *
 *  No portions of this material may be reproduced in any form without        *
 *  the written permission of:                                                *
 *      Broadcom Corporation                                                  *
 *      5300 California Avenue                                                *
 *      Irvine, CA  92617                                                     *
 *                                                                            *
 *  All information contained in this document is Broadcom Corporation        *
 *  company private proprietary, and trade secret.                            *
 *                                                                            *
 ******************************************************************************
 ******************************************************************************/

/** @file srds_api_enum.h
 * Enum types used by Serdes API functions
   This file contains Enum's which are common to all serdes cores
 */

#ifndef SRDS_API_ENUM_PUBLIC_H
#define SRDS_API_ENUM_PUBLIC_H

/** PRBS Polynomial Enum */
enum srds_prbs_polynomial_enum {
  SRDS_PRBS_7  = 0,
  SRDS_PRBS_9  = 1,
  SRDS_PRBS_11 = 2,
  SRDS_PRBS_15 = 3,
  SRDS_PRBS_23 = 4,
  SRDS_PRBS_31 = 5,
  SRDS_PRBS_58 = 6
};

#define srds_return_prbs_order(val) ((val==0)?7:  \
                                (val==1)?9:  \
                                (val==2)?11: \
                                (val==3)?15: \
                                (val==4)?23: \
                                (val==5)?31: \
                                (val==6)?58:0)

/** PRBS Checker Mode Enum */
enum srds_prbs_checker_mode_enum {
  SRDS_PRBS_SELF_SYNC_HYSTERESIS       = 0,
  SRDS_PRBS_INITIAL_SEED_HYSTERESIS    = 1,
  SRDS_PRBS_INITIAL_SEED_NO_HYSTERESIS = 2
};


/** TX_PI Fixed Frequency and Jitter Generation Modes Enum */
enum srds_tx_pi_freq_jit_gen_enum {
  SRDS_TX_PI_FIXED_FREQ      = 0,
  SRDS_TX_PI_SSC_LOW_FREQ    = 1,
  SRDS_TX_PI_SSC_HIGH_FREQ   = 2,
  SRDS_TX_PI_SJ              = 3
};

/** RX AFE Settings Enum */
enum srds_rx_afe_settings_enum {
    SRDS_RX_AFE_PF,
    SRDS_RX_AFE_PF2,
    SRDS_RX_AFE_VGA,
    SRDS_RX_AFE_DFE1,
    SRDS_RX_AFE_DFE2,
    SRDS_RX_AFE_DFE3,
    SRDS_RX_AFE_DFE4,
    SRDS_RX_AFE_DFE5,
    SRDS_RX_AFE_DFE6,
    SRDS_RX_AFE_DFE7,
    SRDS_RX_AFE_DFE8,
    SRDS_RX_AFE_DFE9,
    SRDS_RX_AFE_DFE10,
    SRDS_RX_AFE_DFE11,
    SRDS_RX_AFE_DFE12,
    SRDS_RX_AFE_DFE13,
    SRDS_RX_AFE_DFE14
};

/** TX AFE Settings Enum */
enum srds_tx_afe_settings_enum {
    SRDS_TX_AFE_PRE,
    SRDS_TX_AFE_MAIN,
    SRDS_TX_AFE_POST1,
    SRDS_TX_AFE_POST2,
    SRDS_TX_AFE_POST3,
    SRDS_TX_AFE_AMP,
    SRDS_TX_AFE_DRIVERMODE
};

/** AFE Drivermode Enum */
enum srds_afe_drivermode_enum {
    SRDS_DM_DEFAULT = 0,
    SRDS_DM_NOT_SUPPORTED,
    SRDS_DM_HALF_AMPLITUDE,
    SRDS_DM_HALF_AMPLITUDE_HI_IMPED
};

/** Repeater Mode Enum */
enum srds_rptr_mode_enum {
    SRDS_DATA_IN_SIDE,
    SRDS_DIG_LPBK_SIDE,
    SRDS_RMT_LPBK_SIDE
};

/** Core Powerdown Modes Enum */
enum srds_core_pwrdn_mode_enum {
    SRDS_PWR_ON,
    SRDS_PWRDN,
    SRDS_PWRDN_DEEP,
    SRDS_PWRDN_TX,
    SRDS_PWRDN_RX
};

/** Evenet Log Display Mode Enum */
enum srds_event_log_display_mode_enum {
    SRDS_EVENT_LOG_HEX,
    SRDS_EVENT_LOG_DECODED,
    SRDS_EVENT_LOG_HEX_AND_DECODED,
    SRDS_EVENT_LOG_MODE_MAX
};

/** Eye Scan Direction Enum */
enum srds_eye_scan_dir_enum {
      SRDS_EYE_SCAN_HORIZ = 0,
      SRDS_EYE_SCAN_VERTICAL = 1
};


/* Used by #serdes_display_diag_data to select diag_level             */
/* example:                                                           */
/*  diag_level = SRDS_DIAG_LANE | SRDS_DIAG_CORE | SRDS_DIAG_EVENT    */
enum srds_diag_level_enum {
    SRDS_DIAG_LANE = 1,
    SRDS_DIAG_CORE = 1<<1,
    SRDS_DIAG_EVENT = 1<<2,
    SRDS_DIAG_EYE = 1<<3,
    SRDS_DIAG_REG_CORE = 1<<4,
    SRDS_DIAG_REG_LANE = 1<<5,
    SRDS_DIAG_UC_CORE = 1<<6,
    SRDS_DIAG_UC_LANE = 1<<7,
    SRDS_DIAG_LANE_DEBUG = 1<<8,
    SRDS_DIAG_BER_VERT = 1<<9,
    SRDS_DIAG_BER_HORZ = 1<<10
};

/* Used by #serdes_diag_access to select type of access */
enum srds_diag_access_enum {
    SRDS_REG_READ,
    SRDS_REG_RMW,
    SRDS_CORE_RAM_READ_BYTE,
    SRDS_CORE_RAM_RMW_BYTE,
    SRDS_CORE_RAM_READ_WORD,
    SRDS_CORE_RAM_RMW_WORD,
    SRDS_LANE_RAM_READ_BYTE,
    SRDS_LANE_RAM_RMW_BYTE,
    SRDS_LANE_RAM_READ_WORD,
    SRDS_LANE_RAM_RMW_WORD,
    SRDS_GLOB_RAM_READ_BYTE,
    SRDS_GLOB_RAM_RMW_BYTE,
    SRDS_GLOB_RAM_READ_WORD,
    SRDS_GLOB_RAM_RMW_WORD,
    SRDS_UC_CMD,
    SRDS_EN_BREAKPOINT,
    SRDS_GOTO_BREAKPOINT,
    SRDS_RD_BREAKPOINT,
    SRDS_DIS_BREAKPOINT,
    SRDS_BER_PROJ_DATA
};

#endif
