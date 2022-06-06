/*! \file bcma_clicmd_quit.h
 *
 * CLI 'quit' command.
 */
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

#ifndef BCMA_CLICMD_QUIT_H
#define BCMA_CLICMD_QUIT_H

#include <bcma/cli/bcma_cli.h>

/*! Brief description for CLI command. */
#define BCMA_CLICMD_QUIT_DESC \
    "Quit shell."

/*!
 * \brief CLI command implementation.
 *
 * \param [in] cli CLI object
 * \param [in] args Argument list
 *
 * \return BCMA_CLI_CMD_xxx return values.
 */
extern int
bcma_clicmd_quit(bcma_cli_t *cli, bcma_cli_args_t *args);

#endif /* BCMA_CLICMD_QUIT_H */
