/* 
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
*/

#ifndef __SOC_TMC_API_TCAM_KEY_INCLUDED__

#define __SOC_TMC_API_TCAM_KEY_INCLUDED__




#include <soc/dpp/SAND/Utils/sand_header.h>

#include <soc/dpp/SAND/Management/sand_general_macros.h>
#include <soc/dpp/SAND/Management/sand_error_code.h>

#include <soc/dpp/TMC/tmc_api_tcam.h>
#include <soc/dpp/TMC/tmc_api_pmf_low_level_ce.h>






#define  SOC_TMC_TCAM_RULE_NOF_UINT32S_MAX (9)



#define  SOC_TMC_TCAM_KEY_FLD_NOF_UINT32S_MAX (4)









typedef enum
{
  
  SOC_TMC_EGR_ACL_DB_TYPE_ETH = 0,
  
  SOC_TMC_EGR_ACL_DB_TYPE_IPV4 = 1,
  
  SOC_TMC_EGR_ACL_DB_TYPE_MPLS = 2,
  
  SOC_TMC_EGR_ACL_DB_TYPE_TM = 3,
  
  SOC_TMC_EGR_NOF_ACL_DB_TYPES = 4
}SOC_TMC_EGR_ACL_DB_TYPE;

typedef enum
{
  
  SOC_TMC_PMF_TCAM_KEY_SRC_L2 = 0,
  
  SOC_TMC_PMF_TCAM_KEY_SRC_L3_IPV4 = 1,
  
  SOC_TMC_PMF_TCAM_KEY_SRC_L3_IPV6 = 2,
  
  SOC_TMC_PMF_TCAM_KEY_SRC_B_A = 3,
  
  SOC_TMC_PMF_TCAM_KEY_SRC_A_71_0 = 4,
  
  SOC_TMC_PMF_TCAM_KEY_SRC_A_103_32 = 5,
  
  SOC_TMC_PMF_TCAM_KEY_SRC_A_143_0 = 6,
  
  SOC_TMC_PMF_TCAM_KEY_SRC_A_175_32 = 7,
  
  SOC_TMC_PMF_TCAM_KEY_SRC_B_71_0 = 8,
  
  SOC_TMC_PMF_TCAM_KEY_SRC_B_103_32 = 9,
  
  SOC_TMC_PMF_TCAM_KEY_SRC_B_143_0 = 10,
  
  SOC_TMC_PMF_TCAM_KEY_SRC_B_175_32 = 11,
  
  SOC_TMC_NOF_PMF_TCAM_KEY_SRCS = 12
}SOC_TMC_PMF_TCAM_KEY_SRC;

typedef enum
{
  
  SOC_TMC_TCAM_KEY_FORMAT_TYPE_PMF = 0,
  
  SOC_TMC_TCAM_KEY_FORMAT_TYPE_EGR_ACL = 1,
  
  SOC_TMC_TCAM_NOF_KEY_FORMAT_TYPES = 2
}SOC_TMC_TCAM_KEY_FORMAT_TYPE;

typedef enum
{
  
  SOC_TMC_PMF_TCAM_FLD_L2_LLVP = 0,
  
  SOC_TMC_PMF_TCAM_FLD_L2_STAG = 1,
  
  SOC_TMC_PMF_TCAM_FLD_L2_CTAG_IN_AC = 2,
  
  SOC_TMC_PMF_TCAM_FLD_L2_SA = 3,
  
  SOC_TMC_PMF_TCAM_FLD_L2_DA = 4,
  
  SOC_TMC_PMF_TCAM_FLD_L2_ETHERTYPE = 5,
  
  SOC_TMC_PMF_TCAM_FLD_L2_IN_PP_PORT = 6,
  
  SOC_TMC_NOF_PMF_TCAM_FLD_L2S = 7
}SOC_TMC_PMF_TCAM_FLD_L2;

typedef enum
{
  
  SOC_TMC_PMF_TCAM_FLD_L3_MODE_IPV4 = 0,
  
  SOC_TMC_PMF_TCAM_FLD_L3_MODE_IPV6 = 1,
  
  SOC_TMC_NOF_PMF_TCAM_FLD_L3_MODES = 2
}SOC_TMC_PMF_TCAM_FLD_L3_MODE;

