/** \file in_lif_profile.c
 *
 *  in_lif profile management for DNX.
 */
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2021 Broadcom Inc. All rights reserved.
 */

#ifdef BSL_LOG_MODULE
#error "BSL_LOG_MODULE redefined"
#endif

#define BSL_LOG_MODULE BSL_LS_BCMDNX_LIF
/*
 * Include files
 * {
 */
#include <shared/shrextend/shrextend_debug.h>
#include <bcm_int/dnx/algo/lif_mngr/algo_in_lif_profile.h>
#include <bcm_int/dnx/lif/in_lif_profile.h>
#include <bcm_int/dnx/lif/cs_in_lif_profile_based_on_stages.h>
#include <soc/dnx/dnx_data/auto_generated/dnx_data_headers.h>
#include <soc/dnx/dbal/dbal.h>
#include <bcm/types.h>
#include <bcm_int/dnx/l3/l3.h>
#include <bcm_int/dnx/rx/rx_trap.h>
#include <bcm_int/dnx/port/port_pp.h>
#include <soc/dnx/dnx_data/auto_generated/dnx_data_lif.h>
#include <soc/dnx/dnx_data/auto_generated/dnx_data_l3.h>
#include <soc/dnx/dnx_data/auto_generated/dnx_data_l2.h>
#include <soc/dnx/dnx_data/auto_generated/dnx_data_trap.h>
#include <soc/dnx/dnx_data/auto_generated/dnx_data_qos.h>
#include <soc/dnx/dnx_data/auto_generated/dnx_data_pp.h>
#include <bcm_int/dnx/algo/swstate/auto_generated/access/algo_lif_profile_access.h>
#include <bcm_int/dnx/algo/swstate/auto_generated/access/vlan_access.h>
#include <soc/dnx/dnx_data/auto_generated/dnx_data_switch.h>
#include <soc/dnx/swstate/auto_generated/access/switch_access.h>
#include <soc/dnx/dnx_state_snapshot_manager.h>
#include "bcm_int/dnx/init/init_pp.h"
#include <soc/dnx/dnx_data/auto_generated/dnx_data_dev_init.h>

/*
 * }
 */

/*
 * Defines
 */
/*
 * defines the mask of the in_lif_profile bits that are used for the ingress attributes template manager
 */
#define IN_LIF_PROFILE_USED_BITS_MASK (NOF_IN_LIF_PROFILES - 1)
/*
 * defines the mask of the eth_rif_profile bits that are used for the ingress attributes template manager
 */
#define ETH_RIF_PROFILE_USED_BITS_MASK (NOF_ETH_RIF_PROFILES - 1)
/*
 * defines the mask of the eth_rif_profile bits that are used for the egress attributes
 */
#define ETH_RIF_EGRESS_PROFILE_MASK (1 << (ETH_RIF_PROFILE_EGRESS_RESERVED_NOF_BITS + ETH_RIF_PROFILE_PMF_RESERVED_NOF_BITS)) - 1
/*
 * defines the mask of the in_lif_profile bits that are used for the egress attributes
 */
#define IN_LIF_EGRESS_PROFILE_MASK (1 << (IN_LIF_PROFILE_EGRESS_RESERVED_NOF_BITS + IN_LIF_PROFILE_PMF_RESERVED_NOF_BITS)) - 1
/*
 * defines the offset of the in_lif_profile bit that is used for the IPMC bridge fallback
 */
#define IN_LIF_PROFILE_IPMC_BRIDGE_FALLBACK_BIT_OFFSET 3
/*
 * defines the mask of the in_lif_profile lsb, used for JR1
 */
#define IN_LIF_PROFILE_JR_MODE_LSB_MASK 1
/*
 * defines the mask of the in_lif_profile msb, used for JR1
 */
#define IN_LIF_PROFILE_JR_MODE_MSB_MASK 2
/*
 * defines the mask of the in_lif_profile in jr mode
 */
#define IN_LIF_PROFILE_JR_MODE_MAX_USED_BIT_MASK 3
/*
 * defines the mask of the ingress dscp preserve bits that are used for the egress dscp preserve
 */
#define IN_LIF_PROFILE_DSCP_PRESERVE_MASK 0x1

/** Calculate the offset in the routing enablers vector based on the HW value of the layer type */
#define DNX_ROUTING_ENABLER_ACTION_PROFILE_OFFSET_CALCULATE(unit, hw_enum) (hw_enum * dnx_data_l3.routing_enablers.routing_enable_nof_profiles_per_layer_get(unit) * dnx_data_l3.routing_enablers.routing_enable_indication_nof_bits_per_layer_get(unit))
/** Get the offset in the routing enablers vector based on the layer type */
#define DNX_IN_LIF_ROUTING_ENABLER_OFFSET_GET(unit, layer_type, offset) ((dnx_init_pp_layer_types_value_get(unit, layer_type)) * \
    (dnx_data_l3.routing_enablers.routing_enable_nof_profiles_per_layer_get(unit) * dnx_data_l3.routing_enablers.routing_enable_indication_nof_bits_per_layer_get(unit)) + offset)
/** Get the offset of the UC enabler in the routing enablers vector based on the layer type */
#define DNX_IN_LIF_ROUTING_UC_ENABLER_OFFSET_GET(unit, layer_type) DNX_IN_LIF_ROUTING_ENABLER_OFFSET_GET(unit, layer_type, 0)
/** Get the offset of the MC enabler in the routing enablers vector based on the layer type */
#define DNX_IN_LIF_ROUTING_MC_ENABLER_OFFSET_GET(unit, layer_type) DNX_IN_LIF_ROUTING_ENABLER_OFFSET_GET(unit, layer_type, dnx_data_l3.routing_enablers.routing_enable_indication_nof_bits_per_layer_get(unit))

/*
 * Eums
 */

/*
 * The action on the routing enabler vector
 */
typedef enum
{
    /**
     * Disable a routing option from the routing enablers vector
     */
    ROUTING_VECTOR_ACTION_DISABLE,
    /**
     * Enable a routing option from the routing enablers vector
     */
    ROUTING_VECTOR_ACTION_ENABLE
} in_lif_routing_vector_action_e;

/**
 * \brief -
 *      Initialize the dnx_in_lif_profile_last_info_t structure to its default data.
 */
void
dnx_in_lif_profile_last_info_t_init(
    dnx_in_lif_profile_last_info_t * last_profile)
{
    last_profile->old_routing_enablers_profile = 0;
    last_profile->old_cs_vtt_in_lif_profile = DNX_IN_LIF_PROFILE_CS_DEFAULT;
    last_profile->old_cs_vtt_to_flp_in_lif_profile = DNX_IN_LIF_PROFILE_CS_DEFAULT;
    last_profile->old_cs_flp_in_lif_profile = DNX_IN_LIF_FWD_PROFILE_CS_DEFAULT;
    last_profile->old_da_not_found_dst_profile = DNX_DA_NOT_FOUND_DST_PROFILE_DEFAULT;
    last_profile->old_in_lif_lb_profile = DEFAULT_IN_LIF_LB_PROFILE;
    last_profile->old_in_lif_profile = DNX_IN_LIF_PROFILE_DEFAULT;
}

/**
 * \brief -
 *      Verify the amounrt of bits allocated for egress properties can be supported.
 */
static shr_error_e
dnx_in_lif_profile_algo_egress_profile_and_orientation_iop_mode_soc_prop_verify(
    int unit)
{
    int preserve_dscp_number_of_bits, same_interface_number_of_bits;
    SHR_FUNC_INIT_VARS(unit);

    same_interface_number_of_bits = 0;
    preserve_dscp_number_of_bits = 0;
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
    if (utilex_log2_round_up(dnx_data_lif.in_lif.in_lif_profile_allocate_orientation_get(unit)) >
        dnx_data_lif.in_lif.max_number_of_orientation_bits_iop_mode_get(unit))
    {
        SHR_ERR_EXIT(_SHR_E_PARAM, "The allocated number of orientation value is not supported in IOP mode!\n");
    }
    if ((preserve_dscp_number_of_bits + same_interface_number_of_bits +
         IVE_INNER_QOS_DP_PROFILE_NOF_BITS) >
        (dnx_data_lif.in_lif.max_number_of_egress_lif_profile_bits_iop_mode_get(unit)))
    {
        SHR_ERR_EXIT(_SHR_E_PARAM, "The allocated number of egress properties is not supported in IOP mode!\n");
    }
exit:
    SHR_FUNC_EXIT;
}

/**
 * \brief -
 *      Verify the amounrt of bits allocated for egress properties can be supported.
 */
static shr_error_e
dnx_in_lif_profile_algo_egress_profile_iop_mode_soc_prop_verify(
    int unit)
{
    int preserve_dscp_number_of_bits, same_interface_number_of_bits;
    SHR_FUNC_INIT_VARS(unit);

    same_interface_number_of_bits = 0;
    preserve_dscp_number_of_bits = 0;
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
    if ((preserve_dscp_number_of_bits + same_interface_number_of_bits +
         IVE_INNER_QOS_DP_PROFILE_NOF_BITS + IN_LIF_ORIENTATION_NOF_BITS) >
        (dnx_data_lif.in_lif.max_number_of_egress_lif_profile_bits_iop_mode_get(unit)))
    {
        SHR_ERR_EXIT(_SHR_E_PARAM, "The allocated number of egress properties is not supported in IOP mode!\n");
    }
exit:
    SHR_FUNC_EXIT;
}
/**
 * \brief -
 *  returns the indexed mode part of in_lif_profile
 *
 * \param [in] unit - unit number
 * \param [in] native_indexed_mode - native indexed mode value
 * \param [out] native_indexed_mode_in_lif_profile - profile value for indexed mode
 *
 * \return
 *   \retval  Negative if error was detected. See \ref shr_error_e
 *
 * \remark
 *   none
 *
 * \see
 *   dnx_in_lif_profile_exchange.
 */
static shr_error_e
dnx_in_lif_profile_convert_to_native_indexed_profile(
    int unit,
    uint8 native_indexed_mode,
    uint8 *native_indexed_mode_in_lif_profile)
{
    int indexed_mode_enable;
    SHR_FUNC_INIT_VARS(unit);
    DBAL_FUNC_INIT_VARS(unit);

    SHR_NULL_CHECK(native_indexed_mode_in_lif_profile, _SHR_E_PARAM,
                   "A valid pointer for native_indexed_mode_in_lif_profile is needed!");
    indexed_mode_enable = dnx_data_lif.in_lif.in_lif_profile_allocate_indexed_mode_get(unit);

    if (indexed_mode_enable == IN_LIF_PROFILE_PWE_INDEXED_MODE_NOF_VALUES)
    {
        *native_indexed_mode_in_lif_profile = native_indexed_mode;
    }
    else
    {
        *native_indexed_mode_in_lif_profile = 0;
    }

exit:
    DBAL_FUNC_FREE_VARS;
    SHR_FUNC_EXIT;
}

/**
 * \brief -
 *  returns the indexed mode part of in_lif_profile
 *
 * \param [in] unit - unit number
 * \param [in] hw_enum_value - the unique layer type enum member
 * \param [in] mc_enable_value - indication whether MC routing is enabled for this layer type
 * \param [in] uc_enable_value - indication whether UC routing is enabled for this layer type
 *
 * \return
 *   \retval  the calculated routing enablers vector piece per layer type.
 *
 * \remark
 *   none
 *
 * \see
 *   dnx_in_lif_profile_get_default_routing_enablers_vector
 */
static uint32
dnx_in_lif_profile_get_routing_enablers_vector(
    int unit,
    uint32 hw_enum_value,
    uint8 mc_enable_value,
    uint8 uc_enable_value)
{
    uint32 routing_enablers_vector;
    uint32 profile_size = dnx_data_l3.routing_enablers.routing_enable_action_profile_size_get(unit);

    if (!dnx_data_l3.routing_enablers.routing_enable_action_support_get(unit))
    {
        routing_enablers_vector =
            mc_enable_value << ((NOF_ROUTING_ENABLE_TYPES * hw_enum_value + ROUTING_ENABLE_MC) %
                                32) | uc_enable_value << ((NOF_ROUTING_ENABLE_TYPES * hw_enum_value +
                                                           ROUTING_ENABLE_UC) % 32);
    }
    else
    {
        uint32 offset_in_vector = DNX_ROUTING_ENABLER_ACTION_PROFILE_OFFSET_CALCULATE(unit, hw_enum_value);
        uint32 offset_in_array = (offset_in_vector / SAL_UINT32_NOF_BITS) * SAL_UINT32_NOF_BITS;
        routing_enablers_vector =
            ((uc_enable_value | (mc_enable_value << profile_size)) << offset_in_vector) >> offset_in_array;
    }
    return routing_enablers_vector;
}

/**
 * \brief -
 *  returns the indexed mode part of in_lif_profile
 *
 * \param [in] unit - unit number
 * \param [out] routing_enablers_vector - the default value for the routing_enablers_vector
 *
 * \return
 *   \retval  Negative if error was detected. See \ref shr_error_e
 *
 * \remark
 *   none
 *
 * \see
 *   dnx_in_lif_profile_exchange.
 */
static shr_error_e
dnx_in_lif_profile_get_default_routing_enablers_vector(
    int unit,
    uint32 *routing_enablers_vector)
{
    int entry_to_configure, idx, nof_layers_per_vector_idx;

    uint32 hw_enum_value;
    uint8 mc_enable, uc_enable;
    SHR_FUNC_INIT_VARS(unit);
    DBAL_FUNC_INIT_VARS(unit);

    sal_memset(routing_enablers_vector, 0,
               sizeof(routing_enablers_vector[0]) * ROUTING_ENABLERS_VECTOR_SIZE_IN_UINT32(unit));
    /** Calculate the data for how many layer types fits into a single routing enablers vector index. */
    nof_layers_per_vector_idx =
        dnx_data_device.general.max_nof_layer_protocols_get(unit) / ROUTING_ENABLERS_VECTOR_SIZE_IN_UINT32(unit);

    for (entry_to_configure = 0;; entry_to_configure++)
    {
        dbal_enum_value_field_layer_types_e layer_type_enabler =
            dnx_data_l3.routing_enablers.layer_enablers_by_id_get(unit, entry_to_configure)->layer_type;
        if (layer_type_enabler == DBAL_NOF_ENUM_LAYER_TYPES_VALUES)
        {
            break;
        }
        SHR_IF_ERR_EXIT(dbal_fields_enum_hw_value_get
                        (unit, DBAL_FIELD_LAYER_TYPES, layer_type_enabler, &hw_enum_value));
        mc_enable =
            (dnx_data_l3.routing_enablers.routing_enable_action_support_get(unit)) ?
            dnx_data_l3.routing_enablers.routing_enabled_action_profile_id_get(unit) :
            dnx_data_l3.routing_enablers.layer_enablers_get(unit, layer_type_enabler)->mc_enable;
        uc_enable =
            (dnx_data_l3.routing_enablers.routing_enable_action_support_get(unit)) ?
            dnx_data_l3.routing_enablers.routing_enabled_action_profile_id_get(unit) :
            dnx_data_l3.routing_enablers.layer_enablers_get(unit, layer_type_enabler)->uc_enable;

        idx = hw_enum_value / nof_layers_per_vector_idx;
        routing_enablers_vector[idx] =
            routing_enablers_vector[idx] | dnx_in_lif_profile_get_routing_enablers_vector(unit, hw_enum_value,
                                                                                          mc_enable, uc_enable);
    }
exit:
    DBAL_FUNC_FREE_VARS;
    SHR_FUNC_EXIT;
}
/**
 * \brief -
 *  returns the eth_rif_profile_used_bits_mask
 *
 * \param [in] unit - unit number
 * \param [out] eth_rif_profile_used_bits_mask - eth_rif_profile_used_bits_mask according to soc properties
 * \param [out] egress_reserved_nof_bits - egress_reserved_nof_bits according to soc properties
 * \param [out] eth_rif_profile_egress_reserved_start_bit - eth_rif_profile_egress_reserved_start_bit according to soc properties
 *
 * \return
 *   \retval  Negative if error was detected. See \ref shr_error_e
 *
 * \remark
 *   none
 *
 * \see
 *   dnx_in_lif_profile_exchange.
 */
static shr_error_e
dnx_eth_rif_profile_used_bits_mask_get(
    int unit,
    int *eth_rif_profile_used_bits_mask,
    int *egress_reserved_nof_bits,
    int *eth_rif_profile_egress_reserved_start_bit)
{
    SHR_FUNC_INIT_VARS(unit);

    SHR_NULL_CHECK(eth_rif_profile_used_bits_mask, _SHR_E_PARAM,
                   "A valid pointer for eth_rif_profile_used_bits_mask is needed!");
    if ((dnx_data_headers.system_headers.system_headers_mode_get(unit) ==
         DBAL_ENUM_FVAL_SYSTEM_HEADERS_MODE_JERICHO_MODE)
        && (dnx_data_qos.qos.routing_dscp_preserve_mode_get(unit) == 1))
    {
        if (dnx_data_lif.feature.feature_get(unit, dnx_data_lif_feature_iop_mode_orientation_selection))
        {
            *egress_reserved_nof_bits = IN_LIF_PROFILE_EGRESS_NOF_BITS_JR_MODE;
            *eth_rif_profile_egress_reserved_start_bit =
                (ETH_RIF_PROFILE_PMF_RESERVED_START_BIT - IN_LIF_PROFILE_EGRESS_NOF_BITS_JR_MODE);
            *eth_rif_profile_used_bits_mask = (1 << (NOF_ETH_RIF_PROFILE_BITS - ETH_RIF_PROFILE_PMF_RESERVED_NOF_BITS -
                                                     IN_LIF_PROFILE_EGRESS_NOF_BITS_JR_MODE)) - 1;
        }
        else
        {
            *egress_reserved_nof_bits = IN_LIF_PROFILE_EGRESS_NOF_BITS_DSCP_MODE;
            *eth_rif_profile_egress_reserved_start_bit =
                (ETH_RIF_PROFILE_PMF_RESERVED_START_BIT - IN_LIF_PROFILE_EGRESS_NOF_BITS_DSCP_MODE);
            *eth_rif_profile_used_bits_mask =
                (1 <<
                 (NOF_ETH_RIF_PROFILE_BITS - ETH_RIF_PROFILE_PMF_RESERVED_NOF_BITS -
                  IN_LIF_PROFILE_EGRESS_NOF_BITS_DSCP_MODE)) - 1;
        }
    }
    else
    {
        *eth_rif_profile_used_bits_mask = ETH_RIF_PROFILE_USED_BITS_MASK;
        *egress_reserved_nof_bits = ETH_RIF_PROFILE_EGRESS_RESERVED_NOF_BITS;
        *eth_rif_profile_egress_reserved_start_bit = ETH_RIF_PROFILE_EGRESS_RESERVED_START_BIT;
    }

exit:
    SHR_FUNC_EXIT;
}

/**
 * \brief -
 *  returns the in_lif_profile_used_bits_mask
 *
 * \param [in] unit - unit number
 * \param [out] in_lif_profile_used_bits_mask - in_lif_profile_used_bits_mask according to soc properties
 * \param [out] egress_reserved_nof_bits - egress_reserved_nof_bits according to soc properties
 * \param [out] egress_reserved_start_bit - egress_reserved_start_bit according to soc properties
 *
 * \return
 *   \retval  Negative if error was detected. See \ref shr_error_e
 *
 * \remark
 *   none
 *
 * \see
 *   dnx_in_lif_profile_exchange.
 */
static shr_error_e
dnx_in_lif_profile_used_bits_mask_get(
    int unit,
    int *in_lif_profile_used_bits_mask,
    int *egress_reserved_nof_bits,
    int *egress_reserved_start_bit)
{
    int preserve_dscp_number_of_bits, same_interface_number_of_bits = 0;
    SHR_FUNC_INIT_VARS(unit);

    SHR_NULL_CHECK(in_lif_profile_used_bits_mask, _SHR_E_PARAM,
                   "A valid pointer for in_lif_profile_used_bits_mask is needed!");
    /**
     * The default mode is routing_dscp_preserve_mode = 2, which means preserve per outlif.
     * If the mode is routing_dscp_preserve_mode_get = 1, then we preserve per inlif as well and
     * we need to allocate 1 bit for it.
     */
    if (dnx_data_qos.qos.routing_dscp_preserve_mode_get(unit) == 1)
    {
        preserve_dscp_number_of_bits = 1;
    }
    else
    {
        preserve_dscp_number_of_bits = 0;
    }
    /**
     * In J2P we support orientation and same interface working independently,
     * as same_interface mode can only device scope/ only system scope - for which we need 1 bit
     * or both device and system scope enabled together - 2bits
     */
    if ((dnx_data_lif.in_lif.in_lif_profile_allocate_same_interface_mode_get(unit) ==
         JR_COMP_MODE_ONLY_SYS_SCOPE_ENABLED_BY_SOC_PROPERTIES)
        || (dnx_data_lif.in_lif.in_lif_profile_allocate_same_interface_mode_get(unit) ==
            JR_COMP_MODE_ONLY_DEV_SCOPE_ENABLED_BY_SOC_PROPERTIES))
    {
        same_interface_number_of_bits = 1;
    }
    else if (dnx_data_lif.in_lif.in_lif_profile_allocate_same_interface_mode_get(unit) ==
             JR_COMP_MODE_BOTH_DEV_AND_SYS_SCOPE_ENABLED_BY_SOC_PROPERTIES)
    {
        same_interface_number_of_bits = 2;
    }
    else
    {
        same_interface_number_of_bits = 0;
    }
    if (dnx_data_headers.system_headers.system_headers_mode_get(unit) ==
        DBAL_ENUM_FVAL_SYSTEM_HEADERS_MODE_JERICHO2_MODE)
    {
        *egress_reserved_nof_bits = IN_LIF_PROFILE_EGRESS_RESERVED_NOF_BITS;
        *in_lif_profile_used_bits_mask = IN_LIF_PROFILE_USED_BITS_MASK;
        *egress_reserved_start_bit = IN_LIF_PROFILE_EGRESS_RESERVED_START_BIT;
    }
    else
    {
        if (dnx_data_lif.feature.feature_get(unit, dnx_data_lif_feature_iop_mode_orientation_selection))
        {
            *egress_reserved_nof_bits = IN_LIF_PROFILE_EGRESS_NOF_BITS_JR_MODE;
            *in_lif_profile_used_bits_mask = (1 <<
                                              (NOF_IN_LIF_PROFILE_BITS - IN_LIF_PROFILE_PMF_RESERVED_NOF_BITS -
                                               IN_LIF_PROFILE_EGRESS_NOF_BITS_JR_MODE - NATIVE_INDEXED_MODE_NOF_BITS)) -
                1;
            *egress_reserved_start_bit = IN_LIF_PROFILE_PMF_RESERVED_START_BIT - IN_LIF_PROFILE_EGRESS_NOF_BITS_JR_MODE;
        }
        else
        {
            *egress_reserved_nof_bits = IN_LIF_ORIENTATION_NOF_BITS + IVE_INNER_QOS_DP_PROFILE_NOF_BITS +
                same_interface_number_of_bits + preserve_dscp_number_of_bits;
            *in_lif_profile_used_bits_mask =
                (1 <<
                 (NOF_IN_LIF_PROFILE_BITS - IN_LIF_PROFILE_PMF_RESERVED_NOF_BITS - NATIVE_INDEXED_MODE_NOF_BITS -
                  (IN_LIF_ORIENTATION_NOF_BITS + IVE_INNER_QOS_DP_PROFILE_NOF_BITS + same_interface_number_of_bits +
                   preserve_dscp_number_of_bits))) - 1;
            *egress_reserved_start_bit =
                IN_LIF_PROFILE_PMF_RESERVED_START_BIT - (IN_LIF_ORIENTATION_NOF_BITS +
                                                         IVE_INNER_QOS_DP_PROFILE_NOF_BITS +
                                                         same_interface_number_of_bits + preserve_dscp_number_of_bits);
        }
    }

exit:
    SHR_FUNC_EXIT;
}
/**
 * \brief -
 *  returns the indexed mode part of in_lif_profile
 *
 * \param [in] unit - unit number
 * \param [in] in_lif_profile - profile value
 * \param [out] native_indexed_mode - native indexed mode value
 *
 * \return
 *   \retval  Negative if error was detected. See \ref shr_error_e
 *
 * \remark
 *   none
 *
 * \see
 *   dnx_in_lif_profile_exchange.
 */
static shr_error_e
dnx_in_lif_profile_convert_to_native_indexed_mode(
    int unit,
    int in_lif_profile,
    uint8 *native_indexed_mode)
{
    int indexed_mode_enable, native_indexed_mode_in_lif_profile;
    SHR_FUNC_INIT_VARS(unit);
    DBAL_FUNC_INIT_VARS(unit);

    SHR_NULL_CHECK(native_indexed_mode, _SHR_E_PARAM, "A valid pointer for native_indexed_mode is needed!");
    indexed_mode_enable = dnx_data_lif.in_lif.in_lif_profile_allocate_indexed_mode_get(unit);

    /**If native indexed mode is enabled, extract the value from the inlif profile input*/
    if (indexed_mode_enable == IN_LIF_PROFILE_PWE_INDEXED_MODE_NOF_VALUES)
    {
        native_indexed_mode_in_lif_profile = in_lif_profile >> IN_LIF_PROFILE_NATIVE_INDEXED_MODE_START_BIT;
        *native_indexed_mode = native_indexed_mode_in_lif_profile & 1;
    }
    else
    {
        *native_indexed_mode = 0;
    }

exit:
    DBAL_FUNC_FREE_VARS;
    SHR_FUNC_EXIT;
}
/**
 * \brief -
 *  returns the egress part of in_lif_profile in case the orientation is indexed by iop mode lif profile
 *
 * \param [in] unit - unit number
 * \param [in] egress_fields_info - a struct that includes the given egress fields values
 * \param [in] old_in_lif_profile - old inlif profile
 * \param [out] egress_in_lif_profile - the return egress in_lif_profile value
 *
 * \return
 *   \retval  Negative if error was detected. See \ref shr_error_e
 *
 * \remark
 *   none
 *
 * \see
 *   dnx_in_lif_profile_exchange.
 */
static shr_error_e
dnx_in_lif_profile_algo_convert_to_egress_profile_iop_mode(
    int unit,
    in_lif_profile_info_egress_t egress_fields_info,
    int old_in_lif_profile,
    int *egress_in_lif_profile)
{
    int in_lif_profile_orientation_number_of_values,
        same_interface_number_of_values, old_egress_in_lif_profile, policer_inner_dp,
        policer_outer_dp, preserve_dscp_mode;
    SHR_FUNC_INIT_VARS(unit);

    SHR_NULL_CHECK(egress_in_lif_profile, _SHR_E_PARAM, "A valid pointer for egress_in_lif_profile is needed!");
    same_interface_number_of_values = dnx_data_lif.in_lif.in_lif_profile_allocate_same_interface_mode_get(unit);
    in_lif_profile_orientation_number_of_values = dnx_data_lif.in_lif.in_lif_profile_allocate_orientation_get(unit);
    policer_inner_dp = dnx_data_lif.in_lif.in_lif_profile_allocate_policer_inner_dp_get(unit);
    policer_outer_dp = dnx_data_lif.in_lif.in_lif_profile_allocate_policer_outer_dp_get(unit);
    preserve_dscp_mode = dnx_data_qos.qos.routing_dscp_preserve_mode_get(unit);

    /** If orientation is enabled by soc property, then the in_lif_profile is populated only with its values*/
    if (((in_lif_profile_orientation_number_of_values > 0)
         && (same_interface_number_of_values == JR_COMP_MODE_ONLY_SYS_SCOPE_ENABLED_BY_SOC_PROPERTIES))
        || ((in_lif_profile_orientation_number_of_values > 0)
            && (same_interface_number_of_values == JR_COMP_MODE_ONLY_DEV_SCOPE_ENABLED_BY_SOC_PROPERTIES)))
    {
        if ((egress_fields_info.lif_same_interface_mode == SYSTEM_SCOPE)
            || (egress_fields_info.lif_same_interface_mode == SYSTEM_SCOPE_DISABLE))
        {
            *egress_in_lif_profile = ((egress_fields_info.in_lif_orientation << 1) & IN_LIF_PROFILE_JR_MODE_MSB_MASK)
                | (egress_fields_info.lif_same_interface_mode & IN_LIF_PROFILE_JR_MODE_LSB_MASK);
        }
        else
        {
            if (egress_fields_info.lif_same_interface_mode == DEVICE_SCOPE)
            {
                *egress_in_lif_profile =
                    ((egress_fields_info.in_lif_orientation << 1) & IN_LIF_PROFILE_JR_MODE_MSB_MASK) |
                    (egress_fields_info.lif_same_interface_mode & IN_LIF_PROFILE_JR_MODE_LSB_MASK);
            }
            else
            {
                *egress_in_lif_profile =
                    ((egress_fields_info.in_lif_orientation << 1) & IN_LIF_PROFILE_JR_MODE_MSB_MASK) | (1 &
                                                                                                        IN_LIF_PROFILE_JR_MODE_LSB_MASK);
            }
        }
    }
    /** only orientation is enabled*/
    else if ((in_lif_profile_orientation_number_of_values > 0) && (same_interface_number_of_values == 0)
             && (preserve_dscp_mode != PRESERVE_DSCP_PER_OUTLIF_AND_INLIF))
    {
        *egress_in_lif_profile = egress_fields_info.in_lif_orientation;
    }
    /** If both are enabled, the same if for bridge is the LSB and the logical same if is the MSB*/
    else if ((same_interface_number_of_values == JR_COMP_MODE_BOTH_DEV_AND_SYS_SCOPE_ENABLED_BY_SOC_PROPERTIES)
             || ((in_lif_profile_orientation_number_of_values == 0)
                 && (same_interface_number_of_values == JR_COMP_MODE_ONLY_SYS_SCOPE_ENABLED_BY_SOC_PROPERTIES))
             || ((in_lif_profile_orientation_number_of_values == 0)
                 && (same_interface_number_of_values == JR_COMP_MODE_ONLY_DEV_SCOPE_ENABLED_BY_SOC_PROPERTIES)))
    {
        old_egress_in_lif_profile =
            old_in_lif_profile >> (IN_LIF_PROFILE_PMF_RESERVED_START_BIT - IN_LIF_PROFILE_EGRESS_NOF_BITS_JR_MODE);
        if (same_interface_number_of_values == JR_COMP_MODE_BOTH_DEV_AND_SYS_SCOPE_ENABLED_BY_SOC_PROPERTIES)
        {
            /** In 2 bits mode for filtering the LSB is the Device Scope and the MSB is the System Scope*/
            if (egress_fields_info.lif_same_interface_mode == SYSTEM_SCOPE)
            {
                /** To ensure the MSB bit is set - system scope is enabled*/
                *egress_in_lif_profile = (old_egress_in_lif_profile & 1) | IN_LIF_PROFILE_JR_MODE_MSB_MASK;
            }
            else if (egress_fields_info.lif_same_interface_mode == SYSTEM_SCOPE_DISABLE)
            {
                /** To disable the system scope - mask the MSB*/
                *egress_in_lif_profile = (old_egress_in_lif_profile & 1);
            }
            else if (egress_fields_info.lif_same_interface_mode == DEVICE_SCOPE)
            {
                /** Device scope is enabled when the LSB is 0.*/
                *egress_in_lif_profile = (old_egress_in_lif_profile) & IN_LIF_PROFILE_JR_MODE_MSB_MASK;
            }
            else if (egress_fields_info.lif_same_interface_mode == DISABLE)
            {
                 /** Device scope is disabled when the LSB is 1.*/
                *egress_in_lif_profile = (old_egress_in_lif_profile) | 1;
            }
        }
        else
        {
            if (egress_fields_info.lif_same_interface_mode == SYSTEM_SCOPE)
            {
                *egress_in_lif_profile = old_egress_in_lif_profile | 1;
            }
            else if (egress_fields_info.lif_same_interface_mode == SYSTEM_SCOPE_DISABLE)
            {
                *egress_in_lif_profile = old_egress_in_lif_profile & 0;
            }
            else if (egress_fields_info.lif_same_interface_mode == DEVICE_SCOPE)
            {
                *egress_in_lif_profile = old_egress_in_lif_profile & 0;
            }
            else if (egress_fields_info.lif_same_interface_mode == DISABLE)
            {
                *egress_in_lif_profile = old_egress_in_lif_profile | 1;
            }
        }
    }
    /** In IOP mode if policer is enabled - both inner and outer DP have the same value, which is transfered to the lif profile itself*/
    else if ((policer_inner_dp == 2) && (policer_outer_dp == 2))
    {
        *egress_in_lif_profile = egress_fields_info.ive_outer_qos_dp_profile;
    }
    /** In IOP mode if preserve_dscp is enabled to preserve per inlif and outlif- assign lif profile value from it.*/
    else if (preserve_dscp_mode == PRESERVE_DSCP_PER_OUTLIF_AND_INLIF)
    {
        *egress_in_lif_profile = ((egress_fields_info.in_lif_orientation << 1) & IN_LIF_PROFILE_JR_MODE_MSB_MASK)
            | (egress_fields_info.preserve_dscp_mode & IN_LIF_PROFILE_JR_MODE_LSB_MASK);
    }
exit:
    SHR_FUNC_EXIT;
}

