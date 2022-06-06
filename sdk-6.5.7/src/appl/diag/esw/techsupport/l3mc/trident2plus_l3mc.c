/*
 * $Id: trident2plus_l3mc.c Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * File:    trident2plus_l3mc.c
 * Purpose: Maintains all the debug information for l3mc(layer 3 multicast)
 *          feature for trident2plus.
*/

#include <appl/diag/techsupport.h>
#include <soc/mcm/allenum.h>

extern char * techsupport_l3mc_diag_cmdlist[];
extern char * techsupport_l3mc_sw_dump_cmdlist[];

/* "l3mc" feature's diag command list valid only for Trident2plus */
char * techsupport_l3mc_trident2plus_diag_cmdlist[] = {
    NULL /* Must be the last element in this structure */
};

#ifndef BCM_SW_STATE_DUMP_DISABLE
/* "L3MC" feature's software dump command list valid only for Trident2plus */
char * techsupport_l3mc_trident2plus_sw_dump_cmdlist[] = {
    NULL /* Must be the last element in this structure */
};
#endif /* BCM_SW_STATE_DUMP_DISABLE */


/* Structure that maintains memory list for
 * "l3mc" feature for Trident2plus chipset. */
static soc_mem_t techsupport_l3mc_trident2plus_memory_table_list[] = {
    SOURCE_TRUNK_MAP_MODBASEm,
    PORT_TABm,
    SOURCE_TRUNK_MAP_TABLEm,
    VLAN_MPLSm,
    MY_STATION_TCAMm,
    ING_VLAN_VFI_MEMBERSHIPm,
    L3_IIFm,
    VLAN_TABm,
    L2_HITDA_ONLY_Xm,
    L2_HITDA_ONLY_Ym,
    L2_HITSA_ONLY_Xm,
    L2_HITSA_ONLY_Ym,
    L3_ENTRY_IPV4_MULTICASTm,
    L3_ENTRY_IPV6_MULTICASTm,
    ING_ACTIVE_L3_IIF_PROFILEm,
    INITIAL_ING_L3_NEXT_HOPm,
    L3_IPMC_1m,
    ING_L3_NEXT_HOPm,
    L2MCm,
    L3_IPMCm,
    TRUNK_GROUPm,
    L3_MTU_VALUESm,
    TRUNK_MEMBERm,
    TRUNK_BITMAPm,
    MMU_REPL_GROUP_INFO0m,
    MMU_REPL_GROUP_INFO1m,
    MMU_REPL_HEAD_TBLm,
    MMU_REPL_LIST_TBLm,
    MMU_REPL_GROUP_INITIAL_COPY_COUNTm,
    EGR_GPP_ATTRIBUTESm,
    EGR_IPMCm,
    EGR_L3_INTFm,
    EGR_L3_NEXT_HOPm,
    EGR_VLANm,
    EGR_VLAN_VFI_MEMBERSHIPm,
    EGR_TRILL_RBRIDGE_NICKNAMESm,
    INVALIDm /* Must be the last element in this structure */
};

/* Structure that maintains register list for
 * "l3mc" feature for Trident2plus chipset. */
techsupport_reg_t techsupport_l3mc_trident2plus_reg_list[] = {
    {CPU_CONTROL_1r, register_type_global},
    {PORT_INITIAL_COPY_COUNT_WIDTHr, register_type_global},
    {ING_CONFIG_64r, register_type_global},
    {MMU_TOQ_REPL_PORT_AGG_MAPr, register_type_global},
    {MMU_ENQ_LOGICAL_PORT_TO_PORT_AGG_MAPr, register_type_global},
    {MMU_ENQ_REPL_PORT_AGG_MAPr, register_type_global},
    {EGR_IPMC_CFG2r, register_type_global},
    {INVALIDr, register_type_global } /* Must be the last element in this structure */
};

/* Structure that maintains  diag cmdlist, reg_list, mem_list  for
 * "l3mc" feature for Trident2plus chipset. */
techsupport_data_t techsupport_l3mc_trident2plus_data = {
    techsupport_l3mc_diag_cmdlist,
    techsupport_l3mc_trident2plus_reg_list,
    techsupport_l3mc_trident2plus_memory_table_list,
    techsupport_l3mc_trident2plus_diag_cmdlist
#ifndef BCM_SW_STATE_DUMP_DISABLE
    , techsupport_l3mc_sw_dump_cmdlist,
    techsupport_l3mc_trident2plus_sw_dump_cmdlist
#endif /* BCM_SW_STATE_DUMP_DISABLE */
};

