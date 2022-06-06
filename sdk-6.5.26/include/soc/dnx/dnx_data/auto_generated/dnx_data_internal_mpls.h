
/* *INDENT-OFF* */
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2022 Broadcom Inc. All rights reserved.
 */
#ifndef _DNX_DATA_INTERNAL_MPLS_H_

#define _DNX_DATA_INTERNAL_MPLS_H_

#include <soc/dnx/dnx_data/auto_generated/dnx_data_internal.h>
#include <soc/dnx/dnx_data/auto_generated/dnx_data_mpls.h>



#ifndef BCM_DNX_SUPPORT
#error "This file is for use by DNX family only!"
#endif



typedef enum
{
    dnx_data_mpls_submodule_general,

    
    _dnx_data_mpls_submodule_nof
} dnx_data_mpls_submodule_e;








int dnx_data_mpls_general_feature_get(
    int unit,
    dnx_data_mpls_general_feature_e feature);



typedef enum
{
    dnx_data_mpls_general_define_nof_mpls_termination_profiles,
    dnx_data_mpls_general_define_nof_mpls_push_profiles,
    dnx_data_mpls_general_define_nof_bits_mpls_label,
    dnx_data_mpls_general_define_log_nof_mpls_range_elements,
    dnx_data_mpls_general_define_nof_mpls_range_elements,
    dnx_data_mpls_general_define_mpls_special_label_encap_handling,
    dnx_data_mpls_general_define_mpls_control_word_supported,
    dnx_data_mpls_general_define_mpls_second_stage_parser_handling,
    dnx_data_mpls_general_define_mpls_control_word_flag_bits_lsb,
    dnx_data_mpls_general_define_fhei_mps_type_nof_bits,
    dnx_data_mpls_general_define_mpls_speculative_learning_handling,
    dnx_data_mpls_general_define_gach_with_flow_label_supported,
    dnx_data_mpls_general_define_max_nof_mpls_term_stages,
    dnx_data_mpls_general_define_nof_php_gport_lif,

    
    _dnx_data_mpls_general_define_nof
} dnx_data_mpls_general_define_e;



uint32 dnx_data_mpls_general_nof_mpls_termination_profiles_get(
    int unit);


uint32 dnx_data_mpls_general_nof_mpls_push_profiles_get(
    int unit);


uint32 dnx_data_mpls_general_nof_bits_mpls_label_get(
    int unit);


uint32 dnx_data_mpls_general_log_nof_mpls_range_elements_get(
    int unit);


uint32 dnx_data_mpls_general_nof_mpls_range_elements_get(
    int unit);


uint32 dnx_data_mpls_general_mpls_special_label_encap_handling_get(
    int unit);


uint32 dnx_data_mpls_general_mpls_control_word_supported_get(
    int unit);


uint32 dnx_data_mpls_general_mpls_second_stage_parser_handling_get(
    int unit);


uint32 dnx_data_mpls_general_mpls_control_word_flag_bits_lsb_get(
    int unit);


uint32 dnx_data_mpls_general_fhei_mps_type_nof_bits_get(
    int unit);


uint32 dnx_data_mpls_general_mpls_speculative_learning_handling_get(
    int unit);


uint32 dnx_data_mpls_general_gach_with_flow_label_supported_get(
    int unit);


uint32 dnx_data_mpls_general_max_nof_mpls_term_stages_get(
    int unit);


uint32 dnx_data_mpls_general_nof_php_gport_lif_get(
    int unit);



typedef enum
{
    dnx_data_mpls_general_table_fwd_trap_destination,

    
    _dnx_data_mpls_general_table_nof
} dnx_data_mpls_general_table_e;



const dnx_data_mpls_general_fwd_trap_destination_t * dnx_data_mpls_general_fwd_trap_destination_get(
    int unit,
    int index);



shr_error_e dnx_data_mpls_general_fwd_trap_destination_entry_str_get(
    int unit,
    char *buffer,
    int key0,
    int key1,
    int value_index);



const dnxc_data_table_info_t * dnx_data_mpls_general_fwd_trap_destination_info_get(
    int unit);



shr_error_e dnx_data_mpls_init(
    int unit,
    dnxc_data_module_t *module_data);



#endif 

