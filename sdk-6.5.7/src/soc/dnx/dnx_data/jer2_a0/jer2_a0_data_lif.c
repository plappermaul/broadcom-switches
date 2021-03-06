/** \file jer2_a0_data_lif.c
 * 
 * DEVICE DATA - LIF
 * 
 * Device Data
 * SW component that maintains per device data
 * The data is static and won't be changed after device initialization.
 *     
 * Supported data types:
 *     - Define             - a 'uint32' number (a max value for all devices is maintained)
 *     - feature            - 1 bit per each feature (supported/not supported) - support soc properties 
 *     - table              - the data is accessed with keys and/or can maintain multiple values and/or set by soc property
 *     - numeric            - a 'uint32' number that support soc properties
 * 
 * User interface for DNX DATA component can be found in "dnx_data_if.h" and "dnx_data_if_#module#.h"
 * 
 * Adding the data is done via XMLs placed in "tools/autocoder/DeviceData/dnx/.." 
 * "How to" User Guide can be found in confluence. 
 *        
 *     
 * 
 * AUTO-GENERATED BY AUTOCODER!
 * DO NOT EDIT THIS FILE!
 */
/* *INDENT-OFF* */
/*
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 */
#ifdef BSL_LOG_MODULE
#error "BSL_LOG_MODULE redefined"
#endif
#define BSL_LOG_MODULE BSL_LS_BCMDNX_LIF
/**
 * \brief
 * Mark this file as device data internal file
 */
#define DNX_DATA_INTERNAL
/*
 * INCLUDE FILES:
 * {
 */
#include <soc/dnx/dnx_data/dnx_data_internal_lif.h>
#include <soc/dnx/dnx_data/dnx_data_property.h>
/*
 * }
 */

/*
 * FUNCTIONS:
 * {
 */
/*
 * Submodule: global_lif
 */

/*
 * Features
 */
/*
 * Defines
 */
/**
 * \brief device level function which set define nof_global_in_lifs
 * define info:
 * Number of in global lifs in the device.
 * 
 * \par DIRECT INPUT:
 *   \param [in] unit - Unit #
 * \par INDIRECT INPUT:
 *   * None
 * \par DIRECT OUTPUT:
 *     err - see shr_error_e
 * \par INDIRECT OUTPUT:
 * \remark
 *   * None
 * \see
 *   * None
 */
static shr_error_e
jer2_a0_dnx_data_lif_global_lif_nof_global_in_lifs_set(
    int unit)
{
    dnx_data_define_t *define;
    int module_index = dnx_data_module_lif;
    int submodule_index = dnx_data_lif_submodule_global_lif;
    int define_index = dnx_data_lif_global_lif_define_nof_global_in_lifs;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnx_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    /* Set default value */
    define->default_data = 0x80000;

    /* Set value */
    define->data = 0x80000;

    /* Set data flags as supported */
    define->flags |= DNX_DATA_F_SUPPORTED;

    SHR_FUNC_EXIT;
}

/**
 * \brief device level function which set define nof_global_out_lifs
 * define info:
 * Number of out global lifs in the device.
 * 
 * \par DIRECT INPUT:
 *   \param [in] unit - Unit #
 * \par INDIRECT INPUT:
 *   * None
 * \par DIRECT OUTPUT:
 *     err - see shr_error_e
 * \par INDIRECT OUTPUT:
 * \remark
 *   * None
 * \see
 *   * None
 */
static shr_error_e
jer2_a0_dnx_data_lif_global_lif_nof_global_out_lifs_set(
    int unit)
{
    dnx_data_define_t *define;
    int module_index = dnx_data_module_lif;
    int submodule_index = dnx_data_lif_submodule_global_lif;
    int define_index = dnx_data_lif_global_lif_define_nof_global_out_lifs;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnx_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    /* Set default value */
    define->default_data = 0x80000;

    /* Set value */
    define->data = 0x80000;

    /* Set data flags as supported */
    define->flags |= DNX_DATA_F_SUPPORTED;

    SHR_FUNC_EXIT;
}

/**
 * \brief device level function which set numeric nof_glem_lines
 * numeric info:
 * Number of entries in the GLEM.                     This is the maximum number of egress global lifs in the system.
 * 
 * \par DIRECT INPUT:
 *   \param [in] unit - Unit #
 * \par INDIRECT INPUT:
 *   * None
 * \par DIRECT OUTPUT:
 *     err - see shr_error_e
 * \par INDIRECT OUTPUT:
 * \remark
 *   * None
 * \see
 *   * None
 */
