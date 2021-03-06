/*
 * This software is governed by the Broadcom Switch APIs license.
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenMDK/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 *
 * XGS core commands
 */

#include <cdk/cdk_shell.h>
#include <cdk/cdk_device.h>

#include <cdk/arch/shcmd_xgs_get.h>
#include <cdk/arch/shcmd_xgs_set.h>
#include <cdk/arch/shcmd_xgs_geti.h>
#include <cdk/arch/shcmd_xgs_seti.h>
#include <cdk/arch/shcmd_xgs_list.h>
#include <cdk/arch/shcmd_xgs_unit.h>
#include <cdk/arch/shcmd_xgs_pid.h>
#include <cdk/arch/xgs_cmds.h>

#if CDK_CONFIG_SHELL_INCLUDE_GET == 1
static cdk_shell_command_t shcmd_get = {
    "get",
    cdk_shcmd_xgs_get,
    CDK_SHCMD_XGS_GET_DESC,
    CDK_SHCMD_XGS_GET_SYNOP,
#if CDK_CONFIG_SHELL_INCLUDE_HELP == 1
    { CDK_SHCMD_XGS_GET_HELP, CDK_SHCMD_XGS_GET_HELP_2 }
#endif
};
#endif /* CDK_CONFIG_SHELL_INCLUDE_GET */

#if CDK_CONFIG_SHELL_INCLUDE_SET == 1
static cdk_shell_command_t shcmd_set = {
    "set",
    cdk_shcmd_xgs_set,
    CDK_SHCMD_XGS_SET_DESC,
    CDK_SHCMD_XGS_SET_SYNOP,
#if CDK_CONFIG_SHELL_INCLUDE_HELP == 1
    { CDK_SHCMD_XGS_SET_HELP }
#endif
};
#endif /* CDK_CONFIG_SHELL_INCLUDE_SET */

#if CDK_CONFIG_SHELL_INCLUDE_GETI == 1
static cdk_shell_command_t shcmd_geti = {
    "geti",
    cdk_shcmd_xgs_geti,
    CDK_SHCMD_XGS_GETI_DESC,
    CDK_SHCMD_XGS_GETI_SYNOP,
#if CDK_CONFIG_SHELL_INCLUDE_HELP == 1
    { CDK_SHCMD_XGS_GETI_HELP }
#endif
};
#endif /* CDK_CONFIG_SHELL_INCLUDE_GETI */

#if CDK_CONFIG_SHELL_INCLUDE_SETI == 1
static cdk_shell_command_t shcmd_seti = {
    "seti",
    cdk_shcmd_xgs_seti,
    CDK_SHCMD_XGS_SETI_DESC,
    CDK_SHCMD_XGS_SETI_SYNOP,
#if CDK_CONFIG_SHELL_INCLUDE_HELP == 1
    { CDK_SHCMD_XGS_SETI_HELP }
#endif
};
#endif /* CDK_CONFIG_SHELL_INCLUDE_SETI */

#if CDK_CONFIG_SHELL_INCLUDE_LIST == 1
static cdk_shell_command_t shcmd_list = {
    "list",
    cdk_shcmd_xgs_list,
    CDK_SHCMD_XGS_LIST_DESC,
    CDK_SHCMD_XGS_LIST_SYNOP,
#if CDK_CONFIG_SHELL_INCLUDE_HELP == 1
    { CDK_SHCMD_XGS_LIST_HELP }
#endif
};
#endif /* CDK_CONFIG_SHELL_INCLUDE_LIST */

#if CDK_CONFIG_SHELL_INCLUDE_UNIT == 1
static cdk_shell_command_t shcmd_unit = {
    "unit",
    cdk_shcmd_xgs_unit,
    CDK_SHCMD_XGS_UNIT_DESC,
    CDK_SHCMD_XGS_UNIT_SYNOP,
#if CDK_CONFIG_SHELL_INCLUDE_HELP == 1
    { CDK_SHCMD_XGS_UNIT_HELP }
#endif
};
#endif /* CDK_CONFIG_SHELL_INCLUDE_UNIT */

#if CDK_CONFIG_SHELL_INCLUDE_PID == 1
static cdk_shell_command_t shcmd_pid = {
    "pid",
    cdk_shcmd_xgs_pid,
    CDK_SHCMD_XGS_PID_DESC,
    CDK_SHCMD_XGS_PID_SYNOP,
#if CDK_CONFIG_SHELL_INCLUDE_HELP == 1
    { CDK_SHCMD_XGS_PID_HELP }
#endif
};
#endif /* CDK_CONFIG_SHELL_INCLUDE_PID */

void
cdk_shell_add_xgs_core_cmds(void)
{
#if CDK_CONFIG_SHELL_INCLUDE_GET == 1
    cdk_shell_add_command(&shcmd_get, CDK_DEV_ARCH_XGS);
#endif
#if CDK_CONFIG_SHELL_INCLUDE_SET == 1
    cdk_shell_add_command(&shcmd_set, CDK_DEV_ARCH_XGS);
#endif
#if CDK_CONFIG_SHELL_INCLUDE_GETI == 1
    cdk_shell_add_command(&shcmd_geti, CDK_DEV_ARCH_XGS);
#endif
#if CDK_CONFIG_SHELL_INCLUDE_SETI == 1
    cdk_shell_add_command(&shcmd_seti, CDK_DEV_ARCH_XGS);
#endif
#if CDK_CONFIG_SHELL_INCLUDE_LIST == 1
    cdk_shell_add_command(&shcmd_list, CDK_DEV_ARCH_XGS);
#endif
#if CDK_CONFIG_SHELL_INCLUDE_UNIT == 1
    cdk_shell_add_command(&shcmd_unit, CDK_DEV_ARCH_XGS);
#endif
#if CDK_CONFIG_SHELL_INCLUDE_PID == 1
    cdk_shell_add_command(&shcmd_pid, CDK_DEV_ARCH_XGS);
#endif
}
