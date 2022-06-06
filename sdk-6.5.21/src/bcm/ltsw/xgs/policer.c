/*
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated.
 * Edits to this file will be lost when it is regenerated.
 *
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

#include <bcm/types.h>
#include <bcm_int/control.h>

#include <bcm_int/ltsw/xgs/policer.h>
#include <bcm_int/ltsw/lt_intf.h>
#include <bcm_int/ltsw/dev.h>
#include <bcm_int/ltsw/init.h>
#include <bcm_int/ltsw/generated/policer_ha.h>
#include <bcm_int/ltsw/policer.h>
#include <bcm_int/ltsw/policer_int.h>
#include <bcm_int/ltsw/ha.h>
#include <bcm_int/ltsw/profile_mgmt.h>

#include <shr/shr_bitop.h>
#include <shr/shr_debug.h>
#include <bcmltd/chip/bcmltd_str.h>

/******************************************************************************
 * Local definitions
 */

#define BSL_LOG_MODULE BSL_LS_BCM_POLICER

/******************************************************************************
 * Private functions
 */


/******************************************************************************
 * Private functions
 */
static int
xgs_ltsw_get_meter_pair_mode(
    int unit,
    bcm_policer_mode_t mode,
    char *lt_mode)
{
    SHR_FUNC_ENTER(unit);

    switch (mode) {
    case bcmPolicerModeGreen:
        sal_strcpy(lt_mode, "DEFAULT");
        break;
    case bcmPolicerModeCommitted:
        sal_strcpy(lt_mode, "FLOW");
        break;
    case bcmPolicerModeSrTcmModified:
        sal_strcpy(lt_mode, "MODSRTCM");
        break;
    case bcmPolicerModeSrTcm:
        sal_strcpy(lt_mode, "SRTCM");
        break;
    case bcmPolicerModeTrTcm:
        sal_strcpy(lt_mode, "TRTCM");
        break;
    case bcmPolicerModeTrTcmDs:
        sal_strcpy(lt_mode, "MODTRTCM");
        break;
    default:
        SHR_ERR_EXIT(SHR_E_PARAM);
    }

exit:
    SHR_FUNC_EXIT();
}


