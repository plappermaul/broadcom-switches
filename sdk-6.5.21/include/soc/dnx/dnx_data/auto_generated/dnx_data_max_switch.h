

/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */
#ifndef _DNX_DATA_MAX_SWITCH_H_

#define _DNX_DATA_MAX_SWITCH_H_




#ifndef BCM_DNX_SUPPORT
#error "This file is for use by DNX (JR2) family only!"
#endif



#define DNX_DATA_MAX_SWITCH_LOAD_BALANCING_NOF_LB_CLIENTS (5)


#define DNX_DATA_MAX_SWITCH_LOAD_BALANCING_NOF_LB_CRC_SEL_TCAM_ENTRIES (32)


#define DNX_DATA_MAX_SWITCH_LOAD_BALANCING_NOF_CRC_FUNCTIONS (8)


#define DNX_DATA_MAX_SWITCH_LOAD_BALANCING_INITIAL_RESERVED_LABEL (0)


#define DNX_DATA_MAX_SWITCH_LOAD_BALANCING_INITIAL_RESERVED_LABEL_FORCE (0)


#define DNX_DATA_MAX_SWITCH_LOAD_BALANCING_INITIAL_RESERVED_NEXT_LABEL (65535)


#define DNX_DATA_MAX_SWITCH_LOAD_BALANCING_INITIAL_RESERVED_NEXT_LABEL_VALID (65535)


#define DNX_DATA_MAX_SWITCH_LOAD_BALANCING_RESERVED_NEXT_LABEL_VALID (1)


#define DNX_DATA_MAX_SWITCH_LOAD_BALANCING_NUM_VALID_MPLS_PROTOCOLS (3)


#define DNX_DATA_MAX_SWITCH_LOAD_BALANCING_NOF_LAYER_RECORDS_FROM_PARSER (8)


#define DNX_DATA_MAX_SWITCH_LOAD_BALANCING_NOF_SEEDS_PER_CRC_FUNCTION (65536)


#define DNX_DATA_MAX_SWITCH_LOAD_BALANCING_HASHING_SELECTION_PER_LAYER (1)


#define DNX_DATA_MAX_SWITCH_LOAD_BALANCING_MPLS_SPLIT_STACK_CONFIG (1)


#define DNX_DATA_MAX_SWITCH_LOAD_BALANCING_MPLS_CAM_NEXT_LABEL_VALID_FIELD_EXISTS (1)


#define DNX_DATA_MAX_SWITCH_SVTAG_SUPPORTED (1)


#define DNX_DATA_MAX_SWITCH_SVTAG_SVTAG_LABEL_SIZE_BITS (32)


#define DNX_DATA_MAX_SWITCH_SVTAG_EGRESS_SVTAG_OFFSET_ADDR_SIZE_BITS (8)


#define DNX_DATA_MAX_SWITCH_SVTAG_EGRESS_SVTAG_OFFSET_ADDR_POSITION_BITS (16)


#define DNX_DATA_MAX_SWITCH_SVTAG_EGRESS_SVTAG_SIGNATURE_SIZE_BITS (8)


#define DNX_DATA_MAX_SWITCH_SVTAG_EGRESS_SVTAG_SIGNATURE_POSITION_BITS (24)


#define DNX_DATA_MAX_SWITCH_SVTAG_EGRESS_SVTAG_SCI_SIZE_BITS (10)


#define DNX_DATA_MAX_SWITCH_SVTAG_EGRESS_SVTAG_SCI_POSITION_BITS (0)


#define DNX_DATA_MAX_SWITCH_SVTAG_EGRESS_SVTAG_PKT_TYPE_SIZE_BITS (2)


#define DNX_DATA_MAX_SWITCH_SVTAG_EGRESS_SVTAG_PKT_TYPE_POSITION_BITS (10)


#define DNX_DATA_MAX_SWITCH_SVTAG_EGRESS_SVTAG_IPV6_INDICATION_POSITION_BITS (15)


#define DNX_DATA_MAX_SWITCH_SVTAG_EGRESS_SVTAG_HW_FIELD_POSITION_BITS (0)


#define DNX_DATA_MAX_SWITCH_SVTAG_EGRESS_SVTAG_ACCUMULATION_INDICATION_HW_FIELD_POSITION_BITS (32)


#define DNX_DATA_MAX_SWITCH_SVTAG_UDP_DEDICATED_PORT (0)


#define DNX_DATA_MAX_SWITCH_SVTAG_INGRESS_SVTAG_POSITION_BYTES (12)




#endif 

