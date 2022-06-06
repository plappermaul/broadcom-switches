/*
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated.
 * Edits to this file will be lost when it is regenerated.
 *
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

#ifndef BCMINT_LTSW_MPLS_INT_H
#define BCMINT_LTSW_MPLS_INT_H

#include <sal/sal_types.h>

/*! MPLS gport vpless indicator. */
#define BCMINT_MPLS_GPORT_FAILOVER_VPLESS (1 << 24)

/*! Set MPLS gport vpless indicator. */
#define BCMINT_MPLS_GPORT_FAILOVER_VPLESS_SET(_GP) ((_GP) | BCMINT_MPLS_GPORT_FAILOVER_VPLESS)

/*! Get MPLS gport vpless indicator. */
#define BCMINT_MPLS_GPORT_FAILOVER_VPLESS_GET(_GP) ((_GP) & BCMINT_MPLS_GPORT_FAILOVER_VPLESS)

/*! Clear MPLS gport vpless indicator. */
#define BCMINT_MPLS_GPORT_FAILOVER_VPLESS_CLEAR(_GP) ((_GP) & (~BCMINT_MPLS_GPORT_FAILOVER_VPLESS))

/*!
 * \brief Get scalar value from symbol.
 *
 * \param [in] unit Unit Number.
 * \param [in] symbol symbol String
 * \param [in] val scalar value
 */
typedef int (*mpls_symbol_to_scalar_f)(
    int unit,
    const char *symbol,
    uint64_t *val);

/*!
 * \brief Get scalar value from symbol.
 *
 * \param [in] unit Unit Number.
 * \param [in] val scalar value
 * \param [in] symbol symbol String
 */
typedef int (*mpls_scalar_to_symbol_f)(
    int unit,
    uint64_t val,
    const char **symbol);

/*!
 * \brief MPLS forwarding label actions.
 */
typedef enum bcmint_mpls_fwd_label_action_s {

    /*! No operation. */
    bcmintLtswMplsFwdLabelActionNoop = 0,

    /*! Swap action. */
    bcmintLtswMplsFwdLabelActionSwap = 1,

    /*! PHP action. */
    bcmintLtswMplsFwdLabelActionPhp = 2,

    /*! Pop and route action. */
    bcmintLtswMplsFwdLabelActionPopRoute = 3,

    /*! Pop and switch action. */
    bcmintLtswMplsFwdLabelActionPopSwitch = 4,

    /*! MPLS forwarding label action count. */
    bcmintLtswMplsFwdLabelActionCount = 5

} bcmint_mpls_fwd_label_action_t;

#define BCMINT_MPLS_FWD_LABEL_ACTION_STR \
{ \
    "Noop", \
    "Swap", \
    "Php", \
    "PopRoute", \
    "PopSwitch" \
}

/*!
 * \brief MPLS CW check control mode.
 */
typedef enum bcmint_mpls_cw_check_ctrl_s {

    /*! Disable mode. */
    bcmintLtswMplsCwCheckCtrlDisable = 0,

    /*! Loose mode. */
    bcmintLtswMplsCwCheckCtrlLoose = 1,

    /*! Strict mode. */
    bcmintLtswMplsCwCheckCtrlStrict = 2,

    /*! CW not present. */
    bcmintLtswMplsCwCheckCtrlNotPresent = 3,

    /*! MPLS CW check control mode count. */
    bcmintLtswMplsCwCheckCtrlCount = 4

} bcmint_mpls_cw_check_ctrl_t;

#define BCMINT_MPLS_CW_CHECK_CTRL_STR \
{ \
    "Disable", \
    "Loose", \
    "Strict", \
    "NotPresent" \
}

/*!
 * \brief EGR_MPLS_VPN view type.
 */
typedef enum bcmint_mpls_egr_mpls_vpn_view_s {

    /*! VC LABEL view. */
    bcmintLtswMplsEgrMplsVpnViewVtVcLabelTab = 0,

    /*! IFA attributes view. */
    bcmintLtswMplsEgrMplsVpnViewVtIfaAttributes = 1,

    /*! EGR_MPLS_VPN view type count. */
    bcmintLtswMplsEgrMplsVpnViewCount = 2

} bcmint_mpls_egr_mpls_vpn_view_t;

#define BCMINT_MPLS_EGR_MPLS_VPN_VIEW_STR \
{ \
    "VtVcLabelTab", \
    "VtIfaAttributes" \
}

/*!
 * \brief MPLS TTL mode.
 */
typedef enum bcmint_mpls_ttl_mode_s {

    /*! Pipe mode. */
    bcmintLtswMplsTtlModePipe = 0,

    /*! Uniform mode. */
    bcmintLtswMplsTtlModeUniform = 1,

    /*! MPLS TTL mode count. */
    bcmintLtswMplsTtlModeCount = 2

} bcmint_mpls_ttl_mode_t;

