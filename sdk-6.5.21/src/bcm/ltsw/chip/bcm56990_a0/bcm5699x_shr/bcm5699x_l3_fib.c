/*! \file bcm5699x_l3_fib.c
 *
 * BCM5699x L3 FIB subordinate driver.
 */
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */
#include <bcm_int/ltsw/property.h>
#include <bcm_int/ltsw/lt_intf.h>
#include <bcm_int/ltsw/mbcm/l3_fib.h>
#include <bcm_int/ltsw/xgs/l3_fib.h>
#include <bcm_int/ltsw/l3_fib_int.h>
#include <bcm_int/ltsw/property.h>
#include <bcm_int/ltsw/uft.h>

#include <bcmltd/chip/bcmltd_str.h>
#include <bcmltd/chip/bcmltd_id.h>
#include <shr/shr_debug.h>

/******************************************************************************
 * Local definitions
 */

#define BSL_LOG_MODULE BSL_LS_BCM_L3

static const char *key_type_str[BCMINT_LTSW_L3_ALPM_USAGE_KT_CNT] = {
    KEY_L3_IPV4_UCs,
    KEY_L3_IPV4_UC_VRFs,
    KEY_L3_IPV4_UC_OVERRIDEs,
    KEY_L3_IPV6_UC_SINGLEs,
    KEY_L3_IPV6_UC_SINGLE_VRFs,
    KEY_L3_IPV6_UC_SINGLE_OVERRIDEs,
    KEY_L3_IPV6_UC_DOUBLEs,
    KEY_L3_IPV6_UC_DOUBLE_VRFs,
    KEY_L3_IPV6_UC_DOUBLE_OVERRIDEs,
    KEY_L3_IPV6_UC_QUADs,
    KEY_L3_IPV6_UC_QUAD_VRFs,
    KEY_L3_IPV6_UC_QUAD_OVERRIDEs,
    KEY_L3_IPV4_COMPs,
    KEY_L3_IPV6_COMPs,
    KEY_L3_IPV4_MCs,
    KEY_L3_IPV6_MCs
};

static const char *db_str[] = {
    ALPM_DB_0s,
    ALPM_DB_1s,
    ALPM_DB_2s,
    ALPM_DB_3s
};

static const char *key_input_str[BCMINT_LTSW_L3_ALPM_KI_CNT] = {
    ALPM_KEY_INPUT_LPM_DST_QUADs,
    ALPM_KEY_INPUT_LPM_DST_DOUBLEs,
    ALPM_KEY_INPUT_LPM_DST_SINGLEs,
    ALPM_KEY_INPUT_LPM_SRC_QUADs,
    ALPM_KEY_INPUT_LPM_SRC_DOUBLEs,
    ALPM_KEY_INPUT_LPM_SRC_SINGLEs,
    ALPM_KEY_INPUT_FP_COMP_DSTs,
    ALPM_KEY_INPUT_FP_COMP_SRCs,
    ALPM_KEY_INPUT_LPM_L3MC_QUADs,
    ALPM_KEY_INPUT_LPM_L3MC_DOUBLEs,
};

/*!
 * \brief Get key type from sybmol.
 *
 * \param [in] unit Unit number.
 * \param [in] symbol String.
 * \param [out] value data_type.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
static int
key_type_symbol_to_scalar(int unit, const char *symbol, uint64_t *value)
{
    int i;

    for (i = 0; i < BCMINT_LTSW_L3_ALPM_USAGE_KT_CNT; i++) {
        if (!key_type_str[i]) {
            continue;
        }
        if (!sal_strcmp(symbol, key_type_str[i])) {
            break;
        }
    }

    if (i >= BCMINT_LTSW_L3_ALPM_USAGE_KT_CNT) {
        return SHR_E_INTERNAL;
    }

    *value = i;

    return SHR_E_NONE;
}

/*!
 * \brief Get key type sybmol from value.
 *
 * \param [in] unit Unit number.
 * \param [in] value data_type.
 * \param [out] symbol String.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
static int
key_type_scalar_to_symbol(int unit, uint64_t value, const char **symbol)
{
    if ((value < BCMINT_LTSW_L3_ALPM_USAGE_KT_CNT) && key_type_str[value]) {
        *symbol = key_type_str[value];
    } else {
        return SHR_E_INTERNAL;
    }

    return SHR_E_NONE;
}

/*!
 * \brief Get DB from sybmol.
 *
 * \param [in] unit Unit number.
 * \param [in] symbol String.
 * \param [out] value data_type.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
static int
db_symbol_to_scalar(int unit, const char *symbol, uint64_t *value)
{
    int i;

    for (i = 0; i < BCMINT_LTSW_L3_ALPM_DB_CNT; i++) {
        if (!db_str[i]) {
            continue;
        }
        if (!sal_strcmp(symbol, db_str[i])) {
            break;
        }
    }

    if (i >= BCMINT_LTSW_L3_ALPM_DB_CNT) {
        return SHR_E_INTERNAL;
    }

    *value = i;

    return SHR_E_NONE;
}

/*!
 * \brief Get DB sybmol from value.
 *
 * \param [in] unit Unit number.
 * \param [in] value data_type.
 * \param [out] symbol String.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
static int
db_scalar_to_symbol(int unit, uint64_t value, const char **symbol)
{
    if ((value < BCMINT_LTSW_L3_ALPM_DB_CNT) && db_str[value]) {
        *symbol = db_str[value];
    } else {
        return SHR_E_INTERNAL;
    }

    return SHR_E_NONE;
}

/*!
 * \brief Get key input type from sybmol.
 *
 * \param [in] unit Unit number.
 * \param [in] symbol String.
 * \param [out] value data_type.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
static int
key_input_symbol_to_scalar(int unit, const char *symbol, uint64_t *value)
{
    int i;

    for (i = 0; i < BCMINT_LTSW_L3_ALPM_KI_CNT; i++) {
        if (!key_input_str[i]) {
            continue;
        }
        if (!sal_strcmp(symbol, key_input_str[i])) {
            break;
        }
    }

    if (i >= BCMINT_LTSW_L3_ALPM_KI_CNT) {
        return SHR_E_INTERNAL;
    }

    *value = i;

    return SHR_E_NONE;
}

/*!
 * \brief Get key input type sybmol from value.
 *
 * \param [in] unit Unit number.
 * \param [in] value data_type.
 * \param [out] symbol String.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
static int
key_input_scalar_to_symbol(int unit, uint64_t value, const char **symbol)
{
    if ((value < BCMINT_LTSW_L3_ALPM_KI_CNT) && key_input_str[value]) {
        *symbol = key_input_str[value];
    } else {
        return SHR_E_INTERNAL;
    }

    return SHR_E_NONE;
}

/*!
 * \brief Get operational state from sybmol.
 *
 * \param [in] unit Unit number.
 * \param [in] symbol String.
 * \param [out] value data_type.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
static int
op_state_symbol_to_scalar(int unit, const char *symbol, uint64_t *value)
{
    if (!sal_strcmp(symbol, VALIDs)) {
        *value = BCMINT_LTSW_L3_ALPM_OP_STATE_VALID;
    } else if (!sal_strcmp(symbol, CONFLICTING_ALPM_BANKSs)) {
        *value = BCMINT_LTSW_L3_ALPM_OP_STATE_CONFILICT;
    } else if (!sal_strcmp(symbol, ALPM_IN_USEs)) {
        *value = BCMINT_LTSW_L3_ALPM_OP_STATE_INUSE;
    } else {
        return SHR_E_INTERNAL;
    }

    return SHR_E_NONE;
}

/*!
 * \brief Get operational state sybmol from value.
 *
 * \param [in] unit Unit number.
 * \param [in] value data_type.
 * \param [out] symbol String.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
static int
op_state_scalar_to_symbol(int unit, uint64_t value, const char **symbol)
{
    if (value == BCMINT_LTSW_L3_ALPM_OP_STATE_VALID) {
        *symbol = VALIDs;
    } else if (value == BCMINT_LTSW_L3_ALPM_OP_STATE_CONFILICT) {
        *symbol = CONFLICTING_ALPM_BANKSs;
    } else if (value == BCMINT_LTSW_L3_ALPM_OP_STATE_INUSE) {
        *symbol = ALPM_IN_USEs;
    } else {
        return SHR_E_INTERNAL;
    }

    return SHR_E_NONE;
}


/*
 * L3_IPV4_UC_ROUTE
 */
static const
bcmint_ltsw_l3_fib_fld_t l3_ipv4_uc_gl_flds[BCMINT_XGS_L3_FIB_TBL_UC_FLD_CNT] = {
    [BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_0] = {
        IPV4s,
        L3_IPV4_UC_ROUTEt_IPV4f,
        true,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_0_MASK] = {
        IPV4_MASKs,
        L3_IPV4_UC_ROUTEt_IPV4_MASKf,
        true,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_ECMP_NHOP] = {
        ECMP_NHOPs,
        L3_IPV4_UC_ROUTEt_ECMP_NHOPf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_ECMP_ID] = {
        ECMP_IDs,
        L3_IPV4_UC_ROUTEt_ECMP_IDf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_NHOP_ID] = {
        NHOP_IDs,
        L3_IPV4_UC_ROUTEt_NHOP_IDf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_REPLACE_INT_PRI] = {
        REPLACE_INT_PRIs,
        L3_IPV4_UC_ROUTEt_REPLACE_INT_PRIf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_INT_PRI] = {
        INT_PRIs,
        L3_IPV4_UC_ROUTEt_INT_PRIf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_DROP_ON_DIP_MATCH] = {
        DROP_ON_DIP_MATCHs,
        L3_IPV4_UC_ROUTEt_DROP_ON_DIP_MATCHf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_CLASS_ID] = {
        CLASS_IDs,
        L3_IPV4_UC_ROUTEt_CLASS_IDf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_FLEX_CTR_OBJECT] = {
        CTR_ING_EFLEX_OBJECTs,
        L3_IPV4_UC_ROUTEt_CTR_ING_EFLEX_OBJECTf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_FLEX_CTR_ACTION_ID] = {
        L3_IPV4_UC_ROUTE_CTR_ING_EFLEX_ACTION_IDs,
        L3_IPV4_UC_ROUTEt_L3_IPV4_UC_ROUTE_CTR_ING_EFLEX_ACTION_IDf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_HIT] = {
        HITs,
        L3_IPV4_UC_ROUTEt_HITf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_INT_ACT_PROFILE_ID] = {
        MON_INBAND_TELEMETRY_ACTION_PROFILE_IDs,
        L3_IPV4_UC_ROUTEt_MON_INBAND_TELEMETRY_ACTION_PROFILE_IDf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_INT_OPAQUE_DATA_PROFILE_ID] = {
        MON_INBAND_TELEMETRY_OPAQUE_DATA_PROFILE_IDs,
        L3_IPV4_UC_ROUTEt_MON_INBAND_TELEMETRY_OPAQUE_DATA_PROFILE_IDf,
        false,
        false,
        0,
        NULL,
        NULL
    },
};

/*
 * L3_IPV4_UC_ROUTE_OVERRIDE
 */
static const
bcmint_ltsw_l3_fib_fld_t l3_ipv4_uc_gh_flds[BCMINT_XGS_L3_FIB_TBL_UC_FLD_CNT] = {
    [BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_0] = {
        IPV4s,
        L3_IPV4_UC_ROUTE_OVERRIDEt_IPV4f,
        true,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_0_MASK] = {
        IPV4_MASKs,
        L3_IPV4_UC_ROUTE_OVERRIDEt_IPV4_MASKf,
        true,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_ECMP_NHOP] = {
        ECMP_NHOPs,
        L3_IPV4_UC_ROUTE_OVERRIDEt_ECMP_NHOPf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_ECMP_ID] = {
        ECMP_IDs,
        L3_IPV4_UC_ROUTE_OVERRIDEt_ECMP_IDf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_NHOP_ID] = {
        NHOP_IDs,
        L3_IPV4_UC_ROUTE_OVERRIDEt_NHOP_IDf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_REPLACE_INT_PRI] = {
        REPLACE_INT_PRIs,
        L3_IPV4_UC_ROUTE_OVERRIDEt_REPLACE_INT_PRIf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_INT_PRI] = {
        INT_PRIs,
        L3_IPV4_UC_ROUTE_OVERRIDEt_INT_PRIf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_DROP_ON_DIP_MATCH] = {
        DROP_ON_DIP_MATCHs,
        L3_IPV4_UC_ROUTE_OVERRIDEt_DROP_ON_DIP_MATCHf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_CLASS_ID] = {
        CLASS_IDs,
        L3_IPV4_UC_ROUTE_OVERRIDEt_CLASS_IDf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_FLEX_CTR_OBJECT] = {
        CTR_ING_EFLEX_OBJECTs,
        L3_IPV4_UC_ROUTE_OVERRIDEt_CTR_ING_EFLEX_OBJECTf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_FLEX_CTR_ACTION_ID] = {
        L3_IPV4_UC_ROUTE_CTR_ING_EFLEX_ACTION_IDs,
        L3_IPV4_UC_ROUTE_OVERRIDEt_L3_IPV4_UC_ROUTE_CTR_ING_EFLEX_ACTION_IDf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_HIT] = {
        HITs,
        L3_IPV4_UC_ROUTE_OVERRIDEt_HITf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_INT_ACT_PROFILE_ID] = {
        MON_INBAND_TELEMETRY_ACTION_PROFILE_IDs,
        L3_IPV4_UC_ROUTE_OVERRIDEt_MON_INBAND_TELEMETRY_ACTION_PROFILE_IDf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_INT_OPAQUE_DATA_PROFILE_ID] = {
        MON_INBAND_TELEMETRY_OPAQUE_DATA_PROFILE_IDs,
        L3_IPV4_UC_ROUTE_OVERRIDEt_MON_INBAND_TELEMETRY_OPAQUE_DATA_PROFILE_IDf,
        false,
        false,
        0,
        NULL,
        NULL
    },
};

/*
 * L3_IPV4_UC_ROUTE_VRF
 */
static const
bcmint_ltsw_l3_fib_fld_t l3_ipv4_uc_vrf_flds[BCMINT_XGS_L3_FIB_TBL_UC_FLD_CNT] = {
    [BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_0] = {
        IPV4s,
        L3_IPV4_UC_ROUTE_VRFt_IPV4f,
        true,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_0_MASK] = {
        IPV4_MASKs,
        L3_IPV4_UC_ROUTE_VRFt_IPV4_MASKf,
        true,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_VRF] = {
        VRF_IDs,
        L3_IPV4_UC_ROUTE_VRFt_VRF_IDf,
        true,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_ECMP_NHOP] = {
        ECMP_NHOPs,
        L3_IPV4_UC_ROUTE_VRFt_ECMP_NHOPf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_ECMP_ID] = {
        ECMP_IDs,
        L3_IPV4_UC_ROUTE_VRFt_ECMP_IDf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_NHOP_ID] = {
        NHOP_IDs,
        L3_IPV4_UC_ROUTE_VRFt_NHOP_IDf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_REPLACE_INT_PRI] = {
        REPLACE_INT_PRIs,
        L3_IPV4_UC_ROUTE_VRFt_REPLACE_INT_PRIf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_INT_PRI] = {
        INT_PRIs,
        L3_IPV4_UC_ROUTE_VRFt_INT_PRIf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_DROP_ON_DIP_MATCH] = {
        DROP_ON_DIP_MATCHs,
        L3_IPV4_UC_ROUTE_VRFt_DROP_ON_DIP_MATCHf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_CLASS_ID] = {
        CLASS_IDs,
        L3_IPV4_UC_ROUTE_VRFt_CLASS_IDf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_FLEX_CTR_OBJECT] = {
        CTR_ING_EFLEX_OBJECTs,
        L3_IPV4_UC_ROUTE_VRFt_CTR_ING_EFLEX_OBJECTf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_FLEX_CTR_ACTION_ID] = {
        L3_IPV4_UC_ROUTE_CTR_ING_EFLEX_ACTION_IDs,
        L3_IPV4_UC_ROUTE_VRFt_L3_IPV4_UC_ROUTE_CTR_ING_EFLEX_ACTION_IDf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_HIT] = {
        HITs,
        L3_IPV4_UC_ROUTE_VRFt_HITf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_INT_ACT_PROFILE_ID] = {
        MON_INBAND_TELEMETRY_ACTION_PROFILE_IDs,
        L3_IPV4_UC_ROUTE_VRFt_MON_INBAND_TELEMETRY_ACTION_PROFILE_IDf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_INT_OPAQUE_DATA_PROFILE_ID] = {
        MON_INBAND_TELEMETRY_OPAQUE_DATA_PROFILE_IDs,
        L3_IPV4_UC_ROUTE_VRFt_MON_INBAND_TELEMETRY_OPAQUE_DATA_PROFILE_IDf,
        false,
        false,
        0,
        NULL,
        NULL
    },
};

/*
 * L3_IPV6_UC_ROUTE
 */
static const
bcmint_ltsw_l3_fib_fld_t l3_ipv6_uc_gl_flds[BCMINT_XGS_L3_FIB_TBL_UC_FLD_CNT] = {
    [BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_0] = {
        IPV6_LOWERs,
        L3_IPV6_UC_ROUTEt_IPV6u_LOWERf,
        true,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_0_MASK] = {
        IPV6_LOWER_MASKs,
        L3_IPV6_UC_ROUTEt_IPV6u_LOWER_MASKf,
        true,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_1] = {
        IPV6_UPPERs,
        L3_IPV6_UC_ROUTEt_IPV6u_UPPERf,
        true,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_1_MASK] = {
        IPV6_UPPER_MASKs,
        L3_IPV6_UC_ROUTEt_IPV6u_UPPER_MASKf,
        true,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_ECMP_NHOP] = {
        ECMP_NHOPs,
        L3_IPV6_UC_ROUTEt_ECMP_NHOPf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_ECMP_ID] = {
        ECMP_IDs,
        L3_IPV6_UC_ROUTEt_ECMP_IDf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_NHOP_ID] = {
        NHOP_IDs,
        L3_IPV6_UC_ROUTEt_NHOP_IDf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_REPLACE_INT_PRI] = {
        REPLACE_INT_PRIs,
        L3_IPV6_UC_ROUTEt_REPLACE_INT_PRIf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_INT_PRI] = {
        INT_PRIs,
        L3_IPV6_UC_ROUTEt_INT_PRIf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_DROP_ON_DIP_MATCH] = {
        DROP_ON_DIP_MATCHs,
        L3_IPV6_UC_ROUTEt_DROP_ON_DIP_MATCHf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_CLASS_ID] = {
        CLASS_IDs,
        L3_IPV6_UC_ROUTEt_CLASS_IDf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_FLEX_CTR_OBJECT] = {
        CTR_ING_EFLEX_OBJECTs,
        L3_IPV6_UC_ROUTEt_CTR_ING_EFLEX_OBJECTf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_FLEX_CTR_ACTION_ID] = {
        L3_IPV6_UC_ROUTE_CTR_ING_EFLEX_ACTION_IDs,
        L3_IPV6_UC_ROUTEt_L3_IPV6_UC_ROUTE_CTR_ING_EFLEX_ACTION_IDf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_HIT] = {
        HITs,
        L3_IPV6_UC_ROUTEt_HITf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_INT_ACT_PROFILE_ID] = {
        MON_INBAND_TELEMETRY_ACTION_PROFILE_IDs,
        L3_IPV6_UC_ROUTEt_MON_INBAND_TELEMETRY_ACTION_PROFILE_IDf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_INT_OPAQUE_DATA_PROFILE_ID] = {
        MON_INBAND_TELEMETRY_OPAQUE_DATA_PROFILE_IDs,
        L3_IPV6_UC_ROUTEt_MON_INBAND_TELEMETRY_OPAQUE_DATA_PROFILE_IDf,
        false,
        false,
        0,
        NULL,
        NULL
    },
};

/*
 * L3_IPV6_UC_ROUTE_OVERRIDE
 */
static const
bcmint_ltsw_l3_fib_fld_t l3_ipv6_uc_gh_flds[BCMINT_XGS_L3_FIB_TBL_UC_FLD_CNT] = {
    [BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_0] = {
        IPV6_LOWERs,
        L3_IPV6_UC_ROUTE_OVERRIDEt_IPV6u_LOWERf,
        true,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_0_MASK] = {
        IPV6_UPPERs,
        L3_IPV6_UC_ROUTE_OVERRIDEt_IPV6u_LOWER_MASKf,
        true,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_1] = {
        IPV6_UPPERs,
        L3_IPV6_UC_ROUTE_OVERRIDEt_IPV6u_UPPERf,
        true,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_1_MASK] = {
        IPV6_UPPERs,
        L3_IPV6_UC_ROUTE_OVERRIDEt_IPV6u_UPPER_MASKf,
        true,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_ECMP_NHOP] = {
        ECMP_NHOPs,
        L3_IPV6_UC_ROUTE_OVERRIDEt_ECMP_NHOPf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_ECMP_ID] = {
        ECMP_IDs,
        L3_IPV6_UC_ROUTE_OVERRIDEt_ECMP_IDf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_NHOP_ID] = {
        NHOP_IDs,
        L3_IPV6_UC_ROUTE_OVERRIDEt_NHOP_IDf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_REPLACE_INT_PRI] = {
        REPLACE_INT_PRIs,
        L3_IPV6_UC_ROUTE_OVERRIDEt_REPLACE_INT_PRIf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_INT_PRI] = {
        INT_PRIs,
        L3_IPV6_UC_ROUTE_OVERRIDEt_INT_PRIf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_DROP_ON_DIP_MATCH] = {
        DROP_ON_DIP_MATCHs,
        L3_IPV6_UC_ROUTE_OVERRIDEt_DROP_ON_DIP_MATCHf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_CLASS_ID] = {
        CLASS_IDs,
        L3_IPV6_UC_ROUTE_OVERRIDEt_CLASS_IDf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_FLEX_CTR_OBJECT] = {
        CTR_ING_EFLEX_OBJECTs,
        L3_IPV6_UC_ROUTE_OVERRIDEt_CTR_ING_EFLEX_OBJECTf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_FLEX_CTR_ACTION_ID] = {
        L3_IPV6_UC_ROUTE_CTR_ING_EFLEX_ACTION_IDs,
        L3_IPV6_UC_ROUTE_OVERRIDEt_L3_IPV6_UC_ROUTE_CTR_ING_EFLEX_ACTION_IDf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_HIT] = {
        HITs,
        L3_IPV6_UC_ROUTE_OVERRIDEt_HITf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_INT_ACT_PROFILE_ID] = {
        MON_INBAND_TELEMETRY_ACTION_PROFILE_IDs,
        L3_IPV6_UC_ROUTE_OVERRIDEt_MON_INBAND_TELEMETRY_ACTION_PROFILE_IDf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_INT_OPAQUE_DATA_PROFILE_ID] = {
        MON_INBAND_TELEMETRY_OPAQUE_DATA_PROFILE_IDs,
        L3_IPV6_UC_ROUTE_OVERRIDEt_MON_INBAND_TELEMETRY_OPAQUE_DATA_PROFILE_IDf,
        false,
        false,
        0,
        NULL,
        NULL
    },
};

/*
 * L3_IPV6_UC_ROUTE_VRF
 */
