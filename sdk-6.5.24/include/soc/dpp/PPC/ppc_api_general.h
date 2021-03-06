/* 
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2021 Broadcom Inc. All rights reserved.
*/

#ifndef __SOC_PPC_API_GENERAL_INCLUDED__

#define __SOC_PPC_API_GENERAL_INCLUDED__




#include <soc/dpp/SAND/Utils/sand_header.h>
#include <soc/dpp/SAND/SAND_FM/sand_pp_general.h>

#include <soc/dpp/SAND/Management/sand_general_macros.h>
#include <soc/dpp/SAND/Management/sand_error_code.h>

#include <soc/dpp/TMC/tmc_api_general.h>





#define SOC_PPC_DEBUG                               (SOC_SAND_DEBUG)
#define SOC_PPC_DEBUG_IS_LVL1                       (SOC_PPC_DEBUG >= SOC_SAND_DBG_LVL1)
#define SOC_PPC_DEBUG_IS_LVL2                       (SOC_PPC_DEBUG >= SOC_SAND_DBG_LVL2)
#define SOC_PPC_DEBUG_IS_LVL3                       (SOC_PPC_DEBUG >= SOC_SAND_DBG_LVL3)

#define SOC_PPC_VLAN_TAGS_MAX                         2




#define SOC_PPC_EEI_IDENTIFIER_POP_VAL                       (8)
#define SOC_PPC_EEI_IDENTIFIER_SWAP_VAL                      (9)
#define SOC_PPC_EEI_IDENTIFIER_TRILL_VAL                     (10)
#define SOC_PPC_EEI_IDENTIFIER_ENCAP_VAL                     (15)
#define SOC_PPC_EEI_IDENTIFIER_ENCAP_VAL_2_MSBS               (3)

#define SOC_PPC_RIF_NULL  (0xffffffff)
#define SOC_PPC_EEP_NULL  (0xffffffff)

#define SOC_PPC_MAX_NOF_LOCAL_PORTS_PETRA (64)
#define SOC_PPC_MAX_NOF_LOCAL_PORTS_ARAD  (256)
#define SOC_PPC_MAX_NOF_ADDITIONAL_TPID_VALS (4)







#define SOC_PPC_IP_ROUTING_TABLE_ITER_IS_END(iter) \
          ((((iter)->arr[0] == SOC_SAND_U32_MAX) && (((iter)->arr[1] == SOC_SAND_U32_MAX))))






typedef uint32 SOC_PPC_TM_PORT;



typedef uint32 SOC_PPC_PORT;



typedef uint32 SOC_PPC_FID;



typedef uint32 SOC_PPC_VSI_ID;



typedef uint32 SOC_PPC_SYS_VSI_ID;



typedef uint32 SOC_PPC_FEC_ID;



typedef uint32 SOC_PPC_VRF_ID;


typedef uint32 SOC_PPC_VLAN_TAG;


typedef uint32 SOC_PPC_AC_ID;


#define SOC_PPC_AC_ID_INVALID 	(0xFFFFFFFF)


#define SOC_PPC_VRF_INVALID 	(0xFFFFFFFF)


typedef uint32 SOC_PPC_RIF_ID;



typedef uint32 SOC_PPC_LIF_ID;



typedef uint32 SOC_PPC_MP_LEVEL;


typedef uint32 SOC_SAND_PP_ECID;


typedef enum
{
  
  SOC_PPC_EEI_TYPE_EMPTY = 0,
  
  SOC_PPC_EEI_TYPE_TRILL = 1,
  
  SOC_PPC_EEI_TYPE_MPLS = 2,
  
  SOC_PPC_EEI_TYPE_MIM = 3,
  
  SOC_PPC_NOF_EEI_TYPES_PB = 4,
  
  SOC_PPC_EEI_TYPE_OUTLIF = 4,
  
  SOC_PPC_EEI_TYPE_RAW = 5,
  
  SOC_PPC_NOF_EEI_TYPES_ARAD = 6
}SOC_PPC_EEI_TYPE;

typedef enum
{
  
  SOC_PPC_OUTLIF_ENCODE_TYPE_NONE = 0,
  
  SOC_PPC_OUTLIF_ENCODE_TYPE_RAW = 1,
  
  SOC_PPC_OUTLIF_ENCODE_TYPE_AC = 2,
  
  SOC_PPC_OUTLIF_ENCODE_TYPE_EEP = 3,
  
  SOC_PPC_OUTLIF_ENCODE_TYPE_VSI = 4,
  
  SOC_PPC_OUTLIF_ENCODE_TYPE_RAW_INVALID = 5,
  
  SOC_PPC_NOF_OUTLIF_ENCODE_TYPES = 6
}SOC_PPC_OUTLIF_ENCODE_TYPE;

typedef enum
{
  
  SOC_PPC_FRWRD_DECISION_TYPE_DROP = 0,
  
  SOC_PPC_FRWRD_DECISION_TYPE_UC_FLOW = 1,
  
  SOC_PPC_FRWRD_DECISION_TYPE_UC_LAG = 2,
  
  SOC_PPC_FRWRD_DECISION_TYPE_UC_PORT = 3,
  
  SOC_PPC_FRWRD_DECISION_TYPE_MC = 4,
  
  SOC_PPC_FRWRD_DECISION_TYPE_FEC = 5,
  
  SOC_PPC_FRWRD_DECISION_TYPE_TRAP = 6,
  
  SOC_PPC_NOF_FRWRD_DECISION_TYPES = 7
}SOC_PPC_FRWRD_DECISION_TYPE;

#define SOC_PPC_MPLS_COMMAND_TYPE_PUSH                     SOC_TMC_MPLS_COMMAND_TYPE_PUSH
#define SOC_PPC_MPLS_COMMAND_TYPE_POP                      SOC_TMC_MPLS_COMMAND_TYPE_POP
#define SOC_PPC_MPLS_COMMAND_TYPE_POP_INTO_MPLS_PIPE       SOC_TMC_MPLS_COMMAND_TYPE_POP_INTO_MPLS_PIPE
#define SOC_PPC_MPLS_COMMAND_TYPE_POP_INTO_MPLS_UNIFORM    SOC_TMC_MPLS_COMMAND_TYPE_POP_INTO_MPLS_UNIFORM
#define SOC_PPC_MPLS_COMMAND_TYPE_POP_INTO_IPV4_PIPE       SOC_TMC_MPLS_COMMAND_TYPE_POP_INTO_IPV4_PIPE
#define SOC_PPC_MPLS_COMMAND_TYPE_POP_INTO_IPV4_UNIFORM    SOC_TMC_MPLS_COMMAND_TYPE_POP_INTO_IPV4_UNIFORM
#define SOC_PPC_MPLS_COMMAND_TYPE_POP_INTO_IPV6_PIPE       SOC_TMC_MPLS_COMMAND_TYPE_POP_INTO_IPV6_PIPE
#define SOC_PPC_MPLS_COMMAND_TYPE_POP_INTO_IPV6_UNIFORM    SOC_TMC_MPLS_COMMAND_TYPE_POP_INTO_IPV6_UNIFORM
#define SOC_PPC_MPLS_COMMAND_TYPE_POP_INTO_ETHERNET        SOC_TMC_MPLS_COMMAND_TYPE_POP_INTO_ETHERNET
#define SOC_PPC_MPLS_COMMAND_TYPE_SWAP                     SOC_TMC_MPLS_COMMAND_TYPE_SWAP
#define SOC_PPC_NOF_MPLS_COMMAND_TYPES                     SOC_TMC_NOF_MPLS_COMMAND_TYPES
typedef SOC_TMC_MPLS_COMMAND_TYPE                          SOC_PPC_MPLS_COMMAND_TYPE;

typedef enum
{
  
  SOC_PPC_L2_NEXT_PRTCL_TYPE_MAC_IN_MAC = 0x88E7,
  
  SOC_PPC_L2_NEXT_PRTCL_TYPE_TRILL = 0x22F3,
  
  SOC_PPC_L2_NEXT_PRTCL_TYPE_IPV4 = 0x800,
  
  SOC_PPC_L2_NEXT_PRTCL_TYPE_IPV6 = 0x86DD,
  
  SOC_PPC_L2_NEXT_PRTCL_TYPE_ARP = 0x806,
  
  SOC_PPC_L2_NEXT_PRTCL_TYPE_CFM = 0x8902,
  
  SOC_PPC_L2_NEXT_PRTCL_TYPE_MPLS = 0x8847,
  
  SOC_PPC_L2_NEXT_PRTCL_TYPE_FC_ETH = 0x8906,
  
  SOC_PPC_L2_NEXT_PRTCL_TYPE_OTHER = 0x0,
  
  SOC_PPC_NOF_L2_NEXT_PRTCL_TYPES = 11
}SOC_PPC_L2_NEXT_PRTCL_TYPE;

