/*! \file bcm56880_a0_dna_4_7_11_pktio.c
 *
 * BCM56880_A0 DNA_4_7_11 PKTIO Driver.
 */
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

#include <bcm/pktio.h>
#include <bcm/pktio_defs.h>

#include <bcm_int/ltsw/mbcm/pktio.h>

#include <bcmpkt/chip/bcm56880_a0/dna_4_7_11/bcm56880_a0_dna_4_7_11_bcmpkt_flexhdr.h>
#include <bcmpkt/chip/bcm56880_a0/dna_4_7_11/bcm56880_a0_dna_4_7_11_bcmpkt_rxpmd_flex_data.h>
#include <bcmpkt/chip/bcm56880_a0/dna_4_7_11/bcm56880_a0_dna_4_7_11_bcmpkt_flexhdr_data.h>
#include <shr/shr_bitop.h>
#include <shr/shr_debug.h>

/******************************************************************************
 * Local definitions
 */

#define BSL_LOG_MODULE BSL_LS_BCM_PKTIO

static
bcmint_pktio_fid_info_t bcm56880_a0_dna_4_7_11_rxpmd_flex_db[] = {
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_TIMESTAMP_CTRL_3_0,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_TIMESTAMP_CTRL_3_0,
        .profile_bmp = 0x3c,
    },
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_SYSTEM_DESTINATION_15_0,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_SYSTEM_DESTINATION_15_0,
        .profile_bmp = 0x3c,
    },
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_SVP_NETWORK_GROUP_BITMAP_3_0,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_SVP_NETWORK_GROUP_BITMAP_3_0,
        .profile_bmp = 0x3c,
    },
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_I2E_CLASS_ID_15_0,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_I2E_CLASS_ID_15_0,
        .profile_bmp = 0x3c,
    },
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_PARSER_VHLEN_0_15_0,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_PARSER_VHLEN_0_15_0,
        .profile_bmp = 0x3c,
    },
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_VFI_15_0,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_VFI_15_0,
        .profile_bmp = 0x3c,
    },
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_SYSTEM_OPCODE_3_0,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_SYSTEM_OPCODE_3_0,
        .profile_bmp = 0x3c,
    },
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_IFP_TS_CONTROL_ACTION_3_0,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_IFP_TS_CONTROL_ACTION_3_0,
        .profile_bmp = 0x3c,
    },
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_L2_OIF_10_0,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_L2_OIF_10_0,
        .profile_bmp = 0x28,
    },
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_SYSTEM_SOURCE_15_0,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_SYSTEM_SOURCE_15_0,
        .profile_bmp = 0x3c,
    },
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_ENTROPY_LABEL_LOW_15_0,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_ENTROPY_LABEL_LOW_15_0,
        .profile_bmp = 0x3c,
    },
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_INT_PRI_3_0,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_INT_PRI_3_0,
        .profile_bmp = 0x3c,
    },
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_VLAN_TAG_PRESERVE_CTRL_SVP_MIRROR_ENABLE_1_0,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_VLAN_TAG_PRESERVE_CTRL_SVP_MIRROR_ENABLE_1_0,
        .profile_bmp = 0x3c,
    },
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_INGRESS_PP_PORT_7_0,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_INGRESS_PP_PORT_7_0,
        .profile_bmp = 0x3c,
    },
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_PKT_MISC_CTRL_0_3_0,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_PKT_MISC_CTRL_0_3_0,
        .profile_bmp = 0x3c,
    },
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_DLB_ECMP_DESTINATION_15_0,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_DLB_ECMP_DESTINATION_15_0,
        .profile_bmp = 0x30,
    },
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_NHOP_2_OR_ECMP_GROUP_INDEX_1_15_0,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_NHOP_2_OR_ECMP_GROUP_INDEX_1_15_0,
        .profile_bmp = 0x3c,
    },
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_DROP_CODE_15_0,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_DROP_CODE_15_0,
        .profile_bmp = 0x3c,
    },
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_SVP_15_0,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_SVP_15_0,
        .profile_bmp = 0x3c,
    },
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_DVP_15_0,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_DVP_15_0,
        .profile_bmp = 0x3c,
    },
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_L3_OIF_1_13_0,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_L3_OIF_1_13_0,
        .profile_bmp = 0x3c,
    },
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_NHOP_INDEX_1_15_0,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_NHOP_INDEX_1_15_0,
        .profile_bmp = 0x3c,
    },
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_TUNNEL_PROCESSING_RESULTS_1_3_0,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_TUNNEL_PROCESSING_RESULTS_1_3_0,
        .profile_bmp = 0x3c,
    },
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_L3_IIF_13_0,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_L3_IIF_13_0,
        .profile_bmp = 0x3c,
    },
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_ENTROPY_LABEL_HIGH_3_0,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_ENTROPY_LABEL_HIGH_3_0,
        .profile_bmp = 0x3c,
    },
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_EFFECTIVE_TTL_7_0,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_EFFECTIVE_TTL_7_0,
        .profile_bmp = 0x3c,
    },
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_L2_IIF_10_0,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_L2_IIF_10_0,
        .profile_bmp = 0x14,
    },
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_INGRESS_QOS_REMAP_VALUE_OR_IFP_OPAQUE_OBJ_15_0,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_INGRESS_QOS_REMAP_VALUE_OR_IFP_OPAQUE_OBJ_15_0,
        .profile_bmp = 0x3c,
    },
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_INGRESS_QOS_REMARK_CTRL_3_0,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_INGRESS_QOS_REMARK_CTRL_3_0,
        .profile_bmp = 0x3c,
    },
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_EM_FT_OPAQUE_OBJ_OR_IFP_OPAQUE_OBJ_15_0,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_EM_FT_OPAQUE_OBJ_OR_IFP_OPAQUE_OBJ_15_0,
        .profile_bmp = 0xc,
    },
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_IFP_IOAM_GBP_ACTION_3_0,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_IFP_IOAM_GBP_ACTION_3_0,
        .profile_bmp = 0x3c,
    },
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_EVENT_TRACE_VECTOR_31_0,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_EVENT_TRACE_VECTOR_31_0,
        .profile_bmp = 0x3c,
    },
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_ING_TIMESTAMP_31_0,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_ING_TIMESTAMP_31_0,
        .profile_bmp = 0x3c,
    },
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_MPLS_LABEL_DECAP_COUNT_3_0,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_MPLS_LABEL_DECAP_COUNT_3_0,
        .profile_bmp = 0x3c,
    },
    {
        /* Always last. */
        .fid = BCM_PKTIO_FID_INVALID,
    },
};