static const
bcmint_ltsw_l3_fib_fld_t l3_ipv6_uc_vrf_flds[BCMINT_XGS_L3_FIB_TBL_UC_FLD_CNT] = {
    [BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_0] = {
        IPV6_LOWERs,
        L3_IPV6_UC_ROUTE_VRFt_IPV6u_LOWERf,
        true,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_0_MASK] = {
        IPV6_UPPERs,
        L3_IPV6_UC_ROUTE_VRFt_IPV6u_LOWER_MASKf,
        true,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_1] = {
        IPV6_UPPERs,
        L3_IPV6_UC_ROUTE_VRFt_IPV6u_UPPERf,
        true,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_1_MASK] = {
        IPV6_UPPERs,
        L3_IPV6_UC_ROUTE_VRFt_IPV6u_UPPER_MASKf,
        true,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_VRF] = {
        VRF_IDs,
        L3_IPV6_UC_ROUTE_VRFt_VRF_IDf,
        true,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_ECMP_NHOP] = {
        ECMP_NHOPs,
        L3_IPV6_UC_ROUTE_VRFt_ECMP_NHOPf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_ECMP_ID] = {
        ECMP_IDs,
        L3_IPV6_UC_ROUTE_VRFt_ECMP_IDf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_NHOP_ID] = {
        NHOP_IDs,
        L3_IPV6_UC_ROUTE_VRFt_NHOP_IDf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_REPLACE_INT_PRI] = {
        REPLACE_INT_PRIs,
        L3_IPV6_UC_ROUTE_VRFt_REPLACE_INT_PRIf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_INT_PRI] = {
        INT_PRIs,
        L3_IPV6_UC_ROUTE_VRFt_INT_PRIf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_DROP_ON_DIP_MATCH] = {
        DROP_ON_DIP_MATCHs,
        L3_IPV6_UC_ROUTE_VRFt_DROP_ON_DIP_MATCHf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_CLASS_ID] = {
        CLASS_IDs,
        L3_IPV6_UC_ROUTE_VRFt_CLASS_IDf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_FLEX_CTR_OBJECT] = {
        CTR_ING_EFLEX_OBJECTs,
        L3_IPV6_UC_ROUTE_VRFt_CTR_ING_EFLEX_OBJECTf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_FLEX_CTR_ACTION_ID] = {
        L3_IPV6_UC_ROUTE_CTR_ING_EFLEX_ACTION_IDs,
        L3_IPV6_UC_ROUTE_VRFt_L3_IPV6_UC_ROUTE_CTR_ING_EFLEX_ACTION_IDf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_HIT] = {
        HITs,
        L3_IPV6_UC_ROUTE_VRFt_HITf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_INT_ACT_PROFILE_ID] = {
        MON_INBAND_TELEMETRY_ACTION_PROFILE_IDs,
        L3_IPV6_UC_ROUTE_VRFt_MON_INBAND_TELEMETRY_ACTION_PROFILE_IDf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_INT_OPAQUE_DATA_PROFILE_ID] = {
        MON_INBAND_TELEMETRY_OPAQUE_DATA_PROFILE_IDs,
        L3_IPV6_UC_ROUTE_VRFt_MON_INBAND_TELEMETRY_OPAQUE_DATA_PROFILE_IDf,
        false,
        false,
        0,
        NULL,
        NULL
    },
};

/*
 * L3_SRC_IPV4_UC_ROUTE
 */
static const
bcmint_ltsw_l3_fib_fld_t l3_src_ipv4_uc_gl_flds[BCMINT_XGS_L3_FIB_TBL_UC_FLD_CNT] = {
    [BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_0] = {
        IPV4s,
        L3_SRC_IPV4_UC_ROUTEt_IPV4f,
        true,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_0_MASK] = {
        IPV4_MASKs,
        L3_SRC_IPV4_UC_ROUTEt_IPV4_MASKf,
        true,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_CLASS_ID] = {
        CLASS_IDs,
        L3_SRC_IPV4_UC_ROUTEt_CLASS_IDf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_FLEX_CTR_OBJECT] = {
        CTR_ING_EFLEX_OBJECTs,
        L3_SRC_IPV4_UC_ROUTEt_CTR_ING_EFLEX_OBJECTf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_FLEX_CTR_ACTION_ID] = {
        L3_SRC_IPV4_UC_CTR_ING_EFLEX_ACTION_IDs,
        L3_SRC_IPV4_UC_ROUTEt_L3_SRC_IPV4_UC_CTR_ING_EFLEX_ACTION_IDf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_HIT] = {
        HITs,
        L3_SRC_IPV4_UC_ROUTEt_HITf,
        false,
        false,
        0,
        NULL,
        NULL
    },
};

/*
 * L3_SRC_IPV4_UC_ROUTE_OVERRIDE
 */
static const
bcmint_ltsw_l3_fib_fld_t l3_src_ipv4_uc_gh_flds[BCMINT_XGS_L3_FIB_TBL_UC_FLD_CNT] = {
    [BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_0] = {
        IPV4s,
        L3_SRC_IPV4_UC_ROUTE_OVERRIDEt_IPV4f,
        true,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_0_MASK] = {
        IPV4_MASKs,
        L3_SRC_IPV4_UC_ROUTE_OVERRIDEt_IPV4_MASKf,
        true,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_CLASS_ID] = {
        CLASS_IDs,
        L3_SRC_IPV4_UC_ROUTE_OVERRIDEt_CLASS_IDf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_FLEX_CTR_OBJECT] = {
        CTR_ING_EFLEX_OBJECTs,
        L3_SRC_IPV4_UC_ROUTE_OVERRIDEt_CTR_ING_EFLEX_OBJECTf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_FLEX_CTR_ACTION_ID] = {
        L3_SRC_IPV4_UC_CTR_ING_EFLEX_ACTION_IDs,
        L3_SRC_IPV4_UC_ROUTE_OVERRIDEt_L3_SRC_IPV4_UC_CTR_ING_EFLEX_ACTION_IDf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_HIT] = {
        HITs,
        L3_SRC_IPV4_UC_ROUTE_OVERRIDEt_HITf,
        false,
        false,
        0,
        NULL,
        NULL
    },
};

/*
 * L3_SRC_IPV4_UC_ROUTE_VRF
 */
static const
bcmint_ltsw_l3_fib_fld_t l3_src_ipv4_uc_vrf_flds[BCMINT_XGS_L3_FIB_TBL_UC_FLD_CNT] = {
    [BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_0] = {
        IPV4s,
        L3_SRC_IPV4_UC_ROUTE_VRFt_IPV4f,
        true,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_0_MASK] = {
        IPV4_MASKs,
        L3_SRC_IPV4_UC_ROUTE_VRFt_IPV4_MASKf,
        true,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_VRF] = {
        VRF_IDs,
        L3_SRC_IPV4_UC_ROUTE_VRFt_VRF_IDf,
        true,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_CLASS_ID] = {
        CLASS_IDs,
        L3_SRC_IPV4_UC_ROUTE_VRFt_CLASS_IDf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_FLEX_CTR_OBJECT] = {
        CTR_ING_EFLEX_OBJECTs,
        L3_SRC_IPV4_UC_ROUTE_VRFt_CTR_ING_EFLEX_OBJECTf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_FLEX_CTR_ACTION_ID] = {
        L3_SRC_IPV4_UC_CTR_ING_EFLEX_ACTION_IDs,
        L3_SRC_IPV4_UC_ROUTE_VRFt_L3_SRC_IPV4_UC_CTR_ING_EFLEX_ACTION_IDf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_HIT] = {
        HITs,
        L3_SRC_IPV4_UC_ROUTE_VRFt_HITf,
        false,
        false,
        0,
        NULL,
        NULL
    },
};

/*
 * L3_SRC_IPV6_UC_ROUTE
 */
static const
bcmint_ltsw_l3_fib_fld_t l3_src_ipv6_uc_gl_flds[BCMINT_XGS_L3_FIB_TBL_UC_FLD_CNT] = {
    [BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_0] = {
        IPV6_LOWERs,
        L3_SRC_IPV6_UC_ROUTEt_IPV6u_LOWERf,
        true,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_0_MASK] = {
        IPV6_LOWER_MASKs,
        L3_SRC_IPV6_UC_ROUTEt_IPV6u_LOWER_MASKf,
        true,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_1] = {
        IPV6_UPPERs,
        L3_SRC_IPV6_UC_ROUTEt_IPV6u_UPPERf,
        true,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_1_MASK] = {
        IPV6_UPPER_MASKs,
        L3_SRC_IPV6_UC_ROUTEt_IPV6u_UPPER_MASKf,
        true,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_CLASS_ID] = {
        CLASS_IDs,
        L3_SRC_IPV6_UC_ROUTEt_CLASS_IDf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_FLEX_CTR_OBJECT] = {
        CTR_ING_EFLEX_OBJECTs,
        L3_SRC_IPV6_UC_ROUTEt_CTR_ING_EFLEX_OBJECTf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_FLEX_CTR_ACTION_ID] = {
        L3_SRC_IPV6_UC_CTR_ING_EFLEX_ACTION_IDs,
        L3_SRC_IPV6_UC_ROUTEt_L3_SRC_IPV6_UC_CTR_ING_EFLEX_ACTION_IDf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_HIT] = {
        HITs,
        L3_SRC_IPV6_UC_ROUTEt_HITf,
        false,
        false,
        0,
        NULL,
        NULL
    },
};

/*
 * L3_SRC_IPV6_UC_ROUTE_OVERRIDE
 */
static const
bcmint_ltsw_l3_fib_fld_t l3_src_ipv6_uc_gh_flds[BCMINT_XGS_L3_FIB_TBL_UC_FLD_CNT] = {
    [BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_0] = {
        IPV6_LOWERs,
        L3_SRC_IPV6_UC_ROUTE_OVERRIDEt_IPV6u_LOWERf,
        true,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_0_MASK] = {
        IPV6_LOWER_MASKs,
        L3_SRC_IPV6_UC_ROUTE_OVERRIDEt_IPV6u_LOWER_MASKf,
        true,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_1] = {
        IPV6_UPPERs,
        L3_SRC_IPV6_UC_ROUTE_OVERRIDEt_IPV6u_UPPERf,
        true,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_1_MASK] = {
        IPV6_UPPER_MASKs,
        L3_SRC_IPV6_UC_ROUTE_OVERRIDEt_IPV6u_UPPER_MASKf,
        true,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_CLASS_ID] = {
        CLASS_IDs,
        L3_SRC_IPV6_UC_ROUTE_OVERRIDEt_CLASS_IDf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_FLEX_CTR_OBJECT] = {
        CTR_ING_EFLEX_OBJECTs,
        L3_SRC_IPV6_UC_ROUTE_OVERRIDEt_CTR_ING_EFLEX_OBJECTf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_FLEX_CTR_ACTION_ID] = {
        L3_SRC_IPV6_UC_CTR_ING_EFLEX_ACTION_IDs,
        L3_SRC_IPV6_UC_ROUTE_OVERRIDEt_L3_SRC_IPV6_UC_CTR_ING_EFLEX_ACTION_IDf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_HIT] = {
        HITs,
        L3_SRC_IPV6_UC_ROUTE_OVERRIDEt_HITf,
        false,
        false,
        0,
        NULL,
        NULL
    },
};

/*
 * L3_SRC_IPV6_UC_ROUTE_VRF
 */
static const
bcmint_ltsw_l3_fib_fld_t l3_src_ipv6_uc_vrf_flds[BCMINT_XGS_L3_FIB_TBL_UC_FLD_CNT] = {
    [BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_0] = {
        IPV6_LOWERs,
        L3_SRC_IPV6_UC_ROUTE_VRFt_IPV6u_LOWERf,
        true,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_0_MASK] = {
        IPV6_LOWER_MASKs,
        L3_SRC_IPV6_UC_ROUTE_VRFt_IPV6u_LOWER_MASKf,
        true,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_1] = {
        IPV6_UPPERs,
        L3_SRC_IPV6_UC_ROUTE_VRFt_IPV6u_UPPERf,
        true,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_1_MASK] = {
        IPV6_UPPER_MASKs,
        L3_SRC_IPV6_UC_ROUTE_VRFt_IPV6u_UPPER_MASKf,
        true,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_VRF] = {
        VRF_IDs,
        L3_SRC_IPV6_UC_ROUTE_VRFt_VRF_IDf,
        true,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_CLASS_ID] = {
        CLASS_IDs,
        L3_SRC_IPV6_UC_ROUTE_VRFt_CLASS_IDf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_FLEX_CTR_OBJECT] = {
        CTR_ING_EFLEX_OBJECTs,
        L3_SRC_IPV6_UC_ROUTE_VRFt_CTR_ING_EFLEX_OBJECTf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_FLEX_CTR_ACTION_ID] = {
        L3_SRC_IPV6_UC_CTR_ING_EFLEX_ACTION_IDs,
        L3_SRC_IPV6_UC_ROUTE_VRFt_L3_SRC_IPV6_UC_CTR_ING_EFLEX_ACTION_IDf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_UC_HIT] = {
        HITs,
        L3_SRC_IPV6_UC_ROUTE_VRFt_HITf,
        false,
        false,
        0,
        NULL,
        NULL
    },
};

/*
 * L3_IPV4_MC_ROUTE
 */
static const
bcmint_ltsw_l3_fib_fld_t l3_ipv4_mc_g_flds[BCMINT_XGS_L3_FIB_TBL_MC_FLD_CNT] = {
    [BCMINT_XGS_L3_FIB_TBL_MC_GROUP_0] = {
        MC_GROUPs,
        L3_IPV4_MC_ROUTEt_MC_GROUPf,
        true,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_MC_L3_IIF_ID] = {
        L3_MC_IIF_IDs,
        L3_IPV4_MC_ROUTEt_L3_MC_IIF_IDf,
        true,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_MC_VRF] = {
        L3_IIFs,
        L3_IPV4_MC_ROUTEt_VRF_IDf,
        true,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_MC_SIP_0_ADDR] = {
        SRC_IPV4s,
        L3_IPV4_MC_ROUTEt_SRC_IPV4f,
        true,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_MC_SIP_0_ADDR_MASK] = {
        SRC_IPV4_MASKs,
        L3_IPV4_MC_ROUTEt_SRC_IPV4_MASKf,
        true,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_MC_GROUP_ID] = {
        TM_MC_GROUP_IDs,
        L3_IPV4_MC_ROUTEt_TM_MC_GROUP_IDf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_MC_CLASS_ID] = {
        CLASS_IDs,
        L3_IPV4_MC_ROUTEt_CLASS_IDf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_MC_COPY_TO_CPU] = {
        COPY_TO_CPUs,
        L3_IPV4_MC_ROUTEt_COPY_TO_CPUf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_MC_DROP] = {
        DROPs,
        L3_IPV4_MC_ROUTEt_DROPf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_MC_DROP_ON_GROUP_MATCH] = {
        DROP_ON_GROUP_MATCHs,
        L3_IPV4_MC_ROUTEt_DROP_ON_GROUP_MATCHf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_MC_USE_PORT_TRUNK_ID] = {
        USE_PORT_TRUNK_IDs,
        L3_IPV4_MC_ROUTEt_USE_PORT_TRUNK_IDf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_MC_IS_TRUNK] = {
        IS_TRUNKs,
        L3_IPV4_MC_ROUTEt_IS_TRUNKf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_MC_TRUNK_ID] = {
        TRUNK_IDs,
        L3_IPV4_MC_ROUTEt_TRUNK_IDf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_MC_PORT] = {
        MODPORTs,
        L3_IPV4_MC_ROUTEt_MODPORTf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_MC_EXPECTED_L3_IIF_ID] = {
        EXPECTED_L3_MC_IIF_IDs,
        L3_IPV4_MC_ROUTEt_EXPECTED_L3_MC_IIF_IDf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_MC_FLEX_CTR_OBJECT] = {
        CTR_ING_EFLEX_OBJECTs,
        L3_IPV4_MC_ROUTEt_CTR_ING_EFLEX_OBJECTf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_MC_FLEX_CTR_ACTION_ID] = {
        L3_IPV4_MC_CTR_ING_EFLEX_ACTION_IDs,
        L3_IPV4_MC_ROUTEt_L3_IPV4_MC_CTR_ING_EFLEX_ACTION_IDf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_MC_HIT] = {
        HITs,
        L3_IPV4_MC_ROUTEt_HITf,
        false,
        false,
        0,
        NULL,
        NULL
    },
};

/*
 * L3_IPV6_MC_ROUTE
 */
static const
bcmint_ltsw_l3_fib_fld_t l3_ipv6_mc_g_flds[BCMINT_XGS_L3_FIB_TBL_MC_FLD_CNT] = {
    [BCMINT_XGS_L3_FIB_TBL_MC_GROUP_0] = {
        MC_GROUP_LOWERs,
        L3_IPV6_MC_ROUTEt_MC_GROUPu_LOWERf,
        true,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_MC_GROUP_1] = {
        MC_GROUP_UPPERs,
        L3_IPV6_MC_ROUTEt_MC_GROUPu_UPPERf,
        true,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_MC_L3_IIF_ID] = {
        L3_MC_IIF_IDs,
        L3_IPV6_MC_ROUTEt_L3_MC_IIF_IDf,
        true,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_MC_VRF] = {
        VRF_IDs,
        L3_IPV6_MC_ROUTEt_VRF_IDf,
        true,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_MC_SIP_0_ADDR] = {
        SRC_IPV6_LOWERs,
        L3_IPV6_MC_ROUTEt_SRC_IPV6u_LOWERf,
        true,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_MC_SIP_1_ADDR] = {
        SRC_IPV6_UPPERs,
        L3_IPV6_MC_ROUTEt_SRC_IPV6u_UPPERf,
        true,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_MC_SIP_0_ADDR_MASK] = {
        SRC_IPV6_MASK_LOWERs,
        L3_IPV6_MC_ROUTEt_SRC_IPV6_MASKu_LOWERf,
        true,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_MC_SIP_1_ADDR_MASK] = {
        SRC_IPV6_MASK_UPPERs,
        L3_IPV6_MC_ROUTEt_SRC_IPV6_MASKu_UPPERf,
        true,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_MC_GROUP_ID] = {
        TM_MC_GROUP_IDs,
        L3_IPV6_MC_ROUTEt_TM_MC_GROUP_IDf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_MC_CLASS_ID] = {
        CLASS_IDs,
        L3_IPV6_MC_ROUTEt_CLASS_IDf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_MC_COPY_TO_CPU] = {
        COPY_TO_CPUs,
        L3_IPV6_MC_ROUTEt_COPY_TO_CPUf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_MC_DROP] = {
        DROPs,
        L3_IPV6_MC_ROUTEt_DROPf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_MC_DROP_ON_GROUP_MATCH] = {
        DROP_ON_GROUP_MATCHs,
        L3_IPV6_MC_ROUTEt_DROP_ON_GROUP_MATCHf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_MC_USE_PORT_TRUNK_ID] = {
        USE_PORT_TRUNK_IDs,
        L3_IPV6_MC_ROUTEt_USE_PORT_TRUNK_IDf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_MC_IS_TRUNK] = {
        IS_TRUNKs,
        L3_IPV6_MC_ROUTEt_IS_TRUNKf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_MC_TRUNK_ID] = {
        TRUNK_IDs,
        L3_IPV6_MC_ROUTEt_TRUNK_IDf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_MC_PORT] = {
        MODPORTs,
        L3_IPV6_MC_ROUTEt_MODPORTf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_MC_EXPECTED_L3_IIF_ID] = {
        EXPECTED_L3_MC_IIF_IDs,
        L3_IPV6_MC_ROUTEt_EXPECTED_L3_MC_IIF_IDf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_MC_DROP_SRC_IPV6_LINK_LOCAL] = {
        DROP_SRC_IPV6_LINK_LOCALs,
        L3_IPV6_MC_ROUTEt_DROP_SRC_IPV6_LINK_LOCALf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_MC_FLEX_CTR_OBJECT] = {
        CTR_ING_EFLEX_OBJECTs,
        L3_IPV6_MC_ROUTEt_CTR_ING_EFLEX_OBJECTf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_MC_FLEX_CTR_ACTION_ID] = {
        L3_IPV6_MC_CTR_ING_EFLEX_ACTION_IDs,
        L3_IPV6_MC_ROUTEt_L3_IPV6_MC_CTR_ING_EFLEX_ACTION_IDf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_XGS_L3_FIB_TBL_MC_HIT] = {
        HITs,
        L3_IPV6_MC_ROUTEt_HITf,
        false,
        false,
        0,
        NULL,
        NULL
    },
};

/*
 * L3_ALPM_LEVEL_1_USAGE
 */
static const
bcmint_ltsw_l3_fib_fld_t l3_alpm_lvl_1_usage_flds[BCMINT_LTSW_L3_ALPM_USAGE_TBL_FLD_CNT] = {
    [BCMINT_LTSW_L3_ALPM_USAGE_TBL_KEY_TYPE] = {
        KEY_TYPEs,
        L3_ALPM_LEVEL_1_USAGEt_KEY_TYPEf,
        true,
        true,
        0,
        key_type_symbol_to_scalar,
        key_type_scalar_to_symbol
    },
    [BCMINT_LTSW_L3_ALPM_USAGE_TBL_MAX_ENT] = {
        MAX_ENTRIESs,
        L3_ALPM_LEVEL_1_USAGEt_MAX_ENTRIESf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_LTSW_L3_ALPM_USAGE_TBL_INUSE_ENT] = {
        INUSE_ENTRIESs,
        L3_ALPM_LEVEL_1_USAGEt_INUSE_ENTRIESf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_LTSW_L3_ALPM_USAGE_TBL_ENT_UTILIZATION] = {
        ENTRY_UTILIZATIONs,
        L3_ALPM_LEVEL_1_USAGEt_ENTRY_UTILIZATIONf,
        false,
        false,
        0,
        NULL,
        NULL
    }
};

/*
 * L3_ALPM_LEVEL_2_USAGE
 */
static const
bcmint_ltsw_l3_fib_fld_t l3_alpm_lvl_2_usage_flds[BCMINT_LTSW_L3_ALPM_USAGE_TBL_FLD_CNT] = {
    [BCMINT_LTSW_L3_ALPM_USAGE_TBL_DB] = {
        DBs,
        L3_ALPM_LEVEL_2_USAGEt_DBf,
        true,
        true,
        0,
        db_symbol_to_scalar,
        db_scalar_to_symbol
    },
    [BCMINT_LTSW_L3_ALPM_USAGE_TBL_MAX_ENT] = {
        MAX_ENTRIESs,
        L3_ALPM_LEVEL_2_USAGEt_MAX_ENTRIESf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_LTSW_L3_ALPM_USAGE_TBL_INUSE_ENT] = {
        INUSE_ENTRIESs,
        L3_ALPM_LEVEL_2_USAGEt_INUSE_ENTRIESf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_LTSW_L3_ALPM_USAGE_TBL_ENT_UTILIZATION] = {
        ENTRY_UTILIZATIONs,
        L3_ALPM_LEVEL_2_USAGEt_ENTRY_UTILIZATIONf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_LTSW_L3_ALPM_USAGE_TBL_MAX_RBKT] = {
        MAX_RAW_BUCKETSs,
        L3_ALPM_LEVEL_2_USAGEt_MAX_RAW_BUCKETSf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_LTSW_L3_ALPM_USAGE_TBL_INUSE_RBKT] = {
        INUSE_RAW_BUCKETSs,
        L3_ALPM_LEVEL_2_USAGEt_INUSE_RAW_BUCKETSf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_LTSW_L3_ALPM_USAGE_TBL_RBKT_UTILIZATION] = {
        RAW_BUCKET_UTILIZATIONs,
        L3_ALPM_LEVEL_2_USAGEt_RAW_BUCKET_UTILIZATIONf,
        false,
        false,
        0,
        NULL,
        NULL
    }
};

/*
 * L3_ALPM_LEVEL_3_USAGE
 */
