/***********************************************************************************
 *                                                                                 *
 * Copyright: (c) 2021 Broadcom.                                                   *
 * Broadcom Proprietary and Confidential. All rights reserved.                     *
 *                                                                                 *
 ***********************************************************************************/

/***********************************************************************************
 ***********************************************************************************
 *                                                                                 *
 *  Revision    :       *
 *                                                                                 *
 *  Description :  PRBS test functions provided to IP User                         *
 *                                                                                 *
 ***********************************************************************************
 ***********************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MERLIN7_PCIEG3_API_PRBS_H
#define MERLIN7_PCIEG3_API_PRBS_H

#include "merlin7_pcieg3_ipconfig.h"
#include "common/srds_api_enum.h"
#include "common/srds_api_err_code.h"
#include "common/srds_api_types.h"
#include "merlin7_pcieg3_usr_includes.h"
#include "merlin7_pcieg3_types.h"

/*! @file
 *  @brief PRBS test functions provided to IP User
 */

/*! @addtogroup APITag
 * @{
 */

/*! @defgroup SerdesAPIPrbsTag PRBS APIs
 * Serdes API functions for enabling, disabling, and
 * configuring PRBS APIs. Also contains PRBS error analyzer
 * and BER APIs.
 */

/*! @addtogroup SerdesAPIPrbsTag
 * @{
 */

#define PRBS_VERBOSE 0

#define PCS_PRBS_ENABLE_ADDR        (0x1501)
#define PCS_PRBS_INVERSE_ADDR       (0x1502)
#define PCS_PRBS_ORDER_ADDR         (0x1504)
#define PCS_PRBS_LOCK_ERR_ADDR      (0x7000)
#define PCS_PRBS_MONITOR_ADDR       (0x7003)
/*---------------------------------------*/
/** Serdes prbs chk timer selection Enum */
/*---------------------------------------*/
enum merlin7_pcieg3_prbs_chk_timer_selection_enum {
    USE_HW_TIMERS = 0,
    USE_SW_TIMERS = 1
};
/*------------------------------*/
/*  Shared TX Pattern Generator */
/*------------------------------*/
/** Configure Shared TX Pattern API.
 * An input string (hex or binary) and pattern length are taken in as inputs, based on which the Pattern Generator registers.
 * are programmed to the appropriate values to generate that pattern.\n
 * eg: For a repeating pattern "0000010110101111", input_pattern = "0000010110101111" or "0x05AF" and patt_length_bits = 16.\n\n
 * NOTE: merlin7_pcieg3_tx_shared_patt_gen_en() API should be called to enable the Pattern generator for that particular lane. \n
 * @param[in] sa__ is an opaque state vector passed through to device access functions.
 * @param[in] patt_length_bits Pattern length in bits.
 * @param[in] pattern Input Pattern - Can be in hex (eg: "0xB055") or in binary (eg: "011011").
 *
 * @b Example
 * @code
    EFUN(merlin7_pcieg3_config_shared_tx_pattern(sa__, 4, "0101"));
    EFUN(merlin7_pcieg3_tx_shared_patt_gen(en(sa__, 1, 4));
 * @endcode
 * @return Error Code generated by invalid input pattern or pattern length (returns ERR_CODE_NONE if no errors).
 */
err_code_t merlin7_pcieg3_config_shared_tx_pattern(srds_access_t *sa__, uint8_t patt_length_bits, const char pattern[]);

/**************************************************/
/* LANE Based APIs - Required to be used per Lane */
/**************************************************/

/*----------------------------*/
/*  Enable Pattern Generator  */
/*----------------------------*/
/** Enable/Disable Shared TX pattern generator.
 * Note: The patt_length_bits input to the function should be the value sent to the merlin7_pcieg3_config_shared_tx_pattern() function
 * @param[in] sa__ is an opaque state vector passed through to device access functions.
 * @param[in] enable Enable shared fixed pattern generator (1 = Enable; 0 = Disable)
 * @param[in] patt_length_bits length of the pattern in bits used in merlin7_pcieg3_config_shared_tx_pattern()
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors)
 */
err_code_t merlin7_pcieg3_tx_shared_patt_gen_en(srds_access_t *sa__, uint8_t enable, uint8_t patt_length_bits);

/*----------------------------*/
/*  Configure PRBS Functions  */
/*----------------------------*/

