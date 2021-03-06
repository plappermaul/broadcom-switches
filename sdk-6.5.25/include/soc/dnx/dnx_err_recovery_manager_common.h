
/*
 * 
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2021 Broadcom Inc. All rights reserved.
 */

#ifndef _DNX_ERR_RECOVERY_COMMON_MANAGER_H

#define _DNX_ERR_RECOVERY_COMMON_MANAGER_H

#include <soc/dnx/dnx_er_threading.h>
#include <soc/dnx/recovery/rollback_journal_utils.h>

#define DNX_ERR_RECOVERY_COMMON_IS_ON_INITIALIZED          SAL_BIT(0)
#define DNX_ERR_RECOVERY_COMMON_IS_ON_IN_TRANSACTION       SAL_BIT(1)
#define DNX_ERR_RECOVERY_COMMON_IS_ON_NOT_SUPPRESSED       SAL_BIT(2)
#define DNX_ERR_RECOVERY_COMMON_IS_ON_NOT_INVALIDATED      SAL_BIT(3)
#define DNX_ERR_RECOVERY_COMMON_IS_ON_ALL                 (SAL_BIT(4) - 1)

typedef struct dnx_err_recovery_common_manager_d
{

    uint32 bitmap;
} dnx_err_recovery_common_manager_t;

typedef struct dnx_err_recovery_common_root_d
{

    uint32 is_enable;

        dnx_err_recovery_common_manager_t
        managers[DNX_ERR_RECOVERY_MAX_NOF_ACTIVE_TRANSACTION][DNX_ROLLBACK_JOURNAL_TYPE_NOF];

#ifdef DNX_ERR_RECOVERY_VALIDATION_BASIC

    uint8 validation_enabled;
#endif

} dnx_err_recovery_common_root_t;

shr_error_e dnx_err_recovery_common_init(
    int unit);

shr_error_e dnx_err_recovery_common_enable_disable(
    int unit,
    uint32 is_enable);

shr_error_e dnx_err_recovery_common_validation_enable_disable_set(
    int unit,
    uint32 validation_enabled);

uint8 dnx_err_recovery_common_is_enabled(
    int unit);

uint8 dnx_err_recovery_common_is_validation_enabled(
    int unit);

uint8 dnx_err_recovery_common_is_on(
    int unit,
    int er_thread_id,
    dnx_rollback_journal_type_e mngr_id);

uint8 dnx_err_recovery_common_flag_is_on(
    int unit,
    int er_thread_id,
    dnx_rollback_journal_type_e mngr_id,
    uint32 flag);

uint8 dnx_err_recovery_common_flag_mask_is_on(
    int unit,
    int er_thread_id,
    dnx_rollback_journal_type_e mngr_id,
    uint32 mask);

shr_error_e dnx_err_recovery_common_flag_set(
    int unit,
    int er_thread_id,
    dnx_rollback_journal_type_e mngr_id,
    uint32 flag);

shr_error_e dnx_err_recovery_common_flag_clear(
    int unit,
    int er_thread_id,
    dnx_rollback_journal_type_e mngr_id,
    uint32 flag);

#endif
