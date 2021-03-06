/*
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated.
 * Edits to this file will be lost when it is regenerated.
 *
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

#include <bcm/flexctr.h>

#include <bcm_int/ltsw/flexctr_int.h>
#include <bcm_int/ltsw/mbcm/flexctr.h>
#include <bcm_int/ltsw/flexctr.h>
#include "sub_dispatch.h"

#include <shr/shr_debug.h>
#include <bcmltd/chip/bcmltd_str.h>

/******************************************************************************
 * Local definitions
 */

#define BSL_LOG_MODULE BSL_LS_BCM_FLEXCTR

/*!
 * \brief Flex counter trigger interval database.
 */
static bcmint_flexctr_interval_info_t bcm56990_a0_ltsw_flexctr_interval_db[] = {
    /* Unlimited collection. */
    {bcmFlexctrTriggerIntervalUnlimited},
    /* 100 nanosecond interval. */
    {bcmFlexctrTriggerInterval100ns},
    /* 500 nanosecond interval. */
    {bcmFlexctrTriggerInterval500ns},
    /* 1 microsecond interval. */
    {bcmFlexctrTriggerInterval1us},
    /* 10 microsecond interval. */
    {bcmFlexctrTriggerInterval10us},
    /* 100 microsecond interval. */
    {bcmFlexctrTriggerInterval100us},
    /* 1 millisecond interval. */
    {bcmFlexctrTriggerInterval1ms},
    /* 10 millisecond interval. */
    {bcmFlexctrTriggerInterval10ms},
    /* 100 millisecond interval. */
    {bcmFlexctrTriggerInterval100ms},
    /* 1 second interval. */
    {bcmFlexctrTriggerInterval1s},
};

/*!
 * \brief Flex counter action scale enumeration mapping.
 */
static bcmint_flexctr_enum_map_t bcm56990_a0_ltsw_flexctr_action_scale_enum[] = {
    { SCALE_INFINITEs, 0 },     /* bcmFlexctrTriggerIntervalUnlimited */
    { SCALE_128_NSECs, 1 },     /* bcmFlexctrTriggerInterval100ns */
    { SCALE_512_NSECs, 2 },     /* bcmFlexctrTriggerInterval500ns */
    { SCALE_1024_NSECs, 3 },    /* bcmFlexctrTriggerInterval1us */
    { SCALE_8_USECs, 4 },       /* bcmFlexctrTriggerInterval10us */
    { SCALE_131_USECs, 5 },     /* bcmFlexctrTriggerInterval100us */
    { SCALE_1_MSECs, 6 },       /* bcmFlexctrTriggerInterval1ms */
    { SCALE_8_MSECs, 7 },       /* bcmFlexctrTriggerInterval10ms */
    { SCALE_131_MSECs, 8 },     /* bcmFlexctrTriggerInterval100ms */
    { SCALE_1_SECs, 9 },        /* bcmFlexctrTriggerInterval1s */
};

/*!
 * \brief Flex counter flex action range database.
 */
static bcmint_flexctr_flex_action_range_t bcm56990_a0_ltsw_flexctr_flex_action_range_db[] = {
    { BCMI_LTSW_FLEXCTR_STAGE_ING_CTR, 24, 31 },
    { BCMI_LTSW_FLEXCTR_STAGE_EGR_CTR, 8, 15 },
};

/*!
 * \brief Flex counter packet attribute database.
 */
