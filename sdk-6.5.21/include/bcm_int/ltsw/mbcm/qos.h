/*
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated.
 * Edits to this file will be lost when it is regenerated.
 *
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

#ifndef BCMINT_LTSW_MBCM_QOS_H
#define BCMINT_LTSW_MBCM_QOS_H

#include <bcm/qos.h>
#include <bcm/types.h>

#include <bcm_int/ltsw/qos.h>
#include <bcm_int/ltsw/qos_int.h>

#include <sal/sal_types.h>

/*!
 * \brief Initialize QoS module.
 *
 * \param [in] unit Unit Number.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*qos_init_f)(int unit);

/*!
 * \brief Detach QoS module.
 *
 * \param [in] unit Unit Number.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*qos_detach_f)(int unit);

/*!
 * \brief Clear all QoS tables.
 *
 * \param [in] unit Unit Number.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*qos_table_clear_all_f)(int unit);

/*!
 * \brief Set QoS controls.
 *
 * \param [in] unit Unit Number.
 * \param [in] control The desired configuration parameter to modify.
 * \param [in] value The value with which to set the parameter.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*qos_control_set_f)(
    int unit,
    bcmi_ltsw_qos_control_t control,
    uint32_t value);

/*!
 * \brief Get QoS controls.
 *
 * \param [in] unit Unit Number.
 * \param [in] control The desired configuration parameter to retrieve.
 * \param [out] value Pointer to where the retrieved value will be written.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*qos_control_get_f)(
    int unit,
    bcmi_ltsw_qos_control_t control,
    uint32_t *value);

/*!
 * \brief Create a QoS map.
 *
 * \param [in] unit Unit Number.
 * \param [in] flags QoS flags.
 * \param [out] map_id QoS map id.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*qos_map_create_f)(
    int unit,
    uint32_t flags,
    int *map_id);

/*!
 * \brief Destroy a QoS map.
 *
 * \param [in] unit Unit Number.
 * \param [in] map_id QoS map id.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*qos_map_destroy_f)(
    int unit,
    int map_id);

/*!
 * \brief Add an entry into a QoS map.
 *
 * \param [in] unit Unit Number.
 * \param [in] flags QoS flags.
 * \param [in] map QoS map.
 * \param [in] map_id QoS map id.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*qos_map_add_f)(
    int unit,
    uint32_t flags,
    bcm_qos_map_t *map,
    int map_id);

/*!
 * \brief Delete an entry from a QoS map.
 *
 * \param [in] unit Unit Number.
 * \param [in] flags QoS flags.
 * \param [in] map QoS map.
 * \param [in] map_id QoS map id.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*qos_map_delete_f)(
    int unit,
    uint32_t flags,
    bcm_qos_map_t *map,
    int map_id);

/*!
 * \brief Attach QoS map to an object(Gport).
 *
 * \param [in] unit Unit Number.
 * \param [in] port Port/Gport id.
 * \param [in] ing_map Ingress map id.
 * \param [in] egr_map Egress map id.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*qos_port_map_set_f)(
    int unit,
    bcm_gport_t port,
    int ing_map,
    int egr_map);

/*!
 * \brief Retrieve the configured QoS mapping matching a type for the given GPORT.
 *
 * \param [in] unit Unit Number.
 * \param [in] port Port/Gport id.
 * \param [in] flags QoS flags.
 * \param [out] map_id QoS map id.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*qos_port_map_type_get_f)(
    int unit,
    bcm_gport_t port,
    uint32_t flags,
    int *map_id);

/*!
 * \brief Get an entire QoS mapping for the given QoS Map ID.
 *
 * \param [in] unit Unit Number.
 * \param [in] flags QoS flags.
 * \param [in] map_id QoS map id.
 * \param [in] array_size Size of allocated entries in array.
 * \param [out] array QoS map array.
 * \param [out] count Actual number of QoS map entries.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*qos_map_multi_get_f)(
    int unit,
    uint32_t flags,
    int map_id,
    int array_size,
    bcm_qos_map_t *array,
    int *count);

/*!
 * \brief Get the list of all QoS Map IDs.
 *
 * \param [in] unit Unit Number.
 * \param [in] array_size Size of allocated entries in map_id array.
 * \param [out] map_id QoS map id array.
 * \param [out] flags QoS flags array.
 * \param [out] count Actual number of QoS map id.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*qos_multi_get_f)(
    int unit,
    int array_size,
    int *map_id,
    int *flags,
    int *count);

/*!
 * \brief Set ingress and egress VLAN port based related QoS setting.
 *
 * \param [in] unit Unit Number.
 * \param [in] port Ingress port.
 * \param [in] vid Vlan domain.
 * \param [in] ing_map Ingress QoS id.
 * \param [in] egr_map Egress QoS id.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*qos_port_vlan_map_set_f)(
    int unit,
    bcm_port_t port,
    bcm_vlan_t vid,
    int ing_map,
    int egr_map);

/*!
 * \brief Get ingress and egress VLAN port based related QoS setting.
 *
 * \param [in] unit Unit Number.
 * \param [in] port Ingress port.
 * \param [in] vid Vlan domain.
 * \param [out] ing_map Ingress QoS id.
 * \param [out] egr_map Egress QoS id.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*qos_port_vlan_map_get_f)(
    int unit,
    bcm_port_t port,
    bcm_vlan_t vid,
    int *ing_map,
    int *egr_map);

/*!
 * \brief Resolve QoS map id into mapping pointer and QoS map type.
 *
 * \param [in] unit Unit Number.
 * \param [in] map_id QoS map id.
 * \param [out] type QoS map type. NULL indicates not care.
 * \param [out] ptr Mapping pointer. NULL indicates not care.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*qos_map_id_resolve_f)(
    int unit,
    int map_id,
    bcmi_ltsw_qos_map_type_t *type,
    int *ptr);

/*!
 * \brief Construct QoS map id with mapping pointer and QoS map type.
 *
 * \param [in] unit Unit Number.
 * \param [in] ptr Mapping pointer.
 * \param [in] type QoS map type.
 * \param [out] map_id QoS map id.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*qos_map_id_construct_f)(
    int unit,
    int ptr,
    bcmi_ltsw_qos_map_type_t type,
    int *map_id);

/*!
 * \brief Get default mapping pointer.
 *
 * \param [in] unit Unit Number.
 * \param [in] type QoS map type.
 * \param [out] ptr Mapping pointer.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*qos_default_ptr_get_f)(
    int unit,
    bcmi_ltsw_qos_map_type_t type,
    int *ptr);

/*!
 * \brief Get dummy operation pointer. The pointer indicates QoS remarking will do nothing with int_cn/int_pri/cng.
 *
 * \param [in] unit Unit Number.
 * \param [in] type QoS map type.
 * \param [out] ptr Mapping pointer.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*qos_dummy_ptr_get_f)(
    int unit,
    bcmi_ltsw_qos_map_type_t type,
    int *ptr);

/*!
 * \brief Dump QoS module software bookkeepings.
 *
 * \param [in] unit Unit Number.
 */
