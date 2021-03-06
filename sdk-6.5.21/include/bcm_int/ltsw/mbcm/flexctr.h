/*
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated.
 * Edits to this file will be lost when it is regenerated.
 *
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

#ifndef BCMINT_LTSW_MBCM_FLEXCTR_H
#define BCMINT_LTSW_MBCM_FLEXCTR_H

#include <bcm/flexctr.h>

#include <bcm_int/ltsw/flexctr_int.h>

#include <sal/sal_types.h>

/*!
 * \brief Get flex counter source information.
 *
 * \param [in] unit Unit Number.
 * \param [in] source Flex source.
 * \param [out] source_info Flex counter source info.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*flexctr_source_info_get_f)(
    int unit,
    int source,
    bcmint_flexctr_source_info_t **source_info);

/*!
 * \brief Get flex counter object information.
 *
 * \param [in] unit Unit Number.
 * \param [in] object Flex object.
 * \param [out] object_info Flex counter object info.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*flexctr_object_info_get_f)(
    int unit,
    int object,
    bcmint_flexctr_object_info_t **object_info);

/*!
 * \brief Get overlaid objects for a specified flexctr object.
 *
 * \param [in] unit Unit Number.
 * \param [in] object Flex counter object.
 * \param [in] array_size Number of elements in overlay object array, 0 to query.
 * \param [out] overlay_object_array Overlay object array.
 * \param [out] count The actual number of overlaid objects.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*flexctr_object_overlay_multi_get_f)(
    int unit,
    bcm_flexctr_object_t object,
    int array_size,
    bcm_flexctr_object_t *overlay_object_array,
    int *count);

/*!
 * \brief Get flex counter object enumeration for given object ID.
 *
 * \param [in] unit Unit Number.
 * \param [in] object Flex counter object.
 * \param [in] object_id Flex counter object ID.
 * \param [out] symbol Object symbol in SDKLT.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*flexctr_object_enum_get_f)(
    int unit,
    int object,
    uint32_t object_id,
    const char **symbol);

/*!
 * \brief Flex counter trigger interval validation function.
 *
 * \param [in] unit Unit Number.
 * \param [in] interval Flex counter trigger interval.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*flexctr_trigger_interval_validate_f)(
    int unit,
    int interval);

/*!
 * \brief Flex counter flex action range get function.
 *
 * \param [in] unit Unit Number.
 * \param [in] stage Stage id.
 * \param [out] min Minimum flex action ID.
 * \param [out] max Maximum flex action ID.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*flexctr_flex_action_range_get_f)(
    int unit,
    int stage,
    uint32_t *min,
    uint32_t *max);

/*!
 * \brief flow count flex counter set.
 *
 * \param [in] unit Unit Number.
 * \param [in] val stat_counter_id.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*flexctr_flow_count_set_f)(
    int unit,
    int val);

/*!
 * \brief flow count flex counter get.
 *
 * \param [in] unit Unit Number.
 * \param [out] val stat_counter_id.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*flexctr_flow_count_get_f)(
    int unit,
    int *val);

/*!
 * \brief Get flex counter packet attribute information.
 *
 * \param [in] unit Unit Number.
 * \param [in] attr_type Packet attribute selector type.
 * \param [out] pkt_attr_info Flex counter packet attribute info.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*flexctr_pkt_attr_info_get_f)(
    int unit,
    int attr_type,
    bcmint_flexctr_pkt_attr_info_t **pkt_attr_info);

/*!
 * \brief Get flex counter packet attribute bus information.
 *
 * \param [in] unit Unit Number.
 * \param [in] stage Stage id.
 * \param [in] bit_pos Bit position in packet attribute bus.
 * \param [out] pkt_attr_bus_info Flex counter packet attribute bus info.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*flexctr_pkt_attr_bus_info_get_f)(
    int unit,
    int stage,
    int bit_pos,
    bcmint_flexctr_pkt_attr_bus_info_t **pkt_attr_bus_info);

/*!
 * \brief Get flex counter packet type attribute value.
 *
 * \param [in] unit Unit Number.
 * \param [in] pkt_type Flex counter packet type enumeration in HSDK API.
 * \param [out] value Flex counter packet type attribute value.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*flexctr_pkt_type_value_get_f)(
    int unit,
    int pkt_type,
    uint32_t *value);

/*!
 * \brief Get flex counter VLAN tag attribute value.
 *
 * \param [in] unit Unit Number.
 * \param [in] vlan_tag Flex counter VLAN tag enumeration in HSDK API.
 * \param [out] value Flex counter VLAN tag attribute value.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*flexctr_vlan_tag_value_get_f)(
    int unit,
    int vlan_tag,
    uint32_t *value);

/*!
 * \brief Get field opaque object value for specific flex counter action and counter index.
 *
 * \param [in] unit Unit Number.
 * \param [in] stat_counter_id Flex counter action identifier.
 * \param [in] counter_index Flex counter index.
 * \param [in] size Element number of object value array.
 * \param [out] obj_val Object value array.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*flexctr_field_opaque_object_value_get_f)(
    int unit,
    uint32_t stat_counter_id,
    uint32_t counter_index,
    uint32_t size,
    uint32_t *obj_val);

/*!
 * \brief Egress flex counter pre-select set.
 *
 * \param [in] unit Unit Number.
 * \param [in] val Pre-select value.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*flexctr_egr_presel_set_f)(
    int unit,
    int val);

/*!
 * \brief Egress flex counter pre-select get.
 *
 * \param [in] unit Unit Number.
 * \param [out] val Pre-select value.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*flexctr_egr_presel_get_f)(
    int unit,
    int *val);

/*!
 * \brief Get chip specific action scale enum map.
 *
 * \param [in] unit Unit Number.
 * \param [out] map Flexctr scale enum map.
 * \param [out] count Element count of scale enum array.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*flexctr_action_scale_enum_map_get_f)(
    int unit,
    const bcmint_flexctr_enum_map_t **map,
    uint32_t *count);

/*!
 * \brief Get chip specific quantization object enum map.
 *
 * \param [in] unit Unit Number.
 * \param [out] map Flexctr quantization object enum map.
 * \param [out] count Element count of quantization object enum array.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
typedef int (*flexctr_quant_object_enum_map_get_f)(
    int unit,
    const bcmint_flexctr_enum_map_t **map,
    uint32_t *count);

/*!
 * \brief Flexctr driver structure.
 */
