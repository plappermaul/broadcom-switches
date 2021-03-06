/*
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated.
 * Edits to this file will be lost when it is regenerated.
 *
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

#ifndef BCMINT_LTSW_XFS_FLOW_H
#define BCMINT_LTSW_XFS_FLOW_H

#include <bcm/types.h>
#include <bcm/flow.h>
#include <bcm/trunk.h>

#include <bcm_int/ltsw/flow_int.h>

#include <sal/sal_types.h>

/*!
 * \brief Initialize flow module.
 *
 * \param [in] unit Unit Number.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
xfs_ltsw_flow_init(int unit);

/*!
 * \brief De-initialize flow module.
 *
 * \param [in] unit Unit Number.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
xfs_ltsw_flow_detach(int unit);

/*!
 * \brief Update HW table for given VPN.
 *
 * \param [in] unit Unit Number.
 * \param [in] opt Opt type from ltsw_flow_hw_opt_type_t.
 * \param [in] vpn VPN ID.
 * \param [in-out] info VPN configuration structure.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
xfs_ltsw_flow_vpn_hw_opt(
    int unit,
    bcmint_flow_hw_opt_type_t opt,
    bcm_vpn_t vpn,
    bcm_flow_vpn_config_t *info);

/*!
 * \brief Update HW table for given VP.
 *
 * \param [in] unit Unit Number.
 * \param [in] opt Opt type from ltsw_flow_hw_opt_type_t.
 * \param [in-out] flow_port Flow port information.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
xfs_ltsw_flow_vp_hw_opt(
    int unit,
    bcmint_flow_hw_opt_type_t opt,
    bcm_flow_port_t *flow_port);

/*!
 * \brief Update HW table based on match criteria.
 *
 * \param [in] unit Unit Number.
 * \param [in] opt Opt type from ltsw_flow_hw_opt_type_t.
 * \param [in-out] info Flow match information.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
xfs_ltsw_flow_match_hw_opt(
    int unit,
    bcmint_flow_hw_opt_type_t opt,
    bcm_flow_match_config_t *info);

/*!
 * \brief Traverse HW table based on match criteria.
 *
 * \param [in] unit Unit Number.
 * \param [in] cb User callback function.
 * \param [in] user_data User context data.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
xfs_ltsw_flow_match_traverse(
    int unit,
    bcm_flow_match_traverse_cb cb,
    void *user_data);

/*!
 * \brief Synchronize flow match when deleting members in the match trunk port.
 *
 * \param [in] unit Unit Number.
 * \param [in] trunk_id Trunk port ID.
 * \param [in] trunk_member_count Count of the trunk members.
 * \param [in] trunk_member_array Trunk member gport array.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
xfs_ltsw_flow_match_trunk_member_delete(
    int unit,
    bcm_trunk_t trunk_id,
    int trunk_member_count,
    bcm_port_t *trunk_member_array);

/*!
 * \brief Update HW table based on encap criteria.
 *
 * \param [in] unit Unit Number.
 * \param [in] opt Opt type from ltsw_flow_hw_opt_type_t.
 * \param [in-out] info Flow encap information.
 * \param [in] num_of_fields Number of logical fields.
 * \param [in] field Logical field array.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
xfs_ltsw_flow_encap_hw_opt(
    int unit,
    bcmint_flow_hw_opt_type_t opt,
    bcm_flow_encap_config_t *info,
    uint32_t num_of_fields,
    bcm_flow_logical_field_t *field);

/*!
 * \brief Traverse HW table based on encap criteria.
 *
 * \param [in] unit Unit Number.
 * \param [in] cb User callback function.
 * \param [in] user_data User context data.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
xfs_ltsw_flow_encap_traverse(
    int unit,
    bcm_flow_encap_traverse_cb cb,
    void *user_data);

/*!
 * \brief Update HW table based on VP attributes.
 *
 * \param [in] unit Unit Number.
 * \param [in] opt Opt type from ltsw_flow_hw_opt_type_t.
 * \param [in-out] info Flow  port encap information.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
xfs_ltsw_flow_port_encap_hw_opt(
    int unit,
    bcmint_flow_hw_opt_type_t opt,
    bcm_flow_port_encap_t *info);

/*!
 * \brief Notify the egress ECMP changes to update VP attributes.
 *
 * \param [in] unit Unit Number.
 * \param [in] ecmp_group_id Egress ECMP forwarding object ID.
 * \param [in] ecmp_member_count Number of elements in ecmp_member_array.
 * \param [in] ecmp_member_array Member array of egress forwarding objects.
 * \param [in] add Indicates whether this is member adding or deleting.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
xfs_ltsw_flow_port_encap_l3_ecmp_notify(
    int unit,
    bcm_if_t ecmp_group_id,
    int ecmp_member_count,
    bcm_l3_ecmp_member_t *ecmp_member_array,
    int add);

/*!
 * \brief Get flow port from L2 interface.
 *
 * \param [in] unit Unit Number.
 * \param [in] l2_if L2 interface.
 * \param [out] port Port Number.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
xfs_ltsw_flow_l2_if_to_port(
    int unit,
    int l2_if,
    bcm_port_t *port);

/*!
 * \brief Get L2 interface from flow gport.
 *
 * \param [in] unit Unit Number.
 * \param [in] port Port Number.
 * \param [out] l2_if L2 interface.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
xfs_ltsw_flow_port_to_l2_if(
    int unit,
    bcm_port_t port,
    int *l2_if);

/*!
 * \brief Update HW table based on tunnel terminator.
 *
 * \param [in] unit Unit Number.
 * \param [in] opt Opt type from ltsw_flow_hw_opt_type_t.
 * \param [in-out] info Flow tunnel terminator information.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
xfs_ltsw_flow_tunnel_term_hw_opt(
    int unit,
    bcmint_flow_hw_opt_type_t opt,
    bcm_flow_tunnel_terminator_t *info);

/*!
 * \brief Traverse HW table based on tunnel terminator.
 *
 * \param [in] unit Unit Number.
 * \param [in] cb User callback function.
 * \param [in] user_data User context data.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
xfs_ltsw_flow_tunnel_term_traverse(
    int unit,
    bcm_flow_tunnel_terminator_traverse_cb cb,
    void *user_data);

/*!
 * \brief Update HW table based on tunnel initiator.
 *
 * \param [in] unit Unit Number.
 * \param [in] opt Opt type from ltsw_flow_hw_opt_type_t.
 * \param [in-out] info Flow tunnel initiator information.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
xfs_ltsw_flow_tunnel_initiator_hw_opt(
    int unit,
    bcmint_flow_hw_opt_type_t opt,
    bcm_flow_tunnel_initiator_t *info);

/*!
 * \brief Traverse HW table based on tunnel initiator.
 *
 * \param [in] unit Unit Number.
 * \param [in] cb User callback function.
 * \param [in] user_data User context data.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
xfs_ltsw_flow_tunnel_initiator_traverse(
    int unit,
    bcm_flow_tunnel_initiator_traverse_cb cb,
    void *user_data);

/*!
 * \brief Attach counter to the given table.
 *
 * \param [in] unit Unit Number.
 * \param [in] info Flow stat config structure.
 * \param [in] stat_counter_id Stat counter id.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
xfs_ltsw_flow_stat_attach(
    int unit,
    bcm_flow_stat_info_t *info,
    uint32_t stat_counter_id);

/*!
 * \brief Detach counter from the given table.
 *
 * \param [in] unit Unit Number.
 * \param [in] info Flow stat config structure.
 * \param [in] stat_counter_id Stat counter id.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
xfs_ltsw_flow_stat_detach(
    int unit,
    bcm_flow_stat_info_t *info,
    uint32_t stat_counter_id);

/*!
 * \brief Dump device-specific FLOW S/W state.
 *
 * \param [in] unit Unit Number.
 */
extern void
xfs_ltsw_flow_sw_dump(int unit);

/*!
 * \brief Enable/disable EVPN functionality.
 *
 * \param [in] unit Unit Number.
 * \param [in] enable EVPN enable indicator. 0 : Disable EVPN, 1 : Enable EVPN.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
xfs_ltsw_flow_evpn_enable_set(
    int unit,
    int enable);

/*!
 * \brief Get the enable status of EVPN functionality.
 *
 * \param [in] unit Unit Number.
 * \param [in] enable EVPN enable indicator. 0 : EVPN disabled , 1 : EVPN enabled.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
xfs_ltsw_flow_evpn_enable_get(
    int unit,
    int *enable);

#endif /* BCMINT_LTSW_XFS_FLOW_H */
