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

#include <bcm_int/ltsw/mbcm/hash_output_selection.h>
#include <bcm_int/ltsw/feature.h>
#include <bcm_int/ltsw/lt_intf.h>
#include <bcm_int/ltsw/dev.h>
#include <bcm_int/ltsw/chip/bcmgene_feature.h>
#include <bcm_int/ltsw/chip/bcm56880_a0/bcmgene.h>

#include <shr/shr_debug.h>
#include <bcmltd/chip/bcmltd_str.h>

/******************************************************************************
 * Local definitions
 */

#define BSL_LOG_MODULE BSL_LS_BCM_HASH

/******************************************************************************
 * Private functions
 */


/*!
 * \brief Instance pre-definition type.
 */
/*! Used flow based hashing. */
#define HOS_INSTANCE_FLOW_BASED_HASH 0

/*! Used port based hashing. */
#define HOS_INSTANCE_PORT_BASED_HASH 1

#define HOS_SUB_SEL_COUNT 8
static int sub_sel_width_default[][HOS_SUB_SEL_COUNT] = {
    /* Non-concatenation. */
    { 16, 16, 16, 16, 16, 16, 16, 16},
    /* Concatenation. */
    { 64, 64, 32, 64, 16, 16,  0,  0}
};

static bcmgene_desc_t *bcm56880_a0_hos_gene[BCM_MAX_NUM_UNITS];

/*!
 * \brief Get information of hash output selection functionalities.
 *
 * \param [in] unit Unit Number.
 * \param [in] info The hash output selection information.
 * \param [out] value The hash output selection information value.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
static int
hos_info_get(
    int unit,
    bcmint_hos_info_t info,
    int *value)
{
    SHR_FUNC_ENTER(unit);

    SHR_NULL_CHECK(value, SHR_E_PARAM);

    switch (info) {
    case bcmiHosInfoNumProfiles:
        *value = 1;
        break;
    case bcmiHosInfoNumInstances:
        *value = 2;
        break;
    case bcmiHosInfoNumFlowIds:
        *value = 256;
        break;
    default:
        SHR_ERR_EXIT(SHR_E_PARAM);
    }

exit:
    SHR_FUNC_EXIT();
}

/*!
 * \brief Validate the hash output selection profile parameters.
 *
 * \param [in] unit Unit Number.
 * \param [in] profile_id Profile identifier.
 * \param [in] type The hash output selection type.
 * \param [in] array_size Array size.
 * \param [in] ctrl_type The hash output selection profile control type.
 * \param [in] value The hash output selection profile control value.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
static int
hos_validate_profile_params(
    int unit,
    int profile_id,
    bcmi_ltsw_hos_type_t type,
    bcmi_ltsw_hos_profile_control_t ctrl_type,
    int *value)
{
    SHR_FUNC_ENTER(unit);

    switch (type) {
    case bcmiHosTypeDlbTrunk:
    case bcmiHosTypeDlbEcmp:
        if (!ltsw_feature(unit, LTSW_FT_DLB)) {
            SHR_ERR_EXIT(SHR_E_UNAVAIL);
        }
        break;
    case bcmiHosTypeTrunkVp:
        if (!ltsw_feature(unit, LTSW_FT_VPLAG)) {
            SHR_ERR_EXIT(SHR_E_UNAVAIL);
        }
        break;
    default:
        break;
    }

    switch (type) {
    case bcmiHosTypeTrunkUc:
    case bcmiHosTypeTrunkFailover:
    case bcmiHosTypeTrunkSystem:
    case bcmiHosTypeTrunkSystemFailover:
    case bcmiHosTypeTrunkVp:
    case bcmiHosTypeEcmpUnderlay:
    case bcmiHosTypeEcmpOverlay:
    case bcmiHosTypeDlbTrunk:
    case bcmiHosTypeDlbEcmp:
    case bcmiHosTypeEntropy:
        switch(ctrl_type) {
        case bcmiHosProfileControlInstance:
            if (value && (*value < 0 || *value >= 2)) {
                SHR_ERR_EXIT(SHR_E_PARAM);
            }
            break;
        case bcmiHosProfileControlFlowIdSrc:
            if (value &&
                (*value < bcmiHosFlowIdSrcBusHashIndex ||
                 *value >= bcmiHosFlowIdSrcCount)) {
                SHR_ERR_EXIT(SHR_E_PARAM);
            }
            break;
        default:
            SHR_ERR_EXIT(SHR_E_PARAM);
        }
        break;
    case bcmiHosTypeTrunkNonUc:
        switch(ctrl_type) {
        case bcmiHosProfileControlInstance:
            if (value && (*value < 0 || *value >= 2)) {
                SHR_ERR_EXIT(SHR_E_PARAM);
            }
            break;
        case bcmiHosProfileControlFlowIdSrc:
            if (value &&
                (*value < bcmiHosFlowIdSrcBusHashIndex ||
                 *value >= bcmiHosFlowIdSrcCount)) {
                SHR_ERR_EXIT(SHR_E_PARAM);
            }
            break;
        case bcmiHosProfileControlUseHigig3Entropy:
            break;
        default:
            SHR_ERR_EXIT(SHR_E_PARAM);
        }
        break;
    default:
        SHR_ERR_EXIT(SHR_E_PARAM);
    }

exit:
    SHR_FUNC_EXIT();
}

/*!
 * \brief Validate the hash output selection instance parameters.
 *
 * \param [in] unit Unit Number.
 * \param [in] type The hash output selection type.
 * \param [in] inst_id Instance identifier.
 * \param [in] flow_id Flow identifier.
 * \param [in] array_size Array size.
 * \param [in] ctrl_type The hash output selection instance control type.
 * \param [in] value The hash output selection control value.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
static int
hos_validate_instance_params(
    int unit,
    bcmi_ltsw_hos_type_t type,
    int inst_id,
    int flow_id,
    bcmi_ltsw_hos_instance_control_t ctrl_type,
    int *value)
{
    int inst_count, flow_id_count;
    SHR_FUNC_ENTER(unit);

    switch (type) {
    case bcmiHosTypeDlbTrunk:
    case bcmiHosTypeDlbEcmp:
        if (!ltsw_feature(unit, LTSW_FT_DLB)) {
            SHR_ERR_EXIT(SHR_E_UNAVAIL);
        }
        break;
    case bcmiHosTypeTrunkVp:
        if (!ltsw_feature(unit, LTSW_FT_VPLAG)) {
            SHR_ERR_EXIT(SHR_E_UNAVAIL);
        }
        break;
    case bcmiHosTypeTrunkUc:
    case bcmiHosTypeTrunkNonUc:
    case bcmiHosTypeTrunkFailover:
    case bcmiHosTypeTrunkSystem:
    case bcmiHosTypeTrunkSystemFailover:
    case bcmiHosTypeEcmpOverlay:
    case bcmiHosTypeEcmpUnderlay:
    case bcmiHosTypeEntropy:
        break;
    default:
        SHR_ERR_EXIT(SHR_E_PARAM);
    }

    SHR_IF_ERR_VERBOSE_EXIT
        (hos_info_get(unit, bcmiHosInfoNumInstances, &inst_count));
    SHR_IF_ERR_VERBOSE_EXIT
        (hos_info_get(unit, bcmiHosInfoNumFlowIds, &flow_id_count));
    if (inst_id < 0 || inst_id >= inst_count) {
        SHR_ERR_EXIT(SHR_E_PARAM);
    }
    if ((flow_id < 0 && flow_id != -1) || flow_id >= flow_id_count) {
        SHR_ERR_EXIT(SHR_E_PARAM);
    }

    switch(ctrl_type) {
    case bcmiHosInstaceControlResultSize:
        if (value && (*value < 1 || *value > 16)) {
            SHR_ERR_EXIT(SHR_E_PARAM);
        }
        break;
    case bcmiHosInstaceControlOffset:
        if (value && (*value < 0 || *value >= 64)) {
            SHR_ERR_EXIT(SHR_E_PARAM);
        }
        break;
    case bcmiHosInstaceControlSubSelect:
        if (value && (*value < 0 || *value > 7)) {
            SHR_ERR_EXIT(SHR_E_PARAM);
        }
        break;
    case bcmiHosInstaceControlConcat:
        if (value && (*value < 0 || *value > 1)) {
            SHR_ERR_EXIT(SHR_E_PARAM);
        }
        break;
    default:
        SHR_ERR_EXIT(SHR_E_PARAM);
    }

exit:
    SHR_FUNC_EXIT();
}

/*!
 * \brief Get the default value of result size.
 *
 * \param [in] unit Unit Number.
 * \param [in] type The hash output selection type.
 * \param [out] result_size The default result size.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
static int
hos_result_size_default_get(
    int unit,
    bcmi_ltsw_hos_type_t type,
    int *result_size)
{
    SHR_FUNC_ENTER(unit);

    SHR_NULL_CHECK(result_size, SHR_E_PARAM);

    switch (type) {
    case bcmiHosTypeTrunkUc:
    case bcmiHosTypeTrunkNonUc:
    case bcmiHosTypeTrunkSystem:
        *result_size = 7; /* bits */
        break;
    case bcmiHosTypeTrunkFailover:
        *result_size = 2; /* bits */
        break;
    case bcmiHosTypeTrunkSystemFailover:
        *result_size = 3; /* bits */
        break;
    case bcmiHosTypeTrunkVp:
        *result_size = 12; /* bits */
        break;
    case bcmiHosTypeEcmpOverlay:
        *result_size = 12; /* bits */
        break;
    case bcmiHosTypeEcmpUnderlay:
        *result_size = 12; /* bits */
        break;
    case bcmiHosTypeDlbTrunk:
        *result_size = 6; /* bits */
        break;
    case bcmiHosTypeDlbEcmp:
        *result_size = 12; /* bits */
        break;
    case bcmiHosTypeEntropy:
        *result_size = 7; /* bits */
        break;
    default:
        SHR_ERR_EXIT(SHR_E_PARAM);
    }

