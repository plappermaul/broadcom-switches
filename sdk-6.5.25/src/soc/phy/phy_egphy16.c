/*
 * 
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2021 Broadcom Inc. All rights reserved.
 *
 * File:        phyegphy16.c
 * Purpose:     PHY driver for BCM EGPHY16
 */
#include <sal/types.h>
#include <sal/core/spl.h>
#include <shared/bsl.h>
#include <soc/drv.h>
#include <soc/debug.h>
#include <soc/error.h>
#include <soc/phyreg.h>

#include <soc/phy.h>
#include <soc/phy/phyctrl.h>
#include <soc/phy/drv.h>

#include "phydefs.h"      /* Must include before other phy related includes */

#if defined(INCLUDE_PHY_EGPHY16)
#include "phyconfig.h"    /* Must be the first phy include after phydefs.h */

#include "phyident.h"
#include "phyreg.h"
#include "phyfege.h"
#include "phynull.h"
#include "phy_egphy16.h"

#define SOC_IF_ERROR_BREAK(op, __rv__) \
        if (((__rv__) = (op)) < 0) {   \
            break;                     \
        }

#ifndef DISABLE_CLK125
#define DISABLE_CLK125 0
#endif

#ifndef AUTO_MDIX_WHEN_AN_DIS
#define AUTO_MDIX_WHEN_AN_DIS 0
#endif

/* #define A0_AUTO_FILL_PORT_INDEX 1 */

#define PHY_IS_BCM56275GPHY(_pc) (PHY_MODEL_CHECK((_pc), \
                                  PHY_BCM56275GPHY_OUI, \
                                  PHY_BCM56275GPHY_MODEL))
STATIC int
phy_egphy16_speed_set(int unit, soc_port_t port, int speed);
STATIC int
phy_egphy16_duplex_set(int unit, soc_port_t port, int duplex);
STATIC int
phy_egphy16_master_set(int unit, soc_port_t port, int master);
STATIC int
phy_egphy16_ability_advert_set(int unit, soc_port_t port, soc_port_ability_t *ability);
STATIC int
phy_egphy16_autoneg_set(int unit, soc_port_t port, int autoneg);
STATIC int
phy_egphy16_mdix_set(int unit, soc_port_t port, soc_port_mdix_t mode);
STATIC int
phy_egphy16_ability_local_get(int unit, soc_port_t port, soc_port_ability_t *ability);

STATIC int
phy_egphy16_control_set(int unit, soc_port_t port,
                        soc_phy_control_t type, uint32 value);

extern int
phy_ecd_cable_diag_init_40nm(int unit, soc_port_t port);

extern int
phy_ecd_cable_diag_40nm(int unit, soc_port_t port,
                        soc_port_cable_diag_t *status, uint16 ecd_ctrl);


/*
 * Function:
 *      _phy_egphy16_cl45_reg_read
 * Purpose:
 *      Read Clause 45 Register using Clause 22 register access
 * Parameters:
 *      unit      - StrataSwitch unit #.
 *      pc        - Phy control
 *      flags     - Flags
 *      dev_addr  - Clause 45 Device
 *      reg_addr  - Register Address to read
 *      val       - Value Read
 * Returns:
 *      SOC_E_XXX
 */

STATIC int
_phy_egphy16_cl45_reg_read(int unit, phy_ctrl_t *pc, uint32 flags,
                           uint8 dev_addr, uint16 reg_addr, uint16 *val)
{

    /* Write Device Address to register 0x0D (Set Function field to Address)*/
    SOC_IF_ERROR_RETURN
        (PHYEGPHY16_REG_WRITE(unit, pc, flags, 0x00, 0x0D, (dev_addr & 0x001f)));

    /* Select the register by writing to register address to register 0x0E */
    SOC_IF_ERROR_RETURN
        (PHYEGPHY16_REG_WRITE(unit, pc, flags, 0x00, 0x0E, reg_addr));

    /* Write Device Address to register 0x0D (Set Function field to Data)*/
    SOC_IF_ERROR_RETURN
        (PHYEGPHY16_REG_WRITE(unit, pc, flags, 0x00, 0x0D,
                              ((0x4000) | (dev_addr & 0x001f))));

    /* Read register 0x0E to get the value */
    SOC_IF_ERROR_RETURN
        (PHYEGPHY16_REG_READ(unit, pc, flags, 0x00, 0x0E, val));

    return SOC_E_NONE;
}

/*
 * Function:
 *      _phy_egphy16_cl45_reg_write
 * Purpose:
 *      Write Clause 45 Register content using Clause 22 register access
 * Parameters:
 *      unit      - StrataSwitch unit #.
 *      pc        - Phy control
 *      flags     - Flags
 *      dev_addr  - Clause 45 Device
 *      reg_addr  - Register Address to read
 *      val       - Value to be written
 * Returns:
 *      SOC_E_XXX
 */

STATIC int
_phy_egphy16_cl45_reg_write(int unit, phy_ctrl_t *pc, uint32 flags,
                            uint8 dev_addr, uint16 reg_addr, uint16 val)
{

    /* Write Device Address to register 0x0D (Set Function field to Address)*/
    SOC_IF_ERROR_RETURN
        (PHYEGPHY16_REG_WRITE(unit, pc, flags, 0x00, 0x0D, (dev_addr & 0x001f)));

    /* Select the register by writing to register address to register 0x0E */
    SOC_IF_ERROR_RETURN
        (PHYEGPHY16_REG_WRITE(unit, pc, flags, 0x00, 0x0E, reg_addr));

    /* Write Device Address to register 0x0D (Set Function field to Data)*/
    SOC_IF_ERROR_RETURN
        (PHYEGPHY16_REG_WRITE(unit, pc, flags, 0x00, 0x0D,
                              ((0x4000) | (dev_addr & 0x001f))));

    /* Write register 0x0E to write the value */
    SOC_IF_ERROR_RETURN
        (PHYEGPHY16_REG_WRITE(unit, pc, flags, 0x00, 0x0E, val));

    return SOC_E_NONE;
}

/*
 * Function:
 *      _phy_egphy16_cl45_reg_modify
 * Purpose:
 *      Modify Clause 45 Register contents using Clause 22 register access
 * Parameters:
 *      unit      - StrataSwitch unit #.
 *      pc        - Phy control
 *      flags     - Flags
 *      reg_bank  - Register bank
 *      dev_addr  - Clause 45 Device
 *      reg_addr  - Register Address to read
 *      val       - Value
 *      mask      - Mask for modifying the contents of the register
 * Returns:
 *      SOC_E_XXX
 */

STATIC int
_phy_egphy16_cl45_reg_modify(int unit, phy_ctrl_t *pc, uint32 flags,
                             uint8 dev_addr, uint16 reg_addr, uint16 val,
                             uint16 mask)
{

    uint16 value = 0;

    SOC_IF_ERROR_RETURN
        (PHYEGPHY16_CL45_REG_READ(unit, pc, flags, dev_addr, reg_addr, &value));

    value = (val & mask) | (value & ~mask);

    SOC_IF_ERROR_RETURN
        (PHYEGPHY16_CL45_REG_WRITE(unit, pc, flags, dev_addr, reg_addr, value));

    return SOC_E_NONE;
}

/*
 * Function:
 *      _phy_egphy16_blk_top_lvl_reg_read
 * Purpose:
 *      New mechanism to Read Top Level Registers for EEE and 1588.
 * Parameters:
 *      unit       - StrataSwitch unit #.
 *      pc         - Phy control
 *      flags      - Flags
 *      reg_offset - Register Address to read
 *      val        - Value Read
 * Returns:
 *      SOC_E_XXX
 */

STATIC int
_phy_egphy16_blk_top_lvl_reg_read(int unit, phy_ctrl_t *pc, uint32 flags,
                                  uint16 reg_offset, uint16 *val)
{

    /* Write register 0x17 with the top level reg offset to read (handled in PHYEGPHY16_REG_READ) */
    /* Read register 0x15 to get the value */
    SOC_IF_ERROR_RETURN
        (PHYEGPHY16_REG_READ(unit, pc, flags, (0x0D00|(reg_offset & 0xff)), 0x15, val));

    return SOC_E_NONE;
}

/*
 * Function:
 *      _phy_egphy16_blk_top_lvl_reg_write
 * Purpose:
 *      New mechanism to Write Top Level Registers for EEE and 1588
 * Parameters:
 *      unit      - StrataSwitch unit #.
 *      pc        - Phy control
 *      flags     - Flags
 *      reg_offset  - Register Address to read
 *      val       - Value to be written
 * Returns:
 *      SOC_E_XXX
 */

STATIC int
_phy_egphy16_blk_top_lvl_reg_write(int unit, phy_ctrl_t *pc, uint32 flags,
                                   uint16 reg_offset, uint16 val)
{

    /* Write register 0x17 with the top level reg offset to write (handled in PHYEGPHY16_REG_WRITE) */
    /* Write register 0x15 the value to write to the top level register offset */
    SOC_IF_ERROR_RETURN
        (PHYEGPHY16_REG_WRITE(unit, pc, flags, (0x0D00|(reg_offset & 0xff)), 0x15, val));

    return SOC_E_NONE;
}

/*
 * Function:
 *      _phy_egphy16_blk_top_lvl_reg_modify
 * Purpose:
 *      Modify Clause 45 Register contents using Clause 22 register access
 * Parameters:
 *      unit      - StrataSwitch unit #.
 *      pc        - Phy control
 *      flags     - Flags
 *      reg_bank  - Register bank
 *      dev_addr  - Clause 45 Device
 *      reg_addr  - Register Address to read
 *      val       - Value
 *      mask      - Mask for modifying the contents of the register
 * Returns:
 *      SOC_E_XXX
 */

STATIC int
_phy_egphy16_blk_top_lvl_reg_modify(int unit, phy_ctrl_t *pc, uint32 flags,
                                    uint16 reg_offset, uint16 val, uint16 mask)
{

    uint16 value = 0;

    /* Write register 0x17 with the top level reg offset to read */
    SOC_IF_ERROR_RETURN
        (_phy_egphy16_blk_top_lvl_reg_read(unit, pc, flags,
                                           reg_offset, &value));
    value = (val & mask) | (value & ~mask);

    /* Write register 0x17 with the top level reg offset to read */
    SOC_IF_ERROR_RETURN
        (_phy_egphy16_blk_top_lvl_reg_write(unit, pc, flags,
                                            reg_offset, value));

    return SOC_E_NONE;
}

/*
 * Function:
 *      phy_egphy16_toplvl_reg_read
 * Purpose:
 *      Read a top level register from a supporting chip
 * Parameters:
 *      unit - SOC Unit #.
 *      port - Port number.
 *      flags -
 *      reg_offset  - Offset to the reg
 *      data  - Pointer to data returned
 * Returns:
 *      SOC_E_XXX
 */
int
phy_egphy16_toplvl_reg_read(int unit, soc_port_t port, int flags, uint8 reg_offset, uint16 *data)
{
    phy_ctrl_t    *pc;
    uint16 reg_data, status;
    int         rv = SOC_E_NONE;

    LOG_VERBOSE(BSL_LS_SOC_PHY,
                (BSL_META_U(unit,
                            "entered phy_egphy16_toplvl_reg_read: "
                            "unit %d, port %d, flags %x reg_offset %u\n"),
                 unit, port, flags, reg_offset));

    pc = EXT_PHY_SW_STATE(unit, port);

    /* Write Reg address to Port 5's (offset=4) register 0x1C, shadow 0x0B */
    /* Status READ from Port 3's (offset=2) register 0x15 */

    /* Write Reg offset to Port 5's register 0x1C, shadow 0x0B */
    reg_data = (0xAC00 | reg_offset);
    INT_MCU_LOCK(unit);
    pc->phy_id =  PHYEGPHY16_SLICE_ADDR(pc, 4);
    rv = pc->write(unit, pc->phy_id, 0x1c, reg_data);
    if (SOC_FAILURE(rv)) {
        pc->phy_id =  PHYEGPHY16_DEV_PHY_ID_ORIG(pc);
        INT_MCU_UNLOCK(unit);
        return rv;
    }

    pc->phy_id =  PHYEGPHY16_SLICE_ADDR(pc, 2);
    /* Read data from Top level MII Status register(0x15h) */
    rv = pc->write(unit, pc->phy_id, 0x17, 0x8F0B);
    if (SOC_FAILURE(rv)) {
        pc->phy_id =  PHYEGPHY16_DEV_PHY_ID_ORIG(pc);
        INT_MCU_UNLOCK(unit);
        return rv;
    }
    rv = pc->read(unit, pc->phy_id, 0x15, &status);
    INT_MCU_UNLOCK(unit);
    if (SOC_FAILURE(rv)) {
        pc->phy_id =  PHYEGPHY16_DEV_PHY_ID_ORIG(pc);
        return rv;
    }

    *data = (status & 0xff);

    pc->phy_id =  PHYEGPHY16_DEV_PHY_ID_ORIG(pc);

    return SOC_E_NONE;
}

/*
 * Function:
 *      phy_egphy16_toplvl_reg_write
 * Purpose:
 *      Write to a top level register from a supporting chip
 * Parameters:
 *      unit - SOC Unit #.
 *      port - Port number.
 *      flags -
 *      reg_offset  - Offset to the reg
 *      data  - Data to be written
 * Returns:
 *      SOC_E_XXX
 */
int
phy_egphy16_toplvl_reg_write(int unit, soc_port_t port, int flags, uint8 reg_offset, uint16 data)
{
    phy_ctrl_t *pc;
    uint16 reg_data;
#ifdef BROADCOM_DEBUG
    uint16 status;
#endif
    int         rv = SOC_E_NONE;

    LOG_VERBOSE(BSL_LS_SOC_PHY,
                (BSL_META_U(unit,
                            "entered phy_egphy16_toplvl_reg_write: "
                            "unit %d, port %d, flags %x reg_offset %u, data %u\n"),
                 unit, port, flags, reg_offset, data));

    pc       = EXT_PHY_SW_STATE(unit, port);

    /* Write Reg address to Port 5's register 0x1C, shadow 0x0B */
    /* Write data to Port 6's register 0x1C, shadow 0x0c */
    /* Status READ from Port 3's register 0x15 */

    /* Write Data to port6 (offset = 5), register 0x1C, shadow 0x0c */
    INT_MCU_LOCK(unit);
    pc->phy_id =  PHYEGPHY16_SLICE_ADDR(pc, 5);
    reg_data = (0xB000 | (data & 0xff));
    rv = pc->write(unit, pc->phy_id, 0x1c, reg_data);
    if (SOC_FAILURE(rv)) {
        pc->phy_id =  PHYEGPHY16_DEV_PHY_ID_ORIG(pc);
        INT_MCU_UNLOCK(unit);
        return rv;
    }

    pc->phy_id =  PHYEGPHY16_SLICE_ADDR(pc, 4);
    /* Write Reg address to Port 5's (offset = 4) register 0x1C, shadow 0x0B */
    /* Enable Write ( Port 5's register 0x1C, shadow 0x0B) Bit 7 = 1 */
    reg_data = (0xAC80 | reg_offset);
    rv = pc->write(unit, pc->phy_id, 0x1c, reg_data);
    if (SOC_FAILURE(rv)) {
        pc->phy_id =  PHYEGPHY16_DEV_PHY_ID_ORIG(pc);
        INT_MCU_UNLOCK(unit);
        return rv;
    }

    /* Disable Write ( Port 5's register 0x1C, shadow 0x0B) Bit 7 = 0 */
    reg_data = (0xAC00 | reg_offset);
    rv = pc->write(unit, pc->phy_id, 0x1c, reg_data);
    if (SOC_FAILURE(rv)) {
        pc->phy_id =  PHYEGPHY16_DEV_PHY_ID_ORIG(pc);
        INT_MCU_UNLOCK(unit);
        return rv;
    }

#ifdef BROADCOM_DEBUG
    pc->phy_id =  PHYEGPHY16_SLICE_ADDR(pc, 2);
    /* Read data from Top level MII Status register(0x15h) */
    rv = pc->write(unit, pc->phy_id, 0x17, 0x8F0B);
    if (SOC_FAILURE(rv)) {
        pc->phy_id =  PHYEGPHY16_DEV_PHY_ID_ORIG(pc);
        INT_MCU_UNLOCK(unit);
        return rv;
    }
    rv = pc->read(unit, pc->phy_id, 0x15, &status);

    if (SOC_FAILURE(rv)) {
        pc->phy_id =  PHYEGPHY16_DEV_PHY_ID_ORIG(pc);
        return rv;
    }
#endif

    pc->phy_id =  PHYEGPHY16_DEV_PHY_ID_ORIG(pc);
    INT_MCU_UNLOCK(unit);

    return SOC_E_NONE;
}

/*
 * Function:
 *     phy_egphy16_rdb_reg_read
 * Purpose:
 *     RDB register read
 * Parameters:
 *     reg_addr - RDB register address
 *     data     - the 16-bit value got from the specific RDB register
 * Returns:
 */
STATIC int
phy_egphy16_rdb_reg_read(int unit, phy_ctrl_t *pc,
                         uint16 reg_addr, uint16 *data)
{
    int rv;

    PHYEGPHY16_RDB_ACCESS_MODE(unit, pc);   /* set RDB register access mode */

    /* MDIO write the RDB reg. address to reg.0x1E = <reg_addr> */
    rv = WRITE_PHY_REG(unit, pc,
                       PHYEGPHY16_RDB_ADDR_REG_OFFSET,
                       PHYEGPHY16_RDB_ADDR_REG_ADDR & reg_addr);
    if ( SOC_E_NONE == rv ) {
        /* MDIO read from reg.0x1F to get the RDB register's value as <data> */
        rv = READ_PHY_REG(unit, pc,
                          PHYEGPHY16_RDB_DATA_REG_OFFSET, data);
    }

    if ( SOC_E_NONE != rv ) {          /* failed to access reg. 0x1e or 0x1f */
        _SHR_ERROR_TRACE(rv);
    }

    PHYEGPHY16_LEGACY_ACCESS_MODE(unit, pc);   /* quit RDB assess mode */

    return (rv);
}

/*
 * Function:
 *     phy_egphy16_rdb_reg_write
 * Purpose:
 *     RDB register write
 * Parameters:
 *     reg_addr - RDB register address
 *     data     - 16-bit value writting to the specific RDB register
 * Returns:
 */
STATIC int
phy_egphy16_rdb_reg_write(int unit, phy_ctrl_t *pc,
                          uint16 reg_addr, uint16 data)
{
    int rv;

    PHYEGPHY16_RDB_ACCESS_MODE(unit, pc);   /* set RDB register access mode */

    /* MDIO write the RDB reg. address to reg.0x1E = <reg_addr> */
    rv = WRITE_PHY_REG(unit, pc,
                       PHYEGPHY16_RDB_ADDR_REG_OFFSET,
                       PHYEGPHY16_RDB_ADDR_REG_ADDR & reg_addr);
    if ( SOC_E_NONE == rv ) {
        /* MDIO write to reg.0x1F to set the RDB resister's value as <data> */
        rv = WRITE_PHY_REG(unit, pc,
                           PHYEGPHY16_RDB_DATA_REG_OFFSET, data);
    }

    if ( SOC_E_NONE != rv ) {          /* failed to access reg. 0x1e or 0x1f */
        _SHR_ERROR_TRACE(rv);
    }

    PHYEGPHY16_LEGACY_ACCESS_MODE(unit, pc);   /* quit RDB assess mode */

    return (rv);
}

/*
 * Function:
 *      phy_egphy16_medium_status
 * Purpose:
 *      Indicate the current active medium
 * Parameters:
 *      unit - StrataSwitch unit #.
 *      port - StrataSwitch port #.
 *      medium - (OUT) One of:
 *              SOC_PORT_MEDIUM_COPPER
 *              SOC_PORT_MEDIUM_FIBER
 * Returns:
 *      SOC_E_NONE.
 */
STATIC int
phy_egphy16_medium_status(int unit, soc_port_t port, soc_port_medium_t *medium)
{

    *medium = SOC_PORT_MEDIUM_COPPER;

    return SOC_E_NONE;

}

/*
 * Function:
 *      phy_egphy16_medium_config_set
 * Purpose:
 *      Set the operating parameters that are automatically selected
 *      when medium switches type.
 * Parameters:
 *      unit - StrataSwitch unit #
 *      port - Port number
 *      medium - SOC_PORT_MEDIUM_COPPER/FIBER
 *      cfg - Operating parameters
 * Returns:
 *      SOC_E_XXX
 */
STATIC int
phy_egphy16_medium_config_set(int unit, soc_port_t port,
                              soc_port_medium_t  medium,
                              soc_phy_config_t  *cfg)
{
    COMPILER_REFERENCE(medium);

    SOC_IF_ERROR_RETURN
        (phy_egphy16_speed_set(unit, port, cfg->force_speed));
    SOC_IF_ERROR_RETURN
        (phy_egphy16_duplex_set(unit, port, cfg->force_duplex));
    SOC_IF_ERROR_RETURN
        (phy_egphy16_master_set(unit, port, cfg->master));
    SOC_IF_ERROR_RETURN
        (phy_egphy16_ability_advert_set(unit, port, &cfg->advert_ability));
    SOC_IF_ERROR_RETURN
        (phy_egphy16_autoneg_set(unit, port, cfg->autoneg_enable));
    SOC_IF_ERROR_RETURN
        (phy_egphy16_mdix_set(unit, port, cfg->mdix));

    return SOC_E_NONE;
}

/*
 * Function:
 *      phy_egphy16_medium_config_get
 * Purpose:
 *      Get the operating parameters that are automatically selected
 *      when medium switches type.
 * Parameters:
 *      unit - StrataSwitch unit #
 *      port - Port number
 *      medium - SOC_PORT_MEDIUM_COPPER/FIBER
 *      cfg - (OUT) Operating parameters
 * Returns:
 *      SOC_E_XXX
 */

