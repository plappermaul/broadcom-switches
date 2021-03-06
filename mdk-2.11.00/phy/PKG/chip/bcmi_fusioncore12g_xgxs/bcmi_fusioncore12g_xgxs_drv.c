/*
 * This software is governed by the Broadcom Switch APIs license.
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenMDK/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 *
 * PHY Driver for internal Fusioncore 12G (CX4) XGXS PHY.
 */

#include <phy/phy.h>
#include <phy/phy_xgs_iblk.h>
#include <phy/phy_brcm_serdes_id.h>
#include <phy/ge_phy.h>

#define BCMI_FUSIONCORE12G_XGXS_ID0          0x0143
#define BCMI_FUSIONCORE12G_XGXS_ID1          0xbd90

#define PHY_ID1_REV_MASK                0x000f

#define _PHY_REG_READ(_pc, _r, _v)      phy_xgs_iblk_read(_pc, _r, _v)
#define _PHY_REG_WRITE(_pc, _r, _v)     phy_xgs_iblk_write(_pc, _r, _v)

/* MIIM block definitions for register 0x10-0x1f */
#define MIIM_BLK_BLOCK0                 0x0000
#define MIIM_BLK_BLOCK1                 0x1000
#define MIIM_BLK_TX_BERT                0x2000
#define MIIM_BLK_RX_BERT                0x3000
#define MIIM_BLK_ALL_BERT               0x4000
#define MIIM_BLK_PLL                    0x5000
#define MIIM_BLK_TX0                    0x6000
#define MIIM_BLK_TX1                    0x7000
#define MIIM_BLK_TX2                    0x8000
#define MIIM_BLK_TX3                    0x9000
#define MIIM_BLK_TXALL                  0xa000
#define MIIM_BLK_RX0                    0xb000
#define MIIM_BLK_RX1                    0xc000
#define MIIM_BLK_RX2                    0xd000
#define MIIM_BLK_RX3                    0xe000
#define MIIM_BLK_RXALL                  0xf000

#define XGXS_IEEE_CTRL_REG              0x00
#define XGXS_IEEE_CTRL_SW_RST           (1 << 15)
#define XGXS_IEEE_CTRL_LB               (1 << 14)
#define XGXS_IEEE_CTRL_SW_PD            (1 << 11)
#define XGXS_IEEE_CTRL_PMD_LB           (1 << 0)

#define XGXS_IEEE_STAT_LF               (1 << 7)
#define XGXS_IEEE_STAT_LINK             (1 << 2)
#define XGXS_IEEE_STAT_PD_CAP           (1 << 1)

#define XGXS_MISC_CTRL1                 0x1e
#define XGXS_MISC_CTRL1_PMD_EN          (1 << 9)

#define XGXS_RX_CTRL                    (0x11 | MIIM_BLK_RXALL)
#define XGXS_RX_CTRL_RX_SEQ_DONE        (1 << 4)

/***********************************************************************
 *
 * PHY DRIVER FUNCTIONS
 */

#if PHY_CONFIG_INCLUDE_CHIP_SYMBOLS == 1
extern cdk_symbols_t bcmi_fusioncore12g_xgxs_symbols;
#endif

/*
 * Function:
 *      bcmi_fusioncore12g_xgxs_probe
 * Purpose:     
 *      Probe for PHY.
 * Parameters:
 *      pc - PHY control structure
 * Returns:     
 *      CDK_E_xxx
 */
static int
bcmi_fusioncore12g_xgxs_probe(phy_ctrl_t *pc)
{
    uint32_t phyid0, phyid1;
    int ioerr = 0;

    PHY_CTRL_CHECK(pc);

    ioerr += phy_brcm_serdes_id(pc, &phyid0, &phyid1);

    if (phyid0 == BCMI_FUSIONCORE12G_XGXS_ID0 &&
        (phyid1 & ~PHY_ID1_REV_MASK) == BCMI_FUSIONCORE12G_XGXS_ID1) {
#if PHY_CONFIG_INCLUDE_CHIP_SYMBOLS == 1
        PHY_CTRL_SYMBOLS(pc) = &bcmi_fusioncore12g_xgxs_symbols;
#endif
        return ioerr ? CDK_E_IO : CDK_E_NONE;
    }
    return CDK_E_NOT_FOUND;
}


