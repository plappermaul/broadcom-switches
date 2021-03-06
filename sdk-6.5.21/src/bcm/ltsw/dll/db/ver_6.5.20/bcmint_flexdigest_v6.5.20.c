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
#include "flexdigest_ha.h"

static const issu_field_t bcmissu_fields_bcmint_flexdigest_lkup_entry_qualifier_t[] = {
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
        .is_local_enum = false,
        .enum_type_name = NULL,
    },
    {
        .fid = 0xc5e33391,
        .width = sizeof(int),
        .ha_ptr = HA_PTR_NONE,
        .lt_attrib = HA_LT_NONE,
        .ltid_var_for_fid = ISSU_INVALID_FIELD_ID,
        .is_array = false,
        .var_size_id = ISSU_INVALID_FIELD_ID,
        .size = 0,
        .struct_id = ISSU_INVALID_STRUCT_ID,
        .is_local_enum = false,
        .enum_type_name = NULL,
    },
    {
        .fid = 0x8d777f38,
        .width = sizeof(uint16_t),
        .ha_ptr = HA_PTR_NONE,
        .lt_attrib = HA_LT_NONE,
        .ltid_var_for_fid = ISSU_INVALID_FIELD_ID,
        .is_array = false,
        .var_size_id = ISSU_INVALID_FIELD_ID,
        .size = 0,
        .struct_id = ISSU_INVALID_STRUCT_ID,
        .is_local_enum = false,
        .enum_type_name = NULL,
    },
    {
        .fid = 0xf2ce11eb,
        .width = sizeof(uint16_t),
        .ha_ptr = HA_PTR_NONE,
        .lt_attrib = HA_LT_NONE,
        .ltid_var_for_fid = ISSU_INVALID_FIELD_ID,
        .is_array = false,
        .var_size_id = ISSU_INVALID_FIELD_ID,
        .size = 0,
        .struct_id = ISSU_INVALID_STRUCT_ID,
        .is_local_enum = false,
        .enum_type_name = NULL,
    },
};

static uint32_t bcmint_flexdigest_lkup_entry_qualifier_t_field_offset_get (bcmissu_field_id_t field_id)
{
    bcmint_flexdigest_lkup_entry_qualifier_t var;

    switch (field_id) {
        case 0x4e5868d6:
            return ((uint8_t *)&var.flags) - (uint8_t *)&var;
        case 0xc5e33391:
            return ((uint8_t *)&var.qualifier) - (uint8_t *)&var;
        case 0x8d777f38:
            return ((uint8_t *)&var.data) - (uint8_t *)&var;
        case 0xf2ce11eb:
            return ((uint8_t *)&var.mask) - (uint8_t *)&var;
        default:
            return ISSU_INVALID_FLD_OFFSET;
    }
}

const issu_struct_t bcmissu_struct_flexdigest_ha_bcmint_flexdigest_lkup_entry_qualifier_t_v6_5_20 = {
    .offset_get_func = bcmint_flexdigest_lkup_entry_qualifier_t_field_offset_get,
    .generic_size = sizeof(bcmint_flexdigest_lkup_entry_qualifier_t),
    .field_count = 4,
    .fields = bcmissu_fields_bcmint_flexdigest_lkup_entry_qualifier_t,
};