STATIC int
phy_egphy16_medium_config_get(int unit, soc_port_t port,
                              soc_port_medium_t medium,
                              soc_phy_config_t *cfg)
{
    phy_ctrl_t    *pc;

    COMPILER_REFERENCE(medium);

    pc = EXT_PHY_SW_STATE(unit, port);

    sal_memcpy(cfg, &pc->copper, sizeof (*cfg));

    return SOC_E_NONE;
}


STATIC int
_phy_egphy16_mdi_pair_set(int unit, soc_port_t port, uint32 mdi_pair_map)
{
    phy_ctrl_t *pc;
    uint16  reg_val;
    pc = EXT_PHY_SW_STATE(unit, port);

    if (mdi_pair_map) {
        /*
         * lane 0 (bit0~1), lane 1 (bit2~3), lane 2 (bit4~5), lane 3 (bit6~7)
         * Enable swap (bit 8)
         */
        reg_val = (((mdi_pair_map & (0x3<<12)) >> 6) |
                   ((mdi_pair_map & (0x3<<8)) >> 4)  |
                   ((mdi_pair_map & (0x3<<4)) >> 2)  |
                   (mdi_pair_map & 0x3));
        reg_val |= 0x100; /* Enable */
        SOC_IF_ERROR_RETURN(
            WRITE_PHY_REG(unit, pc, 0x17, 0x0f09));
        SOC_IF_ERROR_RETURN(
            WRITE_PHY_REG(unit, pc, 0x15, reg_val));
    }

    LOG_INFO(BSL_LS_SOC_PORT,
             (BSL_META_U(unit,
                         "_phy_egphy16_mdi_pair_set port %d\n"), port));

    return SOC_E_NONE;
}

static int
_phy56275_ieee_compliance_setup(int unit, phy_ctrl_t *pc)
{
    /*
     * 10bt       - AFE_BIAS_CONFIG_0_RDBr Lower 8b
     * 100bt      - AFE_BIAS_CONFIG_0_RDBr Upper 8b
     * 1000bt     - AFE_BIAS_CONFIG_1_RDBr Lower 8b
     * 1000bt_tm1 - AFE_BIAS_CONFIG_1_RDBr Upper 8b
     */
    uint8_t gphy_100bt_amp = 0xe7; /* Default Value */
    uint16 rd_data = 0, rcalcode = 0, wr_data = 0;

    /*! soc property config variables */
    gphy_100bt_amp = soc_property_port_get(unit, pc->port,
            spn_GPHY_100BT_AMP, gphy_100bt_amp);

    /* CORE_EXP03: Release AFE and PLL */
    SOC_IF_ERROR_RETURN
        (phy_egphy16_rdb_reg_write(unit, pc, CORE_EXP03_RDBr, 0x0006));
    /* CORE_EXP03: Release reset */
    SOC_IF_ERROR_RETURN
        (phy_egphy16_rdb_reg_write(unit, pc, CORE_EXP03_RDBr, 0x0000));
    /* PLL_PLL_RESET_CONTROL : Reset PLL*/
    SOC_IF_ERROR_RETURN
        (phy_egphy16_rdb_reg_write(unit, pc, PLL_PLL_RESET_CONTROL_RDBr, 0x0000));
    /* PLL_NDIV_2 : ndiv_integer */
    SOC_IF_ERROR_RETURN
        (phy_egphy16_rdb_reg_write(unit, pc, PLL_PLL_NDIV_2_RDBr, 0x00A0));
    /* PLL_MANUAL_CONFIG : Set pdiv=1 to avoid auto configuration */
    SOC_IF_ERROR_RETURN
        (phy_egphy16_rdb_reg_write(unit, pc, PLL_PLL_MANUAL_CONFIG_RDBr, 0x0001));
    /* PLL_NDIV: Ndiv_fraction*/
    SOC_IF_ERROR_RETURN
        (phy_egphy16_rdb_reg_write(unit, pc, PLL_PLL_NDIV_RDBr, 0x0000));
    /* PLL_NUDGE: frequency nudge factor*/
    SOC_IF_ERROR_RETURN
        (phy_egphy16_rdb_reg_write(unit, pc, PLL_PLL_NUDGE_RDBr, 0x0000));
    /* PLLREF_REFERENCE: reference frequency: 25MHz, mode 0*/
    SOC_IF_ERROR_RETURN
        (phy_egphy16_rdb_reg_write(unit, pc, PLL_PLLREF_REFERENCE_RDBr, 0x0019));
    /* PLL_CONTROLS_2: Default init_bypass mode*/
    SOC_IF_ERROR_RETURN
        (phy_egphy16_rdb_reg_write(unit, pc, PLL_PLL_CONTROLS_2_RDBr, 0x0000));
    /* PLL_CONTROLS: Bypass code - Default, vcoclk enabled */
    SOC_IF_ERROR_RETURN
        (phy_egphy16_rdb_reg_write(unit, pc, PLL_PLL_CONTROLS_RDBr, 0x0002));
    /* PLL_LDO_CONTROL: LDOs at default settings*/
    SOC_IF_ERROR_RETURN
        (phy_egphy16_rdb_reg_write(unit, pc, PLL_LDO_CONTROL_RDBr, 0x01c0));
    /*
     * PLL_CONFIG_OVERRIDE (PLL_MANUAL_CONFIG_2):
     * config override, dco offset - default setting
     */
    SOC_IF_ERROR_RETURN
        (phy_egphy16_rdb_reg_write(unit, pc, PLL_PLL_MANUAL_CONFIG_2_RDBr, 0x0000));
    /* CORE_EXP03: Release PLL reset*/
    SOC_IF_ERROR_RETURN
        (phy_egphy16_rdb_reg_write(unit, pc, PLL_PLL_RESET_CONTROL_RDBr, 0x0001));
    sal_usleep(10000);
    /* AFE_BG_CONFIG: Bandgap curvature correction to correct default */
    SOC_IF_ERROR_RETURN
        (phy_egphy16_rdb_reg_write(unit, pc, AFE_BG_CONFIG_RDBr, 0x0010));
    /* AFE_CAL_CONFIG_2: enable max averaging*/
    SOC_IF_ERROR_RETURN
        (phy_egphy16_rdb_reg_write(unit, pc, AFE_CAL_CONFIG_2_RDBr, 0x0038));
    sal_usleep(10000);
    /* AFE_CAL_CONFIG_2: no reset and analog powerup */
    SOC_IF_ERROR_RETURN
        (phy_egphy16_rdb_reg_write(unit, pc, AFE_CAL_CONFIG_2_RDBr, 0x003b));
    sal_usleep(10000);
    /* AFE_CAL_CONFIG_2: start calibration */
    SOC_IF_ERROR_RETURN
        (phy_egphy16_rdb_reg_write(unit, pc, AFE_CAL_CONFIG_2_RDBr, 0x003f));
    /* AFE_CAL_CONFIG_0: Vref=1000, Target=10, averaging enabled*/
    SOC_IF_ERROR_RETURN
        (phy_egphy16_rdb_reg_write(unit, pc, AFE_CAL_CONFIG_0_RDBr, 0x1c82));
    sal_usleep(10000);
    /* AFE_CAL_CONFIG_0: no reset and analog powerup*/
    SOC_IF_ERROR_RETURN
        (phy_egphy16_rdb_reg_write(unit, pc, AFE_CAL_CONFIG_0_RDBr, 0x9e82));
    sal_usleep(10000);
    /* AFE_CAL_CONFIG_0: start calibration */
    SOC_IF_ERROR_RETURN
        (phy_egphy16_rdb_reg_write(unit, pc, AFE_CAL_CONFIG_0_RDBr, 0x9f82));
    /* AFE_CAL_CONFIG_0: clear start */
    SOC_IF_ERROR_RETURN
        (phy_egphy16_rdb_reg_write(unit, pc, AFE_CAL_CONFIG_0_RDBr, 0x9e86));
    /* AFE_CAL_CONFIG_0: start calibration */
    SOC_IF_ERROR_RETURN
        (phy_egphy16_rdb_reg_write(unit, pc, AFE_CAL_CONFIG_0_RDBr, 0x9f86));
    /* AFE_CAL_CONFIG_2: select Rext*/
    SOC_IF_ERROR_RETURN
        (phy_egphy16_rdb_reg_write(unit, pc, AFE_CAL_CONFIG_2_RDBr, 0x007f));
    /* AFE_BIAS_CONFIG_0: Adjust 10BT amplitude additional +7% and 100BT +2%*/
    wr_data = (gphy_100bt_amp << 8) | 0xea;
    SOC_IF_ERROR_RETURN
        (phy_egphy16_rdb_reg_write(unit, pc, AFE_BIAS_CONFIG_0_RDBr, wr_data));
    /* AFE_BIAS_CONFIG_1: Adjust 1G mode amplitude and 1G testmode1*/
    SOC_IF_ERROR_RETURN
        (phy_egphy16_rdb_reg_write(unit, pc, AFE_BIAS_CONFIG_1_RDBr, 0xede0));
    /* CORE_EXPA9: Read data */
    SOC_IF_ERROR_RETURN
        (phy_egphy16_rdb_reg_read(unit, pc, CORE_EXPA9_RDBr, &rd_data));
    rcalcode = (rd_data & 0x007E) >> 1;
    rcalcode += 34;
    if (rcalcode > 0x3F) {
        rcalcode = 0x3F;
    }
    wr_data = (rcalcode << 8) + 0xf8;
    /* AFE_CAL_CONFIG_2: 10BT BIAS code adjust after RCAL */
    SOC_IF_ERROR_RETURN
        (phy_egphy16_rdb_reg_write(unit, pc, AFE_CAL_CONFIG_2_RDBr, wr_data));
    /* AFE_BIAS_CONFIG_0: Adjust 10BT amplitude additional +7% andi 100BT +2% */
    wr_data = (gphy_100bt_amp << 8) | 0xdd;
    SOC_IF_ERROR_RETURN
        (phy_egphy16_rdb_reg_write(unit, pc, AFE_BIAS_CONFIG_0_RDBr, wr_data));
    /*
     * AFE_RXCONFIG_3:
     * invert adc clock output and 'adc refp ldo current To correct default
     */
    SOC_IF_ERROR_RETURN
        (phy_egphy16_rdb_reg_write(unit, pc, AFE_RXCONFIG_3_RDBr, 0x8002));
    /*
     * AFE_TX_CONFIG_1:
     * 100BT stair case, high BW, 1G stair case, alternate encode
     */
    SOC_IF_ERROR_RETURN
        (phy_egphy16_rdb_reg_write(unit, pc, AFE_TX_CONFIG_1_RDBr, 0xf882));
    /*
     * AFE_TX_CONFIG_2:
     * 1000BT DAC transition method per Erol, bits[32], DAC Shuffle sequence 1
     */
    SOC_IF_ERROR_RETURN
        (phy_egphy16_rdb_reg_write(unit, pc, AFE_TX_CONFIG_2_RDBr, 0x0201));
    /* AFE_RXCONFIG_1: some non-overlap fix*/
    SOC_IF_ERROR_RETURN
        (phy_egphy16_rdb_reg_write(unit, pc, AFE_RXCONFIG_1_RDBr, 0x0c00));
    /* AFE_RXCONFIG_0 */
    SOC_IF_ERROR_RETURN
        (phy_egphy16_rdb_reg_write(unit, pc, AFE_RXCONFIG_0_RDBr, 0x0020));
    /* AFE_RXCONFIG_CH0 */
    SOC_IF_ERROR_RETURN
        (phy_egphy16_rdb_reg_write(unit, pc, AFE_RXCONFIG_CH0_RDBr, 0x0000));
    /* AFE_RXCONFIG_CH1 */
    SOC_IF_ERROR_RETURN
        (phy_egphy16_rdb_reg_write(unit, pc, AFE_RXCONFIG_CH1_RDBr, 0x0000));
    /* AFE_RXCONFIG_CH2 */
    SOC_IF_ERROR_RETURN
        (phy_egphy16_rdb_reg_write(unit, pc, AFE_RXCONFIG_CH2_RDBr, 0x0000));
    /* AFE_RXCONFIG_CH3 */
    SOC_IF_ERROR_RETURN
        (phy_egphy16_rdb_reg_write(unit, pc, AFE_RXCONFIG_CH3_RDBr, 0x0000));
    /* AFE_RXCONFIG_2 */
    SOC_IF_ERROR_RETURN
        (phy_egphy16_rdb_reg_write(unit, pc, AFE_RXCONFIG_2_RDBr, 0x0800));
    sal_usleep(10000);
    sal_usleep(10000);
    /* AFE_RXCONFIG_0 */
    SOC_IF_ERROR_RETURN
        (phy_egphy16_rdb_reg_write(unit, pc, AFE_RXCONFIG_0_RDBr, 0x0000));

#if 0
    /* Turn off AOF. AFE_TX_CONFIG_0 */
    SOC_IF_ERROR_RETURN
        (phy_egphy16_rdb_reg_write(unit, pc, 0x1E5, 0x0000));
    /* 1g AB symmetry Iq. AFE_TX_CONFIG_1 */
    SOC_IF_ERROR_RETURN
        (phy_egphy16_rdb_reg_write(unit, pc, 0x1EA, 0x0BCC));
    /* LPF BW. AFE_TX_IQ_RX_LP */
    SOC_IF_ERROR_RETURN
        (phy_egphy16_rdb_reg_write(unit, pc, 0x1E4, 0x233F));
    /* RCAL +6LSB to make impedance from 112 to 100ohm. AFE_TEMPSEN_OTHERS */
    SOC_IF_ERROR_RETURN
        (phy_egphy16_rdb_reg_write(unit, pc, 0x1EC, 0xC600));
    /* Enable DSP register access */
    SOC_IF_ERROR_RETURN
        (phy_egphy16_rdb_reg_write(unit, pc, 0x028, 0x4C30));
    /* Make master current -4. DSP_TAP10 */
    SOC_IF_ERROR_RETURN
        (phy_egphy16_rdb_reg_write(unit, pc, 0x0125, 0x191B));
    /* Disabl DSP register access */
    SOC_IF_ERROR_RETURN
        (phy_egphy16_rdb_reg_write(unit, pc, 0x028, 0x4430));
#endif

    return SOC_E_NONE;
}

STATIC int
_phy_egphy16_ieee_compliance_setup(int unit, soc_port_t port)
{
    phy_ctrl_t *pc;

    pc = EXT_PHY_SW_STATE(unit, port);

    if (PHY_IS_BCM56275GPHY(pc)) {
        return _phy56275_ieee_compliance_setup(unit, pc);
    }
#if 0
    if (!PHY_IS_BCM56275GPHY(pc)) {
        return SOC_E_NONE;
    }

    if (pc->phy_rev == BCM56275_A0_REV_ID) {
        /* Set AFE_TX_CONFIG_0(0x1E4) to 0x00C0 */
        SOC_IF_ERROR_RETURN
            (phy_egphy16_rdb_reg_write(unit, pc, 0x1E4, 0x00C0));

        /* Set AFE_VDAC_OTHERS_0(0x1E7) to 0xB008 */
        SOC_IF_ERROR_RETURN
            (phy_egphy16_rdb_reg_write(unit, pc, 0x1E7, 0xB008));

        /* Set AFE_RXCONFIG_2(0x1E2) to 0x02E3 */
        SOC_IF_ERROR_RETURN
            (phy_egphy16_rdb_reg_write(unit, pc, 0x1E2, 0x02E3));

        /* Set AFE_RXCONFIG_0(0x1E0) to 0x0D11 */
        SOC_IF_ERROR_RETURN
            (phy_egphy16_rdb_reg_write(unit, pc, 0x1E0, 0x0D11));

        /* Set AFE_RX_LP_COUNTER(0x1E3) to 0x7FC0 */
        SOC_IF_ERROR_RETURN
            (phy_egphy16_rdb_reg_write(unit, pc, 0x1E3, 0x7FC0));

        /* Set TEMPSEN_OTHERS(0x1EB) to 0x6B40 */
        SOC_IF_ERROR_RETURN
            (phy_egphy16_rdb_reg_write(unit, pc, 0x1EB, 0x6B40));

        /* Set AFE_HPF_TRIM_OTHERS(0x1E8) to 0x0213 */
        SOC_IF_ERROR_RETURN
            (phy_egphy16_rdb_reg_write(unit, pc, 0x1E8, 0x0213));

        /* Set AFE_TX_CONFIG_1(0x1E9) to 0x0020 */
        SOC_IF_ERROR_RETURN
            (phy_egphy16_rdb_reg_write(unit, pc, 0x1E9, 0x0020));

        /* Set CORE_SHD18_000(0x28) to 0x4C30 */
        SOC_IF_ERROR_RETURN
            (phy_egphy16_rdb_reg_write(unit, pc, 0x28, 0x4C30));

        /* Set DSP_TAP10(0x125) to 0x211B */
        SOC_IF_ERROR_RETURN
            (phy_egphy16_rdb_reg_write(unit, pc, 0x125, 0x211B));

        /* Set CORE_BASE1E(0xE) to 0x0013 */
        SOC_IF_ERROR_RETURN
            (phy_egphy16_rdb_reg_write(unit, pc, 0xE, 0x0013));

        /* Set CORE_EXPB0(0xB0) to 0x000C */
        SOC_IF_ERROR_RETURN
            (phy_egphy16_rdb_reg_write(unit, pc, 0xB0, 0x000C));

        /* Set CORE_EXPB0(0xB0) to 0x0000 */
        SOC_IF_ERROR_RETURN
            (phy_egphy16_rdb_reg_write(unit, pc, 0xB0, 0x0000));

    } else if (pc->phy_rev == BCM56160_B0_REV_ID) {
        /* Set AFE_TX_CONFIG_0(0x1E4) to 0x0000 */
        SOC_IF_ERROR_RETURN
            (phy_egphy16_rdb_reg_write(unit, pc, 0x1E4, 0x0000));

        /* Set AFE_VDAC_OTHERS_0(0x1E7) to 0xB008 */
        SOC_IF_ERROR_RETURN
            (phy_egphy16_rdb_reg_write(unit, pc, 0x1E7, 0xB008));

        /* Set AFE_TXCONFIG_2(0x1EA) to 0x2900 */
        SOC_IF_ERROR_RETURN
            (phy_egphy16_rdb_reg_write(unit, pc, 0x1EA, 0x2900));

        /* Set AFE_VDAC_ICTRL_0(0x1E5) to 0xA7AB */
        SOC_IF_ERROR_RETURN
            (phy_egphy16_rdb_reg_write(unit, pc, 0x1E5, 0xA7AB));

        /* Set AFE_HPF_TRIM_OTHERS(0x1E8) to 0x00C3 */
        SOC_IF_ERROR_RETURN
            (phy_egphy16_rdb_reg_write(unit, pc, 0x1E8, 0x00C3));

        /* Set AFE_TX_CONFIG_1(0x1E9) to 0x00CC */
        SOC_IF_ERROR_RETURN
            (phy_egphy16_rdb_reg_write(unit, pc, 0x1E9, 0x00CC));

        /* Set CORE_SHD18_000(0x28) to 0x4C30 */
        SOC_IF_ERROR_RETURN
            (phy_egphy16_rdb_reg_write(unit, pc, 0x28, 0x4C30));

        /* Set DSP_TAP10(0x125) to 0x191B */
        SOC_IF_ERROR_RETURN
            (phy_egphy16_rdb_reg_write(unit, pc, 0x125, 0x191B));

        /* Set CORE_EXPB0(0xB0) to 0x000C */
        SOC_IF_ERROR_RETURN
            (phy_egphy16_rdb_reg_write(unit, pc, 0xB0, 0x000C));

        /* Set CORE_EXPB0(0xB0) to 0x0000 */
        SOC_IF_ERROR_RETURN
            (phy_egphy16_rdb_reg_write(unit, pc, 0xB0, 0x0000));
    }
#endif
    return SOC_E_NONE;
}

/*
 * Function:
 *      _phy_egphy16_reset_setup
 * Purpose:
 *      Function to reset the PHY and set up initial operating registers.
 * Parameters:
 *      unit - StrataSwitch unit #.
 *      port - StrataSwitch port #.
 * Returns:
 *      SOC_E_XXX
 */