/*
 * Function:
 *      bcmi_fusioncore12g_xgxs_notify
 * Purpose:     
 *      Handle PHY notifications.
 * Parameters:
 *      pc - PHY control structure
 *      event - PHY event
 * Returns:     
 *      CDK_E_xxx
 */
static int
bcmi_fusioncore12g_xgxs_notify(phy_ctrl_t *pc, phy_event_t event)
{
    PHY_CTRL_CHECK(pc);

    switch (event) {
    case PhyEvent_MacDisable:
        PHY_CTRL_FLAGS(pc) |= PHY_F_MAC_DISABLE;
        break;
    case PhyEvent_MacEnable:
        PHY_CTRL_FLAGS(pc) &= ~PHY_F_MAC_DISABLE;
        break;
    case PhyEvent_PortDrainStart:
        PHY_CTRL_FLAGS(pc) |= PHY_F_PORT_DRAIN;
        break;
    case PhyEvent_PortDrainStop:
        PHY_CTRL_FLAGS(pc) &= ~PHY_F_PORT_DRAIN;
        break;
    default:
        break;
    }

    return CDK_E_NONE;
}

/*
 * Function:
 *      bcmi_fusioncore12g_xgxs_reset
 * Purpose:     
 *      Reset PHY.
 * Parameters:
 *      pc - PHY control structure
 * Returns:     
 *      CDK_E_xxx
 */
static int
bcmi_fusioncore12g_xgxs_reset(phy_ctrl_t *pc)
{
    return CDK_E_NONE;
}

/*
 * Function:
 *      bcmi_fusioncore12g_xgxs_init
 * Purpose:     
 *      Initialize PHY driver.
 * Parameters:
 *      pc - PHY control structure
 * Returns:     
 *      CDK_E_NONE
 */
static int
bcmi_fusioncore12g_xgxs_init(phy_ctrl_t *pc)
{
    return CDK_E_NONE;
}

/*
 * Function:    
 *      bcmi_fusioncore12g_xgxs_link_get
 * Purpose:     
 *      Determine the current link up/down status.
 * Parameters:
 *      pc - PHY control structure
 *      link - (OUT) non-zero indicates link established.
 *      autoneg_done - (OUT) not supported
 * Returns:
 *      CDK_E_xxx
 */
static int
bcmi_fusioncore12g_xgxs_link_get(phy_ctrl_t *pc, int *link, int *autoneg_done)
{
    int ioerr = 0;
    uint32_t stat;

    ioerr += _PHY_REG_READ(pc, MII_STAT_REG, &stat);

    if (link) {
        *link = (stat & XGXS_IEEE_STAT_LINK) ? 1 : 0;
    }

    return ioerr ? CDK_E_IO : CDK_E_NONE;
}

/*
 * Function:    
 *      bcmi_fusioncore12g_xgxs_duplex_set
 * Purpose:     
 *      Set the current duplex mode (forced).
 * Parameters:
 *      pc - PHY control structure
 *      duplex - non-zero indicates full duplex, zero indicates half
 * Returns:     
 *      CDK_E_xxx
 */
static int
bcmi_fusioncore12g_xgxs_duplex_set(phy_ctrl_t *pc, int duplex)
{
    return (duplex != 0) ? CDK_E_NONE : CDK_E_PARAM;
}

/*
 * Function:    
 *      bcmi_fusioncore12g_xgxs_duplex_get
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
bcmi_fusioncore12g_xgxs_duplex_get(phy_ctrl_t *pc, int *duplex)
{
    *duplex = 1;

    return CDK_E_NONE;
}

/*
 * Function:    
 *      bcmi_fusioncore12g_xgxs_speed_set
 * Purpose:     
 *      Set the current operating speed (forced).
 * Parameters:
 *      pc - PHY control structure
 *      speed - new link speed
 * Returns:     
 *      CDK_E_xxx
 * Notes:
 *      The actual speed is controlled elsewhere, so we accept any value.
 */
static int
bcmi_fusioncore12g_xgxs_speed_set(phy_ctrl_t *pc, uint32_t speed)
{
    return CDK_E_NONE;
}

