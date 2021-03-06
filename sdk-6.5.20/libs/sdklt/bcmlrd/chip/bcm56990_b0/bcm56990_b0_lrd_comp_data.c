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
#include <bcmlrd/bcmlrd_types.h>
#include <bcmlrd/bcmlrd_internal.h>
#include <bcmltd/chip/bcmltd_id.h>
#include <bcmlrd/chip/bcm56990_b0/bcm56990_b0_lrd_comp_data.h>

static bcmlrd_sid_t
bcm56990_b0_lrd_comp_bcmcfg_hdl_config[] = {
    CTR_EFLEX_CONFIGt,
    DEVICE_CONFIGt,
    DEVICE_WAL_CONFIGt,
    FP_CONFIGt,
    METER_FP_CONFIGt,
    PC_SERDES_CONFIGt,
    PORT_CONFIGt,
    SER_CONFIGt,
    TM_PM_FLEX_CONFIGt,
    TM_SCHEDULER_CONFIGt,
    TM_SHAPER_CONFIGt,
    TM_THD_CONFIGt,
    UDF_CONFIGt,
};

static bcmlrd_sid_t
bcm56990_b0_lrd_comp_bcmcth_hdl_ctr_eflex_pkt_resolution_info[] = {
    CTR_ING_EFLEX_PKT_RESOLUTION_INFOt,
};

static bcmlrd_sid_t
bcm56990_b0_lrd_comp_bcmcth_hdl_ctr_eflex_stats[] = {
    CTR_EGR_EFLEX_STATSt,
    CTR_ING_EFLEX_STATSt,
};

static bcmlrd_sid_t
bcm56990_b0_lrd_comp_bcmcth_hdl_device_info[] = {
    DEVICE_INFOt,
};

static bcmlrd_sid_t
bcm56990_b0_lrd_comp_bcmcth_hdl_dlb_ecmp_stats[] = {
    DLB_ECMP_STATSt,
};

static bcmlrd_sid_t
bcm56990_b0_lrd_comp_bcmcth_hdl_lb_hash_info[] = {
    LB_HASH_DEVICE_INFOt,
};

static bcmlrd_sid_t
bcm56990_b0_lrd_comp_bcmcth_hdl_meter_fp_ing_info[] = {
    METER_ING_FP_DEVICE_INFOt,
    METER_ING_FP_GRANULARITY_INFOt,
};

static bcmlrd_sid_t
bcm56990_b0_lrd_comp_bcmcth_hdl_mon_inband_telemetry_metadata_chunk_info[] = {
    MON_INBAND_TELEMETRY_METADATA_CHUNK_INFOt,
};

static bcmlrd_sid_t
bcm56990_b0_lrd_comp_bcmcth_hdl_mon_inband_telemetry_metadata_field_info[] = {
    MON_INBAND_TELEMETRY_METADATA_FIELD_INFOt,
};

static bcmlrd_sid_t
bcm56990_b0_lrd_comp_bcmcth_hdl_tnl_decap[] = {
    TNL_MPLS_DECAPt,
    TNL_MPLS_DECAP_TRUNKt,
};

static bcmlrd_sid_t
bcm56990_b0_lrd_comp_bcmcth_hdl_trunk_failover[] = {
    TRUNK_FAILOVERt,
};

static bcmlrd_sid_t
bcm56990_b0_lrd_comp_bcmcth_hdl_trunk_fast_grp[] = {
    TRUNK_FASTt,
};

static bcmlrd_sid_t
bcm56990_b0_lrd_comp_bcmfp_hdl_egr_grp_info[] = {
    FP_EGR_GRP_TEMPLATE_INFOt,
};

static bcmlrd_sid_t
bcm56990_b0_lrd_comp_bcmfp_hdl_egr_grp_part_info[] = {
    FP_EGR_GRP_TEMPLATE_PARTITION_INFOt,
};

static bcmlrd_sid_t
bcm56990_b0_lrd_comp_bcmfp_hdl_egr_slice_info[] = {
    FP_EGR_SLICE_INFOt,
};

