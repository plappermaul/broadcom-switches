/*
 * This software is governed by the Broadcom Switch APIs license.
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenMDK/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 *
 * PHY driver for BCM5461.
 */

#include <phy/phy.h>
#include <phy/phy_drvlist.h>

#define BCM5461_PHY_ID0                 0x0020
#define BCM5461_PHY_ID1                 0x60c0

#define PHY_ID1_REV_MASK                0x000f

/* Access to shadowed registers at offset 0x1c */
#define REG_1C_SEL(_s)                  ((_s) << 10)
#define REG_1C_WR(_s,_v)                (REG_1C_SEL(_s) | (_v) | 0x8000)

/***********************************************************************
 *
 * PHY DRIVER FUNCTIONS
 */

/*
 * Function:
 *      bcm5461_phy_probe
 * Purpose:     
 *      Probe for 5461 PHY
 * Parameters:
 *      pc - PHY control structure
 * Returns:     
 *      CDK_E_xxx
 */
static int
bcm5461_phy_probe(phy_ctrl_t *pc)
{
    uint32_t phyid0, phyid1;
    int ioerr = 0;

    PHY_CTRL_CHECK(pc);

    ioerr += PHY_BUS_READ(pc, MII_PHY_ID0_REG, &phyid0);
    ioerr += PHY_BUS_READ(pc, MII_PHY_ID1_REG, &phyid1);

    if (phyid0 == BCM5461_PHY_ID0 && 
        (phyid1 & ~PHY_ID1_REV_MASK) == BCM5461_PHY_ID1) {
        return ioerr ? CDK_E_IO : CDK_E_NONE;
    }
    return CDK_E_NOT_FOUND;
}

/*
 * Function:
 *      bcm5461_phy_notify
 * Purpose:     
 *      Handle PHY notifications
 * Parameters:
 *      pc - PHY control structure
 *      event - PHY event
 * Returns:     
 *      CDK_E_xxx
 */
static int
bcm5461_phy_notify(phy_ctrl_t *pc, phy_event_t event)
{
    return bcm5464_drv.pd_notify(pc, event);
}

/*
 * Function:
 *      bcm5461_phy_reset
 * Purpose:     
 *      Reset 5461 PHY
 * Parameters:
 *      pc - PHY control structure
 * Returns:     
 *      CDK_E_xxx
 */
static int
bcm5461_phy_reset(phy_ctrl_t *pc)
{
    return bcm5464_drv.pd_reset(pc);
}

/*
 * Function:
 *      bcm5461_phy_init
 * Purpose:     
 *      Initialize 5461 PHY driver
 * Parameters:
 *      pc - PHY control structure
 * Returns:     
 *      CDK_E_xxx
 */
static int
bcm5461_phy_init(phy_ctrl_t *pc)
{
    int ioerr = 0;
    uint32_t ctrl, ana, tmp;

    PHY_CTRL_CHECK(pc);

    /*
     * The BCM5461 and and BCM5461S (fiber support) cannot be
     * distinguished by PHY ID, however if fiber registers are
     * selected by default, then we can assume we are a BCM5461S.
     * If this is the case, then we still default to GMII copper
     * mode in order to work in designs with no internal SerDes.
     */

    /*
     * The selector field in auto-neg advertisement register
     * is 00001b if copper mode is selected. If not we assume
     * we are a BCM5461S and switch to copper mode. Note that
     * the mode registers are reserved on BCM5461 and should
     * not be accessed.
     */
    ioerr += PHY_BUS_READ(pc, MII_ANA_REG, &ana);
    if ((ana & 0x1f) == 0) {
        /* Select copper mode */
        ioerr += PHY_BUS_WRITE(pc, 0x1c, REG_1C_WR(0x1f, 0x8));

        /* Power up copper interface */
        ioerr += PHY_BUS_READ(pc, MII_CTRL_REG, &ctrl);
        ctrl &= ~MII_CTRL_PD;
        ioerr += PHY_BUS_WRITE(pc, MII_CTRL_REG, ctrl);

        /* Configure auto-medium detect */
        ioerr += PHY_BUS_WRITE(pc, 0x1c, REG_1C_SEL(0x1e));
        ioerr += PHY_BUS_READ(pc, 0x1c, &tmp);
        tmp &= ~0x001f;
        ioerr += PHY_BUS_WRITE(pc, 0x1c, REG_1C_WR(0x1e, tmp));
    }

    return ioerr ? CDK_E_IO : bcm5464_drv.pd_init(pc);
}

