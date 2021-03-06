/*******************************************************************************
 *
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated by npl-info-tbl-gen from NPL output files
 * for device bcm56880_a0 and variant dna_4_7_11.
 *
 * Tool: $SDK/INTERNAL/fltg/bin/npl-info-tbl-gen
 * Resource YML: PortIngEgrBlockProfileInfoTblGen.yml
 *
 * Edits to this file will be lost when it is regenerated.
 *
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

#include <shr/shr_debug.h>
#include <bcmcth/bcmcth_port_drv.h>
#include <bcmltd/chip/bcmltd_id.h>
#include <bcmptm/bcmptm_internal.h>
#include <bcmimm/bcmimm_int_comp.h>
#include <bcmcth/bcmcth_info_table_entry.h>

/*! Total enum fields. */
#define BCM56880_A0_DNA_4_7_11_ENUM_FIELD_TOTAL 17

/*! Structure holding initialization value. */
static bcmcth_info_table_enum_entry_t
bcm56880_a0_dna_4_7_11_cth_port_ing_egr_block_profile_info_enum_data[][17] = {
    {
        {
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt,
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt_PORT_ING_EGR_BLOCK_PROFILE_INFO_IDf,
            "PORT_LAG_FAIL_LOOPBACK_VIEW",
        },
        {
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt,
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt_MEMBERSHIP_PROFILE_PTRf,
            "ING_OBJ_BUS_MEMBERSHIP_SPANNING_TREE_PROFILE_PTR_7_0",
        },
        {
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt,
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt_STATE_PROFILE_PTRf,
            "ING_OBJ_BUS_MEMBERSHIP_SPANNING_TREE_PROFILE_PTR_15_8",
        },
        {
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt,
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt_BITMAP_MASK0_PTRf,
            "ING_OBJ_BUS_INGRESS_PP_PORT",
        },
        {
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt,
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt_BITMAP_MASK1_PTRf,
            "ING_OBJ_BUS_VFI_BITMAP_PROFILE_INDEX",
        },
        {
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt,
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt_BITMAP_MASK2_PTRf,
            "ING_OBJ_BUS_RANGE_TABLE_BITMAP_PROFILE_INDEX_7_0",
        },
        {
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt,
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt_BITMAP_MASK3_PTRf,
            "ING_OBJ_BUS_IFP_BITMAP_PROFILE_INDEX",
        },
        {
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt,
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt_BITMAP_MASK_SEL_0f,
            "ING_CMD_BUS_BITMAP_PROFILE_0_1_SECTION_SEL",
        },
        {
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt,
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt_BITMAP_MASK_SEL_1f,
            "ING_CMD_BUS_BITMAP_PROFILE_0_1_SECTION_SEL",
        },
        {
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt,
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt_BITMAP_MASK_SEL_2f,
            "ING_CMD_BUS_NUM_OF_FWD_EXT_HEADERS_SYSTEM_PORT_BITMAP_PROFILE_SECTION_SEL_1_0",
        },
        {
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt,
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt_BITMAP_MASK_SEL_3f,
            "ING_CMD_BUS_IFP_BITMAP_PROFILE_SECTION_SEL",
        },
        {
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt,
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt_UNICASTf,
            "IPOST_SCRATCH_BUS_UNICAST_FLOW",
        },
        {
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt,
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt_PRUNE_BMPf,
            "IPOST_SCRATCH_BUS_PRUNE_BITMAP",
        },
        {
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt,
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt_L2_BMPf,
            "IPOST_SCRATCH_BUS_L2_BITMAP",
        },
        {
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt,
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt_L3_BMPf,
            "IPOST_SCRATCH_BUS_L3_BITMAP",
        },
        {
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt,
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt_UNICAST_DEST_PORTf,
            "IPOST_SCRATCH_BUS_EGRESS_DEVICE_PORT",
        },
        {
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt,
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt_DLB_DESTINATIONf,
            "IPOST_SCRATCH_BUS_DLB_DESTINATION",
        },
    },
    {
        {
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt,
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt_PORT_ING_EGR_BLOCK_PROFILE_INFO_IDf,
            "PORT_BASE_VIEW",
        },
        {
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt,
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt_MEMBERSHIP_PROFILE_PTRf,
            "ING_OBJ_BUS_MEMBERSHIP_SPANNING_TREE_PROFILE_PTR_7_0",
        },
        {
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt,
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt_STATE_PROFILE_PTRf,
            "ING_OBJ_BUS_MEMBERSHIP_SPANNING_TREE_PROFILE_PTR_15_8",
        },
        {
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt,
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt_BITMAP_MASK0_PTRf,
            "ING_OBJ_BUS_INGRESS_PP_PORT",
        },
        {
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt,
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt_BITMAP_MASK1_PTRf,
            "ING_OBJ_BUS_VFI_BITMAP_PROFILE_INDEX",
        },
        {
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt,
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt_BITMAP_MASK2_PTRf,
            "ING_OBJ_BUS_RANGE_TABLE_BITMAP_PROFILE_INDEX_7_0",
        },
        {
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt,
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt_BITMAP_MASK3_PTRf,
            "ING_OBJ_BUS_IFP_BITMAP_PROFILE_INDEX",
        },
        {
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt,
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt_BITMAP_MASK_SEL_0f,
            "ING_CMD_BUS_BITMAP_PROFILE_0_1_SECTION_SEL",
        },
        {
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt,
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt_BITMAP_MASK_SEL_1f,
            "ING_CMD_BUS_BITMAP_PROFILE_0_1_SECTION_SEL",
        },
        {
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt,
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt_BITMAP_MASK_SEL_2f,
            "ING_CMD_BUS_NUM_OF_FWD_EXT_HEADERS_SYSTEM_PORT_BITMAP_PROFILE_SECTION_SEL_1_0",
        },
        {
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt,
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt_BITMAP_MASK_SEL_3f,
            "ING_CMD_BUS_IFP_BITMAP_PROFILE_SECTION_SEL",
        },
        {
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt,
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt_UNICASTf,
            "IPOST_SCRATCH_BUS_UNICAST_FLOW",
        },
        {
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt,
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt_PRUNE_BMPf,
            "IPOST_SCRATCH_BUS_PRUNE_BITMAP",
        },
        {
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt,
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt_L2_BMPf,
            "IPOST_SCRATCH_BUS_L2_BITMAP",
        },
        {
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt,
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt_L3_BMPf,
            "IPOST_SCRATCH_BUS_L3_BITMAP",
        },
        {
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt,
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt_UNICAST_DEST_PORTf,
            "IPOST_SCRATCH_BUS_EGRESS_DEVICE_PORT",
        },
        {
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt,
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt_DLB_DESTINATIONf,
            "IPOST_SCRATCH_BUS_DLB_DESTINATION",
        },
    },
    {
        {
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt,
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt_PORT_ING_EGR_BLOCK_PROFILE_INFO_IDf,
            "PORT_IFP_VIEW",
        },
        {
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt,
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt_MEMBERSHIP_PROFILE_PTRf,
            "ING_OBJ_BUS_MEMBERSHIP_SPANNING_TREE_PROFILE_PTR_7_0",
        },
        {
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt,
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt_STATE_PROFILE_PTRf,
            "ING_OBJ_BUS_MEMBERSHIP_SPANNING_TREE_PROFILE_PTR_15_8",
        },
        {
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt,
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt_BITMAP_MASK0_PTRf,
            "ING_OBJ_BUS_INGRESS_PP_PORT",
        },
        {
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt,
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt_BITMAP_MASK1_PTRf,
            "ING_OBJ_BUS_VFI_BITMAP_PROFILE_INDEX",
        },
        {
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt,
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt_BITMAP_MASK2_PTRf,
            "ING_OBJ_BUS_RANGE_TABLE_BITMAP_PROFILE_INDEX_7_0",
        },
        {
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt,
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt_BITMAP_MASK3_PTRf,
            "ING_OBJ_BUS_IFP_BITMAP_PROFILE_INDEX",
        },
        {
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt,
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt_BITMAP_MASK_SEL_0f,
            "ING_CMD_BUS_BITMAP_PROFILE_0_1_SECTION_SEL",
        },
        {
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt,
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt_BITMAP_MASK_SEL_1f,
            "ING_CMD_BUS_BITMAP_PROFILE_0_1_SECTION_SEL",
        },
        {
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt,
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt_BITMAP_MASK_SEL_2f,
            "ING_CMD_BUS_NUM_OF_FWD_EXT_HEADERS_SYSTEM_PORT_BITMAP_PROFILE_SECTION_SEL_1_0",
        },
        {
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt,
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt_BITMAP_MASK_SEL_3f,
            "ING_CMD_BUS_IFP_BITMAP_PROFILE_SECTION_SEL",
        },
        {
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt,
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt_UNICASTf,
            "IPOST_SCRATCH_BUS_UNICAST_FLOW",
        },
        {
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt,
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt_PRUNE_BMPf,
            "IPOST_SCRATCH_BUS_PRUNE_BITMAP",
        },
        {
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt,
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt_L2_BMPf,
            "IPOST_SCRATCH_BUS_L2_BITMAP",
        },
        {
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt,
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt_L3_BMPf,
            "IPOST_SCRATCH_BUS_L3_BITMAP",
        },
        {
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt,
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt_UNICAST_DEST_PORTf,
            "IPOST_SCRATCH_BUS_EGRESS_DEVICE_PORT",
        },
        {
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt,
            BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt_DLB_DESTINATIONf,
            "IPOST_SCRATCH_BUS_DLB_DESTINATION",
        },
    },
};


