/**********************************************************************************
 **********************************************************************************
 *                                                                                *
 *  Revision    :   *
 *                                                                                *
 *  Description :  Internal API error functions                                   *
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

/** @file merlin16_internal_error.h
 * Internal API error functions
 */

#ifndef MERLIN16_API_INTERNAL_ERROR_H
#define MERLIN16_API_INTERNAL_ERROR_H

#include "common/srds_api_err_code.h"


/**
 * Error-trapping macro.
 *
 * In other then SerDes-team post-silicon evaluation builds, simply yields
 * the error code supplied as an argument, without further action.
 */
#define _error(err_code) merlin16_INTERNAL_print_err_msg(err_code)

/**@}*/

/** Print Error messages to screen before returning.
 * @param err_code Error Code input which is returned as well
 * @return Error Code
 */
err_code_t merlin16_INTERNAL_print_err_msg(uint16_t err_code);

/** Print Convert Error code to String.
 * @param err_code Error Code input which is converted to string
 * @return String containing Error code information.
 */
char* merlin16_INTERNAL_e2s_err_code(err_code_t err_code);

#endif