static
bcmint_pktio_fid_info_t bcm56880_a0_dna_4_7_11_generic_loopback_db[] = {
    {
        .fid = BCM_PKTIO_GENERIC_LOOPBACK_T_INPUT_PRIORITY,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_GENERIC_LOOPBACK_T_INPUT_PRIORITY,
    },
    {
        .fid = BCM_PKTIO_GENERIC_LOOPBACK_T_RESERVED_1,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_GENERIC_LOOPBACK_T_RESERVED_1,
    },
    {
        .fid = BCM_PKTIO_GENERIC_LOOPBACK_T_START_BYTE,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_GENERIC_LOOPBACK_T_START_BYTE,
    },
    {
        .fid = BCM_PKTIO_GENERIC_LOOPBACK_T_INTERFACE_OBJ,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_GENERIC_LOOPBACK_T_INTERFACE_OBJ,
    },
    {
        .fid = BCM_PKTIO_GENERIC_LOOPBACK_T_PROCESSING_CTRL_1,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_GENERIC_LOOPBACK_T_PROCESSING_CTRL_1,
    },
    {
        .fid = BCM_PKTIO_GENERIC_LOOPBACK_T_PROCESSING_CTRL_0,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_GENERIC_LOOPBACK_T_PROCESSING_CTRL_0,
    },
    {
        .fid = BCM_PKTIO_GENERIC_LOOPBACK_T_DESTINATION_OBJ,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_GENERIC_LOOPBACK_T_DESTINATION_OBJ,
    },
    {
        .fid = BCM_PKTIO_GENERIC_LOOPBACK_T_SOURCE_SYSTEM_PORT,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_GENERIC_LOOPBACK_T_SOURCE_SYSTEM_PORT,
    },
    {
        .fid = BCM_PKTIO_GENERIC_LOOPBACK_T_INTERFACE_CTRL,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_GENERIC_LOOPBACK_T_INTERFACE_CTRL,
    },
    {
        .fid = BCM_PKTIO_GENERIC_LOOPBACK_T_HEADER_TYPE,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_GENERIC_LOOPBACK_T_HEADER_TYPE,
    },
    {
        .fid = BCM_PKTIO_GENERIC_LOOPBACK_T_ENTROPY_OBJ,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_GENERIC_LOOPBACK_T_ENTROPY_OBJ,
    },
    {
        .fid = BCM_PKTIO_GENERIC_LOOPBACK_T_DESTINATION_TYPE,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_GENERIC_LOOPBACK_T_DESTINATION_TYPE,
    },
    {
        .fid = BCM_PKTIO_GENERIC_LOOPBACK_T_RESERVED_2,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_GENERIC_LOOPBACK_T_RESERVED_2,
    },
    {
        .fid = BCM_PKTIO_GENERIC_LOOPBACK_T_FLAGS,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_GENERIC_LOOPBACK_T_FLAGS,
    },
    {
        .fid = BCM_PKTIO_GENERIC_LOOPBACK_T_QOS_OBJ,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_GENERIC_LOOPBACK_T_QOS_OBJ,
    },
    {
        /* Always last. */
        .fid = BCM_PKTIO_FID_INVALID,
    },
};

