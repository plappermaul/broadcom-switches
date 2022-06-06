/* 
 * 
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2021 Broadcom Inc. All rights reserved.
 * 
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated.
 * Edits to this file will be lost when it is regenerated.
 * search for 'sw_state_diagnostic_cbs_t' for the root of the struct
 */

#ifndef _SHR_SW_STATE_MINIEXM_DIAGNOSTIC_H_
#define _SHR_SW_STATE_MINIEXM_DIAGNOSTIC_H_

#ifdef BCM_WARM_BOOT_SUPPORT
/********************************* diagnostic calbacks definitions *************************************/
/* this set of callbacks, are the callbacks used in the diagnostic calbacks struct 'sw_state_cbs_t' to */
/* access the data in 'sw_state_t'.                                                                */
/* the calbacks are inserted into the diagnostic struct by 'sw_state_diagnostic_cb_init'.                  */
/***************************************************************************************************/

#ifdef _SHR_SW_STATE_EXM
/* implemented by: sw_state_miniExm_dump */
typedef int (*sw_state_miniExm_dump_cb)(
    int unit);
#endif /* _SHR_SW_STATE_EXM*/ 

#ifdef _SHR_SW_STATE_EXM
/* implemented by: sw_state_miniExm_regular_dump */
typedef int (*sw_state_miniExm_regular_dump_cb)(
    int unit, int miniExm_idx_0);
#endif /* _SHR_SW_STATE_EXM*/ 

#ifdef _SHR_SW_STATE_EXM
/* implemented by: sw_state_miniExm_singlePtr_dump */
typedef int (*sw_state_miniExm_singlePtr_dump_cb)(
    int unit, int miniExm_idx_0);
#endif /* _SHR_SW_STATE_EXM*/ 

#ifdef _SHR_SW_STATE_EXM
/* implemented by: sw_state_miniExm_staticArr_dump */
typedef int (*sw_state_miniExm_staticArr_dump_cb)(
    int unit, int miniExm_idx_0);
#endif /* _SHR_SW_STATE_EXM*/ 

#ifdef _SHR_SW_STATE_EXM
/* implemented by: sw_state_miniExm_staticArrArr_dump */
typedef int (*sw_state_miniExm_staticArrArr_dump_cb)(
    int unit, int miniExm_idx_0);
#endif /* _SHR_SW_STATE_EXM*/ 

#ifdef _SHR_SW_STATE_EXM
/* implemented by: sw_state_miniExm_ptrVar_dump */
typedef int (*sw_state_miniExm_ptrVar_dump_cb)(
    int unit, int miniExm_idx_0);
#endif /* _SHR_SW_STATE_EXM*/ 

#ifdef _SHR_SW_STATE_EXM
/* implemented by: sw_state_miniExm_arrVar_dump */
typedef int (*sw_state_miniExm_arrVar_dump_cb)(
    int unit, int miniExm_idx_0);
#endif /* _SHR_SW_STATE_EXM*/ 

#ifdef _SHR_SW_STATE_EXM
/* implemented by: sw_state_miniExm_arrArr_dump */
typedef int (*sw_state_miniExm_arrArr_dump_cb)(
    int unit, int miniExm_idx_0);
#endif /* _SHR_SW_STATE_EXM*/ 

#ifdef _SHR_SW_STATE_EXM
/* implemented by: sw_state_miniExm_arrPtr_dump */
typedef int (*sw_state_miniExm_arrPtr_dump_cb)(
    int unit, int miniExm_idx_0);
#endif /* _SHR_SW_STATE_EXM*/ 

#ifdef _SHR_SW_STATE_EXM
/* implemented by: sw_state_miniExm_shr_bitdcl_singlePtr_dump */
typedef int (*sw_state_miniExm_shr_bitdcl_singlePtr_dump_cb)(
    int unit, int miniExm_idx_0);
#endif /* _SHR_SW_STATE_EXM*/ 

#ifdef _SHR_SW_STATE_EXM
/* implemented by: sw_state_miniExm_shr_bitdcl_doublePtr_dump */
typedef int (*sw_state_miniExm_shr_bitdcl_doublePtr_dump_cb)(
    int unit, int miniExm_idx_0);
#endif /* _SHR_SW_STATE_EXM*/ 

#ifdef _SHR_SW_STATE_EXM
/* implemented by: sw_state_miniExm_shr_bitdcl_staticArr_dump */
typedef int (*sw_state_miniExm_shr_bitdcl_staticArr_dump_cb)(
    int unit, int miniExm_idx_0);
#endif /* _SHR_SW_STATE_EXM*/ 

