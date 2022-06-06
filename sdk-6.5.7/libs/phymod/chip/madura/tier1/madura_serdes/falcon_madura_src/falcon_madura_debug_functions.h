/*****************************************************************************************
 *****************************************************************************************
 *                                                                                       *
 *  Revision      :  $Id: falcon_furia_debug_functions.h 776 2014-11-11 18:28:55Z kirand $ *
 *                                                                                       *
 *  Description   :  Functions used internally and available in debug shell only         *
 *                                                                                       *
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$                                                                 *
 *  No portions of this material may be reproduced in any form without                   *
 *  the written permission of:                                                           *
 *      Broadcom Corporation                                                             *
 *      5300 California Avenue                                                           *
 *      Irvine, CA  92617                                                                *
 *                                                                                       *
 *  All information contained in this document is Broadcom Corporation                   *
 *  company private proprietary, and trade secret.                                       *
 *                                                                                       *
 *****************************************************************************************
 *****************************************************************************************/

/** @file falcon_furia_debug_functions.h
 * Functions used internally and available in debug shell only
 */

#ifndef FALCON_FURIA_DEBUG_FUNCTIONS_H
#define FALCON_FURIA_DEBUG_FUNCTIONS_H

#include "falcon_madura_usr_includes.h"
#include "falcon_madura_enum.h"
#include "falcon_madura_common.h"
#include "falcon_madura_api_uc_common.h"

/** Lane User Control Function Structure in Microcode */
struct falcon_furia_madura_usr_ctrl_func_st {
  /** Main Peaking Filter Adaptation */  
  uint16_t pf_adaptation           ;
  /** Low Frequency Peaking Filter adaptation */
  uint16_t pf2_adaptation          ;
  /** DC Offset Adaptation */ 
  uint16_t dc_adaptation           ;
  /** Variable Gain Amplifier adaptation */
  uint16_t vga_adaptation          ;
  /** Slicer Vertical Offset tuning */
  uint16_t slicer_voffset_tuning   ;
  /** Slicer Horizontal Offset tuning */
  uint16_t slicer_hoffset_tuning   ;
  /** Phase Offset adaptation */
  uint16_t phase_offset_adaptation ;
  /** Eye adaptation */
  uint16_t eye_adaptation          ;
  /** All adaptation */
  uint16_t all_adaptation          ;
  uint16_t reserved                ;
};

/** Lane User DFE Control Function Structure in Microcode */
struct falcon_furia_madura_usr_ctrl_dfe_func_st {
  /** DFE tap 1 adaptation */  
  uint8_t dfe_tap1_adaptation      ;
  /** DFE Fixed taps adaptation */
  uint8_t dfe_fx_taps_adaptation   ;
  /** DFE Floating taps adaptation */
  uint8_t dfe_fl_taps_adaptation   ;
  /** DFE tap Duty Cycle Distortion */ 
  uint8_t dfe_dcd_adaptation       ;
};

/** Lane User Control Disable Function Struct */
struct falcon_furia_madura_usr_ctrl_disable_functions_st {
  struct falcon_furia_madura_usr_ctrl_func_st field;
  uint16_t word;
};

/** Lane User Control Disable DFE Function Struct */
struct falcon_furia_madura_usr_ctrl_disable_dfe_functions_st {
  struct falcon_furia_madura_usr_ctrl_dfe_func_st field;
  uint8_t  byte;                        
};

