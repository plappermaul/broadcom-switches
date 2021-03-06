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
#include "bcmltm_ha.h"

static const issu_field_t bcmissu_fields_bcmltm_lt_state_data_t[] = {
    {
        .fid = 0xb4744c9d,
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
        .fid = 0x911e0545,
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
        .fid = 0xaec37382,
        .width = sizeof(bcmltm_state_type_t),
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
        .fid = 0x38b28c0b,
        .width = sizeof(shr_ha_ptr_t),
        .ha_ptr = HA_PTR,
        .lt_attrib = HA_LT_NONE,
        .ltid_var_for_fid = ISSU_INVALID_FIELD_ID,
        .is_array = false,
        .var_size_id = ISSU_INVALID_FIELD_ID,
        .size = 0,
        .struct_id = ISSU_INVALID_STRUCT_ID,
        .enum_type_name = NULL,
    },
    {
        .fid = 0xaffd6297,
        .width = sizeof(shr_ha_ptr_t),
        .ha_ptr = HA_PTR,
        .lt_attrib = HA_LT_NONE,
        .ltid_var_for_fid = ISSU_INVALID_FIELD_ID,
        .is_array = false,
        .var_size_id = ISSU_INVALID_FIELD_ID,
        .size = 0,
        .struct_id = ISSU_INVALID_STRUCT_ID,
        .enum_type_name = NULL,
    },
};

static uint32_t bcmltm_lt_state_data_t_field_offset_get (bcmissu_field_id_t field_id)
{
    bcmltm_lt_state_data_t var;

    switch (field_id) {
        case 0xb4744c9d:
            return ((uint8_t *)&var.ltid) - (uint8_t *)&var;
        case 0x911e0545:
            return ((uint8_t *)&var.state_data_size) - (uint8_t *)&var;
        case 0xaec37382:
            return ((uint8_t *)&var.state_data_type) - (uint8_t *)&var;
        case 0x38b28c0b:
            return ((uint8_t *)&var.state_data_hap) - (uint8_t *)&var;
        case 0xaffd6297:
            return ((uint8_t *)&var.next_state_data_hap) - (uint8_t *)&var;
        default:
            return ISSU_INVALID_FLD_OFFSET;
    }
}

const issu_struct_t bcmissu_struct_bcmltm_ha_bcmltm_lt_state_data_t_v1_3 = {
    .offset_get_func = bcmltm_lt_state_data_t_field_offset_get,
    .generic_size = sizeof(bcmltm_lt_state_data_t),
    .field_count = 5,
    .fields = bcmissu_fields_bcmltm_lt_state_data_t,
};

static const issu_field_t bcmissu_fields_bcmltm_lt_state_t[] = {
    {
        .fid = 0x84d10a1e,
        .width = sizeof(bool),
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
        .fid = 0xb4744c9d,
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
        .fid = 0x3c2fa9c4,
        .width = sizeof(bool),
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
        .fid = 0x03df3c8d,
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
        .fid = 0xcf2a7958,
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
        .fid = 0xc5c1177e,
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
        .fid = 0x299a4159,
        .width = sizeof(shr_ha_ptr_t),
        .ha_ptr = HA_PTR,
        .lt_attrib = HA_LT_NONE,
        .ltid_var_for_fid = ISSU_INVALID_FIELD_ID,
        .is_array = false,
        .var_size_id = ISSU_INVALID_FIELD_ID,
        .size = 0,
        .struct_id = ISSU_INVALID_STRUCT_ID,
        .enum_type_name = NULL,
    },
};

