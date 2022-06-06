
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2022 Broadcom Inc. All rights reserved.
 */

#ifdef BSL_LOG_MODULE
#error "BSL_LOG_MODULE redefined"
#endif
#define BSL_LOG_MODULE BSL_LS_SWSTATEDNX_GENERAL

#include <soc/dnx/swstate/auto_generated/access/flush_access.h>
#include <soc/dnx/swstate/auto_generated/diagnostic/flush_diagnostic.h>
#include <soc/dnx/swstate/auto_generated/layout/flush_layout.h>





int
flush_db_context_is_init(int unit, uint8 *is_init)
{
    DNXC_SW_STATE_INIT_FUNC_DEFS;

    VERIFY_FUNCTION_CALL_ALLOWED(
        unit,
        DNX_SW_STATE_FLUSH_DB_CONTEXT,
        SW_STATE_FUNC_IS_INIT,
        DNXC_SW_STATE_NO_FLAGS);

    *is_init = (NULL != ((flush_db_t*)sw_state_roots_array[unit][FLUSH_MODULE_ID]));
    DNX_SW_STATE_LOG(
        unit,
        DNX_SW_STATE_IS_INIT_LOGGING,
        BSL_LS_SWSTATEDNX_ISINIT,
        DNX_SW_STATE_FLUSH_DB_CONTEXT,
        ((flush_db_t*)sw_state_roots_array[unit][FLUSH_MODULE_ID]),
        "flush_db_context[%d]",
        unit);

    DNXC_SW_STATE_FUNC_RETURN;
}



int
flush_db_context_init(int unit)
{
    DNXC_SW_STATE_INIT_FUNC_DEFS;

    VERIFY_FUNCTION_CALL_ALLOWED(
        unit,
        DNX_SW_STATE_FLUSH_DB_CONTEXT,
        SW_STATE_FUNC_INIT,
        DNXC_SW_STATE_NO_FLAGS);

    DNX_SW_STATE_MODULE_INIT(
        unit,
        DNX_SW_STATE_FLUSH_DB_CONTEXT,
        DNXC_SW_STATE_IMPLEMENTATION_WB,
        flush_db_t,
        DNX_SW_STATE_FLUSH_DB_CONTEXT_NOF_PARAMS,
        DNXC_SW_STATE_AUTO_GENERATED_ALLOCATION,
        "flush_db_context_init",
        DNX_SW_STATE_DIAG_ALLOC,
        flush_init_layout_structure);

    DNX_SW_STATE_LOG(
        unit,
        DNX_SW_STATE_INIT_LOGGING,
        BSL_LS_SWSTATEDNX_INIT,
        DNX_SW_STATE_FLUSH_DB_CONTEXT,
        ((flush_db_t*)sw_state_roots_array[unit][FLUSH_MODULE_ID]),
        "flush_db_context[%d]",
        unit);

    DNX_SW_STATE_DIAG_INFO_POST(
        unit,
        DNX_SW_STATE_FLUSH_DB_CONTEXT,
        flush_db_context_info,
        FLUSH_DB_CONTEXT_INFO,
        DNX_SW_STATE_DIAG_ALLOC,
        ((flush_db_t*)sw_state_roots_array[unit][FLUSH_MODULE_ID]),
        sw_state_layout_array[unit][FLUSH_MODULE_ID]);

    DNXC_SW_STATE_FUNC_RETURN;
}