static const
bcmint_ltsw_l3_fib_fld_t l3_alpm_lvl_3_usage_flds[BCMINT_LTSW_L3_ALPM_USAGE_TBL_FLD_CNT] = {
    [BCMINT_LTSW_L3_ALPM_USAGE_TBL_DB] = {
        DBs,
        L3_ALPM_LEVEL_3_USAGEt_DBf,
        true,
        true,
        0,
        db_symbol_to_scalar,
        db_scalar_to_symbol
    },
    [BCMINT_LTSW_L3_ALPM_USAGE_TBL_MAX_ENT] = {
        MAX_ENTRIESs,
        L3_ALPM_LEVEL_3_USAGEt_MAX_ENTRIESf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_LTSW_L3_ALPM_USAGE_TBL_INUSE_ENT] = {
        INUSE_ENTRIESs,
        L3_ALPM_LEVEL_3_USAGEt_INUSE_ENTRIESf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_LTSW_L3_ALPM_USAGE_TBL_ENT_UTILIZATION] = {
        ENTRY_UTILIZATIONs,
        L3_ALPM_LEVEL_3_USAGEt_ENTRY_UTILIZATIONf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_LTSW_L3_ALPM_USAGE_TBL_MAX_RBKT] = {
        MAX_RAW_BUCKETSs,
        L3_ALPM_LEVEL_3_USAGEt_MAX_RAW_BUCKETSf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_LTSW_L3_ALPM_USAGE_TBL_INUSE_RBKT] = {
        INUSE_RAW_BUCKETSs,
        L3_ALPM_LEVEL_3_USAGEt_INUSE_RAW_BUCKETSf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_LTSW_L3_ALPM_USAGE_TBL_RBKT_UTILIZATION] = {
        RAW_BUCKET_UTILIZATIONs,
        L3_ALPM_LEVEL_3_USAGEt_RAW_BUCKET_UTILIZATIONf,
        false,
        false,
        0,
        NULL,
        NULL
    }
};

/*
 * L3_ALPM_CONTROL
 */
static const
bcmint_ltsw_l3_fib_fld_t l3_alpm_control_flds[BCMINT_LTSW_L3_ALPM_CTRL_TBL_FLD_CNT] = {
    [BCMINT_LTSW_L3_ALPM_CTRL_TBL_OP_STATE] = {
        OPERATIONAL_STATEs,
        L3_ALPM_CONTROLt_OPERATIONAL_STATEf,
        false,
        true,
        0,
        op_state_symbol_to_scalar,
        op_state_scalar_to_symbol
    },
    [BCMINT_LTSW_L3_ALPM_CTRL_TBL_NUM_DB_0_LVLS] = {
        NUM_DB_0_LEVELSs,
        L3_ALPM_CONTROLt_NUM_DB_0_LEVELSf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_LTSW_L3_ALPM_CTRL_TBL_NUM_DB_1_LVLS] = {
        NUM_DB_1_LEVELSs,
        L3_ALPM_CONTROLt_NUM_DB_1_LEVELSf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_LTSW_L3_ALPM_CTRL_TBL_NUM_DB_2_LVLS] = {
        NUM_DB_2_LEVELSs,
        L3_ALPM_CONTROLt_NUM_DB_2_LEVELSf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_LTSW_L3_ALPM_CTRL_TBL_NUM_DB_3_LVLS] = {
        NUM_DB_3_LEVELSs,
        L3_ALPM_CONTROLt_NUM_DB_3_LEVELSf,
        false,
        false,
        0,
        NULL,
        NULL
    },
    [BCMINT_LTSW_L3_ALPM_CTRL_TBL_KEY_INPUT_LVL_1_BLK_0] = {
        KEY_INPUT_LEVEL_1_BLOCK_0s,
        L3_ALPM_CONTROLt_KEY_INPUT_LEVEL_1_BLOCK_0f,
        false,
        true,
        0,
        key_input_symbol_to_scalar,
        key_input_scalar_to_symbol
    },
    [BCMINT_LTSW_L3_ALPM_CTRL_TBL_KEY_INPUT_LVL_1_BLK_1] = {
        KEY_INPUT_LEVEL_1_BLOCK_1s,
        L3_ALPM_CONTROLt_KEY_INPUT_LEVEL_1_BLOCK_1f,
        false,
        true,
        0,
        key_input_symbol_to_scalar,
        key_input_scalar_to_symbol
    },
    [BCMINT_LTSW_L3_ALPM_CTRL_TBL_KEY_INPUT_LVL_1_BLK_2] = {
        KEY_INPUT_LEVEL_1_BLOCK_2s,
        L3_ALPM_CONTROLt_KEY_INPUT_LEVEL_1_BLOCK_2f,
        false,
        true,
        0,
        key_input_symbol_to_scalar,
        key_input_scalar_to_symbol
    },
    [BCMINT_LTSW_L3_ALPM_CTRL_TBL_KEY_INPUT_LVL_1_BLK_3] = {
        KEY_INPUT_LEVEL_1_BLOCK_3s,
        L3_ALPM_CONTROLt_KEY_INPUT_LEVEL_1_BLOCK_3f,
        false,
        true,
        0,
        key_input_symbol_to_scalar,
        key_input_scalar_to_symbol
    },
    [BCMINT_LTSW_L3_ALPM_CTRL_TBL_KEY_INPUT_LVL_1_BLK_4] = {
        KEY_INPUT_LEVEL_1_BLOCK_4s,
        L3_ALPM_CONTROLt_KEY_INPUT_LEVEL_1_BLOCK_4f,
        false,
        true,
        0,
        key_input_symbol_to_scalar,
        key_input_scalar_to_symbol
    },
    [BCMINT_LTSW_L3_ALPM_CTRL_TBL_KEY_INPUT_LVL_1_BLK_5] = {
        KEY_INPUT_LEVEL_1_BLOCK_5s,
        L3_ALPM_CONTROLt_KEY_INPUT_LEVEL_1_BLOCK_5f,
        false,
        true,
        0,
        key_input_symbol_to_scalar,
        key_input_scalar_to_symbol
    },
    [BCMINT_LTSW_L3_ALPM_CTRL_TBL_KEY_INPUT_LVL_1_BLK_6] = {
        KEY_INPUT_LEVEL_1_BLOCK_6s,
        L3_ALPM_CONTROLt_KEY_INPUT_LEVEL_1_BLOCK_6f,
        false,
        true,
        0,
        key_input_symbol_to_scalar,
        key_input_scalar_to_symbol
    },
    [BCMINT_LTSW_L3_ALPM_CTRL_TBL_KEY_INPUT_LVL_1_BLK_7] = {
        KEY_INPUT_LEVEL_1_BLOCK_7s,
        L3_ALPM_CONTROLt_KEY_INPUT_LEVEL_1_BLOCK_7f,
        false,
        true,
        0,
        key_input_symbol_to_scalar,
        key_input_scalar_to_symbol
    },
    [BCMINT_LTSW_L3_ALPM_CTRL_TBL_KEY_INPUT_LVL_1_BLK_8] = {
        KEY_INPUT_LEVEL_1_BLOCK_8s,
        L3_ALPM_CONTROLt_KEY_INPUT_LEVEL_1_BLOCK_8f,
        false,
        true,
        0,
        key_input_symbol_to_scalar,
        key_input_scalar_to_symbol
    },
    [BCMINT_LTSW_L3_ALPM_CTRL_TBL_KEY_INPUT_LVL_1_BLK_9] = {
        KEY_INPUT_LEVEL_1_BLOCK_9s,
        L3_ALPM_CONTROLt_KEY_INPUT_LEVEL_1_BLOCK_9f,
        false,
        true,
        0,
        key_input_symbol_to_scalar,
        key_input_scalar_to_symbol
    },
    [BCMINT_LTSW_L3_ALPM_CTRL_TBL_DB_LVL_1_BLK_0] = {
        DB_LEVEL_1_BLOCK_0s,
        L3_ALPM_CONTROLt_DB_LEVEL_1_BLOCK_0f,
        false,
        true,
        0,
        db_symbol_to_scalar,
        db_scalar_to_symbol
    },
    [BCMINT_LTSW_L3_ALPM_CTRL_TBL_DB_LVL_1_BLK_1] = {
        DB_LEVEL_1_BLOCK_1s,
        L3_ALPM_CONTROLt_DB_LEVEL_1_BLOCK_1f,
        false,
        true,
        0,
        db_symbol_to_scalar,
        db_scalar_to_symbol
    },
    [BCMINT_LTSW_L3_ALPM_CTRL_TBL_DB_LVL_1_BLK_2] = {
        DB_LEVEL_1_BLOCK_2s,
        L3_ALPM_CONTROLt_DB_LEVEL_1_BLOCK_2f,
        false,
        true,
        0,
        db_symbol_to_scalar,
        db_scalar_to_symbol
    },
    [BCMINT_LTSW_L3_ALPM_CTRL_TBL_DB_LVL_1_BLK_3] = {
        DB_LEVEL_1_BLOCK_3s,
        L3_ALPM_CONTROLt_DB_LEVEL_1_BLOCK_3f,
        false,
        true,
        0,
        db_symbol_to_scalar,
        db_scalar_to_symbol
    },
    [BCMINT_LTSW_L3_ALPM_CTRL_TBL_DB_LVL_1_BLK_4] = {
        DB_LEVEL_1_BLOCK_4s,
        L3_ALPM_CONTROLt_DB_LEVEL_1_BLOCK_4f,
        false,
        true,
        0,
        db_symbol_to_scalar,
        db_scalar_to_symbol
    },
    [BCMINT_LTSW_L3_ALPM_CTRL_TBL_DB_LVL_1_BLK_5] = {
        DB_LEVEL_1_BLOCK_5s,
        L3_ALPM_CONTROLt_DB_LEVEL_1_BLOCK_5f,
        false,
        true,
        0,
        db_symbol_to_scalar,
        db_scalar_to_symbol
    },
    [BCMINT_LTSW_L3_ALPM_CTRL_TBL_DB_LVL_1_BLK_6] = {
        DB_LEVEL_1_BLOCK_6s,
        L3_ALPM_CONTROLt_DB_LEVEL_1_BLOCK_6f,
        false,
        true,
        0,
        db_symbol_to_scalar,
        db_scalar_to_symbol
    },
    [BCMINT_LTSW_L3_ALPM_CTRL_TBL_DB_LVL_1_BLK_7] = {
        DB_LEVEL_1_BLOCK_7s,
        L3_ALPM_CONTROLt_DB_LEVEL_1_BLOCK_7f,
        false,
        true,
        0,
        db_symbol_to_scalar,
        db_scalar_to_symbol
    },
    [BCMINT_LTSW_L3_ALPM_CTRL_TBL_DB_LVL_1_BLK_8] = {
        DB_LEVEL_1_BLOCK_8s,
        L3_ALPM_CONTROLt_DB_LEVEL_1_BLOCK_8f,
        false,
        true,
        0,
        db_symbol_to_scalar,
        db_scalar_to_symbol
    },
    [BCMINT_LTSW_L3_ALPM_CTRL_TBL_DB_LVL_1_BLK_9] = {
        DB_LEVEL_1_BLOCK_9s,
        L3_ALPM_CONTROLt_DB_LEVEL_1_BLOCK_9f,
        false,
        true,
        0,
        db_symbol_to_scalar,
        db_scalar_to_symbol
    },
    [BCMINT_LTSW_L3_ALPM_CTRL_TBL_HIT_MODE] = {
        HIT_MODEs,
        L3_ALPM_CONTROLt_HIT_MODEf,
        false,
        false,
        0,
        NULL,
        NULL
    },
};

