/*
 * $Id: feature.h,v 1.545 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * SOC_FEATURE definitions
 *
 * File:        feature.h
 * Purpose:     Define features by chip
 */

#ifndef _SOC_FEATURE_H
#define _SOC_FEATURE_H

#include <shared/enumgen.h>

/*
 * Defines enums of the form soc_feature_xxx, and a corresponding list
 * of initializer strings "xxx".  New additions should be made in the
 * following section between the zero and count entries.
 */

#define SHR_G_ENTRIES(DONT_CARE)                                          \
SHR_G_ENTRY(zero),              /* ALWAYS FIRST PLEASE (DO NOT CHANGE) */ \
SHR_G_ENTRY(arl_hashed),                                                  \
SHR_G_ENTRY(arl_lookup_cmd),                                              \
SHR_G_ENTRY(arl_lookup_retry),  /* All-0's response requires retry */     \
SHR_G_ENTRY(arl_insert_ovr),    /* ARL ins overwrites if key exists */    \
SHR_G_ENTRY(cfap_pool),         /* Software CFAP initialization. */       \
SHR_G_ENTRY(cos_rx_dma),        /* COS based RX DMA */                    \
SHR_G_ENTRY(dcb_type3),         /* 5690 */                                \
SHR_G_ENTRY(dcb_type4),         /* 5670 */                                \
SHR_G_ENTRY(dcb_type5),         /* 5673 */                                \
SHR_G_ENTRY(dcb_type6),         /* 5665 */                                \
SHR_G_ENTRY(dcb_type7),         /* 5695 when enabled */                   \
SHR_G_ENTRY(dcb_type8),         /* not used (5345) */                     \
SHR_G_ENTRY(dcb_type9),         /* 56504 */                               \
SHR_G_ENTRY(dcb_type10),        /* 56601 */                               \
SHR_G_ENTRY(dcb_type11),        /* 56580, 56700, 56800 */                 \
SHR_G_ENTRY(dcb_type12),        /* 56218 */                               \
SHR_G_ENTRY(dcb_type13),        /* 56514 */                               \
SHR_G_ENTRY(dcb_type14),        /* 56624, 56680 */                        \
SHR_G_ENTRY(dcb_type15),        /* 56224 A0 */                            \
SHR_G_ENTRY(dcb_type16),        /* 56820 */                               \
SHR_G_ENTRY(dcb_type17),        /* 53314 */                               \
SHR_G_ENTRY(dcb_type18),        /* 56224 B0 */                            \
SHR_G_ENTRY(dcb_type19),        /* 56634, 56524, 56685 */                 \
SHR_G_ENTRY(dcb_type20),        /* 5623x */                               \
SHR_G_ENTRY(dcb_type21),        /* 56840 */                               \
SHR_G_ENTRY(dcb_type22),        /* 88732 */                               \
SHR_G_ENTRY(dcb_type23),        /* 56640 */                               \
SHR_G_ENTRY(dcb_type24),        /* 56440 */                               \
SHR_G_ENTRY(dcb_type25),        /* 88030 */                               \
SHR_G_ENTRY(dcb_type26),        /* 56850 */                               \
SHR_G_ENTRY(dcb_type27),        /* 88230 */                               \
SHR_G_ENTRY(dcb_type28),        /* 88650 */                               \
SHR_G_ENTRY(dcb_type29),        /* 56450 */                               \
SHR_G_ENTRY(dcb_type30),        /* 56150 */                               \
SHR_G_ENTRY(dcb_type31),        /* 53400 */                               \
SHR_G_ENTRY(dcb_type32),        /* 56960 */                               \
SHR_G_ENTRY(dcb_type33),        /* 56860 */                               \
SHR_G_ENTRY(dcb_type34),        /* 56160, 53570 */                        \
SHR_G_ENTRY(dcb_type35),        /* 56560 */                               \
SHR_G_ENTRY(dcb_type37),        /* 53570 */                               \
SHR_G_ENTRY(fe_gig_macs),                                                 \
SHR_G_ENTRY(trimac),                                                      \
SHR_G_ENTRY(filter),                                                      \
SHR_G_ENTRY(filter_extended),                                             \
SHR_G_ENTRY(filter_xgs),                                                  \
SHR_G_ENTRY(filter_pktfmtext),  /* Enhanced pkt format spec for XGS */    \
SHR_G_ENTRY(filter_metering),                                             \
SHR_G_ENTRY(ingress_metering),                                            \
SHR_G_ENTRY(egress_metering),                                             \
SHR_G_ENTRY(filter_tucana),                                               \
SHR_G_ENTRY(filter_draco15),                                              \
SHR_G_ENTRY(has_gbp),                                                     \
SHR_G_ENTRY(l3),                /* Capable of Layer 3 switching.  */      \
SHR_G_ENTRY(l3_ip6),            /* IPv6 capable device. */                \
SHR_G_ENTRY(l3_lookup_cmd),                                               \
SHR_G_ENTRY(ip_mcast),                                                    \
SHR_G_ENTRY(ip_mcast_repl),     /* IPMC replication */                    \
SHR_G_ENTRY(led_proc),          /* LED microprocessor */                  \
SHR_G_ENTRY(led_data_offset_a0),/* LED RAM data offset for Link Status */ \
SHR_G_ENTRY(schmsg_alias),                                                \
SHR_G_ENTRY(stack_my_modid),                                              \
SHR_G_ENTRY(stat_dma),                                                    \
SHR_G_ENTRY(cpuport_stat_dma),                                            \
SHR_G_ENTRY(table_dma),                                                   \
SHR_G_ENTRY(tslam_dma),                                                   \
SHR_G_ENTRY(stg),               /* Generic STG */                         \
SHR_G_ENTRY(stg_xgs),           /* XGS implementation specific STG */     \
SHR_G_ENTRY(remap_ut_prio),     /* Remap prio of untagged pkts by port */ \
SHR_G_ENTRY(link_down_fd),      /* Bug in BCM5645 MAC; see mac.c */       \
SHR_G_ENTRY(phy_5690_link_war),                                           \
SHR_G_ENTRY(l3x_delete_bf_bug), /* L3 delete fails if bucket full */      \
SHR_G_ENTRY(xgxs_v1),           /* FusionCore version 1 (BCM5670/90 A0) */\
SHR_G_ENTRY(xgxs_v2),           /* FusionCore version 2 (BCM5670/90 A1) */\
SHR_G_ENTRY(xgxs_v3),           /* FusionCore version 3 (BCM5673) */      \
SHR_G_ENTRY(xgxs_v4),           /* FusionCore version 4 (BCM5674) */      \
SHR_G_ENTRY(xgxs_v5),           /* FusionCore version 5 (BCM56504/56601)*/\
SHR_G_ENTRY(xgxs_v6),           /* FusionCore version 6 (BCM56700/56800)*/\
SHR_G_ENTRY(xgxs_v7),           /* FusionCore version 7 (BCM56624/56680)*/\
SHR_G_ENTRY(lmd),               /* Lane Mux/DMux 2.5 Gbps speed support */\
SHR_G_ENTRY(bigmac_fault_stat), /* BigMac supports link fault status*/    \
SHR_G_ENTRY(phy_cl45),          /* Clause 45 phy support */               \
SHR_G_ENTRY(aging_extended),    /* extended l2 age control */             \
SHR_G_ENTRY(dmux),              /* DMUX capability */                     \
SHR_G_ENTRY(ext_gth_hd_ipg),    /* Longer half dup. IPG (BCM5690A0/A1) */ \
SHR_G_ENTRY(l2x_ins_sets_hit),  /* L2X insert hardcodes HIT bit to 1 */   \
SHR_G_ENTRY(l3x_ins_igns_hit),  /* L3X insert does not affect HIT bit */  \
SHR_G_ENTRY(fabric_debug),      /* Debug support works on 5670 types */   \
SHR_G_ENTRY(filter_krules),     /* All FFP rules usable on 5673 */        \
SHR_G_ENTRY(srcmod_filter),     /* Chip can filter on src modid */        \
SHR_G_ENTRY(dcb_st0_bug),       /* Unreliable DCB status 0 (BCM5665 A0) */\
SHR_G_ENTRY(filter_128_rules),  /* 128 FFP rules usable on 5673 */        \
SHR_G_ENTRY(modmap),            /* Module ID remap */                     \
SHR_G_ENTRY(recheck_cntrs),     /* Counter collection bug */              \
SHR_G_ENTRY(bigmac_rxcnt_bug),  /* BigMAC Rx counter bug */               \
SHR_G_ENTRY(block_ctrl_ingress),/* Must block ctrl frames in xPIC */      \
SHR_G_ENTRY(mstp_mask),         /* VLAN mask for MSTP drops */            \
SHR_G_ENTRY(mstp_lookup),       /* VLAN lookup for MSTP drops */          \
SHR_G_ENTRY(mstp_uipmc),        /* MSTP for UIPMC */                      \
SHR_G_ENTRY(ipmc_lookup),       /* IPMC echo suppression */               \
SHR_G_ENTRY(lynx_l3_expanded),  /* Lynx 1.5 with expanded L3 tables */    \
SHR_G_ENTRY(fast_rate_limit),   /* Short pulse rate limits (Lynx !A0) */  \
SHR_G_ENTRY(l3_sgv),            /* (S,G,V) hash key in L3 table */        \
SHR_G_ENTRY(l3_sgv_aisb_hash),  /* (S,G,V) variant hash */                \
SHR_G_ENTRY(l3_dynamic_ecmp_group),/* Ecmp group use base and count  */   \
SHR_G_ENTRY(l3_host_ecmp_group),   /* Ecmp group for Host Entries */   \
SHR_G_ENTRY(l2_hashed),                                                   \
SHR_G_ENTRY(l2_lookup_cmd),                                               \
SHR_G_ENTRY(l2_lookup_retry),   /* All-0's response requires retry */     \
SHR_G_ENTRY(l2_user_table),     /* Implements L2 User table */            \
SHR_G_ENTRY(rsv_mask),          /* RSV_MASK needs 10/100 mode adjust */   \
SHR_G_ENTRY(schan_hw_timeout),  /* H/W can indicate SCHAN timeout    */   \
SHR_G_ENTRY(lpm_tcam),          /* LPM table is a TCAM               */   \
SHR_G_ENTRY(mdio_enhanced),     /* MDIO address remapping + C45/C22  */   \
SHR_G_ENTRY(mem_cmd),           /* Memory commands supported  */          \
SHR_G_ENTRY(two_ingress_pipes), /* Has two ingress pipelines           */ \
SHR_G_ENTRY(multi_pipe_mapped_ports), /* Has multiple pipes and ports mapped per pipe */ \
SHR_G_ENTRY(mpls),              /* MPLS for XGS3 */                       \
SHR_G_ENTRY(xgxs_lcpll),        /* LCPLL Clock for XGXS */                \
SHR_G_ENTRY(dodeca_serdes),     /* Dodeca Serdes */                       \
SHR_G_ENTRY(txdma_purge),       /* TX Purge packet after DMA abort */     \
SHR_G_ENTRY(rxdma_cleanup),     /* Check RX Packet SOP after DMA abort */ \
SHR_G_ENTRY(pkt_tx_align),      /* Fix Tx alignment for 128 byte burst */ \
SHR_G_ENTRY(status_link_fail),  /* H/W linkcsan gives Link fail status */ \
SHR_G_ENTRY(fe_maxframe),       /* FE MAC MAXFR is max frame len + 1   */ \
SHR_G_ENTRY(l2x_parity),        /* Parity support on L2X table         */ \
SHR_G_ENTRY(l3x_parity),        /* Parity support on L3 tables         */ \
SHR_G_ENTRY(l3defip_parity),    /* Parity support on L3 DEFIP tables   */ \
SHR_G_ENTRY(l3_defip_map),      /* Map out unused L3_DEFIP blocks      */ \
SHR_G_ENTRY(l2_modfifo),        /* L2 Modfifo for L2 table sync        */ \
SHR_G_ENTRY(l2_multiple),       /* Multiple L2 tables implemented      */ \
SHR_G_ENTRY(l2_overflow),       /* L2 overflow mechanism implemented   */ \
SHR_G_ENTRY(l2_overflow_bucket), /* L2 overflow table implemented   */ \
SHR_G_ENTRY(vlan_mc_flood_ctrl),/* VLAN multicast flood control (PFM)  */ \
SHR_G_ENTRY(vfi_mc_flood_ctrl), /* VFI multicast flood control (PFM)   */ \
SHR_G_ENTRY(vlan_translation),  /* VLAN translation feature            */ \
SHR_G_ENTRY(egr_vlan_pfm),      /* MH PFM control per VLAN FB A0       */ \
SHR_G_ENTRY(parity_err_tocpu),  /* Packet to CPU on lookup parity error*/ \
SHR_G_ENTRY(nip_l3_err_tocpu),  /* Packet to CPU on non-IP L3 error    */ \
SHR_G_ENTRY(l3mtu_fail_tocpu),  /* Packet to CPU on L3 MTU check fail  */ \
SHR_G_ENTRY(meter_adjust),      /* Packet length adjustment for meter  */ \
SHR_G_ENTRY(prime_ctr_writes),  /* Prepare counter writes with read    */ \
SHR_G_ENTRY(xgxs_power),        /* Power up XGSS                       */ \
SHR_G_ENTRY(src_modid_blk),     /* SRC_MODID_BLOCK table support       */ \
SHR_G_ENTRY(src_modid_blk_ucast_override),/* MODID_BLOCK ucast overide */ \
SHR_G_ENTRY(src_modid_blk_opcode_override),/* MODID_BLOCK opcode overide */ \
SHR_G_ENTRY(src_modid_blk_profile), /* SRC_MODID_* block profile table */ \
SHR_G_ENTRY(egress_blk_ucast_override),  /* egr mask opcode overide    */ \
SHR_G_ENTRY(stat_jumbo_adj),    /* Adjustable stats *OVR threshold     */ \
SHR_G_ENTRY(stat_xgs3),         /* XGS3 dbg counters and IPv6 supports */ \
SHR_G_ENTRY(dbgc_higig_lkup),   /* FB B0 dbg counters w/ HiGig lookup  */ \
SHR_G_ENTRY(port_trunk_index),  /* FB B0 trunk programmable hash       */ \
SHR_G_ENTRY(port_flow_hash),    /* HB/GW enhanced hashing              */ \
SHR_G_ENTRY(seer_bcam_tune),    /* Retune SEER BCAM table              */ \
SHR_G_ENTRY(mcu_fifo_suppress), /* MCU fifo error suppression          */ \
SHR_G_ENTRY(cpuport_switched),  /* CPU port can act as switch port     */ \
SHR_G_ENTRY(cpuport_mirror),    /* CPU port can be mirrored            */ \
SHR_G_ENTRY(higig2),            /* Higig2 support                      */ \
SHR_G_ENTRY(color),             /* Select color from priority/CFI      */ \
SHR_G_ENTRY(color_inner_cfi),   /* Select color from inner tag CFI     */ \
SHR_G_ENTRY(color_prio_map),    /* Map color/int_prio from/to prio/CFI */ \
SHR_G_ENTRY(untagged_vt_miss),  /* Drop untagged VLAN xlate miss       */ \
SHR_G_ENTRY(module_loopback),   /* Allow local module ingress          */ \
SHR_G_ENTRY(dscp_map_per_port), /* Per Port DSCP mapping table support */ \
SHR_G_ENTRY(egr_dscp_map_per_port), /* Per Egress Port DSCP mapping table support */ \
SHR_G_ENTRY(dscp_map_mode_all), /* DSCP map mode ALL or NONE support   */ \
SHR_G_ENTRY(vp_dscp_map),       /* VP DSCP map set and get support     */ \
SHR_G_ENTRY(multi_tunnel_label_count),  /* Multi tunnel label counting */ \
SHR_G_ENTRY(oam_split_counter_pools),    /* Split counter pools for oam*/ \
SHR_G_ENTRY(l3defip_bound_adj), /* L3 defip boundary adjustment        */ \
SHR_G_ENTRY(tunnel_dscp_trust), /* Trust incomming tunnel DSCP         */ \
SHR_G_ENTRY(tunnel_protocol_match),/* Tunnel term key includes protocol. */ \
SHR_G_ENTRY(higig_lookup),      /* FB B0 Proxy (HG lookup)             */ \
SHR_G_ENTRY(egr_l3_mtu),        /* L3 MTU check                        */ \
SHR_G_ENTRY(egr_mirror_path),   /* Alternate path for egress mirror    */ \
SHR_G_ENTRY(xgs1_mirror),       /* XGS1 mirroring compatibility support*/ \
SHR_G_ENTRY(register_hi),       /* _HI designation for > 32 ports      */ \
SHR_G_ENTRY(table_hi),          /* _HI designation for > 32 ports      */ \
SHR_G_ENTRY(trunk_extended),    /* extended designation for > 32 trunks*/ \
SHR_G_ENTRY(trunk_extended_only), /* only support extended trunk mode  */ \
SHR_G_ENTRY(hg_trunking),       /* higig trunking support              */ \
SHR_G_ENTRY(hg_trunk_override), /* higig trunk override support        */ \
SHR_G_ENTRY(trunk_group_size),  /* trunk group size support            */ \
SHR_G_ENTRY(shared_trunk_member_table), /* shared trunk member table   */ \
SHR_G_ENTRY(egr_vlan_check),    /* Support for Egress VLAN check PBM   */ \
SHR_G_ENTRY(mod1),              /* Ports spread over two module Ids    */ \
SHR_G_ENTRY(egr_ts_ctrl),       /* Egress Time Slot Control            */ \
SHR_G_ENTRY(ipmc_grp_parity),   /* Parity support on IPMC GROUP table  */ \
SHR_G_ENTRY(mpls_pop_search),   /* MPLS pop search support             */ \
SHR_G_ENTRY(cpu_proto_prio),    /* CPU protocol priority support       */ \
SHR_G_ENTRY(ignore_pkt_tag),    /* Ignore packet vlan tag support      */ \
SHR_G_ENTRY(port_lag_failover), /* Port LAG failover support           */ \
SHR_G_ENTRY(hg_trunk_failover), /* Higig trunk failover support        */ \
SHR_G_ENTRY(trunk_egress),      /* Trunk egress support                */ \
SHR_G_ENTRY(force_forward),     /* Egress port override support        */ \
SHR_G_ENTRY(port_egr_block_ctl),/* L2/L3 port egress block control     */ \
SHR_G_ENTRY(ipmc_repl_freeze),  /* IPMC repl config pauses traffic     */ \
SHR_G_ENTRY(bucket_support),    /* MAX/MIN BUCKET support              */ \
SHR_G_ENTRY(remote_learn_trust),/* DO_NOT_LEARN bit in HiGig Header    */ \
SHR_G_ENTRY(ipmc_group_mtu),    /* IPMC MTU Setting for all Groups     */ \
SHR_G_ENTRY(tx_fast_path),      /* TX Descriptor mapped send           */ \
SHR_G_ENTRY(deskew_dll),        /* Deskew DLL present                  */ \
SHR_G_ENTRY(src_mac_group),     /* Src MAC Group (MAC Block Index)     */ \
SHR_G_ENTRY(storm_control),     /* Strom Control                       */ \
SHR_G_ENTRY(hw_stats_calc),     /* Calculation of stats in HW          */ \
SHR_G_ENTRY(ext_tcam_sharing),  /* External TCAM sharing support       */ \
SHR_G_ENTRY(mac_learn_limit),   /* MAC Learn limit support             */ \
SHR_G_ENTRY(mac_learn_limit_rollover),   /* MAC Learn limit counter roll over */ \
SHR_G_ENTRY(linear_drr_weight), /* Linear DRR weight calculation       */ \
SHR_G_ENTRY(igmp_mld_support),  /* IGMP MLD snooping support           */ \
SHR_G_ENTRY(basic_dos_ctrl),    /* TCP flags, L4 port, TCP frag, ICMP  */ \
SHR_G_ENTRY(enhanced_dos_ctrl), /* Enhanced DOS control features       */ \
SHR_G_ENTRY(ctr_xaui_activity), /* XAUI counter activity support       */ \
SHR_G_ENTRY(proto_pkt_ctrl),    /* Protocol packet control             */ \
SHR_G_ENTRY(vlan_ctrl),         /* Per VLAN property control           */ \
SHR_G_ENTRY(tunnel_6to4_secure),/* Secure IPv6 to IPv4 tunnel support. */ \
SHR_G_ENTRY(tunnel_any_in_6),   /* Any in IPv6 tunnel support.         */ \
SHR_G_ENTRY(tunnel_gre),        /* GRE  tunneling support.             */ \
SHR_G_ENTRY(unknown_ipmc_tocpu),/* Send unknown IPMC packet to CPU.    */ \
SHR_G_ENTRY(src_trunk_port_bridge), /* Source trunk port bridge        */ \
SHR_G_ENTRY(stat_dma_error_ack),/* Stat DMA error acknowledge bit offset */ \
SHR_G_ENTRY(big_icmpv6_ping_check),/* ICMP V6 oversize packet disacrd  */ \
SHR_G_ENTRY(asf),               /* Alternate Store and Forward         */ \
SHR_G_ENTRY(asf_no_10_100),     /* No ASF support for 10/100Mb speeds  */ \
SHR_G_ENTRY(trunk_group_overlay),/* Direct TGID overlay of modid/port  */ \
SHR_G_ENTRY(xport_convertible), /* XPORT <-> Higig support             */ \
SHR_G_ENTRY(dual_hash),         /* Dual hash tables support            */ \
SHR_G_ENTRY(robo_sw_override),  /* Robo Port Software Override state   */ \
SHR_G_ENTRY(dscp),              /* DiffServ QoS                        */ \
SHR_G_ENTRY(auth),              /* AUTH 802.1x support                 */ \
SHR_G_ENTRY(mstp),                                                        \
SHR_G_ENTRY(arl_mode_control),  /* ARL CONTROL (DS/DD/CPU)             */ \
SHR_G_ENTRY(no_stat_mib),                                                 \
SHR_G_ENTRY(rcpu_1),            /* Remote CPU feature (Raptor)         */ \
SHR_G_ENTRY(unimac),            /* UniMAC 10/100/1000                  */ \
SHR_G_ENTRY(xmac),              /* XMAC                                */ \
SHR_G_ENTRY(xlmac),             /* XLMAC                               */ \
SHR_G_ENTRY(cmac),              /* CMAC                                */ \
SHR_G_ENTRY(clmac),             /* CLMAC                               */ \
SHR_G_ENTRY(hw_dos_prev),       /* hardware dos attack prevention(Robo)*/ \
SHR_G_ENTRY(hw_dos_report),     /* hardware dos event report(Robo)     */ \
SHR_G_ENTRY(generic_table_ops), /* Generic SCHAN table operations      */ \
SHR_G_ENTRY(lpm_prefix_length_max_128),/* Maximum lpm prefix len 128   */ \
SHR_G_ENTRY(tag_enforcement),   /* BRCM tag with tag_enforcement       */ \
SHR_G_ENTRY(vlan_translation_range),   /* VLAN translation range       */ \
SHR_G_ENTRY(vlan_xlate_dtag_range), /*VLAN translation double tag range*/ \
SHR_G_ENTRY(vlan_xlate_has_class_id), /*Ing and Egr VLAN XLATE tables have class_id*/ \
SHR_G_ENTRY(mpls_per_vlan),     /* MPLS enable per-vlan                */ \
SHR_G_ENTRY(mpls_bos_lookup),   /* Second lookup bottom of stack == 0  */ \
SHR_G_ENTRY(mpls_frr_lookup),   /* MPLS Fast Re-Route label lookup     */ \
SHR_G_ENTRY(l3_tunnel_mode_fld_is_keytype), /* L3_TUNNEL MODE field    */ \
SHR_G_ENTRY(class_based_learning),  /* L2 class based learning         */ \
SHR_G_ENTRY(static_pfm),        /* Static MH PFM control               */ \
SHR_G_ENTRY(ipmc_repl_penultimate), /* Flag last IPMC replication      */ \
SHR_G_ENTRY(sgmii_autoneg),     /* Support SGMII autonegotiation       */ \
SHR_G_ENTRY(gmii_clkout),       /* Support GMII clock output           */ \
SHR_G_ENTRY(ip_ep_mem_parity),  /* Parity support on IP, EP memories   */ \
SHR_G_ENTRY(post),              /* Post after reset init               */ \
SHR_G_ENTRY(rcpu_priority),     /* Remote CPU packet QoS handling      */ \
SHR_G_ENTRY(rcpu_tc_mapping),   /* Remote CPU traffic class to queue   */ \
SHR_G_ENTRY(mem_push_pop),      /* SCHAN push/pop operations           */ \
SHR_G_ENTRY(dcb_reason_hi),     /* Number of RX reasons exceeds 32     */ \
SHR_G_ENTRY(multi_sbus_cmds),   /* SCHAN multiple sbus commands        */ \
SHR_G_ENTRY(flexible_dma_steps),/* Flexible sbus address increment step */ \
SHR_G_ENTRY(new_sbus_format),   /* New sbus command format             */ \
SHR_G_ENTRY(new_sbus_old_resp), /* New sbus command format, Old Response */ \
SHR_G_ENTRY(sbus_format_v4),    /* Sbus command format version 4       */ \
SHR_G_ENTRY(blkid_extended_format),  /* Block ID bigger than 127            */ \
SHR_G_ENTRY(esm_support),       /* External TCAM support               */ \
SHR_G_ENTRY(esm_rxfifo_resync), /* ESM with DDR RX_FIFO resync logic   */ \
SHR_G_ENTRY(fifo_dma),          /* fifo DMA support                    */ \
SHR_G_ENTRY(fifo_dma_active),   /* fifo DMA active bit set if its not empty */ \
SHR_G_ENTRY(ipfix),             /* IPFIX support                       */ \
SHR_G_ENTRY(ipfix_rate),        /* IPFIX rate and rate mirror support  */ \
SHR_G_ENTRY(ipfix_flow_mirror), /* IPFIX flow mirror support           */ \
SHR_G_ENTRY(l3_entry_key_type), /* Data stored in first entry only.    */ \
SHR_G_ENTRY(robo_ge_serdes_mac_autosync), /* serdes_autosync with mac  */ \
SHR_G_ENTRY(fp_routing_mirage), /* Mirage fp based routing.            */ \
SHR_G_ENTRY(fp_routing_hk),     /* Hawkeye fp based routing.           */ \
SHR_G_ENTRY(subport),           /* Subport support                     */ \
SHR_G_ENTRY(reset_delay),       /* Delay after CMIC soft reset         */ \
SHR_G_ENTRY(fast_egr_cell_count), /* HW accelerated cell count retrieval */ \
SHR_G_ENTRY(802_3as),           /* 802.3as                             */ \
SHR_G_ENTRY(l2_pending),        /* L2 table pending support            */ \
SHR_G_ENTRY(discard_ability),   /* discard/WRED support                */ \
SHR_G_ENTRY(discard_ability_color_black), /* discard/WRED color black  */ \
SHR_G_ENTRY(distribution_ability), /* distribution/ESET support        */ \
SHR_G_ENTRY(mc_group_ability),  /* McGroup support                     */ \
SHR_G_ENTRY(cosq_gport_stat_ability), /* COS queue stats support       */ \
SHR_G_ENTRY(standalone),        /* standalone switch mode              */ \
SHR_G_ENTRY(internal_loopback), /* internal loopback port              */ \
SHR_G_ENTRY(packet_adj_len),    /* packet adjust length                */ \
SHR_G_ENTRY(vlan_action),       /* VLAN action support                 */ \
SHR_G_ENTRY(vlan_pri_cfi_action), /* VLAN pri/CFI action support       */ \
SHR_G_ENTRY(vlan_copy_action),  /* VLAN copy action support            */ \
SHR_G_ENTRY(packet_rate_limit), /* Packet-based rate limitting to CPU  */ \
SHR_G_ENTRY(system_mac_learn_limit), /* System MAC Learn limit support */ \
SHR_G_ENTRY(fp_based_routing),  /* L3 based on field processor.        */ \
SHR_G_ENTRY(field),             /* Field Processor (FP) for XGS3       */ \
SHR_G_ENTRY(field_slices2),     /* FP TCAM has 2 slices, instead of 16 */ \
SHR_G_ENTRY(field_slices4),     /* FP TCAM has 4 slices, instead of 16 */ \
SHR_G_ENTRY(field_slices8),     /* FP TCAM has 8 slices, instead of 16 */ \
SHR_G_ENTRY(field_slices10),    /* FP TCAM has 10 slices,instead of 16 */ \
SHR_G_ENTRY(field_slices12),    /* FP TCAM has 12 slices,instead of 16 */ \
SHR_G_ENTRY(field_meter_pools4),/* FP TCAM has 4 global meter pools.   */ \
SHR_G_ENTRY(field_meter_pools8),/* FP TCAM has 8 global meter pools.   */ \
SHR_G_ENTRY(field_meter_pools12),/* FP TCAM has 12 global meter pools. */ \
SHR_G_ENTRY(field_mirror_ovr),  /* FP has MirrorOverride action        */ \
SHR_G_ENTRY(field_udf_higig),   /* FP UDF window contains HiGig header */ \
SHR_G_ENTRY(field_udf_higig2),  /* FP UDF window contains HiGig2 header */ \
SHR_G_ENTRY(field_udf_hg),      /* FP UDF2.0->2.2 HiGig header         */ \
SHR_G_ENTRY(field_udf_ethertype), /* FP UDF can ues extra ethertypes   */ \
SHR_G_ENTRY(field_comb_read),   /* FP can read TCAM & Policy as one    */ \
SHR_G_ENTRY(field_wide),        /* FP has wide-mode combining of slices*/ \
SHR_G_ENTRY(field_slice_enable),/* FP enable/disable on per slice basis*/ \
SHR_G_ENTRY(field_cos),         /* FP has CoS Queue actions            */ \
SHR_G_ENTRY(field_ingress_late),/* FP has late ingress pipeline stage  */ \
SHR_G_ENTRY(field_color_indep), /* FP can set color in/dependence      */ \
SHR_G_ENTRY(field_qual_drop),   /* FP can qualify on drop state        */ \
SHR_G_ENTRY(field_qual_IpType), /* FP can qualify on IpType alone      */ \
SHR_G_ENTRY(field_qual_Ip6High), /* FP can qualify on Src/Dst Ip 6 High */ \
SHR_G_ENTRY(field_mirror_pkts_ctl), /* Enable FP for mirror packtes    */ \
SHR_G_ENTRY(field_intraslice_basic_key), /* Only IP4 or IP6 key.       */ \
SHR_G_ENTRY(field_ingress_two_slice_types),/* Ingress FP has 2 slice types*/\
SHR_G_ENTRY(field_ingress_global_meter_pools), /* Global Meter Pools   */ \
SHR_G_ENTRY(field_ingress_ipbm), /* FP IPBM qualifier is always present*/ \
SHR_G_ENTRY(field_egress_flexible_v6_key), /* Egress FP KEY2 SIP6/DIP6.*/ \
SHR_G_ENTRY(field_egress_global_counters), /* Global Counters          */ \
SHR_G_ENTRY(field_egress_metering), /* FP allows metering in egress mode */ \
SHR_G_ENTRY(field_ing_egr_separate_packet_byte_counters), /*           */ \
SHR_G_ENTRY(field_multi_stage), /* Multi staged field processor.       */ \
SHR_G_ENTRY(field_intraslice_double_wide), /* FP double wide in 1 slice*/ \
SHR_G_ENTRY(field_int_fsel_adj),/* FP internal field select adjust     */ \
SHR_G_ENTRY(field_pkt_res_adj), /* FP field packet resolution adjust   */ \
SHR_G_ENTRY(field_virtual_slice_group), /* Virtual slice/groups in FP  */ \
SHR_G_ENTRY(field_qualify_gport), /* EgrObj/Mcast Grp/Gport qualifiers */ \
SHR_G_ENTRY(field_action_redirect_ipmc), /* Redirect to ipmc index.    */ \
SHR_G_ENTRY(field_action_timestamp), /* Copy to cpu with timestamp.    */ \
SHR_G_ENTRY(field_action_l2_change), /* Modify l2 packets sa/da/vlan.  */ \
SHR_G_ENTRY(field_action_fabric_queue), /* Add HiGig2 extension header. */ \
SHR_G_ENTRY(field_virtual_queue),    /* Virtual queue support.         */ \
SHR_G_ENTRY(field_vfp_flex_counter), /* Flex cntrs support in vfp.     */ \
SHR_G_ENTRY(field_tcam_hw_move), /* TCAM move support via HW.          */ \
SHR_G_ENTRY(field_tcam_parity_check), /* TCAM parity check.            */ \
SHR_G_ENTRY(field_qual_my_station_hit), /* FP can qualify on My Station Hit*/ \
SHR_G_ENTRY(field_action_redirect_nexthop), /* Redirect packet to Next Hop index */ \
SHR_G_ENTRY(field_action_redirect_ecmp), /* Redirect packet to ECMP group */ \
SHR_G_ENTRY(field_slice_dest_entity_select), /* FP can qualify on destination type */ \
SHR_G_ENTRY(field_packet_based_metering), /* FP supports packet based metering */ \
SHR_G_ENTRY(field_stage_half_slice), /* Only half the number of total entries in FP_TCAM and FP_POLICY tables are used */ \
SHR_G_ENTRY(field_stage_quarter_slice), /* Only quarter the number of total entries in FP_TCAM and FP_POLICY tables are used */ \
SHR_G_ENTRY(field_slice_size128),/* Only 128 entries in FP_TCAM and FP_POLICY tables are used */ \
SHR_G_ENTRY(field_stage_ingress_256_half_slice),/* Only half entries/slice are used when FP_TCAM and FP_POLICY have 256 entries/slice */\
SHR_G_ENTRY(field_stage_ingress_512_half_slice),/* Only half entries/slice are used when FP_TCAM and FP_POLICY have 512 entries/slice */\
SHR_G_ENTRY(field_stage_egress_256_half_slice),/*  Only half entries/slice are used when EFP_TCAM has 256 entries/slice */\
SHR_G_ENTRY(field_stage_egress_512_half_slice),/*  Only half entries/slice are used when EFP_TCAM has 512 entries/slice */\
SHR_G_ENTRY(field_stage_lookup_256_half_slice),/*  Only half entries/slice are used when VFP_TCAM has 256 entries/slice */\
SHR_G_ENTRY(field_stage_lookup_512_half_slice),/*  Only half entries/slice are used when VFP_TCAM has 512 entries/slice */\
SHR_G_ENTRY(field_oam_actions), /* FP supports OAM actions. */ \
SHR_G_ENTRY(field_ingress_cosq_override), /* Ingress FP overrides assigned CoSQ */ \
SHR_G_ENTRY(field_egress_tocpu), /* Egress copy-to-cpu */\
SHR_G_ENTRY(field_multi_pipe_support), /* FP per-pipe configuration model supported. */\
SHR_G_ENTRY(field_ingress_triple_wide), /* Ingress FP supports pairing of three slices. */\
SHR_G_ENTRY(field_preselector_support), /* IFP preselector Module Support. */\
SHR_G_ENTRY(virtual_switching), /* Virtual lan services support        */ \
SHR_G_ENTRY(lport_tab_profile), /* Lport table is profiled per svp     */ \
SHR_G_ENTRY(xgport_one_xe_six_ge), /* XGPORT mode with 1x10G + 6x1GE   */ \
SHR_G_ENTRY(sample_offset8),  /* Sample threshold is shifted << 8 bits */ \
SHR_G_ENTRY(sample_thresh16), /* Sample threshold 16 bit wide          */ \
SHR_G_ENTRY(sample_thresh24), /* Sample threshold 24 bit wide          */ \
SHR_G_ENTRY(mim),             /* MIM for XGS3                          */ \
SHR_G_ENTRY(oam),             /* OAM for XGS3                          */ \
SHR_G_ENTRY(bfd),             /* BFD for XGS3                          */ \
SHR_G_ENTRY(bhh),             /* BHH for XGS3                          */ \
SHR_G_ENTRY(eth_lm_dm),       /* ETH_LM_DM for XGS3                    */ \
SHR_G_ENTRY(ptp),             /* PTP for XGS3                          */ \
SHR_G_ENTRY(sat),             /* SAT for XGS4                          */ \
SHR_G_ENTRY(hybrid),          /* global hybrid configuration feature   */ \
SHR_G_ENTRY(node_hybrid),     /* node hybrid configuration feature     */ \
SHR_G_ENTRY(arbiter),         /* arbiter feature                       */ \
SHR_G_ENTRY(arbiter_capable), /* arbiter capable feature               */ \
SHR_G_ENTRY(node),            /* node hybrid configuration mode        */ \
SHR_G_ENTRY(lcm),             /* lcm  feature                          */ \
SHR_G_ENTRY(xbar),            /* xbar feature                          */ \
SHR_G_ENTRY(egr_independent_fc), /* egr fifo independent flow control  */ \
SHR_G_ENTRY(egr_multicast_independent_fc), /* egr multicast fifo independent flow control  */ \
SHR_G_ENTRY(always_drive_dbus), /* drive esm dbus                      */ \
SHR_G_ENTRY(ignore_cmic_xgxs_pll_status), /* ignore CMIC XGXS PLL lock status */ \
SHR_G_ENTRY(mmu_virtual_ports), /* internal MMU virtual ports          */ \
SHR_G_ENTRY(phy_lb_needed_in_mac_lb), /* PHY loopback has to be set when in MAC loopback mode */ \
SHR_G_ENTRY(gport_service_counters), /* Counters for ports and services */ \
SHR_G_ENTRY(use_double_freq_for_ddr_pll), /* use double freq for DDR PLL */\
SHR_G_ENTRY(eav_support),     /* EAV supporting                        */ \
SHR_G_ENTRY(wlan),            /* WLAN for XGS3                         */ \
SHR_G_ENTRY(counter_parity),  /* Counter registers have parity fields  */ \
SHR_G_ENTRY(time_support),    /* Time module support                   */ \
SHR_G_ENTRY(time_v3),         /* 3rd gen time arch: 48b TS, dual BS/TS */ \
SHR_G_ENTRY(time_v3_no_bs),   /* No BroadSync for certian SKUs         */ \
SHR_G_ENTRY(timesync_support), /* Time module support                  */ \
SHR_G_ENTRY(timesync_v3),     /* 3rd gen timesync/1588 arch: 48bit TS  */ \
SHR_G_ENTRY(tdpll_outputclk_synce3),/* TDPLL output clock XGPLL2/PMHPLL support */ \
SHR_G_ENTRY(tdpll_outputclk_xgpll3),/* TDPLL output clock XGPLL3 support */ \
SHR_G_ENTRY(ip_source_bind),  /* IP to MAC address binding feature     */ \
SHR_G_ENTRY(auto_multicast),  /* Automatic Multicast Tunneling Support */ \
SHR_G_ENTRY(embedded_higig),  /* Embedded Higig Tunneling Support      */ \
SHR_G_ENTRY(higig_over_ethernet), /* Higig over Ethernet               */ \
SHR_G_ENTRY(hawkeye_a0_war),  /* QSGMII Workaroud                      */ \
SHR_G_ENTRY(vlan_queue_map),  /* VLAN queue mapping  support           */ \
SHR_G_ENTRY(mpls_software_failover), /* MPLS FRR support within Software */   \
SHR_G_ENTRY(mpls_failover),   /* MPLS FRR support                      */ \
SHR_G_ENTRY(extended_pci_error), /* More DMA info for PCI errors       */ \
SHR_G_ENTRY(subport_enhanced), /* Enhanced Subport Support             */ \
SHR_G_ENTRY(priority_flow_control), /* Per-priority flow control       */ \
SHR_G_ENTRY(source_port_priority_flow_control), /* Per-(source port, priority) flow control */ \
SHR_G_ENTRY(flex_port),       /* Flex-port aka hot-swap Support        */ \
SHR_G_ENTRY(qos_profile),     /* QoS profiles for ingress and egress   */ \
SHR_G_ENTRY(fe_ports),        /* Raven/Raptor with FE ports            */ \
SHR_G_ENTRY(mirror_flexible), /* Flexible ingress/egress mirroring     */ \
SHR_G_ENTRY(egr_mirror_true), /* True egress mirroring                 */ \
SHR_G_ENTRY(failover),        /* Generalized VP failover               */ \
SHR_G_ENTRY(delay_core_pll_lock), /* Tune core clock before PLL lock   */ \
SHR_G_ENTRY(extended_cmic_error), /* Block specific errors in CMIC_IRQ_STAT_1/2 */ \
SHR_G_ENTRY(short_cmic_error), /* Override Block specific errors in CMIC_IRQ_STAT_1/2 */ \
SHR_G_ENTRY(urpf),            /* Unicast reverse path check.           */ \
SHR_G_ENTRY(no_bridging),     /* No L2 and L3.                         */ \
SHR_G_ENTRY(no_higig),        /* No higig support                      */ \
SHR_G_ENTRY(no_higig_plus),   /* No higig plus support                 */ \
SHR_G_ENTRY(no_epc_link),     /* No EPC_LINK_BMAP                      */ \
SHR_G_ENTRY(no_mirror),       /* No mirror support                     */ \
SHR_G_ENTRY(no_learning),     /* No CML support                        */ \
SHR_G_ENTRY(rx_timestamp),      /* RX packet timestamp filed should be updated  */ \
SHR_G_ENTRY(rx_timestamp_upper),/* RX packet timestamp uper 32bit filed should be updated  */ \
SHR_G_ENTRY(sysport_remap),   /* Support for remapping system port to physical port */ \
SHR_G_ENTRY(flexible_xgport), /* Allow 10G/1G speed change on XGPORT   */ \
SHR_G_ENTRY(logical_port_num),/* Physical/logical port number mapping  */ \
SHR_G_ENTRY(mmu_config_property),/* MMU config property                */ \
SHR_G_ENTRY(l2_bulk_control), /* L2 bulk control                       */ \
SHR_G_ENTRY(l2_bulk_unified_table), /*L2 bulk control with single table*/ \
SHR_G_ENTRY(l2_bulk_bypass_replace), /* L2 bulk control bypass replace */ \
SHR_G_ENTRY(timestamp_counter), /* Timestamp FIFO in counter range     */ \
SHR_G_ENTRY(l3_ingress_interface), /* Layer-3 Ingress Interface Object */ \
SHR_G_ENTRY(ingress_size_templates), /* Ingress Size Tempaltes         */ \
SHR_G_ENTRY(l3_defip_ecmp_count), /* L3 DEFIP ECMP Count               */ \
SHR_G_ENTRY(ppa_bypass),      /* PPA ignoring key type field in L2X    */ \
SHR_G_ENTRY(ppa_match_vp),    /* PPA allows to match on virtual port   */ \
SHR_G_ENTRY(generic_counters), /* Common counters shared by both XE/GE */ \
SHR_G_ENTRY(mpls_enhanced),   /* MPLS Enhancements                     */ \
SHR_G_ENTRY(trill),           /* Trill support                         */ \
SHR_G_ENTRY(niv),             /* Network interface virtualization      */ \
SHR_G_ENTRY(unimac_tx_crs),   /* Unimac TX CRS */ \
SHR_G_ENTRY(hg_trunk_override_profile), /* Higig trunk override is profiled */ \
SHR_G_ENTRY(modport_map_profile), /* Modport map table is profiled     */ \
SHR_G_ENTRY(hg_dlb),          /* Higig DLB                             */ \
SHR_G_ENTRY(l3_defip_hole),   /* L3 DEFIP Hole                         */ \
SHR_G_ENTRY(eee),             /* Energy Efficient Ethernet             */ \
SHR_G_ENTRY(mdio_setup),      /* MDIO setup for FE devices             */ \
SHR_G_ENTRY(ser_parity),      /* SER parity protection available       */ \
SHR_G_ENTRY(ser_engine),      /* Standalone SER engine available       */ \
SHR_G_ENTRY(ser_fifo),        /* Hardware reports events via SER fifos */ \
SHR_G_ENTRY(ser_hw_bg_read),  /* Standalone SER engine has background read available */ \
SHR_G_ENTRY(mem_cache),       /* Memory cache for SER correction       */ \
SHR_G_ENTRY(regs_as_mem),     /* Registers implemented as memory       */ \
SHR_G_ENTRY(fp_meter_ser_verify), /* Reverify FP METER TABLE SER       */ \
SHR_G_ENTRY(int_cpu_arbiter), /* Internal CPU arbiter                  */ \
SHR_G_ENTRY(ets),             /* Enhanced transmission selection       */ \
SHR_G_ENTRY(qcn),             /* Quantized congestion notification     */ \
SHR_G_ENTRY(xy_tcam),         /* Non data/mask type TCAM               */ \
SHR_G_ENTRY(xy_tcam_direct),  /* Non data/mask type TCAM with h/w translation bypass */ \
SHR_G_ENTRY(xy_tcam_28nm),    /* Non data/mask type TCAM               */ \
SHR_G_ENTRY(xy_tcam_lpt),     /* Non data/mask type TCAM with LPT encoding */ \
SHR_G_ENTRY(bucket_update_freeze), /* Disable refresh when updating bucket */ \
SHR_G_ENTRY(hg_trunk_16_members), /* Higig trunk has fixed size of 16  */ \
SHR_G_ENTRY(vlan_vp),         /* VLAN virtual port switching           */ \
SHR_G_ENTRY(vp_group_ingress_vlan_membership), /* Ingress VLAN virtual port group membership */ \
SHR_G_ENTRY(vp_group_egress_vlan_membership), /* Egress VLAN virtual port group membership */ \
SHR_G_ENTRY(ing_vp_vlan_membership), /* Ingress VLAN virtual port membership */ \
SHR_G_ENTRY(egr_vp_vlan_membership), /* Egress VLAN virtual port membership */ \
/* Supports olicer in flow mode with committed rate */ \
SHR_G_ENTRY(policer_mode_flow_rate_committed), \
SHR_G_ENTRY(vlan_egr_it_inner_replace), \
SHR_G_ENTRY(src_modid_base_index), /* Per source module ID base index  */ \
SHR_G_ENTRY(wesp),            /* Wrapped Encapsulating Security Payload */ \
SHR_G_ENTRY(cmicm),           /* CMICm                                 */ \
SHR_G_ENTRY(cmicm_b0),        /* CMICm B0                              */ \
SHR_G_ENTRY(cmicd),           /* CMICd                                 */ \
SHR_G_ENTRY(continuous_dma),  /* Support for Continuous DMA            */ \
SHR_G_ENTRY(mcs),             /* Micro Controller Subsystem            */ \
SHR_G_ENTRY(iproc),           /* iProc Internal Processor Subsystem    */ \
SHR_G_ENTRY(iproc_7),         /* iProc 7 profile */ \
SHR_G_ENTRY(cmicm_extended_interrupts), /* CMICm IRQ1, IRQ2            */ \
SHR_G_ENTRY(cmicm_multi_dma_cmc), /* CMICm dma on multiple cmc         */ \
SHR_G_ENTRY(ism_memory),      /* Support for ISM memory                */ \
SHR_G_ENTRY(shared_hash_mem), /* Support for shared hash memory        */ \
SHR_G_ENTRY(shared_hash_ins), /* Support for sw based shared hash ins  */ \
SHR_G_ENTRY(unified_port),    /* Support new unified port design       */ \
SHR_G_ENTRY(sbusdma),         /* CMICM SBUSDMA support                 */ \
SHR_G_ENTRY(regex),           /* Regex signature match                 */ \
SHR_G_ENTRY(l3_ecmp_1k_groups),  /* L3 ECMP 1K Groups                  */ \
SHR_G_ENTRY(advanced_flex_counter), /* Advance flexible counter feature */ \
SHR_G_ENTRY(ces),             /* Circuit Emulation Services            */ \
SHR_G_ENTRY(ddr3),            /* External DDR3 Buffer                  */ \
SHR_G_ENTRY(iproc_ddr),       /* iProc DDR initialization              */ \
SHR_G_ENTRY(mpls_entropy),    /* MPLS Entropy-label feature            */ \
SHR_G_ENTRY(global_meter),    /* Service meter                         */ \
SHR_G_ENTRY(global_meter_v2), /* Service meter using driver function   */ \
SHR_G_ENTRY(global_meter_mef_10dot3), /* MEF 10.3 compliant Service meter */ \
SHR_G_ENTRY(global_meter_macro_micro_same_pool), /* Both must be in same pool */ \
SHR_G_ENTRY(global_meter_pool_priority_descending), /* Global meter pool priority Order 0[high]-7[low] */ \
SHR_G_ENTRY(modport_map_dest_is_port_or_trunk), /* Modport map destination is specified as a Higig port or a Higig trunk, instead of as a Higig port bitmap */ \
SHR_G_ENTRY(mirror_encap_profile), /* Egress mirror encap data profile */ \
SHR_G_ENTRY(directed_mirror_only), /* Only directed mirroring mode     */ \
SHR_G_ENTRY(axp),             /* Auxiliary ports                       */ \
SHR_G_ENTRY(etu_support),     /* External TCAM support                 */ \
SHR_G_ENTRY(controlled_counters),            /* L3 ECMP 1K Groups */ \
SHR_G_ENTRY(higig_misc_speed_support), /* Misc speed support - 21G, 42G */ \
SHR_G_ENTRY(e2ecc),           /* End-to-end congestion control         */ \
SHR_G_ENTRY(vpd_profile),     /* VLAN protocol data profile            */ \
SHR_G_ENTRY(color_prio_map_profile), /* Map color priority via profile */ \
SHR_G_ENTRY(hg_dlb_member_id), /* Higig DLB member ports are converted to member IDs */ \
SHR_G_ENTRY(lag_dlb),         /* LAG dynamic load balancing            */ \
SHR_G_ENTRY(ecmp_dlb),        /* ECMP dynamic load balancing           */ \
SHR_G_ENTRY(ecmp_dlb_optimized), /* TH2 version ECMP Dynamic load balancing. */ \
SHR_G_ENTRY(hgt_lag_dlb_optimized), /* TH2 version HGT/LAG Dynamic load balancing. */ \
SHR_G_ENTRY(ecmp_dlb_property_force_set), /* force set without notifying when ecmp member's dlb property conflicts. */ \
SHR_G_ENTRY(dgm),             /* Dynamic Group Multipath */ \
SHR_G_ENTRY(l2gre),           /* L2-VPN over GRE Tunnels               */ \
SHR_G_ENTRY(l2gre_default_tunnel),           /* Default tunnel based forwarding during L2-GRE VPN lookup failure    */ \
SHR_G_ENTRY(static_repl_head_alloc), /* Allocation of REPL_HEAD table entries is static */ \
SHR_G_ENTRY(vlan_double_tag_range_compress), /* VLAN range compression for double tagged packets */\
SHR_G_ENTRY(vlan_protocol_pkt_ctrl), /* per VLAN protocol packet control */ \
SHR_G_ENTRY(l3_extended_host_entry), /* Extended L3 host entry with embedded NHs  */ \
SHR_G_ENTRY(repl_head_ptr_replace), /* MMU supports replacement of REPL_HEAD pointers in multicast queues */ \
SHR_G_ENTRY(remote_encap),    /* Higig2 remote replication encap       */ \
SHR_G_ENTRY(rx_reason_overlay), /* RX reasons are in overlays          */ \
SHR_G_ENTRY(extended_queueing),  /* Extened queueing suport            */ \
SHR_G_ENTRY(dynamic_sched_update), /*  Enable dynamic update of scheduler mode */ \
SHR_G_ENTRY(schan_err_check), /*  Enable dynamic update of scheduler mode */\
SHR_G_ENTRY(l3_reduced_defip_table), /* Reduced L3 route table         */ \
SHR_G_ENTRY(l3_expanded_defip_table), /* Expanded L3 route table - e.g. 32 physical tcams */ \
SHR_G_ENTRY(rtag1_6_max_portcnt_less_than_rtag7), /* Max port count for RTAG 1-6 is less than RTAG 7 */ \
SHR_G_ENTRY(vlan_xlate),      /* Vlan translation on MPLS packets      */ \
SHR_G_ENTRY(split_repl_group_table), /* MMU replication group table is split into 2 halves */ \
SHR_G_ENTRY(pim_bidir),       /* Bidirectional PIM                     */ \
SHR_G_ENTRY(l3_iif_zero_invalid), /* L3 ingress interface ID 0 is invalid */ \
SHR_G_ENTRY(vector_based_spri), /* MMU vector based strict priority scheduling */ \
SHR_G_ENTRY(vxlan),           /* L2-VPN over UDP Tunnels               */ \
SHR_G_ENTRY(ep_redirect),     /* Egress pipeline redirection           */ \
SHR_G_ENTRY(repl_l3_intf_use_next_hop), /* For each L3 interface, MMU replication outputs a next hop index */ \
SHR_G_ENTRY(dynamic_shaper_update), /*  Enable dynamic update of shaper rates */ \
SHR_G_ENTRY(nat),             /* NAT                                   */ \
SHR_G_ENTRY(l3_iif_profile),  /* Profile for L3_IIF                    */ \
SHR_G_ENTRY(l3_ip4_options_profile), /* Supports special handling for IP4 options */\
SHR_G_ENTRY(linkphy_coe),     /* Supports LinkPHY subports (IEEE G.999.1)  */ \
SHR_G_ENTRY(subtag_coe),      /* Supports SubTag (Third VLAN tag) subports */ \
SHR_G_ENTRY(tr3_sp_vector_mask), /* SP <-> WRR configuration sequence  */ \
SHR_G_ENTRY(cmic_reserved_queues), /* CMIC has reserved queues     */ \
SHR_G_ENTRY(pgw_mac_rsv_mask_remap), /* PGW_MAC_RSV_MASK address remap */ \
SHR_G_ENTRY(endpoint_queuing), /* Endpoint queuing                     */ \
SHR_G_ENTRY(service_queuing), /* Service queuing                       */ \
SHR_G_ENTRY(mirror_control_mem), /* Mirror control is not register     */ \
SHR_G_ENTRY(mirror_table_trunk), /* Mirror MTPs duplicate trunk info   */ \
SHR_G_ENTRY(port_extension),  /* Port Extension (IEEE 802.1Qbh)        */ \
SHR_G_ENTRY(port_extension_kt2_key_type), /* Port Extension key type change for certain devices */\
SHR_G_ENTRY(linkscan_pause_timeout), /* wating for Linkscan stopped signal with timeout*/\
SHR_G_ENTRY(port_extender_vp_sharing), /* Port Extender vp sharing for certain devices */\
SHR_G_ENTRY(linkscan_lock_per_unit), /* linkscsan lock per unit instead of spl*/\
SHR_G_ENTRY(easy_reload_wb_compat), /* Support Easy Reload and Warm boot within the same compilation*/\
SHR_G_ENTRY(mac_virtual_port), /* MAC-based virtual port               */ \
SHR_G_ENTRY(virtual_port_routing), /* VP based routing for VP LAG      */ \
SHR_G_ENTRY(counter_toggled_read), /* Toggled read of counter tables   */ \
SHR_G_ENTRY(vp_lag),          /* Virtual port LAG                      */ \
SHR_G_ENTRY(min_resume_limit_1), /* min resume limit for port-sp       */ \
SHR_G_ENTRY(hg_dlb_id_equal_hg_tid), /* Higig DLB ID is the same as Higig trunk ID */ \
SHR_G_ENTRY(hg_resilient_hash), /* Higig resilient hashing */ \
SHR_G_ENTRY(lag_resilient_hash), /* LAG resilient hashing */ \
SHR_G_ENTRY(ecmp_resilient_hash), /* ECMP resilient hashing */ \
SHR_G_ENTRY(min_cell_per_queue), /* reserve a min number of cells for queue */ \
SHR_G_ENTRY(gphy),            /* Built-in GPhy Support                 */ \
SHR_G_ENTRY(cpu_bp_toggle),   /* CMICm backpressure toggle             */ \
SHR_G_ENTRY(ipmc_reduced_table_size), /* IPMC with reduced table size */ \
SHR_G_ENTRY(mmu_reduced_internal_buffer), /* Recuded MMU internal packet buffer */\
SHR_G_ENTRY(ipmc_unicast), /* Support for IPMC with unicast MAC-DA */\
SHR_G_ENTRY(l3_256_defip_table), /* Route table sizing for certain device SKUs */\
SHR_G_ENTRY(l3_32k_defip_table), /* Route table sizing for 32k indexed defip device */\
SHR_G_ENTRY(no_vrf_stats), /* VRF Stats not available on device. E.g. Ranger/Ranger+ */\
SHR_G_ENTRY(mmu_packing), /* MMU buffer packing */\
SHR_G_ENTRY(mmu_hw_flush), /* MMU Hardware Flush */\
SHR_G_ENTRY(l3_shared_defip_table), /* lpm table sharing between 128b and V4, 64b entries */\
SHR_G_ENTRY(l3_nh_attr_table), /* A separate attribute table to hold DLB adjustments and L3 MTU */\
SHR_G_ENTRY(fcoe),            /* fiber channel over ethernet          */ \
SHR_G_ENTRY(system_reserved_vlan), /* Supports System Reserved VLAN */ \
SHR_G_ENTRY(ipmc_remap),      /* Supports IPMC remapping */ \
SHR_G_ENTRY(proxy_port_property), /* Allow configuration of per-source port LPORT properties. */ \
SHR_G_ENTRY(multiple_split_horizon_group), /* multiple split horizon group support */ \
SHR_G_ENTRY(reserve_shg_network_port), /* Reserve split horizon group (1) for network port by default */ \
SHR_G_ENTRY(uc),              /* Embedded core(s) for applications     */ \
SHR_G_ENTRY(uc_mhost),        /* Embedded core(s) - iProc mHost        */ \
SHR_G_ENTRY(overlaid_address_class), /* Overlaid address class support (With over lay PRI bits)*/ \
SHR_G_ENTRY(special_egr_ip_tunnel_ser), /* EGR_IP_TUNNEL overlay memory ser support for older devices */ \
SHR_G_ENTRY(mirror_cos),      /* Mirror UC/MC COS controls */ \
SHR_G_ENTRY(lltag),           /* The out most tag as the LLTAG */\
SHR_G_ENTRY(mem_parity_eccmask), /* memory fields of Parity and ECC masking  */ \
SHR_G_ENTRY(wred_drop_counter_per_port), /* Per port WRED dropped counter  */ \
SHR_G_ENTRY(l2_no_vfi), /* No VFI support */ \
SHR_G_ENTRY(l3mc_use_egress_next_hop), /* L3MC egress logic will use egress next hop for replication */ \
SHR_G_ENTRY(field_action_pfc_class), /* Replace the internal priority of PFC class */ \
SHR_G_ENTRY(fifo_dma_hu2), /* use the hu2 driver fucntion to handle the fifo dma */ \
SHR_G_ENTRY(eee_bb_mode), /* EEE burst and batch mode */ \
SHR_G_ENTRY(cmicd_v2),           /* CMICd v2 */ \
SHR_G_ENTRY(int_common_init),    /* internal function _bcm_common_init */ \
SHR_G_ENTRY(inner_tpid_enable), /* inner tpid enable to detect itag */\
SHR_G_ENTRY(counter_eviction), /* Counter eviction */\
SHR_G_ENTRY(l3_no_ecmp), /* no ECMP */ \
SHR_G_ENTRY(no_tunnel), /* no tunnel */ \
SHR_G_ENTRY(ecn_wred),    /* ECN-WRED architecture */ \
SHR_G_ENTRY(ipmc_use_configured_dest_mac),	/* IPMC configure multicast mac address  */ \
SHR_G_ENTRY(core_clock_300m),    /* system core clock is 300 Mhz */ \
SHR_G_ENTRY(l3_lpm_scaling_enable), /* Allows 64b entries to be added in the paired TCAM*/\
SHR_G_ENTRY(l3_lpm_128b_entries_reserved), /* Paired TCAM space is not reserved */\
SHR_G_ENTRY(hg_no_speed_change),    /* HiGig encap setup without speed modification */ \
SHR_G_ENTRY(mac_based_vlan),       /* MAC based VLAN support                 */ \
SHR_G_ENTRY(esm_correction),     /* Recovery for esm when esm fatal error detected */ \
SHR_G_ENTRY(l3_defip_advanced_lookup),  /* both DIP and SIP lookup using single DEFIP entry */ \
SHR_G_ENTRY(ip_subnet_based_vlan), /* IP subnet based VLAN support   */ \
SHR_G_ENTRY(portmod), /* MAC and PHY managed trough Portmod  */ \
SHR_G_ENTRY(fabric_cell_pcp), /* packet cell packing  */ \
SHR_G_ENTRY(fe_mc_id_range),  /* FE multicast id and mode */ \
SHR_G_ENTRY(fe_mc_priority_mode_enable), /* FE multicast cell priority mode */ \
SHR_G_ENTRY(round_robin_load_balance), /* Round Robin Load Balance for HG Trunk and LAG */ \
SHR_G_ENTRY(randomized_load_balance), /* Randomized Load Balance for HG Trunk and LAG */ \
SHR_G_ENTRY(asymmetric_dual_modid), /* Asymmetric dual module ID support  */ \
SHR_G_ENTRY(mim_reserve_default_port), /* reserve default svp port for MiM  */ \
SHR_G_ENTRY(mim_peer_sharing), /* Support for sharing a peer mim port */ \
SHR_G_ENTRY(tsce), /* TSC Eagle */ \
SHR_G_ENTRY(tscf), /* TSC Falcon */ \
SHR_G_ENTRY(hr2_dual_hash), /* HR2 series including GH style dual hash, \
                               used to handle the difference with other chips */\
