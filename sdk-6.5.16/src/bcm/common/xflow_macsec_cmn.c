/*
 * 
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2019 Broadcom Inc. All rights reserved.
 *
 * File:    xflow_macsec_cmn.c
 * Purpose: XFLOW_MACSEC software module intergation support
 */

#ifdef INCLUDE_XFLOW_MACSEC

#include <shared/bsl.h>

#include <soc/drv.h>
#include <soc/mem.h>
#include <soc/debug.h>
#include <soc/register.h>
#include <soc/memory.h>

#include <sal/core/libc.h>
#include <bcm/debug.h>

#include <bcm_int/common/xflow_macsec_cmn.h>

#include <bcm_int/control.h>
#include <bcm_int/esw_dispatch.h>
#include <bcm_int/esw/oam.h>

#define UNIT_VALID(unit) \
{ \
  if (!BCM_UNIT_VALID(unit)) { return BCM_E_UNIT; } \
}

/*
 * Function:
 *      bcm_common_xflow_macsec_init
 * Purpose:
 *      Initialize XFLOW_MACSEC for the specified unit.
 */
int _bcm_common_xflow_macsec_init(int unit)
{
    int rv = BCM_E_NONE;
    UNIT_VALID(unit);

    /* Initilaize the MACSEC module */
    if (soc_feature(unit, soc_feature_xflow_macsec)) {
        XFLOW_MACSEC_LOCK(unit);
        if (soc_feature(unit, soc_feature_xflow_macsec_stat)) {
            /* Register counter collection callback. */
            soc_counter_extra_register(unit, _xflow_macsec_counters_collect);
        }
        rv = _bcm_xflow_macsec_init(unit);
        XFLOW_MACSEC_UNLOCK(unit);
    }
    return rv;
}

/*
 * Function:
 *      bcm_common_xflow_macsec_deinit
 * Purpose:
 *      Deinitialize XFLOW_MACSEC for the specified unit.
 */
int _bcm_common_xflow_macsec_deinit(int unit)
{
    int rv = BCM_E_NONE;
    UNIT_VALID(unit);

    /* Deinitilaize the MACSEC module */
    if (soc_feature(unit, soc_feature_xflow_macsec)) {
        XFLOW_MACSEC_LOCK(unit);
        if (soc_feature(unit, soc_feature_xflow_macsec_stat)) {
            /* De-Register counter collection callback. */
            soc_counter_extra_unregister(unit, _xflow_macsec_counters_collect);
        }
        rv =  _bcm_xflow_macsec_deinit(unit);
        XFLOW_MACSEC_UNLOCK(unit);
    }
    return rv;
}

/*
 * Function:
 *      bcm_common_xflow_macsec_sync
 * Purpose:
 *      Warmboot sync XFLOW_MACSEC for the specified unit.
 */
int _bcm_common_xflow_macsec_sync(int unit)
{
    int rv = BCM_E_NONE;
    UNIT_VALID(unit);

#ifdef BCM_WARM_BOOT_SUPPORT
    /* Sync the MACSEC module */
    if (soc_feature(unit, soc_feature_xflow_macsec)) {
        XFLOW_MACSEC_LOCK(unit);
        rv = xflow_macsec_wb_sync(unit);
        XFLOW_MACSEC_UNLOCK(unit);
    }
#endif
    return rv;
}

/* Security Channel */

/*
 * Function:
 *      bcm_common_xflow_macsec_secure_chan_create
 * Purpose:
 *      Create an XFLOW_MACSEC security channel
 */
int
bcm_common_xflow_macsec_secure_chan_create(int unit,
        uint32 flags,
        bcm_xflow_macsec_instance_id_t instance_id,
        bcm_xflow_macsec_secure_chan_info_t *chan_info,
        int priority,
        bcm_xflow_macsec_secure_chan_id_t *chan_id)
{
    int rv = BCM_E_UNAVAIL;
    UNIT_VALID(unit);

    if (soc_feature(unit, soc_feature_xflow_macsec)) {
        XFLOW_MACSEC_LOCK(unit);
        rv = xflow_macsec_secure_chan_create(unit, flags,
                                            (xflow_macsec_secure_chan_info_t *)chan_info,
                                            priority,
                                            (xflow_macsec_secure_chan_id_t *)chan_id);
        XFLOW_MACSEC_UNLOCK(unit);
    }
    return rv;
}

