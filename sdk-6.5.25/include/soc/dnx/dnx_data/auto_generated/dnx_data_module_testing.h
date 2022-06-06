
/* *INDENT-OFF* */
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2021 Broadcom Inc. All rights reserved.
 */
#ifndef _DNX_DATA_MODULE_TESTING_H_

#define _DNX_DATA_MODULE_TESTING_H_

#include <sal/limits.h>
#include <soc/property.h>
#include <shared/shrextend/shrextend_debug.h>
#include <shared/utilex/utilex_framework.h>
#include <soc/dnxc/dnxc_data/dnxc_data_utils.h>
#include <soc/dnxc/dnxc_data/dnxc_data_mgmt.h>
#include <bcm/fabric.h>
#include <soc/dnx/dnx_data/auto_generated/dnx_data_max_module_testing.h>



#ifndef BCM_DNX_SUPPORT
#error "This file is for use by DNX (JR2) family only!"
#endif



shr_error_e dnx_data_if_module_testing_init(
    int unit);






typedef struct
{
    
    uint32 num;
} dnx_data_module_testing_unsupported_supported_table_t;


typedef struct
{
    
    uint32 num;
} dnx_data_module_testing_unsupported_unsupported_table_t;



typedef enum
{
    
    dnx_data_module_testing_unsupported_supported_feature,
    
    dnx_data_module_testing_unsupported_unsupported_feature,

    
    _dnx_data_module_testing_unsupported_feature_nof
} dnx_data_module_testing_unsupported_feature_e;



typedef int(
    *dnx_data_module_testing_unsupported_feature_get_f) (
    int unit,
    dnx_data_module_testing_unsupported_feature_e feature);


typedef uint32(
    *dnx_data_module_testing_unsupported_supported_def_get_f) (
    int unit);


typedef uint32(
    *dnx_data_module_testing_unsupported_unsupported_def_get_f) (
    int unit);


typedef const dnx_data_module_testing_unsupported_supported_table_t *(
    *dnx_data_module_testing_unsupported_supported_table_get_f) (
    int unit);


typedef const dnx_data_module_testing_unsupported_unsupported_table_t *(
    *dnx_data_module_testing_unsupported_unsupported_table_get_f) (
    int unit);



typedef struct
{
    
    dnx_data_module_testing_unsupported_feature_get_f feature_get;
    
    dnx_data_module_testing_unsupported_supported_def_get_f supported_def_get;
    
    dnx_data_module_testing_unsupported_unsupported_def_get_f unsupported_def_get;
    
    dnx_data_module_testing_unsupported_supported_table_get_f supported_table_get;
    
    dnxc_data_table_info_get_f supported_table_info_get;
    
    dnx_data_module_testing_unsupported_unsupported_table_get_f unsupported_table_get;
    
    dnxc_data_table_info_get_f unsupported_table_info_get;
} dnx_data_if_module_testing_unsupported_t;






typedef struct
{
    
    uint32 standard_table_value;
} dnx_data_module_testing_standard_submodule_standard_table_t;

#ifdef BCM_DNX2_SUPPORT


typedef struct
{
    
    uint32 table_family_dnx2_value;
} dnx_data_module_testing_standard_submodule_table_family_dnx2_t;


#endif 


typedef enum
{
    
    dnx_data_module_testing_standard_submodule_standard_feature,
#ifdef BCM_DNX2_SUPPORT

    
    dnx_data_module_testing_standard_submodule_feature_family_dnx2,

#endif 

    
    _dnx_data_module_testing_standard_submodule_feature_nof
} dnx_data_module_testing_standard_submodule_feature_e;



typedef int(
    *dnx_data_module_testing_standard_submodule_feature_get_f) (
    int unit,
    dnx_data_module_testing_standard_submodule_feature_e feature);


typedef uint32(
    *dnx_data_module_testing_standard_submodule_standard_define_get_f) (
    int unit);

#ifdef BCM_DNX2_SUPPORT