SHR_G_ENTRY(l2_hw_aging_bug), /* L2 HW aging bug (BCM56850_A0/A1/A2) */ \
SHR_G_ENTRY(flex_hashing), /* Flexible hashing support */ \
SHR_G_ENTRY(udf_hashing), /* UDF hashing support */ \
SHR_G_ENTRY(visibility), /* Instrumentation: visibility support */ \
SHR_G_ENTRY(l3_ecmp_2k_groups),  /* L3 ECMP 2K Groups                  */ \
SHR_G_ENTRY(ecmp_resilient_hash_optimized), /* ECMP RH where RH group is in member table */ \
SHR_G_ENTRY(ecmp_random), /* Randomized Load Balancing for ECMP */ \
SHR_G_ENTRY(ecmp_round_robin), /* Round Robin Load Balancing for ECMP */ \
SHR_G_ENTRY(asf_multimode),   /* Multimode ASF (Cutthrough)  */ \
SHR_G_ENTRY(led_cmicd_v2),    /* LED processor support in CMICd v2 */ \
SHR_G_ENTRY(epc_linkflap_recover), /* WAR for EPC_LINK_BMP modport issue - SDK-55975*/ \
SHR_G_ENTRY(timesync_timestampingmode), /* 32bit or 48bit timestamping mode for 1588 packets*/ \
SHR_G_ENTRY(trill_ttl),  /* TTL control for TRILL Rbridge*/ \
SHR_G_ENTRY(oam_lookup_bypass_cpu_tx_packets), /*Bypass OAM look up for CPU generated packets */ \
SHR_G_ENTRY(unique_acc_type_access), /* unique acc_type = pipe_num */ \
SHR_G_ENTRY(memscan_ifp_slice_adjust), /* memscan will do idx adjustments for narrow, wide ifp_slice */ \
SHR_G_ENTRY(robust_hash), /* Robust Hash support */ \
SHR_G_ENTRY(extended_hash), /* Extended hash tables support (BCM56850|BCM56960) */ \
SHR_G_ENTRY(cport_clmac), /*Chip uses a CLMAC in a CPORT block*/ \
SHR_G_ENTRY(td2p_multi_modid), /* TD2+ supports multiple modids*/ \
SHR_G_ENTRY(vlan_vfi_membership), /* TD2+ has new vlan vfi membership tables*/ \
SHR_G_ENTRY(vlan_filter), /* TD2+ has EN_IFILTER and EN_EFILTER fields per vlan */ \
SHR_G_ENTRY(l3_egr_intf_zero_invalid), /* On TD2+, l3 egress intf index 0 is invalid */ \
SHR_G_ENTRY(egr_vlan_control_is_memory), /* On TD2+, egr_vlan_control_1/2/3 are memories, not registers */ \
SHR_G_ENTRY(meter_control_is_memory), /* On TD2+, meter related egr_counter_control/egr_shaping_control are memories, not registers */ \
SHR_G_ENTRY(shaper_control_is_memory), /* On TD2+, shaper related egr_shaping_control is memory, not register */ \
SHR_G_ENTRY(td2p_style_egr_outer_tpid), /* On TD2+, egr_port_1 is a memory */ \
SHR_G_ENTRY(td2p_dvp_mirroring), /* On TD2+, EGR_MIRROR_ENABLE moved from ING_DVP_TABLE to ING_DVP_2_TABLE */ \
SHR_G_ENTRY(my_station_2), /* TD2+ has MY_STATION_TCAM_2*/ \
SHR_G_ENTRY(ingress_dest_port_enable), /* Ingress pipeline last stage packet block */ \
SHR_G_ENTRY(llfc_force_xon), /* TD2+ and Tomahawk must set llfc xon */ \
SHR_G_ENTRY(parity_injection_l2), /* TD2+ L2 parity */ \
SHR_G_ENTRY(td2_l2gre), /* Td2+/Td2/TH has use the same tabel for l2gre */ \
SHR_G_ENTRY(egr_modport_to_nhi), /* td2p maps global port to nexthop by EGR_DGPP_TO_NHIm */ \
SHR_G_ENTRY(rtag7_port_profile), /* Td2+ get rtag7 profile index from PORT_TAB */ \
SHR_G_ENTRY(egr_l3_next_hop_next_ptr), /* Td2+ has its own style EGR_L3_NEXT_HOP */ \
SHR_G_ENTRY(ing_l3_next_hop_encoded_dest), /* Td2+ has different encodings for different destinations` */ \
SHR_G_ENTRY(td2p_mpls_linear_protection), /* TD2+ 1:1 and 1+1 protection switching */ \
SHR_G_ENTRY(vfi_zero_invalid), /* VFI value 0 is invalid on Tomahawk and TD2+ */ \
SHR_G_ENTRY(ifp_redirect_to_dvp), /* ING_DVP_2 table to be read when IFP redirects a packet to a DVP */ \
SHR_G_ENTRY(tcam_shift), /*TD2+ only*/ \
SHR_G_ENTRY(hgproxy_subtag_coe),   /* Enhanced support SubTag (Third VLAN tag) subports */ \
SHR_G_ENTRY(enable_lp), /*TD2+ enabling low power lookups */ \
SHR_G_ENTRY(shared_bank_lp_disabled), /* Low power mode disabled for shared banks. */ \
SHR_G_ENTRY(pgw_mac_control_frame), /*TD2+ use PGW_MAC_RSV_MASK instead of CLMAC_RX_CTRL.RX_PASS_CTRL */ \
SHR_G_ENTRY(pgw_mac_pfc_frame), /*TD2+ use PGW_MAC_RSV_MASK instead of CLMAC_PFC_CTRL.RX_PASS_PFC */ \
SHR_G_ENTRY(mpls_lsr_ecmp), /*  MPLS LSR ECMP*/ \
SHR_G_ENTRY(avs), /* AVS closeloop algorithm support */ \
SHR_G_ENTRY(avs_openloop), /* AVS openloop algorithm support */ \
SHR_G_ENTRY(src_port_consistency_check_skip), /* Don't perform SRC port consistency check*/ \
SHR_G_ENTRY(hg_proxy_second_pass), /*TD2+ uses reserved DGLP to trigger loopback processing.*/ \
SHR_G_ENTRY(l2_entry_used_as_my_station), /*TD2+ use L2_ENTRY and L2_USER_ENTRY as my station */ \
SHR_G_ENTRY(egr_ipmc_cfg2_is_memory), /* EGR_IPMC_CFG2 register is a memory on TD2+ */ \
SHR_G_ENTRY(vrf_aware_vxlan_termination), /*TD2+ support VRF aware vxlan termination */ \
SHR_G_ENTRY(mbist), /* MBIST support */ \
SHR_G_ENTRY(ecmp_failover), /* ECMP failover support */ \
SHR_G_ENTRY(ukernel_debug), /* ukernel debugging software support */ \
SHR_G_ENTRY(sflow_ing_mem), /* Ingress Sflow data table support */ \
SHR_G_ENTRY(hierarchical_ecmp), /* Hierarchical ECMP support */ \
SHR_G_ENTRY(udf_support), /* UDF Support */ \
SHR_G_ENTRY(vp_sharing), /* TD2+ supports shared VP across multiple VFIs */ \
SHR_G_ENTRY(nexthop_share_dvp), /* TD2 supports shared network DVP across multiple nexthops */ \
SHR_G_ENTRY(vfi_from_vlan_tables), /*TD2+ supports VFI assignment from VLAN, VFP and VLAN_XLATE table */ \
SHR_G_ENTRY(egr_vlan_xlate_key_on_dvp), /*TD2+ supports egress vlan translation key generation on DVP */ \
SHR_G_ENTRY(ing_vlan_xlate_second_lookup), /*TD2+ supports two ingress vlan translation lookups */ \
SHR_G_ENTRY(egr_vlan_xlate_second_lookup), /* Apache supports two egress vlan translation lookups */ \
SHR_G_ENTRY(egr_vxlate_supports_dgpp), /* Apache supports DGPP based entries in EGR_VLAN_XLATE table*/ \
SHR_G_ENTRY(mpls_entry_second_label_lookup), /* Apache supports two mpls entry lookups */ \
SHR_G_ENTRY(multi_next_hops_on_port), /* TD2+ multiple next hops on local port */ \
SHR_G_ENTRY(use_flex_ctr_oam_lm), /* TD2Plus use Flex counters for OAM LM Measurement */ \
SHR_G_ENTRY(ipmc_to_l2mc_table_size_2_to_3), /* TD2+ supports 8kL2MC groups and 16kIPMC groups, L2MC table size 24K*/ \
SHR_G_ENTRY(reserve_vp_lag_resource_index_zero), /* Reserve entry 0 of VP LAG related resource */ \
SHR_G_ENTRY(turbo_boot), /* Feature that speeds up initialization */ \
SHR_G_ENTRY(oob_fc), /* OOB Flow Control */ \
SHR_G_ENTRY(oob_stats), /* OOB Stats */ \
SHR_G_ENTRY(fp_based_oam), /* OAM support based on FP */ \
SHR_G_ENTRY(uc_ccm), /* UC based OAM CCM Engine */ \
SHR_G_ENTRY(mim_bvid_insertion_control), /* MIM BVID insertion controlled by EGR_L3_NEXT_HOP.BVID_VALID */ \
SHR_G_ENTRY(td2p_mpls_entropy_label), /* TD2+ supports MPLS entropy label identifier at TOS*/ \
SHR_G_ENTRY(reset_xlport_block_tsc12), /* reset XLPORT block in TSC12 in 100G config. */ \
SHR_G_ENTRY(egress_lport), /* Egress Lport support */ \
SHR_G_ENTRY(same_vlan_pruning_override), /* Same VLAN Pruning override */ \
SHR_G_ENTRY(separate_ing_lport_rtag7_profile), /* The Ingress LPORT and RTAG7 profiling is done separately */ \
SHR_G_ENTRY(egr_lport_tab_profile), /* Egress LPORT profiling support */ \
SHR_G_ENTRY(misc_i2e_hgclass_combo_profile), /* Misc combined profiling for I2E/HG_CLASSID_SELECT */ \
SHR_G_ENTRY(egr_qos_combo_profile), /* Egress QOS DSCP and PRI_CNG_MAP profiling together */ \
SHR_G_ENTRY(riot), /* Routing Into and Out of Tunnels */ \
SHR_G_ENTRY(vfi_profile), /* VFI Profile */ \
SHR_G_ENTRY(exact_width_match_mode), /* Exact width match mode for lookup */ \
SHR_G_ENTRY(oam_service_pri_map), /* QOS Map infrastructure for adding map in ING/EGR_SERVICE_PRI_MAP_0/1/2 tables */ \
SHR_G_ENTRY(td2p_ovstb_toggle), /* TD2+ execute the OVST sequence */ \
SHR_G_ENTRY(extended_view_no_trunk_support), /* Trunks or Lags not supported in extended view */ \
SHR_G_ENTRY(key_type_valid_on_vp_vlan_membership), /* Key type is valid on ing/egr_vp_vlan_membership */ \
SHR_G_ENTRY(prigrp_to_obm_prio_map), /* Use the Priority Group (PG) Mapping as OBM Priority Map*/ \
SHR_G_ENTRY(sflow_flex),    /*  Flex sFlow support */ \
SHR_G_ENTRY(td2p_fp_sw_war), /* TD2+ A0 FP SW WAR */ \
SHR_G_ENTRY(td2p_a0_sw_war), /* TD2+ A0 SW WAR */ \
SHR_G_ENTRY(advanced_flex_counter_dual_pipe), /* TD2/TD2+/TT2 use advanced flex dual counter memory */ \
SHR_G_ENTRY(sflow_ing_mirror),    /*  Support sflow sample packet ingress mirroring */ \
SHR_G_ENTRY(l3_iif_under_4k), /* l3_iif table size under 4096 */ \
SHR_G_ENTRY(ipmc_l2_use_vlan_vpn),    /* Devices which provide vlan based lookup for IPMC in L3 MCAST ENTRY */ \
SHR_G_ENTRY(ipmc_defip),    /* Lookup defip table when L3_ENTRY table lookup failed for IP MC pkt */ \
SHR_G_ENTRY(fastlag), /* TH supports only Fast LAG in Low Latency Modes */ \
SHR_G_ENTRY(l3_max_2_defip_tcams), /* Only 2 L3_DEFIP tcams present */ \
SHR_G_ENTRY(oam_pm), /* Performance monitoring support for LM DM apps */\
SHR_G_ENTRY(faults_multi_ep_get), /* OAM faults get for all Endpoints */\
SHR_G_ENTRY(sflow_counters), /* sFlow counters support */ \
SHR_G_ENTRY(nh_for_ifp_actions), /* All the IP and non-IP packets will be redirected to the given next hop index if the FP rule hits. */ \
SHR_G_ENTRY(pause_control_update), /* PAUSE Control value updated, to prevent packet loss */ \
SHR_G_ENTRY(egress_failover),    /*egress protection switching for multicast/broadcast/DLF traffic using flood and prune in TD2+*/ \
SHR_G_ENTRY(field_exact_match_support), /* FP per-pipe exact match stage supported. */ \
SHR_G_ENTRY(flexport_based_speed_set), /* Use flexport operation for speed change, where supported, when MAC/PHY
                                          ability masks do not support the requested speed change */ \
