#ifdef BSL_LOG_MODULE
#error "BSL_LOG_MODULE redefined"
#endif
#define BSL_LOG_MODULE BSL_LS_BCMDNX_FLOW

#include <bcm_int/dnx/flow/flow.h>
#include <bcm_int/dnx/algo/swstate/auto_generated/access/algo_qos_access.h>
#include <bcm/qos.h>
#include <bcm_int/dnx/qos/qos.h>
#include <bcm_int/dnx/lif/lif_table_mngr_lib.h>
#include <bcm_int/dnx/flow/flow_lif_mgmt.h>

static shr_error_e
dnx_flow_init_ttl_create(
    int unit,
    uint32 entry_handle_id,
    bcm_flow_initiator_info_t * initiator_info,
    bcm_flow_special_fields_t * special_fields)
{
    bcm_flow_special_field_t special_field_data;
    shr_error_e rv;

    SHR_FUNC_INIT_VARS(unit);

    if (_SHR_IS_FLAG_SET(initiator_info->valid_elements_set, BCM_FLOW_INITIATOR_ELEMENT_TTL_VALID))
    {
        if (initiator_info->egress_qos_model.egress_ttl == bcmQosEgressModelPipeMyNameSpace)
        {
            dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_TTL_MODE, INST_SINGLE,
                                         DBAL_ENUM_FVAL_TTL_MODE_PIPE);
        }
        else
        {
            dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_TTL_MODE, INST_SINGLE,
                                         DBAL_ENUM_FVAL_TTL_MODE_UNIFORM);
        }

        rv = dnx_flow_special_field_data_get(unit, special_fields, FLOW_S_F_MPLS_PHP, &special_field_data);
        SHR_IF_ERR_EXIT_EXCEPT_IF(rv, _SHR_E_NOT_FOUND);
        /** Allocate TTL profile, php does not need to allocate */
        if ((rv != _SHR_E_NONE) && (initiator_info->egress_qos_model.egress_ttl == bcmQosEgressModelPipeMyNameSpace))
        {
            uint8 is_first_ttl_profile_ref;
            uint32 ttl_pipe_profile;

            /** allocate TTL PIPE profile */
            SHR_IF_ERR_EXIT(algo_qos_db.qos_egress_ttl_pipe_profile.allocate_single
                            (unit, 0, (int *) &initiator_info->ttl, NULL, (int *) &ttl_pipe_profile,
                             &is_first_ttl_profile_ref));

            /** Write to HW if new TTL profile was allocated */
            if (is_first_ttl_profile_ref)
            {
                SHR_IF_ERR_EXIT(dnx_qos_egress_ttl_profile_hw_set(unit, ttl_pipe_profile, initiator_info->ttl));
            }
            dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_TTL_PIPE_PROFILE, INST_SINGLE,
                                         ttl_pipe_profile);
        }
    }

exit:
    SHR_FUNC_EXIT;
}

static shr_error_e
dnx_flow_init_ttl_destroy(
    int unit,
    bcm_flow_handle_info_t * flow_handle_info,
    uint32 entry_handle_id,
    dnx_algo_gpm_gport_hw_resources_t * gport_hw_resources)
{
    uint32 profile, ttl_mode;
    uint8 is_last_profile_ref = FALSE, is_set = FALSE;

    SHR_FUNC_INIT_VARS(unit);

    /*
     * A delete CB should be run only in case the field was set at create (resources were allocated).
     * Check only the necessary fields.
     */
    SHR_IF_ERR_EXIT(dnx_flow_lif_mgmt_field_is_set_in_hw
                    (unit, flow_handle_info, gport_hw_resources, DBAL_FIELD_TTL_MODE, &is_set));
    if (is_set)
    {
        SHR_IF_ERR_EXIT(dbal_entry_handle_value_field32_get
                        (unit, entry_handle_id, DBAL_FIELD_TTL_MODE, INST_SINGLE, &ttl_mode));

        if (ttl_mode == DBAL_ENUM_FVAL_TTL_MODE_PIPE)
        {
            uint8 is_field_valid = FALSE;

            /** Get TTL PIPE profile */
            SHR_IF_ERR_EXIT(dnx_lif_table_mngr_uint32_field_on_dbal_handle_get(unit, entry_handle_id,
                                                                               DBAL_FIELD_TTL_PIPE_PROFILE,
                                                                               &is_field_valid, &profile));
            if (is_field_valid)
            {
                /** Free old TTL profile template */
                SHR_IF_ERR_EXIT(algo_qos_db.
                                qos_egress_ttl_pipe_profile.free_single(unit, profile, &is_last_profile_ref));
            }
        }
        if (is_last_profile_ref)
        {
            SHR_IF_ERR_EXIT(dnx_qos_egress_ttl_profile_hw_clear(unit, profile));
        }
    }

exit:
    SHR_FUNC_EXIT;
}

static shr_error_e
dnx_flow_init_ttl_get(
    int unit,
    uint32 entry_handle_id,
    bcm_flow_initiator_info_t * initiator_info,
    uint8 *is_qos_valid)
{
    uint32 ttl_value = 0;
    uint32 ttl_mode;
    uint8 is_field_valid;

    SHR_FUNC_INIT_VARS(unit);

    SHR_IF_ERR_EXIT(dnx_lif_table_mngr_uint32_field_on_dbal_handle_get
                    (unit, entry_handle_id, DBAL_FIELD_TTL_MODE, &is_field_valid, &ttl_mode));
    if (is_field_valid == TRUE)
    {
        (*is_qos_valid) = TRUE;

        /** uniform is default, do nothing*/
        if (ttl_mode == DBAL_ENUM_FVAL_TTL_MODE_PIPE)
        {
            uint32 ttl_pipe_profile;
            int ref_count = 0;

            initiator_info->egress_qos_model.egress_ttl = bcmQosEgressModelPipeMyNameSpace;

            /** Get TTL PIPE profile */
            SHR_IF_ERR_EXIT(dbal_entry_handle_value_field32_get(unit, entry_handle_id, DBAL_FIELD_TTL_PIPE_PROFILE,
                                                                INST_SINGLE, &ttl_pipe_profile));
            /** get TTL value from template manager */
            SHR_IF_ERR_EXIT(algo_qos_db.qos_egress_ttl_pipe_profile.profile_data_get
                            (unit, ttl_pipe_profile, &ref_count, (void *) &ttl_value));

            initiator_info->valid_elements_set |= BCM_FLOW_INITIATOR_ELEMENT_TTL_VALID;
            initiator_info->ttl = ttl_value;
        }
        else
        {
            initiator_info->egress_qos_model.egress_ttl = bcmQosEgressModelUniform;
        }
    }

exit:
    SHR_FUNC_EXIT;
}

