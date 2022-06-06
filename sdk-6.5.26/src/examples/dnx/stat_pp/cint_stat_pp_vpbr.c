/*
 * 
 This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 
 Copyright 2007-2022 Broadcom Inc. All rights reserved. File: cint_stat_pp_vpbr.c Purpose: Statistics examples for SDK ingress APIs using an IPV4/6 VPBR application.
 */

/*
 * File: cint_stat_pp_vpbr.c
 * Purpose: Example of ingress statistics with VPBR lookup using lif_wide_data
 *
 * Configure statistics on VPBR lookup in fwd
 *
 * The cint includes:
 * - Main function to configure routing with statistics enabled along the way
 * - Structs with configurations used to configure the statistics profiles (crps and pp)
 * - A struct with ip host and vpbr required configuration
 *
 */

struct cint_stat_pp_ipv4_vpbr_info_s
{
    int in_port;                                        /** incoming port */
    int out_port;                                       /** outgoing port */
    int intf_in;                                        /** Incoming packet ETH-RIF */
    int intf_out;                                       /** Outgoing packet ETH-RIF */
    bcm_mac_t intf_in_mac_address;                      /** mac for in RIF */
    bcm_mac_t intf_out_mac_address;                     /** mac for out RIF */
    bcm_mac_t arp_next_hop_mac;                         /** mac for out RIF */
    int encap_id;                                       /** encap ID */
    bcm_ip_t ipv4_host_vpbr_miss;                       /** IPv4 host for VPBR miss */
    bcm_ip_t ipv4_host_vpbr_hit;                        /** IPv4 host for VPBR hit */
    bcm_ip_t ipv4_host_sip;                             /** IPv4 SIP */
    bcm_ip6_t ipv6_host_vpbr_miss;                      /** IPv6 host for VPBR miss */
    bcm_ip6_t ipv6_host_vpbr_hit;                       /** IPv6 host for VPBR hit */
    bcm_ip6_t ipv6_host_sip;                            /** IPv6 SIP */
    int vrf;                                            /** VRF */
    int new_vrf;                                        /** New VRF */
    bcm_if_t fec_id;                                    /** FEC id */
};

cint_stat_pp_ipv4_vpbr_info_s cint_stat_pp_vpbr_info =
{
    /*
     * ports : in_port | out_port
     */
    200, 201,
    /*
     * intf_in | intf_out
     */
    15, 100,
    /*
     * intf_in_mac_address | intf_out_mac_address | arp_next_hop_mac
     */
    {0x00, 0x0c, 0x00, 0x02, 0x00, 0x00}, {0x00, 0x12, 0x34, 0x56, 0x78, 0x9a}, {0x00, 0x00, 0x00, 0x00, 0xcd, 0x1d},
    /*
     * encap_id
     */
    0x1384,
    /*
     * ipv4_host_vpbr_miss
     */
    0x7fffff02 /** 127.255.255.2 */,
    /*
     * ipv4_host_vpbr_hit
     */
    0x7fffff03 /** 127.255.255.3 */,
    /*
     * ipv4_host_sip
     */
    0xc0800101 /** 192.128.1.1 */,
    /*
     * ipv6_host_vpbr_miss
     */
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x12, 0x34, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x12 } /** 0000:0000:0000:0000:1234:0000:0000:FF12 */,
    /*
     * ipv6_host_vpbr_hit
     */
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x12, 0x34, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x13 } /** 0000:0000:0000:0000:1234:0000:0000:FF13 */,
    /*
     * ipv6_host_sip
     */
    { 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02 } /** 2000:0000:0000:0000:0000:0000:0000:0002 */,
    /*
     * vrf
     */
    1,
    /*
     * new_vrf
     */
    2,
    /*
     * fec_id (set on run time)
     */
    0
};

cint_stat_pp_config_info_s l3_host_statistics_config =
{
        /**database_id */
        7,
        /**crps_engine_id */
        7,
        /**pp_profile */
        7,
        /**counter_command */
        7,
        /**crps_start */
        0,
        /**crps_end */
        800,
        /**type_id*/
        0,
        /**stat_id*/
        1,
        /**expansion_flag*/
        5
};

cint_stat_pp_config_info_s l3_vpbr_statistics_config =
{
        /**database_id */
        8,
        /**crps_engine_id */
        8,
        /**pp_profile */
        8,
        /**counter_command */
        8,
        /**crps_start */
        0,
        /**crps_end */
        800,
        /**type_id*/
        0,
        /**stat_id*/
        2,
        /**expansion_flag*/
        5
};

