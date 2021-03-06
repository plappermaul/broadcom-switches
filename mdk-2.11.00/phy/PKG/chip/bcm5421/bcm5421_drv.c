/*
 * This software is governed by the Broadcom Switch APIs license.
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenMDK/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 *
 * PHY driver for BCM5421.
 */

#include <phy/phy.h>
#include <phy/ge_phy.h>

#define BCM5421_PHY_ID0                 0x0020
#define BCM5421_PHY_ID1                 0x60e0

#define PHY_ID1_REV_MASK                0x000f

/* Access to shadowed registers at offset 0x18 */
#define REG_18_SEL(_s)                  (((_s) << 12) | 0x7)
#define REG_18_WR(_s,_v)                (((_s) == 7 ? 0x8000 : 0) | (_v) | (_s))

/* Access to shadowed registers at offset 0x1c */
#define REG_1C_SEL(_s)                  ((_s) << 10)
#define REG_1C_WR(_s,_v)                (REG_1C_SEL(_s) | (_v) | 0x8000)

/* Access expansion registers at offset 0x15 */
#define MII_EXP_MAP_REG(_r)             ((_r) | 0x0f00)
#define MII_EXP_UNMAP                   (0)

/*
 * Non-standard MII Registers
 */
#define MII_MISC_CTRL           0x07 /* MII misc control register */

#define MII_1000X_CTRL          0x0b /* SGMII/1000-X control register */
#define MII_1000X_STAT          0x0c /* SGMII/1000-X status register */
#define MII_1000X_ANP           0x0d /* SGMII/1000-X autoneg partner ability */
#define MII_1000X_MISC          0x0e /* SGMII/1000-X misc register */

#define MII_ECR_REG             0x10 /* MII Extended Control Register */
#define MII_EXP_REG             0x15 /* MII Expansion registers */
#define MII_EXP_SEL             0x17 /* MII Expansion register select */

/***********************************************************************
 *
 * PHY DRIVER FUNCTIONS
 */

/*
 * Function:
 *      bcm5421_phy_probe
 * Purpose:     
 *      Probe for 5421 PHY
 * Parameters:
 *      pc - PHY control structure
 * Returns:     
 *      CDK_E_xxx
 */
static int
bcm5421_phy_probe(phy_ctrl_t *pc)
{
    uint32_t phyid0, phyid1;
    int ioerr = 0;

    PHY_CTRL_CHECK(pc);

    ioerr += PHY_BUS_READ(pc, MII_PHY_ID0_REG, &phyid0);
    ioerr += PHY_BUS_READ(pc, MII_PHY_ID1_REG, &phyid1);

    if (phyid0 == BCM5421_PHY_ID0 && 
        (phyid1 & ~PHY_ID1_REV_MASK) == BCM5421_PHY_ID1) {
        return ioerr ? CDK_E_IO : CDK_E_NONE;
    }
    return CDK_E_NOT_FOUND;
}

/*
 * Function:
 *      bcm5421_phy_notify
 * Purpose:     
 *      Handle PHY notifications
 * Parameters:
 *      pc - PHY control structure
 *      event - PHY event
 * Returns:     
 *      CDK_E_xxx
 */
static int
bcm5421_phy_notify(phy_ctrl_t *pc, phy_event_t event)
{
    int rv = CDK_E_NONE;
    int autoneg;

    PHY_CTRL_CHECK(pc);

    switch (event) {
    case PhyEvent_ChangeToCopper:
        PHY_CTRL_FLAGS(pc) &= ~PHY_F_FIBER_MODE;
        PHY_CTRL_FLAGS(pc) &= ~PHY_F_PASSTHRU;
        /* Upstream PHY should operate in passthru mode */
        event = PhyEvent_ChangeToPassthru;
        break;
    case PhyEvent_ChangeToFiber:
        PHY_CTRL_FLAGS(pc) |= PHY_F_FIBER_MODE;
        PHY_CTRL_FLAGS(pc) |= PHY_F_PASSTHRU;
        break;
    default:
        break;
    }

    /* Call up the PHY chain */
    if (CDK_SUCCESS(rv)) {
        rv = PHY_NOTIFY(PHY_CTRL_NEXT(pc), event);
    }

    /* Update autoneg settings for upstream PHY */
    switch (event) {
    case PhyEvent_ChangeToFiber:
        /* Pass current autoneg setting if fiber mode */
        if (CDK_SUCCESS(rv)) {
            rv = PHY_AUTONEG_GET(pc, &autoneg);
        }
        if (CDK_SUCCESS(rv)) {
            rv = PHY_AUTONEG_SET(PHY_CTRL_NEXT(pc), autoneg);
        }
        break;
    case PhyEvent_ChangeToPassthru:
        /* Disable autoneg if passthru mode */
        if (CDK_SUCCESS(rv)) {
            rv = PHY_AUTONEG_SET(PHY_CTRL_NEXT(pc), 0);
        }
        break;
    default:
        break;
    }

    return rv;
}

