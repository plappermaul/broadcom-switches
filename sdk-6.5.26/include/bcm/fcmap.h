/*
 * 
 * 
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2022 Broadcom Inc. All rights reserved.
 *
 */

#ifndef __BCM_FCMAP_H__
#define __BCM_FCMAP_H__

#if defined(INCLUDE_FCMAP)

#include <bcm/types.h>
#include <bfcmap.h>

typedef _shr_bfcmap_dev_addr_t bcm_fcmap_dev_addr_t;

typedef enum bcm_fcmap_dir_e {
    BCM_FCMAP_DIR_EGRESS = _SHR_BFCMAP_DIR_EGRESS, 
    BCM_FCMAP_DIR_INGRESS = _SHR_BFCMAP_DIR_INGRESS 
} bcm_fcmap_dir_t;

typedef enum bcm_fcmap_core_e {
    BCM_FCMAP_CORE_BCM84756 = _SHR_BFCMAP_CORE_BCM84756, 
    BCM_FCMAP_CORE_BCM88060_A0 = _SHR_BFCMAP_CORE_BCM88060_A0, 
    BCM_FCMAP_CORE_BCM88061_A0 = _SHR_BFCMAP_CORE_BCM88061_A0 
} bcm_fcmap_core_t;

typedef enum bcm_fcmap_port_mode_e {
    BCM_FCMAP_FCOE_TO_FC_MODE = _SHR_BFCMAP_FCOE_TO_FC_MODE, 
    BCM_FCMAP_FCOE_TO_FCOE_MODE = _SHR_BFCMAP_FCOE_TO_FCOE_MODE 
} bcm_fcmap_port_mode_t;

typedef enum bcm_fcmap_port_speed_e {
    BCM_FCMAP_PORT_SPEED_AN = _SHR_BFCMAP_PORT_SPEED_AN, 
    BCM_FCMAP_PORT_SPEED_2GBPS = _SHR_BFCMAP_PORT_SPEED_2GBPS, 
    BCM_FCMAP_PORT_SPEED_4GBPS = _SHR_BFCMAP_PORT_SPEED_4GBPS, 
    BCM_FCMAP_PORT_SPEED_8GBPS = _SHR_BFCMAP_PORT_SPEED_8GBPS, 
    BCM_FCMAP_PORT_SPEED_16GBPS = _SHR_BFCMAP_PORT_SPEED_16GBPS, 
    BCM_FCMAP_PORT_SPEED_32GBPS = _SHR_BFCMAP_PORT_SPEED_32GBPS, 
    BCM_FCMAP_PORT_SPEED_AN_2GBPS = _SHR_BFCMAP_PORT_SPEED_AN_2GBPS, 
    BCM_FCMAP_PORT_SPEED_AN_4GBPS = _SHR_BFCMAP_PORT_SPEED_AN_4GBPS, 
    BCM_FCMAP_PORT_SPEED_AN_8GBPS = _SHR_BFCMAP_PORT_SPEED_AN_8GBPS, 
    BCM_FCMAP_PORT_SPEED_AN_16GBPS = _SHR_BFCMAP_PORT_SPEED_AN_16GBPS, 
    BCM_FCMAP_PORT_SPEED_AN_32GBPS = _SHR_BFCMAP_PORT_SPEED_AN_32GBPS, 
    BCM_FCMAP_PORT_SPEED_MAX_COUNT = _SHR_BFCMAP_PORT_SPEED_MAX_COUNT 
} bcm_fcmap_port_speed_t;

typedef enum bcm_fcmap_port_state_e {
    BCM_FCMAP_PORT_STATE_INIT = _SHR_BFCMAP_PORT_STATE_INIT, 
    BCM_FCMAP_PORT_STATE_RESET = _SHR_BFCMAP_PORT_STATE_RESET, 
    BCM_FCMAP_PORT_STATE_ACTIVE = _SHR_BFCMAP_PORT_STATE_ACTIVE, 
    BCM_FCMAP_PORT_STATE_LINKDOWN = _SHR_BFCMAP_PORT_STATE_LINKDOWN, 
    BCM_FCMAP_PORT_STATE_DISABLE = _SHR_BFCMAP_PORT_STATE_DISABLE, 
    BCM_FCMAP_PORT_STATE_MAX_COUNT = _SHR_BFCMAP_PORT_STATE_MAX_COUNT 
} bcm_fcmap_port_state_t;

typedef enum bcm_fcmap_encap_mac_address_e {
    BCM_FCMAP_ENCAP_FCOE_FPMA = _SHR_BFCMAP_ENCAP_FCOE_FPMA, 
    BCM_FCMAP_ENCAP_FCOE_ETH_ADDRESS_NULL = _SHR_BFCMAP_ENCAP_FCOE_ETH_ADDRESS_NULL, 
    BCM_FCMAP_ENCAP_FCOE_ETH_ADDRESS_USER = _SHR_BFCMAP_ENCAP_FCOE_ETH_ADDRESS_USER 
} bcm_fcmap_encap_mac_address_t;

typedef enum bcm_fcmap_8g_fw_on_active_e {
    BCM_FCMAP_8G_FW_ON_ACTIVE_ARBFF = _SHR_BFCMAP_8G_FW_ON_ACTIVE_ARBFF, 
    BCM_FCMAP_8G_FW_ON_ACTIVE_IDLE = _SHR_BFCMAP_8G_FW_ON_ACTIVE_IDLE 
} bcm_fcmap_8g_fw_on_active_t;

