/*
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated.
 * Edits to this file will be lost when it is regenerated.
 *
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

#include <bcm_int/control.h>
#include <bcm_int/ltsw_dispatch.h>

#include <bcm_int/ltsw/xfs/ifa.h>
#include <bcm_int/ltsw/ifa.h>
#include <bcm_int/ltsw/ifa_int.h>
#include <bcm_int/ltsw/mbcm/ifa.h>
#include <bcm_int/ltsw/dev.h>
#include <bcm_int/ltsw/lt_intf.h>
#include <bcm_int/ltsw/flexstate.h>
#include <bcm_int/ltsw/ha.h>
#include <bcm_int/ltsw/generated/ifa_ha.h>
#include <bcm_int/ltsw/issu.h>

#include <bcmltd/chip/bcmltd_str.h>
#include <shr/shr_types.h>
#include <shr/shr_bitop.h>
#include <shr/shr_debug.h>

/******************************************************************************
 * Local definitions
 */

#define BSL_LOG_MODULE BSL_LS_BCM_IFA

/******************************************************************************
 * Private functions
 */


#define IFA_TX_BYTE_FLEX_STAT_ID 2

/* Default IFA protocol in PT. */
#define IFA_PROTOCOL_NUM_DEFAULT 0xfd

static bcmint_ifa_ha_db_t xfs_ltsw_ifa_info[BCM_MAX_NUM_UNITS] = {{ 0 }};

#define XFS_IFA_INFO(unit) (&xfs_ltsw_ifa_info[unit])

#define XFS_IFA_TX_BYTE_FLEX_STAT_ID(unit, pipe) \
            (XFS_IFA_INFO(unit)[pipe].tx_byte_flex_stat_id)

/*!
 * \brief Set IFA protocol number.
 *
 * \param [in] unit Unit Number.
 * \param [in] value Value to set.
 *
 * \retval SHR_E_NONE on success and error code otherwise.
 */
