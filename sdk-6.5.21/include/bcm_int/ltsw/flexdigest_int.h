/*
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated.
 * Edits to this file will be lost when it is regenerated.
 *
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

#ifndef BCMINT_LTSW_FLEXDIGEST_INT_H
#define BCMINT_LTSW_FLEXDIGEST_INT_H

#include <bcm/flexdigest.h>

/*!
 * \brief The device information of flexdigest functionalities.
 */
typedef enum bcmint_flexdigest_info_s {

    /*! The flexdigest normalization seed profile identifier count. */
    bcmiFlexDigestInfoNormSeedProfileIdCount = 0,

    /*! The flexdigest normalization profile identifier count. */
    bcmiFlexDigestInfoNormProfileIdCount = 1,

    /*! The flexdigest hashing profile identifier count. */
    bcmiFlexDigestInfoHashProfileIdCount = 2,

    /*! The number of FD lookup groups. */
    bcmiFlexDigestInfoNumGroups = 3,

    /*! The number of FD lookup entries. */
    bcmiFlexDigestInfoNumEntries = 4,

    /*! The maximum priority of FD lookup group. */
    bcmiFlexDigestInfoGroupPrioMax = 5,

    /*! The maximum priority of FD lookup entry. */
    bcmiFlexDigestInfoEntryPrioMax = 6,

    /*! The maximum of FD lookup qualifier in an entry. */
    bcmiFlexDigestInfoQualifierMaxPerEntry = 7,

    /*! The maximum of FD lookup action in an entry. */
    bcmiFlexDigestInfoActionMaxPerEntry = 8,

    /*! The number of FD lookup match identifiers. */
    bcmiFlexDigestInfoNumMatchIds = 9,

    /*! The bit width of FD lookup match bitmap. */
    bcmiFlexDigestInfoMatchWidth = 10,

    /*! The flexdigest information count. */
    bcmiFlexDigestInfoCount = 11

} bcmint_flexdigest_info_t;

#define BCMINT_FLEXDIGEST_INFO_STR \
{ \
    "NormSeedProfileIdCount", \
    "NormProfileIdCount", \
    "HashProfileIdCount", \
    "NumGroups", \
    "NumEntries", \
    "GroupPrioMax", \
    "EntryPrioMax", \
    "QualifierMaxPerEntry", \
    "ActionMaxPerEntry", \
    "NumMatchIds", \
    "MatchWidth" \
}

/*!
 * \brief Flex digest entry information.
 */
typedef struct bcmint_flexdigest_entry_info_s {

    /*! The entry identifer in a group. */
    bcm_flexdigest_entry_t id;

    /*! The group priority. */
    int group_prio;

    /*! The GENE handler. */
    void *handle;

} bcmint_flexdigest_entry_info_t;

#endif /* BCMINT_LTSW_FLEXDIGEST_INT_H */