static
bcmint_pktio_fid_info_t bcm56880_a0_dna_4_7_11_hg3_db[] = {
    {
        .fid = BCM_PKTIO_HG3_BASE_T_SYSTEM_SOURCE,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_HG3_BASE_T_SYSTEM_SOURCE,
    },
    {
        .fid = BCM_PKTIO_HG3_BASE_T_EXT_HDR_PRESENT,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_HG3_BASE_T_EXT_HDR_PRESENT,
    },
    {
        .fid = BCM_PKTIO_HG3_BASE_T_CN,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_HG3_BASE_T_CN,
    },
    {
        .fid = BCM_PKTIO_HG3_BASE_T_HG3_RESERVED,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_HG3_BASE_T_HG3_RESERVED,
    },
    {
        .fid = BCM_PKTIO_HG3_BASE_T_MIRROR_COPY,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_HG3_BASE_T_MIRROR_COPY,
    },
    {
        .fid = BCM_PKTIO_HG3_BASE_T_L3_ROUTED,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_HG3_BASE_T_L3_ROUTED,
    },
    {
        .fid = BCM_PKTIO_HG3_BASE_T_CNG,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_HG3_BASE_T_CNG,
    },
    {
        .fid = BCM_PKTIO_HG3_BASE_T_RESERVED_ETYPE,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_HG3_BASE_T_RESERVED_ETYPE,
    },
    {
        .fid = BCM_PKTIO_HG3_BASE_T_VERSION,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_HG3_BASE_T_VERSION,
    },
    {
        .fid = BCM_PKTIO_HG3_BASE_T_SYSTEM_DESTINATION,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_HG3_BASE_T_SYSTEM_DESTINATION,
    },
    {
        .fid = BCM_PKTIO_HG3_BASE_T_ENTROPY,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_HG3_BASE_T_ENTROPY,
    },
    {
        .fid = BCM_PKTIO_HG3_BASE_T_TC,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_HG3_BASE_T_TC,
    },
    {
        .fid = BCM_PKTIO_HG3_BASE_T_SYSTEM_DESTINATION_TYPE,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_HG3_BASE_T_SYSTEM_DESTINATION_TYPE,
    },
    {
        .fid = BCM_PKTIO_HG3_EXTENSION_0_T_FORWARDING_DOMAIN,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_HG3_EXTENSION_0_T_FORWARDING_DOMAIN,
    },
    {
        .fid = BCM_PKTIO_HG3_EXTENSION_0_T_DVP_OR_L3_IIF,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_HG3_EXTENSION_0_T_DVP_OR_L3_IIF,
    },
    {
        .fid = BCM_PKTIO_HG3_EXTENSION_0_T_CLASS_ID,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_HG3_EXTENSION_0_T_CLASS_ID,
    },
    {
        .fid = BCM_PKTIO_HG3_EXTENSION_0_T_FLAGS,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_HG3_EXTENSION_0_T_FLAGS,
    },
    {
        .fid = BCM_PKTIO_HG3_EXTENSION_0_T_SVP,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_HG3_EXTENSION_0_T_SVP,
    },
    {
        /* Always last. */
        .fid = BCM_PKTIO_FID_INVALID,
    },
};

