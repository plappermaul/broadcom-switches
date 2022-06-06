#ifdef BSL_LOG_MODULE
#error "BSL_LOG_MODULE redefined"
#endif
#define BSL_LOG_MODULE BSL_LS_BCMDNX_FLOW

#include <soc/sand/shrextend/shrextend_debug.h>
#include <bcm_int/dnx/flow/flow.h>
#include "dedicated_logic/flow_dedicated_logic.h"
#include <bcm/qos.h>
#include <bcm/l3.h>
#include <bcm/types.h>
#include <soc/dnx/dbal/dbal.h>
#include <soc/dnx/dnx_err_recovery_manager.h>
#include <bcm_int/dnx/algo/algo_gpm.h>
#include <bcm_int/dnx/field/field_map.h>
#include <bcm_int/dnx/field/field_port.h>
#include <bcm_int/dnx/lif/lif_lib.h>
#include <bcm_int/dnx/lif/in_lif_profile.h>
#include <bcm_int/dnx/qos/qos.h>
#include <sal/appl/sal.h>
#include <shared/utilex/utilex_str.h>
#include <shared/bitop.h>
#include "src/soc/dnx/dbal/dbal_internal.h"
#include <include/bcm_int/dnx/rx/rx_trap.h>
#include <bcm_int/dnx/auto_generated/dnx_flow_dispatch.h>
#include <soc/dnx/swstate/auto_generated/types/flow_types.h>
#include <soc/dnx/swstate/auto_generated/access/flow_access.h>
#include <bcm_int/dnx/algo/swstate/auto_generated/access/dnx_algo_port_access.h>
#include <soc/dnx/dnx_data/auto_generated/dnx_data_port.h>
#include <bcm_int/dnx/flow/flow_lif_mgmt.h>
#include <bcm_int/dnx/flow/flow_virtual_lif_mgmt.h>
#include <soc/dnx/dnx_data/auto_generated/dnx_data_l3.h>
#include <soc/dnx/dnx_data/auto_generated/dnx_data_flow.h>
#include <bcm_int/dnx/algo/l3/source_address_table_allocation.h>
#include <bcm_int/dnx/algo/swstate/auto_generated/access/algo_l3_access.h>
#include <soc/dnx/dnx_data/auto_generated/dnx_data_internal.h>
#include <soc/dnx/dnx_data/auto_generated/dnx_data_internal_flow.h>
#include <soc/dnx/dnx_data/auto_generated/dnx_data_esem.h>
#include <bcm_int/dnx/field/field_entry.h>
#include <soc/dnx/field/tcam/tcam_handler.h>
#include <soc/dnxc/swstate/dnxc_sw_state_utils.h>
#include "src/bcm/dnx/algo/lif_mngr/auto_generated/dbx_pre_compiled_lif_mdb_auto_generated_xml_parser.h"
#include <soc/dnx/utils/dnx_pp_programmability_utils.h>
#include <bcm_int/dnx/algo/swstate/auto_generated/access/algo_flow_lif_access.h>
#include <appl/diag/bslenable.h>
#include <bcm_int/dnx/mpls/mpls_tunnel_term.h>

extern dnx_flow_app_config_t *dnx_flow_objects[];
extern dnx_flow_common_field_desc_t *dnx_flow_term_common_fields[];
extern dnx_flow_common_field_desc_t *dnx_flow_init_common_fields[];
extern flow_special_field_info_t flow_special_fields_db[];

const uint32 all_zero_array[BCM_FLOW_SPECIAL_FIELD_UIN32_ARR_MAX_SIZE] = { 0 };

extern shr_error_e dbal_tables_physical_table_get(
    int unit,
    dbal_tables_e table_id,
    int physical_tbl_index,
    dbal_physical_tables_e * physical_table_id);

#define DNX_FLOW_VALID_PHASES_PER_DBAL_TABLE_NOF_ELEMENTS 100
#define DNX_FLOW_DBAL_TABLE_PHASE_TO_VALID_RES_TYPES_NOF_ELEMENTS 500
#define FLOW_INITIATOR_SUPPORTED_FLAGS  (0)
#define FLOW_TERMINATOR_SUPPORTED_FLAGS (BCM_FLOW_TERMINATOR_INFO_EXTENDED_TERMINATION)

/* Set encap access - if given by user (!= invalid) --> use it. Otherwise - take default */
#define DNX_FLOW_SET_ENCAP_ACCESS(encap_access, initiator_encap_access, default_encap_access) \
        encap_access = (initiator_encap_access == bcmEncapAccessInvalid) ? default_encap_access : initiator_encap_access;

#define FLOW_ILLEGAL_FLAG_CHECK(flag, str)                              \
    if (_SHR_IS_FLAG_SET(flow_handle_info->flags, flag))                \
    {                                                                   \
        SHR_ERR_EXIT(_SHR_E_PARAM, "Flag %s is not supported for operation", str);    \
    }

/**********************************************************************************************
*                                        verify functions                                     *
**********************************************************************************************/

#define DNX_FLOW_HANDLE_VALIDATE(handle_id)                                  \
    if ((handle_id >= dnx_flow_number_of_apps()) || (handle_id < 1))         \
    {                                                                        \
        SHR_ERR_EXIT(_SHR_E_PARAM, "illegal flow handle ID, %d", handle_id); \
    }

/* Check if verify is needed for flow handle matching application */
#define DNX_FLOW_APP_VERIFY_CHECK(unit,flow_handle,flow_app_info) \
    { \
        uint8 is_verify_disabled; \
        SHR_IF_ERR_EXIT(flow_db.flow_application_info.is_verify_disabled.get(unit, flow_handle, &is_verify_disabled)); \
        if (is_verify_disabled) \
        { \
            SHR_EXIT(); \
        } \
    }

/******************************************************************************************
*                                        Flow Logger                                      *
*******************************************************************************************/
/** holds the current dbal logger info in order to restore it after the flow operation is done */
typedef struct
{
    bsl_severity_t api_severity;
    bsl_severity_t access_severity;
    bsl_severity_t diag_severity;
    dbal_logger_mode_e mode;
    dbal_tables_e table;
} dnx_flow_logger_data_t;

/** the function gets the old values for the dbal logger, for later restoration */
static void
dnx_flow_dbal_logger_old_values_get(
    int unit,
    dnx_flow_logger_data_t * old_logger_data)
{
    CONST dbal_logger_info_t *logger_info;

    /** get the old values */
    dbal_logger_info_get_internal(unit, &logger_info);

    /** get the old severity */
    old_logger_data->api_severity = bslenable_get(bslLayerSocdnx, bslSourceDbaldnx);
    old_logger_data->access_severity = bslenable_get(bslLayerSocdnx, bslSourceDbalaccessdnx);
    old_logger_data->diag_severity = bslenable_get(bslLayerSocdnx, bslSourceDiagdbaldnx);

    /** get the old mode and table */
    old_logger_data->mode = logger_info->logger_mode;
    old_logger_data->table = logger_info->user_log_locked_table;
}

/** the function updates the dbal logger for the flow configurations */
static void
dnx_flow_dbal_logger_flow_config_set(
    int unit,
    dbal_tables_e flow_dbal_table)
{
    /** updating the logger to print only for the relevant dbal table */
    dbal_logger_table_lock(unit, flow_dbal_table);

    /** set the mode for WRITE_ONLY */
    dbal_logger_table_mode_set(unit, DBAL_LOGGER_MODE_WRITE_ONLY);

    /** updating the severities */
    bslenable_set(bslLayerSocdnx, bslSourceDbaldnx, bslSeverityInfo);
    bslenable_set(bslLayerSocdnx, bslSourceDbalaccessdnx, bslSeverityWarn);
    bslenable_set(bslLayerSocdnx, bslSourceDiagdbaldnx, bslSeverityWarn);
}

/** the function gets the current dbal logger configurations and set dbal logger to flow configurations */
static void
dnx_flow_logger_update(
    int unit,
    dbal_tables_e flow_dbal_table,
    dnx_flow_logger_data_t * old_logger_data)
{
    /** first, get the old values for later restoration */
    dnx_flow_dbal_logger_old_values_get(unit, old_logger_data);

    /** second, update the dbal logger */
    dnx_flow_dbal_logger_flow_config_set(unit, flow_dbal_table);
}

/** the function restores the dbal logger to its original configurations */
static void
dnx_flow_logger_restore(
    int unit,
    dnx_flow_logger_data_t * dbal_logger_data)
{
    /** restore the dbal table */
    dbal_logger_table_lock(unit, dbal_logger_data->table);

    /** restore the mode */
    dbal_logger_table_mode_set(unit, dbal_logger_data->mode);

    /** restore the severities */
    bslenable_set(bslLayerSocdnx, bslSourceDbaldnx, dbal_logger_data->api_severity);
    bslenable_set(bslLayerSocdnx, bslSourceDbalaccessdnx, dbal_logger_data->access_severity);
    bslenable_set(bslLayerSocdnx, bslSourceDiagdbaldnx, dbal_logger_data->diag_severity);
}

/** the function prints the logs for the flow */
static shr_error_e
dnx_flow_logs_print(
    int unit,
    bcm_flow_handle_info_t * flow_handle_info,
    dnx_flow_app_config_t * flow_app_info,
    dnx_flow_cmd_control_info_t * flow_cmd_control,
    void *common_fields,
    bcm_flow_special_fields_t * special_fields)
{
    uint8 field_idx = 0;
    char print_string[FLOW_STR_MAX];
    const dnx_flow_common_field_desc_t *common_fields_desc;

    SHR_FUNC_INIT_VARS(unit);

    LOG_CLI((BSL_META("\n*********************FLOW LOGGER - START*********************\n")));

    /** app's name */
    LOG_CLI((BSL_META("Application: %s, "), flow_app_info->app_name));

    /** operation */
    switch (flow_cmd_control->flow_cb_type)
    {
        case FLOW_CB_TYPE_SET:
            if (_SHR_IS_FLAG_SET(flow_handle_info->flags, BCM_FLOW_HANDLE_INFO_REPLACE))
            {
                LOG_CLI((BSL_META("Operation: REPLACE\n")));
            }
            else
            {
                LOG_CLI((BSL_META("Operation: CREATE\n")));
            }
            break;
        case FLOW_CB_TYPE_GET:
            LOG_CLI((BSL_META("Operation: GET\n")));
            break;
        case FLOW_CB_TYPE_DELETE:
            LOG_CLI((BSL_META("Operation: DELETE\n")));
            break;
        case FLOW_CB_TYPE_TRAVERSE:
            LOG_CLI((BSL_META("Operation: TRAVERSE\n")));
            break;
        default:
            SHR_ERR_EXIT(_SHR_E_PARAM, "Invalid callback type (%d)\n", flow_cmd_control->flow_cb_type);
            break;
    }

    /** FLOW ID */
    LOG_CLI((BSL_META("FLOW ID = 0x%x,"), (uint32) flow_handle_info->flow_id));
    if (FLOW_GPORT_IS_VIRTUAL(flow_handle_info->flow_id))
    {
        uint32 virtual_id;
        if (flow_app_info->flow_app_type == FLOW_APP_TYPE_TERM)
        {
            virtual_id = BCM_GPORT_SUB_TYPE_VIRTUAL_INGRESS_GET(flow_handle_info->flow_id);
        }
        else
        {
            virtual_id = BCM_GPORT_SUB_TYPE_VIRTUAL_EGRESS_POINTED_GET(flow_handle_info->flow_id);
        }
        LOG_CLI((BSL_META("Virtual ID = 0x%x\n"), virtual_id));
    }
    else
    {
        LOG_CLI((BSL_META("Global LIF ID = 0x%x\n"),
                 (uint32) _SHR_GPORT_FLOW_LIF_VAL_GET_ID(flow_handle_info->flow_id)));
    }

    /** Flags */
    LOG_CLI((BSL_META("Flags = 0x%x\n"), flow_handle_info->flags));
    if (flow_handle_info->flags != 0)
    {
        uint8 flag_idx;
        for (flag_idx = 0; flag_idx <= FLOW_HANDLE_FLAGS_MAX; flag_idx++)
        {
            if (_SHR_IS_FLAG_SET(flow_handle_info->flags, (1 << flag_idx)))
            {
                LOG_CLI((BSL_META("\t%s\n"), flow_app_valid_flag_to_string(unit, flag_idx)));
            }
        }
    }

    /** FIELDS */
    /** common fields - only for lif apps, in all but delete */
    if (FLOW_APP_IS_LIF(flow_app_info->flow_app_type) && (flow_cmd_control->flow_cb_type != FLOW_CB_TYPE_DELETE))
    {
        uint32 common_fields_bitmap;

        if (flow_app_info->flow_app_type == FLOW_APP_TYPE_TERM)
        {
            common_fields_bitmap = ((bcm_flow_terminator_info_t *) common_fields)->valid_elements_set;
        }
        else    /* FLOW_APP_TYPE_INIT */
        {
            common_fields_bitmap = ((bcm_flow_initiator_info_t *) common_fields)->valid_elements_set;
        }

        if (common_fields_bitmap != 0)
        {
            LOG_CLI((BSL_META("Selected common fields:\n")));
            while (common_fields_bitmap != 0)
            {
                if (common_fields_bitmap & 0x1)
                {
                    SHR_IF_ERR_EXIT(dnx_flow_common_fields_desc_by_app_type_get(unit, flow_app_info->flow_app_type,
                                                                                field_idx, &common_fields_desc));

                    LOG_CLI((BSL_META("\tCommon field %s, "),
                             dnx_flow_common_field_enabler_to_string(unit, flow_app_info->flow_app_type,
                                                                     (1 << field_idx))));

                    SHR_IF_ERR_EXIT(common_fields_desc->print_cb(unit, flow_app_info->flow_app_type,
                                                                 common_fields, print_string));

                    LOG_CLI((BSL_META("value: %s\n"), print_string));

                }
                field_idx++;
                common_fields_bitmap = (common_fields_bitmap >> 2);
            }
        }
    }

    /** special fields, if exists for match apps - always relevant lif apps - relevant only in all but delete */
    if ((FLOW_APP_IS_MATCH(flow_app_info->flow_app_type) ||
         (FLOW_APP_IS_LIF(flow_app_info->flow_app_type) && (flow_cmd_control->flow_cb_type != FLOW_CB_TYPE_DELETE))) &&
        (special_fields->actual_nof_special_fields != 0))
    {

        LOG_CLI((BSL_META("Selected special fields:\n")));
        for (field_idx = 0; field_idx < special_fields->actual_nof_special_fields; field_idx++)
        {
            SHR_IF_ERR_EXIT(flow_special_field_value_print(unit, &(special_fields->special_fields[field_idx]),
                                                           print_string));

            LOG_CLI((BSL_META("\tSpecial field %s, value: %s\n"),
                     dnx_flow_special_field_to_string(unit, special_fields->special_fields[field_idx].field_id),
                     print_string));
        }
    }

    LOG_CLI((BSL_META("**********************FLOW LOGGER - END**********************\n\n")));

exit:
    SHR_FUNC_EXIT;
}

/** if flow logger is active, print the flow data */
#define DNX_FLOW_LOGGER_PRINT(unit, flow_handle_info, flow_app_info, flow_cmd_control, common_fields, special_fields) \
    { \
        if (bsl_check(bslLayerBcmdnx, bslSourceFlow, bslSeverityVerbose, unit)) \
        { \
            SHR_IF_ERR_EXIT(dnx_flow_logs_print(unit, flow_handle_info, flow_app_info, flow_cmd_control, \
                                                (void *) common_fields, special_fields)); \
        } \
    }

/** if flow logger is active, activate the dbal logger and print the flow data */
#define DNX_FLOW_LOGGER_START(unit, flow_handle_info, flow_app_info, flow_cmd_control, common_fields, special_fields, logger_data) \
    { \
        if (bsl_check(bslLayerBcmdnx, bslSourceFlow, bslSeverityVerbose, unit)) \
        { \
            dnx_flow_logger_update(unit, FLOW_LIF_DBAL_TABLE_GET(flow_app_info), &old_logger_data); \
            SHR_IF_ERR_EXIT(dnx_flow_logs_print(unit, flow_handle_info, flow_app_info, flow_cmd_control, \
                                                (void *) common_fields, special_fields)); \
        } \
    }

/** if flow logger is active, restore the dbal logger to its original status */
#define DNX_FLOW_LOGGER_END(unit, logger_data) \
    { \
        if (bsl_check(bslLayerBcmdnx, bslSourceFlow, bslSeverityVerbose, unit)) \
        { \
            dnx_flow_logger_restore(unit, &logger_data); \
        } \
    }

/*******************************************************************************************
*                                       FLOW functions                                     *
********************************************************************************************/
static shr_error_e
flow_special_fields_validate(
    int unit,
    bcm_flow_handle_info_t * flow_handle_info,
    dnx_flow_app_config_t * flow_app_info,
    bcm_flow_special_fields_t * special_fields)
{
    int nof_app_special_fields = 0, app_idx, idx;
    uint8 special_fields_set[BCM_FLOW_SPECIAL_FIELD_MAX_NOF_FIELDS] = { 0 };
    const flow_special_field_info_t *curr_field;

    SHR_FUNC_INIT_VARS(unit);

    while (flow_app_info->special_fields[nof_app_special_fields] != FLOW_S_F_EMPTY)
    {
        nof_app_special_fields++;
    }

    if (FLOW_APP_IS_MATCH(flow_app_info->flow_app_type))
    {
        dbal_table_type_e table_type;
        SHR_IF_ERR_EXIT(dbal_tables_table_type_get(unit, flow_app_info->flow_table[0], &table_type));

        /*
         * For TCAM tables:
         *    - user can add more than the number of app special fields because of mask fields. 
         *    - user can add less than the number of app special fields because unset fields are by default masked 
         * For others:
         *    - user must set all keys
         */
        if ((table_type != DBAL_TABLE_TYPE_TCAM) &&
            (special_fields->actual_nof_special_fields != nof_app_special_fields))
        {
            SHR_ERR_EXIT(_SHR_E_PARAM,
                         "For Match applications - all key special fields must be configured. #App special fields: %d, got %d \n",
                         nof_app_special_fields, special_fields->actual_nof_special_fields);
        }
    }
    else
    {
        if (special_fields->actual_nof_special_fields > nof_app_special_fields)
        {
            SHR_ERR_EXIT(_SHR_E_PARAM,
                         "Invalid configuration, requested %d special fields, nof %s app special fields is %d",
                         special_fields->actual_nof_special_fields, flow_app_info->app_name, nof_app_special_fields);
        }
    }

    /** Iterate over the special fields. Make sure that they're supported by the application */
    for (idx = 0; idx < special_fields->actual_nof_special_fields; idx++)
    {
        flow_special_fields_e user_field = special_fields->special_fields[idx].field_id;

        SHR_IF_ERR_EXIT(flow_special_field_info_get(unit, user_field, &curr_field));

        if ((curr_field->payload_type != FLOW_PAYLOAD_TYPE_UINT32)
            && special_fields->special_fields[idx].shr_var_uint32)
        {
            SHR_ERR_EXIT(_SHR_E_PARAM,
                         "Special field %s is not unit32 type, which means the shr_var_uint32 should be 0 and not %d",
                         dnx_flow_special_field_to_string(unit, user_field),
                         special_fields->special_fields[idx].shr_var_uint32);
        }

        if (!_SHR_IS_FLAG_SET(flow_handle_info->flags, BCM_FLOW_HANDLE_INFO_REPLACE) &&
            special_fields->special_fields[idx].is_clear)
        {
            SHR_ERR_EXIT(_SHR_E_PARAM, "illegal use of is_clear special field %s in create",
                         dnx_flow_special_field_to_string(unit, user_field));
        }

        if (_SHR_IS_FLAG_SET(flow_handle_info->flags, BCM_FLOW_HANDLE_INFO_REPLACE))
        {
            /** replace validations */
            if (FLOW_APP_IS_LIF(flow_app_info->flow_app_type) &&
                dnx_flow_special_field_is_ind_set(unit, curr_field, FLOW_SPECIAL_FIELD_IND_LIF_UNCHANGEABLE) &&
                special_fields->special_fields[idx].is_clear)
            {
                SHR_ERR_EXIT(_SHR_E_PARAM, "Cannot clear unchangeable special field %s",
                             dnx_flow_special_field_to_string(unit, user_field));
            }

            if (FLOW_APP_IS_LIF(flow_app_info->flow_app_type) &&
                dnx_flow_special_field_is_ind_set(unit, curr_field, FLOW_SPECIAL_FIELD_IND_LIF_UNCHANGEABLE) &&
                dnx_flow_special_field_is_ind_set(unit, curr_field, FLOW_SPECIAL_FIELD_IND_LIF_MANDATORY))
            {
                SHR_ERR_EXIT(_SHR_E_PARAM, "Cannot change mandatory unchangeable special field %s",
                             dnx_flow_special_field_to_string(unit, user_field));
            }

            if (FLOW_APP_IS_MATCH(flow_app_info->flow_app_type) &&
                dnx_flow_special_field_is_ind_set(unit, curr_field, FLOW_SPECIAL_FIELD_IND_MATCH_MANDATORY) &&
                special_fields->special_fields[idx].is_clear)
            {
                SHR_ERR_EXIT(_SHR_E_PARAM, "Cannot clear mandatory special field %s in replace",
                             dnx_flow_special_field_to_string(unit, user_field));
            }
            /** in case of replace, cannot clear mandatory lif special fields */
            if (FLOW_APP_IS_LIF(flow_app_info->flow_app_type) &&
                dnx_flow_special_field_is_ind_set(unit, curr_field, FLOW_SPECIAL_FIELD_IND_LIF_MANDATORY) &&
                special_fields->special_fields[idx].is_clear)
            {
                SHR_ERR_EXIT(_SHR_E_PARAM, "Cannot clear mandatory special field %s in replace",
                             dnx_flow_special_field_to_string(unit, user_field));
            }
        }

        for (app_idx = 0; app_idx < nof_app_special_fields; app_idx++)
        {
            if (user_field == flow_app_info->special_fields[app_idx])
            {
                if (special_fields_set[app_idx])
                {
                    SHR_ERR_EXIT(_SHR_E_PARAM, "Special field %s was set multiple times",
                                 dnx_flow_special_field_to_string(unit, user_field));
                }
                /*
                 * Mark this field as was already set by user 
                 */
                special_fields_set[app_idx]++;
                break;
            }
        }
        if (app_idx == nof_app_special_fields)
        {
            SHR_ERR_EXIT(_SHR_E_PARAM, "Special field %s (%d) is not supported in application %s",
                         dnx_flow_special_field_to_string(unit, user_field), user_field, flow_app_info->app_name);
        }
    }
    /** in create, pass on all special fields, verify that all mandatory fields were set */
    if (!_SHR_IS_FLAG_SET(flow_handle_info->flags, BCM_FLOW_HANDLE_INFO_REPLACE))
    {
        for (app_idx = 0; app_idx < nof_app_special_fields; app_idx++)
        {
            /** if special field was set, no need to check it */
            if (special_fields_set[app_idx])
            {
                continue;
            }

            SHR_IF_ERR_EXIT(flow_special_field_info_get(unit, flow_app_info->special_fields[app_idx], &curr_field));

            /** if special field was not set, verify it is not mandatory */
            /** match verify */
            if (FLOW_APP_IS_MATCH(flow_app_info->flow_app_type) &&
                dnx_flow_special_field_is_ind_set(unit, curr_field, FLOW_SPECIAL_FIELD_IND_MATCH_MANDATORY))
            {
                SHR_ERR_EXIT(_SHR_E_PARAM, "Special field %s is mandatory for match applications and must be set",
                             dnx_flow_special_field_to_string(unit, flow_app_info->special_fields[app_idx]));
            }
            /** lif verify */
            if (FLOW_APP_IS_LIF(flow_app_info->flow_app_type) &&
                dnx_flow_special_field_is_ind_set(unit, curr_field, FLOW_SPECIAL_FIELD_IND_LIF_MANDATORY))
            {
                SHR_ERR_EXIT(_SHR_E_PARAM, "Special field %s is mandatory for lif applications and must be set",
                             dnx_flow_special_field_to_string(unit, flow_app_info->special_fields[app_idx]));
            }
        }
    }

exit:
    SHR_FUNC_EXIT;
}

/** for encap access, validate that field is not replaced with different value.
 *  The encap phases aren't relevant only for EEDB entries, not virtual entries. */
static shr_error_e
dnx_flow_encap_validate(
    int unit,
    bcm_flow_handle_info_t * flow_handle_info,
    bcm_flow_initiator_info_t * initiator_info)
{
    bcm_encap_access_t tmp_encap_access, encap_access_default_mapping, encap_access;
    dnx_flow_app_config_t *flow_app_info;

    SHR_FUNC_INIT_VARS(unit);

    flow_app_info = dnx_flow_objects[flow_handle_info->flow_handle];
    encap_access = initiator_info->encap_access;
    encap_access_default_mapping = flow_app_info->encap_access_default_mapping;

    DNX_FLOW_SET_ENCAP_ACCESS(tmp_encap_access, encap_access, encap_access_default_mapping);

    /*
     * For create - make sure that encap id is valid according to arch (sw state) 
     */
    if (!_SHR_IS_FLAG_SET(flow_handle_info->flags, BCM_FLOW_HANDLE_INFO_REPLACE))
    {
        uint32 encap_access_bitmap[1];
        uint8 found;
        dnx_algo_local_outlif_logical_phase_e logical_phase;

        /*
         * Encap access can't be invalid and must be in range
         */
        if ((tmp_encap_access <= bcmEncapAccessInvalid) || (tmp_encap_access > bcmEncapAccessAc))
        {
            SHR_ERR_EXIT(_SHR_E_PARAM, "Encap access is invalid");
        }

        SHR_IF_ERR_EXIT(flow_db.valid_phases_per_dbal_table.find(unit, &FLOW_LIF_DBAL_TABLE_GET(flow_app_info),
                                                                 &encap_access_bitmap[0], &found));

        if (!found)
        {
            SHR_ERR_EXIT(_SHR_E_NOT_FOUND,
                         "entry not found in applications valid encap access hash table for application %s",
                         flow_app_info->app_name);
        }
        SHR_IF_ERR_EXIT(dnx_lif_lib_encap_access_to_logical_phase_convert(unit, tmp_encap_access, &logical_phase));
        if (!SHR_IS_BITSET(encap_access_bitmap, logical_phase))
        {
            SHR_ERR_EXIT(_SHR_E_PARAM, "encap access %s is not supported for application %s", flow_app_info->app_name,
                         flow_encap_access_to_string(unit, tmp_encap_access));
        }
    }

    if (_SHR_IS_FLAG_SET(flow_handle_info->flags, BCM_FLOW_HANDLE_INFO_REPLACE)
        && (encap_access != bcmEncapAccessInvalid))
    {
        SHR_ERR_EXIT(_SHR_E_PARAM, "On replace, encap access must be set to bcmEncapAccessInvalid");
    }

exit:
    SHR_FUNC_EXIT;
}

static shr_error_e
common_fields_validate(
    int unit,
    dnx_flow_app_config_t * flow_app_info,
    uint32 flags,
    void *bcm_app_info)
{
    uint32 valid_elements_clear = 0;
    uint32 valid_elements_set = 0;

    SHR_FUNC_INIT_VARS(unit);

    if (flow_app_info->flow_app_type == FLOW_APP_TYPE_TERM)
    {
        valid_elements_clear = ((bcm_flow_terminator_info_t *) bcm_app_info)->valid_elements_clear;
        valid_elements_set = ((bcm_flow_terminator_info_t *) bcm_app_info)->valid_elements_set;
    }
    else if (flow_app_info->flow_app_type == FLOW_APP_TYPE_INIT)
    {
        valid_elements_clear = ((bcm_flow_initiator_info_t *) bcm_app_info)->valid_elements_clear;
        valid_elements_set = ((bcm_flow_initiator_info_t *) bcm_app_info)->valid_elements_set;
    }
    else if (FLOW_APP_IS_MATCH(flow_app_info->flow_app_type))
    {
        valid_elements_clear = 0;
        valid_elements_set = 0;
    }
    else
    {
        SHR_ERR_EXIT(_SHR_E_PARAM, "invalid flow app type, %d", flow_app_info->flow_app_type);
    }

    /** Make sure that the user has set a valid field on an unsupported application field */
    if ((~flow_app_info->valid_common_fields_bitmap) & valid_elements_set)
    {
        SHR_ERR_EXIT(_SHR_E_PARAM,
                     "Unsupported common bitmap (0x%x) was set to application %s, valid common bitmap is 0x%x",
                     valid_elements_set, flow_app_info->app_name, flow_app_info->valid_common_fields_bitmap);
    }
    /** Make sure that the user has cleared a valid field on an unsupported application field */
    if ((~flow_app_info->valid_common_fields_bitmap) & valid_elements_clear)
    {
        SHR_ERR_EXIT(_SHR_E_PARAM,
                     "Unsupported common bitmap (0x%x) was cleared to application %s, valid common bitmap is 0x%x",
                     valid_elements_clear, flow_app_info->app_name, flow_app_info->valid_common_fields_bitmap);
    }

    /** Make sure that no common field was marked as set and clear at the same time  */
    if (_SHR_IS_FLAG_SET(flags, BCM_FLOW_HANDLE_INFO_REPLACE))
    {
        if (valid_elements_set & valid_elements_clear)
        {
            SHR_ERR_EXIT(_SHR_E_PARAM,
                         "Invalid configuration, VALID flag can only be set on either valid_elements_set(0x%x), valid_elemetns_clear(0x%x)",
                         valid_elements_set, valid_elements_clear);
        }
    }

    /** make sure that in case of termination, l2 and l3 ingress info were not both set */
    if ((flow_app_info->flow_app_type == FLOW_APP_TYPE_TERM) &&
        (valid_elements_set & BCM_FLOW_TERMINATOR_ELEMENT_L3_INGRESS_INFO_VALID) &&
        (valid_elements_set & BCM_FLOW_TERMINATOR_ELEMENT_L2_INGRESS_INFO_VALID))
    {
        SHR_ERR_EXIT(_SHR_E_PARAM,
                     "Cannot set both BCM_FLOW_TERMINATOR_ELEMENT_L3_INGRESS_INFO_VALID and BCM_FLOW_TERMINATOR_ELEMENT_L2_INGRESS_INFO_VALID");
    }

exit:
    SHR_FUNC_EXIT;
}

static shr_error_e
flow_term_specific_validations(
    int unit,
    bcm_flow_handle_info_t * flow_handle_info,
    bcm_flow_terminator_info_t * bcm_app_info,
    bcm_flow_special_fields_t * special_fields)
{
    dnx_flow_app_config_t *flow_app_info = dnx_flow_objects[flow_handle_info->flow_handle];

    SHR_FUNC_INIT_VARS(unit);
    /*
     * Check is added here to silence 'static analyzer' of GCC11.
     * Actually, it is not required.
     */
    SHR_NULL_CHECK(bcm_app_info, _SHR_E_INTERNAL, "bcm_app_info");

    if (_SHR_IS_FLAG_SET(bcm_app_info->flags, ~FLOW_TERMINATOR_SUPPORTED_FLAGS))
    {
        SHR_ERR_EXIT(_SHR_E_PARAM, "invalid TERMINATOR app 'flags' provided - flags = %x, supported_flags = %x",
                     bcm_app_info->flags, FLOW_TERMINATOR_SUPPORTED_FLAGS);
    }

    if ((!dnx_flow_app_is_ind_set(unit, flow_app_info, FLOW_APP_TERM_IND_SUPPORT_EXTENDED_TERM)) &&
        (bcm_app_info->flags & BCM_FLOW_TERMINATOR_INFO_EXTENDED_TERMINATION))
    {
        SHR_ERR_EXIT(_SHR_E_PARAM, "Cannot set the EXTENDED_TERMINATION flag for applications who do not support it");
    }

    /** illegal cases for virtual entries */
    if (_SHR_IS_FLAG_SET(flow_handle_info->flags, BCM_FLOW_HANDLE_INFO_VIRTUAL))
    {
        if ((bcm_app_info->flags & BCM_FLOW_TERMINATOR_INFO_EXTENDED_TERMINATION))
        {
            SHR_ERR_EXIT(_SHR_E_PARAM,
                         "Cannot set the EXTENDED_TERMINATION flag with the BCM_FLOW_HANDLE_INFO_VIRTUAL flag");
        }
        if (_SHR_IS_FLAG_SET(flow_handle_info->flags, BCM_FLOW_HANDLE_INFO_SYMMETRIC_ALLOC))
        {
            SHR_ERR_EXIT(_SHR_E_PARAM,
                         "Cannot set the BCM_FLOW_HANDLE_INFO_SYMMETRIC_ALLOC flag with the BCM_FLOW_HANDLE_INFO_VIRTUAL flag");
        }
    }

    if (_SHR_IS_FLAG_SET(flow_handle_info->flags, BCM_FLOW_HANDLE_INFO_REPLACE) &&
        (bcm_app_info->flags & BCM_FLOW_TERMINATOR_INFO_EXTENDED_TERMINATION))
    {
        SHR_ERR_EXIT(_SHR_E_PARAM, "Cannot set the EXTENDED_TERMINATION in replace");
    }

    /** QOS verification */
    if (_SHR_IS_FLAG_SET(bcm_app_info->valid_elements_set, BCM_FLOW_TERMINATOR_ELEMENT_QOS_MAP_ID_VALID))
    {
        uint32 qos_map_id = bcm_app_info->qos_map_id;
        if (qos_map_id != DNX_QOS_INITIAL_MAP_ID)
        {
            if (!DNX_QOS_MAP_IS_INGRESS(qos_map_id) || !DNX_QOS_MAP_IS_REMARK(qos_map_id)
                || !DNX_QOS_MAP_IS_PHB(qos_map_id))
            {
                SHR_ERR_EXIT(_SHR_E_PARAM, "invalid qos_map_id, 0x%x", qos_map_id);
            }
        }
        else
        {
            SHR_ERR_EXIT(_SHR_E_PARAM, "QOS_MAP_ID value is invalid");
        }
    }

    /** QOS ingress model validate  */
    if (_SHR_IS_FLAG_SET(bcm_app_info->valid_elements_set, BCM_FLOW_TERMINATOR_ELEMENT_QOS_INGRESS_MODEL_VALID))
    {
        SHR_IF_ERR_EXIT(dnx_flow_dl_ingress_qos_profile_validate(unit, flow_handle_info, flow_app_info, bcm_app_info));
    }

    if (_SHR_IS_FLAG_SET(bcm_app_info->valid_elements_set, BCM_FLOW_TERMINATOR_ELEMENT_ACTION_GPORT_VALID))
    {
        SHR_IF_ERR_EXIT(dnx_flow_dl_ingress_trap_profile_verify(unit, bcm_app_info));
    }

    /** l2 learn info flag validation */
    if (_SHR_IS_FLAG_SET(bcm_app_info->valid_elements_set, BCM_FLOW_TERMINATOR_ELEMENT_L2_LEARN_INFO_VALID))
    {
        /** verify that the flag BCM_FLOW_L2_LEARN_INFO_DEST_ONLY is set just if FEC.
         * Because just for FEC, need to know if it is with LIF or not.
         * Without this verification, it will be impossible to know if need to add the flag in "get" API
         */
        if ((_SHR_IS_FLAG_SET(bcm_app_info->l2_learn_info.l2_learn_info_flags, BCM_FLOW_L2_LEARN_INFO_DEST_ONLY)) &&
            (!_SHR_GPORT_IS_FORWARD_PORT(bcm_app_info->l2_learn_info.dest_port)))
        {
            SHR_ERR_EXIT(_SHR_E_PARAM, "BCM_FLOW_L2_LEARN_INFO_DEST_ONLY flag expected just for dest_port type FEC");
        }
    }

    /** ingress info validations */
    if ((bcm_app_info->valid_elements_set & BCM_FLOW_TERMINATOR_ELEMENT_L3_INGRESS_INFO_VALID) &&
        (bcm_app_info->l3_ingress_info.service_type >= bcmFlowServiceTypeCount))
    {
        SHR_ERR_EXIT(_SHR_E_PARAM, "Service type configured %d, max value is %d",
                     bcm_app_info->l3_ingress_info.service_type, (bcmFlowServiceTypeCount - 1));
    }
    if ((bcm_app_info->valid_elements_set & BCM_FLOW_TERMINATOR_ELEMENT_L2_INGRESS_INFO_VALID) &&
        (bcm_app_info->l2_ingress_info.service_type >= bcmFlowServiceTypeCount))
    {
        SHR_ERR_EXIT(_SHR_E_PARAM, "Service type configured %d, max value is %d",
                     bcm_app_info->l2_ingress_info.service_type, (bcmFlowServiceTypeCount - 1));
    }

    /** restrictions which are relevant only for application that have both l2_ingress_info and l3_ingress_info */
    if ((!dnx_data_flow.general.feature_get(unit, dnx_data_flow_general_is_lif_profile_dedicated_logic_enabled)) &&
        (flow_app_info->valid_common_fields_bitmap & BCM_FLOW_TERMINATOR_ELEMENT_L2_INGRESS_INFO_VALID) &&
        (flow_app_info->valid_common_fields_bitmap & BCM_FLOW_TERMINATOR_ELEMENT_L3_INGRESS_INFO_VALID))
    {
        /** l2 ingress info validation - cannot receive default value */
        if ((bcm_app_info->valid_elements_set & BCM_FLOW_TERMINATOR_ELEMENT_L2_INGRESS_INFO_VALID) &&
            (bcm_app_info->l2_ingress_info.ingress_network_group_id == DEFAULT_IN_LIF_ORIENTATION))
        {
            SHR_ERR_EXIT(_SHR_E_PARAM,
                         "BCM_FLOW_TERMINATOR_ELEMENT_L2_INGRESS_INFO_VALID param cannot be default value (%d) in app %s",
                         DEFAULT_IN_LIF_ORIENTATION, flow_app_info->app_name);
        }
        /** l3 ingress info validation - cannot receive default value */
        if (bcm_app_info->valid_elements_set & BCM_FLOW_TERMINATOR_ELEMENT_L3_INGRESS_INFO_VALID)
        {
            if (_SHR_IS_FLAG_SET
                (bcm_app_info->l3_ingress_info.l3_ingress_flags, ~FLOW_TERMINATOR_L3_INGRESS_FLAGS_SUPPORTED))
            {
                SHR_ERR_EXIT(_SHR_E_PARAM, "Unsupported flag or flags provided 0x%x - (supported flags are 0x%x).",
                             bcm_app_info->l3_ingress_info.l3_ingress_flags,
                             FLOW_TERMINATOR_L3_INGRESS_FLAGS_SUPPORTED);
            }
            if ((bcm_app_info->l3_ingress_info.urpf_mode == bcmL3IngressUrpfDisable) &&
                !(_SHR_IS_FLAG_SET
                  (bcm_app_info->l3_ingress_info.l3_ingress_flags, FLOW_TERMINATOR_L3_INGRESS_FLAGS_SUPPORTED)))
            {
                SHR_ERR_EXIT(_SHR_E_PARAM,
                             "Either RPF mode or l3 ingress flags should get value different than the default one.");
            }
        }

    }

    if (dnx_data_flow.general.feature_get(unit, dnx_data_flow_general_is_lif_profile_dedicated_logic_enabled) &&
        (flow_app_info->valid_common_fields_bitmap & BCM_FLOW_TERMINATOR_ELEMENT_FIELD_CLASS_ID_VALID))
    {
        if (_SHR_IS_FLAG_SET(bcm_app_info->valid_elements_set, BCM_FLOW_TERMINATOR_ELEMENT_FIELD_CLASS_ID_VALID))
        {
            uint32 start_bit = 0, nof_bits = 0, field_profile_max_value = 0;
            SHR_IF_ERR_EXIT(dnx_field_port_profile_bits_get
                            (unit, DNX_FIELD_PORT_PROFILE_TYPE_IN_LIF_INGRESS, &start_bit, &nof_bits));

            field_profile_max_value = utilex_max_value_by_size(nof_bits);
            if ((bcm_app_info->field_class_id == DEFAULT_IN_LIF_FIELD_CLASS_ID)
                || (bcm_app_info->field_class_id > field_profile_max_value))
            {
                SHR_ERR_EXIT(_SHR_E_PARAM,
                             "When BCM_FLOW_TERMINATOR_ELEMENT_FIELD_CLASS_ID_VALID is set, field_class_id should be bigger than 0 and smaller than %d but it is %d\n",
                             field_profile_max_value, bcm_app_info->field_class_id);
            }
        }
    }

    if (dnx_data_flow.general.feature_get(unit, dnx_data_flow_general_is_lif_profile_dedicated_logic_enabled) &&
        (flow_app_info->valid_common_fields_bitmap & BCM_FLOW_TERMINATOR_ELEMENT_FIELD_CLASS_CS_ID_VALID))
    {
        if (_SHR_IS_FLAG_SET(bcm_app_info->valid_elements_set, BCM_FLOW_TERMINATOR_ELEMENT_FIELD_CLASS_ID_VALID))
        {
            int field_class_cs_id_size = 0;
            uint32 field_class_cs_id_max_value = 0;
            SHR_IF_ERR_EXIT(dbal_tables_field_size_get
                            (unit, DBAL_TABLE_FIELD_IPMF1_CS_INLIF_PROFILE_MAP, DBAL_FIELD_INLIF_0_PROFILE, FALSE, 0,
                             INST_SINGLE, &field_class_cs_id_size));
            field_class_cs_id_max_value = utilex_max_value_by_size(field_class_cs_id_size);
            if ((bcm_app_info->field_class_cs_id == DEFAULT_IN_LIF_FIELD_CLASS_CS_ID)
                || bcm_app_info->field_class_cs_id > field_class_cs_id_max_value)
            {
                SHR_ERR_EXIT(_SHR_E_PARAM,
                             "When BCM_FLOW_TERMINATOR_ELEMENT_FIELD_CLASS_CS_ID_VALID is set, field_class_cs_id should be bigger than 0 and smalled than %d but it is %d\n",
                             field_class_cs_id_max_value, bcm_app_info->field_class_cs_id);
            }
        }
    }
exit:
    SHR_FUNC_EXIT;
}