STATIC int
_phy_egphy16_reset_setup(int unit, soc_port_t port)
{
    phy_ctrl_t        *pc;
    uint16             tmp;
    uint16             phy_addr;
    soc_port_t         primary_port;
    int                index;
    uint32             phy_mdi_pair_map;

    pc     = EXT_PHY_SW_STATE(unit, port);

    SOC_IF_ERROR_RETURN(phy_ge_init(unit, port));

    if (soc_phy_primary_and_offset_get(unit, port, &primary_port, &index) == SOC_E_NONE) {
        /* Do not change the order */
        index &= ~0x80; /* clear reverse bit (PHYA_REV) */
        PHYEGPHY16_DEV_PHY_ID_ORIG(pc) = pc->phy_id;
        PHYEGPHY16_DEV_PHY_SLICE(pc) = index;
        PHYEGPHY16_DEV_SET_BASE_ADDR(pc); /* phy address of port 0 */

        SOC_IF_ERROR_RETURN
            (phy_egphy16_control_set( unit, port, SOC_PHY_CONTROL_PORT_PRIMARY, primary_port));
        SOC_IF_ERROR_RETURN
            (phy_egphy16_control_set( unit, port, SOC_PHY_CONTROL_PORT_OFFSET, index ));
    } else {
        LOG_WARN(BSL_LS_SOC_PHY,
                 (BSL_META_U(unit,
                             "_phy_egphy16_reset_setup: Config property 'phy_port_primary_and_offset' not set for u=%d, p=%d\n"),
                  unit, port));

        phy_addr = pc->phy_id;
        index = phy_addr & 7;

        /* set the default values that are valid for many boards */
        SOC_IF_ERROR_RETURN
            (phy_egphy16_control_set( unit, port, SOC_PHY_CONTROL_PORT_PRIMARY,
                                      (port - index) < 0 ? 0 : (port - index)));
        SOC_IF_ERROR_RETURN
            (phy_egphy16_control_set( unit, port, SOC_PHY_CONTROL_PORT_OFFSET, index ));
    }

    /* Disable super-isolate */
    SOC_IF_ERROR_RETURN
        (MODIFY_PHYEGPHY16_MII_POWER_CTRLr(unit, pc, 0x0, 1U<<5));

    /* remove power down */
    if (pc->copper.enable) {
        tmp = PHY_DISABLED_MODE(unit, port) ? MII_CTRL_PD : 0;
    } else {
        tmp = MII_CTRL_PD;
    }
    SOC_IF_ERROR_RETURN
        (MODIFY_PHYEGPHY16_MII_CTRLr(unit, pc, tmp, MII_CTRL_PD));

#if DISABLE_CLK125
    /* Reduce EMI emissions by disabling the CLK125 pin if not used */
    SOC_IF_ERROR_RETURN
        (MODIFY_PHYEGPHY16_SPARE_CTRL_3r(unit, pc, 0, 1));
#endif

    /* Get Requested LED selectors (defaults are hardware defaults) */
    pc->ledmode[0] = soc_property_port_get(unit, port, spn_PHY_LED1_MODE, 0);
    pc->ledmode[1] = soc_property_port_get(unit, port, spn_PHY_LED2_MODE, 1);
    pc->ledmode[2] = soc_property_port_get(unit, port, spn_PHY_LED3_MODE, 3);
    pc->ledmode[3] = soc_property_port_get(unit, port, spn_PHY_LED4_MODE, 6);
    pc->ledctrl    = soc_property_port_get(unit, port, spn_PHY_LED_CTRL, 0x8);
    pc->ledselect  = soc_property_port_get(unit, port, spn_PHY_LED_SELECT, 0);

    /* Configure Extended Control Register */
    /* Enable LEDs to indicate traffic status */
    SOC_IF_ERROR_RETURN
        (MODIFY_PHYEGPHY16_MII_ECRr(unit, pc, 0x0020, 0x0020));

#if AUTO_MDIX_WHEN_AN_DIS
    /* Enable Auto-MDIX When autoneg disabled */
    SOC_IF_ERROR_RETURN
        (MODIFY_PHYEGPHY16_MII_MISC_CTRLr(unit, pc, 0x0200, 0x0200));
#endif

    /* Enable extended packet length (4.5k through 25k) */
    SOC_IF_ERROR_RETURN
        (MODIFY_PHYEGPHY16_MII_AUX_CTRLr(unit, pc, 0x4000, 0x4000));

    SOC_IF_ERROR_RETURN
        (MODIFY_PHYEGPHY16_MISC_1000X_CTRL2r(unit, pc, 0x0001, 0x0001));

    SOC_IF_ERROR_RETURN
        (MODIFY_PHYEGPHY16_AUX_CTRLr(unit, pc, 0x0002, 0x0002));

    /* Configure LED selectors */
    tmp = ((pc->ledmode[1] & 0xf) << 4) | (pc->ledmode[0] & 0xf);
    SOC_IF_ERROR_RETURN
        (WRITE_PHYEGPHY16_LED_SELECTOR_1r(unit, pc, tmp));

    tmp = ((pc->ledmode[3] & 0xf) << 4) | (pc->ledmode[2] & 0xf);
    SOC_IF_ERROR_RETURN
        (WRITE_PHYEGPHY16_LED_SELECTOR_2r(unit, pc, tmp));

    tmp = (pc->ledctrl & 0x3ff);
    SOC_IF_ERROR_RETURN
        (WRITE_PHYEGPHY16_LED_CTRLr(unit, pc, tmp));

    SOC_IF_ERROR_RETURN
        (WRITE_PHYEGPHY16_EXP_LED_SELECTORr(unit, pc, pc->ledselect));

    /*
     * Configure Auxiliary control register to turn off
     * carrier extension.  The Intel 7131 NIC does not accept carrier
     * extension and gets CRC errors.
     */
    /* Disable carrier extension */
    SOC_IF_ERROR_RETURN
        (MODIFY_PHYEGPHY16_AUX_CTRLr(unit, pc, 0x0040, 0x0040));

    SOC_IF_ERROR_RETURN
        (MODIFY_PHYEGPHY16_MII_AUX_CTRLr(unit, pc, 0x0c00, 0x0c00));
    SOC_IF_ERROR_RETURN(
        WRITE_PHY_REG(unit, pc, 0x17, 0x000e));
    SOC_IF_ERROR_RETURN(
        WRITE_PHY_REG(unit, pc, 0x15, 0x0752));
    SOC_IF_ERROR_RETURN(
        WRITE_PHY_REG(unit, pc, 0x17, 0x000f));
    SOC_IF_ERROR_RETURN(
        WRITE_PHY_REG(unit, pc, 0x15, 0xe04e));
    SOC_IF_ERROR_RETURN
        (MODIFY_PHYEGPHY16_MII_AUX_CTRLr(unit, pc, 0x0400, 0x0c00));

    PHY_FLAGS_SET(unit, port, PHY_FLAGS_EEE_CAPABLE);

    /* Check config for MDI PAIR */
    phy_mdi_pair_map = soc_property_port_get(unit, port,
                                             spn_PHY_MDI_PAIR_MAP, 0);
    if (phy_mdi_pair_map) {
        SOC_IF_ERROR_RETURN(
            _phy_egphy16_mdi_pair_set(unit, port, phy_mdi_pair_map));
    }

    /* 100BASE-TX initialization change for EEE and autogreen mode */
    SOC_IF_ERROR_RETURN
        (MODIFY_PHYEGPHY16_MII_AUX_CTRLr(unit, pc, 0x0c00, 0x0c00));
    SOC_IF_ERROR_RETURN(
        WRITE_PHY_REG(unit, pc, 0x17, 0x4022));
    SOC_IF_ERROR_RETURN(
        WRITE_PHY_REG(unit, pc, 0x15, 0x017b));
    SOC_IF_ERROR_RETURN
        (MODIFY_PHYEGPHY16_MII_AUX_CTRLr(unit, pc, 0x0400, 0x0c00));

    SOC_IF_ERROR_RETURN
        (_phy_egphy16_ieee_compliance_setup(unit, port));

    SOC_IF_ERROR_RETURN
        (phy_ecd_cable_diag_init_40nm(unit, port));

    return SOC_E_NONE;
}


/*
 * Function:
 *      phy_egphy16_init
 * Purpose:
 *      Init function for EGPHY16 PHY.
 * Parameters:
 *      unit - StrataSwitch unit #.
 *      port - StrataSwitch port #.
 * Returns:
 *      SOC_E_XXX
 */
STATIC int
phy_egphy16_init(int unit, soc_port_t port)
{
    phy_ctrl_t         *pc;

    pc     = EXT_PHY_SW_STATE(unit, port);
    pc->interface = SOC_PORT_IF_GMII;

    pc->automedium = FALSE;
    pc->fiber_detect = FALSE;
    pc->fiber.enable = FALSE;

    pc->copper.enable = TRUE;
    pc->copper.preferred = TRUE;
    pc->copper.autoneg_enable = TRUE;
    pc->copper.force_speed = 1000;
    pc->copper.force_duplex = TRUE;
    pc->copper.master = SOC_PORT_MS_AUTO;
    pc->copper.mdix = SOC_PORT_MDIX_AUTO;


    /* Initially configure for the preferred medium. */

    PHY_FLAGS_SET(unit, port, PHY_FLAGS_COPPER);
    PHY_FLAGS_CLR(unit, port, PHY_FLAGS_FIBER);
    PHY_FLAGS_CLR(unit, port, PHY_FLAGS_PASSTHRU);
    PHY_FLAGS_CLR(unit, pc->port, PHY_FLAGS_EEE_ENABLED);
    PHY_FLAGS_CLR(unit, pc->port, PHY_FLAGS_EEE_MODE);

    SOC_IF_ERROR_RETURN
        (_phy_egphy16_reset_setup(unit, port));

    /* Advertise all possible by default */
    SOC_IF_ERROR_RETURN
        (phy_egphy16_ability_local_get(unit, port, &pc->copper.advert_ability));

    SOC_IF_ERROR_RETURN
        (phy_egphy16_medium_config_set(unit, port, 0, &pc->copper));

    return SOC_E_NONE;

}

/*
 * Function:
 *      phy_egphy16_enable_set
 * Purpose:
 *      Enable or disable the physical interface.
 * Parameters:
 *      unit - StrataSwitch unit #.
 *      port - StrataSwitch port #.
 *      enable - Boolean, true = enable PHY, false = disable.
 * Returns:
 *      SOC_E_XXX
 */

STATIC int
phy_egphy16_enable_set(int unit, soc_port_t port, int enable)
{
    phy_ctrl_t    *pc;
    soc_timeout_t  to;
    uint16 power_down, mii_stat;

    pc = EXT_PHY_SW_STATE(unit, port);

    power_down = (enable) ? 0 : MII_CTRL_PD;

    SOC_IF_ERROR_RETURN
        (MODIFY_PHYEGPHY16_MII_CTRLr(unit, pc, power_down, MII_CTRL_PD));

    
    if (!enable) {
        if (PHY_FLAGS_TST(unit, port, PHY_FLAGS_EEE_ENABLED)) {
            soc_timeout_init(&to, 2000000, 0);
            do {
                SOC_IF_ERROR_RETURN
                    (READ_PHYEGPHY16_MII_STATr(unit, pc, &mii_stat));

                if (soc_timeout_check(&to)) {
                    LOG_WARN(BSL_LS_SOC_PHY,
                             (BSL_META_U(unit,
                                         "phy_egphy16.c: link didn't go down after power down: u=%d p=%d\n"),
                              unit, port));
                    break;
                }
            } while(mii_stat & MII_STAT_LA);
        }
    }

    SOC_IF_ERROR_RETURN
        (phy_fe_ge_enable_set(unit, port, enable));

    return SOC_E_NONE;

}

/*
 * Function:
 *      phy_egphy16_enable_get
 * Purpose:
 *      Enable or disable the physical interface for a EGPHY16 device.
 * Parameters:
 *      unit - StrataSwitch unit #.
 *      port - StrataSwitch port #.
 *      enable - (OUT) Boolean, true = enable PHY, false = disable.
 * Returns:
 *      SOC_E_XXX
 */

STATIC int
phy_egphy16_enable_get(int unit, soc_port_t port, int *enable)
{
    SOC_IF_ERROR_RETURN
        (phy_fe_ge_enable_get(unit, port, enable));

    return SOC_E_NONE;
}


/*
 * Function:
 *      phy_egphy16_link_get
 * Purpose:
 *      Determine the current link up/down status for a EGPHY16 device.
 * Parameters:
 *      unit - StrataSwitch unit #.
 *      port - StrataSwitch port #.
 *      link - (OUT) Boolean, true indicates link established.
 * Returns:
 *      SOC_E_XXX
 * Notes:
 *      If using automedium, also switches the mode.
 */

STATIC int
phy_egphy16_link_get(int unit, soc_port_t port, int *link)
{

    SOC_IF_ERROR_RETURN
        (phy_fe_ge_link_get(unit, port, link));

    return SOC_E_NONE;
}

/*
 * Function:
 *      phy_egphy16_duplex_set
 * Purpose:
 *      Set the current duplex mode (forced).
 * Parameters:
 *      unit - StrataSwitch unit #.
 *      port - StrataSwitch port #.
 *      duplex - Boolean, true indicates full duplex, false indicates half.
 * Returns:
 *      SOC_E_XXX
 *      SOC_E_UNAVAIL - Half duplex requested, and not supported.
 * Notes:
 *      The duplex is set only for the ACTIVE medium.
 *      No synchronization performed at this level.
 *      Autonegotiation is not manipulated.
 */

STATIC int
phy_egphy16_duplex_set(int unit, soc_port_t port, int duplex)
{
    phy_ctrl_t    *pc;

    pc = EXT_PHY_SW_STATE(unit, port);

    SOC_IF_ERROR_RETURN
        (phy_fe_ge_duplex_set(unit, port, duplex));

    pc->copper.force_duplex = duplex;

    return SOC_E_NONE;
}

/*
 * Function:
 *      phy_egphy16_duplex_get
 * Purpose:
 *      Get the current operating duplex mode. If autoneg is enabled,
 *      then operating mode is returned, otherwise forced mode is returned.
 * Parameters:
 *      unit - StrataSwitch unit #.
 *      port - StrataSwitch port #.
 *      duplex - (OUT) Boolean, true indicates full duplex, false
 *              indicates half.
 * Returns:
 *      SOC_E_XXX
 * Notes:
 *      The duplex is retrieved for the ACTIVE medium.
 *      No synchronization performed at this level. Autonegotiation is
 *      not manipulated.
 */

STATIC int
phy_egphy16_duplex_get(int unit, soc_port_t port, int *duplex)
{
    SOC_IF_ERROR_RETURN
        (phy_fe_ge_duplex_get(unit, port, duplex));

    return SOC_E_NONE;
}

/*
 * Function:
 *      phy_egphy16_speed_set
 * Purpose:
 *      Set the current operating speed (forced).
 * Parameters:
 *      unit - StrataSwitch unit #.
 *      port - StrataSwitch port #.
 *      speed - Requested speed.
 * Returns:
 *      SOC_E_XXX
 * Notes:
 *      The speed is set only for the ACTIVE medium.
 */

STATIC int
phy_egphy16_speed_set(int unit, soc_port_t port, int speed)
{
    phy_ctrl_t    *pc;
    pc = EXT_PHY_SW_STATE(unit, port);

    SOC_IF_ERROR_RETURN
        (phy_fe_ge_speed_set(unit, port, speed));

    pc->copper.force_speed = speed;

    return SOC_E_NONE;
}

/*
 * Function:
 *      phy_egphy16_speed_get
 * Purpose:
 *      Get the current operating speed for a EGPHY16 device.
 * Parameters:
 *      unit - StrataSwitch unit #.
 *      port - StrataSwitch port #.
 *      duplex - (OUT) Boolean, true indicates full duplex, false
 *              indicates half.
 * Returns:
 *      SOC_E_XXX
 * Notes:
 *      The speed is retrieved for the ACTIVE medium.
 */

STATIC int
phy_egphy16_speed_get(int unit, soc_port_t port, int *speed)
{
    SOC_IF_ERROR_RETURN
        (phy_fe_ge_speed_get(unit, port, speed));

    return SOC_E_NONE;
}

/*
 * Function:
 *      phy_egphy16_master_set
 * Purpose:
 *      Set the current master mode
 * Parameters:
 *      unit - StrataSwitch unit #.
 *      port - StrataSwitch port #.
 *      master - SOC_PORT_MS_*
 * Returns:
 *      SOC_E_XXX
 * Notes:
 *      The master mode is set only for the ACTIVE medium.
 */

STATIC int
phy_egphy16_master_set(int unit, soc_port_t port, int master)
{
    phy_ctrl_t    *pc;

    pc = EXT_PHY_SW_STATE(unit, port);

    SOC_IF_ERROR_RETURN
        (phy_fe_ge_master_set(unit, port, master));

    pc->copper.master = master;

    return SOC_E_NONE;
}

/*
 * Function:
 *      phy_egphy16_master_get
 * Purpose:
 *      Get the current master mode for a EGPHY16 device.
 * Parameters:
 *      unit - StrataSwitch unit #.
 *      port - StrataSwitch port #.
 *      master - (OUT) SOC_PORT_MS_*
 * Returns:
 *      SOC_E_XXX
 * Notes:
 *      The master mode is retrieved for the ACTIVE medium.
 */

STATIC int
phy_egphy16_master_get(int unit, soc_port_t port, int *master)
{
    SOC_IF_ERROR_RETURN
        (phy_fe_ge_master_get(unit, port, master));

    return SOC_E_NONE;
}

/*
 * Function:
 *      phy_egphy16_autoneg_set
 * Purpose:
 *      Enable or disable auto-negotiation on the specified port.
 * Parameters:
 *      unit - StrataSwitch unit #.
 *      port - StrataSwitch port #.
 *      autoneg - Boolean, if true, auto-negotiation is enabled
 *              (and/or restarted). If false, autonegotiation is disabled.
 * Returns:
 *      SOC_E_XXX
 * Notes:
 *      The autoneg mode is set only for the ACTIVE medium.
 */

STATIC int
phy_egphy16_autoneg_set(int unit, soc_port_t port, int autoneg)
{
    int           rv;
    phy_ctrl_t    *pc;

    pc             = EXT_PHY_SW_STATE(unit, port);
    rv             = SOC_E_NONE;

    /* Set auto-neg on PHY */
    rv = phy_fe_ge_an_set(unit, port, autoneg);
    if (SOC_SUCCESS(rv)) {
        pc->copper.autoneg_enable = autoneg ? 1 : 0;
    }

    return SOC_E_NONE;
}

/*
 * Function:
 *      phy_egphy16_autoneg_get
 * Purpose:
 *      Get the current auto-negotiation status (enabled/busy).
 * Parameters:
 *      unit - StrataSwitch unit #.
 *      port - StrataSwitch port #.
 *      autoneg - (OUT) if true, auto-negotiation is enabled.
 *      autoneg_done - (OUT) if true, auto-negotiation is complete. This
 *              value is undefined if autoneg == FALSE.
 * Returns:
 *      SOC_E_XXX
 * Notes:
 *      The autoneg mode is retrieved for the ACTIVE medium.
 */

STATIC int
phy_egphy16_autoneg_get(int unit, soc_port_t port,
                        int *autoneg, int *autoneg_done)
{

    SOC_IF_ERROR_RETURN
        (phy_fe_ge_an_get(unit, port, autoneg, autoneg_done));

    return SOC_E_NONE;
}

/*
 * Function:
 *      phy_egphy16_ability_advert_set
 * Purpose:
 *      Set the current advertisement for auto-negotiation.
 * Parameters:
 *      unit    - StrataSwitch unit #.
 *      port    - StrataSwitch port #.
 *      ability - Port ability indicating supported options/speeds.
 * Returns:
 *      SOC_E_XXX
 */

STATIC int
phy_egphy16_ability_advert_set(int unit, soc_port_t port, soc_port_ability_t *ability)
{
    phy_ctrl_t *pc;
    uint16     eee_ability = 0;

    if (NULL == ability) {
        return (SOC_E_PARAM);
    }

    pc = EXT_PHY_SW_STATE(unit, port);

    SOC_IF_ERROR_RETURN
        (phy_fe_ge_ability_advert_set(unit, port, ability));

    /* EEE settings */
    if (PHY_FLAGS_TST(unit, port, PHY_FLAGS_EEE_CAPABLE)) {
        eee_ability = 0;
        if (ability->eee & SOC_PA_EEE_1GB_BASET) {
            eee_ability |= 0x4;
        }
        if (ability->eee & SOC_PA_EEE_100MB_BASETX) {
            eee_ability |= 0x2;
        }
        if (PHY_FLAGS_TST(unit, port, PHY_FLAGS_EEE_ENABLED)) {
            SOC_IF_ERROR_RETURN
                (MODIFY_PHYEGPHY16_EEE_ADVr(unit, pc, eee_ability, 0x0006));
        } else {
            SOC_IF_ERROR_RETURN
                (MODIFY_PHYEGPHY16_EEE_ADVr(unit, pc, 0x0000, 0x0006));
        }
    }

    return SOC_E_NONE;

}

/*
 * Function:
 *      phy_egphy16_ability_advert_get
 * Purpose:
 *      Get the current advertisement for auto-negotiation.
 * Parameters:
 *      unit    - StrataSwitch unit #.
 *      port    - StrataSwitch port #.
 *      ability - Port ability indicating supported options/speeds.
 * Returns:
 *      SOC_E_XXX
 * Notes:
 *      The advertisement is retrieved for the ACTIVE medium.
 *      No synchronization performed at this level.
 */

STATIC int
phy_egphy16_ability_advert_get(int unit, soc_port_t port, soc_port_ability_t *ability)
{
    phy_ctrl_t *pc;
    uint16      eee_ability = 0;

    if (NULL == ability) {
        return (SOC_E_PARAM);
    }

    pc = EXT_PHY_SW_STATE(unit, port);

    SOC_IF_ERROR_RETURN
        (phy_fe_ge_ability_advert_get(unit, port, ability));

    /* EEE settings */
    if (PHY_FLAGS_TST(unit, port, PHY_FLAGS_EEE_CAPABLE)) {
        SOC_IF_ERROR_RETURN
            (READ_PHYEGPHY16_EEE_ADVr(unit, pc, &eee_ability));
        if (eee_ability & 0x04) {
            ability->eee |= SOC_PA_EEE_1GB_BASET;
        }
        if (eee_ability & 0x02) {
            ability->eee |= SOC_PA_EEE_100MB_BASETX;
        }
    }

    return SOC_E_NONE;
}

/*
 * Function:
 *      phy_egphy16_ability_remote_get
 * Purpose:
 *      Get partners current advertisement for auto-negotiation.
 * Parameters:
 *      unit    - StrataSwitch unit #.
 *      port    - StrataSwitch port #.
 *      ability - Port ability indicating supported options/speeds.
 * Returns:
 *      SOC_E_XXX
 * Notes:
 *      The remote advertisement is retrieved for the ACTIVE medium.
 *      No synchronization performed at this level. If Autonegotiation is
 *      disabled or in progress, this routine will return an error.
 */