static shr_error_e
dnx_flow_init_ttl_replace(
    int unit,
    uint32 entry_handle_id,
    uint32 get_entry_handle_id,
    bcm_flow_initiator_info_t * initiator_info,
    bcm_flow_special_fields_t * special_fields,
    int *last_ttl_profile)
{
    uint32 ttl_mode = DBAL_ENUM_FVAL_TTL_MODE_UNIFORM;
    uint8 is_ttl_mode_field_valid = FALSE, is_ttl_profile_valid = FALSE, was_old_ttl_profile_set = FALSE;
    uint8 is_first_profile_ref = FALSE, is_last_profile_ref = FALSE;
    uint8 is_pipe_mode_old = FALSE;
    uint8 is_pipe_mode_new = TRUE;
    int old_profile = 0;
    int new_profile = 0;
    bcm_flow_special_field_t special_field_data;
    shr_error_e rv;

    SHR_FUNC_INIT_VARS(unit);

    SHR_IF_ERR_EXIT(dnx_lif_table_mngr_uint32_field_on_dbal_handle_get
                    (unit, get_entry_handle_id, DBAL_FIELD_TTL_MODE, &is_ttl_mode_field_valid, &ttl_mode));

    if (is_ttl_mode_field_valid == TRUE)
    {
        if (ttl_mode == DBAL_ENUM_FVAL_TTL_MODE_PIPE)
        {
            is_pipe_mode_old = TRUE;

            SHR_IF_ERR_EXIT(dnx_lif_table_mngr_uint32_field_on_dbal_handle_get(unit, get_entry_handle_id,
                                                                               DBAL_FIELD_TTL_PIPE_PROFILE,
                                                                               &is_ttl_profile_valid,
                                                                               (uint32 *) &old_profile));
            if (is_ttl_profile_valid)
            {
                was_old_ttl_profile_set = TRUE;
            }
        }
    }

    rv = dnx_flow_special_field_data_get(unit, special_fields, FLOW_S_F_MPLS_PHP, &special_field_data);
    SHR_IF_ERR_EXIT_EXCEPT_IF(rv, _SHR_E_NOT_FOUND);

    /**set ttl pipe profile*/
    if (_SHR_IS_FLAG_SET(initiator_info->valid_elements_set, BCM_FLOW_INITIATOR_ELEMENT_TTL_VALID) ||
        _SHR_IS_FLAG_SET(initiator_info->valid_elements_clear, BCM_FLOW_INITIATOR_ELEMENT_TTL_VALID))
    {
        /**set ttl model*/
        if (initiator_info->valid_elements_set & BCM_FLOW_INITIATOR_ELEMENT_QOS_EGRESS_MODEL_VALID)
        {
            if (initiator_info->egress_qos_model.egress_ttl == bcmQosEgressModelUniform)
            {
                ttl_mode = DBAL_ENUM_FVAL_TTL_MODE_UNIFORM;
                is_pipe_mode_new = FALSE;
            }
            else
            {
                ttl_mode = DBAL_ENUM_FVAL_TTL_MODE_PIPE;
            }
        }
        else
        {
            is_pipe_mode_new = is_pipe_mode_old;
        }

        if (_SHR_IS_FLAG_SET(initiator_info->valid_elements_clear, BCM_FLOW_INITIATOR_ELEMENT_TTL_VALID))
        {
            if (was_old_ttl_profile_set)
            {
                SHR_IF_ERR_EXIT(algo_qos_db.qos_egress_ttl_pipe_profile.free_single(unit, old_profile,
                                                                                    &is_last_profile_ref));
            }
            ttl_mode = DBAL_ENUM_FVAL_TTL_MODE_UNIFORM;
        }
        else    /* updating or 1st writing the ttl */
        {
            /** Allocate TTL template */
            if (((is_pipe_mode_old) == TRUE) && ((is_pipe_mode_new) == TRUE) && (was_old_ttl_profile_set == TRUE))
            {
                /** In this case the old and new TTL model is PIPE - exchange templates */

                /** Exchange TTL PIPE profile */
                SHR_IF_ERR_EXIT(algo_qos_db.qos_egress_ttl_pipe_profile.exchange
                                (unit, 0, (int *) &initiator_info->ttl, old_profile, NULL,
                                 &new_profile, &is_first_profile_ref, &is_last_profile_ref));
            }
            else if ((is_pipe_mode_new == TRUE)
                     && (_SHR_IS_FLAG_SET(initiator_info->valid_elements_set, BCM_FLOW_INITIATOR_ELEMENT_TTL_VALID)))
            {
                /** In this case the old model is UNIFORM and new TTL model is PIPE - allocate new profile */

                /** allocate TTL PIPE profile */
                SHR_IF_ERR_EXIT(algo_qos_db.qos_egress_ttl_pipe_profile.allocate_single
                                (unit, 0, (int *) &initiator_info->ttl, NULL, &new_profile, &is_first_profile_ref));
            }
            else if (((is_pipe_mode_old) == TRUE) && (was_old_ttl_profile_set == TRUE))
            {
                /** In this case the old model is PIPE and new TTL model is UNIFORM - remove old template */

                /** Free old TTL profile template */
                SHR_IF_ERR_EXIT(algo_qos_db.qos_egress_ttl_pipe_profile.free_single(unit, old_profile,
                                                                                    &is_last_profile_ref));
            }

            if ((rv != _SHR_E_NONE) &&
                (_SHR_IS_FLAG_SET(initiator_info->valid_elements_set, BCM_FLOW_INITIATOR_ELEMENT_TTL_VALID)))
            {
                dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_TTL_PIPE_PROFILE, INST_SINGLE,
                                             new_profile);
            }
        }
        dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_TTL_MODE, INST_SINGLE, ttl_mode);
    }
    else
    {
        if (is_ttl_mode_field_valid)
        {
            /** Set old TTL PIPE profile */
            if (was_old_ttl_profile_set)
            {
                dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_TTL_PIPE_PROFILE, INST_SINGLE,
                                             old_profile);
            }
            dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_TTL_MODE, INST_SINGLE, ttl_mode);
        }
    }

    /** Write to HW if new TTL profile was allocated */
    if (is_first_profile_ref)
    {
        SHR_IF_ERR_EXIT(dnx_qos_egress_ttl_profile_hw_set(unit, new_profile, initiator_info->ttl));
    }
    /** If last profile was deallocated remove the entry from HW */
    if (is_last_profile_ref)
    {
        (*last_ttl_profile) = old_profile;
    }

exit:
    SHR_FUNC_EXIT;
}

static shr_error_e
dnx_flow_qos_egress_app_params_create(
    int unit,
    uint32 entry_handle_id,
    bcm_flow_initiator_info_t * initiator_info,
    bcm_flow_special_fields_t * special_fields)
{
    uint8 field_idx;
    uint8 nwk_qos_idx = FALSE, nwk_pri_cfi_en = FALSE, nwk_qos_enabled = FALSE;
    uint8 nwk_qos_pri = 0, nwk_qos_cfi = 0;
    dbal_enum_value_field_encap_qos_model_e encap_qos_model;
    bcm_flow_special_field_t special_field_data = { 0 };
    shr_error_e rv;

    SHR_FUNC_INIT_VARS(unit);

    if (special_fields)
    {
        for (field_idx = 0; field_idx < special_fields->actual_nof_special_fields; field_idx++)
        {
            if (special_fields->special_fields[field_idx].field_id == FLOW_S_F_QOS_DSCP)
            {
                nwk_qos_idx = special_fields->special_fields[field_idx].shr_var_uint32;
                nwk_qos_enabled = TRUE;
                break;
            }
            else if (special_fields->special_fields[field_idx].field_id == FLOW_S_F_QOS_EXP)
            {
                rv = dnx_flow_special_field_data_get(unit, special_fields, FLOW_S_F_QOS_EXP_1, &special_field_data);
                SHR_IF_ERR_EXIT_EXCEPT_IF(rv, _SHR_E_NOT_FOUND);
                if (rv == _SHR_E_NONE)
                {
                    nwk_qos_idx = DNX_QOS_IDX_EXP_TO_NWK_QOS(special_fields->special_fields[field_idx].shr_var_uint32,
                                                             special_field_data.shr_var_uint32);
                }
                else
                {
                    nwk_qos_idx = special_fields->special_fields[field_idx].shr_var_uint32;
                }
                nwk_qos_enabled = TRUE;
                break;
            }
            else if (special_fields->special_fields[field_idx].field_id == FLOW_S_F_QOS_PRI)
            {
                nwk_qos_pri = special_fields->special_fields[field_idx].shr_var_uint32;
                nwk_pri_cfi_en = TRUE;
            }
            else if (special_fields->special_fields[field_idx].field_id == FLOW_S_F_QOS_CFI)
            {
                nwk_qos_cfi = special_fields->special_fields[field_idx].shr_var_uint32;
                nwk_pri_cfi_en = TRUE;
            }
        }
    }

    if (nwk_pri_cfi_en == TRUE)
    {
        nwk_qos_idx = DNX_QOS_IDX_PCP_DEI_TO_NWK_QOS(nwk_qos_pri, nwk_qos_cfi);
        nwk_qos_enabled = TRUE;
    }

    if (nwk_qos_enabled)
    {
        /** QOS PIPE Model */
        if ((initiator_info->egress_qos_model.egress_qos == bcmQosEgressModelPipeMyNameSpace) ||
            (initiator_info->egress_qos_model.egress_qos == bcmQosEgressModelPipeNextNameSpace))
        {
            uint32 nwk_qos_profile = 0;
            uint8 is_first_pipe_profile_ref;

            SHR_IF_ERR_EXIT(dnx_qos_egress_network_qos_pipe_profile_allocate
                            (unit, nwk_qos_idx, nwk_qos_idx, 0, (int *) &nwk_qos_profile, &is_first_pipe_profile_ref));

            if (is_first_pipe_profile_ref)
            {
                SHR_IF_ERR_EXIT(dnx_qos_egress_network_qos_pipe_profile_set
                                (unit, nwk_qos_profile, nwk_qos_idx, nwk_qos_idx, 0));
            }

            dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_NWK_QOS_IDX, INST_SINGLE, nwk_qos_profile);
        }
    }

    /** treat with the common field QOS_EGRESS_MODEL data */
    SHR_IF_ERR_EXIT(dnx_qos_egress_model_bcm_to_dbal(unit, &initiator_info->egress_qos_model, &encap_qos_model));
    dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_ENCAP_QOS_MODEL, INST_SINGLE, encap_qos_model);

exit:
    SHR_FUNC_EXIT;
}

