/*******************************************************************************
 *
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated by fltg from Logical Table mapping files.
 *
 * Tool: $SDK/INTERNAL/fltg/bin/fltg
 *
 * Edits to this file will be lost when it is regenerated.
 *
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */
/* Logical Table Adaptor for component bcmltx */
/* Handler: bcm56990_b0_lta_bcmltx_mirror_encap_udp_xfrm_handler */
#include <bcmlrd/bcmlrd_types.h>
#include <bcmltd/chip/bcmltd_id.h>
#include <bcmltx/bcmmirror/bcmltx_mirror_encap_udp.h>
#include <bcmdrd/chip/bcm56990_b0_enum.h>
#include <bcmlrd/chip/bcm56990_b0/bcm56990_b0_lrd_xfrm_field_desc.h>

extern const bcmltd_field_desc_t
bcm56990_b0_lrd_bcmltx_mirror_encap_udp_src_field_desc_s0[];

extern const bcmltd_field_desc_t
bcm56990_b0_lrd_bcmltx_mirror_encap_udp_src_field_desc_s1[];

extern const bcmltd_field_desc_t
bcm56990_b0_lrd_bcmltx_mirror_encap_udp_src_field_desc_s2[];

extern const bcmltd_field_desc_t
bcm56990_b0_lrd_bcmltx_mirror_encap_udp_src_field_desc_s3[];

extern const bcmltd_field_desc_t
bcm56990_b0_lrd_bcmltx_mirror_encap_udp_src_field_desc_s4[];

extern const bcmltd_field_desc_t
bcm56990_b0_lrd_bcmltx_mirror_encap_udp_src_field_desc_s5[];

extern const bcmltd_field_desc_t
bcm56990_b0_lrd_bcmltx_mirror_encap_udp_src_field_desc_s6[];

extern const bcmltd_field_desc_t
bcm56990_b0_lrd_bcmltx_mirror_encap_udp_src_field_desc_s7[];

extern const bcmltd_field_desc_t
bcm56990_b0_lrd_bcmltx_mirror_encap_udp_src_field_desc_s8[];

extern const bcmltd_field_desc_t
bcm56990_b0_lrd_bcmltx_mirror_encap_udp_src_field_desc_s9[];

extern const bcmltd_field_desc_t
bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_mirror_on_drop_dst_field_desc_d0[];

extern const bcmltd_field_desc_t
bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_mirror_on_drop_ipv6_dst_field_desc_d1[];

extern const bcmltd_field_desc_t
bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_psamp_dst_field_desc_d0[];

extern const bcmltd_field_desc_t
bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_psamp_ipv6_dst_field_desc_d1[];

extern const bcmltd_field_desc_t
bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_psamp_metadata_dst_field_desc_d0[];

extern const bcmltd_field_desc_t
bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_psamp_metadata_ipv6_dst_field_desc_d1[];

extern const bcmltd_field_desc_t
bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_sflow_dst_field_desc_d2[];

extern const bcmltd_field_desc_t
bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_sflow_ipv6_dst_field_desc_d2[];

extern const bcmltd_field_desc_t
bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_sflow_seq_dst_field_desc_d2[];

extern const bcmltd_field_desc_t
bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_sflow_seq_ipv6_dst_field_desc_d2[];

static const
bcmltd_field_list_t
bcm56990_b0_lta_bcmltx_mirror_encap_udp_src_list_s0 = {
    .field_num = 2,
    .field_array = bcm56990_b0_lrd_bcmltx_mirror_encap_udp_src_field_desc_s0
};

static const
bcmltd_field_list_t
bcm56990_b0_lta_bcmltx_mirror_encap_udp_src_list_s1 = {
    .field_num = 2,
    .field_array = bcm56990_b0_lrd_bcmltx_mirror_encap_udp_src_field_desc_s1
};

static const
bcmltd_field_list_t
bcm56990_b0_lta_bcmltx_mirror_encap_udp_src_list_s2 = {
    .field_num = 2,
    .field_array = bcm56990_b0_lrd_bcmltx_mirror_encap_udp_src_field_desc_s2
};

static const
bcmltd_field_list_t
bcm56990_b0_lta_bcmltx_mirror_encap_udp_src_list_s3 = {
    .field_num = 2,
    .field_array = bcm56990_b0_lrd_bcmltx_mirror_encap_udp_src_field_desc_s3
};

static const
bcmltd_field_list_t
bcm56990_b0_lta_bcmltx_mirror_encap_udp_src_list_s4 = {
    .field_num = 2,
    .field_array = bcm56990_b0_lrd_bcmltx_mirror_encap_udp_src_field_desc_s4
};

static const
bcmltd_field_list_t
bcm56990_b0_lta_bcmltx_mirror_encap_udp_src_list_s5 = {
    .field_num = 2,
    .field_array = bcm56990_b0_lrd_bcmltx_mirror_encap_udp_src_field_desc_s5
};

