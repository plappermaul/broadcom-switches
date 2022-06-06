#ifdef BSL_LOG_MODULE
#error "BSL_LOG_MODULE redefined"
#endif
#define BSL_LOG_MODULE BSL_LS_BCMDNX_FLOW

#include <shared/shrextend/shrextend_debug.h>
#include <soc/dnx/dnx_data/auto_generated/dnx_data_internal_flow.h>
#include "flow_def.h"

/*
 * Adding new applications
 * =======================
 * 1) Create a new file in the current (flow) directory - flow_xxx.c (where xxx stands for the application name). Add the new file to FILES.dnx
 * 2) Implement the necessary structure (dnx_flow_app_config_t) with the application's info, as defined in flow_def.h
 *    This structure will hold all of the relevant info for the application:
 *    - application name- must end with TERMINATOR or INITIATOR or MATCH
 *    - application related table
 *    - flow app type - termination/initiator/match
 *    - common application configuration - configuration which is common to some applications, such as verify enable, error_recovery enable
 *    - common fields bitmap - list of fields which are common to most applications, and have a static callbacks for set,get,delete,replace in flow_fields.c.
 *                             All of the common fields are related to a specific enabler, which is reflected in this bitmap
 *    - special fields - list of fields which are unique per application, and are being set/get directly from the flow table
 *    - verify cb - generic validations are done (and can be disabled per application) for all applications. In case unique validation is also required - define this cb
 *    - result type pick cb - result type is chosen by an internal algorithm which considers all of the user's set fields. In case another algorithm is required - define this cb
 * 3) Add the application to the list of applications in this file (below)
 * 4) In order to use the new defined application - call bcm_flow_handle_get() with the new application's name
 * 5) Examples can be found in flow.tcl, gtp tests and flow ctests
 * 6) In case of a new application (not ported) - add it to cleanup in dnx_flow_lib.tcl (in order for it to be cleared and iterated automatically when calling DnxFlowConfigReset etc
 *
 * Adding new common fields
 * ========================
 * 1) A common field must be a field which is shared between a couple of applications (at least)
 * 2) Each common field has corresponding enablers (for terminator applications and initiator applications)
 * 3) In case a new enabler is required - add it in flow.h. Format is BCM_FLOW_TERMINATOR_ELEMENT_*_VALID, BCM_FLOW_INITIATOR_ELEMENT_*_VALID
 *    where * stands for the field name.
 *    In case this field is relevant to only one of {initiator, terminator}, no need to add the other enabler
 * 4) Add the field in flow_fields.c:
 *    Add the field descriptor (dnx_flow_common_field_desc_t) and fill it
 *    - Field name
 *    - Terminator field enabler (set to FLOW_ENABLER_INVALID if not relevant)
 *    - Initiator field enabler (set to FLOW_ENABLER_INVALID if not relevant)
 *    - Each field must have set,get,print,desc cbs defined
 *    - is profile - profile fields are treated differently, and must also have their replace,delete cbs defined. This is also used for diagnostics and testing
 * 5) Add the new field descriptor in this file below in the common fields list
 * 6) Add the new field to the dnx_flow_term_common_fields and/or dnx_flow_init_common_fields lists (whichever is relevant for it)
 *    - IMPORTANT - On those lists - the new field must be placed according to its enabler (i.e - if it's enabler is 1 - it must be first. If it's 1 << 1 - it must be second...)
 * 7) In order to use this new field - it must be supported by the relevant application (in the common fields bitmap)
 */

/**
 * \brief
 * Strings mapping array for the flow application type enum
 */
static const char *flow_app_type_string[FLOW_APP_TYPE_NOF_TYPES] = {
    "TERMINATOR",
    "INITIATOR",
    "MATCH",
};

/** 
 *  \brief Strings mapping array for the flow init application indications
 */
static const char *flow_app_init_indications_string[FLOW_NOF_APP_INIT_IND] = {
    "SRC_ADDR_PROFILE_IND"
};

/**
 * \brief String for returning invalid string
 */
const char *flow_unknown_string = "unknown";

/** 
 *  \brief Strings mapping array for the flow term application indications
 */
