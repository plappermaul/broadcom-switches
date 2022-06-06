/**************************************************************************************
 **************************************************************************************
 *                                                                                    *
 *  Revision      :  $Id: falcon_furia_dependencies.h 947 2015-03-06 21:11:44Z kirand $ *
 *                                                                                    *
 *  Description   :  API Dependencies to be provided by IP user                       *
 *                                                                                    *
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2021 Broadcom Inc. All rights reserved.                                                              *
 *  No portions of this material may be reproduced in any form without                *
 *  the written permission of:                                                        *
 *      Broadcom Corporation                                                          *
 *      5300 California Avenue                                                        *
 *      Irvine, CA  92617                                                             *
 *                                                                                    *
 *  All information contained in this document is Broadcom Corporation                *
 *  company private proprietary, and trade secret.                                    *
 *                                                                                    *
 **************************************************************************************
 **************************************************************************************/

/** @file falcon_furia_dependencies.h
 * Dependencies to be provided by IP User
 */

#ifndef FALCON_FURIA_API_DEPENDENCIES_H
#define FALCON_FURIA_API_DEPENDENCIES_H

#include "falcon_furia_usr_includes.h"
#include "../common/srds_api_err_code.h"
#include <phymod/phymod.h>

/** Read a register from the currently selected Serdes IP Lane.
 * @param address Address of register to be read
 * @param *val value read out from the register
 * @return Error code generated by read function (returns ERR_CODE_NONE if no errors)
 */
err_code_t falcon_furia_pmd_rdt_reg(const phymod_access_t *pa,uint16_t address, uint16_t *val);

/** Write to a register from the currently selected Serdes IP Lane.
 * @param address Address of register to be written
 * @param val Value to be written to the register
 * @return Error code generated by write function (returns ERR_CODE_NONE if no errors)
 */
err_code_t falcon_furia_pmd_wr_reg(const phymod_access_t *pa,uint16_t address, uint16_t val);

/** Masked Register Write to the currently selected Serdes IP core/lane.
 * If using Serdes MDIO controller to access the registers, implement this function using falcon_furia_pmd_mdio_mwr_reg(..)
 *
 * If NOT using a Serdes MDIO controller or the Serdes PMI Masked write feature, please use the following code to
 * implement this function
 *
 *    falcon_furia_pmd_wr_reg(const phymod_access_t *pa,addr, ((falcon_furia_pmd_rd_reg(addr) & ~mask) | (mask & (val << lsb))));
 *
 * @param addr Address of register to be written
 * @param mask 16-bit mask indicating the position of the field with bits of 1s
 * @param lsb  LSB of the field
 * @param val  16bit value to be written
 * @return Error code generated by write function (returns ERR_CODE_NONE if no errors)
 */
err_code_t falcon_furia_pmd_mwr_reg(const phymod_access_t *pa,uint16_t addr, uint16_t mask, uint8_t lsb, uint16_t val);

/** Write message to the logger with the designated verbose level.
 * Output is sent to stdout and a logfile
 * @param message_verbose_level   Verbose level for the current message
 * @param *format Format string as in printf
 * @param ... Additional variables used as in printf
 * @return Error code generated by function (returns ERR_CODE_NONE if no errors)
 */
int furia_logger_write(int message_verbose_level, const char *format, ...);

/** Delay the execution of the code for atleast specified amount of time in nanoseconds.
 * This function is used ONLY for delays less than 1 microsecond, non-zero error code may be returned otherwise.
 * The user can implement this as an empty function if their register access latency exceeds 1 microsecond.
 * @param delay_ns Delay in nanoseconds
 * @return Error code generated by delay function (returns ERR_CODE_NONE if no errors)
 */
err_code_t falcon_furia_delay_ns(uint16_t delay_ns);

/** Delay the execution of the code for atleast specified amount of time in microseconds.
 * For longer delays, accuracy is required. When requested delay is > 100ms, the implemented delay is assumed
 * to be < 10% bigger than requested.
 * This function is used ONLY for delays greater than or equal to 1 microsecond.
 * @param delay_us Delay in microseconds
 * @return Error code generated by delay function (returns ERR_CODE_NONE if no errors)
 */
err_code_t falcon_furia_delay_us(uint32_t delay_us);

/** Delay the execution of the code for atleast specified amount of time in milliseconds.
 * For longer delays, accuracy is required. When requested delay is > 100ms, the implemented delay is assumed
 * to be < 10% bigger than requested.
 * This function is used ONLY for delays greater than or equal to 1 millisecond.
 * @param delay_ms Delay in milliseconds
 * @return Error code generated by delay function (returns ERR_CODE_NONE if no errors)
 */
