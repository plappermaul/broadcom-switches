/* $Id: arad_pp_framework.h,v 1.16 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
*/

#ifndef __ARAD_PP_FRAMEWORK_INCLUDED__
/* { */
#define __ARAD_PP_FRAMEWORK_INCLUDED__

/*************
 * INCLUDES  *
 *************/
/* { */

#include <soc/dpp/SAND/Utils/sand_header.h>

#include <soc/dpp/SAND/Management/sand_general_macros.h>
#include <soc/dpp/SAND/Management/sand_error_code.h>

#include <soc/dpp/SAND/SAND_FM/sand_pp_general.h>

/* } */
/*************
 * DEFINES   *
 *************/
/* { */

/************************************************************************/
/* Procedure Descriptors                                                */
/************************************************************************/
#define ARAD_PP_PROC_DESC_BASE_GENERAL_FIRST                (ARAD_PP_PROC_BITS)
#define ARAD_PP_PROC_DESC_BASE_GENERAL_LAST                 (ARAD_PP_PROC_DESC_BASE_GENERAL_FIRST + 30)
#define ARAD_PP_PROC_DESC_BASE_LLP_PARSE_FIRST              (ARAD_PP_PROC_DESC_BASE_GENERAL_LAST)
#define ARAD_PP_PROC_DESC_BASE_LLP_PARSE_LAST               (ARAD_PP_PROC_DESC_BASE_LLP_PARSE_FIRST + 44)
#define ARAD_PP_PROC_DESC_BASE_LLP_SA_AUTH_FIRST            (ARAD_PP_PROC_DESC_BASE_LLP_PARSE_LAST)
#define ARAD_PP_PROC_DESC_BASE_LLP_SA_AUTH_LAST             (ARAD_PP_PROC_DESC_BASE_LLP_SA_AUTH_FIRST + 28)
#define ARAD_PP_PROC_DESC_BASE_LLP_VID_ASSIGN_FIRST         (ARAD_PP_PROC_DESC_BASE_LLP_SA_AUTH_LAST)
#define ARAD_PP_PROC_DESC_BASE_LLP_VID_ASSIGN_LAST          (ARAD_PP_PROC_DESC_BASE_LLP_VID_ASSIGN_FIRST + 44)
#define ARAD_PP_PROC_DESC_BASE_LLP_FILTER_FIRST             (ARAD_PP_PROC_DESC_BASE_LLP_VID_ASSIGN_LAST)
#define ARAD_PP_PROC_DESC_BASE_LLP_FILTER_LAST              (ARAD_PP_PROC_DESC_BASE_LLP_FILTER_FIRST + 44)
#define ARAD_PP_PROC_DESC_BASE_LLP_COS_FIRST                (ARAD_PP_PROC_DESC_BASE_LLP_FILTER_LAST)
#define ARAD_PP_PROC_DESC_BASE_LLP_COS_LAST                 (ARAD_PP_PROC_DESC_BASE_LLP_COS_FIRST + 68)
#define ARAD_PP_PROC_DESC_BASE_LLP_TRAP_FIRST               (ARAD_PP_PROC_DESC_BASE_LLP_COS_LAST)
#define ARAD_PP_PROC_DESC_BASE_LLP_TRAP_LAST                (ARAD_PP_PROC_DESC_BASE_LLP_TRAP_FIRST + 44)
#define ARAD_PP_PROC_DESC_BASE_LLP_MIRROR_FIRST             (ARAD_PP_PROC_DESC_BASE_LLP_TRAP_LAST)
#define ARAD_PP_PROC_DESC_BASE_LLP_MIRROR_LAST              (ARAD_PP_PROC_DESC_BASE_LLP_MIRROR_FIRST + 32)
#define ARAD_PP_PROC_DESC_BASE_MYMAC_FIRST                  (ARAD_PP_PROC_DESC_BASE_LLP_MIRROR_LAST)
#define ARAD_PP_PROC_DESC_BASE_MYMAC_LAST                   (ARAD_PP_PROC_DESC_BASE_MYMAC_FIRST + 68)
#define ARAD_PP_PROC_DESC_BASE_MPLS_TERM_FIRST              (ARAD_PP_PROC_DESC_BASE_MYMAC_LAST)
#define ARAD_PP_PROC_DESC_BASE_MPLS_TERM_LAST               (ARAD_PP_PROC_DESC_BASE_MPLS_TERM_FIRST + 64)
#define ARAD_PP_PROC_DESC_BASE_VSI_FIRST                    (ARAD_PP_PROC_DESC_BASE_MPLS_TERM_LAST)
#define ARAD_PP_PROC_DESC_BASE_VSI_LAST                     (ARAD_PP_PROC_DESC_BASE_VSI_FIRST + 36)
#define ARAD_PP_PROC_DESC_BASE_LIF_FIRST                    (ARAD_PP_PROC_DESC_BASE_VSI_LAST)
#define ARAD_PP_PROC_DESC_BASE_LIF_LAST                     (ARAD_PP_PROC_DESC_BASE_LIF_FIRST + 105)
#define ARAD_PP_PROC_DESC_BASE_RIF_FIRST                    (ARAD_PP_PROC_DESC_BASE_LIF_LAST)
#define ARAD_PP_PROC_DESC_BASE_RIF_LAST                     (ARAD_PP_PROC_DESC_BASE_RIF_FIRST + 70)
#define ARAD_PP_PROC_DESC_BASE_LIF_ING_VLAN_EDIT_FIRST      (ARAD_PP_PROC_DESC_BASE_RIF_LAST)
#define ARAD_PP_PROC_DESC_BASE_LIF_ING_VLAN_EDIT_LAST       (ARAD_PP_PROC_DESC_BASE_LIF_ING_VLAN_EDIT_FIRST + 52)
#define ARAD_PP_PROC_DESC_BASE_LIF_COS_FIRST                (ARAD_PP_PROC_DESC_BASE_LIF_ING_VLAN_EDIT_LAST)
#define ARAD_PP_PROC_DESC_BASE_LIF_COS_LAST                 (ARAD_PP_PROC_DESC_BASE_LIF_COS_FIRST + 108)
#define ARAD_PP_PROC_DESC_BASE_LIF_TABLE_FIRST              (ARAD_PP_PROC_DESC_BASE_LIF_COS_LAST)
#define ARAD_PP_PROC_DESC_BASE_LIF_TABLE_LAST               (ARAD_PP_PROC_DESC_BASE_LIF_TABLE_FIRST + 24)
#define ARAD_PP_PROC_DESC_BASE_FRWRD_MACT_FIRST             (ARAD_PP_PROC_DESC_BASE_LIF_TABLE_LAST)
#define ARAD_PP_PROC_DESC_BASE_FRWRD_MACT_LAST              (ARAD_PP_PROC_DESC_BASE_FRWRD_MACT_FIRST + 45)
#define ARAD_PP_PROC_DESC_BASE_FRWRD_MACT_MGMT_FIRST        (ARAD_PP_PROC_DESC_BASE_FRWRD_MACT_LAST)
#define ARAD_PP_PROC_DESC_BASE_FRWRD_MACT_MGMT_LAST         (ARAD_PP_PROC_DESC_BASE_FRWRD_MACT_MGMT_FIRST + 145)
#define ARAD_PP_PROC_DESC_BASE_FRWRD_IPV4_FIRST             (ARAD_PP_PROC_DESC_BASE_FRWRD_MACT_MGMT_LAST)
#define ARAD_PP_PROC_DESC_BASE_FRWRD_IPV4_LAST              (ARAD_PP_PROC_DESC_BASE_FRWRD_IPV4_FIRST + 150)
#define ARAD_PP_PROC_DESC_BASE_FRWRD_IPV6_FIRST             (ARAD_PP_PROC_DESC_BASE_FRWRD_IPV4_LAST)
#define ARAD_PP_PROC_DESC_BASE_FRWRD_IPV6_LAST              (ARAD_PP_PROC_DESC_BASE_FRWRD_IPV6_FIRST + 92)
#define ARAD_PP_PROC_DESC_BASE_FRWRD_ILM_FIRST              (ARAD_PP_PROC_DESC_BASE_FRWRD_IPV6_LAST)
#define ARAD_PP_PROC_DESC_BASE_FRWRD_ILM_LAST               (ARAD_PP_PROC_DESC_BASE_FRWRD_ILM_FIRST + 40)
#define ARAD_PP_PROC_DESC_BASE_FRWRD_BMACT_FIRST            (ARAD_PP_PROC_DESC_BASE_FRWRD_ILM_LAST)
#define ARAD_PP_PROC_DESC_BASE_FRWRD_BMACT_LAST             (ARAD_PP_PROC_DESC_BASE_FRWRD_BMACT_FIRST + 45)
#define ARAD_PP_PROC_DESC_BASE_FRWRD_TRILL_FIRST            (ARAD_PP_PROC_DESC_BASE_FRWRD_BMACT_LAST)
#define ARAD_PP_PROC_DESC_BASE_FRWRD_TRILL_LAST             (ARAD_PP_PROC_DESC_BASE_FRWRD_TRILL_FIRST + 60)
#define ARAD_PP_PROC_DESC_BASE_FRWRD_EXTEND_P2P_FIRST       (ARAD_PP_PROC_DESC_BASE_FRWRD_TRILL_LAST)
#define ARAD_PP_PROC_DESC_BASE_FRWRD_EXTEND_P2P_LAST        (ARAD_PP_PROC_DESC_BASE_FRWRD_EXTEND_P2P_FIRST + 100)
#define ARAD_PP_PROC_DESC_BASE_FRWRD_FEC_FIRST              (ARAD_PP_PROC_DESC_BASE_FRWRD_EXTEND_P2P_LAST)
#define ARAD_PP_PROC_DESC_BASE_FRWRD_FEC_LAST               (ARAD_PP_PROC_DESC_BASE_FRWRD_FEC_FIRST + 92)
#define ARAD_PP_PROC_DESC_BASE_EG_FILTER_FIRST              (ARAD_PP_PROC_DESC_BASE_FRWRD_FEC_LAST)
#define ARAD_PP_PROC_DESC_BASE_EG_FILTER_LAST               (ARAD_PP_PROC_DESC_BASE_EG_FILTER_FIRST + 68)
#define ARAD_PP_PROC_DESC_BASE_EG_QOS_FIRST                 (ARAD_PP_PROC_DESC_BASE_EG_FILTER_LAST)
#define ARAD_PP_PROC_DESC_BASE_EG_QOS_LAST                  (ARAD_PP_PROC_DESC_BASE_EG_QOS_FIRST + 36)
#define ARAD_PP_PROC_DESC_BASE_EG_ENCAP_FIRST               (ARAD_PP_PROC_DESC_BASE_EG_QOS_LAST)
#define ARAD_PP_PROC_DESC_BASE_EG_ENCAP_LAST                (ARAD_PP_PROC_DESC_BASE_EG_ENCAP_FIRST + 112)
#define ARAD_PP_PROC_DESC_BASE_EG_AC_FIRST                  (ARAD_PP_PROC_DESC_BASE_EG_ENCAP_LAST)
#define ARAD_PP_PROC_DESC_BASE_EG_AC_LAST                   (ARAD_PP_PROC_DESC_BASE_EG_AC_FIRST + 52)
#define ARAD_PP_PROC_DESC_BASE_EG_VLAN_EDIT_FIRST           (ARAD_PP_PROC_DESC_BASE_EG_AC_LAST)
#define ARAD_PP_PROC_DESC_BASE_EG_VLAN_EDIT_LAST            (ARAD_PP_PROC_DESC_BASE_EG_VLAN_EDIT_FIRST + 68)
#define ARAD_PP_PROC_DESC_BASE_EG_MIRROR_FIRST              (ARAD_PP_PROC_DESC_BASE_EG_VLAN_EDIT_LAST)
#define ARAD_PP_PROC_DESC_BASE_EG_MIRROR_LAST               (ARAD_PP_PROC_DESC_BASE_EG_MIRROR_FIRST + 32)
#define ARAD_PP_PROC_DESC_BASE_PORT_FIRST                   (ARAD_PP_PROC_DESC_BASE_EG_MIRROR_LAST)
#define ARAD_PP_PROC_DESC_BASE_PORT_LAST                    (ARAD_PP_PROC_DESC_BASE_PORT_FIRST + 44)
#define ARAD_PP_PROC_DESC_BASE_LAG_FIRST                    (ARAD_PP_PROC_DESC_BASE_PORT_LAST)
#define ARAD_PP_PROC_DESC_BASE_LAG_LAST                     (ARAD_PP_PROC_DESC_BASE_LAG_FIRST + 52)
#define ARAD_PP_PROC_DESC_BASE_TRAP_MGMT_FIRST              (ARAD_PP_PROC_DESC_BASE_LAG_LAST)
#define ARAD_PP_PROC_DESC_BASE_TRAP_MGMT_LAST               (ARAD_PP_PROC_DESC_BASE_TRAP_MGMT_FIRST + 56)
#define ARAD_PP_PROC_DESC_BASE_METERING_FIRST               (ARAD_PP_PROC_DESC_BASE_TRAP_MGMT_LAST)
#define ARAD_PP_PROC_DESC_BASE_METERING_LAST                (ARAD_PP_PROC_DESC_BASE_METERING_FIRST + 80)
#define ARAD_PP_PROC_DESC_BASE_COUNTING_FIRST               (ARAD_PP_PROC_DESC_BASE_METERING_LAST)
#define ARAD_PP_PROC_DESC_BASE_COUNTING_LAST                (ARAD_PP_PROC_DESC_BASE_COUNTING_FIRST + 40)
#define ARAD_PP_PROC_DESC_BASE_FP_FIRST                     (ARAD_PP_PROC_DESC_BASE_COUNTING_LAST)
#define ARAD_PP_PROC_DESC_BASE_FP_LAST                      (ARAD_PP_PROC_DESC_BASE_FP_FIRST + 110)
#define ARAD_PP_PROC_DESC_BASE_FRWRD_FCF_FIRST              (ARAD_PP_PROC_DESC_BASE_FP_LAST)

