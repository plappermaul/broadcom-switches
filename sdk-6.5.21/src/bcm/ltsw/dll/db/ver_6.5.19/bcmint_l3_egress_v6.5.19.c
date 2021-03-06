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
#include "l3_egress_ha.h"

static const issu_field_t bcmissu_fields_bcmint_l3_ecmp_info_t[] = {
    {
        .fid = 0x43ee93fd,
        .width = sizeof(int),
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

static uint32_t bcmint_l3_ecmp_info_t_field_offset_get (bcmissu_field_id_t field_id)
{
    bcmint_l3_ecmp_info_t var;

    switch (field_id) {
        case 0x43ee93fd:
            return ((uint8_t *)&var.max_paths) - (uint8_t *)&var;
        default:
            return ISSU_INVALID_FLD_OFFSET;
    }
}

const issu_struct_t bcmissu_struct_l3_egress_ha_bcmint_l3_ecmp_info_t_v6_5_19 = {
    .offset_get_func = bcmint_l3_ecmp_info_t_field_offset_get,
    .generic_size = sizeof(bcmint_l3_ecmp_info_t),
    .field_count = 1,
    .fields = bcmissu_fields_bcmint_l3_ecmp_info_t,
};

static const issu_field_t bcmissu_fields_bcmint_l3_ecmp_member_ent_t[] = {
    {
        .fid = 0x599dcce2,
        .width = sizeof(int),
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
        .fid = 0x633da6e1,
        .width = sizeof(int),
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
        .fid = 0x3185376b,
        .width = sizeof(int),
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
        .fid = 0x1aeb891a,
        .width = sizeof(int),
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
        .fid = 0xe80253a7,
        .width = sizeof(int),
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
        .fid = 0x39dd03f8,
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

static uint32_t bcmint_l3_ecmp_member_ent_t_field_offset_get (bcmissu_field_id_t field_id)
{
    bcmint_l3_ecmp_member_ent_t var;

    switch (field_id) {
        case 0x599dcce2:
            return ((uint8_t *)&var.type) - (uint8_t *)&var;
        case 0x633da6e1:
            return ((uint8_t *)&var.ol_egr_obj) - (uint8_t *)&var;
        case 0x3185376b:
            return ((uint8_t *)&var.ul_egr_obj) - (uint8_t *)&var;
        case 0x1aeb891a:
            return ((uint8_t *)&var.dvp) - (uint8_t *)&var;
        case 0xe80253a7:
            return ((uint8_t *)&var.prot_swt_prfl_idx) - (uint8_t *)&var;
        case 0x39dd03f8:
            return ((uint8_t *)&var.ref_cnt) - (uint8_t *)&var;
        default:
            return ISSU_INVALID_FLD_OFFSET;
    }
}

const issu_struct_t bcmissu_struct_l3_egress_ha_bcmint_l3_ecmp_member_ent_t_v6_5_19 = {
    .offset_get_func = bcmint_l3_ecmp_member_ent_t_field_offset_get,
    .generic_size = sizeof(bcmint_l3_ecmp_member_ent_t),
    .field_count = 6,
    .fields = bcmissu_fields_bcmint_l3_ecmp_member_ent_t,
};
