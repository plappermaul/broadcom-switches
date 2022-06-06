/*! \file vlan.h
 *
 * Vlan headfiles to declare internal APIs for XGS devices.
 */
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */
#ifndef XGS_LTSW_VLAN_H
#define XGS_LTSW_VLAN_H

#include <bcm/vlan.h>
#include <bcm/types.h>
#include <bcm_int/ltsw/vlan.h>
#include <bcm_int/ltsw/flexctr.h>
#include <sal/sal_types.h>

/*!
 * \brief Initialize VLAN module.
 *
 * \param [in] unit      Unit Number.
 *
 * \retval SHR_E_NONE    No errors.
 * \retval !SHR_E_NONE   Failure.
 */
extern int
xgs_ltsw_vlan_init(int unit);

/*!
 * \brief De-initialize VLAN module.
 *
 * \param [in] unit      Unit Number.
 *
 * \retval SHR_E_NONE    No errors.
 * \retval !SHR_E_NONE   Failure.
 */
extern int
xgs_ltsw_vlan_detach(int unit);

/*!
 * \brief Initialize VLAN outer TPID information.
 *
 * \param [in] unit      Unit Number.
 *
 * \retval SHR_E_NONE    No errors.
 * \retval !SHR_E_NONE   Failure.
 */
extern int
xgs_ltsw_vlan_otpid_init(int unit);

/*!
 * \brief De-initialize VLAN outer TPID information.
 *
 * \param [in] unit      Unit Number.
 *
 * \retval SHR_E_NONE    No errors.
 * \retval !SHR_E_NONE   Failure.
 */
extern int
xgs_ltsw_vlan_otpid_detach(int unit);

/*!
 * \brief Add VLAN outer TPID.
 *
 * \param [in] unit      Unit Number.
 * \param [in] index     Entry index.
 * \param [in] tpid      Outer TPID value.
 *
 *
 * \retval SHR_E_NONE    No errors.
 * \retval !SHR_E_NONE   Failure.
 */
extern int
xgs_ltsw_vlan_otpid_add(int unit, int index, uint16_t tpid);

/*!
 * \brief Set global inner TPID.
 *
 * This function is used to set global inner TPID.
 *
 * \param [in]  unit                 Unit Number.
 * \param [in]  tpid                 TPID to be set.
 *
 * \retval SHR_E_NONE                No errors.
 * \retval !SHR_E_NONE               Failure.
 */
extern int
xgs_ltsw_vlan_itpid_set(int unit, uint16_t tpid);

/*!
 * \brief Get global inner TPID.
 *
 * This function is used to get global inner TPID.
 *
 * \param [in]  unit                 Unit Number.
 * \param [out]  tpid                Inner TPID.
 *
 * \retval SHR_E_NONE                No errors.
 * \retval !SHR_E_NONE               Failure.
 */
extern int
xgs_ltsw_vlan_itpid_get(int unit, uint16_t *tpid);

/*!
 * \brief Add member ports into a specified vlan.
 *
 * \param [in]  unit          Unit number.
 * \param [in]  vid           VLAN ID.
 * \param [in]  ing_pbmp      Ingress member ports.
 * \param [in]  egr_pbmp      Egress member ports.
 * \param [in]  egr_ubmp      Untagged member ports.
 * \param [in]  flags         BCM_VLAN_PORT_XXX flags.
 *
 * \retval SHR_E_NONE         No errors.
 * \retval !SHR_E_NONE        Failure.
 */
extern int
xgs_ltsw_vlan_port_add(int unit,
                       bcm_vlan_t vid,
                       bcm_pbmp_t ing_pbmp,
                       bcm_pbmp_t egr_pbmp,
                       bcm_pbmp_t egr_ubmp,
                       int flags);

/*!
 * \brief Get member ports from a specified vlan.
 *
 * \param [in]  unit          Unit number.
 * \param [in]  vid           VLAN ID.
 * \param [out] ing_pbmp      Ingress member ports.
 * \param [out] egr_pbmp      Egress member ports.
 * \param [out] egr_ubmp      Untagged member ports.
 *
 * \retval SHR_E_NONE         No errors.
 * \retval !SHR_E_NONE        Failure.
 */
