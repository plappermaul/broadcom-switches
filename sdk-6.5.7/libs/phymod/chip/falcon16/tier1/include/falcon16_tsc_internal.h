/**********************************************************************************
 **********************************************************************************
 *                                                                                *
 *  Revision    :   *
 *                                                                                *
 *  Description :  Internal API functions                                         *
 *                                                                                *
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$                                                          *
 *  No portions of this material may be reproduced in any form without            *
 *  the written permission of:                                                    *
 *      Broadcom Corporation                                                      *
 *      5300 California Avenue                                                    *
 *      Irvine, CA  92617                                                         *
 *                                                                                *
 *  All information contained in this document is Broadcom Corporation            *
 *  company private proprietary, and trade secret.                                *
 *                                                                                *
 **********************************************************************************
 **********************************************************************************/

/** @file falcon16_tsc_internal.h
 * Internal API functions
 */

#ifndef FALCON16_TSC_API_INTERNAL_H
#define FALCON16_TSC_API_INTERNAL_H

#include "falcon16_tsc_ipconfig.h"
#include "falcon16_tsc_select_defns.h"
#include "common/srds_api_err_code.h"
#include "common/srds_api_uc_common.h"
#include "common/srds_api_types.h"
#include "falcon16_tsc_enum.h"
#include "falcon16_tsc_types.h"
#include "falcon16_tsc_usr_includes.h"

/*------------------------------*/
/** Serdes OSR Mode Structure   */
/*------------------------------*/
typedef struct {
  /** TX OSR Mode */
  uint8_t tx;
  /** RX OSR Mode */
  uint8_t rx;
  /** OSR Mode for TX and RX (used when both TX and RX should have same OSR Mode) */
  uint8_t tx_rx;
}falcon16_tsc_osr_mode_st;

/*------------------------------*/
/** Serdes Lane State Structure */
/*------------------------------*/
typedef struct {
  /** uC lane configuration */
  uint16_t ucv_config;
  /** Frequency offset of local reference clock with respect to RX data in ppm */
  int16_t rx_ppm;
  /** Vertical threshold voltage of p1 slicer (mV) */
  int16_t p1_lvl;
  /** Vertical threshold voltage of m1 slicer (mV) */
  int16_t m1_lvl;
  /** Link time in milliseconds */
  uint16_t link_time;
  /** OSR Mode */
  falcon16_tsc_osr_mode_st osr_mode;
  /** Signal Detect */
  uint8_t sig_det;
  uint8_t sig_det_chg;
  /** PMD RX Lock */
  uint8_t rx_lock;
  uint8_t rx_lock_chg;
  /** Delay of zero crossing slicer, m1, wrt to data in PI codes */
  int8_t clk90;
  /** Delay of diagnostic/lms slicer, p1, wrt to data in PI codes */
  int8_t clkp1;
  /** Peaking Filter Main Settings */
  int8_t pf_main;
  /** Peaking Filter Hiz mode enable */
  int8_t pf_hiz;
  /** Peaking Filter DC gain adjustment for CTLE */
  int8_t pf_bst;
  /** Low Frequency Peaking filter control */
  int8_t pf2_ctrl;
  /** Variable Gain Amplifier settings */
  int8_t vga;
  /** DC offset DAC control value */
  int8_t dc_offset;
  /** P1 eyediag status */
  int8_t p1_lvl_ctrl;
  /** DFE tap 1 value */
  int8_t dfe1;
  /** DFE tap 2 value */
  int8_t dfe2;
  /** DFE tap 3 value */
  int8_t dfe3;
  /** DFE tap 4 value */
  int8_t dfe4;
  /** DFE tap 5 value */
  int8_t dfe5;
  /** DFE tap 6 value */
  int8_t dfe6;
  /** DFE tap 1 Duty Cycle Distortion */
  int8_t dfe1_dcd;
  /** DFE tap 2 Duty Cycle Distortion */
  int8_t dfe2_dcd;
  /** Slicer calibration control codes (p1 even) */
  int8_t pe;
  /** Slicer calibration control codes (data even) */
  int8_t ze;
  /** Slicer calibration control codes (m1 even) */
  int8_t me;
  /** Slicer calibration control codes (p1 odd) */
  int8_t po;
  /** Slicer calibration control codes (data odd) */
  int8_t zo;
  /** Slicer calibration control codes (m1 odd) */
  int8_t mo;
  /** Frequency offset of local reference clock with respect to TX data in ppm */
  int16_t tx_ppm;
  /** TX equalization FIR pre tap weight */
  int8_t txfir_pre;
  /** TX equalization FIR main tap weight */
  int8_t txfir_main;
  /** TX equalization FIR post1 tap weight */
  int8_t txfir_post1;
  /** TX equalization FIR post2 tap weight */
  int8_t txfir_post2;
  /** TX equalization FIR post3 tap weight */
  int8_t txfir_post3;
  /** TX equalization FIR rpara tap weight */
  int8_t txfir_rpara;
  /** Horizontal left eye margin @ 1e-5 as seen by internal diagnostic slicer in mUI and mV */
  uint16_t heye_left;
  /** Horizontal right eye margin @ 1e-5 as seen by internal diagnostic slicer in mUI and mV */
  uint16_t heye_right;
  /** Vertical upper eye margin @ 1e-5 as seen by internal diagnostic slicer in mUI and mV */
  uint16_t veye_upper;
  /** Vertical lower eye margin @ 1e-5 as seen by internal diagnostic slicer in mUI and mV */
  uint16_t veye_lower;
  /** Baud Rate Phase Detector enable */
  uint8_t br_pd_en;
  /** lane_reset_state **/
  uint8_t reset_state;
  /** lane_tx_reset_state **/
  uint8_t tx_reset_state;
  /** uC stopped state **/
  uint8_t stop_state;
  /** Sigdet offset correction - positive **/
  uint8_t soc_pos;
  /** Sigdet offset correction - negative **/
  uint8_t soc_neg;
} falcon16_tsc_lane_state_st;

