/***********************************************************************************
 *                                                                                 *
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

/***********************************************************************************
 ***********************************************************************************
 *  File Name     :  blackhawk7_l8p2_internal_error.c                                   *
 *  Created On    :  12/07/2015                                                    *
 *  Created By    :  Brent Roberts                                                 *
 *  Description   :  API functions for internal errors in Serdes IPs               *
 *  Revision      :                                                            *
 */

/** @file blackhawk7_l8p2_internal_error.c
 * Implementation of API internal error functions
 */

#include "blackhawk7_l8p2_internal_error.h"
#include "blackhawk7_l8p2_dependencies.h"
#include "blackhawk7_l8p2_functions.h"
#include "blackhawk7_l8p2_internal.h"
#include "blackhawk7_l8p2_debug_functions.h"

const char* blackhawk7_l8p2_INTERNAL_e2s_err_code(err_code_t err_code)
{
#ifdef SMALL_FOOTPRINT
    return "SMALL_FOOTPRINT";
#else
    switch(err_code) {
    case ERR_CODE_NONE: return "ERR_CODE_NONE";
    case ERR_CODE_INVALID_RAM_ADDR: return "ERR_CODE_INVALID_RAM_ADDR";
    case ERR_CODE_SERDES_DELAY: return "ERR_CODE_SERDES_DELAY";
    case ERR_CODE_POLLING_TIMEOUT: return "ERR_CODE_POLLING_TIMEOUT";
    case ERR_CODE_CFG_PATT_INVALID_PATTERN: return "ERR_CODE_CFG_PATT_INVALID_PATTERN";
    case ERR_CODE_CFG_PATT_INVALID_PATT_LENGTH: return "ERR_CODE_CFG_PATT_INVALID_PATT_LENGTH";
    case ERR_CODE_CFG_PATT_LEN_MISMATCH: return "ERR_CODE_CFG_PATT_LEN_MISMATCH";
    case ERR_CODE_CFG_PATT_PATTERN_BIGGER_THAN_MAXLEN: return "ERR_CODE_CFG_PATT_PATTERN_BIGGER_THAN_MAXLEN";
    case ERR_CODE_CFG_PATT_INVALID_HEX: return "ERR_CODE_CFG_PATT_INVALID_HEX";
    case ERR_CODE_CFG_PATT_INVALID_BIN2HEX: return "ERR_CODE_CFG_PATT_INVALID_BIN2HEX";
    case ERR_CODE_CFG_PATT_INVALID_SEQ_WRITE: return "ERR_CODE_CFG_PATT_INVALID_SEQ_WRITE";
    case ERR_CODE_PATT_GEN_INVALID_MODE_SEL: return "ERR_CODE_PATT_GEN_INVALID_MODE_SEL";
    case ERR_CODE_INVALID_UCODE_LEN: return "ERR_CODE_INVALID_UCODE_LEN";
    case ERR_CODE_MICRO_INIT_NOT_DONE: return "ERR_CODE_MICRO_INIT_NOT_DONE";
    case ERR_CODE_UCODE_LOAD_FAIL: return "ERR_CODE_UCODE_LOAD_FAIL";
    case ERR_CODE_UCODE_VERIFY_FAIL: return "ERR_CODE_UCODE_VERIFY_FAIL";
    case ERR_CODE_INVALID_TEMP_IDX: return "ERR_CODE_INVALID_TEMP_IDX";
    case ERR_CODE_INVALID_PLL_CFG: return "ERR_CODE_INVALID_PLL_CFG";
    case ERR_CODE_TX_HPF_INVALID: return "ERR_CODE_TX_HPF_INVALID";
    case ERR_CODE_VGA_INVALID: return "ERR_CODE_VGA_INVALID";
    case ERR_CODE_PF_INVALID: return "ERR_CODE_PF_INVALID";
    case ERR_CODE_TX_AMP_CTRL_INVALID: return "ERR_CODE_TX_AMP_CTRL_INVALID";
    case ERR_CODE_INVALID_EVENT_LOG_WRITE: return "ERR_CODE_INVALID_EVENT_LOG_WRITE";
    case ERR_CODE_INVALID_EVENT_LOG_READ: return "ERR_CODE_INVALID_EVENT_LOG_READ";
    case ERR_CODE_UC_CMD_RETURN_ERROR: return "ERR_CODE_UC_CMD_RETURN_ERROR";
    case ERR_CODE_DATA_NOTAVAIL: return "ERR_CODE_DATA_NOTAVAIL";
    case ERR_CODE_BAD_PTR_OR_INVALID_INPUT: return "ERR_CODE_BAD_PTR_OR_INVALID_INPUT";
    case ERR_CODE_UC_NOT_STOPPED: return "ERR_CODE_UC_NOT_STOPPED";
    case ERR_CODE_UC_CRC_NOT_MATCH: return "ERR_CODE_UC_CRC_NOT_MATCH";
    case ERR_CODE_CORE_DP_NOT_RESET: return "ERR_CODE_CORE_DP_NOT_RESET";
    case ERR_CODE_LANE_DP_NOT_RESET: return "ERR_CODE_LANE_DP_NOT_RESET";
    case ERR_CODE_EXCEPTION: return "ERR_CODE_EXCEPTION";
    case ERR_CODE_INFO_TABLE_ERROR: return "ERR_CODE_INFO_TABLE_ERROR";
    case ERR_CODE_REFCLK_FREQUENCY_INVALID: return "ERR_CODE_REFCLK_FREQUENCY_INVALID";
    case ERR_CODE_PLL_DIV_INVALID: return "ERR_CODE_PLL_DIV_INVALID";
    case ERR_CODE_VCO_FREQUENCY_INVALID: return "ERR_CODE_VCO_FREQUENCY_INVALID";
    case ERR_CODE_INSUFFICIENT_PARAMETERS: return "ERR_CODE_INSUFFICIENT_PARAMETERS";
    case ERR_CODE_CONFLICTING_PARAMETERS: return "ERR_CODE_CONFLICTING_PARAMETERS";
    case ERR_CODE_BAD_LANE_COUNT: return "ERR_CODE_BAD_LANE_COUNT";
    case ERR_CODE_BAD_LANE: return "ERR_CODE_BAD_LANE";
    case ERR_CODE_UC_NOT_RESET: return "ERR_CODE_UC_NOT_RESET";
    case ERR_CODE_FFE_TAP_INVALID: return "ERR_CODE_FFE_TAP_INVALID";
    case ERR_CODE_FFE_NOT_AVAILABLE: return "ERR_CODE_FFE_NOT_AVAILABLE";
    case ERR_CODE_INVALID_RX_PAM_MODE: return "ERR_CODE_INVALID_RX_PAM_MODE";
    case ERR_CODE_INVALID_PRBS_ERR_ANALYZER_FEC_SIZE: return "ERR_CODE_INVALID_PRBS_ERR_ANALYZER_FEC_SIZE";
    case ERR_CODE_INVALID_PRBS_ERR_ANALYZER_ERR_THRESH: return "ERR_CODE_INVALID_PRBS_ERR_ANALYZER_ERR_THRESH";
    case ERR_CODE_INVALID_PRBS_ERR_ANALYZER_NO_PLL_LOCK: return "ERR_CODE_INVALID_PRBS_ERR_ANALYZER_NO_PLL_LOCK";
    case ERR_CODE_CFG_PATT_INVALID_PAM4: return "ERR_CODE_CFG_PATT_INVALID_PAM4";
    case ERR_CODE_INVALID_TDT_PATTERN_FOR_HW_MODE: return "ERR_CODE_INVALID_TDT_PATTERN_FOR_HW_MODE";
    case ERR_CODE_ODD_PRE_OR_POST_TAP_INPUT: return "ERR_CODE_ODD_PRE_OR_POST_TAP_INPUT";
    case ERR_CODE_RX_PI_DISP_MSB_STATUS_IS_1: return "ERR_CODE_RX_PI_DISP_MSB_STATUS_IS_1";
    case ERR_CODE_IMAGE_SIZE_NOT_SUPPORTED: return "ERR_CODE_IMAGE_SIZE_NOT_SUPPORTED";
    case ERR_CODE_TDT_CLIPPED_WAVEFORM: return "ERR_CODE_TDT_CLIPPED_WAVEFORM";
    case ERR_CODE_DBSTOP_NOT_WORKING: return "ERR_CODE_DBSTOP_NOT_WORKING";
    case ERR_CODE_PRBS_CHK_HW_TIMERS_NOT_EXPIRED: return "ERR_CODE_PRBS_CHK_HW_TIMERS_NOT_EXPIRED";
    case ERR_CODE_INVALID_VALUE: return "ERR_CODE_INVALID_VALUE";
    case ERR_CODE_UC_CMD_POLLING_TIMEOUT: return "ERR_CODE_UC_CMD_POLLING_TIMEOUT";
    case ERR_CODE_INVALID_INFO_TABLE_ADDR: return "ERR_CODE_INVALID_INFO_TABLE_ADDR";
    case ERR_CODE_INVALID_DIG_LPBK_STATE: return "ERR_CODE_INVALID_DIG_LPBK_STATE";
    case ERR_CODE_MEM_ALLOC_FAIL: return "ERROR_CODE_MEM_ALLOC_FAIL";
    case ERR_CODE_PRBS_CHK_DISABLED: return "ERR_CODE_PRBS_CHK_DISABLED";
    case ERR_CODE_RX_CLKGATE_FRC_ON: return "ERR_CODE_RX_CLKGATE_FRC_ON";
    case ERR_CODE_NO_PMD_RX_LOCK: return "ERR_CODE_NO_PMD_RX_LOCK";
    case ERR_CODE_SRDS_REG_ACCESS_FAIL: return "ERR_CODE_SRDS_REG_ACCESS_FAIL";
    case ERR_CODE_INVALID_UAPI_CASE: return "ERR_CODE_INVALID_UAPI_CASE";
    case ERR_TDT_PATTERN_LENGTH_WR_FAILED: return "ERR_TDT_PATTERN_LENGTH_WR_FAILED";     /* Assigned == 255 */
    default:
        switch(err_code >> 8) {
        case 1: return "ERR_CODE_TXFIR";
        case 2: return "ERR_CODE_DFE_TAP";
        case 3: return "ERR_CODE_DIAG";
        default: return "Invalid error code";
        } /* switch(err_code >> 8) */
    }  /* switch(err_code) */
#endif /* SMALL_FOOTPRINT */
}

err_code_t blackhawk7_l8p2_INTERNAL_print_err_msg(srds_access_t *sa__, uint16_t err_code, const char *filename, const char *func_name, uint16_t line_num) {
    if (err_code != ERR_CODE_NONE) {
        USR_PRINTF(("ERROR: SerDes err_code = %s : %d filename = %s, function = %s, linenum=%d\n",blackhawk7_l8p2_INTERNAL_e2s_err_code(err_code),err_code, filename, func_name, line_num));
    }
    return err_code;
}
err_code_t blackhawk7_l8p2_INTERNAL_print_err_msg_and_triage_info(srds_access_t *sa__, uint16_t err_code, const char *filename, const char *func_name, uint16_t line_num) {
    if (err_code != ERR_CODE_NONE) {
        USR_PRINTF(("ERROR:%s->%s() SerDes err_code = %s : %d %d\n", filename, func_name, blackhawk7_l8p2_INTERNAL_e2s_err_code(err_code),err_code, line_num));
        blackhawk7_l8p2_INTERNAL_print_triage_info(sa__, err_code, 1, 1, line_num);
    }
    return err_code;
}