extern int
xgs_ltsw_vlan_port_get(int unit,
                       bcm_vlan_t vid,
                       bcm_pbmp_t *ing_pbmp,
                       bcm_pbmp_t *egr_pbmp,
                       bcm_pbmp_t *egr_ubmp);

/*!
 * \brief Remove member ports from a specified vlan.
 *
 * \param [in]  unit          Unit number.
 * \param [in]  vid           VLAN ID.
 * \param [in]  ing_pbmp      Ingress member ports.
 * \param [in]  egr_pbmp      Egress member ports.
 * \param [in]  egr_ubmp      Untagged member ports.
 *
 * \retval SHR_E_NONE         No errors.
 * \retval !SHR_E_NONE        Failure.
 */
extern int
xgs_ltsw_vlan_port_remove(int unit,
                          bcm_vlan_t vid,
                          bcm_pbmp_t ing_pbmp,
                          bcm_pbmp_t egr_pbmp,
                          bcm_pbmp_t egr_ubmp);

/*!
 * \brief Set per VLAN configuration.
 *
 * \param [in]   unit            Unit number.
 * \param [in]   vlan            VLAN ID.
 * \param [in]   valid_fields    Valid fields for VLAN control structure,
 *                               BCM_VLAN_CONTROL_VLAN_XXX_MASK.
 * \param [in]   control         VLAN control structure.
 *
 * \retval SHR_E_NONE     No errors.
 * \retval !SHR_E_NONE    Failure.
 */
extern int
xgs_ltsw_vlan_control_vlan_set(int unit,
                               bcm_vlan_t vid,
                               uint32_t valid_fields,
                               bcm_vlan_control_vlan_t *control);

/*!
 * \brief Get per VLAN configuration.
 *
 * \param [in]   unit            Unit number.
 * \param [in]   vlan            VLAN ID.
 * \param [in]   valid_fields    Valid fields for VLAN control structure,
 *                               BCM_VLAN_CONTROL_VLAN_XXX_MASK.
 * \param [out]  control         VLAN control structure.
 *
 * \retval SHR_E_NONE     No errors.
 * \retval !SHR_E_NONE    Failure.
 */
extern int
xgs_ltsw_vlan_control_vlan_get(int unit,
                               bcm_vlan_t vid,
                               uint32_t valid_fields,
                               bcm_vlan_control_vlan_t *control);

/*!
 * \brief Allocate and configure a VLAN.
 *
 * \param [in]   unit            Unit number.
 * \param [in]   vid             VLAN ID.
 *
 * \retval SHR_E_NONE     No errors.
 * \retval !SHR_E_NONE    Failure.
 */
extern int
xgs_ltsw_vlan_create(int unit, bcm_vlan_t vid);

/*!
 * \brief Deallocate VLAN from Switch.
 *
 * \param [in]   unit            Unit number.
 * \param [in]   vid             VLAN ID.
 *
 * \retval SHR_E_NONE     No errors.
 * \retval !SHR_E_NONE    Failure.
 */
extern int
xgs_ltsw_vlan_destroy(int unit, bcm_vlan_t vid);

/*!
 * \brief Add a Gport to the specified vlan. Adds WLAN/VLAN/NIV/Extender ports to
 *        broadcast, multicast and unknown unicast groups.
 *
 * \param [in]   unit            Unit number.
 * \param [in]   vlan            VLAN ID.
 * \param [in]   port            Gport id.
 * \param [in]   flags           BCM_VLAN_PORT_XXX.
 *
 * \retval SHR_E_NONE      No errors.
 * \retval !SHR_E_NONE     Failure.
 */
extern int
xgs_ltsw_vlan_gport_add(int unit, bcm_vlan_t vlan, bcm_gport_t port, int flags);

/*!
 * \brief Delete a Gport from the specified vlan with flag.
 *
 * \param [in]   unit            Unit number.
 * \param [in]   vlan            VLAN ID.
 * \param [in]   port            Gport id.
 * \param [out]  flags           BCM_VLAN_PORT_XXX.
 *
 * \retval SHR_E_NONE      No errors.
 * \retval !SHR_E_NONE     Failure.
 */