static bcmint_flexctr_pkt_attr_info_t bcm56990_a0_ltsw_flexctr_pkt_attr_db[] = {
    {
        bcmFlexctrPacketAttributeTypeIngIp,
        BCMI_LTSW_FLEXCTR_STAGE_ING_CTR,
        0x1, 18,
    },
    {
        bcmFlexctrPacketAttributeTypeIngDrop,
        BCMI_LTSW_FLEXCTR_STAGE_ING_CTR,
        0x1, 3,
    },
    {
        bcmFlexctrPacketAttributeTypeIngPktType,
        BCMI_LTSW_FLEXCTR_STAGE_ING_CTR,
        0x3f, 26,
    },
    {
        bcmFlexctrPacketAttributeTypeIngTosEcn,
        BCMI_LTSW_FLEXCTR_STAGE_ING_CTR,
        0x3, 46,
    },
    {
        bcmFlexctrPacketAttributeTypeIngTosDscp,
        BCMI_LTSW_FLEXCTR_STAGE_ING_CTR,
        0x3f, 48,
    },
    {
        bcmFlexctrPacketAttributeTypeIngPort,
        BCMI_LTSW_FLEXCTR_STAGE_ING_CTR,
        0x3f, 32,
    },
    {
        bcmFlexctrPacketAttributeTypeIngIntCn,
        BCMI_LTSW_FLEXCTR_STAGE_ING_CTR,
        0x3, 12,
    },
    {
        bcmFlexctrPacketAttributeTypeIngOuterPri,
        BCMI_LTSW_FLEXCTR_STAGE_ING_CTR,
        0x7, 22,
    },
    {
        bcmFlexctrPacketAttributeTypeIngVlanTag,
        BCMI_LTSW_FLEXCTR_STAGE_ING_CTR,
        0x1, 25,
    },
    {
        bcmFlexctrPacketAttributeTypeIngIntPri,
        BCMI_LTSW_FLEXCTR_STAGE_ING_CTR,
        0xf, 14,
    },
    {
        bcmFlexctrPacketAttributeTypeIngColor,
        BCMI_LTSW_FLEXCTR_STAGE_ING_CTR,
        0x3, 0,
    },
    {
        bcmFlexctrPacketAttributeTypeIngFieldIngressColor,
        BCMI_LTSW_FLEXCTR_STAGE_ING_CTR,
        0x3, 6,
    },
    {
        bcmFlexctrPacketAttributeTypeIngElephant,
        BCMI_LTSW_FLEXCTR_STAGE_ING_CTR,
        0x1, 4,
    },
    {
        bcmFlexctrPacketAttributeTypeIngTcpFlagsSyn,
        BCMI_LTSW_FLEXCTR_STAGE_ING_CTR,
        0x1, 39,
    },
    {
        bcmFlexctrPacketAttributeTypeIngTcpFlagsFin,
        BCMI_LTSW_FLEXCTR_STAGE_ING_CTR,
        0x1, 38,
    },
    {
        bcmFlexctrPacketAttributeTypeIngTcpFlagsRst,
        BCMI_LTSW_FLEXCTR_STAGE_ING_CTR,
        0x1, 40,
    },
    {
        bcmFlexctrPacketAttributeTypeIngTcpFlagsPsh,
        BCMI_LTSW_FLEXCTR_STAGE_ING_CTR,
        0x1, 41,
    },
    {
        bcmFlexctrPacketAttributeTypeIngTcpFlagsAck,
        BCMI_LTSW_FLEXCTR_STAGE_ING_CTR,
        0x1, 42,
    },
    {
        bcmFlexctrPacketAttributeTypeIngTcpFlagsUrg,
        BCMI_LTSW_FLEXCTR_STAGE_ING_CTR,
        0x1, 43,
    },
    {
        bcmFlexctrPacketAttributeTypeIngTcpFlagsEce,
        BCMI_LTSW_FLEXCTR_STAGE_ING_CTR,
        0x1, 44,
    },
    {
        bcmFlexctrPacketAttributeTypeIngTcpFlagsCwr,
        BCMI_LTSW_FLEXCTR_STAGE_ING_CTR,
        0x1, 45
    },
    {
        bcmFlexctrPacketAttributeTypeIngMplsExp,
        BCMI_LTSW_FLEXCTR_STAGE_ING_CTR,
        0x7, 19,
    },
    {
        bcmFlexctrPacketAttributeTypeEgrIp,
        BCMI_LTSW_FLEXCTR_STAGE_EGR_CTR,
        0x1, 13,
    },
    {
        bcmFlexctrPacketAttributeTypeEgrDrop,
        BCMI_LTSW_FLEXCTR_STAGE_EGR_CTR,
        0x1, 5,
    },
    {
        bcmFlexctrPacketAttributeTypeEgrPktTypeMc,
        BCMI_LTSW_FLEXCTR_STAGE_EGR_CTR,
        0x1, 14,
    },
    {
        bcmFlexctrPacketAttributeTypeEgrTosEcn,
        BCMI_LTSW_FLEXCTR_STAGE_EGR_CTR,
        0x3, 32,
    },
    {
        bcmFlexctrPacketAttributeTypeEgrTosDscp,
        BCMI_LTSW_FLEXCTR_STAGE_EGR_CTR,
        0x3f, 34,
    },
    {
        bcmFlexctrPacketAttributeTypeEgrEgressPort,
        BCMI_LTSW_FLEXCTR_STAGE_EGR_CTR,
        0x3f, 26,
    },
    {
        bcmFlexctrPacketAttributeTypeEgrIntCn,
        BCMI_LTSW_FLEXCTR_STAGE_EGR_CTR,
        0x3, 7,
    },
    {
        bcmFlexctrPacketAttributeTypeEgrOuterPri,
        BCMI_LTSW_FLEXCTR_STAGE_EGR_CTR,
        0x7, 22,
    },
    {
        bcmFlexctrPacketAttributeTypeEgrVlanTag,
        BCMI_LTSW_FLEXCTR_STAGE_EGR_CTR,
        0x1, 25,
    },
    {
        bcmFlexctrPacketAttributeTypeEgrIntPri,
        BCMI_LTSW_FLEXCTR_STAGE_EGR_CTR,
        0xf, 9,
    },
    {
        bcmFlexctrPacketAttributeTypeEgrColor,
        BCMI_LTSW_FLEXCTR_STAGE_EGR_CTR,
        0x3, 0,
    },
    {
        bcmFlexctrPacketAttributeTypeEgrCongestionMarked,
        BCMI_LTSW_FLEXCTR_STAGE_EGR_CTR,
        0x1, 2,
    },
    {
        bcmFlexctrPacketAttributeTypeEgrQueueType,
        BCMI_LTSW_FLEXCTR_STAGE_EGR_CTR,
        0x1, 40,
    },
    {
        bcmFlexctrPacketAttributeTypeEgrCos,
        BCMI_LTSW_FLEXCTR_STAGE_EGR_CTR,
        0xf, 15,
    },
    {
        bcmFlexctrPacketAttributeTypeEgrMplsExp,
        BCMI_LTSW_FLEXCTR_STAGE_EGR_CTR,
        0x7, 19,
    },
};