static int
xgs_ltsw_policer_config(
    int unit,
    bcmi_ltsw_policer_stage_t stage,
    uint32_t pid,
    bcm_policer_config_t *pol_cfg,
    int pipe,
    int pool)
{
    int dunit = 0;
    char meter_mode[10] = "";
    uint8_t pkt_mode = 0, color_mode = 0;
    bcmlt_entry_handle_t entry_hdl = BCMLT_INVALID_HDL;

    SHR_FUNC_ENTER(unit);

    /* Input parameters check. */
    SHR_NULL_CHECK(pol_cfg, SHR_E_PARAM);

    SHR_IF_ERR_VERBOSE_EXIT
        (xgs_ltsw_get_meter_pair_mode(unit, pol_cfg->mode, meter_mode));

    dunit = bcmi_ltsw_dev_dunit(unit);
    if (stage == bcmiPolicerStageEgress) {
        SHR_IF_ERR_VERBOSE_EXIT
            (bcmlt_entry_allocate(dunit, METER_EGR_FP_TEMPLATEs, &entry_hdl));
        SHR_IF_ERR_VERBOSE_EXIT
            (bcmlt_entry_field_add(entry_hdl, METER_EGR_FP_TEMPLATE_IDs, pid));
    } else {
        SHR_IF_ERR_VERBOSE_EXIT
            (bcmlt_entry_allocate(dunit, METER_ING_FP_TEMPLATEs, &entry_hdl));
        SHR_IF_ERR_VERBOSE_EXIT
            (bcmlt_entry_field_add(entry_hdl, METER_ING_FP_TEMPLATE_IDs, pid));
    }

    SHR_IF_ERR_VERBOSE_EXIT
        (bcmlt_entry_field_symbol_add(entry_hdl, METER_MODEs, meter_mode));

    if (pool != 0) {
        SHR_IF_ERR_VERBOSE_EXIT
            (bcmlt_entry_field_add(entry_hdl, POOL_INSTANCEs, pool));
    }

    if (pipe != -1) {
        SHR_IF_ERR_VERBOSE_EXIT
            (bcmlt_entry_field_add(entry_hdl, PIPEs, pipe));
    }

    if (pol_cfg->flags & BCM_POLICER_MODE_PACKETS) {
        pkt_mode = 1;
    } else {
        pkt_mode = 0;
    }

    SHR_IF_ERR_VERBOSE_EXIT
        (bcmlt_entry_field_add(entry_hdl, PKT_MODEs, pkt_mode));

    if (pol_cfg->flags & BCM_POLICER_COLOR_BLIND) {
        color_mode = 0;
    } else {
        color_mode = 1;
    }

    SHR_IF_ERR_VERBOSE_EXIT
        (bcmlt_entry_field_add(entry_hdl, COLOR_MODEs, color_mode));

    if (pkt_mode == 1) {
        if (pol_cfg->ckbits_sec > 0) {
            SHR_IF_ERR_VERBOSE_EXIT
                (bcmlt_entry_field_add(entry_hdl,
                                       MIN_RATE_PPSs,
                                       pol_cfg->ckbits_sec));
        }
        if (pol_cfg->ckbits_burst > 0) {
            SHR_IF_ERR_VERBOSE_EXIT
                (bcmlt_entry_field_add(entry_hdl,
                                       MIN_BURST_SIZE_PKTSs,
                                       pol_cfg->ckbits_burst));
        }
        if (pol_cfg->pkbits_sec > 0) {
            SHR_IF_ERR_VERBOSE_EXIT
                (bcmlt_entry_field_add(entry_hdl,
                                       MAX_RATE_PPSs,
                                       pol_cfg->pkbits_sec));
        }
        if (pol_cfg->pkbits_burst > 0) {
            SHR_IF_ERR_VERBOSE_EXIT
                (bcmlt_entry_field_add(entry_hdl,
                                       MAX_BURST_SIZE_PKTSs,
                                       pol_cfg->pkbits_burst));
        }
    } else {
        if (pol_cfg->ckbits_sec > 0) {
            SHR_IF_ERR_VERBOSE_EXIT
                (bcmlt_entry_field_add(entry_hdl,
                                       MIN_RATE_KBPSs,
                                       pol_cfg->ckbits_sec));
        }
        if (pol_cfg->ckbits_burst > 0) {
            SHR_IF_ERR_VERBOSE_EXIT
                (bcmlt_entry_field_add(entry_hdl,
                                       MIN_BURST_SIZE_KBITSs,
                                       pol_cfg->ckbits_burst));
        }
        if (pol_cfg->pkbits_sec > 0) {
            SHR_IF_ERR_VERBOSE_EXIT
                (bcmlt_entry_field_add(entry_hdl,
                                       MAX_RATE_KBPSs,
                                       pol_cfg->pkbits_sec));
        }
        if (pol_cfg->pkbits_burst > 0) {
            SHR_IF_ERR_VERBOSE_EXIT
                (bcmlt_entry_field_add(entry_hdl,
                                       MAX_BURST_SIZE_KBITSs,
                                       pol_cfg->pkbits_burst));
        }
    }

    SHR_IF_ERR_VERBOSE_EXIT
        (bcmi_lt_entry_set_commit(unit, entry_hdl, BCMLT_PRIORITY_NORMAL));

exit:
    if (BCMLT_INVALID_HDL != entry_hdl) {
        (void)bcmlt_entry_free(entry_hdl);
    }
    SHR_FUNC_EXIT();
}

