/*
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated.
 * Edits to this file will be lost when it is regenerated.
 *
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

#if defined(INCLUDE_INT)

#include <bcm_int/control.h>

#include <bcm_int/ltsw/xfs/int.h>
#include <bcm_int/ltsw/mbcm/int.h>
#include <bcm_int/ltsw/dev.h>
#include <bcm_int/ltsw/lt_intf.h>
#include <bcm_int/ltsw/cosq_thd.h>

#include <bcmltd/chip/bcmltd_str.h>
#include <shr/shr_types.h>
#include <shr/shr_debug.h>

/******************************************************************************
 * Local definitions
 */

#define BSL_LOG_MODULE BSL_LS_BCM_INT

/******************************************************************************
 * Private functions
 */


typedef struct int_switch_control_s {
    const char *ltname;
    const char *field_str;
} int_switch_control_t;

static int_switch_control_t int_sc_info[bcmiIntControlCount] =
{
    [bcmiIntControlEgressTimeDelta] = {DEVICE_TS_CONTROLs, LATENCY_ADJUSTs},
};

/*!
 * \brief Set int controls.
 *
 * \param [in] unit Unit Number.
 * \param [in] control Enum value of the INT control.
 * \param [in] value Value to set.
 *
 * \retval SHR_E_NONE on success and error code otherwise.
 */
