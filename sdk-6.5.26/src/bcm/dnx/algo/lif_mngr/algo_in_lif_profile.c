/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2022 Broadcom Inc. All rights reserved.
 */

/** \file algo_in_lif_profile.c
 *
 * Wrapper functions for utilex_multi_set.
 *
 */
#ifdef BSL_LOG_MODULE
#error "BSL_LOG_MODULE redefined"
#endif
#define BSL_LOG_MODULE BSL_LS_BCMDNX_TEMPLATEMNGR
/**
* INCLUDE FILES:
* {
*/
#include <soc/sand/shrextend/shrextend_debug.h>
#include <soc/dnx/dnx_data/auto_generated/dnx_data_l3.h>
#include <soc/dnx/dnx_data/auto_generated/dnx_data_lif.h>
#include <bcm_int/dnx/lif/in_lif_profile.h>
#include <soc/dnx/dbal/dbal.h>
#include <shared/util.h>
#include <bcm_int/dnx/algo/l3/source_address_table_allocation.h>
#include <bcm_int/dnx/l3/l3.h>
#include <bcm_int/dnx/algo/lif_mngr/algo_in_lif_profile.h>
#include <soc/dnx/dnx_data/auto_generated/dnx_data_qos.h>
#include <soc/dnx/dnx_data/auto_generated/dnx_data_headers.h>
#include <soc/dnx/dnx_data/auto_generated/dnx_data_l2.h>
#include <soc/dnx/dnx_data/auto_generated/dnx_data_switch.h>
#include <soc/dnx/dnx_data/auto_generated/dnx_data_trap.h>
#include <bcm_int/dnx/algo/swstate/auto_generated/access/algo_l3_access.h>
#include <bcm_int/dnx/algo/swstate/auto_generated/access/algo_lif_profile_access.h>
#include <soc/dnx/swstate/auto_generated/access/switch_access.h>
#include <soc/dnxc/swstate/dnx_sw_state_dump.h>
#include <soc/dnx/dnx_data/auto_generated/dnx_data_device.h>
#include <bcm_int/dnx/algo/lif_mngr/lif_mngr.h>
#include <bcm_int/dnx/rx/rx_trap.h>
#include <bcm_int/dnx/lif/cs_in_lif_profile_based_on_stages.h>
#include <soc/dnx/dnx_data/auto_generated/dnx_data_dev_init.h>

/**
 * \see
 *   algo_in_lif_profile.h file
 */
void
dnx_algo_in_lif_profile_l3_enablers_profile_print_entry_cb(
    int unit,
    const void *data)
{
#ifdef DNX_SW_STATE_DIAGNOSTIC
    uint32 *enablers_vector = (uint32 *) data;
    uint32 nof_arr_members = BITS2WORDS(dnx_data_l3.routing_enablers.routing_enable_vector_length_get(unit));
#endif
    SW_STATE_PRETTY_PRINT_INIT_VARIABLES();
    SW_STATE_PRETTY_PRINT_ADD_ARR(SW_STATE_PRETTY_PRINT_TYPE_UINT32, "enabler vector", enablers_vector, NULL, "0x%08x",
                                  nof_arr_members);
    SW_STATE_PRETTY_PRINT_FINISH();
}

/**
 * \see
 *   algo_in_lif_profile.h file
 */
void
dnx_algo_in_lif_profile_in_lif_lb_profile_print_entry_cb(
    int unit,
    const void *data)
{
#ifdef DNX_SW_STATE_DIAGNOSTIC
    uint32 *layers_disable = (uint32 *) data;
#endif
    SW_STATE_PRETTY_PRINT_INIT_VARIABLES();
    SW_STATE_PRETTY_PRINT_ADD_LINE(SW_STATE_PRETTY_PRINT_TYPE_UINT32, "Layers Disable", *layers_disable, NULL,
                                   "0x%08x");

    SW_STATE_PRETTY_PRINT_FINISH();
}

/**
 * \see
 *   algo_in_lif_profile.h file
 */