int
flush_db_context_nof_valid_rules_set(int unit, uint32 nof_valid_rules)
{
    DNXC_SW_STATE_INIT_FUNC_DEFS;

    VERIFY_FUNCTION_CALL_ALLOWED(
        unit,
        DNX_SW_STATE_FLUSH_DB_CONTEXT__NOF_VALID_RULES,
        SW_STATE_FUNC_SET,
        DNXC_SW_STATE_NO_FLAGS);

    VERIFY_MODULE_IS_INITIALIZED(
        unit,
        DNX_SW_STATE_FLUSH_DB_CONTEXT__NOF_VALID_RULES,
        sw_state_roots_array[unit][FLUSH_MODULE_ID]);

    DNX_SW_STATE_DIAG_INFO_PRE(
        unit,
        DNX_SW_STATE_FLUSH_DB_CONTEXT__NOF_VALID_RULES,
        DNX_SW_STATE_DIAG_MODIFY);

    DNX_SW_STATE_SET(
        unit,
        DNX_SW_STATE_FLUSH_DB_CONTEXT__NOF_VALID_RULES,
        ((flush_db_t*)sw_state_roots_array[unit][FLUSH_MODULE_ID])->nof_valid_rules,
        nof_valid_rules,
        uint32,
        0,
        "flush_db_context_nof_valid_rules_set");

    DNX_SW_STATE_LOG(
        unit,
        DNX_SW_STATE_SET_LOGGING,
        BSL_LS_SWSTATEDNX_SET,
        DNX_SW_STATE_FLUSH_DB_CONTEXT__NOF_VALID_RULES,
        &nof_valid_rules,
        "flush_db_context[%d]->nof_valid_rules",
        unit);

    DNX_SW_STATE_DIAG_INFO_POST(
        unit,
        DNX_SW_STATE_FLUSH_DB_CONTEXT__NOF_VALID_RULES,
        flush_db_context_info,
        FLUSH_DB_CONTEXT_NOF_VALID_RULES_INFO,
        DNX_SW_STATE_DIAG_MODIFY,
        NULL,
        sw_state_layout_array[unit][FLUSH_MODULE_ID]);

    DNXC_SW_STATE_FUNC_RETURN;
}



int
flush_db_context_nof_valid_rules_get(int unit, uint32 *nof_valid_rules)
{
    DNXC_SW_STATE_INIT_FUNC_DEFS;

    VERIFY_FUNCTION_CALL_ALLOWED(
        unit,
        DNX_SW_STATE_FLUSH_DB_CONTEXT__NOF_VALID_RULES,
        SW_STATE_FUNC_GET,
        DNXC_SW_STATE_NO_FLAGS);

    VERIFY_MODULE_IS_INITIALIZED(
        unit,
        DNX_SW_STATE_FLUSH_DB_CONTEXT__NOF_VALID_RULES,
        sw_state_roots_array[unit][FLUSH_MODULE_ID]);

    DNX_SW_STATE_PTR_NULL_CHECK(
        unit,
        DNX_SW_STATE_FLUSH_DB_CONTEXT__NOF_VALID_RULES,
        nof_valid_rules);

    DNX_SW_STATE_DIAG_INFO_PRE(
        unit,
        DNX_SW_STATE_FLUSH_DB_CONTEXT__NOF_VALID_RULES,
        DNX_SW_STATE_DIAG_READ);

    *nof_valid_rules = ((flush_db_t*)sw_state_roots_array[unit][FLUSH_MODULE_ID])->nof_valid_rules;
    DNX_SW_STATE_LOG(
        unit,
        DNX_SW_STATE_GET_LOGGING,
        BSL_LS_SWSTATEDNX_GET,
        DNX_SW_STATE_FLUSH_DB_CONTEXT__NOF_VALID_RULES,
        &((flush_db_t*)sw_state_roots_array[unit][FLUSH_MODULE_ID])->nof_valid_rules,
        "flush_db_context[%d]->nof_valid_rules",
        unit);

    DNX_SW_STATE_DIAG_INFO_POST(
        unit,
        DNX_SW_STATE_FLUSH_DB_CONTEXT__NOF_VALID_RULES,
        flush_db_context_info,
        FLUSH_DB_CONTEXT_NOF_VALID_RULES_INFO,
        DNX_SW_STATE_DIAG_READ,
        NULL,
        sw_state_layout_array[unit][FLUSH_MODULE_ID]);

    DNXC_SW_STATE_FUNC_RETURN;
}