typedef enum
{
  
  SOC_PPC_L3_NEXT_PRTCL_TYPE_NONE = 0x0,
  
  SOC_PPC_L3_NEXT_PRTCL_TYPE_TCP = 0x6,
  
  SOC_PPC_L3_NEXT_PRTCL_TYPE_UDP = 0x11,
  
  SOC_PPC_L3_NEXT_PRTCL_TYPE_IGMP = 0x2,
  
  SOC_PPC_L3_NEXT_PRTCL_TYPE_ICMP = 0x1,
  
  SOC_PPC_L3_NEXT_PRTCL_TYPE_ICMPV6 = 0x3A,
  
  SOC_PPC_L3_NEXT_PRTCL_TYPE_IPV4 = 0x4,
  
  SOC_PPC_L3_NEXT_PRTCL_TYPE_IPV6 = 0x29,
  
  SOC_PPC_L3_NEXT_PRTCL_TYPE_MPLS = 0x89,
  
  SOC_PPC_NOF_L3_NEXT_PRTCL_TYPES = 9
}SOC_PPC_L3_NEXT_PRTCL_TYPE;


typedef enum
{
  
  SOC_PPC_PKT_TERM_TYPE_NONE = 0,
  
  SOC_PPC_PKT_TERM_TYPE_ETH = 1,
  
  SOC_PPC_PKT_TERM_TYPE_IPV4_ETH = 3,
  
  SOC_PPC_PKT_TERM_TYPE_MPLS_ETH = 4,
  
  SOC_PPC_PKT_TERM_TYPE_CW_MPLS_ETH = 5,
  
  SOC_PPC_PKT_TERM_TYPE_MPLS2_ETH = 6,
  
  SOC_PPC_PKT_TERM_TYPE_CW_MPLS2_ETH = 7,
  
  SOC_PPC_PKT_TERM_TYPE_MPLS3_ETH = 8,
  
  SOC_PPC_PKT_TERM_TYPE_CW_MPLS3_ETH = 9,
  
  SOC_PPC_PKT_TERM_TYPE_TRILL = 10,
  
  SOC_PPC_PKT_TERM_TYPE_IPV6_ETH = 11,
  
  SOC_PPC_NOF_PKT_TERM_TYPES = 11
}SOC_PPC_PKT_TERM_TYPE;


#define SOC_PPC_PKT_FRWRD_TYPE_BRIDGE                 SOC_TMC_PKT_FRWRD_TYPE_BRIDGE
#define SOC_PPC_PKT_FRWRD_TYPE_IPV4_UC                SOC_TMC_PKT_FRWRD_TYPE_IPV4_UC
#define SOC_PPC_PKT_FRWRD_TYPE_IPV4_MC                SOC_TMC_PKT_FRWRD_TYPE_IPV4_MC
#define SOC_PPC_PKT_FRWRD_TYPE_IPV6_UC                SOC_TMC_PKT_FRWRD_TYPE_IPV6_UC
#define SOC_PPC_PKT_FRWRD_TYPE_IPV6_MC                SOC_TMC_PKT_FRWRD_TYPE_IPV6_MC
#define SOC_PPC_PKT_FRWRD_TYPE_MPLS                   SOC_TMC_PKT_FRWRD_TYPE_MPLS
#define SOC_PPC_PKT_FRWRD_TYPE_TRILL                  SOC_TMC_PKT_FRWRD_TYPE_TRILL
#define SOC_PPC_PKT_FRWRD_TYPE_CPU_TRAP               SOC_TMC_PKT_FRWRD_TYPE_CPU_TRAP
#define SOC_PPC_PKT_FRWRD_TYPE_BRIDGE_AFTER_TERM      SOC_TMC_PKT_FRWRD_TYPE_BRIDGE_AFTER_TERM
#define SOC_PPC_PKT_FRWRD_TYPE_CUSTOM1                 SOC_TMC_PKT_FRWRD_TYPE_CUSTOM1
#define SOC_PPC_PKT_FRWRD_TYPE_TM                     SOC_TMC_PKT_FRWRD_TYPE_TM
#define SOC_PPC_NOF_PKT_FRWRD_TYPES                   SOC_TMC_NOF_PKT_FRWRD_TYPES
typedef SOC_TMC_PKT_FRWRD_TYPE                        SOC_PPC_PKT_FRWRD_TYPE;

typedef enum
{
  
  SOC_PPC_PKT_HDR_TYPE_NONE = 0,
  
  SOC_PPC_PKT_HDR_TYPE_ETH = 1,
  
  SOC_PPC_PKT_HDR_TYPE_IPV4 = 2,
  
  SOC_PPC_PKT_HDR_TYPE_IPV6 = 3,
  
  SOC_PPC_PKT_HDR_TYPE_MPLS = 4,
  
  SOC_PPC_PKT_HDR_TYPE_TRILL = 5,
  
  SOC_PPC_NOF_PKT_HDR_TYPES = 6
}SOC_PPC_PKT_HDR_TYPE;

typedef enum
{
  
  SOC_PPC_PKT_HDR_STK_TYPE_ETH = 0x1,
  
  SOC_PPC_PKT_HDR_STK_TYPE_ETH_ETH = 0X11,
  
  SOC_PPC_PKT_HDR_STK_TYPE_IPV4_ETH = 0x21,
  
  SOC_PPC_PKT_HDR_STK_TYPE_ETH_IPV4_ETH = 0x121,
  
  SOC_PPC_PKT_HDR_STK_TYPE_IPV6_ETH = 0x31,
  
  SOC_PPC_PKT_HDR_STK_TYPE_MPLS1_ETH = 0x41,
  
  SOC_PPC_PKT_HDR_STK_TYPE_FC_ENCAP_ETH = 0x51,
  
  SOC_PPC_PKT_HDR_STK_TYPE_FC_STD_ETH = 0x61,
  
  SOC_PPC_PKT_HDR_STK_TYPE_MPLS2_ETH = 0x441,
  
  SOC_PPC_PKT_HDR_STK_TYPE_MPLS3_ETH = 0x4441,
  
  SOC_PPC_PKT_HDR_STK_TYPE_ETH_TRILL_ETH = 0x151,
  
  SOC_PPC_PKT_HDR_STK_TYPE_ETH_MPLS1_ETH = 0x141,
  
  SOC_PPC_PKT_HDR_STK_TYPE_ETH_MPLS2_ETH = 0x1441,
  
  SOC_PPC_PKT_HDR_STK_TYPE_ETH_MPLS3_ETH = 0x14441,
  
  SOC_PPC_PKT_HDR_STK_TYPE_IPV4_IPV4_ETH = 0x221,
  
  SOC_PPC_PKT_HDR_STK_TYPE_IPV4_MPLS1_ETH = 0x241,
  
  SOC_PPC_PKT_HDR_STK_TYPE_IPV4_MPLS2_ETH = 0x2441,
  
  SOC_PPC_PKT_HDR_STK_TYPE_IPV4_MPLS3_ETH = 0x24441,
  
  SOC_PPC_PKT_HDR_STK_TYPE_IPV6_IPV4_ETH = 0x321,
  
  SOC_PPC_PKT_HDR_STK_TYPE_IPV6_MPLS1_ETH = 0x341,
  
  SOC_PPC_PKT_HDR_STK_TYPE_IPV6_MPLS2_ETH = 0x3441,
  
  SOC_PPC_PKT_HDR_STK_TYPE_IPV6_MPLS3_ETH = 0x34441,
  
  SOC_PPC_NOF_PKT_HDR_STK_TYPES = 19
}SOC_PPC_PKT_HDR_STK_TYPE;