static const char *flow_app_term_indications_string[FLOW_NOF_APP_TERM_IND] = {
    "QOS_PHB_DEFAULT_PIPE_IND",
    "IGNORE_L2_LEARN_INFO_SET_IND"
};

/** 
 *  \brief Strings mapping array for the flow match application indications
 */
static const char *flow_app_match_indications_string[FLOW_NOF_APP_MATCH_IND] = {
    "DUMMY_MATCH_IND"
};

/**
 * \brief
 * Strings mapping array for the flow handler flags 
 * The order of the strings should reflect the assigned bits for 
 * the flags. 
 */
static const char *flow_app_valid_flags_string[] = {
    "BCM_FLOW_HANDLE_INFO_WITH_ID",
    "BCM_FLOW_HANDLE_INFO_REPLACE",
    "BCM_FLOW_HANDLE_INFO_SYMMETRIC_ALLOC",
    "BCM_FLOW_HANDLE_INFO_VIRTUAL"
};

/******************************************************************************************************
 *                                              APPLICATIONS
 */
extern const dnx_flow_app_config_t dnx_flow_app_gtp_terminator;
extern const dnx_flow_app_config_t dnx_flow_app_gtp_initiator;
extern const dnx_flow_app_config_t dnx_flow_app_gtp_match;
extern const dnx_flow_app_config_t dnx_flow_app_mpls_tunnel_initiator;
extern const dnx_flow_app_config_t dnx_flow_app_ipv4_tunnel_initiator;
extern const dnx_flow_app_config_t dnx_flow_app_ipv4_tunnel_terminator;
extern const dnx_flow_app_config_t dnx_flow_app_ipv4_tunnel_match_em_p2p;
extern const dnx_flow_app_config_t dnx_flow_app_ipv4_tunnel_match_tcam_p2p;
extern const dnx_flow_app_config_t dnx_flow_app_vlan_port_ll_terminator;
extern const dnx_flow_app_config_t dnx_flow_app_vlan_port_ll_initiator;
extern const dnx_flow_app_config_t dnx_flow_app_vlan_port_ll_match_in_pp_port;
extern const dnx_flow_app_config_t dnx_flow_app_vlan_port_ll_match_in_ac_s_vlan;
extern const dnx_flow_app_config_t dnx_flow_app_vlan_port_ll_match_in_ac_c_vlan;
extern const dnx_flow_app_config_t dnx_flow_app_vlan_port_ll_match_in_ac_s_c_vlan;
extern const dnx_flow_app_config_t dnx_flow_app_vlan_port_ll_match_in_ac_c_c_vlan;
extern const dnx_flow_app_config_t dnx_flow_app_vlan_port_ll_match_in_ac_s_s_vlan;
extern const dnx_flow_app_config_t dnx_flow_app_vlan_port_ll_match_in_ac_untagged;
extern const dnx_flow_app_config_t dnx_flow_app_vlan_port_ll_match_in_ac_tcam;

const dnx_flow_app_config_t *dnx_flow_objects[] = {
    NULL,
    &dnx_flow_app_gtp_terminator,
    &dnx_flow_app_gtp_initiator,
    &dnx_flow_app_gtp_match,
    &dnx_flow_app_mpls_tunnel_initiator,
    &dnx_flow_app_ipv4_tunnel_initiator,
    &dnx_flow_app_ipv4_tunnel_terminator,
    &dnx_flow_app_ipv4_tunnel_match_em_p2p,
    &dnx_flow_app_ipv4_tunnel_match_tcam_p2p,
    &dnx_flow_app_vlan_port_ll_terminator,
    &dnx_flow_app_vlan_port_ll_initiator,
    &dnx_flow_app_vlan_port_ll_match_in_pp_port,
    &dnx_flow_app_vlan_port_ll_match_in_ac_s_vlan,
    &dnx_flow_app_vlan_port_ll_match_in_ac_c_vlan,
    &dnx_flow_app_vlan_port_ll_match_in_ac_s_c_vlan,
    &dnx_flow_app_vlan_port_ll_match_in_ac_c_c_vlan,
    &dnx_flow_app_vlan_port_ll_match_in_ac_s_s_vlan,
    &dnx_flow_app_vlan_port_ll_match_in_ac_untagged,
    &dnx_flow_app_vlan_port_ll_match_in_ac_tcam
};

