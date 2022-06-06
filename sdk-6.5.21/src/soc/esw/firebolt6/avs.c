/*
 * 
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 *
 * File:        avs.c
 */

#include <shared/bsl.h>

#ifdef INCLUDE_AVS
#include <soc/avs.h>
#include <soc/firebolt6.h>
#include <soc/bondoptions.h>

STATIC soc_error_t
_soc_fb6_openloop_voltage_get(int unit, uint32 *voltage)
{
    /* unit of voltage is mV */
    if (soc_feature(unit, soc_feature_untethered_otp)) {
        *voltage = SOC_BOND_INFO(unit)->avs_status;
    }

    return SOC_E_NONE;
}

static soc_avs_functions_t _fb6_avs_functions;

soc_error_t
soc_fb6_avs_init(int unit)
{
    /* register avs functions*/
    sal_memset(&_fb6_avs_functions, 0, sizeof(soc_avs_functions_t));
    _fb6_avs_functions.openloop_voltage_get = &_soc_fb6_openloop_voltage_get;
    soc_avs_function_register(unit, &_fb6_avs_functions);

    return SOC_E_NONE;
}

#endif /* INCLUDE_AVS */