static shr_error_e
dnx_flow_qos_egress_app_params_replace(
    int unit,
    uint32 entry_handle_id,
    uint32 get_entry_handle_id,
    bcm_flow_initiator_info_t * initiator_info,
    bcm_flow_special_fields_t * special_fields,
    int *last_network_qos_profile)
{
    uint8 is_field_valid = FALSE, was_nwk_qos_set = FALSE;
    uint8 is_first_profile_ref = FALSE, is_last_profile_ref = FALSE;
    uint8 field_idx;
    uint8 nwk_qos_idx = FALSE, nwk_qos_cleared = FALSE;
    uint8 nwk_pri_en = FALSE, nwk_cfi_en = FALSE, nwk_qos_pri = 0, nwk_qos_cfi = 0;
    uint8 nwk_qos_update = FALSE;
    uint8 is_pipe_mode_old = FALSE;
    uint8 is_pipe_mode_new = FALSE;
    int old_profile = 0;
    int new_profile = 0;
    dbal_enum_value_field_encap_qos_model_e encap_qos_model;
    bcm_flow_special_field_t special_field_data = { 0 };
    shr_error_e rv;

    SHR_FUNC_INIT_VARS(unit);

    if (special_fields != NULL)
    {
        for (field_idx = 0; field_idx < special_fields->actual_nof_special_fields; field_idx++)
        {
            if (special_fields->special_fields[field_idx].field_id == FLOW_S_F_QOS_DSCP)
            {
                if (special_fields->special_fields[field_idx].is_clear)
                {
                    nwk_qos_cleared = TRUE;
                }
                else
                {
                    nwk_qos_idx = special_fields->special_fields[field_idx].shr_var_uint32;
                    nwk_qos_update = TRUE;
                }
                break;
            }
            if (special_fields->special_fields[field_idx].field_id == FLOW_S_F_QOS_EXP)
            {
                if (special_fields->special_fields[field_idx].is_clear)
                {
                    nwk_qos_cleared = TRUE;
                }
                else
                {
                    rv = dnx_flow_special_field_data_get(unit, special_fields, FLOW_S_F_QOS_EXP_1, &special_field_data);
                    SHR_IF_ERR_EXIT_EXCEPT_IF(rv, _SHR_E_NOT_FOUND);
                    if (rv == _SHR_E_NONE)
                    {
                        nwk_qos_idx =
                            DNX_QOS_IDX_EXP_TO_NWK_QOS(special_fields->special_fields[field_idx].shr_var_uint32,
                                                       special_field_data.shr_var_uint32);
                    }
                    else
                    {
                        nwk_qos_idx = special_fields->special_fields[field_idx].shr_var_uint32;
                    }
                    nwk_qos_update = TRUE;
                }
                break;
            }
            else if (special_fields->special_fields[field_idx].field_id == FLOW_S_F_QOS_PRI)
            {
                if (special_fields->special_fields[field_idx].is_clear)
                {
                    nwk_qos_cleared = TRUE;
                    break;
                }
                else
                {
                    nwk_qos_pri = special_fields->special_fields[field_idx].shr_var_uint32;
                    nwk_pri_en = TRUE;
                }
            }
            else if (special_fields->special_fields[field_idx].field_id == FLOW_S_F_QOS_CFI)
            {
                if (special_fields->special_fields[field_idx].is_clear)
                {
                    nwk_qos_cleared = TRUE;
                    break;
                }
                else
                {
                    nwk_qos_cfi = special_fields->special_fields[field_idx].shr_var_uint32;
                    nwk_cfi_en = TRUE;
                }
            }
        }
    }

    if ((nwk_pri_en == TRUE) || (nwk_cfi_en == TRUE))
    {
        if (nwk_pri_en == FALSE)
        {
            /** pri value was not replaced, take the old value */
            SHR_IF_ERR_EXIT(dnx_flow_special_field_data_get
                            (unit, special_fields, FLOW_S_F_QOS_PRI, &special_field_data));
            nwk_qos_pri = special_field_data.shr_var_uint32;
        }
        if (nwk_cfi_en == FALSE)
        {
            /** cfi value was not replaced, take the old value */
            SHR_IF_ERR_EXIT(dnx_flow_special_field_data_get
                            (unit, special_fields, FLOW_S_F_QOS_CFI, &special_field_data));
            nwk_qos_cfi = special_field_data.shr_var_uint32;
        }

        nwk_qos_idx = DNX_QOS_IDX_PCP_DEI_TO_NWK_QOS(nwk_qos_pri, nwk_qos_cfi);
        nwk_qos_update = TRUE;
    }

    SHR_IF_ERR_EXIT(dnx_lif_table_mngr_uint32_field_on_dbal_handle_get(unit, get_entry_handle_id,
                                                                       DBAL_FIELD_ENCAP_QOS_MODEL,
                                                                       &is_field_valid, &encap_qos_model));

    if (is_field_valid == TRUE)
    {
        if ((encap_qos_model == DBAL_ENUM_FVAL_ENCAP_QOS_MODEL_PIPE_NEXT_SPACE) ||
            (encap_qos_model == DBAL_ENUM_FVAL_ENCAP_QOS_MODEL_PIPE_MY_SPACE) ||
            (encap_qos_model == DBAL_ENUM_FVAL_ENCAP_QOS_MODEL_PIPE_ECN))
        {
            uint32 nwk_qos_idx = 0;
            uint8 is_nwk_qos_index_valid = FALSE;
            /** Get QOS pipe index */
            is_pipe_mode_old = TRUE;
            SHR_IF_ERR_EXIT(dnx_lif_table_mngr_uint32_field_on_dbal_handle_get(unit, get_entry_handle_id,
                                                                               DBAL_FIELD_NWK_QOS_IDX,
                                                                               &is_nwk_qos_index_valid, &nwk_qos_idx));
            if (is_nwk_qos_index_valid)
            {
                old_profile = nwk_qos_idx;
                was_nwk_qos_set = TRUE;
            }
        }
    }

    if (nwk_qos_cleared)
    {
        if (was_nwk_qos_set)
        {
            /** Free old QOS profile template */
            SHR_IF_ERR_EXIT(dnx_qos_egress_network_qos_pipe_profile_free
                            (unit, (int) old_profile, &is_last_profile_ref));
            was_nwk_qos_set = FALSE;
        }
    }
    else
    {
        if (initiator_info->valid_elements_set & BCM_FLOW_INITIATOR_ELEMENT_QOS_EGRESS_MODEL_VALID)
        {
            if ((initiator_info->egress_qos_model.egress_qos == bcmQosEgressModelPipeMyNameSpace) ||
                (initiator_info->egress_qos_model.egress_qos == bcmQosEgressModelPipeNextNameSpace))
            {
                is_pipe_mode_new = TRUE;
            }
            else
            {
                is_pipe_mode_new = FALSE;
            }
        }
        else
        {
            is_pipe_mode_new = is_pipe_mode_old;
        }

        if (nwk_qos_update)
        {
            /** Exchange QOS idx */
            if (is_pipe_mode_old && is_pipe_mode_new && (was_nwk_qos_set == TRUE))
            {
                /** In this case the old and new model is PIPE - exchange templates */
                /** Exchange QOS PIPE profile */
                new_profile = old_profile;

                /** need to update the profile */
                SHR_IF_ERR_EXIT(dnx_qos_egress_network_qos_pipe_profile_update
                                (unit, (uint16) nwk_qos_idx, (uint16) nwk_qos_idx, 0, &new_profile,
                                 &is_first_profile_ref, &is_last_profile_ref));
            }
            else if (is_pipe_mode_new)
            {
                /** In this case the old model is UNIFORM and new model is PIPE - allocate new profile */

                /** allocate QOS PIPE profile */
                SHR_IF_ERR_EXIT(dnx_qos_egress_network_qos_pipe_profile_allocate
                                (unit, (uint8) nwk_qos_idx, (uint8) nwk_qos_idx, 0, (int *) &new_profile,
                                 &is_first_profile_ref));
            }
            else if (is_pipe_mode_old)
            {
                /** In this case the old model is PIPE and new model is UNIFORM - remove old template */
                if (was_nwk_qos_set)
                {
                     /** Free old QOS profile template */
                    SHR_IF_ERR_EXIT(dnx_qos_egress_network_qos_pipe_profile_free
                                    (unit, (int) old_profile, &is_last_profile_ref));
                }
            }

            if (is_pipe_mode_new)
            {
                dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_NWK_QOS_IDX, INST_SINGLE, new_profile);
            }
        }
        else
        {
            /*
             * set old profile
             */
            if (was_nwk_qos_set)
            {
                dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_NWK_QOS_IDX, INST_SINGLE, old_profile);
            }
        }

        if (initiator_info->valid_elements_set & BCM_FLOW_INITIATOR_ELEMENT_QOS_EGRESS_MODEL_VALID)
        {
            /** set egress_qos_model with the new value */
            SHR_IF_ERR_EXIT(dnx_qos_egress_model_bcm_to_dbal
                            (unit, &initiator_info->egress_qos_model, &encap_qos_model));
        }
        if ((is_field_valid == TRUE) ||
            (initiator_info->valid_elements_set & BCM_FLOW_INITIATOR_ELEMENT_QOS_EGRESS_MODEL_VALID))
        {
            dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_ENCAP_QOS_MODEL, INST_SINGLE,
                                         encap_qos_model);
        }
    }

    if (is_first_profile_ref)
    {
        SHR_IF_ERR_EXIT(dnx_qos_egress_network_qos_pipe_profile_set
                        (unit, new_profile, (uint8) nwk_qos_idx, (uint8) nwk_qos_idx, 0));
    }

    if (is_last_profile_ref)
    {
        (*last_network_qos_profile) = old_profile;
    }

exit:
    SHR_FUNC_EXIT;
}

