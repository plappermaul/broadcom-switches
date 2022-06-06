/*******************************************************************************
 *
 * This software is governed by the Broadcom Switch APIs license.
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenMDK/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 *
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated from the registers file.
 * Edits to this file will be lost when it is regenerated.
 *
 * Symbol table file for the BCM5464.
 * This symbol table is used by the Broadcom debug shell.
 */


#include <phy/chip/bcm5464_defs.h>

/* No symbols will be compiled unless this is defined. */
#if PHY_CONFIG_INCLUDE_CHIP_SYMBOLS == 1
/*******************************************************************************
 *
 * If CDK_CONFIG_INCLUDE_FIELD_INFO is 1, then symbol information
 * necessary to encode and decode the individual fields of a register or memory
 * will be available.
 *
 * Without it, only the register and memory names will be symbolically available
 * and their values will be displayed as raw data only. 
 *
 * Field information can be compiled out in the interest of saving code space.
 */
#if CDK_CONFIG_INCLUDE_FIELD_INFO == 1
#ifndef CDK_CONFIG_EXCLUDE_FIELD_INFO_BCM5464

static uint32_t BCM5464_AUTO_DETECT_MEDIUMr_fields[] =
{
    /* AUTO_DETECT_MEDIUM:0:15 */
    CDK_SYMBOL_FIELD_FLAG_LAST | CDK_SYMBOL_FIELD_ENCODE(5, 15, 0)
};
static uint32_t BCM5464_AUTO_DETECT_SGMII_MEDIAr_fields[] =
{
    /* AUTO_DETECT_SGMII_MEDIA:0:15 */
    CDK_SYMBOL_FIELD_FLAG_LAST | CDK_SYMBOL_FIELD_ENCODE(6, 15, 0)
};
static uint32_t BCM5464_AUTO_POWER_DOWNr_fields[] =
{
    /* AUTO_POWER_DOWN:0:15 */
    CDK_SYMBOL_FIELD_FLAG_LAST | CDK_SYMBOL_FIELD_ENCODE(7, 15, 0)
};
static uint32_t BCM5464_AUX_1000X_CTRLr_fields[] =
{
    /* AUX_1000X_CTRL:0:15 */
    CDK_SYMBOL_FIELD_FLAG_LAST | CDK_SYMBOL_FIELD_ENCODE(8, 15, 0)
};
static uint32_t BCM5464_AUX_1000X_STATr_fields[] =
{
    /* AUX_1000X_STAT:0:15 */
    CDK_SYMBOL_FIELD_FLAG_LAST | CDK_SYMBOL_FIELD_ENCODE(9, 15, 0)
};
static uint32_t BCM5464_CLK_ALIGN_CTRLr_fields[] =
{
    /* CLK_ALIGN_CTRL:0:15 */
    CDK_SYMBOL_FIELD_FLAG_LAST | CDK_SYMBOL_FIELD_ENCODE(23, 15, 0)
};
static uint32_t BCM5464_EXP_INTERRUPT_MASKr_fields[] =
{
    /* EXP_INTERRUPT_MASK:0:15 */
    CDK_SYMBOL_FIELD_FLAG_LAST | CDK_SYMBOL_FIELD_ENCODE(25, 15, 0)
};
static uint32_t BCM5464_EXP_INTERRUPT_STATr_fields[] =
{
    /* EXP_INTERRUPT_STAT:0:15 */
    CDK_SYMBOL_FIELD_FLAG_LAST | CDK_SYMBOL_FIELD_ENCODE(26, 15, 0)
};
static uint32_t BCM5464_EXP_LED_BLINK_CTRLr_fields[] =
{
    /* EXP_LED_BLINK_CTRL:0:15 */
    CDK_SYMBOL_FIELD_FLAG_LAST | CDK_SYMBOL_FIELD_ENCODE(27, 15, 0)
};
static uint32_t BCM5464_EXP_LED_FLASH_CTRLr_fields[] =
{
    /* EXP_LED_FLASH_CTRL:0:15 */
    CDK_SYMBOL_FIELD_FLAG_LAST | CDK_SYMBOL_FIELD_ENCODE(28, 15, 0)
};
static uint32_t BCM5464_EXP_LED_SELECTORr_fields[] =
{
    /* EXP_LED_SELECTOR:0:15 */
    CDK_SYMBOL_FIELD_FLAG_LAST | CDK_SYMBOL_FIELD_ENCODE(29, 15, 0)
};
static uint32_t BCM5464_EXP_PASSTHRU_LED_MODEr_fields[] =
{
    /* EXP_PASSTHRU_LED_MODE:0:15 */
    CDK_SYMBOL_FIELD_FLAG_LAST | CDK_SYMBOL_FIELD_ENCODE(30, 15, 0)
};
static uint32_t BCM5464_EXP_PKT_COUNTERr_fields[] =
{
    /* EXP_PKT_COUNTER:0:15 */
    CDK_SYMBOL_FIELD_FLAG_LAST | CDK_SYMBOL_FIELD_ENCODE(31, 15, 0)
};
static uint32_t BCM5464_EXP_SERDES_PASSTHRU_ENr_fields[] =
{
    /* EXP_SERDES_PASSTHRU_EN:0:15 */
    CDK_SYMBOL_FIELD_FLAG_LAST | CDK_SYMBOL_FIELD_ENCODE(32, 15, 0)
};
static uint32_t BCM5464_LED_CTRLr_fields[] =
{
    /* LED_CTRL:0:15 */
    CDK_SYMBOL_FIELD_FLAG_LAST | CDK_SYMBOL_FIELD_ENCODE(39, 15, 0)
};
static uint32_t BCM5464_LED_GPIO_CTRLr_fields[] =
{
    /* LED_GPIO_CTRL:0:15 */
    CDK_SYMBOL_FIELD_FLAG_LAST | CDK_SYMBOL_FIELD_ENCODE(40, 15, 0)
};
static uint32_t BCM5464_LED_SELECTOR_1r_fields[] =
{
    /* LED_SELECTOR_1:0:15 */
    CDK_SYMBOL_FIELD_FLAG_LAST | CDK_SYMBOL_FIELD_ENCODE(41, 15, 0)
};
static uint32_t BCM5464_LED_SELECTOR_2r_fields[] =
{
    /* LED_SELECTOR_2:0:15 */
    CDK_SYMBOL_FIELD_FLAG_LAST | CDK_SYMBOL_FIELD_ENCODE(42, 15, 0)
};
static uint32_t BCM5464_LED_STATr_fields[] =
{
    /* LED_STAT:0:15 */
    CDK_SYMBOL_FIELD_FLAG_LAST | CDK_SYMBOL_FIELD_ENCODE(43, 15, 0)
};
static uint32_t BCM5464_MII_1000X_ANAr_fields[] =
{
    /* CAP_1000BASE_X_FD:5:5 */
    CDK_SYMBOL_FIELD_ENCODE(12, 5, 5),
    /* CAP_1000BASE_X_HD:6:6 */
    CDK_SYMBOL_FIELD_ENCODE(13, 6, 6),
    /* PAUSE:7:8 */
    CDK_SYMBOL_FIELD_ENCODE(70, 8, 7),
    /* REMOTE_FAULT:13:13 */
    CDK_SYMBOL_FIELD_FLAG_LAST | CDK_SYMBOL_FIELD_ENCODE(74, 13, 13)
};
static uint32_t BCM5464_MII_1000X_ANPr_fields[] =
{
    /* CAP_1000BASE_X_FD:5:5 */
    CDK_SYMBOL_FIELD_ENCODE(12, 5, 5),
    /* CAP_1000BASE_X_HD:6:6 */
    CDK_SYMBOL_FIELD_ENCODE(13, 6, 6),
    /* PAUSE:7:8 */
    CDK_SYMBOL_FIELD_ENCODE(70, 8, 7),
    /* REMOTE_FAULT:13:13 */
    CDK_SYMBOL_FIELD_ENCODE(74, 13, 13),
    /* ACK:14:14 */
    CDK_SYMBOL_FIELD_ENCODE(0, 14, 14),
    /* NEXT_PAGE:15:15 */
    CDK_SYMBOL_FIELD_FLAG_LAST | CDK_SYMBOL_FIELD_ENCODE(67, 15, 15)
};
static uint32_t BCM5464_MII_1000X_AN_DEBUGr_fields[] =
{
    /* MII_1000X_AN_DEBUG:0:15 */
    CDK_SYMBOL_FIELD_FLAG_LAST | CDK_SYMBOL_FIELD_ENCODE(54, 15, 0)
};
static uint32_t BCM5464_MII_1000X_AN_EXPr_fields[] =
{
    /* PAGE_RECEIVED:1:1 */
    CDK_SYMBOL_FIELD_ENCODE(68, 1, 1),
    /* NEXT_PAGE:2:2 */
    CDK_SYMBOL_FIELD_FLAG_LAST | CDK_SYMBOL_FIELD_ENCODE(67, 2, 2)
};
static uint32_t BCM5464_MII_1000X_CTRLr_fields[] =
{
    /* RESERVED:6:6 */
    CDK_SYMBOL_FIELD_ENCODE(76, 6, 6),
    /* COLL_TEST_EN:7:7 */
    CDK_SYMBOL_FIELD_ENCODE(24, 7, 7),
    /* FULL_DUPLEX:8:8 */
    CDK_SYMBOL_FIELD_ENCODE(35, 8, 8),
    /* RESTART_AN:9:9 */
    CDK_SYMBOL_FIELD_ENCODE(78, 9, 9),
    /* ISOLATE:10:10 */
    CDK_SYMBOL_FIELD_ENCODE(37, 10, 10),
    /* POWER_DOWN:11:11 */
    CDK_SYMBOL_FIELD_ENCODE(71, 11, 11),
    /* AUTONEG:12:12 */
    CDK_SYMBOL_FIELD_ENCODE(2, 12, 12),
    /* LOOPBACK:14:14 */
    CDK_SYMBOL_FIELD_ENCODE(47, 14, 14),
    /* RESET:15:15 */
    CDK_SYMBOL_FIELD_FLAG_LAST | CDK_SYMBOL_FIELD_ENCODE(77, 15, 15)
};
static uint32_t BCM5464_MII_1000X_EXT_STATr_fields[] =
{
    /* CAP_1000BASE_T_FD:5:13 */
    CDK_SYMBOL_FIELD_ENCODE(10, 13, 5),
    /* CAP_1000BASE_T_HD:12:12 */
    CDK_SYMBOL_FIELD_ENCODE(11, 12, 12),
    /* CAP_1000BASE_X_HD:14:14 */
    CDK_SYMBOL_FIELD_ENCODE(13, 14, 14),
    /* CAP_1000BASE_X_FD:15:15 */
    CDK_SYMBOL_FIELD_FLAG_LAST | CDK_SYMBOL_FIELD_ENCODE(12, 15, 15)
};
static uint32_t BCM5464_MII_1000X_STATr_fields[] =
{
    /* EXT_CAPABILITY:0:0 */
    CDK_SYMBOL_FIELD_ENCODE(33, 0, 0),
    /* JABBER_DETECT:1:1 */
    CDK_SYMBOL_FIELD_ENCODE(38, 1, 1),
    /* LINK:2:2 */
    CDK_SYMBOL_FIELD_ENCODE(44, 2, 2),
    /* AUTONEG_ABILITY:3:3 */
    CDK_SYMBOL_FIELD_ENCODE(3, 3, 3),
    /* REMOTE_FAULT:4:4 */
    CDK_SYMBOL_FIELD_ENCODE(74, 4, 4),
    /* AUTONEG_DONE:5:5 */
    CDK_SYMBOL_FIELD_ENCODE(4, 5, 5),
    /* MF_PREAMBLE_SUP:6:6 */
    CDK_SYMBOL_FIELD_ENCODE(53, 6, 6),
    /* EXT_STATUS:8:8 */
    CDK_SYMBOL_FIELD_ENCODE(34, 8, 8),
    /* CAP_100BASE_T2_HD:9:9 */
    CDK_SYMBOL_FIELD_ENCODE(15, 9, 9),
    /* CAP_100BASE_T2_FD:10:10 */
    CDK_SYMBOL_FIELD_ENCODE(14, 10, 10),
    /* CAP_10BASE_T_HD:11:11 */
    CDK_SYMBOL_FIELD_ENCODE(22, 11, 11),
    /* CAP_10BASE_T_FD:12:12 */
    CDK_SYMBOL_FIELD_ENCODE(21, 12, 12),
    /* CAP_100BASE_X_HD:13:13 */
    CDK_SYMBOL_FIELD_ENCODE(20, 13, 13),
    /* CAP_100BASE_X_FD:14:14 */
    CDK_SYMBOL_FIELD_ENCODE(19, 14, 14),
    /* CAP_100BASE_T4:15:15 */
    CDK_SYMBOL_FIELD_FLAG_LAST | CDK_SYMBOL_FIELD_ENCODE(16, 15, 15)
};
static uint32_t BCM5464_MII_10BASE_Tr_fields[] =
{
    /* MII_10BASE_T:0:15 */
    CDK_SYMBOL_FIELD_FLAG_LAST | CDK_SYMBOL_FIELD_ENCODE(55, 15, 0)
};
static uint32_t BCM5464_MII_ANAr_fields[] =
{
    /* PROTOCOL:0:4 */
    CDK_SYMBOL_FIELD_ENCODE(72, 4, 0),
    /* CAP_10BASE_T_HD:5:5 */
    CDK_SYMBOL_FIELD_ENCODE(22, 5, 5),
    /* CAP_10BASE_T_FD:6:6 */
    CDK_SYMBOL_FIELD_ENCODE(21, 6, 6),
    /* CAP_100BASE_TX_HD:7:7 */
    CDK_SYMBOL_FIELD_ENCODE(18, 7, 7),
    /* CAP_100BASE_TX_FD:8:8 */
    CDK_SYMBOL_FIELD_ENCODE(17, 8, 8),
    /* CAP_100BASE_T4:9:9 */
    CDK_SYMBOL_FIELD_ENCODE(16, 9, 9),
    /* PAUSE:10:10 */
    CDK_SYMBOL_FIELD_ENCODE(70, 10, 10),
    /* ASYM_PAUSE:11:11 */
    CDK_SYMBOL_FIELD_ENCODE(1, 11, 11),
    /* REMOTE_FAULT:13:13 */
    CDK_SYMBOL_FIELD_ENCODE(74, 13, 13),
    /* NEXT_PAGE:15:15 */
    CDK_SYMBOL_FIELD_FLAG_LAST | CDK_SYMBOL_FIELD_ENCODE(67, 15, 15)
};
static uint32_t BCM5464_MII_ANPr_fields[] =
{
    /* PROTOCOL:0:4 */
    CDK_SYMBOL_FIELD_ENCODE(72, 4, 0),
    /* CAP_10BASE_T_HD:5:5 */
    CDK_SYMBOL_FIELD_ENCODE(22, 5, 5),
    /* CAP_10BASE_T_FD:6:6 */
    CDK_SYMBOL_FIELD_ENCODE(21, 6, 6),
    /* CAP_100BASE_TX_HD:7:7 */
    CDK_SYMBOL_FIELD_ENCODE(18, 7, 7),
    /* CAP_100BASE_TX_FD:8:8 */
    CDK_SYMBOL_FIELD_ENCODE(17, 8, 8),
    /* CAP_100BASE_T4:9:9 */
    CDK_SYMBOL_FIELD_ENCODE(16, 9, 9),
    /* PAUSE:10:10 */
    CDK_SYMBOL_FIELD_ENCODE(70, 10, 10),
    /* ASYM_PAUSE:11:11 */
    CDK_SYMBOL_FIELD_ENCODE(1, 11, 11),
    /* REMOTE_FAULT:13:13 */
    CDK_SYMBOL_FIELD_ENCODE(74, 13, 13),
    /* ACK:14:14 */
    CDK_SYMBOL_FIELD_ENCODE(0, 14, 14),
    /* NEXT_PAGE:15:15 */
    CDK_SYMBOL_FIELD_FLAG_LAST | CDK_SYMBOL_FIELD_ENCODE(67, 15, 15)
};
static uint32_t BCM5464_MII_AN_EXPr_fields[] =
{
    /* LP_AN_ABIL:0:0 */
    CDK_SYMBOL_FIELD_ENCODE(50, 0, 0),
    /* PAGE_RECEIVED:1:1 */
    CDK_SYMBOL_FIELD_ENCODE(68, 1, 1),
    /* NEXT_PAGE:2:2 */
    CDK_SYMBOL_FIELD_ENCODE(67, 2, 2),
    /* LP_NEXT_PAGE:3:3 */
    CDK_SYMBOL_FIELD_ENCODE(51, 3, 3),
    /* PARDET_FAULT:4:4 */
    CDK_SYMBOL_FIELD_FLAG_LAST | CDK_SYMBOL_FIELD_ENCODE(69, 4, 4)
};
static uint32_t BCM5464_MII_AUX_CTRLr_fields[] =
{
    /* MII_AUX_CTRL:0:15 */
    CDK_SYMBOL_FIELD_FLAG_LAST | CDK_SYMBOL_FIELD_ENCODE(56, 15, 0)
};
static uint32_t BCM5464_MII_CTRLr_fields[] =
{
    /* SPEED_SEL1:6:6 */
    CDK_SYMBOL_FIELD_ENCODE(83, 6, 6),
    /* COLL_TEST_EN:7:7 */
    CDK_SYMBOL_FIELD_ENCODE(24, 7, 7),
    /* FULL_DUPLEX:8:8 */
    CDK_SYMBOL_FIELD_ENCODE(35, 8, 8),
    /* RESTART_AN:9:9 */
    CDK_SYMBOL_FIELD_ENCODE(78, 9, 9),
    /* ISOLATE:10:10 */
    CDK_SYMBOL_FIELD_ENCODE(37, 10, 10),
    /* POWER_DOWN:11:11 */
    CDK_SYMBOL_FIELD_ENCODE(71, 11, 11),
    /* AUTONEG:12:12 */
    CDK_SYMBOL_FIELD_ENCODE(2, 12, 12),
    /* SPEED_SEL0:13:13 */
    CDK_SYMBOL_FIELD_ENCODE(82, 13, 13),
    /* LOOPBACK:14:14 */
    CDK_SYMBOL_FIELD_ENCODE(47, 14, 14),
    /* RESET:15:15 */
    CDK_SYMBOL_FIELD_FLAG_LAST | CDK_SYMBOL_FIELD_ENCODE(77, 15, 15)
};
static uint32_t BCM5464_MII_ESRr_fields[] =
{
    /* CAP_1000BASE_T_HD:12:12 */
    CDK_SYMBOL_FIELD_ENCODE(11, 12, 12),
    /* CAP_1000BASE_T_FD:13:13 */
    CDK_SYMBOL_FIELD_ENCODE(10, 13, 13),
    /* CAP_1000BASE_X_HD:14:14 */
    CDK_SYMBOL_FIELD_ENCODE(13, 14, 14),
    /* CAP_1000BASE_X_FD:15:15 */
    CDK_SYMBOL_FIELD_FLAG_LAST | CDK_SYMBOL_FIELD_ENCODE(12, 15, 15)
};
static uint32_t BCM5464_MII_GB_CTRLr_fields[] =
{
    /* CAP_1000BASE_T_HD:8:8 */
    CDK_SYMBOL_FIELD_ENCODE(11, 8, 8),
    /* CAP_1000BASE_T_FD:9:9 */
    CDK_SYMBOL_FIELD_ENCODE(10, 9, 9),
    /* SWITCH_DEV:10:10 */
    CDK_SYMBOL_FIELD_ENCODE(84, 10, 10),
    /* MASTER:11:11 */
    CDK_SYMBOL_FIELD_ENCODE(52, 11, 11),
    /* MS_MAN:12:12 */
    CDK_SYMBOL_FIELD_ENCODE(66, 12, 12),
    /* TEST_MODE:13:15 */
    CDK_SYMBOL_FIELD_FLAG_LAST | CDK_SYMBOL_FIELD_ENCODE(86, 15, 13)
};
static uint32_t BCM5464_MII_GB_STATr_fields[] =
{
    /* IDLE_ERRORS:0:7 */
    CDK_SYMBOL_FIELD_ENCODE(36, 7, 0),
    /* LP_1000BASE_T_HD:10:10 */
    CDK_SYMBOL_FIELD_ENCODE(49, 10, 10),
    /* LP_1000BASE_T_FD:11:11 */
    CDK_SYMBOL_FIELD_ENCODE(48, 11, 11),
    /* REMOTE_RCV_OK:12:12 */
    CDK_SYMBOL_FIELD_ENCODE(75, 12, 12),
    /* LOCAL_RCV_OK:13:13 */
    CDK_SYMBOL_FIELD_ENCODE(46, 13, 13),
    /* LOCAL_MASTER:14:14 */
    CDK_SYMBOL_FIELD_ENCODE(45, 14, 14),
    /* MS_CFG_FAULT:15:15 */
    CDK_SYMBOL_FIELD_FLAG_LAST | CDK_SYMBOL_FIELD_ENCODE(65, 15, 15)
};
static uint32_t BCM5464_MII_MISC_CTRLr_fields[] =
{
    /* MII_MISC_CTRL:0:15 */
    CDK_SYMBOL_FIELD_FLAG_LAST | CDK_SYMBOL_FIELD_ENCODE(57, 15, 0)
};
static uint32_t BCM5464_MII_MISC_TESTr_fields[] =
{
    /* MII_MISC_TEST:0:15 */
    CDK_SYMBOL_FIELD_FLAG_LAST | CDK_SYMBOL_FIELD_ENCODE(58, 15, 0)
};
static uint32_t BCM5464_MII_PHY_ECRr_fields[] =
{
    /* MII_PHY_ECR:0:15 */
    CDK_SYMBOL_FIELD_FLAG_LAST | CDK_SYMBOL_FIELD_ENCODE(59, 15, 0)
};
static uint32_t BCM5464_MII_PHY_ESRr_fields[] =
{
    /* MII_PHY_ESR:0:15 */
    CDK_SYMBOL_FIELD_FLAG_LAST | CDK_SYMBOL_FIELD_ENCODE(60, 15, 0)
};
static uint32_t BCM5464_MII_PHY_ID0r_fields[] =
{
    /* REGID:0:15 */
    CDK_SYMBOL_FIELD_FLAG_LAST | CDK_SYMBOL_FIELD_ENCODE(73, 15, 0)
};
static uint32_t BCM5464_MII_PHY_ID1r_fields[] =
{
    /* REGID:0:15 */
    CDK_SYMBOL_FIELD_FLAG_LAST | CDK_SYMBOL_FIELD_ENCODE(73, 15, 0)
};
static uint32_t BCM5464_MII_POWER_CTRLr_fields[] =
{
    /* MII_POWER_CTRL:0:15 */
    CDK_SYMBOL_FIELD_FLAG_LAST | CDK_SYMBOL_FIELD_ENCODE(61, 15, 0)
};
static uint32_t BCM5464_MII_STATr_fields[] =
{
    /* EXT_CAPABILITY:0:0 */
    CDK_SYMBOL_FIELD_ENCODE(33, 0, 0),
    /* JABBER_DETECT:1:1 */
    CDK_SYMBOL_FIELD_ENCODE(38, 1, 1),
    /* LINK:2:2 */
    CDK_SYMBOL_FIELD_ENCODE(44, 2, 2),
    /* AUTONEG_ABILITY:3:3 */
    CDK_SYMBOL_FIELD_ENCODE(3, 3, 3),
    /* REMOTE_FAULT:4:4 */
    CDK_SYMBOL_FIELD_ENCODE(74, 4, 4),
    /* AUTONEG_DONE:5:5 */
    CDK_SYMBOL_FIELD_ENCODE(4, 5, 5),
    /* MF_PREAMBLE_SUP:6:6 */
    CDK_SYMBOL_FIELD_ENCODE(53, 6, 6),
    /* EXT_STATUS:8:8 */
    CDK_SYMBOL_FIELD_ENCODE(34, 8, 8),
    /* CAP_100BASE_T2_HD:9:9 */
    CDK_SYMBOL_FIELD_ENCODE(15, 9, 9),
    /* CAP_100BASE_T2_FD:10:10 */
    CDK_SYMBOL_FIELD_ENCODE(14, 10, 10),
    /* CAP_10BASE_T_HD:11:11 */
    CDK_SYMBOL_FIELD_ENCODE(22, 11, 11),
    /* CAP_10BASE_T_FD:12:12 */
    CDK_SYMBOL_FIELD_ENCODE(21, 12, 12),
    /* CAP_100BASE_X_HD:13:13 */
    CDK_SYMBOL_FIELD_ENCODE(20, 13, 13),
    /* CAP_100BASE_X_FD:14:14 */
    CDK_SYMBOL_FIELD_ENCODE(19, 14, 14),
    /* CAP_100BASE_T4:15:15 */
    CDK_SYMBOL_FIELD_FLAG_LAST | CDK_SYMBOL_FIELD_ENCODE(16, 15, 15)
};
static uint32_t BCM5464_MISC_1000X_CTRLr_fields[] =
{
    /* MISC_1000X_CTRL:0:15 */
    CDK_SYMBOL_FIELD_FLAG_LAST | CDK_SYMBOL_FIELD_ENCODE(62, 15, 0)
};
static uint32_t BCM5464_MISC_1000X_STATr_fields[] =
{
    /* MISC_1000X_STAT:0:15 */
    CDK_SYMBOL_FIELD_FLAG_LAST | CDK_SYMBOL_FIELD_ENCODE(63, 15, 0)
};
static uint32_t BCM5464_MODE_CTRLr_fields[] =
{
    /* MODE_CTRL:0:15 */
    CDK_SYMBOL_FIELD_FLAG_LAST | CDK_SYMBOL_FIELD_ENCODE(64, 15, 0)
};
static uint32_t BCM5464_SPARE_CTRLr_fields[] =
{
    /* SPARE_CTRL:0:15 */
    CDK_SYMBOL_FIELD_FLAG_LAST | CDK_SYMBOL_FIELD_ENCODE(79, 15, 0)
};
static uint32_t BCM5464_SPARE_CTRL_2r_fields[] =
{
    /* SPARE_CTRL_2:0:15 */
    CDK_SYMBOL_FIELD_FLAG_LAST | CDK_SYMBOL_FIELD_ENCODE(80, 15, 0)
};
static uint32_t BCM5464_SPARE_CTRL_3r_fields[] =
{
    /* SPARE_CTRL_3:0:15 */
    CDK_SYMBOL_FIELD_FLAG_LAST | CDK_SYMBOL_FIELD_ENCODE(81, 15, 0)
};
static uint32_t BCM5464_TEST1r_fields[] =
{
    /* TEST1:0:15 */
    CDK_SYMBOL_FIELD_FLAG_LAST | CDK_SYMBOL_FIELD_ENCODE(85, 15, 0)
};



