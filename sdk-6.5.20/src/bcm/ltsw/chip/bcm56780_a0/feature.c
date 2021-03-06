/*
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated.
 * Edits to this file will be lost when it is regenerated.
 *
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

#include <bcm_int/ltsw/mbcm/feature.h>
#include <bcm_int/ltsw/feature.h>
#include <bcm_int/ltsw/chip/bcm56780_a0/variant_dispatch.h>
#include <bcm_int/ltsw/chip/bcm56780_a0/bcmgene.h>

#include <shr/shr_debug.h>

/******************************************************************************
 * Local definitions
 */

#define BSL_LOG_MODULE BSL_LS_BCM_COMMON

/******************************************************************************
 * Private functions
 */

int
bcm56780_a0_ltsw_feature_init(int unit)
{

    SHR_FUNC_ENTER(unit);

    ltsw_feature_enable(unit, LTSW_FT_L3);
    ltsw_feature_enable(unit, LTSW_FT_L3_FIB);
    ltsw_feature_enable(unit, LTSW_FT_RATE);
    ltsw_feature_enable(unit, LTSW_FT_FLEX_FLOW);
    ltsw_feature_enable(unit, LTSW_FT_FLEX_FLOW_VLAN_DOMAIN);
    ltsw_feature_enable(unit, LTSW_FT_FLEXCTR_GEN2);
    ltsw_feature_enable(unit, LTSW_FT_URPF);
    ltsw_feature_enable(unit, LTSW_FT_DLB);
    ltsw_feature_enable(unit, LTSW_FT_DLB_DGM);
    ltsw_feature_enable(unit, LTSW_FT_IPMC);
    ltsw_feature_enable(unit, LTSW_FT_PIM_BIDIR);
    ltsw_feature_enable(unit, LTSW_FT_OOB_FC);
    ltsw_feature_enable(unit, LTSW_FT_SBR);
    ltsw_feature_enable(unit, LTSW_FT_STK);
    ltsw_feature_enable(unit, LTSW_FT_TIME);
    ltsw_feature_enable(unit, LTSW_FT_TELEMETRY);
    ltsw_feature_enable(unit, LTSW_FT_VPLAG);
    ltsw_feature_enable(unit, LTSW_FT_TRACE_DOP);
    ltsw_feature_enable(unit, LTSW_FT_FLEXSTATE);
    ltsw_feature_enable(unit, LTSW_FT_TNL_SEQ_NUM_PRF);
#if defined(INCLUDE_XFLOW_MACSEC)
    ltsw_feature_enable(unit, LTSW_FT_XFLOW_MACSEC);
#endif
    ltsw_feature_enable(unit, LTSW_FT_FLEXCTR_QUANT_32BIT);
    ltsw_feature_enable(unit, LTSW_FT_IFA);
    ltsw_feature_enable(unit, LTSW_FT_FLEXCTR_EVICTION);
    ltsw_feature_enable(unit, LTSW_FT_FLEXCTR_IPMC_HITBIT);

    SHR_IF_ERR_VERBOSE_EXIT
        (bcm56780_a0_gene_feature_init(unit));
exit:
    SHR_FUNC_EXIT();
}

/******************************************************************************
 * Public functions
 */

int
bcm56780_a0_ltsw_feature_drv_attach(int unit)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (bcm56780_a0_ltsw_variant_drv_attach(unit,
                                             BCM56780_A0_LTSW_VARIANT_FEATURE));
exit:
    SHR_FUNC_EXIT();
}