/*
 * Function:    
 *      bcmi_fusioncore12g_xgxs_speed_get
 * Purpose:     
 *      Get the current operating speed.
 * Parameters:
 *      pc - PHY control structure
 *      speed - (OUT) current link speed
 * Returns:     
 *      CDK_E_xxx
 * Notes:
 *      The actual speed is controlled elsewhere, so always return 10000
 *      for sanity purposes.
 */

static int
bcmi_fusioncore12g_xgxs_speed_get(phy_ctrl_t *pc, uint32_t *speed)
{
    *speed = 10000;

    return CDK_E_NONE;
}

/*
 * Function:    
 *      bcmi_fusioncore12g_xgxs_autoneg_set
 * Purpose:     
 *      Enable or disabled auto-negotiation on the specified port.
 * Parameters:
 *      pc - PHY control structure
 *      autoneg - non-zero enables autoneg, zero disables autoneg
 * Returns:     
 *      CDK_E_xxx
 */

static int
bcmi_fusioncore12g_xgxs_autoneg_set(phy_ctrl_t *pc, int autoneg)
{
    return autoneg ? CDK_E_PARAM : CDK_E_NONE;
}

/*
 * Function:    
 *      bcmi_fusioncore12g_xgxs_autoneg_get
 * Purpose:     
 *      Get the current auto-negotiation status (enabled/busy).
 * Parameters:
 *      pc - PHY control structure
 *      autoneg - (OUT) non-zero indicates autoneg enabled
 * Returns:     
 *      CDK_E_xxx
 */
static int
bcmi_fusioncore12g_xgxs_autoneg_get(phy_ctrl_t *pc, int *autoneg)
{
    *autoneg = 0;

    return CDK_E_NONE;
}

/*
 * Function:    
 *      bcmi_fusioncore12g_xgxs_loopback_set
 * Purpose:     
 *      Set PHY loopback mode.
 * Parameters:
 *      pc - PHY control structure
 *      enable - non-zero enables PHY loopback
 * Returns:     
 *      CDK_E_xxx
 */
static int
bcmi_fusioncore12g_xgxs_loopback_set(phy_ctrl_t *pc, int enable)
{
    int ioerr = 0;
    uint32_t rx_ctrl, misc_ctrl1, lb_bit, ctrl;

    /* Control RX signal detect, so that a cable is not needed for loopback */
    ioerr += _PHY_REG_READ(pc, XGXS_RX_CTRL, &rx_ctrl);
    if (enable) {
        /* Disable signal detect on all lanes */
        rx_ctrl |= XGXS_RX_CTRL_RX_SEQ_DONE;
    } else {
        /* Enable signal detect on all lanes (default) */
        rx_ctrl &= ~XGXS_RX_CTRL_RX_SEQ_DONE;
    }
    ioerr += _PHY_REG_WRITE(pc, XGXS_RX_CTRL, rx_ctrl);
    
    /* Select loopback bit depending on HiGig/XE mode */
    ioerr += _PHY_REG_READ(pc, XGXS_MISC_CTRL1, &misc_ctrl1);
    if (misc_ctrl1 & XGXS_MISC_CTRL1_PMD_EN) {
        lb_bit = XGXS_IEEE_CTRL_PMD_LB;
    } else {
        lb_bit = XGXS_IEEE_CTRL_LB;
    }
    /* Set loopback mode */
    ioerr += _PHY_REG_READ(pc, MII_CTRL_REG, &ctrl);
    if (enable) {
        ctrl |= lb_bit;
    } else {
        ctrl &= ~lb_bit;
    }
    ioerr += _PHY_REG_WRITE(pc, MII_CTRL_REG, ctrl);

    return ioerr ? CDK_E_IO : CDK_E_NONE;
}

/*
 * Function:    
 *      bcmi_fusioncore12g_xgxs_loopback_get
 * Purpose:     
 *      Get the current PHY loopback mode.
 * Parameters:
 *      pc - PHY control structure
 *      enable - (OUT) non-zero indicates PHY loopback enabled
 * Returns:     
 *      CDK_E_xxx
 */
