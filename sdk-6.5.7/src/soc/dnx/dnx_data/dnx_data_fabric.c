/** \file dnx_data_fabric.c
 * 
 * MODULE DEVICE DATA - FABRIC
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
#define BSL_LOG_MODULE BSL_LS_BCMDNX_FABRIC
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
#include <soc/dnx/dnx_data/dnx_data_internal_fabric.h>
/*
 * }
 */

/*
 * Extern per device attach function
 */
extern shr_error_e jer2_a0_data_fabric_attach(
    int unit);
/*
 * SUBMODULE - LINKS:
 * {
 */
/*
 * links init
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
dnx_data_fabric_links_init(
    int unit,
    dnx_data_submodule_t *submodule_data)
{
    SHR_FUNC_INIT_VARS(unit);

    submodule_data->name = "links";
    submodule_data->doc = "data about fabric links";
    /*
     * Features
     */
    submodule_data->nof_features = dnx_data_fabric_links_feature_nof;
    submodule_data->features = sal_alloc(sizeof(dnx_data_feature_t) * submodule_data->nof_features, "_dnx_data fabric links features");
    if (submodule_data->features == NULL)
    {
        SHR_ERR_EXIT(_SHR_E_MEMORY, "failed to allocate dnx_data fabric_links features\n");
    }
    sal_memset(submodule_data->features, 0, sizeof(dnx_data_feature_t) * submodule_data->nof_features);

    /*
     * Defines
     */
    submodule_data->nof_defines = dnx_data_fabric_links_define_nof;
    submodule_data->defines = sal_alloc(sizeof(dnx_data_define_t) * submodule_data->nof_defines, "_dnx_data fabric links defines");
    if (submodule_data->defines == NULL)
    {
        SHR_ERR_EXIT(_SHR_E_MEMORY, "failed to allocate dnx_data fabric_links defines\n");
    }
    sal_memset(submodule_data->defines, 0, sizeof(dnx_data_define_t) * submodule_data->nof_defines);

    submodule_data->defines[dnx_data_fabric_links_define_nof_links].name = "nof_links";
    submodule_data->defines[dnx_data_fabric_links_define_nof_links].doc = "nof of fabric links";
    /* Set data type flag */
    submodule_data->defines[dnx_data_fabric_links_define_nof_links].flags |= DNX_DATA_F_DEFINE;

    /*
     * Tables
     */
    submodule_data->nof_tables = dnx_data_fabric_links_table_nof;
    submodule_data->tables = sal_alloc(sizeof(dnx_data_table_t) * submodule_data->nof_tables, "_dnx_data fabric links tables");
    if (submodule_data->tables == NULL)
    {
        SHR_ERR_EXIT(_SHR_E_MEMORY, "failed to allocate dnx_data fabric_links tables\n");
    }
    sal_memset(submodule_data->tables, 0, sizeof(dnx_data_table_t) * submodule_data->nof_tables);


exit:
    SHR_FUNC_EXIT;
}

/*
 * links features
 */
int
dnx_data_fabric_links_feature_get(
    int unit,
    dnx_data_fabric_links_feature_e feature)
{
    return dnx_data_mgmt_feature_data_get(unit, dnx_data_module_fabric, dnx_data_fabric_submodule_links, feature);
}

/*
 * links defines
 */
uint32
dnx_data_fabric_links_nof_links_get(
    int unit)
{
    return dnx_data_mgmt_define_data_get(unit, dnx_data_module_fabric, dnx_data_fabric_submodule_links, dnx_data_fabric_links_define_nof_links);
}

/*
 * links tables
 */
/* Table Get */
/* Get value as str */
/*
 * }
 */

/*
 * SUBMODULE - THRESHOLDS:
 * {
 */
