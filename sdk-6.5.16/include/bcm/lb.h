/*
 * 
 * 
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 * 
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated.
 * Edits to this file will be lost when it is regenerated.
 */

#ifndef __BCM_LB_H__
#define __BCM_LB_H__

#include <bcm/types.h>

/* Link bonding module flags. */
#define BCM_LB_FLAG_GLOBAL                  (1 << 0)   /* Global configuration. */
#define BCM_LB_FLAG_UNMAP                   (1 << 1)   /* Unmap operation. */
#define BCM_LB_FLAG_FLUSH_GLOBAL            (1 << 2)   /* flush for all lbg. */
#define BCM_LB_FLAG_MODEM_SHAPER_SEGMENT    (1 << 3)   /* set modem segment
                                                          shaper. */

/* LBG ID type */
typedef uint32 bcm_lbg_t;

/* MODEM ID type */
typedef uint32 bcm_modem_t;

/* Link bonding direction type eumerations */
typedef enum bcm_lb_direction_type_e {
    bcmLbDirectionRx = 0,   /* Link bonding in receive side */
    bcmLbDirectionTx = 1,   /* Link bonding in transmit side */
    bcmLbDirectionBoth = 2, /* Link bonding in both receive and transmit side */
    bcmLbDirectionCount = 3 /* Always Last. Not a usable value. */
} bcm_lb_direction_type_t;

/* Link bonding control type */
typedef enum bcm_lb_control_e {
    bcmLbControlSegmentationMode = 0,   /* Global configuration in TX side */
    bcmLbControlPacketCrcEnable = 1,    /* Global configuration in TX side */
    bcmLbControlTimeoutThreshold = 2,   /* Per LBG configuration in RX side */
    bcmLbControlMaxOutOfOrder = 3,      /* Per LBG configuration in RX side */
    bcmLbControlMaxBuffer = 4,          /* Per LBG configuration in RX side */
    bcmLbControlExpectedSequenceNumber = 5, /* Per LBG configuration in RX side */
    bcmLbControlNofTotalSharedBuffers = 6, /* Total shared buffers */
    bcmLbControlNofGuaranteedBuffers = 7, /* Guaranteed buffers the link bonding
                                           group can acquire */
    bcmLbControlCount = 8               /* Always Last. Not a usable value. */
} bcm_lb_control_t;

/* Link bonding modem control type */
typedef enum bcm_lb_modem_control_e {
    bcmLbModemControlHeaderCompensation = 0, /* Modem configuration in TX side */
    bcmLbModemControlCount = 1          /* Always Last. Not a usable value. */
} bcm_lb_modem_control_t;

/* Link bonding format type */
typedef enum bcm_lb_format_type_e {
    bcmLbFormatTypeBypass = 0,          /* LB bypss */
    bcmLbFormatTypeNonChannelize = 1,   /* LB non channelize */
    bcmLbFormatTypechannelize = 2,      /* LB channelize */
    bcmLbFormatCount = 3                /* Always Last. Not a usable value. */
} bcm_lb_format_type_t;

/* Link bonding status */
typedef enum bcm_lb_status_type_e {
    bcmLbStatusIsInSync = 0,    /* Is expected sequence number in sync state */
    bcmLbStatusIsEmptyFifo = 1, /* LBG FIFOs empty indication */
    bcmLbStatusCount = 2        /* Always Last. Not a usable value. */
} bcm_lb_status_type_t;

/* Link bonding Modem status */
typedef enum bcm_lb_modem_status_type_e {
    bcmLbModemStatusIsEmptyFifo = 0,    /* Modem FIFO empty indication */
    bcmLbModemStatusCount = 1           /* Always Last. Not a usable value. */
} bcm_lb_modem_status_type_t;

