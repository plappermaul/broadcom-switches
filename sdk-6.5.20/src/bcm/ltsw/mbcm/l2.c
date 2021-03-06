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

#include <bcm_int/ltsw/mbcm/l2.h>
#include <bcm_int/ltsw/mbcm/mbcm.h>

#include <shr/shr_debug.h>

/******************************************************************************
 * Local definitions
 */

#define BSL_LOG_MODULE BSL_LS_BCM_L2

/*!
 * \brief The L2 mbcm drivers.
 */
static mbcm_ltsw_l2_drv_t mbcm_ltsw_l2_drv[BCM_MAX_NUM_UNITS];

/******************************************************************************
 * Private functions
 */

/******************************************************************************
 * Public functions
 */

int
mbcm_ltsw_l2_drv_set(
    int unit,
    mbcm_ltsw_l2_drv_t *drv)
{
    mbcm_ltsw_l2_drv_t *local = &mbcm_ltsw_l2_drv[unit];

    SHR_FUNC_ENTER(unit);


    mbcm_ltsw_drv_init((mbcm_ltsw_drv_t *)local,
                       (mbcm_ltsw_drv_t *)drv,
                       sizeof(*local)/MBCM_LTSW_FNPTR_SZ);

    SHR_EXIT();

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_l2_init(int unit)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_l2_drv[unit].l2_init(unit));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_l2_detach(int unit)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_l2_drv[unit].l2_detach(unit));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_l2_addr_add(
    int unit,
    bcm_l2_addr_t *l2addr)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_l2_drv[unit].l2_addr_add(unit, l2addr));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_l2_addr_get(
    int unit,
    bcm_mac_t mac,
    bcm_vlan_t vid,
    bcm_l2_addr_t *l2addr)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_l2_drv[unit].l2_addr_get(unit, mac, vid, l2addr));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_l2_addr_delete(
    int unit,
    bcm_mac_t mac,
    bcm_vlan_t vid)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_l2_drv[unit].l2_addr_delete(unit, mac, vid));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_l2_addr_replace(
    int unit,
    bcmint_l2_replace_t *rep_st)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_l2_drv[unit].l2_addr_replace(unit, rep_st));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_l2_addr_traverse(
    int unit,
    bcm_l2_traverse_cb trav_fn,
    void *user_data)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_l2_drv[unit].l2_addr_traverse(unit, trav_fn, user_data));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_l2_addr_del_by_tuple(
    int unit,
    bcmint_l2_tuple_ctrl_t *tuple_ctrl)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_l2_drv[unit].l2_addr_del_by_tuple(unit, tuple_ctrl));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_l2_station_size_get(
    int unit,
    uint32_t *tbl_sz)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_l2_drv[unit].l2_station_size_get(unit, tbl_sz));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_l2_overlay_station_size_get(
    int unit,
    uint32_t *tbl_sz)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_l2_drv[unit].l2_overlay_station_size_get(unit, tbl_sz));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_l2_age(int unit)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_l2_drv[unit].l2_age(unit));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_l2_station_add(
    int unit,
    bcm_l2_station_t *station,
    int flags)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_l2_drv[unit].l2_station_add(unit, station, flags));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_l2_station_get(
    int unit,
    bcm_l2_station_t *station,
    int flags)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_l2_drv[unit].l2_station_get(unit, station, flags));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_l2_station_delete(
    int unit,
    bcm_l2_station_t *station,
    int flags)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_l2_drv[unit].l2_station_delete(unit, station, flags));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_l2_force_vlan_set(
    int unit,
    bcm_port_t port,
    bcm_vlan_t vlan,
    uint32_t flags)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_l2_drv[unit].l2_force_vlan_set(unit, port, vlan, flags));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_l2_learn_set(
    int unit,
    int enable)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_l2_drv[unit].l2_learn_set(unit, enable));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_l2_learn_get(
    int unit,
    int *enable)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_l2_drv[unit].l2_learn_get(unit, enable));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_l2_station_vlan_update(
    int unit,
    int vid,
    int flags,
    bcmi_ltsw_l2_station_vlan_t *vlan_info)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_l2_drv[unit].l2_station_vlan_update(unit, vid, flags, vlan_info));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_l2_size_get(
    int unit,
    int *size)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_l2_drv[unit].l2_size_get(unit, size));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_l2_count_get(
    int unit,
    int *count)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_l2_drv[unit].l2_count_get(unit, count));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_l2_cache_init(int unit)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_l2_drv[unit].l2_cache_init(unit));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_l2_cache_set(
    int unit,
    int index,
    bcm_l2_cache_addr_t *l2caddr,
    int *index_used)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_l2_drv[unit].l2_cache_set(unit, index, l2caddr, index_used));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_l2_cache_get(
    int unit,
    int index,
    bcm_l2_cache_addr_t *l2caddr)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_l2_drv[unit].l2_cache_get(unit, index, l2caddr));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_l2_cache_size_get(
    int unit,
    int *size)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_l2_drv[unit].l2_cache_size_get(unit, size));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_l2_cache_delete(
    int unit,
    int index)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_l2_drv[unit].l2_cache_delete(unit, index));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_l2_cache_delete_all(int unit)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_l2_drv[unit].l2_cache_delete_all(unit));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_l2_view_vlan_update(
    int unit,
    bcmi_ltsw_l2_view_vlan_info_t *info)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_l2_drv[unit].l2_view_vlan_update(unit, info));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_l2_opaque_tag_set(
    int unit,
    int opaque_tag_id,
    int tag_size,
    int ether_type,
    int tag_type,
    int valid)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_l2_drv[unit].l2_opaque_tag_set(unit, opaque_tag_id, tag_size, ether_type, tag_type, valid));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_l2_opaque_tag_get(
    int unit,
    int opaque_tag_id,
    int *tag_size,
    int *ether_type,
    int *tag_type,
    int *valid)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_l2_drv[unit].l2_opaque_tag_get(unit, opaque_tag_id, tag_size, ether_type, tag_type, valid));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_l2_payload_opaque_tag_set(
    int unit,
    int opaque_tag_id,
    int tag_size,
    int ether_type,
    int tag_type,
    int valid)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_l2_drv[unit].l2_payload_opaque_tag_set(unit, opaque_tag_id, tag_size, ether_type, tag_type, valid));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_l2_payload_opaque_tag_get(
    int unit,
    int opaque_tag_id,
    int *tag_size,
    int *ether_type,
    int *tag_type,
    int *valid)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_l2_drv[unit].l2_payload_opaque_tag_get(unit, opaque_tag_id, tag_size, ether_type, tag_type, valid));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_l2_frozen_cml_restore(
    int unit,
    int *l2_freeze_mode)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_l2_drv[unit].l2_frozen_cml_restore(unit, l2_freeze_mode));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_l2_frozen_cml_save(
    int unit,
    int *l2_freeze_mode)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_l2_drv[unit].l2_frozen_cml_save(unit, l2_freeze_mode));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_l2_frozen_cml_recover(
    int unit,
    int *l2_freeze_override)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_l2_drv[unit].l2_frozen_cml_recover(unit, l2_freeze_override));

exit:
    SHR_FUNC_EXIT();
}

void
mbcm_ltsw_l2_sw_dump(int unit)
{
    mbcm_ltsw_l2_drv[unit].l2_sw_dump(unit);
}

int
mbcm_ltsw_l2_virtual_info_update(
    int unit,
    bcmi_ltsw_l2_virtual_info_t *info)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_l2_drv[unit].l2_virtual_info_update(unit, info));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_l2_map_db_init(
    int unit,
    const bcmint_l2_map_db_t ** map_db)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_l2_drv[unit].l2_map_db_init(unit, map_db));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_l2_hitbit_detach(
    int unit,
    bcmint_l2_flexctr_info_t *info)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_l2_drv[unit].l2_hitbit_detach(unit, info));

exit:
    SHR_FUNC_EXIT();
}

int
mbcm_ltsw_l2_hitbit_init(
    int unit,
    bcmint_l2_flexctr_info_t *info)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_l2_drv[unit].l2_hitbit_init(unit, info));

exit:
    SHR_FUNC_EXIT();
}