/*!
 * \brief Flex counter packet attribute ingress bus database.
 */
static bcmint_flexctr_pkt_attr_bus_info_t bcm56990_a0_ltsw_flexctr_pkt_attr_ing_bus_db[] = {
    { 0, CNG_BIT0s },
    { 1, CNG_BIT1s },
    { 2, DCNs },
    { 3, DROPs },
    { 4, ELEPHANTs },
    /* { 5, INVALID }, */
    { 6, IFP_CNG_BIT0s },
    { 7, IFP_CNG_BIT1s },
    { 8, IFP_INT_PRI_BIT0s },
    { 9, IFP_INT_PRI_BIT1s },
    { 10, IFP_INT_PRI_BIT2s },
    { 11, IFP_INT_PRI_BIT3s },
    { 12, INT_CN_BIT0s },
    { 13, INT_CN_BIT1s },
    { 14, INT_PRI_BIT0s },
    { 15, INT_PRI_BIT1s },
    { 16, INT_PRI_BIT2s },
    { 17, INT_PRI_BIT3s },
    { 18, IPs },
    { 19, MPLS_EXP_BIT0s },
    { 20, MPLS_EXP_BIT1s },
    { 21, MPLS_EXP_BIT2s },
    { 22, OUTER_DOT1P_BIT0s },
    { 23, OUTER_DOT1P_BIT1s },
    { 24, OUTER_DOT1P_BIT2s },
    { 25, VLAN_SOTs },
    { 26, PKT_RESOLUTION_BIT0s },
    { 27, PKT_RESOLUTION_BIT1s },
    { 28, PKT_RESOLUTION_BIT2s },
    { 29, PKT_RESOLUTION_BIT3s },
    { 30, PKT_RESOLUTION_BIT4s },
    { 31, PKT_RESOLUTION_BIT5s },
    { 32, PORT_ID_BIT0s },
    { 33, PORT_ID_BIT1s },
    { 34, PORT_ID_BIT2s },
    { 35, PORT_ID_BIT3s },
    { 36, PORT_ID_BIT4s },
    { 37, PORT_ID_BIT5s },
    { 38, TCP_FLAG_TYPE_FINs },
    { 39, TCP_FLAG_TYPE_SYNs },
    { 40, TCP_FLAG_TYPE_RSTs },
    { 41, TCP_FLAG_TYPE_PSHs },
    { 42, TCP_FLAG_TYPE_ACKs },
    { 43, TCP_FLAG_TYPE_URGs },
    { 44, TCP_FLAG_TYPE_ECEs },
    { 45, TCP_FLAG_TYPE_CWRs },
    { 46, TOS_ECN_BIT0s },
    { 47, TOS_ECN_BIT1s },
    { 48, TOS_DSCP_BIT0s },
    { 49, TOS_DSCP_BIT1s },
    { 50, TOS_DSCP_BIT2s },
    { 51, TOS_DSCP_BIT3s },
    { 52, TOS_DSCP_BIT4s },
    { 53, TOS_DSCP_BIT5s },
};

