/***********************************************************************************
 *                                                                                 *
 * Copyright: (c) 2021 Broadcom.                                                   *
 * Broadcom Proprietary and Confidential. All rights reserved.                     *
 *                                                                                 *
 ***********************************************************************************/

/***********************************************************************************
 ***********************************************************************************
 *                                                                                 *
 *  Revision    :      *
 *                                                                                 *
 *  Description :  Config functions targeted to IP user                            *
 *                                                                                 *
 ***********************************************************************************
 ***********************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MERLIN7_PCIEG3_API_CONFIG_H
#define MERLIN7_PCIEG3_API_CONFIG_H

#include "merlin7_pcieg3_ipconfig.h"
#include "common/srds_api_enum.h"
#include "common/srds_api_err_code.h"
#include "common/srds_api_types.h"
#include "merlin7_pcieg3_enum.h"
#include "merlin7_pcieg3_types.h"
#include "merlin7_pcieg3_select_defns.h"
#include "merlin7_pcieg3_access.h"

/*! @file
 *  @brief Configuration functions provided to IP User
 */

/*! @addtogroup APITag
 * @{
 */

/*! @defgroup SerdesAPIConfigTag Configuration APIs
 * Serdes API functions which can be used to get and/or set
 * Serdes configuration settings, as well as enable/disable modes.
 * Also includes APIs for loading and verifying uCode.
 */

/*! @addtogroup SerdesAPIConfigTag
 * @{
 */

#define GRACEFUL_STOP_TIME 200