/*
 * Function:
 *      bcm_common_xflow_macsec_secure_chan_set
 * Purpose:
 *      Sets/Resets the value of one of the parameters 
 *      of a given security channel.
 */
int
bcm_common_xflow_macsec_secure_chan_set(int unit,
        uint32 flags,
        bcm_xflow_macsec_secure_chan_id_t chan_id,
        bcm_xflow_macsec_secure_chan_info_t *chan_info,
        int priority)
{
    int rv = BCM_E_UNAVAIL;
    UNIT_VALID(unit);

    if (soc_feature(unit, soc_feature_xflow_macsec)) {
        XFLOW_MACSEC_LOCK(unit);
        rv = xflow_macsec_secure_chan_set(unit, flags, chan_id,
                       (xflow_macsec_secure_chan_info_t *)chan_info, priority);
        XFLOW_MACSEC_UNLOCK(unit);
    }
    return rv;
}

/*
 * Function:
 *      bcm_common_xflow_macsec_secure_chan_get
 * Purpose:
 *      Get the security channel configuration for a given SC index.
 */
int
bcm_common_xflow_macsec_secure_chan_get(int unit,
        bcm_xflow_macsec_secure_chan_id_t chan_id,
        bcm_xflow_macsec_secure_chan_info_t *chan_info,
        int *priority)
{
    int rv = BCM_E_UNAVAIL;
    UNIT_VALID(unit);

    if (soc_feature(unit, soc_feature_xflow_macsec)) {
        XFLOW_MACSEC_LOCK(unit);
        rv = xflow_macsec_secure_chan_get(unit, chan_id, chan_info, priority);
        XFLOW_MACSEC_UNLOCK(unit);
    }
    return rv;
}

/*
 * Function:
 *      bcm_common_xflow_macsec_secure_chan_destroy
 * Purpose:
 *      Delete the given security channel. The corresponding SA must be
 *      destroyed first.
 */
int
bcm_common_xflow_macsec_secure_chan_destroy(int unit,
        bcm_xflow_macsec_secure_chan_id_t chan_id)
{
    int rv = BCM_E_UNAVAIL;
    UNIT_VALID(unit);

    if (soc_feature(unit, soc_feature_xflow_macsec)) {
        XFLOW_MACSEC_LOCK(unit);
        rv = xflow_macsec_secure_chan_destroy(unit, chan_id);
        XFLOW_MACSEC_UNLOCK(unit);
    }
    return rv;
}

/*
 * Function:
 *      bcm_common_xflow_macsec_secure_chan_info_t_init
 * Purpose:
 *      Initialize the data structure.
 */
void
bcm_xflow_macsec_secure_chan_info_t_init(
        bcm_xflow_macsec_secure_chan_info_t *chan_info)
{
        xflow_macsec_secure_chan_info_t_init(chan_info);
}

/*
 * Function:
 *      bcm_common_xflow_macsec_secure_chan_enable_set
 * Purpose:
 *      Enable the Security Channel.
 */
int
bcm_common_xflow_macsec_secure_chan_enable_set(int unit,
        bcm_xflow_macsec_secure_chan_id_t chan_id,
        int enable)
{
    int rv = BCM_E_UNAVAIL;
    UNIT_VALID(unit);

    if (soc_feature(unit, soc_feature_xflow_macsec)) {
        XFLOW_MACSEC_LOCK(unit);
        rv = xflow_macsec_secure_chan_enable_set(unit, chan_id, enable);
        XFLOW_MACSEC_UNLOCK(unit);
    }
    return rv;
}

/*
 * Function:
 *      bcm_common_xflow_macsec_secure_chan_enable_get
 * Purpose:
 *      Check if the Security Channel is enabled.
 */
int
bcm_common_xflow_macsec_secure_chan_enable_get(int unit,
        bcm_xflow_macsec_secure_chan_id_t chan_id,
        int *enable)
{
    int rv = BCM_E_UNAVAIL;
    UNIT_VALID(unit);

    if (soc_feature(unit, soc_feature_xflow_macsec)) {
        XFLOW_MACSEC_LOCK(unit);
        rv = xflow_macsec_secure_chan_enable_get(unit, chan_id, enable);
        XFLOW_MACSEC_UNLOCK(unit);
    }
    return rv;
}



/* Security Association */