typedef void (*qos_sw_dump_f)(int unit);

/*!
 * \brief Set egress L2 map on a port based on egress VLAN priority source.
 *
 * \param [in] unit Unit Number.
 * \param [in] port Port id.
 * \param [in] src Source of egress VLAN priority.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*qos_port_egr_l2_map_set_f)(
    int unit,
    bcm_port_t port,
    int src);

/*!
 * \brief Get egress VLAN priority source on a port based on egress L2 map.
 *
 * \param [in] unit Unit Number.
 * \param [in] port Port id.
 * \param [out] src Source of egress VLAN priority.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*qos_port_egr_l2_map_get_f)(
    int unit,
    bcm_port_t port,
    int *src);

/*!
 * \brief Restore default QoS setting on a new port during flex port.
 *
 * \param [in] unit Unit Number.
 * \param [in] port Port id.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*qos_port_attach_f)(
    int unit,
    bcm_port_t port);

/*!
 * \brief Clear default QoS setting on a to-be-removed port during flex port.
 *
 * \param [in] unit Unit Number.
 * \param [in] port Port id.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*qos_port_detach_f)(
    int unit,
    bcm_port_t port);

/*!
 * \brief Initialize chip-specific QoS control info.
 *
 * \param [in] unit Unit Number.
 * \param [in|out] ctrl_info QoS control info pointer.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*qos_ctrl_info_init_f)(
    int unit,
    const bcmint_qos_ctrl_info_t **ctrl_info);

/*!
 * \brief Get mapping pointer for Vxlan G/BUM flags remarking.
 *
 * \param [in] unit Unit Number.
 * \param [in] g_ena Enable G flag remarking.
 * \param [in] bum_ena Enable BUM flag remarking.
 * \param [out] ptr Mapping pointer.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*qos_vxlan_flags_map_ptr_get_f)(
    int unit,
    int g_ena,
    int bum_ena,
    int *ptr);

/*!
 * \brief Get QoS LT info.
 *
 * \param [in] unit Unit Number.
 * \param [in] lt_db QoS LT info.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*qos_lt_db_get_f)(
    int unit,
    bcmint_qos_lt_db_t *lt_db);

/*!
 * \brief Qos driver structure.
 */
