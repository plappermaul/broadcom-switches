/*! \file bcma_cintcmd.h
 *
 * CINT commands.
 */
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

#ifndef   BCMA_CINTCMD_H
#define   BCMA_CINTCMD_H

#include <bcma/cli/bcma_cli.h>

/*!
 * \brief Add default set of CLI commands for the CINT API.
 *
 * \param[in] cli CLI object.
 *
 * \return Always 0.
 */
extern int
bcma_cintcmd_add_cmds(bcma_cli_t *cli);

#endif /* BCMA_CINTCMD_H */