#ifdef _SHR_SW_STATE_EXM
/* implemented by: sw_state_miniExm_shr_bitdcl_double_staticArr_dump */
typedef int (*sw_state_miniExm_shr_bitdcl_double_staticArr_dump_cb)(
    int unit, int miniExm_idx_0);
#endif /* _SHR_SW_STATE_EXM*/ 

#ifdef _SHR_SW_STATE_EXM
/* implemented by: sw_state_miniExm_buff_dump */
typedef int (*sw_state_miniExm_buff_dump_cb)(
    int unit, int miniExm_idx_0);
#endif /* _SHR_SW_STATE_EXM*/ 

#ifdef _SHR_SW_STATE_EXM
/* implemented by: sw_state_miniExm_regularPbmp_dump */
typedef int (*sw_state_miniExm_regularPbmp_dump_cb)(
    int unit, int miniExm_idx_0);
#endif /* _SHR_SW_STATE_EXM*/ 

#ifdef _SHR_SW_STATE_EXM
/* implemented by: sw_state_miniExm_pbmpPtr_dump */
typedef int (*sw_state_miniExm_pbmpPtr_dump_cb)(
    int unit, int miniExm_idx_0);
#endif /* _SHR_SW_STATE_EXM*/ 

/*********************************** diagnostic calbacks struct ****************************************/
/* this set of structs, rooted at 'sw_state_cbs_t' define the diagnostic layer for the entire SW state.*/
/* use this tree to dump fields in the sw state rooted at 'sw_state_t'.              */
/* NOTE: 'sw_state_t' data should not be accessed directly.                                        */
/***************************************************************************************************/

#ifdef _SHR_SW_STATE_EXM
typedef struct sw_state_miniExm_regular_diagnostic_cbs_s {
    sw_state_miniExm_regular_dump_cb dump;
} sw_state_miniExm_regular_diagnostic_cbs_t;
#endif /* _SHR_SW_STATE_EXM*/ 

#ifdef _SHR_SW_STATE_EXM
typedef struct sw_state_miniExm_singlePtr_diagnostic_cbs_s {
    sw_state_miniExm_singlePtr_dump_cb dump;
} sw_state_miniExm_singlePtr_diagnostic_cbs_t;
#endif /* _SHR_SW_STATE_EXM*/ 

#ifdef _SHR_SW_STATE_EXM
typedef struct sw_state_miniExm_staticArr_diagnostic_cbs_s {
    sw_state_miniExm_staticArr_dump_cb dump;
} sw_state_miniExm_staticArr_diagnostic_cbs_t;
#endif /* _SHR_SW_STATE_EXM*/ 

#ifdef _SHR_SW_STATE_EXM
typedef struct sw_state_miniExm_staticArrArr_diagnostic_cbs_s {
    sw_state_miniExm_staticArrArr_dump_cb dump;
} sw_state_miniExm_staticArrArr_diagnostic_cbs_t;
#endif /* _SHR_SW_STATE_EXM*/ 

#ifdef _SHR_SW_STATE_EXM
typedef struct sw_state_miniExm_ptrVar_diagnostic_cbs_s {
    sw_state_miniExm_ptrVar_dump_cb dump;
} sw_state_miniExm_ptrVar_diagnostic_cbs_t;
#endif /* _SHR_SW_STATE_EXM*/ 

#ifdef _SHR_SW_STATE_EXM
typedef struct sw_state_miniExm_arrVar_diagnostic_cbs_s {
    sw_state_miniExm_arrVar_dump_cb dump;
} sw_state_miniExm_arrVar_diagnostic_cbs_t;
#endif /* _SHR_SW_STATE_EXM*/ 

#ifdef _SHR_SW_STATE_EXM
typedef struct sw_state_miniExm_arrArr_diagnostic_cbs_s {
    sw_state_miniExm_arrArr_dump_cb dump;
} sw_state_miniExm_arrArr_diagnostic_cbs_t;
#endif /* _SHR_SW_STATE_EXM*/ 

#ifdef _SHR_SW_STATE_EXM
typedef struct sw_state_miniExm_arrPtr_diagnostic_cbs_s {
    sw_state_miniExm_arrPtr_dump_cb dump;
} sw_state_miniExm_arrPtr_diagnostic_cbs_t;
#endif /* _SHR_SW_STATE_EXM*/ 

#ifdef _SHR_SW_STATE_EXM
typedef struct sw_state_miniExm_shr_bitdcl_singlePtr_diagnostic_cbs_s {
    sw_state_miniExm_shr_bitdcl_singlePtr_dump_cb dump;
} sw_state_miniExm_shr_bitdcl_singlePtr_diagnostic_cbs_t;
#endif /* _SHR_SW_STATE_EXM*/ 

