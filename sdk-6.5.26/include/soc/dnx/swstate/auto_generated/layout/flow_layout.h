
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2022 Broadcom Inc. All rights reserved.
 */

#ifndef __FLOW_LAYOUT_H__
#define __FLOW_LAYOUT_H__

#include <include/soc/dnxc/swstate/dnxc_sw_state_layout.h>
#include <include/soc/dnxc/swstate/auto_generated/types/dnxc_module_ids_types.h>


int flow_init_layout_structure(int unit);

typedef enum
{
    DNX_SW_STATE_FLOW_DB = DNXC_SW_STATE_LAYOUT_NODE_ID_GET(FLOW_MODULE_ID, 0),
    DNX_SW_STATE_FLOW_DB__FIRST,
    DNX_SW_STATE_FLOW_DB__FIRST_SUB = DNX_SW_STATE_FLOW_DB__FIRST - 1,
    DNX_SW_STATE_FLOW_DB__FLOW_APPLICATION_INFO,
    DNX_SW_STATE_FLOW_DB__VALID_PHASES_PER_DBAL_TABLE,
    DNX_SW_STATE_FLOW_DB__DBAL_TABLE_TO_VALID_RESULT_TYPES,
    DNX_SW_STATE_FLOW_DB__LAST,
    DNX_SW_STATE_FLOW_DB__LAST_SUB = DNX_SW_STATE_FLOW_DB__LAST - 1,
    DNX_SW_STATE_FLOW_DB__FLOW_APPLICATION_INFO__FIRST,
    DNX_SW_STATE_FLOW_DB__FLOW_APPLICATION_INFO__FIRST_SUB = DNX_SW_STATE_FLOW_DB__FLOW_APPLICATION_INFO__FIRST - 1,
    DNX_SW_STATE_FLOW_DB__FLOW_APPLICATION_INFO__IS_VERIFY_DISABLED,
    DNX_SW_STATE_FLOW_DB__FLOW_APPLICATION_INFO__IS_ERROR_RECOVERY_DISABLED,
    DNX_SW_STATE_FLOW_DB__FLOW_APPLICATION_INFO__LAST,
    DNX_SW_STATE_FLOW_DB__FLOW_APPLICATION_INFO__LAST_SUB = DNX_SW_STATE_FLOW_DB__FLOW_APPLICATION_INFO__LAST - 1,
    
    DNX_SW_STATE_FLOW_DB_NOF_PARAMS,
} dnxc_sw_state_layout_flow_node_id_e;

#endif 
