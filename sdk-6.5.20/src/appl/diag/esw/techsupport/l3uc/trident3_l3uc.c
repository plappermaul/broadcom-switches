/*
 * 
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 *
 * File:    trident3_l3uc.c
 * Purpose: Maintains all the debug information for l3uc(layer 3 unicast)
 *          feature for trident3.
*/

#include <appl/diag/techsupport.h>
#include <soc/mcm/allenum.h>

extern char * techsupport_l3uc_diag_cmdlist[];
extern char * techsupport_l3uc_sw_dump_cmdlist[];

/* "l3uc" feature's diag command list valid only for trident3 */
char * techsupport_l3uc_trident3_diag_cmdlist[] = {
    NULL /* Must be the last element in this structure */
};

#ifndef BCM_SW_STATE_DUMP_DISABLE
/* "l3uc" feature's software dump command list valid only for trident3 */
char * techsupport_l3uc_trident3_sw_dump_cmdlist[] = {
    NULL /* Must be the last element in this structure */
};
#endif /* BCM_SW_STATE_DUMP_DISABLE */


/* Structure that maintains memory list for
 * "l3uc" feature for trident3 chipset. */
static soc_mem_t techsupport_l3uc_trident3_memory_table_list[] = {
   SOURCE_TRUNK_MAP_MODBASEm,
   ING_DEVICE_PORTm,
   L3_TUNNELm,
   L3_TUNNEL_DATA_ONLYm,
   L3_TUNNEL_ONLYm,
   SOURCE_VP_2m,
   PARSER1_IPV4_IN_IPV6_PREFIX_MATCH_TABLEm,
   PARSER2_IPV4_IN_IPV6_PREFIX_MATCH_TABLEm,
   LPORT_TABm,
   SOURCE_TRUNK_MAP_TABLEm,
   VFP_POLICY_TABLEm,
   VFP_TCAMm,
   L3_IIFm,
   MY_STATION_TCAMm,
   SOURCE_VPm,
   VLAN_ATTRS_1m,
   ING_ACTIVE_L3_IIF_PROFILEm,
   IP_OPTION_CONTROL_PROFILE_TABLEm,
   L3_DEFIPm,
   L3_DEFIP_ALPM_IPV4m,
   L3_DEFIP_ALPM_IPV4_1m,
   L3_DEFIP_ALPM_IPV6_128m,
   L3_DEFIP_ALPM_IPV6_64m,
   L3_DEFIP_ALPM_IPV6_64_1m,
   L3_DEFIP_ALPM_RAWm,
   L3_DEFIP_AUX_HITBIT_UPDATEm,
   L3_DEFIP_AUX_SCRATCHm,
   L3_DEFIP_AUX_TABLEm,
   L3_DEFIP_DATA_ONLYm,
   L3_DEFIP_ONLYm,
   L3_DEFIP_PAIR_128m,
   L3_DEFIP_PAIR_128_DATA_ONLYm,
   L3_DEFIP_PAIR_128_ONLYm,
   L3_ENTRY_DOUBLEm,
   L3_ENTRY_SINGLEm,
   L3_ENTRY_LPm,
   L3_ENTRY_ONLY_SINGLEm,
   L3_ENTRY_ONLY_DOUBLEm,
   L3_IIF_PROFILEm,
   RTAG7_PORT_BASED_HASHm,
   VRF_ATTRS_2m,
   VLAN_TABm,
   INITIAL_L3_ECMPm,
   INITIAL_L3_ECMP_GROUPm,
   INITIAL_PROT_NHI_TABLEm,
   INITIAL_ING_L3_NEXT_HOPm,
   L3_DEFIP_ALPM_HIT_ONLYm,
   L3_DEFIP_HIT_ONLYm,
   L3_DEFIP_PAIR_128_HIT_ONLYm,
   L3_ENTRY_HIT_ONLYm,
   EGR_MASK_MODBASEm,
   ING_L3_NEXT_HOPm,
   ING_SNATm,
   L3_ECMPm,
   L3_ECMP_COUNTm,
   EGR_MASKm,
   L3_MTU_VALUESm,
   TRUNK_GROUPm,
   TRUNK_MEMBERm,
   TRUNK_BITMAPm,
   EMIRROR_CONTROL3m,
   MMU_REPL_GROUP_INITIAL_COPY_COUNT_SC0m,
   EGR_L3_INTFm,
   EGR_L3_NEXT_HOPm,
   EGR_NAT_PACKET_EDIT_INFOm,
   EGR_PORTm,
   EGR_GPP_ATTRIBUTESm,
   EGR_IP_TUNNELm,
   EGR_VLANm,
   L2_ENTRY_HASH_CONTROLm,
   L3_ENTRY_HASH_CONTROLm,
   UFT_SHARED_BANKS_CONTROLm,
   INVALIDm /* Must be the last element in this structure */
};