/*!
 * \brief Flex counter packet attribute egress bus database.
 */
static bcmint_flexctr_pkt_attr_bus_info_t bcm56990_a0_ltsw_flexctr_pkt_attr_egr_bus_db[] = {
    { 0, CNG_BIT0s },
    { 1, CNG_BIT1s },
    { 2, CONGESTION_MARKEDs },
    /* { 3, INVALID }, */
    /* { 4, INVALID }, */
    { 5, DROPs },
    /* { 6, INVALID }, */
    { 7, INT_CN_BIT0s },
    { 8, INT_CN_BIT1s },
    { 9, INT_PRI_BIT0s },
    { 10, INT_PRI_BIT1s },
    { 11, INT_PRI_BIT2s },
    { 12, INT_PRI_BIT3s },
    { 13, IPs },
    { 14, PKT_TYPE_MULTICASTs },
    { 15, MMU_QUEUE_NUM_BIT0s },
    { 16, MMU_QUEUE_NUM_BIT1s },
    { 17, MMU_QUEUE_NUM_BIT2s },
    { 18, MMU_QUEUE_NUM_BIT3s },
    { 19, MPLS_EXP_BIT0s },
    { 20, MPLS_EXP_BIT1s },
    { 21, MPLS_EXP_BIT2s },
    { 22, OUTER_DOT1P_BIT0s },
    { 23, OUTER_DOT1P_BIT1s },
    { 24, OUTER_DOT1P_BIT2s },
    { 25, VLAN_SOTs },
    { 26, PORT_ID_BIT0s },
    { 27, PORT_ID_BIT1s },
    { 28, PORT_ID_BIT2s },
    { 29, PORT_ID_BIT3s },
    { 30, PORT_ID_BIT4s },
    { 31, PORT_ID_BIT5s },
    { 32, TOS_ECN_BIT0s },
    { 33, TOS_ECN_BIT1s },
    { 34, TOS_DSCP_BIT0s },
    { 35, TOS_DSCP_BIT1s },
    { 36, TOS_DSCP_BIT2s },
    { 37, TOS_DSCP_BIT3s },
    { 38, TOS_DSCP_BIT4s },
    { 39, TOS_DSCP_BIT5s },
    { 40, UNICAST_QUEUEINGs },
};

/*!
 * \brief Flex counter ingress packet type attribute value database.
 */
static bcmint_flexctr_pkt_type_value_info_t bcm56990_a0_ltsw_flexctr_pkt_type_ing_value_db[] = {
    { bcmFlexctrPacketAttributePktTypeUnknown, 0 },
    { bcmFlexctrPacketAttributePktTypeControl, 1 },
    { bcmFlexctrPacketAttributePktTypeBfd, 3 },
    { bcmFlexctrPacketAttributePktTypeBpdu, 4 },
    { bcmFlexctrPacketAttributePktType1588, 6 },
    { bcmFlexctrPacketAttributePktTypeKnownL2Uc, 8 },
    { bcmFlexctrPacketAttributePktTypeUnknownL2Uc, 9 },
    { bcmFlexctrPacketAttributePktTypeKnownL2Mc, 10 },
    { bcmFlexctrPacketAttributePktTypeUnknownL2Mc, 11 },
    { bcmFlexctrPacketAttributePktTypeL2Bc, 12 },
    { bcmFlexctrPacketAttributePktTypeKnownL3Uc, 16 },
    { bcmFlexctrPacketAttributePktTypeUnknownL3Uc, 17 },
    { bcmFlexctrPacketAttributePktTypeKnownIpmc, 18 },
    { bcmFlexctrPacketAttributePktTypeUnknownIpmc, 19 },
    { bcmFlexctrPacketAttributePktTypeKnownMplsL3, 26 },
    { bcmFlexctrPacketAttributePktTypeKnownMpls, 28 },
    { bcmFlexctrPacketAttributePktTypeUnknownMpls, 25 },
};