static int
xgs_ltsw_policer_delete(
    int unit,
    bcmi_ltsw_policer_stage_t stage,
    uint32_t pid)
{
    int dunit = 0;
    bcmlt_entry_handle_t entry_hdl = BCMLT_INVALID_HDL;

    SHR_FUNC_ENTER(unit);

    dunit = bcmi_ltsw_dev_dunit(unit);
    if (stage == bcmiPolicerStageEgress) {
        SHR_IF_ERR_VERBOSE_EXIT
            (bcmlt_entry_allocate(dunit, METER_EGR_FP_TEMPLATEs, &entry_hdl));
        SHR_IF_ERR_VERBOSE_EXIT
            (bcmlt_entry_field_add(entry_hdl, METER_EGR_FP_TEMPLATE_IDs, pid));
    } else {
        SHR_IF_ERR_VERBOSE_EXIT
            (bcmlt_entry_allocate(dunit, METER_ING_FP_TEMPLATEs, &entry_hdl));
        SHR_IF_ERR_VERBOSE_EXIT
            (bcmlt_entry_field_add(entry_hdl, METER_ING_FP_TEMPLATE_IDs, pid));
    }
    SHR_IF_ERR_EXIT_EXCEPT_IF
        (bcmi_lt_entry_commit(unit, entry_hdl,
                              BCMLT_OPCODE_DELETE,
                              BCMLT_PRIORITY_NORMAL), SHR_E_NOT_FOUND);

exit:
    if (BCMLT_INVALID_HDL != entry_hdl) {
        (void)bcmlt_entry_free(entry_hdl);
    }
    SHR_FUNC_EXIT();
}

