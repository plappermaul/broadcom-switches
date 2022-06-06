/*
 * $Id: xmem.c,v 1.8.74.1.2.3 Broadcom SDK $
 *
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 */

#include <shared/bsl.h>

#include <appl/diag/system.h>
#include <appl/diag/parse.h>
#include <appl/diag/shell.h>

char cmd_xmem_usage[] =
        "xmem r addr: read 4 bytes from address\n"
        "xmem w addr data: write 4 bytes data to address\n";

cmd_result_t
cmd_xmem(int unit, args_t *a)
{
    char          *token;
    unsigned int   addr, data;

    if ((token = ARG_GET(a)) == NULL) {
        return CMD_USAGE;
    }

    if (sal_strcasecmp(token, "r") == 0) {
        if ((token = ARG_GET(a)) == NULL) {
            return CMD_USAGE;
        }
        addr = parse_integer(token);
        cli_out("0x%x: %08x\n", addr, *((unsigned int *)(INT_TO_PTR(addr))));
    } else if (sal_strcasecmp(token, "w") == 0) {
        if ((token = ARG_GET(a)) == NULL) {
            return CMD_USAGE;
        }
        addr = parse_integer(token);
        if ((token = ARG_GET(a)) == NULL) {
            return CMD_USAGE;
        }
        data = parse_integer(token);
        *((unsigned int *)(INT_TO_PTR(addr))) = data;
    } else {
        cli_out("%s", cmd_xmem_usage);
        return CMD_FAIL;
    }
    return CMD_OK;
}


#if 1
int custom_cmd(void)
{
    cmd_t cmd = { "xmem", cmd_xmem, cmd_xmem_usage, cmd_xmem_usage };

    return cmdlist_add(&cmd);
}
#endif