static const bcmint_ltsw_l3_fib_tbl_t l3_fib_tbls[] = {
    [BCMINT_LTSW_L3_FIB_TBL_DEFIP_IPV4_MC] = {
        .name = L3_IPV4_MC_ROUTEs,
        .tbl_id = L3_IPV4_MC_ROUTEt,
        .attr = BCMINT_LTSW_L3_FIB_TBL_ATTR_V4 |
                BCMINT_LTSW_L3_FIB_TBL_ATTR_MC |
                BCMINT_LTSW_L3_FIB_TBL_ATTR_SUBNET,
        .multiple = 1,
        .rm_type = BCMINT_LTSW_L3_FIB_TBL_RM_T_ALPM,
        .fld_cnt = L3_IPV4_MC_ROUTEt_FIELD_COUNT,
        .fld_bmp = (1 << BCMINT_XGS_L3_FIB_TBL_MC_GROUP_0) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_MC_L3_IIF_ID) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_MC_VRF) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_MC_SIP_0_ADDR) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_MC_SIP_0_ADDR_MASK) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_MC_GROUP_ID) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_MC_CLASS_ID) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_MC_COPY_TO_CPU) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_MC_DROP) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_MC_DROP_ON_GROUP_MATCH) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_MC_USE_PORT_TRUNK_ID) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_MC_IS_TRUNK) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_MC_PORT) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_MC_TRUNK_ID) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_MC_EXPECTED_L3_IIF_ID) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_MC_FLEX_CTR_OBJECT) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_MC_FLEX_CTR_ACTION_ID) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_MC_HIT),
        .flds = l3_ipv4_mc_g_flds
    },
    [BCMINT_LTSW_L3_FIB_TBL_DEFIP_IPV6_MC_128] = {
        .name = L3_IPV6_MC_ROUTEs,
        .tbl_id = L3_IPV6_MC_ROUTEt,
        .attr = BCMINT_LTSW_L3_FIB_TBL_ATTR_V6 |
                BCMINT_LTSW_L3_FIB_TBL_ATTR_MC |
                BCMINT_LTSW_L3_FIB_TBL_ATTR_SUBNET,
        .multiple = 2,
        .rm_type = BCMINT_LTSW_L3_FIB_TBL_RM_T_ALPM,
        .fld_cnt = L3_IPV6_MC_ROUTEt_FIELD_COUNT,
        .fld_bmp = (1 << BCMINT_XGS_L3_FIB_TBL_MC_GROUP_0) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_MC_GROUP_1) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_MC_L3_IIF_ID) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_MC_VRF) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_MC_SIP_0_ADDR) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_MC_SIP_0_ADDR_MASK) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_MC_SIP_1_ADDR) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_MC_SIP_1_ADDR_MASK) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_MC_GROUP_ID) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_MC_CLASS_ID) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_MC_COPY_TO_CPU) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_MC_DROP) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_MC_DROP_ON_GROUP_MATCH) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_MC_USE_PORT_TRUNK_ID) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_MC_IS_TRUNK) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_MC_PORT) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_MC_TRUNK_ID) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_MC_EXPECTED_L3_IIF_ID) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_MC_FLEX_CTR_OBJECT) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_MC_FLEX_CTR_ACTION_ID) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_MC_DROP_SRC_IPV6_LINK_LOCAL) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_MC_HIT),
        .flds = l3_ipv6_mc_g_flds
    },
    [BCMINT_LTSW_L3_FIB_TBL_ROUTE_IPV4_UC_GL] = {
        .name = L3_IPV4_UC_ROUTEs,
        .tbl_id = L3_IPV4_UC_ROUTEt,
        .attr = BCMINT_LTSW_L3_FIB_TBL_ATTR_V4 |
                BCMINT_LTSW_L3_FIB_TBL_ATTR_UC |
                BCMINT_LTSW_L3_FIB_TBL_ATTR_GL |
                BCMINT_LTSW_L3_FIB_TBL_ATTR_SUBNET,
        .multiple = 1,
        .rm_type = BCMINT_LTSW_L3_FIB_TBL_RM_T_ALPM,
        .fld_cnt = L3_IPV4_UC_ROUTEt_FIELD_COUNT,
        .fld_bmp = (1 << BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_0) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_0_MASK) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_ECMP_NHOP) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_ECMP_ID) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_NHOP_ID) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_REPLACE_INT_PRI) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_INT_PRI) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_DROP_ON_DIP_MATCH) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_CLASS_ID) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_FLEX_CTR_OBJECT) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_FLEX_CTR_ACTION_ID) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_INT_ACT_PROFILE_ID) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_HIT),
        .flds = l3_ipv4_uc_gl_flds
    },
    [BCMINT_LTSW_L3_FIB_TBL_ROUTE_IPV4_UC_VRF] = {
        .name = L3_IPV4_UC_ROUTE_VRFs,
        .tbl_id = L3_IPV4_UC_ROUTE_VRFt,
        .attr = BCMINT_LTSW_L3_FIB_TBL_ATTR_V4 |
                BCMINT_LTSW_L3_FIB_TBL_ATTR_UC |
                BCMINT_LTSW_L3_FIB_TBL_ATTR_VRF |
                BCMINT_LTSW_L3_FIB_TBL_ATTR_SUBNET,
        .multiple = 1,
        .rm_type = BCMINT_LTSW_L3_FIB_TBL_RM_T_ALPM,
        .fld_cnt = L3_IPV4_UC_ROUTE_VRFt_FIELD_COUNT,
        .fld_bmp = (1 << BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_0) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_0_MASK) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_VRF) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_ECMP_NHOP) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_ECMP_ID) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_NHOP_ID) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_REPLACE_INT_PRI) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_INT_PRI) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_DROP_ON_DIP_MATCH) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_CLASS_ID) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_FLEX_CTR_OBJECT) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_FLEX_CTR_ACTION_ID) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_INT_ACT_PROFILE_ID) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_HIT),
        .flds = l3_ipv4_uc_vrf_flds
    },
    [BCMINT_LTSW_L3_FIB_TBL_HOST_IPV4_UC] = {
        .name = L3_IPV4_UC_ROUTE_VRFs,
        .tbl_id = L3_IPV4_UC_ROUTE_VRFt,
        .attr = BCMINT_LTSW_L3_FIB_TBL_ATTR_V4 |
                BCMINT_LTSW_L3_FIB_TBL_ATTR_UC |
                BCMINT_LTSW_L3_FIB_TBL_ATTR_VRF |
                BCMINT_LTSW_L3_FIB_TBL_ATTR_SUBNET |
                BCMINT_LTSW_L3_FIB_TBL_ATTR_HOST,
        .multiple = 1,
        .rm_type = BCMINT_LTSW_L3_FIB_TBL_RM_T_ALPM,
        .fld_cnt = L3_IPV4_UC_ROUTE_VRFt_FIELD_COUNT,
        .fld_bmp = (1 << BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_0) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_0_MASK) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_VRF) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_ECMP_NHOP) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_ECMP_ID) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_NHOP_ID) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_REPLACE_INT_PRI) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_INT_PRI) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_DROP_ON_DIP_MATCH) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_CLASS_ID) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_FLEX_CTR_OBJECT) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_FLEX_CTR_ACTION_ID) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_INT_ACT_PROFILE_ID) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_HIT),
        .flds = l3_ipv4_uc_vrf_flds
    },
    [BCMINT_LTSW_L3_FIB_TBL_ROUTE_IPV4_UC_GH] = {
        .name = L3_IPV4_UC_ROUTE_OVERRIDEs,
        .tbl_id = L3_IPV4_UC_ROUTE_OVERRIDEt,
        .attr = BCMINT_LTSW_L3_FIB_TBL_ATTR_V4 |
                BCMINT_LTSW_L3_FIB_TBL_ATTR_UC |
                BCMINT_LTSW_L3_FIB_TBL_ATTR_GH |
                BCMINT_LTSW_L3_FIB_TBL_ATTR_SUBNET,
        .multiple = 1,
        .rm_type = BCMINT_LTSW_L3_FIB_TBL_RM_T_ALPM,
        .fld_cnt = L3_IPV4_UC_ROUTE_OVERRIDEt_FIELD_COUNT,
        .fld_bmp = (1 << BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_0) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_0_MASK) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_ECMP_NHOP) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_ECMP_ID) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_NHOP_ID) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_REPLACE_INT_PRI) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_INT_PRI) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_DROP_ON_DIP_MATCH) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_CLASS_ID) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_FLEX_CTR_OBJECT) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_FLEX_CTR_ACTION_ID) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_INT_ACT_PROFILE_ID) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_HIT),
        .flds = l3_ipv4_uc_gh_flds
    },
    [BCMINT_LTSW_L3_FIB_TBL_ROUTE_IPV6_UC_GL] = {
        .name = L3_IPV6_UC_ROUTEs,
        .tbl_id = L3_IPV6_UC_ROUTEt,
        .attr = BCMINT_LTSW_L3_FIB_TBL_ATTR_V6 |
                BCMINT_LTSW_L3_FIB_TBL_ATTR_UC |
                BCMINT_LTSW_L3_FIB_TBL_ATTR_GL |
                BCMINT_LTSW_L3_FIB_TBL_ATTR_SUBNET,
        .multiple = 4,
        .rm_type = BCMINT_LTSW_L3_FIB_TBL_RM_T_ALPM,
        .fld_cnt = L3_IPV6_UC_ROUTE_OVERRIDEt_FIELD_COUNT,
        .fld_bmp = (1 << BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_0) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_0_MASK) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_1) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_1_MASK) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_ECMP_NHOP) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_ECMP_ID) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_NHOP_ID) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_REPLACE_INT_PRI) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_INT_PRI) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_DROP_ON_DIP_MATCH) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_CLASS_ID) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_FLEX_CTR_OBJECT) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_FLEX_CTR_ACTION_ID) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_INT_ACT_PROFILE_ID) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_HIT),
        .flds = l3_ipv6_uc_gl_flds
    },
    [BCMINT_LTSW_L3_FIB_TBL_ROUTE_IPV6_UC_VRF] = {
        .name = L3_IPV6_UC_ROUTE_VRFs,
        .tbl_id = L3_IPV6_UC_ROUTE_VRFt,
        .attr = BCMINT_LTSW_L3_FIB_TBL_ATTR_V6 |
                BCMINT_LTSW_L3_FIB_TBL_ATTR_UC |
                BCMINT_LTSW_L3_FIB_TBL_ATTR_VRF |
                BCMINT_LTSW_L3_FIB_TBL_ATTR_SUBNET,
        .multiple = 4,
        .rm_type = BCMINT_LTSW_L3_FIB_TBL_RM_T_ALPM,
        .fld_cnt = L3_IPV6_UC_ROUTE_VRFt_FIELD_COUNT,
        .fld_bmp = (1 << BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_0) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_0_MASK) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_1) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_1_MASK) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_VRF) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_ECMP_NHOP) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_ECMP_ID) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_NHOP_ID) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_REPLACE_INT_PRI) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_INT_PRI) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_DROP_ON_DIP_MATCH) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_CLASS_ID) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_FLEX_CTR_OBJECT) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_FLEX_CTR_ACTION_ID) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_INT_ACT_PROFILE_ID) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_HIT),
        .flds = l3_ipv6_uc_vrf_flds
    },
    [BCMINT_LTSW_L3_FIB_TBL_HOST_IPV6_UC] = {
        .name = L3_IPV6_UC_ROUTE_VRFs,
        .tbl_id = L3_IPV6_UC_ROUTE_VRFt,
        .attr = BCMINT_LTSW_L3_FIB_TBL_ATTR_V6 |
                BCMINT_LTSW_L3_FIB_TBL_ATTR_UC |
                BCMINT_LTSW_L3_FIB_TBL_ATTR_VRF |
                BCMINT_LTSW_L3_FIB_TBL_ATTR_SUBNET |
                BCMINT_LTSW_L3_FIB_TBL_ATTR_HOST,
        .multiple = 4,
        .rm_type = BCMINT_LTSW_L3_FIB_TBL_RM_T_ALPM,
        .fld_cnt = L3_IPV6_UC_ROUTE_VRFt_FIELD_COUNT,
        .fld_bmp = (1 << BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_0) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_0_MASK) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_1) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_1_MASK) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_VRF) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_ECMP_NHOP) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_ECMP_ID) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_NHOP_ID) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_REPLACE_INT_PRI) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_INT_PRI) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_DROP_ON_DIP_MATCH) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_CLASS_ID) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_FLEX_CTR_OBJECT) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_FLEX_CTR_ACTION_ID) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_INT_ACT_PROFILE_ID) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_HIT),
        .flds = l3_ipv6_uc_vrf_flds
    },
    [BCMINT_LTSW_L3_FIB_TBL_ROUTE_IPV6_UC_GH] = {
        .name = L3_IPV6_UC_ROUTE_OVERRIDEs,
        .tbl_id = L3_IPV6_UC_ROUTE_OVERRIDEt,
        .attr = BCMINT_LTSW_L3_FIB_TBL_ATTR_V6 |
                BCMINT_LTSW_L3_FIB_TBL_ATTR_UC |
                BCMINT_LTSW_L3_FIB_TBL_ATTR_GH |
                BCMINT_LTSW_L3_FIB_TBL_ATTR_SUBNET,
        .multiple = 4,
        .rm_type = BCMINT_LTSW_L3_FIB_TBL_RM_T_ALPM,
        .fld_cnt = L3_IPV6_UC_ROUTE_OVERRIDEt_FIELD_COUNT,
        .fld_bmp = (1 << BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_0) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_0_MASK) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_1) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_1_MASK) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_ECMP_NHOP) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_ECMP_ID) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_NHOP_ID) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_REPLACE_INT_PRI) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_INT_PRI) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_DROP_ON_DIP_MATCH) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_CLASS_ID) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_FLEX_CTR_OBJECT) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_FLEX_CTR_ACTION_ID) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_INT_ACT_PROFILE_ID) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_HIT),
        .flds = l3_ipv6_uc_gh_flds
    },
    [BCMINT_LTSW_L3_FIB_TBL_ROUTE_SRC_IPV4_UC_GL] = {
        .name = L3_SRC_IPV4_UC_ROUTEs,
        .tbl_id = L3_SRC_IPV4_UC_ROUTEt,
        .attr = BCMINT_LTSW_L3_FIB_TBL_ATTR_V4 |
                BCMINT_LTSW_L3_FIB_TBL_ATTR_UC |
                BCMINT_LTSW_L3_FIB_TBL_ATTR_GL |
                BCMINT_LTSW_L3_FIB_TBL_ATTR_SUBNET |
                BCMINT_LTSW_L3_FIB_TBL_ATTR_SRC,
        .multiple = 1,
        .rm_type = BCMINT_LTSW_L3_FIB_TBL_RM_T_ALPM,
        .fld_cnt = L3_SRC_IPV4_UC_ROUTEt_FIELD_COUNT,
        .fld_bmp = (1 << BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_0) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_0_MASK) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_CLASS_ID) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_FLEX_CTR_OBJECT) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_FLEX_CTR_ACTION_ID) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_HIT),
        .flds = l3_src_ipv4_uc_gl_flds
    },
    [BCMINT_LTSW_L3_FIB_TBL_ROUTE_SRC_IPV4_UC_VRF] = {
        .name = L3_SRC_IPV4_UC_ROUTE_VRFs,
        .tbl_id = L3_SRC_IPV4_UC_ROUTE_VRFt,
        .attr = BCMINT_LTSW_L3_FIB_TBL_ATTR_V4 |
                BCMINT_LTSW_L3_FIB_TBL_ATTR_UC |
                BCMINT_LTSW_L3_FIB_TBL_ATTR_VRF |
                BCMINT_LTSW_L3_FIB_TBL_ATTR_SRC |
                BCMINT_LTSW_L3_FIB_TBL_ATTR_SUBNET,
        .multiple = 1,
        .rm_type = BCMINT_LTSW_L3_FIB_TBL_RM_T_ALPM,
        .fld_cnt = L3_SRC_IPV4_UC_ROUTE_VRFt_FIELD_COUNT,
        .fld_bmp = (1 << BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_0) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_0_MASK) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_VRF) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_CLASS_ID) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_FLEX_CTR_OBJECT) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_FLEX_CTR_ACTION_ID) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_HIT),
        .flds = l3_src_ipv4_uc_vrf_flds
    },
    [BCMINT_LTSW_L3_FIB_TBL_ROUTE_SRC_IPV4_UC_GH] = {
        .name = L3_SRC_IPV4_UC_ROUTE_OVERRIDEs,
        .tbl_id = L3_SRC_IPV4_UC_ROUTE_OVERRIDEt,
        .attr = BCMINT_LTSW_L3_FIB_TBL_ATTR_V4 |
                BCMINT_LTSW_L3_FIB_TBL_ATTR_UC |
                BCMINT_LTSW_L3_FIB_TBL_ATTR_GH |
                BCMINT_LTSW_L3_FIB_TBL_ATTR_SRC |
                BCMINT_LTSW_L3_FIB_TBL_ATTR_SUBNET,
        .multiple = 1,
        .rm_type = BCMINT_LTSW_L3_FIB_TBL_RM_T_ALPM,
        .fld_cnt = L3_SRC_IPV4_UC_ROUTE_OVERRIDEt_FIELD_COUNT,
        .fld_bmp = (1 << BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_0) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_0_MASK) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_CLASS_ID) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_FLEX_CTR_OBJECT) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_FLEX_CTR_ACTION_ID) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_HIT),
        .flds = l3_src_ipv4_uc_gh_flds
    },
    [BCMINT_LTSW_L3_FIB_TBL_ROUTE_SRC_IPV6_UC_GL] = {
        .name = L3_SRC_IPV6_UC_ROUTEs,
        .tbl_id = L3_SRC_IPV6_UC_ROUTEt,
        .attr = BCMINT_LTSW_L3_FIB_TBL_ATTR_V6 |
                BCMINT_LTSW_L3_FIB_TBL_ATTR_UC |
                BCMINT_LTSW_L3_FIB_TBL_ATTR_GL |
                BCMINT_LTSW_L3_FIB_TBL_ATTR_SRC |
                BCMINT_LTSW_L3_FIB_TBL_ATTR_SUBNET,
        .multiple = 4,
        .rm_type = BCMINT_LTSW_L3_FIB_TBL_RM_T_ALPM,
        .fld_cnt = L3_SRC_IPV6_UC_ROUTE_OVERRIDEt_FIELD_COUNT,
        .fld_bmp = (1 << BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_0) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_0_MASK) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_1) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_1_MASK) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_CLASS_ID) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_FLEX_CTR_OBJECT) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_FLEX_CTR_ACTION_ID) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_HIT),
        .flds = l3_src_ipv6_uc_gl_flds
    },
    [BCMINT_LTSW_L3_FIB_TBL_ROUTE_SRC_IPV6_UC_VRF] = {
        .name = L3_SRC_IPV6_UC_ROUTE_VRFs,
        .tbl_id = L3_SRC_IPV6_UC_ROUTE_VRFt,
        .attr = BCMINT_LTSW_L3_FIB_TBL_ATTR_V6 |
                BCMINT_LTSW_L3_FIB_TBL_ATTR_UC |
                BCMINT_LTSW_L3_FIB_TBL_ATTR_VRF |
                BCMINT_LTSW_L3_FIB_TBL_ATTR_SRC |
                BCMINT_LTSW_L3_FIB_TBL_ATTR_SUBNET,
        .multiple = 4,
        .rm_type = BCMINT_LTSW_L3_FIB_TBL_RM_T_ALPM,
        .fld_cnt = L3_SRC_IPV6_UC_ROUTE_VRFt_FIELD_COUNT,
        .fld_bmp = (1 << BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_0) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_0_MASK) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_1) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_1_MASK) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_VRF) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_CLASS_ID) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_FLEX_CTR_OBJECT) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_FLEX_CTR_ACTION_ID) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_HIT),
        .flds = l3_src_ipv6_uc_vrf_flds
    },
    [BCMINT_LTSW_L3_FIB_TBL_ROUTE_SRC_IPV6_UC_GH] = {
        .name = L3_SRC_IPV6_UC_ROUTE_OVERRIDEs,
        .tbl_id = L3_SRC_IPV6_UC_ROUTE_OVERRIDEt,
        .attr = BCMINT_LTSW_L3_FIB_TBL_ATTR_V6 |
                BCMINT_LTSW_L3_FIB_TBL_ATTR_UC |
                BCMINT_LTSW_L3_FIB_TBL_ATTR_GH |
                BCMINT_LTSW_L3_FIB_TBL_ATTR_SRC |
                BCMINT_LTSW_L3_FIB_TBL_ATTR_SUBNET,
        .multiple = 4,
        .rm_type = BCMINT_LTSW_L3_FIB_TBL_RM_T_ALPM,
        .fld_cnt = L3_SRC_IPV6_UC_ROUTE_OVERRIDEt_FIELD_COUNT,
        .fld_bmp = (1 << BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_0) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_0_MASK) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_1) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_IP_ADDR_1_MASK) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_CLASS_ID) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_FLEX_CTR_OBJECT) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_FLEX_CTR_ACTION_ID) |
                   (1 << BCMINT_XGS_L3_FIB_TBL_UC_HIT),
        .flds = l3_src_ipv6_uc_gh_flds
    },
    [BCMINT_LTSW_L3_FIB_TBL_ALPM_LVL_1_USAGE] = {
        .name = L3_ALPM_LEVEL_1_USAGEs,
        .tbl_id = L3_ALPM_LEVEL_1_USAGEt,
        .attr = 0,
        .multiple = 0,
        .rm_type = 0,
        .fld_cnt = L3_ALPM_LEVEL_1_USAGEt_FIELD_COUNT,
        .fld_bmp = (1 << BCMINT_LTSW_L3_ALPM_USAGE_TBL_KEY_TYPE) |
                   (1 << BCMINT_LTSW_L3_ALPM_USAGE_TBL_MAX_ENT) |
                   (1 << BCMINT_LTSW_L3_ALPM_USAGE_TBL_INUSE_ENT) |
                   (1 << BCMINT_LTSW_L3_ALPM_USAGE_TBL_ENT_UTILIZATION),
        .flds = l3_alpm_lvl_1_usage_flds
    },
    [BCMINT_LTSW_L3_FIB_TBL_ALPM_LVL_2_USAGE] = {
        .name = L3_ALPM_LEVEL_2_USAGEs,
        .tbl_id = L3_ALPM_LEVEL_2_USAGEt,
        .attr = 0,
        .multiple = 0,
        .rm_type = 0,
        .fld_cnt = L3_ALPM_LEVEL_2_USAGEt_FIELD_COUNT,
        .fld_bmp = (1 << BCMINT_LTSW_L3_ALPM_USAGE_TBL_DB) |
                   (1 << BCMINT_LTSW_L3_ALPM_USAGE_TBL_MAX_ENT) |
                   (1 << BCMINT_LTSW_L3_ALPM_USAGE_TBL_INUSE_ENT) |
                   (1 << BCMINT_LTSW_L3_ALPM_USAGE_TBL_ENT_UTILIZATION) |
                   (1 << BCMINT_LTSW_L3_ALPM_USAGE_TBL_MAX_RBKT) |
                   (1 << BCMINT_LTSW_L3_ALPM_USAGE_TBL_INUSE_RBKT) |
                   (1 << BCMINT_LTSW_L3_ALPM_USAGE_TBL_RBKT_UTILIZATION),
        .flds = l3_alpm_lvl_2_usage_flds
    },
    [BCMINT_LTSW_L3_FIB_TBL_ALPM_LVL_3_USAGE] = {
        .name = L3_ALPM_LEVEL_3_USAGEs,
        .tbl_id = L3_ALPM_LEVEL_3_USAGEt,
        .attr = 0,
        .multiple = 0,
        .rm_type = 0,
        .fld_cnt = L3_ALPM_LEVEL_3_USAGEt_FIELD_COUNT,
        .fld_bmp = (1 << BCMINT_LTSW_L3_ALPM_USAGE_TBL_DB) |
                   (1 << BCMINT_LTSW_L3_ALPM_USAGE_TBL_MAX_ENT) |
                   (1 << BCMINT_LTSW_L3_ALPM_USAGE_TBL_INUSE_ENT) |
                   (1 << BCMINT_LTSW_L3_ALPM_USAGE_TBL_ENT_UTILIZATION) |
                   (1 << BCMINT_LTSW_L3_ALPM_USAGE_TBL_MAX_RBKT) |
                   (1 << BCMINT_LTSW_L3_ALPM_USAGE_TBL_INUSE_RBKT) |
                   (1 << BCMINT_LTSW_L3_ALPM_USAGE_TBL_RBKT_UTILIZATION),
        .flds = l3_alpm_lvl_3_usage_flds
    },
    [BCMINT_LTSW_L3_FIB_TBL_ALPM_CONTROL] = {
        .name = L3_ALPM_CONTROLs,
        .tbl_id = L3_ALPM_CONTROLt,
        .attr = 0,
        .multiple = 0,
        .rm_type = 0,
        .fld_cnt = L3_ALPM_CONTROLt_FIELD_COUNT,
        .fld_bmp = (1ULL << BCMINT_LTSW_L3_ALPM_CTRL_TBL_OP_STATE) |
                   (1ULL << BCMINT_LTSW_L3_ALPM_CTRL_TBL_NUM_DB_0_LVLS) |
                   (1ULL << BCMINT_LTSW_L3_ALPM_CTRL_TBL_NUM_DB_1_LVLS) |
                   (1ULL << BCMINT_LTSW_L3_ALPM_CTRL_TBL_NUM_DB_2_LVLS) |
                   (1ULL << BCMINT_LTSW_L3_ALPM_CTRL_TBL_NUM_DB_3_LVLS) |
                   (1ULL << BCMINT_LTSW_L3_ALPM_CTRL_TBL_KEY_INPUT_LVL_1_BLK_0) |
                   (1ULL << BCMINT_LTSW_L3_ALPM_CTRL_TBL_KEY_INPUT_LVL_1_BLK_1) |
                   (1ULL << BCMINT_LTSW_L3_ALPM_CTRL_TBL_KEY_INPUT_LVL_1_BLK_2) |
                   (1ULL << BCMINT_LTSW_L3_ALPM_CTRL_TBL_KEY_INPUT_LVL_1_BLK_3) |
                   (1ULL << BCMINT_LTSW_L3_ALPM_CTRL_TBL_KEY_INPUT_LVL_1_BLK_4) |
                   (1ULL << BCMINT_LTSW_L3_ALPM_CTRL_TBL_KEY_INPUT_LVL_1_BLK_5) |
                   (1ULL << BCMINT_LTSW_L3_ALPM_CTRL_TBL_KEY_INPUT_LVL_1_BLK_6) |
                   (1ULL << BCMINT_LTSW_L3_ALPM_CTRL_TBL_KEY_INPUT_LVL_1_BLK_7) |
                   (1ULL << BCMINT_LTSW_L3_ALPM_CTRL_TBL_DB_LVL_1_BLK_0) |
                   (1ULL << BCMINT_LTSW_L3_ALPM_CTRL_TBL_DB_LVL_1_BLK_1) |
                   (1ULL << BCMINT_LTSW_L3_ALPM_CTRL_TBL_DB_LVL_1_BLK_2) |
                   (1ULL << BCMINT_LTSW_L3_ALPM_CTRL_TBL_DB_LVL_1_BLK_3) |
                   (1ULL << BCMINT_LTSW_L3_ALPM_CTRL_TBL_DB_LVL_1_BLK_4) |
                   (1ULL << BCMINT_LTSW_L3_ALPM_CTRL_TBL_DB_LVL_1_BLK_5) |
                   (1ULL << BCMINT_LTSW_L3_ALPM_CTRL_TBL_DB_LVL_1_BLK_6) |
                   (1ULL << BCMINT_LTSW_L3_ALPM_CTRL_TBL_DB_LVL_1_BLK_7) |
                   (1ULL << BCMINT_LTSW_L3_ALPM_CTRL_TBL_HIT_MODE) |
                   (1ULL << BCMINT_LTSW_L3_ALPM_CTRL_TBL_LARGE_VRF),
        .flds = l3_alpm_control_flds
    }
};

/******************************************************************************
 * Route usage result obteained from testDB (l3 route perf)
 */

typedef struct alpm_grp_usage_s {
    /* Key: Route group. */
    int grp;

    /* Key: ALPM Template. */
    int alpm_temp;

    /* Key: Date mode. 0 - reduce, 1 - full. */
    int data_mode;

    /* Key: IPv6_128b. 0 - disable, 1 - enable, -1 - irrelevant. */
    int alpm_128b_en;

    /* Count of route entry. */
    int route_cnt;

    /* In use number: Level 1 - entry, Level 2/3 - raw bucket. */
    int cnt_inuse[BCM_L3_ALPM_LEVELS];

    /* Maximum number: Level 1 - entry, Level 2/3 - raw bucket*/
    int cnt_total[BCM_L3_ALPM_LEVELS];
} alpm_grp_usage_t;

static alpm_grp_usage_t alpm_test_db_usage_uft_0[] = {
    /* ALPM template 1 */
    {bcmL3RouteGroupPrivateV4, 1, 0, -1, 98014, {1754, 5120, 0}, {5120, 6144, 0}},
    {bcmL3RouteGroupGlobalV4,  1, 0, -1, 98014, {1754, 5120, 0}, {5120, 6144, 0}},

    /* ALPM template 2 */
    {bcmL3RouteGroupPrivateV4, 2, 0, -1, 48980, {2296, 3063, 0}, {2560, 3072, 0}},
    {bcmL3RouteGroupGlobalV4,  2, 0, -1, 48980, {2296, 3063, 0}, {2560, 3072, 0}},

    /* ALPM template 3 */
    {bcmL3RouteGroupPrivateV4, 3, 0, -1, 48980, {2296, 3063, 0}, {3072, 3072, 0}},
    {bcmL3RouteGroupGlobalV4,  3, 0, -1, 48980, {2296, 3063, 0}, {3072, 3072, 0}},

    /* ALPM template 4 */
    {bcmL3RouteGroupPrivateV4, 4, 0, -1, 23242, {1534, 1764, 0}, {1536, 2048, 0}},
    {bcmL3RouteGroupGlobalV4,  4, 0, -1, 7151,  {1022, 1019, 0}, {1536, 1024, 0}},

    /* ALPM template 5 */
    {bcmL3RouteGroupPrivateV4, 5, 0, -1, 98014, {1754, 6130, 0}, {3072, 6144, 0}},
    {bcmL3RouteGroupGlobalV4,  5, 0, -1, 98014, {1754, 6130, 0}, {3072, 6144, 0}},

    /* ALPM template 6 */
    {bcmL3RouteGroupPrivateV4, 6, 0, -1, 65320, {1424, 4085, 0}, {1536, 4096, 0}},
    {bcmL3RouteGroupGlobalV4,  6, 0, -1, 23242, {1534, 1764, 0}, {1536, 2048, 0}},

    /* ALPM template 7 */
    {bcmL3RouteGroupPrivateV4, 7, 0, -1, 32676, {1534, 2044, 0}, {1536, 3072, 0}},
    {bcmL3RouteGroupGlobalV4,  7, 0, -1, 32676, {1534, 2044, 0}, {1536, 3072, 0}},

    /* ALPM template 8 */
    {bcmL3RouteGroupPrivateV4, 8, 0, -1, 1023, {1024, 0, 0}, {1024, 0, 0}},
    {bcmL3RouteGroupGlobalV4,  8, 0, -1, 1023, {1024, 0, 0}, {1024, 0, 0}},

    /* ALPM template 9 */
    {bcmL3RouteGroupPrivateV4, 9, 0, -1, 98014, {1754, 6130, 0}, {4096, 6144, 0}},
    {bcmL3RouteGroupGlobalV4,  9, 0, -1, 98014, {1754, 6130, 0}, {4096, 6144, 0}},

    /* ALPM template 10 */
    {bcmL3RouteGroupPrivateV4, 10, 0, -1, 65320, {1424, 4085, 0}, {2048, 4096, 0}},
    {bcmL3RouteGroupGlobalV4,  10, 0, -1, 26344, {1784, 2041, 0}, {2048, 2048, 0}},

    /* ALPM template 11 */
    {bcmL3RouteGroupPrivateV4, 11, 0, -1, 98014, {1754, 6130, 0}, {2048, 6144, 0}},
    {bcmL3RouteGroupGlobalV4,  11, 0, -1, 98014, {1754, 6130, 0}, {2048, 6144, 0}},

    /* ALPM template 1, V6-64 */
    {bcmL3RouteGroupPrivateV6, 1, 0, 0, 69455, {2429, 4346, 0}, {2432, 6144, 0}},
    {bcmL3RouteGroupGlobalV6,  1, 0, 0, 69455, {2429, 4346, 0}, {2432, 6144, 0}},

    /* ALPM template 2, V6-64 */
    {bcmL3RouteGroupPrivateV6, 2, 0, 0, 16363, {1149, 1407, 0}, {1152, 3072, 0}},
    {bcmL3RouteGroupGlobalV6,  2, 0, 0, 20363, {1277, 1659, 0}, {1280, 3072, 0}},

    /* ALPM template 3, V6-64 */
    {bcmL3RouteGroupPrivateV6, 3, 0, 0, 21995, {1405, 1791, 0}, {1408, 3072, 0}},
    {bcmL3RouteGroupGlobalV6,  3, 0, 0, 21995, {1405, 1791, 0}, {1408, 3072, 0}},

    /* ALPM template 4, V6-64 */
    {bcmL3RouteGroupPrivateV6, 4, 0, 0, 8808, {637, 744, 0}, {640, 2048, 0}},
    {bcmL3RouteGroupGlobalV6,  4, 0, 0, 6522, {765, 765, 0}, {768, 1024, 0}},

    /* ALPM template 5, V6-64 */
    {bcmL3RouteGroupPrivateV6, 5, 0, 0, 40933, {1405, 2563, 0}, {1408, 6144, 0}},
    {bcmL3RouteGroupGlobalV6,  5, 0, 0, 40933, {1405, 2563, 0}, {1408, 6144, 0}},

    /* ALPM template 6, V6-64 */
    {bcmL3RouteGroupPrivateV6, 6, 0, 0, 19216, {637, 1207, 0}, {640, 4096, 0}},
    {bcmL3RouteGroupGlobalV6,  6, 0, 0, 11528, {765, 915, 0}, {768, 2048, 0}},

    /* ALPM template 7, V6-64 */
    {bcmL3RouteGroupPrivateV6, 7, 0, 0, 10122, {637, 827, 0}, {640, 3072, 0}},
    {bcmL3RouteGroupGlobalV6,  7, 0, 0, 10122, {637, 827, 0}, {640, 3072, 0}},

    /* ALPM template 8, V6-64 */
    {bcmL3RouteGroupPrivateV6, 8, 0, 0, 384, {384, 0, 0}, {384, 0, 0}},
    {bcmL3RouteGroupGlobalV6,  8, 0, 0, 384, {384, 0, 0}, {384, 0, 0}},

    /* ALPM template 9, V6-64 */
    {bcmL3RouteGroupPrivateV6, 9, 0, 0, 55268, {1917, 3459, 0}, {1920, 6144, 0}},
    {bcmL3RouteGroupGlobalV6,  9, 0, 0, 55268, {1917, 3459, 0}, {1920, 6144, 0}},

    /* ALPM template 10, V6-64 */
    {bcmL3RouteGroupPrivateV6, 10, 0, 0, 26384, {893, 1655, 0},  {896, 4096, 0}},
    {bcmL3RouteGroupGlobalV6,  10, 0, 0, 14961, {1021, 1024, 0}, {1024, 2048, 0}},

    /* ALPM template 11, V6-64 */
    {bcmL3RouteGroupPrivateV6, 11, 0, 0, 26457, {893, 1659, 0}, {896, 6144, 0}},
    {bcmL3RouteGroupGlobalV6,  11, 0, 0, 26457, {893, 1659, 0}, {896, 6144, 0}},

    /* ALPM template 1, V6-128 */
    {bcmL3RouteGroupPrivateV6, 1, 0, 1, 47760, {1277, 3057, 0}, {1280, 6144, 0}},
    {bcmL3RouteGroupGlobalV6,  1, 0, 1, 47760, {1277, 3057, 0}, {1280, 6144, 0}},

    /* ALPM template 2, V6-128 */
    {bcmL3RouteGroupPrivateV6, 2, 0, 1, 7926, {637, 832, 0}, {640, 3072, 0}},
    {bcmL3RouteGroupGlobalV6,  2, 0, 1, 7926, {637, 832, 0}, {640, 3072, 0}},

    /* ALPM template 3, V6-128 */
    {bcmL3RouteGroupPrivateV6, 3, 0, 1, 10172, {765, 1022, 0}, {768, 3072, 0}},
    {bcmL3RouteGroupGlobalV6,  3, 0, 1, 10172, {765, 1022, 0}, {768, 3072, 0}},

    /* ALPM template 4, V6-128 */
    {bcmL3RouteGroupPrivateV6, 4, 0, 1, 4853, {381, 446, 0}, {384, 2048, 0}},
    {bcmL3RouteGroupGlobalV6,  4, 0, 1, 4346, {381, 381, 0}, {384, 1024, 0}},

    /* ALPM template 5, V6-128 */
    {bcmL3RouteGroupPrivateV6, 5, 0, 1, 27274, {765, 1775, 0}, {768, 6144, 0}},
    {bcmL3RouteGroupGlobalV6,  5, 0, 1, 27274, {765, 1775, 0}, {768, 6144, 0}},

    /* ALPM template 6, V6-128 */
    {bcmL3RouteGroupPrivateV6, 6, 0, 1, 6632, {381, 576, 0}, {384, 4096, 0}},
    {bcmL3RouteGroupGlobalV6,  6, 0, 1, 4853, {381, 446, 0}, {384, 2048, 0}},

    /* ALPM template 7, V6-128 */
    {bcmL3RouteGroupPrivateV6, 7, 0, 1, 5112, {381, 511, 0}, {384, 3072, 0}},
    {bcmL3RouteGroupGlobalV6,  7, 0, 1, 5112, {381, 511, 0}, {384, 3072, 0}},

    /* ALPM template 8, V6-128 */
    {bcmL3RouteGroupPrivateV6, 8, 0, 1, 256, {256, 0, 0}, {256, 0, 0}},
    {bcmL3RouteGroupGlobalV6,  8, 0, 1, 256, {256, 0, 0}, {256, 0, 0}},

    /* ALPM template 9, V6-128 */
    {bcmL3RouteGroupPrivateV6, 9, 0, 1, 37546, {1021, 2417, 0}, {1024, 6144, 0}},
    {bcmL3RouteGroupGlobalV6,  9, 0, 1, 37546, {1021, 2417, 0}, {1024, 6144, 0}},

    /* ALPM template 10, V6-128 */
    {bcmL3RouteGroupPrivateV6, 10, 0, 1, 7656, {509, 704, 0}, {512, 4096, 0}},
    {bcmL3RouteGroupGlobalV6,  10, 0, 1, 5364, {509, 574, 0}, {512, 2048, 0}},

    /* ALPM template 11, V6-128 */
    {bcmL3RouteGroupPrivateV6, 11, 0, 1, 15056, {509, 1015, 0}, {512, 6144, 0}},
    {bcmL3RouteGroupGlobalV6,  11, 0, 1, 15056, {509, 1015, 0}, {512, 6144, 0}},
};

