
/* *INDENT-OFF* */
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2021 Broadcom Inc. All rights reserved.
 */
#ifndef _DNX_DATA_ECGM_H_

#define _DNX_DATA_ECGM_H_

#include <sal/limits.h>
#include <soc/property.h>
#include <shared/shrextend/shrextend_debug.h>
#include <shared/utilex/utilex_framework.h>
#include <soc/dnxc/dnxc_data/dnxc_data_utils.h>
#include <soc/dnxc/dnxc_data/dnxc_data_mgmt.h>
#include <soc/dnx/dnx_data/auto_generated/dnx_data_max_ecgm.h>



#ifndef BCM_DNX_SUPPORT
#error "This file is for use by DNX (JR2) family only!"
#endif



shr_error_e dnx_data_if_ecgm_init(
    int unit);







typedef enum
{
    
    dnx_data_ecgm_general_global_receive_admission_and_drop_test,

    
    _dnx_data_ecgm_general_feature_nof
} dnx_data_ecgm_general_feature_e;



typedef int(
    *dnx_data_ecgm_general_feature_get_f) (
    int unit,
    dnx_data_ecgm_general_feature_e feature);


typedef uint32(
    *dnx_data_ecgm_general_ecgm_is_supported_get_f) (
    int unit);



typedef struct
{
    
    dnx_data_ecgm_general_feature_get_f feature_get;
    
    dnx_data_ecgm_general_ecgm_is_supported_get_f ecgm_is_supported_get;
} dnx_data_if_ecgm_general_t;







typedef enum
{

    
    _dnx_data_ecgm_core_resources_feature_nof
} dnx_data_ecgm_core_resources_feature_e;



typedef int(
    *dnx_data_ecgm_core_resources_feature_get_f) (
    int unit,
    dnx_data_ecgm_core_resources_feature_e feature);


typedef uint32(
    *dnx_data_ecgm_core_resources_nof_sp_get_f) (
    int unit);


typedef uint32(
    *dnx_data_ecgm_core_resources_nof_interface_profiles_get_f) (
    int unit);


typedef uint32(
    *dnx_data_ecgm_core_resources_nof_port_profiles_get_f) (
    int unit);


typedef uint32(
    *dnx_data_ecgm_core_resources_total_pds_get_f) (
    int unit);


typedef uint32(
    *dnx_data_ecgm_core_resources_total_pds_nof_bits_get_f) (
    int unit);


typedef uint32(
    *dnx_data_ecgm_core_resources_total_dbs_get_f) (
    int unit);


typedef uint32(
    *dnx_data_ecgm_core_resources_total_dbs_nof_bits_get_f) (
    int unit);


typedef uint32(
    *dnx_data_ecgm_core_resources_reserved_dbs_per_sp_nof_bits_get_f) (
    int unit);


typedef uint32(
    *dnx_data_ecgm_core_resources_max_alpha_value_get_f) (
    int unit);


typedef uint32(
    *dnx_data_ecgm_core_resources_min_alpha_value_get_f) (
    int unit);


typedef uint32(
    *dnx_data_ecgm_core_resources_max_core_bandwidth_Mbps_get_f) (
    int unit);


typedef uint32(
    *dnx_data_ecgm_core_resources_mc_rcy_reserved_interface_th_ids_get_f) (
    int unit);


typedef uint32(
    *dnx_data_ecgm_core_resources_mc_rcy_reserved_port_th_ids_get_f) (
    int unit);



