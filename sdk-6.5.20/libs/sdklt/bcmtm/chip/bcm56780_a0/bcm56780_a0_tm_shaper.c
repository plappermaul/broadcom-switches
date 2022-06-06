/*! \file bcm56780_a0_tm_shaper.c
 *
 * Shaper related api and function for bcm56780_a0.
 */
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

#include <shr/shr_debug.h>
#include <bcmtm/bcmtm_drv.h>
#include <bcmtm/bcmtm_utils.h>
#include <bcmtm/bcmtm_types.h>
#include <bcmltd/bcmltd_types.h>
#include <bcmdrd/bcmdrd_types.h>
#include <bcmdrd/chip/bcm56780_a0_enum.h>
#include <bcmtm/bcmtm_pt_internal.h>
#include <bcmtm/bcmtm_shaper_encoding_internal.h>
#include <bcmcfg/bcmcfg_lt.h>
#include <bcmtm/chip/bcm56780_a0_tm.h>
#include "bcm56780_a0_tm_scheduler_shaper.h"

/*******************************************************************************
 * Local definitions
 */
#define BSL_LOG_MODULE  BSL_LS_BCMTM_CHIP
/*******************************************************************************
 * Public functions
 */
/*!
 * \brief Shaper port physical table configure.
 *
 * \param [in] unit  Unit number.
 * \param [in] ltid Logical table ID.
 * \param [in] lport Logical port ID.
 * \param [in] shaper_cfg Shaper configuration.
 * \param [in] enable Enables metering for the ports.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Error code for corresponding failures.
 */
static int
bcm56780_a0_tm_shaper_port_set(int unit,
                               bcmtm_lport_t lport,
                               bcmltd_sid_t ltid,
                               bcmtm_shaper_cfg_t *shaper_cfg,
                               int enable)
{
    bcmdrd_sid_t sid;
    bcmdrd_fid_t fid;
    uint32_t ltmbuf_arr[2] = {0}, ltmbuf = 0;
    uint32_t fval = 0;
    int mport, pipe;
    bcmtm_pt_info_t pt_info = {0};

    SHR_FUNC_ENTER(unit);

    /* Indexes */
    SHR_IF_ERR_EXIT(bcmtm_lport_mmu_local_port_get(unit, lport, &mport));
    SHR_IF_ERR_EXIT(bcmtm_lport_pipe_get(unit, lport, &pipe));

    BCMTM_PT_DYN_INFO(pt_info, mport, pipe);

    /* MMU_MTRO_EGRMETERINGCONFIG */
    sid = MMU_MTRO_EGRMETERINGCONFIGm;

    /* Shaping mode (packet mode/ byte mode) */
    fid = PACKET_SHAPINGf;
    fval = shaper_cfg->shaping_mode;
    SHR_IF_ERR_EXIT
        (bcmtm_field_set(unit, sid, fid, ltmbuf_arr, &fval));

    /* Refresh count for metering bucket */
    fid = REFRESHf;
    fval = shaper_cfg->max_bucket_encode.refresh_rate;
    SHR_IF_ERR_EXIT
        (bcmtm_field_set(unit, sid, fid, ltmbuf_arr, &fval));

    /* Meter granularity */
    fid = METER_GRANf;
    fval = shaper_cfg->max_bucket_encode.granularity;
    SHR_IF_ERR_EXIT
        (bcmtm_field_set(unit, sid, fid, ltmbuf_arr, &fval));

    /* Threshold for bucket size. */
    fid = THD_SELf;
    fval = shaper_cfg->max_bucket_encode.bucket_sz;
    SHR_IF_ERR_EXIT
        (bcmtm_field_set(unit, sid, fid, ltmbuf_arr, &fval));

    /* Write to hardware */
    SHR_IF_ERR_EXIT
        (bcmtm_pt_indexed_write(unit, sid, ltid, &pt_info, ltmbuf_arr));

    /* EPOST_EGR_SHAPING_CONTROLm for adjusting bytes. */
    ltmbuf = 0;
    sid = EPOST_EGR_SHAPING_CONTROLm;
    fid = ADJUST_BYTESf;

    BCMTM_PT_DYN_INFO(pt_info, lport, 0);
    fval = shaper_cfg->byte_count_inter_frame_gap;

    if (!shaper_cfg->ifg_encap) {
        /* get 2's compliment for the same. */
        fval = (~fval) + 1;
    }
    SHR_IF_ERR_EXIT
        (bcmtm_field_set(unit, sid, fid, &ltmbuf, &fval));
    SHR_IF_ERR_EXIT
        (bcmtm_pt_indexed_write(unit, sid, ltid, &pt_info, &ltmbuf));

exit:
    SHR_FUNC_EXIT();
}