/******************************************************************************************************
 *                                              FIELDS
 */
/** all existing common fields should be extern to here in order touse the in the DB 
    Note: Any new supported field should be added to the diagnostics.
    see: diag_flow_app_init_entry_print_cb */
extern dnx_flow_common_field_desc_t
    dnx_flow_common_field_vsi,
    dnx_flow_common_field_vrf,
    dnx_flow_common_field_stat_id,
    dnx_flow_common_field_l2_ingress_info,
    dnx_flow_common_field_l3_ingress_info,
    dnx_flow_common_field_stat_pp_profile,
    dnx_flow_common_field_qos_map_id,
    dnx_flow_common_field_qos_ingress_model_info,
    dnx_flow_common_field_flow_service_type_info,
    dnx_flow_common_field_dest_info,
    dnx_flow_common_field_additional_data,
    dnx_flow_common_field_action_gport,
    dnx_flow_common_field_l3_intf_id,
    dnx_flow_common_field_encap_access, dnx_flow_common_field_qos_egress_model, dnx_flow_common_field_mtu_profile,
    dnx_flow_common_field_ttl, dnx_flow_common_field_dscp, dnx_flow_common_field_exp, dnx_flow_common_field_pkt_pri_cfi,
    dnx_flow_common_field_failover_id,
    dnx_flow_common_field_failover_state,
    dnx_flow_common_field_class_id,
    dnx_flow_common_field_field_class_id,
    dnx_flow_common_field_l2_learn_info, dnx_flow_common_field_learn_enable, dnx_flow_common_field_l2_egress_info;

/** Flow termination common field DB  */
dnx_flow_common_field_desc_t *dnx_flow_term_common_fields[] = {
    &dnx_flow_common_field_vrf,
    &dnx_flow_common_field_stat_id,
    &dnx_flow_common_field_l3_ingress_info,
    &dnx_flow_common_field_stat_pp_profile,
    &dnx_flow_common_field_qos_map_id,
    &dnx_flow_common_field_qos_ingress_model_info,
    &dnx_flow_common_field_flow_service_type_info,
    &dnx_flow_common_field_dest_info,
    &dnx_flow_common_field_additional_data,
    &dnx_flow_common_field_action_gport,
    &dnx_flow_common_field_vsi,
    &dnx_flow_common_field_l2_ingress_info,
    &dnx_flow_common_field_failover_id,
    &dnx_flow_common_field_failover_state,
    &dnx_flow_common_field_class_id,
    &dnx_flow_common_field_field_class_id,
    &dnx_flow_common_field_l2_learn_info,
    &dnx_flow_common_field_learn_enable,

};

/** Flow initiator common field DB  */
dnx_flow_common_field_desc_t *dnx_flow_init_common_fields[] = {
    &dnx_flow_common_field_stat_id,
    &dnx_flow_common_field_stat_pp_profile,
    &dnx_flow_common_field_qos_map_id,
    &dnx_flow_common_field_l3_intf_id,
    &dnx_flow_common_field_encap_access,
    &dnx_flow_common_field_qos_egress_model,
    &dnx_flow_common_field_action_gport,
    &dnx_flow_common_field_mtu_profile,
    &dnx_flow_common_field_dscp,
    &dnx_flow_common_field_ttl,
    &dnx_flow_common_field_exp,
    &dnx_flow_common_field_pkt_pri_cfi,
    &dnx_flow_common_field_l2_egress_info,
    &dnx_flow_common_field_failover_id,
    &dnx_flow_common_field_failover_state,

};

/******************************************************************************************************
 *                                              UTILS
 */

/**
 * \brief
 * Retrieve the flow handle's application name string
 */
const char *
dnx_flow_handle_to_string(
    int unit,
    bcm_flow_handle_t flow_handle_id)
{
    return dnx_flow_objects[flow_handle_id]->app_name;
}

/**
 * \brief
 * Retrieve the flow common field name string
 */