/*
 * Function:
 *      bcm5421_phy_reset
 * Purpose:     
 *      Reset 5421 PHY
 * Parameters:
 *      pc - PHY control structure
 * Returns:     
 *      CDK_E_xxx
 */
static int
bcm5421_phy_reset(phy_ctrl_t *pc)
{
    int rv;

    rv = ge_phy_reset(pc);

    /* Call up the PHY chain */
    if (CDK_SUCCESS(rv)) {
        rv = PHY_RESET(PHY_CTRL_NEXT(pc));
    }

    return rv;
}

/*
 * Function:
 *      bcm5421_phy_init
 * Purpose:     
 *      Initialize 5421 PHY driver
 * Parameters:
 *      pc - PHY control structure
 * Returns:     
 *      CDK_E_xxx
 */
static int
bcm5421_phy_init(phy_ctrl_t *pc)
{
    int ioerr = 0;
    int rv = CDK_E_NONE;
    uint32_t tmp;

    PHY_CTRL_CHECK(pc);

    /* Reset PHY */
    if (CDK_SUCCESS(rv)) {
        rv =  PHY_RESET(pc);
    }

    /* Enable SGMII registers */
    ioerr += PHY_BUS_WRITE(pc, 0x1c, REG_1C_WR(0x1f, 1));

    /*
     * Proprietary fix for unusual board/cabling arrangement.
     * Can compromise cable length performance if used unnecessarily.
     */
    ioerr += PHY_BUS_WRITE(pc, 0x18, REG_18_WR(0x0, 0x0c00));
    ioerr += PHY_BUS_WRITE(pc, MII_EXP_SEL, MII_EXP_MAP_REG(0x201f));
    ioerr += PHY_BUS_WRITE(pc, MII_EXP_REG, 0x2aaa);
    ioerr += PHY_BUS_WRITE(pc, MII_EXP_SEL, MII_EXP_UNMAP);

    /*
     * Gamma fix needed if not using internal regulators;
     * does not hurt if used either way.
     */
    ioerr += PHY_BUS_WRITE(pc, 0x18, REG_18_WR(0x0, 0x0c00));
    ioerr += PHY_BUS_WRITE(pc, MII_EXP_SEL, MII_EXP_MAP_REG(0xa));
    ioerr += PHY_BUS_WRITE(pc, MII_EXP_REG, 0x0323);
    ioerr += PHY_BUS_WRITE(pc, MII_EXP_SEL, MII_EXP_UNMAP);

    /* Clear RGMII enable (shadow 111, misc control) */
    ioerr += PHY_BUS_READ(pc, MII_MISC_CTRL, &tmp);
    tmp &= ~0x0080;
    ioerr += PHY_BUS_WRITE(pc, MII_MISC_CTRL, tmp);

    /*
     * Configure Spare Control Register 3.
     * Enabling LEDs to work in fiber mode is important because this
     * signal is used to indicate link in _phy_5421S_link_gmii_fiber().
     */
    tmp  = 0x20;        /* Enable signal/energy detect interrupt */
    tmp |= 0x40;        /* Enable LEDs to work in fiber mode */
    ioerr += PHY_BUS_WRITE(pc, 0x1c, REG_1C_WR(0x05, tmp));

    /* Configure Extended Control Register */
    ioerr += PHY_BUS_READ(pc, MII_ECR_REG, &tmp);
    /* Enable LEDs to indicate traffic status */
    tmp |= 0x0020;
    /* Set high FIFO elasticity to support jumbo frames */
    tmp |= 0x0001;
    ioerr += PHY_BUS_WRITE(pc, MII_ECR_REG, tmp);

    /* Enable extended packet length (4.5k through 25k) */
    ioerr += PHY_BUS_WRITE(pc, 0x18, 0x0007);
    ioerr += PHY_BUS_READ(pc, 0x18, &tmp);
    tmp |= 0x4000;
    ioerr += PHY_BUS_WRITE(pc, 0x18, tmp);

    /* Call up the PHY chain */
    if (CDK_SUCCESS(rv)) {
        rv = PHY_INIT(PHY_CTRL_NEXT(pc));
    }

    /* Set default medium */
    if (CDK_SUCCESS(rv)) {
        PHY_NOTIFY(pc, PhyEvent_ChangeToCopper);
    }

    return ioerr ? CDK_E_IO : rv;
}

