#ifndef BCMPKT_MPLS_CW_T_DEFS_H
#define BCMPKT_MPLS_CW_T_DEFS_H
/*****************************************************************
 *
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated by xfc_map_parser
 * from the NPL output file(s) header.yml.
 * Edits to this file will be lost when it is regenerated.
 *
 * 
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 * All Rights Reserved.$
 *
 * Tool Path: $SDK/INTERNAL/fltg/xfc_map_parser
 */

/*!
 * \name MPLS_CW_T field IDs.
 * \anchor BCMPKT_MPLS_CW_T_XXX
 */
#define BCMPKT_MPLS_CW_T_CW_TYPE    0
#define BCMPKT_MPLS_CW_T_RESERVED   1
#define BCMPKT_MPLS_CW_T_SEQ_NUMBER 2

#define BCMPKT_MPLS_CW_T_FID_INVALID        -1
#define BCMPKT_MPLS_CW_T_FID_COUNT          3
#define BCMPKT_MPLS_CW_T_FID_START          0
#define BCMPKT_MPLS_CW_T_FID_END            (BCMPKT_MPLS_CW_T_FID_COUNT - 1)

#define BCMPKT_MPLS_CW_T_I_SUPPORT          0
#define BCMPKT_MPLS_CW_T_I_FID_COUNT        1

#define BCMPKT_MPLS_CW_T_FIELD_NAME_MAP_INIT \
    {"CW_TYPE", BCMPKT_MPLS_CW_T_CW_TYPE},\
    {"RESERVED", BCMPKT_MPLS_CW_T_RESERVED},\
    {"SEQ_NUMBER", BCMPKT_MPLS_CW_T_SEQ_NUMBER},\
    {"mpls_cw_t fid count", BCMPKT_MPLS_CW_T_FID_COUNT}

#endif /* BCMPKT_MPLS_CW_T_DEFS_H */
