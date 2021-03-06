/* 
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2022 Broadcom Inc. All rights reserved.
 * $
*/

#ifndef __ARAD_PP_VTT_INCLUDED__
#define __ARAD_PP_VTT_INCLUDED__

#include <soc/dpp/SAND/Utils/sand_header.h>

#include <soc/dpp/SAND/Management/sand_general_macros.h>
#include <soc/dpp/SAND/Management/sand_error_code.h>

#include <soc/dpp/ARAD/ARAD_PP/arad_pp_dbal.h>
#include <soc/dpp/ARAD/ARAD_PP/arad_pp_isem_access.h>



#define  ARAD_PP_ISEM_ACCESS_NULL_PREFIX               (0) 
#define  ARAD_PP_ISEM_ACCESS_BRIDGE_PREFIX             (0) 

#define  ARAD_PP_ISEM_ACCESS_OVERLAY_SIP_PREFIX        (1) 
#define  ARAD_PP_ISEM_ACCESS_IP_PREFIX                 (1) 

#define  ARAD_PP_ISEM_ACCESS_FLEXIBLE_Q_IN_Q_DOUBLE    (2) 
#define  ARAD_PP_ISEM_ACCESS_BRIDGE_DT                 (2) 
#define  ARAD_PP_ISEM_ACCESS_FLEXIBLE_Q_IN_Q_SINGLE    (2) 
#define  ARAD_PP_ISEM_ACCESS_BRIDGE_ST                 (2) 

#define  ARAD_PP_ISEM_ACCESS_ISID_DOMAIN_ISID_PREFIX   (3) 
#define  ARAD_PP_ISEM_ACCESS_L2TP_PREFIX			   (3) 
#define  ARAD_PP_ISEM_ACCESS_FC_PREFIX                 (3) 
#define  ARAD_PP_ISEM_ACCESS_TRILL_VSI_PREFIX          (3) 

#define  ARAD_PP_ISEM_ACCESS_TRILL_FINE_A_GRAINED_PREFIX (4) 
#define  ARAD_PP_ISEM_ACCESS_EXTENDER_PE_PREFIX          (4) 
#define  ARAD_PP_ISEM_ACCESS_PPPOE_PREFIX			     (4) 

#define  ARAD_PP_ISEM_ACCESS_VD_OUTER_PCP_PREFIX       (5) 

#define  ARAD_PP_ISEM_ACCESS_RPA_ID_PREFIX             (6) 
#define  ARAD_PP_ISEM_ACCESS_BFD_STAT_TT_ISB_PREFIX    (6)  

#define  ARAD_PP_ISEM_ACCESS_INNER_TPID_PREFIX         (7) 
#define  ARAD_PP_ISEM_ACCESS_BFD_STAT_VT_PREFIX        (7)  

#define  ARAD_PP_ISEM_ACCESS_TRILL_DESIGNATED_VID_PREFIX (8) 
#define  ARAD_PP_ISEM_ACCESS_BFD_STAT_TT_ISA_PREFIX      (8) 
#define  ARAD_PP_ISEM_ACCESS_TEST2_VT_ISA_PREFIX         (8) 

#define  ARAD_PP_ISEM_ACCESS_IP_VSI_PREFIX             (9) 
#define  ARAD_PP_ISEM_ACCESS_OAM_STAT_VT_PREFIX        (9)  

#define  ARAD_PP_ISEM_ACCESS_OAM_STAT_TT_PREFIX           (10) 
#define  ARAD_PP_ISEM_ACCESS_EXTENDER_UNTAG_CHECK_PREFIX  (10) 
#define  ARAD_PP_ISEM_ACCESS_CUSTOM_PP_PORT_TUNNEL_PREFIX (10) 
#define  ARAD_PP_ISEM_ACCESS_VLAN_DOMAIN_MPLS_PREFIX      (10) 

#define  ARAD_PP_ISEM_ACCESS_MPLS_L1ELI_PREFIX         (11) 
#define  ARAD_PP_ISEM_ACCESS_MPLS_L1L3ELI_PREFIX       (11) 
#define  ARAD_PP_ISEM_ACCESS_MPLS_L1L2ELI_PREFIX       (11) 

#define  ARAD_PP_ISEM_ACCESS_MPLS_ELI_UNINDEXED_PREFIX (12) 
#define  ARAD_PP_ISEM_ACCESS_MPLS_L1_PREFIX            (12) 
#define  ARAD_PP_ISEM_ACCESS_MPLS_L1L2_PREFIX          (12) 

#define  ARAD_PP_ISEM_ACCESS_MPLS_L2_PREFIX            (13) 
#define  ARAD_PP_ISEM_ACCESS_MPLS_L3_PREFIX            (13) 
#define  ARAD_PP_ISEM_ACCESS_MPLS_L1L3_PREFIX          (13) 
#define  ARAD_PP_ISEM_ACCESS_MPLS_UNINDEXED_PREFIX     (13) 

#define  ARAD_PP_ISEM_ACCESS_MPLS_L2ELI_PREFIX         (14) 

#define  ARAD_PP_ISEM_ACCESS_TRILL_NICK_PREFIX         (15) 
#define  ARAD_PP_ISEM_ACCESS_MPLS_PWE_GAL              (15) 

#define  ARAD_PP_ISEM_ACCESS_VTEP_INDEX_SIP_VRF_MY_PREFIX (16) 


#define  ARAD_PP_ISEM_ACCESS_MPLS_L1_PREFIX_4bit            (4) 
#define  ARAD_PP_ISEM_ACCESS_MPLS_L2_PREFIX_4bit            (5) 
#define  ARAD_PP_ISEM_ACCESS_MPLS_L3_PREFIX_4bit            (5) 
#define  ARAD_PP_ISEM_ACCESS_MPLS_L1L3_PREFIX_4bit          (5) 
#define  ARAD_PP_ISEM_ACCESS_MPLS_L1L2_PREFIX_4bit          (4) 

#define ARAD_PP_ISEM_ACCESS_QINQ_COMPRESSED_TPID1_PREFIX     (0) 
#define ARAD_PP_ISEM_ACCESS_QINQ_COMPRESSED_TPID2_PREFIX     (1) 
#define ARAD_PP_ISEM_ACCESS_QINANY_TPID1_PREFIX              (2) 
#define ARAD_PP_ISEM_ACCESS_QINANY_TPID2_PREFIX              (3) 
#define ARAD_PP_ISEM_ACCESS_QINANY_PCP_TPID1_PREFIX          (4) 
#define ARAD_PP_ISEM_ACCESS_QINANY_PCP_TPID2_PREFIX          (5) 
#define ARAD_PP_ISEM_ACCESS_1Q_TPID1_PREFIX                  (6) 
#define ARAD_PP_ISEM_ACCESS_1Q_TPID2_PREFIX                  (7) 
#define ARAD_PP_ISEM_ACCESS_1Q_COMPRESSED_TPID1_PREFIX       (8) 
#define ARAD_PP_ISEM_ACCESS_1Q_COMPRESSED_TPID2_PREFIX       (9) 
#define ARAD_PP_ISEM_ACCESS_1Q_COMPRESSED_PCP_TPID1_PREFIX   (10)
#define ARAD_PP_ISEM_ACCESS_1Q_COMPRESSED_PCP_TPID2_PREFIX   (11)



#define  ARAD_PP_ISEM_ACCESS_PON_UNTAGGED_PREFIX       (1) 
#define  ARAD_PP_ISEM_ACCESS_PON_TLS_PREFIX            (1) 
#define  ARAD_PP_ISEM_ACCESS_PON_ONE_TAG_PREFIX        (2) 
#define  ARAD_PP_ISEM_ACCESS_PON_TWO_TAGS_PREFIX       (4) 
#define  ARAD_PP_ISEM_ACCESS_CUS_PON_TWO_TAGS_PREFIX   (4) 
#define  ARAD_PP_ISEM_ACCESS_PON_VD_OUTER_PCP_PREFIX   (3) 
#define  ARAD_PP_ISEM_ACCESS_IPV4_SPOOF_PREFIX         (8) 


#define ARAD_PP_ISEM_ACCESS_EVB_DEFAULT_S_CHANNEL (0x1)







#define ARAD_PP_ISEM_ACCESS_QLFR_ETH_NULL               (0x00)  
#define ARAD_PP_ISEM_ACCESS_QLFR_ETH_OUTER_TPID1        (0x08)  
#define ARAD_PP_ISEM_ACCESS_QLFR_ETH_OUTER_TPID2        (0x10)  
#define ARAD_PP_ISEM_ACCESS_QLFR_ETH_INNER_TPID1        (0x01)  
#define ARAD_PP_ISEM_ACCESS_QLFR_ETH_INNER_TPID2        (0x02)  
#define ARAD_PP_ISEM_ACCESS_QLFR_ETH_OUTER_PCP          (0x04)  

#define ARAD_PP_ISEM_ACCESS_QLFR_ETH_MASK_NXT_PROTOCOL  (0x780)
#define ARAD_PP_ISEM_ACCESS_QLFR_ETH_MASK_ENCAP         (0x060)
#define ARAD_PP_ISEM_ACCESS_QLFR_ETH_MASK_OUTER_TPID    (0x018)
#define ARAD_PP_ISEM_ACCESS_QLFR_ETH_MASK_OUTER_PCP     (0x004)
#define ARAD_PP_ISEM_ACCESS_QLFR_ETH_MASK_INNER_TPID    (0x003)
#define ARAD_PP_ISEM_ACCESS_QLFR_ETH_MASK_ENCAP_PPPOE   (0x020)