static bcmlrd_sid_t
bcm56990_b0_lrd_comp_bcmfp_hdl_egr_stage_info[] = {
    FP_EGR_INFOt,
};

static bcmlrd_sid_t
bcm56990_b0_lrd_comp_bcmfp_hdl_em_fp_grp_info[] = {
    FP_EM_GRP_TEMPLATE_INFOt,
};

static bcmlrd_sid_t
bcm56990_b0_lrd_comp_bcmfp_hdl_em_fp_grp_part_info[] = {
    FP_EM_GRP_TEMPLATE_PARTITION_INFOt,
};

static bcmlrd_sid_t
bcm56990_b0_lrd_comp_bcmfp_hdl_em_fp_pdd_info[] = {
    FP_EM_PDD_TEMPLATE_PARTITION_INFOt,
};

static bcmlrd_sid_t
bcm56990_b0_lrd_comp_bcmfp_hdl_ing_grp_info[] = {
    FP_ING_GRP_TEMPLATE_INFOt,
};

static bcmlrd_sid_t
bcm56990_b0_lrd_comp_bcmfp_hdl_ing_grp_part_info[] = {
    FP_ING_GRP_TEMPLATE_PARTITION_INFOt,
};

static bcmlrd_sid_t
bcm56990_b0_lrd_comp_bcmfp_hdl_ing_slice_info[] = {
    FP_ING_SLICE_INFOt,
};

static bcmlrd_sid_t
bcm56990_b0_lrd_comp_bcmfp_hdl_ing_stage_info[] = {
    FP_ING_INFOt,
};

static bcmlrd_sid_t
bcm56990_b0_lrd_comp_bcmfp_hdl_vlan_grp_info[] = {
    FP_VLAN_GRP_TEMPLATE_INFOt,
};

static bcmlrd_sid_t
bcm56990_b0_lrd_comp_bcmfp_hdl_vlan_grp_part_info[] = {
    FP_VLAN_GRP_TEMPLATE_PARTITION_INFOt,
};

static bcmlrd_sid_t
bcm56990_b0_lrd_comp_bcmfp_hdl_vlan_slice_info[] = {
    FP_VLAN_SLICE_INFOt,
};

static bcmlrd_sid_t
bcm56990_b0_lrd_comp_bcmfp_hdl_vlan_stage_info[] = {
    FP_VLAN_INFOt,
};