static
bcmint_pktio_fid_info_t bcm56880_a0_dna_4_7_11_rxpmd_flex_reason_db[] = {
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_REASON_CML_FLAGS,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_REASON_CML_FLAGS,
    },
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_REASON_L2_SRC_STATIC_MOVE,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_REASON_L2_SRC_STATIC_MOVE,
    },
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_REASON_PKT_INTEGRITY_CHECK_FAILED,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_REASON_PKT_INTEGRITY_CHECK_FAILED,
    },
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_REASON_PROTOCOL_PKT,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_REASON_PROTOCOL_PKT,
    },
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_REASON_L2_DST_LOOKUP_MISS,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_REASON_L2_DST_LOOKUP_MISS,
    },
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_REASON_L2_DST_LOOKUP,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_REASON_L2_DST_LOOKUP,
    },
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_REASON_L3_DST_LOOKUP_MISS,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_REASON_L3_DST_LOOKUP_MISS,
    },
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_REASON_L3_DST_LOOKUP,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_REASON_L3_DST_LOOKUP,
    },
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_REASON_L3_HDR_ERROR,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_REASON_L3_HDR_ERROR,
    },
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_REASON_L3_TTL_ERROR,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_REASON_L3_TTL_ERROR,
    },
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_REASON_IPMC_L3_IIF_OR_RPA_ID_CHECK_FAILED,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_REASON_IPMC_L3_IIF_OR_RPA_ID_CHECK_FAILED,
    },
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_REASON_LEARN_CACHE_FULL,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_REASON_LEARN_CACHE_FULL,
    },
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_REASON_VFP,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_REASON_VFP,
    },
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_REASON_IFP,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_REASON_IFP,
    },
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_REASON_IFP_METER,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_REASON_IFP_METER,
    },
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_REASON_DST_FP,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_REASON_DST_FP,
    },
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_REASON_SVP,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_REASON_SVP,
    },
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_REASON_EM_FT,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_REASON_EM_FT,
    },
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_REASON_IVXLT,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_REASON_IVXLT,
    },
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_REASON_MATCHED_RULE_BIT_0,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_REASON_MATCHED_RULE_BIT_0,
    },
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_REASON_MATCHED_RULE_BIT_1,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_REASON_MATCHED_RULE_BIT_1,
    },
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_REASON_MATCHED_RULE_BIT_2,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_REASON_MATCHED_RULE_BIT_2,
    },
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_REASON_MATCHED_RULE_BIT_3,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_REASON_MATCHED_RULE_BIT_3,
    },
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_REASON_MATCHED_RULE_BIT_4,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_REASON_MATCHED_RULE_BIT_4,
    },
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_REASON_MATCHED_RULE_BIT_5,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_REASON_MATCHED_RULE_BIT_5,
    },
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_REASON_MATCHED_RULE_BIT_6,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_REASON_MATCHED_RULE_BIT_6,
    },
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_REASON_MATCHED_RULE_BIT_7,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_REASON_MATCHED_RULE_BIT_7,
    },
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_REASON_NO_COPY_TO_CPU,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_REASON_NO_COPY_TO_CPU,
    },
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_REASON_L2_SRC_DISCARD,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_REASON_L2_SRC_DISCARD,
    },
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_REASON_MACSA_MULTICAST,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_REASON_MACSA_MULTICAST,
    },
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_REASON_MEMBERSHIP_CHECK_FAILED,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_REASON_MEMBERSHIP_CHECK_FAILED,
    },
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_REASON_SPANNING_TREE_CHECK_FAILED,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_REASON_SPANNING_TREE_CHECK_FAILED,
    },
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_REASON_MIRROR_SAMPLER_SAMPLED,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_REASON_MIRROR_SAMPLER_SAMPLED,
    },
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_REASON_MIRROR_SAMPLER_EGR_SAMPLED,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_REASON_MIRROR_SAMPLER_EGR_SAMPLED,
    },
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_REASON_SER_DROP,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_REASON_SER_DROP,
    },
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_REASON_URPF_CHECK_FAILED,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_REASON_URPF_CHECK_FAILED,
    },
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_REASON_L3_IIF_EQ_L3_OIF,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_REASON_L3_IIF_EQ_L3_OIF,
    },
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_REASON_DLB_ECMP_MONITOR_EN_OR_MEMBER_REASSINED,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_REASON_DLB_ECMP_MONITOR_EN_OR_MEMBER_REASSINED,
    },
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_REASON_DLB_LAG_MONITOR_EN_OR_MEMBER_REASSINED,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_REASON_DLB_LAG_MONITOR_EN_OR_MEMBER_REASSINED,
    },
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_REASON_MPLS_CTRL_PKT_TO_CPU,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_REASON_MPLS_CTRL_PKT_TO_CPU,
    },
    {
        .fid = BCM_PKTIO_RXPMD_FLEX_REASON_TRACE_DOP,
        .lt_fid = BCM56880_A0_DNA_4_7_11_BCMPKT_RXPMD_FLEX_REASON_TRACE_DOP,
    },
    {
        /* Always last. */
        .fid = BCM_PKTIO_FID_INVALID,
    },
};

