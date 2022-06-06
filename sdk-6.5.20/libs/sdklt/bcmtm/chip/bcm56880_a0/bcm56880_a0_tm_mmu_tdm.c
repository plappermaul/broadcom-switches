/*! \file bcm56880_a0_tm_mmu_tdm.
 *
 * File containing flexport sequefor bcm56880_a0.
 *
 * The code in this file is shared with DV team. This file will only be updated
 * by porting DV code changes. If any extra change needs to be made to the
 * shared code, please also inform the DV team.
 *
 * DV file: $DV/trident4/dv/ngsdk_interface/flexport/mmu_tdm.c
 */
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

#include <bsl/bsl.h>
#include <shr/shr_debug.h>
#include <sal/sal_sleep.h>
#include <bcmdrd/chip/bcm56880_a0_enum.h>
#include <bcmtm/chip/bcm56880_a0_tm.h>
#include <bcmtm/bcmtm_utils_internal.h>
#include <bcmtm/bcmtm_utils.h>
#include <bcmtm/bcmtm_drv.h>
#include <bcmtm/bcmtm_pt_internal.h>
#include "bcm56880_a0_tm_idb_flexport.h"
#include "bcm56880_a0_tm_mmu_tdm.h"
#include "bcm56880_a0_tm_e2e_settings.h"

#define TD4_E2E_NUM_PFC_OPT_CFG  2

#define BSL_LOG_MODULE  BSL_LS_BCMTM_FLEXPORT

/*** START SDK API COMMON CODE ***/



/*! @fn void td4_mmu_get_speed_decode(int unit, int speed,
 *              int *speed_decode)
 *  @param unit Device number
 *  @param speed Actual speed value
 *  @param *speed_decode Decoded speed ID value
 *  @brief API to return the decoded speed value, used to program
 *         EBCTM registers
 */
int
td4_mmu_get_speed_decode(int unit, int speed, uint32_t *speed_decode)
{
    if (speed >= 400000) {
        *speed_decode = 7;
    } else if (speed >= 200000) {
        *speed_decode = 6;
    } else if (speed >= 100000) {
        *speed_decode = 5;
    } else if (speed >= 50000) {
        *speed_decode = 4;
    } else if (speed >= 40000) {
        *speed_decode = 3;
    } else if (speed >= 25000) {
        *speed_decode = 2;
    } else if (speed >= 1000) {
        *speed_decode = 1;
    } else {
        *speed_decode = 0;
         return SHR_E_FAIL;
    }
    return SHR_E_NONE;
}

static int
td4_ep_get_ep2mmu_credit_per_speed(int unit, int speed, uint32_t *credit)
{
    uint32_t encode;
    int core_clock_freq, dpp_clock_freq;
    bcmtm_drv_info_t *drv_info;

    SHR_FUNC_ENTER(unit);
    bcmtm_drv_info_get(unit, &drv_info);

    core_clock_freq = drv_info->frequency;
    dpp_clock_freq = drv_info->dpr_clock_frequency;

    SHR_IF_ERR_EXIT
        (td4_mmu_get_speed_decode(unit, speed, &encode));

    if ((core_clock_freq == 1350 && dpp_clock_freq == 1350) ||
        (core_clock_freq == 1500 && dpp_clock_freq == 1500)) {
        *credit = td4_edb_e2e_tbl[encode].mmu_credit_pfc_opt;
    } else {
        *credit = td4_edb_e2e_tbl[encode].mmu_credit;
    }

exit:
    SHR_FUNC_EXIT();
}

static int
td4_get_pipe_pm_from_phy_port(int pnum)
{
    return (((pnum - 1) & 0x1f) >> 3);
}

static int
td4_get_subp_from_phy_port(int pnum)
{
    return ((pnum - 1) & 0x7);
}

int
td4_tdm_global_to_local_phy_num(int phy_port, uint32_t *local_phy_num)
{
    *local_phy_num = 0x3f;

    if (0 <= phy_port && phy_port <= 32) {
        *local_phy_num = phy_port;
    } else if (33 <= phy_port && phy_port <= (TD4_PHYS_PORTS_PER_PIPE * TD4_PIPES_PER_DEV)) {
        *local_phy_num = ((phy_port - 1) % TD4_PHYS_PORTS_PER_PIPE);
    } else if (phy_port == TD4_PHY_PORT_MNG0 || phy_port == TD4_PHY_PORT_MNG1 ||
               phy_port == TD4_PHY_PORT_MNG2 || phy_port == TD4_PHY_PORT_MNG3) {
        *local_phy_num = 32;
    } else if (TD4_PHY_PORT_LPBK0 <= phy_port && phy_port <= TD4_PHY_PORT_LPBK3) {
        *local_phy_num = 33;
    } else {
        return SHR_E_PARAM;
    }

    return SHR_E_NONE;
}

static int
td4_tdm_set_cal_config(int unit,
                       int lport,
                       mmu_sched_inst_name_e sched_inst_name,
                       int is_port_down,
                       int init)
{
    int pm_num;
    int subp;
    int num_50g_slots;
    int is_special_slot;
    int lane, slot;
    int port_sched_table[TD4_TDM_LENGTH];
    int pm_mapping[TD4_PORTS_PER_PBLK] = {0,4,2,6,1,5,3,7};
    int this_port_number;
    int this_slot_valid;
    int this_port_speed;
    int pipe, phy_port;

    uint32_t ltmbuf[BCMTM_MAX_ENTRY_WSIZE] = {0};
    uint32_t fval;
    uint32_t fval_64[BCMTM_MAX_ENTRY_WSIZE] = {0};
    uint32_t this_port_cal_slot_bitmap[2];

    bcmdrd_sid_t reg;
    bcmtm_pt_info_t pt_dyn_info = {0};
    bcmdrd_fid_t fid ;
    bcmltd_sid_t ltid = -1; /* Requesting LT ID not available */
    bcmtm_port_map_info_t *port_map;

    SHR_FUNC_ENTER(unit);

    bcmtm_port_map_get(unit, &port_map);

    phy_port = init ? (BCMTM_MAX_ENTRY_WSIZE + 1) : port_map->lport_map[lport].pport;
    pipe = port_map->lport_map[lport].pipe;
    pm_num = td4_get_pipe_pm_from_phy_port(phy_port);
    subp = td4_get_subp_from_phy_port(phy_port);
    num_50g_slots =
        (port_map->lport_map[lport].max_speed + TDM_SPEED_40G) / TDM_SPEED_50G;

    is_special_slot =
        (port_map->lport_map[lport].mlocal_port >= TD4_FP_PORTS_PER_PIPE) ? 1 : 0;
    for (slot = 0; slot < TD4_TDM_LENGTH; slot++) {
        port_sched_table[slot] = 0;
    }
    if (!is_special_slot) {
        for (lane = subp; lane < subp + num_50g_slots; lane++) {
            slot = pm_mapping[lane] * TD4_PBLKS_PER_PIPE + pm_num;
            port_sched_table[slot] = 1;
        }
    } else {
        port_sched_table[TD4_TDM_LENGTH - 1] = 1;
    }

    this_port_number = (is_port_down == 1) ? 0 :
        ((is_special_slot == 1) ? TD4_TDM_SCHED_SPECIAL_SLOT :
        port_map->pport_map[phy_port].mport & 0x1f);

    this_slot_valid  = (is_port_down == 1) ? 0 : 1;

    if (is_port_down == 1) {
        this_port_speed = 0;
    } else if (is_special_slot == 1) {
        this_port_speed = 3; /* Speed 50G for Aux ports */
    } else {
        switch (port_map->lport_map[lport].max_speed) {
        case TDM_SPEED_400G:
            this_port_speed = 0;
            break;
        case TDM_SPEED_200G:
            this_port_speed = 1;
            break;
        case TDM_SPEED_100G:
            this_port_speed = 2;
            break;
        case TDM_SPEED_50G:
            this_port_speed = 3;
            break;
        default: /* Speed < 50G */
            this_port_speed = 3;
            break;
        }
    }

    SHR_BITCLR_RANGE(this_port_cal_slot_bitmap, 0, 64);
    if (is_port_down == 0) {
        for (slot = 0; slot < TD4_TDM_LENGTH; slot++) {
            if (port_sched_table[slot]==1) {
                SHR_BITSET(this_port_cal_slot_bitmap, slot);
            }
        }
    }

    /* Select the right reg based on sched instance. */
    switch (sched_inst_name) {
    case TDM_MMU_MAIN_SCHED:
        reg = MMU_PTSCH_CAL_CONFIGr;
        break;
    case TDM_MMU_EB_PORT_SCHED:
        reg = MMU_EBPTS_CAL_CONFIGr;
        break;
    case TDM_MMU_EB_PKT_SCHED:
        reg = MMU_EBPTS_PKSCH_CAL_CONFIGr;
        break;
    default:
        LOG_ERROR(BSL_LOG_MODULE,
                  (BSL_META_U(unit,
                              "scheduler instance name is not supported %d\n"),
                   sched_inst_name));
        return SHR_E_PARAM;
        break;
    }
    for (slot = 0; slot < TD4_TDM_LENGTH; slot++) {
        if (port_sched_table[slot] == 1) {
            BCMTM_PT_DYN_INFO(pt_dyn_info, slot, pipe);
            SHR_IF_ERR_EXIT
                (bcmtm_pt_indexed_read(unit, reg, ltid, &pt_dyn_info, ltmbuf));

            fid = THIS_PORT_NUMBERf;
            fval = this_port_number;
            SHR_IF_ERR_EXIT
                (bcmtm_field_set(unit, reg, fid, ltmbuf, &fval));

            fid = THIS_PORT_CAL_SLOT_BITMAPf;
            fval_64[0] = this_port_cal_slot_bitmap[0];
            fval_64[1] = this_port_cal_slot_bitmap[1];
            SHR_IF_ERR_EXIT
                (bcmtm_field_set(unit, reg, fid, ltmbuf, fval_64));

            fid = THIS_SLOT_VALIDf;
            fval = this_slot_valid;
            SHR_IF_ERR_EXIT
                (bcmtm_field_set(unit, reg, fid, ltmbuf, &fval));

            fid = THIS_PORT_SPEEDf;
            fval = this_port_speed;
            SHR_IF_ERR_EXIT
                (bcmtm_field_set(unit, reg, fid, ltmbuf, &fval));

            BCMTM_PT_DYN_INFO(pt_dyn_info, slot, pipe);
            SHR_IF_ERR_EXIT
                (bcmtm_pt_indexed_write(unit, reg, ltid, &pt_dyn_info, ltmbuf));
        }
    }
exit:
    SHR_FUNC_EXIT();
}