int
in_port_intf_wide_set(
    int unit,
    int in_port,
    int eth_rif)
{
    bcm_vlan_port_t vlan_port;
    int rc;
    uint64 wide_data;

    bcm_vlan_port_t_init(&vlan_port);
    vlan_port.port = in_port;
    vlan_port.vsi = eth_rif;
    vlan_port.criteria = BCM_VLAN_PORT_MATCH_PORT;
    vlan_port.flags = BCM_VLAN_PORT_CREATE_INGRESS_ONLY | BCM_VLAN_PORT_INGRESS_WIDE;

    rc = bcm_vlan_port_create(unit, vlan_port);
    if (rc != BCM_E_NONE)
    {
        printf("Error, bcm_vlan_port_create\n");
        return rc;
    }

    rc = bcm_vlan_gport_add(unit, eth_rif, in_port, 0);
    if (rc != BCM_E_NONE)
    {
        printf("Error, bcm_vlan_gport_add \n");
        return rc;
    }

    COMPILER_64_ZERO(wide_data);
    COMPILER_64_SET(wide_data, 0x0, 0x0014FFFF);
    rc = bcm_port_wide_data_set(unit, vlan_port.vlan_port_id, BCM_PORT_WIDE_DATA_INGRESS, wide_data);
    if (rc != BCM_E_NONE)
    {
        printf("Error, bcm_port_wide_data_set\n");
        return rc;
    }

    return rc;
}

/*
 * main function.
 * Calls:
 * cint_stat_pp_config_crps()
 * cint_stat_pp_create_pp_profile()
 * Configures routing based on dnx_basic_example
 * LIF wide data is added to the ingress interface port
 * IPv4 and IPv6 host entries to match host statistics
 * IPv4 and IPv6 vpbr entries to match vpbr statistics
 * IPv4 and IPv6 host entries to match vpbr vrf update without statistics
 */