static bcmlrd_sid_t
bcm56990_b0_lrd_comp_bcmimm_hdl_basic[] = {
    CTR_CONTROLt,
    CTR_EGR_DEBUG_SELECTt,
    CTR_EGR_EFLEX_ACTION_PROFILEt,
    CTR_EGR_EFLEX_ACTION_PROFILE_INFOt,
    CTR_EGR_EFLEX_ERROR_STATSt,
    CTR_EGR_EFLEX_GROUP_ACTION_PROFILEt,
    CTR_EGR_EFLEX_OBJ_QUANTIZATIONt,
    CTR_EGR_EFLEX_OPERAND_PROFILEt,
    CTR_EGR_EFLEX_OPERAND_PROFILE_INFOt,
    CTR_EGR_EFLEX_RANGE_CHK_PROFILEt,
    CTR_EGR_EFLEX_RANGE_CHK_PROFILE_INFOt,
    CTR_EGR_EFLEX_TRIGGERt,
    CTR_EGR_FLEX_POOL_CONTROLt,
    CTR_EGR_FLEX_POOL_INFOt,
    CTR_ETRAPt,
    CTR_EVENT_SYNC_STATEt,
    CTR_EVENT_SYNC_STATE_CONTROLt,
    CTR_ING_DEBUG_SELECTt,
    CTR_ING_EFLEX_ACTION_PROFILEt,
    CTR_ING_EFLEX_ACTION_PROFILE_INFOt,
    CTR_ING_EFLEX_ERROR_STATSt,
    CTR_ING_EFLEX_GROUP_ACTION_PROFILEt,
    CTR_ING_EFLEX_OBJ_QUANTIZATIONt,
    CTR_ING_EFLEX_OPERAND_PROFILEt,
    CTR_ING_EFLEX_OPERAND_PROFILE_INFOt,
    CTR_ING_EFLEX_RANGE_CHK_PROFILEt,
    CTR_ING_EFLEX_RANGE_CHK_PROFILE_INFOt,
    CTR_ING_EFLEX_TRIGGERt,
    CTR_ING_FLEX_POOL_CONTROLt,
    CTR_ING_FLEX_POOL_INFOt,
    CTR_MIRROR_ING_FLEX_SFLOWt,
    DEVICE_EM_BANKt,
    DEVICE_EM_BANK_INFOt,
    DEVICE_EM_GROUPt,
    DEVICE_EM_GROUP_INFOt,
    DEVICE_OP_DSH_INFOt,
    DEVICE_OP_PT_INFOt,
    DEVICE_PKT_RX_Qt,
    DEVICE_TS_SYNCE_CLK_CONTROLt,
    DEVICE_TS_TODt,
    DLB_ECMPt,
    DLB_ECMP_PORT_CONTROLt,
    ECMP_LIMIT_CONTROLt,
    ECMP_OVERLAYt,
    ECMP_UNDERLAYt,
    ECMP_WEIGHTED_OVERLAYt,
    ECMP_WEIGHTED_UNDERLAYt,
    ECN_PROTOCOLt,
    FP_EGR_ENTRYt,
    FP_EGR_GRP_TEMPLATEt,
    FP_EGR_POLICY_TEMPLATEt,
    FP_EGR_RULE_TEMPLATEt,
    FP_EM_ENTRYt,
    FP_EM_GRP_TEMPLATEt,
    FP_EM_PDD_TEMPLATEt,
    FP_EM_POLICY_TEMPLATEt,
    FP_EM_PRESEL_ENTRY_TEMPLATEt,
    FP_EM_RULE_TEMPLATEt,
    FP_EM_SRC_CLASS_MODEt,
    FP_ING_ENTRYt,
    FP_ING_GRP_TEMPLATEt,
    FP_ING_POLICY_TEMPLATEt,
    FP_ING_PRESEL_ENTRY_TEMPLATEt,
    FP_ING_RANGE_CHECK_GROUPt,
    FP_ING_RULE_TEMPLATEt,
    FP_ING_SRC_CLASS_MODEt,
    FP_VLAN_ENTRYt,
    FP_VLAN_GRP_TEMPLATEt,
    FP_VLAN_POLICY_TEMPLATEt,
    FP_VLAN_RULE_TEMPLATEt,
    L2_LEARN_CONTROLt,
    L3_ALPM_CONTROLt,
    L3_ALPM_LEVEL_1_USAGEt,
    L3_ALPM_LEVEL_2_USAGEt,
    L3_ALPM_LEVEL_3_USAGEt,
    L3_LIMIT_CONTROLt,
    METER_ING_FP_TEMPLATEt,
    PC_AUTONEG_PROFILEt,
    PC_FDR_CONTROLt,
    PC_FDR_STATSt,
    PC_MAC_CONTROLt,
    PC_PFCt,
    PC_PHY_CONTROLt,
    PC_PMD_FIRMWAREt,
    PC_PM_COREt,
    PC_PORTt,
    PC_PORT_INFOt,
    PC_PORT_TIMESYNCt,
    PC_TX_TAPSt,
    PORT_BRIDGEt,
    PORT_EGR_MIRRORt,
    PORT_ING_MIRRORt,
    PORT_MEMBERSHIP_POLICYt,
    PORT_SVP_ING_MIRRORt,
    PORT_SYSTEM_ING_MIRROR_PROFILEt,
    SER_CONTROLt,
    SER_INJECTIONt,
    SER_INJECTION_STATUSt,
    SER_LOGt,
    SER_LOG_STATUSt,
    SER_NOTIFICATIONt,
    SER_PT_CONTROLt,
    SER_PT_STATUSt,
    SER_STATSt,
    TABLE_EM_CONTROLt,
    TABLE_EM_INFOt,
    TABLE_OP_DOP_INFOt,
    TABLE_OP_PT_INFOt,
    TM_BST_CONTROLt,
    TM_BST_EVENT_SOURCE_EGRt,
    TM_BST_EVENT_SOURCE_INGt,
    TM_BST_EVENT_SOURCE_REPL_Qt,
    TM_BST_EVENT_STATEt,
    TM_BST_EVENT_STATE_CONTROLt,
    TM_CUT_THROUGH_PORTt,
    TM_EBST_CONTROLt,
    TM_EBST_MC_Qt,
    TM_EBST_PORTt,
    TM_EBST_PORT_SERVICE_POOLt,
    TM_EBST_SERVICE_POOLt,
    TM_EBST_STATUSt,
    TM_EBST_UC_Qt,
    TM_ING_PORTt,
    TM_ING_PORT_PRI_GRPt,
    TM_MC_GROUPt,
    TM_MC_PORT_AGG_LISTt,
    TM_MIRROR_ON_DROP_CONTROLt,
    TM_MIRROR_ON_DROP_DESTINATIONt,
    TM_MIRROR_ON_DROP_ENCAP_PROFILEt,
    TM_OBM_PC_PM_PKT_PARSEt,
    TM_OBM_PORT_FLOW_CTRLt,
    TM_OBM_PORT_PKT_PARSEt,
    TM_OBM_PORT_PKT_PRI_TC_MAPt,
    TM_PFC_DEADLOCK_RECOVERYt,
    TM_PFC_DEADLOCK_RECOVERY_STATUSt,
    TM_PFC_EGRt,
    TM_PFC_PRI_PROFILEt,
    TM_PORT_MC_Q_TO_SERVICE_POOLt,
    TM_PORT_UC_Q_TO_SERVICE_POOLt,
    TM_SCHEDULER_NODEt,
    TM_SCHEDULER_PORT_PROFILEt,
    TM_SCHEDULER_PROFILEt,
    TM_SCHEDULER_SHAPER_CPU_NODEt,
    TM_SCHEDULER_SP_PROFILEt,
    TM_SHAPER_NODEt,
    TM_SHAPER_PORTt,
    TM_THD_DYNAMIC_MARGINt,
    TM_WRED_CNG_NOTIFICATION_PROFILEt,
    TM_WRED_PORT_SERVICE_POOLt,
    TM_WRED_UC_Qt,
    TNL_IPV4_ENCAPt,
    TNL_IPV6_ENCAPt,
    TNL_MPLS_ENCAPt,
    TNL_MPLS_PROTECTION_ENABLEt,
};