void
dnx_algo_in_lif_profile_print_entry_cb(
    int unit,
    const void *data)
{
#ifdef DNX_SW_STATE_DIAGNOSTIC
    in_lif_template_data_t *template_data = (in_lif_template_data_t *) data;
#endif
    SW_STATE_PRETTY_PRINT_INIT_VARIABLES();

    SW_STATE_PRETTY_PRINT_ADD_LINE(SW_STATE_PRETTY_PRINT_TYPE_UINT8, "routing enablers profile",
                                   template_data->routing_enablers_profile, NULL, "0x%08x");
    SW_STATE_PRETTY_PRINT_ADD_LINE(SW_STATE_PRETTY_PRINT_TYPE_UINT8, "rpf mode", template_data->rpf_mode,
                                   NULL, "0x%08x");
    SW_STATE_PRETTY_PRINT_ADD_LINE(SW_STATE_PRETTY_PRINT_TYPE_UINT8, "cs_vtt_in_lif_profile",
                                   template_data->cs_vtt_in_lif_profile, NULL, "0x%08x");
    SW_STATE_PRETTY_PRINT_ADD_LINE(SW_STATE_PRETTY_PRINT_TYPE_UINT8, "cs_vtt_to_flp_in_lif_profile",
                                   template_data->cs_vtt_to_flp_in_lif_profile, NULL, "0x%08x");
    SW_STATE_PRETTY_PRINT_ADD_LINE(SW_STATE_PRETTY_PRINT_TYPE_UINT8, "cs_flp_in_lif_profile",
                                   template_data->cs_flp_in_lif_profile, NULL, "0x%08x");
    SW_STATE_PRETTY_PRINT_ADD_LINE(SW_STATE_PRETTY_PRINT_TYPE_UINT8, "da_not_found_dst_profile",
                                   template_data->da_not_found_dst_profile, NULL, "0x%08x");
    SW_STATE_PRETTY_PRINT_ADD_LINE(SW_STATE_PRETTY_PRINT_TYPE_UINT32, "mact_mode",
                                   template_data->mact_mode, NULL, NULL);
    SW_STATE_PRETTY_PRINT_ADD_LINE(SW_STATE_PRETTY_PRINT_TYPE_UINT32, "l2_v4_mc_fwd_type",
                                   template_data->l2_v4_mc_fwd_type, NULL, NULL);
    SW_STATE_PRETTY_PRINT_ADD_LINE(SW_STATE_PRETTY_PRINT_TYPE_UINT32, "l2_v6_mc_fwd_type",
                                   template_data->l2_v6_mc_fwd_type, NULL, NULL);
    SW_STATE_PRETTY_PRINT_ADD_LINE(SW_STATE_PRETTY_PRINT_TYPE_UINT32, "oam_default_profile",
                                   template_data->oam_default_profile, NULL, NULL);
    if (dnx_data_trap.ingress.protocol_traps_init_mode_get(unit) == DNX_RX_TRAP_PROTOCOL_MODE_IN_LIF)
    {
        SW_STATE_PRETTY_PRINT_ADD_LINE(SW_STATE_PRETTY_PRINT_TYPE_UINT8, "icmpv6_trap_profile",
                                       template_data->protocol_trap_profiles.icmpv6_trap_profile, NULL, NULL);
        SW_STATE_PRETTY_PRINT_ADD_LINE(SW_STATE_PRETTY_PRINT_TYPE_UINT8, "icmpv4_trap_profile",
                                       template_data->protocol_trap_profiles.icmpv4_trap_profile, NULL, NULL);
        SW_STATE_PRETTY_PRINT_ADD_LINE(SW_STATE_PRETTY_PRINT_TYPE_UINT8, "l2cp_trap_profile",
                                       template_data->protocol_trap_profiles.l2cp_trap_profile, NULL, NULL);
        SW_STATE_PRETTY_PRINT_ADD_LINE(SW_STATE_PRETTY_PRINT_TYPE_UINT8, "arp_trap_profile",
                                       template_data->protocol_trap_profiles.arp_trap_profile, NULL, NULL);
        SW_STATE_PRETTY_PRINT_ADD_LINE(SW_STATE_PRETTY_PRINT_TYPE_UINT8, "igmp_trap_profile",
                                       template_data->protocol_trap_profiles.igmp_trap_profile, NULL, NULL);
        SW_STATE_PRETTY_PRINT_ADD_LINE(SW_STATE_PRETTY_PRINT_TYPE_UINT8, "dhcp_trap_profile",
                                       template_data->protocol_trap_profiles.dhcp_trap_profile, NULL, NULL);
        SW_STATE_PRETTY_PRINT_ADD_LINE(SW_STATE_PRETTY_PRINT_TYPE_UINT8, "non_auth_8021x_trap_profile",
                                       template_data->protocol_trap_profiles.non_auth_8021x_trap_profile, NULL, NULL);
    }
    if (dnx_data_switch.load_balancing.hashing_selection_per_layer_get(unit))
    {
        SW_STATE_PRETTY_PRINT_ADD_LINE(SW_STATE_PRETTY_PRINT_TYPE_UINT8, "in_lif_lb_profile",
                                       template_data->in_lif_lb_profile, NULL, NULL);
    }
    if (dnx_data_lif.in_lif.feature_get(unit, dnx_data_lif_in_lif_use_in_lif_profile_range_allocation_per_pmf_profiles))
    {
        SW_STATE_PRETTY_PRINT_ADD_LINE(SW_STATE_PRETTY_PRINT_TYPE_INT, "field_class_id",
                                       template_data->field_class_id, NULL, NULL);
    }
    if (dnx_data_lif.in_lif.feature_get(unit, dnx_data_lif_in_lif_use_mapped_in_lif_profile))
    {
        SW_STATE_PRETTY_PRINT_ADD_LINE(SW_STATE_PRETTY_PRINT_TYPE_UINT8, "in_lif_orientation",
                                       template_data->in_lif_orientation, NULL, NULL);
        SW_STATE_PRETTY_PRINT_ADD_LINE(SW_STATE_PRETTY_PRINT_TYPE_UINT8, "lif_same_interface_mode",
                                       template_data->lif_same_interface_mode, NULL, NULL);
        SW_STATE_PRETTY_PRINT_ADD_LINE(SW_STATE_PRETTY_PRINT_TYPE_UINT8, "ive_outer_qos_dp_profile",
                                       template_data->ive_outer_qos_dp_profile, NULL, NULL);
        SW_STATE_PRETTY_PRINT_ADD_LINE(SW_STATE_PRETTY_PRINT_TYPE_UINT8, "ive_inner_qos_dp_profile",
                                       template_data->ive_inner_qos_dp_profile, NULL, NULL);
    }
    SW_STATE_PRETTY_PRINT_FINISH();
}