typedef struct mbcm_ltsw_qos_drv_s {

    /*! Initialize QoS module. */
    qos_init_f qos_init;

    /*! Detach QoS module. */
    qos_detach_f qos_detach;

    /*! Clear all QoS tables. */
    qos_table_clear_all_f qos_table_clear_all;

    /*! Set QoS controls. */
    qos_control_set_f qos_control_set;

    /*! Get QoS controls. */
    qos_control_get_f qos_control_get;

    /*! Create a QoS map. */
    qos_map_create_f qos_map_create;

    /*! Destroy a QoS map. */
    qos_map_destroy_f qos_map_destroy;

    /*! Add an entry into a QoS map. */
    qos_map_add_f qos_map_add;

    /*! Delete an entry from a QoS map. */
    qos_map_delete_f qos_map_delete;

    /*! Attach QoS map to an object(Gport). */
    qos_port_map_set_f qos_port_map_set;

    /*! Retrieve the configured QoS mapping matching a type for the given GPORT. */
    qos_port_map_type_get_f qos_port_map_type_get;

    /*! Get an entire QoS mapping for the given QoS Map ID. */
    qos_map_multi_get_f qos_map_multi_get;

    /*! Get the list of all QoS Map IDs. */
    qos_multi_get_f qos_multi_get;

    /*! Set ingress and egress VLAN port based related QoS setting. */
    qos_port_vlan_map_set_f qos_port_vlan_map_set;

    /*! Get ingress and egress VLAN port based related QoS setting. */
    qos_port_vlan_map_get_f qos_port_vlan_map_get;

    /*! Resolve QoS map id into mapping pointer and QoS map type. */
    qos_map_id_resolve_f qos_map_id_resolve;

    /*! Construct QoS map id with mapping pointer and QoS map type. */
    qos_map_id_construct_f qos_map_id_construct;

    /*! Get default mapping pointer. */
    qos_default_ptr_get_f qos_default_ptr_get;

    /*! Get dummy operation pointer. The pointer indicates QoS remarking will do nothing with int_cn/int_pri/cng. */
    qos_dummy_ptr_get_f qos_dummy_ptr_get;

    /*! Dump QoS module software bookkeepings. */
    qos_sw_dump_f qos_sw_dump;

    /*! Set egress L2 map on a port based on egress VLAN priority source. */
    qos_port_egr_l2_map_set_f qos_port_egr_l2_map_set;

    /*! Get egress VLAN priority source on a port based on egress L2 map. */
    qos_port_egr_l2_map_get_f qos_port_egr_l2_map_get;

    /*! Restore default QoS setting on a new port during flex port. */
    qos_port_attach_f qos_port_attach;

    /*! Clear default QoS setting on a to-be-removed port during flex port. */
    qos_port_detach_f qos_port_detach;

    /*! Initialize chip-specific QoS control info. */
    qos_ctrl_info_init_f qos_ctrl_info_init;

    /*! Get mapping pointer for Vxlan G/BUM flags remarking. */
    qos_vxlan_flags_map_ptr_get_f qos_vxlan_flags_map_ptr_get;

    /*! Get QoS LT info. */
    qos_lt_db_get_f qos_lt_db_get;

} mbcm_ltsw_qos_drv_t;