typedef struct mbcm_ltsw_flexctr_drv_s {

    /*! Get flex counter source information. */
    flexctr_source_info_get_f flexctr_source_info_get;

    /*! Get flex counter object information. */
    flexctr_object_info_get_f flexctr_object_info_get;

    /*! Get overlaid objects for a specified flexctr object. */
    flexctr_object_overlay_multi_get_f flexctr_object_overlay_multi_get;

    /*! Get flex counter object enumeration for given object ID. */
    flexctr_object_enum_get_f flexctr_object_enum_get;

    /*! Flex counter trigger interval validation function. */
    flexctr_trigger_interval_validate_f flexctr_trigger_interval_validate;

    /*! Flex counter flex action range get function. */
    flexctr_flex_action_range_get_f flexctr_flex_action_range_get;

    /*! flow count flex counter set. */
    flexctr_flow_count_set_f flexctr_flow_count_set;

    /*! flow count flex counter get. */
    flexctr_flow_count_get_f flexctr_flow_count_get;

    /*! Get flex counter packet attribute information. */
    flexctr_pkt_attr_info_get_f flexctr_pkt_attr_info_get;

    /*! Get flex counter packet attribute bus information. */
    flexctr_pkt_attr_bus_info_get_f flexctr_pkt_attr_bus_info_get;

    /*! Get flex counter packet type attribute value. */
    flexctr_pkt_type_value_get_f flexctr_pkt_type_value_get;

    /*! Get flex counter VLAN tag attribute value. */
    flexctr_vlan_tag_value_get_f flexctr_vlan_tag_value_get;

    /*! Get field opaque object value for specific flex counter action and counter index. */
    flexctr_field_opaque_object_value_get_f flexctr_field_opaque_object_value_get;

    /*! Egress flex counter pre-select set. */
    flexctr_egr_presel_set_f flexctr_egr_presel_set;

    /*! Egress flex counter pre-select get. */
    flexctr_egr_presel_get_f flexctr_egr_presel_get;

    /*! Get chip specific action scale enum map. */
    flexctr_action_scale_enum_map_get_f flexctr_action_scale_enum_map_get;

    /*! Get chip specific quantization object enum map. */
    flexctr_quant_object_enum_map_get_f flexctr_quant_object_enum_map_get;

} mbcm_ltsw_flexctr_drv_t;