typedef uint32(
    *dnx_data_module_testing_standard_submodule_define_family_dnx2_get_f) (
    int unit);

#endif 

typedef uint32(
    *dnx_data_module_testing_standard_submodule_standard_numeric_get_f) (
    int unit);

#ifdef BCM_DNX2_SUPPORT


typedef uint32(
    *dnx_data_module_testing_standard_submodule_num_family_dnx2_get_f) (
    int unit);

#endif 
#ifdef BCM_DNX2_SUPPORT


typedef uint32(
    *dnx_data_module_testing_standard_submodule_num_property_family_dnx2_get_f) (
    int unit);

#endif 

typedef const dnx_data_module_testing_standard_submodule_standard_table_t *(
    *dnx_data_module_testing_standard_submodule_standard_table_get_f) (
    int unit,
    int standard_table_key);

#ifdef BCM_DNX2_SUPPORT


typedef const dnx_data_module_testing_standard_submodule_table_family_dnx2_t *(
    *dnx_data_module_testing_standard_submodule_table_family_dnx2_get_f) (
    int unit,
    int table_family_dnx2_key);

#endif 


typedef struct
{
    
    dnx_data_module_testing_standard_submodule_feature_get_f feature_get;
    
    dnx_data_module_testing_standard_submodule_standard_define_get_f standard_define_get;
#ifdef BCM_DNX2_SUPPORT

    
    dnx_data_module_testing_standard_submodule_define_family_dnx2_get_f define_family_dnx2_get;

#endif 
    
    dnx_data_module_testing_standard_submodule_standard_numeric_get_f standard_numeric_get;
#ifdef BCM_DNX2_SUPPORT

    
    dnx_data_module_testing_standard_submodule_num_family_dnx2_get_f num_family_dnx2_get;

#endif 
#ifdef BCM_DNX2_SUPPORT

    
    dnx_data_module_testing_standard_submodule_num_property_family_dnx2_get_f num_property_family_dnx2_get;

#endif 
    
    dnx_data_module_testing_standard_submodule_standard_table_get_f standard_table_get;
    
    dnxc_data_table_info_get_f standard_table_info_get;
#ifdef BCM_DNX2_SUPPORT

    
    dnx_data_module_testing_standard_submodule_table_family_dnx2_get_f table_family_dnx2_get;

#endif 
#ifdef BCM_DNX2_SUPPORT

    
    dnxc_data_table_info_get_f table_family_dnx2_info_get;

#endif 
} dnx_data_if_module_testing_standard_submodule_t;






typedef struct
{
    
    int intval;
    
    uint8 uint8val;
    
    char charval;
    
    uint16 uint16val;
    
    bcm_fabric_device_type_t enumval;
    
    uint32 arrdefaultall[10];
    
    uint32 arrval[10];
    
    bcm_pbmp_t pbmpval;
    
    char *strval;
    
    char bufferval[100];
} dnx_data_module_testing_types_all_t;



typedef enum
{

    
    _dnx_data_module_testing_types_feature_nof
} dnx_data_module_testing_types_feature_e;



typedef int(
    *dnx_data_module_testing_types_feature_get_f) (
    int unit,
    dnx_data_module_testing_types_feature_e feature);


typedef int(
    *dnx_data_module_testing_types_def_signed_get_f) (
    int unit);


typedef uint32(
    *dnx_data_module_testing_types_def_unsigned_get_f) (
    int unit);


typedef int(
    *dnx_data_module_testing_types_num_signed_get_f) (
    int unit);


typedef uint32(
    *dnx_data_module_testing_types_num_unsigned_get_f) (
    int unit);


typedef const dnx_data_module_testing_types_all_t *(
    *dnx_data_module_testing_types_all_get_f) (
    int unit);



