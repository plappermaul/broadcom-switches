/*
 * $Id: phyreg.h,v 1.6 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * This file defines common PHY register definition.
 *
 * Its contents are not used directly by applications; it is used only
 * by header files of parent APIs which need to define PHY register definition.
 */

#ifndef _PHYREG_H
#define _PHYREG_H

#include <shared/phyreg.h>

#define SOC_PHY_SHAD_RETRY_CNT   _SHR_PORT_PHY_SHAD_RETRY_CNT

#define SOC_PHY_INTERNAL      _SHR_PORT_PHY_INTERNAL
#define SOC_PHY_NOMAP         _SHR_PORT_PHY_NOMAP
#define SOC_PHY_CLAUSE45      _SHR_PORT_PHY_CLAUSE45
#define SOC_PHY_I2C_DATA8     _SHR_PORT_PHY_I2C_DATA8       
#define SOC_PHY_I2C_DATA16    _SHR_PORT_PHY_I2C_DATA16  
#define SOC_PHY_PVT_DATA      _SHR_PORT_PHY_PVT_DATA

#define SOC_PHY_CLAUSE45_ADDR(_devad, _regad) \
            _SHR_PORT_PHY_CLAUSE45_ADDR(_devad, _regad)
#define SOC_PHY_CLAUSE45_DEVAD(_reg_addr) \
            _SHR_PORT_PHY_CLAUSE45_DEVAD(_reg_addr)
#define SOC_PHY_CLAUSE45_REGAD(_reg_addr) \
            _SHR_PORT_PHY_CLAUSE45_REGAD(_reg_addr)
#define SOC_PHY_I2C_ADDR(_devad, _regad) \
	_SHR_PORT_PHY_I2C_ADDR(_devad, _regad)
#define SOC_PHY_I2C_DEVAD(_reg_addr) \
	_SHR_PORT_PHY_I2C_DEVAD(_reg_addr) 
#define SOC_PHY_I2C_REGAD(_reg_addr) \
	_SHR_PORT_PHY_I2C_REGAD(_reg_addr)

/* Indirect PHY register address flags */
#define SOC_PHY_REG_NO_RETRY_ACCESS   _SHR_PORT_PHY_REG_NO_RETRY_ACCESS
#define SOC_PHY_REG_RESERVE_ACCESS    _SHR_PORT_PHY_REG_RESERVE_ACCESS
#define SOC_PHY_REG_1000X             _SHR_PORT_PHY_REG_1000X
#define SOC_PHY_REG_INDIRECT          _SHR_PORT_PHY_REG_INDIRECT
#define SOC_PHY_REG_DUMMY_ACCESS      _SHR_PORT_PHY_REG_DUMMY_ACCESS 

#define SOC_PHY_REG_INDIRECT_ADDR(_flags, _reg_type, _reg_selector) \
            _SHR_PORT_PHY_REG_INDIRECT_ADDR(_flags, _reg_type, _reg_selector)
#define SOC_PHY_REG_BANK(_reg_addr)  _SHR_PORT_PHY_REG_BANK(_reg_addr)
#define SOC_PHY_REG_ADDR(_reg_addr)  _SHR_PORT_PHY_REG_ADDR(_reg_addr)
#define SOC_PHY_REG_FLAGS(_reg_addr) _SHR_PORT_PHY_REG_FLAGS(_reg_addr)

#endif  /* !_PHYREG_H */
