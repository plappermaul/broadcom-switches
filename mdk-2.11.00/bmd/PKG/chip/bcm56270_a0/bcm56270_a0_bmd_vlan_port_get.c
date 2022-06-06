#include <bmd_config.h>
#if CDK_CONFIG_INCLUDE_BCM56270_A0 == 1

/*
 * 
 * This software is governed by the Broadcom Switch APIs license.
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenMDK/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

#include <bmd/bmd.h>

#include <cdk/chip/bcm56270_a0_defs.h>
#include <cdk/cdk_debug.h>

#include "bcm56270_a0_bmd.h"
#include "bcm56270_a0_internal.h"

int
bcm56270_a0_bmd_vlan_port_get(int unit, int vlan, int *plist, int *utlist)
{
    int ioerr = 0;
    int ppport;
    VLAN_TABm_t vlan_tab;
    EGR_VLANm_t egr_vlan;
    uint32_t pbmp;
    uint32_t utbmp;
    uint32_t mask;
    int port, pdx, udx;

    BMD_CHECK_UNIT(unit);
    BMD_CHECK_VLAN(unit, vlan);

    ioerr += READ_VLAN_TABm(unit, vlan, &vlan_tab);
    if (VLAN_TABm_VALIDf_GET(vlan_tab) == 0) {
        return ioerr ? CDK_E_IO : CDK_E_NOT_FOUND;
    }

    ioerr += READ_EGR_VLANm(unit, vlan, &egr_vlan);
    pbmp = VLAN_TABm_PORT_BITMAP_W0f_GET(vlan_tab);
    utbmp = EGR_VLANm_UT_PORT_BITMAP_W0f_GET(egr_vlan);
    mask = 1;
    pdx = 0;
    udx = 0;
    /* Loop over first ppport bitmask */
    for (ppport = 0; ppport < 32 && ppport < BMD_CONFIG_MAX_PORTS; ppport++) {
        port = PP2P(unit, ppport);
        if (mask & pbmp) {
            plist[pdx++] = port;
        }
        if (mask & utbmp) {
            utlist[udx++] = port;
        }
        mask <<= 1;
    }
    
    /* Terminate lists */
    plist[pdx] = -1;
    utlist[udx] = -1;

    return ioerr ? CDK_E_IO : CDK_E_NONE;
}

#endif /* CDK_CONFIG_INCLUDE_BCM56270_A0 */