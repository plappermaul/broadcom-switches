/*
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated.
 * Edits to this file will be lost when it is regenerated.
 *
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

#ifndef BCMINT_LTSW_MBCM_ECN_H
#define BCMINT_LTSW_MBCM_ECN_H

#include <bcm/ecn.h>
#include <bcm/types.h>
#include <bcm/switch.h>

#include <sal/sal_types.h>

/*!
 * \brief Deinit ECN module.
 *
 * \param [in] unit Unit Number.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*ecn_deinit_f)(int unit);

/*!
 * \brief Init ECN module.
 *
 * \param [in] unit Unit Number.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*ecn_init_f)(int unit);

/*!
 * \brief Create MPLS ECN map.
 *
 * \param [in] unit Unit Number.
 * \param [in] flags flags.
 * \param [out] ecn_map_id ECN map ID.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*ecn_map_create_f)(
    int unit,
    uint32_t flags,
    int *ecn_map_id);

/*!
 * \brief Destroy MPLS ECN map.
 *
 * \param [in] unit Unit Number.
 * \param [in] ecn_map_id ECN map ID.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*ecn_map_destroy_f)(
    int unit,
    int ecn_map_id);

/*!
 * \brief Set MPLS ECN map.
 *
 * \param [in] unit Unit Number.
 * \param [in] options options, not used currently.
 * \param [in] ecn_map_id ECN map ID.
 * \param [in] ecn_map ECN mapping values.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*ecn_map_set_f)(
    int unit,
    uint32_t options,
    int ecn_map_id,
    bcm_ecn_map_t *ecn_map);

/*!
 * \brief Get MPLS ECN map.
 *
 * \param [in] unit Unit Number.
 * \param [in] ecn_map_id ECN map ID.
 * \param [inout] ecn_map ECN mapping values.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*ecn_map_get_f)(
    int unit,
    int ecn_map_id,
    bcm_ecn_map_t *ecn_map);

/*!
 * \brief To configure the value of responsive indication based on the IP protocol value.
 *
 * \param [in] unit Unit Number.
 * \param [in] ip_proto IP Protocol value.
 * \param [in] responsive Responsice indication.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*ecn_responsive_protocol_set_f)(
    int unit,
    uint8 ip_proto,
    int responsive);

/*!
 * \brief To get the value of responsive indication based on the IP protocol value.
 *
 * \param [in] unit Unit Number.
 * \param [in] ip_proto IP Protocol value.
 * \param [inout] responsive Responsice indication.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*ecn_responsive_protocol_get_f)(
    int unit,
    uint8 ip_proto,
    int *responsive);

/*!
 * \brief To get an ECN mapping info from a port.
 *
 * \param [in] unit Unit Number.
 * \param [in] port gport.
 * \param [inout] ecn_map ECN mapping values.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*ecn_port_map_get_f)(
    int unit,
    bcm_gport_t port,
    bcm_ecn_port_map_t *ecn_map);

/*!
 * \brief To set an ECN mapping info to a port.
 *
 * \param [in] unit Unit Number.
 * \param [in] port gport.
 * \param [inout] ecn_map ECN mapping values.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*ecn_port_map_set_f)(
    int unit,
    bcm_gport_t port,
    bcm_ecn_port_map_t *ecn_map);

/*!
 * \brief Set profile ID based on internal priority.
 *
 * \param [in] unit Unit Number.
 * \param [inout] profile Profile structure.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*ecn_map_profile_set_f)(
    int unit,
    bcm_ecn_map_profile_t *profile);

/*!
 * \brief Get profile ID based on internal priority.
 *
 * \param [in] unit Unit Number.
 * \param [inout] profile Profile structure.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*ecn_map_profile_get_f)(
    int unit,
    bcm_ecn_map_profile_t *profile);

/*!
 * \brief Set latency-based ECN mode.
 *
 * \param [in] unit Unit Number.
 * \param [in] mode mode.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*ecn_latency_based_mode_set_f)(
    int unit,
    bcm_switch_latency_ecn_mark_mode_t mode);

/*!
 * \brief Get latency-based ECN mode.
 *
 * \param [in] unit Unit Number.
 * \param [out] mode mode.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*ecn_latency_based_mode_get_f)(
    int unit,
    bcm_switch_latency_ecn_mark_mode_t *mode);

/*!
 * \brief Set latency-based ECN threshold.
 *
 * \param [in] unit Unit Number.
 * \param [in] gport Port number or GPORT identifier.
 * \param [in] cosq COS queue number.
 * \param [in] mode mode.
 * \param [in] threshold threshold.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*ecn_latency_based_threshold_set_f)(
    int unit,
    bcm_gport_t gport,
    bcm_cos_queue_t cosq,
    bcm_switch_latency_ecn_mark_mode_t mode,
    uint32_t threshold);

/*!
 * \brief Get latency-based ECN threshold.
 *
 * \param [in] unit Unit Number.
 * \param [in] gport Port number or GPORT identifier.
 * \param [in] cosq COS queue number.
 * \param [in] mode mode.
 * \param [out] threshold threshold.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*ecn_latency_based_threshold_get_f)(
    int unit,
    bcm_gport_t gport,
    bcm_cos_queue_t cosq,
    bcm_switch_latency_ecn_mark_mode_t mode,
    uint32_t *threshold);

/*!
 * \brief Resolve ECN map ID.
 *
 * \param [in] unit Unit Number.
 * \param [in] ecn_map_id ECN map ID.
 * \param [out] ecn_map_type ECN map type.
 * \param [out] prof_id ECN map profile ID.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*ecn_map_id_resolve_f)(
    int unit,
    int ecn_map_id,
    int *ecn_map_type,
    uint32_t *prof_id);

/*!
 * \brief Construct ECN map ID.
 *
 * \param [in] unit Unit Number.
 * \param [in] ecn_map_type ECN map type.
 * \param [in] prof_id ECN map profile ID.
 * \param [out] ecn_map_id ECN map ID.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*ecn_map_id_construct_f)(
    int unit,
    int ecn_map_type,
    uint32_t prof_id,
    int *ecn_map_id);

/*!
 * \brief To set the mapped internal congestion notification (int_cn) value.
 *
 * \param [in] unit Unit Number.
 * \param [in] map Internal congestion notification map.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*ecn_traffic_map_set_f)(
    int unit,
    bcm_ecn_traffic_map_info_t *map);

/*!
 * \brief To get the mapped internal congestion notification (int_cn) value.
 *
 * \param [in] unit Unit Number.
 * \param [inout] map Internal congestion notification map.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*ecn_traffic_map_get_f)(
    int unit,
    bcm_ecn_traffic_map_info_t *map);

/*!
 * \brief To set the ECN enqueue action.
 *
 * \param [in] unit Unit Number.
 * \param [in] ecn_config ECN traffic action configuration.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*ecn_action_enqueue_set_f)(
    int unit,
    bcm_ecn_traffic_action_config_t *ecn_config);

/*!
 * \brief To get the ECN enqueue action.
 *
 * \param [in] unit Unit Number.
 * \param [inout] ecn_config ECN traffic action configuration.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*ecn_action_enqueue_get_f)(
    int unit,
    bcm_ecn_traffic_action_config_t *ecn_config);

/*!
 * \brief To set the ECN dequeue action.
 *
 * \param [in] unit Unit Number.
 * \param [in] ecn_config ECN traffic action configuration.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*ecn_action_dequeue_set_f)(
    int unit,
    bcm_ecn_traffic_action_config_t *ecn_config);

/*!
 * \brief To get the ECN dequeue action.
 *
 * \param [in] unit Unit Number.
 * \param [inout] ecn_config ECN traffic action configuration.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*ecn_action_dequeue_get_f)(
    int unit,
    bcm_ecn_traffic_action_config_t *ecn_config);

/*!
 * \brief To set the ECN egress action.
 *
 * \param [in] unit Unit Number.
 * \param [in] ecn_config ECN traffic action configuration.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*ecn_action_egress_set_f)(
    int unit,
    bcm_ecn_traffic_action_config_t *ecn_config);

/*!
 * \brief To get the ECN egress action.
 *
 * \param [in] unit Unit Number.
 * \param [inout] ecn_config ECN traffic action configuration.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*ecn_action_egress_get_f)(
    int unit,
    bcm_ecn_traffic_action_config_t *ecn_config);

/*!
 * \brief To set the ECN egress action.
 *
 * \param [in] unit Unit Number.
 * \param [in] ecn_config ECN traffic action configuration.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*ecn_action_latency_dequeue_set_f)(
    int unit,
    bcm_ecn_traffic_action_config_t *ecn_config);

/*!
 * \brief To get the ECN egress action.
 *
 * \param [in] unit Unit Number.
 * \param [inout] ecn_config ECN traffic action configuration.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*ecn_action_latency_dequeue_get_f)(
    int unit,
    bcm_ecn_traffic_action_config_t *ecn_config);

/*!
 * \brief To set the default responsive value for Non-TCP packets.
 *
 * \param [in] unit Unit Number.
 * \param [in] value Default responsive value.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*ecn_responsive_default_set_f)(
    int unit,
    int value);

/*!
 * \brief To get the default responsive value for Non-TCP packets.
 *
 * \param [in] unit Unit Number.
 * \param [out] value Returned default responsive value.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*ecn_responsive_default_get_f)(
    int unit,
    int *value);

/*!
 * \brief Enable or disable one field ECN control table.
 *
 * \param [in] unit Unit Number.
 * \param [in] lt_name ecn_ctrl_name LT name.
 * \param [in] lt_fld_name ecn_ctrl_fld LT field name.
 * \param [in] val value set in the field.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*ecn_ctrl_set_f)(
    int unit,
    const char *lt_name,
    const char *lt_fld_name,
    int val);

/*!
 * \brief Get status ECN control table.
 *
 * \param [in] unit Unit Number.
 * \param [in] lt_name ecn_ctrl_name LT name.
 * \param [in] lt_fld_name ecn_ctrl_fld LT field name.
 * \param [in] val value get in the field.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*ecn_ctrl_get_f)(
    int unit,
    const char *lt_name,
    const char *lt_fld_name,
    int *val);

/*!
 * \brief Setup SFC QoS PHB with regard to ECN module.
 *
 * \param [in] unit Unit Number.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*ecn_sfc_qos_phb_setup_f)(int unit);

/*!
 * \brief Ecn driver structure.
 */