const char *
dnx_flow_common_field_enabler_to_string(
    int unit,
    dnx_flow_app_type_e flow_app_type,
    uint32 common_field_enabler)
{
    int common_field_index;
    const dnx_flow_common_field_desc_t *field_desc = NULL;
    uint8 rv;

    /** gets the common field's index */
    rv = dnx_flow_common_fields_index_by_enabler_get(unit, flow_app_type, common_field_enabler, &common_field_index);

    if (rv != _SHR_E_NONE)
    {
        return flow_unknown_string;
    }

    /** gets the common field's desc */
    rv = dnx_flow_common_fields_desc_by_app_type_get(unit, flow_app_type, common_field_index, &field_desc);

    if (rv != _SHR_E_NONE)
    {
        return flow_unknown_string;
    }

    /** return "UNKNOWN" in case or error or miss-match */
    if (field_desc == NULL)
    {
        return flow_unknown_string;
    }

    return field_desc->field_name;
}

/**
 * \brief
 * Retrieve the flow handle's application name string
 */
const char *
dnx_flow_app_indications_to_string(
    int unit,
    dnx_flow_app_type_e flow_app_type,
    uint32 app_inidication)
{
    switch (flow_app_type)
    {
        case FLOW_APP_TYPE_INIT:
            if (app_inidication >= FLOW_NOF_APP_INIT_IND)
            {
                return "INVALID - out of range indication";
            }
            return flow_app_init_indications_string[app_inidication];
            break;

        case FLOW_APP_TYPE_TERM:
            if (app_inidication >= FLOW_NOF_APP_TERM_IND)
            {
                return "INVALID - out of range indication";
            }
            return flow_app_term_indications_string[app_inidication];
            break;

        case FLOW_APP_TYPE_MATCH:
            if (app_inidication >= FLOW_NOF_APP_MATCH_IND)
            {
                return "INVALID - out of range indication";
            }
            return flow_app_match_indications_string[app_inidication];
            break;

        default:
            return "INVALID - no such app type";
    }
}

/**
 * \brief
 * Convert an application type to its name string
 */
const char *
flow_app_type_to_string(
    int unit,
    dnx_flow_app_type_e flow_app_type)
{
    if (flow_app_type < FLOW_APP_TYPE_NOF_TYPES)
    {
        return flow_app_type_string[flow_app_type];
    }
    return flow_unknown_string;
}

/**
 * \brief
 * Convert a FLOW handler flag to its name string
 */
const char *
flow_app_valid_flag_to_string(
    int unit,
    uint32 flow_handler_flag)
{
    int nof_valid_flags = sizeof(flow_app_valid_flags_string) / sizeof(flow_app_valid_flags_string[0]);
    if (flow_handler_flag < nof_valid_flags)
    {
        return flow_app_valid_flags_string[flow_handler_flag];
    }
    return flow_unknown_string;
}

const char *
dnx_flow_special_field_to_string(
    int unit,
    dbal_fields_e field_id)
{
    const dnx_data_flow_special_fields_name_mapping_t *name_mapping;

    name_mapping = dnx_data_flow.special_fields.name_mapping_get(unit, field_id);
    return name_mapping->field_name;
}

/**
 * \brief
 * Retrieve the flow application information for a given flow handle ID
 */
const dnx_flow_app_config_t *
dnx_flow_app_info_get(
    int unit,
    bcm_flow_handle_t flow_handle_id)
{
    return dnx_flow_objects[flow_handle_id];
}

int
dnx_flow_number_of_apps(
    void)
{
    return sizeof(dnx_flow_objects) / sizeof(dnx_flow_app_config_t *);
}

shr_error_e
dnx_flow_number_of_common_fields(
    int unit,
    dnx_flow_app_type_e flow_app_type,
    int *nof_fields)
{
    SHR_FUNC_INIT_VARS(unit);

    switch (flow_app_type)
    {
        case FLOW_APP_TYPE_TERM:
            (*nof_fields) = sizeof(dnx_flow_term_common_fields) / sizeof(dnx_flow_common_field_desc_t *);
            break;

        case FLOW_APP_TYPE_INIT:
            (*nof_fields) = sizeof(dnx_flow_init_common_fields) / sizeof(dnx_flow_common_field_desc_t *);
            break;

        case FLOW_APP_TYPE_MATCH:
            (*nof_fields) = 0;
            break;

        default:
            (*nof_fields) = 0;
            SHR_ERR_EXIT(_SHR_E_PARAM, "Unsupported flow_app_type, %d", flow_app_type);
            break;
    }
exit:
    SHR_FUNC_EXIT;
}