static int
xgs_ltsw_policer_lookup(
    int unit,
    bcmi_ltsw_policer_stage_t stage,
    bcmlt_entry_handle_t entry_hdl,
    bcm_policer_config_t *pol_cfg)
{
    uint64_t val = 0;
    const char *meter_mode = NULL;

    SHR_FUNC_ENTER(unit);

    if (stage == bcmiPolicerStageEgress) {
        pol_cfg->flags |= BCM_POLICER_EGRESS;
    }

    SHR_IF_ERR_VERBOSE_EXIT
        (bcmlt_entry_field_symbol_get(entry_hdl, METER_MODEs, &meter_mode));
    if (!sal_strcmp(meter_mode, "DEFAULT")) {
        pol_cfg->mode = bcmPolicerModeGreen;
    } else if (!sal_strcmp(meter_mode, "FLOW")) {
        pol_cfg->mode = bcmPolicerModeCommitted;
    } else if (!sal_strcmp(meter_mode, "SRTCM")) {
        pol_cfg->mode = bcmPolicerModeSrTcm;
    } else if (!sal_strcmp(meter_mode, "MODSRTCM")) {
        pol_cfg->mode = bcmPolicerModeSrTcmModified;
    } else if (!sal_strcmp(meter_mode, "TRTCM")) {
        pol_cfg->mode = bcmPolicerModeTrTcm;
    } else if (!sal_strcmp(meter_mode, "MODTRTCM")) {
        pol_cfg->mode = bcmPolicerModeTrTcmDs;
    } else {
        SHR_ERR_EXIT(SHR_E_PARAM);
    }

    SHR_IF_ERR_VERBOSE_EXIT
        (bcmlt_entry_field_get(entry_hdl, PKT_MODEs, &val));
    if (val == 1) {
        pol_cfg->flags |= BCM_POLICER_MODE_PACKETS;
        SHR_IF_ERR_VERBOSE_EXIT
            (bcmlt_entry_field_get(entry_hdl, MIN_RATE_PPSs, &val));
        pol_cfg->ckbits_sec = val;

        SHR_IF_ERR_VERBOSE_EXIT
            (bcmlt_entry_field_get(entry_hdl, MIN_BURST_SIZE_PKTSs, &val));
        pol_cfg->ckbits_burst = val;

        SHR_IF_ERR_VERBOSE_EXIT
            (bcmlt_entry_field_get(entry_hdl, MIN_RATE_PPS_OPERs, &val));
        pol_cfg->actual_ckbits_sec = val;

        SHR_IF_ERR_VERBOSE_EXIT
            (bcmlt_entry_field_get(entry_hdl, MIN_BURST_SIZE_PKTS_OPERs, &val));
        pol_cfg->actual_ckbits_burst = val;

        if (pol_cfg->mode != bcmPolicerModeCommitted &&
            pol_cfg->mode != bcmPolicerModeGreen) {
            SHR_IF_ERR_VERBOSE_EXIT
                (bcmlt_entry_field_get(entry_hdl, MAX_RATE_PPSs, &val));
            pol_cfg->pkbits_sec = val;

            SHR_IF_ERR_VERBOSE_EXIT
                (bcmlt_entry_field_get(entry_hdl, MAX_BURST_SIZE_PKTSs, &val));
            pol_cfg->pkbits_burst = val;

            SHR_IF_ERR_VERBOSE_EXIT
                (bcmlt_entry_field_get(entry_hdl, MAX_RATE_PPS_OPERs, &val));
            pol_cfg->actual_pkbits_sec = val;

            SHR_IF_ERR_VERBOSE_EXIT
                (bcmlt_entry_field_get(entry_hdl, MAX_BURST_SIZE_PKTS_OPERs, &val));
            pol_cfg->actual_pkbits_burst = val;
        }
    } else {
        SHR_IF_ERR_VERBOSE_EXIT
            (bcmlt_entry_field_get(entry_hdl, MIN_RATE_KBPSs, &val));
        pol_cfg->ckbits_sec = val;

        SHR_IF_ERR_VERBOSE_EXIT
            (bcmlt_entry_field_get(entry_hdl, MIN_BURST_SIZE_KBITSs, &val));
        pol_cfg->ckbits_burst = val;

        SHR_IF_ERR_VERBOSE_EXIT
            (bcmlt_entry_field_get(entry_hdl, MIN_RATE_KBPS_OPERs, &val));
        pol_cfg->actual_ckbits_sec = val;

        SHR_IF_ERR_VERBOSE_EXIT
            (bcmlt_entry_field_get(entry_hdl, MIN_BURST_SIZE_KBITS_OPERs, &val));
        pol_cfg->actual_ckbits_burst = val;

        if (pol_cfg->mode != bcmPolicerModeCommitted &&
            pol_cfg->mode != bcmPolicerModeGreen) {
            SHR_IF_ERR_VERBOSE_EXIT
                (bcmlt_entry_field_get(entry_hdl, MAX_RATE_KBPSs, &val));
            pol_cfg->pkbits_sec = val;

            SHR_IF_ERR_VERBOSE_EXIT
                (bcmlt_entry_field_get(entry_hdl, MAX_BURST_SIZE_KBITSs, &val));
            pol_cfg->pkbits_burst = val;

            SHR_IF_ERR_VERBOSE_EXIT
                (bcmlt_entry_field_get(entry_hdl, MAX_RATE_KBPS_OPERs, &val));
            pol_cfg->actual_pkbits_sec = val;

            SHR_IF_ERR_VERBOSE_EXIT
                (bcmlt_entry_field_get(entry_hdl, MAX_BURST_SIZE_KBITS_OPERs, &val));
            pol_cfg->actual_pkbits_burst = val;
        }
    }

    SHR_IF_ERR_VERBOSE_EXIT
        (bcmlt_entry_field_get(entry_hdl, COLOR_MODEs, &val));
    if (val == 0) {
        pol_cfg->flags |= BCM_POLICER_COLOR_BLIND;
    }

    if (stage == bcmiPolicerStageIngress) {
        SHR_IF_ERR_VERBOSE_EXIT
            (bcmlt_entry_field_get(entry_hdl, POOL_INSTANCEs, &val));
        pol_cfg->pool_id = val;
    }

exit:
    SHR_FUNC_EXIT();
}


/******************************************************************************
 * Public functions
 */

int
xgs_ltsw_policer_deinit(int unit)
{
    return SHR_E_NONE;
}

int
xgs_ltsw_policer_init(int unit)
{
    return SHR_E_NONE;
}

int
xgs_ltsw_policer_create(
    int unit,
    bcm_policer_config_t *pol_cfg,
    bcm_policer_t *policer_id)
{
    return SHR_E_NONE;
}

int
xgs_ltsw_policer_destroy(
    int unit,
    bcm_policer_t policer_id)
{
    uint32_t pid = 0;
    bcmi_ltsw_policer_stage_t stage;

    SHR_FUNC_ENTER(unit);

    if ((policer_id & ~(BCM_FIELD_ID_MASK)) == BCM_FIELD_EFP_ID_BASE) {
        stage = bcmiPolicerStageEgress;
    } else {
        stage = bcmiPolicerStageIngress;
    }

    pid = (policer_id & BCM_FIELD_ID_MASK);

    SHR_IF_ERR_VERBOSE_EXIT
        (xgs_ltsw_policer_delete(unit, stage, pid));

exit:
    SHR_FUNC_EXIT();
}