STATIC int
phy_egphy16_ability_remote_get(int unit, soc_port_t port, soc_port_ability_t *ability)
{
    phy_ctrl_t       *pc;
    uint16            eee_resolution;

    if (NULL == ability) {
        return (SOC_E_PARAM);
    }

    pc = EXT_PHY_SW_STATE(unit, port);

    SOC_IF_ERROR_RETURN
        (phy_fe_ge_ability_remote_get(unit, port, ability));

    /* EEE settings */
    if (PHY_FLAGS_TST(unit, port, PHY_FLAGS_EEE_CAPABLE)) {
        SOC_IF_ERROR_RETURN
            (READ_PHYEGPHY16_EEE_RESOLUTION_STATr(unit, pc, &eee_resolution));
        if (eee_resolution & 0x04) {
            ability->eee |= SOC_PA_EEE_1GB_BASET;
        }
        if (eee_resolution & 0x02) {
            ability->eee |= SOC_PA_EEE_100MB_BASETX;
        }
    }

    return SOC_E_NONE;
}

/*
 * Function:
 *      phy_egphy16_ability_local_get
 * Purpose:
 *      Get the device's complete abilities.
 * Parameters:
 *      unit - StrataSwitch unit #.
 *      port - StrataSwitch port #.
 *      ability - return device's abilities.
 * Returns:
 *      SOC_E_XXX
 */

STATIC int
phy_egphy16_ability_local_get(int unit, soc_port_t port, soc_port_ability_t *ability)
{

    if (NULL == ability) {
        return SOC_E_PARAM;
    }

    ability->speed_half_duplex  = SOC_PA_SPEED_100MB | SOC_PA_SPEED_10MB;
    ability->speed_full_duplex  = SOC_PA_SPEED_1000MB |SOC_PA_SPEED_100MB |
        SOC_PA_SPEED_10MB;

    ability->pause     = SOC_PA_PAUSE | SOC_PA_PAUSE_ASYMM;

    
    /* No SGMII interface */
    ability->interface = SOC_PA_INTF_GMII;
    ability->medium    = SOC_PA_MEDIUM_COPPER;
    ability->loopback  = SOC_PA_LB_PHY;
    ability->flags     = SOC_PA_AUTONEG;

    /* EEE settings */
    if (PHY_FLAGS_TST(unit, port, PHY_FLAGS_EEE_CAPABLE)) {
        ability->eee = SOC_PA_EEE_1GB_BASET | SOC_PA_EEE_100MB_BASETX;
    }

    return SOC_E_NONE;
}


/*
 * Function:
 *      phy_egphy16_lb_set
 * Purpose:
 *      Set the local PHY loopback mode.
 * Parameters:
 *      unit - StrataSwitch unit #.
 *      port - StrataSwitch port #.
 *      loopback - Boolean: true = enable loopback, false = disable.
 * Returns:
 *      SOC_E_XXX
 * Notes:
 *      The loopback mode is set only for the ACTIVE medium.
 *      No synchronization performed at this level.
 */

STATIC int
phy_egphy16_lb_set(int unit, soc_port_t port, int enable)
{
    phy_ctrl_t *pc;
    int link = FALSE;

    pc = EXT_PHY_SW_STATE(unit, port);

    /* No SGMII interface */
    SOC_IF_ERROR_RETURN(phy_fe_ge_lb_set(unit, port, enable));

    /*
     * Force link state machine into pass state.
     * Thus the PHY is forced to show link up without the link partner.
     * This is WA for no link-up issue about forcing 10/100 Mbps
     * in PHY loopback.
     */

    if (PHY_FLAGS_TST(unit, port, PHY_FLAGS_EEE_CAPABLE)) {
        if (enable) {
            SOC_IF_ERROR_RETURN(
                PHY_LINKUP_EVT(pc->pd, unit, port));
        } else {
            SOC_IF_ERROR_RETURN(
                phy_egphy16_link_get(unit, port, &link));
            if (link == TRUE) {
                SOC_IF_ERROR_RETURN(
                    PHY_LINKUP_EVT(pc->pd, unit, port));
            } else {
                SOC_IF_ERROR_RETURN(
                    PHY_LINKDN_EVT(pc->pd, unit, port));
            }
        }
    }
    return SOC_E_NONE;

}

/*
 * Function:
 *      phy_egphy16_lb_get
 * Purpose:
 *      Get the local PHY loopback mode.
 * Parameters:
 *      unit - StrataSwitch unit #.
 *      port - StrataSwitch port #.
 *      loopback - (OUT) Boolean: true = enable loopback, false = disable.
 * Returns:
 *      SOC_E_XXX
 * Notes:
 *      The loopback mode is retrieved for the ACTIVE medium.
 */

STATIC int
phy_egphy16_lb_get(int unit, soc_port_t port, int *enable)
{

    /* No SGMII interface */
    SOC_IF_ERROR_RETURN(phy_fe_ge_lb_get(unit, port, enable));
    return SOC_E_NONE;

}

/*
 * Function:
 *      phy_egphy16_interface_set
 * Purpose:
 *      Set the current operating mode of the internal PHY.
 *      (Pertaining to the MAC/PHY interface, not the line interface).
 * Parameters:
 *      unit - StrataSwitch unit #.
 *      port - StrataSwitch port #.
 *      pif - one of SOC_PORT_IF_*
 * Returns:
 *      SOC_E_XXX
 */

STATIC int
phy_egphy16_interface_set(int unit, soc_port_t port, soc_port_if_t pif)
{
    COMPILER_REFERENCE(unit);
    COMPILER_REFERENCE(port);
    COMPILER_REFERENCE(pif);

    return SOC_E_NONE;

}

/*
 * Function:
 *      phy_egphy16_interface_get
 * Purpose:
 *      Get the current operating mode of the internal PHY.
 *      (Pertaining to the MAC/PHY interface, not the line interface).
 * Parameters:
 *      unit - StrataSwitch unit #.
 *      port - StrataSwitch port #.
 *      pif - (OUT) one of SOC_PORT_IF_*
 * Returns:
 *      SOC_E_XXX
 */

STATIC int
phy_egphy16_interface_get(int unit, soc_port_t port, soc_port_if_t *pif)
{
    phy_ctrl_t *pc;

    pc = EXT_PHY_SW_STATE(unit, port);

    /* No SGMII interface */
    *pif = pc->interface;
    return (SOC_E_NONE);
}

/*
 * Function:
 *      phy_egphy16_mdix_set
 * Description:
 *      Set the Auto-MDIX mode of a port/PHY
 * Parameters:
 *      unit - Device number
 *      port - Port number
 *      mode - One of:
 *              SOC_PORT_MDIX_AUTO
 *                      Enable auto-MDIX when autonegotiation is enabled
 *              SOC_PORT_MDIX_FORCE_AUTO
 *                      Enable auto-MDIX always
 *              SOC_PORT_MDIX_NORMAL
 *                      Disable auto-MDIX
 *              SOC_PORT_MDIX_XOVER
 *                      Disable auto-MDIX, and swap cable pairs
 * Return Value:
 *      SOC_E_XXX
 */
STATIC int
phy_egphy16_mdix_set(int unit, soc_port_t port, soc_port_mdix_t mode)
{
    phy_ctrl_t    *pc;
    int            speed;

    pc = EXT_PHY_SW_STATE(unit, port);

    switch (mode) {
    case SOC_PORT_MDIX_AUTO:
        /* Clear bit 14 for automatic MDI crossover */
        SOC_IF_ERROR_RETURN
            (MODIFY_PHYEGPHY16_MII_ECRr(unit, pc, 0, 0x4000));

        /*
         * Write the result in the register 0x18, shadow copy 7
         */
        /* Clear bit 9 to disable forced auto MDI xover */
        SOC_IF_ERROR_RETURN
            (MODIFY_PHYEGPHY16_MII_MISC_CTRLr(unit, pc, 0, 0x0200));
        break;

    case SOC_PORT_MDIX_FORCE_AUTO:
        /* Clear bit 14 for automatic MDI crossover */
        SOC_IF_ERROR_RETURN
            (MODIFY_PHYEGPHY16_MII_ECRr(unit, pc, 0, 0x4000));

        /*
         * Write the result in the register 0x18, shadow copy 7
         */
        /* Set bit 9 to force automatic MDI crossover */
        SOC_IF_ERROR_RETURN
            (MODIFY_PHYEGPHY16_MII_MISC_CTRLr(unit, pc, 0x0200, 0x0200));
        break;

    case SOC_PORT_MDIX_NORMAL:
        SOC_IF_ERROR_RETURN(phy_egphy16_speed_get(unit, port, &speed));
        if (speed == 0 || speed == 10 || speed == 100) {
            /* Set bit 14 for manual MDI crossover */
            SOC_IF_ERROR_RETURN
                (MODIFY_PHYEGPHY16_MII_ECRr(unit, pc, 0x4000, 0x4000));

            SOC_IF_ERROR_RETURN
                (WRITE_PHYEGPHY16_TEST1r(unit, pc, 0));
        } else {
            return SOC_E_UNAVAIL;
        }
        break;

    case SOC_PORT_MDIX_XOVER:
        SOC_IF_ERROR_RETURN(phy_egphy16_speed_get(unit, port, &speed));
        if (speed == 0 || speed == 10 || speed == 100) {
            /* Set bit 14 for manual MDI crossover */
            SOC_IF_ERROR_RETURN
                (MODIFY_PHYEGPHY16_MII_ECRr(unit, pc, 0x4000, 0x4000));

            SOC_IF_ERROR_RETURN
                (WRITE_PHYEGPHY16_TEST1r(unit, pc, 0x0080));
        } else {
            return SOC_E_UNAVAIL;
        }
        break;

    default:
        return SOC_E_PARAM;
    }

    pc->copper.mdix = mode;
    return SOC_E_NONE;

}

#if defined(BCM_WARM_BOOT_SUPPORT)
/*
 * Function:
 *      _phy_egphy16_mdix_wb_update
 * Description:
 *      Read the hw, derive the mode and update the mdix mode in sw structure
 *      during wb.
 * Parameters:
 *      unit - (IN) Device number
 *      port - (IN) Port number
 *      mode - (OUT) :One of:
 *              SOC_PORT_MDIX_AUTO
 *                      Enable auto-MDIX when autonegotiation is enabled
 *              SOC_PORT_MDIX_FORCE_AUTO
 *                      Enable auto-MDIX always
 *              SOC_PORT_MDIX_NORMAL
 *                      Disable auto-MDIX
 *              SOC_PORT_MDIX_XOVER
 *                      Disable auto-MDIX, and swap cable pairs
 * Return Value:
 *      SOC_E_XXX
 */
STATIC int
_phy_egphy16_mdix_wb_update(int unit, soc_port_t port)
{
    phy_ctrl_t    *pc;
    int            speed;
    int            mode = 0;
    uint16         val = 0;

    pc = EXT_PHY_SW_STATE(unit, port);

    /* Check bit 14 for automatic MDI crossover */
    SOC_IF_ERROR_RETURN (READ_PHYEGPHY16_MII_ECRr(unit, pc, &val));
    if(val & 0x4000) {
        SOC_IF_ERROR_RETURN(phy_egphy16_speed_get(unit, port, &speed));
        if (speed == 0 || speed == 10 || speed == 100) {
            SOC_IF_ERROR_RETURN (READ_PHYEGPHY16_TEST1r(unit, pc, &val));
            if (0 == val) {
                mode = SOC_PORT_MDIX_NORMAL;
            } else if (0x80 == val) {
                mode = SOC_PORT_MDIX_XOVER;
            } else {
                return SOC_E_UNAVAIL;
            }
        } else {
            return SOC_E_UNAVAIL;
        }
    } else {
        /* Check bit 9 forced auto MDI xover */
        SOC_IF_ERROR_RETURN (READ_PHYEGPHY16_MII_MISC_CTRLr(unit, pc,&val));
        if (val & 0x200) {
            /* bit 9 Set, forced auto MDI xover */
            mode = SOC_PORT_MDIX_FORCE_AUTO;
        } else {
            /* bit 9 Clear, forced auto MDI xover */
            mode = SOC_PORT_MDIX_AUTO;
        }
    }
    pc->copper.mdix = mode;

    return SOC_E_NONE;
}
#endif /* defined(BCM_WARM_BOOT_SUPPORT) */

/*
 * Function:
 *      phy_egphy16_mdix_get
 * Description:
 *      Get the Auto-MDIX mode of a port/PHY
 * Parameters:
 *      unit - Device number
 *      port - Port number
 *      mode - (Out) One of:
 *              SOC_PORT_MDIX_AUTO
 *                      Enable auto-MDIX when autonegotiation is enabled
 *              SOC_PORT_MDIX_FORCE_AUTO
 *                      Enable auto-MDIX always
 *              SOC_PORT_MDIX_NORMAL
 *                      Disable auto-MDIX
 *              SOC_PORT_MDIX_XOVER
 *                      Disable auto-MDIX, and swap cable pairs
 * Return Value:
 *      SOC_E_XXX
 */
STATIC int
phy_egphy16_mdix_get(int unit, soc_port_t port, soc_port_mdix_t *mode)
{
    phy_ctrl_t    *pc;
    int            speed;

#if defined(BCM_WARM_BOOT_SUPPORT)
    if (SOC_WARM_BOOT(unit)) {
        return _phy_egphy16_mdix_wb_update(unit, port);
    }
#endif /* defined(BCM_WARM_BOOT_SUPPORT) */

    pc = EXT_PHY_SW_STATE(unit, port);

    SOC_IF_ERROR_RETURN(phy_egphy16_speed_get(unit, port, &speed));
    if (speed == 1000) {
        *mode = SOC_PORT_MDIX_AUTO;
    } else {
        *mode = pc->copper.mdix;
    }

    return SOC_E_NONE;
}

/*
 * Function:
 *      phy_egphy16_mdix_status_get
 * Description:
 *      Get the current MDIX status on a port/PHY
 * Parameters:
 *      unit    - Device number
 *      port    - Port number
 *      status  - (OUT) One of:
 *              SOC_PORT_MDIX_STATUS_NORMAL
 *                      Straight connection
 *              SOC_PORT_MDIX_STATUS_XOVER
 *                      Crossover has been performed
 * Return Value:
 *      SOC_E_XXX
 */
STATIC int
phy_egphy16_mdix_status_get(int unit, soc_port_t port,
                            soc_port_mdix_status_t *status)
{
    phy_ctrl_t    *pc;
    uint16               tmp;

    pc = EXT_PHY_SW_STATE(unit, port);

    SOC_IF_ERROR_RETURN
        (READ_PHYEGPHY16_MII_ESRr(unit, pc, &tmp));
    
    if (tmp & 0x2000) {
        *status = SOC_PORT_MDIX_STATUS_XOVER;
    } else {
        *status = SOC_PORT_MDIX_STATUS_NORMAL;
    }

    return SOC_E_NONE;
}

STATIC int
_phy_egphy16_power_mode_set (int unit, soc_port_t port, int mode)
{
    phy_ctrl_t    *pc;

    pc       = EXT_PHY_SW_STATE(unit, port);

    if (pc->power_mode == mode) {
        return SOC_E_NONE;
    }

    
    if (mode == SOC_PHY_CONTROL_POWER_LOW) {
        return SOC_E_UNAVAIL;
    } else if (mode == SOC_PHY_CONTROL_POWER_FULL) {
        return SOC_E_UNAVAIL;
    } else if (mode == SOC_PHY_CONTROL_POWER_AUTO) {
        SOC_IF_ERROR_RETURN
            (MODIFY_PHYEGPHY16_AUTO_POWER_DOWNr(unit,pc,
                                                PHY_EGPHY16_AUTO_PWRDWN_EN,
                                                PHY_EGPHY16_AUTO_PWRDWN_EN));
    } else if (mode == SOC_PHY_CONTROL_POWER_AUTO_DISABLE) {
        /*
           Power mode added(SDK-30895). Requirement is to disable Auto Power Down
           without changing Tx bias current
         */
        SOC_IF_ERROR_RETURN
            (MODIFY_PHYEGPHY16_AUTO_POWER_DOWNr(unit,pc,
                                                0,
                                                PHY_EGPHY16_AUTO_PWRDWN_EN));

    }

    pc->power_mode = mode;

    return SOC_E_NONE;
}

/*
 * Function:
 *      _phy_egphy16_eee_enable
 * Purpose:
 *      Enable or disable EEE (Native)
 * Parameters:
 *      unit   - StrataSwitch unit #.
 *      port   - StrataSwitch port #.
 *      enable - Enable Native EEE
 * Returns:
 *      SOC_E_NONE
 */
STATIC int
_phy_egphy16_eee_enable(int unit, soc_port_t port, int enable)
{
    phy_ctrl_t *pc;

    if (!PHY_FLAGS_TST(unit, port, PHY_FLAGS_EEE_CAPABLE)) {
        return SOC_E_FAIL;
    }
    pc = EXT_PHY_SW_STATE(unit, port);

    if (enable == 1) {
        SOC_IF_ERROR_RETURN
            (MODIFY_PHYEGPHY16_EEE_ADVr(unit, pc, 0x0006, 0x0006));
        pc->copper.advert_ability.eee |= (SOC_PA_EEE_1GB_BASET | SOC_PA_EEE_100MB_BASETX);

    } else {  /* enable != 1 */
        SOC_IF_ERROR_RETURN
            (MODIFY_PHYEGPHY16_EEE_ADVr(unit, pc, 0x0000, 0x0006));
        pc->copper.advert_ability.eee &= ~(SOC_PA_EEE_1GB_BASET | SOC_PA_EEE_100MB_BASETX);
    }

    return SOC_E_NONE;
}


/*
 * Function:
 *      phy_egphy16_control_set
 * Purpose:
 *      Configure PHY device specific control fucntion.
 * Parameters:
 *      unit  - StrataSwitch unit #.
 *      port  - StrataSwitch port #.
 *      type  - Control to update
 *      value - New setting for the control
 * Returns:
 *      SOC_E_NONE
 */