static const
bcmltd_field_list_t
bcm56990_b0_lta_bcmltx_mirror_encap_udp_src_list_s6 = {
    .field_num = 2,
    .field_array = bcm56990_b0_lrd_bcmltx_mirror_encap_udp_src_field_desc_s6
};

static const
bcmltd_field_list_t
bcm56990_b0_lta_bcmltx_mirror_encap_udp_src_list_s7 = {
    .field_num = 2,
    .field_array = bcm56990_b0_lrd_bcmltx_mirror_encap_udp_src_field_desc_s7
};

static const
bcmltd_field_list_t
bcm56990_b0_lta_bcmltx_mirror_encap_udp_src_list_s8 = {
    .field_num = 2,
    .field_array = bcm56990_b0_lrd_bcmltx_mirror_encap_udp_src_field_desc_s8
};

static const
bcmltd_field_list_t
bcm56990_b0_lta_bcmltx_mirror_encap_udp_src_list_s9 = {
    .field_num = 2,
    .field_array = bcm56990_b0_lrd_bcmltx_mirror_encap_udp_src_field_desc_s9
};

static const bcmltd_field_list_t
bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_mirror_on_drop_dst_list_d0 = {
    .field_num = 1,
    .field_array = bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_mirror_on_drop_dst_field_desc_d0
};

static const bcmltd_field_list_t
bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_mirror_on_drop_ipv6_dst_list_d1 = {
    .field_num = 1,
    .field_array = bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_mirror_on_drop_ipv6_dst_field_desc_d1
};

static const bcmltd_field_list_t
bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_psamp_dst_list_d0 = {
    .field_num = 1,
    .field_array = bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_psamp_dst_field_desc_d0
};

static const bcmltd_field_list_t
bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_psamp_ipv6_dst_list_d1 = {
    .field_num = 1,
    .field_array = bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_psamp_ipv6_dst_field_desc_d1
};

static const bcmltd_field_list_t
bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_psamp_metadata_dst_list_d0 = {
    .field_num = 1,
    .field_array = bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_psamp_metadata_dst_field_desc_d0
};

static const bcmltd_field_list_t
bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_psamp_metadata_ipv6_dst_list_d1 = {
    .field_num = 1,
    .field_array = bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_psamp_metadata_ipv6_dst_field_desc_d1
};

static const bcmltd_field_list_t
bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_sflow_dst_list_d2 = {
    .field_num = 1,
    .field_array = bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_sflow_dst_field_desc_d2
};

static const bcmltd_field_list_t
bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_sflow_ipv6_dst_list_d2 = {
    .field_num = 1,
    .field_array = bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_sflow_ipv6_dst_field_desc_d2
};

static const bcmltd_field_list_t
bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_sflow_seq_dst_list_d2 = {
    .field_num = 1,
    .field_array = bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_sflow_seq_dst_field_desc_d2
};

static const bcmltd_field_list_t
bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_sflow_seq_ipv6_dst_list_d2 = {
    .field_num = 1,
    .field_array = bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_sflow_seq_ipv6_dst_field_desc_d2
};

static const uint32_t
bcm56990_b0_lta_bcmltx_mirror_encap_udp_transform_src_s0[2] = {
    MIRROR_ENCAP_MIRROR_ON_DROPt_L4_SRC_PORTf,
    MIRROR_ENCAP_MIRROR_ON_DROPt_L4_DST_PORTf,
};

static const uint32_t
bcm56990_b0_lta_bcmltx_mirror_encap_udp_transform_src_s1[2] = {
    MIRROR_ENCAP_MIRROR_ON_DROP_IPV6t_L4_SRC_PORTf,
    MIRROR_ENCAP_MIRROR_ON_DROP_IPV6t_L4_DST_PORTf,
};

static const uint32_t
bcm56990_b0_lta_bcmltx_mirror_encap_udp_transform_src_s2[2] = {
    MIRROR_ENCAP_PSAMPt_L4_SRC_PORTf,
    MIRROR_ENCAP_PSAMPt_L4_DST_PORTf,
};

static const uint32_t
bcm56990_b0_lta_bcmltx_mirror_encap_udp_transform_src_s3[2] = {
    MIRROR_ENCAP_PSAMP_IPV6t_L4_SRC_PORTf,
    MIRROR_ENCAP_PSAMP_IPV6t_L4_DST_PORTf,
};

static const uint32_t
bcm56990_b0_lta_bcmltx_mirror_encap_udp_transform_src_s4[2] = {
    MIRROR_ENCAP_PSAMP_METADATAt_L4_SRC_PORTf,
    MIRROR_ENCAP_PSAMP_METADATAt_L4_DST_PORTf,
};

static const uint32_t
bcm56990_b0_lta_bcmltx_mirror_encap_udp_transform_src_s5[2] = {
    MIRROR_ENCAP_PSAMP_METADATA_IPV6t_L4_SRC_PORTf,
    MIRROR_ENCAP_PSAMP_METADATA_IPV6t_L4_DST_PORTf,
};