static bcmlrd_sid_t
bcm56990_b0_lrd_comp_bcmimm_hdl_direct[] = {
    L2_VFI_LEARN_DATAt,
    LM_CONTROLt,
    LM_PORT_CONTROLt,
    PC_PORT_MONITORt,
    PC_PORT_PHYS_MAPt,
    TM_EBST_DATAt,
    TM_EGR_THD_MC_PORT_SERVICE_POOLt,
    TM_EGR_THD_SERVICE_POOLt,
    TM_EGR_THD_UC_PORT_SERVICE_POOLt,
    TM_ING_THD_HEADROOM_POOLt,
    TM_ING_THD_PORT_PRI_GRPt,
    TM_ING_THD_PORT_SERVICE_POOLt,
    TM_ING_THD_SERVICE_POOLt,
    TM_THD_MC_EGR_SERVICE_POOLt,
    TM_THD_MC_Qt,
    TM_THD_Q_GRPt,
    TM_THD_UC_Qt,
};

static bcmlrd_sid_t
bcm56990_b0_lrd_comp_bcmlm_hdl_cth_link_state[] = {
    LM_LINK_STATEt,
};

static bcmlrd_sid_t
bcm56990_b0_lrd_comp_bcmpc_hdl_lth_phy_status[] = {
    PC_PHY_STATUSt,
};