/*******************************************************************************
 * Private functions
 */

static int
bcm56880_a0_dna_4_7_11_cth_port_ing_egr_block_profile_info_get_variant_data(int unit, bcmcth_info_table_t *data)
{
    data->table_id = BCM56880_A0_DNA_4_7_11_PORT_ING_EGR_BLOCK_PROFILE_INFOt;
    sal_memcpy(&data->table_name[0], "PORT_ING_EGR_BLOCK_PROFILE_INFO", sizeof("PORT_ING_EGR_BLOCK_PROFILE_INFO"));
    data->max_policy_supported = 4;
    data->enum_cnt = BCM56880_A0_DNA_4_7_11_ENUM_FIELD_TOTAL;
    data->profile_cnt = sizeof(bcm56880_a0_dna_4_7_11_cth_port_ing_egr_block_profile_info_enum_data) /
                        (sizeof(bcmcth_info_table_enum_entry_t) * BCM56880_A0_DNA_4_7_11_ENUM_FIELD_TOTAL);

    data->enum_info = &bcm56880_a0_dna_4_7_11_cth_port_ing_egr_block_profile_info_enum_data[0][0];
    data->policy_info = NULL;

    return SHR_E_NONE;
}

static bcmcth_port_info_drv_t
bcm56880_a0_dna_4_7_11_cth_port_ing_egr_block_profile_info_drv = {
    .get_data  = bcm56880_a0_dna_4_7_11_cth_port_ing_egr_block_profile_info_get_variant_data,
};

/*******************************************************************************
 * Public functions
 */
bcmcth_port_info_drv_t * bcm56880_a0_dna_4_7_11_cth_port_ing_egr_block_profile_info_drv_get(int unit)
{
    return &bcm56880_a0_dna_4_7_11_cth_port_ing_egr_block_profile_info_drv;
}

