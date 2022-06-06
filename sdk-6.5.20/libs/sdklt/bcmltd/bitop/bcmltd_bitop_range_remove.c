/*! \file bcmltd_bitop_range_remove.c
 *
 * Bit array operations.
 */
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

#include <bcmltd/bcmltd_bitop.h>

/*!
 * \brief Clear select bits in a bit array.
 *
 * INTERNAL USE ONLY.
 *
 * See \ref BCMLTD_BITREMOVE_RANGE for details.
 */
void
bcmltd_bitop_range_remove(const BCMLTD_BITDCL *a1,
        const BCMLTD_BITDCL *a2,
        const int offs, int n,
        BCMLTD_BITDCL *dest)
{
    if (n > 0) {
        BCMLTDi_BITOP_RANGE(a1, a2, offs, n, dest, & ~);
    }
}