/*
 * Function:    
 *      bcm5461_phy_link_get
 * Purpose:     
 *      Determine the current link up/down status
 * Parameters:
 *      pc - PHY control structure
 *      link - (OUT) non-zero indicates link established.
 *      autoneg_done - (OUT) if true, auto-negotiation is complete
 * Returns:
 *      CDK_E_xxx
 */
static int
bcm5461_phy_link_get(phy_ctrl_t *pc, int *link, int *autoneg_done)
{
    return bcm5464_drv.pd_link_get(pc, link, autoneg_done);
}

/*
 * Function:    
 *      bcm5461_phy_duplex_set
 * Purpose:     
 *      Set the current duplex mode (forced).
 * Parameters:
 *      pc - PHY control structure
 *      duplex - non-zero indicates full duplex, zero indicates half
 * Returns:     
 *      CDK_E_xxx
 */
static int
bcm5461_phy_duplex_set(phy_ctrl_t *pc, int duplex)
{
    return bcm5464_drv.pd_duplex_set(pc, duplex);
}

/*
 * Function:    
 *      bcm5461_phy_duplex_get
 * Purpose:     
 *      Get the current operating duplex mode. If autoneg is enabled, 
 *      then operating mode is returned, otherwise forced mode is returned.
 * Parameters:
 *      pc - PHY control structure
 *      duplex - (OUT) non-zero indicates full duplex, zero indicates half
 * Returns:     
 *      CDK_E_xxx
 */
static int
bcm5461_phy_duplex_get(phy_ctrl_t *pc, int *duplex)
{
    return bcm5464_drv.pd_duplex_get(pc, duplex);
}

/*
 * Function:    
 *      bcm5461_phy_speed_set
 * Purpose:     
 *      Set the current operating speed (forced).
 * Parameters:
 *      pc - PHY control structure
 *      speed - new link speed
 * Returns:     
 *      CDK_E_xxx
 */
static int
bcm5461_phy_speed_set(phy_ctrl_t *pc, uint32_t speed)
{
    return bcm5464_drv.pd_speed_set(pc, speed);
}

/*
 * Function:    
 *      bcm5461_phy_speed_get
 * Purpose:     
 *      Get the current operating speed. If autoneg is enabled, 
 *      then operating mode is returned, otherwise forced mode is returned.
 * Parameters:
 *      pc - PHY control structure
 *      speed - (OUT) current link speed
 * Returns:     
 *      CDK_E_xxx
 */
static int
bcm5461_phy_speed_get(phy_ctrl_t *pc, uint32_t *speed)
{
    return bcm5464_drv.pd_speed_get(pc, speed);
}

/*
 * Function:    
 *      bcm5461_phy_autoneg_set
 * Purpose:     
 *      Enable or disabled auto-negotiation on the specified port.
 * Parameters:
 *      pc - PHY control structure
 *      autoneg - non-zero enables autoneg, zero disables autoneg
 * Returns:     
 *      CDK_E_xxx
 */
static int
bcm5461_phy_autoneg_set(phy_ctrl_t *pc, int autoneg)
{
    return bcm5464_drv.pd_autoneg_set(pc, autoneg);
}

/*
 * Function:    
 *      bcm5461_phy_autoneg_get
 * Purpose:     
 *      Get the current auto-negotiation status (enabled/busy)
 * Parameters:
 *      pc - PHY control structure
 *      autoneg - (OUT) non-zero indicates autoneg enabled
 * Returns:     
 *      CDK_E_xxx
 */
static int
bcm5461_phy_autoneg_get(phy_ctrl_t *pc, int *autoneg)
{
    return bcm5464_drv.pd_autoneg_get(pc, autoneg);
}

