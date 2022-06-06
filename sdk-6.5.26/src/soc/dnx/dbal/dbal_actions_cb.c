/** \file dbal_actions_cb.c
 * 
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2022 Broadcom Inc. All rights reserved.
 *
 * cb for other modules to use before and after accessing the HW
 *
 */
#ifdef BSL_LOG_MODULE
#error "BSL_LOG_MODULE redefined"
#endif
#define BSL_LOG_MODULE BSL_LS_SOCDNX_DBALDNX

#include "dbal_internal.h"

#include <soc/dnxc/dnxc_ha.h>
#include <soc/dnxc/multithread_analyzer.h>
#include <soc/dnx/dnx_err_recovery_manager.h>
#include <soc/dnx/dnx_state_snapshot_manager.h>
#include <soc/dnx/dnx_err_recovery_manager_common.h>
#include <soc/dnx/dbal/dbal_journal.h>

shr_error_e
dbal_action_access_error_recovery_invalidate(
    int unit,
    dbal_action_access_func_e action_access_func)
{
    SHR_FUNC_INIT_VARS(unit);

    switch (action_access_func)
    {
        case DBAL_ACTION_ACCESS_FUNC_TABLE_CLEAR:
            if (_SHR_E_NONE != dnx_err_recovery_transaction_invalidate(unit))
            {
                SHR_ERR_EXIT(_SHR_E_INTERNAL, "dbal table clear not supported in Error Recovery transactions!\n");
            }
            break;
        default:
            SHR_ERR_EXIT(_SHR_E_INTERNAL, "Function cannot invalidate the Error Recovery transaction!\n");
    }

    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}

shr_error_e
dbal_action_access_pre_access(
    int unit,
    dbal_entry_handle_t * entry_handle,
    dbal_action_access_type_e action_access_type,
    dbal_action_access_func_e action_access_func)
{
    SHR_FUNC_INIT_VARS(unit);

    if (action_access_type == DBAL_ACTION_ACCESS_TYPE_WRITE)
    {

        SHR_BITSET(entry_handle->table->indications_bm, DBAL_TABLE_IND_IS_TABLE_DIRTY);
    }

    if (entry_handle->er_flags & DNX_DBAL_JOURNAL_ROLLBACK_FLAG
        || entry_handle->er_flags & DNX_DBAL_JOURNAL_COMPARE_FLAG)
    {
        SHR_IF_ERR_EXIT(dnx_err_recovery_dbal_access_region_start(unit));
    }

    if (dnxc_ha_is_access_disabled(unit, UTILEX_SEQ_ALLOW_DBAL))
    {
        SHR_ERR_EXIT(_SHR_E_INTERNAL, "It is not allowed to access DBAL during deinit or warmboot\n");
    }

    DNXC_MTA(dnxc_multithread_analyzer_mark_dbal_region(unit, TRUE));

exit:
    SHR_FUNC_EXIT;
}

shr_error_e
dbal_action_access_post_access(
    int unit,
    dbal_entry_handle_t * entry_handle,
    dbal_entry_handle_t * get_entry_handle,
    dbal_action_access_type_e action_access_type,
    dbal_action_access_func_e action_access_func,
    int rv)
{
    uint32 *key_fields_range_bkp = entry_handle->key_field_ranges;
    uint8 nof_ranged_fields_bkp = entry_handle->nof_ranged_fields;
    uint32 *get_key_fields_range_bkp = NULL;
    uint8 get_nof_ranged_fields_bkp = 0;

    SHR_FUNC_INIT_VARS(unit);

    DNXC_MTA(dnxc_multithread_analyzer_mark_dbal_region(unit, FALSE));

    if (get_entry_handle != NULL)
    {
        get_key_fields_range_bkp = get_entry_handle->key_field_ranges;
        get_nof_ranged_fields_bkp = get_entry_handle->nof_ranged_fields;
    }

    switch (action_access_func)
    {
        case DBAL_ACTION_ACCESS_FUNC_ENTRY_COMMIT:
        {
            int journal_log_add = 1;
            if (DBAL_TABLE_IS_NONE_DIRECT(entry_handle->table))
            {

                if ((entry_handle->phy_entry.indirect_commit_mode == DBAL_INDIRECT_COMMIT_MODE_NORMAL) &&
                    (rv == _SHR_E_EXISTS))
                {
                    journal_log_add = 0;
                }

                if ((entry_handle->phy_entry.indirect_commit_mode == DBAL_INDIRECT_COMMIT_MODE_UPDATE) &&
                    (rv == _SHR_E_NOT_FOUND))
                {
                    journal_log_add = 0;
                }
            }

            if (journal_log_add && ((rv != _SHR_E_FULL) && (rv != _SHR_E_ACCESS)))
            {

                entry_handle->key_field_ranges = NULL;
                entry_handle->nof_ranged_fields = 0;
                if (get_entry_handle != NULL)
                {
                    get_entry_handle->key_field_ranges = NULL;
                    get_entry_handle->nof_ranged_fields = 0;
                }
                DNX_DBAL_JRNL(SHR_IF_ERR_EXIT
                              (dnx_dbal_journal_log_add(unit, get_entry_handle, entry_handle, _SHR_E_NONE != rv)));
                entry_handle->key_field_ranges = key_fields_range_bkp;
                entry_handle->nof_ranged_fields = nof_ranged_fields_bkp;
                if (get_entry_handle != NULL)
                {
                    get_entry_handle->key_field_ranges = get_key_fields_range_bkp;
                    get_entry_handle->nof_ranged_fields = get_nof_ranged_fields_bkp;
                }
            }
            DNXC_MTA(SHR_IF_ERR_EXIT
                     (dnxc_multithread_analyzer_log_resource_use
                      (unit, MTA_RESOURCE_DBAL, entry_handle->table_id, TRUE)));
        }
            break;
        case DBAL_ACTION_ACCESS_FUNC_ENTRY_GET:
            DNXC_MTA(SHR_IF_ERR_EXIT
                     (dnxc_multithread_analyzer_log_resource_use
                      (unit, MTA_RESOURCE_DBAL, entry_handle->table_id, FALSE)));
            break;
        case DBAL_ACTION_ACCESS_FUNC_ENTRY_CLEAR:

            if (rv != _SHR_E_ACCESS)
            {
                DNX_DBAL_JRNL(SHR_IF_ERR_EXIT(dnx_dbal_journal_log_clear(unit, get_entry_handle, _SHR_E_NONE != rv)));
            }
            DNXC_MTA(SHR_IF_ERR_EXIT
                     (dnxc_multithread_analyzer_log_resource_use
                      (unit, MTA_RESOURCE_DBAL, entry_handle->table_id, TRUE)));
            break;
        case DBAL_ACTION_ACCESS_FUNC_TABLE_CLEAR:
            DNX_DBAL_JRNL(SHR_IF_ERR_EXIT
                          (dbal_action_access_error_recovery_invalidate(unit, DBAL_ACTION_ACCESS_FUNC_TABLE_CLEAR)));
            break;
        default:
            break;
    }

    SHR_EXIT();
exit:
    if (entry_handle->er_flags & DNX_DBAL_JOURNAL_ROLLBACK_FLAG
        || entry_handle->er_flags & DNX_DBAL_JOURNAL_COMPARE_FLAG)
    {
        dnx_err_recovery_dbal_access_region_end(unit);
    }

    SHR_FUNC_EXIT;
}