#define BCMINT_MPLS_TTL_MODE_STR \
{ \
    "Pipe", \
    "Uniform" \
}

/*!
 * \brief MPLS bottom of stack label actions.
 */
typedef enum bcmint_mpls_tnl_bos_action_s {

    /*! Invalid action. */
    bcmintLtswMplsTnlBosActionInvalid = 0,

    /*! L3 IIF. */
    bcmintLtswMplsTnlBosL3Iif = 1,

    /*! Swap to NHI. */
    bcmintLtswMplsTnlBosSwapNhi = 2,

    /*! L3 nexthop. */
    bcmintLtswMplsTnlBosL3Nhi = 3,

    /*! L3 ECMP. */
    bcmintLtswMplsTnlBosL3Ecmp = 4,

    /*! Swap to ECMP. */
    bcmintLtswMplsTnlBosSwapEcmp = 5,

    /*! MPLS bottom of stack label actions count. */
    bcmintLtswMplsTnlBosActionCnt = 6

} bcmint_mpls_tnl_bos_action_t;

#define BCMINT_MPLS_TNL_BOS_ACTION_STR \
{ \
    "ActionInvalid", \
    "L3Iif", \
    "SwapNhi", \
    "L3Nhi", \
    "L3Ecmp", \
    "SwapEcmp", \
    "ActionCnt" \
}

/*!
 * \brief Non MPLS bottom of stack label actions.
 */
typedef enum bcmint_mpls_non_tnl_bos_action_s {

    /*! Invalid action. */
    bcmintLtswMplsTnlNonBosActionInvalid = 0,

    /*! POP. */
    bcmintLtswMplsTnlNonBosPop = 1,

    /*! PHP to nexthop. */
    bcmintLtswMplsTnlNonBosPhpNhi = 2,

    /*! swap to L3 nexthop. */
    bcmintLtswMplsTnlNonBosSwapNhi = 3,

    /*! Swap to ECMP. */
    bcmintLtswMplsTnlNonBosSwapEcmp = 4,

    /*! PHP to ECMP. */
    bcmintLtswMplsTnlNonBosPhpEcmp = 5,

    /*! Non MPLS bottom of stack label actions count. */
    bcmintLtswMplsTnlNonBosActionCnt = 6

} bcmint_mpls_non_tnl_bos_action_t;

#define BCMINT_MPLS_NON_TNL_BOS_ACTION_STR \
{ \
    "ActionInvalid", \
    "Pop", \
    "PhpNhi", \
    "SwapNhi", \
    "SwapEcmp", \
    "PhpEcmp", \
    "ActionCnt" \
}

/*!
 * \brief MPLS exp map actions.
 */
typedef enum bcmint_mpls_exp_map_action_s {

    /*! None action. */
    bcmintLtswMplsExpMapActionNone = 0,

    /*! Mapping. */
    bcmintLtswMplsExpMapActionMapPriCng = 1,

    /*! Fix mapping. */
    bcmintLtswMplsExpMapActionFixedPriMapCng = 2,

    /*! MPLS exp map actions count. */
    bcmintLtswMplsExpMapActionCnt = 3

} bcmint_mpls_exp_map_action_t;

#define BCMINT_MPLS_EXP_MAP_ACTION_STR \
{ \
    "None", \
    "MapPriCng", \
    "FixedPriMapCng", \
    "Cnt" \
}

/*!
 * \brief MPLS LT field description.
 */
typedef struct bcmint_mpls_fld_s {

    /*! LT field name. */
    const char *name;

    /*! Callback to get scalar value from symbol. */
    mpls_symbol_to_scalar_f symbol_to_scalar;

    /*! Callback to get symbol from scalar value. */
    mpls_scalar_to_symbol_f scalar_to_symbol;

} bcmint_mpls_fld_t;

/*!
 * \brief MPLS_ENTRY_1 LT fields.
 */
