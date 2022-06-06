#ifdef BSL_LOG_MODULE
#error "BSL_LOG_MODULE redefined"
#endif
#define BSL_LOG_MODULE BSL_LS_BCMDNX_FLOW

#include <shared/shrextend/shrextend_debug.h>
#include <bcm_int/dnx/flow/flow.h>
#include <src/bcm/dnx/flow/flow_def.h>
#include <bcm_int/dnx/switch/switch.h>
#include <soc/dnx/dnx_data/auto_generated/dnx_data_dev_init.h>

/** Supported initiator common fields */
#define VALID_RCH_INITIATOR_COMMON_FIELDS 0

/** Supported initiator special fields */
#define VALID_RCH_INITIATOR_SPECIAL_FIELDS {FLOW_S_F_RCH_VRF, \
                                            FLOW_S_F_RCH_OUTLIF_0_PUSHED_TO_STACK, \
                                            FLOW_S_F_RCH_P2P, \
                                            FLOW_S_F_RCH_DESTINATION, \
                                            FLOW_S_F_RCH_CONTROL_VLAN_PORT}

const dnx_flow_app_config_t dnx_flow_app_rch_initiator = {
    /*
     * Application name
     */
    FLOW_APP_NAME_RCH_INITIATOR,

    /** related DBAL table */
    DBAL_TABLE_EEDB_RCH,

    /** Second Pass related DBAL table */
    DBAL_TABLE_EMPTY,

    /*
     * Flow app type
     */
    FLOW_APP_TYPE_INIT,

    /*
     * encap_access_default_mapping
     */
    bcmEncapAccessAc,

    /*
     * Match payload valid applications
     */
    FLOW_NO_MATCH_PAYLOAD_APPS,

    /*
     * Bitwise for supported common fields
     */
    VALID_RCH_INITIATOR_COMMON_FIELDS,

    /*
     * specific table special fields
     */
    VALID_RCH_INITIATOR_SPECIAL_FIELDS,

    /*
     * Bitmap for supported flow handler flags
     */
    FLOW_APP_BASE_FLAGS,

    /*
     * Indications
     */
    FLOW_APP_INDICATIONS_NONE,

    /*
     * verify cb
     */
    NULL,

    /*
     * app_specific_operations_cb
     */
    NULL,

    /*
     * Generic callback for marking selectable result types - result_type_select_cb
     */
    NULL
};