/*
 * $Id: cosq.c,v 1.1 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * File:    cosq.c
 * Purpose: Manages common COSQ functions
 */

#include <sal/core/libc.h>

#include <soc/drv.h>
#include <soc/debug.h>

#include <bcm/port.h>
#include <bcm/cosq.h>

/*
 * Function:
 *      bcm_cosq_gport_discard_t_init
 * Purpose:
 *      Initialize the COSQ gport discard struct
 * Parameters:
 *      discard - Pointer to the struct to be init'ed
 */

void
bcm_cosq_gport_discard_t_init(bcm_cosq_gport_discard_t *discard)
{
    if (discard != NULL) {
        sal_memset(discard, 0, sizeof(*discard));
        discard->refresh_time = 8;
    }
}

/*
 * Function:
 *      bcm_cosq_congestion_mapping_info_t_init
 * Purpose:
 *      Initialize the COSQ congestion mapping struct
 * Parameters:
 *      config - Pointer to the struct to be init'ed
 */
void
bcm_cosq_congestion_mapping_info_t_init(bcm_cosq_congestion_mapping_info_t *config)
{
    if (config != NULL) {
        sal_memset(config, 0, sizeof(*config));
    }
}

/*
 * Function:
 *      bcm_cosq_classifier_t_init
 * Purpose:
 *      Initialize the COSQ classifier struct
 * Parameters:
 *      classifier - Pointer to the struct to be init'ed
 */

void
bcm_cosq_classifier_t_init(bcm_cosq_classifier_t *classifier)
{
    if (classifier != NULL) {
        sal_memset(classifier, 0, sizeof(*classifier));
    }
}

/*
 * Function:
 *      bcm_cosq_congestion_info_t_init
 * Purpose:
 *      Initialize the COSQ congestion struct
 * Parameters:
 *      config - Pointer to the struct to be init'ed
 */

void
bcm_cosq_congestion_info_t_init(bcm_cosq_congestion_info_t *config)
{
    if (config != NULL) {
        sal_memset(config, 0, sizeof(*config));

        /* Setting the default values to -1 since the validation
         * is done for -1 in all the related APIs.
         */
        config->fabric_modid = -1;
        config->fabric_port = -1;
        config->dest_modid = -1;
        config->dest_port = -1;
    }
}

/*
 * Function:
 *      bcm_cosq_bst_profile_t_init
 * Purpose:
 *      Initialize a CoSQ BST profile structure.
 * Parameters:
 *      profile - Pointer to the struct to be init'ed
 */

void
bcm_cosq_bst_profile_t_init(bcm_cosq_bst_profile_t *profile)
{
    if (profile != NULL) {
        sal_memset(profile, 0, sizeof(*profile));
    }
}
void 
bcm_cosq_threshold_t_init(bcm_cosq_threshold_t *threshold) {
    if (threshold != NULL) {
        sal_memset(threshold, 0, sizeof(*threshold));
        threshold->valid = BCM_COSQ_THRESHOLD_VALID_DP | BCM_COSQ_THRESHOLD_VALID_VALUE;
    }
}

/*
 * Function:
 *      bcm_cosq_pfc_class_mapping_t_init
 * Purpose:
 *      Initialize a PFC class mapping structure.
 * Parameters:
 *      mapping - Pointer to the struct to be init'ed
 */
void
bcm_cosq_pfc_class_mapping_t_init(bcm_cosq_pfc_class_mapping_t *mapping)
{
    int index = 0;

    if (mapping == NULL) {
        return;
    }
    
    sal_memset(mapping, 0, sizeof(*mapping));
    for (index = 0; index < BCM_COSQ_PFC_GPORT_COUNT; index++) {
        mapping->gport_list[index] = BCM_GPORT_INVALID;
    }
}

/*
 * Function:
 *      bcm_cosq_safc_class_mapping_t_init
 * Purpose:
 *      Initialize a SAFC class mapping structure.
 * Parameters:
 *      mapping - Pointer to the struct to be init'ed
 */
void
bcm_cosq_safc_class_mapping_t_init(bcm_cosq_safc_class_mapping_t *mapping)
{
    int index = 0;

    if (mapping == NULL) {
        return;
    }

    sal_memset(mapping, 0, sizeof(*mapping));
    for (index = 0; index < BCM_COSQ_SAFC_GPORT_COUNT; index++) {
        mapping->gport_list[index] = BCM_GPORT_INVALID;
    }
}