/*
 * thresholds init
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
dnx_data_fabric_thresholds_init(
    int unit,
    dnx_data_submodule_t *submodule_data)
{
    SHR_FUNC_INIT_VARS(unit);

    submodule_data->name = "thresholds";
    submodule_data->doc = "default values for threholds";
    /*
     * Features
     */
    submodule_data->nof_features = dnx_data_fabric_thresholds_feature_nof;
    submodule_data->features = sal_alloc(sizeof(dnx_data_feature_t) * submodule_data->nof_features, "_dnx_data fabric thresholds features");
    if (submodule_data->features == NULL)
    {
        SHR_ERR_EXIT(_SHR_E_MEMORY, "failed to allocate dnx_data fabric_thresholds features\n");
    }
    sal_memset(submodule_data->features, 0, sizeof(dnx_data_feature_t) * submodule_data->nof_features);

    submodule_data->features[dnx_data_fabric_thresholds_severity_supported].name = "severity_supported";
    submodule_data->features[dnx_data_fabric_thresholds_severity_supported].doc = "Does threshold per severity supported?";
    submodule_data->features[dnx_data_fabric_thresholds_severity_supported].flags |= DNX_DATA_F_FEATURE;

    submodule_data->features[dnx_data_fabric_thresholds_mode_supported].name = "mode_supported";
    submodule_data->features[dnx_data_fabric_thresholds_mode_supported].doc = "Does threshold per mode supported?";
    submodule_data->features[dnx_data_fabric_thresholds_mode_supported].flags |= DNX_DATA_F_FEATURE;

    /*
     * Defines
     */
    submodule_data->nof_defines = dnx_data_fabric_thresholds_define_nof;
    submodule_data->defines = sal_alloc(sizeof(dnx_data_define_t) * submodule_data->nof_defines, "_dnx_data fabric thresholds defines");
    if (submodule_data->defines == NULL)
    {
        SHR_ERR_EXIT(_SHR_E_MEMORY, "failed to allocate dnx_data fabric_thresholds defines\n");
    }
    sal_memset(submodule_data->defines, 0, sizeof(dnx_data_define_t) * submodule_data->nof_defines);

    submodule_data->defines[dnx_data_fabric_thresholds_define_default_severity].name = "default_severity";
    submodule_data->defines[dnx_data_fabric_thresholds_define_default_severity].doc = "default severity for thresholds";
    /* Set data type flag */
    submodule_data->defines[dnx_data_fabric_thresholds_define_default_severity].flags |= DNX_DATA_F_DEFINE;

    submodule_data->defines[dnx_data_fabric_thresholds_define_default_mode].name = "default_mode";
    submodule_data->defines[dnx_data_fabric_thresholds_define_default_mode].doc = "default mode for thresholds";
    /* Set data type flag */
    submodule_data->defines[dnx_data_fabric_thresholds_define_default_mode].flags |= DNX_DATA_F_DEFINE;

    /*
     * Tables
     */
    submodule_data->nof_tables = dnx_data_fabric_thresholds_table_nof;
    submodule_data->tables = sal_alloc(sizeof(dnx_data_table_t) * submodule_data->nof_tables, "_dnx_data fabric thresholds tables");
    if (submodule_data->tables == NULL)
    {
        SHR_ERR_EXIT(_SHR_E_MEMORY, "failed to allocate dnx_data fabric_thresholds tables\n");
    }
    sal_memset(submodule_data->tables, 0, sizeof(dnx_data_table_t) * submodule_data->nof_tables);

    /*
     * Table - dch_default
     */
    submodule_data->tables[dnx_data_fabric_thresholds_table_dch_default].name = "dch_default";
    submodule_data->tables[dnx_data_fabric_thresholds_table_dch_default].doc = "set of threholds";
    submodule_data->tables[dnx_data_fabric_thresholds_table_dch_default].flags |= DNX_DATA_F_TABLE;
    submodule_data->tables[dnx_data_fabric_thresholds_table_dch_default].size_of_values = sizeof(dnx_data_fabric_thresholds_dch_default_t);
    submodule_data->tables[dnx_data_fabric_thresholds_table_dch_default].entry_get = dnx_data_fabric_thresholds_dch_default_entry_str_get;

    /* Keys */
    submodule_data->tables[dnx_data_fabric_thresholds_table_dch_default].nof_keys = 2;
    submodule_data->tables[dnx_data_fabric_thresholds_table_dch_default].keys[0].name = "severity";
    submodule_data->tables[dnx_data_fabric_thresholds_table_dch_default].keys[0].doc = "severity thresholds";
    submodule_data->tables[dnx_data_fabric_thresholds_table_dch_default].keys[1].name = "mode";
    submodule_data->tables[dnx_data_fabric_thresholds_table_dch_default].keys[1].doc = "mode of threholds according to fabric pipes";

    /* Values */
    submodule_data->tables[dnx_data_fabric_thresholds_table_dch_default].nof_values = 6;
    submodule_data->tables[dnx_data_fabric_thresholds_table_dch_default].values[0].name = "gci0";
    submodule_data->tables[dnx_data_fabric_thresholds_table_dch_default].values[0].type = "uint32";
    submodule_data->tables[dnx_data_fabric_thresholds_table_dch_default].values[0].doc = "gci th level 0";
    submodule_data->tables[dnx_data_fabric_thresholds_table_dch_default].values[0].offset = UTILEX_OFFSETOF(dnx_data_fabric_thresholds_dch_default_t, gci0);
    submodule_data->tables[dnx_data_fabric_thresholds_table_dch_default].values[1].name = "gci1";
    submodule_data->tables[dnx_data_fabric_thresholds_table_dch_default].values[1].type = "uint32";
    submodule_data->tables[dnx_data_fabric_thresholds_table_dch_default].values[1].doc = "gci th level 1";
    submodule_data->tables[dnx_data_fabric_thresholds_table_dch_default].values[1].offset = UTILEX_OFFSETOF(dnx_data_fabric_thresholds_dch_default_t, gci1);
    submodule_data->tables[dnx_data_fabric_thresholds_table_dch_default].values[2].name = "gci2";
    submodule_data->tables[dnx_data_fabric_thresholds_table_dch_default].values[2].type = "uint32";
    submodule_data->tables[dnx_data_fabric_thresholds_table_dch_default].values[2].doc = "gci th level 2";
    submodule_data->tables[dnx_data_fabric_thresholds_table_dch_default].values[2].offset = UTILEX_OFFSETOF(dnx_data_fabric_thresholds_dch_default_t, gci2);
    submodule_data->tables[dnx_data_fabric_thresholds_table_dch_default].values[3].name = "rci0";
    submodule_data->tables[dnx_data_fabric_thresholds_table_dch_default].values[3].type = "uint32";
    submodule_data->tables[dnx_data_fabric_thresholds_table_dch_default].values[3].doc = "rci th level 0";
    submodule_data->tables[dnx_data_fabric_thresholds_table_dch_default].values[3].offset = UTILEX_OFFSETOF(dnx_data_fabric_thresholds_dch_default_t, rci0);
    submodule_data->tables[dnx_data_fabric_thresholds_table_dch_default].values[4].name = "rci1";
    submodule_data->tables[dnx_data_fabric_thresholds_table_dch_default].values[4].type = "uint32";
    submodule_data->tables[dnx_data_fabric_thresholds_table_dch_default].values[4].doc = "rci th level 1";
    submodule_data->tables[dnx_data_fabric_thresholds_table_dch_default].values[4].offset = UTILEX_OFFSETOF(dnx_data_fabric_thresholds_dch_default_t, rci1);
    submodule_data->tables[dnx_data_fabric_thresholds_table_dch_default].values[5].name = "rci2";
    submodule_data->tables[dnx_data_fabric_thresholds_table_dch_default].values[5].type = "uint32";
    submodule_data->tables[dnx_data_fabric_thresholds_table_dch_default].values[5].doc = "rci th level 2";
    submodule_data->tables[dnx_data_fabric_thresholds_table_dch_default].values[5].offset = UTILEX_OFFSETOF(dnx_data_fabric_thresholds_dch_default_t, rci2);

    /* Allocate data buffer */
    submodule_data->tables[dnx_data_fabric_thresholds_table_dch_default].data = sal_alloc(sizeof(dnx_data_fabric_thresholds_dch_default_t) * (1 * DNX_DATA_MAX_FABRIC_THRESHOLDS_DCH_DEFAULT_SEVERITY * DNX_DATA_MAX_FABRIC_THRESHOLDS_DCH_DEFAULT_MODE + 1 /* to store default value */ ), "data of dnx_data_fabric_thresholds_table_dch_default");
    if (submodule_data->tables[dnx_data_fabric_thresholds_table_dch_default].data == NULL)
    {
        SHR_ERR_EXIT(_SHR_E_MEMORY, "failed to allocate data of dnx_data_fabric_thresholds_table_dch_default\n");
    }
    sal_memset(submodule_data->tables[dnx_data_fabric_thresholds_table_dch_default].data, 0, sizeof(dnx_data_fabric_thresholds_dch_default_t) * (1 * DNX_DATA_MAX_FABRIC_THRESHOLDS_DCH_DEFAULT_SEVERITY * DNX_DATA_MAX_FABRIC_THRESHOLDS_DCH_DEFAULT_MODE + 1 /* to store default value */ ));


