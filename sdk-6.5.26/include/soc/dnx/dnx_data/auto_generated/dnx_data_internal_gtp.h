
/* *INDENT-OFF* */
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2022 Broadcom Inc. All rights reserved.
 */
#ifndef _DNX_DATA_INTERNAL_GTP_H_

#define _DNX_DATA_INTERNAL_GTP_H_

#include <soc/dnx/dnx_data/auto_generated/dnx_data_internal.h>
#include <soc/dnx/dnx_data/auto_generated/dnx_data_gtp.h>



#ifndef BCM_DNX_SUPPORT
#error "This file is for use by DNX family only!"
#endif



typedef enum
{
    dnx_data_gtp_submodule_general,

    
    _dnx_data_gtp_submodule_nof
} dnx_data_gtp_submodule_e;








int dnx_data_gtp_general_feature_get(
    int unit,
    dnx_data_gtp_general_feature_e feature);



typedef enum
{
    dnx_data_gtp_general_define_gtp_term_is_lif,

    
    _dnx_data_gtp_general_define_nof
} dnx_data_gtp_general_define_e;



uint32 dnx_data_gtp_general_gtp_term_is_lif_get(
    int unit);



typedef enum
{

    
    _dnx_data_gtp_general_table_nof
} dnx_data_gtp_general_table_e;






shr_error_e dnx_data_gtp_init(
    int unit,
    dnxc_data_module_t *module_data);



#endif 