/*------------------------------*/
/** Serdes Core State Structure */
/*------------------------------*/
typedef struct {
  /** Core DP Reset State */
  uint8_t  core_reset;
  /**  PLL Powerdown enable */
  uint8_t  pll_pwrdn;
  /** Micro active enable */
  uint8_t  uc_active;
  /** Comclk Frequency in Mhz */
  uint16_t comclk_mhz;
  /** uCode Major Version number */
  uint16_t ucode_version;
  /** uCode Minor Version number */
  uint8_t  ucode_minor_version;
  /** AFE Hardware version */
  uint8_t  afe_hardware_version;
  /** uC Die Temperature Index */
  uint8_t  temp_idx;
  /** Average Die Temperature (13-bit format) */
  uint16_t avg_tmon;
  /** Analog Resistor Calibration value */
  uint8_t  rescal;
  /** VCO Rate in MHz */
  uint16_t vco_rate_mhz;
  /**  Analog VCO Range */
  uint8_t  analog_vco_range;
  /** PLL Divider value.  (Same encoding as enum falcon16_tsc_pll_div_enum.) */
  uint32_t pll_div;
  /** PLL Lock */
  uint8_t  pll_lock;
  uint8_t  pll_lock_chg;
  /** Live die temperature in Celsius */
  int16_t die_temp;
  /** Core Status Variable */
  uint8_t core_status;
  /** Refclk doubler enable */
  uint8_t refclk_doubler;
} falcon16_tsc_core_state_st;

/************************************************************************//**
 * @name Direct RAM Access
 *
 * Cores/chips with a built-in microcontroller afford direct, memory-mapped
 * access to the firmware control/status RAM variables.
 *//*************************************************************************/
/**@{*/


/** Base of core variable block, FALCON16 variant. */
#define CORE_VAR_RAM_BASE (0x400)
/** Base of lane variable block, FALCON16 variant. */
#define LANE_VAR_RAM_BASE (0x480)
/** Size of lane variable block, FALCON16 variant. */
#define LANE_VAR_RAM_SIZE (0x130)


/**@}*/

typedef struct {
    uint32_t signature;
    uint32_t diag_mem_ram_base;
    uint32_t diag_mem_ram_size;
    uint32_t core_var_ram_base; /* Must be the same as CORE_VAR_RAM_BASE */
    uint32_t core_var_ram_size;
    uint32_t lane_var_ram_base; /* Must be the same as LANE_VAR_RAM_BASE */
    uint32_t lane_var_ram_size; /* Must be the same as LANE_VAR_RAM_SIZE */
    uint32_t trace_mem_ram_base;
    uint32_t trace_mem_ram_size;
    uint32_t micro_var_ram_base;
    uint8_t  lane_count;
    uint8_t  trace_memory_descending_writes;
    uint8_t  micro_count;
    uint8_t  micro_var_ram_size;
    } falcon16_tsc_info_t;

#define SRDS_INFO_SIGNATURE (0x50c1ab1e)

falcon16_tsc_info_t *falcon16_tsc_INTERNAL_get_falcon16_tsc_info_ptr(void);

/** Verify the internal falcon16_tsc_info.
 * @return Error code, if generated (returns ERR_CODE_NONE if no errors)
 */
err_code_t falcon16_tsc_INTERNAL_verify_falcon16_tsc_info(falcon16_tsc_info_t const *test_info);


/** Check if the micro's operations on that lane are stopped.
 * @param sa__ is an opaque state vector passed through to device access functions.
 * @return err_code Error Code "ERR_CODE_UC_NOT_STOPPED" returned if micro NOT stopped
 */
err_code_t falcon16_tsc_INTERNAL_check_uc_lane_stopped(srds_access_t *sa__);

/** Calculate the mode_sel parameter for tx pattern generator.
 * @param *mode_sel Mode select to be used for generating required pattern
 * @param *zero_pad_len Length of zero padding to be used for generating required pattern
 * @param patt_length Desired Pattern length
 * @return Error Code, if generated (returns ERR_CODE_NONE if no errors)
 */
err_code_t falcon16_tsc_INTERNAL_calc_patt_gen_mode_sel(uint8_t *mode_sel, uint8_t *zero_pad_len, uint8_t patt_length);

/*-----------------------------------------*/
/*  Write Core Config variables to uC RAM  */
/*-----------------------------------------*/

/** Write to core_config uC RAM variable.
 * @param sa__ is an opaque state vector passed through to device access functions.
 * @param struct_val Value to be written into core_config RAM variable.
 *                   (Note that struct_val.word must be = 0, only the fields are used)
 * @return Error Code, if generated (returns ERR_CODE_NONE if no errors)
 */
err_code_t falcon16_tsc_INTERNAL_set_uc_core_config(srds_access_t *sa__, struct falcon16_tsc_uc_core_config_st struct_val);

/*---------------------*/
/*  PLL Configuration  */
/*---------------------*/

/** Return whether the fraction portion of a falcon16_tsc_pll_div_enum value is nonzero. */
#define SRDS_INTERNAL_IS_PLL_DIV_FRACTIONAL(div_) (((uint32_t)(div_) & 0xFFFFF000UL) != 0)

/** Extract the integer portion of a falcon16_tsc_pll_div_enum value. */
#define SRDS_INTERNAL_GET_PLL_DIV_INTEGER(div_) (((uint32_t)(div_)) & 0x00000FFFUL)

/** Extract the fraction portion of a falcon16_tsc_pll_div_enum value.
 *  The result would be suitable for the numerator of a fraction whose denominator is 2^(width_).
 *  width_ must be less than 32.
 */
#define SRDS_INTERNAL_GET_PLL_DIV_FRACTION_NUM(div_, width_)            \
    (((((uint32_t)(div_) & 0xFFFFF000UL) >> (32-(width_)-1)) + 1) >> 1)