/*
 * Function:
 *      bcm_common_xflow_macsec_secure_assoc_create
 * Purpose:
 *      Create the Security Association for the 
 *      given Security Channel.
 */
int
bcm_common_xflow_macsec_secure_assoc_create(int unit,
        uint32 flags,
        bcm_xflow_macsec_secure_chan_id_t chan_id,
        bcm_xflow_macsec_secure_assoc_info_t *assoc_info,
        bcm_xflow_macsec_secure_assoc_id_t *assoc_id)
{
    int rv = BCM_E_UNAVAIL;
    UNIT_VALID(unit);

    if (soc_feature(unit, soc_feature_xflow_macsec)) {
        XFLOW_MACSEC_LOCK(unit);
        rv = xflow_macsec_secure_assoc_create(unit, flags,
                                              chan_id, assoc_info, assoc_id);
        XFLOW_MACSEC_UNLOCK(unit);
    }
    return rv;
}

/*
 * Function:
 *      bcm_common_xflow_macsec_secure_assoc_set
 * Purpose:
 *      Sets/Resets the value of any of the parameters of 
 *      a given security association.
 */
int
bcm_common_xflow_macsec_secure_assoc_set(int unit,
        bcm_xflow_macsec_secure_assoc_id_t assoc_id,
        bcm_xflow_macsec_secure_assoc_info_t *assoc_info)
{
    int rv = BCM_E_UNAVAIL;
    UNIT_VALID(unit);

    if (soc_feature(unit, soc_feature_xflow_macsec)) {
        XFLOW_MACSEC_LOCK(unit);
        rv = xflow_macsec_secure_assoc_set(unit, assoc_id,
                    (xflow_macsec_secure_assoc_info_t *)assoc_info);
        XFLOW_MACSEC_UNLOCK(unit);
    }
    return rv;
}

/*
 * Function:
 *      bcm_common_xflow_macsec_secure_assoc_get
 * Purpose:
 *      Retrieve the Security Association configuration and the
 *      SC index from the SA index.
 */
int
bcm_common_xflow_macsec_secure_assoc_get(int unit,
        bcm_xflow_macsec_secure_assoc_id_t assoc_id,
        bcm_xflow_macsec_secure_assoc_info_t *assoc_info,
        bcm_xflow_macsec_secure_chan_id_t *chan_id)
{
    int rv = BCM_E_UNAVAIL;
    UNIT_VALID(unit);

    if (soc_feature(unit, soc_feature_xflow_macsec)) {
        XFLOW_MACSEC_LOCK(unit);
        rv = xflow_macsec_secure_assoc_get(unit, assoc_id, assoc_info, chan_id);
        XFLOW_MACSEC_UNLOCK(unit);
    }
    return rv;
}

/*
 * Function:
 *      bcm_common_xflow_macsec_secure_assoc_destroy
 * Purpose:
 *      Deletes the SA entry corresponding to the Security Association.
 */
int
bcm_common_xflow_macsec_secure_assoc_destroy(int unit,
        bcm_xflow_macsec_secure_assoc_id_t assoc_id)
{
    int rv = BCM_E_UNAVAIL;
    UNIT_VALID(unit);

    if (soc_feature(unit, soc_feature_xflow_macsec)) {
        XFLOW_MACSEC_LOCK(unit);
        rv = xflow_macsec_secure_assoc_destroy(unit, assoc_id);
        XFLOW_MACSEC_UNLOCK(unit);
    }
    return rv;
}

/*
 * Function:
 *      bcm_common_xflow_macsec_secure_assoc_info_t_init
 * Purpose:
 *      Initialize the Security Association data structure.
 */
void
bcm_xflow_macsec_secure_assoc_info_t_init(
        bcm_xflow_macsec_secure_assoc_info_t *assoc_info)
{
    xflow_macsec_secure_assoc_info_t_init(assoc_info);
}

/*
 * Function:
 *      bcm_common_xflow_macsec_decrypt_policy_create
 * Purpose:
 */