/**
 * \see
 *   algo_in_lif_profile.h file
 */
void
dnx_algo_in_lif_flood_profile_print_entry_cb(
    int unit,
    const void *data)
{
    dnx_default_frwrd_dst_t *template_data = NULL;
    dnx_default_frwrd_dst_t *default_dst_data = (dnx_default_frwrd_dst_t *) data;

    template_data = &default_dst_data[DBAL_ENUM_FVAL_DESTINATION_DISTRIBUTION_TYPE_UC];

    DNX_SW_STATE_PRINT(unit,
                       "\nDefault destinations for unknown unicast packets: destination = 0x%08x, offset = 0x%08x, add_vsi_dst = 0x%x",
                       template_data->destination, template_data->offset, template_data->add_vsi_dst);

    template_data = &default_dst_data[DBAL_ENUM_FVAL_DESTINATION_DISTRIBUTION_TYPE_MC];

    DNX_SW_STATE_PRINT(unit,
                       "\nDefault destinations for unknown multicast packets: destination = 0x%08x, offset = 0x%08x, add_vsi_dst = 0x%x",
                       template_data->destination, template_data->offset, template_data->add_vsi_dst);

    template_data = &default_dst_data[DBAL_ENUM_FVAL_DESTINATION_DISTRIBUTION_TYPE_BC];

    DNX_SW_STATE_PRINT(unit,
                       "\nDefault destinations for broadcast packets: destination = 0x%08x, offset = 0x%08x, add_vsi_dst = 0x%x",
                       template_data->destination, template_data->offset, template_data->add_vsi_dst);

    return;
}

/**
 * \brief
 * Create Template Manager for L3 enablers_vector, with the following properties:
 * * entries : each entry is an enablers vector, more than one RIF profile can point to it.
 * * key : the key represents ETH_RIF_Profile
 * * the template has a default profile which will always exist in index 0.
 * * max references per profile is number of keys + 1,this is in order to always keep the default profile.
 *   this way no matter how many template exchanges we will do, the default profile won't reach 0 references.
 * * according to the user's flags BCM_L3_INGRESS_ROUTE_DISABLE* the profile will be decided and allocated
 *   (in case it doesn't exist already).
 * * a profile will be deleted when the API bcm_l3_intf_delete will be called and no more references
 *   exist for this profile
 * \par DIRECT_INPUT:
 *   \param [in] unit - the unit number
 * \par INDIRECT INPUT:
 *   * The definition DNX_ALGO_L3_ENABLERS_VECTOR_TABLE_RESOURCE
 *   * The definition DNX_ALGO_TEMPLATE_CREATE_USE_DEFAULT_PROFILE
 *   * The definition NOF_ENTRIES_ENABLERS_VECTOR
 * \par DIRECT OUTPUT:
 *   Non-zero in case of an error.
 * \par INDIRECT OUTPUT
 *   * The new created template
 * \remark
 *   * None
 * \see
 *   * None
 */
static shr_error_e
dnx_algo_l3_enablers_vector_template_create(
    int unit)
{
    dnx_algo_template_create_data_t data;
    int nof_inlif1_lifs, nof_inlif2_lifs, nof_cores, nof_references;
    uint32 initial_enablers_vector[BITS2WORDS(DNX_DATA_MAX_L3_ROUTING_ENABLERS_ROUTING_ENABLE_VECTOR_LENGTH)];

    SHR_FUNC_INIT_VARS(unit);
    SHR_IF_ERR_EXIT(dnx_enablers_vectors_initial_set(unit, initial_enablers_vector));
    /*
     * enablers vector table contains 32 entries, first entry will be used as default entry with value:
     * 0x0000000000000004 (can't be deleted), the rest of the entries will be assigned dynamically
     *
     */
    SHR_IF_ERR_EXIT(dnx_algo_lif_mngr_max_inlif_id_get(unit, &nof_inlif1_lifs, &nof_inlif2_lifs));
    nof_cores = dnx_data_device.general.nof_cores_get(unit);
    nof_references = nof_inlif1_lifs + nof_inlif2_lifs * nof_cores + dnx_data_l3.rif.nof_rifs_get(unit);
    sal_memset(&data, 0, sizeof(dnx_algo_template_create_data_t));
    data.data_size = ROUTING_ENABLERS_VECTOR_SIZE_IN_UINT32(unit) * sizeof(uint32);
    data.default_data = initial_enablers_vector;
    data.default_profile = 0;
    data.first_profile = 0;
    data.flags = DNX_ALGO_TEMPLATE_CREATE_USE_DEFAULT_PROFILE;
    data.nof_profiles = NOF_ENTRIES_ENABLERS_VECTOR;
    data.max_references = nof_references;
    sal_strncpy(data.name, DNX_ALGO_IN_LIF_PROFILE_L3_ENABLERS, DNX_ALGO_TEMPLATE_MNGR_MAX_NAME_LENGTH - 1);

    SHR_IF_ERR_EXIT(algo_lif_profile_db.in_lif_profile_l3_enablers.create(unit, &data, NULL));

exit:
    SHR_FUNC_EXIT;
}