/*!
 * \brief Flex counter VLAN tag attribute value database.
 */
static bcmint_flexctr_vlan_tag_value_info_t bcm56990_a0_ltsw_flexctr_vlan_tag_value_db[] = {
    { bcmFlexctrPacketAttributeVlanUnTagged, 0 },
    { bcmFlexctrPacketAttributeVlanOuterTag, 1 },
};

/******************************************************************************
 * Private functions
 */

static int
bcm56990_a0_ltsw_flexctr_trigger_interval_validate(
    int unit,
    int interval)
{
    bcmint_flexctr_interval_info_t *interval_db, *ii;
    size_t i, count;

    count = COUNTOF(bcm56990_a0_ltsw_flexctr_interval_db);
    interval_db = bcm56990_a0_ltsw_flexctr_interval_db;

    if (interval < bcmFlexctrTriggerIntervalUnlimited ||
        interval >= bcmFlexctrTriggerIntervalCount) {
        LOG_ERROR(BSL_LOG_MODULE,
                  (BSL_META_U(unit, "Invalid trigger interval %d\n"),
                   interval));
        return SHR_E_PARAM;
    }

    for (i = 0; i < count; i++) {
        ii = &(interval_db[i]);
        if (interval == ii->interval) {
            break;
        }
    }

    /* Not found. */
    if (i >= count) {
        LOG_INFO(BSL_LOG_MODULE,
                 (BSL_META_U(unit, "Flex counter interval %d not found\n"),
                  interval));

        return SHR_E_UNAVAIL;
    }

    return SHR_E_NONE;
}

static int
bcm56990_a0_ltsw_flexctr_flex_action_range_get(
    int unit,
    int stage,
    uint32_t *min,
    uint32_t *max)
{
    bcmint_flexctr_flex_action_range_t *action_range_db, *ar;
    size_t i, count;

    count = COUNTOF(bcm56990_a0_ltsw_flexctr_flex_action_range_db);
    action_range_db = bcm56990_a0_ltsw_flexctr_flex_action_range_db;

    if (stage >= BCMI_LTSW_FLEXCTR_STAGE_COUNT) {
        LOG_ERROR(BSL_LOG_MODULE,
                  (BSL_META_U(unit, "Invalid flex counter stage %d\n"),
                   stage));
        return SHR_E_PARAM;
    }

    for (i = 0; i < count; i++) {
        ar = &(action_range_db[i]);
        if (stage == ar->stage) {
            *min = ar->flex_action_min;
            *max = ar->flex_action_max;
            break;
        }
    }

    /* Not found. */
    if (i >= count) {
        LOG_INFO(BSL_LOG_MODULE,
                 (BSL_META_U(unit, "Flex counter stage %d not found\n"),
                  stage));

        return SHR_E_UNAVAIL;
    }

    return SHR_E_NONE;
}

