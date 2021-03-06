/*******************************************************************************
 *
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated by npl-pcm from NPL Logical - Physical Map files
 * for device bcm56880_a0 and variant dna_4_6_6.
 *
 * Tool: $SDK/INTERNAL/fltg/bin/npl-pcm
 *
 * Edits to this file will be lost when it is regenerated.
 *
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */
#include <bcmltd/chip/bcmltd_id.h>
#include <bcmlrd/bcmlrd_types.h>
#include <bcmlrd/chip/bcm56880_a0/dna_4_6_6/bcm56880_a0_dna_4_6_6_lrd_table_pcm_data.h>
#include <bcmlrd/chip/bcm56880_a0/bcm56880_a0_tile_hcf.h>

const bcmlrd_pdd_info_t bcm56880_a0_dna_4_6_6_dt_vfp_action_template_vfp_scratch_bus_input_priority_pdd_info[] = {
    {
      /*
       * BITS:  0 - 3
       * CONT:  ing_cmd1__cont_17
       * LBUS:  vfp_scratch_bus.input_priority
       * CYCLE: 93
       * MAP:   ifta40__PDD
       */
        .phy_cont_id = 77,
        .sbr_phy_cont_id = 77,
        .phy_cont_size = 4,
        .offset = 0,
        .width = 4,
        .is_lsb = 0,
    },
};

const bcmlrd_pdd_info_t bcm56880_a0_dna_4_6_6_dt_vfp_action_template_ing_cmd_bus_cml_flags_move_pdd_info[] = {
    {
      /*
       * BITS:  0 - 3
       * CONT:  ing_cmd1__cont_24
       * LBUS:  ing_cmd_bus.cml_flags_move
       * CYCLE: 93
       * MAP:   ifta40__PDD
       */
        .phy_cont_id = 84,
        .sbr_phy_cont_id = 84,
        .phy_cont_size = 4,
        .offset = 0,
        .width = 4,
        .is_lsb = 0,
    },
};

const bcmlrd_pdd_info_t bcm56880_a0_dna_4_6_6_dt_vfp_action_template_ing_cmd_bus_cng_pdd_info[] = {
    {
      /*
       * BITS:  0 - 1
       * CONT:  ing_cmd1__cont_2
       * LBUS:  ing_cmd_bus.cng
       * CYCLE: 93
       * MAP:   ifta40__PDD
       */
        .phy_cont_id = 62,
        .sbr_phy_cont_id = 62,
        .phy_cont_size = 4,
        .offset = 0,
        .width = 2,
        .is_lsb = 0,
    },
};

const bcmlrd_pdd_info_t bcm56880_a0_dna_4_6_6_dt_vfp_action_template_vfp_scratch_bus_change_input_priority_pdd_info[] = {
    {
      /*
       * BITS:  0 - 0
       * CONT:  ing_scr_uat2uat_fsl0__cont_3
       * LBUS:  vfp_scratch_bus.change_input_priority
       * CYCLE: 93
       * MAP:   ifta40__PDD
       */
        .phy_cont_id = 103,
        .sbr_phy_cont_id = 65535,
        .phy_cont_size = 4,
        .offset = 0,
        .width = 1,
        .is_lsb = 0,
    },
};

const bcmlrd_pdd_info_t bcm56880_a0_dna_4_6_6_dt_vfp_action_template_ing_obj_bus_vfp_opaque_obj0_pdd_info[] = {
    {
      /*
       * BITS:  0 - 15
       * CONT:  ing_obj1__cont_13
       * LBUS:  ing_obj_bus.vfp_vlan_xlate_or_ifp_opaque_obj
       * CYCLE: 93
       * MAP:   ifta40__PDD
       */
        .phy_cont_id = 33,
        .sbr_phy_cont_id = 33,
        .phy_cont_size = 16,
        .offset = 0,
        .width = 16,
        .is_lsb = 0,
    },
};

