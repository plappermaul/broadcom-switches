/*******************************************************************************
 *
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated by xfcr from NPL sources files.
 *
 * Tool: $SDK/INTERNAL/fltg/bin/xfcr
 *
 * Edits to this file will be lost when it is regenerated.
 *
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

#include <shr/shr_error.h>
#include <bcmcth/bcmcth_mirror_drv.h>
#include "bcm56780_a0_dna_2_4_13_cth_mirror_data.h"

int
bcm56780_a0_dna_2_4_13_cth_mirror_data_attach(bcmcth_mirror_drv_t *drv)
{
    drv->data = &bcm56780_a0_dna_2_4_13_cth_mirror_data;

    return SHR_E_NONE;
}