/*!
 * \brief Shaper node physical table configure.
 *
 * \param [in] unit  Unit number.
 * \param [in] lport Logical port ID.
 * \param [in] sched_node_id Scheduler node ID.
 * \param [in] ltid Logical table ID.
 * \param [in] shaper_cfg Shaper configuration.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Error code for corresponding failures.
 */
static int
bcm56780_a0_tm_shaper_node_set(int unit,
                               bcmtm_lport_t lport,
                               uint32_t sched_level,
                               uint32_t sched_node_id,
                               bcmltd_sid_t ltid,
                               bcmtm_shaper_cfg_t *shaper_cfg)
{
    bcmdrd_sid_t sid;
    bcmdrd_fid_t fid;
    uint32_t ltmbuf[3] = {0};
    uint32_t fval = 0;
    int mport, pipe, index, l0_node_id;
    bcmtm_pt_info_t pt_info = {0};

    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_EXIT
        (bcmtm_lport_mmu_local_port_get(unit, lport, &mport));

    SHR_IF_ERR_EXIT
        (bcmtm_lport_pipe_get(unit, lport, &pipe));

    SHR_IF_ERR_EXIT
        (bcm56780_a0_tm_sched_node_l0_map_get(unit, shaper_cfg->profile_id,
                                              sched_node_id, &l0_node_id));

    if (bcmtm_lport_is_mgmt(unit, lport)) {
        index = TD4_X9_TM_MGMT_Q_OFFSET + l0_node_id;
    } else if (bcmtm_lport_is_lb(unit, lport)) {
        index = TD4_X9_TM_LB_Q_OFFSET + l0_node_id;
    } else {
        index = mport * TD4_X9_TM_NUM_Q + l0_node_id;
    }
    BCMTM_PT_DYN_INFO(pt_info, index, pipe);

    /* MMU_MTRO_L0 */
    sid = MMU_MTRO_L0_Am;

    /* Shaping mode (packet mode/ byte mode) */
    fid = PACKET_SHAPINGf;
    fval = shaper_cfg->shaping_mode;
    SHR_IF_ERR_EXIT
        (bcmtm_field_set(unit, sid, fid, ltmbuf, &fval));

    /* Refresh count for max metering bucket */
    fid = MAX_REFRESHf;
    fval = shaper_cfg->max_bucket_encode.refresh_rate;
    SHR_IF_ERR_EXIT
        (bcmtm_field_set(unit, sid, fid, ltmbuf, &fval));

    /* Max meter granularity */
    fid = MAX_METER_GRANf;
    fval = shaper_cfg->max_bucket_encode.granularity;
    SHR_IF_ERR_EXIT
        (bcmtm_field_set(unit, sid, fid, ltmbuf, &fval));

    /* Max threshold for bucket size. */
    fid = MAX_THD_SELf;
    fval = shaper_cfg->max_bucket_encode.bucket_sz;
    SHR_IF_ERR_EXIT
        (bcmtm_field_set(unit, sid, fid, ltmbuf, &fval));

    /* Refresh count for min metering bucket */
    fid = MIN_REFRESHf;
    fval = shaper_cfg->min_bucket_encode.refresh_rate;
    SHR_IF_ERR_EXIT
        (bcmtm_field_set(unit, sid, fid, ltmbuf, &fval));

    /* Min meter granularity */
    fid = MIN_METER_GRANf;
    fval = shaper_cfg->min_bucket_encode.granularity;
    SHR_IF_ERR_EXIT
        (bcmtm_field_set(unit, sid, fid, ltmbuf, &fval));

    /* Min threshold for bucket size. */
    fid = MIN_THD_SELf;
    fval = shaper_cfg->min_bucket_encode.bucket_sz;
    SHR_IF_ERR_EXIT
        (bcmtm_field_set(unit, sid, fid, ltmbuf, &fval));

    /* Write to hardware */
    SHR_IF_ERR_EXIT
        (bcmtm_pt_indexed_write(unit, sid, ltid, &pt_info, ltmbuf));
exit:
    SHR_FUNC_EXIT();
}