static const uint32_t
bcm56990_b0_lta_bcmltx_mirror_encap_udp_transform_src_s6[2] = {
    MIRROR_ENCAP_SFLOWt_L4_SRC_PORTf,
    MIRROR_ENCAP_SFLOWt_L4_DST_PORTf,
};

static const uint32_t
bcm56990_b0_lta_bcmltx_mirror_encap_udp_transform_src_s7[2] = {
    MIRROR_ENCAP_SFLOW_IPV6t_L4_SRC_PORTf,
    MIRROR_ENCAP_SFLOW_IPV6t_L4_DST_PORTf,
};

static const uint32_t
bcm56990_b0_lta_bcmltx_mirror_encap_udp_transform_src_s8[2] = {
    MIRROR_ENCAP_SFLOW_SEQt_L4_SRC_PORTf,
    MIRROR_ENCAP_SFLOW_SEQt_L4_DST_PORTf,
};

static const uint32_t
bcm56990_b0_lta_bcmltx_mirror_encap_udp_transform_src_s9[2] = {
    MIRROR_ENCAP_SFLOW_SEQ_IPV6t_L4_SRC_PORTf,
    MIRROR_ENCAP_SFLOW_SEQ_IPV6t_L4_DST_PORTf,
};

static const uint32_t
bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_mirror_on_drop_transform_dst_d0[1] = {
    PSAMPv_PSAMP_HEADER_UDPf,
};

static const uint32_t
bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_mirror_on_drop_ipv6_transform_dst_d1[1] = {
    PSAMP_V6v_PSAMP_HEADER_UDPf,
};

static const uint32_t
bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_psamp_transform_dst_d0[1] = {
    PSAMPv_PSAMP_HEADER_UDPf,
};

static const uint32_t
bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_psamp_ipv6_transform_dst_d1[1] = {
    PSAMP_V6v_PSAMP_HEADER_UDPf,
};

static const uint32_t
bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_psamp_metadata_transform_dst_d0[1] = {
    PSAMPv_PSAMP_HEADER_UDPf,
};

static const uint32_t
bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_psamp_metadata_ipv6_transform_dst_d1[1] = {
    PSAMP_V6v_PSAMP_HEADER_UDPf,
};

static const uint32_t
bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_sflow_transform_dst_d2[1] = {
    SFLOWv_SFLOW_HEADER_UDPf,
};

static const uint32_t
bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_sflow_ipv6_transform_dst_d2[1] = {
    SFLOWv_SFLOW_HEADER_UDPf,
};

static const uint32_t
bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_sflow_seq_transform_dst_d2[1] = {
    SFLOWv_SFLOW_HEADER_UDPf,
};

static const uint32_t
bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_sflow_seq_ipv6_transform_dst_d2[1] = {
    SFLOWv_SFLOW_HEADER_UDPf,
};

static const bcmltd_generic_arg_t
bcm56990_b0_lta_bcmltx_mirror_encap_udp_comp_data = {
    .sid       = MIRROR_ENCAP_MIRROR_ON_DROPt,
    .values    = 0,
    .value     = NULL,
    .user_data = NULL
};

static const bcmltd_generic_arg_t
bcm56990_b0_lta_bcmltx_mirror_encap_udp_comp_data1 = {
    .sid       = MIRROR_ENCAP_MIRROR_ON_DROP_IPV6t,
    .values    = 0,
    .value     = NULL,
    .user_data = NULL
};

static const bcmltd_generic_arg_t
bcm56990_b0_lta_bcmltx_mirror_encap_udp_comp_data2 = {
    .sid       = MIRROR_ENCAP_PSAMPt,
    .values    = 0,
    .value     = NULL,
    .user_data = NULL
};

static const bcmltd_generic_arg_t
bcm56990_b0_lta_bcmltx_mirror_encap_udp_comp_data3 = {
    .sid       = MIRROR_ENCAP_PSAMP_IPV6t,
    .values    = 0,
    .value     = NULL,
    .user_data = NULL
};

static const bcmltd_generic_arg_t
bcm56990_b0_lta_bcmltx_mirror_encap_udp_comp_data4 = {
    .sid       = MIRROR_ENCAP_PSAMP_METADATAt,
    .values    = 0,
    .value     = NULL,
    .user_data = NULL
};

static const bcmltd_generic_arg_t
bcm56990_b0_lta_bcmltx_mirror_encap_udp_comp_data5 = {
    .sid       = MIRROR_ENCAP_PSAMP_METADATA_IPV6t,
    .values    = 0,
    .value     = NULL,
    .user_data = NULL
};

static const bcmltd_generic_arg_t
bcm56990_b0_lta_bcmltx_mirror_encap_udp_comp_data6 = {
    .sid       = MIRROR_ENCAP_SFLOWt,
    .values    = 0,
    .value     = NULL,
    .user_data = NULL
};