static int
td4_tdm_set_max_spacing(int unit,
                        mmu_sched_inst_name_e sched_inst_name,
                        int pipe)
{
    int max_spacing_special_slot;
    int max_spacing_all_speeds, skip_instance;
    bcmtm_drv_info_t *drv_info;
    bcmdrd_sid_t reg;
    bcmtm_pt_info_t pt_dyn_info = {0};
    bcmdrd_fid_t fid ;
    bcmltd_sid_t ltid = -1; /* Requesting LT ID not available */
    uint32_t ltmbuf = 0;
    uint32_t fval;

    SHR_FUNC_ENTER(unit);

    bcmtm_drv_info_get(unit, &drv_info);
    /* Select the right reg based on sched instance. */
    skip_instance = 0;
    switch (sched_inst_name) {
    case TDM_MMU_EB_PORT_SCHED:
        reg = MMU_EBPTS_MAX_SPACINGr;
        break;
    case TDM_MMU_MAIN_SCHED:
    case TDM_MMU_EB_PKT_SCHED:
        skip_instance = 1;
        break;
    default:
        LOG_ERROR(BSL_LS_SOC_PORT,
                  (BSL_META_U(unit,
                              "Unsupported sched_inst_name\n")));
        return SHR_E_PARAM;
        break;
    }

    if (skip_instance == 1) {
        return SHR_E_NONE;
    }

    max_spacing_all_speeds =
        (drv_info->frequency >= 1500) ? td4_max_spacing_settings_tbl[CLK_1500MHZ].all_speed:
        (drv_info->frequency >= 1350) ? td4_max_spacing_settings_tbl[CLK_1350MHZ].all_speed:
        (drv_info->frequency >= 950)  ? td4_max_spacing_settings_tbl[CLK_950MHZ].all_speed:
                                        td4_max_spacing_settings_tbl[CLK_1500MHZ].all_speed;
    max_spacing_special_slot = TD4_TDM_LENGTH;
    BCMTM_PT_DYN_INFO(pt_dyn_info, 0, pipe);
    SHR_IF_ERR_EXIT
        (bcmtm_pt_indexed_read(unit, reg, ltid, &pt_dyn_info, &ltmbuf));
    fid = MAX_SPACING_ALL_SPEEDSf;
    fval = max_spacing_all_speeds;
    SHR_IF_ERR_EXIT
        (bcmtm_field_set(unit, reg, fid, &ltmbuf, &fval));
    fid = MAX_SPACING_SPECIAL_SLOTf;
    fval = max_spacing_special_slot;
    SHR_IF_ERR_EXIT
        (bcmtm_field_set(unit, reg, fid, &ltmbuf, &fval));
    BCMTM_PT_DYN_INFO(pt_dyn_info, 0, pipe);
    SHR_IF_ERR_EXIT
        (bcmtm_pt_indexed_write(unit, reg, ltid, &pt_dyn_info, &ltmbuf));
exit:
    SHR_FUNC_EXIT();
}

static int
td4_tdm_set_feature_ctrl(int unit,
                         mmu_sched_inst_name_e sched_inst_name,
                         int pipe)
{
    int skip_instance;
    int disable_max_spacing;
    int enable_bubble_mop;
    bcmtm_drv_info_t *drv_info;
    bcmdrd_sid_t reg;
    bcmtm_pt_info_t pt_dyn_info = {0};
    bcmdrd_fid_t fid ;
    bcmltd_sid_t ltid = -1; /* Requesting LT ID not available */
    uint32_t ltmbuf = 0;
    uint32_t fval;

    SHR_FUNC_ENTER(unit);

    bcmtm_drv_info_get(unit, &drv_info);

    skip_instance = 0;
    switch (sched_inst_name) {
    case TDM_MMU_EB_PORT_SCHED:
        reg = MMU_EBPTS_FEATURE_CTRLr;
        enable_bubble_mop = 0; /* It's don't care; set to 0. */
        break;
    case TDM_MMU_MAIN_SCHED:
    case TDM_MMU_EB_PKT_SCHED:
        skip_instance = 1;
        break;
    default:
        LOG_ERROR(BSL_LS_SOC_PORT,
                  (BSL_META_U(unit,
                              "Unsupported sched_inst_name\n")));
        return SHR_E_PARAM;
        break;
    }

    if (skip_instance == 1) {
        return SHR_E_NONE;
    }

    disable_max_spacing = 0;
    enable_bubble_mop = 1;
    BCMTM_PT_DYN_INFO(pt_dyn_info, 0, pipe);
    SHR_IF_ERR_EXIT
        (bcmtm_pt_indexed_read(unit, reg, ltid, &pt_dyn_info, &ltmbuf));

    fid = DISABLE_MAX_SPACINGf;
    fval = disable_max_spacing;
    SHR_IF_ERR_EXIT
        (bcmtm_field_set(unit, reg, fid, &ltmbuf, &fval));

    fid = ENABLE_BUBBLE_MOP_400Gf;
    fval = enable_bubble_mop;
    SHR_IF_ERR_EXIT
        (bcmtm_field_set(unit, reg, fid, &ltmbuf, &fval));

    fid = ENABLE_BUBBLE_MOP_200Gf;
    fval = enable_bubble_mop;
    SHR_IF_ERR_EXIT
        (bcmtm_field_set(unit, reg, fid, &ltmbuf, &fval));

    fid = ENABLE_BUBBLE_MOP_100Gf;
    fval = enable_bubble_mop;
    SHR_IF_ERR_EXIT
        (bcmtm_field_set(unit, reg, fid, &ltmbuf, &fval));

    fid = ENABLE_BUBBLE_MOP_50Gf;
    fval = enable_bubble_mop;
    SHR_IF_ERR_EXIT
        (bcmtm_field_set(unit, reg, fid, &ltmbuf, &fval));
    BCMTM_PT_DYN_INFO(pt_dyn_info, 0, pipe);
    SHR_IF_ERR_EXIT
        (bcmtm_pt_indexed_write(unit, reg, ltid, &pt_dyn_info, &ltmbuf));
exit:
    SHR_FUNC_EXIT();
}

