/**********************************************************************************
 **********************************************************************************
 *  File Name     :  srds_api_err_code.h                                          *
 *  Created On    :  18/09/2013                                                   *
 *  Created By    :  Kiran Divakar                                                *
 *  Description   :  Header file with Error Code enums                            *
 *  Revision      :  $Id: srds_api_err_code_public.h 1017 2015-04-24 22:07:57Z kirand $ *
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

/** @file srds_api_err_code.h
 * Error Code enumerations
 */

#ifndef SRDS_API_ERR_CODE_PUBLIC_H
#define SRDS_API_ERR_CODE_PUBLIC_H

#define err_code_t uint16_t

/** ERROR CODE Enum */
enum srds_err_code_enum {
    SRDS_ERR_CODE_NONE = 0,
    SRDS_ERR_CODE_INVALID_RAM_ADDR,
    SRDS_ERR_CODE_SERDES_DELAY,
    SRDS_ERR_CODE_POLLING_TIMEOUT,
    SRDS_ERR_CODE_CFG_PATT_INVALID_PATTERN,
    SRDS_ERR_CODE_CFG_PATT_INVALID_PATT_LENGTH,
    SRDS_ERR_CODE_CFG_PATT_LEN_MISMATCH,
    SRDS_ERR_CODE_CFG_PATT_PATTERN_BIGGER_THAN_MAXLEN,
    SRDS_ERR_CODE_CFG_PATT_INVALID_HEX,
    SRDS_ERR_CODE_CFG_PATT_INVALID_BIN2HEX,
    SRDS_ERR_CODE_CFG_PATT_INVALID_SEQ_WRITE,
    SRDS_ERR_CODE_PATT_GEN_INVALID_MODE_SEL,
    SRDS_ERR_CODE_INVALID_UCODE_LEN,
    SRDS_ERR_CODE_MICRO_INIT_NOT_DONE,
    SRDS_ERR_CODE_UCODE_LOAD_FAIL,
    SRDS_ERR_CODE_UCODE_VERIFY_FAIL,
    SRDS_ERR_CODE_INVALID_TEMP_IDX,
    SRDS_ERR_CODE_INVALID_PLL_CFG,
    SRDS_ERR_CODE_TX_HPF_INVALID,
    SRDS_ERR_CODE_VGA_INVALID,
    SRDS_ERR_CODE_PF_INVALID,
    SRDS_ERR_CODE_TX_AMP_CTRL_INVALID,
    SRDS_ERR_CODE_INVALID_EVENT_LOG_WRITE,
    SRDS_ERR_CODE_INVALID_EVENT_LOG_READ,
    SRDS_ERR_CODE_UC_CMD_RETURN_ERROR,
    SRDS_ERR_CODE_DATA_NOTAVAIL,
    SRDS_ERR_CODE_BAD_PTR_OR_INVALID_INPUT,
    SRDS_ERR_CODE_UC_NOT_STOPPED,
    SRDS_ERR_CODE_UC_CRC_NOT_MATCH,
    SRDS_ERR_CODE_CORE_DP_NOT_RESET,
    SRDS_ERR_CODE_LANE_DP_NOT_RESET,
    SRDS_ERR_CODE_INVALID_CLK90_ADJUST,
    SRDS_ERR_CODE_INVALID_CLK90_OVERRIDE,
    SRDS_ERR_CODE_TXFIR   = 1 << 8,
    SRDS_ERR_CODE_DFE_TAP = 2 << 8,
    SRDS_ERR_CODE_DIAG    = 3 << 8
};

/** TXFIR Error Codes Enum */
enum srds_txfir_failcodes {
    SRDS_ERR_CODE_TXFIR_PRE_INVALID         = ERR_CODE_TXFIR +   1,
    SRDS_ERR_CODE_TXFIR_MAIN_INVALID        = ERR_CODE_TXFIR +   2,
    SRDS_ERR_CODE_TXFIR_POST1_INVALID       = ERR_CODE_TXFIR +   4,
    SRDS_ERR_CODE_TXFIR_POST2_INVALID       = ERR_CODE_TXFIR +   8,
    SRDS_ERR_CODE_TXFIR_POST3_INVALID       = ERR_CODE_TXFIR +  16,
    SRDS_ERR_CODE_TXFIR_V2_LIMIT            = ERR_CODE_TXFIR +  32,
    SRDS_ERR_CODE_TXFIR_SUM_LIMIT           = ERR_CODE_TXFIR +  64,
    SRDS_ERR_CODE_TXFIR_PRE_POST1_SUM_LIMIT = ERR_CODE_TXFIR + 128
};

/** DFE Tap Error Codes Enum */
enum srds_dfe_tap_failcodes {
    SRDS_ERR_CODE_DFE1_INVALID        = ERR_CODE_DFE_TAP +  1,
    SRDS_ERR_CODE_DFE2_INVALID        = ERR_CODE_DFE_TAP +  2,
    SRDS_ERR_CODE_DFE3_INVALID        = ERR_CODE_DFE_TAP +  4,
    SRDS_ERR_CODE_DFE4_INVALID        = ERR_CODE_DFE_TAP +  8,
    SRDS_ERR_CODE_DFE5_INVALID        = ERR_CODE_DFE_TAP + 16,
    SRDS_ERR_CODE_DFE_TAP_IDX_INVALID = ERR_CODE_DFE_TAP + 32
};

/** DIAG Error Codes Enum */
enum srds_diag_failcodes {
    SRDS_ERR_CODE_DIAG_TIMEOUT               = ERR_CODE_DIAG + 1,
    SRDS_ERR_CODE_DIAG_INVALID_STATUS_RETURN = ERR_CODE_DIAG + 2,
    SRDS_ERR_CODE_DIAG_SCAN_NOT_COMPLETE     = ERR_CODE_DIAG + 3
};
#endif