static const bcmltd_generic_arg_t
bcm56990_b0_lta_bcmltx_mirror_encap_udp_comp_data7 = {
    .sid       = MIRROR_ENCAP_SFLOW_IPV6t,
    .values    = 0,
    .value     = NULL,
    .user_data = NULL
};

static const bcmltd_generic_arg_t
bcm56990_b0_lta_bcmltx_mirror_encap_udp_comp_data8 = {
    .sid       = MIRROR_ENCAP_SFLOW_SEQt,
    .values    = 0,
    .value     = NULL,
    .user_data = NULL
};

static const bcmltd_generic_arg_t
bcm56990_b0_lta_bcmltx_mirror_encap_udp_comp_data9 = {
    .sid       = MIRROR_ENCAP_SFLOW_SEQ_IPV6t,
    .values    = 0,
    .value     = NULL,
    .user_data = NULL
};

static const bcmltd_transform_arg_t
bcm56990_b0_lta_bcmltx_mirror_encap_udp_xfrm_handler_fwd_arg_s0_d0 = {
    .values      = 0,
    .value       = NULL,
    .tables      = 0,
    .table       = NULL,
    .fields      = 2,
    .field       = bcm56990_b0_lta_bcmltx_mirror_encap_udp_transform_src_s0,
    .field_list  = &bcm56990_b0_lta_bcmltx_mirror_encap_udp_src_list_s0,
    .rfields     = 1,
    .rfield      = bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_mirror_on_drop_transform_dst_d0,
    .rfield_list = &bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_mirror_on_drop_dst_list_d0,
    .comp_data   = &bcm56990_b0_lta_bcmltx_mirror_encap_udp_comp_data
};

static const bcmltd_transform_arg_t
bcm56990_b0_lta_bcmltx_mirror_encap_udp_xfrm_handler_rev_arg_s0_d0 = {
    .values      = 0,
    .value       = NULL,
    .tables      = 0,
    .table       = NULL,
    .fields      = 1,
    .field       = bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_mirror_on_drop_transform_dst_d0,
    .field_list  = &bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_mirror_on_drop_dst_list_d0,
    .rfields     = 2,
    .rfield      = bcm56990_b0_lta_bcmltx_mirror_encap_udp_transform_src_s0,
    .rfield_list = &bcm56990_b0_lta_bcmltx_mirror_encap_udp_src_list_s0,
    .comp_data   = &bcm56990_b0_lta_bcmltx_mirror_encap_udp_comp_data
};

static const bcmltd_transform_arg_t
bcm56990_b0_lta_bcmltx_mirror_encap_udp_xfrm_handler_fwd_arg_s1_d1 = {
    .values      = 0,
    .value       = NULL,
    .tables      = 0,
    .table       = NULL,
    .fields      = 2,
    .field       = bcm56990_b0_lta_bcmltx_mirror_encap_udp_transform_src_s1,
    .field_list  = &bcm56990_b0_lta_bcmltx_mirror_encap_udp_src_list_s1,
    .rfields     = 1,
    .rfield      = bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_mirror_on_drop_ipv6_transform_dst_d1,
    .rfield_list = &bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_mirror_on_drop_ipv6_dst_list_d1,
    .comp_data   = &bcm56990_b0_lta_bcmltx_mirror_encap_udp_comp_data1
};

static const bcmltd_transform_arg_t
bcm56990_b0_lta_bcmltx_mirror_encap_udp_xfrm_handler_rev_arg_s1_d1 = {
    .values      = 0,
    .value       = NULL,
    .tables      = 0,
    .table       = NULL,
    .fields      = 1,
    .field       = bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_mirror_on_drop_ipv6_transform_dst_d1,
    .field_list  = &bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_mirror_on_drop_ipv6_dst_list_d1,
    .rfields     = 2,
    .rfield      = bcm56990_b0_lta_bcmltx_mirror_encap_udp_transform_src_s1,
    .rfield_list = &bcm56990_b0_lta_bcmltx_mirror_encap_udp_src_list_s1,
    .comp_data   = &bcm56990_b0_lta_bcmltx_mirror_encap_udp_comp_data1
};

static const bcmltd_transform_arg_t
bcm56990_b0_lta_bcmltx_mirror_encap_udp_xfrm_handler_fwd_arg_s2_d0 = {
    .values      = 0,
    .value       = NULL,
    .tables      = 0,
    .table       = NULL,
    .fields      = 2,
    .field       = bcm56990_b0_lta_bcmltx_mirror_encap_udp_transform_src_s2,
    .field_list  = &bcm56990_b0_lta_bcmltx_mirror_encap_udp_src_list_s2,
    .rfields     = 1,
    .rfield      = bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_psamp_transform_dst_d0,
    .rfield_list = &bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_psamp_dst_list_d0,
    .comp_data   = &bcm56990_b0_lta_bcmltx_mirror_encap_udp_comp_data2
};