#define ARAD_PP_ISEM_ACCESS_QLFR_ETH_NEXT_PROTOCOL_MPLS (0xF << 7)
#define ARAD_PP_ISEM_ACCESS_QLFR_ETH_NEXT_PROTOCOL_MAC_IN_MAC (9 << 7)
#define ARAD_PP_ISEM_ACCESS_QLFR_ETH_NEXT_PROTOCOL_ARP (10 << 7)
#define ARAD_PP_ISEM_ACCESS_QLFR_ETH_NEXT_PROTOCOL_CFM (11 << 7)
#define ARAD_PP_ISEM_ACCESS_QLFR_ETH_NEXT_PROTOCOL_MISS (0 << 7)
#define ARAD_PP_ISEM_ACCESS_QLFR_ETH_NEXT_PPPOE_ENCAP   (1 << 6)


#define ARAD_PP_ISEM_ACCESS_QLFR_MPLS_L1                (0x006)  
#define ARAD_PP_ISEM_ACCESS_QLFR_MPLS_L2ELI             (0x000)
#define ARAD_PP_ISEM_ACCESS_QLFR_MPLS_L3ELI             (0x000)
#define ARAD_PP_ISEM_ACCESS_QLFR_MPLS_L1GAL             (0x010)
#define ARAD_PP_ISEM_ACCESS_QLFR_MPLS_L2GAL             (0x040)
#define ARAD_PP_ISEM_ACCESS_QLFR_MPLS_GAL               (0x004)

#define ARAD_PP_ISEM_ACCESS_QLFR_MPLS_MASK_BOS_NEXT_PROT  (0x781)
#define ARAD_PP_ISEM_ACCESS_QLFR_MPLS_MASK_NEXT_PROT      (0x780)
#define ARAD_PP_ISEM_ACCESS_QLFR_MPLS_MASK_L1             (0x006)
#define ARAD_PP_ISEM_ACCESS_QLFR_MPLS_MASK_L2             (0x018)
#define ARAD_PP_ISEM_ACCESS_QLFR_MPLS_MASK_L3             (0x060)
#define ARAD_PP_ISEM_ACCESS_QLFR_MPLS_MASK_R1_V           (0x002)
#define ARAD_PP_ISEM_ACCESS_QLFR_MPLS_MASK_R2_V           (0x008)
#define ARAD_PP_ISEM_ACCESS_QLFR_MPLS_MASK_R3_V           (0x020)
#define ARAD_PP_ISEM_ACCESS_QLFR_MPLS_MASK_R1_W           (0x004)
#define ARAD_PP_ISEM_ACCESS_QLFR_MPLS_MASK_R2_W           (0x010)
#define ARAD_PP_ISEM_ACCESS_QLFR_MPLS_MASK_R3_W           (0x040)

#define ARAD_PP_ISEM_ACCESS_QLFR_IP_NEXT_PROTOCOL_MPLS    (0xF << 7)

#define ARAD_PP_ISEM_ACCESS_16B_INST_PP_PORT_KEY_VAR_X_LSBS(X) ((SOC_IS_JERICHO(unit) ? 0x4A8 : 0x448) | ((X-1) << 12))

#define ARAD_PP_ISEM_ACCESS_VT_PROFILE_MASK                     (SOC_IS_JERICHO(unit) ? 0x1F:0x7)
#define ARAD_PP_ISEM_ACCESS_VT_PROFILE_MASK_EXCEPT_INITIAL_VID  (SOC_IS_JERICHO(unit) ? 0x1E:0x6)
#define ARAD_PP_ISEM_ACCESS_VT_PROFILE_MASK_INITIAL_VID         (0x1)
#define ARAD_PP_ISEM_ACCESS_VT_PROFILE_MASK_DOUBLE_TAG          (0x2)

typedef enum
{
  ARAD_PP_ISEM_ACCESS_PROCESSING_CODE_TYPE_TM                     = 0x0,
  ARAD_PP_ISEM_ACCESS_PROCESSING_CODE_TYPE_MIM                    = 0x1,
  ARAD_PP_ISEM_ACCESS_PROCESSING_CODE_TYPE_COMPATIBLE_MC          = 0x2,
  ARAD_PP_ISEM_ACCESS_PROCESSING_CODE_TYPE_IP                     = 0x3,
  ARAD_PP_ISEM_ACCESS_PROCESSING_CODE_TYPE_TRILL                  = 0x4,
  ARAD_PP_ISEM_ACCESS_PROCESSING_CODE_TYPE_TRILL_DESIGNATED_VLAN  = 0x5,
  ARAD_PP_ISEM_ACCESS_PROCESSING_CODE_TYPE_MPLS                   = 0x6,
  ARAD_PP_ISEM_ACCESS_PROCESSING_CODE_TYPE_ARP                    = 0x7,
  ARAD_PP_ISEM_ACCESS_PROCESSING_CODE_TYPE_CUSTOM_1               = 0x8,
  ARAD_PP_ISEM_ACCESS_PROCESSING_CODE_TYPE_CUSTOM_2               = 0x9,
  ARAD_PP_ISEM_ACCESS_PROCESSING_CODE_TYPE_UNKNOWN_L3             = 0xa,
  ARAD_PP_ISEM_ACCESS_PROCESSING_CODE_TYPE_BRIDGE                 = 0xb,
  ARAD_PP_ISEM_ACCESS_PROCESSING_CODE_TYPE_MY_B_MAC_MC_BRIDGE     = 0xc,
  ARAD_PP_ISEM_ACCESS_PROCESSING_CODE_TYPE_NUM                    = 0xD
} ARAD_PP_ISEM_ACCESS_PROCESSING_CODE_TYPE;



uint32 arad_pp_vtt_init(int unit);


uint32 arad_pp_dbal_vtt_sw_db_get(uint32 unit, ARAD_PP_ISEM_ACCESS_KEY  *isem_key,
                                               uint32                   *nof_tables,
                                               SOC_DPP_DBAL_SW_TABLE_IDS  dbal_tables_id[3],
                                               uint8 *is_duplicate);


uint32 arad_pp_dbal_vtt_fc_vsan_mode_update(uint32 unit, uint32 is_vsan_from_vsi);
uint32 arad_pp_dbal_vtt_fcoe_npv_mode_no_my_mac_set(int unit, int enable);