int
bcm_common_xflow_macsec_decrypt_policy_create(
    int unit,
    uint32 flags,
    bcm_xflow_macsec_instance_id_t instance_id,
    bcm_xflow_macsec_decrypt_policy_info_t *policy_info,
    bcm_xflow_macsec_policy_id_t *policy_id)
{
    int rv = BCM_E_UNAVAIL;
    UNIT_VALID(unit);

    if (soc_feature(unit, soc_feature_xflow_macsec)) {
        XFLOW_MACSEC_LOCK(unit);
        flags |= XFLOW_MACSEC_DECRYPT;
        rv = xflow_macsec_policy_create(unit, flags, policy_info, policy_id);
        XFLOW_MACSEC_UNLOCK(unit);
    }
    return rv;
}

/*
 * Function:
 *      bcm_common_xflow_macsec_decrypt_policy_destroy
 * Purpose:
 */
int
bcm_common_xflow_macsec_decrypt_policy_destroy(
    int unit,
    bcm_xflow_macsec_policy_id_t policy_id)
{
    int rv = BCM_E_UNAVAIL;
    UNIT_VALID(unit);

    if (soc_feature(unit, soc_feature_xflow_macsec)) {
        XFLOW_MACSEC_LOCK(unit);
        rv = xflow_macsec_policy_destroy(unit, policy_id);
        XFLOW_MACSEC_UNLOCK(unit);
    }
    return rv;
}

/*
 * Function:
 *      bcm_common_xflow_macsec_decrypt_policy_get
 * Purpose:
 */
int
bcm_common_xflow_macsec_decrypt_policy_get(
    int unit,
    bcm_xflow_macsec_policy_id_t policy_id,
    bcm_xflow_macsec_decrypt_policy_info_t *policy_info)
{
    int rv = BCM_E_UNAVAIL;
    UNIT_VALID(unit);

    if (soc_feature(unit, soc_feature_xflow_macsec)) {
        XFLOW_MACSEC_LOCK(unit);
        rv = xflow_macsec_policy_get(unit, policy_id, policy_info);
        XFLOW_MACSEC_UNLOCK(unit);
    }
    return rv;
}

/*
 * Function:
 *      bcm_common_xflow_macsec_decrypt_policy_set
 * Purpose:
 */
int
bcm_common_xflow_macsec_decrypt_policy_set(
    int unit,
    bcm_xflow_macsec_policy_id_t policy_id,
    bcm_xflow_macsec_decrypt_policy_info_t *policy_info)

{
    int rv = BCM_E_UNAVAIL;
    UNIT_VALID(unit);

    if (soc_feature(unit, soc_feature_xflow_macsec)) {
        XFLOW_MACSEC_LOCK(unit);
        rv = xflow_macsec_policy_set(unit, policy_id, policy_info);
        XFLOW_MACSEC_UNLOCK(unit);
    }
    return rv;
}

/*
 * Function:
 *      bcm_xflow_macsec_decrypt_policy_info_t_init
 * Purpose:
 *      Initialize the data structure.
 */
void
bcm_xflow_macsec_decrypt_policy_info_t_init(
        bcm_xflow_macsec_decrypt_policy_info_t *policy_info)
{
        xflow_macsec_policy_info_t_init(policy_info);
}

/*
 * Function:
 *      bcm_common_xflow_macsec_decrypt_flow_create
 * Purpose:
 */
int
bcm_common_xflow_macsec_decrypt_flow_create(int unit,
    uint32 flags,
    bcm_xflow_macsec_instance_id_t instance_id,
    bcm_xflow_macsec_decrypt_flow_info_t *flow_info,
    int priority,
    bcm_xflow_macsec_flow_id_t *flow_id)
{
    int rv = BCM_E_UNAVAIL;
    UNIT_VALID(unit);

    if (soc_feature(unit, soc_feature_xflow_macsec)) {
        XFLOW_MACSEC_LOCK(unit);
        flags |= XFLOW_MACSEC_DECRYPT;
        rv = xflow_macsec_flow_create(unit, flags, flow_info, priority,
                                      flow_id);
        XFLOW_MACSEC_UNLOCK(unit);
    }
    return rv;
}

/*
 * Function:
 *      bcm_common_xflow_macsec_decrypt_flow_destroy
 * Purpose:
 */
int
bcm_common_xflow_macsec_decrypt_flow_destroy(int unit,
    bcm_xflow_macsec_flow_id_t flow_id)
{
    int rv = BCM_E_UNAVAIL;
    UNIT_VALID(unit);

    if (soc_feature(unit, soc_feature_xflow_macsec)) {
        XFLOW_MACSEC_LOCK(unit);
        rv = xflow_macsec_flow_destroy(unit, flow_id);
        XFLOW_MACSEC_UNLOCK(unit);
    }
    return rv;
}