static int
td4_tdm_set_cbmg_value(int unit,
                       mmu_sched_inst_name_e sched_inst_name,
                       int pipe_num)
{
    int null_bw, skip_instance;
    int terminal_counter_value, mmu_null_slot_counter_value;

    bcmtm_drv_info_t *drv_info;
    bcmdrd_sid_t reg;
    bcmtm_pt_info_t pt_dyn_info = {0};
    bcmdrd_fid_t fid;
    bcmltd_sid_t ltid = -1; /* Requesting LT ID not available */

    uint32_t ltmbuf = 0;
    uint32_t fval;

    SHR_FUNC_ENTER(unit);

    bcmtm_drv_info_get(unit, &drv_info);
    skip_instance = 0;
    switch (sched_inst_name) {
    case TDM_MMU_EB_PORT_SCHED:
        reg = MMU_EBPTS_CBMG_VALUEr;
        break;
    case TDM_MMU_MAIN_SCHED:
    case TDM_MMU_EB_PKT_SCHED:
        skip_instance = 1;
        break;
    default:
        LOG_ERROR(BSL_LS_SOC_PORT,
                  (BSL_META_U(unit,
                              "Unsupported sched_inst_name\n")));
        return SHR_E_PARAM;
        break;
    }

    if (skip_instance == 1) {
        return SHR_E_NONE;
    }

    /* TD4 only even pipe has null slot generation. */
    if (pipe_num % 2 == 0) {
        /*
         * TD4 bandwidth requirements are roughly as follows:
         * For minimum guarantee of 8.1G(IP) & 3.4(EP) computed at 64B packets,
         * Null/IDLE slots should be picked with an average TDM of once every
         * configurable terminal_counter_value cycles.
         * CBMG_value = [(64+20)*8] * CoreFreq(MHz) / BW(Mbps)
         * IP: IFP Refresh 8.2MHz + 3.8MHz SBUS = 12 MHz total
         * EP: 3.8Mhz SBUS + 1.2MHz TCAM parity scan = 5MHz total
         * Simplified formula:
         * CBMG_value = FLOOR[ CoreFreq(MHz) / NullRate(MHz) ]
         */
        null_bw = 12;
        terminal_counter_value = drv_info->frequency / null_bw -1;
        /*
         * TD4 update: NULL slot value default to 0. Recommended value is 5000.
         * MMU EBPTS Null slot value should be 0 if under PP bypass mode.
         * mmu_null_slot_counter_value = (bypass_mode == 0) ? normal_val : bypass_val;
         */
        mmu_null_slot_counter_value = TD4_NULL_SLOT_COUNTER_VALUE;
    } else {
        terminal_counter_value = 0;
        mmu_null_slot_counter_value = TD4_NULL_SLOT_COUNTER_VALUE;
    }
    BCMTM_PT_DYN_INFO(pt_dyn_info, 0, pipe_num);
    SHR_IF_ERR_EXIT
        (bcmtm_pt_indexed_read(unit, reg, ltid, &pt_dyn_info, &ltmbuf));
    fid = TERMINAL_COUNTER_VALUEf;
    fval = terminal_counter_value;
    SHR_IF_ERR_EXIT
        (bcmtm_field_set(unit, reg, fid, &ltmbuf, &fval));
    fid = MMU_NULL_SLOT_COUNTER_VALUEf;
    fval = mmu_null_slot_counter_value;
    SHR_IF_ERR_EXIT
        (bcmtm_field_set(unit, reg, fid, &ltmbuf, &fval));
    BCMTM_PT_DYN_INFO(pt_dyn_info, 0, pipe_num);
    SHR_IF_ERR_EXIT
        (bcmtm_pt_indexed_write(unit, reg, ltid, &pt_dyn_info, &ltmbuf));
exit:
    SHR_FUNC_EXIT();
}

static int
td4_tdm_set_urg_cell_spacing(int unit,
                             mmu_sched_inst_name_e sched_inst_name)
{
    bcmtm_drv_info_t *drv_info;
    bcmdrd_sid_t reg;
    bcmtm_pt_info_t pt_dyn_info = {0};
    bcmdrd_fid_t fid ;
    bcmltd_sid_t ltid = -1; /* Requesting LT ID not available */
    uint32_t ltmbuf = 0;
    uint32_t fval;
    int core_clock_freq, skip_instance;
    int urg_pick_50g_spacing;
    int urg_pick_100g_spacing;
    int urg_pick_200g_spacing;
    int urg_pick_400g_spacing;

    SHR_FUNC_ENTER(unit);

    bcmtm_drv_info_get(unit, &drv_info);
    /* Select the right reg based on sched instance. */
    skip_instance = 0;

    switch (sched_inst_name) {
    case TDM_MMU_EB_PORT_SCHED:
        reg = MMU_EBPTS_URG_CELL_SPACINGr;
        break;
    case TDM_MMU_MAIN_SCHED:
    case TDM_MMU_EB_PKT_SCHED:
        skip_instance = 1;
        break;
    default:
        LOG_ERROR(BSL_LS_SOC_PORT,
                  (BSL_META_U(unit,
                              "Unsupported sched_inst_name\n")));
        return SHR_E_PARAM;
        break;
    }

    if (skip_instance == 1) {
        return SHR_E_NONE;
    }

    core_clock_freq = drv_info->frequency;
    if (core_clock_freq >= 1500) {
        urg_pick_400g_spacing = td4_urg_pick_spacing_settings_tbl[CLK_1500MHZ].spacing_400;
        urg_pick_200g_spacing = td4_urg_pick_spacing_settings_tbl[CLK_1500MHZ].spacing_200;
        urg_pick_100g_spacing = td4_urg_pick_spacing_settings_tbl[CLK_1500MHZ].spacing_100;
        urg_pick_50g_spacing  = td4_urg_pick_spacing_settings_tbl[CLK_1500MHZ].spacing_50;
    } else if (core_clock_freq >= 1350) {
        urg_pick_400g_spacing = td4_urg_pick_spacing_settings_tbl[CLK_1350MHZ].spacing_400;
        urg_pick_200g_spacing = td4_urg_pick_spacing_settings_tbl[CLK_1350MHZ].spacing_200;
        urg_pick_100g_spacing = td4_urg_pick_spacing_settings_tbl[CLK_1350MHZ].spacing_100;
        urg_pick_50g_spacing  = td4_urg_pick_spacing_settings_tbl[CLK_1350MHZ].spacing_50;
    } else {
        urg_pick_400g_spacing = td4_urg_pick_spacing_settings_tbl[CLK_950MHZ].spacing_400;
        urg_pick_200g_spacing = td4_urg_pick_spacing_settings_tbl[CLK_950MHZ].spacing_200;
        urg_pick_100g_spacing = td4_urg_pick_spacing_settings_tbl[CLK_950MHZ].spacing_100;
        urg_pick_50g_spacing  = td4_urg_pick_spacing_settings_tbl[CLK_950MHZ].spacing_50;
    }
    BCMTM_PT_DYN_INFO(pt_dyn_info, 0, 0);
    SHR_IF_ERR_EXIT
        (bcmtm_pt_indexed_read(unit, reg, ltid, &pt_dyn_info, &ltmbuf));

    fid = URG_PICK_400G_SPACINGf;
    fval = urg_pick_400g_spacing;
    SHR_IF_ERR_EXIT
        (bcmtm_field_set(unit, reg, fid, &ltmbuf, &fval));

    fid = URG_PICK_200G_SPACINGf;
    fval = urg_pick_200g_spacing;
    SHR_IF_ERR_EXIT
        (bcmtm_field_set(unit, reg, fid, &ltmbuf, &fval));

    fid = URG_PICK_100G_SPACINGf;
    fval = urg_pick_100g_spacing;
    SHR_IF_ERR_EXIT
        (bcmtm_field_set(unit, reg, fid, &ltmbuf, &fval));

    fid = URG_PICK_50G_SPACINGf;
    fval = urg_pick_50g_spacing;
    SHR_IF_ERR_EXIT
        (bcmtm_field_set(unit, reg, fid, &ltmbuf, &fval));

    BCMTM_PT_DYN_INFO(pt_dyn_info, 0, 0);
    SHR_IF_ERR_EXIT
        (bcmtm_pt_indexed_write(unit, reg, ltid, &pt_dyn_info, &ltmbuf));
exit:
    SHR_FUNC_EXIT();
}

