/*
 * 
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2022 Broadcom Inc. All rights reserved.
 *
 * IFA - In-band Flow Analyzer Monitoring Embedded Application APP interface
 * Purpose: API to Initialize IFA embedded app Structures.
 *
 */
#include <shared/bslenum.h>
#include <shared/bsl.h>

#include <soc/defs.h>
#include <soc/drv.h>

#include <bcm/error.h>
#include <bcm/ifa.h>

/* Initialize a ifa collector information structure. */
void bcm_ifa_collector_info_t_init(bcm_ifa_collector_info_t *collector_info)
{
    sal_memset(collector_info, 0, sizeof(bcm_ifa_collector_info_t));
}

/* Initialize a ifa config information structure. */
void bcm_ifa_config_info_t_init(bcm_ifa_config_info_t *config_info)
{
    sal_memset(config_info, 0, sizeof(bcm_ifa_config_info_t));
}

/* Initialize a ifa cc config information structure. */
void bcm_ifa_cc_config_t_init(bcm_ifa_cc_config_t *config_info)
{
    sal_memset(config_info, 0, sizeof(bcm_ifa_cc_config_t));
}

/* Initialize a ifa leap config information structure. */
void bcm_ifa_template_transmit_config_t_init(bcm_ifa_template_transmit_config_t *config_info)
{
    sal_memset(config_info, 0, sizeof(bcm_ifa_template_transmit_config_t));
}

/* Initialize the bcm_ifa_header_t struct. */
void bcm_ifa_header_t_init(bcm_ifa_header_t *header)
{
    sal_memset(header, 0, sizeof(bcm_ifa_header_t));
}

/* Initialize ifa control information structure. */
void bcm_ifa_control_info_t_init(bcm_ifa_control_info_t *info)
{
    sal_memset(info, 0, sizeof(bcm_ifa_control_info_t));
}

/* Initialize metadata field entry structure. */
void bcm_ifa_metadata_field_entry_t_init(bcm_ifa_metadata_field_entry_t *field_entry)
{
    sal_memset(field_entry, 0, sizeof(bcm_ifa_metadata_field_entry_t));
}

/* Initialize metadata field information structure. */
void bcm_ifa_metadata_field_info_t_init(bcm_ifa_metadata_field_info_t *field_info)
{
    sal_memset(field_info, 0, sizeof(bcm_ifa_metadata_field_info_t));
}

/* Initialize opaque data profile information structure. */
void bcm_ifa_opaque_data_profile_t_init(bcm_ifa_opaque_data_profile_t *profile)
{
    sal_memset(profile, 0, sizeof(bcm_ifa_opaque_data_profile_t));
}

/* Initialize action profile information structure. */
void bcm_ifa_action_profile_t_init(bcm_ifa_action_profile_t *profile)
{
    sal_memset(profile, 0, sizeof(bcm_ifa_action_profile_t));
}