/** Compose a value in the same format as a falcon16_tsc_pll_div_enum value.
 *  (Though it's the same format, a U32 is used, because rounding may yield a non-enumerated result.)
 *  The fractional portion is fraction_num / (2^fraction_num_width_).
 *  fraction_num_width_ must be 32 or less.
 */
#define SRDS_INTERNAL_COMPOSE_PLL_DIV(integer_, fraction_num_, fraction_num_width_)                   \
    (((uint32_t)(integer_) & 0xFFFUL)                                                                 \
     | ((((((uint32_t)(fraction_num_) << (32-(fraction_num_width_))) >> (32-20-1)) + 1) >> 1) << 12))

/** Resolve PLL parameters.
 * PLL configuration requires two out of the three parameters:  refclk frequency, divider value, and VCO output frequency.
 * @param refclk Reference clock frequency (enumerated), or FALCON16_TSC_PLL_REFCLK_UNKNOWN to auto-compute.
 * @param *refclk_freq_hz Resultant reference clock frequency, in Hz.
 * @param *div Divider value (enumerated), or FALCON16_TSC_PLL_DIV_UNKNOWN to auto-compute.
 *             The final divider value will be filled in if FALCON16_TSC_PLL_DIV_UNKNOWN.
 * @param *vco_freq_khz VCO output frequency, in kHz, or 0 to auto-compute.
 *                      The final VCO output frequency will be filled in if 0.
 * @param refclk_doubler_en Enable Refclk doubler.
 * @return Error Code, if generated (returns ERR_CODE_NONE if no errors)
 */
err_code_t falcon16_tsc_INTERNAL_resolve_pll_parameters(enum falcon16_tsc_pll_refclk_enum refclk,
                                                  uint32_t *refclk_freq_hz,
                                                  enum falcon16_tsc_pll_div_enum *div,
                                                  uint32_t *vco_freq_khz,
                                                  uint8_t refclk_doubler_en);

/** Configure PLL.
 *
 * Use core_s_rstb to re-initialize all registers to default before calling this function.
 *
 * Configures PLL registers to obtain the required configuration.
 * PLL configuration requires two out of the three parameters:  refclk frequency, divider value, and VCO output frequency.
 * @param sa__ is an opaque state vector passed through to device access functions.
 * @param refclk Reference clock frequency (enumerated), or FALCON16_TSC_PLL_REFCLK_UNKNOWN to auto-compute.
 * @param div Divider value (enumerated), or FALCON16_TSC_PLL_DIV_UNKNOWN to auto-compute.
 * @param vco_freq_khz VCO output frequency, in kHz, or 0 to auto-compute.
 * @param refclk_doubler_en Enable Refclk doubler.
 * @return Error Code, if generated (returns ERR_CODE_NONE if no errors)
 */
err_code_t falcon16_tsc_INTERNAL_configure_pll(srds_access_t *sa__,
                                         enum falcon16_tsc_pll_refclk_enum refclk,
                                         enum falcon16_tsc_pll_div_enum div,
                                         uint32_t vco_freq_khz,
                                         uint8_t refclk_doubler_en);

/** Read the programmed PLL div value from the SERDES.
 *  Due to rounding when writing the div value to the PLL,
 *      the returned result may be slightly different from what was written.
 * @param sa__ is an opaque state vector passed through to device access functions.
 * @param *div Divider value, in the same encoding as enum falcon16_tsc_pll_div_enum.
 * @return Error Code, if generated (returns ERR_CODE_NONE if no errors)
 */
err_code_t falcon16_tsc_INTERNAL_read_pll_div(srds_access_t *sa__, uint32_t *div);

/** Convert the PLL_DIV to actual PLL divider value and display
 * as part of falcon16_tsc_display_core_state_line()
 * @param sa__ is an opaque state vector passed through to device access functions.
 * @param div Divider value, in the same encoding as enum falcon16_tsc_pll_div_enum.
 * @return Error Code, if generated (returns ERR_CODE_NONE if no errors)
 */
err_code_t falcon16_tsc_INTERNAL_display_pll_to_divider(srds_access_t *sa__, uint32_t div);

/*-------------------*/
/*  Ladder controls  */
/*-------------------*/
/** Converts a ladder setting to mV, given the range.
 * @param sa__ is an opaque state vector passed through to device access functions.
 * @param ctrl is the threshold control (-31..31) maps to -RANGE to RANGE in non-uniform steps
 * @param range_250 determines the range 0 = +/-150mV, 1 = +/-250mV
 * @return ladder threshold voltage in mV
 */
int16_t falcon16_tsc_INTERNAL_ladder_setting_to_mV(srds_access_t *sa__, int8_t ctrl, uint8_t range_250);



/*-----------------------*/
/*  TX_PI and ULL Setup  */
/*-----------------------*/

/** Safe multiply - multiplies 2 numbers and checks for overflow.
 * @param a First input
 * @param b Second input
 * @param *of Pointer to overflow indicator
 * @return value of a * b
 */
uint32_t falcon16_tsc_INTERNAL_mult_with_overflow_check(uint32_t a, uint32_t b, uint8_t *of);

/*-----------------------------------------*/
/*  APIs used in Config Shared TX Pattern  */
/*-----------------------------------------*/
/** Compute Binary string for a Hex value ['0' to 'F'].
 * @param var Hex value to be converted to Binary (eg: '6', 'A', ...)
 * @param bin Binary string returned by API (eg: '0110', '1010', ...)
 * @return Error Code generated by invalid hex variable (returns ERR_CODE_NONE if no errors)
 */
err_code_t falcon16_tsc_INTERNAL_compute_bin(char var, char bin[]);

/** Compute Hex value for a Binary string ['0000' to '1111'].
 * @param bin Binary string to be coverted (eg: '0110', '1010', ...)
 * @param *hex Hex value calculated from the input Binary string
 * @return Error Code generated by invalid Binary string (returns ERR_CODE_NONE if no errors)
 */