/**
 * \brief -
 *  returns the egress part of in_lif_profile
 *
 * \param [in] unit - unit number
 * \param [in] egress_fields_info - a struct that includes the given egress fields values
 * \param [in] old_in_lif_profile - old inlif profile
 * \param [out] egress_in_lif_profile - the return egress in_lif_profile value
 *
 * \return
 *   \retval  Negative if error was detected. See \ref shr_error_e
 *
 * \remark
 *   none
 *
 * \see
 *   dnx_in_lif_profile_exchange.
 */
static shr_error_e
dnx_in_lif_profile_algo_convert_to_egress_profile_and_orientation_iop_mode(
    int unit,
    in_lif_profile_info_egress_t egress_fields_info,
    int old_in_lif_profile,
    int *egress_in_lif_profile)
{
    int old_egress_in_lif_profile =
        0, preserve_dscp_number_of_bits, lif_same_interface_mode, same_interface_number_of_bits;

    SHR_FUNC_INIT_VARS(unit);

    SHR_NULL_CHECK(egress_in_lif_profile, _SHR_E_PARAM, "A valid pointer for egress_in_lif_profile is needed!");

    same_interface_number_of_bits = 0;
    lif_same_interface_mode = 0;
    /** Decide the number of same interface bits*/
    if (JR_COMP_MODE_SAME_IF_MODE_1B_ALLOCATION
        (dnx_data_lif.in_lif.in_lif_profile_allocate_same_interface_mode_get(unit)))
    {
        old_egress_in_lif_profile =
            old_in_lif_profile >> (IN_LIF_PROFILE_PMF_RESERVED_START_BIT - IN_LIF_ORIENTATION_NOF_BITS - 1);
        same_interface_number_of_bits = 1;
        /** When we have 1 bit enabled - its either device scope enable disable or system scope enable disable*/
        if ((egress_fields_info.lif_same_interface_mode == SYSTEM_SCOPE)
            || (egress_fields_info.lif_same_interface_mode == DISABLE))
        {
            lif_same_interface_mode = (old_egress_in_lif_profile | 1);
        }
        else if ((egress_fields_info.lif_same_interface_mode == DEVICE_SCOPE)
                 || (egress_fields_info.lif_same_interface_mode == SYSTEM_SCOPE_DISABLE))
        {
            lif_same_interface_mode = (old_egress_in_lif_profile & 0);
        }
    }
    /** Both device and system scope is enabled - threat as bit allocation*/
    else if (JR_COMP_MODE_SAME_IF_MODE_2B_ALLOCATION
             (dnx_data_lif.in_lif.in_lif_profile_allocate_same_interface_mode_get(unit)))
    {
        old_egress_in_lif_profile =
            old_in_lif_profile >> (IN_LIF_PROFILE_PMF_RESERVED_START_BIT - IN_LIF_ORIENTATION_NOF_BITS - 2);
        same_interface_number_of_bits = 2;
        if (egress_fields_info.lif_same_interface_mode == SYSTEM_SCOPE)
        {
            /** To ensure the MSB bit is set - system scope is enabled*/
            lif_same_interface_mode = ((old_egress_in_lif_profile & 1) | IN_LIF_PROFILE_JR_MODE_MSB_MASK);
        }
        else if (egress_fields_info.lif_same_interface_mode == SYSTEM_SCOPE_DISABLE)
        {
            /** To disable the system scope - mask the MSB*/
            lif_same_interface_mode = (old_egress_in_lif_profile & 1);
        }
        else if (egress_fields_info.lif_same_interface_mode == DEVICE_SCOPE)
        {
            /** Device scope is enabled when the LSB is 0.*/
            lif_same_interface_mode = ((old_egress_in_lif_profile) & IN_LIF_PROFILE_JR_MODE_MSB_MASK);
        }
        else if (egress_fields_info.lif_same_interface_mode == DISABLE)
        {
             /** Device scope is disabled when the LSB is 1.*/
            lif_same_interface_mode = ((old_egress_in_lif_profile) | 1);
        }
    }

    if (JR_COMP_MODE_SAME_PRESERVE_DSCP_PER_INLIF(dnx_data_qos.qos.routing_dscp_preserve_mode_get(unit)))
    {
        preserve_dscp_number_of_bits = 1;
    }
    else
    {
        preserve_dscp_number_of_bits = 0;
    }
    *egress_in_lif_profile = egress_fields_info.preserve_dscp_mode |
        (egress_fields_info.ive_inner_qos_dp_profile << preserve_dscp_number_of_bits) |
        (lif_same_interface_mode << (preserve_dscp_number_of_bits + IVE_INNER_QOS_DP_PROFILE_NOF_BITS)) |
        (egress_fields_info.in_lif_orientation << (preserve_dscp_number_of_bits + IVE_INNER_QOS_DP_PROFILE_NOF_BITS +
                                                   same_interface_number_of_bits));
exit:
    SHR_FUNC_EXIT;
}

/**
 * \brief -
 *  returns the egress part of in_lif_profile
 *
 * \param [in] unit - unit number
 * \param [in] egress_fields_info - a struct that includes the given egress fields values
 * \param [in] old_in_lif_profile - old inlif profile
 * \param [out] egress_in_lif_profile - the return egress in_lif_profile value
 *
 * \return
 *   \retval  Negative if error was detected. See \ref shr_error_e
 *
 * \remark
 *   none
 *
 * \see
 *   dnx_in_lif_profile_exchange.
 */
static shr_error_e
dnx_in_lif_profile_algo_convert_to_egress_profile(
    int unit,
    in_lif_profile_info_egress_t egress_fields_info,
    int old_in_lif_profile,
    int *egress_in_lif_profile)
{
    int in_lif_profile_orientation_number_of_values, inner_dp_number_of_values, outer_dp_number_of_values,
        same_interface_number_of_values;
    SHR_FUNC_INIT_VARS(unit);

    SHR_NULL_CHECK(egress_in_lif_profile, _SHR_E_PARAM, "A valid pointer for egress_in_lif_profile is needed!");
    same_interface_number_of_values = dnx_data_lif.in_lif.in_lif_profile_allocate_same_interface_mode_get(unit);
    outer_dp_number_of_values = dnx_data_lif.in_lif.in_lif_profile_allocate_policer_outer_dp_get(unit);
    inner_dp_number_of_values = dnx_data_lif.in_lif.in_lif_profile_allocate_policer_inner_dp_get(unit);
    in_lif_profile_orientation_number_of_values = dnx_data_lif.in_lif.in_lif_profile_allocate_orientation_get(unit);

    /**Convert to egress profile in JR2 system header mode*/
    if (dnx_data_headers.system_headers.system_headers_mode_get(unit) ==
        DBAL_ENUM_FVAL_SYSTEM_HEADERS_MODE_JERICHO2_MODE)
    {

        if ((utilex_msb_bit_on(in_lif_profile_orientation_number_of_values) + 1) <
            (utilex_msb_bit_on(egress_fields_info.in_lif_orientation) + 1))
        {
            SHR_ERR_EXIT(_SHR_E_PARAM, "Not enough number of values for orientation were allocated!\n");
        }

        if ((utilex_msb_bit_on(inner_dp_number_of_values) + 1) <
            (utilex_msb_bit_on(egress_fields_info.ive_inner_qos_dp_profile) + 1))
        {
            SHR_ERR_EXIT(_SHR_E_PARAM, "Not enough number of values for inner dp were allocated!\n");
        }

        if ((utilex_msb_bit_on(outer_dp_number_of_values) + 1) <
            (utilex_msb_bit_on(egress_fields_info.ive_outer_qos_dp_profile) + 1))
        {
            SHR_ERR_EXIT(_SHR_E_PARAM, "Not enough number of values for outer DP were allocated!\n");
        }

        if ((utilex_msb_bit_on(same_interface_number_of_values) + 1) <
            (utilex_msb_bit_on(egress_fields_info.lif_same_interface_mode) + 1))
        {
            SHR_ERR_EXIT(_SHR_E_PARAM, "Not enough number of values for same interface were allocated!\n");
        }
        *egress_in_lif_profile = egress_fields_info.ive_inner_qos_dp_profile |
            (egress_fields_info.ive_outer_qos_dp_profile << IVE_INNER_QOS_DP_PROFILE_NOF_BITS) |
            (egress_fields_info.in_lif_orientation << (IVE_INNER_QOS_DP_PROFILE_NOF_BITS +
                                                       IVE_OUTER_QOS_DP_PROFILE_NOF_BITS)) |
            (egress_fields_info.lif_same_interface_mode <<
             (IVE_INNER_QOS_DP_PROFILE_NOF_BITS + IVE_OUTER_QOS_DP_PROFILE_NOF_BITS + IN_LIF_ORIENTATION_NOF_BITS));
    }
    /**Convert to egress profile in JR1 system header mode, in case orientation is indexed by jr1 inlif profile*/
    else if ((dnx_data_headers.system_headers.system_headers_mode_get(unit) ==
              DBAL_ENUM_FVAL_SYSTEM_HEADERS_MODE_JERICHO_MODE)
             && dnx_data_lif.feature.feature_get(unit, dnx_data_lif_feature_iop_mode_orientation_selection))
    {
        SHR_IF_ERR_EXIT(dnx_in_lif_profile_algo_convert_to_egress_profile_iop_mode
                        (unit, egress_fields_info, old_in_lif_profile, egress_in_lif_profile));
    }
    /**Convert to egress profile in JR1 system header mode, in case orientation is indexed by jr2 inlif profile*/
    else
    {
        SHR_IF_ERR_EXIT(dnx_in_lif_profile_algo_convert_to_egress_profile_and_orientation_iop_mode
                        (unit, egress_fields_info, old_in_lif_profile, egress_in_lif_profile));
    }

exit:
    SHR_FUNC_EXIT;
}

/**
 * \brief -
 *  returns the egress part of in_lif_profile
 *
 * \param [in] unit - unit number
 * \param [in] egress_fields_info - a struct that includes the given egress fields values
 * \param [out] egress_eth_rif_profile - the return egress eth_rif_profile value
 *
 * \return
 *   \retval  Negative if error was detected. See \ref shr_error_e
 *
 * \remark
 *   none
 *
 * \see
 *   dnx_in_lif_profile_exchange.
 */
static shr_error_e
dnx_eth_rif_profile_algo_convert_to_egress_profile(
    int unit,
    in_lif_profile_info_egress_t egress_fields_info,
    int *egress_eth_rif_profile)
{
    int system_headers_mode, preserve_dscp_mode;

    SHR_FUNC_INIT_VARS(unit);
    DBAL_FUNC_INIT_VARS(unit);

    SHR_NULL_CHECK(egress_eth_rif_profile, _SHR_E_PARAM, "A valid pointer for egress_eth_rif_profile is needed!");
    system_headers_mode = dnx_data_headers.system_headers.system_headers_mode_get(unit);
    preserve_dscp_mode = dnx_data_qos.qos.routing_dscp_preserve_mode_get(unit);

    if ((system_headers_mode == DBAL_ENUM_FVAL_SYSTEM_HEADERS_MODE_JERICHO_MODE)
        && (preserve_dscp_mode == PRESERVE_DSCP_PER_OUTLIF_AND_INLIF))
    {
        *egress_eth_rif_profile = egress_fields_info.preserve_dscp_mode;
    }
    else
    {
        *egress_eth_rif_profile = 0;
    }

exit:
    DBAL_FUNC_FREE_VARS;
    SHR_FUNC_EXIT;
}

/**
 * \brief -
 *  extracts egress in_lif_profile values from in_lif_profile
 *
 * \param [in] unit - unit number
 * \param [in] in_lif_profile - the whole in_lif_profile value
 * \param [out] egress_in_lif_profile_info - the return struct that includes egress in_lif_profile fields values
 * \param [out] in_lif_profile_jr_mode - the inlif profile in jr sys headers mode
 * \param [out] in_lif_profile_exclude_orientation - the inlif profile in jr sys headers mode excluding orientation
 * \param [out] in_lif_orientation - the inlif orientation in jr sys headers mode
 *
 * \return
 *   \retval  Negative if error was detected. See \ref shr_error_e
 *
 * \remark
 *   none
 *
 * \see
 *   dnx_in_lif_profile_exchange.
 */
static shr_error_e
dnx_in_lif_profile_algo_extract_egress_profile_iop_mode(
    int unit,
    int in_lif_profile,
    in_lif_profile_info_egress_t * egress_in_lif_profile_info,
    int *in_lif_profile_jr_mode,
    int *in_lif_profile_exclude_orientation,
    int *in_lif_orientation)
{

    int egress_in_lif_profile;
    int orientation, same_interface, policer_inner_dp, policer_outer_dp, preserve_dscp_mode;

    SHR_FUNC_INIT_VARS(unit);

    SHR_NULL_CHECK(egress_in_lif_profile_info, _SHR_E_PARAM,
                   "A valid pointer for egress_in_lif_profile_info is needed!");
    /** Verify  the soc properties input */
    SHR_IF_ERR_EXIT(dnx_in_lif_profile_algo_egress_profile_iop_mode_soc_prop_verify(unit));
    egress_in_lif_profile = 0;
    orientation = dnx_data_lif.in_lif.in_lif_profile_allocate_orientation_get(unit);
    same_interface = dnx_data_lif.in_lif.in_lif_profile_allocate_same_interface_mode_get(unit);
    policer_inner_dp = dnx_data_lif.in_lif.in_lif_profile_allocate_policer_inner_dp_get(unit);
    policer_outer_dp = dnx_data_lif.in_lif.in_lif_profile_allocate_policer_outer_dp_get(unit);
    preserve_dscp_mode = dnx_data_qos.qos.routing_dscp_preserve_mode_get(unit);

    egress_in_lif_profile =
        in_lif_profile >> (IN_LIF_PROFILE_PMF_RESERVED_START_BIT - IN_LIF_PROFILE_EGRESS_NOF_BITS_JR_MODE);
    /** If orientation is enabled by soc property, and same interface is not,
     * then the in_lif_profile is populated only with its values and orientation could be in full range
     */
    if ((orientation > 0) && (same_interface == 0) && (preserve_dscp_mode != PRESERVE_DSCP_PER_OUTLIF_AND_INLIF))
    {
        egress_in_lif_profile_info->in_lif_orientation =
            egress_in_lif_profile & IN_LIF_PROFILE_JR_MODE_MAX_USED_BIT_MASK;
        *in_lif_profile_jr_mode = egress_in_lif_profile_info->in_lif_orientation;
        *in_lif_profile_exclude_orientation = egress_in_lif_profile_info->in_lif_orientation;
        *in_lif_orientation = egress_in_lif_profile_info->in_lif_orientation;
        (*egress_in_lif_profile_info).ive_inner_qos_dp_profile = 0;
        (*egress_in_lif_profile_info).ive_outer_qos_dp_profile = 0;
        (*egress_in_lif_profile_info).preserve_dscp_mode = DISABLE_PRESERVE_DSCP_MODE;
    }
    /** If orientation and 1b same interface is enabled, then orientation is limited to up to 2 values - 0/1 */
    else if (((orientation > 0) && (same_interface == JR_COMP_MODE_ONLY_SYS_SCOPE_ENABLED_BY_SOC_PROPERTIES)) ||
             ((orientation > 0) && (same_interface == JR_COMP_MODE_ONLY_DEV_SCOPE_ENABLED_BY_SOC_PROPERTIES)))
    {
        egress_in_lif_profile_info->in_lif_orientation = (egress_in_lif_profile >> 1) & IN_LIF_PROFILE_JR_MODE_LSB_MASK;
        egress_in_lif_profile_info->lif_same_interface_mode = egress_in_lif_profile & IN_LIF_PROFILE_JR_MODE_LSB_MASK;
        *in_lif_profile_jr_mode = egress_in_lif_profile;
        *in_lif_profile_exclude_orientation =
            ((egress_in_lif_profile_info->in_lif_orientation << 1) & IN_LIF_PROFILE_JR_MODE_MSB_MASK) |
            (egress_in_lif_profile_info->lif_same_interface_mode & IN_LIF_PROFILE_JR_MODE_LSB_MASK);
        *in_lif_orientation = egress_in_lif_profile_info->in_lif_orientation;
        (*egress_in_lif_profile_info).ive_inner_qos_dp_profile = 0;
        (*egress_in_lif_profile_info).ive_outer_qos_dp_profile = 0;
        (*egress_in_lif_profile_info).preserve_dscp_mode = DISABLE_PRESERVE_DSCP_MODE;
    }
    /** If both are enabled, the same if for bridge is the LSB and the logical same if is the MSB*/
    else if ((orientation == 0) && (same_interface == JR_COMP_MODE_BOTH_DEV_AND_SYS_SCOPE_ENABLED_BY_SOC_PROPERTIES))
    {
        egress_in_lif_profile_info->lif_same_interface_mode =
            egress_in_lif_profile & IN_LIF_PROFILE_JR_MODE_MAX_USED_BIT_MASK;
        *in_lif_profile_jr_mode = egress_in_lif_profile_info->lif_same_interface_mode;
        *in_lif_profile_exclude_orientation = *in_lif_profile_jr_mode;
        *in_lif_orientation = 0;
        (*egress_in_lif_profile_info).ive_inner_qos_dp_profile = 0;
        (*egress_in_lif_profile_info).ive_outer_qos_dp_profile = 0;
        (*egress_in_lif_profile_info).preserve_dscp_mode = DISABLE_PRESERVE_DSCP_MODE;
    }
    /** If only 1 bit for same if is enabled - the profile is limited to 0/1 values*/
    else if (((orientation == 0) && (same_interface == JR_COMP_MODE_ONLY_SYS_SCOPE_ENABLED_BY_SOC_PROPERTIES)) ||
             ((orientation == 0) && (same_interface == JR_COMP_MODE_ONLY_DEV_SCOPE_ENABLED_BY_SOC_PROPERTIES)))
    {
        egress_in_lif_profile_info->lif_same_interface_mode = egress_in_lif_profile & IN_LIF_PROFILE_JR_MODE_LSB_MASK;
        *in_lif_profile_jr_mode = egress_in_lif_profile_info->lif_same_interface_mode;
        *in_lif_profile_exclude_orientation = *in_lif_profile_jr_mode;
        *in_lif_orientation = 0;
        (*egress_in_lif_profile_info).ive_inner_qos_dp_profile = 0;
        (*egress_in_lif_profile_info).ive_outer_qos_dp_profile = 0;
        (*egress_in_lif_profile_info).preserve_dscp_mode = DISABLE_PRESERVE_DSCP_MODE;
    }
    /**In IOP mode both inner and outer policer DP should be enabled and they both use the same profile value, to be aligned to JR1. */
    else if ((policer_inner_dp == 2) && (policer_outer_dp == 2))
    {
        *in_lif_profile_jr_mode = egress_in_lif_profile;
        *in_lif_profile_exclude_orientation = egress_in_lif_profile;
        *in_lif_orientation = 0;
        (*egress_in_lif_profile_info).lif_same_interface_mode = 0;
        (*egress_in_lif_profile_info).in_lif_orientation = 0;
        (*egress_in_lif_profile_info).ive_inner_qos_dp_profile = egress_in_lif_profile;
        (*egress_in_lif_profile_info).ive_outer_qos_dp_profile = egress_in_lif_profile;
        (*egress_in_lif_profile_info).preserve_dscp_mode = DISABLE_PRESERVE_DSCP_MODE;
    }
    else if (preserve_dscp_mode)
    {
        *in_lif_profile_jr_mode = egress_in_lif_profile;
        *in_lif_profile_exclude_orientation = egress_in_lif_profile;
        /** If orientation is also enabled - then it resides on the most significant bit.*/
        if (orientation)
        {
            *in_lif_orientation = (egress_in_lif_profile >> 1) & IN_LIF_PROFILE_JR_MODE_LSB_MASK;
            (*egress_in_lif_profile_info).in_lif_orientation =
                (egress_in_lif_profile >> 1) & IN_LIF_PROFILE_JR_MODE_LSB_MASK;
        }
        else
        {
            *in_lif_orientation = 0;
            (*egress_in_lif_profile_info).in_lif_orientation = 0;
        }
        (*egress_in_lif_profile_info).lif_same_interface_mode = 0;
        (*egress_in_lif_profile_info).ive_inner_qos_dp_profile = 0;
        (*egress_in_lif_profile_info).ive_outer_qos_dp_profile = 0;
        /** preserve dscp mode 1 means per inlif&outlif*/
        if (preserve_dscp_mode == PRESERVE_DSCP_PER_OUTLIF_AND_INLIF)
        {
            (*egress_in_lif_profile_info).preserve_dscp_mode =
                egress_in_lif_profile & IN_LIF_PROFILE_DSCP_PRESERVE_MASK;
        }
        else
        {
            (*egress_in_lif_profile_info).preserve_dscp_mode = DISABLE_PRESERVE_DSCP_MODE;
        }
    }
exit:
    SHR_FUNC_EXIT;
}

/*
 * See .h file
 */
shr_error_e
dnx_in_lif_profile_algo_extract_egress_profile_and_orientation_iop_mode(
    int unit,
    int in_lif_profile,
    in_lif_profile_info_egress_t * egress_in_lif_profile_info,
    int *in_lif_profile_jr_mode,
    int *in_lif_profile_exclude_orientation,
    int *in_lif_orientation)
{

    int egress_in_lif_profile, preserve_dscp_number_of_bits, same_interface_number_of_bits;

    SHR_FUNC_INIT_VARS(unit);
    SHR_NULL_CHECK(egress_in_lif_profile_info, _SHR_E_PARAM,
                   "A valid pointer for egress_in_lif_profile_info is needed!");
    /** Verify  the soc properties input */
    SHR_IF_ERR_EXIT(dnx_in_lif_profile_algo_egress_profile_and_orientation_iop_mode_soc_prop_verify(unit));
    /**
     * The default mode is routing_dscp_preserve_mode = 2, which means preserve per outlif.
     * If the mode is routing_dscp_preserve_mode_get = 1, then we preserve per inlif as well and
     * we need to allocate 1 bit for it.
     */
    if (dnx_data_qos.qos.routing_dscp_preserve_mode_get(unit) == PRESERVE_DSCP_PER_OUTLIF_AND_INLIF)
    {
        preserve_dscp_number_of_bits = 1;
    }
    else
    {
        preserve_dscp_number_of_bits = 0;
    }
     /**
     * In J2P we support orientation and same interface working independantly,
     * as same_interface mode can only device scope/ only system scope - for which we need 1 bit
     * or both device and system scope enabled together - 2bits
     */
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
    else
    {
        same_interface_number_of_bits = 0;
    }
    /** In IOP mode we allocate 1 bit for both inner and outer policer*/
    egress_in_lif_profile =
        in_lif_profile >> (IN_LIF_PROFILE_PMF_RESERVED_START_BIT -
                           (IN_LIF_ORIENTATION_NOF_BITS + IVE_INNER_QOS_DP_PROFILE_NOF_BITS +
                            same_interface_number_of_bits + preserve_dscp_number_of_bits));

    /** The in_lif_profile on the system headers is the value of the egress_in_lif_profile without orientation*/
    *in_lif_profile_jr_mode =
        egress_in_lif_profile &
        ((1 << (IVE_INNER_QOS_DP_PROFILE_NOF_BITS + preserve_dscp_number_of_bits + same_interface_number_of_bits)) - 1);
    *in_lif_profile_exclude_orientation = *in_lif_profile_jr_mode;

    /** Extract the preserve DSCP value and then calculate the remaining egress_in_lif_profile*/
    egress_in_lif_profile_info->preserve_dscp_mode = egress_in_lif_profile & ((1 << preserve_dscp_number_of_bits) - 1);
    egress_in_lif_profile = egress_in_lif_profile >> preserve_dscp_number_of_bits;

    /** Extract the QOS values and then calculate the remaining egress_in_lif_profile*/
    egress_in_lif_profile_info->ive_inner_qos_dp_profile =
        egress_in_lif_profile & ((1 << IVE_INNER_QOS_DP_PROFILE_NOF_BITS) - 1);
    egress_in_lif_profile = egress_in_lif_profile >> IVE_INNER_QOS_DP_PROFILE_NOF_BITS;
    egress_in_lif_profile_info->ive_outer_qos_dp_profile = egress_in_lif_profile_info->ive_inner_qos_dp_profile;

    /** Extract the same interface value and then calculate the remaining egress_in_lif_profile*/
    egress_in_lif_profile_info->lif_same_interface_mode =
        egress_in_lif_profile & ((1 << same_interface_number_of_bits) - 1);
    egress_in_lif_profile = egress_in_lif_profile >> same_interface_number_of_bits;

    /** Extract the orientation value */
    egress_in_lif_profile_info->in_lif_orientation = egress_in_lif_profile & ((1 << IN_LIF_ORIENTATION_NOF_BITS) - 1);
    *in_lif_orientation = egress_in_lif_profile_info->in_lif_orientation;

exit:
    SHR_FUNC_EXIT;
}
/**
 * \brief -
 *  extracts egress in_lif_profile values from in_lif_profile
 *
 * \param [in] unit - unit number
 * \param [in] in_lif_profile - the whole in_lif_profile value
 * \param [in] intf_type - intf type
 * \param [out] egress_in_lif_profile_info - the return struct that includes egress in_lif_profile fields values
 * \param [out] in_lif_profile_jr_mode - the inlif profile in jr sys headers mode
 * \param [out] in_lif_profile_exclude_orientation - the inlif profile in jr sys headers mode excluding orientation
 * \param [out] in_lif_orientation - the inlif orientation in jr sys headers mode
 *
 * \return
 *   \retval  Negative if error was detected. See \ref shr_error_e
 *
 * \remark
 *   none
 *
 * \see
 *   dnx_in_lif_profile_exchange.
 */
shr_error_e
dnx_in_lif_profile_algo_extract_egress_profile(
    int unit,
    int in_lif_profile,
    in_lif_profile_info_egress_t * egress_in_lif_profile_info,
    int *in_lif_profile_jr_mode,
    int *in_lif_profile_exclude_orientation,
    int *in_lif_orientation,
    in_lif_intf_type_e intf_type)
{
    int egress_in_lif_profile = 0, egress_reserved_start_bit, egress_properties_nof_bits;
    SHR_FUNC_INIT_VARS(unit);
    SHR_NULL_CHECK(egress_in_lif_profile_info, _SHR_E_PARAM,
                   "A valid pointer for egress_in_lif_profile_info is needed!");
    egress_properties_nof_bits = dnx_data_lif.in_lif.nof_in_lif_profile_allocate_orientation_bits_get(unit) +
        dnx_data_lif.in_lif.nof_in_lif_profile_allocate_policer_inner_dp_bits_get(unit) +
        dnx_data_lif.in_lif.nof_in_lif_profile_allocate_policer_outer_dp_bits_get(unit) +
        dnx_data_lif.in_lif.nof_in_lif_profile_allocate_same_interface_mode_bits_get(unit);
    egress_reserved_start_bit =
        dnx_data_lif.in_lif.in_lif_profile_pmf_reserved_start_bit_get(unit) - egress_properties_nof_bits;

    if (dnx_data_headers.system_headers.system_headers_mode_get(unit) ==
        DBAL_ENUM_FVAL_SYSTEM_HEADERS_MODE_JERICHO2_MODE)
    {
        if (intf_type == ETH_RIF)
        {
            egress_in_lif_profile = in_lif_profile >> ETH_RIF_PROFILE_EGRESS_RESERVED_START_BIT;
        }
        else
        {
            dnx_data_lif.in_lif.feature_get(unit, dnx_data_lif_in_lif_mapped_in_lif_profile) == 0 ?
                (egress_in_lif_profile = in_lif_profile >> IN_LIF_PROFILE_EGRESS_RESERVED_START_BIT) :
                (egress_in_lif_profile = in_lif_profile >> egress_reserved_start_bit);
        }

        (*egress_in_lif_profile_info).ive_inner_qos_dp_profile =
            egress_in_lif_profile & ((1 << IVE_INNER_QOS_DP_PROFILE_NOF_BITS) - 1);
        egress_in_lif_profile = egress_in_lif_profile >> IVE_INNER_QOS_DP_PROFILE_NOF_BITS;
        (*egress_in_lif_profile_info).ive_outer_qos_dp_profile =
            egress_in_lif_profile & ((1 << IVE_OUTER_QOS_DP_PROFILE_NOF_BITS) - 1);
        egress_in_lif_profile = egress_in_lif_profile >> IVE_OUTER_QOS_DP_PROFILE_NOF_BITS;
        (*egress_in_lif_profile_info).in_lif_orientation =
            egress_in_lif_profile & ((1 << IN_LIF_ORIENTATION_NOF_BITS) - 1);
        egress_in_lif_profile = egress_in_lif_profile >> IN_LIF_ORIENTATION_NOF_BITS;
        (*egress_in_lif_profile_info).lif_same_interface_mode =
            egress_in_lif_profile & ((1 << SAME_INTERFACE_MODE_NOF_BITS) - 1);
        (*egress_in_lif_profile_info).preserve_dscp_mode = DISABLE_PRESERVE_DSCP_MODE;
        *in_lif_orientation = 0;
        *in_lif_profile_exclude_orientation = 0;
        *in_lif_profile_jr_mode = 0;
    }
    else if ((dnx_data_headers.system_headers.system_headers_mode_get(unit) ==
              DBAL_ENUM_FVAL_SYSTEM_HEADERS_MODE_JERICHO_MODE)
             && dnx_data_lif.feature.feature_get(unit, dnx_data_lif_feature_iop_mode_orientation_selection))
    {
        SHR_IF_ERR_EXIT(dnx_in_lif_profile_algo_extract_egress_profile_iop_mode
                        (unit, in_lif_profile, egress_in_lif_profile_info, in_lif_profile_jr_mode,
                         in_lif_profile_exclude_orientation, in_lif_orientation));
    }
    else
    {
        SHR_IF_ERR_EXIT(dnx_in_lif_profile_algo_extract_egress_profile_and_orientation_iop_mode
                        (unit, in_lif_profile, egress_in_lif_profile_info, in_lif_profile_jr_mode,
                         in_lif_profile_exclude_orientation, in_lif_orientation));
    }
exit:
    SHR_FUNC_EXIT;
}

/**
 * \brief -
 *  extracts egress eth_rif_profile values from eth_rif_profile
 *
 * \param [in] unit - unit number
 * \param [in] eth_rif_profile - the whole eth_rif_profile value
 * \param [out] egress_in_lif_profile_info - the return struct that includes egress lif_profile fields values
 * \param [out] eth_rif_profile_jr_mode - the eth_rif profile in jr sys headers mode
 * \param [out] eth_rif_profile_exclude_orientation - the eth_rif profile in jr sys headers mode excluding orientation
 * \param [out] eth_rif_orientation - the eth_rif orientation in jr sys headers mode
 *
 * \return
 *   \retval  Negative if error was detected. See \ref shr_error_e
 *
 * \remark
 *   none
 *
 * \see
 *   dnx_in_lif_profile_exchange.
 */