/*
 * Function:    
 *      bcm5461_phy_loopback_set
 * Purpose:     
 *      Set the internal PHY loopback mode.
 * Parameters:
 *      pc - PHY control structure
 *      enable - non-zero enables PHY loopback
 * Returns:     
 *      CDK_E_xxx
 */
static int
bcm5461_phy_loopback_set(phy_ctrl_t *pc, int enable)
{
    return bcm5464_drv.pd_loopback_set(pc, enable);
}

/*
 * Function:    
 *      bcm5461_phy_loopback_get
 * Purpose:     
 *      Get the local PHY loopback mode.
 * Parameters:
 *      pc - PHY control structure
 *      enable - (OUT) non-zero indicates PHY loopback enabled
 * Returns:     
 *      CDK_E_xxx
 */
static int
bcm5461_phy_loopback_get(phy_ctrl_t *pc, int *enable)
{
    return bcm5464_drv.pd_loopback_get(pc, enable);
}

/*
 * Function:    
 *      bcm5461_phy_ability_get
 * Purpose:     
 *      Get the abilities of the PHY.
 * Parameters:
 *      pc - PHY control structure
 *      abil - (OUT) ability mask indicating supported options/speeds.
 * Returns:     
 *      CDK_E_xxx
 */
static int
bcm5461_phy_ability_get(phy_ctrl_t *pc, uint32_t *abil)
{
    return bcm5464_drv.pd_ability_get(pc, abil);
}

/*
 * Function:
 *      bcm5461_phy_config_set
 * Purpose:
 *      Modify PHY configuration value.
 * Parameters:
 *      pc - PHY control structure
 *      cfg - Configuration parameter
 *      val - Configuration value
 *      cd - Additional configuration data (if any)
 * Returns:
 *      CDK_E_xxx
 */
static int
bcm5461_phy_config_set(phy_ctrl_t *pc, phy_config_t cfg, uint32_t val, void *cd)
{
    return bcm5464_drv.pd_config_set(pc, cfg, val, cd);
}

/*
 * Function:
 *      bcm5461_phy_config_get
 * Purpose:
 *      Get PHY configuration value.
 * Parameters:
 *      pc - PHY control structure
 *      cfg - Configuration parameter
 *      val - (OUT) Configuration value
 *      cd - (OUT) Additional configuration data (if any)
 * Returns:
 *      CDK_E_xxx
 */
static int
bcm5461_phy_config_get(phy_ctrl_t *pc, phy_config_t cfg, uint32_t *val, void *cd)
{
    return bcm5464_drv.pd_config_get(pc, cfg, val, cd);
}

/*
 * Variable:    bcm5461_phy drv
 * Purpose:     PHY Driver for BCM5461.
 */
phy_driver_t bcm5461_drv = {
    "bcm5461",
    "BCM5461 Gigabit PHY Driver",  
    0,
    bcm5461_phy_probe,                  /* pd_probe */
    bcm5461_phy_notify,                 /* pd_notify */
    bcm5461_phy_reset,                  /* pd_reset */
    bcm5461_phy_init,                   /* pd_init */
    bcm5461_phy_link_get,               /* pd_link_get */
    bcm5461_phy_duplex_set,             /* pd_duplex_set */
    bcm5461_phy_duplex_get,             /* pd_duplex_get */
    bcm5461_phy_speed_set,              /* pd_speed_set */
    bcm5461_phy_speed_get,              /* pd_speed_get */
    bcm5461_phy_autoneg_set,            /* pd_autoneg_set */
    bcm5461_phy_autoneg_get,            /* pd_autoneg_get */
    bcm5461_phy_loopback_set,           /* pd_loopback_set */
    bcm5461_phy_loopback_get,           /* pd_loopback_get */
    bcm5461_phy_ability_get,            /* pd_ability_get */
    bcm5461_phy_config_set,             /* pd_config_set */
    bcm5461_phy_config_get,             /* pd_config_get */
    NULL,                               /* pd_status_get */
    NULL                                /* pd_cable_diag */
};