const bcmlrd_pdd_info_t bcm56880_a0_dna_4_6_6_dt_vfp_action_template_ing_cmd_bus_int_cn_pdd_info[] = {
    {
      /*
       * BITS:  0 - 1
       * CONT:  ing_cmd1__cont_1
       * LBUS:  ing_cmd_bus.int_cn
       * CYCLE: 93
       * MAP:   ifta40__PDD
       */
        .phy_cont_id = 61,
        .sbr_phy_cont_id = 61,
        .phy_cont_size = 4,
        .offset = 0,
        .width = 2,
        .is_lsb = 0,
    },
};

const bcmlrd_pdd_info_t bcm56880_a0_dna_4_6_6_dt_vfp_action_template_ing_obj_bus_vrf_pdd_info[] = {
    {
      /*
       * BITS:  0 - 13
       * CONT:  ing_obj0__cont_7
       * LBUS:  ing_obj_bus.vrf
       * CYCLE: 93
       * MAP:   ifta40__PDD
       */
        .phy_cont_id = 7,
        .sbr_phy_cont_id = 7,
        .phy_cont_size = 16,
        .offset = 0,
        .width = 14,
        .is_lsb = 0,
    },
};

const bcmlrd_pdd_info_t bcm56880_a0_dna_4_6_6_dt_vfp_action_template_vfp_scratch_bus_svp_pdd_info[] = {
    {
      /*
       * BITS:  0 - 15
       * CONT:  ing_scr_uat2uat_fsl1__cont_1
       * LBUS:  vfp_scratch_bus.svp
       * CYCLE: 93
       * MAP:   ifta40__PDD
       */
        .phy_cont_id = 111,
        .sbr_phy_cont_id = 65535,
        .phy_cont_size = 16,
        .offset = 0,
        .width = 16,
        .is_lsb = 0,
    },
};

const bcmlrd_pdd_info_t bcm56880_a0_dna_4_6_6_dt_vfp_action_template_ing_cmd_bus_cml_flags_new_pdd_info[] = {
    {
      /*
       * BITS:  0 - 3
       * CONT:  ing_cmd1__cont_23
       * LBUS:  ing_cmd_bus.cml_flags_new
       * CYCLE: 93
       * MAP:   ifta40__PDD
       */
        .phy_cont_id = 83,
        .sbr_phy_cont_id = 83,
        .phy_cont_size = 4,
        .offset = 0,
        .width = 4,
        .is_lsb = 0,
    },
};

const bcmlrd_pdd_info_t bcm56880_a0_dna_4_6_6_dt_vfp_action_template_ing_cmd_bus_vfp_opaque_ctrl_id_pdd_info[] = {
    {
      /*
       * BITS:  0 - 3
       * CONT:  ing_cmd1__cont_11
       * LBUS:  ing_cmd_bus.vfp_opaque_ctrl_id
       * CYCLE: 93
       * MAP:   ifta40__PDD
       */
        .phy_cont_id = 71,
        .sbr_phy_cont_id = 71,
        .phy_cont_size = 4,
        .offset = 0,
        .width = 4,
        .is_lsb = 0,
    },
};

const bcmlrd_pdd_info_t bcm56880_a0_dna_4_6_6_dt_vfp_action_template_ing_cmd_bus_int_pri_pdd_info[] = {
    {
      /*
       * BITS:  0 - 3
       * CONT:  ing_cmd1__cont_0
       * LBUS:  ing_cmd_bus.int_pri
       * CYCLE: 93
       * MAP:   ifta40__PDD
       */
        .phy_cont_id = 60,
        .sbr_phy_cont_id = 60,
        .phy_cont_size = 4,
        .offset = 0,
        .width = 4,
        .is_lsb = 0,
    },
};

