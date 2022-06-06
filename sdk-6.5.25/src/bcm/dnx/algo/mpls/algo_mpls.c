/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2021 Broadcom Inc. All rights reserved.
 */

/** \file algo_mpls.c
 *
 *  mpls algorithms initialization and deinitialization.
 */

#ifdef BSL_LOG_MODULE
#error "BSL_LOG_MODULE redefined"
#endif
#define BSL_LOG_MODULE BSL_LS_BCMDNX_MPLS

/*************
 * INCLUDES  *
 *************/
/*
 * { 
 */
#include <shared/shrextend/shrextend_debug.h>
#include <bcm_int/dnx/mpls/mpls_tunnel_term.h>
#include <soc/dnx/dnx_data/auto_generated/dnx_data_mpls.h>
#include <soc/dnx/dnx_data/auto_generated/dnx_data_device.h>
#include <soc/dnx/dnx_data/auto_generated/dnx_data_lif.h>
#include <bcm_int/dnx/algo/mpls/algo_mpls.h>
#include <bcm_int/dnx/algo/swstate/auto_generated/access/algo_mpls_access.h>
#include <soc/dnx/swstate/auto_generated/access/mdb_access.h>
#include <bcm_int/dnx/algo/lif_mngr/lif_mngr.h>

/*
 * } 
 */
/*************
 * DEFINES   *
 *************/
/*
 * { 
 */

/*
 * } 
 */
/*************
 * MACROS    *
 *************/
/*
 * { 
 */

/*
 * } 
 */
/*************
 * TYPE DEFS *
 *************/
/*
 * { 
 */

/*
 * } 
 */
/*************
 * GLOBALS   *
 *************/
/*
 * { 
 */

/*
 * } 
 */
/*************
 * FUNCTIONS *
 *************/
/*
 * { 
 */

/**
* \brief
*   Print an entry of the enablers vector template. See
*       \ref dnx_algo_template_print_data_cb for more details.
*/
void
dnx_algo_mpls_termination_profile_template_print_cb(
    int unit,
    const void *data)
{
#ifdef DNX_SW_STATE_DIAGNOSTIC
    dnx_mpls_termination_profile_t *term_profile = (dnx_mpls_termination_profile_t *) data;
#endif
    SW_STATE_PRETTY_PRINT_INIT_VARIABLES();

    SW_STATE_PRETTY_PRINT_ADD_LINE(SW_STATE_PRETTY_PRINT_TYPE_INT, "labels_to_terminate",
                                   term_profile->labels_to_terminate, "labels to terminate", NULL);
    SW_STATE_PRETTY_PRINT_ADD_LINE(SW_STATE_PRETTY_PRINT_TYPE_INT, "ttl_exp_label_index",
                                   term_profile->ttl_exp_label_index, "ttl exp label index", NULL);
    SW_STATE_PRETTY_PRINT_ADD_LINE(SW_STATE_PRETTY_PRINT_TYPE_INT, "reject_ttl_0", term_profile->reject_ttl_0,
                                   "reject_ttl_0", NULL);
    SW_STATE_PRETTY_PRINT_ADD_LINE(SW_STATE_PRETTY_PRINT_TYPE_INT, "reject_ttl_1", term_profile->reject_ttl_1,
                                   "reject_ttl_1", NULL);
    SW_STATE_PRETTY_PRINT_ADD_LINE(SW_STATE_PRETTY_PRINT_TYPE_INT, "has_cw", term_profile->has_cw, "has_cw", NULL);
    SW_STATE_PRETTY_PRINT_ADD_LINE(SW_STATE_PRETTY_PRINT_TYPE_INT, "expect_bos", term_profile->expect_bos,
                                   "expect_bos", NULL);
    SW_STATE_PRETTY_PRINT_ADD_LINE(SW_STATE_PRETTY_PRINT_TYPE_INT, "check_bos", term_profile->check_bos, "check_bos",
                                   NULL);

    SW_STATE_PRETTY_PRINT_FINISH();
}