/*******************************************************************************
 *
 * The following is the field name table.
 */
#if CDK_CONFIG_INCLUDE_FIELD_NAMES == 1
#ifndef CDK_CONFIG_EXCLUDE_FIELD_NAMES_BCM5464
const char* bcm5464_fields[] = 
{
    "ACK",
    "ASYM_PAUSE",
    "AUTONEG",
    "AUTONEG_ABILITY",
    "AUTONEG_DONE",
    "AUTO_DETECT_MEDIUM",
    "AUTO_DETECT_SGMII_MEDIA",
    "AUTO_POWER_DOWN",
    "AUX_1000X_CTRL",
    "AUX_1000X_STAT",
    "CAP_1000BASE_T_FD",
    "CAP_1000BASE_T_HD",
    "CAP_1000BASE_X_FD",
    "CAP_1000BASE_X_HD",
    "CAP_100BASE_T2_FD",
    "CAP_100BASE_T2_HD",
    "CAP_100BASE_T4",
    "CAP_100BASE_TX_FD",
    "CAP_100BASE_TX_HD",
    "CAP_100BASE_X_FD",
    "CAP_100BASE_X_HD",
    "CAP_10BASE_T_FD",
    "CAP_10BASE_T_HD",
    "CLK_ALIGN_CTRL",
    "COLL_TEST_EN",
    "EXP_INTERRUPT_MASK",
    "EXP_INTERRUPT_STAT",
    "EXP_LED_BLINK_CTRL",
    "EXP_LED_FLASH_CTRL",
    "EXP_LED_SELECTOR",
    "EXP_PASSTHRU_LED_MODE",
    "EXP_PKT_COUNTER",
    "EXP_SERDES_PASSTHRU_EN",
    "EXT_CAPABILITY",
    "EXT_STATUS",
    "FULL_DUPLEX",
    "IDLE_ERRORS",
    "ISOLATE",
    "JABBER_DETECT",
    "LED_CTRL",
    "LED_GPIO_CTRL",
    "LED_SELECTOR_1",
    "LED_SELECTOR_2",
    "LED_STAT",
    "LINK",
    "LOCAL_MASTER",
    "LOCAL_RCV_OK",
    "LOOPBACK",
    "LP_1000BASE_T_FD",
    "LP_1000BASE_T_HD",
    "LP_AN_ABIL",
    "LP_NEXT_PAGE",
    "MASTER",
    "MF_PREAMBLE_SUP",
    "MII_1000X_AN_DEBUG",
    "MII_10BASE_T",
    "MII_AUX_CTRL",
    "MII_MISC_CTRL",
    "MII_MISC_TEST",
    "MII_PHY_ECR",
    "MII_PHY_ESR",
    "MII_POWER_CTRL",
    "MISC_1000X_CTRL",
    "MISC_1000X_STAT",
    "MODE_CTRL",
    "MS_CFG_FAULT",
    "MS_MAN",
    "NEXT_PAGE",
    "PAGE_RECEIVED",
    "PARDET_FAULT",
    "PAUSE",
    "POWER_DOWN",
    "PROTOCOL",
    "REGID",
    "REMOTE_FAULT",
    "REMOTE_RCV_OK",
    "RESERVED",
    "RESET",
    "RESTART_AN",
    "SPARE_CTRL",
    "SPARE_CTRL_2",
    "SPARE_CTRL_3",
    "SPEED_SEL0",
    "SPEED_SEL1",
    "SWITCH_DEV",
    "TEST1",
    "TEST_MODE",
};