/** Falcon Lane status Struct */
struct falcon_furia_madura_detailed_lane_status_st {
    /** Frequency offset of local reference clock with respect to RX data in ppm */
    USR_DOUBLE ppm;
    /** Estimate for Main tap */ 
    USR_DOUBLE main_tap_est;
    /** PRBS Error Count */
    uint32_t prbs_chk_errcnt;
    /** One Hot DSC State */ 
    uint16_t dsc_sm[2];
    /** Horizontal left eye margin @ 1e-5 as seen by internal diagnostic slicer in mUI and mV */
    uint16_t heye_left;
    /** Horizontal right eye margin @ 1e-5 as seen by internal diagnostic slicer in mUI and mV */
    uint16_t heye_right;
    /** Vertical upper eye margin @ 1e-5 as seen by internal diagnostic slicer in mUI and mV */
    uint16_t veye_upper;
    /** Vertical lower eye margin @ 1e-5 as seen by internal diagnostic slicer in mUI and mV */
    uint16_t veye_lower;
    /** Link time in milliseconds */
    uint16_t link_time;
    /** Temperature Index */
    uint8_t  temp_idx;
    /** TX driver to disable high voltage control */
    uint8_t  ams_tx_drv_hv_disable;
    /** Analog Resistor Calibration value */
    uint8_t  ams_tx_ana_rescal;
    /* Analog Amplitude control */
    uint8_t  amp_ctrl;
    /** TX equalization FIR pre tap weight */
    uint8_t  pre_tap;
    /** TX equalization FIR main tap weight */
    uint8_t  main_tap;
    /** TX equalization FIR post1 tap weight */
    uint8_t  post1_tap;
    /** TX equalization FIR post2 tap weight */
    int8_t   post2_tap;
    /** TX equalization FIR post3 tap weight */
    int8_t   post3_tap;
    /** Signal Detect */
    uint8_t  sigdet;
    /** PMD RX Lock */
    uint8_t  pmd_lock;
    /** Variable Gain Amplifier settings */
    uint8_t  vga;
    /** Main Peaking Filter setting */
    uint8_t  pf;
    /** Low Frequency Peaking Filter settings */ 
    uint8_t  pf2;
    /** Data Slicer Threshold */
    int8_t   data_thresh;
    /** Phase Slicer Threshold */
    int8_t   phase_thresh;
    /** LMS Slicer Threshold */
    int8_t   lms_thresh;
    /** Horizontal offset between data and dataq */ 
    uint8_t  ddq_hoffset;
    /** Horizontal offset between phase and phaseq */ 
    uint8_t  ppq_hoffset;
    /** Horizontal offset between lms and lmsq */ 
    uint8_t  llq_hoffset;
    /** Horizontal offset between data and phase */ 
    uint8_t  dp_hoffset;
    /** Horizontal offset between data and lms */ 
    uint8_t  dl_hoffset;
    /** DFE tap values */
    int8_t   dfe[26][4];
    /** DC offset DAC control value */
    int8_t   dc_offset;
    /** Slicer calibration control codes (datap) */
    int8_t   thctrl_dp[4];
    /** Slicer calibration control codes (datan) */
    int8_t   thctrl_dn[4];
    /** Slicer calibration control codes (phasep) */
    int8_t   thctrl_zp[4];
    /** Slicer calibration control codes (phasen) */
    int8_t   thctrl_zn[4];
    /** Slicer calibration control codes (lms) */
    int8_t   thctrl_l[4];
    /** PRBS Checker Enable */
    uint8_t  prbs_chk_en;
    /** Order of PRBS Checker */
    uint8_t  prbs_chk_order;
    /** PRBS Checker Lock */
    uint8_t  prbs_chk_lock;
};


/** Isolate Control pins.
 * Can be used for debug to avoid any interference from inputs coming through pins.
 * @param enable Isolate pins enable (1 = Isolate pins; 0 = Pins not isolated)
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors) 
 */
err_code_t falcon_furia_madura_isolate_ctrl_pins(const phymod_access_t *pa, uint8_t enable);


/*-----------------------*/
/*  Stop/Resume uC Lane  */
/*-----------------------*/
/** Stop/Resume Micro operations on a Lane (Graceful Stop).
 * @param enable Enable micro lane stop (1 = Stop Micro opetarions on lane; 0 = Resume Micro operations on lane) 
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors) 
 */
