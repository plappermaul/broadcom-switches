/** \file diag_dnx_field_entry.h
 *
 * Diagnostics procedures, for DNX, for 'entry' operations.
 */
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

#ifndef BCM_DNX_SUPPORT
#error "This file is for use by DNX (JR2) family only!"
#endif

#ifndef DIAG_DNX_ENTRY_H_INCLUDED
/*
 * {
 */
#define DIAG_DNX_ENTRY_H_INCLUDED
/*
 * Include files.
 * {
 */
/** appl */
#include <appl/diag/sand/diag_sand_framework.h>
/*
 * }
 */
/*
 * DEFINEs
 * {
 */
/*
 * }
 */
/**
 * \brief
 *   List of shell options for 'entry' shell commands (list, info)
 * \remark
 *   * Display entry information.
 */
extern sh_sand_cmd_t Sh_dnx_field_entry_cmds[];

#endif /* DIAG_DNX_ENTRY_H_INCLUDED */