shr_error_e
dnx_eth_rif_profile_algo_extract_egress_profile(
    int unit,
    int eth_rif_profile,
    in_lif_profile_info_egress_t * egress_in_lif_profile_info,
    int *eth_rif_profile_jr_mode,
    int *eth_rif_profile_exclude_orientation,
    int *eth_rif_orientation)
{

    int egress_eth_rif_profile, system_headers_mode, preserve_dscp_mode;
    SHR_FUNC_INIT_VARS(unit);

    SHR_NULL_CHECK(egress_in_lif_profile_info, _SHR_E_PARAM,
                   "A valid pointer for egress_in_lif_profile_info is needed!");
    SHR_NULL_CHECK(eth_rif_profile_jr_mode, _SHR_E_PARAM, "A valid pointer for eth_rif_profile_jr_mode is needed!");
    SHR_NULL_CHECK(eth_rif_profile_exclude_orientation, _SHR_E_PARAM,
                   "A valid pointer for eth_rif_profile_exclude_orientation is needed!");
    SHR_NULL_CHECK(eth_rif_orientation, _SHR_E_PARAM, "A valid pointer for eth_rif_orientation is needed!");
    egress_eth_rif_profile = 0;
    preserve_dscp_mode = dnx_data_qos.qos.routing_dscp_preserve_mode_get(unit);
    system_headers_mode = dnx_data_headers.system_headers.system_headers_mode_get(unit);
    if ((preserve_dscp_mode == PRESERVE_DSCP_PER_OUTLIF_AND_INLIF)
        && (system_headers_mode == DBAL_ENUM_FVAL_SYSTEM_HEADERS_MODE_JERICHO_MODE))
    {
        if (dnx_data_lif.feature.feature_get(unit, dnx_data_lif_feature_iop_mode_orientation_selection))
        {
            egress_eth_rif_profile =
                eth_rif_profile >> (ETH_RIF_PROFILE_PMF_RESERVED_START_BIT - IN_LIF_PROFILE_EGRESS_NOF_BITS_JR_MODE);
        }
        else
        {
            egress_eth_rif_profile = eth_rif_profile >> (ETH_RIF_PROFILE_PMF_RESERVED_START_BIT - 1);
        }
        *eth_rif_profile_jr_mode = egress_eth_rif_profile;
        *eth_rif_profile_exclude_orientation = egress_eth_rif_profile;
        *eth_rif_orientation = 0;
        (*egress_in_lif_profile_info).lif_same_interface_mode = 0;
        (*egress_in_lif_profile_info).in_lif_orientation = 0;
        (*egress_in_lif_profile_info).ive_inner_qos_dp_profile = 0;
        (*egress_in_lif_profile_info).ive_outer_qos_dp_profile = 0;
        (*egress_in_lif_profile_info).preserve_dscp_mode = egress_eth_rif_profile & IN_LIF_PROFILE_DSCP_PRESERVE_MASK;
    }
    else
    {
        *eth_rif_profile_jr_mode = 0;
        *eth_rif_profile_exclude_orientation = 0;
        *eth_rif_orientation = 0;
        (*egress_in_lif_profile_info).lif_same_interface_mode = 0;
        (*egress_in_lif_profile_info).in_lif_orientation = 0;
        (*egress_in_lif_profile_info).ive_inner_qos_dp_profile = 0;
        (*egress_in_lif_profile_info).ive_outer_qos_dp_profile = 0;
        (*egress_in_lif_profile_info).preserve_dscp_mode = DISABLE_PRESERVE_DSCP_MODE;
    }
exit:
    SHR_FUNC_EXIT;
}

/**
 * \brief -
 *  write all egress possible values to HW in init time:
 *  egress attributes are retrieved from the egress reserved bits in a direct way,
 *  no template manager is used, therefore the values we write to egress HW is known
 *  in advanced and in order to save time and enhance performace we do all the HW
 *  work in init stage, after that we only return profiles that point to these HW entries.
 *  when exchange api is called no egress HW is accessed.
 *
 * \param [in] unit - unit number
 *
 * \return
 *   \retval  Negative if error was detected. See \ref shr_error_e
 *
 * \remark
 *   none
 *
 * \see
 *   dnx_in_lif_profile_module_init.
 */
static shr_error_e
dnx_in_lif_profile_set_egress_hw(
    int unit)
{
    uint32 entry_handle_id;
    int pmf_reserved_nof_bits, nof_ingress_profiles, system_headers_mode, in_lif_profile_jr_mode;
    int pmf_reserved_bits_value, ingress_bits_value, egress_reserved_start_bit, same_interface, indexed_mode_nof_bits,
        indexed_mode;
    in_lif_profile_info_egress_t egress_fields_info;
    int in_lif_egress_profile, max_in_lif_egress_profile, in_lif_profile_exclude_orientation, in_lif_orientation,
        preserve_dscp_number_of_bits, orientation_number_of_values, same_interface_number_of_bits;

    SHR_FUNC_INIT_VARS(unit);
    DBAL_FUNC_INIT_VARS(unit);

    pmf_reserved_nof_bits = 0;
    nof_ingress_profiles = 0;
    pmf_reserved_bits_value = 0;
    ingress_bits_value = 0;
    in_lif_profile_exclude_orientation = 0;
    in_lif_orientation = 0;
    preserve_dscp_number_of_bits = 0;
    in_lif_profile_jr_mode = 0;
    sal_memset(&egress_fields_info, 0, sizeof(in_lif_profile_info_egress_t));

    /*
     * write all possible egress HW combinations, we do this part at init in order to save time
     * when we exchange, these written HW values won't change anyway
     */
    pmf_reserved_nof_bits = IN_LIF_PROFILE_PMF_RESERVED_NOF_BITS;
    nof_ingress_profiles = NOF_IN_LIF_PROFILES;
    indexed_mode_nof_bits = NATIVE_INDEXED_MODE_NOF_BITS;

    system_headers_mode = dnx_data_headers.system_headers.system_headers_mode_get(unit);
    same_interface = dnx_data_lif.in_lif.in_lif_profile_allocate_same_interface_mode_get(unit);
    orientation_number_of_values = dnx_data_lif.in_lif.in_lif_profile_allocate_orientation_get(unit);
     /**
     * In J2P we support orientation and same interface working independantly,
     * as same_interface mode can only device scope/ only system scope - for which we need 1 bit
     * or both device and system scope enabled together - 2bits
     */
    if ((dnx_data_lif.in_lif.in_lif_profile_allocate_same_interface_mode_get(unit) ==
         JR_COMP_MODE_ONLY_SYS_SCOPE_ENABLED_BY_SOC_PROPERTIES)
        || (dnx_data_lif.in_lif.in_lif_profile_allocate_same_interface_mode_get(unit) ==
            JR_COMP_MODE_ONLY_DEV_SCOPE_ENABLED_BY_SOC_PROPERTIES))
    {
        same_interface_number_of_bits = 1;
    }
    else if (dnx_data_lif.in_lif.in_lif_profile_allocate_same_interface_mode_get(unit) ==
             JR_COMP_MODE_BOTH_DEV_AND_SYS_SCOPE_ENABLED_BY_SOC_PROPERTIES)
    {
        same_interface_number_of_bits = 2;
    }
    else
    {
        same_interface_number_of_bits = 0;
    }
    /**
     * The default mode is routing_dscp_preserve_mode = 2, which means preserve per outlif.
     * If the mode is routing_dscp_preserve_mode = 1, then we preserve per inlif as well and
     * we need to allocate 1 bit for it.
     */
    if (dnx_data_qos.qos.routing_dscp_preserve_mode_get(unit) == PRESERVE_DSCP_PER_OUTLIF_AND_INLIF)
    {
        preserve_dscp_number_of_bits = 1;
    }
    /**Decide the number of egress bits to support*/
    if (system_headers_mode == DBAL_ENUM_FVAL_SYSTEM_HEADERS_MODE_JERICHO2_MODE)
    {
        max_in_lif_egress_profile = (1 << IN_LIF_PROFILE_EGRESS_RESERVED_NOF_BITS) - 1;
        egress_reserved_start_bit = IN_LIF_PROFILE_EGRESS_RESERVED_START_BIT;
    }
    else
    {
        if (dnx_data_lif.feature.feature_get(unit, dnx_data_lif_feature_iop_mode_orientation_selection))
        {
            max_in_lif_egress_profile = (1 << (IN_LIF_PROFILE_EGRESS_NOF_BITS_JR_MODE)) - 1;
            egress_reserved_start_bit = IN_LIF_PROFILE_PMF_RESERVED_START_BIT - IN_LIF_PROFILE_EGRESS_NOF_BITS_JR_MODE;
        }
        else
        {
            max_in_lif_egress_profile = (1 << (IN_LIF_ORIENTATION_NOF_BITS + IVE_INNER_QOS_DP_PROFILE_NOF_BITS +
                                               same_interface_number_of_bits + preserve_dscp_number_of_bits)) - 1;
            egress_reserved_start_bit =
                IN_LIF_PROFILE_PMF_RESERVED_START_BIT - (IN_LIF_ORIENTATION_NOF_BITS +
                                                         IVE_INNER_QOS_DP_PROFILE_NOF_BITS +
                                                         same_interface_number_of_bits + preserve_dscp_number_of_bits);
        }
    }

    SHR_IF_ERR_EXIT(DBAL_HANDLE_ALLOC(unit, DBAL_TABLE_IN_LIF_PROFILE_TABLE, &entry_handle_id));

    for (in_lif_egress_profile = 0; in_lif_egress_profile <= max_in_lif_egress_profile; in_lif_egress_profile++)
    {

        SHR_IF_ERR_EXIT(dnx_in_lif_profile_algo_extract_egress_profile
                        (unit, in_lif_egress_profile << egress_reserved_start_bit, &egress_fields_info,
                         &in_lif_profile_jr_mode, &in_lif_profile_exclude_orientation, &in_lif_orientation, LIF));

        dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_IN_LIF_ORIENTATION, INST_SINGLE,
                                     egress_fields_info.in_lif_orientation);

        dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_INNER_DP_PROFILE, INST_SINGLE,
                                     egress_fields_info.ive_inner_qos_dp_profile);

        dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_OUTER_DP_PROFILE, INST_SINGLE,
                                     egress_fields_info.ive_outer_qos_dp_profile);

        dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_IN_LIF_SAME_INTERFACE, INST_SINGLE,
                                     egress_fields_info.lif_same_interface_mode);
        if (dnx_data_lif.in_lif.feature_get(unit, dnx_data_lif_in_lif_in_lif_profile_iop_mode))
        {
            dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_JR_MODE_IN_LIF_PROFILE, INST_SINGLE,
                                         in_lif_profile_jr_mode);
        }
        if (!dnx_data_lif.feature.feature_get(unit, dnx_data_lif_feature_iop_mode_orientation_selection) &&
            system_headers_mode == DBAL_ENUM_FVAL_SYSTEM_HEADERS_MODE_JERICHO_MODE)
        {
            dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_JR_IN_LIF_ORIENTATION,
                                         INST_SINGLE, egress_fields_info.in_lif_orientation);
        }

        for (pmf_reserved_bits_value = 0; pmf_reserved_bits_value < (1 << pmf_reserved_nof_bits);
             pmf_reserved_bits_value++)
        {
            for (ingress_bits_value = 0; ingress_bits_value < nof_ingress_profiles; ingress_bits_value++)
            {
                for (indexed_mode = 0; indexed_mode < (indexed_mode_nof_bits + 1); indexed_mode++)
                {
                    if (dnx_data_lif.in_lif.feature_get(unit, dnx_data_lif_in_lif_mapped_in_lif_profile))
                    {
                        dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_MAPPED_IN_LIF_PROFILE,
                                                     INST_SINGLE,
                                                     ingress_bits_value | (indexed_mode <<
                                                                           (IN_LIF_PROFILE_NATIVE_INDEXED_MODE_START_BIT))
                                                     | (in_lif_egress_profile << (egress_reserved_start_bit)) |
                                                     (pmf_reserved_bits_value <<
                                                      (IN_LIF_PROFILE_PMF_RESERVED_START_BIT)));
                    }
                    dbal_entry_key_field32_set(unit, entry_handle_id, DBAL_FIELD_IN_LIF_PROFILE,
                                               ingress_bits_value | (indexed_mode <<
                                                                     (IN_LIF_PROFILE_NATIVE_INDEXED_MODE_START_BIT)) |
                                               (in_lif_egress_profile << (egress_reserved_start_bit)) |
                                               (pmf_reserved_bits_value << (IN_LIF_PROFILE_PMF_RESERVED_START_BIT)));
                    SHR_IF_ERR_EXIT(dbal_entry_commit(unit, entry_handle_id, DBAL_COMMIT));
                }
            }
        }
        if ((system_headers_mode == DBAL_ENUM_FVAL_SYSTEM_HEADERS_MODE_JERICHO_MODE) &&
            (dnx_data_lif.feature.feature_get(unit, dnx_data_lif_feature_iop_mode_orientation_selection)))
        {
            /** Configure the mapping between resolved in_lif_profile (2'b) and in_lif_orientation*/
            SHR_IF_ERR_EXIT(DBAL_HANDLE_CLEAR(unit, DBAL_TABLE_JR_MODE_INLIF_PROFILE_ORIENTATION_MAP, entry_handle_id));
            dbal_entry_key_field32_set(unit, entry_handle_id, DBAL_FIELD_JR_MODE_IN_LIF_PROFILE,
                                       in_lif_profile_jr_mode);
            dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_JR_IN_LIF_ORIENTATION, INST_SINGLE,
                                         egress_fields_info.in_lif_orientation);
            SHR_IF_ERR_EXIT(dbal_entry_commit(unit, entry_handle_id, DBAL_COMMIT));

            /**Configure the egress table to receive the inlif profile*/
            SHR_IF_ERR_EXIT(DBAL_HANDLE_CLEAR(unit, DBAL_TABLE_JR_MODE_INLIF_PROFILE_EGRESS_REMAP, entry_handle_id));
            dbal_entry_key_field32_set(unit, entry_handle_id, DBAL_FIELD_JR_MODE_SYSTEM_HEADER_RESERVED_VALUE, 0);
            dbal_entry_key_field32_set(unit, entry_handle_id, DBAL_FIELD_JR_IN_LIF_ORIENTATION, in_lif_orientation);
            dbal_entry_key_field32_set(unit, entry_handle_id, DBAL_FIELD_JR_MODE_IN_LIF_PROFILE,
                                       in_lif_profile_exclude_orientation);

            dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_IN_LIF_PROFILE, INST_SINGLE,
                                         (in_lif_egress_profile << (egress_reserved_start_bit)));
            SHR_IF_ERR_EXIT(dbal_entry_commit(unit, entry_handle_id, DBAL_COMMIT));
            if ((((orientation_number_of_values > 0) && (same_interface == 0))
                 || ((orientation_number_of_values > 0)
                     && (preserve_dscp_number_of_bits != PRESERVE_DSCP_PER_OUTLIF_AND_INLIF))) && ((same_interface == 0)
                                                                                                   &&
                                                                                                   (preserve_dscp_number_of_bits
                                                                                                    !=
                                                                                                    PRESERVE_DSCP_PER_OUTLIF_AND_INLIF)))
            {
                /**
                 * The table should be updated only in case we have the following soc properties configuration:
                 * orientation - enabled and same_interface, preserve_dscp - disabled
                 */
                dbal_entry_key_field32_set(unit, entry_handle_id, DBAL_FIELD_JR_MODE_SYSTEM_HEADER_RESERVED_VALUE, 0);
                dbal_entry_key_field32_set(unit, entry_handle_id, DBAL_FIELD_JR_IN_LIF_ORIENTATION, in_lif_orientation);
                dbal_entry_key_field32_set(unit, entry_handle_id, DBAL_FIELD_JR_MODE_IN_LIF_PROFILE, 0);
                dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_IN_LIF_PROFILE, INST_SINGLE,
                                             (in_lif_egress_profile << (egress_reserved_start_bit)));
                SHR_IF_ERR_EXIT(dbal_entry_commit(unit, entry_handle_id, DBAL_COMMIT));
            }
            if (((orientation_number_of_values > 0) && (same_interface > 0))
                || ((orientation_number_of_values > 0)
                    && (preserve_dscp_number_of_bits == PRESERVE_DSCP_PER_OUTLIF_AND_INLIF)))
            {
                dbal_entry_key_field32_set(unit, entry_handle_id, DBAL_FIELD_JR_MODE_SYSTEM_HEADER_RESERVED_VALUE, 0);
                dbal_entry_key_field32_set(unit, entry_handle_id, DBAL_FIELD_JR_IN_LIF_ORIENTATION, in_lif_orientation);
                dbal_entry_key_field32_set(unit, entry_handle_id, DBAL_FIELD_JR_MODE_IN_LIF_PROFILE,
                                           (in_lif_profile_exclude_orientation & 1));
                dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_IN_LIF_PROFILE, INST_SINGLE,
                                             (in_lif_egress_profile << (egress_reserved_start_bit)));
                SHR_IF_ERR_EXIT(dbal_entry_commit(unit, entry_handle_id, DBAL_COMMIT));
            }
            SHR_IF_ERR_EXIT(DBAL_HANDLE_CLEAR(unit, DBAL_TABLE_IN_LIF_PROFILE_TABLE, entry_handle_id));
        }
        /** If orientation and IOP lif profile are indexed by ingress lif profile, just recover the table*/
        if ((system_headers_mode == DBAL_ENUM_FVAL_SYSTEM_HEADERS_MODE_JERICHO_MODE) &&
            !(dnx_data_lif.feature.feature_get(unit, dnx_data_lif_feature_iop_mode_orientation_selection)))
        {
           /**Configure the egress table to receive the inlif profile*/
            SHR_IF_ERR_EXIT(DBAL_HANDLE_CLEAR(unit, DBAL_TABLE_JR_MODE_INLIF_PROFILE_EGRESS_REMAP, entry_handle_id));
            dbal_entry_key_field32_set(unit, entry_handle_id, DBAL_FIELD_JR_MODE_SYSTEM_HEADER_RESERVED_VALUE, 0);
            dbal_entry_key_field32_set(unit, entry_handle_id, DBAL_FIELD_JR_IN_LIF_ORIENTATION, in_lif_orientation);
            dbal_entry_key_field32_set(unit, entry_handle_id, DBAL_FIELD_JR_MODE_IN_LIF_PROFILE,
                                       in_lif_profile_exclude_orientation);

            dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_IN_LIF_PROFILE, INST_SINGLE,
                                         (in_lif_egress_profile << (egress_reserved_start_bit)));
            SHR_IF_ERR_EXIT(dbal_entry_commit(unit, entry_handle_id, DBAL_COMMIT));

            /**
             * Handle lif_profile recovery when the system is comprised with devices, which have the feature_iop_mode_orientation_selection
             * The below configuration is done, because the in_lif_profile and in_lif_orientation fields have the same values.
             * The case is when orientation takes the whole 2bits of the configuration.
             */
            if ((orientation_number_of_values > 0) && (same_interface == 0) && (preserve_dscp_number_of_bits == 0))
            {
                dbal_entry_key_field32_set(unit, entry_handle_id, DBAL_FIELD_JR_MODE_SYSTEM_HEADER_RESERVED_VALUE, 0);
                dbal_entry_key_field32_set(unit, entry_handle_id, DBAL_FIELD_JR_IN_LIF_ORIENTATION, in_lif_orientation);
                dbal_entry_key_field32_set(unit, entry_handle_id, DBAL_FIELD_JR_MODE_IN_LIF_PROFILE,
                                           in_lif_orientation);
                dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_IN_LIF_PROFILE, INST_SINGLE,
                                             (in_lif_egress_profile << (egress_reserved_start_bit)));
                SHR_IF_ERR_EXIT(dbal_entry_commit(unit, entry_handle_id, DBAL_COMMIT));
            }
            if (((orientation_number_of_values == 2) && (same_interface > 0))
                || ((orientation_number_of_values == 2) && (preserve_dscp_number_of_bits == 1)))
            {
                dbal_entry_key_field32_set(unit, entry_handle_id, DBAL_FIELD_JR_MODE_SYSTEM_HEADER_RESERVED_VALUE, 0);
                dbal_entry_key_field32_set(unit, entry_handle_id, DBAL_FIELD_JR_IN_LIF_ORIENTATION, in_lif_orientation);
                dbal_entry_key_field32_set(unit, entry_handle_id, DBAL_FIELD_JR_MODE_IN_LIF_PROFILE,
                                           ((in_lif_orientation << 1) | in_lif_profile_exclude_orientation));
                dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_IN_LIF_PROFILE, INST_SINGLE,
                                             (in_lif_egress_profile << (egress_reserved_start_bit)));
                SHR_IF_ERR_EXIT(dbal_entry_commit(unit, entry_handle_id, DBAL_COMMIT));
            }
            SHR_IF_ERR_EXIT(DBAL_HANDLE_CLEAR(unit, DBAL_TABLE_IN_LIF_PROFILE_TABLE, entry_handle_id));
        }
    }
exit:
    DBAL_FUNC_FREE_VARS;
    SHR_FUNC_EXIT;
}

/**
 * \brief -
 *  Preconfigure the egress properties for the mapped in lif profile table
 *
 * \param [in] unit - unit number
 *
 * \return
 *   \retval  Negative if error was detected. See \ref shr_error_e
 *
 * \remark
 *   none
 *
 * \see
 *   dnx_in_lif_profile_module_init.
 */
static shr_error_e
dnx_in_lif_profile_set_mapped_in_lif_profile(
    int unit)
{
    uint32 entry_handle_id;
    int pmf_reserved_nof_bits, nof_ingress_profiles, in_lif_profile_jr_mode,
        pmf_reserved_bits_value, ingress_bits_value, egress_reserved_start_bit, indexed_mode_nof_bits,
        indexed_mode, in_lif_egress_profile, max_in_lif_egress_profile, in_lif_profile_exclude_orientation,
        in_lif_orientation, egress_properties_nof_bits;
    in_lif_profile_info_egress_t egress_fields_info;

    SHR_FUNC_INIT_VARS(unit);
    DBAL_FUNC_INIT_VARS(unit);

    in_lif_profile_exclude_orientation = 0;
    in_lif_orientation = 0;
    in_lif_profile_jr_mode = 0;
    sal_memset(&egress_fields_info, 0, sizeof(in_lif_profile_info_egress_t));
    pmf_reserved_nof_bits = IN_LIF_PROFILE_PMF_RESERVED_NOF_BITS;
    indexed_mode_nof_bits = NATIVE_INDEXED_MODE_NOF_BITS;

    egress_properties_nof_bits = dnx_data_lif.in_lif.nof_in_lif_profile_allocate_orientation_bits_get(unit) +
        dnx_data_lif.in_lif.nof_in_lif_profile_allocate_policer_inner_dp_bits_get(unit) +
        dnx_data_lif.in_lif.nof_in_lif_profile_allocate_policer_outer_dp_bits_get(unit) +
        dnx_data_lif.in_lif.nof_in_lif_profile_allocate_same_interface_mode_bits_get(unit);
    nof_ingress_profiles =
        (1 <<
         (NOF_IN_LIF_PROFILE_BITS - egress_properties_nof_bits - IN_LIF_PROFILE_PMF_RESERVED_NOF_BITS -
          NATIVE_INDEXED_MODE_NOF_BITS));
    max_in_lif_egress_profile = (1 << egress_properties_nof_bits) - 1;
    egress_reserved_start_bit =
        dnx_data_lif.in_lif.in_lif_profile_pmf_reserved_start_bit_get(unit) - egress_properties_nof_bits;

    SHR_IF_ERR_EXIT(DBAL_HANDLE_ALLOC(unit, DBAL_TABLE_MAPPED_IN_LIF_PROFILE_TABLE, &entry_handle_id));
    for (in_lif_egress_profile = 0; in_lif_egress_profile <= max_in_lif_egress_profile; in_lif_egress_profile++)
    {

        SHR_IF_ERR_EXIT(dnx_in_lif_profile_algo_extract_egress_profile
                        (unit, in_lif_egress_profile << egress_reserved_start_bit, &egress_fields_info,
                         &in_lif_profile_jr_mode, &in_lif_profile_exclude_orientation, &in_lif_orientation, LIF));

        dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_IN_LIF_ORIENTATION, INST_SINGLE,
                                     egress_fields_info.in_lif_orientation);

        dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_INNER_DP_PROFILE, INST_SINGLE,
                                     egress_fields_info.ive_inner_qos_dp_profile);

        dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_OUTER_DP_PROFILE, INST_SINGLE,
                                     egress_fields_info.ive_outer_qos_dp_profile);

        dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_IN_LIF_SAME_INTERFACE, INST_SINGLE,
                                     egress_fields_info.lif_same_interface_mode);

        for (pmf_reserved_bits_value = 0; pmf_reserved_bits_value < (1 << pmf_reserved_nof_bits);
             pmf_reserved_bits_value++)
        {
            for (ingress_bits_value = 0; ingress_bits_value < nof_ingress_profiles; ingress_bits_value++)
            {
                for (indexed_mode = 0; indexed_mode < (indexed_mode_nof_bits + 1); indexed_mode++)
                {
                    dbal_entry_key_field32_set(unit, entry_handle_id, DBAL_FIELD_MAPPED_IN_LIF_PROFILE,
                                               ingress_bits_value | (indexed_mode <<
                                                                     (IN_LIF_PROFILE_NATIVE_INDEXED_MODE_START_BIT)) |
                                               (in_lif_egress_profile << (egress_reserved_start_bit)) |
                                               (pmf_reserved_bits_value << (IN_LIF_PROFILE_PMF_RESERVED_START_BIT)));
                    SHR_IF_ERR_EXIT(dbal_entry_commit(unit, entry_handle_id, DBAL_COMMIT));
                }
            }
        }
    }
exit:
    DBAL_FUNC_FREE_VARS;
    SHR_FUNC_EXIT;
}

/**
 * \brief -
 *  write all egress possible values to HW in init time:
 *  egress attributes are retrieved from the egress reserved bits in a direct way,
 *  no template manager is used, therefore the values we write to egress HW is known
 *  in advanced and in order to save time and enhance performace we do all the HW
 *  work in init stage, after that we only return profiles that point to these HW entries.
 *  when exchange api is called no egress HW is accessed.
 *
 * \param [in] unit - unit number
 *
 * \return
 *   \retval  Negative if error was detected. See \ref shr_error_e
 *
 * \remark
 *   none
 *
 * \see
 *   dnx_in_lif_profile_module_init.
 */
static shr_error_e
dnx_eth_rif_profile_set_egress_hw(
    int unit)
{
    uint32 entry_handle_id;
    int pmf_reserved_nof_bits, nof_ingress_profiles, eth_rif_profile_jr_mode;
    int pmf_reserved_bits_value, ingress_bits_value, egress_reserved_start_bit;
    in_lif_profile_info_egress_t egress_fields_info;
    int eth_rif_egress_profile, max_eth_rif_egress_profile, eth_rif_profile_exclude_orientation, eth_rif_orientation;

    SHR_FUNC_INIT_VARS(unit);
    DBAL_FUNC_INIT_VARS(unit);

    pmf_reserved_nof_bits = 0;
    nof_ingress_profiles = 0;
    pmf_reserved_bits_value = 0;
    ingress_bits_value = 0;
    eth_rif_profile_exclude_orientation = 0;
    eth_rif_orientation = 0;
    sal_memset(&egress_fields_info, 0, sizeof(in_lif_profile_info_egress_t));

    /*
     * write all possible egress HW combinations, we do this part at init in order to save time
     * when we exchange, these written HW values won't change anyway
     */
    pmf_reserved_nof_bits = ETH_RIF_PROFILE_PMF_RESERVED_NOF_BITS;
    nof_ingress_profiles = NOF_ETH_RIF_PROFILES;
    max_eth_rif_egress_profile = (dnx_data_qos.qos.routing_dscp_preserve_mode_get(unit) == 1) ? 1 : 0;
    if (dnx_data_lif.feature.feature_get(unit, dnx_data_lif_feature_iop_mode_orientation_selection))
    {
        egress_reserved_start_bit = ETH_RIF_PROFILE_PMF_RESERVED_START_BIT - IN_LIF_PROFILE_EGRESS_NOF_BITS_JR_MODE;
    }
    else
    {
        egress_reserved_start_bit = ETH_RIF_PROFILE_PMF_RESERVED_START_BIT - 1;
    }

    SHR_IF_ERR_EXIT(DBAL_HANDLE_ALLOC(unit, DBAL_TABLE_IN_LIF_PROFILE_TABLE, &entry_handle_id));
    for (eth_rif_egress_profile = 0; eth_rif_egress_profile <= max_eth_rif_egress_profile; eth_rif_egress_profile++)
    {
        SHR_IF_ERR_EXIT(dnx_eth_rif_profile_algo_extract_egress_profile
                        (unit, eth_rif_egress_profile << egress_reserved_start_bit, &egress_fields_info,
                         &eth_rif_profile_jr_mode, &eth_rif_profile_exclude_orientation, &eth_rif_orientation));

        dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_IN_LIF_ORIENTATION, INST_SINGLE,
                                     egress_fields_info.in_lif_orientation);

        dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_INNER_DP_PROFILE, INST_SINGLE,
                                     egress_fields_info.ive_inner_qos_dp_profile);

        dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_OUTER_DP_PROFILE, INST_SINGLE,
                                     egress_fields_info.ive_outer_qos_dp_profile);

        dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_IN_LIF_SAME_INTERFACE, INST_SINGLE,
                                     egress_fields_info.lif_same_interface_mode);
        if (dnx_data_lif.in_lif.feature_get(unit, dnx_data_lif_in_lif_in_lif_profile_iop_mode))
        {
            dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_JR_MODE_IN_LIF_PROFILE, INST_SINGLE,
                                         eth_rif_profile_jr_mode);
        }

        for (pmf_reserved_bits_value = 0; pmf_reserved_bits_value < (1 << pmf_reserved_nof_bits);
             pmf_reserved_bits_value++)
        {
            for (ingress_bits_value = 0; ingress_bits_value < nof_ingress_profiles; ingress_bits_value++)
            {
                if (dnx_data_lif.in_lif.feature_get(unit, dnx_data_lif_in_lif_mapped_in_lif_profile))
                {
                    dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_MAPPED_IN_LIF_PROFILE, INST_SINGLE,
                                                 CALCULATE_IN_LIF_PROFILE_FROM_ETH_RIF_PROFILE
                                                 (IN_LIF_PROFILE_TABLE_MSB_PREFIX,
                                                  (ingress_bits_value |
                                                   (eth_rif_egress_profile << (egress_reserved_start_bit)) |
                                                   (pmf_reserved_bits_value <<
                                                    (IN_LIF_PROFILE_PMF_RESERVED_START_BIT)))));
                }
                dbal_entry_key_field32_set(unit, entry_handle_id, DBAL_FIELD_IN_LIF_PROFILE,
                                           CALCULATE_IN_LIF_PROFILE_FROM_ETH_RIF_PROFILE
                                           (IN_LIF_PROFILE_TABLE_MSB_PREFIX,
                                            (ingress_bits_value |
                                             (eth_rif_egress_profile << (egress_reserved_start_bit)) |
                                             (pmf_reserved_bits_value << (IN_LIF_PROFILE_PMF_RESERVED_START_BIT)))));
                SHR_IF_ERR_EXIT(dbal_entry_commit(unit, entry_handle_id, DBAL_COMMIT));
            }
        }
        if (!dnx_data_lif.in_lif.feature_get(unit, dnx_data_lif_in_lif_mapped_in_lif_profile))
        {
            SHR_IF_ERR_EXIT(DBAL_HANDLE_CLEAR(unit, DBAL_TABLE_JR_MODE_INLIF_PROFILE_ORIENTATION_MAP, entry_handle_id));
            dbal_entry_key_field32_set(unit, entry_handle_id, DBAL_FIELD_JR_MODE_IN_LIF_PROFILE,
                                       eth_rif_profile_jr_mode);
            dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_JR_IN_LIF_ORIENTATION, INST_SINGLE,
                                         egress_fields_info.in_lif_orientation);
            SHR_IF_ERR_EXIT(dbal_entry_commit(unit, entry_handle_id, DBAL_COMMIT));

            /**Configure the egress table to receive the eth_rif profile*/
            SHR_IF_ERR_EXIT(DBAL_HANDLE_CLEAR(unit, DBAL_TABLE_JR_MODE_INLIF_PROFILE_EGRESS_REMAP, entry_handle_id));
            dbal_entry_key_field32_set(unit, entry_handle_id, DBAL_FIELD_JR_MODE_SYSTEM_HEADER_RESERVED_VALUE, 0);
            dbal_entry_key_field32_set(unit, entry_handle_id, DBAL_FIELD_JR_IN_LIF_ORIENTATION, eth_rif_orientation);
            dbal_entry_key_field32_set(unit, entry_handle_id, DBAL_FIELD_JR_MODE_IN_LIF_PROFILE,
                                       eth_rif_profile_exclude_orientation);

            dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_IN_LIF_PROFILE, INST_SINGLE,
                                         0 | (eth_rif_egress_profile << (egress_reserved_start_bit)) | (0 <<
                                                                                                        (IN_LIF_PROFILE_PMF_RESERVED_START_BIT)));
            SHR_IF_ERR_EXIT(dbal_entry_commit(unit, entry_handle_id, DBAL_COMMIT));
            SHR_IF_ERR_EXIT(DBAL_HANDLE_CLEAR(unit, DBAL_TABLE_IN_LIF_PROFILE_TABLE, entry_handle_id));
        }
    }
exit:
    DBAL_FUNC_FREE_VARS;
    SHR_FUNC_EXIT;
}

/** see .h file */
shr_error_e
dnx_enablers_vectors_initial_set(
    int unit,
    uint32 *routing_enable_vector)
{
    uint32 hw_enum_value;
    int idx;
    uint8 uc_enable = 1;
    uint8 mc_disable = 0;
    SHR_FUNC_INIT_VARS(unit);
    sal_memset(routing_enable_vector, 0,
               sizeof(routing_enable_vector[0]) * DNX_DATA_MAX_L3_ROUTING_ENABLERS_ROUTING_ENABLE_VECTOR_LENGTH /
               SAL_UINT32_NOF_BITS);
    SHR_IF_ERR_EXIT(dbal_fields_enum_hw_value_get
                    (unit, DBAL_FIELD_LAYER_TYPES, DBAL_ENUM_FVAL_LAYER_TYPES_ETHERNET, &hw_enum_value));
    idx =
        hw_enum_value / (dnx_data_device.general.max_nof_layer_protocols_get(unit) /
                         ROUTING_ENABLERS_VECTOR_SIZE_IN_UINT32(unit));
    if (dnx_data_l3.routing_enablers.routing_enable_action_support_get(unit))
    {
        uc_enable = dnx_data_l3.routing_enablers.routing_enabled_action_profile_id_get(unit);
    }
    routing_enable_vector[idx] =
        routing_enable_vector[idx] | dnx_in_lif_profile_get_routing_enablers_vector(unit, hw_enum_value,
                                                                                    mc_disable, uc_enable);
exit:
    SHR_FUNC_EXIT;
}