/**  Configure PMD/PCS PRBS Generator.
 * Once the PRBS generator is configured, to enable PRBS use the merlin7_pcieg3_tx_prbs_en() API.
 * Note: The following 3 register writes are recommended before configuring/enabling PRBS:
 * Note: 1. EFUN(merlin7_pcieg3_acc_wr_reg(sa__, 0x5001, 0x8000)); (Disable TX rcvr detect req from MAC)
 * Note: 2. EFUN(merlin7_pcieg3_acc_wr_reg(sa__, 0x1300, 0x2080)); (Disable TX eidle from MAC)
 * Note: 3. EFUN(merlin7_pcieg3_acc_wr_reg(sa__, 0x1402, 0x0000)); (Disable 8b10b & verify)
 * @param[in] sa__ is an opaque state vector passed through to device access functions.
 * @param[in] prbs_poly_mode PRBS generator mode select (selects required PRBS polynomial).
 * @param[in] prbs_inv PRBS invert enable.
 *
 * @b Example
 * @code
    EFUN(merlin7_pcieg3_acc_wr_reg(sa__, 0x5001, 0x8000));
    EFUN(merlin7_pcieg3_config_shared_tx_pattern(sa__, 4, "0101"));
    EFUN(merlin7_pcieg3_acc_wr_reg(sa__, 0x1300, 0x2080));
    EFUN(merlin7_pcieg3_acc_wr_reg(sa__, 0x1402, 0x0000));
    EFUN(merlin7_pcieg3_config_tx_prbs(sa__, PCS_PRBS_7, 0));
    EFUN(merlin7_pcieg3_tx_prbs_en(sa__, 2)); (enable PCS)
 * @endcode

 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors).
 */
err_code_t merlin7_pcieg3_config_tx_prbs(srds_access_t *sa__, enum srds_prbs_polynomial_enum prbs_poly_mode, uint8_t prbs_inv);

/**  Get PMD/PCS PRBS Generator Configuration.
 * Note: If only PCS PRBS Generator is enabled, returns PCS PRBS Generator Configuration, else returns PMD PRBS Generator Configuration.
 * @param[in] sa__ is an opaque state vector passed through to device access functions.
 * @param[out] prbs_poly_mode PRBS generator mode select (selects required PRBS polynomial).
 * @param[out] prbs_inv PRBS invert enable.
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors).
 */
err_code_t merlin7_pcieg3_get_tx_prbs_config(srds_access_t *sa__, enum srds_prbs_polynomial_enum *prbs_poly_mode, uint8_t *prbs_inv);

/** PMD/PCS PRBS Generator Enable/Disable.
 * Note: Because the generator and checker for PCS use the same register, disabling or enabling the generator will also respectively disable or enable the checker.
 * @param[in] sa__ is an opaque state vector passed through to device access functions.
 * @param[in] enable Enable PRBS Generator (1 = Enable PMD; 2 = Enable PCS; 0 = Disable).
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors).
 */
err_code_t merlin7_pcieg3_tx_prbs_en(srds_access_t *sa__, uint8_t enable);

/** Get PMD/PCS PRBS Generator Enable/Disable.
 * @param[in] sa__ is an opaque state vector passed through to device access functions.
 * @param[out] enable returns the value of Enable PRBS Generator (1 = PMD Enable; 2 = PCS Enable; 0 = Disable).
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors).
 */
err_code_t merlin7_pcieg3_get_tx_prbs_en(srds_access_t *sa__, uint8_t *enable);

/** PMD/PCS PRBS Generator Single Bit Error Injection.
 * Note: Does not work for PCS PRBS Generator. Only works for PMD PRBS Generator.
 * @param[in] sa__ is an opaque state vector passed through to device access functions.
 * @param[in] enable (1 = error is injected; 0 = no error is injected).
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors).
 */
err_code_t merlin7_pcieg3_tx_prbs_err_inject(srds_access_t *sa__, uint8_t enable);

/**  Get PMD/PCS PRBS Checker congifuration.
 * Note: If only PCS PRBS Generator is enabled, returns PCS PRBS Checker Configuration, else returns PMD PRBS Checker Configuration.
 * @param[in] sa__ is an opaque state vector passed through to device access functions.
 * @param[out] prbs_poly_mode PRBS checker mode select (selects required PRBS polynomial).
 * @param[out] prbs_checker_mode Checker Mode to select PRBS LOCK state machine.
 * @param[out] prbs_inv PRBS invert enable.
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors).
 */
err_code_t merlin7_pcieg3_get_rx_prbs_config(srds_access_t *sa__, enum srds_prbs_polynomial_enum *prbs_poly_mode, enum srds_prbs_checker_mode_enum *prbs_checker_mode, uint8_t *prbs_inv);

/**  Configure PMD/PCS PRBS Checker.
 * Once the PRBS checker is configured, use the merlin7_pcieg3_rx_prbs_en() API to enable the checker.
 * @param[in] sa__ is an opaque state vector passed through to device access functions.
 * @param[in] prbs_poly_mode PRBS checker mode select (selects required PRBS polynomial).
 * @param[in] prbs_checker_mode Checker Mode to select PRBS LOCK state machine.
 * @param[in] prbs_inv PRBS invert enable.
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors).
 */
err_code_t merlin7_pcieg3_config_rx_prbs(srds_access_t *sa__, enum srds_prbs_polynomial_enum prbs_poly_mode, enum srds_prbs_checker_mode_enum prbs_checker_mode, uint8_t prbs_inv);

/** PMD/PCS PRBS Checker Enable/Disable.
 * Note: Because the generator and checker for PCS use the same register, disabling/enabling the checker will also disable/enable the generator.
 * @param[in] sa__ is an opaque state vector passed through to device access functions.
 * @param[in] enable Enable PRBS Checker (1 = Enable PMD; 2 = Enable PCS; 0 = Disable).
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors).
 */
