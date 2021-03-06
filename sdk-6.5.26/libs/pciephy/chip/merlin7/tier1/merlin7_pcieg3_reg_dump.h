/*************************************************************************************
 *                                                                                   *
 * Copyright: (c) 2021 Broadcom.                                                     *
 * Broadcom Proprietary and Confidential. All rights reserved.                       *
 *                                                                                   *
 *************************************************************************************/

/*************************************************************************************
 *************************************************************************************
 *                                                                                   *
 *  Revision      :  Id                                                              *
 *                                                                                   *
 *  Description   :  Defines and externed array used for merlin7_pcieg3_reg_dump()           *
 *                                                                                   *
 *************************************************************************************
 *************************************************************************************/


/* THIS FILE IS GENERATED USING AN AUTOMATED SCRIPT. PLEASE DO NOT EDIT THIS FILE DIRECTLY! */

#ifndef MERLIN7_PCIEG3_API_REG_DUMP_H
#define MERLIN7_PCIEG3_API_REG_DUMP_H

#include "merlin7_pcieg3_usr_includes.h"

/*! @file
 *  @brief Register addresses and types used for register dump APIs
 */

/*! @addtogroup APITag
 * @{
 */

/*! @defgroup SerdesAPIRegDumpTag Support for Register Dump APIs  */

/*! @addtogroup SerdesAPIRegDumpTag
 * @{
 */


/*! @def MERLIN7_PCIEG3_REG_DUMP_SECTIONS
 * The number of contiguous register address ranges that will have their contents dumped.
 */
#define MERLIN7_PCIEG3_REG_DUMP_SECTIONS  (42)

/*! @def MERLIN7_PCIEG3_REG_PLL_START
 * The register address of the first PLL register.
 * A value of 0xffff indicates there are no PLL registers for this IP.
 */
#define MERLIN7_PCIEG3_REG_PLL_START      (0xd0b0)

/*! @def MERLIN7_PCIEG3_REG_UC_START
 * The register address of the first UC register.
 * A value of 0xffff indicates there are no UC registers for this IP.
 */
#define MERLIN7_PCIEG3_REG_UC_START       (0xffff)

/*! @def MERLIN7_PCIEG3_REG_CORE_START_INDEX
 * The merlin7_pcieg3_reg_dump_arr index of the first Core register.
 */
#define MERLIN7_PCIEG3_REG_CORE_START_INDEX      (31)    /* 0xd0f0 */

/*! @def MERLIN7_PCIEG3_REG_PLL_START_INDEX
 * The merlin7_pcieg3_reg_dump_arr index of the first PLL register.
  */
#define MERLIN7_PCIEG3_REG_PLL_START_INDEX      (40)    /* 0xd0b0 */

/*! @def MERLIN7_PCIEG3_REG_UC_START_INDEX
 * The merlin7_pcieg3_reg_dump_arr index of the first UC register.
 */
#define MERLIN7_PCIEG3_REG_UC_START_INDEX       (42)    /* 0xffff */

/*! merlin7_pcieg3_reg_dump_arr_t
 *
 */
typedef const uint16_t merlin7_pcieg3_reg_dump_arr_t[2];

/** Get merlin7_pcieg3_reg_dump_arr
 * @return Pointer to merlin7_pcieg3_reg_dump_arr, which is a 2-element array of const uint16_t.
 *         Element 1 = base address, Element 2 = mask of valid addresses from base address.
 */
merlin7_pcieg3_reg_dump_arr_t *merlin7_pcieg3_get_reg_dump_arr(void);

/*! @} SerdesAPIRegDumpTag */
/*! @} APITag */

#endif /* SERDES_API_REG_DUMP_H */