err_code_t falcon16_tsc_INTERNAL_compute_hex(char bin[], uint8_t *hex);


/*-----------------------------------*/
/*  APIs used in Read Event Logger   */
/*-----------------------------------*/

/* State variable for dumping event log.
 * This must be initialized with zeros.
 */
typedef struct {
    uint16_t index, line_start_index;
} falcon16_tsc_INTERNAL_event_log_dump_state_t;

/** Callback function for dumping event log, intended to be called by falcon16_tsc_INTERNAL_read_event_log_with_callback.
 *  It must also be called once after, with a byte count of 0, to finish printing.
 * @param *arg is a falcon16_tsc_INTERNAL_event_log_dump_state_t pointer for holding state.
 * @param byte_count is the number of bytes to dump.
 *        -# This function is called repeatedly with a byte count of 2 with U16 values to dump.
 *        -# This function may then be called with a byte count of 1 if the event log has an odd number of bytes.
 *        -# Finally, this function must be called with a byte count of 0 to finish up.
 * @param data is the event log data to dump.
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors)
 */
err_code_t falcon16_tsc_INTERNAL_event_log_dump_callback(void *arg, uint8_t byte_count, uint16_t data);

/** Get Event Log from uC, and call callback for every two bytes.
 * @param sa__ is an opaque state vector passed through to device access functions.
 * @param micro_num is the number of the microcontroller whose event log should be read.
 * @param bypass_micro is 1 if the micro should not be involved (by stopping event log).
 *                     This mode works even when the micro is hung, and it does not consume the event log.
 * @param *arg is passed as the first argument to callback
 * @param *callback is called with all of the data read, two bytes at a time.
 *                  The last call of callback may have one byte; in that case, the upper byte is undefined.
 *                  The call is in the form:  callback(arg, byte_count, data)
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors)
 */
err_code_t falcon16_tsc_INTERNAL_read_event_log_with_callback(srds_access_t *sa__,
                                                        uint8_t micro_num,
                                                        uint8_t bypass_micro,
                                                        void *arg,
                                                        err_code_t (*callback)(void *, uint8_t, uint16_t));

/** Convert float8 to usigned int32.
 * uint32 = 1.XXX * 2^Y  where float8 bits are XXXYYYYY
 * @param input Float8 number
 * @return Usigned 32bit number
 */
uint32_t falcon16_tsc_INTERNAL_float8_to_int32(float8_t input);


#ifdef TO_FLOATS
/*-----------------------------------*/
/*  APIs used in uC data conversion  */
/*-----------------------------------*/

/** Convert usigned int32 to float8.
 * uint32 = 1.XXX * 2^Y  where float8 bits are XXXYYYYY
 * @param input Unsigned int
 * @return Float8 8 bit representations of 32bit number
 */
float8_t falcon16_tsc_INTERNAL_int32_to_float8(uint32_t input);
#endif

/** Convert float12 to usigned int32.
 * uint32 = XXXXXXXX * 2^YYYY  where float12 bits X=byte and Y=multi
 * @param byte Float8 8bit
 * @param multi 4 bit multipier
 * @return Usigned 32bit number
 */
uint32_t falcon16_tsc_INTERNAL_float12_to_uint32(uint8_t byte, uint8_t multi);


/*-----------------------------*/
/*  Read / Display Core state  */
/*-----------------------------*/
/** Read current falcon16_tsc core status.
 * @param sa__ is an opaque state vector passed through to device access functions.
 * @param *istate Current falcon16_tsc core status read back and populated by the API
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors)
 */
err_code_t falcon16_tsc_INTERNAL_read_core_state(srds_access_t *sa__, falcon16_tsc_core_state_st *istate);

/** Display current core state.
 * Reads and displays all important core state values.
 * @param sa__ is an opaque state vector passed through to device access functions.
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors)
 */
err_code_t falcon16_tsc_INTERNAL_display_core_state_no_newline(srds_access_t *sa__);

/*-----------------------------*/
/*  Read / Display Lane state  */
/*-----------------------------*/
/** Read current falcon16_tsc lane status.
 * @param sa__ is an opaque state vector passed through to device access functions.
 * @param *istate Current falcon16_tsc lane status read back and populated by the API
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors)
 */
err_code_t falcon16_tsc_INTERNAL_read_lane_state(srds_access_t *sa__, falcon16_tsc_lane_state_st *istate);


/*-----------------*/
/*  Get OSR mode   */
/*-----------------*/
/** Read current falcon16_tsc lane status.
 * @param sa__ is an opaque state vector passed through to device access functions.
 * @param *mode Returns with the osr mode structure
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors)
 */
err_code_t falcon16_tsc_INTERNAL_get_osr_mode(srds_access_t *sa__, falcon16_tsc_osr_mode_st *mode);

/*-------------------*/
/*  Get LCK status   */
/*-------------------*/
/** Read current PMD Lock and PMD Lock change status of a lane.
 * @param sa__ is an opaque state vector passed through to device access functions.
 * @param *pmd_lock Current falcon16_tsc lane pmd_rx_lock status read back and populated by the API
 * @param *pmd_lock_chg Current falcon16_tsc lane pmd_rx_lock_change status read back and populated by the API
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors)
 */
err_code_t falcon16_tsc_INTERNAL_pmd_lock_status(srds_access_t *sa__, uint8_t *pmd_lock, uint8_t *pmd_lock_chg);

/** Read current Signal_detect and Signal_detect change status of a lane.
 * @param sa__ is an opaque state vector passed through to device access functions.
 * @param *sig_det Current falcon16_tsc lane signal_detect status read back and populated by the API
 * @param *sig_det_chg Current falcon16_tsc lane signal_detect_change status read back and populated by the API
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors)
 */
