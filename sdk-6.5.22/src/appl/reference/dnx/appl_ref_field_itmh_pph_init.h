
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2021 Broadcom Inc. All rights reserved.
 */

#ifndef APPL_REF_FIELD_ITMH_PPH_INIT_H_INCLUDED

#define APPL_REF_FIELD_ITMH_PPH_INIT_H_INCLUDED

#include <bcm/types.h>
#include <bcm/error.h>
#include <shared/error.h>

#define ITMH_PPH_NOF_IPMF2_ACTIONS           13
#define ITMH_PPH_NOF_IPMF2_FG2_ACTIONS       8
#define ITMH_PPH_NOF_IPMF2_FG3_ACTIONS       1
#define ITMH_PPH_NOF_IPMF2_FG4_ACTIONS       1
#define ITMH_PPH_NOF_IPMF3_ACTIONS           6
#define ITMH_PPH_NOF_IPMF2_FG5_ACTIONS       6

#define ITMH_PPH_NOF_IPMF2_QUALIFIERS        13
#define ITMH_PPH_NOF_IPMF2_FG2_QUALIFIERS    12
#define ITMH_PPH_NOF_IPMF2_FG3_QUALIFIERS    1
#define ITMH_PPH_NOF_IPMF2_FG4_QUALIFIERS    5
#define ITMH_PPH_NOF_IPMF3_QUALIFIERS        1
#define ITMH_PPH_NOF_IPMF2_FG5_QUALIFIERS    3

#define ITMH_PPH_FHEI_NOF_CMP0_FIRST_KEY_QUALIFIERS      2
#define ITMH_PPH_FHEI_NOF_CMP0_SECOND_KEY_QUALIFIERS     1
#define ITMH_PPH_FHEI_NOF_CMP1_FIRST_KEY_QUALIFIERS      2
#define ITMH_PPH_FHEI_NOF_CMP1_SECOND_KEY_QUALIFIERS     1
#define ITMH_PPH_NOF_CMP0_FIRST_KEY_QUALIFIERS      1
#define ITMH_PPH_NOF_CMP0_SECOND_KEY_QUALIFIERS     1

#define ITMH_PPH_NOF_IPMF2_FEMS              3

#define ITMH_PPH_NOF_FG                      6

#define ITMH_PPH_NOF_CMP_KEY                 1
#define ITMH_PPH_FHEI_NOF_CMP_KEY            2

#define HEADER_0_SIZE        40

#define HEADER_1_SIZE        24

#define HEADER_2_SIZE        48

#define HEADER_3_SIZE        32

#define HEADER_4_SIZE        96

#define PPH_CONTEXT 0x1
#define PPH_FHEI_CONTEXT 0x2
#define PPH_FHEI_VLAN_CONTEXT 0x4

#define TC_SNP_DST_DP_MIR_PPH_QUAL_OFFSET  5

#define EXT_PRESENT_QUAL_OFFSET  39

#define OUTLIF_QUAL_OFFSET    (HEADER_0_SIZE + 2)

#define ASE_INJ_QUAL_OFFSET           4

#define TSH_INJ_QUAL_OFFSET           5

#define TIME_STAMP_QUAL_OFFSET        -32

#define TRAP_CODE_ZERO_QUAL_OFFSET    0

#define FWD_STR_RES_BIT_QUAL_OFFSET   0
#define FWD_STR_QUAL_OFFSET           95
#define TRAP_CODE_FHEI_QUAL_OFFSET    (HEADER_4_SIZE + 27)
#define FWD_QUALIFIER_QUAL_OFFSET     (HEADER_4_SIZE + 8)

#define OAM_MEP_QUAL_OFFSET           4

#define OAM_SUB_TYPE_QUAL_OFFSET      0

#define OAM_OFFSET_QUAL_OFFSET        39

#define OAM_STAMP_OFFSET_QUAL_OFFSET  0

#define PARSING_OFFSET_QUAL_OFFSET    83