#endif
#endif
#endif
#endif /* CDK_CONFIG_INCLUDE_FIELD_INFO */



/*******************************************************************************
 *
 * The following is the symbol table itself. 
 * It defines the entries for all registers and memories.
 * It also incorporates the field information for each register and memory if
 * applicable.
 */
static const cdk_symbol_t bcm5464_syms[] = {
#ifndef CDK_CONFIG_EXCLUDE_CHIP_SYMBOLS_BCM5464
{
	BCM5464_MII_CTRLr,
#if  CDK_CONFIG_INCLUDE_FIELD_INFO == 1
#ifndef CDK_CONFIG_EXCLUDE_FIELD_INFO_BCM5464
	BCM5464_MII_CTRLr_fields,
#else
	0,
#endif
#endif
	 CDK_SYMBOL_INDEX_SIZE_ENCODE(1),
	CDK_SYMBOL_FLAG_REGISTER,
	"MII_CTRLr",
},
{
	BCM5464_MII_STATr,
#if  CDK_CONFIG_INCLUDE_FIELD_INFO == 1
#ifndef CDK_CONFIG_EXCLUDE_FIELD_INFO_BCM5464
	BCM5464_MII_STATr_fields,
#else
	0,
#endif
#endif
	 CDK_SYMBOL_INDEX_SIZE_ENCODE(1),
	CDK_SYMBOL_FLAG_REGISTER,
	"MII_STATr",
},
{
	BCM5464_MII_PHY_ID0r,
#if  CDK_CONFIG_INCLUDE_FIELD_INFO == 1
#ifndef CDK_CONFIG_EXCLUDE_FIELD_INFO_BCM5464
	BCM5464_MII_PHY_ID0r_fields,
#else
	0,
#endif
#endif
	 CDK_SYMBOL_INDEX_SIZE_ENCODE(1),
	CDK_SYMBOL_FLAG_REGISTER,
	"MII_PHY_ID0r",
},
{
	BCM5464_MII_PHY_ID1r,
#if  CDK_CONFIG_INCLUDE_FIELD_INFO == 1
#ifndef CDK_CONFIG_EXCLUDE_FIELD_INFO_BCM5464
	BCM5464_MII_PHY_ID1r_fields,
#else
	0,
#endif
#endif
	 CDK_SYMBOL_INDEX_SIZE_ENCODE(1),
	CDK_SYMBOL_FLAG_REGISTER,
	"MII_PHY_ID1r",
},
{
	BCM5464_MII_ANAr,
#if  CDK_CONFIG_INCLUDE_FIELD_INFO == 1
#ifndef CDK_CONFIG_EXCLUDE_FIELD_INFO_BCM5464
	BCM5464_MII_ANAr_fields,
#else
	0,
#endif
#endif
	 CDK_SYMBOL_INDEX_SIZE_ENCODE(1),
	CDK_SYMBOL_FLAG_REGISTER,
	"MII_ANAr",
},
{
	BCM5464_MII_ANPr,
#if  CDK_CONFIG_INCLUDE_FIELD_INFO == 1
#ifndef CDK_CONFIG_EXCLUDE_FIELD_INFO_BCM5464
	BCM5464_MII_ANPr_fields,
#else
	0,
#endif
#endif
	 CDK_SYMBOL_INDEX_SIZE_ENCODE(1),
	CDK_SYMBOL_FLAG_REGISTER,
	"MII_ANPr",
},
{
	BCM5464_MII_AN_EXPr,
#if  CDK_CONFIG_INCLUDE_FIELD_INFO == 1
#ifndef CDK_CONFIG_EXCLUDE_FIELD_INFO_BCM5464
	BCM5464_MII_AN_EXPr_fields,
#else
	0,
#endif
#endif
	 CDK_SYMBOL_INDEX_SIZE_ENCODE(1),
	CDK_SYMBOL_FLAG_REGISTER,
	"MII_AN_EXPr",
},
{
	BCM5464_MII_GB_CTRLr,
#if  CDK_CONFIG_INCLUDE_FIELD_INFO == 1
#ifndef CDK_CONFIG_EXCLUDE_FIELD_INFO_BCM5464
	BCM5464_MII_GB_CTRLr_fields,
#else
	0,
#endif
#endif
	 CDK_SYMBOL_INDEX_SIZE_ENCODE(1),
	CDK_SYMBOL_FLAG_REGISTER,
	"MII_GB_CTRLr",
},
{
	BCM5464_MII_GB_STATr,
#if  CDK_CONFIG_INCLUDE_FIELD_INFO == 1
#ifndef CDK_CONFIG_EXCLUDE_FIELD_INFO_BCM5464
	BCM5464_MII_GB_STATr_fields,
#else
	0,
#endif
#endif
	 CDK_SYMBOL_INDEX_SIZE_ENCODE(1),
	CDK_SYMBOL_FLAG_REGISTER,
	"MII_GB_STATr",
},
{
	BCM5464_MII_ESRr,
#if  CDK_CONFIG_INCLUDE_FIELD_INFO == 1
#ifndef CDK_CONFIG_EXCLUDE_FIELD_INFO_BCM5464
	BCM5464_MII_ESRr_fields,
#else
	0,
#endif
#endif
	 CDK_SYMBOL_INDEX_SIZE_ENCODE(1),
	CDK_SYMBOL_FLAG_REGISTER,
	"MII_ESRr",
},
{
	BCM5464_MII_PHY_ECRr,
#if  CDK_CONFIG_INCLUDE_FIELD_INFO == 1
#ifndef CDK_CONFIG_EXCLUDE_FIELD_INFO_BCM5464
	BCM5464_MII_PHY_ECRr_fields,
#else
	0,
#endif
#endif
	 CDK_SYMBOL_INDEX_SIZE_ENCODE(1),
	CDK_SYMBOL_FLAG_REGISTER,
	"MII_PHY_ECRr",
},
{
	BCM5464_MII_PHY_ESRr,
#if  CDK_CONFIG_INCLUDE_FIELD_INFO == 1
#ifndef CDK_CONFIG_EXCLUDE_FIELD_INFO_BCM5464
	BCM5464_MII_PHY_ESRr_fields,
#else
	0,
#endif
#endif
	 CDK_SYMBOL_INDEX_SIZE_ENCODE(1),
	CDK_SYMBOL_FLAG_REGISTER,
	"MII_PHY_ESRr",
},
{
	BCM5464_MII_AUX_CTRLr,
#if  CDK_CONFIG_INCLUDE_FIELD_INFO == 1
#ifndef CDK_CONFIG_EXCLUDE_FIELD_INFO_BCM5464
	BCM5464_MII_AUX_CTRLr_fields,
#else
	0,
#endif
#endif
	 CDK_SYMBOL_INDEX_SIZE_ENCODE(1),
	CDK_SYMBOL_FLAG_REGISTER,
	"MII_AUX_CTRLr",
},
{
	BCM5464_MII_10BASE_Tr,
#if  CDK_CONFIG_INCLUDE_FIELD_INFO == 1
#ifndef CDK_CONFIG_EXCLUDE_FIELD_INFO_BCM5464
	BCM5464_MII_10BASE_Tr_fields,
#else
	0,
#endif
#endif
	 CDK_SYMBOL_INDEX_SIZE_ENCODE(1),
	CDK_SYMBOL_FLAG_REGISTER,
	"MII_10BASE_Tr",
},
{
	BCM5464_MII_POWER_CTRLr,
#if  CDK_CONFIG_INCLUDE_FIELD_INFO == 1
#ifndef CDK_CONFIG_EXCLUDE_FIELD_INFO_BCM5464
	BCM5464_MII_POWER_CTRLr_fields,
#else
	0,
#endif
#endif
	 CDK_SYMBOL_INDEX_SIZE_ENCODE(1),
	CDK_SYMBOL_FLAG_REGISTER,
	"MII_POWER_CTRLr",
},
{
	BCM5464_MII_MISC_TESTr,
#if  CDK_CONFIG_INCLUDE_FIELD_INFO == 1
#ifndef CDK_CONFIG_EXCLUDE_FIELD_INFO_BCM5464
	BCM5464_MII_MISC_TESTr_fields,
#else
	0,
#endif
#endif
	 CDK_SYMBOL_INDEX_SIZE_ENCODE(1),
	CDK_SYMBOL_FLAG_REGISTER,
	"MII_MISC_TESTr",
},
{
	BCM5464_MII_MISC_CTRLr,
#if  CDK_CONFIG_INCLUDE_FIELD_INFO == 1
#ifndef CDK_CONFIG_EXCLUDE_FIELD_INFO_BCM5464
	BCM5464_MII_MISC_CTRLr_fields,
#else
	0,
#endif
#endif
	 CDK_SYMBOL_INDEX_SIZE_ENCODE(1),
	CDK_SYMBOL_FLAG_REGISTER,
	"MII_MISC_CTRLr",
},
{
	BCM5464_SPARE_CTRLr,
#if  CDK_CONFIG_INCLUDE_FIELD_INFO == 1
#ifndef CDK_CONFIG_EXCLUDE_FIELD_INFO_BCM5464
	BCM5464_SPARE_CTRLr_fields,
#else
	0,
#endif
#endif
	 CDK_SYMBOL_INDEX_SIZE_ENCODE(1),
	CDK_SYMBOL_FLAG_REGISTER,
	"SPARE_CTRLr",
},
{
	BCM5464_CLK_ALIGN_CTRLr,
#if  CDK_CONFIG_INCLUDE_FIELD_INFO == 1
#ifndef CDK_CONFIG_EXCLUDE_FIELD_INFO_BCM5464
	BCM5464_CLK_ALIGN_CTRLr_fields,
#else
	0,
#endif
#endif
	 CDK_SYMBOL_INDEX_SIZE_ENCODE(1),
	CDK_SYMBOL_FLAG_REGISTER,
	"CLK_ALIGN_CTRLr",
},
{
	BCM5464_SPARE_CTRL_2r,
#if  CDK_CONFIG_INCLUDE_FIELD_INFO == 1
#ifndef CDK_CONFIG_EXCLUDE_FIELD_INFO_BCM5464
	BCM5464_SPARE_CTRL_2r_fields,
#else
	0,
#endif
#endif
	 CDK_SYMBOL_INDEX_SIZE_ENCODE(1),
	CDK_SYMBOL_FLAG_REGISTER,
	"SPARE_CTRL_2r",
},
{
	BCM5464_SPARE_CTRL_3r,
#if  CDK_CONFIG_INCLUDE_FIELD_INFO == 1
#ifndef CDK_CONFIG_EXCLUDE_FIELD_INFO_BCM5464
	BCM5464_SPARE_CTRL_3r_fields,
#else
	0,
#endif
#endif
	 CDK_SYMBOL_INDEX_SIZE_ENCODE(1),
	CDK_SYMBOL_FLAG_REGISTER,
	"SPARE_CTRL_3r",
},
{
	BCM5464_LED_STATr,
#if  CDK_CONFIG_INCLUDE_FIELD_INFO == 1
#ifndef CDK_CONFIG_EXCLUDE_FIELD_INFO_BCM5464
	BCM5464_LED_STATr_fields,
#else
	0,
#endif
#endif
	 CDK_SYMBOL_INDEX_SIZE_ENCODE(1),
	CDK_SYMBOL_FLAG_REGISTER,
	"LED_STATr",
},
{
	BCM5464_LED_CTRLr,
#if  CDK_CONFIG_INCLUDE_FIELD_INFO == 1
#ifndef CDK_CONFIG_EXCLUDE_FIELD_INFO_BCM5464
	BCM5464_LED_CTRLr_fields,
#else
	0,
#endif
#endif
	 CDK_SYMBOL_INDEX_SIZE_ENCODE(1),
	CDK_SYMBOL_FLAG_REGISTER,
	"LED_CTRLr",
},
{
	BCM5464_AUTO_POWER_DOWNr,
#if  CDK_CONFIG_INCLUDE_FIELD_INFO == 1
#ifndef CDK_CONFIG_EXCLUDE_FIELD_INFO_BCM5464
	BCM5464_AUTO_POWER_DOWNr_fields,
#else
	0,
#endif
#endif
	 CDK_SYMBOL_INDEX_SIZE_ENCODE(1),
	CDK_SYMBOL_FLAG_REGISTER,
	"AUTO_POWER_DOWNr",
},
{
	BCM5464_LED_SELECTOR_1r,
#if  CDK_CONFIG_INCLUDE_FIELD_INFO == 1
#ifndef CDK_CONFIG_EXCLUDE_FIELD_INFO_BCM5464
	BCM5464_LED_SELECTOR_1r_fields,
#else
	0,
#endif
#endif
	 CDK_SYMBOL_INDEX_SIZE_ENCODE(1),
	CDK_SYMBOL_FLAG_REGISTER,
	"LED_SELECTOR_1r",
},
{
	BCM5464_LED_SELECTOR_2r,
#if  CDK_CONFIG_INCLUDE_FIELD_INFO == 1
#ifndef CDK_CONFIG_EXCLUDE_FIELD_INFO_BCM5464
	BCM5464_LED_SELECTOR_2r_fields,
#else
	0,
#endif
#endif
	 CDK_SYMBOL_INDEX_SIZE_ENCODE(1),
	CDK_SYMBOL_FLAG_REGISTER,
	"LED_SELECTOR_2r",
},
{
	BCM5464_LED_GPIO_CTRLr,
#if  CDK_CONFIG_INCLUDE_FIELD_INFO == 1
#ifndef CDK_CONFIG_EXCLUDE_FIELD_INFO_BCM5464
	BCM5464_LED_GPIO_CTRLr_fields,
#else
	0,
#endif
#endif
	 CDK_SYMBOL_INDEX_SIZE_ENCODE(1),
	CDK_SYMBOL_FLAG_REGISTER,
	"LED_GPIO_CTRLr",
},
{
	BCM5464_MISC_1000X_CTRLr,
#if  CDK_CONFIG_INCLUDE_FIELD_INFO == 1
#ifndef CDK_CONFIG_EXCLUDE_FIELD_INFO_BCM5464
	BCM5464_MISC_1000X_CTRLr_fields,
#else
	0,
#endif
#endif
	 CDK_SYMBOL_INDEX_SIZE_ENCODE(1),
	CDK_SYMBOL_FLAG_REGISTER,
	"MISC_1000X_CTRLr",
},
{
	BCM5464_AUTO_DETECT_SGMII_MEDIAr,
#if  CDK_CONFIG_INCLUDE_FIELD_INFO == 1
#ifndef CDK_CONFIG_EXCLUDE_FIELD_INFO_BCM5464
	BCM5464_AUTO_DETECT_SGMII_MEDIAr_fields,
#else
	0,
#endif
#endif
	 CDK_SYMBOL_INDEX_SIZE_ENCODE(1),
	CDK_SYMBOL_FLAG_REGISTER,
	"AUTO_DETECT_SGMII_MEDIAr",
},
{
	BCM5464_MII_1000X_AN_DEBUGr,
#if  CDK_CONFIG_INCLUDE_FIELD_INFO == 1
#ifndef CDK_CONFIG_EXCLUDE_FIELD_INFO_BCM5464
	BCM5464_MII_1000X_AN_DEBUGr_fields,
#else
	0,
#endif
#endif
	 CDK_SYMBOL_INDEX_SIZE_ENCODE(1),
	CDK_SYMBOL_FLAG_REGISTER,
	"MII_1000X_AN_DEBUGr",
},
{
	BCM5464_AUX_1000X_CTRLr,
#if  CDK_CONFIG_INCLUDE_FIELD_INFO == 1
#ifndef CDK_CONFIG_EXCLUDE_FIELD_INFO_BCM5464
	BCM5464_AUX_1000X_CTRLr_fields,
#else
	0,
#endif
#endif
	 CDK_SYMBOL_INDEX_SIZE_ENCODE(1),
	CDK_SYMBOL_FLAG_REGISTER,
	"AUX_1000X_CTRLr",
},
{
	BCM5464_AUX_1000X_STATr,
#if  CDK_CONFIG_INCLUDE_FIELD_INFO == 1
#ifndef CDK_CONFIG_EXCLUDE_FIELD_INFO_BCM5464
	BCM5464_AUX_1000X_STATr_fields,
#else
	0,
#endif
#endif
	 CDK_SYMBOL_INDEX_SIZE_ENCODE(1),
	CDK_SYMBOL_FLAG_REGISTER,
	"AUX_1000X_STATr",
},
{
	BCM5464_MISC_1000X_STATr,
#if  CDK_CONFIG_INCLUDE_FIELD_INFO == 1
#ifndef CDK_CONFIG_EXCLUDE_FIELD_INFO_BCM5464
	BCM5464_MISC_1000X_STATr_fields,
#else
	0,
#endif
#endif
	 CDK_SYMBOL_INDEX_SIZE_ENCODE(1),
	CDK_SYMBOL_FLAG_REGISTER,
	"MISC_1000X_STATr",
},
{
	BCM5464_AUTO_DETECT_MEDIUMr,
#if  CDK_CONFIG_INCLUDE_FIELD_INFO == 1
#ifndef CDK_CONFIG_EXCLUDE_FIELD_INFO_BCM5464
	BCM5464_AUTO_DETECT_MEDIUMr_fields,
#else
	0,
#endif
#endif
	 CDK_SYMBOL_INDEX_SIZE_ENCODE(1),
	CDK_SYMBOL_FLAG_REGISTER,
	"AUTO_DETECT_MEDIUMr",
},
{
	BCM5464_MODE_CTRLr,
#if  CDK_CONFIG_INCLUDE_FIELD_INFO == 1
#ifndef CDK_CONFIG_EXCLUDE_FIELD_INFO_BCM5464
	BCM5464_MODE_CTRLr_fields,
#else
	0,
#endif
#endif
	 CDK_SYMBOL_INDEX_SIZE_ENCODE(1),
	CDK_SYMBOL_FLAG_REGISTER,
	"MODE_CTRLr",
},
{
	BCM5464_TEST1r,
#if  CDK_CONFIG_INCLUDE_FIELD_INFO == 1
#ifndef CDK_CONFIG_EXCLUDE_FIELD_INFO_BCM5464
	BCM5464_TEST1r_fields,
#else
	0,
#endif
#endif
	 CDK_SYMBOL_INDEX_SIZE_ENCODE(1),
	CDK_SYMBOL_FLAG_REGISTER,
	"TEST1r",
},
{
	BCM5464_MII_1000X_CTRLr,
#if  CDK_CONFIG_INCLUDE_FIELD_INFO == 1
#ifndef CDK_CONFIG_EXCLUDE_FIELD_INFO_BCM5464
	BCM5464_MII_1000X_CTRLr_fields,
#else
	0,
#endif
#endif
	 CDK_SYMBOL_INDEX_SIZE_ENCODE(1),
	CDK_SYMBOL_FLAG_REGISTER,
	"MII_1000X_CTRLr",
},
{
	BCM5464_MII_1000X_STATr,
#if  CDK_CONFIG_INCLUDE_FIELD_INFO == 1
#ifndef CDK_CONFIG_EXCLUDE_FIELD_INFO_BCM5464
	BCM5464_MII_1000X_STATr_fields,
#else
	0,
#endif
#endif
	 CDK_SYMBOL_INDEX_SIZE_ENCODE(1),
	CDK_SYMBOL_FLAG_REGISTER,
	"MII_1000X_STATr",
},
{
	BCM5464_MII_1000X_ANAr,
#if  CDK_CONFIG_INCLUDE_FIELD_INFO == 1
#ifndef CDK_CONFIG_EXCLUDE_FIELD_INFO_BCM5464
	BCM5464_MII_1000X_ANAr_fields,
#else
	0,
#endif
#endif
	 CDK_SYMBOL_INDEX_SIZE_ENCODE(1),
	CDK_SYMBOL_FLAG_REGISTER,
	"MII_1000X_ANAr",
},
{
	BCM5464_MII_1000X_ANPr,
#if  CDK_CONFIG_INCLUDE_FIELD_INFO == 1
#ifndef CDK_CONFIG_EXCLUDE_FIELD_INFO_BCM5464
	BCM5464_MII_1000X_ANPr_fields,
#else
	0,
#endif
#endif
	 CDK_SYMBOL_INDEX_SIZE_ENCODE(1),
	CDK_SYMBOL_FLAG_REGISTER,
	"MII_1000X_ANPr",
},
{
	BCM5464_MII_1000X_AN_EXPr,
#if  CDK_CONFIG_INCLUDE_FIELD_INFO == 1
#ifndef CDK_CONFIG_EXCLUDE_FIELD_INFO_BCM5464
	BCM5464_MII_1000X_AN_EXPr_fields,
#else
	0,
#endif
#endif
	 CDK_SYMBOL_INDEX_SIZE_ENCODE(1),
	CDK_SYMBOL_FLAG_REGISTER,
	"MII_1000X_AN_EXPr",
},
{
	BCM5464_MII_1000X_EXT_STATr,
#if  CDK_CONFIG_INCLUDE_FIELD_INFO == 1
#ifndef CDK_CONFIG_EXCLUDE_FIELD_INFO_BCM5464
	BCM5464_MII_1000X_EXT_STATr_fields,
#else
	0,
#endif
#endif
	 CDK_SYMBOL_INDEX_SIZE_ENCODE(1),
	CDK_SYMBOL_FLAG_REGISTER,
	"MII_1000X_EXT_STATr",
},
{
	BCM5464_EXP_PKT_COUNTERr,
#if  CDK_CONFIG_INCLUDE_FIELD_INFO == 1
#ifndef CDK_CONFIG_EXCLUDE_FIELD_INFO_BCM5464
	BCM5464_EXP_PKT_COUNTERr_fields,
#else
	0,
#endif
#endif
	 CDK_SYMBOL_INDEX_SIZE_ENCODE(1),
	CDK_SYMBOL_FLAG_REGISTER,
	"EXP_PKT_COUNTERr",
},
{
	BCM5464_EXP_INTERRUPT_STATr,
#if  CDK_CONFIG_INCLUDE_FIELD_INFO == 1
#ifndef CDK_CONFIG_EXCLUDE_FIELD_INFO_BCM5464
	BCM5464_EXP_INTERRUPT_STATr_fields,
#else
	0,
#endif
#endif
	 CDK_SYMBOL_INDEX_SIZE_ENCODE(1),
	CDK_SYMBOL_FLAG_REGISTER,
	"EXP_INTERRUPT_STATr",
},
{
	BCM5464_EXP_INTERRUPT_MASKr,
#if  CDK_CONFIG_INCLUDE_FIELD_INFO == 1
#ifndef CDK_CONFIG_EXCLUDE_FIELD_INFO_BCM5464
	BCM5464_EXP_INTERRUPT_MASKr_fields,
#else
	0,
#endif
#endif
	 CDK_SYMBOL_INDEX_SIZE_ENCODE(1),
	CDK_SYMBOL_FLAG_REGISTER,
	"EXP_INTERRUPT_MASKr",
},
{
	BCM5464_EXP_SERDES_PASSTHRU_ENr,
#if  CDK_CONFIG_INCLUDE_FIELD_INFO == 1
#ifndef CDK_CONFIG_EXCLUDE_FIELD_INFO_BCM5464
	BCM5464_EXP_SERDES_PASSTHRU_ENr_fields,
#else
	0,
#endif
#endif
	 CDK_SYMBOL_INDEX_SIZE_ENCODE(1),
	CDK_SYMBOL_FLAG_REGISTER,
	"EXP_SERDES_PASSTHRU_ENr",
},
{
	BCM5464_EXP_LED_SELECTORr,
#if  CDK_CONFIG_INCLUDE_FIELD_INFO == 1
#ifndef CDK_CONFIG_EXCLUDE_FIELD_INFO_BCM5464
	BCM5464_EXP_LED_SELECTORr_fields,
#else
	0,
#endif
#endif
	 CDK_SYMBOL_INDEX_SIZE_ENCODE(1),
	CDK_SYMBOL_FLAG_REGISTER,
	"EXP_LED_SELECTORr",
},
{
	BCM5464_EXP_LED_FLASH_CTRLr,
#if  CDK_CONFIG_INCLUDE_FIELD_INFO == 1
#ifndef CDK_CONFIG_EXCLUDE_FIELD_INFO_BCM5464
	BCM5464_EXP_LED_FLASH_CTRLr_fields,
#else
	0,
#endif
#endif
	 CDK_SYMBOL_INDEX_SIZE_ENCODE(1),
	CDK_SYMBOL_FLAG_REGISTER,
	"EXP_LED_FLASH_CTRLr",
},
{
	BCM5464_EXP_LED_BLINK_CTRLr,
#if  CDK_CONFIG_INCLUDE_FIELD_INFO == 1
#ifndef CDK_CONFIG_EXCLUDE_FIELD_INFO_BCM5464
	BCM5464_EXP_LED_BLINK_CTRLr_fields,
#else
	0,
#endif
#endif
	 CDK_SYMBOL_INDEX_SIZE_ENCODE(1),
	CDK_SYMBOL_FLAG_REGISTER,
	"EXP_LED_BLINK_CTRLr",
},
{
	BCM5464_EXP_PASSTHRU_LED_MODEr,
#if  CDK_CONFIG_INCLUDE_FIELD_INFO == 1
#ifndef CDK_CONFIG_EXCLUDE_FIELD_INFO_BCM5464
	BCM5464_EXP_PASSTHRU_LED_MODEr_fields,
#else
	0,
#endif
#endif
	 CDK_SYMBOL_INDEX_SIZE_ENCODE(1),
	CDK_SYMBOL_FLAG_REGISTER,
	"EXP_PASSTHRU_LED_MODEr",
},
#endif
};


cdk_symbols_t bcm5464_symbols = 
{
   bcm5464_syms, sizeof(bcm5464_syms)/sizeof(bcm5464_syms[0]),
#if CDK_CONFIG_INCLUDE_FIELD_NAMES == 1
   bcm5464_fields
#else
   NULL
#endif
/* END OF SYMBOL FILE */
};

#endif /* CDK_CONFIG_INCLUDE_CHIP_SYMBOLS */