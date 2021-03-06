/* 
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2021 Broadcom Inc. All rights reserved.
*/


#ifndef __SOC_TMC_API_PMF_LOW_LEVEL_CE_INCLUDED__

#define __SOC_TMC_API_PMF_LOW_LEVEL_CE_INCLUDED__




#include <soc/dpp/SAND/Utils/sand_header.h>

#include <soc/dpp/SAND/Management/sand_general_macros.h>
#include <soc/dpp/SAND/Management/sand_error_code.h>

#include <soc/dpp/TMC/tmc_api_ports.h>














typedef enum
{
  
  SOC_TMC_PMF_CE_SUB_HEADER_0 = 0,
  
  SOC_TMC_PMF_CE_SUB_HEADER_1 = 1,
  
  SOC_TMC_PMF_CE_SUB_HEADER_2 = 2,
  
  SOC_TMC_PMF_CE_SUB_HEADER_3 = 3,
  
  SOC_TMC_PMF_CE_SUB_HEADER_4 = 4,
  
  SOC_TMC_PMF_CE_SUB_HEADER_5 = 5,
  
  SOC_TMC_PMF_CE_SUB_HEADER_FWD = 6,
  
  SOC_TMC_PMF_CE_SUB_HEADER_FWD_POST = 7,
  
  SOC_TMC_NOF_PMF_CE_SUB_HEADERS = 8
}SOC_TMC_PMF_CE_SUB_HEADER;