exit:
    SHR_FUNC_EXIT();
}

/*!
 * \brief Get the sub-feature information for a specified HOS type.
 *
 * \param [in] unit Unit Number.
 * \param [in] type The hash output selection type.
 * \param [out] sub_f The sub-feature.
 * \param [out] overlay This type sub-feature is overlay of others.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
static int
hos_sub_feature_info_get(
    int unit,
    bcmi_ltsw_hos_type_t type,
    uint32_t *sub_f,
    bool *overlay)
{
    SHR_FUNC_ENTER(unit);

    SHR_NULL_CHECK(sub_f, SHR_E_PARAM);
    SHR_NULL_CHECK(overlay, SHR_E_PARAM);

    *overlay = false;

    switch (type) {
    case bcmiHosTypeTrunkUc:
        *sub_f = BCMGENE_HASH_OUTPUT_SELECTION_SUB_F_TRUNK_UC;
        break;
    case bcmiHosTypeTrunkNonUc:
        *sub_f = BCMGENE_HASH_OUTPUT_SELECTION_SUB_F_TRUNK_NONUC;
        break;
    case bcmiHosTypeTrunkFailover:
        *sub_f = BCMGENE_HASH_OUTPUT_SELECTION_SUB_F_TRUNK_FAILOVER;
        break;
    case bcmiHosTypeTrunkSystem:
        *sub_f = BCMGENE_HASH_OUTPUT_SELECTION_SUB_F_TRUNK_SYSTEM;
        break;
    case bcmiHosTypeTrunkSystemFailover:
        *sub_f = BCMGENE_HASH_OUTPUT_SELECTION_SUB_F_TRUNK_SYSTEM_FAILOVER;
        break;
    case bcmiHosTypeTrunkVp:
        *overlay = true;
        *sub_f = BCMGENE_HASH_OUTPUT_SELECTION_SUB_F_ECMP_OVERLAY;
        break;
    case bcmiHosTypeEcmpOverlay:
        *sub_f = BCMGENE_HASH_OUTPUT_SELECTION_SUB_F_ECMP_OVERLAY;
        break;
    case bcmiHosTypeEcmpUnderlay:
        *sub_f = BCMGENE_HASH_OUTPUT_SELECTION_SUB_F_ECMP_UNDERLAY;
        break;
    case bcmiHosTypeDlbTrunk:
        *sub_f = BCMGENE_HASH_OUTPUT_SELECTION_SUB_F_DLB_TRUNK;
        break;
    case bcmiHosTypeDlbEcmp:
        *sub_f = BCMGENE_HASH_OUTPUT_SELECTION_SUB_F_DLB_ECMP;
        break;
    case bcmiHosTypeEntropy:
        *sub_f = BCMGENE_HASH_OUTPUT_SELECTION_SUB_F_ENTROPY;
        break;
    default:
        SHR_ERR_EXIT(SHR_E_PARAM);
    }

exit:
    SHR_FUNC_EXIT();
}

/*!
 * \brief Get the sub-feature for a specified HOS type.
 *
 * \param [in] unit Unit Number.
 * \param [in] type The hash output selection type.
 * \param [out] sub_f The sub-feature.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
static int
hos_sub_feature_get(
    int unit,
    bcmi_ltsw_hos_type_t type,
    uint32_t *sub_f)
{
    bool overlay = false;
    return hos_sub_feature_info_get(unit, type, sub_f, &overlay);
}

static int
hos_profile_create_cb(
    bcmgene_handle_t *hndl,
    void *user_data)
{
    int profile_id = *(int *)user_data;

    SHR_FUNC_ENTER(hndl ? hndl->unit : BSL_UNIT_UNKNOWN);

    SHR_IF_ERR_VERBOSE_EXIT
        (bcm56880_a0_gene_handle_create(hndl));
    SHR_IF_ERR_VERBOSE_EXIT
        (bcm56880_a0_gene_fill_key(hndl, 0, profile_id));
    SHR_IF_ERR_VERBOSE_EXIT
        (bcm56880_a0_gene_fill_opcode(hndl, BCMLT_OPCODE_INSERT));
    SHR_IF_ERR_VERBOSE_EXIT
        (bcm56880_a0_gene_commit(hndl));

exit:
    SHR_FUNC_EXIT();
}

static int
hos_profile_destroy_cb(
    bcmgene_handle_t *hndl,
    void *user_data)
{
    int profile_id = *(int *)user_data;

    SHR_FUNC_ENTER(hndl ? hndl->unit : BSL_UNIT_UNKNOWN);

    SHR_IF_ERR_VERBOSE_EXIT
        (bcm56880_a0_gene_handle_create(hndl));
    SHR_IF_ERR_VERBOSE_EXIT
        (bcm56880_a0_gene_fill_key(hndl, 0, profile_id));
    SHR_IF_ERR_VERBOSE_EXIT
        (bcm56880_a0_gene_fill_opcode(hndl, BCMLT_OPCODE_DELETE));
    SHR_IF_ERR_VERBOSE_EXIT
        (bcm56880_a0_gene_commit(hndl));

exit:
    SHR_FUNC_EXIT();
}

static int
hos_profile_lookup_cb(
    bcmgene_handle_t *hndl,
    void *user_data)
{
    int profile_id = *(int *)user_data;

    SHR_FUNC_ENTER(hndl ? hndl->unit : BSL_UNIT_UNKNOWN);

    SHR_IF_ERR_VERBOSE_EXIT
        (bcm56880_a0_gene_handle_create(hndl));
    SHR_IF_ERR_VERBOSE_EXIT
        (bcm56880_a0_gene_fill_key(hndl, 0, profile_id));
    SHR_IF_ERR_VERBOSE_EXIT
        (bcm56880_a0_gene_lookup(hndl));
    SHR_IF_ERR_VERBOSE_EXIT
        (bcm56880_a0_gene_lookup_done(hndl));

exit:
    SHR_FUNC_EXIT();
}

/*!
 * \brief Get the maximum of hashing value offset.
 *
 * \param [in] unit Unit Number.
 * \param [in] type The hash output selection type.
 * \param [in] concat Concatenation mode.
 * \param [out] max_offset The maximum of hashing value offset.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
static int
hos_instance_offset_max_get(
    int unit,
    bcmi_ltsw_hos_type_t type,
    int concat,
    int *max_offset)
{
    int i, total_width = 0;

    SHR_FUNC_ENTER(unit);

    SHR_NULL_CHECK(max_offset, SHR_E_PARAM);

    if (concat < 0 || concat > 1) {
        SHR_ERR_EXIT(SHR_E_PARAM);
    }
    total_width = 0;
    for (i = 0; i < HOS_SUB_SEL_COUNT; i++) {
        total_width += sub_sel_width_default[concat][i];
    }
    if (max_offset) {
        *max_offset = total_width;
    }

exit:
    SHR_FUNC_EXIT();
}

/*!
 * \brief Instance offset decode.
 *
 * \param [in] unit Unit Number.
 * \param [in] type The hash output selection type.
 * \param [in] concat Concatenation mode.
 * \param [in] offset Hashing value offset.
 * \param [out] sub_sel The sub-selection of hashing value offset.
 * \param [out] sub_offset The sub-offset of hashing value offset.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
static int
hos_instance_offset_decode(
    int unit,
    bcmi_ltsw_hos_type_t type,
    int concat,
    int offset,
    int *sub_sel,
    int *sub_offset)
{
    int i, total_width = 0;
    int found, sub_width;

    SHR_FUNC_ENTER(unit);

    SHR_NULL_CHECK(sub_sel, SHR_E_PARAM);
    SHR_NULL_CHECK(sub_offset, SHR_E_PARAM);

    if (concat < 0 || concat > 1) {
        SHR_ERR_EXIT(SHR_E_PARAM);
    }
    if (offset < 0) {
        SHR_ERR_EXIT(SHR_E_PARAM);
    }
    total_width = 0;
    for (i = 0; i < HOS_SUB_SEL_COUNT; i++) {
        sub_width = sub_sel_width_default[concat][i];
        total_width += sub_width;
    }
    offset %= total_width;
    found = 0;
    total_width = 0;
    for (i = 0; i < HOS_SUB_SEL_COUNT; i++) {
        sub_width = sub_sel_width_default[concat][i];
        if (offset < (total_width + sub_width)) {
            found = 1;
            break;
        }
        total_width += sub_width;
    }
    if (!found) {
        SHR_ERR_EXIT(SHR_E_PARAM);
    }

    if (sub_sel) {
        *sub_sel = i;
    }
    if (sub_sel) {
        *sub_offset = offset - total_width;
    }

exit:
    SHR_FUNC_EXIT();
}

/*!
 * \brief Instance offset encode.
 *
 * \param [in] unit Unit Number.
 * \param [in] type The hash output selection type.
 * \param [in] concat Concatenation mode.
 * \param [in] sub_sel The sub-selection of hashing value offset.
 * \param [in] sub_offset The sub-offset of hashing value offset.
 * \param [out] offset Hashing value offset.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
static int
hos_instance_offset_encode(
    int unit,
    bcmi_ltsw_hos_type_t type,
    int concat,
    int sub_sel,
    int sub_offset,
    int *offset)
{
    int i, total_width = 0;

    SHR_FUNC_ENTER(unit);

    SHR_NULL_CHECK(offset, SHR_E_PARAM);

    if (concat < 0 || concat > 1) {
        SHR_ERR_EXIT(SHR_E_PARAM);
    }
    if (sub_sel < 0 || sub_sel >= HOS_SUB_SEL_COUNT) {
        SHR_ERR_EXIT(SHR_E_PARAM);
    }
    if (sub_offset < 0 || sub_offset >= sub_sel_width_default[concat][sub_sel]) {
        SHR_ERR_EXIT(SHR_E_PARAM);
    }
    total_width = 0;
    for (i = 0; i < sub_sel; i++) {
        total_width += sub_sel_width_default[concat][i];
    }
    if (offset) {
        *offset = total_width + sub_offset;
    }

exit:
    SHR_FUNC_EXIT();
}

/*!
 * \brief Clear hash output selection module LT tables.
 *
 * \param [in] unit Unit number.
 *
 * \retval SHR_E_NONE Success.
 * \retval !SHR_E_NONE Failure.
 */