static const bcmltd_transform_arg_t
bcm56990_b0_lta_bcmltx_mirror_encap_udp_xfrm_handler_rev_arg_s2_d0 = {
    .values      = 0,
    .value       = NULL,
    .tables      = 0,
    .table       = NULL,
    .fields      = 1,
    .field       = bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_psamp_transform_dst_d0,
    .field_list  = &bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_psamp_dst_list_d0,
    .rfields     = 2,
    .rfield      = bcm56990_b0_lta_bcmltx_mirror_encap_udp_transform_src_s2,
    .rfield_list = &bcm56990_b0_lta_bcmltx_mirror_encap_udp_src_list_s2,
    .comp_data   = &bcm56990_b0_lta_bcmltx_mirror_encap_udp_comp_data2
};

static const bcmltd_transform_arg_t
bcm56990_b0_lta_bcmltx_mirror_encap_udp_xfrm_handler_fwd_arg_s3_d1 = {
    .values      = 0,
    .value       = NULL,
    .tables      = 0,
    .table       = NULL,
    .fields      = 2,
    .field       = bcm56990_b0_lta_bcmltx_mirror_encap_udp_transform_src_s3,
    .field_list  = &bcm56990_b0_lta_bcmltx_mirror_encap_udp_src_list_s3,
    .rfields     = 1,
    .rfield      = bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_psamp_ipv6_transform_dst_d1,
    .rfield_list = &bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_psamp_ipv6_dst_list_d1,
    .comp_data   = &bcm56990_b0_lta_bcmltx_mirror_encap_udp_comp_data3
};

static const bcmltd_transform_arg_t
bcm56990_b0_lta_bcmltx_mirror_encap_udp_xfrm_handler_rev_arg_s3_d1 = {
    .values      = 0,
    .value       = NULL,
    .tables      = 0,
    .table       = NULL,
    .fields      = 1,
    .field       = bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_psamp_ipv6_transform_dst_d1,
    .field_list  = &bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_psamp_ipv6_dst_list_d1,
    .rfields     = 2,
    .rfield      = bcm56990_b0_lta_bcmltx_mirror_encap_udp_transform_src_s3,
    .rfield_list = &bcm56990_b0_lta_bcmltx_mirror_encap_udp_src_list_s3,
    .comp_data   = &bcm56990_b0_lta_bcmltx_mirror_encap_udp_comp_data3
};

static const bcmltd_transform_arg_t
bcm56990_b0_lta_bcmltx_mirror_encap_udp_xfrm_handler_fwd_arg_s4_d0 = {
    .values      = 0,
    .value       = NULL,
    .tables      = 0,
    .table       = NULL,
    .fields      = 2,
    .field       = bcm56990_b0_lta_bcmltx_mirror_encap_udp_transform_src_s4,
    .field_list  = &bcm56990_b0_lta_bcmltx_mirror_encap_udp_src_list_s4,
    .rfields     = 1,
    .rfield      = bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_psamp_metadata_transform_dst_d0,
    .rfield_list = &bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_psamp_metadata_dst_list_d0,
    .comp_data   = &bcm56990_b0_lta_bcmltx_mirror_encap_udp_comp_data4
};

static const bcmltd_transform_arg_t
bcm56990_b0_lta_bcmltx_mirror_encap_udp_xfrm_handler_rev_arg_s4_d0 = {
    .values      = 0,
    .value       = NULL,
    .tables      = 0,
    .table       = NULL,
    .fields      = 1,
    .field       = bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_psamp_metadata_transform_dst_d0,
    .field_list  = &bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_psamp_metadata_dst_list_d0,
    .rfields     = 2,
    .rfield      = bcm56990_b0_lta_bcmltx_mirror_encap_udp_transform_src_s4,
    .rfield_list = &bcm56990_b0_lta_bcmltx_mirror_encap_udp_src_list_s4,
    .comp_data   = &bcm56990_b0_lta_bcmltx_mirror_encap_udp_comp_data4
};

static const bcmltd_transform_arg_t
bcm56990_b0_lta_bcmltx_mirror_encap_udp_xfrm_handler_fwd_arg_s5_d1 = {
    .values      = 0,
    .value       = NULL,
    .tables      = 0,
    .table       = NULL,
    .fields      = 2,
    .field       = bcm56990_b0_lta_bcmltx_mirror_encap_udp_transform_src_s5,
    .field_list  = &bcm56990_b0_lta_bcmltx_mirror_encap_udp_src_list_s5,
    .rfields     = 1,
    .rfield      = bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_psamp_metadata_ipv6_transform_dst_d1,
    .rfield_list = &bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_psamp_metadata_ipv6_dst_list_d1,
    .comp_data   = &bcm56990_b0_lta_bcmltx_mirror_encap_udp_comp_data5
};

