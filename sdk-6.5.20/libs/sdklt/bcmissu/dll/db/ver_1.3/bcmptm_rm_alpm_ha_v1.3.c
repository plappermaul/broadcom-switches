/*************************************************************************
 *
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated by HA parser from YAML formated file.
 * Edits to this file will be lost when it is regenerated.
 * Tool: bcmha/scripts/issu_db_gen.py
 *
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

#include <bcmissu/issu_types.h>
#include "bcmptm_rm_alpm_ha.h"

static const issu_field_t bcmissu_fields_bcmptm_rm_alpm_ha_t[] = {
    {
        .fid = 0xd6a2a1db,
        .width = sizeof(uint32_t),
        .ha_ptr = HA_PTR_NONE,
        .lt_attrib = HA_LT_NONE,
        .ltid_var_for_fid = ISSU_INVALID_FIELD_ID,
        .is_array = false,
        .var_size_id = ISSU_INVALID_FIELD_ID,
        .size = 0,
        .struct_id = ISSU_INVALID_STRUCT_ID,
        .enum_type_name = NULL,
    },
    {
        .fid = 0x4dfc1486,
        .width = sizeof(bcmptm_rm_alpm_trans_state_t),
        .ha_ptr = HA_PTR_NONE,
        .lt_attrib = HA_LT_NONE,
        .ltid_var_for_fid = ISSU_INVALID_FIELD_ID,
        .is_array = false,
        .var_size_id = ISSU_INVALID_FIELD_ID,
        .size = 0,
        .struct_id = ISSU_INVALID_STRUCT_ID,
        .enum_type_name = NULL,
    },
};

static uint32_t bcmptm_rm_alpm_ha_t_field_offset_get (bcmissu_field_id_t field_id)
{
    bcmptm_rm_alpm_ha_t var;

    switch (field_id) {
        case 0xd6a2a1db:
            return ((uint8_t *)&var.trans_id) - (uint8_t *)&var;
        case 0x4dfc1486:
            return ((uint8_t *)&var.trans_state) - (uint8_t *)&var;
        default:
            return ISSU_INVALID_FLD_OFFSET;
    }
}

const issu_struct_t bcmissu_struct_bcmptm_rm_alpm_ha_bcmptm_rm_alpm_ha_t_v1_3 = {
    .offset_get_func = bcmptm_rm_alpm_ha_t_field_offset_get,
    .generic_size = sizeof(bcmptm_rm_alpm_ha_t),
    .field_count = 2,
    .fields = bcmissu_fields_bcmptm_rm_alpm_ha_t,
};

static const issu_field_t bcmissu_fields_bcmptm_rm_alpm_l1_ha_t[] = {
    {
        .fid = 0xf7bf3571,
        .width = sizeof(int),
        .ha_ptr = HA_PTR_NONE,
        .lt_attrib = HA_LT_NONE,
        .ltid_var_for_fid = ISSU_INVALID_FIELD_ID,
        .is_array = true,
        .size = ALPM_PKMS,
        .var_size_id = ISSU_INVALID_FIELD_ID,
        .struct_id = ISSU_INVALID_STRUCT_ID,
        .enum_type_name = NULL,
    },
    {
        .fid = 0x3ebe5574,
        .width = sizeof(int),
        .ha_ptr = HA_PTR_NONE,
        .lt_attrib = HA_LT_NONE,
        .ltid_var_for_fid = ISSU_INVALID_FIELD_ID,
        .is_array = true,
        .size = ALPM_PKMS,
        .var_size_id = ISSU_INVALID_FIELD_ID,
        .struct_id = ISSU_INVALID_STRUCT_ID,
        .enum_type_name = NULL,
    },
    {
        .fid = 0x893604b6,
        .width = sizeof(int),
        .ha_ptr = HA_PTR_NONE,
        .lt_attrib = HA_LT_NONE,
        .ltid_var_for_fid = ISSU_INVALID_FIELD_ID,
        .is_array = true,
        .size = ALPM_PKMS,
        .var_size_id = ISSU_INVALID_FIELD_ID,
        .struct_id = ISSU_INVALID_STRUCT_ID,
        .enum_type_name = NULL,
    },
    {
        .fid = 0x8ca89e96,
        .width = sizeof(int),
        .ha_ptr = HA_PTR_NONE,
        .lt_attrib = HA_LT_NONE,
        .ltid_var_for_fid = ISSU_INVALID_FIELD_ID,
        .is_array = true,
        .size = ALPM_PKMS,
        .var_size_id = ISSU_INVALID_FIELD_ID,
        .struct_id = ISSU_INVALID_STRUCT_ID,
        .enum_type_name = NULL,
    },
};

static uint32_t bcmptm_rm_alpm_l1_ha_t_field_offset_get (bcmissu_field_id_t field_id)
{
    bcmptm_rm_alpm_l1_ha_t var;

    switch (field_id) {
        case 0xf7bf3571:
            return ((uint8_t *)&var.range1_first_db0) - (uint8_t *)&var;
        case 0x3ebe5574:
            return ((uint8_t *)&var.range1_first_db1) - (uint8_t *)&var;
        case 0x893604b6:
            return ((uint8_t *)&var.range1_first_db2) - (uint8_t *)&var;
        case 0x8ca89e96:
            return ((uint8_t *)&var.range1_first_db3) - (uint8_t *)&var;
        default:
            return ISSU_INVALID_FLD_OFFSET;
    }
}

const issu_struct_t bcmissu_struct_bcmptm_rm_alpm_ha_bcmptm_rm_alpm_l1_ha_t_v1_3 = {
    .offset_get_func = bcmptm_rm_alpm_l1_ha_t_field_offset_get,
    .generic_size = sizeof(bcmptm_rm_alpm_l1_ha_t),
    .field_count = 4,
    .fields = bcmissu_fields_bcmptm_rm_alpm_l1_ha_t,
};

static const issu_field_t bcmissu_fields_bcmptm_rm_alpm_ln_stat_t[] = {
    {
        .fid = 0x40f00f44,
        .width = sizeof(uint32_t),
        .ha_ptr = HA_PTR_NONE,
        .lt_attrib = HA_LT_NONE,
        .ltid_var_for_fid = ISSU_INVALID_FIELD_ID,
        .is_array = false,
        .var_size_id = ISSU_INVALID_FIELD_ID,
        .size = 0,
        .struct_id = ISSU_INVALID_STRUCT_ID,
        .enum_type_name = NULL,
    },
    {
        .fid = 0xbb72bbf9,
        .width = sizeof(uint32_t),
        .ha_ptr = HA_PTR_NONE,
        .lt_attrib = HA_LT_NONE,
        .ltid_var_for_fid = ISSU_INVALID_FIELD_ID,
        .is_array = false,
        .var_size_id = ISSU_INVALID_FIELD_ID,
        .size = 0,
        .struct_id = ISSU_INVALID_STRUCT_ID,
        .enum_type_name = NULL,
    },
    {
        .fid = 0x3176e0f7,
        .width = sizeof(uint32_t),
        .ha_ptr = HA_PTR_NONE,
        .lt_attrib = HA_LT_NONE,
        .ltid_var_for_fid = ISSU_INVALID_FIELD_ID,
        .is_array = false,
        .var_size_id = ISSU_INVALID_FIELD_ID,
        .size = 0,
        .struct_id = ISSU_INVALID_STRUCT_ID,
        .enum_type_name = NULL,
    },
    {
        .fid = 0xde86f1a7,
        .width = sizeof(uint32_t),
        .ha_ptr = HA_PTR_NONE,
        .lt_attrib = HA_LT_NONE,
        .ltid_var_for_fid = ISSU_INVALID_FIELD_ID,
        .is_array = false,
        .var_size_id = ISSU_INVALID_FIELD_ID,
        .size = 0,
        .struct_id = ISSU_INVALID_STRUCT_ID,
        .enum_type_name = NULL,
    },
    {
        .fid = 0x39ddf2da,
        .width = sizeof(uint32_t),
        .ha_ptr = HA_PTR_NONE,
        .lt_attrib = HA_LT_NONE,
        .ltid_var_for_fid = ISSU_INVALID_FIELD_ID,
        .is_array = false,
        .var_size_id = ISSU_INVALID_FIELD_ID,
        .size = 0,
        .struct_id = ISSU_INVALID_STRUCT_ID,
        .enum_type_name = NULL,
    },
    {
        .fid = 0x090599f2,
        .width = sizeof(uint32_t),
        .ha_ptr = HA_PTR_NONE,
        .lt_attrib = HA_LT_NONE,
        .ltid_var_for_fid = ISSU_INVALID_FIELD_ID,
        .is_array = false,
        .var_size_id = ISSU_INVALID_FIELD_ID,
        .size = 0,
        .struct_id = ISSU_INVALID_STRUCT_ID,
        .enum_type_name = NULL,
    },
    {
        .fid = 0x85e69700,
        .width = sizeof(uint32_t),
        .ha_ptr = HA_PTR_NONE,
        .lt_attrib = HA_LT_NONE,
        .ltid_var_for_fid = ISSU_INVALID_FIELD_ID,
        .is_array = false,
        .var_size_id = ISSU_INVALID_FIELD_ID,
        .size = 0,
        .struct_id = ISSU_INVALID_STRUCT_ID,
        .enum_type_name = NULL,
    },
    {
        .fid = 0x3e3a78db,
        .width = sizeof(uint32_t),
        .ha_ptr = HA_PTR_NONE,
        .lt_attrib = HA_LT_NONE,
        .ltid_var_for_fid = ISSU_INVALID_FIELD_ID,
        .is_array = false,
        .var_size_id = ISSU_INVALID_FIELD_ID,
        .size = 0,
        .struct_id = ISSU_INVALID_STRUCT_ID,
        .enum_type_name = NULL,
    },
    {
        .fid = 0x039b217a,
        .width = sizeof(uint32_t),
        .ha_ptr = HA_PTR_NONE,
        .lt_attrib = HA_LT_NONE,
        .ltid_var_for_fid = ISSU_INVALID_FIELD_ID,
        .is_array = false,
        .var_size_id = ISSU_INVALID_FIELD_ID,
        .size = 0,
        .struct_id = ISSU_INVALID_STRUCT_ID,
        .enum_type_name = NULL,
    },
};

static uint32_t bcmptm_rm_alpm_ln_stat_t_field_offset_get (bcmissu_field_id_t field_id)
{
    bcmptm_rm_alpm_ln_stat_t var;

    switch (field_id) {
        case 0x40f00f44:
            return ((uint8_t *)&var.c_insert) - (uint8_t *)&var;
        case 0xbb72bbf9:
            return ((uint8_t *)&var.c_update_dt) - (uint8_t *)&var;
        case 0x3176e0f7:
            return ((uint8_t *)&var.c_update) - (uint8_t *)&var;
        case 0xde86f1a7:
            return ((uint8_t *)&var.c_delete) - (uint8_t *)&var;
        case 0x39ddf2da:
            return ((uint8_t *)&var.c_peak) - (uint8_t *)&var;
        case 0x090599f2:
            return ((uint8_t *)&var.c_last_peak) - (uint8_t *)&var;
        case 0x85e69700:
            return ((uint8_t *)&var.c_last_full) - (uint8_t *)&var;
        case 0x3e3a78db:
            return ((uint8_t *)&var.c_curr) - (uint8_t *)&var;
        case 0x039b217a:
            return ((uint8_t *)&var.c_traverse_merge) - (uint8_t *)&var;
        default:
            return ISSU_INVALID_FLD_OFFSET;
    }
}

const issu_struct_t bcmissu_struct_bcmptm_rm_alpm_ha_bcmptm_rm_alpm_ln_stat_t_v1_3 = {
    .offset_get_func = bcmptm_rm_alpm_ln_stat_t_field_offset_get,
    .generic_size = sizeof(bcmptm_rm_alpm_ln_stat_t),
    .field_count = 9,
    .fields = bcmissu_fields_bcmptm_rm_alpm_ln_stat_t,
};

static const issu_field_t bcmissu_fields_bcmptm_rm_alpm_ln_ha_t[] = {
    {
        .fid = 0x57408384,
        .width = sizeof(bcmptm_rm_alpm_ln_stat_t),
        .ha_ptr = HA_PTR_NONE,
        .lt_attrib = HA_LT_NONE,
        .ltid_var_for_fid = ISSU_INVALID_FIELD_ID,
        .is_array = false,
        .var_size_id = ISSU_INVALID_FIELD_ID,
        .size = 0,
        .struct_id = 0xe13e959c1bde179e,
        .enum_type_name = NULL,
    },
    {
        .fid = 0xc0fbc83a,
        .width = sizeof(bcmptm_rm_alpm_ln_stat_t),
        .ha_ptr = HA_PTR_NONE,
        .lt_attrib = HA_LT_NONE,
        .ltid_var_for_fid = ISSU_INVALID_FIELD_ID,
        .is_array = false,
        .var_size_id = ISSU_INVALID_FIELD_ID,
        .size = 0,
        .struct_id = 0xe13e959c1bde179e,
        .enum_type_name = NULL,
    },
    {
        .fid = 0x6b91b3cb,
        .width = sizeof(bcmptm_rm_alpm_ln_stat_t),
        .ha_ptr = HA_PTR_NONE,
        .lt_attrib = HA_LT_NONE,
        .ltid_var_for_fid = ISSU_INVALID_FIELD_ID,
        .is_array = false,
        .var_size_id = ISSU_INVALID_FIELD_ID,
        .size = 0,
        .struct_id = 0xe13e959c1bde179e,
        .enum_type_name = NULL,
    },
    {
        .fid = 0x869cbef8,
        .width = sizeof(bcmptm_rm_alpm_ln_stat_t),
        .ha_ptr = HA_PTR_NONE,
        .lt_attrib = HA_LT_NONE,
        .ltid_var_for_fid = ISSU_INVALID_FIELD_ID,
        .is_array = false,
        .var_size_id = ISSU_INVALID_FIELD_ID,
        .size = 0,
        .struct_id = 0xe13e959c1bde179e,
        .enum_type_name = NULL,
    },
};

static uint32_t bcmptm_rm_alpm_ln_ha_t_field_offset_get (bcmissu_field_id_t field_id)
{
    bcmptm_rm_alpm_ln_ha_t var;

    switch (field_id) {
        case 0x57408384:
            return ((uint8_t *)&var.db0_stats) - (uint8_t *)&var;
        case 0xc0fbc83a:
            return ((uint8_t *)&var.db1_stats) - (uint8_t *)&var;
        case 0x6b91b3cb:
            return ((uint8_t *)&var.db2_stats) - (uint8_t *)&var;
        case 0x869cbef8:
            return ((uint8_t *)&var.db3_stats) - (uint8_t *)&var;
        default:
            return ISSU_INVALID_FLD_OFFSET;
    }
}

const issu_struct_t bcmissu_struct_bcmptm_rm_alpm_ha_bcmptm_rm_alpm_ln_ha_t_v1_3 = {
    .offset_get_func = bcmptm_rm_alpm_ln_ha_t_field_offset_get,
    .generic_size = sizeof(bcmptm_rm_alpm_ln_ha_t),
    .field_count = 4,
    .fields = bcmissu_fields_bcmptm_rm_alpm_ln_ha_t,
};