typedef enum
{
  
  SOC_PPC_IP_ROUTING_TABLE_ITER_TYPE_FAST = 0,
  
  SOC_PPC_IP_ROUTING_TABLE_ITER_TYPE_IP_PREFIX_ORDERED = 1,
  
  SOC_PPC_IP_ROUTING_TABLE_ITER_TYPE_PREFIX_IP_ORDERED = 2,
  
  SOC_PPC_NOF_IP_ROUTING_TABLE_ITER_TYPES = 3
}SOC_PPC_IP_ROUTING_TABLE_ITER_TYPE;

typedef enum
{
  
  SOC_PPC_HASH_MASKS_MAC_SA = 0x1,
  
  SOC_PPC_HASH_MASKS_MAC_DA = 0x2,
  
  SOC_PPC_HASH_MASKS_VSI = 0x4,
  
  SOC_PPC_HASH_MASKS_ETH_TYPE_CODE = 0x8,
  
  SOC_PPC_HASH_MASKS_MPLS_LABEL_1 = 0x10,
  
  SOC_PPC_HASH_MASKS_MPLS_LABEL_2 = 0x20,
  
  SOC_PPC_HASH_MASKS_MPLS_LABEL_3 = 0x40,
  
  SOC_PPC_HASH_MASKS_IPV4_SIP = 0x80,
  
  SOC_PPC_HASH_MASKS_IPV4_DIP = 0x100,
  
  SOC_PPC_HASH_MASKS_IPV4_PROTOCOL = 0x200,
  
  SOC_PPC_HASH_MASKS_IPV6_SIP = 0x400,
  
  SOC_PPC_HASH_MASKS_IPV6_DIP = 0x800,
  
  SOC_PPC_HASH_MASKS_IPV6_PROTOCOL = 0x1000,
  
  SOC_PPC_HASH_MASKS_L4_SRC_PORT = 0x2000,
  
  SOC_PPC_HASH_MASKS_L4_DEST_PORT = 0x4000,
  
  SOC_PPC_HASH_MASKS_FC_DEST_ID = 0x8000,
  
  SOC_PPC_HASH_MASKS_FC_SRC_ID = 0x10000,
  
  SOC_PPC_HASH_MASKS_FC_SEQ_ID = 0x20000,
  
  SOC_PPC_HASH_MASKS_FC_ORG_EX_ID = 0x40000,
  
  SOC_PPC_HASH_MASKS_FC_RES_EX_ID = 0x80000,
  
  SOC_PPC_HASH_MASKS_FC_VFI = 0x100000,
  
  SOC_PPC_HASH_MASKS_TRILL_EG_NICK = 0x200000,
  
  SOC_PPC_HASH_MASKS_MPLS_LABEL_4 = 0x400000,
  
  SOC_PPC_HASH_MASKS_MPLS_LABEL_5 = 0x800000,
  
  SOC_PPC_HASH_MASKS_IPV6_LABLE_LO = 0x1000000,
  
  SOC_PPC_HASH_MASKS_IPV6_LABLE_HI = 0x2000000,
  
  SOC_PPC_NOF_HASH_MASKS_PB = 21,
  SOC_PPC_NOF_HASH_MASKS = 26
}SOC_PPC_HASH_MASKS;



#define SOC_PPC_TRAP_CODE_OAM_CPU   \
            (SOC_PPC_TRAP_CODE_USER_DEFINED_LAST - 5 - SOC_PPC_TRAP_CODE_NOF_USER_DEFINED_SNOOPS_FOR_TM)
#define SOC_PPC_TRAP_CODE_OAM_CPU_SNOOP   \
            (SOC_PPC_TRAP_CODE_USER_DEFINED_LAST - 4 - SOC_PPC_TRAP_CODE_NOF_USER_DEFINED_SNOOPS_FOR_TM)
#define SOC_PPC_TRAP_CODE_OAM_RECYCLE   \
            (SOC_PPC_TRAP_CODE_USER_DEFINED_LAST - 3 - SOC_PPC_TRAP_CODE_NOF_USER_DEFINED_SNOOPS_FOR_TM)
#define SOC_PPC_TRAP_CODE_OAM_CPU_UP   \
            (SOC_PPC_TRAP_CODE_USER_DEFINED_LAST - 2 - SOC_PPC_TRAP_CODE_NOF_USER_DEFINED_SNOOPS_FOR_TM)
#define SOC_PPC_TRAP_CODE_OAM_MIP_EGRESS_SNOOP_WITH_FTMH  \
            (SOC_PPC_TRAP_CODE_USER_DEFINED_LAST - 10 - SOC_PPC_TRAP_CODE_NOF_USER_DEFINED_SNOOPS_FOR_TM)





#define SOC_PPC_TRAP_CODE_ITMH_SNOOP_0     SOC_PPC_TRAP_CODE_USER_DEFINED_44  
#define SOC_PPC_TRAP_CODE_ITMH_SNOOP_1     SOC_PPC_TRAP_CODE_USER_DEFINED_45  
#define SOC_PPC_TRAP_CODE_ITMH_SNOOP_2     SOC_PPC_TRAP_CODE_USER_DEFINED_46  
#define SOC_PPC_TRAP_CODE_ITMH_SNOOP_3     SOC_PPC_TRAP_CODE_USER_DEFINED_47  
#define SOC_PPC_TRAP_CODE_ITMH_SNOOP_4     SOC_PPC_TRAP_CODE_USER_DEFINED_48  
#define SOC_PPC_TRAP_CODE_ITMH_SNOOP_5     SOC_PPC_TRAP_CODE_USER_DEFINED_49  
#define SOC_PPC_TRAP_CODE_ITMH_SNOOP_6     SOC_PPC_TRAP_CODE_USER_DEFINED_50  
#define SOC_PPC_TRAP_CODE_ITMH_SNOOP_7     SOC_PPC_TRAP_CODE_USER_DEFINED_51  
#define SOC_PPC_TRAP_CODE_ITMH_SNOOP_8     SOC_PPC_TRAP_CODE_USER_DEFINED_52  
#define SOC_PPC_TRAP_CODE_ITMH_SNOOP_9     SOC_PPC_TRAP_CODE_USER_DEFINED_53  
#define SOC_PPC_TRAP_CODE_ITMH_SNOOP_10    SOC_PPC_TRAP_CODE_USER_DEFINED_54  
#define SOC_PPC_TRAP_CODE_ITMH_SNOOP_11    SOC_PPC_TRAP_CODE_USER_DEFINED_55  
#define SOC_PPC_TRAP_CODE_ITMH_SNOOP_12    SOC_PPC_TRAP_CODE_USER_DEFINED_56  
#define SOC_PPC_TRAP_CODE_ITMH_SNOOP_13    SOC_PPC_TRAP_CODE_USER_DEFINED_57  
#define SOC_PPC_TRAP_CODE_ITMH_SNOOP_14    SOC_PPC_TRAP_CODE_USER_DEFINED_58  
#define SOC_PPC_TRAP_CODE_ITMH_SNOOP_15    SOC_PPC_TRAP_CODE_USER_DEFINED_59  

#define SOC_PPC_TRAP_CODE_SAT_0            SOC_PPC_TRAP_CODE_USER_DEFINED_26  
#define SOC_PPC_TRAP_CODE_SAT_1            SOC_PPC_TRAP_CODE_USER_DEFINED_25  
#define SOC_PPC_TRAP_CODE_SAT_2            SOC_PPC_TRAP_CODE_USER_DEFINED_24  


#define SOC_PPC_TRAP_CODE_OAM_CPU_MIRROR   \
            (SOC_PPC_TRAP_CODE_USER_DEFINED_LAST - 9 - SOC_PPC_TRAP_CODE_NOF_USER_DEFINED_SNOOPS_FOR_TM)
#define SOC_PPC_TRAP_CODE_OAM_OAMP_MIRROR   \
            (SOC_PPC_TRAP_CODE_USER_DEFINED_LAST - 8 - SOC_PPC_TRAP_CODE_NOF_USER_DEFINED_SNOOPS_FOR_TM)
#define SOC_PPC_TRAP_CODE_OAM_CPU_SNOOP_UP   \
            (SOC_PPC_TRAP_CODE_USER_DEFINED_LAST - 7 - SOC_PPC_TRAP_CODE_NOF_USER_DEFINED_SNOOPS_FOR_TM)