static shr_error_e
dnx_flow_mandatory_l3_intf_validate(
    int unit,
    bcm_flow_handle_info_t * flow_handle_info,
    bcm_flow_initiator_info_t * bcm_app_info)
{
    SHR_FUNC_INIT_VARS(unit);

    /** in create, check if l3_intf is mandatory and was not set */
    if ((!_SHR_IS_FLAG_SET(flow_handle_info->flags, BCM_FLOW_HANDLE_INFO_REPLACE)) &&
        (!(bcm_app_info->valid_elements_set & BCM_FLOW_INITIATOR_ELEMENT_L3_INTF_ID_VALID)))
    {
        SHR_ERR_EXIT(_SHR_E_PARAM, "BCM_FLOW_INITIATOR_ELEMENT_L3_INTF_ID_VALID is a mandatory field that must be set");
    }
    /** in replace, check if l3_intf is mandatory and was cleared */
    if ((_SHR_IS_FLAG_SET(flow_handle_info->flags, BCM_FLOW_HANDLE_INFO_REPLACE)) &&
        (bcm_app_info->valid_elements_clear & BCM_FLOW_INITIATOR_ELEMENT_L3_INTF_ID_VALID))
    {
        SHR_ERR_EXIT(_SHR_E_PARAM,
                     "BCM_FLOW_INITIATOR_ELEMENT_L3_INTF_ID_VALID is a mandatory field and cannot be cleared");
    }

exit:
    SHR_FUNC_EXIT;
}

static shr_error_e
flow_init_specific_validations(
    int unit,
    bcm_flow_handle_info_t * flow_handle_info,
    bcm_flow_initiator_info_t * bcm_app_info,
    bcm_flow_special_fields_t * special_fields)
{
    dnx_flow_app_config_t *flow_app_info = dnx_flow_objects[flow_handle_info->flow_handle];
    int is_local_lif;

    SHR_FUNC_INIT_VARS(unit);
    /*
     * Check is added here to silence 'static analyzer' of GCC11.
     * Actually, it is not required.
     */
    SHR_NULL_CHECK(bcm_app_info, _SHR_E_INTERNAL, "bcm_app_info");

    if (_SHR_IS_FLAG_SET(bcm_app_info->flags, ~FLOW_INITIATOR_SUPPORTED_FLAGS))
    {
        SHR_ERR_EXIT(_SHR_E_PARAM, "invalid initiator app 'flags' provided - flags = %x, supported_flags = %x",
                     bcm_app_info->flags, FLOW_INITIATOR_SUPPORTED_FLAGS);
    }

    /** Encap access validation handling, relevant only for EEDB entries, not for ESEM entries */
    is_local_lif = !(dnx_flow_app_is_ind_set(unit, flow_app_info, FLOW_APP_INIT_IND_NO_LOCAL_LIF));
    if (is_local_lif)
    {
        /** Encap access validation handling  */
        SHR_IF_ERR_EXIT(dnx_flow_encap_validate(unit, flow_handle_info, bcm_app_info));
    }

    /** Qos validations */
    SHR_IF_ERR_EXIT(dnx_flow_dl_egress_qos_profile_validate
                    (unit, flow_handle_info, flow_app_info, bcm_app_info, special_fields));

    /** mandatory l3_intf validation */
    if (dnx_flow_app_is_ind_set(unit, flow_app_info, FLOW_APP_INIT_IND_MANDATORY_L3_INTF))
    {
        SHR_IF_ERR_EXIT(dnx_flow_mandatory_l3_intf_validate(unit, flow_handle_info, bcm_app_info));
    }

    /** outlif group can only be set in create */
    if (_SHR_IS_FLAG_SET(flow_handle_info->flags, BCM_FLOW_HANDLE_INFO_REPLACE) &&
        _SHR_IS_FLAG_SET(bcm_app_info->valid_elements_set, BCM_FLOW_INITIATOR_ELEMENT_OUTLIF_PROFILE_GROUP_VALID))
    {
        SHR_ERR_EXIT(_SHR_E_PARAM, "common field outlif_group can only be set in create");
    }

    /** outlif group cannot be set to 0 */
    if (_SHR_IS_FLAG_SET(bcm_app_info->valid_elements_set, BCM_FLOW_INITIATOR_ELEMENT_OUTLIF_PROFILE_GROUP_VALID) &&
        (bcm_app_info->outlif_profile_group == 0))
    {
        SHR_ERR_EXIT(_SHR_E_PARAM, "common field outlif_group cannot be set to 0");
    }

    /** outlif group cannot be set above allowed max value */
    if (_SHR_IS_FLAG_SET(bcm_app_info->valid_elements_set, BCM_FLOW_INITIATOR_ELEMENT_OUTLIF_PROFILE_GROUP_VALID) &&
        (bcm_app_info->outlif_profile_group > dnx_data_lif.global_lif.global_lif_group_max_val_get(unit)))
    {
        SHR_ERR_EXIT(_SHR_E_PARAM, "common field outlif_group cannot be above max value %d",
                     dnx_data_lif.global_lif.global_lif_group_max_val_get(unit));
    }

    /** outlif group cannot be cleared */
    if (_SHR_IS_FLAG_SET(bcm_app_info->valid_elements_clear, BCM_FLOW_INITIATOR_ELEMENT_OUTLIF_PROFILE_GROUP_VALID))
    {
        SHR_ERR_EXIT(_SHR_E_PARAM, "common field outlif_group cannot be cleared");
    }

    /** svtag_en cannot be changed in replace */
    if (_SHR_IS_FLAG_SET(flow_handle_info->flags, BCM_FLOW_HANDLE_INFO_REPLACE) &&
        _SHR_IS_FLAG_SET(bcm_app_info->valid_elements_clear, BCM_FLOW_INITIATOR_ELEMENT_SVTAG_ENABLE_VALID))
    {
        SHR_ERR_EXIT(_SHR_E_PARAM, "common field svtag_enable cannot be cleared");
    }
    /** svtag_en cannot be changed in replace */
    if (_SHR_IS_FLAG_SET(flow_handle_info->flags, BCM_FLOW_HANDLE_INFO_REPLACE) &&
        _SHR_IS_FLAG_SET(bcm_app_info->valid_elements_set, BCM_FLOW_INITIATOR_ELEMENT_SVTAG_ENABLE_VALID))
    {
        SHR_ERR_EXIT(_SHR_E_PARAM, "common field svtag_enable cannot be set in replace");
    }

    if (_SHR_IS_FLAG_SET(bcm_app_info->valid_elements_set, BCM_FLOW_INITIATOR_ELEMENT_ACTION_GPORT_VALID))
    {
        SHR_IF_ERR_EXIT(dnx_flow_dl_egress_trap_profile_verify(unit, bcm_app_info));
    }

exit:
    SHR_FUNC_EXIT;
}

/** common function to verify correct flow id encoding for virtual types (relevant only for WITH_ID creation) */
static shr_error_e
dnx_flow_gport_virtual_encoding_verify(
    int unit,
    bcm_flow_handle_info_t * flow_handle_info,
    dnx_flow_app_config_t * flow_app_info)
{
    SHR_FUNC_INIT_VARS(unit);

    /*
     * For virtual, we're validating only in case of virtual subtype encoding. Otherwise - with_ID without encode,
     * don't validate
     */
    if (BCM_GPORT_IS_FLOW_LIF(flow_handle_info->flow_id))
    {
        if (FLOW_GPORT_IS_VIRTUAL(flow_handle_info->flow_id))
        {
            uint32 gport_subtype = BCM_GPORT_SUB_TYPE_GET(flow_handle_info->flow_id);
            dbal_tables_e dbal_table_id = FLOW_LIF_DBAL_TABLE_GET(flow_app_info);
            uint8 is_local_lif = !(dnx_flow_app_is_ind_set(unit, flow_app_info, FLOW_APP_INIT_IND_NO_LOCAL_LIF));

            if (gport_subtype == BCM_GPORT_SUB_TYPE_VIRTUAL_EGRESS_DEFAULT)
            {
                if ((is_local_lif) || (dbal_table_id != DBAL_TABLE_ESEM_DEFAULT_RESULT_TABLE)
                    || (flow_app_info->flow_app_type != FLOW_APP_TYPE_INIT))
                {
                    SHR_ERR_EXIT(_SHR_E_PARAM,
                                 "Error, Virtual flag set, for subtype VIRTUAL_EGRESS_DEFAULT, flow application should be related to ESEM default (no local lif) and application type must be initiator");
                }
            }
            else if (gport_subtype == BCM_GPORT_SUB_TYPE_VIRTUAL_EGRESS_MATCH)
            {
                if ((is_local_lif) || (dbal_table_id == DBAL_TABLE_ESEM_DEFAULT_RESULT_TABLE)
                    || (flow_app_info->flow_app_type != FLOW_APP_TYPE_INIT))
                {
                    SHR_ERR_EXIT(_SHR_E_PARAM,
                                 "Error, Virtual flag set, for subtype VIRTUAL_EGRESS_MATCH, flow application should be related to ESEM (no local lif) and application type must be initiator");
                }
            }
            else if (gport_subtype == BCM_GPORT_SUB_TYPE_VIRTUAL_EGRESS_POINTED)
            {
                if ((!is_local_lif) || (flow_app_info->flow_app_type != FLOW_APP_TYPE_INIT))
                {
                    SHR_ERR_EXIT(_SHR_E_PARAM,
                                 "Error, Virtual flag set, for subtype BCM_GPORT_SUB_TYPE_VIRTUAL_EGRESS_POINTED, local lif must be present and application type must be initiator");
                }
            }
            else if (gport_subtype == BCM_GPORT_SUB_TYPE_VIRTUAL_INGRESS)
            {
                if (flow_app_info->flow_app_type != FLOW_APP_TYPE_TERM)
                {
                    SHR_ERR_EXIT(_SHR_E_PARAM,
                                 "Error, Virtual flag set, for subtype BCM_GPORT_SUB_TYPE_VIRTUAL_INGRESS, application type must be initiator");
                }
            }
            else
            {
                SHR_ERR_EXIT(_SHR_E_PARAM, "Error, Virtual flag set, with unsupported subtype");
            }
        }
        else
        {
            SHR_ERR_EXIT(_SHR_E_PARAM, "Unsupported virtual gport encoding 0x%x", flow_handle_info->flow_id);
        }
    }
    else if (!BCM_GPORT_IS_SET(flow_handle_info->flow_id))
    {
        /*
         * No validations 
         */
        SHR_SET_CURRENT_ERR(_SHR_E_NONE);
        SHR_EXIT();
    }
    else
    {
        SHR_ERR_EXIT(_SHR_E_PARAM, "Unsupported gport encoding 0x%x", flow_handle_info->flow_id);
    }

exit:
    SHR_FUNC_EXIT;
}

/** common function to verify create operations for all app_types */
static shr_error_e
dnx_flow_create_verify(
    int unit,
    bcm_flow_handle_info_t * flow_handle_info,
    void *bcm_app_info,
    bcm_flow_special_fields_t * special_fields)
{
    int num_common_fields = 0;
    dnx_flow_app_config_t *flow_app_info;
    dnx_flow_app_type_e flow_app_type;
    int is_virtual_only = 0;

    SHR_FUNC_INIT_VARS(unit);

    SHR_NULL_CHECK(flow_handle_info, _SHR_E_PARAM, "flow_handle_info");
    DNX_FLOW_HANDLE_VALIDATE(flow_handle_info->flow_handle);

    flow_app_info = dnx_flow_objects[flow_handle_info->flow_handle];
    flow_app_type = flow_app_info->flow_app_type;
    if ((flow_app_type == FLOW_APP_TYPE_TERM) || (flow_app_type == FLOW_APP_TYPE_INIT))
    {
        SHR_NULL_CHECK(bcm_app_info, _SHR_E_PARAM, "bcm_info");
    }

    DNX_FLOW_APP_VERIFY_CHECK(unit, flow_handle_info->flow_handle, flow_app_info);
    SHR_IF_ERR_EXIT(dnx_flow_number_of_common_fields(unit, flow_app_type, &num_common_fields));

    /** General validations */
    if (flow_app_info->flow_app_type != flow_app_type)
    {
        SHR_ERR_EXIT(_SHR_E_INTERNAL, "Mismatch between caller API app type and application app type");
    }

    /** Verify that the only allowed flow handler flags are used */
    if ((flow_handle_info->flags & flow_app_info->valid_flow_flags) != flow_handle_info->flags)
    {
        SHR_ERR_EXIT(_SHR_E_PARAM,
                     "Invalid configuration, a set flow handler flag(0x%x) isn't valid(0x%x) for application %s",
                     flow_handle_info->flags, flow_app_info->valid_flow_flags, flow_app_info->app_name);
    }

    FLOW_ILLEGAL_FLAG_CHECK(BCM_FLOW_HANDLE_TRAVERSE_DELETE_ALL, "BCM_FLOW_HANDLE_TRAVERSE_DELETE_ALL");

    if (_SHR_IS_FLAG_SET(flow_handle_info->flags, BCM_FLOW_HANDLE_INFO_REPLACE))
    {
        FLOW_ILLEGAL_FLAG_CHECK(BCM_FLOW_HANDLE_INFO_SYMMETRIC_ALLOC, "BCM_FLOW_HANDLE_INFO_SYMMETRIC_ALLOC");
        FLOW_ILLEGAL_FLAG_CHECK(BCM_FLOW_HANDLE_NATIVE, "BCM_FLOW_HANDLE_NATIVE");
        FLOW_ILLEGAL_FLAG_CHECK(BCM_FLOW_HANDLE_INFO_ALLOC_BY_CORE_BM, "BCM_FLOW_HANDLE_INFO_ALLOC_BY_CORE_BM");
        FLOW_ILLEGAL_FLAG_CHECK(BCM_FLOW_HANDLE_INFO_VIRTUAL, "BCM_FLOW_HANDLE_INFO_VIRTUAL");
        if (!_SHR_IS_FLAG_SET(flow_handle_info->flags, BCM_FLOW_HANDLE_INFO_WITH_ID))
        {
            SHR_ERR_EXIT(_SHR_E_PARAM, "Flag BCM_FLOW_HANDLE_INFO_WITH_ID Must be set for replace operations");
        }
    }
    /** core_bm validations */
    if (_SHR_IS_FLAG_SET(flow_handle_info->flags, BCM_FLOW_HANDLE_INFO_ALLOC_BY_CORE_BM))
    {
        int core_iter, nof_cores_set = 0;
        int nof_cores = dnx_data_device.general.nof_cores_get(unit);
        for (core_iter = 0; core_iter < nof_cores; core_iter++)
        {
            if (flow_handle_info->core_bitmap & SAL_BIT(core_iter))
            {
                nof_cores_set++;
            }
        }
        if ((nof_cores_set == nof_cores) || (nof_cores_set == 0))
        {
            SHR_ERR_EXIT(_SHR_E_PARAM, "illegal value for core_bitmap (%x), setting  all/no cores is not legal",
                         flow_handle_info->core_bitmap);
        }
    }
    else
    {
        if (flow_handle_info->core_bitmap != 0)
        {
            SHR_ERR_EXIT(_SHR_E_PARAM, "When using core_bitmap (%x) BCM_FLOW_HANDLE_INFO_ALLOC_BY_CORE_BM must be set",
                         flow_handle_info->core_bitmap);
        }
    }

    if (num_common_fields)
    {
        SHR_IF_ERR_EXIT(common_fields_validate(unit, flow_app_info, flow_handle_info->flags, bcm_app_info));
    }

        /** special fields validations */
    if (special_fields)
    {
        SHR_IF_ERR_EXIT(flow_special_fields_validate(unit, flow_handle_info, flow_app_info, special_fields));
    }

    if (flow_app_type == FLOW_APP_TYPE_INIT)
    {
        SHR_IF_ERR_EXIT(flow_init_specific_validations
                        (unit, flow_handle_info, ((bcm_flow_initiator_info_t *) bcm_app_info), special_fields));
    }

    if (flow_app_type == FLOW_APP_TYPE_TERM)
    {
        SHR_IF_ERR_EXIT(flow_term_specific_validations
                        (unit, flow_handle_info, ((bcm_flow_terminator_info_t *) bcm_app_info), special_fields));
    }

    /** In case it's a create WITH_ID, validate the allocation of the FLOW-ID  */
    if ((_SHR_IS_FLAG_SET(flow_handle_info->flags, BCM_FLOW_HANDLE_INFO_WITH_ID)) &&
        (!(_SHR_IS_FLAG_SET(flow_handle_info->flags, BCM_FLOW_HANDLE_INFO_REPLACE))) &&
        (!(FLOW_APP_IS_MATCH(flow_app_info->flow_app_type))))
    {
        uint8 is_allocated = FALSE;

        if (_SHR_IS_FLAG_SET(flow_handle_info->flags, BCM_FLOW_HANDLE_INFO_VIRTUAL))
        {
            SHR_IF_ERR_EXIT(dnx_flow_gport_virtual_encoding_verify(unit, flow_handle_info, flow_app_info));
        }
        else    /* Non-virtual */
        {
            int global_lif = _SHR_GPORT_FLOW_LIF_VAL_GET_ID(flow_handle_info->flow_id);

            if (!global_lif)
            {
                SHR_ERR_EXIT(_SHR_E_PARAM,
                             "Error, Flow Gport is not given when flag BCM_FLOW_HANDLE_INFO_WITH_ID is set");
            }

            if (BCM_GPORT_IS_FLOW_LIF(flow_handle_info->flow_id))
            {
                if (((flow_app_info->flow_app_type == FLOW_APP_TYPE_INIT)
                     && (BCM_GPORT_SUB_TYPE_IS_IN_LIF(flow_handle_info->flow_id)))
                    || ((flow_app_info->flow_app_type == FLOW_APP_TYPE_TERM)
                        && (BCM_GPORT_SUB_TYPE_IS_OUT_LIF(flow_handle_info->flow_id))))
                {
                    SHR_ERR_EXIT(_SHR_E_PARAM, "Error, flow lif gport encoding doesn't match the flow-id subtype");
                }
                else if ((!BCM_GPORT_SUB_TYPE_IS_IN_LIF(flow_handle_info->flow_id))
                         && (!BCM_GPORT_SUB_TYPE_IS_OUT_LIF(flow_handle_info->flow_id)))
                {
                    SHR_ERR_EXIT(_SHR_E_PARAM, "Unsupported gport sub type");
                }
            }
            else if (BCM_GPORT_IS_SET(flow_handle_info->flow_id))
            {
                SHR_ERR_EXIT(_SHR_E_PARAM, "Unsupported gport encoding 0x%x", flow_handle_info->flow_id);
            }
        }

        SHR_IF_ERR_EXIT(flow_lif_mgmt_is_gport_allocated(unit, flow_handle_info, flow_app_info, &is_allocated));
        if (is_allocated)
        {
            SHR_ERR_EXIT(_SHR_E_EXISTS,
                         "Trying to allocte ID that is allocated using Flag BCM_FLOW_HANDLE_INFO_WITH_ID (flow_id=0x%08x)\n",
                         flow_handle_info->flow_id);
        }
    }

    if (FLOW_APP_IS_LIF(flow_app_info->flow_app_type)
        && _SHR_IS_FLAG_SET(flow_handle_info->flags, BCM_FLOW_HANDLE_INFO_REPLACE))
    {
        uint8 is_allocated = FALSE;
        SHR_IF_ERR_EXIT(flow_lif_mgmt_is_gport_allocated(unit, flow_handle_info, flow_app_info, &is_allocated));
        if (!is_allocated)
        {
            SHR_ERR_EXIT(_SHR_E_NOT_FOUND,
                         "Flag BCM_FLOW_HANDLE_INFO_REPLACE for entry that is not exists (flow_id=0x%08x)\n",
                         flow_handle_info->flow_id);
        }
    }

    /** Perform Virtual gport validations  */
    if ((_SHR_IS_FLAG_SET(flow_handle_info->flags, BCM_FLOW_HANDLE_INFO_VIRTUAL)) &&
        !dnx_data_flow.general.feature_get(unit, dnx_data_flow_general_is_virtual_supported))
    {
        SHR_ERR_EXIT(_SHR_E_PARAM, "virtual flag BCM_FLOW_HANDLE_INFO_VIRTUAL is not supported for this device ");
    }

    is_virtual_only =
        dnx_flow_app_is_ind_set(unit, flow_app_info, (flow_app_info->flow_app_type == FLOW_APP_TYPE_INIT) ?
                                FLOW_APP_INIT_IND_VIRTUAL_LIF_ONLY : FLOW_APP_TERM_IND_VIRTUAL_LIF_ONLY);

    if ((!_SHR_IS_FLAG_SET(flow_handle_info->flags, BCM_FLOW_HANDLE_INFO_REPLACE)) &&
        (!(_SHR_IS_FLAG_SET(flow_handle_info->flags, BCM_FLOW_HANDLE_INFO_VIRTUAL))) && (is_virtual_only))
    {
        SHR_ERR_EXIT(_SHR_E_PARAM,
                     "Missing virtual flag BCM_FLOW_HANDLE_INFO_VIRTUAL while creating an object for the Virtual Only application %s",
                     flow_app_info->app_name);
    }

    if ((!_SHR_IS_FLAG_SET(flow_handle_info->flags, BCM_FLOW_HANDLE_INFO_REPLACE)) &&
        (_SHR_IS_FLAG_SET(flow_handle_info->flags, BCM_FLOW_HANDLE_INFO_VIRTUAL)) && !is_virtual_only)
    {
        if (!dnx_flow_app_is_ind_set
            (unit, flow_app_info, (flow_app_info->flow_app_type == FLOW_APP_TYPE_INIT) ?
             FLOW_APP_INIT_IND_VIRTUAL_LIF_SUPPORTED : FLOW_APP_TERM_IND_VIRTUAL_LIF_SUPPORTED))
        {
            SHR_ERR_EXIT(_SHR_E_PARAM, "Flag BCM_FLOW_HANDLE_INFO_VIRTUAL is not supported for application %s",
                         flow_app_info->app_name);
        }
    }

    /*
     * Invoke specific verify function only for create, for replace we will call it after get operation
     */
    if (flow_app_info->verify_cb && (!(_SHR_IS_FLAG_SET(flow_handle_info->flags, BCM_FLOW_HANDLE_INFO_REPLACE))))
    {
        dnx_flow_cmd_control_info_t flow_cmd_control = { FLOW_CB_TYPE_SET, FLOW_COMMAND_CONTINUE };

        SHR_IF_ERR_EXIT(flow_app_info->verify_cb
                        (unit, flow_handle_info, &flow_cmd_control, bcm_app_info, special_fields, -1));
    }

exit:
    SHR_FUNC_EXIT;
}

static shr_error_e
dnx_flow_delete_verify(
    int unit,
    dnx_flow_app_type_e flow_app_type,
    bcm_flow_handle_info_t * flow_handle_info,
    bcm_flow_special_fields_t * special_fields)
{
    dnx_flow_app_config_t *flow_app_info;

    SHR_FUNC_INIT_VARS(unit);

    SHR_NULL_CHECK(flow_handle_info, _SHR_E_PARAM, "flow_handle_info");

    DNX_FLOW_HANDLE_VALIDATE(flow_handle_info->flow_handle);

    flow_app_info = dnx_flow_objects[flow_handle_info->flow_handle];

    DNX_FLOW_APP_VERIFY_CHECK(unit, flow_handle_info->flow_handle, flow_app_info);

    FLOW_ILLEGAL_FLAG_CHECK(BCM_FLOW_HANDLE_INFO_REPLACE, "BCM_FLOW_HANDLE_INFO_REPLACE");
    FLOW_ILLEGAL_FLAG_CHECK(BCM_FLOW_HANDLE_INFO_SYMMETRIC_ALLOC, "BCM_FLOW_HANDLE_INFO_SYMMETRIC_ALLOC");
    FLOW_ILLEGAL_FLAG_CHECK(BCM_FLOW_HANDLE_TRAVERSE_DELETE_ALL, "BCM_FLOW_HANDLE_TRAVERSE_DELETE_ALL");
    FLOW_ILLEGAL_FLAG_CHECK(BCM_FLOW_HANDLE_NATIVE, "BCM_FLOW_HANDLE_NATIVE");
    FLOW_ILLEGAL_FLAG_CHECK(BCM_FLOW_HANDLE_INFO_ALLOC_BY_CORE_BM, "BCM_FLOW_HANDLE_INFO_ALLOC_BY_CORE_BM");
    FLOW_ILLEGAL_FLAG_CHECK(BCM_FLOW_HANDLE_INFO_VIRTUAL, "BCM_FLOW_HANDLE_INFO_VIRTUAL");
    FLOW_ILLEGAL_FLAG_CHECK(BCM_FLOW_HANDLE_INFO_WITH_ID, "BCM_FLOW_HANDLE_INFO_WITH_ID");

    /** Verify that the only allowed flow handler flags are used */
    if ((flow_handle_info->flags & flow_app_info->valid_flow_flags) != flow_handle_info->flags)
    {
        SHR_ERR_EXIT(_SHR_E_PARAM,
                     "Invalid configuration, a set flow handler flag(0x%x) isn't valid(0x%x) for application %s",
                     flow_handle_info->flags, flow_app_info->valid_flow_flags, flow_app_info->app_name);
    }

    if (special_fields)
    {
        SHR_IF_ERR_EXIT(flow_special_fields_validate(unit, flow_handle_info, flow_app_info, special_fields));
    }

    if ((FLOW_APP_IS_MATCH(flow_app_type)) && (!special_fields))
    {
        SHR_ERR_EXIT(_SHR_E_PARAM, "Special fields must exist in application %s", flow_app_info->app_name);
    }

    /*
     * Invoke specific verify function Do we need casting here for the bcm_app_info to the correct bcm struct
     */
    if (flow_app_info->verify_cb)
    {
        dnx_flow_cmd_control_info_t flow_cmd_control = {
            FLOW_CB_TYPE_DELETE, FLOW_COMMAND_CONTINUE
        };

        SHR_IF_ERR_EXIT(flow_app_info->verify_cb(unit, flow_handle_info, &flow_cmd_control, NULL, special_fields, -1));
    }

exit:
    SHR_FUNC_EXIT;
}

/*
 * Verify flow lif get
 */
static shr_error_e
dnx_flow_get_verify(
    int unit,
    dnx_flow_app_type_e flow_app_type,
    bcm_flow_handle_info_t * flow_handle_info,
    void *info,
    bcm_flow_special_fields_t * special_fields)
{
    dnx_flow_app_config_t *flow_app_info;

    SHR_FUNC_INIT_VARS(unit);

    SHR_NULL_CHECK(flow_handle_info, _SHR_E_PARAM, "flow_handle_info");

    if ((flow_app_type == FLOW_APP_TYPE_TERM) || (flow_app_type == FLOW_APP_TYPE_INIT))
    {
        SHR_NULL_CHECK(info, _SHR_E_PARAM, "bcm_info");
    }

    DNX_FLOW_HANDLE_VALIDATE(flow_handle_info->flow_handle);

    flow_app_info = dnx_flow_objects[flow_handle_info->flow_handle];

    DNX_FLOW_APP_VERIFY_CHECK(unit, flow_handle_info->flow_handle, flow_app_info);

    FLOW_ILLEGAL_FLAG_CHECK(BCM_FLOW_HANDLE_INFO_REPLACE, "BCM_FLOW_HANDLE_INFO_REPLACE");
    FLOW_ILLEGAL_FLAG_CHECK(BCM_FLOW_HANDLE_INFO_SYMMETRIC_ALLOC, "BCM_FLOW_HANDLE_INFO_SYMMETRIC_ALLOC");
    FLOW_ILLEGAL_FLAG_CHECK(BCM_FLOW_HANDLE_TRAVERSE_DELETE_ALL, "BCM_FLOW_HANDLE_TRAVERSE_DELETE_ALL");
    FLOW_ILLEGAL_FLAG_CHECK(BCM_FLOW_HANDLE_NATIVE, "BCM_FLOW_HANDLE_NATIVE");
    FLOW_ILLEGAL_FLAG_CHECK(BCM_FLOW_HANDLE_INFO_ALLOC_BY_CORE_BM, "BCM_FLOW_HANDLE_INFO_ALLOC_BY_CORE_BM");
    FLOW_ILLEGAL_FLAG_CHECK(BCM_FLOW_HANDLE_INFO_VIRTUAL, "BCM_FLOW_HANDLE_INFO_VIRTUAL");
    FLOW_ILLEGAL_FLAG_CHECK(BCM_FLOW_HANDLE_INFO_WITH_ID, "BCM_FLOW_HANDLE_INFO_WITH_ID");

    /** Verify that the only allowed flow handler flags are used */
    if ((flow_handle_info->flags & flow_app_info->valid_flow_flags) != flow_handle_info->flags)
    {
        SHR_ERR_EXIT(_SHR_E_PARAM,
                     "Invalid configuration, a set flow handler flag(0x%x) isn't valid(0x%x) for application %s",
                     flow_handle_info->flags, flow_app_info->valid_flow_flags, flow_app_info->app_name);
    }

    /*
     * If the application supports special fields - special_fields can't be NULL
     */
    if (flow_app_info->special_fields[0] != FLOW_S_F_EMPTY)
    {
        SHR_NULL_CHECK(special_fields, _SHR_E_PARAM, "special_fields");
    }

    /*
     * Invoke specific verify function Do we need casting here for the bcm_app_info to the correct bcm struct
     */
    if (flow_app_info->verify_cb)
    {
        dnx_flow_cmd_control_info_t flow_cmd_control = { FLOW_CB_TYPE_GET, FLOW_COMMAND_CONTINUE };

        SHR_IF_ERR_EXIT(flow_app_info->verify_cb(unit, flow_handle_info, &flow_cmd_control, NULL, special_fields, -1));
    }

exit:
    SHR_FUNC_EXIT;
}

/*
 * Verify flow lif traverse
 */
static shr_error_e
dnx_flow_traverse_verify(
    int unit,
    bcm_flow_handle_info_t * flow_handle_info,
    void *cb,
    void *user_data)
{
    SHR_FUNC_INIT_VARS(unit);

    SHR_NULL_CHECK(flow_handle_info, _SHR_E_PARAM, "flow_handle_info");
    if (!_SHR_IS_FLAG_SET(flow_handle_info->flags, BCM_FLOW_HANDLE_TRAVERSE_DELETE_ALL))
    {
        SHR_NULL_CHECK(cb, _SHR_E_PARAM, "cb");
    }

    FLOW_ILLEGAL_FLAG_CHECK(BCM_FLOW_HANDLE_INFO_REPLACE, "BCM_FLOW_HANDLE_INFO_REPLACE");
    FLOW_ILLEGAL_FLAG_CHECK(BCM_FLOW_HANDLE_INFO_SYMMETRIC_ALLOC, "BCM_FLOW_HANDLE_INFO_SYMMETRIC_ALLOC");
    FLOW_ILLEGAL_FLAG_CHECK(BCM_FLOW_HANDLE_NATIVE, "BCM_FLOW_HANDLE_NATIVE");
    FLOW_ILLEGAL_FLAG_CHECK(BCM_FLOW_HANDLE_INFO_ALLOC_BY_CORE_BM, "BCM_FLOW_HANDLE_INFO_ALLOC_BY_CORE_BM");
    FLOW_ILLEGAL_FLAG_CHECK(BCM_FLOW_HANDLE_INFO_BUD, "BCM_FLOW_HANDLE_INFO_BUD");
    FLOW_ILLEGAL_FLAG_CHECK(BCM_FLOW_HANDLE_INFO_VIRTUAL, "BCM_FLOW_HANDLE_INFO_VIRTUAL");

    /*
     * If flow handle doesn't indicate to traverse all applications - verify it with common verify 
     */
    if (flow_handle_info->flow_handle != BCM_FLOW_HANDLE_TRAVERSE_ALL)
    {
        DNX_FLOW_HANDLE_VALIDATE(flow_handle_info->flow_handle);
    }
exit:
    SHR_FUNC_EXIT;
}

/*
 * Verify flow handle control
 */
static shr_error_e
dnx_flow_handle_control_verify(
    int unit,
    bcm_flow_handle_t flow_handle,
    bcm_flow_handle_control_t type,
    uint32 value)
{
    SHR_FUNC_INIT_VARS(unit);

    DNX_FLOW_HANDLE_VALIDATE(flow_handle);
    if ((type <= bcmFlowHandleControlInvalid) || (type >= bcmbcmFlowHandleControl__Count))
    {
        SHR_ERR_EXIT(_SHR_E_PARAM, "Invalid control type %d", type);
    }
    if ((type == bcmFlowHandleControlVerifyEnable) && ((value != TRUE) && (value != FALSE)))
    {
        SHR_ERR_EXIT(_SHR_E_PARAM, "Flow control verify enable supports 1,0 (got %d)", value);
    }
    else if ((type == bcmFlowHandleControlErrorRecoveryEnable) && ((value != TRUE) && (value != FALSE)))
    {
        SHR_ERR_EXIT(_SHR_E_PARAM, "Flow control error recovery enable supports 1,0 (got %d)", value);
    }

exit:
    SHR_FUNC_EXIT;
}
/**********************************************************************************************
*                                        verify functions - END                               *
**********************************************************************************************/

