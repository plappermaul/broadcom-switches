/*
 * 
 * 
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2021 Broadcom Inc. All rights reserved.
 * This file is auto-generated by autoCoder
 * DO NOT EDIT THIS FILE!
 */

#include <shared/bsl.h>
#include <soc/defs.h>
#ifdef BCM_88950_A0

#include <soc/dfe/cmn/dfe_config_defs.h>
#include <soc/dcmn/dcmn_defs.h>
#include <soc/dfe/fe3200/fe3200_intr.h>
#include <soc/dfe/fe3200/fe3200_config_defs.h>

#ifdef _ERR_MSG_MODULE_NAME
    #error "_ERR_MSG_MODULE_NAME redefined"
#endif 

#define _ERR_MSG_MODULE_NAME BSL_SOC_INIT

CONST soc_dfe_defines_t soc_fe3200_defines= {
    SOC_FE3200_NOF_LINKS,
    SOC_FE3200_NOF_INSTANCES_MAC,
    SOC_FE3200_NOF_LINKS_IN_MAC,
    SOC_FE3200_NOF_INSTANCES_MAC_FSRD,
    SOC_FE3200_NOF_INSTANCES_BRDC_FSRD,
    SOC_FE3200_NOF_LINKS_IN_FSRD,
    SOC_FE3200_NOF_QUADS_IN_FSRD,
    SOC_FE3200_NOF_LINKS_IN_QUAD,
    SOC_FE3200_NOF_INSTANCES_DCH,
    SOC_FE3200_NOF_INSTANCES_CCS,
    SOC_FE3200_NOF_LINKS_IN_CCS,
    SOC_FE3200_NOF_INSTANCES_DCL,
    SOC_FE3200_NOF_LINKS_IN_DCQ,
    SOC_FE3200_NOF_INSTANCES_RTP,
    SOC_FE3200_NOF_INSTANCES_OCCG,
    SOC_FE3200_NOF_INSTANCES_ECI,
    SOC_FE3200_NOF_INSTANCES_DCMA,
    SOC_FE3200_NOF_INSTANCES_DCMB,
    SOC_FE3200_NOF_INSTANCES_DCMC,
    SOC_FE3200_NOF_INSTANCES_DCM,
    SOC_FE3200_NOF_INSTANCES_CMIC,
    SOC_FE3200_NOF_INSTANCES_MESH_TOPOLOGY,
    SOC_FE3200_NOF_INSTANCES_OTPC,
    SOC_FE3200_NOF_INSTANCES_BRDC_FMACH,
    SOC_FE3200_NOF_INSTANCES_BRDC_FMACL,
    SOC_FE3200_NOF_INSTANCES_BRDC_FMAC_AC,
    SOC_FE3200_NOF_INSTANCES_BRDC_FMAC_BD,
    SOC_FE3200_NOF_INSTANCES_BRDC_DCH,
    SOC_FE3200_NOF_INSTANCES_BRDC_DCM,
    SOC_FE3200_NOF_INSTANCES_BRDC_DCL,
    SOC_FE3200_NOF_INSTANCES_BRDC_CCS,
    SOC_FE3200_MAX_LINK_SCORE,
    SOC_FE3200_MULTICAST_ROW_SIZE_IN_UINT32,
    SOC_FE3200_MAX_BUCKET_FILL_RATE,
    SOC_FE3200_RTP_RMHMT_GROUP_SIZE,
    SOC_FE3200_NOF_LCPLL,
    SOC_FE3200_NOF_PORT_IN_LCPLL,
    SOC_FE3200_RTP_RMGR_UNITS,
    SOC_FE3200_RTP_RMGR_NOF_LINKS,
    SOC_FE3200_NOF_INTERRUPTS,
    SOC_FE3200_NOF_RTP_MCLBTP_INSTANCES,
    SOC_FE3200_NOF_RTP_MCLBTS_INSTANCES,
    SOC_FE3200_NOF_LINKS_IN_DCL,
    SOC_FE3200_FIFO_DMA_FABRIC_CELL_FIRST_CHANNEL,
    SOC_FE3200_FIFO_DMA_NOF_FABRIC_CELL_CHANNELS,
    SOC_FE3200_FIFO_DMA_FABRIC_CELL_NOF_ENTRIES_PER_CELL,
    SOC_FE3200_REPEATER_NONE_EMPTY_CELL_SIZE,
    SOC_FE3200_REPEATER_DEFAULT_EMPTY_CELL_SIZE,
    SOC_FE3200_NOF_LOCAL_MODID,
    SOC_FE3200_NOF_LOCAL_MODID_FE13,
    SOC_FE3200_FABRIC_MAC_BUS_SIZE,
    SOC_FE3200_LINK_MASK,
    SOC_FE3200_RX_CPU_CELL_MAX_PAYLOAD_SIZE,
    SOC_FE3200_SOURCE_ROUTED_CELLS_HEADER_OFFSET,
    SOC_FE3200_ALDWP_MAX_VAL,
};


int soc_fe3200_defines_init(int unit)
{
    SOCDNX_INIT_FUNC_DEFS;

    if ( soc_dfe_defines[unit] == NULL ) {
        soc_dfe_defines[unit] = (soc_dfe_defines_t *) sal_alloc(sizeof(soc_dfe_defines_t),"Defines struct");
        if ( soc_dfe_defines[unit] == NULL ) {
            SOCDNX_EXIT_WITH_ERR(SOC_E_MEMORY, (_BSL_SOCDNX_MSG("Failed to allocate memory for configuration defines struct")));
        }
    }
        sal_memcpy(soc_dfe_defines[unit], &soc_fe3200_defines, sizeof(soc_dfe_defines_t));

exit:
    SOCDNX_FUNC_RETURN;
}


int soc_fe3200_defines_deinit(int unit)
{
    if ( soc_dfe_defines[unit] != NULL ) {
        sal_free(soc_dfe_defines[unit]);
        soc_dfe_defines[unit] = NULL;
    }

    return SOC_E_NONE;
}





#endif 