static int
td4_tdm_set_min_spacing(int unit)
{
    bcmtm_drv_info_t *drv_info;
    bcmdrd_sid_t reg;
    bcmtm_pt_info_t pt_dyn_info = {0};
    bcmdrd_fid_t fid ;
    bcmltd_sid_t ltid = -1; /* Requesting LT ID not available */
    uint32_t ltmbuf = 0;
    uint32_t fval;
    int clk_code, speed_code;
    static const bcmdrd_fid_t any_cell_spacing_fields[] = {
        ANY_CELL_50G_MIN_SPACINGf,  ANY_CELL_100G_MIN_SPACINGf,
        ANY_CELL_200G_MIN_SPACINGf, ANY_CELL_400G_MIN_SPACINGf
    };

    static const bcmdrd_fid_t inter_pkt_spacing_fields[] = {
        INTER_PKT_50G_MIN_SPACINGf,  INTER_PKT_100G_MIN_SPACINGf,
        INTER_PKT_200G_MIN_SPACINGf, INTER_PKT_400G_MIN_SPACINGf
    };

    static const bcmdrd_fid_t mid_pkt_spacing_fields[] = {
        MID_PKT_50G_MIN_SPACINGf,  MID_PKT_100G_MIN_SPACINGf,
        MID_PKT_200G_MIN_SPACINGf, MID_PKT_400G_MIN_SPACINGf
    };

    SHR_FUNC_ENTER(unit);

    bcmtm_drv_info_get(unit, &drv_info);

    clk_code = (drv_info->frequency >= 1500) ? CLK_1500MHZ:
               (drv_info->frequency >= 1350) ? CLK_1350MHZ:
               (drv_info->frequency >= 950)  ? CLK_950MHZ : CLK_1500MHZ;

    reg = MMU_EBPTS_MIN_CELL_SPACINGr;
    BCMTM_PT_DYN_INFO(pt_dyn_info, 0, 0);
    SHR_IF_ERR_EXIT
        (bcmtm_pt_indexed_read(unit, reg, ltid, &pt_dyn_info, &ltmbuf));
    for (speed_code = 0; speed_code < 4; speed_code++) {
        fid = any_cell_spacing_fields[speed_code];
        fval = td4_min_spacing_settings_tbl[clk_code][speed_code].any_cell;
        SHR_IF_ERR_EXIT
            (bcmtm_field_set(unit, reg, fid, &ltmbuf, &fval));
    }
    SHR_IF_ERR_EXIT
        (bcmtm_pt_indexed_write(unit, reg, ltid, &pt_dyn_info, &ltmbuf));

    reg = MMU_EBPTS_MIN_CELL_SPACING_EOP_TO_SOPr;
    BCMTM_PT_DYN_INFO(pt_dyn_info, 0, 0);
    SHR_IF_ERR_EXIT
        (bcmtm_pt_indexed_read(unit, reg, ltid, &pt_dyn_info, &ltmbuf));
    for (speed_code = 0; speed_code < 4; speed_code++) {
        fid = inter_pkt_spacing_fields[speed_code];
        fval = td4_min_spacing_settings_tbl[clk_code][speed_code].inter_pkt;
        SHR_IF_ERR_EXIT
            (bcmtm_field_set(unit, reg, fid, &ltmbuf, &fval));
    }
    SHR_IF_ERR_EXIT
        (bcmtm_pt_indexed_write(unit, reg, ltid, &pt_dyn_info, &ltmbuf));

    reg = MMU_EBPTS_MIN_PORT_PICK_SPACING_WITHIN_PKTr;
    BCMTM_PT_DYN_INFO(pt_dyn_info, 0, 0);
    SHR_IF_ERR_EXIT
        (bcmtm_pt_indexed_read(unit, reg, ltid, &pt_dyn_info, &ltmbuf));
    for (speed_code = 0; speed_code < 4; speed_code++) {
        fid = mid_pkt_spacing_fields[speed_code];
        fval = td4_min_spacing_settings_tbl[clk_code][speed_code].mid_pkt;
        SHR_IF_ERR_EXIT
            (bcmtm_field_set(unit, reg, fid, &ltmbuf, &fval));
    }
    SHR_IF_ERR_EXIT
        (bcmtm_pt_indexed_write(unit, reg, ltid, &pt_dyn_info, &ltmbuf));

exit:
    SHR_FUNC_EXIT();
}


