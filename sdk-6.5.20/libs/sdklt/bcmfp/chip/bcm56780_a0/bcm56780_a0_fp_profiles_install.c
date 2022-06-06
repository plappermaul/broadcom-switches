/*! \file bcm56780_a0_fp_profiles_install.c
 *
 * Chip Specific Stage profiles install function
 * for Trident4-X9(56780_A0).
 */
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */
#include <shr/shr_error.h>
#include <shr/shr_debug.h>
#include <bcmfp/bcmfp_internal.h>
#include <bcmfp/bcmfp_chip_internal.h>
#include <bcmfp/chip/bcm56780_a0_fp.h>

#define BSL_LOG_MODULE BSL_LS_BCMFP_INIT

int
bcm56780_a0_fp_profiles_install(int unit,
                                const bcmltd_op_arg_t *op_arg,
                                uint32_t flags,
                                bcmfp_stage_t *stage,
                                bcmfp_group_id_t group_id,
                                bcmfp_group_oper_info_t *opinfo,
                                bcmfp_buffers_t *buffers)
{
    bcmfp_stage_id_t stage_id;

    SHR_FUNC_ENTER(unit);

    stage_id = stage->stage_id;

    switch (stage_id) {
        case BCMFP_STAGE_ID_INGRESS:
        case BCMFP_STAGE_ID_LOOKUP:
        case BCMFP_STAGE_ID_EGRESS:
            break;
        case BCMFP_STAGE_ID_EXACT_MATCH:
        case BCMFP_STAGE_ID_FLOW_TRACKER:
            SHR_IF_ERR_VERBOSE_EXIT
                (bcm56780_a0_fp_em_profiles_install(unit,
                                                    stage_id,
                                                    op_arg->trans_id,
                                                    flags,
                                                    group_id,
                                                    opinfo,
                                                    buffers));
            break;
        default:
            SHR_ERR_EXIT(SHR_E_INTERNAL);
    }
exit:
    SHR_FUNC_EXIT();

}