exit:
    SHR_FUNC_EXIT;
}

/*
 * thresholds features
 */
int
dnx_data_fabric_thresholds_feature_get(
    int unit,
    dnx_data_fabric_thresholds_feature_e feature)
{
    return dnx_data_mgmt_feature_data_get(unit, dnx_data_module_fabric, dnx_data_fabric_submodule_thresholds, feature);
}

/*
 * thresholds defines
 */
uint32
dnx_data_fabric_thresholds_default_severity_get(
    int unit)
{
    return dnx_data_mgmt_define_data_get(unit, dnx_data_module_fabric, dnx_data_fabric_submodule_thresholds, dnx_data_fabric_thresholds_define_default_severity);
}

uint32
dnx_data_fabric_thresholds_default_mode_get(
    int unit)
{
    return dnx_data_mgmt_define_data_get(unit, dnx_data_module_fabric, dnx_data_fabric_submodule_thresholds, dnx_data_fabric_thresholds_define_default_mode);
}

/*
 * thresholds tables
 */
/* Table Get */
const dnx_data_fabric_thresholds_dch_default_t *
dnx_data_fabric_thresholds_dch_default_get(
    int unit,
    int severity,
    int mode)
{
    char *data;
    dnx_data_table_t *table;

    /* Get table */
    table = dnx_data_mgmt_table_get(unit, dnx_data_module_fabric, dnx_data_fabric_submodule_thresholds, dnx_data_fabric_thresholds_table_dch_default);
    /* Get data */
    data = dnx_data_mgmt_table_data_get(unit, table, severity, mode);
    return (const dnx_data_fabric_thresholds_dch_default_t *) data;

}

