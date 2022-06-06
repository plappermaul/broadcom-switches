/*
 * $Id: diag_field.h,v 1.2 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * File:        diag_field.h
 * Purpose:     
 */

#ifndef   _DIAG_FIELD_H_
#define   _DIAG_FIELD_H_

#include <appl/diag/shell.h>
#include <appl/diag/parse.h>

cmd_result_t
cmd_dpp_diag_field(int unit, args_t* a); 

void
print_field_usage(int unit);



cmd_result_t
cmd_dpp_diag_dbal(int unit, args_t* a); 

void
print_dbal_usage(int unit);

#endif /* _DIAG_FIELD_H_ */

