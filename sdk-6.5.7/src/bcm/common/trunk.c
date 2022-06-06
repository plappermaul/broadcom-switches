/*
 * $Id: trunk.c,v 1.9 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 */

#include <sal/types.h>

#include <soc/mem.h>
#include <soc/debug.h>
#include <soc/drv.h>

#include <bcm/error.h>
#include <bcm/vlan.h>
#include <bcm/mcast.h>
#include <bcm/l2.h>
#include <bcm/stack.h>
#include <bcm/port.h>
#include <bcm/trunk.h>
#include <bcm/switch.h>

#include <bcm_int/common/trunk.h>

/*
 * Function:
 *      bcm_trunk_info_t_init
 * Purpose:
 *      Initialize the bcm_trunk_info_t structure
 * Parameters:
 *      trunk_info - Pointer to structure which should be initialized
 * Returns:
 *      NONE
 */
void 
bcm_trunk_info_t_init(bcm_trunk_info_t *trunk_info)
{
    if (NULL != trunk_info) {
        sal_memset(trunk_info, 0, sizeof (*trunk_info));
    } 
    return;
}

/*
 * Function:
 *      bcm_trunk_add_info_t_init
 * Purpose:
 *      Initialize the bcm_trunk_add_info_t structure
 * Parameters:
 *      add_info - Pointer to structure which should be initialized
 * Returns:
 *      NONE
 */
void 
bcm_trunk_add_info_t_init(bcm_trunk_add_info_t *add_info)
{
    if (NULL != add_info) {
        sal_memset(add_info, 0, sizeof (*add_info));
    } 
    return;
}

/*
 * Function:
 *      bcm_trunk_chip_info_t_init
 * Purpose:
 *      Initialize the bcm_trunk_chip_info_t structure
 * Parameters:
 *      trunk_chip_info - Pointer to trunk chip info structure
 * Returns:
 *      NONE
 */
void
bcm_trunk_chip_info_t_init(bcm_trunk_chip_info_t *trunk_chip_info)
{
    if (trunk_chip_info != NULL) {
        sal_memset(trunk_chip_info, 0, sizeof (*trunk_chip_info));
    }
    return;
}

/*
 * Function:
 *      bcm_trunk_member_t_init
 * Purpose:
 *      Initialize the bcm_trunk_member_t structure
 * Parameters:
 *      trunk_member - Pointer to structure which should be initialized
 * Returns:
 *      NONE
 */
void 
bcm_trunk_member_t_init(bcm_trunk_member_t *trunk_member)
{
    if (NULL != trunk_member) {
        sal_memset(trunk_member, 0, sizeof (*trunk_member));
    } 
    return;
}