/* Get value as str */
shr_error_e
dnx_data_fabric_thresholds_dch_default_entry_str_get(
    int unit,
    char *buffer,
    int key0,
    int key1,
    int value_index)
{
    const dnx_data_fabric_thresholds_dch_default_t *data;
    SHR_FUNC_INIT_VARS(unit);

    data = dnx_data_fabric_thresholds_dch_default_get(unit, key0, key1);
    switch (value_index)
    {
        case 0:
            sal_sprintf(buffer, "%d", data->gci0);
            break;
        case 1:
            sal_sprintf(buffer, "%d", data->gci1);
            break;
        case 2:
            sal_sprintf(buffer, "%d", data->gci2);
            break;
        case 3:
            sal_sprintf(buffer, "%d", data->rci0);
            break;
        case 4:
            sal_sprintf(buffer, "%d", data->rci1);
            break;
        case 5:
            sal_sprintf(buffer, "%d", data->rci2);
            break;
    }

    SHR_FUNC_EXIT;
}

/*
 * }
 */

/*
 * SUBMODULE - CELL:
 * {
 */
/*
 * cell init
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
dnx_data_fabric_cell_init(
    int unit,
    dnx_data_submodule_t *submodule_data)
{
    SHR_FUNC_INIT_VARS(unit);

    submodule_data->name = "cell";
    submodule_data->doc = "cell attributes";
    /*
     * Features
     */
    submodule_data->nof_features = dnx_data_fabric_cell_feature_nof;
    submodule_data->features = sal_alloc(sizeof(dnx_data_feature_t) * submodule_data->nof_features, "_dnx_data fabric cell features");
    if (submodule_data->features == NULL)
    {
        SHR_ERR_EXIT(_SHR_E_MEMORY, "failed to allocate dnx_data fabric_cell features\n");
    }
    sal_memset(submodule_data->features, 0, sizeof(dnx_data_feature_t) * submodule_data->nof_features);

    /*
     * Defines
     */
    submodule_data->nof_defines = dnx_data_fabric_cell_define_nof;
    submodule_data->defines = sal_alloc(sizeof(dnx_data_define_t) * submodule_data->nof_defines, "_dnx_data fabric cell defines");
    if (submodule_data->defines == NULL)
    {
        SHR_ERR_EXIT(_SHR_E_MEMORY, "failed to allocate dnx_data fabric_cell defines\n");
    }
    sal_memset(submodule_data->defines, 0, sizeof(dnx_data_define_t) * submodule_data->nof_defines);

    submodule_data->defines[dnx_data_fabric_cell_define_nof_priorities].name = "nof_priorities";
    submodule_data->defines[dnx_data_fabric_cell_define_nof_priorities].doc = "nof of supported fabric priorities";
    /* Set data type flag */
    submodule_data->defines[dnx_data_fabric_cell_define_nof_priorities].flags |= DNX_DATA_F_DEFINE;

    /*
     * Tables
     */
    submodule_data->nof_tables = dnx_data_fabric_cell_table_nof;
    submodule_data->tables = sal_alloc(sizeof(dnx_data_table_t) * submodule_data->nof_tables, "_dnx_data fabric cell tables");
    if (submodule_data->tables == NULL)
    {
        SHR_ERR_EXIT(_SHR_E_MEMORY, "failed to allocate dnx_data fabric_cell tables\n");
    }
    sal_memset(submodule_data->tables, 0, sizeof(dnx_data_table_t) * submodule_data->nof_tables);

    /*
     * Table - pcp
     */
    submodule_data->tables[dnx_data_fabric_cell_table_pcp].name = "pcp";
    submodule_data->tables[dnx_data_fabric_cell_table_pcp].doc = "packet cell packing";
    submodule_data->tables[dnx_data_fabric_cell_table_pcp].flags |= DNX_DATA_F_TABLE;
    submodule_data->tables[dnx_data_fabric_cell_table_pcp].size_of_values = sizeof(dnx_data_fabric_cell_pcp_t);
    submodule_data->tables[dnx_data_fabric_cell_table_pcp].entry_get = dnx_data_fabric_cell_pcp_entry_str_get;

    /* Keys */
    submodule_data->tables[dnx_data_fabric_cell_table_pcp].nof_keys = 0;

    /* Values */
    submodule_data->tables[dnx_data_fabric_cell_table_pcp].nof_values = 1;
    submodule_data->tables[dnx_data_fabric_cell_table_pcp].values[0].name = "enable";
    submodule_data->tables[dnx_data_fabric_cell_table_pcp].values[0].type = "uint32";
    submodule_data->tables[dnx_data_fabric_cell_table_pcp].values[0].doc = "enable/disable packet cell packing";
    submodule_data->tables[dnx_data_fabric_cell_table_pcp].values[0].offset = UTILEX_OFFSETOF(dnx_data_fabric_cell_pcp_t, enable);

    /* Allocate data buffer */
    submodule_data->tables[dnx_data_fabric_cell_table_pcp].data = sal_alloc(sizeof(dnx_data_fabric_cell_pcp_t) * (1 + 1 /* to store default value */ ), "data of dnx_data_fabric_cell_table_pcp");
    if (submodule_data->tables[dnx_data_fabric_cell_table_pcp].data == NULL)
    {
        SHR_ERR_EXIT(_SHR_E_MEMORY, "failed to allocate data of dnx_data_fabric_cell_table_pcp\n");
    }
    sal_memset(submodule_data->tables[dnx_data_fabric_cell_table_pcp].data, 0, sizeof(dnx_data_fabric_cell_pcp_t) * (1 + 1 /* to store default value */ ));