SHR_G_ENTRY(udf_multi_pipe_support), /* UDF per-pipe configuration model supported. */\
SHR_G_ENTRY(fast_ecmp), /* Fast ECMP */ \
SHR_G_ENTRY(multi_level_ecmp), /* Multi-level ECMP support */ \
SHR_G_ENTRY(miml),  /* Mac-in-Mac Lite (MiML) support */ \
SHR_G_ENTRY(flowcnt),       /* flow counter */ \
SHR_G_ENTRY(custom_header),  /* 32-bits custom header support */ \
SHR_G_ENTRY(ing_capwap_parser),  /* Ingress CAPWAP parser support */ \
SHR_G_ENTRY(agm), /* Aggregation Monitor */ \
SHR_G_ENTRY(flex_counter_opaque_stat_id),  /* Flex counter opaque Stat ID */ \
SHR_G_ENTRY(field_quarter_slice_single_tcam), /* Support for Quarter Slice in Chips where there is only 
                                               * one tcam per slice */ \
SHR_G_ENTRY(field_half_slice_single_tcam), /* Support for Half Slice in Chips where there is only 
                                               * one tcam per slice */ \
SHR_G_ENTRY(th_a0_sw_war), /* Feature used to enable all SW WARs for TH A0 */ \
SHR_G_ENTRY(time_synce_divisor_setting), /* SyncE Divisor Setting */ \
SHR_G_ENTRY(pfc_deadlock), /* PFC Deadclock Detection and Recovery feature */ \
SHR_G_ENTRY(alpm), /* ALPM feature */ \
SHR_G_ENTRY(alpm_flex_stat), /* ALPM flex counter feature */ \
SHR_G_ENTRY(alpm_flex_stat_v6_64), /* ALPM v6_64 flex counter WAR */ \
SHR_G_ENTRY(trust_outer_dot1p), /*trust outer dot1p */ \
SHR_G_ENTRY(gh_style_pfc_config), /* GH style PFC configuration */ \
SHR_G_ENTRY(mpls_5_label_parsing), /* MPLS 5 label parsing */ \
SHR_G_ENTRY(egr_ipmc_mem_field_l3_payload_valid), /* egress ipmc mem field l3_payload valid */ \
SHR_G_ENTRY(centralized_counter), /* Support for Centralized counter model */ \
SHR_G_ENTRY(l3_2k_defip_table), /* Route table size for 56832 SKU 
                                   since it has only 2 TCAMs */ \