#define SOC_PPC_TRAP_CODE_OAM_CPU_FREE_UP   \
            (SOC_PPC_TRAP_CODE_USER_DEFINED_LAST - 6 - SOC_PPC_TRAP_CODE_NOF_USER_DEFINED_SNOOPS_FOR_TM)


#define SOC_PPC_TRAP_CODE_OAM_FTMH_MIRROR_TRAP_CODES_FIRST (SOC_PPC_TRAP_CODE_OAM_CPU_MIRROR)
#define SOC_PPC_TRAP_CODE_OAM_FTMH_MIRROR_TRAP_CODES_NUM (4)
#define SOC_PPC_TRAP_CODE_OAM_FTMH_MIRROR_TRAP_CODES_MIP_EGRESS_SNOOP (SOC_PPC_TRAP_CODE_OAM_FTMH_MIRROR_TRAP_CODES_FIRST -1)

#define SOC_PPC_TRAP_CODE_OAM_IS_FTMH_MIRROR(trap_code)  ((trap_code>=SOC_PPC_TRAP_CODE_OAM_FTMH_MIRROR_TRAP_CODES_FIRST) && \
                                                          (trap_code<=(SOC_PPC_TRAP_CODE_OAM_FTMH_MIRROR_TRAP_CODES_FIRST + SOC_PPC_TRAP_CODE_OAM_FTMH_MIRROR_TRAP_CODES_NUM)))

#define SOC_PPC_TRAP_CODE_FCOE_WA_FIX_OFFSET       (SOC_PPC_TRAP_CODE_USER_DEFINED_19)

#define SOC_PPC_TRAP_CODE_IPV6_UC_RPF_2PASS   \
            (SOC_PPC_TRAP_CODE_USER_DEFINED_LAST - 11 - SOC_PPC_TRAP_CODE_NOF_USER_DEFINED_SNOOPS_FOR_TM)

#define SOC_PPC_TRAP_CODE_BFD_ECHO   \
            (SOC_PPC_TRAP_CODE_USER_DEFINED_LAST - 12 - SOC_PPC_TRAP_CODE_NOF_USER_DEFINED_SNOOPS_FOR_TM)


#define SOC_PPC_TRAP_CODE_BFD_IPV4_YOUR_DISCR_NOT_FOUND   \
            (SOC_PPC_TRAP_CODE_USER_DEFINED_LAST - 13 - SOC_PPC_TRAP_CODE_NOF_USER_DEFINED_SNOOPS_FOR_TM)

#define SOC_PPC_TRAP_CODE_BFD_IPV6_YOUR_DISCR_NOT_FOUND  SOC_PPC_TRAP_CODE_USER_DEFINED_29

#define SOC_PPC_TRAP_CODE_TRAP_BFD_O_IPV6   SOC_PPC_TRAP_CODE_USER_DEFINED_28

#define SOC_PPC_TRAP_CODE_BFD_ECHO_O_IPV6   SOC_PPC_TRAP_CODE_USER_DEFINED_22

#define SOC_PPC_TRAP_CODE_SER    SOC_PPC_TRAP_CODE_USER_DEFINED_23      
#define SOC_PPC_TRAP_CODE_SER_IS_SUPPORTED(unit)    (SOC_IS_JERICHO_PLUS(unit)) 


#define SOC_PPC_TRAP_CODE_BFD_IPV4_IPV6_YOUR_DISCR_NOT_FOUND SOC_PPC_TRAP_CODE_BFD_IPV4_YOUR_DISCR_NOT_FOUND