typedef enum bcm_fcmap_map_table_input_e {
    BCM_FCMAP_MAP_TABLE_INPUT_VID = _SHR_BFCMAP_MAP_TABLE_INPUT_VID, 
    BCM_FCMAP_MAP_TABLE_INPUT_VFID = _SHR_BFCMAP_MAP_TABLE_INPUT_VFID 
} bcm_fcmap_map_table_input_t;

typedef enum bcm_fcmap_fc_crc_mode_e {
    BCM_FCMAP_FC_CRC_MODE_NO_CRC_CHECK = _SHR_BFCMAP_FC_CRC_MODE_NO_CRC_CHECK, 
    BCM_FCMAP_FC_CRC_MODE_NORMAL = _SHR_BFCMAP_FC_CRC_MODE_NORMAL 
} bcm_fcmap_fc_crc_mode_t;

typedef enum bcm_fcmap_vfthdr_proc_mode_e {
    BCM_FCMAP_VFTHDR_PRESERVE = _SHR_BFCMAP_VFTHDR_PRESERVE, 
    BCM_FCMAP_VFTHDR_INSERT = _SHR_BFCMAP_VFTHDR_INSERT, 
    BCM_FCMAP_VFTHDR_DELETE = _SHR_BFCMAP_VFTHDR_DELETE 
} bcm_fcmap_vfthdr_proc_mode_t;

typedef enum bcm_fcmap_vlantag_proc_mode_e {
    BCM_FCMAP_VLANTAG_PRESERVE = _SHR_BFCMAP_VLANTAG_PRESERVE, 
    BCM_FCMAP_VLANTAG_INSERT = _SHR_BFCMAP_VLANTAG_INSERT, 
    BCM_FCMAP_VLANTAG_DELETE = _SHR_BFCMAP_VLANTAG_DELETE 
} bcm_fcmap_vlantag_proc_mode_t;

typedef enum bcm_fcmap_vfid_mapsrc_e {
    BCM_FCMAP_VFID_MACSRC_PASSTHRU = _SHR_BFCMAP_VFID_MACSRC_PASSTHRU, 
    BCM_FCMAP_VFID_MACSRC_PORT_DEFAULT = _SHR_BFCMAP_VFID_MACSRC_PORT_DEFAULT, 
    BCM_FCMAP_VFID_MACSRC_VID = _SHR_BFCMAP_VFID_MACSRC_VID, 
    BCM_FCMAP_VFID_MACSRC_MAPPER = _SHR_BFCMAP_VFID_MACSRC_MAPPER, 
    BCM_FCMAP_VFID_MACSRC_DISABLE = _SHR_BFCMAP_VFID_MACSRC_DISABLE 
} bcm_fcmap_vfid_mapsrc_t;

typedef enum bcm_fcmap_vid_mapsrc_e {
    BCM_FCMAP_VID_MACSRC_PASSTHRU = _SHR_BFCMAP_VID_MACSRC_PASSTHRU, 
    BCM_FCMAP_VID_MACSRC_PORT_DEFAULT = _SHR_BFCMAP_VID_MACSRC_PORT_DEFAULT, 
    BCM_FCMAP_VID_MACSRC_VFID = _SHR_BFCMAP_VID_MACSRC_VFID, 
    BCM_FCMAP_VID_MACSRC_MAPPER = _SHR_BFCMAP_VID_MACSRC_MAPPER 
} bcm_fcmap_vid_mapsrc_t;

typedef enum bcm_fcmap_vlan_pri_map_mode_e {
    BCM_FCMAP_VLAN_PRI_MAP_MODE_PORT_DEFAULT = _SHR_BFCMAP_VLAN_PRI_MAP_MODE_PORT_DEFAULT, 
    BCM_FCMAP_VLAN_PRI_MAP_MODE_PASSTHRU = _SHR_BFCMAP_VLAN_PRI_MAP_MODE_PASSTHRU 
} bcm_fcmap_vlan_pri_map_mode_t;

typedef enum bcm_fcmap_hopcnt_check_mode_e {
    BCM_FCMAP_HOPCNT_CHECK_MODE_NO_CHK = _SHR_BFCMAP_HOPCNT_CHECK_MODE_NO_CHK, 
    BCM_FCMAP_HOPCNT_CHECK_MODE_FAIL_DROP = _SHR_BFCMAP_HOPCNT_CHECK_MODE_FAIL_DROP, 
    BCM_FCMAP_HOPCNT_CHECK_MODE_FAIL_EOFNI = _SHR_BFCMAP_HOPCNT_CHECK_MODE_FAIL_EOFNI 
} bcm_fcmap_hopcnt_check_mode_t;

