/*! \file bcmfp_ingress_init.c
 *
 * Init/Cleanup APIs for Ingress Field Processor.
 */
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

#include <shr/shr_error.h>
#include <shr/shr_debug.h>
#include <bcmfp/bcmfp_internal.h>
#include <bcmfp/bcmfp_ingress_internal.h>
#include <bcmltd/chip/bcmltd_id.h>
#include <bcmlrd/bcmlrd_client.h>
#include <bcmfp/bcmfp_compression_internal.h>

#define BSL_LOG_MODULE BSL_LS_BCMFP_DEV

int
bcmfp_ingress_cleanup(int unit)
{
    bcmfp_stage_t *stage = NULL;
    bcmfp_stage_id_t stage_id = BCMFP_STAGE_ID_INGRESS;

    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT(bcmfp_stage_get(unit, stage_id, &stage));

    if (stage == NULL) {
        SHR_EXIT();
    }

    SHR_IF_ERR_VERBOSE_EXIT(bcmfp_ext_cfg_db_cleanup(unit, stage));

    SHR_IF_ERR_VERBOSE_EXIT(bcmfp_qual_cfg_db_cleanup(unit, stage));

    SHR_IF_ERR_VERBOSE_EXIT(bcmfp_action_cfg_db_cleanup(unit, stage));

    SHR_IF_ERR_VERBOSE_EXIT(bcmfp_stage_hw_entry_info_cleanup(unit, stage));

    SHR_IF_ERR_VERBOSE_EXIT(bcmfp_stage_hw_field_info_cleanup(unit, stage));

    SHR_IF_ERR_VERBOSE_EXIT(bcmfp_stage_misc_info_cleanup(unit, stage));

    SHR_IF_ERR_VERBOSE_EXIT(bcmfp_stage_pdd_hw_info_cleanup(unit, stage));

    SHR_IF_ERR_VERBOSE_EXIT(bcmfp_stage_sbr_hw_info_cleanup(unit, stage));

    SHR_IF_ERR_VERBOSE_EXIT(bcmfp_stage_profiles_hw_info_cleanup(unit, stage));

    SHR_IF_ERR_VERBOSE_EXIT(bcmfp_compress_cleanup(unit, stage));

    SHR_IF_ERR_VERBOSE_EXIT(bcmfp_ltd_buffers_cleanup(unit, stage));

    SHR_FREE(stage);

    SHR_IF_ERR_VERBOSE_EXIT(bcmfp_stage_set(unit, stage_id, NULL));

exit:
    SHR_FUNC_EXIT();
}

int
bcmfp_ingress_init(int unit, bool warm)
{
    bcmfp_stage_t *stage = NULL;
    bcmfp_stage_id_t stage_id = BCMFP_STAGE_ID_INGRESS;

    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT(bcmfp_stage_get(unit, stage_id, &stage));

    if (stage == NULL) {
        SHR_EXIT();
    }

    SHR_IF_ERR_EXIT(bcmfp_stage_init(unit, stage, warm));

    SHR_IF_ERR_VERBOSE_EXIT(bcmfp_lrd_info_init(unit, stage));

    SHR_IF_ERR_VERBOSE_EXIT(bcmfp_ltd_buffers_init(unit, stage));

    SHR_IF_ERR_VERBOSE_EXIT
        (bcmfp_compress_init(unit, stage, bcmfp_cid_updates, NULL));

    SHR_IF_ERR_VERBOSE_EXIT
        (bcmfp_compress_fid_hw_info_init(unit));

exit:
    SHR_FUNC_EXIT();
}
