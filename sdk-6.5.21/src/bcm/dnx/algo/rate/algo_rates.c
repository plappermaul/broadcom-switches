/** \file algo/rate/algo_rates.c
 *
 * Implementation for shapers rate calculations.
 */
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

#ifdef BSL_LOG_MODULE
#error "BSL_LOG_MODULE redefined"
#endif
#define BSL_LOG_MODULE BSL_LS_BCMDNX_COSQ

/**
* Include files:
* {
*/

#include <shared/shrextend/shrextend_debug.h>
#include <sal/compiler.h>
#include <bcm_int/dnx/algo/rate/algo_rates.h>
#include <soc/dnx/dnx_data/auto_generated/dnx_data_device.h>
#include <shared/utilex/utilex_integer_arithmetic.h>

/** 
 * } 
 */

/**
* Defines:
* {
*/

/* 
 * The first value of 'credit' when searching for a matching 'credit' 
 * and 'interval' for 'rate'.
 */
#define DNX_ALGO_RATES_CREDIT_VAL_FIRST   1024

/** 
 * } 
 */

/**
* Functions:
* {
*/

/**
 * \brief
 *   Given credit size in Bytes, convert a shaper's rate given in Kbits/sec units
 *   to an interval between 2 consecutive credits in device clocks.
 * 
 *  \param [in] unit -
 *    The unit id.
 *  \param [in] credit -
 *    The credit size [Bytes].
 *  \param [in] rate -
 *      The shaper's rate [KBytes/Sec].
 *  \param [out] interval -
 *      Interval between credits [clocks].
 *  \see
 *    * dnx_algo_rates_credit_interval_param_get
 */
static shr_error_e
dnx_algo_rates_credit_rate_to_interval_get(
    int unit,
    uint32 credit,
    uint32 rate,
    uint32 *interval)
{
    uint64 result;
    SHR_FUNC_INIT_VARS(unit);

    if (0 == rate)
    {
        /** Divide by zero */
        SHR_ERR_EXIT(_SHR_E_INTERNAL, "rate can't be 0.");
    }

    /** The calculation */
    COMPILER_64_SET(result, 0, credit);
    COMPILER_64_UMUL_32(result, dnx_data_device.general.core_clock_khz_get(unit));
    COMPILER_64_UDIV_32(result, rate);

    if (COMPILER_64_HI(result) != 0)
    {
        /** Overflow */
        SHR_ERR_EXIT(_SHR_E_INTERNAL, "Computation overflow.");
    }

    *interval = COMPILER_64_LO(result);

exit:
    SHR_FUNC_EXIT;
}

/**
 * \brief
 *   Given credit size in Bytes, convert an interval between 2 consecutive
 *   credits in device clocks to a shaper's rate given in Kbits/sec units.
 *
 *  \param [in] unit -
 *    The unit id.
 *  \param [in] credit -
 *    The credit size [Bytes].
 *  \param [in] interval -
 *      Interval between credits [clocks].
 *  \param [out] rate -
 *      The shaper's rate [KBytes/Sec].
 *  \see
 *    * dnx_algo_rates_credit_interval_param_get
 */
static shr_error_e
dnx_algo_rates_credit_interval_to_rate_get(
    int unit,
    uint32 credit,
    uint32 interval,
    uint32 *rate)
{
    uint64 result;
    SHR_FUNC_INIT_VARS(unit);

    if (0 == interval)
    {
        /** Divide by zero */
        SHR_ERR_EXIT(_SHR_E_INTERNAL, "interval can't be 0.");
    }

    /** The calculation */
    COMPILER_64_SET(result, 0, credit);
    COMPILER_64_UMUL_32(result, dnx_data_device.general.core_clock_khz_get(unit));
    COMPILER_64_UDIV_32(result, interval);

    if (COMPILER_64_HI(result) != 0)
    {
        /** Overflow */
        SHR_ERR_EXIT(_SHR_E_INTERNAL, "Computation overflow.");
    }

    *rate = COMPILER_64_LO(result);

exit:
    SHR_FUNC_EXIT;
}

/**
 * See algo_rates.h
 */
shr_error_e
dnx_algo_rates_kbits_per_sec_to_clocks(
    int unit,
    uint32 rate,
    uint32 credit,
    uint32 *interval)
{
    SHR_FUNC_INIT_VARS(unit);

    /** internal function gets rate in KBytes/sec */
    SHR_IF_ERR_EXIT(dnx_algo_rates_credit_rate_to_interval_get(unit, credit, rate / UTILEX_NOF_BITS_IN_CHAR, interval));
exit:
    SHR_FUNC_EXIT;
}

/*
 * See algo_rates.h
 */