/**
 * \brief
 * Create Template Manager for LB LIF Profile enablers:
 */
static shr_error_e
dnx_algo_lif_lb_profile_template_create(
    int unit)
{
    dnx_algo_template_create_data_t data;
    int default_data;
    SHR_FUNC_INIT_VARS(unit);

    default_data = DNX_IN_LIF_LB_PROFILE_DISABLERS_FLAGS_DEFAULT;
    sal_memset(&data, 0, sizeof(dnx_algo_template_create_data_t));
    data.data_size = sizeof(uint32);
    data.default_data = &default_data;
    data.default_profile = 0;
    data.first_profile = 0;
    data.flags = DNX_ALGO_TEMPLATE_CREATE_USE_DEFAULT_PROFILE;
    data.nof_profiles = 4;
    data.max_references = dnx_data_l3.rif.nof_rifs_get(unit);
    sal_strncpy(data.name, DNX_ALGO_IN_LIF_LB_PROFILE_ENABLERS, DNX_ALGO_TEMPLATE_MNGR_MAX_NAME_LENGTH - 1);

    SHR_IF_ERR_EXIT(algo_lif_profile_db.in_lif_lb_profile_disablers_flags.create(unit, &data, NULL));

exit:
    SHR_FUNC_EXIT;
}
/** } **/

/**
 * \brief
 *   Create Template Manager for in_lif_profile, the template is created with the following properties:
 * * entries : each entry represents LIF properties, more than one LIF profile can point to it.
 * * key : the key represents IN_LIF_PROFILE
 * * the template has a default profile which will always exist in index 0.
 * * max references per profile = number of keys
 * * according to the user's flags the profile will be decided and allocated
 *   (in case it doesn't exist already).
 * \par DIRECT INPUT
 *   \param [in] unit -
 *     the unit number
 * \par INDIRECT INPUT
 *   * The definition DEFAULT_ROUTING_ENABLERS_PROFILE
 *   * The definition DEFAULT_RPF_MODE
 *   * The definition DNX_ALGO_TEMPLATE_CREATE_USE_DEFAULT_PROFILE
 *   * The definition DNX_ALGO_IN_LIF_PROFILE
 * \par DIRECT OUTPUT
 *   \retval Zero if no error was detected
 *   \retval Negative if error was detected. See \ref shr_error_e
 * \par INDIRECT OUTPUT
 *   * The new created template
 * \see
 *   * None
 */