exit:
    SHR_FUNC_EXIT;
}

/*
 * cell features
 */
int
dnx_data_fabric_cell_feature_get(
    int unit,
    dnx_data_fabric_cell_feature_e feature)
{
    return dnx_data_mgmt_feature_data_get(unit, dnx_data_module_fabric, dnx_data_fabric_submodule_cell, feature);
}

/*
 * cell defines
 */
uint32
dnx_data_fabric_cell_nof_priorities_get(
    int unit)
{
    return dnx_data_mgmt_define_data_get(unit, dnx_data_module_fabric, dnx_data_fabric_submodule_cell, dnx_data_fabric_cell_define_nof_priorities);
}

/*
 * cell tables
 */
/* Table Get */
const dnx_data_fabric_cell_pcp_t *
dnx_data_fabric_cell_pcp_get(
    int unit)
{
    char *data;
    dnx_data_table_t *table;

    /* Get table */
    table = dnx_data_mgmt_table_get(unit, dnx_data_module_fabric, dnx_data_fabric_submodule_cell, dnx_data_fabric_cell_table_pcp);
    /* Get data */
    data = dnx_data_mgmt_table_data_get(unit, table, 0, 0);
    return (const dnx_data_fabric_cell_pcp_t *) data;

}

/* Get value as str */
shr_error_e
dnx_data_fabric_cell_pcp_entry_str_get(
    int unit,
    char *buffer,
    int key0,
    int key1,
    int value_index)
{
    const dnx_data_fabric_cell_pcp_t *data;
    SHR_FUNC_INIT_VARS(unit);

    data = dnx_data_fabric_cell_pcp_get(unit);
    switch (value_index)
    {
        case 0:
            sal_sprintf(buffer, "%d", data->enable);
            break;
    }

    SHR_FUNC_EXIT;
}

/*
 * }
 */

/*
 * SUBMODULE - PIPES:
 * {
 */