/**********************************************************************************************
*                            EXTENDED TERMINATION functions - START                           *
**********************************************************************************************/
static shr_error_e
dnx_flow_extended_termination_set(
    int unit,
    int global_lif_id)
{
    lif_mapping_local_lif_payload_t local_lif_payload = {
        .local_lifs_array = {{0}},
        .phy_table = 0,
        .is_symmetric = 0
    };

    SHR_FUNC_INIT_VARS(unit);

    SHR_IF_ERR_EXIT(dnx_algo_lif_mapping_global_to_local_get
                    (unit, DNX_ALGO_LIF_INGRESS, global_lif_id, &local_lif_payload));

    if (local_lif_payload.is_symmetric == TRUE)
    {
        SHR_IF_ERR_EXIT(dnx_global_lif_reclassify_write
                        (unit, global_lif_id, DBAL_CORE_ALL, local_lif_payload.local_lifs_array.local_lifs[0], FALSE));
    }
    else        /* not symmetric */
    {
        uint8 local_lif_idx;
        uint32 nof_cores = dnx_data_device.general.nof_cores_get(unit);

        /** pass on all local lifs */
        for (local_lif_idx = 0; local_lif_idx < nof_cores; local_lif_idx++)
        {
            if (local_lif_payload.local_lifs_array.local_lifs[local_lif_idx] == LIF_MNGR_INVALID)
            {
                continue;
            }
            /** setting the current local_lif */
            SHR_IF_ERR_EXIT(dnx_global_lif_reclassify_write(unit, global_lif_id, local_lif_idx,
                                                            local_lif_payload.
                                                            local_lifs_array.local_lifs[local_lif_idx], FALSE));
        }
    }

exit:
    SHR_FUNC_EXIT;
}

static shr_error_e
dnx_flow_extended_termination_get(
    int unit,
    int global_lif_id,
    uint32 *terminato_flags)
{
    uint8 is_entry_written = FALSE;
    lif_mapping_local_lif_payload_t local_lif_payload = {
        .local_lifs_array = {{0}},
        .phy_table = 0,
        .is_symmetric = 0
    };

    SHR_FUNC_INIT_VARS(unit);

    SHR_IF_ERR_EXIT(dnx_algo_lif_mapping_global_to_local_get(unit, DNX_ALGO_LIF_INGRESS, global_lif_id,
                                                             &local_lif_payload));

    if (local_lif_payload.is_symmetric == TRUE)
    {
        SHR_IF_ERR_EXIT(dnx_global_lif_reclassify_get(unit, global_lif_id, DBAL_CORE_DEFAULT, &is_entry_written));
    }
    else        /* not symmetric */
    {
        uint8 local_lif_idx;
        uint32 nof_cores = dnx_data_device.general.nof_cores_get(unit);

        /** pass on all local lifs */
        for (local_lif_idx = 0; local_lif_idx < nof_cores; local_lif_idx++)
        {
            if (local_lif_payload.local_lifs_array.local_lifs[local_lif_idx] == LIF_MNGR_INVALID)
            {
                continue;
            }
            /** getting the current local_lif */
            SHR_IF_ERR_EXIT(dnx_global_lif_reclassify_get(unit, global_lif_id, local_lif_idx, &is_entry_written));
            break;
        }
    }

    if (is_entry_written == TRUE)
    {
        (*terminato_flags) |= BCM_FLOW_TERMINATOR_INFO_EXTENDED_TERMINATION;
    }

exit:
    SHR_FUNC_EXIT;
}

static shr_error_e
dnx_flow_extended_termination_replace(
    int unit,
    int global_lif_id)
{
    uint8 is_entry_written = FALSE;
    lif_mapping_local_lif_payload_t local_lif_payload = {
        .local_lifs_array = {{0}},
        .phy_table = 0,
        .is_symmetric = 0
    };

    SHR_FUNC_INIT_VARS(unit);

    SHR_IF_ERR_EXIT(dnx_algo_lif_mapping_global_to_local_get(unit, DNX_ALGO_LIF_INGRESS,
                                                             global_lif_id, &local_lif_payload));

    if (local_lif_payload.is_symmetric == TRUE)
    {
        SHR_IF_ERR_EXIT(dnx_global_lif_reclassify_get(unit, global_lif_id, DBAL_CORE_DEFAULT, &is_entry_written));

        if (is_entry_written == TRUE)
        {
            SHR_IF_ERR_EXIT(dnx_global_lif_reclassify_write(unit, global_lif_id, DBAL_CORE_ALL,
                                                            local_lif_payload.local_lifs_array.local_lifs[0], TRUE));
        }
    }
    else        /* not symmetric */
    {
        uint8 local_lif_idx;
        uint32 nof_cores = dnx_data_device.general.nof_cores_get(unit);

        /** pass on all local lifs */
        for (local_lif_idx = 0; local_lif_idx < nof_cores; local_lif_idx++)
        {
            if (local_lif_payload.local_lifs_array.local_lifs[local_lif_idx] == LIF_MNGR_INVALID)
            {
                continue;
            }
            /** getting the current local_lif */
            SHR_IF_ERR_EXIT(dnx_global_lif_reclassify_get(unit, global_lif_id, local_lif_idx, &is_entry_written));

            if (is_entry_written == TRUE)
            {
                SHR_IF_ERR_EXIT(dnx_global_lif_reclassify_write(unit, global_lif_id, local_lif_idx,
                                                                local_lif_payload.
                                                                local_lifs_array.local_lifs[local_lif_idx], TRUE));
            }
            break;
        }
        /** if need to write to rest of the local lifs */
        if (is_entry_written == TRUE)
        {
            /** continue from the last local lif */
            for (; local_lif_idx < nof_cores; local_lif_idx++)
            {
                if (local_lif_payload.local_lifs_array.local_lifs[local_lif_idx] == LIF_MNGR_INVALID)
                {
                    continue;
                }
                SHR_IF_ERR_EXIT(dnx_global_lif_reclassify_write(unit, global_lif_id, local_lif_idx,
                                                                local_lif_payload.
                                                                local_lifs_array.local_lifs[local_lif_idx], TRUE));
            }
        }
    }

exit:
    SHR_FUNC_EXIT;
}

static shr_error_e
dnx_flow_extended_termination_delete(
    int unit,
    int global_lif_id)
{
    uint8 is_entry_written = FALSE;
    lif_mapping_local_lif_payload_t local_lif_payload = {
        .local_lifs_array = {{0}},
        .phy_table = 0,
        .is_symmetric = 0
    };

    SHR_FUNC_INIT_VARS(unit);

    SHR_IF_ERR_EXIT(dnx_algo_lif_mapping_global_to_local_get(unit, DNX_ALGO_LIF_INGRESS, global_lif_id,
                                                             &local_lif_payload));

    if (local_lif_payload.is_symmetric == TRUE)
    {
        SHR_IF_ERR_EXIT(dnx_global_lif_reclassify_get(unit, global_lif_id, DBAL_CORE_DEFAULT, &is_entry_written));

        if (is_entry_written == TRUE)
        {
            SHR_IF_ERR_EXIT(dnx_global_lif_reclassify_delete(unit, global_lif_id, DBAL_CORE_ALL));
        }
    }
    else        /* not symmetric */
    {
        uint8 local_lif_idx;
        uint32 nof_cores = dnx_data_device.general.nof_cores_get(unit);

        /** pass on all local lifs */
        for (local_lif_idx = 0; local_lif_idx < nof_cores; local_lif_idx++)
        {
            if (local_lif_payload.local_lifs_array.local_lifs[local_lif_idx] == LIF_MNGR_INVALID)
            {
                continue;
            }
            /** getting the current local_lif */
            SHR_IF_ERR_EXIT(dnx_global_lif_reclassify_get(unit, global_lif_id, local_lif_idx, &is_entry_written));

            if (is_entry_written == TRUE)
            {
                SHR_IF_ERR_EXIT(dnx_global_lif_reclassify_delete(unit, global_lif_id, local_lif_idx));
            }
            break;
        }
        /** if need to write to rest of the local lifs */
        if (is_entry_written == TRUE)
        {
            /** continue from the last local lif */
            for (; local_lif_idx < nof_cores; local_lif_idx++)
            {
                if (local_lif_payload.local_lifs_array.local_lifs[local_lif_idx] == LIF_MNGR_INVALID)
                {
                    continue;
                }
                SHR_IF_ERR_EXIT(dnx_global_lif_reclassify_delete(unit, global_lif_id, local_lif_idx));
            }
        }
    }

exit:
    SHR_FUNC_EXIT;
}

/**********************************************************************************************
*                            EXTENDED TERMINATION functions - END                             *
**********************************************************************************************/

/**********************************************************************************************
*                                        Internal functions                                   *
**********************************************************************************************/

/* Flow init procedure - encap access
 * This function is used initiatlizing encap access related sw states:
 * 1. application --> valid encap access bitmap
 * 2. application + encap access --> valid result types
 * Those will be used for verifications during create and result type selection
 */
static shr_error_e
dnx_flow_app_encap_access_init(
    int unit)
{
    char etps_to_phases_xml_path[RHFILE_MAX_SIZE];
    void *curTop = NULL, *curPhase;
    char *image_name = NULL;
    uint8 success;
    uint8 is_standard_image = FALSE;
    sw_state_htbl_init_info_t hash_tbl_init_info;
    flow_app_encap_info_t flow_app_encap_info;
    uint32 res_type_bitmap[1] = { 0 };
    uint32 encap_access_bitmap[1] = { 0 };
    uint8 found;

    SHR_FUNC_INIT_VARS(unit);

    SHR_IF_ERR_EXIT(dnx_pp_prgm_device_image_name_get(unit, &image_name));
    SHR_IF_ERR_EXIT(dnx_pp_prgm_default_image_check(unit, &is_standard_image));

    if (is_standard_image)
    {
        sal_strncpy(etps_to_phases_xml_path, "mdb/auto_generated/", RHFILE_MAX_SIZE - 1);
    }
    else
    {
        sal_strncpy(etps_to_phases_xml_path, "bare_metal/mdb/auto_generated/", RHFILE_MAX_SIZE - 1);
    }
    sal_strncat_s(etps_to_phases_xml_path, image_name, sizeof(etps_to_phases_xml_path));
    sal_strncat_s(etps_to_phases_xml_path, "/", sizeof(etps_to_phases_xml_path));
    sal_strncat_s(etps_to_phases_xml_path, image_name, sizeof(etps_to_phases_xml_path));
    sal_strncat_s(etps_to_phases_xml_path, "_etps_in_eedb_phases.xml", sizeof(etps_to_phases_xml_path));

    /*
     * Read XMl
     */
    if (is_standard_image)
    {
        curTop =
            dbx_pre_compiled_lif_phases_top_get(unit, etps_to_phases_xml_path, "PhasesEedbCatalog",
                                                CONF_OPEN_PER_DEVICE);
    }
    else
    {
        curTop = dbx_pre_compiled_lif_phases_top_get(unit, etps_to_phases_xml_path, "PhasesEedbCatalog", 0);
    }
    if (curTop == NULL)
    {
        SHR_ERR_EXIT(_SHR_E_NOT_FOUND, "Cannot find etps to phase XML file: %s\n", etps_to_phases_xml_path);
    }

    /*
     * Create the sw states 
     */
    sal_memset(&hash_tbl_init_info, 0, sizeof(sw_state_htbl_init_info_t));
    hash_tbl_init_info.max_nof_elements = DNX_FLOW_DBAL_TABLE_PHASE_TO_VALID_RES_TYPES_NOF_ELEMENTS;
    hash_tbl_init_info.expected_nof_elements = hash_tbl_init_info.max_nof_elements;
    hash_tbl_init_info.print_cb_name = "dnx_algo_flow_dbal_table_to_valid_result_types_print_cb";

    SHR_IF_ERR_EXIT(flow_db.dbal_table_to_valid_result_types.create(unit, &hash_tbl_init_info));

    sal_memset(&hash_tbl_init_info, 0, sizeof(sw_state_htbl_init_info_t));
    hash_tbl_init_info.max_nof_elements = DNX_FLOW_VALID_PHASES_PER_DBAL_TABLE_NOF_ELEMENTS;
    hash_tbl_init_info.expected_nof_elements = hash_tbl_init_info.max_nof_elements;

    SHR_IF_ERR_EXIT(flow_db.valid_phases_per_dbal_table.create(unit, &hash_tbl_init_info));

    /*
     * Run over all phases and look for flow applications, if found - update
     */
    RHDATA_ITERATOR(curPhase, curTop, "EedbPhase")
    {
        int logical_phase = 0;
        int nof_mappings = 0;
        char phase_name[10];
        void *curElement;
        sw_state_htbl_init_info_t hash_tbl_init_info;

        RHDATA_GET_XSTR_STOP(curPhase, "Name", phase_name, 10);
        RHDATA_GET_INT_STOP(curPhase, "NumOfMappings", nof_mappings);

        /*
         * Assuming phase names are EEDB_A -> EEDB_H,
         * the mapping to logical phase is done by taking the 6th char and reduce 'A'
         */
        logical_phase = phase_name[5] - 'A';

        sal_memset(&hash_tbl_init_info, 0, sizeof(sw_state_htbl_init_info_t));

        hash_tbl_init_info.max_nof_elements = nof_mappings;
        hash_tbl_init_info.expected_nof_elements = hash_tbl_init_info.max_nof_elements;

        RHDATA_ITERATOR(curElement, curPhase, "Element")
        {
            /*
             * Add all elements in XML to SW state.
             * Translate strings to DBAL IDs
             */
            char dbal_table_name[DBAL_MAX_STRING_LENGTH], dbal_result_type_name[DBAL_MAX_STRING_LENGTH];
            dbal_tables_e dbal_table_id;
            int is_table_active;
            int result_type;

            RHDATA_GET_XSTR_STOP(curElement, "AppDb", dbal_table_name, DBAL_MAX_STRING_LENGTH);
            RHDATA_GET_XSTR_STOP(curElement, "ResultType", dbal_result_type_name, DBAL_MAX_STRING_LENGTH);

            SHR_IF_ERR_EXIT(dbal_logical_table_string_to_id(unit, dbal_table_name, &dbal_table_id));
            SHR_IF_ERR_EXIT(dbal_tables_is_table_active(unit, dbal_table_id, &is_table_active));
            if (!is_table_active)
            {
                continue;
            }

            if (sal_strncmp(dbal_result_type_name, "", DBAL_MAX_STRING_LENGTH) == 0)
            {
                result_type = -1;
            }
            else
            {
                SHR_IF_ERR_EXIT(dbal_tables_result_type_by_name_get
                                (unit, dbal_table_id, dbal_result_type_name, &result_type));
            }

            if (result_type >= 0)
            {
                sal_memset(&flow_app_encap_info, 0, sizeof(flow_app_encap_info_t));
                sal_memset(&res_type_bitmap, 0, sizeof(res_type_bitmap));

                flow_app_encap_info.dbal_table = dbal_table_id;
                flow_app_encap_info.encap_access = logical_phase;

                res_type_bitmap[0] = 0;

                SHR_IF_ERR_EXIT(flow_db.
                                dbal_table_to_valid_result_types.find(unit, &flow_app_encap_info, &res_type_bitmap[0],
                                                                      &found));

                SHR_BITSET(res_type_bitmap, result_type);

                SHR_IF_ERR_EXIT(flow_db.
                                dbal_table_to_valid_result_types.insert(unit, &flow_app_encap_info, &res_type_bitmap[0],
                                                                        &success));

                if (success == 0)
                {
                    SHR_ERR_EXIT(_SHR_E_INTERNAL,
                                 "Internal error - failed to add table %s + encap access %d to result type %s sw state",
                                 dbal_logical_table_to_string(unit, dbal_table_id), logical_phase,
                                 dbal_result_type_name);
                }
            }

            encap_access_bitmap[0] = 0;
            SHR_IF_ERR_EXIT(flow_db.
                            valid_phases_per_dbal_table.find(unit, &dbal_table_id, &encap_access_bitmap[0], &found));

            SHR_BITSET(encap_access_bitmap, logical_phase);

            SHR_IF_ERR_EXIT(flow_db.valid_phases_per_dbal_table.insert(unit, &dbal_table_id, &encap_access_bitmap[0],
                                                                       &success));

            if (success == 0)
            {
                SHR_ERR_EXIT(_SHR_E_INTERNAL,
                             "Internal error - failed to add table %s to encap access %d sw state",
                             dbal_logical_table_to_string(unit, dbal_table_id), logical_phase);
            }
        }
    }

exit:
    dbx_xml_top_close(curTop);
    SHR_FUNC_EXIT;
}

/** Flow init procedures */
shr_error_e
dnx_flow_init(
    int unit)
{
    int num_apps;
    uint8 is_init;

    SHR_FUNC_INIT_VARS(unit);
    DNX_ERR_RECOVERY_START(unit);

    num_apps = dnx_flow_number_of_apps();

    if (!sw_state_is_warm_boot(unit))
    {
        /*
         * Initialize flow SW state.
         */
        SHR_IF_ERR_EXIT(flow_db.is_init(unit, &is_init));

        if (!is_init)
        {
            SHR_IF_ERR_EXIT(flow_db.init(unit));
        }

        SHR_IF_ERR_EXIT(flow_db.flow_application_info.alloc(unit, num_apps));

        SHR_IF_ERR_EXIT(dnx_flow_app_encap_access_init(unit));
    }

    /*
     * Initialize the FLOW LIF Mgmnt module
     */
    SHR_IF_ERR_EXIT(dnx_flow_lif_mgmt_init(unit));

exit:
    DNX_ERR_RECOVERY_END(unit);
    SHR_FUNC_EXIT;
}

/* Flow de-init procedures */
shr_error_e
dnx_flow_deinit(
    int unit)
{
    SHR_FUNC_INIT_VARS(unit);

    /*
     * de-initialize the FLOW LIF-Table-Mngr APIs
     */
    SHR_IF_ERR_EXIT(dnx_flow_lif_mgmt_deinit(unit));

exit:
    SHR_FUNC_EXIT;
}

shr_error_e
flow_lif_flow_app_from_gport_get(
    int unit,
    bcm_gport_t gport,
    dnx_flow_app_type_e flow_app_type,
    uint32 *flow_handle)
{
    dbal_tables_e table_id;
    int is_virtual = FALSE;
    dnx_algo_gpm_gport_hw_resources_t gport_hw_resources;

    SHR_FUNC_INIT_VARS(unit);

    if (FLOW_GPORT_IS_VIRTUAL(gport))
    {
        is_virtual = TRUE;
    }

    if (flow_app_type == FLOW_APP_TYPE_TERM)
    {
        SHR_IF_ERR_REPLACE_AND_EXIT(dnx_algo_gpm_gport_to_hw_resources
                                    (unit, gport, DNX_ALGO_GPM_GPORT_HW_RESOURCES_LOCAL_LIF_INGRESS,
                                     &gport_hw_resources), _SHR_E_NOT_FOUND, _SHR_E_PARAM);

        table_id = gport_hw_resources.inlif_dbal_table_id;
    }
    else if (flow_app_type == FLOW_APP_TYPE_INIT)
    {
        uint32 lif_flags = (is_virtual) ? DNX_ALGO_GPM_GPORT_HW_RESOURCES_EGRESS_LOCAL_LIF_OR_VIRTUAL :
            (DNX_ALGO_GPM_GPORT_HW_RESOURCES_LOCAL_LIF_EGRESS | DNX_ALGO_GPM_GPORT_HW_RESOURCES_GLOBAL_LIF_EGRESS);

        SHR_IF_ERR_REPLACE_AND_EXIT(dnx_algo_gpm_gport_to_hw_resources
                                    (unit, gport, lif_flags, &gport_hw_resources), _SHR_E_NOT_FOUND, _SHR_E_PARAM);

        table_id = gport_hw_resources.outlif_dbal_table_id;
    }
    else
    {
        SHR_ERR_EXIT(_SHR_E_PARAM, "Invalid value for flow_app_type (%d) ", flow_app_type);
    }

    SHR_IF_ERR_EXIT(flow_lif_flow_app_from_table_id_get(unit, table_id, flow_app_type, flow_handle));
exit:
    SHR_FUNC_EXIT;
}

shr_error_e
flow_lif_flow_app_from_encoded_gport_get(
    int unit,
    bcm_gport_t gport,
    uint32 *flow_handle)
{
    SHR_FUNC_INIT_VARS(unit);

    if (BCM_GPORT_SUB_TYPE_IS_IN_LIF(gport) || BCM_GPORT_SUB_TYPE_IS_VIRTUAL_INGRESS(gport))
    {
        SHR_IF_ERR_EXIT(flow_lif_flow_app_from_gport_get(unit, gport, FLOW_APP_TYPE_TERM, flow_handle));
    }
    else if (BCM_GPORT_SUB_TYPE_IS_OUT_LIF(gport) ||
             BCM_GPORT_SUB_TYPE_IS_VIRTUAL_EGRESS_MATCH(gport) ||
             BCM_GPORT_SUB_TYPE_IS_VIRTUAL_EGRESS_DEFAULT(gport) || BCM_GPORT_SUB_TYPE_IS_VIRTUAL_EGRESS_POINTED(gport))
    {
        SHR_IF_ERR_EXIT(flow_lif_flow_app_from_gport_get(unit, gport, FLOW_APP_TYPE_INIT, flow_handle));
    }
    else
    {
        SHR_ERR_EXIT(_SHR_E_PARAM, "Unrecognized gport sub type 0x%x", BCM_GPORT_SUB_TYPE_GET(gport));
    }

exit:
    SHR_FUNC_EXIT;
}

/**
 * Refer to the .h file
 */
shr_error_e
flow_lif_flow_app_from_table_id_get(
    int unit,
    dbal_tables_e table_id,
    dnx_flow_app_type_e flow_app_type,
    uint32 *flow_handle)
{
    int app_idx;
    int num_apps = dnx_flow_number_of_apps();
    SHR_FUNC_INIT_VARS(unit);
    if ((flow_app_type != FLOW_APP_TYPE_INIT) && flow_app_type != FLOW_APP_TYPE_TERM)
    {
        SHR_ERR_EXIT(_SHR_E_PARAM, "Invalid flow application type passed\n");
    }
    for (app_idx = FLOW_APP_FIRST_IDX; app_idx < num_apps; app_idx++)
    {
        if (FLOW_APP_IS_NOT_VALID(dnx_flow_objects[app_idx]))
        {
            continue;
        }
        if (FLOW_LIF_DBAL_TABLE_GET(dnx_flow_objects[app_idx]) == table_id)
        {
            (*flow_handle) = app_idx;
            SHR_EXIT();
        }
    }

    SHR_SET_CURRENT_ERR(_SHR_E_NOT_FOUND);

exit:
    SHR_FUNC_EXIT;
}

/**
 * Returns true if the app indication is set. 
 */
int
dnx_flow_special_field_is_ind_set(
    int unit,
    const flow_special_field_info_t * field,
    uint32 ind)
{
    uint32 field_ind[2] = { 0 };

    field_ind[0] = field->indications_bm;

    return SHR_IS_BITSET(field_ind, ind);
}

/*
 * Internal ingress flow lif replace
 */
static shr_error_e
dnx_flow_commit_error_check(
    int unit,
    int entry_handle_id)
{
    dbal_entry_handle_t *entry_handle_ptr = NULL;

    SHR_FUNC_INIT_VARS(unit);

    SHR_IF_ERR_EXIT(dbal_entry_handle_get_internal(unit, entry_handle_id, &entry_handle_ptr));
    if (entry_handle_ptr->error_info.error_exists)
    {
        if (entry_handle_ptr->error_info.field_and_instance)
        {
            SHR_ERR_EXIT(_SHR_E_PARAM, "There was an issue with field %s",
                         dbal_field_to_string(unit, entry_handle_ptr->error_info.field_and_instance));
        }
        else
        {
            SHR_ERR_EXIT(_SHR_E_INTERNAL, "Internal error with entry commit");
        }
    }

exit:
    SHR_FUNC_EXIT;
}

static shr_error_e
dnx_flow_special_fields_set_one_field(
    int unit,
    uint32 entry_handle_id,
    bcm_flow_handle_info_t * flow_handle_info,
    dnx_flow_app_config_t * flow_app_info,
    dnx_algo_gpm_gport_hw_resources_t * gport_hw_resources,
    bcm_flow_special_fields_t * special_fields,
    int field_idx)
{
    const flow_special_field_info_t *curr_field;
    int is_match_app = FLOW_APP_IS_MATCH(flow_app_info->flow_app_type) ? TRUE : FALSE;
    bcm_flow_special_field_t *bcm_special_fields = &(special_fields->special_fields[field_idx]);
    int is_key, is_tcam = 0;

    SHR_FUNC_INIT_VARS(unit);

    /** Determine whether it's TCAM field key */
    if (is_match_app)
    {
        dbal_table_type_e table_type;
        SHR_IF_ERR_EXIT(dbal_tables_table_type_get(unit, flow_app_info->flow_table[0], &table_type));
        if (table_type == DBAL_TABLE_TYPE_TCAM)
        {
            is_tcam = TRUE;
        }
    }

    SHR_IF_ERR_EXIT(flow_special_field_info_get(unit, bcm_special_fields->field_id, &curr_field));

    /** A key field is either a match application field or a field that has a key indication (Virtual application) */
    is_key = is_match_app || dnx_flow_special_field_is_ind_set(unit, curr_field, FLOW_SPECIAL_FIELD_IND_IS_KEY);

    if (curr_field->set != NULL)
    {
        SHR_IF_ERR_EXIT(curr_field->set
                        (unit, entry_handle_id, flow_handle_info, flow_app_info, gport_hw_resources, field_idx,
                         special_fields));
    }
    else if (is_key && is_tcam
             && (dnx_flow_special_field_is_ind_set(unit, curr_field, FLOW_SPECIAL_FIELD_IND_MASK_SUPPORTED)))
    {
        SHR_IF_ERR_EXIT(flow_special_fields_mask_field_value_set(unit, entry_handle_id, flow_handle_info, flow_app_info,
                                                                 gport_hw_resources, field_idx, special_fields));
    }
    else if (dnx_flow_special_field_is_ind_set(unit, curr_field, FLOW_SPECIAL_FIELD_IND_SRC_ADDR_PROFILE_SUPPORTED) &&
             dnx_flow_app_is_ind_set(unit, flow_app_info, FLOW_APP_INIT_IND_SRC_ADDR_PROFILE))
    {
        /** logic is done under src profile dedicated logic  */
        SHR_EXIT();
    }
    else if (curr_field->mapped_dbal_field != FLOW_S_FIELD_UNMAPPED)
    {
        /** set directly according to dbal field and payload type */
        switch (curr_field->payload_type)
        {
            case FLOW_PAYLOAD_TYPE_UINT32:
                if (is_key)
                {
                    dbal_entry_key_field32_set(unit, entry_handle_id, curr_field->mapped_dbal_field,
                                               bcm_special_fields->shr_var_uint32);
                }
                else
                {
                    dbal_entry_value_field32_set(unit, entry_handle_id, curr_field->mapped_dbal_field, INST_SINGLE,
                                                 bcm_special_fields->shr_var_uint32);

                }
                break;
            case FLOW_PAYLOAD_TYPE_UINT32_ARR:
                if (is_key)
                {
                    dbal_entry_key_field_arr32_set(unit, entry_handle_id, curr_field->mapped_dbal_field,
                                                   bcm_special_fields->shr_var_uint32_arr);
                }
                else
                {
                    dbal_entry_value_field_arr32_set(unit, entry_handle_id, curr_field->mapped_dbal_field, INST_SINGLE,
                                                     bcm_special_fields->shr_var_uint32_arr);

                }
                break;
            case FLOW_PAYLOAD_TYPE_UINT8_ARR:
                if (is_key)
                {
                    dbal_entry_key_field_arr8_set(unit, entry_handle_id, curr_field->mapped_dbal_field,
                                                  bcm_special_fields->shr_var_uint8_arr);
                }
                else
                {
                    dbal_entry_value_field_arr8_set(unit, entry_handle_id, curr_field->mapped_dbal_field, INST_SINGLE,
                                                    bcm_special_fields->shr_var_uint8_arr);

                }
                break;

            case FLOW_PAYLOAD_TYPE_ENUM:
            case FLOW_PAYLOAD_TYPE_BCM_ENUM:

                if (is_key)
                {
                    dbal_entry_key_field32_set(unit, entry_handle_id, curr_field->mapped_dbal_field,
                                               bcm_special_fields->symbol);
                }
                else
                {
                    dbal_entry_value_field32_set(unit, entry_handle_id, curr_field->mapped_dbal_field, INST_SINGLE,
                                                 bcm_special_fields->symbol);

                }
                break;
            case FLOW_PAYLOAD_TYPE_ENABLER:
                if (is_key)
                {
                    dbal_entry_key_field32_set(unit, entry_handle_id, curr_field->mapped_dbal_field, 0x1);
                }
                else
                {
                    dbal_entry_value_field32_set(unit, entry_handle_id, curr_field->mapped_dbal_field, INST_SINGLE,
                                                 0x1);
                }
                break;
            default:
                SHR_ERR_EXIT(_SHR_E_INTERNAL, "unrecognized payload_type %d\n", curr_field->payload_type);
        }
    }
    else
    {
        SHR_ERR_EXIT(_SHR_E_INTERNAL, "field has no mapping %s\n", curr_field->name);
    }

exit:
    SHR_FUNC_EXIT;
}

/** copy one field HW value (from get_entry_handle_id) to new handle the field to perform the operations is
 *  flow_app_info->special_fields[field_idx]  */
static shr_error_e
dnx_flow_special_fields_copy_one_field(
    int unit,
    uint32 entry_handle_id,
    uint32 get_entry_handle_id,
    bcm_flow_handle_info_t * flow_handle_info,
    dnx_flow_app_config_t * flow_app_info,
    dnx_algo_gpm_gport_hw_resources_t * gport_hw_resources,
    int field_idx)
{
    flow_special_field_info_t *curr_field;
    int rv;

    SHR_FUNC_INIT_VARS(unit);

    curr_field = &flow_special_fields_db[flow_app_info->special_fields[field_idx]];

    if (dnx_flow_special_field_is_ind_set(unit,
                                          curr_field,
                                          FLOW_SPECIAL_FIELD_IND_SRC_ADDR_PROFILE_SUPPORTED)
        && (dnx_flow_app_is_ind_set(unit, flow_app_info, FLOW_APP_INIT_IND_SRC_ADDR_PROFILE)))
    {
        /** logic is done under src profile dedicated logic  */
        SHR_EXIT();
    }
    else if (curr_field->mapped_dbal_field != FLOW_S_FIELD_UNMAPPED)
    {
        /** set directly according to dbal field and payload type */
        switch (curr_field->payload_type)
        {
            case FLOW_PAYLOAD_TYPE_UINT32:
            case FLOW_PAYLOAD_TYPE_ENUM:
            case FLOW_PAYLOAD_TYPE_BCM_ENUM:
            case FLOW_PAYLOAD_TYPE_ENABLER:
            {
                uint32 value = 0;

                rv = dbal_entry_handle_value_field32_get(unit, get_entry_handle_id, curr_field->mapped_dbal_field,
                                                         INST_SINGLE, &value);
                if (rv == _SHR_E_NONE)
                {
                    dbal_entry_value_field32_set(unit, entry_handle_id, curr_field->mapped_dbal_field, INST_SINGLE,
                                                 value);
                }
            }
                break;

            case FLOW_PAYLOAD_TYPE_UINT32_ARR:
            {
                uint32 uint32_arr_value[BCM_FLOW_SPECIAL_FIELD_UIN32_ARR_MAX_SIZE] = { 0 };

                rv = dbal_entry_handle_value_field_arr32_get
                    (unit, get_entry_handle_id, curr_field->mapped_dbal_field, INST_SINGLE, uint32_arr_value);
                if (rv == _SHR_E_NONE)
                {
                    dbal_entry_value_field_arr32_set(unit, entry_handle_id, curr_field->mapped_dbal_field, INST_SINGLE,
                                                     uint32_arr_value);
                }
            }
                break;

            case FLOW_PAYLOAD_TYPE_UINT8_ARR:
            {
                uint8 uint8_arr_value[BCM_FLOW_SPECIAL_FIELD_UIN8_ARR_MAX_SIZE] = { 0 };

                rv = dbal_entry_handle_value_field_arr8_get
                    (unit, get_entry_handle_id, curr_field->mapped_dbal_field, INST_SINGLE, uint8_arr_value);
                if (rv == _SHR_E_NONE)
                {
                    dbal_entry_value_field_arr8_set(unit, entry_handle_id, curr_field->mapped_dbal_field, INST_SINGLE,
                                                    uint8_arr_value);
                }

            }
                break;

            default:
                SHR_ERR_EXIT(_SHR_E_INTERNAL, "unrecognized payload_type %d\n", curr_field->payload_type);
        }
    }
    else
    {
        SHR_ERR_EXIT(_SHR_E_INTERNAL, "field has no mapping %s\n", curr_field->name);
    }

    if (rv == _SHR_E_NOT_FOUND)
    {
        /** rv not found, means that the special field was not set. this is valid. */
        SHR_SET_CURRENT_ERR(_SHR_E_NONE);
    }
    else if (rv != _SHR_E_NONE)
    {
        SHR_SET_CURRENT_ERR(rv);
        SHR_EXIT();
    }

exit:
    SHR_FUNC_EXIT;
}

static shr_error_e
dnx_flow_special_fields_set(
    int unit,
    uint32 entry_handle_id,
    bcm_flow_handle_info_t * flow_handle_info,
    dnx_flow_app_config_t * flow_app_info,
    dnx_algo_gpm_gport_hw_resources_t * gport_hw_resources,
    bcm_flow_special_fields_t * special_fields)
{
    int ii;
    flow_special_field_info_t *curr_field;

    SHR_FUNC_INIT_VARS(unit);

    for (ii = 0; ii < special_fields->actual_nof_special_fields; ii++)
    {
        curr_field = &flow_special_fields_db[special_fields->special_fields[ii].field_id];

        if ((FLOW_APP_IS_MATCH(flow_app_info->flow_app_type) &&
             dnx_flow_special_field_is_ind_set(unit, curr_field, FLOW_SPECIAL_FIELD_IND_MATCH_IGNORE_OPERATION)) ||
            (FLOW_APP_IS_LIF(flow_app_info->flow_app_type) &&
             dnx_flow_special_field_is_ind_set(unit, curr_field, FLOW_SPECIAL_FIELD_IND_LIF_IGNORE_OPERATION)))
        {
            continue;
        }
        SHR_IF_ERR_EXIT(dnx_flow_special_fields_set_one_field(unit, entry_handle_id, flow_handle_info, flow_app_info,
                                                              gport_hw_resources, special_fields, ii));
    }

exit:
    SHR_FUNC_EXIT;
}

/** This function is applicable for Initiator and Terminator applications */
static shr_error_e
dnx_flow_special_fields_destroy(
    int unit,
    uint32 entry_handle_id,
    bcm_flow_handle_info_t * flow_handle_info,
    dnx_flow_app_config_t * flow_app_info,
    dnx_algo_gpm_gport_hw_resources_t * gport_hw_resources)
{
    int sp_field_idx;
    flow_special_field_info_t *curr_field;
    uint8 is_valid;

    SHR_FUNC_INIT_VARS(unit);

    for (sp_field_idx = 0; flow_app_info->special_fields[sp_field_idx] != FLOW_S_F_EMPTY; sp_field_idx++)
    {
        curr_field = &flow_special_fields_db[flow_app_info->special_fields[sp_field_idx]];

        if (dnx_flow_special_field_is_ind_set(unit,
                                              curr_field,
                                              FLOW_SPECIAL_FIELD_IND_SRC_ADDR_PROFILE_SUPPORTED)
            && dnx_flow_app_is_ind_set(unit, flow_app_info, FLOW_APP_INIT_IND_SRC_ADDR_PROFILE))
        {
            /** logic is done under src profile dedicated logic  */
            continue;
        }
        else if (dnx_flow_special_field_is_ind_set(unit,
                                                   curr_field,
                                                   FLOW_SPECIAL_FIELD_IND_LIF_IGNORE_OPERATION)
                 || dnx_flow_special_field_is_ind_set(unit, curr_field, FLOW_SPECIAL_FIELD_IND_IS_KEY))
        {
            continue;
        }
        if (curr_field->clear != NULL)
        {
            dbal_fields_e mapped_dbal_field = curr_field->mapped_dbal_field;

            SHR_IF_ERR_EXIT(dnx_flow_lif_mgmt_field_is_set_in_hw
                            (unit, flow_handle_info, gport_hw_resources, mapped_dbal_field, &is_valid));
            if (is_valid)
            {
                SHR_IF_ERR_EXIT(curr_field->clear
                                (unit, entry_handle_id, flow_handle_info, flow_app_info, gport_hw_resources,
                                 flow_app_info->special_fields[sp_field_idx]));
            }
        }
    }

exit:
    SHR_FUNC_EXIT;
}