typedef enum
{
     
  SOC_PPC_TRAP_CODE_PBP_SA_DROP_0,
  
  SOC_PPC_TRAP_CODE_PBP_SA_DROP_1,
  
  SOC_PPC_TRAP_CODE_PBP_SA_DROP_2,
  
  SOC_PPC_TRAP_CODE_PBP_SA_DROP_3,
  
  SOC_PPC_TRAP_CODE_PBP_TE_TRANSPLANT,
  
  SOC_PPC_TRAP_CODE_PBP_TE_UNKNOWN_TUNNEL,
  
  SOC_PPC_TRAP_CODE_PBP_TRANSPLANT,
  
  SOC_PPC_TRAP_CODE_PBP_LEARN_SNOOP,
  
  SOC_PPC_TRAP_CODE_SA_AUTHENTICATION_FAILED,
  
  SOC_PPC_TRAP_CODE_PORT_NOT_PERMITTED,
  
  SOC_PPC_TRAP_CODE_UNEXPECTED_VID,
  
  SOC_PPC_TRAP_CODE_SA_MULTICAST,
  
  SOC_PPC_TRAP_CODE_SA_EQUALS_DA,
  
  SOC_PPC_TRAP_CODE_8021X,
  
  SOC_PPC_TRAP_CODE_ACCEPTABLE_FRAME_TYPE_DROP,
  
  SOC_PPC_TRAP_CODE_ACCEPTABLE_FRAME_TYPE_ACCEPT,
  
  SOC_PPC_TRAP_CODE_ACCEPTABLE_FRAME_TYPE_CUSTOM_1,
  
  SOC_PPC_TRAP_CODE_ACCEPTABLE_FRAME_TYPE_CUSTOM_2,
  
  SOC_PPC_TRAP_CODE_MY_ARP,
  
  SOC_PPC_TRAP_CODE_ARP,
  
  SOC_PPC_TRAP_CODE_IGMP_MEMBERSHIP_QUERY,
  
  SOC_PPC_TRAP_CODE_IGMP_REPORT_LEAVE_MSG,
  
  SOC_PPC_TRAP_CODE_IGMP_UNDEFINED,
  
  SOC_PPC_TRAP_CODE_ICMPV6_MLD_MC_LISTENER_QUERY,
  
  SOC_PPC_TRAP_CODE_RESERVED_MC_0,
  
  SOC_PPC_TRAP_CODE_RESERVED_MC_1,
  
  SOC_PPC_TRAP_CODE_RESERVED_MC_2,
  
  SOC_PPC_TRAP_CODE_RESERVED_MC_3,
  
  SOC_PPC_TRAP_CODE_RESERVED_MC_4,
  
  SOC_PPC_TRAP_CODE_RESERVED_MC_5,
  
  SOC_PPC_TRAP_CODE_RESERVED_MC_6,
  
  SOC_PPC_TRAP_CODE_RESERVED_MC_7,
  
  SOC_PPC_TRAP_CODE_ICMPV6_MLD_REPORT_DONE_MSG,
  
  SOC_PPC_TRAP_CODE_ICMPV6_MLD_UNDEFINED,
  
  SOC_PPC_TRAP_CODE_DHCP_SERVER,
  
  SOC_PPC_TRAP_CODE_DHCP_CLIENT,
  
  SOC_PPC_TRAP_CODE_DHCPV6_SERVER,
  
  SOC_PPC_TRAP_CODE_DHCPV6_CLIENT,
  
  SOC_PPC_TRAP_CODE_PROG_TRAP_0,
  
  SOC_PPC_TRAP_CODE_PROG_TRAP_1,
  
  SOC_PPC_TRAP_CODE_PROG_TRAP_2,
  
  SOC_PPC_TRAP_CODE_PROG_TRAP_3,
  
  SOC_PPC_TRAP_CODE_PORT_NOT_VLAN_MEMBER,
  
  SOC_PPC_TRAP_CODE_HEADER_SIZE_ERR,
  
  SOC_PPC_TRAP_CODE_HEADER_SIZE_ERR_O_MPLS,
  
  SOC_PPC_TRAP_CODE_MY_B_MAC_AND_LEARN_NULL,
  
  SOC_PPC_TRAP_CODE_MY_B_DA_UNKNOWN_I_SID,
  
  SOC_PPC_TRAP_CODE_MY_B_MAC_MC_CONTINUE,
  
  SOC_PPC_TRAP_CODE_STP_STATE_BLOCK,
  
  SOC_PPC_TRAP_CODE_STP_STATE_LEARN,
  
  SOC_PPC_TRAP_CODE_IP_COMP_MC_INVALID_IP,
  
  SOC_PPC_TRAP_CODE_MY_MAC_AND_IP_DISABLE,
  
  SOC_PPC_TRAP_CODE_ETHER_IP_VERSION_ERROR,
  
  SOC_PPC_TRAP_CODE_LIF_PROTECT_PATH_INVALID,
  
  SOC_PPC_TRAP_CODE_TRILL_VERSION,
  
  SOC_PPC_TRAP_CODE_TRILL_INVALID_TTL,
  
  SOC_PPC_TRAP_CODE_TRILL_CHBH,
  
  SOC_PPC_TRAP_CODE_TRILL_NO_REVERSE_FEC,
  
  SOC_PPC_TRAP_CODE_TRILL_CITE,
  
  SOC_PPC_TRAP_CODE_TRILL_ILLEGAL_INNER_MC,
 
  SOC_PPC_TRAP_CODE_TRILL_NO_ADJACENT,
  
  SOC_PPC_TRAP_CODE_MY_MAC_AND_MPLS_DISABLE,
  
  SOC_PPC_TRAP_CODE_MY_MAC_AND_ARP,
  
  SOC_PPC_TRAP_CODE_MY_MAC_AND_UNKNOWN_L3,
  
  SOC_PPC_TRAP_CODE_MPLS_LABEL_VALUE_0,
  
  SOC_PPC_TRAP_CODE_MPLS_LABEL_VALUE_1,
  
  SOC_PPC_TRAP_CODE_MPLS_LABEL_VALUE_2,
  
  SOC_PPC_TRAP_CODE_MPLS_LABEL_VALUE_3,
  
  SOC_PPC_TRAP_CODE_MPLS_NO_RESOURCES,
  
  SOC_PPC_TRAP_CODE_INVALID_LABEL_IN_RANGE,
  
  SOC_PPC_TRAP_CODE_MPLS_INVALID_LABEL_IN_SEM,
  
  SOC_PPC_TRAP_CODE_MPLS_INVALID_LIF,
 
  SOC_PPC_TRAP_CODE_MPLS_TERM_ERROR,
  
  SOC_PPC_TRAP_CODE_MPLS_LSP_BOS,
  
  SOC_PPC_TRAP_CODE_MPLS_PWE_NO_BOS_LABEL_14,
  
  SOC_PPC_TRAP_CODE_MPLS_PWE_NO_BOS,
  
  SOC_PPC_TRAP_CODE_MPLS_VRF_NO_BOS,
  
  SOC_PPC_TRAP_CODE_MPLS_UNEXPECTED_NO_BOS,
  
  SOC_PPC_TRAP_CODE_MPLS_UNEXPECTED_BOS,
  
  SOC_PPC_TRAP_CODE_MPLS_TERM_TTL_0,
  
  SOC_PPC_TRAP_CODE_MPLS_TERM_TTL_1,
  
  SOC_PPC_TRAP_CODE_MPLS_TERM_CONTROL_WORD_TRAP,
  
  SOC_PPC_TRAP_CODE_MPLS_TERM_CONTROL_WORD_DROP,
  
  SOC_PPC_TRAP_CODE_IPV4_TERM_VERSION_ERROR,
  
  SOC_PPC_TRAP_CODE_IPV4_TERM_CHECKSUM_ERROR,
  
  SOC_PPC_TRAP_CODE_IPV4_TERM_HEADER_LENGTH_ERROR,
  
  SOC_PPC_TRAP_CODE_IPV4_TERM_TOTAL_LENGTH_ERROR,
  
  SOC_PPC_TRAP_CODE_IPV4_TERM_TTL0,
  
  SOC_PPC_TRAP_CODE_IPV4_TERM_HAS_OPTIONS,
  
  SOC_PPC_TRAP_CODE_IPV4_TERM_TTL1,
  
  SOC_PPC_TRAP_CODE_IPV4_TERM_SIP_EQUAL_DIP,
  
  SOC_PPC_TRAP_CODE_IPV4_TERM_DIP_ZERO,
  
  SOC_PPC_TRAP_CODE_IPV4_TERM_SIP_IS_MC,
  
  SOC_PPC_TRAP_CODE_CFM_ACCELERATED_INGRESS,
  
  SOC_PPC_TRAP_CODE_ILLEGEL_PFC,
  
  SOC_PPC_TRAP_CODE_SA_DROP_0,
  
  SOC_PPC_TRAP_CODE_SA_DROP_1,
  
  SOC_PPC_TRAP_CODE_SA_DROP_2,
  
  SOC_PPC_TRAP_CODE_SA_DROP_3,
  
  SOC_PPC_TRAP_CODE_SA_NOT_FOUND_0,
  
  SOC_PPC_TRAP_CODE_SA_NOT_FOUND_1,
  
  SOC_PPC_TRAP_CODE_SA_NOT_FOUND_2,
  
  SOC_PPC_TRAP_CODE_SA_NOT_FOUND_3,
  
  SOC_PPC_TRAP_CODE_UNKNOWN_DA_0,
  
  SOC_PPC_TRAP_CODE_UNKNOWN_DA_1,
  
  SOC_PPC_TRAP_CODE_UNKNOWN_DA_2,
  
  SOC_PPC_TRAP_CODE_UNKNOWN_DA_3,
  
  SOC_PPC_TRAP_CODE_UNKNOWN_DA_4,
  
  SOC_PPC_TRAP_CODE_UNKNOWN_DA_5,
  
  SOC_PPC_TRAP_CODE_UNKNOWN_DA_6,
  
  SOC_PPC_TRAP_CODE_UNKNOWN_DA_7,
  
  SOC_PPC_TRAP_CODE_1588_PACKET_0,
  
  SOC_PPC_TRAP_CODE_1588_PACKET_1,
  
  SOC_PPC_TRAP_CODE_1588_PACKET_2,
  
  SOC_PPC_TRAP_CODE_1588_PACKET_3,
  
  SOC_PPC_TRAP_CODE_1588_PACKET_4,
  
  SOC_PPC_TRAP_CODE_1588_PACKET_5,
  
  SOC_PPC_TRAP_CODE_1588_PACKET_6,
  
  SOC_PPC_TRAP_CODE_1588_PACKET_7,
  
  SOC_PPC_TRAP_CODE_FCOE_SRC_SA_MISMATCH,
  
  SOC_PPC_TRAP_CODE_SIP_TRANSLANT,
  
  SOC_PPC_TRAP_CODE_ELK_ERROR,
  
  SOC_PPC_TRAP_CODE_ELK_REJECTED,
  
  SOC_PPC_TRAP_CODE_DA_NOT_FOUND_0,
  
  SOC_PPC_TRAP_CODE_DA_NOT_FOUND_1,
  
  SOC_PPC_TRAP_CODE_DA_NOT_FOUND_2,
  
  SOC_PPC_TRAP_CODE_DA_NOT_FOUND_3,
  
  SOC_PPC_TRAP_CODE_P2P_MISCONFIGURATION,
  
  SOC_PPC_TRAP_CODE_SAME_INTERFACE,
  
  SOC_PPC_TRAP_CODE_TRILL_UNKNOWN_UC,
  
  SOC_PPC_TRAP_CODE_TRILL_UNKNOWN_MC,
  
  SOC_PPC_TRAP_CODE_UC_LOOSE_RPF_FAIL,
  
  SOC_PPC_TRAP_CODE_DEFAULT_UCV6,
  
  SOC_PPC_TRAP_CODE_DEFAULT_MCV6,
  
  SOC_PPC_TRAP_CODE_MPLS_P2P_NO_BOS,
  
  SOC_PPC_TRAP_CODE_MPLS_CONTROL_WORD_TRAP,
  
  SOC_PPC_TRAP_CODE_MPLS_CONTROL_WORD_DROP,
  
  SOC_PPC_TRAP_CODE_MPLS_UNKNOWN_LABEL,
  
  SOC_PPC_TRAP_CODE_MPLS_P2P_MPLSX4,
  
  SOC_PPC_TRAP_CODE_ETH_L2CP_PEER,
  
  SOC_PPC_TRAP_CODE_ETH_L2CP_DROP,
  
  SOC_PPC_TRAP_CODE_ETH_FL_IGMP_MEMBERSHIP_QUERY,
  
  SOC_PPC_TRAP_CODE_ETH_FL_IGMP_REPORT_LEAVE_MSG,
  
  SOC_PPC_TRAP_CODE_ETH_FL_IGMP_UNDEFINED,
  
  SOC_PPC_TRAP_CODE_ETH_FL_ICMPV6_MLD_MC_LISTENER_QUERY,
  
  SOC_PPC_TRAP_CODE_ETH_FL_ICMPV6_MLD_REPORT_DONE,
  
  SOC_PPC_TRAP_CODE_ETH_FL_ICMPV6_MLD_UNDEFINED,
  
  SOC_PPC_TRAP_CODE_IPV4_VERSION_ERROR,
  
  SOC_PPC_TRAP_CODE_IPV4_CHECKSUM_ERROR,
  
  SOC_PPC_TRAP_CODE_IPV4_HEADER_LENGTH_ERROR,
  
  SOC_PPC_TRAP_CODE_IPV4_TOTAL_LENGTH_ERROR,
  
  SOC_PPC_TRAP_CODE_IPV4_TTL0,
  
  SOC_PPC_TRAP_CODE_IPV4_HAS_OPTIONS,
  
  SOC_PPC_TRAP_CODE_IPV4_TTL1,
  
  SOC_PPC_TRAP_CODE_IPV4_SIP_EQUAL_DIP,
  
  SOC_PPC_TRAP_CODE_IPV4_DIP_ZERO,
  
  SOC_PPC_TRAP_CODE_IPV4_SIP_IS_MC,
  
  SOC_PPC_TRAP_CODE_IPV4_TUNNEL_TERMINATION_AND_FRAGMENTED,
  
  SOC_PPC_TRAP_CODE_IPV6_VERSION_ERROR,
  
  SOC_PPC_TRAP_CODE_IPV6_HOP_COUNT0,
  
  SOC_PPC_TRAP_CODE_IPV6_HOP_COUNT1,
  
  SOC_PPC_TRAP_CODE_IPV6_UNSPECIFIED_DESTINATION,
  
  SOC_PPC_TRAP_CODE_IPV6_LOOPBACK_ADDRESS,
  
  SOC_PPC_TRAP_CODE_IPV6_MULTICAST_SOURCE,
  
  SOC_PPC_TRAP_CODE_IPV6_NEXT_HEADER_NULL,
  
  SOC_PPC_TRAP_CODE_IPV6_UNSPECIFIED_SOURCE,
  
  SOC_PPC_TRAP_CODE_IPV6_LOCAL_LINK_DESTINATION,
  
  SOC_PPC_TRAP_CODE_IPV6_LOCAL_SITE_DESTINATION,
  
  SOC_PPC_TRAP_CODE_IPV6_LOCAL_LINK_SOURCE,
  
  SOC_PPC_TRAP_CODE_IPV6_LOCAL_SITE_SOURCE,
  
  SOC_PPC_TRAP_CODE_IPV6_IPV4_COMPATIBLE_DESTINATION,
  
  SOC_PPC_TRAP_CODE_IPV6_IPV4_MAPPED_DESTINATION,
  
  SOC_PPC_TRAP_CODE_IPV6_MULTICAST_DESTINATION,
  
  SOC_PPC_TRAP_CODE_MPLS_TTL0,
  
  SOC_PPC_TRAP_CODE_MPLS_TTL1,
  
  SOC_PPC_TRAP_CODE_TCP_SN_FLAGS_ZERO,
  
  SOC_PPC_TRAP_CODE_TCP_SN_ZERO_FLAGS_SET,
  
  SOC_PPC_TRAP_CODE_TCP_SYN_FIN,
  
  SOC_PPC_TRAP_CODE_TCP_EQUAL_PORTS,
  
  SOC_PPC_TRAP_CODE_TCP_FRAGMENT_INCOMPLETE_HEADER,
  
  SOC_PPC_TRAP_CODE_TCP_FRAGMENT_OFFSET_LT8,
  
  SOC_PPC_TRAP_CODE_UDP_EQUAL_PORTS,
  
  SOC_PPC_TRAP_CODE_ICMP_DATA_GT_576,
  
  SOC_PPC_TRAP_CODE_ICMP_FRAGMENTED,
  
  SOC_PPC_TRAP_CODE_GENERAL,
  
  SOC_PPC_TRAP_CODE_FACILITY_INVALID,
  
  SOC_PPC_TRAP_CODE_FEC_ENTRY_ACCESSED,
  
  SOC_PPC_TRAP_CODE_UC_STRICT_RPF_FAIL,
  
  SOC_PPC_TRAP_CODE_MC_EXPLICIT_RPF_FAIL,
  
  SOC_PPC_TRAP_CODE_MC_USE_SIP_AS_IS_RPF_FAIL,
  
  SOC_PPC_TRAP_CODE_MC_USE_SIP_RPF_FAIL,
  
  SOC_PPC_TRAP_CODE_MC_USE_SIP_ECMP,
  
  SOC_PPC_TRAP_CODE_ICMP_REDIRECT,
  
  SOC_PPC_TRAP_CODE_MP_NON_ACCELERATED,
  
  SOC_PPC_TRAP_CODE_MORE_THAN_ONE_ING_COUNTER,
  
  SOC_PPC_TRAP_CODE_PWE_INVALID_PROTOCOL,
  
  SOC_PPC_TRAP_CODE_IP_OVER_MPLS,
  
  SOC_PPC_TRAP_CODE_TIME_SYNC,
  
  SOC_PPC_TRAP_CODE_AC_KEY_MISS,
  
  SOC_PPC_TRAP_CODE_EG_MEP_NON_ACCELERATED,
  
  SOC_PPC_TRAP_CODE_EG_FILTER_VSI_MEMBERSHIP,
  
  SOC_PPC_TRAP_CODE_EG_FILTER_HAIR_PIN,
  
  SOC_PPC_TRAP_CODE_EG_FILTER_SPLIT_HORIZON,
  
  SOC_PPC_TRAP_CODE_EG_FILTER_UNKNOWN_DA_UC,
  
  SOC_PPC_TRAP_CODE_EG_FILTER_UNKNOWN_DA_MC,
  
  SOC_PPC_TRAP_CODE_EG_FILTER_UNKNOWN_DA_BC,
  
  SOC_PPC_TRAP_CODE_EG_FILTER_MTU_VIOLATE,
  
  SOC_PPC_TRAP_CODE_EG_FILTER_STP,
  
  SOC_PPC_TRAP_CODE_EG_FILTER_ACCEPT_FRAME,
  
  SOC_PPC_TRAP_CODE_USER_OAMP,
  
  SOC_PPC_TRAP_CODE_USER_ETH_OAM_ACCELERATED,
  
  SOC_PPC_TRAP_CODE_USER_MPLS_OAM_ACCELERATED,
  
  SOC_PPC_TRAP_CODE_USER_BFD_IP_OAM_TUNNEL_ACCELERATED,
  
  SOC_PPC_TRAP_CODE_USER_BFD_PWE_OAM_ACCELERATED,
  
  SOC_PPC_TRAP_CODE_USER_ETH_OAM_UP_ACCELERATED,
  
  SOC_PPC_TRAP_CODE_TRILL_DISABLE_BRIDGE_IF_DESIGNATED,
  
  SOC_PPC_TRAP_CODE_TRAP_ETH_OAM,
  
  SOC_PPC_TRAP_CODE_TRAP_Y1731_O_MPLS_TP,
  
  SOC_PPC_TRAP_CODE_TRAP_Y1731_O_PWE,
  
  SOC_PPC_TRAP_CODE_TRAP_BFD_O_IPV4,
  
  SOC_PPC_TRAP_CODE_TRAP_BFD_O_MPLS,
  
  SOC_PPC_TRAP_CODE_TRAP_BFD_O_PWE,
  
  SOC_PPC_TRAP_CODE_TRAP_BFDCC_O_MPLS_TP,
  
  SOC_PPC_TRAP_CODE_TRAP_BFDCV_O_MPLS_TP,
  
  SOC_PPC_TRAP_CODE_OAM_LEVEL,
  
  SOC_PPC_TRAP_CODE_OAM_PASSIVE,
  
  SOC_PPC_TRAP_CODE_ARP_FLP_FAIL,
  
  SOC_PPC_TRAP_CODE_USER_DEFINED_0,
  
  SOC_PPC_TRAP_CODE_USER_DEFINED_1,
  
  SOC_PPC_TRAP_CODE_USER_DEFINED_2,
  
  SOC_PPC_TRAP_CODE_USER_DEFINED_3,
  
  SOC_PPC_TRAP_CODE_USER_DEFINED_4,
  
  SOC_PPC_TRAP_CODE_USER_DEFINED_5,
  
  SOC_PPC_TRAP_CODE_USER_DEFINED_6,
  
  SOC_PPC_TRAP_CODE_USER_DEFINED_7,
  
  SOC_PPC_TRAP_CODE_USER_DEFINED_8,
  
  SOC_PPC_TRAP_CODE_USER_DEFINED_9,
  
  SOC_PPC_TRAP_CODE_USER_DEFINED_10,
  
  SOC_PPC_TRAP_CODE_USER_DEFINED_11,
  
  SOC_PPC_TRAP_CODE_USER_DEFINED_12,
  
  SOC_PPC_TRAP_CODE_USER_DEFINED_13,
  
  SOC_PPC_TRAP_CODE_USER_DEFINED_14,
  
  SOC_PPC_TRAP_CODE_USER_DEFINED_15,
  
  SOC_PPC_TRAP_CODE_USER_DEFINED_16,
  
  SOC_PPC_TRAP_CODE_USER_DEFINED_17,
  
  SOC_PPC_TRAP_CODE_USER_DEFINED_18,
  
  SOC_PPC_TRAP_CODE_USER_DEFINED_19,
  
  SOC_PPC_TRAP_CODE_USER_DEFINED_20,
  
  SOC_PPC_TRAP_CODE_USER_DEFINED_21,
  
  SOC_PPC_TRAP_CODE_USER_DEFINED_22,
  
  SOC_PPC_TRAP_CODE_USER_DEFINED_23,
  
  SOC_PPC_TRAP_CODE_USER_DEFINED_24,
  
  SOC_PPC_TRAP_CODE_USER_DEFINED_25,
  
  SOC_PPC_TRAP_CODE_USER_DEFINED_26,
  
  SOC_PPC_TRAP_CODE_USER_DEFINED_27,
  
  SOC_PPC_TRAP_CODE_USER_DEFINED_28,
  
  SOC_PPC_TRAP_CODE_USER_DEFINED_29,
  
  SOC_PPC_TRAP_CODE_USER_DEFINED_30,
  
  SOC_PPC_TRAP_CODE_USER_DEFINED_31,
  
  SOC_PPC_TRAP_CODE_USER_DEFINED_32,
  
  SOC_PPC_TRAP_CODE_USER_DEFINED_33,
  
  SOC_PPC_TRAP_CODE_USER_DEFINED_34,
  
  SOC_PPC_TRAP_CODE_USER_DEFINED_35,
  
  SOC_PPC_TRAP_CODE_USER_DEFINED_36,
  
  SOC_PPC_TRAP_CODE_USER_DEFINED_37,
  
  SOC_PPC_TRAP_CODE_USER_DEFINED_38,
  
  SOC_PPC_TRAP_CODE_USER_DEFINED_39,
  
  SOC_PPC_TRAP_CODE_USER_DEFINED_40,
  
  SOC_PPC_TRAP_CODE_USER_DEFINED_41,
  
  SOC_PPC_TRAP_CODE_USER_DEFINED_42,
  
  SOC_PPC_TRAP_CODE_USER_DEFINED_43,
  
  SOC_PPC_TRAP_CODE_USER_DEFINED_44,
  
  SOC_PPC_TRAP_CODE_USER_DEFINED_45,
  
  SOC_PPC_TRAP_CODE_USER_DEFINED_46,
  
  SOC_PPC_TRAP_CODE_USER_DEFINED_47,
  
  SOC_PPC_TRAP_CODE_USER_DEFINED_48,
  
  SOC_PPC_TRAP_CODE_USER_DEFINED_49,
  
  SOC_PPC_TRAP_CODE_USER_DEFINED_50,
  
  SOC_PPC_TRAP_CODE_USER_DEFINED_51,
  
  SOC_PPC_TRAP_CODE_USER_DEFINED_52,
  
  SOC_PPC_TRAP_CODE_USER_DEFINED_53,
  
  SOC_PPC_TRAP_CODE_USER_DEFINED_54,
  
  SOC_PPC_TRAP_CODE_USER_DEFINED_55,
  
  SOC_PPC_TRAP_CODE_USER_DEFINED_56,
  
  SOC_PPC_TRAP_CODE_USER_DEFINED_57,
  
  SOC_PPC_TRAP_CODE_USER_DEFINED_58,
  
  SOC_PPC_TRAP_CODE_USER_DEFINED_59,
  
  SOC_PPC_TRAP_CODE_USER_DEFINED_LAST = SOC_PPC_TRAP_CODE_USER_DEFINED_59,
  
  SOC_PPC_TRAP_CODE_FCOE_FCF_FLP_LOOKUP_FAIL,
  
  SOC_PPC_TRAP_CODE_INNER_IP_COMP_MC_INVALID_IP_0,
  
  SOC_PPC_TRAP_CODE_MPLS_ILLEGAL_LABEL,
  
  SOC_PPC_TRAP_CODE_INNER_ETHERNET_MY_MAC_IP_DISABLED,
  
  SOC_PPC_TRAP_CODE_MPLS_TUNNEL_TERMINATION_TTL0,
  
  SOC_PPC_TRAP_CODE_MPLS_TUNNEL_TERMINATION_TTL1,
  
  SOC_PPC_TRAP_CODE_MPLS_FORWARDING_TTL0,
  
  SOC_PPC_TRAP_CODE_MPLS_FORWARDING_TTL1,
  
  SOC_PPC_NOF_TRAP_CODES
}SOC_PPC_TRAP_CODE;