shr_error_e
dnx_algo_rates_clocks_to_kbits_per_sec(
    int unit,
    uint32 interval,
    uint32 credit,
    uint32 *rate)
{
    uint32 kbytes_per_sec;

    SHR_FUNC_INIT_VARS(unit);

    /** internal function returns rate in KBytes/sec */
    SHR_IF_ERR_EXIT(dnx_algo_rates_credit_interval_to_rate_get(unit, credit, interval, &kbytes_per_sec));
    *rate = kbytes_per_sec * UTILEX_NOF_BITS_IN_CHAR;

exit:
    SHR_FUNC_EXIT;

}

/** See .h file */
shr_error_e
dnx_algo_rates_rate_to_credit_interval(
    int unit,
    uint32 shaper_rate,
    dnx_algo_rates_shaper_bounds_t * shaper_bounds,
    uint32 interval_resolution,
    uint32 credit_resolution,
    uint32 *shaper_interval,
    uint32 *shaper_credit)
{
    uint32 interval_value = 0;
    uint32 interval_value_in_clocks = 0;
    uint32 credit_value = 0;
    uint32 current_rate = 0;
    uint32 abs = 0;
    uint32 min_abs = shaper_rate;
    uint32 min_credit_value = shaper_bounds->credit_min;
    uint32 max_credit_value = shaper_bounds->credit_max;
    uint32 closest_credit_value = shaper_bounds->credit_min;
    uint32 closest_interval_value = shaper_bounds->interval_max;
    SHR_FUNC_INIT_VARS(unit);

    if (interval_resolution < 1)
    {
        SHR_ERR_EXIT(_SHR_E_INTERNAL, "Interval resolution must be at least 1 clock");
    }

    if (credit_resolution < 1)
    {
        SHR_ERR_EXIT(_SHR_E_INTERNAL, "Credit resolution must be at least 1 byte");
    }

    /*
     * Skip calculations when shaper_rate is 0.
     */
    if (0 == shaper_rate)
    {
        *shaper_credit = shaper_bounds->credit_min;
        *shaper_interval = shaper_bounds->interval_max;
        SHR_EXIT();
    }

    /*
     * Find matching calendar and delay that will result in the
     * required shaper's shaper_rate.
     */
    for (credit_value = min_credit_value; credit_value <= max_credit_value; ++credit_value)
    {
        /*
         * Get interval for chosen credit
         */
        SHR_IF_ERR_EXIT(dnx_algo_rates_credit_rate_to_interval_get
                        (unit, credit_value * credit_resolution, shaper_rate, &interval_value_in_clocks));
        interval_value = interval_value_in_clocks / interval_resolution;

        if (interval_value > shaper_bounds->interval_max)
        {
            /*
             * Since credit_value is getting higher in every loop, and
             * current interval is too high, it means that consequent
             * intervals will also be too high, so we can stop the loop.
             */
            break;
        }

        if (interval_value >= shaper_bounds->interval_min)
        {
            /*
             * Get current shaper_rate according to the calculated 'credit_value' and 'interval_value'.
             */
            SHR_IF_ERR_EXIT(dnx_algo_rates_credit_interval_to_rate_get
                            (unit, credit_value * credit_resolution, interval_value * interval_resolution,
                             &current_rate));

            /*
             * Save closest results
             */
            abs = UTILEX_DELTA(current_rate, shaper_rate);
            if (abs < min_abs)
            {
                min_abs = abs;
                closest_credit_value = credit_value;
                closest_interval_value = interval_value;
            }

            if (current_rate == shaper_rate)
            {
                /** Found credit and interval */
                break;
            }
        }
    }

    /*
     * Use closest results
     */
    *shaper_credit = closest_credit_value;
    *shaper_interval = closest_interval_value;

exit:
    SHR_FUNC_EXIT;
}

/** See .h file */
shr_error_e
dnx_algo_rates_credit_interval_to_rate(
    int unit,
    uint32 credit_resolution,
    uint32 interval_resolution,
    uint32 shaper_interval,
    uint32 shaper_credit,
    uint32 *shaper_rate)
{
    SHR_FUNC_INIT_VARS(unit);

    if (interval_resolution < 1)
    {
        SHR_ERR_EXIT(_SHR_E_INTERNAL, "Interval resolution must be at least 1 clock");
    }

    if (credit_resolution < 1)
    {
        SHR_ERR_EXIT(_SHR_E_INTERNAL, "Credit resolution must be at least 1 byte");
    }

    SHR_IF_ERR_EXIT(dnx_algo_rates_credit_interval_to_rate_get
                    (unit, shaper_credit * credit_resolution, shaper_interval * interval_resolution, shaper_rate));

exit:
    SHR_FUNC_EXIT;
}

/**
 * }
 */