static shr_error_e
dnx_flow_qos_egress_app_params_get(
    int unit,
    dnx_flow_app_config_t * flow_app_info,
    uint32 entry_handle_id,
    bcm_flow_initiator_info_t * initiator_info,
    bcm_flow_special_fields_t * special_fields,
    uint8 *is_qos_valid)
{
    uint8 is_field_valid;
    uint8 idx;
    dbal_enum_value_field_encap_qos_model_e encap_qos_model;
    bcm_qos_egress_model_t model;

    SHR_FUNC_INIT_VARS(unit);

    SHR_IF_ERR_EXIT(dnx_lif_table_mngr_uint32_field_on_dbal_handle_get
                    (unit, entry_handle_id, DBAL_FIELD_ENCAP_QOS_MODEL, &is_field_valid, &encap_qos_model));
    if (is_field_valid == TRUE)
    {
        (*is_qos_valid) = TRUE;

        SHR_IF_ERR_EXIT(dnx_qos_egress_model_dbal_to_bcm(unit, encap_qos_model, &model));
        initiator_info->egress_qos_model.egress_qos = model.egress_qos;
        initiator_info->egress_qos_model.egress_ecn = model.egress_ecn;

        if ((encap_qos_model == DBAL_ENUM_FVAL_ENCAP_QOS_MODEL_PIPE_NEXT_SPACE) ||
            (encap_qos_model == DBAL_ENUM_FVAL_ENCAP_QOS_MODEL_PIPE_MY_SPACE) ||
            (encap_qos_model == DBAL_ENUM_FVAL_ENCAP_QOS_MODEL_PIPE_ECN))
        {
            uint32 qos_model_idx;
            uint8 qos_profile_get_dummy_var;
            uint8 nwk_idx;
            uint8 pri_cfi_found = 0;

            /** Get QOS pipe index */
            SHR_IF_ERR_EXIT(dnx_lif_table_mngr_uint32_field_on_dbal_handle_get(unit, entry_handle_id,
                                                                               DBAL_FIELD_NWK_QOS_IDX, &is_field_valid,
                                                                               &qos_model_idx));
            if (is_field_valid == FALSE)
            {
                SHR_EXIT();
            }

            SHR_IF_ERR_EXIT(dnx_qos_egress_network_qos_pipe_profile_attr_get
                            (unit, qos_model_idx, &nwk_idx, &qos_profile_get_dummy_var, &qos_profile_get_dummy_var));

            for (idx = 0; flow_app_info->special_fields[idx] != FLOW_S_F_EMPTY; idx++)
            {
                if (flow_app_info->special_fields[idx] == FLOW_S_F_QOS_DSCP)
                {
                    SPECIAL_FIELD_UINT32_DATA_ADD(special_fields, FLOW_S_F_QOS_DSCP, nwk_idx);
                    break;
                }
                else if (flow_app_info->special_fields[idx] == FLOW_S_F_QOS_EXP)
                {
                    SPECIAL_FIELD_UINT32_DATA_ADD(special_fields, FLOW_S_F_QOS_EXP,
                                                  DNX_QOS_IDX_NWK_QOS_GET_EXP0(nwk_idx));
                    break;
                }
                else if (flow_app_info->special_fields[idx] == FLOW_S_F_QOS_EXP_1)
                {
                    SPECIAL_FIELD_UINT32_DATA_ADD(special_fields, FLOW_S_F_QOS_EXP_1,
                                                  DNX_QOS_IDX_NWK_QOS_GET_EXP1(nwk_idx));
                    break;
                }
                else if (flow_app_info->special_fields[idx] == FLOW_S_F_QOS_PRI)
                {
                    SPECIAL_FIELD_UINT32_DATA_ADD(special_fields, FLOW_S_F_QOS_PRI,
                                                  DNX_QOS_IDX_NWK_QOS_PCP_GET(nwk_idx));
                    pri_cfi_found++;
                    if (pri_cfi_found > 1)
                    {
                        break;
                    }
                }
                else if (flow_app_info->special_fields[idx] == FLOW_S_F_QOS_CFI)
                {
                    SPECIAL_FIELD_UINT32_DATA_ADD(special_fields, FLOW_S_F_QOS_CFI,
                                                  DNX_QOS_IDX_NWK_QOS_CFI_GET(nwk_idx));
                    pri_cfi_found++;
                    if (pri_cfi_found > 1)
                    {
                        break;
                    }
                }
            }
            if (flow_app_info->special_fields[idx] == FLOW_S_F_EMPTY)
            {
                SHR_ERR_EXIT(_SHR_E_PARAM, "No network qos field supported for app");
            }
        }
    }

exit:
    SHR_FUNC_EXIT;
}

static shr_error_e
dnx_flow_qos_egress_encap_destroy(
    int unit,
    bcm_flow_handle_info_t * flow_handle_info,
    uint32 entry_handle_id,
    dnx_algo_gpm_gport_hw_resources_t * gport_hw_resources)
{
    dbal_enum_value_field_encap_qos_model_e encap_qos_model;
    bcm_qos_egress_model_t model;
    uint32 profile;
    uint8 is_last_profile_ref = FALSE, is_set = FALSE;

    SHR_FUNC_INIT_VARS(unit);

    SHR_IF_ERR_EXIT(dnx_flow_lif_mgmt_field_is_set_in_hw
                    (unit, flow_handle_info, gport_hw_resources, DBAL_FIELD_NWK_QOS_IDX, &is_set));
    if (is_set)
    {
        SHR_IF_ERR_EXIT(dbal_entry_handle_value_field32_get
                        (unit, entry_handle_id, DBAL_FIELD_ENCAP_QOS_MODEL, INST_SINGLE, &encap_qos_model));
        SHR_IF_ERR_EXIT(dnx_qos_egress_model_dbal_to_bcm(unit, encap_qos_model, &model));
        if ((encap_qos_model == DBAL_ENUM_FVAL_ENCAP_QOS_MODEL_PIPE_NEXT_SPACE) ||
            (encap_qos_model == DBAL_ENUM_FVAL_ENCAP_QOS_MODEL_PIPE_MY_SPACE) ||
            (encap_qos_model == DBAL_ENUM_FVAL_ENCAP_QOS_MODEL_PIPE_ECN))
        {
            uint8 is_field_valid = FALSE;

            SHR_IF_ERR_EXIT(dnx_lif_table_mngr_uint32_field_on_dbal_handle_get(unit, entry_handle_id,
                                                                               DBAL_FIELD_NWK_QOS_IDX,
                                                                               &is_field_valid, &profile));
            if (is_field_valid)
            {
                /** Free old QOS profile template */
                SHR_IF_ERR_EXIT(dnx_qos_egress_network_qos_pipe_profile_free
                                (unit, (int) profile, &is_last_profile_ref));
            }
            if (is_last_profile_ref)
            {
                SHR_IF_ERR_EXIT(dnx_qos_egress_network_qos_pipe_profile_hw_clear(unit, profile));
            }
        }
    }

exit:
    SHR_FUNC_EXIT;
}

static shr_error_e
dnx_flow_qos_egress_is_logic_required(
    int unit,
    dnx_flow_app_config_t * flow_app_info,
    uint8 *is_logic_required)
{
    uint8 field_idx;

    SHR_FUNC_INIT_VARS(unit);

    (*is_logic_required) = FALSE;

    /** if app does not support common field qos_egress_model, logic is not requried */
    if (!_SHR_IS_FLAG_SET(flow_app_info->valid_common_fields_bitmap, BCM_FLOW_INITIATOR_ELEMENT_QOS_EGRESS_MODEL_VALID))
    {
        SHR_EXIT();
    }

    if (_SHR_IS_FLAG_SET(flow_app_info->valid_common_fields_bitmap, BCM_FLOW_INITIATOR_ELEMENT_TTL_VALID))
    {
        (*is_logic_required) = TRUE;
        SHR_EXIT();
    }

    for (field_idx = 0; flow_app_info->special_fields[field_idx] != FLOW_S_F_EMPTY; field_idx++)
    {
        if (flow_app_info->special_fields[field_idx] == FLOW_S_F_QOS_EXP)
        {
            (*is_logic_required) = TRUE;
            break;
        }

        if (flow_app_info->special_fields[field_idx] == FLOW_S_F_QOS_DSCP)
        {
            (*is_logic_required) = TRUE;
            break;
        }

        if (flow_app_info->special_fields[field_idx] == FLOW_S_F_QOS_PRI)
        {
            (*is_logic_required) = TRUE;
            break;
        }

        if (flow_app_info->special_fields[field_idx] == FLOW_S_F_QOS_CFI)
        {
            (*is_logic_required) = TRUE;
            break;
        }
    }

exit:
    SHR_FUNC_EXIT;
}

static shr_error_e
dnx_flow_qos_egress_get(
    int unit,
    dnx_flow_app_config_t * flow_app_info,
    uint32 entry_handle_id,
    bcm_flow_initiator_info_t * initiator_info,
    bcm_flow_special_fields_t * special_fields)
{
    uint8 is_qos_valid = FALSE;

    SHR_FUNC_INIT_VARS(unit);

    SHR_IF_ERR_EXIT(dnx_flow_init_ttl_get(unit, entry_handle_id, initiator_info, &is_qos_valid));

    SHR_IF_ERR_EXIT(dnx_flow_qos_egress_app_params_get
                    (unit, flow_app_info, entry_handle_id, initiator_info, special_fields, &is_qos_valid));

    if (is_qos_valid == TRUE)
    {
        initiator_info->valid_elements_set |= BCM_FLOW_INITIATOR_ELEMENT_QOS_EGRESS_MODEL_VALID;
    }

exit:
    SHR_FUNC_EXIT;
}