const bcmlrd_pdd_info_t bcm56880_a0_dna_4_6_6_dt_vfp_action_template_vfp_scratch_bus_my_station_1_hit_pdd_info[] = {
    {
      /*
       * BITS:  0 - 0
       * CONT:  ing_cmd0__cont_7
       * LBUS:  vfp_scratch_bus.my_station_1_hit
       * CYCLE: 93
       * MAP:   ifta40__PDD
       */
        .phy_cont_id = 47,
        .sbr_phy_cont_id = 47,
        .phy_cont_size = 4,
        .offset = 0,
        .width = 1,
        .is_lsb = 0,
    },
};

const bcmlrd_pdd_info_t bcm56880_a0_dna_4_6_6_dt_vfp_action_template_ing_obj_bus_vfp_class_id_pdd_info[] = {
    {
      /*
       * BITS:  0 - 15
       * CONT:  ing_obj1__cont_16
       * LBUS:  ing_obj_bus.vfp_class_id
       * CYCLE: 93
       * MAP:   ifta40__PDD
       */
        .phy_cont_id = 36,
        .sbr_phy_cont_id = 36,
        .phy_cont_size = 16,
        .offset = 0,
        .width = 16,
        .is_lsb = 0,
    },
};

const bcmlrd_pdd_info_t bcm56880_a0_dna_4_6_6_dt_vfp_action_template_ing_cmd_bus_vrf_strength_pdd_info[] = {
    {
      /*
       * BITS:  0 - 2
       * CONT:  ing_cmd1__cont_4
       * LBUS:  ing_cmd_bus.vrf_strength
       * CYCLE: 93
       * MAP:   ifta40__PDD
       */
        .phy_cont_id = 64,
        .sbr_phy_cont_id = 64,
        .phy_cont_size = 4,
        .offset = 0,
        .width = 3,
        .is_lsb = 0,
    },
};

const bcmlrd_pdd_info_t bcm56880_a0_dna_4_6_6_dt_vfp_action_template_ing_obj_bus_flow_group_id_pdd_info[] = {
    {
      /*
       * BITS:  0 - 9
       * CONT:  ing_obj1__cont_17
       * LBUS:  ing_obj_bus.flow_group_id
       * CYCLE: 93
       * MAP:   ifta40__PDD
       */
        .phy_cont_id = 37,
        .sbr_phy_cont_id = 37,
        .phy_cont_size = 16,
        .offset = 0,
        .width = 10,
        .is_lsb = 0,
    },
};

const bcmlrd_pdd_info_t bcm56880_a0_dna_4_6_6_dt_vfp_action_template_vfp_scratch_bus_drop_action_pdd_info[] = {
    {
      /*
       * BITS:  0 - 1
       * CONT:  ing_scr_uat2uat_fsl0__cont_4
       * LBUS:  vfp_scratch_bus.drop_action
       * CYCLE: 93
       * MAP:   ifta40__PDD
       */
        .phy_cont_id = 104,
        .sbr_phy_cont_id = 65535,
        .phy_cont_size = 4,
        .offset = 0,
        .width = 2,
        .is_lsb = 0,
    },
};

const bcmlrd_pdd_info_t bcm56880_a0_dna_4_6_6_dt_vfp_action_template_vfp_scratch_bus_change_vfi_pdd_info[] = {
    {
      /*
       * BITS:  0 - 0
       * CONT:  ing_scr_uat2uat_fsl0__cont_2
       * LBUS:  vfp_scratch_bus.change_vfi
       * CYCLE: 93
       * MAP:   ifta40__PDD
       */
        .phy_cont_id = 102,
        .sbr_phy_cont_id = 65535,
        .phy_cont_size = 4,
        .offset = 0,
        .width = 1,
        .is_lsb = 0,
    },
};