err_code_t falcon16_tsc_INTERNAL_sigdet_status(srds_access_t *sa__, uint8_t *sig_det, uint8_t *sig_det_chg);

/** Read current PLL Lock and PLL Lock change status of a core.
 * @param sa__ is an opaque state vector passed through to device access functions.
 * @param *pll_lock Current falcon16_tsc lane pll_lock status read back and populated by the API
 * @param *pll_lock_chg Current falcon16_tsc lane pll_lock_change status read back and populated by the API
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors)
 */
err_code_t falcon16_tsc_INTERNAL_pll_lock_status(srds_access_t *sa__, uint8_t *pll_lock, uint8_t *pll_lock_chg);

/** Display current lane state.
 * Reads and displays all important lane state values.
 * @param sa__ is an opaque state vector passed through to device access functions.
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors)
 */
err_code_t falcon16_tsc_INTERNAL_display_lane_state_no_newline(srds_access_t *sa__);

/** Convert eye margin to mV.
 * Takes in the ladder setting with 3 fractional bits and converts to mV.
 * @param sa__ is an opaque state vector passed through to device access functions.
 * @param var Ladder setting with 3 fractional bits
 * @param ladder_range Specified if ladder is configured for 150mV or 250mV range
 * @return Eye opening in mV
 */
uint16_t falcon16_tsc_INTERNAL_eye_to_mV(srds_access_t *sa__, uint8_t var, uint8_t ladder_range);

/** Convert eye margin to mUI.
 * Takes in a horizontal margin in Phase Interpolator codes and converts it to mUI.
 * @param sa__ is an opaque state vector passed through to device access functions.
 * @param var Horizontal margin in Phase Interpolator codes with 3 fractional bits
 * @return Eye opening in mV
 */
uint16_t falcon16_tsc_INTERNAL_eye_to_mUI(srds_access_t *sa__, uint8_t var);

/** Serdes Core ClockGate.
 * Along with falcon16_tsc_core_clkgate(), all lanes should also be clock gated using falcon16_tsc_lane_clkgate() to complete a Core Clockgate
 * @param sa__ is an opaque state vector passed through to device access functions.
 * @param enable Enable clockgate (1 = Enable clokgate; 0 = Disable clockgate)
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors)
 */
err_code_t falcon16_tsc_INTERNAL_core_clkgate(srds_access_t *sa__, uint8_t enable);

/** Serdes Lane ClockGate.
 * @param sa__ is an opaque state vector passed through to device access functions.
 * @param enable Enable lane clockgate (1 = Enable clockgate; 0 = Disable clockgate)
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors)
 */
err_code_t falcon16_tsc_INTERNAL_lane_clkgate(srds_access_t *sa__, uint8_t enable);


/** Set function for PF.
 * @param sa__ is an opaque state vector passed through to device access functions.
 * @param val Signed input value
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors)
 */
err_code_t falcon16_tsc_INTERNAL_set_rx_pf_main(srds_access_t *sa__, uint8_t val);

/** Get function for PF
 * @param sa__ is an opaque state vector passed through to device access functions.
 * @param *val PF read value
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors)
 */
err_code_t falcon16_tsc_INTERNAL_get_rx_pf_main(srds_access_t *sa__, int8_t *val);

/** Set function for PF2.
 * @param sa__ is an opaque state vector passed through to device access functions.
 * @param val signed input value
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors)
 */
err_code_t falcon16_tsc_INTERNAL_set_rx_pf2(srds_access_t *sa__, uint8_t val);

/** Get function for PF2.
 * @param sa__ is an opaque state vector passed through to device access functions.
 * @param *val PF2 read value
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors)
 */
err_code_t falcon16_tsc_INTERNAL_get_rx_pf2(srds_access_t *sa__, int8_t *val);

/** Set function for VGA.
 * @param sa__ is an opaque state vector passed through to device access functions.
 * @param val signed input value
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors)
 */
err_code_t falcon16_tsc_INTERNAL_set_rx_vga(srds_access_t *sa__, uint8_t val);

/** Get function for VGA.
 * @param sa__ is an opaque state vector passed through to device access functions.
 * @param *val VGA read value
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors)
 */
err_code_t falcon16_tsc_INTERNAL_get_rx_vga(srds_access_t *sa__, int8_t *val);


/** Set function for DFE Tap1.
 * @param sa__ is an opaque state vector passed through to device access functions.
 * @param val signed input value
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors)
 */
err_code_t falcon16_tsc_INTERNAL_set_rx_dfe1(srds_access_t *sa__, int8_t val);

/** Get function for DFE Tap1.
 * @param sa__ is an opaque state vector passed through to device access functions.
 * @param *val DFE1 read value
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors)
 */
err_code_t falcon16_tsc_INTERNAL_get_rx_dfe1(srds_access_t *sa__, int8_t *val);

/** Set function for DFE Tap2.
 * @param sa__ is an opaque state vector passed through to device access functions.
 * @param val signed input value
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors)
 */
err_code_t falcon16_tsc_INTERNAL_set_rx_dfe2(srds_access_t *sa__, int8_t val);

/** Get function for DFE Tap2.
 * @param sa__ is an opaque state vector passed through to device access functions.
 * @param *val DFE2 read value
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors)
 */
err_code_t falcon16_tsc_INTERNAL_get_rx_dfe2(srds_access_t *sa__, int8_t *val);

/** Set function for DFE Tap3.
 * @param sa__ is an opaque state vector passed through to device access functions.
 * @param val signed input value
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors)
 */
err_code_t falcon16_tsc_INTERNAL_set_rx_dfe3(srds_access_t *sa__, int8_t val);

/** Get function for DFE Tap3.
 * @param sa__ is an opaque state vector passed through to device access functions.
 * @param *val DFE3 read value
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors)
 */
err_code_t falcon16_tsc_INTERNAL_get_rx_dfe3(srds_access_t *sa__, int8_t *val);