static alpm_grp_usage_t alpm_test_db_usage_uft_4[] = {
    /* ALPM template 1 */
    {bcmL3RouteGroupPrivateV4, 1, 0, -1, 513163, {1148, 3774, 32768}, {5120, 6144, 32768}},
    {bcmL3RouteGroupGlobalV4,  1, 0, -1, 513163, {1148, 3774, 32768}, {5120, 6144, 32768}},

    /* ALPM template 2 */
    {bcmL3RouteGroupPrivateV4, 2, 0, -1, 122600, {2558, 2600, 9551}, {2560, 3072, 16384}},
    {bcmL3RouteGroupGlobalV4,  2, 0, -1, 122600, {2558, 2600, 9551}, {2560, 3072, 16384}},

    /* ALPM template 3 */
    {bcmL3RouteGroupPrivateV4, 3, 0, -1, 391593, {2672, 3063, 24477}, {3072, 3072, 32768}},
    {bcmL3RouteGroupGlobalV4,  3, 0, -1, 391593, {2672, 3063, 24477}, {3072, 3072, 32768}},

    /* ALPM template 4 */
    {bcmL3RouteGroupPrivateV4, 4, 0, -1, 93329, {1534, 1540, 5902}, {1536, 2048, 16384}},
    {bcmL3RouteGroupGlobalV4,  4, 0, -1, 20072, {1026, 1019, 1544}, {1536, 1024, 16384}},

    /* ALPM template 5 */
    {bcmL3RouteGroupPrivateV4, 5, 0, -1, 513163, {1148, 3774, 32758}, {3072, 6144, 32768}},
    {bcmL3RouteGroupGlobalV4,  5, 0, -1, 513163, {1148, 3774, 32758}, {3072, 6144, 32768}},

    /* ALPM template 6 */
    {bcmL3RouteGroupPrivateV4, 6, 0, -1, 143440, {1534, 2789, 10828}, {1536, 4096, 16384}},
    {bcmL3RouteGroupGlobalV4,  6, 0, -1, 93329,  {1534, 1540, 5902},  {1536, 2048, 16384}},

    /* ALPM template 7 */
    {bcmL3RouteGroupPrivateV4, 7, 0, -1, 225704, {1534, 1763, 14108}, {1536, 3072, 32768}},
    {bcmL3RouteGroupGlobalV4,  7, 0, -1, 225704, {1534, 1763, 14108}, {1536, 3072, 32768}},

    /* ALPM template 8 */
    {bcmL3RouteGroupPrivateV4, 8, 0, -1, 1023, {1024, 0, 0}, {1024, 0, 0}},
    {bcmL3RouteGroupGlobalV4,  8, 0, -1, 1023, {1024, 0, 0}, {1024, 0, 0}},

    /* ALPM template 9 */
    {bcmL3RouteGroupPrivateV4, 9, 0, -1, 513163, {1148, 3774, 32758}, {4096, 6144, 32768}},
    {bcmL3RouteGroupGlobalV4,  9, 0, -1, 513163, {1148, 3774, 32758}, {4096, 6144, 32768}},

    /* ALPM template 10 */
    {bcmL3RouteGroupPrivateV4, 10, 0, -1, 189880, {2046, 3706, 14404}, {2048, 4096, 16384}},
    {bcmL3RouteGroupGlobalV4,  10, 0, -1, 125393, {2034, 2041, 7906},  {2048, 2048, 16384}},

    /* ALPM template 11 */
    {bcmL3RouteGroupPrivateV4, 11, 0, -1, 513163, {1148, 3774, 32758}, {2048, 6144, 32768}},
    {bcmL3RouteGroupGlobalV4,  11, 0, -1, 513163, {1148, 3774, 32758}, {2048, 6144, 32768}},

    /* ALPM template 1, V6-64 */
    {bcmL3RouteGroupPrivateV6, 1, 0, 0, 349208, {2429, 4877, 21846}, {2432, 6144, 32768}},
    {bcmL3RouteGroupGlobalV6,  1, 0, 0, 349208, {2429, 4877, 21846}, {2432, 6144, 32768}},

    /* ALPM template 2, V6-64 */
    {bcmL3RouteGroupPrivateV6, 2, 0, 0, 43936, {1149, 1152, 3894}, {1152, 3072, 16384}},
    {bcmL3RouteGroupGlobalV6,  2, 0, 0, 48967, {1277, 1280, 4341}, {1280, 3072, 16384}},

    /* ALPM template 3, V6-64 */
    {bcmL3RouteGroupPrivateV6, 3, 0, 0, 94609, {1405, 1422, 6253}, {1408, 3072, 32768}},
    {bcmL3RouteGroupGlobalV6,  3, 0, 0, 94609, {1405, 1422, 6253}, {1408, 3072, 32768}},

    /* ALPM template 4, V6-64 */
    {bcmL3RouteGroupPrivateV6, 4, 0, 0, 19244, {637, 646, 1576}, {640, 2048, 16384}},
    {bcmL3RouteGroupGlobalV6,  4, 0, 0, 20200, {765, 765, 1520}, {768, 1024, 16384}},

    /* ALPM template 5, V6-64 */
    {bcmL3RouteGroupPrivateV6, 5, 0, 0, 205056, {1405, 2828, 12834}, {1408, 6144, 32768}},
    {bcmL3RouteGroupGlobalV6,  5, 0, 0, 205056, {1405, 2828, 12834}, {1408, 6144, 32768}},

    /* ALPM template 6, V6-64 */
    {bcmL3RouteGroupPrivateV6, 6, 0, 0, 29836, {637, 695, 2772}, {640, 4096, 16384}},
    {bcmL3RouteGroupGlobalV6,  6, 0, 0, 23089, {765, 782, 1895}, {768, 2048, 16384}},

    /* ALPM template 7, V6-64 */
    {bcmL3RouteGroupPrivateV6, 7, 0, 0, 45457, {637, 654, 2989}, {640, 3072, 32768}},
    {bcmL3RouteGroupGlobalV6,  7, 0, 0, 45457, {637, 654, 2989}, {640, 3072, 32768}},

    /* ALPM template 8, V6-64 */
    {bcmL3RouteGroupPrivateV6, 8, 0, 0, 384, {384, 0, 0}, {384, 0, 0}},
    {bcmL3RouteGroupGlobalV6,  8, 0, 0, 384, {384, 0, 0}, {384, 0, 0}},

    /* ALPM template 9, V6-64 */
    {bcmL3RouteGroupPrivateV6, 9, 0, 0, 277259, {1917, 3852, 17349}, {1920, 6144, 32768}},
    {bcmL3RouteGroupGlobalV6,  9, 0, 0, 277259, {1917, 3852, 17349}, {1920, 6144, 32768}},

    /* ALPM template 10, V6-64 */
    {bcmL3RouteGroupPrivateV6, 10, 0, 0, 39308, {893, 959, 3828},   {896, 4096, 16384}},
    {bcmL3RouteGroupGlobalV6,  10, 0, 0, 28300, {1021, 1038, 2454}, {1024, 2048, 16384}},

    /* ALPM template 11, V6-64 */
    {bcmL3RouteGroupPrivateV6, 11, 0, 0, 132608, {893, 1804, 8306}, {896, 6144, 32768}},
    {bcmL3RouteGroupGlobalV6,  11, 0, 0, 132608, {893, 1804, 8306}, {896, 6144, 32768}},

    /* ALPM template 1, V6-128 */
    {bcmL3RouteGroupPrivateV6, 1, 0, 1, 108812, {1277, 2591, 11392}, {1280, 6144, 32768}},
    {bcmL3RouteGroupGlobalV6,  1, 0, 1, 108812, {1277, 2591, 11392}, {1280, 6144, 32768}},

    /* ALPM template 2, V6-128 */
    {bcmL3RouteGroupPrivateV6, 2, 0, 1, 16518, {637, 640, 2456}, {640, 3072, 16384}},
    {bcmL3RouteGroupGlobalV6,  2, 0, 1, 16518, {637, 640, 2456}, {640, 3072, 16384}},

    /* ALPM template 3, V6-128 */
    {bcmL3RouteGroupPrivateV6, 3, 0, 1, 45945, {765, 861, 3952}, {768, 3072, 32768}},
    {bcmL3RouteGroupGlobalV6,  3, 0, 1, 45945, {765, 861, 3952}, {768, 3072, 32768}},

    /* ALPM template 4, V6-128 */
    {bcmL3RouteGroupPrivateV6, 4, 0, 1, 6120, {381, 382, 763}, {384, 2048, 16384}},
    {bcmL3RouteGroupGlobalV6,  4, 0, 1, 5364, {381, 381, 574}, {384, 1024, 16384}},

    /* ALPM template 5, V6-128 */
    {bcmL3RouteGroupPrivateV6, 5, 0, 1, 67724, {765, 1562, 6951}, {768, 6144, 32768}},
    {bcmL3RouteGroupGlobalV6,  5, 0, 1, 67724, {765, 1562, 6951}, {768, 6144, 32768}},

    /* ALPM template 6, V6-128 */
    {bcmL3RouteGroupPrivateV6, 6, 0, 1, 13644, {381, 422, 1682}, {384, 4096, 16384}},
    {bcmL3RouteGroupGlobalV6,  6, 0, 1, 6120,  {381, 382, 763},  {384, 2048, 16384}},

    /* ALPM template 7, V6-128 */
    {bcmL3RouteGroupPrivateV6, 7, 0, 1, 22920, {381, 429, 1943}, {384, 3072, 32768}},
    {bcmL3RouteGroupGlobalV6,  7, 0, 1, 22920, {381, 429, 1943}, {384, 3072, 32768}},

    /* ALPM template 8, V6-128 */
    {bcmL3RouteGroupPrivateV6, 8, 0, 1, 256, {256, 0, 0}, {256, 0, 0}},
    {bcmL3RouteGroupGlobalV6,  8, 0, 1, 256, {256, 0, 0}, {256, 0, 0}},

    /* ALPM template 9, V6-128 */
    {bcmL3RouteGroupPrivateV6, 9, 0, 1, 89023, {1021, 2077, 9214}, {1024, 6144, 32768}},
    {bcmL3RouteGroupGlobalV6,  9, 0, 1, 89023, {1021, 2077, 9214}, {1024, 6144, 32768}},

    /* ALPM template 10, V6-128 */
    {bcmL3RouteGroupPrivateV6, 10, 0, 1, 17219, {509, 558, 2164}, {512, 4096, 16384}},
    {bcmL3RouteGroupGlobalV6,  10, 0, 1, 7146,  {509, 510, 1019}, {512, 2048, 16384}},

    /* ALPM template 11, V6-128 */
    {bcmL3RouteGroupPrivateV6, 11, 0, 1, 47072, {509, 1047, 4725}, {512, 6144, 32768}},
    {bcmL3RouteGroupGlobalV6,  11, 0, 1, 47072, {509, 1047, 4725}, {512, 6144, 32768}},
};

static alpm_grp_usage_t alpm_test_db_usage_uft_5[] = {
    /* ALPM template 1 */
    {bcmL3RouteGroupPrivateV4, 1, 0, -1, 655038, {1910, 3826, 40947}, {5120, 6144, 40960}},
    {bcmL3RouteGroupGlobalV4,  1, 0, -1, 655038, {1910, 3826, 40947}, {5120, 6144, 40960}},

    /* ALPM template 2 */
    {bcmL3RouteGroupPrivateV4, 2, 0, -1, 284513, {2500, 3063, 17808}, {2560, 3072, 24576}},
    {bcmL3RouteGroupGlobalV4,  2, 0, -1, 122600, {2558, 2600, 9551},  {2560, 3072, 16384}},

    /* ALPM template 3 */
    {bcmL3RouteGroupPrivateV4, 3, 0, -1, 568241, {3046, 3063, 35527}, {3072, 3072, 40960}},
    {bcmL3RouteGroupGlobalV4,  3, 0, -1, 568241, {3046, 3063, 35527}, {3072, 3072, 40960}},

    /* ALPM template 4 */
    {bcmL3RouteGroupPrivateV4, 4, 0, -1, 93329, {1534, 1540, 5902}, {1536, 2048, 16384}},
    {bcmL3RouteGroupGlobalV4,  4, 0, -1, 28284, {1022, 1019, 1769}, {1536, 1024, 16384}},

    /* ALPM template 5 */
    {bcmL3RouteGroupPrivateV4, 5, 0, -1, 655038, {1910, 3826, 40947}, {3072, 6144, 40960}},
    {bcmL3RouteGroupGlobalV4,  5, 0, -1, 655038, {1910, 3826, 40947}, {3072, 6144, 40960}},

    /* ALPM template 6 */
    {bcmL3RouteGroupPrivateV4, 6, 0, -1, 143440, {1534, 2789, 10828}, {1536, 4096, 16384}},
    {bcmL3RouteGroupGlobalV4,  6, 0, -1, 122862, {1534, 1558, 7684},  {1536, 2048, 24576}},

    /* ALPM template 7 */
    {bcmL3RouteGroupPrivateV4, 7, 0, -1, 284673, {1534, 1543, 17795}, {1536, 3072, 40960}},
    {bcmL3RouteGroupGlobalV4,  7, 0, -1, 284673, {1534, 1543, 17795}, {1536, 3072, 40960}},

    /* ALPM template 8 */
    {bcmL3RouteGroupPrivateV4, 8, 0, -1, 1023, {1024, 0, 0}, {1024, 0, 0}},
    {bcmL3RouteGroupGlobalV4,  8, 0, -1, 1023, {1024, 0, 0}, {1024, 0, 0}},

    /* ALPM template 9 */
    {bcmL3RouteGroupPrivateV4, 9, 0, -1, 655038, {1910, 3826, 40947}, {4096, 6144, 40960}},
    {bcmL3RouteGroupGlobalV4,  9, 0, -1, 655038, {1910, 3826, 40947}, {4096, 6144, 40960}},

    /* ALPM template 10 */
    {bcmL3RouteGroupPrivateV4, 10, 0, -1, 189880, {2046, 3706, 14404}, {2048, 4096, 16384}},
    {bcmL3RouteGroupGlobalV4,  10, 0, -1, 164028, {2014, 2041, 10256}, {2048, 2048, 24576}},

    /* ALPM template 11 */
    {bcmL3RouteGroupPrivateV4, 11, 0, -1, 655038, {1910, 3826, 40947}, {2048, 6144, 40960}},
    {bcmL3RouteGroupGlobalV4,  11, 0, -1, 655038, {1910, 3826, 40947}, {2048, 6144, 40960}},

    /* ALPM template 1, V6-64 */
    {bcmL3RouteGroupPrivateV6, 1, 0, 0, 346112, {2429, 4861, 21643}, {2432, 6144, 40960}},
    {bcmL3RouteGroupGlobalV6,  1, 0, 0, 346112, {2429, 4861, 21643}, {2432, 6144, 40960}},

    /* ALPM template 2, V6-64 */
    {bcmL3RouteGroupPrivateV6, 2, 0, 0, 85184, {1149, 1193, 5410}, {1152, 3072, 24576}},
    {bcmL3RouteGroupGlobalV6,  2, 0, 0, 48967, {1277, 1280, 4341}, {1280, 3072, 16384}},

    /* ALPM template 3, V6-64 */
    {bcmL3RouteGroupPrivateV6, 3, 0, 0, 145169, {1405, 1406, 9364}, {1408, 3072, 40960}},
    {bcmL3RouteGroupGlobalV6,  3, 0, 0, 145169, {1405, 1406, 9364}, {1408, 3072, 40960}},

    /* ALPM template 4, V6-64 */
    {bcmL3RouteGroupPrivateV6, 4, 0, 0, 19244, {637, 646, 1576}, {640, 2048, 16384}},
    {bcmL3RouteGroupGlobalV6,  4, 0, 0, 14315, {765, 765, 1151}, {768, 1024, 24576}},

    /* ALPM template 5, V6-64 */
    {bcmL3RouteGroupPrivateV6, 5, 0, 0, 201246, {1405, 2814, 12592}, {1408, 6144, 40960}},
    {bcmL3RouteGroupGlobalV6,  5, 0, 0, 201246, {1405, 2814, 12592}, {1408, 6144, 40960}},

    /* ALPM template 6, V6-64 */
    {bcmL3RouteGroupPrivateV6, 6, 0, 0, 29836, {637, 695, 2772}, {640, 4096, 16384}},
    {bcmL3RouteGroupGlobalV6,  6, 0, 0, 25468, {765, 768, 2041}, {768, 2048, 24576}},

    /* ALPM template 7, V6-64 */
    {bcmL3RouteGroupPrivateV6, 7, 0, 0, 65295, {637, 638, 4212}, {640, 3072, 40960}},
    {bcmL3RouteGroupGlobalV6,  7, 0, 0, 65295, {637, 638, 4212}, {640, 3072, 40960}},

    /* ALPM template 8, V6-64 */
    {bcmL3RouteGroupPrivateV6, 8, 0, 0, 384, {384, 0, 0}, {384, 0, 0}},
    {bcmL3RouteGroupGlobalV6,  8, 0, 0, 384, {384, 0, 0}, {384, 0, 0}},

    /* ALPM template 9, V6-64 */
    {bcmL3RouteGroupPrivateV6, 9, 0, 0, 273664, {1917, 3837, 17115}, {3837, 6144, 40960}},
    {bcmL3RouteGroupGlobalV6,  9, 0, 0, 273664, {1917, 3837, 17115}, {3837, 6144, 40960}},

    /* ALPM template 10, V6-64 */
    {bcmL3RouteGroupPrivateV6, 10, 0, 0, 39308, {893, 959, 3828},   {896, 4096, 16384}},
    {bcmL3RouteGroupGlobalV6,  10, 0, 0, 35196, {1021, 1024, 2777}, {1024, 2048, 24576}},

    /* ALPM template 11, V6-64 */
    {bcmL3RouteGroupPrivateV6, 11, 0, 0, 128766, {893, 1789, 8062}, {896, 6144, 40960}},
    {bcmL3RouteGroupGlobalV6,  11, 0, 0, 128766, {893, 1789, 8062}, {896, 6144, 40960}},

    /* ALPM template 1, V6-128 */
    {bcmL3RouteGroupPrivateV6, 1, 0, 1, 193295, {1277, 2543, 15263}, {1280, 6144, 40960}},
    {bcmL3RouteGroupGlobalV6,  1, 0, 1, 193295, {1277, 2543, 15263}, {1280, 6144, 40960}},

    /* ALPM template 2, V6-128 */
    {bcmL3RouteGroupPrivateV6, 2, 0, 1, 23013, {637, 638, 2519}, {640, 3072, 24576}},
    {bcmL3RouteGroupGlobalV6,  2, 0, 1, 16518, {637, 640, 2456}, {640, 3072, 16384}},

    /* ALPM template 3, V6-128 */
    {bcmL3RouteGroupPrivateV6, 3, 0, 1, 45963, {765, 766, 3640}, {768, 3072, 40960}},
    {bcmL3RouteGroupGlobalV6,  3, 0, 1, 45963, {765, 766, 3640}, {768, 3072, 40960}},

    /* ALPM template 4, V6-128 */
    {bcmL3RouteGroupPrivateV6, 4, 0, 1, 6120, {381, 382, 763}, {384, 2048, 16384}},
    {bcmL3RouteGroupGlobalV6,  4, 0, 1, 7290, {381, 381, 673}, {384, 1024, 24576}},

    /* ALPM template 5, V6-128 */
    {bcmL3RouteGroupPrivateV6, 5, 0, 1, 115279, {765, 1528, 9128}, {768, 6144, 40960}},
    {bcmL3RouteGroupGlobalV6,  5, 0, 1, 115279, {765, 1528, 9128}, {768, 6144, 40960}},

    /* ALPM template 6, V6-128 */
    {bcmL3RouteGroupPrivateV6, 6, 0, 1, 13644, {381, 422, 1682}, {384, 4096, 16384}},
    {bcmL3RouteGroupGlobalV6,  6, 0, 1, 10108, {381, 383, 1018}, {384, 2048, 24576}},

    /* ALPM template 7, V6-128 */
    {bcmL3RouteGroupPrivateV6, 7, 0, 1, 22923, {381, 382, 1816}, {384, 3072, 40960}},
    {bcmL3RouteGroupGlobalV6,  7, 0, 1, 22923, {381, 382, 1816}, {384, 3072, 40960}},

    /* ALPM template 8, V6-128 */
    {bcmL3RouteGroupPrivateV6, 8, 0, 1, 256, {256, 0, 0}, {256, 0, 0}},
    {bcmL3RouteGroupGlobalV6,  8, 0, 1, 256, {256, 0, 0}, {256, 0, 0}},

    /* ALPM template 9, V6-128 */
    {bcmL3RouteGroupPrivateV6, 9, 0, 1, 154924, {1021, 2036, 12233}, {1024, 6144, 40960}},
    {bcmL3RouteGroupGlobalV6,  9, 0, 1, 154924, {1021, 2036, 12233}, {1024, 6144, 40960}},

    /* ALPM template 10, V6-128 */
    {bcmL3RouteGroupPrivateV6, 10, 0, 1, 17219, {509, 558, 2164}, {512, 4096, 16384}},
    {bcmL3RouteGroupGlobalV6,  10, 0, 1, 17001, {509, 512, 1488}, {512, 2048, 24576}},

    /* ALPM template 11, V6-128 */
    {bcmL3RouteGroupPrivateV6, 11, 0, 1, 77847, {509, 1021, 6138}, {512, 6144, 40960}},
    {bcmL3RouteGroupGlobalV6,  11, 0, 1, 77847, {509, 1021, 6138}, {512, 6144, 40960}},
};