static int
td4_tdm_set_ebpcc_tct_speed(
    int unit)
{
    bcmtm_drv_info_t *drv_info;
    bcmdrd_sid_t reg;
    bcmtm_pt_info_t pt_dyn_info = {0};
    bcmdrd_fid_t fid ;
    bcmltd_sid_t ltid = -1; /* Requesting LT ID not available */
    uint32_t ltmbuf = 0;
    uint32_t fval;

    SHR_FUNC_ENTER(unit);

    bcmtm_drv_info_get(unit, &drv_info);

    /* 10G */
    reg = MMU_EBPCC_TCT_SPEED_0_CFGr;
    BCMTM_PT_DYN_INFO(pt_dyn_info, 0, 0);
    fid = EPORT_CELL_CREDITSf;
    fval = td4_tdm_mmu_eb_credit_config_vals[0];
    SHR_IF_ERR_EXIT
        (bcmtm_field_set(unit, reg, fid, &ltmbuf, &fval));
    SHR_IF_ERR_EXIT
        (bcmtm_pt_indexed_write(unit, reg, ltid, &pt_dyn_info, &ltmbuf));

    /* 25G */
    reg = MMU_EBPCC_TCT_SPEED_1_CFGr;
    BCMTM_PT_DYN_INFO(pt_dyn_info, 0, 0);
    fid = EPORT_CELL_CREDITSf;
    fval = td4_tdm_mmu_eb_credit_config_vals[0];
    SHR_IF_ERR_EXIT
        (bcmtm_field_set(unit, reg, fid, &ltmbuf, &fval));
    SHR_IF_ERR_EXIT
        (bcmtm_pt_indexed_write(unit, reg, ltid, &pt_dyn_info, &ltmbuf));

    /* 40G */
    reg = MMU_EBPCC_TCT_SPEED_2_CFGr;
    BCMTM_PT_DYN_INFO(pt_dyn_info, 0, 0);
    fid = EPORT_CELL_CREDITSf;
    fval = td4_tdm_mmu_eb_credit_config_vals[0];
    SHR_IF_ERR_EXIT
        (bcmtm_field_set(unit, reg, fid, &ltmbuf, &fval));
    SHR_IF_ERR_EXIT
        (bcmtm_pt_indexed_write(unit, reg, ltid, &pt_dyn_info, &ltmbuf));

    /* 50G */
    reg = MMU_EBPCC_TCT_SPEED_3_CFGr;
    BCMTM_PT_DYN_INFO(pt_dyn_info, 0, 0);
    fid = EPORT_CELL_CREDITSf;
    fval = td4_tdm_mmu_eb_credit_config_vals[0];
    SHR_IF_ERR_EXIT
        (bcmtm_field_set(unit, reg, fid, &ltmbuf, &fval));
    SHR_IF_ERR_EXIT
        (bcmtm_pt_indexed_write(unit, reg, ltid, &pt_dyn_info, &ltmbuf));

    /* 100G */
    reg = MMU_EBPCC_TCT_SPEED_4_CFGr;
    BCMTM_PT_DYN_INFO(pt_dyn_info, 0, 0);
    fid = EPORT_CELL_CREDITSf;
    fval = td4_tdm_mmu_eb_credit_config_vals[1];
    SHR_IF_ERR_EXIT
        (bcmtm_field_set(unit, reg, fid, &ltmbuf, &fval));
    SHR_IF_ERR_EXIT
        (bcmtm_pt_indexed_write(unit, reg, ltid, &pt_dyn_info, &ltmbuf));

    /* 200G */
    reg = MMU_EBPCC_TCT_SPEED_5_CFGr;
    BCMTM_PT_DYN_INFO(pt_dyn_info, 0, 0);
    fid = EPORT_CELL_CREDITSf;
    fval = td4_tdm_mmu_eb_credit_config_vals[2];
    SHR_IF_ERR_EXIT
        (bcmtm_field_set(unit, reg, fid, &ltmbuf, &fval));
    SHR_IF_ERR_EXIT
        (bcmtm_pt_indexed_write(unit, reg, ltid, &pt_dyn_info, &ltmbuf));

    /* 400G */
    reg = MMU_EBPCC_TCT_SPEED_6_CFGr;
    BCMTM_PT_DYN_INFO(pt_dyn_info, 0, 0);
    fid = EPORT_CELL_CREDITSf;
    fval = td4_tdm_mmu_eb_credit_config_vals[3];
    SHR_IF_ERR_EXIT
        (bcmtm_field_set(unit, reg, fid, &ltmbuf, &fval));
    SHR_IF_ERR_EXIT
        (bcmtm_pt_indexed_write(unit, reg, ltid, &pt_dyn_info, &ltmbuf));

exit:
    SHR_FUNC_EXIT();
}


static int
td4_tdm_set_pksch_pkt_credits(int unit,
                              mmu_sched_inst_name_e sched_inst_name)
{
    int skip_instance;
    int pkt_credit_count;
    int pkt_credit_50g;
    int pkt_credit_100g;
    int pkt_credit_200g;
    int pkt_credit_400g;
    bcmtm_drv_info_t *drv_info;
    bcmdrd_sid_t reg;
    bcmtm_pt_info_t pt_dyn_info = {0};
    bcmdrd_fid_t fid ;
    bcmltd_sid_t ltid = -1; /* Requesting LT ID not available */
    uint32_t ltmbuf = 0;
    uint32_t fval;

    SHR_FUNC_ENTER(unit);

    bcmtm_drv_info_get(unit, &drv_info);
    /* TD4 recommended */
    pkt_credit_count = 20;
    skip_instance = 0;
    switch (sched_inst_name) {
    case TDM_MMU_EB_PKT_SCHED:
        reg = MMU_EBPTS_PKSCH_PKT_CREDITS_PER_PIPEr;
        break;
    case TDM_MMU_MAIN_SCHED:
    case TDM_MMU_EB_PORT_SCHED:
        skip_instance = 1;
        break;
    default:
        LOG_ERROR(BSL_LS_SOC_PORT,
                  (BSL_META_U(unit,
                              "Unsupported sched_inst_name\n")));
        return SHR_E_PARAM;
        break;
    }

    if (skip_instance == 1) {
        return SHR_E_NONE;
    }

    BCMTM_PT_DYN_INFO(pt_dyn_info, 0, 0);
    SHR_IF_ERR_EXIT
        (bcmtm_pt_indexed_read(unit, reg, ltid, &pt_dyn_info, &ltmbuf));
    fid = PKT_CREDIT_COUNTf;
    fval = pkt_credit_count;
    SHR_IF_ERR_EXIT
        (bcmtm_field_set(unit, reg, fid, &ltmbuf, &fval));
    SHR_IF_ERR_EXIT
        (bcmtm_pt_indexed_write(unit, reg, ltid, &pt_dyn_info, &ltmbuf));

    reg = MMU_EBPTS_PKSCH_PKT_CREDITS_PER_PORTr;
    pkt_credit_50g = 1;
    pkt_credit_100g = 2;
    pkt_credit_200g = 3;
    pkt_credit_400g = 4;
    BCMTM_PT_DYN_INFO(pt_dyn_info, 0, 0);
    SHR_IF_ERR_EXIT
        (bcmtm_pt_indexed_read(unit, reg, ltid, &pt_dyn_info, &ltmbuf));
    fid = PKT_CREDITS_FOR_50Gf;
    fval = pkt_credit_50g;
    SHR_IF_ERR_EXIT
        (bcmtm_field_set(unit, reg, fid, &ltmbuf, &fval));

    fid = PKT_CREDITS_FOR_100Gf;
    fval = pkt_credit_100g;
    SHR_IF_ERR_EXIT
        (bcmtm_field_set(unit, reg, fid, &ltmbuf, &fval));

    fid = PKT_CREDITS_FOR_200Gf;
    fval = pkt_credit_200g;
    SHR_IF_ERR_EXIT
        (bcmtm_field_set(unit, reg, fid, &ltmbuf, &fval));

    fid = PKT_CREDITS_FOR_400Gf;
    fval = pkt_credit_400g;
    SHR_IF_ERR_EXIT
        (bcmtm_field_set(unit, reg, fid, &ltmbuf, &fval));
    SHR_IF_ERR_EXIT
        (bcmtm_pt_indexed_write(unit, reg, ltid, &pt_dyn_info, &ltmbuf));
exit:
    SHR_FUNC_EXIT();
}