err_code_t falcon_furia_delay_ms(uint32_t delay_ms);

/** Return the address of current selected Serdes IP Core.
 * @return the IP level address of the current core.
 */
uint8_t falcon_furia_get_core(const phymod_access_t *pa);

/** Return the address of current selected Serdes IP lane.
 * @return the IP level address of the current lane. 0 to N-1, for an N lane IP
 */
uint8_t falcon_furia_get_lane(const phymod_access_t *pa);


/** Convert uC lane index.
* Convert uC lane index to system ID string.
* @param *string a 256-byte output buffer to receive system ID
* @param uc_lane_idx uC lane index
* @return Error Code generated by API (returns ERR_CODE_NONE if no errors)
* The textual identifier is pre-filled with a default:  implementors may
* safely return without modifying it if the default text is sufficient.
*/
err_code_t falcon_furia_uc_lane_idx_to_system_id(const phymod_access_t *pa, const char *string , uint8_t uc_lane_idx);

/*-----------------------------------*/
/*  Serdes uC Lane Info Structure    */
/*-----------------------------------*/

/** uC Lane Info Struct */
typedef struct {
    /** Direct RAM access available */
    uint8_t   is_direct_ram_access_avail;
    /** Core variable base address */
    uint16_t  core_var_base_address;
    /** Trace memory base address */
    uint16_t  trace_mem_base_address;
    /** Lane variable base address */
    uint16_t  lane_var_base_address;
    /** Diagnostic base address [tracemem_size/num_uc_lanes*uc_lane_idx] */
    uint16_t  diag_base_address;
    /** Micro Lane index */
    uint8_t   uc_lane_idx;
    /** RAM size available to each lane */
    uint16_t  lane_ram_size;
    /** Maximum index into diag lane memory (tracemem_size/num_uc_lanes - 1) */
    uint16_t  diag_max_idx;
    /** Trace Memory size */
    uint16_t  trace_mem_size;
    /** Number of Lanes */
    uint8_t   num_lanes;
} falcon_furia_uc_lane_info_st;

/** Retrieve Lane Based Information.
* Retrieve current lane memory address information.
* @param *ext_uc_lane_info an output pointer pointed to uC lane info struct
* @return Error Code generated by API (returns ERR_CODE_NONE if no errors)
*/
err_code_t falcon_furia_get_uc_ln_info(const phymod_access_t *pa, falcon_furia_uc_lane_info_st *ext_uc_lane_info);

/*---------------------------------------------*/
/*  Serdes IP RAM access - Lane RAM Variables  */
/*---------------------------------------------*/
/*          rd - read; wr - write              */
/*          b  - byte; w  - word               */
/*---------------------------------------------*/
/** Unsigned Word Read of a uC RAM variable.
* Write access through Micro Register Interface for PMD IPs. Output is in little-endian format (LSB is the lower address).
* @param *rd_val 16bit unsigned value to be read from RAM variable
* @param addr Address of RAM variable to be read
* @param size Size of RAM in word to be read
* @return Error Code generated by API (returns ERR_CODE_NONE if no errors)
*/
err_code_t falcon_furia_rdw_ram(const phymod_access_t *pa, uint16_t *rd_val, uint16_t addr, uint16_t size);

/** Unsigned Byte Read of a uC RAM variable.
* Write access through Micro Register Interface for PMD IPs.
* @param *rd_val 8bit unsigned value to be read from RAM variable
* @param addr Address of RAM variable to be read
* @param size Size of RAM in byte to be read
* @return Error Code generated by API (returns ERR_CODE_NONE if no errors)
*/
err_code_t falcon_furia_rdb_ram(const phymod_access_t *pa, uint8_t *rd_val, uint16_t addr, uint16_t size);

/** Unsigned Byte Write of a uC RAM variable.
* Write access through Micro Register Interface for PMD IPs.
* @param addr Address of RAM variable to be written
* @param size Size of RAM in byte to be written
* @param *wr_val 8bit unsigned value to be written to RAM variable
* @return Error Code generated by API (returns ERR_CODE_NONE if no errors)
*/
err_code_t falcon_furia_wrb_ram(const phymod_access_t *pa, uint16_t addr, uint16_t size, uint8_t *wr_val);

/** Unsigned Word Write of a uC RAM variable.
* Write access through Micro Register Interface for PMD IPs.
* @param addr Address of RAM variable to be written
* @param size Size of RAM in word to be written
* @param *wr_val 16bit unsigned value to be written to RAM variable
* @return Error Code generated by API (returns ERR_CODE_NONE if no errors)
*/
err_code_t falcon_furia_wrw_ram(const phymod_access_t *pa, uint16_t addr, uint16_t size, uint16_t *wr_val);

#endif