static alpm_grp_usage_t alpm_test_db_usage_uft_6[] = {
    /* ALPM template 1 */
    {bcmL3RouteGroupPrivateV4, 1, 0, -1, 786058, {2294, 4589, 49137}, {5120, 6144, 49152}},
    {bcmL3RouteGroupGlobalV4,  1, 0, -1, 786058, {2294, 4589, 49137}, {5120, 6144, 49152}},

    /* ALPM template 2 */
    {bcmL3RouteGroupPrivateV4, 2, 0, -1, 375065, {2558, 2932, 23444}, {2560, 3072, 32768}},
    {bcmL3RouteGroupGlobalV4,  2, 0, -1, 122600, {2558, 2600, 9551},  {2560, 3072, 16384}},

    /* ALPM template 3 */
    {bcmL3RouteGroupPrivateV4, 3, 0, -1, 524446, {3058, 3063, 32785}, {3072, 3072, 49152}},
    {bcmL3RouteGroupGlobalV4,  3, 0, -1, 524446, {3058, 3063, 32785}, {3072, 3072, 49152}},

    /* ALPM template 4 */
    {bcmL3RouteGroupPrivateV4, 4, 0, -1, 93329, {1534, 1540, 5902}, {1536, 2048, 16384}},
    {bcmL3RouteGroupGlobalV4,  4, 0, -1, 46973, {1022, 1019, 2939}, {1536, 1024, 32768}},

    /* ALPM template 5 */
    {bcmL3RouteGroupPrivateV4, 5, 0, -1, 786058, {2294, 4589, 49137}, {3072, 6144, 49152}},
    {bcmL3RouteGroupGlobalV4,  5, 0, -1, 786058, {2294, 4589, 49137}, {3072, 6144, 49152}},

    /* ALPM template 6 */
    {bcmL3RouteGroupPrivateV4, 6, 0, -1, 143440, {1534, 2789, 10828}, {1536, 4096, 16384}},
    {bcmL3RouteGroupGlobalV4,  6, 0, -1, 127350, {1534, 1544, 7975},  {1536, 2048, 32768}},

    /* ALPM template 7 */
    {bcmL3RouteGroupPrivateV4, 7, 0, -1, 265054, {1534, 1537, 16573}, {1536, 3072, 49152}},
    {bcmL3RouteGroupGlobalV4,  7, 0, -1, 265054, {1534, 1537, 16573}, {1536, 3072, 49152}},

    /* ALPM template 8 */
    {bcmL3RouteGroupPrivateV4, 8, 0, -1, 1023, {1024, 0, 0}, {1024, 0, 0}},
    {bcmL3RouteGroupGlobalV4,  8, 0, -1, 1023, {1024, 0, 0}, {1024, 0, 0}},

    /* ALPM template 9 */
    {bcmL3RouteGroupPrivateV4, 9, 0, -1, 786058, {2294, 4589, 49137}, {4096, 6144, 49152}},
    {bcmL3RouteGroupGlobalV4,  9, 0, -1, 786058, {2294, 4589, 49137}, {4096, 6144, 49152}},

    /* ALPM template 10 */
    {bcmL3RouteGroupPrivateV4, 10, 0, -1, 189880, {2046, 3706, 14404}, {2048, 4096, 16384}},
    {bcmL3RouteGroupGlobalV4,  10, 0, -1, 168680, {2026, 2041, 10563}, {2048, 2048, 32768}},

    /* ALPM template 11 */
    {bcmL3RouteGroupPrivateV4, 11, 0, -1, 701994, {2046, 4092, 43883}, {2048, 6144, 49152}},
    {bcmL3RouteGroupGlobalV4,  11, 0, -1, 701994, {2046, 4092, 43883}, {2048, 6144, 49152}},

    /* ALPM template 1, V6-64 */
    {bcmL3RouteGroupPrivateV6, 1, 0, 0, 757466, {2220, 5524, 49140}, {2432, 6144, 49152}},
    {bcmL3RouteGroupGlobalV6,  1, 0, 0, 757466, {2220, 5524, 49140}, {2432, 6144, 49152}},

    /* ALPM template 2, V6-64 */
    {bcmL3RouteGroupPrivateV6, 2, 0, 0, 78225, {1149, 1166, 5165}, {1152, 3072, 32768}},
    {bcmL3RouteGroupGlobalV6,  2, 0, 0, 48967, {1277, 1280, 4341}, {1280, 3072, 16384}},

    /* ALPM template 3, V6-64 */
    {bcmL3RouteGroupPrivateV6, 3, 0, 0, 220231, {1405, 1407, 14203}, {1408, 3072, 49152}},
    {bcmL3RouteGroupGlobalV6,  3, 0, 0, 220231, {1405, 1407, 14203}, {1408, 3072, 49152}},

    /* ALPM template 4, V6-64 */
    {bcmL3RouteGroupPrivateV6, 4, 0, 0, 19244, {637, 646, 1576}, {640, 2048, 16384}},
    {bcmL3RouteGroupGlobalV6,  4, 0, 0, 22240, {765, 765, 1396}, {768, 1024, 32768}},

    /* ALPM template 5, V6-64 */
    {bcmL3RouteGroupPrivateV6, 5, 0, 0, 479284, {1405, 3487, 31079}, {1408, 6144, 49152}},
    {bcmL3RouteGroupGlobalV6,  5, 0, 0, 479284, {1405, 3487, 31079}, {1408, 6144, 49152}},

    /* ALPM template 6, V6-64 */
    {bcmL3RouteGroupPrivateV6, 6, 0, 0, 29836, {637, 695, 2772}, {640, 4096, 16384}},
    {bcmL3RouteGroupGlobalV6,  6, 0, 0, 42888, {765, 771, 2989}, {768, 2048, 32768}},

    /* ALPM template 7, V6-64 */
    {bcmL3RouteGroupPrivateV6, 7, 0, 0, 98119, {637, 639, 6331}, {640, 3072, 49152}},
    {bcmL3RouteGroupGlobalV6,  7, 0, 0, 98119, {637, 639, 6331}, {640, 3072, 49152}},

    /* ALPM template 8, V6-64 */
    {bcmL3RouteGroupPrivateV6, 8, 0, 0, 384, {384, 0, 0}, {384, 0, 0}},
    {bcmL3RouteGroupGlobalV6,  8, 0, 0, 384, {384, 0, 0}, {384, 0, 0}},

    /* ALPM template 9, V6-64 */
    {bcmL3RouteGroupPrivateV6, 9, 0, 0, 654362, {1917, 4766, 42445}, {1920, 6144, 49152}},
    {bcmL3RouteGroupGlobalV6,  9, 0, 0, 654362, {1917, 4766, 42445}, {1920, 6144, 49152}},

    /* ALPM template 10, V6-64 */
    {bcmL3RouteGroupPrivateV6, 10, 0, 0, 39308, {893, 959, 3828},   {896, 4096, 16384}},
    {bcmL3RouteGroupGlobalV6,  10, 0, 0, 54928, {1021, 1027, 3743}, {1024, 2048, 32768}},

    /* ALPM template 11, V6-64 */
    {bcmL3RouteGroupPrivateV6, 11, 0, 0, 303204, {893, 2203, 19640}, {896, 6144, 49152}},
    {bcmL3RouteGroupGlobalV6,  11, 0, 0, 303204, {893, 2203, 19640}, {896, 6144, 49152}},

    /* ALPM template 1, V6-128 */
    {bcmL3RouteGroupPrivateV6, 1, 0, 1, 221440, {1277, 2564, 14142}, {1280, 6144, 49152}},
    {bcmL3RouteGroupGlobalV6,  1, 0, 1, 221440, {1277, 2564, 14142}, {1280, 6144, 49152}},

    /* ALPM template 2, V6-128 */
    {bcmL3RouteGroupPrivateV6, 2, 0, 1, 38280, {637, 717, 3287}, {640, 3072, 32768}},
    {bcmL3RouteGroupGlobalV6,  2, 0, 1, 16518, {637, 640, 2456}, {640, 3072, 16384}},

    /* ALPM template 3, V6-128 */
    {bcmL3RouteGroupPrivateV6, 3, 0, 1, 47022, {765, 766, 3674}, {768, 3072, 49152}},
    {bcmL3RouteGroupGlobalV6,  3, 0, 1, 47022, {765, 766, 3674}, {768, 3072, 49152}},

    /* ALPM template 4, V6-128 */
    {bcmL3RouteGroupPrivateV6, 4, 0, 1, 6120, {381, 382, 763}, {384, 2048, 16384}},
    {bcmL3RouteGroupGlobalV6,  4, 0, 1, 6632, {381, 381, 576}, {384, 1024, 32768}},

    /* ALPM template 5, V6-128 */
    {bcmL3RouteGroupPrivateV6, 5, 0, 1, 133376, {765, 1540, 8550}, {768, 6144, 49152}},
    {bcmL3RouteGroupGlobalV6,  5, 0, 1, 133376, {765, 1540, 8550}, {768, 6144, 49152}},

    /* ALPM template 6, V6-128 */
    {bcmL3RouteGroupPrivateV6, 6, 0, 1, 13644, {381, 422, 1682}, {384, 4096, 16384}},
    {bcmL3RouteGroupGlobalV6,  6, 0, 1, 13264, {381, 382, 1149}, {384, 2048, 32768}},

    /* ALPM template 7, V6-128 */
    {bcmL3RouteGroupPrivateV6, 7, 0, 1, 23982, {381, 382, 1850}, {384, 3072, 49152}},
    {bcmL3RouteGroupGlobalV6,  7, 0, 1, 23982, {381, 382, 1850}, {384, 3072, 49152}},

    /* ALPM template 8, V6-128 */
    {bcmL3RouteGroupPrivateV6, 8, 0, 1, 256, {256, 0, 0}, {256, 0, 0}},
    {bcmL3RouteGroupGlobalV6,  8, 0, 1, 256, {256, 0, 0}, {256, 0, 0}},

    /* ALPM template 9, V6-128 */
    {bcmL3RouteGroupPrivateV6, 9, 0, 1, 176402, {1021, 2053, 11284}, {1024, 6144, 49152}},
    {bcmL3RouteGroupGlobalV6,  9, 0, 1, 176402, {1021, 2053, 11284}, {1024, 6144, 49152}},

    /* ALPM template 10, V6-128 */
    {bcmL3RouteGroupPrivateV6, 10, 0, 1, 17219, {509, 558, 2164}, {512, 4096, 16384}},
    {bcmL3RouteGroupGlobalV6,  10, 0, 1, 15312, {509, 510, 1405}, {512, 2048, 32768}},

    /* ALPM template 11, V6-128 */
    {bcmL3RouteGroupPrivateV6, 11, 0, 1, 90368, {509, 1028, 5822}, {512, 6144, 49152}},
    {bcmL3RouteGroupGlobalV6,  11, 0, 1, 90368, {509, 1028, 5822}, {512, 6144, 49152}},
};

static alpm_grp_usage_t alpm_test_db_usage_uft_8[] = {
    /* ALPM template 1 */
    {bcmL3RouteGroupPrivateV4, 1, 0, -1, 1041680, {1826, 4348, 65520}, {5120, 6144, 65536}},
    {bcmL3RouteGroupGlobalV4,  1, 0, -1, 1041680, {1826, 4348, 65520}, {5120, 6144, 65536}},

    /* ALPM template 2 */
    {bcmL3RouteGroupPrivateV4, 2, 0, -1, 375065, {2558, 2932, 23444}, {2560, 3072, 32768}},
    {bcmL3RouteGroupGlobalV4,  2, 0, -1, 375065, {2558, 2932, 23444}, {2560, 3072, 32768}},

    /* ALPM template 3 */
    {bcmL3RouteGroupPrivateV4, 3, 0, -1, 391593, {2672, 3063, 24477}, {3072, 3072, 32768}},
    {bcmL3RouteGroupGlobalV4,  3, 0, -1, 391593, {2672, 3063, 24477}, {3072, 3072, 32768}},

    /* ALPM template 4 */
    {bcmL3RouteGroupPrivateV4, 4, 0, -1, 93329, {1534, 1540, 5902}, {1536, 2048, 16384}},
    {bcmL3RouteGroupGlobalV4,  4, 0, -1, 20072, {1026, 1019, 1544}, {1536, 1024, 16384}},

    /* ALPM template 5 */
    {bcmL3RouteGroupPrivateV4, 5, 0, -1, 1041680, {1826, 4348, 65520}, {3072, 6144, 65536}},
    {bcmL3RouteGroupGlobalV4,  5, 0, -1, 1041680, {1826, 4348, 65520}, {3072, 6144, 65536}},

    /* ALPM template 6 */
    {bcmL3RouteGroupPrivateV4, 6, 0, -1, 501312, {1534, 3530, 32635}, {1536, 4096, 32768}},
    {bcmL3RouteGroupGlobalV4,  6, 0, -1, 127350, {1534, 1544, 7975},  {1536, 2048, 32768}},

    /* ALPM template 7 */
    {bcmL3RouteGroupPrivateV4, 7, 0, -1, 45457, {637, 654, 2989}, {640, 3072, 32768}},
    {bcmL3RouteGroupGlobalV4,  7, 0, -1, 45457, {637, 654, 2989}, {640, 3072, 32768}},

    /* ALPM template 8 */
    {bcmL3RouteGroupPrivateV4, 8, 0, -1, 1023, {1024, 0, 0}, {1024, 0, 0}},
    {bcmL3RouteGroupGlobalV4,  8, 0, -1, 1023, {1024, 0, 0}, {1024, 0, 0}},

    /* ALPM template 9 */
    {bcmL3RouteGroupPrivateV4, 9, 0, -1, 1041680, {1826, 4348, 65520}, {4096, 6144, 65536}},
    {bcmL3RouteGroupGlobalV4,  9, 0, -1, 1041680, {1826, 4348, 65520}, {4096, 6144, 65536}},

    /* ALPM template 10 */
    {bcmL3RouteGroupPrivateV4, 10, 0, -1, 503196, {1536, 3541, 32758}, {2048, 4096, 32768}},
    {bcmL3RouteGroupGlobalV4,  10, 0, -1, 168680, {2026, 2041, 10563}, {2048, 2048, 32768}},

    /* ALPM template 11 */
    {bcmL3RouteGroupPrivateV4, 11, 0, -1, 1041680, {1826, 4348, 65520}, {2048, 6144, 65536}},
    {bcmL3RouteGroupGlobalV4,  11, 0, -1, 1041680, {1826, 4348, 65520}, {2048, 6144, 65536}},

    /* ALPM template 1, V6-64 */
    {bcmL3RouteGroupPrivateV6, 1, 0, 0, 953464, {2429, 5673, 61941}, {2432, 6144, 65536}},
    {bcmL3RouteGroupGlobalV6,  1, 0, 0, 953464, {2429, 5673, 61941}, {2432, 6144, 65536}},

    /* ALPM template 2, V6-64 */
    {bcmL3RouteGroupPrivateV6, 2, 0, 0, 78225, {1149, 1166, 5165}, {1152, 3072, 32768}},
    {bcmL3RouteGroupGlobalV6,  2, 0, 0, 86417, {1277, 1294, 5709}, {1280, 3072, 32768}},

    /* ALPM template 3, V6-64 */
    {bcmL3RouteGroupPrivateV6, 3, 0, 0, 94609, {1405, 1422, 6253}, {1408, 3072, 32768}},
    {bcmL3RouteGroupGlobalV6,  3, 0, 0, 94609, {1405, 1422, 6253}, {1408, 3072, 32768}},

    /* ALPM template 4, V6-64 */
    {bcmL3RouteGroupPrivateV6, 4, 0, 0, 19244, {637, 646, 1576}, {640, 2048, 16384}},
    {bcmL3RouteGroupGlobalV6,  4, 0, 0, 20200, {765, 765, 1520}, {768, 1024, 16384}},

    /* ALPM template 5, V6-64 */
    {bcmL3RouteGroupPrivateV6, 5, 0, 0, 553584, {1405, 3823, 36048}, {1408, 6144, 65536}},
    {bcmL3RouteGroupGlobalV6,  5, 0, 0, 553584, {1405, 3823, 36048}, {1408, 6144, 65536}},

    /* ALPM template 6, V6-64 */
    {bcmL3RouteGroupPrivateV6, 6, 0, 0, 61848, {637, 698, 3925}, {640, 4096, 32768}},
    {bcmL3RouteGroupGlobalV6,  6, 0, 0, 42888, {765, 771, 2989}, {768, 2048, 32768}},

    /* ALPM template 7, V6-64 */
    {bcmL3RouteGroupPrivateV6, 7, 0, 0, 45457, {637, 654, 2989}, {640, 3072, 32768}},
    {bcmL3RouteGroupGlobalV6,  7, 0, 0, 45457, {637, 654, 2989}, {640, 3072, 32768}},

    /* ALPM template 8, V6-64 */
    {bcmL3RouteGroupPrivateV6, 8, 0, 0, 384, {384, 0, 0}, {384, 0, 0}},
    {bcmL3RouteGroupGlobalV6,  8, 0, 0, 384, {384, 0, 0}, {384, 0, 0}},

    /* ALPM template 9, V6-64 */
    {bcmL3RouteGroupPrivateV6, 9, 0, 0, 753446, {1917, 4477, 48999}, {1920, 6144, 65536}},
    {bcmL3RouteGroupGlobalV6,  9, 0, 0, 753446, {1917, 4477, 48999}, {1920, 6144, 65536}},

    /* ALPM template 10, V6-64 */
    {bcmL3RouteGroupPrivateV6, 10, 0, 0, 82824, {893, 954, 5236},   {896, 4096, 32768}},
    {bcmL3RouteGroupGlobalV6,  10, 0, 0, 54928, {1021, 1027, 3743}, {1024, 2048, 32768}},

    /* ALPM template 11, V6-64 */
    {bcmL3RouteGroupPrivateV6, 11, 0, 0, 353400, {893, 2084, 22992}, {896, 6144, 65536}},
    {bcmL3RouteGroupGlobalV6,  11, 0, 0, 353400, {893, 2084, 22992}, {896, 6144, 65536}},

    /* ALPM template 1, V6-128 */
    {bcmL3RouteGroupPrivateV6, 1, 0, 1, 261620, {1277, 2554, 16582}, {1280, 6144, 65536}},
    {bcmL3RouteGroupGlobalV6,  1, 0, 1, 261620, {1277, 2554, 16582}, {1280, 6144, 65536}},

    /* ALPM template 2, V6-128 */
    {bcmL3RouteGroupPrivateV6, 2, 0, 1, 38280, {637, 717, 3287}, {640, 3072, 32768}},
    {bcmL3RouteGroupGlobalV6,  2, 0, 1, 38280, {637, 717, 3287}, {640, 3072, 32768}},

    /* ALPM template 3, V6-128 */
    {bcmL3RouteGroupPrivateV6, 3, 0, 1, 45945, {765, 861, 3952}, {768, 3072, 32768}},
    {bcmL3RouteGroupGlobalV6,  3, 0, 1, 45945, {765, 861, 3952}, {768, 3072, 32768}},

    /* ALPM template 4, V6-128 */
    {bcmL3RouteGroupPrivateV6, 4, 0, 1, 6120, {381, 382, 763}, {384, 2048, 16384}},
    {bcmL3RouteGroupGlobalV6,  4, 0, 1, 5364, {381, 381, 574}, {384, 1024, 16384}},

    /* ALPM template 5, V6-128 */
    {bcmL3RouteGroupPrivateV6, 5, 0, 1, 157760, {765, 1532, 9998}, {768, 6144, 65536}},
    {bcmL3RouteGroupGlobalV6,  5, 0, 1, 157760, {765, 1532, 9998}, {768, 6144, 65536}},

    /* ALPM template 6, V6-128 */
    {bcmL3RouteGroupPrivateV6, 6, 0, 1, 22944, {381, 383, 2103}, {384, 4096, 32768}},
    {bcmL3RouteGroupGlobalV6,  6, 0, 1, 13264, {381, 382, 1149}, {384, 2048, 32768}},

    /* ALPM template 7, V6-128 */
    {bcmL3RouteGroupPrivateV6, 7, 0, 1, 22920, {381, 429, 1943}, {387, 3072, 32768}},
    {bcmL3RouteGroupGlobalV6,  7, 0, 1, 22920, {381, 429, 1943}, {387, 3072, 32768}},

    /* ALPM template 8, V6-128 */
    {bcmL3RouteGroupPrivateV6, 8, 0, 1, 256, {256, 0, 0}, {256, 0, 0}},
    {bcmL3RouteGroupGlobalV6,  8, 0, 1, 256, {256, 0, 0}, {256, 0, 0}},

    /* ALPM template 9, V6-128 */
    {bcmL3RouteGroupPrivateV6, 9, 0, 1, 210425, {1021, 2043, 13342}, {1024, 6144, 65536}},
    {bcmL3RouteGroupGlobalV6,  9, 0, 1, 210425, {1021, 2043, 13342}, {1024, 6144, 65536}},

    /* ALPM template 10, V6-128 */
    {bcmL3RouteGroupPrivateV6, 10, 0, 1, 30624, {509, 511, 2807}, {512, 4096, 32768}},
    {bcmL3RouteGroupGlobalV6,  10, 0, 1, 15312, {509, 510, 1405}, {512, 2048, 32768}},

    /* ALPM template 11, V6-128 */
    {bcmL3RouteGroupPrivateV6, 11, 0, 1, 104980, {509, 1019, 6659}, {512, 6144, 65536}},
    {bcmL3RouteGroupGlobalV6,  11, 0, 1, 104980, {509, 1019, 6659}, {512, 6144, 65536}},
};

static alpm_grp_usage_t *alpm_test_db_usage[] = {
    alpm_test_db_usage_uft_0,   /* 0 UFT BANKs for ALPM */
    NULL,                       /* 1 UFT BANKs for ALPM */
    NULL,                       /* 2 UFT BANKs for ALPM */
    NULL,                       /* 3 UFT BANKs for ALPM */
    alpm_test_db_usage_uft_4,   /* 4 UFT BANKs for ALPM */
    alpm_test_db_usage_uft_5,   /* 5 UFT BANKs for ALPM */
    alpm_test_db_usage_uft_6,   /* 6 UFT BANKs for ALPM */
    NULL,                       /* 7 UFT BANKs for ALPM */
    alpm_test_db_usage_uft_8,   /* 8 UFT BANKs for ALPM */
};

