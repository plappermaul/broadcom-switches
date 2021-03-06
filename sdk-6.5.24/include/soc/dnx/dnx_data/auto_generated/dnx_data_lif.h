
/* *INDENT-OFF* */
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2021 Broadcom Inc. All rights reserved.
 */
#ifndef _DNX_DATA_LIF_H_

#define _DNX_DATA_LIF_H_

#include <sal/limits.h>
#include <soc/property.h>
#include <shared/shrextend/shrextend_debug.h>
#include <shared/utilex/utilex_framework.h>
#include <soc/dnxc/dnxc_data/dnxc_data_utils.h>
#include <soc/dnxc/dnxc_data/dnxc_data_mgmt.h>
#include <soc/dnx/dbal/dbal.h>
#include <soc/dnx/dnx_data/auto_generated/dnx_data_max_lif.h>



#ifndef BCM_DNX_SUPPORT
#error "This file is for use by DNX (JR2) family only!"
#endif



shr_error_e dnx_data_if_lif_init(
    int unit);







typedef enum
{
    
    dnx_data_lif_global_lif_prevent_tunnel_update_rif,
    dnx_data_lif_global_lif_glem_bypass_per_phase,
    dnx_data_lif_global_lif_outlif_profile_in_glem,

    
    _dnx_data_lif_global_lif_feature_nof
} dnx_data_lif_global_lif_feature_e;



typedef int(
    *dnx_data_lif_global_lif_feature_get_f) (
    int unit,
    dnx_data_lif_global_lif_feature_e feature);


typedef uint32(
    *dnx_data_lif_global_lif_use_mdb_size_get_f) (
    int unit);


typedef uint32(
    *dnx_data_lif_global_lif_glem_rif_optimization_enabled_get_f) (
    int unit);


typedef uint32(
    *dnx_data_lif_global_lif_null_lif_get_f) (
    int unit);


typedef uint32(
    *dnx_data_lif_global_lif_rif_not_in_glem_get_f) (
    int unit);


typedef uint32(
    *dnx_data_lif_global_lif_nof_global_in_lifs_get_f) (
    int unit);


typedef uint32(
    *dnx_data_lif_global_lif_nof_global_out_lifs_get_f) (
    int unit);


typedef uint32(
    *dnx_data_lif_global_lif_egress_in_lif_null_value_get_f) (
    int unit);


typedef uint32(
    *dnx_data_lif_global_lif_egress_out_lif_null_value_get_f) (
    int unit);


typedef uint32(
    *dnx_data_lif_global_lif_nof_global_l2_gport_lifs_get_f) (
    int unit);


typedef uint32(
    *dnx_data_lif_global_lif_global_lif_bta_max_val_get_f) (
    int unit);


typedef uint32(
    *dnx_data_lif_global_lif_global_lif_grain_size_get_f) (
    int unit);


typedef uint32(
    *dnx_data_lif_global_lif_mpls_data_table_skip_phase_check_get_f) (
    int unit);



typedef struct
{
    
    dnx_data_lif_global_lif_feature_get_f feature_get;
    
    dnx_data_lif_global_lif_use_mdb_size_get_f use_mdb_size_get;
    
    dnx_data_lif_global_lif_glem_rif_optimization_enabled_get_f glem_rif_optimization_enabled_get;
    
    dnx_data_lif_global_lif_null_lif_get_f null_lif_get;
    
    dnx_data_lif_global_lif_rif_not_in_glem_get_f rif_not_in_glem_get;
    
    dnx_data_lif_global_lif_nof_global_in_lifs_get_f nof_global_in_lifs_get;
    
    dnx_data_lif_global_lif_nof_global_out_lifs_get_f nof_global_out_lifs_get;
    
    dnx_data_lif_global_lif_egress_in_lif_null_value_get_f egress_in_lif_null_value_get;
    
    dnx_data_lif_global_lif_egress_out_lif_null_value_get_f egress_out_lif_null_value_get;
    
    dnx_data_lif_global_lif_nof_global_l2_gport_lifs_get_f nof_global_l2_gport_lifs_get;
    
    dnx_data_lif_global_lif_global_lif_bta_max_val_get_f global_lif_bta_max_val_get;
    
    dnx_data_lif_global_lif_global_lif_grain_size_get_f global_lif_grain_size_get;
    
    dnx_data_lif_global_lif_mpls_data_table_skip_phase_check_get_f mpls_data_table_skip_phase_check_get;
} dnx_data_if_lif_global_lif_t;