typedef struct
{
    
    dnx_data_module_testing_types_feature_get_f feature_get;
    
    dnx_data_module_testing_types_def_signed_get_f def_signed_get;
    
    dnx_data_module_testing_types_def_unsigned_get_f def_unsigned_get;
    
    dnx_data_module_testing_types_num_signed_get_f num_signed_get;
    
    dnx_data_module_testing_types_num_unsigned_get_f num_unsigned_get;
    
    dnx_data_module_testing_types_all_get_f all_get;
    
    dnxc_data_table_info_get_f all_info_get;
} dnx_data_if_module_testing_types_t;






typedef struct
{
    
    uint32 val;
} dnx_data_module_testing_property_methods_enable_t;


typedef struct
{
    
    uint32 val;
} dnx_data_module_testing_property_methods_port_enable_t;


typedef struct
{
    
    uint32 val;
} dnx_data_module_testing_property_methods_suffix_enable_t;


typedef struct
{
    
    uint32 val;
} dnx_data_module_testing_property_methods_range_t;


typedef struct
{
    
    uint32 val;
} dnx_data_module_testing_property_methods_port_range_t;


typedef struct
{
    
    uint32 val;
} dnx_data_module_testing_property_methods_suffix_range_t;


typedef struct
{
    
    uint32 val;
} dnx_data_module_testing_property_methods_direct_map_t;


typedef struct
{
    
    uint32 val;
} dnx_data_module_testing_property_methods_port_direct_map_t;


typedef struct
{
    
    uint32 val;
} dnx_data_module_testing_property_methods_suffix_direct_map_t;


typedef struct
{
    
    uint32 val;
} dnx_data_module_testing_property_methods_custom_t;


typedef struct
{
    
    bcm_pbmp_t val;
} dnx_data_module_testing_property_methods_pbmp_t;


typedef struct
{
    
    bcm_pbmp_t val;
} dnx_data_module_testing_property_methods_port_pbmp_t;


typedef struct
{
    
    bcm_pbmp_t val;
} dnx_data_module_testing_property_methods_suffix_pbmp_t;


typedef struct
{
    
    char *val;
} dnx_data_module_testing_property_methods_str_t;


typedef struct
{
    
    char *val;
} dnx_data_module_testing_property_methods_port_str_t;


typedef struct
{
    
    char *val;
} dnx_data_module_testing_property_methods_suffix_str_t;



typedef enum
{
    
    dnx_data_module_testing_property_methods_feature_enable,
    
    dnx_data_module_testing_property_methods_feature_disable,

    
    _dnx_data_module_testing_property_methods_feature_nof
} dnx_data_module_testing_property_methods_feature_e;



typedef int(
    *dnx_data_module_testing_property_methods_feature_get_f) (
    int unit,
    dnx_data_module_testing_property_methods_feature_e feature);


typedef uint32(
    *dnx_data_module_testing_property_methods_numeric_range_get_f) (
    int unit);


typedef const dnx_data_module_testing_property_methods_enable_t *(
    *dnx_data_module_testing_property_methods_enable_get_f) (
    int unit);


typedef const dnx_data_module_testing_property_methods_port_enable_t *(
    *dnx_data_module_testing_property_methods_port_enable_get_f) (
    int unit,
    int port);


typedef const dnx_data_module_testing_property_methods_suffix_enable_t *(
    *dnx_data_module_testing_property_methods_suffix_enable_get_f) (
    int unit,
    int link);


typedef const dnx_data_module_testing_property_methods_range_t *(
    *dnx_data_module_testing_property_methods_range_get_f) (
    int unit);


typedef const dnx_data_module_testing_property_methods_port_range_t *(
    *dnx_data_module_testing_property_methods_port_range_get_f) (
    int unit,
    int port);


typedef const dnx_data_module_testing_property_methods_suffix_range_t *(
    *dnx_data_module_testing_property_methods_suffix_range_get_f) (
    int unit,
    int link);


typedef const dnx_data_module_testing_property_methods_direct_map_t *(
    *dnx_data_module_testing_property_methods_direct_map_get_f) (
    int unit);