typedef enum
{
  
  SOC_TMC_PMF_FLD_IPV4_L4OPS = 0,
  
  SOC_TMC_PMF_FLD_IPV4_NEXT_PRTCL = 1,
  
  SOC_TMC_PMF_FLD_IPV4_DF = 2,
  
  SOC_TMC_PMF_FLD_IPV4_MF = 3,
  
  SOC_TMC_PMF_FLD_IPV4_FRAG_NON_0 = 4,
  
  SOC_TMC_PMF_FLD_IPV4_L4OPS_FLAG = 5,
  
  SOC_TMC_PMF_FLD_IPV4_SIP = 6,
  
  SOC_TMC_PMF_FLD_IPV4_DIP = 7,
  
  SOC_TMC_PMF_FLD_IPV4_L4OPS_OPT = 8,
  
  SOC_TMC_PMF_FLD_IPV4_SRC_PORT = 9,
  
  SOC_TMC_PMF_FLD_IPV4_DEST_PORT = 10,
  
  SOC_TMC_PMF_FLD_IPV4_TOS = 11,
  
  SOC_TMC_PMF_FLD_IPV4_TCP_CTL = 12,
  
  SOC_TMC_PMF_FLD_IPV4_IN_AC_VRF = 13,
  
  SOC_TMC_PMF_FLD_IPV4_IN_PP_PORT = 14,
  
  SOC_TMC_PMF_FLD_IPV4_IN_VID = 15,
  
  SOC_TMC_NOF_PMF_FLD_IPV4S = 16
}SOC_TMC_PMF_FLD_IPV4;

typedef enum
{
  
  SOC_TMC_PMF_FLD_IPV6_L4OPS = 0,
  
  SOC_TMC_PMF_FLD_IPV6_SIP_HIGH = 1,
  SOC_TMC_PMF_FLD_IPV6_SIP_LOW = 2,
  
   SOC_TMC_PMF_FLD_IPV6_DIP_HIGH = 3,
   SOC_TMC_PMF_FLD_IPV6_DIP_LOW = 4,
  
  SOC_TMC_PMF_FLD_IPV6_NEXT_PRTCL = 5,
  
  SOC_TMC_PMF_FLD_IPV6_PP_PORT_TCP_CTL = 6,
  
  SOC_TMC_PMF_FLD_IPV6_IN_AC_VRF = 7,
  
  SOC_TMC_NOF_PMF_FLD_IPV6S = 8
}SOC_TMC_PMF_FLD_IPV6;

typedef enum
{
  
  SOC_TMC_PMF_TCAM_FLD_A_B_LOC_IRPP = 0,
  
  SOC_TMC_PMF_TCAM_FLD_A_B_LOC_HDR = 1,
  
  SOC_TMC_NOF_PMF_TCAM_FLD_A_B_LOCS = 2
}SOC_TMC_PMF_TCAM_FLD_A_B_LOC;

typedef enum
{
  
  SOC_TMC_EGR_ACL_TCAM_FLD_L2_ETH_TYPE_CODE = 0,
  
  SOC_TMC_EGR_ACL_TCAM_FLD_L2_ETH_TAG_FORMAT = 1,
  
  SOC_TMC_EGR_ACL_TCAM_FLD_L2_OUTER_TAG = 2,
  
  SOC_TMC_EGR_ACL_TCAM_FLD_L2_INNER_TAG = 3,
  
  SOC_TMC_EGR_ACL_TCAM_FLD_L2_SA = 4,
  
  SOC_TMC_EGR_ACL_TCAM_FLD_L2_DA = 5,
  
  SOC_TMC_EGR_ACL_TCAM_FLD_L2_OUT_PP_PORT_ACL_DATA = 6,
  
  SOC_TMC_EGR_NOF_ACL_TCAM_FLD_L2S = 7
}SOC_TMC_EGR_ACL_TCAM_FLD_L2;

typedef enum
{
  
  SOC_TMC_EGR_ACL_TCAM_FLD_IPV4_TOS = 0,
  
  SOC_TMC_EGR_ACL_TCAM_FLD_IPV4_L4_PRO_CODE = 1,
  
  SOC_TMC_EGR_ACL_TCAM_FLD_IPV4_SIP = 2,
  
  SOC_TMC_EGR_ACL_TCAM_FLD_IPV4_DIP = 3,
  
  SOC_TMC_EGR_ACL_TCAM_FLD_IPV4_OUTER_VID = 4,
  
  SOC_TMC_EGR_ACL_TCAM_FLD_IPV4_OUT_PP_PORT_ACL_DATA = 5,
  
  SOC_TMC_EGR_NOF_ACL_TCAM_FLD_IPV4S = 6
}SOC_TMC_EGR_ACL_TCAM_FLD_IPV4;

typedef enum
{
  
  SOC_TMC_EGR_ACL_TCAM_FLD_MPLS_FTMH = 0,
  
  SOC_TMC_EGR_ACL_TCAM_FLD_MPLS_HDR_DATA = 1,
  
   SOC_TMC_EGR_ACL_TCAM_FLD_MPLS_LABEL = 2,
   SOC_TMC_EGR_ACL_TCAM_FLD_MPLS_EXP = 3,
   SOC_TMC_EGR_ACL_TCAM_FLD_MPLS_TTL = 4,
  
  SOC_TMC_EGR_ACL_TCAM_FLD_MPLS_OUT_PP_PORT_ACL_DATA = 5,
  
  SOC_TMC_EGR_NOF_ACL_TCAM_FLD_MPLSS = 6
}SOC_TMC_EGR_ACL_TCAM_FLD_MPLS;