const bcmlrd_pdd_info_t bcm56880_a0_dna_4_6_6_dt_vfp_action_template_vfp_scratch_bus_change_svp_pdd_info[] = {
    {
      /*
       * BITS:  0 - 0
       * CONT:  ing_scr_uat2uat_fsl0__cont_1
       * LBUS:  vfp_scratch_bus.change_svp
       * CYCLE: 93
       * MAP:   ifta40__PDD
       */
        .phy_cont_id = 101,
        .sbr_phy_cont_id = 65535,
        .phy_cont_size = 4,
        .offset = 0,
        .width = 1,
        .is_lsb = 0,
    },
};

const bcmlrd_pdd_info_t bcm56880_a0_dna_4_6_6_dt_vfp_action_template_vfp_scratch_bus_vfi_pdd_info[] = {
    {
      /*
       * BITS:  0 - 15
       * CONT:  ing_scr_uat2uat_fsl1__cont_0
       * LBUS:  vfp_scratch_bus.vfi
       * CYCLE: 93
       * MAP:   ifta40__PDD
       */
        .phy_cont_id = 110,
        .sbr_phy_cont_id = 65535,
        .phy_cont_size = 16,
        .offset = 0,
        .width = 16,
        .is_lsb = 0,
    },
};

const bcmlrd_pdd_info_t bcm56880_a0_dna_4_6_6_dt_vfp_action_template_vfp_scratch_bus_copy_to_cpu_action_pdd_info[] = {
    {
      /*
       * BITS:  0 - 1
       * CONT:  ing_scr_uat2uat_fsl0__cont_0
       * LBUS:  vfp_scratch_bus.copy_to_cpu_action
       * CYCLE: 93
       * MAP:   ifta40__PDD
       */
        .phy_cont_id = 100,
        .sbr_phy_cont_id = 65535,
        .phy_cont_size = 4,
        .offset = 0,
        .width = 2,
        .is_lsb = 0,
    },
};


const bcmlrd_field_pdd_info_t bcm56880_a0_dna_4_6_6_dt_vfp_action_template_vfp_scratch_bus_input_priority_field_pdd_info = {
        .count = 1,
        .sbr_type = BCMLRD_SBR_INTERNAL,
        .info = bcm56880_a0_dna_4_6_6_dt_vfp_action_template_vfp_scratch_bus_input_priority_pdd_info,
};

const bcmlrd_field_pdd_info_t bcm56880_a0_dna_4_6_6_dt_vfp_action_template_ing_cmd_bus_cml_flags_move_field_pdd_info = {
        .count = 1,
        .sbr_type = BCMLRD_SBR_EXTERNAL,
        .info = bcm56880_a0_dna_4_6_6_dt_vfp_action_template_ing_cmd_bus_cml_flags_move_pdd_info,
};

const bcmlrd_field_pdd_info_t bcm56880_a0_dna_4_6_6_dt_vfp_action_template_ing_cmd_bus_cng_field_pdd_info = {
        .count = 1,
        .sbr_type = BCMLRD_SBR_EXTERNAL,
        .info = bcm56880_a0_dna_4_6_6_dt_vfp_action_template_ing_cmd_bus_cng_pdd_info,
};

const bcmlrd_field_pdd_info_t bcm56880_a0_dna_4_6_6_dt_vfp_action_template_vfp_scratch_bus_change_input_priority_field_pdd_info = {
        .count = 1,
        .sbr_type = BCMLRD_SBR_NONE,
        .info = bcm56880_a0_dna_4_6_6_dt_vfp_action_template_vfp_scratch_bus_change_input_priority_pdd_info,
};

const bcmlrd_field_pdd_info_t bcm56880_a0_dna_4_6_6_dt_vfp_action_template_ing_obj_bus_vfp_opaque_obj0_field_pdd_info = {
        .count = 1,
        .sbr_type = BCMLRD_SBR_EXTERNAL,
        .info = bcm56880_a0_dna_4_6_6_dt_vfp_action_template_ing_obj_bus_vfp_opaque_obj0_pdd_info,
};