/* link fault trigger. */
typedef enum bcm_fcmap_lf_tr_e {
    BCM_FCMAP_LF_TR_NONE = _SHR_BFCMAP_LF_TR_NONE, 
    BCM_FCMAP_LF_TR_PORT_INIT = _SHR_BFCMAP_LF_TR_PORT_INIT, 
    BCM_FCMAP_LF_TR_OPEN_LINK = _SHR_BFCMAP_LF_TR_OPEN_LINK, 
    BCM_FCMAP_LF_TR_LINK_FAILURE = _SHR_BFCMAP_LF_TR_LINK_FAILURE, 
    BCM_FCMAP_LF_TR_OLS_RCVD = _SHR_BFCMAP_LF_TR_OLS_RCVD, 
    BCM_FCMAP_LF_TR_NOS_RCVD = _SHR_BFCMAP_LF_TR_NOS_RCVD, 
    BCM_FCMAP_LF_TR_SYNC_LOSS = _SHR_BFCMAP_LF_TR_SYNC_LOSS, 
    BCM_FCMAP_LF_TR_BOUCELINK_FROM_ADMIN = _SHR_BFCMAP_LF_TR_BOUCELINK_FROM_ADMIN, 
    BCM_FCMAP_LF_TR_CHGSPEED_FROM_ADMIN = _SHR_BFCMAP_LF_TR_CHGSPEED_FROM_ADMIN, 
    BCM_FCMAP_LF_TR_DISABLE_FROM_ADMIN = _SHR_BFCMAP_LF_TR_DISABLE_FROM_ADMIN, 
    BCM_FCMAP_LF_TR_RESET_FROM_ADMIN = _SHR_BFCMAP_LF_TR_RESET_FROM_ADMIN, 
    BCM_FCMAP_LF_TR_LR_RCVD = _SHR_BFCMAP_LF_TR_LR_RCVD, 
    BCM_FCMAP_LF_TR_LRR_RCVD = _SHR_BFCMAP_LF_TR_LRR_RCVD, 
    BCM_FCMAP_LF_TR_ED_TOV = _SHR_BFCMAP_LF_TR_ED_TOV, 
    BCM_FCMAP_LF_TR_SYS_LNK_FAILURE = _SHR_BFCMAP_LF_TR_SYS_LNK_FAILURE 
} bcm_fcmap_lf_tr_t;

/* link fault reason code. */
typedef enum bcm_fcmap_lf_rc_e {
    BCM_FCMAP_LF_RC_NONE = _SHR_BFCMAP_LF_RC_NONE, 
    BCM_FCMAP_LF_RC_PORT_INIT = _SHR_BFCMAP_LF_RC_PORT_INIT, 
    BCM_FCMAP_LF_RC_OPEN_LINK = _SHR_BFCMAP_LF_RC_OPEN_LINK, 
    BCM_FCMAP_LF_RC_LINK_FAILURE = _SHR_BFCMAP_LF_RC_LINK_FAILURE, 
    BCM_FCMAP_LF_RC_OLS_RCVD = _SHR_BFCMAP_LF_RC_OLS_RCVD, 
    BCM_FCMAP_LF_RC_NOS_RCVD = _SHR_BFCMAP_LF_RC_NOS_RCVD, 
    BCM_FCMAP_LF_RC_SYNC_LOSS = _SHR_BFCMAP_LF_RC_SYNC_LOSS, 
    BCM_FCMAP_LF_RC_BOUCELINK_FROM_ADMIN = _SHR_BFCMAP_LF_RC_BOUCELINK_FROM_ADMIN, 
    BCM_FCMAP_LF_RC_CHGSPEED_FROM_ADMIN = _SHR_BFCMAP_LF_RC_CHGSPEED_FROM_ADMIN, 
    BCM_FCMAP_LF_RC_DISABLE_FROM_ADMIN = _SHR_BFCMAP_LF_RC_DISABLE_FROM_ADMIN, 
    BCM_FCMAP_LF_RC_RESET_FAILURE = _SHR_BFCMAP_LF_RC_RESET_FAILURE, 
    BCM_FCMAP_LF_RC_SYS_LNK_FAILURE = _SHR_BFCMAP_LF_RC_SYS_LNK_FAILURE 
} bcm_fcmap_lf_rc_t;

typedef enum bcm_fcmap_diag_code_e {
    BCM_FCMAP_DIAG_OK = _SHR_BFCMAP_DIAG_OK, 
    BCM_FCMAP_DIAG_PORT_INIT = _SHR_BFCMAP_DIAG_PORT_INIT, 
    BCM_FCMAP_DIAG_OPEN_LINK = _SHR_BFCMAP_DIAG_OPEN_LINK, 
    BCM_FCMAP_DIAG_LINK_FAILUR = _SHR_BFCMAP_DIAG_LINK_FAILURE, 
    BCM_FCMAP_DIAG_OLS_RCVD = _SHR_BFCMAP_DIAG_OLS_RCVD, 
    BCM_FCMAP_DIAG_NOS_RCVD = _SHR_BFCMAP_DIAG_NOS_RCVD, 
    BCM_FCMAP_DIAG_SYNC_LOSS = _SHR_BFCMAP_DIAG_SYNC_LOSS, 
    BCM_FCMAP_DIAG_BOUCELINK_FROM_ADMIN = _SHR_BFCMAP_DIAG_BOUCELINK_FROM_ADMIN, 
    BCM_FCMAP_DIAG_CHGSPEED_FROM_ADMIN = _SHR_BFCMAP_DIAG_CHGSPEED_FROM_ADMIN, 
    BCM_FCMAP_DIAG_DISABLE_FROM_ADMIN = _SHR_BFCMAP_DIAG_DISABLE_FROM_ADMIN, 
    BCM_FCMAP_DIAG_AN_NO_SIGNAL = _SHR_BFCMAP_DIAG_AN_NO_SIGNAL, 
    BCM_FCMAP_DIAG_AN_TIMEOUT = _SHR_BFCMAP_DIAG_AN_TIMEOUT, 
    BCM_FCMAP_DIAG_PROTO_TIMEOUT = _SHR_BFCMAP_DIAG_PROTO_TIMEOUT, 
    BCM_FCMAP_DIAG_SYS_LNK_FAILURE = _SHR_BFCMAP_DIAG_SYS_LNK_FAILURE 
} bcm_fcmap_diag_code_t;