/*
 * Function:
 *      bcm_common_xflow_macsec_decrypt_flow_enable_get
 * Purpose:
 */
int
bcm_common_xflow_macsec_decrypt_flow_enable_get(int unit,
    bcm_xflow_macsec_flow_id_t flow_id,
    int *enable)

{
    int rv = BCM_E_UNAVAIL;
    UNIT_VALID(unit);

    if (soc_feature(unit, soc_feature_xflow_macsec)) {
        XFLOW_MACSEC_LOCK(unit);
        rv = xflow_macsec_flow_enable_get(unit, flow_id, enable);
        XFLOW_MACSEC_UNLOCK(unit);
    }
    return rv;
}

/*
 * Function:
 *      bcm_common_xflow_macsec_decrypt_flow_enable_set
 * Purpose:
 */
int
bcm_common_xflow_macsec_decrypt_flow_enable_set(int unit,
    bcm_xflow_macsec_flow_id_t flow_id,
    int enable)
{
    int rv = BCM_E_UNAVAIL;
    UNIT_VALID(unit);

    if (soc_feature(unit, soc_feature_xflow_macsec)) {
        XFLOW_MACSEC_LOCK(unit);
        rv = xflow_macsec_flow_enable_set(unit, flow_id, enable);
        XFLOW_MACSEC_UNLOCK(unit);
    }
    return rv;
}

/*
 * Function:
 *      bcm_common_xflow_macsec_decrypt_flow_get
 * Purpose:
 */
int
bcm_common_xflow_macsec_decrypt_flow_get(int unit,
    bcm_xflow_macsec_flow_id_t flow_id,
    bcm_xflow_macsec_decrypt_flow_info_t *flow_info,
    int *priority)
{
    int rv = BCM_E_UNAVAIL;
    UNIT_VALID(unit);

    if (soc_feature(unit, soc_feature_xflow_macsec)) {
        XFLOW_MACSEC_LOCK(unit);
        rv = xflow_macsec_flow_get(unit, flow_id, flow_info, priority);
        XFLOW_MACSEC_UNLOCK(unit);
    }
    return rv;
}

/*
 * Function:
 *      bcm_common_xflow_macsec_decrypt_flow_set
 * Purpose:
 */
int
bcm_common_xflow_macsec_decrypt_flow_set(int unit,
    bcm_xflow_macsec_flow_id_t flow_id,
    bcm_xflow_macsec_decrypt_flow_info_t *flow_info,
    int priority)

{
    int rv = BCM_E_UNAVAIL;
    UNIT_VALID(unit);

    if (soc_feature(unit, soc_feature_xflow_macsec)) {
        XFLOW_MACSEC_LOCK(unit);
        rv = xflow_macsec_flow_set(unit, flow_id, flow_info, priority);
        XFLOW_MACSEC_UNLOCK(unit);
    }
    return rv;
}

/*
 * Function:
 *      bcm_xflow_macsec_decrypt_flow_info_t_init
 * Purpose:
 *      Initialize the data structure.
 */
void
bcm_xflow_macsec_decrypt_flow_info_t_init(
        bcm_xflow_macsec_decrypt_flow_info_t *flow_info)
{
        xflow_macsec_flow_info_t_init(flow_info);
}

/*
 * Function:
 *      bcm_common_xflow_macsec_subport_id_get
 * Purpose:
 */
int
bcm_common_xflow_macsec_subport_id_get(int unit,
    bcm_xflow_macsec_id_t id,
    bcm_xflow_macsec_subport_id_t *macsec_subport_id)
{
    int rv = BCM_E_UNAVAIL;
    UNIT_VALID(unit);

    if (soc_feature(unit, soc_feature_xflow_macsec)) {
        XFLOW_MACSEC_LOCK(unit);
        rv = xflow_macsec_subport_id_get(unit, id, macsec_subport_id);
        XFLOW_MACSEC_UNLOCK(unit);
    }
    return rv;
}

/*
 * Function:
 *      bcm_common_xflow_macsec_control_set
 * Purpose:
 *      Sets the value in HW for the macsec control type provided.
 */
