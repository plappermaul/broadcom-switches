/*
 * $Id: trident2plus_ifp.c Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * File:    trident2plus_ifp.c
 * Purpose: Maintains all the debug information for ifp
 *          feature for trident2plus.
*/

#include <appl/diag/techsupport.h>
#include <soc/mcm/allenum.h>

extern char * techsupport_ifp_diag_cmdlist[];
extern char * techsupport_ifp_sw_dump_cmdlist[];

/* "ifp" feature's diag command list */
char * techsupport_ifp_trident2plus_diag_cmdlist[] = {
    NULL /* Must be the last element in this structure */
};

#ifndef BCM_SW_STATE_DUMP_DISABLE
/* "ifp" feature's software dump command list */
char * techsupport_ifp_trident2plus_sw_dump_cmdlist[] = {
    NULL /* Must be the last element in this structure */
};
#endif /* BCM_SW_STATE_DUMP_DISABLE */

/* Structure that maintains memory list for
 * "ifp" feature for Trident2plus chipset. */
static soc_mem_t techsupport_ifp_trident2plus_memory_table_list[] = {
    PORT_TABm,
    LPORT_TABm,
    PFS_PACKET_TYPE_OFFSETm,
    FP_PORT_FIELD_SELm,
    TTL_FNm,
    TOS_FNm,
    TCP_FNm,
    FP_COUNTER_TABLE_Xm,
    FP_COUNTER_TABLE_Ym,
    FP_GLOBAL_MASK_TCAM_Xm,
    FP_GLOBAL_MASK_TCAM_Ym,
    FP_GM_FIELDSm,
    FP_POLICY_TABLEm,
    FP_SLICE_KEY_CONTROLm,
    FP_SLICE_MAPm,
    FP_RANGE_CHECKm,
    FP_TCAMm,
    FP_I2E_CLASSID_SELECTm,
    FP_HG_CLASSID_SELECTm,
    FP_PORT_METER_MAPm,
    FP_METER_TABLEm,
    FP_METER_CONTROLm,
    IFP_REDIRECTION_PROFILEm,
    IFP_COS_MAPm,
    INVALIDm /* Must be the last element in this structure */
};

/* Structure that maintains register list for
 * "ifp" feature for Trident2plus chipset. */
techsupport_reg_t techsupport_ifp_trident2plus_reg_list[] = {
    {CPU_CONTROL_0r, register_type_global},
    {CPU_CONTROL_1r, register_type_global},
    {ING_BYPASS_CTRLr, register_type_global},
    {SW2_FP_DST_ACTION_CONTROLr, register_type_global},
    {IFP_GM_LOGICAL_TO_PHYSICAL_MAPPINGr, register_type_global},
    {FP_SLICE_INDEX_CONTROLr, register_type_global},
    {FP_SLICE_ENABLEr, register_type_global},
    {FP_FORCE_FORWARDING_FIELDr, register_type_global},
    {FP_SLICE_METER_MAP_ENABLEr, register_type_global},
    {ING_CONFIG_64r, register_type_global},
    {INVALIDr, register_type_global } /* Must be the last element in this structure */
};

/* Structure that maintains  diag cmdlist, reg_list, mem_list  for
 * "ifp" feature for Trident2plus chipset. */
techsupport_data_t techsupport_ifp_trident2plus_data = {
    techsupport_ifp_diag_cmdlist,
    techsupport_ifp_trident2plus_reg_list,
    techsupport_ifp_trident2plus_memory_table_list,
    techsupport_ifp_trident2plus_diag_cmdlist
#ifndef BCM_SW_STATE_DUMP_DISABLE
    , techsupport_ifp_sw_dump_cmdlist,
    techsupport_ifp_trident2plus_sw_dump_cmdlist
#endif /* BCM_SW_STATE_DUMP_DISABLE */
};