#define BCM_FCMAP_ATTR_PORT_MODE_MASK       _SHR_FCMAP_ATTR_PORT_MODE_MASK 
#define BCM_FCMAP_ATTR_SPEED_MASK           _SHR_FCMAP_ATTR_SPEED_MASK 
#define BCM_FCMAP_ATTR_TX_BB_CREDITS_MASK   _SHR_FCMAP_ATTR_TX_BB_CREDITS_MASK 
#define BCM_FCMAP_ATTR_RX_BB_CREDITS_MASK   _SHR_FCMAP_ATTR_RX_BB_CREDITS_MASK 
#define BCM_FCMAP_ATTR_MAX_FRAME_LENGTH_MASK _SHR_FCMAP_ATTR_MAX_FRAME_LENGTH_MASK 
#define BCM_FCMAP_ATTR_BB_SC_N_MASK         _SHR_FCMAP_ATTR_BB_SC_N_MASK 
#define BCM_FCMAP_ATTR_PORT_STATE_MASK      _SHR_FCMAP_ATTR_PORT_STATE_MASK 
#define BCM_FCMAP_ATTR_R_T_TOV_MASK         _SHR_FCMAP_ATTR_R_T_TOV_MASK 
#define BCM_FCMAP_ATTR_INTERRUPT_ENABLE_MASK _SHR_FCMAP_ATTR_INTERRUPT_ENABLE_MASK 
#define BCM_FCMAP_ATTR_FW_ON_ACTIVE_8G_MASK _SHR_FCMAP_ATTR_FW_ON_ACTIVE_8G_MASK 
#define BCM_FCMAP_ATTR_SRC_MAC_CONSTRUCT_MASK _SHR_FCMAP_ATTR_SRC_MAC_CONSTRUCT_MASK 
#define BCM_FCMAP_ATTR_DST_MAC_CONSTRUCT_MASK _SHR_FCMAP_ATTR_DST_MAC_CONSTRUCT_MASK 
#define BCM_FCMAP_ATTR_VLAN_TAG_MASK        _SHR_FCMAP_ATTR_VLAN_TAG_MASK 
#define BCM_FCMAP_ATTR_VFT_TAG_MASK         _SHR_FCMAP_ATTR_VFT_TAG_MASK 
#define BCM_FCMAP_ATTR_MAPPER_LEN_MASK      _SHR_FCMAP_ATTR_MAPPER_LEN_MASK 
#define BCM_FCMAP_ATTR_INGRESS_MAPPER_BYPASS_MASK _SHR_FCMAP_ATTR_INGRESS_MAPPER_BYPASS_MASK 
#define BCM_FCMAP_ATTR_EGRESS_MAPPER_BYPASS_MASK _SHR_FCMAP_ATTR_EGRESS_MAPPER_BYPASS_MASK 
#define BCM_FCMAP_ATTR_INGRESS_MAP_TABLE_INPUT_MASK _SHR_FCMAP_ATTR_INGRESS_MAP_TABLE_INPUT_MASK 
#define BCM_FCMAP_ATTR_EGRESS_MAP_TABLE_INPUT_MASK _SHR_FCMAP_ATTR_EGRESS_MAP_TABLE_INPUT_MASK 
#define BCM_FCMAP_ATTR_INGRESS_FC_CRC_MODE_MASK _SHR_FCMAP_ATTR_INGRESS_FC_CRC_MODE_MASK 
#define BCM_FCMAP_ATTR_EGRESS_FC_CRC_MODE_MASK _SHR_FCMAP_ATTR_EGRESS_FC_CRC_MODE_MASK 
#define BCM_FCMAP_ATTR_INGRESS_VFTHDR_PROC_MODE_MASK _SHR_FCMAP_ATTR_INGRESS_VFTHDR_PROC_MODE_MASK 
#define BCM_FCMAP_ATTR_EGRESS_VFTHDR_PROC_MODE_MASK _SHR_FCMAP_ATTR_EGRESS_VFTHDR_PROC_MODE_MASK 
#define BCM_FCMAP_ATTR_INGRESS_VLANTAG_PROC_MODE_MASK _SHR_FCMAP_ATTR_INGRESS_VLANTAG_PROC_MODE_MASK 
#define BCM_FCMAP_ATTR_EGRESS_VLANTAG_PROC_MODE_MASK _SHR_FCMAP_ATTR_EGRESS_VLANTAG_PROC_MODE_MASK 
#define BCM_FCMAP_ATTR_INGRESS_VFID_MAPSRC_MASK _SHR_FCMAP_ATTR_INGRESS_VFID_MAPSRC_MASK 
#define BCM_FCMAP_ATTR_EGRESS_VFID_MAPSRC_MASK _SHR_FCMAP_ATTR_EGRESS_VFID_MAPSRC_MASK 
#define BCM_FCMAP_ATTR_INGRESS_VID_MAPSRC_MASK _SHR_FCMAP_ATTR_INGRESS_VID_MAPSRC_MASK 
#define BCM_FCMAP_ATTR_EGRESS_VID_MAPSRC_MASK _SHR_FCMAP_ATTR_EGRESS_VID_MAPSRC_MASK 
#define BCM_FCMAP_ATTR_INGRESS_VLAN_PRI_MAP_MODE_MASK _SHR_FCMAP_ATTR_INGRESS_VLAN_PRI_MAP_MODE_MASK 
#define BCM_FCMAP_ATTR_EGRESS_VLAN_PRI_MAP_MODE_MASK _SHR_FCMAP_ATTR_EGRESS_VLAN_PRI_MAP_MODE_MASK 
#define BCM_FCMAP_ATTR_INGRESS_HOPCNT_CHECK_MODE_MASK _SHR_FCMAP_ATTR_INGRESS_HOPCNT_CHECK_MODE_MASK 
#define BCM_FCMAP_ATTR2_EGRESS_HOPCNT_DEC_ENABLE_MASK _SHR_FCMAP_ATTR2_EGRESS_HOPCNT_DEC_ENABLE_MASK 
#define BCM_FCMAP_ATTR2_USE_TTS_PCS_16G_MASK _SHR_FCMAP_ATTR2_USE_TTS_PCS_16G_MASK 
#define BCM_FCMAP_ATTR2_USE_TTS_PCS_32G_MASK _SHR_FCMAP_ATTR2_USE_TTS_PCS_32G_MASK 
#define BCM_FCMAP_ATTR2_TRAINING_ENABLE_16G_MASK _SHR_FCMAP_ATTR2_TRAINING_ENABLE_16G_MASK 
#define BCM_FCMAP_ATTR2_TRAINING_ENABLE_32G_MASK _SHR_FCMAP_ATTR2_TRAINING_ENABLE_32G_MASK 
#define BCM_FCMAP_ATTR2_FEC_ENABLE_16G_MASK _SHR_FCMAP_ATTR2_FEC_ENABLE_16G_MASK 
#define BCM_FCMAP_ATTR2_FEC_ENABLE_32G_MASK _SHR_FCMAP_ATTR2_FEC_ENABLE_32G_MASK 
#define BCM_FCMAP_ATTR2_INGRESS_FCS_CRRPT_EOF_ENABLE_MASK _SHR_FCMAP_ATTR2_INGRESS_FCS_CRRPT_EOF_ENABLE_MASK 
#define BCM_FCMAP_ATTR2_INGRESS_VLANTAG_PRESENCE_ENABLE_MASK _SHR_FCMAP_ATTR2_INGRESS_VLANTAG_PRESENCE_ENABLE_MASK 
#define BCM_FCMAP_ATTR2_EGRESS_HOPCNT_CHECK_MODE_MASK _SHR_FCMAP_ATTR2_EGRESS_HOPCNT_CHECK_MODE_MASK 
#define BCM_FCMAP_ATTR2_EGRESS_PASS_CTRL_FRAME_ENABLE_MASK _SHR_FCMAP_ATTR2_EGRESS_PASS_CTRL_FRAME_ENABLE_MASK 
#define BCM_FCMAP_ATTR2_EGRESS_PASS_PFC_FRAME_ENABLE_MASK _SHR_FCMAP_ATTR2_EGRESS_PASS_PFC_FRAME_ENABLE_MASK 
#define BCM_FCMAP_ATTR2_EGRESS_PASS_PAUSE_FRAME_ENABLE_MASK _SHR_FCMAP_ATTR2_EGRESS_PASS_PAUSE_FRAME_ENABLE_MASK 
#define BCM_FCMAP_ATTR2_EGRESS_FCOE_VER_CHK_DISABLE_MASK _SHR_FCMAP_ATTR2_EGRESS_FCOE_VER_CHK_DISABLE_MASK 
#define BCM_FCMAP_ATTR2_EGRESS_DEFAULT_COS_VALUE_MASK _SHR_FCMAP_ATTR2_EGRESS_DEFAULT_COS_VALUE_MASK 
#define BCM_FCMAP_ATTR2_EGRESS_USE_IP_COS_MAP_MASK _SHR_FCMAP_ATTR2_EGRESS_USE_IP_COS_MAP_MASK 
#define BCM_FCMAP_ATTR2_INGRESS_HOPCNT_DEC_ENABLE_MASK _SHR_FCMAP_ATTR2_INGRESS_HOPCNT_DEC_ENABLE_MASK 
#define BCM_FCMAP_ATTR2_SCRAMBLING_ENABLE_MASK _SHR_FCMAP_ATTR2_SCRAMBLING_ENABLE_MASK 
#define BCM_FCMAP_ATTR2_EGRESS_PAUSE_ENABLE_MASK _SHR_FCMAP_ATTR2_EGRESS_PAUSE_ENABLE_MASK 
#define BCM_FCMAP_ATTR2_EGRESS_PFC_ENABLE_MASK _SHR_FCMAP_ATTR2_EGRESS_PFC_ENABLE_MASK 
#define BCM_FCMAP_ATTR2_STAT_INTERVAL_MASK  _SHR_FCMAP_ATTR2_STAT_INTERVAL_MASK 
#define BCM_FCMAP_ATTR2_TRCM_ATTRIBS_MASK   _SHR_FCMAP_ATTR2_TRCM_ATTRIBS_MASK 
#define BCM_FCMAP_ATTR2_COS_TO_PRI_MASK     _SHR_FCMAP_ATTR2_COS_TO_PRI_MASK 