/* Structure that maintains register list for
 * "l3uc" feature for trident3 chipset. */
techsupport_reg_t techsupport_l3uc_trident3_reg_list[] = {
   {CPU_CONTROL_1r, register_type_global },
   {CPU_CONTROL_Mr, register_type_global },
   {DOS_CONTROLr, register_type_global },
   {DOS_CONTROL_2r, register_type_global },
   {EGR_EL3_ECC_PARITY_CONTROLr, register_type_global },
   {EGR_FLEX_CTR_COUNTER_UPDATE_CONTROL_0r, register_type_global },
   {EGR_FLEX_CTR_PKT_ATTR_SELECTOR_KEY_0r, register_type_global },
   {ING_CONFIG_64r, register_type_global },
   {ING_FLEX_CTR_COUNTER_UPDATE_CONTROL_4r, register_type_global },
   {ING_FLEX_CTR_PKT_ATTR_SELECTOR_KEY_0r, register_type_global },
   {ING_FLEX_CTR_PKT_ATTR_SELECTOR_KEY_12r, register_type_global },
   {ING_FLEX_CTR_PKT_ATTR_SELECTOR_KEY_16r, register_type_global },
   {ING_FLEX_CTR_PKT_ATTR_SELECTOR_KEY_4r, register_type_global },
   {ING_FLEX_CTR_PKT_ATTR_SELECTOR_KEY_8r, register_type_global },
   {ING_MISC_CONFIGr, register_type_global },
   {ING_MISC_CONFIG2r, register_type_global },
   {PARSER1_IPV6_MIN_FRAG_SIZEr, register_type_global },
   {PARSER2_IPV6_MIN_FRAG_SIZEr, register_type_global },
   {L3_DEFIP_ALPM_CFGr, register_type_global },
   {L3_DEFIP_AUX_CTRLr, register_type_global },
   {L3_DEFIP_AUX_CTRL_1r, register_type_global },
   {L3_DEFIP_KEY_SELr, register_type_global },
   {L3_DEFIP_RPF_CONTROLr, register_type_global },
   {PORT_INITIAL_COPY_COUNT_WIDTHr, register_type_global },
   {PROTOCOL_PKT_CONTROLr, register_type_global },
   {INVALIDr, register_type_global } /* Must be the last element in this structure */
};

/* Structure that maintains  diag cmdlist, reg_list, mem_list  for
 * "l3uc" feature for trident3 chipset. */
techsupport_data_t techsupport_l3uc_trident3_data = {
    techsupport_l3uc_diag_cmdlist,
    techsupport_l3uc_trident3_reg_list,
    techsupport_l3uc_trident3_memory_table_list,
    techsupport_l3uc_trident3_diag_cmdlist
#ifndef BCM_SW_STATE_DUMP_DISABLE
    , techsupport_l3uc_sw_dump_cmdlist,
    techsupport_l3uc_trident3_sw_dump_cmdlist
#endif /* BCM_SW_STATE_DUMP_DISABLE */
};