typedef enum bcmint_mpls_fld_mpls_entry_1_s {

    /*! LABEL. */
    BCMINT_LTSW_MPLS_FLD_ME_1_LABEL = 0,

    /*! LABEL_CONTEXT_ID. */
    BCMINT_LTSW_MPLS_FLD_ME_1_LABEL_CONTEXT_ID = 1,

    /*! ENTRY_VALID. */
    BCMINT_LTSW_MPLS_FLD_ME_1_ENTRY_VALID = 2,

    /*! TERM_LABEL. */
    BCMINT_LTSW_MPLS_FLD_ME_1_TERM_LABEL = 3,

    /*! OPAQUE_CTRL_0. */
    BCMINT_LTSW_MPLS_FLD_ME_1_OPAQUE_CTRL_0 = 4,

    /*! OPAQUE_CTRL_1. */
    BCMINT_LTSW_MPLS_FLD_ME_1_OPAQUE_CTRL_1 = 5,

    /*! EXP_TO_ECN_MAPPING_PTR. */
    BCMINT_LTSW_MPLS_FLD_ME_1_EXP_TO_ECN_MAPPING_PTR = 6,

    /*! EXP_QOS_CTRL_ID. */
    BCMINT_LTSW_MPLS_FLD_ME_1_EXP_QOS_CTRL_ID = 7,

    /*! EXP_REMARK_CTRL_ID. */
    BCMINT_LTSW_MPLS_FLD_ME_1_EXP_REMARK_CTRL_ID = 8,

    /*! PW_TERM_NUM_VALID. */
    BCMINT_LTSW_MPLS_FLD_ME_1_PW_TERM_NUM_VALID = 9,

    /*! PW_CC_TYPE. */
    BCMINT_LTSW_MPLS_FLD_ME_1_PW_CC_TYPE = 10,

    /*! CW_CHECK_CTRL. */
    BCMINT_LTSW_MPLS_FLD_ME_1_CW_CHECK_CTRL = 11,

    /*! TTL_MODE. */
    BCMINT_LTSW_MPLS_FLD_ME_1_TTL_MODE = 12,

    /*! ECMP_AND_PROT_SWT_SFC_PROFILE_INDEX. */
    BCMINT_LTSW_MPLS_FLD_ME_1_ECMP_AND_PROT_SWT_SFC_PROFILE_INDEX = 13,

    /*! DROP_DATA_ENABLE. */
    BCMINT_LTSW_MPLS_FLD_ME_1_DROP_DATA_ENABLE = 14,

    /*! ENTROPY_LABEL_PRESENT. */
    BCMINT_LTSW_MPLS_FLD_ME_1_ENTROPY_LABEL_PRESENT = 15,

    /*! POINT2POINT_FLOW. */
    BCMINT_LTSW_MPLS_FLD_ME_1_POINT2POINT_FLOW = 16,

    /*! LABEL_ACTION. */
    BCMINT_LTSW_MPLS_FLD_ME_1_LABEL_ACTION = 17,

    /*! DESTINATION_TYPE. */
    BCMINT_LTSW_MPLS_FLD_ME_1_DESTINATION_TYPE = 18,

    /*! EXP_MAPPING_PTR. */
    BCMINT_LTSW_MPLS_FLD_ME_1_EXP_MAPPING_PTR = 19,

    /*! PW_TERM_NUM. */
    BCMINT_LTSW_MPLS_FLD_ME_1_PW_TERM_NUM = 20,

    /*! CLASS_ID. */
    BCMINT_LTSW_MPLS_FLD_ME_1_CLASS_ID = 21,

    /*! SVP. */
    BCMINT_LTSW_MPLS_FLD_ME_1_SVP = 22,

    /*! DESTINATION. */
    BCMINT_LTSW_MPLS_FLD_ME_1_DESTINATION = 23,

    /*! VFI_L3_IIF_NHOP_2_ECMP_GROUP_INDEX_1. */
    BCMINT_LTSW_MPLS_FLD_ME_1_VFI_L3_IIF_NHOP_2_ECMP_GROUP_INDEX_1 = 24,

    /*! MPLS_ENTRY_1 field count. */
    BCMINT_LTSW_MPLS_FLD_ME_1_FLD_CNT = 25

} bcmint_mpls_fld_mpls_entry_1_t;

#define BCMINT_MPLS_FLD_MPLS_ENTRY_1_STR \
{ \
    "LABEL", \
    "LABEL_CONTEXT_ID", \
    "ENTRY_VALID", \
    "TERM_LABEL", \
    "OPAQUE_CTRL_0", \
    "OPAQUE_CTRL_1", \
    "EXP_TO_ECN_MAPPING_PTR", \
    "EXP_QOS_CTRL_ID", \
    "EXP_REMARK_CTRL_ID", \
    "PW_TERM_NUM_VALID", \
    "PW_CC_TYPE", \
    "CW_CHECK_CTRL", \
    "TTL_MODE", \
    "ECMP_AND_PROT_SWT_SFC_PROFILE_INDEX", \
    "DROP_DATA_ENABLE", \
    "ENTROPY_LABEL_PRESENT", \
    "POINT2POINT_FLOW", \
    "LABEL_ACTION", \
    "DESTINATION_TYPE", \
    "EXP_MAPPING_PTR", \
    "PW_TERM_NUM", \
    "CLASS_ID", \
    "SVP", \
    "DESTINATION", \
    "VFI_L3_IIF_NHOP_2_ECMP_GROUP_INDEX_1", \
    "FLD_CNT" \
}

/*!
 * \brief EGR_L3_TUNNEL_0 LT fields.
 */