/* Link bonding stat */
typedef enum bcm_lb_stat_val_e {
    bcmLbStatsUnicastPkts = 0,          /* Unicast packets */
    bcmLbStatsMulticastPkts = 1,        /* Multicast packets */
    bcmLbStatsBroadcastPkts = 2,        /* Broadcast packets */
    bcmLbStatsPkts64Octets = 3, 
    bcmLbStatsPkts65to127Octets = 4, 
    bcmLbStatsPkts128to255Octets = 5, 
    bcmLbStatsPkts256to511Octets = 6, 
    bcmLbStatsPkts512to1023Octets = 7, 
    bcmLbStatsPkts1024to1518Octets = 8, 
    bcmLbStatsPkts1519Octets = 9, 
    bcmLbStatsPkts = 10, 
    bcmLbStatsOctets = 11, 
    bcmLbStatsDiscardFragments = 12, 
    bcmLbStatsErrorPkts = 13, 
    bcmLbStatsCount = 14                /* Always Last. Not a usable value. */
} bcm_lb_stat_val_t;

/* Link bonding Modem stat */
typedef enum bcm_lb_modem_stat_val_e {
    bcmLbModemStatsPkts = 0, 
    bcmLbModemStatsOctets = 1, 
    bcmLbModemStatsCount = 2    /* Always Last. Not a usable value. */
} bcm_lb_modem_stat_val_t;

/* Link bonding Segment Mode */
typedef enum bcm_lb_segment_mode_e {
    bcmLbSegmentMode128 = 0, 
    bcmLbSegmentMode192 = 1, 
    bcmLbSegmentMode192Enhanced = 2, 
    bcmLbSegmentMode128Enhanced = 3, 
    bcmLbSegmentModeCount = 4           /* Always Last. Not a usable value. */
} bcm_lb_segment_mode_t;

/* Link bonding flush operation type */
typedef enum bcm_lb_flush_type_e {
    bcmLbFlushTypeForceFlush = 0,       /* For debug only. Should not be used in
                                           normal operation */
    bcmLbFlushTypeFlushOnGroupOverflow = 1, 
    bcmLbFlushTypeFlushOnFifoOverflow = 2, 
    bcmLbFlushTypeFlushOnSharedOverflow = 3, 
    bcmLbFlushTypeCount = 4             /* Always Last. Not a usable value. */
} bcm_lb_flush_type_t;

/* Index of mapping traffic to modem */
typedef struct bcm_lb_rx_modem_map_index_s {
    bcm_gport_t port; 
    bcm_vlan_t vlan; 
} bcm_lb_rx_modem_map_index_t;

/* Modem info */
typedef struct bcm_lb_rx_modem_map_config_s {
    bcm_lb_format_type_t lbg_type; 
    bcm_modem_t modem_id; 
} bcm_lb_rx_modem_map_config_t;

/* packet info */
typedef struct bcm_lb_packet_config_s {
    uint16 outer_vlan_tpid; 
    uint16 lbg_tpid; 
} bcm_lb_packet_config_t;

/* packet info */
typedef struct bcm_lb_modem_packet_config_s {
    bcm_lb_format_type_t pkt_format; 
    bcm_mac_t dst_mac; 
    bcm_mac_t src_mac; 
    uint16 vlan; 
    int use_global_priority_map; 
} bcm_lb_modem_packet_config_t;

/* tc dp info */
typedef struct bcm_lb_tc_dp_s {
    uint32 tc; 
    uint32 dp; 
} bcm_lb_tc_dp_t;

/* packet priority info */
typedef struct bcm_lb_pkt_pri_s {
    uint32 pkt_pri; 
    uint32 pkt_cfi; 
    uint32 pkt_dp; 
} bcm_lb_pkt_pri_t;

/* LBG weight info */
typedef struct bcm_lb_lbg_weight_s {
    bcm_lbg_t lbg_id; 
    int lbg_weight; 
} bcm_lb_lbg_weight_t;

/* packet priority info */
typedef struct bcm_lb_modem_shaper_config_s {
    uint32 enable; 
    uint32 rate; 
    uint32 max_burst; 
} bcm_lb_modem_shaper_config_t;

/* Initialize structure bcm_lb_rx_modem_map_index_t */
extern void bcm_lb_rx_modem_map_index_t_init(
    bcm_lb_rx_modem_map_index_t *map_index);

/* Initialize structure bcm_lb_rx_modem_map_config_t */
extern void bcm_lb_rx_modem_map_config_t_init(
    bcm_lb_rx_modem_map_config_t *map_config);

