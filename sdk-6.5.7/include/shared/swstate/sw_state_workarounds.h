/*
 * $Id: $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$ 
 *  
 */
#ifndef _SHR_SW_STATE_WORKAROUNDS_H
#define _SHR_SW_STATE_WORKAROUNDS_H

#if defined(INCLUDE_KBP) && !defined(BCM_88030)
#include <soc/kbp/alg_kbp/include/db.h>
#endif /* defined(INCLUDE_KBP) && !defined(BCM_88030) */



#if defined(INCLUDE_KBP) && !defined(BCM_88030)
/* 
 * this workaround is used to avoid the use of struct kbp_db inside the PARSED driver code
 * this file is not parsed by autocoder
 * use kbp_db_t in driver code instead.
 */
typedef struct kbp_db kbp_db_t;
typedef struct kbp_entry kbp_entry_t;
typedef struct kbp_ad kbp_ad_t;
typedef struct kbp_key kbp_key_t;
typedef struct kbp_ad_db kbp_ad_db_t;
typedef struct kbp_instruction kbp_instruction_t;

#endif /* defined(INCLUDE_KBP) && !defined(BCM_88030) */



#endif /* _SHR_SW_STATE_WORKAROUNDS_H */