extern int
xgs_ltsw_vlan_gport_delete(int unit, bcm_vlan_t vlan, bcm_gport_t port, int flags);

/*!
 * \brief Delete all Gports from the specified vlan.
 *
 * \param [in]   unit            Unit number.
 * \param [in]   vlan            VLAN ID.
 *
 * \retval SHR_E_NONE      No errors.
 * \retval !SHR_E_NONE     Failure.
 */
extern int
xgs_ltsw_vlan_gport_delete_all(int unit, bcm_vlan_t vlan);

/*!
  * \brief Get a virtual or physical port from the specified VLAN.
  *
  * \param [in]   unit            Unit number.
  * \param [in]   vlan            VLAN ID.
  * \param [in]   port            Gport id.
  * \param [out]  flags           BCM_VLAN_PORT_XXX.
  *
  * \retval SHR_E_NONE      No errors.
  * \retval !SHR_E_NONE     Failure.
  */
extern int
xgs_ltsw_vlan_gport_get(int unit, bcm_vlan_t vlan,
                        bcm_gport_t port, int* flags);

/*!
  * \brief Get all virtual and physical ports from the specified VLAN.
  *
  * \param [in]   unit            Unit number.
  * \param [in]   vlan            VLAN ID.
  * \param [in]   array_max       Max array size.
  * \param [out]  gport_array     Gport id array.
  * \param [out]  flags_array     BCM_VLAN_PORT_XXX array.
  * \param [out]  array_size      Output array size.
  *
  * \retval SHR_E_NONE      No errors.
  * \retval !SHR_E_NONE     Failure.
  */
extern int
xgs_ltsw_vlan_gport_get_all(int unit, bcm_vlan_t vlan, int array_max,
                            bcm_gport_t *gport_array, int *flags_array,
                            int* array_size);

/*!
 * \brief Get VLAN actions per port.
 *
 * This function is used to get VLAN actions per port.
 *
 * \param [in] unit              Unit number.
 * \param [in] port              Port number.
 * \param [out]action            VLAN actions.
 *
 * \retval SHR_E_NONE            No errors.
 * \retval !SHR_E_NONE           Failure.
 */
extern int
xgs_ltsw_vlan_port_action_get(int unit,
                              bcm_port_t port,
                              bcm_vlan_action_set_t *action);

/*!
 * \brief Set VLAN actions per port.
 *
 * This function is used to set VLAN actions per port.
 *
 * \param [in] unit              Unit number.
 * \param [in] port              Port number.
 * \param [out]action            VLAN actions.
 *
 * \retval SHR_E_NONE            No errors.
 * \retval !SHR_E_NONE           Failure.
 */
extern int
xgs_ltsw_vlan_port_action_set(int unit,
                              bcm_port_t port,
                              bcm_vlan_action_set_t *action);

/*!
 * \brief Reset VLAN actions per port.
 *
 * This function is used to reset VLAN actions per port.
 *
 * \param [in] unit              Unit number.
 * \param [in] port              Port number.
 *
 * \retval SHR_E_NONE            No errors.
 * \retval !SHR_E_NONE           Failure.
 */
extern int
xgs_ltsw_vlan_port_action_reset(int unit,
                                bcm_port_t port);

/*!
 * \brief Set egr VLAN actions per port.
 *
 * This function is used to set VLAN actions per port.
 *
 * \param [in] unit              Unit number.
 * \param [in] port              Port number.
 * \param [out]action            VLAN actions.
 *
 * \retval SHR_E_NONE            No errors.
 * \retval !SHR_E_NONE           Failure.
 */
extern int
xgs_ltsw_egr_vlan_port_action_set(int unit,
                                  bcm_port_t port,
                                  bcm_vlan_action_set_t *action);

/*!
 * \brief Get egr VLAN actions per port.
 *
 * This function is used to get VLAN actions per port.
 *
 * \param [in] unit              Unit number.
 * \param [in] port              Port number.
 * \param [out]action            VLAN actions.
 *
 * \retval SHR_E_NONE            No errors.
 * \retval !SHR_E_NONE           Failure.
 */