/*
 * pipes init
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
dnx_data_fabric_pipes_init(
    int unit,
    dnx_data_submodule_t *submodule_data)
{
    SHR_FUNC_INIT_VARS(unit);

    submodule_data->name = "pipes";
    submodule_data->doc = "pipes attributes";
    /*
     * Features
     */
    submodule_data->nof_features = dnx_data_fabric_pipes_feature_nof;
    submodule_data->features = sal_alloc(sizeof(dnx_data_feature_t) * submodule_data->nof_features, "_dnx_data fabric pipes features");
    if (submodule_data->features == NULL)
    {
        SHR_ERR_EXIT(_SHR_E_MEMORY, "failed to allocate dnx_data fabric_pipes features\n");
    }
    sal_memset(submodule_data->features, 0, sizeof(dnx_data_feature_t) * submodule_data->nof_features);

    /*
     * Defines
     */
    submodule_data->nof_defines = dnx_data_fabric_pipes_define_nof;
    submodule_data->defines = sal_alloc(sizeof(dnx_data_define_t) * submodule_data->nof_defines, "_dnx_data fabric pipes defines");
    if (submodule_data->defines == NULL)
    {
        SHR_ERR_EXIT(_SHR_E_MEMORY, "failed to allocate dnx_data fabric_pipes defines\n");
    }
    sal_memset(submodule_data->defines, 0, sizeof(dnx_data_define_t) * submodule_data->nof_defines);

    submodule_data->defines[dnx_data_fabric_pipes_define_max_nof_pipes].name = "max_nof_pipes";
    submodule_data->defines[dnx_data_fabric_pipes_define_max_nof_pipes].doc = "max supported fabric pipes";
    /* Set data type flag */
    submodule_data->defines[dnx_data_fabric_pipes_define_max_nof_pipes].flags |= DNX_DATA_F_DEFINE;

    /*
     * Tables
     */
    submodule_data->nof_tables = dnx_data_fabric_pipes_table_nof;
    submodule_data->tables = sal_alloc(sizeof(dnx_data_table_t) * submodule_data->nof_tables, "_dnx_data fabric pipes tables");
    if (submodule_data->tables == NULL)
    {
        SHR_ERR_EXIT(_SHR_E_MEMORY, "failed to allocate dnx_data fabric_pipes tables\n");
    }
    sal_memset(submodule_data->tables, 0, sizeof(dnx_data_table_t) * submodule_data->nof_tables);

    /*
     * Table - nof_pipes
     */
    submodule_data->tables[dnx_data_fabric_pipes_table_nof_pipes].name = "nof_pipes";
    submodule_data->tables[dnx_data_fabric_pipes_table_nof_pipes].doc = "number of actual (configured) pipes";
    submodule_data->tables[dnx_data_fabric_pipes_table_nof_pipes].flags |= DNX_DATA_F_TABLE;
    submodule_data->tables[dnx_data_fabric_pipes_table_nof_pipes].size_of_values = sizeof(dnx_data_fabric_pipes_nof_pipes_t);
    submodule_data->tables[dnx_data_fabric_pipes_table_nof_pipes].entry_get = dnx_data_fabric_pipes_nof_pipes_entry_str_get;

    /* Keys */
    submodule_data->tables[dnx_data_fabric_pipes_table_nof_pipes].nof_keys = 0;

    /* Values */
    submodule_data->tables[dnx_data_fabric_pipes_table_nof_pipes].nof_values = 1;
    submodule_data->tables[dnx_data_fabric_pipes_table_nof_pipes].values[0].name = "val";
    submodule_data->tables[dnx_data_fabric_pipes_table_nof_pipes].values[0].type = "uint32";
    submodule_data->tables[dnx_data_fabric_pipes_table_nof_pipes].values[0].doc = "see table doc";
    submodule_data->tables[dnx_data_fabric_pipes_table_nof_pipes].values[0].offset = UTILEX_OFFSETOF(dnx_data_fabric_pipes_nof_pipes_t, val);

    /* Allocate data buffer */
    submodule_data->tables[dnx_data_fabric_pipes_table_nof_pipes].data = sal_alloc(sizeof(dnx_data_fabric_pipes_nof_pipes_t) * (1 + 1 /* to store default value */ ), "data of dnx_data_fabric_pipes_table_nof_pipes");
    if (submodule_data->tables[dnx_data_fabric_pipes_table_nof_pipes].data == NULL)
    {
        SHR_ERR_EXIT(_SHR_E_MEMORY, "failed to allocate data of dnx_data_fabric_pipes_table_nof_pipes\n");
    }
    sal_memset(submodule_data->tables[dnx_data_fabric_pipes_table_nof_pipes].data, 0, sizeof(dnx_data_fabric_pipes_nof_pipes_t) * (1 + 1 /* to store default value */ ));

    /*
     * Table - map
     */
    submodule_data->tables[dnx_data_fabric_pipes_table_map].name = "map";
    submodule_data->tables[dnx_data_fabric_pipes_table_map].doc = "map traffic cast X priority to fabric pipe";
    submodule_data->tables[dnx_data_fabric_pipes_table_map].flags |= DNX_DATA_F_TABLE;
    submodule_data->tables[dnx_data_fabric_pipes_table_map].size_of_values = sizeof(dnx_data_fabric_pipes_map_t);
    submodule_data->tables[dnx_data_fabric_pipes_table_map].entry_get = dnx_data_fabric_pipes_map_entry_str_get;

    /* Keys */
    submodule_data->tables[dnx_data_fabric_pipes_table_map].nof_keys = 0;

    /* Values */
    submodule_data->tables[dnx_data_fabric_pipes_table_map].nof_values = 2;
    submodule_data->tables[dnx_data_fabric_pipes_table_map].values[0].name = "uc";
    submodule_data->tables[dnx_data_fabric_pipes_table_map].values[0].type = "uint32[DNX_DATA_MAX_FABRIC_CELL_NOF_PRIORITIES]";
    submodule_data->tables[dnx_data_fabric_pipes_table_map].values[0].doc = "each element represents uc priority pipe";
    submodule_data->tables[dnx_data_fabric_pipes_table_map].values[0].offset = UTILEX_OFFSETOF(dnx_data_fabric_pipes_map_t, uc);
    submodule_data->tables[dnx_data_fabric_pipes_table_map].values[1].name = "mc";
    submodule_data->tables[dnx_data_fabric_pipes_table_map].values[1].type = "uint32[DNX_DATA_MAX_FABRIC_CELL_NOF_PRIORITIES]";
    submodule_data->tables[dnx_data_fabric_pipes_table_map].values[1].doc = "each element represents mc priority pipe";
    submodule_data->tables[dnx_data_fabric_pipes_table_map].values[1].offset = UTILEX_OFFSETOF(dnx_data_fabric_pipes_map_t, mc);

    /* Allocate data buffer */
    submodule_data->tables[dnx_data_fabric_pipes_table_map].data = sal_alloc(sizeof(dnx_data_fabric_pipes_map_t) * (1 + 1 /* to store default value */ ), "data of dnx_data_fabric_pipes_table_map");
    if (submodule_data->tables[dnx_data_fabric_pipes_table_map].data == NULL)
    {
        SHR_ERR_EXIT(_SHR_E_MEMORY, "failed to allocate data of dnx_data_fabric_pipes_table_map\n");
    }
    sal_memset(submodule_data->tables[dnx_data_fabric_pipes_table_map].data, 0, sizeof(dnx_data_fabric_pipes_map_t) * (1 + 1 /* to store default value */ ));

    /*
     * Table - system
     */
    submodule_data->tables[dnx_data_fabric_pipes_table_system].name = "system";
    submodule_data->tables[dnx_data_fabric_pipes_table_system].doc = "system attributes";
    submodule_data->tables[dnx_data_fabric_pipes_table_system].flags |= DNX_DATA_F_TABLE;
    submodule_data->tables[dnx_data_fabric_pipes_table_system].size_of_values = sizeof(dnx_data_fabric_pipes_system_t);
    submodule_data->tables[dnx_data_fabric_pipes_table_system].entry_get = dnx_data_fabric_pipes_system_entry_str_get;

    /* Keys */
    submodule_data->tables[dnx_data_fabric_pipes_table_system].nof_keys = 0;

    /* Values */
    submodule_data->tables[dnx_data_fabric_pipes_table_system].nof_values = 1;
    submodule_data->tables[dnx_data_fabric_pipes_table_system].values[0].name = "multi_pipe";
    submodule_data->tables[dnx_data_fabric_pipes_table_system].values[0].type = "uint32";
    submodule_data->tables[dnx_data_fabric_pipes_table_system].values[0].doc = "set to 1 if there is a device in the system in multi pipes mode";
    submodule_data->tables[dnx_data_fabric_pipes_table_system].values[0].offset = UTILEX_OFFSETOF(dnx_data_fabric_pipes_system_t, multi_pipe);

    /* Allocate data buffer */
    submodule_data->tables[dnx_data_fabric_pipes_table_system].data = sal_alloc(sizeof(dnx_data_fabric_pipes_system_t) * (1 + 1 /* to store default value */ ), "data of dnx_data_fabric_pipes_table_system");
    if (submodule_data->tables[dnx_data_fabric_pipes_table_system].data == NULL)
    {
        SHR_ERR_EXIT(_SHR_E_MEMORY, "failed to allocate data of dnx_data_fabric_pipes_table_system\n");
    }
    sal_memset(submodule_data->tables[dnx_data_fabric_pipes_table_system].data, 0, sizeof(dnx_data_fabric_pipes_system_t) * (1 + 1 /* to store default value */ ));


