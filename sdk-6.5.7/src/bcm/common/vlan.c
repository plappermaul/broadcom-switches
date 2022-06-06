/*
 * $Id: vlan.c,v 1.17 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 */

#include <soc/drv.h>
#include <soc/mem.h>
#include <soc/debug.h>

#include <sal/core/boot.h>

#include <bcm/error.h>
#include <bcm/vlan.h>
#include <bcm/stg.h>
#include <bcm/port.h>
#include <bcm/trunk.h>
#include <bcm/debug.h>
#include <bcm/switch.h>
#include <bcm/stat.h>

/*
 * Function:
 *      bcm_vlan_ip_t_init
 * Purpose:
 *      Initialize the vlan_ip_t structure
 * Parameters:
 *      vlan_ip - Pointer to structure which should be initialized
 * Returns:
 *      NONE
*/
void 
bcm_vlan_ip_t_init(bcm_vlan_ip_t *vlan_ip)
{
    if (NULL != vlan_ip) {
        sal_memset(vlan_ip, 0, sizeof (bcm_vlan_ip_t));
    }
    return;
}

/*
 * Function:
 *      bcm_vlan_data_t_init
 * Purpose:
 *      Initialize the bcm_vlan_data_t structure
 * Parameters:
 *      vlan_data - Pointer to vlan data info structure
 * Returns:
 *      NONE
 */
void
bcm_vlan_data_t_init(bcm_vlan_data_t *vlan_data)
{
    if (vlan_data != NULL) {
        sal_memset(vlan_data, 0, sizeof (bcm_vlan_data_t));
    }
    return;
}

/*
 * Function:
 *      bcm_vlan_control_vlan_t_init
 * Purpose:
 *      Initialize the bcm_vlan_control_vlan_t structure
 * Parameters:
 *      data - Pointer to vlan control structure to inititalize
 * Returns:
 *      NONE
 */
void
bcm_vlan_control_vlan_t_init(bcm_vlan_control_vlan_t *data)
{
    if (data != NULL) {
        sal_memset(data, 0, sizeof(bcm_vlan_control_vlan_t));
        data->outer_tpid = 0x8100;
    }
    return;
}

/*
 * Function:
 *      bcm_vlan_block_t_init
 * Purpose:
 *      Initialize the bcm_vlan_block_t structure
 * Parameters:
 *      data - Pointer to vlan block structure to inititalize
 * Returns:
 *      NONE
 */
void
bcm_vlan_block_t_init(bcm_vlan_block_t *data)
{
    if (data != NULL) {
        sal_memset(data, 0, sizeof(bcm_vlan_block_t));
    }
    return;
}


/*
 * Function:
 *      bcm_vlan_action_set_t_init
 * Purpose:
 *      Initialize the bcm_vlan_action_set_t structure
 * Parameters:
 *      data - Pointer to vlan action structure to inititalize
 * Returns:
 *      NONE
 */
void
bcm_vlan_action_set_t_init(bcm_vlan_action_set_t *data)
{
    if (data != NULL) {
        sal_memset(data, 0, sizeof(bcm_vlan_action_set_t));
    }
    return;
}

/*
* Function:
*      bcm_vlan_translate_action_class_t_init
* Purpose:
*      Initialize the bcm_vlan_translate_action_class_t structure
* Parameters:
*      info - Pointer to the structure which should be initialized
* Returns:
*      NONE
*/
void 
bcm_vlan_translate_action_class_t_init(bcm_vlan_translate_action_class_t *action_class)
{
    if (NULL != action_class) {
        sal_memset(action_class, 0, sizeof (*action_class));
    }
    return;
}

/*
 * Function:
 *      bcm_vlan_port_t_init
 * Purpose:
 *      Initialize the bcm_vlan_port_t structure
 * Parameters:
 *      data - Pointer to vlan port structure to inititalize
 * Returns:
 *      NONE
 */
void
bcm_vlan_port_t_init(bcm_vlan_port_t *data)
{
    if (data != NULL) {
        sal_memset(data, 0, sizeof(bcm_vlan_port_t));
        data->outlif_counting_profile = BCM_STAT_LIF_COUNTING_PROFILE_NONE;
        data->inlif_counting_profile = BCM_STAT_LIF_COUNTING_PROFILE_NONE;
    }
    return;
}

/*
* Function:
*      bcm_vlan_port_translation_t_init
* Purpose:
*      Initialize the bcm_vlan_port_translation_t structure
* Parameters:
*      info - Pointer to the structure which should be initialized
* Returns:
*      NONE
*/
void 
bcm_vlan_port_translation_t_init(bcm_vlan_port_translation_t *vlan_port_translation)
{
    if (NULL != vlan_port_translation) {
        sal_memset(vlan_port_translation, 0, sizeof (*vlan_port_translation));
    }
    return;
}

/*
 * Function:
 *      bcm_vlan_gport_info_t_init
 * Purpose:
 *      Initialize the bcm_vlan_gport_info_t structure
 * Parameters:
 *      data - Pointer to vlan gport info structure to inititalize
 * Returns:
 *      NONE
 */
void
bcm_vlan_gport_info_t_init(bcm_vlan_gport_info_t *data)
{
    if (data != NULL) {
        sal_memset(data, 0, sizeof(bcm_vlan_gport_info_t));
    }
    return;
}

#if defined(INCLUDE_L3)
/*
 * Function:
 *      bcm_vlan_vpn_config_t_init
 * Purpose:
 *      Initialize VLAN vpn config struct
 * Parameters:
 *      info - Pointer to the struct to be inited
 * Returns:
 *      NONE
 */
void bcm_vlan_vpn_config_t_init(bcm_vlan_vpn_config_t *info)
{
    if (info != NULL) {
        sal_memset(info, 0, sizeof(*info));
    }

    return;
}
#endif