/*
 * Function used to set default values to ENABLERS_VECTORS
 */
static shr_error_e
dnx_enablers_vectors_table_init(
    int unit,
    int routing_enablers_profile,
    int is_dealloc)
{
    uint32 entry_handle_id,
        default_enablers[DNX_DATA_MAX_L3_ROUTING_ENABLERS_ROUTING_ENABLE_VECTOR_LENGTH / SAL_UINT32_NOF_BITS];
    SHR_FUNC_INIT_VARS(unit);
    DBAL_FUNC_INIT_VARS(unit);

    /*
     * set default enablers vector value
     * By default, Eth enabler is turned on.
     */
    SHR_IF_ERR_EXIT(dnx_enablers_vectors_initial_set(unit, default_enablers));

    SHR_IF_ERR_EXIT(DBAL_HANDLE_ALLOC(unit, DBAL_TABLE_ENABLERS_VECTORS, &entry_handle_id));
    dbal_entry_key_field32_set(unit, entry_handle_id, DBAL_FIELD_ROUTING_ENABLERS_PROFILE, routing_enablers_profile);
    dbal_entry_value_field_arr32_set(unit, entry_handle_id, DBAL_FIELD_ENABLERS_VECTOR, INST_SINGLE, default_enablers);
    if (is_dealloc)
    {
        SHR_IF_ERR_EXIT(dbal_entry_clear(unit, entry_handle_id, DBAL_COMMIT));
    }
    else
    {
        SHR_IF_ERR_EXIT(dbal_entry_commit(unit, entry_handle_id, DBAL_COMMIT));
    }

exit:
    DBAL_FUNC_FREE_VARS;
    SHR_FUNC_EXIT;
}
/*
 * Function used to set default values to VSI_PROFILE_TABLE
 */
static shr_error_e
dnx_vsi_profile_table_init(
    int unit,
    int in_lif_profile,
    int is_dealloc,
    int pmf_reserved_nof_bits,
    int egress_reserved_nof_bits,
    int intf_type)
{
    int max_counter_value, hw_write_counter;
    uint32 entry_handle_id, in_lif_profile_hw;
    uint8 ipv4_fwd_type_sel;
    uint8 ipv6_fwd_type_sel;
    SHR_FUNC_INIT_VARS(unit);
    DBAL_FUNC_INIT_VARS(unit);

    SHR_IF_ERR_EXIT(DBAL_HANDLE_ALLOC(unit, DBAL_TABLE_VSI_PROFILE_TABLE, &entry_handle_id));
    /** Restore the default init configuration for all the egress allocated bits*/
    dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_ROUTING_ENABLERS_PROFILE, INST_SINGLE,
                                 DEFAULT_ROUTING_ENABLERS_PROFILE);

#ifdef BCM_DNX2_SUPPORT
    if (dnx_data_dev_init.general.feature_get(unit, dnx_data_dev_init_general_application_v1))
    {
        dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_VSI_PROFILE_BRIDGE, INST_SINGLE,
                                     DBAL_ENUM_FVAL_VSI_PROFILE_BRIDGE_SVL);
    }
#endif

    SHR_IF_ERR_EXIT(switch_db.l3mcastl2_ipv4_fwd_type.get(unit, &ipv4_fwd_type_sel));
    SHR_IF_ERR_EXIT(switch_db.l3mcastl2_ipv6_fwd_type.get(unit, &ipv6_fwd_type_sel));
    dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_L2_V4_MC_FWD_TYPE, INST_SINGLE, ipv4_fwd_type_sel);
    dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_L2_V6_MC_FWD_TYPE, INST_SINGLE, ipv6_fwd_type_sel);

    if (is_dealloc)
    {
        /*
         * loop over the possible values of the reserved bits, and duplicate HW value to all the possible keys
         * the key is built by shifting the reserved bits value left and append new_in_lif_profile
         */
        if (intf_type == ETH_RIF)
        {
            max_counter_value = 1 << (pmf_reserved_nof_bits + egress_reserved_nof_bits);
        }
        else
        {
            max_counter_value = 1 << (pmf_reserved_nof_bits + egress_reserved_nof_bits + NATIVE_INDEXED_MODE_NOF_BITS);
        }
        for (hw_write_counter = 0; hw_write_counter < max_counter_value; hw_write_counter++)
        {
            if (intf_type == ETH_RIF)
            {
                in_lif_profile_hw = CALCULATE_VSI_PROFILE_FROM_ETH_RIF_PROFILE(VSI_MSB_PREFIX,
                                                                               (in_lif_profile |
                                                                                (hw_write_counter <<
                                                                                 (NOF_ETH_RIF_PROFILE_BITS -
                                                                                  (pmf_reserved_nof_bits +
                                                                                   egress_reserved_nof_bits)))));
            }
            else
            {
                in_lif_profile_hw = in_lif_profile | (hw_write_counter <<
                                                      (NOF_IN_LIF_PROFILE_BITS -
                                                       (pmf_reserved_nof_bits + egress_reserved_nof_bits)));
            }
            dbal_entry_key_field32_set(unit, entry_handle_id, DBAL_FIELD_VSI_PROFILE, in_lif_profile_hw);
            SHR_IF_ERR_EXIT(dbal_entry_commit(unit, entry_handle_id, DBAL_COMMIT));
        }
    }
    else
    {
        /*
         * routing enablers profile init
         * this table has a key of 10 bits, and can be accessed with two kinds of profiles:
         * 1. in_lif_profile : 8 bits, in this case the 2 MSBs of the key are set to 0 by ARR
         * 2. eth_rif_profile: 7 bits, in this case the 3 MSBs are set to a static prefix 111 by ARR
         * so we need to initiate the two entries 0000000000 and 1110000000 with the default values
         */
        /*
         * in_lif_profile
         */
        dbal_entry_key_field32_set(unit, entry_handle_id, DBAL_FIELD_VSI_PROFILE, DNX_IN_LIF_PROFILE_DEFAULT);
        SHR_IF_ERR_EXIT(dbal_entry_commit(unit, entry_handle_id, DBAL_COMMIT));
        /*
         * eth_rif_profile
         */
        dbal_entry_key_field32_set(unit, entry_handle_id, DBAL_FIELD_VSI_PROFILE,
                                   CALCULATE_VSI_PROFILE_FROM_ETH_RIF_PROFILE(VSI_MSB_PREFIX,
                                                                              DNX_IN_LIF_PROFILE_DEFAULT));
        SHR_IF_ERR_EXIT(dbal_entry_commit(unit, entry_handle_id, DBAL_COMMIT));
    }
exit:
    DBAL_FUNC_FREE_VARS;
    SHR_FUNC_EXIT;
}

/*
 * Function used to set default values to VSI_PROFILE_TABLE
 */
static shr_error_e
dnx_vsi_profile_table_for_mapped_in_lif_profile_dealloc(
    int unit,
    int in_lif_profile,
    int pmf_reserved_nof_bits,
    int intf_type)
{
    int max_counter_value, hw_write_counter;
    uint32 entry_handle_id, in_lif_profile_hw;

    SHR_FUNC_INIT_VARS(unit);
    DBAL_FUNC_INIT_VARS(unit);

    SHR_IF_ERR_EXIT(DBAL_HANDLE_ALLOC(unit, DBAL_TABLE_VSI_PROFILE_TABLE, &entry_handle_id));
    /** Restore the default init configuration for all the egress allocated bits*/

    /*
     * loop over the possible values of the reserved bits, and duplicate HW value to all the possible keys
     * the key is built by shifting the reserved bits value left and append new_in_lif_profile
     */
    if (intf_type == ETH_RIF)
    {
        max_counter_value = 1 << (pmf_reserved_nof_bits);
    }
    else
    {
        max_counter_value = 1 << (pmf_reserved_nof_bits + NATIVE_INDEXED_MODE_NOF_BITS);
    }
    for (hw_write_counter = 0; hw_write_counter < max_counter_value; hw_write_counter++)
    {
        if (intf_type == ETH_RIF)
        {
            in_lif_profile_hw = CALCULATE_VSI_PROFILE_FROM_ETH_RIF_PROFILE(VSI_MSB_PREFIX,
                                                                           (in_lif_profile |
                                                                            (hw_write_counter <<
                                                                             (NOF_ETH_RIF_PROFILE_BITS -
                                                                              (pmf_reserved_nof_bits)))));
        }
        else
        {
            in_lif_profile_hw = in_lif_profile | (hw_write_counter <<
                                                  (NOF_IN_LIF_PROFILE_BITS - (pmf_reserved_nof_bits)));
        }
        dbal_entry_key_field32_set(unit, entry_handle_id, DBAL_FIELD_VSI_PROFILE, in_lif_profile_hw);
        SHR_IF_ERR_EXIT(dbal_entry_clear(unit, entry_handle_id, DBAL_COMMIT));
    }

exit:
    DBAL_FUNC_FREE_VARS;
    SHR_FUNC_EXIT;
}
/*
 * Function used to set default values to IN_LIF_PROFILE_TABLE
 */
static shr_error_e
dnx_in_lif_profile_table_init(
    int unit,
    int in_lif_profile,
    int is_dealloc,
    int pmf_reserved_nof_bits,
    int egress_reserved_nof_bits,
    int native_indexed_mode_bits,
    int intf_type)
{
    int max_counter_value, hw_write_counter;
    uint32 entry_handle_id, cs_vtt_to_flp_in_lif_profile = DNX_IN_LIF_PROFILE_CS_DEFAULT,
        cs_flp_in_lif_profile = DNX_IN_LIF_FWD_PROFILE_CS_DEFAULT;
    SHR_FUNC_INIT_VARS(unit);
    DBAL_FUNC_INIT_VARS(unit);
    /*
     * IN_LIF_PROFILE_TABLE init
     */
    SHR_IF_ERR_EXIT(DBAL_HANDLE_ALLOC(unit, DBAL_TABLE_IN_LIF_PROFILE_TABLE, &entry_handle_id));

    if (dnx_data_lif.in_lif.feature_get(unit, dnx_data_lif_in_lif_cs_in_lif_profile_based_on_stages))
    {
        dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_CS_IN_LIF_VTT_TO_FLP_PROFILE,
                                     INST_SINGLE, cs_vtt_to_flp_in_lif_profile);
    }
    dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_CS_IN_LIF_VTT_TO_VTT_PROFILE,
                                 INST_SINGLE, DNX_IN_LIF_PROFILE_CS_DEFAULT);
    dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_CS_IN_LIF_FLP_TO_FLP_PROFILE,
                                 INST_SINGLE, cs_flp_in_lif_profile);
    dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_RPF_MODE, INST_SINGLE,
                                 DBAL_ENUM_FVAL_RPF_MODE_DISABLE);
    dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_OAM_DEFAULT_PROFILE, INST_SINGLE,
                                 DEFAULT_OAM_PROFILE);
    if (dnx_data_switch.load_balancing.hashing_selection_per_layer_get(unit))
    {
        dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_IN_LIF_LB_PROFILE, INST_SINGLE,
                                     DEFAULT_IN_LIF_LB_PROFILE);
    }
    /*
     * trap profiling fields
     */
    dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_L_2_CP_TRAP_HANDLING_PROFILE, INST_SINGLE,
                                 DEFAULT_TRAP_PROFILES_DATA);
    dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_ICMPV4_TRAP_HANDLING_PROFILE, INST_SINGLE,
                                 DEFAULT_TRAP_PROFILES_DATA);
    dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_ICMPV6_TRAP_HANDLING_PROFILE, INST_SINGLE,
                                 DEFAULT_TRAP_PROFILES_DATA);
    dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_ARP_TRAP_HANDLING_PROFILE, INST_SINGLE,
                                 DEFAULT_TRAP_PROFILES_DATA);
    dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_IGMP_TRAP_HANDLING_PROFILE, INST_SINGLE,
                                 DEFAULT_TRAP_PROFILES_DATA);
    dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_DHCP_TRAP_HANDLING_PROFILE, INST_SINGLE,
                                 DEFAULT_TRAP_PROFILES_DATA);
    dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_NON_AUTH_8021X_TRAP_HANDLING_PROFILE, INST_SINGLE,
                                 DEFAULT_TRAP_PROFILES_DATA);

    if (is_dealloc)
    {
        if (intf_type == ETH_RIF)
        {
            max_counter_value = 1 << (pmf_reserved_nof_bits + egress_reserved_nof_bits);
        }
        else
        {
            max_counter_value = 1 << (pmf_reserved_nof_bits + egress_reserved_nof_bits + native_indexed_mode_bits);
        }

        /*
         * loop over the possible values of the reserved bits, and duplicate HW value to all the possible keys
         * the key is built by shifting the reserved bits value left and append in_lif_profile
         */
        for (hw_write_counter = 0; hw_write_counter < max_counter_value; hw_write_counter++)
        {
            if (intf_type == ETH_RIF)
            {
                dbal_entry_key_field32_set(unit, entry_handle_id, DBAL_FIELD_IN_LIF_PROFILE,
                                           CALCULATE_IN_LIF_PROFILE_FROM_ETH_RIF_PROFILE
                                           (IN_LIF_PROFILE_TABLE_MSB_PREFIX,
                                            (in_lif_profile |
                                             (hw_write_counter <<
                                              (NOF_ETH_RIF_PROFILE_BITS -
                                               (pmf_reserved_nof_bits + egress_reserved_nof_bits))))));
                SHR_IF_ERR_EXIT(dbal_entry_commit(unit, entry_handle_id, DBAL_COMMIT));
            }
            else
            {
                dbal_entry_key_field32_set(unit, entry_handle_id, DBAL_FIELD_IN_LIF_PROFILE,
                                           in_lif_profile | (hw_write_counter <<
                                                             (NOF_IN_LIF_PROFILE_BITS -
                                                              (pmf_reserved_nof_bits + egress_reserved_nof_bits +
                                                               native_indexed_mode_bits))));
                SHR_IF_ERR_EXIT(dbal_entry_commit(unit, entry_handle_id, DBAL_COMMIT));

                if (!dnx_data_lif.in_lif.feature_get(unit, dnx_data_lif_in_lif_mapped_in_lif_profile))
                {
                    dbal_entry_key_field32_set(unit, entry_handle_id, DBAL_FIELD_IN_LIF_PROFILE, in_lif_profile);
                    SHR_IF_ERR_EXIT(dbal_entry_clear(unit, entry_handle_id, DBAL_COMMIT));
                }
            }
        }
    }
    else
    {
        if (!dnx_data_lif.feature.feature_get(unit, dnx_data_lif_feature_iop_mode_orientation_selection) &&
            dnx_data_headers.system_headers.system_headers_mode_get(unit) ==
            DBAL_ENUM_FVAL_SYSTEM_HEADERS_MODE_JERICHO_MODE)
        {
            dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_JR_IN_LIF_ORIENTATION, INST_SINGLE,
                                         DEFAULT_IN_LIF_ORIENTATION);
        }
        /*
         * in_lif_orientation
         */
        dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_IN_LIF_ORIENTATION, INST_SINGLE,
                                     DEFAULT_IN_LIF_ORIENTATION);
        dbal_entry_value_field8_set(unit, entry_handle_id, DBAL_FIELD_IN_LIF_SAME_INTERFACE, INST_SINGLE,
                                    DBAL_ENUM_FVAL_IN_LIF_SAME_INTERFACE_DEVICE_SCOPE);
        /*
         * ive outer/inner dp profile
         */
        dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_INNER_DP_PROFILE, INST_SINGLE, 0);
        dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_OUTER_DP_PROFILE, INST_SINGLE, 0);
        if (dnx_data_lif.in_lif.feature_get(unit, dnx_data_lif_in_lif_in_lif_profile_iop_mode))
        {
            dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_JR_MODE_IN_LIF_PROFILE, INST_SINGLE,
                                         DNX_IN_LIF_PROFILE_DEFAULT);
        }

        /*
         * this table has a key of 8 bits, and can be accessed with two kinds of profiles:
         * 1. in_lif_profile : 8 bits, in this case the key = all these 8 bits
         * 2. eth_rif_profile: 7 bits, in this case the MSB bit is set to a static prefix "1" by PEMLA
         * so we need to initiate the two entries 00000000 and 10000000
         */
        dbal_entry_key_field32_set(unit, entry_handle_id, DBAL_FIELD_IN_LIF_PROFILE, DNX_IN_LIF_PROFILE_DEFAULT);
        SHR_IF_ERR_EXIT(dbal_entry_commit(unit, entry_handle_id, DBAL_COMMIT));
        dbal_entry_key_field32_set(unit, entry_handle_id, DBAL_FIELD_IN_LIF_PROFILE,
                                   CALCULATE_IN_LIF_PROFILE_FROM_ETH_RIF_PROFILE
                                   (IN_LIF_PROFILE_MSB_BITS_MASK_FOR_ETH_RIF_PROFILE, DNX_IN_LIF_PROFILE_DEFAULT));
        SHR_IF_ERR_EXIT(dbal_entry_commit(unit, entry_handle_id, DBAL_COMMIT));

        if ((dnx_data_headers.system_headers.system_headers_mode_get(unit) ==
             DBAL_ENUM_FVAL_SYSTEM_HEADERS_MODE_JERICHO_MODE)
            && dnx_data_lif.feature.feature_get(unit, dnx_data_lif_feature_iop_mode_orientation_selection))
        {
            SHR_IF_ERR_EXIT(DBAL_HANDLE_CLEAR(unit, DBAL_TABLE_JR_MODE_INLIF_PROFILE_ORIENTATION_MAP, entry_handle_id));
            dbal_entry_key_field32_set(unit, entry_handle_id, DBAL_FIELD_JR_MODE_IN_LIF_PROFILE,
                                       DNX_IN_LIF_PROFILE_DEFAULT);
            dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_JR_IN_LIF_ORIENTATION, INST_SINGLE,
                                         DEFAULT_IN_LIF_ORIENTATION);
            SHR_IF_ERR_EXIT(dbal_entry_commit(unit, entry_handle_id, DBAL_COMMIT));
        }
    }

exit:
    DBAL_FUNC_FREE_VARS;
    SHR_FUNC_EXIT;
}

static shr_error_e
dnx_in_lif_profile_table_for_mapped_in_lif_profile_dealloc(
    int unit,
    int in_lif_profile,
    int pmf_reserved_nof_bits,
    int native_indexed_mode_bits,
    int intf_type)
{
    int max_counter_value, hw_write_counter;
    uint32 entry_handle_id;
    SHR_FUNC_INIT_VARS(unit);
    DBAL_FUNC_INIT_VARS(unit);
    /*
     * IN_LIF_PROFILE_TABLE init
     */
    SHR_IF_ERR_EXIT(DBAL_HANDLE_ALLOC(unit, DBAL_TABLE_IN_LIF_PROFILE_TABLE, &entry_handle_id));
    if (intf_type == ETH_RIF)
    {
        max_counter_value = 1 << (pmf_reserved_nof_bits);
    }
    else
    {
        max_counter_value = 1 << (pmf_reserved_nof_bits + native_indexed_mode_bits);
    }

    /*
     * loop over the possible values of the reserved bits, and duplicate HW value to all the possible keys
     * the key is built by shifting the reserved bits value left and append in_lif_profile
     */
    for (hw_write_counter = 0; hw_write_counter < max_counter_value; hw_write_counter++)
    {
        if (intf_type == ETH_RIF)
        {
            dbal_entry_key_field32_set(unit, entry_handle_id, DBAL_FIELD_IN_LIF_PROFILE,
                                       CALCULATE_IN_LIF_PROFILE_FROM_ETH_RIF_PROFILE
                                       (IN_LIF_PROFILE_TABLE_MSB_PREFIX,
                                        (in_lif_profile |
                                         (hw_write_counter << (NOF_ETH_RIF_PROFILE_BITS - (pmf_reserved_nof_bits))))));
            SHR_IF_ERR_EXIT(dbal_entry_clear(unit, entry_handle_id, DBAL_COMMIT));
        }
        else
        {
            dbal_entry_key_field32_set(unit, entry_handle_id, DBAL_FIELD_IN_LIF_PROFILE,
                                       in_lif_profile | (hw_write_counter <<
                                                         (NOF_IN_LIF_PROFILE_BITS -
                                                          (pmf_reserved_nof_bits + native_indexed_mode_bits))));
            SHR_IF_ERR_EXIT(dbal_entry_clear(unit, entry_handle_id, DBAL_COMMIT));
        }
    }

exit:
    DBAL_FUNC_FREE_VARS;
    SHR_FUNC_EXIT;
}
/**
 * \see
 *   in_lif_profile.h file
 */
shr_error_e
dnx_in_lif_profile_module_init(
    int unit)
{
    SHR_FUNC_INIT_VARS(unit);

    /**Default configuration of the LIF Profile table is needed only for devices, which do not support the mapped in lif profile or in IOP mode*/
    if (dnx_data_lif.in_lif.feature_get(unit, dnx_data_lif_in_lif_use_mapped_in_lif_profile))
    {
        SHR_IF_ERR_EXIT(dnx_in_lif_profile_set_mapped_in_lif_profile(unit));
    }
    else
    {
        SHR_IF_ERR_EXIT(dnx_in_lif_profile_table_init(unit, DNX_IN_LIF_PROFILE_DEFAULT, FALSE, 0, 0, 0, 0));
        SHR_IF_ERR_EXIT(dnx_vsi_profile_table_init(unit, DNX_IN_LIF_PROFILE_DEFAULT, FALSE, 0, 0, 0));
        SHR_IF_ERR_EXIT(dnx_in_lif_profile_set_egress_hw(unit));
    }
    SHR_IF_ERR_EXIT(dnx_enablers_vectors_table_init(unit, DEFAULT_ROUTING_ENABLERS_PROFILE, FALSE));
    if (dnx_data_headers.system_headers.system_headers_mode_get(unit) ==
        DBAL_ENUM_FVAL_SYSTEM_HEADERS_MODE_JERICHO_MODE)
    {
        SHR_IF_ERR_EXIT(dnx_eth_rif_profile_set_egress_hw(unit));
    }

exit:
    SHR_FUNC_EXIT;
}

/**
 * \brief -
 *  Get in-lif-profile template data according to interface type.
 *
 * \param [in] unit - The unit ID
 * \param [in] in_lif_profile - in_lif_profile id to get template data for
 * \param [in] intf_type - can be one of the following:
 *                         ETH_RIF - interface type is ETH_RIF
 *                         LIF - interface type is LIF
 * \param [out] in_lif_profile_template_data - in-lif-profile template data.
 *
 * \return
 *   shr_error_e
 *
 * \remark
 *   The template data may be got from DNX_ALGO_ETH_RIF_PROFILE,
 *   DNX_ALGO_ETH_RIF_PROFILE or DNX_ALGO_IN_LIF_PROFILE according
 *   to the in_lif_profile id and intf_type.
 *
 * \see
 *   dnx_in_lif_profile_exchange
 *   dnx_algo_in_lif_profile_template_create
 *   dnx_algo_in_lif_profile_and_eth_rif_shared_template_create
 */
static shr_error_e
dnx_in_lif_profile_template_data_get(
    int unit,
    int in_lif_profile,
    in_lif_intf_type_e intf_type,
    in_lif_template_data_t * in_lif_profile_template_data)
{
    int ref_count, eth_rif_profile_used_bits_mask, eth_rif_profile_egress_reserved_nof_bits,
        in_lif_profile_egress_reserved_nof_bits, eth_rif_profile_egress_reserved_start_bit,
        in_lif_profile_egress_reserved_start_bit, in_lif_profile_used_bits_mask;

    SHR_FUNC_INIT_VARS(unit);
    DBAL_FUNC_INIT_VARS(unit);

    SHR_NULL_CHECK(in_lif_profile_template_data, _SHR_E_PARAM, "NULL input - in_lif_profile_template_data");

    sal_memset(in_lif_profile_template_data, 0, sizeof(in_lif_template_data_t));

    if (intf_type == ETH_RIF)
    {
        SHR_IF_ERR_EXIT(dnx_eth_rif_profile_used_bits_mask_get
                        (unit, &eth_rif_profile_used_bits_mask, &eth_rif_profile_egress_reserved_nof_bits,
                         &eth_rif_profile_egress_reserved_start_bit));
        /*
         * the template manager includes only the ingress attributes, so only the relevant part of the in_lif_profile is used as key for the template manager
         */
        in_lif_profile = in_lif_profile & eth_rif_profile_used_bits_mask;
        /*
         * check if in_lif_profile is in range
         */
        if ((in_lif_profile < 0) | (in_lif_profile >= dnx_data_lif.in_lif.nof_in_lif_profiles_get(unit)))
        {
            SHR_ERR_EXIT(_SHR_E_PARAM, "in_lif_profile out of range : %d", in_lif_profile);
        }
        SHR_IF_ERR_EXIT(algo_lif_profile_db.eth_rif_profile.profile_data_get
                        (unit, in_lif_profile, &ref_count, in_lif_profile_template_data));
    }
    else if (intf_type == LIF)
    {
        SHR_IF_ERR_EXIT(dnx_in_lif_profile_used_bits_mask_get
                        (unit, &in_lif_profile_used_bits_mask, &in_lif_profile_egress_reserved_nof_bits,
                         &in_lif_profile_egress_reserved_start_bit));
        /*
         * the template manager includes only the ingress attributes, so only the relevant part of the in_lif_profile is used as key for the template manager
         */
        in_lif_profile = in_lif_profile & in_lif_profile_used_bits_mask;
        /*
         * check if in_lif_profile is in range
         */
        if ((in_lif_profile < 0) | (in_lif_profile >= dnx_data_lif.in_lif.nof_in_lif_profiles_get(unit)))
        {
            SHR_ERR_EXIT(_SHR_E_PARAM, "in_lif_profile out of range : %d", in_lif_profile);
        }
        SHR_IF_ERR_EXIT(algo_lif_profile_db.in_lif_profile.profile_data_get
                        (unit, in_lif_profile, &ref_count, in_lif_profile_template_data));
    }
    else
    {
        SHR_ERR_EXIT(_SHR_E_FAIL, "invalid intf_type\n");
    }

exit:
    DBAL_FUNC_FREE_VARS;
    SHR_FUNC_EXIT;
}

/**
 * \brief
 *   Helper function for dnx_in_lif_profile_routing_enablers_vector_convert.
 *   disables the proper bit in routing_enablers_vector if the corresponding flag BCM_L3_INGRESS_ROUTE_DISABLE_XXX is given by the user.
 * \par DIRECT INPUT
 *   \param [in] unit -
 *     The unit number.
 *   \param [in] routing_enablers_vector -
 *     Pointer to routing_enablers_vector \n
 *     \b As \b output - \n
 *       the Enablers Vector to modify: 2x32 bits array
 *   \param [in] in_lif_profile_info -
 *     Pointer to in_lif_profile_info \n
 *     \b As \b input - \n
 *       includes the lif profile properties, only flags is used
 *   \param [in] flag -
 *     The flag which determines the enabler to disable.
 *   \param [in] enabler_offset -
 *     The enabler's offset in the enablers vector.
 *   \param [in] layer_type -
 *     The layer identification of type of dbal_enum_value_field_layer_types_e
 *   \param [in] action -
 *     The action to do, disable/enable the protocol in the vector.
 * \par INDIRECT INPUT
 *   * \b *in_lif_profile_info \n
 *     See 'in_lif_profile_info' in DIRECT INPUT above
 * \par DIRECT OUTPUT
 *   \retval Zero if no error was detected
 *   \retval Negative if error was detected. See \ref shr_error_e
 * \par INDIRECT OUTPUT
 *   * \b *routing_enablers_vector \n
 *     See 'routing_enablers_vector' in DIRECT INPUT above
 * \see
 *   * None
 */
static shr_error_e
dnx_in_lif_profile_routing_enablers_vector_assign(
    int unit,
    uint32 *routing_enablers_vector,
    in_lif_profile_info_t * in_lif_profile_info,
    int flag,
    int enabler_offset,
    uint32 layer_type,
    in_lif_routing_vector_action_e action)
{
    SHR_FUNC_INIT_VARS(unit);

    /*
     * check if the flag is set by the user
     */
    if (in_lif_profile_info->ingress_fields.l3_ingress_flags & flag)
    {
        if (dnx_data_l3.routing_enablers.routing_enable_action_support_get(unit))
        {
            /** Set the two bits representing the profile to 0. */
            SHR_IF_ERR_EXIT(utilex_bitstream_reset_bit_range
                            (routing_enablers_vector, enabler_offset, enabler_offset + 1));
            if (action == ROUTING_VECTOR_ACTION_DISABLE)
            {
                /** Set the routing enable action profile for disabling layer termination */
                SHR_IF_ERR_EXIT(utilex_bitstream_set_field(routing_enablers_vector, enabler_offset,
                                                           dnx_data_l3.
                                                           routing_enablers.routing_enable_action_profile_size_get
                                                           (unit), dnx_data_l3.routing_enablers.layer_enablers_get(unit,
                                                                                                                   layer_type)->disable_profile));
            }
            else
            {
                SHR_IF_ERR_EXIT(utilex_bitstream_set_field(routing_enablers_vector, enabler_offset,
                                                           dnx_data_l3.
                                                           routing_enablers.routing_enable_action_profile_size_get
                                                           (unit),
                                                           dnx_data_l3.
                                                           routing_enablers.routing_enabled_action_profile_id_get
                                                           (unit)));
            }
        }
        else
        {
            if (action == ROUTING_VECTOR_ACTION_DISABLE)
            {
                SHR_IF_ERR_EXIT(utilex_bitstream_reset_bit(routing_enablers_vector, enabler_offset));
            }
            else
            {
                SHR_IF_ERR_EXIT(utilex_bitstream_set_bit(routing_enablers_vector, enabler_offset));
            }
        }
    }
exit:
    SHR_FUNC_EXIT;
}

static shr_error_e
dnx_cs_lif_profile_sw_state_flags_get(
    int unit,
    in_lif_profile_info_t * in_lif_profile_info)
{
    uint32 entry_handle_id;
    dbal_enum_value_field_cs_lif_profile_flags_e cs_lif_profile_flags_uint32;
    SHR_FUNC_INIT_VARS(unit);
    DBAL_FUNC_INIT_VARS(unit);
    SHR_IF_ERR_EXIT(DBAL_HANDLE_ALLOC(unit, DBAL_TABLE_CS_LIF_PROFILE_FLAGS_PER_GPORT_SW, &entry_handle_id));
    dbal_entry_key_field32_set(unit, entry_handle_id, DBAL_FIELD_GPORT, in_lif_profile_info->ingress_fields.lif_gport);
    SHR_IF_ERR_EXIT(dbal_entry_get(unit, entry_handle_id, DBAL_GET_ALL_FIELDS));
    SHR_IF_ERR_EXIT(dbal_entry_handle_value_field32_get
                    (unit, entry_handle_id, DBAL_FIELD_CS_LIF_PROFILE_FLAGS, INST_SINGLE,
                     &cs_lif_profile_flags_uint32));
    if (cs_lif_profile_flags_uint32 == DBAL_ENUM_FVAL_CS_LIF_PROFILE_FLAGS_MPLS_INTERFACE)
    {
        in_lif_profile_info->ingress_fields.l3_ingress_flags |= BCM_L3_INGRESS_MPLS_INTF_NAMESPACE;
    }
    else if (cs_lif_profile_flags_uint32 == DBAL_ENUM_FVAL_CS_LIF_PROFILE_FLAGS_VXLAN_ESP)
    {
        in_lif_profile_info->ingress_fields.cs_in_lif_profile_flags |= DNX_IN_LIF_PROFILE_CS_PROFILE_LIF_VXLAN_OVER_ESP;
    }
exit:
    DBAL_FUNC_FREE_VARS;
    SHR_FUNC_EXIT;
}

/**
 * \brief
 *   Helper for dnx_in_lif_profile_get_data.
 *   translates in_lif_profile_template_data to l3_ingress_flags
 * \param [in] unit -
 *   The unit number.
 * \param [in] in_lif_profile_template_data -
 *   The in_lif_profile Template Manager data
 * \param [in] in_lif_profile_info -
 *   The InLIF profile flags
 * \param [in] inlif_dbal_table_id -
 *   used to retrieve information about dbal table of the LIF
 * \return
 *   \retval Zero if no error was detected
 *   \retval Negative if error was detected. See \ref shr_error_e
 * \see
 *   * None
 */