/** get the value directly according to mapped dbal field and payload type */
static shr_error_e
flow_special_field_direct_mapping_get(
    int unit,
    flow_special_field_info_t * curr_field,
    uint32 entry_handle_id,
    bcm_flow_special_field_t * special_field)
{
    SHR_FUNC_INIT_VARS(unit);

    switch (curr_field->payload_type)
    {
        case FLOW_PAYLOAD_TYPE_UINT32:
            FLOW_FIELD_GET_ERR_EXIT(dbal_entry_handle_value_field32_get
                                    (unit, entry_handle_id, curr_field->mapped_dbal_field, INST_SINGLE,
                                     &special_field->shr_var_uint32));
            break;

        case FLOW_PAYLOAD_TYPE_UINT32_ARR:
            FLOW_FIELD_GET_ERR_EXIT(dbal_entry_handle_value_field_arr32_get(unit,
                                                                            entry_handle_id,
                                                                            curr_field->mapped_dbal_field,
                                                                            INST_SINGLE,
                                                                            special_field->shr_var_uint32_arr));
            break;

        case FLOW_PAYLOAD_TYPE_UINT8_ARR:
            FLOW_FIELD_GET_ERR_EXIT(dbal_entry_handle_value_field_arr8_get(unit,
                                                                           entry_handle_id,
                                                                           curr_field->mapped_dbal_field,
                                                                           INST_SINGLE,
                                                                           special_field->shr_var_uint8_arr));
            break;

        case FLOW_PAYLOAD_TYPE_ENABLER:
        {
            uint32 value;
            FLOW_FIELD_GET_ERR_EXIT(dbal_entry_handle_value_field32_get(unit,
                                                                        entry_handle_id,
                                                                        curr_field->mapped_dbal_field,
                                                                        INST_SINGLE, &value));
                /** for enabler, if there is a value, it means the field exists, no need to update its data */
                /** if there is no value, it means that the field was not set and so the NOT_FOUND marking*/
            if (value == 0)
            {
                SHR_IF_ERR_EXIT_NO_MSG(_SHR_E_NOT_FOUND);
            }
        }
            break;

        case FLOW_PAYLOAD_TYPE_ENUM:
        case FLOW_PAYLOAD_TYPE_BCM_ENUM:
        {
            uint32 value;
            FLOW_FIELD_GET_ERR_EXIT(dbal_entry_handle_value_field32_get
                                    (unit, entry_handle_id, curr_field->mapped_dbal_field, INST_SINGLE, &value));
            special_field->symbol = (int) value;
        }
            break;

        default:
            SHR_ERR_EXIT(_SHR_E_INTERNAL, "unrecognized payload_type %d\n", curr_field->payload_type);
    }

exit:
    SHR_FUNC_EXIT;
}

/** get the value directly according to mapped dbal field and payload type (for key fields) */
static shr_error_e
flow_special_field_key_direct_mapping_get(
    int unit,
    flow_special_field_info_t * curr_field,
    uint32 entry_handle_id,
    dbal_table_type_e table_type,
    bcm_flow_special_field_t * special_field)
{
    SHR_FUNC_INIT_VARS(unit);

    if (table_type == DBAL_TABLE_TYPE_TCAM)
    {
        /** for TCAM, first validating that the field is set */
        uint32 value[BCM_FLOW_SPECIAL_FIELD_UIN32_ARR_MAX_SIZE] = { 0 };
        uint32 mask[BCM_FLOW_SPECIAL_FIELD_UIN32_ARR_MAX_SIZE] = { 0 };
        SHR_IF_ERR_EXIT(dbal_entry_handle_key_field_arr32_masked_get
                        (unit, entry_handle_id, curr_field->mapped_dbal_field, value, mask));
        if (sal_memcmp(mask, all_zero_array, sizeof(BCM_FLOW_SPECIAL_FIELD_UIN32_ARR_MAX_SIZE) * sizeof(uint32)) == 0)
        {
            SHR_IF_ERR_EXIT_NO_MSG(_SHR_E_UNAVAIL);
        }
    }

    switch (curr_field->payload_type)
    {
        case FLOW_PAYLOAD_TYPE_UINT32:
            SHR_IF_ERR_EXIT(dbal_entry_handle_key_field_arr32_get
                            (unit, entry_handle_id, curr_field->mapped_dbal_field, &special_field->shr_var_uint32));
            break;

        case FLOW_PAYLOAD_TYPE_UINT32_ARR:
            SHR_IF_ERR_EXIT(dbal_entry_handle_key_field_arr32_get(unit,
                                                                  entry_handle_id,
                                                                  curr_field->mapped_dbal_field,
                                                                  special_field->shr_var_uint32_arr));
            break;

        case FLOW_PAYLOAD_TYPE_UINT8_ARR:
            SHR_IF_ERR_EXIT(dbal_entry_handle_key_field_arr8_get(unit,
                                                                 entry_handle_id,
                                                                 curr_field->mapped_dbal_field,
                                                                 special_field->shr_var_uint8_arr));
            break;

        case FLOW_PAYLOAD_TYPE_ENABLER:
            break;

        case FLOW_PAYLOAD_TYPE_ENUM:
        case FLOW_PAYLOAD_TYPE_BCM_ENUM:
        {
            uint32 value;

            SHR_IF_ERR_EXIT(dbal_entry_handle_key_field_arr32_get(unit,
                                                                  entry_handle_id,
                                                                  curr_field->mapped_dbal_field, &value));
            special_field->symbol = (int) value;
        }
            break;

        default:
            SHR_ERR_EXIT(_SHR_E_INTERNAL, "unrecognized payload_type %d\n", curr_field->payload_type);
    }

exit:
    SHR_FUNC_EXIT;
}

static shr_error_e
dnx_flow_special_fields_get(
    int unit,
    uint32 entry_handle_id,
    bcm_flow_handle_info_t * flow_handle_info,
    dnx_flow_app_config_t * flow_app_info,
    dnx_algo_gpm_gport_hw_resources_t * gport_hw_resources,
    bcm_flow_special_fields_t * special_fields)
{
    int idx = 0, rv = _SHR_E_NONE;

    SHR_FUNC_INIT_VARS(unit);

    if (flow_app_info->special_fields[0] != FLOW_S_F_EMPTY)
    {
        int ii;

        sal_memset(special_fields, 0, sizeof(bcm_flow_special_fields_t));

        special_fields->actual_nof_special_fields = 0;

        for (ii = 0; flow_app_info->special_fields[ii] != FLOW_S_F_EMPTY; ii++)
        {
            flow_special_field_info_t *curr_field;

            curr_field = &flow_special_fields_db[flow_app_info->special_fields[ii]];

            if (dnx_flow_special_field_is_ind_set(unit, curr_field, FLOW_SPECIAL_FIELD_IND_LIF_IGNORE_OPERATION))
            {
                continue;
            }
            if (curr_field->get != NULL)
            {
                rv = curr_field->get
                    (unit, entry_handle_id, flow_handle_info, flow_app_info, gport_hw_resources,
                     flow_app_info->special_fields[ii], &(special_fields->special_fields[idx]));
                if (rv == _SHR_E_NONE)
                {
                    idx++;
                    continue;
                }
                else
                {
                    if (rv != _SHR_E_NOT_FOUND)
                    {
                        SHR_ERR_EXIT(_SHR_E_PARAM, "issue when getting field %s ", curr_field->name);
                    }
                    else
                    {
                        continue;
                    }
                }
            }

            if (dnx_flow_special_field_is_ind_set(unit, curr_field, FLOW_SPECIAL_FIELD_IND_SRC_ADDR_PROFILE_SUPPORTED)
                && dnx_flow_app_is_ind_set(unit, flow_app_info, FLOW_APP_INIT_IND_SRC_ADDR_PROFILE))
            {
                /** logic is done under src profile dedicated logic  */
                continue;
            }

            if (curr_field->mapped_dbal_field != FLOW_S_FIELD_UNMAPPED)
            {
                int is_virtual_match_key =
                    dnx_flow_special_field_is_ind_set(unit, curr_field, FLOW_SPECIAL_FIELD_IND_IS_KEY);
                if (!is_virtual_match_key)
                {
                    rv = flow_special_field_direct_mapping_get(unit, curr_field, entry_handle_id,
                                                               &(special_fields->special_fields[idx]));
                }
                else
                {
                    rv = flow_special_field_key_direct_mapping_get(unit, curr_field, entry_handle_id,
                                                                   DBAL_TABLE_TYPE_NONE,
                                                                   &(special_fields->special_fields[idx]));
                }

                if (rv == _SHR_E_NONE)
                {
                    special_fields->special_fields[idx].field_id = flow_app_info->special_fields[ii];
                    idx++;
                }
                else
                {
                    if (rv != _SHR_E_NOT_FOUND)
                    {
                        SHR_ERR_EXIT(rv, "issue when getting field %s ", curr_field->name);
                    }
                }
                continue;
            }

            /** reaching here means that the field was not handled before, this is wrong */
            SHR_ERR_EXIT(_SHR_E_PARAM, "error mapping field %s ", curr_field->name);

        }

        special_fields->actual_nof_special_fields = idx;
    }

exit:
    SHR_FUNC_EXIT;
}

static shr_error_e
dnx_flow_unchangeable_special_field_replace_verify(
    int unit,
    bcm_flow_handle_info_t * flow_handle_info,
    dnx_flow_app_config_t * flow_app_info,
    uint32 get_entry_handle_id,
    dnx_algo_gpm_gport_hw_resources_t * gport_hw_resources,
    bcm_flow_special_field_t * special_field)
{
    shr_error_e rv;
    uint8 replace_valid = TRUE;
    flow_special_field_info_t *curr_field = &flow_special_fields_db[special_field->field_id];

    SHR_FUNC_INIT_VARS(unit);

    /** unchangeable field, which are not mandatory, must be either direct mapping or have get_cb */
    if (curr_field->mapped_dbal_field != FLOW_S_FIELD_UNMAPPED)
    {
        SHR_IF_ERR_EXIT((dnx_flow_lif_mgmt_field_is_set_in_hw(unit, flow_handle_info, gport_hw_resources,
                                                              special_field->field_id, &replace_valid)));
    }
    else        /* get_cb */
    {
        rv = curr_field->get(unit, get_entry_handle_id, flow_handle_info, flow_app_info, NULL,
                             special_field->field_id, special_field);
        SHR_IF_ERR_EXIT_EXCEPT_IF(rv, _SHR_E_NOT_FOUND);

        if (rv == _SHR_E_NONE)
        {
            /** cannot update an already set unchangeable field */
            replace_valid = FALSE;
        }
    }

    if (replace_valid == FALSE)
    {
        SHR_ERR_EXIT(_SHR_E_PARAM, "Cannot replace value of an already set unchangeable field (%s)",
                     dnx_flow_special_field_to_string(unit, special_field->field_id));
    }

exit:
    SHR_FUNC_EXIT;
}

static shr_error_e
dnx_flow_special_fields_replace(
    int unit,
    uint32 entry_handle_id,
    uint32 get_entry_handle_id,
    bcm_flow_handle_info_t * flow_handle_info,
    dnx_flow_app_config_t * flow_app_info,
    dnx_algo_gpm_gport_hw_resources_t * gport_hw_resources,
    bcm_flow_special_fields_t * special_fields)
{
    int field = 0, used_field = 0;
    uint32 special_fields_bitmap[1] = { 0 };
    flow_special_field_info_t *curr_field;
    uint32 actual_nof_special_fields;

    SHR_FUNC_INIT_VARS(unit);

    if (special_fields == NULL)
    {
        actual_nof_special_fields = 0;
    }
    else
    {
        actual_nof_special_fields = special_fields->actual_nof_special_fields;
    }
    /*
     * Iterate over app special fields, set bit for each supported field 
     */
    while (flow_app_info->special_fields[field] != FLOW_S_F_EMPTY)
    {
        SHR_BITSET(special_fields_bitmap, field);
        field++;
    }

    for (field = 0; field < actual_nof_special_fields; field++)
    {
        used_field = 0;

        /*
         * Find the field set by the user in the application supported special fields, later to be marked as used
         */
        while ((flow_app_info->special_fields[used_field] != special_fields->special_fields[field].field_id) &&
               (flow_app_info->special_fields[used_field] != FLOW_S_F_EMPTY))
        {
            used_field++;
        }

        if (flow_app_info->special_fields[used_field] == FLOW_S_F_EMPTY)
        {
            SHR_ERR_EXIT(_SHR_E_PARAM, "Couldn't find special field given %s in app",
                         dnx_flow_special_field_to_string(unit, special_fields->special_fields[field].field_id));
        }

        /*
         * Clear the field bit in the bitmap, this way we're marking it as if it was already handled. Later - we
         * iterate over this bitmap, and each set bit - we take the original value (if existed) 
         */
        SHR_BITCLR(special_fields_bitmap, used_field);

        curr_field = &flow_special_fields_db[special_fields->special_fields[field].field_id];

        if (dnx_flow_special_field_is_ind_set(unit, curr_field, FLOW_SPECIAL_FIELD_IND_LIF_IGNORE_OPERATION))
        {
            continue;
        }

        /*
         * If is_clear is off and field was set --> set requested value
         * Special fields with is_clear set should be ignored
         */
        if (!special_fields->special_fields[field].is_clear)
        {
            /** for unchangeable field, which aren't mandatory, which have a mapped dbal field, for check if the field was previously set */
            if (dnx_flow_special_field_is_ind_set(unit, curr_field, FLOW_SPECIAL_FIELD_IND_LIF_UNCHANGEABLE) &&
                (!dnx_flow_special_field_is_ind_set(unit, curr_field, FLOW_SPECIAL_FIELD_IND_LIF_MANDATORY)))
            {
                SHR_IF_ERR_EXIT(dnx_flow_unchangeable_special_field_replace_verify
                                (unit, flow_handle_info, flow_app_info, get_entry_handle_id, gport_hw_resources,
                                 &(special_fields->special_fields[field])));
            }

            if (dnx_flow_app_is_ind_set(unit, flow_app_info, FLOW_APP_INIT_IND_SRC_ADDR_PROFILE) &&
                dnx_flow_special_field_is_ind_set(unit, curr_field, FLOW_SPECIAL_FIELD_IND_SRC_ADDR_PROFILE_SUPPORTED))
            {
                /** logic is done under src profile dedicated logic  */
                continue;
            }
            else if (curr_field->replace != NULL)
            {
                SHR_IF_ERR_EXIT(curr_field->replace
                                (unit, entry_handle_id, get_entry_handle_id,
                                 flow_handle_info, flow_app_info, gport_hw_resources,
                                 &special_fields->special_fields[field]));

            }
            else
            {
                /** in case of regular mapping, just need to update the new value */
                SHR_IF_ERR_EXIT(dnx_flow_special_fields_set_one_field
                                (unit, entry_handle_id, flow_handle_info, flow_app_info, gport_hw_resources,
                                 special_fields, field));
            }
        }
        else
        {
            if (dnx_flow_app_is_ind_set(unit, flow_app_info, FLOW_APP_INIT_IND_SRC_ADDR_PROFILE) &&
                dnx_flow_special_field_is_ind_set(unit, curr_field, FLOW_SPECIAL_FIELD_IND_SRC_ADDR_PROFILE_SUPPORTED))
            {
                /** logic is done in src_profile dedicated logic */
                continue;
            }
            else if (curr_field->clear != NULL)
            {
                SHR_IF_ERR_EXIT(curr_field->clear
                                (unit, entry_handle_id, flow_handle_info, flow_app_info, gport_hw_resources,
                                 special_fields->special_fields[field].field_id));
                continue;
            }
        }
    }

    /*
     * Iterate on the special fields bitmap. Each set field is a field which the user didn't relate to. This field
     * should keep its original value (if existed) 
     */
    field = 0;
    while (special_fields_bitmap[0])
    {
        if (special_fields_bitmap[0] & 0x1)
        {
            curr_field = &flow_special_fields_db[flow_app_info->special_fields[field]];

            if (dnx_flow_special_field_is_ind_set(unit, curr_field, FLOW_SPECIAL_FIELD_IND_LIF_IGNORE_OPERATION) ||
                dnx_flow_special_field_is_ind_set(unit, curr_field, FLOW_SPECIAL_FIELD_IND_IS_KEY))
            {
                special_fields_bitmap[0] = (special_fields_bitmap[0] >> 1);
                field++;
                continue;
            }
            SHR_IF_ERR_EXIT(dnx_flow_special_fields_copy_one_field(unit, entry_handle_id, get_entry_handle_id,
                                                                   flow_handle_info,
                                                                   flow_app_info, gport_hw_resources, field));
        }
        special_fields_bitmap[0] = (special_fields_bitmap[0] >> 1);
        field++;
    }

exit:
    SHR_FUNC_EXIT;
}

static shr_error_e
dnx_flow_common_fields_set(
    int unit,
    dnx_flow_app_config_t * flow_app_info,
    dnx_flow_app_type_e flow_app_type,
    bcm_flow_handle_info_t * flow_handle_info,
    void *bcm_struct_info,
    dnx_algo_gpm_gport_hw_resources_t * gport_hw_resources,
    uint32 entry_handle_id)
{
    int field = 0;
    dnx_flow_common_field_desc_t **common_fields_arr;
    uint32 valid_elements_set;

    SHR_FUNC_INIT_VARS(unit);

    if (flow_app_type == FLOW_APP_TYPE_TERM)
    {
        common_fields_arr = dnx_flow_term_common_fields;
        valid_elements_set = ((bcm_flow_terminator_info_t *) bcm_struct_info)->valid_elements_set;
    }
    else
    {
        common_fields_arr = dnx_flow_init_common_fields;
        valid_elements_set = ((bcm_flow_initiator_info_t *) bcm_struct_info)->valid_elements_set;
    }

    /*
     * Iterate on the common fields. If a field flag was set by the user - call relevant callback 
     */
    while (valid_elements_set != 0)
    {
        if (valid_elements_set & 0x1)
        {
            if (common_fields_arr[field]->field_cbs.field_set_cb)
            {
                /** generic call to the common field set callback */
                SHR_IF_ERR_EXIT(common_fields_arr[field]->
                                field_cbs.field_set_cb(unit, entry_handle_id, flow_app_type, flow_handle_info,
                                                       gport_hw_resources, flow_app_info, bcm_struct_info));
            }
            else
            {
                SHR_ERR_EXIT(_SHR_E_PARAM, "common field has no set CB Application %s, common field set, %s",
                             flow_app_info->app_name, common_fields_arr[field]->field_name);
            }
        }
        valid_elements_set = (valid_elements_set >> 1);
        field++;
    }

exit:
    SHR_FUNC_EXIT;
}

static shr_error_e
dnx_flow_common_fields_replace(
    int unit,
    dnx_flow_app_config_t * flow_app_info,
    dnx_flow_app_type_e flow_app_type,
    bcm_flow_handle_info_t * flow_handle_info,
    void *bcm_struct_info,
    dnx_algo_gpm_gport_hw_resources_t * gport_hw_resources,
    uint32 entry_handle_id,
    uint32 get_entry_handle_id)
{
    int num_common_fields;
    int field;
    dnx_flow_common_field_desc_t **common_fields_arr;
    uint32 valid_elements_set, valid_elements_clear;
    bcm_flow_initiator_info_t get_initiator_info = { 0 };
    bcm_flow_terminator_info_t get_terminator_info = { 0 };
    bcm_flow_terminator_info_t *terminator_info = NULL;
    bcm_flow_initiator_info_t *initiator_info = NULL;

    int rv;
    void *get_info;

    SHR_FUNC_INIT_VARS(unit);

    if (flow_app_type == FLOW_APP_TYPE_TERM)
    {
        common_fields_arr = dnx_flow_term_common_fields;

        /** get_terminator_info will be used for profiles delete  */
        sal_memcpy(&get_terminator_info, ((bcm_flow_terminator_info_t *) bcm_struct_info),
                   sizeof(bcm_flow_terminator_info_t));
        get_info = &get_terminator_info;
        terminator_info = ((bcm_flow_terminator_info_t *) bcm_struct_info);
        valid_elements_set = terminator_info->valid_elements_set;
        valid_elements_clear = terminator_info->valid_elements_clear;
    }
    else
    {
        common_fields_arr = dnx_flow_init_common_fields;

        /** get_initiator_info will be used for profiles delete  */
        sal_memcpy(&get_initiator_info, ((bcm_flow_initiator_info_t *) bcm_struct_info),
                   sizeof(bcm_flow_initiator_info_t));
        get_info = &get_initiator_info;
        initiator_info = ((bcm_flow_initiator_info_t *) bcm_struct_info);
        valid_elements_set = initiator_info->valid_elements_set;
        valid_elements_clear = initiator_info->valid_elements_clear;
    }

    SHR_IF_ERR_EXIT(dnx_flow_number_of_common_fields(unit, flow_app_type, &num_common_fields));

    /*
     * Iterate over the flow common fields. If requested by the user - call relevant callback
     */
    for (field = 0; field < num_common_fields; field++)
    {
        uint32 field_valid_bit = (1 << field);

        /*
         * If the application does not support this common field, skip all set/get actions
         * Reasoning:
         * If not skipped, the user should be aware which common fields are not supported for the application
         * and set the corresponding "valid_elements_clear" bit in order to skip get during replace
         */
        if (!_SHR_IS_FLAG_SET(flow_app_info->valid_common_fields_bitmap, field_valid_bit))
        {
            continue;
        }

        /*
         * If valid_elements is set --> field should be replaced. Call replace cb
         */
        if (_SHR_IS_FLAG_SET(valid_elements_set, field_valid_bit))
        {
            /*
             * For standard fields - perform simple set
             * For profiles etc - a replace cb can be defined, in case the old profile should be deleted after creating the new one etc
             */
            if (common_fields_arr[field]->field_cbs.field_replace_cb)
            {
                SHR_IF_ERR_EXIT(common_fields_arr[field]->
                                field_cbs.field_replace_cb(unit, entry_handle_id, get_entry_handle_id, flow_app_type,
                                                           flow_handle_info, gport_hw_resources, flow_app_info,
                                                           bcm_struct_info));
            }
            else if (common_fields_arr[field]->field_cbs.field_set_cb)
            {
                SHR_IF_ERR_EXIT(common_fields_arr[field]->
                                field_cbs.field_set_cb(unit, entry_handle_id, flow_app_type, flow_handle_info,
                                                       gport_hw_resources, flow_app_info, bcm_struct_info));
            }
        }
        /*
         * If no flag was set - keep original field value. Perform get on original entry. If exists - set previous
         * value. Otherwise - leave default value
         */
        else if (!_SHR_IS_FLAG_SET(valid_elements_clear, field_valid_bit))
        {
            if (common_fields_arr[field]->field_cbs.field_get_cb)
            {
                rv = common_fields_arr[field]->field_cbs.field_get_cb(unit, get_entry_handle_id, flow_app_type,
                                                                      flow_handle_info, gport_hw_resources,
                                                                      flow_app_info, bcm_struct_info);

                if (rv == _SHR_E_NONE)
                {
                    if (common_fields_arr[field]->field_cbs.field_set_cb)
                    {
                        SHR_IF_ERR_EXIT(common_fields_arr[field]->
                                        field_cbs.field_set_cb(unit, entry_handle_id, flow_app_type, flow_handle_info,
                                                               gport_hw_resources, flow_app_info, bcm_struct_info));
                    }
                }
                else if (rv == _SHR_E_NOT_FOUND)
                {
                    SHR_SET_CURRENT_ERR(_SHR_E_NONE);
                }
                else
                {
                    SHR_SET_CURRENT_ERR(rv);
                    SHR_EXIT();
                }
            }
        }
        /*
         * Valid elements clear - in case of a profile with a delete cb - call this cb to free the profile. No need to set anything
         */
        else
        {
            if ((common_fields_arr[field]->field_cbs.field_delete_cb)
                && (common_fields_arr[field]->field_cbs.field_get_cb))
            {
                rv = common_fields_arr[field]->field_cbs.field_get_cb(unit, get_entry_handle_id, flow_app_type,
                                                                      flow_handle_info, gport_hw_resources,
                                                                      flow_app_info, get_info);
                /*
                 * If profile existed on entry - need to restore it to default value
                 */
                if (rv == _SHR_E_NONE)
                {
                    SHR_IF_ERR_EXIT_EXCEPT_IF(common_fields_arr[field]->
                                              field_cbs.field_delete_cb(unit, entry_handle_id, flow_app_type,
                                                                        flow_handle_info, gport_hw_resources,
                                                                        flow_app_info, bcm_struct_info),
                                              _SHR_E_NOT_FOUND);
                }
            }
        }
    }

exit:
    SHR_FUNC_EXIT;
}

static shr_error_e
dnx_flow_common_fields_get(
    int unit,
    dnx_flow_app_type_e flow_app_type,
    dnx_flow_app_config_t * flow_app_info,
    bcm_flow_handle_info_t * flow_handle_info,
    bcm_flow_initiator_info_t * initiator_info,
    bcm_flow_terminator_info_t * terminator_info,
    dnx_algo_gpm_gport_hw_resources_t * gport_hw_resources,
    uint32 entry_handle_id)
{
    int field = 0;
    shr_error_e rv;
    dnx_flow_common_field_desc_t **common_fields_arr;
    uint32 temp_common_bitmap = flow_app_info->valid_common_fields_bitmap;
    void *bcm_struct_info;

    SHR_FUNC_INIT_VARS(unit);

    if (flow_app_type == FLOW_APP_TYPE_TERM)
    {
        common_fields_arr = dnx_flow_term_common_fields;
        terminator_info->valid_elements_set = 0;
        bcm_struct_info = terminator_info;
    }
    else
    {
        common_fields_arr = dnx_flow_init_common_fields;
        initiator_info->valid_elements_set = 0;
        bcm_struct_info = initiator_info;
    }

    /*
     * Iterate over all common fields, and call relevant get callback.
     * If not found - ignore, field didn't exist on entry.
     * Mark the found fields in the valid_elements_set bitmap.
     */
    while (temp_common_bitmap != 0)
    {
        if ((temp_common_bitmap & 0x1) && (common_fields_arr[field]->field_cbs.field_get_cb))
        {
            rv = common_fields_arr[field]->field_cbs.field_get_cb(unit, entry_handle_id,
                                                                  flow_app_type,
                                                                  flow_handle_info,
                                                                  gport_hw_resources, flow_app_info, bcm_struct_info);
            if (rv == _SHR_E_NONE)
            {
                if (flow_app_type == FLOW_APP_TYPE_TERM)
                {
                    terminator_info->valid_elements_set |= common_fields_arr[field]->term_field_enabler;
                }
                else
                {
                    initiator_info->valid_elements_set |= common_fields_arr[field]->init_field_enabler;
                }
            }
            else if (rv != _SHR_E_NOT_FOUND)
            {
                SHR_ERR_EXIT(rv, "Error, retrieving flow field %s", common_fields_arr[field]->field_name);
            }
        }
        temp_common_bitmap = (temp_common_bitmap >> 1);
        field++;
    }

exit:
    SHR_FUNC_EXIT;
}


shr_error_e
dnx_flow_local_lif_to_flow_id(
    int unit,
    const dnx_flow_app_config_t * flow_app_info,
    bcm_flow_handle_info_t * flow_handle_info,
    uint32 lif,
    int core_id)
{
    lif_mapping_local_lif_key_t local_lif_info;
    int global_lif, is_virtual = FALSE;
    shr_error_e rv;
    uint32 mapping_flags =
        (flow_app_info->flow_app_type == FLOW_APP_TYPE_TERM_MATCH) ? DNX_ALGO_LIF_INGRESS : DNX_ALGO_LIF_EGRESS;

    SHR_FUNC_INIT_VARS(unit);

    sal_memset(&local_lif_info, 0, sizeof(local_lif_info));

    /** in in_lif_phy_db_dpc the LIF is created over all cores. When !in_lif_phy_db_dpc, there is a need to check to
     *  which LIF phy DB the lif is related in order to find the mapping */
    if (flow_app_info->flow_app_type == FLOW_APP_TYPE_TERM_MATCH)
    {
        if (!dnx_data_lif.in_lif.feature_get(unit, dnx_data_lif_in_lif_phy_db_dpc))
        {
            bcm_flow_handle_t match_payload_flow_handle_id;
            dnx_flow_app_config_t *payload_flow_app_info;

            SHR_IF_ERR_EXIT(bcm_dnx_flow_handle_get(unit,
                                                    flow_app_info->match_payload_apps[0],
                                                    &match_payload_flow_handle_id));
            payload_flow_app_info = dnx_flow_objects[match_payload_flow_handle_id];

            SHR_IF_ERR_EXIT(dbal_tables_physical_table_get(unit,
                                                           FLOW_LIF_DBAL_TABLE_GET(payload_flow_app_info),
                                                           DBAL_PHY_DB_DEFAULT_INDEX, &(local_lif_info.phy_table)));
        }
    }

    local_lif_info.local_lif = lif;
    local_lif_info.core_id = core_id;
    rv = dnx_algo_lif_mapping_local_to_global_get(unit, mapping_flags, &local_lif_info, &global_lif);
    SHR_IF_ERR_EXIT_EXCEPT_IF(rv, _SHR_E_NOT_FOUND);
    if (rv == _SHR_E_NOT_FOUND)
    {
        is_virtual = TRUE;
    }

    if (flow_app_info->flow_app_type == FLOW_APP_TYPE_INIT_MATCH)
    {
        BCM_GPORT_SUB_TYPE_OUT_LIF_SET(flow_handle_info->flow_id, global_lif);
    }
    else if (!is_virtual)
    {
        BCM_GPORT_SUB_TYPE_IN_LIF_SET(flow_handle_info->flow_id, global_lif);
    }
    else
    {
        BCM_GPORT_SUB_TYPE_VIRTUAL_INGRESS_SET(flow_handle_info->flow_id, global_lif);
    }

    BCM_GPORT_FLOW_LIF_ID_SET(flow_handle_info->flow_id, flow_handle_info->flow_id);

exit:
    SHR_FUNC_EXIT;
}

static shr_error_e
dnx_flow_global_lif_allocate(
    int unit,
    bcm_flow_handle_info_t * flow_handle_info,
    dnx_flow_app_config_t * flow_app_info,
    global_lif_alloc_info_t * alloc_info)
{
    int global_lif = DNX_ALGO_GPM_LIF_INVALID;
    uint32 alloc_flags = 0;
    uint32 direction_flags =
        (flow_app_info->flow_app_type == FLOW_APP_TYPE_TERM) ? DNX_ALGO_LIF_INGRESS : DNX_ALGO_LIF_EGRESS;

    SHR_FUNC_INIT_VARS(unit);

    if (_SHR_IS_FLAG_SET(flow_handle_info->flags, BCM_FLOW_HANDLE_INFO_WITH_ID))
    {
        alloc_flags |= LIF_MNGR_GLOBAL_LIF_WITH_ID;
        global_lif = _SHR_GPORT_FLOW_LIF_VAL_GET_ID(flow_handle_info->flow_id);
    }

    if (((flow_app_info->flow_app_type == FLOW_APP_TYPE_INIT)
         && dnx_flow_app_is_ind_set(unit, flow_app_info, FLOW_APP_INIT_IND_L2_GLOBAL_LIF))
        || ((flow_app_info->flow_app_type == FLOW_APP_TYPE_TERM)
            && dnx_flow_app_is_ind_set(unit, flow_app_info, FLOW_APP_TERM_IND_L2_GLOBAL_LIF)))
    {
        alloc_flags |= LIF_MNGR_L2_GPORT_GLOBAL_LIF;
    }

    if (_SHR_IS_FLAG_SET(flow_handle_info->flags, BCM_FLOW_HANDLE_INFO_SYMMETRIC_ALLOC))
    {
        /** Allocate as symmetric only if it wasn't already allocated or reserved as such */
        uint8 is_symmetric_allocated = FALSE;
        if (_SHR_IS_FLAG_SET(flow_handle_info->flags, BCM_FLOW_HANDLE_INFO_WITH_ID))
        {
            SHR_IF_ERR_EXIT(lif_mngr_db.global_lif_allocation.
                            symmetric_global_lif.is_allocated(unit, global_lif, &is_symmetric_allocated));
        }

        if (!is_symmetric_allocated)
        {
            alloc_flags |= LIF_MNGR_ONE_SIDED_SYMMETRIC_GLOBAL_LIF;
        }

    }

    SHR_IF_ERR_EXIT(dnx_algo_global_lif_allocation_allocate
                    (unit, alloc_flags, direction_flags, alloc_info, &global_lif));

    if (flow_app_info->flow_app_type == FLOW_APP_TYPE_INIT)
    {
        BCM_GPORT_SUB_TYPE_OUT_LIF_SET(flow_handle_info->flow_id, global_lif);
    }
    else
    {
        BCM_GPORT_SUB_TYPE_IN_LIF_SET(flow_handle_info->flow_id, global_lif);
    }
    BCM_GPORT_FLOW_LIF_ID_SET(flow_handle_info->flow_id, flow_handle_info->flow_id);

exit:
    SHR_FUNC_EXIT;
}

/** allocate a flow ID - it can be a global lif or virtual ID according to the application / flags */
static shr_error_e
dnx_flow_gport_flow_id_allocate(
    int unit,
    bcm_flow_handle_info_t * flow_handle_info,
    dnx_flow_app_config_t * flow_app_info,
    global_lif_alloc_info_t * alloc_info,
    int is_local_lif)
{
    SHR_FUNC_INIT_VARS(unit);

    if (_SHR_IS_FLAG_SET(flow_handle_info->flags, BCM_FLOW_HANDLE_INFO_VIRTUAL))
    {
        if (flow_app_info->flow_app_type == FLOW_APP_TYPE_INIT)
        {
            SHR_IF_ERR_EXIT(dnx_flow_egress_virtual_gport_allocate
                            (unit, flow_handle_info, flow_app_info, is_local_lif));
        }
        else
        {
            SHR_IF_ERR_EXIT(dnx_flow_ingress_virtual_gport_allocate(unit, flow_handle_info, flow_app_info));
        }
        SHR_EXIT();
    }
    else
    {
        SHR_IF_ERR_EXIT(dnx_flow_global_lif_allocate(unit, flow_handle_info, flow_app_info, alloc_info));
    }

exit:
    SHR_FUNC_EXIT;
}

/** free the flow ID - it can be a global lif or virtual ID according to the application / flags */
static shr_error_e
dnx_flow_gport_flow_id_free(
    int unit,
    dnx_flow_app_config_t * flow_app_info,
    bcm_flow_handle_info_t * flow_handle_info,
    dnx_algo_gpm_gport_hw_resources_t * gport_hw_resources)
{
    int is_virtual = FLOW_GPORT_IS_VIRTUAL(flow_handle_info->flow_id);

    SHR_FUNC_INIT_VARS(unit);

    if (is_virtual)
    {
        if (flow_app_info->flow_app_type == FLOW_APP_TYPE_INIT)
        {
            SHR_IF_ERR_EXIT(dnx_flow_egress_virtual_gport_free(unit, flow_handle_info->flow_id));

        }
        else
        {
            SHR_IF_ERR_EXIT(dnx_flow_ingress_virtual_gport_free(unit, flow_handle_info->flow_id));
        }
    }
    else
    {
        uint32 direction_flags = DNX_ALGO_LIF_INGRESS;
        int global_lif = gport_hw_resources->global_in_lif;
        if (flow_app_info->flow_app_type == FLOW_APP_TYPE_INIT)
        {
            direction_flags = DNX_ALGO_LIF_EGRESS;
            global_lif = gport_hw_resources->global_out_lif;

        }
        SHR_IF_ERR_EXIT(dnx_algo_global_lif_allocation_free(unit, direction_flags, global_lif));
    }

exit:
    SHR_FUNC_EXIT;
}

/* Match traverse cb, used by dnx_flow_match_info_by_gport_multiple_get(). If entry gport matches the required gport - update relevant data */
static int
flow_app_match_entry_cb(
    int unit,
    bcm_flow_handle_info_t * flow_handle_info,
    bcm_flow_special_fields_t * key_special_fields,
    void *user_data)
{
    dnx_flow_match_gport_traverse_data_t *traverse_flow_data;

    SHR_FUNC_INIT_VARS(unit);

    /*
     * Retrieve prt_control_t from the user_data
     */
    traverse_flow_data = (dnx_flow_match_gport_traverse_data_t *) user_data;

    if (traverse_flow_data->gport != flow_handle_info->flow_id)
    {
        /*
         * We're only interested in entries with a matching gport 
         */
        SHR_SET_CURRENT_ERR(_SHR_E_NONE);
        SHR_EXIT();
    }

    if (traverse_flow_data->entries->nof_entries >= FLOW_MAX_NOF_MATCH_ENTRIES)
    {
        SHR_ERR_EXIT(_SHR_E_FULL, "Nof match entries exceeds the max allowed (%d)", FLOW_MAX_NOF_MATCH_ENTRIES);
    }
    sal_memcpy(&traverse_flow_data->entries->match_entries[traverse_flow_data->entries->nof_entries].special_fields,
               key_special_fields, sizeof(bcm_flow_special_fields_t));
    sal_memcpy(&traverse_flow_data->entries->match_entries[traverse_flow_data->entries->nof_entries].flow_handle_info,
               flow_handle_info, sizeof(bcm_flow_handle_info_t));
    traverse_flow_data->entries->nof_entries++;

exit:
    SHR_FUNC_EXIT;
}

