/** \file crps_ctrs_db.c
 * 
 *  crps module gather all functionality that related to the counter/statisitc processor driver for DNX.
 *  crps_ctrs_db.c responsible of the counters memory space database. In allocate/free sw counters memory set values in the memory and read from it.
 *  It is internal crps file . (do not have interfaces with other modules)
 *  THis file implemantion can be modified by customers who want to replace the sw counters memory handling/management
 */

/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2021 Broadcom Inc. All rights reserved.
 */
#ifdef BSL_LOG_MODULE
#error "BSL_LOG_MODULE redefined"
#endif
#define BSL_LOG_MODULE BSL_LS_BCMDNX_CNT

/*
 * Include files.
 * {
 */
#include <shared/shrextend/shrextend_debug.h>
#include <shared/utilex/utilex_bitstream.h>
#include <bcm/types.h>
#include <bcm/error.h>
#include <bcm/stat.h>
#include <soc/dnx/dnx_data/auto_generated/dnx_data_device.h>
#include <soc/dnx/dnx_data/auto_generated/dnx_data_crps.h>
#include <bcm_int/dnx/algo/swstate/auto_generated/access/crps_access.h>
#include <include/shared/utilex/utilex_integer_arithmetic.h>
#include <include/bcm_int/dnx/cmn/dnxcmn.h>
#include <include/bcm_int/dnx/algo/crps/crps_ctrs_db.h>
#include <bcm_int/dnx/stat/crps/crps_mgmt.h>

/*
 * }
 */

/*
 * DEFINES
 * {
 */

/*
 * }
 */
/*
 * MACROs
 * {
 */

/*
 * }
 */

static shr_error_e
dnx_crps_max_nof_sub_counters_per_entry_get(
    int unit,
    int *nof_sub_countes_per_entry)
{
    bcm_stat_counter_format_type_t format_type;
    int max_nof_sub_counters_per_entry = 0, nof_sub_counters = 0;
    int current_nof_counters_per_entry = 0, current_nof_sub_counters_per_entry = 0;

    SHR_FUNC_INIT_VARS(unit);

    for (format_type = bcmStatCounterFormatPacketsAndBytes; format_type < bcmStatCounterFormatCount; format_type++)
    {
        /** Skip formats that can't perform eviction to the host */
        if (!dnx_data_crps.engine.counter_format_types_get(unit, format_type)->valid ||
            !dnx_crps_mgmt_eviction_destination_for_format_type_is_supported(unit, format_type,
                                                                             bcmStatEvictionDestinationLocalHost))
        {
            continue;
        }

        current_nof_counters_per_entry =
            dnx_data_crps.engine.counter_format_types_get(unit, format_type)->nof_counters_per_entry;

        nof_sub_counters = DNX_CRPS_MGMT_NOF_SUB_COUNT(unit, format_type);

        current_nof_sub_counters_per_entry = current_nof_counters_per_entry * nof_sub_counters;

        if (current_nof_sub_counters_per_entry > max_nof_sub_counters_per_entry)
        {
            max_nof_sub_counters_per_entry = current_nof_sub_counters_per_entry;
        }
    }

    *nof_sub_countes_per_entry = max_nof_sub_counters_per_entry;

    SHR_FUNC_EXIT;
}

/** see .h file */
shr_error_e
dnx_crps_ctrs_db_init(
    int unit)
{
    uint8 is_alloc;
    int core_id, nof_engines, engine_id;
    int max_nof_sub_counters_per_entry, nof_sub_counters_per_engine;
    const dnx_data_crps_engine_engines_info_t *engine_info;

    SHR_FUNC_INIT_VARS(unit);

    nof_engines = dnx_data_crps.engine.nof_engines_get(unit);
    SHR_IF_ERR_EXIT(dnx_crps_max_nof_sub_counters_per_entry_get(unit, &max_nof_sub_counters_per_entry));

    DNXCMN_CORES_ITER(unit, BCM_CORE_ALL, core_id)
    {
        /** for each engine allocate array of SW counters, according to the engine_size*SUB_COUNT. */
        /** driver holds 2 uint64 counters for each HW counter (one for packet and one for bytes */
        for (engine_id = 0; engine_id < nof_engines; engine_id++)
        {
            SHR_IF_ERR_EXIT(dnx_crps_db.proc.counter.is_allocated(unit, core_id, engine_id, &is_alloc));
            if (is_alloc == FALSE)
            {
                engine_info = dnx_data_crps.engine.engines_info_get(unit, engine_id);
                /** The maximum number of counters per entry will be always for single counter type */
                nof_sub_counters_per_engine = engine_info->nof_entries * max_nof_sub_counters_per_entry;
                SHR_IF_ERR_EXIT(dnx_crps_db.proc.counter.alloc(unit, core_id, engine_id, nof_sub_counters_per_engine));
            }
            else
            {
                SHR_ERR_EXIT(_SHR_E_INIT,
                             "dnx_crps_db.proc.counter.is_allocated=TRUE for unit=%d, core=%d, engine=%d\n",
                             unit, core_id, engine_id);
            }
        }
    }
exit:
    SHR_FUNC_EXIT;
}