static shr_error_e
dnx_in_lif_profile_translate_to_l3_ingress_flags(
    int unit,
    in_lif_template_data_t in_lif_profile_template_data,
    in_lif_profile_info_t * in_lif_profile_info,
    dbal_tables_e inlif_dbal_table_id)
{
    uint32 cs_vtt_in_lif_profile;
    SHR_FUNC_INIT_VARS(unit);

    SHR_NULL_CHECK(in_lif_profile_info, _SHR_E_PARAM, "NULL input - in_lif_profile_info");
    cs_vtt_in_lif_profile = in_lif_profile_template_data.cs_vtt_in_lif_profile;
    if (inlif_dbal_table_id == DBAL_TABLE_IN_LIF_FORMAT_PWE)
    {
        dbal_enum_value_field_nof_sd_tags_e pwe_nof_tags = 0;
        dbal_enum_value_field_lif_vtt4_csp_1bit_pwe_tag_namespace_e pwe_namespace = 0;

        SHR_IF_ERR_EXIT(dbal_fields_struct_field_decode(unit, DBAL_FIELD_PWE_INLIF_CS_PROFILE, DBAL_FIELD_NOF_SD_TAGS,
                                                        &pwe_nof_tags, &cs_vtt_in_lif_profile));
        SHR_IF_ERR_EXIT(dbal_fields_struct_field_decode(unit, DBAL_FIELD_PWE_INLIF_CS_PROFILE, DBAL_FIELD_TAG_NAMESPACE,
                                                        &pwe_namespace, &cs_vtt_in_lif_profile));
        /*
         * LSB indicates LIF/NWK scope and bits 2 and 3 LSB indicate the nof_tags
         */
        in_lif_profile_info->ingress_fields.pwe_nof_eth_tags = pwe_nof_tags;
        if (pwe_namespace == DBAL_ENUM_FVAL_LIF_VTT4_CSP_1BIT_PWE_TAG_NAMESPACE_LIF_SCOPED)
        {
            in_lif_profile_info->ingress_fields.cs_in_lif_profile_flags = DNX_IN_LIF_PROFILE_CS_PROFILE_PWE_LIF_SCOPED;
        }
    }
    if (inlif_dbal_table_id == DBAL_TABLE_IN_LIF_FORMAT_EVPN)
    {
        dbal_enum_value_field_lif_vttx_cs_evpn_iml_ah_e evpn_iml_ah = 0;
        dbal_enum_value_field_lif_vtt4_csp_1bit_pwe_tag_namespace_e pwe_namespace = 0;

        SHR_IF_ERR_EXIT(dbal_fields_struct_field_decode
                        (unit, DBAL_FIELD_EVPN_IML_INLIF_CS_PROFILE, DBAL_FIELD_IML_ADDITIONAL_HEADERS, &evpn_iml_ah,
                         &cs_vtt_in_lif_profile));
        SHR_IF_ERR_EXIT(dbal_fields_struct_field_decode
                        (unit, DBAL_FIELD_EVPN_IML_INLIF_CS_PROFILE, DBAL_FIELD_TAG_NAMESPACE, &pwe_namespace,
                         &cs_vtt_in_lif_profile));

        if (pwe_namespace == DBAL_ENUM_FVAL_LIF_VTT4_CSP_1BIT_PWE_TAG_NAMESPACE_LIF_SCOPED)
        {
            in_lif_profile_info->ingress_fields.cs_in_lif_profile_flags =
                DNX_IN_LIF_PROFILE_CS_PROFILE_EVPN_IML_LIF_SCOPED;
        }
        if (evpn_iml_ah == DBAL_ENUM_FVAL_LIF_VTTX_CS_EVPN_IML_AH_NO_FL)
        {
            in_lif_profile_info->ingress_fields.cs_in_lif_profile_flags |=
                DNX_IN_LIF_PROFILE_CS_PROFILE_EVPN_IML_AH_NO_FL;
        }
        if (evpn_iml_ah == DBAL_ENUM_FVAL_LIF_VTTX_CS_EVPN_IML_AH_FL_ONLY)
        {
            in_lif_profile_info->ingress_fields.cs_in_lif_profile_flags |=
                DNX_IN_LIF_PROFILE_CS_PROFILE_EVPN_IML_AH_FL_ONLY;
        }
        if (evpn_iml_ah == DBAL_ENUM_FVAL_LIF_VTTX_CS_EVPN_IML_AH_FL_AND_CW)
        {
            in_lif_profile_info->ingress_fields.cs_in_lif_profile_flags |=
                DNX_IN_LIF_PROFILE_CS_PROFILE_EVPN_IML_AH_FL_CW;
        }
    }
    if ((inlif_dbal_table_id == DBAL_TABLE_IN_LIF_IPvX_TUNNELS) ||
        (inlif_dbal_table_id == DBAL_TABLE_IN_LIF_FORMAT_LSP)
        || (inlif_dbal_table_id == dnx_data_l2.vsi.vsi_table_get(unit)))
    {
        dbal_enum_value_field_lif_vtt5_csp_1bit_l3lif_uc_db_mode_e uc_mode = 0;
        dbal_enum_value_field_lif_vtt5_csp_1bit_nat_direction_and_virtual_ip_and_srv6_gsid_e
            nat_or_virtual_ip_or_srv6_gsid = 0;
        dbal_enum_value_field_lif_xxxx_csp_1bit_l3lif_mpls_namespace_and_algo_acl_or_esp_vxlan_e
            mpls_namespace_or_aacl_or_esp_vxlan = 0;

        SHR_IF_ERR_EXIT(dbal_fields_struct_field_decode
                        (unit, DBAL_FIELD_ROUTING_VTT2FWD1_INLIF_CS_PROFILE,
                         DBAL_FIELD_NAT_DIRECTION_OR_VIP_MODE_OR_SRV6_GSID, &nat_or_virtual_ip_or_srv6_gsid,
                         &cs_vtt_in_lif_profile));
        SHR_IF_ERR_EXIT(dbal_fields_struct_field_decode
                        (unit, DBAL_FIELD_ROUTING_VTT2FWD1_INLIF_CS_PROFILE,
                         DBAL_FIELD_MPLS_NAMESPACE_OR_AACL_OR_ESP_VXLAN, &mpls_namespace_or_aacl_or_esp_vxlan,
                         &cs_vtt_in_lif_profile));
        SHR_IF_ERR_EXIT(dbal_fields_struct_field_decode
                        (unit, DBAL_FIELD_ROUTING_VTT2FWD1_INLIF_CS_PROFILE, DBAL_FIELD_UC_DB_MODE, &uc_mode,
                         &cs_vtt_in_lif_profile));
        if ((nat_or_virtual_ip_or_srv6_gsid ==
             DBAL_ENUM_FVAL_LIF_VTT5_CSP_1BIT_NAT_DIRECTION_AND_VIRTUAL_IP_AND_SRV6_GSID_NAT_DOWNSTREAM_VIP_ENABLED_SRV6_GSID_EOC)
            && (inlif_dbal_table_id == dnx_data_l2.vsi.vsi_table_get(unit)))
        {
            in_lif_profile_info->ingress_fields.l3_ingress_flags |= BCM_L3_INGRESS_VIP_ENABLED;
        }
        else if ((nat_or_virtual_ip_or_srv6_gsid ==
                  DBAL_ENUM_FVAL_LIF_VTT5_CSP_1BIT_NAT_DIRECTION_AND_VIRTUAL_IP_AND_SRV6_GSID_NAT_DOWNSTREAM_VIP_ENABLED_SRV6_GSID_EOC)
                 && (inlif_dbal_table_id == DBAL_TABLE_IN_LIF_IPvX_TUNNELS))
        {
            in_lif_profile_info->ingress_fields.cs_in_lif_profile_flags |=
                DNX_IN_LIF_PROFILE_CS_PROFILE_LIF_WITH_SRV6_GSID;
        }
        if (uc_mode == DBAL_ENUM_FVAL_LIF_VTT5_CSP_1BIT_L3LIF_UC_DB_MODE_UC_PRIVATE_PUBLIC_OPTIMIZED)
        {
            in_lif_profile_info->ingress_fields.l3_ingress_flags |= BCM_L3_INGRESS_GLOBAL_ROUTE;
        }

        if (mpls_namespace_or_aacl_or_esp_vxlan ==
            DBAL_ENUM_FVAL_LIF_XXXX_CSP_1BIT_L3LIF_MPLS_NAMESPACE_AND_ALGO_ACL_OR_ESP_VXLAN_MPLS_PER_INTERFACE_AACL_OR_ESP_VXLAN_ENABLED)
        {
            SHR_IF_ERR_EXIT(dnx_cs_lif_profile_sw_state_flags_get(unit, in_lif_profile_info));
        }
    }

    if (inlif_dbal_table_id == dnx_data_l2.vsi.vsi_table_get(unit))
    {
        if (in_lif_profile_template_data.cs_flp_in_lif_profile &
            (DBAL_ENUM_FVAL_LIF_FWD1_CSP_1BIT_ETH_RIF_BRIDGE_FALLBACK_MODE_IP_COMPATIBLE_MC_BRIDGE_FALLBACK_ENABLE <<
             IN_LIF_PROFILE_IPMC_BRIDGE_FALLBACK_BIT_OFFSET))
        {
            in_lif_profile_info->ingress_fields.l3_ingress_flags |= BCM_L3_INGRESS_IPMC_BRIDGE_FALLBACK;
        }
        if (in_lif_profile_template_data.l2_v4_mc_fwd_type == DBAL_ENUM_FVAL_L2_V4_MC_FWD_TYPE_IPV4MC)
        {
            in_lif_profile_info->ingress_fields.l3_ingress_flags |= BCM_L3_INGRESS_L3_MCAST_L2;
        }
        if (in_lif_profile_template_data.l2_v6_mc_fwd_type == DBAL_ENUM_FVAL_L2_V6_MC_FWD_TYPE_IPV6MC)
        {
            in_lif_profile_info->ingress_fields.l3_ingress_flags |= BCM_L3_INGRESS_IP6_L3_MCAST_L2;
        }
    }

exit:
    SHR_FUNC_EXIT;
}

/**
 * \brief
 *   Helper for dnx_in_lif_profile_algo_handle_routing_enablers_profile.
 *   Converts BCM_L3_INGRESS_ROUTE_DISABLE_XXX to in_lif_profile template data.
 * \par DIRECT INPUT
 *   \param [in] unit -
 *     The unit number.
 *   \param [in] routing_enablers_vector -
 *     Pointer to routing_enablers_vector \n
 *     \b As \b output - \n
 *       the Enablers Vector to modify: 2x32 bits array
 *   \param [in] in_lif_profile_info -
 *     Pointer to in_lif_profile_info \n
 *     \b As \b input - \n
 *       includes the lif profile properties, only flags is used
 * \par INDIRECT INPUT
 *   * \b *in_lif_profile_info \n
 *     See 'in_lif_profile_info' in DIRECT INPUT above
 * \par DIRECT OUTPUT
 *   \retval Zero if no error was detected
 *   \retval Negative if error was detected. See \ref shr_error_e
 * \par INDIRECT OUTPUT
 *   * \b *routing_enablers_vector \n
 *     See 'routing_enablers_vector' in DIRECT INPUT above
 * \see
 *   * None
 */
static shr_error_e
dnx_in_lif_profile_routing_enablers_vector_convert(
    int unit,
    uint32 *routing_enablers_vector,
    in_lif_profile_info_t * in_lif_profile_info)
{
    SHR_FUNC_INIT_VARS(unit);

    SHR_IF_ERR_EXIT(dnx_in_lif_profile_routing_enablers_vector_assign
                    (unit, routing_enablers_vector, in_lif_profile_info, BCM_L3_INGRESS_ROUTE_ENABLE_UNKNOWN,
                     DNX_IN_LIF_ROUTING_UC_ENABLER_OFFSET_GET(unit, DBAL_ENUM_FVAL_LAYER_TYPES_UNKNOWN),
                     DBAL_ENUM_FVAL_LAYER_TYPES_UNKNOWN, ROUTING_VECTOR_ACTION_ENABLE));

    SHR_IF_ERR_EXIT(dnx_in_lif_profile_routing_enablers_vector_assign
                    (unit, routing_enablers_vector, in_lif_profile_info, BCM_L3_INGRESS_ROUTE_ENABLE_UNKNOWN,
                     DNX_IN_LIF_ROUTING_MC_ENABLER_OFFSET_GET(unit, DBAL_ENUM_FVAL_LAYER_TYPES_UNKNOWN),
                     DBAL_ENUM_FVAL_LAYER_TYPES_UNKNOWN, ROUTING_VECTOR_ACTION_ENABLE));

    SHR_IF_ERR_EXIT(dnx_in_lif_profile_routing_enablers_vector_assign
                    (unit, routing_enablers_vector, in_lif_profile_info, BCM_L3_INGRESS_ROUTE_DISABLE_IP4_UCAST,
                     DNX_IN_LIF_ROUTING_UC_ENABLER_OFFSET_GET(unit, DBAL_ENUM_FVAL_LAYER_TYPES_IPV4),
                     DBAL_ENUM_FVAL_LAYER_TYPES_IPV4, ROUTING_VECTOR_ACTION_DISABLE));

    SHR_IF_ERR_EXIT(dnx_in_lif_profile_routing_enablers_vector_assign
                    (unit, routing_enablers_vector, in_lif_profile_info, BCM_L3_INGRESS_ROUTE_DISABLE_IP4_MCAST,
                     DNX_IN_LIF_ROUTING_MC_ENABLER_OFFSET_GET(unit, DBAL_ENUM_FVAL_LAYER_TYPES_IPV4),
                     DBAL_ENUM_FVAL_LAYER_TYPES_IPV4, ROUTING_VECTOR_ACTION_DISABLE));

    SHR_IF_ERR_EXIT(dnx_in_lif_profile_routing_enablers_vector_assign
                    (unit, routing_enablers_vector, in_lif_profile_info, BCM_L3_INGRESS_ROUTE_DISABLE_IP6_UCAST,
                     DNX_IN_LIF_ROUTING_UC_ENABLER_OFFSET_GET(unit, DBAL_ENUM_FVAL_LAYER_TYPES_IPV6),
                     DBAL_ENUM_FVAL_LAYER_TYPES_IPV6, ROUTING_VECTOR_ACTION_DISABLE));

    SHR_IF_ERR_EXIT(dnx_in_lif_profile_routing_enablers_vector_assign
                    (unit, routing_enablers_vector, in_lif_profile_info, BCM_L3_INGRESS_ROUTE_DISABLE_IP6_MCAST,
                     DNX_IN_LIF_ROUTING_MC_ENABLER_OFFSET_GET(unit, DBAL_ENUM_FVAL_LAYER_TYPES_IPV6),
                     DBAL_ENUM_FVAL_LAYER_TYPES_IPV6, ROUTING_VECTOR_ACTION_DISABLE));

    SHR_IF_ERR_EXIT(dnx_in_lif_profile_routing_enablers_vector_assign
                    (unit, routing_enablers_vector, in_lif_profile_info, BCM_L3_INGRESS_ROUTE_DISABLE_MPLS,
                     DNX_IN_LIF_ROUTING_UC_ENABLER_OFFSET_GET(unit, DBAL_ENUM_FVAL_LAYER_TYPES_MPLS),
                     DBAL_ENUM_FVAL_LAYER_TYPES_MPLS, ROUTING_VECTOR_ACTION_DISABLE));
    SHR_IF_ERR_EXIT(dnx_in_lif_profile_routing_enablers_vector_assign
                    (unit, routing_enablers_vector, in_lif_profile_info, BCM_L3_INGRESS_ROUTE_DISABLE_MPLS,
                     DNX_IN_LIF_ROUTING_MC_ENABLER_OFFSET_GET(unit, DBAL_ENUM_FVAL_LAYER_TYPES_MPLS),
                     DBAL_ENUM_FVAL_LAYER_TYPES_MPLS, ROUTING_VECTOR_ACTION_DISABLE));

    SHR_IF_ERR_EXIT(dnx_in_lif_profile_routing_enablers_vector_assign
                    (unit, routing_enablers_vector, in_lif_profile_info, BCM_L3_INGRESS_TUNNEL_DISABLE_NATIVE_ETH,
                     DNX_IN_LIF_ROUTING_UC_ENABLER_OFFSET_GET(unit, DBAL_ENUM_FVAL_LAYER_TYPES_ETHERNET),
                     DBAL_ENUM_FVAL_LAYER_TYPES_ETHERNET, ROUTING_VECTOR_ACTION_DISABLE));

    SHR_IF_ERR_EXIT(dnx_in_lif_profile_routing_enablers_vector_assign
                    (unit, routing_enablers_vector, in_lif_profile_info, BCM_L3_INGRESS_TUNNEL_DISABLE_NATIVE_ETH,
                     DNX_IN_LIF_ROUTING_MC_ENABLER_OFFSET_GET(unit, DBAL_ENUM_FVAL_LAYER_TYPES_ETHERNET),
                     DBAL_ENUM_FVAL_LAYER_TYPES_ETHERNET, ROUTING_VECTOR_ACTION_DISABLE));

    if (in_lif_profile_info->ingress_fields.allow_udp_tcp_above_termination)
    {
        int offsets_to_enable[3 * NOF_ROUTING_ENABLE_TYPES];
        int offset_index = 0;
        offsets_to_enable[offset_index++] =
            DNX_IN_LIF_ROUTING_UC_ENABLER_OFFSET_GET(unit, DBAL_ENUM_FVAL_LAYER_TYPES_UDP);
        offsets_to_enable[offset_index++] =
            DNX_IN_LIF_ROUTING_MC_ENABLER_OFFSET_GET(unit, DBAL_ENUM_FVAL_LAYER_TYPES_UDP);
        offsets_to_enable[offset_index++] =
            DNX_IN_LIF_ROUTING_UC_ENABLER_OFFSET_GET(unit, DBAL_ENUM_FVAL_LAYER_TYPES_TCP);
        offsets_to_enable[offset_index++] =
            DNX_IN_LIF_ROUTING_MC_ENABLER_OFFSET_GET(unit, DBAL_ENUM_FVAL_LAYER_TYPES_TCP);
        offsets_to_enable[offset_index++] =
            DNX_IN_LIF_ROUTING_UC_ENABLER_OFFSET_GET(unit, DBAL_ENUM_FVAL_LAYER_TYPES_ICMP);
        offsets_to_enable[offset_index++] =
            DNX_IN_LIF_ROUTING_MC_ENABLER_OFFSET_GET(unit, DBAL_ENUM_FVAL_LAYER_TYPES_ICMP);

        for (offset_index = 0; offset_index < 3 * NOF_ROUTING_ENABLE_TYPES; offset_index++)
        {
            if (dnx_data_l3.routing_enablers.routing_enable_action_support_get(unit))
            {
                SHR_IF_ERR_EXIT(utilex_bitstream_set_field(routing_enablers_vector, offsets_to_enable[offset_index],
                                                           dnx_data_l3.
                                                           routing_enablers.routing_enable_action_profile_size_get
                                                           (unit),
                                                           dnx_data_l3.
                                                           routing_enablers.routing_enabled_action_profile_id_get
                                                           (unit)));
            }
            else
            {
                SHR_IF_ERR_EXIT(utilex_bitstream_set_bit(routing_enablers_vector, offsets_to_enable[offset_index]));
            }
        }
    }
exit:
    SHR_FUNC_EXIT;
}

/**
 * \brief
 *   Given old_lif_profile_id and new routing enablers information,
 *   this function exchanges the routing enablers profile and configures
 *   the routing enablers profile in HW in case it is the first reference.
 * \par DIRECT INPUT
 *   \param [in] unit -
 *     The unit number.
 *   \param [in] old_in_lif_profile -
 *     old LIF profile which should be exchanged
 *   \param [in] routing_enablers_profile -
 *     Pointer to routing_enablers_profile \n
 *     \b As \b input - \n
 *       the new enablers vector according to user's flags
 *   \param [in] in_lif_profile_info -
 *     Pointer to in_lif_profile_info \n
 *     \b As \b input - \n
 *       includes the lif profile properties, only flags is used
 *   \param [in] is_first -
 *     Pointer to is_first \n
 *     \b As \b output - \n
 *       says if the new profile was added to the template
 *       for the first time
 *   \param [in] is_last -
 *     Pointer to is_last \n
 *     \b As \b output - \n
 *       says if the old profile has no more pointers in template manager
 * \param [in] intf_type -
 *   can be one of the following:
 *       ETH_RIF - interface type is ETH_RIF
 *       LIF - interface type is LIF
 * \par INDIRECT INPUT
 *   * \b *routing_enablers_profile \n
 *     See 'routing_enablers_profile' in DIRECT INPUT above
 *   * \b *in_lif_profile_info \n
 *     See 'in_lif_profile_info' in DIRECT INPUT above
 * \par DIRECT OUTPUT
 *   \retval Zero if no error was detected
 *   \retval Negative if error was detected. See \ref shr_error_e
 * \par INDIRECT OUTPUT
 *   * \b *is_first \n
 *     See 'is_first' in DIRECT INPUT above
 *   * \b *is_last \n
 *     See 'is_last' in DIRECT INPUT above
 * \remark
 *   we don't clean HW tables in case of last profile case, because of the
 *   issue that when we delete the profile till we move the enablers profile
 *   to the new place traffic will get some "undefine" state.
 * \see
 *   * None
 */
static shr_error_e
dnx_in_lif_profile_algo_handle_routing_enablers_profile(
    int unit,
    int old_in_lif_profile,
    int *routing_enablers_profile,
    in_lif_profile_info_t * in_lif_profile_info,
    uint8 *is_first,
    uint8 *is_last,
    in_lif_intf_type_e intf_type)
{
    uint32 routing_enablers_vector[DNX_DATA_MAX_L3_ROUTING_ENABLERS_ROUTING_ENABLE_VECTOR_LENGTH / SAL_UINT32_NOF_BITS],
        entry_handle_id, old_routing_enablers_profile;
    int ref_count, eth_rif_profile_used_bits_mask =
        0, egress_reserved_nof_bits, eth_rif_profile_egress_reserved_start_bit, in_lif_profile_used_bits_mask,
        in_lif_profile_egress_reserved_nof_bits, in_lif_profile_egress_reserved_start_bit;
    in_lif_template_data_t old_in_lif_profile_data;

    SHR_FUNC_INIT_VARS(unit);
    DBAL_FUNC_INIT_VARS(unit);

    /*
     * init local vars
     */
    ref_count = 0;
    old_routing_enablers_profile = DEFAULT_ROUTING_ENABLERS_PROFILE;
    sal_memset(&old_in_lif_profile_data, 0, sizeof(old_in_lif_profile_data));
    /*
     * init to default, later we will disable enablers according to input ingress flags
     */
    SHR_IF_ERR_EXIT(dnx_in_lif_profile_get_default_routing_enablers_vector(unit, routing_enablers_vector));
    if (intf_type == ETH_RIF)
    {
        SHR_IF_ERR_EXIT(dnx_eth_rif_profile_used_bits_mask_get
                        (unit, &eth_rif_profile_used_bits_mask, &egress_reserved_nof_bits,
                         &eth_rif_profile_egress_reserved_start_bit));
        old_in_lif_profile = old_in_lif_profile & eth_rif_profile_used_bits_mask;
    }
    else if (intf_type == LIF)
    {
        SHR_IF_ERR_EXIT(dnx_in_lif_profile_used_bits_mask_get
                        (unit, &in_lif_profile_used_bits_mask, &in_lif_profile_egress_reserved_nof_bits,
                         &in_lif_profile_egress_reserved_start_bit));
    }
    /*
     * verify input
     */
    if ((old_in_lif_profile < 0) || (old_in_lif_profile > dnx_data_lif.in_lif.nof_in_lif_profiles_get(unit) - 1))
    {
        SHR_ERR_EXIT(_SHR_E_FAIL, "old_in_lif_profile out of range: %d, must be between 0 and %d\n",
                     old_in_lif_profile, dnx_data_lif.in_lif.nof_in_lif_profiles_get(unit) - 1);
    }
    SHR_NULL_CHECK(in_lif_profile_info, _SHR_E_FAIL, "in_lif_profile_info");

    /*
     * Update the proper bits in enablers vector according to user's flags
     */
    SHR_IF_ERR_EXIT(dnx_in_lif_profile_routing_enablers_vector_convert
                    (unit, routing_enablers_vector, in_lif_profile_info));

    /*
     * get old enablers profile
     */
    if (intf_type == LIF)
    {
        /*
         * the template manager key is only the ingress part of the in_lif_profile
         */
        old_in_lif_profile = old_in_lif_profile & in_lif_profile_used_bits_mask;
        SHR_IF_ERR_EXIT(algo_lif_profile_db.in_lif_profile.profile_data_get
                        (unit, old_in_lif_profile, &ref_count, &old_in_lif_profile_data));
    }
    else if (intf_type == ETH_RIF)
    {
        /*
         * the template manager key is only the ingress part of the in_lif_profile
         */
        old_in_lif_profile = old_in_lif_profile & eth_rif_profile_used_bits_mask;
        SHR_IF_ERR_EXIT(algo_lif_profile_db.eth_rif_profile.profile_data_get
                        (unit, old_in_lif_profile, &ref_count, &old_in_lif_profile_data));
    }
    else
    {
        SHR_ERR_EXIT(_SHR_E_FAIL, "invalid intf_type\n");
    }
    old_routing_enablers_profile = old_in_lif_profile_data.routing_enablers_profile;

    SHR_IF_ERR_EXIT(algo_lif_profile_db.in_lif_profile_l3_enablers.exchange
                    (unit, 0, routing_enablers_vector, old_routing_enablers_profile,
                     NULL, routing_enablers_profile, is_first, is_last));

    /*
     * in case the new profile is used for the first time, need to write it to HW
     */
    if (*is_first)
    {
        SHR_IF_ERR_EXIT(DBAL_HANDLE_ALLOC(unit, DBAL_TABLE_ENABLERS_VECTORS, &entry_handle_id));
        dbal_entry_key_field32_set(unit, entry_handle_id,
                                   DBAL_FIELD_ROUTING_ENABLERS_PROFILE, *routing_enablers_profile);
        dbal_entry_value_field_arr32_set(unit, entry_handle_id,
                                         DBAL_FIELD_ENABLERS_VECTOR, INST_SINGLE, routing_enablers_vector);
        SHR_IF_ERR_EXIT(dbal_entry_commit(unit, entry_handle_id, DBAL_COMMIT));
    }

exit:
    DBAL_FUNC_FREE_VARS;
    SHR_FUNC_EXIT;
}

/**
 * \brief
 *   Set default values to the LB layer records.
 * \param [in] unit -
 *   The unit number.
 * \param [in] lif_lb_enablers_profile -
 *   In LIF LB enablers profile to clear from DBAL
 * \return
 *   \retval Zero if no error was detected
 *   \retval Negative if error was detected. See \ref shr_error_e
 * \remark
 *   None
 * \see
 *   None
 */
static shr_error_e
dnx_in_lif_profile_lif_lb_profile_init(
    int unit,
    int lif_lb_enablers_profile)
{
    uint32 entry_handle_id;
    SHR_FUNC_INIT_VARS(unit);
    DBAL_FUNC_INIT_VARS(unit);
    SHR_IF_ERR_EXIT(DBAL_HANDLE_ALLOC(unit, DBAL_TABLE_LOAD_BALANCING_USE_LAYER_RECORD, &entry_handle_id));
    /** Write default values to LOAD_BALANCING_USE_LAYER_RECORD table -  set all to enabled */
    dbal_entry_key_field32_range_set(unit, entry_handle_id, DBAL_FIELD_LAYER_PROTOCOL, DBAL_RANGE_ALL, DBAL_RANGE_ALL);
    dbal_entry_key_field32_range_set(unit, entry_handle_id, DBAL_FIELD_PORT_LB_PROFILE, DBAL_RANGE_ALL, DBAL_RANGE_ALL);
    dbal_entry_key_field32_set(unit, entry_handle_id, DBAL_FIELD_IN_LIF_LB_PROFILE, lif_lb_enablers_profile);
    dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_USE_LAYER_RECORD, INST_SINGLE, TRUE);
    SHR_IF_ERR_EXIT(dbal_entry_commit(unit, entry_handle_id, DBAL_COMMIT));

exit:
    DBAL_FUNC_FREE_VARS;
    SHR_FUNC_EXIT;
}

/**
 * \brief
 * The function handles exchange on inlif profile based on the input disable flags
 */
/**
 * \brief
 *   Given old_lif_profile_id and new LB enablers information,
 *   this function exchanges the LB enablers profile and configures
 *   the LB enablers profile in HW in case it is the first reference.
 * \param [in] unit -
 *     The unit number.
 * \param [in] old_in_lif_profile -
 *     old LIF profile which should be exchanged
 * \param [out] lif_lb_enablers_profile -
 *     Pointer to new LB enablers profile selected
 *       according to in_lif_profile_info.
 * \param [in] in_lif_profile_info -
 *     Pointer to in_lif_profile_info - includes the lif profile properties, only lb_per_lif_flags field is used
 * \param [out] is_first - indicates if the new profile was referenced for the first time in the template
 * \param [out] is_last - indicates if the old profile has no more references to it in template manager
 * \param [in] intf_type -
 *   can be one of the following:
 *       ETH_RIF - interface type is ETH_RIF
 *       LIF - interface type is LIF
 * \remark
 *   we don't clean HW tables in case of last profile case, because of the
 *   issue that when we delete the profile till we move the enablers profile
 *   to the new place traffic will get some "undefine" state.
 * \see
 *   * None
 */
static shr_error_e
dnx_in_lif_profile_algo_handle_lif_lb_profile(
    int unit,
    int old_in_lif_profile,
    int *lif_lb_enablers_profile,
    in_lif_profile_info_t * in_lif_profile_info,
    uint8 *is_first,
    uint8 *is_last,
    in_lif_intf_type_e intf_type)
{
    uint32 lif_lb_profile_disablers_flags_vector;
    int ref_count;
    in_lif_template_data_t old_in_lif_profile_data;

    int eth_rif_profile_used_bits_mask = 0, eth_rif_profile_egress_reserved_nof_bits = 0,
        in_lif_profile_egress_reserved_nof_bits = 0, eth_rif_profile_egress_reserved_start_bit = 0,
        in_lif_profile_egress_reserved_start_bit = 0, in_lif_profile_used_bits_mask = 0;

    SHR_FUNC_INIT_VARS(unit);

    /*
     * init local vars
     */
    ref_count = 0;

    sal_memset(&old_in_lif_profile_data, 0, sizeof(old_in_lif_profile_data));
    /*
     * init to default, later we will disable enablers according to input ingress flags
     */

    if (intf_type == ETH_RIF)
    {
        /*
         * the template manager key is only the ingress part of the in_lif_profile
         */
        SHR_IF_ERR_EXIT(dnx_eth_rif_profile_used_bits_mask_get
                        (unit, &eth_rif_profile_used_bits_mask, &eth_rif_profile_egress_reserved_nof_bits,
                         &eth_rif_profile_egress_reserved_start_bit));
        old_in_lif_profile = old_in_lif_profile & eth_rif_profile_used_bits_mask;
    }
    else
    {
        SHR_IF_ERR_EXIT(dnx_in_lif_profile_used_bits_mask_get
                        (unit, &in_lif_profile_used_bits_mask, &in_lif_profile_egress_reserved_nof_bits,
                         &in_lif_profile_egress_reserved_start_bit));
        old_in_lif_profile = old_in_lif_profile & in_lif_profile_used_bits_mask;
    }
    /*
     * verify input
     */
    if ((old_in_lif_profile < 0) || (old_in_lif_profile > dnx_data_lif.in_lif.nof_in_lif_profiles_get(unit) - 1))
    {
        SHR_ERR_EXIT(_SHR_E_FAIL, "old_in_lif_profile out of range: %d, must be between 0 and %d\n",
                     old_in_lif_profile, dnx_data_lif.in_lif.nof_in_lif_profiles_get(unit) - 1);
    }
    SHR_NULL_CHECK(in_lif_profile_info, _SHR_E_FAIL, "in_lif_profile_info");

    /*
     * disable the proper bits in enablers vector according to user's flags
     */
    lif_lb_profile_disablers_flags_vector = in_lif_profile_info->ingress_fields.lb_per_lif_flags;

    /*
     * get old enablers profile
     */
    if (intf_type == ETH_RIF)
    {
        SHR_IF_ERR_EXIT(algo_lif_profile_db.eth_rif_profile.profile_data_get
                        (unit, old_in_lif_profile, &ref_count, &old_in_lif_profile_data));
    }
    else
    {
        SHR_IF_ERR_EXIT(algo_lif_profile_db.in_lif_profile.profile_data_get
                        (unit, old_in_lif_profile, &ref_count, &old_in_lif_profile_data));

    }

    SHR_IF_ERR_EXIT(algo_lif_profile_db.in_lif_lb_profile_disablers_flags.exchange
                    (unit, 0, &lif_lb_profile_disablers_flags_vector, old_in_lif_profile_data.in_lif_lb_profile,
                     NULL, lif_lb_enablers_profile, is_first, is_last));

    if (*is_first)
    {
        /** Write lif lb profile and disabled protocols to LOAD_BALANCING_USE_LAYER_RECORD table */
        SHR_IF_ERR_EXIT(dnx_port_lif_lb_profile_dbal_configure
                        (unit, in_lif_profile_info->ingress_fields.lb_per_lif_flags, DBAL_FIELD_IN_LIF_LB_PROFILE,
                         *lif_lb_enablers_profile));
    }

exit:
    SHR_FUNC_EXIT;
}