/* Get match entries which payload is a given gport */
shr_error_e
dnx_flow_match_info_by_gport_multiple_get(
    int unit,
    bcm_gport_t gport,
    flow_match_entries_t * match_entries)
{
    int flow_app_idx, lif_app_idx, flow_number_of_apps;
    bcm_flow_handle_info_t flow_handle_info = { 0 };
    bcm_flow_handle_t flow_handle_id = -1;
    const dnx_flow_app_config_t *flow_app_info;
    dnx_flow_match_gport_traverse_data_t traverse_data;

    SHR_FUNC_INIT_VARS(unit);

    /*
     * First - find the non-match application which is related to this gport 
     */
    SHR_IF_ERR_EXIT(flow_lif_flow_app_from_encoded_gport_get(unit, gport, &flow_handle_id));

    flow_app_info = dnx_flow_app_info_get(unit, flow_handle_id);

    flow_number_of_apps = dnx_flow_number_of_apps();

    traverse_data.gport = gport;
    traverse_data.entries = match_entries;

    /*
     * Run over the flow apps 
     */
    for (flow_app_idx = FLOW_APP_FIRST_IDX; flow_app_idx < flow_number_of_apps; flow_app_idx++)
    {
        const dnx_flow_app_config_t *match_flow_app_info = dnx_flow_app_info_get(unit, flow_app_idx);
        if (match_flow_app_info->flow_app_type != FLOW_APP_TYPE_TERM_MATCH)
        {
            continue;
        }

        /*
         * Run over the match related init or term apps 
         */
        for (lif_app_idx = 0; lif_app_idx < FLOW_MAX_NOF_MATCH_PAYLOAD_APPS; lif_app_idx++)
        {
            if (!sal_strncasecmp
                (match_flow_app_info->match_payload_apps[lif_app_idx], flow_app_info->app_name, FLOW_STR_MAX))
            {
                int table_idx;

                SHR_IF_ERR_EXIT(bcm_flow_handle_get
                                (unit, match_flow_app_info->app_name, &flow_handle_info.flow_handle));

                /*
                 * Traverse over the entries, look for the requested gport 
                 */
                for (table_idx = 0; table_idx < FLOW_MAX_NOF_MATCH_DBAL_TABLES; table_idx++)
                {
                    dbal_tables_e curr_table = match_flow_app_info->flow_table[table_idx];

                    if (curr_table == DBAL_TABLE_EMPTY)
                    {
                        break;
                    }

                    SHR_IF_ERR_EXIT(bcm_flow_match_info_traverse
                                    (unit, &flow_handle_info, flow_app_match_entry_cb, &traverse_data));
                }
            }
        }
    }

exit:
    SHR_FUNC_EXIT;
}

/**********************************************************************************************
*                                  TERMINATOR functions - START                                    *
**********************************************************************************************/
/*
 * Internal ingress flow lif replace
 */
static shr_error_e
dnx_flow_terminator_info_replace(
    int unit,
    bcm_flow_handle_info_t * flow_handle_info,
    dnx_flow_cmd_control_info_t * flow_cmd_control,
    bcm_flow_terminator_info_t * terminator_info,
    bcm_flow_special_fields_t * special_fields)
{
    dnx_algo_gpm_gport_hw_resources_t gport_hw_resources;
    uint32 entry_handle_id, get_entry_handle_id = 0;
    dnx_flow_app_config_t *flow_app_info = dnx_flow_objects[flow_handle_info->flow_handle];
    uint32 selectable_result_types = 0;
    int is_virtual = FLOW_GPORT_IS_VIRTUAL(flow_handle_info->flow_id), global_in_lif_field_val = 0;
    dnx_flow_logger_data_t old_logger_data = { 0 };
    int last_qos_profile, last_trap_profile, last_mpls_term_profile = FLOW_PROFILE_INVALID;

    SHR_FUNC_INIT_VARS(unit);
    DBAL_FUNC_INIT_VARS(unit);

    DNX_FLOW_LOGGER_START(unit, flow_handle_info, flow_app_info, flow_cmd_control, terminator_info,
                          special_fields, logger_data);

    /** Take handle to write to DBAL table*/
    SHR_IF_ERR_EXIT(DBAL_HANDLE_ALLOC(unit, FLOW_LIF_DBAL_TABLE_GET(flow_app_info), &entry_handle_id));

    /** Result type is superset, will be decided internally by lif table manager */
    dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_RESULT_TYPE, INST_SINGLE, DBAL_SUPERSET_RESULT_TYPE);

    /** Take handle to read from DBAL table */
    SHR_IF_ERR_EXIT(DBAL_HANDLE_ALLOC(unit, FLOW_LIF_DBAL_TABLE_GET(flow_app_info), &get_entry_handle_id));

    /** Get Local inLIF ID */
    SHR_IF_ERR_EXIT(dnx_algo_gpm_gport_to_hw_resources(unit, flow_handle_info->flow_id,
                                                       DNX_ALGO_GPM_GPORT_HW_RESOURCES_LOCAL_LIF_INGRESS |
                                                       DNX_ALGO_GPM_GPORT_HW_RESOURCES_GLOBAL_LIF_INGRESS,
                                                       &gport_hw_resources));

    SHR_IF_ERR_EXIT(dnx_flow_lif_mgmt_terminator_info_get
                    (unit, flow_app_info, flow_handle_info, &gport_hw_resources, get_entry_handle_id));

    /** for replace calling verify just after get */
    if (flow_app_info->verify_cb)
    {
        dnx_flow_cmd_control_info_t verify_cmd_control = { FLOW_CB_TYPE_SET, FLOW_COMMAND_CONTINUE };

        SHR_IF_ERR_EXIT(flow_app_info->verify_cb
                        (unit, flow_handle_info, &verify_cmd_control, (void *) (terminator_info), special_fields,
                         get_entry_handle_id));
    }

    if (!is_virtual)
    {
        global_in_lif_field_val = _SHR_GPORT_FLOW_LIF_VAL_GET_ID(flow_handle_info->flow_id);
        dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_GLOB_IN_LIF, INST_SINGLE,
                                     global_in_lif_field_val);
    }

    SHR_IF_ERR_EXIT(dnx_flow_common_fields_replace
                    (unit, flow_app_info, FLOW_APP_TYPE_TERM, flow_handle_info, terminator_info, &gport_hw_resources,
                     entry_handle_id, get_entry_handle_id));

    SHR_IF_ERR_EXIT(dnx_flow_special_fields_replace(unit, entry_handle_id, get_entry_handle_id, flow_handle_info,
                                                    flow_app_info, &gport_hw_resources, special_fields));

    /** Check if errors occurred on handle, using DBAL validation to make sure that the user values are correct. */
    SHR_IF_ERR_EXIT(dnx_flow_commit_error_check(unit, entry_handle_id));

    SHR_IF_ERR_EXIT(dnx_flow_dl_ingress_qos_profile_replace
                    (unit, flow_handle_info, flow_app_info, entry_handle_id, get_entry_handle_id, &gport_hw_resources,
                     terminator_info, special_fields, &last_qos_profile));

    SHR_IF_ERR_EXIT(dnx_flow_dl_ingress_trap_profile_replace
                    (unit, flow_handle_info, flow_app_info, entry_handle_id, get_entry_handle_id, &gport_hw_resources,
                     terminator_info, special_fields, &last_trap_profile));

    if (dnx_flow_app_is_ind_set(unit, flow_app_info, FLOW_APP_TERM_IND_SUPPORT_MPLS_PROFILE))
    {
        SHR_IF_ERR_EXIT(dnx_flow_dl_ingress_mpls_termination_profile_replace
                        (unit, flow_handle_info, flow_app_info, entry_handle_id, get_entry_handle_id, special_fields,
                         &last_mpls_term_profile));
    }

    /** If a result type selection callback is defined - call it */
    if (flow_app_info->result_type_select_cb)
    {
        SHR_IF_ERR_EXIT(flow_app_info->result_type_select_cb
                        (unit, entry_handle_id, get_entry_handle_id, flow_handle_info, &gport_hw_resources,
                         terminator_info, special_fields, &selectable_result_types));
    }

    /** If a specific application callback is defined - call it */
    if (flow_app_info->app_specific_operations_cb)
    {
        SHR_IF_ERR_EXIT(flow_app_info->app_specific_operations_cb
                        (unit, entry_handle_id, get_entry_handle_id, flow_handle_info, flow_cmd_control,
                         &gport_hw_resources, terminator_info, special_fields, &selectable_result_types));
        if (flow_cmd_control->flow_command != FLOW_COMMAND_CONTINUE)
        {
            SHR_EXIT();
        }
    }

    SHR_IF_ERR_EXIT(dnx_flow_lif_mgmt_terminator_info_replace
                    (unit, flow_handle_info, &gport_hw_resources, entry_handle_id, selectable_result_types));

    if (last_qos_profile != FLOW_PROFILE_INVALID)
    {
        SHR_IF_ERR_EXIT(dnx_qos_ingress_propagation_profile_hw_clear(unit, last_qos_profile));
    }

    if (last_trap_profile != FLOW_PROFILE_INVALID)
    {
        SHR_IF_ERR_EXIT(dnx_rx_trap_lif_ingress_action_profile_hw_clear(unit, last_trap_profile));
    }

    if (last_mpls_term_profile != FLOW_PROFILE_INVALID)
    {
        SHR_IF_ERR_EXIT(dnx_mpls_hw_termination_profile_delete(unit, last_mpls_term_profile));
    }

    /** if the app supports extended termination, check if it was set in the create and if so, update the dbal table */
    if (dnx_flow_app_is_ind_set(unit, flow_app_info, FLOW_APP_TERM_IND_SUPPORT_EXTENDED_TERM))
    {
        SHR_IF_ERR_EXIT(dnx_flow_extended_termination_replace
                        (unit, _SHR_GPORT_FLOW_LIF_VAL_GET_ID(flow_handle_info->flow_id)));
    }

exit:
    DNX_FLOW_LOGGER_END(unit, old_logger_data);
    DBAL_FUNC_FREE_VARS;
    SHR_FUNC_EXIT;
}

static shr_error_e
dnx_flow_terminator_dedicated_logic_destroy(
    int unit,
    dnx_flow_app_config_t * flow_app_info,
    bcm_flow_handle_info_t * flow_handle_info,
    uint32 entry_handle_id,
    dnx_algo_gpm_gport_hw_resources_t * gport_hw_resources)
{
    SHR_FUNC_INIT_VARS(unit);

    SHR_IF_ERR_EXIT(dnx_flow_dl_ingress_qos_profile_destroy
                    (unit, flow_handle_info, flow_app_info, entry_handle_id, gport_hw_resources));

    SHR_IF_ERR_EXIT(dnx_flow_dl_ingress_trap_profile_destroy
                    (unit, flow_app_info, flow_handle_info, entry_handle_id, gport_hw_resources));

    if (dnx_flow_app_is_ind_set(unit, flow_app_info, FLOW_APP_TERM_IND_SUPPORT_MPLS_PROFILE))
    {
        SHR_IF_ERR_EXIT(dnx_flow_dl_ingress_mpls_termination_profile_destroy
                        (unit, flow_app_info, flow_handle_info, entry_handle_id));
    }

exit:
    SHR_FUNC_EXIT;
}

static shr_error_e
dnx_flow_terminator_dedicated_logic_get(
    int unit,
    dnx_flow_app_config_t * flow_app_info,
    bcm_flow_handle_info_t * flow_handle_info,
    uint32 entry_handle_id,
    dnx_algo_gpm_gport_hw_resources_t * gport_hw_resources,
    bcm_flow_terminator_info_t * terminator_info,
    bcm_flow_special_fields_t * special_fields)
{
    SHR_FUNC_INIT_VARS(unit);

    SHR_IF_ERR_EXIT_EXCEPT_IF(dnx_flow_dl_ingress_qos_profile_get
                              (unit, flow_app_info, flow_handle_info, entry_handle_id, gport_hw_resources,
                               terminator_info, special_fields), _SHR_E_NOT_FOUND);

    SHR_IF_ERR_EXIT(dnx_flow_dl_ingress_trap_profile_get
                    (unit, entry_handle_id, gport_hw_resources, terminator_info, special_fields));

    if (dnx_flow_app_is_ind_set(unit, flow_app_info, FLOW_APP_TERM_IND_SUPPORT_MPLS_PROFILE))
    {
        SHR_IF_ERR_EXIT(dnx_flow_dl_ingress_mpls_termination_profile_get
                        (unit, flow_handle_info, entry_handle_id, special_fields));
    }

exit:
    SHR_FUNC_EXIT;
}

static shr_error_e
dnx_flow_terminator_dedicated_logic_create(
    int unit,
    dnx_flow_app_config_t * flow_app_info,
    bcm_flow_handle_info_t * flow_handle_info,
    uint32 entry_handle_id,
    bcm_flow_terminator_info_t * terminator_info,
    bcm_flow_special_fields_t * special_fields)
{
    SHR_FUNC_INIT_VARS(unit);

    SHR_IF_ERR_EXIT(dnx_flow_dl_ingress_qos_profile_create
                    (unit, entry_handle_id, flow_app_info, terminator_info, special_fields));

    SHR_IF_ERR_EXIT(dnx_flow_dl_ingress_trap_profile_create
                    (unit, entry_handle_id, flow_app_info, terminator_info, special_fields));

    if (dnx_flow_app_is_ind_set(unit, flow_app_info, FLOW_APP_TERM_IND_SUPPORT_MPLS_PROFILE))
    {
        SHR_IF_ERR_EXIT(dnx_flow_dl_ingress_mpls_termination_profile_create
                        (unit, entry_handle_id, flow_handle_info, special_fields));
    }

exit:
    SHR_FUNC_EXIT;
}

/*
 * Internal ingress flow lif create
 */
static shr_error_e
dnx_flow_terminator_info_create(
    int unit,
    bcm_flow_handle_info_t * flow_handle_info,
    dnx_flow_cmd_control_info_t * flow_cmd_control,
    bcm_flow_terminator_info_t * terminator_info,
    bcm_flow_special_fields_t * special_fields)
{
    int global_lif_id = 0;
    uint32 entry_handle_id;
    dnx_algo_gpm_gport_hw_resources_t gport_hw_resources = { 0 };
    dnx_flow_app_config_t *flow_app_info = dnx_flow_objects[flow_handle_info->flow_handle];
    uint32 selectable_result_types = 0;
    int is_virtual = (_SHR_IS_FLAG_SET(flow_handle_info->flags, BCM_FLOW_HANDLE_INFO_VIRTUAL)) ? TRUE : FALSE;
    dnx_flow_logger_data_t old_logger_data = { 0 };

    SHR_FUNC_INIT_VARS(unit);
    DBAL_FUNC_INIT_VARS(unit);

    SHR_IF_ERR_EXIT(DBAL_HANDLE_ALLOC(unit, FLOW_LIF_DBAL_TABLE_GET(flow_app_info), &entry_handle_id));

    /** Result type is superset, will be decided internally by lif table manager */
    dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_RESULT_TYPE, INST_SINGLE, DBAL_SUPERSET_RESULT_TYPE);

    SHR_IF_ERR_EXIT(dnx_flow_gport_flow_id_allocate(unit, flow_handle_info, flow_app_info, NULL, TRUE));

    DNX_FLOW_LOGGER_START(unit, flow_handle_info, flow_app_info, flow_cmd_control, terminator_info,
                          special_fields, logger_data);

    if (!is_virtual)
    {
        global_lif_id = _SHR_GPORT_FLOW_LIF_VAL_GET_ID(flow_handle_info->flow_id);
        dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_GLOB_IN_LIF, INST_SINGLE, global_lif_id);
    }

    SHR_IF_ERR_EXIT(dnx_flow_common_fields_set
                    (unit, flow_app_info, FLOW_APP_TYPE_TERM, flow_handle_info, terminator_info, &gport_hw_resources,
                     entry_handle_id));

    if (special_fields)
    {
        SHR_IF_ERR_EXIT(dnx_flow_special_fields_set(unit,
                                                    entry_handle_id,
                                                    flow_handle_info,
                                                    flow_app_info, &gport_hw_resources, special_fields));
    }

    /** Check if errors occurred on handle, using DBAL validation to make sure that the user values are correct. */
    SHR_IF_ERR_EXIT(dnx_flow_commit_error_check(unit, entry_handle_id));

    SHR_IF_ERR_EXIT(dnx_flow_terminator_dedicated_logic_create
                    (unit, flow_app_info, flow_handle_info, entry_handle_id, terminator_info, special_fields));

    /** If a result type selection callback is defined - call it */
    if (flow_app_info->result_type_select_cb)
    {
        SHR_IF_ERR_EXIT(flow_app_info->result_type_select_cb
                        (unit, entry_handle_id, -1, flow_handle_info, &gport_hw_resources, terminator_info,
                         special_fields, &selectable_result_types));
    }

    /*
     * If a specific application callback is defined - call it
     */
    if (flow_app_info->app_specific_operations_cb)
    {
        SHR_IF_ERR_EXIT(flow_app_info->app_specific_operations_cb
                        (unit, entry_handle_id, FLOW_ENTRY_HANDLE_ID_INVALID, flow_handle_info, flow_cmd_control,
                         &gport_hw_resources, terminator_info, special_fields, &selectable_result_types));
        if (flow_cmd_control->flow_command != FLOW_COMMAND_CONTINUE)
        {
            SHR_EXIT();
        }
    }

    /** Call lif table manager set */
    SHR_IF_ERR_EXIT(dnx_flow_lif_mgmt_terminator_info_create
                    (unit, flow_handle_info, entry_handle_id, selectable_result_types));

    /** update the reclassify table if required*/
    if (terminator_info->flags & BCM_FLOW_TERMINATOR_INFO_EXTENDED_TERMINATION)
    {
        SHR_IF_ERR_EXIT(dnx_flow_extended_termination_set(unit, global_lif_id));
    }

exit:
    DNX_FLOW_LOGGER_END(unit, old_logger_data);
    DBAL_FUNC_FREE_VARS;
    SHR_FUNC_EXIT;
}

/*
 * Internal ingress flow lif get
 */
static shr_error_e
dnx_flow_terminator_info_get(
    int unit,
    bcm_flow_handle_info_t * flow_handle_info,
    dnx_flow_cmd_control_info_t * flow_cmd_control,
    bcm_flow_terminator_info_t * terminator_info,
    bcm_flow_special_fields_t * special_fields)
{
    uint32 entry_handle_id = 0;
    dnx_algo_gpm_gport_hw_resources_t gport_hw_resources;
    dnx_flow_app_config_t *flow_app_info = dnx_flow_objects[flow_handle_info->flow_handle];

    SHR_FUNC_INIT_VARS(unit);
    DBAL_FUNC_INIT_VARS(unit);

    SHR_IF_ERR_EXIT(dnx_algo_gpm_gport_to_hw_resources(unit,
                                                       flow_handle_info->flow_id,
                                                       DNX_ALGO_GPM_GPORT_HW_RESOURCES_LOCAL_LIF_INGRESS,
                                                       &gport_hw_resources));

    /** Take handle to read from DBAL table */
    SHR_IF_ERR_EXIT(DBAL_HANDLE_ALLOC(unit, gport_hw_resources.inlif_dbal_table_id, &entry_handle_id));

    SHR_IF_ERR_EXIT(dnx_flow_lif_mgmt_terminator_info_get(unit,
                                                          flow_app_info,
                                                          flow_handle_info, &gport_hw_resources, entry_handle_id));

    SHR_IF_ERR_EXIT(dnx_flow_common_fields_get(unit,
                                               FLOW_APP_TYPE_TERM,
                                               flow_app_info,
                                               flow_handle_info,
                                               NULL, terminator_info, &gport_hw_resources, entry_handle_id));

    SHR_IF_ERR_EXIT(dnx_flow_special_fields_get(unit,
                                                entry_handle_id,
                                                flow_handle_info, flow_app_info, &gport_hw_resources, special_fields));

    SHR_IF_ERR_EXIT(dnx_flow_terminator_dedicated_logic_get
                    (unit, flow_app_info, flow_handle_info, entry_handle_id, &gport_hw_resources, terminator_info,
                     special_fields));

    /** If a specific application callback is defined - call it */
    if (flow_app_info->app_specific_operations_cb)
    {
        SHR_IF_ERR_EXIT(flow_app_info->app_specific_operations_cb
                        (unit, entry_handle_id, FLOW_ENTRY_HANDLE_ID_INVALID, flow_handle_info, flow_cmd_control,
                         &gport_hw_resources, terminator_info, special_fields, NULL));
        if (flow_cmd_control->flow_command != FLOW_COMMAND_CONTINUE)
        {
            SHR_EXIT();
        }
    }

    /** if app supports the extended termination, check if it was set */
    if (dnx_flow_app_is_ind_set(unit, flow_app_info, FLOW_APP_TERM_IND_SUPPORT_EXTENDED_TERM))
    {
        SHR_IF_ERR_EXIT(dnx_flow_extended_termination_get
                        (unit, _SHR_GPORT_FLOW_LIF_VAL_GET_ID(flow_handle_info->flow_id), &(terminator_info->flags)));
    }

    DNX_FLOW_LOGGER_PRINT(unit, flow_handle_info, flow_app_info, flow_cmd_control, terminator_info, special_fields);

exit:
    DBAL_FUNC_FREE_VARS;
    SHR_FUNC_EXIT;
}

/** designated LIF is the LIF that will returned to the user, it is needed since per user entry intranlly SDK adds entry
 *  per requested core. this function will returns is_designated true when the lif on the lowest core_id for the
 *  related user entry */
static shr_error_e
dnx_flow_terminator_is_lif_designated(
    int unit,
    int core_id,
    bcm_flow_handle_info_t * flow_handle_info,
    int global_lif,
    int virtual_id,
    int is_virtual_id,
    int *is_designated)
{
    lif_mapping_local_lif_payload_t local_lif_payload;
    int core_iter;

    SHR_FUNC_INIT_VARS(unit);

    (*is_designated) = FALSE;

    /** Retrieve a local In-LIFs array either for a Global In-LIF
     *  or a Virtual-Id */
    if (!is_virtual_id)
    {
        SHR_IF_ERR_EXIT(dnx_algo_lif_mapping_global_to_local_get
                        (unit, DNX_ALGO_LIF_INGRESS, global_lif, &local_lif_payload));
    }
    else
    {
        SHR_IF_ERR_EXIT(dnx_flow_ingress_virtual_local_lif_get_all
                        (unit, virtual_id, &local_lif_payload.local_lifs_array));

    }

    (*is_designated) = TRUE;
    /** run over all cores up to the core_id, and check if a lif exists if yes the lif is not designated since a lower
     *  lif exists */
    for (core_iter = 0; core_iter < core_id; core_iter++)
    {
        if (local_lif_payload.local_lifs_array.local_lifs[core_iter] != LIF_MNGR_INLIF_INVALID)
        {
            (*is_designated) = FALSE;
            break;
        }
    }

exit:
    SHR_FUNC_EXIT;
}

static shr_error_e
dnx_flow_terminator_info_traverse(
    int unit,
    bcm_flow_handle_info_t * flow_handle_info,
    bcm_flow_terminator_info_traverse_cb cb,
    void *user_data,
    dnx_flow_traverse_operation_e traverse_operation)
{
    uint32 entry_handle_id;
    int is_end;
    bcm_flow_terminator_info_t terminator_info = { 0 };
    dnx_flow_app_config_t *flow_app_info = dnx_flow_objects[flow_handle_info->flow_handle];
    bcm_flow_special_fields_t special_fields = { 0 };
    int global_lif;
    lif_mapping_local_lif_key_t local_lif_info;
    uint32 key_field;
    dbal_physical_tables_e physical_table_id;
    dnx_flow_cmd_control_info_t flow_cmd_control = { FLOW_CB_TYPE_GET, FLOW_COMMAND_CONTINUE };

    SHR_FUNC_INIT_VARS(unit);
    DBAL_FUNC_INIT_VARS(unit);
    DNX_ERR_RECOVERY_NOT_NEEDED(unit);

    /*
     * Get physical table id
     */
    SHR_IF_ERR_EXIT(dbal_tables_physical_table_get
                    (unit, FLOW_LIF_DBAL_TABLE_GET(flow_app_info), DBAL_PHY_DB_DEFAULT_INDEX, &physical_table_id));

    SHR_IF_ERR_EXIT(DBAL_HANDLE_ALLOC(unit, FLOW_LIF_DBAL_TABLE_GET(flow_app_info), &entry_handle_id));

    SHR_IF_ERR_EXIT(dbal_iterator_init(unit, entry_handle_id, DBAL_ITER_MODE_GET_ALL_EXCEPT_DEFAULT));

    if ((!_SHR_IS_FLAG_SET(flow_app_info->valid_flow_flags, BCM_FLOW_HANDLE_INFO_ALLOC_BY_CORE_BM))
        && (dnx_data_lif.in_lif.feature_get(unit, dnx_data_lif_in_lif_phy_db_dpc)))
    {
        uint32 core_id = DBAL_CORE_DEFAULT;
        /** in case that LIF tables are per core and the application not supporting per app allocation no need to run over
         *  all cores.  So Add KEY rule to skip duplicated entry  */
        SHR_IF_ERR_EXIT(dbal_iterator_key_field_arr32_rule_add(unit, entry_handle_id, DBAL_FIELD_CORE_ID,
                                                               DBAL_CONDITION_EQUAL_TO, &core_id, NULL));
    }

    /** Receive first entry in table. */
    SHR_IF_ERR_EXIT(dbal_iterator_get_next(unit, entry_handle_id, &is_end));
    while (!is_end)
    {
        shr_error_e rv;
        uint32 core_id;
        int virtual_id = 0, is_virtual;

        SHR_IF_ERR_EXIT(dbal_entry_handle_key_field_arr32_get(unit, entry_handle_id, DBAL_FIELD_IN_LIF, &key_field));
        sal_memset(&local_lif_info, 0, sizeof(local_lif_info));
        local_lif_info.local_lif = key_field;
        local_lif_info.phy_table = physical_table_id;

        if (_SHR_IS_FLAG_SET(flow_app_info->valid_flow_flags, BCM_FLOW_HANDLE_INFO_ALLOC_BY_CORE_BM) &&
            dnx_data_lif.in_lif.feature_get(unit, dnx_data_lif_in_lif_phy_db_dpc))
        {
            SHR_IF_ERR_EXIT(dbal_entry_handle_key_field_arr32_get(unit, entry_handle_id, DBAL_FIELD_CORE_ID, &core_id));
            local_lif_info.core_id = core_id;
        }
        else
        {
            core_id = 0;
        }

        rv = dnx_algo_lif_mapping_local_to_global_get(unit, DNX_ALGO_LIF_INGRESS, &local_lif_info, &global_lif);
        SHR_IF_ERR_EXIT_EXCEPT_IF(rv, _SHR_E_NOT_FOUND);
        if (rv == _SHR_E_NONE)
        {
            BCM_GPORT_SUB_TYPE_IN_LIF_SET(flow_handle_info->flow_id, global_lif);
            BCM_GPORT_FLOW_LIF_ID_SET(flow_handle_info->flow_id, flow_handle_info->flow_id);
            is_virtual = FALSE;
        }
        else
        {
            /** If no Global-LIF is found - it may be a virtual gport */
            rv = dnx_flow_ingress_virtual_gport_get(unit, key_field, core_id, &virtual_id);
            SHR_IF_ERR_EXIT_EXCEPT_IF(rv, _SHR_E_NOT_FOUND);
            BCM_GPORT_SUB_TYPE_VIRTUAL_INGRESS_SET(flow_handle_info->flow_id, virtual_id);
            BCM_GPORT_FLOW_LIF_ID_SET(flow_handle_info->flow_id, flow_handle_info->flow_id);
            is_virtual = TRUE;
        }

        if (rv == _SHR_E_NONE)
        {
            if (traverse_operation == FLOW_TRAVERSE_OPERATION_DELETE)
            {
                /** zero illegal flags that might fail the destroy */
                flow_handle_info->flags &= (~BCM_FLOW_HANDLE_NATIVE);
                /** Delete the entry */
                SHR_IF_ERR_EXIT(bcm_dnx_flow_terminator_info_destroy(unit, flow_handle_info));
            }
            else
            {
                int is_designated;

                if (_SHR_IS_FLAG_SET(flow_app_info->valid_flow_flags, BCM_FLOW_HANDLE_INFO_ALLOC_BY_CORE_BM) &&
                    dnx_data_lif.in_lif.feature_get(unit, dnx_data_lif_in_lif_phy_db_dpc))
                {
                    SHR_IF_ERR_EXIT(dnx_flow_terminator_is_lif_designated
                                    (unit, core_id, flow_handle_info, global_lif, virtual_id, is_virtual,
                                     &is_designated));
                }
                else
                {
                    /** in case that app is not supporting allocation by core, lif is always the designated */
                    is_designated = TRUE;
                }

                if (is_designated)
                {
                    SHR_IF_ERR_EXIT(dnx_flow_terminator_info_get
                                    (unit, flow_handle_info, &flow_cmd_control, &terminator_info, &special_fields));

                    /** Invoke callback function */
                    SHR_IF_ERR_EXIT((*cb) (unit, flow_handle_info, &terminator_info, &special_fields, user_data));
                }
            }
        }

        /** Receive next entry in table.*/
        SHR_IF_ERR_EXIT(dbal_iterator_get_next(unit, entry_handle_id, &is_end));
    }

exit:
    DBAL_FUNC_FREE_VARS;
    SHR_FUNC_EXIT;
}

/**********************************************************************************************
*                                  INITIATOR functions - START                                    *
**********************************************************************************************/

static shr_error_e
dnx_flow_initiator_dedicated_logic_destroy(
    int unit,
    dnx_flow_app_config_t * flow_app_info,
    bcm_flow_handle_info_t * flow_handle_info,
    uint32 entry_handle_id,
    dnx_algo_gpm_gport_hw_resources_t * gport_hw_resources)
{
    SHR_FUNC_INIT_VARS(unit);

    SHR_IF_ERR_EXIT(dnx_flow_dl_egress_src_addr_profile_destroy
                    (unit, entry_handle_id, flow_handle_info, flow_app_info, gport_hw_resources));

    SHR_IF_ERR_EXIT(dnx_flow_dl_egress_qos_profile_destroy
                    (unit, flow_handle_info, flow_app_info, entry_handle_id, gport_hw_resources));

    SHR_IF_ERR_EXIT(dnx_flow_dl_egress_trap_profile_destroy
                    (unit, flow_app_info, flow_handle_info, entry_handle_id, gport_hw_resources));

    SHR_IF_ERR_EXIT(dnx_flow_dl_outlif_profile_destroy(unit, entry_handle_id, flow_app_info, gport_hw_resources));

exit:
    SHR_FUNC_EXIT;
}

static shr_error_e
dnx_flow_initiator_dedicated_logic_get(
    int unit,
    dnx_flow_app_config_t * flow_app_info,
    bcm_flow_handle_info_t * flow_handle_info,
    uint32 entry_handle_id,
    dnx_algo_gpm_gport_hw_resources_t * gport_hw_resources,
    bcm_flow_initiator_info_t * initiator_info,
    bcm_flow_special_fields_t * special_fields)
{
    SHR_FUNC_INIT_VARS(unit);

    if (dnx_flow_app_is_ind_set(unit, flow_app_info, FLOW_APP_INIT_IND_SRC_ADDR_PROFILE))
    {
        SHR_IF_ERR_EXIT_EXCEPT_IF(dnx_flow_dl_egress_src_addr_profile_get
                                  (unit, entry_handle_id, flow_handle_info, flow_app_info, gport_hw_resources,
                                   special_fields), _SHR_E_NOT_FOUND);
    }

    SHR_IF_ERR_EXIT(dnx_flow_dl_egress_qos_profile_get
                    (unit, flow_app_info, entry_handle_id, initiator_info, special_fields));

    SHR_IF_ERR_EXIT(dnx_flow_dl_egress_trap_profile_get
                    (unit, entry_handle_id, gport_hw_resources, initiator_info, special_fields));

    SHR_IF_ERR_EXIT(dnx_flow_dl_outlif_profile_get
                    (unit, entry_handle_id, initiator_info, gport_hw_resources, special_fields));

exit:
    SHR_FUNC_EXIT;
}

static shr_error_e
dnx_flow_initiator_dedicated_logic_create(
    int unit,
    dnx_flow_app_config_t * flow_app_info,
    bcm_flow_handle_info_t * flow_handle_info,
    uint32 entry_handle_id,
    bcm_flow_initiator_info_t * initiator_info,
    bcm_flow_special_fields_t * special_fields)
{
    SHR_FUNC_INIT_VARS(unit);

    if (special_fields && dnx_flow_app_is_ind_set(unit, flow_app_info, FLOW_APP_INIT_IND_SRC_ADDR_PROFILE))
    {
        SHR_IF_ERR_EXIT(dnx_flow_dl_egress_src_addr_profile_create
                        (unit, entry_handle_id, flow_handle_info, flow_app_info, special_fields));
    }

    if (_SHR_IS_FLAG_SET(initiator_info->valid_elements_set, BCM_FLOW_INITIATOR_ELEMENT_QOS_EGRESS_MODEL_VALID))
    {
        /** update the QoS */
        SHR_IF_ERR_EXIT(dnx_flow_dl_egress_qos_profile_create
                        (unit, entry_handle_id, flow_app_info, initiator_info, special_fields));
    }

    SHR_IF_ERR_EXIT(dnx_flow_dl_egress_trap_profile_create
                    (unit, entry_handle_id, flow_app_info, initiator_info, special_fields));

    SHR_IF_ERR_EXIT(dnx_flow_dl_outlif_profile_create
                    (unit, entry_handle_id, flow_app_info, initiator_info, special_fields));

exit:
    SHR_FUNC_EXIT;
}

/*
 * set the alloc info structure in case of a concatenated Flow object
 */
static shr_error_e
dnx_flow_initiator_global_lif_alloc_info_set(
    int unit,
    dnx_flow_app_config_t * flow_app_info,
    bcm_flow_initiator_info_t * initiator_info,
    global_lif_alloc_info_t * alloc_info)
{
    SHR_FUNC_INIT_VARS(unit);

    /** if BTA is supported add the bta info to the allocation info */
    if (dnx_data_lif.global_lif.global_lif_bta_max_val_get(unit))
    {
        if (_SHR_IS_FLAG_SET(initiator_info->valid_elements_set, BCM_FLOW_INITIATOR_ELEMENT_L3_INTF_ID_VALID) &&
            (initiator_info->l3_intf_id != 0) && (initiator_info->l3_intf_id != BCM_IF_INVALID))
        {
            /** ipv4 or ipv6 over UDP is possible + GTP. Take the smaller between them */
            if (sal_strncmp(flow_app_info->app_name, "GTP_INITIATOR", sizeof(flow_app_info->app_name)) == 0)
            {
                alloc_info->bta = dnx_data_lif.out_lif.global_gtp_concat_ip_outlif_bta_sop_get(unit);
            }

            /** BIER */
            if (sal_strncmp(flow_app_info->app_name, "BIER_BASE_INITIATOR", sizeof(flow_app_info->app_name)) == 0)
            {
                alloc_info->bta = dnx_data_lif.out_lif.global_bier_outlif_bta_sop_get(unit);
            }

        }
    }

    /** until exposed to the user, set outlif_intf to disabled */
    alloc_info->outlif_intf = GLOBAL_LIF_INTF_DISABLED;

    /** if outlif group is supported add the outlif group info to the allocation info */
    if (dnx_data_lif.global_lif.global_lif_group_max_val_get(unit))
    {
        if (_SHR_IS_FLAG_SET(initiator_info->valid_elements_set, BCM_FLOW_INITIATOR_ELEMENT_OUTLIF_PROFILE_GROUP_VALID))
        {
            alloc_info->outlif_group = initiator_info->outlif_profile_group;
        }
    }

/** exit: */
    SHR_FUNC_EXIT;
}

/*
 * Internal egress flow lif create
 */