static int
td4_tdm_set_eb_credit_config(int unit,
                             int lport,
                             mmu_sched_inst_name_e sched_inst_name,
                             int is_port_down)
{
    int inst;
    int mmu_port;
    uint32_t num_eb_credits;
    int skip_instance;
    uint32_t rval;
    bcmdrd_sid_t reg;
    bcmtm_pt_info_t pt_dyn_info = {0};
    bcmtm_port_map_info_t *port_map;
    bcmltd_sid_t ltid = -1; /* Requesting LT ID not available */

    SHR_FUNC_ENTER(unit);

    bcmtm_port_map_get(unit, &port_map);

    mmu_port = port_map->lport_map[lport].mport;

    skip_instance = 0;
    switch (sched_inst_name) {
    case TDM_MMU_MAIN_SCHED:
        reg = MMU_PTSCH_EB_CREDIT_CONFIGr;
        break;
    case TDM_MMU_EB_PORT_SCHED:
    case TDM_MMU_EB_PKT_SCHED:
        skip_instance = 1;
        break;
    default:
        LOG_ERROR(BSL_LS_SOC_PORT,
                  (BSL_META_U(unit,
                              "Unsupported sched_inst_name\n")));
        return SHR_E_PARAM;
        break;
    }

    /* Only MMU main prt scheduler gets this reg configured. */
    if (skip_instance == 1) {
        return SHR_E_NONE;
    }

    if (is_port_down == 1) {
        num_eb_credits = 0;
    } else {
        switch (port_map->lport_map[lport].max_speed) {
        case TDM_SPEED_400G:
            num_eb_credits = td4_tdm_mmu_eb_credit_config_vals[3];
            break;
        case TDM_SPEED_200G:
            num_eb_credits = td4_tdm_mmu_eb_credit_config_vals[2];
            break;
        case TDM_SPEED_100G:
            num_eb_credits = td4_tdm_mmu_eb_credit_config_vals[1];
            break;
        default: /* any speed <= 50G; */
            num_eb_credits = td4_tdm_mmu_eb_credit_config_vals[0];
            break;
        }
    }

    rval = 0;
    SHR_IF_ERR_EXIT
        (bcmtm_field_set(unit, reg, NUM_EB_CREDITSf, &rval, &num_eb_credits));

    switch (sched_inst_name) {
    case TDM_MMU_MAIN_SCHED:
        inst = mmu_port;
        BCMTM_PT_DYN_INFO(pt_dyn_info, 0, inst);
        SHR_IF_ERR_EXIT
            (bcmtm_pt_indexed_write(unit, reg, ltid, &pt_dyn_info, &rval));
        break;
    /*
     * COVERITY
     *
     * This default is unreachable. It is kept intentionally as a defensive
     * default for error checking.
     */
    /* coverity[dead_error_begin] */
    default:
        LOG_ERROR(BSL_LS_SOC_PORT,
                  (BSL_META_U(unit,
                              "Unsupported sched_inst_name 2nd\n")));
        return SHR_E_PARAM;
        break;
    }

exit:
    SHR_FUNC_EXIT();
}

/*! @fn int td4_tdm_clear_edb_credit_counter_per_port(
 *    int unit,
 *    soc_port_schedule_state_t *port_schedule_state,
 *    mmu_sched_inst_name_e sched_inst_name,
 *    int pipe,
 *    int phy_port,
 *    int is_port_down)
 *  @param unit Chip unit number.
 *  @param port_schedule_state Pointer to a soc_port_schedule_state_t
 *             struct variable.
 *  @param sched_inst_name name enum of instance
 *  @param pipe pipe number.
 *  @param phy_port phy_port to configure.
 *  @param is_port_down. boolean to indicate if port down
 *  @brief API to clear per port EDB_CREDIT_COUNTER
 * Description:
 *      API to initialize/clear per port EDB_CREDIT_COUNTER
 *      Regs/Mems configured: MMU EB_PORT_SCHED
 * Parameters:
 *      unit - Device number
 *      *port_schedule_state_t - Agreed structure between SDK and DV
 * Return Value:
 *      SOC_E_*
 */
static int
td4_tdm_clear_edb_credit_counter_per_port(int unit,
                                          int lport,
                                          mmu_sched_inst_name_e name,
                                          int is_port_down)
{
    int inst;
    int mmu_port;
    uint32_t num_edb_credits;
    int skip_instance;
    uint32_t rval;
    bcmdrd_sid_t reg;
    bcmtm_pt_info_t pt_dyn_info = {0};
    bcmltd_sid_t ltid = -1; /* Requesting LT ID not available */
    bcmtm_port_map_info_t *port_map;

    SHR_FUNC_ENTER(unit);
    bcmtm_port_map_get(unit, &port_map);

    skip_instance = 0;
    switch (name) {
    case TDM_MMU_EB_PORT_SCHED:
        reg = MMU_EBPTS_EDB_CREDIT_COUNTERr;
        break;
    case TDM_MMU_MAIN_SCHED:
    case TDM_MMU_EB_PKT_SCHED:
        skip_instance = 1;
        break;
    default:
        LOG_ERROR(BSL_LS_SOC_PORT,
                  (BSL_META_U(unit,
                              "Unsupported sched_inst_name\n")));
        return SHR_E_PARAM;
        break;
    }

    /* Only EB prt scheduler gets this reg configured. */
    if (skip_instance == 1) {
        return SHR_E_NONE;
    }

    mmu_port = port_map->lport_map[lport].mport;
    num_edb_credits = 0;

    rval = 0;
    SHR_IF_ERR_EXIT
        (bcmtm_field_set(unit, reg, NUM_EDB_CREDITSf, &rval, &num_edb_credits));

    switch (name) {
    case TDM_MMU_EB_PORT_SCHED:
        inst = mmu_port;
        BCMTM_PT_DYN_INFO(pt_dyn_info, 0, inst);
        SHR_IF_ERR_EXIT
            (bcmtm_pt_indexed_write(unit, reg, ltid, &pt_dyn_info, &rval));

        break;
    /*
     * COVERITY
     *
     * This default is unreachable. It is kept intentionally as a defensive
     * default for error checking.
     */
    /* coverity[dead_error_begin] */
    default:
        LOG_ERROR(BSL_LS_SOC_PORT,
                  (BSL_META_U(unit,
                              "Unsupported sched_inst_name 2nd\n")));
        return SHR_E_PARAM;
        break;
    }

exit:
    SHR_FUNC_EXIT();
}

/*! @fn int td4_tdm_set_eb_satisfied_threshold(
 *    int unit,
 *    soc_port_schedule_state_t *port_schedule_state,
 *    mmu_sched_inst_name_e sched_inst_name,
 *    int pipe_num)
 *  @param unit Chip unit number.
 *  @param sched_inst_name name enum of instance
 *  @param pipe pipe number.
 *  @brief API to initialize/clear per port EB_SATISFIED_THRESHOLD
 * Description:
 *      API to initialize/clear per port EB_SATISFIED_THRESHOLD
 *      Regs/Mems configured: MMU MAIN PRT SCHED
 * Parameters:
 *      unit - Device number
 *      *port_schedule_state_t - Agreed structure between SDK and DV
 * Return Value:
 *      SOC_E_*
 */