typedef enum bcmint_mpls_fld_egr_l3_tunnel_0_s {

    /*! EGR_L3_TUNNEL_IDX_0. */
    BCMINT_LTSW_MPLS_FLD_EGR_L3_TNL_0_EGR_L3_TUNNEL_IDX_0 = 0,

    /*! EDIT_CTRL_ID. */
    BCMINT_LTSW_MPLS_FLD_EGR_L3_TNL_0_EDIT_CTRL_ID = 1,

    /*! IPV6_SRC_ADDR. */
    BCMINT_LTSW_MPLS_FLD_EGR_L3_TNL_0_IPV6_SRC_ADDR = 2,

    /*! TOS_OR_EXP_REMARK_BASE_PTR. */
    BCMINT_LTSW_MPLS_FLD_EGR_L3_TNL_0_TOS_OR_EXP_REMARK_BASE_PTR = 3,

    /*! MPLS_PUSH_BITMAP. */
    BCMINT_LTSW_MPLS_FLD_EGR_L3_TNL_0_MPLS_PUSH_BITMAP = 4,

    /*! TTL_OR_MPLS_CTRL. */
    BCMINT_LTSW_MPLS_FLD_EGR_L3_TNL_0_TTL_OR_MPLS_CTRL = 5,

    /*! EGR_L3_TUNNEL_0 LT field count. */
    BCMINT_LTSW_MPLS_FLD_EGR_L3_TNL_0_FLD_CNT = 6

} bcmint_mpls_fld_egr_l3_tunnel_0_t;

#define BCMINT_MPLS_FLD_EGR_L3_TUNNEL_0_STR \
{ \
    "EGR_L3_TUNNEL_IDX_0", \
    "EDIT_CTRL_ID", \
    "IPV6_SRC_ADDR", \
    "TOS_OR_EXP_REMARK_BASE_PTR", \
    "MPLS_PUSH_BITMAP", \
    "TTL_OR_MPLS_CTRL", \
    "FLD_CNT" \
}

/*!
 * \brief EGR_L3_TUNNEL_1 LT fields.
 */
typedef enum bcmint_mpls_fld_egr_l3_tunnel_1_s {

    /*! EGR_L3_TUNNEL_IDX_0. */
    BCMINT_LTSW_MPLS_FLD_EGR_L3_TNL_1_EGR_L3_TUNNEL_IDX_0 = 0,

    /*! IPV6_DST_ADDR_HI_1. */
    BCMINT_LTSW_MPLS_FLD_EGR_L3_TNL_1_IPV6_DST_ADDR_HI_1 = 1,

    /*! IPV6_DST_ADDR_HI_0. */
    BCMINT_LTSW_MPLS_FLD_EGR_L3_TNL_1_IPV6_DST_ADDR_HI_0 = 2,

    /*! IPV6_DST_ADDR_LO. */
    BCMINT_LTSW_MPLS_FLD_EGR_L3_TNL_1_IPV6_DST_ADDR_LO = 3,

    /*! Strength profile index. */
    BCMINT_LTSW_MPLS_FLD_EGR_L3_TNL_1_STRENGTH_PRFL_IDX = 4,

    /*! EGR_L3_TUNNEL_1 LT field count. */
    BCMINT_LTSW_MPLS_FLD_EGR_L3_TNL_1_FLD_CNT = 5

} bcmint_mpls_fld_egr_l3_tunnel_1_t;

#define BCMINT_MPLS_FLD_EGR_L3_TUNNEL_1_STR \
{ \
    "EGR_L3_TUNNEL_IDX_0", \
    "IPV6_DST_ADDR_HI_1", \
    "IPV6_DST_ADDR_HI_0", \
    "IPV6_DST_ADDR_LO", \
    "STRENGTH_PRFL_IDX", \
    "FLD_CNT" \
}

/*!
 * \brief EGR_MPLS_VPN LT fields.
 */
typedef enum bcmint_mpls_fld_egr_mpls_vpn_s {

    /*! MPLS_VPN_OR_INT_INDEX. */
    BCMINT_LTSW_MPLS_FLD_EGR_MPLS_VPN_MPLS_VPN_OR_INT_INDEX = 0,

    /*! VIEW_T. */
    BCMINT_LTSW_MPLS_FLD_EGR_MPLS_VPN_VIEW_T = 1,

    /*! VPN_LABEL. */
    BCMINT_LTSW_MPLS_FLD_EGR_MPLS_VPN_VPN_LABEL = 2,

    /*! PROCESS_CTRL. */
    BCMINT_LTSW_MPLS_FLD_EGR_MPLS_VPN_PROCESS_CTRL = 3,

    /*! EGR_MPLS_VPN LT field count. */
    BCMINT_LTSW_MPLS_FLD_EGR_MPLS_VPN_FLD_CNT = 4

} bcmint_mpls_fld_egr_mpls_vpn_t;

#define BCMINT_MPLS_FLD_EGR_MPLS_VPN_STR \
{ \
    "MPLS_VPN_OR_INT_INDEX", \
    "VIEW_T", \
    "VPN_LABEL", \
    "PROCESS_CTRL", \
    "FLD_CNT" \
}

/*!
 * \brief ING_SVP_TABLE LT fields.
 */