static bcmlrd_sid_t
bcm56990_b0_lrd_comp_bcmpc_hdl_lth_phys_port[] = {
    PC_PHYS_PORTt,
};

static bcmlrd_sid_t
bcm56990_b0_lrd_comp_bcmpc_hdl_lth_pm_prop[] = {
    PC_PM_PROPt,
};

static bcmlrd_sid_t
bcm56990_b0_lrd_comp_bcmpc_hdl_lth_pmd_firmware_status[] = {
    PC_PMD_FIRMWARE_STATUSt,
};

static bcmlrd_sid_t
bcm56990_b0_lrd_comp_bcmpc_hdl_lth_port_abilities[] = {
    PC_PORT_ABILITIESt,
};

static bcmlrd_sid_t
bcm56990_b0_lrd_comp_bcmpc_hdl_lth_port_diag_stats[] = {
    PC_PORT_DIAG_STATSt,
};

static bcmlrd_sid_t
bcm56990_b0_lrd_comp_bcmpc_hdl_lth_port_status[] = {
    PC_PORT_STATUSt,
};

static bcmlrd_sid_t
bcm56990_b0_lrd_comp_bcmpc_hdl_lth_tx_taps_status[] = {
    PC_TX_TAPS_STATUSt,
};

static bcmlrd_sid_t
bcm56990_b0_lrd_comp_bcmtm_hdl_ct_port_info[] = {
    TM_CUT_THROUGH_PORT_INFOt,
};

static bcmlrd_sid_t
bcm56990_b0_lrd_comp_bcmtm_hdl_device_info[] = {
    TM_DEVICE_INFOt,
};

static bcmlrd_sid_t
bcm56990_b0_lrd_comp_bcmtm_hdl_lt_pipe_map_info[] = {
    TM_PIPE_MAP_INFOt,
};

static bcmlrd_sid_t
bcm56990_b0_lrd_comp_bcmtm_hdl_lt_pmap_info[] = {
    TM_PORT_MAP_INFOt,
};

static bcmlrd_sid_t
bcm56990_b0_lrd_comp_bcmtm_hdl_obm_port_usage[] = {
    CTR_TM_OBM_PORT_USAGEt,
};

static bcmlrd_sid_t
bcm56990_b0_lrd_comp_bcmtm_hdl_scheduler_profile_q_info[] = {
    TM_SCHEDULER_PROFILE_Q_INFOt,
};


static const bcmlrd_comp_hdl_t
bcm56990_b0_lrd_comp_bcmcfg_hdl[] = {
    {
        .hdl_name = "CONFIG" /* config */,
        .num_tbl = 13,
        .tbl = bcm56990_b0_lrd_comp_bcmcfg_hdl_config,
    },
};