err_code_t falcon_furia_madura_stop_uc_lane(const phymod_access_t *pa, uint8_t enable);

/** Status of whether Micro is stopped on a lane.
 * @param *uc_lane_stopped Micro lane stopped status returned by API
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors) 
 */
err_code_t falcon_furia_madura_stop_uc_lane_status(const phymod_access_t *pa, uint8_t *uc_lane_stopped);

/** Write to lane user control disable startup function uC RAM variable.
 *  Note: This function should be used only during configuration under dp_reset.
 * @param set_val Value to be written into lane user control disable startup function RAM variable
 * @return Error Code, if generated (returns ERR_CODE_NONE if no errors) 
 */ 
err_code_t falcon_furia_madura_set_usr_ctrl_disable_startup(const phymod_access_t *pa, struct falcon_furia_madura_usr_ctrl_disable_functions_st set_val);

/** Write to lane user control disable startup dfe function uC RAM variable.
 *  Note: This function should be used only during configuration under dp_reset.
 * @param set_val Value to be written into lane user control disable startup dfe function RAM variable
 * @return Error Code, if generated (returns ERR_CODE_NONE if no errors) 
 */ 
err_code_t falcon_furia_madura_set_usr_ctrl_disable_startup_dfe(const phymod_access_t *pa, struct falcon_furia_madura_usr_ctrl_disable_dfe_functions_st set_val);

/** Write to lane user control disable steady-state function uC RAM variable.
 *  Note: This function should be used only during configuration under dp_reset.
 * @param set_val Value to be written into lane user control disable  steady-state function RAM variable
 * @return Error Code, if generated (returns ERR_CODE_NONE if no errors) 
 */ 
err_code_t falcon_furia_madura_set_usr_ctrl_disable_steady_state(const phymod_access_t *pa, struct falcon_furia_madura_usr_ctrl_disable_functions_st set_val);

/** Write to lane user control disable steady-state dfe function uC RAM variable.
 *  Note: This function should be used only during configuration under dp_reset.
 * @param set_val Value to be written into lane user control disable  steady-state dfe function RAM variable
 * @return Error Code, if generated (returns ERR_CODE_NONE if no errors) 
 */ 
err_code_t falcon_furia_madura_set_usr_ctrl_disable_steady_state_dfe(const phymod_access_t *pa, struct falcon_furia_madura_usr_ctrl_disable_dfe_functions_st set_val);

/** Read value of lane user control disable startup uC RAM variable.
 * @param *get_val Value read from lane user control disable startup RAM variable
 * @return Error Code, if generated (returns ERR_CODE_NONE if no errors) 
 */ 
err_code_t falcon_furia_madura_get_usr_ctrl_disable_startup(const phymod_access_t *pa, struct falcon_furia_madura_usr_ctrl_disable_functions_st *get_val);

/** Read value of lane user control disable startup dfe uC RAM variable.
 * @param *get_val Value read from lane user control disable startup dfe RAM variable
 * @return Error Code, if generated (returns ERR_CODE_NONE if no errors) 
 */
err_code_t falcon_furia_madura_get_usr_ctrl_disable_startup_dfe(const phymod_access_t *pa, struct falcon_furia_madura_usr_ctrl_disable_dfe_functions_st *get_val);

/** Read value of lane user control disable steady-state uC RAM variable.
 * @param *get_val Value read from lane user control disable steady-state RAM variable
 * @return Error Code, if generated (returns ERR_CODE_NONE if no errors) 
 */
err_code_t falcon_furia_madura_get_usr_ctrl_disable_steady_state(const phymod_access_t *pa, struct falcon_furia_madura_usr_ctrl_disable_functions_st *get_val);

/** Read value of lane user control disable steady-state dfe uC RAM variable.
 * @param *get_val Value read from lane user control disable steady-state dfe RAM variable
 * @return Error Code, if generated (returns ERR_CODE_NONE if no errors) 
 */