#define BCM_FCMAP_ATTR_ALL_MASK _SHR_FCMAP_ATTR_ALL_MASK 

#define BCM_FCMAP_ATTR2_ALL_MASK    _SHR_FCMAP_ATTR2_ALL_MASK 

typedef bfcmap_dev_io_f bcm_fcmap_dev_io_t;

typedef _shr_bfcmap_cos_to_pri_t bcm_fcmap_cos_to_pri_t_t;

typedef _shr_bfcmap_port_config_t bcm_fcmap_port_config_t;

#ifndef BCM_HIDE_DISPATCHABLE

/* Set all FCMAP configuration for the specified port. */
extern int bcm_fcmap_port_config_set(
    int unit, 
    bcm_port_t port, 
    bcm_fcmap_port_config_t *cfg);

/* Returns all FCMAP configuration for the specified port. */
extern int bcm_fcmap_port_config_get(
    int unit, 
    bcm_port_t port, 
    bcm_fcmap_port_config_t *cfg);

/* 
 * Set FCMAP configuration for the specified port.
 * The configured port attributes are specified in the cfg->action_mask
 * and cfg->action_mask2.
 */
extern int bcm_fcmap_port_config_selective_set(
    int unit, 
    bcm_port_t port, 
    bcm_fcmap_port_config_t *cfg);