typedef enum bcmint_mpls_fld_ing_svp_table_s {

    /*! SVP. */
    BCMINT_LTSW_MPLS_FLD_ING_SVP_TABLE_SVP = 0,

    /*! NETWORK_GROUP_BITMAP. */
    BCMINT_LTSW_MPLS_FLD_ING_SVP_TABLE_NETWORK_GROUP_BITMAP = 1,

    /*! VPN_LABEL. */
    BCMINT_LTSW_MPLS_FLD_ING_SVP_TABLE_VPN_LABEL = 2,

    /*! PROCESS_CTRL. */
    BCMINT_LTSW_MPLS_FLD_ING_SVP_TABLE_PROCESS_CTRL = 3,

    /*! Vlan tag preserve control. */
    BCMINT_LTSW_MPLS_FLD_ING_SVP_TABLE_VLAN_TAG_PRESERVE_CTRL = 4,

    /*! CML_FLAGS_NEW. */
    BCMINT_LTSW_MPLS_FLD_ING_SVP_TABLE_CML_FLAGS_NEW = 5,

    /*! CML_FLAGS_MOVE. */
    BCMINT_LTSW_MPLS_FLD_ING_SVP_TABLE_CML_FLAGS_MOVE = 6,

    /*! Remap multicast group. */
    BCMINT_LTSW_MPLS_FLD_ING_SVP_TABLE_IPMC_REMAP_ENABLE = 7,

    /*! Dot1p mapping pointer. */
    BCMINT_LTSW_MPLS_FLD_ING_SVP_TABLE_DOT1P_PTR = 8,

    /*! DSCP mapping pointer. */
    BCMINT_LTSW_MPLS_FLD_ING_SVP_TABLE_DSCP_PTR = 9,

    /*! Use DSCP for PHB mapping. */
    BCMINT_LTSW_MPLS_FLD_ING_SVP_TABLE_USE_DSCP_FOR_PHB = 10,

    /*! Use Otag dot1p CFI for PHB mapping. */
    BCMINT_LTSW_MPLS_FLD_ING_SVP_TABLE_USE_OTAG_DOT1P_CFI_FOR_PHB = 11,

    /*! Use Itag dot1p CFI for PHB mapping. */
    BCMINT_LTSW_MPLS_FLD_ING_SVP_TABLE_USE_ITAG_DOT1P_CFI_FOR_PHB = 12,

    /*! ING_SVP_TABLE LT field count. */
    BCMINT_LTSW_MPLS_FLD_ING_SVP_TABLE_FLD_CNT = 13

} bcmint_mpls_fld_ing_svp_table_t;

#define BCMINT_MPLS_FLD_ING_SVP_TABLE_STR \
{ \
    "SVP", \
    "NETWORK_GROUP_BITMAP", \
    "VPN_LABEL", \
    "PROCESS_CTRL", \
    "VLAN_TAG_PRESERVE_CTRL", \
    "CML_FLAGS_NEW", \
    "CML_FLAGS_MOVE", \
    "IPMC_REMAP_ENABLE", \
    "DOT1P_PTR", \
    "DSCP_PTR", \
    "USE_DSCP_FOR_PHB", \
    "USE_OTAG_DOT1P_CFI_FOR_PHB", \
    "USE_ITAG_DOT1P_CFI_FOR_PHB", \
    "FLD_CNT" \
}

/*!
 * \brief ING_DVP_TABLE LT fields.
 */
typedef enum bcmint_mpls_fld_ing_dvp_table_s {

    /*! DVP. */
    BCMINT_LTSW_MPLS_FLD_ING_DVP_TABLE_DVP = 0,

    /*! NO_CUT_THRU. */
    BCMINT_LTSW_MPLS_FLD_ING_DVP_TABLE_NO_CUT_THRU = 1,

    /*! STRENGTH_PROFILE_INDEX. */
    BCMINT_LTSW_MPLS_FLD_ING_DVP_TABLE_STRENGTH_PROFILE_INDEX = 2,

    /*! Destination value. */
    BCMINT_LTSW_MPLS_FLD_ING_DVP_TABLE_DESTINATION = 3,

    /*! Destination type indicator. */
    BCMINT_LTSW_MPLS_FLD_ING_DVP_TABLE_L2MC_L3MC_L2_OIF_SYS_DST_VALID = 4,

    /*! ING_DVP_TABLE LT field count. */
    BCMINT_LTSW_MPLS_FLD_ING_DVP_TABLE_FLD_CNT = 5

} bcmint_mpls_fld_ing_dvp_table_t;

#define BCMINT_MPLS_FLD_ING_DVP_TABLE_STR \
{ \
    "DVP", \
    "NO_CUT_THRU", \
    "STRENGTH_PROFILE_INDEX", \
    "DESTINATION", \
    "L2MC_L3MC_L2_OIF_SYS_DST_VALID", \
    "FLD_CNT" \
}

/*!
 * \brief EGR_DVP LT fields.
 */