static shr_error_e
dnx_flow_qos_egress_replace(
    int unit,
    dnx_flow_app_config_t * flow_app_info,
    uint32 entry_handle_id,
    uint32 get_entry_handle_id,
    bcm_flow_initiator_info_t * initiator_info,
    bcm_flow_special_fields_t * special_fields,
    int *last_network_qos_profile,
    int *last_ttl_profile)
{
    bcm_flow_initiator_info_t prev_initiator_info = { 0 };
    bcm_flow_special_fields_t prev_special_fields = { 0 };

    SHR_FUNC_INIT_VARS(unit);

    /** get the old params */
    SHR_IF_ERR_EXIT(dnx_flow_qos_egress_get(unit, flow_app_info, get_entry_handle_id,
                                            &prev_initiator_info, &prev_special_fields));

    /** treat with the common field QOS_EGRESS_MODEL data */
    if ((initiator_info->valid_elements_set & BCM_FLOW_INITIATOR_ELEMENT_QOS_EGRESS_MODEL_VALID) ||
        (prev_initiator_info.valid_elements_set & BCM_FLOW_INITIATOR_ELEMENT_QOS_EGRESS_MODEL_VALID))
    {
        /** treat with the ttl */
        SHR_IF_ERR_EXIT(dnx_flow_init_ttl_replace(unit, entry_handle_id, get_entry_handle_id, initiator_info,
                                                  special_fields, last_ttl_profile));
        /** treat with the qos app params */
        SHR_IF_ERR_EXIT(dnx_flow_qos_egress_app_params_replace(unit, entry_handle_id, get_entry_handle_id,
                                                               initiator_info, special_fields,
                                                               last_network_qos_profile));

    }

exit:
    SHR_FUNC_EXIT;
}

shr_error_e
dnx_flow_dl_egress_qos_profile_get(
    int unit,
    dnx_flow_app_config_t * flow_app_info,
    uint32 entry_handle_id,
    bcm_flow_initiator_info_t * initiator_info,
    bcm_flow_special_fields_t * special_fields)
{
    uint8 is_logic_required;

    SHR_FUNC_INIT_VARS(unit);

    /** check if the logic is required to run */
    SHR_IF_ERR_EXIT(dnx_flow_qos_egress_is_logic_required(unit, flow_app_info, &is_logic_required));
    if (is_logic_required == FALSE)
    {
        SHR_EXIT();
    }

    SHR_IF_ERR_EXIT(dnx_flow_qos_egress_get(unit, flow_app_info, entry_handle_id, initiator_info, special_fields));

exit:
    SHR_FUNC_EXIT;
}

shr_error_e
dnx_flow_dl_egress_qos_profile_create(
    int unit,
    uint32 entry_handle_id,
    dnx_flow_app_config_t * flow_app_info,
    bcm_flow_initiator_info_t * initiator_info,
    bcm_flow_special_fields_t * special_fields)
{
    uint8 is_logic_required;

    SHR_FUNC_INIT_VARS(unit);

    /** check if the logic is required to run */
    SHR_IF_ERR_EXIT(dnx_flow_qos_egress_is_logic_required(unit, flow_app_info, &is_logic_required));
    if (is_logic_required == FALSE)
    {
        SHR_EXIT();
    }

    /** treat with the ttl field */
    SHR_IF_ERR_EXIT(dnx_flow_init_ttl_create(unit, entry_handle_id, initiator_info, special_fields));

    /** treat with the rest of the qos fields */
    SHR_IF_ERR_EXIT(dnx_flow_qos_egress_app_params_create(unit, entry_handle_id, initiator_info, special_fields));

exit:
    SHR_FUNC_EXIT;
}

shr_error_e
dnx_flow_dl_egress_qos_profile_destroy(
    int unit,
    bcm_flow_handle_info_t * flow_handle_info,
    dnx_flow_app_config_t * flow_app_info,
    uint32 entry_handle_id,
    dnx_algo_gpm_gport_hw_resources_t * gport_hw_resources)
{
    uint8 is_logic_required;

    SHR_FUNC_INIT_VARS(unit);

    /** check if the logic is required to run */
    SHR_IF_ERR_EXIT(dnx_flow_qos_egress_is_logic_required(unit, flow_app_info, &is_logic_required));
    if (is_logic_required == FALSE)
    {
        SHR_EXIT();
    }

    /** destroy ttl profile if relevant */
    SHR_IF_ERR_EXIT(dnx_flow_init_ttl_destroy(unit, flow_handle_info, entry_handle_id, gport_hw_resources));

    /** destroy encap qos profile if relevant */
    SHR_IF_ERR_EXIT(dnx_flow_qos_egress_encap_destroy(unit, flow_handle_info, entry_handle_id, gport_hw_resources));

exit:
    SHR_FUNC_EXIT;
}

shr_error_e
dnx_flow_dl_egress_qos_profile_replace(
    int unit,
    bcm_flow_handle_info_t * flow_handle_info,
    dnx_flow_app_config_t * flow_app_info,
    uint32 entry_handle_id,
    uint32 get_entry_handle_id,
    dnx_algo_gpm_gport_hw_resources_t * gport_hw_resources,
    bcm_flow_initiator_info_t * initiator_info,
    bcm_flow_special_fields_t * special_fields,
    int *last_network_qos_profile,
    int *last_ttl_profile)
{
    uint8 is_set;
    uint8 is_logic_required;

    SHR_FUNC_INIT_VARS(unit);

    (*last_network_qos_profile) = FLOW_PROFILE_INVALID;
    (*last_ttl_profile) = FLOW_PROFILE_INVALID;

    /** check if the logic is required to run */
    SHR_IF_ERR_EXIT(dnx_flow_qos_egress_is_logic_required(unit, flow_app_info, &is_logic_required));
    if (is_logic_required == FALSE)
    {
        SHR_EXIT();
    }

    if (_SHR_IS_FLAG_SET(initiator_info->valid_elements_set, BCM_FLOW_INITIATOR_ELEMENT_QOS_EGRESS_MODEL_VALID))
    {
        SHR_IF_ERR_EXIT(dnx_flow_lif_mgmt_field_is_set_in_hw(unit, flow_handle_info, gport_hw_resources,
                                                             DBAL_FIELD_ENCAP_QOS_MODEL, &is_set));

        if (is_set)
        {
            /** update the QoS */
            SHR_IF_ERR_EXIT(dnx_flow_qos_egress_replace(unit, flow_app_info, entry_handle_id, get_entry_handle_id,
                                                        initiator_info, special_fields, last_network_qos_profile,
                                                        last_ttl_profile));
        }
        else
        {
            /** in case egress_qos_model was not set previous - need to do a create */
            SHR_IF_ERR_EXIT(dnx_flow_dl_egress_qos_profile_create
                            (unit, entry_handle_id, flow_app_info, initiator_info, special_fields));
        }
    }
    else if (_SHR_IS_FLAG_SET(initiator_info->valid_elements_clear, BCM_FLOW_INITIATOR_ELEMENT_QOS_EGRESS_MODEL_VALID))
    {
        /** need to delete the QoS */
        SHR_IF_ERR_EXIT(dnx_flow_dl_egress_qos_profile_destroy(unit, flow_handle_info, flow_app_info,
                                                               get_entry_handle_id, gport_hw_resources));
    }
    else
    {
        /** update the QoS */
        SHR_IF_ERR_EXIT(dnx_flow_qos_egress_replace(unit, flow_app_info, entry_handle_id, get_entry_handle_id,
                                                    initiator_info, special_fields, last_network_qos_profile,
                                                    last_ttl_profile));
    }

exit:
    SHR_FUNC_EXIT;
}