static const bcmltd_transform_arg_t
bcm56990_b0_lta_bcmltx_mirror_encap_udp_xfrm_handler_rev_arg_s5_d1 = {
    .values      = 0,
    .value       = NULL,
    .tables      = 0,
    .table       = NULL,
    .fields      = 1,
    .field       = bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_psamp_metadata_ipv6_transform_dst_d1,
    .field_list  = &bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_psamp_metadata_ipv6_dst_list_d1,
    .rfields     = 2,
    .rfield      = bcm56990_b0_lta_bcmltx_mirror_encap_udp_transform_src_s5,
    .rfield_list = &bcm56990_b0_lta_bcmltx_mirror_encap_udp_src_list_s5,
    .comp_data   = &bcm56990_b0_lta_bcmltx_mirror_encap_udp_comp_data5
};

static const bcmltd_transform_arg_t
bcm56990_b0_lta_bcmltx_mirror_encap_udp_xfrm_handler_fwd_arg_s6_d2 = {
    .values      = 0,
    .value       = NULL,
    .tables      = 0,
    .table       = NULL,
    .fields      = 2,
    .field       = bcm56990_b0_lta_bcmltx_mirror_encap_udp_transform_src_s6,
    .field_list  = &bcm56990_b0_lta_bcmltx_mirror_encap_udp_src_list_s6,
    .rfields     = 1,
    .rfield      = bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_sflow_transform_dst_d2,
    .rfield_list = &bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_sflow_dst_list_d2,
    .comp_data   = &bcm56990_b0_lta_bcmltx_mirror_encap_udp_comp_data6
};

static const bcmltd_transform_arg_t
bcm56990_b0_lta_bcmltx_mirror_encap_udp_xfrm_handler_rev_arg_s6_d2 = {
    .values      = 0,
    .value       = NULL,
    .tables      = 0,
    .table       = NULL,
    .fields      = 1,
    .field       = bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_sflow_transform_dst_d2,
    .field_list  = &bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_sflow_dst_list_d2,
    .rfields     = 2,
    .rfield      = bcm56990_b0_lta_bcmltx_mirror_encap_udp_transform_src_s6,
    .rfield_list = &bcm56990_b0_lta_bcmltx_mirror_encap_udp_src_list_s6,
    .comp_data   = &bcm56990_b0_lta_bcmltx_mirror_encap_udp_comp_data6
};

static const bcmltd_transform_arg_t
bcm56990_b0_lta_bcmltx_mirror_encap_udp_xfrm_handler_fwd_arg_s7_d2 = {
    .values      = 0,
    .value       = NULL,
    .tables      = 0,
    .table       = NULL,
    .fields      = 2,
    .field       = bcm56990_b0_lta_bcmltx_mirror_encap_udp_transform_src_s7,
    .field_list  = &bcm56990_b0_lta_bcmltx_mirror_encap_udp_src_list_s7,
    .rfields     = 1,
    .rfield      = bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_sflow_ipv6_transform_dst_d2,
    .rfield_list = &bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_sflow_ipv6_dst_list_d2,
    .comp_data   = &bcm56990_b0_lta_bcmltx_mirror_encap_udp_comp_data7
};

static const bcmltd_transform_arg_t
bcm56990_b0_lta_bcmltx_mirror_encap_udp_xfrm_handler_rev_arg_s7_d2 = {
    .values      = 0,
    .value       = NULL,
    .tables      = 0,
    .table       = NULL,
    .fields      = 1,
    .field       = bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_sflow_ipv6_transform_dst_d2,
    .field_list  = &bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_sflow_ipv6_dst_list_d2,
    .rfields     = 2,
    .rfield      = bcm56990_b0_lta_bcmltx_mirror_encap_udp_transform_src_s7,
    .rfield_list = &bcm56990_b0_lta_bcmltx_mirror_encap_udp_src_list_s7,
    .comp_data   = &bcm56990_b0_lta_bcmltx_mirror_encap_udp_comp_data7
};

static const bcmltd_transform_arg_t
bcm56990_b0_lta_bcmltx_mirror_encap_udp_xfrm_handler_fwd_arg_s8_d2 = {
    .values      = 0,
    .value       = NULL,
    .tables      = 0,
    .table       = NULL,
    .fields      = 2,
    .field       = bcm56990_b0_lta_bcmltx_mirror_encap_udp_transform_src_s8,
    .field_list  = &bcm56990_b0_lta_bcmltx_mirror_encap_udp_src_list_s8,
    .rfields     = 1,
    .rfield      = bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_sflow_seq_transform_dst_d2,
    .rfield_list = &bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_sflow_seq_dst_list_d2,
    .comp_data   = &bcm56990_b0_lta_bcmltx_mirror_encap_udp_comp_data8
};

