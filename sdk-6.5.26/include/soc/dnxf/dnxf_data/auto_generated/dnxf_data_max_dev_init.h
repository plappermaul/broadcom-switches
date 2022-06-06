
/* *INDENT-OFF* */
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2022 Broadcom Inc. All rights reserved.
 */
#ifndef _DNXF_DATA_MAX_DEV_INIT_H_

#define _DNXF_DATA_MAX_DEV_INIT_H_




#ifndef BCM_DNXF_SUPPORT
#error "This file is for use by DNX family only!"
#endif


#define DNXF_DATA_MAX_DEV_INIT_TIME_INIT_TOTAL_THRESH (1)
#ifdef BCM_DNXF1_SUPPORT

#undef DNXF_DATA_MAX_DEV_INIT_TIME_INIT_TOTAL_THRESH
#define DNXF_DATA_MAX_DEV_INIT_TIME_INIT_TOTAL_THRESH (14560000)

#endif 

#define DNXF_DATA_MAX_DEV_INIT_TIME_APPL_INIT_TOTAL_THRESH (1)
#ifdef BCM_DNXF1_SUPPORT

#undef DNXF_DATA_MAX_DEV_INIT_TIME_APPL_INIT_TOTAL_THRESH
#define DNXF_DATA_MAX_DEV_INIT_TIME_APPL_INIT_TOTAL_THRESH (20000000)

#endif 




#endif 

