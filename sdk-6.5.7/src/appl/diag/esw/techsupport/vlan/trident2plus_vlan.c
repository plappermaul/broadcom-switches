/*
 * $Id: trident2plus_vlan.c Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * File:    trident2plus_vlan.c
 * Purpose: Maintains all the debug information for vlan
 *          feature for trident2plus.
*/

#include <appl/diag/techsupport.h>
#include <soc/mcm/allenum.h>

extern char * techsupport_vlan_diag_cmdlist[];
extern char * techsupport_vlan_sw_dump_cmdlist[];

/* "vlan" feature's diag command list only for Trident2plus */
char * techsupport_vlan_trident2plus_diag_cmdlist[] = {
    NULL /* Must be the last element in this structure */
};

#ifndef BCM_SW_STATE_DUMP_DISABLE
/* "vlan" feature's software dump command list only for Trident2plus */
char * techsupport_vlan_trident2plus_sw_dump_cmdlist[] = {
    NULL /* Must be the last element in this structure */
};
#endif /* BCM_SW_STATE_DUMP_DISABLE */


/* Structure that maintains memory list for
 * "vlan" feature for Trident2plus chipset. */
static soc_mem_t techsupport_vlan_trident2plus_memory_table_list[] = {
    SOURCE_TRUNK_MAP_MODBASEm,
    SYSTEM_CONFIG_TABLEm,
    SYSTEM_CONFIG_TABLE_MODBASEm,
    PORT_TABm,
    ING_VLAN_RANGEm,
    SOURCE_VP_2m,
    LPORT_TABm,
    SOURCE_TRUNK_MAP_TABLEm,
    ING_VLAN_TAG_ACTION_PROFILEm,
    VLAN_PROTOCOLm,
    VLAN_PROTOCOL_DATAm,
    VLAN_SUBNETm,
    VLAN_XLATE_LPm,
    VLAN_XLATEm,
    VLAN_MACm,
    SOURCE_VPm,
    VLAN_MPLSm,
    ING_VLAN_VFI_MEMBERSHIPm,
    STG_TABm,
    VLAN_PROFILE_TABm,
    VLAN_TABm,
    L2_HITDA_ONLY_Xm,
    L2_HITDA_ONLY_Ym,
    L2_HITSA_ONLY_Xm,
    L2_HITSA_ONLY_Ym,
    L2_USER_ENTRYm,
    L2_LEARN_INSERT_FAILUREm,
    L2_BULK_REPLACE_MASKm,
    L2_BULK_REPLACE_DATAm,
    L2_BULK_MATCH_MASKm,
    L2_BULK_MATCH_DATAm,
    L2_MOD_FIFOm,
    L2_ENDPOINT_IDm,
    L2Xm,
    ING_DVP_TABLEm,
    TRUNK_CBL_TABLEm,
    PORT_CBL_TABLE_MODBASEm,
    ING_DVP_2_TABLEm,
    L2MCm,
    TRUNK_GROUPm,
    TRUNK_MEMBERm,
    VLAN_PROFILE_2m,
    VFI_PROFILE_2m,
    ING_EN_EFILTER_BITMAPm,
    TRUNK_BITMAPm,
    EGR_DVP_ATTRIBUTEm,
    EGR_GPP_ATTRIBUTESm,
    EGR_MAP_MHm,
    EGR_PORTm,
    EGR_VLAN_CONTROL_1m,
    EGR_VLANm,
    EGR_VLAN_TAG_ACTION_PROFILEm,
    EGR_VLAN_VFI_MEMBERSHIPm,
    EGR_VLAN_XLATEm,
    EGR_VLAN_STGm,
    EGR_VLAN_CONTROL_3m,
    EGR_VLAN_CONTROL_2m,
    EGR_TRILL_RBRIDGE_NICKNAMESm,
    EGR_PORT_1m,
    INVALIDm /* Must be the last element in this structure */
};

/* Structure that maintains register list for
 * "vlan" feature for Trident2plus chipset. */
techsupport_reg_t techsupport_vlan_trident2plus_reg_list[] = {
    {CPU_CONTROL_1r, register_type_global},
    {EGR_CONFIGr, register_type_global},
    {EGR_INGRESS_PORT_TPID_SELECTr, register_type_global},
    {EGR_PVLAN_EPORT_CONTROLr, register_type_global},
    {ING_CONFIG_64r, register_type_global},
    {PORT_INITIAL_COPY_COUNT_WIDTHr, register_type_global},
    {VLAN_CTRLr, register_type_global},
    {VLAN_XLATE_HASH_CONTROLr, register_type_global},
    {ING_OUTER_TPIDr, register_type_global},
    {ING_MPLS_INNER_TPIDr, register_type_global},
    {EGR_OUTER_TPIDr, register_type_global},
    {IARB_LEARN_CONTROLr, register_type_global},
    {CBL_ATTRIBUTEr, register_type_global},
    {MC_CONTROL_1r, register_type_global},
    {INVALIDr, register_type_global } /* Must be the last element in this structure */
};

/* Structure that maintains  diag cmdlist, reg_list, mem_list  for
 * "vlan" feature for Trident2plus chipset. */
techsupport_data_t techsupport_vlan_trident2plus_data = {
    techsupport_vlan_diag_cmdlist,
    techsupport_vlan_trident2plus_reg_list,
    techsupport_vlan_trident2plus_memory_table_list,
    techsupport_vlan_trident2plus_diag_cmdlist
#ifndef BCM_SW_STATE_DUMP_DISABLE
    , techsupport_vlan_sw_dump_cmdlist,
    techsupport_vlan_trident2plus_sw_dump_cmdlist
#endif /* BCM_SW_STATE_DUMP_DISABLE */
};