SHR_G_ENTRY(egr_mmu_cell_credit_is_memory), /* EGR_MMU_CELL_CREDIT is a memory instead of register on TH2 */ \
SHR_G_ENTRY(mmu_sed), /* New block is introduced in TH2 for MMU Slice Control Enqueue Dequeue (SED) */ \
SHR_G_ENTRY(ing_trill_adjacency_is_memory), /* ING_TRILL_ADJACENCY is a memory instead of register on TH2 */ \
SHR_G_ENTRY(clmac_16byte_interface_mode), /* CLMAC supports 16 Byte interface mode */ \
SHR_G_ENTRY(pm_refclk_master), /* port macros act as the refclk master */ \
SHR_G_ENTRY(mpls_ecn),  /* mpls ecn feature */ \
SHR_G_ENTRY(xgs5_flexport_legacy_mode), /* xgs5 implementation for legacy flexport APIs */ \
SHR_G_ENTRY(cmicd_v4), /* CMICd V4 */ \
SHR_G_ENTRY(led_cmicd_v4), /* LED processor support in CMICd v4 */ \
SHR_G_ENTRY(mor_dma), /* Support for Multiple Outstanding Request DMA */ \
SHR_G_ENTRY(mpls_segment_routing), /* Segment routing support on Apache */ \
SHR_G_ENTRY(ep_redirect_v2), /* EP Redirect V2 */ \
SHR_G_ENTRY(flex_ctr_mpls_3_label_count), /* 3 Label MPLS counting support on Apache */ \
SHR_G_ENTRY(hierarchical_protection), /* hierarchical protection support on Apache */ \
SHR_G_ENTRY(olp), /* Device supports OLP */ \
SHR_G_ENTRY(cpu_as_olp), /* Device supports configuring cpu port as olp port*/ \
SHR_G_ENTRY(deprecated_api), /* For APIs need to be deprecated */ \
SHR_G_ENTRY(fp_nw_tcam_prio_order_war), /* Feature used to enable SW WAR for adjusting narrow wide TCAM indices in priority order. */ \
SHR_G_ENTRY(field_qual_vlanformat_reverse), /* The position of outer/inner tagged bits are reverse */ \
SHR_G_ENTRY(cpureg_dump), /* Use reg info to dump cpureg */ \
SHR_G_ENTRY(mirror_four_port_trunk), /* Mirror supports upto 4 ports in trunk group */ \
SHR_G_ENTRY(field_compression), /*FP compression supported. */\
SHR_G_ENTRY(fast_egr_vlan_action), /* Egress VLAN action for low latency mode */ \
SHR_G_ENTRY(vlan_vfi), /* Vlan Vfi */ \
SHR_G_ENTRY(egr_vlan_action), /* Egress VLAN action for balanced mode */ \
SHR_G_ENTRY(clmac_partial_support), /* Only few CPORTs support CLMAC. */ \
SHR_G_ENTRY(hr3_switch_encap_index_shift2_war), /* Feature used to enable SW WAR for
                                   adjusting EGR_HEADER_ENCAP_DATA indices are shifted << 2 bits. */ \