typedef enum
{
    
    dnx_data_lif_in_lif_mapped_in_lif_profile,
    
    dnx_data_lif_in_lif_in_lif_profile_iop_mode,
    
    dnx_data_lif_in_lif_phy_db_dpc,
    
    dnx_data_lif_in_lif_cs_in_lif_profile_based_on_stages,
    
    dnx_data_lif_in_lif_p2p_or_mp_based_on_cs_in_lif_profile,

    
    _dnx_data_lif_in_lif_feature_nof
} dnx_data_lif_in_lif_feature_e;



typedef int(
    *dnx_data_lif_in_lif_feature_get_f) (
    int unit,
    dnx_data_lif_in_lif_feature_e feature);


typedef uint32(
    *dnx_data_lif_in_lif_inlif_index_bit_size_get_f) (
    int unit);


typedef uint32(
    *dnx_data_lif_in_lif_inlif_minimum_index_size_get_f) (
    int unit);


typedef uint32(
    *dnx_data_lif_in_lif_inlif_resource_tag_size_get_f) (
    int unit);


typedef uint32(
    *dnx_data_lif_in_lif_nof_in_lif_profiles_get_f) (
    int unit);


typedef uint32(
    *dnx_data_lif_in_lif_nof_used_in_lif_profile_bits_get_f) (
    int unit);


typedef uint32(
    *dnx_data_lif_in_lif_nof_used_eth_rif_profile_bits_get_f) (
    int unit);


typedef uint32(
    *dnx_data_lif_in_lif_nof_in_lif_incoming_orientation_get_f) (
    int unit);


typedef uint32(
    *dnx_data_lif_in_lif_nof_eth_rif_profiles_get_f) (
    int unit);


typedef uint32(
    *dnx_data_lif_in_lif_nof_da_not_found_destination_profiles_get_f) (
    int unit);


typedef uint32(
    *dnx_data_lif_in_lif_default_da_not_found_destination_profile_get_f) (
    int unit);


typedef uint32(
    *dnx_data_lif_in_lif_drop_in_lif_get_f) (
    int unit);


typedef uint32(
    *dnx_data_lif_in_lif_mc_bridge_fallback_per_lif_profile_get_f) (
    int unit);


typedef uint32(
    *dnx_data_lif_in_lif_max_number_of_orientation_bits_iop_mode_get_f) (
    int unit);


typedef uint32(
    *dnx_data_lif_in_lif_max_number_of_egress_lif_profile_bits_iop_mode_get_f) (
    int unit);


typedef uint32(
    *dnx_data_lif_in_lif_algo_flow_inlif_bank_size_get_f) (
    int unit);


typedef uint32(
    *dnx_data_lif_in_lif_in_lif_profile_pmf_reserved_start_bit_get_f) (
    int unit);


typedef uint32(
    *dnx_data_lif_in_lif_eth_rif_profile_pmf_reserved_start_bit_get_f) (
    int unit);


typedef uint32(
    *dnx_data_lif_in_lif_in_lif_profile_allocate_indexed_mode_get_f) (
    int unit);


typedef uint32(
    *dnx_data_lif_in_lif_in_lif_profile_allocate_orientation_get_f) (
    int unit);


typedef uint32(
    *dnx_data_lif_in_lif_in_lif_profile_allocate_policer_inner_dp_get_f) (
    int unit);


typedef uint32(
    *dnx_data_lif_in_lif_in_lif_profile_allocate_policer_outer_dp_get_f) (
    int unit);


typedef uint32(
    *dnx_data_lif_in_lif_in_lif_profile_allocate_same_interface_mode_get_f) (
    int unit);