static shr_error_e
dnx_flow_initiator_info_create(
    int unit,
    bcm_flow_handle_info_t * flow_handle_info,
    dnx_flow_cmd_control_info_t * flow_cmd_control,
    bcm_flow_initiator_info_t * initiator_info,
    bcm_flow_special_fields_t * special_fields)
{
    uint32 entry_handle_id;
    dnx_flow_app_config_t *flow_app_info = dnx_flow_objects[flow_handle_info->flow_handle];
    dnx_algo_gpm_gport_hw_resources_t gport_hw_resources = { 0 };
    uint32 selectable_result_types = 0;
    dnx_algo_local_outlif_logical_phase_e logical_phase;
    bcm_encap_access_t encap_access;
    int is_local_lif;
    global_lif_alloc_info_t alloc_info = { 0 };
    dnx_flow_logger_data_t old_logger_data = { 0 };

    SHR_FUNC_INIT_VARS(unit);
    DBAL_FUNC_INIT_VARS(unit);
    DNX_ERR_RECOVERY_START(unit);

    is_local_lif = !(dnx_flow_app_is_ind_set(unit, flow_app_info, FLOW_APP_INIT_IND_NO_LOCAL_LIF));

    /** Take DBAL handle */
    SHR_IF_ERR_EXIT(DBAL_HANDLE_ALLOC(unit, FLOW_LIF_DBAL_TABLE_GET(flow_app_info), &entry_handle_id));

    /** Result type is superset, will be decided internally by lif table manager */
    dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_RESULT_TYPE, INST_SINGLE, DBAL_SUPERSET_RESULT_TYPE);

    /** Create global lif */
    SHR_IF_ERR_EXIT(dnx_flow_initiator_global_lif_alloc_info_set(unit, flow_app_info, initiator_info, &alloc_info));

    SHR_IF_ERR_EXIT(dnx_flow_gport_flow_id_allocate(unit, flow_handle_info, flow_app_info, &alloc_info, is_local_lif));

    DNX_FLOW_LOGGER_START(unit, flow_handle_info, flow_app_info, flow_cmd_control, initiator_info,
                          special_fields, logger_data);

    /*
     * Special treatment for encap access - mandatory for initiator, set on lif table manager outlif info
     * struct. Applicable only for EEDB entries, not for Virtual entries with no Local Out-LIF
     */
    if (is_local_lif)
    {
        DNX_FLOW_SET_ENCAP_ACCESS(encap_access, initiator_info->encap_access,
                                  flow_app_info->encap_access_default_mapping);
        SHR_IF_ERR_EXIT(dnx_lif_lib_encap_access_to_logical_phase_convert(unit, encap_access, &logical_phase));
    }

    /*
     * Call the set callbacks for the common and special fields
     */
    SHR_IF_ERR_EXIT(dnx_flow_common_fields_set
                    (unit, flow_app_info, FLOW_APP_TYPE_INIT, flow_handle_info, initiator_info, &gport_hw_resources,
                     entry_handle_id));

    if (special_fields)
    {
        SHR_IF_ERR_EXIT(dnx_flow_special_fields_set(unit,
                                                    entry_handle_id,
                                                    flow_handle_info,
                                                    flow_app_info, &gport_hw_resources, special_fields));
    }

    /** executing logic related to NATIVE flag */
    if (_SHR_IS_FLAG_SET(flow_app_info->valid_flow_flags, BCM_FLOW_HANDLE_NATIVE))
    {
        if (_SHR_IS_FLAG_SET(flow_handle_info->flags, BCM_FLOW_HANDLE_NATIVE))
        {
            dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_EGRESS_LAST_LAYER, INST_SINGLE, FALSE);
        }
        else
        {
            dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_EGRESS_LAST_LAYER, INST_SINGLE, TRUE);
        }
    }

    /** Check if errors occurred on handle, using DBAL validation to make sure that the user values are correct. */
    SHR_IF_ERR_EXIT(dnx_flow_commit_error_check(unit, entry_handle_id));

    SHR_IF_ERR_EXIT(dnx_flow_initiator_dedicated_logic_create
                    (unit, flow_app_info, flow_handle_info, entry_handle_id, initiator_info, special_fields));

    /** If a result type selection callback is defined - call it */
    if (flow_app_info->result_type_select_cb)
    {
        SHR_IF_ERR_EXIT(flow_app_info->result_type_select_cb
                        (unit, entry_handle_id, -1, flow_handle_info, &gport_hw_resources, initiator_info,
                         special_fields, &selectable_result_types));
    }

    /** If a specific application callback is defined - call it  */
    if (flow_app_info->app_specific_operations_cb)
    {
        SHR_IF_ERR_EXIT(flow_app_info->app_specific_operations_cb
                        (unit, entry_handle_id, FLOW_ENTRY_HANDLE_ID_INVALID, flow_handle_info, flow_cmd_control,
                         &gport_hw_resources, initiator_info, special_fields, &selectable_result_types));
        if (flow_cmd_control->flow_command != FLOW_COMMAND_CONTINUE)
        {
            SHR_EXIT();
        }
    }

    /** Write to HW */
    SHR_IF_ERR_EXIT(dnx_flow_lif_mgmt_initiator_info_create
                    (unit, flow_handle_info, entry_handle_id, logical_phase, selectable_result_types));

exit:
    DNX_FLOW_LOGGER_END(unit, old_logger_data);
    DNX_ERR_RECOVERY_END(unit);
    DBAL_FUNC_FREE_VARS;
    SHR_FUNC_EXIT;
}

shr_error_e
dnx_flow_initiator_encap_access_get(
    int unit,
    dnx_algo_gpm_gport_hw_resources_t * gport_hw_resources,
    bcm_flow_initiator_info_t * initiator_info,
    bcm_encap_access_t * encap_access)
{
    dnx_algo_local_outlif_logical_phase_e logical_phase;

    SHR_FUNC_INIT_VARS(unit);

    SHR_IF_ERR_EXIT(dnx_lif_mngr_outlif_sw_info_get(unit,
                                                    gport_hw_resources->local_out_lif,
                                                    NULL, NULL, &logical_phase, NULL, NULL, NULL));

    SHR_IF_ERR_EXIT(dnx_lif_lib_logical_phase_to_encap_access_convert(unit, logical_phase, encap_access));

exit:
    SHR_FUNC_EXIT;
}

/*
 * Internal replace egress flow lif
 */
static shr_error_e
dnx_flow_initiator_info_replace(
    int unit,
    bcm_flow_handle_info_t * flow_handle_info,
    dnx_flow_cmd_control_info_t * flow_cmd_control,
    bcm_flow_initiator_info_t * initiator_info,
    bcm_flow_special_fields_t * special_fields)
{
    dnx_algo_gpm_gport_hw_resources_t gport_hw_resources = { 0 };
    uint32 lif_get_flags;
    uint32 entry_handle_id, get_entry_handle_id = 0;
    dnx_flow_app_config_t *flow_app_info = dnx_flow_objects[flow_handle_info->flow_handle];
    uint32 selectable_result_types = 0;
    int is_virtual;
    int last_network_qos_profile = FLOW_PROFILE_INVALID,
        last_ttl_profile = FLOW_PROFILE_INVALID,
        last_src_addr_profile = FLOW_PROFILE_INVALID,
        last_outlif_profile = FLOW_PROFILE_INVALID, last_trap_profile = FLOW_PROFILE_INVALID;

    dnx_flow_logger_data_t old_logger_data = { 0 };

    SHR_FUNC_INIT_VARS(unit);
    DBAL_FUNC_INIT_VARS(unit);

    DNX_FLOW_LOGGER_START(unit, flow_handle_info, flow_app_info, flow_cmd_control, initiator_info,
                          special_fields, logger_data);

    is_virtual = FLOW_GPORT_IS_VIRTUAL(flow_handle_info->flow_id);

    lif_get_flags = (is_virtual) ? DNX_ALGO_GPM_GPORT_HW_RESOURCES_EGRESS_LOCAL_LIF_OR_VIRTUAL :
        (DNX_ALGO_GPM_GPORT_HW_RESOURCES_LOCAL_LIF_EGRESS | DNX_ALGO_GPM_GPORT_HW_RESOURCES_GLOBAL_LIF_EGRESS);
    SHR_IF_ERR_EXIT(dnx_algo_gpm_gport_to_hw_resources
                    (unit, flow_handle_info->flow_id, lif_get_flags, &gport_hw_resources));
    SHR_IF_ERR_EXIT(DBAL_HANDLE_ALLOC(unit, gport_hw_resources.outlif_dbal_table_id, &get_entry_handle_id));

    SHR_IF_ERR_EXIT(dnx_flow_lif_mgmt_initiator_info_get
                    (unit, flow_app_info, flow_handle_info, &gport_hw_resources, get_entry_handle_id));

    /** for replace calling verify just after get */
    if (flow_app_info->verify_cb)
    {
        dnx_flow_cmd_control_info_t verify_cmd_control = { FLOW_CB_TYPE_SET, FLOW_COMMAND_CONTINUE };

        SHR_IF_ERR_EXIT(flow_app_info->verify_cb
                        (unit, flow_handle_info, &verify_cmd_control, (void *) (initiator_info), special_fields,
                         get_entry_handle_id));
    }

    /** Take DBAL handle */
    SHR_IF_ERR_EXIT(DBAL_HANDLE_ALLOC(unit, gport_hw_resources.outlif_dbal_table_id, &entry_handle_id));

    /** Result type is superset, will be decided internally by lif table manager */
    dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_RESULT_TYPE, INST_SINGLE, DBAL_SUPERSET_RESULT_TYPE);

    SHR_IF_ERR_EXIT(dnx_flow_common_fields_replace
                    (unit, flow_app_info, FLOW_APP_TYPE_INIT, flow_handle_info, initiator_info, &gport_hw_resources,
                     entry_handle_id, get_entry_handle_id));

    SHR_IF_ERR_EXIT(dnx_flow_special_fields_replace
                    (unit, entry_handle_id, get_entry_handle_id, flow_handle_info,
                     flow_app_info, &gport_hw_resources, special_fields));

    /** Check if errors occurred on handle, using DBAL validation to make sure that the user values are correct. */
    SHR_IF_ERR_EXIT(dnx_flow_commit_error_check(unit, entry_handle_id));

    /** executing logic related to NATIVE flag */
    if (_SHR_IS_FLAG_SET(flow_app_info->valid_flow_flags, BCM_FLOW_HANDLE_NATIVE))
    {
        uint32 last_layer = 0;
        shr_error_e rv;
        rv = dbal_entry_handle_value_field32_get
            (unit, get_entry_handle_id, DBAL_FIELD_EGRESS_LAST_LAYER, INST_SINGLE, &last_layer);
        SHR_IF_ERR_EXIT_EXCEPT_IF(rv, _SHR_E_NOT_FOUND);

        if (rv != _SHR_E_NOT_FOUND)
        {
            dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_EGRESS_LAST_LAYER, INST_SINGLE, last_layer);
        }
    }

    if (special_fields && dnx_flow_app_is_ind_set(unit, flow_app_info, FLOW_APP_INIT_IND_SRC_ADDR_PROFILE))
    {
        SHR_IF_ERR_EXIT(dnx_flow_dl_egress_src_addr_profile_replace
                        (unit, entry_handle_id, get_entry_handle_id, flow_handle_info, flow_app_info,
                         &gport_hw_resources, special_fields, &last_src_addr_profile));
    }

    SHR_IF_ERR_EXIT(dnx_flow_dl_egress_trap_profile_replace
                    (unit, flow_handle_info, flow_app_info, entry_handle_id, get_entry_handle_id,
                     &gport_hw_resources, initiator_info, special_fields, &last_trap_profile));

    /** special treatment for qos egress fields */
    SHR_IF_ERR_EXIT(dnx_flow_dl_egress_qos_profile_replace
                    (unit, flow_handle_info, flow_app_info, entry_handle_id, get_entry_handle_id, &gport_hw_resources,
                     initiator_info, special_fields, &last_network_qos_profile, &last_ttl_profile));

    SHR_IF_ERR_EXIT(dnx_flow_dl_outlif_profile_replace
                    (unit, entry_handle_id, get_entry_handle_id, flow_app_info, initiator_info, &gport_hw_resources,
                     special_fields, &last_outlif_profile));

    /*
     * If a result type selection callback is defined - call it
     */
    if (flow_app_info->result_type_select_cb)
    {
        SHR_IF_ERR_EXIT(flow_app_info->result_type_select_cb
                        (unit, entry_handle_id, get_entry_handle_id, flow_handle_info, &gport_hw_resources,
                         initiator_info, special_fields, &selectable_result_types));
    }
    /*
     * If a specific application callback is defined - call it 
     */
    if (flow_app_info->app_specific_operations_cb)
    {
        SHR_IF_ERR_EXIT(flow_app_info->app_specific_operations_cb
                        (unit, entry_handle_id, get_entry_handle_id, flow_handle_info, flow_cmd_control,
                         &gport_hw_resources, initiator_info, special_fields, &selectable_result_types));
        if (flow_cmd_control->flow_command != FLOW_COMMAND_CONTINUE)
        {
            SHR_EXIT();
        }
    }

    /** Write to HW */
    SHR_IF_ERR_EXIT(dnx_flow_lif_mgmt_initiator_info_replace
                    (unit, flow_handle_info, &gport_hw_resources, entry_handle_id, selectable_result_types));

    /** Clearing the HW for needed profiles */
    if (last_network_qos_profile != FLOW_PROFILE_INVALID)
    {
        SHR_IF_ERR_EXIT(dnx_qos_egress_network_qos_pipe_profile_hw_clear(unit, last_network_qos_profile));
    }

    if (last_ttl_profile != FLOW_PROFILE_INVALID)
    {
        SHR_IF_ERR_EXIT(dnx_qos_egress_ttl_profile_hw_clear(unit, last_ttl_profile));
    }

    if (last_src_addr_profile != FLOW_PROFILE_INVALID)
    {
        SHR_IF_ERR_EXIT(dnx_flow_dl_egress_src_addr_profile_hw_clear(unit, flow_app_info, last_src_addr_profile));
    }

    if (last_trap_profile != FLOW_PROFILE_INVALID)
    {
        SHR_IF_ERR_EXIT(dnx_rx_trap_lif_etpp_trap_action_profile_hw_clear(unit, last_trap_profile));
    }

    
    /**
    if (last_outlif_profile != FLOW_PROFILE_INVALID)
    {
        
    } 
    */

exit:
    DNX_FLOW_LOGGER_END(unit, old_logger_data);
    DBAL_FUNC_FREE_VARS;
    SHR_FUNC_EXIT;
}

/*
 * Internal egress flow lif get
 */
static shr_error_e
dnx_flow_initiator_info_get(
    int unit,
    bcm_flow_handle_info_t * flow_handle_info,
    dnx_flow_cmd_control_info_t * flow_cmd_control,
    bcm_flow_initiator_info_t * initiator_info,
    bcm_flow_special_fields_t * special_fields)
{
    uint32 entry_handle_id = 0;
    uint32 lif_flags;
    dnx_algo_gpm_gport_hw_resources_t gport_hw_resources;
    dnx_flow_app_config_t *flow_app_info = dnx_flow_objects[flow_handle_info->flow_handle];
    bcm_encap_access_t encap_access;
    int is_virtual, is_local_lif;

    SHR_FUNC_INIT_VARS(unit);
    DBAL_FUNC_INIT_VARS(unit);

    is_local_lif = !(dnx_flow_app_is_ind_set(unit, flow_app_info, FLOW_APP_INIT_IND_NO_LOCAL_LIF));
    is_virtual = FLOW_GPORT_IS_VIRTUAL(flow_handle_info->flow_id);

    lif_flags = (is_virtual) ? DNX_ALGO_GPM_GPORT_HW_RESOURCES_EGRESS_LOCAL_LIF_OR_VIRTUAL :
        (DNX_ALGO_GPM_GPORT_HW_RESOURCES_LOCAL_LIF_EGRESS | DNX_ALGO_GPM_GPORT_HW_RESOURCES_GLOBAL_LIF_EGRESS);
    SHR_IF_ERR_EXIT(dnx_algo_gpm_gport_to_hw_resources(unit,
                                                       flow_handle_info->flow_id, lif_flags, &gport_hw_resources));

    SHR_IF_ERR_EXIT(DBAL_HANDLE_ALLOC(unit, gport_hw_resources.outlif_dbal_table_id, &entry_handle_id));

    SHR_IF_ERR_EXIT(dnx_flow_lif_mgmt_initiator_info_get(unit,
                                                         flow_app_info,
                                                         flow_handle_info, &gport_hw_resources, entry_handle_id));

    /*
     * Encap access get. In case a default application value exists and it is similar to what was set - return encap
     * access invalid. Otherwise - return the encap access set
     */
    if (is_local_lif)
    {
        SHR_IF_ERR_EXIT(dnx_flow_initiator_encap_access_get(unit, &gport_hw_resources, initiator_info, &encap_access));
        initiator_info->encap_access =
            (flow_app_info->encap_access_default_mapping == encap_access) ? bcmEncapAccessInvalid : encap_access;
    }

    SHR_IF_ERR_EXIT(dnx_flow_common_fields_get(unit,
                                               FLOW_APP_TYPE_INIT,
                                               flow_app_info,
                                               flow_handle_info,
                                               initiator_info, NULL, &gport_hw_resources, entry_handle_id));

    SHR_IF_ERR_EXIT(dnx_flow_special_fields_get(unit, entry_handle_id, flow_handle_info, flow_app_info,
                                                &gport_hw_resources, special_fields));

    if (_SHR_IS_FLAG_SET(flow_app_info->valid_flow_flags, BCM_FLOW_HANDLE_NATIVE))
    {
        uint32 last_layer = 0;
        shr_error_e rv;
        rv = dbal_entry_handle_value_field32_get
            (unit, entry_handle_id, DBAL_FIELD_EGRESS_LAST_LAYER, INST_SINGLE, &last_layer);
        SHR_IF_ERR_EXIT_EXCEPT_IF(rv, _SHR_E_NOT_FOUND);
        if ((rv != _SHR_E_NOT_FOUND) && !last_layer)
        {
            flow_handle_info->flags |= BCM_FLOW_HANDLE_NATIVE;
        }
    }

    SHR_IF_ERR_EXIT(dnx_flow_initiator_dedicated_logic_get
                    (unit, flow_app_info, flow_handle_info, entry_handle_id, &gport_hw_resources, initiator_info,
                     special_fields));

    /** If a specific application callback is defined - call it */
    if (flow_app_info->app_specific_operations_cb)
    {
        SHR_IF_ERR_EXIT(flow_app_info->app_specific_operations_cb
                        (unit, entry_handle_id, FLOW_ENTRY_HANDLE_ID_INVALID, flow_handle_info, flow_cmd_control,
                         &gport_hw_resources, initiator_info, special_fields, NULL));
        if (flow_cmd_control->flow_command != FLOW_COMMAND_CONTINUE)
        {
            SHR_EXIT();
        }
    }

    DNX_FLOW_LOGGER_PRINT(unit, flow_handle_info, flow_app_info, flow_cmd_control, initiator_info, special_fields);

exit:
    DBAL_FUNC_FREE_VARS;
    SHR_FUNC_EXIT;
}

static shr_error_e
dnx_flow_initiator_info_traverse_no_local_lif(
    int unit,
    bcm_flow_handle_info_t * flow_handle_info,
    bcm_flow_initiator_info_traverse_cb cb,
    void *user_data,
    dnx_flow_traverse_operation_e traverse_operation)
{
    uint32 entry_handle_id = -1;
    int is_end;
    bcm_flow_initiator_info_t initiator_info;
    bcm_flow_special_fields_t special_fields;
    dnx_flow_app_config_t *flow_app_info = dnx_flow_objects[flow_handle_info->flow_handle];
    dnx_flow_cmd_control_info_t flow_cmd_control = { FLOW_CB_TYPE_GET, FLOW_COMMAND_CONTINUE };
    uint32 dbal_table = FLOW_LIF_DBAL_TABLE_GET(flow_app_info);

    SHR_FUNC_INIT_VARS(unit);
    DBAL_FUNC_INIT_VARS(unit);

    SHR_IF_ERR_EXIT(dbal_iterator_init(unit, entry_handle_id, DBAL_ITER_MODE_GET_ALL_EXCEPT_DEFAULT));

    /** Set the iterator rules for entries that related only to the relevant dbal tables  */
    SHR_IF_ERR_EXIT(dbal_iterator_value_field32_rule_add(unit, entry_handle_id, DBAL_FIELD_TABLE_ID, INST_SINGLE,
                                                         DBAL_CONDITION_EQUAL_TO, dbal_table, 0));

    /** Receive first entry in table. */
    SHR_IF_ERR_EXIT(dbal_iterator_get_next(unit, entry_handle_id, &is_end));
    while (!is_end)
    {
        uint32 ret_field_val[DBAL_FIELD_ARRAY_MAX_SIZE_IN_WORDS] = { 0 };

        SHR_IF_ERR_EXIT(dbal_entry_handle_key_field_arr32_get(unit, entry_handle_id, DBAL_FIELD_GPORT, ret_field_val));

        flow_handle_info->flow_id = ret_field_val[0];

        if (traverse_operation == FLOW_TRAVERSE_OPERATION_DELETE)
        {
            /** zero illegal flags that might fail the destroy */
            flow_handle_info->flags &= (~BCM_FLOW_HANDLE_NATIVE);
            /** Delete the entry */
            SHR_IF_ERR_EXIT(bcm_dnx_flow_initiator_info_destroy(unit, flow_handle_info));
        }
        else
        {
            SHR_IF_ERR_EXIT(dnx_flow_initiator_info_get
                            (unit, flow_handle_info, &flow_cmd_control, &initiator_info, &special_fields));
            /** Invoke callback function */
            SHR_IF_ERR_EXIT((*cb) (unit, flow_handle_info, &initiator_info, &special_fields, user_data));
        }

        /** Receive next entry in table.*/
        SHR_IF_ERR_EXIT(dbal_iterator_get_next(unit, entry_handle_id, &is_end));
    }

exit:
    DBAL_FUNC_FREE_VARS;
    SHR_FUNC_EXIT;
}

static shr_error_e
dnx_flow_initiator_info_traverse(
    int unit,
    bcm_flow_handle_info_t * flow_handle_info,
    bcm_flow_initiator_info_traverse_cb cb,
    void *user_data,
    dnx_flow_traverse_operation_e traverse_operation)
{
    uint32 entry_handle_id;
    int is_end;
    uint32 key_field;
    bcm_flow_initiator_info_t initiator_info;
    bcm_flow_special_fields_t special_fields;
    dnx_flow_app_config_t *flow_app_info;
    dnx_flow_cmd_control_info_t flow_cmd_control = { FLOW_CB_TYPE_GET, FLOW_COMMAND_CONTINUE };
    dbal_table_type_e table_type;
    dbal_physical_tables_e physical_table_id = DBAL_PHYSICAL_TABLE_NONE;
    dbal_core_mode_e core_mode = DBAL_CORE_NONE;
    uint32 core_id = DBAL_CORE_DEFAULT;
    bcm_gport_t gport_lif;

    SHR_FUNC_INIT_VARS(unit);
    DBAL_FUNC_INIT_VARS(unit);

    flow_app_info = dnx_flow_objects[flow_handle_info->flow_handle];

    if (dnx_flow_app_is_ind_set(unit, flow_app_info, FLOW_APP_INIT_IND_NO_LOCAL_LIF))
    {
        SHR_IF_ERR_EXIT(dnx_flow_initiator_info_traverse_no_local_lif
                        (unit, flow_handle_info, cb, user_data, traverse_operation));
        SHR_EXIT();
    }

    SHR_IF_ERR_EXIT(DBAL_HANDLE_ALLOC(unit, FLOW_LIF_DBAL_TABLE_GET(flow_app_info), &entry_handle_id));

    SHR_IF_ERR_EXIT(dbal_iterator_init(unit, entry_handle_id, DBAL_ITER_MODE_GET_ALL_EXCEPT_DEFAULT));

    SHR_IF_ERR_EXIT(dbal_tables_table_type_get(unit, FLOW_LIF_DBAL_TABLE_GET(flow_app_info), &table_type));
    if (table_type != DBAL_TABLE_TYPE_DIRECT)
    {
        SHR_IF_ERR_EXIT(dbal_tables_physical_table_get
                        (unit, FLOW_LIF_DBAL_TABLE_GET(flow_app_info), DBAL_PHY_DB_DEFAULT_INDEX, &physical_table_id));
        core_mode = dbal_mdb_phy_table_core_mode_get(unit, physical_table_id);
    }

    if (core_mode == DBAL_CORE_MODE_DPC)
    {
        /** Add KEY rule to skip duplicated entry */
        SHR_IF_ERR_EXIT(dbal_iterator_key_field_arr32_rule_add(unit, entry_handle_id, DBAL_FIELD_CORE_ID,
                                                               DBAL_CONDITION_EQUAL_TO, &core_id, NULL));
    }

    /** Receive first entry in table. */
    SHR_IF_ERR_EXIT(dbal_iterator_get_next(unit, entry_handle_id, &is_end));
    while (!is_end)
    {
        gport_lif = BCM_GPORT_INVALID;
        SHR_IF_ERR_EXIT(dbal_entry_handle_key_field_arr32_get(unit, entry_handle_id, DBAL_FIELD_OUT_LIF, &key_field));
        /** In case local lif is not found, return next pointer 0 */
        SHR_IF_ERR_EXIT(dnx_algo_gpm_gport_from_lif(unit,
                                                    DNX_ALGO_GPM_GPORT_HW_RESOURCES_LOCAL_LIF_EGRESS |
                                                    DNX_ALGO_GPM_GPORT_HW_RESOURCES_NON_STRICT, _SHR_CORE_ALL,
                                                    key_field, &gport_lif));
        if (gport_lif != BCM_GPORT_INVALID)
        {
            if (!FLOW_GPORT_IS_VIRTUAL(gport_lif))
            {
                dnx_algo_gpm_gport_hw_resources_t gport_hw_resources;
                SHR_IF_ERR_EXIT(dnx_algo_gpm_gport_to_hw_resources
                                (unit, gport_lif, DNX_ALGO_GPM_GPORT_HW_RESOURCES_GLOBAL_LIF_EGRESS,
                                 &gport_hw_resources));
                BCM_GPORT_SUB_TYPE_OUT_LIF_SET(flow_handle_info->flow_id, gport_hw_resources.global_out_lif);
                BCM_GPORT_FLOW_LIF_ID_SET(flow_handle_info->flow_id, flow_handle_info->flow_id);
            }
            else
            {
                BCM_GPORT_FLOW_LIF_ID_SET(flow_handle_info->flow_id, gport_lif);
            }

            if (traverse_operation == FLOW_TRAVERSE_OPERATION_DELETE)
            {
                /** zero illegal flags that might fail the destroy */
                flow_handle_info->flags &= (~BCM_FLOW_HANDLE_NATIVE);
                /** Delete the entry */
                SHR_IF_ERR_EXIT(bcm_dnx_flow_initiator_info_destroy(unit, flow_handle_info));
            }
            else
            {
                SHR_IF_ERR_EXIT(dnx_flow_initiator_info_get
                                (unit, flow_handle_info, &flow_cmd_control, &initiator_info, &special_fields));
                /** Invoke callback function */
                SHR_IF_ERR_EXIT((*cb) (unit, flow_handle_info, &initiator_info, &special_fields, user_data));
            }
        }

        /** Receive next entry in table.*/
        SHR_IF_ERR_EXIT(dbal_iterator_get_next(unit, entry_handle_id, &is_end));
    }

exit:
    DBAL_FUNC_FREE_VARS;
    SHR_FUNC_EXIT;
}

/**********************************************************************************************
*                                  MATCH functions - START                                    *
**********************************************************************************************/
static shr_error_e
dnx_flow_match_key_fields_get(
    int unit,
    uint32 entry_handle_id,
    bcm_flow_handle_info_t * flow_handle_info,
    dnx_flow_app_config_t * flow_app_info,
    dbal_table_type_e table_type,
    bcm_flow_special_fields_t * key_special_fields,
    int nof_app_special_fields)
{
    int key_idx, nof_reduced_fields = 0;
    flow_special_field_info_t *curr_field;

    SHR_FUNC_INIT_VARS(unit);

/** Get each key, if the key has mask keeping the mask value and the field ID. at the end appending them to the array
 *  of special fields returning to the user */
    for (key_idx = 0; key_idx < nof_app_special_fields; key_idx++)
    {
        curr_field = &flow_special_fields_db[key_special_fields->special_fields[key_idx].field_id];

        if ((table_type == DBAL_TABLE_TYPE_TCAM)
            && dnx_flow_special_field_is_ind_set(unit, curr_field, FLOW_SPECIAL_FIELD_IND_MASK_SUPPORTED))
        {
            SHR_IF_ERR_EXIT(flow_special_fields_mask_field_value_get
                            (unit, entry_handle_id, curr_field, key_special_fields, key_idx));
        }
        else if (dnx_flow_special_field_is_ind_set(unit, curr_field, FLOW_SPECIAL_FIELD_IND_IS_MASK) ||
                 dnx_flow_special_field_is_ind_set(unit, curr_field, FLOW_SPECIAL_FIELD_IND_MATCH_IGNORE_OPERATION))
        {
        /** do nothing */
            continue;
        }
        else if (curr_field->get != NULL)
        {
            int rv;

            rv = curr_field->get
                (unit, entry_handle_id, flow_handle_info, flow_app_info, NULL,
                 key_special_fields->special_fields[key_idx].field_id, &(key_special_fields->special_fields[key_idx]));
            SHR_IF_ERR_EXIT_EXCEPT_IF(rv, _SHR_E_UNAVAIL);
            if (rv == _SHR_E_UNAVAIL)
            {
                continue;
            }
        }
        else
        {
            int rv;

            rv = flow_special_field_key_direct_mapping_get
                (unit, curr_field, entry_handle_id, table_type, &(key_special_fields->special_fields[key_idx]));
            SHR_IF_ERR_EXIT_EXCEPT_IF(rv, _SHR_E_UNAVAIL);
            if (rv == _SHR_E_UNAVAIL)
            {
                key_special_fields->special_fields[key_idx].field_id = 0;
                nof_reduced_fields++;
            }
        }
    }

    /** in case fields were reduced, need to shrink the array to return all the fields conscutive (without holes) */
    if (nof_reduced_fields)
    {
        int iter;
        for (key_idx = 0; key_idx < nof_app_special_fields - nof_reduced_fields; key_idx++)
        {
            /** we found a hole (field that is not set), lets replace with valid field */
            if (key_special_fields->special_fields[key_idx].field_id == 0)
            {
                for (iter = nof_app_special_fields - nof_reduced_fields; iter < nof_app_special_fields; iter++)
                {
                    if (key_special_fields->special_fields[iter].field_id != 0)
                    {
                        sal_memcpy(&key_special_fields->special_fields[key_idx],
                                   &key_special_fields->special_fields[iter], sizeof(bcm_flow_special_field_t));
                        sal_memset(&key_special_fields->special_fields[iter], 0, sizeof(bcm_flow_special_field_t));
                        break;
                    }
                }

                if (iter == nof_app_special_fields)
                {
                    SHR_ERR_EXIT(_SHR_E_INTERNAL, "No field found to replace the hole pos %d\n", key_idx);
                }
            }
        }
        key_special_fields->actual_nof_special_fields -= nof_reduced_fields;
    }

exit:
    SHR_FUNC_EXIT;
}

shr_error_e
dnx_flow_match_entry_from_global_lif_get(
    int unit,
    int global_lif,
    bcm_flow_handle_info_t * flow_handle_info,
    bcm_flow_special_fields_t * key_special_fields)
{
    uint32 entry_handle_id;
    int is_end, ii = 0;
    lif_mapping_local_lif_payload_t local_lif_payload;
    dnx_flow_app_config_t *flow_app_info;
    dbal_tables_e dbal_table;
    int max_nof_tables = FLOW_MAX_NOF_MATCH_DBAL_TABLES, bud_table_used = 0;

    SHR_FUNC_INIT_VARS(unit);
    DBAL_FUNC_INIT_VARS(unit);

    DNX_FLOW_HANDLE_VALIDATE(flow_handle_info->flow_handle);

    flow_app_info = dnx_flow_objects[flow_handle_info->flow_handle];
    dbal_table = flow_app_info->flow_table[0];

    if (dnx_flow_app_is_ind_set(unit, flow_app_info, FLOW_APP_TERM_MATCH_IND_MULTIPLE_DBAL_TABLES))
    {
        /** not supported for multiple tables yet since the relevant dbal table is not known */
        SHR_ERR_EXIT(_SHR_E_UNAVAIL, "API not supported for MULTIPLE_DBAL_TABLES applications");
    }

    if (_SHR_IS_FLAG_SET(flow_app_info->valid_flow_flags,
                         BCM_FLOW_HANDLE_INFO_BUD) && !dnx_flow_app_is_ind_set(unit,
                                                                               flow_app_info,
                                                                               FLOW_APP_TERM_MATCH_IND_BUD_BY_KEY_FIELD))
    {
        max_nof_tables++;
    }

    while ((dbal_table != DBAL_TABLE_EMPTY) && (ii < max_nof_tables))
    {
        SHR_IF_ERR_EXIT(DBAL_HANDLE_ALLOC(unit, dbal_table, &entry_handle_id));

        SHR_IF_ERR_EXIT(dbal_iterator_init(unit, entry_handle_id, DBAL_ITER_MODE_GET_ALL_EXCEPT_DEFAULT));

        if (flow_app_info->flow_app_type == FLOW_APP_TYPE_TERM_MATCH)
        {
            SHR_IF_ERR_EXIT(dnx_algo_lif_mapping_global_to_local_get
                            (unit, DNX_ALGO_LIF_INGRESS, global_lif, &local_lif_payload));

            if (dbal_table != DBAL_TABLE_INGRESS_PP_PORT)
            {
                SHR_IF_ERR_EXIT(dbal_iterator_value_field32_rule_add
                                (unit, entry_handle_id, DBAL_FIELD_IN_LIF, INST_SINGLE, DBAL_CONDITION_EQUAL_TO,
                                 local_lif_payload.local_lifs_array.local_lifs[0], 0));
            }
            else
            {
                SHR_IF_ERR_EXIT(dbal_iterator_value_field32_rule_add
                                (unit, entry_handle_id, DBAL_FIELD_DEFAULT_LIF, INST_SINGLE, DBAL_CONDITION_EQUAL_TO,
                                 local_lif_payload.local_lifs_array.local_lifs[0], 0));
            }
        }
        else if (flow_app_info->flow_app_type == FLOW_APP_TYPE_INIT_MATCH)
        {
            SHR_IF_ERR_EXIT(dnx_algo_lif_mapping_global_to_local_get
                            (unit, DNX_ALGO_LIF_EGRESS, global_lif, &local_lif_payload));

            SHR_IF_ERR_EXIT(dbal_iterator_value_field32_rule_add(unit, entry_handle_id, DBAL_FIELD_OUT_LIF, INST_SINGLE,
                                                                 DBAL_CONDITION_EQUAL_TO,
                                                                 local_lif_payload.local_lifs_array.local_lifs[0], 0));
        }
        else
        {
            SHR_ERR_EXIT(_SHR_E_PARAM, "Illegal flow handle (%s), only match applications supported ",
                         flow_app_info->app_name);
        }

        /** Receive first entry in table. */
        SHR_IF_ERR_EXIT(dbal_iterator_get_next(unit, entry_handle_id, &is_end));
        if (is_end)
        {
            /** no matching entry found */
            SHR_SET_CURRENT_ERR(_SHR_E_NOT_FOUND);
            SHR_EXIT();
        }
        else
        {
            int nof_app_special_fields = 0;
            dbal_table_type_e table_type;

            if (flow_app_info->is_entry_related_cb &&
                dnx_flow_app_is_ind_set(unit, flow_app_info, FLOW_APP_TERM_MATCH_IND_MANY_TO_ONE))
            {
                int is_entry_related;

                SHR_IF_ERR_EXIT(flow_app_info->is_entry_related_cb(unit, entry_handle_id, &is_entry_related));
                if (!is_entry_related)
                {
                    /** no matching entry found */
                    continue;
                }
            }

            SHR_IF_ERR_EXIT(dbal_tables_table_type_get(unit, dbal_table, &table_type));

            /** Get number of key special fields, serves as key for match applications Assign each key field id */
            while (flow_app_info->special_fields[nof_app_special_fields] != FLOW_S_F_EMPTY)
            {
                key_special_fields->special_fields[nof_app_special_fields].field_id =
                    flow_app_info->special_fields[nof_app_special_fields];
                nof_app_special_fields++;
            }

            key_special_fields->actual_nof_special_fields = nof_app_special_fields;

            SHR_IF_ERR_EXIT(dnx_flow_match_key_fields_get
                            (unit, entry_handle_id, flow_handle_info, flow_app_info, table_type, key_special_fields,
                             nof_app_special_fields));

            if (flow_app_info->dbal_table_to_key_field_map_cb)
            {
                /** used in traverse to map the dbal table to the key field. */
                SHR_IF_ERR_EXIT(flow_app_info->dbal_table_to_key_field_map_cb
                                (unit, entry_handle_id, flow_handle_info, FLOW_MAP_TYPE_TABLE_TO_KEY_FIELD,
                                 key_special_fields, &dbal_table));
            }

            /** If a specific application callback is defined - call it which traverse cmd */
            if (flow_app_info->app_specific_operations_cb)
            {
                dnx_flow_cmd_control_info_t flow_cmd_control = { FLOW_CB_TYPE_TRAVERSE, FLOW_COMMAND_CONTINUE };

                SHR_IF_ERR_EXIT(flow_app_info->app_specific_operations_cb
                                (unit, entry_handle_id, FLOW_ENTRY_HANDLE_ID_INVALID, flow_handle_info,
                                 &flow_cmd_control, NULL, NULL, key_special_fields, NULL));
            }

        }
        if (bud_table_used && !dnx_flow_app_is_ind_set(unit, flow_app_info, FLOW_APP_TERM_MATCH_IND_BUD_BY_KEY_FIELD))
        {
            dbal_table = DBAL_TABLE_EMPTY;
        }
        else if (_SHR_IS_FLAG_SET(flow_app_info->valid_flow_flags, BCM_FLOW_HANDLE_INFO_BUD)
                 && (ii == FLOW_MAX_NOF_MATCH_DBAL_TABLES - 1))
        {
            if (dnx_flow_app_is_ind_set(unit, flow_app_info, FLOW_APP_TERM_MATCH_IND_BUD_BY_KEY_FIELD))
            {
                dbal_table = (dbal_tables_e) flow_app_info->second_pass_param;
                bud_table_used = 1;
            }
            else
            {
                dbal_table = DBAL_TABLE_EMPTY;
            }
        }
        else
        {
            dbal_table = flow_app_info->flow_table[++ii];
            if (_SHR_IS_FLAG_SET(flow_app_info->valid_flow_flags, BCM_FLOW_HANDLE_INFO_BUD) &&
                !dnx_flow_app_is_ind_set(unit, flow_app_info, FLOW_APP_TERM_MATCH_IND_BUD_BY_KEY_FIELD)
                && (dbal_table == DBAL_TABLE_EMPTY))
            {
                dbal_table = (dbal_tables_e) flow_app_info->second_pass_param;
                bud_table_used = 1;
            }
        }
    }

exit:
    DBAL_FUNC_FREE_VARS;
    SHR_FUNC_EXIT;
}