typedef enum
{

    ARAD_PP_DBAL_VT_PROG_INVALID = -1,
    
    ARAD_PP_DBAL_VT_PROG_TM,
    ARAD_PP_DBAL_VT_PROG_TM_STAT,
    
    ARAD_PP_DBAL_VT_PROG_VDxINITIALVID,
    
    ARAD_PP_DBAL_VT_PROG_VDxOUTERVID,
    
    ARAD_PP_DBAL_VT_PROG_VDxINITIALVID_L1,
    
    ARAD_PP_DBAL_VT_PROG_VDxOUTERVID_L1,
    
    ARAD_PP_DBAL_VT_PROG_VDxOUTER_INNER_VID_L1,

    ARAD_PP_DBAL_VT_PROG_VDxOUTER_INNER_VID_OR_OUTER_VID_L1,

    ARAD_PP_DBAL_VT_PROG_VD_INITIALVID_L1FRR,
    ARAD_PP_DBAL_VT_PROG_VD_OUTERVID_L1FRR,
    ARAD_PP_DBAL_VT_PROG_VD_OUTER_OR_VD_OUTER_PCP,
    ARAD_PP_DBAL_VT_PROG_VD_OUTER_INNER_OR_VD_OUTER_INNER_OUTERPCP,
    ARAD_PP_DBAL_VT_PROG_HIGH_VD_OUTER_INNER_OUTERPCP_OR_VD_OUTER_INNER_OR_VD_OUTER,
    ARAD_PP_DBAL_VT_PROG_VD_INITIALVID_OR_VD_INITIALVID,
    ARAD_PP_DBAL_VT_PROG_VD_OUTER_OR_VD_OUTER,
    ARAD_PP_DBAL_VT_PROG_VD_OUTER_INNER_OR_VD_OUTER_INNER,
    ARAD_PP_DBAL_VT_PROG_INDX_VD_INITIALVID_FRR_L1,
    ARAD_PP_DBAL_VT_PROG_INDX_VD_OUTERVID_FRR_L1,

    
    ARAD_PP_DBAL_VT_PROG_VDxINITIALVID_TRILL,
    ARAD_PP_DBAL_VT_PROG_VDxOUTERVID_TRILL,
    ARAD_PP_DBAL_VT_PROG_VDxOUTER_INNER_VID_TRILL,
    ARAD_PP_DBAL_VT_PROG_VD_DESIGNATED_VID_TRILL,

    ARAD_PP_DBAL_VT_PROG_VDxINITIALVID_L1_L2ELI,
    ARAD_PP_DBAL_VT_PROG_VDxOUTERVID_L1_L2ELI,

    ARAD_PP_DBAL_VT_PROG_VDxINITIALVID_L1L2,
    ARAD_PP_DBAL_VT_PROG_VDxOUTERVID_L1L2,

    ARAD_PP_DBAL_VT_PROG_INDEXED_VDxINITIALVID_L1,
    ARAD_PP_DBAL_VT_PROG_INDEXED_VDxOUTERVID_L1,
    ARAD_PP_DBAL_VT_PROG_MPLS_PORT_L1,
    ARAD_PP_DBAL_VT_PROG_INDEXED_VDxINITIALVID_L3,
    ARAD_PP_DBAL_VT_PROG_INDEXED_VDxOUTERVID_L3,

    ARAD_PP_DBAL_VT_PROG_IPV4_VDxINITIALVID,
    ARAD_PP_DBAL_VT_PROG_IPV4_VDxOUTERVID,
    ARAD_PP_DBAL_VT_PROG_IPV4_PORT,
    ARAD_PP_DBAL_VT_PROG_EVB_UN_C_TAG,
    ARAD_PP_DBAL_VT_PROG_EVB_S_TAG,
    ARAD_PP_DBAL_VT_PROG_DOUBLE_TAG_PRIORITY_INITIAL_VID,
    ARAD_PP_DBAL_VT_PROG_DOUBLE_TAG_INITIAL_VID,
    ARAD_PP_DBAL_VT_PROG_IPV4_VDxINITIALVID_AFTER_RCY,
    ARAD_PP_DBAL_VT_PROG_IPV4_VDxOUTERVID_AFTER_RCY,
    ARAD_PP_DBAL_VT_PROG_SINGLE_TAG_5_TUPPLE_QINQ,
    ARAD_PP_DBAL_VT_PROG_DOUBLE_TAG_5_TUPPLE_QINQ,
    ARAD_PP_DBAL_VT_PROG_UNTAGGED_TST1,
    ARAD_PP_DBAL_VT_PROG_ONE_TAG_TST1,
    ARAD_PP_DBAL_VT_PROG_DOUBLE_TAG_TST1,
    ARAD_PP_DBAL_VT_PROG_UNTAGGED_VRRP,
    ARAD_PP_DBAL_VT_PROG_ONE_TAG_VRRP,
    ARAD_PP_DBAL_VT_PROG_DOUBLE_TAG_VRRP,
    ARAD_PP_DBAL_VT_PROG_UNTAGGED_EXP_NULL_TST1,
    ARAD_PP_DBAL_VT_PROG_ONE_TAG_EXP_NULL_TST1,
    ARAD_PP_DBAL_VT_PROG_DOUBLE_TAG_EXP_NULL_TST1,
    ARAD_PP_DBAL_VT_PROG_OUTER_INNER_PCP_1_TST2,
    ARAD_PP_DBAL_VT_PROG_OUTER_INNER_PCP_2_TST2,
    ARAD_PP_DBAL_VT_PROG_OUTER_PCP_1_TST2,
    ARAD_PP_DBAL_VT_PROG_OUTER_PCP_2_TST2,
    ARAD_PP_DBAL_VT_PROG_OUTER_INNER_1_TST2,
    ARAD_PP_DBAL_VT_PROG_OUTER_INNER_2_TST2,
    ARAD_PP_DBAL_VT_PROG_OUTER_1_TST2,
    ARAD_PP_DBAL_VT_PROG_OUTER_2_TST2,
    ARAD_PP_DBAL_VT_PROG_UNTAGGED_TST2,
    ARAD_PP_DBAL_VT_PROG_VT_TEST2,
    ARAD_PP_DBAL_VT_PROG_EXTENDER_PE,
    ARAD_PP_DBAL_VT_PROG_EXTENDER_PE_UT,
    ARAD_PP_DBAL_VT_PROG_VT_CUSTOM_PP_PORT_TUNNEL,

    ARAD_PP_DBAL_VT_PROG_VD_INITIALVID_VLAN_DOMAIN_MPLS_L1,
    ARAD_PP_DBAL_VT_PROG_PON_UNTAGGED,
    ARAD_PP_DBAL_VT_PROG_PON_ONE_TAG,
    ARAD_PP_DBAL_VT_PROG_PON_TWO_TAGS,
    ARAD_PP_DBAL_VT_PROG_PON_TWO_TAGS_VS_TUNNEL_ID,
    ARAD_PP_DBAL_VT_PROG_SECTION_OAM_ONE_TAG,
    ARAD_PP_DBAL_VT_PROG_SECTION_OAM_TWO_TAGS,

    ARAD_PP_DBAL_VT_PROG_INDEXED_VDxINITIALVID_MPLS4_L1L2,
    ARAD_PP_DBAL_VT_PROG_INDEXED_VDxOUTERVID_MPLS4_L1L2,

    ARAD_PP_DBAL_VT_PROG_IPV6_VDxINITIALVID,
    ARAD_PP_DBAL_VT_PROG_IPV6_VDxOUTERVID,
    ARAD_PP_DBAL_VT_PROG_IPV6_VDxINITIALVID_AFTER_RECYCLE,
    ARAD_PP_DBAL_VT_PROG_IPV6_VDxOUTERVID_AFTER_RECYCLE,
    ARAD_PP_DBAL_VT_PROG_GENERALIZED_RCH0,
    ARAD_PP_DBAL_VT_PROG_GENERALIZED_RCH1,

	ARAD_PP_DBAL_VT_PROG_3MTSE_PLUS_FRR_VDxINITIALVID_L2_L1,
	ARAD_PP_DBAL_VT_PROG_3MTSE_PLUS_FRR_VDxOUTERVID_L2_L1,
	ARAD_PP_DBAL_VT_PROG_3MTSE_PLUS_FRR_VDxINITIALVID_L2_L4B_L5GAL,
	ARAD_PP_DBAL_VT_PROG_3MTSE_PLUS_FRR_VDxOUTERVID_L2_L4B_L5GAL,
	ARAD_PP_DBAL_VT_PROG_2MTSE_PLUS_FRR_VDxINITIALVID_FRR,
	ARAD_PP_DBAL_VT_PROG_2MTSE_PLUS_FRR_VDxOUTERVID_FRR,
	ARAD_PP_DBAL_VT_PROG_2MTSE_PLUS_FRR_VDxOUTER_INNER_VID_FRR,
	ARAD_PP_DBAL_VT_PROG_2MTSE_PLUS_FRR_VDxOUTER_INNER_PCP_VID_FRR,

    ARAD_PP_DBAL_VT_NOF_PROGRAMS,  




    ARAD_PP_DBAL_TT_PROG_INVALID = -1,
    
    ARAD_PP_DBAL_TT_PROG_TM,
    ARAD_PP_DBAL_TT_PROG_OAM_STAT,
    ARAD_PP_DBAL_TT_PROG_BFD_STAT,
    ARAD_PP_DBAL_TT_PROG_MLDP_OVERLAY_AFTER_RCY,
    ARAD_PP_DBAL_TT_PROG_MPLS_L2,
    ARAD_PP_DBAL_TT_PROG_MPLS_L3,
    ARAD_PP_DBAL_TT_PROG_MPLS_L2_L3_ELI,
    ARAD_PP_DBAL_TT_PROG_INDEXED_L1_L1,
    ARAD_PP_DBAL_TT_PROG_INDEXED_L1_L1_GAL,
    ARAD_PP_DBAL_TT_PROG_INDEXED_L1_L2,
    ARAD_PP_DBAL_TT_PROG_INDEXED_L1_L2_GAL,
    ARAD_PP_DBAL_TT_PROG_INDEXED_L2_L2,
    ARAD_PP_DBAL_TT_PROG_INDEXED_L2_L3_GAL,
    ARAD_PP_DBAL_TT_PROG_INDEXED_L2_L3_ELI,
    ARAD_PP_DBAL_TT_PROG_INDEXED_L1_L2_INRIF,
    ARAD_PP_DBAL_TT_PROG_INDEXED_L1_L2ELI_INRIF,
    ARAD_PP_DBAL_TT_PROG_INDEXED_L1_L2_L3ELI_INRIF,
    ARAD_PP_DBAL_TT_PROG_INDEXED_L3_L2,
    ARAD_PP_DBAL_TT_PROG_INDEXED_MPLS_PORT_L3_L2,
    ARAD_PP_DBAL_TT_PROG_INDEXED_L3_L4,
    ARAD_PP_DBAL_TT_PROG_ARP,
    ARAD_PP_DBAL_TT_PROG_BRIDGE_STAR,
    ARAD_PP_DBAL_TT_PROG_TT_UNKNOWN_L3,
    ARAD_PP_DBAL_TT_PROG_TRILL_ONE_TAG,
    ARAD_PP_DBAL_TT_PROG_TRILL_TWO_TAGS,
    ARAD_PP_DBAL_TT_PROG_TRILL_TRAP,
    ARAD_PP_DBAL_TT_PROG_FC_WITH_VFT,
    ARAD_PP_DBAL_TT_PROG_FC,
    ARAD_PP_DBAL_TT_PROG_MIM_MC,
    ARAD_PP_DBAL_TT_PROG_MIM_WITH_BTAG,
    ARAD_PP_DBAL_TT_PROG_IPV4_ROUTER,
    ARAD_PP_DBAL_TT_PROG_IPV4_ROUTER_PWE_GRE_DIP_FOUND,
    ARAD_PP_DBAL_TT_PROG_IPV4_ROUTER_DIP_FOUND,
    ARAD_PP_DBAL_TT_PROG_IPV4_ROUTER_L2_GRE_PORT,
    ARAD_PP_DBAL_TT_PROG_IPV4_ROUTER_VXLAN_PORT,
    ARAD_PP_DBAL_TT_PROG_IPV4_ROUTER_UC_DIP_NOT_FOUND,
    ARAD_PP_DBAL_TT_PROG_IPV4_ROUTER_MC_DIP_NOT_FOUND,
    ARAD_PP_DBAL_TT_PROG_IPV4_ROUTER_COMPATIBLE_MC,
    ARAD_PP_DBAL_TT_PROG_IPV6_ROUTER,
    ARAD_PP_DBAL_TT_PROG_DIPV6_COMPRESSED,
    ARAD_PP_DBAL_TT_PROG_EXTENDER_UNTAG_CHECK_IP_MC,
    ARAD_PP_DBAL_TT_PROG_EXTENDER_UNTAG_CHECK_IP_UC,
    ARAD_PP_DBAL_TT_PROG_EXTENDER_UNTAG_CHECK,
    ARAD_PP_DBAL_TT_PROG_IPV4_ROUTER_DIP_SIP_VRF,
    ARAD_PP_DBAL_TT_PROG_IPV4_ROUTER_L3_GRE_PORT,
    ARAD_PP_DBAL_TT_PROG_IPV4_FOR_EXPLICIT_NULL,
    ARAD_PP_DBAL_TT_PROG_TUNNEL_TWO_EXPLICIT_NULL,
    ARAD_PP_DBAL_TT_PROG_EXTENDER_CB_IP_MC,
    ARAD_PP_DBAL_TT_PROG_EXTENDER_CB_IP_UC,
    ARAD_PP_DBAL_TT_PROG_EXTENDER_CB,
    ARAD_PP_DBAL_TT_PROG_INDEXED_L1_GAL_ONLY,
    ARAD_PP_DBAL_TT_PROG_INDEXED_L2_GAL_ONLY,
    ARAD_PP_DBAL_TT_PROG_VLAN_DOMAIN_MPLS_L2,
    ARAD_PP_DBAL_TT_PROG_INDEX_AS_ORDER_L2_L3,
    ARAD_PP_DBAL_TT_PROG_INDEX_AS_ORDER_L2_L2,
    ARAD_PP_DBAL_TT_PROG_INDEX_AS_ORDER_L1_L2,
    ARAD_PP_DBAL_TT_PROG_INDEX_AS_ORDER_L1_L1,
    ARAD_PP_DBAL_TT_PROG_INDEX_AS_ORDER_NOP,
    ARAD_PP_DBAL_TT_PROG_INDEX_MPLS4_L3_L4,
    ARAD_PP_DBAL_TT_PROG_INDEX_MPLS4_NOP,
    ARAD_PP_DBAL_TT_PROG_IP_DISABLE_ON_LIF,
    ARAD_PP_DBAL_TT_PROG_IPV4_DISABLE_ON_LIF,
    ARAD_PP_DBAL_TT_PROG_IPV6_DISABLE_ON_LIF,
    ARAD_PP_DBAL_TT_PROG_IPV6_ROUTER_COMP_DEST_FOUND,
    ARAD_PP_DBAL_TT_PROG_IPV6_ROUTER_AFTER_RECYCLE,
    ARAD_PP_DBAL_TT_PROG_PPPOE,
    ARAD_PP_DBAL_TT_PROG_L2TP,
    ARAD_PP_DBAL_TT_PROG_GENERALIZED_RCH_MPLS,
    ARAD_PP_DBAL_TT_PROG_MYMAC_IP_DISABLED_TRAP,
    ARAD_PP_DBAL_TT_PROG_3MTSE_PLUS_FRR_L1_L1,
    ARAD_PP_DBAL_TT_PROG_3MTSE_PLUS_FRR_TFL_L3_L3,
    ARAD_PP_DBAL_TT_PROG_3MTSE_PLUS_FRR_L3_L1,
    ARAD_PP_DBAL_TT_PROG_3MTSE_PLUS_FRR_TFL_L4_L3,
    ARAD_PP_DBAL_TT_PROG_3MTSE_PLUS_FRR_L1,
    ARAD_PP_DBAL_TT_PROG_3MTSE_PLUS_FRR_L2_L1,
    ARAD_PP_DBAL_TT_PROG_2MTSE_PLUS_FRR_L1_L1,
    ARAD_PP_DBAL_TT_PROG_2MTSE_PLUS_FRR_L2_L2,
    ARAD_PP_DBAL_TT_PROG_2MTSE_PLUS_FRR_L2_L1,
    ARAD_PP_DBAL_TT_PROG_2MTSE_PLUS_FRR_L3_L2,
    ARAD_PP_DBAL_TT_PROG_IPV4_ROUTER_DIP_FOUND_IPMC_RCY,
    ARAD_PP_DBAL_TT_PROG_MC_PPPOE,

    ARAD_PP_DBAL_TT_NOF_PROGRAMS

} ARAD_PP_DBAL_VTT_PROGRAMS;


