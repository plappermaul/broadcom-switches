/*
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated.
 * Edits to this file will be lost when it is regenerated.
 *
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

#ifndef BCMINT_LTSW_XFS_TIME_H
#define BCMINT_LTSW_XFS_TIME_H

#include <bcm/time.h>
#include <bcm/types.h>
#include <bcm/switch.h>

/*!
 * \brief Initialize time module.
 *
 * \param [in] unit Unit Number.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
xfs_ltsw_time_init(int unit);

/*!
 * \brief De-initializes time module.
 *
 * \param [in] unit Unit Number.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
xfs_ltsw_time_deinit(int unit);

/*!
 * \brief Set syncE clock output frequency beased on the input divisor settings.
 *
 * \param [in] unit Unit Number.
 * \param [in] clk_src SyncE clock source type.
 * \param [in] divisor SyncE divisor setting.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
xfs_ltsw_time_synce_clock_set(
    int unit,
    bcm_time_synce_clock_src_type_t clk_src,
    bcm_time_synce_divisor_setting_t *divisor);

/*!
 * \brief Get current syncE input divisor settings.
 *
 * \param [in] unit Unit Number.
 * \param [in] clk_src SyncE clock source type.
 * \param [out] divisor SyncE divisor setting.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
xfs_ltsw_time_synce_clock_get(
    int unit,
    bcm_time_synce_clock_src_type_t clk_src,
    bcm_time_synce_divisor_setting_t *divisor);

/*!
 * \brief Set Primary or Secondary SyncE Recovered clock.
 *
 * \param [in] unit Unit Number.
 * \param [in] clk_src_config SyncE clock source type.
 * \param [in] control SyncE divisor setting.
 * \param [in] value Value to set.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
xfs_ltsw_time_synce_clock_source_control_set(
    int unit,
    bcm_time_synce_clock_source_config_t *clk_src_config,
    bcm_time_synce_clock_source_control_t control,
    int value);

/*!
 * \brief Get Primary or Secondary SyncE Recovered clock.
 *
 * \param [in] unit Unit Number.
 * \param [in] clk_src_config SyncE clock source type.
 * \param [in] control SyncE divisor setting.
 * \param [out] value Value to set.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
xfs_ltsw_time_synce_clock_source_control_get(
    int unit,
    bcm_time_synce_clock_source_config_t *clk_src_config,
    bcm_time_synce_clock_source_control_t control,
    int *value);

/*!
 * \brief Set time of the day values at various stages in ASIC.
 *
 * \param [in] unit Unit Number.
 * \param [in] stages Time stamping stages.
 * \param [in] tod Structure for Time of the day values.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
xfs_ltsw_time_tod_set(
    int unit,
    uint32 stages,
    bcm_time_tod_t *tod);

/*!
 * \brief Get time of the day values at various stages in ASIC.
 *
 * \param [in] unit Unit Number.
 * \param [in] stages Time stamping stages.
 * \param [out] tod Structure for Time of the day values.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
xfs_ltsw_time_tod_get(
    int unit,
    uint32 stages,
    bcm_time_tod_t *tod);

/*!
 * \brief Set timesync configurations for the port.
 *
 * \param [in] unit Unit Number.
 * \param [in] port Port Number.
 * \param [in] config_count Input configuration size.
 * \param [in] config_array Timesync configuration array.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
xfs_ltsw_time_port_timesync_config_set(
    int unit,
    bcm_port_t port,
    int config_count,
    bcm_port_timesync_config_t *config_array);

/*!
 * \brief Get timesync configurations for the port.
 *
 * \param [in] unit Unit Number.
 * \param [in] port Port Number.
 * \param [in] config_count Input array size.
 * \param [out] config_array Timesync configuration array.
 * \param [out] array_count Fetched array size.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
xfs_ltsw_time_port_timesync_config_get(
    int unit,
    bcm_port_t port,
    int config_count,
    bcm_port_timesync_config_t *config_array,
    int *array_count);

/*!
 * \brief Specify general time switch behaviors.
 *
 * \param [in] unit Unit Number.
 * \param [in] type The desired configuration parameter to modify.
 * \param [in] arg Value to set.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
xfs_ltsw_time_switch_control_set(
    int unit,
    bcm_switch_control_t type,
    int arg);

/*!
 * \brief Retrieve general time switch behaviors.
 *
 * \param [in] unit Unit Number.
 * \param [in] type The desired configuration parameter to modify.
 * \param [out] arg Value to get.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
xfs_ltsw_time_switch_control_get(
    int unit,
    bcm_switch_control_t type,
    int *arg);

/*!
 * \brief Specify time port-specific and device-wide operating configuration.
 *
 * \param [in] unit Unit Number.
 * \param [in] port Port Number.
 * \param [in] type Switch control type.
 * \param [in] arg Value to set.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
xfs_ltsw_time_switch_control_port_set(
    int unit,
    bcm_port_t port,
    bcm_switch_control_t type,
    int arg);

/*!
 * \brief Retrieve time port-specific and device-wide operating configuration.
 *
 * \param [in] unit Unit Number.
 * \param [in] port Port Number.
 * \param [in] type Switch control type.
 * \param [out] arg Value to get.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
xfs_ltsw_time_switch_control_port_get(
    int unit,
    bcm_port_t port,
    bcm_switch_control_t type,
    int *arg);

#endif /* BCMINT_LTSW_XFS_TIME_H */