static shr_error_e
dnx_flow_match_related_dbal_table_get(
    int unit,
    bcm_flow_handle_info_t * flow_handle_info,
    dnx_flow_app_config_t * flow_app_info,
    bcm_flow_special_fields_t * key_special_fields,
    dbal_tables_e * selected_dbal_table)
{
    SHR_FUNC_INIT_VARS(unit);

    if (dnx_flow_app_is_ind_set(unit, flow_app_info, FLOW_APP_TERM_MATCH_IND_MULTIPLE_DBAL_TABLES))
    {
        if (flow_app_info->dbal_table_to_key_field_map_cb)
        {
            SHR_IF_ERR_EXIT(flow_app_info->dbal_table_to_key_field_map_cb
                            (unit, 0, flow_handle_info, FLOW_MAP_TYPE_KEY_FIELD_TO_TABLE,
                             key_special_fields, selected_dbal_table));
        }
        else
        {
            SHR_ERR_EXIT(_SHR_E_INTERNAL, "MATCH with MULTIPLE_DBAL_TABLES with no cb function");
        }
    }
    else
    {
        (*selected_dbal_table) = flow_app_info->flow_table[0];
    }

    if (_SHR_IS_FLAG_SET(flow_handle_info->flags, BCM_FLOW_HANDLE_INFO_BUD) &&
        (!dnx_flow_app_is_ind_set(unit, flow_app_info, FLOW_APP_TERM_MATCH_IND_BUD_BY_KEY_FIELD)))
    {
        (*selected_dbal_table) = (dbal_tables_e) flow_app_info->second_pass_param;
    }

exit:
    SHR_FUNC_EXIT;
}
/** \brief
 *  Function set the key fields for entry according to the key_special_fields. for TCAM match table there is a dedicated
 *  logic for mask fields, for those fields, first finding all the mask and maskable fields pos in
 *  key_special_fields, than looking for matching pairs and eventually setting them to DBAL */
/*
 * Internal match add entry
 */
static shr_error_e
dnx_flow_match_info_add(
    int unit,
    bcm_flow_handle_info_t * flow_handle_info,
    dnx_flow_cmd_control_info_t * flow_cmd_control,
    bcm_flow_special_fields_t * key_special_fields)
{
    uint32 entry_handle_id;
    uint32 lif_flags;
    dnx_algo_gpm_gport_hw_resources_t gport_hw_resources;
    dnx_flow_app_config_t *flow_app_info = dnx_flow_objects[flow_handle_info->flow_handle];
    dbal_table_type_e table_type;
    uint32 entry_access_id;
    dbal_tables_e dbal_table = DBAL_TABLE_EMPTY;
    int lif_per_core = FALSE;

    SHR_FUNC_INIT_VARS(unit);
    DBAL_FUNC_INIT_VARS(unit);

    DNX_FLOW_LOGGER_PRINT(unit, flow_handle_info, flow_app_info, flow_cmd_control, NULL, key_special_fields);

    SHR_IF_ERR_EXIT(dnx_flow_match_related_dbal_table_get(unit,
                                                          flow_handle_info,
                                                          flow_app_info, key_special_fields, &dbal_table));

    SHR_IF_ERR_EXIT(dbal_tables_table_type_get(unit, dbal_table, &table_type));

    SHR_IF_ERR_EXIT(DBAL_HANDLE_ALLOC(unit, dbal_table, &entry_handle_id));

    /** Create entry access id to TCAM */
    if (table_type == DBAL_TABLE_TYPE_TCAM)
    {
        SHR_IF_ERR_EXIT(dbal_entry_attribute_set
                        (unit, entry_handle_id, DBAL_ENTRY_ATTR_PRIORITY, flow_handle_info->flow_priority));
    }

    if (flow_app_info->flow_app_type == FLOW_APP_TYPE_TERM_MATCH)
    {
        lif_flags =
            DNX_ALGO_GPM_GPORT_HW_RESOURCES_LOCAL_LIF_INGRESS | DNX_ALGO_GPM_GPORT_HW_RESOURCES_GLOBAL_LIF_INGRESS;
    }
    else
    {
        lif_flags =
            DNX_ALGO_GPM_GPORT_HW_RESOURCES_LOCAL_LIF_EGRESS | DNX_ALGO_GPM_GPORT_HW_RESOURCES_GLOBAL_LIF_EGRESS;
    }

    SHR_IF_ERR_EXIT(dnx_algo_gpm_gport_to_hw_resources
                    (unit, flow_handle_info->flow_id, lif_flags, &gport_hw_resources));

    SHR_IF_ERR_EXIT(dnx_flow_special_fields_set(unit, entry_handle_id,
                                                flow_handle_info,
                                                flow_app_info, &gport_hw_resources, key_special_fields));

    /** Setting BUD field */
    if (dnx_flow_app_is_ind_set(unit, flow_app_info, FLOW_APP_TERM_MATCH_IND_BUD_BY_KEY_FIELD))
    {
        dbal_entry_key_field32_set(unit, entry_handle_id, (dbal_fields_e) flow_app_info->second_pass_param,
                                   _SHR_IS_FLAG_SET(flow_handle_info->flags, BCM_FLOW_HANDLE_INFO_BUD));
    }

    if (flow_app_info->flow_app_type == FLOW_APP_TYPE_TERM_MATCH)
    {
        if (dnx_data_lif.in_lif.feature_get(unit, dnx_data_lif_in_lif_phy_db_dpc) &&
            (gport_hw_resources.local_in_lif_per_core.core_id[0] != _SHR_CORE_ALL))
        {
            lif_per_core = TRUE;
        }
        else
        {
            if (dbal_table != DBAL_TABLE_INGRESS_PP_PORT)
            {
                if ((dbal_table == DBAL_TABLE_IPV6_MP_TT_TCAM_BASIC)
                    || (dbal_table == DBAL_TABLE_IPV6_MP_TT_TCAM_2ND_PASS))
                {
                    /** The IPv6 TCAM tables has  field called IPv6_DIP_IDX_DATA */
                    dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_IPV6_DIP_IDX_DATA, INST_SINGLE,
                                                 gport_hw_resources.local_in_lif);
                    dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_IPV6_DIP_IDX_TYPE, INST_SINGLE,
                                                 DBAL_ENUM_FVAL_IPV6_DIP_IDX_TYPE_LIF_ELIGIBLE);
                }
                else
                {
                    /**  The INGRESS_PP_PORT has a different In-LIF field name - DEFAULT_LIF, this is handled in the specific app cb */
                    dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_IN_LIF, INST_SINGLE,
                                                 gport_hw_resources.local_in_lif);
                }
            }
        }
    }
    else
    {
        dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_OUT_LIF, INST_SINGLE,
                                     gport_hw_resources.local_out_lif);
    }

    if (flow_app_info->app_specific_operations_cb)
    {
        SHR_IF_ERR_EXIT(flow_app_info->app_specific_operations_cb
                        (unit, entry_handle_id, FLOW_ENTRY_HANDLE_ID_INVALID, flow_handle_info, flow_cmd_control,
                         &gport_hw_resources, NULL, key_special_fields, NULL));
        if (flow_cmd_control->flow_command != FLOW_COMMAND_CONTINUE)
        {
            SHR_EXIT();
        }
    }

    /** Check if errors occurred on handle, using DBAL validation to make sure that the user values are correct. */
    SHR_IF_ERR_EXIT(dnx_flow_commit_error_check(unit, entry_handle_id));

    if (!lif_per_core)
    {
        dbal_entry_key_field32_set(unit, entry_handle_id, DBAL_FIELD_CORE_ID, DBAL_CORE_ALL);

        if (_SHR_IS_FLAG_SET(flow_handle_info->flags, BCM_FLOW_HANDLE_INFO_REPLACE))
        {
            if (table_type == DBAL_TABLE_TYPE_TCAM)
            {
                SHR_IF_ERR_EXIT(dbal_entry_access_id_by_key_get(unit, entry_handle_id, &entry_access_id, DBAL_COMMIT));
                SHR_IF_ERR_EXIT(dbal_entry_handle_access_id_set(unit, entry_handle_id, entry_access_id));
            }
            SHR_IF_ERR_EXIT(dbal_entry_commit(unit, entry_handle_id, DBAL_COMMIT_UPDATE));
        }
        else
        {
            SHR_IF_ERR_EXIT(dbal_entry_commit(unit, entry_handle_id, DBAL_COMMIT));
        }
    }
    else
    {
        int core_iter;

        for (core_iter = 0; core_iter < gport_hw_resources.local_in_lif_per_core.nof_in_lifs; core_iter++)
        {
            dbal_entry_key_field32_set(unit, entry_handle_id, DBAL_FIELD_CORE_ID,
                                       gport_hw_resources.local_in_lif_per_core.core_id[core_iter]);

            if ((dbal_table == DBAL_TABLE_IPV6_MP_TT_TCAM_BASIC) || (dbal_table == DBAL_TABLE_IPV6_MP_TT_TCAM_2ND_PASS))
            {
                /** The IPv6 TCAM tables has  field called IPv6_DIP_IDX_DATA */
                dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_IPV6_DIP_IDX_DATA, INST_SINGLE,
                                             gport_hw_resources.local_in_lif_per_core.local_in_lif[core_iter]);
                dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_IPV6_DIP_IDX_TYPE, INST_SINGLE,
                                             DBAL_ENUM_FVAL_IPV6_DIP_IDX_TYPE_LIF_ELIGIBLE);
            }
            else
            {
                dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_IN_LIF, INST_SINGLE,
                                             gport_hw_resources.local_in_lif_per_core.local_in_lif[core_iter]);
            }
            if (_SHR_IS_FLAG_SET(flow_handle_info->flags, BCM_FLOW_HANDLE_INFO_REPLACE))
            {
                if (table_type == DBAL_TABLE_TYPE_TCAM)
                {
                    SHR_IF_ERR_EXIT(dbal_entry_access_id_by_key_get
                                    (unit, entry_handle_id, &entry_access_id, DBAL_COMMIT));
                    SHR_IF_ERR_EXIT(dbal_entry_handle_access_id_set(unit, entry_handle_id, entry_access_id));
                }
                SHR_IF_ERR_EXIT(dbal_entry_commit(unit, entry_handle_id, DBAL_COMMIT_UPDATE));
            }
            else
            {
                SHR_IF_ERR_EXIT(dbal_entry_commit(unit, entry_handle_id, DBAL_COMMIT));
            }
        }
    }

exit:
    DBAL_FUNC_FREE_VARS;
    SHR_FUNC_EXIT;
}

/*
 * Internal match entry delete
 */
static shr_error_e
dnx_flow_match_info_delete(
    int unit,
    bcm_flow_handle_info_t * flow_handle_info,
    dnx_flow_cmd_control_info_t * flow_cmd_control,
    bcm_flow_special_fields_t * key_special_fields)
{
    uint32 entry_handle_id;
    dnx_flow_app_config_t *flow_app_info = dnx_flow_objects[flow_handle_info->flow_handle];
    dbal_table_type_e table_type;
    dbal_tables_e dbal_table = DBAL_TABLE_EMPTY;
    dbal_core_mode_e core_mode = DBAL_CORE_NONE;

    SHR_FUNC_INIT_VARS(unit);
    DBAL_FUNC_INIT_VARS(unit);

    DNX_FLOW_LOGGER_PRINT(unit, flow_handle_info, flow_app_info, flow_cmd_control, NULL, key_special_fields);

    SHR_IF_ERR_EXIT(dnx_flow_match_related_dbal_table_get(unit,
                                                          flow_handle_info,
                                                          flow_app_info, key_special_fields, &dbal_table));

    SHR_IF_ERR_EXIT(dbal_tables_table_type_get(unit, dbal_table, &table_type));

    SHR_IF_ERR_EXIT(dbal_tables_core_mode_get(unit, dbal_table, &core_mode));

    SHR_IF_ERR_EXIT(DBAL_HANDLE_ALLOC(unit, dbal_table, &entry_handle_id));

    SHR_IF_ERR_EXIT(dnx_flow_special_fields_set(unit,
                                                entry_handle_id,
                                                flow_handle_info, flow_app_info, NULL, key_special_fields));

    /** Setting BUD field */
    if (dnx_flow_app_is_ind_set(unit, flow_app_info, FLOW_APP_TERM_MATCH_IND_BUD_BY_KEY_FIELD))
    {
        dbal_entry_key_field32_set(unit, entry_handle_id, (dbal_fields_e) flow_app_info->second_pass_param,
                                   _SHR_IS_FLAG_SET(flow_handle_info->flags, BCM_FLOW_HANDLE_INFO_BUD));
    }

    /** If a specific application callback is defined - call it */
    if (flow_app_info->app_specific_operations_cb)
    {
        SHR_IF_ERR_EXIT(flow_app_info->app_specific_operations_cb
                        (unit, entry_handle_id, FLOW_ENTRY_HANDLE_ID_INVALID, flow_handle_info, flow_cmd_control, NULL,
                         NULL, key_special_fields, NULL));
        if (flow_cmd_control->flow_command != FLOW_COMMAND_CONTINUE)
        {
            SHR_EXIT();
        }
    }

    {
        int core_iter;
        int nof_cores = (core_mode == DBAL_CORE_MODE_SBC || (dnx_data_lif.in_lif.feature_get(unit,
                                                                                             dnx_data_lif_in_lif_phy_db_dpc)))
            ? 1 : dnx_data_device.general.nof_cores_get(unit);
        int entry_clear_rv = 0;
        int entry_cleared = 0;

        for (core_iter = 0; core_iter < nof_cores; core_iter++)
        {
            if (nof_cores == 1)
            {
                dbal_entry_key_field32_set(unit, entry_handle_id, DBAL_FIELD_CORE_ID, DBAL_CORE_ALL);
            }
            else
            {
                dbal_entry_key_field32_set(unit, entry_handle_id, DBAL_FIELD_CORE_ID, core_iter);
            }
            if (table_type == DBAL_TABLE_TYPE_TCAM)
            {
                int rv;
                uint32 entry_access_id;
                rv = dbal_entry_access_id_by_key_get(unit, entry_handle_id, &entry_access_id, DBAL_COMMIT);

                if (rv == _SHR_E_NONE)
                {
                    SHR_IF_ERR_EXIT(dbal_entry_handle_access_id_set(unit, entry_handle_id, entry_access_id));
                    /** Delete entry from DBAL table*/
                    SHR_IF_ERR_EXIT(dbal_entry_clear(unit, entry_handle_id, DBAL_COMMIT));
                    entry_cleared = 1;
                }
            }
            else
            {
                entry_clear_rv |= dbal_entry_clear(unit, entry_handle_id, DBAL_COMMIT);
                if (entry_clear_rv == _SHR_E_NONE)
                {

                    entry_cleared = 1;
                }
            }
        }

        if (!entry_cleared)
        {
            SHR_ERR_EXIT(_SHR_E_NOT_FOUND, "Entry not found for related key \n");
        }
    }

exit:
    DBAL_FUNC_FREE_VARS;
    SHR_FUNC_EXIT;
}

/** Internal match entry get */
static shr_error_e
dnx_flow_match_info_get(
    int unit,
    bcm_flow_handle_info_t * flow_handle_info,
    dnx_flow_cmd_control_info_t * flow_cmd_control,
    bcm_flow_special_fields_t * key_special_fields)
{
    uint32 entry_handle_id;
    uint32 local_lif;
    dnx_flow_app_config_t *flow_app_info = dnx_flow_objects[flow_handle_info->flow_handle];
    dbal_table_type_e table_type;
    dbal_core_mode_e core_mode = DBAL_CORE_NONE;
    dbal_tables_e dbal_table = DBAL_TABLE_EMPTY;

    SHR_FUNC_INIT_VARS(unit);
    DBAL_FUNC_INIT_VARS(unit);

    DNX_FLOW_LOGGER_PRINT(unit, flow_handle_info, flow_app_info, flow_cmd_control, NULL, key_special_fields);

    SHR_IF_ERR_EXIT(dnx_flow_match_related_dbal_table_get(unit,
                                                          flow_handle_info,
                                                          flow_app_info, key_special_fields, &dbal_table));
    SHR_IF_ERR_EXIT(dbal_tables_table_type_get(unit, dbal_table, &table_type));

    SHR_IF_ERR_EXIT(DBAL_HANDLE_ALLOC(unit, dbal_table, &entry_handle_id));

    /** setting the key of the entry */
    SHR_IF_ERR_EXIT(dnx_flow_special_fields_set(unit,
                                                entry_handle_id,
                                                flow_handle_info, flow_app_info, NULL, key_special_fields));

    SHR_IF_ERR_EXIT(dbal_tables_core_mode_get(unit, dbal_table, &core_mode));
    if ((core_mode == DBAL_CORE_MODE_DPC) && !(dnx_data_lif.in_lif.feature_get(unit, dnx_data_lif_in_lif_phy_db_dpc)))
    {
        /** setting the core_id to default (get requires to be done on only one of the cores) */
        dbal_entry_key_field32_set(unit, entry_handle_id, DBAL_FIELD_CORE_ID, DBAL_CORE_DEFAULT);
    }

    /** Setting BUD field */
    if (dnx_flow_app_is_ind_set(unit, flow_app_info, FLOW_APP_TERM_MATCH_IND_BUD_BY_KEY_FIELD))
    {
        dbal_entry_key_field32_set(unit, entry_handle_id, (dbal_fields_e) flow_app_info->second_pass_param,
                                   _SHR_IS_FLAG_SET(flow_handle_info->flags, BCM_FLOW_HANDLE_INFO_BUD));
    }

    if (flow_app_info->app_specific_operations_cb)
    {
        SHR_IF_ERR_EXIT(flow_app_info->app_specific_operations_cb
                        (unit, entry_handle_id, FLOW_ENTRY_HANDLE_ID_INVALID, flow_handle_info, flow_cmd_control, NULL,
                         NULL, key_special_fields, NULL));
        if (flow_cmd_control->flow_command != FLOW_COMMAND_CONTINUE)
        {
            SHR_EXIT();
        }
    }

    if (table_type == DBAL_TABLE_TYPE_TCAM)
    {
        uint32 tcam_handler_id, entry_access_id;
        SHR_IF_ERR_EXIT(dbal_tables_tcam_handler_id_get(unit, dbal_table, &tcam_handler_id));
        SHR_IF_ERR_EXIT_NO_MSG(dbal_entry_access_id_by_key_get(unit, entry_handle_id, &entry_access_id, DBAL_COMMIT));
        SHR_IF_ERR_EXIT(dbal_entry_handle_access_id_set(unit, entry_handle_id, entry_access_id));

        SHR_IF_ERR_EXIT(dnx_field_tcam_handler_entry_priority_get
                        (unit, tcam_handler_id, entry_access_id, &flow_handle_info->flow_priority));
    }

    /** in case of entry was added not to all the cores need to loop over all cores and see if entry exists once found,
     *  need to return the related global LIF */
    if ((core_mode == DBAL_CORE_MODE_DPC) && (flow_app_info->flow_app_type == FLOW_APP_TYPE_TERM_MATCH) &&
        dnx_data_lif.in_lif.feature_get(unit, dnx_data_lif_in_lif_phy_db_dpc))
    {
        int rv, core_iter, entry_found = 0;
        int nof_cores = dnx_data_device.general.nof_cores_get(unit);
        for (core_iter = 0; core_iter < nof_cores; core_iter++)
        {
            dbal_entry_key_field32_set(unit, entry_handle_id, DBAL_FIELD_CORE_ID, core_iter);
            rv = dbal_entry_get(unit, entry_handle_id, DBAL_GET_ALL_FIELDS);
            if (rv == _SHR_E_NONE)
            {

                SHR_IF_ERR_EXIT(dbal_entry_handle_value_field32_get
                                (unit, entry_handle_id, DBAL_FIELD_IN_LIF, INST_SINGLE, &local_lif));

                /** Retrieve the flow id from the lif and encode it as a gport */
                SHR_IF_ERR_EXIT(dnx_flow_local_lif_to_flow_id
                                (unit, flow_app_info, flow_handle_info, local_lif, core_iter));

                entry_found = 1;
                break;
            }
        }

        if (!entry_found)
        {
            SHR_ERR_EXIT(_SHR_E_NOT_FOUND, "Entry not found for related key \n");
        }
    }
    else
    {
        SHR_IF_ERR_EXIT(dbal_entry_get(unit, entry_handle_id, DBAL_GET_ALL_FIELDS));
        if (flow_app_info->flow_app_type == FLOW_APP_TYPE_TERM_MATCH)
        {
            if ((dbal_table == DBAL_TABLE_IPV6_MP_TT_TCAM_BASIC) || (dbal_table == DBAL_TABLE_IPV6_MP_TT_TCAM_2ND_PASS))
            {
                /** The IPv6 TCAM tables has  field called IPv6_DIP_IDX_DATA */
                SHR_IF_ERR_EXIT(dbal_entry_handle_value_field32_get
                                (unit, entry_handle_id, DBAL_FIELD_IPV6_DIP_IDX_DATA, INST_SINGLE, &local_lif));
            }
            else
            {
                SHR_IF_ERR_EXIT(dbal_entry_handle_value_field32_get
                                (unit, entry_handle_id, DBAL_FIELD_IN_LIF, INST_SINGLE, &local_lif));
            }
        }
        else
        {
            SHR_IF_ERR_EXIT(dbal_entry_handle_value_field32_get
                            (unit, entry_handle_id, DBAL_FIELD_OUT_LIF, INST_SINGLE, &local_lif));
        }

        /** Retrieve the flow id from the lif and encode it as a gport */
        SHR_IF_ERR_EXIT(dnx_flow_local_lif_to_flow_id
                        (unit, flow_app_info, flow_handle_info, local_lif, DBAL_CORE_DEFAULT));
    }

    if (flow_app_info->is_entry_related_cb &&
        dnx_flow_app_is_ind_set(unit, flow_app_info, FLOW_APP_TERM_MATCH_IND_MANY_TO_ONE))
    {
        int is_entry_related;

        SHR_IF_ERR_EXIT(flow_app_info->is_entry_related_cb(unit, entry_handle_id, &is_entry_related));
        if (!is_entry_related)
        {
            SHR_ERR_EXIT(_SHR_E_NOT_FOUND, "Entry not found for related key \n");
        }
    }

exit:
    DBAL_FUNC_FREE_VARS;
    SHR_FUNC_EXIT;
}

/** returns is_designated true according to predefine logic, this function is only valid for applications that are
 *  using the indication FLOW_APP_TERM_MATCH_IND_PER_CORE. there are two options,
 *  a) user entry is mapped to one HW entry (in case of port) so in this case the entry is always designated
 *  b) user entry is mapped to multiple HW entries, (such as vlan domain or LAG) in this case the function should return
 *  true only for the designated entry
 *  
 *  in case no indication for PER core we expect that for each user entry there will be added num_of_cores entries,
 *  and the iterator will run only over the default core entries */
static shr_error_e
dnx_flow_match_is_entry_designated(
    int unit,
    int core_id,
    dnx_flow_app_config_t * flow_app_info,
    uint32 entry_handle_id,
    bcm_flow_handle_info_t * flow_handle_info,
    int global_lif,
    int *is_designated)
{
    int port_found = 0;
    uint32 field_value = 0;
    int idx = 0;

    SHR_FUNC_INIT_VARS(unit);

    (*is_designated) = 0;

    while (flow_app_info->special_fields[idx] != FLOW_S_F_EMPTY)
    {
        if (flow_app_info->special_fields[idx] == FLOW_S_F_IN_PORT)
        {
            uint32 pp_port;
            bcm_port_t phy_port_gport;
            int valid;

            port_found = 1;
            /** Retrieve the PP-Port */
            SHR_IF_ERR_EXIT(dbal_entry_handle_key_field_arr32_get(unit,
                                                                  entry_handle_id, DBAL_FIELD_IN_PP_PORT, &pp_port));

            /** Convert to a logical Port */
            SHR_IF_ERR_EXIT(dnx_algo_port_db.pp.ref_count.get(unit, core_id, pp_port, &valid));

            if (valid)
            {
                SHR_IF_ERR_EXIT(dnx_algo_port_pp_to_gport_get(unit, core_id, pp_port, &phy_port_gport));
                field_value = phy_port_gport;
                break;
            }
            else
            {
                port_found = 0;
            }
        }
        idx++;
    }

    if (port_found)
    {
        dnx_algo_gpm_gport_phy_info_t gport_info;
        uint32 pp_port_index;
        uint32 min_core_id;

        /** Convert the local port to a list of PP-Port + Core-ID */
        SHR_IF_ERR_EXIT(dnx_algo_gpm_gport_phy_info_get
                        (unit, field_value, DNX_ALGO_GPM_GPORT_TO_PHY_OP_PP_PORT_IS_MANDATORY, &gport_info));

        min_core_id = gport_info.internal_port_pp_info.core_id[0];
        for (pp_port_index = 1; pp_port_index < gport_info.internal_port_pp_info.nof_pp_ports; pp_port_index++)
        {
            if (min_core_id > gport_info.internal_port_pp_info.core_id[pp_port_index])
            {
                min_core_id = gport_info.internal_port_pp_info.core_id[pp_port_index];
            }
        }

        if (min_core_id == core_id)
        {
            (*is_designated) = 1;
        }
    }
    else
    {
        
        (*is_designated) = 1;
    }

exit:
    SHR_FUNC_EXIT;
}

static shr_error_e
dnx_flow_match_info_traverse_per_table(
    int unit,
    bcm_flow_handle_info_t * flow_handle_info,
    dnx_flow_app_config_t * flow_app_info,
    bcm_flow_match_info_traverse_cb cb,
    void *user_data,
    dnx_flow_traverse_operation_e traverse_operation,
    dbal_tables_e dbal_table,
    bcm_flow_special_fields_t * key_special_fields,
    int bud_table_in_use)
{
    dbal_table_type_e table_type;
    dbal_physical_tables_e physical_table_id = DBAL_PHYSICAL_TABLE_NONE;
    dbal_core_mode_e core_mode = DBAL_CORE_NONE;
    uint32 entry_handle_id;
    int is_end, global_lif, local_lif, is_phy_dpc =
        dnx_data_lif.in_lif.feature_get(unit, dnx_data_lif_in_lif_phy_db_dpc), is_lif_dpc = 0;
    lif_mapping_local_lif_key_t local_lif_info;
    dnx_flow_cmd_control_info_t flow_cmd_control = { FLOW_CB_TYPE_TRAVERSE, FLOW_COMMAND_CONTINUE };
    uint32 core_id = DBAL_CORE_DEFAULT;
    uint32 orig_flags = flow_handle_info->flags;
    bcm_flow_special_fields_t orig_key_special_fields = { 0 };

    SHR_FUNC_INIT_VARS(unit);
    DBAL_FUNC_INIT_VARS(unit);

    /** keeping the original special fields, in case changed we will be able to restore */
    sal_memcpy(&orig_key_special_fields, key_special_fields, sizeof(bcm_flow_special_fields_t));

    if (is_phy_dpc)
    {
    /** lif is dpc if the app is DPC and the system is dpc */
        is_lif_dpc = dnx_flow_app_is_ind_set(unit, flow_app_info, FLOW_APP_TERM_MATCH_IND_PER_CORE);
    }

    SHR_IF_ERR_EXIT(dbal_tables_table_type_get(unit, dbal_table, &table_type));
    if (table_type != DBAL_TABLE_TYPE_DIRECT)
    {
        SHR_IF_ERR_EXIT(dbal_tables_physical_table_get
                        (unit, dbal_table, DBAL_PHY_DB_DEFAULT_INDEX, &physical_table_id));
        core_mode = dbal_mdb_phy_table_core_mode_get(unit, physical_table_id);
    }

    SHR_IF_ERR_EXIT(DBAL_HANDLE_ALLOC(unit, dbal_table, &entry_handle_id));

    SHR_IF_ERR_EXIT(dbal_iterator_init(unit, entry_handle_id, DBAL_ITER_MODE_GET_ALL_EXCEPT_DEFAULT));

    if (!is_lif_dpc)
    {
        if ((flow_app_info->flow_app_type == FLOW_APP_TYPE_TERM_MATCH) && (core_mode != DBAL_CORE_MODE_SBC)
            && (dbal_table != DBAL_TABLE_INGRESS_PP_PORT))
        {
            /** Add KEY rule to skip duplicated entry */
            SHR_IF_ERR_EXIT(dbal_iterator_key_field_arr32_rule_add(unit, entry_handle_id, DBAL_FIELD_CORE_ID,
                                                                   DBAL_CONDITION_EQUAL_TO, &core_id, NULL));
        }
    }

    /** Receive first entry in table. */
    SHR_IF_ERR_EXIT(dbal_iterator_get_next(unit, entry_handle_id, &is_end));
    while (!is_end)
    {
        int rv;

        if (key_special_fields->actual_nof_special_fields != orig_key_special_fields.actual_nof_special_fields)
        {
            sal_memcpy(key_special_fields, &orig_key_special_fields, sizeof(bcm_flow_special_fields_t));
        }

        if (flow_app_info->is_entry_related_cb &&
            dnx_flow_app_is_ind_set(unit, flow_app_info, FLOW_APP_TERM_MATCH_IND_MANY_TO_ONE))
        {
            int is_entry_related;

            SHR_IF_ERR_EXIT(flow_app_info->is_entry_related_cb(unit, entry_handle_id, &is_entry_related));
            if (!is_entry_related)
            {
            /** Receive next entry in table.*/
                SHR_IF_ERR_EXIT(dbal_iterator_get_next(unit, entry_handle_id, &is_end));
                continue;
            }
        }

        SHR_IF_ERR_EXIT(dnx_flow_match_key_fields_get
                        (unit, entry_handle_id, flow_handle_info, flow_app_info, table_type, key_special_fields,
                         key_special_fields->actual_nof_special_fields));

        /** Getting BUD field  value and updating the flag   */
        if (dnx_flow_app_is_ind_set(unit, flow_app_info, FLOW_APP_TERM_MATCH_IND_BUD_BY_KEY_FIELD))
        {
            uint32 second_pass_val;

            SHR_IF_ERR_EXIT(dbal_entry_handle_key_field_arr32_get
                            (unit, entry_handle_id, (dbal_fields_e) flow_app_info->second_pass_param,
                             &second_pass_val));
            if (second_pass_val)
            {
                flow_handle_info->flags |= BCM_FLOW_HANDLE_INFO_BUD;
            }
        }

        if (flow_app_info->dbal_table_to_key_field_map_cb)
        {
            /** used in traverse to map the dbal table to the key field. */
            SHR_IF_ERR_EXIT(flow_app_info->dbal_table_to_key_field_map_cb
                            (unit, entry_handle_id, flow_handle_info, FLOW_MAP_TYPE_TABLE_TO_KEY_FIELD,
                             key_special_fields, &dbal_table));
        }
        /*
         * If a specific application callback is defined - call it
         */
        if (flow_app_info->app_specific_operations_cb)
        {
            SHR_IF_ERR_EXIT(flow_app_info->app_specific_operations_cb
                            (unit, entry_handle_id, FLOW_ENTRY_HANDLE_ID_INVALID, flow_handle_info,
                             &flow_cmd_control, NULL, NULL, key_special_fields, NULL));
        }

        if (flow_cmd_control.flow_command == FLOW_COMMAND_CONTINUE)
        {
            if (flow_app_info->flow_app_type == FLOW_APP_TYPE_TERM_MATCH)
            {
                if ((dbal_table == DBAL_TABLE_IPV6_MP_TT_TCAM_BASIC)
                    || (dbal_table == DBAL_TABLE_IPV6_MP_TT_TCAM_2ND_PASS))
                {
                    /** The IPv6 TCAM tables has  field called IPv6_DIP_IDX_DATA */
                    SHR_IF_ERR_EXIT(dbal_entry_handle_value_field32_get
                                    (unit, entry_handle_id, DBAL_FIELD_IPV6_DIP_IDX_DATA, INST_SINGLE,
                                     (uint32 *) &local_lif));
                }
                else
                {
                    SHR_IF_ERR_EXIT(dbal_entry_handle_value_field32_get
                                    (unit, entry_handle_id, DBAL_FIELD_IN_LIF, INST_SINGLE, (uint32 *) &local_lif));
                }
                if (!is_phy_dpc)
                {
                    local_lif_info.phy_table = (physical_table_id == DBAL_PHYSICAL_TABLE_ISEM_2) ?
                        DBAL_PHYSICAL_TABLE_INLIF_2 : DBAL_PHYSICAL_TABLE_INLIF_1;
                }
                local_lif_info.local_lif = local_lif;
                local_lif_info.core_id = core_id;
                rv = dnx_algo_lif_mapping_local_to_global_get(unit, DNX_ALGO_LIF_INGRESS, &local_lif_info, &global_lif);
                SHR_IF_ERR_EXIT_EXCEPT_IF(rv, _SHR_E_NOT_FOUND);
                if (rv == _SHR_E_NONE)
                {
                    BCM_GPORT_SUB_TYPE_IN_LIF_SET(flow_handle_info->flow_id, global_lif);
                }
                else
                {
                    int virtual_id = 0;
                    rv = dnx_flow_ingress_virtual_gport_get(unit, local_lif, core_id, &virtual_id);
                    SHR_IF_ERR_EXIT_EXCEPT_IF(rv, _SHR_E_NOT_FOUND);
                    if (rv == _SHR_E_NONE)
                    {
                        BCM_GPORT_SUB_TYPE_VIRTUAL_INGRESS_SET(flow_handle_info->flow_id, virtual_id);
                    }
                    else
                    {
                        LOG_WARN_EX(BSL_LOG_MODULE,
                                    "Travrese encountered a Local-LIF (0x%x) that has no gport association%s%s%s\n",
                                    local_lif, EMPTY, EMPTY, EMPTY);
                        SHR_IF_ERR_EXIT(dbal_iterator_get_next(unit, entry_handle_id, &is_end));
                    }
                }
            }
            else
            {
                SHR_IF_ERR_EXIT(dbal_entry_handle_value_field32_get
                                (unit, entry_handle_id, DBAL_FIELD_OUT_LIF, INST_SINGLE, (uint32 *) &local_lif));
                local_lif_info.local_lif = local_lif;
                local_lif_info.core_id = core_id;
                rv = dnx_algo_lif_mapping_local_to_global_get(unit, DNX_ALGO_LIF_EGRESS, &local_lif_info, &global_lif);
                SHR_IF_ERR_EXIT_EXCEPT_IF(rv, _SHR_E_NOT_FOUND);
                if (rv == _SHR_E_NONE)
                {
                    BCM_GPORT_SUB_TYPE_OUT_LIF_SET(flow_handle_info->flow_id, global_lif);
                }
            }
            BCM_GPORT_FLOW_LIF_ID_SET(flow_handle_info->flow_id, flow_handle_info->flow_id);
        }
        if (flow_cmd_control.flow_command != FLOW_COMMAND_SKIP_TO_END)
        {
            if (traverse_operation == FLOW_TRAVERSE_OPERATION_USE_CALLBACK)
            {
                int is_designated = 1;

                /** Invoke the callback function */
                if ((flow_app_info->flow_app_type == FLOW_APP_TYPE_TERM_MATCH) && (core_mode != DBAL_CORE_MODE_SBC)
                    && (dbal_table != DBAL_TABLE_INGRESS_PP_PORT) && (is_lif_dpc))
                {
                    uint32 core_id;
                    SHR_IF_ERR_EXIT(dbal_entry_handle_key_field_arr32_get
                                    (unit, entry_handle_id, DBAL_FIELD_CORE_ID, &core_id));

                    SHR_IF_ERR_EXIT(dnx_flow_match_is_entry_designated
                                    (unit, core_id, flow_app_info, entry_handle_id, flow_handle_info, global_lif,
                                     &is_designated));
                }

                if (is_designated)
                {
                    if (bud_table_in_use)
                    {
                        flow_handle_info->flags |= BCM_FLOW_HANDLE_INFO_BUD;
                    }
                    SHR_IF_ERR_EXIT((*cb) (unit, flow_handle_info, key_special_fields, user_data));
                    flow_handle_info->flags = orig_flags;
                }
            }

            if (traverse_operation == FLOW_TRAVERSE_OPERATION_DELETE)
            {
                dnx_flow_cmd_control_info_t flow_cmd_control = { FLOW_CB_TYPE_DELETE, FLOW_COMMAND_CONTINUE };

                SHR_IF_ERR_EXIT(dnx_flow_match_info_delete
                                (unit, flow_handle_info, &flow_cmd_control, key_special_fields));
                flow_handle_info->flags = orig_flags;
            }
        }

        /** Receive next entry in table.*/
        SHR_IF_ERR_EXIT(dbal_iterator_get_next(unit, entry_handle_id, &is_end));
    }

exit:
    flow_handle_info->flags = orig_flags;
    DBAL_FUNC_FREE_VARS;
    SHR_FUNC_EXIT;
}