static uint32_t bcmltm_lt_state_t_field_offset_get (bcmissu_field_id_t field_id)
{
    bcmltm_lt_state_t var;

    switch (field_id) {
        case 0x84d10a1e:
            return ((uint8_t *)&var.initialized) - (uint8_t *)&var;
        case 0xb4744c9d:
            return ((uint8_t *)&var.ltid) - (uint8_t *)&var;
        case 0x3c2fa9c4:
            return ((uint8_t *)&var.table_op_pt_info_enable) - (uint8_t *)&var;
        case 0x03df3c8d:
            return ((uint8_t *)&var.entry_count) - (uint8_t *)&var;
        case 0xcf2a7958:
            return ((uint8_t *)&var.max_entry_count) - (uint8_t *)&var;
        case 0xc5c1177e:
            return ((uint8_t *)&var.bitmap_size) - (uint8_t *)&var;
        case 0x299a4159:
            return ((uint8_t *)&var.table_state_data_hap) - (uint8_t *)&var;
        default:
            return ISSU_INVALID_FLD_OFFSET;
    }
}

const issu_struct_t bcmissu_struct_bcmltm_ha_bcmltm_lt_state_t_v1_3 = {
    .offset_get_func = bcmltm_lt_state_t_field_offset_get,
    .generic_size = sizeof(bcmltm_lt_state_t),
    .field_count = 7,
    .fields = bcmissu_fields_bcmltm_lt_state_t,
};

static const issu_field_t bcmissu_fields_bcmltm_transaction_status_t[] = {
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
        .fid = 0xdc370d1b,
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
        .fid = 0xa81561fa,
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
        .fid = 0xe009192b,
        .width = sizeof(shr_ha_ptr_t),
        .ha_ptr = HA_PTR,
        .lt_attrib = HA_LT_NONE,
        .ltid_var_for_fid = ISSU_INVALID_FIELD_ID,
        .is_array = false,
        .var_size_id = ISSU_INVALID_FIELD_ID,
        .size = 0,
        .struct_id = ISSU_INVALID_STRUCT_ID,
        .enum_type_name = NULL,
    },
};

static uint32_t bcmltm_transaction_status_t_field_offset_get (bcmissu_field_id_t field_id)
{
    bcmltm_transaction_status_t var;

    switch (field_id) {
        case 0xd6a2a1db:
            return ((uint8_t *)&var.trans_id) - (uint8_t *)&var;
        case 0xdc370d1b:
            return ((uint8_t *)&var.max_lt_num) - (uint8_t *)&var;
        case 0xa81561fa:
            return ((uint8_t *)&var.lt_num) - (uint8_t *)&var;
        case 0xe009192b:
            return ((uint8_t *)&var.ltid_list_hap) - (uint8_t *)&var;
        default:
            return ISSU_INVALID_FLD_OFFSET;
    }
}

const issu_struct_t bcmissu_struct_bcmltm_ha_bcmltm_transaction_status_t_v1_3 = {
    .offset_get_func = bcmltm_transaction_status_t_field_offset_get,
    .generic_size = sizeof(bcmltm_transaction_status_t),
    .field_count = 4,
    .fields = bcmissu_fields_bcmltm_transaction_status_t,
};

static const issu_field_t bcmissu_fields_bcmltm_ha_block_header_t[] = {
    {
        .fid = 0xac201fd2,
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
        .fid = 0x471827d8,
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
        .fid = 0xbdc2fa1b,
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

static uint32_t bcmltm_ha_block_header_t_field_offset_get (bcmissu_field_id_t field_id)
{
    bcmltm_ha_block_header_t var;

    switch (field_id) {
        case 0xac201fd2:
            return ((uint8_t *)&var.signature) - (uint8_t *)&var;
        case 0x471827d8:
            return ((uint8_t *)&var.block_size) - (uint8_t *)&var;
        case 0xbdc2fa1b:
            return ((uint8_t *)&var.allocated_size) - (uint8_t *)&var;
        default:
            return ISSU_INVALID_FLD_OFFSET;
    }
}

const issu_struct_t bcmissu_struct_bcmltm_ha_bcmltm_ha_block_header_t_v1_3 = {
    .offset_get_func = bcmltm_ha_block_header_t_field_offset_get,
    .generic_size = sizeof(bcmltm_ha_block_header_t),
    .field_count = 3,
    .fields = bcmissu_fields_bcmltm_ha_block_header_t,
};