int
bcm_common_xflow_macsec_control_set(int unit, uint32 flags,
                    bcm_xflow_macsec_instance_id_t instance_id,
                    bcm_xflow_macsec_control_t type, uint64 value)
{
    int rv = BCM_E_UNAVAIL;
    UNIT_VALID(unit);

    if (soc_feature(unit, soc_feature_xflow_macsec)) {
        XFLOW_MACSEC_LOCK(unit);
        rv = xflow_macsec_control_set(unit, flags, type, value);
        XFLOW_MACSEC_UNLOCK(unit);
    }
    return rv;
}

/*
 * Function:
 *      bcm_common_xflow_macsec_control_get
 * Purpose:
 *      Gets the value from HW for the macsec control type provided.
 */
int
bcm_common_xflow_macsec_control_get(int unit, uint32 flags,
                    bcm_xflow_macsec_instance_id_t instance_id,
                    bcm_xflow_macsec_control_t type, uint64 *value)
{
    int rv = BCM_E_UNAVAIL;
    UNIT_VALID(unit);

    if (soc_feature(unit, soc_feature_xflow_macsec)) {
        XFLOW_MACSEC_LOCK(unit);
        rv = xflow_macsec_control_get(unit, flags, type, value);
        XFLOW_MACSEC_UNLOCK(unit);
    }
    return rv;
}

int
bcm_common_xflow_macsec_stat_get(
    int unit, 
    uint32 flags, 
    bcm_xflow_macsec_id_t id, 
    bcm_xflow_macsec_stat_type_t  stat_type, 
    uint64 *value)
{
    int rv = BCM_E_UNAVAIL;
    UNIT_VALID(unit);

    if (soc_feature(unit, soc_feature_xflow_macsec_stat)) {
        XFLOW_MACSEC_LOCK(unit);
        rv = xflow_macsec_stat_get(unit, flags, id, stat_type, value);
        XFLOW_MACSEC_UNLOCK(unit);
    }
    return rv;
}

int
bcm_common_xflow_macsec_stat_set(
    int unit, 
    uint32 flags, 
    bcm_xflow_macsec_id_t id, 
    bcm_xflow_macsec_stat_type_t  stat_type, 
    uint64 value)
{
    int rv = BCM_E_UNAVAIL;
    UNIT_VALID(unit);

    if (soc_feature(unit, soc_feature_xflow_macsec_stat)) {
        XFLOW_MACSEC_LOCK(unit);
        rv = xflow_macsec_stat_set(unit, flags, id, stat_type, value);
        XFLOW_MACSEC_UNLOCK(unit);
    }
    return rv;
}

int
bcm_common_xflow_macsec_stat_multi_get(
    int unit,
    uint32 flags,
    bcm_xflow_macsec_id_t id,
    uint32 num_stats,
    bcm_xflow_macsec_stat_type_t  *stat_type_array,
    uint64 *value_array)
{
    int rv = BCM_E_UNAVAIL;
    UNIT_VALID(unit);

    if (soc_feature(unit, soc_feature_xflow_macsec_stat)) {
        XFLOW_MACSEC_LOCK(unit);
        rv = xflow_macsec_stat_multi_get(unit, flags, id, num_stats,
                    (xflow_macsec_stat_type_t *)stat_type_array, value_array);
        XFLOW_MACSEC_UNLOCK(unit);
    }
    return rv;
}

int
bcm_common_xflow_macsec_stat_multi_set(
    int unit,
    uint32 flags,
    bcm_xflow_macsec_id_t id,
    uint32 num_stats,
    bcm_xflow_macsec_stat_type_t  *stat_type_array,
    uint64 *value_array)
{
    int rv = BCM_E_UNAVAIL;
    UNIT_VALID(unit);

    if (soc_feature(unit, soc_feature_xflow_macsec_stat)) {
        XFLOW_MACSEC_LOCK(unit);
        rv = xflow_macsec_stat_multi_set(unit, flags, id, num_stats,
                    (xflow_macsec_stat_type_t *)stat_type_array, value_array);
        XFLOW_MACSEC_UNLOCK(unit);
    }
    return rv;
}

