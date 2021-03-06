/*******************************************************************************
 *
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated by fltg from
 *    INTERNAL/fltg/xgs/l3/bcm56780_a0/bcm56780_a0_L3_ALPM_CONTROL.map.ltl for
 *      bcm56780_a0 variant dna_2_4_13
 *
 * Tool: $SDK/INTERNAL/fltg/bin/fltg
 *
 * Edits to this file will be lost when it is regenerated.
 *
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */
#include <bcmlrd/bcmlrd_internal.h>
#include <bcmlrd/chip/bcmlrd_id.h>
#include <bcmlrd/chip/bcm56780_a0/dna_2_4_13/bcm56780_a0_dna_2_4_13_lrd_field_data.h>
#include <bcmlrd/chip/bcm56780_a0/bcm56780_a0_lrd_ltm_intf.h>
#include <bcmlrd/chip/bcm56780_a0/dna_2_4_13/bcm56780_a0_dna_2_4_13_lrd_ltm_intf.h>
#include <bcmlrd/chip/bcm56780_a0/dna_2_4_13/bcm56780_a0_dna_2_4_13_lrd_xfrm_field_desc.h>
#include <bcmdrd/chip/bcm56780_a0_enum.h>
#include "bcmltd/chip/bcmltd_common_enumpool.h"
#include "../bcm56780_a0_lrd_enumpool.h"
#include "bcm56780_a0_dna_2_4_13_lrd_enumpool.h"
#include <bcmltd/bcmltd_handler.h>
/* L3_ALPM_CONTROL field init */
static const bcmlrd_field_data_t bcm56780_a0_dna_2_4_13_lrd_l3_alpm_control_map_field_data_mmd[] = {
    { /* 0 OPERATIONAL_STATE */
      .flags = BCMLRD_FIELD_F_READ_ONLY | BCMLTD_FIELD_F_ENUM,
      .min = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x0,
      .def = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x0,
      .max = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x2,
      .depth = 0,
      .width = 2,
      .edata = BCM56780_A0_LRD_ALPM_CONTROL_STATE_T_DATA,
    },
    { /* 1 ALPM_MODE */
      .flags = BCMLRD_FIELD_F_UNMAPPED,
      .min = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x0,
      .def = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x0,
      .max = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x0,
      .depth = 0,
      .width = 0,
      .edata = NULL,
    },
    { /* 2 NUM_DB_0_LEVELS */
      .flags = 0,
      .min = &bcm56780_a0_dna_2_4_13_lrd_ifd_u8_0x0,
      .def = &bcm56780_a0_dna_2_4_13_lrd_ifd_u8_0x0,
      .max = &bcm56780_a0_dna_2_4_13_lrd_ifd_u8_0x3,
      .depth = 0,
      .width = 2,
      .edata = NULL,
    },
    { /* 3 NUM_DB_1_LEVELS */
      .flags = 0,
      .min = &bcm56780_a0_dna_2_4_13_lrd_ifd_u8_0x0,
      .def = &bcm56780_a0_dna_2_4_13_lrd_ifd_u8_0x0,
      .max = &bcm56780_a0_dna_2_4_13_lrd_ifd_u8_0x3,
      .depth = 0,
      .width = 2,
      .edata = NULL,
    },
    { /* 4 NUM_DB_2_LEVELS */
      .flags = 0,
      .min = &bcm56780_a0_dna_2_4_13_lrd_ifd_u8_0x0,
      .def = &bcm56780_a0_dna_2_4_13_lrd_ifd_u8_0x0,
      .max = &bcm56780_a0_dna_2_4_13_lrd_ifd_u8_0x3,
      .depth = 0,
      .width = 2,
      .edata = NULL,
    },
    { /* 5 NUM_DB_3_LEVELS */
      .flags = 0,
      .min = &bcm56780_a0_dna_2_4_13_lrd_ifd_u8_0x0,
      .def = &bcm56780_a0_dna_2_4_13_lrd_ifd_u8_0x0,
      .max = &bcm56780_a0_dna_2_4_13_lrd_ifd_u8_0x3,
      .depth = 0,
      .width = 2,
      .edata = NULL,
    },
    { /* 6 KEY_INPUT_LEVEL_1_BLOCK_0 */
      .flags = BCMLTD_FIELD_F_ENUM,
      .min = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x0,
      .def = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x0,
      .max = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x7,
      .depth = 0,
      .width = 3,
      .edata = BCM56780_A0_LRD_ALPM_KEY_INPUT_T_DATA,
    },
    { /* 7 KEY_INPUT_LEVEL_1_BLOCK_1 */
      .flags = BCMLTD_FIELD_F_ENUM,
      .min = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x0,
      .def = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x0,
      .max = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x7,
      .depth = 0,
      .width = 3,
      .edata = BCM56780_A0_LRD_ALPM_KEY_INPUT_T_DATA,
    },
    { /* 8 KEY_INPUT_LEVEL_1_BLOCK_2 */
      .flags = BCMLTD_FIELD_F_ENUM,
      .min = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x0,
      .def = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x0,
      .max = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x7,
      .depth = 0,
      .width = 3,
      .edata = BCM56780_A0_LRD_ALPM_KEY_INPUT_T_DATA,
    },
    { /* 9 KEY_INPUT_LEVEL_1_BLOCK_3 */
      .flags = BCMLTD_FIELD_F_ENUM,
      .min = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x0,
      .def = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x0,
      .max = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x7,
      .depth = 0,
      .width = 3,
      .edata = BCM56780_A0_LRD_ALPM_KEY_INPUT_T_DATA,
    },
    { /* 10 KEY_INPUT_LEVEL_1_BLOCK_4 */
      .flags = BCMLRD_FIELD_F_UNMAPPED,
      .min = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x0,
      .def = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x0,
      .max = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x0,
      .depth = 0,
      .width = 0,
      .edata = BCM56780_A0_LRD_ALPM_KEY_INPUT_T_DATA,
    },
    { /* 11 KEY_INPUT_LEVEL_1_BLOCK_5 */
      .flags = BCMLRD_FIELD_F_UNMAPPED,
      .min = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x0,
      .def = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x0,
      .max = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x0,
      .depth = 0,
      .width = 0,
      .edata = BCM56780_A0_LRD_ALPM_KEY_INPUT_T_DATA,
    },
    { /* 12 KEY_INPUT_LEVEL_1_BLOCK_6 */
      .flags = BCMLRD_FIELD_F_UNMAPPED,
      .min = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x0,
      .def = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x0,
      .max = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x0,
      .depth = 0,
      .width = 0,
      .edata = BCM56780_A0_LRD_ALPM_KEY_INPUT_T_DATA,
    },
    { /* 13 KEY_INPUT_LEVEL_1_BLOCK_7 */
      .flags = BCMLRD_FIELD_F_UNMAPPED,
      .min = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x0,
      .def = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x0,
      .max = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x0,
      .depth = 0,
      .width = 0,
      .edata = BCM56780_A0_LRD_ALPM_KEY_INPUT_T_DATA,
    },
    { /* 14 KEY_INPUT_LEVEL_1_BLOCK_8 */
      .flags = BCMLRD_FIELD_F_UNMAPPED,
      .min = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x0,
      .def = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x0,
      .max = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x0,
      .depth = 0,
      .width = 0,
      .edata = BCM56780_A0_LRD_ALPM_KEY_INPUT_T_DATA,
    },
    { /* 15 KEY_INPUT_LEVEL_1_BLOCK_9 */
      .flags = BCMLRD_FIELD_F_UNMAPPED,
      .min = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x0,
      .def = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x0,
      .max = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x0,
      .depth = 0,
      .width = 0,
      .edata = BCM56780_A0_LRD_ALPM_KEY_INPUT_T_DATA,
    },
    { /* 16 DB_LEVEL_1_BLOCK_0 */
      .flags = BCMLTD_FIELD_F_ENUM,
      .min = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x0,
      .def = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x0,
      .max = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x3,
      .depth = 0,
      .width = 2,
      .edata = BCM56780_A0_LRD_ALPM_DB_T_DATA,
    },
    { /* 17 DB_LEVEL_1_BLOCK_1 */
      .flags = BCMLTD_FIELD_F_ENUM,
      .min = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x0,
      .def = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x0,
      .max = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x3,
      .depth = 0,
      .width = 2,
      .edata = BCM56780_A0_LRD_ALPM_DB_T_DATA,
    },
    { /* 18 DB_LEVEL_1_BLOCK_2 */
      .flags = BCMLTD_FIELD_F_ENUM,
      .min = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x0,
      .def = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x0,
      .max = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x3,
      .depth = 0,
      .width = 2,
      .edata = BCM56780_A0_LRD_ALPM_DB_T_DATA,
    },
    { /* 19 DB_LEVEL_1_BLOCK_3 */
      .flags = BCMLTD_FIELD_F_ENUM,
      .min = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x0,
      .def = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x0,
      .max = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x3,
      .depth = 0,
      .width = 2,
      .edata = BCM56780_A0_LRD_ALPM_DB_T_DATA,
    },
    { /* 20 DB_LEVEL_1_BLOCK_4 */
      .flags = BCMLRD_FIELD_F_UNMAPPED,
      .min = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x0,
      .def = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x0,
      .max = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x0,
      .depth = 0,
      .width = 0,
      .edata = BCM56780_A0_LRD_ALPM_DB_T_DATA,
    },
    { /* 21 DB_LEVEL_1_BLOCK_5 */
      .flags = BCMLRD_FIELD_F_UNMAPPED,
      .min = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x0,
      .def = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x0,
      .max = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x0,
      .depth = 0,
      .width = 0,
      .edata = BCM56780_A0_LRD_ALPM_DB_T_DATA,
    },
    { /* 22 DB_LEVEL_1_BLOCK_6 */
      .flags = BCMLRD_FIELD_F_UNMAPPED,
      .min = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x0,
      .def = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x0,
      .max = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x0,
      .depth = 0,
      .width = 0,
      .edata = BCM56780_A0_LRD_ALPM_DB_T_DATA,
    },
    { /* 23 DB_LEVEL_1_BLOCK_7 */
      .flags = BCMLRD_FIELD_F_UNMAPPED,
      .min = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x0,
      .def = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x0,
      .max = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x0,
      .depth = 0,
      .width = 0,
      .edata = BCM56780_A0_LRD_ALPM_DB_T_DATA,
    },
    { /* 24 DB_LEVEL_1_BLOCK_8 */
      .flags = BCMLRD_FIELD_F_UNMAPPED,
      .min = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x0,
      .def = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x0,
      .max = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x0,
      .depth = 0,
      .width = 0,
      .edata = BCM56780_A0_LRD_ALPM_DB_T_DATA,
    },
    { /* 25 DB_LEVEL_1_BLOCK_9 */
      .flags = BCMLRD_FIELD_F_UNMAPPED,
      .min = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x0,
      .def = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x0,
      .max = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x0,
      .depth = 0,
      .width = 0,
      .edata = BCM56780_A0_LRD_ALPM_DB_T_DATA,
    },
    { /* 26 HIT_MODE */
      .flags = BCMLRD_FIELD_F_UNMAPPED,
      .min = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x0,
      .def = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x0,
      .max = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x0,
      .depth = 0,
      .width = 0,
      .edata = NULL,
    },
    { /* 27 DESTINATION */
      .flags = 0,
      .min = &bcm56780_a0_dna_2_4_13_lrd_ifd_u16_0x0,
      .def = &bcm56780_a0_dna_2_4_13_lrd_ifd_u16_0x0,
      .max = &bcm56780_a0_dna_2_4_13_lrd_ifd_u16_0xffff,
      .depth = 0,
      .width = 16,
      .edata = NULL,
    },
    { /* 28 DESTINATION_MASK */
      .flags = 0,
      .min = &bcm56780_a0_dna_2_4_13_lrd_ifd_u16_0x0,
      .def = &bcm56780_a0_dna_2_4_13_lrd_ifd_u16_0x0,
      .max = &bcm56780_a0_dna_2_4_13_lrd_ifd_u16_0xffff,
      .depth = 0,
      .width = 16,
      .edata = NULL,
    },
    { /* 29 DESTINATION_TYPE_MATCH */
      .flags = BCMLTD_FIELD_F_ENUM,
      .min = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x0,
      .def = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x0,
      .max = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0xa,
      .depth = 0,
      .width = 4,
      .edata = BCM56780_A0_DNA_2_4_13_LRD_DESTINATION_TYPE_DATA,
    },
    { /* 30 DESTINATION_TYPE_NON_MATCH */
      .flags = BCMLTD_FIELD_F_ENUM,
      .min = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x0,
      .def = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x0,
      .max = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0xa,
      .depth = 0,
      .width = 4,
      .edata = BCM56780_A0_DNA_2_4_13_LRD_DESTINATION_TYPE_DATA,
    },
    { /* 31 IPV4_UC_STRENGTH_PROFILE_INDEX */
      .flags = 0,
      .min = &bcm56780_a0_dna_2_4_13_lrd_ifd_u8_0x0,
      .def = &bcm56780_a0_dna_2_4_13_lrd_ifd_u8_0x0,
      .max = &bcm56780_a0_dna_2_4_13_lrd_ifd_u8_0xf,
      .depth = 0,
      .width = 4,
      .edata = NULL,
    },
    { /* 32 IPV4_UC_VRF_STRENGTH_PROFILE_INDEX */
      .flags = 0,
      .min = &bcm56780_a0_dna_2_4_13_lrd_ifd_u8_0x0,
      .def = &bcm56780_a0_dna_2_4_13_lrd_ifd_u8_0x0,
      .max = &bcm56780_a0_dna_2_4_13_lrd_ifd_u8_0xf,
      .depth = 0,
      .width = 4,
      .edata = NULL,
    },
    { /* 33 IPV4_UC_OVERRIDE_STRENGTH_PROFILE_INDEX */
      .flags = 0,
      .min = &bcm56780_a0_dna_2_4_13_lrd_ifd_u8_0x0,
      .def = &bcm56780_a0_dna_2_4_13_lrd_ifd_u8_0x0,
      .max = &bcm56780_a0_dna_2_4_13_lrd_ifd_u8_0xf,
      .depth = 0,
      .width = 4,
      .edata = NULL,
    },
    { /* 34 IPV6_UC_STRENGTH_PROFILE_INDEX */
      .flags = 0,
      .min = &bcm56780_a0_dna_2_4_13_lrd_ifd_u8_0x0,
      .def = &bcm56780_a0_dna_2_4_13_lrd_ifd_u8_0x0,
      .max = &bcm56780_a0_dna_2_4_13_lrd_ifd_u8_0xf,
      .depth = 0,
      .width = 4,
      .edata = NULL,
    },
    { /* 35 IPV6_UC_VRF_STRENGTH_PROFILE_INDEX */
      .flags = 0,
      .min = &bcm56780_a0_dna_2_4_13_lrd_ifd_u8_0x0,
      .def = &bcm56780_a0_dna_2_4_13_lrd_ifd_u8_0x0,
      .max = &bcm56780_a0_dna_2_4_13_lrd_ifd_u8_0xf,
      .depth = 0,
      .width = 4,
      .edata = NULL,
    },
    { /* 36 IPV6_UC_OVERRIDE_STRENGTH_PROFILE_INDEX */
      .flags = 0,
      .min = &bcm56780_a0_dna_2_4_13_lrd_ifd_u8_0x0,
      .def = &bcm56780_a0_dna_2_4_13_lrd_ifd_u8_0x0,
      .max = &bcm56780_a0_dna_2_4_13_lrd_ifd_u8_0xf,
      .depth = 0,
      .width = 4,
      .edata = NULL,
    },
    { /* 37 IPV4_COMPRESSION_STRENGTH_PROFILE_INDEX */
      .flags = 0,
      .min = &bcm56780_a0_dna_2_4_13_lrd_ifd_u8_0x0,
      .def = &bcm56780_a0_dna_2_4_13_lrd_ifd_u8_0x0,
      .max = &bcm56780_a0_dna_2_4_13_lrd_ifd_u8_0xf,
      .depth = 0,
      .width = 4,
      .edata = NULL,
    },
    { /* 38 IPV6_COMPRESSION_STRENGTH_PROFILE_INDEX */
      .flags = 0,
      .min = &bcm56780_a0_dna_2_4_13_lrd_ifd_u8_0x0,
      .def = &bcm56780_a0_dna_2_4_13_lrd_ifd_u8_0x0,
      .max = &bcm56780_a0_dna_2_4_13_lrd_ifd_u8_0xf,
      .depth = 0,
      .width = 4,
      .edata = NULL,
    },
    { /* 39 LARGE_VRF */
      .flags = BCMLRD_FIELD_F_UNMAPPED,
      .min = &bcm56780_a0_dna_2_4_13_lrd_ifd_is_true_0x0,
      .def = &bcm56780_a0_dna_2_4_13_lrd_ifd_is_true_0x0,
      .max = &bcm56780_a0_dna_2_4_13_lrd_ifd_is_true_0x0,
      .depth = 0,
      .width = 0,
      .edata = NULL,
    },
    { /* 40 COMP_KEY_TYPE */
      .flags = BCMLRD_FIELD_F_UNMAPPED,
      .min = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x0,
      .def = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x0,
      .max = &bcm56780_a0_dna_2_4_13_lrd_ifd_u32_0x0,
      .depth = 0,
      .width = 0,
      .edata = NULL,
    },
};
const bcmlrd_map_field_data_t bcm56780_a0_dna_2_4_13_lrd_l3_alpm_control_map_field_data = {
    .fields = 41,
    .field = bcm56780_a0_dna_2_4_13_lrd_l3_alpm_control_map_field_data_mmd
};