err_code_t falcon_furia_madura_get_usr_ctrl_disable_steady_state_dfe(const phymod_access_t *pa, struct falcon_furia_madura_usr_ctrl_disable_dfe_functions_st *get_val);


/*-------------------------------------------*/
/*  Registers and Core uC RAM Variable Dump  */
/*-------------------------------------------*/
/** Display values of both Core level and (currently selected) Lane level Registers.
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors) 
 */
err_code_t falcon_furia_madura_reg_dump(const phymod_access_t *pa );

/** Display values of all Core uC RAM Variables (Core RAM Variable Dump).
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors) 
 */
err_code_t falcon_furia_madura_uc_core_var_dump(const phymod_access_t *pa );


/*-----------------------------*/
/*  uC RAM Lane Variable Dump  */
/*-----------------------------*/
/** Display values of all Lane uC RAM Variables (Lane RAM Variable Dump).
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors) 
 */
err_code_t falcon_furia_madura_uc_lane_var_dump(const phymod_access_t *pa );


/*--------------------------*/
/*  TX_PI Jitter Generation */
/*--------------------------*/

/** Generate TX_PI Sinusoidal or Spread-Spectrum (SSC) jitter.
 * @param enable Enable/Disable jitter generation (1 = Enable; 0 = Disable)
 * @param freq_override_val Fixed Frequency Override value (-8192 to + 8192)
 * @param jit_type Jitter generation mode
 * @param tx_pi_jit_freq_idx Jitter generation frequency index (0 to 63) [see spec for more details]
 * @param tx_pi_jit_amp Jitter generation amplification factor (0 to 63) [max value of this register depends on tx_pi_jit_freq_idx and freq_override values]
 * @return Error Code generated by invalid TX_PI settings (returns ERR_CODE_NONE if no errors)
 */
err_code_t falcon_furia_madura_tx_pi_jitt_gen(const phymod_access_t *pa, uint8_t enable, int16_t freq_override_val, enum srds_tx_pi_freq_jit_gen_enum jit_type, uint8_t tx_pi_jit_freq_idx, uint8_t tx_pi_jit_amp);


/*-------------*/
/*  Event Log  */
/*-------------*/
/** Get Event Log from uC.
 * Dump uC events from core memory.
 * @param *trace_mem Pre-allocated buffer to store trace memory dump.
 * @param display_mode Enable event logs decoding. \n 
 *                      EVENT_LOG_ENCODED(0) - display log in hex octets, \n
 *                      EVENT_LOG_DECODED(1) - display log in plain text, \n
 *                      EVENT_LOG_ENCODED_AND_DECODED(2) - display log in both hex octects and plain text
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors)
 */ 
err_code_t falcon_furia_madura_read_event_log(const phymod_access_t *pa, uint8_t *trace_mem,enum srds_event_log_display_mode_enum display_mode);

/** Stop Serdes uC Event Logging.
 * Stop writing uC events to core memory.
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors)
 */ 
err_code_t falcon_furia_madura_event_log_stop(const phymod_access_t *pa );

/** Read Serdes uC Event Logging.
 * Dump uC events from core memory.
 * @param *trace_mem Pre-allocated buffer to store trace memory dump.
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors)
 */ 
err_code_t falcon_furia_madura_event_log_readmem(const phymod_access_t *pa, uint8_t *trace_mem);

/** Display Serdes uC Event Logging.
 * Display uC events from buffer.
 * @param *trace_mem Pre-allocated buffer to store trace memory dump.
 * @param display_mode Enable event logs decoding. \n
 *                      EVENT_LOG_ENCODED(0) - display log in hex octets, \n 
 *                      EVENT_LOG_DECODED(1) - display log in plain text, \n
 *                      EVENT_LOG_ENCODED_AND_DECODED(2) - display log in both hex octects and plain text
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors)
 */ 