static void
dnx_lif_profile_sw_state_lif_profile_flags_get(
    int unit,
    in_lif_profile_info_t * in_lif_profile_info,
    dbal_enum_value_field_cs_lif_profile_flags_e * cs_lif_profile_flags)
{

    if (in_lif_profile_info->ingress_fields.l3_ingress_flags & BCM_L3_INGRESS_MPLS_INTF_NAMESPACE)
    {
        *cs_lif_profile_flags = DBAL_ENUM_FVAL_CS_LIF_PROFILE_FLAGS_MPLS_INTERFACE;
    }
    else if (in_lif_profile_info->
             ingress_fields.cs_in_lif_profile_flags & DNX_IN_LIF_PROFILE_CS_PROFILE_LIF_VXLAN_OVER_ESP)
    {
        *cs_lif_profile_flags = DBAL_ENUM_FVAL_CS_LIF_PROFILE_FLAGS_VXLAN_ESP;
    }
}

/**
 * \brief
 *   calculates saves in swstate CS LIF profile relevant information
 * \param [in] unit -
 *   The unit number.
 * \param [in] in_lif_profile_info -
 *   Pointer to in_lif_profile_info
 *   includes the lif profile properties
 * \return
 *   \retval Zero if no error was detected
 *   \retval Negative if error was detected. See \ref shr_error_e
 * \see
 *   * None
 */
static shr_error_e
dnx_cs_lif_profile_sw_state_add(
    int unit,
    in_lif_profile_info_t * in_lif_profile_info)
{
    uint32 entry_handle_id;
    uint8 entry_exists = 0;
    int rv;
    dbal_enum_value_field_cs_lif_profile_flags_e cs_lif_profile_flags = 0;
    SHR_FUNC_INIT_VARS(unit);
    DBAL_FUNC_INIT_VARS(unit);

    /**
     * We need to save the flags, as we get into a collision between VXLAN_ESP and MPLS_INTERFACE
     */
    SHR_IF_ERR_EXIT(DBAL_HANDLE_ALLOC(unit, DBAL_TABLE_CS_LIF_PROFILE_FLAGS_PER_GPORT_SW, &entry_handle_id));
    dbal_entry_key_field32_set(unit, entry_handle_id, DBAL_FIELD_GPORT, in_lif_profile_info->ingress_fields.lif_gport);
    rv = dbal_entry_get(unit, entry_handle_id, DBAL_GET_ALL_FIELDS);
    /** If this is the first time we write the entry, we should commit it, otherwise in case of "exchange", we should update it */
    if (rv == _SHR_E_NONE)
    {
        entry_exists = 1;
    }
    dnx_lif_profile_sw_state_lif_profile_flags_get(unit, in_lif_profile_info, &cs_lif_profile_flags);
    dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_CS_LIF_PROFILE_FLAGS, INST_SINGLE,
                                 cs_lif_profile_flags);
    SHR_IF_ERR_EXIT(dbal_entry_commit(unit, entry_handle_id, entry_exists ? DBAL_COMMIT_UPDATE : DBAL_COMMIT));
exit:
    DBAL_FUNC_FREE_VARS;
    SHR_FUNC_EXIT;
}
/**
 * \brief
 *   calculates cs_in_lif_profile_value according to in_lif_profile properties
 * \param [in] unit -
 *   The unit number.
 * \param [in] intf_type -
 *   can be ETH_RIF or LIF
 * \param [in] inlif_dbal_table_id -
 *   used to retrieve information about dbal table of the LIF
 * \param [in] in_lif_profile_info -
 *   Pointer to in_lif_profile_info
 *   includes the lif profile properties
 * \param [out] cs_in_lif_profile_value
 *   the concluded cs_in_lif_profile value
 * \param [out] cs_in_lif_fwd_profile_value
 *   the concluded cs_in_lif_fwd_profile value
 * \return
 *   \retval Zero if no error was detected
 *   \retval Negative if error was detected. See \ref shr_error_e
 * \see
 *   * None
 */
static shr_error_e
dnx_in_lif_profile_algo_convert_to_cs_in_lif_profile(
    int unit,
    in_lif_intf_type_e intf_type,
    dbal_tables_e inlif_dbal_table_id,
    in_lif_profile_info_t * in_lif_profile_info,
    uint8 *cs_in_lif_profile_value,
    uint8 *cs_in_lif_fwd_profile_value)
{
    uint32 vtt_cs_in_lif_proifile = 0, flp_cs_in_lif_profile = 0;
    SHR_FUNC_INIT_VARS(unit);

    if (inlif_dbal_table_id == DBAL_TABLE_IN_LIF_FORMAT_PWE)
    {
        dbal_enum_value_field_lif_vtt4_csp_1bit_pwe_tag_namespace_e pwe_namespace = 0;
        dbal_enum_value_field_nof_sd_tags_e pwe_nof_tags = 0;

        /*
         * LIF type is PWE, only DNX_IN_LIF_PROFILE_CS_PROFILE_PWE_LIF_SCOPED and
         * in_lif_profile_info_t.pwe_nof_eth_tags are relevant
         */
        if ((in_lif_profile_info->ingress_fields.l3_ingress_flags & BCM_L3_INGRESS_GLOBAL_ROUTE)
            || (in_lif_profile_info->ingress_fields.l3_ingress_flags & BCM_L3_INGRESS_MPLS_INTF_NAMESPACE))
        {
            SHR_ERR_EXIT(_SHR_E_FAIL, "invalid fields or flags for PWE LIF\n");
        }

        pwe_nof_tags = DBAL_ENUM_FVAL_NOF_SD_TAGS_0_SD_TAGS + in_lif_profile_info->ingress_fields.pwe_nof_eth_tags;
        /*
         * Second, set the context scope (LiF/Network) scoped
         */
        if (in_lif_profile_info->ingress_fields.cs_in_lif_profile_flags & DNX_IN_LIF_PROFILE_CS_PROFILE_PWE_LIF_SCOPED)
        {
            pwe_namespace = DBAL_ENUM_FVAL_LIF_VTT4_CSP_1BIT_PWE_TAG_NAMESPACE_LIF_SCOPED;
        }
        else
        {
            pwe_namespace = DBAL_ENUM_FVAL_LIF_VTT4_CSP_1BIT_PWE_TAG_NAMESPACE_NWK_SCOPED;
        }

        /** VTT CS perofile encoding*/
        SHR_IF_ERR_EXIT(dbal_fields_struct_field_encode(unit, DBAL_FIELD_PWE_INLIF_CS_PROFILE, DBAL_FIELD_NOF_SD_TAGS,
                                                        &pwe_nof_tags, &vtt_cs_in_lif_proifile));
        SHR_IF_ERR_EXIT(dbal_fields_struct_field_encode(unit, DBAL_FIELD_PWE_INLIF_CS_PROFILE, DBAL_FIELD_TAG_NAMESPACE,
                                                        &pwe_namespace, &vtt_cs_in_lif_proifile));

        /** FLP CS perofile encoding*/
        SHR_IF_ERR_EXIT(dbal_fields_struct_field_encode(unit, DBAL_FIELD_BRIDGING_FWD12FWD2_INLIF_CS_PROFILE,
                                                        DBAL_FIELD_EXTENDED_P2P_NOF_SD_TAGS, &pwe_nof_tags,
                                                        &flp_cs_in_lif_profile));
    }

    else if (inlif_dbal_table_id == DBAL_TABLE_IN_LIF_FORMAT_EVPN)
    {
        dbal_enum_value_field_lif_vttx_cs_evpn_iml_ah_e evpn_iml_ah = 0;
        dbal_enum_value_field_lif_vtt4_csp_1bit_pwe_tag_namespace_e pwe_namespace = 0;

        /*
         * LIF type is PWE, only DNX_IN_LIF_PROFILE_CS_PROFILE_PWE_LIF_SCOPED and
         * in_lif_profile_info_t.pwe_nof_eth_tags are relevant
         */
        if ((in_lif_profile_info->ingress_fields.l3_ingress_flags & BCM_L3_INGRESS_GLOBAL_ROUTE)
            || (in_lif_profile_info->ingress_fields.l3_ingress_flags & BCM_L3_INGRESS_MPLS_INTF_NAMESPACE))
        {
            SHR_ERR_EXIT(_SHR_E_FAIL, "invalid fields or flags for EVPN_IML\n");
        }
        /*
         * set the context scope, Additional headers scoped
         */
        if (in_lif_profile_info->
            ingress_fields.cs_in_lif_profile_flags & DNX_IN_LIF_PROFILE_CS_PROFILE_EVPN_IML_AH_NO_FL)
        {
            evpn_iml_ah = DBAL_ENUM_FVAL_LIF_VTTX_CS_EVPN_IML_AH_NO_FL;
        }
        if (in_lif_profile_info->
            ingress_fields.cs_in_lif_profile_flags & DNX_IN_LIF_PROFILE_CS_PROFILE_EVPN_IML_AH_FL_ONLY)
        {
            evpn_iml_ah = DBAL_ENUM_FVAL_LIF_VTTX_CS_EVPN_IML_AH_FL_ONLY;
        }
        if (in_lif_profile_info->
            ingress_fields.cs_in_lif_profile_flags & DNX_IN_LIF_PROFILE_CS_PROFILE_EVPN_IML_AH_FL_CW)
        {
            evpn_iml_ah = DBAL_ENUM_FVAL_LIF_VTTX_CS_EVPN_IML_AH_FL_AND_CW;
        }

        /*
         * set the context scope (LiF/Network) scoped
         */
        if (in_lif_profile_info->
            ingress_fields.cs_in_lif_profile_flags & DNX_IN_LIF_PROFILE_CS_PROFILE_EVPN_IML_LIF_SCOPED)
        {
            pwe_namespace = DBAL_ENUM_FVAL_LIF_VTT4_CSP_1BIT_PWE_TAG_NAMESPACE_LIF_SCOPED;
        }
        else
        {
            pwe_namespace = DBAL_ENUM_FVAL_LIF_VTT4_CSP_1BIT_PWE_TAG_NAMESPACE_NWK_SCOPED;
        }
        SHR_IF_ERR_EXIT(dbal_fields_struct_field_encode
                        (unit, DBAL_FIELD_EVPN_IML_INLIF_CS_PROFILE, DBAL_FIELD_IML_ADDITIONAL_HEADERS, &evpn_iml_ah,
                         &vtt_cs_in_lif_proifile));
        SHR_IF_ERR_EXIT(dbal_fields_struct_field_encode
                        (unit, DBAL_FIELD_EVPN_IML_INLIF_CS_PROFILE, DBAL_FIELD_TAG_NAMESPACE, &pwe_namespace,
                         &vtt_cs_in_lif_proifile));
    }

    else if ((inlif_dbal_table_id == DBAL_TABLE_IN_LIF_IPvX_TUNNELS) ||
             (inlif_dbal_table_id == DBAL_TABLE_IN_LIF_FORMAT_LSP) ||
             (inlif_dbal_table_id == dnx_data_l2.vsi.vsi_table_get(unit)))
    {
        dbal_enum_value_field_lif_vtt5_csp_1bit_l3lif_uc_db_mode_e uc_mode;
        dbal_enum_value_field_lif_vtt5_csp_1bit_nat_direction_and_virtual_ip_and_srv6_gsid_e
            nat_or_virtual_ip_or_srv6_gsid;
        dbal_enum_value_field_lif_xxxx_csp_1bit_l3lif_mpls_namespace_and_algo_acl_or_esp_vxlan_e
            mpls_namespace_or_aacl_or_esp_vxlan;
        /*
         * LIF type is LIF or ETH_RIF, add relevant common parts
         */
        if (in_lif_profile_info->ingress_fields.pwe_nof_eth_tags > ZERO_TAGS)
        {
            SHR_ERR_EXIT(_SHR_E_FAIL, "invalid fields or flags for LIF\n");
        }


        if (in_lif_profile_info->ingress_fields.l3_ingress_flags & BCM_L3_INGRESS_FIELD_COMPRESSION_ENABLE)
        {
            uint8 aacl_valid = 0;

            if (!aacl_valid)
            {
                SHR_ERR_EXIT(_SHR_E_FAIL, "FIELD_COMPRESSION flag cannot be used, as Algo ACLs are disabled\n");
            }
        }

        if ((in_lif_profile_info->ingress_fields.l3_ingress_flags & BCM_L3_INGRESS_FIELD_COMPRESSION_ENABLE) &&
            (in_lif_profile_info->ingress_fields.l3_ingress_flags & BCM_L3_INGRESS_MPLS_INTF_NAMESPACE))
        {
            SHR_ERR_EXIT(_SHR_E_FAIL,
                         "Compressed Field and MPLS per interface cannot be set together on the same LIF\n");
        }
        if (((in_lif_profile_info->ingress_fields.l3_ingress_flags & BCM_L3_INGRESS_FIELD_COMPRESSION_ENABLE) ||
             (in_lif_profile_info->ingress_fields.l3_ingress_flags & BCM_L3_INGRESS_MPLS_INTF_NAMESPACE))
            && (in_lif_profile_info->
                ingress_fields.cs_in_lif_profile_flags & DNX_IN_LIF_PROFILE_CS_PROFILE_LIF_VXLAN_OVER_ESP))
        {
            SHR_ERR_EXIT(_SHR_E_FAIL,
                         "Compressed Field and MPLS per interface cannot be set with the ESPoVXLAN flag\n");
        }

        if (in_lif_profile_info->
            ingress_fields.cs_in_lif_profile_flags & DNX_IN_LIF_PROFILE_CS_PROFILE_LIF_WITH_SRV6_GSID)
        {
            if (in_lif_profile_info->ingress_fields.l3_ingress_flags & BCM_L3_INGRESS_VIP_ENABLED)
            {
                SHR_ERR_EXIT(_SHR_E_FAIL, "SRv6 GSID EOC and Virtual IP cannot be set together on the same LIF\n");
            }
        }
        if (in_lif_profile_info->ingress_fields.l3_ingress_flags & BCM_L3_INGRESS_GLOBAL_ROUTE)
        {
            uc_mode = DBAL_ENUM_FVAL_LIF_VTT5_CSP_1BIT_L3LIF_UC_DB_MODE_UC_PRIVATE_PUBLIC_OPTIMIZED;
        }
        else
        {
            uc_mode = DBAL_ENUM_FVAL_LIF_VTT5_CSP_1BIT_L3LIF_UC_DB_MODE_UC_PRIVATE_ONLY;
        }

        if ((in_lif_profile_info->ingress_fields.l3_ingress_flags & BCM_L3_INGRESS_MPLS_INTF_NAMESPACE) ||
            (in_lif_profile_info->ingress_fields.l3_ingress_flags & BCM_L3_INGRESS_FIELD_COMPRESSION_ENABLE) ||
            (in_lif_profile_info->
             ingress_fields.cs_in_lif_profile_flags & DNX_IN_LIF_PROFILE_CS_PROFILE_LIF_VXLAN_OVER_ESP))
        {
            SHR_IF_ERR_EXIT(dnx_cs_lif_profile_sw_state_add(unit, in_lif_profile_info));
            mpls_namespace_or_aacl_or_esp_vxlan =
                DBAL_ENUM_FVAL_LIF_XXXX_CSP_1BIT_L3LIF_MPLS_NAMESPACE_AND_ALGO_ACL_OR_ESP_VXLAN_MPLS_PER_INTERFACE_AACL_OR_ESP_VXLAN_ENABLED;
        }
        else
        {
            mpls_namespace_or_aacl_or_esp_vxlan =
                DBAL_ENUM_FVAL_LIF_XXXX_CSP_1BIT_L3LIF_MPLS_NAMESPACE_AND_ALGO_ACL_OR_ESP_VXLAN_MPLS_PER_PLATFORM_AACL_OR_ESP_VXLAN_DISABLED;
        }

        if (in_lif_profile_info->ingress_fields.l3_ingress_flags & BCM_L3_INGRESS_VIP_ENABLED)
        {
            nat_or_virtual_ip_or_srv6_gsid =
                DBAL_ENUM_FVAL_LIF_VTT5_CSP_1BIT_NAT_DIRECTION_AND_VIRTUAL_IP_AND_SRV6_GSID_NAT_DOWNSTREAM_VIP_ENABLED_SRV6_GSID_EOC;
        }
        else
        {
            if (in_lif_profile_info->
                ingress_fields.cs_in_lif_profile_flags & DNX_IN_LIF_PROFILE_CS_PROFILE_LIF_WITH_SRV6_GSID)
            {
                nat_or_virtual_ip_or_srv6_gsid =
                    DBAL_ENUM_FVAL_LIF_VTT5_CSP_1BIT_NAT_DIRECTION_AND_VIRTUAL_IP_AND_SRV6_GSID_NAT_DOWNSTREAM_VIP_ENABLED_SRV6_GSID_EOC;
            }
            else
            {
                nat_or_virtual_ip_or_srv6_gsid =
                    DBAL_ENUM_FVAL_LIF_VTT5_CSP_1BIT_NAT_DIRECTION_AND_VIRTUAL_IP_AND_SRV6_GSID_NAT_UPSTREAM_VIP_DISABLED_SRV6_GSID_SOC;
            }
        }

        SHR_IF_ERR_EXIT(dbal_fields_struct_field_encode
                        (unit, DBAL_FIELD_ROUTING_VTT2FWD1_INLIF_CS_PROFILE,
                         DBAL_FIELD_NAT_DIRECTION_OR_VIP_MODE_OR_SRV6_GSID, &nat_or_virtual_ip_or_srv6_gsid,
                         &vtt_cs_in_lif_proifile));
        SHR_IF_ERR_EXIT(dbal_fields_struct_field_encode
                        (unit, DBAL_FIELD_ROUTING_VTT2FWD1_INLIF_CS_PROFILE,
                         DBAL_FIELD_MPLS_NAMESPACE_OR_AACL_OR_ESP_VXLAN, &mpls_namespace_or_aacl_or_esp_vxlan,
                         &vtt_cs_in_lif_proifile));
        SHR_IF_ERR_EXIT(dbal_fields_struct_field_encode
                        (unit, DBAL_FIELD_ROUTING_VTT2FWD1_INLIF_CS_PROFILE, DBAL_FIELD_UC_DB_MODE, &uc_mode,
                         &vtt_cs_in_lif_proifile));

        /** Encoding FLP CS profile*/
        if (inlif_dbal_table_id == dnx_data_l2.vsi.vsi_table_get(unit))
        {
            if (in_lif_profile_info->ingress_fields.l3_ingress_flags & BCM_L3_INGRESS_IPMC_BRIDGE_FALLBACK)
            {
                flp_cs_in_lif_profile |=
                    (DBAL_ENUM_FVAL_LIF_FWD1_CSP_1BIT_ETH_RIF_BRIDGE_FALLBACK_MODE_IP_COMPATIBLE_MC_BRIDGE_FALLBACK_ENABLE
                     << IN_LIF_PROFILE_IPMC_BRIDGE_FALLBACK_BIT_OFFSET);
            }
            else
            {
                flp_cs_in_lif_profile |=
                    (DBAL_ENUM_FVAL_LIF_FWD1_CSP_1BIT_ETH_RIF_BRIDGE_FALLBACK_MODE_IP_COMPATIBLE_MC_BRIDGE_FALLBACK_DISABLE
                     << IN_LIF_PROFILE_IPMC_BRIDGE_FALLBACK_BIT_OFFSET);
            }
        }
    }

    *cs_in_lif_profile_value = (uint8) vtt_cs_in_lif_proifile;
    *cs_in_lif_fwd_profile_value = (uint8) flp_cs_in_lif_profile;
exit:
    SHR_FUNC_EXIT;
}

/**
 * \brief
 *   calculates RPF mode value, returns RPF mode
 * \par DIRECT INPUT
 *   \param [in] unit -
 *     The unit number.
 *   \param [in] rpf_mode -
 *     Pointer to rpf_mode \n
 *     \b As \b output - \n
 *       the rpf_mode to return
 *   \param [in] in_lif_profile_info -
 *     Pointer to in_lif_profile_info \n
 *     \b As \b input - \n
 *       includes the lif profile properties, only flags is used
 * \par INDIRECT INPUT
 *   * \b *in_lif_profile_info \n
 *     See 'in_lif_profile_info' in DIRECT INPUT above
 * \par DIRECT OUTPUT
 *   \retval Zero if no error was detected
 *   \retval Negative if error was detected. See \ref shr_error_e
 * \par INDIRECT OUTPUT
 *   * \b *rpf_mode \n
 *     See 'rpf_mode' in DIRECT INPUT above
 * \see
 *   * None
 */
static shr_error_e
dnx_in_lif_profile_algo_convert_to_rpf_mode(
    int unit,
    uint8 *rpf_mode,
    in_lif_profile_info_t * in_lif_profile_info)
{

    SHR_FUNC_INIT_VARS(unit);
    DBAL_FUNC_INIT_VARS(unit);

    /*
     * handle RPF_MODE
     */
    switch (in_lif_profile_info->ingress_fields.urpf_mode)
    {
        case bcmL3IngressUrpfLoose:
            *rpf_mode = DBAL_ENUM_FVAL_RPF_MODE_LOOSE;
            break;
        case bcmL3IngressUrpfStrict:
            *rpf_mode = DBAL_ENUM_FVAL_RPF_MODE_STRICT;
            break;
        case bcmL3IngressUrpfDisable:
            *rpf_mode = DBAL_ENUM_FVAL_RPF_MODE_DISABLE;
            break;
        default:
            SHR_ERR_EXIT(_SHR_E_PARAM, "urpf_mode invalid : %d\n", in_lif_profile_info->ingress_fields.urpf_mode);
            break;
    }

exit:
    DBAL_FUNC_FREE_VARS;
    SHR_FUNC_EXIT;
}

/**
 * \brief -
 *  Allocate a da_not_found_dst_profile per LIF according to the default
 *  destinations for unknown packets.
 *
 * \param [in] unit - The unit ID
 * \param [in] old_in_lif_profile - old in_lif_profile the old
 *             da_not_found_dst_profile residing in.
 * \param [in] intf_type - can be one of the following:
 *             ETH_RIF - interface type is ETH_RIF
 *             LIF - interface type is LIF
 * \param [in] default_frwrd_dst - Default destinations for unknown packets.
 * \param [out] da_not_found_dst_profile - da_not_found_dst_profile per LIF.
 * \param [out] last_reference - Indication that the last reference to the old_in_lif_profile was removed.
 *
 * \return
 *   \retval  Negative if error was detected. See \ref shr_error_e
 *
 * \remark
 *   The default destination may be a set of offset to VSI.default_frwrd_destionation,
 *   or a set of destinations for unknown UC, MC and BC packets.
 *
 * \see
 *   dnx_in_lif_profile_exchange.
 */
static shr_error_e
dnx_in_lif_profile_algo_handle_lif_flood_profile(
    int unit,
    int old_in_lif_profile,
    in_lif_intf_type_e intf_type,
    dnx_default_frwrd_dst_t default_frwrd_dst[DBAL_NOF_ENUM_DESTINATION_DISTRIBUTION_TYPE_VALUES],
    int *da_not_found_dst_profile,
    uint8 *last_reference)
{
    int lif_flood_profile, port_flood_profile;
    uint8 first_reference;
    in_lif_template_data_t in_lif_profile_template_data;

    SHR_FUNC_INIT_VARS(unit);
    DBAL_FUNC_INIT_VARS(unit);

    SHR_NULL_CHECK(da_not_found_dst_profile, _SHR_E_PARAM, "A valid pointer for da_not_found_dst_profile is needed!");

    /*
     * Get in_lif_profile template data
     */
    SHR_IF_ERR_EXIT(dnx_in_lif_profile_template_data_get
                    (unit, old_in_lif_profile, intf_type, &in_lif_profile_template_data));

    /*
     * Exchange the lif_flood_profile.
     */
    lif_flood_profile = 0;
    first_reference = FALSE;
    SHR_IF_ERR_EXIT(algo_lif_profile_db.in_lif_da_not_found_destination.exchange
                    (unit, 0, default_frwrd_dst, in_lif_profile_template_data.da_not_found_dst_profile,
                     NULL, &lif_flood_profile, &first_reference, last_reference));

    *da_not_found_dst_profile = lif_flood_profile;

    /*
     * If new lif_flood_profile is used for the first time, update its hw table.
     */
    if (first_reference)
    {
        port_flood_profile = DBAL_ENUM_FVAL_DA_NOT_FOUND_ACTION_PROFILE_NONE;
        SHR_IF_ERR_EXIT(dnx_port_lif_flood_profile_action_set
                        (unit, port_flood_profile, lif_flood_profile, default_frwrd_dst));
    }

exit:
    DBAL_FUNC_FREE_VARS;
    SHR_FUNC_EXIT;
}

/**
 * \see
 *   in_lif_profile.h file
 */
shr_error_e
dnx_in_lif_profile_dealloc(
    int unit,
    int in_lif_profile,
    int *new_in_lif_profile,
    in_lif_profile_info_t * in_lif_profile_info,
    in_lif_intf_type_e intf_type)
{
    uint8 is_first, is_last_routing_profile, is_last_lif_profile, is_first_lif_lb_profile, is_last_lif_lb_profile,
        is_first_lif_flood_profile, is_last_lif_flood_profile;
    in_lif_template_data_t default_in_lif_profile_template_data;
    in_lif_template_data_t old_in_lif_profile_data;
    uint32 routing_enablers_vector[DNX_DATA_MAX_L3_ROUTING_ENABLERS_ROUTING_ENABLE_VECTOR_LENGTH / SAL_UINT32_NOF_BITS],
        old_routing_enablers_profile;
    uint32 in_lif_lb_profile_disablers_flags_vector = DNX_IN_LIF_LB_PROFILE_DISABLERS_FLAGS_DEFAULT;
    int ref_count = 0,
        routing_enablers_profile, egress_reserved_nof_bits, pmf_reserved_nof_bits, indexed_mode_nof_bits,
        eth_rif_profile_used_bits_mask, eth_rif_profile_egress_reserved_start_bit,
        in_lif_profile_egress_reserved_start_bit, in_lif_profile_used_bits_mask, in_lif_lb_profile, old_lif_lb_profile,
        old_lif_flood_profile, lif_flood_profile;
    dnx_default_frwrd_dst_t default_frwrd_dst[DBAL_NOF_ENUM_DESTINATION_DISTRIBUTION_TYPE_VALUES];

    SHR_FUNC_INIT_VARS(unit);
    DBAL_FUNC_INIT_VARS(unit);
    sal_memset(&default_in_lif_profile_template_data, 0, sizeof(default_in_lif_profile_template_data));
#ifdef BCM_DNX2_SUPPORT
    if (dnx_data_dev_init.general.feature_get(unit, dnx_data_dev_init_general_application_v1))
    {
        default_in_lif_profile_template_data.mact_mode = DBAL_ENUM_FVAL_VSI_PROFILE_BRIDGE_SVL;
    }
#endif

    is_first = 0;
    is_last_routing_profile = 0;
    is_last_lif_profile = 0;
    is_last_lif_flood_profile = 0;

    /*
     * init to default, Eth enabler is turned on.
     */
    SHR_IF_ERR_EXIT(dnx_enablers_vectors_initial_set(unit, routing_enablers_vector));

    egress_reserved_nof_bits = 0;
    old_routing_enablers_profile = 0;
    old_lif_lb_profile = 0;
    is_first_lif_lb_profile = 0;
    is_last_lif_lb_profile = 0;
    old_lif_flood_profile = 0;

    
    if (dnx_data_dev_init.general.feature_get(unit, dnx_data_dev_init_general_application_v1))
    {
        lif_flood_profile = dnx_data_lif.in_lif.default_da_not_found_destination_profile_get(unit);
    }
    indexed_mode_nof_bits = NATIVE_INDEXED_MODE_NOF_BITS;
    /*
     * there are 2 options:
     * the profile is ETH-RIF profile, in this case the old profile exists in DNX_ALGO_ETH_RIF_PROFILE
     * the profile is LIF profile, in this case the old profile exists in DNX_ALGO_IN_LIF_PROFILE
     */
    if (intf_type == ETH_RIF)
    {
        SHR_IF_ERR_EXIT(dnx_eth_rif_profile_used_bits_mask_get
                        (unit, &eth_rif_profile_used_bits_mask, &egress_reserved_nof_bits,
                         &eth_rif_profile_egress_reserved_start_bit));
        in_lif_profile = in_lif_profile & eth_rif_profile_used_bits_mask;
        SHR_IF_ERR_EXIT(algo_lif_profile_db.eth_rif_profile.profile_data_get
                        (unit, in_lif_profile, &ref_count, &old_in_lif_profile_data));
        pmf_reserved_nof_bits = ETH_RIF_PROFILE_PMF_RESERVED_NOF_BITS;
    }
    /** Free the sw state allocated profiles by exchanging the profile with the default one*/
    else if (intf_type == LIF)
    {
        SHR_IF_ERR_EXIT(dnx_in_lif_profile_used_bits_mask_get
                        (unit, &in_lif_profile_used_bits_mask, &egress_reserved_nof_bits,
                         &in_lif_profile_egress_reserved_start_bit));
        in_lif_profile = in_lif_profile & in_lif_profile_used_bits_mask;
        SHR_IF_ERR_EXIT(algo_lif_profile_db.in_lif_profile.profile_data_get
                        (unit, in_lif_profile, &ref_count, &old_in_lif_profile_data));
        pmf_reserved_nof_bits = IN_LIF_PROFILE_PMF_RESERVED_NOF_BITS;
    }
    else
    {
        SHR_ERR_EXIT(_SHR_E_FAIL, "invalid intf_type\n");
    }

    /*
     * Update the proper bits in enablers vector according to user's flags
     */
    old_routing_enablers_profile = old_in_lif_profile_data.routing_enablers_profile;
    SHR_IF_ERR_EXIT(algo_lif_profile_db.in_lif_profile_l3_enablers.exchange
                    (unit, 0, routing_enablers_vector, old_routing_enablers_profile,
                     NULL, &routing_enablers_profile, &is_first, &is_last_routing_profile));
    default_in_lif_profile_template_data.routing_enablers_profile = routing_enablers_profile;

    if (dnx_data_switch.load_balancing.hashing_selection_per_layer_get(unit))
    {
        old_lif_lb_profile = old_in_lif_profile_data.in_lif_lb_profile;
        SHR_IF_ERR_EXIT(algo_lif_profile_db.in_lif_lb_profile_disablers_flags.exchange
                        (unit, 0, &in_lif_lb_profile_disablers_flags_vector, old_lif_lb_profile,
                         NULL, &in_lif_lb_profile, &is_first_lif_lb_profile, &is_last_lif_lb_profile));
    }
    if (intf_type == ETH_RIF)
    {
        SHR_IF_ERR_EXIT(algo_lif_profile_db.eth_rif_profile.exchange
                        (unit, 0, &default_in_lif_profile_template_data,
                         in_lif_profile, NULL, new_in_lif_profile, &is_first, &is_last_lif_profile));
    }
    else
    {
        old_lif_flood_profile = old_in_lif_profile_data.da_not_found_dst_profile;
        
        if (dnx_data_dev_init.general.feature_get(unit, dnx_data_dev_init_general_application_v1))
        {
            SHR_IF_ERR_EXIT(algo_lif_profile_db.in_lif_da_not_found_destination.profile_data_get
                            (unit, lif_flood_profile, &ref_count, default_frwrd_dst));
            SHR_IF_ERR_EXIT(algo_lif_profile_db.in_lif_da_not_found_destination.exchange
                            (unit, 0, default_frwrd_dst, old_lif_flood_profile,
                             NULL, &lif_flood_profile, &is_first_lif_flood_profile, &is_last_lif_flood_profile));
        }
        SHR_IF_ERR_EXIT(algo_lif_profile_db.in_lif_profile.exchange
                        (unit, 0, &default_in_lif_profile_template_data,
                         in_lif_profile, NULL, new_in_lif_profile, &is_first, &is_last_lif_profile));
    }

    if (is_last_routing_profile)
    {
        SHR_IF_ERR_EXIT(dnx_enablers_vectors_table_init(unit, old_routing_enablers_profile, TRUE));
    }
    if (is_last_lif_flood_profile)
    {
        int port_flood_profile = DBAL_ENUM_FVAL_DA_NOT_FOUND_ACTION_PROFILE_NONE;
        SHR_IF_ERR_EXIT(dnx_port_lif_flood_profile_action_set
                        (unit, port_flood_profile, old_lif_flood_profile, default_frwrd_dst));
    }
    if (dnx_data_switch.load_balancing.hashing_selection_per_layer_get(unit))
    {
        if (is_last_lif_lb_profile)
        {
            SHR_IF_ERR_EXIT(dnx_in_lif_profile_lif_lb_profile_init(unit, old_lif_lb_profile));
        }
    }
    if (is_last_lif_profile)
    {
        if (dnx_data_lif.in_lif.feature_get(unit, dnx_data_lif_in_lif_use_mapped_in_lif_profile))
        {
            SHR_IF_ERR_EXIT(dnx_in_lif_profile_table_for_mapped_in_lif_profile_dealloc
                            (unit, in_lif_profile, pmf_reserved_nof_bits, indexed_mode_nof_bits, intf_type));
            SHR_IF_ERR_EXIT(dnx_vsi_profile_table_for_mapped_in_lif_profile_dealloc
                            (unit, in_lif_profile, pmf_reserved_nof_bits, intf_type));
        }
        else
        {
            SHR_IF_ERR_EXIT(dnx_in_lif_profile_table_init
                            (unit, in_lif_profile, TRUE, pmf_reserved_nof_bits, egress_reserved_nof_bits,
                             indexed_mode_nof_bits, intf_type));
            SHR_IF_ERR_EXIT(dnx_vsi_profile_table_init
                            (unit, in_lif_profile, TRUE, pmf_reserved_nof_bits, egress_reserved_nof_bits, intf_type));
        }
    }
    if (dnx_data_lif.in_lif.feature_get(unit, dnx_data_lif_in_lif_cs_in_lif_profile_based_on_stages))
    {
    }
    *new_in_lif_profile = DNX_IN_LIF_PROFILE_DEFAULT;

exit:
    DBAL_FUNC_FREE_VARS;
    SHR_FUNC_EXIT;
}

