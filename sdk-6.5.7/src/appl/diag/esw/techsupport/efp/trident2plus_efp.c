/*
 * $Id: trident2plus_efp.c Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * File:    trident2plus_efp.c
 * Purpose: Maintains all the debug information for efp
 *          feature for trident2plus.
*/

#include <appl/diag/techsupport.h>
#include <soc/mcm/allenum.h>

extern char * techsupport_efp_diag_cmdlist[];
extern char * techsupport_efp_sw_dump_cmdlist[];

/* "efp" feature's diag command list valid only for Trident2plus */
char * techsupport_efp_trident2plus_diag_cmdlist[] = {
     NULL /* Must be the last element in this structure */
};

#ifndef BCM_SW_STATE_DUMP_DISABLE
/* "efp" feature's software dump command list valid only for Trident2plus */
char * techsupport_efp_trident2plus_sw_dump_cmdlist[] = {
    NULL /* Must be the last element in this structure */
};
#endif /* BCM_SW_STATE_DUMP_DISABLE */


/* Structure that maintains memory list for
 * "efp" feature for Trident2plus chipset. */
static soc_mem_t techsupport_efp_trident2plus_memory_table_list[] = {
    EGR_PORTm,
    EGR_LPORT_PROFILEm,
    EFP_TCAMm,
    EFP_POLICY_TABLEm,
    EFP_METER_TABLE_Xm,
    EFP_METER_TABLE_Ym,
    EFP_COUNTER_TABLE_Xm,
    EFP_COUNTER_TABLE_Ym,
    INVALIDm /* Must be the last element in this structure */
};

/* Structure that maintains register list for
 * "efp" feature for Trident2plus chipset. */
techsupport_reg_t techsupport_efp_trident2plus_reg_list[] = {
    {EFP_SLICE_MAPr, register_type_global},
    {EFP_SLICE_CONTROLr, register_type_global},
    {EFP_KEY4_MDL_SELECTORr, register_type_global},
    {EFP_KEY4_DVP_SELECTORr, register_type_global},
    {EFP_DEST_PORT_SELECTORr, register_type_global},
    {EFP_CLASSID_SELECTORr, register_type_global},
    {EFP_METER_CONTROLr, register_type_global},
    {EGR_BYPASS_CTRLr, register_type_global},
    {EGR_DROP_VECTORr, register_type_global},
    {INVALIDr, register_type_global } /* Must be the last element in this structure */
};

/* Structure that maintains  diag cmdlist, reg_list, mem_list  for
 * "efp" feature for Trident2plus chipset. */
techsupport_data_t techsupport_efp_trident2plus_data = {
    techsupport_efp_diag_cmdlist,
    techsupport_efp_trident2plus_reg_list,
    techsupport_efp_trident2plus_memory_table_list,
    techsupport_efp_trident2plus_diag_cmdlist
#ifndef BCM_SW_STATE_DUMP_DISABLE
    , techsupport_efp_sw_dump_cmdlist,
    techsupport_efp_trident2plus_sw_dump_cmdlist
#endif /* BCM_SW_STATE_DUMP_DISABLE */
};

