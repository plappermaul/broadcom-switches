/*! \file bcm56780_a0_dna_2_5_13_feature.c
 *
 * BCM56780_A0 DNA_2_5_13 feature driver.
 */
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

#include <bcm_int/ltsw/mbcm/feature.h>
#include <bcm_int/ltsw/feature.h>
#include <bcm_int/ltsw/chip/bcm56780_a0/feature.h>

#include <shr/shr_debug.h>

/******************************************************************************
 * Local definitions
 */

#define BSL_LOG_MODULE BSL_LS_BCM_COMMON

/******************************************************************************
 * Private functions
 */

/*!
 * \brief Initialize features to be supported.
 *
 * \param [in] unit Unit Number.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
static int
bcm56780_a0_dna_2_5_13_ltsw_feature_init(int unit)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (bcm56780_a0_ltsw_feature_init(unit));

    ltsw_feature_enable(unit, LTSW_FT_MPLS);
    ltsw_feature_enable(unit, LTSW_FT_MPLS_VLAN_DOMAIN);
    ltsw_feature_enable(unit, LTSW_FT_MPLS_SPECIAL_LABEL);
    ltsw_feature_enable(unit, LTSW_FT_MPLS_CONTROL_PKT);
    ltsw_feature_enable(unit, LTSW_FT_DEST_SYSPORT_TO_UL_NH);

exit:
    SHR_FUNC_EXIT();
}

/*!
 * \brief Feature driver function variable for bcm56780_a0 DNA_2_5_13 device.
 */
static mbcm_ltsw_feature_drv_t bcm56780_a0_dna_2_5_13_ltsw_feature_drv = {
    .feature_init = bcm56780_a0_dna_2_5_13_ltsw_feature_init
};

/******************************************************************************
 * Public functions
 */

int
bcm56780_a0_dna_2_5_13_ltsw_feature_drv_attach(int unit)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_feature_drv_set(unit, &bcm56780_a0_dna_2_5_13_ltsw_feature_drv));

exit:
    SHR_FUNC_EXIT();
}