/** Set function for DFE Tap4.
 * @param sa__ is an opaque state vector passed through to device access functions.
 * @param val signed input value
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors)
 */
err_code_t falcon16_tsc_INTERNAL_set_rx_dfe4(srds_access_t *sa__, int8_t val);

/** Get function for DFE Tap4.
 * @param sa__ is an opaque state vector passed through to device access functions.
 * @param *val DFE4 read value
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors)
 */
err_code_t falcon16_tsc_INTERNAL_get_rx_dfe4(srds_access_t *sa__, int8_t *val);

/** Set function for DFE Tap5.
 * @param sa__ is an opaque state vector passed through to device access functions.
 * @param val signed input value
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors)
 */
err_code_t falcon16_tsc_INTERNAL_set_rx_dfe5(srds_access_t *sa__, int8_t val);

/** Get function for DFE Tap5
 * @param sa__ is an opaque state vector passed through to device access functions.
 * @param *val DFE5 read value
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors)
 */
err_code_t falcon16_tsc_INTERNAL_get_rx_dfe5(srds_access_t *sa__, int8_t *val);

/* Set/Get DFE tap values */
err_code_t falcon16_tsc_INTERNAL_set_rx_dfe6(srds_access_t *sa__, int8_t val);
err_code_t falcon16_tsc_INTERNAL_get_rx_dfe6(srds_access_t *sa__, int8_t *val);
err_code_t falcon16_tsc_INTERNAL_set_rx_dfe7(srds_access_t *sa__, int8_t val);
err_code_t falcon16_tsc_INTERNAL_get_rx_dfe7(srds_access_t *sa__, int8_t *val);
err_code_t falcon16_tsc_INTERNAL_set_rx_dfe8(srds_access_t *sa__, int8_t val);
err_code_t falcon16_tsc_INTERNAL_get_rx_dfe8(srds_access_t *sa__, int8_t *val);
err_code_t falcon16_tsc_INTERNAL_set_rx_dfe9(srds_access_t *sa__, int8_t val);
err_code_t falcon16_tsc_INTERNAL_get_rx_dfe9(srds_access_t *sa__, int8_t *val);
err_code_t falcon16_tsc_INTERNAL_set_rx_dfe10(srds_access_t *sa__, int8_t val);
err_code_t falcon16_tsc_INTERNAL_get_rx_dfe10(srds_access_t *sa__, int8_t *val);
err_code_t falcon16_tsc_INTERNAL_set_rx_dfe11(srds_access_t *sa__, int8_t val);
err_code_t falcon16_tsc_INTERNAL_get_rx_dfe11(srds_access_t *sa__, int8_t *val);
err_code_t falcon16_tsc_INTERNAL_set_rx_dfe12(srds_access_t *sa__, int8_t val);
err_code_t falcon16_tsc_INTERNAL_get_rx_dfe12(srds_access_t *sa__, int8_t *val);
err_code_t falcon16_tsc_INTERNAL_set_rx_dfe13(srds_access_t *sa__, int8_t val);
err_code_t falcon16_tsc_INTERNAL_get_rx_dfe13(srds_access_t *sa__, int8_t *val);
err_code_t falcon16_tsc_INTERNAL_set_rx_dfe14(srds_access_t *sa__, int8_t val);
err_code_t falcon16_tsc_INTERNAL_get_rx_dfe14(srds_access_t *sa__, int8_t *val);

/* Set/Get TX Tap values */
err_code_t falcon16_tsc_INTERNAL_load_txfir_taps(srds_access_t *sa__);
err_code_t falcon16_tsc_INTERNAL_set_tx_pre(srds_access_t *sa__, uint8_t val);
err_code_t falcon16_tsc_INTERNAL_get_tx_pre(srds_access_t *sa__, int8_t *val);
err_code_t falcon16_tsc_INTERNAL_set_tx_main(srds_access_t *sa__, uint8_t val);
err_code_t falcon16_tsc_INTERNAL_get_tx_main(srds_access_t *sa__, int8_t *val);
err_code_t falcon16_tsc_INTERNAL_set_tx_post1(srds_access_t *sa__, int8_t val);
err_code_t falcon16_tsc_INTERNAL_get_tx_post1(srds_access_t *sa__, int8_t *val);
err_code_t falcon16_tsc_INTERNAL_set_tx_post2(srds_access_t *sa__, int8_t val);
err_code_t falcon16_tsc_INTERNAL_get_tx_post2(srds_access_t *sa__, int8_t *val);
err_code_t falcon16_tsc_INTERNAL_set_tx_post3(srds_access_t *sa__, int8_t val);
err_code_t falcon16_tsc_INTERNAL_get_tx_post3(srds_access_t *sa__, int8_t *val);
err_code_t falcon16_tsc_INTERNAL_set_tx_rpara(srds_access_t *sa__, uint8_t val);
err_code_t falcon16_tsc_INTERNAL_get_tx_rpara(srds_access_t *sa__, int8_t *val);

/* Lane Config Struct */
err_code_t falcon16_tsc_INTERNAL_update_uc_lane_config_st(struct falcon16_tsc_uc_lane_config_st *st);
err_code_t falcon16_tsc_INTERNAL_update_uc_lane_config_word(struct falcon16_tsc_uc_lane_config_st *st);

/* Lane User Control Disable Startup Function Struct */
err_code_t falcon16_tsc_INTERNAL_update_usr_ctrl_disable_functions_st(struct falcon16_tsc_usr_ctrl_disable_functions_st *st);
err_code_t falcon16_tsc_INTERNAL_update_usr_ctrl_disable_functions_byte(struct falcon16_tsc_usr_ctrl_disable_functions_st *st);

