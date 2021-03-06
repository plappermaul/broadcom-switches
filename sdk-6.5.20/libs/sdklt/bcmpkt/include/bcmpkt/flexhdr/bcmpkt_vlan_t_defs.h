#ifndef BCMPKT_VLAN_T_DEFS_H
#define BCMPKT_VLAN_T_DEFS_H
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
 * \name VLAN_T field IDs.
 * \anchor BCMPKT_VLAN_T_XXX
 */
#define BCMPKT_VLAN_T_VID  0
#define BCMPKT_VLAN_T_TPID 1
#define BCMPKT_VLAN_T_PCP  2
#define BCMPKT_VLAN_T_CFI  3

#define BCMPKT_VLAN_T_FID_INVALID        -1
#define BCMPKT_VLAN_T_FID_COUNT          4
#define BCMPKT_VLAN_T_FID_START          0
#define BCMPKT_VLAN_T_FID_END            (BCMPKT_VLAN_T_FID_COUNT - 1)

#define BCMPKT_VLAN_T_I_SUPPORT          0
#define BCMPKT_VLAN_T_I_FID_COUNT        1

#define BCMPKT_VLAN_T_FIELD_NAME_MAP_INIT \
    {"VID", BCMPKT_VLAN_T_VID},\
    {"TPID", BCMPKT_VLAN_T_TPID},\
    {"PCP", BCMPKT_VLAN_T_PCP},\
    {"CFI", BCMPKT_VLAN_T_CFI},\
    {"vlan_t fid count", BCMPKT_VLAN_T_FID_COUNT}

#endif /* BCMPKT_VLAN_T_DEFS_H */