static int
hos_init_clear_hw(int unit)
{
    bcmgene_desc_t *gene = bcm56880_a0_hos_gene[unit];
    bcmgene_func_desc_t *func = NULL;
    bcmgene_table_desc_t *tbl = NULL;
    int i, j;

    SHR_FUNC_ENTER(unit);

    for (i = 0; i < gene->funcs; i++) {
        func = &gene->func[i];
        for (j = 0; j < func->tbls; j++) {
            tbl = &func->tbl[j];
            SHR_IF_ERR_VERBOSE_EXIT
                (bcmi_lt_clear(unit, tbl->table));
        }
    }

exit:
    SHR_FUNC_EXIT();
}

/*!
 * \brief Initialize hash output selection instance LT tables.
 *
 * \param [in] unit Unit number.
 *
 * \retval SHR_E_NONE Success.
 * \retval !SHR_E_NONE Failure.
 */
static int
hos_instance_init(int unit)
{
    bcmgene_desc_t *gene = bcm56880_a0_hos_gene[unit];
    bcmgene_handle_t hndl;
    uint32_t sub_f;
    bool overlay;
    int i, rv, inst_id, inst_count, flow_id, flow_id_count;
    int concat, hash_offset, sub_sel, sub_offset, result_size, max_offset;

    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (hos_info_get(unit, bcmiHosInfoNumInstances, &inst_count));
    SHR_IF_ERR_VERBOSE_EXIT
        (hos_info_get(unit, bcmiHosInfoNumFlowIds, &flow_id_count));

    for (i = 0; i < bcmiHosTypeCount; i++) {
        rv = hos_sub_feature_info_get(unit, i, &sub_f, &overlay);
        if (SHR_FAILURE(rv)) {
            continue;
        }
        if (overlay) {
            continue;
        }
        SHR_IF_ERR_VERBOSE_EXIT
            (bcm56880_a0_gene_targeting(unit, gene,
                                        BCMGENE_HASH_OUTPUT_SELECTION_F_INSTANCE,
                                        sub_f,
                                        &hndl));
        for (inst_id = 0; inst_id < inst_count; inst_id++) {
            SHR_IF_ERR_VERBOSE_EXIT
                (bcm56880_a0_gene_batch_handle_create(&hndl));
            for (flow_id = 0; flow_id < flow_id_count; flow_id++) {
                SHR_IF_ERR_VERBOSE_EXIT
                    (bcm56880_a0_gene_handle_create(&hndl));
                SHR_IF_ERR_VERBOSE_EXIT
                    (bcm56880_a0_gene_fill_key(&hndl, 0, inst_id));
                SHR_IF_ERR_VERBOSE_EXIT
                    (bcm56880_a0_gene_fill_key(&hndl, 1, flow_id));
                concat = 0;
                if (inst_id == HOS_INSTANCE_PORT_BASED_HASH) {
                    hash_offset = 0;
                } else {
                    hash_offset = inst_id * flow_id_count + flow_id;
                }
                rv = hos_instance_offset_max_get(unit, i, concat, &max_offset);
                SHR_IF_ERR_VERBOSE_EXIT
                    (bcm56880_a0_gene_error_return(&hndl, rv));
                hash_offset %= max_offset;
                rv = hos_instance_offset_decode(unit, i, concat, hash_offset,
                                                &sub_sel, &sub_offset);
                SHR_IF_ERR_VERBOSE_EXIT
                    (bcm56880_a0_gene_error_return(&hndl, rv));
                rv = hos_result_size_default_get(unit, i, &result_size);
                SHR_IF_ERR_VERBOSE_EXIT
                    (bcm56880_a0_gene_error_return(&hndl, rv));
                SHR_IF_ERR_VERBOSE_EXIT
                    (bcm56880_a0_gene_fill_field(&hndl, 0,
                                                 bcmiHosInstaceControlConcat,
                                                 concat));
                SHR_IF_ERR_VERBOSE_EXIT
                    (bcm56880_a0_gene_fill_field(&hndl, 0,
                                                 bcmiHosInstaceControlSubSelect,
                                                 sub_sel));
                SHR_IF_ERR_VERBOSE_EXIT
                    (bcm56880_a0_gene_fill_field(&hndl, 0,
                                                 bcmiHosInstaceControlOffset,
                                                 sub_offset));
                SHR_IF_ERR_VERBOSE_EXIT
                    (bcm56880_a0_gene_fill_field(&hndl, 0,
                                                 bcmiHosInstaceControlResultSize,
                                                 result_size));
                SHR_IF_ERR_VERBOSE_EXIT
                    (bcm56880_a0_gene_fill_opcode(&hndl, BCMLT_OPCODE_INSERT));
                SHR_IF_ERR_VERBOSE_EXIT
                    (bcm56880_a0_gene_fill_batch_handle(&hndl));
            }
            SHR_IF_ERR_VERBOSE_EXIT
                (bcm56880_a0_gene_commit_batch(&hndl));
        }
    }

exit:
    SHR_FUNC_EXIT();
}