#define SOC_PPC_NOF_TRAP_QUALIFIERS 4096

typedef struct
{
  SOC_SAND_MAGIC_NUM_VAR
  
  SOC_PPC_TRAP_CODE trap_code;
  
  uint32 frwrd_action_strength;
  
  uint32 snoop_action_strength;

} SOC_PPC_ACTION_PROFILE;

typedef struct
{
  SOC_SAND_MAGIC_NUM_VAR
  
  SOC_PPC_ACTION_PROFILE action_profile;

} SOC_PPC_TRAP_INFO;

typedef struct
{
  SOC_SAND_MAGIC_NUM_VAR
  
  SOC_PPC_OUTLIF_ENCODE_TYPE type;
  
  uint32 val;

} SOC_PPC_OUTLIF;

typedef struct
{
  SOC_SAND_MAGIC_NUM_VAR
  
  SOC_PPC_MPLS_COMMAND_TYPE command;
  
  SOC_SAND_PP_MPLS_LABEL label;
  
  uint32 push_profile;
  
  uint32 tpid_profile;
  
  SOC_SAND_PP_MPLS_TUNNEL_MODEL model;
  
  uint8 has_cw;
  
  SOC_PPC_PKT_FRWRD_TYPE pop_next_header;

} SOC_PPC_MPLS_COMMAND;