shr_error_e
dnx_flow_dl_egress_qos_profile_validate(
    int unit,
    bcm_flow_handle_info_t * flow_handle_info,
    dnx_flow_app_config_t * flow_app_info,
    bcm_flow_initiator_info_t * bcm_app_info,
    bcm_flow_special_fields_t * special_fields)
{
    bcm_flow_special_field_t special_field_data;
    shr_error_e rv;

    SHR_FUNC_INIT_VARS(unit);

    if (_SHR_IS_FLAG_SET(bcm_app_info->valid_elements_set, BCM_FLOW_INITIATOR_ELEMENT_QOS_EGRESS_MODEL_VALID))
    {
        if ((bcm_app_info->egress_qos_model.egress_qos < bcmQosEgressModelUniform) ||
            (bcm_app_info->egress_qos_model.egress_qos > bcmQosEgressModelInitial))
        {
            SHR_ERR_EXIT(_SHR_E_PARAM, "egess qos mode is out of range");
        }
    }

    if ((!_SHR_IS_FLAG_SET(flow_handle_info->flags, BCM_FLOW_HANDLE_INFO_REPLACE)) &&
        (!_SHR_IS_FLAG_SET(bcm_app_info->valid_elements_set, BCM_FLOW_INITIATOR_ELEMENT_QOS_EGRESS_MODEL_VALID)))
    {
        rv = dnx_flow_special_field_data_get(unit, special_fields, FLOW_S_F_QOS_DSCP, &special_field_data);
        SHR_IF_ERR_EXIT_EXCEPT_IF(rv, _SHR_E_NOT_FOUND);
        if (rv == _SHR_E_NONE)
        {
            SHR_ERR_EXIT(_SHR_E_PARAM,
                         "cannot set special_field FLOW_S_F_QOS_DSCP without setting common field BCM_FLOW_INITIATOR_ELEMENT_QOS_EGRESS_MODEL_VALID");
        }

        rv = dnx_flow_special_field_data_get(unit, special_fields, FLOW_S_F_QOS_EXP, &special_field_data);
        SHR_IF_ERR_EXIT_EXCEPT_IF(rv, _SHR_E_NOT_FOUND);
        if (rv == _SHR_E_NONE)
        {
            SHR_ERR_EXIT(_SHR_E_PARAM,
                         "cannot set special_field FLOW_S_F_QOS_EXP without setting common field BCM_FLOW_INITIATOR_ELEMENT_QOS_EGRESS_MODEL_VALID");
        }

        rv = dnx_flow_special_field_data_get(unit, special_fields, FLOW_S_F_QOS_PRI, &special_field_data);
        SHR_IF_ERR_EXIT_EXCEPT_IF(rv, _SHR_E_NOT_FOUND);
        if (rv == _SHR_E_NONE)
        {
            SHR_ERR_EXIT(_SHR_E_PARAM,
                         "cannot set special_field FLOW_S_F_QOS_PRI without setting common field BCM_FLOW_INITIATOR_ELEMENT_QOS_EGRESS_MODEL_VALID");
        }

        rv = dnx_flow_special_field_data_get(unit, special_fields, FLOW_S_F_QOS_CFI, &special_field_data);
        SHR_IF_ERR_EXIT_EXCEPT_IF(rv, _SHR_E_NOT_FOUND);
        if (rv == _SHR_E_NONE)
        {
            SHR_ERR_EXIT(_SHR_E_PARAM,
                         "cannot set special_field FLOW_S_F_QOS_CFI without setting common field BCM_FLOW_INITIATOR_ELEMENT_QOS_EGRESS_MODEL_VALID");
        }

        if (_SHR_IS_FLAG_SET(bcm_app_info->valid_elements_set, BCM_FLOW_INITIATOR_ELEMENT_TTL_VALID))
        {
            SHR_ERR_EXIT(_SHR_E_PARAM,
                         "cannot set common field BCM_FLOW_INITIATOR_ELEMENT_TTL_VALID without setting common field BCM_FLOW_INITIATOR_ELEMENT_QOS_EGRESS_MODEL_VALID");
        }
    }
    else if ((!_SHR_IS_FLAG_SET(flow_handle_info->flags, BCM_FLOW_HANDLE_INFO_REPLACE)) &&
             (_SHR_IS_FLAG_SET(bcm_app_info->valid_elements_set, BCM_FLOW_INITIATOR_ELEMENT_QOS_EGRESS_MODEL_VALID)))
    {
        /** if BCM_FLOW_INITIATOR_ELEMENT_QOS_EGRESS_MODEL_VALID was set, check that either both pri and cfi were set, or none of them */
        uint8 pri_found, cfi_found;

        rv = dnx_flow_special_field_data_get(unit, special_fields, FLOW_S_F_QOS_PRI, &special_field_data);
        SHR_IF_ERR_EXIT_EXCEPT_IF(rv, _SHR_E_NOT_FOUND);
        pri_found = (rv == _SHR_E_NONE) ? TRUE : FALSE;

        rv = dnx_flow_special_field_data_get(unit, special_fields, FLOW_S_F_QOS_CFI, &special_field_data);
        SHR_IF_ERR_EXIT_EXCEPT_IF(rv, _SHR_E_NOT_FOUND);
        cfi_found = (rv == _SHR_E_NONE) ? TRUE : FALSE;

        if (pri_found != cfi_found)
        {
            SHR_ERR_EXIT(_SHR_E_PARAM, "special_fields FLOW_S_F_QOS_PRI and FLOW_S_F_QOS_CFI must be set together");
        }
    }
    if (_SHR_IS_FLAG_SET(flow_handle_info->flags, BCM_FLOW_HANDLE_INFO_REPLACE) &&
        _SHR_IS_FLAG_SET(flow_app_info->valid_common_fields_bitmap, BCM_FLOW_INITIATOR_ELEMENT_QOS_EGRESS_MODEL_VALID))
    {
        /** in replace, cannot clear pri or cfi without clearing the other */
        uint8 pri_clear = FALSE, cfi_clear = FALSE;

        rv = dnx_flow_special_field_data_get(unit, special_fields, FLOW_S_F_QOS_PRI, &special_field_data);
        SHR_IF_ERR_EXIT_EXCEPT_IF(rv, _SHR_E_NOT_FOUND);
        if (rv == _SHR_E_NONE)
        {
            pri_clear = special_field_data.is_clear ? TRUE : FALSE;
        }

        rv = dnx_flow_special_field_data_get(unit, special_fields, FLOW_S_F_QOS_CFI, &special_field_data);
        SHR_IF_ERR_EXIT_EXCEPT_IF(rv, _SHR_E_NOT_FOUND);
        if (rv == _SHR_E_NONE)
        {
            cfi_clear = special_field_data.is_clear ? TRUE : FALSE;
        }

        if (pri_clear != cfi_clear)
        {
            SHR_ERR_EXIT(_SHR_E_PARAM, "special_fields FLOW_S_F_QOS_PRI and FLOW_S_F_QOS_CFI must be cleared together");
        }
    }

exit:
    SHR_FUNC_EXIT;
}

/************************ INGRESS_QOS_PROFILE APIs ************************/

static shr_error_e
dnx_flow_ingress_qos_profile_replace_internal(
    int unit,
    uint32 set_entry_handle_id,
    int old_propag_profile,
    dnx_flow_app_config_t * flow_app_info,
    bcm_flow_terminator_info_t * terminator_info,
    uint8 *qos_propagation_prof_last_ref)
{
    dnx_qos_propagation_type_e ttl_propagation_type = DNX_QOS_PROPAGATION_TYPE_SHORT_PIPE;
    dnx_qos_propagation_type_e phb_propagation_type = DNX_QOS_PROPAGATION_TYPE_SHORT_PIPE;
    dnx_qos_propagation_type_e remark_propagation_type = DNX_QOS_PROPAGATION_TYPE_SHORT_PIPE;
    dnx_qos_propagation_type_e ecn_propagation_type = DNX_QOS_PROPAGATION_TYPE_SHORT_PIPE;
    int propag_profile = 0;
    uint8 qos_propagation_prof_first_ref = 0;

    SHR_FUNC_INIT_VARS(unit);

    (*qos_propagation_prof_last_ref) = 0;

    /** Set QOS */
    ecn_propagation_type = DNX_QOS_PROPAGATION_TYPE_SHORT_PIPE;
    if (dnx_flow_app_is_ind_set(unit, flow_app_info, FLOW_APP_TERM_IND_QOS_PHB_DEFAULT_PIPE))
    {
        phb_propagation_type = DNX_QOS_PROPAGATION_TYPE_PIPE;
    }

    if (dnx_flow_app_is_ind_set(unit, flow_app_info, FLOW_APP_TERM_IND_QOS_RM_DEFAULT_PIPE))
    {
        remark_propagation_type = DNX_QOS_PROPAGATION_TYPE_PIPE;
    }

    if (dnx_flow_app_is_ind_set(unit, flow_app_info, FLOW_APP_TERM_IND_QOS_TTL_DEFAULT_PIPE))
    {
        ttl_propagation_type = DNX_QOS_PROPAGATION_TYPE_PIPE;
    }

    /*
     * If this field is to be restored to default - leave the profile properties with their default values, so that the 
     * original profile will be replaced with the default one 
     */
    if (!_SHR_IS_FLAG_SET(terminator_info->valid_elements_clear, BCM_FLOW_TERMINATOR_ELEMENT_QOS_INGRESS_MODEL_VALID))
    {
        if (terminator_info->ingress_qos_model.ingress_phb != bcmQosIngressModelInvalid)
        {
            SHR_IF_ERR_EXIT(dnx_qos_ingress_qos_model_to_propag_type
                            (unit, terminator_info->ingress_qos_model.ingress_phb, &phb_propagation_type));
        }

        if (terminator_info->ingress_qos_model.ingress_remark != bcmQosIngressModelInvalid)
        {
            SHR_IF_ERR_EXIT(dnx_qos_ingress_qos_model_to_propag_type
                            (unit, terminator_info->ingress_qos_model.ingress_remark, &remark_propagation_type));
        }

        if (terminator_info->ingress_qos_model.ingress_ttl != bcmQosIngressModelInvalid)
        {
            SHR_IF_ERR_EXIT(dnx_qos_ingress_qos_model_to_propag_type
                            (unit, terminator_info->ingress_qos_model.ingress_ttl, &ttl_propagation_type));
        }
    }

    /** Update propagation profile ID. */
    SHR_IF_ERR_EXIT(dnx_qos_ingress_propagation_profile_alloc
                    (unit, phb_propagation_type, remark_propagation_type,
                     ecn_propagation_type, ttl_propagation_type, old_propag_profile, &propag_profile,
                     &qos_propagation_prof_first_ref, qos_propagation_prof_last_ref));

    /** Update the profile table if it is first used*/
    if (qos_propagation_prof_first_ref == TRUE)
    {
        SHR_IF_ERR_EXIT(dnx_qos_ingress_propagation_profile_hw_set(unit, propag_profile, phb_propagation_type,
                                                                   remark_propagation_type,
                                                                   ecn_propagation_type, ttl_propagation_type));
    }

    /*
     * In case this is a cleared-field - no need to set the default profile to HW, only manage the profiles 
     */
    if (!_SHR_IS_FLAG_SET(terminator_info->valid_elements_clear, BCM_FLOW_TERMINATOR_ELEMENT_QOS_INGRESS_MODEL_VALID))
    {
        dbal_entry_value_field32_set(unit, set_entry_handle_id, DBAL_FIELD_PROPAGATION_PROFILE, INST_SINGLE,
                                     (uint32) propag_profile);
    }

    if (terminator_info->ingress_qos_model.ingress_ecn == bcmQosIngressEcnModelEligible)
    {
        dbal_entry_value_field32_set(unit, set_entry_handle_id, DBAL_FIELD_ECN_MAPPING_PROFILE, INST_SINGLE,
                                     DNX_QOS_ECN_MAPPING_PROFILE_ELIGIBLE);
    }

exit:
    SHR_FUNC_EXIT;
}