typedef enum
{

    ARAD_PP_DBAL_VT_PROG_SELECTION_INVALID = -1,
    ARAD_PP_DBAL_VT_PROG_SELECTION_TM,
    ARAD_PP_DBAL_VT_PROG_SELECTION_TM_STAT,
    ARAD_PP_DBAL_VT_PROG_SELECTION_VD_INITIALVID,
    ARAD_PP_DBAL_VT_PROG_SELECTION_VD_INITIALVID_L1,
    ARAD_PP_DBAL_VT_PROG_SELECTION_VD_OUTERVID,
    ARAD_PP_DBAL_VT_PROG_SELECTION_VD_OUTERVID_L1,
    ARAD_PP_DBAL_VT_PROG_SELECTION_VD_OUTER_INNER_VID,
    ARAD_PP_DBAL_VT_PROG_SELECTION_VD_OUTER_INNER_VID_OR_OUTER_VID,
    ARAD_PP_DBAL_VT_PROG_SELECTION_VD_INITIALVID_L1FRR,
    ARAD_PP_DBAL_VT_PROG_SELECTION_VD_OUTERVID_L1FRR,
    ARAD_PP_DBAL_VT_PROG_SELECTION_VD_INITIALVID_L1L2,
    ARAD_PP_DBAL_VT_PROG_SELECTION_VD_OUTERVID_L1L2,
    ARAD_PP_DBAL_VT_PROG_SELECTION_VD_OUTER_OR_VD_OUTER_PCP,
    ARAD_PP_DBAL_VT_PROG_SELECTION_VD_OUTER_INNER_OR_VD_OUTER_INNER_OUTERPCP,
    ARAD_PP_DBAL_VT_PROG_SELECTION_HIGH_VD_OUTER_INNER_OUTERPCP_OR_VD_OUTER_INNER_OR_VD_OUTER,
    ARAD_PP_DBAL_VT_PROG_SELECTION_VD_INITIALVID_OR_VD_INITIALVID,
    ARAD_PP_DBAL_VT_PROG_SELECTION_VD_OUTER_OR_VD_OUTER,
    ARAD_PP_DBAL_VT_PROG_SELECTION_VD_OUTER_INNER_OR_VD_OUTER_INNER,
    ARAD_PP_DBAL_VT_PROG_SELECTION_INDX_VD_INITIALVID_FRR_L1,
    ARAD_PP_DBAL_VT_PROG_SELECTION_INDX_VD_OUTERVID_FRR_L1,
    ARAD_PP_DBAL_VT_PROG_SELECTION_VD_INITIALVID_TRILL,
    ARAD_PP_DBAL_VT_PROG_SELECTION_VD_OUTERVID_TRILL,
    ARAD_PP_DBAL_VT_PROG_SELECTION_VD_OUTER_INNER_VID_TRILL,
    ARAD_PP_DBAL_VT_PROG_SELECTION_VD_DESIGNATED_VID_TRILL,
    ARAD_PP_DBAL_VT_PROG_SELECTION_VD_INITIALVID_L1_L2ELI,
    ARAD_PP_DBAL_VT_PROG_SELECTION_VD_OUTERVID_L1_L2ELI,
    ARAD_PP_DBAL_VT_PROG_SELECTION_VD_INDEXED_VDxINITIALVID_L1,
    ARAD_PP_DBAL_VT_PROG_SELECTION_VD_INDEXED_VDxOUTERVID_L1,
    ARAD_PP_DBAL_VT_PROG_SELECTION_VD_MPLS_PORT_L1,
    ARAD_PP_DBAL_VT_PROG_SELECTION_VD_INDEXED_VDxINITIALVID_L3,
    ARAD_PP_DBAL_VT_PROG_SELECTION_VD_INDEXED_VDxOUTERVID_L3,
    ARAD_PP_DBAL_VT_PROG_SELECTION_IPV4_VDxINITIALVID,
    ARAD_PP_DBAL_VT_PROG_SELECTION_IPV4_VDxOUTERVID,
    ARAD_PP_DBAL_VT_PROG_SELECTION_IPV4_PORT_TERMINATION,
    ARAD_PP_DBAL_VT_PROG_SELECTION_EVB_UN_C_TAG,
    ARAD_PP_DBAL_VT_PROG_SELECTION_EVB_S_TAG,
    ARAD_PP_DBAL_VT_PROG_SELECTION_DOUBLE_TAG_PRIORITY_INITIAL_VID,
    ARAD_PP_DBAL_VT_PROG_SELECTION_DOUBLE_TAG_INITIAL_VID,
    ARAD_PP_DBAL_VT_PROG_SELECTION_IPV4_VDxINITIALVID_AFTER_RCY,
    ARAD_PP_DBAL_VT_PROG_SELECTION_IPV4_VDxOUTERVID_AFTER_RCY,
    ARAD_PP_DBAL_VT_PROG_SELECTION_SINGLE_TAG_5_TUPLE_QINQ,
    ARAD_PP_DBAL_VT_PROG_SELECTION_DOUBLE_TAG_5_TUPLE_QINQ,
    ARAD_PP_DBAL_VT_PROG_SELECTION_UNTAGGED_TST1,
    ARAD_PP_DBAL_VT_PROG_SELECTION_ONE_TAG_TST1,
    ARAD_PP_DBAL_VT_PROG_SELECTION_DOUBLE_TAG_TST1,
    ARAD_PP_DBAL_VT_PROG_SELECTION_UNTAGGED_VRRP,
    ARAD_PP_DBAL_VT_PROG_SELECTION_ONE_TAG_VRRP,
    ARAD_PP_DBAL_VT_PROG_SELECTION_DOUBLE_TAG_VRRP,
    ARAD_PP_DBAL_VT_PROG_SELECTION_UNTAGGED_EXP_NULL_TST1,
    ARAD_PP_DBAL_VT_PROG_SELECTION_ONE_TAG_EXP_NULL_TST1,
    ARAD_PP_DBAL_VT_PROG_SELECTION_DOUBLE_TAG_EXP_NULL_TST1,
    ARAD_PP_DBAL_VT_PROG_SELECTION_OUTER_INNER_PCP_1_TST2,
    ARAD_PP_DBAL_VT_PROG_SELECTION_OUTER_INNER_PCP_2_TST2,
    ARAD_PP_DBAL_VT_PROG_SELECTION_OUTER_PCP_1_TST2,
    ARAD_PP_DBAL_VT_PROG_SELECTION_OUTER_PCP_2_TST2,
    ARAD_PP_DBAL_VT_PROG_SELECTION_OUTER_INNER_1_TST2,
    ARAD_PP_DBAL_VT_PROG_SELECTION_OUTER_INNER_2_TST2,
    ARAD_PP_DBAL_VT_PROG_SELECTION_OUTER_1_TST2,
    ARAD_PP_DBAL_VT_PROG_SELECTION_OUTER_2_TST2,
    ARAD_PP_DBAL_VT_PROG_SELECTION_UNTAGGED_TST2,
    ARAD_PP_DBAL_VT_PROG_SELECTION_TEST2,
    ARAD_PP_DBAL_VT_PROG_SELECTION_EXTENDER_PE,
    ARAD_PP_DBAL_VT_PROG_SELECTION_EXTENDER_PE_UT,
    ARAD_PP_DBAL_VT_PROG_SELECTION_CUSTOM_PP_PORT_TUNNEL,
    ARAD_PP_DBAL_VT_PROG_SELECTION_VD_INITIALVID_VLAN_DOMAIN_MPLS_L1,
    ARAD_PP_DBAL_VT_PROG_SELECTION_PON_UNTAGGED,
    ARAD_PP_DBAL_VT_PROG_SELECTION_PON_PRIORITY_TAG,
    ARAD_PP_DBAL_VT_PROG_SELECTION_PON_ONE_TAG_1,
    ARAD_PP_DBAL_VT_PROG_SELECTION_PON_ONE_TAG_2,
    ARAD_PP_DBAL_VT_PROG_SELECTION_PON_TWO_TAGS,
    ARAD_PP_DBAL_VT_PROG_SELECTION_PON_TWO_TAGS_VS_TUNNEL_ID,
    ARAD_PP_DBAL_VT_PROG_SELECTION_PON_IGNORE_2ND_TAG,
    ARAD_PP_DBAL_VT_PROG_SELECTION_SECTION_OAM_ONE_TAG_1,
    ARAD_PP_DBAL_VT_PROG_SELECTION_SECTION_OAM_ONE_TAG_2,
    ARAD_PP_DBAL_VT_PROG_SELECTION_SECTION_OAM_DOUBLE_TAGS,
    ARAD_PP_DBAL_VT_PROG_SELECTION_VD_INDEXED_VDxINITIALVID_MPLS4_L1L2,
    ARAD_PP_DBAL_VT_PROG_SELECTION_VD_INDEXED_VDxOUTERVID_MPLS4_L1L2,
    ARAD_PP_DBAL_VT_PROG_SELECTION_IPV6_VDxINITIALVID,
    ARAD_PP_DBAL_VT_PROG_SELECTION_IPV6_VDxOUTERVID,
    ARAD_PP_DBAL_VT_PROG_SELECTION_IPV6_VDxINITIALVID_AFTER_RECYCLE,
    ARAD_PP_DBAL_VT_PROG_SELECTION_IPV6_VDxOUTERVID_AFTER_RECYCLE,
    ARAD_PP_DBAL_VT_PROG_SELECTION_GENERALIZED_RCH0,
    ARAD_PP_DBAL_VT_PROG_SELECTION_GENERALIZED_RCH1,
    ARAD_PP_DBAL_VT_PROG_SELECTION_3MTSE_PLUS_FRR_UNTAGGED_M3_PLUS_ELI,
	ARAD_PP_DBAL_VT_PROG_SELECTION_3MTSE_PLUS_FRR_TAG_1_M3_PLUS_ELI,
	ARAD_PP_DBAL_VT_PROG_SELECTION_3MTSE_PLUS_FRR_UNTAGGED_M2_PLUS,
	ARAD_PP_DBAL_VT_PROG_SELECTION_3MTSE_PLUS_FRR_TAG_1_M2_PLUS,
	ARAD_PP_DBAL_VT_PROG_SELECTION_3MTSE_PLUS_FRR_UNTAGGED,
	ARAD_PP_DBAL_VT_PROG_SELECTION_3MTSE_PLUS_FRR_TAG_1,
	ARAD_PP_DBAL_VT_PROG_SELECTION_2MTSE_PLUS_FRR_DOUBLE_TAG_L1,
	ARAD_PP_DBAL_VT_PROG_SELECTION_2MTSE_PLUS_FRR_DOUBLE_TAG_PCP_L1,
    ARAD_PP_DBAL_VT_NOF_PROGRAM_SELECTION,


    ARAD_PP_DBAL_TT_PROG_SELECTION_INVALID = -1,
    ARAD_PP_DBAL_TT_PROG_SELECTION_TM,
    ARAD_PP_DBAL_TT_PROG_SELECTION_OAM_STAT,
    ARAD_PP_DBAL_TT_PROG_SELECTION_BFD_STAT,
    ARAD_PP_DBAL_TT_PROG_SELECTION_MLDP_OVERLAY_AFTER_RCY,
    ARAD_PP_DBAL_TT_PROG_SELECTION_UNINDEXED_MPLS_L2,
    ARAD_PP_DBAL_TT_PROG_SELECTION_INDEXED_MPLS_L3_L2,
    ARAD_PP_DBAL_TT_PROG_SELECTION_INDEXED_MPLS_PORT_L3_L2,
    ARAD_PP_DBAL_TT_PROG_SELECTION_UNINDEXED_MPLS_L3,
    ARAD_PP_DBAL_TT_PROG_SELECTION_UNINDEXED_MPLS_L2_L3_ELI,
    ARAD_PP_DBAL_TT_PROG_SELECTION_INDEXED_L1_L1,
    ARAD_PP_DBAL_TT_PROG_SELECTION_INDEXED_L1_L1_GAL,
    ARAD_PP_DBAL_TT_PROG_SELECTION_INDEXED_L1_L2,
    ARAD_PP_DBAL_TT_PROG_SELECTION_INDEXED_L1_L2_GAL,
    ARAD_PP_DBAL_TT_PROG_SELECTION_INDEXED_L2_L2,
    ARAD_PP_DBAL_TT_PROG_SELECTION_INDEXED_L2_L3_GAL,
    ARAD_PP_DBAL_TT_PROG_SELECTION_INDEXED_L2_L3_ELI,
    ARAD_PP_DBAL_TT_PROG_SELECTION_INDEXED_L1_L2_INRIF,
    ARAD_PP_DBAL_TT_PROG_SELECTION_INDEXED_L1_L2ELI_INRIF,
    ARAD_PP_DBAL_TT_PROG_SELECTION_INDEXED_L1_L2_L3ELI_INRIF,
    ARAD_PP_DBAL_TT_PROG_SELECTION_INDEXED_MPLS_L3_L4,
    ARAD_PP_DBAL_TT_PROG_SELECTION_INDEX_AS_ORDER_L2_L3,
    ARAD_PP_DBAL_TT_PROG_SELECTION_INDEX_AS_ORDER_L2_L2,
    ARAD_PP_DBAL_TT_PROG_SELECTION_INDEX_AS_ORDER_L1_L2,
    ARAD_PP_DBAL_TT_PROG_SELECTION_INDEX_AS_ORDER_L1_L1,
    ARAD_PP_DBAL_TT_PROG_SELECTION_INDEX_AS_ORDER_NOP,
    ARAD_PP_DBAL_TT_PROG_SELECTION_ARP,
    ARAD_PP_DBAL_TT_PROG_SELECTION_BRIDGE_STAR,
    ARAD_PP_DBAL_TT_PROG_SELECTION_BRIDGE_STAR_ROUTE_DISABLE,
    ARAD_PP_DBAL_TT_PROG_SELECTION_BRIDGE_STAR_ROUTE_DISABLE_IP,
    ARAD_PP_DBAL_TT_PROG_SELECTION_BRIDGE_OAM,
    ARAD_PP_DBAL_TT_PROG_SELECTION_UNKNOWN_L3,
    ARAD_PP_DBAL_TT_PROG_SELECTION_TRILL_ONE_TAG,
    ARAD_PP_DBAL_TT_PROG_SELECTION_TRILL_TWO_TAGS,
    ARAD_PP_DBAL_TT_PROG_SELECTION_TRILL_TRAP,
    ARAD_PP_DBAL_TT_PROG_SELECTION_FC_WITH_VFT,
    ARAD_PP_DBAL_TT_PROG_SELECTION_FC,
    ARAD_PP_DBAL_TT_PROG_SELECTION_MIM_MC,
    ARAD_PP_DBAL_TT_PROG_SELECTION_MIM_WITH_BTAG,
    ARAD_PP_DBAL_TT_PROG_SELECTION_IPV4_ROUTER,    
    ARAD_PP_DBAL_TT_PROG_SELECTION_IPV4_ROUTER_PWE_GRE_IPMC_RCY,
    ARAD_PP_DBAL_TT_PROG_SELECTION_IPV4_ROUTER_PWE_GRE_DIP_FOUND,
    ARAD_PP_DBAL_TT_PROG_SELECTION_IPV4_ROUTER_DIP_FOUND_IPMC_RCY,
    ARAD_PP_DBAL_TT_PROG_SELECTION_IPV4_ROUTER_DIP_FOUND,
    ARAD_PP_DBAL_TT_PROG_SELECTION_IPV4_ROUTER_L2_GRE_PORT,
    ARAD_PP_DBAL_TT_PROG_SELECTION_IPV4_ROUTER_VXLAN_PORT,
    ARAD_PP_DBAL_TT_PROG_SELECTION_IPV4_ROUTER_UC_DIP_NOT_FOUND,
    ARAD_PP_DBAL_TT_PROG_SELECTION_IPV4_ROUTER_MC_DIP_NOT_FOUND,
    ARAD_PP_DBAL_TT_PROG_SELECTION_IPV4_ROUTER_COMPATIBLE_MC,
    ARAD_PP_DBAL_TT_PROG_SELECTION_IPV6_ROUTER,
    ARAD_PP_DBAL_TT_PROG_SELECTION_DIPV6_COMPRESSED,
    ARAD_PP_DBAL_TT_PROG_SELECTION_EXTENDER_UNTAG_CHECK_IP_MC,
    ARAD_PP_DBAL_TT_PROG_SELECTION_EXTENDER_UNTAG_CHECK_IP_UC,
    ARAD_PP_DBAL_TT_PROG_SELECTION_EXTENDER_UNTAG_CHECK,
    ARAD_PP_DBAL_TT_PROG_SELECTION_PON_IP_BRIDGE_COMPATIBLE_MC,
    ARAD_PP_DBAL_TT_PROG_SELECTION_PON_IPV4_BRIDGE_UC,
    ARAD_PP_DBAL_TT_PROG_SELECTION_PON_IPV6_BRIDGE_UC,
    ARAD_PP_DBAL_TT_PROG_SELECTION_PON_IPV4_BRIDGE_DIP_FOUND,
    ARAD_PP_DBAL_TT_PROG_SELECTION_IPV4_ROUTER_UC_DIP_SIP_VRF,
    ARAD_PP_DBAL_TT_PROG_SELECTION_IPV4_ROUTER_L3_GRE_PORT,
    ARAD_PP_DBAL_TT_PROG_SELECTION_IPV4_FOR_EXPLICIT_NULL,
    ARAD_PP_DBAL_TT_PROG_SELECTION_TUNNEL_FOR_TWO_EXPLICIT_NULL,
    ARAD_PP_DBAL_TT_PROG_SELECTION_EXTENDER_CB_IP_MC,
    ARAD_PP_DBAL_TT_PROG_SELECTION_EXTENDER_CB_IP_UC,
    ARAD_PP_DBAL_TT_PROG_SELECTION_EXTENDER_CB,
    ARAD_PP_DBAL_TT_PROG_SELECTION_IPV4_ROUTER_TUNNELS,
    ARAD_PP_DBAL_TT_PROG_SELECTION_INDEXED_L1_GAL_ONLY,
    ARAD_PP_DBAL_TT_PROG_SELECTION_INDEXED_L2_GAL_ONLY,
    ARAD_PP_DBAL_TT_PROG_SELECTION_VLAN_DOMAIN_MPLS_L2,
    ARAD_PP_DBAL_TT_PROG_SELECTION_INDEXED_MPLS4_L3_L4,
    ARAD_PP_DBAL_TT_PROG_SELECTION_INDEXED_MPLS4_NOP,
    ARAD_PP_DBAL_TT_PROG_SELECTION_IP_DISABLE_ON_LIF,
    ARAD_PP_DBAL_TT_PROG_SELECTION_IPV4_DISABLE_ON_LIF,
    ARAD_PP_DBAL_TT_PROG_SELECTION_IPV6_DISABLE_ON_LIF,
    ARAD_PP_DBAL_TT_PROG_SELECTION_IPV6_ROUTER_COMP_DEST_FOUND,
    ARAD_PP_DBAL_TT_PROG_SELECTION_IPV6_ROUTER_AFTER_RECYCLE,
    ARAD_PP_DBAL_TT_PROG_SELECTION_IPV6_ROUTER_COMP_DEST_FOUND_AFTER_RECYCLE,
    ARAD_PP_DBAL_TT_PROG_SELECTION_PPPOE,
	ARAD_PP_DBAL_TT_PROG_SELECTION_L2TP,
    ARAD_PP_DBAL_TT_PROG_SELECTION_GENERALIZED_RCH_IP,
    ARAD_PP_DBAL_TT_PROG_SELECTION_GENERALIZED_RCH_MPLS,
    ARAD_PP_DBAL_TT_PROG_SELECTION_3MTSE_PLUS_FRR_M1,
    ARAD_PP_DBAL_TT_PROG_SELECTION_3MTSE_PLUS_FRR_M2_HIT_SEM,
    ARAD_PP_DBAL_TT_PROG_SELECTION_3MTSE_PLUS_FRR_M2_MISS_SEM,
    ARAD_PP_DBAL_TT_PROG_SELECTION_3MTSE_PLUS_FRR_M2,
    ARAD_PP_DBAL_TT_PROG_SELECTION_3MTSE_PLUS_FRR_M3_PLUS_ELI_IML_SEM_HIT,
    ARAD_PP_DBAL_TT_PROG_SELECTION_3MTSE_PLUS_FRR_M3_PLUS_ELI_IML_SEM_MISS,
    ARAD_PP_DBAL_TT_PROG_SELECTION_3MTSE_PLUS_FRR_M3_PLUS_ELI,
    ARAD_PP_DBAL_TT_PROG_SELECTION_3MTSE_PLUS_FRR_M3_ELI,
    ARAD_PP_DBAL_TT_PROG_SELECTION_3MTSE_PLUS_FRR_M3_HIT_TCAM,
    ARAD_PP_DBAL_TT_PROG_SELECTION_3MTSE_PLUS_FRR_M3_MISS_TCAM,
    ARAD_PP_DBAL_TT_PROG_SELECTION_3MTSE_PLUS_FRR_M3_HIT_SEM,
    ARAD_PP_DBAL_TT_PROG_SELECTION_3MTSE_PLUS_FRR_M3_MISS_SEM,
    ARAD_PP_DBAL_TT_PROG_SELECTION_3MTSE_PLUS_FRR_M3_PLUS_HIT_SEM_AND_TCAM,
    ARAD_PP_DBAL_TT_PROG_SELECTION_3MTSE_PLUS_FRR_M3_PLUS_HIT_TCAM,
    ARAD_PP_DBAL_TT_PROG_SELECTION_3MTSE_PLUS_FRR_M3_PLUS_MISS_TCAM,
    ARAD_PP_DBAL_TT_PROG_SELECTION_3MTSE_PLUS_FRR_M3_PLUS_MISS_SEM_AND_TCAM,
    ARAD_PP_DBAL_TT_PROG_SELECTION_3MTSE_PLUS_FRR_M3_PLUS_ELI_GAL,
    ARAD_PP_DBAL_TT_PROG_SELECTION_3MTSE_PLUS_FRR_M3_PLUS_ELI_HIT_SEM,
    ARAD_PP_DBAL_TT_PROG_SELECTION_3MTSE_PLUS_FRR_M3_PLUS_ELI_MISS_SEM,
    ARAD_PP_DBAL_TT_PROG_SELECTION_3MTSE_PLUS_FRR_M3_PLUS_ELI_GAL_HIT_TCAM,
    ARAD_PP_DBAL_TT_PROG_SELECTION_3MTSE_PLUS_FRR_M3_PLUS_ELI_GAL_MISS_TCAM,
    ARAD_PP_DBAL_TT_PROG_SELECTION_3MTSE_PLUS_FRR_M3_PLUS_NO_RANGE_HIT_TCAM,
    ARAD_PP_DBAL_TT_PROG_SELECTION_3MTSE_PLUS_FRR_M3_PLUS_NO_RANGE_MISS_TCAM,
    ARAD_PP_DBAL_TT_PROG_SELECTION_3MTSE_PLUS_FRR_M3_PLUS_ELI_HIT_TCAM,
    ARAD_PP_DBAL_TT_PROG_SELECTION_3MTSE_PLUS_FRR_M3_PLUS_ELI_MISS_TCAM,
    ARAD_PP_DBAL_TT_PROG_SELECTION_2MTSE_PLUS_FRR_M1,
    ARAD_PP_DBAL_TT_PROG_SELECTION_2MTSE_PLUS_FRR_M3_PLUS_R1_ELI,
    ARAD_PP_DBAL_TT_PROG_SELECTION_2MTSE_PLUS_FRR_M2_HIT_TCAM,
    ARAD_PP_DBAL_TT_PROG_SELECTION_2MTSE_PLUS_FRR_M2_MISS_TCAM,
    ARAD_PP_DBAL_TT_PROG_SELECTION_2MTSE_PLUS_FRR_M3_PLUS_R3_GAL_HIT_TCAM,
    ARAD_PP_DBAL_TT_PROG_SELECTION_2MTSE_PLUS_FRR_M3_PLUS_R3_GAL_MISS_TCAM,
    ARAD_PP_DBAL_TT_PROG_SELECTION_2MTSE_PLUS_FRR_M3_R2_IML,
    ARAD_PP_DBAL_TT_PROG_SELECTION_2MTSE_PLUS_FRR_M2,
    ARAD_PP_DBAL_TT_PROG_SELECTION_2MTSE_PLUS_FRR_M3_ELI,
    ARAD_PP_DBAL_TT_PROG_SELECTION_2MTSE_PLUS_FRR_M3_PLUS_ELI_GAL,
    ARAD_PP_DBAL_TT_PROG_SELECTION_2MTSE_PLUS_FRR_M3_PLUS_ELI_HIT_TCAM,
    ARAD_PP_DBAL_TT_PROG_SELECTION_2MTSE_PLUS_FRR_M3_PLUS_ELI_MISS_TCAM,
    ARAD_PP_DBAL_TT_PROG_SELECTION_2MTSE_PLUS_FRR_M3_PLUS_R2_ELI,
    ARAD_PP_DBAL_TT_PROG_SELECTION_2MTSE_PLUS_FRR_M3_PLUS_NO_RANGE_HIT_TCAM,
    ARAD_PP_DBAL_TT_PROG_SELECTION_2MTSE_PLUS_FRR_M3_PLUS_NO_RANGE_MISS_TCAM,
    ARAD_PP_DBAL_TT_PROG_SELECTION_MC_PPPOE,

    ARAD_PP_DBAL_TT_NOF_PROGRAM_SELECTION

} ARAD_PP_DBAL_VTT_PROGRAM_SELECTION;