/* Initialize structure bcm_lb_packet_config_t */
extern void bcm_lb_packet_config_t_init(
    bcm_lb_packet_config_t *packet_config);

/* Initialize structure bcm_lb_modem_packet_config_t */
extern void bcm_lb_modem_packet_config_t_init(
    bcm_lb_modem_packet_config_t *packet_config);

/* Initialize structure bcm_lb_tc_dp_t */
extern void bcm_lb_tc_dp_t_init(
    bcm_lb_tc_dp_t *tc_dp);

/* Initialize structure bcm_lb_pkt_pri_t */
extern void bcm_lb_pkt_pri_t_init(
    bcm_lb_pkt_pri_t *pkt_pri);

/* Initialize structure bcm_lb_lbg_weight_t */
extern void bcm_lb_lbg_weight_t_init(
    bcm_lb_lbg_weight_t *lbg_weights);

/* Initialize structure bcm_lb_modem_shaper_config_t */
extern void bcm_lb_modem_shaper_config_t_init(
    bcm_lb_modem_shaper_config_t *shaper);

#ifndef BCM_HIDE_DISPATCHABLE

/* Connect Modem to logic port */
extern int bcm_lb_modem_to_port_map_set(
    int unit, 
    bcm_modem_t modem_id, 
    uint32 flags, 
    bcm_gport_t port);

/* Connect Modem to logic port */
extern int bcm_lb_modem_to_port_map_get(
    int unit, 
    bcm_modem_t modem_id, 
    uint32 flags, 
    bcm_gport_t *port);

/* Enable or disable LBG */
extern int bcm_lb_enable_set(
    int unit, 
    bcm_lbg_t lbg_id, 
    uint32 flags, 
    uint32 enable);

/* Enable or disable LBG */
extern int bcm_lb_enable_get(
    int unit, 
    bcm_lbg_t lbg_id, 
    uint32 flags, 
    uint32 *enable);

/* Connect Modem to LBG */
extern int bcm_lb_modem_to_lbg_map_set(
    int unit, 
    bcm_modem_t modem_id, 
    bcm_lb_direction_type_t direction, 
    uint32 flags, 
    bcm_lbg_t lbg_id);

/* Connect Modem to LBG */
extern int bcm_lb_modem_to_lbg_map_get(
    int unit, 
    bcm_modem_t modem_id, 
    bcm_lb_direction_type_t direction, 
    uint32 flags, 
    bcm_lbg_t *lbg_id);

/* Set sequence number width */
extern int bcm_lb_sequence_number_width_set(
    int unit, 
    bcm_lbg_t lbg_id, 
    bcm_lb_direction_type_t direction, 
    uint32 flags, 
    int number_width);

/* Get sequence number width */
extern int bcm_lb_sequence_number_width_get(
    int unit, 
    bcm_lbg_t lbg_id, 
    bcm_lb_direction_type_t direction, 
    uint32 flags, 
    int *number_width);

/* Set configuration */
extern int bcm_lb_control_set(
    int unit, 
    bcm_lbg_t lbg_id, 
    bcm_lb_direction_type_t direction, 
    uint32 flags, 
    bcm_lb_control_t lb_control, 
    int arg);

/* Set configuration */
extern int bcm_lb_control_get(
    int unit, 
    bcm_lbg_t lbg_id, 
    bcm_lb_direction_type_t direction, 
    uint32 flags, 
    bcm_lb_control_t lb_control, 
    int *arg);

/* Set configuration */
extern int bcm_lb_modem_control_set(
    int unit, 
    bcm_modem_t modem_id, 
    bcm_lb_direction_type_t direction, 
    uint32 flags, 
    bcm_lb_modem_control_t modem_control, 
    int arg);

/* Get configuration */
extern int bcm_lb_modem_control_get(
    int unit, 
    bcm_modem_t modem_id, 
    bcm_lb_direction_type_t direction, 
    uint32 flags, 
    bcm_lb_modem_control_t modem_control, 
    int *arg);

/* Get status of LBG or global */
extern int bcm_lb_status_get(
    int unit, 
    bcm_lbg_t lbg_id, 
    bcm_lb_direction_type_t direction, 
    uint32 flags, 
    bcm_lb_status_type_t status_type, 
    int *value);