static
bcmint_pktio_fid_info_t bcm56880_a0_dna_4_7_11_match_id_db[] = {
    {
        .fid = BCM_PKTIO_MATCH_ID_INNER_L2_HDR_ETAG,
        .lt_str = "INGRESS_PKT_INNER_L2_HDR_ETAG",
    },
    {
        .fid = BCM_PKTIO_MATCH_ID_INNER_L2_HDR_ITAG,
        .lt_str = "INGRESS_PKT_INNER_L2_HDR_ITAG",
    },
    {
        .fid = BCM_PKTIO_MATCH_ID_INNER_L2_HDR_L2,
        .lt_str = "INGRESS_PKT_INNER_L2_HDR_L2",
    },
    {
        .fid = BCM_PKTIO_MATCH_ID_INNER_L2_HDR_OTAG,
        .lt_str = "INGRESS_PKT_INNER_L2_HDR_OTAG",
    },
    {
        .fid = BCM_PKTIO_MATCH_ID_INNER_L2_HDR_SNAP_OR_LLC,
        .lt_str = "INGRESS_PKT_INNER_L2_HDR_SNAP_OR_LLC",
    },
    {
        .fid = BCM_PKTIO_MATCH_ID_INNER_L2_HDR_VNTAG,
        .lt_str = "INGRESS_PKT_INNER_L2_HDR_VNTAG",
    },
    {
        .fid = BCM_PKTIO_MATCH_ID_INNER_L3_L4_HDR_ARP,
        .lt_str = "INGRESS_PKT_INNER_L3_L4_HDR_ARP",
    },
    {
        .fid = BCM_PKTIO_MATCH_ID_INNER_L3_L4_HDR_AUTH_EXT_1,
        .lt_str = "INGRESS_PKT_INNER_L3_L4_HDR_AUTH_EXT_1",
    },
    {
        .fid = BCM_PKTIO_MATCH_ID_INNER_L3_L4_HDR_AUTH_EXT_2,
        .lt_str = "INGRESS_PKT_INNER_L3_L4_HDR_AUTH_EXT_2",
    },
    {
        .fid = BCM_PKTIO_MATCH_ID_INNER_L3_L4_HDR_BFD,
        .lt_str = "INGRESS_PKT_INNER_L3_L4_HDR_BFD",
    },
    {
        .fid = BCM_PKTIO_MATCH_ID_INNER_L3_L4_HDR_ESP_EXT,
        .lt_str = "INGRESS_PKT_INNER_L3_L4_HDR_ESP_EXT",
    },
    {
        .fid = BCM_PKTIO_MATCH_ID_INNER_L3_L4_HDR_ETHERTYPE,
        .lt_str = "INGRESS_PKT_INNER_L3_L4_HDR_ETHERTYPE",
    },
    {
        .fid = BCM_PKTIO_MATCH_ID_INNER_L3_L4_HDR_FRAG_EXT_1,
        .lt_str = "INGRESS_PKT_INNER_L3_L4_HDR_FRAG_EXT_1",
    },
    {
        .fid = BCM_PKTIO_MATCH_ID_INNER_L3_L4_HDR_FRAG_EXT_2,
        .lt_str = "INGRESS_PKT_INNER_L3_L4_HDR_FRAG_EXT_2",
    },
    {
        .fid = BCM_PKTIO_MATCH_ID_INNER_L3_L4_HDR_ICMP,
        .lt_str = "INGRESS_PKT_INNER_L3_L4_HDR_ICMP",
    },
    {
        .fid = BCM_PKTIO_MATCH_ID_INNER_L3_L4_HDR_IGMP,
        .lt_str = "INGRESS_PKT_INNER_L3_L4_HDR_IGMP",
    },
    {
        .fid = BCM_PKTIO_MATCH_ID_INNER_L3_L4_HDR_IPV4,
        .lt_str = "INGRESS_PKT_INNER_L3_L4_HDR_IPV4",
    },
    {
        .fid = BCM_PKTIO_MATCH_ID_INNER_L3_L4_HDR_IPV6,
        .lt_str = "INGRESS_PKT_INNER_L3_L4_HDR_IPV6",
    },
    {
        .fid = BCM_PKTIO_MATCH_ID_INNER_L3_L4_HDR_P_1588,
        .lt_str = "INGRESS_PKT_INNER_L3_L4_HDR_P_1588",
    },
    {
        .fid = BCM_PKTIO_MATCH_ID_INNER_L3_L4_HDR_RARP,
        .lt_str = "INGRESS_PKT_INNER_L3_L4_HDR_RARP",
    },
    {
        .fid = BCM_PKTIO_MATCH_ID_INNER_L3_L4_HDR_TCP_FIRST_4BYTES,
        .lt_str = "INGRESS_PKT_INNER_L3_L4_HDR_TCP_FIRST_4BYTES",
    },
    {
        .fid = BCM_PKTIO_MATCH_ID_INNER_L3_L4_HDR_TCP_LAST_16BYTES,
        .lt_str = "INGRESS_PKT_INNER_L3_L4_HDR_TCP_LAST_16BYTES",
    },
    {
        .fid = BCM_PKTIO_MATCH_ID_INNER_L3_L4_HDR_UDP,
        .lt_str = "INGRESS_PKT_INNER_L3_L4_HDR_UDP",
    },
    {
        .fid = BCM_PKTIO_MATCH_ID_INNER_L3_L4_HDR_UNKNOWN_L3,
        .lt_str = "INGRESS_PKT_INNER_L3_L4_HDR_UNKNOWN_L3",
    },
    {
        .fid = BCM_PKTIO_MATCH_ID_INNER_L3_L4_HDR_UNKNOWN_L4,
        .lt_str = "INGRESS_PKT_INNER_L3_L4_HDR_UNKNOWN_L4",
    },
    {
        .fid = BCM_PKTIO_MATCH_ID_INNER_L3_L4_HDR_UNKNOWN_L5,
        .lt_str = "INGRESS_PKT_INNER_L3_L4_HDR_UNKNOWN_L5",
    },
    {
        .fid = BCM_PKTIO_MATCH_ID_OUTER_L2_HDR_ETAG,
        .lt_str = "INGRESS_PKT_OUTER_L2_HDR_ETAG",
    },
    {
        .fid = BCM_PKTIO_MATCH_ID_OUTER_L2_HDR_HG3_BASE,
        .lt_str = "INGRESS_PKT_OUTER_L2_HDR_HG3_BASE",
    },
    {
        .fid = BCM_PKTIO_MATCH_ID_OUTER_L2_HDR_HG3_EXT_0,
        .lt_str = "INGRESS_PKT_OUTER_L2_HDR_HG3_EXT_0",
    },
    {
        .fid = BCM_PKTIO_MATCH_ID_OUTER_L2_HDR_ITAG,
        .lt_str = "INGRESS_PKT_OUTER_L2_HDR_ITAG",
    },
    {
        .fid = BCM_PKTIO_MATCH_ID_OUTER_L2_HDR_L2,
        .lt_str = "INGRESS_PKT_OUTER_L2_HDR_L2",
    },
    {
        .fid = BCM_PKTIO_MATCH_ID_OUTER_L2_HDR_OTAG,
        .lt_str = "INGRESS_PKT_OUTER_L2_HDR_OTAG",
    },
    {
        .fid = BCM_PKTIO_MATCH_ID_OUTER_L2_HDR_SNAP_OR_LLC,
        .lt_str = "INGRESS_PKT_OUTER_L2_HDR_SNAP_OR_LLC",
    },
    {
        .fid = BCM_PKTIO_MATCH_ID_OUTER_L2_HDR_VNTAG,
        .lt_str = "INGRESS_PKT_OUTER_L2_HDR_VNTAG",
    },
    {
        .fid = BCM_PKTIO_MATCH_ID_OUTER_L3_L4_HDR_ARP,
        .lt_str = "INGRESS_PKT_OUTER_L3_L4_HDR_ARP",
    },
    {
        .fid = BCM_PKTIO_MATCH_ID_OUTER_L3_L4_HDR_AUTH_EXT_1,
        .lt_str = "INGRESS_PKT_OUTER_L3_L4_HDR_AUTH_EXT_1",
    },
    {
        .fid = BCM_PKTIO_MATCH_ID_OUTER_L3_L4_HDR_AUTH_EXT_2,
        .lt_str = "INGRESS_PKT_OUTER_L3_L4_HDR_AUTH_EXT_2",
    },
    {
        .fid = BCM_PKTIO_MATCH_ID_OUTER_L3_L4_HDR_BFD,
        .lt_str = "INGRESS_PKT_OUTER_L3_L4_HDR_BFD",
    },
    {
        .fid = BCM_PKTIO_MATCH_ID_OUTER_L3_L4_HDR_ESP_EXT,
        .lt_str = "INGRESS_PKT_OUTER_L3_L4_HDR_ESP_EXT",
    },
    {
        .fid = BCM_PKTIO_MATCH_ID_OUTER_L3_L4_HDR_ETHERTYPE,
        .lt_str = "INGRESS_PKT_OUTER_L3_L4_HDR_ETHERTYPE",
    },
    {
        .fid = BCM_PKTIO_MATCH_ID_OUTER_L3_L4_HDR_FRAG_EXT_1,
        .lt_str = "INGRESS_PKT_OUTER_L3_L4_HDR_FRAG_EXT_1",
    },
    {
        .fid = BCM_PKTIO_MATCH_ID_OUTER_L3_L4_HDR_FRAG_EXT_2,
        .lt_str = "INGRESS_PKT_OUTER_L3_L4_HDR_FRAG_EXT_2",
    },
    {
        .fid = BCM_PKTIO_MATCH_ID_OUTER_L3_L4_HDR_GPE,
        .lt_str = "INGRESS_PKT_OUTER_L3_L4_HDR_GPE",
    },
    {
        .fid = BCM_PKTIO_MATCH_ID_OUTER_L3_L4_HDR_GRE,
        .lt_str = "INGRESS_PKT_OUTER_L3_L4_HDR_GRE",
    },
    {
        .fid = BCM_PKTIO_MATCH_ID_OUTER_L3_L4_HDR_GRE_CHKSUM,
        .lt_str = "INGRESS_PKT_OUTER_L3_L4_HDR_GRE_CHKSUM",
    },
    {
        .fid = BCM_PKTIO_MATCH_ID_OUTER_L3_L4_HDR_GRE_KEY,
        .lt_str = "INGRESS_PKT_OUTER_L3_L4_HDR_GRE_KEY",
    },
    {
        .fid = BCM_PKTIO_MATCH_ID_OUTER_L3_L4_HDR_GRE_ROUT,
        .lt_str = "INGRESS_PKT_OUTER_L3_L4_HDR_GRE_ROUT",
    },
    {
        .fid = BCM_PKTIO_MATCH_ID_OUTER_L3_L4_HDR_GRE_SEQ,
        .lt_str = "INGRESS_PKT_OUTER_L3_L4_HDR_GRE_SEQ",
    },
    {
        .fid = BCM_PKTIO_MATCH_ID_OUTER_L3_L4_HDR_ICMP,
        .lt_str = "INGRESS_PKT_OUTER_L3_L4_HDR_ICMP",
    },
    {
        .fid = BCM_PKTIO_MATCH_ID_OUTER_L3_L4_HDR_IFA_METADATA,
        .lt_str = "INGRESS_PKT_OUTER_L3_L4_HDR_IFA_METADATA",
    },
    {
        .fid = BCM_PKTIO_MATCH_ID_OUTER_L3_L4_HDR_IGMP,
        .lt_str = "INGRESS_PKT_OUTER_L3_L4_HDR_IGMP",
    },
    {
        .fid = BCM_PKTIO_MATCH_ID_OUTER_L3_L4_HDR_IPV4,
        .lt_str = "INGRESS_PKT_OUTER_L3_L4_HDR_IPV4",
    },
    {
        .fid = BCM_PKTIO_MATCH_ID_OUTER_L3_L4_HDR_IPV6,
        .lt_str = "INGRESS_PKT_OUTER_L3_L4_HDR_IPV6",
    },
    {
        .fid = BCM_PKTIO_MATCH_ID_OUTER_L3_L4_HDR_MPLS0,
        .lt_str = "INGRESS_PKT_OUTER_L3_L4_HDR_MPLS0",
    },
    {
        .fid = BCM_PKTIO_MATCH_ID_OUTER_L3_L4_HDR_MPLS1,
        .lt_str = "INGRESS_PKT_OUTER_L3_L4_HDR_MPLS1",
    },
    {
        .fid = BCM_PKTIO_MATCH_ID_OUTER_L3_L4_HDR_MPLS2,
        .lt_str = "INGRESS_PKT_OUTER_L3_L4_HDR_MPLS2",
    },
    {
        .fid = BCM_PKTIO_MATCH_ID_OUTER_L3_L4_HDR_MPLS3,
        .lt_str = "INGRESS_PKT_OUTER_L3_L4_HDR_MPLS3",
    },
    {
        .fid = BCM_PKTIO_MATCH_ID_OUTER_L3_L4_HDR_MPLS4,
        .lt_str = "INGRESS_PKT_OUTER_L3_L4_HDR_MPLS4",
    },
    {
        .fid = BCM_PKTIO_MATCH_ID_OUTER_L3_L4_HDR_MPLS5,
        .lt_str = "INGRESS_PKT_OUTER_L3_L4_HDR_MPLS5",
    },
    {
        .fid = BCM_PKTIO_MATCH_ID_OUTER_L3_L4_HDR_MPLS6,
        .lt_str = "INGRESS_PKT_OUTER_L3_L4_HDR_MPLS6",
    },
    {
        .fid = BCM_PKTIO_MATCH_ID_OUTER_L3_L4_HDR_MPLS_ACH,
        .lt_str = "INGRESS_PKT_OUTER_L3_L4_HDR_MPLS_ACH",
    },
    {
        .fid = BCM_PKTIO_MATCH_ID_OUTER_L3_L4_HDR_MPLS_CW,
        .lt_str = "INGRESS_PKT_OUTER_L3_L4_HDR_MPLS_CW",
    },
    {
        .fid = BCM_PKTIO_MATCH_ID_OUTER_L3_L4_HDR_P_1588,
        .lt_str = "INGRESS_PKT_OUTER_L3_L4_HDR_P_1588",
    },
    {
        .fid = BCM_PKTIO_MATCH_ID_OUTER_L3_L4_HDR_RARP,
        .lt_str = "INGRESS_PKT_OUTER_L3_L4_HDR_RARP",
    },
    {
        .fid = BCM_PKTIO_MATCH_ID_OUTER_L3_L4_HDR_TCP_FIRST_4BYTES,
        .lt_str = "INGRESS_PKT_OUTER_L3_L4_HDR_TCP_FIRST_4BYTES",
    },
    {
        .fid = BCM_PKTIO_MATCH_ID_OUTER_L3_L4_HDR_TCP_LAST_16BYTES,
        .lt_str = "INGRESS_PKT_OUTER_L3_L4_HDR_TCP_LAST_16BYTES",
    },
    {
        .fid = BCM_PKTIO_MATCH_ID_OUTER_L3_L4_HDR_UDP,
        .lt_str = "INGRESS_PKT_OUTER_L3_L4_HDR_UDP",
    },
    {
        .fid = BCM_PKTIO_MATCH_ID_OUTER_L3_L4_HDR_UNKNOWN_L3,
        .lt_str = "INGRESS_PKT_OUTER_L3_L4_HDR_UNKNOWN_L3",
    },
    {
        .fid = BCM_PKTIO_MATCH_ID_OUTER_L3_L4_HDR_UNKNOWN_L4,
        .lt_str = "INGRESS_PKT_OUTER_L3_L4_HDR_UNKNOWN_L4",
    },
    {
        .fid = BCM_PKTIO_MATCH_ID_OUTER_L3_L4_HDR_UNKNOWN_L5,
        .lt_str = "INGRESS_PKT_OUTER_L3_L4_HDR_UNKNOWN_L5",
    },
    {
        .fid = BCM_PKTIO_MATCH_ID_OUTER_L3_L4_HDR_VXLAN,
        .lt_str = "INGRESS_PKT_OUTER_L3_L4_HDR_VXLAN",
    },
    {
        .fid = BCM_PKTIO_MATCH_ID_SYS_HDR_LOOPBACK,
        .lt_str = "INGRESS_PKT_SYS_HDR_LOOPBACK",
    },
    {
        /* Always last. */
        .fid = BCM_PKTIO_FID_INVALID,
    },
};
/******************************************************************************
 * Private functions
 */