typedef enum
{
    MPLS_LABEL_0,
    MPLS_LABEL_1,
    MPLS_LABEL_2,
    MPLS_LABEL_3,
    MPLS_LABEL_4,
    MPLS_DUMMY
}ARAD_PP_DBAL_VTT_TABLE_MPLS_LABELS;

typedef enum
{
    IS_OAM_STAT,
    IS_BFD_STAT,
    OAM_FWD_CODE_0,
    OAM_FWD_CODE_1,
    BFD_FWD_CODE_0,
    BFD_FWD_CODE_1,
    TM_STAT_OFF,
    TM_STAT_ON
}ARAD_PP_DBAL_VTT_TABLE_STAT_INFO;

typedef enum
{
    VSAN_INIT,
    VSAN_FROM_VFT,
    VSAN_FROM_VSI,
    NO_VFT,
    WITH_VFT
}ARAD_PP_DBAL_VTT_TABLE_FC_INFO;

typedef enum
{
    MPLS_LABEL_0_IDX0,
    MPLS_LABEL_0_IDX1,
    MPLS_LABEL_0_IDX2,
    MPLS_LABEL_0_IDX13,
    MPLS_LABEL_0_IDX12,
    MPLS_LABEL_0_ELI_IDX0,   
    MPLS_LABEL_0_ELI_IDX1,
    MPLS_LABEL_0_ELI_IDX2,
    MPLS_LABEL_0_ELI_IDX13,
    MPLS_LABEL_0_ELI_IDX12,
    MPLS_LABEL_1_IDX0 = MPLS_LABEL_0_ELI_IDX0,
    MPLS_LABEL_1_IDX1,
    MPLS_LABEL_1_IDX2,
    MPLS_LABEL_1_IDX13,
    MPLS_LABEL_1_IDX12,
    MPLS_LABEL_2_IDX0,
    MPLS_LABEL_2_IDX1,
    MPLS_LABEL_2_IDX2,
    MPLS_LABEL_2_IDX13,
    MPLS_LABEL_2_IDX12,
    MPLS_LABEL_3_IDX0,
    MPLS_LABEL_3_IDX1,
    MPLS_LABEL_3_IDX2,
    MPLS_LABEL_3_IDX13,
    MPLS_LABEL_3_IDX12,
    MPLS_LABEL_BOS_START,
    MPLS_LABEL_0_BOS_IDX0 = MPLS_LABEL_BOS_START,
    MPLS_LABEL_0_BOS_IDX1,
    MPLS_LABEL_0_BOS_IDX2,
    MPLS_LABEL_0_BOS_IDX13,
    MPLS_LABEL_0_BOS_IDX12,
    MPLS_LABEL_1_BOS_IDX0,
    MPLS_LABEL_1_BOS_IDX1,
    MPLS_LABEL_1_BOS_IDX2,
    MPLS_LABEL_1_BOS_IDX13,
    MPLS_LABEL_1_BOS_IDX12,
    MPLS_LABEL_2_BOS_IDX0,
    MPLS_LABEL_2_BOS_IDX1,
    MPLS_LABEL_2_BOS_IDX2,
    MPLS_LABEL_2_BOS_IDX13,
    MPLS_LABEL_2_BOS_IDX12,
    MPLS_LABEL_3_BOS_IDX0,
    MPLS_LABEL_3_BOS_IDX1,
    MPLS_LABEL_3_BOS_IDX2,
    MPLS_LABEL_3_BOS_IDX13,
    MPLS_LABEL_3_BOS_IDX12
}ARAD_PP_DBAL_VTT_TABLE_INDEXED_MPLS_LABELS;