/*
 * Function:    
 *      bcm5421_phy_link_get
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
bcm5421_phy_link_get(phy_ctrl_t *pc, int *link, int *autoneg_done)
{
    int ioerr = 0;
    int rv = CDK_E_NONE;
    uint32_t tmp;
    int fiber_detect, led_link;
    int autoneg, an_done;
    int autoneg_local_fault, autoneg_remote_fault;

    ioerr += PHY_BUS_READ(pc, MII_ECR_REG, &tmp);
    fiber_detect = ((tmp & 0x2000) != 0);

    if (fiber_detect) {

        if ((PHY_CTRL_FLAGS(pc) & PHY_F_FIBER_MODE) == 0) {
            PHY_NOTIFY(pc, PhyEvent_ChangeToFiber);
        }

        /*
         * Read LED link status (shadow 01000, LED status register).
         * This works because the init routine has configured the Fiber LED
         * mode.  It provides real time indication of true link, whether or
         * not autonegotiation is enabled.
         */
        ioerr += PHY_BUS_WRITE(pc, 0x1c, REG_1C_SEL(0x08));
        ioerr += PHY_BUS_READ(pc, 0x1c, &tmp);
        led_link = ((tmp & 0x20) == 0);

        ioerr += PHY_BUS_READ(pc, MII_1000X_CTRL, &tmp);
        autoneg = ((tmp & 0x4) == 0);

        if (!autoneg) {
            *link = led_link;
            return ioerr ? CDK_E_IO : CDK_E_NONE;
        }

        ioerr += PHY_BUS_READ(pc, MII_1000X_STAT, &tmp);
        an_done = ((tmp & 0x4) == 0);

        if (autoneg_done) {
            *autoneg_done = an_done;
        }

        if (link == NULL) {
            return ioerr ? CDK_E_IO : CDK_E_NONE;
        }

        if (!an_done) {
            *link = 0;
            return ioerr ? CDK_E_IO : CDK_E_NONE;
        }

        autoneg_local_fault = ((tmp & 1) != 0);

        if (autoneg_local_fault) {
            autoneg_remote_fault = 0;
        } else {
            ioerr += PHY_BUS_READ(pc, MII_1000X_ANP, &tmp);
            autoneg_remote_fault = ((tmp & 0x60) != 0);
        }

        if (autoneg_local_fault || autoneg_remote_fault) {
            /* Restart autonegotiation */
            ioerr += PHY_BUS_WRITE(pc, MII_1000X_MISC, 1);
            *link = 0;
            return ioerr ? CDK_E_IO : CDK_E_NONE;
        }

        *link = 1;
        return ioerr ? CDK_E_IO : CDK_E_NONE;
    }

    if ((PHY_CTRL_FLAGS(pc) & PHY_F_FIBER_MODE) != 0) {
        PHY_NOTIFY(pc, PhyEvent_ChangeToCopper);
    }

    /* Get copper link status */
    rv = ge_phy_link_get(pc, link, autoneg_done);

    return ioerr ? CDK_E_IO : rv;
}

/*
 * Function:    
 *      bcm5421_phy_duplex_set
 * Purpose:     
 *      Set the current duplex mode (forced).
 * Parameters:
 *      pc - PHY control structure
 *      duplex - non-zero indicates full duplex, zero indicates half
 * Returns:     
 *      CDK_E_xxx
 */
static int
bcm5421_phy_duplex_set(phy_ctrl_t *pc, int duplex)
{
    int rv = CDK_E_NONE;

    PHY_CTRL_CHECK(pc);

    /* Call up the PHY chain */
    if (CDK_SUCCESS(rv)) {
        rv = PHY_DUPLEX_SET(PHY_CTRL_NEXT(pc), duplex);
    }

    if (CDK_SUCCESS(rv)) {
        if ((PHY_CTRL_FLAGS(pc) & PHY_F_FIBER_MODE) == 0) {
            rv = ge_phy_duplex_set(pc, duplex);
        } else if (duplex == 0) {
            rv = CDK_E_PARAM;
        }
    }

    return rv;
}