static int
bcm56880_a0_dna_4_7_11_ltsw_pktio_fid_support_get(
    int unit,
    bcmint_pktio_hdr_info_t *hdr_info,
    bcm_pktio_bitmap_t *support)
{
    bcmint_pktio_fid_info_t *fid_db, *fid_p;
    int i = 0;

    SHR_FUNC_ENTER(unit);

    switch (hdr_info->pmd_type) {
        case bcmPktioPmdTypeRxFlex:
            fid_p = fid_db = bcm56880_a0_dna_4_7_11_rxpmd_flex_db;
            break;

        case bcmPktioPmdTypeGih:
            fid_p = fid_db = bcm56880_a0_dna_4_7_11_generic_loopback_db;
            break;

        case bcmPktioPmdTypeHigig3:
            fid_p = fid_db = bcm56880_a0_dna_4_7_11_hg3_db;
            break;

        default:
            SHR_ERR_EXIT(SHR_E_UNAVAIL);
    }

    while (fid_p->fid != BCM_PKTIO_FID_INVALID) {
        if (hdr_info->profile_chk) {
            if (!(fid_p->profile_bmp & (1 << hdr_info->profile))) {
                fid_p = &(fid_db[i++]);
                continue;
            }
        }
        SHR_BITSET(support->pbits, fid_p->fid);
        fid_p = &(fid_db[i++]);
    }

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56880_a0_dna_4_7_11_ltsw_pktio_fid_info_get(
    int unit,
    bcmint_pktio_hdr_info_t *hdr_info,
    bcmint_pktio_fid_info_t **fid_info)
{
    bcmint_pktio_fid_info_t *fid_db, *fid_p;
    int i = 0;
    bool found = FALSE;

    SHR_FUNC_ENTER(unit);

    switch (hdr_info->pmd_type) {
        case bcmPktioPmdTypeRxFlex:
            fid_p = fid_db = bcm56880_a0_dna_4_7_11_rxpmd_flex_db;
            break;

        case bcmPktioPmdTypeGih:
            fid_p = fid_db = bcm56880_a0_dna_4_7_11_generic_loopback_db;
            break;

        case bcmPktioPmdTypeHigig3:
            fid_p = fid_db = bcm56880_a0_dna_4_7_11_hg3_db;
            break;

        case bcmintPktioPmdTypeMatchId:
            fid_p = fid_db = bcm56880_a0_dna_4_7_11_match_id_db;
            break;

        default:
            SHR_ERR_EXIT(SHR_E_UNAVAIL);
    }

    while (fid_p->fid != BCM_PKTIO_FID_INVALID) {
        if (fid_p->fid == hdr_info->fid) {
            found = TRUE;
            break;
        }

        fid_p = &(fid_db[i++]);
    }

    if (found) {
        *fid_info = fid_p;
    } else {
        SHR_ERR_EXIT(SHR_E_UNAVAIL);
    }

exit:
    SHR_FUNC_EXIT();
}

static int
bcm56880_a0_dna_4_7_11_ltsw_pktio_fid_info_lookup(
    int unit,
    int opcode,
    bcmint_pktio_hdr_info_t *hdr_info,
    bcmint_pktio_fid_info_t **fid_info)
{
    bcmint_pktio_fid_info_t *fid_db, *fid_p;
    int i = 0, chk_fid;
    bool found = FALSE;

    SHR_FUNC_ENTER(unit);

    switch (hdr_info->pmd_type) {
        case bcmPktioPmdTypeRxFlex:
            fid_p = fid_db = bcm56880_a0_dna_4_7_11_rxpmd_flex_reason_db;
            break;

        default:
            SHR_ERR_EXIT(SHR_E_UNAVAIL);
    }

    while (fid_p->fid != BCM_PKTIO_FID_INVALID) {
        switch (opcode) {
            case BCMINT_PKTIO_OP_API2LT:
                chk_fid = fid_p->fid;
                break;

            case BCMINT_PKTIO_OP_LT2API:
                chk_fid = fid_p->lt_fid;
                break;

            default:
                SHR_ERR_EXIT(SHR_E_INTERNAL);
        }

        if (chk_fid == hdr_info->fid) {
            found = TRUE;
            break;
        }

        fid_p = &(fid_db[i++]);
    }

    if (found) {
        *fid_info = fid_p;
    } else {
        SHR_ERR_EXIT(SHR_E_UNAVAIL);
    }

exit:
    SHR_FUNC_EXIT();
}

/*!
 * \brief Pktio driver function variable for bcm56880_a0_dna_4_7_11 device.
 */
static mbcm_ltsw_pktio_drv_t bcm56880_a0_dna_4_7_11_ltsw_pktio_drv = {
    .pktio_fid_support_get = bcm56880_a0_dna_4_7_11_ltsw_pktio_fid_support_get,
    .pktio_fid_info_get = bcm56880_a0_dna_4_7_11_ltsw_pktio_fid_info_get,
    .pktio_fid_info_lookup = bcm56880_a0_dna_4_7_11_ltsw_pktio_fid_info_lookup
};

/******************************************************************************
 * Public functions
 */

int
bcm56880_a0_dna_4_7_11_ltsw_pktio_drv_attach(int unit)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_pktio_drv_set(unit, &bcm56880_a0_dna_4_7_11_ltsw_pktio_drv));

exit:
    SHR_FUNC_EXIT();
}