shr_error_e
dnx_flow_match_info_traverse(
    int unit,
    bcm_flow_handle_info_t * flow_handle_info,
    bcm_flow_match_info_traverse_cb cb,
    void *user_data,
    dnx_flow_traverse_operation_e traverse_operation)
{
    bcm_flow_special_fields_t key_special_fields = { 0 };
    int table_idx = 0;
    dnx_flow_app_config_t *flow_app_info;
    int nof_app_special_fields = 0;
    dbal_tables_e dbal_table;
    int bud_table_used = 0, is_bud_uses_extra_table = 0;
    int nof_regular_tables = 1;

    SHR_FUNC_INIT_VARS(unit);

    flow_app_info = dnx_flow_objects[flow_handle_info->flow_handle];

    dbal_table = flow_app_info->flow_table[table_idx];

    if (_SHR_IS_FLAG_SET(flow_app_info->valid_flow_flags, BCM_FLOW_HANDLE_INFO_BUD)
        && (!dnx_flow_app_is_ind_set(unit, flow_app_info, FLOW_APP_TERM_MATCH_IND_BUD_BY_KEY_FIELD)))
    {
        is_bud_uses_extra_table = 1;
    }

    if (dnx_flow_app_is_ind_set(unit, flow_app_info, FLOW_APP_TERM_MATCH_IND_MULTIPLE_DBAL_TABLES))
    {
        while ((nof_regular_tables < FLOW_MAX_NOF_MATCH_DBAL_TABLES) &&
               (flow_app_info->flow_table[nof_regular_tables] != DBAL_TABLE_EMPTY))
        {
            nof_regular_tables++;
        }
    }

     /** Get number of key special fields, serves as key for match applications Assign each key field id */
    while (flow_app_info->special_fields[nof_app_special_fields] != FLOW_S_F_EMPTY)
    {
        key_special_fields.special_fields[nof_app_special_fields].field_id =
            flow_app_info->special_fields[nof_app_special_fields];
        nof_app_special_fields++;
    }

    key_special_fields.actual_nof_special_fields = nof_app_special_fields;

    while (dbal_table != DBAL_TABLE_EMPTY)
    {
        SHR_IF_ERR_EXIT(dnx_flow_match_info_traverse_per_table
                        (unit, flow_handle_info, flow_app_info, cb, user_data, traverse_operation, dbal_table,
                         &key_special_fields, bud_table_used));

       /** selecting the next table to handle, should run over all valid tables and BUD table if exists */
        table_idx++;
        if (bud_table_used)
        {
            break;
        }

        if (table_idx == nof_regular_tables)
        {
            if (!is_bud_uses_extra_table)
            {
               /** all tables traversed - exit */
                break;
            }
            else
            {
               /** need to traverse also the BUD table */
                bud_table_used = 1;
                dbal_table = (dbal_tables_e) flow_app_info->second_pass_param;
            }
        }
        else
        {
            dbal_table = flow_app_info->flow_table[table_idx];
        }
    }

exit:
    SHR_FUNC_EXIT;
}

/**********************************************************************************************
*                                  Internal functions - END                                   *
**********************************************************************************************/

/**********************************************************************************************
*                                        APIs                                                 *
**********************************************************************************************/

/**
 * \brief
 * Get flow handle based on application.
 *
 * \param [in]  unit - Relevant unit.
 * \param [in]  flow_name - relevant application matching name \n
 * \param [out] handle - the flow handle \n
 * \return
 *   \retval Zero in case of NO ERROR.
 *   \retval Negative in case of an error.
 */
shr_error_e
bcm_dnx_flow_handle_get(
    int unit,
    const char *flow_name,
    bcm_flow_handle_t * handle)
{
    int app_idx, num_apps;

    SHR_FUNC_INIT_VARS(unit);
    DNX_ERR_RECOVERY_NOT_NEEDED(unit);

    SHR_SET_CURRENT_ERR(_SHR_E_NONE);

    num_apps = dnx_flow_number_of_apps();

    for (app_idx = FLOW_APP_FIRST_IDX; app_idx < num_apps; app_idx++)
    {
        if (FLOW_APP_IS_NOT_VALID(dnx_flow_objects[app_idx]))
        {
            continue;
        }
        if (!sal_strncasecmp(flow_name, dnx_flow_objects[app_idx]->app_name, FLOW_STR_MAX))
        {
            *handle = app_idx;
            break;
        }
    }
    if (app_idx == num_apps)
    {
        SHR_ERR_EXIT(_SHR_E_PARAM, "Couldn't find matching application object by the name %s", flow_name);
    }

exit:
    SHR_FUNC_EXIT;
}

/**
* \brief
* Create the terminator flow lif.
*
* \param [in]  unit - Relevant unit.
* \param [in,out] flow_handle_info - pointer to a struct containing the relevant general flow attributes \n
* \param [in,out] terminator_info - pointer to a struct containing the relevant terminator flow attributes \n
* \param [in,out] special_fields - list of fields which are not common (not part of the terminator flow struct) \n
*
* \return
*   \retval Zero in case of NO ERROR.
*   \retval Negative in case of an error.
*/
shr_error_e
bcm_dnx_flow_terminator_info_create(
    int unit,
    bcm_flow_handle_info_t * flow_handle_info,
    bcm_flow_terminator_info_t * terminator_info,
    bcm_flow_special_fields_t * special_fields)
{
    dnx_flow_cmd_control_info_t flow_cmd_control = { FLOW_CB_TYPE_SET, FLOW_COMMAND_CONTINUE };

    SHR_FUNC_INIT_VARS(unit);
    DNX_ERR_RECOVERY_START(unit);

    SHR_INVOKE_VERIFY_DNXC(dnx_flow_create_verify(unit, flow_handle_info, terminator_info, special_fields));

    if (!_SHR_IS_FLAG_SET(flow_handle_info->flags, BCM_FLOW_HANDLE_INFO_REPLACE))
    {
        SHR_IF_ERR_EXIT(dnx_flow_terminator_info_create
                        (unit, flow_handle_info, &flow_cmd_control, terminator_info, special_fields));
    }
    else
    {
        SHR_IF_ERR_EXIT(dnx_flow_terminator_info_replace
                        (unit, flow_handle_info, &flow_cmd_control, terminator_info, special_fields));
    }

exit:
    DNX_ERR_RECOVERY_END(unit);
    SHR_FUNC_EXIT;
}

/**
* \brief
* Destroy the terminator flow lif - in order to be traffic safe, first delete the LIF entry, once done remove all LIF 
* related profiles. 
*
* \param [in]  unit - Relevant unit.
* \param [in,out] flow_handle_info - pointer to a struct containing the relevant general flow attributes \n
*
* \return
*   \retval Zero in case of NO ERROR.
*   \retval Negative in case of an error.
*/
shr_error_e
bcm_dnx_flow_terminator_info_destroy(
    int unit,
    bcm_flow_handle_info_t * flow_handle_info)
{
    uint32 lif_flags;
    dnx_algo_gpm_gport_hw_resources_t gport_hw_resources;
    int field = 0;
    uint32 temp_common_bitmap;
    uint32 entry_handle_id;
    bcm_flow_terminator_info_t terminator_info;
    dnx_flow_app_config_t *flow_app_info = dnx_flow_objects[flow_handle_info->flow_handle];
    dnx_flow_cmd_control_info_t flow_cmd_control = { FLOW_CB_TYPE_DELETE, FLOW_COMMAND_CONTINUE };

    SHR_FUNC_INIT_VARS(unit);
    DBAL_FUNC_INIT_VARS(unit);
    DNX_ERR_RECOVERY_START(unit);

    SHR_INVOKE_VERIFY_DNXC(dnx_flow_delete_verify(unit, FLOW_APP_TYPE_TERM, flow_handle_info, NULL));

    DNX_FLOW_LOGGER_PRINT(unit, flow_handle_info, flow_app_info, &flow_cmd_control, NULL, NULL);

    SHR_IF_ERR_EXIT(DBAL_HANDLE_ALLOC(unit, FLOW_LIF_DBAL_TABLE_GET(flow_app_info), &entry_handle_id));

    /** Get global and local inLIF from GPort hw resources */
    lif_flags = DNX_ALGO_GPM_GPORT_HW_RESOURCES_LOCAL_LIF_INGRESS | DNX_ALGO_GPM_GPORT_HW_RESOURCES_GLOBAL_LIF_INGRESS;
    SHR_IF_ERR_EXIT(dnx_algo_gpm_gport_to_hw_resources
                    (unit, flow_handle_info->flow_id, lif_flags, &gport_hw_resources));

    SHR_IF_ERR_EXIT(dnx_flow_lif_mgmt_terminator_info_get
                    (unit, flow_app_info, flow_handle_info, &gport_hw_resources, entry_handle_id));

    /** if app supports the extended termination, check if it was set */
    if (dnx_flow_app_is_ind_set(unit, flow_app_info, FLOW_APP_TERM_IND_SUPPORT_EXTENDED_TERM))
    {
        SHR_IF_ERR_EXIT(dnx_flow_extended_termination_delete(unit,
                                                             _SHR_GPORT_FLOW_LIF_VAL_GET_ID
                                                             (flow_handle_info->flow_id)));
    }

    /** Delete entry from an In-LIF table - Delete the local In-LIF - doing only the HW since some of the SW info is
     *  required in order to release the special fields    */
    SHR_IF_ERR_EXIT(dnx_flow_lif_mgmt_terminator_info_clear(unit, &gport_hw_resources));

    /*
     * Iterate over all common fields. If exists in application and delete callback exists - delete it
     */
    temp_common_bitmap = flow_app_info->valid_common_fields_bitmap;
    while (temp_common_bitmap != 0)
    {
        if ((temp_common_bitmap & 0x1) && (dnx_flow_term_common_fields[field]->field_cbs.field_delete_cb))
        {
            SHR_IF_ERR_EXIT_EXCEPT_IF(dnx_flow_term_common_fields[field]->
                                      field_cbs.field_delete_cb(unit, entry_handle_id, FLOW_APP_TYPE_TERM,
                                                                flow_handle_info, &gport_hw_resources, flow_app_info,
                                                                &terminator_info), _SHR_E_NOT_FOUND);
        }
        temp_common_bitmap = (temp_common_bitmap >> 1);
        field++;
    }

    SHR_IF_ERR_EXIT(dnx_flow_special_fields_destroy
                    (unit, entry_handle_id, flow_handle_info, flow_app_info, &gport_hw_resources));

    SHR_IF_ERR_EXIT(dnx_flow_terminator_dedicated_logic_destroy
                    (unit, flow_app_info, flow_handle_info, entry_handle_id, &gport_hw_resources));

    /** If a specific application callback is defined - call it */
    if (flow_app_info->app_specific_operations_cb)
    {
        SHR_IF_ERR_EXIT(flow_app_info->app_specific_operations_cb
                        (unit, entry_handle_id, FLOW_ENTRY_HANDLE_ID_INVALID, flow_handle_info, &flow_cmd_control,
                         &gport_hw_resources, &terminator_info, NULL, NULL));
        if (flow_cmd_control.flow_command != FLOW_COMMAND_CONTINUE)
        {
            SHR_EXIT();
        }
    }

    SHR_IF_ERR_EXIT(dnx_flow_lif_mgmt_valid_fields_clear(unit, &gport_hw_resources, flow_handle_info, flow_app_info));

    SHR_IF_ERR_EXIT(dnx_flow_gport_flow_id_free(unit, flow_app_info, flow_handle_info, &gport_hw_resources));

exit:
    DNX_ERR_RECOVERY_END(unit);
    DBAL_FUNC_FREE_VARS;
    SHR_FUNC_EXIT;
}

/**
* \brief
* Get a terminator flow lif.
*
* \param [in]  unit - Relevant unit.
* \param [in,out] flow_handle_info - pointer to a struct containing the relevant general flow attributes \n
* \param [in,out] terminator_info - pointer to a struct containing the relevant terminator flow attributes \n
* \param [in,out] special_fields - list of fields which are not common (not part of the terminator flow struct) \n
*
* \return
*   \retval Zero in case of NO ERROR.
*   \retval Negative in case of an error.
*/
shr_error_e
bcm_dnx_flow_terminator_info_get(
    int unit,
    bcm_flow_handle_info_t * flow_handle_info,
    bcm_flow_terminator_info_t * terminator_info,
    bcm_flow_special_fields_t * special_fields)
{
    dnx_flow_cmd_control_info_t flow_cmd_control = { FLOW_CB_TYPE_GET, FLOW_COMMAND_CONTINUE };

    SHR_FUNC_INIT_VARS(unit);
    DBAL_FUNC_INIT_VARS(unit);
    DNX_ERR_RECOVERY_NOT_NEEDED(unit);

    SHR_INVOKE_VERIFY_DNXC(dnx_flow_get_verify
                           (unit, FLOW_APP_TYPE_TERM, flow_handle_info, terminator_info, special_fields));

    SHR_IF_ERR_EXIT(dnx_flow_terminator_info_get
                    (unit, flow_handle_info, &flow_cmd_control, terminator_info, special_fields));

exit:
    DBAL_FUNC_FREE_VARS;
    SHR_FUNC_EXIT;
}

/**
* \brief
* Traverse all entries related to the lif application and run a callback function provided by the user.
*
* \param [in]  unit - Relevant unit.
* \param [in] flow_handle_info - flow_handle_info
* \param [in] cb - user callback
* \param [in] user_data - user data
*
* \return
*   \retval Zero in case of NO ERROR.
*   \retval Negative in case of an error.
*/
shr_error_e
bcm_dnx_flow_terminator_info_traverse(
    int unit,
    bcm_flow_handle_info_t * flow_handle_info,
    bcm_flow_terminator_info_traverse_cb cb,
    void *user_data)
{
    dnx_flow_traverse_operation_e traverse_operation = _SHR_IS_FLAG_SET(flow_handle_info->flags,
                                                                        BCM_FLOW_HANDLE_TRAVERSE_DELETE_ALL) ?
        FLOW_TRAVERSE_OPERATION_DELETE : FLOW_TRAVERSE_OPERATION_USE_CALLBACK;

    SHR_FUNC_INIT_VARS(unit);
    DNX_ERR_RECOVERY_NOT_NEEDED(unit);

    /** Verification of input parameters */
    SHR_INVOKE_VERIFY_DNXC(dnx_flow_traverse_verify(unit, flow_handle_info, (void *) &cb, user_data));

    if (traverse_operation == FLOW_TRAVERSE_OPERATION_DELETE)
    {
        /** removing BCM_FLOW_HANDLE_TRAVERSE_DELETE_ALL flag since traverse will call entry destroy and this flag is illegal there */
        flow_handle_info->flags = 0;
    }

    /*
     * Traverse all terminator applications 
     */
    if (flow_handle_info->flow_handle == BCM_FLOW_HANDLE_TRAVERSE_ALL)
    {
        int num_apps = dnx_flow_number_of_apps();
        int app_idx;
        bcm_flow_handle_info_t traverse_flow_handle_info = { 0 };

        for (app_idx = FLOW_APP_FIRST_IDX; app_idx < num_apps; app_idx++)
        {
            if (FLOW_APP_IS_NOT_VALID(dnx_flow_objects[app_idx]))
            {
                continue;
            }
            if (dnx_flow_objects[app_idx]->flow_app_type == FLOW_APP_TYPE_TERM)
            {
                traverse_flow_handle_info.flow_handle = app_idx;
                SHR_IF_ERR_EXIT(dnx_flow_terminator_info_traverse
                                (unit, &traverse_flow_handle_info, cb, user_data, traverse_operation));
            }
        }
    }
    else
    {
        SHR_IF_ERR_EXIT(dnx_flow_terminator_info_traverse(unit, flow_handle_info, cb, user_data, traverse_operation));
    }

    if (traverse_operation == FLOW_TRAVERSE_OPERATION_DELETE)
    {
    /** returning the flag to original value */
        flow_handle_info->flags |= BCM_FLOW_HANDLE_TRAVERSE_DELETE_ALL;
    }

exit:
    SHR_FUNC_EXIT;
}

/**
* \brief
* Create a initiator flow lif.
*
* \param [in]  unit - Relevant unit.
* \param [in,out] flow_handle_info - pointer to a struct containing the relevant general flow attributes \n
* \param [in,out] initiator_info - pointer to a struct containing the relevant initiator flow attributes \n
* \param [in,out] special_fields - list of fields which are not common (not part of the terminator flow struct) \n
*
* \return
*   \retval Zero in case of NO ERROR.
*   \retval Negative in case of an error.
*/
shr_error_e
bcm_dnx_flow_initiator_info_create(
    int unit,
    bcm_flow_handle_info_t * flow_handle_info,
    bcm_flow_initiator_info_t * initiator_info,
    bcm_flow_special_fields_t * special_fields)
{
    dnx_flow_cmd_control_info_t flow_cmd_control = { FLOW_CB_TYPE_SET, FLOW_COMMAND_CONTINUE };

    SHR_FUNC_INIT_VARS(unit);
    DNX_ERR_RECOVERY_START(unit);

    SHR_INVOKE_VERIFY_DNXC(dnx_flow_create_verify(unit, flow_handle_info, initiator_info, special_fields));

    if (!_SHR_IS_FLAG_SET(flow_handle_info->flags, BCM_FLOW_HANDLE_INFO_REPLACE))
    {
        SHR_IF_ERR_EXIT(dnx_flow_initiator_info_create
                        (unit, flow_handle_info, &flow_cmd_control, initiator_info, special_fields));
    }
    else
    {
        SHR_IF_ERR_EXIT(dnx_flow_initiator_info_replace
                        (unit, flow_handle_info, &flow_cmd_control, initiator_info, special_fields));
    }

exit:
    DNX_ERR_RECOVERY_END(unit);
    SHR_FUNC_EXIT;
}

/**
* \brief
* Destory an initiator flow lif  - in order to be traffic safe first delete the LIF entry, once done remove all LIF 
* related profiles. 
*
* \param [in]  unit - Relevant unit.
* \param [in,out] flow_handle_info - pointer to a struct containing the relevant general flow attributes \n
*
* \return
*   \retval Zero in case of NO ERROR.
*   \retval Negative in case of an error.
*/
shr_error_e
bcm_dnx_flow_initiator_info_destroy(
    int unit,
    bcm_flow_handle_info_t * flow_handle_info)
{
    uint32 lif_flags;
    dnx_algo_gpm_gport_hw_resources_t gport_hw_resources;
    int field = 0;
    uint32 temp_common_bitmap;
    uint32 entry_handle_id;
    bcm_flow_initiator_info_t initiator_info;
    dnx_flow_app_config_t *flow_app_info = dnx_flow_objects[flow_handle_info->flow_handle];
    dnx_flow_cmd_control_info_t flow_cmd_control = { FLOW_CB_TYPE_DELETE, FLOW_COMMAND_CONTINUE };
    int is_virtual;

    SHR_FUNC_INIT_VARS(unit);
    DBAL_FUNC_INIT_VARS(unit);
    DNX_ERR_RECOVERY_START(unit);

    sal_memset(&initiator_info, 0, sizeof(bcm_flow_initiator_info_t));
    SHR_INVOKE_VERIFY_DNXC(dnx_flow_delete_verify(unit, FLOW_APP_TYPE_INIT, flow_handle_info, NULL));

    DNX_FLOW_LOGGER_PRINT(unit, flow_handle_info, flow_app_info, &flow_cmd_control, NULL, NULL);

    is_virtual = FLOW_GPORT_IS_VIRTUAL(flow_handle_info->flow_id);

    SHR_IF_ERR_EXIT(DBAL_HANDLE_ALLOC
                    (unit, FLOW_LIF_DBAL_TABLE_GET(dnx_flow_objects[flow_handle_info->flow_handle]), &entry_handle_id));

    /** Get local Out-LIF or virtual gport info from GPort hw resources (and global Out-LIF) */
    lif_flags = (is_virtual) ? DNX_ALGO_GPM_GPORT_HW_RESOURCES_EGRESS_LOCAL_LIF_OR_VIRTUAL :
        (DNX_ALGO_GPM_GPORT_HW_RESOURCES_LOCAL_LIF_EGRESS | DNX_ALGO_GPM_GPORT_HW_RESOURCES_GLOBAL_LIF_EGRESS);
    SHR_IF_ERR_EXIT(dnx_algo_gpm_gport_to_hw_resources
                    (unit, flow_handle_info->flow_id, lif_flags, &gport_hw_resources));

    /** Retrieve the entry into the handler and free the allocation */

    SHR_IF_ERR_EXIT(dnx_flow_lif_mgmt_initiator_info_get
                    (unit, flow_app_info, flow_handle_info, &gport_hw_resources, entry_handle_id));

    /** Remove flow data from EEDB outlif table or from an ESEM table */
    SHR_IF_ERR_EXIT(dnx_flow_lif_mgmt_initiator_info_clear(unit, &gport_hw_resources, flow_handle_info, flow_app_info));

    /** Iterate on all common fields. If a delete callback exists - call it (usually it is used to release profiles) */
    temp_common_bitmap = flow_app_info->valid_common_fields_bitmap;
    while (temp_common_bitmap != 0)
    {
        if ((temp_common_bitmap & 0x1) && (dnx_flow_init_common_fields[field]->field_cbs.field_delete_cb))
        {
            SHR_IF_ERR_EXIT_EXCEPT_IF(dnx_flow_init_common_fields[field]->
                                      field_cbs.field_delete_cb(unit, entry_handle_id, FLOW_APP_TYPE_INIT,
                                                                flow_handle_info, &gport_hw_resources, flow_app_info,
                                                                &initiator_info), _SHR_E_NOT_FOUND);
        }
        temp_common_bitmap = (temp_common_bitmap >> 1);
        field++;
    }

    SHR_IF_ERR_EXIT(dnx_flow_special_fields_destroy
                    (unit, entry_handle_id, flow_handle_info, flow_app_info, &gport_hw_resources));

    SHR_IF_ERR_EXIT(dnx_flow_initiator_dedicated_logic_destroy
                    (unit, flow_app_info, flow_handle_info, entry_handle_id, &gport_hw_resources));

    /** If a specific application callback is defined - call it */
    if (flow_app_info->app_specific_operations_cb)
    {
        SHR_IF_ERR_EXIT(flow_app_info->app_specific_operations_cb
                        (unit, entry_handle_id, FLOW_ENTRY_HANDLE_ID_INVALID, flow_handle_info, &flow_cmd_control,
                         &gport_hw_resources, &initiator_info, NULL, NULL));
        if (flow_cmd_control.flow_command != FLOW_COMMAND_CONTINUE)
        {
            SHR_EXIT();
        }
    }

    /** releasing SW info */
    SHR_IF_ERR_EXIT(dnx_flow_lif_mgmt_valid_fields_clear(unit, &gport_hw_resources, flow_handle_info, flow_app_info));

    SHR_IF_ERR_EXIT(dnx_flow_gport_flow_id_free(unit, flow_app_info, flow_handle_info, &gport_hw_resources));

exit:
    DNX_ERR_RECOVERY_END(unit);
    DBAL_FUNC_FREE_VARS;
    SHR_FUNC_EXIT;
}

/**
* \brief
* Get an initiator flow lif.
*
* \param [in]  unit - Relevant unit.
* \param [in,out] flow_handle_info - pointer to a struct containing the relevant general flow attributes \n
* \param [in,out] initiator_info - pointer to a struct containing the relevant initiator flow attributes \n
* \param [in,out] special_fields - list of fields which are not common (not part of the terminator flow struct) \n
*
* \return
*   \retval Zero in case of NO ERROR.
*   \retval Negative in case of an error.
*/
shr_error_e
bcm_dnx_flow_initiator_info_get(
    int unit,
    bcm_flow_handle_info_t * flow_handle_info,
    bcm_flow_initiator_info_t * initiator_info,
    bcm_flow_special_fields_t * special_fields)
{
    dnx_flow_cmd_control_info_t flow_cmd_control = { FLOW_CB_TYPE_GET, FLOW_COMMAND_CONTINUE };

    SHR_FUNC_INIT_VARS(unit);
    DNX_ERR_RECOVERY_NOT_NEEDED(unit);

    SHR_INVOKE_VERIFY_DNXC(dnx_flow_get_verify
                           (unit, FLOW_APP_TYPE_INIT, flow_handle_info, initiator_info, special_fields));

    SHR_IF_ERR_EXIT(dnx_flow_initiator_info_get
                    (unit, flow_handle_info, &flow_cmd_control, initiator_info, special_fields));

exit:
    SHR_FUNC_EXIT;
}

/**
* \brief
* Traverse all entries related to the lif application and run a callback function provided by the user.
*
* \param [in]  unit - Relevant unit.
* \param [in] flow_handle_info - flow_handle_info
* \param [in] cb - user callback
* \param [in] user_data - user data
*
* \return
*   \retval Zero in case of NO ERROR.
*   \retval Negative in case of an error.
*/
shr_error_e
bcm_dnx_flow_initiator_info_traverse(
    int unit,
    bcm_flow_handle_info_t * flow_handle_info,
    bcm_flow_initiator_info_traverse_cb cb,
    void *user_data)
{
    dnx_flow_traverse_operation_e traverse_operation = _SHR_IS_FLAG_SET(flow_handle_info->flags,
                                                                        BCM_FLOW_HANDLE_TRAVERSE_DELETE_ALL) ?
        FLOW_TRAVERSE_OPERATION_DELETE : FLOW_TRAVERSE_OPERATION_USE_CALLBACK;

    SHR_FUNC_INIT_VARS(unit);
    DNX_ERR_RECOVERY_NOT_NEEDED(unit);

    /** Verification of input parameters */
    SHR_INVOKE_VERIFY_DNXC(dnx_flow_traverse_verify(unit, flow_handle_info, (void *) &cb, user_data));

    if (traverse_operation == FLOW_TRAVERSE_OPERATION_DELETE)
    {
    /** removing  BCM_FLOW_HANDLE_TRAVERSE_DELETE_ALL flag since traverse will call entry destroy and this flag is
     *  illegal there */
        flow_handle_info->flags = 0;
    }
    /*
     * Traverse all initiator applications 
     */
    if (flow_handle_info->flow_handle == BCM_FLOW_HANDLE_TRAVERSE_ALL)
    {
        int num_apps = dnx_flow_number_of_apps();
        int app_idx;
        bcm_flow_handle_info_t traverse_flow_handle_info = { 0 };

        for (app_idx = FLOW_APP_FIRST_IDX; app_idx < num_apps; app_idx++)
        {
            if (FLOW_APP_IS_NOT_VALID(dnx_flow_objects[app_idx]))
            {
                continue;
            }
            if (dnx_flow_objects[app_idx]->flow_app_type == FLOW_APP_TYPE_INIT)
            {
                traverse_flow_handle_info.flow_handle = app_idx;
                SHR_IF_ERR_EXIT(dnx_flow_initiator_info_traverse
                                (unit, &traverse_flow_handle_info, cb, user_data, traverse_operation));
            }
        }
    }
    else
    {
        SHR_IF_ERR_EXIT(dnx_flow_initiator_info_traverse(unit, flow_handle_info, cb, user_data, traverse_operation));
    }

    if (traverse_operation == FLOW_TRAVERSE_OPERATION_DELETE)
    {
    /** returning the flag to original value */
        flow_handle_info->flags |= BCM_FLOW_HANDLE_TRAVERSE_DELETE_ALL;
    }

exit:
    SHR_FUNC_EXIT;
}

/**
* \brief
* Add match entry
*
* \param [in] unit - Relevant unit.
* \param [in] flow_handle_info - pointer to a struct containing the relevant general flow attributes \n
* \param [in] key_special_fields - list of key fields \n
*
* \return
*   \retval Zero in case of NO ERROR.
*   \retval Negative in case of an error.
*/
shr_error_e
bcm_dnx_flow_match_info_add(
    int unit,
    bcm_flow_handle_info_t * flow_handle_info,
    bcm_flow_special_fields_t * key_special_fields)
{
    dnx_flow_cmd_control_info_t flow_cmd_control = { FLOW_CB_TYPE_SET, FLOW_COMMAND_CONTINUE };

    SHR_FUNC_INIT_VARS(unit);
    DNX_ERR_RECOVERY_START(unit);

    SHR_INVOKE_VERIFY_DNXC(dnx_flow_create_verify(unit, flow_handle_info, NULL, key_special_fields));

    SHR_IF_ERR_EXIT(dnx_flow_match_info_add(unit, flow_handle_info, &flow_cmd_control, key_special_fields));

exit:
    DNX_ERR_RECOVERY_END(unit);
    SHR_FUNC_EXIT;
}

/**
* \brief
* Delete match entry
*
* \param [in] unit - Relevant unit.
* \param [in] flow_handle_info - pointer to a struct containing the relevant general flow attributes \n
* \param [in] key_special_fields - list of key fields \n
*
* \return
*   \retval Zero in case of NO ERROR.
*   \retval Negative in case of an error.
*/
shr_error_e
bcm_dnx_flow_match_info_delete(
    int unit,
    bcm_flow_handle_info_t * flow_handle_info,
    bcm_flow_special_fields_t * key_special_fields)
{
    dnx_flow_cmd_control_info_t flow_cmd_control = { FLOW_CB_TYPE_DELETE, FLOW_COMMAND_CONTINUE };

    SHR_FUNC_INIT_VARS(unit);
    DNX_ERR_RECOVERY_START(unit);

    SHR_INVOKE_VERIFY_DNXC(dnx_flow_delete_verify
                           (unit, dnx_flow_objects[flow_handle_info->flow_handle]->flow_app_type, flow_handle_info,
                            key_special_fields));

    SHR_IF_ERR_EXIT(dnx_flow_match_info_delete(unit, flow_handle_info, &flow_cmd_control, key_special_fields));

exit:
    DNX_ERR_RECOVERY_END(unit);
    SHR_FUNC_EXIT;
}

/**
* \brief
* Get match entry
* Note: Get is currently done only by the keys (to retreive the in-lif)
*
* \param [in] unit - Relevant unit.
* \param [in,out] flow_handle_info - pointer to a struct containing the relevant general flow attributes \n
* \param [in] key_special_fields - list of key fields \n
*
* \return
*   \retval Zero in case of NO ERROR.
*   \retval Negative in case of an error.
*/
shr_error_e
bcm_dnx_flow_match_info_get(
    int unit,
    bcm_flow_handle_info_t * flow_handle_info,
    bcm_flow_special_fields_t * key_special_fields)
{
    dnx_flow_cmd_control_info_t flow_cmd_control = { FLOW_CB_TYPE_GET, FLOW_COMMAND_CONTINUE };
    SHR_FUNC_INIT_VARS(unit);
    DNX_ERR_RECOVERY_START(unit);

    SHR_INVOKE_VERIFY_DNXC(dnx_flow_get_verify(unit, dnx_flow_objects[flow_handle_info->flow_handle]->flow_app_type,
                                               flow_handle_info, NULL, key_special_fields));

    SHR_IF_ERR_EXIT(dnx_flow_match_info_get(unit, flow_handle_info, &flow_cmd_control, key_special_fields));

exit:
    DNX_ERR_RECOVERY_END(unit);
    SHR_FUNC_EXIT;
}

/**
 * \brief
 * Traverse all entries on match table
 *
 * \param [in] unit - Relevant unit.
 * \param [in] flow_handle_info - flow_handle_info
 * \param [in] cb - cb function to be called \n
 * \param [out] user_data - Additional user data \n
 *
 * \return
 *   \retval Zero in case of NO ERROR.
 *   \retval Negative in case of an error.
 */
shr_error_e
bcm_dnx_flow_match_info_traverse(
    int unit,
    bcm_flow_handle_info_t * flow_handle_info,
    bcm_flow_match_info_traverse_cb cb,
    void *user_data)
{
    dnx_flow_traverse_operation_e traverse_operation = FLOW_TRAVERSE_OPERATION_USE_CALLBACK;

    SHR_FUNC_INIT_VARS(unit);
    DNX_ERR_RECOVERY_NOT_NEEDED(unit);

    /** Verification of input parameters */
    SHR_INVOKE_VERIFY_DNXC(dnx_flow_traverse_verify(unit, flow_handle_info, (void *) &cb, user_data));

    if (_SHR_IS_FLAG_SET(flow_handle_info->flags, BCM_FLOW_HANDLE_TRAVERSE_DELETE_ALL))
    {
        flow_handle_info->flags = 0;
        traverse_operation = FLOW_TRAVERSE_OPERATION_DELETE;
    }
    /*
     * Traverse all match applications 
     */
    if (flow_handle_info->flow_handle == BCM_FLOW_HANDLE_TRAVERSE_ALL)
    {
        int num_apps = dnx_flow_number_of_apps();
        int app_idx;
        bcm_flow_handle_info_t traverse_flow_handle_info = { 0 };

        for (app_idx = FLOW_APP_FIRST_IDX; app_idx < num_apps; app_idx++)
        {
            if (FLOW_APP_IS_NOT_VALID(dnx_flow_objects[app_idx]))
            {
                continue;
            }
            if (FLOW_APP_IS_MATCH(dnx_flow_objects[app_idx]->flow_app_type))
            {
                traverse_flow_handle_info.flow_handle = app_idx;
                SHR_IF_ERR_EXIT(dnx_flow_match_info_traverse
                                (unit, &traverse_flow_handle_info, cb, user_data, traverse_operation));
            }
        }
    }
    else
    {
        SHR_IF_ERR_EXIT(dnx_flow_match_info_traverse(unit, flow_handle_info, cb, user_data, traverse_operation));
    }

exit:
    SHR_FUNC_EXIT;
}

/**
 * \brief
 * Get field id according to field name
 *
 * \param [in] unit - Relevant unit.
 * \param [in] flow_handle - handle which relates to the relevant FLOW application\n
 * \param [in] field_name - the requested field name string \n
 * \param [out] field_id - the relevant field id that is related to this field string\n
 *
 * \return
 *   \retval Zero in case of NO ERROR.
 *   \retval Negative in case of an error.
 */
shr_error_e
bcm_dnx_flow_logical_field_id_get(
    int unit,
    bcm_flow_handle_t flow_handle,
    const char *field_name,
    bcm_flow_field_id_t * field_id)
{
    dnx_flow_app_config_t *flow_app_info;
    int iter;
    int nof_app_special_fields;

    SHR_FUNC_INIT_VARS(unit);
    DNX_ERR_RECOVERY_START(unit);

    DNX_FLOW_HANDLE_VALIDATE(flow_handle);

    flow_app_info = dnx_flow_objects[flow_handle];

    nof_app_special_fields = sizeof(flow_app_info->special_fields) / sizeof(flow_app_info->special_fields[0]);

    /** running over all the special fields related to the application and look for corresponding field */
    for (iter = 0; iter < nof_app_special_fields; iter++)
    {
        if (sal_strncmp(flow_special_fields_db[flow_app_info->special_fields[iter]].name, field_name, FLOW_STR_MAX)
            == 0)
        {
            *field_id = flow_app_info->special_fields[iter];
            SHR_SET_CURRENT_ERR(_SHR_E_NONE);
            SHR_EXIT();
        }
    }

    SHR_ERR_EXIT(_SHR_E_NOT_FOUND, "Field %s is not supported by the application %s \n", field_name,
                 flow_app_info->app_name);

exit:
    DNX_ERR_RECOVERY_END(unit);
    SHR_FUNC_EXIT;
}

/* Set flow handle matching application property according to property enum */
shr_error_e
bcm_dnx_flow_handle_control_set(
    int unit,
    bcm_flow_handle_t flow_handle,
    bcm_flow_handle_control_t type,
    uint32 value)
{
    SHR_FUNC_INIT_VARS(unit);
    DNX_ERR_RECOVERY_START(unit);

    /** Verification of input parameters */
    SHR_INVOKE_VERIFY_DNXC(dnx_flow_handle_control_verify(unit, flow_handle, type, value));

    switch (type)
    {
        case bcmFlowHandleControlVerifyEnable:
            SHR_IF_ERR_EXIT(flow_db.flow_application_info.is_verify_disabled.set(unit, flow_handle, !value));
            break;
        case bcmFlowHandleControlErrorRecoveryEnable:
            SHR_IF_ERR_EXIT(flow_db.flow_application_info.is_error_recovery_disabled.set(unit, flow_handle, !value));
            break;
        default:
            SHR_ERR_EXIT(_SHR_E_PARAM, "Unsupported type %d", type);
    }

exit:
    DNX_ERR_RECOVERY_END(unit);
    SHR_FUNC_EXIT;

}

/* Get flow handle matching application property according to property enum */
shr_error_e
bcm_dnx_flow_handle_control_get(
    int unit,
    bcm_flow_handle_t flow_handle,
    bcm_flow_handle_control_t type,
    uint32 *value)
{
    uint8 val = 0;

    SHR_FUNC_INIT_VARS(unit);
    DNX_ERR_RECOVERY_START(unit);

    *value = 0;

    /** Verification of input parameters */
    SHR_INVOKE_VERIFY_DNXC(dnx_flow_handle_control_verify(unit, flow_handle, type, *value));

    switch (type)
    {
        case bcmFlowHandleControlVerifyEnable:
            SHR_IF_ERR_EXIT(flow_db.flow_application_info.is_verify_disabled.get(unit, flow_handle, &val));
            break;
        case bcmFlowHandleControlErrorRecoveryEnable:
            SHR_IF_ERR_EXIT(flow_db.flow_application_info.is_error_recovery_disabled.get(unit, flow_handle, &val));
            break;
        default:
            SHR_ERR_EXIT(_SHR_E_PARAM, "Unsupported type %d", type);
    }
    *value = !val;

exit:
    DNX_ERR_RECOVERY_END(unit);
    SHR_FUNC_EXIT;
}

/* Initialize bcm_flow_initiator_info_t struct*/
void
bcm_dnx_flow_initiator_info_t_init(
    bcm_flow_initiator_info_t * info)
{
    if (info != NULL)
    {
        sal_memset(info, 0, sizeof(bcm_flow_initiator_info_t));
    }
    return;
}

/* Initialize bcm_flow_terminator_info_t struct*/
void
bcm_dnx_flow_terminator_info_t_init(
    bcm_flow_terminator_info_t * info)
{
    if (info != NULL)
    {
        sal_memset(info, 0, sizeof(bcm_flow_terminator_info_t));
    }
    return;
}

/* Initialize bcm_flow_dest_info_t struct*/
void
bcm_dnx_flow_dest_info_t_init(
    bcm_flow_dest_info_t * info)
{
    if (info != NULL)
    {
        sal_memset(info, 0, sizeof(bcm_flow_dest_info_t));
    }
    return;
}

/* Initialize bcm_flow_l3_ingress_info_t struct*/
void
bcm_dnx_flow_l3_ingress_info_t_init(
    bcm_flow_l3_ingress_info_t * info)
{
    if (info != NULL)
    {
        sal_memset(info, 0, sizeof(bcm_flow_l3_ingress_info_t));
    }
    return;
}
/**********************************************************************************************
*                                        APIs - END                                           *
**********************************************************************************************/
