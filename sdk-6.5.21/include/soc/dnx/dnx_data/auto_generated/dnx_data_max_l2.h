

/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */
#ifndef _DNX_DATA_MAX_L2_H_

#define _DNX_DATA_MAX_L2_H_




#ifndef BCM_DNX_SUPPORT
#error "This file is for use by DNX (JR2) family only!"
#endif


#define DNX_DATA_MAX_L2_GENERAL_VSI_OFFSET_SHIFT (70)

#define DNX_DATA_MAX_L2_GENERAL_LIF_OFFSET_SHIFT (0)


#define DNX_DATA_MAX_L2_VSI_NOF_VSI_AGING_PROFILES (32)


#define DNX_DATA_MAX_L2_VSI_NOF_EVENT_FORWARDING_PROFILES (4)


#define DNX_DATA_MAX_L2_VSI_NOF_VSI_LEARNING_PROFILES (32)


#define DNX_DATA_MAX_L2_VLAN_DOMAIN_NOF_VLAN_DOMAINS (2048)


#define DNX_DATA_MAX_L2_VLAN_DOMAIN_NOF_BITS_NEXT_LAYER_NETWORK_DOMAIN (11)


#define DNX_DATA_MAX_L2_DMA_FLUSH_NOF_DMA_ENTRIES (64)


#define DNX_DATA_MAX_L2_DMA_FLUSH_DB_NOF_DMA_RULES (16)


#define DNX_DATA_MAX_L2_DMA_FLUSH_DB_NOF_DMA_RULES_PER_TABLE (8)


#define DNX_DATA_MAX_L2_DMA_FLUSH_DB_RULE_SIZE (67)


#define DNX_DATA_MAX_L2_DMA_FLUSH_DB_DATA_SIZE (25)


#define DNX_DATA_MAX_L2_DMA_FLUSH_GROUP_SIZE (4)


#define DNX_DATA_MAX_L2_AGE_AND_FLUSH_MACHINE_MAX_AGE_STATES (8)


#define DNX_DATA_MAX_L2_OLP_LPKGV_SHIFT (26)


#define DNX_DATA_MAX_L2_OLP_LPKGV_MASK  (63)


#define DNX_DATA_MAX_L2_OLP_LPKGV_WITH_OUTLIF (32)


#define DNX_DATA_MAX_L2_OLP_LPKGV_WO_OUTLIF (48)


#define DNX_DATA_MAX_L2_OLP_DESTINATION_OFFSET (31)


#define DNX_DATA_MAX_L2_OLP_OUTLIF_OFFSET (9)


#define DNX_DATA_MAX_L2_OLP_EEI_OFFSET  (7)


#define DNX_DATA_MAX_L2_OLP_FEC_OFFSET  (31)


#define DNX_DATA_MAX_L2_OLP_JR_MODE_ENHANCED_PERFORMANCE_ENABLE (1)




#endif 