/*!
 * \brief Set the FLEXCTR driver of the device.
 *
 * \param [in] unit Unit Number.
 * \param [in] drv Flexctr driver to set.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_flexctr_drv_set(
    int unit,
    mbcm_ltsw_flexctr_drv_t *drv);

/*!
 * \brief Get flex counter source information.
 *
 * \param [in] unit Unit Number.
 * \param [in] source Flex source.
 * \param [out] source_info Flex counter source info.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_flexctr_source_info_get(
    int unit,
    int source,
    bcmint_flexctr_source_info_t **source_info);

/*!
 * \brief Get flex counter object information.
 *
 * \param [in] unit Unit Number.
 * \param [in] object Flex object.
 * \param [out] object_info Flex counter object info.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_flexctr_object_info_get(
    int unit,
    int object,
    bcmint_flexctr_object_info_t **object_info);

/*!
 * \brief Get overlaid objects for a specified flexctr object.
 *
 * \param [in] unit Unit Number.
 * \param [in] object Flex counter object.
 * \param [in] array_size Number of elements in overlay object array, 0 to query.
 * \param [out] overlay_object_array Overlay object array.
 * \param [out] count The actual number of overlaid objects.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_flexctr_object_overlay_multi_get(
    int unit,
    bcm_flexctr_object_t object,
    int array_size,
    bcm_flexctr_object_t *overlay_object_array,
    int *count);

/*!
 * \brief Get flex counter object enumeration for given object ID.
 *
 * \param [in] unit Unit Number.
 * \param [in] object Flex counter object.
 * \param [in] object_id Flex counter object ID.
 * \param [out] symbol Object symbol in SDKLT.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_flexctr_object_enum_get(
    int unit,
    int object,
    uint32_t object_id,
    const char **symbol);

/*!
 * \brief Flex counter trigger interval validation function.
 *
 * \param [in] unit Unit Number.
 * \param [in] interval Flex counter trigger interval.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_flexctr_trigger_interval_validate(
    int unit,
    int interval);

/*!
 * \brief Flex counter flex action range get function.
 *
 * \param [in] unit Unit Number.
 * \param [in] stage Stage id.
 * \param [out] min Minimum flex action ID.
 * \param [out] max Maximum flex action ID.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_flexctr_flex_action_range_get(
    int unit,
    int stage,
    uint32_t *min,
    uint32_t *max);

/*!
 * \brief flow count flex counter set.
 *
 * \param [in] unit Unit Number.
 * \param [in] val stat_counter_id.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_flexctr_flow_count_set(
    int unit,
    int val);

/*!
 * \brief flow count flex counter get.
 *
 * \param [in] unit Unit Number.
 * \param [out] val stat_counter_id.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_flexctr_flow_count_get(
    int unit,
    int *val);

/*!
 * \brief Get flex counter packet attribute information.
 *
 * \param [in] unit Unit Number.
 * \param [in] attr_type Packet attribute selector type.
 * \param [out] pkt_attr_info Flex counter packet attribute info.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_flexctr_pkt_attr_info_get(
    int unit,
    int attr_type,
    bcmint_flexctr_pkt_attr_info_t **pkt_attr_info);

/*!
 * \brief Get flex counter packet attribute bus information.
 *
 * \param [in] unit Unit Number.
 * \param [in] stage Stage id.
 * \param [in] bit_pos Bit position in packet attribute bus.
 * \param [out] pkt_attr_bus_info Flex counter packet attribute bus info.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_flexctr_pkt_attr_bus_info_get(
    int unit,
    int stage,
    int bit_pos,
    bcmint_flexctr_pkt_attr_bus_info_t **pkt_attr_bus_info);

/*!
 * \brief Get flex counter packet type attribute value.
 *
 * \param [in] unit Unit Number.
 * \param [in] pkt_type Flex counter packet type enumeration in HSDK API.
 * \param [out] value Flex counter packet type attribute value.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_flexctr_pkt_type_value_get(
    int unit,
    int pkt_type,
    uint32_t *value);

/*!
 * \brief Get flex counter VLAN tag attribute value.
 *
 * \param [in] unit Unit Number.
 * \param [in] vlan_tag Flex counter VLAN tag enumeration in HSDK API.
 * \param [out] value Flex counter VLAN tag attribute value.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_flexctr_vlan_tag_value_get(
    int unit,
    int vlan_tag,
    uint32_t *value);

/*!
 * \brief Get field opaque object value for specific flex counter action and counter index.
 *
 * \param [in] unit Unit Number.
 * \param [in] stat_counter_id Flex counter action identifier.
 * \param [in] counter_index Flex counter index.
 * \param [in] size Element number of object value array.
 * \param [out] obj_val Object value array.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_flexctr_field_opaque_object_value_get(
    int unit,
    uint32_t stat_counter_id,
    uint32_t counter_index,
    uint32_t size,
    uint32_t *obj_val);

/*!
 * \brief Egress flex counter pre-select set.
 *
 * \param [in] unit Unit Number.
 * \param [in] val Pre-select value.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_flexctr_egr_presel_set(
    int unit,
    int val);

/*!
 * \brief Egress flex counter pre-select get.
 *
 * \param [in] unit Unit Number.
 * \param [out] val Pre-select value.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_flexctr_egr_presel_get(
    int unit,
    int *val);

/*!
 * \brief Get chip specific action scale enum map.
 *
 * \param [in] unit Unit Number.
 * \param [out] map Flexctr scale enum map.
 * \param [out] count Element count of scale enum array.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_flexctr_action_scale_enum_map_get(
    int unit,
    const bcmint_flexctr_enum_map_t **map,
    uint32_t *count);

/*!
 * \brief Get chip specific quantization object enum map.
 *
 * \param [in] unit Unit Number.
 * \param [out] map Flexctr quantization object enum map.
 * \param [out] count Element count of quantization object enum array.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
mbcm_ltsw_flexctr_quant_object_enum_map_get(
    int unit,
    const bcmint_flexctr_enum_map_t **map,
    uint32_t *count);

#endif /* BCMINT_LTSW_MBCM_FLEXCTR_H */
