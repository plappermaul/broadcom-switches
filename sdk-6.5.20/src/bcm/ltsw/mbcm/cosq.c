/*
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated.
 * Edits to this file will be lost when it is regenerated.
 *
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

#include <bcm/types.h>

#include <bcm_int/ltsw/mbcm/cosq.h>
#include <bcm_int/ltsw/mbcm/mbcm.h>

#include <bcm_int/ltsw/cosq_int.h>
#include <shr/shr_debug.h>

/******************************************************************************
 * Local definitions
 */

#define BSL_LOG_MODULE BSL_LS_BCM_COSQ

/*!
 * \brief The Cosq mbcm drivers.
 */
static mbcm_ltsw_cosq_drv_t mbcm_ltsw_cosq_drv[BCM_MAX_NUM_UNITS];

/******************************************************************************
 * Private functions
 */

/******************************************************************************
 * Public functions
 */

int
mbcm_ltsw_cosq_drv_set(
    int unit,
    mbcm_ltsw_cosq_drv_t *drv)
{
    mbcm_ltsw_cosq_drv_t *local = &mbcm_ltsw_cosq_drv[unit];

    SHR_FUNC_ENTER(unit);


    mbcm_ltsw_drv_init((mbcm_ltsw_drv_t *)local,
                       (mbcm_ltsw_drv_t *)drv,
                       sizeof(*local)/MBCM_LTSW_FNPTR_SZ);

    SHR_EXIT();

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_cosq_init(int unit)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_cosq_drv[unit].cosq_init(unit));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_cosq_detach(int unit)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_cosq_drv[unit].cosq_detach(unit));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_cosq_map_db_init(
    int unit,
    const bcmint_cosq_map_db_t ** map_db)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_cosq_drv[unit].cosq_map_db_init(unit, map_db));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_cosq_queue_assignment_info_get(
    int unit,
    const char *info_id,
    bcmint_cosq_assignment_info_t *info)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_cosq_drv[unit].cosq_queue_assignment_info_get(unit, info_id, info));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_cosq_gport_port_resolve(
    int unit,
    bcm_gport_t gport,
    bcm_gport_t *port,
    int *id)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_cosq_drv[unit].cosq_gport_port_resolve(unit, gport, port, id));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_cosq_port_itm_validate(
    int unit,
    int port,
    int *itm)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_cosq_drv[unit].cosq_port_itm_validate(unit, port, itm));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_cosq_sw_dump(int unit)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_cosq_drv[unit].cosq_sw_dump(unit));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_cosq_bucket_set(
    int unit,
    bcm_gport_t gport,
    bcm_cos_queue_t cosq,
    uint32_t min_quantum,
    uint32_t max_quantum,
    bool burst_size_auto,
    uint32_t kbits_burst_min,
    uint32_t kbits_burst_max,
    uint32_t flags)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_cosq_drv[unit].cosq_bucket_set(unit, gport, cosq, min_quantum, max_quantum, burst_size_auto, kbits_burst_min, kbits_burst_max, flags));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_cosq_bucket_get(
    int unit,
    bcm_gport_t gport,
    bcm_cos_queue_t cosq,
    uint32_t *min_quantum,
    uint32_t *max_quantum,
    bool burst_size_auto,
    uint32_t *kbits_burst_min,
    uint32_t *kbits_burst_max,
    uint32_t *flags)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_cosq_drv[unit].cosq_bucket_get(unit, gport, cosq, min_quantum, max_quantum, burst_size_auto, kbits_burst_min, kbits_burst_max, flags));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_cosq_port_detach(
    int unit,
    bcm_port_t port)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_cosq_drv[unit].cosq_port_detach(unit, port));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_cosq_port_attach(
    int unit,
    bcm_port_t port)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_cosq_drv[unit].cosq_port_attach(unit, port));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_cosq_mc_cos_mirror_set(
    int unit,
    int enable)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_cosq_drv[unit].cosq_mc_cos_mirror_set(unit, enable));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_cosq_mc_cos_mirror_get(
    int unit,
    int *enable)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_cosq_drv[unit].cosq_mc_cos_mirror_get(unit, enable));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_cosq_sched_node_child_get(
    int unit,
    bcm_gport_t sched_gport,
    int *num_uc_child,
    int *uc_queue,
    int *num_mc_child,
    int *mc_queue)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_cosq_drv[unit].cosq_sched_node_child_get(unit, sched_gport, num_uc_child, uc_queue, num_mc_child, mc_queue));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_cosq_gport_get(
    int unit,
    bcm_gport_t gport,
    bcm_gport_t *port,
    int *numq,
    uint32_t *flags)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_cosq_drv[unit].cosq_gport_get(unit, gport, port, numq, flags));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_cosq_gport_child_get(
    int unit,
    bcm_gport_t in_gport,
    bcm_cos_queue_t cosq,
    bcm_gport_t *out_gport)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_cosq_drv[unit].cosq_gport_child_get(unit, in_gport, cosq, out_gport));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_cosq_gport_attach(
    int unit,
    bcm_gport_t input_gport,
    bcm_gport_t parent_gport,
    bcm_cos_queue_t cosq)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_cosq_drv[unit].cosq_gport_attach(unit, input_gport, parent_gport, cosq));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_cosq_gport_detach(
    int unit,
    bcm_gport_t input_gport,
    bcm_gport_t parent_gport,
    bcm_cos_queue_t cosq)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_cosq_drv[unit].cosq_gport_detach(unit, input_gport, parent_gport, cosq));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_cosq_gport_attach_get(
    int unit,
    bcm_gport_t sched_gport,
    bcm_gport_t *input_gport,
    bcm_cos_queue_t *cosq)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_cosq_drv[unit].cosq_gport_attach_get(unit, sched_gport, input_gport, cosq));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_cosq_gport_traverse(
    int unit,
    bcm_cosq_gport_traverse_cb cb,
    void *user_data)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_cosq_drv[unit].cosq_gport_traverse(unit, cb, user_data));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_cosq_sched_weight_max_get(
    int unit,
    int mode,
    int *weight_max)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_cosq_drv[unit].cosq_sched_weight_max_get(unit, mode, weight_max));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_cosq_schedq_mapping_set(
    int unit,
    int profile_index,
    int size,
    bcm_cosq_mapping_t *cosq_mapping_arr)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_cosq_drv[unit].cosq_schedq_mapping_set(unit, profile_index, size, cosq_mapping_arr));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_cosq_schedq_mapping_get(
    int unit,
    int profile_index,
    int array_max,
    bcm_cosq_mapping_t *cosq_mapping_arr,
    int *size)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_cosq_drv[unit].cosq_schedq_mapping_get(unit, profile_index, array_max, cosq_mapping_arr, size));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_cosq_port_scheduler_profile_set(
    int unit,
    bcm_port_t port,
    int profile_id)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_cosq_drv[unit].cosq_port_scheduler_profile_set(unit, port, profile_id));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_cosq_port_scheduler_profile_get(
    int unit,
    bcm_port_t port,
    int *profile_id)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_cosq_drv[unit].cosq_port_scheduler_profile_get(unit, port, profile_id));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_cosq_sched_set(
    int unit,
    bcm_port_t gport,
    bcm_cos_queue_t cosq,
    int mode,
    int weight)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_cosq_drv[unit].cosq_sched_set(unit, gport, cosq, mode, weight));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_cosq_sched_get(
    int unit,
    bcm_port_t gport,
    bcm_cos_queue_t cosq,
    int *mode,
    int *weight)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_cosq_drv[unit].cosq_sched_get(unit, gport, cosq, mode, weight));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_cosq_cosq_profile_max_num_get(
    int unit,
    bcm_cosq_profile_type_t profile_type,
    int *max_num)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_cosq_drv[unit].cosq_cosq_profile_max_num_get(unit, profile_type, max_num));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_cosq_port_sched_config_set(
    int unit,
    bcm_pbmp_t pbm,
    int mode,
    int weight_arr_size,
    int *weights)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_cosq_drv[unit].cosq_port_sched_config_set(unit, pbm, mode, weight_arr_size, weights));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_cosq_port_sched_config_get(
    int unit,
    bcm_pbmp_t pbm,
    int weight_arr_size,
    int *weights,
    int *weight_arr_count,
    int *mode)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_cosq_drv[unit].cosq_port_sched_config_get(unit, pbm, weight_arr_size, weights, weight_arr_count, mode));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_cosq_discard_set(
    int unit,
    uint32_t flags)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_cosq_drv[unit].cosq_discard_set(unit, flags));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_cosq_discard_get(
    int unit,
    uint32_t *flags)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_cosq_drv[unit].cosq_discard_get(unit, flags));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_cosq_gport_discard_set(
    int unit,
    bcm_gport_t gport,
    bcm_cos_queue_t cosq,
    bcm_cosq_gport_discard_t *discard)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_cosq_drv[unit].cosq_gport_discard_set(unit, gport, cosq, discard));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_cosq_gport_discard_get(
    int unit,
    bcm_gport_t gport,
    bcm_cos_queue_t cosq,
    bcm_cosq_gport_discard_t *discard)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_cosq_drv[unit].cosq_gport_discard_get(unit, gport, cosq, discard));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_cosq_discard_profile_create(
    int unit,
    int *profile_id)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_cosq_drv[unit].cosq_discard_profile_create(unit, profile_id));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_cosq_discard_profile_destroy(
    int unit,
    int profile_id)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_cosq_drv[unit].cosq_discard_profile_destroy(unit, profile_id));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_cosq_discard_profile_update(
    int unit,
    int profile_id,
    bcm_cosq_discard_rule_t *rule)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_cosq_drv[unit].cosq_discard_profile_update(unit, profile_id, rule));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_cosq_discard_profile_get(
    int unit,
    int profile_id,
    int max,
    bcm_cosq_discard_rule_t *rule_array,
    int *rule_count)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_cosq_drv[unit].cosq_discard_profile_get(unit, profile_id, max, rule_array, rule_count));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_cosq_cos_mapping_set(
    int unit,
    bcm_port_t local_port,
    bcm_cos_t priority,
    uint32_t flags,
    bcm_gport_t gport,
    bcm_cos_queue_t cosq)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_cosq_drv[unit].cosq_cos_mapping_set(unit, local_port, priority, flags, gport, cosq));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_cosq_cos_mapping_get(
    int unit,
    bcm_port_t local_port,
    bcm_cos_t priority,
    uint32_t flags,
    bcm_gport_t *gport,
    bcm_cos_queue_t *cosq)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_cosq_drv[unit].cosq_cos_mapping_get(unit, local_port, priority, flags, gport, cosq));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_cosq_shaper_config_get(
    int unit,
    int *refresh_time,
    int *itu_mode,
    int *shaper)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_cosq_drv[unit].cosq_shaper_config_get(unit, refresh_time, itu_mode, shaper));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_cosq_field_classifier_id_create(
    int unit,
    int *classifier_id)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_cosq_drv[unit].cosq_field_classifier_id_create(unit, classifier_id));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_cosq_field_classifier_id_destroy(
    int unit,
    int classifier_id)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_cosq_drv[unit].cosq_field_classifier_id_destroy(unit, classifier_id));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_cosq_field_classifier_map_set(
    int unit,
    int classifier_id,
    int count,
    bcm_cos_t *priority_array,
    bcm_cos_queue_t *cosq_array)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_cosq_drv[unit].cosq_field_classifier_map_set(unit, classifier_id, count, priority_array, cosq_array));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_cosq_field_classifier_map_get(
    int unit,
    int classifier_id,
    int array_max,
    bcm_cos_t *priority_array,
    bcm_cos_queue_t *cosq_array,
    int *array_count)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_cosq_drv[unit].cosq_field_classifier_map_get(unit, classifier_id, array_max, priority_array, cosq_array, array_count));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_cosq_field_classifier_map_clear(
    int unit,
    int classifier_id)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_cosq_drv[unit].cosq_field_classifier_map_clear(unit, classifier_id));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_cosq_field_hi_classifier_id_create(
    int unit,
    int *classifier_id)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_cosq_drv[unit].cosq_field_hi_classifier_id_create(unit, classifier_id));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_cosq_field_hi_classifier_id_destroy(
    int unit,
    int classifier_id)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_cosq_drv[unit].cosq_field_hi_classifier_id_destroy(unit, classifier_id));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_cosq_field_hi_classifier_map_set(
    int unit,
    int classifier_id,
    int count,
    bcm_cos_t *priority_array,
    bcm_cos_queue_t *cosq_array)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_cosq_drv[unit].cosq_field_hi_classifier_map_set(unit, classifier_id, count, priority_array, cosq_array));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_cosq_field_hi_classifier_map_get(
    int unit,
    int classifier_id,
    int array_max,
    bcm_cos_t *priority_array,
    bcm_cos_queue_t *cosq_array,
    int *array_count)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_cosq_drv[unit].cosq_field_hi_classifier_map_get(unit, classifier_id, array_max, priority_array, cosq_array, array_count));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_cosq_field_hi_classifier_map_clear(
    int unit,
    int classifier_id)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_cosq_drv[unit].cosq_field_hi_classifier_map_clear(unit, classifier_id));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_cosq_num_queue_get(
    int unit,
    int *num_ucq,
    int *num_mcq)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_cosq_drv[unit].cosq_num_queue_get(unit, num_ucq, num_mcq));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_cosq_port_map_info_get(
    int unit,
    bcm_port_t port,
    bcmi_ltsw_cosq_port_map_info_t *info)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_cosq_drv[unit].cosq_port_map_info_get(unit, port, info));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_cosq_device_info_get(
    int unit,
    bcmi_ltsw_cosq_device_info_t *info)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_cosq_drv[unit].cosq_device_info_get(unit, info));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_cosq_port_ct_mode_set(
    int unit,
    bcm_port_t port,
    int enable)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_cosq_drv[unit].cosq_port_ct_mode_set(unit, port, enable));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_cosq_port_ct_mode_get(
    int unit,
    bcm_port_t port,
    int *enable)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_cosq_drv[unit].cosq_port_ct_mode_get(unit, port, enable));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_cosq_port_asf_config_dump(
    int unit,
    bcm_port_t port,
    int port_speed)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_cosq_drv[unit].cosq_port_asf_config_dump(unit, port, port_speed));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_cosq_port_asf_config_show(
    int unit,
    bcm_port_t port,
    int port_speed)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_cosq_drv[unit].cosq_port_asf_config_show(unit, port, port_speed));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_cosq_rx_queue_channel_get(
    int unit,
    bcm_cos_queue_t queue_id,
    bcm_rx_chan_t chan_id,
    bool *enable)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_cosq_drv[unit].cosq_rx_queue_channel_get(unit, queue_id, chan_id, enable));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_cosq_rx_queue_channel_set(
    int unit,
    bcm_cos_queue_t queue_id,
    bcm_rx_chan_t chan_id,
    bool enable)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_cosq_drv[unit].cosq_rx_queue_channel_set(unit, queue_id, chan_id, enable));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_cosq_rx_queue_channel_set_test(
    int unit,
    bcm_cos_queue_t queue_id,
    bcm_rx_chan_t chan_id)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_cosq_drv[unit].cosq_rx_queue_channel_set_test(unit, queue_id, chan_id));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_cosq_gport_traverse_by_port(
    int unit,
    bcm_gport_t port,
    bcm_cosq_gport_traverse_cb cb,
    void *user_data)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_cosq_drv[unit].cosq_gport_traverse_by_port(unit, port, cb, user_data));

exit:
    SHR_FUNC_EXIT();
}