static shr_error_e
dnx_flow_ingress_qos_profile_set_to_default(
    int unit,
    uint32 set_entry_handle_id,
    int old_propag_profile,
    dnx_flow_app_config_t * flow_app_info,
    bcm_flow_terminator_info_t * terminator_info,
    uint8 *qos_propagation_prof_last_ref)
{
    dnx_qos_propagation_type_e ttl_propagation_type = DNX_QOS_PROPAGATION_TYPE_SHORT_PIPE;
    dnx_qos_propagation_type_e phb_propagation_type = DNX_QOS_PROPAGATION_TYPE_SHORT_PIPE;
    dnx_qos_propagation_type_e remark_propagation_type = DNX_QOS_PROPAGATION_TYPE_SHORT_PIPE;
    dnx_qos_propagation_type_e ecn_propagation_type = DNX_QOS_PROPAGATION_TYPE_SHORT_PIPE;
    int propag_profile = 0;
    uint8 qos_propagation_prof_first_ref = 0;

    SHR_FUNC_INIT_VARS(unit);

    (*qos_propagation_prof_last_ref) = 0;

    /** Set QOS */
    ecn_propagation_type = DNX_QOS_PROPAGATION_TYPE_SHORT_PIPE;
    if (dnx_flow_app_is_ind_set(unit, flow_app_info, FLOW_APP_TERM_IND_QOS_PHB_DEFAULT_PIPE))
    {
        phb_propagation_type = DNX_QOS_PROPAGATION_TYPE_PIPE;
    }

    if (dnx_flow_app_is_ind_set(unit, flow_app_info, FLOW_APP_TERM_IND_QOS_RM_DEFAULT_PIPE))
    {
        remark_propagation_type = DNX_QOS_PROPAGATION_TYPE_PIPE;
    }

    if (dnx_flow_app_is_ind_set(unit, flow_app_info, FLOW_APP_TERM_IND_QOS_TTL_DEFAULT_PIPE))
    {
        ttl_propagation_type = DNX_QOS_PROPAGATION_TYPE_PIPE;
    }

    /** Update propagation profile ID. */
    SHR_IF_ERR_EXIT(dnx_qos_ingress_propagation_profile_alloc
                    (unit, phb_propagation_type, remark_propagation_type,
                     ecn_propagation_type, ttl_propagation_type, old_propag_profile, &propag_profile,
                     &qos_propagation_prof_first_ref, qos_propagation_prof_last_ref));

    /** Update the profile table if it is first used*/
    if (qos_propagation_prof_first_ref == TRUE)
    {
        SHR_IF_ERR_EXIT(dnx_qos_ingress_propagation_profile_hw_set(unit, propag_profile, phb_propagation_type,
                                                                   remark_propagation_type,
                                                                   ecn_propagation_type, ttl_propagation_type));
    }

exit:
    SHR_FUNC_EXIT;
}

shr_error_e
dnx_flow_dl_ingress_qos_profile_get(
    int unit,
    dnx_flow_app_config_t * flow_app_info,
    bcm_flow_handle_info_t * flow_handle_info,
    uint32 entry_handle_id,
    dnx_algo_gpm_gport_hw_resources_t * gport_hw_resources,
    bcm_flow_terminator_info_t * terminator_info,
    bcm_flow_special_fields_t * special_fields)
{
    dnx_qos_propagation_type_e ttl_propagation_type = DNX_QOS_PROPAGATION_TYPE_SHORT_PIPE;
    dnx_qos_propagation_type_e phb_propagation_type = DNX_QOS_PROPAGATION_TYPE_SHORT_PIPE;
    dnx_qos_propagation_type_e remark_propagation_type = DNX_QOS_PROPAGATION_TYPE_SHORT_PIPE;
    dnx_qos_propagation_type_e ecn_propagation_type = DNX_QOS_PROPAGATION_TYPE_SHORT_PIPE;
    uint32 dbal_field = 0;
    int propagation_prof;
    uint8 is_valid;
    dnx_qos_propagation_type_e phb_default_type, ttl_default_type, remark_default_type;

    SHR_FUNC_INIT_VARS(unit);

    FLOW_FIELD_GET_ERR_EXIT(dbal_entry_handle_value_field32_get
                            (unit, entry_handle_id, DBAL_FIELD_PROPAGATION_PROFILE, INST_SINGLE, &dbal_field));

    terminator_info->valid_elements_set |= BCM_FLOW_TERMINATOR_ELEMENT_QOS_INGRESS_MODEL_VALID;

    propagation_prof = (int) dbal_field;

    /** Update QOS flags according to returned propagation_prof */
    SHR_IF_ERR_EXIT(dnx_qos_ingress_propagation_profile_attr_get
                    (unit, propagation_prof, &phb_propagation_type, &remark_propagation_type, &ecn_propagation_type,
                     &ttl_propagation_type));

    if (dnx_flow_app_is_ind_set(unit, flow_app_info, FLOW_APP_TERM_IND_QOS_PHB_DEFAULT_PIPE))
    {
        phb_default_type = DNX_QOS_PROPAGATION_TYPE_PIPE;
    }
    else
    {
        phb_default_type = DNX_QOS_PROPAGATION_TYPE_SHORT_PIPE;
    }
    if (dnx_flow_app_is_ind_set(unit, flow_app_info, FLOW_APP_TERM_IND_QOS_TTL_DEFAULT_PIPE))
    {
        ttl_default_type = DNX_QOS_PROPAGATION_TYPE_PIPE;
    }
    else
    {
        ttl_default_type = DNX_QOS_PROPAGATION_TYPE_SHORT_PIPE;
    }
    if (dnx_flow_app_is_ind_set(unit, flow_app_info, FLOW_APP_TERM_IND_QOS_RM_DEFAULT_PIPE))
    {
        remark_default_type = DNX_QOS_PROPAGATION_TYPE_PIPE;
    }
    else
    {
        remark_default_type = DNX_QOS_PROPAGATION_TYPE_SHORT_PIPE;
    }
    if (phb_propagation_type != phb_default_type)
    {
        SHR_IF_ERR_EXIT(dnx_qos_ingress_propag_type_to_qos_model
                        (unit, phb_propagation_type, &(terminator_info->ingress_qos_model.ingress_phb)));
    }

    if (remark_propagation_type != remark_default_type)
    {
        SHR_IF_ERR_EXIT(dnx_qos_ingress_propag_type_to_qos_model
                        (unit, remark_propagation_type, &(terminator_info->ingress_qos_model.ingress_remark)));
    }

    if (ttl_propagation_type != ttl_default_type)
    {
        SHR_IF_ERR_EXIT(dnx_qos_ingress_propag_type_to_qos_model
                        (unit, ttl_propagation_type, &(terminator_info->ingress_qos_model.ingress_ttl)));
    }

    SHR_IF_ERR_EXIT_NO_MSG(dnx_flow_lif_mgmt_field_is_set_in_hw
                           (unit, flow_handle_info, gport_hw_resources, DBAL_FIELD_ECN_MAPPING_PROFILE, &is_valid));

    if (is_valid)
    {
        uint32 ecn_mapping_profile;

        SHR_IF_ERR_EXIT(dbal_entry_handle_value_field32_get
                        (unit, entry_handle_id, DBAL_FIELD_ECN_MAPPING_PROFILE, INST_SINGLE, &ecn_mapping_profile));

        if (ecn_mapping_profile == DNX_QOS_ECN_MAPPING_PROFILE_ELIGIBLE)
        {
            terminator_info->ingress_qos_model.ingress_ecn = bcmQosIngressEcnModelEligible;
        }
        else
        {
            terminator_info->ingress_qos_model.ingress_ecn = bcmQosIngressEcnModelInvalid;
        }
    }

exit:
    SHR_FUNC_EXIT;
}