#define ARAD_PP_PROC_DESC_BASE_FRWRD_FCF_LAST               (ARAD_PP_PROC_DESC_BASE_FRWRD_FCF_LAST + 40)
#define ARAD_PP_PROC_DESC_BASE_FP_KEY_FIRST                 (ARAD_PP_PROC_DESC_BASE_FP_LAST)
#define ARAD_PP_PROC_DESC_BASE_FP_KEY_LAST                  (ARAD_PP_PROC_DESC_BASE_FP_KEY_FIRST + 100)
#define ARAD_PP_PROC_DESC_BASE_DIAG_FIRST                   (ARAD_PP_PROC_DESC_BASE_FP_KEY_LAST)
#define ARAD_PP_PROC_DESC_BASE_DIAG_LAST                    (ARAD_PP_PROC_DESC_BASE_DIAG_FIRST + 95)
#define ARAD_PP_PROC_DESC_BASE_SW_DB_FIRST                  (ARAD_PP_PROC_DESC_BASE_DIAG_LAST)
#define ARAD_PP_PROC_DESC_BASE_SW_DB_LAST                   (ARAD_PP_PROC_DESC_BASE_SW_DB_FIRST + 12)
#define ARAD_PP_PROC_DESC_BASE_TBL_ACCESS_FIRST             (ARAD_PP_PROC_DESC_BASE_SW_DB_LAST)
#define ARAD_PP_PROC_DESC_BASE_TBL_ACCESS_LAST              (ARAD_PP_PROC_DESC_BASE_TBL_ACCESS_FIRST + 350)
#define ARAD_PP_PROC_DESC_BASE_LEM_ACCESS_FIRST             (ARAD_PP_PROC_DESC_BASE_TBL_ACCESS_LAST)
#define ARAD_PP_PROC_DESC_BASE_LEM_ACCESS_LAST              (ARAD_PP_PROC_DESC_BASE_LEM_ACCESS_FIRST + 28)
#define ARAD_PP_PROC_DESC_BASE_FRAMEWORK_FIRST              (ARAD_PP_PROC_DESC_BASE_LEM_ACCESS_LAST)
#define ARAD_PP_PROC_DESC_BASE_FRAMEWORK_LAST               (ARAD_PP_PROC_DESC_BASE_FRAMEWORK_FIRST + 14)
#define ARAD_PP_PROC_DESC_BASE_ISEM_ACCESS_FIRST            (ARAD_PP_PROC_DESC_BASE_FRAMEWORK_LAST)
#define ARAD_PP_PROC_DESC_BASE_ISEM_ACCESS_LAST             (ARAD_PP_PROC_DESC_BASE_ISEM_ACCESS_FIRST + 12)
#define ARAD_PP_PROC_DESC_BASE_MGMT_FIRST                   (ARAD_PP_PROC_DESC_BASE_ISEM_ACCESS_LAST)
#define ARAD_PP_PROC_DESC_BASE_MGMT_LAST                    (ARAD_PP_PROC_DESC_BASE_MGMT_FIRST + 30)
#define ARAD_PP_PROC_DESC_BASE_TCAM_FIRST                   (ARAD_PP_PROC_DESC_BASE_MGMT_LAST)
#define ARAD_PP_PROC_DESC_BASE_TCAM_LAST                    (ARAD_PP_PROC_DESC_BASE_TCAM_FIRST + 25)
#define ARAD_PP_PROC_DESC_BASE_TCAM_MGMT_FIRST              (ARAD_PP_PROC_DESC_BASE_TCAM_LAST)
#define ARAD_PP_PROC_DESC_BASE_TCAM_MGMT_LAST               (ARAD_PP_PROC_DESC_BASE_TCAM_MGMT_FIRST + 10)
#define ARAD_PP_PROC_DESC_BASE_INIT_FIRST                   (ARAD_PP_PROC_DESC_BASE_TCAM_MGMT_LAST)
#define ARAD_PP_PROC_DESC_BASE_INIT_LAST                    (ARAD_PP_PROC_DESC_BASE_INIT_FIRST + 80)
#define ARAD_PP_PROC_DESC_BASE_SVEM_ACCESS_FIRST            (ARAD_PP_PROC_DESC_BASE_TCAM_LAST)
#define ARAD_PP_PROC_DESC_BASE_SVEM_ACCESS_LAST             (ARAD_PP_PROC_DESC_BASE_SVEM_ACCESS_FIRST + 80)
#define ARAD_PP_PROC_DESC_BASE_ESEM_FIRST                   (ARAD_PP_PROC_DESC_BASE_SVEM_ACCESS_LAST + 1)
#define ARAD_PP_PROC_DESC_BASE_ESEM_LAST                    (ARAD_PP_PROC_DESC_BASE_ESEM_FIRST + 12)
#define ARAD_PP_PROC_DESC_BASE_FP_EGR_FIRST                 (ARAD_PP_PROC_DESC_BASE_ESEM_LAST + 1)
#define ARAD_PP_PROC_DESC_BASE_FP_EGR_LAST                  (ARAD_PP_PROC_DESC_BASE_FP_EGR_FIRST + 12)
#define ARAD_PP_PROC_DESC_BASE_FP_FEM_FIRST                 (ARAD_PP_PROC_DESC_BASE_FP_EGR_LAST + 1)
#define ARAD_PP_PROC_DESC_BASE_FP_FEM_LAST                  (ARAD_PP_PROC_DESC_BASE_FP_FEM_FIRST + 12)
#define ARAD_PP_PROC_DESC_BASE_SSR_FIRST                    (ARAD_PP_PROC_DESC_BASE_FP_FEM_LAST + 1)
#define ARAD_PP_PROC_DESC_BASE_SSR_LAST                     (ARAD_PP_PROC_DESC_BASE_SSR_FIRST + 15)
#define ARAD_PP_PROC_DESC_BASE_EG_ENCAP_ACCESS_FIRST        (ARAD_PP_PROC_DESC_BASE_SSR_LAST + 1)
#define ARAD_PP_PROC_DESC_BASE_EG_ENCAP_ACCESS_LAST         (ARAD_PP_PROC_DESC_BASE_EG_ENCAP_ACCESS_FIRST + 20)
#define ARAD_PP_PROC_DESC_BASE_ARR_MEM_ALLOCATOR_FIRST      (ARAD_PP_PROC_DESC_BASE_EG_ENCAP_ACCESS_LAST + 1)
#define ARAD_PP_PROC_DESC_BASE_ARR_MEM_ALLOCATOR_LAST       (ARAD_PP_PROC_DESC_BASE_ARR_MEM_ALLOCATOR_FIRST + 30)
#define ARAD_PP_PROC_DESC_BASE_OAM_FIRST                    (ARAD_PP_PROC_DESC_BASE_ARR_MEM_ALLOCATOR_LAST + 1)
#define ARAD_PP_PROC_DESC_BASE_OAM_LAST                     (ARAD_PP_PROC_DESC_BASE_OAM_FIRST + 200) 
#define ARAD_PP_PROC_DESC_BASE_SRC_BIND_FIRST               (ARAD_PP_PROC_DESC_BASE_OAM_LAST + 1)
#define ARAD_PP_PROC_DESC_BASE_SRC_BIND_LAST                (ARAD_PP_PROC_DESC_BASE_SRC_BIND_FIRST + 50) 
#define ARAD_PP_PROC_DESC_BASE_PTP_FIRST                    (ARAD_PP_PROC_DESC_BASE_SRC_BIND_LAST + 1)
#define ARAD_PP_PROC_DESC_BASE_PTP_LAST                     (ARAD_PP_PROC_DESC_BASE_PTP_FIRST + 50) 