typedef enum
{
    NOT_DUMMY_TYPE,
    DUMMY_TYPE,
    DUMMY_FOR_EXPLICIT_NULL_TYPE
}ARAD_PP_DBAL_VTT_TABLE_TYPE;
typedef enum
{
    INITIAL_VID       = 0,
    OUTER_VID         = 1,
    OUTER_INNER_VID   = 2,
    OUTER_INITIAL_VID = 5,
    NOF_VID_TYPES = 4,  
    OUTER_INNER_VID_PCP = 6,
    INNER_VID = 7,

    TRILL_ONE_TAG        = 1,
    TRILL_TWO_TAG        = 2,
    TRILL_DESIGNATED_VID = 3,

    TST1_VID_UNTAGGED       = 0,
    TST1_VID_ONE_TAG        = 1,
    TST1_VID_INITIAL_TAG    = 1,
    TST1_VID_CMPRSD_ONE_TAG = 2,
    TST1_VID_DOUBLE_TAG     = 3

}ARAD_PP_DBAL_VTT_TABLE_VID;

typedef enum
{
    PON_TUNNEL_SEMA                        = 0,
    PON_TUNNEL_SEMB                        = 1,
    PON_TUNNEL_ETHTYPE                     = 2, 
    PON_TUNNEL_OUTER_VID_SEMA              = 3,
    PON_TUNNEL_OUTER_VID_SEMB              = 4,
    PON_TUNNEL_ETHTYPE_PCP_OUTER_VID       = 5, 
    PON_TUNNEL_OUTER_INNER_VID_SEMA        = 6,
    PON_TUNNEL_OUTER_INNER_VID_SEMB        = 7,
    PON_TUNNEL_ETHTYPE_PCP_OUTER_INNER_VID = 8, 
    PON_NOF_TUNNEL_TYPE                    = 7
}ARAD_PP_DBAL_VTT_TABLE_PON_TUNNEL;