STATIC int
phy_egphy16_control_set(int unit, soc_port_t port,
                        soc_phy_control_t type, uint32 value)
{
    int rv;
    phy_ctrl_t *pc;
    uint16 data;
    soc_port_t primary;
    int offset, saved_autoneg;

    PHY_CONTROL_TYPE_CHECK(type);

    pc = EXT_PHY_SW_STATE(unit, port);
    rv = SOC_E_NONE;

    switch(type) {
    case SOC_PHY_CONTROL_POWER:
        rv = _phy_egphy16_power_mode_set(unit,port,value);
        break;

    case SOC_PHY_CONTROL_DLL_POWER_AUTO:
        /*
           Deep Green Mode Control(PHY-401) - to enable/diable DLL Auto Power Down
         */
        if(1 == value) { /* enable */
            SOC_IF_ERROR_RETURN
                (MODIFY_PHYEGPHY16_SPARE_CTRL_3r(unit, pc,
                                                 0,
                                                 PHY_EGPHY16_SPARE_CTRL3_REG_DLL_AUTO_PWR_DOWN));

        } else if(0 == value){ /* disable */
            SOC_IF_ERROR_RETURN
                (MODIFY_PHYEGPHY16_SPARE_CTRL_3r(unit, pc,
                                                 PHY_EGPHY16_SPARE_CTRL3_REG_DLL_AUTO_PWR_DOWN,
                                                 PHY_EGPHY16_SPARE_CTRL3_REG_DLL_AUTO_PWR_DOWN));
        } else {
            rv = SOC_E_PARAM;
        }
        break;

    case SOC_PHY_CONTROL_POWER_AUTO_WAKE_TIME:
        if (value <= PHY_EGPHY16_AUTO_PWRDWN_WAKEUP_MAX) {

            /* at least one unit */
            if (value < PHY_EGPHY16_AUTO_PWRDWN_WAKEUP_UNIT) {
                value = PHY_EGPHY16_AUTO_PWRDWN_WAKEUP_UNIT;
            }
        } else { /* anything more then max, set to the max */
            value = PHY_EGPHY16_AUTO_PWRDWN_WAKEUP_MAX;
        }
        SOC_IF_ERROR_RETURN
            (MODIFY_PHYEGPHY16_AUTO_POWER_DOWNr(unit,pc,
                                                value/PHY_EGPHY16_AUTO_PWRDWN_WAKEUP_UNIT,
                                                PHY_EGPHY16_AUTO_PWRDWN_WAKEUP_MASK));
        break;

    case SOC_PHY_CONTROL_POWER_AUTO_SLEEP_TIME:
        /* sleep time configuration is either 2.7s or 5.4 s, default is 2.7s */
        if (value < PHY_EGPHY16_AUTO_PWRDWN_SLEEP_MAX) {
            data = 0; /* anything less than 5.4s, default to 2.7s */
        } else {
            data = PHY_EGPHY16_AUTO_PWRDWN_SLEEP_MASK;
        }
        SOC_IF_ERROR_RETURN
            (MODIFY_PHYEGPHY16_AUTO_POWER_DOWNr(unit,pc,
                                                data,
                                                PHY_EGPHY16_AUTO_PWRDWN_SLEEP_MASK));
        break;

    case SOC_PHY_CONTROL_PORT_PRIMARY:
        SOC_IF_ERROR_RETURN
            (soc_phyctrl_primary_set(unit, port, (soc_port_t)value));
        break;

    case SOC_PHY_CONTROL_PORT_OFFSET:
        SOC_IF_ERROR_RETURN
            (soc_phyctrl_offset_set(unit, port, (int)value));
        break;

    case SOC_PHY_CONTROL_CLOCK_ENABLE:
        SOC_IF_ERROR_RETURN
            (soc_phyctrl_primary_get(unit, port, &primary));
        SOC_IF_ERROR_RETURN
            (soc_phyctrl_offset_get(unit, port, &offset));
        SOC_IF_ERROR_RETURN
            (phy_egphy16_toplvl_reg_read(unit, port, primary, 0x0, &data));
        if ( value ) {
            SOC_IF_ERROR_RETURN
                (phy_egphy16_toplvl_reg_write(unit, port, primary, 0x0,
                                              ( data & 0xf0 ) | ( offset & 0x7 )));
        } else {
            SOC_IF_ERROR_RETURN
                (phy_egphy16_toplvl_reg_write(unit, port, primary, 0x0,
                                              ( data & 0xf0 ) | 0x8 ));
        }
        break;

    case SOC_PHY_CONTROL_CLOCK_SECONDARY_ENABLE:
        SOC_IF_ERROR_RETURN
            (soc_phyctrl_primary_get(unit, port, &primary));
        SOC_IF_ERROR_RETURN
            (soc_phyctrl_offset_get(unit, port, &offset));
        SOC_IF_ERROR_RETURN
            (phy_egphy16_toplvl_reg_read(unit, port, primary, 0x0, &data));
        if ( value ) {
            SOC_IF_ERROR_RETURN
                (phy_egphy16_toplvl_reg_write(unit, port, primary, 0x0,
                                              ( data & 0x0f ) | (( offset & 0x7 )<<4 )));
        } else {
            SOC_IF_ERROR_RETURN
                (phy_egphy16_toplvl_reg_write(unit, port, primary, 0x0,
                                              ( data & 0x0f ) | (0x8<<4) ));
        }
        break;

    case SOC_PHY_CONTROL_LINKDOWN_TRANSMIT:
        /* Not supported for 10/100/1000BASE-T interfaces */
        rv = SOC_E_UNAVAIL;
        break;

    case SOC_PHY_CONTROL_EEE:
        if (!PHY_FLAGS_TST(unit, port, PHY_FLAGS_EEE_CAPABLE)) {
            rv = SOC_E_UNAVAIL;
        }

        if (value == 1) {
            /* Enable Native EEE */
            SOC_IF_ERROR_RETURN
                (_phy_egphy16_eee_enable(unit, port, 1));
            saved_autoneg = pc->copper.autoneg_enable;
            /* Initiate AN */
            SOC_IF_ERROR_RETURN
                (phy_egphy16_autoneg_set(unit, port, 1));
            pc->copper.autoneg_enable = saved_autoneg;
            PHY_FLAGS_SET(unit, pc->port, PHY_FLAGS_EEE_ENABLED);
            PHY_FLAGS_CLR(unit, pc->port, PHY_FLAGS_EEE_MODE);
        } else {
            SOC_IF_ERROR_RETURN
                (_phy_egphy16_eee_enable(unit, port, 0));
            /* Initiate AN if administratively enabled */
            SOC_IF_ERROR_RETURN
                (phy_egphy16_autoneg_set(unit, port, pc->copper.autoneg_enable ? 1 : 0));
            PHY_FLAGS_CLR(unit, pc->port, PHY_FLAGS_EEE_ENABLED);

        }


        break;

    case SOC_PHY_CONTROL_EEE_AUTO:
        
        rv = SOC_E_UNAVAIL;

        break;

    case SOC_PHY_CONTROL_EEE_AUTO_IDLE_THRESHOLD:
        
        rv = SOC_E_UNAVAIL;

        break;


    case SOC_PHY_CONTROL_EEE_AUTO_FIXED_LATENCY:
        
        rv = SOC_E_UNAVAIL;
        break;


    case SOC_PHY_CONTROL_EEE_AUTO_BUFFER_LIMIT:
        /* Buffer limit modification is not allowed */

    case SOC_PHY_CONTROL_EEE_TRANSMIT_WAKE_TIME:
    case SOC_PHY_CONTROL_EEE_RECEIVE_WAKE_TIME:
    case SOC_PHY_CONTROL_EEE_TRANSMIT_SLEEP_TIME:
    case SOC_PHY_CONTROL_EEE_RECEIVE_SLEEP_TIME:
    case SOC_PHY_CONTROL_EEE_TRANSMIT_QUIET_TIME:
    case SOC_PHY_CONTROL_EEE_RECEIVE_QUIET_TIME:
    case SOC_PHY_CONTROL_EEE_TRANSMIT_REFRESH_TIME:
        if (!PHY_FLAGS_TST(unit, port, PHY_FLAGS_EEE_CAPABLE)) {
            rv = SOC_E_UNAVAIL;
        }

        /* Time modification not allowed, IEEE EEE spec constants */
        rv = SOC_E_UNAVAIL;
        break;

    case SOC_PHY_CONTROL_EEE_STATISTICS_CLEAR:
        if (!PHY_FLAGS_TST(unit, port, PHY_FLAGS_EEE_CAPABLE)) {
            rv = SOC_E_UNAVAIL;
        }

        if (value == 1) {
            /* Clearing the stats: Enable clear on Read  */
            SOC_IF_ERROR_RETURN
                (MODIFY_PHYEGPHY16_EXP_MII_BUF_CNTL_STAT1r(unit, pc,
                                                           0x1000, 0x1000));

            /* Read all the stats to clear */
            SOC_IF_ERROR_RETURN
                (READ_PHYEGPHY16_EXP_EEE_TX_EVENTSr(unit, pc, &data));
            SOC_IF_ERROR_RETURN
                (READ_PHYEGPHY16_EXP_EEE_TX_EVENTSr(unit, pc, &data));
            SOC_IF_ERROR_RETURN
                (READ_PHYEGPHY16_EXP_EEE_TX_DURATIONr(unit, pc, &data));
            SOC_IF_ERROR_RETURN
                (READ_PHYEGPHY16_EXP_EEE_TX_DURATIONr(unit, pc, &data));
            SOC_IF_ERROR_RETURN
                (READ_PHYEGPHY16_EXP_EEE_RX_EVENTSr(unit, pc, &data));
            SOC_IF_ERROR_RETURN
                (READ_PHYEGPHY16_EXP_EEE_RX_EVENTSr(unit, pc, &data));
            SOC_IF_ERROR_RETURN
                (READ_PHYEGPHY16_EXP_EEE_RX_DURATIONr(unit, pc, &data));
            SOC_IF_ERROR_RETURN
                (READ_PHYEGPHY16_EXP_EEE_RX_DURATIONr(unit, pc, &data));

            /* Disable Clear on Read  */
            SOC_IF_ERROR_RETURN
                (MODIFY_PHYEGPHY16_EXP_MII_BUF_CNTL_STAT1r(unit, pc,
                                                           0x0000, 0x1000));
        }
        break;

    case SOC_PHY_CONTROL_LOOPBACK_EXTERNAL:
        if (value) {
            int saved_autoneg, saved_speed;

            saved_autoneg = pc->copper.autoneg_enable;
            SOC_IF_ERROR_RETURN
                (phy_egphy16_autoneg_set(unit, port, 0));
            pc->copper.autoneg_enable = saved_autoneg;

            saved_speed = pc->copper.force_speed;
            SOC_IF_ERROR_RETURN
                (phy_egphy16_speed_set(unit, port, 1000));
            pc->copper.force_speed = saved_speed;

            SOC_IF_ERROR_RETURN
                (MODIFY_PHYEGPHY16_MII_GB_CTRLr(unit, pc, (1U << 12 | 1U << 11), (1U << 12 | 1U << 11)));
            SOC_IF_ERROR_RETURN
                (MODIFY_PHYEGPHY16_MII_AUX_CTRLr(unit, pc, (1U << 15 | 1U << 10), (1U << 15 | 1U << 10)));
        } else {
            SOC_IF_ERROR_RETURN
                (MODIFY_PHYEGPHY16_MII_GB_CTRLr(unit, pc, 0, (1U << 12 | 1U << 11)));
            SOC_IF_ERROR_RETURN
                (MODIFY_PHYEGPHY16_MII_AUX_CTRLr(unit, pc, 0, (1U << 15 | 1U << 10)));
            SOC_IF_ERROR_RETURN
                (phy_egphy16_speed_set(unit, port, pc->copper.force_speed));
            SOC_IF_ERROR_RETURN
                (phy_egphy16_autoneg_set(unit, port, pc->copper.autoneg_enable ? 1 : 0));
        }
        break;

    default:
        rv = SOC_E_UNAVAIL;
        break;
    }
    return rv;
}

/*
 * Function:
 *      phy_egphy16_control_get
 * Purpose:
 *      Get current control settign of the PHY.
 * Parameters:
 *      unit  - StrataSwitch unit #.
 *      port  - StrataSwitch port #.
 *      type  - Control to update
 *      value - (OUT)Current setting for the control
 * Returns:
 *      SOC_E_NONE
 */
STATIC int
phy_egphy16_control_get(int unit, soc_port_t port,
                        soc_phy_control_t type, uint32 *value)
{
    int rv;
    phy_ctrl_t *pc;
    uint16 data;
    uint32 temp;
    soc_port_t primary;
    int offset;

    PHY_CONTROL_TYPE_CHECK(type);

    pc = EXT_PHY_SW_STATE(unit, port);
    rv = SOC_E_NONE;

    switch(type) {
    case SOC_PHY_CONTROL_POWER:
        *value = pc->power_mode;
        break;

    case SOC_PHY_CONTROL_POWER_AUTO_SLEEP_TIME:
        SOC_IF_ERROR_RETURN
            (READ_PHYEGPHY16_AUTO_POWER_DOWNr(unit,pc, &data));

        if (data & PHY_EGPHY16_AUTO_PWRDWN_SLEEP_MASK) {
            *value = PHY_EGPHY16_AUTO_PWRDWN_SLEEP_MAX;
        } else {
            *value = 2700;
        }
        break;

    case SOC_PHY_CONTROL_POWER_AUTO_WAKE_TIME:
        SOC_IF_ERROR_RETURN
            (READ_PHYEGPHY16_AUTO_POWER_DOWNr(unit,pc, &data));

        data &= PHY_EGPHY16_AUTO_PWRDWN_WAKEUP_MASK;
        *value = data * PHY_EGPHY16_AUTO_PWRDWN_WAKEUP_UNIT;
        break;

    case SOC_PHY_CONTROL_PORT_PRIMARY:
        SOC_IF_ERROR_RETURN
            (soc_phyctrl_primary_get(unit, port, &primary));
        *value = (uint32) primary;
        break;

    case SOC_PHY_CONTROL_PORT_OFFSET:
        SOC_IF_ERROR_RETURN
            (soc_phyctrl_offset_get(unit, port, &offset));
        *value = (uint32) offset;
        break;

    case SOC_PHY_CONTROL_CLOCK_ENABLE:
        SOC_IF_ERROR_RETURN
            (soc_phyctrl_primary_get(unit, port, &primary));
        SOC_IF_ERROR_RETURN
            (soc_phyctrl_offset_get(unit, port, &offset));
        if (phy_egphy16_toplvl_reg_read(unit, port, primary, 0x0, &data) == SOC_E_NONE) {
            if (( data & 0x8 ) || (( data & 0x7 ) != offset)) {
                *value = FALSE;
            } else {
                *value = TRUE;
            }
        }
        break;

    case SOC_PHY_CONTROL_CLOCK_SECONDARY_ENABLE:
        SOC_IF_ERROR_RETURN
            (soc_phyctrl_primary_get(unit, port, &primary));
        SOC_IF_ERROR_RETURN
            (soc_phyctrl_offset_get(unit, port, &offset));
        if (phy_egphy16_toplvl_reg_read(unit, port, primary, 0x0, &data) == SOC_E_NONE) {
            if (( data & (0x8<<4) ) || (( data & (0x7<<4) ) != (offset<<4) )) {
                *value = FALSE;
            } else {
                *value = TRUE;
            }
        }
        break;

    case SOC_PHY_CONTROL_LINKDOWN_TRANSMIT:
        /* Not supported for 10/100/1000BASE-T interfaces */
        rv = SOC_E_UNAVAIL;
        break;

    case SOC_PHY_CONTROL_EEE:
        if (!PHY_FLAGS_TST(unit, port, PHY_FLAGS_EEE_CAPABLE)) {
            rv = SOC_E_UNAVAIL;
        }
        *value = (PHY_FLAGS_TST(unit, port, PHY_FLAGS_EEE_ENABLED) &&
                  !PHY_FLAGS_TST(unit, port, PHY_FLAGS_EEE_MODE)) ? 1 : 0;
        break;

    case SOC_PHY_CONTROL_EEE_AUTO:
        rv = SOC_E_UNAVAIL;
        break;

    case SOC_PHY_CONTROL_EEE_AUTO_FIXED_LATENCY:
        rv = SOC_E_UNAVAIL;
        break;

    case SOC_PHY_CONTROL_EEE_AUTO_IDLE_THRESHOLD:
        rv = SOC_E_UNAVAIL;
        break;

    case SOC_PHY_CONTROL_EEE_TRANSMIT_EVENTS:
        rv = SOC_E_UNAVAIL;
        break;

    case SOC_PHY_CONTROL_EEE_TRANSMIT_DURATION:
        rv = SOC_E_UNAVAIL;
        break;

    case SOC_PHY_CONTROL_EEE_RECEIVE_EVENTS:
        if (!PHY_FLAGS_TST(unit, port, PHY_FLAGS_EEE_CAPABLE)) {
            rv = SOC_E_UNAVAIL;
        }
        SOC_IF_ERROR_RETURN
            (PHYEGPHY16_REG_MODIFY(unit, pc, 0x00, 0x0FAF,0x15, 0x0, 1U<<14)); /* exp af */
        SOC_IF_ERROR_RETURN
            (READ_PHYEGPHY16_EXP_EEE_RX_EVENTSr(unit, pc, &data));
        temp = data;
        SOC_IF_ERROR_RETURN
            (PHYEGPHY16_REG_MODIFY(unit, pc, 0x00, 0x0FAF,0x15, 1U<<14, 1U<<14)); /* exp af */
        SOC_IF_ERROR_RETURN
            (READ_PHYEGPHY16_EXP_EEE_RX_EVENTSr(unit, pc, &data));
        temp |= (data << 16);
        *value = temp;
        break;

    case SOC_PHY_CONTROL_EEE_RECEIVE_DURATION:
        if (!PHY_FLAGS_TST(unit, port, PHY_FLAGS_EEE_CAPABLE)) {
            rv = SOC_E_UNAVAIL;
        }
        SOC_IF_ERROR_RETURN
            (PHYEGPHY16_REG_MODIFY(unit, pc, 0x00, 0x0FAF,0x15, 0x0, 1U<<14)); /* exp af */
        SOC_IF_ERROR_RETURN
            (READ_PHYEGPHY16_EXP_EEE_RX_DURATIONr(unit, pc, &data));
        temp = data;
        SOC_IF_ERROR_RETURN
            (PHYEGPHY16_REG_MODIFY(unit, pc, 0x00, 0x0FAF,0x15, 1U<<14, 1U<<14)); /* exp af */
        SOC_IF_ERROR_RETURN
            (READ_PHYEGPHY16_EXP_EEE_RX_DURATIONr(unit, pc, &data));
        temp |= (data << 16);
        *value = temp;
        break;

    case SOC_PHY_CONTROL_LOOPBACK_EXTERNAL:
        SOC_IF_ERROR_RETURN
            (READ_PHYEGPHY16_MII_AUX_CTRLr(unit, pc, &data));
        *value = data & (1U<<15) ? 1 : 0;
        break;

    default:
        rv = SOC_E_UNAVAIL;
        break;
    }
    return rv;
}

/*
 * Function:
 *      phy_egphy16_cable_diag
 * Purpose:
 *      Run 546x cable diagnostics
 * Parameters:
 *      unit - device number
 *      port - port number
 *      status - (OUT) cable diagnotic status structure
 * Returns:
 *      SOC_E_XXX
 */
STATIC int
phy_egphy16_cable_diag(int unit, soc_port_t port,
                       soc_port_cable_diag_t *status)
{
    /*ECD_CTRL :break link */
    uint16 ecd_ctrl = 0x1000;

    if (SOC_IS_HURRICANE4(unit)) {
        SOC_FEATURE_SET(unit, soc_feature_wh2_miim_delay);
    }

    SOC_IF_ERROR_RETURN
        (phy_ecd_cable_diag_init_40nm(unit, port));

    SOC_IF_ERROR_RETURN
        (phy_ecd_cable_diag_40nm(unit, port, status, ecd_ctrl));

    if (SOC_IS_HURRICANE4(unit)) {
        SOC_FEATURE_CLEAR(unit, soc_feature_wh2_miim_delay);
    }

    return SOC_E_NONE;
}

/*
 * Function:
 *      phy_egphy16_probe
 * Purpose:
 *      Complement the generic phy probe routine to identify this phy when its
 *      phy id0 and id1 is same as some other phy's.
 * Parameters:
 *      unit - StrataSwitch unit #.
 *      pc   - phy ctrl descriptor.
 * Returns:
 *      SOC_E_NONE,SOC_E_NOT_FOUND and SOC_E_<error>
 */
STATIC int
phy_egphy16_probe(int unit, phy_ctrl_t *pc)
{
    uint16 id0, id1;
    int rv = SOC_E_NOT_FOUND;

    if (READ_PHYEGPHY16_MII_PHY_ID0r(unit, pc, &id0) < 0) {
        return rv;
    }
    if (READ_PHYEGPHY16_MII_PHY_ID1r(unit, pc, &id1) < 0) {
        return rv;
    }

    switch (PHY_MODEL(id0, id1)) {

    case PHY_BCM56275GPHY_MODEL:
        rv = SOC_E_NONE;
        break;

    default:
        break;

    }
    pc->size = sizeof(PHYEGPHY16_DEV_DESC_t);

    return rv;
}


STATIC int
phy_egphy16_link_up(int unit, soc_port_t port)
{
    phy_ctrl_t *pc;
    int speed;
    uint16  phy_auto, value;


    pc = EXT_PHY_SW_STATE(unit, port);

    SOC_IF_ERROR_RETURN
        (phy_egphy16_speed_get(unit, port, &speed));

    if (speed == 100) {
        SOC_IF_ERROR_RETURN(
            READ_PHY_REG(unit, pc, 0x11, &value));
        if(value & (1U << 13)) {
            SOC_IF_ERROR_RETURN
                (PHYEGPHY16_REG_MODIFY(unit, pc, 0x00, 0x0FF5,0x15, 0x0<<10, 0x3<<10)); /* exp reg f5 */
        } else {
            SOC_IF_ERROR_RETURN
                (PHYEGPHY16_REG_MODIFY(unit, pc, 0x00, 0x0FF5,0x15, 0x1<<10, 0x3<<10)); /* exp reg f5 */
        }
    }

    if (PHY_FLAGS_TST(unit, port, PHY_FLAGS_EEE_ENABLED)) { /* EEE */
        sal_usleep(10000);
        SOC_IF_ERROR_RETURN(
            READ_PHYEGPHY16_AUTO_POWER_DOWNr(unit, pc, &phy_auto));
        if ((phy_auto & PHY_EGPHY16_AUTO_PWRDWN_EN) != 0) {
            /* Disable PHY Auto Power-Down mode */
            SOC_IF_ERROR_RETURN(
                MODIFY_PHYEGPHY16_AUTO_POWER_DOWNr(unit, pc,
                                                   0, PHY_EGPHY16_AUTO_PWRDWN_EN));
        }
        SOC_IF_ERROR_RETURN(
            MODIFY_PHYEGPHY16_MII_AUX_CTRLr(unit, pc, (1U << 11), (1U << 11)));
        SOC_IF_ERROR_RETURN(
            WRITE_PHY_REG(unit, pc, 0x17, 0x001a));
        SOC_IF_ERROR_RETURN(
            WRITE_PHY_REG(unit, pc, 0x15, 0x0003));
        SOC_IF_ERROR_RETURN(
            MODIFY_PHYEGPHY16_MII_AUX_CTRLr(unit, pc, 0, (1U << 11)));
        if ((phy_auto & PHY_EGPHY16_AUTO_PWRDWN_EN) != 0) {
            /* Enable PHY Auto Power-Down mode */
            SOC_IF_ERROR_RETURN(
                MODIFY_PHYEGPHY16_AUTO_POWER_DOWNr(unit, pc,
                                                   PHY_EGPHY16_AUTO_PWRDWN_EN, PHY_EGPHY16_AUTO_PWRDWN_EN));
        }
    }

    /* Notify interface to internal PHY */
    SOC_IF_ERROR_RETURN
        (soc_phyctrl_notify(unit, port, phyEventInterface, SOC_PORT_IF_GMII));

    /* Situation observed with Saber devices XGXS16g1l+BCM54640E
       Linkscan is not updating speed for internal phy xgxs16g1l.
       It was observed that when speed is set to 100M the int phy
       is still set to 1G and traffic does not flow through untill
       int phy speed is also update to 100M. This applies to other
       speeds as well.
       Explicitly send speed notify to int phy from ext phy.
     */
    SOC_IF_ERROR_RETURN
        (soc_phyctrl_notify(unit, port, phyEventSpeed, speed));


    return SOC_E_NONE;
}

