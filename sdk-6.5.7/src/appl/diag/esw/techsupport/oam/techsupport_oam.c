/*
 * $Id: techsupport_oam.c  Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * File:    techsupport_oam.c
 * Purpose: This file implements techsupport utility for
 *          oam.
*/

#include <appl/diag/techsupport.h>

/* "mmu" feature's diag command list */
char * techsupport_oam_diag_cmdlist[] = {
    "oam group show",
    "oam endpoint show",
    NULL /* Must be the last element in this structure */
};

#ifndef BCM_SW_STATE_DUMP_DISABLE
/* "OAM" feature's software dump command list for all devices */
char * techsupport_oam_sw_dump_cmdlist[] = {
    "dump sw bfd",
    "dump sw oam",
    NULL /* Must be the last element in this structure */
};
#endif /* BCM_SW_STATE_DUMP_DISABLE */

/* Function:   techsupport_mmu
 * Purpose :   Implements techsupport utility for mmu feature.
 * Parameters: unit - Unit number
 *             a - pointer to argument.
 *             techsupport_data_t -structure that maintains per chip per feature debug info.
 * Returns:    CMD_OK :done
 *             CMD_FAIL : INVALID INPUT
 * Notes:      This function is a place holder to implement feature specific objects.
 */
int techsupport_oam(int unit, args_t *a, techsupport_data_t *techsupport_data)
{
    return techsupport_feature_process(unit, a, techsupport_data);
}