static int
bcm56990_a0_ltsw_flexctr_pkt_attr_info_get(
    int unit,
    int attr_type,
    bcmint_flexctr_pkt_attr_info_t **pkt_attr_info)
{
    bcmint_flexctr_pkt_attr_info_t *pkt_attr_db, *pai;
    size_t i, count;

    count = COUNTOF(bcm56990_a0_ltsw_flexctr_pkt_attr_db);
    pkt_attr_db = bcm56990_a0_ltsw_flexctr_pkt_attr_db;

    if (attr_type < 0 || attr_type >= bcmFlexctrPacketAttributeTypeCount) {
        LOG_ERROR(BSL_LOG_MODULE,
                  (BSL_META_U(unit, "Invalid flex counter packet attribute "
                                    "selector type %d\n"),
                   attr_type));

        *pkt_attr_info = NULL;
        return SHR_E_PARAM;
    }

    for (i = 0; i < count; i++) {
        pai = &(pkt_attr_db[i]);
        if (attr_type == pai->attr_type) {
            *pkt_attr_info = pai;
            break;
        }
    }

    /* Not found. */
    if (i >= count) {
        LOG_INFO(BSL_LOG_MODULE,
                 (BSL_META_U(unit, "Flex counter packet attribute "
                                   "selector type %d not found\n"),
                  attr_type));

        return SHR_E_UNAVAIL;
    }

    return SHR_E_NONE;
}

static int
bcm56990_a0_ltsw_flexctr_pkt_attr_bus_info_get(
    int unit,
    int stage,
    int bit_pos,
    bcmint_flexctr_pkt_attr_bus_info_t **pkt_attr_bus_info)
{
    bcmint_flexctr_pkt_attr_bus_info_t *pkt_attr_bus_db, *pabi;
    size_t i, count;

    if (stage == BCMI_LTSW_FLEXCTR_STAGE_ING_CTR) {
        count = COUNTOF(bcm56990_a0_ltsw_flexctr_pkt_attr_ing_bus_db);
        pkt_attr_bus_db = bcm56990_a0_ltsw_flexctr_pkt_attr_ing_bus_db;
    } else if (stage == BCMI_LTSW_FLEXCTR_STAGE_EGR_CTR) {
        count = COUNTOF(bcm56990_a0_ltsw_flexctr_pkt_attr_egr_bus_db);
        pkt_attr_bus_db = bcm56990_a0_ltsw_flexctr_pkt_attr_egr_bus_db;
    } else {
        LOG_ERROR(BSL_LOG_MODULE,
                  (BSL_META_U(unit, "Invalid flex counter stage %d\n"),
                   stage));
        *pkt_attr_bus_info = NULL;
        return SHR_E_PARAM;
    }

    for (i = 0; i < count; i++) {
        pabi = &(pkt_attr_bus_db[i]);
        if (bit_pos == pabi->bit_pos) {
            *pkt_attr_bus_info = pabi;
            break;
        }
    }

    /* Not found. */
    if (i >= count) {
        LOG_INFO(BSL_LOG_MODULE,
                 (BSL_META_U(unit, "Flex counter packet attribute "
                                   "sbit position %d not found in stage %d\n"),
                  bit_pos, stage));

        return SHR_E_UNAVAIL;
    }

    return SHR_E_NONE;
}

static int
bcm56990_a0_ltsw_flexctr_pkt_type_value_get(
    int unit,
    int pkt_type,
    uint32_t *value)
{
    bcmint_flexctr_pkt_type_value_info_t *pkt_type_value_db, *ptvi;
    size_t i, count;

    count = COUNTOF(bcm56990_a0_ltsw_flexctr_pkt_type_ing_value_db);
    pkt_type_value_db = bcm56990_a0_ltsw_flexctr_pkt_type_ing_value_db;

    if (pkt_type >= bcmFlexctrPacketAttributePktTypeCount) {
        LOG_ERROR(BSL_LOG_MODULE,
                  (BSL_META_U(unit, "Invalid flex counter packet type %d\n"),
                   pkt_type));
        return SHR_E_PARAM;
    }

    for (i = 0; i < count; i++) {
        ptvi = &(pkt_type_value_db[i]);
        if (pkt_type == ptvi->pkt_type) {
            *value = ptvi->value;
            break;
        }
    }

    /* Not found. */
    if (i >= count) {
        LOG_INFO(BSL_LOG_MODULE,
                 (BSL_META_U(unit, "Flex counter packet type %d not found\n"),
                  pkt_type));

        return SHR_E_UNAVAIL;
    }

    return SHR_E_NONE;
}

