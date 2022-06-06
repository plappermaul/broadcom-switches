/*************************************************************************************
 *                                                                                   *
 * Copyright: (c) 2021 Broadcom.                                                     *
 * Broadcom Proprietary and Confidential. All rights reserved.                       *
 *                                                                                   *
 *************************************************************************************/

/**********************************************************************************
 **********************************************************************************
 *                                                                                *
 *  Revision      :  
 *                                                                                *
 *  Description   :  Defines and Enumerations required by Serdes ucode and API    *
 *                                                                                *
 **********************************************************************************
 **********************************************************************************/

/** @file srds_api_uc_common.h
 * Defines and Enumerations shared by Serdes API and Microcode
 */

#ifndef SRDS_API_UC_COMMON_PUBLIC_H
#define SRDS_API_UC_COMMON_PUBLIC_H

/* Max, min, and abs functions for SERDES code. */
/* Warning:  These can evaluate a_ or b_ twice. */
#define SRDS_MAX(a_, b_) (((a_)>(b_)) ? (a_) : (b_))
#define SRDS_MIN(a_, b_) (((a_)<(b_)) ? (a_) : (b_))
#define SRDS_ABS(a_)     (((a_)>=0)   ? (a_) : (-(a_)))

/*-----------------------------*/
/*  Generic Serdes items first */
/*-----------------------------*/

/** SERDES_PMD_UC_COMMANDS Enum */
enum srds_pmd_uc_cmd_enum {
    SRDS_CMD_NULL                  =  0,
    SRDS_CMD_UC_CTRL               =  1,
    SRDS_CMD_HEYE_OFFSET           =  2,
    SRDS_CMD_VEYE_OFFSET           =  3,
    SRDS_CMD_UC_DBG                =  4,
    SRDS_CMD_DIAG_EN               =  5,
    SRDS_CMD_READ_UC_LANE_BYTE     =  6,
    SRDS_CMD_WRITE_UC_LANE_BYTE    =  7,
    SRDS_CMD_READ_UC_CORE_BYTE     =  8,
    SRDS_CMD_WRITE_UC_CORE_BYTE    =  9,
    SRDS_CMD_READ_UC_LANE_WORD     = 10,
    SRDS_CMD_WRITE_UC_LANE_WORD    = 11,
    SRDS_CMD_READ_UC_CORE_WORD     = 12,
    SRDS_CMD_WRITE_UC_CORE_WORD    = 13,
    SRDS_CMD_EVENT_LOG_CTRL        = 14,
    SRDS_CMD_EVENT_LOG_READ        = 15,
    SRDS_CMD_CAPTURE_BER_START     = 16,
    SRDS_CMD_READ_DIAG_DATA_BYTE   = 17,
    SRDS_CMD_READ_DIAG_DATA_WORD   = 18,
    SRDS_CMD_CAPTURE_BER_END       = 19,
    SRDS_CMD_CALC_CRC              = 20,
    SRDS_CMD_FREEZE_STEADY_STATE   = 21,
    SRDS_CMD_TDT_EN                = 22,
    SRDS_CMD_UC_EMULATION          = 23,
    SRDS_CMD_CHAR_ISI              = 24,
    SRDS_CMD_INTERNAL_LPBK         = 25,
    SRDS_CMD_RUN_SC_REG_BITS_TEST  = 26,
    SRDS_CMD_CLK90_SCREEN_TEST     = 27,
    SRDS_CMD_API_EVENT             = 28,
    SRDS_CMD_DBG_FB_START          = 30,
    SRDS_CMD_DBG_FB_END            = 31,
    SRDS_CMD_RELEASE_DBSTOP        = 32,
    SRDS_CMD_UC_TEST_DBG           = 33,
SRDS_#if defined(CLANG_NOARM)
    SRDS_CMD_SAR_MEASURE           = 34,
    SRDS_CMD_TUNING_FUNCTION       = 35,
    SRDS_CMD_DIAG_FRAMEWORK        = 36,
SRDS_#endif
    SRDS_CMD_MEASURE_ENOB          = 37,
    SRDS_CMD_CONFIG_LINKCAT        = 50,
    SRDS_CMD_MAXIMUM_VALUE         = 63,             /* Maximum value for any uc_cmd */
    SRDS_CMD_INVALID_OVFL          = 64              /* Out of range for any uc_cmd */
};