err_code_t falcon_furia_madura_event_log_display(const phymod_access_t *pa, uint8_t *trace_mem,enum srds_event_log_display_mode_enum display_mode);

/** Write to usr_ctrl_lane_event_log_level uC RAM variable.
 *  Note: This function should be used only during configuration under dp_reset.
 * @param lane_event_log_level Value to be written into usr_ctrl_lane_event_log_level RAM variable
 * @return Error Code, if generated (returns ERR_CODE_NONE if no errors) 
 */
err_code_t falcon_furia_madura_set_usr_ctrl_lane_event_log_level(const phymod_access_t *pa, uint8_t lane_event_log_level);

/** Read value of usr_ctrl_lane_event_log_level uC RAM variable.
 * @param *lane_event_log_level Value read from usr_ctrl_lane_event_log_level RAM variable
 * @return Error Code, if generated (returns ERR_CODE_NONE if no errors) 
 */
err_code_t falcon_furia_madura_get_usr_ctrl_lane_event_log_level(const phymod_access_t *pa, uint8_t *lane_event_log_level);

/** Write to usr_ctrl_core_event_log_level uC RAM variable. 
 * @param core_event_log_level Value to be written into the usr_ctrl_core_event_log_level RAM variable
 * @return Error Code, if generated (returns ERR_CODE_NONE if no errors) 
 */
err_code_t falcon_furia_madura_set_usr_ctrl_core_event_log_level(const phymod_access_t *pa, uint8_t core_event_log_level);

/** Read value of usr_ctrl_core_event_log_level uC RAM variable.
 * @param *core_event_log_level Value read from usr_ctrl_core_event_log_level RAM variable
 * @return Error Code, if generated (returns ERR_CODE_NONE if no errors) 
 */
err_code_t falcon_furia_madura_get_usr_ctrl_core_event_log_level(const phymod_access_t *pa, uint8_t *core_event_log_level);


/*---------------------------------------------*/
/*  Serdes IP RAM access - Lane RAM Variables  */
/*---------------------------------------------*/
/*          rd - read; wr - write              */ 
/*          b  - byte; w  - word               */
/*          l  - lane; c  - core               */
/*          s  - signed                        */
/*---------------------------------------------*/
/** Unsigned Byte Read of a uC RAM Lane variable.
 * Read access through Micro Register Interface for PMD IPs; through Serdes FW DSC Command Interface for external micro IPs.
 * @param *err_code Error Code generated by API (returns ERR_CODE_NONE if no errors)
 * @param addr Address of RAM lane variable to be read
 * @return 8bit unsigned value read from uC RAM
 */ 
uint8_t falcon_furia_madura_rdbl_uc_var(const phymod_access_t *pa, err_code_t *err_code, uint16_t addr);

/** Unsigned Word Read of a uC RAM Lane variable.
 * Read access through Micro Register Interface for PMD IPs; through Serdes FW DSC Command Interface for external micro IPs.
 * @param *err_code Error Code generated by API (returns ERR_CODE_NONE if no errors)
 * @param addr Address of RAM lane variable to be read
 * @return 16bit unsigned value read from uC RAM
 */ 
uint16_t falcon_furia_madura_rdwl_uc_var(const phymod_access_t *pa, err_code_t *err_code, uint16_t addr);

/** Unsigned Byte Write of a uC RAM Lane variable.
 * Write access through Micro Register Interface for PMD IPs; through Serdes FW DSC Command Interface for external micro IPs.
 * @param addr Address of RAM lane variable to be written
 * @param wr_val 8bit unsigned value to be written to RAM variable
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors)
 */ 
err_code_t falcon_furia_madura_wrbl_uc_var(const phymod_access_t *pa, uint16_t addr, uint8_t wr_val);

/** Unsigned Word Write of a uC RAM Lane variable.
 * Write access through Micro Register Interface for PMD IPs; through Serdes FW DSC Command Interface for external micro IPs.
 * @param addr Address of RAM lane variable to be written
 * @param wr_val 16bit unsigned value to be written to RAM variable
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors)
 */ 
