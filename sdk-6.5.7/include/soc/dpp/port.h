/*
 * $Id: port.h Exp $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 */

#ifndef _SOC_DPP_PORT_H
#define _SOC_DPP_PORT_H

/*!
 * @struct soc_dpp_pm_entry_s
 * @brief This struct contains information about pm in pml table
 */
typedef struct soc_dpp_pm_entry_s {
    int is_qsgmii;
    int is_valid;
    int pml_instance;
    int phy_id;
} soc_dpp_pm_entry_t;

typedef struct soc_dpp_port_prd_config_s
{
    uint32 untagged_pcp;
} soc_dpp_port_prd_config_t;

#ifdef BCM_88675_A0
#define SOC_DPP_IS_ILKN_PORT_OVER_FABRIC(unit, ilkn_if_id) \
        ((ilkn_if_id == 4 && SOC_DPP_CONFIG(unit)->jer->nif.ilkn_over_fabric[0]) ||  \
        (ilkn_if_id == 5 && SOC_DPP_CONFIG(unit)->jer->nif.ilkn_over_fabric[1]))
#define SOC_DPP_FIRST_FABRIC_PHY_PORT(unit) 192
#else 
#define SOC_DPP_IS_ILKN_PORT_OVER_FABRIC(unit, ilkn_if_id) 0
#define SOC_DPP_FIRST_FABRIC_PHY_PORT(unit) 0
#endif

#endif /* _SOC_DPP_PORT_H */