static const issu_field_t bcmissu_fields_bcmint_flexdigest_lkup_entry_action_t[] = {
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
        .is_local_enum = false,
        .enum_type_name = NULL,
    },
    {
        .fid = 0x418c5509,
        .width = sizeof(int),
        .ha_ptr = HA_PTR_NONE,
        .lt_attrib = HA_LT_NONE,
        .ltid_var_for_fid = ISSU_INVALID_FIELD_ID,
        .is_array = false,
        .var_size_id = ISSU_INVALID_FIELD_ID,
        .size = 0,
        .struct_id = ISSU_INVALID_STRUCT_ID,
        .is_local_enum = false,
        .enum_type_name = NULL,
    },
    {
        .fid = 0x1f4d76ab,
        .width = sizeof(uint16_t),
        .ha_ptr = HA_PTR_NONE,
        .lt_attrib = HA_LT_NONE,
        .ltid_var_for_fid = ISSU_INVALID_FIELD_ID,
        .is_array = false,
        .var_size_id = ISSU_INVALID_FIELD_ID,
        .size = 0,
        .struct_id = ISSU_INVALID_STRUCT_ID,
        .is_local_enum = false,
        .enum_type_name = NULL,
    },
    {
        .fid = 0xa2cbb63a,
        .width = sizeof(uint16_t),
        .ha_ptr = HA_PTR_NONE,
        .lt_attrib = HA_LT_NONE,
        .ltid_var_for_fid = ISSU_INVALID_FIELD_ID,
        .is_array = false,
        .var_size_id = ISSU_INVALID_FIELD_ID,
        .size = 0,
        .struct_id = ISSU_INVALID_STRUCT_ID,
        .is_local_enum = false,
        .enum_type_name = NULL,
    },
};

static uint32_t bcmint_flexdigest_lkup_entry_action_t_field_offset_get (bcmissu_field_id_t field_id)
{
    bcmint_flexdigest_lkup_entry_action_t var;

    switch (field_id) {
        case 0x4e5868d6:
            return ((uint8_t *)&var.flags) - (uint8_t *)&var;
        case 0x418c5509:
            return ((uint8_t *)&var.action) - (uint8_t *)&var;
        case 0x1f4d76ab:
            return ((uint8_t *)&var.param0) - (uint8_t *)&var;
        case 0xa2cbb63a:
            return ((uint8_t *)&var.param1) - (uint8_t *)&var;
        default:
            return ISSU_INVALID_FLD_OFFSET;
    }
}

const issu_struct_t bcmissu_struct_flexdigest_ha_bcmint_flexdigest_lkup_entry_action_t_v6_5_20 = {
    .offset_get_func = bcmint_flexdigest_lkup_entry_action_t_field_offset_get,
    .generic_size = sizeof(bcmint_flexdigest_lkup_entry_action_t),
    .field_count = 4,
    .fields = bcmissu_fields_bcmint_flexdigest_lkup_entry_action_t,
};

static const issu_field_t bcmissu_fields_bcmint_flexdigest_lkup_entry_t[] = {
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
        .is_local_enum = false,
        .enum_type_name = NULL,
    },
    {
        .fid = 0xb80bb774,
        .width = sizeof(int),
        .ha_ptr = HA_PTR_NONE,
        .lt_attrib = HA_LT_NONE,
        .ltid_var_for_fid = ISSU_INVALID_FIELD_ID,
        .is_array = false,
        .var_size_id = ISSU_INVALID_FIELD_ID,
        .size = 0,
        .struct_id = ISSU_INVALID_STRUCT_ID,
        .is_local_enum = false,
        .enum_type_name = NULL,
    },
    {
        .fid = 0xe060bb62,
        .width = sizeof(int),
        .ha_ptr = HA_PTR_NONE,
        .lt_attrib = HA_LT_NONE,
        .ltid_var_for_fid = ISSU_INVALID_FIELD_ID,
        .is_array = false,
        .var_size_id = ISSU_INVALID_FIELD_ID,
        .size = 0,
        .struct_id = ISSU_INVALID_STRUCT_ID,
        .is_local_enum = false,
        .enum_type_name = NULL,
    },
};

static uint32_t bcmint_flexdigest_lkup_entry_t_field_offset_get (bcmissu_field_id_t field_id)
{
    bcmint_flexdigest_lkup_entry_t var;

    switch (field_id) {
        case 0x4e5868d6:
            return ((uint8_t *)&var.flags) - (uint8_t *)&var;
        case 0xb80bb774:
            return ((uint8_t *)&var.id) - (uint8_t *)&var;
        case 0xe060bb62:
            return ((uint8_t *)&var.pri) - (uint8_t *)&var;
        default:
            return ISSU_INVALID_FLD_OFFSET;
    }
}

