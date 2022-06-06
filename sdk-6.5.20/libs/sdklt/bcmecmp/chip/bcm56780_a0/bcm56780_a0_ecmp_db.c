/*! \file bcm56780_a0_ecmp_db.c
 *
 * Chip specific functions for BCMECMP
 */
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

#include <shr/shr_debug.h>
#include <shr/shr_error.h>
#include <shr/shr_pb.h>
#include <shr/shr_ha.h>
#include <shr/shr_bitop.h>
#include <bcmdrd/chip/bcm56780_a0_enum.h>
#include <bcmdrd/bcmdrd_pt.h>
#include <bcmptm/bcmptm.h>
#include <bcmltd/chip/bcmltd_id.h>
#include <bcmlrd/bcmlrd_map.h>
#include <bcmlrd/bcmlrd_table.h>
#include <bcmecmp/bcmecmp.h>
#include <bcmecmp/chip/bcm56780_a0/bcm56780_a0_ecmp.ptrm.data.h>
#include <bcmecmp/chip/bcm56780_a0/bcm56780_a0_ecmp_mem_dest.ptrm.data.h>

#define BSL_LOG_MODULE BSL_LS_BCMLTX_ECMP

static bcmecmp_drv_var_ids_t
   bcm56780_a0_ecmp_drv_var_ids = {
   .ecmp_mlevel0_ltid  = ECMP_MEMBER0t,
   .ecmp_mlevel0_group_id  = ECMP_MEMBER0t_ECMP_MEMBER0_IDf,
   .ecmp_mlevel0_num_entries_id  = ECMP_MEMBER0t_NUM_ENTRIESf,
   .ecmp_mlevel0_base_index_id  = ECMP_MEMBER0t_BASE_INDEXf,
   .ecmp_mlevel0_base_index_auto_id  = ECMP_MEMBER0t_BASE_INDEX_AUTOf,
   .ecmp_mlevel0_base_index_oper_id  = ECMP_MEMBER0t_BASE_INDEX_OPERf,
   .ecmp_level1_ltid = ECMP_LEVEL1t,
   .ecmp_level1_ecmp_id = ECMP_LEVEL1t_ECMP_IDf,
   .ecmp_level1_lb_mode_id = ECMP_LEVEL1t_LB_MODEf,
   .ecmp_level1_max_paths_id = ECMP_LEVEL1t_MAX_PATHSf,
   .ecmp_level1_num_paths_id= ECMP_LEVEL1t_NUM_PATHSf,
   .ecmp_mlevel1_ltid  = ECMP_MEMBER1t,
   .ecmp_mlevel1_group_id  = ECMP_MEMBER1t_ECMP_MEMBER1_IDf,
   .ecmp_mlevel1_num_entries_id  = ECMP_MEMBER1t_NUM_ENTRIESf,
   .ecmp_mlevel1_base_index_id  = ECMP_MEMBER1t_BASE_INDEXf,
   .ecmp_mlevel1_base_index_auto_id  = ECMP_MEMBER1t_BASE_INDEX_AUTOf,
   .ecmp_mlevel1_base_index_oper_id  = ECMP_MEMBER1t_BASE_INDEX_OPERf,
   .ecmp_wlevel1_ltid = ECMP_LEVEL1_WEIGHTEDt,
   .ecmp_wlevel1_ecmp_id = ECMP_LEVEL1_WEIGHTEDt_ECMP_IDf,
   .ecmp_wlevel1_wt_size_id = ECMP_LEVEL1_WEIGHTEDt_WEIGHTED_SIZEf,
   .ecmp_wlevel1_num_paths_id  = ECMP_LEVEL1_WEIGHTEDt_NUM_PATHSf,
   .ecmp_wlevel1_rh_mode_id  = ECMP_LEVEL1_WEIGHTEDt_RH_MODEf,
   .ecmp_wlevel1_rh_rand_seed_id  = ECMP_LEVEL1_WEIGHTEDt_RH_RANDOM_SEEDf,
   .ecmp_wlevel1_rh_num_paths_id  = ECMP_LEVEL1_WEIGHTEDt_RH_NUM_PATHSf,
   .ecmp_wlevel1_wt_mode_id  = ECMP_LEVEL1_WEIGHTEDt_WEIGHTED_MODEf,
   .ecmp_wlevel1_wt_id  = ECMP_LEVEL1_WEIGHTEDt_WEIGHTf,
   .ecmp_wlevel1_wt_nhop_index_id  = ECMP_LEVEL1_WEIGHTEDt_WEIGHTED_NHOP_INDEXf,
   .ecmp_level0_ltid = ECMP_LEVEL0t,
   .ecmp_level0_ecmp_id = ECMP_LEVEL0t_ECMP_IDf,
   .ecmp_level0_lb_mode_id = ECMP_LEVEL0t_LB_MODEf,
   .ecmp_level0_max_paths_id = ECMP_LEVEL0t_MAX_PATHSf,
   .ecmp_level0_num_paths_id= ECMP_LEVEL0t_NUM_PATHSf,
   .ecmp_wlevel0_ltid = ECMP_LEVEL0_WEIGHTEDt,
   .ecmp_wlevel0_ecmp_id = ECMP_LEVEL0_WEIGHTEDt_ECMP_IDf,
   .ecmp_wlevel0_wt_size_id = ECMP_LEVEL0_WEIGHTEDt_WEIGHTED_SIZEf,
   .ecmp_wlevel0_num_paths_id  = ECMP_LEVEL0_WEIGHTEDt_NUM_PATHSf,
   .ecmp_wlevel0_rh_mode_id  = ECMP_LEVEL0_WEIGHTEDt_RH_MODEf,
   .ecmp_wlevel0_rh_rand_seed_id  = ECMP_LEVEL0_WEIGHTEDt_RH_RANDOM_SEEDf,
   .ecmp_wlevel0_rh_num_paths_id  = ECMP_LEVEL0_WEIGHTEDt_RH_NUM_PATHSf,
};
int bcm56780_a0_ecmp_drv_var_attach(
           int unit,
           bcmecmp_drv_var_t *drv_var)
{
    SHR_FUNC_ENTER(unit);
    drv_var->flds_mem0 = bcm56780_a0_ecmp_flds_mem0;
    drv_var->flds_mem1 = bcm56780_a0_ecmp_flds_mem1;
    drv_var->wflds_mem0 = bcm56780_a0_ecmp_wflds_mem0;
    drv_var->wflds_mem1 = bcm56780_a0_ecmp_wflds_mem1;
    drv_var->dest_flds_mem0 = bcm56780_a0_ecmp_mem_dest_flds_mem0;
    drv_var->dest_flds_mem1 = bcm56780_a0_ecmp_mem_dest_flds_mem1;
    drv_var->member0_info = &bcm56780_a0_ecmp_member0_info;
    drv_var->member1_info = &bcm56780_a0_ecmp_member1_info;
    drv_var->wmember0_info = &bcm56780_a0_ecmp_wmember0_info;
    drv_var->wmember1_info = &bcm56780_a0_ecmp_wmember1_info;
    drv_var->dest_member0_info = &bcm56780_a0_ecmp_mem_dest_member0_info;
    drv_var->dest_member1_info = &bcm56780_a0_ecmp_mem_dest_member1_info;
    drv_var->ecmp_mem0 = bcm56780_a0_ecmp_mem0;
    drv_var->ecmp_mem1 = bcm56780_a0_ecmp_mem1;
    drv_var->dest_ecmp_mem0 = bcm56780_a0_ecmp_mem_dest_mem0;
    drv_var->ids = &bcm56780_a0_ecmp_drv_var_ids;
    SHR_FUNC_EXIT();
}
