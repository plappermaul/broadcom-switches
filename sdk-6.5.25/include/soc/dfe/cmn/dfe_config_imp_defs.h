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
#ifndef DFE_CONFIG_IMP_DEFS
#define DFE_CONFIG_IMP_DEFS

#include <soc/error.h>
#include <soc/dcmn/error.h>
#include <soc/drv.h>
#include <sal/core/alloc.h>

#include <soc/dfe/fe3200/fe3200_config_imp_defs.h>








typedef struct soc_dfe_implementation_defines_s {
    int        soc_dfe_imp_defs_rtp_reachabilty_gen_rate_per_link;        
    int        soc_dfe_imp_defs_rtp_reachabilty_watchdog_rate;            
    int        soc_dfe_imp_defs_mdio_int_dividend_default;                
    int        soc_dfe_imp_defs_mdio_int_freq_default;                    
    int        soc_dfe_imp_defs_mdio_int_out_delay_default;               
    int        soc_dfe_imp_defs_fabric_clock_freq_in_default;             
    int        soc_dfe_imp_defs_fabric_clock_freq_out_default;            
    int        soc_dfe_imp_defs_lcpll_control1_125_val;                   
    int        soc_dfe_imp_defs_lcpll_control1_156_25_val;                
    int        soc_dfe_imp_defs_lcpll_control3_125_val;                   
    int        soc_dfe_imp_defs_lcpll_control3_156_25_val;                
    int        soc_dfe_imp_defs_fifo_dma_nof_channels;                    
} soc_dfe_implementation_defines_t;

extern soc_dfe_implementation_defines_t *soc_dfe_implementation_defines[SOC_MAX_NUM_DEVICES];







#define SOC_DFE_IMP_DEFS_SET(unit, dfe_define, value)    {soc_dfe_implementation_defines[unit]->soc_dfe_imp_defs_##dfe_define = value;}
#define SOC_DFE_IMP_DEFS_GET(unit, dfe_define)           ( soc_dfe_implementation_defines[unit]->soc_dfe_imp_defs_##dfe_define )
#define SOC_DFE_IMP_DEFS_MAX(dfe_define)                 SOC_DFE_IMP_DEFS_##dfe_define##_MAX

#ifdef BCM_88950_A0
#ifdef SOC_DFE_IMP_DEFS_RTP_REACHABILTY_GEN_RATE_PER_LINK_MAX
#if (50000) > SOC_DFE_IMP_DEFS_RTP_REACHABILTY_GEN_RATE_PER_LINK_MAX
#undef SOC_DFE_IMP_DEFS_RTP_REACHABILTY_GEN_RATE_PER_LINK_MAX
#define SOC_DFE_IMP_DEFS_RTP_REACHABILTY_GEN_RATE_PER_LINK_MAX 50000
#endif 
#else
#define SOC_DFE_IMP_DEFS_RTP_REACHABILTY_GEN_RATE_PER_LINK_MAX 50000
#endif 
#endif 



#ifdef BCM_88950_A0
#ifdef SOC_DFE_IMP_DEFS_RTP_REACHABILTY_WATCHDOG_RATE_MAX
#if (100000) > SOC_DFE_IMP_DEFS_RTP_REACHABILTY_WATCHDOG_RATE_MAX
#undef SOC_DFE_IMP_DEFS_RTP_REACHABILTY_WATCHDOG_RATE_MAX
#define SOC_DFE_IMP_DEFS_RTP_REACHABILTY_WATCHDOG_RATE_MAX 100000
#endif 
#else
#define SOC_DFE_IMP_DEFS_RTP_REACHABILTY_WATCHDOG_RATE_MAX 100000
#endif 
#endif 



#ifdef BCM_88950_A0
#ifdef SOC_DFE_IMP_DEFS_MDIO_INT_DIVIDEND_DEFAULT_MAX
#if (1) > SOC_DFE_IMP_DEFS_MDIO_INT_DIVIDEND_DEFAULT_MAX
#undef SOC_DFE_IMP_DEFS_MDIO_INT_DIVIDEND_DEFAULT_MAX
#define SOC_DFE_IMP_DEFS_MDIO_INT_DIVIDEND_DEFAULT_MAX 1
#endif 
#else
#define SOC_DFE_IMP_DEFS_MDIO_INT_DIVIDEND_DEFAULT_MAX 1
#endif 
#endif 



#ifdef BCM_88950_A0
#ifdef SOC_DFE_IMP_DEFS_MDIO_INT_FREQ_DEFAULT_MAX
#if (10) > SOC_DFE_IMP_DEFS_MDIO_INT_FREQ_DEFAULT_MAX
#undef SOC_DFE_IMP_DEFS_MDIO_INT_FREQ_DEFAULT_MAX
#define SOC_DFE_IMP_DEFS_MDIO_INT_FREQ_DEFAULT_MAX 10
#endif 
#else
#define SOC_DFE_IMP_DEFS_MDIO_INT_FREQ_DEFAULT_MAX 10
#endif 
#endif 