typedef const dnx_data_module_testing_property_methods_port_direct_map_t *(
    *dnx_data_module_testing_property_methods_port_direct_map_get_f) (
    int unit,
    int port);


typedef const dnx_data_module_testing_property_methods_suffix_direct_map_t *(
    *dnx_data_module_testing_property_methods_suffix_direct_map_get_f) (
    int unit,
    int link);


typedef const dnx_data_module_testing_property_methods_custom_t *(
    *dnx_data_module_testing_property_methods_custom_get_f) (
    int unit,
    int link,
    int pipe);


typedef const dnx_data_module_testing_property_methods_pbmp_t *(
    *dnx_data_module_testing_property_methods_pbmp_get_f) (
    int unit);


typedef const dnx_data_module_testing_property_methods_port_pbmp_t *(
    *dnx_data_module_testing_property_methods_port_pbmp_get_f) (
    int unit,
    int port);


typedef const dnx_data_module_testing_property_methods_suffix_pbmp_t *(
    *dnx_data_module_testing_property_methods_suffix_pbmp_get_f) (
    int unit,
    int index);


typedef const dnx_data_module_testing_property_methods_str_t *(
    *dnx_data_module_testing_property_methods_str_get_f) (
    int unit);


typedef const dnx_data_module_testing_property_methods_port_str_t *(
    *dnx_data_module_testing_property_methods_port_str_get_f) (
    int unit,
    int port);


typedef const dnx_data_module_testing_property_methods_suffix_str_t *(
    *dnx_data_module_testing_property_methods_suffix_str_get_f) (
    int unit,
    int index);



typedef struct
{
    
    dnx_data_module_testing_property_methods_feature_get_f feature_get;
    
    dnx_data_module_testing_property_methods_numeric_range_get_f numeric_range_get;
    
    dnx_data_module_testing_property_methods_enable_get_f enable_get;
    
    dnxc_data_table_info_get_f enable_info_get;
    
    dnx_data_module_testing_property_methods_port_enable_get_f port_enable_get;
    
    dnxc_data_table_info_get_f port_enable_info_get;
    
    dnx_data_module_testing_property_methods_suffix_enable_get_f suffix_enable_get;
    
    dnxc_data_table_info_get_f suffix_enable_info_get;
    
    dnx_data_module_testing_property_methods_range_get_f range_get;
    
    dnxc_data_table_info_get_f range_info_get;
    
    dnx_data_module_testing_property_methods_port_range_get_f port_range_get;
    
    dnxc_data_table_info_get_f port_range_info_get;
    
    dnx_data_module_testing_property_methods_suffix_range_get_f suffix_range_get;
    
    dnxc_data_table_info_get_f suffix_range_info_get;
    
    dnx_data_module_testing_property_methods_direct_map_get_f direct_map_get;
    
    dnxc_data_table_info_get_f direct_map_info_get;
    
    dnx_data_module_testing_property_methods_port_direct_map_get_f port_direct_map_get;
    
    dnxc_data_table_info_get_f port_direct_map_info_get;
    
    dnx_data_module_testing_property_methods_suffix_direct_map_get_f suffix_direct_map_get;
    
    dnxc_data_table_info_get_f suffix_direct_map_info_get;
    
    dnx_data_module_testing_property_methods_custom_get_f custom_get;
    
    dnxc_data_table_info_get_f custom_info_get;
    
    dnx_data_module_testing_property_methods_pbmp_get_f pbmp_get;
    
    dnxc_data_table_info_get_f pbmp_info_get;
    
    dnx_data_module_testing_property_methods_port_pbmp_get_f port_pbmp_get;
    
    dnxc_data_table_info_get_f port_pbmp_info_get;
    
    dnx_data_module_testing_property_methods_suffix_pbmp_get_f suffix_pbmp_get;
    
    dnxc_data_table_info_get_f suffix_pbmp_info_get;
    
    dnx_data_module_testing_property_methods_str_get_f str_get;
    
    dnxc_data_table_info_get_f str_info_get;
    
    dnx_data_module_testing_property_methods_port_str_get_f port_str_get;
    
    dnxc_data_table_info_get_f port_str_info_get;
    
    dnx_data_module_testing_property_methods_suffix_str_get_f suffix_str_get;
    
    dnxc_data_table_info_get_f suffix_str_info_get;
} dnx_data_if_module_testing_property_methods_t;