static const bcmltd_transform_arg_t
bcm56990_b0_lta_bcmltx_mirror_encap_udp_xfrm_handler_rev_arg_s8_d2 = {
    .values      = 0,
    .value       = NULL,
    .tables      = 0,
    .table       = NULL,
    .fields      = 1,
    .field       = bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_sflow_seq_transform_dst_d2,
    .field_list  = &bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_sflow_seq_dst_list_d2,
    .rfields     = 2,
    .rfield      = bcm56990_b0_lta_bcmltx_mirror_encap_udp_transform_src_s8,
    .rfield_list = &bcm56990_b0_lta_bcmltx_mirror_encap_udp_src_list_s8,
    .comp_data   = &bcm56990_b0_lta_bcmltx_mirror_encap_udp_comp_data8
};

static const bcmltd_transform_arg_t
bcm56990_b0_lta_bcmltx_mirror_encap_udp_xfrm_handler_fwd_arg_s9_d2 = {
    .values      = 0,
    .value       = NULL,
    .tables      = 0,
    .table       = NULL,
    .fields      = 2,
    .field       = bcm56990_b0_lta_bcmltx_mirror_encap_udp_transform_src_s9,
    .field_list  = &bcm56990_b0_lta_bcmltx_mirror_encap_udp_src_list_s9,
    .rfields     = 1,
    .rfield      = bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_sflow_seq_ipv6_transform_dst_d2,
    .rfield_list = &bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_sflow_seq_ipv6_dst_list_d2,
    .comp_data   = &bcm56990_b0_lta_bcmltx_mirror_encap_udp_comp_data9
};

static const bcmltd_transform_arg_t
bcm56990_b0_lta_bcmltx_mirror_encap_udp_xfrm_handler_rev_arg_s9_d2 = {
    .values      = 0,
    .value       = NULL,
    .tables      = 0,
    .table       = NULL,
    .fields      = 1,
    .field       = bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_sflow_seq_ipv6_transform_dst_d2,
    .field_list  = &bcm56990_b0_lrd_bcmltx_mirror_encap_udp_mirror_encap_sflow_seq_ipv6_dst_list_d2,
    .rfields     = 2,
    .rfield      = bcm56990_b0_lta_bcmltx_mirror_encap_udp_transform_src_s9,
    .rfield_list = &bcm56990_b0_lta_bcmltx_mirror_encap_udp_src_list_s9,
    .comp_data   = &bcm56990_b0_lta_bcmltx_mirror_encap_udp_comp_data9
};

const bcmltd_xfrm_handler_t
bcm56990_b0_lta_bcmltx_mirror_encap_udp_xfrm_handler_fwd_s0_d0 = {
    .transform     = bcmltx_mirror_encap_udp_transform,
    .arg           = &bcm56990_b0_lta_bcmltx_mirror_encap_udp_xfrm_handler_fwd_arg_s0_d0
};

const bcmltd_xfrm_handler_t
bcm56990_b0_lta_bcmltx_mirror_encap_udp_xfrm_handler_fwd_s1_d1 = {
    .transform     = bcmltx_mirror_encap_udp_transform,
    .arg           = &bcm56990_b0_lta_bcmltx_mirror_encap_udp_xfrm_handler_fwd_arg_s1_d1
};

const bcmltd_xfrm_handler_t
bcm56990_b0_lta_bcmltx_mirror_encap_udp_xfrm_handler_fwd_s2_d0 = {
    .transform     = bcmltx_mirror_encap_udp_transform,
    .arg           = &bcm56990_b0_lta_bcmltx_mirror_encap_udp_xfrm_handler_fwd_arg_s2_d0
};

const bcmltd_xfrm_handler_t
bcm56990_b0_lta_bcmltx_mirror_encap_udp_xfrm_handler_fwd_s3_d1 = {
    .transform     = bcmltx_mirror_encap_udp_transform,
    .arg           = &bcm56990_b0_lta_bcmltx_mirror_encap_udp_xfrm_handler_fwd_arg_s3_d1
};

const bcmltd_xfrm_handler_t
bcm56990_b0_lta_bcmltx_mirror_encap_udp_xfrm_handler_fwd_s4_d0 = {
    .transform     = bcmltx_mirror_encap_udp_transform,
    .arg           = &bcm56990_b0_lta_bcmltx_mirror_encap_udp_xfrm_handler_fwd_arg_s4_d0
};

const bcmltd_xfrm_handler_t
bcm56990_b0_lta_bcmltx_mirror_encap_udp_xfrm_handler_fwd_s5_d1 = {
    .transform     = bcmltx_mirror_encap_udp_transform,
    .arg           = &bcm56990_b0_lta_bcmltx_mirror_encap_udp_xfrm_handler_fwd_arg_s5_d1
};

const bcmltd_xfrm_handler_t
bcm56990_b0_lta_bcmltx_mirror_encap_udp_xfrm_handler_fwd_s6_d2 = {
    .transform     = bcmltx_mirror_encap_udp_transform,
    .arg           = &bcm56990_b0_lta_bcmltx_mirror_encap_udp_xfrm_handler_fwd_arg_s6_d2
};

