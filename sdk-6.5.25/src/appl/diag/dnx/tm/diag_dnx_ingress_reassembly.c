/** \file diag_dnx_ingress_reassembly.c
 * 
 * diagnostics for ingress reassmbly
 * 
 */
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2021 Broadcom Inc. All rights reserved.
 */
#ifdef BSL_LOG_MODULE
#error "BSL_LOG_MODULE redefined"
#endif
#define BSL_LOG_MODULE BSL_LS_APPL_PORT

/** appl */
#include <appl/diag/diag.h>
#include <appl/diag/sand/diag_sand_framework.h>
#include <appl/diag/sand/diag_sand_prt.h>
/** bcm */
#include <bcm_int/dnx/algo/port/algo_port_mgmt.h>
/** sal */
#include <sal/appl/sal.h>
#include <bcm_int/dnx/port/port_ingr_reassembly.h>
#include "diag_dnx_ingress_reassembly.h"

sh_sand_option_t sh_dnx_ingress_reassembly_options[] = {
    /*
     * name type desc default ext
     */
    {NULL}
};

sh_sand_man_t sh_dnx_ingress_reassembly_man = {
    .brief = "Print reassembly and port termination table",
    .full = NULL,
    .synopsis = NULL,
    .examples = NULL
};

/**
 * \brief - dump reassembly and port termination context table
 */
shr_error_e
sh_dnx_ingr_reassembly_cmd(
    int unit,
    args_t * args,
    sh_sand_control_t * sand_control)
{
    int priority;

    int core_id = 0;
    uint32 in_tm_port;
    bcm_pbmp_t logical_ports;
    bcm_port_t logical_port;
    uint32 reassembly_context[DNX_DATA_MAX_INGR_REASSEMBLY_PRIORITY_PRIORITIES_NOF];
    uint32 port_termination_context;

    PRT_INIT_VARS;
    SHR_FUNC_INIT_VARS(unit);

    /*
     * Print table header
     */
    PRT_TITLE_SET("Reassembly Context");
    PRT_COLUMN_ADD("Port");
    PRT_COLUMN_ADD("Core");
    PRT_COLUMN_ADD("IN TM Port");
    for (priority = 0; priority < DNX_DATA_MAX_INGR_REASSEMBLY_PRIORITY_PRIORITIES_NOF; priority++)
    {
        PRT_COLUMN_ADD("Reassembly Context - priority %d", priority);
    }
    if (dnx_data_ingr_reassembly.
        context.feature_get(unit, dnx_data_ingr_reassembly_context_port_termination_in_context_table))
    {
        PRT_COLUMN_ADD("Port Termination");
    }

    SHR_IF_ERR_EXIT(dnx_algo_port_logicals_get
                    (unit, BCM_CORE_ALL, DNX_ALGO_PORT_LOGICALS_TYPE_TM_ING, 0, &logical_ports));

    /** iterate all logical ports */
    BCM_PBMP_ITER(logical_ports, logical_port)
    {
        SHR_IF_ERR_EXIT(dnx_port_ingr_reassembly_reassembly_and_port_termination_get_all
                        (unit, logical_port, reassembly_context, &port_termination_context));
        if (reassembly_context[0] != DNX_PORT_INGR_REASSEMBLY_NON_INGRESS_PORT_CONTEXT ||
            reassembly_context[1] != DNX_PORT_INGR_REASSEMBLY_NON_INGRESS_PORT_CONTEXT ||
            port_termination_context != DNX_PORT_INGR_REASSEMBLY_NON_INGRESS_PORT_CONTEXT)
        {
            SHR_IF_ERR_EXIT(dnx_algo_port_in_tm_port_get(unit, logical_port, &core_id, &in_tm_port));

            PRT_ROW_ADD(PRT_ROW_SEP_NONE);      /* PRT_ROW_SEP_UNDERSCORE */
            PRT_CELL_SET("%d", logical_port);
            PRT_CELL_SET("%d", core_id);
            PRT_CELL_SET("%d", in_tm_port);
            for (priority = 0; priority < DNX_DATA_MAX_INGR_REASSEMBLY_PRIORITY_PRIORITIES_NOF; priority++)
            {
                if (reassembly_context[priority] == DNX_PORT_INGR_REASSEMBLY_NON_INGRESS_PORT_CONTEXT)
                {
                    /** priority does not exist for this port type */
                    PRT_CELL_SET("%s", "-");
                }
                else
                {
                    PRT_CELL_SET("%d", reassembly_context[priority]);
                }
            }
            if (dnx_data_ingr_reassembly.
                context.feature_get(unit, dnx_data_ingr_reassembly_context_port_termination_in_context_table))
            {
                PRT_CELL_SET("%d", port_termination_context);
            }
        }
    }
    PRT_COMMITX;

exit:
    PRT_FREE;
    SHR_FUNC_EXIT;
}
