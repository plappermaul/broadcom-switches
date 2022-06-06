/*************************************************************************************
 *                                                                                   *
 * Copyright: (c) 2019 Broadcom.                                                     *
 * Broadcom Proprietary and Confidential. All rights reserved.                       *
 */

/*************************************************************************************
 *************************************************************************************
 *                                                                                   *
 *  Revision      :   *
 *                                                                                   *
 *  Description   :  Defines and Enumerations required by Blackhawk ucode            *
 *                                                                                   *
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.                                                             *
 *  No portions of this material may be reproduced in any form without               *
 *  the written permission of:                                                       *
 *      Broadcom Corporation                                                         *
 *      5300 California Avenue                                                       *
 *      Irvine, CA  92617                                                            *
 *                                                                                   *
 *  All information contained in this document is Broadcom Corporation               *
 *  company private proprietary, and trade secret.                                   *
 */

/** @file blackhawk7_api_uc_common.h
 * Defines and Enumerations shared by Blackhawk IP Specific API and Microcode
 */

#ifndef BLACKHAWK_API_UC_COMMON_H
#define BLACKHAWK_API_UC_COMMON_H

/* Add Blackhawk specific items below this */
#define BLACKHAWK7_L8P2_LANE_TIMER_SHIFT (3)
/* Please note that when adding entries here you should update the #defines in the blackhawk7_l8p2_common.h */

/** OSR_MODES Enum */
enum blackhawk7_l8p2_osr_mode_enum {
  /* If the enumerations change, then consider updating OSR_MODE_SUPPORTS_EYE_TESTS(). */
  BLACKHAWK7_L8P2_OSX1      = 0,
  BLACKHAWK7_L8P2_OSX2      = 1,
  BLACKHAWK7_L8P2_OSX4      = 2,
  BLACKHAWK7_L8P2_OSX2P5    = 3,
  BLACKHAWK7_L8P2_OSX21P25  = 4,
  BLACKHAWK7_L8P2_OSX8      = 5,
  BLACKHAWK7_L8P2_OSX16P5   = 8,
  BLACKHAWK7_L8P2_OSX16     = 9,
  BLACKHAWK7_L8P2_OSX20P625 = 12,
  BLACKHAWK7_L8P2_OSX32     = 13
};

/* UNIQUIFY_PUBLIC_END  - Marker used by API Uniquify script */

/** Return true if an OSR mode supports eye tests. **/
#define OSR_MODE_SUPPORTS_EYE_TESTS(mode) (((mode) <= BLACKHAWK7_L8P2_OSX4) || ((mode) == BLACKHAWK7_L8P2_OSX2P5))

/** CDR mode Enum **/
enum blackhawk7_l8p2_cdr_mode_enum {
  BLACKHAWK7_L8P2_CDR_MODE_OS_ALL_EDGES             = 0,
  BLACKHAWK7_L8P2_CDR_MODE_OS_PATTERN               = 1,
  BLACKHAWK7_L8P2_CDR_MODE_OS_PATTERN_ENHANCED      = 2,
  BLACKHAWK7_L8P2_CDR_MODE_BR_PATTERN               = 3,
  BLACKHAWK7_L8P2_CDR_MODE_PAM4_NR_OS_PATTERN       = 4,
  BLACKHAWK7_L8P2_CDR_MODE_PAM4_NR_BR_PATTERN       = 5,
  BLACKHAWK7_L8P2_CDR_MODE_PAM4_ER_DFE_DATA_PATTERN = 6,
  BLACKHAWK7_L8P2_CDR_MODE_PAM4_ER_PATTERN          = 7
};

/** Lane User Control Clause93/72 Force Value **/
enum blackhawk7_l8p2_cl93n72_frc_val_enum {
  BLACKHAWK7_L8P2_CL93N72_FORCE_OS  = 0,
  BLACKHAWK7_L8P2_CL93N72_FORCE_BR  = 1
};

/** AFE Override Slicer Selection Value **/
typedef enum {
  INVALID_SLICER = 0,
  DATA23_SLICER  = 1,
  DATA05_SLICER  = 3,
  PHASE1_SLICER  = 4,
  PHASE02_SLICER = 5,
  DATA14_SLICER  = 2,
  DFE_TAPS_2_3   = 6,
  LMS_SLICER     = 8
} afe_override_slicer_sel_t;