SHR_G_ENTRY(mpls_exp_to_phb_cng_map), /* MPLS EXP to PHB,CNG Map tables */ \
SHR_G_ENTRY(mmu_hqos_four_level), /*Four level Hqos suppport for Apache */\
SHR_G_ENTRY(management_port_lanes), /* Number of lanes for Management port */ \
SHR_G_ENTRY(fp_oam_drop_control), /* FP based OAM drop control */ \
SHR_G_ENTRY(rx_pkt_hdr_format_higig2), /* CPU packets recevied from CMIC have higig2 header */ \
SHR_G_ENTRY(mpls_swap_label_preserve), /* mpls swap label is kept intact*/ \
SHR_G_ENTRY(fast_init), /* Fast init for low latency modes */ \
SHR_G_ENTRY(cmicd_v3), /* CMICD V3 */ \
SHR_G_ENTRY(no_l2_remote_trunk), /*No  L2 remote trunk (BCM56560_A0/BCM56860_A0) */ \
SHR_G_ENTRY(dump_socmem_uses_dma), /* Feature to enable table dma for dump socmem */ \
SHR_G_ENTRY(mmu_3k_uc_queue), /*device has 3k ucq */\
SHR_G_ENTRY(mmu_1k_uc_queue), /*device has 1k ucq */\
SHR_G_ENTRY(untethered_otp), /* Untethred Otp */ \
SHR_G_ENTRY(uc_ulink), /* Ucore maintains link status */ \
SHR_G_ENTRY(nh_ifp_action_src_dst_mac_swap), /* Swap source/destination MAC for outgoing packets, for IFP application only. */ \
SHR_G_ENTRY(ignore_mem_write_nak), /* war to avoid the unexpected mem_write nak */ \
SHR_G_ENTRY(ingress_failover), /* Support ingress protection switching */ \
SHR_G_ENTRY(l2_use_l3_class_id), /* Enable L3 Lookup to get classids based on DIP & SIP for L2 Switched packets. */ \
SHR_G_ENTRY(field_tcam_hitless_update_on_modify), /* Hitless update of tcams on modify */ \
SHR_G_ENTRY(extended_address_class), /*extended address class id*/ \
SHR_G_ENTRY(unimac_reset_wo_clock), /* Unimac sync reset without clock */ \
SHR_G_ENTRY(internal_phy_link_check), /* checking internal phy link status when 
                                         external phy link up is detected */ \