typedef struct
{
    
    dnx_data_lif_in_lif_feature_get_f feature_get;
    
    dnx_data_lif_in_lif_inlif_index_bit_size_get_f inlif_index_bit_size_get;
    
    dnx_data_lif_in_lif_inlif_minimum_index_size_get_f inlif_minimum_index_size_get;
    
    dnx_data_lif_in_lif_inlif_resource_tag_size_get_f inlif_resource_tag_size_get;
    
    dnx_data_lif_in_lif_nof_in_lif_profiles_get_f nof_in_lif_profiles_get;
    
    dnx_data_lif_in_lif_nof_used_in_lif_profile_bits_get_f nof_used_in_lif_profile_bits_get;
    
    dnx_data_lif_in_lif_nof_used_eth_rif_profile_bits_get_f nof_used_eth_rif_profile_bits_get;
    
    dnx_data_lif_in_lif_nof_in_lif_incoming_orientation_get_f nof_in_lif_incoming_orientation_get;
    
    dnx_data_lif_in_lif_nof_eth_rif_profiles_get_f nof_eth_rif_profiles_get;
    
    dnx_data_lif_in_lif_nof_da_not_found_destination_profiles_get_f nof_da_not_found_destination_profiles_get;
    
    dnx_data_lif_in_lif_default_da_not_found_destination_profile_get_f default_da_not_found_destination_profile_get;
    
    dnx_data_lif_in_lif_drop_in_lif_get_f drop_in_lif_get;
    
    dnx_data_lif_in_lif_mc_bridge_fallback_per_lif_profile_get_f mc_bridge_fallback_per_lif_profile_get;
    
    dnx_data_lif_in_lif_max_number_of_orientation_bits_iop_mode_get_f max_number_of_orientation_bits_iop_mode_get;
    
    dnx_data_lif_in_lif_max_number_of_egress_lif_profile_bits_iop_mode_get_f max_number_of_egress_lif_profile_bits_iop_mode_get;
    
    dnx_data_lif_in_lif_algo_flow_inlif_bank_size_get_f algo_flow_inlif_bank_size_get;
    
    dnx_data_lif_in_lif_in_lif_profile_pmf_reserved_start_bit_get_f in_lif_profile_pmf_reserved_start_bit_get;
    
    dnx_data_lif_in_lif_eth_rif_profile_pmf_reserved_start_bit_get_f eth_rif_profile_pmf_reserved_start_bit_get;
    
    dnx_data_lif_in_lif_in_lif_profile_allocate_indexed_mode_get_f in_lif_profile_allocate_indexed_mode_get;
    
    dnx_data_lif_in_lif_in_lif_profile_allocate_orientation_get_f in_lif_profile_allocate_orientation_get;
    
    dnx_data_lif_in_lif_in_lif_profile_allocate_policer_inner_dp_get_f in_lif_profile_allocate_policer_inner_dp_get;
    
    dnx_data_lif_in_lif_in_lif_profile_allocate_policer_outer_dp_get_f in_lif_profile_allocate_policer_outer_dp_get;
    
    dnx_data_lif_in_lif_in_lif_profile_allocate_same_interface_mode_get_f in_lif_profile_allocate_same_interface_mode_get;
} dnx_data_if_lif_in_lif_t;






typedef struct
{
    
    int physical_phase;
} dnx_data_lif_out_lif_logical_to_physical_phase_map_t;



typedef enum
{
    
    dnx_data_lif_out_lif_out_lif_profile_supported,

    
    _dnx_data_lif_out_lif_feature_nof
} dnx_data_lif_out_lif_feature_e;



typedef int(
    *dnx_data_lif_out_lif_feature_get_f) (
    int unit,
    dnx_data_lif_out_lif_feature_e feature);


typedef uint32(
    *dnx_data_lif_out_lif_nof_out_lif_profiles_get_f) (
    int unit);


typedef uint32(
    *dnx_data_lif_out_lif_nof_erpp_out_lif_profiles_get_f) (
    int unit);


typedef uint32(
    *dnx_data_lif_out_lif_nof_out_lif_outgoing_orientation_get_f) (
    int unit);


typedef uint32(
    *dnx_data_lif_out_lif_outrif_profile_width_get_f) (
    int unit);


typedef uint32(
    *dnx_data_lif_out_lif_glem_result_get_f) (
    int unit);


typedef uint32(
    *dnx_data_lif_out_lif_physical_bank_pointer_size_get_f) (
    int unit);


typedef uint32(
    *dnx_data_lif_out_lif_outlif_eedb_banks_pointer_size_get_f) (
    int unit);


typedef uint32(
    *dnx_data_lif_out_lif_global_outlif_bta_sop_resolution_get_f) (
    int unit);


typedef uint32(
    *dnx_data_lif_out_lif_pmf_outlif_profile_width_get_f) (
    int unit);


typedef uint32(
    *dnx_data_lif_out_lif_pmf_outrif_profile_width_get_f) (
    int unit);


typedef uint32(
    *dnx_data_lif_out_lif_outlif_pointer_size_get_f) (
    int unit);


typedef uint32(
    *dnx_data_lif_out_lif_allocation_bank_size_get_f) (
    int unit);


typedef uint32(
    *dnx_data_lif_out_lif_local_outlif_width_get_f) (
    int unit);


typedef uint32(
    *dnx_data_lif_out_lif_outlif_profile_width_get_f) (
    int unit);


