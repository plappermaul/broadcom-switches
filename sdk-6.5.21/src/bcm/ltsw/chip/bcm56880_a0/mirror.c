/*
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated.
 * Edits to this file will be lost when it is regenerated.
 *
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

#include <bcm/mirror.h>
#include <bcm_int/ltsw/cosq.h>

#include <bcm_int/ltsw/mbcm/mirror.h>
#include <bcm_int/ltsw/xfs/mirror.h>
#include <bcm_int/ltsw/mirror.h>
#include <bcm_int/ltsw/lt_intf.h>
#include <bcm_int/ltsw/feature.h>

#include <shr/shr_debug.h>
#include <bcmltd/chip/bcmltd_str.h>

/******************************************************************************
 * Local definitions
 */

#define BSL_LOG_MODULE BSL_LS_BCM_MIRROR

/*! Mirror container size. */
#define MIRROR_CONTAINER_SIZE 8

/*! Mirror session array size. */
#define MIRROR_SESSION_ARRAY_SIZE 8

/*! Mirror instance size. */
#define MIRROR_INSTANCE_SIZE 16

/*! Mirror-on-Drop destination size. */
#define MIRROR_MOD_DEST_SIZE 1

/******************************************************************************
 * Private functions
 */


/*!
 * \brief Get sample profile num.
 *
 * \param [in] unit Unit number.
 * \param [out] num Sample profile num.
 *
 * \retval SHR_E_NONE on success and error code otherwise.
 */
static int
mirror_sample_profile_num_get(int unit, int *num)
{
    uint64_t min = 0;
    uint64_t max = 0;

    SHR_FUNC_ENTER(unit);

    SHR_NULL_CHECK(num, SHR_E_PARAM);

    SHR_IF_ERR_VERBOSE_EXIT
        (bcmi_lt_field_value_range_get(unit,
                                       MIRROR_SFLOWs,
                                       BASE_INDEXs,
                                       &min,
                                       &max));

     *num = max + 1;

exit:
    SHR_FUNC_EXIT();
}