static int
bcm56990_a0_ltsw_flexctr_vlan_tag_value_get(
    int unit,
    int vlan_tag,
    uint32_t *value)
{
    bcmint_flexctr_vlan_tag_value_info_t *vlan_tag_value_db, *vtvi;
    size_t i, count;

    count = COUNTOF(bcm56990_a0_ltsw_flexctr_vlan_tag_value_db);
    vlan_tag_value_db = bcm56990_a0_ltsw_flexctr_vlan_tag_value_db;

    if (vlan_tag >= bcmFlexctrPacketAttributeVlanCount) {
        LOG_ERROR(BSL_LOG_MODULE,
                  (BSL_META_U(unit, "Invalid flex counter VLAN tag %d\n"),
                   vlan_tag));
        return SHR_E_PARAM;
    }

    for (i = 0; i < count; i++) {
        vtvi = &(vlan_tag_value_db[i]);
        if (vlan_tag == vtvi->vlan_tag) {
            *value = vtvi->value;
            break;
        }
    }

    /* Not found. */
    if (i >= count) {
        LOG_INFO(BSL_LOG_MODULE,
                 (BSL_META_U(unit, "Flex counter VLAN tag %d not found\n"),
                  vlan_tag));

        return SHR_E_UNAVAIL;
    }

    return SHR_E_NONE;
}

static int
bcm56990_a0_ltsw_flexctr_action_scale_enum_map_get(
    int unit,
    const bcmint_flexctr_enum_map_t **map,
    uint32_t *count)
{
    if ((count == NULL) || (map == NULL)) {
        return SHR_E_PARAM;
    }

    *map = bcm56990_a0_ltsw_flexctr_action_scale_enum;
    *count = COUNTOF(bcm56990_a0_ltsw_flexctr_action_scale_enum);
    return SHR_E_NONE;
}

/*!
 * \brief Flexctr driver function variable for bcm56990_a0 device.
 */
static mbcm_ltsw_flexctr_drv_t bcm56990_a0_ltsw_flexctr_drv = {
    .flexctr_trigger_interval_validate = bcm56990_a0_ltsw_flexctr_trigger_interval_validate,
    .flexctr_flex_action_range_get = bcm56990_a0_ltsw_flexctr_flex_action_range_get,
    .flexctr_pkt_attr_info_get = bcm56990_a0_ltsw_flexctr_pkt_attr_info_get,
    .flexctr_pkt_attr_bus_info_get = bcm56990_a0_ltsw_flexctr_pkt_attr_bus_info_get,
    .flexctr_pkt_type_value_get = bcm56990_a0_ltsw_flexctr_pkt_type_value_get,
    .flexctr_vlan_tag_value_get = bcm56990_a0_ltsw_flexctr_vlan_tag_value_get,
    .flexctr_action_scale_enum_map_get = bcm56990_a0_ltsw_flexctr_action_scale_enum_map_get
};

/******************************************************************************
 * Public functions
 */

int
bcm56990_a0_ltsw_flexctr_drv_attach(int unit)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_flexctr_drv_set(unit, &bcm56990_a0_ltsw_flexctr_drv));

    SHR_IF_ERR_VERBOSE_EXIT
        (bcm56990_a0_sub_drv_attach(unit, &bcm56990_a0_ltsw_flexctr_drv,
                                    BCM56990_A0_SUB_DRV_BCM56990_Ax,
                                    BCM56990_A0_SUB_MOD_FLEXCTR));

exit:
    SHR_FUNC_EXIT();
}

/*!
 * \brief Attach functions for other members.
 */
#define BCM56990_A0_DRV_ATTACH_ENTRY(_dn,_vn,_pf,_pd,_r0) \
int _vn##_ltsw_flexctr_drv_attach(int unit) \
{ \
    SHR_FUNC_ENTER(unit); \
    SHR_IF_ERR_VERBOSE_EXIT \
        (mbcm_ltsw_flexctr_drv_set(unit, &bcm56990_a0_ltsw_flexctr_drv)); \
    SHR_IF_ERR_VERBOSE_EXIT \
        (bcm56990_a0_sub_drv_attach(unit, &bcm56990_a0_ltsw_flexctr_drv, \
                                    BCM56990_A0_SUB_DRV_BCM5699x, \
                                    BCM56990_A0_SUB_MOD_FLEXCTR)); \
exit: \
    SHR_FUNC_EXIT(); \
}
#include "sub_devlist.h"