static int
bcm56880_a0_ltsw_hos_info_get(
    int unit,
    bcmint_hos_info_t info,
    int *value)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (hos_info_get(unit, info, value));

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56880_a0_ltsw_hos_validate_profile_params(
    int unit,
    int profile_id,
    bcmi_ltsw_hos_type_t type,
    int array_size,
    bcmi_ltsw_hos_profile_control_t *ctrl_type,
    int *value)
{
    int i;
    SHR_FUNC_ENTER(unit);

    for (i = 0; i < array_size; i++) {
        SHR_IF_ERR_VERBOSE_EXIT
            (hos_validate_profile_params(unit, profile_id,
                                         type, ctrl_type[i],
                                         value ? &value[i] : NULL));
    }

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56880_a0_ltsw_hos_validate_instance_params(
    int unit,
    bcmi_ltsw_hos_type_t type,
    int inst_id,
    int flow_id,
    int array_size,
    bcmi_ltsw_hos_instance_control_t *ctrl_type,
    int *value)
{
    int i;
    SHR_FUNC_ENTER(unit);

    for (i = 0; i < array_size; i++) {
        SHR_IF_ERR_VERBOSE_EXIT
            (hos_validate_instance_params(unit, type, inst_id, flow_id,
                                          ctrl_type[i],
                                          value ? &value[i] : NULL));
    }

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56880_a0_ltsw_hos_profile_create(
    int unit,
    uint32_t option,
    int *profile_id)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (bcm56880_a0_gene_func_traverse(unit, bcm56880_a0_hos_gene[unit],
                                        BCMGENE_HASH_OUTPUT_SELECTION_F_PROFILE,
                                        hos_profile_create_cb, profile_id));
exit:
    SHR_FUNC_EXIT();
}

static int
bcm56880_a0_ltsw_hos_profile_destroy(
    int unit,
    int profile_id)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (bcm56880_a0_gene_func_traverse(unit, bcm56880_a0_hos_gene[unit],
                                        BCMGENE_HASH_OUTPUT_SELECTION_F_PROFILE,
                                        hos_profile_destroy_cb, &profile_id));
exit:
    SHR_FUNC_EXIT();
}