/**
* \brief
*   Print the push profile termplate . See
*       \ref dnx_algo_template_print_data_cb for more details.
*/
void
dnx_algo_mpls_push_profile_template_print_cb(
    int unit,
    const void *data)
{
#ifdef DNX_SW_STATE_DIAGNOSTIC
    dnx_jr_mode_mpls_push_profile_t *push_profile = (dnx_jr_mode_mpls_push_profile_t *) data;
#endif
    SW_STATE_PRETTY_PRINT_INIT_VARIABLES();

    SW_STATE_PRETTY_PRINT_ADD_LINE(SW_STATE_PRETTY_PRINT_TYPE_UINT8, "ttl_model", push_profile->ttl_model, NULL, NULL);
    SW_STATE_PRETTY_PRINT_ADD_LINE(SW_STATE_PRETTY_PRINT_TYPE_UINT8, "exp_model", push_profile->exp_model, NULL, NULL);
    SW_STATE_PRETTY_PRINT_ADD_LINE(SW_STATE_PRETTY_PRINT_TYPE_UINT8, "has_cw", push_profile->has_cw, NULL, NULL);
    SW_STATE_PRETTY_PRINT_ADD_LINE(SW_STATE_PRETTY_PRINT_TYPE_UINT8, "ttl", push_profile->ttl, NULL, NULL);
    SW_STATE_PRETTY_PRINT_ADD_LINE(SW_STATE_PRETTY_PRINT_TYPE_UINT8, "exp", push_profile->exp, NULL, NULL);
    SW_STATE_PRETTY_PRINT_ADD_LINE(SW_STATE_PRETTY_PRINT_TYPE_UINT8, "remark_profile",
                                   push_profile->remark_profile, NULL, NULL);
    SW_STATE_PRETTY_PRINT_ADD_LINE(SW_STATE_PRETTY_PRINT_TYPE_UINT8, "add_entropy_label",
                                   push_profile->add_entropy_label, NULL, NULL);
    SW_STATE_PRETTY_PRINT_ADD_LINE(SW_STATE_PRETTY_PRINT_TYPE_UINT8, "add_entropy_label_indicator",
                                   push_profile->add_entropy_label_indicator, NULL, NULL);
    SW_STATE_PRETTY_PRINT_ADD_LINE(SW_STATE_PRETTY_PRINT_TYPE_UINT8, "esem_command_profile",
                                   push_profile->esem_command_profile, NULL, NULL);

    SW_STATE_PRETTY_PRINT_FINISH();

}
/**
 * \brief 
 * allocate resource pool for all MPLS profiles
 */
shr_error_e
dnx_algo_mpls_init(
    int unit)
{
    dnx_algo_template_create_data_t template_data;
    dnx_algo_res_create_data_t res_data;

    SHR_FUNC_INIT_VARS(unit);

    SHR_IF_ERR_EXIT(algo_mpls_db.init(unit));
    /*
     * Init MPLS Termination Profile
     */
    sal_memset(&template_data, 0, sizeof(dnx_algo_template_create_data_t));
    template_data.flags = 0;
    template_data.data_size = sizeof(dnx_mpls_termination_profile_t);
    template_data.first_profile = 0;
    SHR_IF_ERR_EXIT(dnx_algo_lif_mngr_nof_inlifs_get(unit, NULL, &template_data.max_references));
    template_data.nof_profiles = dnx_data_mpls.general.nof_mpls_termination_profiles_get(unit);
    sal_strncpy(template_data.name, DNX_MPLS_TERMINATION_PROFILE_TEMPLATE, DNX_ALGO_TEMPLATE_MNGR_MAX_NAME_LENGTH - 1);

    SHR_IF_ERR_EXIT(algo_mpls_db.termination_profile.create(unit, &template_data, NULL));
    /*
     * Init MPLS Push Profile
     */
    if (dnx_data_mpls.general.nof_mpls_push_profiles_get(unit) > 0)
    {
        sal_memset(&template_data, 0, sizeof(dnx_algo_template_create_data_t));
        template_data.flags = 0;
        template_data.data_size = sizeof(dnx_jr_mode_mpls_push_profile_t);
        template_data.first_profile = 1;
        template_data.max_references = SAL_INT32_MAX;
        template_data.nof_profiles = dnx_data_mpls.general.nof_mpls_push_profiles_get(unit);
        sal_strncpy(template_data.name, DNX_MPLS_PUSH_PROFILE_TEMPLATE, DNX_ALGO_TEMPLATE_MNGR_MAX_NAME_LENGTH - 1);

        SHR_IF_ERR_EXIT(algo_mpls_db.push_profile.create(unit, &template_data, NULL));
    }
    /*
     * Init MPLS Label range resource manager
     */
    sal_memset(&res_data, 0, sizeof(res_data));
    res_data.advanced_algorithm = DNX_ALGO_RESOURCE_ADVANCED_ALGORITHM_BASIC;
    res_data.first_element = 1;
    res_data.nof_elements = dnx_data_mpls.general.nof_mpls_range_elements_get(unit) - 1;
    sal_strncpy(res_data.name, "MPLS Label Range Profile", sizeof(res_data.name));
    SHR_IF_ERR_EXIT(algo_mpls_db.label_range_profile.create(unit, &res_data, NULL));
exit:
    SHR_FUNC_EXIT;
}

/*
 * } 
 */
