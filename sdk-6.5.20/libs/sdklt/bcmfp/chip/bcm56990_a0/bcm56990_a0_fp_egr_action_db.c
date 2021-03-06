/*! \file bcm56990_a0_fp_egr_action_db.c
 *
 * EFP action configuration DB initialization function for Tomahawk-4(56990_A0).
 */
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

#include <shr/shr_error.h>
#include <shr/shr_debug.h>
#include <bcmfp/bcmfp_internal.h>
#include <bcmfp/bcmfp_action_internal.h>
#include <bcmfp/chip/bcm56990_a0_fp.h>
#include <bcmltd/chip/bcmltd_id.h>

#define BSL_LOG_MODULE BSL_LS_BCMFP_DEV

/* only default and vxlan views are supported in TH4 */
int
bcmfp_bcm56990_a0_egress_action_cfg_db_init(int unit,
                                            bcmfp_stage_t *stage)
{
    uint32_t fid = 0;
    BCMFP_ACTION_CFG_DECL;
    bcmfp_action_viewtype_t viewtype;

    SHR_FUNC_ENTER(unit);

    SHR_NULL_CHECK(stage, SHR_E_PARAM);

    /* MEMORY EFP_POLICY_TABLE */

    /* DROP ACTION */
    /* def view*/
    BCMFP_ACTION_CFG_INIT(unit);
    viewtype = BCMFP_ACTION_VIEWTYPE_DEFAULT;
    BCMFP_ACTION_CFG_OFFSET_ADD(0, 0, 39, 2, 1);
    BCMFP_ACTION_CFG_OFFSET_ADD(1, 0, 0, 2, 0);
    BCMFP_ACTION_CFG_VIEWTYPE_ADD(viewtype);
    fid = FP_EGR_POLICY_TEMPLATEt_ACTION_DROPf;
    BCMFP_ACTION_CFG_INSERT(unit, stage, fid, 0);
    /*vxlan view*/
    BCMFP_ACTION_CFG_INIT(unit);
    viewtype = BCMFP_ACTION_VIEWTYPE_VXLAN;
    BCMFP_ACTION_CFG_OFFSET_ADD(0, 0, 35, 2, 1);
    BCMFP_ACTION_CFG_OFFSET_ADD(1, 0, 0, 2, 1);
    BCMFP_ACTION_CFG_VIEWTYPE_ADD(viewtype);
    fid = FP_EGR_POLICY_TEMPLATEt_ACTION_DROPf;
    BCMFP_ACTION_CFG_INSERT(unit, stage, fid, 0);


    /* DONOT DROP */
    BCMFP_ACTION_CFG_INIT(unit);
    /* def view*/
    viewtype = BCMFP_ACTION_VIEWTYPE_DEFAULT;
    BCMFP_ACTION_CFG_OFFSET_ADD(0, 0, 39, 2, 2);
    BCMFP_ACTION_CFG_OFFSET_ADD(1, 0, 0, 2, 0);
    BCMFP_ACTION_CFG_VIEWTYPE_ADD(viewtype);
    fid = FP_EGR_POLICY_TEMPLATEt_ACTION_DROP_CANCELf;
    BCMFP_ACTION_CFG_INSERT(unit, stage, fid, 0);
    /*vxlan view*/
    BCMFP_ACTION_CFG_INIT(unit);
    viewtype = BCMFP_ACTION_VIEWTYPE_VXLAN;
    BCMFP_ACTION_CFG_OFFSET_ADD(0, 0, 35, 2, 2);
    BCMFP_ACTION_CFG_OFFSET_ADD(1, 0, 0, 2, 1);
    BCMFP_ACTION_CFG_VIEWTYPE_ADD(viewtype);
    fid = FP_EGR_POLICY_TEMPLATEt_ACTION_DROP_CANCELf;
    BCMFP_ACTION_CFG_INSERT(unit, stage, fid, 0);

    /* CHANGE DSCP ACTION*/
    BCMFP_ACTION_CFG_INIT(unit);
    /* def view*/
    viewtype = BCMFP_ACTION_VIEWTYPE_DEFAULT;
    BCMFP_ACTION_CFG_OFFSET_ADD(0, 0, 36, 1, 1);
    BCMFP_ACTION_CFG_OFFSET_ADD(1, 0, 44, 6, -1);
    BCMFP_ACTION_CFG_OFFSET_ADD(2, 0, 0, 2, 0);
    BCMFP_ACTION_CFG_VIEWTYPE_ADD(viewtype);
    fid = FP_EGR_POLICY_TEMPLATEt_ACTION_NEW_DSCPf;
    BCMFP_ACTION_CFG_INSERT(unit, stage, fid, 0);
    /*vxlan view*/
    BCMFP_ACTION_CFG_INIT(unit);
    viewtype = BCMFP_ACTION_VIEWTYPE_VXLAN;
    BCMFP_ACTION_CFG_OFFSET_ADD(0, 0, 32, 1, 1);
    BCMFP_ACTION_CFG_OFFSET_ADD(1, 0, 40, 6, -1);
    BCMFP_ACTION_CFG_OFFSET_ADD(2, 0, 0, 2, 1);
    BCMFP_ACTION_CFG_VIEWTYPE_ADD(viewtype);
    fid = FP_EGR_POLICY_TEMPLATEt_ACTION_NEW_DSCPf;
    BCMFP_ACTION_CFG_INSERT(unit, stage, fid, 0);

    /* DSCP MASK*/
    BCMFP_ACTION_CFG_INIT(unit);
    /* def view*/
    viewtype = BCMFP_ACTION_VIEWTYPE_DEFAULT;
    BCMFP_ACTION_CFG_OFFSET_ADD(0, 0, 50, 6, -1);
    BCMFP_ACTION_CFG_OFFSET_ADD(1, 0, 0, 2, 0);
    BCMFP_ACTION_CFG_VIEWTYPE_ADD(viewtype);
    fid = FP_EGR_POLICY_TEMPLATEt_ACTION_DSCP_MASKf;
    BCMFP_ACTION_CFG_INSERT(unit, stage, fid, 0);
    /*vxlan view*/
    BCMFP_ACTION_CFG_INIT(unit);
    viewtype = BCMFP_ACTION_VIEWTYPE_VXLAN;
    BCMFP_ACTION_CFG_OFFSET_ADD(0, 0, 46, 6, -1);
    BCMFP_ACTION_CFG_OFFSET_ADD(1, 0, 0, 2, 1);
    BCMFP_ACTION_CFG_VIEWTYPE_ADD(viewtype);
    fid = FP_EGR_POLICY_TEMPLATEt_ACTION_DSCP_MASKf;
    BCMFP_ACTION_CFG_INSERT(unit, stage, fid, 0);


    /* CHANGE ECN ACTION */
     BCMFP_ACTION_CFG_INIT(unit);
    /* def view*/
    viewtype = BCMFP_ACTION_VIEWTYPE_DEFAULT;
    BCMFP_ACTION_CFG_OFFSET_ADD(0, 0, 37, 1, 1);
    BCMFP_ACTION_CFG_OFFSET_ADD(1, 0, 56, 2, -1);
    BCMFP_ACTION_CFG_OFFSET_ADD(2, 0, 0, 2, 0);
    BCMFP_ACTION_CFG_VIEWTYPE_ADD(viewtype);
    fid = FP_EGR_POLICY_TEMPLATEt_ACTION_NEW_ECNf;
    BCMFP_ACTION_CFG_INSERT(unit, stage, fid, 0);
    /*vxlan view*/
     BCMFP_ACTION_CFG_INIT(unit);
    viewtype = BCMFP_ACTION_VIEWTYPE_VXLAN;
    BCMFP_ACTION_CFG_OFFSET_ADD(0, 0, 33, 1, 1);
    BCMFP_ACTION_CFG_OFFSET_ADD(1, 0, 52, 2, -1);
    BCMFP_ACTION_CFG_OFFSET_ADD(2, 0, 0, 2, 1);
    BCMFP_ACTION_CFG_VIEWTYPE_ADD(viewtype);
    fid = FP_EGR_POLICY_TEMPLATEt_ACTION_NEW_ECNf;
    BCMFP_ACTION_CFG_INSERT(unit, stage, fid, 0);


    /* ECN MASK */
    BCMFP_ACTION_CFG_INIT(unit);
    /* def view*/
    viewtype = BCMFP_ACTION_VIEWTYPE_DEFAULT;
    BCMFP_ACTION_CFG_OFFSET_ADD(0, 0, 58, 2, -1);
    BCMFP_ACTION_CFG_OFFSET_ADD(1, 0, 0, 2, 0);
    BCMFP_ACTION_CFG_VIEWTYPE_ADD(viewtype);
    fid = FP_EGR_POLICY_TEMPLATEt_ACTION_ECN_MASKf;
    BCMFP_ACTION_CFG_INSERT(unit, stage, fid, 0);
    /*vxlan view*/
    BCMFP_ACTION_CFG_INIT(unit);
    viewtype = BCMFP_ACTION_VIEWTYPE_VXLAN;
    BCMFP_ACTION_CFG_OFFSET_ADD(0, 0, 54, 2, -1);
    BCMFP_ACTION_CFG_OFFSET_ADD(1, 0, 0, 2, 1);
    BCMFP_ACTION_CFG_VIEWTYPE_ADD(viewtype);
    fid = FP_EGR_POLICY_TEMPLATEt_ACTION_ECN_MASKf;
    BCMFP_ACTION_CFG_INSERT(unit, stage, fid, 0);

    /* CHANGE DOT1P ACTION*/
     BCMFP_ACTION_CFG_INIT(unit);
    /* def view*/
    viewtype = BCMFP_ACTION_VIEWTYPE_DEFAULT;
    BCMFP_ACTION_CFG_OFFSET_ADD(0, 0, 35, 1, 1);
    BCMFP_ACTION_CFG_OFFSET_ADD(1, 0, 41, 3, -1);
    BCMFP_ACTION_CFG_OFFSET_ADD(2, 0, 0, 2, 0);
    BCMFP_ACTION_CFG_VIEWTYPE_ADD(viewtype);
    fid = FP_EGR_POLICY_TEMPLATEt_ACTION_NEW_OUTER_DOT1Pf;
    BCMFP_ACTION_CFG_INSERT(unit, stage, fid, 0);
    /*vxlan view*/
     BCMFP_ACTION_CFG_INIT(unit);
    viewtype = BCMFP_ACTION_VIEWTYPE_VXLAN;
    BCMFP_ACTION_CFG_OFFSET_ADD(0, 0, 31, 1, 1);
    BCMFP_ACTION_CFG_OFFSET_ADD(1, 0, 37, 3, -1);
    BCMFP_ACTION_CFG_OFFSET_ADD(2, 0, 0, 2, 1);
    BCMFP_ACTION_CFG_VIEWTYPE_ADD(viewtype);
    fid = FP_EGR_POLICY_TEMPLATEt_ACTION_NEW_OUTER_DOT1Pf;
    BCMFP_ACTION_CFG_INSERT(unit, stage, fid, 0);

    /* CHANGE CFI ACTION */
     BCMFP_ACTION_CFG_INIT(unit);
    /* def view*/
    viewtype = BCMFP_ACTION_VIEWTYPE_DEFAULT;
    BCMFP_ACTION_CFG_OFFSET_ADD(0, 0, 38, 1, 1);
    BCMFP_ACTION_CFG_OFFSET_ADD(1, 0, 60, 1, -1);
    BCMFP_ACTION_CFG_OFFSET_ADD(2, 0, 0, 2, 0);
    BCMFP_ACTION_CFG_VIEWTYPE_ADD(viewtype);
    fid = FP_EGR_POLICY_TEMPLATEt_ACTION_NEW_OUTER_CFIf;
    BCMFP_ACTION_CFG_INSERT(unit, stage, fid, 0);
    /*vxlan view*/
     BCMFP_ACTION_CFG_INIT(unit);
    viewtype = BCMFP_ACTION_VIEWTYPE_VXLAN;
    BCMFP_ACTION_CFG_OFFSET_ADD(0, 0, 34, 1, 1);
    BCMFP_ACTION_CFG_OFFSET_ADD(1, 0, 56, 1, -1);
    BCMFP_ACTION_CFG_OFFSET_ADD(2, 0, 0, 2, 1);
    BCMFP_ACTION_CFG_VIEWTYPE_ADD(viewtype);
    fid = FP_EGR_POLICY_TEMPLATEt_ACTION_NEW_OUTER_CFIf;
    BCMFP_ACTION_CFG_INSERT(unit, stage, fid, 0);

    /* flex ctr action*/
    BCMFP_ACTION_CFG_INIT(unit);
    /* def view*/
    viewtype = BCMFP_ACTION_VIEWTYPE_DEFAULT;
    BCMFP_ACTION_CFG_OFFSET_ADD(0, 0, 6, 9, -1);
    BCMFP_ACTION_CFG_OFFSET_ADD(1, 0, 0, 2, 0);
    BCMFP_ACTION_CFG_VIEWTYPE_ADD(viewtype);
    fid = FP_EGR_POLICY_TEMPLATEt_CTR_EGR_EFLEX_OBJECTf;
    BCMFP_ACTION_CFG_INSERT(unit, stage, fid, 0);
    /* vxlan view */
    BCMFP_ACTION_CFG_INIT(unit);
    viewtype = BCMFP_ACTION_VIEWTYPE_VXLAN;
    BCMFP_ACTION_CFG_OFFSET_ADD(0, 0, 6, 9, -1);
    BCMFP_ACTION_CFG_OFFSET_ADD(1, 0, 0, 2, 1);
    BCMFP_ACTION_CFG_VIEWTYPE_ADD(viewtype);
    fid = FP_EGR_POLICY_TEMPLATEt_CTR_EGR_EFLEX_OBJECTf;
    BCMFP_ACTION_CFG_INSERT(unit, stage, fid, 0);


    /* CHNAGE OUTER VLAN*/
     BCMFP_ACTION_CFG_INIT(unit);
    /* def view*/
    viewtype = BCMFP_ACTION_VIEWTYPE_DEFAULT;
    BCMFP_ACTION_CFG_OFFSET_ADD(0, 0, 106, 1, 1);
    BCMFP_ACTION_CFG_OFFSET_ADD(1, 0, 91, 12, -1);
    BCMFP_ACTION_CFG_OFFSET_ADD(2, 0, 0, 2, 0);
    BCMFP_ACTION_CFG_VIEWTYPE_ADD(viewtype);
    fid = FP_EGR_POLICY_TEMPLATEt_ACTION_NEW_OUTER_VLANIDf;
    BCMFP_ACTION_CFG_INSERT(unit, stage, fid, 0);
    /*vxlan view*/
     BCMFP_ACTION_CFG_INIT(unit);
    viewtype = BCMFP_ACTION_VIEWTYPE_VXLAN;
    BCMFP_ACTION_CFG_OFFSET_ADD(0, 0, 102, 1, 1);
    BCMFP_ACTION_CFG_OFFSET_ADD(1, 0, 87, 12, -1);
    BCMFP_ACTION_CFG_OFFSET_ADD(2, 0, 0, 2, 1);
    BCMFP_ACTION_CFG_VIEWTYPE_ADD(viewtype);
    fid = FP_EGR_POLICY_TEMPLATEt_ACTION_NEW_OUTER_VLANIDf;
    BCMFP_ACTION_CFG_INSERT(unit, stage, fid, 0);

    /* CHANGE OUTER TPID */
     BCMFP_ACTION_CFG_INIT(unit);
    /* def view*/
    viewtype = BCMFP_ACTION_VIEWTYPE_DEFAULT;
    BCMFP_ACTION_CFG_OFFSET_ADD(0, 0, 105, 1, 1);
    BCMFP_ACTION_CFG_OFFSET_ADD(1, 0, 103, 2, -1);
    BCMFP_ACTION_CFG_OFFSET_ADD(2, 0, 0, 2, 0);
    BCMFP_ACTION_CFG_VIEWTYPE_ADD(viewtype);
    fid = FP_EGR_POLICY_TEMPLATEt_ACTION_NEW_OUTER_TPIDf;
    BCMFP_ACTION_CFG_INSERT(unit, stage, fid, 0);
    /*vxlan view*/
     BCMFP_ACTION_CFG_INIT(unit);
    viewtype = BCMFP_ACTION_VIEWTYPE_VXLAN;
    BCMFP_ACTION_CFG_OFFSET_ADD(0, 0, 101, 1, 1);
    BCMFP_ACTION_CFG_OFFSET_ADD(1, 0, 99, 2, -1);
    BCMFP_ACTION_CFG_OFFSET_ADD(2, 0, 0, 2, 1);
    BCMFP_ACTION_CFG_VIEWTYPE_ADD(viewtype);
    fid = FP_EGR_POLICY_TEMPLATEt_ACTION_NEW_OUTER_TPIDf;
    BCMFP_ACTION_CFG_INSERT(unit, stage, fid, 0);

    /* MPLS EXP */
     BCMFP_ACTION_CFG_INIT(unit);
    /* def view*/
    viewtype = BCMFP_ACTION_VIEWTYPE_DEFAULT;
    BCMFP_ACTION_CFG_OFFSET_ADD(0, 0, 31, 1, 1);
    BCMFP_ACTION_CFG_OFFSET_ADD(1, 0, 32, 3, -1);
    BCMFP_ACTION_CFG_OFFSET_ADD(2, 0, 0, 2, 0);
    BCMFP_ACTION_CFG_VIEWTYPE_ADD(viewtype);
    fid = FP_EGR_POLICY_TEMPLATEt_ACTION_NEW_MPLS_EXPf;
    BCMFP_ACTION_CFG_INSERT(unit, stage, fid, 0);

    /* LOOPBACK PKT PROFILE CHANGE */
    BCMFP_ACTION_CFG_INIT(unit);
    /* def view*/
    viewtype = BCMFP_ACTION_VIEWTYPE_DEFAULT;
    BCMFP_ACTION_CFG_OFFSET_ADD(0, 0, 61, 1, 1);
    BCMFP_ACTION_CFG_OFFSET_ADD(1, 0, 62, 3, -1);
    BCMFP_ACTION_CFG_OFFSET_ADD(2, 0, 0, 2, 0);
    BCMFP_ACTION_CFG_VIEWTYPE_ADD(viewtype);
    fid = FP_EGR_POLICY_TEMPLATEt_ACTION_NEW_LOOPBACK_PACKET_PROFILEf;
    BCMFP_ACTION_CFG_INSERT(unit, stage, fid, 0);
    /*vxlan view*/
    BCMFP_ACTION_CFG_INIT(unit);
    viewtype = BCMFP_ACTION_VIEWTYPE_VXLAN;
    BCMFP_ACTION_CFG_OFFSET_ADD(0, 0, 57, 1, 1);
    BCMFP_ACTION_CFG_OFFSET_ADD(1, 0, 58, 3, -1);
    BCMFP_ACTION_CFG_OFFSET_ADD(2, 0, 0, 2, 1);
    BCMFP_ACTION_CFG_VIEWTYPE_ADD(viewtype);
    fid = FP_EGR_POLICY_TEMPLATEt_ACTION_NEW_LOOPBACK_PACKET_PROFILEf;
    BCMFP_ACTION_CFG_INSERT(unit, stage, fid, 0);

    /* LOOPBACK PP PORT CHANGE */
    BCMFP_ACTION_CFG_INIT(unit);
    /* def view*/
    viewtype = BCMFP_ACTION_VIEWTYPE_DEFAULT;
    BCMFP_ACTION_CFG_OFFSET_ADD(0, 0, 65, 1, 1);
    BCMFP_ACTION_CFG_OFFSET_ADD(1, 0, 66, 9, -1);
    BCMFP_ACTION_CFG_OFFSET_ADD(2, 0, 0, 2, 0);
    BCMFP_ACTION_CFG_VIEWTYPE_ADD(viewtype);
    /*vxlan view*/
    BCMFP_ACTION_CFG_INIT(unit);
    viewtype = BCMFP_ACTION_VIEWTYPE_VXLAN;
    BCMFP_ACTION_CFG_OFFSET_ADD(0, 0, 61, 1, 1);
    BCMFP_ACTION_CFG_OFFSET_ADD(1, 0, 62, 9, -1);
    BCMFP_ACTION_CFG_VIEWTYPE_ADD(viewtype);
    fid = FP_EGR_POLICY_TEMPLATEt_ACTION_NEW_LOOPBACK_PP_PORTf;
    BCMFP_ACTION_CFG_INSERT(unit, stage, fid, 0);

    /* LOOPBACK SRC PORT CHANGE */
    BCMFP_ACTION_CFG_INIT(unit);
    /* def view*/
    viewtype = BCMFP_ACTION_VIEWTYPE_DEFAULT;
    BCMFP_ACTION_CFG_OFFSET_ADD(0, 0, 75, 1, 1);
    BCMFP_ACTION_CFG_OFFSET_ADD(1, 0, 76, 9, -1);
    BCMFP_ACTION_CFG_OFFSET_ADD(2, 0, 0, 2, 0);
    BCMFP_ACTION_CFG_VIEWTYPE_ADD(viewtype);
    fid = FP_EGR_POLICY_TEMPLATEt_ACTION_NEW_LOOPBACK_SRC_PORTf;
    BCMFP_ACTION_CFG_INSERT(unit, stage, fid, 0);
    /*vxlan view*/
    BCMFP_ACTION_CFG_INIT(unit);
    viewtype = BCMFP_ACTION_VIEWTYPE_VXLAN;
    BCMFP_ACTION_CFG_OFFSET_ADD(0, 0, 71, 1, 1);
    BCMFP_ACTION_CFG_OFFSET_ADD(1, 0, 72, 9, -1);
    BCMFP_ACTION_CFG_OFFSET_ADD(2, 0, 0, 2, 1);
    BCMFP_ACTION_CFG_VIEWTYPE_ADD(viewtype);
    fid = FP_EGR_POLICY_TEMPLATEt_ACTION_NEW_LOOPBACK_SRC_PORTf;
    BCMFP_ACTION_CFG_INSERT(unit, stage, fid, 0);

    /* LOOPBACK TYPE CHANGE */
    BCMFP_ACTION_CFG_INIT(unit);
    /* def view*/
    viewtype = BCMFP_ACTION_VIEWTYPE_DEFAULT;
    BCMFP_ACTION_CFG_OFFSET_ADD(0, 0, 85, 1, 1);
    BCMFP_ACTION_CFG_OFFSET_ADD(1, 0, 86, 5, -1);
    BCMFP_ACTION_CFG_OFFSET_ADD(2, 0, 0, 2, 0);
    BCMFP_ACTION_CFG_VIEWTYPE_ADD(viewtype);
    fid = FP_EGR_POLICY_TEMPLATEt_ACTION_NEW_LOOPBACK_TYPEf;
    BCMFP_ACTION_CFG_INSERT(unit, stage, fid, 0);
    /*vxlan view*/
    BCMFP_ACTION_CFG_INIT(unit);
    viewtype = BCMFP_ACTION_VIEWTYPE_VXLAN;
    BCMFP_ACTION_CFG_OFFSET_ADD(0, 0, 81, 1, 1);
    BCMFP_ACTION_CFG_OFFSET_ADD(1, 0, 82, 5, -1);
    BCMFP_ACTION_CFG_OFFSET_ADD(2, 0, 0, 2, 1);
    BCMFP_ACTION_CFG_VIEWTYPE_ADD(viewtype);
    fid = FP_EGR_POLICY_TEMPLATEt_ACTION_NEW_LOOPBACK_TYPEf;
    BCMFP_ACTION_CFG_INSERT(unit, stage, fid, 0);

    /* VXLAN SPECIFIC ACTIONS */
    BCMFP_ACTION_CFG_INIT(unit);
    viewtype = BCMFP_ACTION_VIEWTYPE_VXLAN;
    BCMFP_ACTION_CFG_OFFSET_ADD(0, 0, 143, 1, 1);
    BCMFP_ACTION_CFG_OFFSET_ADD(1, 0, 103, 8, -1);
    BCMFP_ACTION_CFG_OFFSET_ADD(2, 0, 0, 2, 1);
    BCMFP_ACTION_CFG_VIEWTYPE_ADD(viewtype);
    fid = FP_EGR_POLICY_TEMPLATEt_ACTION_SET_VXLAN_FLAGSf;
    BCMFP_ACTION_CFG_INSERT(unit, stage, fid, 0);

exit:
    SHR_FUNC_EXIT();
}
