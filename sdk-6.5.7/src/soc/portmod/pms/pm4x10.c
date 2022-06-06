/*
 *
 * $Id:$
 * 
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *         
 *     
 */

#include <soc/types.h>
#include <soc/error.h>
#include <soc/wb_engine.h>
#include <soc/portmod/portmod_internal.h>
#include <soc/portmod/portmod.h>
#include <soc/portmod/portmod_common.h>
#include <soc/portmod/portmod_dispatch.h>
#include <soc/portmod/portmod_chain.h>
#include <soc/portmod/portmod_legacy_phy.h>
#include <soc/portmod/portmod_system.h>


#include <soc/portmod/xlmac.h>
#include <soc/portmod/pm4x10.h>
#include <soc/portmod/pm12x10_internal.h>
#include <soc/portmod/pm12x10_xgs_internal.h>

#ifdef _ERR_MSG_MODULE_NAME 
#error "_ERR_MSG_MODULE_NAME redefined" 
#endif
#define _ERR_MSG_MODULE_NAME BSL_LS_SOC_PORT

#ifdef PORTMOD_PM4X10_SUPPORT

#if 0
#define PORTMOD_CORE_TO_PHY_ACCESS(_phy_access, _core_access) \
    do{\
        PHYMOD_MEMCPY(&(_phy_access)->access, &(_core_access)->access, sizeof((_phy_access)->access));\
        (_phy_access)->type = (_core_access)->type; \
    }while(0)
#endif

/* 10G mode at 10.3125G and 1G mode at 1.25G */
#define PM4X10_LANES_PER_CORE (4)
#define MAX_PORTS_PER_PM4X10 (4)

#define PM_4x10_INFO(pm_info) ((pm_info)->pm_data.pm4x10_db)

#define PM4x10_PHY_ACCESS_GET(unit, port, pm_info, phy_acc) \
    do { \
        uint32 is_bypass; \
        uint8 in_pm12x10 = PM_4x10_INFO(pm_info)->in_pm12x10; \
        phy_acc = 0; \
        SOC_WB_ENGINE_GET_VAR(unit, SOC_WB_ENGINE_PORTMOD, pm_info->wb_vars_ids[isBypassed], &is_bypass); \
        phy_acc = (is_bypass && !in_pm12x10 && PM_4x10_INFO(pm_info)->first_phy != -1) ? (PM_4x10_INFO(pm_info)->first_phy | SOC_REG_ADDR_PHY_ACC_MASK) : port; \
    } while (0)

#define PM4x10_WB_BUFFER_VERSION        (7)

#define PM4x10_PORT_DYNAMIC_STATE_SET(unit, pm_info, dynamic_state, \
                                      port_index)                   \
    SOC_WB_ENGINE_SET_ARR(unit, SOC_WB_ENGINE_PORTMOD,              \
                          pm_info->wb_vars_ids[portDynamicState],   \
                          &dynamic_state, port_index)
#define PM4x10_PORT_DYNAMIC_STATE_GET(unit, pm_info, dynamic_state, \
                                      port_index)                   \
    SOC_WB_ENGINE_GET_ARR(unit, SOC_WB_ENGINE_PORTMOD,              \
                          pm_info->wb_vars_ids[portDynamicState],   \
                          dynamic_state, port_index)
#define PM4x10_SPEED_SET(unit, pm_info, speed, port_index)     \
    SOC_WB_ENGINE_SET_ARR(unit, SOC_WB_ENGINE_PORTMOD,         \
                          pm_info->wb_vars_ids[dataRate],      \
                          &speed, port_index)
#define PM4x10_SPEED_GET(unit, pm_info, speed, port_index)     \
    SOC_WB_ENGINE_GET_ARR(unit, SOC_WB_ENGINE_PORTMOD,         \
                          pm_info->wb_vars_ids[dataRate],      \
                          speed, port_index)
#define PM4x10_MAX_SPEED_SET(unit, pm_info, max_speed, port_index) \
    SOC_WB_ENGINE_SET_ARR(unit, SOC_WB_ENGINE_PORTMOD,             \
                          pm_info->wb_vars_ids[maxSpeed],          \
                          &max_speed, port_index)
#define PM4x10_MAX_SPEED_GET(unit, pm_info, max_speed, port_index) \
    SOC_WB_ENGINE_GET_ARR(unit, SOC_WB_ENGINE_PORTMOD,             \
                          pm_info->wb_vars_ids[maxSpeed],          \
                          max_speed, port_index)
#define PM4x10_INTERFACE_TYPE_SET(unit, pm_info, interface_type, port_index) \
    SOC_WB_ENGINE_SET_ARR(unit, SOC_WB_ENGINE_PORTMOD,                       \
                          pm_info->wb_vars_ids[interfaceType],               \
                          &interface_type, port_index)
#define PM4x10_INTERFACE_TYPE_GET(unit, pm_info, interface_type, port_index) \
    SOC_WB_ENGINE_GET_ARR(unit, SOC_WB_ENGINE_PORTMOD,                       \
                          pm_info->wb_vars_ids[interfaceType],               \
                          interface_type, port_index)
#define PM4x10_OUTMOST_PHY_INTERFACE_TYPE_SET(unit, pm_info, interface_type, port_index) \
    SOC_WB_ENGINE_SET_ARR(unit, SOC_WB_ENGINE_PORTMOD,                                   \
                          pm_info->wb_vars_ids[interfaceWrittenIntoOutmostPhy],          \
                          &interface_type, port_index)
#define PM4x10_OUTMOST_PHY_INTERFACE_TYPE_GET(unit, pm_info, interface_type, port_index) \
    SOC_WB_ENGINE_GET_ARR(unit, SOC_WB_ENGINE_PORTMOD,                                   \
                          pm_info->wb_vars_ids[interfaceWrittenIntoOutmostPhy],          \
                          interface_type, port_index)
#define PM4x10_SERDES_INTERFACE_TYPE_SET(unit, pm_info, serdes_interface,    \
                                         port_index)                         \
    SOC_WB_ENGINE_SET_ARR(unit, SOC_WB_ENGINE_PORTMOD,                       \
                          pm_info->wb_vars_ids[serdesInterfaceType],         \
                          &serdes_interface, port_index)
#define PM4x10_SERDES_INTERFACE_TYPE_GET(unit, pm_info, serdes_interface,    \
                                         port_index)                         \
    SOC_WB_ENGINE_GET_ARR(unit, SOC_WB_ENGINE_PORTMOD,                       \
                          pm_info->wb_vars_ids[serdesInterfaceType],         \
                          serdes_interface, port_index)
#define PM4x10_INTERFACE_MODES_SET(unit, pm_info, interface_modes, port_index) \
    SOC_WB_ENGINE_SET_ARR(unit, SOC_WB_ENGINE_PORTMOD,                         \
                          pm_info->wb_vars_ids[interfaceModes],                \
                          &interface_modes, port_index)
#define PM4x10_INTERFACE_MODES_GET(unit, pm_info, interface_modes, port_index) \
    SOC_WB_ENGINE_GET_ARR(unit, SOC_WB_ENGINE_PORTMOD,                         \
                          pm_info->wb_vars_ids[interfaceModes],                \
                          interface_modes, port_index)
#define PM4x10_IS_INTERFACE_RESTORE_SET(unit, pm_info, is_interface_restore, \
                                        port_index)                          \
    SOC_WB_ENGINE_SET_ARR(unit, SOC_WB_ENGINE_PORTMOD,                       \
                          pm_info->wb_vars_ids[isInterfaceRestore],          \
                          &is_interface_restore, port_index)
#define PM4x10_IS_INTERFACE_RESTORE_GET(unit, pm_info, is_interface_restore, \
                                        port_index)                          \
    SOC_WB_ENGINE_GET_ARR(unit, SOC_WB_ENGINE_PORTMOD,                       \
                          pm_info->wb_vars_ids[isInterfaceRestore],          \
                          is_interface_restore, port_index)
#define PM4x10_IS_HG_SET(unit, pm_info, is_hg, port_index)                \
    SOC_WB_ENGINE_SET_ARR(unit, SOC_WB_ENGINE_PORTMOD,                    \
                          pm_info->wb_vars_ids[isHg], &is_hg, port_index)
#define PM4x10_IS_HG_GET(unit, pm_info, is_hg, port_index)                \
    SOC_WB_ENGINE_GET_ARR(unit, SOC_WB_ENGINE_PORTMOD,                    \
                          pm_info->wb_vars_ids[isHg], is_hg, port_index)
#define PM4x10_AN_MODE_SET(unit, pm_info, an_mode, port_index)               \
    SOC_WB_ENGINE_SET_ARR(unit, SOC_WB_ENGINE_PORTMOD,                       \
                          pm_info->wb_vars_ids[anMode], &an_mode, port_index)
#define PM4x10_AN_MODE_GET(unit, pm_info, an_mode, port_index)               \
    SOC_WB_ENGINE_GET_ARR(unit, SOC_WB_ENGINE_PORTMOD,                       \
                          pm_info->wb_vars_ids[anMode], an_mode, port_index)
#define PM4x10_FS_CL72_SET(unit, pm_info, fs_cl72, port_index)               \
    SOC_WB_ENGINE_SET_ARR(unit, SOC_WB_ENGINE_PORTMOD,                       \
                          pm_info->wb_vars_ids[fsCl72], &fs_cl72, port_index)
#define PM4x10_FS_CL72_GET(unit, pm_info, fs_cl72, port_index)               \
    SOC_WB_ENGINE_GET_ARR(unit, SOC_WB_ENGINE_PORTMOD,                       \
                          pm_info->wb_vars_ids[fsCl72], fs_cl72, port_index)
#define PM4x10_CX4_10G_SET(unit, pm_info, cx4_10G, port_index)              \
    SOC_WB_ENGINE_SET_ARR(unit, SOC_WB_ENGINE_PORTMOD,                      \
                       pm_info->wb_vars_ids[cx4In10G], &cx4_10G, port_index)
#define PM4x10_CX4_10G_GET(unit, pm_info, cx4_10G, port_index)              \
    SOC_WB_ENGINE_GET_ARR(unit, SOC_WB_ENGINE_PORTMOD,                      \
                       pm_info->wb_vars_ids[cx4In10G], cx4_10G, port_index)
#define PM4x10_AN_CL72_SET(unit, pm_info, an_cl72, port_index)              \
    SOC_WB_ENGINE_SET_ARR(unit, SOC_WB_ENGINE_PORTMOD,                      \
                          pm_info->wb_vars_ids[anCl72], &an_cl72, port_index)
#define PM4x10_AN_CL72_GET(unit, pm_info, an_cl72, port_index)              \
    SOC_WB_ENGINE_GET_ARR(unit, SOC_WB_ENGINE_PORTMOD,                      \
                          pm_info->wb_vars_ids[anCl72], an_cl72, port_index)
#define PM4x10_AN_FEC_SET(unit, pm_info, an_fec, port_index)                \
    SOC_WB_ENGINE_SET_ARR(unit, SOC_WB_ENGINE_PORTMOD,                      \
                          pm_info->wb_vars_ids[anFec], &an_fec, port_index)
#define PM4x10_AN_FEC_GET(unit, pm_info, an_fec, port_index)                \
    SOC_WB_ENGINE_GET_ARR(unit, SOC_WB_ENGINE_PORTMOD,                      \
                          pm_info->wb_vars_ids[anFec], an_fec, port_index)
#define PM4x10_AN_CL37_SET(unit, pm_info, an_cl37, port_index)              \
    SOC_WB_ENGINE_SET_ARR(unit, SOC_WB_ENGINE_PORTMOD,                      \
                          pm_info->wb_vars_ids[anCl37], &an_cl37, port_index)
#define PM4x10_AN_CL37_GET(unit, pm_info, an_cl37, port_index)              \
    SOC_WB_ENGINE_GET_ARR(unit, SOC_WB_ENGINE_PORTMOD,                      \
                          pm_info->wb_vars_ids[anCl37], an_cl37, port_index)
#define PM4x10_AN_MASTER_LANE_SET(unit, pm_info, an_master_lane, port_index) \
    SOC_WB_ENGINE_SET_ARR(unit, SOC_WB_ENGINE_PORTMOD,                       \
                          pm_info->wb_vars_ids[anMasterLane],                \
                          &an_master_lane, port_index)
#define PM4x10_AN_MASTER_LANE_GET(unit, pm_info, an_master_lane, port_index) \
    SOC_WB_ENGINE_GET_ARR(unit, SOC_WB_ENGINE_PORTMOD,                       \
                          pm_info->wb_vars_ids[anMasterLane],                \
                          an_master_lane, port_index)
#define PM4x10_CL37_SGMII_CNT_SET(unit, pm_info, cl37_sgmii_cnt, port_index)  \
    SOC_WB_ENGINE_SET_ARR(unit, SOC_WB_ENGINE_PORTMOD,                        \
                          pm_info->wb_vars_ids[cl37SgmiiCnt], &cl37_sgmii_cnt,\
                          port_index)
#define PM4x10_CL37_SGMII_CNT_GET(unit, pm_info, cl37_sgmii_cnt, port_index)  \
    SOC_WB_ENGINE_GET_ARR(unit, SOC_WB_ENGINE_PORTMOD,                        \
                          pm_info->wb_vars_ids[cl37SgmiiCnt], cl37_sgmii_cnt, \
                          port_index)
#define PM4x10_CL37_SGMII_RESTART_CNT_SET(unit, pm_info,                     \
                                          cl37_sgmii_restart_cnt, port_index)\
    SOC_WB_ENGINE_SET_ARR(unit, SOC_WB_ENGINE_PORTMOD,                       \
                          pm_info->wb_vars_ids[cl37SgmiiRestartCnt],         \
                          &cl37_sgmii_restart_cnt, port_index)
#define PM4x10_CL37_SGMII_RESTART_CNT_GET(unit, pm_info,                     \
                                          cl37_sgmii_restart_cnt, port_index)\
    SOC_WB_ENGINE_GET_ARR(unit, SOC_WB_ENGINE_PORTMOD,                       \
                          pm_info->wb_vars_ids[cl37SgmiiRestartCnt],         \
                          cl37_sgmii_restart_cnt, port_index)
#define PM4x10_PORT_OP_MODE_SET(unit, pm_info, port_op_mode, port_index)     \
    SOC_WB_ENGINE_SET_ARR(unit, SOC_WB_ENGINE_PORTMOD,                       \
                          pm_info->wb_vars_ids[portOpMode],                  \
                          &port_op_mode, port_index)
#define PM4x10_PORT_OP_MODE_GET(unit, pm_info, port_op_mode, port_index)     \
    SOC_WB_ENGINE_GET_ARR(unit, SOC_WB_ENGINE_PORTMOD,                       \
                          pm_info->wb_vars_ids[portOpMode],                  \
                          port_op_mode, port_index)
#define PM4x10_RXAUI_MODE_SET(unit, pm_info, rxaui_mode, port_index)    \
    SOC_WB_ENGINE_SET_ARR(unit, SOC_WB_ENGINE_PORTMOD,                  \
                          pm_info->wb_vars_ids[rxauiMode],              \
                          &rxaui_mode, port_index)
#define PM4x10_RXAUI_MODE_GET(unit, pm_info, rxaui_mode, port_index)    \
    SOC_WB_ENGINE_GET_ARR(unit, SOC_WB_ENGINE_PORTMOD,                  \
                          pm_info->wb_vars_ids[rxauiMode],              \
                          rxaui_mode, port_index)

#define PM4x10_TX_PARAMS_USER_FLAG_SET(unit, pm_info,                        \
                                       tx_params_user_flag, port_index)      \
    SOC_WB_ENGINE_SET_ARR(unit, SOC_WB_ENGINE_PORTMOD,                       \
                          pm_info->wb_vars_ids[txParamsUserFlag],            \
                          &tx_params_user_flag, port_index)
#define PM4x10_TX_PARAMS_USER_FLAG_GET(unit, pm_info,                        \
                                       tx_params_user_flag, port_index)      \
    SOC_WB_ENGINE_GET_ARR(unit, SOC_WB_ENGINE_PORTMOD,                       \
                          pm_info->wb_vars_ids[txParamsUserFlag],            \
                          tx_params_user_flag, port_index)

#define PM4x10_TX_PARAMS_SET(unit, pm_info,                  \
                             tx_params, index)               \
    SOC_WB_ENGINE_SET_ARR(unit, SOC_WB_ENGINE_PORTMOD,       \
                             pm_info->wb_vars_ids[txParams], \
                             &tx_params, index)
#define PM4x10_TX_PARAMS_GET(unit, pm_info,                  \
                             tx_params, index)               \
    SOC_WB_ENGINE_GET_ARR(unit, SOC_WB_ENGINE_PORTMOD,       \
                          pm_info->wb_vars_ids[txParams],    \
                          tx_params, index)

#define PM4x10_EXT_PHY_TX_PARAMS_USER_FLAG_SET(unit, pm_info,                \
                                   ext_phy_tx_params_user_flag, port_index)  \
    SOC_WB_ENGINE_SET_ARR(unit, SOC_WB_ENGINE_PORTMOD,                       \
                          pm_info->wb_vars_ids[extPhyTxParamsUserFlag],      \
                          &ext_phy_tx_params_user_flag, port_index)
#define PM4x10_EXT_PHY_TX_PARAMS_USER_FLAG_GET(unit, pm_info,                \
                                   ext_phy_tx_params_user_flag, port_index)  \
    SOC_WB_ENGINE_GET_ARR(unit, SOC_WB_ENGINE_PORTMOD,                       \
                          pm_info->wb_vars_ids[extPhyTxParamsUserFlag],      \
                          ext_phy_tx_params_user_flag, port_index)

#define PM4x10_EXT_PHY_TX_PARAMS_SET(unit, pm_info,                    \
                                     ext_phy_tx_params, index)         \
    SOC_WB_ENGINE_SET_ARR(unit, SOC_WB_ENGINE_PORTMOD,                 \
                             pm_info->wb_vars_ids[extPhyTxParams],     \
                             &ext_phy_tx_params, index)
#define PM4x10_EXT_PHY_TX_PARAMS_GET(unit, pm_info,                    \
                                     ext_phy_tx_params, index)         \
    SOC_WB_ENGINE_GET_ARR(unit, SOC_WB_ENGINE_PORTMOD,                 \
                          pm_info->wb_vars_ids[extPhyTxParams],        \
                          ext_phy_tx_params, index)

#define PM4x10_PORT_FALLBACK_LANE_SET(unit, pm_info, fallback_lane, port_index)\
    SOC_WB_ENGINE_SET_ARR(unit, SOC_WB_ENGINE_PORTMOD,                         \
                          pm_info->wb_vars_ids[fallbackLane],                  \
                          &fallback_lane, port_index)
#define PM4x10_PORT_FALLBACK_LANE_GET(unit, pm_info, fallback_lane, port_index)\
    SOC_WB_ENGINE_GET_ARR(unit, SOC_WB_ENGINE_PORTMOD,                         \
                          pm_info->wb_vars_ids[fallbackLane],                  \
                          fallback_lane, port_index)

#define PM4x10_PORT_LANE_CONNECTION_DB_SET(unit, pm_info, lane_connection_db, xphy_idx, lane) \
    SOC_WB_ENGINE_SET_DBL_ARR(unit, SOC_WB_ENGINE_PORTMOD, pm_info->wb_vars_ids[laneConnectionDb], \
        (uint8 *)lane_connection_db, lane, xphy_idx)
#define PM4x10_PORT_LANE_CONNECTION_DB_GET(unit, pm_info, lane_connection_db, xphy_idx, lane) \
    SOC_WB_ENGINE_GET_DBL_ARR(unit, SOC_WB_ENGINE_PORTMOD, pm_info->wb_vars_ids[laneConnectionDb], \
        (uint8 *)lane_connection_db, lane, xphy_idx)

#define PM4x10_PORT_NUM_PHYS_SET(unit, pm_info, nof_phys, lane)   \
        SOC_WB_ENGINE_SET_ARR(unit, SOC_WB_ENGINE_PORTMOD, pm_info->wb_vars_ids[nofPhys], nof_phys, lane)
#define PM4x10_PORT_NUM_PHYS_GET(unit, pm_info, nof_phys, lane)   \
        SOC_WB_ENGINE_GET_ARR(unit, SOC_WB_ENGINE_PORTMOD, pm_info->wb_vars_ids[nofPhys], nof_phys, lane)

#define PM4x10_PORT_PLL_DIVIDER_REQ_SET(unit, pm_info,                \
                                        pll_divider_req,              \
                                        port_index)                   \
    SOC_WB_ENGINE_SET_ARR(unit, SOC_WB_ENGINE_PORTMOD,                \
                          pm_info->wb_vars_ids[pllDividerReq],        \
                          &pll_divider_req, port_index)
#define PM4x10_PORT_PLL_DIVIDER_REQ_GET(unit, pm_info,                \
                                        pll_divider_req,              \
                                        port_index)                   \
    SOC_WB_ENGINE_GET_ARR(unit, SOC_WB_ENGINE_PORTMOD,                \
                          pm_info->wb_vars_ids[pllDividerReq],        \
                          pll_divider_req, port_index)


/* Warm Boot Variables to indicate the boot state */
typedef enum pm4x10_fabric_wb_vars{
    isInitialized,
    ports,
    isActive,
    isBypassed,
    threePortsMode,
    phy_type,
    interfaceConfig,
    portInitConfig,
    phyInitConfig,
    phyInterfaceConfig,
    portDynamicState,
    nofPhys, /* number of phys for the port */
    laneConnectionDb, /* represents the physical topology of the  serdes and ext phy lanes */
    dataRate,/* port speed - phy_intf_config.data_rate */
    maxSpeed, /* Max Port Speed Supported */
    /* interface type -  portmod_port_interface_config_t.interface */
    interfaceType, /* interface of type soc_port_if_t */
    /* serdes interface -  portmod_port_interface_config_t.serdes_interface */
    serdesInterfaceType,
    interfaceModes, /* see PHYMOD_INTF_MODES_ */
    isInterfaceRestore, /*indicate whether interface should be restored from DB (otherwise retrieved from HW), replace PHYMOD_INTF_F_INTF_PARAM_SET_ONLY*/
    isHg, /* specify wheather the port is higig port */
    anMode, /* spn_PHY_AN_MODE - Specify the AN mode */
    fsCl72, /* spn_FORCED_INIT_CL72 - if trainig should be enabled in Forced speed mode */
    cx4In10G, /* see PHYMOD_BAM_CL37_CAP_10G_CX4 */
    anCl72, /* spn_PHY_AN_C72 - Specify if the link training should be enabled */
    anFec, /* spn_FORCED_INIT_FEC specify if FEC should be enabled in AN mode */
    anCl37, /**< AN CL37 */
    anMasterLane, /* spn_PHY_AUTONEG_MASTER_LANE - Specify the AN master lane */
    cl37SgmiiCnt, /* store cl37 sgmii count */
    cl37SgmiiRestartCnt, /* spn_SERDES_CL37_SGMII_RESTART_COUNT - cl37_sgmii_cnt threshold*/
    portOpMode, /* spn_PHY_PCS_REPEATER - phy operation Mode */
    rxauiMode,  /* portmod_port_init_config_t.rxaui_mode */
    /* tx params flag - phy_init_config_t.tx_params_user_flag */
    txParamsUserFlag,
    /* tx params flag - phy_init_config_t.tx */
    txParams,
    /* ext tx params flag - phy_init_config_t.ext_phy_tx_params_user_flag */
    extPhyTxParamsUserFlag,
    /* ext tx params - phy_init_config_t.ext_phy_tx */
    extPhyTxParams,
    /* port fallback lane - portmod_port_init_config_t.port_fallback_lane */
    fallbackLane,
    /* pll_divider_req setting on the port */
    pllDividerReq,
    /* the interface type written into the outmost PHY */
    interfaceWrittenIntoOutmostPhy
}pm4x10_wb_vars_t;

struct pm4x10_s{
    portmod_pbmp_t phys;
    int first_phy;
    phymod_ref_clk_t ref_clk;
    phymod_polarity_t polarity;
    phymod_lane_map_t lane_map;
    phymod_firmware_load_method_t fw_load_method;
    phymod_firmware_loader_f external_fw_loader;
    phymod_core_access_t int_core_access ; /* for internal SerDes only */
    int nof_phys[PM4X10_LANES_PER_CORE]; /* internal + External Phys for each lane*/
    uint8 in_pm12x10;
    uint8 default_fw_loader_is_used;
    uint8 default_bus_is_used;
    portmod_phy_external_reset_f  portmod_phy_external_reset;
    portmod_mac_soft_reset_f portmod_mac_soft_reset;
    int core_num;
    int core_num_int;  /* 0, 1, 2 for TSC12 */
    portmod_xphy_lane_connection_t lane_conn[MAX_PHYN][PM4X10_LANES_PER_CORE];
    int warmboot_skip_db_restore;
    int rescal;
    int is_pm4x10q;
};

#define PM4x10_QUAD_MODE_IF(interface) \
    (interface == SOC_PORT_IF_CAUI  || \
     interface == SOC_PORT_IF_XLAUI || \
     interface == SOC_PORT_IF_CR4   || \
     interface == SOC_PORT_IF_SR4   || \
     interface == SOC_PORT_IF_KR4   || \
     interface == SOC_PORT_IF_XGMII || \
     interface == SOC_PORT_IF_DNX_XAUI)

#define PM4x10_DUAL_MODE_IF(interface) \
    (interface == SOC_PORT_IF_RXAUI || \
     interface == SOC_PORT_IF_SR2   || \
     interface == SOC_PORT_IF_KR2   || \
     interface == SOC_PORT_IF_CR2)

STATIC
int pm4x10_port_soft_reset(int unit, int port, pm_info_t pm_info, int enable);

STATIC
int _pm4x10_pm_version_get(int unit, int port, pm_info_t pm_info, pm_version_t *version)
{
    uint32 reg_val;
    int phy_acc;
    SOC_INIT_FUNC_DEFS;

    if(version == NULL)
        return SOC_E_PARAM;

    if (!SOC_REG_IS_VALID(unit, XLPORT_PM_VERSION_IDr)) {
        version->type           = PM_TYPE_COUNT;
        version->tech_process   = PM_TECH_COUNT;
        version->version_number = PM_VERSION_NUMBER_UNKNOWN;
        version->revision_letter = PM_REVISION_COUNT;
    }
    else {
        PM4x10_PHY_ACCESS_GET(unit, port, pm_info, phy_acc);
        _SOC_IF_ERR_EXIT(READ_XLPORT_PM_VERSION_IDr(unit, phy_acc, &reg_val));

        version->type           = soc_reg_field_get(unit, XLPORT_PM_VERSION_IDr, reg_val, PM_TYPEf);
        version->tech_process   = soc_reg_field_get(unit, XLPORT_PM_VERSION_IDr, reg_val, TECH_PROCESSf);
        version->version_number = soc_reg_field_get(unit, XLPORT_PM_VERSION_IDr, reg_val, REV_NUMBERf);
        version->revision_letter = soc_reg_field_get(unit, XLPORT_PM_VERSION_IDr, reg_val, REV_LETTERf);
    }

exit:
    SOC_FUNC_RETURN;
}

int pm4x10_pm_interface_type_is_supported(int unit, soc_port_if_t interface, 
                                          int* is_supported)
{
    SOC_INIT_FUNC_DEFS;
    switch(interface){
        case SOC_PORT_IF_SGMII:
        case SOC_PORT_IF_GMII:
        case SOC_PORT_IF_KX:
        case SOC_PORT_IF_KR:
        case SOC_PORT_IF_DNX_XAUI:
        case SOC_PORT_IF_XLAUI:
        case SOC_PORT_IF_RXAUI:
        case SOC_PORT_IF_KR2:
        case SOC_PORT_IF_CR2:
        case SOC_PORT_IF_XFI:
        case SOC_PORT_IF_SFI:
        case SOC_PORT_IF_SR4:
        case SOC_PORT_IF_CR4:
        case SOC_PORT_IF_KR4:
        case SOC_PORT_IF_ER4:
        case SOC_PORT_IF_XGMII:
            *is_supported = TRUE;
            break;

        /* these modes pm4x10 should be bypass */
        case SOC_PORT_IF_QSGMII:
        case SOC_PORT_IF_CAUI:
            *is_supported = TRUE;
            break;
        default:
            *is_supported = FALSE;
            break;
    }
    SOC_FUNC_RETURN;
}

STATIC
int _pm4x10_port_index_get(int unit, int port, pm_info_t pm_info,
                           int *first_index, uint32 *bitmap)
{
   int i, rv = 0, tmp_port = 0;

   SOC_INIT_FUNC_DEFS;

   *first_index = -1;
   *bitmap = 0;

   for( i = 0 ; i < MAX_PORTS_PER_PM4X10; i++) {
       rv = SOC_WB_ENGINE_GET_ARR (unit, SOC_WB_ENGINE_PORTMOD, 
                     pm_info->wb_vars_ids[ports], &tmp_port, i);
       _SOC_IF_ERR_EXIT(rv);

       if(tmp_port == port){
           *first_index = (*first_index == -1 ? i : *first_index);
           SHR_BITSET(bitmap, i);
       }
   }

   if(*first_index == -1) {
       _SOC_EXIT_WITH_ERR(SOC_E_INTERNAL,
              (_SOC_MSG("port was not found in internal DB %d"), port));
   }

exit:
    SOC_FUNC_RETURN;
}

/*
 * Function:
 *      portmod_pm4x10_wb_upgrade_func
 * Purpose:
 *      This function will take care of the warmboot variable manipulation
 *      in case of upgrade case, when variable definition got change or
 *      unspoorted varaible from previous version.
 * Parameters:
 *      unit                -(IN) Device unit number .
 *      arg                 -(IN) Generic pointer for a specific module to be used.
 *      recovered_version   -(IN) Warmboot version of the existing data.
 *      new_version         -(IN) Warmboot version of new data.
 * Returns:
 *      Status
 */

int portmod_pm4x10_wb_upgrade_func(int unit, void *arg, int recovered_version, int new_version)
{
    pm_info_t pm_info;
    int i;
    int nof_phys;
    soc_port_if_t interface;
    SOC_INIT_FUNC_DEFS;

    pm_info = arg;

    /*
     * skip the warmboot db restore, it is  only needed for
     * certain upgrade which will be updated below.
     */
    PM_4x10_INFO(pm_info)->warmboot_skip_db_restore  = TRUE;

    if ((recovered_version <= 4) &&
        (new_version >= 5)) {

        /*
         * Need to do db restore.
         */
        PM_4x10_INFO(pm_info)->warmboot_skip_db_restore  = FALSE;
    }

    if ((recovered_version <= 3) &&
        (new_version >= 4)) {

        /*
         * in version 3 or earlier, nof_phys was not populated properly.
         * It need to initialize to 1  to count for internal phy. 1 may not
         * be proper value as some port may have ext phy.
         */
        nof_phys = 1;
        for(i = 0 ; i < PM4X10_LANES_PER_CORE ; i++){
            _SOC_IF_ERR_EXIT(PM4x10_PORT_NUM_PHYS_SET(unit, pm_info, &nof_phys, i));
            PM_4x10_INFO(pm_info)->nof_phys[i] = nof_phys;
        }
    }

    if ((recovered_version <= 6) &&
        (new_version >= 7)) {
        /* interface_written_into_outmost_phy does not exist in version 6 or earlier */
        interface = SOC_PORT_IF_COUNT;
        for (i = 0 ; i < MAX_PORTS_PER_PM4X10 ; i++) {
            _SOC_IF_ERR_EXIT(
                PM4x10_OUTMOST_PHY_INTERFACE_TYPE_SET(unit, pm_info, interface, i));
        }
    }

exit:
    SOC_FUNC_RETURN;
}
/*
 * Initialize the buffer to support warmboot
 * The state of warmboot is store in the variables like
 * isInitialized, isActive, isBypassed, ports.. etc.,
 * All of these variables need to be added to warmboot
 * any variables added to save the state of warmboot should be
 * included here.
 */
STATIC
int pm4x10_wb_buffer_init(int unit, int wb_buffer_index, pm_info_t pm_info)
{
    /* Declare the common variables needed for warmboot */
    WB_ENGINE_INIT_TABLES_DEFS;
    int wb_var_id, rv;
    int buffer_id = wb_buffer_index; /*required by SOC_WB_ENGINE_ADD_ Macros*/
    SOC_INIT_FUNC_DEFS;

    COMPILER_REFERENCE(buffer_is_dynamic);

    SOC_WB_ENGINE_ADD_BUFF(SOC_WB_ENGINE_PORTMOD, wb_buffer_index, "pm4x10",
                           portmod_pm4x10_wb_upgrade_func, pm_info,
                           PM4x10_WB_BUFFER_VERSION, 1, SOC_WB_ENGINE_PRE_RELEASE);
    _SOC_IF_ERR_EXIT(rv);

    _SOC_IF_ERR_EXIT(portmod_next_wb_var_id_get(unit, &wb_var_id));
    SOC_WB_ENGINE_ADD_VAR(SOC_WB_ENGINE_PORTMOD, wb_var_id, "is_initialized",
                          wb_buffer_index, sizeof(uint32), NULL, VERSION(1));
    _SOC_IF_ERR_EXIT(rv);
    pm_info->wb_vars_ids[isInitialized] = wb_var_id;

    _SOC_IF_ERR_EXIT(portmod_next_wb_var_id_get(unit, &wb_var_id));
    SOC_WB_ENGINE_ADD_ARR(SOC_WB_ENGINE_PORTMOD, wb_var_id, "ports",
                          wb_buffer_index, sizeof(int), NULL,
                          MAX_PORTS_PER_PM4X10, VERSION(1));
    _SOC_IF_ERR_EXIT(rv);
    pm_info->wb_vars_ids[ports] = wb_var_id;

    _SOC_IF_ERR_EXIT(portmod_next_wb_var_id_get(unit, &wb_var_id));
    SOC_WB_ENGINE_ADD_VAR(SOC_WB_ENGINE_PORTMOD, wb_var_id, "is_active",
                          wb_buffer_index, sizeof(uint32), NULL, VERSION(1));
    _SOC_IF_ERR_EXIT(rv);
    pm_info->wb_vars_ids[isActive] = wb_var_id;

    _SOC_IF_ERR_EXIT(portmod_next_wb_var_id_get(unit, &wb_var_id));
    SOC_WB_ENGINE_ADD_VAR(SOC_WB_ENGINE_PORTMOD, wb_var_id, "is_bypassed",
                          wb_buffer_index, sizeof(uint32), NULL, VERSION(1));
    _SOC_IF_ERR_EXIT(rv);
    pm_info->wb_vars_ids[isBypassed] = wb_var_id;

    _SOC_IF_ERR_EXIT(portmod_next_wb_var_id_get(unit, &wb_var_id));
    SOC_WB_ENGINE_ADD_VAR(SOC_WB_ENGINE_PORTMOD, wb_var_id, "three_ports_mode",
                          wb_buffer_index, sizeof(uint32), NULL, VERSION(1));
    _SOC_IF_ERR_EXIT(rv);
    pm_info->wb_vars_ids[threePortsMode] = wb_var_id;

    _SOC_IF_ERR_EXIT(portmod_next_wb_var_id_get(unit, &wb_var_id));
    SOC_WB_ENGINE_ADD_ARR(SOC_WB_ENGINE_PORTMOD, wb_var_id, "phy_type",
                          wb_buffer_index, sizeof(int), NULL,
                          MAX_PORTS_PER_PM4X10, VERSION(1));
    _SOC_IF_ERR_EXIT(rv);
    pm_info->wb_vars_ids[phy_type] = wb_var_id;

    /* Deleting the below WB structures since most likely to be added in the future in a diffrent format */
    _SOC_IF_ERR_EXIT(portmod_next_wb_var_id_get(unit, &wb_var_id));
    SOC_WB_ENGINE_ADD_VAR_WITH_FEATURES(SOC_WB_ENGINE_PORTMOD, wb_var_id,
                                        "interface_config", wb_buffer_index,
                                        sizeof(portmod_port_interface_config_t),
                                        NULL, 1, 1, 0xffffffff, 0xffffffff,
                                        VERSION(2), VERSION(3), NULL);
    _SOC_IF_ERR_EXIT(rv);
    pm_info->wb_vars_ids[interfaceConfig] = wb_var_id;

    _SOC_IF_ERR_EXIT(portmod_next_wb_var_id_get(unit, &wb_var_id));
    SOC_WB_ENGINE_ADD_VAR_WITH_FEATURES(SOC_WB_ENGINE_PORTMOD, wb_var_id,
                                        "port_init_config", wb_buffer_index,
                                        sizeof(portmod_port_init_config_t),
                                        NULL, 1, 1, 0xffffffff, 0xffffffff,
                                        VERSION(2), VERSION(3), NULL);
    _SOC_IF_ERR_EXIT(rv);
    pm_info->wb_vars_ids[portInitConfig] = wb_var_id;

    _SOC_IF_ERR_EXIT(portmod_next_wb_var_id_get(unit, &wb_var_id));
    SOC_WB_ENGINE_ADD_VAR_WITH_FEATURES(SOC_WB_ENGINE_PORTMOD, wb_var_id,
                                        "phy_init_config", wb_buffer_index,
                                        sizeof(phymod_phy_init_config_t),
                                        NULL, 1, 1, 0xffffffff, 0xffffffff,
                                        VERSION(2), VERSION(3), NULL);
    _SOC_IF_ERR_EXIT(rv);
    pm_info->wb_vars_ids[phyInitConfig] = wb_var_id;

    _SOC_IF_ERR_EXIT(portmod_next_wb_var_id_get(unit, &wb_var_id));
    SOC_WB_ENGINE_ADD_VAR_WITH_FEATURES(SOC_WB_ENGINE_PORTMOD, wb_var_id,
                                       "phy_interface_config", wb_buffer_index,
                                       sizeof(phymod_phy_inf_config_t), NULL,
                                       1, 1, 0xffffffff, 0xffffffff, VERSION(2),
                                       VERSION(3), NULL);
    _SOC_IF_ERR_EXIT(rv);
    pm_info->wb_vars_ids[phyInterfaceConfig] = wb_var_id;

    /*
     * Keeping the below 2 WB var/arr since most likely to be used in the future.
     */
    _SOC_IF_ERR_EXIT(portmod_next_wb_var_id_get(unit, &wb_var_id));
    SOC_WB_ENGINE_ADD_ARR(SOC_WB_ENGINE_PORTMOD, wb_var_id,
                          "port_dynamic_state", wb_buffer_index,
                          sizeof(uint32_t), NULL, PM4X10_LANES_PER_CORE,
                          VERSION(2));
    _SOC_IF_ERR_EXIT(rv);
    pm_info->wb_vars_ids[portDynamicState] = wb_var_id;

    _SOC_IF_ERR_EXIT(portmod_next_wb_var_id_get(unit, &wb_var_id));
    SOC_WB_ENGINE_ADD_ARR(SOC_WB_ENGINE_PORTMOD, wb_var_id, "nof_phys",
                          wb_buffer_index, sizeof(int), NULL,
                          PM4X10_LANES_PER_CORE, VERSION(2));
    _SOC_IF_ERR_EXIT(rv);
    pm_info->wb_vars_ids[nofPhys] = wb_var_id;

    _SOC_IF_ERR_EXIT(portmod_next_wb_var_id_get(unit, &wb_var_id));
    SOC_WB_ENGINE_ADD_2D_ARR(SOC_WB_ENGINE_PORTMOD, wb_var_id, "lane_connection_db",
                          wb_buffer_index, sizeof(portmod_xphy_lane_connection_t), NULL,
                          PM4X10_LANES_PER_CORE, MAX_PHYN, VERSION(4));
    _SOC_IF_ERR_EXIT(rv);
    pm_info->wb_vars_ids[laneConnectionDb] = wb_var_id;

    _SOC_IF_ERR_EXIT(portmod_next_wb_var_id_get(unit, &wb_var_id));
    SOC_WB_ENGINE_ADD_ARR(SOC_WB_ENGINE_PORTMOD, wb_var_id, "data_rate",
                          wb_buffer_index, sizeof(int), NULL,
                          MAX_PORTS_PER_PM4X10, VERSION(5));
    _SOC_IF_ERR_EXIT(rv);
    pm_info->wb_vars_ids[dataRate] = wb_var_id;
    _SOC_IF_ERR_EXIT(portmod_next_wb_var_id_get(unit, &wb_var_id));
    SOC_WB_ENGINE_ADD_ARR(SOC_WB_ENGINE_PORTMOD, wb_var_id, "max_speed",
                          wb_buffer_index, sizeof(uint32), NULL,
                          MAX_PORTS_PER_PM4X10, VERSION(5));
    _SOC_IF_ERR_EXIT(rv);
    pm_info->wb_vars_ids[maxSpeed] = wb_var_id;
    _SOC_IF_ERR_EXIT(portmod_next_wb_var_id_get(unit, &wb_var_id));
    SOC_WB_ENGINE_ADD_ARR(SOC_WB_ENGINE_PORTMOD, wb_var_id, "interface_type",
                          wb_buffer_index, sizeof(uint32), NULL,
                          MAX_PORTS_PER_PM4X10, VERSION(5));
    _SOC_IF_ERR_EXIT(rv);
    pm_info->wb_vars_ids[interfaceType] = wb_var_id;
    _SOC_IF_ERR_EXIT(portmod_next_wb_var_id_get(unit, &wb_var_id));
    SOC_WB_ENGINE_ADD_ARR(SOC_WB_ENGINE_PORTMOD, wb_var_id, "serdes_interface",
                          wb_buffer_index, sizeof(uint32), NULL,
                          MAX_PORTS_PER_PM4X10, VERSION(5));
    _SOC_IF_ERR_EXIT(rv);
    pm_info->wb_vars_ids[serdesInterfaceType] = wb_var_id;
    _SOC_IF_ERR_EXIT(portmod_next_wb_var_id_get(unit, &wb_var_id));
    SOC_WB_ENGINE_ADD_ARR(SOC_WB_ENGINE_PORTMOD, wb_var_id, "interface_modes",
                          wb_buffer_index, sizeof(uint32), NULL,
                          MAX_PORTS_PER_PM4X10, VERSION(5));
    _SOC_IF_ERR_EXIT(rv);
    pm_info->wb_vars_ids[interfaceModes] = wb_var_id;
    _SOC_IF_ERR_EXIT(portmod_next_wb_var_id_get(unit, &wb_var_id));
    SOC_WB_ENGINE_ADD_ARR(SOC_WB_ENGINE_PORTMOD, wb_var_id,
                          "is_interface_restore", wb_buffer_index,
                          sizeof(uint32), NULL, PM4X10_LANES_PER_CORE,
                          VERSION(5));
    _SOC_IF_ERR_EXIT(rv);
    pm_info->wb_vars_ids[isInterfaceRestore] = wb_var_id;
    _SOC_IF_ERR_EXIT(portmod_next_wb_var_id_get(unit, &wb_var_id));
    SOC_WB_ENGINE_ADD_ARR(SOC_WB_ENGINE_PORTMOD, wb_var_id, "is_hg",
                          wb_buffer_index, sizeof(uint32), NULL,
                          PM4X10_LANES_PER_CORE, VERSION(5));
    _SOC_IF_ERR_EXIT(rv);
    pm_info->wb_vars_ids[isHg] = wb_var_id;
    _SOC_IF_ERR_EXIT(portmod_next_wb_var_id_get(unit, &wb_var_id));
    SOC_WB_ENGINE_ADD_ARR(SOC_WB_ENGINE_PORTMOD, wb_var_id, "an_cl73",
                          wb_buffer_index, sizeof(uint32), NULL,
                          PM4X10_LANES_PER_CORE, VERSION(5));
    _SOC_IF_ERR_EXIT(rv);
    pm_info->wb_vars_ids[anMode] = wb_var_id;
    _SOC_IF_ERR_EXIT(portmod_next_wb_var_id_get(unit, &wb_var_id));
    SOC_WB_ENGINE_ADD_ARR(SOC_WB_ENGINE_PORTMOD, wb_var_id, "fs_cl72",
                          wb_buffer_index, sizeof(uint32), NULL,
                          PM4X10_LANES_PER_CORE, VERSION(5));
    _SOC_IF_ERR_EXIT(rv);
    pm_info->wb_vars_ids[fsCl72] = wb_var_id;
    _SOC_IF_ERR_EXIT(portmod_next_wb_var_id_get(unit, &wb_var_id));
    SOC_WB_ENGINE_ADD_ARR(SOC_WB_ENGINE_PORTMOD, wb_var_id, "cx4_10G",
                          wb_buffer_index, sizeof(uint32), NULL,
                          PM4X10_LANES_PER_CORE, VERSION(5));
    _SOC_IF_ERR_EXIT(rv);
    pm_info->wb_vars_ids[cx4In10G] = wb_var_id;
    _SOC_IF_ERR_EXIT(portmod_next_wb_var_id_get(unit, &wb_var_id));
    SOC_WB_ENGINE_ADD_ARR(SOC_WB_ENGINE_PORTMOD, wb_var_id, "an_Cl72",
                          wb_buffer_index, sizeof(uint32), NULL,
                          PM4X10_LANES_PER_CORE, VERSION(5));
    _SOC_IF_ERR_EXIT(rv);
    pm_info->wb_vars_ids[anCl72] = wb_var_id;
    _SOC_IF_ERR_EXIT(portmod_next_wb_var_id_get(unit, &wb_var_id));
    SOC_WB_ENGINE_ADD_ARR(SOC_WB_ENGINE_PORTMOD, wb_var_id, "an_fec",
                          wb_buffer_index, sizeof(uint32), NULL,
                          PM4X10_LANES_PER_CORE,  VERSION(5));
    _SOC_IF_ERR_EXIT(rv);
    pm_info->wb_vars_ids[anFec] = wb_var_id;
    _SOC_IF_ERR_EXIT(portmod_next_wb_var_id_get(unit, &wb_var_id));
    SOC_WB_ENGINE_ADD_ARR(SOC_WB_ENGINE_PORTMOD, wb_var_id, "an_cl37",
                          wb_buffer_index, sizeof(uint32), NULL,
                          PM4X10_LANES_PER_CORE, VERSION(5));
    _SOC_IF_ERR_EXIT(rv);
    pm_info->wb_vars_ids[anCl37] = wb_var_id;
    _SOC_IF_ERR_EXIT(portmod_next_wb_var_id_get(unit, &wb_var_id));
    SOC_WB_ENGINE_ADD_ARR(SOC_WB_ENGINE_PORTMOD, wb_var_id, "an_master_lane",
                          wb_buffer_index, sizeof(int), NULL,
                          PM4X10_LANES_PER_CORE, VERSION(5));
    _SOC_IF_ERR_EXIT(rv);
    pm_info->wb_vars_ids[anMasterLane] = wb_var_id;
    _SOC_IF_ERR_EXIT(portmod_next_wb_var_id_get(unit, &wb_var_id));
    SOC_WB_ENGINE_ADD_ARR(SOC_WB_ENGINE_PORTMOD, wb_var_id, "cl37_sgmii_count",
                          wb_buffer_index, sizeof(int), NULL,
                          PM4X10_LANES_PER_CORE, VERSION(5));
    _SOC_IF_ERR_EXIT(rv);
    pm_info->wb_vars_ids[cl37SgmiiCnt] = wb_var_id;
    _SOC_IF_ERR_EXIT(portmod_next_wb_var_id_get(unit, &wb_var_id));
    SOC_WB_ENGINE_ADD_ARR(SOC_WB_ENGINE_PORTMOD, wb_var_id,
                          "cl37_sgmii_restart_count", wb_buffer_index,
                          sizeof(int), NULL, PM4X10_LANES_PER_CORE, VERSION(5));
    _SOC_IF_ERR_EXIT(rv);
    pm_info->wb_vars_ids[cl37SgmiiRestartCnt] = wb_var_id;
    _SOC_IF_ERR_EXIT(portmod_next_wb_var_id_get(unit, &wb_var_id));
    SOC_WB_ENGINE_ADD_ARR(SOC_WB_ENGINE_PORTMOD, wb_var_id, "port_op_mode",
                          wb_buffer_index, sizeof(int), NULL,
                          PM4X10_LANES_PER_CORE, VERSION(5));
    _SOC_IF_ERR_EXIT(rv);
    pm_info->wb_vars_ids[portOpMode] = wb_var_id;
    _SOC_IF_ERR_EXIT(portmod_next_wb_var_id_get(unit, &wb_var_id));
    SOC_WB_ENGINE_ADD_ARR(SOC_WB_ENGINE_PORTMOD, wb_var_id, "rxaui_mode",
                          wb_buffer_index, sizeof(int), NULL,
                          PM4X10_LANES_PER_CORE, VERSION(5));
    _SOC_IF_ERR_EXIT(rv);
    pm_info->wb_vars_ids[rxauiMode] = wb_var_id;
    _SOC_IF_ERR_EXIT(portmod_next_wb_var_id_get(unit, &wb_var_id));
    SOC_WB_ENGINE_ADD_ARR(SOC_WB_ENGINE_PORTMOD, wb_var_id,
                          "tx_params_user_flag",
                          wb_buffer_index, sizeof(uint32), NULL,
                          PHYMOD_MAX_LANES_PER_PORT, VERSION(5));
    _SOC_IF_ERR_EXIT(rv);
    pm_info->wb_vars_ids[txParamsUserFlag] = wb_var_id;

    _SOC_IF_ERR_EXIT(portmod_next_wb_var_id_get(unit, &wb_var_id));
    SOC_WB_ENGINE_ADD_ARR(SOC_WB_ENGINE_PORTMOD, wb_var_id,
                             "tx_params", wb_buffer_index,
                             sizeof(phymod_tx_t), NULL,
                             PHYMOD_MAX_LANES_PER_PORT,
                             VERSION(5));
    _SOC_IF_ERR_EXIT(rv);
    pm_info->wb_vars_ids[txParams] = wb_var_id;

    _SOC_IF_ERR_EXIT(portmod_next_wb_var_id_get(unit, &wb_var_id));
    SOC_WB_ENGINE_ADD_ARR(SOC_WB_ENGINE_PORTMOD, wb_var_id,
                          "ext_phy_tx_params_user_flag",
                          wb_buffer_index, sizeof(uint32), NULL,
                          PHYMOD_MAX_LANES_PER_PORT, VERSION(5));
    _SOC_IF_ERR_EXIT(rv);
    pm_info->wb_vars_ids[extPhyTxParamsUserFlag] = wb_var_id;

    _SOC_IF_ERR_EXIT(portmod_next_wb_var_id_get(unit, &wb_var_id));
    SOC_WB_ENGINE_ADD_ARR(SOC_WB_ENGINE_PORTMOD, wb_var_id,
                             "ext_phy_tx_params", wb_buffer_index,
                             sizeof(phymod_tx_t), NULL,
                             PHYMOD_MAX_LANES_PER_PORT,
                             VERSION(5));
    _SOC_IF_ERR_EXIT(rv);
    pm_info->wb_vars_ids[extPhyTxParams] = wb_var_id;

    _SOC_IF_ERR_EXIT(portmod_next_wb_var_id_get(unit, &wb_var_id));
    SOC_WB_ENGINE_ADD_ARR(SOC_WB_ENGINE_PORTMOD, wb_var_id, "fallback_lane",
                          wb_buffer_index, sizeof(int), NULL,
                          PM4X10_LANES_PER_CORE, VERSION(5));
    _SOC_IF_ERR_EXIT(rv);
    pm_info->wb_vars_ids[fallbackLane] = wb_var_id;

    _SOC_IF_ERR_EXIT(portmod_next_wb_var_id_get(unit, &wb_var_id));
    SOC_WB_ENGINE_ADD_ARR(SOC_WB_ENGINE_PORTMOD, wb_var_id,
                          "pll_divider_req",
                          wb_buffer_index, sizeof(int), NULL,
                          PM4X10_LANES_PER_CORE,
                          VERSION(6));
    _SOC_IF_ERR_EXIT(rv);
    pm_info->wb_vars_ids[pllDividerReq] = wb_var_id;

    _SOC_IF_ERR_EXIT(portmod_next_wb_var_id_get(unit, &wb_var_id));
    SOC_WB_ENGINE_ADD_ARR(SOC_WB_ENGINE_PORTMOD, wb_var_id,
                          "interface_written_into_outmost_phy",
                          wb_buffer_index, sizeof(uint32), NULL,
                          MAX_PORTS_PER_PM4X10, VERSION(7));
    _SOC_IF_ERR_EXIT(rv);
    pm_info->wb_vars_ids[interfaceWrittenIntoOutmostPhy] = wb_var_id;

    _SOC_IF_ERR_EXIT(soc_wb_engine_init_buffer(unit, SOC_WB_ENGINE_PORTMOD,
                     wb_buffer_index, FALSE));

    if(!SOC_WARM_BOOT(unit)){
        rv = SOC_WB_ENGINE_MEMSET_ARR(unit, SOC_WB_ENGINE_PORTMOD,
                                      pm_info->wb_vars_ids[ports], -1);
        _SOC_IF_ERR_EXIT(rv);
        rv = SOC_WB_ENGINE_MEMSET_ARR(unit, SOC_WB_ENGINE_PORTMOD,
                                      pm_info->wb_vars_ids[dataRate], 0);
        _SOC_IF_ERR_EXIT(rv);
        rv = SOC_WB_ENGINE_MEMSET_ARR(unit, SOC_WB_ENGINE_PORTMOD,
                                      pm_info->wb_vars_ids[maxSpeed], 0);
        _SOC_IF_ERR_EXIT(rv);
        rv = SOC_WB_ENGINE_MEMSET_ARR(unit, SOC_WB_ENGINE_PORTMOD,
                                      pm_info->wb_vars_ids[interfaceType],
                                      _SHR_PORT_IF_COUNT);
        _SOC_IF_ERR_EXIT(rv);
        rv = SOC_WB_ENGINE_MEMSET_ARR(unit, SOC_WB_ENGINE_PORTMOD,
                               pm_info->wb_vars_ids[serdesInterfaceType],
                               _SHR_PORT_IF_NULL);
        _SOC_IF_ERR_EXIT(rv);
        rv = SOC_WB_ENGINE_MEMSET_ARR(unit, SOC_WB_ENGINE_PORTMOD,
                                   pm_info->wb_vars_ids[interfaceModes], 0);
        _SOC_IF_ERR_EXIT(rv);
        rv = SOC_WB_ENGINE_MEMSET_ARR(unit, SOC_WB_ENGINE_PORTMOD,
                                      pm_info->wb_vars_ids[nofPhys], 0);
        _SOC_IF_ERR_EXIT(rv);
        rv = SOC_WB_ENGINE_MEMSET_ARR(unit, SOC_WB_ENGINE_PORTMOD,
                                   pm_info->wb_vars_ids[portDynamicState], 0);
        _SOC_IF_ERR_EXIT(rv);
        rv = SOC_WB_ENGINE_MEMSET_ARR(unit, SOC_WB_ENGINE_PORTMOD,
                                 pm_info->wb_vars_ids[isInterfaceRestore], 0);
        _SOC_IF_ERR_EXIT(rv);
        rv = SOC_WB_ENGINE_MEMSET_ARR(unit, SOC_WB_ENGINE_PORTMOD,
                               pm_info->wb_vars_ids[isHg], _SHR_PORT_IF_COUNT);
        _SOC_IF_ERR_EXIT(rv);
        rv = SOC_WB_ENGINE_MEMSET_ARR(unit, SOC_WB_ENGINE_PORTMOD,
                                      pm_info->wb_vars_ids[anMode],
                                      phymod_AN_MODE_Count);
        _SOC_IF_ERR_EXIT(rv);
        rv = SOC_WB_ENGINE_MEMSET_ARR(unit, SOC_WB_ENGINE_PORTMOD,
                                      pm_info->wb_vars_ids[fsCl72], 0);
        _SOC_IF_ERR_EXIT(rv);
        rv = SOC_WB_ENGINE_MEMSET_ARR(unit, SOC_WB_ENGINE_PORTMOD,
                                      pm_info->wb_vars_ids[cx4In10G], 0);
        _SOC_IF_ERR_EXIT(rv);
        rv = SOC_WB_ENGINE_MEMSET_ARR(unit, SOC_WB_ENGINE_PORTMOD,
                                      pm_info->wb_vars_ids[anCl72], 0);
        _SOC_IF_ERR_EXIT(rv);
        rv = SOC_WB_ENGINE_MEMSET_ARR(unit, SOC_WB_ENGINE_PORTMOD,
                                      pm_info->wb_vars_ids[anFec], 0);
        _SOC_IF_ERR_EXIT(rv);
        rv = SOC_WB_ENGINE_MEMSET_ARR(unit, SOC_WB_ENGINE_PORTMOD,
                                      pm_info->wb_vars_ids[anCl37], 0);
        _SOC_IF_ERR_EXIT(rv);
        rv = SOC_WB_ENGINE_MEMSET_ARR(unit, SOC_WB_ENGINE_PORTMOD,
                                      pm_info->wb_vars_ids[anMasterLane], -1);
        _SOC_IF_ERR_EXIT(rv);
        rv = SOC_WB_ENGINE_MEMSET_ARR(unit, SOC_WB_ENGINE_PORTMOD,
                                      pm_info->wb_vars_ids[rxauiMode], 0);
        _SOC_IF_ERR_EXIT(rv);
        rv = SOC_WB_ENGINE_MEMSET_ARR(unit, SOC_WB_ENGINE_PORTMOD,
                                   pm_info->wb_vars_ids[txParamsUserFlag], 0);
        _SOC_IF_ERR_EXIT(rv);
        rv = SOC_WB_ENGINE_MEMSET_ARR(unit, SOC_WB_ENGINE_PORTMOD,
                        pm_info->wb_vars_ids[extPhyTxParamsUserFlag], 0);
        _SOC_IF_ERR_EXIT(rv);
        rv = SOC_WB_ENGINE_MEMSET_ARR(unit, SOC_WB_ENGINE_PORTMOD,
                        pm_info->wb_vars_ids[pllDividerReq], 0);
        _SOC_IF_ERR_EXIT(rv);
        rv = SOC_WB_ENGINE_MEMSET_ARR(unit, SOC_WB_ENGINE_PORTMOD,
                        pm_info->wb_vars_ids[interfaceWrittenIntoOutmostPhy], _SHR_PORT_IF_COUNT);
        _SOC_IF_ERR_EXIT(rv);
    }
exit:
    SOC_FUNC_RETURN;
}

STATIC portmod_ucode_buf_t pm4x10_ucode_buf[SOC_MAX_NUM_DEVICES] = {{NULL, 0}};

int pm4x10_pm_destroy(int unit, pm_info_t pm_info)
{
    if(pm_info->pm_data.pm4x10_db != NULL) {
        sal_free(pm_info->pm_data.pm4x10_db);
        pm_info->pm_data.pm4x10_db = NULL;
    }

    /* free pm4x10_ucode_buf */
    if(pm4x10_ucode_buf[unit].ucode_dma_buf != NULL){
        portmod_sys_dma_free(unit, pm4x10_ucode_buf[unit].ucode_dma_buf);
        pm4x10_ucode_buf[unit].ucode_dma_buf = NULL;
    }
    pm4x10_ucode_buf[unit].ucode_alloc_size = 0;

    return SOC_E_NONE;
}

int
pm4x10_default_fw_loader(const phymod_core_access_t* core, uint32_t length, const uint8_t* data)
{
    int rv;
    portmod_default_user_access_t *user_data;
    int unit = ((portmod_default_user_access_t*)core->access.user_acc)->unit;
    portmod_ucode_buf_order_t load_order;
    SOC_INIT_FUNC_DEFS;

    user_data = (portmod_default_user_access_t*)core->access.user_acc;

    if (PORTMOD_USER_ACCESS_FW_LOAD_REVERSE_GET(user_data)) {
        load_order = portmod_ucode_buf_order_reversed;
    } else {
        load_order = portmod_ucode_buf_order_straight;
    }

    rv = portmod_firmware_set(unit,
                              user_data->blk_id,
                              data,
                              length,
                              load_order,
                              &(pm4x10_ucode_buf[unit]),
                              NULL,
                              XLPORT_WC_UCMEM_DATAm,
                              XLPORT_WC_UCMEM_CTRLr);
    _SOC_IF_ERR_EXIT(rv);

exit:
    SOC_FUNC_RETURN;
}

STATIC
int
pm4x10_default_bus_write(void* user_acc, uint32_t core_addr, uint32_t reg_addr, uint32_t val)
{
    return portmod_common_phy_sbus_reg_write(XLPORT_WC_UCMEM_DATAm, user_acc, core_addr, reg_addr, val);
}

STATIC
int
pm4x10_default_bus_read(void* user_acc, uint32_t core_addr, uint32_t reg_addr, uint32_t *val)
{
    return portmod_common_phy_sbus_reg_read(XLPORT_WC_UCMEM_DATAm, user_acc, core_addr, reg_addr, val);
}

/*
 * XLPORT_SOFT_RESET by physical port
 * This function expects 2nd argument port is physical port.
 */
STATIC
int _pm4x10_port_soft_reset_set_by_phyport(int unit, int port, pm_info_t pm_info, int idx, int val)
{
    int phy_port;
    uint32 reg_val, raddr;
    int    block;
    uint8  at;
    soc_field_t port_fields[] = {PORT0f, PORT1f, PORT2f, PORT3f};

    SOC_INIT_FUNC_DEFS;

    phy_port = port;
    raddr = soc_reg_addr_get(unit, XLPORT_SOFT_RESETr, phy_port, 0,
                 SOC_REG_ADDR_OPTION_PRESERVE_PORT,
                 &block, &at);

    _SOC_IF_ERR_EXIT
      (_soc_reg32_get(unit, block, at, raddr, &reg_val));

    soc_reg_field_set(unit, XLPORT_SOFT_RESETr, &reg_val, port_fields[idx], val);

    raddr = soc_reg_addr_get(unit, XLPORT_SOFT_RESETr, phy_port, 0,
                SOC_REG_ADDR_OPTION_WRITE | SOC_REG_ADDR_OPTION_PRESERVE_PORT,
                &block, &at);
    _SOC_IF_ERR_EXIT(_soc_reg32_set(unit, block, at, raddr, reg_val));

exit:
    SOC_FUNC_RETURN;
}

STATIC
int _pm4x10_port_soft_reset_set(int unit, int port, pm_info_t pm_info, int idx, int val)
{
    int phy_acc;
    uint32 reg_val;
    soc_field_t port_fields[] = {PORT0f, PORT1f, PORT2f, PORT3f};

    SOC_INIT_FUNC_DEFS;

    PM4x10_PHY_ACCESS_GET(unit, port, pm_info, phy_acc);

    _SOC_IF_ERR_EXIT(READ_XLPORT_SOFT_RESETr(unit, phy_acc, &reg_val));
    soc_reg_field_set(unit, XLPORT_SOFT_RESETr, &reg_val, port_fields[idx],
                      val);
    _SOC_IF_ERR_EXIT(WRITE_XLPORT_SOFT_RESETr(unit, phy_acc, reg_val));

exit:
    SOC_FUNC_RETURN;
}

int pm4x10_port_soft_reset_toggle(int unit, int port, pm_info_t pm_info, int idx)
{                    
    int phy_acc;      
    uint32 reg_val, old_val;
    soc_field_t port_fields[] = {PORT0f, PORT1f, PORT2f, PORT3f};

    SOC_INIT_FUNC_DEFS;

    PM4x10_PHY_ACCESS_GET(unit, port, pm_info, phy_acc);
            
    _SOC_IF_ERR_EXIT(READ_XLPORT_SOFT_RESETr(unit, phy_acc, &reg_val));
    old_val = reg_val;

    soc_reg_field_set(unit, XLPORT_SOFT_RESETr, &reg_val, port_fields[idx], 1); 

    _SOC_IF_ERR_EXIT(WRITE_XLPORT_SOFT_RESETr(unit, phy_acc, reg_val));
    _SOC_IF_ERR_EXIT(WRITE_XLPORT_SOFT_RESETr(unit, phy_acc, old_val));

exit:
    SOC_FUNC_RETURN;        
}

phymod_bus_t pm4x10_default_bus = {
    "PM4x10 Bus",
    pm4x10_default_bus_read,
    pm4x10_default_bus_write,
    NULL,
    portmod_common_mutex_take,
    portmod_common_mutex_give,
    PHYMOD_BUS_CAP_WR_MODIFY | PHYMOD_BUS_CAP_LANE_CTRL
};

int pm4x10_pm_init(int unit,
                   const portmod_pm_create_info_internal_t* pm_add_info,
                   int wb_buffer_index, pm_info_t pm_info)
{
    const portmod_pm4x10_create_info_t *info;
    pm4x10_t pm4x10_data = NULL;
    int i,j, pm_is_active, rv, three_ports_mode, nof_phys = 0;
    int pm_is_initialized;
    int bypass_enable;
    int lane, phyn;
    portmod_xphy_lane_connection_t lane_conn; 
    portmod_xphy_lane_connection_t *db_lane_conn;    
 
    SOC_INIT_FUNC_DEFS;

    info = &pm_add_info->pm_specific_info.pm4x10;
    pm4x10_data = sal_alloc(sizeof(struct pm4x10_s), "pm4x10_specific_db");
    SOC_NULL_CHECK(pm4x10_data);

    pm_info->type = pm_add_info->type;
    pm_info->unit = unit;
    pm_info->wb_buffer_id = wb_buffer_index;
    pm_info->pm_data.pm4x10_db = pm4x10_data;
    PORTMOD_PBMP_ASSIGN(pm4x10_data->phys, pm_add_info->phys);
    pm4x10_data->int_core_access.type = phymodDispatchTypeCount;
    pm4x10_data->default_bus_is_used = 0;
    pm4x10_data->default_fw_loader_is_used = 0;
    pm4x10_data->portmod_phy_external_reset = pm_add_info->pm_specific_info.pm4x10.portmod_phy_external_reset;
    pm4x10_data->portmod_mac_soft_reset = pm_add_info->pm_specific_info.pm4x10.portmod_mac_soft_reset;
    pm4x10_data->first_phy = -1;
    pm4x10_data->warmboot_skip_db_restore = TRUE;
    pm4x10_data->rescal = pm_add_info->pm_specific_info.pm4x10.rescal;
    pm4x10_data->is_pm4x10q = pm_add_info->pm_specific_info.pm4x10.is_pm4x10q;

    /* initialize num of phys for each lane */
    for(i = 0 ; i < PM4X10_LANES_PER_CORE ; i++){
        pm4x10_data->nof_phys[i] = 0;
    }
    /* init intertnal SerDes core access */
    phymod_core_access_t_init(&pm4x10_data->int_core_access);

    /* initialize lane connections */
    for(i = 0 ; i < MAX_PHYN ; i++){
        for(j = 0 ; j < PM4X10_LANES_PER_CORE ; j++){
            portmod_xphy_lane_connection_t_init(unit, &pm4x10_data->lane_conn[i][j]);
        }
    }

    sal_memcpy(&pm4x10_data->polarity, &info->polarity, 
                sizeof(phymod_polarity_t));
    sal_memcpy(&(pm4x10_data->int_core_access.access), &info->access,
                sizeof(phymod_access_t));
    sal_memcpy(&pm4x10_data->lane_map, &info->lane_map,
                sizeof(pm4x10_data->lane_map));
    pm4x10_data->ref_clk = info->ref_clk;
    pm4x10_data->fw_load_method = info->fw_load_method;
    pm4x10_data->external_fw_loader = info->external_fw_loader;

    if(info->access.bus == NULL) {
        /* if null - use default */
        pm4x10_data->int_core_access.access.bus = &pm4x10_default_bus;
        pm4x10_data->default_bus_is_used = 1;
    }

    if(pm4x10_data->external_fw_loader == NULL) {
        /* if null - use default */
        pm4x10_data->external_fw_loader = pm4x10_default_fw_loader;
        pm4x10_data->default_fw_loader_is_used = 1;
    }
 
   /* initialize num of phys for each lane */
    for(i = 0 ; i < PM4X10_LANES_PER_CORE ; i++){
        pm4x10_data->nof_phys[i] = 1;
    }
 
    pm4x10_data->in_pm12x10   = info->in_pm_12x10;
    pm4x10_data->core_num     = info->core_num;
    pm4x10_data->core_num_int = info->core_num_int;

    /*init wb buffer*/
    _SOC_IF_ERR_EXIT(pm4x10_wb_buffer_init(unit, wb_buffer_index,  pm_info));

    if(SOC_WARM_BOOT(unit)){

        rv = SOC_WB_ENGINE_GET_ARR(unit, SOC_WB_ENGINE_PORTMOD,
                         pm_info->wb_vars_ids[phy_type], &pm4x10_data->int_core_access.type, 0);

        /* restore nof_phys from wb */
        for(i = 0 ; i < PM4X10_LANES_PER_CORE ; i++){
            _SOC_IF_ERR_EXIT(PM4x10_PORT_NUM_PHYS_GET(unit, pm_info, &nof_phys, i));
            pm4x10_data->nof_phys[i] = nof_phys;
        }

        for(phyn = 0 ; phyn < MAX_PHYN ; phyn++){
            for(lane = 0 ; lane < PM4X10_LANES_PER_CORE ; lane++){
                db_lane_conn = &(PM_4x10_INFO(pm_info)->lane_conn[phyn][lane]);
                _SOC_IF_ERR_EXIT(PM4x10_PORT_LANE_CONNECTION_DB_GET(unit, pm_info,
                                 db_lane_conn, phyn, lane));
            }
        }

        _SOC_IF_ERR_EXIT(rv);
    }

    if(!SOC_WARM_BOOT(unit)){
        /* Cold Boot  */

        for(i = 0 ; i < PM4X10_LANES_PER_CORE ; i++){
            nof_phys = 1;
            rv = PM4x10_PORT_NUM_PHYS_SET(unit, pm_info, &nof_phys, i);
            _SOC_IF_ERR_EXIT(rv);
        }

        pm_is_active = 0;
        rv = SOC_WB_ENGINE_SET_VAR (unit, SOC_WB_ENGINE_PORTMOD, 
                                pm_info->wb_vars_ids[isActive], &pm_is_active);
        _SOC_IF_ERR_EXIT(rv);

        pm_is_initialized = 0;
        rv = SOC_WB_ENGINE_SET_VAR (unit, SOC_WB_ENGINE_PORTMOD, 
                                pm_info->wb_vars_ids[isInitialized], &pm_is_initialized);
        _SOC_IF_ERR_EXIT(rv);

        bypass_enable = 0;
        rv = SOC_WB_ENGINE_SET_VAR (unit, SOC_WB_ENGINE_PORTMOD, 
                             pm_info->wb_vars_ids[isBypassed], &bypass_enable);
        _SOC_IF_ERR_EXIT(rv);

        three_ports_mode = info->three_ports_mode;
        rv = SOC_WB_ENGINE_SET_VAR(unit, SOC_WB_ENGINE_PORTMOD, 
                      pm_info->wb_vars_ids[threePortsMode], &three_ports_mode);
        _SOC_IF_ERR_EXIT(rv);

         rv = SOC_WB_ENGINE_SET_ARR(unit, SOC_WB_ENGINE_PORTMOD,
                    pm_info->wb_vars_ids[phy_type],
                    &PM_4x10_INFO(pm_info)->int_core_access.type, 0);
         _SOC_IF_ERR_EXIT(rv);

         for(phyn = 0 ; phyn < MAX_PHYN ; phyn++){
            for(lane = 0 ; lane < PM4X10_LANES_PER_CORE ; lane++){
                 portmod_xphy_lane_connection_t_init(unit, &lane_conn);
                _SOC_IF_ERR_EXIT(PM4x10_PORT_LANE_CONNECTION_DB_SET(unit, pm_info,
                                 &lane_conn, phyn, lane));
            }
        }
    }

exit:
    if(SOC_FUNC_ERROR){
        pm4x10_pm_destroy(unit, pm_info);
    }
    SOC_FUNC_RETURN;
}

int
pm4x10_default_bus_update(int unit, pm_info_t pm_info, const portmod_bus_update_t* update) 
{
    portmod_default_user_access_t* user_acc;
    soc_error_t rv = SOC_E_NONE;
    SOC_INIT_FUNC_DEFS;

    if (update == NULL) /*restore to default*/
    {
        /* if null - use default */
        PM_4x10_INFO(pm_info)->int_core_access.access.bus = &pm4x10_default_bus;
        PM_4x10_INFO(pm_info)->int_core_access.type = phymodDispatchTypeCount;
        PM_4x10_INFO(pm_info)->default_bus_is_used = 1;
    
        PM_4x10_INFO(pm_info)->external_fw_loader = pm4x10_default_fw_loader;
        PM_4x10_INFO(pm_info)->default_fw_loader_is_used = 1;
    }
    else
    {

        if(PM_4x10_INFO(pm_info)->default_fw_loader_is_used && update->external_fw_loader != NULL) {
            PM_4x10_INFO(pm_info)->external_fw_loader = update->external_fw_loader;
            user_acc = (portmod_default_user_access_t*)PM_4x10_INFO(pm_info)->int_core_access.access.user_acc;
            user_acc->blk_id = (update->blk_id == -1) ? user_acc->blk_id : update->blk_id;
            PM_4x10_INFO(pm_info)->default_fw_loader_is_used = 0;
        }

        if(PM_4x10_INFO(pm_info)->default_bus_is_used && update->default_bus != NULL) {
            PM_4x10_INFO(pm_info)->int_core_access.access.bus = update->default_bus; 
            user_acc = (portmod_default_user_access_t*)PM_4x10_INFO(pm_info)->int_core_access.access.user_acc;
            user_acc->blk_id = (update->blk_id == -1) ? user_acc->blk_id : update->blk_id;
            if (PM_4x10_INFO(pm_info)->int_core_access.type >= phymodDispatchTypeCount) {
                PM_4x10_INFO(pm_info)->int_core_access.type = phymodDispatchTypeCount;
            }
            PM_4x10_INFO(pm_info)->default_bus_is_used = 0;
        }

        if (update->user_acc != NULL) {
            PM_4x10_INFO(pm_info)->int_core_access.access.user_acc = update->user_acc;
        }

    }

    /*update warmboot engine*/
    rv = SOC_WB_ENGINE_SET_ARR(unit, SOC_WB_ENGINE_PORTMOD,
        pm_info->wb_vars_ids[phy_type],
        &PM_4x10_INFO(pm_info)->int_core_access.type, 0);
    _SOC_IF_ERR_EXIT(rv);

exit:
    SOC_FUNC_RETURN;
}

STATIC
int _pm4x10_port_tricore_lane_info_get(int unit, int port, pm_info_t pm_info,
                                       int *start_lane, int* end_lane,
                                       int *line_start_lane, int* line_end_lane,
                                       uint32 *int_core_lane_mask);

STATIC
int pm4x10_port_phy_init_config_restore (int unit, int port, pm_info_t pm_info,
                               const portmod_port_init_config_t *init_config)
{
    phymod_phy_access_t         phy_access_arr[1+MAX_PHYN];
    portmod_access_get_params_t params;
    phymod_tx_t                 phymod_tx, phymod_tx_default;
    int core_start_lane=-1, core_end_lane=-1, line_start_lane=-1, line_end_lane=-1;
    int lane;
    uint32 int_lane_mask=0xf;

    int     nof_phys, rv = 0;
    int     i, is_legacy_present;

    phymod_tx_t_init(&phymod_tx);
    phymod_tx_t_init(&phymod_tx_default);

    rv = portmod_access_get_params_t_init(unit, &params);
    if (SOC_FAILURE(rv)) return (rv);

    rv = portmod_port_chain_phy_access_get(unit, port, pm_info,
                                           phy_access_arr ,(1+MAX_PHYN),
                                           &nof_phys);
    if (SOC_FAILURE(rv)) return (rv);

    /* Figure out the start lane if the current core belongs to a tricore. */ 
    rv = _pm4x10_port_tricore_lane_info_get(unit, port, pm_info,
                                            &core_start_lane, &core_end_lane,
                                            &line_start_lane, &line_end_lane,
                                            &int_lane_mask);
    if (SOC_FAILURE(rv)) return (rv);
    /* if current core does not belong to a tricore, set the start lane to 0 */
    if(core_start_lane==-1){
        core_start_lane = 0;
    }

    if( nof_phys > 1 ){
        rv = portmod_port_is_legacy_ext_phy_present(unit, port, &is_legacy_present);
        if (SOC_FAILURE(rv)) return (rv);
        if (!is_legacy_present) {
            /* get external phy default tx params */
            rv = phymod_phy_media_type_tx_get(&phy_access_arr[nof_phys-1], phymodMediaTypeChipToChip, &phymod_tx_default);
            if (SOC_FAILURE(rv)) return (rv);
        }

        for(i=0 ; i<PM4X10_LANES_PER_CORE ; i++) {
            lane = i + core_start_lane;
            phymod_tx = phymod_tx_default;

            /* overlap external phy preemphasis and amp from config */
            if(init_config->ext_phy_tx_params_user_flag[lane] & PORTMOD_USER_SET_TX_PREEMPHASIS_BY_CONFIG) {
                phymod_tx.pre  = init_config->ext_phy_tx_params[lane].pre ;
                phymod_tx.main = init_config->ext_phy_tx_params[lane].main ;
                phymod_tx.post = init_config->ext_phy_tx_params[lane].post ;
            }
            if(init_config->ext_phy_tx_params_user_flag[lane] & PORTMOD_USER_SET_TX_AMP_BY_CONFIG) {
                phymod_tx.amp  = init_config->ext_phy_tx_params[lane].amp ;
            }
            /* save to warmboot buffer */
            rv = PM4x10_EXT_PHY_TX_PARAMS_SET(unit, pm_info, phymod_tx, i);
            if (SOC_FAILURE(rv)) return (rv);
        }
    }

    /* get internal serdes default tx params */
    rv = phymod_phy_media_type_tx_get(&phy_access_arr[0], phymodMediaTypeChipToChip, &phymod_tx_default);
    if (SOC_FAILURE(rv)) return (rv);

    for(i=0 ; i<PM4X10_LANES_PER_CORE ; i++) {
        lane = i + core_start_lane;
        phymod_tx = phymod_tx_default;

        /* overlap serdes preemphasis and amp from config */
        if(init_config->tx_params_user_flag[lane] & PORTMOD_USER_SET_TX_PREEMPHASIS_BY_CONFIG) {
            phymod_tx.pre  = init_config->tx_params[lane].pre ;
            phymod_tx.main = init_config->tx_params[lane].main ;
            phymod_tx.post = init_config->tx_params[lane].post ;
        }
        if(init_config->tx_params_user_flag[lane] & PORTMOD_USER_SET_TX_AMP_BY_CONFIG) {
            phymod_tx.amp  = init_config->tx_params[lane].amp ;
        }
        /* save to warmboot buffer */
        rv = PM4x10_TX_PARAMS_SET(unit, pm_info, phymod_tx, i);
        if (SOC_FAILURE(rv)) return (rv);
    }

    return (SOC_E_NONE);
}

STATIC
int _pm4x10_nof_lanes_get(int unit, int port, pm_info_t pm_info)
{
    int port_index, rv;
    uint32_t bitmap, bcnt = 0;

    rv = _pm4x10_port_index_get (unit, port, pm_info, &port_index,
                                             &bitmap);
    if (rv) return (0); /* bit count is zero will flag error */

    while (bitmap) {
        if (bitmap & 0x1) bcnt++;
        bitmap >>= 1;
    }

    return (bcnt);
}

STATIC
int _pm4x10_an_mode_update (int unit, int port, pm_info_t pm_info,
                            int an_cl37, int an_cl73);

int pm4x10_port_warmboot_db_restore(int unit, int port, pm_info_t pm_info,
                         const portmod_port_interface_config_t *f_intf_config,
                         const portmod_port_init_config_t      *f_init_config,
                         const phymod_operation_mode_t phy_op_mode)
{
    pm4x10_t pm4x10_data = NULL;
    phymod_phy_access_t                 phy_access[1+MAX_PHYN];
    phymod_phy_inf_config_t            phy_intf_config;
    phymod_autoneg_control_t            an; 
    portmod_encap_t                    encap_mode;
    soc_port_if_t                      interface;

    int     phy, nof_phys, port_index, rv = 0, flags = 0;
    uint32  bitmap, an_done, interface_modes = 0, is_interface_restore = 0;

    /*
     * if no warmboot db restore is require, just return here.
     */
    if(PM_4x10_INFO(pm_info)->warmboot_skip_db_restore){
        return SOC_E_NONE;
    }

    rv = _pm4x10_port_index_get (unit, port, pm_info, &port_index, &bitmap);
    if (SOC_FAILURE(rv)) return (rv);

    pm4x10_data      = pm_info->pm_data.pm4x10_db;
    phymod_phy_inf_config_t_init(&phy_intf_config);

    rv = PM4x10_IS_HG_SET(unit, pm_info, f_init_config->is_hg, port_index);
    if (SOC_FAILURE(rv)) return (rv);

    rv = PM4x10_AN_MODE_SET(unit, pm_info, f_init_config->an_mode, port_index);
    if (SOC_FAILURE(rv)) return (rv);

    rv = PM4x10_FS_CL72_SET(unit, pm_info, f_init_config->fs_cl72, port_index);
    if (SOC_FAILURE(rv)) return (rv);

    rv = PM4x10_CX4_10G_SET(unit, pm_info, f_init_config->cx4_10g, port_index);
    if (SOC_FAILURE(rv)) return (rv);

    rv = PM4x10_AN_CL72_SET(unit, pm_info, f_init_config->an_cl72, port_index);
    if (SOC_FAILURE(rv)) return (rv);

    rv = PM4x10_AN_FEC_SET(unit, pm_info, f_init_config->an_fec, port_index);
    if (SOC_FAILURE(rv)) return (rv);

    rv = PM4x10_AN_CL37_SET(unit, pm_info, f_init_config->an_cl37, port_index);
    if (SOC_FAILURE(rv)) return (rv);

    rv = PM4x10_AN_MASTER_LANE_SET(unit, pm_info,
                                   f_init_config->an_master_lane,
                                   port_index);
    if (SOC_FAILURE(rv)) return (rv);

    rv = PM4x10_PORT_FALLBACK_LANE_SET(unit, pm_info,
                                   f_init_config->port_fallback_lane,
                                   port_index);
    if (SOC_FAILURE(rv)) return (rv);

    rv = PM4x10_MAX_SPEED_SET(unit, pm_info, f_intf_config->max_speed,
                              port_index);
    if (SOC_FAILURE(rv)) return (rv);

    rv = PM4x10_RXAUI_MODE_SET(unit, pm_info, f_init_config->rxaui_mode,
                               port_index);
    if (SOC_FAILURE(rv)) return (rv);


    rv = PM4x10_INTERFACE_TYPE_SET(unit, pm_info,
                                   f_intf_config->interface,
                                   port_index);
    if (SOC_FAILURE(rv)) return (rv);

    rv = PM4x10_SERDES_INTERFACE_TYPE_SET(unit, pm_info,
                                          f_intf_config->serdes_interface,
                                          port_index);
    if (SOC_FAILURE(rv)) return (rv);

    PORTMOD_PBMP_ITER(PM_4x10_INFO(pm_info)->phys, phy) {
        pm4x10_data->first_phy = phy;
        break;
    }

    rv = portmod_port_chain_phy_access_get (unit, port, pm_info, phy_access,
                                           (1+MAX_PHYN), &nof_phys);
    if (SOC_FAILURE(rv)) return (rv);

    rv = portmod_port_phychain_interface_config_get(unit, port, phy_access, nof_phys,
                                   0, phymodRefClk156Mhz, &phy_intf_config, PORTMOD_INIT_F_EXTERNAL_MOST_ONLY);
    if (SOC_FAILURE(rv)) return (rv);

    rv = PM4x10_IS_INTERFACE_RESTORE_SET(unit, pm_info, is_interface_restore,
                                         port_index);
    if (SOC_FAILURE(rv)) return (rv);

    rv = PM4x10_INTERFACE_MODES_GET(unit, pm_info, &interface_modes,
                                    port_index);
    if (SOC_FAILURE(rv)) return (rv);

    interface_modes |= phy_intf_config.interface_modes;

    rv = PM4x10_INTERFACE_MODES_SET(unit, pm_info, interface_modes, port_index);
    if (SOC_FAILURE(rv)) return (rv);

    rv = portmod_port_interface_type_get (unit, port, &interface);
    if (SOC_FAILURE(rv)) return (rv);

    rv = PM4x10_INTERFACE_TYPE_SET(unit, pm_info, interface, port_index);
    if (SOC_FAILURE(rv)) return (rv);

    rv = PM4x10_SPEED_SET(unit, pm_info, phy_intf_config.data_rate, port_index);
    if (SOC_FAILURE(rv)) return (rv);

    xlmac_encap_get(unit, port, &flags, &encap_mode);

    if ((encap_mode == SOC_ENCAP_HIGIG2) || (encap_mode == SOC_ENCAP_HIGIG)) {
        interface_modes |= PHYMOD_INTF_MODES_HIGIG;
        rv = PM4x10_INTERFACE_MODES_SET(unit, pm_info, interface_modes,
                                        port_index);
        if (SOC_FAILURE(rv)) return (rv);
    }
    
    rv = portmod_port_phychain_autoneg_get(unit, port, phy_access, nof_phys, PORTMOD_INIT_F_EXTERNAL_MOST_ONLY, &an, &an_done);
    if (SOC_FAILURE(rv)) return (rv);

    rv = PM4x10_AN_MODE_SET(unit, pm_info, an.an_mode, port_index);
    if (SOC_FAILURE(rv)) return (rv);

    if(an.an_mode == phymod_AN_MODE_NONE){
        _pm4x10_an_mode_update(unit, port, pm_info, f_init_config->an_cl37,
                               f_init_config->an_cl73);
    }
    
    rv = PM4x10_PORT_OP_MODE_SET(unit, pm_info, phy_op_mode, port_index);
    if (SOC_FAILURE(rv)) return (rv);

    rv = pm4x10_port_phy_init_config_restore(unit, port, pm_info,
                                             f_init_config);
    if (SOC_FAILURE(rv)) return (rv);

    return (SOC_E_NONE);
}

STATIC
int _pm4x10_tsc_reset(int unit, pm_info_t pm_info, int port, int in_reset)
{
    int phy_acc;
    uint32 reg_val, phy;
    SOC_INIT_FUNC_DEFS;

    PM4x10_PHY_ACCESS_GET(unit, port, pm_info, phy_acc);

    PORTMOD_PBMP_ITER(PM_4x10_INFO(pm_info)->phys, phy) {break;};

    if(PM_4x10_INFO(pm_info)->in_pm12x10) {
#ifdef PORTMOD_PM12X10_SUPPORT
#ifdef PORTMOD_PM12X10_XGS_SUPPORT
        
        if (SOC_IS_APACHE(unit)) {
            _SOC_IF_ERR_EXIT(pm12x10_xgs_pm4x10_tsc_reset(unit, port, phy, in_reset));
        } else
#endif
        {
            _SOC_IF_ERR_EXIT(pm12x10_pm4x10_tsc_reset(unit, port, phy, in_reset));
        }
#endif
    } else {
        _SOC_IF_ERR_EXIT(READ_XLPORT_XGXS0_CTRL_REGr(unit, phy_acc, &reg_val));

        soc_reg_field_set (unit, XLPORT_XGXS0_CTRL_REGr, &reg_val, RSTB_HWf,
                           in_reset ? 0 : 1);
        soc_reg_field_set (unit, XLPORT_XGXS0_CTRL_REGr, &reg_val, PWRDWNf,
                           in_reset ? 1 : 0);
        soc_reg_field_set (unit, XLPORT_XGXS0_CTRL_REGr, &reg_val, IDDQf,
                           in_reset ? 1 : 0);

        _SOC_IF_ERR_EXIT(WRITE_XLPORT_XGXS0_CTRL_REGr(unit, phy_acc, reg_val));
    }

    sal_usleep(1100);

exit:
    SOC_FUNC_RETURN;
}

static int _xlport_mode_get(int unit, int phy_acc, int bindex, 
                      portmod_core_port_mode_t *core_mode, int *cur_lanes)
{
    uint32_t rval;

    SOC_INIT_FUNC_DEFS;

    /* Toggle link bit to notify IPIPE on link up */
    _SOC_IF_ERR_EXIT(READ_XLPORT_MODE_REGr(unit, phy_acc, &rval));

    *core_mode = soc_reg_field_get(unit, XLPORT_MODE_REGr, rval, XPORT0_CORE_PORT_MODEf);

    switch (*core_mode) {
        case portmodPortModeQuad:
            *cur_lanes = 1;
            break;

        case portmodPortModeTri012: 
            *cur_lanes = (bindex == 2 ? 2 : 1);
            break;

        case portmodPortModeTri023:
            *cur_lanes = (bindex == 0 ? 2 : 1);
            break;

        case portmodPortModeDual: 
            *cur_lanes = 2;
            break;

        case portmodPortModeSingle:
            *cur_lanes = 4;
            break;

        default:
            return SOC_E_FAIL;
    }

exit:
    SOC_FUNC_RETURN;
}

static int _xlport_mode_set(int unit, int phy_acc, portmod_core_port_mode_t cur_mode)
{
    uint32_t rval; 
    int      mode;

    SOC_INIT_FUNC_DEFS;

    switch (cur_mode) {
        case portmodPortModeQuad:
            mode = 0;
            break;

        case portmodPortModeTri012: 
            mode = 1; 
            break;

        case portmodPortModeTri023:
            mode = 2; 
            break;

        case portmodPortModeDual: 
            mode = 3; 
            break;

        case portmodPortModeSingle:
            mode = 4; 
            break;

        default:
            return SOC_E_FAIL;
    }

    /* Toggle link bit to notify IPIPE on link up */
    _SOC_IF_ERR_EXIT(READ_XLPORT_MODE_REGr(unit, phy_acc, &rval));

    soc_reg_field_set(unit, XLPORT_MODE_REGr, &rval, XPORT0_CORE_PORT_MODEf, mode);
    soc_reg_field_set(unit, XLPORT_MODE_REGr, &rval, XPORT0_PHY_PORT_MODEf, mode);

    _SOC_IF_ERR_EXIT(WRITE_XLPORT_MODE_REGr(unit, phy_acc, rval));

exit:
    SOC_FUNC_RETURN;
}

int pm4x10_port_cntmaxsize_get(int unit, int port, pm_info_t pm_info, int *val)
{
    int phy_acc;

    PM4x10_PHY_ACCESS_GET(unit, port, pm_info, phy_acc);

    return (READ_XLPORT_CNTMAXSIZEr(unit, phy_acc, (uint32_t *)val));
}

int pm4x10_port_cntmaxsize_set(int unit, int port, pm_info_t pm_info, int val)
{
    uint32 reg_val;
    int phy_acc, rv = 0;

    PM4x10_PHY_ACCESS_GET(unit, port, pm_info, phy_acc);

    rv = READ_XLPORT_CNTMAXSIZEr(unit, phy_acc, &reg_val);
    if (SOC_FAILURE(rv)) return (rv);

    soc_reg_field_set(unit, XLPORT_CNTMAXSIZEr, &reg_val, CNTMAXSIZEf, val);

    return(WRITE_XLPORT_CNTMAXSIZEr(unit, phy_acc, reg_val));
}

int pm4x10_port_mib_reset_toggle(int unit, int port, pm_info_t pm_info, int port_index)
{
    uint32 reg_val[1];
    int    rv = 0, phy_acc;

    PM4x10_PHY_ACCESS_GET(unit, port, pm_info, phy_acc);

    rv = READ_XLPORT_MIB_RESETr(unit, phy_acc, reg_val);
    if (SOC_FAILURE(rv)) return (rv);
    SHR_BITSET(reg_val, port_index);

    rv = WRITE_XLPORT_MIB_RESETr(unit, phy_acc, *reg_val);
    if (SOC_FAILURE(rv)) return (rv);
    SHR_BITCLR(reg_val, port_index);

    return(WRITE_XLPORT_MIB_RESETr(unit, phy_acc, *reg_val));
}
int pm4x10_port_modid_set (int unit, int port, pm_info_t pm_info, int value)
{
    uint32_t rval, modid;
    int      flen, phy_acc;
    SOC_INIT_FUNC_DEFS;

    PM4x10_PHY_ACCESS_GET(unit, port, pm_info, phy_acc);

    flen = soc_reg_field_length(unit, XLPORT_CONFIGr, MY_MODIDf)? value : 0;
    modid =  (value < (1 <<  flen))? value : 0;

    _SOC_IF_ERR_EXIT(READ_XLPORT_CONFIGr(unit, phy_acc, &rval));
    soc_reg_field_set(unit, XLPORT_CONFIGr, &rval, MY_MODIDf, modid);
    _SOC_IF_ERR_EXIT(WRITE_XLPORT_CONFIGr(unit, phy_acc, rval));

exit:
    SOC_FUNC_RETURN;
}

int pm4x10_port_higig2_mode_set (int unit, int port, pm_info_t pm_info, int mode)
{
    int phy_acc;
    uint32_t rval;
    SOC_INIT_FUNC_DEFS;

    PM4x10_PHY_ACCESS_GET(unit, port, pm_info, phy_acc);

    _SOC_IF_ERR_EXIT(READ_XLPORT_CONFIGr(unit, phy_acc, &rval));
    soc_reg_field_set(unit, XLPORT_CONFIGr, &rval, HIGIG2_MODEf, mode);
    _SOC_IF_ERR_EXIT(WRITE_XLPORT_CONFIGr(unit, phy_acc, rval));

exit:
    SOC_FUNC_RETURN;
}

int pm4x10_port_higig_mode_set (int unit, int port, pm_info_t pm_info, int mode)
{
    int phy_acc;
    uint32_t rval;
    SOC_INIT_FUNC_DEFS;

    PM4x10_PHY_ACCESS_GET(unit, port, pm_info, phy_acc);

    _SOC_IF_ERR_EXIT(READ_XLPORT_CONFIGr(unit, phy_acc, &rval));
    soc_reg_field_set(unit, XLPORT_CONFIGr, &rval, HIGIG_MODEf, mode);
    _SOC_IF_ERR_EXIT(WRITE_XLPORT_CONFIGr(unit, phy_acc, rval));

exit:
    SOC_FUNC_RETURN;
}

int pm4x10_port_higig2_mode_get (int unit, int port, pm_info_t pm_info, int *mode)
{
    int phy_acc;
    uint32_t rval;
    SOC_INIT_FUNC_DEFS;

    PM4x10_PHY_ACCESS_GET(unit, port, pm_info, phy_acc);

    _SOC_IF_ERR_EXIT(READ_XLPORT_CONFIGr(unit, phy_acc, &rval));
    *(mode) = soc_reg_field_get(unit, XLPORT_CONFIGr, rval, HIGIG2_MODEf);

exit:
    SOC_FUNC_RETURN;
}

int pm4x10_port_higig_mode_get (int unit, int port, pm_info_t pm_info, int *mode)
{
    int phy_acc;
    uint32_t rval;
    SOC_INIT_FUNC_DEFS;

    PM4x10_PHY_ACCESS_GET(unit, port, pm_info, phy_acc);

    _SOC_IF_ERR_EXIT(READ_XLPORT_CONFIGr(unit, phy_acc, &rval));
    *(mode) = soc_reg_field_get(unit, XLPORT_CONFIGr, rval, HIGIG_MODEf);

exit:
    SOC_FUNC_RETURN;
}

int pm4x10_port_encap_set (int unit, int port, pm_info_t pm_info, 
                                int flags, portmod_encap_t encap)
{
    return (xlmac_encap_set(unit, port, flags, encap));

}

int pm4x10_port_encap_get (int unit, int port, pm_info_t pm_info, 
                            int *flags, portmod_encap_t *encap)
{
    return (xlmac_encap_get(unit, port, flags, encap));
}

int pm4x10_port_config_port_type_set (int unit, int port, pm_info_t pm_info, int type)
{
    int phy_acc;
    uint32_t rval;
    SOC_INIT_FUNC_DEFS;

    PM4x10_PHY_ACCESS_GET(unit, port, pm_info, phy_acc);

    _SOC_IF_ERR_EXIT(READ_XLPORT_CONFIGr(unit, phy_acc, &rval));
    soc_reg_field_set(unit, XLPORT_CONFIGr, &rval, PORT_TYPEf, type);
    _SOC_IF_ERR_EXIT(WRITE_XLPORT_CONFIGr(unit, phy_acc, rval));

exit:
    SOC_FUNC_RETURN;
}

int pm4x10_port_config_port_type_get (int unit, int port, pm_info_t pm_info, int *type)
{
    int phy_acc;
    uint32_t rval;
    SOC_INIT_FUNC_DEFS;

    PM4x10_PHY_ACCESS_GET(unit, port, pm_info, phy_acc);

    _SOC_IF_ERR_EXIT(READ_XLPORT_CONFIGr(unit, phy_acc, &rval));
    *(type) = soc_reg_field_get(unit, XLPORT_CONFIGr, rval, PORT_TYPEf);

exit:
    SOC_FUNC_RETURN;
}

int pm4x10_port_led_chain_config (int unit, int port, pm_info_t pm_info, 
                          int value)
{
    SOC_INIT_FUNC_DEFS;

    _SOC_IF_ERR_EXIT(WRITE_XLPORT_LED_CHAIN_CONFIGr (unit, port, value));

exit:
    SOC_FUNC_RETURN;
}

typedef enum xlmac_port_mode_e{
    XLMAC_4_LANES_SEPARATE  = 0,
    XLMAC_3_TRI_0_1_2_2     = 1,
    XLMAC_3_TRI_0_0_2_3     = 2,
    XLMAC_2_LANES_DUAL      = 3,
    XLMAC_4_LANES_TOGETHER  = 4
} xlmac_port_mode_t;

STATIC
int _pm4x10_port_tricore_lane_info_get(int unit, int port, pm_info_t pm_info,
                                       int *start_lane, int* end_lane, 
                                       int *line_start_lane, int* line_end_lane, 
                                       uint32 *int_core_lane_mask) 
{
    uint32  iphy_lane_mask, interface_modes = 0;
    int port_index=0;
    int core_num_int;
    int data_rate;
    SOC_INIT_FUNC_DEFS;

    /* get port index and lane mask */
    _SOC_IF_ERR_EXIT(_pm4x10_port_index_get(unit, port, pm_info, &port_index,
                                            &iphy_lane_mask));

    *start_lane  = 0;
    *end_lane    = PM4X10_LANES_PER_CORE-1;
    *int_core_lane_mask = 0x0F;

    *line_start_lane = 0;
    *line_end_lane = PM4X10_LANES_PER_CORE-1;

    core_num_int = PM_4x10_INFO(pm_info)->core_num_int;
                                  
    _SOC_IF_ERR_EXIT(PM4x10_INTERFACE_MODES_GET(unit, pm_info,
                                                &interface_modes,
                                                port_index));

    if(interface_modes & PHYMOD_INTF_MODES_TRIPLE_CORE) {
       _SOC_IF_ERR_EXIT(PM4x10_SPEED_GET(unit, pm_info,
                                         &data_rate,
                                         port_index));

        /* first check if 120G or 127G */
        if (data_rate >= 120000) {
            if(core_num_int == 0){
                *start_lane  = 0;
                *end_lane    = PM4X10_LANES_PER_CORE-1;
                *int_core_lane_mask = 0x0F;
                *line_start_lane = -1;
                *line_end_lane = -1;
             } else if (core_num_int == 1) {
                *start_lane  = 4;
                *end_lane    = 7;
                *line_start_lane = 0;
                *line_end_lane = PM4X10_LANES_PER_CORE-1;
                *int_core_lane_mask = 0x0F;
            } else if (core_num_int == 2) {
                *start_lane     = 8;
                *end_lane       = 11;
                *int_core_lane_mask = 0x0F;
                *line_start_lane = -1;
                *line_end_lane = -1;
            }

        } else {
            if(core_num_int == 0){
                if (interface_modes & PHYMOD_INTF_MODES_TC_244){
                    *start_lane     = 0  ;
                    *end_lane       = 1  ;
                    *int_core_lane_mask = 0x03;
                } else if (interface_modes & PHYMOD_INTF_MODES_TC_343){
                    *start_lane     = 0  ;  
                    *end_lane       = 2  ;
                    *int_core_lane_mask = 0x07;
                } else if (interface_modes & PHYMOD_INTF_MODES_TC_442){
                    *start_lane  = 0;
                    *end_lane    = PM4X10_LANES_PER_CORE-1;
                    *int_core_lane_mask = 0x0F;
                }
                *line_start_lane = -1;
                *line_end_lane = -1;

            } else if (core_num_int == 1) {
                if (interface_modes & PHYMOD_INTF_MODES_TC_244){
                    *start_lane     = 2  ;  
                    *end_lane       = 5  ;
                } else if (interface_modes & PHYMOD_INTF_MODES_TC_343){
                    *start_lane     = 3  ;
                    *end_lane       = 6  ;
                } else if (interface_modes & PHYMOD_INTF_MODES_TC_442){
                    *start_lane  = 4;
                    *end_lane    = 7;
                }
                *line_start_lane = 0;
                *line_end_lane = PM4X10_LANES_PER_CORE-1;
                *int_core_lane_mask = 0x0F;
            } else if (core_num_int == 2) {
                if (interface_modes & PHYMOD_INTF_MODES_TC_244){
                    *start_lane     = 6  ;  
                    *end_lane       = 9  ;
                    *int_core_lane_mask = 0x0F;
                } else if (interface_modes & PHYMOD_INTF_MODES_TC_343){
                    *start_lane     = 7  ;
                    *end_lane       = 9  ;
                    *int_core_lane_mask = 0x07;
                } else if (interface_modes & PHYMOD_INTF_MODES_TC_442){
                    *start_lane  = 8;
                    *end_lane    = 9;
                    *int_core_lane_mask = 0x03;
                }
                *line_start_lane = -1;
                *line_end_lane = -1;
            }
        }
    }
exit:
    SOC_FUNC_RETURN;
}

int pm4x10_port_phy_lane_access_get(int unit, int port, pm_info_t pm_info,
                                     const portmod_access_get_params_t* params,
                                     int max_phys,
                                     phymod_phy_access_t* phy_access,
                                     int* nof_phys, int* is_most_ext)
{
    int phyn = 0, rv;
    uint32  iphy_lane_mask; 
    int port_index=0;
    int serdes_lane=-1;
    int xphy_lane_mask=0;
    uint32 xphy_id = 0;
    uint32 lane_mask[MAX_NUM_CORES];
    phymod_port_loc_t port_loc[MAX_NUM_CORES];
    int i, is_legacy_phy;
    uint32 xphy_idx;
    uint32 pm_is_bypassed = 0;
    int num_of_phys;
    int done;
    int sys_start_lane=0;
    int sys_end_lane=0;
    int line_start_lane=0;
    int line_end_lane=0;
    int start_lane = 0;
    int end_lane = 0;
    uint32 int_lane_mask = 0xf;
    phymod_core_access_t core_access;


    SOC_INIT_FUNC_DEFS;

    start_lane  = 0;
    end_lane    = PM4X10_LANES_PER_CORE-1;

    if(max_phys > MAX_NUM_CORES)
    {
        _SOC_EXIT_WITH_ERR(SOC_E_PARAM,
               (_SOC_MSG("max_phys parameter exceeded the MAX value. max_phys=%d, max allowed %d."),
                       max_phys, MAX_NUM_CORES));
    }

    /* get port index and lane mask */
    _SOC_IF_ERR_EXIT(_pm4x10_port_index_get(unit, port, pm_info, &port_index,
                                            &iphy_lane_mask));

    _SOC_IF_ERR_EXIT(_pm4x10_port_tricore_lane_info_get(unit, port, pm_info,
                                                        &sys_start_lane, &sys_end_lane,
                                                        &line_start_lane, &line_end_lane,
                                                        &int_lane_mask));

    rv = SOC_WB_ENGINE_GET_VAR(unit, SOC_WB_ENGINE_PORTMOD, pm_info->wb_vars_ids[isBypassed], &pm_is_bypassed);
    _SOC_IF_ERR_EXIT(rv);

    if(!params->apply_lane_mask){
        int_lane_mask = 0xF;
    }

    if(params->phyn >= PM_4x10_INFO(pm_info)->nof_phys[port_index]){
        _SOC_EXIT_WITH_ERR(SOC_E_PARAM,
               (_SOC_MSG("phyn exceeded. max allowed %d. got %d"),
               PM_4x10_INFO(pm_info)->nof_phys[port_index] - 1, params->phyn));
    }


    for( i = 0 ; i < max_phys; i++) {
        lane_mask[i] = 0;
        _SOC_IF_ERR_EXIT(phymod_phy_access_t_init(&phy_access[i]));
    }

    phyn = params->phyn;

    /* if phyn is -1, it is looking for outer most phy.
       assumption is that all lane has same phy depth. */
    if(phyn < 0) {
        phyn = PM_4x10_INFO(pm_info)->nof_phys[port_index] - 1;
    }

    if( phyn == 0 ) {
        /* internal core */
        sal_memcpy (&phy_access[0], &(PM_4x10_INFO(pm_info)->int_core_access),
                    sizeof(phymod_phy_access_t));
        phy_access[0].access.lane_mask = iphy_lane_mask;
        if (params->lane != -1) {

            start_lane  = sys_start_lane;
            end_lane    = sys_end_lane;

            if((params->lane >= start_lane) && (params->lane <= end_lane)) { 
                serdes_lane = port_index + params->lane-start_lane;
                phy_access[0].access.lane_mask &= (0x1 << serdes_lane );
                *nof_phys = 1;
            } else {
                phy_access[0].access.lane_mask &= 0; 
                *nof_phys = 0;
            }
        } else {
            *nof_phys = 1;
            phy_access[0].access.lane_mask &= int_lane_mask;
        }
        phy_access[0].port_loc = phymodPortLocLine; /* only line is availabe for internal. */

        /* PCS bypass flag is set when the underlying device is DNX */
        if ((pm_is_bypassed) && (!PM_4x10_INFO(pm_info)->in_pm12x10)) {
            PHYMOD_DEVICE_OP_MODE_PCS_BYPASS_SET(phy_access[0].device_op_mode);
        }

        /* if it is warm boot, get probed information from wb db instead of re-probing. */
        if(SOC_WARM_BOOT(unit)) {
            rv = SOC_WB_ENGINE_GET_VAR(unit, SOC_WB_ENGINE_PORTMOD, pm_info->wb_vars_ids[phy_type], &(phy_access[0].type));
            _SOC_IF_ERR_EXIT(rv);
        }
    } else {

        /* external phy */
        num_of_phys = 0;

        /* xphy idx is 1 less than phyn since internal core does not count */    
        xphy_idx = phyn-1;
    
        if (params->lane != -1){ /* specific lane */

            if(params->sys_side == PORTMOD_SIDE_SYSTEM){
                start_lane = sys_start_lane;
                end_lane    = sys_end_lane;
            } else { /* PORTMOD_SIDE_LINE */
                start_lane = line_start_lane;
                end_lane    = line_end_lane;
            }

            if((params->lane >= start_lane) && (params->lane <= end_lane)) { 
                serdes_lane = port_index + params->lane-start_lane;
            } else {
                serdes_lane = -1; /* no matching lane in this core. */
                *nof_phys = 0;
            }
            if( serdes_lane != -1){    
                xphy_id = PM_4x10_INFO(pm_info)->lane_conn[xphy_idx][serdes_lane].xphy_id;

                if(xphy_id != PORTMOD_XPHY_ID_INVALID){
                    _SOC_IF_ERR_EXIT ( portmod_xphy_core_access_get(unit, xphy_id, &core_access, &is_legacy_phy));
                    sal_memcpy (&phy_access[num_of_phys], &core_access,
                                sizeof(phymod_phy_access_t));
                    xphy_lane_mask = ( params->sys_side == PORTMOD_SIDE_SYSTEM)? PM_4x10_INFO(pm_info)->lane_conn[xphy_idx][serdes_lane].ss_lane_mask:
                                  PM_4x10_INFO(pm_info)->lane_conn[xphy_idx][serdes_lane].ls_lane_mask;
                    if( xphy_lane_mask != -1 ){
                        phy_access[0].access.lane_mask = xphy_lane_mask;
                    }
                    phy_access[0].port_loc = ( params->sys_side == PORTMOD_SIDE_SYSTEM) ?
                                               phymodPortLocSys : phymodPortLocLine;
                    *nof_phys = 1;
                }
            }
        } else { /* all lanes */
            /* go thru all the lanes related to this port. */
            serdes_lane = 0;

            for (serdes_lane = 0; serdes_lane < MAX_PORTS_PER_PM4X10; serdes_lane++) {
                if (iphy_lane_mask & (1U<<serdes_lane)) {
                    xphy_id = PM_4x10_INFO(pm_info)->lane_conn[xphy_idx][serdes_lane].xphy_id;
                    if(xphy_id != PORTMOD_XPHY_ID_INVALID){
                        _SOC_IF_ERR_EXIT(portmod_xphy_core_access_get(unit, xphy_id, &core_access, &is_legacy_phy));
                        if(num_of_phys == 0 ) {/* first one */
                            sal_memcpy (&phy_access[num_of_phys], &core_access,
                                        sizeof(phymod_phy_access_t));
                            xphy_lane_mask = ( params->sys_side == PORTMOD_SIDE_SYSTEM)?
                                        PM_4x10_INFO(pm_info)->lane_conn[xphy_idx][serdes_lane].ss_lane_mask:
                                        PM_4x10_INFO(pm_info)->lane_conn[xphy_idx][serdes_lane].ls_lane_mask;
                            if( xphy_lane_mask != -1) {
                                lane_mask[num_of_phys] |= xphy_lane_mask;
                            }
                                port_loc[num_of_phys] = ( params->sys_side == PORTMOD_SIDE_SYSTEM) ? 
                                                                phymodPortLocSys : phymodPortLocLine;
                                num_of_phys++;
                        } else {
                            /* if not the first one, see if this one sharing core with other 
                               lanes.  */
                            done = 0;

                            /* Skip if the lane is used by an inactive port marked with :i in the config
                             *  * property.
                             */
                            if (SOC_PBMP_MEMBER(SOC_PORT_DISABLED_BITMAP(unit,all), port+serdes_lane)){
                                continue;
                            }

                            for( i = 0 ; (i < num_of_phys) &&(!done) ; i++) {
                                 if((!sal_memcmp (&phy_access[i], &core_access,
                                                   sizeof(phymod_phy_access_t)))) { /* found a match */
                                    /* update lane */
                                    xphy_lane_mask = ( params->sys_side == PORTMOD_SIDE_SYSTEM)?
                                                PM_4x10_INFO(pm_info)->lane_conn[xphy_idx][serdes_lane].ss_lane_mask:
                                                PM_4x10_INFO(pm_info)->lane_conn[xphy_idx][serdes_lane].ls_lane_mask;
                                    if( xphy_lane_mask != -1) {
                                        lane_mask[i] |= xphy_lane_mask;
                                    }
                                    done = 1;
                                }
                            }
                            if((!done) && (num_of_phys < max_phys)){
                                sal_memcpy (&phy_access[num_of_phys], &core_access,
                                sizeof(phymod_phy_access_t));
                                xphy_lane_mask = ( params->sys_side == PORTMOD_SIDE_SYSTEM)?
                                            PM_4x10_INFO(pm_info)->lane_conn[xphy_idx][serdes_lane].ss_lane_mask:
                                            PM_4x10_INFO(pm_info)->lane_conn[xphy_idx][serdes_lane].ls_lane_mask;
                                if( xphy_lane_mask != -1) {
                                    lane_mask[num_of_phys] = xphy_lane_mask;
                                }
                                    port_loc[num_of_phys] = ( params->sys_side == PORTMOD_SIDE_SYSTEM)?
                                                                    phymodPortLocSys : phymodPortLocLine ;
                                    num_of_phys++;
                            } 
                        }
                    }
                }
            }
            *nof_phys = num_of_phys;
            for( i = 0 ; (i < *nof_phys) ; i++) {
                phy_access[i].access.lane_mask = lane_mask[i];
                phy_access[i].port_loc = port_loc[i];
            }
        }
    } /* ext phys */ 
       
    /* assumption is that all logical port have same phy depths, that 
       will not true when simplex introduced , until then we can keep 
       this code. */
    if (is_most_ext) {
        if (phyn == PM_4x10_INFO(pm_info)->nof_phys[port_index]-1) {
            *is_most_ext = 1;
        } else {
            *is_most_ext = 0;
        }
    }
 
exit:
    SOC_FUNC_RETURN;
}

STATIC 
int _pm4x10_core_probe ( int unit, int port, pm_info_t pm_info, phymod_core_access_t* core_access, int *probe, int is_probe_for_legacy) 
{
    int is_phymod_probed=0, xphy_id;
    uint32 is_identify=0;
    soc_error_t rv = SOC_E_NONE;
    SOC_INIT_FUNC_DEFS;

    *probe = 0;

    if (core_access->type == phymodDispatchTypeCount)
    {
        rv = phymod_core_probe( &(core_access->access), &(core_access->type), &is_phymod_probed);
        if(SOC_FAILURE(rv)) {
            /* restore the old value */
            core_access->type = phymodDispatchTypeCount;
            _SOC_IF_ERR_EXIT(rv);
        }

        if (!is_phymod_probed)
        {
            /* Reset type */
            core_access->type = phymodDispatchTypeCount;

            if (is_probe_for_legacy)
            {
                if (portmod_port_legacy_phy_probe(unit, port))
                {
                    xphy_id = core_access->access.addr;
                    _SOC_IF_ERR_EXIT(portmod_xphy_is_legacy_phy_set(unit, xphy_id, 1));
                    *probe = 1;
                }
            }
        }
        else
        {
            *probe = 1;
        }
    }

    if (core_access->type == phymodDispatchTypeQuadra28) {
        PHYMOD_ACC_F_PRECONDITION_SET(&core_access->access);
        SOC_IF_ERROR_RETURN(phymod_core_identify(core_access, 0, &is_identify));
        PHYMOD_ACC_F_PRECONDITION_CLR(&core_access->access);
    }

exit:
    /* In case that the call to phymod_core_probe hasn't failed and the returned parameter is_phymod_probed is true - 
     * We are interested in changing the value of core_access->access
     */
    /* coverity[end_of_path] */
    SOC_FUNC_RETURN;
}

int pm4x10_port_mac_rsv_mask_set(int unit, int port, pm_info_t pm_info, uint32 rsv_mask)
{
        return(WRITE_XLPORT_MAC_RSV_MASKr(unit, port, rsv_mask)); 
}

STATIC
int _pm4x10_pm_port_init(int unit, int port, int phy_acc, int internal_port,
                         const portmod_port_add_info_t* add_info, int enable, pm_info_t pm_info)
{
    int rv;
    uint32 reg_val, flags;
    soc_field_t port_fields[] = {PORT0f, PORT1f, PORT2f, PORT3f};
    uint32 rsv_mask;
    SOC_INIT_FUNC_DEFS;

    if(enable) {
        /* RSV Mask */
        rsv_mask = 0;
        SHR_BITSET(&rsv_mask, 3); /* Receive terminate/code error */
        SHR_BITSET(&rsv_mask, 4); /* CRC error */
        SHR_BITSET(&rsv_mask, 6); /* PGW_MACTruncated/Frame out of Range */
        SHR_BITSET(&rsv_mask, 17); /* RUNT detected*/

         rv = __portmod__dispatch__[pm_info->type]->f_portmod_port_mac_rsv_mask_set(unit, port, pm_info, rsv_mask);
        _SOC_IF_ERR_EXIT(rv);


        /* Soft reset */
        _SOC_IF_ERR_EXIT(READ_XLPORT_SOFT_RESETr(unit, phy_acc, &reg_val));
        soc_reg_field_set(unit, XLPORT_SOFT_RESETr, &reg_val, 
                                  port_fields[internal_port], 1);
        _SOC_IF_ERR_EXIT(WRITE_XLPORT_SOFT_RESETr(unit, phy_acc, reg_val));

        soc_reg_field_set(unit, XLPORT_SOFT_RESETr, &reg_val, 
                                  port_fields[internal_port], 0);
        _SOC_IF_ERR_EXIT(WRITE_XLPORT_SOFT_RESETr(unit, phy_acc, reg_val));

        
        /* Port enable */
        _SOC_IF_ERR_EXIT(READ_XLPORT_ENABLE_REGr(unit, phy_acc, &reg_val));
        soc_reg_field_set(unit, XLPORT_ENABLE_REGr, &reg_val, 
                                   port_fields[internal_port], 1);
        _SOC_IF_ERR_EXIT(WRITE_XLPORT_ENABLE_REGr(unit, phy_acc, reg_val));

        /* Init MAC */
        flags = 0;
        if(PORTMOD_PORT_ADD_F_RX_SRIP_CRC_GET(add_info)) {
            flags |= XLMAC_INIT_F_RX_STRIP_CRC;
        }

        if(PORTMOD_PORT_ADD_F_TX_APPEND_CRC_GET(add_info)) {
            flags |= XLMAC_INIT_F_TX_APPEND_CRC;
        }

        if(PORTMOD_PORT_ADD_F_TX_REPLACE_CRC_GET(add_info)) {
            flags |= XLMAC_INIT_F_TX_REPLACE_CRC;
        }

        if(PORTMOD_PORT_ADD_F_TX_PASS_THROUGH_CRC_GET(add_info)) {
            flags |= XLMAC_INIT_F_TX_PASS_THROUGH_CRC_MODE;
        }

        if (IS_HG_PORT(unit,port)){
            flags |= XLMAC_INIT_F_IPG_CHECK_DISABLE;
        } else {
            flags &= ~XLMAC_INIT_F_IPG_CHECK_DISABLE;
        }

        if(PHYMOD_INTF_MODES_HIGIG_GET(&(add_info->interface_config))) {
            flags |= XLMAC_INIT_F_IS_HIGIG;
        }
        rv = xlmac_init(unit, port, flags);
        _SOC_IF_ERR_EXIT(rv);

        /* LSS */
        _SOC_IF_ERR_EXIT(READ_XLPORT_FAULT_LINK_STATUSr(unit, phy_acc, &reg_val));
        soc_reg_field_set(unit, XLPORT_FAULT_LINK_STATUSr, &reg_val,
                          REMOTE_FAULTf, 1);
        soc_reg_field_set(unit, XLPORT_FAULT_LINK_STATUSr, &reg_val,
                          LOCAL_FAULTf, 1);
        _SOC_IF_ERR_EXIT(WRITE_XLPORT_FAULT_LINK_STATUSr(unit, phy_acc, reg_val)); 

        /* Counter MAX size */
        rv = __portmod__dispatch__[pm_info->type]->f_portmod_port_cntmaxsize_set(unit, port, pm_info, 1518);
       _SOC_IF_ERR_EXIT(rv);
 
        /* Reset MIB counters */
        rv = __portmod__dispatch__[pm_info->type]->f_portmod_port_mib_reset_toggle(unit, port, pm_info, internal_port);
       _SOC_IF_ERR_EXIT(rv);

    } else {
        /* Port disable */
        _SOC_IF_ERR_EXIT(READ_XLPORT_ENABLE_REGr(unit, phy_acc, &reg_val));
        soc_reg_field_set (unit, XLPORT_ENABLE_REGr, &reg_val, 
                           port_fields[internal_port], 0);
        _SOC_IF_ERR_EXIT(WRITE_XLPORT_ENABLE_REGr(unit, phy_acc, reg_val));

         /* Soft reset */
        _SOC_IF_ERR_EXIT(READ_XLPORT_SOFT_RESETr(unit, phy_acc, &reg_val));
        soc_reg_field_set(unit, XLPORT_SOFT_RESETr, &reg_val, 
                          port_fields[internal_port], 1);
        _SOC_IF_ERR_EXIT(WRITE_XLPORT_SOFT_RESETr(unit, phy_acc, reg_val));
    }

exit:
    SOC_FUNC_RETURN;
}

STATIC
int _pm4x10_ext_phy_interface_check(int unit, int port, phymod_phy_access_t *phy_acc,
                               const portmod_port_interface_config_t *interface_config,
                               soc_port_if_t interface, int *is_valid)
{
    int rv = 0;
    int num_lanes = 0, data_rate = 0;
    uint32_t interface_mode = 0;
    phymod_interface_t  phymod_intf;
    int is_legacy_phy = 0, xphy_id;
    SOC_INIT_FUNC_DEFS;

    num_lanes      = interface_config->port_num_lanes;
    interface_mode = interface_config->interface_modes;
    data_rate      = interface_config->speed;

    xphy_id = phy_acc->access.addr;
    _SOC_IF_ERR_EXIT(portmod_xphy_is_legacy_phy_get(unit, xphy_id, &is_legacy_phy));

    if (is_legacy_phy) {
        /* if its a legacy PHY let the PHY driver decide the valid interface types */
        *is_valid = 1;
        rv = SOC_E_NONE;
        SOC_EXIT;
    }

    /* convert portmod interface to phymod interface */
    rv = portmod_intf_to_phymod_intf(unit, data_rate, interface, &phymod_intf);
    _SOC_IF_ERR_EXIT(rv); 

    rv = portmod_port_line_interface_is_valid(unit, phy_acc, port, data_rate, phymod_intf,
                                            num_lanes, interface_mode, is_valid);
    _SOC_IF_ERR_EXIT(rv);  
  
    if (!*is_valid) {
        LOG_ERROR(BSL_LS_BCM_PORT,
              (BSL_META_UP(unit, port,
               "ERROR: u=%d p=%d interface type %d not supported by external PHY for this speed %d \n"),
               unit, port, interface, data_rate));
        rv = SOC_E_PARAM;
        _SOC_IF_ERR_EXIT(rv); 
    }

exit:
    SOC_FUNC_RETURN;
}

STATIC
int _pm4x10_interface_check(int unit, int port, pm_info_t pm_info,
                            const portmod_port_interface_config_t *interface_config,
                            soc_port_if_t interface, int *is_valid)
{
    int is_higig;
    int num_lanes;

    *is_valid = 0;
    num_lanes = interface_config->port_num_lanes;
    is_higig = ((interface_config->encap_mode == SOC_ENCAP_HIGIG2) ||
                 (interface_config->encap_mode == SOC_ENCAP_HIGIG) ||
                 PHYMOD_INTF_MODES_HIGIG_GET(interface_config)) ? 1 : 0;

    if (SOC_PORT_IF_ILKN == interface) {
        *is_valid = 1;
        return SOC_E_NONE;
    }

    switch (interface_config->speed) {
        case 0:
            /* When AN=1 the input speed to portmod may be 0. At
             * this moment the speed is still undecided so there
             * is no reason to invalidate the interface type.
             */
            *is_valid = 1;
            break;
        case 10:
            if ((SOC_PORT_IF_SGMII == interface) ||
                (SOC_PORT_IF_GMII == interface)) {
                *is_valid = 1;
            }
            break;
        case 100:
            if ((SOC_PORT_IF_SGMII == interface) ||
                (SOC_PORT_IF_GMII == interface)) {
                *is_valid = 1;
            }
            break;
        case 1000:
            if ((SOC_PORT_IF_SGMII == interface) ||
                (SOC_PORT_IF_GMII == interface) ||
                (SOC_PORT_IF_KX == interface) ||
                (SOC_PORT_IF_CX == interface) ||
                ((SOC_PORT_IF_QSGMII == interface) &&
                (PM_4x10_INFO(pm_info)->is_pm4x10q))) {
                *is_valid = 1;
            }
            break;
        case 2500:
            if ((SOC_PORT_IF_SGMII == interface) ||
                (SOC_PORT_IF_GMII == interface)) {
                *is_valid = 1;
            }
            break;
        case 5000:
            if (num_lanes == 1) {
                if ((SOC_PORT_IF_XFI == interface) ||
                    (SOC_PORT_IF_KR == interface) ||
                    (SOC_PORT_IF_GMII == interface)) {
                    *is_valid = 1;
                }
            } else if (num_lanes == 2) {
                if (SOC_PORT_IF_RXAUI == interface) {
                    *is_valid = 1;
                }
            }
            break;
        case 10000:
        case 11000:
            if (is_higig) {
                if (num_lanes == 1) {
                    if ((SOC_PORT_IF_XFI == interface) ||
                        (SOC_PORT_IF_SFI == interface) ||
                        (SOC_PORT_IF_KR == interface) ||
                        (SOC_PORT_IF_CR == interface) ||
                        (SOC_PORT_IF_SR == interface)) {
                        *is_valid = 1;
                    }
                } else if (num_lanes == 2) {
                    if (SOC_PORT_IF_RXAUI == interface) {
                        *is_valid = 1;
                    }
                } else if (num_lanes == 4) {
                    if ((SOC_PORT_IF_XGMII == interface) ||
                        (SOC_PORT_IF_XLAUI == interface)) {
                        *is_valid = 1;
                    }
                }
            } else {
                if (num_lanes == 1) {
                    if ((SOC_PORT_IF_SFI == interface) ||
                        (SOC_PORT_IF_XFI == interface) ||
                        (SOC_PORT_IF_KR == interface) ||
                        (SOC_PORT_IF_CR == interface) ||
                        (SOC_PORT_IF_SR == interface) ||
                        (SOC_PORT_IF_LR == interface) ||
                        (SOC_PORT_IF_ER == interface) ||
                        (SOC_PORT_IF_CX == interface) ||
                        (SOC_PORT_IF_LRM == interface)) {
                        *is_valid = 1;
                    }
                } else if (num_lanes == 2) {
                    if (SOC_PORT_IF_RXAUI == interface) {
                        *is_valid = 1;
                    }
                } else if (num_lanes == 4) {
                    if ((SOC_PORT_IF_XGMII == interface) ||
                        (SOC_PORT_IF_XLAUI == interface) ||
                        (SOC_PORT_IF_DNX_XAUI == interface) ||
                        (SOC_PORT_IF_XAUI == interface) ||
                        (SOC_PORT_IF_SFI == interface) ||
                        (SOC_PORT_IF_XFI == interface) ||
                        (SOC_PORT_IF_QSGMII == interface)) {
                        *is_valid = 1;
                    }
                }
            }
            break;
        case 3125:
        case 5750:
        case 6250:
        case 8125:
        case 8500:
        case 10312:
        case 10937:
        case 11250:
            if (SOC_PORT_IF_ILKN == interface) {
                /*
                 * COVERITY
                 * It is kept intentionally as a defensive check.
                 */
                /* coverity[dead_error_line] */
                *is_valid = 1;
            }
            break;
        case 10500:
        case 12500:
            break;
        case 11500:
            if (SOC_PORT_IF_XFI == interface) {
                *is_valid = 1;
            }
            break;
        case 12000:
            if ((num_lanes == 4) &&
                (SOC_PORT_IF_XGMII == interface)) {
                *is_valid = 1;
            } else if ((num_lanes == 2) &&
                (SOC_PORT_IF_RXAUI == interface)) {
                *is_valid = 1;
            } else if ((num_lanes == 1) &&
                ((SOC_PORT_IF_XFI == interface) ||
                 (SOC_PORT_IF_SFI == interface) ||
                 (SOC_PORT_IF_SR == interface) ||
                 (SOC_PORT_IF_LR == interface) ||
                 (SOC_PORT_IF_CR == interface) ||
                 (SOC_PORT_IF_KR == interface))) {
                *is_valid = 1;
            }
            break;
        case 12773:   /*  12733Mbps */
            if (SOC_PORT_IF_RXAUI == interface) {
                *is_valid = 1;
            }
            break;
        case 13000:
            if ((num_lanes == 4) &&
                (SOC_PORT_IF_XGMII == interface)) {
                *is_valid = 1;
            } else if ((num_lanes == 2) &&
                (SOC_PORT_IF_RXAUI == interface)) {
                *is_valid = 1;
            }
            break;
        case 15000:
            if ((num_lanes == 4) &&
                (SOC_PORT_IF_XGMII == interface)) {
                *is_valid = 1;
            } else if ((num_lanes == 2) &&
                (SOC_PORT_IF_RXAUI == interface)) {
                *is_valid = 1;
            }
            break;
        case 15750:
        case 16000:  /* setting RX66_CONTROL_CC_EN/CC_DATA_SEL failed this 16000 HI_DXGXS mode */
            if ((num_lanes == 4) &&
                (SOC_PORT_IF_XGMII == interface)) {
                *is_valid = 1;
            } else if ((num_lanes == 2) &&
                (SOC_PORT_IF_RXAUI == interface)) {
                *is_valid = 1;
            }
            break;
        case 20000:
        case 21000:
            if (is_higig) {
                if (num_lanes == 2) {
                    if ((SOC_PORT_IF_KR2 == interface) ||
                        (SOC_PORT_IF_CR2 == interface) ||
                        (SOC_PORT_IF_RXAUI == interface)) {
                        *is_valid = 1;
                    }
                } else if (num_lanes == 4) {
                    if (SOC_PORT_IF_XGMII == interface) {
                        *is_valid = 1;
                    }
                }
            } else {
                if (num_lanes == 2) {
                    if ((SOC_PORT_IF_KR2 == interface) ||
                        (SOC_PORT_IF_CR2 == interface) ||
                        (SOC_PORT_IF_RXAUI == interface)) {
                        *is_valid = 1;
                    }
                } else if (num_lanes == 4) {
                    if ((SOC_PORT_IF_XGMII == interface) ||
                        (SOC_PORT_IF_SR4 == interface) ||
                        (SOC_PORT_IF_CR4 == interface) ||
                        (SOC_PORT_IF_KR4 == interface) ||
                        (SOC_PORT_IF_DNX_XAUI == interface)) {
                        *is_valid = 1;
                    }
                }
            }
            break;
        case 23000:
            if (num_lanes == 2) {
                if (SOC_PORT_IF_RXAUI == interface) {
                    *is_valid = 1;
                }
            }
            break;
        case 25000:
        case 25450:
        case 25455:
        case 30000:
        case 31500:
            if (num_lanes == 4) {
                if ((SOC_PORT_IF_XGMII == interface) ||
                    (SOC_PORT_IF_KR4 == interface)) {
                    *is_valid = 1;
                }
            } else if (num_lanes == 2) {
                if (SOC_PORT_IF_KR2 == interface) {
                    *is_valid = 1;
                }
            } else {
                if (SOC_PORT_IF_KR == interface) {
                    *is_valid = 1;
                }
            }
            break;
        case 40000:
            if (is_higig) {
                if ((SOC_PORT_IF_KR4 == interface) ||
                    (SOC_PORT_IF_CR4 == interface) ||
                    (SOC_PORT_IF_XGMII == interface)) {
                    *is_valid = 1;
                }
            } else {
                if ((SOC_PORT_IF_KR4 == interface) ||
                    (SOC_PORT_IF_CR4 == interface) ||
                    (SOC_PORT_IF_SR4 == interface) ||
                    (SOC_PORT_IF_LR4 == interface) ||
                    (SOC_PORT_IF_ER4 == interface) ||
                    (SOC_PORT_IF_XLAUI == interface) ||
                    (SOC_PORT_IF_XLPPI == interface)) {
                    *is_valid = 1;
                }
            }
            break;
        case 42000:
            if (is_higig) {
                if (num_lanes == 4) {
                    if ((SOC_PORT_IF_KR4 == interface) ||
                        (SOC_PORT_IF_CR4 == interface) ||
                        (SOC_PORT_IF_XLAUI == interface) ||
                        (SOC_PORT_IF_SR4 == interface) ||
                        (SOC_PORT_IF_XGMII == interface)) {
                        *is_valid = 1;
                    }
                }
            } else {
                if (num_lanes == 4) {
                    if ((SOC_PORT_IF_KR4 == interface) ||
                        (SOC_PORT_IF_XLAUI == interface)) {
                        *is_valid = 1;
                    }
                }
            }
            break;
        case 48000:
            if (!is_higig && num_lanes == 4) {
                if (SOC_PORT_IF_XLAUI == interface) {
                    *is_valid = 1;
                }
            }
            break;
        case 100000:
            if ((SOC_PORT_IF_CR10 == interface) ||
                (SOC_PORT_IF_KR10 == interface) ||
                (SOC_PORT_IF_LR10 == interface) ||
                (SOC_PORT_IF_SR10 == interface) ||
                (SOC_PORT_IF_CAUI == interface)) {
                *is_valid = 1;
            }
            break;
        case 106000:
            if ((SOC_PORT_IF_CR10 == interface) ||
                (SOC_PORT_IF_KR10 == interface) ||
                (SOC_PORT_IF_CAUI == interface)) {
                *is_valid = 1;
            }
            break;
        case 120000:
            if (SOC_PORT_IF_CAUI == interface) {
                *is_valid = 1;
            }
            break;
        case 127000:
            if (SOC_PORT_IF_CAUI == interface) {
                *is_valid = 1;
            }
            break;
        default:
            break;
    }

    return SOC_E_NONE;
}

STATIC
int _pm4x10_default_interface_get(int unit, int port,
                               const portmod_port_interface_config_t *interface_config,
                               soc_port_if_t *interface)
{
    int is_higig;
    int num_lanes;
    soc_port_if_t interface_tmp = SOC_PORT_IF_NULL;
    SOC_INIT_FUNC_DEFS;

    *interface = SOC_PORT_IF_NULL;
    num_lanes = interface_config->port_num_lanes;
    is_higig = ((interface_config->encap_mode == SOC_ENCAP_HIGIG2) ||
                 (interface_config->encap_mode == SOC_ENCAP_HIGIG)) ? 1 : 0;
    _SOC_IF_ERR_EXIT(portmod_line_intf_from_config_get(interface_config, &interface_tmp));

    switch (interface_config->speed) {
        case 10:
            *interface = SOC_PORT_IF_SGMII;
            break;
        case 100:
            *interface = SOC_PORT_IF_SGMII;
            break;
        case 1000:
            if (PHYMOD_INTF_MODES_FIBER_GET(interface_config)) {
                *interface = SOC_PORT_IF_GMII;
            } else if(interface_tmp == SOC_PORT_IF_KX) {
                *interface = SOC_PORT_IF_KX;
            } else {
                *interface = SOC_PORT_IF_SGMII;
            }
            break;
        case 2500:
            *interface = PHYMOD_INTF_MODES_FIBER_GET(interface_config)?
                                        SOC_PORT_IF_GMII: SOC_PORT_IF_SGMII;
            break;
        case 5000:
            if (num_lanes == 1) {
                *interface = SOC_PORT_IF_XFI;
            } else if (num_lanes == 2) {
                *interface = SOC_PORT_IF_RXAUI;
            } else {
                *interface = SOC_PORT_IF_XFI;
            }
            break;
        case 10000:
        case 11000:
            if (num_lanes == 2) {
                *interface = SOC_PORT_IF_RXAUI;
            } else if (num_lanes == 4) {
                *interface = SOC_PORT_IF_XGMII;
            } else {
                *interface = PHYMOD_INTF_MODES_FIBER_GET(interface_config)?
                                    SOC_PORT_IF_SFI : SOC_PORT_IF_XFI;
            }
            break;
        case 3125:
        case 5750:
        case 6250:
        case 8125:
        case 8500:
        case 10312:
        case 10937:
        case 11250:
            *interface = SOC_PORT_IF_ILKN;
            break;
        case 10500:
        case 12500:
            break;
        case 20000:
        case 21000:
            if (num_lanes == 4) {
                *interface = SOC_PORT_IF_XGMII;
            } else {
                *interface = SOC_PORT_IF_RXAUI;
            }
            break;
        case 23000:
              *interface = SOC_PORT_IF_RXAUI;
            break;
        case 25000:
        case 25450:
        case 25455:
        case 30000:
        case 31500:
            if (num_lanes == 4) {
                *interface = SOC_PORT_IF_KR4;
            } else if (num_lanes == 2) {
                *interface = SOC_PORT_IF_KR2;
            } else {
                *interface = SOC_PORT_IF_KR;
            }
            break;
        case 40000:
            if (is_higig) {
                *interface = SOC_PORT_IF_XGMII;
            } else {
                *interface = SOC_PORT_IF_XLAUI;
            }
            break;
        case 42000:
            if (is_higig) {
                *interface = SOC_PORT_IF_XGMII;
            } else {
                *interface = SOC_PORT_IF_KR4;
            }
            break;
        case 100000:
            *interface = SOC_PORT_IF_CAUI;
            break;
        case 106000:
            *interface = SOC_PORT_IF_CAUI;
            break;
        case 120000:
            *interface = SOC_PORT_IF_CAUI;
            break;
        case 127000:
            *interface = SOC_PORT_IF_CAUI;
            break;
        default:
            if (num_lanes == 1) {
                if (interface_config->speed >= 10000) {
                    *interface = SOC_PORT_IF_CR;
                } else {
                    *interface = SOC_PORT_IF_SGMII;
                }
            } else if (num_lanes == 2) {
                if (is_higig) {
                    *interface = SOC_PORT_IF_RXAUI;
                } else {
                    *interface = SOC_PORT_IF_CR2;
                }
            } else if (num_lanes == 4) {
                if (interface_config->speed >= 40000) {
                    if (is_higig){
                        *interface = SOC_PORT_IF_CR4;
                    } else{
                        *interface = SOC_PORT_IF_XLAUI;
                    }
                } else {
                    *interface = SOC_PORT_IF_XGMII;
                }
            } else if (num_lanes == 10) {
                    *interface = SOC_PORT_IF_CAUI;
            } else if (num_lanes == 12) {
                    *interface = SOC_PORT_IF_CAUI;
            } else {
                *interface = SOC_PORT_IF_ILKN;
            }
            break;
    }

exit:
    SOC_FUNC_RETURN;
}

STATIC
int _pm4x10_line_side_phymod_interface_get(int unit, int port,
                                pm_info_t pm_info, int rxaui_mode,
                                const portmod_port_interface_config_t *config,
                                phymod_interface_t *interface_type)
{
    int port_index = 0;
    int is_valid = 0, nof_phys = 0;
    uint32 bitmap;
    soc_port_if_t interface = SOC_PORT_IF_NULL;
    phymod_phy_access_t phy_access_arr[1+MAX_PHYN];
    SOC_INIT_FUNC_DEFS;

    _SOC_IF_ERR_EXIT(_pm4x10_port_index_get(unit, port, pm_info, &port_index,
                                            &bitmap));

    _SOC_IF_ERR_EXIT
            (portmod_port_chain_phy_access_get(unit, port, pm_info,
                                               phy_access_arr ,(1+MAX_PHYN),
                                               &nof_phys));

    _SOC_IF_ERR_EXIT(portmod_line_intf_from_config_get(config, &interface));

    if (nof_phys > 1) {
        /* EXT PHY interface */
        _SOC_IF_ERR_EXIT
            (_pm4x10_ext_phy_interface_check(unit, port, &phy_access_arr[nof_phys-1], config, interface,
                                                 &is_valid));
    } else if (nof_phys == 1) {
        _SOC_IF_ERR_EXIT(_pm4x10_interface_check(unit, port, pm_info, config, interface, &is_valid));
    }
    
    if (!is_valid) {
        /* interface check validation for internal sedes failed */ 
        LOG_ERROR(BSL_LS_BCM_PORT,
              (BSL_META_UP(unit, port,
               "ERROR: u=%d p=%d interface type %d not supported by internal SERDES for this speed %d \n"),
               unit, port, interface, config->speed));
        _SOC_IF_ERR_EXIT(SOC_E_PARAM);    
    }

    
    _SOC_IF_ERR_EXIT(portmod_intf_to_phymod_intf(unit, config->speed,
                                                 interface, interface_type));
    if ((config->speed == 10000) && (config->port_num_lanes == 2)) {
        if (rxaui_mode == 0x2) {
            *interface_type = phymodInterfaceRXAUI;
        }
        else if (rxaui_mode) {
            *interface_type = phymodInterfaceX2;
        } else {
            *interface_type = phymodInterfaceRXAUI;
        }
    }

exit:
    SOC_FUNC_RETURN;
}

int _pm4x10_phymod_interface_mode_get(int unit, int port,pm_info_t pm_info,
                                   const portmod_port_interface_config_t *config,
                                   phymod_phy_inf_config_t *phy_config)
{
    int port_index = 0;
    uint32 bitmap;
    soc_port_if_t interface = SOC_PORT_IF_NULL;
    SOC_INIT_FUNC_DEFS;

    _SOC_IF_ERR_EXIT(_pm4x10_port_index_get(unit, port, pm_info, &port_index,
                                            &bitmap));

    _SOC_IF_ERR_EXIT(PM4x10_INTERFACE_TYPE_GET(unit, pm_info, &interface,
                                               port_index));
    phy_config->interface_modes = config->interface_modes;

    switch (config->speed) {
        case 6250:
            if (PHYMOD_INTF_MODES_OS2_GET(config)) {/* for dual rate OS2 is required */
                PHYMOD_INTF_MODES_OS2_SET(phy_config);
            }
            break;
        case 10000:
            if (config->port_num_lanes == 2) {
                if (PHYMOD_INTF_MODES_SCR_GET(config)) {
                    PHYMOD_INTF_MODES_SCR_SET(phy_config);
                }
            }
            break;
        case 20000:
            if (config->port_num_lanes == 4) {
                if (PHYMOD_INTF_MODES_SCR_GET(config)) {
                    PHYMOD_INTF_MODES_SCR_SET(phy_config);
                }
            }
            break;
        case 100000:
        case 106000:
        case 107000:
        case 120000:
        case 127000:
          if ((phy_config->interface_type == phymodInterfaceSR10) ||
              (phy_config->interface_type == phymodInterfaceLR10))  {
              PHYMOD_INTF_MODES_FIBER_SET(phy_config);
          }
          break;            
        default:
            break;
    }

    _SOC_IF_ERR_EXIT(portmod_media_type_from_port_intf(unit, interface,
                                                       phy_config));
exit:
    SOC_FUNC_RETURN;
}

/*
*   overwrite default tx params from config file.
*   set phy_init_config in PM.
*/
STATIC
int _pm4x10_port_tx_config_init_set(int unit, pm_info_t pm_info,
                                    int core_start_lane,
                                    int core_end_lane,
                                    phymod_tx_t *tx_params_default,
                                    const phymod_tx_t lane_tx_params_config[],
                                    const int *config_flag, int is_ext_phy)
{
    int i = 0, rv = SOC_E_NONE;
    phymod_tx_t per_lane_tx_params_default[PM4X10_LANES_PER_CORE];
    int start_lane = core_start_lane, lane;
    
    /* if the current core does not belong to a tricore */
    if(core_start_lane==-1){
        start_lane = 0;
    }

    for(i=0; i<PM4X10_LANES_PER_CORE; i++){
        per_lane_tx_params_default[i].pre   = tx_params_default->pre;
        per_lane_tx_params_default[i].main  = tx_params_default->main;
        per_lane_tx_params_default[i].post  = tx_params_default->post;
        per_lane_tx_params_default[i].post2 = tx_params_default->post2;
        per_lane_tx_params_default[i].post3 = tx_params_default->post3;
        per_lane_tx_params_default[i].amp   = tx_params_default->amp;
        per_lane_tx_params_default[i].drivermode   = tx_params_default->drivermode;
    }

    /* overlap preemphasis and amp from config */
    for(i=0; i<PM4X10_LANES_PER_CORE; i++){
        lane = i + start_lane;
        if(config_flag[lane] & PORTMOD_USER_SET_TX_PREEMPHASIS_BY_CONFIG) {
            per_lane_tx_params_default[i].pre  = lane_tx_params_config[lane].pre ;
            per_lane_tx_params_default[i].main = lane_tx_params_config[lane].main ;
            per_lane_tx_params_default[i].post = lane_tx_params_config[lane].post ;
        }
        if(config_flag[lane] & PORTMOD_USER_SET_TX_AMP_BY_CONFIG) {
            per_lane_tx_params_default[i].amp  = lane_tx_params_config[lane].amp ;
        }
    }

    for(i=0 ; i<PM4X10_LANES_PER_CORE ; i++) {
        lane = i + start_lane;
        if(is_ext_phy) {
           rv = PM4x10_EXT_PHY_TX_PARAMS_USER_FLAG_SET(unit, pm_info,
                                                       config_flag[lane], i);
           if (SOC_FAILURE(rv)) return (rv);

           rv = PM4x10_EXT_PHY_TX_PARAMS_SET(unit, pm_info,
                                             per_lane_tx_params_default[i], i);
           if (SOC_FAILURE(rv)) return (rv);
        }
        else {
           rv = PM4x10_TX_PARAMS_USER_FLAG_SET(unit, pm_info,
                                               config_flag[lane], i);
           if (SOC_FAILURE(rv)) return (rv);

           rv = PM4x10_TX_PARAMS_SET(unit, pm_info,
                                     per_lane_tx_params_default[i], i);
           if (SOC_FAILURE(rv)) return (rv);
        }
    }

    return SOC_E_NONE;
}


/*
 *  clear pm_info's phy_interface_config, and set few members of phy_interface_config
 *  initialize phy_init_config tx params and set registers 
*/
int _pm4x10_port_interface_config_init_set(int unit, int port, 
                        pm_info_t pm_info, 
                        const portmod_port_interface_config_t* config,
                        const portmod_port_init_config_t* init_config)
{
    uint32 reg_val, pm_is_bypassed = 0; 
    int    nof_phys = 0, rv = 0, phy_acc, flags = 0;
    int    is_ext_phy = 0, i;
    int    core_start_lane=-1, core_end_lane=-1, line_start_lane=-1, line_end_lane=-1;
    uint32 int_lane_mask=0xf;
    phymod_phy_access_t phy_access[1 + MAX_PHYN];
    phymod_phy_inf_config_t     phy_interface_config;
    phymod_tx_t                 phymod_tx;
    int                         port_index, is_legacy_phy = 0;
    int                         is_higig, rxaui_mode, xphy_id;
    uint32_t                    bitmap;
	phymod_interface_t phymod_interface = phymodInterfaceCount;
    phymod_tx_t          ext_phy_tx_params[PHYMOD_MAX_LANES_PER_CORE];

    SOC_INIT_FUNC_DEFS;

    PM4x10_PHY_ACCESS_GET(unit, port, pm_info, phy_acc);

    _SOC_IF_ERR_EXIT(_pm4x10_port_index_get (unit, port, pm_info, &port_index,
                                             &bitmap));

    phymod_tx_t_init(&phymod_tx);

    is_higig =  ((config->encap_mode == SOC_ENCAP_HIGIG2) ||
                 (config->encap_mode == SOC_ENCAP_HIGIG) ||
                 PHYMOD_INTF_MODES_HIGIG_GET(config)) ? 1 : 0;

    rv = SOC_WB_ENGINE_GET_VAR (unit, SOC_WB_ENGINE_PORTMOD, 
                                pm_info->wb_vars_ids[isBypassed], 
                                &pm_is_bypassed);
    _SOC_IF_ERR_EXIT(rv);
   
    if(!pm_is_bypassed){
        /* set HiG mode */
        _SOC_IF_ERR_EXIT(READ_XLPORT_CONFIGr(unit, phy_acc, &reg_val));
        soc_reg_field_set (unit, XLPORT_CONFIGr, &reg_val, HIGIG2_MODEf, is_higig);
        _SOC_IF_ERR_EXIT(WRITE_XLPORT_CONFIGr(unit, phy_acc, reg_val));
    
        /* set port speed */
        flags = XLMAC_SPEED_SET_FLAGS_SOFT_RESET_DIS;
        rv = xlmac_speed_set(unit, port, flags, config->speed);
        _SOC_IF_ERR_EXIT(rv);

        /* set encapsulation */
        flags = XLMAC_ENCAP_SET_FLAGS_SOFT_RESET_DIS;
        rv = xlmac_encap_set (unit, port, flags, is_higig ?  SOC_ENCAP_HIGIG2 : 
                                                             SOC_ENCAP_IEEE);
        _SOC_IF_ERR_EXIT(rv);

        rv = xlmac_strict_preamble_set(unit, port, (!is_higig && (config->speed >= 10000)));

        _SOC_IF_ERR_EXIT(rv);

    }

    _SOC_IF_ERR_EXIT(portmod_port_chain_phy_access_get(unit, port, pm_info, 
                                                       phy_access ,(1+MAX_PHYN),
                                                       &nof_phys));

    _SOC_IF_ERR_EXIT(PM4x10_RXAUI_MODE_GET(unit, pm_info,
                                           &rxaui_mode, port_index));

    _SOC_IF_ERR_EXIT(phymod_phy_inf_config_t_init (&phy_interface_config));
    _SOC_IF_ERR_EXIT(_pm4x10_line_side_phymod_interface_get(unit, port,
                                                            pm_info, rxaui_mode,
                                                            config,
                                                            &phymod_interface));
    phy_interface_config.interface_type = phymod_interface;
    _SOC_IF_ERR_EXIT(_pm4x10_phymod_interface_mode_get(unit, port,
                                                       pm_info, config,
                                                       &phy_interface_config));

    phy_interface_config.data_rate       = config->speed;
    phy_interface_config.pll_divider_req = config->pll_divider_req;
    phy_interface_config.ref_clock       = PM_4x10_INFO(pm_info)->ref_clk;

    _SOC_IF_ERR_EXIT(PM4x10_SPEED_SET(unit, pm_info,
                                      config->speed, port_index));


    /* find the start_lane and end_lane of the current core */
    _SOC_IF_ERR_EXIT(_pm4x10_port_tricore_lane_info_get(unit, port, pm_info,
                                                        &core_start_lane, &core_end_lane,
                                                        &line_start_lane, &line_end_lane,
                                                        &int_lane_mask));

    if(nof_phys > 1)
    {   /* external phy. */
        is_ext_phy = 1;
        /* get most external phy default tx params */
        xphy_id = phy_access[nof_phys-1].access.addr;
        _SOC_IF_ERR_EXIT(portmod_xphy_is_legacy_phy_get(unit, xphy_id, &is_legacy_phy));
        if (!is_legacy_phy) {
            _SOC_IF_ERR_EXIT(phymod_phy_media_type_tx_get(&phy_access[nof_phys-1], phymodMediaTypeChipToChip, &phymod_tx));
        }

        /* overlap most external phy  preemphasis and amp from config */
        _SOC_IF_ERR_EXIT(
                _pm4x10_port_tx_config_init_set(unit, pm_info, core_start_lane,
                                       core_end_lane, &phymod_tx,
                                       init_config->ext_phy_tx_params,
                                       init_config->ext_phy_tx_params_user_flag,
                                       is_ext_phy));
    }

    is_ext_phy = 0;
    /* get serdes default tx params */
    _SOC_IF_ERR_EXIT(phymod_phy_media_type_tx_get(phy_access, phymodMediaTypeChipToChip, &phymod_tx));

    /* overlap serdes preemphasis and amp from config */
    _SOC_IF_ERR_EXIT(
            _pm4x10_port_tx_config_init_set(unit, pm_info, core_start_lane,
                                            core_end_lane, &phymod_tx,
                                            init_config->tx_params,
                                            init_config->tx_params_user_flag,
                                            is_ext_phy));

    /* set the outmost external PHY tx. internal and intermediary phy will be modified in portmod_port_phychain_phy_init*/
    if (nof_phys > 1){
        for(i=0; i < PHYMOD_MAX_LANES_PER_CORE; i++){
            phymod_tx_t_init(&(ext_phy_tx_params[i]));
            _SOC_IF_ERR_EXIT(PM4x10_EXT_PHY_TX_PARAMS_GET(unit, pm_info,
                                              &ext_phy_tx_params[i], i));
        }
        SOC_IF_ERROR_RETURN(
                portmod_port_phychain_tx_set(unit, phy_access, nof_phys,
                                             ext_phy_tx_params));
    }
    
    /* update the interface type in the scache buffer */
    _SOC_IF_ERR_EXIT(PM4x10_PORT_OP_MODE_SET(unit, pm_info,
                                             config->port_op_mode,
                                             port_index));

exit:
    SOC_FUNC_RETURN;
}


STATIC
int _pm4x10_pm_disable (int unit, int port, pm_info_t pm_info, int phy_acc)
{
    uint32 reg_val;
    int rv = 0;

    /* Put Serdes in reset*/
    if(PM_4x10_INFO(pm_info)->portmod_phy_external_reset) {
        rv = PM_4x10_INFO(pm_info)->portmod_phy_external_reset(unit, port, 1);
    } else {
        rv = _pm4x10_tsc_reset(unit, pm_info, port, 1);
    }
    if (rv) return (rv);

    /* put MAC in reset */
    rv = READ_XLPORT_MAC_CONTROLr(unit, phy_acc, &reg_val);
    if (rv) return (rv);

    soc_reg_field_set(unit, XLPORT_MAC_CONTROLr, &reg_val, XMAC0_RESETf, 1);
    return(WRITE_XLPORT_MAC_CONTROLr(unit, phy_acc, reg_val));
}

STATIC
int _pm4x10_pm_xlport_init (int unit, int port, pm_info_t pm_info,
                            int port_index, int phy_acc, int interface,
                            int timestamp_mode)     
{
    uint32 reg_val, port_mode;
    int    three_ports_mode, rv;
    SOC_INIT_FUNC_DEFS;

    /* Power Save */
    _SOC_IF_ERR_EXIT(READ_XLPORT_POWER_SAVEr(unit, phy_acc, &reg_val));
    soc_reg_field_set(unit, XLPORT_POWER_SAVEr, &reg_val, XPORT_CORE0f, 0); 
    _SOC_IF_ERR_EXIT(WRITE_XLPORT_POWER_SAVEr(unit, phy_acc, reg_val));

    /* Port configuration */
    rv = SOC_WB_ENGINE_GET_VAR(unit, SOC_WB_ENGINE_PORTMOD, 
               pm_info->wb_vars_ids[threePortsMode], &three_ports_mode);
    _SOC_IF_ERR_EXIT(rv);

    if(PM4x10_QUAD_MODE_IF(interface)) {
        port_mode = XLMAC_4_LANES_TOGETHER;
    } else if(PM4x10_DUAL_MODE_IF(interface)){
        port_mode = (!three_ports_mode) ?  XLMAC_2_LANES_DUAL :
                      ((port_index < 2) ?  XLMAC_3_TRI_0_0_2_3 : 
                                           XLMAC_3_TRI_0_1_2_2);
    } else { /*single mode*/
        port_mode = (!three_ports_mode) ? XLMAC_4_LANES_SEPARATE :
                     ((port_index < 2) ?  XLMAC_3_TRI_0_1_2_2 :
                                          XLMAC_3_TRI_0_0_2_3);
    }

    _SOC_IF_ERR_EXIT(READ_XLPORT_MODE_REGr(unit, phy_acc, &reg_val));
    soc_reg_field_set (unit, XLPORT_MODE_REGr, &reg_val, XPORT0_CORE_PORT_MODEf, port_mode);
    soc_reg_field_set (unit, XLPORT_MODE_REGr, &reg_val, XPORT0_PHY_PORT_MODEf,  port_mode);
    soc_reg_field_set (unit, XLPORT_MODE_REGr, &reg_val, EGR_1588_TIMESTAMPING_MODEf, timestamp_mode); 
    _SOC_IF_ERR_EXIT(WRITE_XLPORT_MODE_REGr(unit, phy_acc, reg_val));

    /* Bring MAC OOR */
    _SOC_IF_ERR_EXIT(READ_XLPORT_MAC_CONTROLr(unit, phy_acc, &reg_val));
    soc_reg_field_set(unit, XLPORT_MAC_CONTROLr, &reg_val, XMAC0_RESETf, 0);
    _SOC_IF_ERR_EXIT(WRITE_XLPORT_MAC_CONTROLr(unit, phy_acc, reg_val));

    /* Get Serdes OOR */
    if(PM_4x10_INFO(pm_info)->portmod_phy_external_reset) {
        if(!((interface == SOC_PORT_IF_CAUI)&&(PM_4x10_INFO(pm_info)->core_num_int))) {
            _SOC_IF_ERR_EXIT(PM_4x10_INFO(pm_info)->portmod_phy_external_reset(unit, port, 1));
            _SOC_IF_ERR_EXIT(PM_4x10_INFO(pm_info)->portmod_phy_external_reset(unit, port, 0));
        }
    } else {
        _SOC_IF_ERR_EXIT(_pm4x10_tsc_reset(unit, pm_info, port, 1));
        _SOC_IF_ERR_EXIT(_pm4x10_tsc_reset(unit, pm_info, port, 0));
    }

exit:
    SOC_FUNC_RETURN;        
}

STATIC
int _pm4x10_pm_core_probe (int unit, int port, pm_info_t pm_info, const portmod_port_add_info_t* add_info)
{
    int     rv = SOC_E_NONE;
    int    temp, phy;
    portmod_pbmp_t port_phys_in_pm;
    int probe =0;
    uint32 xphy_id;
    uint32 nof_ext_phys =0;
    int i, is_legacy_phy;
    int core_probed;
    phymod_core_access_t core_access;
    phymod_firmware_load_method_t fw_load_method;
    SOC_INIT_FUNC_DEFS;

    /* probe phys (both internal and external cores. */

    /* Get the first phy related to this port */
    PORTMOD_PBMP_ASSIGN(port_phys_in_pm, add_info->phys);
    PORTMOD_PBMP_AND(port_phys_in_pm, PM_4x10_INFO(pm_info)->phys);

    /* if internal core is not probe, probe it. Currently checking to type to see
       phymodDispatchTypeCount, later need to move to WB.  */
    _SOC_IF_ERR_EXIT(_pm4x10_core_probe(unit, port, pm_info, &PM_4x10_INFO(pm_info)->int_core_access,&probe, 0));

    /* save probed phy type to wb. */
    if(probe) {
         rv = SOC_WB_ENGINE_SET_VAR(unit, SOC_WB_ENGINE_PORTMOD,
                     pm_info->wb_vars_ids[phy_type],
                     &PM_4x10_INFO(pm_info)->int_core_access.type);
         _SOC_IF_ERR_EXIT(rv);
    }

    /* probe ext core related to this logical port. */
    temp = 0;
    PORTMOD_PBMP_ITER(PM_4x10_INFO(pm_info)->phys, phy) {
        if(PORTMOD_PBMP_MEMBER(port_phys_in_pm, phy)) {
            if( PM_4x10_INFO(pm_info)->nof_phys[temp] >= 2 ){
                nof_ext_phys = PM_4x10_INFO(pm_info)->nof_phys[temp]-1;
                for(i=0 ; i<nof_ext_phys ; i++) {
                    xphy_id = PM_4x10_INFO(pm_info)->lane_conn[i][temp].xphy_id;
                    if(xphy_id != PORTMOD_XPHY_ID_INVALID){
                        _SOC_IF_ERR_EXIT(portmod_xphy_core_probed_get(unit, xphy_id, &core_probed));
                            _SOC_IF_ERR_EXIT(portmod_xphy_fw_load_method_get(unit, xphy_id, &fw_load_method));
                        if( !core_probed ){
                            _SOC_IF_ERR_EXIT(portmod_xphy_core_access_get(unit, xphy_id, &core_access, &is_legacy_phy));
                            probe = 0;
                            if(SOC_E_NONE != (_pm4x10_core_probe(unit,port, pm_info, &core_access, &probe, 1))) {
                                /* 
                                 * Remove un-probed external PHY from phy number count.
                                 * Also save it to warmboot memory. 
                                 */
                                PM_4x10_INFO(pm_info)->nof_phys[temp] = i + 1;
                                rv = PM4x10_PORT_NUM_PHYS_SET(unit, pm_info, &(PM_4x10_INFO(pm_info)->nof_phys[temp]), temp);
                                _SOC_IF_ERR_EXIT(rv);
                                /* Set firmware download method to phymodFirmwareLoadMethodNone to avoid image download for unprobed XPHY */
                                fw_load_method = phymodFirmwareLoadMethodNone;
                            } else {
                                if (!probe) {
                                /* 
                                 * case where port_phy_addr is specified and phy is not connetced.
                                 * Remove un-probed external PHY from phy number count.
                                 * Also save it to warmboot memory. 
                                 */
                                PM_4x10_INFO(pm_info)->nof_phys[temp] = i + 1;
                                rv = PM4x10_PORT_NUM_PHYS_SET(unit, pm_info,&(PM_4x10_INFO(pm_info)->nof_phys[temp]), temp);
                                _SOC_IF_ERR_EXIT(rv);
                                /* Set firmware download method to phymodFirmwareLoadMethodNone to avoid image download for unprobed XPHY */
                                fw_load_method = phymodFirmwareLoadMethodNone;
                                } else {
                                    core_probed = 1;
                                }
                            }
                            _SOC_IF_ERR_EXIT(portmod_xphy_core_probed_set(unit, xphy_id, core_probed));
                            _SOC_IF_ERR_EXIT(portmod_xphy_fw_load_method_set(unit, xphy_id, fw_load_method));
                            /* Check if after the probe the ext phy is legacy, so we dont overrun the current value */
                            _SOC_IF_ERR_EXIT(portmod_xphy_is_legacy_phy_get(unit, xphy_id, &is_legacy_phy));
                            _SOC_IF_ERR_EXIT(portmod_xphy_core_access_set(unit, xphy_id, &core_access, is_legacy_phy));
                        }
                    }
                }
            }
        }
        temp++;
    }

   /*update warmboot engine*/
    rv = SOC_WB_ENGINE_SET_ARR(unit, SOC_WB_ENGINE_PORTMOD,
        pm_info->wb_vars_ids[phy_type],
        &PM_4x10_INFO(pm_info)->int_core_access.type, 0);
    _SOC_IF_ERR_EXIT(rv);

exit:
    SOC_FUNC_RETURN;
}

STATIC
int _pm4x10_pm_serdes_core_init(int unit, pm_info_t pm_info, const portmod_port_add_info_t* add_info)
{
    int     rv;
    phymod_core_init_config_t core_conf;
    phymod_core_status_t core_status;
    const portmod_port_interface_config_t* config;
    uint32 core_is_initialized;
    uint32 init_flags, is_bypass;
    soc_port_if_t interface = SOC_PORT_IF_NULL;
    SOC_INIT_FUNC_DEFS;

    config = &(add_info->interface_config);

    _SOC_IF_ERR_EXIT(phymod_core_init_config_t_init(&core_conf));

    rv = SOC_WB_ENGINE_GET_VAR(unit, SOC_WB_ENGINE_PORTMOD, pm_info->wb_vars_ids[isBypassed], &is_bypass);
    _SOC_IF_ERR_EXIT(rv);
    /* set internal setting. */

    core_conf.firmware_load_method = PM_4x10_INFO(pm_info)->fw_load_method;
    core_conf.firmware_loader = PM_4x10_INFO(pm_info)->external_fw_loader;
    if ((add_info->interface_config.interface ==  SOC_PORT_IF_QSGMII) &&
        PM_4x10_INFO(pm_info)->is_pm4x10q) {
        core_conf.lane_map = add_info->init_config.lane_map[0];
    } else {
        core_conf.lane_map = PM_4x10_INFO(pm_info)->lane_map;
    }

    _SOC_IF_ERR_EXIT(phymod_phy_inf_config_t_init(&core_conf.interface));
    _SOC_IF_ERR_EXIT(portmod_line_intf_from_config_get(config, &interface));
    _SOC_IF_ERR_EXIT(portmod_intf_to_phymod_intf(unit, config->speed,
                        interface, &core_conf.interface.interface_type));
    core_conf.interface.data_rate = config->speed;
    core_conf.interface.interface_modes = config->interface_modes;
    core_conf.interface.ref_clock = PM_4x10_INFO(pm_info)->ref_clk;
    core_conf.interface.pll_divider_req = add_info->init_config.pll_divider_req;

    _SOC_IF_ERR_EXIT(phymod_core_status_t_init(&core_status));

    if(PORTMOD_PORT_ADD_F_FIRMWARE_LOAD_VERIFY_GET(add_info)) {
        PHYMOD_CORE_INIT_F_FIRMWARE_LOAD_VERIFY_SET(&core_conf);
    }

    init_flags = PORTMOD_PORT_ADD_F_INIT_PASS1_GET(add_info) | PORTMOD_PORT_ADD_F_INIT_PASS2_GET(add_info);
    if (PORTMOD_PORT_ADD_F_INIT_PASS1_GET(add_info)) {
        PHYMOD_CORE_INIT_F_EXECUTE_PASS1_SET(&core_conf);
    }

    if(PORTMOD_PORT_ADD_F_INIT_PASS2_GET(add_info)) {
        PHYMOD_CORE_INIT_F_EXECUTE_PASS2_SET(&core_conf);
    }

    core_conf.op_datapath = add_info->phy_op_datapath;

    /* deal witn internal serdes first */
    rv = SOC_WB_ENGINE_GET_VAR (unit, SOC_WB_ENGINE_PORTMOD,
                                pm_info->wb_vars_ids[isInitialized],
                                &core_is_initialized);
     _SOC_IF_ERR_EXIT(rv);

    if (!PORTMOD_CORE_INIT_FLAG_INITIALZIED_GET(core_is_initialized)) {
        /* firmware load will happen after init_pass1 */
        if (!PORTMOD_CORE_INIT_FLAG_FIRMWARE_LOADED_GET(core_is_initialized) ||
           PORTMOD_PORT_ADD_F_INIT_PASS2_GET(add_info)) {
            if ((is_bypass) && (!PM_4x10_INFO(pm_info)->in_pm12x10)) {
                PHYMOD_DEVICE_OP_MODE_PCS_BYPASS_SET(PM_4x10_INFO(pm_info)->int_core_access.device_op_mode);
            }
            _SOC_IF_ERR_EXIT(phymod_core_init(&PM_4x10_INFO(pm_info)->int_core_access,
                                              &core_conf,
                                              &core_status));

            if (PORTMOD_PORT_ADD_F_INIT_PASS1_GET(add_info) || init_flags == 0) { 
                PORTMOD_CORE_INIT_FLAG_FIRMWARE_LOADED_SET(core_is_initialized);
                rv = SOC_WB_ENGINE_SET_VAR (unit, SOC_WB_ENGINE_PORTMOD,
                                        pm_info->wb_vars_ids[isInitialized],
                                        &core_is_initialized);
                _SOC_IF_ERR_EXIT(rv);
            }
            if (PORTMOD_PORT_ADD_F_INIT_PASS2_GET(add_info) || init_flags == 0) { 
                PORTMOD_CORE_INIT_FLAG_INITIALZIED_SET(core_is_initialized);
                rv = SOC_WB_ENGINE_SET_VAR (unit, SOC_WB_ENGINE_PORTMOD,
                                        pm_info->wb_vars_ids[isInitialized],
                                        &core_is_initialized);
                _SOC_IF_ERR_EXIT(rv);
            }
        }
    }

    /*update warmboot engine*/
    rv = SOC_WB_ENGINE_SET_ARR(unit, SOC_WB_ENGINE_PORTMOD,
        pm_info->wb_vars_ids[phy_type],
        &PM_4x10_INFO(pm_info)->int_core_access.type, 0);
    _SOC_IF_ERR_EXIT(rv);

    exit:
        SOC_FUNC_RETURN;
}

STATIC
int _pm4x10_pm_ext_phy_core_init(int unit, pm_info_t pm_info, const portmod_port_add_info_t* add_info)
{
    int    temp, phy;
    phymod_core_init_config_t core_conf;
    phymod_core_status_t core_status;
    portmod_pbmp_t port_phys_in_pm;
    uint32 xphy_id;
    uint32 nof_ext_phys =0;
    int i;
    int is_initialized;
    int lane;
    int init_all;
    phymod_firmware_load_method_t fw_load_method;
    int force_fw_load;
    phymod_lane_map_t lane_map;
    phymod_core_access_t core_access;
    uint32 primary_core_num;
    int is_legacy_phy;
    SOC_INIT_FUNC_DEFS;

    init_all = (!PORTMOD_PORT_ADD_F_INIT_CORE_PROBE_GET(add_info) &&
                    !PORTMOD_PORT_ADD_F_INIT_PASS1_GET(add_info) &&
                    !PORTMOD_PORT_ADD_F_INIT_PASS2_GET(add_info)) ? 1 : 0;

    /* Get the first phy related to this port */
    PORTMOD_PBMP_ASSIGN(port_phys_in_pm, add_info->phys);
    PORTMOD_PBMP_AND(port_phys_in_pm, PM_4x10_INFO(pm_info)->phys);

    /* core init (both internal and external cores. */

    _SOC_IF_ERR_EXIT(phymod_core_init_config_t_init(&core_conf));

    core_conf.interface.pll_divider_req =
                          add_info->init_config.pll_divider_req;

    _SOC_IF_ERR_EXIT(phymod_core_status_t_init(&core_status));


    if (PORTMOD_PORT_ADD_F_INIT_PASS1_GET(add_info)) {
        PHYMOD_CORE_INIT_F_EXECUTE_PASS1_SET(&core_conf);
    }

    if(PORTMOD_PORT_ADD_F_INIT_PASS2_GET(add_info)) {
        PHYMOD_CORE_INIT_F_EXECUTE_PASS2_SET(&core_conf);
    }

    core_conf.op_datapath = add_info->phy_op_datapath;

    /* deal with ext phys now ... */
    if(PORTMOD_PORT_ADD_F_PORT_ATTACH_EXT_PHY_SKIP_GET(add_info)) {
        return(SOC_E_NONE);
    }

    /* adjust config setup for external phys. */

    /* for external phys, FW load method is default to Internal
       unless it is force to NONE. */


    core_conf.firmware_load_method = (PM_4x10_INFO(pm_info)->fw_load_method ==
                                        phymodFirmwareLoadMethodNone) ?
                                        phymodFirmwareLoadMethodNone :
                                        phymodFirmwareLoadMethodInternal;
    core_conf.firmware_loader = NULL;

    /* config "no swap" for external phys.set later using APIs*/
    for(lane=0 ; lane < (core_conf.lane_map.num_of_lanes); lane++) {
        core_conf.lane_map.lane_map_rx[lane] = lane;
        core_conf.lane_map.lane_map_tx[lane] = lane;
    }


    temp = 0;
    PORTMOD_PBMP_ITER(PM_4x10_INFO(pm_info)->phys, phy) {
        if(PORTMOD_PBMP_MEMBER(port_phys_in_pm, phy)) {
            if( PM_4x10_INFO(pm_info)->nof_phys[temp] >= 2 ){
                nof_ext_phys = PM_4x10_INFO(pm_info)->nof_phys[temp]-1;
                for(i=0 ; i<nof_ext_phys ; i++) {
                    xphy_id = PM_4x10_INFO(pm_info)->lane_conn[i][temp].xphy_id;
                    if(xphy_id != PORTMOD_XPHY_ID_INVALID){

                        _SOC_IF_ERR_EXIT(portmod_xphy_core_access_get(unit, xphy_id, &core_access, &is_legacy_phy));
                        _SOC_IF_ERR_EXIT(portmod_xphy_fw_load_method_get(unit, xphy_id, &fw_load_method));
                        _SOC_IF_ERR_EXIT(portmod_xphy_force_fw_load_get(unit, xphy_id, &force_fw_load));
                        _SOC_IF_ERR_EXIT(portmod_xphy_lane_map_get(unit, xphy_id, &lane_map));
                        _SOC_IF_ERR_EXIT(portmod_xphy_primary_core_num_get(unit, xphy_id, &primary_core_num));

                        /* for external phys, FW load method is default to Internal
                        unless it is force to NONE. */
                        core_conf.firmware_load_method = fw_load_method;
                        core_conf.firmware_loader = NULL;

                        if (force_fw_load == phymodFirmwareLoadForce) {
                            PHYMOD_CORE_INIT_F_FW_FORCE_DOWNLOAD_SET(&core_conf);
                        }
                        else if (force_fw_load == phymodFirmwareLoadAuto) {
                            PHYMOD_CORE_INIT_F_FW_AUTO_DOWNLOAD_SET(&core_conf);
                        }

                        /* config "no swap" for external phys.set later using APIs*/
                        for(lane=0 ; lane < lane_map.num_of_lanes; lane++) {
                            core_conf.lane_map.lane_map_rx[lane] = lane_map.lane_map_rx[lane];
                            core_conf.lane_map.lane_map_tx[lane] = lane_map.lane_map_tx[lane];
                        }
                        if (xphy_id == primary_core_num) {
                            _SOC_IF_ERR_EXIT(portmod_xphy_is_initialized_get(unit, xphy_id, &is_initialized));
                             /* Very first time this function gets called, honor the call and 
                              * ignore the flags */
                             if (PORTMOD_PORT_ADD_F_INIT_PASS1_GET(add_info)) {
                                 if (PM_4x10_INFO(pm_info)->first_phy == phy) {
                                    _SOC_IF_ERR_EXIT(portmod_xphy_core_config_set(unit, xphy_id, &core_conf));
                                 }
                             }
                        } else {
                            _SOC_IF_ERR_EXIT(portmod_xphy_is_initialized_get(unit, primary_core_num, &is_initialized));
                        } 


                        if ((PORTMOD_PORT_ADD_F_INIT_PASS1_GET(add_info) || init_all) &&
                            (is_initialized & PHYMOD_CORE_INIT_F_EXECUTE_PASS1))
                            continue;

                        if ((PORTMOD_PORT_ADD_F_INIT_PASS2_GET(add_info) || init_all) &&
                            (is_initialized & PHYMOD_CORE_INIT_F_EXECUTE_PASS2)) {
                            continue;
                        }

                        _SOC_IF_ERR_EXIT(portmod_xphy_is_legacy_phy_get(unit, xphy_id, &is_legacy_phy));
                        if (is_legacy_phy) {
                            /* Dont do a core init for legacy Phys.*/
                        } else {
                           _SOC_IF_ERR_EXIT(phymod_core_init(&core_access, &core_conf, &core_status));
                        }

                        if (init_all) {
                            is_initialized = (PHYMOD_CORE_INIT_F_EXECUTE_PASS2 | PHYMOD_CORE_INIT_F_EXECUTE_PASS1);
                        }
                        else if (PORTMOD_PORT_ADD_F_INIT_PASS2_GET(add_info)) {
                            is_initialized |= PHYMOD_CORE_INIT_F_EXECUTE_PASS2;
                        } else {
                            is_initialized = PHYMOD_CORE_INIT_F_EXECUTE_PASS1;
                        }

                        _SOC_IF_ERR_EXIT(portmod_xphy_is_initialized_set(unit, xphy_id, is_initialized));

                        if (xphy_id != primary_core_num) {
                            _SOC_IF_ERR_EXIT(portmod_xphy_is_initialized_set(unit, primary_core_num, is_initialized));
                        }
                    }
                }
            }
        }
        temp++;
    }

exit:
    SOC_FUNC_RETURN;
}



STATIC
int _pm4x10_pm_core_init (int unit, pm_info_t pm_info, const portmod_port_add_info_t* add_info)
{
    SOC_INIT_FUNC_DEFS;
    _SOC_IF_ERR_EXIT(_pm4x10_pm_serdes_core_init(unit,pm_info,add_info));
    _SOC_IF_ERR_EXIT(_pm4x10_pm_ext_phy_core_init(unit,pm_info,add_info));

exit:
    SOC_FUNC_RETURN;
}


STATIC
int _pm4x10_port_attach_core_probe (int unit, int port, pm_info_t pm_info, 
                        const portmod_port_add_info_t* add_info)
{
    int        port_index = -1, rv = 0, phy, i, phys_count = 0;
    uint32     pm_is_active = 0;
    portmod_pbmp_t port_phys_in_pm;
    int first_phy = -1, phy_acc;
    int prev_phy = -1;

    SOC_INIT_FUNC_DEFS;

    /* Get the first phy related to this port */
    PORTMOD_PBMP_ASSIGN(port_phys_in_pm, add_info->phys);
    PORTMOD_PBMP_AND(port_phys_in_pm, PM_4x10_INFO(pm_info)->phys);
    PORTMOD_PBMP_COUNT(port_phys_in_pm, phys_count);

    /* if first phy is initialized and use it, default is -1 anyways */
    first_phy = PM_4x10_INFO(pm_info)->first_phy;

    i = 0;
    PORTMOD_PBMP_ITER(PM_4x10_INFO(pm_info)->phys, phy) {
        if (prev_phy != -1) {
            i += phy - prev_phy;
        }

        if(PORTMOD_PBMP_MEMBER(port_phys_in_pm, phy)) {
            rv = SOC_WB_ENGINE_SET_ARR(unit, SOC_WB_ENGINE_PORTMOD,
                             pm_info->wb_vars_ids[ports], &port, i);
            _SOC_IF_ERR_EXIT(rv);
            first_phy = (first_phy == -1) ? phy : first_phy;
            port_index = (port_index == -1 ? i : port_index);
        }
        prev_phy = phy;
    }
    PM_4x10_INFO(pm_info)->first_phy = first_phy;

    PM4x10_PHY_ACCESS_GET(unit, port, pm_info, phy_acc);

    if(port_index<0 || port_index>=PM4X10_LANES_PER_CORE) {
        _SOC_EXIT_WITH_ERR(SOC_E_PARAM,
                           (_SOC_MSG("can't attach port %d. can't find relevant phy"), port));
    }

     rv = PM4x10_IS_HG_SET(unit, pm_info, add_info->init_config.is_hg,
                          port_index);
    _SOC_IF_ERR_EXIT(rv);
    rv = PM4x10_AN_MODE_SET(unit, pm_info, add_info->init_config.an_mode,
                            port_index);
    _SOC_IF_ERR_EXIT(rv);
    rv = PM4x10_FS_CL72_SET(unit, pm_info, add_info->init_config.fs_cl72,
                            port_index);
    _SOC_IF_ERR_EXIT(rv);
    rv = PM4x10_CX4_10G_SET(unit, pm_info, add_info->init_config.cx4_10g,
                            port_index);
    _SOC_IF_ERR_EXIT(rv);
    rv = PM4x10_AN_CL72_SET(unit, pm_info, add_info->init_config.an_cl72,
                            port_index);
    _SOC_IF_ERR_EXIT(rv);
    rv = PM4x10_AN_FEC_SET(unit, pm_info, add_info->init_config.an_fec,
                           port_index);
    _SOC_IF_ERR_EXIT(rv);
    rv = PM4x10_AN_CL37_SET(unit, pm_info, add_info->init_config.an_cl37,
                            port_index);
    _SOC_IF_ERR_EXIT(rv);
    rv = PM4x10_AN_MASTER_LANE_SET(unit, pm_info,
                                   add_info->init_config.an_master_lane,
                                   port_index);
    _SOC_IF_ERR_EXIT(rv);
    rv = PM4x10_CL37_SGMII_CNT_SET(unit, pm_info,
                                   add_info->init_config.cl37_sgmii_cnt,
                                   port_index);
    _SOC_IF_ERR_EXIT(rv);
    rv = PM4x10_CL37_SGMII_RESTART_CNT_SET(unit, pm_info,
                                  add_info->init_config.cl37_sgmii_RESTART_CNT,
                                  port_index);
    _SOC_IF_ERR_EXIT(rv);
    rv = PM4x10_PORT_FALLBACK_LANE_SET(unit, pm_info,
                                       add_info->init_config.port_fallback_lane,
                                       port_index);
    _SOC_IF_ERR_EXIT(rv);
    rv = PM4x10_RXAUI_MODE_SET(unit, pm_info,
                               add_info->init_config.rxaui_mode,
                               port_index);
    _SOC_IF_ERR_EXIT(rv);
    rv = PM4x10_MAX_SPEED_SET(unit, pm_info,
                              add_info->interface_config.max_speed, port_index);
    _SOC_IF_ERR_EXIT(rv);
    rv = PM4x10_INTERFACE_TYPE_SET(unit, pm_info,
                                   add_info->interface_config.interface,
                                   port_index);
    _SOC_IF_ERR_EXIT(rv);
    rv = PM4x10_OUTMOST_PHY_INTERFACE_TYPE_SET(unit, pm_info,
                                               add_info->interface_config.interface,
                                               port_index);
    _SOC_IF_ERR_EXIT(rv); 
    rv = PM4x10_SERDES_INTERFACE_TYPE_SET(unit, pm_info,
                              add_info->interface_config.serdes_interface,
                              port_index);
    _SOC_IF_ERR_EXIT(rv);
    rv = PM4x10_INTERFACE_MODES_SET(unit, pm_info,
                                    add_info->interface_config.interface_modes,
                                    port_index);
    _SOC_IF_ERR_EXIT(rv);
    rv = PM4x10_PORT_OP_MODE_SET(unit, pm_info,
                                 add_info->interface_config.port_op_mode,
                                 port_index);
    _SOC_IF_ERR_EXIT(rv);

    rv = PM4x10_PORT_PLL_DIVIDER_REQ_SET(unit, pm_info,
                                 add_info->init_config.pll_divider_req,
                                 port_index);
    _SOC_IF_ERR_EXIT(rv);

    if (add_info->init_config.polarity_overwrite == 1) {
        sal_memcpy(&PM_4x10_INFO(pm_info)->polarity,
            &add_info->init_config.polarity[PM_4x10_INFO(pm_info)->core_num_int],
               sizeof(phymod_polarity_t));
    }
    if (add_info->init_config.lane_map_overwrite == 1) {
        sal_memcpy(&PM_4x10_INFO(pm_info)->lane_map,
            &add_info->init_config.lane_map[PM_4x10_INFO(pm_info)->core_num_int],
               sizeof(phymod_lane_map_t));
    }
    if (add_info->init_config.ref_clk_overwrite == 1) {
        PM_4x10_INFO(pm_info)->ref_clk = add_info->init_config.ref_clk;
    }
    if (add_info->init_config.fw_load_method_overwrite == 1) {
        PM_4x10_INFO(pm_info)->fw_load_method =
            add_info->init_config.fw_load_method[PM_4x10_INFO(pm_info)->core_num_int];
    }

    /* port_index is 0, 1, 2, or 3 */
    rv = SOC_WB_ENGINE_GET_VAR (unit, SOC_WB_ENGINE_PORTMOD,
                            pm_info->wb_vars_ids[isActive], &pm_is_active);
    _SOC_IF_ERR_EXIT(rv);

    /* if not active - initalize PM */
    if(!pm_is_active){

#ifdef PORTMOD_PM12X10_SUPPORT
        if(PM_4x10_INFO(pm_info)->in_pm12x10) {
            PORTMOD_PBMP_ITER(PM_4x10_INFO(pm_info)->phys, phy) {break;}
            /* this will be redirected to pm12x10 */
#ifdef PORTMOD_PM12X10_XGS_SUPPORT
            
            if (SOC_IS_APACHE(unit)) {
                rv = pm12x10_xgs_pm4x10_enable(unit, port, phy, 1);
                _SOC_IF_ERR_EXIT(rv);
            } else
#endif
            {
                rv = pm12x10_pm4x10_enable(unit, port, phy, 1);
                _SOC_IF_ERR_EXIT(rv);
            }
        }
#endif /* PORTMOD_PM12X10_SUPPORT */

        rv = _pm4x10_pm_xlport_init (unit, port, pm_info, port_index,
                 phy_acc, add_info->interface_config.interface,
                 add_info->flags & PORTMOD_PORT_ADD_F_EGR_1588_TIMESTAMP_MODE_48BIT ? 1 : 0);
        _SOC_IF_ERR_EXIT(rv);

        pm_is_active = 1;
        rv = SOC_WB_ENGINE_SET_VAR(unit, SOC_WB_ENGINE_PORTMOD,
             pm_info->wb_vars_ids[isActive], &pm_is_active);
        _SOC_IF_ERR_EXIT(rv);
    }

    /*probe the core*/
    rv = _pm4x10_pm_core_probe(unit, port, pm_info, add_info);
    _SOC_IF_ERR_EXIT(rv);

exit:
    SOC_FUNC_RETURN;
}

STATIC
int _pm4x10_port_attach_resume_fw_load (int unit, int port, pm_info_t pm_info, 
                        const portmod_port_add_info_t* add_info)
{
    int        port_index = -1, rv = 0, i, nof_phys, port_i;
    uint32     bitmap;
    int my_i, phy_acc, is_bypass, found_first_active_lane;
    int fec_enable = 0;
    phymod_interface_t          phymod_serdes_interface = phymodInterfaceCount; 
    phymod_phy_init_config_t    phy_init_config;
    phymod_phy_inf_config_t     *phy_interface_config = &phy_init_config.interface;
    phymod_phy_access_t phy_access[1+MAX_PHYN];
    phymod_phy_access_t phy_access_lane;
    portmod_port_ability_t port_ability;
    portmod_access_get_params_t params;
    soc_port_if_t interface = SOC_PORT_IF_NULL;
    uint32 tx_params_user_flag, ext_phy_tx_params_user_flag;

    SOC_INIT_FUNC_DEFS;
    PM4x10_PHY_ACCESS_GET(unit, port, pm_info, phy_acc);

    rv = SOC_WB_ENGINE_GET_VAR(unit, SOC_WB_ENGINE_PORTMOD, pm_info->wb_vars_ids[isBypassed], &is_bypass);
    _SOC_IF_ERR_EXIT(rv);

    /* initialze phys */

    _SOC_IF_ERR_EXIT(_pm4x10_port_index_get (unit, port, pm_info, &port_index,
                                             &bitmap));

    /* clean up polarity and tx of phy_init_config */
    _SOC_IF_ERR_EXIT(phymod_phy_init_config_t_init(&phy_init_config));

    _SOC_IF_ERR_EXIT(portmod_access_get_params_t_init(unit, &params));
    _SOC_IF_ERR_EXIT(pm4x10_port_phy_lane_access_get(unit, port, pm_info,
                                &params, 1, &phy_access_lane, &nof_phys, NULL));

    /*init the core*/
    rv = _pm4x10_pm_core_init(unit, pm_info, add_info);
    _SOC_IF_ERR_EXIT(rv);

    /* initalize port */
    rv = _pm4x10_pm_port_init(unit, port, phy_acc, port_index, add_info, 1, pm_info);
    _SOC_IF_ERR_EXIT(rv);

    _SOC_IF_ERR_EXIT(portmod_port_chain_phy_access_get(unit, port, pm_info,
                                                           phy_access ,(1+MAX_PHYN),
                                                           &nof_phys));
    my_i = 0;
    found_first_active_lane = 0;
    for(i=0 ; i<PM4X10_LANES_PER_CORE ; i++) {

        rv = SOC_WB_ENGINE_GET_ARR (unit, SOC_WB_ENGINE_PORTMOD,
                         pm_info->wb_vars_ids[ports], &port_i, i);
        _SOC_IF_ERR_EXIT(rv);

        if(port_i != port) {
            /* There is a difference between XGS and DNX devices. For DNX, a port can consist inconsecutive physical lanes.
             * For example, a port can consist lane 1 and 3 of the same core. For XGS, on the other hand, a port always consist 
             * consecutive physical lanes. Therefore my_i below needs not be incremented for XGS devices. 
             */
            if(PHYMOD_DEVICE_OP_MODE_PCS_BYPASS_GET(PM_4x10_INFO(pm_info)->int_core_access.device_op_mode) && found_first_active_lane){
                my_i++;
            }
            continue;
        } else {
            found_first_active_lane = 1;
        }

        if(SHR_BITGET(&PM_4x10_INFO(pm_info)->polarity.tx_polarity,i)) {
            SHR_BITSET(&phy_init_config.polarity.tx_polarity, my_i);
        }

        if(SHR_BITGET(&PM_4x10_INFO(pm_info)->polarity.rx_polarity,i)) {
            SHR_BITSET(&phy_init_config.polarity.rx_polarity, my_i);
        }
        my_i++;
    }

    _SOC_IF_ERR_EXIT(PM4x10_PORT_OP_MODE_SET(unit, pm_info,
                                             add_info->phy_op_mode,
                                             port_index));

    
    _SOC_IF_ERR_EXIT(_pm4x10_port_interface_config_init_set(unit, port, pm_info,
                                            &add_info->interface_config,
                                            &add_info->init_config));

    /* copy the tx params from PM database to the phy init config */
    for(i=0 ; i<PM4X10_LANES_PER_CORE ; i++) {

        _SOC_IF_ERR_EXIT(PM4x10_TX_PARAMS_USER_FLAG_GET(unit, pm_info,
                                                  &tx_params_user_flag, i));
        phy_init_config.tx_params_user_flag[i] = tx_params_user_flag;

        _SOC_IF_ERR_EXIT(PM4x10_TX_PARAMS_GET(unit, pm_info,
                                              &phy_init_config.tx[i], i));

        _SOC_IF_ERR_EXIT(PM4x10_EXT_PHY_TX_PARAMS_USER_FLAG_GET(unit, pm_info,
                                            &ext_phy_tx_params_user_flag, i));
        phy_init_config.ext_phy_tx_params_user_flag[i] =
                                            ext_phy_tx_params_user_flag;

        _SOC_IF_ERR_EXIT(PM4x10_EXT_PHY_TX_PARAMS_GET(unit, pm_info,
                                        &phy_init_config.ext_phy_tx[i], i));
    }

    /* get phy interface config details */

    _SOC_IF_ERR_EXIT(PM4x10_INTERFACE_TYPE_GET(unit, pm_info, &interface,
                                               port_index));
    _SOC_IF_ERR_EXIT(
            PM4x10_INTERFACE_MODES_GET(unit, pm_info,
                                       &phy_interface_config->interface_modes,
                                       port_index));

    /* add_info->interface_config->interface comes from soc_esw_portctrl_config_get()
     * and is a very general value based on the port speed, number of lanes, and
     * encap regardless of the underlying PM type and whether there is an ext PHY
     * on the port. Here pm4x10 may change the default interface type under certain
     * situations during system init according to the more fine-grained conditions.
     */ 
    if ((add_info->interface_config.speed == 42000 ||
         add_info->interface_config.speed == 40000 ) &&
        phy_interface_config->interface_modes & PHYMOD_INTF_MODES_HIGIG &&
        nof_phys == 1) {
       _SOC_IF_ERR_EXIT(_pm4x10_default_interface_get(unit, port,
                            &(add_info->interface_config), &interface));

       /* Since the interface type written into the outmost PHY has changed,
        * the software state also needs to be updated so that "ps" will show
        * the correct interface type.
        */
       _SOC_IF_ERR_EXIT
           (PM4x10_INTERFACE_TYPE_SET(unit, pm_info, interface, port_index));
       _SOC_IF_ERR_EXIT
           (PM4x10_OUTMOST_PHY_INTERFACE_TYPE_SET(unit, pm_info, interface, port_index));
    }

    _SOC_IF_ERR_EXIT(PM4x10_PORT_OP_MODE_GET(unit, pm_info,
                                             &phy_init_config.op_mode,
                                             port_index));

    _SOC_IF_ERR_EXIT(portmod_intf_to_phymod_intf(unit,
                               add_info->interface_config.speed,
                               interface,
                               &phy_interface_config->interface_type));
    phy_interface_config->data_rate = add_info->interface_config.speed;
    phy_interface_config->pll_divider_req =
                    add_info->interface_config.pll_divider_req;
    phy_interface_config->ref_clock = PM_4x10_INFO(pm_info)->ref_clk;

    _SOC_IF_ERR_EXIT(portmod_port_phychain_phy_init(unit, phy_access, nof_phys,
                                                    &phy_init_config));

    /* enable fec if set, should be done before setting the speed */
    fec_enable = add_info->init_config.serdes_fec_enable;

    if (fec_enable) {
        int value = 0;

        /* FEC CL91 not supported on TSCE */
        if (fec_enable != PORTMOD_PORT_FEC_CL74) {
            _SOC_IF_ERR_EXIT(SOC_E_PARAM);
        }

        value |= TRUE;
        rv = pm4x10_port_fec_enable_set(unit, port, pm_info, value);
        _SOC_IF_ERR_EXIT(rv);
    }

    if (add_info->interface_config.serdes_interface != SOC_PORT_IF_NULL) {
        _SOC_IF_ERR_EXIT(portmod_intf_to_phymod_intf(unit, 
                                add_info->interface_config.speed, 
                                add_info->interface_config.serdes_interface, 
                                &phymod_serdes_interface));
    }

    /* store the speed in PM database */
    _SOC_IF_ERR_EXIT(PM4x10_SPEED_SET(unit, pm_info,
                                      add_info->interface_config.speed,
                                      port_index));


    /* Apply to ALL PHY with same interface config */
    _SOC_IF_ERR_EXIT(
            portmod_port_phychain_interface_config_set(unit, port, phy_access, nof_phys,
                                          add_info->interface_config.flags ,
                                          phy_interface_config,
                                          phymod_serdes_interface,
                                          PM_4x10_INFO(pm_info)->ref_clk,
                                          PORTMOD_INIT_F_ALL_PHYS));


    if(add_info->init_config.fs_cl72) {
       /* config port_init_cl72 is used to enable fs cl72 only in the serdes,
        do not enable cl72 on external phys. Set the no of phys argument to 1 to avoid setting cl72 enable 
        on ext phys */
       /* need to check if multi core and not pcs by pass mode */
       if ((phy_interface_config->data_rate >= 100000) &&
           (phy_interface_config->interface_type != phymodInterfaceBypass)) {
            uint32_t temp_flag = add_info->interface_config.flags;
            if (temp_flag == PHYMOD_INTF_F_SET_SPD_NO_TRIGGER) {
                _SOC_IF_ERR_EXIT(portmod_port_phychain_cl72_set(unit, port, phy_access, 1, 1));
            }
       } else {
                _SOC_IF_ERR_EXIT(portmod_port_phychain_cl72_set(unit, port, phy_access, 1, 1));
       }
    }
    /* set the default advert ability */
    _SOC_IF_ERR_EXIT
        (pm4x10_port_ability_local_get(unit, port, pm_info, PORTMOD_INIT_F_EXTERNAL_MOST_ONLY, &port_ability));
    _SOC_IF_ERR_EXIT
        (pm4x10_port_ability_advert_set(unit, port, pm_info, PORTMOD_INIT_F_EXTERNAL_MOST_ONLY, &port_ability));

    /* De-Assert SOFT_RESET */
        _SOC_IF_ERR_EXIT(pm4x10_port_soft_reset(unit, port, pm_info, 0));

exit:
    SOC_FUNC_RETURN;

}

STATIC
int _pm4x10_an_mode_update (int unit, int port, pm_info_t pm_info,
                            int an_cl37, int an_cl73)
{
    int is_hg, rv;
    phymod_an_mode_type_t an_mode;
    int    port_index;
    uint32_t bitmap;

    SOC_INIT_FUNC_DEFS;
    _SOC_IF_ERR_EXIT(_pm4x10_port_index_get (unit, port, pm_info, &port_index,
                                             &bitmap));

    rv = PM4x10_IS_HG_GET(unit, pm_info, &is_hg, port_index);
    _SOC_IF_ERR_EXIT(rv);
    rv = PM4x10_AN_MODE_GET(unit, pm_info, &an_mode, port_index);
    _SOC_IF_ERR_EXIT(rv);
    if(is_hg) {
        an_mode = phymod_AN_MODE_CL37BAM ;
        if (an_cl37 == PORTMOD_CL37_W_BAM_HG) {
            an_mode = phymod_AN_MODE_CL37BAM_10P9375G_VCO;
        }
    } else if (an_cl73) {
        switch (an_cl73) {
            case PORTMOD_CL73_W_BAM:
                an_mode = phymod_AN_MODE_CL73BAM;
                break;
         /* case PORTMOD_CL73_WO_BAM:
          *     an_mode = phymod_AN_MODE_CL73;
          *     break;
          */
            default:
                an_mode = phymod_AN_MODE_CL73;
                break;
        }
    } else {
        an_mode = phymod_AN_MODE_CL37;
        if(an_cl37 == PORTMOD_CL37_SGMII_COMBO){
           an_mode = phymod_AN_MODE_CL37_SGMII;
        }
    }

    rv = PM4x10_AN_MODE_SET(unit, pm_info, an_mode, port_index);
    _SOC_IF_ERR_EXIT(rv);
exit:
    SOC_FUNC_RETURN;
}

int pm4x10_port_attach (int unit, int port, pm_info_t pm_info, 
                        const portmod_port_add_info_t* add_info)
{
    phymod_phy_access_t serdes_access;
    int init_all = (!PORTMOD_PORT_ADD_F_INIT_CORE_PROBE_GET(add_info) &&
                    !PORTMOD_PORT_ADD_F_INIT_PASS1_GET(add_info) &&
                    !PORTMOD_PORT_ADD_F_INIT_PASS2_GET(add_info)) ? 1 : 0;

    if (PORTMOD_PORT_ADD_F_INIT_CORE_PROBE_GET(add_info) || (init_all)) {
        SOC_IF_ERROR_RETURN(_pm4x10_port_attach_core_probe (unit, port, pm_info, add_info));
    }

    if (PORTMOD_PORT_ADD_F_INIT_CORE_PROBE_GET(add_info) &&
        !PORTMOD_PORT_ADD_F_INIT_PASS1_GET(add_info)) {
        return (SOC_E_NONE);
    }

    if (PORTMOD_PORT_ADD_F_INIT_PASS1_GET(add_info) || (init_all)) {

        if (!PORTMOD_PORT_ADD_F_AUTONEG_CONFIG_SKIP_GET(add_info))
        {
            SOC_IF_ERROR_RETURN(_pm4x10_an_mode_update (unit, port, pm_info,
                                              add_info->init_config.an_cl37,
                                              add_info->init_config.an_cl73));
        }

        if (PM_4x10_INFO(pm_info)->rescal != -1) {
            sal_memcpy(&serdes_access, &(PM_4x10_INFO(pm_info)->int_core_access), sizeof(phymod_phy_access_t));
            phymod_phy_rescal_set(&serdes_access, 1, PM_4x10_INFO(pm_info)->rescal);
        }

        SOC_IF_ERROR_RETURN(_pm4x10_pm_core_init(unit, pm_info, add_info));
    }

    if (PORTMOD_PORT_ADD_F_INIT_PASS1_GET(add_info)) {
        return (SOC_E_NONE);
    }

    SOC_IF_ERROR_RETURN(_pm4x10_port_attach_resume_fw_load (unit, port, pm_info, add_info));

    return (SOC_E_NONE);
}


int pm4x10_port_detach(int unit, int port, pm_info_t pm_info)
{
    phymod_phy_access_t phy_access[1+MAX_PHYN];
    phymod_phy_inf_config_t phy_interface_config;

    int     enable, tmp_port, i=0, rv=0, port_index = -1, nof_phys;
    int     invalid_port = -1, is_last_one = TRUE, phy_acc;
    int     phytype;
    uint32  inactive = 0;

    SOC_INIT_FUNC_DEFS;

    PM4x10_PHY_ACCESS_GET(unit, port, pm_info, phy_acc);

    _SOC_IF_ERR_EXIT(pm4x10_port_enable_get(unit, port, pm_info, 0, &enable));
    if(enable) {
        _SOC_EXIT_WITH_ERR(SOC_E_PARAM, 
                  (_SOC_MSG("can't detach active port %d"), port));
    }

    /* in case of 100G disable the speed for the port */
    _SOC_IF_ERR_EXIT(portmod_port_chain_phy_access_get(unit, port, pm_info, phy_access,(1+MAX_PHYN), &nof_phys));

   if( (nof_phys > 1) && (phy_access[nof_phys-1].access.lane_mask == 0) ) {
        nof_phys = 1;
    }

    _SOC_IF_ERR_EXIT(portmod_port_phychain_interface_config_get(unit, port, phy_access, nof_phys,
                                   0, PM_4x10_INFO(pm_info)->ref_clk, &phy_interface_config, PORTMOD_INIT_F_EXTERNAL_MOST_ONLY));

    if ( phy_interface_config.data_rate >= 100000 ) {
        _SOC_IF_ERR_EXIT(portmod_port_phychain_interface_config_set(unit, port, phy_access, nof_phys,
                                   PHYMOD_INTF_F_SET_SPD_DISABLE,
                                   &phy_interface_config, phy_interface_config.interface_type, 
                                   PM_4x10_INFO(pm_info)->ref_clk,
                                   PORTMOD_INIT_F_ALL_PHYS ));
        _SOC_IF_ERR_EXIT(portmod_port_phychain_interface_config_set(unit, port, phy_access, nof_phys,
                                   PHYMOD_INTF_F_SPEED_CONFIG_CLEAR,
                                   &phy_interface_config, phy_interface_config.interface_type,
                                   PM_4x10_INFO(pm_info)->ref_clk,
                                   PORTMOD_INIT_F_INTERNAL_SERDES_ONLY));
    }


    /*remove from array and check if it was the last one*/
    for( i = 0 ; i < MAX_PORTS_PER_PM4X10; i++) {
       rv = SOC_WB_ENGINE_GET_ARR(unit, SOC_WB_ENGINE_PORTMOD, 
                    pm_info->wb_vars_ids[ports], &tmp_port, i);
       _SOC_IF_ERR_EXIT(rv);

       if(tmp_port == port){
           port_index = (port_index == -1 ? i : port_index);
           rv = SOC_WB_ENGINE_SET_ARR(unit, SOC_WB_ENGINE_PORTMOD, 
                    pm_info->wb_vars_ids[ports], &invalid_port, i);
           _SOC_IF_ERR_EXIT(rv);
       } else if (tmp_port != -1) {
           is_last_one = FALSE;
       }
    }

    if(port_index == -1) { 
        _SOC_EXIT_WITH_ERR(SOC_E_PORT,(_SOC_MSG("Port %d wasn't found"),port));
    }   

    rv = _pm4x10_pm_port_init(unit, port, phy_acc, port_index, NULL, 0, pm_info);
    _SOC_IF_ERR_EXIT(rv);

    /*deinit PM in case of last one*/
    if (is_last_one) {
       rv = _pm4x10_pm_disable(unit, port, pm_info, phy_acc);
       _SOC_IF_ERR_EXIT(rv);

       PHYMOD_DEVICE_OP_MODE_PCS_BYPASS_CLR(PM_4x10_INFO(pm_info)->int_core_access.device_op_mode);

#ifdef PORTMOD_PM12X10_SUPPORT
       if(PM_4x10_INFO(pm_info)->in_pm12x10) {
           uint32 phy;
           PORTMOD_PBMP_ITER(PM_4x10_INFO(pm_info)->phys, phy) {break;};
            /* this will be redirected to pm12x10 */
#ifdef PORTMOD_PM12X10_XGS_SUPPORT
            
            if (SOC_IS_APACHE(unit)) {
                rv = pm12x10_xgs_pm4x10_enable(unit, port, phy, 1);
                _SOC_IF_ERR_EXIT(rv);
            } else
#endif
            {
                rv = pm12x10_pm4x10_enable(unit, port, phy, 0);
                _SOC_IF_ERR_EXIT(rv);
            }
        }
#endif /* PORTMOD_PM12X10_SUPPORT */

       rv = SOC_WB_ENGINE_SET_VAR(unit, SOC_WB_ENGINE_PORTMOD, 
                             pm_info->wb_vars_ids[isActive], &inactive);
       _SOC_IF_ERR_EXIT(rv);

       rv = SOC_WB_ENGINE_SET_VAR(unit, SOC_WB_ENGINE_PORTMOD,
                                   pm_info->wb_vars_ids[isInitialized],
                                   &inactive);
       _SOC_IF_ERR_EXIT(rv);

       rv = SOC_WB_ENGINE_SET_VAR(unit, SOC_WB_ENGINE_PORTMOD,
                                   pm_info->wb_vars_ids[isBypassed], &inactive);
       _SOC_IF_ERR_EXIT(rv);
       /* Updating phymod type in pm_info structure and warmboot engine */
       phytype = phymodDispatchTypeCount;
       rv = SOC_WB_ENGINE_SET_ARR(unit, SOC_WB_ENGINE_PORTMOD, pm_info->wb_vars_ids[phy_type], &phytype, 0);
       _SOC_IF_ERR_EXIT(rv);
       PM_4x10_INFO(pm_info)->int_core_access.type = phymodDispatchTypeCount;
    }

exit:
    SOC_FUNC_RETURN;
}

int pm4x10_port_replace(int unit, int port, pm_info_t pm_info, int new_port)
{
    int i, tmp_port;
    int rv = 0;
    SOC_INIT_FUNC_DEFS;

    /* replace old port with new port */
    for (i = 0; i < MAX_PORTS_PER_PM4X10; i++) {
        rv = SOC_WB_ENGINE_GET_ARR(unit, SOC_WB_ENGINE_PORTMOD, 
                     pm_info->wb_vars_ids[ports], &tmp_port, i);
        _SOC_IF_ERR_EXIT(rv);
       
        if (tmp_port == port){
            rv = SOC_WB_ENGINE_SET_ARR(unit, SOC_WB_ENGINE_PORTMOD, 
                             pm_info->wb_vars_ids[ports], &new_port, i);
            _SOC_IF_ERR_EXIT(rv);                
        }
    }
        
exit:
    SOC_FUNC_RETURN; 
    
}

int pm4x10_pm_bypass_set(int unit, pm_info_t pm_info, int bypass_enable)
{
    int     rv = 0;
    uint32  pm_is_active = 0;
    uint32 is_bypass;

    SOC_INIT_FUNC_DEFS;

    rv = SOC_WB_ENGINE_GET_VAR(unit, SOC_WB_ENGINE_PORTMOD, pm_info->wb_vars_ids[isBypassed], &is_bypass);
    _SOC_IF_ERR_EXIT(rv);

    if (bypass_enable != is_bypass) {
        rv = SOC_WB_ENGINE_GET_VAR(unit, SOC_WB_ENGINE_PORTMOD, pm_info->wb_vars_ids[isActive], &pm_is_active);
        _SOC_IF_ERR_EXIT(rv);

        if(pm_is_active) {
            _SOC_EXIT_WITH_ERR(SOC_E_PARAM, (_SOC_MSG("can't chenge bypass mode for active pm")));
        }

        rv = SOC_WB_ENGINE_SET_VAR (unit, SOC_WB_ENGINE_PORTMOD, pm_info->wb_vars_ids[isBypassed], &bypass_enable);
        _SOC_IF_ERR_EXIT(rv);

    }


exit:
    SOC_FUNC_RETURN;
}

int pm4x10_pm_core_info_get(int unit, pm_info_t pm_info, int phyn, portmod_pm_core_info_t* core_info)
{
    core_info->ref_clk = PM_4x10_INFO(pm_info)->ref_clk;
    return SOC_E_NONE;
}

int pm4x10_pm_phys_get(int unit, pm_info_t pm_info, portmod_pbmp_t* phys)
{
    SOC_INIT_FUNC_DEFS;
    PORTMOD_PBMP_ASSIGN(*phys, PM_4x10_INFO(pm_info)->phys);
    SOC_FUNC_RETURN;
}

int pm4x10_port_enable_set (int unit, int port, pm_info_t pm_info, 
                            int flags, int enable)
{
    uint32  is_bypassed = 0, phychain_flag;
    int     rv = 0, rst_flags = 0;
    int     actual_flags = flags, nof_phys = 0, xlmac_flags = 0;
    phymod_phy_power_t           phy_power;
    phymod_phy_access_t phy_access[1+MAX_PHYN];

    SOC_INIT_FUNC_DEFS;

    /* If no Rx/Tx flags - set both */
    if ((!PORTMOD_PORT_ENABLE_TX_GET(flags)) && 
        (!PORTMOD_PORT_ENABLE_RX_GET(flags)))
    {
        PORTMOD_PORT_ENABLE_RX_SET(actual_flags);
        PORTMOD_PORT_ENABLE_TX_SET(actual_flags);
    }

    /* If no Mac/Phy flags - set both */
    if ((!PORTMOD_PORT_ENABLE_PHY_GET(flags)) &&
        (!PORTMOD_PORT_ENABLE_MAC_GET(flags)))
    {
        PORTMOD_PORT_ENABLE_PHY_SET(actual_flags);
        PORTMOD_PORT_ENABLE_MAC_SET(actual_flags);
    }

     /* if MAC is set and either RX or TX set is invalid combination */
    if( (PORTMOD_PORT_ENABLE_MAC_GET(actual_flags)) && (!PORTMOD_PORT_ENABLE_PHY_GET(actual_flags)) ) {
        if((!PORTMOD_PORT_ENABLE_TX_GET(actual_flags)) || (!PORTMOD_PORT_ENABLE_RX_GET(actual_flags))){
            _SOC_EXIT_WITH_ERR(SOC_E_PARAM, (_SOC_MSG("MAC RX and TX can't be enabled separately")));
        }
    }

    rv = SOC_WB_ENGINE_GET_VAR(unit, SOC_WB_ENGINE_PORTMOD, 
                               pm_info->wb_vars_ids[isBypassed], &is_bypassed);
    _SOC_IF_ERR_EXIT(rv);

    _SOC_IF_ERR_EXIT(phymod_phy_power_t_init(&phy_power));
    phy_power.rx = phymodPowerNoChange;
    phy_power.tx = phymodPowerNoChange;

    if(PORTMOD_PORT_ENABLE_RX_GET(actual_flags)){
        phy_power.rx = (enable) ? phymodPowerOn : phymodPowerOff;
        xlmac_flags |= XLMAC_ENABLE_SET_FLAGS_RX_EN;
    }
    if(PORTMOD_PORT_ENABLE_TX_GET(actual_flags)){
        phy_power.tx = (enable) ? phymodPowerOn : phymodPowerOff;
        xlmac_flags |= XLMAC_ENABLE_SET_FLAGS_TX_EN;
    }

    /* phychain_flag is meant to be used if we want only part of the phychin to be affected*/
    phychain_flag = PORTMOD_PORT_ENABLE_INTERNAL_PHY_ONLY_GET(flags) ? 
        PORTMOD_INIT_F_INTERNAL_SERDES_ONLY : 
        PORTMOD_INIT_F_EXTERNAL_MOST_ONLY ;

    if(enable){
        if((PORTMOD_PORT_ENABLE_MAC_GET(actual_flags)) && (!is_bypassed)){
              if(PM_4x10_INFO(pm_info)->portmod_mac_soft_reset) {
                rst_flags |= XLMAC_ENABLE_SET_FLAGS_SOFT_RESET_DIS;
            }    
            rv = xlmac_enable_set(unit, port, rst_flags, 1);
            _SOC_IF_ERR_EXIT(rv);
              if(PM_4x10_INFO(pm_info)->portmod_mac_soft_reset) {
                _SOC_IF_ERR_EXIT(pm4x10_port_soft_reset(unit, port, pm_info, 0));
            }
        }

        if(PORTMOD_PORT_ENABLE_PHY_GET(actual_flags))
        {
            _SOC_IF_ERR_EXIT
                (portmod_port_chain_phy_access_get(unit, port, pm_info, 
                                                  phy_access ,(1+MAX_PHYN),
                                                  &nof_phys));

            _SOC_IF_ERR_EXIT
                (portmod_port_phychain_power_set(unit, port, phy_access, nof_phys, phychain_flag, &phy_power));

            if (PORTMOD_PORT_ENABLE_TX_GET(actual_flags)) {
                _SOC_IF_ERR_EXIT
                (portmod_port_phychain_tx_lane_control_set(unit, port, phy_access, nof_phys, phychain_flag,
                                                       phymodTxSquelchOff));
            }

            if(PORTMOD_PORT_ENABLE_RX_GET(actual_flags)) {
                _SOC_IF_ERR_EXIT
                (portmod_port_phychain_rx_lane_control_set(unit, port, phy_access, nof_phys, phychain_flag,
                                                      phymodRxSquelchOff));  
            }

        }
    } else {
        if (PORTMOD_PORT_ENABLE_PHY_GET(actual_flags)) {
            _SOC_IF_ERR_EXIT
                (portmod_port_chain_phy_access_get(unit, port, pm_info, 
                                                  phy_access ,(1+MAX_PHYN),
                                                   &nof_phys));

            if (PORTMOD_PORT_ENABLE_TX_GET(actual_flags)) {
                _SOC_IF_ERR_EXIT
                (portmod_port_phychain_tx_lane_control_set(unit, port, phy_access, nof_phys, phychain_flag,
                                                       phymodTxSquelchOn));
            }

            if (PORTMOD_PORT_ENABLE_RX_GET(actual_flags)) {
                _SOC_IF_ERR_EXIT
                (portmod_port_phychain_rx_lane_control_set(unit, port, phy_access, nof_phys, phychain_flag,
                                                      phymodRxSquelchOn));  
            }

            _SOC_IF_ERR_EXIT
                (portmod_port_phychain_power_set(unit, port, phy_access, nof_phys, phychain_flag, &phy_power));
        }

        /* if((PORTMOD_PORT_ENABLE_MAC_GET(actual_flags))  && (!is_bypassed)){ */
        /* Not checking is_bypassed as if clmac was enable before bypass was set */
        if((PORTMOD_PORT_ENABLE_MAC_GET(actual_flags)) ){
            rv = xlmac_enable_set(unit, port, xlmac_flags, 0);
            _SOC_IF_ERR_EXIT(rv);
        }
    }

exit:
    SOC_FUNC_RETURN;
}

int pm4x10_port_enable_get (int unit, int port, pm_info_t pm_info, int flags, 
                            int* enable)
{
    uint32  is_bypassed = 0, phychain_flag;
    phymod_phy_access_t phy_access[1+MAX_PHYN];
    int nof_phys = 0;
    phymod_phy_tx_lane_control_t tx_control = phymodTxSquelchOn;
    phymod_phy_rx_lane_control_t rx_control = phymodRxSquelchOn;
    int phy_enable = 1, mac_enable = 1;
    int mac_rx_enable = 0, mac_tx_enable = 0;
    int actual_flags = flags;
    SOC_INIT_FUNC_DEFS;
    SOC_NULL_CHECK(pm_info);

    /* If no RX\TX flags - set both*/
    if((!PORTMOD_PORT_ENABLE_TX_GET(flags)) && (!PORTMOD_PORT_ENABLE_RX_GET(flags))){
        PORTMOD_PORT_ENABLE_RX_SET(actual_flags);
        PORTMOD_PORT_ENABLE_TX_SET(actual_flags);
    }

    /* if no MAC\Phy flags - set both*/
    if((!PORTMOD_PORT_ENABLE_PHY_GET(flags)) && (!PORTMOD_PORT_ENABLE_MAC_GET(flags))){
        PORTMOD_PORT_ENABLE_PHY_SET(actual_flags);
        PORTMOD_PORT_ENABLE_MAC_SET(actual_flags);
    }

    /* phychain_flag is meant to be used if we want only part of the phychin to be affected*/
    phychain_flag = PORTMOD_PORT_ENABLE_INTERNAL_PHY_ONLY_GET(flags) ? 
        PORTMOD_INIT_F_INTERNAL_SERDES_ONLY : 
        PORTMOD_INIT_F_EXTERNAL_MOST_ONLY ;

    if (PORTMOD_PORT_ENABLE_PHY_GET(actual_flags)) {

        _SOC_IF_ERR_EXIT
            (portmod_port_chain_phy_access_get(unit, port, pm_info, phy_access ,(1+MAX_PHYN), &nof_phys));

        _SOC_IF_ERR_EXIT
            (portmod_port_phychain_tx_lane_control_get(unit, port, phy_access, nof_phys, phychain_flag, &tx_control));
        _SOC_IF_ERR_EXIT
            (portmod_port_phychain_rx_lane_control_get(unit, port, phy_access, nof_phys, phychain_flag, &rx_control));

        phy_enable = 0;

        if (PORTMOD_PORT_ENABLE_RX_GET(actual_flags)) {
            phy_enable |= (rx_control == phymodRxSquelchOn) ? 0 : 1;
        } 
        if (PORTMOD_PORT_ENABLE_TX_GET(actual_flags)) {
            phy_enable |= (tx_control == phymodTxSquelchOn) ? 0 : 1;
        }
    } 

    _SOC_IF_ERR_EXIT(SOC_WB_ENGINE_GET_VAR(unit, SOC_WB_ENGINE_PORTMOD, 
                     pm_info->wb_vars_ids[isBypassed], &is_bypassed));

    if ((PORTMOD_PORT_ENABLE_MAC_GET(actual_flags)) && (!is_bypassed)) {

        mac_enable = 0;

        if (PORTMOD_PORT_ENABLE_RX_GET(actual_flags)) {
            _SOC_IF_ERR_EXIT(xlmac_enable_get(unit, port, XLMAC_ENABLE_SET_FLAGS_RX_EN, &mac_rx_enable));
            mac_enable |= (mac_rx_enable) ? 1 : 0;
        }
        if (PORTMOD_PORT_ENABLE_TX_GET(actual_flags)) {
            _SOC_IF_ERR_EXIT(xlmac_enable_get(unit, port, XLMAC_ENABLE_SET_FLAGS_TX_EN, &mac_tx_enable));
            mac_enable |= (mac_tx_enable) ? 1 : 0;
        }
    }

    *enable = (mac_enable && phy_enable);

exit:
    SOC_FUNC_RETURN;
}

int pm4x10_port_interface_config_set(int unit, int port, 
                        pm_info_t pm_info, 
                        const portmod_port_interface_config_t* config,
                        int phy_init_flags)
{
    uint32 reg_val;
    uint32 pm_is_bypassed = 0, pm_is_initialized = 0; 
    int    nof_phys = 0, rv = 0, phy_acc, flags = 0;
    phymod_phy_access_t phy_access_arr[1+MAX_PHYN];
    phymod_phy_access_t         phy_access;
    phymod_phy_inf_config_t     phy_interface_config;
    portmod_access_get_params_t params;
    int                         port_index;
    uint32_t                    bitmap;
    portmod_port_interface_config_t  config_temp ;
    phymod_interface_t   phymod_interface;
    phymod_interface_t   phymod_serdes_interface = phymodInterfaceCount;
    phymod_tx_t          updated_tx_params[PHYMOD_MAX_LANES_PER_CORE];
    phymod_tx_t          tx_params[PHYMOD_MAX_LANES_PER_CORE];
    phymod_tx_t          ext_phy_tx_params[PHYMOD_MAX_LANES_PER_CORE];
    soc_port_if_t        interface = SOC_PORT_IF_NULL;
    int is_legacy_phy = 0;
    int i, xphy_id;
    int is_higig, rxaui_mode, fs_cl72;
    uint32 is_interface_restore, interface_modes;
#ifdef PORTMOD_PM12X10_SUPPORT
    soc_100g_lane_config_t          lane_config=0;
#endif
    SOC_INIT_FUNC_DEFS;

    /* 
     * Ignore the call to interface config set if the pm is not iniialized.
     * This happens if the legacy external phy does a call back prior to initializing
     * and probing internal phys 
     */  
    rv = SOC_WB_ENGINE_GET_VAR (unit, SOC_WB_ENGINE_PORTMOD, 
                                pm_info->wb_vars_ids[isInitialized], 
                                &pm_is_initialized);
    _SOC_IF_ERR_EXIT(rv);
    if (!pm_is_initialized) return (SOC_E_NONE);


    PM4x10_PHY_ACCESS_GET(unit, port, pm_info, phy_acc);

    _SOC_IF_ERR_EXIT(_pm4x10_port_index_get (unit, port, pm_info, &port_index,
                                             &bitmap));

    is_higig =  ((config->encap_mode == SOC_ENCAP_HIGIG2) ||
                 (config->encap_mode == SOC_ENCAP_HIGIG) ||
                 PHYMOD_INTF_MODES_HIGIG_GET(config)) ? 1 : 0;

    if (config->flags & PHYMOD_INTF_F_UPDATE_SPEED_LINKUP) {
        int mac_speed = 0, serdes_speed = 0, mac_only = 0;
        int nof_lanes = SOC_INFO(unit).port_num_lanes[port];

        _SOC_IF_ERR_EXIT(portmod_port_speed_get(unit, port, &serdes_speed));
        if (config->speed == serdes_speed) {
            xlmac_speed_get(unit, port, &mac_speed);
            mac_speed *= nof_lanes;
            if (config->speed == mac_speed) {
                SOC_FUNC_RETURN;
            }

            mac_only = 1;
        }

        flags = XLMAC_SPEED_SET_FLAGS_SOFT_RESET_DIS;
        rv = xlmac_speed_set(unit, port, flags, config->speed);

        if (mac_only) {
            /* line side speed and SerDes speed are equal, no need update SerDes configuration */
            SOC_FUNC_RETURN;
        }

        phy_init_flags = PORTMOD_INIT_F_ALL_PHYS;
    }

    if ((config->encap_mode == SOC_ENCAP_HIGIG2) ||
        (config->encap_mode == SOC_ENCAP_HIGIG)  ||
        (PHYMOD_INTF_MODES_HIGIG_GET(config))) {
        rv = PM4x10_INTERFACE_MODES_GET(unit, pm_info, &interface_modes,
                                        port_index);
        _SOC_IF_ERR_EXIT(rv);
        interface_modes |= PHYMOD_INTF_MODES_HIGIG;
        rv = PM4x10_INTERFACE_MODES_SET(unit, pm_info, interface_modes,
                                        port_index);
        _SOC_IF_ERR_EXIT(rv);
    } else if ((config->encap_mode == SOC_ENCAP_IEEE)) {
        rv = PM4x10_INTERFACE_MODES_GET(unit, pm_info, &interface_modes, port_index);
        _SOC_IF_ERR_EXIT(rv);
        interface_modes &= ~PHYMOD_INTF_MODES_HIGIG;
        rv = PM4x10_INTERFACE_MODES_SET(unit, pm_info, interface_modes, port_index);
        _SOC_IF_ERR_EXIT(rv);
    }

    _SOC_IF_ERR_EXIT(portmod_access_get_params_t_init(unit, &params));
    params.phyn = 0;
    params.lane = -1;
    params.sys_side = PORTMOD_SIDE_LINE;

    _SOC_IF_ERR_EXIT(pm4x10_port_phy_lane_access_get(unit, port, pm_info,
                                    &params, 1, &phy_access, &nof_phys, NULL));

    _SOC_IF_ERR_EXIT
        (portmod_port_chain_phy_access_get(unit, port, pm_info,
            phy_access_arr ,(1+MAX_PHYN), &nof_phys));

    rv = SOC_WB_ENGINE_GET_VAR (unit, SOC_WB_ENGINE_PORTMOD, 
                                pm_info->wb_vars_ids[isBypassed], 
                                &pm_is_bypassed);
    _SOC_IF_ERR_EXIT(rv);

    if(config->flags & PHYMOD_INTF_F_INTF_PARAM_SET_ONLY) {
        /* Update the software state of the interface type. Note that currently
         * the legacy ext PHY does not raise the PHYMOD_INTF_F_INTF_PARAM_SET_ONLY
         * flag so that the following function call cannot be reached when a legacy
         * ext PHY uses the portmod callback function to set the interface type
         * of the internal PHY.
         */
        rv = PM4x10_INTERFACE_TYPE_SET(unit, pm_info, config->interface,
                                       port_index);
        _SOC_IF_ERR_EXIT(rv);

        is_interface_restore = 1;
        rv = PM4x10_IS_INTERFACE_RESTORE_SET(unit, pm_info,
                                             is_interface_restore, port_index);
        _SOC_IF_ERR_EXIT(rv);

        SOC_FUNC_RETURN;
    }

    rv = PM4x10_IS_INTERFACE_RESTORE_GET(unit, pm_info, &is_interface_restore,
                                             port_index);
    _SOC_IF_ERR_EXIT(rv);
    /*
         * if the interface is already set with PARAM_SET_ONLY
         * h/w interface needs to be updated during speed set.
         * So load the interface information and clear the flag;
         */
    if(is_interface_restore) {
        rv = PM4x10_INTERFACE_TYPE_GET(unit, pm_info, &interface, port_index);
        _SOC_IF_ERR_EXIT(rv);
        ((portmod_port_interface_config_t*)config)->interface = interface;

        is_interface_restore = 0;
        rv = PM4x10_IS_INTERFACE_RESTORE_SET(unit, pm_info,
                                       is_interface_restore, port_index);
        _SOC_IF_ERR_EXIT(rv);
    }

    if (!(config->flags & (PHYMOD_INTF_F_SET_SPD_TRIGGER | PHYMOD_INTF_F_UPDATE_SPEED_LINKUP)) && (!pm_is_bypassed)){

        rv = PM4x10_INTERFACE_MODES_GET(unit, pm_info, &interface_modes,
                                        port_index);
        _SOC_IF_ERR_EXIT(rv);
        /* set HiG mode */
        _SOC_IF_ERR_EXIT(READ_XLPORT_CONFIGr(unit, phy_acc, &reg_val));
        soc_reg_field_set (unit, XLPORT_CONFIGr, &reg_val, HIGIG2_MODEf, 
                           (interface_modes & PHYMOD_INTF_MODES_HIGIG)? 1: 0);
        _SOC_IF_ERR_EXIT(WRITE_XLPORT_CONFIGr(unit, phy_acc, reg_val));

        /* set port speed */
        flags = XLMAC_SPEED_SET_FLAGS_SOFT_RESET_DIS;
        rv = xlmac_speed_set(unit, port, flags, config->speed);
        _SOC_IF_ERR_EXIT(rv);

        /* set encapsulation */
        flags = XLMAC_ENCAP_SET_FLAGS_SOFT_RESET_DIS;
        rv = xlmac_encap_set (unit, port, flags, config->encap_mode);
        _SOC_IF_ERR_EXIT(rv);

        rv = xlmac_strict_preamble_set(unit, port, (!is_higig && (config->speed >= 10000)));
        _SOC_IF_ERR_EXIT(rv);

        /* De-Assert SOFT_RESET */
        rv = pm4x10_port_soft_reset(unit, port, pm_info, 0);
        _SOC_IF_ERR_EXIT(rv);
    }

    _SOC_IF_ERR_EXIT(phymod_phy_inf_config_t_init (&phy_interface_config));

    _SOC_IF_ERR_EXIT(PM4x10_RXAUI_MODE_GET(unit, pm_info,
                                           &rxaui_mode, port_index));

    _SOC_IF_ERR_EXIT(_pm4x10_line_side_phymod_interface_get(unit, port,
                                                            pm_info, rxaui_mode,
                                                            config,
                                                            &phymod_interface));
    phy_interface_config.interface_type = phymod_interface;
    _SOC_IF_ERR_EXIT(_pm4x10_phymod_interface_mode_get(unit, port, pm_info, config, &phy_interface_config));
    _SOC_IF_ERR_EXIT(PM4x10_SPEED_SET(unit, pm_info,
                                      config->speed, port_index));

    phy_interface_config.data_rate       = config->speed;
    phy_interface_config.pll_divider_req = config->pll_divider_req;
    phy_interface_config.ref_clock       = PM_4x10_INFO(pm_info)->ref_clk;

    /*
     * copy const *config to config_temp cause
     * config.interface may come from storage
     */
    _SOC_IF_ERR_EXIT(portmod_port_interface_config_t_init(unit, &config_temp));
    sal_memcpy(&config_temp, config, sizeof(portmod_port_interface_config_t));

    rv = PM4x10_IS_INTERFACE_RESTORE_GET(unit, pm_info,
                                         &is_interface_restore, port_index);
    _SOC_IF_ERR_EXIT(rv);

    if(is_interface_restore) {
        /* overwrite config->interface form storage */
         rv = PM4x10_INTERFACE_TYPE_GET(unit, pm_info, &interface, port_index);
         _SOC_IF_ERR_EXIT(rv);
        config_temp.interface  = interface ;
        is_interface_restore = 0;
        rv = PM4x10_IS_INTERFACE_RESTORE_SET(unit, pm_info,
                                          is_interface_restore, port_index);
        _SOC_IF_ERR_EXIT(rv);
    }

    _SOC_IF_ERR_EXIT(PM4x10_FS_CL72_GET(unit, pm_info, &fs_cl72, port_index));
    if(fs_cl72) {
        /*
         * PHYMOD_INTF_F_CL72_REQUESTED_BY_CNFG config is only honoured
         * by tsce. incase if the external phys start using this flag,
         * cl72 should not be set for the external phys. Because port_init_cl72
         * should enable cl72 only on the serdes.
         */
        config_temp.flags |= PHYMOD_INTF_F_CL72_REQUESTED_BY_CNFG ;
    }

    if( nof_phys > 1 ){
        /* external phy preemphasis and current */
        if (!(config->flags & (PHYMOD_INTF_F_SET_SPD_TRIGGER | PHYMOD_INTF_F_UPDATE_SPEED_LINKUP))) {
            xphy_id = phy_access_arr[nof_phys-1].access.addr;
            _SOC_IF_ERR_EXIT(portmod_xphy_is_legacy_phy_get(unit, xphy_id, &is_legacy_phy));
            if (!is_legacy_phy) {
                for(i=0; i < PHYMOD_MAX_LANES_PER_CORE; i++){
                    phymod_tx_t_init(&(tx_params[i]));
                    _SOC_IF_ERR_EXIT(PM4x10_EXT_PHY_TX_PARAMS_GET(unit, pm_info,
                                                     &ext_phy_tx_params[i], i));
                }
                _SOC_IF_ERR_EXIT(
                    portmod_port_phychain_tx_set(unit, &phy_access_arr[nof_phys-1],
                                                 1, ext_phy_tx_params));
            }
        }
    }

#ifdef PORTMOD_PM12X10_SUPPORT
    if ((nof_phys > 1) &&  (config->interface_modes & PHYMOD_INTF_MODES_TRIPLE_CORE)) {
        /* external phy TRI-CORE MODE */
        xphy_id = phy_access_arr[nof_phys-1].access.addr;
        _SOC_IF_ERR_EXIT(portmod_xphy_is_legacy_phy_get(unit, xphy_id, &is_legacy_phy));
        if((SOC_INFO(unit).port_speed_max[port] >= 100000) && !is_legacy_phy) {
            /* read the portmap config to get the map info */
            lane_config = SOC_INFO(unit).port_100g_lane_config[port];
            /* convert SOC level lane config to interface mode setting */
            switch(lane_config) {
                case SOC_LANE_CONFIG_100G_4_4_2:
                    phy_interface_config.interface_modes |= PHYMOD_INTF_MODES_TC_442; 
                    break;
                case SOC_LANE_CONFIG_100G_3_4_3:
                    phy_interface_config.interface_modes |= PHYMOD_INTF_MODES_TC_343; 
                    break;
                case SOC_LANE_CONFIG_100G_2_4_4:
                    phy_interface_config.interface_modes |= PHYMOD_INTF_MODES_TC_244; 
                default:
                    break;
            }
        }
    }
#endif

    /* internal phy preemphasis and current */
    if (!(config->flags & (PHYMOD_INTF_F_SET_SPD_TRIGGER | PHYMOD_INTF_F_UPDATE_SPEED_LINKUP))) {
        uint32 port_dynamic_state;
        /* if the tx params are updated from non default value read the non dflt value from the HW and update it */

        rv = PM4x10_PORT_DYNAMIC_STATE_GET(unit, pm_info,
                                      &port_dynamic_state, port_index);
        _SOC_IF_ERR_EXIT(rv);
        if (port_dynamic_state) {
            for(i=0; i<PHYMOD_MAX_LANES_PER_CORE; i++){
                phymod_tx_t_init(&(updated_tx_params[i]));
            }
            _SOC_IF_ERR_EXIT(portmod_port_phychain_tx_get(unit, phy_access_arr, 1, updated_tx_params));
            _SOC_IF_ERR_EXIT(portmod_port_phychain_tx_set(unit, phy_access_arr, 1, updated_tx_params));
        } else {
            for(i=0; i < PHYMOD_MAX_LANES_PER_CORE; i++){
                phymod_tx_t_init(&(tx_params[i]));
                _SOC_IF_ERR_EXIT(PM4x10_TX_PARAMS_GET(unit, pm_info,
                                                      &tx_params[i], i));
            }
            _SOC_IF_ERR_EXIT(
                portmod_port_phychain_tx_set(unit, phy_access_arr, 1, tx_params));
        }
    }

    if (config->serdes_interface != SOC_PORT_IF_NULL) {
        _SOC_IF_ERR_EXIT(portmod_intf_to_phymod_intf(unit, 
                        config->speed,  
                        config->serdes_interface, 
                        &phymod_serdes_interface));
        _SOC_IF_ERR_EXIT(PM4x10_SERDES_INTERFACE_TYPE_SET(unit, pm_info,
                                                          config->serdes_interface,
                                                          port_index));
    }
    _SOC_IF_ERR_EXIT
        (portmod_port_phychain_interface_config_set(unit, port, phy_access_arr, nof_phys,
                                                config_temp.flags,
                                                &phy_interface_config, 
                                                phymod_serdes_interface, 
                                                PM_4x10_INFO(pm_info)->ref_clk,
                                                phy_init_flags));

    /* The interface type software state needs to be stored even if the config flags
     * does not include PHYMOD_INTF_F_INTF_PARAM_SET_ONLY. For example, when changing
     * the encap, portctrl tries to config the PHY interface type and speed in one
     * shot without setting the PHYMOD_INTF_F_INTF_PARAM_SET_ONLY flag. However,
     * when the current function is called by the legacy ext PHY through the portmod
     * callback function, the interface type software state should not be modified
     * since PM4x10_INTERFACE_TYPE_GET() should return the outmost PHY interface type. 
     */
    if (!(nof_phys > 1 && phy_init_flags == PORTMOD_INIT_F_INTERNAL_SERDES_ONLY)) {
        _SOC_IF_ERR_EXIT
            (portmod_intf_from_phymod_intf(unit, phy_interface_config.interface_type, &interface));
        _SOC_IF_ERR_EXIT
            (PM4x10_INTERFACE_TYPE_SET(unit, pm_info, interface, port_index));
        _SOC_IF_ERR_EXIT
            (PM4x10_OUTMOST_PHY_INTERFACE_TYPE_SET(unit, pm_info, interface, port_index));
    }

exit:
    SOC_FUNC_RETURN;
}

int pm4x10_port_interface_config_get (int unit, int port, pm_info_t pm_info, 
                                      portmod_port_interface_config_t* config, int phy_init_flags)
{
    phymod_phy_access_t phy_access[1+MAX_PHYN];
    phymod_phy_inf_config_t      phy_interface_config;
    int                          port_index, pll_divider_req;
    uint32_t                     bitmap;
    int flags=0, nof_phys = 0, rv = 0, ref_clock, max_speed;
    phymod_interface_t           prev_phymod_if;
    soc_port_if_t   interface = SOC_PORT_IF_NULL;
    
    SOC_INIT_FUNC_DEFS;
    
    ref_clock = PM_4x10_INFO(pm_info)->ref_clk;

    _SOC_IF_ERR_EXIT(_pm4x10_port_index_get (unit, port, pm_info, &port_index,
                                             &bitmap));

    _SOC_IF_ERR_EXIT(phymod_phy_inf_config_t_init(&phy_interface_config));

    _SOC_IF_ERR_EXIT
        (portmod_port_chain_phy_access_get(unit, port, pm_info, 
                                          phy_access ,(1+MAX_PHYN),
                                           &nof_phys));

    rv = PM4x10_MAX_SPEED_GET(unit, pm_info, &max_speed, port_index);
    _SOC_IF_ERR_EXIT(rv);

    rv = PM4x10_OUTMOST_PHY_INTERFACE_TYPE_GET(unit, pm_info, &interface, port_index);
    _SOC_IF_ERR_EXIT(rv);

    phy_interface_config.data_rate =  max_speed;
    phy_interface_config.ref_clock =  ref_clock;

    _SOC_IF_ERR_EXIT( portmod_intf_to_phymod_intf(unit, max_speed, interface,
                                                  &prev_phymod_if));

    _SOC_IF_ERR_EXIT
        (portmod_port_phychain_interface_config_get(unit, port, phy_access, nof_phys,
                                                    0, ref_clock,
                                                    &phy_interface_config, phy_init_flags));

    /* It is possible that the underlying phymod interface get API does not
     * distinguish among certain interface types. For example, CR2 and KR2
     * are treated the same from TSCE's perspective. In these cases portmod
     * has to rely on the software state to retrieve the most-recently used
     * interface type written into phymod.
     */
    if (phy_interface_config.interface_type != prev_phymod_if &&
        prev_phymod_if != phymodInterfaceCount) {
        phy_interface_config.interface_type = prev_phymod_if;
    }

    config->speed           = phy_interface_config.data_rate;
    config->interface_modes = phy_interface_config.interface_modes;
    config->flags           = 0;
    config->interface       = interface;

    rv = PM4x10_PORT_PLL_DIVIDER_REQ_GET(unit, pm_info,
                                         &pll_divider_req,
                                         port_index);

    config->pll_divider_req = pll_divider_req;

    _SOC_IF_ERR_EXIT(
                PM4x10_SERDES_INTERFACE_TYPE_GET(unit, pm_info,
                                                 &config->serdes_interface,
                                                 port_index));

#if 1
    rv = portmod_intf_from_phymod_intf (unit, 
             phy_interface_config.interface_type, &(config->interface));
    _SOC_IF_ERR_EXIT(rv);
#endif

    rv = xlmac_encap_get (unit, port, &flags, &config->encap_mode); 
    _SOC_IF_ERR_EXIT(rv);
                          
    rv = PM4x10_PORT_OP_MODE_GET(unit, pm_info, &config->port_op_mode,
                                 port_index);
    _SOC_IF_ERR_EXIT(rv);

    config->port_num_lanes = SOC_INFO(unit).port_num_lanes[port];

    rv = PM4x10_MAX_SPEED_GET(unit, pm_info, &config->max_speed, port_index);
    _SOC_IF_ERR_EXIT(rv);

    rv = PM4x10_INTERFACE_MODES_GET(unit, pm_info, &config->interface_modes,
                                    port_index);
    _SOC_IF_ERR_EXIT(rv);



exit:
    SOC_FUNC_RETURN;
}

int pm4x10_port_default_interface_get(int unit, int port, pm_info_t pm_info,
                                      const portmod_port_interface_config_t* config,
                                      soc_port_if_t* interface)
{
    phymod_phy_access_t phy_access[1+MAX_PHYN];
    int nof_phys = 0;
    int is_legacy_present, xphy_id;
    portmod_port_interface_config_t interface_config;
    SOC_INIT_FUNC_DEFS;

    _SOC_IF_ERR_EXIT(portmod_port_chain_phy_access_get(unit, port, pm_info,
                                           phy_access ,(1+MAX_PHYN),
                                           &nof_phys));
    if (nof_phys==1) {
        _SOC_IF_ERR_EXIT
            (_pm4x10_default_interface_get(unit, port, config, interface));
    } else {
        xphy_id = phy_access[nof_phys - 1].access.addr;
        _SOC_IF_ERR_EXIT(portmod_xphy_is_legacy_phy_get(unit, xphy_id, &is_legacy_present));

        if (!is_legacy_present) {
            _SOC_IF_ERR_EXIT
                (portmod_port_phymod_xphy_default_interface_get(unit, &(phy_access[nof_phys-1]),
                                                                config, interface));
        } else {
            interface_config = *config;
            _SOC_IF_ERR_EXIT
                (portmod_common_default_interface_get(&interface_config));

            /* A TD2P+G28 40G/42G HG2 port is of XLAUI interface type on the
             * line side historically. On the system side, the int PHY should
             * be of BRCM mode (XGMII) and this is taken care by
             * portmod_port_line_to_sys_intf_map().
             */
            if ((config->encap_mode == SOC_ENCAP_HIGIG || 
                 config->encap_mode == SOC_ENCAP_HIGIG2) &&
                (config->speed == 40000 || 
                 config->speed == 42000)) {
                interface_config.interface = SOC_PORT_IF_XLAUI; 
            }

            *interface = interface_config.interface;
        }
    }

exit:
    SOC_FUNC_RETURN;
}

int pm4x10_port_cl72_set(int unit, int port, pm_info_t pm_info, uint32 enable)
{
    phymod_phy_access_t phy_access[1+MAX_PHYN];
    int   nof_phys = 0;

    SOC_INIT_FUNC_DEFS;

    _SOC_IF_ERR_EXIT
        (portmod_port_chain_phy_access_get(unit, port, pm_info, 
                                           phy_access ,(1+MAX_PHYN),
                                           &nof_phys));

    _SOC_IF_ERR_EXIT
        (portmod_port_phychain_cl72_set(unit, port, phy_access, nof_phys, enable));

exit:
    SOC_FUNC_RETURN;
}
int pm4x10_port_cl72_get(int unit, int port, pm_info_t pm_info, uint32* enable)
{
    phymod_phy_access_t phy_access[1+MAX_PHYN];
    int   nof_phys = 0;

    SOC_INIT_FUNC_DEFS;

    _SOC_IF_ERR_EXIT
        (portmod_port_chain_phy_access_get(unit, port, pm_info, 
                                           phy_access ,(1+MAX_PHYN),
                                           &nof_phys));

    _SOC_IF_ERR_EXIT
        (portmod_port_phychain_cl72_get(unit, phy_access, nof_phys, enable));

exit:
    SOC_FUNC_RETURN;
}

int pm4x10_port_cl72_status_get(int unit, int port, pm_info_t pm_info, phymod_cl72_status_t* status)
{
    phymod_phy_access_t phy_access[1+MAX_PHYN];
    int   nof_phys = 0;

    SOC_INIT_FUNC_DEFS;

    _SOC_IF_ERR_EXIT
        (portmod_port_chain_phy_access_get(unit, port, pm_info, 
                                           phy_access ,(1+MAX_PHYN),
                                           &nof_phys));

    _SOC_IF_ERR_EXIT
        (portmod_port_phychain_cl72_status_get(unit, phy_access, nof_phys, status));

exit:
    SOC_FUNC_RETURN;
}


int pm4x10_port_loopback_get(int unit, int port, pm_info_t pm_info, 
                 portmod_loopback_mode_t loopback_type, int* enable)
{
    phymod_phy_access_t phy_access[1+MAX_PHYN];
    int                 nof_phys;
    uint32_t tmp_enable=0;
    phymod_loopback_mode_t phymod_lb_type;
    int rv = PHYMOD_E_NONE;
    int port_index;
    uint32 bitmap;

    SOC_INIT_FUNC_DEFS;

    _SOC_IF_ERR_EXIT(_pm4x10_port_index_get (unit, port, pm_info, &port_index,
                                             &bitmap));

    switch(loopback_type){
        case portmodLoopbackMacOuter:
            _SOC_IF_ERR_EXIT(xlmac_loopback_get(unit, port, enable));
            break;

        case portmodLoopbackPhyRloopPCS:
        case portmodLoopbackPhyGloopPMD:
        case portmodLoopbackPhyRloopPMD: /*slide*/
        case portmodLoopbackPhyGloopPCS: /*slide*/
             if(PM_4x10_INFO(pm_info)->nof_phys[port_index] == 0) {
                (*enable) = 0; /* No phy --> no phy loopback*/
             } else {
                _SOC_IF_ERR_EXIT(portmod_commmon_portmod_to_phymod_loopback_type(unit, 
                                                loopback_type, &phymod_lb_type));
                _SOC_IF_ERR_EXIT
                    (portmod_port_chain_phy_access_get(unit, port, pm_info, 
                                                      phy_access ,(1+MAX_PHYN),
                                                      &nof_phys));

                rv = portmod_port_phychain_loopback_get(unit, port, phy_access,
                                                    nof_phys,
                                                    phymod_lb_type,
                                                    &tmp_enable);
                if (rv == PHYMOD_E_UNAVAIL) {
                    rv = PHYMOD_E_NONE;
                    tmp_enable = 0;
                }

                _SOC_IF_ERR_EXIT(rv);
                *enable = tmp_enable;

             }
             break;
        default:
            (*enable) = 0; /* not supported --> no loopback */
            break;
    }

exit:
    SOC_FUNC_RETURN;
}

int pm4x10_port_loopback_set (int unit, int port, pm_info_t pm_info, 
                  portmod_loopback_mode_t loopback_type, int enable)
{
    phymod_phy_access_t phy_access[1+MAX_PHYN];
    int                 nof_phys;
    phymod_loopback_mode_t phymod_lb_type;
    int port_index;
    uint32 bitmap;

    SOC_INIT_FUNC_DEFS;

    _SOC_IF_ERR_EXIT(_pm4x10_port_index_get (unit, port, pm_info, &port_index,
                                             &bitmap));

    /* loopback type validation*/
    switch(loopback_type){
    case portmodLoopbackMacOuter:
        _SOC_IF_ERR_EXIT(xlmac_loopback_set(unit, port, enable));
        break;

    case portmodLoopbackPhyRloopPCS:
    case portmodLoopbackPhyRloopPMD: /*slide*/
    case portmodLoopbackPhyGloopPMD:
    case portmodLoopbackPhyGloopPCS: /*slide*/
        if(PM_4x10_INFO(pm_info)->nof_phys[port_index] != 0) {
            if (enable) {
                _SOC_IF_ERR_EXIT(xlmac_loopback_set(unit, port, 0));
            }
            _SOC_IF_ERR_EXIT(portmod_commmon_portmod_to_phymod_loopback_type(unit, 
                                                loopback_type, &phymod_lb_type));
            
            _SOC_IF_ERR_EXIT
                (portmod_port_chain_phy_access_get(unit, port, pm_info, 
                                                  phy_access ,(1+MAX_PHYN),
                                                  &nof_phys));

            _SOC_IF_ERR_EXIT(portmod_port_phychain_loopback_set(unit, port,  phy_access, 
                                                                nof_phys, 
                                                                phymod_lb_type, 
                                                                enable));
         }
         break;
    default:
        _SOC_EXIT_WITH_ERR(SOC_E_UNAVAIL, (
                  _SOC_MSG("unsupported loopback type %d"), loopback_type));
            break;
    }
    if (enable)
        _SOC_IF_ERR_EXIT(PM_DRIVER(pm_info)->f_portmod_port_lag_failover_status_toggle(unit, port, pm_info));

exit:
    SOC_FUNC_RETURN; 
}

int pm4x10_port_rx_mac_enable_set(int unit, int port, pm_info_t pm_info, int enable)
{
    SOC_INIT_FUNC_DEFS;

    _SOC_IF_ERR_EXIT(xlmac_rx_enable_set(unit, port, enable));

exit:
    SOC_FUNC_RETURN;

}

int pm4x10_port_rx_mac_enable_get(int unit, int port, pm_info_t pm_info, int* enable)
{
    SOC_INIT_FUNC_DEFS;

    _SOC_IF_ERR_EXIT(xlmac_rx_enable_get(unit, port, enable));

exit:
    SOC_FUNC_RETURN;

}

int pm4x10_port_tx_mac_enable_set(int unit, int port, pm_info_t pm_info, int enable)
{
    SOC_INIT_FUNC_DEFS;

    _SOC_IF_ERR_EXIT(xlmac_tx_enable_set(unit, port, enable));

exit:
    SOC_FUNC_RETURN;

}

int pm4x10_port_tx_mac_enable_get(int unit, int port, pm_info_t pm_info, int* enable)
{
    SOC_INIT_FUNC_DEFS;

    _SOC_IF_ERR_EXIT(xlmac_tx_enable_get(unit, port, enable));

exit:
    SOC_FUNC_RETURN;

}

int pm4x10_port_ability_local_get (int unit, int port, pm_info_t pm_info, uint32 phy_flags,
                                   portmod_port_ability_t* ability)
{
    portmod_port_ability_t legacy_phy_ability;
    int                 fiber_pref, port_index;
    int                 max_speed = 11000;
    int                 num_of_lanes = 0;
    uint32              bitmap, tmpbit, interface_modes;
    phymod_phy_access_t phy_access[1+MAX_PHYN];
    int                 nof_phys, xphy_id, is_legacy_phy, rv = SOC_E_NONE;

    SOC_INIT_FUNC_DEFS;

    sal_memset(ability, 0, sizeof(portmod_port_ability_t));
    sal_memset(&legacy_phy_ability, 0, sizeof(portmod_port_ability_t));

    _SOC_IF_ERR_EXIT
        (portmod_port_chain_phy_access_get(unit, port, pm_info, phy_access,
                                           (1+MAX_PHYN), &nof_phys));

    _SOC_IF_ERR_EXIT
        (_pm4x10_port_index_get (unit, port, pm_info, &port_index, &bitmap));
    
    rv = PM4x10_MAX_SPEED_GET(unit, pm_info, &max_speed, port_index);
    _SOC_IF_ERR_EXIT(rv);

    rv = PM4x10_INTERFACE_MODES_GET(unit, pm_info, &interface_modes,
                                    port_index);
    _SOC_IF_ERR_EXIT(rv);

    fiber_pref = (interface_modes & PHYMOD_INTF_MODES_FIBER)? 1 : 0;

    tmpbit = bitmap ;
    while(tmpbit) {
        num_of_lanes += ((tmpbit&1)?1:0) ;
        tmpbit >>= 1 ;
    }

    ability->loopback  = SOC_PA_LB_PHY | SOC_PA_LB_MAC;
    ability->medium    = SOC_PA_MEDIUM_FIBER;
    ability->pause     = SOC_PA_PAUSE | SOC_PA_PAUSE_ASYMM;
    ability->fec       = SOC_PA_FEC_NONE | SOC_PA_FEC_CL74;
    ability->flags     = SOC_PA_AUTONEG;
    ability->speed_half_duplex  = SOC_PA_ABILITY_NONE;
    ability->encap     = SOC_PA_ENCAP_IEEE | SOC_PA_ENCAP_HIGIG | SOC_PA_ENCAP_HIGIG2;

    if (num_of_lanes == 1) {
        if (interface_modes & PHYMOD_INTF_MODES_HIGIG) {
            switch(max_speed) {
                case 10000: max_speed = 11000 ; break ;
                default:  break;
            }
            rv = PM4x10_MAX_SPEED_SET(unit, pm_info, max_speed, port_index);
            _SOC_IF_ERR_EXIT(rv);
        } else {
            /* 10M, 100M, 1G, 2.5G, 5G don't have equivalent HiGig speeds
               Should only be supported on a non-HiGig port
             */
            if (!fiber_pref) {
                ability->speed_full_duplex |= SOC_PA_SPEED_10MB;
            }
            ability->speed_full_duplex |= SOC_PA_SPEED_100MB;
            ability->speed_full_duplex |= SOC_PA_SPEED_1000MB;

            switch(max_speed) {
                case 11000: max_speed = 10000; break;
                default:  break;
            }
            rv = PM4x10_MAX_SPEED_SET(unit, pm_info, max_speed, port_index);
            _SOC_IF_ERR_EXIT(rv);

            /* Must include all supported speeds */
            switch(max_speed) {
                case 12000:
                case 11000:
                case 10000:
                case 5000:
                    ability->speed_full_duplex |= SOC_PA_SPEED_5000MB;
                case 2500:
                    /* Need to revisit: 2500MB for copper mode */
                    ability->speed_full_duplex |= SOC_PA_SPEED_2500MB;
                default: break;
            }
        }
        switch(max_speed) {  /* Must include all the supported speeds */
            case 12000:
                ability->speed_full_duplex |= SOC_PA_SPEED_12GB;
            case 11000:
                ability->speed_full_duplex |= SOC_PA_SPEED_11GB;
            case 10000:
                ability->speed_full_duplex |= SOC_PA_SPEED_10GB;
            default:
                break;
        }
        ability->pause     = SOC_PA_PAUSE | SOC_PA_PAUSE_ASYMM;
        ability->interface = SOC_PA_INTF_GMII | SOC_PA_INTF_SGMII;
        if (max_speed >= 10000) {
            ability->interface |= SOC_PA_INTF_XGMII;
        }
        ability->medium    = SOC_PA_MEDIUM_FIBER;

        /* single lane port */
        ability->flags = SOC_PA_AUTONEG;
    } else if (num_of_lanes == 2) {
        if(interface_modes & PHYMOD_INTF_MODES_HIGIG) {
            switch(max_speed) {
                case 10000: max_speed = 11000 ; break ;
                case 20000: max_speed = 21000 ; break ;
                default:  break ;
            }
            rv = PM4x10_MAX_SPEED_SET(unit, pm_info, max_speed, port_index);
            _SOC_IF_ERR_EXIT(rv);
        } else {
            switch(max_speed) {
                case 11000: max_speed = 10000 ; break ;
                case 21000: max_speed = 20000 ; break ;
                default:  break ;
            }
            rv = PM4x10_MAX_SPEED_SET(unit, pm_info, max_speed, port_index);
            _SOC_IF_ERR_EXIT(rv);
        }
        switch(max_speed) {  /* must include all the supported speedss */
            case 21000:
                ability->speed_full_duplex |= SOC_PA_SPEED_21GB;
            case 20000:
                ability->speed_full_duplex |= SOC_PA_SPEED_20GB;
            case 16000:
                ability->speed_full_duplex |= SOC_PA_SPEED_16GB;
            case 15000:
                ability->speed_full_duplex |= SOC_PA_SPEED_15GB;
            case 13000:
                ability->speed_full_duplex |= SOC_PA_SPEED_13GB;
            case 12000:
                ability->speed_full_duplex |= SOC_PA_SPEED_12GB;
            case 11000:
                ability->speed_full_duplex |= SOC_PA_SPEED_11GB;
            case 10000:
                ability->speed_full_duplex |= SOC_PA_SPEED_10GB;
            default:
                break;
        }
        ability->pause     = SOC_PA_PAUSE | SOC_PA_PAUSE_ASYMM;
        ability->interface = SOC_PA_INTF_XGMII;
        ability->medium    = SOC_PA_MEDIUM_FIBER;
        ability->flags = 0;
    } else {
        if(interface_modes & PHYMOD_INTF_MODES_HIGIG) {
            switch(max_speed) {
                case 10000: max_speed = 11000 ; break ;
                case 20000: max_speed = 21000 ; break ;
                case 40000: max_speed = 42000 ; break ;
                case 100000: max_speed = 106000 ; break ;
                case 120000: max_speed = 127000 ; break ;
                default:  break ;
            }
            rv = PM4x10_MAX_SPEED_SET(unit, pm_info, max_speed, port_index);
            _SOC_IF_ERR_EXIT(rv);
        } else {
            switch(max_speed) {
                case 11000: max_speed = 10000 ; break ;
                case 21000: max_speed = 20000 ; break ;
                case 42000: max_speed = 40000 ; break ;
                case 106000: max_speed = 100000 ; break ;
                case 127000: max_speed = 120000 ; break ;
                default:  break ;
            }
            rv = PM4x10_MAX_SPEED_SET(unit, pm_info, max_speed, port_index);
            _SOC_IF_ERR_EXIT(rv);
        }
        switch(max_speed) {
            case 127000:
                ability->speed_full_duplex |= SOC_PA_SPEED_127GB;
            case 120000:
                ability->speed_full_duplex |= SOC_PA_SPEED_120GB;
            case 106000:
                ability->speed_full_duplex |= SOC_PA_SPEED_106GB;
            case 100000:
                ability->speed_full_duplex |= SOC_PA_SPEED_100GB;
                break; /* PM12x10 with lanes >=10 only supports above speeds */
            case 42000:
                ability->speed_full_duplex |= SOC_PA_SPEED_42GB;
            case 40000:
                ability->speed_full_duplex |= SOC_PA_SPEED_40GB;
            case 30000:
                ability->speed_full_duplex |= SOC_PA_SPEED_30GB;
            case 25000:
                ability->speed_full_duplex |= SOC_PA_SPEED_25GB;
            case 21000:
                ability->speed_full_duplex |= SOC_PA_SPEED_21GB;
            case 20000:
                ability->speed_full_duplex |= SOC_PA_SPEED_20GB;
            case 16000:
                ability->speed_full_duplex |= SOC_PA_SPEED_16GB; 
            case 15000:
                ability->speed_full_duplex |= SOC_PA_SPEED_15GB; 
            case 13000:
                ability->speed_full_duplex |= SOC_PA_SPEED_13GB;
            case 12000:
                ability->speed_full_duplex |= SOC_PA_SPEED_12GB;
            case 11000:
                ability->speed_full_duplex |= SOC_PA_SPEED_11GB;
            case 10000:
                ability->speed_full_duplex |= SOC_PA_SPEED_10GB;
            default:
                break;
        }
        ability->pause     = SOC_PA_PAUSE | SOC_PA_PAUSE_ASYMM;
        ability->interface = SOC_PA_INTF_XGMII;
        ability->medium    = SOC_PA_MEDIUM_FIBER;
        ability->flags     = SOC_PA_AUTONEG;
    }

    /* If legacy external phy presents, retrieve the local ability from the
     * external phy as it has more variants and the ability might vary for each.
     * However, if this function is called by the legacy ext PHY driver through
     * the PORTMOD callback function portmod_ext_to_int_cmd_get(), the returned
     * data should be of the internal SERDES. portmod_ext_to_int_cmd_get() uses
     * PORTMOD_USER_ACC_CMD_FOR_PHY_SET() to hint its callees that the target
     * of data retrieving is the internal SERDES.
     */
    if (nof_phys > 1 && !(phy_flags == PORTMOD_INIT_F_INTERNAL_SERDES_ONLY)) {
        /* check if the legacy phy present */
        xphy_id = phy_access[nof_phys-1].access.addr;
        _SOC_IF_ERR_EXIT(portmod_xphy_is_legacy_phy_get(unit, xphy_id, &is_legacy_phy));
        if(is_legacy_phy) {
            /* call portmod phy chain function to retrieve the ability */
            _SOC_IF_ERR_EXIT
                (portmod_port_phychain_local_ability_get(unit, port, phy_access,
                                                         nof_phys, &legacy_phy_ability));
            /* PHY driver returns all the speeds supported by PHY
             * Advertise only the speeds supported by both PortMacro and the PHY
             */
            legacy_phy_ability.speed_full_duplex &= ability->speed_full_duplex;
            /* PHY driver may not return all supported encap modes
             * Advertise encap modes supported by both PortMacro and the PHY
             */
            legacy_phy_ability.encap |= ability->encap;
            *ability = legacy_phy_ability;
        } else {
            /* We might need to consider calling portmod phy chain for
             * non legacy phys as well*/
        }
    }

    LOG_INFO(BSL_LS_SOC_PHY,
             (BSL_META_U(unit,
                         "%-22s:unit=%d p=%d sp=%08x bitmap=%x\n"),
              __FUNCTION__, unit, port, ability->speed_full_duplex, bitmap));

exit:
    SOC_FUNC_RETURN;

}

int pm4x10_port_autoneg_set (int unit, int port, pm_info_t pm_info, uint32 phy_flags, 
                             const phymod_autoneg_control_t* an)
{
    phymod_phy_access_t phy_access[1+MAX_PHYN];
    int nof_phys, rv;
    int port_index, an_cl37, cl37_sgmii_cnt;
    uint32_t bitmap, an_done, port_dynamic_state;
    phymod_autoneg_control_t  *pAn = (phymod_autoneg_control_t*)an;
    phymod_firmware_lane_config_t fw_config;
    SOC_INIT_FUNC_DEFS;

    _SOC_IF_ERR_EXIT(_pm4x10_port_index_get (unit, port, pm_info, &port_index,
                                             &bitmap));

    if(PM_4x10_INFO(pm_info)->nof_phys[port_index] == 0) {
        _SOC_EXIT_WITH_ERR(SOC_E_UNAVAIL, (_SOC_MSG("AN isn't supported")));
    } else {
        if (pAn->num_lane_adv == 0) {
            pAn->num_lane_adv = _pm4x10_nof_lanes_get(unit, port, pm_info);
        }
        
        if (pAn->an_mode == phymod_AN_MODE_NONE) {
            rv = PM4x10_AN_MODE_GET(unit, pm_info, &pAn->an_mode, port_index);
            _SOC_IF_ERR_EXIT(rv);
        }

        if (pAn->an_mode == phymod_AN_MODE_NONE) {
            pAn->an_mode = phymod_AN_MODE_SGMII; 
        }

        rv = PM4x10_PORT_DYNAMIC_STATE_GET(unit, pm_info,
                                      &port_dynamic_state, port_index);
        _SOC_IF_ERR_EXIT(rv);
        if (PORTMOD_PORT_IS_AUTONEG_MODE_UPDATED(port_dynamic_state))  {
            rv = PM4x10_AN_MODE_GET(unit, pm_info, &pAn->an_mode, port_index);
            _SOC_IF_ERR_EXIT(rv);
            PORTMOD_PORT_AUTONEG_MODE_UPDATED_CLR(port_dynamic_state);
            rv = PM4x10_PORT_DYNAMIC_STATE_SET(unit, pm_info,
                                     port_dynamic_state, port_index);
            _SOC_IF_ERR_EXIT(rv);
        }

        _SOC_IF_ERR_EXIT
            (portmod_port_chain_phy_access_get(unit, port, pm_info, 
                                              phy_access ,(1+MAX_PHYN),
                                              &nof_phys));

        /* Setting SerDes SGMII AN mode*/
        if (nof_phys <= 1) {
            SOC_IF_ERROR_RETURN
                (portmod_port_phychain_firmware_lane_config_get(unit, phy_access, nof_phys, &fw_config));
            if (pAn->an_mode == phymod_AN_MODE_CL37) {
                if (!(fw_config.MediaType & phymodFirmwareMediaTypeOptics)) {
                    pAn->an_mode = phymod_AN_MODE_SGMII;
                }
            }
        }

        /* In case of warm boot, get autoneg informaton from hardware */  
        if(SOC_WARM_BOOT(unit)) {
            if (pAn->an_mode >= phymod_AN_MODE_Count) {
                _SOC_IF_ERR_EXIT
                    (portmod_port_phychain_autoneg_get(unit, port, phy_access, nof_phys, phy_flags, pAn, &an_done));
            }
        }

        rv = PM4x10_AN_CL37_GET(unit, pm_info, &an_cl37, port_index);
        _SOC_IF_ERR_EXIT(rv);
        if(an_cl37 == PORTMOD_CL37_SGMII_COMBO){
           if(pAn->enable){
              cl37_sgmii_cnt = 0; 
              _SOC_IF_ERR_EXIT(PM4x10_CL37_SGMII_CNT_SET(unit, pm_info,
                                                         cl37_sgmii_cnt,
                                                         port_index));
           }
        }

        _SOC_IF_ERR_EXIT
            (portmod_port_phychain_autoneg_set(unit, port, phy_access, nof_phys, phy_flags, an));
    }
     
exit:
    SOC_FUNC_RETURN;
 
}

int pm4x10_port_autoneg_get (int unit, int port, pm_info_t pm_info, uint32 phy_flags, 
                             phymod_autoneg_control_t* an)
{      
    phymod_phy_access_t phy_access[1+MAX_PHYN];
    int         nof_phys;
    uint32      an_done;
    int port_index, rv;
    uint32_t bitmap;

    SOC_INIT_FUNC_DEFS;
    _SOC_IF_ERR_EXIT(_pm4x10_port_index_get (unit, port, pm_info, &port_index,
                                             &bitmap));
    
    if(PM_4x10_INFO(pm_info)->nof_phys[port_index] == 0) {
        _SOC_EXIT_WITH_ERR(SOC_E_UNAVAIL, (_SOC_MSG("AN isn't supported")));
    } else {
        
        if (an->num_lane_adv == 0) {
            an->num_lane_adv = _pm4x10_nof_lanes_get(unit, port, pm_info);
        }
        rv = PM4x10_AN_MODE_GET(unit, pm_info, &an->an_mode, port_index);
        _SOC_IF_ERR_EXIT(rv);

        if (an->an_mode == phymod_AN_MODE_NONE) {
            an->an_mode = phymod_AN_MODE_SGMII; 
        }

        _SOC_IF_ERR_EXIT
            (portmod_port_chain_phy_access_get(unit, port, pm_info, 
                                              phy_access ,(1+MAX_PHYN),
                                               &nof_phys));
        _SOC_IF_ERR_EXIT
            (portmod_port_phychain_autoneg_get(unit, port, phy_access, nof_phys, phy_flags, an, &an_done));
    }

exit:
    SOC_FUNC_RETURN;
 
}

int pm4x10_port_autoneg_status_get (int unit, int port, pm_info_t pm_info,
                                    phymod_autoneg_status_t* an_status)
{   
    phymod_phy_access_t phy_access[1+MAX_PHYN];
    int nof_phys;
    int port_index;
    uint32 bitmap;
    portmod_port_interface_config_t port_intf_config;
    phymod_interface_t phy_intf;
    
    SOC_INIT_FUNC_DEFS; 

    _SOC_IF_ERR_EXIT(_pm4x10_port_index_get (unit, port, pm_info, &port_index,
                                             &bitmap));

    if(PM_4x10_INFO(pm_info)->nof_phys[port_index] == 0) {
        _SOC_EXIT_WITH_ERR(SOC_E_UNAVAIL,
                          (_SOC_MSG("Invalid number of Phys")));
    } 


    _SOC_IF_ERR_EXIT
        (portmod_port_chain_phy_access_get(unit, port, pm_info, 
                                         phy_access ,(1+MAX_PHYN),
                                         &nof_phys));
    _SOC_IF_ERR_EXIT
        (portmod_port_phychain_autoneg_status_get(unit, port, phy_access, nof_phys, an_status));

    /* Even if AN is enabled the data rate and interface should be retrieved through 
     * interface_config_get function
     */
    _SOC_IF_ERR_EXIT
        (pm4x10_port_interface_config_get(unit, port, pm_info, &port_intf_config, PORTMOD_INIT_F_EXTERNAL_MOST_ONLY));
    _SOC_IF_ERR_EXIT
        (portmod_intf_to_phymod_intf(unit, port_intf_config.speed, port_intf_config.interface, &phy_intf));

    if (an_status->enabled && an_status->locked) {
        an_status->data_rate = port_intf_config.speed;
        an_status->interface = phy_intf;
    } else {
        /* upper layer should not rely on the 
         *  data rate and interface if the AN is not locked
         */
        an_status->data_rate = 0;
        an_status->interface = SOC_PORT_IF_NULL;        
    }

exit:
    SOC_FUNC_RETURN;
    
}

int pm4x10_port_link_get(int unit, int port, pm_info_t pm_info, int flags, int* link)
{
    phymod_phy_access_t phy_access[1+MAX_PHYN];
    int nof_phys;
    int sys_start_lane=-1;
    int sys_end_lane=-1;
    int line_start_lane=-1;
    int line_end_lane=-1;
    uint32 int_lane_mask=0xf;
    uint32 bitmap, an_done;
    int    port_index, an_cl37, cl37_sgmii_war = 0;
    int    cl37_sgmii_cnt, cl37_sgmii_restart_cnt;
    phymod_autoneg_control_t an;
    SOC_INIT_FUNC_DEFS;

    _SOC_IF_ERR_EXIT
        (portmod_port_chain_phy_access_get(unit, port, pm_info, 
                                           phy_access ,(1+MAX_PHYN),
                                           &nof_phys));

    _SOC_IF_ERR_EXIT(_pm4x10_port_tricore_lane_info_get(unit, port, pm_info,
                                                        &sys_start_lane, &sys_end_lane,
                                                        &line_start_lane, &line_end_lane,
                                                        &int_lane_mask));
    /* only look at the lanes that are used. */
    phy_access[0].access.lane_mask &= int_lane_mask;

    _SOC_IF_ERR_EXIT
            (portmod_port_phychain_link_status_get(unit, port, phy_access, nof_phys, flags, (uint32_t*) link));

    _SOC_IF_ERR_EXIT(_pm4x10_port_index_get (unit, port, pm_info, &port_index,
                                             &bitmap));

    _SOC_IF_ERR_EXIT(PM4x10_AN_CL37_GET(unit, pm_info, &an_cl37, port_index));
     if(an_cl37 == PORTMOD_CL37_SGMII_COMBO) {
        _SOC_IF_ERR_EXIT(portmod_port_phychain_autoneg_get(unit, port, phy_access,
                                                           nof_phys, PORTMOD_INIT_F_EXTERNAL_MOST_ONLY, 
                                                           &an, &an_done));
        if (an.enable) {
            cl37_sgmii_war = 1;
        }
     }

    if(*link == 0){
       if(cl37_sgmii_war){
          _SOC_IF_ERR_EXIT(PM4x10_CL37_SGMII_CNT_GET(unit, pm_info,
                                                     &cl37_sgmii_cnt,
                                                     port_index));
          _SOC_IF_ERR_EXIT(PM4x10_CL37_SGMII_RESTART_CNT_GET(unit, pm_info,
                                                     &cl37_sgmii_restart_cnt,
                                                     port_index));
          if(++cl37_sgmii_cnt >= cl37_sgmii_restart_cnt){
             cl37_sgmii_cnt = 0;
             _SOC_IF_ERR_EXIT(PM4x10_CL37_SGMII_CNT_SET(unit, pm_info,
                                                        cl37_sgmii_cnt,
                                                        port_index));
             _SOC_IF_ERR_EXIT
                (portmod_port_phychain_autoneg_restart_set(unit, phy_access,nof_phys));
            }
        }
    }

exit:
    SOC_FUNC_RETURN;
}

int pm4x10_port_link_latch_down_get(int unit, int port, pm_info_t pm_info, uint32 flags, int* link)
{
    int first_index, rv;
    uint32 reg32_val, bitmap;
    soc_field_t status_field, clear_field;
    SOC_INIT_FUNC_DEFS;
    
    *link = 0;
    
    rv = _pm4x10_port_index_get(unit, port, pm_info, &first_index, &bitmap);
    _SOC_IF_ERR_EXIT(rv);

     switch(first_index) {
        case 0:
            status_field = PORT0_LINKSTATUSf;
            clear_field = PORT0_LINKDOWN_CLEARf;
            break;

        case 1:
            status_field = PORT1_LINKSTATUSf;
            clear_field = PORT1_LINKDOWN_CLEARf;
            break;

        case 2:
            status_field = PORT2_LINKSTATUSf;
            clear_field = PORT2_LINKDOWN_CLEARf;
            break;

        case 3:
            status_field = PORT3_LINKSTATUSf;
            clear_field = PORT3_LINKDOWN_CLEARf;
            break;

        default:
            _SOC_EXIT_WITH_ERR(SOC_E_INTERNAL,
                     (_SOC_MSG("Port %d, failed to get port index"), port));
    }

    rv = READ_XLPORT_LINKSTATUS_DOWNr(unit, port, &reg32_val);
    _SOC_IF_ERR_EXIT(rv);

    (*link) = soc_reg_field_get(unit, XLPORT_LINKSTATUS_DOWNr, 
                                        reg32_val, status_field);

    if (PORTMOD_PORT_LINK_LATCH_DOWN_F_CLEAR & flags) {

        rv = READ_XLPORT_LINKSTATUS_DOWN_CLEARr(unit, port, &reg32_val);
        _SOC_IF_ERR_EXIT(rv);

        soc_reg_field_set(unit, XLPORT_LINKSTATUS_DOWN_CLEARr, 
                                        &reg32_val, clear_field, 1);

        rv = WRITE_XLPORT_LINKSTATUS_DOWN_CLEARr(unit, port, reg32_val);
        _SOC_IF_ERR_EXIT(rv);

        sal_usleep(10);

        soc_reg_field_set(unit, XLPORT_LINKSTATUS_DOWN_CLEARr, 
                                        &reg32_val, clear_field, 0);

        /* reset default register value */
        rv = WRITE_XLPORT_LINKSTATUS_DOWN_CLEARr(unit, port, reg32_val);
        _SOC_IF_ERR_EXIT(rv);

    }

        
exit:
    SOC_FUNC_RETURN;
}

int pm4x10_port_phy_link_up_event(int unit, int port, pm_info_t pm_info)
{
    phymod_phy_access_t phy_access[1+MAX_PHYN];
    int nof_phys;

    SOC_INIT_FUNC_DEFS;

    _SOC_IF_ERR_EXIT
        (portmod_port_chain_phy_access_get(unit, port, pm_info,
                                           phy_access ,(1+MAX_PHYN),
                                           &nof_phys));
    _rv = portmod_port_phychain_phy_link_up_event(unit, port, phy_access, nof_phys);
exit:
    SOC_FUNC_RETURN;
}

int pm4x10_port_phy_link_down_event(int unit, int port, pm_info_t pm_info)
{
    phymod_phy_access_t phy_access[1+MAX_PHYN];
    int nof_phys;

    SOC_INIT_FUNC_DEFS;

    _SOC_IF_ERR_EXIT
        (portmod_port_chain_phy_access_get(unit, port, pm_info,
                                           phy_access ,(1+MAX_PHYN),
                                           &nof_phys));
    
    _rv = portmod_port_phychain_phy_link_down_event(unit, port, phy_access, nof_phys);

exit:
    SOC_FUNC_RETURN;

}

int pm4x10_port_mac_link_get(int unit, int port, pm_info_t pm_info, int* link)
{
    uint32 reg_val, bitmap;
    int port_index, phy_acc;
    SOC_INIT_FUNC_DEFS;
    SOC_NULL_CHECK(pm_info);

    PM4x10_PHY_ACCESS_GET(unit, port, pm_info, phy_acc);

    _SOC_IF_ERR_EXIT(_pm4x10_port_index_get (unit, port, pm_info, &port_index, 
                                             &bitmap));

    switch(port_index) {
        case 0:
            _SOC_IF_ERR_EXIT(READ_XLPORT_XGXS0_LN0_STATUS0_REGr(unit, phy_acc, 
                                        &reg_val));
            (*link) = soc_reg_field_get(unit, XLPORT_XGXS0_LN0_STATUS0_REGr, 
                                        reg_val, LINK_STATUSf);
            break;

        case 1:
            _SOC_IF_ERR_EXIT(READ_XLPORT_XGXS0_LN1_STATUS0_REGr(unit, phy_acc, 
                                         &reg_val));
            (*link) = soc_reg_field_get(unit, XLPORT_XGXS0_LN1_STATUS0_REGr, 
                                          reg_val, LINK_STATUSf);
            break;

        case 2:
            _SOC_IF_ERR_EXIT(READ_XLPORT_XGXS0_LN2_STATUS0_REGr(unit, phy_acc, 
                                          &reg_val));
            (*link) = soc_reg_field_get(unit, XLPORT_XGXS0_LN2_STATUS0_REGr, 
                                          reg_val, LINK_STATUSf);
            break;

        case 3:
            _SOC_IF_ERR_EXIT(READ_XLPORT_XGXS0_LN3_STATUS0_REGr(unit, phy_acc, 
                                           &reg_val));
            (*link) = soc_reg_field_get(unit, XLPORT_XGXS0_LN3_STATUS0_REGr, 
                                           reg_val, LINK_STATUSf);
            break;

        default:
            _SOC_EXIT_WITH_ERR(SOC_E_INTERNAL,
                     (_SOC_MSG("Port %d, failed to get port index"), port));
    }

exit:
    SOC_FUNC_RETURN;
}


int pm4x10_port_prbs_config_set(int unit, int port, pm_info_t pm_info, 
                     portmod_prbs_mode_t mode, int flags, const phymod_prbs_t* config)
{
    phymod_phy_access_t phy_access[1+MAX_PHYN];
    int nof_phys;
    int port_index;
    uint32 bitmap;
    
    SOC_INIT_FUNC_DEFS;

    if(mode == 1 /*MAC*/) {
        _SOC_EXIT_WITH_ERR(SOC_E_PARAM,
                 (_SOC_MSG("MAC PRBS is not supported for PM4x10")));
    }

    _SOC_IF_ERR_EXIT(_pm4x10_port_index_get (unit, port, pm_info, &port_index,
                                             &bitmap));

    if(PM_4x10_INFO(pm_info)->nof_phys[port_index] == 0) {
        _SOC_EXIT_WITH_ERR(SOC_E_UNAVAIL,
                            (_SOC_MSG("phy PRBS isn't supported")));
    }

    _SOC_IF_ERR_EXIT
        (portmod_port_chain_phy_access_get(unit, port, pm_info, 
                                           phy_access ,(1+MAX_PHYN),
                                           &nof_phys));
    _SOC_IF_ERR_EXIT
        (portmod_port_phychain_prbs_config_set(unit, phy_access, nof_phys, flags, config));

exit:
    SOC_FUNC_RETURN;
}

int pm4x10_port_prbs_config_get (int unit, int port, pm_info_t pm_info, 
                                 portmod_prbs_mode_t mode, int flags, phymod_prbs_t* config)
{
    phymod_phy_access_t phy_access[1+MAX_PHYN];
    int nof_phys;
    int port_index;
    uint32 bitmap;

    SOC_INIT_FUNC_DEFS;

    if(mode == 1 /*MAC*/) {
        _SOC_EXIT_WITH_ERR(SOC_E_PARAM,
                      (_SOC_MSG("MAC PRBS is not supported for PM4x10")));
    }

    _SOC_IF_ERR_EXIT(_pm4x10_port_index_get (unit, port, pm_info, &port_index,
                                             &bitmap));

    if(PM_4x10_INFO(pm_info)->nof_phys[port_index] == 0) {
        _SOC_EXIT_WITH_ERR(SOC_E_UNAVAIL,
                       (_SOC_MSG("phy PRBS isn't supported")));
    }

    _SOC_IF_ERR_EXIT
        (portmod_port_chain_phy_access_get(unit, port, pm_info, 
                                           phy_access ,(1+MAX_PHYN),
                                           &nof_phys));
    _SOC_IF_ERR_EXIT
        (portmod_port_phychain_prbs_config_get(unit, phy_access, nof_phys, flags, config));

exit:
    SOC_FUNC_RETURN;
}


int pm4x10_port_prbs_enable_set (int unit, int port, pm_info_t pm_info, 
                                 portmod_prbs_mode_t mode, int flags, int enable)
{
    phymod_phy_access_t phy_access[1+MAX_PHYN];
    int nof_phys;
    int port_index;
    uint32 bitmap;

    SOC_INIT_FUNC_DEFS;
    
    if(mode == 1 /*MAC*/) {
        _SOC_EXIT_WITH_ERR(SOC_E_PARAM, (
              _SOC_MSG("MAC PRBS is not supported for PM4x10")));
    }
    
    _SOC_IF_ERR_EXIT(_pm4x10_port_index_get (unit, port, pm_info, &port_index,
                                             &bitmap));

    if(PM_4x10_INFO(pm_info)->nof_phys[port_index] == 0) {
        _SOC_EXIT_WITH_ERR(SOC_E_UNAVAIL, (
              _SOC_MSG("phy PRBS isn't supported")));
    }

    _SOC_IF_ERR_EXIT
        (portmod_port_chain_phy_access_get(unit, port, pm_info, 
                                           phy_access ,(1+MAX_PHYN),
                                           &nof_phys));
    _SOC_IF_ERR_EXIT
        (portmod_port_phychain_prbs_enable_set(unit, phy_access, nof_phys, flags, enable));

exit:
    SOC_FUNC_RETURN;
}

int pm4x10_port_prbs_enable_get (int unit, int port, pm_info_t pm_info, 
                                 portmod_prbs_mode_t mode, int flags, int* enable)
{
    phymod_phy_access_t phy_access[1+MAX_PHYN];
    uint32 is_enabled;
    int nof_phys;
    int port_index;
    uint32 bitmap;

    SOC_INIT_FUNC_DEFS;

    if(mode == 1 /*MAC*/) {
        _SOC_EXIT_WITH_ERR(SOC_E_PARAM,
                 (_SOC_MSG("MAC PRBS is not supported for PM4x10")));
    }

    _SOC_IF_ERR_EXIT(_pm4x10_port_index_get (unit, port, pm_info, &port_index,
                                             &bitmap));

    if(PM_4x10_INFO(pm_info)->nof_phys[port_index] == 0) {
        _SOC_EXIT_WITH_ERR(SOC_E_UNAVAIL,
                    (_SOC_MSG("phy PRBS isn't supported")));
    }

    _SOC_IF_ERR_EXIT
        (portmod_port_chain_phy_access_get(unit, port, pm_info, 
                                           phy_access ,(1+MAX_PHYN),
                                           &nof_phys));

    _SOC_IF_ERR_EXIT
        (portmod_port_phychain_prbs_enable_get(unit, phy_access, nof_phys, flags, &is_enabled));

    (*enable) = (is_enabled ? 1 : 0);

exit:
    SOC_FUNC_RETURN;
}


int pm4x10_port_prbs_status_get(int unit, int port, pm_info_t pm_info, 
                    portmod_prbs_mode_t mode, int flags, phymod_prbs_status_t* status)
{
    phymod_phy_access_t phy_access[1+MAX_PHYN];
    int nof_phys;
    int port_index;
    uint32 bitmap;

    SOC_INIT_FUNC_DEFS;

    if(mode == 1 /*MAC*/) {
        _SOC_EXIT_WITH_ERR(SOC_E_PARAM, 
                (_SOC_MSG("MAC PRBS is not supported for PM4x10")));
    }

    _SOC_IF_ERR_EXIT(_pm4x10_port_index_get (unit, port, pm_info, &port_index,
                                             &bitmap));

    if(PM_4x10_INFO(pm_info)->nof_phys[port_index] == 0) {
        _SOC_EXIT_WITH_ERR(SOC_E_UNAVAIL, 
                    (_SOC_MSG("phy PRBS isn't supported")));
    }

    _SOC_IF_ERR_EXIT
        (portmod_port_chain_phy_access_get(unit, port, pm_info, 
                                           phy_access ,(1+MAX_PHYN),
                                           &nof_phys));
    _SOC_IF_ERR_EXIT
        (portmod_port_phychain_prbs_status_get(unit, phy_access, nof_phys, flags, status));

exit:
    SOC_FUNC_RETURN;

}

int pm4x10_port_nof_lanes_get(int unit, int port, pm_info_t pm_info, int *nof_lanes)
{
    int temp_nof_lanes = 0;
    SOC_INIT_FUNC_DEFS;

    temp_nof_lanes = _pm4x10_nof_lanes_get(unit, port, pm_info);
    if(0 == temp_nof_lanes){
        SOC_EXIT;
    }
    else{
        *nof_lanes = temp_nof_lanes;
    }

exit:
    SOC_FUNC_RETURN;
}


int pm4x10_port_firmware_mode_set (int unit, int port, pm_info_t pm_info,
                                   phymod_firmware_mode_t fw_mode)
{
        
    SOC_INIT_FUNC_DEFS;
    SOC_NULL_CHECK(pm_info);
    
    /* Place your code here */

        
exit:
    SOC_FUNC_RETURN; 
    
}

int pm4x10_port_firmware_mode_get (int unit, int port, pm_info_t pm_info, 
                                   phymod_firmware_mode_t* fw_mode)
{
        
    SOC_INIT_FUNC_DEFS;
    SOC_NULL_CHECK(pm_info);
    
    /* Place your code here */

        
exit:
    SOC_FUNC_RETURN; 
    
}


int pm4x10_port_runt_threshold_set (int unit, int port, 
                                    pm_info_t pm_info, int value)
{
        
    SOC_INIT_FUNC_DEFS;
    SOC_NULL_CHECK(pm_info);
    
    /* Place your code here */
        
exit:
    SOC_FUNC_RETURN; 
    
}

int pm4x10_port_runt_threshold_get(int unit, int port, pm_info_t pm_info, int* value)
{
        
    SOC_INIT_FUNC_DEFS;
    SOC_NULL_CHECK(pm_info);
    
    /* Place your code here */

        
exit:
    SOC_FUNC_RETURN; 
    
}


int pm4x10_port_max_packet_size_set (int unit, int port, 
                                     pm_info_t pm_info, int value)
{
    SOC_INIT_FUNC_DEFS;

    _SOC_IF_ERR_EXIT(xlmac_rx_max_size_set(unit, port, value));

exit:
    SOC_FUNC_RETURN;
}

int pm4x10_port_max_packet_size_get (int unit, int port, 
                                     pm_info_t pm_info, int* value)
{
    SOC_INIT_FUNC_DEFS;
    _SOC_IF_ERR_EXIT(xlmac_rx_max_size_get(unit, port, value));

exit:
    SOC_FUNC_RETURN;
}


int pm4x10_port_pad_size_set(int unit, int port, pm_info_t pm_info, int value)
{        
    SOC_INIT_FUNC_DEFS;
    SOC_NULL_CHECK(pm_info);
    
    _SOC_IF_ERR_EXIT(xlmac_pad_size_set(unit, port , value));
        
exit:
    SOC_FUNC_RETURN; 
    
}

int pm4x10_port_pad_size_get(int unit, int port, pm_info_t pm_info, int* value)
{        
    SOC_INIT_FUNC_DEFS;
    SOC_NULL_CHECK(pm_info);
    
    _SOC_IF_ERR_EXIT(xlmac_pad_size_get(unit, port , value));
        
exit:
    SOC_FUNC_RETURN;     
}


int pm4x10_port_tx_drop_on_local_fault_set (int unit, int port, 
                                            pm_info_t pm_info, int enable)
{
    portmod_local_fault_control_t control;
    SOC_INIT_FUNC_DEFS;

    portmod_local_fault_control_t_init(unit, &control);
    _SOC_IF_ERR_EXIT(pm4x10_port_local_fault_control_get(unit, port, pm_info, &control));

    control.drop_tx_on_fault  = enable;
    _SOC_IF_ERR_EXIT(pm4x10_port_local_fault_control_set(unit, port, pm_info, &control));

exit:
    SOC_FUNC_RETURN;
}

int pm4x10_port_tx_drop_on_local_fault_get (int unit, int port, 
                                            pm_info_t pm_info, int* enable)
{
    portmod_local_fault_control_t control;
    SOC_INIT_FUNC_DEFS;

    portmod_local_fault_control_t_init(unit, &control);
    _SOC_IF_ERR_EXIT(pm4x10_port_local_fault_control_get(unit, port, pm_info, &control));
    *enable = control.drop_tx_on_fault;

exit:
    SOC_FUNC_RETURN;
}


int pm4x10_port_tx_drop_on_remote_fault_set(int unit, int port,
                                            pm_info_t pm_info, int enable)
{
    portmod_remote_fault_control_t control;
    SOC_INIT_FUNC_DEFS;

    portmod_remote_fault_control_t_init(unit, &control);

    _SOC_IF_ERR_EXIT(pm4x10_port_remote_fault_control_get(unit, port, pm_info, &control));
    control.drop_tx_on_fault = enable;
    _SOC_IF_ERR_EXIT(pm4x10_port_remote_fault_control_set(unit, port, pm_info,  &control));

exit:
    SOC_FUNC_RETURN;
}

int pm4x10_port_tx_drop_on_remote_fault_get (int unit, int port, 
                                             pm_info_t pm_info, int* enable)
{
    portmod_remote_fault_control_t control;
    SOC_INIT_FUNC_DEFS;

    portmod_remote_fault_control_t_init(unit, &control);
    _SOC_IF_ERR_EXIT(pm4x10_port_remote_fault_control_get(unit, port, pm_info, &control));
    *enable = control.drop_tx_on_fault;

exit:
    SOC_FUNC_RETURN;
}

int pm4x10_port_local_fault_enable_set (int unit, int port, 
                                        pm_info_t pm_info, int enable)
{
    portmod_local_fault_control_t control;
    SOC_INIT_FUNC_DEFS;

    portmod_local_fault_control_t_init(unit, &control);

    _SOC_IF_ERR_EXIT(pm4x10_port_local_fault_control_get(unit, port, pm_info, &control));
    control.enable = enable;
    _SOC_IF_ERR_EXIT(pm4x10_port_local_fault_control_set(unit, port, pm_info, &control));

exit:
    SOC_FUNC_RETURN;
}

int pm4x10_port_local_fault_enable_get (int unit, int port, 
                                        pm_info_t pm_info, int* enable)
{
    portmod_local_fault_control_t control;
    SOC_INIT_FUNC_DEFS;

    portmod_local_fault_control_t_init(unit, &control);

    _SOC_IF_ERR_EXIT(pm4x10_port_local_fault_control_get(unit, port, pm_info, &control));
    *enable = control.enable;

exit:
    SOC_FUNC_RETURN;
}

int pm4x10_port_local_fault_status_get(int unit, int port,
                                       pm_info_t pm_info, int* value)
{
    int rv;
    SOC_INIT_FUNC_DEFS;

    rv = xlmac_local_fault_status_get(unit, port, value);
    _SOC_IF_ERR_EXIT(rv);
exit:
    SOC_FUNC_RETURN;
}

int pm4x10_port_local_fault_control_set(int unit, int port, pm_info_t pm_info,
                    const portmod_local_fault_control_t* control)
{
    int rv;
    SOC_INIT_FUNC_DEFS;

    rv = xlmac_local_fault_control_set(unit, port, control);
    _SOC_IF_ERR_EXIT(rv);
exit:
    SOC_FUNC_RETURN;
}

int pm4x10_port_local_fault_control_get(int unit, int port, pm_info_t pm_info,
                    portmod_local_fault_control_t* control)
{
    int rv;
    SOC_INIT_FUNC_DEFS;

    rv = xlmac_local_fault_control_get(unit, port, control);
    _SOC_IF_ERR_EXIT(rv);
exit:
    SOC_FUNC_RETURN;
}

int pm4x10_port_remote_fault_status_get(int unit, int port,
                                        pm_info_t pm_info, int* value)
{
    int rv;
    SOC_INIT_FUNC_DEFS;

    rv = xlmac_remote_fault_status_get(unit, port, value);
    _SOC_IF_ERR_EXIT(rv);
exit:
    SOC_FUNC_RETURN;
}

int pm4x10_port_remote_fault_control_set(int unit, int port, pm_info_t pm_info,
                    const portmod_remote_fault_control_t* control)
{
    int rv;
    SOC_INIT_FUNC_DEFS;

    rv = xlmac_remote_fault_control_set(unit, port, control);
    _SOC_IF_ERR_EXIT(rv);
exit:
    SOC_FUNC_RETURN;
}

int pm4x10_port_remote_fault_control_get(int unit, int port, pm_info_t pm_info,
                    portmod_remote_fault_control_t* control)
{
    int rv;
    SOC_INIT_FUNC_DEFS;

    rv = xlmac_remote_fault_control_get(unit, port, control);
    _SOC_IF_ERR_EXIT(rv);
exit:
    SOC_FUNC_RETURN;
}


int pm4x10_port_pause_control_set(int unit, int port, pm_info_t pm_info, 
                                  const portmod_pause_control_t* control)
{
    SOC_INIT_FUNC_DEFS;
    _SOC_IF_ERR_EXIT(xlmac_pause_control_set(unit, port, control));
exit:
    SOC_FUNC_RETURN;
}

int pm4x10_port_pause_control_get(int unit, int port, pm_info_t pm_info,
                                  portmod_pause_control_t* control)
{
    SOC_INIT_FUNC_DEFS;
    _SOC_IF_ERR_EXIT(xlmac_pause_control_get(unit, port, control));
exit:
    SOC_FUNC_RETURN;
}

int pm4x10_port_llfc_control_set(int unit, int port, 
           pm_info_t pm_info, const portmod_llfc_control_t* control)
{
    SOC_INIT_FUNC_DEFS;
    _SOC_IF_ERR_EXIT(xlmac_llfc_control_set(unit, port, control));
exit:
    SOC_FUNC_RETURN;
}

int pm4x10_port_llfc_control_get(int unit, int port,
            pm_info_t pm_info, portmod_llfc_control_t* control)
{
    SOC_INIT_FUNC_DEFS;
    _SOC_IF_ERR_EXIT(xlmac_llfc_control_get(unit, port, control));

exit:
    SOC_FUNC_RETURN;
}

int pm4x10_port_core_access_get (int unit, int port, 
                                 pm_info_t pm_info, int phyn, int max_cores, 
                                 phymod_core_access_t* core_access_arr, 
                                 int* nof_cores,
                                 int* is_most_ext)
{
    int port_index;
    uint32 bitmap;
    int index;
    int done=0;
    int i,j, is_legacy_phy;
    uint32 xphy_id;
    phymod_core_access_t core_access;   
 
    SOC_INIT_FUNC_DEFS;

    _SOC_IF_ERR_EXIT(_pm4x10_port_index_get (unit, port, pm_info, &port_index,
                                             &bitmap));

    if(phyn > PM_4x10_INFO(pm_info)->nof_phys[port_index]){
        _SOC_EXIT_WITH_ERR(SOC_E_PARAM, 
               (_SOC_MSG("phyn exceeded. max allowed %d. got %d"), 
                       PM_4x10_INFO(pm_info)->nof_phys[port_index] - 1, phyn));
    }

    if(phyn < 0)
    {
        phyn = PM_4x10_INFO(pm_info)->nof_phys[port_index] - 1;
    }

    _SOC_IF_ERR_EXIT(phymod_core_access_t_init(&core_access_arr[0]));

    if( phyn == 0 ){
        sal_memcpy(&core_access_arr[0], &(PM_4x10_INFO(pm_info)->int_core_access),
                     sizeof(phymod_core_access_t));
        *nof_cores = 1;
    } else {
        index = 0;
        done = 0;
        for(i=0 ; (i< MAX_PORTS_PER_PM4X10); i++) {
            if(bitmap & (0x1U << i)) {
                xphy_id = PM_4x10_INFO(pm_info)->lane_conn[phyn-1][i].xphy_id;
                if(xphy_id != PORTMOD_XPHY_ID_INVALID){
                    if(index == 0){
                        _SOC_IF_ERR_EXIT(portmod_xphy_core_access_get(unit, xphy_id, &core_access, &is_legacy_phy));
                        sal_memcpy(&core_access_arr[index], &core_access,
                                   sizeof(phymod_core_access_t));
                        index++;
                    } else {
                        for( j = 0 ; (j < index) &&(!done) ; j++) {
                             if((!sal_memcmp (&core_access_arr[j], &core_access,
                                               sizeof(phymod_core_access_t)))) { /* found a match */
                                done = 1;
                            }
                        }
                        if((!done) && (index < max_cores)){                        
                            sal_memcpy(&core_access_arr[index], &core_access,
                                       sizeof(phymod_core_access_t));
                            index++;
                        }
                    }
                }
            }
        }
        *nof_cores = index;
    }
    if (is_most_ext) {
        if (phyn == PM_4x10_INFO(pm_info)->nof_phys[port_index]-1) {
            *is_most_ext = 1;
        } else {
            *is_most_ext = 0;
        }
    }

exit:
    SOC_FUNC_RETURN;
}

/*!
 * pm4x10_port_frame_spacing_stretch_set
 *
 * @brief Port Mac Control Spacing Stretch 
 *
 * @param [in]  unit            - unit id
 * @param [in]  port            - logical port
 * @param [in]  spacing         - 
 */
int pm4x10_port_frame_spacing_stretch_set (int unit, int port, 
                                           pm_info_t pm_info,int spacing)
{
    return(xlmac_frame_spacing_stretch_set(unit, port, spacing));
}

/*! 
 * pm4x10_port_frame_spacing_stretch_get
 *
 * @brief Port Mac Control Spacing Stretch 
 *
 * @param [in]  unit            - unit id
 * @param [in]  port            - logical port
 * @param [in]  spacing         - 
 */
int pm4x10_port_frame_spacing_stretch_get (int unit, int port,
                                            pm_info_t pm_info, 
                                            const int *spacing)
{
    return (xlmac_frame_spacing_stretch_get(unit, port, (int*)spacing));
}

/*! 
 * pm4x10_port_diag_fifo_status_get
 *
 * @brief get port timestamps in fifo 
 *
 * @param [in]  unit            - unit id
 * @param [in]  port            - logical port
 * @param [in]  diag_info       - 
 */
int pm4x10_port_diag_fifo_status_get (int unit, int port,pm_info_t pm_info, 
                                 const portmod_fifo_status_t* diag_info)
{
    return(xlmac_diag_fifo_status_get(unit, port, diag_info));
}

int _xlport_pfc_config_set(int unit, int port, uint32 value)
{
    int rv = SOC_E_UNAVAIL;
    uint32 rval32 = 0;

    if (SOC_REG_IS_VALID(unit,XLPORT_MAC_RSV_MASKr)) {
        rv = READ_XLPORT_MAC_RSV_MASKr(unit, port, &rval32);
        if (SOC_SUCCESS(rv)) {
            /* XLPORT_MAC_RSV_MASK: Bit 18 PFC frame detected
             * Enable  PFC Frame : Set 0. Go through
             * Disable PFC Frame : Set 1. Purged.
             */
            if(value) {
                rval32 &= ~(1 << 18);
            } else {
                rval32 |= (1 << 18);
            }
            rv = WRITE_XLPORT_MAC_RSV_MASKr(unit, port, rval32);
        }
    }

    return rv;
}

int _xlport_pfc_config_get(int unit, int port, uint32* value)
{
    int rv = SOC_E_NONE;
    uint32 rval32 = 0;

    if (SOC_REG_IS_VALID(unit,XLPORT_MAC_RSV_MASKr)) {
        rv = READ_XLPORT_MAC_RSV_MASKr(unit, port, &rval32);
        if (SOC_SUCCESS(rv)) {
            /* XLPORT_MAC_RSV_MASK: Bit 18 PFC frame detected
             * Enable  PFC Frame : Set 0. Go through
             * Disable PFC Frame : Set 1. Purged.
             */
            *value = ((rval32 & 0x40000) >> 18) ? 0 : 1;
        }
    }

    return rv;
}

/*! 
 * pm4x10_port_pfc_config_set
 *
 * @brief set pass control frames. 
 *
 * @param [in]  unit            - unit id
 * @param [in]  port            - logical port
 * @param [in]  pfc_cfg         - 
 */
int pm4x10_port_pfc_config_set (int unit, int port,pm_info_t pm_info,
                           const portmod_pfc_config_t* pfc_cfg)
{
    SOC_INIT_FUNC_DEFS;

    if (pfc_cfg->classes != 8) {
        return SOC_E_PARAM;
    }
    _SOC_IF_ERR_EXIT(xlmac_pfc_config_set(unit, port, pfc_cfg));
    _SOC_IF_ERR_EXIT(_xlport_pfc_config_set(unit, port, pfc_cfg->rxpass));
exit:
    SOC_FUNC_RETURN; 
}


/*! 
 * pm4x10_port_pfc_config_get
 *
 * @brief set pass control frames. 
 *
 * @param [in]  unit            - unit id
 * @param [in]  port            - logical port
 * @param [in]  pfc_cfg         - 
 */
int pm4x10_port_pfc_config_get (int unit, int port,pm_info_t pm_info, 
                                portmod_pfc_config_t* pfc_cfg)
{
    SOC_INIT_FUNC_DEFS;

    pfc_cfg->classes = 8;
    _SOC_IF_ERR_EXIT(xlmac_pfc_config_get(unit, port, pfc_cfg));

    _SOC_IF_ERR_EXIT(_xlport_pfc_config_get(unit, port, &pfc_cfg->rxpass));
exit:
    SOC_FUNC_RETURN; 
}

int pm4x10_port_pfc_control_set(int unit, int port, pm_info_t pm_info, const portmod_pfc_control_t* control)
{
        
    SOC_INIT_FUNC_DEFS;
    _SOC_IF_ERR_EXIT(xlmac_pfc_control_set(unit, port, control));        
exit:
    SOC_FUNC_RETURN; 
    
}

int pm4x10_port_pfc_control_get(int unit, int port, pm_info_t pm_info, portmod_pfc_control_t* control)
{
        
    SOC_INIT_FUNC_DEFS;
    _SOC_IF_ERR_EXIT(xlmac_pfc_control_get(unit, port, control));     
exit:
    SOC_FUNC_RETURN; 
    
}

/*!
 * pm4x10_port_eee_set
 *
 * @brief set EEE control and timers
 *
 * @param [in]  unit            - unit id
 * @param [in]  port            - logical port
 * @param [in]  eee             -
 */
int pm4x10_port_eee_set(int unit, int port, pm_info_t pm_info,const portmod_eee_t* eee)
{
    return (xlmac_eee_set(unit, port, eee));
}

/*!
 * pm4x10_port_eee_get
 *
 * @brief get EEE control and timers
 *
 * @param [in]  unit            - unit id
 * @param [in]  port            - logical port
 * @param [in]  eee             -
 */
int pm4x10_port_eee_get(int unit, int port, pm_info_t pm_info, portmod_eee_t* eee)
{
    return (xlmac_eee_get(unit, port, eee));
}

/*! 
 * pm4x10_port_vlan_tag_set
 *
 * @brief vlan tag set. 
 *
 * @param [in]  unit            - unit id
 * @param [in]  port            - logical port
 * @param [in]  vlan_tag        - 
 */
int pm4x10_port_vlan_tag_set(int unit, int port, pm_info_t pm_info,const portmod_vlan_tag_t* vlan_tag)
{
    return (xlmac_rx_vlan_tag_set (unit, port, vlan_tag->outer_vlan_tag,
                                   vlan_tag->inner_vlan_tag));
}


/*! 
 * pm4x10_port_vlan_tag_get
 *
 * @brief vlan tag get. 
 *
 * @param [in]  unit            - unit id
 * @param [in]  port            - logical port
 * @param [in]  vlan_tag        - 
 */
int pm4x10_port_vlan_tag_get(int unit, int port, pm_info_t pm_info, portmod_vlan_tag_t* vlan_tag)
{
    return (xlmac_rx_vlan_tag_get (unit, port, (int*)&vlan_tag->outer_vlan_tag,
                                   (int*)&vlan_tag->inner_vlan_tag));
}

/*! 
 * pm4x10_port_duplex_set
 *
 * @brief duplex set. 
 *
 * @param [in]  unit            - unit id
 * @param [in]  port            - logical port
 * @param [in]  enable        - 
 */
int pm4x10_port_duplex_set(int unit, int port, pm_info_t pm_info,int enable)
{
    return (xlmac_duplex_set (unit, port, enable)); 
}


/*! 
 * pm4x10_port_duplex_get
 *
 * @brief duplex get. 
 *
 * @param [in]  unit            - unit id
 * @param [in]  port            - logical port
 * @param [in]  vlan_tag        - 
 */
int pm4x10_port_duplex_get(int unit, int port, pm_info_t pm_info, int* enable)
{
    return (xlmac_duplex_get (unit, port, enable)); 
}

/*!
 * pm4x10_port_speed_get
 *
 * @brief duplex get.
 *
 * @param [in]  unit            - unit id
 * @param [in]  port            - logical port
 * @param [in]  speed         -
 */
int pm4x10_port_speed_get(int unit, int port, pm_info_t pm_info, int* speed)
{
    phymod_phy_access_t phy_access[1+MAX_PHYN];
    phymod_phy_inf_config_t config;
    int nof_phys, rv;

    rv = portmod_port_chain_phy_access_get(unit, port, pm_info,
                                           phy_access ,(1+MAX_PHYN),
                                           &nof_phys);
    if (rv) return(rv);

    rv = phymod_phy_interface_config_get(&phy_access[0], 0,0,  &config);
    if (rv) return (rv);

    *speed = config.data_rate;
    return (rv);
}

int pm4x10_port_phy_reg_read(int unit, int port, pm_info_t pm_info, int flags, int reg_addr, uint32* value)
{
    phymod_phy_access_t phy_access[1+MAX_PHYN];
    int nof_phys;
    SOC_INIT_FUNC_DEFS;

    _SOC_IF_ERR_EXIT
        (portmod_port_chain_phy_access_get(unit, port, pm_info, 
                                           phy_access ,(1+MAX_PHYN),
                                           &nof_phys));
    _SOC_IF_ERR_EXIT
        (portmod_port_phychain_reg_read(unit, port, phy_access, nof_phys, flags, reg_addr,value));

exit:
    SOC_FUNC_RETURN;
}

int pm4x10_port_phy_reg_write(int unit, int port, pm_info_t pm_info, int flags, int reg_addr, uint32 value)
{
    phymod_phy_access_t phy_access[1+MAX_PHYN];
    int nof_phys;
    SOC_INIT_FUNC_DEFS;

    _SOC_IF_ERR_EXIT
        (portmod_port_chain_phy_access_get(unit, port, pm_info, 
                                           phy_access ,(1+MAX_PHYN),
                                           &nof_phys));
    _SOC_IF_ERR_EXIT
        (portmod_port_phychain_reg_write(unit, port, phy_access, nof_phys, flags, reg_addr,value));

exit:
    SOC_FUNC_RETURN;
}

int pm4x10_port_check_legacy_phy (int unit, int port, pm_info_t pm_info, int *legacy_phy)
{
    phymod_phy_access_t phy_access[1+MAX_PHYN];
    int nof_phys, xphy_id, is_legacy_phy;
    int port_index;
    uint32 bitmap;

    SOC_INIT_FUNC_DEFS;

    _SOC_IF_ERR_EXIT(_pm4x10_port_index_get (unit, port, pm_info, &port_index,
                                             &bitmap));

    *legacy_phy = 0;

    if (PM_4x10_INFO(pm_info)->nof_phys[port_index] == 0) {
        _SOC_EXIT_WITH_ERR(SOC_E_UNAVAIL,
                           (_SOC_MSG("phy reset is not supported")));
    }

    _SOC_IF_ERR_EXIT(portmod_port_chain_phy_access_get(unit, port, pm_info, 
                                                       phy_access ,(1+MAX_PHYN),
                                                       &nof_phys));
     while (nof_phys > 1) {
         nof_phys--;
         xphy_id = phy_access[nof_phys].access.addr;
         _SOC_IF_ERR_EXIT(portmod_xphy_is_legacy_phy_get(unit, xphy_id, &is_legacy_phy));
         if (is_legacy_phy) {
             *legacy_phy = 1;
             break;
         }          
     }

exit:
    SOC_FUNC_RETURN;
}

int pm4x10_port_reset_set (int unit, int port, pm_info_t pm_info, 
                           int reset_mode, int opcode, int direction)
{
    phymod_core_access_t core_access[1+MAX_PHYN];
    int nof_phys;
    int port_index;
    uint32 bitmap;

    SOC_INIT_FUNC_DEFS;

    _SOC_IF_ERR_EXIT(_pm4x10_port_index_get (unit, port, pm_info, &port_index,
                                             &bitmap));

    if (PM_4x10_INFO(pm_info)->nof_phys[port_index] == 0) {
        _SOC_EXIT_WITH_ERR(SOC_E_UNAVAIL,
                           (_SOC_MSG("phy reset is not supported")));
    }

    _SOC_IF_ERR_EXIT
        (portmod_port_chain_phy_access_get(unit, port, pm_info,
                                           (phymod_phy_access_t*)core_access,
                                           (1+MAX_PHYN), &nof_phys));

    _SOC_IF_ERR_EXIT
        (portmod_port_phychain_core_reset_set(unit, core_access, nof_phys,
                                            (phymod_reset_mode_t)reset_mode, 
                                            (phymod_reset_direction_t)direction));

exit:
    SOC_FUNC_RETURN;
}

int pm4x10_port_reset_get (int unit, int port, pm_info_t pm_info, 
                           int reset_mode, int opcode, int* direction)
{
    phymod_core_access_t core_access[1+MAX_PHYN];
    int nof_phys;
    int port_index;
    uint32 bitmap;

    SOC_INIT_FUNC_DEFS;

    _SOC_IF_ERR_EXIT(_pm4x10_port_index_get (unit, port, pm_info, &port_index,
                                             &bitmap));

    if (PM_4x10_INFO(pm_info)->nof_phys[port_index] == 0) {
        _SOC_EXIT_WITH_ERR(SOC_E_UNAVAIL,
                           (_SOC_MSG("phy reset is not supported")));
    }

    _SOC_IF_ERR_EXIT
        (portmod_port_chain_phy_access_get(unit, port, pm_info,
                                           (phymod_phy_access_t*)core_access,
                                           (1+MAX_PHYN), &nof_phys));

    _SOC_IF_ERR_EXIT
        (portmod_port_phychain_core_reset_get(unit, core_access, nof_phys,
                       (phymod_reset_mode_t)reset_mode, 
                       (phymod_reset_direction_t*)direction));

exit:
    SOC_FUNC_RETURN;
}

/*Port remote Adv get*/
int pm4x10_port_adv_remote_get (int unit, int port, pm_info_t pm_info, 
                                int* ability_mask)
{
    return (0);
}

/*get port fec enable*/
int pm4x10_port_fec_enable_get(int unit, int port, pm_info_t pm_info, 
                                 uint32_t* enable)
{
    phymod_phy_access_t phy_access[1+MAX_PHYN];
    int   nof_phys = 0;

    SOC_INIT_FUNC_DEFS;

    _SOC_IF_ERR_EXIT
        (portmod_port_chain_phy_access_get(unit, port, pm_info, 
                                           phy_access ,(1+MAX_PHYN),
                                           &nof_phys));

    _SOC_IF_ERR_EXIT
        (portmod_port_phychain_fec_enable_get(unit, port, phy_access, nof_phys, enable));

exit:
    SOC_FUNC_RETURN;
}

/*set port fec enable*/
int pm4x10_port_fec_enable_set(int unit, int port, pm_info_t pm_info, 
                                 uint32_t enable)
{
    phymod_phy_access_t phy_access[1+MAX_PHYN];
    int   nof_phys = 0;

    SOC_INIT_FUNC_DEFS;

    _SOC_IF_ERR_EXIT
        (portmod_port_chain_phy_access_get(unit, port, pm_info, 
                                           phy_access ,(1+MAX_PHYN),
                                           &nof_phys));

    _SOC_IF_ERR_EXIT
        (portmod_port_phychain_fec_enable_set(unit, port, phy_access, nof_phys, enable));

exit:
    SOC_FUNC_RETURN;
}

/*get port auto negotiation local ability*/
int pm4x10_port_ability_advert_get(int unit, int port, pm_info_t pm_info, uint32 phy_flags,
                                 portmod_port_ability_t* ability)
{
    phymod_phy_access_t              phy_access[1+MAX_PHYN];
    phymod_autoneg_ability_t         an_ability;
    portmod_port_ability_t port_ability;

    int     nof_phys = 0, port_index, flags;
    uint32  bitmap;
    int     rv;
    int phy_index, xphy_id, is_legacy_phy;

    SOC_INIT_FUNC_DEFS;

    _SOC_IF_ERR_EXIT
        (portmod_port_chain_phy_access_get(unit, port, pm_info, 
                                           phy_access ,(1+MAX_PHYN),
                                           &nof_phys));
    _SOC_IF_ERR_EXIT
        (portmod_port_phychain_autoneg_ability_get(unit, port, phy_access, nof_phys, phy_flags,
                                                   &an_ability, &port_ability));
    phy_index = nof_phys -1;
    if (phy_index) {
        xphy_id = phy_access[phy_index].access.addr;
        _SOC_IF_ERR_EXIT(portmod_xphy_is_legacy_phy_get(unit, xphy_id, &is_legacy_phy));

        if (is_legacy_phy) {
            sal_memcpy(ability, &port_ability, sizeof(portmod_port_ability_t));
        }

    } else {
        /*
         * SGMII speed should be initialized to count but is initialized to 0
         * which is treated as 10M. so for sesto mask this out.
         */
        if (nof_phys > 1) {
#ifdef PHYMOD_SESTO_SUPPORT
            if (phy_access[nof_phys-1].type == phymodDispatchTypeSesto) {
                an_ability.sgmii_speed =  phymod_CL37_SGMII_Count;
            }
#endif /* PHYMOD_SESTO_SUPPORT */
        }
        portmod_common_phy_to_port_ability(&an_ability, ability);
    }
    _SOC_IF_ERR_EXIT(_pm4x10_port_index_get (unit, port, pm_info, &port_index,
                                             &bitmap));

    rv = PM4x10_INTERFACE_TYPE_GET(unit, pm_info, &ability->interface,
                                   port_index);
    _SOC_IF_ERR_EXIT(rv);

    _SOC_IF_ERR_EXIT(xlmac_encap_get (unit, port, &flags, &ability->encap)); 

exit:
    SOC_FUNC_RETURN;
}

/*
 * Function:
 *     _pm4x10_port_to_phy_ability 
 * Purpose:
 *      This function will translate the portAbility request, which passed 
 *      from BCM api to anAbility format required by phymod api.
 *      
 * Parameters:
 *      portAbility         -(IN)  port an ability request from BCM Api 
 *      anAbility           -(OUT) phymod an ability to phymod api.
 *      port_num_lanes      -(IN)  number of lane
 *      line_interface      -(IN)  line interface
 *      cx4_10g             -(IN)  cx4 port
 *      an_cl72             -(IN)  an cl72 configuration
 *      an_fec              -(IN)  fec configuration 
 *      an_hg_mode          -(IN)  hg_mode
 * Returns:
 *      None
 */

void _pm4x10_port_to_phy_ability (portmod_port_ability_t *portAbility, 
                                  phymod_autoneg_ability_t *anAbility,
                                  int port_num_lanes, 
                                  int line_interface, 
                                  int cx4_10g,
                                  int an_cl72,
                                  int an_fec,
                                  int hg_mode)
{
    uint32_t                  an_tech_ability   = 0;
    uint32_t                  an_bam37_ability  = 0;
    uint32_t                  an_bam73_ability  = 0;
    _shr_port_mode_t          speed_full_duplex = portAbility->speed_full_duplex;
    phymod_autoneg_ability_t_init(anAbility);

    /* 
     * an_tech_ability 
     */
    if (port_num_lanes == 10 ||
        port_num_lanes == 12 ) {
        if (speed_full_duplex & SOC_PA_SPEED_100GB) { 
            PHYMOD_AN_CAP_100G_CR10_SET(an_tech_ability);
        }
    } else if (port_num_lanes == 4) {
        if (speed_full_duplex & SOC_PA_SPEED_100GB) { 
        /*
         * This is only possible for port with external
         * phy in gearbox mode.
         */
            if (portAbility->medium) {
                if (portAbility->medium & SOC_PA_MEDIUM_BACKPLANE) {
                    PHYMOD_AN_CAP_100G_KR4_SET(an_tech_ability);
                } else if (portAbility->medium & SOC_PA_MEDIUM_COPPER) {
                    PHYMOD_AN_CAP_100G_CR4_SET(an_tech_ability);
                } else {
                    PHYMOD_AN_CAP_100G_KR4_SET(an_tech_ability);
                }
            } else {
                if (line_interface == SOC_PORT_IF_CR4){
                    PHYMOD_AN_CAP_100G_CR4_SET(an_tech_ability);
                } else {
                    PHYMOD_AN_CAP_100G_KR4_SET(an_tech_ability);
                }
            }
        }
        if (speed_full_duplex & SOC_PA_SPEED_40GB) {

            if (portAbility->medium) {
            /*
             * If the medium is defined, medium configuration will be
             * used to determine which interface to advertise.  
             */
                if (portAbility->medium & SOC_PA_MEDIUM_BACKPLANE) {
                    PHYMOD_AN_CAP_40G_KR4_SET(an_tech_ability);
                } else if (portAbility->medium & SOC_PA_MEDIUM_COPPER) {
                    PHYMOD_AN_CAP_40G_CR4_SET(an_tech_ability);
                } else {
                    PHYMOD_AN_CAP_40G_KR4_SET(an_tech_ability);
                }
            } else {
            /* 
             * If the medium is not defined, it will look at line 
             * interface.  This case is for backward compatiblility
             * during transition time, will remove this support later.
             */
                if (line_interface ==SOC_PORT_IF_CR4) {
                    PHYMOD_AN_CAP_40G_CR4_SET(an_tech_ability);
                } else {
                    PHYMOD_AN_CAP_40G_KR4_SET(an_tech_ability);
                }
            }
        }

        if (speed_full_duplex & SOC_PA_SPEED_10GB) {
            PHYMOD_AN_CAP_10G_KX4_SET(an_tech_ability);
        }
    } else if (port_num_lanes == 1) {
        /* port_num_lanes == 1 */
        if (speed_full_duplex & SOC_PA_SPEED_10GB) {
            PHYMOD_AN_CAP_10G_KR_SET(an_tech_ability);
        }
        if (speed_full_duplex & SOC_PA_SPEED_2500MB) {
            PHYMOD_AN_CAP_2P5G_X_SET(an_tech_ability);
        }
        if (speed_full_duplex & SOC_PA_SPEED_5000MB) {
            PHYMOD_AN_CAP_5G_KR1_SET(an_tech_ability);
        }
        if (speed_full_duplex & SOC_PA_SPEED_1000MB) {
            PHYMOD_AN_CAP_1G_KX_SET(an_tech_ability);
        }
    }

    /* 
     * an_bam73_ability 
     */
    if (port_num_lanes == 2) {     /* 2 lanes */
        if (speed_full_duplex & SOC_PA_SPEED_20GB) {
            if (portAbility->medium) {
                if (portAbility->medium & SOC_PA_MEDIUM_BACKPLANE) {
                    PHYMOD_BAM_CL73_CAP_20G_KR2_SET(an_bam73_ability);
                } else if (portAbility->medium & SOC_PA_MEDIUM_COPPER) {
                    PHYMOD_BAM_CL73_CAP_20G_CR2_SET(an_bam73_ability);
                } else {
                    PHYMOD_BAM_CL73_CAP_20G_KR2_SET(an_bam73_ability);
                }
            } else {
                if (line_interface ==SOC_PORT_IF_CR2) {
                    PHYMOD_BAM_CL73_CAP_20G_CR2_SET(an_bam73_ability);
                } else {
                    PHYMOD_BAM_CL73_CAP_20G_KR2_SET(an_bam73_ability);
                }
            }
        }
    } 

    /* 
     * an_bam37_ability 
     */
    if (port_num_lanes == 4) {
        if (speed_full_duplex & SOC_PA_SPEED_40GB) {
            PHYMOD_BAM_CL37_CAP_40G_SET(an_bam37_ability);
        }

        if (speed_full_duplex & SOC_PA_SPEED_30GB) {
            PHYMOD_BAM_CL37_CAP_31P5G_SET(an_bam37_ability);
        }

        if (speed_full_duplex & SOC_PA_SPEED_25GB) {
            PHYMOD_BAM_CL37_CAP_25P455G_SET(an_bam37_ability);
        }

        if (speed_full_duplex & SOC_PA_SPEED_21GB) {
            PHYMOD_BAM_CL37_CAP_21G_X4_SET(an_bam37_ability);
        }

        if (speed_full_duplex & SOC_PA_SPEED_20GB) {
            PHYMOD_BAM_CL37_CAP_20G_X4_SET(an_bam37_ability);
            PHYMOD_BAM_CL37_CAP_20G_X4_CX4_SET(an_bam37_ability);
        }

        if (speed_full_duplex & SOC_PA_SPEED_16GB) {
            PHYMOD_BAM_CL37_CAP_16G_X4_SET(an_bam37_ability);
        }

        if (speed_full_duplex & SOC_PA_SPEED_15GB) {
            PHYMOD_BAM_CL37_CAP_15G_X4_SET(an_bam37_ability);
        }

        if (speed_full_duplex & SOC_PA_SPEED_13GB) {
            PHYMOD_BAM_CL37_CAP_13G_X4_SET(an_bam37_ability);
        }

        if (speed_full_duplex & SOC_PA_SPEED_10GB) {
            if (cx4_10g) {
                PHYMOD_BAM_CL37_CAP_10G_CX4_SET(an_bam37_ability);
            } else {
                PHYMOD_BAM_CL37_CAP_10G_HIGIG_SET(an_bam37_ability);
            }
        }

    } else if (port_num_lanes == 2) {     /* 2 lanes */

        if (speed_full_duplex & SOC_PA_SPEED_20GB){
            PHYMOD_BAM_CL37_CAP_20G_X2_SET(an_bam37_ability);
            PHYMOD_BAM_CL37_CAP_20G_X2_CX4_SET(an_bam37_ability);
        }

        if (speed_full_duplex & SOC_PA_SPEED_16GB) {
            PHYMOD_BAM_CL37_CAP_15P75G_R2_SET(an_bam37_ability);
        }

        if (speed_full_duplex & SOC_PA_SPEED_13GB) {
            PHYMOD_BAM_CL37_CAP_12P7_DXGXS_SET(an_bam37_ability);
        }

        if (speed_full_duplex & SOC_PA_SPEED_11GB) {
            PHYMOD_BAM_CL37_CAP_10P5G_DXGXS_SET(an_bam37_ability);
        }

        if (speed_full_duplex & SOC_PA_SPEED_10GB) {
            PHYMOD_BAM_CL37_CAP_10G_X2_CX4_SET(an_bam37_ability);
            PHYMOD_BAM_CL37_CAP_10G_DXGXS_SET(an_bam37_ability);
        }
    } else if (port_num_lanes == 1) {

        if (speed_full_duplex & SOC_PA_SPEED_2500MB) {
            PHYMOD_BAM_CL37_CAP_2P5G_SET(an_bam37_ability);
        }
    }

    anAbility->an_cap = an_tech_ability;
    anAbility->cl73bam_cap = an_bam73_ability;
    anAbility->cl37bam_cap = an_bam37_ability;

    switch (portAbility->pause & (SOC_PA_PAUSE_TX | SOC_PA_PAUSE_RX)) {
    case SOC_PA_PAUSE_TX:
        PHYMOD_AN_CAP_ASYM_PAUSE_SET(anAbility);
        break;
    case SOC_PA_PAUSE_RX:
        /* an_adv |= MII_ANA_C37_PAUSE | MII_ANA_C37_ASYM_PAUSE; */
        PHYMOD_AN_CAP_ASYM_PAUSE_SET(anAbility);
        PHYMOD_AN_CAP_SYMM_PAUSE_SET(anAbility);
        break;
    case SOC_PA_PAUSE_TX | SOC_PA_PAUSE_RX:
        PHYMOD_AN_CAP_SYMM_PAUSE_SET(anAbility);
        break;
    }

    if ((speed_full_duplex & SOC_PA_SPEED_10MB) ||
        (speed_full_duplex & SOC_PA_SPEED_100MB) ||
        (speed_full_duplex & SOC_PA_SPEED_1000MB)) {
        if (portAbility->medium) {
            if (!(portAbility->medium & SOC_PA_MEDIUM_FIBER)) {

                PHYMOD_AN_CAP_SGMII_SET(anAbility);

                /* also set the sgmii speed */
                if (portAbility->speed_full_duplex & SOC_PA_SPEED_1000MB) {
                    anAbility->sgmii_speed = phymod_CL37_SGMII_1000M;
                } else if (portAbility->speed_full_duplex & SOC_PA_SPEED_100MB) {
                    anAbility->sgmii_speed = phymod_CL37_SGMII_100M;
                } else if (portAbility->speed_full_duplex & SOC_PA_SPEED_10MB) {
                    anAbility->sgmii_speed = phymod_CL37_SGMII_10M;
                } else {
                    anAbility->sgmii_speed = phymod_CL37_SGMII_1000M;
                }
            }
        } else {
            if (SOC_PORT_IF_SGMII == line_interface) {

                PHYMOD_AN_CAP_SGMII_SET(anAbility);

                /* also set the sgmii speed */
                if (portAbility->speed_full_duplex & SOC_PA_SPEED_1000MB) {
                    anAbility->sgmii_speed = phymod_CL37_SGMII_1000M;
                } else if (portAbility->speed_full_duplex & SOC_PA_SPEED_100MB) {
                    anAbility->sgmii_speed = phymod_CL37_SGMII_100M;
                } else if (portAbility->speed_full_duplex & SOC_PA_SPEED_10MB) {
                    anAbility->sgmii_speed = phymod_CL37_SGMII_10M;
                } else {
                    anAbility->sgmii_speed = phymod_CL37_SGMII_1000M;
                }
            } else {
                if (portAbility->speed_full_duplex & SOC_PA_SPEED_100MB) {
                    PHYMOD_AN_CAP_100M_SET(anAbility->an_cap);
                }
            }
        }
    }

    /* next check if we need to set cl37 attribute */
    if (an_cl72) {
        anAbility->an_cl72 = 1;
    }
    if (hg_mode) {
        anAbility->an_hg2 = 1;
    }

    /*
     * check if Cl74 fec is requested from AN BCM api.
     */
    if (portAbility->fec & SOC_PA_FEC_NONE) {
        PHYMOD_AN_FEC_OFF_SET(anAbility->an_fec);
    } else if (portAbility->fec & SOC_PA_FEC_CL74) {
        PHYMOD_AN_FEC_CL74_SET(anAbility->an_fec);
    }

    /*
     * check if Cl74 fec is requested from config.
     * spn_PHY_AN_FEC is only for CL74
     */
    if (an_fec) {
        PHYMOD_AN_FEC_CL74_SET(anAbility->an_fec);
    }
}

int pm4x10_port_ability_advert_set(int unit, int port, pm_info_t pm_info, uint32 phy_flags,
                                 portmod_port_ability_t* ability)
{
    phymod_phy_access_t phy_access[1+MAX_PHYN];
    int   nof_phys = 0, port_index, rv;
    uint32 bitmap, interface_modes;
    portmod_port_ability_t      port_ability;
    phymod_autoneg_ability_t    an_ability;
    portmod_access_get_params_t params;

    int port_num_lanes, line_interface, cx4_10g;
    soc_port_if_t temp_line_intf;
    int an_cl72, an_fec, hg_mode;
    int total_num_lanes = 0;
    int line_side;
    SOC_INIT_FUNC_DEFS;

    _SOC_IF_ERR_EXIT(_pm4x10_port_index_get (unit, port, pm_info, &port_index, 
                                             &bitmap));

    _SOC_IF_ERR_EXIT(portmod_access_get_params_t_init(unit, &params));
    params.lane = -1;
    params.phyn = 0;
    params.sys_side = PORTMOD_SIDE_LINE;

    _SOC_IF_ERR_EXIT
        (pm4x10_port_ability_local_get(unit, port, pm_info, phy_flags, &port_ability));

    /* Make sure to advertise only abilities supported by the port */
    port_ability.pause             &= ability->pause;
    port_ability.interface         &= ability->interface;
    port_ability.medium            &= ability->medium;
    port_ability.eee               &= ability->eee;
    port_ability.loopback          &= ability->loopback;
    port_ability.flags             &= ability->flags;
    port_ability.speed_half_duplex &= ability->speed_half_duplex;
    port_ability.speed_full_duplex &= ability->speed_full_duplex;
    port_ability.fec               &= ability->fec;

    SHR_BITCOUNT_RANGE(&bitmap, port_num_lanes, 0, PM4X10_LANES_PER_CORE);
    rv = PM4x10_INTERFACE_TYPE_GET(unit, pm_info, &line_interface, port_index);
    _SOC_IF_ERR_EXIT(rv);
    rv = PM4x10_CX4_10G_GET(unit, pm_info, &cx4_10g, port_index);
    _SOC_IF_ERR_EXIT(rv);
    rv = PM4x10_AN_CL72_GET(unit, pm_info, &an_cl72, port_index);
    _SOC_IF_ERR_EXIT(rv);
    rv = PM4x10_AN_FEC_GET(unit, pm_info, &an_fec, port_index);
    _SOC_IF_ERR_EXIT(rv);
    rv = PM4x10_INTERFACE_MODES_GET(unit, pm_info, &interface_modes,
                                    port_index);
    _SOC_IF_ERR_EXIT(rv);
    hg_mode = (interface_modes & PHYMOD_INTF_MODES_HIGIG)? 1: 0;


    _SOC_IF_ERR_EXIT
        (portmod_port_chain_phy_access_get(unit, port, pm_info, 
                                           phy_access ,(1+MAX_PHYN),
                                           &nof_phys));

    /* 
     * if line interface is CAUI -- check it is 4x25 or 10x10
     * change the line interface for this purpose.
     *
     * Default interface for 100G is CR10. However if line interface is Falcon
     * the interface should be CR4 (not CR10). Fix the line interface here..,
     */ 
    if (nof_phys > 1) {
      portmod_port_line_interface_map_for_xphy(unit, &(phy_access[nof_phys-1]), port, line_interface, &temp_line_intf);
      line_interface = temp_line_intf;
    }

    /*
     * Get the total number of line side lanes, if it is triple core mode (PM12x10).
     * and update the port_num_lanes with the new count.
     */

    if (interface_modes & PHYMOD_INTF_MODES_TRIPLE_CORE) {
        line_side = TRUE ;
        _SOC_IF_ERR_EXIT
            (portmod_port_lane_count_get(unit, port, line_side,
                                        &total_num_lanes));
        port_num_lanes = total_num_lanes;
    }

    _pm4x10_port_to_phy_ability(&port_ability, &an_ability,
                                port_num_lanes, line_interface, cx4_10g,
                                an_cl72, an_fec, hg_mode);

    rv = PM4x10_AN_MASTER_LANE_GET(unit, pm_info, &an_ability.an_master_lane,
                                   port_index);
    _SOC_IF_ERR_EXIT(rv);

    _SOC_IF_ERR_EXIT
        (portmod_port_phychain_autoneg_ability_set(unit, port, phy_access, nof_phys, phy_flags, &an_ability, &port_ability));

    LOG_VERBOSE(BSL_LS_BCM_PORT,
                (BSL_META_U(unit,
                     "Speed(HD=0x%08x, FD=0x%08x) Pause=0x%08x orig(HD=0x%08x, FD=0x%08x) \n"
                     "Interface=0x%08x Medium=0x%08x Loopback=0x%08x Flags=0x%08x\n"),
                     port_ability.speed_half_duplex,
                     port_ability.speed_full_duplex,
                     port_ability.pause,
                     ability->speed_half_duplex,
                     ability->speed_full_duplex,
                     port_ability.interface,
                     port_ability.medium, port_ability.loopback,
                     port_ability.flags));
exit:
    SOC_FUNC_RETURN;
}

/*Port ability remote Adv get*/
int pm4x10_port_ability_remote_get(int unit, int port, pm_info_t pm_info, uint32 phy_flags,
                                       portmod_port_ability_t* ability)
{
    phymod_phy_access_t phy_access[1+MAX_PHYN];
    int                         nof_phys;
    portmod_port_ability_t      port_ability;
    phymod_autoneg_ability_t    an_ability;

    SOC_INIT_FUNC_DEFS;

    sal_memset(&port_ability, 0, sizeof(portmod_port_ability_t));

    _SOC_IF_ERR_EXIT
        (portmod_port_chain_phy_access_get(unit, port, pm_info, 
                                           phy_access ,(1+MAX_PHYN),
                                           &nof_phys));

    _SOC_IF_ERR_EXIT
        (portmod_port_phychain_autoneg_remote_ability_get(unit, port, phy_access, nof_phys, phy_flags, &an_ability));

    portmod_common_phy_to_port_ability(&an_ability, ability);

exit:
    SOC_FUNC_RETURN;
}

int _xlport_rx_control_set(int unit, int port, int value)
{
    int rv = SOC_E_UNAVAIL;
    uint32 rval32 = 0;

    if (SOC_REG_IS_VALID(unit,XLPORT_MAC_RSV_MASKr)) {
        rv = READ_XLPORT_MAC_RSV_MASKr(unit, port, &rval32);
        if (SOC_SUCCESS(rv)) {
            /* XLPORT_MAC_RSV_MASK: Bit 11 Control Frame recieved
             * Enable  Control Frame : Set 0. Packet go through
             * Disable Control Frame : Set 1. Packet is purged.
             */
            if(value) {
                rval32 &= ~(1 << 11);
            } else {
                rval32 |= (1 << 11);
            }
            rv = WRITE_XLPORT_MAC_RSV_MASKr(unit, port, rval32);
        }
    }

    return rv;
}

int pm4x10_port_rx_control_set (int unit, int port, pm_info_t pm_info,
                                 const portmod_rx_control_t* rx_ctrl)
{
    int phy_acc;
    uint32 reg_val;

    PM4x10_PHY_ACCESS_GET(unit, port, pm_info, phy_acc);
    SOC_IF_ERROR_RETURN(READ_XLPORT_MAC_RSV_MASKr(unit, phy_acc, &reg_val));

    if (rx_ctrl->flags & PORTMOD_MAC_PASS_CONTROL_FRAME) {
        if (rx_ctrl->pass_control_frames) {
            reg_val &= ~(1 << 11);
        } else{
            reg_val |= (1 << 11);
        }
    }

    if (rx_ctrl->flags & PORTMOD_MAC_PASS_PFC_FRAME) {
        if (rx_ctrl->pass_pfc_frames) {
            reg_val &= ~(1 << 18);
        } else{
            reg_val |= (1 << 18);
        }
    }

    if (rx_ctrl->flags & PORTMOD_MAC_PASS_PAUSE_FRAME) {
        if (rx_ctrl->pass_pause_frames) {
            reg_val &= ~(1 << 12);
        } else{
            reg_val |= (1 << 12);
        }
    }

    SOC_IF_ERROR_RETURN(WRITE_XLPORT_MAC_RSV_MASKr(unit, phy_acc, reg_val));

    return (SOC_E_NONE);
}

int pm4x10_port_rx_control_get (int unit, int port, pm_info_t pm_info,
                                         portmod_rx_control_t* rx_ctrl)
{
    int phy_acc;
    uint32 reg_val;

    PM4x10_PHY_ACCESS_GET(unit, port, pm_info, phy_acc);
    SOC_IF_ERROR_RETURN(READ_XLPORT_MAC_RSV_MASKr(unit, phy_acc, &reg_val));
    rx_ctrl->pass_control_frames = ((reg_val & 0x800) >> 11) ? 0 : 1;
    rx_ctrl->pass_pfc_frames = ((reg_val & 0x40000) >> 18) ? 0 : 1;
    rx_ctrl->pass_pause_frames = ((reg_val & 0x1000) >> 12) ? 0 : 1;

    return (SOC_E_NONE);
}

int pm4x10_port_tx_mac_sa_set(int unit, int port, pm_info_t pm_info, sal_mac_addr_t mac_addr)
{
    return (xlmac_tx_mac_sa_set(unit, port, mac_addr));
}


int pm4x10_port_tx_mac_sa_get(int unit, int port, pm_info_t pm_info, sal_mac_addr_t mac_addr)
{
    return (xlmac_tx_mac_sa_get(unit, port, mac_addr));
}

int pm4x10_port_rx_mac_sa_set(int unit, int port, pm_info_t pm_info, sal_mac_addr_t mac_addr)
{
    return (xlmac_rx_mac_sa_set(unit, port, mac_addr));
}


int pm4x10_port_rx_mac_sa_get(int unit, int port, pm_info_t pm_info, sal_mac_addr_t mac_addr)
{
    return (xlmac_rx_mac_sa_get(unit, port, mac_addr));
}

int pm4x10_port_tx_average_ipg_set (int unit, int port, 
                                    pm_info_t pm_info, int value)
{
    return (xlmac_tx_average_ipg_set(unit, port, value));
}


int pm4x10_port_tx_average_ipg_get (int unit, int port, 
                                    pm_info_t pm_info, int* value)
{
    return (xlmac_tx_average_ipg_get(unit, port, value));
}

int pm4x10_port_update (int unit, int port, pm_info_t pm_info,
                        const portmod_port_update_control_t* update_control)
{
    int link, flags = 0;
    int duplex = 0, tx_pause = 0, rx_pause = 0;
    phymod_autoneg_status_t autoneg;
    portmod_pause_control_t pause_control;
    portmod_port_ability_t local_advert, remote_advert;
    portmod_port_interface_config_t interface_config;
    int port_index, ref_clock, max_speed;
    uint32_t                    bitmap;
    phymod_phy_inf_config_t     phy_interface_config;
    soc_port_if_t   interface = SOC_PORT_IF_NULL;
    phymod_phy_access_t phy_access[1+MAX_PHYN];
    int nof_phys;

#if defined(INCLUDE_PHY_542XX)
    int sys_start_lane = -1;
    int sys_end_lane = -1;
    int line_start_lane = -1;
    int line_end_lane=-1;
    uint32 int_lane_mask=0xf;
    uint32_t serdes_link_status=0;
    phymod_phy_rx_lane_control_t rx_control;
#endif /* defined(INCLUDE_PHY_542XX) */
    SOC_INIT_FUNC_DEFS;

    _SOC_IF_ERR_EXIT(phymod_autoneg_status_t_init(&autoneg));
    _SOC_IF_ERR_EXIT(portmod_pause_control_t_init(unit, &pause_control));
    sal_memset(&local_advert, 0, sizeof(portmod_port_ability_t));
    sal_memset(&remote_advert, 0, sizeof(portmod_port_ability_t));

    _SOC_IF_ERR_EXIT
        (portmod_port_chain_phy_access_get(unit, port, pm_info,
                                           phy_access ,(1+MAX_PHYN),
                                           &nof_phys));

    if(update_control->link_status == -1) {
        _SOC_IF_ERR_EXIT(pm4x10_port_link_get(unit, port, pm_info,PORTMOD_INIT_F_EXTERNAL_MOST_ONLY, &link)); 
    } else {
        link = update_control->link_status;
    }

    _SOC_IF_ERR_EXIT(pm4x10_port_autoneg_status_get(unit, port, pm_info, &autoneg));

    /*
     * Update the interface type stored in the SW database when the port is in the AN mode
     * and link is up. The update has to be done before any possible early return in this
     * function or portmod_port_interface_config_get() will get outdated interface type
     * causing "BCM.0> ps" to show outdated interface types in the AN mode.
     */
    if (link && autoneg.enabled && autoneg.locked) {
        /* update interface*/
        _SOC_IF_ERR_EXIT(phymod_phy_inf_config_t_init(&phy_interface_config));
        _SOC_IF_ERR_EXIT(_pm4x10_port_index_get (unit, port, pm_info, &port_index, &bitmap));
        _SOC_IF_ERR_EXIT(PM4x10_MAX_SPEED_GET(unit, pm_info, &max_speed, port_index));

        ref_clock = PM_4x10_INFO(pm_info)->ref_clk;
        phy_interface_config.data_rate =  max_speed;
        phy_interface_config.ref_clock =  ref_clock;
        _SOC_IF_ERR_EXIT
            (portmod_port_phychain_interface_config_get(unit, port, phy_access, nof_phys,
                                                        0, ref_clock, &phy_interface_config,
                                                        PORTMOD_INIT_F_EXTERNAL_MOST_ONLY));
        _SOC_IF_ERR_EXIT
            (portmod_intf_from_phymod_intf(unit, phy_interface_config.interface_type, &interface));
        _SOC_IF_ERR_EXIT
            (PM4x10_INTERFACE_TYPE_SET(unit, pm_info, interface, port_index));
        _SOC_IF_ERR_EXIT
            (PM4x10_OUTMOST_PHY_INTERFACE_TYPE_SET(unit, pm_info, interface, port_index));
    }

#if defined(INCLUDE_PHY_542XX)
/* Following code applies to the phy 54290 */
    if (PORTMOD_PORT_UPDATE_F_UPDATE_SERDES_LINK_GET(update_control)){
        _SOC_IF_ERR_EXIT(_pm4x10_port_tricore_lane_info_get(unit, port, pm_info,
                                              &sys_start_lane, &sys_end_lane,
                                              &line_start_lane, &line_end_lane,
                                              &int_lane_mask));
        /* only look at the lanes that are used. */
        phy_access[0].access.lane_mask &= int_lane_mask;

        if (nof_phys > 1){
            _SOC_IF_ERR_EXIT
                (phymod_phy_link_status_get(&phy_access[0],&serdes_link_status));

            if(link) {
                _SOC_IF_ERR_EXIT
                    (phymod_phy_rx_lane_control_get(&phy_access[0],&rx_control));
                if(rx_control == phymodRxSquelchOn){
                    phymod_phy_rx_lane_control_set(&phy_access[0],
                                                   phymodRxSquelchOff);
                }
            } else {
                /* if external phy and link is down, internal serdes link is up,
                   force internal SerDes link to be down. */
                if(serdes_link_status){
                    phymod_phy_rx_lane_control_set(&phy_access[0],
                                                   phymodRxSquelchOn);
                    phymod_phy_rx_lane_control_set(&phy_access[0],
                                                   phymodRxSquelchOff);
                }
            }
        }

        /*
         * PORTMOD_PORT_UPDATE_F_UPDATE_SERDES_LINK is set for XGS devices. Since XGS devices
         * already update their MAC registers elsewhere, there is an early return below to
         * avoid redundant works.
         */
        return (SOC_E_NONE);
    }
#endif /* defined(INCLUDE_PHY_542XX) */



    if(!link) {
        /* PHY is down.  Disable the MAC. */
        if (PORTMOD_PORT_UPDATE_F_DISABLE_MAC_GET(update_control)) {
            _SOC_IF_ERR_EXIT(xlmac_enable_set(unit, port, 0, 0));
        }

        /* TBD - do we need phymod_link_down_event ? */
    } else {

        /* TBD - do we need phymod_link_up_event ? */

        if(autoneg.enabled && autoneg.locked) {
            /* update MAC */
            _SOC_IF_ERR_EXIT(pm4x10_port_interface_config_get(unit, port, pm_info, &interface_config, PORTMOD_INIT_F_EXTERNAL_MOST_ONLY));
            _SOC_IF_ERR_EXIT(xlmac_enable_set(unit, port, 0, 0));
            flags = XLMAC_SPEED_SET_FLAGS_SOFT_RESET_DIS;
            _SOC_IF_ERR_EXIT(xlmac_speed_set(unit, port, flags, interface_config.speed));
            _SOC_IF_ERR_EXIT(xlmac_enable_set(unit, port, 0, 1));
            /* update pause in MAC on the base of local and remote pause ability*/
            _SOC_IF_ERR_EXIT(pm4x10_port_duplex_get(unit, port, pm_info, &duplex));
            _SOC_IF_ERR_EXIT(pm4x10_port_ability_advert_get(unit, port, pm_info, PORTMOD_INIT_F_EXTERNAL_MOST_ONLY, &local_advert));
            _SOC_IF_ERR_EXIT(pm4x10_port_ability_remote_get(unit, port, pm_info, PORTMOD_INIT_F_EXTERNAL_MOST_ONLY, &remote_advert));
            /* IEEE 802.3 Flow Control Resolution.
                   * Please see $SDK/doc/pause-resolution.txt for more information. */
            if (duplex) {
                tx_pause = \
                    ((remote_advert.pause & SOC_PA_PAUSE_RX) && \
                    (local_advert.pause & SOC_PA_PAUSE_RX)) || \
                    ((remote_advert.pause & SOC_PA_PAUSE_RX) && \
                    !(remote_advert.pause & SOC_PA_PAUSE_TX) && \
                    (local_advert.pause & SOC_PA_PAUSE_TX)); 
                rx_pause = \
                    ((remote_advert.pause & SOC_PA_PAUSE_RX) && \
                    (local_advert.pause & SOC_PA_PAUSE_RX)) || \
                    ((local_advert.pause & SOC_PA_PAUSE_RX) && \
                    (remote_advert.pause & SOC_PA_PAUSE_TX) && \
                    !(local_advert.pause & SOC_PA_PAUSE_TX));
            }
            else {
                rx_pause = tx_pause = 0;
            }
            _SOC_IF_ERR_EXIT(pm4x10_port_pause_control_get(unit, port, pm_info, &pause_control));
            if ((pause_control.rx_enable != rx_pause) || (pause_control.tx_enable != tx_pause)) {
                pause_control.rx_enable = rx_pause;
                pause_control.tx_enable = tx_pause;
                _SOC_IF_ERR_EXIT(pm4x10_port_pause_control_set(unit, port, pm_info, &pause_control));
            }
        }

        _SOC_IF_ERR_EXIT(PM_DRIVER(pm_info)->f_portmod_port_lag_failover_status_toggle(unit, port, pm_info));
    }

exit:
    SOC_FUNC_RETURN;
}

int pm4x10_port_drv_name_get (int unit, int port, pm_info_t pm_info, 
                              char* buf, int len)
{
    strncpy(buf, "PM4X10 Driver", len);
    return (SOC_E_NONE);
}

int pm4x10_port_local_fault_status_clear(int unit, int port, pm_info_t pm_info)
{
    int lcl_fault, rmt_fault;

    SOC_IF_ERROR_RETURN (pm4x10_port_clear_rx_lss_status_get (unit, port, pm_info, &lcl_fault, &rmt_fault));
    if (lcl_fault) {
        SOC_IF_ERROR_RETURN (pm4x10_port_clear_rx_lss_status_set(unit, port, pm_info, 0, rmt_fault)); 
    }
    return (pm4x10_port_clear_rx_lss_status_set (unit, port, pm_info, 1, rmt_fault));
}

int pm4x10_port_remote_fault_status_clear(int unit, int port, pm_info_t pm_info)
{
    int lcl_fault, rmt_fault;

    SOC_IF_ERROR_RETURN (pm4x10_port_clear_rx_lss_status_get (unit, port, pm_info, &lcl_fault, &rmt_fault));
    if (rmt_fault) {
        SOC_IF_ERROR_RETURN (pm4x10_port_clear_rx_lss_status_set(unit, port, pm_info, lcl_fault, 0)); 
    }
    return (pm4x10_port_clear_rx_lss_status_set (unit, port, pm_info, lcl_fault, 1));
}


int pm4x10_port_clear_rx_lss_status_set (int unit, soc_port_t port, 
                           pm_info_t pm_info, int lcl_fault, int rmt_fault)
{
    return (xlmac_clear_rx_lss_status_set (unit, port, lcl_fault, rmt_fault));
}

int pm4x10_port_clear_rx_lss_status_get (int unit, soc_port_t port, 
                           pm_info_t pm_info, int *lcl_fault, int *rmt_fault)
{
    return (xlmac_clear_rx_lss_status_get (unit, port, lcl_fault, rmt_fault));
}

int pm4x10_port_lag_failover_status_toggle (int unit, soc_port_t port, pm_info_t pm_info)
{
    int phy_acc;
    uint32_t rval;
    int link = 0;
    soc_timeout_t to;

    SOC_INIT_FUNC_DEFS;

    PM4x10_PHY_ACCESS_GET(unit, port, pm_info, phy_acc);

    /* Link status to IPIPE is updated by H/W and driven based on both the
     * rising edge of XLPORT_LAG_FAILOVER_CONFIG.LINK_STATUS_UP and
     * actual link up status from Serdes.
     * In some loopback scenarios it may take longer time to see Serdes link up status.
     */
    soc_timeout_init(&to, 10000, 0);

    do {
        _SOC_IF_ERR_EXIT(pm4x10_port_mac_link_get(unit, port, pm_info, &link));
        if (link) {
            break;
        }
    } while(!soc_timeout_check(&to));

    /* Toggle link_status_up bit to notify IPIPE on link up */
    _SOC_IF_ERR_EXIT(READ_XLPORT_LAG_FAILOVER_CONFIGr(unit, phy_acc, &rval));
    soc_reg_field_set(unit, XLPORT_LAG_FAILOVER_CONFIGr, &rval, LINK_STATUS_UPf, 1);
    _SOC_IF_ERR_EXIT(WRITE_XLPORT_LAG_FAILOVER_CONFIGr(unit, phy_acc, rval));
    soc_reg_field_set(unit, XLPORT_LAG_FAILOVER_CONFIGr, &rval, LINK_STATUS_UPf, 0);
    _SOC_IF_ERR_EXIT(WRITE_XLPORT_LAG_FAILOVER_CONFIGr(unit, phy_acc, rval));

exit:
    SOC_FUNC_RETURN;
}

int pm4x10_port_lag_failover_loopback_set (int unit, soc_port_t port, 
                                        pm_info_t pm_info, int value)
{
    return (xlmac_lag_failover_loopback_set(unit, port, value));
}

int pm4x10_port_lag_failover_loopback_get (int unit, soc_port_t port, 
                                        pm_info_t pm_info, int *value)
{
    return(xlmac_lag_failover_loopback_get(unit, port, value));
}

int pm4x10_port_mode_set(int unit, soc_port_t port,
                     pm_info_t pm_info, const portmod_port_mode_info_t *mode)
{
    int phy_acc;

    PM4x10_PHY_ACCESS_GET(unit, port, pm_info, phy_acc);

    return (_xlport_mode_set(unit, phy_acc, mode->cur_mode));
}

int pm4x10_port_mode_get(int unit, soc_port_t port,
                     pm_info_t pm_info, portmod_port_mode_info_t *mode)
{
    int port_index, rv, phy_acc;
    uint32 bitmap;

    PM4x10_PHY_ACCESS_GET(unit, port, pm_info, phy_acc);

    rv = _pm4x10_port_index_get (unit, port, pm_info, &port_index, &bitmap);
    if (rv) return (rv);

    return(_xlport_mode_get(unit, phy_acc, port_index, &mode->cur_mode, &mode->lanes));
}

int
pm4x10_port_trunk_hwfailover_config_set(int unit, soc_port_t port, pm_info_t pm_info, int hw_count)
{
    int    old_failover_en=0, new_failover_en = 0;
    int    old_link_status_sel=0, new_link_status_sel=0;
    int    old_reset_flow_control=0, new_reset_flow_control = 0;
    int    lag_failover_lpbk;

    if (hw_count) {
        new_failover_en        = 1;
        new_link_status_sel    = 1;
        new_reset_flow_control = 1;
    }

    SOC_IF_ERROR_RETURN (xlmac_lag_failover_loopback_get(unit, port, &lag_failover_lpbk));
    if (lag_failover_lpbk) return (SOC_E_NONE);


    SOC_IF_ERROR_RETURN (xlmac_lag_failover_en_get(unit, port, &old_failover_en));
    SOC_IF_ERROR_RETURN (xlmac_sw_link_status_select_get (unit, port, &old_link_status_sel));

    if (old_failover_en     != new_failover_en ||
        old_link_status_sel != new_link_status_sel) {

        SOC_IF_ERROR_RETURN (xlmac_sw_link_status_select_set (unit, port, new_link_status_sel));
        SOC_IF_ERROR_RETURN (xlmac_lag_failover_en_set(unit, port, new_failover_en));
    }

    SOC_IF_ERROR_RETURN (xlmac_reset_fc_timers_on_link_dn_get(unit,
                                       port, &old_reset_flow_control));
    if (old_reset_flow_control != new_reset_flow_control) {
        SOC_IF_ERROR_RETURN (xlmac_reset_fc_timers_on_link_dn_set(unit, 
                                        port, new_reset_flow_control));
    }

    return 0;
}

int
pm4x10_port_trunk_hwfailover_config_get(int unit, soc_port_t port, pm_info_t pm_info, 
                                        int *enable)
{
    return (xlmac_lag_failover_en_get(unit, port, enable));
}


int
pm4x10_port_trunk_hwfailover_status_get(int unit, soc_port_t port, pm_info_t pm_info, 
                                        int *loopback)
{
    return (xlmac_lag_failover_loopback_get(unit, port, loopback));
}

int pm4x10_port_diag_ctrl(int unit, soc_port_t port, pm_info_t pm_info,
                      uint32 inst, int op_type, int op_cmd, const void *arg) 
{
    phymod_phy_access_t phy_access[1+MAX_PHYN];
    phymod_tx_t          ln_txparam[PM4X10_LANES_PER_CORE];
    int nof_phys;
    uint32 lane_map;
    portmod_access_get_params_t params;


    SOC_IF_ERROR_RETURN(portmod_access_get_params_t_init(unit, &params));

    if( PHY_DIAG_INST_DEV(inst) == PHY_DIAG_DEV_INT ) {
        params.phyn = 0 ;
    } else { 
        /* most external  PHY_DIAG_DEV_DFLT and PHY_DIAG_DEV_EXT */  
        params.phyn = -1 ;  
    }

    if(PHY_DIAG_INST_INTF(inst) == PHY_DIAG_INTF_SYS ){
        params.sys_side = PORTMOD_SIDE_SYSTEM;
    } else { /* line side is default */
        params.sys_side = PORTMOD_SIDE_LINE;
    }

    params.apply_lane_mask = 1;

    SOC_IF_ERROR_RETURN(pm4x10_port_phy_lane_access_get(unit, port, pm_info,
                                    &params, 1, phy_access, &nof_phys, NULL));

    lane_map = phy_access[0].access.lane_mask;

    switch(op_cmd) {
        case PHY_DIAG_CTRL_DSC:
            LOG_INFO(BSL_LS_SOC_PHY,
                     (BSL_META_U(unit,
                                 "pm4x10_port_diag_ctrl: "
                                 "u=%d p=%d PHY_DIAG_CTRL_DSC 0x%x\n"),
                      unit, port, PHY_DIAG_CTRL_DSC));

            SOC_IF_ERROR_RETURN
                (portmod_port_phychain_pmd_info_dump(unit, phy_access, nof_phys,
                                                 (void*)arg));

            break;

        case PHY_DIAG_CTRL_PCS:
            LOG_INFO(BSL_LS_SOC_PHY,
                     (BSL_META_U(unit,
                                 "pm4x10_port_diag_ctrl: "
                                 "u=%d p=%d PHY_DIAG_CTRL_PCS 0x%x\n"),
                      unit, port, PHY_DIAG_CTRL_PCS));

            SOC_IF_ERROR_RETURN
                (portmod_port_phychain_pcs_info_dump(unit, phy_access, nof_phys,
                                                    (void*)arg));
            break;

        case PHY_DIAG_CTRL_LINKMON_MODE:
            LOG_INFO(BSL_LS_SOC_PHY,
                     (BSL_META_U(unit,
                                 "pm4x10_port_diag_ctrl: "
                                 "u=%d p=%d PHY_DIAG_CTRL_LINKMON_MODE  0x%x\n"),
                      unit, port, PHY_DIAG_CTRL_LINKMON_MODE));

            if(phy_access[0].access.lane_mask){
                SOC_IF_ERROR_RETURN
                    (portmod_pm_phy_link_mon_enable_set(phy_access, nof_phys, PTR_TO_INT(arg)));
            }
            break;

        case PHY_DIAG_CTRL_LINKMON_STATUS:
            LOG_INFO(BSL_LS_SOC_PHY,
                     (BSL_META_U(unit,
                                 "pm4x10_port_diag_ctrl: "
                                 "u=%d p=%d PHY_DIAG_CTRL_LINKMON_STATUS 0x%x\n"),
                      unit, port, PHY_DIAG_CTRL_LINKMON_STATUS));

            if(phy_access[0].access.lane_mask){
                SOC_IF_ERROR_RETURN
                    (portmod_pm_phy_link_mon_status_get(phy_access, nof_phys));
            }
            break;


        default:
            if(op_type == PHY_DIAG_CTRL_SET) {
                LOG_INFO(BSL_LS_SOC_PHY,
                         (BSL_META_U(unit,
                                     "pm4x10_port_diag_ctrl: "
                                     "u=%d p=%d PHY_DIAG_CTRL_SET 0x%x\n"),
                          unit, port, PHY_DIAG_CTRL_SET));
                if (!SAL_BOOT_SIMULATION) {
                    if( !(phy_access->access.lane_mask == 0)){
                        SOC_IF_ERROR_RETURN(portmod_pm_phy_control_set(phy_access, nof_phys, op_cmd, ln_txparam, lane_map, PTR_TO_INT(arg)));
                    }
                }
            } else if(op_type == PHY_DIAG_CTRL_GET) {
                LOG_INFO(BSL_LS_SOC_PHY,
                         (BSL_META_U(unit,
                                     "pm4x10_port_diag_ctrl: "
                                     "u=%d p=%d PHY_DIAG_CTRL_GET 0x%x\n"),
                          unit, port, PHY_DIAG_CTRL_GET));
                if (!SAL_BOOT_SIMULATION) {
                    if( !(phy_access->access.lane_mask == 0)){
                        SOC_IF_ERROR_RETURN(portmod_pm_phy_control_get(phy_access,nof_phys,op_cmd, ln_txparam, lane_map, (uint32 *)arg));
                    } else {
                        *(uint32 *)arg = 0;
                    }
                }
            } else {
                return (SOC_E_UNAVAIL);
            }
            break ;
    }
    return (SOC_E_NONE);
}

int
pm4x10_port_ref_clk_get(int unit, soc_port_t port, pm_info_t pm_info, int *ref_clk)
{
    *ref_clk = pm_info->pm_data.pm4x10_db->ref_clk;

    return (SOC_E_NONE);
}

int pm4x10_port_lag_remove_failover_lpbk_get(int unit, int port, pm_info_t pm_info, int *val)
{
    return (xlmac_lag_remove_failover_lpbk_get(unit, port, val));
}

int pm4x10_port_lag_remove_failover_lpbk_set(int unit, int port, pm_info_t pm_info, int val)
{
    return (xlmac_lag_remove_failover_lpbk_set(unit, port, val));
}

int pm4x10_port_lag_failover_disable(int unit, int port, pm_info_t pm_info)
{
    return (xlmac_lag_failover_disable(unit, port));
}

int pm4x10_port_mac_ctrl_set(int unit, int port, 
                   pm_info_t pm_info, uint64 ctrl)
{
    return(xlmac_mac_ctrl_set(unit, port, ctrl));
}

int pm4x10_port_drain_cell_get(int unit, int port, 
           pm_info_t pm_info, portmod_drain_cells_t *drain_cells)
{
    return (xlmac_drain_cell_get(unit, port, drain_cells));
}

int pm4x10_port_drain_cell_stop (int unit, int port, 
           pm_info_t pm_info, const portmod_drain_cells_t *drain_cells)
{
    return(xlmac_drain_cell_stop (unit, port, drain_cells));
}

int pm4x10_port_drain_cell_start(int unit, int port, pm_info_t pm_info)
{
    return (xlmac_drain_cell_start(unit, port));
}

int pm4x10_port_txfifo_cell_cnt_get(int unit, int port,
                       pm_info_t pm_info, uint32* fval)
{
    return(xlmac_txfifo_cell_cnt_get(unit, port, fval));
}

int pm4x10_port_egress_queue_drain_get(int unit, int port,
                   pm_info_t pm_info, uint64 *ctrl, int *rx)
{
    return (xlmac_egress_queue_drain_get(unit, port, ctrl, rx));
}

int pm4x10_port_drain_cells_rx_enable (int unit, int port,
                   pm_info_t pm_info, int rx_en)
{
    return (xlmac_drain_cells_rx_enable(unit, port, rx_en));
}

int pm4x10_port_egress_queue_drain_rx_en(int unit, int port,
                   pm_info_t pm_info, int rx_en)
{
    return (xlmac_egress_queue_drain_rx_en(unit, port, rx_en));
}

int pm4x10_port_mac_reset_set(int unit, int port,
                   pm_info_t pm_info, int val)
{
    int rv;
    SOC_INIT_FUNC_DEFS;

    /* if callback defined, go to local soft reset function */
    rv = (PM_4x10_INFO(pm_info)->portmod_mac_soft_reset) ? pm4x10_port_soft_reset(unit, port, pm_info, val) : xlmac_soft_reset_set(unit, port, val);
    _SOC_IF_ERR_EXIT(rv);

exit:
    SOC_FUNC_RETURN;
}

int pm4x10_port_mac_reset_check(int unit, int port, pm_info_t pm_info,
                                int enable, int* reset)
{
    return (xlmac_reset_check(unit, port, enable, reset));
}

int pm4x10_port_core_num_get(int unit, int port, pm_info_t pm_info,
                                int* core_num)
{
    *core_num = PM_4x10_INFO(pm_info)->core_num;
    return (SOC_E_NONE);
}

int pm4x10_port_e2ecc_hdr_set (int unit, int port, pm_info_t pm_info, 
                               const portmod_port_higig_e2ecc_hdr_t* e2ecc_hdr)
{
    return (xlmac_e2ecc_hdr_set (unit, port, (uint32_t *)e2ecc_hdr->words));
}


int pm4x10_port_e2ecc_hdr_get (int unit, int port, pm_info_t pm_info, 
                               portmod_port_higig_e2ecc_hdr_t* e2ecc_hdr)
{
    return (xlmac_e2ecc_hdr_get (unit, port, e2ecc_hdr->words));
}

int pm4x10_port_e2e_enable_set (int unit, int port, pm_info_t pm_info, int enable) 
{
    return (xlmac_e2e_enable_set(unit, port, enable));
}


int pm4x10_port_e2e_enable_get (int unit, int port, pm_info_t pm_info, int *enable)
{
    return (xlmac_e2e_enable_get(unit, port, enable));
}

int pm4x10_port_fallback_lane_get(int unit, int port, pm_info_t pm_info, int* fallback_lane)
{
    int                         port_index;
    uint32_t                    bitmap;

    SOC_INIT_FUNC_DEFS;
    _SOC_IF_ERR_EXIT(_pm4x10_port_index_get (unit, port, pm_info, &port_index,
                                             &bitmap));
    _SOC_IF_ERR_EXIT(PM4x10_PORT_FALLBACK_LANE_GET(unit, pm_info, fallback_lane,
                                                   port_index));

exit:
    SOC_FUNC_RETURN;

}

/* in_out: 1= in reset, 0= in and out of reset */
STATIC
int pm4x10_port_soft_reset(int unit, int port, pm_info_t pm_info, int in_out)
{
    int rv, rx_enable = 0;
    int phy_port, block, bindex, i;
    portmod_drain_cells_t drain_cells;
    uint32 cell_count;
    uint64 mac_ctrl;
    soc_timeout_t to;
    SOC_INIT_FUNC_DEFS;

    /* Callback soft reset function */
    if (PM_4x10_INFO(pm_info)->portmod_mac_soft_reset) {
        rv = PM_4x10_INFO(pm_info)->portmod_mac_soft_reset(unit, port, portmodCallBackActionTypePre);
        _SOC_IF_ERR_EXIT(rv);
    }

    if(in_out == 0) {
    
        rv = xlmac_egress_queue_drain_get(unit, port, &mac_ctrl, &rx_enable); 
        _SOC_IF_ERR_EXIT(rv);

        /* Drain cells */
        rv = xlmac_drain_cell_get(unit, port, &drain_cells);
        _SOC_IF_ERR_EXIT(rv);

        /* Start TX FIFO draining */
        rv = xlmac_drain_cell_start(unit, port);
        _SOC_IF_ERR_EXIT(rv);

        /* De-assert SOFT_RESET to let the drain start */
        rv = xlmac_soft_reset_set(unit, port, 0);           
        _SOC_IF_ERR_EXIT(rv);

        /* Wait until TX fifo cell count is 0 */
        soc_timeout_init(&to, 250000, 0);
        for (;;) {
            rv = xlmac_txfifo_cell_cnt_get(unit, port, &cell_count);
               _SOC_IF_ERR_EXIT(rv);
               if (cell_count == 0) {
                    break;
            }
            if (soc_timeout_check(&to)) {
                    LOG_ERROR(BSL_LS_BCM_PORT,
                              (BSL_META_UP(unit, port,
                                   "ERROR: u=%d p=%d timeout draining "
                                   "TX FIFO (%d cells remain)\n"),
                               unit, port, cell_count));
                    return SOC_E_INTERNAL;
            }
        }
   
        /* Stop TX FIFO draining */
        rv = xlmac_drain_cell_stop (unit, port, &drain_cells);
        _SOC_IF_ERR_EXIT(rv);

        /* Reset port FIFO */
        phy_port = SOC_INFO(unit).port_l2p_mapping[port];
       for (i = 0; i < SOC_DRIVER(unit)->port_num_blktype; i++) {
            block = SOC_PORT_IDX_BLOCK(unit, phy_port, i);
                if ((SOC_BLOCK_INFO(unit, block).type == SOC_BLK_XLPORT) ||
                    (SOC_BLOCK_INFO(unit, block).type == SOC_BLK_XLPORTB0)) {
                    bindex = SOC_PORT_IDX_BINDEX(unit, phy_port, i);
                    _SOC_IF_ERR_EXIT(portmod_port_soft_reset_toggle(unit, port, bindex));
                    break;
            }
        }
    }

    /* Put port into SOFT_RESET */
    rv = xlmac_soft_reset_set(unit, port, 1);
    _SOC_IF_ERR_EXIT(rv);
    
    if (in_out == 0) {

        /* Callback soft reset function */
       if (PM_4x10_INFO(pm_info)->portmod_mac_soft_reset) {
            rv = PM_4x10_INFO(pm_info)->portmod_mac_soft_reset(unit, port, portmodCallBackActionTypeDuring);
               _SOC_IF_ERR_EXIT(rv);
               soc_reg64_field32_set (unit, XLMAC_CTRLr, &mac_ctrl, SOFT_RESETf, 0); /*make sure restored data wont put mac back in reset*/
        }

        /* Enable TX, set RX, de-assert SOFT_RESET */
       rv = xlmac_egress_queue_drain_rx_en(unit, port, rx_enable);
        _SOC_IF_ERR_EXIT(rv);

        /* Restore XLMAC_CTRL to original value */
        rv = xlmac_mac_ctrl_set(unit, port, mac_ctrl);
        _SOC_IF_ERR_EXIT(rv);
    }

    /* Callback soft reset function */
    if (PM_4x10_INFO(pm_info)->portmod_mac_soft_reset) {
        rv = PM_4x10_INFO(pm_info)->portmod_mac_soft_reset(unit, port, portmodCallBackActionTypePost);
        _SOC_IF_ERR_EXIT(rv);
    }
 
exit:
    SOC_FUNC_RETURN;
}

/*Port discard set*/
int pm4x10_port_discard_set(int unit, int port, pm_info_t pm_info, int discard)
{
    return(xlmac_discard_set(unit, port, discard));
}

/*Port soft reset set set*/
int pm4x10_port_soft_reset_set(int unit, int port, pm_info_t pm_info, int idx,
                               int val, int flags)
{
    SOC_INIT_FUNC_DEFS;

    if (PORTMOD_PORT_REG_ACCESS_DIRECT == flags) {
        _SOC_IF_ERR_EXIT(_pm4x10_port_soft_reset_set_by_phyport(unit, port,
                                                        pm_info, idx, val));
    } else {
        _SOC_IF_ERR_EXIT(_pm4x10_port_soft_reset_set(unit, port, pm_info, idx, val));
    }

exit:
    SOC_FUNC_RETURN;
}

/*Port tx_en=0 and softreset mac*/
int pm4x10_port_tx_down(int unit, int port, pm_info_t pm_info)
{
    SOC_INIT_FUNC_DEFS;

    _SOC_IF_ERR_EXIT(xlmac_tx_enable_set(unit, port, 0));
    _SOC_IF_ERR_EXIT(xlmac_discard_set(unit, port, 0));
    _SOC_IF_ERR_EXIT(xlmac_soft_reset_set(unit, port, 1));

exit:
    SOC_FUNC_RETURN;
}

int pm4x10_port_pgw_reconfig (int unit, int port, pm_info_t pm_info, 
                              const portmod_port_mode_info_t *pmode, int phy_port, int flags)
{
    int rv, idx;
    int phy_acc;
    uint32 reg_val;
    soc_field_t port_fields[] = {PORT0f, PORT1f, PORT2f, PORT3f};

    SOC_INIT_FUNC_DEFS; 

    PM4x10_PHY_ACCESS_GET(unit, port, pm_info, phy_acc);

    if (PORTMOD_PORT_PGW_MAC_RESET == flags) {
        rv = xlmac_discard_set(unit, port, 1);
        _SOC_IF_ERR_EXIT(rv);
        rv = xlmac_rx_enable_set(unit, port, 0);
        _SOC_IF_ERR_EXIT(rv);
        rv = xlmac_tx_enable_set(unit, port, 0);
        _SOC_IF_ERR_EXIT(rv);
        rv = xlmac_soft_reset_set(unit, port, 1);
        _SOC_IF_ERR_EXIT(rv);

        _SOC_IF_ERR_EXIT(READ_XLPORT_MAC_CONTROLr(unit, phy_acc, &reg_val));
        soc_reg_field_set(unit, XLPORT_MAC_CONTROLr, &reg_val, XMAC0_RESETf, 1);
        _SOC_IF_ERR_EXIT(WRITE_XLPORT_MAC_CONTROLr(unit, phy_acc, reg_val));

    } else if (PORTMOD_PORT_PGW_POWER_SAVE == flags) {
         _SOC_IF_ERR_EXIT(READ_XLPORT_POWER_SAVEr(unit, phy_acc, &reg_val));
         soc_reg_field_set(unit, XLPORT_POWER_SAVEr, &reg_val, XPORT_CORE0f, 1);
         _SOC_IF_ERR_EXIT(WRITE_XLPORT_POWER_SAVEr(unit, phy_acc, reg_val));

    } else if (PORTMOD_PORT_PGW_MAC_UNRESET == flags) {
        _SOC_IF_ERR_EXIT(READ_XLPORT_MAC_CONTROLr(unit, phy_acc, &reg_val));
        soc_reg_field_set(unit, XLPORT_MAC_CONTROLr, &reg_val, XMAC0_RESETf, 0);
        _SOC_IF_ERR_EXIT(WRITE_XLPORT_MAC_CONTROLr(unit, phy_acc, reg_val));
    } else if (PORTMOD_PORT_PGW_CONFIGURE == flags) {
        /* 10/20/40G RECONFIGURE SEQUENCE */
        idx = pmode->port_index;

        /* 1.a st SOFT_RESET field in XLMAC_CTRL for new ports that use XLMAC */
        rv = xlmac_soft_reset_set(unit, port, 1);
        _SOC_IF_ERR_EXIT(rv);

        /* 1.a set PORT[subport] field in XLPORT_SOFT_RESET for all new ports */
        _SOC_IF_ERR_EXIT(READ_XLPORT_SOFT_RESETr(unit, phy_acc, &reg_val));
        soc_reg_field_set(unit, XLPORT_SOFT_RESETr, &reg_val, port_fields[idx], 1);
        _SOC_IF_ERR_EXIT(WRITE_XLPORT_SOFT_RESETr(unit, phy_acc, reg_val));

        /* Port enable */
        _SOC_IF_ERR_EXIT(READ_XLPORT_ENABLE_REGr(unit, phy_acc, &reg_val));
        soc_reg_field_set (unit, XLPORT_ENABLE_REGr, &reg_val,
                               port_fields[idx], 1);
        _SOC_IF_ERR_EXIT(WRITE_XLPORT_ENABLE_REGr(unit, phy_acc, reg_val));

        /* set port mode */
        _SOC_IF_ERR_EXIT(_xlport_mode_set(unit, phy_acc, pmode->cur_mode));

        /* Release XLPORT SOFT RESET */
        _SOC_IF_ERR_EXIT(READ_XLPORT_SOFT_RESETr(unit, phy_acc, &reg_val));
        soc_reg_field_set(unit, XLPORT_SOFT_RESETr, &reg_val, port_fields[idx], 0);
        _SOC_IF_ERR_EXIT(WRITE_XLPORT_SOFT_RESETr(unit, phy_acc, reg_val));

        /*Release Mac Soft Reset */
        rv = xlmac_soft_reset_set(unit, port, 0);
        _SOC_IF_ERR_EXIT(rv);
    }
exit:
    SOC_FUNC_RETURN;
}

int pm4x10_port_notify(int unit, int port, pm_info_t pm_info, int link) {
    phymod_phy_access_t phy_access[1+MAX_PHYN];
    int nof_phys;
    portmod_port_interface_config_t interface_config;
    SOC_INIT_FUNC_DEFS;

    _SOC_IF_ERR_EXIT
        (portmod_port_chain_phy_access_get(unit, port, pm_info, 
                                           phy_access ,(1+MAX_PHYN),
                                           &nof_phys));
    /* If nof phys > 1 change the internal phy speed based on the external 
    link speed if the link is up*/
    if (nof_phys > 1) {
        if (link) {
            _SOC_IF_ERR_EXIT(portmod_port_interface_config_get(unit,
                                                               port,
                                                               &interface_config,
                                                               PORTMOD_INIT_F_INTERNAL_SERDES_ONLY));
            /* Set the interface config to internal phy */
            _SOC_IF_ERR_EXIT(portmod_port_interface_config_set(unit,
                                                               port,
                                                               &interface_config, 
                                                               PORTMOD_INIT_F_INTERNAL_SERDES_ONLY));
        } else {
            /*TBD*/
        }
    } 
    exit:
    SOC_FUNC_RETURN;   
}

int pm4x10_port_timesync_config_set(int unit, int port, pm_info_t pm_info, const portmod_phy_timesync_config_t* config)
{
    phymod_phy_access_t phy_access[1+MAX_PHYN];
    int nof_phys;
    SOC_INIT_FUNC_DEFS;

    _SOC_IF_ERR_EXIT(portmod_port_chain_phy_access_get(unit, port, pm_info,
                                                       phy_access ,(1+MAX_PHYN),
                                                       &nof_phys));
    SOC_IF_ERROR_RETURN(portmod_port_phychain_timesync_config_set(unit, port, phy_access,
                                                                  nof_phys,config));
    exit:
    SOC_FUNC_RETURN;   
}

int pm4x10_port_timesync_config_get(int unit, int port, pm_info_t pm_info, portmod_phy_timesync_config_t* config)
{
    phymod_phy_access_t phy_access[1+MAX_PHYN];
    int nof_phys;
    SOC_INIT_FUNC_DEFS;

    _SOC_IF_ERR_EXIT(portmod_port_chain_phy_access_get(unit, port, pm_info,
                                                       phy_access ,(1+MAX_PHYN),
                                                       &nof_phys));
    SOC_IF_ERROR_RETURN(portmod_port_phychain_timesync_config_get(unit, port, phy_access,
                                                                  nof_phys,config));
    exit:
    SOC_FUNC_RETURN;  
}

int pm4x10_port_timesync_enable_set(int unit, int port, pm_info_t pm_info, uint32 enable)
{
    phymod_phy_access_t phy_access[1+MAX_PHYN];
    int nof_phys;
    SOC_INIT_FUNC_DEFS;

    _SOC_IF_ERR_EXIT(portmod_port_chain_phy_access_get(unit, port, pm_info,
                                                       phy_access ,(1+MAX_PHYN),
                                                       &nof_phys));
    SOC_IF_ERROR_RETURN(portmod_port_phychain_timesync_enable_set(unit, phy_access,
                                                                  nof_phys,enable));
    exit:
    SOC_FUNC_RETURN;   
}

int pm4x10_port_timesync_enable_get(int unit, int port, pm_info_t pm_info, uint32* enable)
{
    phymod_phy_access_t phy_access[1+MAX_PHYN];
    int nof_phys;
    SOC_INIT_FUNC_DEFS;

    _SOC_IF_ERR_EXIT(portmod_port_chain_phy_access_get(unit, port, pm_info,
                                                       phy_access ,(1+MAX_PHYN),
                                                       &nof_phys));
    SOC_IF_ERROR_RETURN(portmod_port_phychain_timesync_enable_get(unit, phy_access,
                                                                  nof_phys,enable));

    exit:
    SOC_FUNC_RETURN;
}

int pm4x10_port_timesync_nco_addend_set(int unit, int port, pm_info_t pm_info, uint32 freq_step)
{
    phymod_phy_access_t phy_access[1+MAX_PHYN];
    int nof_phys;
    SOC_INIT_FUNC_DEFS;

    _SOC_IF_ERR_EXIT(portmod_port_chain_phy_access_get(unit, port, pm_info,
                                                       phy_access ,(1+MAX_PHYN),
                                                       &nof_phys));
    SOC_IF_ERROR_RETURN(portmod_port_phychain_timesync_nco_addend_set(unit, phy_access,
                                                                      nof_phys,freq_step));
    exit:
    SOC_FUNC_RETURN;
}

int pm4x10_port_timesync_nco_addend_get(int unit, int port, pm_info_t pm_info, uint32* freq_step)
{
    phymod_phy_access_t phy_access[1+MAX_PHYN];
    int nof_phys;
    SOC_INIT_FUNC_DEFS;

    _SOC_IF_ERR_EXIT(portmod_port_chain_phy_access_get(unit, port, pm_info,
                                                       phy_access ,(1+MAX_PHYN),
                                                       &nof_phys));
    SOC_IF_ERROR_RETURN(portmod_port_phychain_timesync_nco_addend_get(unit, phy_access,
                                                                      nof_phys,freq_step));
    exit:
    SOC_FUNC_RETURN;
}

int pm4x10_port_timesync_framesync_mode_set(int unit, int port, pm_info_t pm_info, 
                                            const portmod_timesync_framesync_t* framesync)
{
    phymod_phy_access_t phy_access[1+MAX_PHYN];
    int nof_phys;
    SOC_INIT_FUNC_DEFS;

    _SOC_IF_ERR_EXIT(portmod_port_chain_phy_access_get(unit, port, pm_info,
                                                       phy_access ,(1+MAX_PHYN),
                                                       &nof_phys));
    SOC_IF_ERROR_RETURN(portmod_port_phychain_timesync_framesync_mode_set(unit, phy_access,
                                                                          nof_phys,framesync));
    exit:
    SOC_FUNC_RETURN;
}

int pm4x10_port_timesync_framesync_mode_get(int unit, int port, pm_info_t pm_info, 
                                            portmod_timesync_framesync_t* framesync)
{
    phymod_phy_access_t phy_access[1+MAX_PHYN];
    int nof_phys;
    SOC_INIT_FUNC_DEFS;

    _SOC_IF_ERR_EXIT(portmod_port_chain_phy_access_get(unit, port, pm_info,
                                                       phy_access ,(1+MAX_PHYN),
                                                       &nof_phys));
    SOC_IF_ERROR_RETURN(portmod_port_phychain_timesync_framesync_mode_get(unit, phy_access,
                                                                      nof_phys,framesync));
    exit:
    SOC_FUNC_RETURN;
}

int pm4x10_port_timesync_local_time_set(int unit, int port, pm_info_t pm_info, const uint64 local_time)
{
    phymod_phy_access_t phy_access[1+MAX_PHYN];
    int nof_phys;
    SOC_INIT_FUNC_DEFS;

    _SOC_IF_ERR_EXIT(portmod_port_chain_phy_access_get(unit, port, pm_info,
                                                       phy_access ,(1+MAX_PHYN),
                                                       &nof_phys));
    SOC_IF_ERROR_RETURN(portmod_port_phychain_timesync_local_time_set(unit, phy_access,
                                                                      nof_phys,local_time));
    exit:
    SOC_FUNC_RETURN;
}


int pm4x10_port_timesync_local_time_get(int unit, int port, pm_info_t pm_info, uint64* local_time)
{
    phymod_phy_access_t phy_access[1+MAX_PHYN];
    int nof_phys;
    SOC_INIT_FUNC_DEFS;

    _SOC_IF_ERR_EXIT(portmod_port_chain_phy_access_get(unit, port, pm_info,
                                                       phy_access ,(1+MAX_PHYN),
                                                       &nof_phys));
    SOC_IF_ERROR_RETURN(portmod_port_phychain_timesync_local_time_get(unit, phy_access, nof_phys,
                                                                      local_time));
    exit:
    SOC_FUNC_RETURN;
}

int pm4x10_port_timesync_load_ctrl_set(int unit, int port, pm_info_t pm_info, 
                                       uint32 load_once, uint32 load_always)
{
    phymod_phy_access_t phy_access[1+MAX_PHYN];
    int nof_phys;
    SOC_INIT_FUNC_DEFS;

    _SOC_IF_ERR_EXIT(portmod_port_chain_phy_access_get(unit, port, pm_info,
                                                       phy_access ,(1+MAX_PHYN),
                                                       &nof_phys));
    SOC_IF_ERROR_RETURN(portmod_port_phychain_timesync_load_ctrl_set(unit, phy_access, nof_phys,
                                                                     load_once, load_always));
    exit:
    SOC_FUNC_RETURN;
}

int pm4x10_port_timesync_load_ctrl_get(int unit, int port, pm_info_t pm_info, 
                                       uint32* load_once, uint32* load_always)
{
    phymod_phy_access_t phy_access[1+MAX_PHYN];
    int nof_phys;
    SOC_INIT_FUNC_DEFS;

    _SOC_IF_ERR_EXIT(portmod_port_chain_phy_access_get(unit, port, pm_info,
                                                       phy_access ,(1+MAX_PHYN),
                                                       &nof_phys));
    SOC_IF_ERROR_RETURN(portmod_port_phychain_timesync_load_ctrl_get(unit, phy_access, nof_phys,
                                                                     load_once, load_always));
    exit:
    SOC_FUNC_RETURN;
}

int pm4x10_port_timesync_tx_timestamp_offset_set(int unit, int port, pm_info_t pm_info, uint32 ts_offset)
{
    phymod_phy_access_t phy_access[1+MAX_PHYN];
    int nof_phys;
    SOC_INIT_FUNC_DEFS;

    _SOC_IF_ERR_EXIT(portmod_port_chain_phy_access_get(unit, port, pm_info,
                                                       phy_access ,(1+MAX_PHYN),
                                                       &nof_phys));
    SOC_IF_ERROR_RETURN(portmod_port_phychain_timesync_tx_timestamp_offset_set(unit, phy_access, nof_phys,
                                                                               ts_offset));
    exit:
    SOC_FUNC_RETURN;
}

int pm4x10_port_timesync_tx_timestamp_offset_get(int unit, int port, pm_info_t pm_info, uint32* ts_offset)
{
    phymod_phy_access_t phy_access[1+MAX_PHYN];
    int nof_phys;
    SOC_INIT_FUNC_DEFS;

    _SOC_IF_ERR_EXIT(portmod_port_chain_phy_access_get(unit, port, pm_info,
                                                       phy_access ,(1+MAX_PHYN),
                                                       &nof_phys));
    SOC_IF_ERROR_RETURN(portmod_port_phychain_timesync_tx_timestamp_offset_get(unit, phy_access, nof_phys,
                                                                               ts_offset));
    exit:
    SOC_FUNC_RETURN;
}

int pm4x10_port_timesync_rx_timestamp_offset_set(int unit, int port, pm_info_t pm_info, uint32 ts_offset)
{
    phymod_phy_access_t phy_access[1+MAX_PHYN];
    int nof_phys;
    SOC_INIT_FUNC_DEFS;

    _SOC_IF_ERR_EXIT(portmod_port_chain_phy_access_get(unit, port, pm_info,
                                                       phy_access ,(1+MAX_PHYN),
                                                       &nof_phys));
    SOC_IF_ERROR_RETURN(portmod_port_phychain_timesync_rx_timestamp_offset_set(unit, phy_access, nof_phys,
                                                                               ts_offset));
    exit:
    SOC_FUNC_RETURN;
}

int pm4x10_port_timesync_rx_timestamp_offset_get(int unit, int port, pm_info_t pm_info, uint32* ts_offset)
{
    phymod_phy_access_t phy_access[1+MAX_PHYN];
    int nof_phys;
    SOC_INIT_FUNC_DEFS;

    _SOC_IF_ERR_EXIT(portmod_port_chain_phy_access_get(unit, port, pm_info,
                                                       phy_access ,(1+MAX_PHYN),
                                                       &nof_phys));
    SOC_IF_ERROR_RETURN(portmod_port_phychain_timesync_rx_timestamp_offset_get(unit, phy_access, nof_phys,
                                                                               ts_offset));
    exit:
    SOC_FUNC_RETURN;
}

int pm4x10_port_phy_intr_enable_set(int unit, int port, pm_info_t pm_info, uint32 intr_enable)
{
    phymod_phy_access_t phy_access[1+MAX_PHYN];
    int nof_phys;
    SOC_INIT_FUNC_DEFS;

    _SOC_IF_ERR_EXIT(portmod_port_chain_phy_access_get(unit, port, pm_info,
                                                       phy_access ,(1+MAX_PHYN),
                                                       &nof_phys));
    SOC_IF_ERROR_RETURN(portmod_port_phychain_phy_intr_enable_set(unit, phy_access, nof_phys,
                                                                     intr_enable));
    exit:
    SOC_FUNC_RETURN;
}

int pm4x10_port_phy_intr_enable_get(int unit, int port, pm_info_t pm_info, uint32* intr_enable)
{
    phymod_phy_access_t phy_access[1+MAX_PHYN];
    int nof_phys;
    SOC_INIT_FUNC_DEFS;

    _SOC_IF_ERR_EXIT(portmod_port_chain_phy_access_get(unit, port, pm_info,
                                                       phy_access ,(1+MAX_PHYN),
                                                       &nof_phys));
    SOC_IF_ERROR_RETURN(portmod_port_phychain_phy_intr_enable_get(unit, phy_access, nof_phys,
                                                                  intr_enable));
    exit:
    SOC_FUNC_RETURN;
}

int pm4x10_port_phy_intr_status_get(int unit, int port, pm_info_t pm_info, uint32* intr_enable)
{
    phymod_phy_access_t phy_access[1+MAX_PHYN];
    int nof_phys;
    SOC_INIT_FUNC_DEFS;

    _SOC_IF_ERR_EXIT(portmod_port_chain_phy_access_get(unit, port, pm_info,
                                                       phy_access ,(1+MAX_PHYN),
                                                       &nof_phys));
    SOC_IF_ERROR_RETURN(portmod_port_phychain_phy_intr_status_get(unit, phy_access, nof_phys,
                                                                  intr_enable));
    exit:
    SOC_FUNC_RETURN;
}

int pm4x10_port_phy_intr_status_clear(int unit, int port, pm_info_t pm_info)
{
    phymod_phy_access_t phy_access[1+MAX_PHYN];
    int nof_phys;
    SOC_INIT_FUNC_DEFS;

    _SOC_IF_ERR_EXIT(portmod_port_chain_phy_access_get(unit, port, pm_info,
                                                       phy_access ,(1+MAX_PHYN),
                                                       &nof_phys));
    SOC_IF_ERROR_RETURN(portmod_port_phychain_phy_intr_status_clear(unit, phy_access, nof_phys));

    exit:
    SOC_FUNC_RETURN;
}

int pm4x10_port_phy_timesync_do_sync(int unit, int port, pm_info_t pm_info )
{
    phymod_phy_access_t phy_access[1+MAX_PHYN];
    int nof_phys;
    SOC_INIT_FUNC_DEFS;

    _SOC_IF_ERR_EXIT(portmod_port_chain_phy_access_get(unit, port, pm_info,
                                                       phy_access ,(1+MAX_PHYN),
                                                       &nof_phys));
    SOC_IF_ERROR_RETURN(portmod_port_phychain_phy_timesync_do_sync(unit, phy_access, nof_phys ));
    exit:
    SOC_FUNC_RETURN;
}

int pm4x10_port_timesync_capture_timestamp_get(int unit, int port, pm_info_t pm_info, uint64* cap_ts)
{
    phymod_phy_access_t phy_access[1+MAX_PHYN];
    int nof_phys;
    SOC_INIT_FUNC_DEFS;

    _SOC_IF_ERR_EXIT(portmod_port_chain_phy_access_get(unit, port, pm_info,
                                                       phy_access ,(1+MAX_PHYN),
                                                       &nof_phys));
    SOC_IF_ERROR_RETURN(portmod_port_phychain_timesync_capture_timestamp_get(unit, phy_access, nof_phys,
                                                                             cap_ts));
    exit:
    SOC_FUNC_RETURN;
}


int pm4x10_port_timesync_heartbeat_timestamp_get(int unit, int port, pm_info_t pm_info, uint64* hb_ts)
{
    phymod_phy_access_t phy_access[1+MAX_PHYN];
    int nof_phys;
    SOC_INIT_FUNC_DEFS;

    _SOC_IF_ERR_EXIT(portmod_port_chain_phy_access_get(unit, port, pm_info,
                                                       phy_access ,(1+MAX_PHYN),
                                                       &nof_phys));
    SOC_IF_ERROR_RETURN(portmod_port_phychain_timesync_heartbeat_timestamp_get(unit, phy_access, nof_phys,
                                                                              hb_ts));
    exit:
    SOC_FUNC_RETURN;
}

int pm4x10_port_edc_config_set(int unit, int port, pm_info_t pm_info, 
                               const portmod_edc_config_t* config)
{
    phymod_phy_access_t phy_access[1+MAX_PHYN];
    int nof_phys;
    SOC_INIT_FUNC_DEFS;

    _SOC_IF_ERR_EXIT(portmod_port_chain_phy_access_get(unit, port, pm_info,
                                                       phy_access ,(1+MAX_PHYN),
                                                       &nof_phys));
    SOC_IF_ERROR_RETURN(portmod_port_phychain_edc_config_set(unit, phy_access, nof_phys, config));
    exit:
    SOC_FUNC_RETURN;
}

int pm4x10_port_edc_config_get(int unit, int port, pm_info_t pm_info, 
                               portmod_edc_config_t* config)
{
    phymod_phy_access_t phy_access[1+MAX_PHYN];
    int nof_phys;
    SOC_INIT_FUNC_DEFS;

    _SOC_IF_ERR_EXIT(portmod_port_chain_phy_access_get(unit, port, pm_info,
                                                       phy_access ,(1+MAX_PHYN),
                                                       &nof_phys));
    SOC_IF_ERROR_RETURN(portmod_port_phychain_edc_config_get(unit, phy_access, nof_phys, config));
    exit:
    SOC_FUNC_RETURN;
}

/* get interrupt value. */
int pm4x10_port_interrupt_enable_get (int unit, int port, pm_info_t pm_info,
                                    int intr_type, uint32 *val)
{
    uint32 reg_val;
    int phy_acc;
    pm_version_t version;
    SOC_INIT_FUNC_DEFS;

    PM4x10_PHY_ACCESS_GET(unit, port, pm_info, phy_acc);
    _SOC_IF_ERR_EXIT(READ_XLPORT_INTR_ENABLEr(unit, phy_acc, &reg_val));

    _SOC_IF_ERR_EXIT(_pm4x10_pm_version_get(unit, port, pm_info, &version));

    switch(intr_type) {
        case portmodIntrTypeMibTxMemErr:
             *val = soc_reg_field_get(unit, XLPORT_INTR_ENABLEr, reg_val, MIB_TX_MEM_ERRf);
             break;
        case portmodIntrTypeMibRxMemErr:
             *val = soc_reg_field_get(unit, XLPORT_INTR_ENABLEr, reg_val, MIB_RX_MEM_ERRf);
             break;
        case portmodIntrTypeMacTxCdcMemErr:
             if(PORTMOD_PM_VERSION_NUMBER_IS_UNKNOWN(version.version_number)) {
                 *val = soc_reg_field_get(unit, XLPORT_INTR_ENABLEr, reg_val, MAC_TX_CDC_MEM_ERRf);
                 break;
             }
             else {
                 return SOC_E_UNAVAIL;
             }
        case portmodIntrTypeMacRxCdcMemErr:
             if(PORTMOD_PM_VERSION_NUMBER_IS_UNKNOWN(version.version_number)) {
                 *val = soc_reg_field_get(unit, XLPORT_INTR_ENABLEr, reg_val, MAC_RX_CDC_MEM_ERRf);
                 break;
             }
             else {
                 return SOC_E_UNAVAIL;
             }
        case portmodIntrTypeTscErr:
             *val = soc_reg_field_get(unit, XLPORT_INTR_ENABLEr, reg_val, TSC_ERRf);
             break;
        case portmodIntrTypeRxFcReqFull:
             *val = soc_reg_field_get(unit, XLPORT_INTR_ENABLEr, reg_val, RX_FLOWCONTROL_REQ_FULLf);
             break;
        case portmodIntrTypeLinkdown:
             if(version.version_number >= PM_VERSION_NUM_0) {
                 *val = soc_reg_field_get(unit, XLPORT_INTR_ENABLEr, reg_val, LINK_DOWNf);
                 break;
             }
             else {
                 return SOC_E_UNAVAIL;
             }
        case portmodIntrTypeMacErr:
             if(version.version_number >= PM_VERSION_NUM_0) {
                 *val = soc_reg_field_get(unit, XLPORT_INTR_ENABLEr, reg_val, MAC_ERRf);
                 break;
             }
             else {
                 return SOC_E_UNAVAIL;
             }
        case portmodIntrTypeTxPktUnderflow :
        case portmodIntrTypeTxPktOverflow :
        case portmodIntrTypeTxLlfcMsgOverflow :
        case portmodIntrTypeTxTSFifoOverflow :
        case portmodIntrTypeRxPktOverflow :
        case portmodIntrTypeRxMsgOverflow :
        case portmodIntrTypeTxCdcSingleBitErr :
        case portmodIntrTypeTxCdcDoubleBitErr :
        case portmodIntrTypeRxCdcSingleBitErr :
        case portmodIntrTypeRxCdcDoubleBitErr :
        case portmodIntrTypeLocalFaultStatus :
        case portmodIntrTypeRemoteFaultStatus :
        case portmodIntrTypeLinkInterruptionStatus :
        case portmodIntrTypeTsEntryValid :
             if(version.version_number >= PM_VERSION_NUM_0) {
                 _SOC_IF_ERR_EXIT(xlmac_interrupt_enable_get(unit, port, intr_type, val));
                 break;
             }
             else {
                 return SOC_E_UNAVAIL;
             }
        default:
            _SOC_EXIT_WITH_ERR(SOC_E_PARAM, (_SOC_MSG("Invalid interrupt type")));
            break;
    }

exit:
    SOC_FUNC_RETURN;
}

/* set interrupt value. */
int pm4x10_port_interrupt_enable_set (int unit, int port, pm_info_t pm_info,
                                    int intr_type, uint32 val)
{
    uint32 reg_val;
    int phy_acc;
    pm_version_t version;
    SOC_INIT_FUNC_DEFS;

    PM4x10_PHY_ACCESS_GET(unit, port, pm_info, phy_acc);
    _SOC_IF_ERR_EXIT(READ_XLPORT_INTR_ENABLEr(unit, phy_acc, &reg_val));

    _SOC_IF_ERR_EXIT(_pm4x10_pm_version_get(unit, port, pm_info, &version));

    switch(intr_type) {
        case portmodIntrTypeMibTxMemErr:
             soc_reg_field_set(unit, XLPORT_INTR_ENABLEr, &reg_val, MIB_TX_MEM_ERRf, val);
             break;
        case portmodIntrTypeMibRxMemErr:
             soc_reg_field_set(unit, XLPORT_INTR_ENABLEr, &reg_val, MIB_RX_MEM_ERRf, val);
             break;
        case portmodIntrTypeMacTxCdcMemErr:
             if(PORTMOD_PM_VERSION_NUMBER_IS_UNKNOWN(version.version_number)) {
                soc_reg_field_set(unit, XLPORT_INTR_ENABLEr, &reg_val, MAC_TX_CDC_MEM_ERRf, val);
                break;
             }
             else {
                return SOC_E_UNAVAIL;
             }
        case portmodIntrTypeMacRxCdcMemErr:
             if(PORTMOD_PM_VERSION_NUMBER_IS_UNKNOWN(version.version_number)) {
                soc_reg_field_set(unit, XLPORT_INTR_ENABLEr, &reg_val, MAC_RX_CDC_MEM_ERRf, val);
                break;
             }
             else {
                return SOC_E_UNAVAIL;
             }
        case portmodIntrTypeTscErr:
             soc_reg_field_set(unit, XLPORT_INTR_ENABLEr, &reg_val, TSC_ERRf, val);
             break;
        case portmodIntrTypeRxFcReqFull:
             soc_reg_field_set(unit, XLPORT_INTR_ENABLEr, &reg_val, RX_FLOWCONTROL_REQ_FULLf, val);
             break;
        case portmodIntrTypeLinkdown:
             if(version.version_number >= PM_VERSION_NUM_0) {
                 soc_reg_field_set(unit, XLPORT_INTR_ENABLEr, &reg_val, LINK_DOWNf, val);
                 break;
             }
             else {
                 return SOC_E_UNAVAIL;
             }
        case portmodIntrTypeMacErr:
             if(version.version_number >= PM_VERSION_NUM_0) {
                soc_reg_field_set(unit, XLPORT_INTR_ENABLEr, &reg_val, MAC_ERRf, val);
                break;
             }
             else {
                return SOC_E_UNAVAIL;
             }
        case portmodIntrTypeTxPktUnderflow :
        case portmodIntrTypeTxPktOverflow :
        case portmodIntrTypeTxLlfcMsgOverflow :
        case portmodIntrTypeTxTSFifoOverflow :
        case portmodIntrTypeRxPktOverflow :
        case portmodIntrTypeRxMsgOverflow :
        case portmodIntrTypeTxCdcSingleBitErr :
        case portmodIntrTypeTxCdcDoubleBitErr :
        case portmodIntrTypeRxCdcSingleBitErr :
        case portmodIntrTypeRxCdcDoubleBitErr :
        case portmodIntrTypeLocalFaultStatus :
        case portmodIntrTypeRemoteFaultStatus :
        case portmodIntrTypeLinkInterruptionStatus :
        case portmodIntrTypeTsEntryValid :
             if(version.version_number >= PM_VERSION_NUM_0) {
                _SOC_IF_ERR_EXIT(xlmac_interrupt_enable_set(unit, port, intr_type, val));
                break;
             }
             else {
                return SOC_E_UNAVAIL;
             }
        default:
            _SOC_EXIT_WITH_ERR(SOC_E_PARAM, (_SOC_MSG("Invalid interrupt type")));
            break;
    }
    _SOC_IF_ERR_EXIT(WRITE_XLPORT_INTR_ENABLEr(unit, phy_acc, reg_val));

exit:
    SOC_FUNC_RETURN;
}

/* get interrupt value. */
int pm4x10_port_interrupt_get (int unit, int port, pm_info_t pm_info,
                               int intr_type, uint32* val)
{
    uint32 reg_val;
    int phy_acc;
    pm_version_t version;
    SOC_INIT_FUNC_DEFS;

    PM4x10_PHY_ACCESS_GET(unit, port, pm_info, phy_acc);
    _SOC_IF_ERR_EXIT(READ_XLPORT_INTR_STATUSr(unit, phy_acc, &reg_val));

    _SOC_IF_ERR_EXIT(_pm4x10_pm_version_get(unit, port, pm_info, &version));

    switch(intr_type) {
        case portmodIntrTypeMibTxMemErr:
             *val = soc_reg_field_get(unit, XLPORT_INTR_STATUSr, reg_val, MIB_TX_MEM_ERRf);
             break;
        case portmodIntrTypeMibRxMemErr:
             *val = soc_reg_field_get(unit, XLPORT_INTR_STATUSr, reg_val, MIB_RX_MEM_ERRf);
             break;
        case portmodIntrTypeMacTxCdcMemErr:
             if(PORTMOD_PM_VERSION_NUMBER_IS_UNKNOWN(version.version_number)) {
                *val = soc_reg_field_get(unit, XLPORT_INTR_STATUSr, reg_val, MAC_TX_CDC_MEM_ERRf);
                break;
             }
             else {
                return SOC_E_UNAVAIL;
             }
        case portmodIntrTypeMacRxCdcMemErr:
             if(PORTMOD_PM_VERSION_NUMBER_IS_UNKNOWN(version.version_number)) {
                *val = soc_reg_field_get(unit, XLPORT_INTR_STATUSr, reg_val, MAC_RX_CDC_MEM_ERRf);
                break;
             }
             else {
                return SOC_E_UNAVAIL;
              }
        case portmodIntrTypeTscErr:
             *val = soc_reg_field_get(unit, XLPORT_INTR_STATUSr, reg_val, TSC_ERRf);
             break;
        case portmodIntrTypeRxFcReqFull:
             *val = soc_reg_field_get(unit, XLPORT_INTR_STATUSr, reg_val, RX_FLOWCONTROL_REQ_FULLf);
             break;
        case portmodIntrTypeLinkdown:
             if(version.version_number >= PM_VERSION_NUM_0) {
                *val = soc_reg_field_get(unit, XLPORT_INTR_STATUSr, reg_val, LINK_DOWNf);
                break;
             }
             else {
                return SOC_E_UNAVAIL;
             }
        case portmodIntrTypeMacErr:
             if(version.version_number >= PM_VERSION_NUM_0) {
                   *val = soc_reg_field_get(unit, XLPORT_INTR_STATUSr, reg_val, MAC_ERRf);
                    break;
             }
             else {
                  return SOC_E_UNAVAIL;
             }
        case portmodIntrTypeTxPktUnderflow :
        case portmodIntrTypeTxPktOverflow :
        case portmodIntrTypeTxLlfcMsgOverflow :
        case portmodIntrTypeTxTSFifoOverflow :
        case portmodIntrTypeRxPktOverflow :
        case portmodIntrTypeRxMsgOverflow :
        case portmodIntrTypeTxCdcSingleBitErr :
        case portmodIntrTypeTxCdcDoubleBitErr :
        case portmodIntrTypeRxCdcSingleBitErr :
        case portmodIntrTypeRxCdcDoubleBitErr :
        case portmodIntrTypeLocalFaultStatus :
        case portmodIntrTypeRemoteFaultStatus :
        case portmodIntrTypeLinkInterruptionStatus :
        case portmodIntrTypeTsEntryValid :
             if(version.version_number >= PM_VERSION_NUM_0) {
                _SOC_IF_ERR_EXIT(xlmac_interrupt_status_get(unit, port, intr_type, val));
                break;
             }
             else {
                return SOC_E_UNAVAIL;
             }
        default:
            _SOC_EXIT_WITH_ERR(SOC_E_PARAM, (_SOC_MSG("Invalid interrupt type")));
            break;
    }
exit:
    SOC_FUNC_RETURN;
}

int pm4x10_port_interrupts_get (int unit, int port, pm_info_t pm_info,
                                int arr_max_size, uint32* intr_arr, uint32* size)
{
    uint32 reg_val, cnt=0;
    int phy_acc;
    uint32 mac_intr_cnt = 0;
    pm_version_t version;
    SOC_INIT_FUNC_DEFS;

    PM4x10_PHY_ACCESS_GET(unit, port, pm_info, phy_acc);
    _SOC_IF_ERR_EXIT(READ_XLPORT_INTR_STATUSr(unit, phy_acc, &reg_val));

    _SOC_IF_ERR_EXIT(_pm4x10_pm_version_get(unit, port, pm_info, &version));

    if (soc_reg_field_get(unit, XLPORT_INTR_STATUSr, reg_val, MIB_TX_MEM_ERRf)) {
        if (cnt >= arr_max_size) {
            _SOC_EXIT_WITH_ERR(SOC_E_PARAM, (_SOC_MSG("Insufficient Array size")));
        }
        intr_arr[cnt++] = portmodIntrTypeMibTxMemErr;
    }

    if (soc_reg_field_get(unit, XLPORT_INTR_STATUSr, reg_val, MIB_RX_MEM_ERRf)) {
        if (cnt >= arr_max_size) {
            _SOC_EXIT_WITH_ERR(SOC_E_PARAM, (_SOC_MSG("Insufficient Array size")));
        }
        intr_arr[cnt++] = portmodIntrTypeMibRxMemErr;
    }

    if(PORTMOD_PM_VERSION_NUMBER_IS_UNKNOWN(version.version_number)) {
        if (soc_reg_field_get(unit, XLPORT_INTR_STATUSr, reg_val, MAC_TX_CDC_MEM_ERRf)) {
            if (cnt >= arr_max_size) {
                _SOC_EXIT_WITH_ERR(SOC_E_PARAM, (_SOC_MSG("Insufficient Array size")));
            }
            intr_arr[cnt++] = portmodIntrTypeMacTxCdcMemErr;
        }
        if (soc_reg_field_get(unit, XLPORT_INTR_STATUSr, reg_val, MAC_RX_CDC_MEM_ERRf)) {
             if (cnt >= arr_max_size) {
                 _SOC_EXIT_WITH_ERR(SOC_E_PARAM, (_SOC_MSG("Insufficient Array size")));
             }
             intr_arr[cnt++] = portmodIntrTypeMacRxCdcMemErr;
        }
    }

    if (soc_reg_field_get(unit, XLPORT_INTR_STATUSr, reg_val, TSC_ERRf)) {
        if (cnt >= arr_max_size) {
            _SOC_EXIT_WITH_ERR(SOC_E_PARAM, (_SOC_MSG("Insufficient Array size")));
        }
        intr_arr[cnt++] = portmodIntrTypeTscErr;
    }
    if (soc_reg_field_get(unit, XLPORT_INTR_STATUSr, reg_val, RX_FLOWCONTROL_REQ_FULLf)) {
        if (cnt >= arr_max_size) {
            _SOC_EXIT_WITH_ERR(SOC_E_PARAM, (_SOC_MSG("Insufficient Array size")));
        }
        intr_arr[cnt++] = portmodIntrTypeRxFcReqFull;
    }

    if(version.version_number >= PM_VERSION_NUM_0) {
        if(soc_reg_field_get(unit, XLPORT_INTR_STATUSr, reg_val, LINK_DOWNf)) {
             if (cnt >= arr_max_size) {
                  _SOC_EXIT_WITH_ERR(SOC_E_PARAM, (_SOC_MSG("Insufficient Array size")));
             }
             intr_arr[cnt++] = portmodIntrTypeLinkdown;
        }

        if(soc_reg_field_get(unit, XLPORT_INTR_STATUSr, reg_val, MAC_ERRf)) {
             if (cnt >= arr_max_size) {
                  _SOC_EXIT_WITH_ERR(SOC_E_PARAM, (_SOC_MSG("Insufficient Array size")));
             }
             intr_arr[cnt++] = portmodIntrTypeMacErr;
        }

        _SOC_IF_ERR_EXIT(xlmac_interrupts_status_get(unit, port, (arr_max_size- cnt), (intr_arr + cnt), &mac_intr_cnt));
        cnt += mac_intr_cnt;
     }

    *size = cnt;

exit:
    SOC_FUNC_RETURN;
}

/*!
 * pm4x10_port_eee_clock_set
 *
 * @brief set EEE Config.
 *
 * @param [in]  unit            - unit id
 * @param [in]  port            - logical port
 * @param [in]  eee             -
 */
int pm4x10_port_eee_clock_set(int unit, int port, pm_info_t pm_info,
                              const portmod_eee_clock_t* eee_clk)
{
    int phy_acc;
    SOC_INIT_FUNC_DEFS;

    PM4x10_PHY_ACCESS_GET(unit, port, pm_info, phy_acc);

    _SOC_IF_ERR_EXIT(WRITE_XLPORT_EEE_CLOCK_GATEr(unit, phy_acc, eee_clk->clock_gate));
    _SOC_IF_ERR_EXIT(WRITE_XLPORT_EEE_CORE0_CLOCK_GATE_COUNTERr(unit, phy_acc, eee_clk->clock_count));
    _SOC_IF_ERR_EXIT(WRITE_XLPORT_EEE_DURATION_TIMER_PULSEr(unit, phy_acc, eee_clk->timer_pulse));

exit:
    SOC_FUNC_RETURN;
}


/*!
 * portmod_port_eee_clock_get
 *
 * @brief set EEE Config.
 *
 * @param [in]  unit            - unit id
 * @param [in]  port            - logical port
 * @param [out]  eee             -
 */
int pm4x10_port_eee_clock_get (int unit, int port, pm_info_t pm_info,
                               portmod_eee_clock_t* eee_clk)
{
    int phy_acc;
    SOC_INIT_FUNC_DEFS;

    PM4x10_PHY_ACCESS_GET(unit, port, pm_info, phy_acc);

    _SOC_IF_ERR_EXIT(READ_XLPORT_EEE_CLOCK_GATEr(unit, phy_acc, &eee_clk->clock_gate));
    _SOC_IF_ERR_EXIT(READ_XLPORT_EEE_CORE0_CLOCK_GATE_COUNTERr(unit, phy_acc, &eee_clk->clock_count));
    _SOC_IF_ERR_EXIT(READ_XLPORT_EEE_DURATION_TIMER_PULSEr(unit, phy_acc, &eee_clk->timer_pulse));

exit:
    SOC_FUNC_RETURN;
}

int pm4x10_port_failover_mode_set(int unit, int port, pm_info_t pm_info, phymod_failover_mode_t failover)
{
    phymod_phy_access_t phy_access[1+MAX_PHYN];
    int nof_phys;
    int port_index;
    uint32 bitmap;

    SOC_INIT_FUNC_DEFS;

    _SOC_IF_ERR_EXIT(_pm4x10_port_index_get (unit, port, pm_info, &port_index,
                                             &bitmap));

    if (PM_4x10_INFO(pm_info)->nof_phys[port_index] == 0) {
        _SOC_EXIT_WITH_ERR(SOC_E_UNAVAIL,
                           (_SOC_MSG("phy failover is not supported")));
    }

    _SOC_IF_ERR_EXIT(portmod_port_chain_phy_access_get(unit, port, pm_info,
                                                       phy_access ,(1+MAX_PHYN),
                                                       &nof_phys));

    SOC_IF_ERROR_RETURN(portmod_port_phychain_failover_mode_set(unit, phy_access, nof_phys, failover));

exit:
    SOC_FUNC_RETURN;
}

int pm4x10_port_failover_mode_get(int unit, int port, pm_info_t pm_info, phymod_failover_mode_t* failover)
{
    phymod_phy_access_t phy_access[1+MAX_PHYN];
    int nof_phys;
    int port_index;
    uint32 bitmap;

    SOC_INIT_FUNC_DEFS;

    _SOC_IF_ERR_EXIT(_pm4x10_port_index_get (unit, port, pm_info, &port_index,
                                             &bitmap));

    if (PM_4x10_INFO(pm_info)->nof_phys[port_index] == 0) {
        _SOC_EXIT_WITH_ERR(SOC_E_UNAVAIL,
                           (_SOC_MSG("phy failover is not supported")));
    }

    _SOC_IF_ERR_EXIT(portmod_port_chain_phy_access_get(unit, port, pm_info,
                                                       phy_access ,(1+MAX_PHYN),
                                                       &nof_phys));

    SOC_IF_ERROR_RETURN(portmod_port_phychain_failover_mode_get(unit, phy_access, nof_phys, failover));

exit:
    SOC_FUNC_RETURN;
}

int pm4x10_port_flow_control_set (int unit, int port, pm_info_t pm_info, 
                                  int merge_mode_en, int parallel_fc_en)
{
    int phy_acc, rv;
    uint32 rval;

    PM4x10_PHY_ACCESS_GET(unit, port, pm_info, phy_acc);

    rv = READ_XLPORT_FLOW_CONTROL_CONFIGr(unit, phy_acc, &rval);
    if (SOC_FAILURE(rv)) return (rv);

    soc_reg_field_set(unit, XLPORT_FLOW_CONTROL_CONFIGr, &rval, MERGE_MODE_ENf, merge_mode_en);
    soc_reg_field_set(unit, XLPORT_FLOW_CONTROL_CONFIGr, &rval, PARALLEL_FC_ENf, parallel_fc_en);

    return(WRITE_XLPORT_FLOW_CONTROL_CONFIGr(unit, phy_acc, rval));
}

int pm4x10_port_lane_map_set(int unit, int port, pm_info_t pm_info, const phymod_lane_map_t* lane_map)
{
    SOC_INIT_FUNC_DEFS;

    PM_4x10_INFO(pm_info)->lane_map = *lane_map;


    SOC_FUNC_RETURN;
}

int pm4x10_port_lane_map_get(int unit, int port, pm_info_t pm_info, phymod_lane_map_t* lane_map)
{
    SOC_INIT_FUNC_DEFS;

    *lane_map = PM_4x10_INFO(pm_info)->lane_map;


    SOC_FUNC_RETURN;
}

int pm4x10_xphy_lane_attach_to_pm (int unit, pm_info_t pm_info, int iphy, int phyn, const portmod_xphy_lane_connection_t* lane_conn)
{   
    portmod_xphy_lane_connection_t *candidate_lane_conn = NULL;
    int lane_index = 0, phy_index = 0;
    int pm_is_active = 0, rv = 0;
    int nof_phys = 0;
    uint32 xphy_id;
    int lane_mask;
    uint32 active_lanes;
    SOC_INIT_FUNC_DEFS;

    /* Validate parameters */
    _SOC_IF_ERR_EXIT(portmod_xphy_lane_connection_t_validate(unit,lane_conn));

    if (MAX_PHYN <= phyn){
        _SOC_EXIT_WITH_ERR(SOC_E_PARAM, (_SOC_MSG("Invalid Ext PHY location.")));
    }
    /* Making sure the port macro is not active */
    rv = SOC_WB_ENGINE_GET_VAR(unit, SOC_WB_ENGINE_PORTMOD, pm_info->wb_vars_ids[isActive], &pm_is_active);
    _SOC_IF_ERR_EXIT(rv);

    /* Attaching the external phy to the phy chain of the port macro */

    /* find lane index corresponding to internal physical port */
    lane_index = 0;
    PORTMOD_PBMP_ITER(PM_4x10_INFO(pm_info)->phys, phy_index) {
        if (phy_index == iphy) {
            break;
        }
        lane_index++;
    }

    if (!SOC_WARM_BOOT(unit)) {
        rv = PM4x10_PORT_LANE_CONNECTION_DB_SET(unit, pm_info, lane_conn, phyn-1, lane_index); 
        _SOC_IF_ERR_EXIT(rv);
        candidate_lane_conn = &(PM_4x10_INFO(pm_info)->lane_conn[phyn-1][lane_index]); 
        sal_memcpy(candidate_lane_conn, lane_conn, sizeof(portmod_xphy_lane_connection_t));

        xphy_id = candidate_lane_conn->xphy_id;
        lane_mask = candidate_lane_conn->ss_lane_mask;

        /*
         * Keeping track of the xphy lane usage.  If the xphy lane is connected
         * it will set the corresponding bit to mark as lane is active.
         */

        if ((lane_mask != -1) && (xphy_id != PORTMOD_XPHY_ID_INVALID)) {
            _SOC_IF_ERR_EXIT(portmod_xphy_active_lane_get(unit, xphy_id, &active_lanes));
            active_lanes |= lane_mask;
            _SOC_IF_ERR_EXIT(portmod_xphy_active_lane_set(unit,xphy_id, active_lanes));
        }

        PM4x10_PORT_NUM_PHYS_GET(unit, pm_info, &nof_phys, lane_index); 
        if (nof_phys <= phyn) {
            nof_phys = phyn + 1;
            rv = PM4x10_PORT_NUM_PHYS_SET(unit, pm_info, &nof_phys, lane_index);
            _SOC_IF_ERR_EXIT(rv);
            PM_4x10_INFO(pm_info)->nof_phys[lane_index] = phyn+1;
        }
    } 
  
exit:
   SOC_FUNC_RETURN;
}

int pm4x10_xphy_lane_detach_from_pm (int unit, pm_info_t pm_info, int iphy, int phyn, portmod_xphy_lane_connection_t* lane_conn)
{
    portmod_xphy_lane_connection_t *candidate_lane_conn = NULL;
    int lane_index = 0, phy_index = 0;
    int pm_is_active = 0, rv = 0;
    int nof_phys = 0;
    uint32 active_lanes;
    uint32 xphy_id;
    int lane_mask;
    SOC_INIT_FUNC_DEFS;

    if (MAX_PHYN <= phyn){
        _SOC_EXIT_WITH_ERR(SOC_E_PARAM, (_SOC_MSG("Invalid Ext PHY location.")));
    }
    /* Making sure the port macro is not active */
    rv = SOC_WB_ENGINE_GET_VAR(unit, SOC_WB_ENGINE_PORTMOD, pm_info->wb_vars_ids[isActive], &pm_is_active);
    _SOC_IF_ERR_EXIT(rv);

    /* Attaching the external phy to the phy chain of the port macro */

    /* find lane index corresponding to internal physical port */
    lane_index = 0;
    PORTMOD_PBMP_ITER(PM_4x10_INFO(pm_info)->phys, phy_index) {
        if (phy_index == iphy) {
            break;
        }
        lane_index++;
    }
 
    if (!SOC_WARM_BOOT(unit)) {
        candidate_lane_conn = &(PM_4x10_INFO(pm_info)->lane_conn[phyn-1][lane_index]);

        xphy_id = candidate_lane_conn->xphy_id;
        lane_mask = candidate_lane_conn->ss_lane_mask;

        if ((lane_mask != -1) && (xphy_id != PORTMOD_XPHY_ID_INVALID)) {

            /*
            * Keeping track of the xphy lane usage.  If the xphy lane is disconnected
            * it will clear the corresponding bit to mark as lane is not active.
            * When none of the lane are active, it clear intialization status,
            * phy_type and probe status.  When it probe again, it will re-initialize
            * this xphy again.
            */
            _SOC_IF_ERR_EXIT(portmod_xphy_active_lane_get(unit, xphy_id, &active_lanes));
            active_lanes &= ~lane_mask;
            _SOC_IF_ERR_EXIT(portmod_xphy_active_lane_set(unit, xphy_id, active_lanes));

            if (active_lanes == 0) { /* none of the lanes are active */
                _SOC_IF_ERR_EXIT(portmod_xphy_core_probed_set(unit, xphy_id, 0));
                _SOC_IF_ERR_EXIT(portmod_xphy_phy_type_set(unit,xphy_id,phymodDispatchTypeCount));
                _SOC_IF_ERR_EXIT(portmod_xphy_is_initialized_set(unit, xphy_id,0));
            }
        }

        portmod_xphy_lane_connection_t_init(unit, candidate_lane_conn);
        rv = PM4x10_PORT_LANE_CONNECTION_DB_SET(unit, pm_info, candidate_lane_conn, phyn-1, lane_index);
        _SOC_IF_ERR_EXIT(rv);

        if (phyn > 0) {
            nof_phys = phyn;
            rv = PM4x10_PORT_NUM_PHYS_SET(unit, pm_info, &nof_phys, lane_index);
            _SOC_IF_ERR_EXIT(rv);
            PM_4x10_INFO(pm_info)->nof_phys[lane_index] = nof_phys;
        }
    }
 
exit:
   SOC_FUNC_RETURN;
}

int pm4x10_port_control_phy_timesync_set(int unit, int port, pm_info_t pm_info, const portmod_port_control_phy_timesync_t config, uint64 value)
{
    phymod_phy_access_t phy_access[1+MAX_PHYN];
    int nof_phys;
    SOC_INIT_FUNC_DEFS;

    _SOC_IF_ERR_EXIT(portmod_port_chain_phy_access_get(unit, port, pm_info,
                                                       phy_access ,(1+MAX_PHYN),
                                                       &nof_phys));

    SOC_IF_ERROR_RETURN(portmod_port_phychain_control_phy_timesync_set(unit, port, phy_access, nof_phys,
                                                                       config, value));

    exit:
    SOC_FUNC_RETURN;
}

int pm4x10_port_control_phy_timesync_get(int unit, int port, pm_info_t pm_info, const portmod_port_control_phy_timesync_t config, uint64* value)
{
    phymod_phy_access_t phy_access[1+MAX_PHYN];
    int nof_phys;
    SOC_INIT_FUNC_DEFS;

    _SOC_IF_ERR_EXIT(portmod_port_chain_phy_access_get(unit, port, pm_info,
                                                       phy_access ,(1+MAX_PHYN),
                                                       &nof_phys));

    SOC_IF_ERROR_RETURN(portmod_port_phychain_control_phy_timesync_get(unit, port, phy_access, nof_phys,
                                                                       config, value));

    exit:
    SOC_FUNC_RETURN;
}

int pm4x10_port_update_dynamic_state(int unit, int port, pm_info_t pm_info, uint32_t port_dynamic_state) {
    
    int rv = 0;
    int port_index;
    uint32_t bitmap;
    phymod_an_mode_type_t an_mode;
    uint32_t an_updated = FALSE, tmp_state;
    SOC_INIT_FUNC_DEFS;


    rv = _pm4x10_port_index_get (unit, port, pm_info, &port_index, &bitmap);
    if (SOC_FAILURE(rv)) {
        _SOC_EXIT_WITH_ERR(SOC_E_PARAM, (_SOC_MSG("Unable to retrieve the port index")));
    }

    if (PORTMOD_PORT_IS_AUTONEG_MODE_UPDATED(port_dynamic_state)) {
        rv = PM4x10_PORT_DYNAMIC_STATE_GET(unit, pm_info, &tmp_state,
                                           port_index);
        _SOC_IF_ERR_EXIT(rv);
        /*we need to change the data base An_mode based on the new value */
        switch (port_dynamic_state >> 16 & 0xffff) { 
        case SOC_PHY_CONTROL_AUTONEG_MODE_CL37:
            an_mode = phymod_AN_MODE_CL37;
            an_updated = TRUE;
            break;
        case SOC_PHY_CONTROL_AUTONEG_MODE_CL37_BAM:
            an_mode = phymod_AN_MODE_CL37BAM;
            an_updated = TRUE;
            break;
        case SOC_PHY_CONTROL_AUTONEG_MODE_CL73:
            an_mode = phymod_AN_MODE_CL73;
            an_updated = TRUE;
            break;
        case SOC_PHY_CONTROL_AUTONEG_MODE_CL73_BAM:
            an_mode = phymod_AN_MODE_CL73BAM;
            an_updated = TRUE;
            break;
        default:
            break;
        }
    } else if (PORTMOD_PORT_IS_DEFAULT_TX_PARAMS_UPDATED(port_dynamic_state)) {
        rv = PM4x10_PORT_DYNAMIC_STATE_SET(unit, pm_info,
                                      port_dynamic_state, port_index);
        _SOC_IF_ERR_EXIT(rv);
    }
    if (an_updated == TRUE) {
        rv = PM4x10_AN_MODE_SET(unit, pm_info, an_mode, port_index);
        _SOC_IF_ERR_EXIT(rv);
        PORTMOD_PORT_AUTONEG_MODE_UPDATED_SET(tmp_state);
        rv = PM4x10_PORT_DYNAMIC_STATE_SET(unit, pm_info, tmp_state,
                                           port_index);
        _SOC_IF_ERR_EXIT(rv);
    }

exit:
    SOC_FUNC_RETURN;

}

int pm4x10_port_phy_op_mode_get(int unit, int port, pm_info_t pm_info, phymod_operation_mode_t* val)
{
    uint32 bitmap;
    int    port_index, rv = 0;

    rv = _pm4x10_port_index_get (unit, port, pm_info, &port_index, &bitmap);
    if SOC_FAILURE(rv) return (rv);

    rv = PM4x10_PORT_OP_MODE_GET(unit, pm_info, val, port_index);

    return (rv);
}

int pm4x10_port_medium_config_set(int unit, int port, pm_info_t pm_info, soc_port_medium_t medium, soc_phy_config_t* config)
{
    int rv;
    SOC_INIT_FUNC_DEFS;

    rv = portmod_port_phychain_medium_config_set(unit, port, medium, config);
    if (SOC_FAILURE(rv)) return (rv);

    SOC_FUNC_RETURN;
}

int pm4x10_port_medium_config_get(int unit, int port, pm_info_t pm_info, soc_port_medium_t medium, soc_phy_config_t* config)
{
    int rv;
    SOC_INIT_FUNC_DEFS;

    rv = portmod_port_phychain_medium_config_get(unit, port, medium, config);

    if (SOC_FAILURE(rv)) return (rv);

    SOC_FUNC_RETURN;
}

int pm4x10_port_medium_get(int unit, int port, pm_info_t pm_info, soc_port_medium_t* medium)
{
    int rv;
    SOC_INIT_FUNC_DEFS;

    rv = portmod_port_phychain_medium_get(unit, port, medium);
    if (SOC_FAILURE(rv)) return (rv);

    SOC_FUNC_RETURN;
}

int pm4x10_port_multi_get(int unit, int port, pm_info_t pm_info, portmod_multi_get_t* multi_get)
{
    phymod_phy_access_t phy_access[1+MAX_PHYN];
    int nof_phys;
    SOC_INIT_FUNC_DEFS;

    _SOC_IF_ERR_EXIT
        (portmod_port_chain_phy_access_get(unit, port, pm_info, 
                                           phy_access ,(1+MAX_PHYN),
                                           &nof_phys));
    _SOC_IF_ERR_EXIT
        (portmod_port_phychain_multi_get(unit, port, phy_access, nof_phys, multi_get));

exit:
    SOC_FUNC_RETURN;

}

int pm4x10_port_polarity_set(int unit, int port, pm_info_t pm_info, const phymod_polarity_t* polarity)
{
    SOC_INIT_FUNC_DEFS;

    PM_4x10_INFO(pm_info)->polarity = *polarity;


    SOC_FUNC_RETURN;
}

int pm4x10_port_polarity_get(int unit, int port, pm_info_t pm_info, phymod_polarity_t* polarity)
{
    SOC_INIT_FUNC_DEFS;

    *polarity = PM_4x10_INFO(pm_info)->polarity;


    SOC_FUNC_RETURN;
}

int pm4x10_pm_is_in_pm12x10(int unit, pm_info_t pm_info, int* in_pm12x10)
{
    SOC_INIT_FUNC_DEFS;

    if (PM_4x10_INFO(pm_info)->in_pm12x10){
        *in_pm12x10 = 1;
    } else {
        *in_pm12x10 = 0;
    }

    SOC_FUNC_RETURN;

}

int pm4x10_port_pll_div_get(int unit, int port,
                            const portmod_port_resources_t* port_resource,
                            uint32_t* pll_div)
{
    SOC_INIT_FUNC_DEFS;

    switch (port_resource->speed){
      case 10:
          *pll_div = phymod_TSCE_PLL_DIV66;
          break;
      case 100:
          *pll_div = phymod_TSCE_PLL_DIV66;
          break;
      case 1000:
          *pll_div = phymod_TSCE_PLL_DIV66;
          break;
      case 2500:
          *pll_div = phymod_TSCE_PLL_DIV66;
          break;
      case 5000:
          *pll_div = phymod_TSCE_PLL_DIV40;
          break;
      case 10000:
          if (port_resource->num_lane == 1){
              *pll_div = phymod_TSCE_PLL_DIV66;
          } else {
              *pll_div = phymod_TSCE_PLL_DIV40;
          }
          break;
      case 105000:
          *pll_div = phymod_TSCE_PLL_DIV42;
          break;
      case 11000:
          if (port_resource->num_lane == 1){
              *pll_div = phymod_TSCE_PLL_DIV70;
          } else {
              *pll_div = phymod_TSCE_PLL_DIV42;
          }
          break;
      case 12000:
          *pll_div = phymod_TSCE_PLL_DIV80;
          break;
      case 13000:
          if (port_resource->num_lane == 2){
              *pll_div = phymod_TSCE_PLL_DIV42;
          } else {
              *pll_div = phymod_TSCE_PLL_DIV52;
          }
          break;
      case 15000:
          *pll_div = phymod_TSCE_PLL_DIV66;
          break;
      case 15700:
          *pll_div = phymod_TSCE_PLL_DIV52;
          break;
      case 16000:
          *pll_div = phymod_TSCE_PLL_DIV64;
          break;
      case 20000:
          if (port_resource->num_lane == 2){
              *pll_div = phymod_TSCE_PLL_DIV66;
          } else {
              *pll_div = phymod_TSCE_PLL_DIV40;
          }
          break;
      case 21000:
          if (port_resource->num_lane == 2){
              *pll_div = phymod_TSCE_PLL_DIV70;
          } else {
              *pll_div = phymod_TSCE_PLL_DIV42;
          }
          break;
      case 25000:
          if (port_resource->num_lane == 2){
              *pll_div = phymod_TSCE_PLL_DIV80;
          } else {
              *pll_div = phymod_TSCE_PLL_DIV42;
          }
          break;
      case 25450:
          *pll_div = phymod_TSCE_PLL_DIV42;
          break;
      case 30000:
      case 32000:
          *pll_div = phymod_TSCE_PLL_DIV52;
          break;
      case 40000:
          *pll_div = phymod_TSCE_PLL_DIV66;
          break;
      case 42000:
          *pll_div = phymod_TSCE_PLL_DIV70;
          break;
      case 48000:
          *pll_div = phymod_TSCE_PLL_DIV80;
          break;
      case 100000:
          *pll_div = phymod_TSCE_PLL_DIV66;
          break;
      case 106000:
      case 107000:
          *pll_div = phymod_TSCE_PLL_DIV70;
          break;
      case 120000:
          *pll_div = phymod_TSCE_PLL_DIV66;
          break;
      case 127000:
          *pll_div = phymod_TSCE_PLL_DIV70;
          break;
      default:
          *pll_div = phymod_TSCE_PLL_DIV66;
          break;
    }

    SOC_FUNC_RETURN;
}

int pm4x10_port_master_get(int unit, int port, pm_info_t pm_info, int* master_mode)
{
    SOC_INIT_FUNC_DEFS;

    _SOC_IF_ERR_EXIT(portmod_port_phychain_master_get(unit, port, pm_info, master_mode));

exit:
    SOC_FUNC_RETURN;
}


int pm4x10_port_lane_count_get (int unit, int port, pm_info_t pm_info, int line_side, int* num_lanes)
{
    portmod_access_get_params_t params;
    phymod_phy_access_t phy_access;
    int nof_phys;
    int is_outer_most = 0;
    SOC_INIT_FUNC_DEFS;

    portmod_access_get_params_t_init(unit, &params);

    if(line_side){
        params.sys_side =  PORTMOD_SIDE_LINE;
    } else {
        params.sys_side =  PORTMOD_SIDE_SYSTEM;
    }

    _SOC_IF_ERR_EXIT(pm4x10_port_phy_lane_access_get(unit, port, pm_info,
                                                     &params, 1 , &phy_access,
                                                     &nof_phys, &is_outer_most));

    *num_lanes = 0;
    PORTMOD_BIT_COUNT(phy_access.access.lane_mask,*num_lanes);

exit:
    SOC_FUNC_RETURN;
}


#endif /* PORTMOD_PM4X10_SUPPORT */

#undef _ERR_MSG_MODULE_NAME

