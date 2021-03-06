/** \file jer2_a0_data_cmodel.c
 * 
 * DEVICE DATA - CMODEL
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
#define BSL_LOG_MODULE BSL_LS_BCMDNX_TX
/**
 * \brief
 * Mark this file as device data internal file
 */
#define DNX_DATA_INTERNAL
/*
 * INCLUDE FILES:
 * {
 */
#include <soc/dnx/dnx_data/dnx_data_internal_cmodel.h>
#include <soc/dnx/dnx_data/dnx_data_property.h>
/*
 * }
 */

/*
 * FUNCTIONS:
 * {
 */
/*
 * Submodule: tx
 */

/*
 * Features
 */
/*
 * Defines
 */
/**
 * \brief device level function which set numeric loopback_enable
 * numeric info:
 * Use loopback in the cmodel server instead of go through the pipe when receiving tx packets
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
jer2_a0_dnx_data_cmodel_tx_loopback_enable_set(
    int unit)
{
    dnx_data_define_t *define;
    int module_index = dnx_data_module_cmodel;
    int submodule_index = dnx_data_cmodel_submodule_tx;
    int define_index = dnx_data_cmodel_tx_define_loopback_enable;
    SHR_FUNC_INIT_VARS(unit);

    define = &_dnx_data[unit].modules[module_index].submodules[submodule_index].defines[define_index];
    /* Set default value */
    define->default_data = 0;

    /* Set value */
    define->data = 0;

    /* Set data flags as supported */
    define->flags |= DNX_DATA_F_SUPPORTED;
    /* Property */
    define->property.name = "cmodel_loopback_enable";
    define->property.doc = 
        "\n"
        "Use loopback in the cmodel server when receiving packets.\n"
        "Default: 0\n"
        "\n"
    ;
    define->property.method = dnx_data_property_method_enable;
    define->property.method_str = "enable";
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
jer2_a0_data_cmodel_attach(
    int unit)
{
    dnx_data_module_t *module = NULL;
    dnx_data_submodule_t *submodule = NULL;
    dnx_data_define_t *define = NULL;
    dnx_data_feature_t *feature = NULL;
    dnx_data_table_t *table = NULL;
    int module_index = dnx_data_module_cmodel;
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
     * Attach submodule: tx
     */
    submodule_index = dnx_data_cmodel_submodule_tx;
    submodule = &module->submodules[submodule_index];

    /*
     * Attach defines: 
     */
    data_index = dnx_data_cmodel_tx_define_loopback_enable;
    define = &submodule->defines[data_index];
    define->set = jer2_a0_dnx_data_cmodel_tx_loopback_enable_set;

    /*
     * Attach features: 
     */

    /*
     * Attach tables: 
     */

    SHR_FUNC_EXIT;
}
/* *INDENT-ON* */