typedef struct
{
  SOC_SAND_MAGIC_NUM_VAR
  
  SOC_SAND_PP_TRILL_DEST trill_dest;
  
  SOC_PPC_MPLS_COMMAND mpls_command;
  
  SOC_SAND_PP_ISID isid;
  
  uint32 raw;
  
  uint32 outlif;

} SOC_PPC_EEI_VAL;

typedef struct
{
  SOC_SAND_MAGIC_NUM_VAR
  
  SOC_PPC_EEI_TYPE type;
  
  SOC_PPC_EEI_VAL val;

} SOC_PPC_EEI;

typedef struct
{
  SOC_SAND_MAGIC_NUM_VAR
  
  SOC_PPC_EEI eei;
  
  SOC_PPC_OUTLIF outlif;
  
  SOC_PPC_TRAP_INFO trap_info;

} SOC_PPC_FRWRD_DECISION_TYPE_INFO;

typedef struct
{
  SOC_SAND_MAGIC_NUM_VAR
  
  SOC_PPC_FRWRD_DECISION_TYPE type;
  
  uint32 dest_id;
  
  SOC_PPC_FRWRD_DECISION_TYPE_INFO additional_info;

  uint32 flags;

} SOC_PPC_FRWRD_DECISION_INFO;

typedef struct
{
  SOC_SAND_MAGIC_NUM_VAR
  
  uint8 tpid1_index;
  
  uint8 tpid2_index;

} SOC_PPC_TPID_PROFILE;

typedef struct
{
  SOC_SAND_MAGIC_NUM_VAR
  
  SOC_PPC_PORT port;
  
  SOC_PPC_VSI_ID vsi;

} SOC_PPC_PEP_KEY;