typedef enum
{
  
  SOC_TMC_EGR_ACL_TCAM_FLD_TM_FTMH = 0,
  
  SOC_TMC_EGR_ACL_TCAM_FLD_TM_HDR_DATA = 1,
  
  SOC_TMC_EGR_ACL_TCAM_FLD_TM_OUT_PP_PORT_ACL_DATA = 2,
  
  SOC_TMC_EGR_NOF_ACL_TCAM_FLD_TMS = 3
}SOC_TMC_EGR_ACL_TCAM_FLD_TM;

typedef struct
{
  SOC_SAND_MAGIC_NUM_VAR
  
  uint32 val[SOC_TMC_TCAM_RULE_NOF_UINT32S_MAX];
  
  uint32 mask[SOC_TMC_TCAM_RULE_NOF_UINT32S_MAX];

} SOC_TMC_TCAM_RULE_INFO;

typedef struct
{
  SOC_SAND_MAGIC_NUM_VAR
  
  SOC_TMC_TCAM_KEY_FORMAT_TYPE type;
  
  SOC_TMC_PMF_TCAM_KEY_SRC pmf;
  
  SOC_TMC_EGR_ACL_DB_TYPE egr_acl;

} SOC_TMC_TCAM_KEY_FORMAT;

typedef struct
{
  SOC_SAND_MAGIC_NUM_VAR
  
  SOC_TMC_TCAM_BANK_ENTRY_SIZE size;
  
  SOC_TMC_TCAM_RULE_INFO data;
  
  SOC_TMC_TCAM_KEY_FORMAT format;
  
  uint32 pmf_pgm_id;

} SOC_TMC_TCAM_KEY;

typedef struct
{
  SOC_SAND_MAGIC_NUM_VAR
  
  uint32 val;

} SOC_TMC_TCAM_OUTPUT;

typedef struct
{
  SOC_SAND_MAGIC_NUM_VAR
  
  SOC_TMC_PMF_TCAM_FLD_L3_MODE mode;
  
  SOC_TMC_PMF_FLD_IPV4 ipv4_fld;
  
  SOC_TMC_PMF_FLD_IPV6 ipv6_fld;

} SOC_TMC_PMF_TCAM_FLD_L3;

typedef struct
{
  SOC_SAND_MAGIC_NUM_VAR
  
  SOC_TMC_PMF_TCAM_FLD_A_B_LOC loc;
  
  SOC_TMC_PMF_IRPP_INFO_FIELD irpp_fld;
  
  SOC_TMC_PMF_CE_SUB_HEADER sub_header;
  
  int32 sub_header_offset;
  
  uint32 pmf_pgm_id;

} SOC_TMC_PMF_TCAM_FLD_A_B;


typedef union
{
  
  SOC_TMC_PMF_TCAM_FLD_L2 l2;
  
  SOC_TMC_PMF_TCAM_FLD_L3 l3;
  
  SOC_TMC_PMF_TCAM_FLD_A_B a_b;
  
  SOC_TMC_EGR_ACL_TCAM_FLD_L2 egr_l2;
  
  SOC_TMC_EGR_ACL_TCAM_FLD_IPV4 egr_ipv4;
  
  SOC_TMC_EGR_ACL_TCAM_FLD_MPLS egr_mpls;
  
  SOC_TMC_EGR_ACL_TCAM_FLD_TM egr_tm;

} SOC_TMC_TCAM_KEY_FLD_TYPE;

typedef struct
{
  SOC_SAND_MAGIC_NUM_VAR
  
  uint32 val[SOC_TMC_TCAM_KEY_FLD_NOF_UINT32S_MAX];

} SOC_TMC_TCAM_KEY_FLD_VAL;









void
  SOC_TMC_TCAM_RULE_INFO_clear(
    SOC_SAND_OUT SOC_TMC_TCAM_RULE_INFO *info
  );

void
  SOC_TMC_TCAM_KEY_FORMAT_clear(
    SOC_SAND_OUT SOC_TMC_TCAM_KEY_FORMAT *info
  );

void
  SOC_TMC_TCAM_KEY_clear(
    SOC_SAND_OUT SOC_TMC_TCAM_KEY *info
  );

void
  SOC_TMC_TCAM_OUTPUT_clear(
    SOC_SAND_OUT SOC_TMC_TCAM_OUTPUT *info
  );


void
  SOC_TMC_PMF_TCAM_FLD_L3_clear(
    SOC_SAND_OUT SOC_TMC_PMF_TCAM_FLD_L3 *info
  );