/*
 * Function:    
 *      bcm5421_phy_duplex_get
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
bcm5421_phy_duplex_get(phy_ctrl_t *pc, int *duplex)
{
    PHY_CTRL_CHECK(pc);

    if (PHY_CTRL_FLAGS(pc) & PHY_F_FIBER_MODE) {
        *duplex = 1;
        return CDK_E_NONE;
    }

    return ge_phy_duplex_get(pc, duplex);
}

/*
 * Function:    
 *      bcm5421_phy_speed_set
 * Purpose:     
 *      Set the current operating speed (forced).
 * Parameters:
 *      pc - PHY control structure
 *      speed - new link speed
 * Returns:     
 *      CDK_E_xxx
 */
static int
bcm5421_phy_speed_set(phy_ctrl_t *pc, uint32_t speed)
{
    int rv = CDK_E_NONE;

    PHY_CTRL_CHECK(pc);

    /* Call up the PHY chain */
    if (CDK_SUCCESS(rv)) {
        rv = PHY_SPEED_SET(PHY_CTRL_NEXT(pc), speed);
    }

    if (CDK_SUCCESS(rv)) {
        if ((PHY_CTRL_FLAGS(pc) & PHY_F_FIBER_MODE) == 0) {
            rv = ge_phy_speed_set(pc, speed);
        } else if (speed != 0 && speed != 1000) {
            rv = CDK_E_PARAM;
        }
    }

    return rv;
}

/*
 * Function:    
 *      bcm5421_phy_speed_get
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
bcm5421_phy_speed_get(phy_ctrl_t *pc, uint32_t *speed)
{
    PHY_CTRL_CHECK(pc);

    if (PHY_CTRL_FLAGS(pc) & PHY_F_FIBER_MODE) {
        *speed = 1000;
        return CDK_E_NONE;
    }

    return ge_phy_speed_get(pc, speed);
}

/*
 * Function:    
 *      bcm5421_phy_autoneg_set
 * Purpose:     
 *      Enable or disabled auto-negotiation on the specified port.
 * Parameters:
 *      pc - PHY control structure
 *      autoneg - non-zero enables autoneg, zero disables autoneg
 * Returns:     
 *      CDK_E_xxx
 */
static int
bcm5421_phy_autoneg_set(phy_ctrl_t *pc, int autoneg)
{
    int ioerr = 0;
    int rv = CDK_E_NONE;
    uint32_t tmp;

    PHY_CTRL_CHECK(pc);

    /* Call up the PHY chain if pssthru mode */
    if (PHY_CTRL_FLAGS(pc) & PHY_F_PASSTHRU) {
        rv = PHY_AUTONEG_SET(PHY_CTRL_NEXT(pc), autoneg);
    }

    if (CDK_SUCCESS(rv)) {
        if (PHY_CTRL_FLAGS(pc) & PHY_F_FIBER_MODE) {
            ioerr += PHY_BUS_READ(pc, MII_1000X_CTRL, &tmp);
            tmp &= ~0x4;
            if (autoneg) {
                tmp |= 0x4;
            }
            ioerr += PHY_BUS_WRITE(pc, MII_1000X_CTRL, tmp);
            /* Restart autonegotiation */
            ioerr += PHY_BUS_WRITE(pc, MII_1000X_MISC, 1);
        }
        rv = ge_phy_autoneg_set(pc, autoneg);
    }

    return ioerr ? CDK_E_IO : rv;
}

/*
 * Function:    
 *      bcm5421_phy_autoneg_get
 * Purpose:     
 *      Get the current auto-negotiation status (enabled/busy)
 * Parameters:
 *      pc - PHY control structure
 *      autoneg - (OUT) non-zero indicates autoneg enabled
 *      autoneg_done - (OUT) if true, auto-negotiation is complete
 * Returns:     
 *      CDK_E_xxx
 */
static int
bcm5421_phy_autoneg_get(phy_ctrl_t *pc, int *autoneg)
{
    return ge_phy_autoneg_get(pc, autoneg);
}

/*
 * Function:    
 *      bcm5421_phy_loopback_set
 * Purpose:     
 *      Set the internal PHY loopback mode.
 * Parameters:
 *      pc - PHY control structure
 *      enable - non-zero enables PHY loopback
 * Returns:     
 *      CDK_E_xxx
 */
