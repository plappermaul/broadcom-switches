/***********************************************************************************
 *                                                                                 *
 * Copyright: (c) 2019 Broadcom.                                                   *
 * Broadcom Proprietary and Confidential. All rights reserved.                     *
 *                                                                                 *
 ***********************************************************************************/

/**************************************************************************************
 **************************************************************************************
 *                                                                                    *
 *  Revision      :   *
 *                                                                                    *
 *  Description   :  API Dependencies to be provided by IP user                       *
 *                                                                                    *
 **************************************************************************************
 **************************************************************************************/

/** @file falcon16_tsc_dependencies.h
 * Dependencies to be provided by IP User
 */

#ifndef FALCON16_TSC_API_DEPENDENCIES_H
#define FALCON16_TSC_API_DEPENDENCIES_H

#include "common/srds_api_err_code.h"
#include "common/srds_api_types.h"

/** Read a register from the currently selected Serdes IP Lane.
 * @param sa__ is an opaque state vector passed through to device access functions.
 * @param address Address of register to be read
 * @param *val value read out from the register
 * @return Error code generated by read function (returns ERR_CODE_NONE if no errors)
 */
err_code_t falcon16_tsc_pmd_rdt_reg(srds_access_t *sa__, uint16_t address, uint16_t *val);

/** Write to a register from the currently selected Serdes IP Lane.
 * @param sa__ is an opaque state vector passed through to device access functions.
 * @param address Address of register to be written
 * @param val Value to be written to the register
 * @return Error code generated by write function (returns ERR_CODE_NONE if no errors)
 */
err_code_t falcon16_tsc_pmd_wr_reg(srds_access_t *sa__, uint16_t address, uint16_t val);

/** Masked Register Write to the currently selected Serdes IP core/lane.
 * If using Serdes MDIO controller to access the registers, implement this function using falcon16_tsc_pmd_mdio_mwr_reg(..)
 *
 * If NOT using a Serdes MDIO controller or the Serdes PMI Masked write feature, please use the following code to
 * implement this function
 *
 *    falcon16_tsc_pmd_wr_reg(addr, ((falcon16_tsc_pmd_rd_reg(addr) & ~mask) | (mask & (val << lsb))));
 *
 * @param sa__ is an opaque state vector passed through to device access functions.
 * @param addr Address of register to be written
 * @param mask 16-bit mask indicating the position of the field with bits of 1s
 * @param lsb  LSB of the field
 * @param val  16bit value to be written
 * @return Error code generated by write function (returns ERR_CODE_NONE if no errors)
 */
err_code_t falcon16_tsc_pmd_mwr_reg(srds_access_t *sa__, uint16_t addr, uint16_t mask, uint8_t lsb, uint16_t val);

/** Write to a PRAM location for the currently selected Serdes IP Core.
 *  The address is auto-incrementing, per the PRAM interface specification.
 * @param sa__ is an opaque state vector passed through to device access functions.
 * @param val Value to be written
 * @return Error code generated by write function (returns ERR_CODE_NONE if no errors)
 */
err_code_t falcon16_tsc_pmd_wr_pram(srds_access_t *sa__, uint8_t val);

/** Write message to the logger with the designated verbose level.
 * Output is sent to stdout and a logfile
 * @param message_verbose_level   Verbose level for the current message
 * @param *format Format string as in printf
 * @param ... Additional variables used as in printf
 * @return Error code generated by function (returns ERR_CODE_NONE if no errors)
 */
#ifdef SRDS_API_ALL_FUNCTIONS_HAVE_ACCESS_STRUCT
int logger_write(srds_access_t *sa__, int message_verbose_level, const char *format, ...);
#else
int logger_write(int message_verbose_level, const char *format, ...);
#endif

/** Delay the execution of the code for atleast specified amount of time in nanoseconds.
 * This function is used ONLY for delays less than 1 microsecond, non-zero error code may be returned otherwise.
 * The user can implement this as an empty function if their register access latency exceeds 1 microsecond.
 * @param delay_ns Delay in nanoseconds
 * @return Error code generated by delay function (returns ERR_CODE_NONE if no errors)
 */
#ifdef SRDS_API_ALL_FUNCTIONS_HAVE_ACCESS_STRUCT
err_code_t falcon16_tsc_delay_ns(srds_access_t *sa__, uint16_t delay_ns);
#else
err_code_t falcon16_tsc_delay_ns(uint16_t delay_ns);
#endif

/** Delay the execution of the code for atleast specified amount of time in microseconds.
 * For longer delays, accuracy is required. When requested delay is > 100ms, the implemented delay is assumed
 * to be < 10% bigger than requested.
 * This function is used ONLY for delays greater than or equal to 1 microsecond.
 * @param delay_us Delay in microseconds
 * @return Error code generated by delay function (returns ERR_CODE_NONE if no errors)
 */
#ifdef SRDS_API_ALL_FUNCTIONS_HAVE_ACCESS_STRUCT
err_code_t falcon16_tsc_delay_us(srds_access_t *sa__, uint32_t delay_us);
#else
err_code_t falcon16_tsc_delay_us(uint32_t delay_us);
#endif

/** Delay the execution of the code for atleast specified amount of time in milliseconds.
 * For longer delays, accuracy is required. When requested delay is > 100ms, the implemented delay is assumed
 * to be < 10% bigger than requested.
 * This function is used ONLY for delays greater than or equal to 1 millisecond.
 * @param delay_ms Delay in milliseconds
 * @return Error code generated by delay function (returns ERR_CODE_NONE if no errors)
 */
#ifdef SRDS_API_ALL_FUNCTIONS_HAVE_ACCESS_STRUCT
err_code_t falcon16_tsc_delay_ms(srds_access_t *sa__, uint32_t delay_ms);
#else
err_code_t falcon16_tsc_delay_ms(uint32_t delay_ms);
#endif

/** Return the address of current selected Serdes IP Core.
 * @param sa__ is an opaque state vector passed through to device access functions.
 * @return the IP level address of the current core.
 */
uint8_t falcon16_tsc_get_core(srds_access_t *sa__);

/** Return the logical address of current selected Serdes IP lane.
 * @param sa__ is an opaque state vector passed through to device access functions.
 * @return the IP level address of the current lane. 0 to N-1, for an N lane IP
 */
uint8_t falcon16_tsc_get_lane(srds_access_t *sa__);

#define falcon16_tsc_get_physical_lane falcon16_tsc_get_lane

/** Set the logical address of current selected Serdes IP lane.  Used in diagnostic
 * and core-level management functions.
 * @param sa__ is an opaque state vector passed through to device access functions.
 * @param lane_index is the lane index
 * @return Any error code generated during execution; ERR_CODE NONE otherwise.
 */
err_code_t falcon16_tsc_set_lane(srds_access_t *sa__, uint8_t lane_index);

#define falcon16_tsc_set_physical_lane falcon16_tsc_set_lane



#if defined(SERDES_EXTERNAL_INFO_TABLE_EN)
/** Returns address of falcon16_tsc info table stored in upper level software.
 * @param sa__ is an opaque state vector passed through to device access functions.
 */
srds_info_t *falcon16_tsc_get_info_table_address(srds_access_t *sa__);
#endif

#endif