const bcmlrd_field_pdd_info_t bcm56880_a0_dna_4_6_6_dt_vfp_action_template_ing_cmd_bus_int_cn_field_pdd_info = {
        .count = 1,
        .sbr_type = BCMLRD_SBR_EXTERNAL,
        .info = bcm56880_a0_dna_4_6_6_dt_vfp_action_template_ing_cmd_bus_int_cn_pdd_info,
};

const bcmlrd_field_pdd_info_t bcm56880_a0_dna_4_6_6_dt_vfp_action_template_ing_obj_bus_vrf_field_pdd_info = {
        .count = 1,
        .sbr_type = BCMLRD_SBR_EXTERNAL,
        .info = bcm56880_a0_dna_4_6_6_dt_vfp_action_template_ing_obj_bus_vrf_pdd_info,
};

const bcmlrd_field_pdd_info_t bcm56880_a0_dna_4_6_6_dt_vfp_action_template_vfp_scratch_bus_svp_field_pdd_info = {
        .count = 1,
        .sbr_type = BCMLRD_SBR_NONE,
        .info = bcm56880_a0_dna_4_6_6_dt_vfp_action_template_vfp_scratch_bus_svp_pdd_info,
};

const bcmlrd_field_pdd_info_t bcm56880_a0_dna_4_6_6_dt_vfp_action_template_ing_cmd_bus_cml_flags_new_field_pdd_info = {
        .count = 1,
        .sbr_type = BCMLRD_SBR_EXTERNAL,
        .info = bcm56880_a0_dna_4_6_6_dt_vfp_action_template_ing_cmd_bus_cml_flags_new_pdd_info,
};

const bcmlrd_field_pdd_info_t bcm56880_a0_dna_4_6_6_dt_vfp_action_template_ing_cmd_bus_vfp_opaque_ctrl_id_field_pdd_info = {
        .count = 1,
        .sbr_type = BCMLRD_SBR_EXTERNAL,
        .info = bcm56880_a0_dna_4_6_6_dt_vfp_action_template_ing_cmd_bus_vfp_opaque_ctrl_id_pdd_info,
};

const bcmlrd_field_pdd_info_t bcm56880_a0_dna_4_6_6_dt_vfp_action_template_ing_cmd_bus_int_pri_field_pdd_info = {
        .count = 1,
        .sbr_type = BCMLRD_SBR_EXTERNAL,
        .info = bcm56880_a0_dna_4_6_6_dt_vfp_action_template_ing_cmd_bus_int_pri_pdd_info,
};

const bcmlrd_field_pdd_info_t bcm56880_a0_dna_4_6_6_dt_vfp_action_template_vfp_scratch_bus_my_station_1_hit_field_pdd_info = {
        .count = 1,
        .sbr_type = BCMLRD_SBR_INTERNAL,
        .info = bcm56880_a0_dna_4_6_6_dt_vfp_action_template_vfp_scratch_bus_my_station_1_hit_pdd_info,
};

const bcmlrd_field_pdd_info_t bcm56880_a0_dna_4_6_6_dt_vfp_action_template_ing_obj_bus_vfp_class_id_field_pdd_info = {
        .count = 1,
        .sbr_type = BCMLRD_SBR_EXTERNAL,
        .info = bcm56880_a0_dna_4_6_6_dt_vfp_action_template_ing_obj_bus_vfp_class_id_pdd_info,
};

const bcmlrd_field_pdd_info_t bcm56880_a0_dna_4_6_6_dt_vfp_action_template_ing_cmd_bus_vrf_strength_field_pdd_info = {
        .count = 1,
        .sbr_type = BCMLRD_SBR_EXTERNAL,
        .info = bcm56880_a0_dna_4_6_6_dt_vfp_action_template_ing_cmd_bus_vrf_strength_pdd_info,
};

const bcmlrd_field_pdd_info_t bcm56880_a0_dna_4_6_6_dt_vfp_action_template_ing_obj_bus_flow_group_id_field_pdd_info = {
        .count = 1,
        .sbr_type = BCMLRD_SBR_EXTERNAL,
        .info = bcm56880_a0_dna_4_6_6_dt_vfp_action_template_ing_obj_bus_flow_group_id_pdd_info,
};

