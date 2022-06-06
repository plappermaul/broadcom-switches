/*
 * $Id: compat_656.h,v 1.0 2016/10/27
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * RPC Compatibility with sdk-6.5.6 routines
 */

#ifndef _COMPAT_656_H_
#define _COMPAT_656_H_

#ifdef	BCM_RPC_SUPPORT

#include <bcm/types.h>
#include <bcm/field.h>

#define BCM_COMPAT656_FIELD_QUALIFY_CNT     (620)

typedef struct bcm_compat656_field_qset_s {
    SHR_BITDCL w[_SHR_BITDCLSIZE(
                    BCM_COMPAT656_FIELD_QUALIFY_CNT + BCM_FIELD_USER_NUM_UDFS)];
    SHR_BITDCL udf_map[_SHR_BITDCLSIZE(BCM_FIELD_USER_NUM_UDFS)];
} bcm_compat656_field_qset_t;

extern int bcm_compat656_field_group_create(
    int unit,
    bcm_compat656_field_qset_t qset,
    int pri,
    bcm_field_group_t *group);

extern int bcm_compat656_field_group_create_id(
    int unit,
    bcm_compat656_field_qset_t qset,
    int pri,
    bcm_field_group_t group);

extern int bcm_compat656_field_qset_data_qualifier_add(
    int unit,
    bcm_compat656_field_qset_t *qset,
    int qual_id);

#endif	/* BCM_RPC_SUPPORT */

#endif	/* !_COMPAT_656_H */