static const bcmlrd_comp_hdl_t
bcm56990_b0_lrd_comp_bcmcth_hdl[] = {
    {
        .hdl_name = "CTR_EFLEX_PKT_RESOLUTION_INFO" /* ctr_eflex_pkt_resolution_info */,
        .num_tbl = 1,
        .tbl = bcm56990_b0_lrd_comp_bcmcth_hdl_ctr_eflex_pkt_resolution_info,
    },
    {
        .hdl_name = "CTR_EFLEX_STATS" /* ctr_eflex_stats */,
        .num_tbl = 2,
        .tbl = bcm56990_b0_lrd_comp_bcmcth_hdl_ctr_eflex_stats,
    },
    {
        .hdl_name = "DEVICE_INFO" /* device_info */,
        .num_tbl = 1,
        .tbl = bcm56990_b0_lrd_comp_bcmcth_hdl_device_info,
    },
    {
        .hdl_name = "DLB_ECMP_STATS" /* dlb_ecmp_stats */,
        .num_tbl = 1,
        .tbl = bcm56990_b0_lrd_comp_bcmcth_hdl_dlb_ecmp_stats,
    },
    {
        .hdl_name = "LB_HASH_INFO" /* lb_hash_info */,
        .num_tbl = 1,
        .tbl = bcm56990_b0_lrd_comp_bcmcth_hdl_lb_hash_info,
    },
    {
        .hdl_name = "METER_FP_ING_INFO" /* meter_fp_ing_info */,
        .num_tbl = 2,
        .tbl = bcm56990_b0_lrd_comp_bcmcth_hdl_meter_fp_ing_info,
    },
    {
        .hdl_name = "MON_INBAND_TELEMETRY_METADATA_CHUNK_INFO" /* mon_inband_telemetry_metadata_chunk_info */,
        .num_tbl = 1,
        .tbl = bcm56990_b0_lrd_comp_bcmcth_hdl_mon_inband_telemetry_metadata_chunk_info,
    },
    {
        .hdl_name = "MON_INBAND_TELEMETRY_METADATA_FIELD_INFO" /* mon_inband_telemetry_metadata_field_info */,
        .num_tbl = 1,
        .tbl = bcm56990_b0_lrd_comp_bcmcth_hdl_mon_inband_telemetry_metadata_field_info,
    },
    {
        .hdl_name = "TNL_DECAP" /* tnl_decap */,
        .num_tbl = 2,
        .tbl = bcm56990_b0_lrd_comp_bcmcth_hdl_tnl_decap,
    },
    {
        .hdl_name = "TRUNK_FAILOVER" /* trunk_failover */,
        .num_tbl = 1,
        .tbl = bcm56990_b0_lrd_comp_bcmcth_hdl_trunk_failover,
    },
    {
        .hdl_name = "TRUNK_FAST_GRP" /* trunk_fast_grp */,
        .num_tbl = 1,
        .tbl = bcm56990_b0_lrd_comp_bcmcth_hdl_trunk_fast_grp,
    },
};

static const bcmlrd_comp_hdl_t
bcm56990_b0_lrd_comp_bcmfp_hdl[] = {
    {
        .hdl_name = "EGR_GRP_INFO" /* egr_grp_info */,
        .num_tbl = 1,
        .tbl = bcm56990_b0_lrd_comp_bcmfp_hdl_egr_grp_info,
    },
    {
        .hdl_name = "EGR_GRP_PART_INFO" /* egr_grp_part_info */,
        .num_tbl = 1,
        .tbl = bcm56990_b0_lrd_comp_bcmfp_hdl_egr_grp_part_info,
    },
    {
        .hdl_name = "EGR_SLICE_INFO" /* egr_slice_info */,
        .num_tbl = 1,
        .tbl = bcm56990_b0_lrd_comp_bcmfp_hdl_egr_slice_info,
    },
    {
        .hdl_name = "EGR_STAGE_INFO" /* egr_stage_info */,
        .num_tbl = 1,
        .tbl = bcm56990_b0_lrd_comp_bcmfp_hdl_egr_stage_info,
    },
    {
        .hdl_name = "EM_FP_GRP_INFO" /* em_fp_grp_info */,
        .num_tbl = 1,
        .tbl = bcm56990_b0_lrd_comp_bcmfp_hdl_em_fp_grp_info,
    },
    {
        .hdl_name = "EM_FP_GRP_PART_INFO" /* em_fp_grp_part_info */,
        .num_tbl = 1,
        .tbl = bcm56990_b0_lrd_comp_bcmfp_hdl_em_fp_grp_part_info,
    },
    {
        .hdl_name = "EM_FP_PDD_INFO" /* em_fp_pdd_info */,
        .num_tbl = 1,
        .tbl = bcm56990_b0_lrd_comp_bcmfp_hdl_em_fp_pdd_info,
    },
    {
        .hdl_name = "ING_GRP_INFO" /* ing_grp_info */,
        .num_tbl = 1,
        .tbl = bcm56990_b0_lrd_comp_bcmfp_hdl_ing_grp_info,
    },
    {
        .hdl_name = "ING_GRP_PART_INFO" /* ing_grp_part_info */,
        .num_tbl = 1,
        .tbl = bcm56990_b0_lrd_comp_bcmfp_hdl_ing_grp_part_info,
    },
    {
        .hdl_name = "ING_SLICE_INFO" /* ing_slice_info */,
        .num_tbl = 1,
        .tbl = bcm56990_b0_lrd_comp_bcmfp_hdl_ing_slice_info,
    },
    {
        .hdl_name = "ING_STAGE_INFO" /* ing_stage_info */,
        .num_tbl = 1,
        .tbl = bcm56990_b0_lrd_comp_bcmfp_hdl_ing_stage_info,
    },
    {
        .hdl_name = "VLAN_GRP_INFO" /* vlan_grp_info */,
        .num_tbl = 1,
        .tbl = bcm56990_b0_lrd_comp_bcmfp_hdl_vlan_grp_info,
    },
    {
        .hdl_name = "VLAN_GRP_PART_INFO" /* vlan_grp_part_info */,
        .num_tbl = 1,
        .tbl = bcm56990_b0_lrd_comp_bcmfp_hdl_vlan_grp_part_info,
    },
    {
        .hdl_name = "VLAN_SLICE_INFO" /* vlan_slice_info */,
        .num_tbl = 1,
        .tbl = bcm56990_b0_lrd_comp_bcmfp_hdl_vlan_slice_info,
    },
    {
        .hdl_name = "VLAN_STAGE_INFO" /* vlan_stage_info */,
        .num_tbl = 1,
        .tbl = bcm56990_b0_lrd_comp_bcmfp_hdl_vlan_stage_info,
    },
};