/* Lane User Control Disable Startup DFE Function Struct */
err_code_t falcon16_tsc_INTERNAL_update_usr_ctrl_disable_dfe_functions_st(struct falcon16_tsc_usr_ctrl_disable_dfe_functions_st *st);
err_code_t falcon16_tsc_INTERNAL_update_usr_ctrl_disable_dfe_functions_byte(struct falcon16_tsc_usr_ctrl_disable_dfe_functions_st *st);

/* Core Config Struct */
err_code_t falcon16_tsc_INTERNAL_update_uc_core_config_st(struct  falcon16_tsc_uc_core_config_st *st);
err_code_t falcon16_tsc_INTERNAL_update_uc_core_config_word(struct  falcon16_tsc_uc_core_config_st *st);

uint8_t falcon16_tsc_INTERNAL_rdb_uc_var(srds_access_t *sa__, err_code_t *err_code_p, uint16_t addr);
uint16_t falcon16_tsc_INTERNAL_rdw_uc_var(srds_access_t *sa__, err_code_t *err_code_p, uint16_t addr);
err_code_t falcon16_tsc_INTERNAL_wrb_uc_var(srds_access_t *sa__, uint16_t addr, uint8_t wr_val);
err_code_t falcon16_tsc_INTERNAL_wrw_uc_var(srds_access_t *sa__, uint16_t addr, uint16_t wr_val);

/*--------------------------*/
/*  Register field polling  */
/*--------------------------*/

err_code_t falcon16_tsc_INTERNAL_print_uc_dsc_error(srds_access_t *sa__, enum srds_pmd_uc_cmd_enum cmd);

/** Polls lane variable "usr_diag_status" to verify data is available in uC diag buffer.
 * Define macro CUSTOM_REG_POLLING to replace the default implementation provided in falcon16_tsc_internal.c.
 *
 * @param sa__ is an opaque state vector passed through to device access functions.
 * @param *status returns a status byte \n
 *    bit 15 - indicates the ey scan is complete \n
 *    bit 14-0 - reserved for debug
 *
 * @param timeout_ms Maximum time interval in milliseconds for which the polling is done
 * @return Error code generated by polling function (returns ERR_CODE_NONE if no errors)
 */
err_code_t falcon16_tsc_INTERNAL_poll_diag_done(srds_access_t *sa__, uint16_t *status, uint32_t timeout_ms);

/** Polls for register field "uc_dsc_ready_for_cmd" to be 1 within the time interval specified by timeout_ms.
 * Function returns 0 if polling passes, else it returns error code.
 * Define macro CUSTOM_REG_POLLING to replace the default implementation provided in falcon16_tsc_internal.c.
 * @param sa__ is an opaque state vector passed through to device access functions.
 * @param timeout_ms Maximum time interval in milliseconds for which the polling is done
 * @param cmd The command that was issued; this is used for error reporting.
 * @return Error code generated by polling function (returns ERR_CODE_NONE if no errors)
 */
err_code_t falcon16_tsc_INTERNAL_poll_uc_dsc_ready_for_cmd_equals_1(srds_access_t *sa__, uint32_t timeout_ms, enum srds_pmd_uc_cmd_enum cmd);

/** Polls for register field "dsc_state" to be "DSC_STATE_UC_TUNE"
 * within the time interval specified by timeout_ms.
 * Function returns 0 if polling passes, else it returns error code.
 * Define macro CUSTOM_REG_POLLING to replace the default implementation provided in falcon16_tsc_internal.c.
 * @param sa__ is an opaque state vector passed through to device access functions.
 * @param timeout_ms Maximum time interval in milliseconds for which the polling is done
 * @return Error code generated by polling function (returns ERR_CODE_NONE if no errors)
 */
err_code_t falcon16_tsc_INTERNAL_poll_dsc_state_equals_uc_tune(srds_access_t *sa__, uint32_t timeout_ms);


/** Polls for register field "micro_ra_initdone" to be 1 within the time interval specified by timeout_ms.
 * Function returns 0 if polling passes, else it returns error code.
 * Define macro CUSTOM_REG_POLLING to replace the default implementation provided in falcon16_tsc_internal.c.
 * @param sa__ is an opaque state vector passed through to device access functions.
 * @param timeout_ms Maximum time interval in milliseconds for which the polling is done
 * @return Error code generated by polling function (returns ERR_CODE_NONE if no errors)
 */
err_code_t falcon16_tsc_INTERNAL_poll_micro_ra_initdone(srds_access_t *sa__, uint32_t timeout_ms);

/** Maps AFE slicer offset adjustment values.
 * @param x Signed 8bit number
 * @return Signed 8bit number
 */
int8_t falcon16_tsc_INTERNAL_afe_slicer_offset_mapping(int8_t x);

/*--------------------------------*/
/*  RAM Block Read with Callback  */
/*--------------------------------*/

/* Utilities to account for endianness when reading from uc RAM */
static const int falcon16_tsc_endian_const = 1;
#define is_big_endian() ((*(char*)&falcon16_tsc_endian_const) == 0)
#define get_endian_offset(addr) (is_big_endian() ? \
                                (\
                                (((uintptr_t)(addr))%4 == 0) ?  3 : \
                                (((uintptr_t)(addr))%4 == 1) ?  1 : \
                                (((uintptr_t)(addr))%4 == 2) ? -1 : \
                                (((uintptr_t)(addr))%4 == 3) ? -3 : \
                                0) \
                                : 0)

typedef struct {
    uint8_t *mem_ptr;
} falcon16_tsc_INTERNAL_rdblk_callback_arg_t;

/** Callback function for reading uc RAM memory by falcon16_tsc_INTERNAL_rdblk_uc_generic_ram().
 * @param *arg is a falcon16_tsc_INTERNAL_rdblk_callback_arg_t pointer for holding state.
 * @param byte_count is the number of bytes to store.
 *        -# This function is called repeatedly with a byte count of 2 with U16 values to store.
 *        -# This function may then be called with a byte count of 1 if there are an odd number of bytes.
 * @param data is the RAM data to store.
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors)
 */