static shr_error_e
dnx_algo_in_lif_profile_template_create(
    int unit)
{
    uint8 ipv4_fwd_type_sel;
    uint8 ipv6_fwd_type_sel;
    dnx_algo_template_create_data_t data;
    in_lif_template_data_t default_template_data;
    int nof_ingress_profile_bits = 0, system_headers_mode = 0, preserve_dscp_number_of_bits =
        0, same_interface_number_of_bits = 0;
    uint32 in_lif_profile_pmf_bits_base = 0;
    SHR_FUNC_INIT_VARS(unit);

    system_headers_mode = dnx_data_headers.system_headers.system_headers_mode_get(unit);
    if (JR_COMP_MODE_SAME_IF_MODE_1B_ALLOCATION
        (dnx_data_lif.in_lif.in_lif_profile_allocate_same_interface_mode_get(unit)))
    {
        same_interface_number_of_bits = 1;
    }
    else if (JR_COMP_MODE_SAME_IF_MODE_2B_ALLOCATION
             (dnx_data_lif.in_lif.in_lif_profile_allocate_same_interface_mode_get(unit)))
    {
        same_interface_number_of_bits = 2;
    }

    if (JR_COMP_MODE_SAME_PRESERVE_DSCP_PER_INLIF(dnx_data_qos.qos.routing_dscp_preserve_mode_get(unit)))
    {
        preserve_dscp_number_of_bits = 1;
    }
    /*
     * initialize the default template data with default values
     */
    sal_memset(&default_template_data, 0, sizeof(default_template_data));
    default_template_data.routing_enablers_profile = DEFAULT_ROUTING_ENABLERS_PROFILE;
    default_template_data.rpf_mode = DEFAULT_RPF_MODE;
    default_template_data.cs_vtt_in_lif_profile = DNX_IN_LIF_PROFILE_CS_DEFAULT;
    default_template_data.cs_vtt_to_flp_in_lif_profile = DNX_IN_LIF_PROFILE_CS_DEFAULT;
    default_template_data.cs_flp_in_lif_profile = DNX_IN_LIF_PROFILE_CS_DEFAULT;
    if (dnx_data_dev_init.general.feature_get(unit, dnx_data_dev_init_general_application_v1))
    {
        default_template_data.mact_mode = DBAL_ENUM_FVAL_VSI_PROFILE_BRIDGE_SVL;
    }
    SHR_IF_ERR_EXIT(switch_db.l3mcastl2_ipv4_fwd_type.get(unit, &ipv4_fwd_type_sel));
    SHR_IF_ERR_EXIT(switch_db.l3mcastl2_ipv6_fwd_type.get(unit, &ipv6_fwd_type_sel));
    default_template_data.l2_v4_mc_fwd_type = ipv4_fwd_type_sel;
    default_template_data.l2_v6_mc_fwd_type = ipv6_fwd_type_sel;
    default_template_data.oam_default_profile = DEFAULT_OAM_PROFILE;
    default_template_data.protocol_trap_profiles.arp_trap_profile = DEFAULT_TRAP_PROFILES_DATA;
    default_template_data.protocol_trap_profiles.dhcp_trap_profile = DEFAULT_TRAP_PROFILES_DATA;
    default_template_data.protocol_trap_profiles.icmpv4_trap_profile = DEFAULT_TRAP_PROFILES_DATA;
    default_template_data.protocol_trap_profiles.icmpv6_trap_profile = DEFAULT_TRAP_PROFILES_DATA;
    default_template_data.protocol_trap_profiles.igmp_trap_profile = DEFAULT_TRAP_PROFILES_DATA;
    default_template_data.protocol_trap_profiles.l2cp_trap_profile = DEFAULT_TRAP_PROFILES_DATA;
    default_template_data.protocol_trap_profiles.non_auth_8021x_trap_profile = DEFAULT_TRAP_PROFILES_DATA;
    default_template_data.in_lif_lb_profile = DEFAULT_IN_LIF_LB_PROFILE;
    if (dnx_data_lif.in_lif.feature_get(unit, dnx_data_lif_in_lif_use_in_lif_profile_range_allocation_per_pmf_profiles))
    {
        default_template_data.field_class_id = DEFAULT_IN_LIF_FIELD_CLASS_ID;
    }
    if (dnx_data_lif.in_lif.feature_get(unit, dnx_data_lif_in_lif_use_mapped_in_lif_profile))
    {
        default_template_data.in_lif_orientation = DEFAULT_IN_LIF_ORIENTATION;
        default_template_data.ive_outer_qos_dp_profile = DEFAULT_IN_LIF_OUTER_DP_PROFILE;
        default_template_data.ive_inner_qos_dp_profile = DEFAULT_IN_LIF_INNER_DP_PROFILE;
        default_template_data.lif_same_interface_mode = DEFAULT_IN_LIF_SAME_INTERFACE_MODE;
    }
    /*
     * initialize the templates data/properties
     */
    sal_memset(&data, 0, sizeof(dnx_algo_template_create_data_t));
    data.data_size = sizeof(default_template_data);
    data.default_data = &default_template_data;
    data.default_profile = 0;
    data.first_profile = 0;
    if (dnx_data_lif.in_lif.feature_get(unit, dnx_data_lif_in_lif_use_in_lif_profile_range_allocation_per_pmf_profiles))
    {
        data.flags = DNX_ALGO_TEMPLATE_CREATE_USE_DEFAULT_PROFILE | DNX_ALGO_TEMPLATE_CREATE_USE_ADVANCED_ALGORITHM;
        data.advanced_algorithm = DNX_ALGO_TEMPLATE_ADVANCED_ALGORITHM_SMART_TEMPLATE;
    }
    else
    {
        data.flags = DNX_ALGO_TEMPLATE_CREATE_USE_DEFAULT_PROFILE;
    }

    /** Calculate the number of ingress bits in in_lif_profile*/
    if (system_headers_mode == DBAL_ENUM_FVAL_SYSTEM_HEADERS_MODE_JERICHO2_MODE)
    {
        nof_ingress_profile_bits = NOF_IN_LIF_PROFILE_INGRESS_BITS;
    }
    else if ((system_headers_mode == DBAL_ENUM_FVAL_SYSTEM_HEADERS_MODE_JERICHO_MODE)
             && !dnx_data_lif.feature.feature_get(unit, dnx_data_lif_feature_iop_mode_orientation_selection))
    {
       /** If policer is enabled, it is allocated only 1 bit, IVE_OUTER_QOS_DP_PROFILE_NOF_BITS should not be calculated */
        nof_ingress_profile_bits =
            NOF_IN_LIF_PROFILE_BITS - IN_LIF_PROFILE_PMF_RESERVED_NOF_BITS - (IN_LIF_ORIENTATION_NOF_BITS +
                                                                              IVE_INNER_QOS_DP_PROFILE_NOF_BITS +
                                                                              same_interface_number_of_bits +
                                                                              preserve_dscp_number_of_bits) -
            NATIVE_INDEXED_MODE_NOF_BITS;
    }
    else
    {
        /** Relevant for devices with fixed size of egress bits */
        nof_ingress_profile_bits =
            (NOF_IN_LIF_PROFILE_BITS - IN_LIF_PROFILE_EGRESS_NOF_BITS_JR_MODE - IN_LIF_PROFILE_PMF_RESERVED_NOF_BITS -
             NATIVE_INDEXED_MODE_NOF_BITS);
    }

    if (dnx_data_lif.in_lif.feature_get(unit, dnx_data_lif_in_lif_use_in_lif_profile_range_allocation_per_pmf_profiles))
    {
        /*
         * The number of profiles in use is equal to the number of profiles per range (according to nof_ingress_profile_bits) multiplied by the number of possible PMF profiles
         * In case that the ranges are nonconsecutive, it is needed to calculate also the profiles that are left between the dedicated by the pmf bits ranges
         */
        if (IN_LIF_PROFILE_PMF_RESERVED_NOF_BITS > 0)
        {
            SHR_IF_ERR_EXIT(utilex_bitstream_set_bit_range
                            (&in_lif_profile_pmf_bits_base, IN_LIF_PROFILE_PMF_RESERVED_START_BIT,
                             IN_LIF_PROFILE_PMF_RESERVED_START_BIT + IN_LIF_PROFILE_PMF_RESERVED_NOF_BITS - 1));
        }
        data.nof_profiles = (1 << nof_ingress_profile_bits) + in_lif_profile_pmf_bits_base;
    }
    else
    {
        data.nof_profiles = (1 << nof_ingress_profile_bits);
    }
    if ((nof_ingress_profile_bits == 0) || (data.nof_profiles > (1 << NOF_IN_LIF_PROFILE_BITS)))
    {
        LOG_INFO_EX(BSL_LOG_MODULE, "%d: Outer DP profile: %d, Inner DP profile: %d, Orientation: %d\n", unit,
                    IVE_OUTER_QOS_DP_PROFILE_NOF_BITS, IVE_INNER_QOS_DP_PROFILE_NOF_BITS, IN_LIF_ORIENTATION_NOF_BITS);
        LOG_INFO_EX(BSL_LOG_MODULE, "%d: Same interface: %d,   Index mode: %d,       PMF reserved: %d\n", unit,
                    SAME_INTERFACE_MODE_NOF_BITS, NATIVE_INDEXED_MODE_NOF_BITS, IN_LIF_PROFILE_PMF_RESERVED_NOF_BITS);
        SHR_ERR_EXIT(_SHR_E_RESOURCE, "Too more SOC property enabled\n");
    }
    SHR_IF_ERR_EXIT(dnx_algo_lif_mngr_max_inlif_id_get(unit, &data.max_references, &data.max_references));
    sal_strncpy(data.name, DNX_ALGO_IN_LIF_PROFILE, DNX_ALGO_TEMPLATE_MNGR_MAX_NAME_LENGTH - 1);

    SHR_IF_ERR_EXIT(algo_lif_profile_db.in_lif_profile.create(unit, &data, NULL));

exit:
    SHR_FUNC_EXIT;
}