extern int
xgs_ltsw_egr_vlan_port_action_get(int unit,
                                  bcm_port_t port,
                                  bcm_vlan_action_set_t *action);

/*!
 * \brief Reset egr VLAN actions per port.
 *
 * This function is used to reset VLAN actions per port.
 *
 * \param [in] unit              Unit number.
 * \param [in] port              Port number.
 *
 * \retval SHR_E_NONE            No errors.
 * \retval !SHR_E_NONE           Failure.
 */
extern int
xgs_ltsw_egr_vlan_port_action_reset(int unit,
                                    bcm_port_t port);

/*!
 * \brief Set VLAN mcast flood mode.
 *
 * This function is used to set vlan flood mode.
 *
 * \param [in] unit              Unit number.
 * \param [in] vid               VLAN ID.
 * \param [in] mode              VLAN flood mode.
 *
 * \retval SHR_E_NONE            No errors.
 * \retval !SHR_E_NONE           Failure.
 */
extern int
xgs_ltsw_vlan_mcast_flood_set(int unit,
                              bcm_vlan_t vid,
                              bcm_vlan_mcast_flood_t mode);

/*!
 * \brief Get VLAN mcast flood mode.
 *
 * This function is used to get vlan flood mode.
 *
 * \param [in] unit              Unit number.
 * \param [in] vid               VLAN ID.
 * \param [out] mode             VLAN flood mode.
 *
 * \retval SHR_E_NONE            No errors.
 * \retval !SHR_E_NONE           Failure.
 */
extern int
xgs_ltsw_vlan_mcast_flood_get(int unit,
                              bcm_vlan_t vid,
                              bcm_vlan_mcast_flood_t *mode);

/*!
 * \brief Set vlan specific traffic blocking parameters.
 *
 * This function is used to set vlan specific traffic blocking parameters.
 *
 * \param [in] unit              Unit number.
 * \param [in] vid               VLAN ID.
 * \param [in] block             Block configuration.
 *
 * \retval SHR_E_NONE            No errors.
 * \retval !SHR_E_NONE           Failure.
 */
extern int
xgs_ltsw_vlan_block_set(int unit,
                        bcm_vlan_t vid,
                        bcm_vlan_block_t *block);

/*!
 * \brief Get vlan specific traffic blocking parameters.
 *
 * This function is used to get vlan specific traffic blocking parameters.
 *
 * \param [in] unit              Unit number.
 * \param [in] vid               VLAN ID.
 * \param [out] block             Block configuration.
 *
 * \retval SHR_E_NONE            No errors.
 * \retval !SHR_E_NONE           Failure.
 */
extern int
xgs_ltsw_vlan_block_get(int unit,
                        bcm_vlan_t vid,
                        bcm_vlan_block_t *block);

/*!
 * \brief Get miscellaneous VLAN-specific chip options.
 *
 * \param [in]    unit     Unit number.
 * \param [in]    type     A value from bcm_vlan_control_t enumeration list.
 * \param [out]   arg      State whose meaning is dependent on 'type'.
 *
 * \retval SHR_E_NONE     No errors.
 * \retval !SHR_E_NONE    Failure.
 */
extern int
xgs_ltsw_vlan_control_get(int unit, bcm_vlan_control_t type, int *arg);

/*!
 * \brief Set miscellaneous VLAN-specific chip options.
 *
 * \param [in]   unit     Unit number.
 * \param [in]   type     A value from bcm_vlan_control_t enumeration list.
 * \param [in]   arg      State whose meaning is dependent on 'type'.
 *
 * \retval SHR_E_NONE     No errors.
 * \retval !SHR_E_NONE    Failure.
 */
extern int
xgs_ltsw_vlan_control_set(int unit, bcm_vlan_control_t type, int arg);