err_code_t falcon16_tsc_INTERNAL_rdblk_callback(void *arg, uint8_t byte_count, uint16_t data);

/** Block RAM read through Micro Register Interface, suitable for program RAM or non-program RAM.
 *  This reads a block of RAM, possibly starting midway through it and wrapping back to the beginning.
 *  The order of operations is:
 *      -# RAM will be read from (block_addr+start_offset) to (block_addr+block_size-1),
 *         up to cnt bytes.
 *      -# RAM will be read from block_addr to (block_addr+block_size-1), repeating
 *         indefinitely, until cnt bytes total are read.
 * @param sa__ is an opaque state vector passed through to device access functions.
 * @param block_addr Address of RAM block to be read.
 *                   For program RAM, the upper 16 bits are 0x0000.  Otherwise, the upper 16 bits are 0x2000.
 * @param block_size Length of RAM block to read.
 * @param start_offset Offset within RAM block to start reading
 * @param cnt Number of bytes to be read
 * @param *arg is passed as the first argument to callback
 * @param *callback is called with all of the data read, two bytes at a time.
 *                  The last call of callback may have one byte; in that case, the upper byte is undefined.
 *                  The call is in the form:  callback(arg, byte_count, data)
 * @return Error Code generated by API or the callback (returns ERR_CODE_NONE if no errors)
 */
err_code_t falcon16_tsc_INTERNAL_rdblk_uc_generic_ram(srds_access_t *sa__,
                                                uint32_t block_addr,
                                                uint16_t block_size,
                                                uint16_t start_offset,
                                                uint16_t cnt,
                                                void *arg,
                                                err_code_t (*callback)(void *, uint8_t, uint16_t));

/** Block RAM read through Micro Register Interface, suitable for program RAM or non-program RAM.
 *  This reads a block of RAM, possibly starting midway through it and wrapping back to the beginning.
 *  The order of operations is:
 *      -# RAM will be read from (block_addr+start_offset) down to block_addr, up to cnt bytes.
 *      -# RAM will be read from (block_addr+block_size-1) down to block_addr, repeating
 *         indefinitely, until cnt bytes total are read.
 *  In other words, this is the same as falcon16_tsc_INTERNAL_rdblk_uc_generic_ram(), but reading in descending address order.
 * @param sa__ is an opaque state vector passed through to device access functions.
 * @param block_addr Address of RAM block to be read.
 *                   For program RAM, the upper 16 bits are 0x0000.  Otherwise, the upper 16 bits are 0x2000.
 * @param block_size Length of RAM block to read.
 * @param start_offset Offset within RAM block to start reading
 * @param cnt Number of bytes to be read
 * @param *arg is passed as the first argument to callback
 * @param *callback is called with all of the data read, two bytes at a time.
 *                  The last call of callback may have one byte; in that case, the upper byte is undefined.
 *                  The call is in the form:  callback(arg, byte_count, data)
 * @return Error Code generated by API or the callback (returns ERR_CODE_NONE if no errors)
 */
err_code_t falcon16_tsc_INTERNAL_rdblk_uc_generic_ram_descending(srds_access_t *sa__,
                                                           uint32_t block_addr,
                                                           uint16_t block_size,
                                                           uint16_t start_offset,
                                                           uint16_t cnt,
                                                           void *arg,
                                                           err_code_t (*callback)(void *, uint8_t, uint16_t));

/*-------------------*/
/*  Name resolution  */
/*-------------------*/

/* For bitfields that differ for different cores, these macros are defined to resolve the bitfield. */

#if defined(wr_rx_pf_ctrl)
#   define WR_RX_PF_CTRL  wr_rx_pf_ctrl
#   define RD_RX_PF_CTRL  rd_rx_pf_ctrl
#   define WR_RX_PF2_CTRL wr_rx_pf2_ctrl
#   define RD_RX_PF2_CTRL rd_rx_pf2_ctrl
#elif defined(wr_rx_pf_ctrl_val)
#   define WR_RX_PF_CTRL  wr_rx_pf_ctrl_val
#   define RD_RX_PF_CTRL  rd_rx_pf_ctrl_val
#   define WR_RX_PF2_CTRL wr_rx_pf2_ctrl_val
#   define RD_RX_PF2_CTRL rd_rx_pf2_ctrl_val
#elif defined(wr_pf_ctrl)
#   define WR_RX_PF_CTRL  wr_pf_ctrl
#   define RD_RX_PF_CTRL  rd_pf_ctrl
#   define WR_RX_PF2_CTRL wr_pf2_lowp_ctrl
#   define RD_RX_PF2_CTRL rd_pf2_lowp_ctrl
#else
#   error "Could not find write to pf_ctrl bitfield."
#endif

#if defined(reg_rd_TLB_RX_RXPRBS_CHK_ERR_CNT_MSB_STATUS)
#   define REG_RD_TLB_RX_PRBS_CHK_ERR_CNT_MSB_STATUS reg_rd_TLB_RX_RXPRBS_CHK_ERR_CNT_MSB_STATUS
#   define REG_RD_TLB_RX_PRBS_CHK_ERR_CNT_LSB_STATUS reg_rd_TLB_RX_RXPRBS_CHK_ERR_CNT_LSB_STATUS
#elif defined(reg_rd_TLB_RX_PRBS_CHK_ERR_CNT_MSB_STATUS)
#   define REG_RD_TLB_RX_PRBS_CHK_ERR_CNT_MSB_STATUS reg_rd_TLB_RX_PRBS_CHK_ERR_CNT_MSB_STATUS
#   define REG_RD_TLB_RX_PRBS_CHK_ERR_CNT_LSB_STATUS reg_rd_TLB_RX_PRBS_CHK_ERR_CNT_LSB_STATUS
#else
#   error "Could not find PRBS_CHK_ERR_CNT_MSB_STATUS."
#endif

#endif
