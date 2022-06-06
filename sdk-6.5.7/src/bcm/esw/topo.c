/*
 * $Id: topo.c,v 1.10 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 */

#include <soc/drv.h>

#include <bcm/topo.h>
#include <bcm/error.h>
#include <bcm/stack.h>
#include <bcm_int/common/topo.h>

/*
 * Function:
 *      bcm_esw_topo_port_get
 * Purpose:
 *      Determine what port should be used on unit to reach dest_modid
 * Parameters:
 *      unit        - The source unit
 *      dest_modid  - Destination to reach
 *      exit_port   - (OUT) Port to exit device on
 * Returns:
 *      BCM_E_XXX
 * Notes:
 *      The code for the local board is programmed by 
 */
int
bcm_esw_topo_port_get(int unit, int dest_modid, bcm_port_t *exit_port)
{
    return _bcm_topo_port_get(unit, dest_modid, exit_port);
}