#define PARSING_TYPE_QUAL_OFFSET      90

#define FHEI_SIZE_QUAL_OFFSET         78

#define LIF_EXT_QUAL_OFFSET           80

#define FHEI_TRAP_TYPE_QUAL_OFFSET    (HEADER_4_SIZE + 36)

#define FHEI_VLAN_TYPE_QUAL_OFFSET    (HEADER_4_SIZE + 39)

#define FHEI_BRIDGE_5B_INNER_VLAN_QUAL_OFFSET      (HEADER_4_SIZE + 0)

#define FHEI_BRIDGE_5B_VLAN_ACT_OUT_VLAN_QUAL_OFFSET     (HEADER_4_SIZE + 16)

#define PARS_OFFSET_TYPE_SH_ADJUST_FWD_LAYER_EGR_PRS_IDX_QUAL_OFFSET         0

#define TTL_SET_QUAL_OFFSET      69

#define IN_LIF_PROFILE_QUAL_OFFSET      13

#define FWD_ADDITIONAL_INFO_QUAL_OFFSET      6

#define NWK_QOS_QUAL_OFFSET      61

#define END_OF_PKT_EDIT_QUAL_OFFSET      10

#define IN_LIF_0_QUAL_OFFSET      39

#define FWD_DOMAIN_QUAL_OFFSET      21

#define TC_SNP_DST_DP_MIR_PPH_QUAL_LENGTH  34

#define EXT_PRESENT_QUAL_LENGTH  1

#define OUTLIF_QUAL_LENGTH    22

#define ASE_INJ_QUAL_LENGTH           1

#define TSH_INJ_QUAL_LENGTH           1

#define TIME_STAMP_QUAL_LENGTH        32

#define TRAP_CODE_ZERO_QUAL_LENGTH    9

#define FWD_STR_RES_BIT_QUAL_LENGTH   3
#define FWD_STR_QUAL_LENGTH           1
#define FWD_QUALIFIER_QUAL_LENGTH     19
#define TRAP_CODE_FHEI_QUAL_LENGTH    9

#define OAM_MEP_QUAL_LENGTH           1

#define OAM_SUB_TYPE_QUAL_LENGTH      4

#define OAM_OFFSET_QUAL_LENGTH        8

#define OAM_STAMP_OFFSET_QUAL_LENGTH   8

#define PARSING_OFFSET_VALID_QUAL_LENGTH    1

#define PARSING_OFFSET_QUAL_LENGTH    7

#define PARSING_TYPE_VALID_QUAL_LENGTH    1

#define PARSING_TYPE_QUAL_LENGTH      5

#define PACKET_STRIP_QUAL_LENGTH      9

#define TTL_SET_QUAL_LENGTH     8

#define IN_LIF_PROFILE_QUAL_LENGTH     8

#define FWD_ADDITIONAL_INFO_QUAL_LENGTH    4

#define NWK_QOS_QUAL_LENGTH    8

#define END_OF_PKT_EDIT_QUAL_LENGTH    3

#define IN_LIF_0_QUAL_LENGTH    22

#define FWD_DOMAIN_QUAL_LENGTH    18

#define SYS_HDR_ADJUST_SIZE_FWD_LAYER_INDEX_EGR_PRS_IDX_QUAL_LENGTH    7

#define FHEI_SIZE_QUAL_LENGTH      2

#define FHEI_TRAP_TYPE_QUAL_LENGTH      4

#define FHEI_VLAN_TYPE_QUAL_LENGTH      1

#define LIF_EXT_TYPE_QUAL_LENGTH        3

#define FHEI_BRIDGE_5B_INNER_VLAN_QUAL_LENGTH      16

#define FHEI_BRIDGE_5B_VLAN_ACT_OUT_VLAN_QUAL_LENGTH     23

#define SNOOP_DATA_ACTION_LENGTH         5

#define DST_ACTION_LENGTH                21