SHR_G_ENTRY(ing_l2tunnel_tpid_parser), /*Global Inner TPID of L2 Tunnel payload */ \
SHR_G_ENTRY(l2_tunnel_transit_payload_tpid_parser), /* Matched TPID of Transit MIM/VXLAN/L2GRE Payload for Parser */ \
SHR_G_ENTRY(field_udf_offset_hg_114B),   /* HG+ header is located at Byte 114-125 in UDF */ \
SHR_G_ENTRY(field_udf_offset_hg2_110B),  /* HG2 header is located at Byte 110-125 in UDF */ \
SHR_G_ENTRY(tcb),             /* support tcb */ \
SHR_G_ENTRY(ecmp_hash_bit_count_select),  /* support to select lower bits of 16 bits hash value */ \
SHR_G_ENTRY(cxl_mib), /* separate mib counter for different port types */ \
SHR_G_ENTRY(pipe2_pipe3_disabled), /* Pipe2/Pipe3 are disabled */ \
SHR_G_ENTRY(field_exact_match), /* Field exact match */ \
SHR_G_ENTRY(efp), /* EFP enabled */ \
SHR_G_ENTRY(ifp), /* IFP enabled */ \
SHR_G_ENTRY(vfp), /* VFP enabled */ \
SHR_G_ENTRY(lpm), /* LPM enabled */ \
SHR_G_ENTRY(uft_bank_0), /* UFT bank 0 enabled */ \
SHR_G_ENTRY(uft_bank_1), /* UFT bank 1 enabled */ \
SHR_G_ENTRY(uft_bank_2), /* UFT bank 2 enabled */ \
SHR_G_ENTRY(uft_bank_3), /* UFT bank 3 enabled */ \
SHR_G_ENTRY(mpls_frr), /* MPLS fast reroute */ \
SHR_G_ENTRY(dynamic_load_balancing), /* DLB enabled */ \
SHR_G_ENTRY(dynamic_alternate_path), /* Enable dynamic alternate path */ \
SHR_G_ENTRY(timestamp), /* Timestamp enabled */ \
SHR_G_ENTRY(field_multi_pipe_enhanced),  /* TH2 enhanced Field features */ \
SHR_G_ENTRY(miml_no_l3),  /* Mac-in-Mac Lite (MiML) support without L3 feature */ \
SHR_G_ENTRY(l3_tunnel_mpls_frr),         /*To check l3 tunnel mpls frr lookup in apache*/ \
SHR_G_ENTRY(l3_ecmp_4k_groups),  /* L3 ECMP 4K Groups                  */ \
SHR_G_ENTRY(esm_l2_lookup_for_ip6),         /*Enable ESM L2 lookup for IPv6 packets*/ \
SHR_G_ENTRY(flex_stat_ing_pools_4), /* Ingress Flexible Counters with 4 pools */ \
SHR_G_ENTRY(hg_trunk_groups_max_2), /* Max Number of HiGig Trunk Groups is 2 */\
SHR_G_ENTRY(hg_trunk_group_members_max_4),/* HiGig Trunk Group with maximum of 4 Members*/\
SHR_G_ENTRY(dvp_2_config_required),         /*Required to configure ING_DVP_2_TABLE*/ \
SHR_G_ENTRY(xmac_reset_on_mode_change),   /* Reset xmac on mode change */ \
SHR_G_ENTRY(forwarding_db_no_support), /* forwarding database id-no support*/ \
SHR_G_ENTRY(xlportb0),         /* XLPORT B0 for PM4x10 in Apache A0 */ \
SHR_G_ENTRY(clport_gen2),      /* CLPORT GEN2 for PM4x25 in Apache B00 */ \
SHR_G_ENTRY(l2_drop_mc_mac), /* L2 discard traffic with multicast destination */ \
SHR_G_ENTRY(shared_defip_stat_support), /* Statistics support on shared defip tables */ \
SHR_G_ENTRY(mpls_xgs5_nw_port_match), /* match mpls nw port to see if duplicate entry exists */ \
SHR_G_ENTRY(mirror_ing_egr_single), /* restrict ingress/egress mirror on same port */ \
SHR_G_ENTRY(dvp_group_pruning),     /* Dvp group based pruning */ \
SHR_G_ENTRY(hr3_lite_lpm_shadow_hit), /* HR3-Lite's LPM shadow hit(LPM entry in range of 32~63 for LPM hit operation will be occurre on LPM entry in 512~543 */ \
SHR_G_ENTRY(l3_kt2_shared_defip_table), /* kt2 shared defip support */ \
SHR_G_ENTRY(eee_stat_clear_directly), /* Clear EEE statistics by writing EEE counter/register directly */ \
SHR_G_ENTRY(vc_and_swap_table_overlaid), /* vc and swap label tabe overlaid by egr_dvp_attribute_1*/ \
SHR_G_ENTRY(field_action_l3_route_disabled), /* L3 routing base on FP action is disabled */ \
SHR_G_ENTRY(th_tflow), /* Tomahawk Random sampling */ \
SHR_G_ENTRY(th_fp_ctc_manipulate),  /* To support CTC Manipulate SW WAR for TH Field Module. */ \
SHR_G_ENTRY(th_pkt_trace_sw_war), /* SW workaround for TH packet trace. */ \
SHR_G_ENTRY(pktpri_as_dot1p), /* Treat packet priority as dot1p. */ \
SHR_G_ENTRY(th_nh_no_overlap_ecmp), /* TH alloc NHI without overlapping ECMP indexes. */ \
SHR_G_ENTRY(trill_egr_dvp_classid), /* Class ID assignment for TRILL flows using EGR_DVP_ATTRIBUTE_1 table */ \
SHR_G_ENTRY(egr_nh_class_id_valid), /* feature check to read class id from egr nh only for valid devices */ \
SHR_G_ENTRY(cosq_hol_drop_packet_count), /* Per COSQ HOL drop packet count. */ \
SHR_G_ENTRY(hg_proxy_module_config), /* Module configurations for HG-Proxy and Two-pass loopback applications */ \
SHR_G_ENTRY(l3_tunnel_expanded_mod_mask), /* 8 bit mod mask for l3 tunnel term */ \
SHR_G_ENTRY(fp_qual_recovery_with_group_create), /* Group Qualifier offset recovery based on dummy group create */ \
SHR_G_ENTRY(linkscan_continuous), /* Perform MDIO write/read operation without stalling active linkscan operation */ \
SHR_G_ENTRY(vlan_egress_membership_l3_only), /* Egress VLAN membership check for L3 flow only */ \
SHR_G_ENTRY(egr_ip_tnl_mpls_double_wide), /* Apache variants support double wide EGR_IP_TUNNEL_MPLS */ \
SHR_G_ENTRY(mpls_lm_dm), /* Device supports MPLS_LM_DM App */ \
SHR_G_ENTRY(lls_port_mema_config_match), /* ES_PIPE0/1_LLS_PORT_MEMA/B_CONFIG table size defined in regsfile match with actual physical memory size */ \
SHR_G_ENTRY(link_status_get_need_phy_read_twice), /* In legacy devices, PHY status register may show wrong status on first read and we need to confirm by reading second time*/ \
SHR_G_ENTRY(pm_fc_merge_mode), /* Merge mode should be enabled in PM FC config */ \
SHR_G_ENTRY(egress_object_mac_da_replace), /* When MPLS egress object is replaced/converted to L3 object using BCM_L3_REPLACE flag, MAC DA in the L3 object will be configred with new value instead of using MAC DA from MPLS egress object */ \
SHR_G_ENTRY(lpm_atomic_write), /* In order to provide atomicity during LPM entry updates (add or delete). */ \
SHR_G_ENTRY(gh2_my_station),	/* GH2 style MY_STATION_TCAM support */ \
SHR_G_ENTRY(gh2_rtag7),	/* RTAG7 support for GH2 */ \
SHR_G_ENTRY(high_portcount_register),	/* pbmp register is splited to two more reg due to 
                                            high port count(over 64 bits) */ \