/*!
 * \brief Get miscellaneous port-specific VLAN options.
 *
 * This function is used to get miscellaneous port-specific VLAN options.
 *
 * \param [in]  unit             Unit number.
 * \param [in]  port             PORT ID.
 * \param [in]  type             Control port type.
 * \param [out] arg              Arg vlaue which is dependent on control type.
 *
 * \retval SHR_E_NONE            No errors.
 * \retval !SHR_E_NONE           Failure.
 */
extern int
xgs_ltsw_vlan_control_port_get(int unit,
                               bcm_port_t port,
                               bcm_vlan_control_port_t type,
                               int *arg);

/*!
 * \brief Set miscellaneous port-specific VLAN options.
 *
 * This function is used to set miscellaneous port-specific VLAN options.
 *
 * \param [in]  unit             Unit number.
 * \param [in]  port             PORT ID.
 * \param [in]  type             Control port type.
 * \param [in]  arg              Arg vlaue which is dependent on control type.
 *
 * \retval SHR_E_NONE            No errors.
 * \retval !SHR_E_NONE           Failure.
 */
extern int
xgs_ltsw_vlan_control_port_set(int unit,
                               bcm_port_t port,
                               bcm_vlan_control_port_t type,
                               int arg);

/*!
 * \brief Add a VLAN cross connect entry.
 *
 * This function is used to add a VLAN cross connect entry.
 *
 * \param [in] unit              Unit number.
 * \param [in] outer_vlan        Outer VLAN ID.
 * \param [in] inner_vlan        Inner VLAN ID.
 * \param [in] port_1            First port in the cross-connect.
 * \param [in] port_2            Second port in the cross-connect.
 *
 * \retval SHR_E_NONE            No errors.
 * \retval !SHR_E_NONE           Failure.
 */
extern int
xgs_ltsw_vlan_cross_connect_add(int unit,
                                bcm_vlan_t outer_vlan, bcm_vlan_t inner_vlan,
                                bcm_gport_t port_1, bcm_gport_t port_2);

/*!
 * \brief Delete a VLAN cross connect entry.
 *
 * This function is used to delete a VLAN cross connect entry.
 *
 * \param [in] unit              Unit number.
 * \param [in] outer_vlan        Outer VLAN ID.
 * \param [in] inner_vlan        Inner VLAN ID.
 *
 * \retval SHR_E_NONE            No errors.
 * \retval !SHR_E_NONE           Failure.
 */
extern int
xgs_ltsw_vlan_cross_connect_delete(int unit,
                                   bcm_vlan_t outer_vlan, bcm_vlan_t inner_vlan);

/*!
 * \brief Delete all VLAN cross connect entries.
 *
 * This function is used to delete all VLAN cross connect entries.
 *
 * \param [in] unit              Unit number.
 *
 * \retval SHR_E_NONE            No errors.
 * \retval !SHR_E_NONE           Failure.
 */
extern int
xgs_ltsw_vlan_cross_connect_delete_all(int unit);

/*!
 * \brief Traverse VLAN cross connect entries.
 *
 * This function is used to traverse VLAN cross connect entries.
 *
 * \param [in] unit              Unit number.
 * \param [in] cb                Callback function.
 * \param [in] user_data         User data to be passed to callback function.
 *
 * \retval SHR_E_NONE            No errors.
 * \retval !SHR_E_NONE           Failure.
 */
extern int
xgs_ltsw_vlan_cross_connect_traverse(int unit,
                                     bcm_vlan_cross_connect_traverse_cb cb,
                                     void *user_data);

/*!
 * \brief Attach counter entries to the given vlan.
 *
 * This function is used to attach flex counter to the given vlan.
 *
 * \param [in] unit              Unit number.
 * \param [in] vid               VLAN ID.
 * \param [in] info              Counter information.
 *
 * \retval SHR_E_NONE            No errors.
 * \retval !SHR_E_NONE           Failure.
 */
extern int
xgs_ltsw_vlan_stat_attach(int unit, bcm_vlan_t vid,
                          bcmi_ltsw_flexctr_counter_info_t *info);