err_code_t falcon_furia_madura_wrwl_uc_var(const phymod_access_t *pa, uint16_t addr, uint16_t wr_val);


/* Signed version of above 4 functions */

/** Signed Byte Read of a uC RAM Lane variable.
 * Read access through Micro Register Interface for PMD IPs; through Serdes FW DSC Command Interface for external micro IPs.
 * @param *err_code Error Code generated by API (returns ERR_CODE_NONE if no errors)
 * @param addr Address of RAM lane variable to be read
 * @return 8bit signed value read from uC RAM
 */ 
int8_t falcon_furia_madura_rdbls_uc_var(const phymod_access_t *pa, err_code_t *err_code, uint16_t addr);

/** Signed Word Read of a uC RAM Lane variable.
 * Read access through Micro Register Interface for PMD IPs; through Serdes FW DSC Command Interface for external micro IPs.
 * @param *err_code Error Code generated by API (returns ERR_CODE_NONE if no errors)
 * @param addr Address of RAM lane variable to be read
 * @return 16bit signed value read from uC RAM
 */ 
int16_t falcon_furia_madura_rdwls_uc_var(const phymod_access_t *pa, err_code_t *err_code, uint16_t addr);

/** Signed Byte Write of a uC RAM Lane variable.
 * Write access through Micro Register Interface for PMD IPs; through Serdes FW DSC Command Interface for external micro IPs.
 * @param addr Address of RAM lane variable to be written
 * @param wr_val 8bit signed value to be written to RAM variable
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors)
 */ 
err_code_t falcon_furia_madura_wrbls_uc_var(const phymod_access_t *pa, uint16_t addr, int8_t wr_val);

/** Signed Word Write of a uC RAM Lane variable.
 * Write access through Micro Register Interface for PMD IPs; through Serdes FW DSC Command Interface for external micro IPs.
 * @param addr Address of RAM lane variable to be written
 * @param wr_val 16bit signed value to be written to RAM variable
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors)
 */ 
err_code_t falcon_furia_madura_wrwls_uc_var(const phymod_access_t *pa, uint16_t addr, int16_t wr_val);


/*---------------------------------------------*/
/*  Serdes IP RAM access - Core RAM Variables  */
/*---------------------------------------------*/
/** Unsigned Byte Read of a uC RAM Core variable.
 * Read access through Micro Register Interface for PMD IPs; through Serdes FW DSC Command Interface for external micro IPs.
 * @param *err_code Error Code generated by API (returns ERR_CODE_NONE if no errors)
 * @param addr Address of RAM core variable to be read
 * @return 8bit unsigned value read from uC RAM
 */ 
uint8_t falcon_furia_madura_rdbc_uc_var(const phymod_access_t *pa, err_code_t *err_code, uint8_t addr);

/** Unsigned Word Read of a uC RAM Core variable.
 * Read access through Micro Register Interface for PMD IPs; through Serdes FW DSC Command Interface for external micro IPs.
 * @param *err_code Error Code generated by API (returns ERR_CODE_NONE if no errors)
 * @param addr Address of RAM core variable to be read
 * @return 16bit unsigned value read from uC RAM
 */ 
uint16_t falcon_furia_madura_rdwc_uc_var(const phymod_access_t *pa, err_code_t *err_code, uint8_t addr);

/** Unsigned Byte Write of a uC RAM Core variable.
 * Write access through Micro Register Interface for PMD IPs; through Serdes FW DSC Command Interface for external micro IPs.
 * @param addr Address of RAM core variable to be written
 * @param wr_val 8bit unsigned value to be written to RAM variable
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors)
 */ 
err_code_t falcon_furia_madura_wrbc_uc_var(const phymod_access_t *pa, uint8_t addr, uint8_t wr_val);