static int
bcm56880_a0_ltsw_mirror_ctrl_info_init(
    int unit,
    bcmint_mirror_ctrl_info_t *mirror_ctrl)
{
    int alloc_size = 0, num = 0;
    void *ptr = NULL;
    int rv = SHR_E_NONE;

    /*
    * TD4 Mirror H/W resources introduction:
    * 1. Supports 8 containers.
    * 2. Container source type can be port, svp, fp action, sFlow, etc.
    * 3. Each container provides an index.
    * 4. Supports 8 session tables, each with 16 entries.
      5. Session table id is 1-1 mapping with container id.
    * 6. The index in container 'n' is used as index to select
    *    one of 16 entries in session table 'n'.
    * 7. Each session entry provides Mirror destination port, encap index, etc.
    * 8. Entry 0 in each session table is reserved as invalid(H/W limitation).
    *
    * TD4 Mirror S/W resources introduction:
    * 1. The exclusive model indicates Mirror session is exclusive to Mirror
    *    container. In this model, the Mirror session is divided into eight
    *    different Mirror session arrays, and each Mirror session array can be
    *    (and only be) used by one Mirror container. The Mirror container is
    *    always one-to-one mapping with Mirror session array. Each Mirror
    *    session array has 16 entries, which is called Mirror instance.
    * 2. The total number of session entries is 128 (8*16).
    * 3. Instance number zero will be reserved as invalid.
    * 4. Support Mirror Destination nubmer is 120.
    */
    mirror_ctrl->container_num = MIRROR_CONTAINER_SIZE;
    mirror_ctrl->session_attrs.model = bcmintMirrorSessionModelExclusive;
    mirror_ctrl->session_attrs.session_array_num = MIRROR_SESSION_ARRAY_SIZE;
    mirror_ctrl->session_attrs.instance_num = MIRROR_INSTANCE_SIZE;
    mirror_ctrl->session_attrs.session_num =
        mirror_ctrl->session_attrs.session_array_num *
        mirror_ctrl->session_attrs.instance_num;
    mirror_ctrl->session_attrs.rsvd_instance_zero = true;
    mirror_ctrl->session_attrs.mod_dest_shared_encap = false;
    mirror_ctrl->dest_num = mirror_ctrl->session_attrs.session_array_num *
        (mirror_ctrl->session_attrs.instance_num - 1);

    alloc_size = mirror_ctrl->container_num * sizeof(uint32_t);
    ptr = sal_alloc(alloc_size, "ltswMcUserFlags");
    if (ptr == NULL) {
        return SHR_E_MEMORY;
    }
    sal_memset(ptr, 0, alloc_size);
    mirror_ctrl->mc_st_flags = ptr;

    /*
    * According to TD4 DNA, list all supported source types for each
    * Mirror container:
    * 0--egress port, sampler, ingress vp, ingress port
    * 1--egress port, sampler, ingress vp, ingress port
    * 2--egress port, sampler, ingress vp, ingress port
    * 3--egress port, sampler, ingress vp, ingress port
    * 4--egress port, sampler, fp ingress
    * 5--egress port, sampler, fp ingress
    * 6--egress port, sampler, fp ingress
    * 7--egress port, sampler,
    */
    mirror_ctrl->mc_st_flags[0] = BCMINT_MIRROR_ST_F_PORT_EGR |
                                  BCMINT_MIRROR_ST_F_SAMPLE_INGRESS |
                                  BCMINT_MIRROR_ST_F_SAMPLE_FLEX |
                                  BCMINT_MIRROR_ST_F_VP_ING |
                                  BCMINT_MIRROR_ST_F_PORT_ING |
                                  BCMINT_MIRROR_ST_F_TRACE_EVENT |
                                  BCMINT_MIRROR_ST_F_DROP_EVENT;
    mirror_ctrl->mc_st_flags[1] = BCMINT_MIRROR_ST_F_PORT_EGR |
                                  BCMINT_MIRROR_ST_F_SAMPLE_INGRESS |
                                  BCMINT_MIRROR_ST_F_SAMPLE_FLEX |
                                  BCMINT_MIRROR_ST_F_VP_ING |
                                  BCMINT_MIRROR_ST_F_PORT_ING |
                                  BCMINT_MIRROR_ST_F_TRACE_EVENT |
                                  BCMINT_MIRROR_ST_F_DROP_EVENT;
    mirror_ctrl->mc_st_flags[2] = BCMINT_MIRROR_ST_F_PORT_EGR |
                                  BCMINT_MIRROR_ST_F_SAMPLE_INGRESS |
                                  BCMINT_MIRROR_ST_F_SAMPLE_FLEX |
                                  BCMINT_MIRROR_ST_F_PORT_ING |
                                  BCMINT_MIRROR_ST_F_FP_ING |
                                  BCMINT_MIRROR_ST_F_FP_EGR |
                                  BCMINT_MIRROR_ST_F_TRACE_EVENT |
                                  BCMINT_MIRROR_ST_F_DROP_EVENT;
    mirror_ctrl->mc_st_flags[3] = BCMINT_MIRROR_ST_F_PORT_EGR |
                                  BCMINT_MIRROR_ST_F_SAMPLE_INGRESS |
                                  BCMINT_MIRROR_ST_F_SAMPLE_FLEX |
                                  BCMINT_MIRROR_ST_F_PORT_ING |
                                  BCMINT_MIRROR_ST_F_FP_ING |
                                  BCMINT_MIRROR_ST_F_FP_EGR |
                                  BCMINT_MIRROR_ST_F_TRACE_EVENT |
                                  BCMINT_MIRROR_ST_F_DROP_EVENT;
    mirror_ctrl->mc_st_flags[4] = BCMINT_MIRROR_ST_F_PORT_EGR |
                                  BCMINT_MIRROR_ST_F_SAMPLE_INGRESS |
                                  BCMINT_MIRROR_ST_F_SAMPLE_FLEX |
                                  BCMINT_MIRROR_ST_F_FP_ING |
                                  BCMINT_MIRROR_ST_F_FP_EGR |
                                  BCMINT_MIRROR_ST_F_TRACE_EVENT |
                                  BCMINT_MIRROR_ST_F_DROP_EVENT;
    mirror_ctrl->mc_st_flags[5] = BCMINT_MIRROR_ST_F_PORT_EGR |
                                  BCMINT_MIRROR_ST_F_SAMPLE_INGRESS |
                                  BCMINT_MIRROR_ST_F_SAMPLE_FLEX |
                                  BCMINT_MIRROR_ST_F_FP_ING |
                                  BCMINT_MIRROR_ST_F_FP_EGR |
                                  BCMINT_MIRROR_ST_F_TRACE_EVENT |
                                  BCMINT_MIRROR_ST_F_DROP_EVENT;
    mirror_ctrl->mc_st_flags[6] = BCMINT_MIRROR_ST_F_PORT_EGR |
                                  BCMINT_MIRROR_ST_F_SAMPLE_INGRESS |
                                  BCMINT_MIRROR_ST_F_SAMPLE_FLEX |
                                  BCMINT_MIRROR_ST_F_TRACE_EVENT |
                                  BCMINT_MIRROR_ST_F_DROP_EVENT |
                                  BCMINT_MIRROR_ST_F_FIELD_DEST;
    mirror_ctrl->mc_st_flags[7] = BCMINT_MIRROR_ST_F_PORT_EGR |
                                  BCMINT_MIRROR_ST_F_SAMPLE_INGRESS |
                                  BCMINT_MIRROR_ST_F_SAMPLE_FLEX |
                                  BCMINT_MIRROR_ST_F_TRACE_EVENT |
                                  BCMINT_MIRROR_ST_F_DROP_EVENT;

    mirror_ctrl->mod_dest_num = MIRROR_MOD_DEST_SIZE;

    /* Initialize sample profile num. */
    rv = mirror_sample_profile_num_get(unit, &num);
    if (SHR_FAILURE(rv)) {
        return rv;
    }
    mirror_ctrl->sample_prf_info.prf_num = num;
    /* Profile id 0 is reserved to indicate no sample. */
    mirror_ctrl->sample_prf_info.rsvd_prf_id_zero = true;

    return SHR_E_NONE;
}

