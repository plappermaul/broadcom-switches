/*! \file variant_list.h
 *
 * BCM56880_A0_LTSW_VARIANT_LIST_ENTRY macros for the variant dispatch.
 */
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

#ifdef BCM56880_A0_LTSW_VARIANT_LIST_ENTRY
/*
 * BCM56880_A0_LTSW_VARIANT_LIST_ENTRY macros.
 *
 * Before including this file, define BCM56880_A0_LTSW_VARIANT_LIST_ENTRY
 * as a macro to operate on the following parameters:
 *
 *     #define BCM56880_A0_LTSW_VARIANT_LIST_ENTRY(_va,_ve,_vd,_r0)
 *
 *     _va: Variant name (lower case)
 *     _ve: Variant enum symbol (upper case)
 *     _vd: Variant Description
 *     _r0: Reserved
 *
 * BCM56880_A0_LTSW_VARIANT_LIST_ENTRY is undefined at the end of this file.
 */

BCM56880_A0_LTSW_VARIANT_LIST_ENTRY(dna_4_6_6,DNA_4_6_6,NULL,0)


#undef BCM56880_A0_LTSW_VARIANT_LIST_ENTRY
#endif /* BCM56880_A0_LTSW_VARIANT_LIST_ENTRY */