/** see .h file */
shr_error_e
dnx_crps_ctrs_db_deinit(
    int unit)
{
    SHR_FUNC_INIT_VARS(unit);

    /*
     * sw state memory free is done automatically at device deinit 
     */

    SHR_FUNC_EXIT;
}

/** see .h file */
shr_error_e
dnx_crps_ctrs_db_counter_set(
    int unit,
    int flags,
    int core_id,
    int engine_id,
    int counter_id,
    int sub_counter,
    bcm_stat_counter_format_type_t format,
    uint64 value)
{
    int shadow_idx;
    uint64 old_val;
    uint64 counter;
    SHR_FUNC_INIT_VARS(unit);

    shadow_idx = counter_id * DNX_CRPS_MGMT_SUB_COUNT;
    SHR_IF_ERR_EXIT(dnx_crps_db.proc.counter.get(unit, core_id, engine_id, (shadow_idx + sub_counter), &old_val));
    /** update the counter with new value */
    if (_SHR_IS_FLAG_SET(flags, DNX_CRPS_CTRS_OVERWRITE_COUNTER_VALUE))
    {
        /** update counter into sw state */
        SHR_IF_ERR_EXIT(dnx_crps_db.proc.counter.set(unit, core_id, engine_id, (shadow_idx + sub_counter), value));
    }
    /** increase counter */
    else
    {
        if (format == bcmStatCounterFormatMaxSize)
        {
            if (COMPILER_64_GT(value, old_val))
            {
                /** update counter into sw state */
                SHR_IF_ERR_EXIT(dnx_crps_db.proc.
                                counter.set(unit, core_id, engine_id, (shadow_idx + sub_counter), value));
            }
        }
        else
        {
            COMPILER_64_ZERO(counter);
            COMPILER_64_ADD_64(counter, old_val);
            COMPILER_64_ADD_64(counter, value);
            if (COMPILER_64_LT(counter, old_val))
            {
                /** dont allow wrap; saturate instead */
                COMPILER_64_ALLONES(counter);
            }
            /** update counter into sw state */
            SHR_IF_ERR_EXIT(dnx_crps_db.proc.
                            counter.set(unit, core_id, engine_id, (shadow_idx + sub_counter), counter));
        }
    }

exit:
    SHR_FUNC_EXIT;
}

/** see .h file */
shr_error_e
dnx_crps_ctrs_db_counter_get(
    int unit,
    int flags,
    int core_id,
    int engine_id,
    int counter_id,
    int sub_counter,
    uint64 *value)
{
    int ofs;
    uint64 work_reg_64;

    SHR_FUNC_INIT_VARS(unit);

    ofs = counter_id * DNX_CRPS_MGMT_SUB_COUNT;
    SHR_IF_ERR_EXIT(dnx_crps_db.proc.counter.get(unit, core_id, engine_id, (ofs + sub_counter), value));
    /*
     * if flag clear on read is set - after the value is get,
     *  clear the counter
     */
    COMPILER_64_ZERO(work_reg_64);
    if (_SHR_IS_FLAG_SET(flags, BCM_STAT_COUNTER_CLEAR_ON_READ))
    {
        SHR_IF_ERR_EXIT(dnx_crps_db.proc.counter.set(unit, core_id, engine_id, (ofs + sub_counter), work_reg_64));
    }

exit:
    SHR_FUNC_EXIT;

}

/** see .h file */
shr_error_e
dnx_crps_ctrs_db_counter_clear(
    int unit,
    int core_id,
    int engine_id)
{
    int counter_index, nof_sub_counters_per_entry;
    uint8 is_alloc;
    const dnx_data_crps_engine_engines_info_t *engine_info;
    uint64 work_reg_64;
    SHR_FUNC_INIT_VARS(unit);

    SHR_IF_ERR_EXIT(dnx_crps_db.proc.counter.is_allocated(unit, core_id, engine_id, &is_alloc));
    /** check that memory is allocated */
    if (is_alloc == FALSE)
    {
        SHR_ERR_EXIT(_SHR_E_INTERNAL,
                     "dnx_crps_db.proc.counter.is_allocated=FALSE for unit=%d, core=%d, engine=%d\n",
                     unit, core_id, engine_id);
    }

    SHR_IF_ERR_EXIT(dnx_crps_max_nof_sub_counters_per_entry_get(unit, &nof_sub_counters_per_entry));
    engine_info = dnx_data_crps.engine.engines_info_get(unit, engine_id);

    /** run over all counters for this engine and set each one to zero */
    COMPILER_64_ZERO(work_reg_64);
    for (counter_index = 0; counter_index < (engine_info->nof_entries * nof_sub_counters_per_entry); counter_index++)
    {
        SHR_IF_ERR_EXIT(dnx_crps_db.proc.counter.set(unit, core_id, engine_id, counter_index, work_reg_64));
    }

exit:
    SHR_FUNC_EXIT;
}
