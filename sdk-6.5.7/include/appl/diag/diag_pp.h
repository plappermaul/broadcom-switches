/*
 * $Id: diag_pp.h,v 1.2 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * File:        diag_pp.h
 * Purpose:     
 */

#ifndef   _DIAG_PP_H_
#define   _DIAG_PP_H_

#include <appl/diag/shell.h>
#include <appl/diag/parse.h>

/*Structs*/
typedef struct
{
    const char     *long_name;
    const char     *short_name;
    int           (*cmd_diag_pp_function) (int, args_t *); /* define callback for print */

} DIAG_PP_COMMAND_DEF;

/* Functions */
cmd_result_t
cmd_dpp_diag_pp(int unit, args_t* a); 

void
print_pp_usage(int unit);

#endif /* _DIAG_PP_H_ */