STATIC int
phy_egphy16_link_down(int unit, soc_port_t port)
{
    phy_ctrl_t *pc;
    uint16  phy_auto;

    if (!PHY_FLAGS_TST(unit, port, PHY_FLAGS_EEE_ENABLED)) { /*non EEE */
        return SOC_E_NONE;
    }

    pc = EXT_PHY_SW_STATE(unit, port);

    SOC_IF_ERROR_RETURN(
        READ_PHYEGPHY16_AUTO_POWER_DOWNr(unit, pc, &phy_auto));
    if ((phy_auto & PHY_EGPHY16_AUTO_PWRDWN_EN) != 0) {
        /* Disable PHY Auto Power-Down mode */
        SOC_IF_ERROR_RETURN(
            MODIFY_PHYEGPHY16_AUTO_POWER_DOWNr(unit, pc,
                                               0, PHY_EGPHY16_AUTO_PWRDWN_EN));
    }
    SOC_IF_ERROR_RETURN(
        MODIFY_PHYEGPHY16_MII_AUX_CTRLr(unit, pc, (1U << 11), (1U << 11)));
    SOC_IF_ERROR_RETURN(
        WRITE_PHY_REG(unit, pc, 0x17, 0x001a));
    SOC_IF_ERROR_RETURN(
        WRITE_PHY_REG(unit, pc, 0x15, 0x0007));
    SOC_IF_ERROR_RETURN(
        MODIFY_PHYEGPHY16_MII_AUX_CTRLr(unit, pc, 0, (1U << 11)));
    if ((phy_auto & PHY_EGPHY16_AUTO_PWRDWN_EN) != 0) {
        /* Enable PHY Auto Power-Down mode */
        SOC_IF_ERROR_RETURN(
            MODIFY_PHYEGPHY16_AUTO_POWER_DOWNr(unit, pc,
                                               PHY_EGPHY16_AUTO_PWRDWN_EN, PHY_EGPHY16_AUTO_PWRDWN_EN));
    }

    return SOC_E_NONE;
}

STATIC int
phy_egphy16_cable_diag_dispatch(int unit, soc_port_t port,
                                soc_port_cable_diag_t *status)
{
    SOC_IF_ERROR_RETURN(
        phy_egphy16_cable_diag(unit, port, status));

    return SOC_E_NONE;
}

void _phy_egphy16_encode_egress_message_mode(soc_port_phy_timesync_event_message_egress_mode_t mode,
                                             int offset, uint16 *value)
{

    switch (mode) {
    case SOC_PORT_PHY_TIMESYNC_EVENT_MESSAGE_EGRESS_MODE_NONE:
        *value |= (0x0 << offset);
        break;
    case SOC_PORT_PHY_TIMESYNC_EVENT_MESSAGE_EGRESS_MODE_UPDATE_CORRECTIONFIELD:
        *value |= (0x1 << offset);
        break;
    case SOC_PORT_PHY_TIMESYNC_EVENT_MESSAGE_EGRESS_MODE_REPLACE_CORRECTIONFIELD_ORIGIN:
        *value |= (0x2 << offset);
        break;
    case SOC_PORT_PHY_TIMESYNC_EVENT_MESSAGE_EGRESS_MODE_CAPTURE_TIMESTAMP:
        *value |= (0x3 << offset);
        break;
    default:
        break;
    }

}

void _phy_egphy16_encode_ingress_message_mode(soc_port_phy_timesync_event_message_ingress_mode_t mode,
                                              int offset, uint16 *value)
{

    switch (mode) {
    case SOC_PORT_PHY_TIMESYNC_EVENT_MESSAGE_INGRESS_MODE_NONE:
        *value |= (0x0 << offset);
        break;
    case SOC_PORT_PHY_TIMESYNC_EVENT_MESSAGE_INGRESS_MODE_UPDATE_CORRECTIONFIELD:
        *value |= (0x1 << offset);
        break;
    case SOC_PORT_PHY_TIMESYNC_EVENT_MESSAGE_INGRESS_MODE_INSERT_TIMESTAMP:
        *value |= (0x2 << offset);
        break;
    case SOC_PORT_PHY_TIMESYNC_EVENT_MESSAGE_INGRESS_MODE_INSERT_DELAYTIME:
        *value |= (0x3 << offset);
        break;
    default:
        break;
    }

}

void _phy_egphy16_decode_egress_message_mode(uint16 value, int offset,
                                             soc_port_phy_timesync_event_message_egress_mode_t *mode)
{

    switch ((value >> offset) & 0x3) {
    case 0x0:
        *mode = SOC_PORT_PHY_TIMESYNC_EVENT_MESSAGE_EGRESS_MODE_NONE;
        break;
    case 0x1:
        *mode = SOC_PORT_PHY_TIMESYNC_EVENT_MESSAGE_EGRESS_MODE_UPDATE_CORRECTIONFIELD;
        break;
    case 0x2:
        *mode = SOC_PORT_PHY_TIMESYNC_EVENT_MESSAGE_EGRESS_MODE_REPLACE_CORRECTIONFIELD_ORIGIN;
        break;
    case 0x3:
        *mode = SOC_PORT_PHY_TIMESYNC_EVENT_MESSAGE_EGRESS_MODE_CAPTURE_TIMESTAMP;
        break;
    }

}

void _phy_egphy16_decode_ingress_message_mode(uint16 value, int offset,
                                              soc_port_phy_timesync_event_message_ingress_mode_t *mode)
{

    switch ((value >> offset) & 0x3) {
    case 0x0:
        *mode = SOC_PORT_PHY_TIMESYNC_EVENT_MESSAGE_INGRESS_MODE_NONE;
        break;
    case 0x1:
        *mode = SOC_PORT_PHY_TIMESYNC_EVENT_MESSAGE_INGRESS_MODE_UPDATE_CORRECTIONFIELD;
        break;
    case 0x2:
        *mode = SOC_PORT_PHY_TIMESYNC_EVENT_MESSAGE_INGRESS_MODE_INSERT_TIMESTAMP;
        break;
    case 0x3:
        *mode = SOC_PORT_PHY_TIMESYNC_EVENT_MESSAGE_INGRESS_MODE_INSERT_DELAYTIME;
        break;
    }

}

void _phy_egphy16_encode_gmode(soc_port_phy_timesync_global_mode_t mode,
                               uint16 *value)
{

    switch (mode) {
    case SOC_PORT_PHY_TIMESYNC_MODE_FREE:
        *value = 0x1;
        break;
    case SOC_PORT_PHY_TIMESYNC_MODE_SYNCIN:
        *value = 0x2;
        break;
    case SOC_PORT_PHY_TIMESYNC_MODE_CPU:
        *value = 0x3;
        break;
    default:
        break;
    }

}

void _phy_egphy16_decode_gmode(uint16 value,
                               soc_port_phy_timesync_global_mode_t *mode)
{

    switch (value & 0x3) {
    case 0x1:
        *mode = SOC_PORT_PHY_TIMESYNC_MODE_FREE;
        break;
    case 0x2:
        *mode = SOC_PORT_PHY_TIMESYNC_MODE_SYNCIN;
        break;
    case 0x3:
        *mode = SOC_PORT_PHY_TIMESYNC_MODE_CPU;
        break;
    default:
        break;
    }

}

void _phy_egphy16_encode_framesync_mode(soc_port_phy_timesync_framesync_mode_t mode,
                                        uint16 *value)
{

    switch (mode) {
    case SOC_PORT_PHY_TIMESYNC_FRAMESYNC_SYNCIN0:
        *value = 1U << 0;
        break;
    case SOC_PORT_PHY_TIMESYNC_FRAMESYNC_SYNCIN1:
        *value = 1U << 1;
        break;
    case SOC_PORT_PHY_TIMESYNC_FRAMESYNC_SYNCOUT:
        *value = 1U << 2;
        break;
    case SOC_PORT_PHY_TIMESYNC_FRAMESYNC_CPU:
        *value = 1U << 3;
        break;
    default:
        break;
    }

}

void _phy_egphy16_decode_framesync_mode(uint16 value,
                                        soc_port_phy_timesync_framesync_mode_t *mode)
{

    switch (value & 0xf) {
    case 1U << 0:
        *mode = SOC_PORT_PHY_TIMESYNC_FRAMESYNC_SYNCIN0;
        break;
    case 1U << 1:
        *mode = SOC_PORT_PHY_TIMESYNC_FRAMESYNC_SYNCIN1;
        break;
    case 1U << 2:
        *mode = SOC_PORT_PHY_TIMESYNC_FRAMESYNC_SYNCOUT;
        break;
    case 1U << 3:
        *mode = SOC_PORT_PHY_TIMESYNC_FRAMESYNC_CPU;
        break;
    default:
        break;
    }

}

void _phy_egphy16_encode_syncout_mode(soc_port_phy_timesync_syncout_mode_t mode,
                                      uint16 *value)
{

    switch (mode) {
    case SOC_PORT_PHY_TIMESYNC_SYNCOUT_DISABLE:
        *value = 0x0;
        break;
    case SOC_PORT_PHY_TIMESYNC_SYNCOUT_ONE_TIME:
        *value = 0x1;
        break;
    case SOC_PORT_PHY_TIMESYNC_SYNCOUT_PULSE_TRAIN:
        *value = 0x2;
        break;
    case SOC_PORT_PHY_TIMESYNC_SYNCOUT_PULSE_TRAIN_WITH_SYNC:
        *value = 0x3;
        break;
    default:
        break;
    }

}

void _phy_egphy16_decode_syncout_mode(uint16 value,
                                      soc_port_phy_timesync_syncout_mode_t *mode)
{

    switch (value & 0x3) {
    case 0x0:
        *mode = SOC_PORT_PHY_TIMESYNC_SYNCOUT_DISABLE;
        break;
    case 0x1:
        *mode = SOC_PORT_PHY_TIMESYNC_SYNCOUT_ONE_TIME;
        break;
    case 0x2:
        *mode = SOC_PORT_PHY_TIMESYNC_SYNCOUT_PULSE_TRAIN;
        break;
    case 0x3:
        *mode = SOC_PORT_PHY_TIMESYNC_SYNCOUT_PULSE_TRAIN_WITH_SYNC;
        break;
    }

}


STATIC int
phy_egphy16_timesync_config_set(int unit, soc_port_t port, soc_port_phy_timesync_config_t *conf)
{
    phy_ctrl_t *pc;
    int offset, rv = SOC_E_NONE;
    uint16 rx_control_reg = 0, tx_control_reg = 0, rx_crc_control_reg = 0,
           en_control_reg = 0, tx_capture_en_reg = 0, rx_capture_en_reg = 0,
           nse_nco_6 = 0, nse_nco_2c = 0, value, mask,
           gmode = 0, framesync_mode = 0, syncout_mode = 0,
           pulse_1_length, pulse_2_length, length_threshold, event_offset;
    uint32 interval;

    pc = EXT_PHY_SW_STATE(unit, port);

    offset = PHYEGPHY16_DEV_PHY_SLICE(pc);

    do {
        if (conf->validity_mask & SOC_PORT_PHY_TIMESYNC_VALID_FLAGS) {

            if (conf->flags & SOC_PORT_PHY_TIMESYNC_ENABLE) {
                en_control_reg |= ((1U << (offset + 8)) | (1U << offset));
                nse_nco_6 |= (1U << 12); /* NSE init */
                SOC_IF_ERROR_BREAK
                    (PHYEGPHY16_REG_MODIFY(unit, pc, 0x00, 0x0FF5,0x15, 1U<<0, 1U<<0), rv); /* exp reg f5 */
            } else {
                SOC_IF_ERROR_BREAK
                    (PHYEGPHY16_REG_MODIFY(unit, pc, 0x00, 0x0FF5,0x15, 0, 1U<<0), rv); /* exp reg f5 */
            }

            /* pc->phy_id =  PHYEGPHY16_SLICE_ADDR(pc, 0); */
            pc->phy_id =  PHYEGPHY16_DEV_PHY_ID_BASE(pc);

            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_modify(unit, pc, 0, 0x10, en_control_reg,
                                                     ((1U << (offset + 8)) | (1U << offset))), rv);

            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_read(unit, pc, 0, 0x10, &en_control_reg), rv);

            if (conf->flags & SOC_PORT_PHY_TIMESYNC_CAPTURE_TS_ENABLE) {
                tx_capture_en_reg |= (1U << offset);
                rx_capture_en_reg |= (1U << (offset + 8));
            }

            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_modify(unit, pc, 0, 0x12, tx_capture_en_reg,
                                                     (1U << offset)), rv);

            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_modify(unit, pc, 0, 0x13, rx_capture_en_reg,
                                                     (1U << (offset + 8))), rv);

            if (conf->flags & SOC_PORT_PHY_TIMESYNC_HEARTBEAT_TS_ENABLE) {
                nse_nco_6 |= (1U << 13);
            }

            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_modify(unit, pc, 0, 0x4f, nse_nco_6,
                                                     (1U << 13)), rv);

            if (conf->flags & SOC_PORT_PHY_TIMESYNC_RX_CRC_ENABLE) {
                rx_crc_control_reg |= (1U << 3);
            }

            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_write(unit, pc, 0, 0x34, rx_crc_control_reg), rv);

            if (conf->flags & SOC_PORT_PHY_TIMESYNC_8021AS_ENABLE) {
                rx_control_reg |= (1U << 3);
                tx_control_reg |= (1U << 3);
            }

            if (conf->flags & SOC_PORT_PHY_TIMESYNC_L2_ENABLE) {
                rx_control_reg |= (1U << 2);
                tx_control_reg |= (1U << 2);
            }

            if (conf->flags & SOC_PORT_PHY_TIMESYNC_IP4_ENABLE) {
                rx_control_reg |= (1U << 1);
                tx_control_reg |= (1U << 1);
            }

            if (conf->flags & SOC_PORT_PHY_TIMESYNC_IP6_ENABLE) {
                rx_control_reg |= (1U << 0);
                tx_control_reg |= (1U << 0);
            }

            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_write(unit, pc, 0, 0x32, tx_control_reg), rv);

            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_write(unit, pc, 0, 0x33, rx_control_reg), rv);

            if (conf->flags & SOC_PORT_PHY_TIMESYNC_CLOCK_SRC_EXT) {
                nse_nco_2c &= (uint16) ~(1U << 14);
            } else {
                nse_nco_2c |= (1U << 14);
            }

            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_modify(unit, pc, 0, 0x47, nse_nco_2c,
                                                     (1U << 14)), rv);
        }

        if (conf->validity_mask & SOC_PORT_PHY_TIMESYNC_VALID_ITPID) {
            /* VLAN TAG register */
            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_write(unit, pc, 0, 0x35, conf->itpid), rv);
        }

        if (conf->validity_mask & SOC_PORT_PHY_TIMESYNC_VALID_OTPID) {
            /* OUTER VLAN TAG register */
            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_write(unit, pc, 0, 0x36, conf->otpid), rv);
        }

        if (conf->validity_mask & SOC_PORT_PHY_TIMESYNC_VALID_OTPID2) {
            /* INNER VLAN TAG register */
            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_write(unit, pc, 0, 0x52, conf->otpid2), rv);
        }

        if (conf->validity_mask & SOC_PORT_PHY_TIMESYNC_VALID_TS_DIVIDER) {
            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_write(unit, pc, 0, 0x4b, conf->ts_divider & 0xfff), rv);
        }


        if (conf->validity_mask & SOC_PORT_PHY_TIMESYNC_VALID_RX_LINK_DELAY) {
            pc->phy_id =  PHYEGPHY16_DEV_PHY_ID_ORIG(pc);
            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_write(unit, pc, 0, 0x6f, conf->rx_link_delay & 0xffff), rv);
            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_write(unit, pc, 0, 0x70, (conf->rx_link_delay >> 16) & 0xffff), rv);
            pc->phy_id =  PHYEGPHY16_DEV_PHY_ID_BASE(pc);
        }

        if (conf->validity_mask & SOC_PORT_PHY_TIMESYNC_VALID_ORIGINAL_TIMECODE) {
            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_write(unit, pc, 0, 0x25, (uint16)(conf->original_timecode.nanoseconds & 0xffff)), rv);

            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_write(unit, pc, 0, 0x26, (uint16)((conf->original_timecode.nanoseconds >> 16) & 0xffff)), rv);

            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_write(unit, pc, 0, 0x27, (uint16)(COMPILER_64_LO(conf->original_timecode.seconds) & 0xffff)), rv);

            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_write(unit, pc, 0, 0x28, (uint16)((COMPILER_64_LO(conf->original_timecode.seconds) >> 16) & 0xffff)), rv);

            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_write(unit, pc, 0, 0x29, (uint16)(COMPILER_64_HI(conf->original_timecode.seconds) & 0xffff)), rv);

        }
        mask = 0;

        if (conf->validity_mask & SOC_PORT_PHY_TIMESYNC_VALID_GMODE) {
            _phy_egphy16_encode_gmode(conf->gmode,&gmode);
            mask |= (0x3 << 14);
        }

        if (conf->validity_mask & SOC_PORT_PHY_TIMESYNC_VALID_FRAMESYNC_MODE) {
            _phy_egphy16_encode_framesync_mode(conf->framesync.mode, &framesync_mode);
            mask |= (0xf << 2) |  (0x1 << 11);

            /* Divide by 8 */
            length_threshold = conf->framesync.length_threshold >> 3;
            event_offset = conf->framesync.event_offset >> 3;

            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_write(unit, pc, 0, 0x51, length_threshold), rv);
            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_write(unit, pc, 0, 0x50, event_offset), rv);
        }

        if (conf->validity_mask & SOC_PORT_PHY_TIMESYNC_VALID_SYNCOUT_MODE) {
            _phy_egphy16_encode_syncout_mode(conf->syncout.mode, &syncout_mode);
            mask |= (0x3 << 0);

            /* Divide by 8 */
            interval = conf->syncout.interval >> 3;
            pulse_1_length = conf->syncout.pulse_1_length >> 3;
            pulse_2_length = conf->syncout.pulse_2_length >> 3;

            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_write(unit, pc, 0, 0x48, interval & 0xffff), rv);
            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_write(unit, pc, 0, 0x49,
                                                    ((pulse_2_length & 0x3) << 14) | ((interval >> 16) & 0x3fff)), rv);
            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_write(unit, pc, 0, 0x4a,
                                                    ((pulse_1_length & 0x1ff) << 7) | ((pulse_2_length >> 2) & 0x7f)), rv);

            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_write(unit, pc, 0, 0x4e,
                                                    (uint16)(COMPILER_64_LO(conf->syncout.syncout_ts) & 0xffff)), rv);
            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_write(unit, pc, 0, 0x4d,
                                                    (uint16)((COMPILER_64_LO(conf->syncout.syncout_ts) >> 16) & 0xffff)), rv);
            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_write(unit, pc, 0, 0x4c,
                                                    (uint16)(COMPILER_64_HI(conf->syncout.syncout_ts) & 0xffff)), rv);
        }

        SOC_IF_ERROR_BREAK
            (_phy_egphy16_blk_top_lvl_reg_modify(unit, pc, 0, 0x4f, (gmode << 14) |
                                                 ((framesync_mode & 0x2) ? (1U << 11) : (1U << 6)) | /* if bits 1 or 2 are set, turn off syncout */
                                                 (framesync_mode << 2) | (syncout_mode << 0), mask), rv);

        /* tx_timestamp_offset */
        if (conf->validity_mask & SOC_PORT_PHY_TIMESYNC_VALID_TX_TIMESTAMP_OFFSET) {
            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_write(unit, pc, 0, 0x15 + offset, (uint16)(conf->tx_timestamp_offset & 0xffff)), rv);

            if (offset < 4 ) {
                SOC_IF_ERROR_BREAK
                    (_phy_egphy16_blk_top_lvl_reg_modify(unit, pc, 0, 0x63,
                                                         ((conf->tx_timestamp_offset & 0xf0000) >> ((4 - offset) << 2)), 0xf << (offset << 2)), rv);
            } else {
                uint16 offset1 = offset - 4;
                SOC_IF_ERROR_BREAK
                    (_phy_egphy16_blk_top_lvl_reg_modify(unit, pc, 0, 0x64,
                                                         ((conf->tx_timestamp_offset & 0xf0000) >> ((4 - offset1) << 2)), 0xf << (offset1 << 2)), rv);
            }
        }

        /* rx_timestamp_offset */
        if (conf->validity_mask & SOC_PORT_PHY_TIMESYNC_VALID_RX_TIMESTAMP_OFFSET) {
            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_write(unit, pc, 0, 0x1d + offset, (uint16)(conf->rx_timestamp_offset & 0xffff)), rv);

            if (offset < 4 ) {
                SOC_IF_ERROR_BREAK
                    (_phy_egphy16_blk_top_lvl_reg_modify(unit, pc, 0, 0x65,
                                                         ((conf->rx_timestamp_offset & 0xf0000) >> ((4 - offset) << 2)), 0xf << (offset << 2)), rv);
            } else {
                uint16 offset1 = offset - 4;
                SOC_IF_ERROR_BREAK
                    (_phy_egphy16_blk_top_lvl_reg_modify(unit, pc, 0, 0x66,
                                                         ((conf->rx_timestamp_offset & 0xf0000) >> ((4 - offset1) << 2)), 0xf << (offset1 << 2)), rv);
            }
        }

        value = 0;
        mask = 0;

        if (conf->validity_mask & SOC_PORT_PHY_TIMESYNC_VALID_TX_SYNC_MODE) {
            _phy_egphy16_encode_egress_message_mode(conf->tx_sync_mode, 0, &value);
            mask |= 0x3 << 0;
        }

        if (conf->validity_mask & SOC_PORT_PHY_TIMESYNC_VALID_TX_DELAY_REQUEST_MODE) {
            _phy_egphy16_encode_egress_message_mode(conf->tx_delay_request_mode, 2, &value);
            mask |= 0x3 << 2;
        }

        if (conf->validity_mask & SOC_PORT_PHY_TIMESYNC_VALID_TX_PDELAY_REQUEST_MODE) {
            _phy_egphy16_encode_egress_message_mode(conf->tx_pdelay_request_mode, 4, &value);
            mask |= 0x3 << 4;
        }

        if (conf->validity_mask & SOC_PORT_PHY_TIMESYNC_VALID_TX_PDELAY_RESPONSE_MODE) {
            _phy_egphy16_encode_egress_message_mode(conf->tx_pdelay_response_mode, 6, &value);
            mask |= 0x3 << 6;
        }

        pc->phy_id =  PHYEGPHY16_DEV_PHY_ID_ORIG(pc);

        SOC_IF_ERROR_BREAK
            (_phy_egphy16_blk_top_lvl_reg_modify(unit, pc, 0, 0x11, value, mask), rv);

        value = 0;
        mask = 0;

        if (conf->validity_mask & SOC_PORT_PHY_TIMESYNC_VALID_RX_SYNC_MODE) {
            _phy_egphy16_encode_ingress_message_mode(conf->rx_sync_mode, 0, &value);
            mask |= 0x3 << 0;
        }

        if (conf->validity_mask & SOC_PORT_PHY_TIMESYNC_VALID_RX_DELAY_REQUEST_MODE) {
            _phy_egphy16_encode_ingress_message_mode(conf->rx_delay_request_mode, 2, &value);
            mask |= 0x3 << 2;
        }

        if (conf->validity_mask & SOC_PORT_PHY_TIMESYNC_VALID_RX_PDELAY_REQUEST_MODE) {
            _phy_egphy16_encode_ingress_message_mode(conf->rx_pdelay_request_mode, 4, &value);
            mask |= 0x3 << 4;
        }

        if (conf->validity_mask & SOC_PORT_PHY_TIMESYNC_VALID_RX_PDELAY_RESPONSE_MODE) {
            _phy_egphy16_encode_ingress_message_mode(conf->rx_pdelay_response_mode, 6, &value);
            mask |= 0x3 << 6;
        }


        SOC_IF_ERROR_BREAK
            (_phy_egphy16_blk_top_lvl_reg_modify(unit, pc, 0, 0x12, value << 8, mask << 8), rv);

        /* pc->phy_id =  PHYEGPHY16_SLICE_ADDR(pc, 0); */
        pc->phy_id =  PHYEGPHY16_DEV_PHY_ID_BASE(pc);

        if (conf->validity_mask & SOC_PORT_PHY_TIMESYNC_VALID_PHY_1588_DPLL_REF_PHASE) {
            /* Initial ref phase [15:0] */
            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_write(unit, pc, 0, 0x38, (uint16)(COMPILER_64_LO(conf->phy_1588_dpll_ref_phase) & 0xffff)), rv);

            /* Initial ref phase [31:16]  */
            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_write(unit, pc, 0, 0x39, (uint16)((COMPILER_64_LO(conf->phy_1588_dpll_ref_phase) >> 16) & 0xffff)), rv);

            /*  Initial ref phase [47:32] */
            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_write(unit, pc, 0, 0x3a, (uint16)(COMPILER_64_HI(conf->phy_1588_dpll_ref_phase) & 0xffff)), rv);
        }

        if (conf->validity_mask & SOC_PORT_PHY_TIMESYNC_VALID_PHY_1588_DPLL_REF_PHASE_DELTA) {
            /* Ref phase delta [15:0] */
            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_write(unit, pc, 0, 0x3b, (uint16)(conf->phy_1588_dpll_ref_phase_delta & 0xffff)), rv);

            /* Ref phase delta [31:16]  */
            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_write(unit, pc, 0, 0x3c, (uint16)((conf->phy_1588_dpll_ref_phase_delta >> 16) & 0xffff)), rv);
        }

        /* DPLL K1 & K2 */
        mask = 0;

        if (conf->validity_mask & SOC_PORT_PHY_TIMESYNC_VALID_PHY_1588_DPLL_K1) {
            mask |= 0xff << 8;
        }

        if (conf->validity_mask & SOC_PORT_PHY_TIMESYNC_VALID_PHY_1588_DPLL_K2) {
            mask |= 0xff;
        }
        SOC_IF_ERROR_BREAK
            (_phy_egphy16_blk_top_lvl_reg_modify(unit, pc, 0, 0x3d, (conf->phy_1588_dpll_k1 & 0xff)<<8 | (conf->phy_1588_dpll_k2 & 0xff), mask ), rv);

        if (conf->validity_mask & SOC_PORT_PHY_TIMESYNC_VALID_PHY_1588_DPLL_K3) {
            /* DPLL K3 */
            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_write(unit, pc, 0, 0x3e, conf->phy_1588_dpll_k3 & 0xff), rv);
        }


        if (conf->validity_mask & SOC_PORT_PHY_TIMESYNC_VALID_PHY_1588_DPLL_LOOP_FILTER) {
            /* Initial loop filter[15:0] */
            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_write(unit, pc, 0, 0x3f, (uint16)(COMPILER_64_LO(conf->phy_1588_dpll_loop_filter) & 0xffff)), rv);

            /* Initial loop filter[31:16]  */
            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_write(unit, pc, 0, 0x40, (uint16)((COMPILER_64_LO(conf->phy_1588_dpll_loop_filter) >> 16) & 0xffff)), rv);

            /*  Initial loop filter[47:32] */
            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_write(unit, pc, 0, 0x41, (uint16)(COMPILER_64_HI(conf->phy_1588_dpll_loop_filter) & 0xffff)), rv);

            /* Initial loop filter[63:48]  */
            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_write(unit, pc, 0, 0x42, (uint16)((COMPILER_64_HI(conf->phy_1588_dpll_loop_filter) >> 16) & 0xffff)), rv);
        }

    } while(0);

    pc->phy_id =  PHYEGPHY16_DEV_PHY_ID_ORIG(pc);

    return SOC_E_NONE;
}