static int
bcmi_fusioncore12g_xgxs_loopback_get(phy_ctrl_t *pc, int *enable)
{
    int ioerr = 0;
    uint32_t data, lb_bit, misc_ctrl1;

    /* Select loopback bit depending on HiGig/XE mode */
    ioerr += _PHY_REG_READ(pc, XGXS_MISC_CTRL1, &misc_ctrl1);
    if (misc_ctrl1 & XGXS_MISC_CTRL1_PMD_EN) {
        lb_bit = XGXS_IEEE_CTRL_PMD_LB;
    } else {
        lb_bit = XGXS_IEEE_CTRL_LB;
    }
    /* Get loopback mode */
    ioerr += _PHY_REG_READ(pc, MII_CTRL_REG, &data);
    *enable = (data & lb_bit) ? 1 : 0;

    return ioerr ? CDK_E_IO : CDK_E_NONE;
}

/*
 * Function:    
 *      bcmi_fusioncore12g_xgxs_ability_get
 * Purpose:     
 *      Get the abilities of the PHY.
 * Parameters:
 *      pc - PHY control structure
 *      abil - (OUT) ability mask indicating supported options/speeds.
 * Returns:     
 *      CDK_E_xxx
 */
static int
bcmi_fusioncore12g_xgxs_ability_get(phy_ctrl_t *pc, uint32_t *abil)
{
    *abil = (PHY_ABIL_10GB | 
             PHY_ABIL_LOOPBACK | PHY_ABIL_XGMII);
    
    return CDK_E_NONE;
}

/*
 * Function:
 *      bcmi_fusioncore12g_xgxs_config_set
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
bcmi_fusioncore12g_xgxs_config_set(phy_ctrl_t *pc, phy_config_t cfg, uint32_t val, void *cd)
{
    PHY_CTRL_CHECK(pc);

    switch (cfg) {
    case PhyConfig_Enable:
    case PhyConfig_PortInterface:
        return CDK_E_NONE;
    default:
        break;
    }

    return CDK_E_UNAVAIL;
}

/*
 * Function:
 *      bcmi_fusioncore12g_xgxs_config_get
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
bcmi_fusioncore12g_xgxs_config_get(phy_ctrl_t *pc, phy_config_t cfg, uint32_t *val, void *cd)
{
    PHY_CTRL_CHECK(pc);

    switch (cfg) {
    case PhyConfig_Enable:
        *val = 1;
        return CDK_E_NONE;
    case PhyConfig_PortInterface:
        *val = PHY_IF_XGMII;
        return CDK_E_NONE;
    default:
        break;
    }

    return CDK_E_UNAVAIL;
}

/* Public PHY Driver Structure */
phy_driver_t bcmi_fusioncore12g_xgxs_drv = {
    "bcmi_fusioncore12g_xgxs", 
    "Internal Fusioncore 12G XGXS PHY Driver",  
    PHY_DRIVER_F_INTERNAL,
    bcmi_fusioncore12g_xgxs_probe,          /* pd_probe */
    bcmi_fusioncore12g_xgxs_notify,         /* pd_notify */
    bcmi_fusioncore12g_xgxs_reset,          /* pd_reset */
    bcmi_fusioncore12g_xgxs_init,           /* pd_init */
    bcmi_fusioncore12g_xgxs_link_get,       /* pd_link_get */
    bcmi_fusioncore12g_xgxs_duplex_set,     /* pd_duplex_set */
    bcmi_fusioncore12g_xgxs_duplex_get,     /* pd_duplex_get */
    bcmi_fusioncore12g_xgxs_speed_set,      /* pd_speed_set */
    bcmi_fusioncore12g_xgxs_speed_get,      /* pd_speed_get */
    bcmi_fusioncore12g_xgxs_autoneg_set,    /* pd_autoneg_set */
    bcmi_fusioncore12g_xgxs_autoneg_get,    /* pd_autoneg_get */
    bcmi_fusioncore12g_xgxs_loopback_set,   /* pd_loopback_set */
    bcmi_fusioncore12g_xgxs_loopback_get,   /* pd_loopback_get */
    bcmi_fusioncore12g_xgxs_ability_get,    /* pd_ability_get */
    bcmi_fusioncore12g_xgxs_config_set,     /* pd_config_set */
    bcmi_fusioncore12g_xgxs_config_get,     /* pd_config_get */
    NULL,                                   /* pd_status_get */
    NULL                                    /* pd_cable_diag */
};