/* Get status of LBG or global */
extern int bcm_lb_modem_status_get(
    int unit, 
    bcm_modem_t modem_id, 
    bcm_lb_direction_type_t direction, 
    uint32 flags, 
    bcm_lb_modem_status_type_t status_type, 
    int *value);

/* Get stat of LBG or global */
extern int bcm_lb_stat_get(
    int unit, 
    bcm_lbg_t lbg_id, 
    bcm_lb_direction_type_t direction, 
    uint32 flags, 
    bcm_lb_stat_val_t type, 
    uint64 *value);

/* Get stat of Modem */
extern int bcm_lb_modem_stat_get(
    int unit, 
    bcm_modem_t modem_id, 
    bcm_lb_direction_type_t direction, 
    uint32 flags, 
    bcm_lb_modem_stat_val_t type, 
    uint64 *value);

/* Set mapping of port to modem */
extern int bcm_lb_rx_modem_map_set(
    int unit, 
    bcm_lb_rx_modem_map_index_t *map_index, 
    uint32 flags, 
    bcm_lb_rx_modem_map_config_t *map_config);

/* Get mapping of port to modem */
extern int bcm_lb_rx_modem_map_get(
    int unit, 
    bcm_lb_rx_modem_map_index_t *map_index, 
    uint32 flags, 
    bcm_lb_rx_modem_map_config_t *map_config);

/* Set packet configuration */
extern int bcm_lb_packet_config_set(
    int unit, 
    uint32 flags, 
    bcm_lb_packet_config_t *packet_config);

/* Set packet configuration */
extern int bcm_lb_packet_config_get(
    int unit, 
    uint32 flags, 
    bcm_lb_packet_config_t *packet_config);

/* Set packet configuration */
extern int bcm_lb_modem_packet_config_set(
    int unit, 
    bcm_modem_t modem_id, 
    uint32 flags, 
    bcm_lb_modem_packet_config_t *packet_config);

/* Get packet configuration */
extern int bcm_lb_modem_packet_config_get(
    int unit, 
    bcm_modem_t modem_id, 
    uint32 flags, 
    bcm_lb_modem_packet_config_t *packet_config);

/* Set mapping for tc,dp to priority */
extern int bcm_lb_tc_dp_to_priority_config_set(
    int unit, 
    uint32 flags, 
    bcm_lb_tc_dp_t *tc_dp, 
    bcm_lb_pkt_pri_t *pkt_pri);

/* Get mapping for tc,dp to priority */
extern int bcm_lb_tc_dp_to_priority_config_get(
    int unit, 
    uint32 flags, 
    bcm_lb_tc_dp_t *tc_dp, 
    bcm_lb_pkt_pri_t *pkt_pri);

/* Set scheduler between lbg */
extern int bcm_lb_tx_sched_set(
    int unit, 
    uint32 flags, 
    int lbg_count, 
    bcm_lb_lbg_weight_t *lbg_weights);

/* Set scheduler between lbg */
extern int bcm_lb_tx_sched_get(
    int unit, 
    uint32 flags, 
    int max_lbg_count, 
    bcm_lb_lbg_weight_t *lbg_weights, 
    int *lbg_count);

/* Set modem shaper */
extern int bcm_lb_modem_shaper_set(
    int unit, 
    bcm_modem_t modem_id, 
    uint32 flags, 
    bcm_lb_modem_shaper_config_t *shaper);

/* Get modem shaper */
extern int bcm_lb_modem_shaper_get(
    int unit, 
    bcm_modem_t modem_id, 
    uint32 flags, 
    bcm_lb_modem_shaper_config_t *shaper);

/* Set flush operation */
extern int bcm_lb_flush_configure_set(
    int unit, 
    bcm_lbg_t lbg_id, 
    uint32 flags, 
    bcm_lb_flush_type_t type, 
    int arg);

/* Get configuration */
extern int bcm_lb_flush_configure_get(
    int unit, 
    bcm_lbg_t lbg_id, 
    uint32 flags, 
    bcm_lb_flush_type_t type, 
    int *arg);

#endif /* BCM_HIDE_DISPATCHABLE */

#endif /* __BCM_LB_H__ */
