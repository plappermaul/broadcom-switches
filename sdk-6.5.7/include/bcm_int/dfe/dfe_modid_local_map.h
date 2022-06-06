/*
 * $Id: dfe_modid_local_map.h,v 1.3 Broadcom SDK $
 *
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * DFE MODID LOCAL MAP H
 */
 
#ifndef _BCM_DFE_MODID_LOCAL_MAP_H_
#define _BCM_DFE_MODID_LOCAL_MAP_H_

#include <bcm/types.h>

#include <soc/dfe/cmn/dfe_drv.h>

#define DFE_MODID_LOCAL_MAP_ROWS(unit)  (SOC_DFE_MODID_LOCAL_NOF(unit))

#define DFE_MODID_LOCAL_MAP_ROW_VALIDATE(unit, raw) (((raw < DFE_MODID_LOCAL_MAP_ROWS(unit)) && (raw > -1)) ? 1 : 0)

typedef  soc_dfe_modid_local_map_t _bcm_dfe_modid_local_map_t;

int bcm_dfe_modid_local_map_clear(int unit);
int bcm_dfe_modid_local_map_module_clear(int unit, bcm_module_t local_module_id);
int bcm_dfe_modid_local_map_is_valid(int unit, bcm_module_t local_module_id, int* is_valid);
int bcm_dfe_modid_local_map_add(int unit, bcm_module_t local_module_id, bcm_module_t module_id, int allow_override);
int bcm_dfe_modid_local_map_get(int unit, bcm_module_t local_module_id, bcm_module_t* module_id);
int bcm_dfe_modid_local_map_modid_to_local(int unit, bcm_module_t module_id, bcm_module_t* local_module_id);


#endif /*_BCM_DFE_MODID_LOCAL_MAP_H_*/