typedef struct
{
  SOC_SAND_MAGIC_NUM_VAR
  
  SOC_PPC_IP_ROUTING_TABLE_ITER_TYPE type;
  
  SOC_SAND_U64 payload;

} SOC_PPC_IP_ROUTING_TABLE_ITER;

typedef struct
{
  SOC_SAND_MAGIC_NUM_VAR
  
  SOC_PPC_IP_ROUTING_TABLE_ITER start;
  
  uint32 entries_to_scan;
  
  uint32 entries_to_act;
  
  uint32 entries_from_lem;
} SOC_PPC_IP_ROUTING_TABLE_RANGE;

typedef struct
{
  SOC_SAND_MAGIC_NUM_VAR
  
  SOC_SAND_PP_TPID tpid_vals[SOC_PPC_MAX_NOF_ADDITIONAL_TPID_VALS];
} SOC_PPC_ADDITIONAL_TPID_VALUES;

typedef struct
{
  SOC_SAND_MAGIC_NUM_VAR
  
  uint32 in_lif;
  
  uint32 inner_tag;
  
  uint32 outer_tag;
  
  uint32 nof_tags;

} SOC_PPC_FRWRD_MATCH_INFO;









void
  SOC_PPC_TRAP_INFO_clear(
    SOC_SAND_OUT SOC_PPC_TRAP_INFO *info
  );

void
  SOC_PPC_OUTLIF_clear(
    SOC_SAND_OUT SOC_PPC_OUTLIF *info
  );

void
  SOC_PPC_MPLS_COMMAND_clear(
    SOC_SAND_OUT SOC_PPC_MPLS_COMMAND *info
  );

void
  SOC_PPC_EEI_VAL_clear(
    SOC_SAND_OUT SOC_PPC_EEI_VAL *info
  );

void
  SOC_PPC_EEI_clear(
    SOC_SAND_OUT SOC_PPC_EEI *info
  );

void
  SOC_PPC_FRWRD_DECISION_TYPE_INFO_clear(
    SOC_SAND_OUT SOC_PPC_FRWRD_DECISION_TYPE_INFO *info
  );

void
  SOC_PPC_FRWRD_DECISION_INFO_clear(
    SOC_SAND_OUT SOC_PPC_FRWRD_DECISION_INFO *info
  );

void
  SOC_PPC_ACTION_PROFILE_clear(
    SOC_SAND_OUT SOC_PPC_ACTION_PROFILE *info
  );

void
  SOC_PPC_TPID_PROFILE_clear(
    SOC_SAND_OUT SOC_PPC_TPID_PROFILE *info
  );

void
  SOC_PPC_PEP_KEY_clear(
    SOC_SAND_OUT SOC_PPC_PEP_KEY *info
  );

void
  SOC_PPC_IP_ROUTING_TABLE_ITER_clear(
    SOC_SAND_OUT SOC_PPC_IP_ROUTING_TABLE_ITER *info
  );

void
  SOC_PPC_IP_ROUTING_TABLE_RANGE_clear(
    SOC_SAND_OUT SOC_PPC_IP_ROUTING_TABLE_RANGE *info
  );

void
  SOC_PPC_ADDITIONAL_TPID_VALUES_clear(
    SOC_SAND_OUT SOC_PPC_ADDITIONAL_TPID_VALUES *info
  );

void
  SOC_PPC_FRWRD_MATCH_INFO_clear(
    SOC_SAND_OUT SOC_PPC_FRWRD_MATCH_INFO *info
  );

#if SOC_PPC_DEBUG_IS_LVL1

const char*
  SOC_PPC_EEI_TYPE_to_string(
    SOC_SAND_IN  SOC_PPC_EEI_TYPE enum_val
  );

const char*
  SOC_PPC_OUTLIF_ENCODE_TYPE_to_string(
    SOC_SAND_IN  SOC_PPC_OUTLIF_ENCODE_TYPE enum_val
  );

void
  SOC_PPC_FRWRD_DECISION_INFO_print_table_format(
    SOC_SAND_IN  char                *line_pref,
    SOC_SAND_IN  SOC_PPC_FRWRD_DECISION_INFO *info
  );

const char*
  SOC_PPC_FRWRD_DECISION_TYPE_to_string(
    SOC_SAND_IN  SOC_PPC_FRWRD_DECISION_TYPE enum_val
  );

const char*
  SOC_PPC_MPLS_COMMAND_TYPE_to_string(
    SOC_SAND_IN  SOC_PPC_MPLS_COMMAND_TYPE enum_val
  );

const char*
  SOC_PPC_L2_NEXT_PRTCL_TYPE_to_string(
    SOC_SAND_IN  SOC_PPC_L2_NEXT_PRTCL_TYPE enum_val
  );

const char*
  SOC_PPC_L3_NEXT_PRTCL_TYPE_to_string(
    SOC_SAND_IN  SOC_PPC_L3_NEXT_PRTCL_TYPE enum_val
  );

const char*
  SOC_PPC_PKT_TERM_TYPE_to_string(
    SOC_SAND_IN  SOC_PPC_PKT_TERM_TYPE enum_val
  );

const char*
  SOC_PPC_PKT_FRWRD_TYPE_to_string(
    SOC_SAND_IN  SOC_PPC_PKT_FRWRD_TYPE enum_val
  );

const char*
  SOC_PPC_PKT_HDR_TYPE_to_string(
    SOC_SAND_IN  SOC_PPC_PKT_HDR_TYPE enum_val
  );

const char*
  SOC_PPC_IP_ROUTING_TABLE_ITER_TYPE_to_string(
    SOC_SAND_IN  SOC_PPC_IP_ROUTING_TABLE_ITER_TYPE enum_val
  );

const char*
  SOC_PPC_HASH_MASKS_to_string(
    SOC_SAND_IN  SOC_PPC_HASH_MASKS enum_val
  );

void
  SOC_PPC_TRAP_INFO_print(
    SOC_SAND_IN  SOC_PPC_TRAP_INFO *info
  );

void
  SOC_PPC_OUTLIF_print(
    SOC_SAND_IN  SOC_PPC_OUTLIF *info
  );

void
  SOC_PPC_MPLS_COMMAND_print(
    SOC_SAND_IN  SOC_PPC_MPLS_COMMAND *info
  );

void
  SOC_PPC_EEI_VAL_print(
    SOC_SAND_IN  SOC_PPC_EEI_VAL *info
  );

void
  SOC_PPC_EEI_print(
    SOC_SAND_IN  SOC_PPC_EEI *info
  );

void
  SOC_PPC_FRWRD_DECISION_TYPE_INFO_print(
    SOC_SAND_IN  SOC_PPC_FRWRD_DECISION_TYPE      frwrd_type,
    SOC_SAND_IN  SOC_PPC_FRWRD_DECISION_TYPE_INFO *info
  );

void
  SOC_PPC_FRWRD_DECISION_INFO_print(
    SOC_SAND_IN  SOC_PPC_FRWRD_DECISION_INFO *info
  );

void
  SOC_PPC_ACTION_PROFILE_print(
    SOC_SAND_IN  SOC_PPC_ACTION_PROFILE *info
  );

void
  SOC_PPC_TPID_PROFILE_print(
    SOC_SAND_IN  SOC_PPC_TPID_PROFILE *info
  );

void
  SOC_PPC_PEP_KEY_print(
    SOC_SAND_IN  SOC_PPC_PEP_KEY *info
  );

void
  SOC_PPC_IP_ROUTING_TABLE_ITER_print(
    SOC_SAND_IN  SOC_PPC_IP_ROUTING_TABLE_ITER *info
  );

void
  SOC_PPC_IP_ROUTING_TABLE_RANGE_print(
    SOC_SAND_IN  SOC_PPC_IP_ROUTING_TABLE_RANGE *info
  );

void
  SOC_PPC_ADDITIONAL_TPID_VALUES_print(
    SOC_SAND_IN  SOC_PPC_ADDITIONAL_TPID_VALUES *info
  );

void
  SOC_PPC_FRWRD_MATCH_INFO_print(
    SOC_SAND_IN SOC_PPC_FRWRD_MATCH_INFO *info
  );

#endif 



#include <soc/dpp/SAND/Utils/sand_footer.h>


#endif