static int alpm_test_db_usage_tblsz[] = {
    COUNTOF(alpm_test_db_usage_uft_0),   /* 0 UFT BANKs for ALPM */
    0,                                   /* 1 UFT BANKs for ALPM */
    0,                                   /* 2 UFT BANKs for ALPM */
    0,                                   /* 3 UFT BANKs for ALPM */
    COUNTOF(alpm_test_db_usage_uft_4),   /* 4 UFT BANKs for ALPM */
    COUNTOF(alpm_test_db_usage_uft_5),   /* 5 UFT BANKs for ALPM */
    COUNTOF(alpm_test_db_usage_uft_6),   /* 6 UFT BANKs for ALPM */
    0,                                   /* 7 UFT BANKs for ALPM */
    COUNTOF(alpm_test_db_usage_uft_8),   /* 8 UFT BANKs for ALPM */

};


int
bcm5699x_ltsw_l3_alpm_control_set(int unit,
                                  int alpm_temp,
                                  bcmi_ltsw_uft_mode_t uft_mode)
{
    int alpm_128b_en;
    int alpm_level, fp_level;
    int hitbit_mode;
    int comp_key_type;
    uint32_t l2_uft_bank_count, em_uft_bank_count;
    bcmi_lt_entry_t lt_ent;
    bcmi_lt_field_t flds[] = {
        { /* 0 */
            KEY_INPUT_LEVEL_1_BLOCK_0s,
            BCMI_LT_FIELD_F_SET | BCMI_LT_FIELD_F_SYMBOL, 0 , {0}
        },
        { /* 1 */
            KEY_INPUT_LEVEL_1_BLOCK_1s,
            BCMI_LT_FIELD_F_SET | BCMI_LT_FIELD_F_SYMBOL, 0 , {0}
        },
        { /* 2 */
            KEY_INPUT_LEVEL_1_BLOCK_2s,
            BCMI_LT_FIELD_F_SET | BCMI_LT_FIELD_F_SYMBOL, 0 , {0}
        },
        { /* 3 */
            KEY_INPUT_LEVEL_1_BLOCK_3s,
            BCMI_LT_FIELD_F_SET | BCMI_LT_FIELD_F_SYMBOL, 0 , {0}
        },
        { /* 4 */
            KEY_INPUT_LEVEL_1_BLOCK_4s,
            BCMI_LT_FIELD_F_SET | BCMI_LT_FIELD_F_SYMBOL, 0 , {0}
        },
        { /* 5 */
            KEY_INPUT_LEVEL_1_BLOCK_5s,
            BCMI_LT_FIELD_F_SET | BCMI_LT_FIELD_F_SYMBOL, 0 , {0}
        },
        { /* 6 */
            KEY_INPUT_LEVEL_1_BLOCK_6s,
            BCMI_LT_FIELD_F_SET | BCMI_LT_FIELD_F_SYMBOL, 0 , {0}
        },
        { /* 7 */
            KEY_INPUT_LEVEL_1_BLOCK_7s,
            BCMI_LT_FIELD_F_SET | BCMI_LT_FIELD_F_SYMBOL, 0 , {0}
        },
        { /* 8 */
            KEY_INPUT_LEVEL_1_BLOCK_8s,
            BCMI_LT_FIELD_F_SET | BCMI_LT_FIELD_F_SYMBOL, 0 , {0}
        },
        { /* 9 */
            KEY_INPUT_LEVEL_1_BLOCK_9s,
            BCMI_LT_FIELD_F_SET | BCMI_LT_FIELD_F_SYMBOL, 0 , {0}
        },
        { /* 10 */
            DB_LEVEL_1_BLOCK_0s,
            BCMI_LT_FIELD_F_SET | BCMI_LT_FIELD_F_SYMBOL, 0 , {0}
        },
        { /* 11 */
            DB_LEVEL_1_BLOCK_1s,
            BCMI_LT_FIELD_F_SET | BCMI_LT_FIELD_F_SYMBOL, 0 , {0}
        },
        { /* 12 */
            DB_LEVEL_1_BLOCK_2s,
            BCMI_LT_FIELD_F_SET | BCMI_LT_FIELD_F_SYMBOL, 0 , {0}
        },
        { /* 13 */
            DB_LEVEL_1_BLOCK_3s,
            BCMI_LT_FIELD_F_SET | BCMI_LT_FIELD_F_SYMBOL, 0 , {0}
        },
        { /* 14 */
            DB_LEVEL_1_BLOCK_4s,
            BCMI_LT_FIELD_F_SET | BCMI_LT_FIELD_F_SYMBOL, 0 , {0}
        },
        { /* 15 */
            DB_LEVEL_1_BLOCK_5s,
            BCMI_LT_FIELD_F_SET | BCMI_LT_FIELD_F_SYMBOL, 0 , {0}
        },
        { /* 16 */
            DB_LEVEL_1_BLOCK_6s,
            BCMI_LT_FIELD_F_SET | BCMI_LT_FIELD_F_SYMBOL, 0 , {0}
        },
        { /* 17 */
            DB_LEVEL_1_BLOCK_7s,
            BCMI_LT_FIELD_F_SET | BCMI_LT_FIELD_F_SYMBOL, 0 , {0}
        },
        { /* 18 */
            DB_LEVEL_1_BLOCK_8s,
            BCMI_LT_FIELD_F_SET | BCMI_LT_FIELD_F_SYMBOL, 0 , {0}
        },
        { /* 19 */
            DB_LEVEL_1_BLOCK_9s,
            BCMI_LT_FIELD_F_SET | BCMI_LT_FIELD_F_SYMBOL, 0 , {0}
        },
        { /* 20 */
            NUM_DB_0_LEVELSs, BCMI_LT_FIELD_F_SET, 0 , {0}
        },
        { /* 21 */
            NUM_DB_1_LEVELSs, BCMI_LT_FIELD_F_SET, 0 , {0}
        },
        { /* 22 */
            NUM_DB_2_LEVELSs, BCMI_LT_FIELD_F_SET, 0 , {0}
        },
        { /* 23 */
            NUM_DB_3_LEVELSs, BCMI_LT_FIELD_F_SET, 0 , {0}
        },
        { /* 24 */
            HIT_MODEs,
            BCMI_LT_FIELD_F_SET | BCMI_LT_FIELD_F_SYMBOL, 0 , {0}
        },
        { /* 25 */
            COMP_KEY_TYPEs,
            BCMI_LT_FIELD_F_SET | BCMI_LT_FIELD_F_SYMBOL, 0 , {0}
        }
    };

    static const char *hit_mode_str[] = {
        ALPM_HIT_MODE_DISABLEs,
        ALPM_HIT_MODE_FORCE_CLEARs,
        ALPM_HIT_MODE_FORCE_SETs,
    };

    static const char *comp_key_type_str[] = {
        ALPM_COMP_KEY_L4_PORTs,
        ALPM_COMP_KEY_VRFs,
        ALPM_COMP_KEY_FULLs,
    };

    SHR_FUNC_ENTER(unit);
    hitbit_mode =
        bcmi_ltsw_property_get(unit, BCMI_CPN_L3_ALPM_HIT_MODE, 0);

    SHR_IF_ERR_EXIT
        (bcmi_ltsw_uft_bank_max_get(unit, bcmHashTableExactMatch,
                                    &em_uft_bank_count));
    if (em_uft_bank_count > 0) {
        fp_level = 2;
    } else {
        fp_level = 3;
    }

    SHR_IF_ERR_EXIT
        (bcmi_ltsw_uft_bank_max_get(unit, bcmHashTableL2, &l2_uft_bank_count));

    if (l2_uft_bank_count == 4) {
        alpm_level = 2;
        fp_level = 2;
    } else {
        alpm_level = 3;
    }

    flds[24].u.sym_val = hit_mode_str[hitbit_mode];

    comp_key_type = bcmi_ltsw_property_get(unit,
                                          BCMI_CPN_COMP_KEY_TYPE,
                                          1);
    flds[25].u.sym_val = comp_key_type_str[comp_key_type];

    alpm_128b_en = bcmi_ltsw_property_get(unit,
                                          BCMI_CPN_IPV6_LPM_128B_ENABLE,
                                          1);
    switch (alpm_temp) {
        case 1: /* Combined */
            if (alpm_128b_en) {
                flds[0].u.sym_val = ALPM_KEY_INPUT_LPM_L3MC_QUADs;
                flds[1].u.sym_val = ALPM_KEY_INPUT_LPM_DST_QUADs;
                flds[2].u.sym_val = ALPM_KEY_INPUT_LPM_DST_QUADs;
                flds[3].u.sym_val = ALPM_KEY_INPUT_LPM_DST_QUADs;
                flds[4].u.sym_val = ALPM_KEY_INPUT_LPM_DST_QUADs;
                flds[5].u.sym_val = ALPM_KEY_INPUT_LPM_DST_QUADs;
                flds[6].u.sym_val = ALPM_KEY_INPUT_LPM_DST_QUADs;
                flds[7].u.sym_val = ALPM_KEY_INPUT_LPM_DST_QUADs;
                flds[8].u.sym_val = ALPM_KEY_INPUT_LPM_DST_QUADs;
                flds[9].u.sym_val = ALPM_KEY_INPUT_LPM_DST_QUADs;
            } else {
                flds[0].u.sym_val = ALPM_KEY_INPUT_LPM_L3MC_QUADs;
                flds[1].u.sym_val = ALPM_KEY_INPUT_LPM_DST_DOUBLEs;
                flds[2].u.sym_val = ALPM_KEY_INPUT_LPM_DST_DOUBLEs;
                flds[3].u.sym_val = ALPM_KEY_INPUT_LPM_DST_DOUBLEs;
                flds[4].u.sym_val = ALPM_KEY_INPUT_LPM_DST_DOUBLEs;
                flds[5].u.sym_val = ALPM_KEY_INPUT_LPM_DST_DOUBLEs;
                flds[6].u.sym_val = ALPM_KEY_INPUT_LPM_DST_DOUBLEs;
                flds[7].u.sym_val = ALPM_KEY_INPUT_LPM_DST_DOUBLEs;
                flds[8].u.sym_val = ALPM_KEY_INPUT_LPM_DST_DOUBLEs;
                flds[9].u.sym_val = ALPM_KEY_INPUT_LPM_DST_DOUBLEs;
            }
            flds[10].u.sym_val = ALPM_DB_0s;
            flds[11].u.sym_val = ALPM_DB_0s;
            flds[12].u.sym_val = ALPM_DB_0s;
            flds[13].u.sym_val = ALPM_DB_0s;
            flds[14].u.sym_val = ALPM_DB_0s;
            flds[15].u.sym_val = ALPM_DB_0s;
            flds[16].u.sym_val = ALPM_DB_0s;
            flds[17].u.sym_val = ALPM_DB_0s;
            flds[18].u.sym_val = ALPM_DB_0s;
            flds[19].u.sym_val = ALPM_DB_0s;
            flds[20].u.val = alpm_level;
            flds[21].u.val = 0;
            flds[22].u.val = 0;
            flds[23].u.val = 0;
            break;
        case 2: /* Parallel */
            if (alpm_128b_en) {
                flds[0].u.sym_val = ALPM_KEY_INPUT_LPM_L3MC_QUADs;
                flds[1].u.sym_val = ALPM_KEY_INPUT_LPM_DST_QUADs;
                flds[2].u.sym_val = ALPM_KEY_INPUT_LPM_DST_QUADs;
                flds[3].u.sym_val = ALPM_KEY_INPUT_LPM_DST_QUADs;
                flds[4].u.sym_val = ALPM_KEY_INPUT_LPM_DST_QUADs;
                flds[5].u.sym_val = ALPM_KEY_INPUT_LPM_DST_QUADs;
                flds[6].u.sym_val = ALPM_KEY_INPUT_LPM_DST_QUADs;
                flds[7].u.sym_val = ALPM_KEY_INPUT_LPM_DST_QUADs;
                flds[8].u.sym_val = ALPM_KEY_INPUT_LPM_DST_QUADs;
                flds[9].u.sym_val = ALPM_KEY_INPUT_LPM_DST_QUADs;
            } else {
                flds[0].u.sym_val = ALPM_KEY_INPUT_LPM_L3MC_QUADs;
                flds[1].u.sym_val = ALPM_KEY_INPUT_LPM_DST_DOUBLEs;
                flds[2].u.sym_val = ALPM_KEY_INPUT_LPM_DST_DOUBLEs;
                flds[3].u.sym_val = ALPM_KEY_INPUT_LPM_DST_DOUBLEs;
                flds[4].u.sym_val = ALPM_KEY_INPUT_LPM_DST_DOUBLEs;
                flds[5].u.sym_val = ALPM_KEY_INPUT_LPM_DST_DOUBLEs;
                flds[6].u.sym_val = ALPM_KEY_INPUT_LPM_DST_DOUBLEs;
                flds[7].u.sym_val = ALPM_KEY_INPUT_LPM_DST_DOUBLEs;
                flds[8].u.sym_val = ALPM_KEY_INPUT_LPM_DST_DOUBLEs;
                flds[9].u.sym_val = ALPM_KEY_INPUT_LPM_DST_DOUBLEs;
            }
            flds[10].u.sym_val = ALPM_DB_0s;
            flds[11].u.sym_val = ALPM_DB_0s;
            flds[12].u.sym_val = ALPM_DB_0s;
            flds[13].u.sym_val = ALPM_DB_0s;
            flds[14].u.sym_val = ALPM_DB_0s;
            flds[15].u.sym_val = ALPM_DB_1s;
            flds[16].u.sym_val = ALPM_DB_1s;
            flds[17].u.sym_val = ALPM_DB_1s;
            flds[18].u.sym_val = ALPM_DB_1s;
            flds[19].u.sym_val = ALPM_DB_1s;
            flds[20].u.val = alpm_level;
            flds[21].u.val = alpm_level;
            flds[22].u.val = 0;
            flds[23].u.val = 0;
            break;
        case 3: /* Combined + FP_COMP */
            if (alpm_128b_en) {
                flds[0].u.sym_val = ALPM_KEY_INPUT_LPM_L3MC_QUADs;
                flds[1].u.sym_val = ALPM_KEY_INPUT_LPM_DST_QUADs;
                flds[2].u.sym_val = ALPM_KEY_INPUT_LPM_DST_QUADs;
                flds[3].u.sym_val = ALPM_KEY_INPUT_LPM_DST_QUADs;
                flds[4].u.sym_val = ALPM_KEY_INPUT_LPM_DST_QUADs;
                flds[5].u.sym_val = ALPM_KEY_INPUT_LPM_DST_QUADs;
                flds[6].u.sym_val = ALPM_KEY_INPUT_FP_COMP_DSTs;
                flds[7].u.sym_val = ALPM_KEY_INPUT_FP_COMP_DSTs;
                flds[8].u.sym_val = ALPM_KEY_INPUT_FP_COMP_SRCs;
                flds[9].u.sym_val = ALPM_KEY_INPUT_FP_COMP_SRCs;
            } else {
                flds[0].u.sym_val = ALPM_KEY_INPUT_LPM_L3MC_QUADs;
                flds[1].u.sym_val = ALPM_KEY_INPUT_LPM_DST_DOUBLEs;
                flds[2].u.sym_val = ALPM_KEY_INPUT_LPM_DST_DOUBLEs;
                flds[3].u.sym_val = ALPM_KEY_INPUT_LPM_DST_DOUBLEs;
                flds[4].u.sym_val = ALPM_KEY_INPUT_LPM_DST_DOUBLEs;
                flds[5].u.sym_val = ALPM_KEY_INPUT_LPM_DST_DOUBLEs;
                flds[6].u.sym_val = ALPM_KEY_INPUT_FP_COMP_DSTs;
                flds[7].u.sym_val = ALPM_KEY_INPUT_FP_COMP_DSTs;
                flds[8].u.sym_val = ALPM_KEY_INPUT_FP_COMP_SRCs;
                flds[9].u.sym_val = ALPM_KEY_INPUT_FP_COMP_SRCs;
            }
            flds[10].u.sym_val = ALPM_DB_0s;
            flds[11].u.sym_val = ALPM_DB_0s;
            flds[12].u.sym_val = ALPM_DB_0s;
            flds[13].u.sym_val = ALPM_DB_0s;
            flds[14].u.sym_val = ALPM_DB_0s;
            flds[15].u.sym_val = ALPM_DB_0s;
            flds[16].u.sym_val = ALPM_DB_2s;
            flds[17].u.sym_val = ALPM_DB_2s;
            flds[18].u.sym_val = ALPM_DB_3s;
            flds[19].u.sym_val = ALPM_DB_3s;
            flds[20].u.val = alpm_level;
            flds[21].u.val = 0;
            flds[22].u.val = fp_level;
            flds[23].u.val = fp_level;
            break;
        case 9: /* Combined + less FP_COMP */
            if (alpm_128b_en) {
                flds[0].u.sym_val = ALPM_KEY_INPUT_LPM_L3MC_QUADs;
                flds[1].u.sym_val = ALPM_KEY_INPUT_LPM_DST_QUADs;
                flds[2].u.sym_val = ALPM_KEY_INPUT_LPM_DST_QUADs;
                flds[3].u.sym_val = ALPM_KEY_INPUT_LPM_DST_QUADs;
                flds[4].u.sym_val = ALPM_KEY_INPUT_LPM_DST_QUADs;
                flds[5].u.sym_val = ALPM_KEY_INPUT_LPM_DST_QUADs;
                flds[6].u.sym_val = ALPM_KEY_INPUT_LPM_DST_QUADs;
                flds[7].u.sym_val = ALPM_KEY_INPUT_LPM_DST_QUADs;
                flds[8].u.sym_val = ALPM_KEY_INPUT_FP_COMP_DSTs;
                flds[9].u.sym_val = ALPM_KEY_INPUT_FP_COMP_SRCs;
            } else {
                flds[0].u.sym_val = ALPM_KEY_INPUT_LPM_L3MC_QUADs;
                flds[1].u.sym_val = ALPM_KEY_INPUT_LPM_DST_DOUBLEs;
                flds[2].u.sym_val = ALPM_KEY_INPUT_LPM_DST_DOUBLEs;
                flds[3].u.sym_val = ALPM_KEY_INPUT_LPM_DST_DOUBLEs;
                flds[4].u.sym_val = ALPM_KEY_INPUT_LPM_DST_DOUBLEs;
                flds[5].u.sym_val = ALPM_KEY_INPUT_LPM_DST_DOUBLEs;
                flds[6].u.sym_val = ALPM_KEY_INPUT_LPM_DST_DOUBLEs;
                flds[7].u.sym_val = ALPM_KEY_INPUT_LPM_DST_DOUBLEs;
                flds[8].u.sym_val = ALPM_KEY_INPUT_FP_COMP_DSTs;
                flds[9].u.sym_val = ALPM_KEY_INPUT_FP_COMP_SRCs;
            }
            flds[10].u.sym_val = ALPM_DB_0s;
            flds[11].u.sym_val = ALPM_DB_0s;
            flds[12].u.sym_val = ALPM_DB_0s;
            flds[13].u.sym_val = ALPM_DB_0s;
            flds[14].u.sym_val = ALPM_DB_0s;
            flds[15].u.sym_val = ALPM_DB_0s;
            flds[16].u.sym_val = ALPM_DB_0s;
            flds[17].u.sym_val = ALPM_DB_0s;
            flds[18].u.sym_val = ALPM_DB_2s;
            flds[19].u.sym_val = ALPM_DB_3s;
            flds[20].u.val = alpm_level;
            flds[21].u.val = 0;
            flds[22].u.val = 1;
            flds[23].u.val = 1;
            break;
        case 4: /* Parallel + FP_COMP */
            if (alpm_128b_en) {
                flds[0].u.sym_val = ALPM_KEY_INPUT_LPM_L3MC_QUADs;
                flds[1].u.sym_val = ALPM_KEY_INPUT_LPM_DST_QUADs;
                flds[2].u.sym_val = ALPM_KEY_INPUT_LPM_DST_QUADs;
                flds[3].u.sym_val = ALPM_KEY_INPUT_LPM_DST_QUADs;
                flds[4].u.sym_val = ALPM_KEY_INPUT_LPM_DST_QUADs;
                flds[5].u.sym_val = ALPM_KEY_INPUT_LPM_DST_QUADs;
                flds[6].u.sym_val = ALPM_KEY_INPUT_FP_COMP_DSTs;
                flds[7].u.sym_val = ALPM_KEY_INPUT_FP_COMP_DSTs;
                flds[8].u.sym_val = ALPM_KEY_INPUT_FP_COMP_SRCs;
                flds[9].u.sym_val = ALPM_KEY_INPUT_FP_COMP_SRCs;
            } else {
                flds[0].u.sym_val = ALPM_KEY_INPUT_LPM_L3MC_QUADs;
                flds[1].u.sym_val = ALPM_KEY_INPUT_LPM_DST_DOUBLEs;
                flds[2].u.sym_val = ALPM_KEY_INPUT_LPM_DST_DOUBLEs;
                flds[3].u.sym_val = ALPM_KEY_INPUT_LPM_DST_DOUBLEs;
                flds[4].u.sym_val = ALPM_KEY_INPUT_LPM_DST_DOUBLEs;
                flds[5].u.sym_val = ALPM_KEY_INPUT_LPM_DST_DOUBLEs;
                flds[6].u.sym_val = ALPM_KEY_INPUT_FP_COMP_DSTs;
                flds[7].u.sym_val = ALPM_KEY_INPUT_FP_COMP_DSTs;
                flds[8].u.sym_val = ALPM_KEY_INPUT_FP_COMP_SRCs;
                flds[9].u.sym_val = ALPM_KEY_INPUT_FP_COMP_SRCs;
            }
            flds[10].u.sym_val = ALPM_DB_0s;
            flds[11].u.sym_val = ALPM_DB_0s;
            flds[12].u.sym_val = ALPM_DB_0s;
            flds[13].u.sym_val = ALPM_DB_1s;
            flds[14].u.sym_val = ALPM_DB_1s;
            flds[15].u.sym_val = ALPM_DB_1s;
            flds[16].u.sym_val = ALPM_DB_2s;
            flds[17].u.sym_val = ALPM_DB_2s;
            flds[18].u.sym_val = ALPM_DB_3s;
            flds[19].u.sym_val = ALPM_DB_3s;
            flds[20].u.val = alpm_level;
            flds[21].u.val = alpm_level;
            flds[22].u.val = fp_level;
            flds[23].u.val = fp_level;
            break;
        case 10: /* Parallel + less FP_COMP */
            if (alpm_128b_en) {
                flds[0].u.sym_val = ALPM_KEY_INPUT_LPM_L3MC_QUADs;
                flds[1].u.sym_val = ALPM_KEY_INPUT_LPM_DST_QUADs;
                flds[2].u.sym_val = ALPM_KEY_INPUT_LPM_DST_QUADs;
                flds[3].u.sym_val = ALPM_KEY_INPUT_LPM_DST_QUADs;
                flds[4].u.sym_val = ALPM_KEY_INPUT_LPM_DST_QUADs;
                flds[5].u.sym_val = ALPM_KEY_INPUT_LPM_DST_QUADs;
                flds[6].u.sym_val = ALPM_KEY_INPUT_LPM_DST_QUADs;
                flds[7].u.sym_val = ALPM_KEY_INPUT_LPM_DST_QUADs;
                flds[8].u.sym_val = ALPM_KEY_INPUT_FP_COMP_DSTs;
                flds[9].u.sym_val = ALPM_KEY_INPUT_FP_COMP_SRCs;
            } else {
                flds[0].u.sym_val = ALPM_KEY_INPUT_LPM_L3MC_QUADs;
                flds[1].u.sym_val = ALPM_KEY_INPUT_LPM_DST_DOUBLEs;
                flds[2].u.sym_val = ALPM_KEY_INPUT_LPM_DST_DOUBLEs;
                flds[3].u.sym_val = ALPM_KEY_INPUT_LPM_DST_DOUBLEs;
                flds[4].u.sym_val = ALPM_KEY_INPUT_LPM_DST_DOUBLEs;
                flds[5].u.sym_val = ALPM_KEY_INPUT_LPM_DST_DOUBLEs;
                flds[6].u.sym_val = ALPM_KEY_INPUT_LPM_DST_DOUBLEs;
                flds[7].u.sym_val = ALPM_KEY_INPUT_LPM_DST_DOUBLEs;
                flds[8].u.sym_val = ALPM_KEY_INPUT_FP_COMP_DSTs;
                flds[9].u.sym_val = ALPM_KEY_INPUT_FP_COMP_SRCs;
            }
            flds[10].u.sym_val = ALPM_DB_0s;
            flds[11].u.sym_val = ALPM_DB_0s;
            flds[12].u.sym_val = ALPM_DB_0s;
            flds[13].u.sym_val = ALPM_DB_0s;
            flds[14].u.sym_val = ALPM_DB_1s;
            flds[15].u.sym_val = ALPM_DB_1s;
            flds[16].u.sym_val = ALPM_DB_1s;
            flds[17].u.sym_val = ALPM_DB_1s;
            flds[18].u.sym_val = ALPM_DB_2s;
            flds[19].u.sym_val = ALPM_DB_3s;
            flds[20].u.val = alpm_level;
            flds[21].u.val = alpm_level;
            flds[22].u.val = 1;
            flds[23].u.val = 1;
            break;
        case 5: /* Combined + SIP lookup */
            if (alpm_128b_en) {
                flds[0].u.sym_val = ALPM_KEY_INPUT_LPM_L3MC_QUADs;
                flds[1].u.sym_val = ALPM_KEY_INPUT_LPM_DST_QUADs;
                flds[2].u.sym_val = ALPM_KEY_INPUT_LPM_DST_QUADs;
                flds[3].u.sym_val = ALPM_KEY_INPUT_LPM_DST_QUADs;
                flds[4].u.sym_val = ALPM_KEY_INPUT_LPM_DST_QUADs;
                flds[5].u.sym_val = ALPM_KEY_INPUT_LPM_DST_QUADs;
                flds[6].u.sym_val = ALPM_KEY_INPUT_LPM_SRC_QUADs;
                flds[7].u.sym_val = ALPM_KEY_INPUT_LPM_SRC_QUADs;
                flds[8].u.sym_val = ALPM_KEY_INPUT_LPM_SRC_QUADs;
                flds[9].u.sym_val = ALPM_KEY_INPUT_LPM_SRC_QUADs;
            } else {
                flds[0].u.sym_val = ALPM_KEY_INPUT_LPM_L3MC_QUADs;
                flds[1].u.sym_val = ALPM_KEY_INPUT_LPM_DST_DOUBLEs;
                flds[2].u.sym_val = ALPM_KEY_INPUT_LPM_DST_DOUBLEs;
                flds[3].u.sym_val = ALPM_KEY_INPUT_LPM_DST_DOUBLEs;
                flds[4].u.sym_val = ALPM_KEY_INPUT_LPM_DST_DOUBLEs;
                flds[5].u.sym_val = ALPM_KEY_INPUT_LPM_DST_DOUBLEs;
                flds[6].u.sym_val = ALPM_KEY_INPUT_LPM_SRC_DOUBLEs;
                flds[7].u.sym_val = ALPM_KEY_INPUT_LPM_SRC_DOUBLEs;
                flds[8].u.sym_val = ALPM_KEY_INPUT_LPM_SRC_DOUBLEs;
                flds[9].u.sym_val = ALPM_KEY_INPUT_LPM_SRC_DOUBLEs;
            }
            flds[10].u.sym_val = ALPM_DB_0s;
            flds[11].u.sym_val = ALPM_DB_0s;
            flds[12].u.sym_val = ALPM_DB_0s;
            flds[13].u.sym_val = ALPM_DB_0s;
            flds[14].u.sym_val = ALPM_DB_0s;
            flds[15].u.sym_val = ALPM_DB_0s;
            flds[16].u.sym_val = ALPM_DB_2s;
            flds[17].u.sym_val = ALPM_DB_2s;
            flds[18].u.sym_val = ALPM_DB_2s;
            flds[19].u.sym_val = ALPM_DB_2s;
            flds[20].u.val = alpm_level;
            flds[21].u.val = 0;
            flds[22].u.val = 1;
            flds[23].u.val = 0;
            break;
        case 6: /* Parallel + SIP lookup */
            if (alpm_128b_en) {
                flds[0].u.sym_val = ALPM_KEY_INPUT_LPM_L3MC_QUADs;
                flds[1].u.sym_val = ALPM_KEY_INPUT_LPM_DST_QUADs;
                flds[2].u.sym_val = ALPM_KEY_INPUT_LPM_DST_QUADs;
                flds[3].u.sym_val = ALPM_KEY_INPUT_LPM_DST_QUADs;
                flds[4].u.sym_val = ALPM_KEY_INPUT_LPM_DST_QUADs;
                flds[5].u.sym_val = ALPM_KEY_INPUT_LPM_DST_QUADs;
                flds[6].u.sym_val = ALPM_KEY_INPUT_LPM_SRC_QUADs;
                flds[7].u.sym_val = ALPM_KEY_INPUT_LPM_SRC_QUADs;
                flds[8].u.sym_val = ALPM_KEY_INPUT_LPM_SRC_QUADs;
                flds[9].u.sym_val = ALPM_KEY_INPUT_LPM_SRC_QUADs;
            } else {
                flds[0].u.sym_val = ALPM_KEY_INPUT_LPM_L3MC_QUADs;
                flds[1].u.sym_val = ALPM_KEY_INPUT_LPM_DST_DOUBLEs;
                flds[2].u.sym_val = ALPM_KEY_INPUT_LPM_DST_DOUBLEs;
                flds[3].u.sym_val = ALPM_KEY_INPUT_LPM_DST_DOUBLEs;
                flds[4].u.sym_val = ALPM_KEY_INPUT_LPM_DST_DOUBLEs;
                flds[5].u.sym_val = ALPM_KEY_INPUT_LPM_DST_DOUBLEs;
                flds[6].u.sym_val = ALPM_KEY_INPUT_LPM_SRC_DOUBLEs;
                flds[7].u.sym_val = ALPM_KEY_INPUT_LPM_SRC_DOUBLEs;
                flds[8].u.sym_val = ALPM_KEY_INPUT_LPM_SRC_DOUBLEs;
                flds[9].u.sym_val = ALPM_KEY_INPUT_LPM_SRC_DOUBLEs;
            }
            flds[10].u.sym_val = ALPM_DB_0s;
            flds[11].u.sym_val = ALPM_DB_0s;
            flds[12].u.sym_val = ALPM_DB_0s;
            flds[13].u.sym_val = ALPM_DB_1s;
            flds[14].u.sym_val = ALPM_DB_1s;
            flds[15].u.sym_val = ALPM_DB_1s;
            flds[16].u.sym_val = ALPM_DB_2s;
            flds[17].u.sym_val = ALPM_DB_2s;
            flds[18].u.sym_val = ALPM_DB_3s;
            flds[19].u.sym_val = ALPM_DB_3s;
            flds[20].u.val = alpm_level;
            flds[21].u.val = alpm_level;
            flds[22].u.val = 1;
            flds[23].u.val = 1;
            break;
        case 7: /* Combined + FP_COMP + SIP lookup */
            if (alpm_128b_en) {
                flds[0].u.sym_val = ALPM_KEY_INPUT_LPM_L3MC_QUADs;
                flds[1].u.sym_val = ALPM_KEY_INPUT_LPM_DST_QUADs;
                flds[2].u.sym_val = ALPM_KEY_INPUT_LPM_DST_QUADs;
                flds[3].u.sym_val = ALPM_KEY_INPUT_LPM_SRC_QUADs;
                flds[4].u.sym_val = ALPM_KEY_INPUT_LPM_SRC_QUADs;
                flds[5].u.sym_val = ALPM_KEY_INPUT_LPM_SRC_QUADs;
                flds[6].u.sym_val = ALPM_KEY_INPUT_FP_COMP_DSTs;
                flds[7].u.sym_val = ALPM_KEY_INPUT_FP_COMP_DSTs;
                flds[8].u.sym_val = ALPM_KEY_INPUT_FP_COMP_SRCs;
                flds[9].u.sym_val = ALPM_KEY_INPUT_FP_COMP_SRCs;
            } else {
                flds[0].u.sym_val = ALPM_KEY_INPUT_LPM_L3MC_QUADs;
                flds[1].u.sym_val = ALPM_KEY_INPUT_LPM_DST_DOUBLEs;
                flds[2].u.sym_val = ALPM_KEY_INPUT_LPM_DST_DOUBLEs;
                flds[3].u.sym_val = ALPM_KEY_INPUT_LPM_SRC_DOUBLEs;
                flds[4].u.sym_val = ALPM_KEY_INPUT_LPM_SRC_DOUBLEs;
                flds[5].u.sym_val = ALPM_KEY_INPUT_LPM_SRC_DOUBLEs;
                flds[6].u.sym_val = ALPM_KEY_INPUT_FP_COMP_DSTs;
                flds[7].u.sym_val = ALPM_KEY_INPUT_FP_COMP_DSTs;
                flds[8].u.sym_val = ALPM_KEY_INPUT_FP_COMP_SRCs;
                flds[9].u.sym_val = ALPM_KEY_INPUT_FP_COMP_SRCs;
            }
            flds[10].u.sym_val = ALPM_DB_0s;
            flds[11].u.sym_val = ALPM_DB_0s;
            flds[12].u.sym_val = ALPM_DB_0s;
            flds[13].u.sym_val = ALPM_DB_1s;
            flds[14].u.sym_val = ALPM_DB_1s;
            flds[15].u.sym_val = ALPM_DB_1s;
            flds[16].u.sym_val = ALPM_DB_2s;
            flds[17].u.sym_val = ALPM_DB_2s;
            flds[18].u.sym_val = ALPM_DB_3s;
            flds[19].u.sym_val = ALPM_DB_3s;
            flds[20].u.val = alpm_level;
            flds[21].u.val = 1;
            flds[22].u.val = fp_level;
            flds[23].u.val = fp_level;
            break;
        case 11: /* Combined + less FP_COMP + SIP lookup */
            if (alpm_128b_en) {
                flds[0].u.sym_val = ALPM_KEY_INPUT_LPM_L3MC_QUADs;
                flds[1].u.sym_val = ALPM_KEY_INPUT_LPM_DST_QUADs;
                flds[2].u.sym_val = ALPM_KEY_INPUT_LPM_DST_QUADs;
                flds[3].u.sym_val = ALPM_KEY_INPUT_LPM_DST_QUADs;
                flds[4].u.sym_val = ALPM_KEY_INPUT_LPM_SRC_QUADs;
                flds[5].u.sym_val = ALPM_KEY_INPUT_LPM_SRC_QUADs;
                flds[6].u.sym_val = ALPM_KEY_INPUT_LPM_SRC_QUADs;
                flds[7].u.sym_val = ALPM_KEY_INPUT_LPM_SRC_QUADs;
                flds[8].u.sym_val = ALPM_KEY_INPUT_FP_COMP_DSTs;
                flds[9].u.sym_val = ALPM_KEY_INPUT_FP_COMP_SRCs;
            } else {
                flds[0].u.sym_val = ALPM_KEY_INPUT_LPM_L3MC_QUADs;
                flds[1].u.sym_val = ALPM_KEY_INPUT_LPM_DST_DOUBLEs;
                flds[2].u.sym_val = ALPM_KEY_INPUT_LPM_DST_DOUBLEs;
                flds[3].u.sym_val = ALPM_KEY_INPUT_LPM_DST_DOUBLEs;
                flds[4].u.sym_val = ALPM_KEY_INPUT_LPM_SRC_DOUBLEs;
                flds[5].u.sym_val = ALPM_KEY_INPUT_LPM_SRC_DOUBLEs;
                flds[6].u.sym_val = ALPM_KEY_INPUT_LPM_SRC_DOUBLEs;
                flds[7].u.sym_val = ALPM_KEY_INPUT_LPM_SRC_DOUBLEs;
                flds[8].u.sym_val = ALPM_KEY_INPUT_FP_COMP_DSTs;
                flds[9].u.sym_val = ALPM_KEY_INPUT_FP_COMP_SRCs;
            }
            flds[10].u.sym_val = ALPM_DB_0s;
            flds[11].u.sym_val = ALPM_DB_0s;
            flds[12].u.sym_val = ALPM_DB_0s;
            flds[13].u.sym_val = ALPM_DB_0s;
            flds[14].u.sym_val = ALPM_DB_1s;
            flds[15].u.sym_val = ALPM_DB_1s;
            flds[16].u.sym_val = ALPM_DB_1s;
            flds[17].u.sym_val = ALPM_DB_1s;
            flds[18].u.sym_val = ALPM_DB_2s;
            flds[19].u.sym_val = ALPM_DB_3s;
            flds[20].u.val = alpm_level;
            flds[21].u.val = 1;
            flds[22].u.val = 1;
            flds[23].u.val = 1;
            break;
        case 8: /* FP_COMP */
            if (alpm_128b_en) {
                flds[0].u.sym_val = ALPM_KEY_INPUT_LPM_L3MC_QUADs;
                flds[1].u.sym_val = ALPM_KEY_INPUT_LPM_DST_QUADs;
                flds[2].u.sym_val = ALPM_KEY_INPUT_FP_COMP_DSTs;
                flds[3].u.sym_val = ALPM_KEY_INPUT_FP_COMP_DSTs;
                flds[4].u.sym_val = ALPM_KEY_INPUT_FP_COMP_DSTs;
                flds[5].u.sym_val = ALPM_KEY_INPUT_FP_COMP_DSTs;
                flds[6].u.sym_val = ALPM_KEY_INPUT_FP_COMP_SRCs;
                flds[7].u.sym_val = ALPM_KEY_INPUT_FP_COMP_SRCs;
                flds[8].u.sym_val = ALPM_KEY_INPUT_FP_COMP_SRCs;
                flds[9].u.sym_val = ALPM_KEY_INPUT_FP_COMP_SRCs;
            } else {
                flds[0].u.sym_val = ALPM_KEY_INPUT_LPM_L3MC_QUADs;
                flds[1].u.sym_val = ALPM_KEY_INPUT_LPM_DST_DOUBLEs;
                flds[2].u.sym_val = ALPM_KEY_INPUT_FP_COMP_DSTs;
                flds[3].u.sym_val = ALPM_KEY_INPUT_FP_COMP_DSTs;
                flds[4].u.sym_val = ALPM_KEY_INPUT_FP_COMP_DSTs;
                flds[5].u.sym_val = ALPM_KEY_INPUT_FP_COMP_DSTs;
                flds[6].u.sym_val = ALPM_KEY_INPUT_FP_COMP_SRCs;
                flds[7].u.sym_val = ALPM_KEY_INPUT_FP_COMP_SRCs;
                flds[8].u.sym_val = ALPM_KEY_INPUT_FP_COMP_SRCs;
                flds[9].u.sym_val = ALPM_KEY_INPUT_FP_COMP_SRCs;
            }
            flds[10].u.sym_val = ALPM_DB_0s;
            flds[11].u.sym_val = ALPM_DB_0s;
            flds[12].u.sym_val = ALPM_DB_2s;
            flds[13].u.sym_val = ALPM_DB_2s;
            flds[14].u.sym_val = ALPM_DB_2s;
            flds[15].u.sym_val = ALPM_DB_2s;
            flds[16].u.sym_val = ALPM_DB_3s;
            flds[17].u.sym_val = ALPM_DB_3s;
            flds[18].u.sym_val = ALPM_DB_3s;
            flds[19].u.sym_val = ALPM_DB_3s;
            flds[20].u.val = 1; /* DB0 */
            /* All level 3 resources are for FP COMP. */
            flds[22].u.val = alpm_level; /* DB1 */
            flds[23].u.val = alpm_level; /* DB2 */
            break;
        default:
            SHR_IF_ERR_VERBOSE_EXIT(SHR_E_CONFIG);
    }

    /* Update the L3_ALPM_CONTROL */
    lt_ent.fields = flds;
    lt_ent.nfields = sizeof(flds) / sizeof(flds[0]);
    lt_ent.attr = 0;
    SHR_IF_ERR_VERBOSE_EXIT
        (bcmi_lt_entry_update(unit, L3_ALPM_CONTROLs, &lt_ent, NULL));

exit:
    SHR_FUNC_EXIT();
}