#define EXT_EXIST_ACTION_LENGTH          1

#define OUT_LIF_ACTION_LENGTH            22

#define ASE_INJ_ACTION_LENGTH            1

#define TSH_INJ_ACTION_LENGTH            1

#define MIRROR_ACTION_LENGTH             1

#define FWD_STR_ACTION_LENGTH           13

#define OAM_DATA_ACTION_LENGTH          13

#define PARS_START_SH_ADJUST_FWD_LAYER_EGR_PRS_IDX_ACTION_LENGTH     21

#define IPMF3_ZERO_ACTION_LENGTH                11

#define SYS_HDR_ADJUST_SIZE_ACTION_LENGTH       1

#define PARSING_START_OFFSET_ACTION_LENGTH       8

#define FHEI_CMP0_SECOND_KEY_GEN_OFFSET     (SYS_HDR_ADJUST_SIZE_FWD_LAYER_INDEX_EGR_PRS_IDX_QUAL_LENGTH)
#define FHEI_CMP1_SECOND_KEY_GEN_OFFSET     (SYS_HDR_ADJUST_SIZE_FWD_LAYER_INDEX_EGR_PRS_IDX_QUAL_LENGTH + FHEI_TRAP_TYPE_QUAL_LENGTH + LIF_EXT_TYPE_QUAL_LENGTH)
#define PPH_CMP0_SECOND_KEY_GEN_OFFSET      (SYS_HDR_ADJUST_SIZE_FWD_LAYER_INDEX_EGR_PRS_IDX_QUAL_LENGTH)

#define FHEI_CMP0_SECOND_KEY_GEN_LENGTH     (FHEI_TRAP_TYPE_QUAL_LENGTH + LIF_EXT_TYPE_QUAL_LENGTH)
#define FHEI_CMP1_SECOND_KEY_GEN_LENGTH     (FHEI_VLAN_TYPE_QUAL_LENGTH + LIF_EXT_TYPE_QUAL_LENGTH)
#define PPH_CMP0_SECOND_KEY_GEN_LENGTH     (LIF_EXT_TYPE_QUAL_LENGTH)

#define ITMH_PPH_APPL_ITMH_LAYER    0
#define ITMH_PPH_APPL_ASE_TSH_LAYER 1
#define ITMH_PPH_APPL_PPH_LAYER     2

typedef struct
{
    char *name;
    bcm_field_qualify_t qual_id;
    uint32 qual_length;
    bcm_field_qualify_attach_info_t qual_attach_info;
} field_itmh_pph_qual_info_t;

typedef struct
{
    char *name;
    bcm_field_action_t action_id;
    bcm_field_action_info_t action_info;
    uint8 action_with_valid_bit;
    bcm_field_action_priority_t action_priority;
} field_itmh_pph_action_info_t;

typedef struct
{
    int nof_conditions;
    int condition_ids[16];
    int nof_extractions;
    int nof_mapping_bits;
    bcm_field_fem_action_info_t fem_action_info;
} field_itmh_pph_fem_action_info_t;

typedef struct
{
    char *name;
    bcm_field_group_t fg_id;
    bcm_field_stage_t stage;
    bcm_field_group_type_t fg_type;
    int nof_quals;
    int nof_actions;
    field_itmh_pph_qual_info_t *itmh_pph_qual_info;
    field_itmh_pph_action_info_t *itmh_pph_action_info;
    field_itmh_pph_fem_action_info_t *itmh_pph_fem_action_info;
    int context_ids;
} field_itmh_pph_fg_info_t;

typedef struct
{
    int pair_id;
    int nof_first_key_quals;
    int nof_second_key_quals;
    field_itmh_pph_qual_info_t *itmh_pph_first_key_qual_info;
    field_itmh_pph_qual_info_t *itmh_pph_second_key_qual_info;
} field_itmh_pph_cmp_key_info_t;

shr_error_e appl_dnx_field_itmh_pph_init(
    int unit);

#endif