typedef enum
{
    SECTION_OAM_PORT_OUTER_VID_ETH    = 0,
    SECTION_OAM_PORT_OUTER_INNER_VID_ETH     = 1
}ARAD_PP_DBAL_VTT_TABLE_SECTION_OAM;

typedef enum
{
    VXLAN,
    L2_GRE
}ARAD_PP_DBAL_VTT_TUNNEL_TYPE;

typedef enum
{
    PROG_SEL_PON_UNTAGGED                        = 0,
    PROG_SEL_PON_PRIORITY_TAG                    = 1,
    PROG_SEL_PON_ONE_TAG_1                       = 2,
    PROG_SEL_PON_ONE_TAG_2                       = 3,
    PROG_SEL_PON_TWO_TAGS                        = 4,
    PROG_SEL_PON_TWO_TAGS_VS_TUNNEL_ID           = 5,
    PROG_SEL_PON_IGNORE_2ND_TAG                  = 6,
    PROG_SEL_PON_NOF_TYPES                       = 7
}ARAD_PP_DBAL_VTT_PROG_SEL_PON_TYPE;

typedef enum
{
    PROG_SEL_SECTION_OAM_ONE_TAG_1    = 0,
    PROG_SEL_SECTION_OAM_ONE_TAG_2     = 1,
    PROG_SEL_SECTION_OAM_TWO_TAGS	 = 2
}ARAD_PP_DBAL_VTT_PROG_SEL_SECTION_OAM_TYPE;


typedef enum
{
    BRIDGE_WITHOUT_MYMAC    = 0,
    BRIDGE_ROUTE_DISABLE    = 1,
    BRIDGE_OAM              = 2,
    BRIDGE_ROUTE_DISABLE_IP = 3
}ARAD_PP_DBAL_VTT_TABLE_BRIDGE;

typedef enum
{
    MAC_IN_MAC_MC,
    MAC_IN_MAC_WITH_BTAG
}ARAD_PP_DBAL_VTT_TABLE_MIM_TYPE;
typedef enum
{
    IP_NOT_COMPRESSED,
    IP_COMPRESSED
}ARAD_PP_DBAL_VTT_IP_TYPE;

typedef enum
{
    TPID1,
    TPID2
}ARAD_PP_DBAL_VTT_TPID;

typedef enum
{
    NO_PCP_PORT,
    PCP_PORT
}ARAD_PP_DBAL_VTT_PCP_PORT;

typedef enum
{
    EXTENDER_ETH,       
    EXTENDER_IP_UC,
    EXTENDER_IP_MC
}ARAD_PP_DBAL_VTT_EXTENDER_TYPE;