const issu_struct_t bcmissu_struct_flexdigest_ha_bcmint_flexdigest_lkup_entry_t_v6_5_20 = {
    .offset_get_func = bcmint_flexdigest_lkup_entry_t_field_offset_get,
    .generic_size = sizeof(bcmint_flexdigest_lkup_entry_t),
    .field_count = 3,
    .fields = bcmissu_fields_bcmint_flexdigest_lkup_entry_t,
};

static const issu_field_t bcmissu_fields_bcmint_flexdigest_lkup_group_t[] = {
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
        .is_local_enum = false,
        .enum_type_name = NULL,
    },
    {
        .fid = 0xb80bb774,
        .width = sizeof(int),
        .ha_ptr = HA_PTR_NONE,
        .lt_attrib = HA_LT_NONE,
        .ltid_var_for_fid = ISSU_INVALID_FIELD_ID,
        .is_array = false,
        .var_size_id = ISSU_INVALID_FIELD_ID,
        .size = 0,
        .struct_id = ISSU_INVALID_STRUCT_ID,
        .is_local_enum = false,
        .enum_type_name = NULL,
    },
    {
        .fid = 0xe060bb62,
        .width = sizeof(int),
        .ha_ptr = HA_PTR_NONE,
        .lt_attrib = HA_LT_NONE,
        .ltid_var_for_fid = ISSU_INVALID_FIELD_ID,
        .is_array = false,
        .var_size_id = ISSU_INVALID_FIELD_ID,
        .size = 0,
        .struct_id = ISSU_INVALID_STRUCT_ID,
        .is_local_enum = false,
        .enum_type_name = NULL,
    },
    {
        .fid = 0xc0d4a470,
        .width = sizeof(int),
        .ha_ptr = HA_PTR_NONE,
        .lt_attrib = HA_LT_NONE,
        .ltid_var_for_fid = ISSU_INVALID_FIELD_ID,
        .is_array = false,
        .var_size_id = ISSU_INVALID_FIELD_ID,
        .size = 0,
        .struct_id = ISSU_INVALID_STRUCT_ID,
        .is_local_enum = false,
        .enum_type_name = NULL,
    },
    {
        .fid = 0xa56e463b,
        .width = sizeof(int),
        .ha_ptr = HA_PTR_NONE,
        .lt_attrib = HA_LT_NONE,
        .ltid_var_for_fid = ISSU_INVALID_FIELD_ID,
        .is_array = false,
        .var_size_id = ISSU_INVALID_FIELD_ID,
        .size = 0,
        .struct_id = ISSU_INVALID_STRUCT_ID,
        .is_local_enum = false,
        .enum_type_name = NULL,
    },
    {
        .fid = 0x7630e09e,
        .width = sizeof(int),
        .ha_ptr = HA_PTR_NONE,
        .lt_attrib = HA_LT_NONE,
        .ltid_var_for_fid = ISSU_INVALID_FIELD_ID,
        .is_array = false,
        .var_size_id = ISSU_INVALID_FIELD_ID,
        .size = 0,
        .struct_id = ISSU_INVALID_STRUCT_ID,
        .is_local_enum = false,
        .enum_type_name = NULL,
    },
    {
        .fid = 0xffe30da5,
        .width = sizeof(int),
        .ha_ptr = HA_PTR_NONE,
        .lt_attrib = HA_LT_NONE,
        .ltid_var_for_fid = ISSU_INVALID_FIELD_ID,
        .is_array = false,
        .var_size_id = ISSU_INVALID_FIELD_ID,
        .size = 0,
        .struct_id = ISSU_INVALID_STRUCT_ID,
        .is_local_enum = false,
        .enum_type_name = NULL,
    },
    {
        .fid = 0x08d03f1e,
        .width = sizeof(int),
        .ha_ptr = HA_PTR_NONE,
        .lt_attrib = HA_LT_NONE,
        .ltid_var_for_fid = ISSU_INVALID_FIELD_ID,
        .is_array = false,
        .var_size_id = ISSU_INVALID_FIELD_ID,
        .size = 0,
        .struct_id = ISSU_INVALID_STRUCT_ID,
        .is_local_enum = false,
        .enum_type_name = NULL,
    },
};