exit:
    SHR_FUNC_EXIT;
}

/*
 * pipes features
 */
int
dnx_data_fabric_pipes_feature_get(
    int unit,
    dnx_data_fabric_pipes_feature_e feature)
{
    return dnx_data_mgmt_feature_data_get(unit, dnx_data_module_fabric, dnx_data_fabric_submodule_pipes, feature);
}

/*
 * pipes defines
 */
uint32
dnx_data_fabric_pipes_max_nof_pipes_get(
    int unit)
{
    return dnx_data_mgmt_define_data_get(unit, dnx_data_module_fabric, dnx_data_fabric_submodule_pipes, dnx_data_fabric_pipes_define_max_nof_pipes);
}

/*
 * pipes tables
 */
/* Table Get */
const dnx_data_fabric_pipes_nof_pipes_t *
dnx_data_fabric_pipes_nof_pipes_get(
    int unit)
{
    char *data;
    dnx_data_table_t *table;

    /* Get table */
    table = dnx_data_mgmt_table_get(unit, dnx_data_module_fabric, dnx_data_fabric_submodule_pipes, dnx_data_fabric_pipes_table_nof_pipes);
    /* Get data */
    data = dnx_data_mgmt_table_data_get(unit, table, 0, 0);
    return (const dnx_data_fabric_pipes_nof_pipes_t *) data;

}

