/*! \file bcmfp_em_ft_init.c
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
#include <bcmfp/bcmfp_em_ft_internal.h>
#include <bcmptm/bcmptm_uft.h>
#include <bcmltd/chip/bcmltd_id.h>
#include <bcmlrd/bcmlrd_client.h>
#include <bcmfp/bcmfp_compression_internal.h>

#define BSL_LOG_MODULE BSL_LS_BCMFP_DEV

int
bcmfp_em_ft_cleanup(int unit)
{
    bcmfp_stage_t *stage = NULL;
    bcmfp_stage_id_t stage_id = BCMFP_STAGE_ID_FLOW_TRACKER;

    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_EXIT(bcmfp_stage_get(unit, stage_id, &stage));

    if (stage == NULL) {
        SHR_EXIT();
    }

    SHR_IF_ERR_EXIT(bcmfp_ext_cfg_db_cleanup(unit, stage));

    SHR_IF_ERR_EXIT(bcmfp_qual_cfg_db_cleanup(unit, stage));

    SHR_IF_ERR_EXIT(bcmfp_action_cfg_db_cleanup(unit, stage));

    SHR_IF_ERR_EXIT(bcmfp_stage_hw_entry_info_cleanup(unit, stage));

    SHR_IF_ERR_EXIT(bcmfp_stage_misc_info_cleanup(unit, stage));

    SHR_IF_ERR_EXIT(bcmfp_stage_pdd_hw_info_cleanup(unit, stage));

    SHR_IF_ERR_EXIT(bcmfp_stage_sbr_hw_info_cleanup(unit, stage));

    SHR_IF_ERR_VERBOSE_EXIT(bcmfp_hit_context_cleanup(unit,stage));

    SHR_IF_ERR_VERBOSE_EXIT(bcmfp_stage_profiles_hw_info_cleanup(unit, stage));

    SHR_IF_ERR_VERBOSE_EXIT(bcmfp_compress_cleanup(unit, stage));

    SHR_IF_ERR_VERBOSE_EXIT(bcmfp_ltd_buffers_cleanup(unit, stage));

    SHR_FREE(stage);

    SHR_IF_ERR_EXIT(bcmfp_stage_set(unit, stage_id, NULL));

exit:
    SHR_FUNC_EXIT();
}

int
bcmfp_em_ft_init(int unit, bool warm)
{
    bcmfp_stage_t *stage = NULL;
    bcmfp_stage_id_t stage_id = BCMFP_STAGE_ID_FLOW_TRACKER;

    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_EXIT(bcmfp_stage_get(unit, stage_id, &stage));

    if (stage == NULL) {
        SHR_EXIT();
    }

    SHR_IF_ERR_EXIT(bcmfp_stage_init(unit, stage, warm));

    SHR_IF_ERR_VERBOSE_EXIT(bcmfp_lrd_info_init(unit, stage));

    SHR_IF_ERR_VERBOSE_EXIT(bcmfp_ltd_buffers_init(unit, stage));

    SHR_IF_ERR_VERBOSE_EXIT
        (bcmfp_hit_context_init(unit, BCMPTM_DIRECT_TRANS_ID, stage, warm));

    SHR_IF_ERR_VERBOSE_EXIT
        (bcmfp_compress_init(unit, stage, bcmfp_cid_updates));
exit:
    SHR_FUNC_EXIT();
}

void
bcmfp_em_ft_grp_change_event_cb(int unit, const char *event, uint64_t ev_data)
{
    int rv = 0;
    uint32_t trans_id = 0;
    bcmfp_stage_t *stage = NULL;
    bcmfp_stage_id_t stage_id = BCMFP_STAGE_ID_FLOW_TRACKER;

    rv = bcmfp_stage_get(unit, stage_id, &stage);
    if (rv != SHR_E_NONE || stage == NULL) {
        return;
    }

    if (!BCMFP_STAGE_FLAGS_TEST(stage, BCMFP_STAGE_UFT_BANKS_DYNAMIC)) {
        return;
    }

    trans_id = BCMPTM_UFT_EV_INFO_TRANS_ID_GET(ev_data);

    /*
     * Copy the valid profiles and existing presel entries
     * from bank 0 to all valid banks
     */
    rv = (bcmfp_profiles_copy(unit,
                              trans_id,
                              stage));

    (void)bcmfp_bank_info_set(unit, stage);

    (void)bcmfp_hit_context_init(unit, trans_id, stage, false);

    return;
}