static uint32_t bcmint_flexdigest_lkup_group_t_field_offset_get (bcmissu_field_id_t field_id)
{
    bcmint_flexdigest_lkup_group_t var;

    switch (field_id) {
        case 0x4e5868d6:
            return ((uint8_t *)&var.flags) - (uint8_t *)&var;
        case 0xb80bb774:
            return ((uint8_t *)&var.id) - (uint8_t *)&var;
        case 0xe060bb62:
            return ((uint8_t *)&var.pri) - (uint8_t *)&var;
        case 0xc0d4a470:
            return ((uint8_t *)&var.sub_comp_id_base) - (uint8_t *)&var;
        case 0xa56e463b:
            return ((uint8_t *)&var.qualifier_max_per_entry) - (uint8_t *)&var;
        case 0x7630e09e:
            return ((uint8_t *)&var.action_max_per_entry) - (uint8_t *)&var;
        case 0xffe30da5:
            return ((uint8_t *)&var.entry_id_base) - (uint8_t *)&var;
        case 0x08d03f1e:
            return ((uint8_t *)&var.entry_id_count) - (uint8_t *)&var;
        default:
            return ISSU_INVALID_FLD_OFFSET;
    }
}

const issu_struct_t bcmissu_struct_flexdigest_ha_bcmint_flexdigest_lkup_group_t_v6_5_20 = {
    .offset_get_func = bcmint_flexdigest_lkup_group_t_field_offset_get,
    .generic_size = sizeof(bcmint_flexdigest_lkup_group_t),
    .field_count = 8,
    .fields = bcmissu_fields_bcmint_flexdigest_lkup_group_t,
};

static const issu_field_t bcmissu_fields_bcmint_flexdigest_lkup_group_qualifier_presel_t[] = {
    {
        .fid = 0x9f7d0ee8,
        .width = sizeof(bool),
        .ha_ptr = HA_PTR_NONE,
        .lt_attrib = HA_LT_NONE,
        .ltid_var_for_fid = ISSU_INVALID_FIELD_ID,
        .is_array = false,
        .var_size_id = ISSU_INVALID_FIELD_ID,
        .size = 0,
        .struct_id = ISSU_INVALID_STRUCT_ID,
        .is_local_enum = false,
        .enum_type_name = NULL,
    },
    {
        .fid = 0xc5e33391,
        .width = sizeof(int),
        .ha_ptr = HA_PTR_NONE,
        .lt_attrib = HA_LT_NONE,
        .ltid_var_for_fid = ISSU_INVALID_FIELD_ID,
        .is_array = false,
        .var_size_id = ISSU_INVALID_FIELD_ID,
        .size = 0,
        .struct_id = ISSU_INVALID_STRUCT_ID,
        .is_local_enum = false,
        .enum_type_name = NULL,
    },
};

static uint32_t bcmint_flexdigest_lkup_group_qualifier_presel_t_field_offset_get (bcmissu_field_id_t field_id)
{
    bcmint_flexdigest_lkup_group_qualifier_presel_t var;

    switch (field_id) {
        case 0x9f7d0ee8:
            return ((uint8_t *)&var.valid) - (uint8_t *)&var;
        case 0xc5e33391:
            return ((uint8_t *)&var.qualifier) - (uint8_t *)&var;
        default:
            return ISSU_INVALID_FLD_OFFSET;
    }
}

const issu_struct_t bcmissu_struct_flexdigest_ha_bcmint_flexdigest_lkup_group_qualifier_presel_t_v6_5_20 = {
    .offset_get_func = bcmint_flexdigest_lkup_group_qualifier_presel_t_field_offset_get,
    .generic_size = sizeof(bcmint_flexdigest_lkup_group_qualifier_presel_t),
    .field_count = 2,
    .fields = bcmissu_fields_bcmint_flexdigest_lkup_group_qualifier_presel_t,
};