/*
 * Function:
 *      bcm_cosq_service_pool_t_init
 * Purpose:
 *      Initialize a COSQ service pool structure.
 * Parameters:
 *      service_pool - Pointer to the struct to be init'ed
 */

void
bcm_cosq_service_pool_t_init(bcm_cosq_service_pool_t *service_pool)
{
    if (service_pool != NULL) {
        sal_memset(service_pool, 0, sizeof(*service_pool));
    }
}

/*
 * Function:
 *      bcm_cosq_pfc_deadlock_config_t_init
 * Purpose:
 *      Initialize a COSq PFC deadlock config structure.
 * Parameters:
 *      config - Pointer to the struct to be init'ed
 */
void bcm_cosq_pfc_deadlock_config_t_init(
    bcm_cosq_pfc_deadlock_config_t *config)
{
    if (config != NULL) {
        sal_memset(config, 0, sizeof(bcm_cosq_pfc_deadlock_config_t));
    }
}

/*
 * Function:
 *      bcm_cosq_service_pool_t_init
 * Purpose:
 *      Initialize a COSq PFC deadlock queue config structure.
 * Parameters:
 *      queue_config - Pointer to the struct to be init'ed
 */
void bcm_cosq_pfc_deadlock_queue_config_t_init(
    bcm_cosq_pfc_deadlock_queue_config_t *q_config)
{
    if (q_config != NULL) {
        sal_memset(q_config, 0, sizeof(bcm_cosq_pfc_deadlock_queue_config_t));
    }
}

/*
 * Function:
 *      bcm_cosq_service_pool_t_init
 * Purpose:
 *      Initialize a COSq PFC deadlock info structure.
 * Parameters:
 *      info - Pointer to the struct to be init'ed
 */
void bcm_cosq_pfc_deadlock_info_t_init(
    bcm_cosq_pfc_deadlock_info_t *pfc_deadlock_info)
{
    if (pfc_deadlock_info != NULL) {
        sal_memset(pfc_deadlock_info, 0, sizeof(bcm_cosq_pfc_deadlock_info_t));
    }
}

/*
 * Function:
 *      bcm_cosq_tas_profile_t_init
 * Purpose:
 *      Initialize a COSq TAS profile info structure.
 * Parameters:
 *      profile - Pointer to the struct to be init'ed
 */
void bcm_cosq_tas_profile_t_init(
    bcm_cosq_tas_profile_t *profile)
{
    if (profile != NULL) {
        sal_memset(profile, 0, sizeof(bcm_cosq_tas_profile_t));
    }
}

/*
 * Function:
 *      bcm_cosq_tas_profile_status_t_init
 * Purpose:
 *      Initialize a COSq TAS profile status structure.
 * Parameters:
 *      profile - Pointer to the struct to be init'ed
 */
void bcm_cosq_tas_profile_status_t_init(
    bcm_cosq_tas_profile_status_t *profile_status)
{
     if (profile_status != NULL) {
        sal_memset(profile_status, 0, sizeof(bcm_cosq_tas_profile_status_t));
    }
}
#if defined (INCLUDE_TCB)
/*
 * Function:
 *      bcm_cosq_tcb_config_t_init
 * Purpose:
 *      Initialize a tcb config info structure.
 * Parameters:
 *      info - Pointer to the struct to be init'ed
 */
void bcm_cosq_tcb_config_t_init(
    bcm_cosq_tcb_config_t *config_info)
{
    if (config_info!= NULL) {
        sal_memset(config_info, 0, sizeof(bcm_cosq_tcb_config_t));
    }
}
#endif

#if defined (INCLUDE_TCB)
/*
 * Function:
 *      bcm_cosq_tcb_threshold_profile_t_init
 * Purpose:
 *      Initialize a tcb config threshold profile structure.
 * Parameters:
 *      threshold - Pointer to the struct to be init'ed
 */
void bcm_cosq_tcb_threshold_profile_t_init(
    bcm_cosq_tcb_threshold_profile_t *threshold)
{
    if (threshold != NULL) {
        sal_memset(threshold, 0, sizeof(bcm_cosq_tcb_threshold_profile_t));
    }
}
#endif