int
bcm_common_xflow_macsec_vlan_tpid_array_set(
    int unit,
    bcm_xflow_macsec_instance_id_t instance_id,
    bcm_xflow_macsec_vlan_tpid_t *vlan_tpid)
{
    int rv = BCM_E_UNAVAIL;
    UNIT_VALID(unit);

    if (soc_feature(unit, soc_feature_xflow_macsec)) {
        XFLOW_MACSEC_LOCK(unit);
        rv = xflow_macsec_vlan_tpid_array_set(unit, vlan_tpid);
        XFLOW_MACSEC_UNLOCK(unit);
    }
    return rv;
}

int
bcm_common_xflow_macsec_vlan_tpid_array_get(
    int unit,
    bcm_xflow_macsec_instance_id_t instance_id,
    bcm_xflow_macsec_vlan_tpid_t *vlan_tpid)
{
    int rv = BCM_E_UNAVAIL;
    UNIT_VALID(unit);

    if (soc_feature(unit, soc_feature_xflow_macsec)) {
        XFLOW_MACSEC_LOCK(unit);
        rv = xflow_macsec_vlan_tpid_array_get(unit, vlan_tpid);
        XFLOW_MACSEC_UNLOCK(unit);
    }
    return rv;
}

int
bcm_common_xflow_macsec_vlan_tpid_array_index_get(
    int unit,
    bcm_xflow_macsec_instance_id_t instance_id,
    uint32 vlan_tpid,
    uint8 *tpid_index_sel)
{
    int rv = BCM_E_UNAVAIL;
    UNIT_VALID(unit);

    if (soc_feature(unit, soc_feature_xflow_macsec)) {
        XFLOW_MACSEC_LOCK(unit);
        rv = xflow_macsec_vlan_tpid_array_index_get(unit, vlan_tpid,
                                                    tpid_index_sel);
        XFLOW_MACSEC_UNLOCK(unit);
    }
    return rv;
}

int bcm_common_xflow_macsec_mtu_set(
    int unit,
    int flags,
    bcm_xflow_macsec_instance_id_t instance_id,
    uint32 mtu,
    bcm_xflow_macsec_mtu_t *mtu_sel)
{
    int rv = BCM_E_UNAVAIL;
    UNIT_VALID(unit);

    if (soc_feature(unit, soc_feature_xflow_macsec)) {
        XFLOW_MACSEC_LOCK(unit);
        rv = xflow_macsec_mtu_set(unit, flags, mtu,
                                  (xflow_macsec_mtu_t *)mtu_sel);
        XFLOW_MACSEC_UNLOCK(unit);
    }
    return rv;
}

int bcm_common_xflow_macsec_mtu_get(
    int unit,
    int flags,
    bcm_xflow_macsec_instance_id_t instance_id,
    bcm_xflow_macsec_mtu_t mtu_sel,
    uint32 *mtu)
{
    int rv = BCM_E_UNAVAIL;
    UNIT_VALID(unit);

    if (soc_feature(unit, soc_feature_xflow_macsec)) {
        XFLOW_MACSEC_LOCK(unit);
        rv = xflow_macsec_mtu_get(unit, flags, mtu_sel, mtu);
        XFLOW_MACSEC_UNLOCK(unit);
    }
    return rv;
}

int bcm_common_xflow_macsec_sectag_etype_set(
    int unit,
    int flags,
    bcm_xflow_macsec_instance_id_t instance_id,
    uint32 sectag_etype,
    bcm_xflow_macsec_sectag_ethertype_t *sectag_etype_sel)
{
    int rv = BCM_E_UNAVAIL;
    UNIT_VALID(unit);

    if (soc_feature(unit, soc_feature_xflow_macsec)) {
        XFLOW_MACSEC_LOCK(unit);
        rv = xflow_macsec_sectag_etype_set(unit, flags, sectag_etype,
                        (xflow_macsec_sectag_ethertype_t *)sectag_etype_sel);
        XFLOW_MACSEC_UNLOCK(unit);
    }
    return rv;
}

int bcm_common_xflow_macsec_sectag_etype_get(
    int unit,
    bcm_xflow_macsec_instance_id_t instance_id,
    bcm_xflow_macsec_sectag_ethertype_t sectag_etype_sel,
    uint32 *sectag_etype)
{
    int rv = BCM_E_UNAVAIL;
    UNIT_VALID(unit);

    if (soc_feature(unit, soc_feature_xflow_macsec)) {
        XFLOW_MACSEC_LOCK(unit);
        rv = xflow_macsec_sectag_etype_get(unit, sectag_etype_sel,
                                           sectag_etype);
        XFLOW_MACSEC_UNLOCK(unit);
    }
    return rv;
}

