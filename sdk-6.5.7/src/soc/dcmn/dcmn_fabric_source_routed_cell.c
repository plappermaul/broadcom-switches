/*
 * $Id: dcmn_fabric_source_routed_cell.c,v 1.3 Broadcom SDK $
 *
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 */
#ifdef _ERR_MSG_MODULE_NAME
  #error "_ERR_MSG_MODULE_NAME redefined"
#endif

#define _ERR_MSG_MODULE_NAME BSL_SOC_FABRIC
#include <shared/bsl.h>
#include <soc/dcmn/error.h>
#include <soc/drv.h>
#include <soc/dcmn/dcmn_fabric_source_routed_cell.h>
#include <soc/dcmn/dcmn_error.h>


/*
* Function:
*      soc_dcmn_actual_entity_value
* Purpose:
*      Convert type dcmn_fabric_device_type_t to type soc_dcmn_device_type_actual_value_t
* Parameters:
*      device_entity  - (IN)  Value to translate
*      actual_entity  - (IN)  Translated value
* Returns:
*      SOC_E_XXX
*/
soc_error_t
soc_dcmn_actual_entity_value(
                            int unit,
                            dcmn_fabric_device_type_t            device_entity,
                            soc_dcmn_device_type_actual_value_t* actual_entity
                            )
{
    SOCDNX_INIT_FUNC_DEFS;

    switch(device_entity)
    {
    case dcmnFabricDeviceTypeFE1:
        {
            *actual_entity = soc_dcmn_device_type_actual_value_FE1;
            break;
        }
    case dcmnFabricDeviceTypeFE2:
        {
            *actual_entity = soc_dcmn_device_type_actual_value_FE2;
            break;
        }
    case dcmnFabricDeviceTypeFE3:
        {
            *actual_entity = soc_dcmn_device_type_actual_value_FE3;
            break;
        }
    case dcmnFabricDeviceTypeFAP:
        {
            *actual_entity = soc_dcmn_device_type_actual_value_FAP;
            break;
        }
    case dcmnFabricDeviceTypeFOP:
        {
            *actual_entity = soc_dcmn_device_type_actual_value_FOP;
            break;
        }
    case dcmnFabricDeviceTypeFIP:
        {
            *actual_entity = soc_dcmn_device_type_actual_value_FIP;
            break;
        }
    case dcmnFabricDeviceTypeUnknown:
    case dcmnFabricDeviceTypeFE13:
        {
            /*
            * In the context of cells, there is no.
            * FE13 or Unknown entity.
            */
            SOCDNX_EXIT_WITH_ERR(SOC_E_PARAM, (_BSL_SOCDNX_MSG("device_entity %d can't be FE13 or unknown"),device_entity));
            break;
        }
    default:
        {
            /*
            * This is the case of bad use of the method.
            * (Input value is out of range)
            */
            SOCDNX_EXIT_WITH_ERR(SOC_E_PARAM, (_BSL_SOCDNX_MSG("device_entity %d is out-of-range"),device_entity));
            break;
        }
    }

exit:
    SOCDNX_FUNC_RETURN;
}

/*
* Function:
*      soc_dcmn_actual_entity_value
* Purpose:
*      Convert type dcmn_fabric_device_type_t to type soc_dcmn_device_type_actual_value_t
* Parameters:
*      device_entity  - (IN)  Value to translate
*      actual_entity  - (IN)  Translated value
* Returns:
*      SOC_E_XXX
*/
soc_error_t
soc_dcmn_device_entity_value(
                            int unit,
                            soc_dcmn_device_type_actual_value_t actual_entity,
                            dcmn_fabric_device_type_t*          device_entity

                            )
{
    SOCDNX_INIT_FUNC_DEFS;

    switch(actual_entity)
    {
    case soc_dcmn_device_type_actual_value_FE1:
        {
            *device_entity = dcmnFabricDeviceTypeFE1;
            break;
        }
    case soc_dcmn_device_type_actual_value_FE2:
    case soc_dcmn_device_type_actual_value_FE2_1:
        {
            *device_entity = dcmnFabricDeviceTypeFE2;
            break;
        }
    case soc_dcmn_device_type_actual_value_FE3:
        {
            *device_entity = dcmnFabricDeviceTypeFE3;
            break;
        }
    case soc_dcmn_device_type_actual_value_FAP:
    case soc_dcmn_device_type_actual_value_FAP_1:
        {
            *device_entity = dcmnFabricDeviceTypeFAP;
            break;
        }
    case soc_dcmn_device_type_actual_value_FOP:
        {
            *device_entity = dcmnFabricDeviceTypeFOP;
            break;
        }
    case soc_dcmn_device_type_actual_value_FIP:
        {
            *device_entity = dcmnFabricDeviceTypeFIP;
            break;
        }
    default:
        {
            /*
            * This is the case of bad use of the method.
            * (Input value is out of range)
            */
            SOCDNX_EXIT_WITH_ERR(SOC_E_PARAM, (_BSL_SOCDNX_MSG("actual_entity %d is out-of-range"),actual_entity));
            break;
        }
    }

exit:
    SOCDNX_FUNC_RETURN;
}

#undef _ERR_MSG_MODULE_NAME