/** Unsigned Word Write of a uC RAM Core variable.
 * Write access through Micro Register Interface for PMD IPs; through Serdes FW DSC Command Interface for external micro IPs.
 * @param addr Address of RAM core variable to be written
 * @param wr_val 16bit unsigned value to be written to RAM variable
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors)
 */ 
err_code_t falcon_furia_madura_wrwc_uc_var(const phymod_access_t *pa, uint8_t addr, uint16_t wr_val);


/* Signed version of above 4 functions */

/** Signed Byte Read of a uC RAM Core variable.
 * Read access through Micro Register Interface for PMD IPs; through Serdes FW DSC Command Interface for external micro IPs.
 * @param *err_code Error Code generated by API (returns ERR_CODE_NONE if no errors)
 * @param addr Address of RAM core variable to be read
 * @return 8bit signed value read from uC RAM
 */ 
int8_t falcon_furia_madura_rdbcs_uc_var(const phymod_access_t *pa, err_code_t *err_code, uint8_t addr);

/** Signed Word Read of a uC RAM Core variable.
 * Read access through Micro Register Interface for PMD IPs; through Serdes FW DSC Command Interface for external micro IPs.
 * @param *err_code Error Code generated by API (returns ERR_CODE_NONE if no errors)
 * @param addr Address of RAM core variable to be read
 * @return 16bit signed value read from uC RAM
 */ 
int16_t falcon_furia_madura_rdwcs_uc_var(const phymod_access_t *pa, err_code_t *err_code, uint8_t addr);

/** Signed Byte Write of a uC RAM Core variable.
 * Write access through Micro Register Interface for PMD IPs; through Serdes FW DSC Command Interface for external micro IPs.
 * @param addr Address of RAM core variable to be written
 * @param wr_val 8bit signed value to be written to RAM variable
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors)
 */ 
err_code_t falcon_furia_madura_wrbcs_uc_var(const phymod_access_t *pa, uint8_t addr, int8_t wr_val);

/** Signed Word Write of a uC RAM Core variable.
 * Write access through Micro Register Interface for PMD IPs; through Serdes FW DSC Command Interface for external micro IPs.
 * @param addr Address of RAM core variable to be written
 * @param wr_val 16bit signed value to be written to RAM variable
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors)
 */ 
err_code_t falcon_furia_madura_wrwcs_uc_var(const phymod_access_t *pa, uint8_t addr, int16_t wr_val);


/*---------------------------------------------------*/
/*  Micro Commands through uC DSC Command Interface  */
/*---------------------------------------------------*/
/** Issue a Micro command through the uC DSC Command Interface and return control immediately.
 * @param cmd Micro command to be issued
 * @param supp_info Supplement information for the Micro command to be issued (RAM read/write address or Micro Control command)
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors) 
 */
err_code_t falcon_furia_madura_pmd_uc_cmd_return_immediate(const phymod_access_t *pa, enum srds_pmd_uc_cmd_enum cmd, uint8_t supp_info);

/** Issue a Micro command through the uC DSC Command Interface. Control returned only after command is finished.
 * @param cmd Micro command to be issued
 * @param supp_info Supplement information for the Micro command to be issued (RAM read/write address or Micro Control command)
 * @param timeout_ms Time interval in milliseconds inside which the command should be completed; else error issued
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors) 
 */
err_code_t falcon_furia_madura_pmd_uc_cmd(const phymod_access_t *pa, enum srds_pmd_uc_cmd_enum cmd, uint8_t supp_info, uint32_t timeout_ms);                         

/** Issue a Micro command with data through the uC DSC Command Interface and return control immediately.
 * @param cmd Micro command to be issued
 * @param supp_info Supplement information for the Micro command to be issued (RAM write address)
 * @param data Data to be written to dsc_data for use by uC
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors) 
 */