int
bcm5699x_a0_ltsw_l3_fib_tbl_db_get(
    int unit,
    bcmint_ltsw_l3_fib_tbl_db_t *tbl_db)
{
    SHR_FUNC_ENTER(unit);

    SHR_NULL_CHECK(tbl_db, SHR_E_PARAM);

    tbl_db->tbl_bmp = (1 << BCMINT_LTSW_L3_FIB_TBL_ROUTE_IPV4_UC_GL) |
                      (1 << BCMINT_LTSW_L3_FIB_TBL_ROUTE_IPV4_UC_VRF) |
                      (1 << BCMINT_LTSW_L3_FIB_TBL_ROUTE_IPV4_UC_GH) |
                      (1 << BCMINT_LTSW_L3_FIB_TBL_ROUTE_IPV6_UC_GL) |
                      (1 << BCMINT_LTSW_L3_FIB_TBL_ROUTE_IPV6_UC_VRF) |
                      (1 << BCMINT_LTSW_L3_FIB_TBL_ROUTE_IPV6_UC_GH) |
                      (1 << BCMINT_LTSW_L3_FIB_TBL_ROUTE_SRC_IPV4_UC_GL) |
                      (1 << BCMINT_LTSW_L3_FIB_TBL_ROUTE_SRC_IPV4_UC_VRF) |
                      (1 << BCMINT_LTSW_L3_FIB_TBL_ROUTE_SRC_IPV4_UC_GH) |
                      (1 << BCMINT_LTSW_L3_FIB_TBL_ROUTE_SRC_IPV6_UC_GL) |
                      (1 << BCMINT_LTSW_L3_FIB_TBL_ROUTE_SRC_IPV6_UC_VRF) |
                      (1 << BCMINT_LTSW_L3_FIB_TBL_ROUTE_SRC_IPV6_UC_GH)  |
                      (1 << BCMINT_LTSW_L3_FIB_TBL_HOST_IPV4_UC)  |
                      (1 << BCMINT_LTSW_L3_FIB_TBL_HOST_IPV6_UC)  |
                      (1 << BCMINT_LTSW_L3_FIB_TBL_DEFIP_IPV4_MC) |
                      (1 << BCMINT_LTSW_L3_FIB_TBL_DEFIP_IPV6_MC_128) |
                      (1 << BCMINT_LTSW_L3_FIB_TBL_ALPM_LVL_1_USAGE) |
                      (1 << BCMINT_LTSW_L3_FIB_TBL_ALPM_LVL_2_USAGE) |
                      (1 << BCMINT_LTSW_L3_FIB_TBL_ALPM_LVL_3_USAGE) |
                      (1 << BCMINT_LTSW_L3_FIB_TBL_ALPM_CONTROL);;

    tbl_db->tbls = l3_fib_tbls;

exit:
    SHR_FUNC_EXIT();
}

/*!
 * \brief Route projection based on testDB.
 *
 * \param [in] unit Unit number.
 * \param [in] grp ALPM route group.
 * \param [out] prj_cnt Uni-dimensional projected maximum number of route.
 * \param [out] max_usage_lvl The level with maximum resource usage.
 * \param [out] max_usage Maximum resource usage.
 *
 * retval SHR_E_NONE No errors.
 * retval !SHR_E_NONE Failure.
 */
static int
bcm5699x_a0_l3_alpm_grp_prj_db_get(int unit,
                                             bcm_l3_route_group_t grp,
                                             int *prj_cnt,
                                             int *max_usage_lvl,
                                             int *max_usage)
{
    int alpm_uft_banks = 0;
    int alpm_temp, en_128b, i, usage, usage_m, lvl_m, lvl_thres, delta;
    alpm_grp_usage_t *db;
    uint32_t l2_uft_bank_count, em_bank_count;

    SHR_FUNC_ENTER(unit);

    SHR_NULL_CHECK(prj_cnt, SHR_E_PARAM);


    SHR_IF_ERR_EXIT
        (bcmi_ltsw_uft_bank_max_get(unit, bcmHashTableL2, &l2_uft_bank_count));

    SHR_IF_ERR_EXIT
        (bcmi_ltsw_uft_bank_max_get(unit, bcmHashTableExactMatch, &em_bank_count));

    alpm_uft_banks = 8 - l2_uft_bank_count - em_bank_count;

    alpm_temp = bcmi_ltsw_property_get(unit, BCMI_CPN_L3_ALPM_TEMPLATE, 1);
    en_128b = bcmi_ltsw_property_get(unit, BCMI_CPN_IPV6_LPM_128B_ENABLE, 1);

    db = alpm_test_db_usage[alpm_uft_banks];
    if (!db) {
        SHR_ERR_EXIT(SHR_E_INTERNAL);
    }

    for (i = 0; i < alpm_test_db_usage_tblsz[alpm_uft_banks]; i++) {
        if ((grp == db->grp) && (alpm_temp == db->alpm_temp) &&
            ((db->alpm_128b_en == -1) || (en_128b == db->alpm_128b_en))) {
            break;
        }
        db++;
    }

    if (i == alpm_test_db_usage_tblsz[alpm_uft_banks]) {
        SHR_ERR_EXIT(SHR_E_NOT_FOUND);
    }

    lvl_m = 0;
    usage_m = 0;
    for (i = 0; i < BCM_L3_ALPM_LEVELS; i++) {
        if (db->cnt_total[i] == 0) {
            break;
        }

        usage = db->cnt_inuse[i] * 10000L / db->cnt_total[i];

        /* Give 1% preference to L3 usage. */
        delta = (lvl_m == 3) ? 1 : 0;
        if ((usage + delta) > usage_m) {
            lvl_m = i + 1;
            usage_m = usage;
        }
    }

    /* Test DB result wrong. */
    if (usage_m <= 0) {
        SHR_ERR_EXIT(SHR_E_INTERNAL);
    }

    if (lvl_m < 3) {
        lvl_thres = 100;
    } else {
        lvl_thres = bcmi_ltsw_property_get(unit,
                                           BCMI_CPN_L3_ALPM_BNK_THRESHOLD,
                                           95);
    }

    *prj_cnt = db->route_cnt * lvl_thres * 100L / usage_m;
    if (max_usage) {
        *max_usage = lvl_thres;
    }
    if (max_usage_lvl) {
        *max_usage_lvl = lvl_m;
    }

exit:
    SHR_FUNC_EXIT();
}

/*!
 * \brief L3_fib sub driver functions for bcm5699x.
 */
static mbcm_ltsw_l3_fib_drv_t bcm5699x_l3_fib_sub_drv = {
    .l3_alpm_control_set = bcm5699x_ltsw_l3_alpm_control_set,
    .l3_fib_tbl_db_get = bcm5699x_a0_ltsw_l3_fib_tbl_db_get,
    /* ALPM Projection. */
    .l3_alpm_grp_prj_db_get = bcm5699x_a0_l3_alpm_grp_prj_db_get,
};

/******************************************************************************
 * Public functions
 */

int
bcm5699x_l3_fib_sub_drv_attach(int unit, void *hdl)
{
    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_VERBOSE_EXIT
        (mbcm_ltsw_l3_fib_drv_set(unit, &bcm5699x_l3_fib_sub_drv));

exit:
    SHR_FUNC_EXIT();
}