/* 
 * Returns the FCMAP configuration for the specified port.
 * The retrieved port attributes are specified in the cfg->action_mask
 * and cfg->action_mask2.
 */
extern int bcm_fcmap_port_config_selective_get(
    int unit, 
    bcm_port_t port, 
    bcm_fcmap_port_config_t *cfg);

/* Set speed attribute of the FC port. */
extern int bcm_fcmap_port_speed_set(
    int unit, 
    bcm_port_t port, 
    bcm_fcmap_port_speed_t speed);

/* 
 * Perform LINK Reset protocol on FCMAP Port, if the port is configured
 * to FC port.
 */
extern int bcm_fcmap_port_link_reset(
    int unit, 
    bcm_port_t port);

/* 
 * Perform LINK down protocol on FCMAP Port, if the port is configured to
 * FC port.
 */
extern int bcm_fcmap_port_shutdown(
    int unit, 
    bcm_port_t port);

/* Enable the FC port. */
extern int bcm_fcmap_port_enable(
    int unit, 
    bcm_port_t port);

#endif /* BCM_HIDE_DISPATCHABLE */

/*  Prototype for Port interation callback function. */
typedef int (*bcm_fcmap_port_traverse_cb)(
    int unit, 
    bcm_port_t port, 
    bcm_fcmap_core_t dev_core, 
    bcm_fcmap_dev_addr_t dev_addr, 
    int dev_port, 
    bcm_fcmap_dev_io_t devio_f, 
    void *user_data);

#ifndef BCM_HIDE_DISPATCHABLE

/* 
 * bcm_fcmap_port_traverse iterates over all the FCMAP Ports and calls
 * user-provided callback function for each port.
 */
extern int bcm_fcmap_port_traverse(
    int unit, 
    bcm_fcmap_port_traverse_cb callbk, 
    void *user_data);

#endif /* BCM_HIDE_DISPATCHABLE */

typedef _shr_bfcmap_vlan_vsan_map_t bcm_fcmap_vlan_vsan_map_t;

#ifndef BCM_HIDE_DISPATCHABLE

/* Add the VLAN to VSAN mapping on the specified port */
extern int bcm_fcmap_vlan_map_add(
    int unit, 
    bcm_port_t port, 
    bcm_fcmap_vlan_vsan_map_t *vlan);

/* Get the VLAN to VSAN mapping on the specified port */
extern int bcm_fcmap_vlan_map_get(
    int unit, 
    bcm_port_t port, 
    bcm_fcmap_vlan_vsan_map_t *vlan);

/* Delete the VLAN to VSAN mapping on the specified port */
extern int bcm_fcmap_vlan_map_delete(
    int unit, 
    bcm_port_t port, 
    bcm_fcmap_vlan_vsan_map_t *vlan);

#endif /* BCM_HIDE_DISPATCHABLE */

typedef enum bcm_fcmap_event_e {
    BCM_FCMAP_EVENT_FC_LINK_INIT = _SHR_BFCMAP_EVENT_FC_LINK_INIT, 
    BCM_FCMAP_EVENT_FC_LINK_RESET = _SHR_BFCMAP_EVENT_FC_LINK_RESET, 
    BCM_FCMAP_EVENT_FC_LINK_DOWN = _SHR_BFCMAP_EVENT_FC_LINK_DOWN, 
    BCM_FCMAP_EVENT_FC_R_T_TIMEOUT = _SHR_BFCMAP_EVENT_FC_R_T_TIMEOUT, 
    BCM_FCMAP_EVENT_FC_E_D_TIMEOUT = _SHR_BFCMAP_EVENT_FC_E_D_TIMEOUT 
} bcm_fcmap_event_t;

/*  Prototype for the event callback function. */
typedef int (*bcm_fcmap_event_cb)(
    int unit, 
    bcm_port_t port, 
    bcm_fcmap_event_t event, 
    void *user_data);

#ifndef BCM_HIDE_DISPATCHABLE

/* Register application event handler function */
extern int bcm_fcmap_event_register(
    int unit, 
    bcm_fcmap_event_cb cb, 
    void *user_data);

/* Unregister the application event handler function. */
extern int bcm_fcmap_event_unregister(
    int unit, 
    bcm_fcmap_event_cb cb);

/* Enable or disable event notification for the specified event. */
extern int bcm_fcmap_event_enable_set(
    int unit, 
    bcm_fcmap_event_t t, 
    int enable);

/* Get current event notification status for the specified event. */
extern int bcm_fcmap_event_enable_get(
    int unit, 
    bcm_fcmap_event_t t, 
    int *enable);

#endif /* BCM_HIDE_DISPATCHABLE */