#define IF_API_ID_CODES_DONT_MATCH_CORE_ID_CODES      \
uint16_t revid, revid2;                               \
ESTM(revid     = rdc_revid_model());                  \
ESTM(revid2    = rdc_revid2());                       \
if (MERLIN7_PCIEG3_REVID_MODEL_DEFAULT != revid || MERLIN7_PCIEG3_REVID2_DEFAULT != revid2) { \
    EFUN_PRINTF(("ERROR: IP Hardware ID Mismatch. Expected: 0x%08X. Actual: 0x%08X.\n",                                                     \
                (uint32_t)((MERLIN7_PCIEG3_REVID_MODEL_DEFAULT << 16) | (MERLIN7_PCIEG3_REVID2_DEFAULT << 8)),                                              \
                (uint32_t)((revid << 16) | (revid2 << 8))));



#define RELEASE_LOCK_AND_RETURN(__x__) \
    USR_RELEASE_LOCK; \
    return (__x__);

/****************************************************/
/*  CORE Based APIs - Required to be used per Core  */
/****************************************************/
/* Returns API Version Number */
/** API Version Number.
 * @param[in] sa__ is an opaque state vector passed through to device access functions.
 * @param[out] api_version API Version Number returned by the API
 * @return Error Code, if generated (returns ERR_CODE_NONE if no errors)
 */
err_code_t merlin7_pcieg3_version(srds_access_t *sa__, uint32_t *api_version);



/*------------------------------------------------*/
/*  APIs to get number of uc cores  */
/*------------------------------------------------*/
/** Get the number of uc cores.
 *  Note that various API configuration functions can modify core config.
 *  Since the value returned by this can become stale, re-read after modifying core configuration.
 * @param[in] sa__ is an opaque state vector passed through to device access functions.
 * @param[out] num_micros Value containing the number of uc cores returned by the API.
 * @return Error Code, if generated (returns ERR_CODE_NONE if no errors)
 */
err_code_t merlin7_pcieg3_get_micro_num_uc_cores(srds_access_t *sa__, uint8_t *num_micros);

/*-----------------------------------*/
/*  Microcode Load/Verify Functions  */
/*-----------------------------------*/
/** Performs the initial register writes which are required before loading microcode.
 * @param[in] sa__ is an opaque state vector passed through to device access functions.
 * @param[in] pram indicates if the microcode is loaded through the PRAM interface (1 = yes; 0 = no )
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors)
 */
err_code_t merlin7_pcieg3_ucode_load_init(srds_access_t *sa__, uint8_t pram);

/** Writes all or part of the microcode image into Micro.
 * Note: If microcode is being loaded with multiple writes, then ucode_len must be divisble by 4 (32 bit aligned)
 * for all writes, except for the final write, which does not need to be 32 bit aligned.
 * Note: merlin7_pcieg3_ucode_load_init() must be called before the first call to merlin7_pcieg3_ucode_load_write(),
 * and merlin7_pcieg3_ucode_load_close() must be called after the last call to merlin7_pcieg3_ucode_load_write().
 * @param[in] sa__ is an opaque state vector passed through to device access functions.
 * @param[in] ucode_image pointer to the Microcode image organized in bytes.
 * @param[in] ucode_len Length of Microcode Image (number of bytes).
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors)
 */
err_code_t merlin7_pcieg3_ucode_load_write(srds_access_t *sa__, uint8_t *ucode_image, uint32_t ucode_len);

/** Ends Microcode loading state. Call after the last merlin7_pcieg3_ucode_load_write().
 * @param[in] sa__ is an opaque state vector passed through to device access functions.
 * @param[in] pram indicates if the microcode is loaded through the PRAM interface (1 = yes; 0 = no )
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors)
 */
err_code_t merlin7_pcieg3_ucode_load_close(srds_access_t *sa__, uint8_t pram);

/** Load Microcode into current one core through Register (MDIO) Interface.
 * Once the microcode is loaded, de-assert reset to micro to start executing microcode "wrc_micro_core_rstb(0x1)".
 * Note: Micro should be loaded only after issuing a merlin7_pcieg3_uc_reset(1) followed by asserting and de-asserting
 * core_s_reset. Information table should be intialized with merlin7_pcieg3_init_merlin7_pcieg3_info after microcode load.
 * See relevant Programmers guide for more details.
 * @param[in] sa__ is an opaque state vector passed through to device access functions.
 * @param[in] ucode_image pointer to the Microcode image organized in bytes.
 * @param[in] ucode_len Length of Microcode Image (number of bytes).
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors)
 */
err_code_t merlin7_pcieg3_ucode_load(srds_access_t *sa__, uint8_t *ucode_image, uint32_t ucode_len);


/** To verify the Microcode image loaded in the Micro.
 * Read back the microcode from Micro and check against expected microcode image.
 * @param[in] sa__ is an opaque state vector passed through to device access functions.
 * @param[in] ucode_image pointer to the expected Microcode image organized in bytes.
 * @param[in] ucode_len Length of Microcode Image (number of bytes).
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors)
 */
err_code_t merlin7_pcieg3_ucode_load_verify(srds_access_t *sa__, uint8_t *ucode_image, uint32_t ucode_len);

/** To verify the CRC of the microcode loaded in the Micro.
 * Instruct uC to read image and calculate CRC and check against expected CRC.
 * @param[in] sa__ is an opaque state vector passed through to device access functions.
 * @param[in] ucode_len Length of Microcode Image (number of bytes)
 * @param[in] expected_crc_value Expected CRC value of the microcode
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors)
 */
err_code_t merlin7_pcieg3_ucode_crc_verify(srds_access_t *sa__, uint32_t ucode_len, uint16_t expected_crc_value);

/** To instruct the micro to start calculating the CRC of the microcode.
 * Control is returned after triggering start of CRC calculation (does NOT wait for completion of CRC calculation). \n \n
 * NOTE: No uC commands should be executed between starting the CRC calculaion [merlin7_pcieg3_ucode_crc_verify()] and verifying the CRC value [merlin7_pcieg3_start_ucode_crc_calc()].
 * @param[in] sa__ is an opaque state vector passed through to device access functions.
 * @param[in] ucode_len Length of Microcode Image (number of bytes).
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors).
 */
err_code_t merlin7_pcieg3_start_ucode_crc_calc(srds_access_t *sa__, uint16_t ucode_len);

/** To check the expected CRC against the CRC calulated by the micro.
 * NOTE: No uC commands should be executed between starting the CRC calculation [merlin7_pcieg3_ucode_crc_verify()] and verifying the CRC value [merlin7_pcieg3_start_ucode_crc_calc()].
 * @param[in] sa__ is an opaque state vector passed through to device access functions.
 * @param[in] expected_crc_value Expected CRC value of the microcode.
 * @param[in] timeout_ms Time interval in milliseconds inside which the previous command (calculate CRC) should be completed.
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors).
 */
err_code_t merlin7_pcieg3_check_ucode_crc(srds_access_t *sa__, uint16_t expected_crc_value, uint32_t timeout_ms);

/** Load the Micro through the PRAM bus.
 * NOTE: Information table should be intialized with merlin7_pcieg3_init_merlin7_pcieg3_info after microcode load.
 * @param[in] sa__ is an opaque state vector passed through to device access functions.
 * @param[in] ucode_image Microcode Image to be written.
 * @param[in] ucode_len Length of Microcode Image (number of bytes).
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors)
 */
err_code_t merlin7_pcieg3_ucode_pram_load(srds_access_t *sa__, char const * ucode_image, uint32_t ucode_len);

/** Enable or Disable the uC reset.
 * Dummy function to maintain compatibility with BHK7 APIs [calls merlin7_pcieg3_uc_reset(..) internally].
 * Note: Micro should be reset using the API everytime before reloading the microcode.
 * @param[in] sa__ is an opaque state vector passed through to device access functions.
 * @param[in] enable Enable/Disable uC reset (1 = Enable; 0 = Disable).
 * @param[in] ucode_info struct has information regarding stack size.
 * @return Error Code generated by invalid access (returns ERR_CODE_NONE if no errors).
 */
err_code_t merlin7_pcieg3_uc_reset_with_info(srds_access_t *sa__, uint8_t enable, ucode_info_t ucode_info);

/** Enable or Disable the uC reset.
 * Note: Micro should be reset using the API everytime before reloading the microcode.
 * @param[in] sa__ is an opaque state vector passed through to device access functions.
 * @param[in] enable Enable/Disable uC reset (1 = Enable; 0 = Disable).
 * @param[in] ucode_info struct has information regarding stack size.
 * @return Error Code generated by invalid access (returns ERR_CODE_NONE if no errors)
 */
err_code_t merlin7_pcieg3_uc_reset(srds_access_t *sa__, uint8_t enable, ucode_info_t ucode_info);

/** API to assert Serdes core reset.
 * Issues merlin7_pcieg3_uc_reset followed by merlin7_pcieg3 core reset (core_s_rstb) after selecting com_clk as micro_clk.
 * NOTE: This API should be used to issue a core reset instead of directly using the core_s_rstb register.
 * @param[in] sa__ is an opaque state vector passed through to device access functions.
 * @param[in] ucode_info struct has information regarding stack size.
 * @return Error Code generated by invalid access (returns ERR_CODE_NONE if no errors).
 */
err_code_t merlin7_pcieg3_core_reset(srds_access_t *sa__, ucode_info_t ucode_info);


#define INFO_TABLE_END INFO_TABLE_END_LEGACY

/** Initialize the merlin7_pcieg3_info for the uC.
 * @param[in] sa__ is an opaque state vector passed through to device access functions.
 * @return Error Code generated if uC does not become active (returns ERR_CODE_NONE if no errors).
 */
err_code_t merlin7_pcieg3_init_merlin7_pcieg3_info(srds_access_t *sa__);


/*--------------------------------------------*/
/*  APIs to Reset Lane to Default             */
/*--------------------------------------------*/

/** Resets TX Lane to Default Configuration.
 * Toggles TX ln_s_rstb and leaves the lane TX in datapath reset.
 * @param[in] sa__ is an opaque state vector passed through to device access functions.
 * @return Error Code generated by invalid access (returns ERR_CODE_NONE if no errors).
 */
err_code_t merlin7_pcieg3_reset_tx_lane_to_default(srds_access_t *sa__);

/** Resets RX Lane to Default Configuration.
 * Toggles RX ln_s_rstb and leaves the lane RX in datapath reset.
 * @param[in] sa__ is an opaque state vector passed through to device access functions.
 * @return Error Code generated by invalid access (returns ERR_CODE_NONE if no errors).
 */
err_code_t merlin7_pcieg3_reset_rx_lane_to_default(srds_access_t *sa__);

/** Resets Lane to Default Configuration.
 * Toggles ln_s_rstb and leaves the lane in datapath reset.
 * @param[in] sa__ is an opaque state vector passed through to device access functions.
 * @return Error Code generated by invalid access (returns ERR_CODE_NONE if no errors).
 */
err_code_t merlin7_pcieg3_reset_lane_to_default(srds_access_t *sa__);

/*--------------------------------------------*/
/*  APIs to Enable or Disable datapath reset  */
/*--------------------------------------------*/

/** Enable or Disable TX datapath reset.
 * Asserts handshake signals upon disable.
 * @param[in] sa__ is an opaque state vector passed through to device access functions.
 * @param[in] enable Enable/Disable TX datapath reset (1 = Enable; 0 = Disable).
 * @return Error Code generated by invalid access (returns ERR_CODE_NONE if no errors).
 */
err_code_t merlin7_pcieg3_tx_dp_reset(srds_access_t *sa__, uint8_t enable);

/** Enable or Disable RX datapath reset.
 * @param[in] sa__ is an opaque state vector passed through to device access functions.
 * @param[in] enable Enable/Disable RX datapath reset (1 = Enable; 0 = Disable).
 * @return Error Code generated by invalid access (returns ERR_CODE_NONE if no errors).
 */
err_code_t merlin7_pcieg3_rx_dp_reset(srds_access_t *sa__, uint8_t enable);

/** Enable or Disable Core datapath reset.
 * @param[in] sa__ is an opaque state vector passed through to device access functions.
 * @param[in] enable Enable/Disable Core datapath reset (1 = Enable; 0 = Disable).
 * @return Error Code generated by invalid access (returns ERR_CODE_NONE if no errors).
 */
err_code_t merlin7_pcieg3_core_dp_reset(srds_access_t *sa__, uint8_t enable);

/** Enable or Disable Lane datapath reset.
 * @param[in] sa__ is an opaque state vector passed through to device access functions.
 * @param[in] enable Enable/Disable Lane datapath reset (1 = Enable; 0 = Disable).
 * @return Error Code generated by invalid access (returns ERR_CODE_NONE if no errors).
 */
err_code_t merlin7_pcieg3_ln_dp_reset(srds_access_t *sa__, uint8_t enable);

/*-----------------------------------------*/
/*  APIs for setting and getting OSR mode  */
/*-----------------------------------------*/

/** Configure if the TX and RX osr mode pins should only be used to configure the TX and RX osr modes.
 * Note: Calling this API will overwrite any OSR mode value previously programmed through merlin7_pcieg3_set_osr_mode(),
 *       merlin7_pcieg3_set_tx_osr_mode(), or merlin7_pcieg3_set_rx_osr_mode().
 * @param[in] sa__ is an opaque state vector passed through to device access functions.
 * @param[in] use_pins_only Allow the use of TX and RX osr mode pins (1 = Yes; 0 = No).
 * @return Error Code generated by invalid access (returns ERR_CODE_NONE if no errors).
 */
err_code_t merlin7_pcieg3_set_use_osr_mode_pins_only(srds_access_t *sa__, uint8_t use_pins_only);

/** Get the TX and RX osr mode pins only status.
 * @param[in] sa__ is an opaque state vector passed through to device access functions.
 * @param[out] tx_use_pins_only Returns whether the TX OSR mode pins only should be used (1 = Yes; 0 = No).
 * @param[out] rx_use_pins_only Returns whether the RX OSR mode pins only should be used (1 = Yes; 0 = No).
 * @return Error Code generated by invalid access (returns ERR_CODE_NONE if no errors).
 */
err_code_t merlin7_pcieg3_get_use_osr_mode_pins_only(srds_access_t *sa__, uint8_t *tx_use_pins_only, uint8_t *rx_use_pins_only);

/** Configure if the RX osr mode pins should only be used to configure the RX osr mode.
 * Note: Calling this API will overwrite any OSR mode value previously programmed through merlin7_pcieg3_set_osr_mode()
 *        or merlin7_pcieg3_set_rx_osr_mode().
 * @param[in] sa__ is an opaque state vector passed through to device access functions.
 * @param[in] rx_use_pins_only Allow the use of RX osr mode pins (1 = Yes; 0 = No).
 * @return Error Code generated by invalid access (returns ERR_CODE_NONE if no errors).
 */
err_code_t merlin7_pcieg3_set_use_rx_osr_mode_pins_only(srds_access_t *sa__, uint8_t rx_use_pins_only);

/** Get the RX osr mode pins only status.
 * @param[in] sa__ is an opaque state vector passed through to device access functions.
 * @param[out] rx_use_pins_only Returns whether the RX OSR mode pins only should be used (1 = Yes; 0 = No).
 * @return Error Code generated by invalid access (returns ERR_CODE_NONE if no errors).
 */
err_code_t merlin7_pcieg3_get_use_rx_osr_mode_pins_only(srds_access_t *sa__, uint8_t *rx_use_pins_only);

/** Configure if the RX osr mode pins should only be used to configure the TX osr mode.
 * Note: Calling this API will overwrite any OSR mode value previously programmed through merlin7_pcieg3_set_osr_mode()
 *        or merlin7_pcieg3_set_tx_osr_mode().
 * @param[in] sa__ is an opaque state vector passed through to device access functions.
 * @param[in] tx_use_pins_only Allow the use of TX osr mode pins (1 = Yes; 0 = No).
 * @return Error Code generated by invalid access (returns ERR_CODE_NONE if no errors).
 */
err_code_t merlin7_pcieg3_set_use_tx_osr_mode_pins_only(srds_access_t *sa__, uint8_t tx_use_pins_only);

/** Get the TX osr mode pins only status.
 * @param[in] sa__ is an opaque state vector passed through to device access functions.
 * @param[out] tx_use_pins_only Returns whether the TX OSR mode pins only should be used (1 = Yes; 0 = No).
 * @return Error Code generated by invalid access (returns ERR_CODE_NONE if no errors).
 */
err_code_t merlin7_pcieg3_get_use_tx_osr_mode_pins_only(srds_access_t *sa__, uint8_t *tx_use_pins_only);

/** Set both the TX and RX osr mode.
 * Note: Calling this API will overwrite the merlin7_pcieg3_set_use_osr_mode_pins_only() setting if it was previously set.
 * Note: This API should be used for configuring TX and RX osr modes when pins aren't being used. Do not write directly to the following
 *       register fields: rx_osr_mode_frc, rx_osr_mode_frc_val, tx_osr_mode_frc, tx_osr_mode_frc_val, osr_mode_frc, and osr_mode_frc_val.
 * @param[in] sa__ is an opaque state vector passed through to device access functions.
 * @param[in] osr_mode The desired osr mode value that will be programmed.
 * @return Error Code generated by invalid access (returns ERR_CODE_NONE if no errors).
 */
err_code_t merlin7_pcieg3_set_osr_mode(srds_access_t *sa__, enum merlin7_pcieg3_osr_mode_enum osr_mode);

/** Get both the TX and RX osr mode.
 * Note: A return value of 0x3f is uninitialized.
 * Note: This API returns the control status, not the actual hardware status. Use merlin7_pcieg3_INTERNAL_get_osr_mode()
 *       if the actual hardware status is needed.
 * @param[in] sa__ is an opaque state vector passed through to device access functions.
 * @param[out] tx_osr_mode The returned TX osr mode.
 * @param[out] rx_osr_mode The returned RX osr mode.
 * @return Error Code generated by invalid access (returns ERR_CODE_NONE if no errors).
 */
err_code_t merlin7_pcieg3_get_osr_mode(srds_access_t *sa__, enum merlin7_pcieg3_osr_mode_enum *tx_osr_mode, enum merlin7_pcieg3_osr_mode_enum *rx_osr_mode);

/** Set the RX osr mode.
 * Note: Calling this API will overwrite the merlin7_pcieg3_set_use_rx_osr_mode_pins_only() setting if it was previously set.
 * Note: This API should be used for configuring TX and RX osr modes when pins aren't being used. Do not write directly to the following
 *       register fields: rx_osr_mode_frc, rx_osr_mode_frc_val, tx_osr_mode_frc, tx_osr_mode_frc_val, osr_mode_frc, and osr_mode_frc_val.
 * @param[in] sa__ is an opaque state vector passed through to device access functions.
 * @param[in] rx_osr_mode The desired RX osr mode value that will be programmed.
 * @return Error Code generated by invalid access (returns ERR_CODE_NONE if no errors).
 */
err_code_t merlin7_pcieg3_set_rx_osr_mode(srds_access_t *sa__, enum merlin7_pcieg3_osr_mode_enum rx_osr_mode);

/** Get the RX osr mode.
 * Note: A return value of 0x3f is uninitialized.
 * Note: This API returns the control status, not the actual hardware status. Use merlin7_pcieg3_INTERNAL_get_osr_mode()
 *       if the actual hardware status is needed.
 * @param[in] sa__ is an opaque state vector passed through to device access functions.
 * @param[out] rx_osr_mode The returned RX osr mode.
 * @return Error Code generated by invalid access (returns ERR_CODE_NONE if no errors).
 */
err_code_t merlin7_pcieg3_get_rx_osr_mode(srds_access_t *sa__, enum merlin7_pcieg3_osr_mode_enum *rx_osr_mode);

/** Set the TX osr mode.
 * Note: Calling this API will overwrite the merlin7_pcieg3_set_use_tx_osr_mode_pins_only() setting if it was previously set.
 * Note: This API should be used for configuring TX and RX osr modes when pins aren't being used. Do not write directly to the following
 *       register fields: rx_osr_mode_frc, rx_osr_mode_frc_val, tx_osr_mode_frc, tx_osr_mode_frc_val, osr_mode_frc, and osr_mode_frc_val.
 * @param[in] sa__ is an opaque state vector passed through to device access functions.
 * @param[in] tx_osr_mode The desired TX osr mode value that will be programmed.
 * @return Error Code generated by invalid access (returns ERR_CODE_NONE if no errors).
 */
err_code_t merlin7_pcieg3_set_tx_osr_mode(srds_access_t *sa__, enum merlin7_pcieg3_osr_mode_enum tx_osr_mode);

/** Get the TX osr mode.
 * Note: A return value of 0x3f is uninitialized.
 * Note: This API returns the control status, not the actual hardware status. Use merlin7_pcieg3_INTERNAL_get_osr_mode()
 *       if the actual hardware status is needed.
 * @param[in] sa__ is an opaque state vector passed through to device access functions.
 * @param[out] tx_osr_mode The returned TX osr mode.
 * @return Error Code generated by invalid access (returns ERR_CODE_NONE if no errors).
 */
err_code_t merlin7_pcieg3_get_tx_osr_mode(srds_access_t *sa__, enum merlin7_pcieg3_osr_mode_enum *tx_osr_mode);




/*----------------*/
/*   PMD_RX_LOCK  */
/*----------------*/

/** PMD rx lock status of current lane.
 * @param[in] sa__ is an opaque state vector passed through to device access functions.
 * @param[out] *pmd_rx_lock PMD_RX_LOCK status of current lane returned by API.
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors).
 */
err_code_t merlin7_pcieg3_pmd_lock_status(srds_access_t *sa__, uint8_t *pmd_rx_lock);

/*--------------------------------*/
/*  Serdes TX disable/RX Restart  */
/*--------------------------------*/
/** TX Disable.
 * @param[in] sa__ is an opaque state vector passed through to device access functions.
 * @param[in] enable Enable/Disable TX disable (1 = TX Disable asserted; 0 = TX Disable removed).
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors).
 */
err_code_t merlin7_pcieg3_tx_disable(srds_access_t *sa__, uint8_t enable);




/*-----------------------------*/
/*  Stop/Resume RX Adaptation  */
/*-----------------------------*/
/** Stop RX Adaptation on a Lane. Control is returned only after attempting to stop adaptation.
 * RX Adaptation needs to be stopped before modifying any of the VGA, PF or DFE taps.
 * @param[in] sa__ is an opaque state vector passed through to device access functions.
 * @param[in] enable Enable RX Adaptation stop (1 = Stop RX Adaptation on lane; 0 = Resume RX Adaptation on lane)
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors)
 */
err_code_t merlin7_pcieg3_stop_rx_adaptation(srds_access_t *sa__, uint8_t enable);

/** Request to stop RX Adaptation on a Lane.
 * Control will be returned immediately before adaptaion is completely stopped.
 * RX Adaptation needs to be stopped before modifying any of the VGA, PF or DFE taps.
 * To resume RX adaptation, use the merlin7_pcieg3_stop_rx_adaptation() API.
 * @param[in] sa__ is an opaque state vector passed through to device access functions.
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors)
 */
err_code_t merlin7_pcieg3_request_stop_rx_adaptation(srds_access_t *sa__);

/*------------------------------------*/
/*  Read/Write all RX AFE parameters  */
/*------------------------------------*/

/** Write to RX AFE settings.
 * Note: RX Adaptation needs to be stopped before modifying any of the VGA, PF or DFE taps.
 * @param[in] sa__ is an opaque state vector passed through to device access functions.
 * @param[in] param Enum (#srds_rx_afe_settings_enum) to select the required RX AFE setting to be modified
 * @param[in] val Value to be written to the selected AFE setting
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors)
 */
err_code_t merlin7_pcieg3_write_rx_afe(srds_access_t *sa__, enum srds_rx_afe_settings_enum param, int8_t val);

/** Read from RX AFE settings.
 * @param[in] sa__ is an opaque state vector passed through to device access functions.
 * @param[in] param Enum (#srds_rx_afe_settings_enum) to select the required RX AFE setting to be read.
 * @param[out] val RX AFE value returned from API.
 * @return Error Code generated by API (returns ERR_CODE_NONE if no errors).
 */
err_code_t merlin7_pcieg3_read_rx_afe(srds_access_t *sa__, enum srds_rx_afe_settings_enum param, int8_t *val);

/*-----------------------------*/
/*  TX_PI Fixed Frequency Mode */
/*-----------------------------*/

/** TX_PI Fixed Frequency Mode.
 * @param[in] sa__ is an opaque state vector passed through to device access functions.
 * @param[in] enable Enable/Disable TX_PI (1 = Enable; 0 = Disable)
 * @param[in] freq_override_val Fixed Frequency Override value (freq_override_val = desired_ppm*8192/781.25; Range: -8192 to + 8192);
 * @return Error Code generated by invalid TX_PI settings (returns ERR_CODE_NONE if no errors)
 */
err_code_t merlin7_pcieg3_tx_pi_freq_override(srds_access_t *sa__, uint8_t enable, int16_t freq_override_val);

/** This function allows user to align TX clocks in selected lanes.
 * The current lane is expected to be the master lane.
 * @param[in] sa__ is an opaque state vector passed through to device access functions.
 * @param[in] enable - 0: enable tx_clock_align for selected lanes with current lane as master;
 *                 1: disable tx_clock_align for selected lanes including master lane.
 * @param[in] lane_mask: bit mask for selecting lanes (0x00001111 - lane 0-3 are selected).
 * NOTE: The current lane should be selected as part of the lane_mask; else API errors out.
 * @return Error Code, if generated (returns ERR_CODE_NONE if no errors)
 */
err_code_t merlin7_pcieg3_tx_clock_align_enable(srds_access_t *sa__, uint8_t enable, uint8_t lane_mask);

/*--------------------------------------------*/
/*  Loopback and Ultra-Low Latency Functions  */
/*--------------------------------------------*/


/*! @} SerdesAPIConfigTag */
/*! @} APITag */
#endif
#ifdef __cplusplus
}
#endif
