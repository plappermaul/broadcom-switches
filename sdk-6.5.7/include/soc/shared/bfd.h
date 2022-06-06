/*
 * $Id: bfd.h,v 1.37 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * File:    bfd.h
 * Purpose: BFD definitions common to SDK and uKernel
 *
 * Notes:   Definition changes should be avoided in order to
 *          maintain compatibility between SDK and uKernel since
 *          both images are built and loaded separately.
 *
 */

#ifndef _SOC_SHARED_BFD_H
#define _SOC_SHARED_BFD_H

#ifdef BCM_UKERNEL
  /* Build for uKernel not SDK */
  #include "sdk_typedefs.h"
#else
  #include <sal/types.h>
#endif

#include <shared/bfd.h>


/*****************************************
 * BFD uController Error codes
 */
typedef enum shr_bfd_uc_error_e {
    SHR_BFD_UC_E_NONE = 0,
    SHR_BFD_UC_E_INTERNAL,
    SHR_BFD_UC_E_MEMORY,
    SHR_BFD_UC_E_UNIT,
    SHR_BFD_UC_E_PARAM,
    SHR_BFD_UC_E_EMPTY,
    SHR_BFD_UC_E_FULL,
    SHR_BFD_UC_E_NOT_FOUND,
    SHR_BFD_UC_E_EXISTS,
    SHR_BFD_UC_E_TIMEOUT,
    SHR_BFD_UC_E_BUSY,
    SHR_BFD_UC_E_FAIL,
    SHR_BFD_UC_E_DISABLED,
    SHR_BFD_UC_E_BADID,
    SHR_BFD_UC_E_RESOURCE,
    SHR_BFD_UC_E_CONFIG,
    SHR_BFD_UC_E_UNAVAIL,
    SHR_BFD_UC_E_INIT,
    SHR_BFD_UC_E_PORT
} shr_bfd_uc_error_t;

#define SHR_BFD_UC_SUCCESS(rv)              ((rv) == SHR_BFD_UC_E_NONE)
#define SHR_BFD_UC_FAILURE(rv)              ((rv) != SHR_BFD_UC_E_NONE)

/*
 * Macro:
 *      SHR_BFD_IF_ERROR_RETURN
 * Purpose:
 *      Evaluate _op as an expression, and if an error, return.
 * Notes:
 *      This macro uses a do-while construct to maintain expected
 *      "C" blocking, and evaluates "op" ONLY ONCE so it may be
 *      a function call that has side affects.
 */

#define SHR_BFD_IF_ERROR_RETURN(op)                                     \
    do {                                                                \
        int __rv__;                                                     \
        if ((__rv__ = (op)) != SHR_BFD_UC_E_NONE) {                     \
            return(__rv__);                                             \
        }                                                               \
    } while(0)


/*
 * BFD for Unknown Remote Discriminator
 * 'your_discriminator' zero
 */
#define SHR_BFD_DISCR_UNKNOWN_SESSION_ID        0x7FF    /* Session ID */
#define SHR_BFD_DISCR_UNKNOWN_YOUR_DISCR        0        /* Discriminator */

/*
 * BFD Session Set flags
 */
#define SHR_BFD_SESS_SET_F_CREATE               0x00000001
#define SHR_BFD_SESS_SET_F_LOCAL_DISC           0x00000002
#define SHR_BFD_SESS_SET_F_LOCAL_MIN_TX         0x00000004
#define SHR_BFD_SESS_SET_F_LOCAL_MIN_RX         0x00000008
#define SHR_BFD_SESS_SET_F_LOCAL_MIN_ECHO_RX    0x00000010
#define SHR_BFD_SESS_SET_F_LOCAL_DIAG           0x00000020
#define SHR_BFD_SESS_SET_F_LOCAL_DEMAND         0x00000040
#define SHR_BFD_SESS_SET_F_LOCAL_DETECT_MULT    0x00000080
#define SHR_BFD_SESS_SET_F_SESSION_DESTROY      0x00000100
#define SHR_BFD_SESS_SET_F_SHA1_XMT_SEQ_INCR    0x00000200
#define SHR_BFD_SESS_SET_F_ENCAP                0x00000400
#define SHR_BFD_SESS_SET_F_REMOTE_DISC          0x00000800
#define SHR_BFD_SESS_SET_F_AUTH_TYPE            0x00001000
#define SHR_BFD_SESS_SET_F_LOCAL_ECHO           0x00002000
#define SHR_BFD_SESS_SET_F_MEP_ID_LENGTH        0x00004000
#define SHR_BFD_SESS_SET_F_MEP_ID               0x00008000
#define SHR_BFD_SESS_SET_F_REMOTE_MEP_ID_LENGTH 0x00010000
#define SHR_BFD_SESS_SET_F_REMOTE_MEP_ID        0x00020000
#define SHR_BFD_SESS_SET_F_TRUNK                0x00040000
#define SHR_BFD_SESS_SET_F_MICRO_BFD            0x00080000
#define SHR_BFD_SESS_SET_F_SAMPLING_RATIO       0x00100000
#define SHR_BFD_SESS_SET_F_MPLS_PHP             0x00200000
/*
 * BFD sampling flags
 */
#define SHR_BFD_SAMPLING_F_TRIGGER              0x80

/*
 * BFD Encapsulation types
 */
#define SHR_BFD_ENCAP_TYPE_RAW             0
#define SHR_BFD_ENCAP_TYPE_V4UDP           1
#define SHR_BFD_ENCAP_TYPE_V6UDP           2
#define SHR_BFD_ENCAP_TYPE_MPLS_TP_CC      3 /* MPLS-TP CC only mode */   
#define SHR_BFD_ENCAP_TYPE_MPLS_TP_CC_CV   4 /* MPLS-TP CC CV */   
#define SHR_BFD_ENCAP_TYPE_MPLS_PHP        5 /* MPLS PHP*/

#endif /* _SOC_SHARED_BFD_H */
