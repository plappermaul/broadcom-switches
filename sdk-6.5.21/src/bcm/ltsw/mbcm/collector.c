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

#include <bcm_int/ltsw/mbcm/collector.h>
#include <bcm_int/ltsw/mbcm/mbcm.h>

#include <sal/sal_types.h>
#include <shr/shr_debug.h>

/******************************************************************************
 * Local definitions
 */

#define BSL_LOG_MODULE BSL_LS_BCM_COLLECTOR

/*!
 * \brief The Collector mbcm drivers.
 */
static mbcm_ltsw_collector_drv_t mbcm_ltsw_collector_drv[BCM_MAX_NUM_UNITS];

/******************************************************************************
 * Private functions
 */

/******************************************************************************
 * Public functions
 */

int
mbcm_ltsw_collector_drv_set(
    int unit,
    mbcm_ltsw_collector_drv_t *drv)
{
    mbcm_ltsw_collector_drv_t *local = &mbcm_ltsw_collector_drv[unit];

    SHR_FUNC_ENTER(unit);


    mbcm_ltsw_drv_init((mbcm_ltsw_drv_t *)local,
                       (mbcm_ltsw_drv_t *)drv,
                       sizeof(*local)/MBCM_LTSW_FNPTR_SZ);

    SHR_EXIT();

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_collector_export_profile_ref_count_update(
    int unit,
    int id,
    int update)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_collector_drv[unit].collector_export_profile_ref_count_update(unit, id, update));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_collector_ref_count_update(
    int unit,
    bcm_collector_t id,
    int update)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_collector_drv[unit].collector_ref_count_update(unit, id, update));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_collector_user_update(
    int unit,
    bcm_collector_t id,
    bcmi_ltsw_collector_export_app_t user)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_collector_drv[unit].collector_user_update(unit, id, user));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_collector_check_user_is_other(
    int unit,
    bcm_collector_t id,
    bcmi_ltsw_collector_export_app_t my_user)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_collector_drv[unit].collector_check_user_is_other(unit, id, my_user));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_collector_internal_id_get(
    int unit,
    bcm_collector_t id,
    uint32_t *internal_id,
    bcmi_ltsw_collector_transport_type_t *type)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_collector_drv[unit].collector_internal_id_get(unit, id, internal_id, type));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_collector_external_id_get(
    int unit,
    uint32_t internal_id,
    bcmi_ltsw_collector_transport_type_t type,
    bcm_collector_t *id)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_collector_drv[unit].collector_external_id_get(unit, internal_id, type, id));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_collector_mirror_dest_id_bk(
    int unit,
    bcm_collector_t id,
    bcm_gport_t mirror_dest_id)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_collector_drv[unit].collector_mirror_dest_id_bk(unit, id, mirror_dest_id));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_collector_mirror_dest_id_get(
    int unit,
    bcm_collector_t id,
    bcm_gport_t *mirror_dest_id)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_collector_drv[unit].collector_mirror_dest_id_get(unit, id, mirror_dest_id));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_collector_init(int unit)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_collector_drv[unit].collector_init(unit));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_collector_export_profile_create(
    int unit,
    uint32_t options,
    bcm_collector_t *id,
    bcm_collector_export_profile_t *export_profile_info)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_collector_drv[unit].collector_export_profile_create(unit, options, id, export_profile_info));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_collector_export_profile_get(
    int unit,
    int id,
    bcm_collector_export_profile_t *export_profile_info)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_collector_drv[unit].collector_export_profile_get(unit, id, export_profile_info));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_collector_export_profile_ids_get_all(
    int unit,
    int max_size,
    int *export_profile_ids_list,
    int *list_size)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_collector_drv[unit].collector_export_profile_ids_get_all(unit, max_size, export_profile_ids_list, list_size));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_collector_export_profile_destroy(
    int unit,
    int id)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_collector_drv[unit].collector_export_profile_destroy(unit, id));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_collector_create(
    int unit,
    uint32_t options,
    bcm_collector_t *id,
    bcm_collector_info_t *collector_info)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_collector_drv[unit].collector_create(unit, options, id, collector_info));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_collector_get(
    int unit,
    bcm_collector_t id,
    bcm_collector_info_t *collector_info)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_collector_drv[unit].collector_get(unit, id, collector_info));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_collector_get_all(
    int unit,
    int max_size,
    bcm_collector_t *collector_list,
    int *list_size)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_collector_drv[unit].collector_get_all(unit, max_size, collector_list, list_size));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_collector_destroy(
    int unit,
    bcm_collector_t id)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_collector_drv[unit].collector_destroy(unit, id));

exit:
    SHR_FUNC_EXIT();
}