/** Refer to in_lif_profile.h file for more information */
shr_error_e
dnx_in_lif_profile_destroy(
    int unit,
    dnx_in_lif_profile_last_info_t last_profile,
    in_lif_intf_type_e intf_type)
{
    int pmf_reserved_nof_bits = 0, egress_reserved_nof_bits = 0, indexed_mode_nof_bits = 0,
        eth_rif_profile_used_bits_mask = 0, in_lif_profile_egress_reserved_start_bit = 0,
        in_lif_profile_used_bits_mask = 0;
    SHR_FUNC_INIT_VARS(unit);

    indexed_mode_nof_bits = NATIVE_INDEXED_MODE_NOF_BITS;
    pmf_reserved_nof_bits = 0;
    egress_reserved_nof_bits = 0;
    eth_rif_profile_used_bits_mask = 0;
    in_lif_profile_egress_reserved_start_bit = 0;
    in_lif_profile_used_bits_mask = 0;
    /*
     * there are 2 options:
     * the profile is ETH-RIF profile, in this case the old profile exists in DNX_ALGO_ETH_RIF_PROFILE
     * the profile is LIF profile, in this case the old profile exists in DNX_ALGO_IN_LIF_PROFILE
     */
    if (intf_type == ETH_RIF)
    {
        SHR_IF_ERR_EXIT(dnx_eth_rif_profile_used_bits_mask_get
                        (unit, &eth_rif_profile_used_bits_mask, &egress_reserved_nof_bits,
                         &in_lif_profile_egress_reserved_start_bit));
        pmf_reserved_nof_bits = ETH_RIF_PROFILE_PMF_RESERVED_NOF_BITS;
    }
    /** Free the sw state allocated profiles by exchanging the profile with the default one*/
    else if (intf_type == LIF)
    {
        SHR_IF_ERR_EXIT(dnx_in_lif_profile_used_bits_mask_get
                        (unit, &in_lif_profile_used_bits_mask, &egress_reserved_nof_bits,
                         &in_lif_profile_egress_reserved_start_bit));
        pmf_reserved_nof_bits = IN_LIF_PROFILE_PMF_RESERVED_NOF_BITS;
    }
    else
    {
        SHR_ERR_EXIT(_SHR_E_FAIL, "invalid intf_type\n");
    }

    if (last_profile.old_in_lif_profile != DNX_IN_LIF_PROFILE_DEFAULT)
    {
        if (intf_type == ETH_RIF)
        {
            if (dnx_data_lif.in_lif.feature_get(unit, dnx_data_lif_in_lif_use_mapped_in_lif_profile))
            {
                SHR_IF_ERR_EXIT(dnx_in_lif_profile_table_for_mapped_in_lif_profile_dealloc
                                (unit, last_profile.old_in_lif_profile & eth_rif_profile_used_bits_mask,
                                 pmf_reserved_nof_bits, indexed_mode_nof_bits, intf_type));
                SHR_IF_ERR_EXIT(dnx_vsi_profile_table_for_mapped_in_lif_profile_dealloc
                                (unit, last_profile.old_in_lif_profile & eth_rif_profile_used_bits_mask,
                                 pmf_reserved_nof_bits, intf_type));
            }
            else
            {
                SHR_IF_ERR_EXIT(dnx_in_lif_profile_table_init
                                (unit, (last_profile.old_in_lif_profile & eth_rif_profile_used_bits_mask), TRUE,
                                 pmf_reserved_nof_bits, egress_reserved_nof_bits, indexed_mode_nof_bits, intf_type));
                SHR_IF_ERR_EXIT(dnx_vsi_profile_table_init
                                (unit, (last_profile.old_in_lif_profile & eth_rif_profile_used_bits_mask), TRUE,
                                 pmf_reserved_nof_bits, egress_reserved_nof_bits, intf_type));
            }
        }
        else
        {
            if (dnx_data_lif.in_lif.feature_get(unit, dnx_data_lif_in_lif_use_mapped_in_lif_profile))
            {
                SHR_IF_ERR_EXIT(dnx_in_lif_profile_table_for_mapped_in_lif_profile_dealloc
                                (unit, last_profile.old_in_lif_profile & in_lif_profile_used_bits_mask,
                                 pmf_reserved_nof_bits, indexed_mode_nof_bits, intf_type));
                SHR_IF_ERR_EXIT(dnx_vsi_profile_table_for_mapped_in_lif_profile_dealloc
                                (unit, last_profile.old_in_lif_profile & in_lif_profile_used_bits_mask,
                                 pmf_reserved_nof_bits, intf_type));
            }
            else
            {
                SHR_IF_ERR_EXIT(dnx_in_lif_profile_table_init
                                (unit, (last_profile.old_in_lif_profile & in_lif_profile_used_bits_mask), TRUE,
                                 pmf_reserved_nof_bits, egress_reserved_nof_bits, indexed_mode_nof_bits, intf_type));
                SHR_IF_ERR_EXIT(dnx_vsi_profile_table_init
                                (unit, (last_profile.old_in_lif_profile & in_lif_profile_used_bits_mask), TRUE,
                                 pmf_reserved_nof_bits, egress_reserved_nof_bits, intf_type));
            }
        }
    }
    /** Initialize the LB profile and the routing profile to default if there are no references to the profiles */
    if (last_profile.old_routing_enablers_profile != 0)
    {
        SHR_IF_ERR_EXIT(dnx_enablers_vectors_table_init(unit, last_profile.old_routing_enablers_profile, TRUE));
    }

    if (last_profile.old_in_lif_lb_profile != DEFAULT_IN_LIF_LB_PROFILE)
    {
        SHR_IF_ERR_EXIT(dnx_in_lif_profile_lif_lb_profile_init(unit, last_profile.old_in_lif_lb_profile));
    }

exit:
    SHR_FUNC_EXIT;
}
/**
 * \see
 *   in_lif_profile.h file
 */
shr_error_e
dnx_in_lif_profile_exchange(
    int unit,
    in_lif_profile_info_t * in_lif_profile_info,
    int old_in_lif_profile,
    int *new_in_lif_profile,
    in_lif_intf_type_e intf_type,
    dbal_tables_e inlif_dbal_table_id,
    dnx_in_lif_profile_last_info_t * last_profile)
{
    in_lif_template_data_t in_lif_profile_template_data, old_in_lif_profile_template_data;
    uint32 entry_handle_id;
    int routing_enablers_profile, da_not_found_dst_profile, in_lif_lb_profile, egress_properties_nof_bits;
    int egress_in_lif_profile, hw_write_counter, pmf_reserved_nof_bits, egress_reserved_nof_bits,
        max_counter_value, pmf_reserved_start_bit, pmf_in_lif_profile, pmf_bits_mask, eth_rif_profile_used_bits_mask,
        in_lif_profile_egress_reserved_start_bit, egress_eth_rif_profile, ref_count,
        eth_rif_profile_egress_reserved_start_bit, in_lif_profile_used_bits_mask;
    uint8 cs_vtt_in_lif_profile_value, cs_vtt_to_flp_in_lif_profile_value, rpf_mode, is_first, routing_profile_is_first,
        routing_profile_is_last, cs_in_lif_flp_profile_value, native_indexed_mode_in_lif_profile,
        lif_lb_enablers_profile_first, lif_lb_enablers_profile_last, flood_profile_is_last, lif_profile_is_last;

    SHR_FUNC_INIT_VARS(unit);
    DBAL_FUNC_INIT_VARS(unit);

    /*
     * local variables init
     */
    pmf_reserved_nof_bits = 0;
    in_lif_lb_profile = 0;
    max_counter_value = 0;
    da_not_found_dst_profile = 0;
    is_first = 0;
    lif_profile_is_last = 0;
    routing_profile_is_first = 0;
    routing_profile_is_last = 0;
    flood_profile_is_last = 0;
    rpf_mode = 0;
    sal_memset(&old_in_lif_profile_template_data, 0, sizeof(old_in_lif_profile_template_data));
    cs_vtt_in_lif_profile_value = 0;
    cs_vtt_to_flp_in_lif_profile_value = 0;
    cs_in_lif_flp_profile_value = 0;
    hw_write_counter = 0;
    egress_in_lif_profile = 0;
    pmf_reserved_start_bit = 0;
    pmf_in_lif_profile = 0;
    pmf_bits_mask = 0;
    native_indexed_mode_in_lif_profile = 0;
    in_lif_profile_egress_reserved_start_bit = 0;
    egress_eth_rif_profile = 0;
    eth_rif_profile_used_bits_mask = 0;
    eth_rif_profile_egress_reserved_start_bit = 0;
    ref_count = 0;
    egress_properties_nof_bits = 0;

    if (dnx_data_lif.in_lif.feature_get(unit, dnx_data_lif_in_lif_cs_in_lif_profile_based_on_stages))
    {
    }
    /*
     * there are 2 options:
     * the profile is ETH-RIF profile, in this case the old profile exists in DNX_ALGO_ETH_RIF_PROFILE
     * the profile is LIF profile, in this case the old profile exists in DNX_ALGO_IN_LIF_PROFILE
     */
    if (intf_type == ETH_RIF)
    {
        SHR_IF_ERR_EXIT(dnx_eth_rif_profile_used_bits_mask_get
                        (unit, &eth_rif_profile_used_bits_mask, &egress_reserved_nof_bits,
                         &in_lif_profile_egress_reserved_start_bit));
        SHR_IF_ERR_EXIT(algo_lif_profile_db.eth_rif_profile.profile_data_get
                        (unit, old_in_lif_profile & eth_rif_profile_used_bits_mask, &ref_count,
                         &old_in_lif_profile_template_data));
    }
    /** Free the sw state allocated profiles by exchanging the profile with the default one*/
    else if (intf_type == LIF)
    {
        SHR_IF_ERR_EXIT(dnx_in_lif_profile_used_bits_mask_get
                        (unit, &in_lif_profile_used_bits_mask, &egress_reserved_nof_bits,
                         &in_lif_profile_egress_reserved_start_bit));
        SHR_IF_ERR_EXIT(algo_lif_profile_db.in_lif_profile.profile_data_get
                        (unit, old_in_lif_profile & in_lif_profile_used_bits_mask, &ref_count,
                         &old_in_lif_profile_template_data));
    }
    else
    {
        SHR_ERR_EXIT(_SHR_E_FAIL, "invalid intf_type\n");
    }
    /*
     * handle routing enablers profile
     */
    SHR_IF_ERR_EXIT(dnx_in_lif_profile_algo_handle_routing_enablers_profile
                    (unit, old_in_lif_profile, &routing_enablers_profile, in_lif_profile_info,
                     &routing_profile_is_first, &routing_profile_is_last, intf_type));
    last_profile->old_routing_enablers_profile =
        routing_profile_is_last ? old_in_lif_profile_template_data.routing_enablers_profile : 0;

    /*
     * handle lif lb enablers profile
     */
    if (dnx_data_switch.load_balancing.hashing_selection_per_layer_get(unit))
    {
        SHR_IF_ERR_EXIT(dnx_in_lif_profile_algo_handle_lif_lb_profile
                        (unit, old_in_lif_profile, &in_lif_lb_profile, in_lif_profile_info,
                         &lif_lb_enablers_profile_first, &lif_lb_enablers_profile_last, intf_type));
        last_profile->old_in_lif_lb_profile =
            lif_lb_enablers_profile_last ? old_in_lif_profile_template_data.in_lif_lb_profile :
            DEFAULT_IN_LIF_LB_PROFILE;
    }
    /*
     * handle RPF
     */
    SHR_IF_ERR_EXIT(dnx_in_lif_profile_algo_convert_to_rpf_mode(unit, &rpf_mode, in_lif_profile_info));

    /*
     * handle cs_in_lif_profile
     */
    if (dnx_data_lif.in_lif.feature_get(unit, dnx_data_lif_in_lif_cs_in_lif_profile_based_on_stages))
    {
    }
    else
    {
        SHR_IF_ERR_EXIT(dnx_in_lif_profile_algo_convert_to_cs_in_lif_profile
                        (unit, intf_type, inlif_dbal_table_id, in_lif_profile_info, &cs_vtt_in_lif_profile_value,
                         &cs_in_lif_flp_profile_value));
    }
    /*
     * handle da_not_found_dst_profile
     */
    
    if (dnx_data_lif.feature.feature_get(unit, dnx_data_lif_feature_ingress_egress_vsi_separation))
    {
        SHR_IF_ERR_EXIT(dnx_in_lif_profile_algo_handle_lif_flood_profile
                        (unit, old_in_lif_profile, intf_type, in_lif_profile_info->ingress_fields.default_frwrd_dst,
                         &da_not_found_dst_profile, &flood_profile_is_last));
    }

    last_profile->old_da_not_found_dst_profile =
        flood_profile_is_last ? old_in_lif_profile_template_data.da_not_found_dst_profile : 0;

    /*
     * handle the in_lif template (outer template)
     */
    sal_memset(&in_lif_profile_template_data, 0, sizeof(in_lif_profile_template_data));
    in_lif_profile_template_data.routing_enablers_profile = routing_enablers_profile;
    in_lif_profile_template_data.rpf_mode = rpf_mode;
    in_lif_profile_template_data.cs_vtt_in_lif_profile = cs_vtt_in_lif_profile_value;
    in_lif_profile_template_data.cs_vtt_to_flp_in_lif_profile = cs_vtt_to_flp_in_lif_profile_value;
    in_lif_profile_template_data.cs_flp_in_lif_profile = cs_in_lif_flp_profile_value;
    in_lif_profile_template_data.da_not_found_dst_profile = da_not_found_dst_profile;
    in_lif_profile_template_data.protocol_trap_profiles = in_lif_profile_info->ingress_fields.protocol_trap_profiles;
    in_lif_profile_template_data.oam_default_profile = in_lif_profile_info->ingress_fields.oam_default_profile;
    in_lif_profile_template_data.in_lif_lb_profile = in_lif_lb_profile;
    if (dnx_data_lif.in_lif.feature_get(unit, dnx_data_lif_in_lif_use_mapped_in_lif_profile))
    {
        in_lif_profile_template_data.in_lif_orientation = in_lif_profile_info->egress_fields.in_lif_orientation;
        in_lif_profile_template_data.ive_outer_qos_dp_profile =
            in_lif_profile_info->egress_fields.ive_outer_qos_dp_profile;
        in_lif_profile_template_data.ive_inner_qos_dp_profile =
            in_lif_profile_info->egress_fields.ive_inner_qos_dp_profile;
        in_lif_profile_template_data.lif_same_interface_mode =
            in_lif_profile_info->egress_fields.lif_same_interface_mode;
    }
#ifdef BCM_DNX2_SUPPORT
    if (dnx_data_dev_init.general.feature_get(unit, dnx_data_dev_init_general_application_v1))
    {
        in_lif_profile_template_data.mact_mode = DBAL_ENUM_FVAL_VSI_PROFILE_BRIDGE_SVL;
    }
#endif

    if (intf_type == ETH_RIF)
    {
        in_lif_profile_template_data.mact_mode = in_lif_profile_info->ingress_fields.mact_mode;
        in_lif_profile_template_data.l2_v4_mc_fwd_type = in_lif_profile_info->ingress_fields.l2_v4_mc_fwd_type;
        in_lif_profile_template_data.l2_v6_mc_fwd_type = in_lif_profile_info->ingress_fields.l2_v6_mc_fwd_type;
    }

    /*
     * handle the in_lif template (outer template)
     */
    if (intf_type == ETH_RIF)
    {
        /** Based on the input combination of egress properties decide the egress eth rif profile bits */
        SHR_IF_ERR_EXIT(dnx_eth_rif_profile_algo_convert_to_egress_profile
                        (unit, in_lif_profile_info->egress_fields, &egress_eth_rif_profile));

        /**Decide the eth_rif_profile and the inlif_profile used bits masks based on soc properties and mode*/
        SHR_IF_ERR_EXIT(dnx_eth_rif_profile_used_bits_mask_get(unit, &eth_rif_profile_used_bits_mask,
                                                               &egress_reserved_nof_bits,
                                                               &eth_rif_profile_egress_reserved_start_bit));
        SHR_IF_ERR_EXIT(algo_lif_profile_db.eth_rif_profile.exchange
                        (unit, 0, &in_lif_profile_template_data,
                         old_in_lif_profile & eth_rif_profile_used_bits_mask, NULL, new_in_lif_profile, &is_first,
                         &lif_profile_is_last));
    }
    else if (intf_type == LIF)
    {
        /** Based on the input combination of egress properties decide the egress profile bits */
        SHR_IF_ERR_EXIT(dnx_in_lif_profile_algo_convert_to_egress_profile
                        (unit, in_lif_profile_info->egress_fields, old_in_lif_profile, &egress_in_lif_profile));
        /**Decide the eth_rif_profile and the inlif_profile used bits masks based on soc properties and mode*/
        SHR_IF_ERR_EXIT(dnx_in_lif_profile_used_bits_mask_get
                        (unit, &in_lif_profile_used_bits_mask, &egress_reserved_nof_bits,
                         &in_lif_profile_egress_reserved_start_bit));
        SHR_IF_ERR_EXIT(algo_lif_profile_db.in_lif_profile.exchange
                        (unit, 0, &in_lif_profile_template_data,
                         old_in_lif_profile & in_lif_profile_used_bits_mask, NULL, new_in_lif_profile, &is_first,
                         &lif_profile_is_last));
    }
    last_profile->old_in_lif_profile = lif_profile_is_last ? old_in_lif_profile : DNX_IN_LIF_PROFILE_DEFAULT;

    /*
     * decide the pmf and egress reserved bits based on soc properties configuration
     */
    if (intf_type == ETH_RIF)
    {
        pmf_reserved_nof_bits = ETH_RIF_PROFILE_PMF_RESERVED_NOF_BITS;
        pmf_reserved_start_bit = ETH_RIF_PROFILE_PMF_RESERVED_START_BIT;
        pmf_bits_mask = (((1 << pmf_reserved_nof_bits) - 1) << pmf_reserved_start_bit);
        pmf_in_lif_profile = old_in_lif_profile & pmf_bits_mask;
    }
    else
    {
        pmf_reserved_nof_bits = IN_LIF_PROFILE_PMF_RESERVED_NOF_BITS;
        pmf_reserved_start_bit = IN_LIF_PROFILE_PMF_RESERVED_START_BIT;
        pmf_bits_mask = (((1 << pmf_reserved_nof_bits) - 1) << pmf_reserved_start_bit);
        pmf_in_lif_profile = old_in_lif_profile & pmf_bits_mask;
    }

    /*
     * write ingress part: if we have is_first = 1, then ingress attributes were changed, need to write to ingress HW
     */
    if (is_first)
    {
        /*
         * when we add a new LIF profile, we should duplicate the profile in all the indexes that can be accessed
         * with the original in_lif_profile/eth_rif_profile and the reserved bits of PMF/Egress, we duplicate according to intf_type and nof reserved bits.
         */
        SHR_IF_ERR_EXIT(DBAL_HANDLE_ALLOC(unit, DBAL_TABLE_IN_LIF_PROFILE_TABLE, &entry_handle_id));

        dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_RPF_MODE, INST_SINGLE, rpf_mode);
        dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_CS_IN_LIF_VTT_TO_VTT_PROFILE,
                                     INST_SINGLE, cs_vtt_in_lif_profile_value);
        if (dnx_data_lif.in_lif.feature_get(unit, dnx_data_lif_in_lif_cs_in_lif_profile_based_on_stages))
        {
            dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_CS_IN_LIF_VTT_TO_FLP_PROFILE,
                                         INST_SINGLE, cs_vtt_to_flp_in_lif_profile_value);
        }
        dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_CS_IN_LIF_FLP_TO_FLP_PROFILE,
                                     INST_SINGLE, cs_in_lif_flp_profile_value);
        if ((in_lif_profile_info->ingress_fields.l3_ingress_flags & BCM_L3_INGRESS_IPMC_BRIDGE_FALLBACK)
            && dnx_data_lif.in_lif.mc_bridge_fallback_per_lif_profile_get(unit))
        {
            dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_MC_BRIDGE_FALLBACK_ALLOW, INST_SINGLE, 1);
        }
        if (dnx_data_trap.ingress.protocol_traps_init_mode_get(unit) == DNX_RX_TRAP_PROTOCOL_MODE_IN_LIF)
        {
            dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_L_2_CP_TRAP_HANDLING_PROFILE,
                                         INST_SINGLE,
                                         in_lif_profile_info->ingress_fields.protocol_trap_profiles.l2cp_trap_profile);
            dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_ICMPV4_TRAP_HANDLING_PROFILE,
                                         INST_SINGLE,
                                         in_lif_profile_info->ingress_fields.
                                         protocol_trap_profiles.icmpv4_trap_profile);
            dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_ICMPV6_TRAP_HANDLING_PROFILE, INST_SINGLE,
                                         in_lif_profile_info->ingress_fields.
                                         protocol_trap_profiles.icmpv6_trap_profile);
            dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_ARP_TRAP_HANDLING_PROFILE, INST_SINGLE,
                                         in_lif_profile_info->ingress_fields.protocol_trap_profiles.arp_trap_profile);
            dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_IGMP_TRAP_HANDLING_PROFILE, INST_SINGLE,
                                         in_lif_profile_info->ingress_fields.protocol_trap_profiles.igmp_trap_profile);
            dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_DHCP_TRAP_HANDLING_PROFILE, INST_SINGLE,
                                         in_lif_profile_info->ingress_fields.protocol_trap_profiles.dhcp_trap_profile);
            dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_NON_AUTH_8021X_TRAP_HANDLING_PROFILE,
                                         INST_SINGLE,
                                         in_lif_profile_info->ingress_fields.
                                         protocol_trap_profiles.non_auth_8021x_trap_profile);
        }

        dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_DA_NOT_FOUND_DESTINATION_PROFILE,
                                     INST_SINGLE, da_not_found_dst_profile);
        dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_OAM_DEFAULT_PROFILE,
                                     INST_SINGLE, in_lif_profile_info->ingress_fields.oam_default_profile);
        if (dnx_data_switch.load_balancing.hashing_selection_per_layer_get(unit))
        {
            dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_IN_LIF_LB_PROFILE, INST_SINGLE,
                                         in_lif_lb_profile);
        }

        if (intf_type == ETH_RIF)
        {
            if (dnx_data_lif.in_lif.feature_get(unit, dnx_data_lif_in_lif_use_mapped_in_lif_profile))
            {
                dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_MAPPED_IN_LIF_PROFILE, INST_SINGLE,
                                             (egress_eth_rif_profile << eth_rif_profile_egress_reserved_start_bit));
            }
            max_counter_value = 1 << (pmf_reserved_nof_bits + egress_reserved_nof_bits);
        }
        else
        {
            if (dnx_data_lif.in_lif.feature_get(unit, dnx_data_lif_in_lif_use_mapped_in_lif_profile))
            {
                /**We need to write to the mapped in lif profile field, the lif profile that devices without this property would have received*/
                egress_properties_nof_bits =
                    dnx_data_lif.in_lif.nof_in_lif_profile_allocate_orientation_bits_get(unit) +
                    dnx_data_lif.in_lif.nof_in_lif_profile_allocate_policer_inner_dp_bits_get(unit) +
                    dnx_data_lif.in_lif.nof_in_lif_profile_allocate_policer_outer_dp_bits_get(unit) +
                    dnx_data_lif.in_lif.nof_in_lif_profile_allocate_same_interface_mode_bits_get(unit);
                dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_MAPPED_IN_LIF_PROFILE, INST_SINGLE,
                                             (egress_in_lif_profile <<
                                              (IN_LIF_PROFILE_PMF_RESERVED_START_BIT - egress_properties_nof_bits)));
            }
            /**Loop over the PMF and native idx mode bits, when mapped_in_lif_profile is used, egress_reserved_nof_bits = 0*/
            max_counter_value = 1 << (pmf_reserved_nof_bits + egress_reserved_nof_bits + NATIVE_INDEXED_MODE_NOF_BITS);
        }
        /*
         * loop over the possible values of the reserved bits, and duplicate HW value to all the possible keys
         * the key is built by shifting the reserved bits value left and append new_in_lif_profile
         */
        for (hw_write_counter = 0; hw_write_counter < max_counter_value; hw_write_counter++)
        {
            if (intf_type == ETH_RIF)
            {
                dbal_entry_key_field32_set(unit, entry_handle_id, DBAL_FIELD_IN_LIF_PROFILE,
                                           CALCULATE_IN_LIF_PROFILE_FROM_ETH_RIF_PROFILE
                                           (IN_LIF_PROFILE_TABLE_MSB_PREFIX,
                                            (*new_in_lif_profile |
                                             (hw_write_counter <<
                                              (NOF_ETH_RIF_PROFILE_BITS -
                                               (pmf_reserved_nof_bits + egress_reserved_nof_bits))))));
            }
            else
            {
                dbal_entry_key_field32_set(unit, entry_handle_id, DBAL_FIELD_IN_LIF_PROFILE,
                                           *new_in_lif_profile | (hw_write_counter <<
                                                                  (NOF_IN_LIF_PROFILE_BITS -
                                                                   (pmf_reserved_nof_bits +
                                                                    egress_reserved_nof_bits +
                                                                    NATIVE_INDEXED_MODE_NOF_BITS))));
            }

            SHR_IF_ERR_EXIT(dbal_entry_commit(unit, entry_handle_id, DBAL_COMMIT));
        }

        SHR_IF_ERR_EXIT(DBAL_HANDLE_ALLOC(unit, DBAL_TABLE_VSI_PROFILE_TABLE, &entry_handle_id));
        dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_ROUTING_ENABLERS_PROFILE, INST_SINGLE,
                                     routing_enablers_profile);
        dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_VSI_PROFILE_BRIDGE, INST_SINGLE,
                                     in_lif_profile_info->ingress_fields.mact_mode);
        dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_L2_V4_MC_FWD_TYPE, INST_SINGLE,
                                     in_lif_profile_info->ingress_fields.l2_v4_mc_fwd_type);
        dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_L2_V6_MC_FWD_TYPE, INST_SINGLE,
                                     in_lif_profile_info->ingress_fields.l2_v6_mc_fwd_type);

        /*
         * loop over the possible values of the reserved bits, and duplicate HW value to all the possible keys
         * the key is built by shifting the reserved bits value left and append new_in_lif_profile
         */
        for (hw_write_counter = 0; hw_write_counter < max_counter_value; hw_write_counter++)
        {
            if (intf_type == ETH_RIF)
            {
                dbal_entry_key_field32_set(unit, entry_handle_id, DBAL_FIELD_VSI_PROFILE,
                                           CALCULATE_VSI_PROFILE_FROM_ETH_RIF_PROFILE(VSI_MSB_PREFIX,
                                                                                      (*new_in_lif_profile |
                                                                                       (hw_write_counter <<
                                                                                        (NOF_ETH_RIF_PROFILE_BITS -
                                                                                         (pmf_reserved_nof_bits +
                                                                                          egress_reserved_nof_bits))))));
            }
            else
            {
                dbal_entry_key_field32_set(unit, entry_handle_id, DBAL_FIELD_VSI_PROFILE,
                                           *new_in_lif_profile | (hw_write_counter <<
                                                                  (NOF_IN_LIF_PROFILE_BITS -
                                                                   (pmf_reserved_nof_bits +
                                                                    egress_reserved_nof_bits))));
            }
            SHR_IF_ERR_EXIT(dbal_entry_commit(unit, entry_handle_id, DBAL_COMMIT));
        }
    }

    /*
     * append the egress in_lif_profile bits to the output
     */
    if (intf_type == ETH_RIF)
    {
        *new_in_lif_profile =
            pmf_in_lif_profile | (egress_eth_rif_profile << eth_rif_profile_egress_reserved_start_bit) |
            *new_in_lif_profile;
    }
    else
    {
        /**no need to include egress in lif profile in devices with remapped lif profile in JR2 system headers mode*/
        if (dnx_data_lif.in_lif.feature_get(unit, dnx_data_lif_in_lif_use_mapped_in_lif_profile))
        {
            egress_in_lif_profile = 0;
        }
        /** Based on the native indexed mode parameter decide the indexed mode bit */
        SHR_IF_ERR_EXIT(dnx_in_lif_profile_convert_to_native_indexed_profile
                        (unit, in_lif_profile_info->native_indexed_mode, &native_indexed_mode_in_lif_profile));
        *new_in_lif_profile =
            pmf_in_lif_profile | (egress_in_lif_profile << in_lif_profile_egress_reserved_start_bit) |
            (native_indexed_mode_in_lif_profile << IN_LIF_PROFILE_NATIVE_INDEXED_MODE_START_BIT) | *new_in_lif_profile;
    }
exit:
    DBAL_FUNC_FREE_VARS;
    SHR_FUNC_EXIT;
}

/*
 * \brief adds ingress flag to flags if the corresponding bit is on in the routing enablers vector
 * routing_enablers_vector - the input routing enablers vector
 * flags - the output ingress flags
 * ingress_flag - the input ingress flag to check, if the corresponding bit in the routing enablers vector is on then we add this flag to flags
 * enabler_offset - the offset of the corresponding enabler in the routing enablers vector
 * action - the action to do, disable/enable the protocol in the vector. See 'in_lif_profile_info'
 */
static shr_error_e
dnx_in_lif_profile_routing_enablers_restore_flag(
    int unit,
    uint32 *routing_enablers_vector,
    uint32 *flags,
    int ingress_flag,
    int enabler_offset,
    uint32 layer_type,
    in_lif_routing_vector_action_e action)
{
    SHR_FUNC_INIT_VARS(unit);

    SHR_NULL_CHECK(routing_enablers_vector, _SHR_E_PARAM, "NULL input - routing_enablers_vector");

    /*
     * check if the flag is set by the user
     */
    if (dnx_data_l3.routing_enablers.routing_enable_action_support_get(unit))
    {
        uint32 configured_profile;
        SHR_IF_ERR_EXIT(utilex_bitstream_get_field(routing_enablers_vector, enabler_offset,
                                                   dnx_data_l3.
                                                   routing_enablers.routing_enable_action_profile_size_get(unit),
                                                   &configured_profile));
        if (((action == ROUTING_VECTOR_ACTION_DISABLE)
             && (configured_profile ==
                 dnx_data_l3.routing_enablers.layer_enablers_get(unit, layer_type)->disable_profile))
            || ((action == ROUTING_VECTOR_ACTION_ENABLE)
                && (configured_profile == dnx_data_l3.routing_enablers.routing_enabled_action_profile_id_get(unit))))
        {
            *flags |= ingress_flag;
        }
    }
    else
    {
        if (((action == ROUTING_VECTOR_ACTION_DISABLE)
             && (!utilex_bitstream_test_bit(routing_enablers_vector, enabler_offset)))
            || ((action == ROUTING_VECTOR_ACTION_ENABLE)
                && (utilex_bitstream_test_bit(routing_enablers_vector, enabler_offset))))
        {
            *flags |= ingress_flag;
        }
    }
exit:
    SHR_FUNC_EXIT;
}