static const bcmlrd_comp_hdl_t
bcm56990_b0_lrd_comp_bcmimm_hdl[] = {
    {
        .hdl_name = "BASIC" /* basic */,
        .num_tbl = 154,
        .tbl = bcm56990_b0_lrd_comp_bcmimm_hdl_basic,
    },
    {
        .hdl_name = "DIRECT" /* direct */,
        .num_tbl = 17,
        .tbl = bcm56990_b0_lrd_comp_bcmimm_hdl_direct,
    },
};

static const bcmlrd_comp_hdl_t
bcm56990_b0_lrd_comp_bcmlm_hdl[] = {
    {
        .hdl_name = "CTH_LINK_STATE" /* cth_link_state */,
        .num_tbl = 1,
        .tbl = bcm56990_b0_lrd_comp_bcmlm_hdl_cth_link_state,
    },
};

static const bcmlrd_comp_hdl_t
bcm56990_b0_lrd_comp_bcmpc_hdl[] = {
    {
        .hdl_name = "LTH_PHY_STATUS" /* lth_phy_status */,
        .num_tbl = 1,
        .tbl = bcm56990_b0_lrd_comp_bcmpc_hdl_lth_phy_status,
    },
    {
        .hdl_name = "LTH_PHYS_PORT" /* lth_phys_port */,
        .num_tbl = 1,
        .tbl = bcm56990_b0_lrd_comp_bcmpc_hdl_lth_phys_port,
    },
    {
        .hdl_name = "LTH_PM_PROP" /* lth_pm_prop */,
        .num_tbl = 1,
        .tbl = bcm56990_b0_lrd_comp_bcmpc_hdl_lth_pm_prop,
    },
    {
        .hdl_name = "LTH_PMD_FIRMWARE_STATUS" /* lth_pmd_firmware_status */,
        .num_tbl = 1,
        .tbl = bcm56990_b0_lrd_comp_bcmpc_hdl_lth_pmd_firmware_status,
    },
    {
        .hdl_name = "LTH_PORT_ABILITIES" /* lth_port_abilities */,
        .num_tbl = 1,
        .tbl = bcm56990_b0_lrd_comp_bcmpc_hdl_lth_port_abilities,
    },
    {
        .hdl_name = "LTH_PORT_DIAG_STATS" /* lth_port_diag_stats */,
        .num_tbl = 1,
        .tbl = bcm56990_b0_lrd_comp_bcmpc_hdl_lth_port_diag_stats,
    },
    {
        .hdl_name = "LTH_PORT_STATUS" /* lth_port_status */,
        .num_tbl = 1,
        .tbl = bcm56990_b0_lrd_comp_bcmpc_hdl_lth_port_status,
    },
    {
        .hdl_name = "LTH_TX_TAPS_STATUS" /* lth_tx_taps_status */,
        .num_tbl = 1,
        .tbl = bcm56990_b0_lrd_comp_bcmpc_hdl_lth_tx_taps_status,
    },
};