typedef struct mbcm_ltsw_ecn_drv_s {

    /*! Deinit ECN module. */
    ecn_deinit_f ecn_deinit;

    /*! Init ECN module. */
    ecn_init_f ecn_init;

    /*! Create MPLS ECN map. */
    ecn_map_create_f ecn_map_create;

    /*! Destroy MPLS ECN map. */
    ecn_map_destroy_f ecn_map_destroy;

    /*! Set MPLS ECN map. */
    ecn_map_set_f ecn_map_set;

    /*! Get MPLS ECN map. */
    ecn_map_get_f ecn_map_get;

    /*! To configure the value of responsive indication based on the IP protocol value. */
    ecn_responsive_protocol_set_f ecn_responsive_protocol_set;

    /*! To get the value of responsive indication based on the IP protocol value. */
    ecn_responsive_protocol_get_f ecn_responsive_protocol_get;

    /*! To get an ECN mapping info from a port. */
    ecn_port_map_get_f ecn_port_map_get;

    /*! To set an ECN mapping info to a port. */
    ecn_port_map_set_f ecn_port_map_set;

    /*! Set profile ID based on internal priority. */
    ecn_map_profile_set_f ecn_map_profile_set;

    /*! Get profile ID based on internal priority. */
    ecn_map_profile_get_f ecn_map_profile_get;

    /*! Set latency-based ECN mode. */
    ecn_latency_based_mode_set_f ecn_latency_based_mode_set;

    /*! Get latency-based ECN mode. */
    ecn_latency_based_mode_get_f ecn_latency_based_mode_get;

    /*! Set latency-based ECN threshold. */
    ecn_latency_based_threshold_set_f ecn_latency_based_threshold_set;

    /*! Get latency-based ECN threshold. */
    ecn_latency_based_threshold_get_f ecn_latency_based_threshold_get;

    /*! Resolve ECN map ID. */
    ecn_map_id_resolve_f ecn_map_id_resolve;

    /*! Construct ECN map ID. */
    ecn_map_id_construct_f ecn_map_id_construct;

    /*! To set the mapped internal congestion notification (int_cn) value. */
    ecn_traffic_map_set_f ecn_traffic_map_set;

    /*! To get the mapped internal congestion notification (int_cn) value. */
    ecn_traffic_map_get_f ecn_traffic_map_get;

    /*! To set the ECN enqueue action. */
    ecn_action_enqueue_set_f ecn_action_enqueue_set;

    /*! To get the ECN enqueue action. */
    ecn_action_enqueue_get_f ecn_action_enqueue_get;

    /*! To set the ECN dequeue action. */
    ecn_action_dequeue_set_f ecn_action_dequeue_set;

    /*! To get the ECN dequeue action. */
    ecn_action_dequeue_get_f ecn_action_dequeue_get;

    /*! To set the ECN egress action. */
    ecn_action_egress_set_f ecn_action_egress_set;

    /*! To get the ECN egress action. */
    ecn_action_egress_get_f ecn_action_egress_get;

    /*! To set the ECN egress action. */
    ecn_action_latency_dequeue_set_f ecn_action_latency_dequeue_set;

    /*! To get the ECN egress action. */
    ecn_action_latency_dequeue_get_f ecn_action_latency_dequeue_get;

    /*! To set the default responsive value for Non-TCP packets. */
    ecn_responsive_default_set_f ecn_responsive_default_set;

    /*! To get the default responsive value for Non-TCP packets. */
    ecn_responsive_default_get_f ecn_responsive_default_get;

    /*! Enable or disable one field ECN control table. */
    ecn_ctrl_set_f ecn_ctrl_set;

    /*! Get status ECN control table. */
    ecn_ctrl_get_f ecn_ctrl_get;

    /*! Setup SFC QoS PHB with regard to ECN module. */
    ecn_sfc_qos_phb_setup_f ecn_sfc_qos_phb_setup;

} mbcm_ltsw_ecn_drv_t;