static shr_error_e
jer2_a0_dnx_data_lif_global_lif_nof_glem_lines_set(
    int unit)
{
    dnx_data_define_t *define;
    int module_index = dnx_data_module_lif;
    int submodule_index = dnx_data_lif_submodule_global_lif;
    int define_index = dnx_data_lif_global_lif_define_nof_glem_lines;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnx_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    /* Set default value */
    define->default_data = 0x18000;

    /* Set value */
    define->data = 0x18000;

    /* Set data flags as supported */
    define->flags |= DNX_DATA_F_SUPPORTED;
    /* Property */
    define->property.name = "mdb_profile";
    define->property.doc = 
        "MDB profile ID presents the allocation of all the flexible databases in the device\n"
    ;
    define->property.method = dnx_data_property_method_direct_map;
    define->property.method_str = "direct_map";
    define->property.nof_mapping = 1;
    define->property.mapping = sal_alloc(sizeof(dnx_data_property_map_t) * define->property.nof_mapping, "dnx_data property mapping");
    if (define->property.mapping == NULL)
    {
        SHR_ERR_EXIT(_SHR_E_MEMORY, "failed to allocate properties mapping for define nof_glem_lines \n");
    }
    sal_memset(define->property.mapping, 0, sizeof(dnx_data_property_map_t) * define->property.nof_mapping);

    define->property.mapping[0].name = "0";
    define->property.mapping[0].val = 0x18000;
    SHR_IF_ERR_EXIT(dnx_data_mgmt_property_read(unit, &define->property, -1, &define->data));
    /* Set data flags as property */
    define->flags |= (DNX_DATA_F_PROPERTY);


exit:
    SHR_FUNC_EXIT;
}

/*
 * Tables
 */
/*
 * Submodule: in_lif
 */

/*
 * Features
 */
/*
 * Defines
 */
/**
 * \brief device level function which set numeric nof_local_in_lifs
 * numeric info:
 * Number of local inlifs in the device.
 * 
 * \par DIRECT INPUT:
 *   \param [in] unit - Unit #
 * \par INDIRECT INPUT:
 *   * None
 * \par DIRECT OUTPUT:
 *     err - see shr_error_e
 * \par INDIRECT OUTPUT:
 * \remark
 *   * None
 * \see
 *   * None
 */
static shr_error_e
jer2_a0_dnx_data_lif_in_lif_nof_local_in_lifs_set(
    int unit)
{
    dnx_data_define_t *define;
    int module_index = dnx_data_module_lif;
    int submodule_index = dnx_data_lif_submodule_in_lif;
    int define_index = dnx_data_lif_in_lif_define_nof_local_in_lifs;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnx_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    /* Set default value */
    define->default_data = 0x20000;

    /* Set value */
    define->data = 0x20000;

    /* Set data flags as supported */
    define->flags |= DNX_DATA_F_SUPPORTED;
    /* Property */
    define->property.name = "mdb_profile";
    define->property.doc = 
        "MDB profile ID presents the allocation of all the flexible databases in the device\n"
    ;
    define->property.method = dnx_data_property_method_direct_map;
    define->property.method_str = "direct_map";
    define->property.nof_mapping = 1;
    define->property.mapping = sal_alloc(sizeof(dnx_data_property_map_t) * define->property.nof_mapping, "dnx_data property mapping");
    if (define->property.mapping == NULL)
    {
        SHR_ERR_EXIT(_SHR_E_MEMORY, "failed to allocate properties mapping for define nof_local_in_lifs \n");
    }
    sal_memset(define->property.mapping, 0, sizeof(dnx_data_property_map_t) * define->property.nof_mapping);

    define->property.mapping[0].name = "0";
    define->property.mapping[0].val = 0x20000;
    SHR_IF_ERR_EXIT(dnx_data_mgmt_property_read(unit, &define->property, -1, &define->data));
    /* Set data flags as property */
    define->flags |= (DNX_DATA_F_PROPERTY);


exit:
    SHR_FUNC_EXIT;
}

/*
 * Tables
 */
/*
 * Submodule: out_lif
 */

/*
 * Features
 */
/*
 * Defines
 */
/**
 * \brief device level function which set numeric nof_local_out_lifs
 * numeric info:
 * Number of local outlifs in the device.
 * 
 * \par DIRECT INPUT:
 *   \param [in] unit - Unit #
 * \par INDIRECT INPUT:
 *   * None
 * \par DIRECT OUTPUT:
 *     err - see shr_error_e
 * \par INDIRECT OUTPUT:
 * \remark
 *   * None
 * \see
 *   * None
 */