/*
 * Function:
 *      bcm_cosq_object_id_t_init
 * Purpose:
 *      Initializes the bcm_cosq_object_id_t structure
 * Parameters:
 *      id - Pointer to the struct to be init'ed
 */
void
bcm_cosq_object_id_t_init(bcm_cosq_object_id_t *id)
{
    if (id != NULL) {
        sal_memset(id, 0, sizeof(bcm_cosq_object_id_t));
        id->port = BCM_GPORT_INVALID;
        id->cosq = BCM_COS_INVALID;
        id->buffer = BCM_COSQ_BUFFER_ID_INVALID;
    }
}

/*
 * Function:
 *      bcm_cosq_control_data_t_init
 * Purpose:
 *      Initializes the bcm_cosq_control_data_t structure
 * Parameters:
 *      id - Pointer to the struct to be init'ed
 */
void
bcm_cosq_control_data_t_init(bcm_cosq_control_data_t *control)
{
    if (control != NULL) {
        sal_memset(control, 0, sizeof(bcm_cosq_control_data_t));
    }
}

/*
 * Function:
 *      bcm_cosq_ingress_drop_control_frame_config_t_init
 * Purpose:
 *      Initialize a bcm_cosq_ingress_drop_control_frame_config_t structure.
 * Parameters:
 *      config - Pointer to the struct to be init'ed
 */
void bcm_cosq_ingress_drop_control_frame_config_t_init(
    bcm_cosq_ingress_drop_control_frame_config_t *config)
{
    if (config != NULL) {
        sal_memset(config, 0, sizeof(bcm_cosq_ingress_drop_control_frame_config_t));
    }
}

/*
 * Function:
 *      bcm_cosq_ingress_drop_flex_key_construct_id_t_init
 * Purpose:
 *      Initialize a bcm_cosq_ingress_drop_flex_key_construct_id_t structure.
 * Parameters:
 *      config - Pointer to the struct to be init'ed
 */
void bcm_cosq_ingress_drop_flex_key_construct_id_t_init(
    bcm_cosq_ingress_drop_flex_key_construct_id_t *config)
{
    if (config != NULL) {
        sal_memset(config, 0, sizeof(bcm_cosq_ingress_drop_flex_key_construct_id_t));
    }
}

/*
 * Function:
 *      bcm_cosq_ingress_drop_flex_key_construct_t_init
 * Purpose:
 *      Initialize a bcm_cosq_ingress_drop_flex_key_construct_t structure.
 * Parameters:
 *      config - Pointer to the struct to be init'ed
 */
void bcm_cosq_ingress_drop_flex_key_construct_t_init(
    bcm_cosq_ingress_drop_flex_key_construct_t *config)
{
    if (config != NULL) {
        sal_memset(config, 0, sizeof(bcm_cosq_ingress_drop_flex_key_construct_t));
    }
}

/*
 * Function:
 *      bcm_cosq_ingress_drop_flex_key_t_init
 * Purpose:
 *      Initialize a bcm_cosq_ingress_drop_flex_key_t structure.
 * Parameters:
 *      config - Pointer to the struct to be init'ed
 */
void bcm_cosq_ingress_drop_flex_key_t_init(
    bcm_cosq_ingress_drop_flex_key_t *config)
{
    if (config != NULL) {
        sal_memset(config, 0, sizeof(bcm_cosq_ingress_drop_flex_key_t));
    }
}


/*
 * Function:
 *      bcm_cosq_ingress_drop_flex_key_entry_t_init
 * Purpose:
 *      Initialize a bcm_cosq_ingress_drop_flex_key_entry_t structure.
 * Parameters:
 *      config - Pointer to the struct to be init'ed
 */
void bcm_cosq_ingress_drop_flex_key_entry_t_init(
    bcm_cosq_ingress_drop_flex_key_entry_t *config)
{
    if (config != NULL) {
        sal_memset(config, 0, sizeof(bcm_cosq_ingress_drop_flex_key_entry_t));
    }
}

/*
 * Function:
 *      bcm_cosq_fadt_info_t_init
 * Purpose:
 *      Initialize a bcm_cosq_fadt_info_t structure.
 * Parameters:
 *      info - Pointer to the struct to be init'ed
 */
void bcm_cosq_fadt_info_t_init(
    bcm_cosq_fadt_info_t* info)
{
    if (info != NULL) {
        sal_memset(info, 0, sizeof(bcm_cosq_fadt_info_t));
    }
}