/*!
 * \brief Set the ECN driver of the device.
 *
 * \param [in] unit Unit Number.
 * \param [in] drv Ecn driver to set.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_ecn_drv_set(
    int unit,
    mbcm_ltsw_ecn_drv_t *drv);

/*!
 * \brief Deinit ECN module.
 *
 * \param [in] unit Unit Number.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_ecn_deinit(int unit);

/*!
 * \brief Init ECN module.
 *
 * \param [in] unit Unit Number.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_ecn_init(int unit);

/*!
 * \brief Create MPLS ECN map.
 *
 * \param [in] unit Unit Number.
 * \param [in] flags flags.
 * \param [out] ecn_map_id ECN map ID.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_ecn_map_create(
    int unit,
    uint32_t flags,
    int *ecn_map_id);

/*!
 * \brief Destroy MPLS ECN map.
 *
 * \param [in] unit Unit Number.
 * \param [in] ecn_map_id ECN map ID.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_ecn_map_destroy(
    int unit,
    int ecn_map_id);

/*!
 * \brief Set MPLS ECN map.
 *
 * \param [in] unit Unit Number.
 * \param [in] options options, not used currently.
 * \param [in] ecn_map_id ECN map ID.
 * \param [in] ecn_map ECN mapping values.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_ecn_map_set(
    int unit,
    uint32_t options,
    int ecn_map_id,
    bcm_ecn_map_t *ecn_map);

/*!
 * \brief Get MPLS ECN map.
 *
 * \param [in] unit Unit Number.
 * \param [in] ecn_map_id ECN map ID.
 * \param [inout] ecn_map ECN mapping values.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_ecn_map_get(
    int unit,
    int ecn_map_id,
    bcm_ecn_map_t *ecn_map);

/*!
 * \brief To configure the value of responsive indication based on the IP protocol value.
 *
 * \param [in] unit Unit Number.
 * \param [in] ip_proto IP Protocol value.
 * \param [in] responsive Responsice indication.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_ecn_responsive_protocol_set(
    int unit,
    uint8 ip_proto,
    int responsive);

/*!
 * \brief To get the value of responsive indication based on the IP protocol value.
 *
 * \param [in] unit Unit Number.
 * \param [in] ip_proto IP Protocol value.
 * \param [inout] responsive Responsice indication.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_ecn_responsive_protocol_get(
    int unit,
    uint8 ip_proto,
    int *responsive);

/*!
 * \brief To get an ECN mapping info from a port.
 *
 * \param [in] unit Unit Number.
 * \param [in] port gport.
 * \param [inout] ecn_map ECN mapping values.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_ecn_port_map_get(
    int unit,
    bcm_gport_t port,
    bcm_ecn_port_map_t *ecn_map);

/*!
 * \brief To set an ECN mapping info to a port.
 *
 * \param [in] unit Unit Number.
 * \param [in] port gport.
 * \param [inout] ecn_map ECN mapping values.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_ecn_port_map_set(
    int unit,
    bcm_gport_t port,
    bcm_ecn_port_map_t *ecn_map);

/*!
 * \brief Set profile ID based on internal priority.
 *
 * \param [in] unit Unit Number.
 * \param [inout] profile Profile structure.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_ecn_map_profile_set(
    int unit,
    bcm_ecn_map_profile_t *profile);

/*!
 * \brief Get profile ID based on internal priority.
 *
 * \param [in] unit Unit Number.
 * \param [inout] profile Profile structure.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_ecn_map_profile_get(
    int unit,
    bcm_ecn_map_profile_t *profile);

/*!
 * \brief Set latency-based ECN mode.
 *
 * \param [in] unit Unit Number.
 * \param [in] mode mode.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_ecn_latency_based_mode_set(
    int unit,
    bcm_switch_latency_ecn_mark_mode_t mode);

/*!
 * \brief Get latency-based ECN mode.
 *
 * \param [in] unit Unit Number.
 * \param [out] mode mode.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_ecn_latency_based_mode_get(
    int unit,
    bcm_switch_latency_ecn_mark_mode_t *mode);

/*!
 * \brief Set latency-based ECN threshold.
 *
 * \param [in] unit Unit Number.
 * \param [in] gport Port number or GPORT identifier.
 * \param [in] cosq COS queue number.
 * \param [in] mode mode.
 * \param [in] threshold threshold.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_ecn_latency_based_threshold_set(
    int unit,
    bcm_gport_t gport,
    bcm_cos_queue_t cosq,
    bcm_switch_latency_ecn_mark_mode_t mode,
    uint32_t threshold);

/*!
 * \brief Get latency-based ECN threshold.
 *
 * \param [in] unit Unit Number.
 * \param [in] gport Port number or GPORT identifier.
 * \param [in] cosq COS queue number.
 * \param [in] mode mode.
 * \param [out] threshold threshold.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_ecn_latency_based_threshold_get(
    int unit,
    bcm_gport_t gport,
    bcm_cos_queue_t cosq,
    bcm_switch_latency_ecn_mark_mode_t mode,
    uint32_t *threshold);

/*!
 * \brief Resolve ECN map ID.
 *
 * \param [in] unit Unit Number.
 * \param [in] ecn_map_id ECN map ID.
 * \param [out] ecn_map_type ECN map type.
 * \param [out] prof_id ECN map profile ID.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_ecn_map_id_resolve(
    int unit,
    int ecn_map_id,
    int *ecn_map_type,
    uint32_t *prof_id);

/*!
 * \brief Construct ECN map ID.
 *
 * \param [in] unit Unit Number.
 * \param [in] ecn_map_type ECN map type.
 * \param [in] prof_id ECN map profile ID.
 * \param [out] ecn_map_id ECN map ID.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_ecn_map_id_construct(
    int unit,
    int ecn_map_type,
    uint32_t prof_id,
    int *ecn_map_id);

/*!
 * \brief To set the mapped internal congestion notification (int_cn) value.
 *
 * \param [in] unit Unit Number.
 * \param [in] map Internal congestion notification map.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_ecn_traffic_map_set(
    int unit,
    bcm_ecn_traffic_map_info_t *map);

/*!
 * \brief To get the mapped internal congestion notification (int_cn) value.
 *
 * \param [in] unit Unit Number.
 * \param [inout] map Internal congestion notification map.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_ecn_traffic_map_get(
    int unit,
    bcm_ecn_traffic_map_info_t *map);

/*!
 * \brief To set the ECN enqueue action.
 *
 * \param [in] unit Unit Number.
 * \param [in] ecn_config ECN traffic action configuration.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_ecn_action_enqueue_set(
    int unit,
    bcm_ecn_traffic_action_config_t *ecn_config);

/*!
 * \brief To get the ECN enqueue action.
 *
 * \param [in] unit Unit Number.
 * \param [inout] ecn_config ECN traffic action configuration.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_ecn_action_enqueue_get(
    int unit,
    bcm_ecn_traffic_action_config_t *ecn_config);

/*!
 * \brief To set the ECN dequeue action.
 *
 * \param [in] unit Unit Number.
 * \param [in] ecn_config ECN traffic action configuration.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_ecn_action_dequeue_set(
    int unit,
    bcm_ecn_traffic_action_config_t *ecn_config);

/*!
 * \brief To get the ECN dequeue action.
 *
 * \param [in] unit Unit Number.
 * \param [inout] ecn_config ECN traffic action configuration.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_ecn_action_dequeue_get(
    int unit,
    bcm_ecn_traffic_action_config_t *ecn_config);

/*!
 * \brief To set the ECN egress action.
 *
 * \param [in] unit Unit Number.
 * \param [in] ecn_config ECN traffic action configuration.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_ecn_action_egress_set(
    int unit,
    bcm_ecn_traffic_action_config_t *ecn_config);

/*!
 * \brief To get the ECN egress action.
 *
 * \param [in] unit Unit Number.
 * \param [inout] ecn_config ECN traffic action configuration.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_ecn_action_egress_get(
    int unit,
    bcm_ecn_traffic_action_config_t *ecn_config);

/*!
 * \brief To set the ECN egress action.
 *
 * \param [in] unit Unit Number.
 * \param [in] ecn_config ECN traffic action configuration.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_ecn_action_latency_dequeue_set(
    int unit,
    bcm_ecn_traffic_action_config_t *ecn_config);

/*!
 * \brief To get the ECN egress action.
 *
 * \param [in] unit Unit Number.
 * \param [inout] ecn_config ECN traffic action configuration.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_ecn_action_latency_dequeue_get(
    int unit,
    bcm_ecn_traffic_action_config_t *ecn_config);

/*!
 * \brief To set the default responsive value for Non-TCP packets.
 *
 * \param [in] unit Unit Number.
 * \param [in] value Default responsive value.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_ecn_responsive_default_set(
    int unit,
    int value);

/*!
 * \brief To get the default responsive value for Non-TCP packets.
 *
 * \param [in] unit Unit Number.
 * \param [out] value Returned default responsive value.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_ecn_responsive_default_get(
    int unit,
    int *value);

/*!
 * \brief Enable or disable one field ECN control table.
 *
 * \param [in] unit Unit Number.
 * \param [in] lt_name ecn_ctrl_name LT name.
 * \param [in] lt_fld_name ecn_ctrl_fld LT field name.
 * \param [in] val value set in the field.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_ecn_ctrl_set(
    int unit,
    const char *lt_name,
    const char *lt_fld_name,
    int val);

/*!
 * \brief Get status ECN control table.
 *
 * \param [in] unit Unit Number.
 * \param [in] lt_name ecn_ctrl_name LT name.
 * \param [in] lt_fld_name ecn_ctrl_fld LT field name.
 * \param [in] val value get in the field.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_ecn_ctrl_get(
    int unit,
    const char *lt_name,
    const char *lt_fld_name,
    int *val);

/*!
 * \brief Setup SFC QoS PHB with regard to ECN module.
 *
 * \param [in] unit Unit Number.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_ecn_sfc_qos_phb_setup(int unit);

#endif /* BCMINT_LTSW_MBCM_ECN_H */