int
flush_db_context_flush_in_bulk_set(int unit, uint8 flush_in_bulk)
{
    DNXC_SW_STATE_INIT_FUNC_DEFS;

    VERIFY_FUNCTION_CALL_ALLOWED(
        unit,
        DNX_SW_STATE_FLUSH_DB_CONTEXT__FLUSH_IN_BULK,
        SW_STATE_FUNC_SET,
        DNXC_SW_STATE_NO_FLAGS);

    VERIFY_MODULE_IS_INITIALIZED(
        unit,
        DNX_SW_STATE_FLUSH_DB_CONTEXT__FLUSH_IN_BULK,
        sw_state_roots_array[unit][FLUSH_MODULE_ID]);

    DNX_SW_STATE_DIAG_INFO_PRE(
        unit,
        DNX_SW_STATE_FLUSH_DB_CONTEXT__FLUSH_IN_BULK,
        DNX_SW_STATE_DIAG_MODIFY);

    DNX_SW_STATE_SET(
        unit,
        DNX_SW_STATE_FLUSH_DB_CONTEXT__FLUSH_IN_BULK,
        ((flush_db_t*)sw_state_roots_array[unit][FLUSH_MODULE_ID])->flush_in_bulk,
        flush_in_bulk,
        uint8,
        0,
        "flush_db_context_flush_in_bulk_set");

    DNX_SW_STATE_LOG(
        unit,
        DNX_SW_STATE_SET_LOGGING,
        BSL_LS_SWSTATEDNX_SET,
        DNX_SW_STATE_FLUSH_DB_CONTEXT__FLUSH_IN_BULK,
        &flush_in_bulk,
        "flush_db_context[%d]->flush_in_bulk",
        unit);

    DNX_SW_STATE_DIAG_INFO_POST(
        unit,
        DNX_SW_STATE_FLUSH_DB_CONTEXT__FLUSH_IN_BULK,
        flush_db_context_info,
        FLUSH_DB_CONTEXT_FLUSH_IN_BULK_INFO,
        DNX_SW_STATE_DIAG_MODIFY,
        NULL,
        sw_state_layout_array[unit][FLUSH_MODULE_ID]);

    DNXC_SW_STATE_FUNC_RETURN;
}



int
flush_db_context_flush_in_bulk_get(int unit, uint8 *flush_in_bulk)
{
    DNXC_SW_STATE_INIT_FUNC_DEFS;

    VERIFY_FUNCTION_CALL_ALLOWED(
        unit,
        DNX_SW_STATE_FLUSH_DB_CONTEXT__FLUSH_IN_BULK,
        SW_STATE_FUNC_GET,
        DNXC_SW_STATE_NO_FLAGS);

    VERIFY_MODULE_IS_INITIALIZED(
        unit,
        DNX_SW_STATE_FLUSH_DB_CONTEXT__FLUSH_IN_BULK,
        sw_state_roots_array[unit][FLUSH_MODULE_ID]);

    DNX_SW_STATE_PTR_NULL_CHECK(
        unit,
        DNX_SW_STATE_FLUSH_DB_CONTEXT__FLUSH_IN_BULK,
        flush_in_bulk);

    DNX_SW_STATE_DIAG_INFO_PRE(
        unit,
        DNX_SW_STATE_FLUSH_DB_CONTEXT__FLUSH_IN_BULK,
        DNX_SW_STATE_DIAG_READ);

    *flush_in_bulk = ((flush_db_t*)sw_state_roots_array[unit][FLUSH_MODULE_ID])->flush_in_bulk;
    DNX_SW_STATE_LOG(
        unit,
        DNX_SW_STATE_GET_LOGGING,
        BSL_LS_SWSTATEDNX_GET,
        DNX_SW_STATE_FLUSH_DB_CONTEXT__FLUSH_IN_BULK,
        &((flush_db_t*)sw_state_roots_array[unit][FLUSH_MODULE_ID])->flush_in_bulk,
        "flush_db_context[%d]->flush_in_bulk",
        unit);

    DNX_SW_STATE_DIAG_INFO_POST(
        unit,
        DNX_SW_STATE_FLUSH_DB_CONTEXT__FLUSH_IN_BULK,
        flush_db_context_info,
        FLUSH_DB_CONTEXT_FLUSH_IN_BULK_INFO,
        DNX_SW_STATE_DIAG_READ,
        NULL,
        sw_state_layout_array[unit][FLUSH_MODULE_ID]);

    DNXC_SW_STATE_FUNC_RETURN;
}




flush_db_context_cbs flush_db_context = 	{
	
	flush_db_context_is_init,
	flush_db_context_init,
		{
		
		flush_db_context_nof_valid_rules_set,
		flush_db_context_nof_valid_rules_get}
	,
		{
		
		flush_db_context_flush_in_bulk_set,
		flush_db_context_flush_in_bulk_get}
	}
;
#undef BSL_LOG_MODULE