const bcmltd_xfrm_handler_t
bcm56990_b0_lta_bcmltx_mirror_encap_udp_xfrm_handler_fwd_s7_d2 = {
    .transform     = bcmltx_mirror_encap_udp_transform,
    .arg           = &bcm56990_b0_lta_bcmltx_mirror_encap_udp_xfrm_handler_fwd_arg_s7_d2
};

const bcmltd_xfrm_handler_t
bcm56990_b0_lta_bcmltx_mirror_encap_udp_xfrm_handler_fwd_s8_d2 = {
    .transform     = bcmltx_mirror_encap_udp_transform,
    .arg           = &bcm56990_b0_lta_bcmltx_mirror_encap_udp_xfrm_handler_fwd_arg_s8_d2
};

const bcmltd_xfrm_handler_t
bcm56990_b0_lta_bcmltx_mirror_encap_udp_xfrm_handler_fwd_s9_d2 = {
    .transform     = bcmltx_mirror_encap_udp_transform,
    .arg           = &bcm56990_b0_lta_bcmltx_mirror_encap_udp_xfrm_handler_fwd_arg_s9_d2
};

const bcmltd_xfrm_handler_t
bcm56990_b0_lta_bcmltx_mirror_encap_udp_xfrm_handler_rev_s0_d0 = {
    .transform     = bcmltx_mirror_encap_udp_rev_transform,
    .arg           = &bcm56990_b0_lta_bcmltx_mirror_encap_udp_xfrm_handler_rev_arg_s0_d0
};

const bcmltd_xfrm_handler_t
bcm56990_b0_lta_bcmltx_mirror_encap_udp_xfrm_handler_rev_s1_d1 = {
    .transform     = bcmltx_mirror_encap_udp_rev_transform,
    .arg           = &bcm56990_b0_lta_bcmltx_mirror_encap_udp_xfrm_handler_rev_arg_s1_d1
};

const bcmltd_xfrm_handler_t
bcm56990_b0_lta_bcmltx_mirror_encap_udp_xfrm_handler_rev_s2_d0 = {
    .transform     = bcmltx_mirror_encap_udp_rev_transform,
    .arg           = &bcm56990_b0_lta_bcmltx_mirror_encap_udp_xfrm_handler_rev_arg_s2_d0
};

const bcmltd_xfrm_handler_t
bcm56990_b0_lta_bcmltx_mirror_encap_udp_xfrm_handler_rev_s3_d1 = {
    .transform     = bcmltx_mirror_encap_udp_rev_transform,
    .arg           = &bcm56990_b0_lta_bcmltx_mirror_encap_udp_xfrm_handler_rev_arg_s3_d1
};

const bcmltd_xfrm_handler_t
bcm56990_b0_lta_bcmltx_mirror_encap_udp_xfrm_handler_rev_s4_d0 = {
    .transform     = bcmltx_mirror_encap_udp_rev_transform,
    .arg           = &bcm56990_b0_lta_bcmltx_mirror_encap_udp_xfrm_handler_rev_arg_s4_d0
};

const bcmltd_xfrm_handler_t
bcm56990_b0_lta_bcmltx_mirror_encap_udp_xfrm_handler_rev_s5_d1 = {
    .transform     = bcmltx_mirror_encap_udp_rev_transform,
    .arg           = &bcm56990_b0_lta_bcmltx_mirror_encap_udp_xfrm_handler_rev_arg_s5_d1
};

const bcmltd_xfrm_handler_t
bcm56990_b0_lta_bcmltx_mirror_encap_udp_xfrm_handler_rev_s6_d2 = {
    .transform     = bcmltx_mirror_encap_udp_rev_transform,
    .arg           = &bcm56990_b0_lta_bcmltx_mirror_encap_udp_xfrm_handler_rev_arg_s6_d2
};

const bcmltd_xfrm_handler_t
bcm56990_b0_lta_bcmltx_mirror_encap_udp_xfrm_handler_rev_s7_d2 = {
    .transform     = bcmltx_mirror_encap_udp_rev_transform,
    .arg           = &bcm56990_b0_lta_bcmltx_mirror_encap_udp_xfrm_handler_rev_arg_s7_d2
};

const bcmltd_xfrm_handler_t
bcm56990_b0_lta_bcmltx_mirror_encap_udp_xfrm_handler_rev_s8_d2 = {
    .transform     = bcmltx_mirror_encap_udp_rev_transform,
    .arg           = &bcm56990_b0_lta_bcmltx_mirror_encap_udp_xfrm_handler_rev_arg_s8_d2
};

const bcmltd_xfrm_handler_t
bcm56990_b0_lta_bcmltx_mirror_encap_udp_xfrm_handler_rev_s9_d2 = {
    .transform     = bcmltx_mirror_encap_udp_rev_transform,
    .arg           = &bcm56990_b0_lta_bcmltx_mirror_encap_udp_xfrm_handler_rev_arg_s9_d2
};