typedef enum bcm_fcmap_stat_e {
    bcm_fc_rxdebug0 = _shr_fc_rxdebug0, 
    bcm_fc_rxdebug1 = _shr_fc_rxdebug1, 
    bcm_fc_rxunicastpkts = _shr_fc_rxunicastpkts, 
    bcm_fc_rxgoodframes = _shr_fc_rxgoodframes, 
    bcm_fc_rxbcastpkts = _shr_fc_rxbcastpkts, 
    bcm_fc_rxbbcredit0 = _shr_fc_rxbbcredit0, 
    bcm_fc_rxinvalidcrc = _shr_fc_rxinvalidcrc, 
    bcm_fc_rxframetoolong = _shr_fc_rxframetoolong, 
    bcm_fc_rxtruncframes = _shr_fc_rxtruncframes, 
    bcm_fc_rxdelimitererr = _shr_fc_rxdelimitererr, 
    bcm_fc_rxothererr = _shr_fc_rxothererr, 
    bcm_fc_rxruntframes = _shr_fc_rxruntframes, 
    bcm_fc_rxlipcount = _shr_fc_rxlipcount, 
    bcm_fc_rxnoscount = _shr_fc_rxnoscount, 
    bcm_fc_rxerrframes = _shr_fc_rxerrframes, 
    bcm_fc_rxdropframes = _shr_fc_rxdropframes, 
    bcm_fc_rxlinkfail = _shr_fc_rxlinkfail, 
    bcm_fc_rxlosssync = _shr_fc_rxlosssync, 
    bcm_fc_rxlosssig = _shr_fc_rxlosssig, 
    bcm_fc_rxprimseqerr = _shr_fc_rxprimseqerr, 
    bcm_fc_rxinvalidword = _shr_fc_rxinvalidword, 
    bcm_fc_rxinvalidset = _shr_fc_rxinvalidset, 
    bcm_fc_rxencodedisparity = _shr_fc_rxencodedisparity, 
    bcm_fc_rxbyt = _shr_fc_rxbyt, 
    bcm_fc_txdebug0 = _shr_fc_txdebug0, 
    bcm_fc_txdebug1 = _shr_fc_txdebug1, 
    bcm_fc_txunicastpkts = _shr_fc_txunicastpkts, 
    bcm_fc_txbcastpkts = _shr_fc_txbcastpkts, 
    bcm_fc_txbbcredit0 = _shr_fc_txbbcredit0, 
    bcm_fc_txgoodframes = _shr_fc_txgoodframes, 
    bcm_fc_txfifounderrun = _shr_fc_txfifounderrun, 
    bcm_fc_txdropframes = _shr_fc_txdropframes, 
    bcm_fc_txbyt = _shr_fc_txbyt, 
    bcm_fc_class2_rxgoodframes = _shr_fc_class2_rxgoodframes, 
    bcm_fc_class2_rxinvalidcrc = _shr_fc_class2_rxinvalidcrc, 
    bcm_fc_class2_rxframetoolong = _shr_fc_class2_rxframetoolong, 
    bcm_fc_class3_rxgoodframes = _shr_fc_class3_rxgoodframes, 
    bcm_fc_class3_rxinvalidcrc = _shr_fc_class3_rxinvalidcrc, 
    bcm_fc_class3_rxframetoolong = _shr_fc_class3_rxframetoolong, 
    bcm_fc_classf_rxgoodframes = _shr_fc_classf_rxgoodframes, 
    bcm_fc_classf_rxinvalidcrc = _shr_fc_classf_rxinvalidcrc, 
    bcm_fc_classf_rxframetoolong = _shr_fc_classf_rxframetoolong, 
    bcm_fc_rxbbc0drop = _shr_fc_rxbbc0drop, 
    bcm_fc_rxsyncfail = _shr_fc_rxsyncfail, 
    bcm_fc_rxbadxword = _shr_fc_rxbadxword, 
    bcm_fc_class2_rxruntframes = _shr_fc_class2_rxruntframes, 
    bcm_fc_class3_rxruntframes = _shr_fc_class3_rxruntframes, 
    bcm_fc_classf_rxruntframes = _shr_fc_classf_rxruntframes, 
    bcm_fc_class2_rxbyt = _shr_fc_class2_rxbyt, 
    bcm_fc_class3_rxbyt = _shr_fc_class3_rxbyt, 
    bcm_fc_classf_rxbyt = _shr_fc_classf_rxbyt, 
    bcm_fc_class2_txframes = _shr_fc_class2_txframes, 
    bcm_fc_class3_txframes = _shr_fc_class3_txframes, 
    bcm_fc_classf_txframes = _shr_fc_classf_txframes, 
    bcm_fc_txframes = _shr_fc_txframes, 
    bcm_fc_class2_txoversized_frames = _shr_fc_class2_txoversized_frames, 
    bcm_fc_class3_txoversized_frames = _shr_fc_class3_txoversized_frames, 
    bcm_fc_classf_txoversized_frames = _shr_fc_classf_txoversized_frames, 
    bcm_fc_txoversized_frames = _shr_fc_txoversized_frames, 
    bcm_fc_class2_txbyt = _shr_fc_class2_txbyt, 
    bcm_fc_class3_txbyt = _shr_fc_class3_txbyt, 
    bcm_fc_classf_txbyt = _shr_fc_classf_txbyt, 
    bcm_fcmap_stat__count = _shr_bfcmap_stat__count 
} bcm_fcmap_stat_t;

#ifndef BCM_HIDE_DISPATCHABLE

/* Clear all the statistics for the specified port. */
extern int bcm_fcmap_stat_clear(
    int unit, 
    bcm_port_t port);

/* Return the statistics for the specified statistic type. */
extern int bcm_fcmap_stat_get(
    int unit, 
    bcm_port_t port, 
    bcm_fcmap_stat_t stat, 
    uint64 *val);