/*!
 * \brief Set the QOS driver of the device.
 *
 * \param [in] unit Unit Number.
 * \param [in] drv Qos driver to set.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_qos_drv_set(
    int unit,
    mbcm_ltsw_qos_drv_t *drv);

/*!
 * \brief Initialize QoS module.
 *
 * \param [in] unit Unit Number.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_qos_init(int unit);

/*!
 * \brief Detach QoS module.
 *
 * \param [in] unit Unit Number.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_qos_detach(int unit);

/*!
 * \brief Clear all QoS tables.
 *
 * \param [in] unit Unit Number.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_qos_table_clear_all(int unit);

/*!
 * \brief Set QoS controls.
 *
 * \param [in] unit Unit Number.
 * \param [in] control The desired configuration parameter to modify.
 * \param [in] value The value with which to set the parameter.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_qos_control_set(
    int unit,
    bcmi_ltsw_qos_control_t control,
    uint32_t value);

/*!
 * \brief Get QoS controls.
 *
 * \param [in] unit Unit Number.
 * \param [in] control The desired configuration parameter to retrieve.
 * \param [out] value Pointer to where the retrieved value will be written.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_qos_control_get(
    int unit,
    bcmi_ltsw_qos_control_t control,
    uint32_t *value);

/*!
 * \brief Create a QoS map.
 *
 * \param [in] unit Unit Number.
 * \param [in] flags QoS flags.
 * \param [out] map_id QoS map id.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_qos_map_create(
    int unit,
    uint32_t flags,
    int *map_id);

/*!
 * \brief Destroy a QoS map.
 *
 * \param [in] unit Unit Number.
 * \param [in] map_id QoS map id.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_qos_map_destroy(
    int unit,
    int map_id);

/*!
 * \brief Add an entry into a QoS map.
 *
 * \param [in] unit Unit Number.
 * \param [in] flags QoS flags.
 * \param [in] map QoS map.
 * \param [in] map_id QoS map id.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_qos_map_add(
    int unit,
    uint32_t flags,
    bcm_qos_map_t *map,
    int map_id);

/*!
 * \brief Delete an entry from a QoS map.
 *
 * \param [in] unit Unit Number.
 * \param [in] flags QoS flags.
 * \param [in] map QoS map.
 * \param [in] map_id QoS map id.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_qos_map_delete(
    int unit,
    uint32_t flags,
    bcm_qos_map_t *map,
    int map_id);

/*!
 * \brief Attach QoS map to an object(Gport).
 *
 * \param [in] unit Unit Number.
 * \param [in] port Port/Gport id.
 * \param [in] ing_map Ingress map id.
 * \param [in] egr_map Egress map id.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_qos_port_map_set(
    int unit,
    bcm_gport_t port,
    int ing_map,
    int egr_map);

/*!
 * \brief Retrieve the configured QoS mapping matching a type for the given GPORT.
 *
 * \param [in] unit Unit Number.
 * \param [in] port Port/Gport id.
 * \param [in] flags QoS flags.
 * \param [out] map_id QoS map id.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_qos_port_map_type_get(
    int unit,
    bcm_gport_t port,
    uint32_t flags,
    int *map_id);

/*!
 * \brief Get an entire QoS mapping for the given QoS Map ID.
 *
 * \param [in] unit Unit Number.
 * \param [in] flags QoS flags.
 * \param [in] map_id QoS map id.
 * \param [in] array_size Size of allocated entries in array.
 * \param [out] array QoS map array.
 * \param [out] count Actual number of QoS map entries.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_qos_map_multi_get(
    int unit,
    uint32_t flags,
    int map_id,
    int array_size,
    bcm_qos_map_t *array,
    int *count);

/*!
 * \brief Get the list of all QoS Map IDs.
 *
 * \param [in] unit Unit Number.
 * \param [in] array_size Size of allocated entries in map_id array.
 * \param [out] map_id QoS map id array.
 * \param [out] flags QoS flags array.
 * \param [out] count Actual number of QoS map id.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_qos_multi_get(
    int unit,
    int array_size,
    int *map_id,
    int *flags,
    int *count);

/*!
 * \brief Set ingress and egress VLAN port based related QoS setting.
 *
 * \param [in] unit Unit Number.
 * \param [in] port Ingress port.
 * \param [in] vid Vlan domain.
 * \param [in] ing_map Ingress QoS id.
 * \param [in] egr_map Egress QoS id.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_qos_port_vlan_map_set(
    int unit,
    bcm_port_t port,
    bcm_vlan_t vid,
    int ing_map,
    int egr_map);

/*!
 * \brief Get ingress and egress VLAN port based related QoS setting.
 *
 * \param [in] unit Unit Number.
 * \param [in] port Ingress port.
 * \param [in] vid Vlan domain.
 * \param [out] ing_map Ingress QoS id.
 * \param [out] egr_map Egress QoS id.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_qos_port_vlan_map_get(
    int unit,
    bcm_port_t port,
    bcm_vlan_t vid,
    int *ing_map,
    int *egr_map);

/*!
 * \brief Resolve QoS map id into mapping pointer and QoS map type.
 *
 * \param [in] unit Unit Number.
 * \param [in] map_id QoS map id.
 * \param [out] type QoS map type. NULL indicates not care.
 * \param [out] ptr Mapping pointer. NULL indicates not care.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_qos_map_id_resolve(
    int unit,
    int map_id,
    bcmi_ltsw_qos_map_type_t *type,
    int *ptr);

/*!
 * \brief Construct QoS map id with mapping pointer and QoS map type.
 *
 * \param [in] unit Unit Number.
 * \param [in] ptr Mapping pointer.
 * \param [in] type QoS map type.
 * \param [out] map_id QoS map id.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_qos_map_id_construct(
    int unit,
    int ptr,
    bcmi_ltsw_qos_map_type_t type,
    int *map_id);

/*!
 * \brief Get default mapping pointer.
 *
 * \param [in] unit Unit Number.
 * \param [in] type QoS map type.
 * \param [out] ptr Mapping pointer.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_qos_default_ptr_get(
    int unit,
    bcmi_ltsw_qos_map_type_t type,
    int *ptr);

/*!
 * \brief Get dummy operation pointer. The pointer indicates QoS remarking will do nothing with int_cn/int_pri/cng.
 *
 * \param [in] unit Unit Number.
 * \param [in] type QoS map type.
 * \param [out] ptr Mapping pointer.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_qos_dummy_ptr_get(
    int unit,
    bcmi_ltsw_qos_map_type_t type,
    int *ptr);

/*!
 * \brief Dump QoS module software bookkeepings.
 *
 * \param [in] unit Unit Number.
 */