static int
bcm56880_a0_ltsw_hos_profile_configured(
    int unit,
    int profile_id)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (bcm56880_a0_gene_func_traverse(unit, bcm56880_a0_hos_gene[unit],
                                        BCMGENE_HASH_OUTPUT_SELECTION_F_PROFILE,
                                        hos_profile_lookup_cb, &profile_id));

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56880_a0_ltsw_hos_profile_set(
    int unit,
    int profile_id,
    bcmi_ltsw_hos_type_t type,
    int array_size,
    bcmi_ltsw_hos_profile_control_t *ctrl_type,
    int *value)
{
    bcmgene_desc_t *gene = bcm56880_a0_hos_gene[unit];
    bcmgene_handle_t hndl;
    int i;
    uint32_t sub_f;

    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (hos_sub_feature_get(unit, type, &sub_f));

    SHR_IF_ERR_VERBOSE_EXIT
        (bcm56880_a0_gene_targeting(unit, gene,
                                    BCMGENE_HASH_OUTPUT_SELECTION_F_PROFILE,
                                    sub_f,
                                    &hndl));
    SHR_IF_ERR_VERBOSE_EXIT
        (bcm56880_a0_gene_handle_create(&hndl));
    SHR_IF_ERR_VERBOSE_EXIT
        (bcm56880_a0_gene_fill_key(&hndl, 0, profile_id));

    for (i = 0; i < array_size; i++) {
        SHR_IF_ERR_VERBOSE_EXIT
            (bcm56880_a0_gene_fill_field(&hndl, type, ctrl_type[i],
                                         value[i] & 0xffffffff));
    }

    SHR_IF_ERR_VERBOSE_EXIT
        (bcm56880_a0_gene_fill_opcode(&hndl, BCMLT_OPCODE_UPDATE));
    SHR_IF_ERR_VERBOSE_EXIT
        (bcm56880_a0_gene_commit(&hndl));

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56880_a0_ltsw_hos_profile_get(
    int unit,
    int profile_id,
    bcmi_ltsw_hos_type_t type,
    bcmi_ltsw_hos_profile_control_t ctrl_type,
    int *value)
{
    bcmgene_desc_t *gene = bcm56880_a0_hos_gene[unit];
    bcmgene_handle_t hndl;
    uint64_t value_tmp;
    uint32_t sub_f;

    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (hos_sub_feature_get(unit, type, &sub_f));

    SHR_IF_ERR_VERBOSE_EXIT
        (bcm56880_a0_gene_targeting(unit, gene,
                                    BCMGENE_HASH_OUTPUT_SELECTION_F_PROFILE,
                                    sub_f,
                                    &hndl));
    SHR_IF_ERR_VERBOSE_EXIT
        (bcm56880_a0_gene_handle_create(&hndl));
    SHR_IF_ERR_VERBOSE_EXIT
        (bcm56880_a0_gene_fill_key(&hndl, 0, profile_id));
    SHR_IF_ERR_VERBOSE_EXIT
        (bcm56880_a0_gene_lookup(&hndl));
    SHR_IF_ERR_VERBOSE_EXIT
        (bcm56880_a0_gene_lookup_field(&hndl, type, ctrl_type, &value_tmp));

    *value = value_tmp;

    SHR_IF_ERR_VERBOSE_EXIT
        (bcm56880_a0_gene_lookup_done(&hndl));

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56880_a0_ltsw_hos_instance_info_get(
    int unit,
    bcmi_ltsw_hos_type_t type,
    int *inst_count,
    int *flow_id_count)
{
    SHR_FUNC_ENTER(unit);

    if (inst_count) {
        SHR_IF_ERR_VERBOSE_EXIT
            (hos_info_get(unit, bcmiHosInfoNumInstances, inst_count));
    }
    if (flow_id_count) {
        SHR_IF_ERR_VERBOSE_EXIT
            (hos_info_get(unit, bcmiHosInfoNumFlowIds, flow_id_count));
    }

    SHR_EXIT();

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56880_a0_ltsw_hos_instance_offset_max_get(
    int unit,
    bcmi_ltsw_hos_type_t type,
    int concat,
    int *max_offset)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (hos_instance_offset_max_get(unit, type, concat, max_offset));

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56880_a0_ltsw_hos_instance_offset_decode(
    int unit,
    bcmi_ltsw_hos_type_t type,
    int concat,
    int offset,
    int *sub_sel,
    int *sub_offset)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (hos_instance_offset_decode(unit, type, concat, offset, sub_sel, sub_offset));

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56880_a0_ltsw_hos_instance_offset_encode(
    int unit,
    bcmi_ltsw_hos_type_t type,
    int concat,
    int sub_sel,
    int sub_offset,
    int *offset)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (hos_instance_offset_encode(unit, type, concat, sub_sel, sub_offset, offset));

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56880_a0_ltsw_hos_instance_offset_seed_set(
    int unit,
    bcmi_ltsw_hos_type_t type,
    int inst_id,
    int concat,
    uint32_t seed)
{
    bcmgene_desc_t *gene = bcm56880_a0_hos_gene[unit];
    bcmgene_handle_t hndl;
    uint32_t sub_f;
    int flow_id, flow_id_count, random, sub_sel, sub_offset;

    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (hos_sub_feature_get(unit, type, &sub_f));

    SHR_IF_ERR_VERBOSE_EXIT
        (bcm56880_a0_gene_targeting(unit, gene,
                                    BCMGENE_HASH_OUTPUT_SELECTION_F_INSTANCE,
                                    sub_f,
                                    &hndl));
    SHR_IF_ERR_VERBOSE_EXIT
        (hos_info_get(unit, bcmiHosInfoNumFlowIds, &flow_id_count));

    SHR_IF_ERR_VERBOSE_EXIT
        (bcm56880_a0_gene_batch_handle_create(&hndl));

    sal_srand(seed);

    for (flow_id = 0; flow_id < flow_id_count; flow_id++) {
        /* Pick low 7 bits. */
        random = sal_rand() & 0x7F;
        if (concat) {
            sub_sel = 0;
            sub_offset = random & 0x3F;
        } else {
            sub_sel = (random >> 4) % HOS_SUB_SEL_COUNT;
            sub_offset = random & 0xF;
        }
        SHR_IF_ERR_VERBOSE_EXIT
            (bcm56880_a0_gene_handle_create(&hndl));
        SHR_IF_ERR_VERBOSE_EXIT
            (bcm56880_a0_gene_fill_key(&hndl, 0, inst_id));
        SHR_IF_ERR_VERBOSE_EXIT
            (bcm56880_a0_gene_fill_key(&hndl, 1, flow_id));

        SHR_IF_ERR_VERBOSE_EXIT
            (bcm56880_a0_gene_fill_field(&hndl,
                                         0, bcmiHosInstaceControlSubSelect,
                                         sub_sel));
        SHR_IF_ERR_VERBOSE_EXIT
            (bcm56880_a0_gene_fill_field(&hndl,
                                         0, bcmiHosInstaceControlOffset,
                                         sub_offset));
        SHR_IF_ERR_VERBOSE_EXIT
            (bcm56880_a0_gene_fill_opcode(&hndl, BCMLT_OPCODE_UPDATE));
        SHR_IF_ERR_VERBOSE_EXIT
            (bcm56880_a0_gene_fill_batch_handle(&hndl));
    }

    SHR_IF_ERR_VERBOSE_EXIT
        (bcm56880_a0_gene_commit_batch(&hndl));

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56880_a0_ltsw_hos_instance_offset_stride_set(
    int unit,
    bcmi_ltsw_hos_type_t type,
    int inst_id,
    int concat,
    int min,
    int max,
    int stride)
{
    bcmgene_desc_t *gene = bcm56880_a0_hos_gene[unit];
    bcmgene_handle_t hndl;
    uint32_t sub_f;
    int flow_id, flow_id_count, hash_offset, sub_sel, sub_offset;
    int rv;

    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (hos_sub_feature_get(unit, type, &sub_f));

    SHR_IF_ERR_VERBOSE_EXIT
        (bcm56880_a0_gene_targeting(unit, gene,
                                    BCMGENE_HASH_OUTPUT_SELECTION_F_INSTANCE,
                                    sub_f,
                                    &hndl));
    SHR_IF_ERR_VERBOSE_EXIT
        (hos_info_get(unit, bcmiHosInfoNumFlowIds, &flow_id_count));

    SHR_IF_ERR_VERBOSE_EXIT
        (bcm56880_a0_gene_batch_handle_create(&hndl));

    hash_offset = min;

    for (flow_id = 0; flow_id < flow_id_count; flow_id++) {
        rv = hos_instance_offset_decode(unit, type, concat, hash_offset,
                                        &sub_sel, &sub_offset);
        SHR_IF_ERR_VERBOSE_EXIT
            (bcm56880_a0_gene_error_return(&hndl, rv));
        SHR_IF_ERR_VERBOSE_EXIT
            (bcm56880_a0_gene_handle_create(&hndl));
        SHR_IF_ERR_VERBOSE_EXIT
            (bcm56880_a0_gene_fill_key(&hndl, 0, inst_id));
        SHR_IF_ERR_VERBOSE_EXIT
            (bcm56880_a0_gene_fill_key(&hndl, 1, flow_id));
        SHR_IF_ERR_VERBOSE_EXIT
            (bcm56880_a0_gene_fill_field(&hndl,
                                         0, bcmiHosInstaceControlSubSelect,
                                         sub_sel));
        SHR_IF_ERR_VERBOSE_EXIT
            (bcm56880_a0_gene_fill_field(&hndl,
                                         0, bcmiHosInstaceControlOffset,
                                         sub_offset));
        SHR_IF_ERR_VERBOSE_EXIT
            (bcm56880_a0_gene_fill_opcode(&hndl, BCMLT_OPCODE_UPDATE));
        SHR_IF_ERR_VERBOSE_EXIT
            (bcm56880_a0_gene_fill_batch_handle(&hndl));
        hash_offset += stride;
        if (hash_offset > max) {
            hash_offset = min;
        }
    }

    SHR_IF_ERR_VERBOSE_EXIT
        (bcm56880_a0_gene_commit_batch(&hndl));

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56880_a0_ltsw_hos_instance_set(
    int unit,
    bcmi_ltsw_hos_type_t type,
    int inst_id,
    int flow_id,
    int array_size,
    bcmi_ltsw_hos_instance_control_t *ctrl_type,
    int *value)
{
    bcmgene_desc_t *gene = bcm56880_a0_hos_gene[unit];
    bcmgene_handle_t hndl;
    int i;
    uint32_t sub_f;
    int flow_id_count;

    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (hos_sub_feature_get(unit, type, &sub_f));

    SHR_IF_ERR_VERBOSE_EXIT
        (bcm56880_a0_gene_targeting(unit, gene,
                                    BCMGENE_HASH_OUTPUT_SELECTION_F_INSTANCE,
                                    sub_f,
                                    &hndl));
    SHR_IF_ERR_VERBOSE_EXIT
        (hos_info_get(unit, bcmiHosInfoNumFlowIds, &flow_id_count));

    if (flow_id > 0 && flow_id < flow_id_count) {
        SHR_IF_ERR_VERBOSE_EXIT
            (bcm56880_a0_gene_handle_create(&hndl));
        SHR_IF_ERR_VERBOSE_EXIT
            (bcm56880_a0_gene_fill_key(&hndl, 0, inst_id));
        SHR_IF_ERR_VERBOSE_EXIT
            (bcm56880_a0_gene_fill_key(&hndl, 1, flow_id));
        for (i = 0; i < array_size; i++) {
            SHR_IF_ERR_VERBOSE_EXIT
                (bcm56880_a0_gene_fill_field(&hndl, 0, ctrl_type[i],
                                             value[i]));
        }
        SHR_IF_ERR_VERBOSE_EXIT
            (bcm56880_a0_gene_fill_opcode(&hndl, BCMLT_OPCODE_UPDATE));
        SHR_IF_ERR_VERBOSE_EXIT
            (bcm56880_a0_gene_commit(&hndl));
    } else if (flow_id == -1) {
        SHR_IF_ERR_VERBOSE_EXIT
            (bcm56880_a0_gene_batch_handle_create(&hndl));
        for (flow_id = 0; flow_id < flow_id_count; flow_id++) {
            SHR_IF_ERR_VERBOSE_EXIT
                (bcm56880_a0_gene_handle_create(&hndl));
            SHR_IF_ERR_VERBOSE_EXIT
                (bcm56880_a0_gene_fill_key(&hndl, 0, inst_id));
            SHR_IF_ERR_VERBOSE_EXIT
                (bcm56880_a0_gene_fill_key(&hndl, 1, flow_id));
            for (i = 0; i < array_size; i++) {
                SHR_IF_ERR_VERBOSE_EXIT
                    (bcm56880_a0_gene_fill_field(&hndl, 0, ctrl_type[i],
                                                 value[i]));
            }
            SHR_IF_ERR_VERBOSE_EXIT
                (bcm56880_a0_gene_fill_opcode(&hndl, BCMLT_OPCODE_UPDATE));
            SHR_IF_ERR_VERBOSE_EXIT
                (bcm56880_a0_gene_fill_batch_handle(&hndl));
        }
        SHR_IF_ERR_VERBOSE_EXIT
            (bcm56880_a0_gene_commit_batch(&hndl));
    } else {
        SHR_ERR_EXIT(SHR_E_PARAM);
    }

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56880_a0_ltsw_hos_instance_get(
    int unit,
    bcmi_ltsw_hos_type_t type,
    int inst_id,
    int flow_id,
    int array_size,
    bcmi_ltsw_hos_instance_control_t *ctrl_type,
    int *value)
{
    bcmgene_desc_t *gene = bcm56880_a0_hos_gene[unit];
    bcmgene_handle_t hndl;
    int i;
    uint32_t sub_f;
    uint64_t value_tmp;

    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (hos_sub_feature_get(unit, type, &sub_f));
    SHR_IF_ERR_VERBOSE_EXIT
        (bcm56880_a0_gene_targeting(unit, gene,
                                    BCMGENE_HASH_OUTPUT_SELECTION_F_INSTANCE,
                                    sub_f,
                                    &hndl));
    SHR_IF_ERR_VERBOSE_EXIT
        (bcm56880_a0_gene_handle_create(&hndl));
    SHR_IF_ERR_VERBOSE_EXIT
        (bcm56880_a0_gene_fill_key(&hndl, 0, inst_id));
    SHR_IF_ERR_VERBOSE_EXIT
        (bcm56880_a0_gene_fill_key(&hndl, 1, flow_id));
    SHR_IF_ERR_VERBOSE_EXIT
        (bcm56880_a0_gene_lookup(&hndl));
    for (i = 0; i < array_size; i++) {
        SHR_IF_ERR_VERBOSE_EXIT
            (bcm56880_a0_gene_lookup_field(&hndl, 0, ctrl_type[i], &value_tmp));
        value[i] = value_tmp & 0xffffffff;
    }
    SHR_IF_ERR_VERBOSE_EXIT
        (bcm56880_a0_gene_lookup_done(&hndl));

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56880_a0_ltsw_hos_db_init(
    int unit,
    int warm)
{
    bcmgene_desc_t *gene = NULL;

    SHR_FUNC_ENTER(unit);

    gene = bcm56880_a0_gene_extractor(unit, "HashOutputSelection");

    if (!gene) {
        SHR_VERBOSE_EXIT(SHR_E_UNAVAIL);
    }

    bcm56880_a0_hos_gene[unit] = gene;

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56880_a0_ltsw_hos_db_deinit(int unit)
{
    SHR_FUNC_ENTER(unit);

    if (bcm56880_a0_hos_gene[unit]) {
        bcm56880_a0_hos_gene[unit] = NULL;
    }
    SHR_EXIT();

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56880_a0_ltsw_hos_init(int unit)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (hos_init_clear_hw(unit));

    SHR_IF_ERR_VERBOSE_EXIT
        (hos_instance_init(unit));
exit:
    SHR_FUNC_EXIT();
}

static int
bcm56880_a0_ltsw_hos_deinit(int unit)
{
    SHR_FUNC_ENTER(unit);

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT();
}

/*!
 * \brief Hos driver function variable for bcm56880_a0 device.
 */
static mbcm_ltsw_hos_drv_t bcm56880_a0_ltsw_hos_drv = {
    .hos_info_get = bcm56880_a0_ltsw_hos_info_get,
    .hos_validate_profile_params = bcm56880_a0_ltsw_hos_validate_profile_params,
    .hos_validate_instance_params = bcm56880_a0_ltsw_hos_validate_instance_params,
    .hos_profile_create = bcm56880_a0_ltsw_hos_profile_create,
    .hos_profile_destroy = bcm56880_a0_ltsw_hos_profile_destroy,
    .hos_profile_configured = bcm56880_a0_ltsw_hos_profile_configured,
    .hos_profile_set = bcm56880_a0_ltsw_hos_profile_set,
    .hos_profile_get = bcm56880_a0_ltsw_hos_profile_get,
    .hos_instance_info_get = bcm56880_a0_ltsw_hos_instance_info_get,
    .hos_instance_offset_max_get = bcm56880_a0_ltsw_hos_instance_offset_max_get,
    .hos_instance_offset_decode = bcm56880_a0_ltsw_hos_instance_offset_decode,
    .hos_instance_offset_encode = bcm56880_a0_ltsw_hos_instance_offset_encode,
    .hos_instance_offset_seed_set = bcm56880_a0_ltsw_hos_instance_offset_seed_set,
    .hos_instance_offset_stride_set = bcm56880_a0_ltsw_hos_instance_offset_stride_set,
    .hos_instance_set = bcm56880_a0_ltsw_hos_instance_set,
    .hos_instance_get = bcm56880_a0_ltsw_hos_instance_get,
    .hos_db_init = bcm56880_a0_ltsw_hos_db_init,
    .hos_db_deinit = bcm56880_a0_ltsw_hos_db_deinit,
    .hos_init = bcm56880_a0_ltsw_hos_init,
    .hos_deinit = bcm56880_a0_ltsw_hos_deinit
};

/******************************************************************************
 * Public functions
 */

int
bcm56880_a0_ltsw_hos_drv_attach(int unit)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_hos_drv_set(unit, &bcm56880_a0_ltsw_hos_drv));

exit:
    SHR_FUNC_EXIT();
}
