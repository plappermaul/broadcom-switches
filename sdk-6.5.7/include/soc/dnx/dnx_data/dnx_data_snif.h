/** \file dnx_data_snif.h
 * 
 * MODULE DATA INTERFACE - 
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
#ifndef _DNX_DATA_SNIF_H_
/*{*/
#define _DNX_DATA_SNIF_H_
/*
 * INCLUDE FILES:
 * {
 */
#include <soc/dnx/dnx_data/dnx_data_max_snif.h>
/*
 * }
 */

/*!
* \brief This file is only used by DNX (JR2 family). Including it by
* software that is not specific to DNX is an error.
*/
#ifndef BCM_DNX_SUPPORT
#error "This file is for use by DNX (JR2) family only!"
#endif

/*
 * MODULE FUNCTIONS:
 * {
 */
/**
 * \brief Init default data structure - dnx_data_if_snif
 * 
 * \par DIRECT INPUT:
 *   \param [in] unit - Unit #
 * \par INDIRECT INPUT:
 *   * None
 * \par DIRECT OUTPUT:
 *     rv - see 'shr_error_e
 * \par INDIRECT OUTPUT:
 * \remark
 *   * None
 * \see
 *   * None
 */
shr_error_e dnx_data_if_snif_init(
    int unit);

/*
 * }
 */

/*
 * SUBMODULE  - INGRESS:
 * SNIF profiles data
 * {
 */
/*
 * Table value structure
 */
/*
 * Feature enum
 */
/**
 * \brief 
 * Submodule features
 */
typedef enum
{

    /**
     * Must be last one!
     */
    dnx_data_snif_ingress_feature_nof
} dnx_data_snif_ingress_feature_e;

/*
 * Submodule functions typedefs
 */
/**
 * \brief get submodule feauture - disabled/enabled
 * 
 * \par DIRECT INPUT:
 *   \param [in] unit - Unit #
 *   \param [in] feature - feature to get (see enum defintion for more details
 * \par INDIRECT INPUT:
 *   device data - module global data
 * \par DIRECT OUTPUT:
 *     is_supported - returns 1 iff supported and 0 otherwise
 * \par INDIRECT OUTPUT:
 * \remark
 *   * None
 * \see
 *   * None
 */
typedef int(
    *dnx_data_snif_ingress_feature_get_f) (
    int unit,
    dnx_data_snif_ingress_feature_e feature);

/**
 * \brief returns define data of nof_profiles
 * Module - 'snif', Submodule - 'ingress', data - 'nof_profiles'
 * Number of SNIF proflies supported by ingress SNIF
 * 
 * \par DIRECT INPUT:
 *   \param [in] unit - unit #
 * \par INDIRECT INPUT:
 *   _dnx_data[unit] - module global data
 * \par DIRECT OUTPUT:
 *     nof_profiles - the required data value
 * \par INDIRECT OUTPUT:
 * \remark
 *   * None
 * \see
 *   * None
 */
typedef uint32(
    *dnx_data_snif_ingress_nof_profiles_get_f) (
    int unit);

/*
 * SUBMODULE INTERFACE DNX_DATA_IF_SNIF - INGRESS:
 * {
 */
/**
 * \brief Interface for snif ingress data
 */
typedef struct
{
    /**
     * get submodule feauture - disabled/enabled
     */
    dnx_data_snif_ingress_feature_get_f feature_get;
    /**
     * returns define data of nof_profiles
     */
    dnx_data_snif_ingress_nof_profiles_get_f nof_profiles_get;
} dnx_data_if_snif_ingress_t;

/*
 * }
 */

/*
 * MODULE INTERFACE DNX_DATA_IF_SNIF:
 * {
 */
/**
 * \brief Interface for snif data
 */
typedef struct
{
    /**
     * Interface for snif ingress data
     */
    dnx_data_if_snif_ingress_t ingress;
} dnx_data_if_snif_t;

/*
 * }
 */
/*
 * Extern:
 * {
 */
/**
 * \brief
 * Module structured interface - used to get the required data stored in dnx data
 */
extern dnx_data_if_snif_t dnx_data_snif;
/*
 * }
 */
/*}*/
#else     /*DNX_DATA_SNIF_H_*/
/*{*/
#ifndef DNX_DATA_INTERNAL
/*{*/
#error "dnx_data_snif file might include any header files, therfore header files must not include it.\nAny include to dnx_data_snif must be from 'c' file"
/*}*/
#endif /*DNX_DATA_INTERNAL*/
/*}*/
#endif /*_DNX_DATA_SNIF_H_*/
/* *INDENT-ON* */