/**
 * \brief
 * Retrieve a pointer to a common fields descriptor, according 
 * to a supplied application type and field index to a common 
 * fields descriptors array.
 */
shr_error_e
dnx_flow_common_fields_desc_by_app_type_get(
    int unit,
    dnx_flow_app_type_e flow_app_type,
    int field_idx,
    const dnx_flow_common_field_desc_t ** common_field_desc)
{
    int nof_common_fields;
    SHR_FUNC_INIT_VARS(unit);

    /*
     * Validate the supplied application type and field index
     */
    if (flow_app_type >= FLOW_APP_TYPE_NOF_TYPES)
    {
        SHR_ERR_EXIT(_SHR_E_PARAM, "Error, FLOW application type Out-Of-Range - %d", flow_app_type);
    }

    SHR_IF_ERR_EXIT(dnx_flow_number_of_common_fields(unit, flow_app_type, &nof_common_fields));
    if (field_idx >= nof_common_fields)
    {
        SHR_ERR_EXIT(_SHR_E_PARAM, "Error, Field index (%d) larger the common fields array size (%d)", field_idx,
                     nof_common_fields);
    }

    /*
     * Retrieve a pointer to the common field descriptor according to the supplied 
     * application type and field index to a common fields descriptors array.
     */
    switch (flow_app_type)
    {
        case FLOW_APP_TYPE_TERM:
            *common_field_desc = dnx_flow_term_common_fields[field_idx];
            break;
        case FLOW_APP_TYPE_INIT:
            *common_field_desc = dnx_flow_init_common_fields[field_idx];
            break;
        case FLOW_APP_TYPE_MATCH:
        default:
            *common_field_desc = NULL;
            break;
    }

exit:
    SHR_FUNC_EXIT;
}

/**
 * \brief
 * Get the common fields index for term, init according to enabler
 */
shr_error_e
dnx_flow_common_fields_index_by_enabler_get(
    int unit,
    dnx_flow_app_type_e flow_app_type,
    int enabler,
    int *index)
{
    int nof_common_fields, idx, field_enabler;
    dnx_flow_common_field_desc_t **dnx_flow_common_fields;

    SHR_FUNC_INIT_VARS(unit);

    /*
     * Validate the supplied application type and field index
     */
    if (flow_app_type >= FLOW_APP_TYPE_NOF_TYPES)
    {
        SHR_ERR_EXIT(_SHR_E_PARAM, "Error, FLOW application type Out-Of-Range - %d", flow_app_type);
    }

    SHR_IF_ERR_EXIT(dnx_flow_number_of_common_fields(unit, flow_app_type, &nof_common_fields));

    /*
     * Retrieve a pointer to the common field descriptor according to the supplied
     * application type.
     */
    switch (flow_app_type)
    {
        case FLOW_APP_TYPE_TERM:
            dnx_flow_common_fields = dnx_flow_term_common_fields;
            break;
        case FLOW_APP_TYPE_INIT:
            dnx_flow_common_fields = dnx_flow_init_common_fields;
            break;
        case FLOW_APP_TYPE_MATCH:
        default:
            SHR_ERR_EXIT(_SHR_E_PARAM, "Only term, init applications have common fields");
            break;
    }

    for (idx = 0; idx < nof_common_fields; idx++)
    {
        field_enabler =
            (flow_app_type ==
             FLOW_APP_TYPE_TERM) ? dnx_flow_common_fields[idx]->
            term_field_enabler : dnx_flow_common_fields[idx]->init_field_enabler;
        if (field_enabler == enabler)
        {
            *index = idx;
            break;
        }
    }
    if (idx == nof_common_fields)
    {
        SHR_ERR_EXIT(_SHR_E_PARAM, "Couldn't find matching field for enabler %d", enabler);
    }

exit:
    SHR_FUNC_EXIT;
}