static int
bcm56880_a0_ltsw_mirror_init(int unit)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (xfs_ltsw_mirror_init(unit));

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56880_a0_ltsw_mirror_deinit(int unit)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (xfs_ltsw_mirror_deinit(unit));

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56880_a0_ltsw_mirror_encap_add(
    int unit,
    bool egr,
    bcm_mirror_destination_t *mirror_dest,
    int *index)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (xfs_ltsw_mirror_encap_add(unit, egr, mirror_dest, index));

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56880_a0_ltsw_mirror_encap_delete(
    int unit,
    bcm_mirror_destination_t *mirror_dest,
    int index)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (xfs_ltsw_mirror_encap_delete(unit, mirror_dest, index));

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56880_a0_ltsw_mirror_encap_get(
    int unit,
    int index,
    bcm_mirror_destination_t *mirror_dest)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (xfs_ltsw_mirror_encap_get(unit, index, mirror_dest));

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56880_a0_ltsw_mirror_session_add(
    int unit,
    int session_id,
    bcmint_mirror_session_entry_t *session)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (xfs_ltsw_mirror_session_add(unit, session_id, session));

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56880_a0_ltsw_mirror_session_delete(
    int unit,
    int session_id,
    bool egr,
    bool cosq_mod)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (xfs_ltsw_mirror_session_delete(unit, session_id, egr, cosq_mod));

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56880_a0_ltsw_mirror_session_get(
    int unit,
    int session_id,
    bcmint_mirror_session_entry_t *session)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (xfs_ltsw_mirror_session_get(unit, session_id, session));

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56880_a0_ltsw_mirror_session_stat_get(
    int unit,
    int session_id,
    bool sync,
    uint64_t *value)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (xfs_ltsw_mirror_session_stat_get(unit, session_id, sync, value));

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56880_a0_ltsw_mirror_session_stat_set(
    int unit,
    int session_id,
    uint64_t value)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (xfs_ltsw_mirror_session_stat_set(unit, session_id, value));

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56880_a0_ltsw_mirror_enable_get(
    int unit,
    bcmint_mirror_source_t *source,
    int *enable,
    int *instance_id)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (xfs_ltsw_mirror_enable_get(unit, source, enable, instance_id));

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56880_a0_ltsw_mirror_enable_set(
    int unit,
    bcmint_mirror_source_t *source,
    int mc_bmp,
    int enable,
    int *instance_id)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (xfs_ltsw_mirror_enable_set(unit, source, mc_bmp, enable, instance_id));

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56880_a0_ltsw_mirror_misc_init(int unit)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (xfs_ltsw_mirror_misc_init(unit));

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56880_a0_ltsw_mirror_misc_deinit(int unit)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (xfs_ltsw_mirror_misc_deinit(unit));

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56880_a0_ltsw_mirror_sample_profile_init(int unit)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (xfs_ltsw_mirror_sample_profile_init(unit));

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56880_a0_ltsw_mirror_sample_profile_deinit(int unit)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (xfs_ltsw_mirror_sample_profile_deinit(unit));

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56880_a0_ltsw_mirror_egress_sampler_rate_range_max(
    int unit,
    int *range_max)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (xfs_ltsw_mirror_egress_sampler_rate_range_max(unit, range_max));

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56880_a0_ltsw_mirror_sampler_seed_set(
    int unit,
    bcmi_ltsw_mirror_sampler_seed_type_t type,
    int value)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (xfs_ltsw_mirror_sampler_seed_set(unit, type, value));

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56880_a0_ltsw_mirror_sampler_seed_get(
    int unit,
    bcmi_ltsw_mirror_sampler_seed_type_t type,
    int *value)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (xfs_ltsw_mirror_sampler_seed_get(unit, type, value));

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56880_a0_ltsw_mirror_mc_cos_set(
    int unit,
    int mc_cos)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (xfs_ltsw_mirror_mc_cos_set(unit, mc_cos));

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56880_a0_ltsw_mirror_mc_cos_get(
    int unit,
    int *mc_cos)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (xfs_ltsw_mirror_mc_cos_get(unit, mc_cos));

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56880_a0_ltsw_mirror_cpu_cos_set(
    int unit,
    int cpu_cos)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (xfs_ltsw_mirror_cpu_cos_set(unit, cpu_cos));

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56880_a0_ltsw_mirror_cpu_cos_get(
    int unit,
    int *cpu_cos)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (xfs_ltsw_mirror_cpu_cos_get(unit, cpu_cos));

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56880_a0_ltsw_mirror_destination_validate(
    int unit,
    bcm_mirror_destination_t *mirror_dest)
{
    bcmi_ltsw_cosq_port_map_info_t info;
    bcm_port_t local_port;
    int max_cosq;
    SHR_FUNC_ENTER(unit);

    /* Do not support. */
    if ((mirror_dest->flags & BCM_MIRROR_DEST_ID_SHARE) ||
        (mirror_dest->flags & BCM_MIRROR_DEST_MTP_ADD) ||
        (mirror_dest->flags & BCM_MIRROR_DEST_MTP_REPLACE) ||
        (mirror_dest->flags & BCM_MIRROR_DEST_MTP_DELETE) ||
        (mirror_dest->flags & BCM_MIRROR_DEST_INT_PRI_SET) ||
        (mirror_dest->flags & BCM_MIRROR_DEST_TUNNEL_TRILL) ||
        (mirror_dest->flags & BCM_MIRROR_DEST_TUNNEL_WITH_SPAN_ID) ||
        (mirror_dest->flags & BCM_MIRROR_DEST_TUNNEL_RSPAN) ||
        (mirror_dest->flags & BCM_MIRROR_DEST_TUNNEL_ETAG) ||
        (mirror_dest->flags2 & BCM_MIRROR_DEST_FLAGS2_IFA) ||
        (mirror_dest->flags2 & BCM_MIRROR_DEST_FLAGS2_INT_PROBE) ||
        (mirror_dest->flags2 & BCM_MIRROR_DEST_FLAGS2_TRUE_EGRESS)) {
        SHR_ERR_EXIT(SHR_E_UNAVAIL);
    }

    if (mirror_dest->flags & BCM_MIRROR_DEST_PAYLOAD_UNTAGGED) {
        SHR_ERR_EXIT(SHR_E_UNAVAIL);
    }

    /* TD4 does not support mirroring to Trunk group. */
    if (BCM_GPORT_IS_TRUNK(mirror_dest->gport)) {
        SHR_ERR_EXIT(SHR_E_UNAVAIL);
    }

    if ((mirror_dest->flags & BCM_MIRROR_DEST_TUNNEL_L2) &&
        (mirror_dest->flags2 & BCM_MIRROR_DEST_FLAGS2_LOOPBACK_ENABLE)) {
        SHR_ERR_EXIT(SHR_E_UNAVAIL);
    }

    if ((mirror_dest->truncate != BCM_MIRROR_PAYLOAD_TRUNCATE) &&
        (mirror_dest->truncate != BCM_MIRROR_PAYLOAD_DO_NOT_TRUNCATE)) {
        SHR_ERR_EXIT(SHR_E_UNAVAIL);
    }

    /* Get local port first. */
    SHR_IF_ERR_VERBOSE_EXIT
        (bcmi_ltsw_mirror_local_port_get(unit,
                                         mirror_dest->gport,
                                         NULL,
                                         &local_port));

    /* Get UC/MC queue number. */
    SHR_IF_ERR_VERBOSE_EXIT
        (bcmi_ltsw_cosq_port_map_info_get(unit, local_port, &info));

    max_cosq = info.num_mc_q;
    /* It can be uc queue for Mirror-on-drop. */
    if (mirror_dest->flags2 & BCM_MIRROR_DEST_FLAGS2_MOD_TUNNEL_PSAMP) {
        max_cosq = info.num_mc_q + info.num_uc_q;
    }

    /* The cosq exceeds the maximum queue number. */
    if (mirror_dest->cosq >= max_cosq ||
        mirror_dest->cosq < 0) {
        SHR_ERR_EXIT(SHR_E_PARAM);
    }

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56880_a0_ltsw_mirror_sample_profile_set(
    int unit,
    bcmint_mirror_sample_type_t type,
    int profile_id,
    bcm_mirror_sample_profile_t *profile)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (xfs_ltsw_mirror_sample_profile_set(unit, type, profile_id, profile));

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56880_a0_ltsw_mirror_sample_profile_get(
    int unit,
    bcmint_mirror_sample_type_t type,
    int profile_id,
    bcm_mirror_sample_profile_t *profile)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (xfs_ltsw_mirror_sample_profile_get(unit, type, profile_id, profile));

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56880_a0_ltsw_mirror_sample_stat_attach(
    int unit,
    bcmint_mirror_sample_type_t type,
    uint32_t stat_counter_id)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (xfs_ltsw_mirror_sample_stat_attach(unit, type, stat_counter_id));

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56880_a0_ltsw_mirror_sample_stat_detach(
    int unit,
    bcmint_mirror_sample_type_t type,
    uint32_t stat_counter_id)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (xfs_ltsw_mirror_sample_stat_detach(unit, type, stat_counter_id));

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56880_a0_ltsw_mirror_global_truncate_set(
    int unit,
    bcm_mirror_global_truncate_t *global_truncate)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (xfs_ltsw_mirror_global_truncate_set(unit, global_truncate));

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56880_a0_ltsw_mirror_global_truncate_get(
    int unit,
    bcm_mirror_global_truncate_t *global_truncate)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (xfs_ltsw_mirror_global_truncate_get(unit, global_truncate));

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56880_a0_ltsw_mirror_truncate_length_profile_create(
    int unit,
    uint32_t options,
    int *profile_id,
    bcm_mirror_truncate_length_profile_t *profile)
{
    SHR_FUNC_ENTER(unit);

    SHR_NULL_CHECK(profile, SHR_E_PARAM);

    /* Only support bcmMirrorTruncateAdjustment. */
    if (profile->mode != bcmMirrorTruncateAdjustment) {
        SHR_ERR_EXIT(SHR_E_UNAVAIL);
    }

    SHR_IF_ERR_VERBOSE_EXIT
        (xfs_ltsw_mirror_truncate_length_profile_create(unit, options, profile_id, profile));

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56880_a0_ltsw_mirror_truncate_length_profile_destroy(
    int unit,
    int profile_id)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (xfs_ltsw_mirror_truncate_length_profile_destroy(unit, profile_id));

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56880_a0_ltsw_mirror_truncate_length_profile_get(
    int unit,
    int profile_id,
    bcm_mirror_truncate_length_profile_t *profile)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (xfs_ltsw_mirror_truncate_length_profile_get(unit, profile_id, profile));

exit:
    SHR_FUNC_EXIT();
}

/*!
 * \brief Mirror driver function variable for bcm56880_a0 device.
 */
static mbcm_ltsw_mirror_drv_t bcm56880_a0_ltsw_mirror_drv = {
    .mirror_ctrl_info_init = bcm56880_a0_ltsw_mirror_ctrl_info_init,
    .mirror_init = bcm56880_a0_ltsw_mirror_init,
    .mirror_deinit = bcm56880_a0_ltsw_mirror_deinit,
    .mirror_encap_add = bcm56880_a0_ltsw_mirror_encap_add,
    .mirror_encap_delete = bcm56880_a0_ltsw_mirror_encap_delete,
    .mirror_encap_get = bcm56880_a0_ltsw_mirror_encap_get,
    .mirror_session_add = bcm56880_a0_ltsw_mirror_session_add,
    .mirror_session_delete = bcm56880_a0_ltsw_mirror_session_delete,
    .mirror_session_get = bcm56880_a0_ltsw_mirror_session_get,
    .mirror_session_stat_get = bcm56880_a0_ltsw_mirror_session_stat_get,
    .mirror_session_stat_set = bcm56880_a0_ltsw_mirror_session_stat_set,
    .mirror_enable_get = bcm56880_a0_ltsw_mirror_enable_get,
    .mirror_enable_set = bcm56880_a0_ltsw_mirror_enable_set,
    .mirror_misc_init = bcm56880_a0_ltsw_mirror_misc_init,
    .mirror_misc_deinit = bcm56880_a0_ltsw_mirror_misc_deinit,
    .mirror_sample_profile_init = bcm56880_a0_ltsw_mirror_sample_profile_init,
    .mirror_sample_profile_deinit = bcm56880_a0_ltsw_mirror_sample_profile_deinit,
    .mirror_egress_sampler_rate_range_max = bcm56880_a0_ltsw_mirror_egress_sampler_rate_range_max,
    .mirror_sampler_seed_set = bcm56880_a0_ltsw_mirror_sampler_seed_set,
    .mirror_sampler_seed_get = bcm56880_a0_ltsw_mirror_sampler_seed_get,
    .mirror_mc_cos_set = bcm56880_a0_ltsw_mirror_mc_cos_set,
    .mirror_mc_cos_get = bcm56880_a0_ltsw_mirror_mc_cos_get,
    .mirror_cpu_cos_set = bcm56880_a0_ltsw_mirror_cpu_cos_set,
    .mirror_cpu_cos_get = bcm56880_a0_ltsw_mirror_cpu_cos_get,
    .mirror_destination_validate = bcm56880_a0_ltsw_mirror_destination_validate,
    .mirror_sample_profile_set = bcm56880_a0_ltsw_mirror_sample_profile_set,
    .mirror_sample_profile_get = bcm56880_a0_ltsw_mirror_sample_profile_get,
    .mirror_sample_stat_attach = bcm56880_a0_ltsw_mirror_sample_stat_attach,
    .mirror_sample_stat_detach = bcm56880_a0_ltsw_mirror_sample_stat_detach,
    .mirror_global_truncate_set = bcm56880_a0_ltsw_mirror_global_truncate_set,
    .mirror_global_truncate_get = bcm56880_a0_ltsw_mirror_global_truncate_get,
    .mirror_truncate_length_profile_create = bcm56880_a0_ltsw_mirror_truncate_length_profile_create,
    .mirror_truncate_length_profile_destroy = bcm56880_a0_ltsw_mirror_truncate_length_profile_destroy,
    .mirror_truncate_length_profile_get = bcm56880_a0_ltsw_mirror_truncate_length_profile_get,
};

/******************************************************************************
 * Public functions
 */

int
bcm56880_a0_ltsw_mirror_drv_attach(int unit)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_mirror_drv_set(unit, &bcm56880_a0_ltsw_mirror_drv));

exit:
    SHR_FUNC_EXIT();
}