/*!
 * \brief Detach counter entries to the given vlan.
 *
 * This function is used to detach flex counter to the given vlan.
 *
 * \param [in] unit              Unit number.
 * \param [in] vid               VLAN ID.
 * \param [in] info              Counter information.
 *
 * \retval SHR_E_NONE            No errors.
 * \retval !SHR_E_NONE           Failure.
 */
extern int
xgs_ltsw_vlan_stat_detach(int unit, bcm_vlan_t vid,
                          bcmi_ltsw_flexctr_counter_info_t *info);

/*!
 * \brief Set ingress vlan qos id.
 *
 * This function is used to set ingress vlan qos id.
 *
 * \param [in] unit              Unit number.
 * \param [in] vid               VLAN ID.
 * \param [in] id                vlan qos id.
 *
 * \retval SHR_E_NONE            No errors.
 * \retval !SHR_E_NONE           Failure.
 */
extern int
xgs_ltsw_vlan_ing_qos_set(int unit, bcm_vlan_t vid, int id);

/*!
 * \brief Get ingress vlan qos id.
 *
 * This function is used to get ingress vlan qos id.
 *
 * \param [in] unit              Unit number.
 * \param [in] vid               VLAN ID.
 * \param [out] id               vlan qos id.
 *
 * \retval SHR_E_NONE            No errors.
 * \retval !SHR_E_NONE           Failure.
 */
extern int
xgs_ltsw_vlan_ing_qos_get(int unit, bcm_vlan_t vid, int *id);

/*!
 * \brief Set egress vlan qos id.
 *
 * This function is used to set egress vlan qos id.
 *
 * \param [in] unit              Unit number.
 * \param [in] vid               VLAN ID.
 * \param [in] id                vlan qos id.
 *
 * \retval SHR_E_NONE            No errors.
 * \retval !SHR_E_NONE           Failure.
 */
extern int
xgs_ltsw_vlan_egr_qos_set(int unit, bcm_vlan_t vid, int id);

/*!
 * \brief Get egress vlan qos id.
 *
 * This function is used to get egress vlan qos id.
 *
 * \param [in] unit              Unit number.
 * \param [in] vid               VLAN ID.
 * \param [out] id               vlan qos id.
 *
 * \retval SHR_E_NONE            No errors.
 * \retval !SHR_E_NONE           Failure.
 */
extern int
xgs_ltsw_vlan_egr_qos_get(int unit, bcm_vlan_t vid, int *id);

/*!
 * \brief Set vlan switch control configurations.
 *
 * \param [in] unit              Unit number.
 * \param [in] type              The desired configuration parameter.
 * \param [in] value             The value to set the parameter.
 *
 * \retval SHR_E_NONE            No errors.
 * \retval !SHR_E_NONE           Failure.
 */
extern int
xgs_ltsw_vlan_internal_control_set(int unit,
                                   bcmi_ltsw_vlan_control_t type,
                                   int value);

/*!
 * \brief Get vlan switch control configurations.
 *
 * \param [in] unit              Unit number.
 * \param [in] type              The desired configuration parameter.
 * \param [in] value             The value pointer to get the parameter.
 *
 * \retval SHR_E_NONE            No errors.
 * \retval !SHR_E_NONE           Failure.
 */
extern int
xgs_ltsw_vlan_internal_control_get(int unit,
                                   bcmi_ltsw_vlan_control_t type,
                                   int *value);

extern int
xgs_ltsw_vlan_stg_set(int unit, bcm_vlan_t vid, int ingress, bcm_stg_t stg);

extern int
xgs_ltsw_vlan_stg_get(int unit, bcm_vlan_t vid, int ingress, bcm_stg_t *stg);

/*!
 * \brief Traverse vlan table to get needed info.
 *
 * This function is used to traverse vlan table without lock.
 *
 * \param [in] unit              Unit number.
 * \param [in] cb                Callback function.
 * \param [in] user_data         User data of callback.
 *
 * \retval SHR_E_NONE            No errors.
 * \retval !SHR_E_NONE           Failure.
 */
extern int
xgs_ltsw_vlan_traverse(int unit,
                       bcmi_ltsw_vlan_traverse_cb cb, void *user_data);
#endif /* XGS_LTSW_VLAN_H */