typedef enum bcmint_mpls_fld_egr_dvp_s {

    /*! DVP. */
    BCMINT_LTSW_MPLS_FLD_EGR_DVP_DVP = 0,

    /*! OBJ_TABLE_SEL_1 */
    BCMINT_LTSW_MPLS_FLD_EGR_DVP_OBJ_TABLE_SEL_1 = 1,

    /*! EDIT_CTRL_ID. */
    BCMINT_LTSW_MPLS_FLD_EGR_DVP_EDIT_CTRL_ID = 2,

    /*! TUNNEL_IDX_0. */
    BCMINT_LTSW_MPLS_FLD_EGR_DVP_TUNNEL_IDX_0 = 3,

    /*! VC_LABEL_INDEX. */
    BCMINT_LTSW_MPLS_FLD_EGR_DVP_VC_LABEL_INDEX = 4,

    /*! CLASS_ID */
    BCMINT_LTSW_MPLS_FLD_EGR_DVP_CLASS_ID = 5,

    /*! SPLIT_HORIZON_CTRL. */
    BCMINT_LTSW_MPLS_FLD_EGR_DVP_SPLIT_HORIZON_CTRL = 6,

    /*! STRENGTH_PRFL_IDX. */
    BCMINT_LTSW_MPLS_FLD_EGR_DVP_STRENGTH_PRFL_IDX = 7,

    /*! MTU for check. */
    BCMINT_LTSW_MPLS_FLD_EGR_DVP_L2_MTU_VALUE = 8,

    /*! Process control 2. */
    BCMINT_LTSW_MPLS_FLD_EGR_DVP_PROCESS_CTRL_2 = 9,

    /*! Sequence number profile index. */
    BCMINT_LTSW_MPLS_FLD_EGR_DVP_SEQ_NUM_PROFILE_IDX = 10,

    /*! Sequence number counter index. */
    BCMINT_LTSW_MPLS_FLD_EGR_DVP_SEQ_NUM_COUNTER_IDX = 11,

    /*! EGR_DVP LT field count. */
    BCMINT_LTSW_MPLS_FLD_EGR_DVP_FLD_CNT = 12

} bcmint_mpls_fld_egr_dvp_t;

#define BCMINT_MPLS_FLD_EGR_DVP_STR \
{ \
    "DVP", \
    "OBJ_TABLE_SEL_1", \
    "EDIT_CTRL_ID", \
    "TUNNEL_IDX_0", \
    "VC_LABEL_INDEX", \
    "CLASS_ID", \
    "SPLIT_HORIZON_CTRL", \
    "STRENGTH_PRFL_IDX", \
    "L2_MTU_VALUE", \
    "PROCESS_CTRL_2", \
    "SEQ_NUM_PROFILE_IDX", \
    "SEQ_NUM_COUNTER_IDX", \
    "FLD_CNT" \
}

/*!
 * \brief ING_VFI_TABLE LT fields.
 */
typedef enum bcmint_mpls_fld_ing_vfi_table_s {

    /*! VFI. */
    BCMINT_LTSW_MPLS_FLD_ING_VFI_TABLE_VFI = 0,

    /*! DESTINATION. */
    BCMINT_LTSW_MPLS_FLD_ING_VFI_TABLE_DESTINATION = 1,

    /*! DESTINATION_TYPE. */
    BCMINT_LTSW_MPLS_FLD_ING_VFI_TABLE_DESTINATION_TYPE = 2,

    /*! ING_DVP_TABLE LT field count. */
    BCMINT_LTSW_MPLS_FLD_ING_VFI_TABLE_FLD_CNT = 3

} bcmint_mpls_fld_ing_vfi_table_t;

#define BCMINT_MPLS_FLD_ING_VFI_TABLE_STR \
{ \
    "VFI", \
    "DESTINATION", \
    "DESTINATION_TYPE", \
    "FLD_CNT" \
}

/*!
 * \brief TNL_MPLS_CTR_ING_EFLEX_ACTION LT fields.
 */
typedef enum bcmint_mpls_fld_eflex_action_s {

    /*! TNL_MPLS_CTR_ING_EFLEX_ACTION_ID. */
    BCMINT_LTSW_MPLS_FLD_EFA_ID = 0,

    /*! ACTION. */
    BCMINT_LTSW_MPLS_FLD_EFA_ACTION = 1,

    /*! TNL_MPLS_CTR_ING_EFLEX_ACTION LT field count. */
    BCMINT_LTSW_MPLS_FLD_EFA_FLD_CNT = 2

} bcmint_mpls_fld_eflex_action_t;

#define BCMINT_MPLS_FLD_EFLEX_ACTION_STR \
{ \
    "ID", \
    "ACTION", \
    "FLD_CNT" \
}

/*!
 * \brief TNL_MPLS_DECAP/TNL_MPLS_DECAP_TRUNK LT fields.
 */
