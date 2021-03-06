/*! \file bcma_clicmd_expr.h
 *
 * CLI 'expr' command.
 */
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

#ifndef BCMA_CLICMD_EXPR_H
#define BCMA_CLICMD_EXPR_H

#include <bcma/cli/bcma_cli.h>

/*! Brief description for CLI command. */
#define BCMA_CLICMD_EXPR_DESC \
    "Evaluate infix expression."

/*! Syntax for CLI command. */
#define BCMA_CLICMD_EXPR_SYNOP \
    "<c-style arithmetic expression>"

/*! Help for CLI command. */
#define BCMA_CLICMD_EXPR_HELP \
    "Evaluate an expression and store the result in local variable $EXPR.\n" \
    "The result will also be used as the function return value, but this\n" \
    "value will be truncated to an int type, even if the function uses\n" \
    "64-bit arithmetic internally."

/*!
 * \brief CLI 'expr' command implementation.
 *
 * \param [in] cli CLI object
 * \param [in] args Argument list
 *
 * \return BCMA_CLI_CMD_xxx return values.
 */
extern int
bcma_clicmd_expr(bcma_cli_t *cli, bcma_cli_args_t *args);

#endif /* BCMA_CLICMD_EXPR_H */