#ifdef BCM_88950_A0
#ifdef SOC_DFE_IMP_DEFS_MDIO_INT_OUT_DELAY_DEFAULT_MAX
#if (0x9) > SOC_DFE_IMP_DEFS_MDIO_INT_OUT_DELAY_DEFAULT_MAX
#undef SOC_DFE_IMP_DEFS_MDIO_INT_OUT_DELAY_DEFAULT_MAX
#define SOC_DFE_IMP_DEFS_MDIO_INT_OUT_DELAY_DEFAULT_MAX 0x9
#endif 
#else
#define SOC_DFE_IMP_DEFS_MDIO_INT_OUT_DELAY_DEFAULT_MAX 0x9
#endif 
#endif 



#ifdef BCM_88950_A0
#ifdef SOC_DFE_IMP_DEFS_FABRIC_CLOCK_FREQ_IN_DEFAULT_MAX
#if (0) > SOC_DFE_IMP_DEFS_FABRIC_CLOCK_FREQ_IN_DEFAULT_MAX
#undef SOC_DFE_IMP_DEFS_FABRIC_CLOCK_FREQ_IN_DEFAULT_MAX
#define SOC_DFE_IMP_DEFS_FABRIC_CLOCK_FREQ_IN_DEFAULT_MAX 0
#endif 
#else
#define SOC_DFE_IMP_DEFS_FABRIC_CLOCK_FREQ_IN_DEFAULT_MAX 0
#endif 
#endif 



#ifdef BCM_88950_A0
#ifdef SOC_DFE_IMP_DEFS_FABRIC_CLOCK_FREQ_OUT_DEFAULT_MAX
#if (0) > SOC_DFE_IMP_DEFS_FABRIC_CLOCK_FREQ_OUT_DEFAULT_MAX
#undef SOC_DFE_IMP_DEFS_FABRIC_CLOCK_FREQ_OUT_DEFAULT_MAX
#define SOC_DFE_IMP_DEFS_FABRIC_CLOCK_FREQ_OUT_DEFAULT_MAX 0
#endif 
#else
#define SOC_DFE_IMP_DEFS_FABRIC_CLOCK_FREQ_OUT_DEFAULT_MAX 0
#endif 
#endif 



#ifdef BCM_88950_A0
#ifdef SOC_DFE_IMP_DEFS_LCPLL_CONTROL1_125_VAL_MAX
#if (-1) > SOC_DFE_IMP_DEFS_LCPLL_CONTROL1_125_VAL_MAX
#undef SOC_DFE_IMP_DEFS_LCPLL_CONTROL1_125_VAL_MAX
#define SOC_DFE_IMP_DEFS_LCPLL_CONTROL1_125_VAL_MAX -1
#endif 
#else
#define SOC_DFE_IMP_DEFS_LCPLL_CONTROL1_125_VAL_MAX -1
#endif 
#endif 



#ifdef BCM_88950_A0
#ifdef SOC_DFE_IMP_DEFS_LCPLL_CONTROL1_156_25_VAL_MAX
#if (-1) > SOC_DFE_IMP_DEFS_LCPLL_CONTROL1_156_25_VAL_MAX
#undef SOC_DFE_IMP_DEFS_LCPLL_CONTROL1_156_25_VAL_MAX
#define SOC_DFE_IMP_DEFS_LCPLL_CONTROL1_156_25_VAL_MAX -1
#endif 
#else
#define SOC_DFE_IMP_DEFS_LCPLL_CONTROL1_156_25_VAL_MAX -1
#endif 
#endif 



#ifdef BCM_88950_A0
#ifdef SOC_DFE_IMP_DEFS_LCPLL_CONTROL3_125_VAL_MAX
#if (-1) > SOC_DFE_IMP_DEFS_LCPLL_CONTROL3_125_VAL_MAX
#undef SOC_DFE_IMP_DEFS_LCPLL_CONTROL3_125_VAL_MAX
#define SOC_DFE_IMP_DEFS_LCPLL_CONTROL3_125_VAL_MAX -1
#endif 
#else
#define SOC_DFE_IMP_DEFS_LCPLL_CONTROL3_125_VAL_MAX -1
#endif 
#endif 



#ifdef BCM_88950_A0
#ifdef SOC_DFE_IMP_DEFS_LCPLL_CONTROL3_156_25_VAL_MAX
#if (-1) > SOC_DFE_IMP_DEFS_LCPLL_CONTROL3_156_25_VAL_MAX
#undef SOC_DFE_IMP_DEFS_LCPLL_CONTROL3_156_25_VAL_MAX
#define SOC_DFE_IMP_DEFS_LCPLL_CONTROL3_156_25_VAL_MAX -1
#endif 
#else
#define SOC_DFE_IMP_DEFS_LCPLL_CONTROL3_156_25_VAL_MAX -1
#endif 
#endif 



#ifdef BCM_88950_A0
#ifdef SOC_DFE_IMP_DEFS_FIFO_DMA_NOF_CHANNELS_MAX
#if (4) > SOC_DFE_IMP_DEFS_FIFO_DMA_NOF_CHANNELS_MAX
#undef SOC_DFE_IMP_DEFS_FIFO_DMA_NOF_CHANNELS_MAX
#define SOC_DFE_IMP_DEFS_FIFO_DMA_NOF_CHANNELS_MAX 4
#endif 
#else
#define SOC_DFE_IMP_DEFS_FIFO_DMA_NOF_CHANNELS_MAX 4
#endif 
#endif 



int soc_dfe_implementation_defines_init(int unit);
int soc_dfe_implementation_defines_deinit(int unit);

#endif 
