/** \file bcm_int/dnx/port/port_signal_quality.h
 *
 * Port signal quality functions
 *
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2022 Broadcom Inc. All rights reserved.
 */

#ifndef DNX_PORT_SIGNAL_QUALITY_INCLUDED_H
/*
 * {
 */
#define DNX_PORT_SIGNAL_QUALITY_INCLUDED_H

#ifndef BCM_DNX_SUPPORT
#error "This file is for use by DNX family only!"
#endif

/*
 * Include files.
 * {
 */
#include <soc/sand/shrextend/shrextend_error.h>

/*
 * }
 */

/*
 * DEFINES
 * {
 */

/*
 * }
 */

/**
 * \brief - Initialize module
 *
 * \param [in] unit - chip unit id
 *
 * \return
 *   int - see _SHR_E_*
 *
 * \remarks
 *   * None
 * \see
 *   * None
 */
shr_error_e dnx_port_signal_quality_er_init(
    int unit);

/**
 * \brief - Verify port and type
 *
 * \param [in] unit - chip unit id
 * \param [in] port - logical port
 * \param [in] type - port control type
 *
 * \return
 *   int - see _SHR_E_*
 *
 * \remarks
 *   * None
 * \see
 *   * None
 */
shr_error_e dnx_port_signal_quality_er_control_port_and_type_verify(
    int unit,
    bcm_port_t port,
    bcm_port_control_t type);

/**
 * \brief - Get number of symbol errors per frame
 * \param [in] unit - unit number
 * \param [in] link_id - LINK ID
 * \param [out] symbol_error_counter  - watermark
 * \return
 *   shr_error_e
 */
shr_error_e dnx_port_signal_quality_fabric_symbol_error_info_get(
    int unit,
    int link_id,
    uint32 *symbol_error_counter);

/**
 * \brief - Configure the Signal quality er controls
 *
 * \param [in] unit - chip unit id
 * \param [in] port - logical port
 * \param [in] type - port control type
 * \param [in] value - configured values
 *
 * \return
 *   int - see _SHR_E_*
 *
 * \remarks
 *   * None
 * \see
 *   * None
 */
shr_error_e dnx_port_signal_quality_er_control_set(
    int unit,
    bcm_port_t port,
    bcm_port_control_t type,
    int value);

/**
 * \brief -
 *  Get various features at the port level.
 *
 * \param [in] unit - the relevant unit.
 * \param [in] port - Device or logical port number.
 * \param [in] type - Port feature enumerator, see /bcm_port_control_t
 * \param [in] *value_p - Value of the bit field in port table
 *
 * \return
 *   shr_error_e
 *
 * \remark
 *   * None
 * \see
 *   bcm_dnx_port_control_set
 */
shr_error_e dnx_port_signal_quality_er_control_get(
    int unit,
    bcm_port_t port,
    bcm_port_control_t type,
    int *value_p);

/**
 *  * \brief -
 *  Get the last calculated error rate on a specific port
 *
 * \param [in] unit
 * \param [in] port
 * \param [out] value_p
 *
 * \return
 *   shr_error_e
 *
 * \remark
 *   * None
 * \see
 *   * None
 */
shr_error_e dnx_port_signal_quality_last_er_error_rate_get(
    int unit,
    bcm_port_t port,
    uint32 *value_p);

/**
 *  * \brief -
 *  Clear state of all enabled ports to remove stale information
 *
 * \param [in] unit
 *
 * \return
 *   bcm_error_t
 *
 * \remark
 *   * None
 * \see
 *   * None
 */
shr_error_e dnx_port_signal_quality_er_state_db_clear(
    int unit);

/*
 * }
 */
#endif /** DNX_PORT_SIGNAL_QUALITY_INCLUDED_H */
