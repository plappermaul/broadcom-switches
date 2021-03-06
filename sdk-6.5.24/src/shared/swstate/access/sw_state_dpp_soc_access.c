/* 
 * 
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2021 Broadcom Inc. All rights reserved.
 * 
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated.
 * Edits to this file will be lost when it is regenerated.
 */

#include <sal/core/libc.h>
#include <shared/swstate/sw_state.h>
#include <shared/error.h>
#include <shared/swstate/sw_state_defs.h>
#include <shared/bsl.h>
#include <shared/swstate/access/sw_state_access.h>
#include <shared/swstate/access/sw_state_dpp_soc_access.h>

#ifdef _ERR_MSG_MODULE_NAME
    #error "_ERR_MSG_MODULE_NAME redefined"
#endif

#define _ERR_MSG_MODULE_NAME BSL_LS_SHARED_SWSTATE

extern shr_sw_state_t *sw_state[BCM_MAX_NUM_UNITS];

#ifdef BCM_PETRA_SUPPORT
/* AUTO-GENERATED - DO NOT MODIFY */
int sw_state_dpp_soc_verify(int unit){
    SW_STATE_INIT_FUNC_DEFS;
    _rv = sw_state_access[unit].dpp.verify( unit);
    _SOC_IF_ERR_EXIT(_rv);
    SW_STATE_FUNC_RETURN;
}

#endif /* BCM_PETRA_SUPPORT*/ 


int
sw_state_dpp_soc_access_cb_init(int unit){
    sw_state_dpp_soc_arad_access_cb_init(unit);
    sw_state_dpp_soc_config_access_cb_init(unit);
    sw_state_dpp_soc_jericho_access_cb_init(unit);
    sw_state_dpp_soc_qax_access_cb_init(unit);
    sw_state_dpp_soc_sand_access_cb_init(unit);
#ifdef BCM_PETRA_SUPPORT
    sw_state_access[unit].dpp.soc.verify = sw_state_dpp_soc_verify;
#endif /* BCM_PETRA_SUPPORT*/ 
    return _SHR_E_NONE;
}

#undef _ERR_MSG_MODULE_NAME