/** SERDES_UC_CTRL_COMMANDS Enum */
enum srds_pmd_uc_ctrl_cmd_enum {
    SRDS_CMD_UC_CTRL_STOP_GRACEFULLY   = 0,
    SRDS_CMD_UC_CTRL_STOP_IMMEDIATE    = 1,
    SRDS_CMD_UC_CTRL_RESUME            = 2
};

/** SERDES_UC_DIAG_COMMANDS Enum */
enum srds_pmd_uc_diag_cmd_enum {
    SRDS_CMD_UC_DIAG_DISABLE         = 3,
    SRDS_CMD_UC_DIAG_PASSIVE         = 1,
    SRDS_CMD_UC_DIAG_DENSITY         = 2,
    SRDS_CMD_UC_DIAG_START_VSCAN_EYE = 4,
    SRDS_CMD_UC_DIAG_START_HSCAN_EYE = 5,
    SRDS_CMD_UC_DIAG_GET_EYE_SAMPLE  = 6,
    SRDS_CMD_UC_DIAG_START_EYE_SLICE = 7,
    SRDS_CMD_UC_DIAG_START_INTR_EYE  = 8
};


/** SERDES_UC_TEST_DBG_COMMANDS Enum */
#if defined(CLANG_NOARM)
enum srds_pmd_uc_test_dbg_cmd_enum {
    SRDS_CMD_UC_DBG_WR_ADC_SKEW_CTRL =  0,
    SRDS_CMD_UC_DBG_RD_ADC_SKEW_CTRL =  1,
    SRDS_CMD_UC_DBG_WR_DIG_ADC_GAIN = 2,
    SRDS_CMD_UC_DBG_RD_DIG_ADC_GAIN = 3,
    SRDS_CMD_UC_DBG_WR_ANA_ADC_GAIN = 4,
    SRDS_CMD_UC_DBG_RD_ANA_ADC_GAIN = 5,
    SRDS_CMD_UC_DBG_WR_FFE_OFFSET =6,
    SRDS_CMD_UC_DBG_RD_FFE_OFFSET =7,
    SRDS_CMD_UC_DBG_WR_DFE_TAP=8,
    SRDS_CMD_UC_DBG_RD_DFE_TAP=9,
    SRDS_CMD_UC_DBG_WR_DFE_OFFSET=10,
    SRDS_CMD_UC_DBG_RD_DFE_OFFSET=11,
    SRDS_CMD_UC_DBG_WR_FFE_TAP=12,
    SRDS_CMD_UC_DBG_RD_FFE_TAP=13,
    SRDS_CMD_UC_DBG_WR_RCFIR_TAP=14,
    SRDS_CMD_UC_DBG_RD_RCFIR_TAP=15,
    SRDS_CMD_UC_DBG_WR_PGA_VAL=16,
    SRDS_CMD_UC_DBG_RD_PGA_VAL=17
};

enum srds_pmd_uc_sar_measure_cmd_enum {
    SRDS_CMD_SAR_MEASURE_OFFSET  = 0,
    SRDS_CMD_SAR_MEASURE_GAIN    = 1,
    SRDS_CMD_SAR_SWEEP_OFFSET    = 2,
    SRDS_CMD_SAR_SWEEP_GAIN      = 3,
    SRDS_CMD_SAR_MEASURE_DONE    = 10
};

enum srds_pmd_uc_tuning_cmd_enum {
    SRDS_CMD_TUNING_ADC_HW_TIMING_CAL,
    SRDS_CMD_TUNING_ADC_HW_OFFSET_CAL,
    SRDS_CMD_TUNING_CFG_CDR_BLIND_MODE,
    SRDS_CMD_TUNING_BLIND_DCO,
    SRDS_CMD_TUNING_BLIND_PGA,
    SRDS_CMD_TUNING_BLIND_CTLE,
    SRDS_CMD_TUNING_BLIND_FFE_OFFSET,
    SRDS_CMD_TUNING_BLIND_ADC_GAIN,
    SRDS_CMD_TUNING_BLIND_ADC_SKEW,
    SRDS_CMD_TUNING_OPTIMIZE_FFE_OFFSET,
    SRDS_CMD_TUNING_OPTIMIZE_ADC_GAIN,
    SRDS_CMD_TUNING_OPTIMIZE_ADC_SKEW
};
#endif
#if defined(CLANG_NOARM) 
/** SERDES_UC_EMULATION_COMMANDS Enum */
enum srds_pmd_uc_emulation_cmd_enum {
    SRDS_CMD_UC_EMULATION_START      = 0,
    SRDS_CMD_UC_EMULATION_STOP       = 1
};
#endif

