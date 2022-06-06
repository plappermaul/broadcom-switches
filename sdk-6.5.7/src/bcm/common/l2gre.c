/*
 * $Id: l2gre.c,v 1.1 Broadcom SDK $
 *
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 */
/*
 * L2GRE initializers
 */

#if defined(INCLUDE_L3)

#include <sal/core/libc.h>

#include <bcm/l2gre.h>

/* 
 * Function:
 *      bcm_l2gre_port_t_init
 * Purpose:
 *      Initialize L2GRE  port struct
 * Parameters: 
 *      l2gre_port - Pointer to the struct to be init'ed
 */
void
bcm_l2gre_port_t_init(bcm_l2gre_port_t *l2gre_port)
{   
    if (l2gre_port != NULL) {
        sal_memset(l2gre_port, 0, sizeof(*l2gre_port));
    }
    return;
}

/* 
 * Function:
 *      bcm_l2gre_vpn_config_t_init
 * Purpose:
 *      Initialize L2GRE  vpn config struct
 * Parameters: 
 *      l2gre_vpn - Pointer to the struct to be init'ed
 */
void
bcm_l2gre_vpn_config_t_init(bcm_l2gre_vpn_config_t *l2gre_vpn)
{   
    if (l2gre_vpn != NULL) {
        sal_memset(l2gre_vpn, 0, sizeof(*l2gre_vpn));
    }
    return;
}

#else
int _bcm_l2gre_not_empty;
#endif  /* INCLUDE_L3 */