static int
bcm5421_phy_loopback_set(phy_ctrl_t *pc, int enable)
{
    int ioerr = 0;
    int rv = CDK_E_NONE;
    uint32_t tmp;

    PHY_CTRL_CHECK(pc);

    if (PHY_CTRL_FLAGS(pc) & PHY_F_PASSTHRU) {
        return PHY_LOOPBACK_SET(PHY_CTRL_NEXT(pc), enable);
    }

    ioerr += PHY_BUS_READ(pc, MII_1000X_CTRL, &tmp);
    tmp &= ~1;
    if (enable) {
        tmp |= 1;
    }
    ioerr += PHY_BUS_WRITE(pc, MII_1000X_CTRL, tmp);

    rv = ge_phy_loopback_set(pc, enable);

    return ioerr ? CDK_E_IO : rv;
}

/*
 * Function:    
 *      bcm5421_phy_loopback_get
 * Purpose:     
 *      Get the local PHY loopback mode.
 * Parameters:
 *      pc - PHY control structure
 *      enable - (OUT) non-zero indicates PHY loopback enabled
 * Returns:     
 *      CDK_E_xxx
 */
static int
bcm5421_phy_loopback_get(phy_ctrl_t *pc, int *enable)
{
    PHY_CTRL_CHECK(pc);

    if (PHY_CTRL_FLAGS(pc) & PHY_F_PASSTHRU) {
        return PHY_LOOPBACK_GET(PHY_CTRL_NEXT(pc), enable);
    }

    return ge_phy_loopback_get(pc, enable);
}

/*
 * Function:    
 *      bcm5421_phy_ability_get
 * Purpose:     
 *      Get the abilities of the PHY.
 * Parameters:
 *      pc - PHY control structure
 *      abil - (OUT) ability mask indicating supported options/speeds.
 * Returns:     
 *      CDK_E_xxx
 */
static int
bcm5421_phy_ability_get(phy_ctrl_t *pc, uint32_t *abil)
{
    PHY_CTRL_CHECK(pc);

    *abil = (PHY_ABIL_1000MB | PHY_ABIL_100MB | PHY_ABIL_10MB | 
             PHY_ABIL_LOOPBACK | PHY_ABIL_GMII);
    
    return CDK_E_NONE;
}

/*
 * Function:
 *      bcm5421_phy_config_set
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
bcm5421_phy_config_set(phy_ctrl_t *pc, phy_config_t cfg, uint32_t val, void *cd)
{
    PHY_CTRL_CHECK(pc);

    switch (cfg) {
    case PhyConfig_Enable:
        return CDK_E_NONE;
    case PhyConfig_PortInterface:
        switch (val) {
        case PHY_IF_GMII:
        case PHY_IF_SGMII:
            return CDK_E_NONE;
        default:
            break;
        }
        break;
    default:
        break;
    }

    return CDK_E_UNAVAIL;
}

/*
 * Function:
 *      bcm5421_phy_config_get
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
bcm5421_phy_config_get(phy_ctrl_t *pc, phy_config_t cfg, uint32_t *val, void *cd)
{
    PHY_CTRL_CHECK(pc);

    switch (cfg) {
    case PhyConfig_Enable:
        *val = 1;
        return CDK_E_NONE;
    case PhyConfig_PortInterface:
        *val = PHY_IF_GMII;
        return CDK_E_NONE;
    default:
        break;
    }

    return CDK_E_UNAVAIL;
}

/*
 * Variable:    bcm5421_phy drv
 * Purpose:     PHY Driver for BCM5421.
 */
phy_driver_t bcm5421_drv = {
    "bcm5421",
    "BCM5421 Gigabit PHY Driver",  
    0,
    bcm5421_phy_probe,                  /* pd_probe */
    bcm5421_phy_notify,                 /* pd_notify */
    bcm5421_phy_reset,                  /* pd_reset */
    bcm5421_phy_init,                   /* pd_init */
    bcm5421_phy_link_get,               /* pd_link_get */
    bcm5421_phy_duplex_set,             /* pd_duplex_set */
    bcm5421_phy_duplex_get,             /* pd_duplex_get */
    bcm5421_phy_speed_set,              /* pd_speed_set */
    bcm5421_phy_speed_get,              /* pd_speed_get */
    bcm5421_phy_autoneg_set,            /* pd_autoneg_set */
    bcm5421_phy_autoneg_get,            /* pd_autoneg_get */
    bcm5421_phy_loopback_set,           /* pd_loopback_set */
    bcm5421_phy_loopback_get,           /* pd_loopback_get */
    bcm5421_phy_ability_get,            /* pd_ability_get */
    bcm5421_phy_config_set,             /* pd_config_set */
    bcm5421_phy_config_get,             /* pd_config_get */
    NULL,                               /* pd_status_get */
    NULL                                /* pd_cable_diag */
};