typedef uint32(
    *dnx_data_lif_out_lif_nof_local_out_lifs_get_f) (
    int unit);


typedef uint32(
    *dnx_data_lif_out_lif_physical_bank_size_get_f) (
    int unit);


typedef uint32(
    *dnx_data_lif_out_lif_global_ipv6_outlif_bta_sop_get_f) (
    int unit);


typedef uint32(
    *dnx_data_lif_out_lif_global_vxlan_outlif_bta_sop_get_f) (
    int unit);


typedef uint32(
    *dnx_data_lif_out_lif_global_mpls_outlif_bta_sop_get_f) (
    int unit);


typedef uint32(
    *dnx_data_lif_out_lif_global_srv6_outlif_bta_sop_get_f) (
    int unit);


typedef uint32(
    *dnx_data_lif_out_lif_global_sflow_extended_gateway_two_outlif_bta_sop_get_f) (
    int unit);


typedef uint32(
    *dnx_data_lif_out_lif_global_sflow_extended_gateway_one_outlif_bta_sop_get_f) (
    int unit);


typedef uint32(
    *dnx_data_lif_out_lif_global_sflow_header_sampling_outlif_bta_sop_get_f) (
    int unit);


typedef uint32(
    *dnx_data_lif_out_lif_global_sflow_dp_outlif_bta_sop_get_f) (
    int unit);


typedef uint32(
    *dnx_data_lif_out_lif_global_gtp_concat_ip_outlif_bta_sop_get_f) (
    int unit);


typedef uint32(
    *dnx_data_lif_out_lif_global_bier_outlif_bta_sop_get_f) (
    int unit);


typedef uint32(
    *dnx_data_lif_out_lif_global_lif_allocation_estimated_encap_size_enable_get_f) (
    int unit);


typedef uint32(
    *dnx_data_lif_out_lif_mpls_data_table_skip_phase_check_get_f) (
    int unit);


typedef const dnx_data_lif_out_lif_logical_to_physical_phase_map_t *(
    *dnx_data_lif_out_lif_logical_to_physical_phase_map_get_f) (
    int unit,
    int logical_phase);



typedef struct
{
    
    dnx_data_lif_out_lif_feature_get_f feature_get;
    
    dnx_data_lif_out_lif_nof_out_lif_profiles_get_f nof_out_lif_profiles_get;
    
    dnx_data_lif_out_lif_nof_erpp_out_lif_profiles_get_f nof_erpp_out_lif_profiles_get;
    
    dnx_data_lif_out_lif_nof_out_lif_outgoing_orientation_get_f nof_out_lif_outgoing_orientation_get;
    
    dnx_data_lif_out_lif_outrif_profile_width_get_f outrif_profile_width_get;
    
    dnx_data_lif_out_lif_glem_result_get_f glem_result_get;
    
    dnx_data_lif_out_lif_physical_bank_pointer_size_get_f physical_bank_pointer_size_get;
    
    dnx_data_lif_out_lif_outlif_eedb_banks_pointer_size_get_f outlif_eedb_banks_pointer_size_get;
    
    dnx_data_lif_out_lif_global_outlif_bta_sop_resolution_get_f global_outlif_bta_sop_resolution_get;
    
    dnx_data_lif_out_lif_pmf_outlif_profile_width_get_f pmf_outlif_profile_width_get;
    
    dnx_data_lif_out_lif_pmf_outrif_profile_width_get_f pmf_outrif_profile_width_get;
    
    dnx_data_lif_out_lif_outlif_pointer_size_get_f outlif_pointer_size_get;
    
    dnx_data_lif_out_lif_allocation_bank_size_get_f allocation_bank_size_get;
    
    dnx_data_lif_out_lif_local_outlif_width_get_f local_outlif_width_get;
    
    dnx_data_lif_out_lif_outlif_profile_width_get_f outlif_profile_width_get;
    
    dnx_data_lif_out_lif_nof_local_out_lifs_get_f nof_local_out_lifs_get;
    
    dnx_data_lif_out_lif_physical_bank_size_get_f physical_bank_size_get;
    
    dnx_data_lif_out_lif_global_ipv6_outlif_bta_sop_get_f global_ipv6_outlif_bta_sop_get;
    
    dnx_data_lif_out_lif_global_vxlan_outlif_bta_sop_get_f global_vxlan_outlif_bta_sop_get;
    
    dnx_data_lif_out_lif_global_mpls_outlif_bta_sop_get_f global_mpls_outlif_bta_sop_get;
    
    dnx_data_lif_out_lif_global_srv6_outlif_bta_sop_get_f global_srv6_outlif_bta_sop_get;
    
    dnx_data_lif_out_lif_global_sflow_extended_gateway_two_outlif_bta_sop_get_f global_sflow_extended_gateway_two_outlif_bta_sop_get;
    
    dnx_data_lif_out_lif_global_sflow_extended_gateway_one_outlif_bta_sop_get_f global_sflow_extended_gateway_one_outlif_bta_sop_get;
    
    dnx_data_lif_out_lif_global_sflow_header_sampling_outlif_bta_sop_get_f global_sflow_header_sampling_outlif_bta_sop_get;
    
    dnx_data_lif_out_lif_global_sflow_dp_outlif_bta_sop_get_f global_sflow_dp_outlif_bta_sop_get;
    
    dnx_data_lif_out_lif_global_gtp_concat_ip_outlif_bta_sop_get_f global_gtp_concat_ip_outlif_bta_sop_get;
    
    dnx_data_lif_out_lif_global_bier_outlif_bta_sop_get_f global_bier_outlif_bta_sop_get;
    
    dnx_data_lif_out_lif_global_lif_allocation_estimated_encap_size_enable_get_f global_lif_allocation_estimated_encap_size_enable_get;
    
    dnx_data_lif_out_lif_mpls_data_table_skip_phase_check_get_f mpls_data_table_skip_phase_check_get;
    
    dnx_data_lif_out_lif_logical_to_physical_phase_map_get_f logical_to_physical_phase_map_get;
    
    dnxc_data_table_info_get_f logical_to_physical_phase_map_info_get;
} dnx_data_if_lif_out_lif_t;