/*!
 * \brief Shaper cpu node physical table configure.
 *
 * \param [in] unit  Unit number.
 * \param [in] sched_node_id Scheduler node ID.
 * \param [in] sched_level Scheduler level.
 * \param [in] ltid Logical table ID.
 * \param [in] shaper_cfg Shaper configuration.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Error code for corresponding failures.
 */
static int
bcm56780_a0_tm_shaper_cpu_node_set(int unit,
                                   uint32_t sched_node_id,
                                   uint32_t sched_level,
                                   bcmltd_sid_t ltid,
                                   bcmtm_shaper_cfg_t *shaper_cfg)
{
    bcmdrd_sid_t sid;
    bcmdrd_fid_t fid;
    uint32_t ltmbuf[3] = {0};
    uint32_t fval = 0;
    int index;
    bcmtm_pt_info_t pt_info = {0};

    SHR_FUNC_ENTER(unit);

    if ((sched_level == L0_SCHED_COS) && (sched_node_id >= TD4_X9_NUM_L0_NODE)) {
        SHR_ERR_EXIT(SHR_E_PARAM);
    }

    if (sched_level == L0_SCHED_COS) { /* L0 level*/
        sid = MMU_MTRO_L0_Am;
        index = TD4_X9_TM_CPU_Q_OFFSET + sched_node_id;
    } else {
        sid = MMU_MTRO_CPU_L1_Am;
        index = sched_node_id;
    }
    BCMTM_PT_DYN_INFO(pt_info, index , 0);

    /* Shaping mode (packet mode/ byte mode) */
    fid = PACKET_SHAPINGf;
    fval = shaper_cfg->shaping_mode;
    SHR_IF_ERR_EXIT
        (bcmtm_field_set(unit, sid, fid, ltmbuf, &fval));

    /* Refresh count for max metering bucket */
    fid = MAX_REFRESHf;
    fval = shaper_cfg->max_bucket_encode.refresh_rate;
    SHR_IF_ERR_EXIT
        (bcmtm_field_set(unit, sid, fid, ltmbuf, &fval));

    /* Max meter granularity */
    fid = MAX_METER_GRANf;
    fval = shaper_cfg->max_bucket_encode.granularity;
    SHR_IF_ERR_EXIT
        (bcmtm_field_set(unit, sid, fid, ltmbuf, &fval));
    /* Max threshold for bucket size. */
    fid = MAX_THD_SELf;
    fval = shaper_cfg->max_bucket_encode.bucket_sz;
    SHR_IF_ERR_EXIT
        (bcmtm_field_set(unit, sid, fid, ltmbuf, &fval));

    /* Refresh count for min metering bucket */
    fid = MIN_REFRESHf;
    fval = shaper_cfg->min_bucket_encode.refresh_rate;
    SHR_IF_ERR_EXIT
        (bcmtm_field_set(unit, sid, fid, ltmbuf, &fval));

    /* Min meter granularity */
    fid = MIN_METER_GRANf;
    fval = shaper_cfg->min_bucket_encode.granularity;
    SHR_IF_ERR_EXIT
        (bcmtm_field_set(unit, sid, fid, ltmbuf, &fval));

    /* Min threshold for bucket size. */
    fid = MIN_THD_SELf;
    fval = shaper_cfg->min_bucket_encode.bucket_sz;
    SHR_IF_ERR_EXIT
        (bcmtm_field_set(unit, sid, fid, ltmbuf, &fval));

    /* Write to hardware */
    SHR_IF_ERR_EXIT
        (bcmtm_pt_indexed_write(unit, sid, ltid, &pt_info, ltmbuf));
exit:
    SHR_FUNC_EXIT();
}