const bcmlrd_field_pdd_info_t bcm56880_a0_dna_4_6_6_dt_vfp_action_template_vfp_scratch_bus_drop_action_field_pdd_info = {
        .count = 1,
        .sbr_type = BCMLRD_SBR_NONE,
        .info = bcm56880_a0_dna_4_6_6_dt_vfp_action_template_vfp_scratch_bus_drop_action_pdd_info,
};

const bcmlrd_field_pdd_info_t bcm56880_a0_dna_4_6_6_dt_vfp_action_template_vfp_scratch_bus_change_vfi_field_pdd_info = {
        .count = 1,
        .sbr_type = BCMLRD_SBR_NONE,
        .info = bcm56880_a0_dna_4_6_6_dt_vfp_action_template_vfp_scratch_bus_change_vfi_pdd_info,
};

const bcmlrd_field_pdd_info_t bcm56880_a0_dna_4_6_6_dt_vfp_action_template_vfp_scratch_bus_change_svp_field_pdd_info = {
        .count = 1,
        .sbr_type = BCMLRD_SBR_NONE,
        .info = bcm56880_a0_dna_4_6_6_dt_vfp_action_template_vfp_scratch_bus_change_svp_pdd_info,
};

const bcmlrd_field_pdd_info_t bcm56880_a0_dna_4_6_6_dt_vfp_action_template_vfp_scratch_bus_vfi_field_pdd_info = {
        .count = 1,
        .sbr_type = BCMLRD_SBR_NONE,
        .info = bcm56880_a0_dna_4_6_6_dt_vfp_action_template_vfp_scratch_bus_vfi_pdd_info,
};

const bcmlrd_field_pdd_info_t bcm56880_a0_dna_4_6_6_dt_vfp_action_template_vfp_scratch_bus_copy_to_cpu_action_field_pdd_info = {
        .count = 1,
        .sbr_type = BCMLRD_SBR_NONE,
        .info = bcm56880_a0_dna_4_6_6_dt_vfp_action_template_vfp_scratch_bus_copy_to_cpu_action_pdd_info,
};