#ifdef _SHR_SW_STATE_EXM
typedef struct sw_state_miniExm_shr_bitdcl_doublePtr_diagnostic_cbs_s {
    sw_state_miniExm_shr_bitdcl_doublePtr_dump_cb dump;
} sw_state_miniExm_shr_bitdcl_doublePtr_diagnostic_cbs_t;
#endif /* _SHR_SW_STATE_EXM*/ 

#ifdef _SHR_SW_STATE_EXM
typedef struct sw_state_miniExm_shr_bitdcl_staticArr_diagnostic_cbs_s {
    sw_state_miniExm_shr_bitdcl_staticArr_dump_cb dump;
} sw_state_miniExm_shr_bitdcl_staticArr_diagnostic_cbs_t;
#endif /* _SHR_SW_STATE_EXM*/ 

#ifdef _SHR_SW_STATE_EXM
typedef struct sw_state_miniExm_shr_bitdcl_double_staticArr_diagnostic_cbs_s {
    sw_state_miniExm_shr_bitdcl_double_staticArr_dump_cb dump;
} sw_state_miniExm_shr_bitdcl_double_staticArr_diagnostic_cbs_t;
#endif /* _SHR_SW_STATE_EXM*/ 

#ifdef _SHR_SW_STATE_EXM
typedef struct sw_state_miniExm_buff_diagnostic_cbs_s {
    sw_state_miniExm_buff_dump_cb dump;
} sw_state_miniExm_buff_diagnostic_cbs_t;
#endif /* _SHR_SW_STATE_EXM*/ 

#ifdef _SHR_SW_STATE_EXM
typedef struct sw_state_miniExm_regularPbmp_diagnostic_cbs_s {
    sw_state_miniExm_regularPbmp_dump_cb dump;
} sw_state_miniExm_regularPbmp_diagnostic_cbs_t;
#endif /* _SHR_SW_STATE_EXM*/ 

#ifdef _SHR_SW_STATE_EXM
typedef struct sw_state_miniExm_pbmpPtr_diagnostic_cbs_s {
    sw_state_miniExm_pbmpPtr_dump_cb dump;
} sw_state_miniExm_pbmpPtr_diagnostic_cbs_t;
#endif /* _SHR_SW_STATE_EXM*/ 

#ifdef _SHR_SW_STATE_EXM
typedef struct sw_state_miniExm_diagnostic_cbs_s {
#ifdef _SHR_SW_STATE_EXM
    sw_state_miniExm_dump_cb dump;
#endif /* _SHR_SW_STATE_EXM*/ 
    sw_state_miniExm_regular_diagnostic_cbs_t regular;
    sw_state_miniExm_singlePtr_diagnostic_cbs_t singlePtr;
    sw_state_miniExm_staticArr_diagnostic_cbs_t staticArr;
    sw_state_miniExm_staticArrArr_diagnostic_cbs_t staticArrArr;
    sw_state_miniExm_ptrVar_diagnostic_cbs_t ptrVar;
    sw_state_miniExm_arrVar_diagnostic_cbs_t arrVar;
    sw_state_miniExm_arrArr_diagnostic_cbs_t arrArr;
    sw_state_miniExm_arrPtr_diagnostic_cbs_t arrPtr;
    sw_state_miniExm_shr_bitdcl_singlePtr_diagnostic_cbs_t shr_bitdcl_singlePtr;
    sw_state_miniExm_shr_bitdcl_doublePtr_diagnostic_cbs_t shr_bitdcl_doublePtr;
    sw_state_miniExm_shr_bitdcl_staticArr_diagnostic_cbs_t shr_bitdcl_staticArr;
    sw_state_miniExm_shr_bitdcl_double_staticArr_diagnostic_cbs_t shr_bitdcl_double_staticArr;
    sw_state_miniExm_buff_diagnostic_cbs_t buff;
    sw_state_miniExm_regularPbmp_diagnostic_cbs_t regularPbmp;
    sw_state_miniExm_pbmpPtr_diagnostic_cbs_t pbmpPtr;
} sw_state_miniExm_diagnostic_cbs_t;

#endif /* _SHR_SW_STATE_EXM*/ 

int sw_state_miniExm_diagnostic_cb_init(int unit);

#endif /* BCM_WARM_BOOT_SUPPORT */

#endif /* _SHR_SW_STATE_MINIEXM_DIAGNOSTIC_H_ */