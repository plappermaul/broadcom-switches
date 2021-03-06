/*
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated from the registers file.
 * Edits to this file will be lost when it is regenerated.
 *
 * $Id: $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * File:	driver.h
 * Purpose:	Chip driver declarations
 * Supports:	bcm53242_a0
 *		bcm53262_a0
 *		bcm53115_a0
 *		bcm53118_a0
 *		bcm53280_a0
 *		bcm53280_b0
 *		bcm53101_a0
 *		bcm53125_a0
 *		bcm53128_a0
 *		bcm53134_a0
 *		bcm53134_b0
 *		bcm53600_a0
 *		bcm89500_a0
 *		bcm53010_a0
 *		bcm5389_a0
 *		bcm53020_a0
 *		bcm5396_a0
 */

#ifndef _SOC_ROBO_DRIVER_H
#define _SOC_ROBO_DRIVER_H


#include <soc/drv.h>

/****************************************************************
 *
 *    Chip driver declarations
 *
 ****************************************************************/

EXTERN soc_driver_t soc_driver_bcm53242_a0;
EXTERN soc_driver_t soc_driver_bcm53262_a0;
EXTERN soc_driver_t soc_driver_bcm53115_a0;
EXTERN soc_driver_t soc_driver_bcm53118_a0;
EXTERN soc_driver_t soc_driver_bcm53280_a0;
EXTERN soc_driver_t soc_driver_bcm53280_b0;
EXTERN soc_driver_t soc_driver_bcm53101_a0;
EXTERN soc_driver_t soc_driver_bcm53125_a0;
EXTERN soc_driver_t soc_driver_bcm53128_a0;
EXTERN soc_driver_t soc_driver_bcm53134_a0;
EXTERN soc_driver_t soc_driver_bcm53134_b0;
EXTERN soc_driver_t soc_driver_bcm53600_a0;
EXTERN soc_driver_t soc_driver_bcm89500_a0;
EXTERN soc_driver_t soc_driver_bcm53010_a0;
EXTERN soc_driver_t soc_driver_bcm5389_a0;
EXTERN soc_driver_t soc_driver_bcm53020_a0;
EXTERN soc_driver_t soc_driver_bcm5396_a0;
#ifdef ETH_MII_DEBUG
EXTERN soc_driver_t soc_driver_bcm4713_a0;
#endif
/* The table of all known drivers */
/* Indexed by supported chips */
EXTERN soc_driver_t *soc_robo_base_driver_table[SOC_ROBO_NUM_SUPPORTED_CHIPS];

EXTERN int soc_robo_chip_type_to_index(soc_chip_types chip);
EXTERN soc_chip_types soc_robo_index_to_chip_type(int chip);


#endif	/* !_SOC_ROBO_DRIVER_H */