/* The following functions translate between a VCO frequency in MHz and the
 * vco_rate that is found in the Core Config Variable Structure using the
 * formula:
 *
 *     vco_rate = (frequency_in_ghz * 16.0) - 232.0
 *
 * The valid VCO ranges from 14.5GHz to 30GHz
 *
 * Both functions round to the nearest resulting value.  This
 * provides the highest accuracy possible, and ensures that:
 *
 *     vco_rate == MHZ_TO_VCO_RATE(VCO_RATE_TO_MHZ(vco_rate))
 *
 * In the microcode, these functions should only be called with a numeric
 * literal parameter.
 */
#define MHZ_TO_VCO_RATE(mhz) ((uint8_t)(((((uint16_t)(mhz) * 2) + 62) / 125) - 232))
#define VCO_RATE_TO_MHZ(vco_rate) (((((uint16_t)(vco_rate) + 232) * 125) + 1) >> 1)

/* BOUNDARIES FOR FIR TAP VALUES
 *
 * Hardware limits the sum of the taps to be TXFIR_SUM_LIMIT or TXFIR_PAM4_SUM_LIMIT:
 *
 *     sum(n=0..11, abs(tap[n])) <= TXFIR_NRZ_SUM_LIMIT, if in NRZ mode
 *     sum(n=0..11, abs(tap[n])) <= TXFIR_PAM4_UC_SUM_LIMIT, if in PAM4 mode
 */
#define TXFIR_NRZ_SUM_LIMIT     (127)
#define TXFIR_PAM4_UC_SUM_LIMIT (168)

/*
 * All taps have bitfield limits:
 */
#define TXFIR_NRZ_TAP_MIN     (-127)
#define TXFIR_NRZ_TAP_MAX     ( 127)
#define TXFIR_PAM4_UC_TAP_MIN (-168)
#define TXFIR_PAM4_UC_TAP_MAX ( 168)

/* defined(__arm__) || defined(EMULATION_EN) */

/*
 * Note (c) under Table 72-8 of IEEE 802.3 states that V2 must be greater than
 * 40 mV peak-to-zero in all situations.  We test this using the equation:
 *
 *     abs(main) - abs(pre) - abs(post1) >=
 *         TXFIR_V2_LIMIT
 *
 * 40 mV peak-to-zero corresponds to 80 mV peak-to-peak.
 */
#define TXFIR_NRZ_V2_LIMIT  (12)
#define TXFIR_PAM4_V2_LIMIT (16)

/*
 * Macros for identifying BHK7 versions with different AMS
 */
#define AMS_VERSION_A0     (1)
#define AMS_VERSION_B0     (2)

#define GET_AMS_VERSION(_version)      \
    (((_version) == 0x0 || (_version) == 0x1 || (_version) == 0x2 || (_version) == 0x4 || (_version) == 0x6 || \
      (_version) == 0x9 || (_version) == 0xA) ? (AMS_VERSION_A0) : (AMS_VERSION_B0))

/*
 * Peaking Filter Boundaries
 */
#define PF_MIN_VALUE          ( 0)
#define PF_MAX_VALUE          (15)
#define PF_MAX_VALUE_B0       (31)
#define PF3_MIN_VALUE         ( 0)
#define PF3_MAX_VALUE         (15)
#define PF3_MAX_VALUE_B0      (31)


/* Maximum values for rx_vga_ctrl_val */
#define RX_VGA_CTRL_VAL_MAX (37)



/**************************************************************************
 *               PVTMON code generated by script                          *
 */

/* BEGIN_GENERATED_TEMPERATURE_CODE */

/*
 * The formula for PVTMON is:
 *
 *     T = 356.07000 - 0.23755 * reg_bin
 */
#define _bin_to_degC_double(bin_) (356.07000 - (0.23755 * (USR_DOUBLE)(bin_)))


/* Identify the temperature from the PVTMON output. */
#define _bin_to_degC(bin_) (((((int32_t)(  746732913L) +           \
                               ((int32_t)(bin_) * (    -498178L))) \
                              >> 20) + 1) >> 1)

/* Identify the PVTMON output corresponding to the temperature. */
#define _degC_to_bin(degc_) (((((int32_t)(  196467300L) +           \
                                ((int32_t)(degc_) * (    -551766L))) \
                               >> 16) + 1) >> 1)



/* END_GENERATED_TEMPERATURE_CODE */

#endif
