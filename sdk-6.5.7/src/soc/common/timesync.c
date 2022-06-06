/*
 * $Id: timesync.c,v 1.3 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * StrataSwitch port control API
 */
#include <shared/bsl.h>

#include <sal/core/libc.h>
#include <shared/alloc.h>
#include <sal/core/spl.h>

#include <soc/drv.h>
#include <soc/phy/drv.h>
#include <soc/debug.h>
#include <soc/timesync.h>

/*
 * Function:
 *      soc_port_phy_timesync_config_set
 * Purpose:
 *      Update PHY's phy_timesync config
 * Parameters:
 *      unit - SOC Unit #.
 *      port - Port number.
 *      conf - Config
 * Returns:
 *      SOC_E_XXX
 */
int
soc_port_phy_timesync_config_set(int unit, soc_port_t port, soc_port_phy_timesync_config_t *conf)
{
    int         rv;
    phy_ctrl_t *pc;

    rv = SOC_E_UNAVAIL;

    pc = EXT_PHY_SW_STATE(unit, port);

    if (NULL != pc) {
        INT_MCU_LOCK(unit);
        rv = PHY_TIMESYNC_CONFIG_SET(pc->pd, unit, port, conf);
        INT_MCU_UNLOCK(unit);
    }

    if (SOC_FAILURE(rv)) {
        LOG_WARN(BSL_LS_SOC_COMMON,
                 (BSL_META_U(unit,
                             "soc_port_phy_timesync_config_set failed %d\n"), rv));
    }
    return rv;
}

/*
 * Function:
 *      soc_port_phy_timesync_config_get
 * Purpose:
 *      Retrive PHY's phy_timesync config
 * Parameters:
 *      unit - SOC Unit #.
 *      port - Port number.
 *      conf - Config
 * Returns:
 *      SOC_E_XXX
 */
int
soc_port_phy_timesync_config_get(int unit, soc_port_t port, soc_port_phy_timesync_config_t *conf)
{
    int         rv;
    phy_ctrl_t *pc;

    rv = SOC_E_UNAVAIL;

    pc = EXT_PHY_SW_STATE(unit, port);

    if (NULL != pc) {
        INT_MCU_LOCK(unit);
        rv = PHY_TIMESYNC_CONFIG_GET(pc->pd, unit, port, conf);
        INT_MCU_UNLOCK(unit);
    }

    if (SOC_FAILURE(rv)) {
        LOG_WARN(BSL_LS_SOC_COMMON,
                 (BSL_META_U(unit,
                             "soc_port_phy_timesync_config_get failed %d\n"), rv));
    }
    return rv;
}

/*
 * Function:
 *      soc_port_control_phy_timesync_set
 * Purpose:
 *      Update PHY's phy_timesync parameter
 * Parameters:
 *      unit - SOC Unit #.
 *      port - Port number.
 *      type - operation
 *      value - parameter
 * Returns:
 *      SOC_E_XXX
 */
int
soc_port_control_phy_timesync_set(int unit, soc_port_t port, soc_port_control_phy_timesync_t type, uint64 value)
{
    int         rv;
    phy_ctrl_t *pc;

    rv = SOC_E_UNAVAIL;

    pc = EXT_PHY_SW_STATE(unit, port);

    if (NULL != pc) {
        INT_MCU_LOCK(unit);
        rv = PHY_TIMESYNC_CONTROL_SET(pc->pd, unit, port, type, value);
        INT_MCU_UNLOCK(unit);
    }

    if (SOC_FAILURE(rv)) {
        LOG_WARN(BSL_LS_SOC_COMMON,
                 (BSL_META_U(unit,
                             "soc_port_control_phy_timesync_set failed %d\n"), rv));
    }
    return rv;
}

/*
 * Function:
 *      soc_port_control_phy_timesync_get
 * Purpose:
 *      Retrive PHY's phy_timesync parameter
 * Parameters:
 *      unit - SOC Unit #.
 *      port - Port number.
 *      type - operation
 *      value - *parameter
 * Returns:
 *      SOC_E_XXX
 */
int
soc_port_control_phy_timesync_get(int unit, soc_port_t port, soc_port_control_phy_timesync_t type, uint64 *value)
{
    int         rv;
    phy_ctrl_t *pc;

    rv = SOC_E_UNAVAIL;

    pc = EXT_PHY_SW_STATE(unit, port);

    if (NULL != pc) {
        INT_MCU_LOCK(unit);
        rv = PHY_TIMESYNC_CONTROL_GET(pc->pd, unit, port, type, value);
        INT_MCU_UNLOCK(unit);
    }

    if (SOC_FAILURE(rv)) {
        LOG_WARN(BSL_LS_SOC_COMMON,
                 (BSL_META_U(unit,
                             "soc_port_control_phy_timesync_get failed %d\n"), rv));
    }
    return rv;
}

/*
 * Function:
 *      soc_port_phy_timesync_enhanced_capture_get
 * Purpose:
 *      Retrive PHY's phy_timesync parameter
 * Parameters:
 *      unit - SOC Unit #.
 *      port - Port number.
 *      type - operation
 *      value - *parameter
 * Returns:
 *      SOC_E_XXX
 */
int
soc_port_phy_timesync_enhanced_capture_get(int unit, soc_port_t port, soc_port_phy_timesync_enhanced_capture_t *value)
{
    int         rv;
    phy_ctrl_t *pc;

    rv = SOC_E_UNAVAIL;

    pc = EXT_PHY_SW_STATE(unit, port);

    if (NULL != pc) {
        INT_MCU_LOCK(unit);
        rv = PHY_TIMESYNC_ENHANCED_CAPTURE_GET(pc->pd, unit, port, value);
        INT_MCU_UNLOCK(unit);
    }

    if (SOC_FAILURE(rv)) {
        LOG_WARN(BSL_LS_SOC_COMMON,
                 (BSL_META_U(unit,
                             "soc_port_phy_timesync_enhanced_capture_get failed %d\n"), rv));
    }
    return rv;
}