static shr_error_e
jer2_a0_dnx_data_lif_out_lif_nof_local_out_lifs_set(
    int unit)
{
    dnx_data_define_t *define;
    int module_index = dnx_data_module_lif;
    int submodule_index = dnx_data_lif_submodule_out_lif;
    int define_index = dnx_data_lif_out_lif_define_nof_local_out_lifs;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnx_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    /* Set default value */
    define->default_data = 0x32000;

    /* Set value */
    define->data = 0x32000;

    /* Set data flags as supported */
    define->flags |= DNX_DATA_F_SUPPORTED;
    /* Property */
    define->property.name = "mdb_profile";
    define->property.doc = 
        "MDB profile ID presents the allocation of all the flexible databases in the device\n"
    ;
    define->property.method = dnx_data_property_method_direct_map;
    define->property.method_str = "direct_map";
    define->property.nof_mapping = 1;
    define->property.mapping = sal_alloc(sizeof(dnx_data_property_map_t) * define->property.nof_mapping, "dnx_data property mapping");
    if (define->property.mapping == NULL)
    {
        SHR_ERR_EXIT(_SHR_E_MEMORY, "failed to allocate properties mapping for define nof_local_out_lifs \n");
    }
    sal_memset(define->property.mapping, 0, sizeof(dnx_data_property_map_t) * define->property.nof_mapping);

    define->property.mapping[0].name = "0";
    define->property.mapping[0].val = 0x32000;
    SHR_IF_ERR_EXIT(dnx_data_mgmt_property_read(unit, &define->property, -1, &define->data));
    /* Set data flags as property */
    define->flags |= (DNX_DATA_F_PROPERTY);


exit:
    SHR_FUNC_EXIT;
}

/*
 * Tables
 */
/*
 * Device attach func
 */
/**
 * \brief Attach device to module - attach set function to data structure
 * 
 * \par DIRECT INPUT:
 *   \param [in] unit - Unit #
 * \par INDIRECT INPUT:
 *   * None
 * \par DIRECT OUTPUT:
 *     err - 
 * \par INDIRECT OUTPUT:
 * \remark
 *   * None
 * \see
 *   * None
 */
shr_error_e
jer2_a0_data_lif_attach(
    int unit)
{
    dnx_data_module_t *module = NULL;
    dnx_data_submodule_t *submodule = NULL;
    dnx_data_define_t *define = NULL;
    dnx_data_feature_t *feature = NULL;
    dnx_data_table_t *table = NULL;
    int module_index = dnx_data_module_lif;
    int submodule_index = -1, data_index = -1;
    SHR_FUNC_INIT_VARS(unit);

    COMPILER_REFERENCE(define);
    COMPILER_REFERENCE(feature);
    COMPILER_REFERENCE(table);
    COMPILER_REFERENCE(submodule);
    COMPILER_REFERENCE(data_index);
    COMPILER_REFERENCE(submodule_index);
    module = &_dnx_data[unit].modules[module_index];
    /*
     * Attach submodule: global_lif
     */
    submodule_index = dnx_data_lif_submodule_global_lif;
    submodule = &module->submodules[submodule_index];

    /*
     * Attach defines: 
     */
    data_index = dnx_data_lif_global_lif_define_nof_global_in_lifs;
    define = &submodule->defines[data_index];
    define->set = jer2_a0_dnx_data_lif_global_lif_nof_global_in_lifs_set;
    data_index = dnx_data_lif_global_lif_define_nof_global_out_lifs;
    define = &submodule->defines[data_index];
    define->set = jer2_a0_dnx_data_lif_global_lif_nof_global_out_lifs_set;
    data_index = dnx_data_lif_global_lif_define_nof_glem_lines;
    define = &submodule->defines[data_index];
    define->set = jer2_a0_dnx_data_lif_global_lif_nof_glem_lines_set;

    /*
     * Attach features: 
     */

    /*
     * Attach tables: 
     */
    /*
     * Attach submodule: in_lif
     */
    submodule_index = dnx_data_lif_submodule_in_lif;
    submodule = &module->submodules[submodule_index];

    /*
     * Attach defines: 
     */
    data_index = dnx_data_lif_in_lif_define_nof_local_in_lifs;
    define = &submodule->defines[data_index];
    define->set = jer2_a0_dnx_data_lif_in_lif_nof_local_in_lifs_set;

    /*
     * Attach features: 
     */

    /*
     * Attach tables: 
     */
    /*
     * Attach submodule: out_lif
     */
    submodule_index = dnx_data_lif_submodule_out_lif;
    submodule = &module->submodules[submodule_index];

    /*
     * Attach defines: 
     */
    data_index = dnx_data_lif_out_lif_define_nof_local_out_lifs;
    define = &submodule->defines[data_index];
    define->set = jer2_a0_dnx_data_lif_out_lif_nof_local_out_lifs_set;

    /*
     * Attach features: 
     */

    /*
     * Attach tables: 
     */

    SHR_FUNC_EXIT;
}
/* *INDENT-ON* */