const dnx_data_fabric_pipes_map_t *
dnx_data_fabric_pipes_map_get(
    int unit)
{
    char *data;
    dnx_data_table_t *table;

    /* Get table */
    table = dnx_data_mgmt_table_get(unit, dnx_data_module_fabric, dnx_data_fabric_submodule_pipes, dnx_data_fabric_pipes_table_map);
    /* Get data */
    data = dnx_data_mgmt_table_data_get(unit, table, 0, 0);
    return (const dnx_data_fabric_pipes_map_t *) data;

}

const dnx_data_fabric_pipes_system_t *
dnx_data_fabric_pipes_system_get(
    int unit)
{
    char *data;
    dnx_data_table_t *table;

    /* Get table */
    table = dnx_data_mgmt_table_get(unit, dnx_data_module_fabric, dnx_data_fabric_submodule_pipes, dnx_data_fabric_pipes_table_system);
    /* Get data */
    data = dnx_data_mgmt_table_data_get(unit, table, 0, 0);
    return (const dnx_data_fabric_pipes_system_t *) data;

}

/* Get value as str */
shr_error_e
dnx_data_fabric_pipes_nof_pipes_entry_str_get(
    int unit,
    char *buffer,
    int key0,
    int key1,
    int value_index)
{
    const dnx_data_fabric_pipes_nof_pipes_t *data;
    SHR_FUNC_INIT_VARS(unit);

    data = dnx_data_fabric_pipes_nof_pipes_get(unit);
    switch (value_index)
    {
        case 0:
            sal_sprintf(buffer, "%d", data->val);
            break;
    }

    SHR_FUNC_EXIT;
}

shr_error_e
dnx_data_fabric_pipes_map_entry_str_get(
    int unit,
    char *buffer,
    int key0,
    int key1,
    int value_index)
{
    const dnx_data_fabric_pipes_map_t *data;
    SHR_FUNC_INIT_VARS(unit);

    data = dnx_data_fabric_pipes_map_get(unit);
    switch (value_index)
    {
        case 0:
            DNX_DATA_MGMT_ARR_STR(buffer, DNX_DATA_MAX_FABRIC_CELL_NOF_PRIORITIES, data->uc);
            break;
        case 1:
            DNX_DATA_MGMT_ARR_STR(buffer, DNX_DATA_MAX_FABRIC_CELL_NOF_PRIORITIES, data->mc);
            break;
    }

    SHR_FUNC_EXIT;
}

shr_error_e
dnx_data_fabric_pipes_system_entry_str_get(
    int unit,
    char *buffer,
    int key0,
    int key1,
    int value_index)
{
    const dnx_data_fabric_pipes_system_t *data;
    SHR_FUNC_INIT_VARS(unit);

    data = dnx_data_fabric_pipes_system_get(unit);
    switch (value_index)
    {
        case 0:
            sal_sprintf(buffer, "%d", data->multi_pipe);
            break;
    }

    SHR_FUNC_EXIT;
}

/*
 * }
 */

shr_error_e
dnx_data_fabric_init(
    int unit,
    dnx_data_module_t *module_data)
{
    SHR_FUNC_INIT_VARS(unit);

    /* Data Struct Init */
    module_data->name = "fabric";
    module_data->nof_submodules = dnx_data_fabric_submodule_nof;
    module_data->submodules = sal_alloc(sizeof(dnx_data_submodule_t) * module_data->nof_submodules, "_dnx_data fabric submodules");
    if (module_data->submodules == NULL)
    {
        SHR_ERR_EXIT(_SHR_E_MEMORY, "failed to allocate dnx_data fabric submodules\n");
    }
    sal_memset(module_data->submodules, 0, sizeof(dnx_data_submodule_t) * module_data->nof_submodules);

    /*
     * Init Submodule data
     */
    SHR_IF_ERR_EXIT(dnx_data_fabric_links_init(unit, &module_data->submodules[dnx_data_fabric_submodule_links]));
    SHR_IF_ERR_EXIT(dnx_data_fabric_thresholds_init(unit, &module_data->submodules[dnx_data_fabric_submodule_thresholds]));
    SHR_IF_ERR_EXIT(dnx_data_fabric_cell_init(unit, &module_data->submodules[dnx_data_fabric_submodule_cell]));
    SHR_IF_ERR_EXIT(dnx_data_fabric_pipes_init(unit, &module_data->submodules[dnx_data_fabric_submodule_pipes]));
    /*
     * Attach device module
     */
    if (SOC_IS_JERICHO_2_A0(unit))
    {
        SHR_IF_ERR_EXIT(jer2_a0_data_fabric_attach(unit));
    }

exit:
    SHR_FUNC_EXIT;
}
/* *INDENT-ON* */