/**
 * \see
 *   in_lif_profile.h file
 */
shr_error_e
in_lif_profile_info_t_init(
    int unit,
    in_lif_profile_info_t * in_lif_profile_info)
{
    uint8 ipv4_fwd_type_sel;
    uint8 ipv6_fwd_type_sel;
    SHR_FUNC_INIT_VARS(unit);
    /*
     * init ingress in_lif_profile fields
     */
    in_lif_profile_info->ingress_fields.l3_ingress_flags = 0;
    in_lif_profile_info->ingress_fields.pwe_nof_eth_tags = ZERO_TAGS;
    in_lif_profile_info->ingress_fields.urpf_mode = bcmL3IngressUrpfDisable;
    sal_memset(&(in_lif_profile_info->ingress_fields.protocol_trap_profiles), 0,
               sizeof(in_lif_profile_info->ingress_fields.protocol_trap_profiles));
    in_lif_profile_info->ingress_fields.cs_in_lif_profile_flags = 0;
#ifdef BCM_DNX2_SUPPORT
    if (dnx_data_dev_init.general.feature_get(unit, dnx_data_dev_init_general_application_v1))
    {
        in_lif_profile_info->ingress_fields.mact_mode = DBAL_ENUM_FVAL_VSI_PROFILE_BRIDGE_SVL;
    }
#endif
    in_lif_profile_info->ingress_fields.oam_default_profile = DEFAULT_OAM_PROFILE;
    in_lif_profile_info->ingress_fields.in_lif_lb_profile = DEFAULT_IN_LIF_LB_PROFILE;
    SHR_IF_ERR_EXIT(switch_db.l3mcastl2_ipv4_fwd_type.get(unit, &ipv4_fwd_type_sel));
    SHR_IF_ERR_EXIT(switch_db.l3mcastl2_ipv6_fwd_type.get(unit, &ipv6_fwd_type_sel));
    in_lif_profile_info->ingress_fields.l2_v4_mc_fwd_type = ipv4_fwd_type_sel;
    in_lif_profile_info->ingress_fields.l2_v6_mc_fwd_type = ipv6_fwd_type_sel;
    in_lif_profile_info->ingress_fields.lb_per_lif_flags = 0;
    in_lif_profile_info->ingress_fields.allow_udp_tcp_above_termination = 0;
    in_lif_profile_info->ingress_fields.lif_gport = 0;

    /** Init default_frwrd_dst as add offset(0) to vsi.default-destination*/
    sal_memset(in_lif_profile_info->ingress_fields.default_frwrd_dst, 0,
               sizeof(in_lif_profile_info->ingress_fields.default_frwrd_dst));
    in_lif_profile_info->ingress_fields.default_frwrd_dst[DBAL_ENUM_FVAL_DESTINATION_DISTRIBUTION_TYPE_MC].add_vsi_dst =
        TRUE;
    in_lif_profile_info->ingress_fields.default_frwrd_dst[DBAL_ENUM_FVAL_DESTINATION_DISTRIBUTION_TYPE_BC].add_vsi_dst =
        TRUE;
    in_lif_profile_info->ingress_fields.default_frwrd_dst[DBAL_ENUM_FVAL_DESTINATION_DISTRIBUTION_TYPE_UC].add_vsi_dst =
        TRUE;

    in_lif_profile_info->native_indexed_mode = DEFAULT_NATIVE_INDEXED_MODE;
    /*
     * init egress in_lif_profile fields
     */
    in_lif_profile_info->egress_fields.in_lif_orientation = DEFAULT_IN_LIF_ORIENTATION;
    in_lif_profile_info->egress_fields.ive_inner_qos_dp_profile = 0;
    in_lif_profile_info->egress_fields.ive_outer_qos_dp_profile = 0;
    in_lif_profile_info->egress_fields.lif_same_interface_mode = DEVICE_SCOPE;
    in_lif_profile_info->egress_fields.preserve_dscp_mode = DISABLE_PRESERVE_DSCP_MODE;
    in_lif_profile_info->egress_fields.in_lif_profile_jr_mode = 0;

    if (dnx_data_lif.in_lif.feature_get(unit, dnx_data_lif_in_lif_cs_in_lif_profile_based_on_stages))
    {
    }
exit:
    SHR_FUNC_EXIT;
}

/**
 * \see
 *   in_lif_profile.h file
 */
shr_error_e
dnx_in_lif_profile_get_data(
    int unit,
    int in_lif_profile,
    in_lif_profile_info_t * in_lif_profile_info,
    dbal_tables_e inlif_dbal_table_id,
    in_lif_intf_type_e intf_type)
{
    in_lif_profile_info_egress_t egress_in_lif_profile_info;
    in_lif_template_data_t in_lif_profile_template_data;
    uint32 enablers_vector_template_data[DNX_DATA_MAX_L3_ROUTING_ENABLERS_ROUTING_ENABLE_VECTOR_LENGTH /
                                         SAL_UINT32_NOF_BITS], lif_lb_profile_disablers_flags_vector;
    uint8 native_indexed_mode;
    int enablers_vector_ref_count, ref_count, in_lif_profile_jr_mode,
        in_lif_profile_exclude_orientation, in_lif_orientation, lif_lb_profile_ref_count;
    uint32 flags, lif_lb_profile_flags;
    uint8 lif_lb_profile;
    dnx_default_frwrd_dst_t default_frwrd_dst[DBAL_NOF_ENUM_DESTINATION_DISTRIBUTION_TYPE_VALUES];

    SHR_FUNC_INIT_VARS(unit);
    DBAL_FUNC_INIT_VARS(unit);

    flags = 0;
    lif_lb_profile = 0;
    lif_lb_profile_flags = 0;
    in_lif_profile_jr_mode = 0;
    in_lif_profile_exclude_orientation = 0;
    in_lif_orientation = 0;
    lif_lb_profile_ref_count = 0;
    lif_lb_profile_disablers_flags_vector = DNX_IN_LIF_LB_PROFILE_DISABLERS_FLAGS_DEFAULT;
    SHR_NULL_CHECK(in_lif_profile_info, _SHR_E_PARAM, "NULL input - in_lif_profile_info");
    if (dnx_data_lif.in_lif.feature_get(unit, dnx_data_lif_in_lif_cs_in_lif_profile_based_on_stages))
    {
    }
    /*
     * Get in_lif_profile template data
     */
    SHR_IF_ERR_EXIT(dnx_in_lif_profile_template_data_get
                    (unit, in_lif_profile, intf_type, &in_lif_profile_template_data));

    /*
     * assign RPF mode
     */
    switch (in_lif_profile_template_data.rpf_mode)
    {
        case DBAL_ENUM_FVAL_RPF_MODE_LOOSE:
            in_lif_profile_info->ingress_fields.urpf_mode = bcmL3IngressUrpfLoose;
            break;
        case DBAL_ENUM_FVAL_RPF_MODE_STRICT:
            in_lif_profile_info->ingress_fields.urpf_mode = bcmL3IngressUrpfStrict;
            break;
        case DBAL_ENUM_FVAL_RPF_MODE_DISABLE:
            in_lif_profile_info->ingress_fields.urpf_mode = bcmL3IngressUrpfDisable;
            break;
        default:
            SHR_ERR_EXIT(_SHR_E_PARAM, "urpf_mode invalid : %d\n", in_lif_profile_template_data.rpf_mode);
            break;
    }

    /*
     * get routing enablers vector
     */
    SHR_IF_ERR_EXIT(algo_lif_profile_db.in_lif_profile_l3_enablers.profile_data_get
                    (unit, in_lif_profile_template_data.routing_enablers_profile,
                     &enablers_vector_ref_count, enablers_vector_template_data));

    if (dnx_data_switch.load_balancing.hashing_selection_per_layer_get(unit))
    {
        SHR_IF_ERR_EXIT(algo_lif_profile_db.in_lif_lb_profile_disablers_flags.profile_data_get
                        (unit, in_lif_profile_template_data.in_lif_lb_profile,
                         &lif_lb_profile_ref_count, &lif_lb_profile_disablers_flags_vector));
    }

    /*
     * assign l3_ingress_flags
     */
    if (dnx_data_lif.in_lif.feature_get(unit, dnx_data_lif_in_lif_cs_in_lif_profile_based_on_stages))
    {
    }
    else
    {
        SHR_IF_ERR_EXIT(dnx_in_lif_profile_translate_to_l3_ingress_flags
                        (unit, in_lif_profile_template_data, in_lif_profile_info, inlif_dbal_table_id));
        /*
         * assign nof ethernet tags
         */
        if (inlif_dbal_table_id == DBAL_TABLE_IN_LIF_FORMAT_PWE)
        {
            uint32 cs_in_lif_profile;
            dbal_enum_value_field_nof_sd_tags_e pwe_nof_tags = 0;

            cs_in_lif_profile = in_lif_profile_template_data.cs_vtt_in_lif_profile;
            SHR_IF_ERR_EXIT(dbal_fields_struct_field_decode
                            (unit, DBAL_FIELD_PWE_INLIF_CS_PROFILE, DBAL_FIELD_NOF_SD_TAGS, &pwe_nof_tags,
                             &cs_in_lif_profile));
            /*
             * LSB indicates LIF/NWK scope and bits 2 and 3 LSB indicate the nof_tags
             */
            in_lif_profile_info->ingress_fields.pwe_nof_eth_tags = pwe_nof_tags;
        }
    }

    /** Restore routing enablers flags */
    dnx_in_lif_profile_routing_enablers_restore_flag(unit, enablers_vector_template_data, &flags,
                                                     BCM_L3_INGRESS_ROUTE_ENABLE_UNKNOWN,
                                                     DNX_IN_LIF_ROUTING_UC_ENABLER_OFFSET_GET(unit,
                                                                                              DBAL_ENUM_FVAL_LAYER_TYPES_UNKNOWN),
                                                     DBAL_ENUM_FVAL_LAYER_TYPES_UNKNOWN, ROUTING_VECTOR_ACTION_ENABLE);
    dnx_in_lif_profile_routing_enablers_restore_flag(unit, enablers_vector_template_data, &flags,
                                                     BCM_L3_INGRESS_ROUTE_ENABLE_UNKNOWN,
                                                     DNX_IN_LIF_ROUTING_MC_ENABLER_OFFSET_GET(unit,
                                                                                              DBAL_ENUM_FVAL_LAYER_TYPES_UNKNOWN),
                                                     DBAL_ENUM_FVAL_LAYER_TYPES_UNKNOWN, ROUTING_VECTOR_ACTION_ENABLE);
    dnx_in_lif_profile_routing_enablers_restore_flag(unit, enablers_vector_template_data, &flags,
                                                     BCM_L3_INGRESS_ROUTE_DISABLE_IP4_UCAST,
                                                     DNX_IN_LIF_ROUTING_UC_ENABLER_OFFSET_GET(unit,
                                                                                              DBAL_ENUM_FVAL_LAYER_TYPES_IPV4),
                                                     DBAL_ENUM_FVAL_LAYER_TYPES_IPV4, ROUTING_VECTOR_ACTION_DISABLE);
    dnx_in_lif_profile_routing_enablers_restore_flag(unit, enablers_vector_template_data, &flags,
                                                     BCM_L3_INGRESS_ROUTE_DISABLE_IP4_MCAST,
                                                     DNX_IN_LIF_ROUTING_MC_ENABLER_OFFSET_GET(unit,
                                                                                              DBAL_ENUM_FVAL_LAYER_TYPES_IPV4),
                                                     DBAL_ENUM_FVAL_LAYER_TYPES_IPV4, ROUTING_VECTOR_ACTION_DISABLE);
    dnx_in_lif_profile_routing_enablers_restore_flag(unit, enablers_vector_template_data, &flags,
                                                     BCM_L3_INGRESS_ROUTE_DISABLE_IP6_UCAST,
                                                     DNX_IN_LIF_ROUTING_UC_ENABLER_OFFSET_GET(unit,
                                                                                              DBAL_ENUM_FVAL_LAYER_TYPES_IPV6),
                                                     DBAL_ENUM_FVAL_LAYER_TYPES_IPV6, ROUTING_VECTOR_ACTION_DISABLE);
    dnx_in_lif_profile_routing_enablers_restore_flag(unit, enablers_vector_template_data, &flags,
                                                     BCM_L3_INGRESS_ROUTE_DISABLE_IP6_MCAST,
                                                     DNX_IN_LIF_ROUTING_MC_ENABLER_OFFSET_GET(unit,
                                                                                              DBAL_ENUM_FVAL_LAYER_TYPES_IPV6),
                                                     DBAL_ENUM_FVAL_LAYER_TYPES_IPV6, ROUTING_VECTOR_ACTION_DISABLE);
    dnx_in_lif_profile_routing_enablers_restore_flag(unit, enablers_vector_template_data, &flags,
                                                     BCM_L3_INGRESS_ROUTE_DISABLE_MPLS,
                                                     DNX_IN_LIF_ROUTING_UC_ENABLER_OFFSET_GET(unit,
                                                                                              DBAL_ENUM_FVAL_LAYER_TYPES_MPLS),
                                                     DBAL_ENUM_FVAL_LAYER_TYPES_MPLS, ROUTING_VECTOR_ACTION_DISABLE);

    dnx_in_lif_profile_routing_enablers_restore_flag(unit, enablers_vector_template_data, &flags,
                                                     BCM_L3_INGRESS_TUNNEL_DISABLE_NATIVE_ETH,
                                                     DNX_IN_LIF_ROUTING_UC_ENABLER_OFFSET_GET(unit,
                                                                                              DBAL_ENUM_FVAL_LAYER_TYPES_ETHERNET),
                                                     DBAL_ENUM_FVAL_LAYER_TYPES_ETHERNET,
                                                     ROUTING_VECTOR_ACTION_DISABLE);
    dnx_in_lif_profile_routing_enablers_restore_flag(unit, enablers_vector_template_data, &flags,
                                                     BCM_L3_INGRESS_TUNNEL_DISABLE_NATIVE_ETH,
                                                     DNX_IN_LIF_ROUTING_MC_ENABLER_OFFSET_GET(unit,
                                                                                              DBAL_ENUM_FVAL_LAYER_TYPES_ETHERNET),
                                                     DBAL_ENUM_FVAL_LAYER_TYPES_ETHERNET,
                                                     ROUTING_VECTOR_ACTION_DISABLE);
    /*
     * UDP TCP above termination restore
     * Check UDP UC flag only, assuming the four flags (UDP/TCP UC/MC are set together.
     */
    {
        int offset_to_check = DNX_IN_LIF_ROUTING_UC_ENABLER_OFFSET_GET(unit, DBAL_ENUM_FVAL_LAYER_TYPES_UDP);
        if (utilex_bitstream_test_bit(enablers_vector_template_data, offset_to_check))
        {
            in_lif_profile_info->ingress_fields.allow_udp_tcp_above_termination = TRUE;
        }
    }

    /** Restore inlif lb profile disablers flags */
    lif_lb_profile_flags = lif_lb_profile_disablers_flags_vector;

    in_lif_profile_info->ingress_fields.l3_ingress_flags |= flags;
    in_lif_profile_info->ingress_fields.lb_per_lif_flags |= lif_lb_profile_flags;
    in_lif_profile_info->ingress_fields.in_lif_lb_profile |= lif_lb_profile;
    in_lif_profile_info->ingress_fields.oam_default_profile = in_lif_profile_template_data.oam_default_profile;

    /*
     * assign trap profiling
     */
    in_lif_profile_info->ingress_fields.protocol_trap_profiles = in_lif_profile_template_data.protocol_trap_profiles;
    /*
     * assign mact mode
     */
    in_lif_profile_info->ingress_fields.mact_mode = in_lif_profile_template_data.mact_mode;
    /*
     * assign l2 mc forward type
     */
    in_lif_profile_info->ingress_fields.l2_v4_mc_fwd_type = in_lif_profile_template_data.l2_v4_mc_fwd_type;
    in_lif_profile_info->ingress_fields.l2_v6_mc_fwd_type = in_lif_profile_template_data.l2_v6_mc_fwd_type;
    /*
     * assign egress in_lif_profile fields
     */
    if (dnx_data_lif.in_lif.feature_get(unit, dnx_data_lif_in_lif_use_mapped_in_lif_profile))
    {
        in_lif_profile_info->egress_fields.in_lif_orientation = in_lif_profile_template_data.in_lif_orientation;
        in_lif_profile_info->egress_fields.ive_outer_qos_dp_profile =
            in_lif_profile_template_data.ive_outer_qos_dp_profile;
        in_lif_profile_info->egress_fields.ive_inner_qos_dp_profile =
            in_lif_profile_template_data.ive_inner_qos_dp_profile;
        in_lif_profile_info->egress_fields.lif_same_interface_mode =
            in_lif_profile_template_data.lif_same_interface_mode;
    }
    else
    {
        SHR_IF_ERR_EXIT(dnx_in_lif_profile_algo_extract_egress_profile
                        (unit, in_lif_profile, &egress_in_lif_profile_info, &in_lif_profile_jr_mode,
                         &in_lif_profile_exclude_orientation, &in_lif_orientation, intf_type));
        in_lif_profile_info->egress_fields.in_lif_orientation = egress_in_lif_profile_info.in_lif_orientation;
        in_lif_profile_info->egress_fields.ive_outer_qos_dp_profile =
            egress_in_lif_profile_info.ive_outer_qos_dp_profile;
        in_lif_profile_info->egress_fields.ive_inner_qos_dp_profile =
            egress_in_lif_profile_info.ive_inner_qos_dp_profile;
        in_lif_profile_info->egress_fields.lif_same_interface_mode = egress_in_lif_profile_info.lif_same_interface_mode;
        in_lif_profile_info->egress_fields.preserve_dscp_mode = egress_in_lif_profile_info.preserve_dscp_mode;
        in_lif_profile_info->egress_fields.in_lif_profile_jr_mode = egress_in_lif_profile_info.in_lif_profile_jr_mode;
    }
    /*
     * if the intf type is ETH RIF, check in IOP mode whether we have configured preserve DSCP mode
     */
    if ((dnx_data_headers.system_headers.system_headers_mode_get(unit) ==
         DBAL_ENUM_FVAL_SYSTEM_HEADERS_MODE_JERICHO_MODE)
        && (dnx_data_qos.qos.routing_dscp_preserve_mode_get(unit) == 1) && (intf_type == ETH_RIF))
    {
        SHR_IF_ERR_EXIT(dnx_eth_rif_profile_algo_extract_egress_profile
                        (unit, in_lif_profile, &egress_in_lif_profile_info,
                         &in_lif_profile_jr_mode, &in_lif_profile_exclude_orientation, &in_lif_orientation));
        in_lif_profile_info->egress_fields.preserve_dscp_mode = egress_in_lif_profile_info.preserve_dscp_mode;
    }

    /*
     * assign indexed_mode based on the inlif profile
     */
    if (intf_type == LIF)
    {
        SHR_IF_ERR_EXIT(dnx_in_lif_profile_convert_to_native_indexed_mode(unit, in_lif_profile, &native_indexed_mode));
        in_lif_profile_info->native_indexed_mode = native_indexed_mode;
    }

    /*
     * assign in_lif default forward destinations
     */
    sal_memset(default_frwrd_dst, 0, sizeof(default_frwrd_dst));
    
    if (dnx_data_dev_init.general.feature_get(unit, dnx_data_dev_init_general_application_v1))
    {
        SHR_IF_ERR_EXIT(algo_lif_profile_db.in_lif_da_not_found_destination.profile_data_get
                        (unit, in_lif_profile_template_data.da_not_found_dst_profile, &ref_count, default_frwrd_dst));
        sal_memcpy(in_lif_profile_info->ingress_fields.default_frwrd_dst, default_frwrd_dst, sizeof(default_frwrd_dst));
    }

exit:
    DBAL_FUNC_FREE_VARS;
    SHR_FUNC_EXIT;
}

/**
 * \see
 *   in_lif_profile.h file
 */
shr_error_e
dnx_in_lif_profile_get_ref_count(
    int unit,
    int in_lif_profile,
    int *ref_count,
    in_lif_intf_type_e intf_type)
{
    int eth_rif_profile_used_bits_mask, egress_reserved_nof_bits, eth_rif_profile_egress_reserved_start_bit,
        in_lif_profile_used_bits_mask, in_lif_profile_egress_reserved_nof_bits,
        in_lif_profile_egress_reserved_start_bit;
    in_lif_template_data_t in_lif_profile_template_data;

    SHR_FUNC_INIT_VARS(unit);
    DBAL_FUNC_INIT_VARS(unit);

    /*
     * determine the template manager to look in according to intf_type
     */
    SHR_IF_ERR_EXIT(dnx_eth_rif_profile_used_bits_mask_get
                    (unit, &eth_rif_profile_used_bits_mask, &egress_reserved_nof_bits,
                     &eth_rif_profile_egress_reserved_start_bit));
    SHR_IF_ERR_EXIT(dnx_in_lif_profile_used_bits_mask_get
                    (unit, &in_lif_profile_used_bits_mask, &in_lif_profile_egress_reserved_nof_bits,
                     &in_lif_profile_egress_reserved_start_bit));
    if (intf_type == ETH_RIF)
    {
        in_lif_profile = in_lif_profile & eth_rif_profile_used_bits_mask;
        SHR_IF_ERR_EXIT(algo_lif_profile_db.eth_rif_profile.profile_data_get
                        (unit, in_lif_profile, ref_count, &in_lif_profile_template_data));
    }
    else if (intf_type == LIF)
    {
        in_lif_profile = in_lif_profile & in_lif_profile_used_bits_mask;
        SHR_IF_ERR_EXIT(algo_lif_profile_db.in_lif_profile.profile_data_get
                        (unit, in_lif_profile, ref_count, &in_lif_profile_template_data));
    }
    else
    {
        SHR_ERR_EXIT(_SHR_E_FAIL, "invalid intf_type\n");
    }

exit:
    DBAL_FUNC_FREE_VARS;
    SHR_FUNC_EXIT;
}

/**
 * \see
 *   in_lif_profile.h file
 */
shr_error_e
dnx_in_lif_vsi_profile_get(
    int unit,
    bcm_port_t gport,
    uint32 *intf_profile,
    uint32 *result_type,
    in_lif_intf_type_e intf_type)
{
    uint32 entry_handle_id;
    dnx_algo_gpm_gport_hw_resources_t gport_hw_resources;
    SHR_FUNC_INIT_VARS(unit);
    DBAL_FUNC_INIT_VARS(unit);
    if (intf_type == ETH_RIF)
    {
        /** RIF type*/
        SHR_IF_ERR_EXIT(DBAL_HANDLE_ALLOC(unit, dnx_data_l2.vsi.vsi_table_get(unit), &entry_handle_id));
        dbal_entry_key_field32_set(unit, entry_handle_id, DBAL_FIELD_VSI, gport);
        SHR_IF_ERR_EXIT(dbal_entry_get(unit, entry_handle_id, DBAL_GET_ALL_FIELDS));
        SHR_IF_ERR_EXIT(dbal_entry_handle_value_field32_get(unit, entry_handle_id, DBAL_FIELD_RESULT_TYPE, INST_SINGLE,
                                                            result_type));
        SHR_IF_ERR_EXIT(dbal_entry_handle_value_field32_get(unit, entry_handle_id, DBAL_FIELD_VSI_PROFILE, INST_SINGLE,
                                                            intf_profile));
        *intf_profile = CALCULATE_VSI_PROFILE_FROM_ETH_RIF_PROFILE(VSI_MSB_PREFIX, *intf_profile);
    }
    else if (intf_type == LIF)
    {
        /** LIF type */
        SHR_IF_ERR_REPLACE_AND_EXIT(dnx_algo_gpm_gport_to_hw_resources
                                    (unit, gport, DNX_ALGO_GPM_GPORT_HW_RESOURCES_LOCAL_LIF_INGRESS,
                                     &gport_hw_resources), _SHR_E_NOT_FOUND, _SHR_E_PARAM);
        *result_type = gport_hw_resources.inlif_dbal_result_type;
        SHR_IF_ERR_EXIT(DBAL_HANDLE_ALLOC(unit, gport_hw_resources.inlif_dbal_table_id, &entry_handle_id));
        dbal_entry_key_field32_set(unit, entry_handle_id, DBAL_FIELD_IN_LIF, gport_hw_resources.local_in_lif);
        if (dnx_data_lif.in_lif.feature_get(unit, dnx_data_lif_in_lif_phy_db_dpc))
        {
            dbal_entry_key_field32_set(unit, entry_handle_id, DBAL_FIELD_CORE_ID, DBAL_CORE_DEFAULT);
        }
        dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_RESULT_TYPE, INST_SINGLE,
                                     gport_hw_resources.inlif_dbal_result_type);
        SHR_IF_ERR_EXIT(dbal_entry_get(unit, entry_handle_id, DBAL_GET_ALL_FIELDS));
        SHR_IF_ERR_EXIT(dbal_entry_handle_value_field32_get
                        (unit, entry_handle_id, DBAL_FIELD_IN_LIF_PROFILE, INST_SINGLE, intf_profile));
    }
    else
    {
        /** Invalid value passed to the function */
        SHR_ERR_EXIT(_SHR_E_PARAM,
                     "Invalid interface type was provided to the function - expected either LIF or ETH_RIF\n");
    }
exit:
    DBAL_FUNC_FREE_VARS;
    SHR_FUNC_EXIT;
}

/**
 * \brief
 * The function is responsible for connecting the in_lif_profile to mapped_in_lif_profile, when we set pmf bits.
 * The mapped_in_lif_profile is set on the system headers, used by the ERPP
 */
static shr_error_e
dnx_mapped_inlif_profile_with_pmf_bits_set(
    int unit,
    uint32 intf_profile,
    in_lif_intf_type_e intf_type)
{
    uint32 entry_handle_id, mapped_in_lif_profile;
    int ingress_bits_mask, egress_reserved_nof_bits, egress_reserved_start_bit, pmf_bits = 0;

    SHR_FUNC_INIT_VARS(unit);
    DBAL_FUNC_INIT_VARS(unit);

    /** Extract only the pmf bits, which are set on the profile and decide some mask values, based on intf_type*/
    if (intf_type == ETH_RIF)
    {
        SHR_IF_ERR_EXIT(dnx_eth_rif_profile_used_bits_mask_get(unit, &ingress_bits_mask,
                                                               &egress_reserved_nof_bits, &egress_reserved_start_bit));
        pmf_bits =
            (intf_profile >> ETH_RIF_PROFILE_PMF_RESERVED_START_BIT) & ((1 << ETH_RIF_PROFILE_PMF_RESERVED_NOF_BITS) -
                                                                        1);
    }
    else if (intf_type == LIF)
    {
        SHR_IF_ERR_EXIT(dnx_in_lif_profile_used_bits_mask_get(unit, &ingress_bits_mask,
                                                              &egress_reserved_nof_bits, &egress_reserved_start_bit));
        pmf_bits =
            (intf_profile >> IN_LIF_PROFILE_PMF_RESERVED_START_BIT) & ((1 << IN_LIF_PROFILE_PMF_RESERVED_NOF_BITS) - 1);
    }

    /**From the ingress part of the lif_profile, take the mapped_in_lif_profile, which will also contain the mapped egress bits */
    SHR_IF_ERR_EXIT(DBAL_HANDLE_ALLOC(unit, DBAL_TABLE_IN_LIF_PROFILE_TABLE, &entry_handle_id));
    if (intf_type == ETH_RIF)
    {
        dbal_entry_key_field32_set(unit, entry_handle_id, DBAL_FIELD_IN_LIF_PROFILE,
                                   CALCULATE_IN_LIF_PROFILE_FROM_ETH_RIF_PROFILE
                                   (IN_LIF_PROFILE_TABLE_MSB_PREFIX, (intf_profile & ingress_bits_mask)));
    }
    else
    {
        dbal_entry_key_field32_set(unit, entry_handle_id, DBAL_FIELD_IN_LIF_PROFILE,
                                   (intf_profile & ingress_bits_mask));
    }
    SHR_IF_ERR_EXIT(dbal_entry_get(unit, entry_handle_id, DBAL_GET_ALL_FIELDS));
    SHR_IF_ERR_EXIT(dbal_entry_handle_value_field32_get
                    (unit, entry_handle_id, DBAL_FIELD_MAPPED_IN_LIF_PROFILE, INST_SINGLE, &mapped_in_lif_profile));

    /** Write the resulting mapped_in_lif_profile with the pmf bits, to the proper lif_profile, including the pmf bits as well*/
    if (intf_type == ETH_RIF)
    {
        dbal_entry_key_field32_set(unit, entry_handle_id, DBAL_FIELD_IN_LIF_PROFILE,
                                   CALCULATE_IN_LIF_PROFILE_FROM_ETH_RIF_PROFILE
                                   (IN_LIF_PROFILE_TABLE_MSB_PREFIX, intf_profile));
        dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_MAPPED_IN_LIF_PROFILE, INST_SINGLE,
                                     mapped_in_lif_profile | (pmf_bits << ETH_RIF_PROFILE_PMF_RESERVED_START_BIT));
    }
    else
    {
        dbal_entry_key_field32_set(unit, entry_handle_id, DBAL_FIELD_IN_LIF_PROFILE, intf_profile);
        dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_MAPPED_IN_LIF_PROFILE, INST_SINGLE,
                                     mapped_in_lif_profile | (pmf_bits << IN_LIF_PROFILE_PMF_RESERVED_START_BIT));
    }
    SHR_IF_ERR_EXIT(dbal_entry_commit(unit, entry_handle_id, DBAL_COMMIT));

exit:
    DBAL_FUNC_FREE_VARS;
    SHR_FUNC_EXIT;
}

/**
 * \see
 *   in_lif_profile.h file
 */
shr_error_e
dnx_in_lif_vsi_profile_set(
    int unit,
    bcm_port_t gport,
    uint32 intf_profile,
    uint32 result_type,
    in_lif_intf_type_e intf_type)
{
    uint32 entry_handle_id;
    SHR_FUNC_INIT_VARS(unit);
    DBAL_FUNC_INIT_VARS(unit);
    if (intf_type == ETH_RIF)
    {
        intf_profile = CALCULATE_VSI_PROFILE_FROM_ETH_RIF_PROFILE(VSI_MSB_PREFIX, intf_profile);

        SHR_IF_ERR_EXIT(DBAL_HANDLE_ALLOC(unit, dnx_data_l2.vsi.vsi_table_get(unit), &entry_handle_id));
        dbal_entry_key_field32_set(unit, entry_handle_id, DBAL_FIELD_VSI, gport);
        dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_RESULT_TYPE, INST_SINGLE, result_type);
        dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_VSI_PROFILE, INST_SINGLE, intf_profile);
        SHR_IF_ERR_EXIT(dbal_entry_commit(unit, entry_handle_id, DBAL_COMMIT));
    }
    else if (intf_type == LIF)
    {
        dnx_algo_gpm_gport_hw_resources_t gport_hw_resources;

        SHR_IF_ERR_REPLACE_AND_EXIT(dnx_algo_gpm_gport_to_hw_resources
                                    (unit, gport, DNX_ALGO_GPM_GPORT_HW_RESOURCES_LOCAL_LIF_INGRESS,
                                     &gport_hw_resources), _SHR_E_NOT_FOUND, _SHR_E_PARAM);

        SHR_IF_ERR_EXIT(DBAL_HANDLE_ALLOC(unit, gport_hw_resources.inlif_dbal_table_id, &entry_handle_id));

        dbal_entry_key_field32_set(unit, entry_handle_id, DBAL_FIELD_IN_LIF, gport_hw_resources.local_in_lif);
        if (dnx_data_lif.in_lif.feature_get(unit, dnx_data_lif_in_lif_phy_db_dpc))
        {
            dbal_entry_key_field32_set(unit, entry_handle_id, DBAL_FIELD_CORE_ID, _SHR_CORE_ALL);
        }
        dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_RESULT_TYPE, INST_SINGLE, result_type);
        dbal_entry_value_field32_set(unit, entry_handle_id, DBAL_FIELD_IN_LIF_PROFILE, INST_SINGLE, intf_profile);

        SHR_IF_ERR_EXIT(dbal_entry_commit(unit, entry_handle_id, DBAL_COMMIT));
    }
    else
    {
        /** Invalid value passed to the function */
        SHR_ERR_EXIT(_SHR_E_PARAM,
                     "Invalid interface type was provided to the function - expected either LIF or ETH_RIF\n");
    }

    /**If the device works with mapped inlif profile, make sure to configure it properly*/
    if (dnx_data_lif.in_lif.feature_get(unit, dnx_data_lif_in_lif_use_mapped_in_lif_profile))
    {
        SHR_IF_ERR_EXIT(dnx_mapped_inlif_profile_with_pmf_bits_set(unit, intf_profile, intf_type));
    }
exit:
    DBAL_FUNC_FREE_VARS;
    SHR_FUNC_EXIT;
}