/**
 * \brief
 *   Create Template Manager for eth_rif_profile, the template is created with the following properties:
 * * entries : each entry represents ETH_RIF properties, more than one ETH_RIF profile can point to it.
 * * key : the key represents ETH_RIF_PROFILE
 * * the template has a default profile which will always exist in index 0.
 * * max references per profile = number of keys
 * * according to the user's flags the profile will be decided and allocated
 *   (in case it doesnt exist already).
 * \par DIRECT INPUT
 *   \param [in] unit -
 *     the unit number
 * \par INDIRECT INPUT
 *   * The definition DEFAULT_ROUTING_ENABLERS_PROFILE
 *   * The definition DEFAULT_RPF_MODE
 *   * The definition DNX_ALGO_TEMPLATE_CREATE_USE_DEFAULT_PROFILE
 *   * The definition DNX_ALGO_ETH_RIF_PROFILE
 * \par DIRECT OUTPUT
 *   \retval Zero if no error was detected
 *   \retval Negative if error was detected. See \ref shr_error_e
 * \par INDIRECT OUTPUT
 *   * The new created template
 * \see
 *   * None
 */
static shr_error_e
dnx_algo_eth_rif_profile_template_create(
    int unit)
{
    uint8 ipv4_fwd_type_sel;
    uint8 ipv6_fwd_type_sel;
    dnx_algo_template_create_data_t data;
    in_lif_template_data_t default_template_data;
    SHR_FUNC_INIT_VARS(unit);

    /*
     * initialize the default template data with default values
     */
    sal_memset(&default_template_data, 0, sizeof(default_template_data));
    default_template_data.routing_enablers_profile = DEFAULT_ROUTING_ENABLERS_PROFILE;
    default_template_data.rpf_mode = DEFAULT_RPF_MODE;
    default_template_data.cs_vtt_in_lif_profile = DNX_IN_LIF_PROFILE_CS_DEFAULT;
    default_template_data.cs_vtt_to_flp_in_lif_profile = DNX_IN_LIF_PROFILE_CS_DEFAULT;
    default_template_data.cs_flp_in_lif_profile = DNX_IN_LIF_FWD_PROFILE_CS_DEFAULT;
    if (dnx_data_dev_init.general.feature_get(unit, dnx_data_dev_init_general_application_v1))
    {
        default_template_data.mact_mode = DBAL_ENUM_FVAL_VSI_PROFILE_BRIDGE_SVL;
    }
    SHR_IF_ERR_EXIT(switch_db.l3mcastl2_ipv4_fwd_type.get(unit, &ipv4_fwd_type_sel));
    SHR_IF_ERR_EXIT(switch_db.l3mcastl2_ipv6_fwd_type.get(unit, &ipv6_fwd_type_sel));
    default_template_data.l2_v4_mc_fwd_type = ipv4_fwd_type_sel;
    default_template_data.l2_v6_mc_fwd_type = ipv6_fwd_type_sel;
    default_template_data.oam_default_profile = DEFAULT_OAM_PROFILE;
    default_template_data.protocol_trap_profiles.arp_trap_profile = DEFAULT_TRAP_PROFILES_DATA;
    default_template_data.protocol_trap_profiles.dhcp_trap_profile = DEFAULT_TRAP_PROFILES_DATA;
    default_template_data.protocol_trap_profiles.icmpv4_trap_profile = DEFAULT_TRAP_PROFILES_DATA;
    default_template_data.protocol_trap_profiles.icmpv6_trap_profile = DEFAULT_TRAP_PROFILES_DATA;
    default_template_data.protocol_trap_profiles.igmp_trap_profile = DEFAULT_TRAP_PROFILES_DATA;
    default_template_data.protocol_trap_profiles.l2cp_trap_profile = DEFAULT_TRAP_PROFILES_DATA;
    default_template_data.protocol_trap_profiles.non_auth_8021x_trap_profile = DEFAULT_TRAP_PROFILES_DATA;
    default_template_data.in_lif_lb_profile = DEFAULT_IN_LIF_LB_PROFILE;
    if (dnx_data_lif.
        in_lif.feature_get(unit, dnx_data_lif_in_lif_use_eth_rif_profile_range_allocation_per_pmf_profiles))
    {
        default_template_data.field_class_id = DEFAULT_IN_LIF_FIELD_CLASS_ID;
    }
    /*
     * initialize the templates data/properties
     */
    sal_memset(&data, 0, sizeof(dnx_algo_template_create_data_t));
    data.data_size = sizeof(default_template_data);
    data.default_data = &default_template_data;
    data.default_profile = 0;
    data.first_profile = 0;
    if (dnx_data_lif.
        in_lif.feature_get(unit, dnx_data_lif_in_lif_use_eth_rif_profile_range_allocation_per_pmf_profiles))
    {
        data.flags = DNX_ALGO_TEMPLATE_CREATE_USE_DEFAULT_PROFILE | DNX_ALGO_TEMPLATE_CREATE_USE_ADVANCED_ALGORITHM;
        data.advanced_algorithm = DNX_ALGO_TEMPLATE_ADVANCED_ALGORITHM_SMART_TEMPLATE;
    }
    else
    {
        data.flags = DNX_ALGO_TEMPLATE_CREATE_USE_DEFAULT_PROFILE;
    }
    if ((dnx_data_headers.system_headers.system_headers_mode_get(unit) ==
         DBAL_ENUM_FVAL_SYSTEM_HEADERS_MODE_JERICHO_MODE)
        && (dnx_data_qos.qos.routing_dscp_preserve_mode_get(unit) == 1))
    {
        if (dnx_data_lif.feature.feature_get(unit, dnx_data_lif_feature_iop_mode_orientation_selection))
        {
            data.nof_profiles = (1 << (NOF_ETH_RIF_PROFILE_INGRESS_BITS - IN_LIF_PROFILE_EGRESS_NOF_BITS_JR_MODE));
        }
        else
        {
            data.nof_profiles = (1 << (NOF_ETH_RIF_PROFILE_INGRESS_BITS - IN_LIF_PROFILE_EGRESS_NOF_BITS_DSCP_MODE));
        }
    }
    else
    {
        if (dnx_data_lif.
            in_lif.feature_get(unit, dnx_data_lif_in_lif_use_eth_rif_profile_range_allocation_per_pmf_profiles))
        {
            /** The number of profiles is equal to the number of profiles per range (according to NOF_ETH_RIF_PROFILE_INGRESS_BITS) multiplied by the number of possible PMF profiles */
            data.nof_profiles = (1 << NOF_ETH_RIF_PROFILE_INGRESS_BITS) * (1 << ETH_RIF_PROFILE_PMF_RESERVED_NOF_BITS);
        }
        else
        {
            data.nof_profiles = (1 << NOF_ETH_RIF_PROFILE_INGRESS_BITS);
        }
    }
    SHR_IF_ERR_EXIT(dnx_algo_lif_mngr_max_inlif_id_get(unit, &data.max_references, &data.max_references));
    sal_strncpy(data.name, DNX_ALGO_ETH_RIF_PROFILE, DNX_ALGO_TEMPLATE_MNGR_MAX_NAME_LENGTH - 1);

    SHR_IF_ERR_EXIT(algo_lif_profile_db.eth_rif_profile.create(unit, &data, NULL));

exit:
    SHR_FUNC_EXIT;
}

