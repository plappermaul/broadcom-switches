/*
 * $Id: low_level_defines.csv,v 1.1.2.5 Broadcom SDK $
 * 
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 * This file is auto-generated by autoCoder
 * DO NOT EDIT THIS FILE!
 *
 */
#include <shared/bsl.h>
#include <soc/dpp/dpp_config_imp_defs.h>

#ifdef _ERR_MSG_MODULE_NAME
     #error "_ERR_MSG_MODULE_NAME redefined"
#endif /* _ERR_MSG_MODULE_NAME */

#define _ERR_MSG_MODULE_NAME BSL_SOC_INIT

soc_dpp_implementation_defines_t *soc_dpp_implementation_defines[SOC_MAX_NUM_DEVICES] = {NULL};

int soc_dpp_implementation_defines_init(int unit)
{
    SOCDNX_INIT_FUNC_DEFS;

    switch(SOC_INFO(unit).chip_type){

#ifdef BCM_ARAD_SUPPORT
    case SOC_INFO_CHIP_TYPE_ARAD:
    case SOC_INFO_CHIP_TYPE_ARADPLUS:
        SOCDNX_IF_ERR_EXIT(soc_dpp_arad_implementation_defines_init(unit));
        break;
#endif /* BCM_ARAD_SUPPORT */

#ifdef BCM_JERICHO_SUPPORT
    case SOC_INFO_CHIP_TYPE_JERICHO:
    case SOC_INFO_CHIP_TYPE_QMX:
        SOCDNX_IF_ERR_EXIT(soc_dpp_jericho_implementation_defines_init(unit));
        break;
#endif /* BCM_JERICHO_SUPPORT */

#ifdef BCM_JERICHO_PLUS_SUPPORT
    case SOC_INFO_CHIP_TYPE_JERICHO_PLUS:
        SOCDNX_IF_ERR_EXIT(soc_dpp_jericho_plus_implementation_defines_init(unit));
        break;
#endif /* BCM_JERICHO_PLUS_SUPPORT */

#ifdef BCM_88202_A0
    case SOC_INFO_CHIP_TYPE_ARDON:
        SOCDNX_IF_ERR_EXIT(soc_dpp_ardon_implementation_defines_init(unit));
        break;
#endif /* BCM_88202_A0 */

#ifdef BCM_QAX_SUPPORT
    case SOC_INFO_CHIP_TYPE_QAX:
    case SOC_INFO_CHIP_TYPE_KALIA:
        SOCDNX_IF_ERR_EXIT(soc_dpp_qax_implementation_defines_init(unit));
        break;
#endif /* BCM_QAX_SUPPORT */

#ifdef BCM_QUX_SUPPORT
    case SOC_INFO_CHIP_TYPE_QUX:
        SOCDNX_IF_ERR_EXIT(soc_dpp_qux_implementation_defines_init(unit));
        break;
#endif /* BCM_QUX_SUPPORT */

#ifdef BCM_FLAIR_SUPPORT
    case SOC_INFO_CHIP_TYPE_FLAIR:
        SOCDNX_IF_ERR_EXIT(soc_dpp_flair_implementation_defines_init(unit));
        break;
#endif /* BCM_FLAIR_SUPPORT */

    default:
         SOCDNX_EXIT_WITH_ERR(SOC_E_INIT, (_BSL_SOCDNX_MSG("Defines share autoCoder: Unsupported device, chip type: %d"), SOC_INFO(unit).chip_type));
        break;
    }

exit:
    SOCDNX_FUNC_RETURN;
}

int soc_dpp_implementation_defines_deinit(int unit)
{
    SOCDNX_INIT_FUNC_DEFS;

    switch(SOC_INFO(unit).chip_type){

#ifdef BCM_ARAD_SUPPORT
    case SOC_INFO_CHIP_TYPE_ARAD:
    case SOC_INFO_CHIP_TYPE_ARADPLUS:
        SOCDNX_IF_ERR_EXIT(soc_dpp_arad_implementation_defines_deinit(unit));
        break;
#endif /* BCM_ARAD_SUPPORT */

#ifdef BCM_JERICHO_SUPPORT
    case SOC_INFO_CHIP_TYPE_JERICHO:
    case SOC_INFO_CHIP_TYPE_QMX:
        SOCDNX_IF_ERR_EXIT(soc_dpp_jericho_implementation_defines_deinit(unit));
        break;
#endif /* BCM_JERICHO_SUPPORT */

#ifdef BCM_JERICHO_PLUS_SUPPORT
    case SOC_INFO_CHIP_TYPE_JERICHO_PLUS:
        SOCDNX_IF_ERR_EXIT(soc_dpp_jericho_plus_implementation_defines_deinit(unit));
        break;
#endif /* BCM_JERICHO_PLUS_SUPPORT */

#ifdef BCM_88202_A0
    case SOC_INFO_CHIP_TYPE_ARDON:
        SOCDNX_IF_ERR_EXIT(soc_dpp_ardon_implementation_defines_deinit(unit));
        break;
#endif /* BCM_88202_A0 */

#ifdef BCM_QAX_SUPPORT
    case SOC_INFO_CHIP_TYPE_QAX:
    case SOC_INFO_CHIP_TYPE_KALIA:
        SOCDNX_IF_ERR_EXIT(soc_dpp_qax_implementation_defines_deinit(unit));
        break;
#endif /* BCM_QAX_SUPPORT */

#ifdef BCM_QUX_SUPPORT
    case SOC_INFO_CHIP_TYPE_QUX:
        SOCDNX_IF_ERR_EXIT(soc_dpp_qux_implementation_defines_deinit(unit));
        break;
#endif /* BCM_QUX_SUPPORT */

#ifdef BCM_FLAIR_SUPPORT
    case SOC_INFO_CHIP_TYPE_FLAIR:
        SOCDNX_IF_ERR_EXIT(soc_dpp_flair_implementation_defines_deinit(unit));
        break;
#endif /* BCM_FLAIR_SUPPORT */

    default:
         SOCDNX_EXIT_WITH_ERR(SOC_E_INIT, (_BSL_SOCDNX_MSG("Defines share autoCoder: Unsupported device, chip type: %d"), SOC_INFO(unit).chip_type));
        break;
    }

exit:
    SOCDNX_FUNC_RETURN;
}