static int
td4_tdm_set_eb_satisfied_threshold(int unit,
                                   mmu_sched_inst_name_e sched_inst_name)
{
    uint32_t num_satisfaction_credits_200g;
    uint32_t num_satisfaction_credits_100g;
    uint32_t num_satisfaction_credits_50g;
    uint32_t num_satisfaction_credits_400g;
    int skip_instance;
    uint32_t rval;
    bcmdrd_sid_t reg;
    bcmtm_pt_info_t pt_dyn_info = {0};
    bcmltd_sid_t ltid = -1; /* Requesting LT ID not available */

    SHR_FUNC_ENTER(unit);

    /* Select the right reg based on sched instance. */
    skip_instance = 0;

    switch (sched_inst_name) {
    case TDM_MMU_MAIN_SCHED:
        reg = MMU_PTSCH_EB_SATISFIED_THRESHOLDr;
        break;
    case TDM_MMU_EB_PORT_SCHED:
    case TDM_MMU_EB_PKT_SCHED:
        skip_instance = 1;
        break;
    default:
        LOG_ERROR(BSL_LS_SOC_PORT,
                  (BSL_META_U(unit,
                              "Unsupported sched_inst_name\n")));
        return SHR_E_PARAM;
        break;
    }

    if (skip_instance == 1) {return SHR_E_NONE;}

    /* 50% from EB_CREDIT_CONFIG */
    num_satisfaction_credits_50g  = td4_tdm_mmu_eb_credit_config_vals[0] / 2;
    num_satisfaction_credits_100g = td4_tdm_mmu_eb_credit_config_vals[1] / 2;
    num_satisfaction_credits_200g = td4_tdm_mmu_eb_credit_config_vals[2] / 2;
    num_satisfaction_credits_400g = td4_tdm_mmu_eb_credit_config_vals[3] / 2;

    rval = 0;
    SHR_IF_ERR_EXIT(bcmtm_field_set(unit,
                                    reg,
                                    NUM_SATISFACTION_CREDITS_200Gf,
                                    &rval,
                                    &num_satisfaction_credits_200g));
    SHR_IF_ERR_EXIT(bcmtm_field_set(unit,
                                    reg,
                                    NUM_SATISFACTION_CREDITS_100Gf,
                                    &rval,
                                    &num_satisfaction_credits_100g));
    SHR_IF_ERR_EXIT(bcmtm_field_set(unit,
                                    reg,
                                    NUM_SATISFACTION_CREDITS_50Gf,
                                    &rval,
                                    &num_satisfaction_credits_50g));
    SHR_IF_ERR_EXIT(bcmtm_field_set(unit,
                                    reg,
                                    NUM_SATISFACTION_CREDITS_400Gf,
                                    &rval,
                                    &num_satisfaction_credits_400g));

    switch (sched_inst_name) {
    case TDM_MMU_MAIN_SCHED:
        BCMTM_PT_DYN_INFO(pt_dyn_info, 0, 0);
        SHR_IF_ERR_EXIT
            (bcmtm_pt_indexed_write(unit, reg, ltid, &pt_dyn_info, &rval));
        break;
    /*
     * COVERITY
     *
     * This default is unreachable. It is kept intentionally as a defensive
     * default for error checking.
     */
    /* coverity[dead_error_begin] */
    default:
        LOG_ERROR(BSL_LS_SOC_PORT,
                  (BSL_META_U(unit,
                              "Unsupported sched_inst_name 2nd\n")));
        return SHR_E_PARAM;
        break;
    }

exit:
    SHR_FUNC_EXIT();
}


/*! @fn int td4_tdm_set_mmu_edb_credit_threshold(
 *    int unit,
 *    soc_port_schedule_state_t *port_schedule_state,
 *    mmu_sched_inst_name_e sched_inst_name,
 *    int pipe_num)
 *  @param unit Chip unit number.
 *  @param port_schedule_state Pointer to a soc_port_schedule_state_t
 *             struct variable.
 *  @param sched_inst_name name enum of instance
 *  @param pipe pipe number.
 *  @brief API to initialize/clear per port EDB_PORT_CREDIT_THRESHOLD_0/1
 * Description:
 *      API to initialize/clear per port EDB_PORT_CREDIT_THRESHOLD_0/1
 *      Regs/Mems configured: MMU EB PRT SCHED
 * Parameters:
 *      unit - Device number
 *      *port_schedule_state_t - Agreed structure between SDK and DV
 * Return Value:
 *      SOC_E_*
 */
static int
td4_tdm_set_mmu_edb_credit_threshold(int unit,
                                     mmu_sched_inst_name_e sched_inst_name)
{
    uint32_t edb_credit_threshold_50g;
    uint32_t edb_credit_threshold_100g;
    uint32_t edb_credit_threshold_200g;
    uint32_t edb_credit_threshold_400g;
    int skip_instance;
    uint32_t rval_0, rval_1;
    bcmdrd_sid_t reg_0, reg_1;
    bcmtm_pt_info_t pt_dyn_info = {0};
    bcmltd_sid_t ltid = -1; /* Requesting LT ID not available */

    SHR_FUNC_ENTER(unit);

    /* Select the right reg based on sched instance. */
    skip_instance = 0;
    switch (sched_inst_name) {
    case TDM_MMU_EB_PORT_SCHED:
        reg_0 = MMU_EBPTS_EDB_PORT_CREDIT_THRESHOLD_0r;
        reg_1 = MMU_EBPTS_EDB_PORT_CREDIT_THRESHOLD_1r;
        break;
    case TDM_MMU_MAIN_SCHED:
    case TDM_MMU_EB_PKT_SCHED:
        skip_instance = 1;
        break;
    default:
        LOG_ERROR(BSL_LS_SOC_PORT,
                  (BSL_META_U(unit,
                              "Unsupported sched_inst_name\n")));
        return SHR_E_PARAM;
        break;
    }

    if (skip_instance == 1) {return SHR_E_NONE;}

    /* 50% from EDB to MMU CREDITs for that speed */
    SHR_IF_ERR_EXIT
        (td4_ep_get_ep2mmu_credit_per_speed(unit,
                                            TDM_SPEED_50G,
                                            &edb_credit_threshold_50g));
    edb_credit_threshold_50g  = edb_credit_threshold_50g / 2;
    SHR_IF_ERR_EXIT
        (td4_ep_get_ep2mmu_credit_per_speed(unit,
                                            TDM_SPEED_100G,
                                            &edb_credit_threshold_100g));
    edb_credit_threshold_100g  = edb_credit_threshold_100g / 2;
    SHR_IF_ERR_EXIT
        (td4_ep_get_ep2mmu_credit_per_speed(unit,
                                            TDM_SPEED_200G,
                                            &edb_credit_threshold_200g));
    edb_credit_threshold_200g  = edb_credit_threshold_200g / 2;
    SHR_IF_ERR_EXIT
        (td4_ep_get_ep2mmu_credit_per_speed(unit,
                                            TDM_SPEED_400G,
                                            &edb_credit_threshold_400g));
    edb_credit_threshold_400g  = edb_credit_threshold_400g / 2;

    rval_0 = 0;
    rval_1 = 0;
    SHR_IF_ERR_EXIT
        (bcmtm_field_set(unit,
                         reg_0,
                         EDB_CREDIT_THRESHOLD_50Gf,
                         &rval_0,
                         &edb_credit_threshold_50g));
    SHR_IF_ERR_EXIT
        (bcmtm_field_set(unit,
                         reg_0,
                         EDB_CREDIT_THRESHOLD_100Gf,
                         &rval_0,
                         &edb_credit_threshold_100g));
    SHR_IF_ERR_EXIT
        (bcmtm_field_set(unit,
                         reg_1,
                         EDB_CREDIT_THRESHOLD_200Gf,
                         &rval_1,
                         &edb_credit_threshold_200g));
    SHR_IF_ERR_EXIT
        (bcmtm_field_set(unit,
                         reg_1,
                         EDB_CREDIT_THRESHOLD_400Gf,
                         &rval_1,
                         &edb_credit_threshold_400g));

    switch (sched_inst_name) {
    case TDM_MMU_EB_PORT_SCHED:
        BCMTM_PT_DYN_INFO(pt_dyn_info, 0, 0);

        SHR_IF_ERR_EXIT
            (bcmtm_pt_indexed_write(unit, reg_0, ltid, &pt_dyn_info, &rval_0));
        SHR_IF_ERR_EXIT
            (bcmtm_pt_indexed_write(unit, reg_1, ltid, &pt_dyn_info, &rval_1));
        break;
    /*
     * COVERITY
     *
     * This default is unreachable. It is kept intentionally as a defensive
     * default for error checking.
     */
    /* coverity[dead_error_begin] */
    default:
        LOG_ERROR(BSL_LS_SOC_PORT,
                  (BSL_META_U(unit,
                              "Unsupported sched_inst_name 2nd\n")));
        return SHR_E_PARAM;
        break;
    }

exit:
    SHR_FUNC_EXIT();
}

