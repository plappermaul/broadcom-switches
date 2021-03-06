/* 
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 *
 * File:        chipsim_appl.c
 * Purpose:     main() for DPP Chipsim application integrated via bde interface
 */

#include <unistd.h>
#include <stdlib.h>

#include <shared/bsl.h>
#include <soc/drv.h>
#include <appl/diag/system.h>
#if defined(INCLUDE_DUNE_UI) && defined (BCM_PETRAB_SUPPORT)
#include <appl/dpp/UserInterface/ui_pure_defi.h>
#endif

#include <sal/core/boot.h>
#include <sal/appl/config.h>
#include <sal/appl/sal.h>

#include <ibde.h>
#include "chipsim_bde.h"

ibde_t *bde;

int
bde_create(void)
{
    chipsim_bde_bus_t   bus;

    bus.base_addr_start = 0x40000000;
    bus.int_line = 2;
    bus.be_pio = 1;
    bus.be_packet = 0;
    bus.be_other = 1;

    return chipsim_bde_create(&bus, &bde);
}

int
main(int argc, char *argv[])
{
    int     use_sand_sim = 0;
    char    *use_sand_sim_str;
    int len = 0;
    char *config_file = NULL, *config_temp = NULL;

    use_sand_sim_str = getenv("USE_SAND_SIM");
    if (use_sand_sim_str != NULL) {
        use_sand_sim = atoi(use_sand_sim_str);
        if (use_sand_sim) {
            printf("Using SOC_SAND sim \n");
        }
    }

    if ((config_file = getenv("BCM_CONFIG_FILE")) != NULL) {
        len = sal_strlen(config_file);
        if ((config_temp = sal_alloc(len+5, NULL)) != NULL) {
            sal_strncpy(config_temp, config_file, len);
            sal_strncpy(&config_temp[len], ".tmp", 5);
            sal_config_file_set(config_file, config_temp);
            /* coverity[tainted_data] */            
            sal_free(config_temp);
        } else {
            LOG_CLI((BSL_META("sal_alloc failed. \n")));
            exit(1);
        }
    }

    /* initialize SDK */    
    if (sal_core_init() < 0) {
        printf("sal_core_init failed, exiting chipsim application\n");
        return (-1);
    }
    if (sal_appl_init() < 0) {
        printf("sal_appl_init failed, exiting chipsim application\n");
        return (-1);
    }

    if(use_sand_sim) {
    }

#if defined(INCLUDE_DUNE_UI) && defined (BCM_PETRAB_SUPPORT)
    /* Init Dune UI */
    init_ui_module();
#endif

    /* launch BCM shell */
    diag_shell();

    return 0; /* only when diag_shell exits */
}