int
cint_stat_pp_ipv4_vpbr_example(
    int unit,
    int in_port,
    int out_port)
{
    int rv;
    bcm_gport_t gport;
    l3_ingress_intf ingress_rif;
    l3_ingress_intf_init(&ingress_rif);
    int encoded_fec;
    bcm_l3_host_t host;
    bcm_l3_vpbr_entry_t vpbr_entry;
    bcm_ip6_t ip6_mask = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

    char *proc_name;

    proc_name = "dnx_stat_pp_ipv4_vpbr_example";

    cint_stat_pp_vpbr_info.in_port = in_port;
    cint_stat_pp_vpbr_info.out_port = out_port;

    /** Configure statistics for l3 host */
    rv = cint_stat_pp_config_main(unit, l3_host_statistics_config, bcmStatCounterInterfaceIngressReceivePp, 0, BCM_CORE_ALL, 0);
    if (rv != BCM_E_NONE)
    {
        printf("cint_stat_pp_config_main failed for l3_host_statistics_config\n");
        return rv;
    }

    /** Configure statistics for l3 vpbr */
    rv = cint_stat_pp_config_main(unit, l3_vpbr_statistics_config, bcmStatCounterInterfaceIngressReceivePp, 0, BCM_CORE_ALL, 0);
    if (rv != BCM_E_NONE)
    {
        printf("cint_stat_pp_config_main failed for l3_vpbr_statistics_config\n");
        return rv;
    }

    rv = get_first_fec_in_range_which_not_in_ecmp_range(unit, 0, &cint_stat_pp_vpbr_info.fec_id);
    if (rv != BCM_E_NONE)
    {
        printf("%s(): Error, get_first_fec_in_range_which_not_in_ecmp_range hierarchy 1\n", proc_name);
        return rv;
    }

    /*
     * 1. Set In-Port to In ETh-RIF
     */
    rv = in_port_intf_wide_set(unit, cint_stat_pp_vpbr_info.in_port, cint_stat_pp_vpbr_info.intf_in);
    if (rv != BCM_E_NONE)
    {
        printf("%s(): Error, in_port_intf_set intf_in in_port %d\n", proc_name, cint_stat_pp_vpbr_info.in_port);
        return rv;
    }
    /*
     * 2. Set Out-Port default properties, in case of ARP+AC no need
     */
    rv = out_port_set(unit, cint_stat_pp_vpbr_info.out_port);
    if (rv != BCM_E_NONE)
    {
        printf("%s(): Error, out_port_set intf_out out_port %d\n", proc_name, cint_stat_pp_vpbr_info.out_port);
        return rv;
    }
    /*
     * 3. Create ETH-RIF and set its properties
     */
    rv = intf_eth_rif_create(unit, cint_stat_pp_vpbr_info.intf_in, cint_stat_pp_vpbr_info.intf_in_mac_address);
    if (rv != BCM_E_NONE)
    {
        printf("%s(): Error, intf_eth_rif_create intf_in %d\n", proc_name, cint_stat_pp_vpbr_info.intf_in);
        return rv;
    }
    rv = intf_eth_rif_create(unit, cint_stat_pp_vpbr_info.intf_out, cint_stat_pp_vpbr_info.intf_out_mac_address);
    if (rv != BCM_E_NONE)
    {
        printf("%s(): Error, intf_eth_rif_create intf_out %d\n", proc_name, cint_stat_pp_vpbr_info.intf_out);
        return rv;
    }
    /*
     * 4. Set Incoming ETH-RIF properties
     */
    ingress_rif.vrf = cint_stat_pp_vpbr_info.vrf;
    ingress_rif.intf_id = cint_stat_pp_vpbr_info.intf_in;
    rv = intf_ingress_rif_set(unit, &ingress_rif);
    if (rv != BCM_E_NONE)
    {
        printf("%s(): Error, intf_ingress_rif_set\n", proc_name);
        return rv;
    }
    /*
     * 5. Create ARP and set its properties
     */
    rv = create_arp_plus_ac_type(unit, 0, cint_stat_pp_vpbr_info.arp_next_hop_mac,
                                 &cint_stat_pp_vpbr_info.encap_id, cint_stat_pp_vpbr_info.intf_out);
    if (rv != BCM_E_NONE)
    {
        printf("%s(): Error, create_arp_plus_ac_type\n", proc_name);
        return rv;
    }

    /*
     * 6. Create FEC and set its properties
     * only in case the host format is not "no-fec"
     */
    if (BCM_GPORT_IS_SET(cint_stat_pp_vpbr_info.out_port))
    {
       gport = cint_stat_pp_vpbr_info.out_port;
    }
    else {
       BCM_GPORT_LOCAL_SET(gport, cint_stat_pp_vpbr_info.out_port);
    }
    rv = l3__egress_only_fec__create_inner(unit, cint_stat_pp_vpbr_info.fec_id, cint_stat_pp_vpbr_info.intf_out,
                                           cint_stat_pp_vpbr_info.encap_id, gport, 0, 0, 0, &encoded_fec);
    if (rv != BCM_E_NONE)
    {
        printf("%s(): Error, create egress object FEC only\n", proc_name);
        return rv;
    }
    printf("%s(): fec 0x%08X encoded_fec 0x%08X.\r\n", proc_name, cint_stat_pp_vpbr_info.fec_id, encoded_fec);

    /*
     * 7. Add host entry
     */
    int _l3_itf;
    BCM_L3_ITF_SET(&_l3_itf, BCM_L3_ITF_TYPE_FEC, cint_stat_pp_vpbr_info.fec_id);
    if (BCM_GPORT_IS_SET(cint_stat_pp_vpbr_info.out_port))
    {
       gport = cint_stat_pp_vpbr_info.out_port;
    }
    else {
       BCM_GPORT_LOCAL_SET(gport, cint_stat_pp_vpbr_info.out_port);
    }

    /** Add ipv4 host entry - vpbr miss */
    bcm_l3_host_t_init(&host);
    host.l3a_ip_addr = cint_stat_pp_vpbr_info.ipv4_host_vpbr_miss;
    host.l3a_vrf = cint_stat_pp_vpbr_info.vrf;
    host.l3a_intf = cint_stat_pp_vpbr_info.fec_id;
    host.stat_id = l3_host_statistics_config.stat_id;
    host.stat_pp_profile = l3_host_statistics_config.pp_profile;
    rv = bcm_l3_host_add(unit, &host);
    if (rv != BCM_E_NONE)
    {
        printf("Error, in function bcm_l3_host_add vpbr miss\n");
        return rv;
    }

    /** Add ipv4 host entry - vpbr hit */
    bcm_l3_host_t_init(&host);
    host.l3a_ip_addr = cint_stat_pp_vpbr_info.ipv4_host_vpbr_hit;
    host.l3a_vrf = cint_stat_pp_vpbr_info.new_vrf;
    host.l3a_intf = cint_stat_pp_vpbr_info.fec_id;
    rv = bcm_l3_host_add(unit, &host);
    if (rv != BCM_E_NONE)
    {
        printf("Error, in function bcm_l3_host_add vpbr hit\n");
        return rv;
    }

    /** Add ipv4 vpbr entry */
    bcm_l3_vpbr_entry_t_init(&vpbr_entry);
    vpbr_entry.flags = 0;
    vpbr_entry.priority = 1;
    vpbr_entry.vrf = cint_stat_pp_vpbr_info.vrf;
    vpbr_entry.vrf_mask = 0xFFFFFFFF;
    vpbr_entry.sip_addr = cint_stat_pp_vpbr_info.ipv4_host_sip;
    vpbr_entry.sip_addr_mask = 0xFFFFFFFF;
    vpbr_entry.dip_addr = cint_stat_pp_vpbr_info.ipv4_host_vpbr_hit;
    vpbr_entry.dip_addr_mask = 0xFFFFFFFF;
    vpbr_entry.dscp = 0xF0;
    vpbr_entry.dscp_mask = 0x0F;
    vpbr_entry.src_port = cint_stat_pp_vpbr_info.in_port;
    vpbr_entry.src_port_mask = 0;
    vpbr_entry.dst_port = cint_stat_pp_vpbr_info.out_port;
    vpbr_entry.dst_port_mask = 0;
    vpbr_entry.l3_intf_id = cint_stat_pp_vpbr_info.fec_id;
    vpbr_entry.l3_intf_id_mask = 0;
    vpbr_entry.gport_wide_data = 0x5;
    vpbr_entry.gport_wide_data_mask = 0xF;
    vpbr_entry.new_vrf = cint_stat_pp_vpbr_info.new_vrf;
    vpbr_entry.stat_id = l3_vpbr_statistics_config.stat_id;
    vpbr_entry.stat_pp_profile = l3_vpbr_statistics_config.pp_profile;
    rv = bcm_l3_vpbr_entry_add(unit, &vpbr_entry);
    if (rv != BCM_E_NONE)
    {
        printf("Error, in function bcm_l3_vpbr_entry_add\n");
        return rv;
    }

    /** Add ipv6 host entry - vpbr miss */
    bcm_l3_host_t_init(&host);
    host.l3a_flags = BCM_L3_IP6;
    sal_memcpy(host.l3a_ip6_addr, cint_stat_pp_vpbr_info.ipv6_host_vpbr_miss, 16);
    host.l3a_vrf = cint_stat_pp_vpbr_info.vrf;
    host.l3a_intf = cint_stat_pp_vpbr_info.fec_id;
    host.stat_id = l3_host_statistics_config.stat_id;
    host.stat_pp_profile = l3_host_statistics_config.pp_profile;
    rv = bcm_l3_host_add(unit, &host);
    if (rv != BCM_E_NONE)
    {
        printf("Error, in function bcm_l3_host_add vpbr miss\n");
        return rv;
    }

    /** Add ipv6 host entry - vpbr hit */
    bcm_l3_host_t_init(&host);
    host.l3a_flags = BCM_L3_IP6;
    sal_memcpy(host.l3a_ip6_addr, cint_stat_pp_vpbr_info.ipv6_host_vpbr_hit, 16);
    host.l3a_vrf = cint_stat_pp_vpbr_info.new_vrf;
    host.l3a_intf = cint_stat_pp_vpbr_info.fec_id;
    rv = bcm_l3_host_add(unit, &host);
    if (rv != BCM_E_NONE)
    {
        printf("Error, in function bcm_l3_host_add vpbr hit\n");
        return rv;
    }

    /** Add ipv6 vpbr entry */
    bcm_l3_vpbr_entry_t_init(&vpbr_entry);
    vpbr_entry.flags = BCM_L3_VPBR_IP6;
    vpbr_entry.priority = 1;
    vpbr_entry.vrf = cint_stat_pp_vpbr_info.vrf;
    vpbr_entry.vrf_mask = 0xFFFFFFFF;
    sal_memcpy(vpbr_entry.sip6_addr, cint_stat_pp_vpbr_info.ipv6_host_sip, 16);
    sal_memcpy(vpbr_entry.sip6_addr_mask, ip6_mask, 16);
    sal_memcpy(vpbr_entry.dip6_addr, cint_stat_pp_vpbr_info.ipv6_host_vpbr_hit, 16);
    sal_memcpy(vpbr_entry.dip6_addr_mask, ip6_mask, 16);
    vpbr_entry.dscp = 0xF0;
    vpbr_entry.dscp_mask = 0x0F;
    vpbr_entry.src_port = cint_stat_pp_vpbr_info.in_port;
    vpbr_entry.src_port_mask = 0;
    vpbr_entry.dst_port = cint_stat_pp_vpbr_info.out_port;
    vpbr_entry.dst_port_mask = 0;
    vpbr_entry.l3_intf_id = cint_stat_pp_vpbr_info.fec_id;
    vpbr_entry.l3_intf_id_mask = 0;
    vpbr_entry.gport_wide_data = 0x5;
    vpbr_entry.gport_wide_data_mask = 0xF;
    vpbr_entry.new_vrf = cint_stat_pp_vpbr_info.new_vrf;
    vpbr_entry.stat_id = l3_vpbr_statistics_config.stat_id;
    vpbr_entry.stat_pp_profile = l3_vpbr_statistics_config.pp_profile;
    rv = bcm_l3_vpbr_entry_add(unit, &vpbr_entry);
    if (rv != BCM_E_NONE)
    {
        printf("Error, in function bcm_l3_vpbr_entry_add\n");
        return rv;
    }

    printf("%s(): Exit\r\n", proc_name);
    return rv;
}