typedef enum
{
  
  SOC_TMC_PMF_IRPP_INFO_FIELD_TM_PORT = 0,
  
  SOC_TMC_PMF_IRPP_INFO_FIELD_PCKT_HEADER_SIZE = 1,
  
  SOC_TMC_PMF_IRPP_INFO_FIELD_SYST_PORT_SRC = 2,
  
  SOC_TMC_PMF_IRPP_INFO_FIELD_IN_PP_PORT = 3,
  
  SOC_TMC_PMF_IRPP_INFO_FIELD_PP_CONTEXT = 4,
  
  SOC_TMC_PMF_IRPP_INFO_FIELD_HEADER_OFFSET_0 = 5,
  
  SOC_TMC_PMF_IRPP_INFO_FIELD_HEADER_OFFSET_1 = 6,
  
  SOC_TMC_PMF_IRPP_INFO_FIELD_HEADER_OFFSET_2 = 7,
  
  SOC_TMC_PMF_IRPP_INFO_FIELD_HEADER_OFFSET_3 = 8,
  
  SOC_TMC_PMF_IRPP_INFO_FIELD_HEADER_OFFSET_4 = 9,
  
  SOC_TMC_PMF_IRPP_INFO_FIELD_HEADER_OFFSET_5 = 10,
  
  SOC_TMC_PMF_IRPP_INFO_FIELD_PCKT_FORMAT_QUAL_0 = 11,
  
  SOC_TMC_PMF_IRPP_INFO_FIELD_PCKT_FORMAT_QUAL_1 = 12,
  
  SOC_TMC_PMF_IRPP_INFO_FIELD_PCKT_FORMAT_QUAL_2 = 13,
  
  SOC_TMC_PMF_IRPP_INFO_FIELD_PCKT_FORMAT_QUAL_3 = 14,
  
  SOC_TMC_PMF_IRPP_INFO_FIELD_PCKT_FORMAT_QUAL_4 = 15,
  
  SOC_TMC_PMF_IRPP_INFO_FIELD_PCKT_FORMAT_QUAL_5 = 16,
  
  SOC_TMC_PMF_IRPP_INFO_FIELD_PCKT_FORMAT_QUAL1_EXT = 17,
  
  SOC_TMC_PMF_IRPP_INFO_FIELD_PARSER_PMF_PROFILE = 18,
  
  SOC_TMC_PMF_IRPP_INFO_FIELD_PACKET_FORMAT_CODE = 19,
  
  SOC_TMC_PMF_IRPP_INFO_FIELD_LLVP_INCOMING_TAG_STRUCT = 20,
  
  SOC_TMC_PMF_IRPP_INFO_FIELD_FWD_ACTION_STRENGTH = 21,
  
  SOC_TMC_PMF_IRPP_INFO_FIELD_FWD_ACTION_DEST_VALID = 22,
  
  SOC_TMC_PMF_IRPP_INFO_FIELD_FWD_ACTION_TC_VALID = 23,
  
  SOC_TMC_PMF_IRPP_INFO_FIELD_FWD_ACTION_DP_VALID = 24,
  
  SOC_TMC_PMF_IRPP_INFO_FIELD_FWD_ACTION_CPU_TRAP_CODE = 25,
  
  SOC_TMC_PMF_IRPP_INFO_FIELD_FWD_ACTION_CPU_TRAP_QUAL = 26,
  
  SOC_TMC_PMF_IRPP_INFO_FIELD_CPU_TRAP_CODE_PRO = 27,
  
  SOC_TMC_PMF_IRPP_INFO_FIELD_UP = 28,
  
  SOC_TMC_PMF_IRPP_INFO_FIELD_SNOOP_STRENGTH = 29,
  
  SOC_TMC_PMF_IRPP_INFO_FIELD_SNOOP_CODE = 30,
  
  SOC_TMC_PMF_IRPP_INFO_FIELD_LEARN_DEST = 31,
  
  SOC_TMC_PMF_IRPP_INFO_FIELD_LEARN_ASD = 32,
  
  SOC_TMC_PMF_IRPP_INFO_FIELD_SEM_NDX = 33,
  
  SOC_TMC_PMF_IRPP_INFO_FIELD_LL_MIRROR_PROFILE = 34,
  
  SOC_TMC_PMF_IRPP_INFO_FIELD_VSI = 35,
  
  SOC_TMC_PMF_IRPP_INFO_FIELD_SYSTEM_VSI = 36,
  
  SOC_TMC_PMF_IRPP_INFO_FIELD_ORIENTATION_IS_HUB = 37,
  
  SOC_TMC_PMF_IRPP_INFO_FIELD_VLAN_EDIT_TAG = 38,
  
  SOC_TMC_PMF_IRPP_INFO_FIELD_STP_STATE = 39,
  
  SOC_TMC_PMF_IRPP_INFO_FIELD_FWDING_CODE = 40,
  
  SOC_TMC_PMF_IRPP_INFO_FIELD_FWDING_OFFSET_NDX = 41,
  
  SOC_TMC_PMF_IRPP_INFO_FIELD_FWDING_OFFSET_EXT = 42,
  
  SOC_TMC_PMF_IRPP_INFO_FIELD_TUNNEL_TERMINATION_CODE = 43,
  
  SOC_TMC_PMF_IRPP_INFO_FIELD_IN_RIF = 44,
  
  SOC_TMC_PMF_IRPP_INFO_FIELD_VRF = 45,
  
  SOC_TMC_PMF_IRPP_INFO_FIELD_PCKT_IS_COMP_MC = 46,
  
  SOC_TMC_PMF_IRPP_INFO_FIELD_MY_BMAC = 47,
  
  SOC_TMC_PMF_IRPP_INFO_FIELD_EEI_OR_OUTLIF = 48,
  
  SOC_TMC_PMF_IRPP_INFO_FIELD_IN_TTL = 49,
  
  SOC_TMC_PMF_IRPP_INFO_FIELD_IN_DSCP_EXP = 50,
  
  SOC_TMC_PMF_IRPP_INFO_FIELD_RPF_FEC_PTR_VALID = 51,
  
  SOC_TMC_PMF_IRPP_INFO_FIELD_SEM_1ST_LOOKUP_RESULT = 52,
  
  SOC_TMC_PMF_IRPP_INFO_FIELD_SEM_2ND_LOOKUP_RESULT = 53,
  
   SOC_TMC_PMF_IRPP_INFO_FIELD_LEM_1ST_LOOKUP_RESULT_DEST = 54,
  
  SOC_TMC_PMF_IRPP_INFO_FIELD_LEM_2ND_LOOKUP_RESULT_DEST = 55,
  
  SOC_TMC_PMF_IRPP_INFO_FIELD_LPM_1ST_LOOKUP_RESULT = 56,
  
  SOC_TMC_PMF_IRPP_INFO_FIELD_LPM_2ND_LOOKUP_RESULT = 57,
  
  SOC_TMC_PMF_IRPP_INFO_FIELD_TCAM_LOOKUP_RESULT = 58,
  
  SOC_TMC_PMF_IRPP_INFO_FIELD_ELK_LOOKUP_RESULT = 59,
  
  SOC_TMC_PMF_IRPP_INFO_FIELD_KEY_GEN_VAR = 60,
  
  SOC_TMC_PMF_IRPP_INFO_FIELD_COPY_PGM_VARIABLE = 61,
  
  SOC_TMC_PMF_IRPP_INFO_FIELD_PACKET_SIZE_RANGE = 62,
  
  SOC_TMC_PMF_IRPP_INFO_FIELD_L4_OPS = 63,
  
  SOC_TMC_PMF_IRPP_INFO_FIELD_KEY_PGM_PROFILE = 64,
  
  SOC_TMC_PMF_IRPP_INFO_FIELD_CUSTOM1 = 65,
  
  SOC_TMC_PMF_IRPP_INFO_FIELD_CUSTOM2 = 66,
  
  SOC_TMC_PMF_IRPP_INFO_FIELD_CUSTOM3 = 67,
  
  SOC_TMC_PMF_IRPP_INFO_FIELD_TM_PORT32 = 68,
  
  SOC_TMC_PMF_IRPP_INFO_FIELD_ZERO_8 = 69,
  SOC_TMC_PMF_IRPP_INFO_FIELD_ZERO_16 = 70,
  SOC_TMC_PMF_IRPP_INFO_FIELD_ZERO_32 = 71,
  SOC_TMC_PMF_IRPP_INFO_FIELD_ONES_32 = 72,
  SOC_TMC_PMF_IRPP_INFO_FIELD_LEARN_INFO = 73,
  
   SOC_TMC_PMF_IRPP_INFO_FIELD_LEM_1ST_LOOKUP_RESULT_ASD = 74,
  
  SOC_TMC_PMF_IRPP_INFO_FIELD_LEM_2ND_LOOKUP_RESULT_ASD = 75,
  
  SOC_TMC_PMF_IRPP_INFO_FIELD_PACKET_FORMAT_CODE_2MSB = 76,

  
  SOC_TMC_NOF_PMF_IRPP_INFO_FIELDS = 77
}SOC_TMC_PMF_IRPP_INFO_FIELD;

typedef struct
{
  SOC_SAND_MAGIC_NUM_VAR
  
  SOC_TMC_PMF_CE_SUB_HEADER sub_header;
  
  int32 offset;
  
  uint32 nof_bits;

} SOC_TMC_PMF_CE_PACKET_HEADER_INFO;










void
  SOC_TMC_PMF_CE_PACKET_HEADER_INFO_clear(
    SOC_SAND_OUT SOC_TMC_PMF_CE_PACKET_HEADER_INFO *info
  );

#if SOC_TMC_DEBUG_IS_LVL1

const char*
  SOC_TMC_PMF_CE_SUB_HEADER_to_string(
    SOC_SAND_IN  SOC_TMC_PMF_CE_SUB_HEADER enum_val
  );

const char*
  SOC_TMC_PMF_IRPP_INFO_FIELD_to_string(
    SOC_SAND_IN  SOC_TMC_PMF_IRPP_INFO_FIELD enum_val
  );

void
  SOC_TMC_PMF_CE_PACKET_HEADER_INFO_print(
    SOC_SAND_IN  SOC_TMC_PMF_CE_PACKET_HEADER_INFO *info
  );

#endif 



#include <soc/dpp/SAND/Utils/sand_footer.h>


#endif