STATIC int
phy_egphy16_timesync_config_get(int unit, soc_port_t port, soc_port_phy_timesync_config_t *conf)
{
    phy_ctrl_t *pc;
    int offset, rv = SOC_E_NONE;
    uint16 tx_control_reg = 0, rx_crc_control_reg = 0,
           en_control_reg = 0, tx_capture_en_reg = 0,
           nse_nco_6 = 0, nse_nco_2c = 0, temp1, temp2, temp3, temp4, value;
    soc_port_phy_timesync_global_mode_t gmode = SOC_PORT_PHY_TIMESYNC_MODE_FREE;
    soc_port_phy_timesync_framesync_mode_t framesync_mode = SOC_PORT_PHY_TIMESYNC_FRAMESYNC_NONE;
    soc_port_phy_timesync_syncout_mode_t syncout_mode = SOC_PORT_PHY_TIMESYNC_SYNCOUT_DISABLE;

    pc = EXT_PHY_SW_STATE(unit, port);

    conf->flags = 0;

    offset = PHYEGPHY16_DEV_PHY_SLICE(pc);

    do {
        /* pc->phy_id =  PHYEGPHY16_SLICE_ADDR(pc, 0); */
        pc->phy_id =  PHYEGPHY16_DEV_PHY_ID_BASE(pc);

        SOC_IF_ERROR_BREAK
            (_phy_egphy16_blk_top_lvl_reg_read(unit, pc, 0, 0x4f, &nse_nco_6), rv);

        if (conf->validity_mask & SOC_PORT_PHY_TIMESYNC_VALID_FLAGS) {
            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_read(unit, pc, 0, 0x10, &en_control_reg), rv);

            if (en_control_reg & (1U << offset)) {
                conf->flags |= SOC_PORT_PHY_TIMESYNC_ENABLE;
            }

            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_read(unit, pc, 0, 0x12, &tx_capture_en_reg), rv);

            if (tx_capture_en_reg & (1U << offset)) {
                conf->flags |= SOC_PORT_PHY_TIMESYNC_CAPTURE_TS_ENABLE;
            }

            /*
               SOC_IF_ERROR_BREAK
               (_phy_egphy16_blk_top_lvl_reg_read(unit, pc, 0, 0x4f, &nse_nco_6), rv);
             */

            if (nse_nco_6 & (1U << 13)) {
                conf->flags |= SOC_PORT_PHY_TIMESYNC_HEARTBEAT_TS_ENABLE;
            }

            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_read(unit, pc, 0, 0x34, &rx_crc_control_reg), rv);

            if (rx_crc_control_reg & (1U << 3)) {
                conf->flags |= SOC_PORT_PHY_TIMESYNC_RX_CRC_ENABLE;
            }

            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_read(unit, pc, 0, 0x32, &tx_control_reg), rv);

            if (tx_control_reg & (1U << 3)) {
                conf->flags |= SOC_PORT_PHY_TIMESYNC_8021AS_ENABLE;
            }

            if (tx_control_reg & (1U << 2)) {
                conf->flags |= SOC_PORT_PHY_TIMESYNC_L2_ENABLE;
            }

            if (tx_control_reg & (1U << 1)) {
                conf->flags |= SOC_PORT_PHY_TIMESYNC_IP4_ENABLE;
            }

            if (tx_control_reg & (1U << 0)) {
                conf->flags |= SOC_PORT_PHY_TIMESYNC_IP6_ENABLE;
            }

            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_read(unit, pc, 0, 0x47, &nse_nco_2c), rv);

            if (!(nse_nco_2c & (1U << 14))) {
                conf->flags |= SOC_PORT_PHY_TIMESYNC_CLOCK_SRC_EXT;
            }
        }

        if (conf->validity_mask & SOC_PORT_PHY_TIMESYNC_VALID_GMODE) {
            _phy_egphy16_decode_gmode((nse_nco_6  >> 14),&gmode);
            conf->gmode = gmode;
        }

        if (conf->validity_mask & SOC_PORT_PHY_TIMESYNC_VALID_FRAMESYNC_MODE) {
            _phy_egphy16_decode_framesync_mode((nse_nco_6  >> 2), &framesync_mode);
            conf->framesync.mode = framesync_mode;
            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_read(unit, pc, 0, 0x51, &temp1), rv);
            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_read(unit, pc, 0, 0x50, &temp2), rv);

            /* Multiply by 8 */
            conf->framesync.length_threshold = temp1 << 3;
            conf->framesync.event_offset = temp2 << 3;
        }

        if (conf->validity_mask & SOC_PORT_PHY_TIMESYNC_VALID_SYNCOUT_MODE) {
            _phy_egphy16_decode_syncout_mode((nse_nco_6  >> 0), &syncout_mode);
            conf->syncout.mode = syncout_mode;

            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_read(unit, pc, 0, 0x48, &temp1), rv);
            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_read(unit, pc, 0, 0x49, &temp2), rv);
            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_read(unit, pc, 0, 0x4a, &temp3), rv);

            /* Multiply by 8 */
            conf->syncout.pulse_1_length = ((temp3 >> 7) & 0x1ff) << 3;
            conf->syncout.pulse_2_length =  (((temp3 & 0x7f) << 2) | ((temp2 >> 14) & 0x3)) << 3;
            conf->syncout.interval =  (((temp2 & 0x3fff) << 16) | temp1) << 3;

            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_read(unit, pc, 0, 0x4e, &temp1), rv);
            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_read(unit, pc, 0, 0x4d, &temp2), rv);
            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_read(unit, pc, 0, 0x4c, &temp3), rv);

            COMPILER_64_SET(conf->syncout.syncout_ts, ((uint32)temp3),  (((uint32)temp2<<16)|((uint32)temp1)));
        }

        if (conf->validity_mask & SOC_PORT_PHY_TIMESYNC_VALID_ITPID) {
            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_read(unit, pc, 0, 0x35, &conf->itpid), rv);
        }

        if (conf->validity_mask & SOC_PORT_PHY_TIMESYNC_VALID_OTPID) {
            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_read(unit, pc, 0, 0x36, &conf->otpid), rv);
        }

        if (conf->validity_mask & SOC_PORT_PHY_TIMESYNC_VALID_OTPID2) {
            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_read(unit, pc, 0, 0x52, &conf->otpid2), rv);
        }

        if (conf->validity_mask & SOC_PORT_PHY_TIMESYNC_VALID_TS_DIVIDER) {
            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_read(unit, pc, 0, 0x4b, &conf->ts_divider), rv);
        }

        if (conf->validity_mask & SOC_PORT_PHY_TIMESYNC_VALID_RX_LINK_DELAY) {
            pc->phy_id =  PHYEGPHY16_DEV_PHY_ID_ORIG(pc);

            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_read(unit, pc, 0, 0x6f, &temp1), rv);
            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_read(unit, pc, 0, 0x70, &temp2), rv);

            conf->rx_link_delay = ((uint32)temp2 << 16) | temp1;

            /* pc->phy_id =  PHYEGPHY16_SLICE_ADDR(pc, 0); */
            pc->phy_id =  PHYEGPHY16_DEV_PHY_ID_BASE(pc);
        }


        if (conf->validity_mask & SOC_PORT_PHY_TIMESYNC_VALID_ORIGINAL_TIMECODE) {
            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_read(unit, pc, 0, 0x25, &temp1), rv);

            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_read(unit, pc, 0, 0x26, &temp2), rv);

            conf->original_timecode.nanoseconds = ((uint32)temp2 << 16) | temp1;

            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_read(unit, pc, 0, 0x27, &temp1), rv);

            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_read(unit, pc, 0, 0x28, &temp2), rv);

            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_read(unit, pc, 0, 0x29, &temp3), rv);

            /* conf->original_timecode.seconds = ((uint64)temp3 << 32) | ((uint32)temp2 << 16) | temp1; */

            COMPILER_64_SET(conf->original_timecode.seconds, ((uint32)temp3),  (((uint32)temp2<<16)|((uint32)temp1)));
        }

        /* tx_timestamp_offset */
        if (conf->validity_mask & SOC_PORT_PHY_TIMESYNC_VALID_TX_TIMESTAMP_OFFSET) {
            uint32 temp32;

            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_read(unit, pc, 0, 0x15 + offset, &temp1), rv);
            if (offset < 4 ) {
                SOC_IF_ERROR_BREAK
                    (_phy_egphy16_blk_top_lvl_reg_read(unit, pc, 0, 0x63, &temp2), rv);
                temp32 = (temp2 & (0xf << (offset << 2))) << ((4 - offset) << 2);
            } else {
                uint16 offset1 = offset - 4;
                SOC_IF_ERROR_BREAK
                    (_phy_egphy16_blk_top_lvl_reg_read(unit, pc, 0, 0x64, &temp2), rv);
                temp32 = (temp2 & (0xf << (offset1 << 2))) << ((4 - offset1) << 2);
            }
            conf->tx_timestamp_offset = temp32 | temp1;
        }

        /* rx_timestamp_offset */
        if (conf->validity_mask & SOC_PORT_PHY_TIMESYNC_VALID_RX_TIMESTAMP_OFFSET) {
            uint32 temp32;

            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_read(unit, pc, 0, 0x1d + offset, &temp1), rv);
            if (offset < 4 ) {
                SOC_IF_ERROR_BREAK
                    (_phy_egphy16_blk_top_lvl_reg_read(unit, pc, 0, 0x65, &temp2), rv);
                temp32 = (temp2 & (0xf << (offset << 2))) << ((4 - offset) << 2);
            } else {
                uint16 offset1 = offset - 4;
                SOC_IF_ERROR_BREAK
                    (_phy_egphy16_blk_top_lvl_reg_read(unit, pc, 0, 0x66, &temp2), rv);
                temp32 = (temp2 & (0xf << (offset1 << 2))) << ((4 - offset1) << 2);
            }
            conf->rx_timestamp_offset = temp32 | temp1;
        }

        pc->phy_id =  PHYEGPHY16_DEV_PHY_ID_ORIG(pc);

        SOC_IF_ERROR_BREAK
            (_phy_egphy16_blk_top_lvl_reg_read(unit, pc, 0, 0x11, &value), rv);

        if (conf->validity_mask & SOC_PORT_PHY_TIMESYNC_VALID_TX_SYNC_MODE) {
            _phy_egphy16_decode_egress_message_mode(value, 0, &conf->tx_sync_mode);
        }

        if (conf->validity_mask & SOC_PORT_PHY_TIMESYNC_VALID_TX_DELAY_REQUEST_MODE) {
            _phy_egphy16_decode_egress_message_mode(value, 2, &conf->tx_delay_request_mode);
        }

        if (conf->validity_mask & SOC_PORT_PHY_TIMESYNC_VALID_TX_PDELAY_REQUEST_MODE) {
            _phy_egphy16_decode_egress_message_mode(value, 4, &conf->tx_pdelay_request_mode);
        }

        if (conf->validity_mask & SOC_PORT_PHY_TIMESYNC_VALID_TX_PDELAY_RESPONSE_MODE) {
            _phy_egphy16_decode_egress_message_mode(value, 6, &conf->tx_pdelay_response_mode);
        }

        SOC_IF_ERROR_BREAK
            (_phy_egphy16_blk_top_lvl_reg_read(unit, pc, 0, 0x12, &value), rv);

        value >>= 8;

        if (conf->validity_mask & SOC_PORT_PHY_TIMESYNC_VALID_RX_SYNC_MODE) {
            _phy_egphy16_decode_ingress_message_mode(value, 0, &conf->rx_sync_mode);
        }

        if (conf->validity_mask & SOC_PORT_PHY_TIMESYNC_VALID_RX_DELAY_REQUEST_MODE) {
            _phy_egphy16_decode_ingress_message_mode(value, 2, &conf->rx_delay_request_mode);
        }

        if (conf->validity_mask & SOC_PORT_PHY_TIMESYNC_VALID_RX_PDELAY_REQUEST_MODE) {
            _phy_egphy16_decode_ingress_message_mode(value, 4, &conf->rx_pdelay_request_mode);
        }

        if (conf->validity_mask & SOC_PORT_PHY_TIMESYNC_VALID_RX_PDELAY_RESPONSE_MODE) {
            _phy_egphy16_decode_ingress_message_mode(value, 6, &conf->rx_pdelay_response_mode);
        }

        /* pc->phy_id =  PHYEGPHY16_SLICE_ADDR(pc, 0); */
        pc->phy_id =  PHYEGPHY16_DEV_PHY_ID_BASE(pc);

        if (conf->validity_mask & SOC_PORT_PHY_TIMESYNC_VALID_PHY_1588_DPLL_REF_PHASE) {
            /* Initial ref phase [15:0] */
            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_read(unit, pc, 0, 0x38, &temp1), rv);

            /* Initial ref phase [31:16]  */
            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_read(unit, pc, 0, 0x39, &temp2), rv);

            /*  Initial ref phase [47:32] */
            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_read(unit, pc, 0, 0x3a, &temp3), rv);

            COMPILER_64_SET(conf->phy_1588_dpll_ref_phase, ((uint32)temp3), (((uint32)temp2<<16)|((uint32)temp1)));
        }

        if (conf->validity_mask & SOC_PORT_PHY_TIMESYNC_VALID_PHY_1588_DPLL_REF_PHASE_DELTA) {
            /* Ref phase delta [15:0] */
            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_read(unit, pc, 0, 0x3b, &temp1), rv);

            /* Ref phase delta [31:16]  */
            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_read(unit, pc, 0, 0x3c, &temp2), rv);

            conf->phy_1588_dpll_ref_phase_delta = (temp2 << 16) | temp1;
        }

        /* DPLL K1 & K2 */
        SOC_IF_ERROR_BREAK
            (_phy_egphy16_blk_top_lvl_reg_read(unit, pc, 0, 0x3d, &temp1 ), rv);

        if (conf->validity_mask & SOC_PORT_PHY_TIMESYNC_VALID_PHY_1588_DPLL_K1) {
            conf->phy_1588_dpll_k1 = (temp1 >> 8 ) & 0xff;
        }

        if (conf->validity_mask & SOC_PORT_PHY_TIMESYNC_VALID_PHY_1588_DPLL_K2) {
            conf->phy_1588_dpll_k2 = temp1 & 0xff;
        }

        /* DPLL K3 */
        if (conf->validity_mask & SOC_PORT_PHY_TIMESYNC_VALID_PHY_1588_DPLL_K3) {
            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_read(unit, pc, 0, 0x3e, &temp1), rv);
            conf->phy_1588_dpll_k3 = temp1 & 0xff;
        }

        if (conf->validity_mask & SOC_PORT_PHY_TIMESYNC_VALID_PHY_1588_DPLL_LOOP_FILTER) {
            /* Initial loop filter[15:0] */
            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_read(unit, pc, 0, 0x3f, &temp1), rv);

            /* Initial loop filter[31:16]  */
            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_read(unit, pc, 0, 0x40, &temp2), rv);

            /*  Initial loop filter[47:32] */
            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_read(unit, pc, 0, 0x41, &temp3), rv);

            /* Initial loop filter[63:48]  */
            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_read(unit, pc, 0, 0x42, &temp4), rv);

            COMPILER_64_SET(conf->phy_1588_dpll_loop_filter, (((uint32)temp4<<16)|((uint32)temp3)), (((uint32)temp2<<16)|((uint32)temp1)));
        }
    } while(0);

    pc->phy_id =  PHYEGPHY16_DEV_PHY_ID_ORIG(pc);

    return SOC_E_NONE;
}