static int
ifa_control_ifa_protocol_set(int unit, uint32_t value)
{
    bcmlt_entry_handle_t entry_hdl = BCMLT_INVALID_HDL;
    const bcmint_ifa_lt_t *lt_info;
    const bcmint_ifa_fld_t *flds;
    const char *fld_name;
    int dunit, rv, fid;

    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (bcmint_ifa_lt_get(unit, BCMINT_LTSW_IFA_LT_R_IFA_2_PROTOCOL_NUMBER,
                           &lt_info););

    dunit = bcmi_ltsw_dev_dunit(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (bcmlt_entry_allocate(dunit, lt_info->name, &entry_hdl));

    flds = lt_info->flds;
    fid = BCMINT_LTSW_IFA_FLD_R_IFA_2_PROTOCOL_NUMBER_VALUE;
    fld_name = flds[fid].name;

    rv = bcmlt_entry_field_add(entry_hdl, fld_name, value);
    if (rv == SHR_E_NOT_FOUND) {
        SHR_ERR_EXIT(SHR_E_UNAVAIL);
    } else if (SHR_FAILURE(rv)){
        SHR_ERR_EXIT(rv);
    }

    SHR_IF_ERR_VERBOSE_EXIT
        (bcmi_lt_entry_set_commit(unit, entry_hdl, BCMLT_PRIORITY_NORMAL));

exit:
    if (entry_hdl != BCMLT_INVALID_HDL) {
        (void)bcmlt_entry_free(entry_hdl);
        entry_hdl = BCMLT_INVALID_HDL;
    }
    SHR_FUNC_EXIT();
}

/*!
 * \brief Get IFA protocol number.
 *
 * \param [in] unit Unit Number.
 * \param [out] value Get value.
 *
 * \retval SHR_E_NONE on success and error code otherwise.
 */
static int
ifa_control_ifa_protocol_get(int unit, uint32_t *value)
{
    bcmlt_entry_handle_t entry_hdl = BCMLT_INVALID_HDL;
    const bcmint_ifa_lt_t *lt_info;
    const bcmint_ifa_fld_t *flds;
    const char *fld_name;
    bcmlt_field_def_t fld_def;
    uint64_t field_value;
    int dunit, rv, fid;

    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (bcmint_ifa_lt_get(unit, BCMINT_LTSW_IFA_LT_R_IFA_2_PROTOCOL_NUMBER,
                           &lt_info););

    dunit = bcmi_ltsw_dev_dunit(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (bcmlt_entry_allocate(dunit, lt_info->name, &entry_hdl));

    flds = lt_info->flds;
    fid = BCMINT_LTSW_IFA_FLD_R_IFA_2_PROTOCOL_NUMBER_VALUE;
    fld_name = flds[fid].name;

    rv = bcmi_lt_entry_commit(unit, entry_hdl, BCMLT_OPCODE_LOOKUP,
                              BCMLT_PRIORITY_NORMAL);
    if (rv == SHR_E_NOT_FOUND) {
        SHR_IF_ERR_VERBOSE_EXIT
            (bcmi_lt_field_def_get(unit, lt_info->name,
                                   fld_name, &fld_def));
        *value = fld_def.def;
        SHR_EXIT();
    }

    rv = bcmlt_entry_field_get(entry_hdl, fld_name,
                               &field_value);
    if (rv == SHR_E_NOT_FOUND) {
        SHR_ERR_EXIT(SHR_E_UNAVAIL);
    } else if (SHR_FAILURE(rv)){
        SHR_ERR_EXIT(rv);
    }
    *value = field_value;

exit:
    if (entry_hdl != BCMLT_INVALID_HDL) {
        (void)bcmlt_entry_free(entry_hdl);
        entry_hdl = BCMLT_INVALID_HDL;
    }
    SHR_FUNC_EXIT();
}

/*!
 * \brief Initialize metadata length.
 *
 * \param [in] unit Unit Number.
 *
 * \retval SHR_E_NONE on success and error code otherwise.
 */
static int
ifa_metadata_length_init(int unit)
{
    bcmlt_entry_handle_t entry_hdl = BCMLT_INVALID_HDL;
    const bcmint_ifa_lt_t *lt_info;
    const char *fld_name;
    const bcmint_ifa_fld_t *flds;
    uint64_t idx_min, idx_max;
    int dunit = 0, idx, fid, rv;

    SHR_FUNC_ENTER(unit);

    rv = bcmint_ifa_lt_get(unit,
                           BCMINT_LTSW_IFA_LT_EGR_IFA_ATTRIBUTES_2, &lt_info);
    if (rv == SHR_E_UNAVAIL) {
        return SHR_E_NONE;
    }

    dunit = bcmi_ltsw_dev_dunit(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (bcmlt_entry_allocate(dunit, lt_info->name, &entry_hdl));

    flds = lt_info->flds;
    fid = BCMINT_LTSW_IFA_FLD_EGR_IFA_ATTR_2_IFA_REQUEST_VECTOR;
    fld_name = flds[fid].name;
    SHR_IF_ERR_VERBOSE_EXIT
        (bcmi_lt_field_value_range_get(unit, lt_info->name, fld_name,
                                       &idx_min, &idx_max));

    for (idx = idx_min; idx <= idx_max; idx++) {
        fid = BCMINT_LTSW_IFA_FLD_EGR_IFA_ATTR_2_IFA_REQUEST_VECTOR;
        fld_name = flds[fid].name;
        SHR_IF_ERR_VERBOSE_EXIT
            (bcmlt_entry_field_add(entry_hdl, fld_name, idx));

        /* Temporarily set length as 8 since request vector is fix as 0xFF. */
        fid = BCMINT_LTSW_IFA_FLD_EGR_IFA_ATTR_2_IFA_NODE_LENGTH;
        fld_name = flds[fid].name;
        SHR_IF_ERR_VERBOSE_EXIT
            (bcmlt_entry_field_add(entry_hdl, fld_name, 8));

        /* Operation commit. */
        SHR_IF_ERR_VERBOSE_EXIT
            (bcmi_lt_entry_set_commit(unit, entry_hdl, BCMLT_PRIORITY_NORMAL));
    }

exit:
    if (entry_hdl != BCMLT_INVALID_HDL) {
        (void)bcmlt_entry_free(entry_hdl);
    }
    SHR_FUNC_EXIT();
}

/*!
 * \brief Initialize tx byte state.
 *
 * \param [in] unit Unit Number.
 *
 * \retval SHR_E_NONE on success and error code otherwise.
 */
static int
ifa_tx_byte_state_init(int unit)
{
    int max_pipe, pipe;
    bcm_flexstate_action_t action = {0};
    uint32_t stat_counter_id = 0;
    bcm_flexstate_index_operation_t *index_op = NULL;
    bcm_flexstate_value_operation_t *value_a_true_op = NULL;
    bcm_flexstate_value_operation_t *value_b_true_op = NULL;
    int options;

    SHR_FUNC_ENTER(unit);

    max_pipe = bcmi_ltsw_dev_max_pp_pipe_num(unit);

    for (pipe = 0; pipe < max_pipe; pipe++) {
        action.flags = BCM_FLEXSTATE_ACTION_FLAGS_HINT_OUTPUT;
        BCM_PBMP_PORT_SET(action.ports, pipe);
        action.source = bcmiFlexstateSourceTxByteState;
        /* Output to flexstate object 0. */
        action.hint = 1;
        action.mode = bcmFlexstateCounterModeNormal;
        action.index_num = 256;

        /* port. */
        index_op = &action.index_operation;
        index_op->object[0] = bcmFlexstateObjectEgrPipePortQueueNum;
        index_op->mask_size[0] = 6;
        index_op->shift[0] = 0;
        index_op->object[1] = bcmFlexstateObjectConstZero;
        index_op->mask_size[1] = 1;
        index_op->shift[1] = 0;

        action.operation_a.update_compare = bcmFlexstateUpdateCompareAlwaysTrue;
        value_a_true_op = &action.operation_a.operation_true;
        value_a_true_op->object[0] = bcmFlexstateObjectEgrPktLenObj;
        value_a_true_op->mask_size[0] = 15;
        value_a_true_op->shift[0] = 0;
        value_a_true_op->object[1] = bcmFlexstateObjectConstZero;
        value_a_true_op->mask_size[1] = 1;
        value_a_true_op->shift[1] = 0;
        value_a_true_op->select = bcmFlexstateValueSelectCounterValue;
        value_a_true_op->type = bcmFlexstateValueOperationTypeInc;

        action.operation_b.update_compare = bcmFlexstateUpdateCompareAlwaysTrue;
        value_b_true_op = &action.operation_b.operation_true;
        value_b_true_op->object[0] = bcmFlexstateObjectConstZero;
        value_b_true_op->mask_size[0] = 1;
        value_b_true_op->shift[0] = 0;
        value_b_true_op->object[1] = bcmFlexstateObjectConstZero;
        value_b_true_op->mask_size[1] = 1;
        value_b_true_op->shift[1] = 0;
        value_b_true_op->select = bcmFlexstateValueSelectCounterValue;
        value_b_true_op->type = bcmFlexstateValueOperationTypeNoop;

        /* Must be the flexstate action 2. */
        options = BCM_FLEXSTATE_OPTIONS_WITH_ID |
                  BCMI_LTSW_FLEXSTATE_OPTIONS_INTERNAL |
                  BCMI_LTSW_FLEXSTATE_OPTIONS_PIPE_BMP;
        stat_counter_id = IFA_TX_BYTE_FLEX_STAT_ID;

        SHR_IF_ERR_VERBOSE_EXIT
            (bcm_ltsw_flexstate_action_create(unit, options, &action,
                                              &stat_counter_id));
        XFS_IFA_TX_BYTE_FLEX_STAT_ID(unit, pipe) = stat_counter_id;
    }

exit:
    SHR_FUNC_EXIT();
}

/*!
 * \brief De-initialize tx byte state.
 *
 * \param [in] unit Unit Number.
 *
 * \retval SHR_E_NONE on success and error code otherwise.
 */
void
ifa_tx_byte_state_deinit(int unit)
{
    int max_pipe, pipe;
    uint32_t stat_counter_id;
    int warmboot;

    warmboot = bcmi_warmboot_get(unit);

    if (!warmboot) {
        max_pipe = bcmi_ltsw_dev_max_pp_pipe_num(unit);
        /* Try to destroy existing flex state action. */
        for (pipe = 0; pipe < max_pipe; pipe++) {
            stat_counter_id = XFS_IFA_TX_BYTE_FLEX_STAT_ID(unit, pipe);
            if (stat_counter_id != BCMI_LTSW_FLEXSTATE_ACTION_INVALID) {
                /*
                 * It is possible that these resources have been freed at flex
                 * stat module init time.
                 */
                (void)bcm_ltsw_flexstate_action_destroy(unit, stat_counter_id);
                XFS_IFA_TX_BYTE_FLEX_STAT_ID(unit, pipe) =
                    BCMI_LTSW_FLEXSTATE_ACTION_INVALID;
            }
        }
    }
}

/*!
 * \brief De-initialize IFA database.
 *
 * \param [in] unit Unit Number.
 *
 * \retval SHR_E_NONE on success and error code otherwise.
 */
void
ifa_db_deinit(int unit)
{
    bcmint_ifa_ha_db_t *ifa_info = NULL;
    int warmboot; /* during warmboot */

    warmboot = bcmi_warmboot_get(unit);

    ifa_info = XFS_IFA_INFO(unit);

    if (!warmboot) {
        if (ifa_info) {
            (void)bcmi_ltsw_ha_mem_free(unit, ifa_info);
        }
    }
}

/*!
 * \brief Initialize IFA database.
 *
 * \param [in] unit Unit Number.
 *
 * \retval SHR_E_NONE on success and error code otherwise.
 */
static int
ifa_db_init(int unit)
{
    bcmint_ifa_ha_db_t *ifa_info = NULL;
    int warmboot; /* during warmboot */
    uint32_t alloc_size = 0, req_size = 0;
    uint32_t ha_instance_size = 0, ha_array_size = 0;
    int rv;

    SHR_FUNC_ENTER(unit);

    warmboot = bcmi_warmboot_get(unit);

    ifa_info = XFS_IFA_INFO(unit);

    /* Init TX byte flex state counter ID. */
    ha_instance_size = sizeof(bcmint_ifa_ha_db_t);
    ha_array_size = bcmi_ltsw_dev_max_pp_pipe_num(unit);
    req_size = ha_instance_size * ha_array_size;
    alloc_size = req_size;
    ifa_info =
        bcmi_ltsw_ha_mem_alloc(unit, BCMI_HA_COMP_ID_IFA,
                               BCMINT_IFA_TX_BYTE_STAT_SUB_COMP_ID,
                               "bcmIfaTxByteStatId",
                               &alloc_size);

    SHR_NULL_CHECK(ifa_info, SHR_E_MEMORY);
    SHR_IF_ERR_VERBOSE_EXIT
        ((alloc_size < req_size) ? SHR_E_MEMORY : SHR_E_NONE);

    if (!warmboot) {
        sal_memset(ifa_info, 0, alloc_size);
    }

    rv = bcmi_ltsw_issu_struct_info_report(unit,
             BCMI_HA_COMP_ID_IFA,
             BCMINT_IFA_TX_BYTE_STAT_SUB_COMP_ID,
             0, ha_instance_size, ha_array_size,
             BCMINT_IFA_HA_DB_T_ID);
    if (rv != SHR_E_EXISTS) {
       SHR_IF_ERR_VERBOSE_EXIT(rv);
    }

exit:
    if (SHR_FUNC_ERR()) {
        ifa_db_deinit(unit);
    }
    SHR_FUNC_EXIT();
}


/******************************************************************************
 * Public functions
 */

int
xfs_ltsw_ifa_init(int unit)
{
    const bcmint_ifa_lt_t *lt_info;
    int rv, warmboot; /* during warmboot */

    SHR_FUNC_ENTER(unit);

    warmboot = bcmi_warmboot_get(unit);

    if (!warmboot) {
        /* Reset all LTs */
        rv = bcmint_ifa_lt_get(unit,
                               BCMINT_LTSW_IFA_LT_EGR_IFA_ATTRIBUTES_2,
                               &lt_info);
        if (SHR_SUCCESS(rv)) {
            SHR_IF_ERR_VERBOSE_EXIT
                (bcmi_lt_clear(unit, lt_info->name));
        }
    }

    /* Initialize IFA database. */
    SHR_IF_ERR_VERBOSE_EXIT(ifa_db_init(unit));

    SHR_IF_ERR_VERBOSE_EXIT
        (ifa_metadata_length_init(unit));

    if (!warmboot) {
        SHR_IF_ERR_VERBOSE_EXIT
            (ifa_tx_byte_state_init(unit));

        /* Set default IFA protocol per PT. */
        rv = ifa_control_ifa_protocol_set(unit, IFA_PROTOCOL_NUM_DEFAULT);
        if (SHR_FAILURE(rv) && (rv != SHR_E_UNAVAIL)) {
            SHR_ERR_EXIT(rv);
        }
    }

exit:
    if (SHR_FUNC_ERR()) {
        SHR_IF_ERR_CONT
            (xfs_ltsw_ifa_detach(unit));
    }
    SHR_FUNC_EXIT();
}

int
xfs_ltsw_ifa_detach(int unit)
{
    const bcmint_ifa_lt_t *lt_info;
    int rv, warmboot;

    SHR_FUNC_ENTER(unit);

    warmboot = bcmi_warmboot_get(unit);
    /* Do not clear LT entries when "exit clean". */
    if (!warmboot) {
        rv = bcmint_ifa_lt_get(unit,
                               BCMINT_LTSW_IFA_LT_EGR_IFA_ATTRIBUTES_2,
                               &lt_info);
        if (SHR_SUCCESS(rv)) {
            SHR_IF_ERR_VERBOSE_EXIT
                (bcmi_lt_clear(unit, lt_info->name));
        }
    }

    ifa_db_deinit(unit);
    ifa_tx_byte_state_deinit(unit);

exit:
    SHR_FUNC_EXIT();
}

int
xfs_ltsw_ifa_control_set(
    int unit,
    bcmi_ltsw_ifa_control_t control,
    uint32_t value)
{
    SHR_FUNC_ENTER(unit);

    switch (control) {
        case bcmiIfaControlIfaProtocol:
            SHR_IF_ERR_VERBOSE_EXIT
                (ifa_control_ifa_protocol_set(unit, value));
            break;
        default:
            SHR_ERR_EXIT(SHR_E_UNAVAIL);
    }

exit:
    SHR_FUNC_EXIT();
}

int
xfs_ltsw_ifa_control_get(
    int unit,
    bcmi_ltsw_ifa_control_t control,
    uint32_t *value)
{
    SHR_FUNC_ENTER(unit);

    switch (control) {
        case bcmiIfaControlIfaProtocol:
            SHR_IF_ERR_VERBOSE_EXIT
                (ifa_control_ifa_protocol_get(unit, value));
            break;
        default:
            SHR_ERR_EXIT(SHR_E_UNAVAIL);
    }

exit:
    SHR_FUNC_EXIT();
}