/** SERDES_EVENT_LOG_READ Enum */
enum srds_pmd_event_rd_cmd_enum {
    SRDS_CMD_EVENT_LOG_READ_START      = 0,
    SRDS_CMD_EVENT_LOG_READ_NEXT       = 1,
    SRDS_CMD_EVENT_LOG_READ_DONE       = 2
};

/** SERDES_UC_DBG_COMMANDS Enum */
enum srds_pmd_uc_dbg_cmd_enum {
    SRDS_CMD_UC_DBG_DIE_TEMP        = 0,
    SRDS_CMD_UC_DBG_TIMESTAMP       = 1,
    SRDS_CMD_UC_DBG_LANE_IDX        = 2,
    SRDS_CMD_UC_DBG_LANE_TIMER      = 3
};

/** SERDES_INTERNAL_LPBK_COMMANDS Enum */
enum srds_pmd_internal_lpbk_cmd_enum {
    SRDS_CMD_INTERNAL_LPBK_DISABLE      = 0,
    SRDS_CMD_INTERNAL_LPBK_ENABLE       = 1
};

/** Erroneous command completion codes to be placed in supp_info.           **
 ** Values 0-15 are allowed.  (See MAKE_UC_ERROR_SUPP_INFO below.)          **
 ** Please update serdes_INTERNAL_print_uc_dsc_error() in                   **
 ** serdes_api_internal.c when adding to or removing from this list.        **/
enum srds_pmd_uc_error_enum {
    SRDS_UC_CMD_ERROR_INVALID_COMMAND              = 0x1,
    SRDS_UC_CMD_ERROR_BUSY                         = 0x5,
    SRDS_UC_CMD_ERROR_GET_EYE_SAMPLE_ERROR         = 0x6,
    SRDS_UC_CMD_ERROR_PRBS_NOT_LOCKED              = 0x8,
    SRDS_UC_CMD_ERROR_COMMAND_IN_PROGRESS          = 0xC,
    SRDS_UC_CMD_ERROR_INVALID_MODE                 = 0xD
};

/** Macros for translating srds_pmd_uc_error_enum error codes    **
 ** into the supp_info field that is read by software, and back. **/
#define MAKE_UC_ERROR_SUPP_INFO(error_code, other_info) ((((error_code) & 0xF) << 4) | ((other_info) & 0xF))
#define SUPP_INFO_TO_ERROR_CODE(supp_info) (((supp_info) >> 4) & 0xF)
#define SUPP_INFO_TO_OTHER_INFO(supp_info) ((supp_info) & 0xF)

/** supp_info values used with successful command completion **/
enum srds_pmd_uc_success_enum {
    SRDS_UC_CMD_SUCCESS               = 0x00,
    SRDS_UC_CMD_SUCCESS_READ_COMPLETE = 0x01
};

/** Media type Enum */
enum srds_media_type_enum {
    SRDS_MEDIA_TYPE_PCB_TRACE_BACKPLANE = 0,
    SRDS_MEDIA_TYPE_COPPER_CABLE        = 1,
    SRDS_MEDIA_TYPE_OPTICS              = 2,
    SRDS_MEDIA_TYPE_OPTICS_ZR           = 3
};

/** DIAG_BER mode settings **/
enum srds_diag_ber_mode_enum {
    SRDS_DIAG_BER_POS  = 0,
    SRDS_DIAG_BER_NEG  = 1,
    SRDS_DIAG_BER_VERT = 0,
    SRDS_DIAG_BER_HORZ = 1<<1,
    SRDS_DIAG_BER_PASS = 0,
    SRDS_DIAG_BER_INTR = 1<<2,
    SRDS_DIAG_BER_P1_NARROW = 1<<3,
    SRDS_DIAG_BER_P1_WIDE = 0,
    SRDS_DIAG_BER_FAST = 1<<6
};

/** DIAG EYE SCAN status macros **/
#define DIAG_EYE_SCAN_V_HALF        (1<<10)
#define DIAG_EYE_SCAN_H_FULL        (0<<11)
#define DIAG_EYE_SCAN_H_HALF        (1<<11)
#define DIAG_EYE_SCAN_INTR          (2<<11)
#define DIAG_EYE_SCAN_SLICE         (3<<11)
#define DIAG_EYE_SCAN_READ_FASTER   (1<<13)
#define DIAG_EYE_SCAN_WRITE_FASTER  (1<<14)
#define DIAG_EYE_SCAN_END           (1<<15)