STATIC int
phy_egphy16_timesync_control_set(int unit, soc_port_t port, soc_port_control_phy_timesync_t type, uint64 value)
{
    phy_ctrl_t *pc;
    uint16 temp1, temp2;
    uint32 value0;
    int offset, rv = SOC_E_NONE;

    pc = EXT_PHY_SW_STATE(unit, port);

    offset = PHYEGPHY16_DEV_PHY_SLICE(pc);

    do {
        /* pc->phy_id =  PHYEGPHY16_SLICE_ADDR(pc, 0); */
        pc->phy_id =  PHYEGPHY16_DEV_PHY_ID_BASE(pc);

        switch (type) {
        case SOC_PORT_CONTROL_PHY_TIMESYNC_CAPTURE_TIMESTAMP:
        case SOC_PORT_CONTROL_PHY_TIMESYNC_HEARTBEAT_TIMESTAMP:
            return SOC_E_FAIL;

        case SOC_PORT_CONTROL_PHY_TIMESYNC_NCOADDEND:
            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_write(unit, pc, 0, 0x44, (uint16)COMPILER_64_LO(value)), rv);
            break;

        case SOC_PORT_CONTROL_PHY_TIMESYNC_FRAMESYNC:
            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_read(unit, pc, 0, 0x4f, &temp1), rv);
            temp2 = ((temp1 | (0x3 << 14) | (0x1 << 12)) & ~(0xf << 2)) | (0x1 << 5);
            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_write(unit, pc, 0, 0x4f, temp2), rv);
            sal_udelay(1);
            temp2 &= ~((0x1 << 5) | (0x1 << 12));
            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_write(unit, pc, 0, 0x4f, temp2), rv);
            sal_udelay(1);
            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_write(unit, pc, 0, 0x4f, temp1), rv);

            break;

        case SOC_PORT_CONTROL_PHY_TIMESYNC_LOCAL_TIME:
            COMPILER_64_SHR(value, 4);
            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_write(unit, pc, 0, 0x45, (uint16)(COMPILER_64_LO(value) & 0xffff)), rv);
            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_write(unit, pc, 0, 0x46, (uint16)((COMPILER_64_LO(value) >> 16) & 0xffff)), rv);
            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_modify(unit, pc, 0, 0x47, (uint16)(COMPILER_64_HI(value) & 0xfff), 0xfff), rv);
            break;

        case SOC_PORT_CONTROL_PHY_TIMESYNC_LOAD_CONTROL:
            temp1 = 0;
            temp2 = 0;

            value0 = COMPILER_64_LO(value);

            if (value0 &  SOC_PORT_PHY_TIMESYNC_TN_LOAD) {
                temp1 |= 1U << 11;
            }
            if (value0 &  SOC_PORT_PHY_TIMESYNC_TN_ALWAYS_LOAD) {
                temp2 |= 1U << 11;
            }
            if (value0 &  SOC_PORT_PHY_TIMESYNC_TIMECODE_LOAD) {
                temp1 |= 1U << 10;
            }
            if (value0 &  SOC_PORT_PHY_TIMESYNC_TIMECODE_ALWAYS_LOAD) {
                temp2 |= 1U << 10;
            }
            if (value0 &  SOC_PORT_PHY_TIMESYNC_SYNCOUT_LOAD) {
                temp1 |= 1U << 9;
            }
            if (value0 &  SOC_PORT_PHY_TIMESYNC_SYNCOUT_ALWAYS_LOAD) {
                temp2 |= 1U << 9;
            }
            if (value0 &  SOC_PORT_PHY_TIMESYNC_NCO_DIVIDER_LOAD) {
                temp1 |= 1U << 8;
            }
            if (value0 &  SOC_PORT_PHY_TIMESYNC_NCO_DIVIDER_ALWAYS_LOAD) {
                temp2 |= 1U << 8;
            }
            if (value0 &  SOC_PORT_PHY_TIMESYNC_LOCAL_TIME_LOAD) {
                temp1 |= 1U << 7;
            }
            if (value0 &  SOC_PORT_PHY_TIMESYNC_LOCAL_TIME_ALWAYS_LOAD) {
                temp2 |= 1U << 7;
            }
            if (value0 &  SOC_PORT_PHY_TIMESYNC_NCO_ADDEND_LOAD) {
                temp1 |= 1U << 6;
            }
            if (value0 &  SOC_PORT_PHY_TIMESYNC_NCO_ADDEND_ALWAYS_LOAD) {
                temp2 |= 1U << 6;
            }
            if (value0 &  SOC_PORT_PHY_TIMESYNC_DPLL_LOOP_FILTER_LOAD) {
                temp1 |= 1U << 5;
            }
            if (value0 &  SOC_PORT_PHY_TIMESYNC_DPLL_LOOP_FILTER_ALWAYS_LOAD) {
                temp2 |= 1U << 5;
            }
            if (value0 &  SOC_PORT_PHY_TIMESYNC_DPLL_REF_PHASE_LOAD) {
                temp1 |= 1U << 4;
            }
            if (value0 &  SOC_PORT_PHY_TIMESYNC_DPLL_REF_PHASE_ALWAYS_LOAD) {
                temp2 |= 1U << 4;
            }
            if (value0 &  SOC_PORT_PHY_TIMESYNC_DPLL_REF_PHASE_DELTA_LOAD) {
                temp1 |= 1U << 3;
            }
            if (value0 &  SOC_PORT_PHY_TIMESYNC_DPLL_REF_PHASE_DELTA_ALWAYS_LOAD) {
                temp2 |= 1U << 3;
            }
            if (value0 &  SOC_PORT_PHY_TIMESYNC_DPLL_K3_LOAD) {
                temp1 |= 1U << 2;
            }
            if (value0 &  SOC_PORT_PHY_TIMESYNC_DPLL_K3_ALWAYS_LOAD) {
                temp2 |= 1U << 2;
            }
            if (value0 &  SOC_PORT_PHY_TIMESYNC_DPLL_K2_LOAD) {
                temp1 |= 1U << 1;
            }
            if (value0 &  SOC_PORT_PHY_TIMESYNC_DPLL_K2_ALWAYS_LOAD) {
                temp2 |= 1U << 1;
            }
            if (value0 &  SOC_PORT_PHY_TIMESYNC_DPLL_K1_LOAD) {
                temp1 |= 1U << 0;
            }
            if (value0 &  SOC_PORT_PHY_TIMESYNC_DPLL_K1_ALWAYS_LOAD) {
                temp2 |= 1U << 0;
            }
            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_write(unit, pc, 0, 0x2e, temp1), rv);
            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_write(unit, pc, 0, 0x2f, temp2), rv);
            break;

        case SOC_PORT_CONTROL_PHY_TIMESYNC_INTERRUPT_MASK:
            temp1 = 0;

            value0 = COMPILER_64_LO(value);

            if (value0 &  SOC_PORT_PHY_TIMESYNC_TIMESTAMP_INTERRUPT_MASK) {
                temp1 |= 1U << 1;
            }
            if (value0 &  SOC_PORT_PHY_TIMESYNC_FRAMESYNC_INTERRUPT_MASK) {
                temp1 |= 1U << 0;
            }
            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_write(unit, pc, 0, 0x30, temp1), rv);
            break;

        case SOC_PORT_CONTROL_PHY_TIMESYNC_TX_TIMESTAMP_OFFSET:
            value0 = COMPILER_64_LO(value);

            /* tx_timestamp_offset */
            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_write(unit, pc, 0, 0x15 + offset, (uint16)(value0 & 0xffff)), rv);

            if (offset < 4 ) {
                SOC_IF_ERROR_BREAK
                    (_phy_egphy16_blk_top_lvl_reg_modify(unit, pc, 0, 0x63,
                                                         ((value0 & 0xf0000) >> ((4 - offset) << 2)), 0xf << (offset << 2)), rv);
            } else {
                uint16 offset1 = offset - 4;
                SOC_IF_ERROR_BREAK
                    (_phy_egphy16_blk_top_lvl_reg_modify(unit, pc, 0, 0x64,
                                                         ((value0 & 0xf0000) >> ((4 - offset1) << 2)), 0xf << (offset1 << 2)), rv);
            }
            break;

        case SOC_PORT_CONTROL_PHY_TIMESYNC_RX_TIMESTAMP_OFFSET:
            value0 = COMPILER_64_LO(value);

            /* rx_timestamp_offset */
            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_write(unit, pc, 0, 0x1d + offset, (uint16)(value0 & 0xffff)), rv);

            if (offset < 4 ) {
                SOC_IF_ERROR_BREAK
                    (_phy_egphy16_blk_top_lvl_reg_modify(unit, pc, 0, 0x65,
                                                         ((value0 & 0xf0000) >> ((4 - offset) << 2)), 0xf << (offset << 2)), rv);
            } else {
                uint16 offset1 = offset - 4;
                SOC_IF_ERROR_BREAK
                    (_phy_egphy16_blk_top_lvl_reg_modify(unit, pc, 0, 0x66,
                                                         ((value0 & 0xf0000) >> ((4 - offset1) << 2)), 0xf << (offset1 << 2)), rv);
            }
            break;


        default:
            pc->phy_id =  PHYEGPHY16_DEV_PHY_ID_ORIG(pc);
            return SOC_E_FAIL;
        }
    } while(0);

    pc->phy_id =  PHYEGPHY16_DEV_PHY_ID_ORIG(pc);

    return SOC_E_NONE;
}

STATIC int
phy_egphy16_timesync_control_get(int unit, soc_port_t port, soc_port_control_phy_timesync_t type, uint64 *value)
{
    phy_ctrl_t *pc;
    uint16 value0;
    uint16 value1;
    uint16 value2;
    uint32 temp32;
    int offset, rv = SOC_E_NONE;

    pc = EXT_PHY_SW_STATE(unit, port);

    offset = PHYEGPHY16_DEV_PHY_SLICE(pc);

    do {
        /* pc->phy_id =  PHYEGPHY16_SLICE_ADDR(pc, 0); */
        pc->phy_id =  PHYEGPHY16_DEV_PHY_ID_BASE(pc);

        switch (type) {
        case SOC_PORT_CONTROL_PHY_TIMESYNC_HEARTBEAT_TIMESTAMP:
            SOC_IF_ERROR_BREAK
                (WRITE_PHYEGPHY16_EXP_READ_START_END_CTRLr(unit, pc, 0x1), rv);

            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_read(unit, pc, 0, 0x56, &value0), rv);
            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_read(unit, pc, 0, 0x57, &value1), rv);
            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_read(unit, pc, 0, 0x58, &value2), rv);

            SOC_IF_ERROR_BREAK
                (WRITE_PHYEGPHY16_EXP_READ_START_END_CTRLr(unit, pc, 0x2), rv);
            SOC_IF_ERROR_BREAK
                (WRITE_PHYEGPHY16_EXP_READ_START_END_CTRLr(unit, pc, 0x0), rv);

            COMPILER_64_SET((*value), ((uint32)value2),  (((uint32)value1<<16)|((uint32)value0)));

            /*    *value =  (((uint64)value2) << 32) | (((uint64)value1) << 16) | ((uint64)value0); */
            break;

        case SOC_PORT_CONTROL_PHY_TIMESYNC_CAPTURE_TIMESTAMP:
            SOC_IF_ERROR_BREAK
                (WRITE_PHYEGPHY16_EXP_READ_START_END_CTRLr(unit, pc, 0x4), rv);

            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_read(unit, pc, 0, 0x53, &value0), rv);
            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_read(unit, pc, 0, 0x54, &value1), rv);
            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_read(unit, pc, 0, 0x55, &value2), rv);

            SOC_IF_ERROR_BREAK
                (WRITE_PHYEGPHY16_EXP_READ_START_END_CTRLr(unit, pc, 0x8), rv);
            SOC_IF_ERROR_BREAK
                (WRITE_PHYEGPHY16_EXP_READ_START_END_CTRLr(unit, pc, 0x0), rv);

            COMPILER_64_SET((*value), ((uint32)value2),  (((uint32)value1<<16)|((uint32)value0)));

            /*   *value = (((uint64)value2) << 32) | (((uint64)value1) << 16) | ((uint64)value0); */
            break;

        case SOC_PORT_CONTROL_PHY_TIMESYNC_NCOADDEND:
            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_read(unit, pc, 0, 0x44, &value0), rv);
            /* *value = value0; */
            COMPILER_64_SET((*value), 0,  (uint32)value0);
            break;

        case SOC_PORT_CONTROL_PHY_TIMESYNC_LOCAL_TIME:
            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_read(unit, pc, 0, 0x45, &value0), rv);
            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_read(unit, pc, 0, 0x46, &value1), rv);
            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_read(unit, pc, 0, 0x47, &value2), rv);

            COMPILER_64_SET((*value), (value2 & 0x0fff), ((uint32)value1 << 16) | value0 );
            COMPILER_64_SHL((*value), 4);
            break;


        case SOC_PORT_CONTROL_PHY_TIMESYNC_LOAD_CONTROL:
            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_read(unit, pc, 0, 0x2e, &value1), rv);
            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_read(unit, pc, 0, 0x2f, &value2), rv);
            value0 = 0;
            if (value1 & (1U << 11)) {
                value0 |= (uint16) SOC_PORT_PHY_TIMESYNC_TN_LOAD;
            }
            if (value2 & (1U << 11)) {
                value0 |= (uint16) SOC_PORT_PHY_TIMESYNC_TN_ALWAYS_LOAD;
            }
            if (value1 & (1U << 10)) {
                value0 |= (uint16) SOC_PORT_PHY_TIMESYNC_TIMECODE_LOAD;
            }
            if (value2 & (1U << 10)) {
                value0 |= (uint16) SOC_PORT_PHY_TIMESYNC_TIMECODE_ALWAYS_LOAD;
            }
            if (value1 & (1U << 9)) {
                value0 |= (uint16) SOC_PORT_PHY_TIMESYNC_SYNCOUT_LOAD;
            }
            if (value2 & (1U << 9)) {
                value0 |= (uint16) SOC_PORT_PHY_TIMESYNC_SYNCOUT_ALWAYS_LOAD;
            }
            if (value1 & (1U << 8)) {
                value0 |= (uint16) SOC_PORT_PHY_TIMESYNC_NCO_DIVIDER_LOAD;
            }
            if (value1 & (1U << 8)) {
                value0 |= (uint16) SOC_PORT_PHY_TIMESYNC_NCO_DIVIDER_ALWAYS_LOAD;
            }
            if (value1 & (1U << 7)) {
                value0 |= (uint16) SOC_PORT_PHY_TIMESYNC_LOCAL_TIME_LOAD;
            }
            if (value1 & (1U << 7)) {
                value0 |= (uint16) SOC_PORT_PHY_TIMESYNC_LOCAL_TIME_ALWAYS_LOAD;
            }
            if (value1 & (1U << 6)) {
                value0 |= (uint16) SOC_PORT_PHY_TIMESYNC_NCO_ADDEND_LOAD;
            }
            if (value2 & (1U << 6)) {
                value0 |= (uint16) SOC_PORT_PHY_TIMESYNC_NCO_ADDEND_ALWAYS_LOAD;
            }
            if (value1 & (1U << 5)) {
                value0 |= (uint16) SOC_PORT_PHY_TIMESYNC_DPLL_LOOP_FILTER_LOAD;
            }
            if (value2 & (1U << 5)) {
                value0 |= (uint16) SOC_PORT_PHY_TIMESYNC_DPLL_LOOP_FILTER_ALWAYS_LOAD;
            }
            if (value1 & (1U << 4)) {
                value0 |= (uint16) SOC_PORT_PHY_TIMESYNC_DPLL_REF_PHASE_LOAD;
            }
            if (value2 & (1U << 4)) {
                value0 |= (uint16) SOC_PORT_PHY_TIMESYNC_DPLL_REF_PHASE_ALWAYS_LOAD;
            }
            if (value1 & (1U << 3)) {
                value0 |= (uint16) SOC_PORT_PHY_TIMESYNC_DPLL_REF_PHASE_DELTA_LOAD;
            }
            if (value2 & (1U << 3)) {
                value0 |= (uint16) SOC_PORT_PHY_TIMESYNC_DPLL_REF_PHASE_DELTA_ALWAYS_LOAD;
            }
            if (value1 & (1U << 2)) {
                value0 |= (uint16) SOC_PORT_PHY_TIMESYNC_DPLL_K3_LOAD;
            }
            if (value2 & (1U << 2)) {
                value0 |= (uint16) SOC_PORT_PHY_TIMESYNC_DPLL_K3_ALWAYS_LOAD;
            }
            if (value1 & (1U << 1)) {
                value0 |= (uint16) SOC_PORT_PHY_TIMESYNC_DPLL_K2_LOAD;
            }
            if (value2 & (1U << 1)) {
                value0 |= (uint16) SOC_PORT_PHY_TIMESYNC_DPLL_K2_ALWAYS_LOAD;
            }
            if (value1 & (1U << 0)) {
                value0 |= (uint16) SOC_PORT_PHY_TIMESYNC_DPLL_K1_LOAD;
            }
            if (value2 & (1U << 0)) {
                value0 |= (uint16) SOC_PORT_PHY_TIMESYNC_DPLL_K1_ALWAYS_LOAD;
            }
            COMPILER_64_SET((*value), 0, (uint32)value0);
            break;

        case SOC_PORT_CONTROL_PHY_TIMESYNC_INTERRUPT:
            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_read(unit, pc, 0, 0x31, &value1), rv);
            value0 = 0;
            if (value1 & (1U << 1)) {
                value0 |= SOC_PORT_PHY_TIMESYNC_TIMESTAMP_INTERRUPT;
            }
            if (value1 & (1U << 0)) {
                value0 |= SOC_PORT_PHY_TIMESYNC_FRAMESYNC_INTERRUPT;
            }
            COMPILER_64_SET((*value), 0, (uint32)value0);
            break;

        case SOC_PORT_CONTROL_PHY_TIMESYNC_INTERRUPT_MASK:
            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_read(unit, pc, 0, 0x30, &value1), rv);
            value0 = 0;
            if (value1 & (1U << 1)) {
                value0 |= SOC_PORT_PHY_TIMESYNC_TIMESTAMP_INTERRUPT_MASK;
            }
            if (value1 & (1U << 0)) {
                value0 |= SOC_PORT_PHY_TIMESYNC_FRAMESYNC_INTERRUPT_MASK;
            }
            COMPILER_64_SET((*value), 0, (uint32)value0);
            break;


        case SOC_PORT_CONTROL_PHY_TIMESYNC_TX_TIMESTAMP_OFFSET:
            /* tx_timestamp_offset */
            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_read(unit, pc, 0, 0x15 + offset, &value1), rv);
            if (offset < 4 ) {
                SOC_IF_ERROR_BREAK
                    (_phy_egphy16_blk_top_lvl_reg_read(unit, pc, 0, 0x63, &value2), rv);
                temp32 = (value2 & (0xf << (offset << 2))) << ((4 - offset) << 2);
            } else {
                uint16 offset1 = offset - 4;
                SOC_IF_ERROR_BREAK
                    (_phy_egphy16_blk_top_lvl_reg_read(unit, pc, 0, 0x64, &value2), rv);
                temp32 = (value2 & (0xf << (offset1 << 2))) << ((4 - offset1) << 2);
            }
            /* *value = temp32 | value1; */
            COMPILER_64_SET((*value), 0, (uint32)(temp32 | value1));
            break;

        case SOC_PORT_CONTROL_PHY_TIMESYNC_RX_TIMESTAMP_OFFSET:
            /* rx_timestamp_offset */
            SOC_IF_ERROR_BREAK
                (_phy_egphy16_blk_top_lvl_reg_read(unit, pc, 0, 0x1d + offset, &value1), rv);
            if (offset < 4 ) {
                SOC_IF_ERROR_BREAK
                    (_phy_egphy16_blk_top_lvl_reg_read(unit, pc, 0, 0x65, &value2), rv);
                temp32 = (value2 & (0xf << (offset << 2))) << ((4 - offset) << 2);
            } else {
                uint16 offset1 = offset - 4;
                SOC_IF_ERROR_BREAK
                    (_phy_egphy16_blk_top_lvl_reg_read(unit, pc, 0, 0x66, &value2), rv);
                temp32 = (value2 & (0xf << (offset1 << 2))) << ((4 - offset1) << 2);
            }
            /* *value = temp32 | value1; */
            COMPILER_64_SET((*value), 0, (uint32)(temp32 | value1));
            break;

        default:
            pc->phy_id =  PHYEGPHY16_DEV_PHY_ID_BASE(pc);
            return SOC_E_FAIL;
        }
    } while(0);

    pc->phy_id =  PHYEGPHY16_DEV_PHY_ID_BASE(pc);

    return SOC_E_NONE;
}

/*
 * Variable:    phy_54680drv_ge
 * Purpose:     PHY driver for 54680
 */

phy_driver_t phy_egphy16drv_ge = {
    "egphy16 Gigabit PHY Driver",
    phy_egphy16_init,
    phy_fe_ge_reset,
    phy_egphy16_link_get,
    phy_egphy16_enable_set,
    phy_egphy16_enable_get,
    phy_egphy16_duplex_set,
    phy_egphy16_duplex_get,
    phy_egphy16_speed_set,
    phy_egphy16_speed_get,
    phy_egphy16_master_set,
    phy_egphy16_master_get,
    phy_egphy16_autoneg_set,
    phy_egphy16_autoneg_get,
    NULL,
    NULL,
    NULL,
    phy_egphy16_lb_set,
    phy_egphy16_lb_get,
    phy_egphy16_interface_set,
    phy_egphy16_interface_get,
    NULL,                       /* Deprecated */
    phy_egphy16_link_up,          /* Link up event */
    phy_egphy16_link_down,
    phy_egphy16_mdix_set,
    phy_egphy16_mdix_get,
    phy_egphy16_mdix_status_get,
    phy_egphy16_medium_config_set,
    phy_egphy16_medium_config_get,
    phy_egphy16_medium_status,
    phy_egphy16_cable_diag_dispatch,
    NULL,                        /* phy_link_change */
    phy_egphy16_control_set,       /* phy_control_set */
    phy_egphy16_control_get,       /* phy_control_get */
    phy_ge_reg_read,
    phy_ge_reg_write,
    phy_ge_reg_modify,
    NULL,                        /* Phy notify event */
    phy_egphy16_probe,             /* pd_probe  */
    phy_egphy16_ability_advert_set,/* pd_ability_advert_set */
    phy_egphy16_ability_advert_get,/* pd_ability_advert_get */
    phy_egphy16_ability_remote_get,/* pd_ability_remote_get */
    phy_egphy16_ability_local_get, /* pd_ability_local_get  */
    NULL,                        /* pd_firmware_set */
    phy_egphy16_timesync_config_set,
    phy_egphy16_timesync_config_get,
    phy_egphy16_timesync_control_set,
    phy_egphy16_timesync_control_get
};

#else /* INCLUDE_PHY_EGPHY16 */
typedef int _phy_egphy16_not_empty; /* Make ISO compilers happy. */
#endif /* INCLUDE_PHY_EGPHY16 */