err_code_t merlin7_pcieg3_rx_prbs_en(srds_access_t *sa__, uint8_t enable);

/** Get PMD/PCS PRBS Checker Enable/Disable.
 * @param[in] sa__ is an opaque state vector passed through to device access functions.
 * @param[out] enable returns the value of Enable PRBS Checker (1 = PMD Enable; 2 = PCS Enable; 0 = Disable).
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors).
 */
err_code_t merlin7_pcieg3_get_rx_prbs_en(srds_access_t *sa__, uint8_t *enable);

/**  PMD/PCS PRBS Checker LOCK status (live status).
 * Note: If only PCS PRBS Checker is enabled, returns PCS PRBS lock state, else returns PMD PRBS lock state.
 * @param[in] sa__ is an opaque state vector passed through to device access functions.
 * @param[out] chk_lock Live lock status read by API.
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors).
 */
err_code_t merlin7_pcieg3_prbs_chk_lock_state(srds_access_t *sa__, uint8_t *chk_lock);

/** PMD/PCS PRBS Error Count and Lock Lost status.
 * Note: If only PCS PRBS Checker is enabled, returns PCS PRBS Error Count and Lock Lost status, else returns PMD PRBS Error Count and Lock Lost status.
 * Error count and lock lost read back as a single 32bit value. Bit 31 is lock lost and [30:0] is error count.
 * @param[in] sa__ is an opaque state vector passed through to device access functions.
 * @param[out] prbs_err_cnt 32bit value returned by API ([30:0] = Error Count; [31] = Lock lost).
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors).
 */
err_code_t merlin7_pcieg3_prbs_err_count_ll(srds_access_t *sa__, uint32_t *prbs_err_cnt);

/** PMD/PCS PRBS Error Count and Lock Lost status.
 * Error count and lock lost read back on separate variables.
 * Note: If only PCS PRBS Checker is enabled, returns PCS PRBS Error Count and Lock Lost status, else returns PMD PRBS Error Count and Lock Lost status.
 * @param[in] sa__ is an opaque state vector passed through to device access functions.
 * @param[out] prbs_err_cnt 32bit Error count value.
 * @param[out] lock_lost Lock Lost status (1 = if lock was ever lost).
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors).
 */
err_code_t merlin7_pcieg3_prbs_err_count_state(srds_access_t *sa__, uint32_t *prbs_err_cnt, uint8_t *lock_lost);


enum prbs_chk_lock_state_enum {
    PRBS_CHECKER_NOT_ENABLED = 0,
    PRBS_CHECKER_WAITING_FOR_LOCK = 1,
    PRBS_CHECKER_LOCKED = 2,
    PRBS_CHECKER_LOCK_LOST_AFTER_LOCK = 3
};


/** Header display for detailed PRBS display function.
 * This can be displayed once and then several cores and lanes after each showing one line.
 * @param[in] sa__ is an opaque state vector passed through to device access functions.
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors).
 */
err_code_t merlin7_pcieg3_display_detailed_prbs_state_hdr(srds_access_t *sa__);

/** Display detailed PRBS data per lane including Burst Error.
 *
 * @param[in] sa__ is an opaque state vector passed through to device access functions.
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors)
 */
err_code_t merlin7_pcieg3_display_detailed_prbs_state(srds_access_t *sa__);

/** Get a srds_prbs_polynomial_enum string.
 *
 * @param[in] prbs_poly_mode The PRBS enum value which a string should be returned for
 * @return Pointer to the PRBS enum in string form. Returns "ERR" for unsupported values and "UNKNOWN" for invalid values.
 */
const char* merlin7_pcieg3_get_e2s_prbs_mode_enum(enum srds_prbs_polynomial_enum prbs_poly_mode);

/** Get a PCS PRBS string.
 *
 * @param[in] prbs_poly_mode The PRBS enum value which a string should be returned for
 * @return Pointer to the PRBS enum in string form. Returns "ERR" for unsupported values and "UNKNOWN" for invalid values.
 */
const char* merlin7_pcieg3_get_e2s_pcs_prbs_mode_enum(enum srds_prbs_polynomial_enum prbs_poly_mode);


/** Issue commands to PCS to utilize its PRBS generator in a PRBS23 pattern.
 * @param[in] sa__ is an opaque state vector passed through to device access functions.
 * @return Error Code generated by API (ERR_CODE_NONE if no errors).
 */
err_code_t merlin7_pcieg3_pcs_prbs23_enable(srds_access_t *sa__);

/** Call this function to continually Read PCS lanes for PRBS status.
 * Read PRBS monitor; status=0x8000 is passing.
 * @param[in] sa__ is an opaque state vector passed through to device access functions.
 * @return Error Code generated by API (ERR_CODE_NONE if no errors).
 */
err_code_t merlin7_pcieg3_pcs_prbs_monitor_status(srds_access_t *sa__);

/*! @} SerdesAPIPrbsTag */
/*! @} APITag */
#endif /* !SERDES_API_PRBS_H */
#ifdef __cplusplus
}
#endif