err_code_t falcon_furia_madura_pmd_uc_cmd_with_data_return_immediate(const phymod_access_t *pa, enum srds_pmd_uc_cmd_enum cmd, uint8_t supp_info, uint16_t data);

/** Issue a Micro command with data through the uC DSC Command Interface. Control returned only after command is finished.
 * @param cmd Micro command to be issued
 * @param supp_info Supplement information for the Micro command to be issued (RAM write address)
 * @param data Data to be written to dsc_data for use by uC
 * @param timeout_ms Time interval in milliseconds inside which the command should be completed; else error issued
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors) 
 */
err_code_t falcon_furia_madura_pmd_uc_cmd_with_data(const phymod_access_t *pa, enum srds_pmd_uc_cmd_enum cmd, uint8_t supp_info, uint16_t data, uint32_t timeout_ms); 

/** Issue a Micro Control command through the uC DSC Command Interface and return control immediately.
 * @param control Micro Control command to be issued
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors) 
 */
err_code_t falcon_furia_madura_pmd_uc_control_return_immediate(const phymod_access_t *pa, enum srds_pmd_uc_ctrl_cmd_enum control);

/** Issue a Micro Control command through the uC DSC Command Interface. Control returned only after command is finished.
 * @param control Micro Control command to be issued
 * @param timeout_ms Time interval in milliseconds inside which the command should be completed; else error issued
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors) 
 */
err_code_t falcon_furia_madura_pmd_uc_control(const phymod_access_t *pa, enum srds_pmd_uc_ctrl_cmd_enum control, uint32_t timeout_ms);                              

/** Issue a Micro Control command through the uC DSC DIAG_EN Command Interface. Control returned only after command is finished.
 * @param control Micro DIAG Control command to be issued
 * @param timeout_ms Time interval in milliseconds inside which the command should be completed; else error issued
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors) 
 */
err_code_t falcon_furia_madura_pmd_uc_diag_cmd(const phymod_access_t *pa, enum srds_pmd_uc_diag_cmd_enum control, uint32_t timeout_ms);

/** Writes Serdes TXFIR tap settings.
 * Returns failcodes if TXFIR settings are invalid
 * @param pre   TXFIR pre tap value (0..31)
 * @param main  TXFIR main tap value (0..112)
 * @param post1 TXFIR post tap value (0..63)
 * @param post2 TXFIR post2 tap value (-15..15)
 * @param post3 TXFIR post3 tap value (-15..15)
 * @return Error Code generated by invalid tap settings (returns ERR_CODE_NONE if no errors) 
 */
err_code_t falcon_furia_madura_apply_txfir_cfg(const phymod_access_t *pa, int8_t pre, int8_t main, int8_t post1, int8_t post2, int8_t post3);

/** Reads current pmd lane status and populates the provided structure of type #falcon_furia_madura_detailed_lane_status_st.
 * @param *lane_st All detailed lane info read and populated into this structure
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors) 
 */
err_code_t falcon_furia_madura_log_full_pmd_state (const phymod_access_t *pa, struct falcon_furia_madura_detailed_lane_status_st *lane_st);

/** Displays the lane status stored in the input struct of type #falcon_furia_madura_detailed_lane_status_st.
 * @param *lane_st  Lane struct to be diplayed
 * @param num_lanes Number of lanes
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors) 
 */
err_code_t falcon_furia_madura_disp_full_pmd_state (const phymod_access_t *pa, struct falcon_furia_madura_detailed_lane_status_st *lane_st, uint8_t num_lanes);



/** Unsigned Block RAM read through Micro Register Interface.
 * @param *mem Pointer to memory location where the read values from the Block RAM read should be stored
 * @param addr Starting Address of RAM block to be read
 * @param cnt Number of byte locations to be read from the block
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors)
 */ 
err_code_t falcon_furia_madura_rdblk_uc_ram(const phymod_access_t *pa, uint8_t *mem, uint16_t addr, uint16_t cnt);

#endif
