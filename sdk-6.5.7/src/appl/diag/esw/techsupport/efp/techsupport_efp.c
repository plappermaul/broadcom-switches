/*
 * $Id: techsupport_efp.c  Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * File:    techsupport_efp.c
 * Purpose: This file implements techsupport utility for
 *          efp.
*/

#include <appl/diag/techsupport.h>

/* "efp" feature's diag command list for all devices*/
char * techsupport_efp_diag_cmdlist[] = {
    "fp show",
    NULL /* Must be the last element in this structure */
};

#ifndef BCM_SW_STATE_DUMP_DISABLE
/* "efp" feature's software dump command list for all devices */
char * techsupport_efp_sw_dump_cmdlist[] = {
    "dump sw efp",
    NULL /* Must be the last element in this structure */
};
#endif /* BCM_SW_STATE_DUMP_DISABLE */


/* Function:   techsupport_efp
 * Purpose :   Implements techsupport utility for efp feature.
 * Parameters: unit - Unit number
 *             a - pointer to argument.
 *             techsupport_data_t -structure that maintains per chip per feature debug info.
 * Returns:    CMD_OK :done
 *             CMD_FAIL : INVALID INPUT
 * Notes:      This function is a place holder to implement feature specific objects.
 */
int techsupport_efp(int unit, args_t *a, techsupport_data_t *techsupport_data)
{
    return techsupport_feature_process(unit, a, techsupport_data);
}
