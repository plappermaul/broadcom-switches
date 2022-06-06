/******************************************************************************
******************************************************************************
*  Revision      :  $Id: falcon_tsc_enum.h 1261 2015-11-19 15:51:37Z eroes $ *
*                                                                            *
*  Description   :  Enum types used by Serdes API functions                  *
*                                                                            *
* $Copyright: (c) 2016 Broadcom.
* Broadcom Proprietary and Confidential. All rights reserved.$                                                      *
*  No portions of this material may be reproduced in any form without        *
*  the written permission of:                                                *
*      Broadcom Corporation                                                  *
*      5300 California Avenue                                                *
*      Irvine, CA  92617                                                     *
*                                                                            *
*  All information contained in this document is Broadcom Corporation        *
*  company private proprietary, and trade secret.                            *
*                                                                            *
******************************************************************************
******************************************************************************/

/** @file falcon_tsc_enum.h
 * Enum types used by Serdes API functions
 */

#ifndef FALCON_TSC_API_ENUM_H
#define FALCON_TSC_API_ENUM_H

#include "common/srds_api_enum.h"





/** Falcon TSC PLL Config Enum */
enum falcon_tsc_pll_enum {
	FALCON_TSC_pll_div_128x,
	FALCON_TSC_pll_div_132x,
	FALCON_TSC_pll_div_140x,
	FALCON_TSC_pll_div_160x,
	FALCON_TSC_pll_div_165x,
	FALCON_TSC_pll_div_168x,
	FALCON_TSC_pll_div_175x,
	FALCON_TSC_pll_div_180x,
	FALCON_TSC_pll_div_184x,
	FALCON_TSC_pll_div_200x,
	FALCON_TSC_pll_div_224x,
	FALCON_TSC_pll_div_264x
};















#endif