void
  SOC_TMC_PMF_TCAM_FLD_A_B_clear(
    SOC_SAND_OUT SOC_TMC_PMF_TCAM_FLD_A_B *info
  );
  
void
  SOC_TMC_TCAM_KEY_FLD_TYPE_clear(
    SOC_SAND_IN  SOC_TMC_TCAM_KEY_FORMAT    *key_format,
    SOC_SAND_OUT SOC_TMC_TCAM_KEY_FLD_TYPE *info
  );

void
  SOC_TMC_TCAM_KEY_FLD_VAL_clear(
    SOC_SAND_OUT SOC_TMC_TCAM_KEY_FLD_VAL *info
  );

#if SOC_TMC_DEBUG_IS_LVL1

const char*
  SOC_TMC_EGR_ACL_DB_TYPE_to_string(
    SOC_SAND_IN  SOC_TMC_EGR_ACL_DB_TYPE enum_val
  );

const char*
  SOC_TMC_PMF_TCAM_KEY_SRC_to_string(
    SOC_SAND_IN  SOC_TMC_PMF_TCAM_KEY_SRC enum_val
  );

const char*
  SOC_TMC_TCAM_KEY_FORMAT_TYPE_to_string(
    SOC_SAND_IN  SOC_TMC_TCAM_KEY_FORMAT_TYPE enum_val
  );

const char*
  SOC_TMC_PMF_TCAM_FLD_L2_to_string(
    SOC_SAND_IN  SOC_TMC_PMF_TCAM_FLD_L2 enum_val
  );

const char*
  SOC_TMC_PMF_TCAM_FLD_L3_MODE_to_string(
    SOC_SAND_IN  SOC_TMC_PMF_TCAM_FLD_L3_MODE enum_val
  );

const char*
  SOC_TMC_PMF_FLD_IPV4_to_string(
    SOC_SAND_IN  SOC_TMC_PMF_FLD_IPV4 enum_val
  );

const char*
  SOC_TMC_PMF_FLD_IPV6_to_string(
    SOC_SAND_IN  SOC_TMC_PMF_FLD_IPV6 enum_val
  );

const char*
  SOC_TMC_PMF_TCAM_FLD_A_B_LOC_to_string(
    SOC_SAND_IN  SOC_TMC_PMF_TCAM_FLD_A_B_LOC enum_val
  );

const char*
  SOC_TMC_EGR_ACL_TCAM_FLD_L2_to_string(
    SOC_SAND_IN  SOC_TMC_EGR_ACL_TCAM_FLD_L2 enum_val
  );

const char*
  SOC_TMC_EGR_ACL_TCAM_FLD_IPV4_to_string(
    SOC_SAND_IN  SOC_TMC_EGR_ACL_TCAM_FLD_IPV4 enum_val
  );

const char*
  SOC_TMC_EGR_ACL_TCAM_FLD_MPLS_to_string(
    SOC_SAND_IN  SOC_TMC_EGR_ACL_TCAM_FLD_MPLS enum_val
  );

const char*
  SOC_TMC_EGR_ACL_TCAM_FLD_TM_to_string(
    SOC_SAND_IN  SOC_TMC_EGR_ACL_TCAM_FLD_TM enum_val
  );

void
  SOC_TMC_TCAM_RULE_INFO_print(
    SOC_SAND_IN  SOC_TMC_TCAM_RULE_INFO *info
  );

void
  SOC_TMC_TCAM_KEY_FORMAT_print(
    SOC_SAND_IN  SOC_TMC_TCAM_KEY_FORMAT *info
  );

void
  SOC_TMC_TCAM_KEY_print(
    SOC_SAND_IN  SOC_TMC_TCAM_KEY *info
  );

void
  SOC_TMC_TCAM_OUTPUT_print(
    SOC_SAND_IN  SOC_TMC_TCAM_OUTPUT *info
  );

void
  SOC_TMC_PMF_TCAM_FLD_L3_print(
    SOC_SAND_IN  SOC_TMC_PMF_TCAM_FLD_L3 *info
  );

void
  SOC_TMC_PMF_TCAM_FLD_A_B_print(
    SOC_SAND_IN  SOC_TMC_PMF_TCAM_FLD_A_B *info
  );


void
  SOC_TMC_TCAM_KEY_FLD_TYPE_print(
    SOC_SAND_IN  SOC_TMC_TCAM_KEY_FLD_TYPE *info
  );

void
  SOC_TMC_TCAM_KEY_FLD_VAL_print(
    SOC_SAND_IN  SOC_TMC_TCAM_KEY_FLD_VAL *info
  );

#endif 



#include <soc/dpp/SAND/Utils/sand_footer.h>


#endif