typedef enum
{
    
    dnx_data_lif_lif_table_manager_skip_in_lif_example_table,
    
    dnx_data_lif_lif_table_manager_use_per_field_attributes,

    
    _dnx_data_lif_lif_table_manager_feature_nof
} dnx_data_lif_lif_table_manager_feature_e;



typedef int(
    *dnx_data_lif_lif_table_manager_feature_get_f) (
    int unit,
    dnx_data_lif_lif_table_manager_feature_e feature);


typedef uint32(
    *dnx_data_lif_lif_table_manager_max_nof_lif_tables_get_f) (
    int unit);


typedef uint32(
    *dnx_data_lif_lif_table_manager_max_nof_result_types_get_f) (
    int unit);


typedef uint32(
    *dnx_data_lif_lif_table_manager_max_fields_per_result_type_get_f) (
    int unit);


typedef uint32(
    *dnx_data_lif_lif_table_manager_max_fields_per_table_get_f) (
    int unit);



typedef struct
{
    
    dnx_data_lif_lif_table_manager_feature_get_f feature_get;
    
    dnx_data_lif_lif_table_manager_max_nof_lif_tables_get_f max_nof_lif_tables_get;
    
    dnx_data_lif_lif_table_manager_max_nof_result_types_get_f max_nof_result_types_get;
    
    dnx_data_lif_lif_table_manager_max_fields_per_result_type_get_f max_fields_per_result_type_get;
    
    dnx_data_lif_lif_table_manager_max_fields_per_table_get_f max_fields_per_table_get;
} dnx_data_if_lif_lif_table_manager_t;







typedef enum
{
    
    dnx_data_lif_feature_iop_mode_outlif_selection,
    
    dnx_data_lif_feature_iop_mode_orientation_selection,
    
    dnx_data_lif_feature_in_lif_tunnel_wide_data,
    
    dnx_data_lif_feature_ingress_egress_vsi_separation,

    
    _dnx_data_lif_feature_feature_nof
} dnx_data_lif_feature_feature_e;



typedef int(
    *dnx_data_lif_feature_feature_get_f) (
    int unit,
    dnx_data_lif_feature_feature_e feature);



typedef struct
{
    
    dnx_data_lif_feature_feature_get_f feature_get;
} dnx_data_if_lif_feature_t;





typedef struct
{
    
    dnx_data_if_lif_global_lif_t global_lif;
    
    dnx_data_if_lif_in_lif_t in_lif;
    
    dnx_data_if_lif_out_lif_t out_lif;
    
    dnx_data_if_lif_lif_table_manager_t lif_table_manager;
    
    dnx_data_if_lif_feature_t feature;
} dnx_data_if_lif_t;




extern dnx_data_if_lif_t dnx_data_lif;


#endif 