static const bcmlrd_map_table_attr_t bcm56780_a0_dna_2_4_13_lrd_l3_alpm_controlt_attr_entry[] = {
    { /* 0 */
        .key   = BCMLRD_MAP_TABLE_ATTRIBUTE_ENTRY_LIMIT,
        .value = 1,
    },
    { /* 1 */
        .key   = BCMLRD_MAP_TABLE_ATTRIBUTE_INTERACTIVE,
        .value = FALSE,
    },
    { /* 2 */
        .key   = BCMLRD_MAP_TABLE_ATTRIBUTE_TABLE_MAX_INDEX,
        .value = 0,
    },
    { /* 3 */
        .key   = BCMLRD_MAP_TABLE_ATTRIBUTE_TABLE_MIN_INDEX,
        .value = 0,
    },
};

static const bcmlrd_map_attr_t bcm56780_a0_dna_2_4_13_lrd_l3_alpm_controlt_attr_group = {
    .attributes = 4,
    .attr = bcm56780_a0_dna_2_4_13_lrd_l3_alpm_controlt_attr_entry,
};

static const bcmlrd_map_entry_t bcm56780_a0_dna_2_4_13_lrd_bcmimm_l3_alpm_control_entry[] = {
    { /* 0 */
        .entry_type = BCMLRD_MAP_ENTRY_TABLE_HANDLER,
        .desc = {
            .field_id  = 0,
            .field_idx = 0,
            .minbit    = 0,
            .maxbit    = 0,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            /* handler: bcm56780_a0_dna_2_4_13_lta_bcmimm_l3_alpm_control_cth_handler */
            .handler_id = BCMLTD_TABLE_BCM56780_A0_DNA_2_4_13_LTA_BCMIMM_L3_ALPM_CONTROL_CTH_HANDLER_ID
        }
    },
};
static const bcmlrd_map_entry_t
bcm56780_a0_dna_2_4_13_lrd_bcmltx_l3_alpm_control_validate_entry[] = {
    { /* 0 */
        .entry_type = BCMLRD_MAP_ENTRY_VALUE_FIELD_VALIDATION,
        .desc = {
            .field_id  = 0,
            .field_idx = 0,
            .minbit    = 0,
            .maxbit    = 0,
            .entry_idx = 0,
            .reserved  = 0
        },
        .u = {
            /* handler: bcm56780_a0_dna_2_4_13_lta_bcmltx_l3_alpm_control_std_val_fv_handler */
            .handler_id = BCMLTD_VALIDATE_BCM56780_A0_DNA_2_4_13_LTA_BCMLTX_L3_ALPM_CONTROL_STD_VAL_FV_HANDLER_ID
        }
    },
};
static const bcmlrd_map_group_t bcm56780_a0_dna_2_4_13_lrd_l3_alpm_control_map_group[] = {
    {
        .dest = {
            .kind = BCMLRD_MAP_CUSTOM,
            .id = 0,
        },
        .entries = 1,
        .entry = bcm56780_a0_dna_2_4_13_lrd_bcmimm_l3_alpm_control_entry
    },
    {
        .dest = {
            .kind = BCMLRD_MAP_VALIDATION,
            .id = 0,
        },
        .entries = 1,
        .entry = bcm56780_a0_dna_2_4_13_lrd_bcmltx_l3_alpm_control_validate_entry
    },
};
const bcmlrd_map_t bcm56780_a0_dna_2_4_13_lrd_l3_alpm_control_map = {
    .src_id = L3_ALPM_CONTROLt,
    .field_data = &bcm56780_a0_dna_2_4_13_lrd_l3_alpm_control_map_field_data,
    .groups = 2,
    .group  = bcm56780_a0_dna_2_4_13_lrd_l3_alpm_control_map_group,
    .table_attr = &bcm56780_a0_dna_2_4_13_lrd_l3_alpm_controlt_attr_group,
    .entry_ops = BCMLRD_MAP_TABLE_ENTRY_OPERATION_UPDATE | BCMLRD_MAP_TABLE_ENTRY_OPERATION_LOOKUP | BCMLRD_MAP_TABLE_ENTRY_OPERATION_TRAVERSE
};