typedef struct
{
    
    dnx_data_ecgm_core_resources_feature_get_f feature_get;
    
    dnx_data_ecgm_core_resources_nof_sp_get_f nof_sp_get;
    
    dnx_data_ecgm_core_resources_nof_interface_profiles_get_f nof_interface_profiles_get;
    
    dnx_data_ecgm_core_resources_nof_port_profiles_get_f nof_port_profiles_get;
    
    dnx_data_ecgm_core_resources_total_pds_get_f total_pds_get;
    
    dnx_data_ecgm_core_resources_total_pds_nof_bits_get_f total_pds_nof_bits_get;
    
    dnx_data_ecgm_core_resources_total_dbs_get_f total_dbs_get;
    
    dnx_data_ecgm_core_resources_total_dbs_nof_bits_get_f total_dbs_nof_bits_get;
    
    dnx_data_ecgm_core_resources_reserved_dbs_per_sp_nof_bits_get_f reserved_dbs_per_sp_nof_bits_get;
    
    dnx_data_ecgm_core_resources_max_alpha_value_get_f max_alpha_value_get;
    
    dnx_data_ecgm_core_resources_min_alpha_value_get_f min_alpha_value_get;
    
    dnx_data_ecgm_core_resources_max_core_bandwidth_Mbps_get_f max_core_bandwidth_Mbps_get;
    
    dnx_data_ecgm_core_resources_mc_rcy_reserved_interface_th_ids_get_f mc_rcy_reserved_interface_th_ids_get;
    
    dnx_data_ecgm_core_resources_mc_rcy_reserved_port_th_ids_get_f mc_rcy_reserved_port_th_ids_get;
} dnx_data_if_ecgm_core_resources_t;







typedef enum
{
    
    dnx_data_ecgm_port_resources_profile_management,

    
    _dnx_data_ecgm_port_resources_feature_nof
} dnx_data_ecgm_port_resources_feature_e;



typedef int(
    *dnx_data_ecgm_port_resources_feature_get_f) (
    int unit,
    dnx_data_ecgm_port_resources_feature_e feature);


typedef uint32(
    *dnx_data_ecgm_port_resources_max_nof_ports_get_f) (
    int unit);



typedef struct
{
    
    dnx_data_ecgm_port_resources_feature_get_f feature_get;
    
    dnx_data_ecgm_port_resources_max_nof_ports_get_f max_nof_ports_get;
} dnx_data_if_ecgm_port_resources_t;






typedef struct
{
    
    uint32 fqp_high_priority;
    
    uint32 stop_mc_low;
    
    uint32 stop_mc_high;
    
    uint32 stop_all;
} dnx_data_ecgm_delete_fifo_thresholds_t;



typedef enum
{

    
    _dnx_data_ecgm_delete_fifo_feature_nof
} dnx_data_ecgm_delete_fifo_feature_e;



typedef int(
    *dnx_data_ecgm_delete_fifo_feature_get_f) (
    int unit,
    dnx_data_ecgm_delete_fifo_feature_e feature);


typedef const dnx_data_ecgm_delete_fifo_thresholds_t *(
    *dnx_data_ecgm_delete_fifo_thresholds_get_f) (
    int unit,
    int fifo_type);



typedef struct
{
    
    dnx_data_ecgm_delete_fifo_feature_get_f feature_get;
    
    dnx_data_ecgm_delete_fifo_thresholds_get_f thresholds_get;
    
    dnxc_data_table_info_get_f thresholds_info_get;
} dnx_data_if_ecgm_delete_fifo_t;






typedef struct
{
    
    char *reason;
} dnx_data_ecgm_info_dropped_reason_rqp_t;


typedef struct
{
    
    char *reason;
} dnx_data_ecgm_info_dropped_reason_pqp_t;



typedef enum
{
    
    dnx_data_ecgm_info_fc_egr_if_vector_select,

    
    _dnx_data_ecgm_info_feature_nof
} dnx_data_ecgm_info_feature_e;



typedef int(
    *dnx_data_ecgm_info_feature_get_f) (
    int unit,
    dnx_data_ecgm_info_feature_e feature);


typedef uint32(
    *dnx_data_ecgm_info_fc_q_pair_vector_select_nof_bits_get_f) (
    int unit);


typedef uint32(
    *dnx_data_ecgm_info_fc_q_pair_vector_nof_presented_get_f) (
    int unit);


typedef uint32(
    *dnx_data_ecgm_info_fc_egr_if_vector_select_nof_bits_get_f) (
    int unit);