/************************************************************************/
/* Error Descriptors                                                    */
/************************************************************************/
#define ARAD_PP_ERR_DESC_BASE_GENERAL_FIRST                 (SOC_SAND_ARAD_PP_START_ERR_NUMBER)
#define ARAD_PP_ERR_DESC_BASE_GENERAL_LAST                  (ARAD_PP_ERR_DESC_BASE_GENERAL_FIRST + 75)
#define ARAD_PP_ERR_DESC_BASE_LLP_PARSE_FIRST               (ARAD_PP_ERR_DESC_BASE_GENERAL_LAST)
#define ARAD_PP_ERR_DESC_BASE_LLP_PARSE_LAST                (ARAD_PP_ERR_DESC_BASE_LLP_PARSE_FIRST + 20)
#define ARAD_PP_ERR_DESC_BASE_LLP_SA_AUTH_FIRST             (ARAD_PP_ERR_DESC_BASE_LLP_PARSE_LAST)
#define ARAD_PP_ERR_DESC_BASE_LLP_SA_AUTH_LAST              (ARAD_PP_ERR_DESC_BASE_LLP_SA_AUTH_FIRST + 9)
#define ARAD_PP_ERR_DESC_BASE_LLP_VID_ASSIGN_FIRST          (ARAD_PP_ERR_DESC_BASE_LLP_SA_AUTH_LAST)
#define ARAD_PP_ERR_DESC_BASE_LLP_VID_ASSIGN_LAST           (ARAD_PP_ERR_DESC_BASE_LLP_VID_ASSIGN_FIRST + 12)
#define ARAD_PP_ERR_DESC_BASE_LLP_FILTER_FIRST              (ARAD_PP_ERR_DESC_BASE_LLP_VID_ASSIGN_LAST)
#define ARAD_PP_ERR_DESC_BASE_LLP_FILTER_LAST               (ARAD_PP_ERR_DESC_BASE_LLP_FILTER_FIRST + 13)
#define ARAD_PP_ERR_DESC_BASE_LLP_COS_FIRST                 (ARAD_PP_ERR_DESC_BASE_LLP_FILTER_LAST)
#define ARAD_PP_ERR_DESC_BASE_LLP_COS_LAST                  (ARAD_PP_ERR_DESC_BASE_LLP_COS_FIRST + 30)
#define ARAD_PP_ERR_DESC_BASE_LLP_TRAP_FIRST                (ARAD_PP_ERR_DESC_BASE_LLP_COS_LAST)
#define ARAD_PP_ERR_DESC_BASE_LLP_TRAP_LAST                 (ARAD_PP_ERR_DESC_BASE_LLP_TRAP_FIRST + 25)
#define ARAD_PP_ERR_DESC_BASE_LLP_MIRROR_FIRST              (ARAD_PP_ERR_DESC_BASE_LLP_TRAP_LAST)
#define ARAD_PP_ERR_DESC_BASE_LLP_MIRROR_LAST               (ARAD_PP_ERR_DESC_BASE_LLP_MIRROR_FIRST + 11)
#define ARAD_PP_ERR_DESC_BASE_MYMAC_FIRST                   (ARAD_PP_ERR_DESC_BASE_LLP_MIRROR_LAST)
#define ARAD_PP_ERR_DESC_BASE_MYMAC_LAST                    (ARAD_PP_ERR_DESC_BASE_MYMAC_FIRST + 30)
#define ARAD_PP_ERR_DESC_BASE_MPLS_TERM_FIRST               (ARAD_PP_ERR_DESC_BASE_MYMAC_LAST)
#define ARAD_PP_ERR_DESC_BASE_MPLS_TERM_LAST                (ARAD_PP_ERR_DESC_BASE_MPLS_TERM_FIRST + 17)
#define ARAD_PP_ERR_DESC_BASE_VSI_FIRST                     (ARAD_PP_ERR_DESC_BASE_MPLS_TERM_LAST)
#define ARAD_PP_ERR_DESC_BASE_VSI_LAST                      (ARAD_PP_ERR_DESC_BASE_VSI_FIRST + 15)
#define ARAD_PP_ERR_DESC_BASE_LIF_FIRST                     (ARAD_PP_ERR_DESC_BASE_VSI_LAST)
#define ARAD_PP_ERR_DESC_BASE_LIF_LAST                      (ARAD_PP_ERR_DESC_BASE_LIF_FIRST + 38)
#define ARAD_PP_ERR_DESC_BASE_RIF_FIRST                     (ARAD_PP_ERR_DESC_BASE_LIF_LAST)
#define ARAD_PP_ERR_DESC_BASE_RIF_LAST                      (ARAD_PP_ERR_DESC_BASE_RIF_FIRST + 35)
#define ARAD_PP_ERR_DESC_BASE_LIF_ING_VLAN_EDIT_FIRST       (ARAD_PP_ERR_DESC_BASE_RIF_LAST)
#define ARAD_PP_ERR_DESC_BASE_LIF_ING_VLAN_EDIT_LAST        (ARAD_PP_ERR_DESC_BASE_LIF_ING_VLAN_EDIT_FIRST + 19)
#define ARAD_PP_ERR_DESC_BASE_LIF_COS_FIRST                 (ARAD_PP_ERR_DESC_BASE_LIF_ING_VLAN_EDIT_LAST)
#define ARAD_PP_ERR_DESC_BASE_LIF_COS_LAST                  (ARAD_PP_ERR_DESC_BASE_LIF_COS_FIRST + 29)
#define ARAD_PP_ERR_DESC_BASE_LIF_TABLE_FIRST               (ARAD_PP_ERR_DESC_BASE_LIF_COS_LAST)
#define ARAD_PP_ERR_DESC_BASE_LIF_TABLE_LAST                (ARAD_PP_ERR_DESC_BASE_LIF_TABLE_FIRST + 10)
#define ARAD_PP_ERR_DESC_BASE_FRWRD_MACT_FIRST              (ARAD_PP_ERR_DESC_BASE_LIF_TABLE_LAST)
#define ARAD_PP_ERR_DESC_BASE_FRWRD_MACT_LAST               (ARAD_PP_ERR_DESC_BASE_FRWRD_MACT_FIRST + 25)
#define ARAD_PP_ERR_DESC_BASE_FRWRD_MACT_MGMT_FIRST         (ARAD_PP_ERR_DESC_BASE_FRWRD_MACT_LAST)
#define ARAD_PP_ERR_DESC_BASE_FRWRD_MACT_MGMT_LAST          (ARAD_PP_ERR_DESC_BASE_FRWRD_MACT_MGMT_FIRST + 30)
#define ARAD_PP_ERR_DESC_BASE_FRWRD_IPV4_FIRST              (ARAD_PP_ERR_DESC_BASE_FRWRD_MACT_MGMT_LAST)
#define ARAD_PP_ERR_DESC_BASE_FRWRD_IPV4_LAST               (ARAD_PP_ERR_DESC_BASE_FRWRD_IPV4_FIRST + 40)
#define ARAD_PP_ERR_DESC_BASE_FRWRD_IPV6_FIRST              (ARAD_PP_ERR_DESC_BASE_FRWRD_IPV4_LAST)
#define ARAD_PP_ERR_DESC_BASE_FRWRD_IPV6_LAST               (ARAD_PP_ERR_DESC_BASE_FRWRD_IPV6_FIRST + 17)
#define ARAD_PP_ERR_DESC_BASE_FRWRD_ILM_FIRST               (ARAD_PP_ERR_DESC_BASE_FRWRD_IPV6_LAST)
#define ARAD_PP_ERR_DESC_BASE_FRWRD_ILM_LAST                (ARAD_PP_ERR_DESC_BASE_FRWRD_ILM_FIRST + 10)
#define ARAD_PP_ERR_DESC_BASE_FRWRD_BMACT_FIRST             (ARAD_PP_ERR_DESC_BASE_FRWRD_ILM_LAST)
#define ARAD_PP_ERR_DESC_BASE_FRWRD_BMACT_LAST              (ARAD_PP_ERR_DESC_BASE_FRWRD_BMACT_FIRST + 20)
#define ARAD_PP_ERR_DESC_BASE_FRWRD_TRILL_FIRST             (ARAD_PP_ERR_DESC_BASE_FRWRD_BMACT_LAST)
#define ARAD_PP_ERR_DESC_BASE_FRWRD_TRILL_LAST              (ARAD_PP_ERR_DESC_BASE_FRWRD_TRILL_FIRST + 14)
#define ARAD_PP_ERR_DESC_BASE_FRWRD_EXTEND_P2P_FIRST        (ARAD_PP_ERR_DESC_BASE_FRWRD_TRILL_LAST)
#define ARAD_PP_ERR_DESC_BASE_FRWRD_EXTEND_P2P_LAST         (ARAD_PP_ERR_DESC_BASE_FRWRD_EXTEND_P2P_FIRST + 13)
#define ARAD_PP_ERR_DESC_BASE_FRWRD_FEC_FIRST               (ARAD_PP_ERR_DESC_BASE_FRWRD_EXTEND_P2P_LAST)
#define ARAD_PP_ERR_DESC_BASE_FRWRD_FEC_LAST                (ARAD_PP_ERR_DESC_BASE_FRWRD_FEC_FIRST + 40)
#define ARAD_PP_ERR_DESC_BASE_EG_FILTER_FIRST               (ARAD_PP_ERR_DESC_BASE_FRWRD_FEC_LAST)
#define ARAD_PP_ERR_DESC_BASE_EG_FILTER_LAST                (ARAD_PP_ERR_DESC_BASE_EG_FILTER_FIRST + 19)
#define ARAD_PP_ERR_DESC_BASE_EG_QOS_FIRST                  (ARAD_PP_ERR_DESC_BASE_EG_FILTER_LAST)
#define ARAD_PP_ERR_DESC_BASE_EG_QOS_LAST                   (ARAD_PP_ERR_DESC_BASE_EG_QOS_FIRST + 15)
#define ARAD_PP_ERR_DESC_BASE_EG_ENCAP_FIRST                (ARAD_PP_ERR_DESC_BASE_EG_QOS_LAST)
#define ARAD_PP_ERR_DESC_BASE_EG_ENCAP_LAST                 (ARAD_PP_ERR_DESC_BASE_EG_ENCAP_FIRST + 49)
#define ARAD_PP_ERR_DESC_BASE_EG_AC_FIRST                   (ARAD_PP_ERR_DESC_BASE_EG_ENCAP_LAST)
#define ARAD_PP_ERR_DESC_BASE_EG_AC_LAST                    (ARAD_PP_ERR_DESC_BASE_EG_AC_FIRST + 18)
#define ARAD_PP_ERR_DESC_BASE_EG_VLAN_EDIT_FIRST            (ARAD_PP_ERR_DESC_BASE_EG_AC_LAST)
#define ARAD_PP_ERR_DESC_BASE_EG_VLAN_EDIT_LAST             (ARAD_PP_ERR_DESC_BASE_EG_VLAN_EDIT_FIRST + 23)
#define ARAD_PP_ERR_DESC_BASE_EG_MIRROR_FIRST               (ARAD_PP_ERR_DESC_BASE_EG_VLAN_EDIT_LAST)
#define ARAD_PP_ERR_DESC_BASE_EG_MIRROR_LAST                (ARAD_PP_ERR_DESC_BASE_EG_MIRROR_FIRST + 10)
#define ARAD_PP_ERR_DESC_BASE_PORT_FIRST                    (ARAD_PP_ERR_DESC_BASE_EG_MIRROR_LAST)
#define ARAD_PP_ERR_DESC_BASE_PORT_LAST                     (ARAD_PP_ERR_DESC_BASE_PORT_FIRST + 19)
#define ARAD_PP_ERR_DESC_BASE_LAG_FIRST                     (ARAD_PP_ERR_DESC_BASE_PORT_LAST)
#define ARAD_PP_ERR_DESC_BASE_LAG_LAST                      (ARAD_PP_ERR_DESC_BASE_LAG_FIRST + 18)
#define ARAD_PP_ERR_DESC_BASE_TRAP_MGMT_FIRST               (ARAD_PP_ERR_DESC_BASE_LAG_LAST)
#define ARAD_PP_ERR_DESC_BASE_TRAP_MGMT_LAST                (ARAD_PP_ERR_DESC_BASE_TRAP_MGMT_FIRST + 33)
#define ARAD_PP_ERR_DESC_BASE_METERING_FIRST                (ARAD_PP_ERR_DESC_BASE_TRAP_MGMT_LAST)
#define ARAD_PP_ERR_DESC_BASE_METERING_LAST                 (ARAD_PP_ERR_DESC_BASE_METERING_FIRST + 35)
#define ARAD_PP_ERR_DESC_BASE_COUNTING_FIRST                (ARAD_PP_ERR_DESC_BASE_METERING_LAST)
#define ARAD_PP_ERR_DESC_BASE_COUNTING_LAST                 (ARAD_PP_ERR_DESC_BASE_COUNTING_FIRST + 14)
#define ARAD_PP_ERR_DESC_BASE_FP_FIRST                      (ARAD_PP_ERR_DESC_BASE_COUNTING_LAST)
#define ARAD_PP_ERR_DESC_BASE_FP_LAST                       (ARAD_PP_ERR_DESC_BASE_FP_FIRST + 50)
#define ARAD_PP_ERR_DESC_BASE_FRWRD_FCF_FIRST                      (ARAD_PP_ERR_DESC_BASE_FP_LAST)
#define ARAD_PP_ERR_DESC_BASE_FRWRD_FCF_LAST                       (ARAD_PP_ERR_DESC_BASE_FRWRD_FCF_FIRST + 30)
#define ARAD_PP_ERR_DESC_BASE_FP_KEY_FIRST                  (ARAD_PP_ERR_DESC_BASE_FP_LAST)
#define ARAD_PP_ERR_DESC_BASE_FP_KEY_LAST                   (ARAD_PP_ERR_DESC_BASE_FP_KEY_FIRST + 40)
#define ARAD_PP_ERR_DESC_BASE_DIAG_FIRST                    (ARAD_PP_ERR_DESC_BASE_FP_KEY_LAST)
#define ARAD_PP_ERR_DESC_BASE_DIAG_LAST                     (ARAD_PP_ERR_DESC_BASE_DIAG_FIRST + 50)
#define ARAD_PP_ERR_DESC_BASE_SW_DB_FIRST                   (ARAD_PP_ERR_DESC_BASE_DIAG_LAST)
#define ARAD_PP_ERR_DESC_BASE_SW_DB_LAST                    (ARAD_PP_ERR_DESC_BASE_SW_DB_FIRST + 5)
#define ARAD_PP_ERR_DESC_BASE_TBL_ACCESS_FIRST              (ARAD_PP_ERR_DESC_BASE_SW_DB_LAST)
#define ARAD_PP_ERR_DESC_BASE_TBL_ACCESS_LAST               (ARAD_PP_ERR_DESC_BASE_TBL_ACCESS_FIRST + 5)
#define ARAD_PP_ERR_DESC_BASE_LEM_ACCESS_FIRST              (ARAD_PP_ERR_DESC_BASE_TBL_ACCESS_LAST)
#define ARAD_PP_ERR_DESC_BASE_LEM_ACCESS_LAST               (ARAD_PP_ERR_DESC_BASE_LEM_ACCESS_FIRST + 25)
#define ARAD_PP_ERR_DESC_BASE_FRAMEWORK_FIRST               (ARAD_PP_ERR_DESC_BASE_LEM_ACCESS_LAST)
#define ARAD_PP_ERR_DESC_BASE_FRAMEWORK_LAST                (ARAD_PP_ERR_DESC_BASE_FRAMEWORK_FIRST + 5)
#define ARAD_PP_ERR_DESC_BASE_ISEM_ACCESS_FIRST             (ARAD_PP_ERR_DESC_BASE_FRAMEWORK_LAST)
#define ARAD_PP_ERR_DESC_BASE_ISEM_ACCESS_LAST              (ARAD_PP_ERR_DESC_BASE_ISEM_ACCESS_FIRST + 5)
#define ARAD_PP_ERR_DESC_BASE_MGMT_FIRST                    (ARAD_PP_ERR_DESC_BASE_ISEM_ACCESS_LAST)
#define ARAD_PP_ERR_DESC_BASE_MGMT_LAST                     (ARAD_PP_ERR_DESC_BASE_MGMT_FIRST + 5)
#define ARAD_PP_ERR_DESC_BASE_PMF_LOW_LEVEL_FIRST           (ARAD_PP_ERR_DESC_BASE_MGMT_LAST)
#define ARAD_PP_ERR_DESC_BASE_PMF_LOW_LEVEL_LAST            (ARAD_PP_ERR_DESC_BASE_PMF_LOW_LEVEL_FIRST + 80)
#define ARAD_PP_ERR_DESC_BASE_TCAM_FIRST                    (ARAD_PP_ERR_DESC_BASE_PMF_LOW_LEVEL_LAST)
#define ARAD_PP_ERR_DESC_BASE_TCAM_LAST                     (ARAD_PP_ERR_DESC_BASE_TCAM_FIRST + 10)
#define ARAD_PP_ERR_DESC_BASE_TCAM_MGMT_FIRST               (ARAD_PP_ERR_DESC_BASE_TCAM_LAST)
#define ARAD_PP_ERR_DESC_BASE_TCAM_MGMT_LAST                (ARAD_PP_ERR_DESC_BASE_TCAM_MGMT_FIRST + 5)
#define ARAD_PP_ERR_DESC_BASE_INIT_FIRST                    (ARAD_PP_ERR_DESC_BASE_TCAM_MGMT_LAST)
#define ARAD_PP_ERR_DESC_BASE_INIT_LAST                     (ARAD_PP_ERR_DESC_BASE_INIT_FIRST + 10)
#define ARAD_PP_ERR_DESC_BASE_SVEM_ACCESS_FIRST             (ARAD_PP_ERR_DESC_BASE_INIT_LAST)
#define ARAD_PP_ERR_DESC_BASE_SVEM_ACCESS_LAST              (ARAD_PP_ERR_DESC_BASE_SVEM_ACCESS_FIRST + 10)
#define ARAD_PP_ERR_DESC_BASE_ESEM_FIRST                    (ARAD_PP_ERR_DESC_BASE_SVEM_ACCESS_LAST + 1)
#define ARAD_PP_ERR_DESC_BASE_ESEM_ACCESS_LAST              (ARAD_PP_ERR_DESC_BASE_ESEM_FIRST + 10)
#define ARAD_PP_ERR_DESC_BASE_FP_EGR_FIRST                  (ARAD_PP_ERR_DESC_BASE_SVEM_ACCESS_LAST + 1)
#define ARAD_PP_ERR_DESC_BASE_FP_EGR_LAST                   (ARAD_PP_ERR_DESC_BASE_FP_EGR_FIRST + 10)
#define ARAD_PP_ERR_DESC_BASE_FP_FEM_FIRST                  (ARAD_PP_ERR_DESC_BASE_FP_EGR_LAST + 1)
#define ARAD_PP_ERR_DESC_BASE_FP_FEM_LAST                   (ARAD_PP_ERR_DESC_BASE_FP_FEM_FIRST + 10)
#define ARAD_PP_ERR_DESC_BASE_SSR_FIRST                     (ARAD_PP_ERR_DESC_BASE_FP_FEM_LAST + 1)
#define ARAD_PP_ERR_DESC_BASE_SSR_LAST                      (ARAD_PP_ERR_DESC_BASE_SSR_FIRST + 10)
#define ARAD_PP_ERR_DESC_BASE_EG_ENCAP_ACCESS_FIRST         (ARAD_PP_ERR_DESC_BASE_SSR_LAST + 1)
#define ARAD_PP_ERR_DESC_BASE_EG_ENCAP_ACCESS_LAST          (ARAD_PP_ERR_DESC_BASE_EG_ENCAP_ACCESS_FIRST + 10)
#define ARAD_PP_ERR_DESC_BASE_ARRAY_MEMORY_ALLOCATOR_FIRST  (ARAD_PP_ERR_DESC_BASE_EG_ENCAP_ACCESS_LAST + 1)
#define ARAD_PP_ERR_DESC_BASE_ARRAY_MEMORY_ALLOCATOR_LAST   (ARAD_PP_ERR_DESC_BASE_ARRAY_MEMORY_ALLOCATOR_FIRST + 10)
#define ARAD_PP_ERR_DESC_BASE_OAM_FIRST                     (ARAD_PP_ERR_DESC_BASE_ARRAY_MEMORY_ALLOCATOR_LAST + 1)
#define ARAD_PP_ERR_DESC_BASE_OAM_LAST                      (ARAD_PP_ERR_DESC_BASE_OAM_FIRST + 40)
#define ARAD_PP_ERR_DESC_BASE_SRC_BIND_FIRST                (ARAD_PP_ERR_DESC_BASE_OAM_LAST + 1)
#define ARAD_PP_ERR_DESC_BASE_SRC_BIND_LAST                 (ARAD_PP_ERR_DESC_BASE_OAM_LAST + 20)
#define ARAD_PP_ERR_DESC_BASE_PTP_FIRST                     (ARAD_PP_ERR_DESC_BASE_SRC_BIND_LAST + 1)
#define ARAD_PP_ERR_DESC_BASE_PTP_LAST                      (ARAD_PP_ERR_DESC_BASE_SRC_BIND_LAST + 30)