/** DIAG EYE SCAN status mask macros **/
#define DIAG_EYE_SCAN_BUFFER_SIZE_MASK  (0xFF)
#define DIAG_EYE_SCAN_V_RANGE_MASK      (1<<10)
#define DIAG_EYE_SCAN_RANGE_MASK        (3<<11)
#define DIAG_EYE_SCAN_READ_MASK         (1<<13)
#define DIAG_EYE_SCAN_WRITE_MASK        (1<<14)
#define DIAG_EYE_SCAN_END_MASK          (1<<15)


typedef uint8_t float8_t;   /* used to decimate or truncate large floating eye data */

/*****************************************************************************\
 * Information table found in code RAM                                       *
 *                                                                           *
 * It starts at this location:                                               *
\*                                                                           */
#define INFO_TABLE_RAM_BASE  (0x100)
/*                                                                           *\
 * The table is signified by this signature, "INF(A)", whose last character  *
 * is the version number. But is the first 2 hex characters in the signature *
 * (A) = 0x41 ASCII code                                                     *
\*                                                                           */
#define INFO_TABLE_SIGNATURE (0x42464E49)
/*                                                                           *\
 * The table starts with the signature and various version fields at these   *
 * offsets:                                                                  *
\*                                                                           */
#define INFO_TABLE_OFFS_SIGNATURE               (0x00)
#define INFO_TABLE_OFFS_UC_VERSION              (0x04)
/*                                                                           *\
 * The table includes the trace and lane memory size:                        *
 *     (upper 16 bits is lane size; lower 16 bits is trace size)             *
\*                                                                           */
#define INFO_TABLE_OFFS_TRACE_LANE_MEM_SIZE     (0x08)
/*                                                                           *\
 * The table includes other sizes:                                           *
 *     Byte 0:  lane count                                                   *
 *     Byte 1:  core size, in bytes                                          *
 *     Byte 2:  icore size, in bytes                                         *
 *     Byte 3:  options:                                                     *
 *              Bit 0:  0 if trace memory is written with ascending          *
 *                           addresses.                                      *
 *                      1 if trace memory is written with descending         *
 *                           addresses.                                      *
\*                                                                           */
#define INFO_TABLE_OFFS_OTHER_SIZE              (0x0C)
/*                                                                           *\
 * The table includes RAM locations of various tables:                       *
\*                                                                           */
#define INFO_TABLE_OFFS_TRACE_MEM_PTR           (0x10)
#define INFO_TABLE_OFFS_CORE_MEM_PTR            (0x14)
#define INFO_TABLE_OFFS_ICORE_MEM_PTR           (0x18)
#define INFO_TABLE_OFFS_LANE_MEM_PTR            (0x1C)
#define INFO_TABLE_OFFS_MICRO_MEM_PTR           (0x64)
#define INFO_TABLE_OFFS_GROUP_RAM_SIZE          (0x68)
#define INFO_TABLE_OFFS_DIAG_LANE_MEM_SIZE      (0x6C)
#define INFO_TABLE_OFFS_DIAG_MEM_PTR            (0x70)
#define INFO_TABLE_OFFS_LANE_STATIC_MEM_PTR     (0x74)   /* Only used in  uCode. */
#define INFO_TABLE_OFFS_LANE_STATIC_MEM_SIZE    (0x78)   /* Only used in  uCode. */

#define INFO_TABLE_OFFS_COMMON_BLOCK_MEM_PTR    (0x7C)   /* Only used in Peregrine uCode. */
#define INFO_TABLE_OFFS_COMMON_BLOCK_MEM_SIZE   (0x80)   /* Only used in Peregrine uCode. */

#define INFO_TABLE_OFFS_DEBUG_BLOCK_MEM_PTR     (0x84)   /* Only used in Peregrine uCode. */
#define INFO_TABLE_OFFS_DEBUG_BLOCK_MEM_SIZE    (0x88)   /* Only used in Peregrine uCode. */

/*                                                                           *\
 * The table includes other sizes:                                           *
 *     Byte 0:  microcontroller count                                        *
\*                                                                           */
#define INFO_TABLE_OFFS_OTHER_SIZE_2            (0x60)

/*****************************************************************************/

/* The offset just past the end of the table */
/* _LEGACY is for Merlin7, BHK7, OSP7, OSPL7 */
#define INFO_TABLE_END_LEGACY                (0x80)
/* _COMMONBLK is for DSP CORES - PER5 */
#define INFO_TABLE_END_COMMON_BLOCK          (0x8C)


#endif/*SRDS_API_UC_COMMON_PUBLIC_H*/