/**
 * \brief -
 *  Create the template for inlif flood profile.
 *
 * \param [in] unit - The unit ID
 *
 * \return
 *   \retval  Negative if error was detected. See \ref shr_error_e
 *
 * \remark
 *   Create the template with number of profiles available defined in DNX.
 *   Meanwhile, the default in-lif-flood-profile is allocated and will be used
 *   everywhere.
 *
 * \see
 *   None.
 */
static shr_error_e
dnx_algo_in_lif_flood_profile_template_create(
    int unit)
{
    dnx_algo_template_create_data_t data;
    dnx_default_frwrd_dst_t default_frwrd_dst[DBAL_NOF_ENUM_DESTINATION_DISTRIBUTION_TYPE_VALUES];
    int nof_inlif1_lifs, nof_inlif2_lifs, nof_cores, nof_references;
    SHR_FUNC_INIT_VARS(unit);

    /*
     * initialize the default template data with default values
     */
    sal_memset(&default_frwrd_dst, 0, sizeof(default_frwrd_dst));
    default_frwrd_dst[DBAL_ENUM_FVAL_DESTINATION_DISTRIBUTION_TYPE_MC].add_vsi_dst = TRUE;
    default_frwrd_dst[DBAL_ENUM_FVAL_DESTINATION_DISTRIBUTION_TYPE_BC].add_vsi_dst = TRUE;
    default_frwrd_dst[DBAL_ENUM_FVAL_DESTINATION_DISTRIBUTION_TYPE_UC].add_vsi_dst = TRUE;

    /*
     * initialize the templates data/properties
     */
    SHR_IF_ERR_EXIT(dnx_algo_lif_mngr_max_inlif_id_get(unit, &nof_inlif1_lifs, &nof_inlif2_lifs));
    nof_cores = dnx_data_device.general.nof_cores_get(unit);
    nof_references = nof_inlif1_lifs + nof_inlif2_lifs * nof_cores;

    sal_memset(&data, 0, sizeof(dnx_algo_template_create_data_t));
    data.data_size = sizeof(default_frwrd_dst);
    data.default_data = default_frwrd_dst;
    data.default_profile = dnx_data_lif.in_lif.default_da_not_found_destination_profile_get(unit);
    data.first_profile = 0;
    data.flags = DNX_ALGO_TEMPLATE_CREATE_USE_DEFAULT_PROFILE;
    data.nof_profiles = dnx_data_lif.in_lif.nof_da_not_found_destination_profiles_get(unit);

    data.max_references = nof_references;
    sal_strncpy(data.name, DNX_ALGO_IN_LIF_DA_NOT_FOUND_DESTINATION, DNX_ALGO_TEMPLATE_MNGR_MAX_NAME_LENGTH - 1);

    SHR_IF_ERR_EXIT(algo_lif_profile_db.in_lif_da_not_found_destination.create(unit, &data, NULL));

exit:
    SHR_FUNC_EXIT;
}

/**
 * \see
 *   algo_in_lif_profile.h file
 */
shr_error_e
dnx_algo_in_lif_profile_init(
    int unit)
{
    SHR_FUNC_INIT_VARS(unit);

    SHR_IF_ERR_EXIT(algo_lif_profile_db.init(unit));
    /*
     * Initialize in_lif_profile_high template.
     */

    SHR_IF_ERR_EXIT(dnx_algo_in_lif_profile_template_create(unit));

    /*
     * Initialize l3 enablers vector template.
     */
    SHR_IF_ERR_EXIT(dnx_algo_l3_enablers_vector_template_create(unit));

    /*
     * Initialize lif_lb enablers vector template.
     */
    SHR_IF_ERR_EXIT(dnx_algo_lif_lb_profile_template_create(unit));

    /*
     * Initialize in_lif_profile_low template.
     */
    SHR_IF_ERR_EXIT(dnx_algo_eth_rif_profile_template_create(unit));

    /*
     * Initialize da_not_found_dst_profile template.
     */
    SHR_IF_ERR_EXIT(dnx_algo_in_lif_flood_profile_template_create(unit));

exit:
    SHR_FUNC_EXIT;
}

/**
 * }
 */
