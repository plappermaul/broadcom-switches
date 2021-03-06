/**************************************************************************
 *
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated by HA parser from YAML formated file.
 * Edits to this file will be lost when it is regenerated.
 * Tool: bcmha/scripts/ha_yml_parser.py
 *
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */
#ifndef BCMCFG_TRM_RESOURCES_HA_CONFIG_H
#define BCMCFG_TRM_RESOURCES_HA_CONFIG_H

#include <shr/shr_ha.h>

/*!
 * \brief Software component configuration for transaction manager resources.
 * 
 * The software component configuration for transaction manager resources is derived
 * 
 * from the BCMCFG configuration YAML file using the following
 * 
 * schema:
 * 
 * \code{.unparsed}
 * 
 *  component:
 * 
 *    trm_resources:
 * 
 *      max_entries: integer
 * 
 *      max_transactions: integer
 * 
 * \endcode
 */
typedef struct {
   /*! \brief Maximum number of system wide active entries.
     *
     * The number of entries are shared between all the units.
     * Entry allocation and free operations are fast due to allocation
     * of the entry structure from a free list of structures. The OS
     * memory allocation is done conservatively only when the free
     * list is empty. Then, the system allocates another chunk of
     * entry structures and adds them onto the free list.
     * This value is used as the upper bound for the number of entries
     * that will be allocated from the OS. The purpose of this upper
     * bound is to be able to detect leakage of entry resources.
     * Memory leakage can happen if the application allocates an entry
     * but never frees it later.
     * Note that entries that are part of a transaction will be
     * automatically freed once the transaction is freed.\n
     * Also note that this number restricts the number of entries that
     * can be included in a single transaction.\n
     * The default value is 512. Setting this number too low might
     * cause the application to fail an attempt of allocating an entry.
     * Low number may also reduce the system performance as more
     * operations will have to be sequenced. */
   uint32_t max_entries;
   /*! \brief Maximum number of system wide active transactions.
     *
     * Similar to the max_entries, the max_transactions is the upper
     * bound number of the instances of active transactions in the
     * system. The purpose of this limit is to detect memory leaks
     * where a transaction was allocated but never freed.\n
     * The default value is 128. */
   uint32_t max_transactions;
} bcmcfg_trm_resources_ha_config_t;

#define BCMCFG_TRM_RESOURCES_HA_CONFIG_T_ID 0x09dcf57b5c44f8f2

#endif /* BCMCFG_TRM_RESOURCES_HA_CONFIG_H */