typedef enum bcmint_mpls_fld_decap_s {

    /*! MPLS_LABEL. */
    BCMINT_LTSW_MPLS_FLD_DECAP_LABEL = 0,

    /*! TRUNK_ID/MODPORT. */
    BCMINT_LTSW_MPLS_FLD_DECAP_SOURCE = 1,

    /*! BOS_ACTIONS. */
    BCMINT_LTSW_MPLS_FLD_DECAP_BOS_ACTIONS = 2,

    /*! NON_BOS_ACTIONS. */
    BCMINT_LTSW_MPLS_FLD_DECAP_NON_BOS_ACTIONS = 3,

    /*! L3_IIF_ID. */
    BCMINT_LTSW_MPLS_FLD_DECAP_L3_IIF_ID = 4,

    /*! NHOP_ID. */
    BCMINT_LTSW_MPLS_FLD_DECAP_NHOP_ID = 5,

    /*! ECMP_ID. */
    BCMINT_LTSW_MPLS_FLD_DECAP_ECMP_ID = 6,

    /*! USE_TTL_FROM_DECAP_HDR. */
    BCMINT_LTSW_MPLS_FLD_DECAP_USE_TTL_FROM_DECAP_HDR = 7,

    /*! KEEP_PAYLOAD_DSCP. */
    BCMINT_LTSW_MPLS_FLD_DECAP_KEEP_PAYLOAD_DSCP = 8,

    /*! TNL_EXP_TO_INNER_EXP. */
    BCMINT_LTSW_MPLS_FLD_DECAP_TNL_EXP_MAP = 9,

    /*! EXP_MAP_ACTION. */
    BCMINT_LTSW_MPLS_FLD_DECAP_EXP_MAP_ACTION = 10,

    /*! PHB_ING_MPLS_EXP_TO_INT_PRI_ID. */
    BCMINT_LTSW_MPLS_FLD_DECAP_EXP_TO_INT_PRI_ID = 11,

    /*! IPV6_PAYLOAD. */
    BCMINT_LTSW_MPLS_FLD_DECAP_IPV6_PAYLOAD = 12,

    /*! IPV4_PAYLOAD. */
    BCMINT_LTSW_MPLS_FLD_DECAP_IPV4_PAYLOAD = 13,

    /*! DROP. */
    BCMINT_LTSW_MPLS_FLD_DECAP_DROP = 14,

    /*! INT_PRI. */
    BCMINT_LTSW_MPLS_FLD_DECAP_INT_PRI = 15,

    /*! ECN_MPLS_EXP_TO_IP_ECN_ID. */
    BCMINT_LTSW_MPLS_FLD_DECAP_EXP_TO_IP_ECN_ID = 16,

    /*! BFD. */
    BCMINT_LTSW_MPLS_FLD_DECAP_BFD = 17,

    /*! CTR_ING_EFLEX_OBJECT. */
    BCMINT_LTSW_MPLS_FLD_DECAP_CTR_EFLEX_OBJECT = 18,

    /*! TNL_MPLS_CTR_ING_EFLEX_ACTION_ID. */
    BCMINT_LTSW_MPLS_FLD_DECAP_CTR_EFLEX_ACTION = 19,

    /*! TNL_MPLS_DECAP/TNL_MPLS_DECAP_TRUNK LT field count. */
    BCMINT_LTSW_MPLS_FLD_DECAP_FLD_CNT = 20

} bcmint_mpls_fld_decap_t;

#define BCMINT_MPLS_FLD_DECAP_STR \
{ \
    "LABEL", \
    "SOURCE", \
    "BOS_ACTIONS", \
    "NON_BOS_ACTIONS", \
    "L3_IIF_ID", \
    "NHOP_ID", \
    "ECMP_ID", \
    "USE_TTL_FROM_DECAP_HDR", \
    "KEEP_PAYLOAD_DSCP", \
    "TNL_EXP_MAP", \
    "EXP_MAP_ACTION", \
    "EXP_TO_INT_PRI_ID", \
    "IPV6_PAYLOAD", \
    "IPV4_PAYLOAD", \
    "DROP", \
    "INT_PRI", \
    "EXP_TO_IP_ECN_ID", \
    "BFD", \
    "CTR_EFLEX_OBJECT", \
    "CTR_EFLEX_ACTION", \
    "FLD_CNT" \
}

/*!
 * \brief MPLS LT description.
 */
typedef struct bcmint_mpls_lt_s {

    /*! LT name. */
    const char *name;

    /*! Bitmap of valid fields. */
    uint64_t fld_bmp;

    /*! LT fields. */
    const bcmint_mpls_fld_t *flds;

} bcmint_mpls_lt_t;

/*!
 * \brief MPLS LTs.
 */