static int
td4_tdm_sched_init(int unit,
                   mmu_sched_inst_name_e sched_inst_name, uint32_t pipe_map)
{
    uint32_t pipe;
    bcmtm_drv_info_t *drv_info;

    SHR_FUNC_ENTER(unit);
    bcmtm_drv_info_get(unit, &drv_info);

    /* Takes effect only in MMU MAIN PRT scheduler */
    SHR_IF_ERR_EXIT
        (td4_tdm_set_eb_satisfied_threshold(unit, sched_inst_name));
    /* Takes effect only in MMU EB   PRT scheduler */
    SHR_IF_ERR_EXIT
        (td4_tdm_set_mmu_edb_credit_threshold(unit, sched_inst_name));


    /* Takes effect only in MMU EB   PRT scheduler */
    SHR_IF_ERR_EXIT
        (td4_tdm_set_urg_cell_spacing(unit, sched_inst_name));
    /* Takes effect only in PKT SCH*/
    SHR_IF_ERR_EXIT
        (td4_tdm_set_pksch_pkt_credits(unit, sched_inst_name));

    /* Per pipe configs; at init time only; not part of FlexPort */
    for (pipe = 0; pipe < TD4_PIPES_PER_DEV; pipe++) {
        if ((pipe_map & (1 << pipe)) == 0) {
            continue;
        }

        /* Takes effect only in IDB/MMU_EB PRT SCHED*/
        SHR_IF_ERR_EXIT
            (td4_tdm_set_max_spacing(unit, sched_inst_name, pipe));
        SHR_IF_ERR_EXIT
            (td4_tdm_set_feature_ctrl(unit,sched_inst_name, pipe));
        SHR_IF_ERR_EXIT
            (td4_tdm_set_cbmg_value(unit, sched_inst_name, pipe));

    }
    if (sched_inst_name == TDM_MMU_EB_PORT_SCHED) {
        SHR_IF_ERR_EXIT(td4_tdm_set_ebpcc_tct_speed(unit));
    }

exit:
    SHR_FUNC_EXIT();

}

/*! @fn int td4_tdm_mmu_init(int unit,
 *              soc_port_schedule_state_t *port_schedule_state)
 *  @param unit Chip unit number.
 *  @param port_schedule_state Pointer to a soc_port_schedule_state_t
 *             struct variable.
 *  @brief API to initialize the TD4 MMU schedulers.
 * Description:
 *      API to initialize the TD4 MMU schedulers.
 *      Regs/Mems configured: TD4 MMU schedulers.
 * Parameters:
 *      unit - Device number
 *      *port_schedule_state_t - Agreed structure between SDK and DV
 * Return Value:
 *      SOC_E_*
 */
static int
td4_tdm_mmu_init(int unit, uint32_t pipe_map)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_EXIT
        (td4_tdm_set_min_spacing(unit));

    SHR_IF_ERR_EXIT
        (td4_tdm_sched_init(unit, TDM_MMU_MAIN_SCHED, pipe_map));
    SHR_IF_ERR_EXIT
        (td4_tdm_sched_init(unit, TDM_MMU_EB_PORT_SCHED, pipe_map));
    SHR_IF_ERR_EXIT
        (td4_tdm_sched_init(unit, TDM_MMU_EB_PKT_SCHED, pipe_map));

exit:
    SHR_FUNC_EXIT();
}

static int
td4_tdm_sched_flexport(int unit,
                       int *lport_bmp,
                       mmu_sched_inst_name_e sched_inst_name,
                       int ports_down)
{
    int lport;
    bcmtm_drv_info_t *drv_info;

    SHR_FUNC_ENTER(unit);
    bcmtm_drv_info_get(unit, &drv_info);

    if (ports_down == 1) { /* remove ports from scheduler. */
        for (lport = 0; lport < TD4_DEV_PORTS_PER_DEV; lport++) {
            if (lport_bmp[lport]) {

                /* Removes slots allocated to this port from scheduler. */
                SHR_IF_ERR_EXIT
                    (td4_tdm_set_cal_config(unit,
                                            lport,
                                            sched_inst_name,
                                            ports_down,
                                            0));
            }
        }
    } else {               /* Aadd ports to scheduler. */
        for (lport = 0; lport < TD4_DEV_PORTS_PER_DEV; lport++) {
            if (lport_bmp[lport]) {

                /* Add slots allocated to this port in scheduler. */
                SHR_IF_ERR_EXIT
                    (td4_tdm_set_cal_config(unit,
                                            lport,
                                            sched_inst_name,
                                            ports_down,
                                            0));
                /* Configure per port EB credits. */
                SHR_IF_ERR_EXIT
                    (td4_tdm_set_eb_credit_config(unit,
                                                  lport,
                                                  sched_inst_name,
                                                  ports_down));
            }
        }
    }

 exit:
    SHR_FUNC_EXIT();
}

/*! @fn int td4_tdm_mmu_flexport(int unit,
 *              soc_port_schedule_state_t *port_schedule_state)
 *  @param unit Chip unit number.
 *  @param port_schedule_state Pointer to a soc_port_schedule_state_t
 *             struct variable.
 *  @brief API to FLEX the TD4 MMU schedulers.
 * Description:
 *      API to FLEX the TD4 MMU schedulers.
 *      Regs/Mems configured: TD4 MMU schedulers.
 * Parameters:
 *      unit - Device number
 *      *port_schedule_state_t - Agreed structure between SDK and DV
 * Return Value:
 *      SOC_E_*
 */
int
td4_mmu_tdm_port_down(int unit, int *down_bmp)
{
    int down;

    SHR_FUNC_ENTER(unit);

    /* Remove DOWN ports. */
    down = 1;
    SHR_IF_ERR_EXIT
        (td4_tdm_sched_flexport(unit, down_bmp, TDM_MMU_MAIN_SCHED, down));
    SHR_IF_ERR_EXIT
        (td4_tdm_sched_flexport(unit, down_bmp, TDM_MMU_EB_PORT_SCHED, down));
    SHR_IF_ERR_EXIT
        (td4_tdm_sched_flexport(unit, down_bmp, TDM_MMU_EB_PKT_SCHED, down));

 exit:
    SHR_FUNC_EXIT();

}

int
td4_mmu_tdm_port_up(int unit, int *up_bmp)
{
    int down;

    SHR_FUNC_ENTER(unit);

    /* Add UP ports. */
    down = 0;
    SHR_IF_ERR_EXIT
        (td4_tdm_sched_flexport(unit, up_bmp, TDM_MMU_MAIN_SCHED, down));
    SHR_IF_ERR_EXIT
        (td4_tdm_sched_flexport(unit, up_bmp, TDM_MMU_EB_PORT_SCHED, down));
    SHR_IF_ERR_EXIT
        (td4_tdm_sched_flexport(unit, up_bmp, TDM_MMU_EB_PKT_SCHED, down));

 exit:
    SHR_FUNC_EXIT();

}
/*** END SDK API COMMON CODE ***/

int
bcm56880_a0_tm_tdm_mmu_init(int unit)
{
    uint32_t pipe_map;

    SHR_FUNC_ENTER(unit);
    SHR_IF_ERR_EXIT
        (bcmdrd_dev_valid_pipes_get(unit, &pipe_map));
    SHR_IF_ERR_EXIT
        (td4_tdm_mmu_init(unit, pipe_map));

exit:
    SHR_FUNC_EXIT();
}


int
bcm56880_a0_tm_edb_credit_reset (int unit, bcmtm_pport_t port)
{
    bcmtm_port_map_info_t *port_map;
    int lport;

    SHR_FUNC_ENTER(unit);

    bcmtm_port_map_get(unit, &port_map);
    lport = port_map->pport_map[port].lport;
    SHR_IF_ERR_EXIT
        (td4_tdm_clear_edb_credit_counter_per_port(unit, lport,
            TDM_MMU_EB_PORT_SCHED, 1));
exit:
    SHR_FUNC_EXIT();
}
