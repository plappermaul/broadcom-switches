/* 
 * $Id: counter.h,v 1.3 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * File:        phymod_sdk_diag.h
 * Purpose:     Extern declarations for phymod diagnostics support.
 */

#ifndef   _PHYMOD_SDK_DIAG_H_
#define   _PHYMOD_SDK_DIAG_H_

#include <phymod/phymod.h>
#include <phymod/phymod_diag.h>

typedef int (*phymod_diag_map_func_f)(int unit, int port, int array_size, phymod_phy_access_t *access_array, int *actual_size);
cmd_result_t phymod_sdk_diag(int unit, phymod_diag_map_func_f port_map ,args_t *args);

#endif /* _PHYMOD_SDK_DIAG_H_ */