/*  Return the current statistics for the specified counter. */
extern int bcm_fcmap_stat_get32(
    int unit, 
    bcm_port_t port, 
    bcm_fcmap_stat_t stat, 
    uint32 *val);

/* 
 * Returns the linkfault trigger code and reason code from the specified
 * FC port.
 */
extern int bcm_fcmap_linkfault_trigger_rc_get(
    int unit, 
    bcm_port_t port, 
    bcm_fcmap_lf_tr_t *trigger, 
    bcm_fcmap_lf_rc_t *rc);

/* Returns the current diagnostic code from the specified FC port. */
extern int bcm_fcmap_diag_get(
    int unit, 
    bcm_port_t port, 
    bcm_fcmap_diag_code_t *diag);

#endif /* BCM_HIDE_DISPATCHABLE */

/* Port ability */
typedef _shr_bfcmap_port_ability_t bcm_fcmap_port_ability_t;

#define BCM_FCMAP_PORT_ABILITY_SPEED_AN     _SHR_FCMAP_PORT_ABILITY_SPEED_AN 
#define BCM_FCMAP_PORT_ABILITY_SPEED_2GBPS  _SHR_FCMAP_PORT_ABILITY_SPEED_2GBPS 
#define BCM_FCMAP_PORT_ABILITY_SPEED_4GBPS  _SHR_FCMAP_PORT_ABILITY_SPEED_4GBPS 
#define BCM_FCMAP_PORT_ABILITY_SPEED_8GBPS  _SHR_FCMAP_PORT_ABILITY_SPEED_8GBPS 
#define BCM_FCMAP_PORT_ABILITY_SPEED_16GBPS _SHR_FCMAP_PORT_ABILITY_SPEED_16GBPS 
#define BCM_FCMAP_PORT_ABILITY_SPEED_32GBPS _SHR_FCMAP_PORT_ABILITY_SPEED_32GBPS 

#ifndef BCM_HIDE_DISPATCHABLE

/* 
 * Sets the discrete speed values for auto-neg or single forced speed for
 * the specified FC port.
 */
extern int bcm_fcmap_port_ability_advert_set(
    int unit, 
    bcm_port_t port, 
    bcm_fcmap_port_ability_t *ability_mask);

/* Returns the supported port speeds for the specified FC port. */
extern int bcm_fcmap_port_ability_advert_get(
    int unit, 
    bcm_port_t port, 
    bcm_fcmap_port_ability_t *ability_mask);

#endif /* BCM_HIDE_DISPATCHABLE */

#define BCM_FCMAP_PORT_SCRAMBLING_SPEED_2GBPS _SHR_FCMAP_PORT_SCRAMBLING_SPEED_2GBPS 
#define BCM_FCMAP_PORT_SCRAMBLING_SPEED_4GBPS _SHR_FCMAP_PORT_SCRAMBLING_SPEED_4GBPS 
#define BCM_FCMAP_PORT_SCRAMBLING_SPEED_8GBPS _SHR_FCMAP_PORT_SCRAMBLING_SPEED_8GBPS 
#define BCM_FCMAP_PORT_SCRAMBLING_SPEED_16GBPS _SHR_FCMAP_PORT_SCRAMBLING_SPEED_16GBPS 
#define BCM_FCMAP_PORT_SCRAMBLING_SPEED_32GBPS _SHR_FCMAP_PORT_SCRAMBLING_SPEED_32GBPS 

#define BCM_FCMAP_PORT_MODULE_NO_FW_I2C     _SHR_FCMAP_PORT_MODULE_NO_FW_I2C 
#define BCM_FCMAP_PORT_MODULE_IS_COPPER     _SHR_FCMAP_PORT_MODULE_IS_COPPER 
#define BCM_FCMAP_PORT_MODULE_SPEED_CAP32G  _SHR_FCMAP_PORT_MODULE_SPEED_CAP32G 
#define BCM_FCMAP_PORT_MODULE_SPEED_CAP16G  _SHR_FCMAP_PORT_MODULE_SPEED_CAP16G 
#define BCM_FCMAP_PORT_MODULE_SPEED_CAP8G   _SHR_FCMAP_PORT_MODULE_SPEED_CAP8G 
#define BCM_FCMAP_PORT_MODULE_SPEED_CAP4G   _SHR_FCMAP_PORT_MODULE_SPEED_CAP4G 
#define BCM_FCMAP_PORT_MODULE_FEC_CAP32G    _SHR_FCMAP_PORT_MODULE_FEC_CAP32G 
#define BCM_FCMAP_PORT_MODULE_FEC_CAP16G    _SHR_FCMAP_PORT_MODULE_FEC_CAP16G 
#define BCM_FCMAP_PORT_MODULE_IS_QSFP       _SHR_FCMAP_PORT_MODULE_IS_QSFP 
#define BCM_FCMAP_PORT_MODULE_SUPP_RS       _SHR_FCMAP_PORT_MODULE_SUPP_RS 
#define BCM_FCMAP_PORT_MODULE_RS_HRD_RX_CUTOFF _SHR_FCMAP_PORT_MODULE_RS_HRD_RX_CUTOFF 
#define BCM_FCMAP_PORT_MODULE_RS_HRD_TX_CUTOFF _SHR_FCMAP_PORT_MODULE_RS_HRD_TX_CUTOFF 

#endif /* defined(INCLUDE_FCMAP) */

#endif /* __BCM_FCMAP_H__ */