typedef struct
{
    
    uint32 num_cores;
} dnx_data_module_testing_data_pointers_table_pointers_t;



typedef enum
{

    
    _dnx_data_module_testing_data_pointers_feature_nof
} dnx_data_module_testing_data_pointers_feature_e;



typedef int(
    *dnx_data_module_testing_data_pointers_feature_get_f) (
    int unit,
    dnx_data_module_testing_data_pointers_feature_e feature);


typedef uint32(
    *dnx_data_module_testing_data_pointers_cores_get_f) (
    int unit);


typedef uint32(
    *dnx_data_module_testing_data_pointers_add_cores_get_f) (
    int unit);


typedef uint32(
    *dnx_data_module_testing_data_pointers_mul_cores_get_f) (
    int unit);


typedef uint32(
    *dnx_data_module_testing_data_pointers_div_cores_get_f) (
    int unit);


typedef uint32(
    *dnx_data_module_testing_data_pointers_op_cores_get_f) (
    int unit);


typedef uint32(
    *dnx_data_module_testing_data_pointers_num_cores_get_f) (
    int unit);


typedef uint32(
    *dnx_data_module_testing_data_pointers_num_add_cores_get_f) (
    int unit);


typedef uint32(
    *dnx_data_module_testing_data_pointers_num_mul_cores_get_f) (
    int unit);


typedef uint32(
    *dnx_data_module_testing_data_pointers_num_div_cores_get_f) (
    int unit);


typedef uint32(
    *dnx_data_module_testing_data_pointers_num_op_cores_get_f) (
    int unit);


typedef uint32(
    *dnx_data_module_testing_data_pointers_num_last_cores_get_f) (
    int unit);


typedef uint32(
    *dnx_data_module_testing_data_pointers_num_table_cores_get_f) (
    int unit);


typedef const dnx_data_module_testing_data_pointers_table_pointers_t *(
    *dnx_data_module_testing_data_pointers_table_pointers_get_f) (
    int unit,
    int index);



typedef struct
{
    
    dnx_data_module_testing_data_pointers_feature_get_f feature_get;
    
    dnx_data_module_testing_data_pointers_cores_get_f cores_get;
    
    dnx_data_module_testing_data_pointers_add_cores_get_f add_cores_get;
    
    dnx_data_module_testing_data_pointers_mul_cores_get_f mul_cores_get;
    
    dnx_data_module_testing_data_pointers_div_cores_get_f div_cores_get;
    
    dnx_data_module_testing_data_pointers_op_cores_get_f op_cores_get;
    
    dnx_data_module_testing_data_pointers_num_cores_get_f num_cores_get;
    
    dnx_data_module_testing_data_pointers_num_add_cores_get_f num_add_cores_get;
    
    dnx_data_module_testing_data_pointers_num_mul_cores_get_f num_mul_cores_get;
    
    dnx_data_module_testing_data_pointers_num_div_cores_get_f num_div_cores_get;
    
    dnx_data_module_testing_data_pointers_num_op_cores_get_f num_op_cores_get;
    
    dnx_data_module_testing_data_pointers_num_last_cores_get_f num_last_cores_get;
    
    dnx_data_module_testing_data_pointers_num_table_cores_get_f num_table_cores_get;
    
    dnx_data_module_testing_data_pointers_table_pointers_get_f table_pointers_get;
    
    dnxc_data_table_info_get_f table_pointers_info_get;
} dnx_data_if_module_testing_data_pointers_t;






