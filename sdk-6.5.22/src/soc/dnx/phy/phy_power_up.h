#ifndef PHY_POWER_UP_INTERNAL_H_INCLUDED
#define PHY_POWER_UP_INTERNAL_H_INCLUDED

#include <shared/shrextend/shrextend_debug.h>

shr_error_e dnx_phy_nif_bh_power_up(
    int unit);

shr_error_e dnx_phy_fabric_bh_power_up(
    int unit);

shr_error_e dnx_phy_nif_falcon_power_up(
    int unit);

#endif
