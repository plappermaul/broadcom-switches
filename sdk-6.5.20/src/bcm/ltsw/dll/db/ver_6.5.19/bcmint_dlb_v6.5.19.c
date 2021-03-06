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
#include "dlb_ha.h"

static const issu_field_t bcmissu_fields_bcmint_dlb_info_t[] = {
    {
        .fid = 0x4e5868d6,
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

static uint32_t bcmint_dlb_info_t_field_offset_get (bcmissu_field_id_t field_id)
{
    bcmint_dlb_info_t var;

    switch (field_id) {
        case 0x4e5868d6:
            return ((uint8_t *)&var.flags) - (uint8_t *)&var;
        default:
            return ISSU_INVALID_FLD_OFFSET;
    }
}

const issu_struct_t bcmissu_struct_dlb_ha_bcmint_dlb_info_t_v6_5_19 = {
    .offset_get_func = bcmint_dlb_info_t_field_offset_get,
    .generic_size = sizeof(bcmint_dlb_info_t),
    .field_count = 1,
    .fields = bcmissu_fields_bcmint_dlb_info_t,
};

static const issu_field_t bcmissu_fields_bcmint_dlb_ctrl_info_t[] = {
    {
        .fid = 0x9916510b,
        .width = sizeof(uint64_t),
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
        .fid = 0xeea0ee3c,
        .width = sizeof(uint64_t),
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
        .fid = 0x35abac98,
        .width = sizeof(uint64_t),
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
        .fid = 0x8f7ba461,
        .width = sizeof(uint64_t),
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
        .fid = 0xb7cf7509,
        .width = sizeof(uint64_t),
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
        .fid = 0x50d83d0d,
        .width = sizeof(uint64_t),
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
        .fid = 0xb1910f87,
        .width = sizeof(uint64_t),
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
        .fid = 0xe77dabb1,
        .width = sizeof(uint64_t),
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

static uint32_t bcmint_dlb_ctrl_info_t_field_offset_get (bcmissu_field_id_t field_id)
{
    bcmint_dlb_ctrl_info_t var;

    switch (field_id) {
        case 0x9916510b:
            return ((uint8_t *)&var.bytes_per_cell) - (uint8_t *)&var;
        case 0xeea0ee3c:
            return ((uint8_t *)&var.sample_rate) - (uint8_t *)&var;
        case 0x35abac98:
            return ((uint8_t *)&var.egress_bytes_min) - (uint8_t *)&var;
        case 0x8f7ba461:
            return ((uint8_t *)&var.egress_bytes_max) - (uint8_t *)&var;
        case 0xb7cf7509:
            return ((uint8_t *)&var.queued_bytes_min) - (uint8_t *)&var;
        case 0x50d83d0d:
            return ((uint8_t *)&var.queued_bytes_max) - (uint8_t *)&var;
        case 0xb1910f87:
            return ((uint8_t *)&var.physical_queued_bytes_min) - (uint8_t *)&var;
        case 0xe77dabb1:
            return ((uint8_t *)&var.physical_queued_bytes_max) - (uint8_t *)&var;
        default:
            return ISSU_INVALID_FLD_OFFSET;
    }
}

const issu_struct_t bcmissu_struct_dlb_ha_bcmint_dlb_ctrl_info_t_v6_5_19 = {
    .offset_get_func = bcmint_dlb_ctrl_info_t_field_offset_get,
    .generic_size = sizeof(bcmint_dlb_ctrl_info_t),
    .field_count = 8,
    .fields = bcmissu_fields_bcmint_dlb_ctrl_info_t,
};