static const issu_field_t bcmissu_fields_bcmint_flexdigest_lkup_match_t[] = {
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
        .is_local_enum = false,
        .enum_type_name = NULL,
    },
    {
        .fid = 0xe3cc92c1,
        .width = sizeof(int),
        .ha_ptr = HA_PTR_NONE,
        .lt_attrib = HA_LT_NONE,
        .ltid_var_for_fid = ISSU_INVALID_FIELD_ID,
        .is_array = false,
        .var_size_id = ISSU_INVALID_FIELD_ID,
        .size = 0,
        .struct_id = ISSU_INVALID_STRUCT_ID,
        .is_local_enum = false,
        .enum_type_name = NULL,
    },
};

static uint32_t bcmint_flexdigest_lkup_match_t_field_offset_get (bcmissu_field_id_t field_id)
{
    bcmint_flexdigest_lkup_match_t var;

    switch (field_id) {
        case 0x4e5868d6:
            return ((uint8_t *)&var.flags) - (uint8_t *)&var;
        case 0xe3cc92c1:
            return ((uint8_t *)&var.match) - (uint8_t *)&var;
        default:
            return ISSU_INVALID_FLD_OFFSET;
    }
}

const issu_struct_t bcmissu_struct_flexdigest_ha_bcmint_flexdigest_lkup_match_t_v6_5_20 = {
    .offset_get_func = bcmint_flexdigest_lkup_match_t_field_offset_get,
    .generic_size = sizeof(bcmint_flexdigest_lkup_match_t),
    .field_count = 2,
    .fields = bcmissu_fields_bcmint_flexdigest_lkup_match_t,
};

static const issu_field_t bcmissu_fields_bcmint_flexdigest_lkup_match_info_t[] = {
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
        .is_local_enum = false,
        .enum_type_name = NULL,
    },
    {
        .fid = 0xb80bb774,
        .width = sizeof(uint16_t),
        .ha_ptr = HA_PTR_NONE,
        .lt_attrib = HA_LT_NONE,
        .ltid_var_for_fid = ISSU_INVALID_FIELD_ID,
        .is_array = false,
        .var_size_id = ISSU_INVALID_FIELD_ID,
        .size = 0,
        .struct_id = ISSU_INVALID_STRUCT_ID,
        .is_local_enum = false,
        .enum_type_name = NULL,
    },
    {
        .fid = 0xb3d2f97e,
        .width = sizeof(uint32_t),
        .ha_ptr = HA_PTR_NONE,
        .lt_attrib = HA_LT_NONE,
        .ltid_var_for_fid = ISSU_INVALID_FIELD_ID,
        .is_array = false,
        .var_size_id = ISSU_INVALID_FIELD_ID,
        .size = 0,
        .struct_id = ISSU_INVALID_STRUCT_ID,
        .is_local_enum = false,
        .enum_type_name = NULL,
    },
};

static uint32_t bcmint_flexdigest_lkup_match_info_t_field_offset_get (bcmissu_field_id_t field_id)
{
    bcmint_flexdigest_lkup_match_info_t var;

    switch (field_id) {
        case 0x4e5868d6:
            return ((uint8_t *)&var.flags) - (uint8_t *)&var;
        case 0xb80bb774:
            return ((uint8_t *)&var.id) - (uint8_t *)&var;
        case 0xb3d2f97e:
            return ((uint8_t *)&var.ref_count) - (uint8_t *)&var;
        default:
            return ISSU_INVALID_FLD_OFFSET;
    }
}

const issu_struct_t bcmissu_struct_flexdigest_ha_bcmint_flexdigest_lkup_match_info_t_v6_5_20 = {
    .offset_get_func = bcmint_flexdigest_lkup_match_info_t_field_offset_get,
    .generic_size = sizeof(bcmint_flexdigest_lkup_match_info_t),
    .field_count = 3,
    .fields = bcmissu_fields_bcmint_flexdigest_lkup_match_info_t,
};