SHR_G_ENTRY(preemption), /* TSN preemption  */ \
SHR_G_ENTRY(preemption_cnt), /* TSN preemption counter */ \
SHR_G_ENTRY(tas), /* TSN Time Aware Scheduling  */ \
SHR_G_ENTRY(preempt_mib_support), /* Preempt MIB counters support  */ \
SHR_G_ENTRY(clmib_support), /* CLMIB counters support for CL port */ \
SHR_G_ENTRY(hg2_light_in_portmacro), /* to support HG2-Lite in PortMacro */ \
SHR_G_ENTRY(vxlan_lite), /* L2-VPN over UDP Tunnels Lite support */ \
SHR_G_ENTRY(tsn), /* Time-Sensitive Networking (TSN) */ \
SHR_G_ENTRY(tsn_mtu_stu), /* TSN MTU/STU size check */ \
SHR_G_ENTRY(tsn_sr), /* TSN Seamless Redundancy (SR) support */ \
SHR_G_ENTRY(tsn_sr_hsr), /* TSN SR High-availability Seamless Redundancy (HSR) support */ \
SHR_G_ENTRY(tsn_sr_prp), /* TSN SR Parallel Redundancy Protocol (PRP) support */ \
SHR_G_ENTRY(tsn_sr_802_1cb), /* TSN SR IEEE 802.1CB support */ \
SHR_G_ENTRY(tsn_sr_auto_learn), /* TSN SR automatic flow learning support */ \
SHR_G_ENTRY(rroce), /* Routable RDMA over Converged Ethernet (RRoCE) */ \
SHR_G_ENTRY(discontinuous_pp_port), /* discontinuous pp port feature */ \
SHR_G_ENTRY(port_group_for_ivxlt), /* Port-Group key type for ingress vlan translation */ \
SHR_G_ENTRY(apache_round_robin_fp_lag), /*  Apache Round Robin LAG support */ \
SHR_G_ENTRY(truncate_cpu_copy), /* truncate_cpu_copy support */ \
SHR_G_ENTRY(no_fabric), /* no Fabric */ \
SHR_G_ENTRY(iddq_new_default), /* default value of iddq field changed */ \
SHR_G_ENTRY(no_tdm), /* no TDM */ \
SHR_G_ENTRY(no_sw_rx_los), /* No SOFTWARE_RX_LOS supported.   */ \
SHR_G_ENTRY(cmicm_multi_schan_cmc), /* CMICm schan on multiple cmc         */ \
SHR_G_ENTRY(evt_replace_tpid_only), /* EGR_VLAN_XLATE supports the ability to replace SD tag TPID only */ \
SHR_G_ENTRY(redirect_nh_to_ecmp_for_mpls_php), /* Redirect NH to ECMP for MPLS PHP */ \
SHR_G_ENTRY(sb2plus_1k_ecmp_groups), /*Differentiate Max ECMP group of SB2+(1024) from SB2(128) */ \
SHR_G_ENTRY(inner_vlan_range_check), /* Support the ability to check inner vlan range */ \
SHR_G_ENTRY(l2_cache_use_my_station), /* L2 cache entry configured with BCM_L2_CACHE_L3 will use my_station_tcam table */ \
SHR_G_ENTRY(tcam_scan_engine), /* HW TCAM scan engine */ \
SHR_G_ENTRY(port_leverage), /* Leverage DV port up/down for port enable/disable */\
SHR_G_ENTRY(pstats), /* Packetized mmu Statistic */ \
SHR_G_ENTRY(flexport_no_legacy), /* Don't support legacy flexport API */\
SHR_G_ENTRY(ecmp_1k_paths_4_subgroups), /* 1K members per ECMP group with 4 subgroups having 256 members */ \
SHR_G_ENTRY(ifp_timestamptocpu_optimized), /*IFP COPY-with-TIMESTAMP action doesn't need Drop to be set on TD2/TD2+/TH/TH2*/ \
SHR_G_ENTRY(mpls_nh_ttl_control), /*Control TTL decrement per Next Hop for MPLS flows */\
SHR_G_ENTRY(rescal), /*support rescal*/ \
SHR_G_ENTRY(sw_autoneg), /* Support for SW AUTONEG */ \
SHR_G_ENTRY(vfp_match_on_inner_ip_fields), /*VFP to match on Inner IP fields */ \
SHR_G_ENTRY(td3_style_fp), /* TD3 Field Programming changes */ \
SHR_G_ENTRY(ifp_action_profiling), /* IFP action profiling feature changes */ \
SHR_G_ENTRY(ifp_no_inports_support), /* IFP qualifying on Inports is not supported */ \
SHR_G_ENTRY(no_vlan_vrf), /* VRF is supported but has no VLAN based VRF */ \
SHR_G_ENTRY(half_of_l3_defip_ipv4_capacity), /* disable half of l3_defip ipv4 capacity */ \
SHR_G_ENTRY(count)            /* ALWAYS LAST PLEASE (DO NOT CHANGE)  */