typedef enum bcmint_mpls_lt_id_s {

    /*! LT MPLS_ENTRY_1. */
    BCMINT_LTSW_MPLS_LT_MPLS_ENTRY_1 = 0,

    /*! LT MPLS_ENTRY_2. */
    BCMINT_LTSW_MPLS_LT_MPLS_ENTRY_2 = 1,

    /*! LT MPLS_ENTRY_3. */
    BCMINT_LTSW_MPLS_LT_MPLS_ENTRY_3 = 2,

    /*! LT EGR_L3_TUNNEL_0. */
    BCMINT_LTSW_MPLS_LT_EGR_L3_TUNNEL_0 = 3,

    /*! LT EGR_L3_TUNNEL_1. */
    BCMINT_LTSW_MPLS_LT_EGR_L3_TUNNEL_1 = 4,

    /*! LT EGR_MPLS_VPN. */
    BCMINT_LTSW_MPLS_LT_EGR_MPLS_VPN = 5,

    /*! LT ING_SVP_TABLE. */
    BCMINT_LTSW_MPLS_LT_ING_SVP_TABLE = 6,

    /*! LT ING_DVP_TABLE. */
    BCMINT_LTSW_MPLS_LT_ING_DVP_TABLE = 7,

    /*! LT EGR_DVP. */
    BCMINT_LTSW_MPLS_LT_EGR_DVP = 8,

    /*! LT ING_VFI_TABLE. */
    BCMINT_LTSW_MPLS_LT_ING_VFI_TABLE = 9,

    /*! LT TNL_MPLS_CTR_ING_EFLEX_ACTION. */
    BCMINT_LTSW_MPLS_LT_EFLEX_ACTION = 10,

    /*! LT TNL_MPLS_DECAP_TRUNK. */
    BCMINT_LTSW_MPLS_LT_DECAP_TRUNK = 11,

    /*! LT TNL_MPLS_DECAP. */
    BCMINT_LTSW_MPLS_LT_DECAP = 12,

    /*! MPLS LT count. */
    BCMINT_LTSW_MPLS_LT_CNT = 13

} bcmint_mpls_lt_id_t;

#define BCMINT_MPLS_LT_ID_STR \
{ \
    "MPLS_ENTRY_1", \
    "MPLS_ENTRY_2", \
    "MPLS_ENTRY_3", \
    "EGR_L3_TUNNEL_0", \
    "EGR_L3_TUNNEL_1", \
    "EGR_MPLS_VPN", \
    "ING_SVP_TABLE", \
    "ING_DVP_TABLE", \
    "EGR_DVP", \
    "ING_VFI_TABLE", \
    "EFLEX_ACTION", \
    "DECAP_TRUNK", \
    "DECAP", \
    "CNT" \
}

/*!
 * \brief HA sub component ID.
 */
typedef enum bcmint_mpls_ha_sub_comp_id_s {

    /*! VPWS VPN Bitmap. */
    bcmLtswMplsHaVpwsVpnBmp = 0,

    /*! VPWS VPN member information. */
    bcmLtswMplsHaVpwsVpnMemInfo = 1,

    /*! Xfs MPls info PW termination bitmap. */
    bcmLtswMplsHaXmiPwTermBmp = 2,

    /*! fs MPls info vp information. */
    bcmLtswMplsHaXmiVpInfo = 3,

    /*! Xfs MPls info VP to L2_iif map. */
    bcmLtswMplsHaXmiL2iifMap = 4,

    /*! Xfs MPls info Speical label control. */
    bcmLtswMplsHaXmiSpecLablCtrl = 5,

    /*! MPLS HA sub component ID count. */
    bcmLtswMplsHaCount = 6

} bcmint_mpls_ha_sub_comp_id_t;

#define BCMINT_MPLS_HA_SUB_COMP_ID_STR \
{ \
    "VpwsVpnBmp", \
    "VpwsVpnMemInfo", \
    "XmiPwTermBmp", \
    "XmiVpInfo", \
    "XmiL2iifMap", \
    "XmiSpecLablCtrl" \
}

/*!
 * \brief MPLS LT field description.
 */
typedef struct bcmint_mpls_lt_db_s {

    /*! LT bitmap. */
    uint32_t lt_bmp;

    /*! LT array. */
    const bcmint_mpls_lt_t *lts;

} bcmint_mpls_lt_db_t;

/*!
 * \brief Get MPLS LT info with the given LT ID.
 *
 * \param [in] unit Unit Number.
 * \param [in] lt_id LT ID.
 * \param [out] lt_info LT info.
 */
extern int
bcmint_mpls_mpls_lt_get(
    int unit,
    bcmint_mpls_lt_id_t lt_id,
    const bcmint_mpls_lt_t **lt_info);

/*!
 * \brief Get the LT info with a LT ID.
 *
 * \param [in] unit Unit Number.
 * \param [in] lt_id LT ID.
 * \param [out] lt_info LT info.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
bcmint_mpls_lt_get(
    int unit,
    bcmint_mpls_lt_id_t lt_id,
    const bcmint_mpls_lt_t **lt_info);

#endif /* BCMINT_LTSW_MPLS_INT_H */