extern void
mbcm_ltsw_qos_sw_dump(int unit);

/*!
 * \brief Set egress L2 map on a port based on egress VLAN priority source.
 *
 * \param [in] unit Unit Number.
 * \param [in] port Port id.
 * \param [in] src Source of egress VLAN priority.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_qos_port_egr_l2_map_set(
    int unit,
    bcm_port_t port,
    int src);

/*!
 * \brief Get egress VLAN priority source on a port based on egress L2 map.
 *
 * \param [in] unit Unit Number.
 * \param [in] port Port id.
 * \param [out] src Source of egress VLAN priority.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_qos_port_egr_l2_map_get(
    int unit,
    bcm_port_t port,
    int *src);

/*!
 * \brief Restore default QoS setting on a new port during flex port.
 *
 * \param [in] unit Unit Number.
 * \param [in] port Port id.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_qos_port_attach(
    int unit,
    bcm_port_t port);

/*!
 * \brief Clear default QoS setting on a to-be-removed port during flex port.
 *
 * \param [in] unit Unit Number.
 * \param [in] port Port id.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_qos_port_detach(
    int unit,
    bcm_port_t port);

/*!
 * \brief Initialize chip-specific QoS control info.
 *
 * \param [in] unit Unit Number.
 * \param [in|out] ctrl_info QoS control info pointer.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_qos_ctrl_info_init(
    int unit,
    const bcmint_qos_ctrl_info_t **ctrl_info);

/*!
 * \brief Get mapping pointer for Vxlan G/BUM flags remarking.
 *
 * \param [in] unit Unit Number.
 * \param [in] g_ena Enable G flag remarking.
 * \param [in] bum_ena Enable BUM flag remarking.
 * \param [out] ptr Mapping pointer.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_qos_vxlan_flags_map_ptr_get(
    int unit,
    int g_ena,
    int bum_ena,
    int *ptr);

/*!
 * \brief Get QoS LT info.
 *
 * \param [in] unit Unit Number.
 * \param [in] lt_db QoS LT info.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_qos_lt_db_get(
    int unit,
    bcmint_qos_lt_db_t *lt_db);

#endif /* BCMINT_LTSW_MBCM_QOS_H */