/*!
 * \brief Shaper configuration.
 *
 * \param [in]  unit   Unit number.
 *
 * \retval SHR_E_NONE  No error.
 */
static int
bcm56780_a0_tm_shaper_config_init(int unit)
{
    bcmdrd_sid_t sid;
    bcmltd_sid_t ltid = TM_SHAPER_CONFIGt;
    uint64_t field_value = 0;
    uint32_t fval = 0, ltmbuf = 0;
    bcmtm_pt_info_t pt_info = {0};
    int rv;

    SHR_FUNC_ENTER(unit);

    /*! Update the entries from config table. */
    rv = bcmcfg_field_get(unit, ltid,
                          TM_SHAPER_CONFIGt_ITU_MODEf,
                          &field_value);
    if (rv == SHR_E_UNAVAIL) {
        SHR_EXIT();
    }

    sid = MMU_MTRO_CONFIGr;
    BCMTM_PT_DYN_INFO(pt_info, 0, 0);
    /* ITU settings */
    fval = (uint32_t) field_value;
    SHR_IF_ERR_EXIT
        (bcmtm_field_set(unit, sid, ITU_MODE_SELf, &ltmbuf, &fval));
    /* Metering enable/disable */
    rv = bcmcfg_field_get(unit, ltid,
                          TM_SHAPER_CONFIGt_SHAPERf,
                          &field_value);
    fval = (uint32_t) field_value;
    SHR_IF_ERR_EXIT
        (bcmtm_field_set(unit, sid, REFRESH_DISABLEf, &ltmbuf, &fval));
    SHR_IF_ERR_EXIT
        (bcmtm_pt_indexed_write(unit, sid, ltid, &pt_info, &fval));

exit:
    SHR_FUNC_EXIT();
}

/*******************************************************************************
 * Public functions
 */
int
bcm56780_a0_tm_shaper_drv_get(int unit, void *shaper_drv)
{
    bcmtm_shaper_drv_t bcm56780_a0_shaper_drv = {
        .port_set = bcm56780_a0_tm_shaper_port_set,
        .node_set = bcm56780_a0_tm_shaper_node_set,
        .cpu_node_set = bcm56780_a0_tm_shaper_cpu_node_set,
        .rate_to_bucket = bcm56780_a0_tm_shaper_rate_to_bucket,
        .bucket_to_rate = bcm56780_a0_tm_shaper_bucket_to_rate
    };
    SHR_FUNC_ENTER(unit);

    SHR_NULL_CHECK(shaper_drv, SHR_E_INTERNAL);
    *((bcmtm_shaper_drv_t *)shaper_drv) = bcm56780_a0_shaper_drv;
exit:
    SHR_FUNC_EXIT();
}

int
bcm56780_a0_tm_shaper_chip_init(int unit, bool warm)
{

    SHR_FUNC_ENTER(unit);
    if (!warm) {
        SHR_IF_ERR_EXIT
            (bcm56780_a0_tm_shaper_config_init(unit));
    }
exit:
    SHR_FUNC_EXIT();
}