typedef struct
{
    
    uint32 vlan_id_in_ingress;
    
    uint32 fid_in_mact;
    
    uint32 key_max_val;
    
    uint32 key_field_size;
} dnx_data_module_testing_dbal_all_t;



typedef enum
{

    
    _dnx_data_module_testing_dbal_feature_nof
} dnx_data_module_testing_dbal_feature_e;



typedef int(
    *dnx_data_module_testing_dbal_feature_get_f) (
    int unit,
    dnx_data_module_testing_dbal_feature_e feature);


typedef uint32(
    *dnx_data_module_testing_dbal_vrf_field_size_get_f) (
    int unit);


typedef uint32(
    *dnx_data_module_testing_dbal_field_define_example_get_f) (
    int unit);


typedef uint32(
    *dnx_data_module_testing_dbal_res_max_val_get_f) (
    int unit);


typedef uint32(
    *dnx_data_module_testing_dbal_res_field_size_get_f) (
    int unit);


typedef const dnx_data_module_testing_dbal_all_t *(
    *dnx_data_module_testing_dbal_all_get_f) (
    int unit);



typedef struct
{
    
    dnx_data_module_testing_dbal_feature_get_f feature_get;
    
    dnx_data_module_testing_dbal_vrf_field_size_get_f vrf_field_size_get;
    
    dnx_data_module_testing_dbal_field_define_example_get_f field_define_example_get;
    
    dnx_data_module_testing_dbal_res_max_val_get_f res_max_val_get;
    
    dnx_data_module_testing_dbal_res_field_size_get_f res_field_size_get;
    
    dnx_data_module_testing_dbal_all_get_f all_get;
    
    dnxc_data_table_info_get_f all_info_get;
} dnx_data_if_module_testing_dbal_t;






typedef struct
{
    
    uint32 vlan_id_in_ingress;
} dnx_data_module_testing_example_tests_all_t;



typedef enum
{

    
    _dnx_data_module_testing_example_tests_feature_nof
} dnx_data_module_testing_example_tests_feature_e;



typedef int(
    *dnx_data_module_testing_example_tests_feature_get_f) (
    int unit,
    dnx_data_module_testing_example_tests_feature_e feature);


typedef uint32(
    *dnx_data_module_testing_example_tests_field_size_get_f) (
    int unit);


typedef uint32(
    *dnx_data_module_testing_example_tests_large_nof_elements_get_f) (
    int unit);


typedef const dnx_data_module_testing_example_tests_all_t *(
    *dnx_data_module_testing_example_tests_all_get_f) (
    int unit);



typedef struct
{
    
    dnx_data_module_testing_example_tests_feature_get_f feature_get;
    
    dnx_data_module_testing_example_tests_field_size_get_f field_size_get;
    
    dnx_data_module_testing_example_tests_large_nof_elements_get_f large_nof_elements_get;
    
    dnx_data_module_testing_example_tests_all_get_f all_get;
    
    dnxc_data_table_info_get_f all_info_get;
} dnx_data_if_module_testing_example_tests_t;



#ifdef BCM_DNX2_SUPPORT




typedef struct
{
    
    uint32 dnx2_submodule_table_value;
} dnx_data_module_testing_submodule_family_dnx2_dnx2_submodule_table_t;



typedef enum
{
    
    dnx_data_module_testing_submodule_family_dnx2_dnx2_submodule_feature,

    
    _dnx_data_module_testing_submodule_family_dnx2_feature_nof
} dnx_data_module_testing_submodule_family_dnx2_feature_e;



typedef int(
    *dnx_data_module_testing_submodule_family_dnx2_feature_get_f) (
    int unit,
    dnx_data_module_testing_submodule_family_dnx2_feature_e feature);


typedef uint32(
    *dnx_data_module_testing_submodule_family_dnx2_dnx2_submodule_define_get_f) (
    int unit);


typedef uint32(
    *dnx_data_module_testing_submodule_family_dnx2_dnx2_submodule_numeric_get_f) (
    int unit);


