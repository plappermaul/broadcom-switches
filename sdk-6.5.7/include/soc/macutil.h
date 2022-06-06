/*
 * $Id: macutil.h,v 1.7 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 */

#ifndef _SOC_MAC_UTIL_H
#define _SOC_MAC_UTIL_H

#define spn_LINK_DELAY(s) ((100000 == (s)) ? spn_LINK_DELAY_100GBE_NS : \
                           (40000 == (s)) ?  spn_LINK_DELAY_40GBE_NS:   \
                           (10000 == (s)) ?  spn_LINK_DELAY_10GBE_NS:   \
                           (2500  == (s)) ?  spn_LINK_DELAY_2_5GBE_NS:  \
                           (1000  == (s)) ?  spn_LINK_DELAY_1GBE_NS:    \
                           (100   == (s)) ?  spn_LINK_DELAY_100MBE_NS:  \
                           (10    == (s)) ?  spn_LINK_DELAY_10MBE_NS:   \
                           spn_LINK_DELAY_NS)

#define spn_TIMESTAMP_ADJUST(s) ((100000 == (s)) ? spn_TIMESTAMP_ADJUST_100GBE_NS : \
                                 (40000 == (s)) ?  spn_TIMESTAMP_ADJUST_40GBE_NS: \
                                 (10000 == (s)) ?  spn_TIMESTAMP_ADJUST_10GBE_NS: \
                                 (2500  == (s)) ?  spn_TIMESTAMP_ADJUST_2_5GBE_NS: \
                                 (1000  == (s)) ?  spn_TIMESTAMP_ADJUST_1GBE_NS: \
                                 (100   == (s)) ?  spn_TIMESTAMP_ADJUST_100MBE_NS: \
                                 (10    == (s)) ?  spn_TIMESTAMP_ADJUST_10MBE_NS: \
                                 spn_TIMESTAMP_ADJUST_NS)

extern int
soc_egress_cell_count(int unit, soc_port_t port, uint32 *count);

extern int
soc_mmu_flush_enable(int unit, soc_port_t port, int enable);

extern int
soc_mmu_port_flush_hw(int unit, soc_port_t port, uint32 drain_timeout);

extern int
soc_egress_drain_cells(int unit, soc_port_t port, uint32 drain_timeout);

extern int
soc_txfifo_drain_cells(int unit, soc_port_t port, uint32 drain_timeout);

extern int
soc_port_credit_reset(int unit, soc_port_t port);

extern int
soc_port_ingress_buffer_reset(int unit, soc_port_t port, int reset);

extern int
soc_port_egress_buffer_sft_reset(int unit, soc_port_t port, int reset);

extern int
soc_port_fifo_reset(int unit, soc_port_t port);

extern int
soc_port_blk_init(int unit, soc_port_t port);

extern int
soc_egress_enable(int unit, soc_port_t port, int enable);

extern int 
soc_packet_purge_control_init(int unit, soc_port_t port);

extern int
soc_port_speed_update(int unit, soc_port_t port, int speed);

extern int
soc_port_thdo_rx_enable_set(int unit, soc_port_t port, int enable);

extern int
soc_llfc_xon_set(int unit, soc_port_t port, int enable);

extern int
_soc_egress_metering_freeze(int unit, soc_port_t port, void **setting);

extern int
_soc_egress_metering_thaw(int unit, soc_port_t port, void *setting);

typedef struct mem_entry_s {
    soc_mem_t mem;
    int index;
    uint32 entry[SOC_MAX_MEM_WORDS];
} mem_entry_t;

extern int 
soc_port_eee_timers_setup(int unit, soc_port_t port, int speed);

extern int 
soc_port_eee_timers_init(int unit, soc_port_t port, int speed);

extern soc_error_t
soc_granular_speed_get(int unit, soc_port_t port, int *speed);

extern int
soc_port_hg_speed_get(int unit, soc_port_t port, int *speed);

extern int
soc_mac_x_sync_fifo_reset(int unit, soc_port_t port);

extern int
soc_pgw_rx_fifo_reset(int unit, int port, int reset);

#ifdef BCM_TRIUMPH3_SUPPORT
/* NOTE: _mac_x_drain_cells should be used  for ibodsync */
extern int
_mac_x_drain_cells(int unit, soc_port_t port, int notify_phy);

extern int
_mac_x_reinit(int unit, soc_port_t port);

extern void _soc_egress_drain_timeout_flag_set(int unit, int flag);
extern void _soc_egress_drain_timeout_flag_get(int unit, int *flag);
extern int  _soc_egress_drain_debug(int unit, soc_port_t port, soc_reg_t reg);
extern int _soc_egress_drain_credit_calc(uint32 credit_current, uint32 credit_default,
                            uint32 mmu_req, uint32 ep_level, uint32 *credit_set);
extern int _soc_egress_drain_ep_max_level_get(int unit, soc_port_t port, uint32 *ep_max_level);
#endif

#endif  /* !_SOC_MAC_UTIL_H */
