
/* *INDENT-OFF* */
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2021 Broadcom Inc. All rights reserved.
 */
#ifndef _DNX_DATA_INTERNAL_FIELD_MAP_H_

#define _DNX_DATA_INTERNAL_FIELD_MAP_H_

#include <soc/dnx/dnx_data/auto_generated/dnx_data_internal.h>
#include <soc/dnx/dnx_data/auto_generated/dnx_data_field_map.h>



#ifndef BCM_DNX_SUPPORT
#error "This file is for use by DNX (JR2) family only!"
#endif



typedef enum
{
    dnx_data_field_map_submodule_action,
    dnx_data_field_map_submodule_qual,

    
    _dnx_data_field_map_submodule_nof
} dnx_data_field_map_submodule_e;








int dnx_data_field_map_action_feature_get(
    int unit,
    dnx_data_field_map_action_feature_e feature);



typedef enum
{

    
    _dnx_data_field_map_action_define_nof
} dnx_data_field_map_action_define_e;




typedef enum
{
    dnx_data_field_map_action_table_params,

    
    _dnx_data_field_map_action_table_nof
} dnx_data_field_map_action_table_e;



const dnx_data_field_map_action_params_t * dnx_data_field_map_action_params_get(
    int unit,
    int stage,
    int action);



shr_error_e dnx_data_field_map_action_params_entry_str_get(
    int unit,
    char *buffer,
    int key0,
    int key1,
    int value_index);



const dnxc_data_table_info_t * dnx_data_field_map_action_params_info_get(
    int unit);






int dnx_data_field_map_qual_feature_get(
    int unit,
    dnx_data_field_map_qual_feature_e feature);



typedef enum
{

    
    _dnx_data_field_map_qual_define_nof
} dnx_data_field_map_qual_define_e;




typedef enum
{
    dnx_data_field_map_qual_table_params,
    dnx_data_field_map_qual_table_special_metadata_qual,
    dnx_data_field_map_qual_table_layer_record_info_ingress,
    dnx_data_field_map_qual_table_layer_record_info_egress,

    
    _dnx_data_field_map_qual_table_nof
} dnx_data_field_map_qual_table_e;



const dnx_data_field_map_qual_params_t * dnx_data_field_map_qual_params_get(
    int unit,
    int stage,
    int qual);


const dnx_data_field_map_qual_special_metadata_qual_t * dnx_data_field_map_qual_special_metadata_qual_get(
    int unit,
    int stage,
    int qual);


const dnx_data_field_map_qual_layer_record_info_ingress_t * dnx_data_field_map_qual_layer_record_info_ingress_get(
    int unit,
    int layer_record);


const dnx_data_field_map_qual_layer_record_info_egress_t * dnx_data_field_map_qual_layer_record_info_egress_get(
    int unit,
    int layer_record);



shr_error_e dnx_data_field_map_qual_params_entry_str_get(
    int unit,
    char *buffer,
    int key0,
    int key1,
    int value_index);


shr_error_e dnx_data_field_map_qual_special_metadata_qual_entry_str_get(
    int unit,
    char *buffer,
    int key0,
    int key1,
    int value_index);


shr_error_e dnx_data_field_map_qual_layer_record_info_ingress_entry_str_get(
    int unit,
    char *buffer,
    int key0,
    int key1,
    int value_index);


shr_error_e dnx_data_field_map_qual_layer_record_info_egress_entry_str_get(
    int unit,
    char *buffer,
    int key0,
    int key1,
    int value_index);



const dnxc_data_table_info_t * dnx_data_field_map_qual_params_info_get(
    int unit);


const dnxc_data_table_info_t * dnx_data_field_map_qual_special_metadata_qual_info_get(
    int unit);


const dnxc_data_table_info_t * dnx_data_field_map_qual_layer_record_info_ingress_info_get(
    int unit);


const dnxc_data_table_info_t * dnx_data_field_map_qual_layer_record_info_egress_info_get(
    int unit);



shr_error_e dnx_data_field_map_init(
    int unit,
    dnxc_data_module_t *module_data);



#endif 