typedef const dnx_data_module_testing_submodule_family_dnx2_dnx2_submodule_table_t *(
    *dnx_data_module_testing_submodule_family_dnx2_dnx2_submodule_table_get_f) (
    int unit,
    int dnx2_submodule_table_key);



typedef struct
{
    
    dnx_data_module_testing_submodule_family_dnx2_feature_get_f feature_get;
    
    dnx_data_module_testing_submodule_family_dnx2_dnx2_submodule_define_get_f dnx2_submodule_define_get;
    
    dnx_data_module_testing_submodule_family_dnx2_dnx2_submodule_numeric_get_f dnx2_submodule_numeric_get;
    
    dnx_data_module_testing_submodule_family_dnx2_dnx2_submodule_table_get_f dnx2_submodule_table_get;
    
    dnxc_data_table_info_get_f dnx2_submodule_table_info_get;
} dnx_data_if_module_testing_submodule_family_dnx2_t;




#endif 


#ifdef INCLUDE_XFLOW_MACSEC


typedef struct
{
    
    uint32 table_flags_value;
} dnx_data_module_testing_compileflags_submodule_standard_table_flags_t;


#endif 


typedef enum
{
#ifdef INCLUDE_XFLOW_MACSEC

    
    dnx_data_module_testing_compileflags_submodule_standard_feature_flags,

#endif 

    
    _dnx_data_module_testing_compileflags_submodule_standard_feature_nof
} dnx_data_module_testing_compileflags_submodule_standard_feature_e;



typedef int(
    *dnx_data_module_testing_compileflags_submodule_standard_feature_get_f) (
    int unit,
    dnx_data_module_testing_compileflags_submodule_standard_feature_e feature);

#ifdef INCLUDE_XFLOW_MACSEC


typedef uint32(
    *dnx_data_module_testing_compileflags_submodule_standard_define_flags_get_f) (
    int unit);

#endif 
#ifdef INCLUDE_XFLOW_MACSEC


typedef uint32(
    *dnx_data_module_testing_compileflags_submodule_standard_numeric_flags_get_f) (
    int unit);

#endif 
#ifdef INCLUDE_XFLOW_MACSEC


typedef const dnx_data_module_testing_compileflags_submodule_standard_table_flags_t *(
    *dnx_data_module_testing_compileflags_submodule_standard_table_flags_get_f) (
    int unit,
    int table_flags_key);

#endif 


typedef struct
{
    
    dnx_data_module_testing_compileflags_submodule_standard_feature_get_f feature_get;
#ifdef INCLUDE_XFLOW_MACSEC

    
    dnx_data_module_testing_compileflags_submodule_standard_define_flags_get_f define_flags_get;

#endif 
#ifdef INCLUDE_XFLOW_MACSEC

    
    dnx_data_module_testing_compileflags_submodule_standard_numeric_flags_get_f numeric_flags_get;

#endif 
#ifdef INCLUDE_XFLOW_MACSEC

    
    dnx_data_module_testing_compileflags_submodule_standard_table_flags_get_f table_flags_get;

#endif 
#ifdef INCLUDE_XFLOW_MACSEC

    
    dnxc_data_table_info_get_f table_flags_info_get;

#endif 
} dnx_data_if_module_testing_compileflags_submodule_standard_t;



#ifdef INCLUDE_XFLOW_MACSEC




typedef struct
{
    
    uint32 table_no_flags_value;
} dnx_data_module_testing_compileflags_submodule_flags_table_no_flags_t;

#ifdef BCM_WARM_BOOT_SUPPORT


typedef struct
{
    
    uint32 table_flag_value;
} dnx_data_module_testing_compileflags_submodule_flags_table_flags_t;


#endif 


typedef enum
{
    
    dnx_data_module_testing_compileflags_submodule_flags_feature_no_flags,
#ifdef BCM_WARM_BOOT_SUPPORT

    
    dnx_data_module_testing_compileflags_submodule_flags_feature_flags,

#endif 

    
    _dnx_data_module_testing_compileflags_submodule_flags_feature_nof
} dnx_data_module_testing_compileflags_submodule_flags_feature_e;