shr_error_e
dnx_flow_dl_ingress_qos_profile_create(
    int unit,
    uint32 entry_handle_id,
    dnx_flow_app_config_t * flow_app_info,
    bcm_flow_terminator_info_t * terminator_info,
    bcm_flow_special_fields_t * special_fields)
{
    dnx_qos_propagation_type_e ttl_propagation_type = DNX_QOS_PROPAGATION_TYPE_SHORT_PIPE;
    dnx_qos_propagation_type_e phb_propagation_type = DNX_QOS_PROPAGATION_TYPE_SHORT_PIPE;
    dnx_qos_propagation_type_e remark_propagation_type = DNX_QOS_PROPAGATION_TYPE_SHORT_PIPE;
    dnx_qos_propagation_type_e ecn_propagation_type = DNX_QOS_PROPAGATION_TYPE_SHORT_PIPE;
    int propag_profile = 0;
    int old_propag_profile = 0;
    uint8 qos_propagation_prof_first_ref = 0;
    uint8 qos_propagation_prof_last_ref;

    SHR_FUNC_INIT_VARS(unit);

    if (!_SHR_IS_FLAG_SET(terminator_info->valid_elements_set, BCM_FLOW_TERMINATOR_ELEMENT_QOS_INGRESS_MODEL_VALID))
    {
        /** no qos related fields, just exit */
        SHR_EXIT();
    }

    /** Set QOS default values   */
    if (dnx_flow_app_is_ind_set(unit, flow_app_info, FLOW_APP_TERM_IND_QOS_PHB_DEFAULT_PIPE))
    {
        phb_propagation_type = DNX_QOS_PROPAGATION_TYPE_PIPE;
    }

    if (dnx_flow_app_is_ind_set(unit, flow_app_info, FLOW_APP_TERM_IND_QOS_RM_DEFAULT_PIPE))
    {
        remark_propagation_type = DNX_QOS_PROPAGATION_TYPE_PIPE;
    }

    if (dnx_flow_app_is_ind_set(unit, flow_app_info, FLOW_APP_TERM_IND_QOS_TTL_DEFAULT_PIPE))
    {
        ttl_propagation_type = DNX_QOS_PROPAGATION_TYPE_PIPE;
    }

    /** set qos values */
    if (terminator_info->ingress_qos_model.ingress_phb != bcmQosIngressModelInvalid)
    {
        SHR_IF_ERR_EXIT(dnx_qos_ingress_qos_model_to_propag_type
                        (unit, terminator_info->ingress_qos_model.ingress_phb, &phb_propagation_type));
    }

    if (terminator_info->ingress_qos_model.ingress_remark != bcmQosIngressModelInvalid)
    {
        SHR_IF_ERR_EXIT(dnx_qos_ingress_qos_model_to_propag_type
                        (unit, terminator_info->ingress_qos_model.ingress_remark, &remark_propagation_type));
    }

    if (terminator_info->ingress_qos_model.ingress_ttl != bcmQosIngressModelInvalid)
    {
        SHR_IF_ERR_EXIT(dnx_qos_ingress_qos_model_to_propag_type
                        (unit, terminator_info->ingress_qos_model.ingress_ttl, &ttl_propagation_type));
    }

    /** Alloc propagation profile ID corresponding to chosen QOS model */
    old_propag_profile = DNX_QOS_INGRESS_DEFAULT_PROPAGATION_PROFILE;
    SHR_IF_ERR_EXIT(dnx_qos_ingress_propagation_profile_alloc(unit, phb_propagation_type,
                                                              remark_propagation_type, ecn_propagation_type,
                                                              ttl_propagation_type, old_propag_profile,
                                                              &propag_profile,
                                                              &qos_propagation_prof_first_ref,
                                                              &qos_propagation_prof_last_ref));

    /** Update H/W */
    if (qos_propagation_prof_first_ref == TRUE)
    {
        SHR_IF_ERR_EXIT(dnx_qos_ingress_propagation_profile_hw_set(unit, propag_profile, phb_propagation_type,
                                                                   remark_propagation_type,
                                                                   ecn_propagation_type, ttl_propagation_type));
    }

    dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_PROPAGATION_PROFILE, INST_SINGLE, propag_profile);

    if (terminator_info->ingress_qos_model.ingress_ecn == bcmQosIngressEcnModelEligible)
    {
        dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_ECN_MAPPING_PROFILE, INST_SINGLE,
                                     DNX_QOS_ECN_MAPPING_PROFILE_ELIGIBLE);
    }

exit:
    SHR_FUNC_EXIT;
}

shr_error_e
dnx_flow_dl_ingress_qos_profile_validate(
    int unit,
    bcm_flow_handle_info_t * flow_handle_info,
    dnx_flow_app_config_t * flow_app_info,
    bcm_flow_terminator_info_t * terminator_info)
{
    SHR_FUNC_INIT_VARS(unit);

    if ((terminator_info->ingress_qos_model.ingress_phb == bcmQosIngressModelStuck)
        || (terminator_info->ingress_qos_model.ingress_remark == bcmQosIngressModelStuck)
        || (terminator_info->ingress_qos_model.ingress_ttl == bcmQosIngressModelStuck))
    {
        SHR_ERR_EXIT(_SHR_E_PARAM, "Unsupported qos model - stuck/invalid");
    }

exit:
    SHR_FUNC_EXIT;
}

shr_error_e
dnx_flow_dl_ingress_qos_profile_destroy(
    int unit,
    bcm_flow_handle_info_t * flow_handle_info,
    dnx_flow_app_config_t * flow_app_info,
    uint32 entry_handle_id,
    dnx_algo_gpm_gport_hw_resources_t * gport_hw_resources)
{
    uint32 propagation_prof = 0;
    uint8 is_set = FALSE;

    SHR_FUNC_INIT_VARS(unit);

    /** Should run only in case the field was set at create (resources were allocated). Check only the necessary fields. */
    SHR_IF_ERR_EXIT(dnx_flow_lif_mgmt_field_is_set_in_hw
                    (unit, flow_handle_info, gport_hw_resources, DBAL_FIELD_PROPAGATION_PROFILE, &is_set));
    if (is_set)
    {
        int rv;

        /** Get entry from inLIF table */
        rv = dbal_entry_handle_value_field32_get
            (unit, entry_handle_id, DBAL_FIELD_PROPAGATION_PROFILE, INST_SINGLE, &propagation_prof);
        
        SHR_IF_ERR_EXIT_EXCEPT_IF(rv, _SHR_E_NOT_FOUND);

        if (rv == _SHR_E_NONE)
        {
            SHR_IF_ERR_EXIT(dnx_qos_ingress_propagation_profile_free_and_hw_clear(unit, propagation_prof, NULL));
        }
    }

exit:
    SHR_FUNC_EXIT;
}

shr_error_e
dnx_flow_dl_ingress_qos_profile_replace(
    int unit,
    bcm_flow_handle_info_t * flow_handle_info,
    dnx_flow_app_config_t * flow_app_info,
    uint32 entry_handle_id,
    uint32 get_entry_handle_id,
    dnx_algo_gpm_gport_hw_resources_t * gport_hw_resources,
    bcm_flow_terminator_info_t * terminator_info,
    bcm_flow_special_fields_t * special_fields,
    int *last_profile)
{
    int is_field_set = FALSE, is_field_clear = FALSE;
    shr_error_e rv;
    uint32 old_propag_profile;
    uint8 is_last_ref;

    SHR_FUNC_INIT_VARS(unit);

    (*last_profile) = FLOW_PROFILE_INVALID;

    if (_SHR_IS_FLAG_SET(terminator_info->valid_elements_set, BCM_FLOW_TERMINATOR_ELEMENT_QOS_INGRESS_MODEL_VALID))
    {
        is_field_set = TRUE;
    }
    else if (_SHR_IS_FLAG_SET
             (terminator_info->valid_elements_clear, BCM_FLOW_TERMINATOR_ELEMENT_QOS_INGRESS_MODEL_VALID))
    {
        is_field_clear = TRUE;
    }

    rv = dbal_entry_handle_value_field32_get
        (unit, get_entry_handle_id, DBAL_FIELD_PROPAGATION_PROFILE, INST_SINGLE, &old_propag_profile);
    SHR_IF_ERR_EXIT_EXCEPT_IF(rv, _SHR_E_NOT_FOUND);

    if (rv == _SHR_E_NOT_FOUND)
    {
        if (is_field_set)
        {
            /** first time this field is being set, just create.. */
            SHR_IF_ERR_EXIT(dnx_flow_dl_ingress_qos_profile_create
                            (unit, entry_handle_id, flow_app_info, terminator_info, special_fields));
        }
    }
    else
    {
        if (is_field_set)
        {
            SHR_IF_ERR_EXIT(dnx_flow_ingress_qos_profile_replace_internal
                            (unit, entry_handle_id, old_propag_profile, flow_app_info, terminator_info, &is_last_ref));
            if (is_last_ref)
            {
                (*last_profile) = old_propag_profile;
            }
        }
        else if (is_field_clear)
        {
            SHR_IF_ERR_EXIT(dnx_flow_ingress_qos_profile_set_to_default
                            (unit, entry_handle_id, old_propag_profile, flow_app_info, terminator_info, &is_last_ref));

        }
        else
        {
            /** No new info, copy the old profile to the new entry */
            uint32 old_ecn_profile;
            dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_PROPAGATION_PROFILE, INST_SINGLE,
                                         old_propag_profile);
            rv = dbal_entry_handle_value_field32_get(unit, get_entry_handle_id, DBAL_FIELD_ECN_MAPPING_PROFILE,
                                                     INST_SINGLE, &old_ecn_profile);
            if (rv == _SHR_E_NONE)
            {
                dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_ECN_MAPPING_PROFILE, INST_SINGLE,
                                             old_ecn_profile);
            }
        }
    }
exit:
    SHR_FUNC_EXIT;
}
