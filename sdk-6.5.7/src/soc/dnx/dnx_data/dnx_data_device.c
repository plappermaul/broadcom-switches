/** \file dnx_data_device.c
 * 
 * MODULE DEVICE DATA - DEVICE
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
#define BSL_LOG_MODULE BSL_LS_BCMDNX_DEVICE
/**
 * \brief
 * Mark this file as device data internal file
 */
#define DNX_DATA_INTERNAL
/*
 * INCLUDE FILES:
 * {
 */
#include <soc/dnx/dnx_data/dnx_data.h>
#include <soc/dnx/dnx_data/dnx_data_internal.h>
#include <soc/dnx/dnx_data/dnx_data_internal_device.h>
/*
 * }
 */

/*
 * Extern per device attach function
 */
extern shr_error_e jer2_a0_data_device_attach(
    int unit);
/*
 * SUBMODULE - GENERAL:
 * {
 */
/*
 * general init
 */
/**
 * \brief Set submodule data to default value per device type
 * 
 * \par DIRECT INPUT:
 *   \param [in] unit - Unit #
 * \par INDIRECT INPUT:
 *   * None
 * \par DIRECT OUTPUT:
 *     rv - see 'shr_error_e
 * \par INDIRECT OUTPUT:
 *   \param [out] submodule_data - pointer to submodule data
 * \remark
 *   * None
 * \see
 *   * None
 */
static shr_error_e
dnx_data_device_general_init(
    int unit,
    dnx_data_submodule_t *submodule_data)
{
    SHR_FUNC_INIT_VARS(unit);

    submodule_data->name = "general";
    submodule_data->doc = "General attributes";
    /*
     * Features
     */
    submodule_data->nof_features = dnx_data_device_general_feature_nof;
    submodule_data->features = sal_alloc(sizeof(dnx_data_feature_t) * submodule_data->nof_features, "_dnx_data device general features");
    if (submodule_data->features == NULL)
    {
        SHR_ERR_EXIT(_SHR_E_MEMORY, "failed to allocate dnx_data device_general features\n");
    }
    sal_memset(submodule_data->features, 0, sizeof(dnx_data_feature_t) * submodule_data->nof_features);

    /*
     * Defines
     */
    submodule_data->nof_defines = dnx_data_device_general_define_nof;
    submodule_data->defines = sal_alloc(sizeof(dnx_data_define_t) * submodule_data->nof_defines, "_dnx_data device general defines");
    if (submodule_data->defines == NULL)
    {
        SHR_ERR_EXIT(_SHR_E_MEMORY, "failed to allocate dnx_data device_general defines\n");
    }
    sal_memset(submodule_data->defines, 0, sizeof(dnx_data_define_t) * submodule_data->nof_defines);

    submodule_data->defines[dnx_data_device_general_define_nof_cores].name = "nof_cores";
    submodule_data->defines[dnx_data_device_general_define_nof_cores].doc = "Number of cores in the device.";
    /* Set data type flag */
    submodule_data->defines[dnx_data_device_general_define_nof_cores].flags |= DNX_DATA_F_DEFINE;

    /*
     * Tables
     */
    submodule_data->nof_tables = dnx_data_device_general_table_nof;
    submodule_data->tables = sal_alloc(sizeof(dnx_data_table_t) * submodule_data->nof_tables, "_dnx_data device general tables");
    if (submodule_data->tables == NULL)
    {
        SHR_ERR_EXIT(_SHR_E_MEMORY, "failed to allocate dnx_data device_general tables\n");
    }
    sal_memset(submodule_data->tables, 0, sizeof(dnx_data_table_t) * submodule_data->nof_tables);


exit:
    SHR_FUNC_EXIT;
}

/*
 * general features
 */
int
dnx_data_device_general_feature_get(
    int unit,
    dnx_data_device_general_feature_e feature)
{
    return dnx_data_mgmt_feature_data_get(unit, dnx_data_module_device, dnx_data_device_submodule_general, feature);
}

/*
 * general defines
 */
uint32
dnx_data_device_general_nof_cores_get(
    int unit)
{
    return dnx_data_mgmt_define_data_get(unit, dnx_data_module_device, dnx_data_device_submodule_general, dnx_data_device_general_define_nof_cores);
}

/*
 * general tables
 */
/* Table Get */
/* Get value as str */
/*
 * }
 */

shr_error_e
dnx_data_device_init(
    int unit,
    dnx_data_module_t *module_data)
{
    SHR_FUNC_INIT_VARS(unit);

    /* Data Struct Init */
    module_data->name = "device";
    module_data->nof_submodules = dnx_data_device_submodule_nof;
    module_data->submodules = sal_alloc(sizeof(dnx_data_submodule_t) * module_data->nof_submodules, "_dnx_data device submodules");
    if (module_data->submodules == NULL)
    {
        SHR_ERR_EXIT(_SHR_E_MEMORY, "failed to allocate dnx_data device submodules\n");
    }
    sal_memset(module_data->submodules, 0, sizeof(dnx_data_submodule_t) * module_data->nof_submodules);

    /*
     * Init Submodule data
     */
    SHR_IF_ERR_EXIT(dnx_data_device_general_init(unit, &module_data->submodules[dnx_data_device_submodule_general]));
    /*
     * Attach device module
     */
    if (SOC_IS_JERICHO_2_A0(unit))
    {
        SHR_IF_ERR_EXIT(jer2_a0_data_device_attach(unit));
    }

exit:
    SHR_FUNC_EXIT;
}
/* *INDENT-ON* */