/* } */
/*************
 * MACROS    *
 *************/
/* { */

#define ARAD_PP_DO_NOTHING_AND_EXIT                                         \
          SOC_SAND_IGNORE_UNUSED_VAR(res);                                    \
          goto exit

#define ARAD_PP_STRUCT_VERIFY(type, name, exit_num, exit_place)             \
          do                                                              \
          {                                                               \
            res = type##_verify(                                          \
                    name                                                  \
                  );                                                      \
            SOC_SAND_CHECK_FUNC_RESULT(res, exit_num, exit_place);            \
          } while(0)

#define ARAD_PP_STRUCT_VERIFY_UNIT(type, unit, name, exit_num, exit_place)             \
          do                                                              \
          {                                                               \
            res = type##_verify(                                          \
                    unit,                                                 \
                    name                                                  \
                  );                                                      \
            SOC_SAND_CHECK_FUNC_RESULT(res, exit_num, exit_place);            \
          } while(0)


/* } */
/*************
 * TYPE DEFS *
 *************/
/* { */

typedef enum
{
  /*
   * Auto generated. Do not edit following section {
   */
  ARAD_PP_PROCEDURE_DESC_ADD = ARAD_PP_PROC_DESC_BASE_FRAMEWORK_FIRST,
  ARAD_PP_ERRORS_DESC_ADD,
  ARAD_PP_FRAMEWORK_GET_PROCS_PTR,
  ARAD_PP_FRAMEWORK_GET_ERRS_PTR,
  /*
   * } Auto generated. Do not edit previous section.
   */

  /*
   * Last element. Do no touch.
   */
  ARAD_PP_FRAMEWORK_PROCEDURE_DESC_LAST
} ARAD_PP_FRAMEWORK_PROCEDURE_DESC;

typedef enum
{
  /*
   * Auto generated. Do not edit following section {
   */
  /*
   * } Auto generated. Do not edit previous section.
   */


  /*
   * Last element. Do no touch.
   */
  ARAD_PP_FRAMEWORK_ERR_LAST
} ARAD_PP_FRAMEWORK_ERR;

/* } */
/*************
 * GLOBALS   *
 *************/
/* { */

/* } */

#include <soc/dpp/SAND/Utils/sand_footer.h>

/* } __ARAD_PP_FRAMEWORK_INCLUDED__*/
#endif