typedef int(
    *dnx_data_module_testing_compileflags_submodule_flags_feature_get_f) (
    int unit,
    dnx_data_module_testing_compileflags_submodule_flags_feature_e feature);


typedef uint32(
    *dnx_data_module_testing_compileflags_submodule_flags_define_no_flags_get_f) (
    int unit);

#ifdef BCM_WARM_BOOT_SUPPORT


typedef uint32(
    *dnx_data_module_testing_compileflags_submodule_flags_define_flags_get_f) (
    int unit);

#endif 

typedef uint32(
    *dnx_data_module_testing_compileflags_submodule_flags_numeric_no_flags_get_f) (
    int unit);

#ifdef BCM_WARM_BOOT_SUPPORT


typedef uint32(
    *dnx_data_module_testing_compileflags_submodule_flags_numeric_flags_get_f) (
    int unit);

#endif 

typedef const dnx_data_module_testing_compileflags_submodule_flags_table_no_flags_t *(
    *dnx_data_module_testing_compileflags_submodule_flags_table_no_flags_get_f) (
    int unit,
    int table_no_flags_key);

#ifdef BCM_WARM_BOOT_SUPPORT


typedef const dnx_data_module_testing_compileflags_submodule_flags_table_flags_t *(
    *dnx_data_module_testing_compileflags_submodule_flags_table_flags_get_f) (
    int unit,
    int table_flag_key);

#endif 


typedef struct
{
    
    dnx_data_module_testing_compileflags_submodule_flags_feature_get_f feature_get;
    
    dnx_data_module_testing_compileflags_submodule_flags_define_no_flags_get_f define_no_flags_get;
#ifdef BCM_WARM_BOOT_SUPPORT

    
    dnx_data_module_testing_compileflags_submodule_flags_define_flags_get_f define_flags_get;

#endif 
    
    dnx_data_module_testing_compileflags_submodule_flags_numeric_no_flags_get_f numeric_no_flags_get;
#ifdef BCM_WARM_BOOT_SUPPORT

    
    dnx_data_module_testing_compileflags_submodule_flags_numeric_flags_get_f numeric_flags_get;

#endif 
    
    dnx_data_module_testing_compileflags_submodule_flags_table_no_flags_get_f table_no_flags_get;
    
    dnxc_data_table_info_get_f table_no_flags_info_get;
#ifdef BCM_WARM_BOOT_SUPPORT

    
    dnx_data_module_testing_compileflags_submodule_flags_table_flags_get_f table_flags_get;

#endif 
#ifdef BCM_WARM_BOOT_SUPPORT

    
    dnxc_data_table_info_get_f table_flags_info_get;

#endif 
} dnx_data_if_module_testing_compileflags_submodule_flags_t;




#endif 


typedef struct
{
    
    dnx_data_if_module_testing_unsupported_t unsupported;
    
    dnx_data_if_module_testing_standard_submodule_t standard_submodule;
    
    dnx_data_if_module_testing_types_t types;
    
    dnx_data_if_module_testing_property_methods_t property_methods;
    
    dnx_data_if_module_testing_data_pointers_t data_pointers;
    
    dnx_data_if_module_testing_dbal_t dbal;
    
    dnx_data_if_module_testing_example_tests_t example_tests;
#ifdef BCM_DNX2_SUPPORT

    
    dnx_data_if_module_testing_submodule_family_dnx2_t submodule_family_dnx2;

#endif 
    
    dnx_data_if_module_testing_compileflags_submodule_standard_t compileflags_submodule_standard;
#ifdef INCLUDE_XFLOW_MACSEC

    
    dnx_data_if_module_testing_compileflags_submodule_flags_t compileflags_submodule_flags;

#endif 
} dnx_data_if_module_testing_t;




extern dnx_data_if_module_testing_t dnx_data_module_testing;


#endif 