static const bcmlrd_comp_hdl_t
bcm56990_b0_lrd_comp_bcmtm_hdl[] = {
    {
        .hdl_name = "CT_PORT_INFO" /* ct_port_info */,
        .num_tbl = 1,
        .tbl = bcm56990_b0_lrd_comp_bcmtm_hdl_ct_port_info,
    },
    {
        .hdl_name = "DEVICE_INFO" /* device_info */,
        .num_tbl = 1,
        .tbl = bcm56990_b0_lrd_comp_bcmtm_hdl_device_info,
    },
    {
        .hdl_name = "LT_PIPE_MAP_INFO" /* lt_pipe_map_info */,
        .num_tbl = 1,
        .tbl = bcm56990_b0_lrd_comp_bcmtm_hdl_lt_pipe_map_info,
    },
    {
        .hdl_name = "LT_PMAP_INFO" /* lt_pmap_info */,
        .num_tbl = 1,
        .tbl = bcm56990_b0_lrd_comp_bcmtm_hdl_lt_pmap_info,
    },
    {
        .hdl_name = "OBM_PORT_USAGE" /* obm_port_usage */,
        .num_tbl = 1,
        .tbl = bcm56990_b0_lrd_comp_bcmtm_hdl_obm_port_usage,
    },
    {
        .hdl_name = "SCHEDULER_PROFILE_Q_INFO" /* scheduler_profile_q_info */,
        .num_tbl = 1,
        .tbl = bcm56990_b0_lrd_comp_bcmtm_hdl_scheduler_profile_q_info,
    },
};


static const bcmlrd_comp_desc_t bcm56990_b0_lrd_comp_desc[] = {
    {
        .comp_name = "BCMCFG" /* bcmcfg */,
        .num_hdl = 1,
        .hdl = bcm56990_b0_lrd_comp_bcmcfg_hdl,
    },
    {
        .comp_name = "BCMCTH" /* bcmcth */,
        .num_hdl = 11,
        .hdl = bcm56990_b0_lrd_comp_bcmcth_hdl,
    },
    {
        .comp_name = "BCMFP" /* bcmfp */,
        .num_hdl = 15,
        .hdl = bcm56990_b0_lrd_comp_bcmfp_hdl,
    },
    {
        .comp_name = "BCMIMM" /* bcmimm */,
        .num_hdl = 2,
        .hdl = bcm56990_b0_lrd_comp_bcmimm_hdl,
    },
    {
        .comp_name = "BCMLM" /* bcmlm */,
        .num_hdl = 1,
        .hdl = bcm56990_b0_lrd_comp_bcmlm_hdl,
    },
    {
        .comp_name = "BCMPC" /* bcmpc */,
        .num_hdl = 8,
        .hdl = bcm56990_b0_lrd_comp_bcmpc_hdl,
    },
    {
        .comp_name = "BCMTM" /* bcmtm */,
        .num_hdl = 6,
        .hdl = bcm56990_b0_lrd_comp_bcmtm_hdl,
    },
};


const bcmlrd_dev_comp_t bcm56990_b0_lrd_custom = {
   .num_comp = 7,
   .desc = bcm56990_b0_lrd_comp_desc
};