int bcm_common_xflow_macsec_secure_chan_info_traverse (
    int unit,
    uint32 flags,
    bcm_xflow_macsec_instance_id_t instance_id,
    bcm_xflow_macsec_chan_traverse_cb callback,
    void *user_data)
{
    int rv = BCM_E_UNAVAIL;
    UNIT_VALID(unit);

    if (soc_feature(unit, soc_feature_xflow_macsec)) {
        XFLOW_MACSEC_LOCK(unit);
        rv = xflow_macsec_secure_chan_info_traverse(unit, flags, callback,
                                                    user_data);
        XFLOW_MACSEC_UNLOCK(unit);
    }
    return rv;
}

int bcm_common_xflow_macsec_secure_assoc_traverse(
    int unit,
    bcm_xflow_macsec_secure_chan_id_t chan_id,
    bcm_xflow_macsec_secure_assoc_traverse_cb callback,
    void *user_data)
{
    int rv = BCM_E_UNAVAIL;
    UNIT_VALID(unit);

    if (soc_feature(unit, soc_feature_xflow_macsec)) {
        XFLOW_MACSEC_LOCK(unit);
        rv = xflow_macsec_secure_assoc_traverse(unit, chan_id, callback,
                                                user_data);
        XFLOW_MACSEC_UNLOCK(unit);
    }
    return rv;
}

int bcm_common_xflow_macsec_event_register(
    int unit, bcm_xflow_macsec_event_cb cb,
    void *user_data)
{
    int rv = BCM_E_UNAVAIL;
    UNIT_VALID(unit);

    if (soc_feature(unit, soc_feature_xflow_macsec)) {
        XFLOW_MACSEC_LOCK(unit);
        rv = xflow_macsec_event_register(unit, (xflow_macsec_event_cb)cb,
                                         user_data);
        XFLOW_MACSEC_UNLOCK(unit);
    }
    return rv;
}

int bcm_common_xflow_macsec_event_deregister(
    int unit, bcm_xflow_macsec_event_cb cb)
{
    int rv = BCM_E_UNAVAIL;
    UNIT_VALID(unit);

    if (soc_feature(unit, soc_feature_xflow_macsec)) {
        XFLOW_MACSEC_LOCK(unit);
        rv = xflow_macsec_event_deregister(unit, (xflow_macsec_event_cb)cb);
        XFLOW_MACSEC_UNLOCK(unit);
    }
    return rv;
}

int
bcm_common_xflow_macsec_mac_addr_control_set(
    int unit, 
    uint32 flags, 
    bcm_xflow_macsec_instance_id_t instance_id, 
    bcm_xflow_macsec_mac_addr_control_t control_type, 
    bcm_xflow_macsec_mac_addr_info_t *control_info)
{
    int rv = BCM_E_UNAVAIL;
    UNIT_VALID(unit);

    if (!control_info ||
        (control_type != bcmXflowMacsecStationDstMac)) {
        return BCM_E_PARAM;
    }
    if (soc_feature(unit, soc_feature_xflow_macsec)) {
        XFLOW_MACSEC_LOCK(unit);
        rv = xflow_macsec_station_mac_addr_set(unit, control_info->mac_addr);
        XFLOW_MACSEC_UNLOCK(unit);
    }
    return rv;
}

int
bcm_common_xflow_macsec_mac_addr_control_get(
    int unit, 
    uint32 flags, 
    bcm_xflow_macsec_instance_id_t instance_id, 
    bcm_xflow_macsec_mac_addr_control_t control_type, 
    bcm_xflow_macsec_mac_addr_info_t *control_info)
{
    int rv = BCM_E_UNAVAIL;
    UNIT_VALID(unit);

    if (!control_info ||
        (control_type != bcmXflowMacsecStationDstMac)) {
        return BCM_E_PARAM;
    }
    if (soc_feature(unit, soc_feature_xflow_macsec)) {
        XFLOW_MACSEC_LOCK(unit);
        rv = xflow_macsec_station_mac_addr_get(unit, control_info->mac_addr);
        XFLOW_MACSEC_UNLOCK(unit);
    }
    return rv;
}


#else
int _xflow_macsec_cmn_not_empty;
#endif /* INCLUDE_XFLOW_MACSEC */
