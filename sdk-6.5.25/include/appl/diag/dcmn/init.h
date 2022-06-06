/* 
 * 
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2021 Broadcom Inc. All rights reserved.
 *
 * File:        init.h
 * Purpose:     Extern declarations for DPP init routines.
 */

#ifndef _DIAG_DCMN_INIT_H
#define _DIAG_DCMN_INIT_H

#include <appl/diag/dcmn/init_deinit.h>

#ifdef BCM_DFE_SUPPORT
uint32
    fe3200_card_power_monitor(
        int eq_sel
    );
uint32 fe3200_card_board_synt_set(uint32 targetfreq, uint32 nominalfreq);
#endif

int appl_dpp_bcm_diag_init(int unit, appl_dcmn_init_param_t* init_param);
int appl_dpp_stk_diag_init(int unit);
int appl_dpp_cosq_diag_init(int unit);

#endif /* _DIAG_DCMN_INIT_H */
