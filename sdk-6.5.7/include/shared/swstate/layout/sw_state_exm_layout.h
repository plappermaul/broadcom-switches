/* $Id: $
 * 
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 * 
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated.
 * Edits to this file will be lost when it is regenerated.
*/
#ifndef _SHR_sw_state_exm_H_
#define _SHR_sw_state_exm_H_


#include <shared/swstate/layout/sw_state_exm_twoLevelsArr_layout.h>
#include <shared/swstate/layout/sw_state_exm_twoLevelsArrArr_layout.h>
#include <shared/swstate/layout/sw_state_exm_twoLevelsArrPtr_layout.h>
#include <shared/swstate/layout/sw_state_exm_twoLevelsPtr_layout.h>
#include <shared/swstate/layout/sw_state_exm_twoLevelsRegular_layout.h>
#include <shared/swstate/layout/sw_state_exm_twoLevelsRegularAllowWbAccess_layout.h>
#include <shared/swstate/layout/sw_state_exm_twoLevelsSinglePtr_layout.h>
#include <shared/swstate/layout/sw_state_exm_twoLevelsStaticArrArr_layout.h>
#include <shared/swstate/layout/sw_state_exm_twoLevelsStaticArrInt_layout.h>
#define SW_STATE_EXM_LAYOUT_NOF_MAX_LAYOUT_NODES (1 + SW_STATE_EXM_TWOLEVELSREGULAR_LAYOUT_NOF_MAX_LAYOUT_NODES + SW_STATE_EXM_TWOLEVELSSINGLEPTR_LAYOUT_NOF_MAX_LAYOUT_NODES + SW_STATE_EXM_TWOLEVELSSTATICARRINT_LAYOUT_NOF_MAX_LAYOUT_NODES + SW_STATE_EXM_TWOLEVELSSTATICARRARR_LAYOUT_NOF_MAX_LAYOUT_NODES + SW_STATE_EXM_TWOLEVELSPTR_LAYOUT_NOF_MAX_LAYOUT_NODES + SW_STATE_EXM_TWOLEVELSARR_LAYOUT_NOF_MAX_LAYOUT_NODES + SW_STATE_EXM_TWOLEVELSARRARR_LAYOUT_NOF_MAX_LAYOUT_NODES + SW_STATE_EXM_TWOLEVELSARRPTR_LAYOUT_NOF_MAX_LAYOUT_NODES + SW_STATE_EXM_TWOLEVELSREGULARALLOWWBACCESS_LAYOUT_NOF_MAX_LAYOUT_NODES) 

#ifdef _SHR_SW_STATE_EXM
int sw_state_exm_layout_node_create(int unit, int* root_node_id, uint32* next_free_node_id);
#endif /* _SHR_SW_STATE_EXM*/ 

#endif /* _SHR_sw_state_exm_H_ */