static int
int_control_set(
    int unit,
    bcmi_ltsw_int_control_t control,
    uint32_t value)
{
    bcmlt_entry_handle_t entry_hdl = BCMLT_INVALID_HDL;
    int dunit, rv;

    SHR_FUNC_ENTER(unit);

    dunit = bcmi_ltsw_dev_dunit(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (bcmlt_entry_allocate(dunit, int_sc_info[control].ltname, &entry_hdl));

    rv = bcmlt_entry_field_add(entry_hdl, int_sc_info[control].field_str,
                               value);
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
 * \brief get int controls.
 *
 * \param [in] unit Unit Number.
 * \param [in] control Enum value of the INT control.
 * \param [out] value Get value.
 *
 * \retval SHR_E_NONE on success and error code otherwise.
 */
static int
int_control_get(
    int unit,
    bcmi_ltsw_int_control_t control,
    uint32_t *value)
{
    bcmlt_entry_handle_t entry_hdl = BCMLT_INVALID_HDL;
    bcmlt_field_def_t fld_def;
    uint64_t field_value;
    int dunit, rv;

    SHR_FUNC_ENTER(unit);

    dunit = bcmi_ltsw_dev_dunit(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (bcmlt_entry_allocate(dunit, int_sc_info[control].ltname, &entry_hdl));

    rv = bcmi_lt_entry_commit(unit, entry_hdl, BCMLT_OPCODE_LOOKUP,
                              BCMLT_PRIORITY_NORMAL);
    if (rv == SHR_E_NOT_FOUND) {
        SHR_IF_ERR_VERBOSE_EXIT
            (bcmi_lt_field_def_get(unit, int_sc_info[control].ltname,
                                   int_sc_info[control].field_str,
                                   &fld_def));
        *value = fld_def.def;
        SHR_EXIT();
    }

    rv = bcmlt_entry_field_get(entry_hdl, int_sc_info[control].field_str,
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
 * \brief Check INT cosq config parameter.
 *
 * \param [in] unit Unit Number.
 * \param [in] profile Action profile.
 *
 * \retval SHR_E_NONE on success and error code otherwise.
 */
static int
int_cosq_stat_config_check(
    int unit,
    bcm_int_cosq_stat_config_t *config)
{
    SHR_FUNC_ENTER(unit);

    SHR_NULL_CHECK(config, SHR_E_PARAM);

    /* It's fixed as Queue Based mode. */
    if ((config->report_mode != bcmIntCosqStatReportQueueBased)) {
        SHR_ERR_EXIT(SHR_E_PARAM);
    }

exit:
    SHR_FUNC_EXIT();
}

/*!
 * \brief Insert MON_INBAND_TELEMETRY_TM_STATS_CONTROL entries into LT.
 *
 * \param [in] unit Unit Number
 * \param [in] config Cosq stat configurations.
 *
 * \return SHR_E_NONE on success and error code otherwise.
 */
static int
int_cosq_stat_config_set(
    int unit,
    bcm_int_cosq_stat_config_t *config)
{
    bcmlt_entry_handle_t entry_hdl = BCMLT_INVALID_HDL;
    int dunit = 0, cells = 0;

    SHR_FUNC_ENTER(unit);

    /* Validate action parameter. */
    SHR_IF_ERR_VERBOSE_EXIT
        (int_cosq_stat_config_check(unit, config));

    dunit = bcmi_ltsw_dev_dunit(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (bcmlt_entry_allocate(dunit, MON_INBAND_TELEMETRY_TM_STATS_CONTROLs,
                              &entry_hdl));

    /*  Add MON_INBAND_TELEMETRY_TM_STATS_CONTROL fields. */
    SHR_IF_ERR_VERBOSE_EXIT
        (bcmlt_entry_field_add(entry_hdl, USE_DYNAMIC_MAX_USAGE_CELLSs,
                               config->max_bytes_enable ? 1 : 0));

    SHR_IF_ERR_VERBOSE_EXIT
        (bcmi_ltsw_cosq_thd_byte_to_cell(unit, config->max_bytes, &cells));
    SHR_IF_ERR_VERBOSE_EXIT
        (bcmlt_entry_field_add(entry_hdl, MAX_USAGE_CELLSs, cells));

    SHR_IF_ERR_VERBOSE_EXIT
        (bcmi_ltsw_cosq_thd_byte_to_cell(unit, config->current_bytes, &cells));
    SHR_IF_ERR_VERBOSE_EXIT
        (bcmlt_entry_field_add(entry_hdl, CURRENT_USAGE_CELLSs, cells));

    SHR_IF_ERR_VERBOSE_EXIT
        (bcmi_ltsw_cosq_thd_byte_to_cell(unit, config->current_available_bytes,
                                         &cells));
    SHR_IF_ERR_VERBOSE_EXIT
        (bcmlt_entry_field_add(entry_hdl, CURRENT_AVAILABLE_CELLSs, cells));

    SHR_IF_ERR_VERBOSE_EXIT
        (bcmi_ltsw_cosq_thd_byte_to_cell(unit, config->min_available_bytes,
                                         &cells));
    SHR_IF_ERR_VERBOSE_EXIT
        (bcmlt_entry_field_add(entry_hdl, MIN_AVAILABLE_CELLSs, cells));

    /* Operation commit. */
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
 * \brief Insert MON_INBAND_TELEMETRY_TM_STATS_CONTROL entries into LT.
 *
 * \param [in] unit Unit Number
 * \param [out] config Cosq stat configurations.
 *
 * \return SHR_E_NONE on success and error code otherwise.
 */
static int
int_cosq_stat_config_get(
    int unit,
    bcm_int_cosq_stat_config_t *config)
{
    bcmlt_entry_handle_t entry_hdl = BCMLT_INVALID_HDL;
    const char *lt_name = MON_INBAND_TELEMETRY_TM_STATS_CONTROLs;
    bcmlt_field_def_t fld_def;
    int dunit = 0, rv, bytes = 0;
    uint64_t value = 0;

    SHR_FUNC_ENTER(unit);

    SHR_NULL_CHECK(config, SHR_E_PARAM);

    dunit = bcmi_ltsw_dev_dunit(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (bcmlt_entry_allocate(dunit, lt_name, &entry_hdl));

    rv = bcmi_lt_entry_commit(unit, entry_hdl, BCMLT_OPCODE_LOOKUP,
                              BCMLT_PRIORITY_NORMAL);

    if (rv == SHR_E_NOT_FOUND) {
        SHR_IF_ERR_VERBOSE_EXIT
            (bcmi_lt_field_def_get(unit, lt_name, USE_DYNAMIC_MAX_USAGE_CELLSs,
                                   &fld_def));
        config->max_bytes_enable = fld_def.def;

        SHR_IF_ERR_VERBOSE_EXIT
            (bcmi_lt_field_def_get(unit, lt_name, MAX_USAGE_CELLSs,
                                   &fld_def));
        SHR_IF_ERR_VERBOSE_EXIT
           (bcmi_ltsw_cosq_thd_cell_to_byte(unit, fld_def.def, &bytes));
        config->max_bytes = bytes;

        SHR_IF_ERR_VERBOSE_EXIT
            (bcmi_lt_field_def_get(unit, lt_name, CURRENT_USAGE_CELLSs,
                                   &fld_def));
        SHR_IF_ERR_VERBOSE_EXIT
           (bcmi_ltsw_cosq_thd_cell_to_byte(unit, fld_def.def, &bytes));
        config->current_bytes = bytes;

        SHR_IF_ERR_VERBOSE_EXIT
            (bcmi_lt_field_def_get(unit, lt_name, CURRENT_AVAILABLE_CELLSs,
                                   &fld_def));
        SHR_IF_ERR_VERBOSE_EXIT
           (bcmi_ltsw_cosq_thd_cell_to_byte(unit, fld_def.def, &bytes));
        config->current_available_bytes = bytes;

        SHR_IF_ERR_VERBOSE_EXIT
            (bcmi_lt_field_def_get(unit, lt_name, MIN_AVAILABLE_CELLSs,
                                   &fld_def));
        SHR_IF_ERR_VERBOSE_EXIT
           (bcmi_ltsw_cosq_thd_cell_to_byte(unit, fld_def.def, &bytes));
        config->min_available_bytes = bytes;

        /* It's fixed as Queue Based mode. */
        config->report_mode = bcmIntCosqStatReportQueueBased;
        SHR_EXIT();
    }

    SHR_IF_ERR_VERBOSE_EXIT
        (bcmlt_entry_field_get(entry_hdl, USE_DYNAMIC_MAX_USAGE_CELLSs, &value));
    config->max_bytes_enable = value;

    SHR_IF_ERR_VERBOSE_EXIT
        (bcmlt_entry_field_get(entry_hdl, MAX_USAGE_CELLSs, &value));
    SHR_IF_ERR_VERBOSE_EXIT
       (bcmi_ltsw_cosq_thd_cell_to_byte(unit, value, &bytes));
    config->max_bytes = bytes;

    SHR_IF_ERR_VERBOSE_EXIT
        (bcmlt_entry_field_get(entry_hdl, CURRENT_USAGE_CELLSs, &value));
    SHR_IF_ERR_VERBOSE_EXIT
       (bcmi_ltsw_cosq_thd_cell_to_byte(unit, value, &bytes));
    config->current_bytes = bytes;

    SHR_IF_ERR_VERBOSE_EXIT
        (bcmlt_entry_field_get(entry_hdl, CURRENT_AVAILABLE_CELLSs, &value));
    SHR_IF_ERR_VERBOSE_EXIT
       (bcmi_ltsw_cosq_thd_cell_to_byte(unit, value, &bytes));
    config->current_available_bytes = bytes;

    SHR_IF_ERR_VERBOSE_EXIT
        (bcmlt_entry_field_get(entry_hdl, MIN_AVAILABLE_CELLSs, &value));
    SHR_IF_ERR_VERBOSE_EXIT
       (bcmi_ltsw_cosq_thd_cell_to_byte(unit, value, &bytes));
    config->min_available_bytes = bytes;

    /* It's fixed as Queue Based mode. */
    config->report_mode = bcmIntCosqStatReportQueueBased;

exit:
    if (entry_hdl != BCMLT_INVALID_HDL) {
        (void)bcmlt_entry_free(entry_hdl);
        entry_hdl = BCMLT_INVALID_HDL;
    }
    SHR_FUNC_EXIT();
}


/******************************************************************************
 * Public functions
 */

int
xfs_ltsw_int_init(int unit)
{
    int warmboot; /* during warmboot */

    SHR_FUNC_ENTER(unit);

    warmboot = bcmi_warmboot_get(unit);

    if (!warmboot) {
        /* Reset all LTs */
        SHR_IF_ERR_VERBOSE_EXIT
            (bcmi_lt_clear(unit, MON_INBAND_TELEMETRY_TM_STATS_CONTROLs));
    }

exit:
    if (SHR_FUNC_ERR()) {
        SHR_IF_ERR_CONT
            (xfs_ltsw_int_detach(unit));
    }
    SHR_FUNC_EXIT();
}

int
xfs_ltsw_int_detach(int unit)
{
    return SHR_E_NONE;
}

int
xfs_ltsw_int_control_set(
    int unit,
    bcmi_ltsw_int_control_t control,
    uint32_t value)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (int_control_set(unit, control, value));

exit:
    SHR_FUNC_EXIT();
}

int
xfs_ltsw_int_control_get(
    int unit,
    bcmi_ltsw_int_control_t control,
    uint32_t *value)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (int_control_get(unit, control, value));

exit:
    SHR_FUNC_EXIT();
}

int
xfs_ltsw_int_cosq_stat_config_set(
    int unit,
    bcm_int_cosq_stat_config_t *config)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (int_cosq_stat_config_set(unit, config));

exit:
    SHR_FUNC_EXIT();
}

int
xfs_ltsw_int_cosq_stat_config_get(
    int unit,
    bcm_int_cosq_stat_config_t *config)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (int_cosq_stat_config_get(unit, config));

exit:
    SHR_FUNC_EXIT();
}

#endif /* INCLUDE_INT */