int
xgs_ltsw_policer_destroy_all(int unit)
{
    return SHR_E_NONE;
}

int
xgs_ltsw_policer_set(
    int unit,
    bcm_policer_t policer_id,
    bcm_policer_config_t *pol_cfg)
{
    return SHR_E_NONE;
}

int
xgs_ltsw_policer_get(
    int unit,
    bcm_policer_t policer_id,
    bcm_policer_config_t *pol_cfg)
{
    bcmi_ltsw_policer_stage_t stage;
    uint32_t pid = 0;
    int dunit = 0;
    bcmlt_entry_handle_t entry_hdl = BCMLT_INVALID_HDL;

    SHR_FUNC_ENTER(unit);

    /* Input parameters check. */
    SHR_NULL_CHECK(pol_cfg, SHR_E_PARAM);

    if ((policer_id & ~(BCM_FIELD_ID_MASK)) == BCM_FIELD_EFP_ID_BASE) {
        stage = bcmiPolicerStageEgress;
    } else {
        stage = bcmiPolicerStageIngress;
    }

    pid = (policer_id & BCM_FIELD_ID_MASK);

    dunit = bcmi_ltsw_dev_dunit(unit);
    if (stage == bcmiPolicerStageEgress) {
        SHR_IF_ERR_VERBOSE_EXIT
            (bcmlt_entry_allocate(dunit, METER_EGR_FP_TEMPLATEs, &entry_hdl));
        SHR_IF_ERR_VERBOSE_EXIT
            (bcmlt_entry_field_add(entry_hdl, METER_EGR_FP_TEMPLATE_IDs, pid));
    } else {
        SHR_IF_ERR_VERBOSE_EXIT
            (bcmlt_entry_allocate(dunit, METER_ING_FP_TEMPLATEs, &entry_hdl));
        SHR_IF_ERR_VERBOSE_EXIT
            (bcmlt_entry_field_add(entry_hdl, METER_ING_FP_TEMPLATE_IDs, pid));
    }
    SHR_IF_ERR_EXIT_EXCEPT_IF
        (bcmi_lt_entry_commit(unit, entry_hdl,
                              BCMLT_OPCODE_LOOKUP,
                              BCMLT_PRIORITY_NORMAL), SHR_E_NOT_FOUND);

    SHR_IF_ERR_VERBOSE_EXIT
        (xgs_ltsw_policer_lookup(unit, stage, entry_hdl, pol_cfg));

exit:
    if (BCMLT_INVALID_HDL != entry_hdl) {
        (void)bcmlt_entry_free(entry_hdl);
    }
    SHR_FUNC_EXIT();
}

int
xgs_ltsw_policer_traverse(
    int unit,
    bcm_policer_traverse_cb cb,
    void *user_data)
{
    return SHR_E_NONE;
}

int
xgs_ltsw_policer_install(
    int unit,
    bcm_policer_t policer_id,
    bcm_policer_config_t *pol_cfg,
    int pipe,
    int pool)
{
    bcmi_ltsw_policer_stage_t stage;
    uint32_t pid = 0;

    SHR_FUNC_ENTER(unit);

    /* Input parameters check. */
    SHR_NULL_CHECK(pol_cfg, SHR_E_PARAM);

    if ((policer_id & ~(BCM_FIELD_ID_MASK)) == BCM_FIELD_EFP_ID_BASE) {
        stage = bcmiPolicerStageEgress;
    } else {
        stage = bcmiPolicerStageIngress;
    }

    pid = (policer_id & BCM_FIELD_ID_MASK);

    SHR_IF_ERR_VERBOSE_EXIT
        (xgs_ltsw_policer_config(unit, stage, pid, pol_cfg, pipe, pool));

exit:
    SHR_FUNC_EXIT();
}
