/*
 * $Id: dcmn_fake.c,v 1.0 Broadcom SDK $
 *
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 */
 
#include <soc/dcmn/dcmn_gen_err.h>

#if (defined(BCM_DPP_SUPPORT) || defined(BCM_DFE_SUPPORT)) && defined(BCM_GEN_ERR_MECHANISM)
sal_mutex_t gen_err_lock[SOC_MAX_NUM_DEVICES] = { NULL };
int gen_err_test[SOC_MAX_NUM_DEVICES];
int gen_err_running[SOC_MAX_NUM_DEVICES];
int gen_err_cmp[SOC_MAX_NUM_DEVICES][GEN_ERR_TYPE_NUMS];
int gen_err_cnt[SOC_MAX_NUM_DEVICES][GEN_ERR_TYPE_NUMS];
#endif

