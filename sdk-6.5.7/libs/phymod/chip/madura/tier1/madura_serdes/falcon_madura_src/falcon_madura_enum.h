/******************************************************************************
 ******************************************************************************
 *  Revision      :  $Id: falcon_furia_enum.h 692 2014-09-09 20:47:56Z kirand $ *
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

/** @file falcon_furia_enum.h
 * Enum types used by Serdes API functions
 */

#ifndef FALCON_FURIA_API_ENUM_H
#define FALCON_FURIA_API_ENUM_H

#include "../common/srds_api_enum.h"







/** Falcon Furia PLL Config Enum */
enum falcon_furia_madura_pll_enum {
  FALCON_FURIA_pll_div_80x,
  FALCON_FURIA_pll_div_96x,
  FALCON_FURIA_pll_div_120x,
  FALCON_FURIA_pll_div_128x,
  FALCON_FURIA_pll_div_132x,
  FALCON_FURIA_pll_div_140x,
  FALCON_FURIA_pll_div_144x,
  FALCON_FURIA_pll_div_160x,
  FALCON_FURIA_pll_div_160x_vco2,
  FALCON_FURIA_pll_div_160x_refc174,
  FALCON_FURIA_pll_div_165x,
  FALCON_FURIA_pll_div_168x,
  FALCON_FURIA_pll_div_175x,
  FALCON_FURIA_pll_div_180x,
  FALCON_FURIA_pll_div_184x,
  FALCON_FURIA_pll_div_198x,
  FALCON_FURIA_pll_div_200x,
  FALCON_FURIA_pll_div_224x,
  FALCON_FURIA_pll_div_264x
};

#endif