typedef uint32(
    *dnx_data_ecgm_info_fc_egr_if_vector_nof_presented_get_f) (
    int unit);


typedef uint32(
    *dnx_data_ecgm_info_nof_dropped_reasons_rqp_get_f) (
    int unit);


typedef uint32(
    *dnx_data_ecgm_info_nof_dropped_reasons_pqp_get_f) (
    int unit);


typedef uint32(
    *dnx_data_ecgm_info_nof_bits_in_pd_count_get_f) (
    int unit);


typedef const dnx_data_ecgm_info_dropped_reason_rqp_t *(
    *dnx_data_ecgm_info_dropped_reason_rqp_get_f) (
    int unit,
    int index);


typedef const dnx_data_ecgm_info_dropped_reason_pqp_t *(
    *dnx_data_ecgm_info_dropped_reason_pqp_get_f) (
    int unit,
    int index);



typedef struct
{
    
    dnx_data_ecgm_info_feature_get_f feature_get;
    
    dnx_data_ecgm_info_fc_q_pair_vector_select_nof_bits_get_f fc_q_pair_vector_select_nof_bits_get;
    
    dnx_data_ecgm_info_fc_q_pair_vector_nof_presented_get_f fc_q_pair_vector_nof_presented_get;
    
    dnx_data_ecgm_info_fc_egr_if_vector_select_nof_bits_get_f fc_egr_if_vector_select_nof_bits_get;
    
    dnx_data_ecgm_info_fc_egr_if_vector_nof_presented_get_f fc_egr_if_vector_nof_presented_get;
    
    dnx_data_ecgm_info_nof_dropped_reasons_rqp_get_f nof_dropped_reasons_rqp_get;
    
    dnx_data_ecgm_info_nof_dropped_reasons_pqp_get_f nof_dropped_reasons_pqp_get;
    
    dnx_data_ecgm_info_nof_bits_in_pd_count_get_f nof_bits_in_pd_count_get;
    
    dnx_data_ecgm_info_dropped_reason_rqp_get_f dropped_reason_rqp_get;
    
    dnxc_data_table_info_get_f dropped_reason_rqp_info_get;
    
    dnx_data_ecgm_info_dropped_reason_pqp_get_f dropped_reason_pqp_get;
    
    dnxc_data_table_info_get_f dropped_reason_pqp_info_get;
} dnx_data_if_ecgm_info_t;







typedef enum
{
    
    dnx_data_ecgm_pool_db_size_pool_db_size,

    
    _dnx_data_ecgm_pool_db_size_feature_nof
} dnx_data_ecgm_pool_db_size_feature_e;



typedef int(
    *dnx_data_ecgm_pool_db_size_feature_get_f) (
    int unit,
    dnx_data_ecgm_pool_db_size_feature_e feature);


typedef uint32(
    *dnx_data_ecgm_pool_db_size_uc_pool_db_max_size_get_f) (
    int unit);


typedef uint32(
    *dnx_data_ecgm_pool_db_size_mc_pool_db_max_size_get_f) (
    int unit);



typedef struct
{
    
    dnx_data_ecgm_pool_db_size_feature_get_f feature_get;
    
    dnx_data_ecgm_pool_db_size_uc_pool_db_max_size_get_f uc_pool_db_max_size_get;
    
    dnx_data_ecgm_pool_db_size_mc_pool_db_max_size_get_f mc_pool_db_max_size_get;
} dnx_data_if_ecgm_pool_db_size_t;





typedef struct
{
    
    dnx_data_if_ecgm_general_t general;
    
    dnx_data_if_ecgm_core_resources_t core_resources;
    
    dnx_data_if_ecgm_port_resources_t port_resources;
    
    dnx_data_if_ecgm_delete_fifo_t delete_fifo;
    
    dnx_data_if_ecgm_info_t info;
    
    dnx_data_if_ecgm_pool_db_size_t pool_db_size;
} dnx_data_if_ecgm_t;




extern dnx_data_if_ecgm_t dnx_data_ecgm;


#endif 

