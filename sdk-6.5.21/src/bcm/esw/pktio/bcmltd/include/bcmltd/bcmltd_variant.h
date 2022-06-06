/*! \file bcmltd_variant.h
 *
 * \brief BCMLTD Variant initerfaces and definitions
 *
 * Logical table variant inclusion and exclusion support within the
 * BCMLTD can be specified as a combination of the following defines:
 *
 *     #define BCMLTD_CONFIG_INCLUDE_<CHIPNAME>_<REV>X_<VARIANT> [1|0]
 *           -- Include or exclude the specified variant
 *           Example: #define BCMLTD_CONFIG_INCLUDE_BCM0213_EDNA    1
 *
 * The value of BCMLTD_CONFIG_INCLUDE_VARIANT_DEFAULT is used for any
 * variants which are left unspecified. Set this value to 1 or 0 to
 * include or exclude all variants by default.
 *
 * BCMLTD_VARIANT_ENTRY macros.
 *
 * If a list of variant entries is needed, before including this file,
 * define BCMLTD_VARIANT_ENTRY as a macro to operate on the following
 * parameters:
 *
 *     #define BCMLTD_VARIANT_ENTRY(_bd,_bu,_va,_ve,_vu,_vv,_vo,_vd,_r0,_r1)
 *
 *     _bd: SW Base Driver (lower case)
 *     _bu: SW Base Driver (upper case)
 *     _va: Variant name (lower case or empty for BASE)
 *     _ve: Variant enum symbol (upper case)
 *     _vu: Variant name (underscore or empty for BASE)
 *     _vv: Variant numeric value
 *     _vo: Device relative offset
 *     _vd: Variant Description
 *     _r0: Reserved
 *     _r1: Reserved
 *
 * Note that BCMLTD_VARIANT_ENTRY will be undefined at the end of this file.
 */
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

#ifndef BCMLTD_VARIANT_H
#define BCMLTD_VARIANT_H

#include <bcmltd/chip/bcmltd_variant_defs.h>

#endif /* BCMLTD_VARIANT_H */

/* This include must be placed outside the include guard. */
#include <bcmltd/chip/bcmltd_variant.h>