const bcmlrd_field_info_t bcm56880_a0_dna_4_6_6_dt_vfp_action_template_field_info[] = {
    {
        .name = "vfp_scratch_bus_input_priority",
        .id = BCM56880_A0_DNA_4_6_6_DT_VFP_ACTION_TEMPLATEt_VFP_SCRATCH_BUS_INPUT_PRIORITYf,
        .cont_info = NULL,
        .pdd_info = &bcm56880_a0_dna_4_6_6_dt_vfp_action_template_vfp_scratch_bus_input_priority_field_pdd_info,
    },
    {
        .name = "ing_cmd_bus_cml_flags_move",
        .id = BCM56880_A0_DNA_4_6_6_DT_VFP_ACTION_TEMPLATEt_ING_CMD_BUS_CML_FLAGS_MOVEf,
        .cont_info = NULL,
        .pdd_info = &bcm56880_a0_dna_4_6_6_dt_vfp_action_template_ing_cmd_bus_cml_flags_move_field_pdd_info,
    },
    {
        .name = "ing_cmd_bus_cng",
        .id = BCM56880_A0_DNA_4_6_6_DT_VFP_ACTION_TEMPLATEt_ING_CMD_BUS_CNGf,
        .cont_info = NULL,
        .pdd_info = &bcm56880_a0_dna_4_6_6_dt_vfp_action_template_ing_cmd_bus_cng_field_pdd_info,
    },
    {
        .name = "vfp_scratch_bus_change_input_priority",
        .id = BCM56880_A0_DNA_4_6_6_DT_VFP_ACTION_TEMPLATEt_VFP_SCRATCH_BUS_CHANGE_INPUT_PRIORITYf,
        .cont_info = NULL,
        .pdd_info = &bcm56880_a0_dna_4_6_6_dt_vfp_action_template_vfp_scratch_bus_change_input_priority_field_pdd_info,
    },
    {
        .name = "ing_obj_bus_vfp_opaque_obj0",
        .id = BCM56880_A0_DNA_4_6_6_DT_VFP_ACTION_TEMPLATEt_ING_OBJ_BUS_VFP_OPAQUE_OBJ0f,
        .cont_info = NULL,
        .pdd_info = &bcm56880_a0_dna_4_6_6_dt_vfp_action_template_ing_obj_bus_vfp_opaque_obj0_field_pdd_info,
    },
    {
        .name = "ing_cmd_bus_int_cn",
        .id = BCM56880_A0_DNA_4_6_6_DT_VFP_ACTION_TEMPLATEt_ING_CMD_BUS_INT_CNf,
        .cont_info = NULL,
        .pdd_info = &bcm56880_a0_dna_4_6_6_dt_vfp_action_template_ing_cmd_bus_int_cn_field_pdd_info,
    },
    {
        .name = "ing_obj_bus_vrf",
        .id = BCM56880_A0_DNA_4_6_6_DT_VFP_ACTION_TEMPLATEt_ING_OBJ_BUS_VRFf,
        .cont_info = NULL,
        .pdd_info = &bcm56880_a0_dna_4_6_6_dt_vfp_action_template_ing_obj_bus_vrf_field_pdd_info,
    },
    {
        .name = "vfp_scratch_bus_svp",
        .id = BCM56880_A0_DNA_4_6_6_DT_VFP_ACTION_TEMPLATEt_VFP_SCRATCH_BUS_SVPf,
        .cont_info = NULL,
        .pdd_info = &bcm56880_a0_dna_4_6_6_dt_vfp_action_template_vfp_scratch_bus_svp_field_pdd_info,
    },
    {
        .name = "ing_cmd_bus_cml_flags_new",
        .id = BCM56880_A0_DNA_4_6_6_DT_VFP_ACTION_TEMPLATEt_ING_CMD_BUS_CML_FLAGS_NEWf,
        .cont_info = NULL,
        .pdd_info = &bcm56880_a0_dna_4_6_6_dt_vfp_action_template_ing_cmd_bus_cml_flags_new_field_pdd_info,
    },
    {
        .name = "ing_cmd_bus_vfp_opaque_ctrl_id",
        .id = BCM56880_A0_DNA_4_6_6_DT_VFP_ACTION_TEMPLATEt_ING_CMD_BUS_VFP_OPAQUE_CTRL_IDf,
        .cont_info = NULL,
        .pdd_info = &bcm56880_a0_dna_4_6_6_dt_vfp_action_template_ing_cmd_bus_vfp_opaque_ctrl_id_field_pdd_info,
    },
    {
        .name = "ing_cmd_bus_int_pri",
        .id = BCM56880_A0_DNA_4_6_6_DT_VFP_ACTION_TEMPLATEt_ING_CMD_BUS_INT_PRIf,
        .cont_info = NULL,
        .pdd_info = &bcm56880_a0_dna_4_6_6_dt_vfp_action_template_ing_cmd_bus_int_pri_field_pdd_info,
    },
    {
        .name = "vfp_scratch_bus_my_station_1_hit",
        .id = BCM56880_A0_DNA_4_6_6_DT_VFP_ACTION_TEMPLATEt_VFP_SCRATCH_BUS_MY_STATION_1_HITf,
        .cont_info = NULL,
        .pdd_info = &bcm56880_a0_dna_4_6_6_dt_vfp_action_template_vfp_scratch_bus_my_station_1_hit_field_pdd_info,
    },
    {
        .name = "ing_obj_bus_vfp_class_id",
        .id = BCM56880_A0_DNA_4_6_6_DT_VFP_ACTION_TEMPLATEt_ING_OBJ_BUS_VFP_CLASS_IDf,
        .cont_info = NULL,
        .pdd_info = &bcm56880_a0_dna_4_6_6_dt_vfp_action_template_ing_obj_bus_vfp_class_id_field_pdd_info,
    },
    {
        .name = "ing_cmd_bus_vrf_strength",
        .id = BCM56880_A0_DNA_4_6_6_DT_VFP_ACTION_TEMPLATEt_ING_CMD_BUS_VRF_STRENGTHf,
        .cont_info = NULL,
        .pdd_info = &bcm56880_a0_dna_4_6_6_dt_vfp_action_template_ing_cmd_bus_vrf_strength_field_pdd_info,
    },
    {
        .name = "ing_obj_bus_flow_group_id",
        .id = BCM56880_A0_DNA_4_6_6_DT_VFP_ACTION_TEMPLATEt_ING_OBJ_BUS_FLOW_GROUP_IDf,
        .cont_info = NULL,
        .pdd_info = &bcm56880_a0_dna_4_6_6_dt_vfp_action_template_ing_obj_bus_flow_group_id_field_pdd_info,
    },
    {
        .name = "vfp_scratch_bus_drop_action",
        .id = BCM56880_A0_DNA_4_6_6_DT_VFP_ACTION_TEMPLATEt_VFP_SCRATCH_BUS_DROP_ACTIONf,
        .cont_info = NULL,
        .pdd_info = &bcm56880_a0_dna_4_6_6_dt_vfp_action_template_vfp_scratch_bus_drop_action_field_pdd_info,
    },
    {
        .name = "vfp_scratch_bus_change_vfi",
        .id = BCM56880_A0_DNA_4_6_6_DT_VFP_ACTION_TEMPLATEt_VFP_SCRATCH_BUS_CHANGE_VFIf,
        .cont_info = NULL,
        .pdd_info = &bcm56880_a0_dna_4_6_6_dt_vfp_action_template_vfp_scratch_bus_change_vfi_field_pdd_info,
    },
    {
        .name = "vfp_scratch_bus_change_svp",
        .id = BCM56880_A0_DNA_4_6_6_DT_VFP_ACTION_TEMPLATEt_VFP_SCRATCH_BUS_CHANGE_SVPf,
        .cont_info = NULL,
        .pdd_info = &bcm56880_a0_dna_4_6_6_dt_vfp_action_template_vfp_scratch_bus_change_svp_field_pdd_info,
    },
    {
        .name = "vfp_scratch_bus_vfi",
        .id = BCM56880_A0_DNA_4_6_6_DT_VFP_ACTION_TEMPLATEt_VFP_SCRATCH_BUS_VFIf,
        .cont_info = NULL,
        .pdd_info = &bcm56880_a0_dna_4_6_6_dt_vfp_action_template_vfp_scratch_bus_vfi_field_pdd_info,
    },
    {
        .name = "vfp_scratch_bus_copy_to_cpu_action",
        .id = BCM56880_A0_DNA_4_6_6_DT_VFP_ACTION_TEMPLATEt_VFP_SCRATCH_BUS_COPY_TO_CPU_ACTIONf,
        .cont_info = NULL,
        .pdd_info = &bcm56880_a0_dna_4_6_6_dt_vfp_action_template_vfp_scratch_bus_copy_to_cpu_action_field_pdd_info,
    },
};


const bcmlrd_table_pcm_info_t bcm56880_a0_dna_4_6_6_dt_vfp_action_template_table_pcm_info = {
    .src_id = BCM56880_A0_DNA_4_6_6_DT_VFP_ACTION_TEMPLATEt,
    .field_count = 20,
    .field_info = bcm56880_a0_dna_4_6_6_dt_vfp_action_template_field_info,
    .tile_id = BCMLRD_BCM56880_A0_TILE_IFTA40,
};