typedef enum
{
    PROG_SEL_3MTSE_PLUS_FRR_UNTAGGED_M3_PLUS_ELI      = 0,
    PROG_SEL_3MTSE_PLUS_FRR_TAG_1_M3_PLUS_ELI         = 1,
    PROG_SEL_3MTSE_PLUS_FRR_UNTAGGED_M2_PLUS          = 2,
    PROG_SEL_3MTSE_PLUS_FRR_TAG_1_M2_PLUS             = 3,
    PROG_SEL_3MTSE_PLUS_FRR_UNTAGGED                  = 4,
    PROG_SEL_3MTSE_PLUS_FRR_TAG_1                     = 5

}ARAD_PP_DBAL_VTT_PROG_SEL_3MTSE_PLUS_FRR_TYPE;

typedef enum
{
    VTT_3MTSE_FRR_L1_L1,       
    VTT_3MTSE_FRR_L3_L3,
    VTT_3MTSE_FRR_L3_L1,
    VTT_3MTSE_FRR_L4_L3,
    VTT_3MTSE_FRR_L1,
    VTT_3MTSE_FRR_L2_L1
}ARAD_PP_DBAL_VTT_3MTSE_PLUS_FRR_TYPE;

typedef enum
{
    VTT_2MTSE_FRR_L1_L1,       
    VTT_2MTSE_FRR_L2_L2,
    VTT_2MTSE_FRR_L2_L1,
    VTT_2MTSE_FRR_L3_L2
}ARAD_PP_DBAL_VTT_2MTSE_PLUS_FRR_TYPE;

typedef enum
{
    VTT_3MTSE_PLUS_FRR_M1,
    VTT_3MTSE_PLUS_FRR_M2_HIT_SEM,
    VTT_3MTSE_PLUS_FRR_M2_MISS_SEM,
    VTT_3MTSE_PLUS_FRR_M2,
    VTT_3MTSE_PLUS_FRR_M3_PLUS_ELI_IML_SEM_HIT,
    VTT_3MTSE_PLUS_FRR_M3_PLUS_ELI_IML_SEM_MISS,
    VTT_3MTSE_PLUS_FRR_M3_PLUS_ELI,
    VTT_3MTSE_PLUS_FRR_M3_ELI,
    VTT_3MTSE_PLUS_FRR_M3_HIT_SEM_AND_TCAM,
    VTT_3MTSE_PLUS_FRR_M3_DC_SEM_AND_TCAM,
    VTT_3MTSE_PLUS_FRR_M3_HIT_SEM,
    VTT_3MTSE_PLUS_FRR_M3_MISS_SEM,
    VTT_3MTSE_PLUS_FRR_M3_PLUS_HIT_SEM_AND_TCAM,
    VTT_3MTSE_PLUS_FRR_M3_PLUS_HIT_TCAM,
    VTT_3MTSE_PLUS_FRR_M3_PLUS_MISS_TCAM,
    VTT_3MTSE_PLUS_FRR_M3_PLUS_MISS_SEM_AND_TCAM,
    VTT_3MTSE_PLUS_FRR_M3_PLUS_ELI_GAL,
    VTT_3MTSE_PLUS_FRR_M3_PLUS_ELI_HIT_SEM,
    VTT_3MTSE_PLUS_FRR_M3_PLUS_ELI_MISS_SEM,
    VTT_3MTSE_PLUS_FRR_M3_PLUS_ELI_GAL_HIT_SEM_AND_TCAM,
    VTT_3MTSE_PLUS_FRR_M3_PLUS_ELI_GAL_DC_SEM_AND_TCAM,
    VTT_3MTSE_PLUS_FRR_M3_PLUS_NO_RANGE_HIT_TCAM,
    VTT_3MTSE_PLUS_FRR_M3_PLUS_NO_RANGE_MISS_TCAM,
    VTT_3MTSE_PLUS_FRR_M3_PLUS_DFLT_HIT_SEM_AND_TCAM,
    VTT_3MTSE_PLUS_FRR_M3_PLUS_DFLT_DC_SEM_AND_TCAM
}ARAD_PP_DBAL_TT_3MTSE_PLUS_FRR_TYPE;

typedef enum
{
    VTT_2MTSE_PLUS_FRR_M1,
    VTT_2MTSE_PLUS_FRR_M3_PLUS_R1_ELI,
    VTT_2MTSE_PLUS_FRR_M2_HIT_TCAM,
    VTT_2MTSE_PLUS_FRR_M2_MISS_TCAM,
    VTT_2MTSE_PLUS_FRR_M3_PLUS_R3_GAL_HIT_TCAM,
    VTT_2MTSE_PLUS_FRR_M3_PLUS_R3_GAL_MISS_TCAM,
    VTT_2MTSE_PLUS_FRR_M3_R2_IML,
    VTT_2MTSE_PLUS_FRR_M2,
    VTT_2MTSE_PLUS_FRR_M3_ELI,
    VTT_2MTSE_PLUS_FRR_M3_PLUS_ELI_GAL,
    VTT_2MTSE_PLUS_FRR_M3_PLUS_ELI_HIT_TCAM,
    VTT_2MTSE_PLUS_FRR_M3_PLUS_ELI_MISS_TCAM,
    VTT_2MTSE_PLUS_FRR_M3_PLUS_R2_ELI,
    VTT_2MTSE_PLUS_FRR_M3_PLUS_NO_RANGE_HIT_TCAM,
    VTT_2MTSE_PLUS_FRR_M3_PLUS_NO_RANGE_MISS_TCAM

}ARAD_PP_DBAL_TT_2MTSE_PLUS_FRR_TYPE;


typedef struct
{
    uint32 prefix;        
    uint32 prefix_len;    
    int nof_qulifiers;    
    SOC_DPP_DBAL_QUAL_INFO   qual_info[SOC_PPC_FP_NOF_QUALS_PER_DB_MAX]; 
}ARAD_PP_DBAL_VTT_SW_DB_PROPERTY;

typedef struct
{
    uint8    valid_indication[BCM_MAX_NUM_UNITS];  
    char* name;               
    SOC_DPP_DBAL_SW_TABLE_IDS dbal_sw_db_id;  
    void   (*vtt_table_property_set) (int unit, ARAD_PP_DBAL_VTT_SW_DB_PROPERTY* sw_db_property, uint32 additional_info); 
    SOC_DPP_DBAL_PHYSICAL_DB_TYPES mem_type;    
    uint32 table_additional_info;  
} ARAD_PP_DBAL_VTT_TABLE;

typedef union
{
    uint8  qualifier_to_ce_id[SOC_DPP_DBAL_PROGRAM_NOF_KEYS][SOC_PPC_FP_NOF_QUALS_PER_DB_MAX]; 
    uint8  use_32_bit_ce     [SOC_DPP_DBAL_PROGRAM_NOF_KEYS][SOC_PPC_FP_NOF_QUALS_PER_DB_MAX];
}ARAD_PP_QUALIFIER_TO_CE_INFO;

typedef struct
{
    uint32 nof_keys;             
    uint32 nof_dbs;              
    uint32 vtt_table_index[3];   
    uint32 implicit_flag;        
    ARAD_PP_QUALIFIER_TO_CE_INFO qual_to_ce_info;    
    uint32 tcam_profile;         
    uint32 pd_bitmap_0; 
    uint32 pd_bitmap_1; 
    uint32 processing_profile;
    uint32 key_program_variable;

    
    uint32 isa_key_initial_from_vt;
    uint32 isb_key_initial_from_vt;
    uint32 tcam_key_initial_from_vt;
    uint32 result_to_use_0; 
    uint32 result_to_use_1; 
    uint32 result_to_use_2; 
    uint32 processing_code;
    uint32 second_stage_parsing;
    uint32 terminate_first_label;

    
    uint8 is_use_strength;
    uint8 pd_0_tcam_strength;
    uint8 pd_0_isa_strength;
    uint8 pd_0_isb_strength;
    uint8 pd_1_tcam_strength;
    uint8 pd_1_isa_strength;
    uint8 pd_1_isb_strength;
}ARAD_PP_DBAL_VTT_PROGRAM_PROPERTY;

typedef struct
{
    ARAD_PP_DBAL_VTT_PROGRAMS   prog_name;  
    void   (*vtt_program_property_set)  (int unit, ARAD_PP_DBAL_VTT_PROGRAM_PROPERTY *prog, uint32 additional_info); 
    uint32 prog_used[BCM_MAX_NUM_UNITS];   
    uint32 additional_info;
}ARAD_PP_DBAL_VTT_PROGRAM_INFO;

typedef struct
{
	    ARAD_PP_DBAL_VTT_PROGRAM_SELECTION vtt_prog_selection_name;  
    int32   (*vt_program_selection_set) (int unit, ARAD_PP_IHP_VTT1ST_KEY_PROGRAM_SELECTION_CAM_TBL_DATA   *prog_selection, int next_line, uint32 additional_info); 
    uint32  priority[BCM_MAX_NUM_UNITS];                         
    ARAD_PP_DBAL_VTT_PROGRAMS prog_name[BCM_MAX_NUM_UNITS];      
    uint32 additional_info;
} ARAD_PP_DBAL_VT_PROGRAM_SELECTION_INFO;

typedef struct
{
    ARAD_PP_DBAL_VTT_PROGRAM_SELECTION vtt_prog_selection_name;  
    int32   (*tt_program_selection_set) (int unit, ARAD_PP_IHP_VTT2ND_KEY_PROGRAM_SELECTION_CAM_TBL_DATA   *prog_selection, int next_line, uint32 additional_info); 
    uint32  priority[BCM_MAX_NUM_UNITS];                         
    ARAD_PP_DBAL_VTT_PROGRAMS prog_name[BCM_MAX_NUM_UNITS];      
    uint32 additional_info;
} ARAD_PP_DBAL_TT_PROGRAM_SELECTION_INFO;

#include <soc/dpp/SAND/Utils/sand_footer.h>

#endif 