/* Make the enums */
#undef  SHR_G_MAKE_STR
#undef  SHR_G_MAKE_STR1
#undef  SHR_G_MAKE_STR_CONCAT
#define SHR_G_MAKE_STR1(a) a
#define SHR_G_MAKE_STR_CONCAT(e,a) SHR_G_MAKE_STR1(e##a)
#define SHR_G_MAKE_STR(a)  SHR_G_MAKE_STR_CONCAT(soc_feature_,a)
SHR_G_MAKE_ENUM(soc_feature_);

/* Make the string array */
#undef  SHR_G_MAKE_STR
#define SHR_G_MAKE_STR(a)     #a
#define SOC_FEATURE_NAME_INITIALIZER                                        \
        SHR_G_NAME_BEGIN(dont_care)                                         \
        SHR_G_ENTRIES(dont_care)                                          \
        SHR_G_NAME_END(dont_care)

/* Feature-defining functions */
extern int soc_features_bcm5670_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm5673_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm5690_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm5665_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm5695_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm5675_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm5674_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm5665_b0(int unit, soc_feature_t feature);
extern int soc_features_bcm56601_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm56601_b0(int unit, soc_feature_t feature);
extern int soc_features_bcm56601_c0(int unit, soc_feature_t feature);
extern int soc_features_bcm56602_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm56602_b0(int unit, soc_feature_t feature);
extern int soc_features_bcm56602_c0(int unit, soc_feature_t feature);
extern int soc_features_bcm56504_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm56504_b0(int unit, soc_feature_t feature);
extern int soc_features_bcm56102_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm56304_b0(int unit, soc_feature_t feature);
extern int soc_features_bcm56112_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm56314_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm5650_c0(int unit, soc_feature_t feature);
extern int soc_features_bcm56800_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm56218_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm56514_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm56624_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm56624_b0(int unit, soc_feature_t feature);
extern int soc_features_bcm56680_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm56680_b0(int unit, soc_feature_t feature);
extern int soc_features_bcm56634_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm56634_b0(int unit, soc_feature_t feature);
extern int soc_features_bcm56524_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm56524_b0(int unit, soc_feature_t feature);
extern int soc_features_bcm56685_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm56685_b0(int unit, soc_feature_t feature);
extern int soc_features_bcm56334_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm56334_b0(int unit, soc_feature_t feature);
extern int soc_features_bcm56840_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm56840_b0(int unit, soc_feature_t feature);
extern int soc_features_bcm56640_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm56640_b0(int unit, soc_feature_t feature);
extern int soc_features_bcm56340_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm56540_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm56850_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm56960_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm56965_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm56965_a1(int unit, soc_feature_t feature);
extern int soc_features_bcm56970_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm5338_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm5338_b0(int unit, soc_feature_t feature);
extern int soc_features_bcm5324_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm5324_a1(int unit, soc_feature_t feature);
extern int soc_features_bcm5380_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm5388_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm5396_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm5389_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm5398_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm5325_a1(int unit, soc_feature_t feature);
extern int soc_features_bcm5348_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm5397_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm5347_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm5395_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm53242_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm56224_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm56224_b0(int unit, soc_feature_t feature);
extern int soc_features_bcm53262_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm56820_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm56725_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm53314_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm53324_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm53115_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm88020_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm88025_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm88030_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm88030_a1(int unit, soc_feature_t feature);
extern int soc_features_bcm88030_b0(int unit, soc_feature_t feature);
extern int soc_features_bcm53118_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm88230_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm88230_b0(int unit, soc_feature_t feature);
extern int soc_features_bcm88230_c0(int unit, soc_feature_t feature);
extern int soc_features_bcm53280_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm53280_b0(int unit, soc_feature_t feature);
extern int soc_features_bcm56142_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm56150_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm53101_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm53125_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm53128_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm53134_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm53134_b0(int unit, soc_feature_t feature);
extern int soc_features_bcm88650_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm2801pm_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm88650_b0(int unit, soc_feature_t feature);
extern int soc_features_bcm88660_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm88675_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm88675_b0(int unit, soc_feature_t feature);
extern int soc_features_bcm88375_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm88375_b0(int unit, soc_feature_t feature);
extern int soc_features_bcm88470_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm88470_b0(int unit, soc_feature_t feature);
extern int soc_features_bcm88270_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm8206_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm88680_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm88690_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm88202_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm88732_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm56440_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm56440_b0(int unit, soc_feature_t feature);
extern int soc_features_bcm56450_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm56450_b0(int unit, soc_feature_t feature);
extern int soc_features_bcm56450_b1(int unit, soc_feature_t feature);
extern int soc_features_bcm56860_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm56560_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm56560_b0(int unit, soc_feature_t feature);
extern int soc_features_bcm53600_a0(int unit, soc_feature_t featrue);
extern int soc_features_bcm89500_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm88750_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm88750_b0(int unit, soc_feature_t feature);
extern int soc_features_bcm88754_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm56260_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm56260_b0(int unit, soc_feature_t feature);
extern int soc_features_bcm56270_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm53460_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm88752_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm88752_b0(int unit, soc_feature_t feature);
extern int soc_features_bcm88950_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm88790_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm83207_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm83208_a0(int unit, soc_feature_t feature);

extern int soc_features_bcm53010_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm53020_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm53400_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm56160_a0(int unit, soc_feature_t feature);
extern int soc_features_bcm53570_a0(int unit, soc_feature_t feature);

extern void soc_feature_init(int unit);
extern char *soc_feature_name[];

#define soc_feature(unit, feature)      SOC_FEATURE_GET(unit, feature)

#endif  /* !_SOC_FEATURE_H */
