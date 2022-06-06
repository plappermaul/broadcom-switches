/*
 * 
 * 
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2021 Broadcom Inc. All rights reserved.
 * This file is auto-generated by autoCoder
 * DO NOT EDIT THIS FILE!
 *
 */
#ifndef FE3200_CONFIG_IMP_DEFS
#define FE3200_CONFIG_IMP_DEFS

#include <soc/defs.h>
#ifdef BCM_88950_A0


#define SOC_FE3200_GET(dfe_define)    SOC_FE3200_##dfe_define
#define SOC_FE3200_RTP_REACHABILTY_GEN_RATE_PER_LINK        (50000)
#define SOC_FE3200_RTP_REACHABILTY_WATCHDOG_RATE            (100000)
#define SOC_FE3200_MDIO_INT_DIVIDEND_DEFAULT                (1)
#define SOC_FE3200_MDIO_INT_FREQ_DEFAULT                    (10)
#define SOC_FE3200_MDIO_INT_OUT_DELAY_DEFAULT               (0x9)
#define SOC_FE3200_FABRIC_CLOCK_FREQ_IN_DEFAULT             (0)
#define SOC_FE3200_FABRIC_CLOCK_FREQ_OUT_DEFAULT            (0)
#define SOC_FE3200_LCPLL_CONTROL1_125_VAL                   (-1)
#define SOC_FE3200_LCPLL_CONTROL1_156_25_VAL                (-1)
#define SOC_FE3200_LCPLL_CONTROL3_125_VAL                   (-1)
#define SOC_FE3200_LCPLL_CONTROL3_156_25_VAL                (-1)
#define SOC_FE3200_FIFO_DMA_NOF_CHANNELS                    (4)


int soc_fe3200_implementation_defines_init(int unit);
int soc_fe3200_implementation_defines_deinit(int unit);


#else 
#define SOC_FE3200_GET(dfe_define)    0
#endif 
#endif 
