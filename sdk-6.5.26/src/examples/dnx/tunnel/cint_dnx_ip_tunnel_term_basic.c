/*
 * 
 This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 
 Copyright 2007-2022 Broadcom Inc. All rights reserved. File: cint_dnx_ip_tunnel_term_basic.c Purpose: Basic examples for IP Tunnels Termination.
 */

/*
 *
 * Configuration:
 *
 * cint;
 * cint_reset();
 * exit;
 * cint ../../../../src/examples/dnx/utility/cint_dnx_utils_global.c
 * cint ../../../../src/examples/dnx/l3/cint_dnx_ip_route_basic.c
 * cint ../../../../src/examples/dnx/cint_dnx_ip_tunnel_term_basic.c
 * cint
 * int unit = 0;
 * int rv = 0;
 * int provider_port = 200;
 * int access_port = 201;
 * rv = ip_tunnel_term_basic(unit,provider_port,access_port);
 * print rv;
 *
 *  Scenarios configured in this cint:
 *
 *  ~~~~~~~~~~~~~~~~~~~~~~~~
 *  1) Basic IP Tunnel termination
 *  ~~~~~~~~~~~~~~~~~~~~~~~~
 *  A GRE4oIPv4 tunnel header is being terminated.
 *
 *  Traffic:
 *
 *  Send the following traffic:
 *
 *  ###############################################################################################################################
 *  Sending packet from  provider port == 200 :   ----------->
 *   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *   |    +-+-+-+-+-+-+-+-+-+-+-+-+-++-+-+-+-+-+-+-+-+-++-+-+-+-+-+-+-+-+-+-+-+-+-++-+-+-+-+-+-+-+-+-+-+-+-++-+-+-+-+-+-+-+-+-+-+-+-+
 *   |    |      DA      | SA           || IPv4 tunnel SIP || IPv4 tunnel DIP ||  GRE  ||      IPv4 SIP     ||      IPv4 DIP      ||
 *   |    |0C:00:02:00:00|11:00:00:01:12||    172.0.0.1    ||    171.0.0.1    ||       ||  160.161.161.162  ||    192.128.1.1     ||
 *   |    +-+-+-+-+-+-+-+-+-+-+-+-+-+--+-+-+-+-+-+-+-+-++-+-+-+-+-+-+-+-+-+-+-+-+-+--+-+-+-+-+-+-+-+-+-+-+-++-+-+-+-+-+-+-+-+-+-+-+-+
 *   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 *  Receiving packet on access port == 201:   <------------
 *   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *   |    +-+-+-+-+-+-+-+-+-+-+-+-+-++-+-+-+-+-+-+-+-+-++-+-+-+-+-+-+-+-+-+-+-+-+-++-+-+-+-+-+-+-+-+-+-+-+-+-+-+-++-+-+-+-+-+-+-+-
 *   |    |      DA      | SA           ||      IPv4 SIP     ||      IPv4 DIP      ||
 *   |    |00:00:00:CD:1D|0C:00:02:00:01||  160.161.161.162  ||    192.128.1.1     ||
 *   |    +-+-+-+-+-+-+-+-+-+-+-+-+-+--+-+-+-+-+-+-+-+-++-+-+-+-+-+-+-+-+-+-+-+-+-+--+-+-+-+-+-+-+-+-+-+-+-+-+-+-++-+-+-+-+-+-+-+
 *   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * ################################################################################################################################
 *
 */
struct cint_ip_tunnel_basic_info_s
{
    int eth_rif_intf_provider;              /* in RIF */
    int eth_rif_intf_access;                /* out RIF */
    int tunnel_if;                          /* RIF, property of the tunnel termination inLif.*/
    bcm_mac_t intf_provider_mac_address;    /* mac for in RIF */
    bcm_mac_t intf_access_mac_address;      /* mac for out RIF */
    bcm_mac_t arp_next_hop_mac;             /* mac for next hop */
    int access_vrf;                         /* router instance VRF resolved at VTT1*/
    int provider_vrf;                       /* VRF after termination, resolved as Tunnel-InLif property */
    int default_vrf;                        /* VRF default value. For VXLAN VNI->VRF lookup miss */
    int fec_id;                             /* FEC id */
    int arp_id;                             /* Id for ARP entry */
    bcm_ip_t route_dip;                     /* Route DIP */
    uint32 mask;                            /* mask for route DIP */
    bcm_ip_t tunnel_dip;                    /* tunnel DIP */
    bcm_ip_t tunnel_dip_mask;               /* tunnel DIP mask*/
    bcm_ip_t tunnel_sip;                    /* tunnel SIP */
    bcm_ip_t tunnel_sip_mask;               /* tunnel SIP mask */
    int tunnel_type;                        /* bcmTunnelType */
    int tunnel_id;                          /* tunnel ID */
    int is_tunnel_contain_wide_data;        /* indicate wide_data support, support on J2C and above */
};

cint_ip_tunnel_basic_info_s cint_ip_tunnel_basic_info =
{
    /*
     * eth_rif_intf_provider | eth_rif_intf_access | tunnel_if
     */
    15,                        100,                  20,
    /*
     * intf_provider_mac_address
     */
    {0x00, 0x0c, 0x00, 0x02, 0x00, 0x00},
    /*
     * intf_access_mac_address
     */
    {0x00, 0x0c, 0x00, 0x02, 0x00, 0x01},
    /*
     * arp_next_hop_mac
     */
    {0x00, 0x00, 0x00, 0x00, 0xcd, 0x1d},
    /*
     * access_vrf | provider_vrf | default_vrf
     */
    5,              1,             123,
    /*
     * fec_id
     */
    5001,
    /*
     * arp_id
     */
    0,
    /*
     * route_dip
     */
    0xA0A1A1A2, /* 160.161.161.162 */
    /*
     * mask
     */
    0xfffffff0,
    /*
     * tunnel_dip
     */
    0xAB000001, /* 171.0.0.1 */
    /*
     * tunnel_dip_mask
     */
    0xffffffff,
    /*
     * tunnel_sip
     */
    0xAC000001, /* 172.0.0.1 */
    /*
     * tunnel_sip_mask
     */
    0xffffffff,
    /*
     * tunnel_type | tunnel_id
     */
    bcmTunnelTypeGreAnyIn4, 0,
    /*
     * is_tunnel_contain_wide_data
     */
    0
};

/*
* Main function for basic ip  tunnel termination scenario with TCAM lookup in Jericho 2 
* The function ip_tunnel_term_tcam implements the basic tunnel termination scenario with TCAM lookup.
* Inputs:
* unit - relevant unit;
* provider_port - incoming port;
* access_port - outgoing port;
*/
int
ip_tunnel_term_tcam(
    int unit,
    int provider_port,
    int access_port)
{
    cint_ip_tunnel_basic_info.tunnel_dip_mask = 0xFFFF0000; 
    ip_tunnel_term_basic(unit, provider_port, access_port);
}

/*
 * Main function for basic ip  tunnel termination scenarios in Jericho 2
 * The function basic_example implements the basic tunnel termination scenario.
 * Inputs:
 * unit - relevant unit;
 * provider_port - incoming port;
 * access_port - outgoing port;
 */
int
ip_tunnel_term_basic(
    int unit,
    int provider_port,
    int access_port)
{
    int rv = BCM_E_NONE;
    int fec_flags2 = 0;
    char *proc_name;
    l3_ingress_intf ingr_itf;
    l3_ingress_intf_init(&ingr_itf);

    proc_name = "ip_tunnel_term_basic";

    /*
     * Set In-Port to In ETh-RIF
     */
    rv = in_port_intf_set(unit, provider_port, cint_ip_tunnel_basic_info.eth_rif_intf_provider);
    if (rv != BCM_E_NONE)
    {
        printf("%s(): Error, in_port_intf_set, eth_rif_intf_provider\n", proc_name);
        return rv;
    }

    /*
     * Set Out-Port default properties
     */
    rv = out_port_set(unit, access_port);
    if (rv != BCM_E_NONE)
    {
        printf("%s(): Error, out_port_set, access_port\n", proc_name);
        return rv;
    }

    /*
     * Create routing interface for the tunnel's IP.
     * We are using it as ingress ETH-RIF to perform ETH termination (my-mac procedure), to enable IPv4 routing for this ETH-RIF and to set the VRF.
     */
    rv = intf_eth_rif_create_vrf(unit,
            cint_ip_tunnel_basic_info.eth_rif_intf_provider,
            cint_ip_tunnel_basic_info.provider_vrf,
            cint_ip_tunnel_basic_info.intf_provider_mac_address);
    if (rv != BCM_E_NONE)
    {
        printf("%s(): Error, intf_eth_rif_create, eth_rif_intf_provider\n", proc_name);
        return rv;
    }

    /*
     * Create egress routing interface used for routing after the tunnel termination.
     * We are using it as egress ETH-RIF, providing the link layer SA.
     */
    rv = intf_eth_rif_create(unit, cint_ip_tunnel_basic_info.eth_rif_intf_access, cint_ip_tunnel_basic_info.intf_access_mac_address);
    if (rv != BCM_E_NONE)
    {
        printf("%s(): Error, intf_eth_rif_create, eth_rif_intf_access\n", proc_name);
        return rv;
    }

    /*
     * Set Incoming ETH-RIF properties, VRF is updated for the rif.
     * This VRF value is used for IP-Tunnel-Term.VRF lookup key.

    ingr_itf.intf_id = cint_ip_tunnel_basic_info.eth_rif_intf_provider;
    ingr_itf.vrf     = cint_ip_tunnel_basic_info.provider_vrf;
    rv = intf_ingress_rif_set(unit, &ingr_itf);
    if (rv != BCM_E_NONE)
    {
        printf("%s(): Error, intf_ingress_rif_set\n", proc_name);
        return rv;
    }
*/
    /*
     * Create the tunnel termination lookup and the tunnel termination inlif
     */
    rv = tunnel_terminator_create(unit);
    if (rv != BCM_E_NONE)
    {
        printf("%s(): Error, tunnel_terminator_create\n", proc_name);
        return rv;
    }

    /*
     * Create ARP entry and set its properties
     */
    rv = l3__egress_only_encap__create(unit, 0, &(cint_ip_tunnel_basic_info.arp_id),
                                       cint_ip_tunnel_basic_info.arp_next_hop_mac,
                                       0);

    if (rv != BCM_E_NONE)
    {
        printf("%s(): Error, l3__egress_only_encap__create\n", proc_name);
        return rv;
    }

    /*
     * Create FEC and configure its: Outlif (ARP), Destination port, Global Out-EthRIF
     */
    rv = get_first_fec_in_range_which_not_in_ecmp_range(unit, 0, 0, &cint_ip_tunnel_basic_info.fec_id);
    if (rv != BCM_E_NONE)
    {
        printf("Error, get_first_fec_in_range_which_not_in_ecmp_range\n");
        return rv;
    }

    if(*dnxc_data_get(unit, "l3", "feature", "separate_fwd_rpf_dbs", NULL))
    {
        fec_flags2 |= BCM_L3_FLAGS2_FWD_ONLY;
    }

    rv = l3__egress_only_fec__create(unit, cint_ip_tunnel_basic_info.fec_id,
                                     cint_ip_tunnel_basic_info.eth_rif_intf_access,
                                     cint_ip_tunnel_basic_info.arp_id, access_port, 0, fec_flags2);

    if (rv != BCM_E_NONE)
    {
        printf("%s(): Error,l3__egress_only_fec__create\n", proc_name);
        return rv;
    }

    /*
     * Add Route entry
     */
    rv = add_route_ipv4(unit, cint_ip_tunnel_basic_info.route_dip, cint_ip_tunnel_basic_info.mask,
                        cint_ip_tunnel_basic_info.access_vrf, cint_ip_tunnel_basic_info.fec_id);
    if (rv != BCM_E_NONE)
    {
        printf("%s(): Error, add_route_ipv4\n", proc_name);
        return rv;
    }

    return rv;
}

/** for vxlan-gpe, set default vrf in case no hit in VNI->FODO
 */
int 
tunnel_terminator_create_set_default_vrf(
    int unit, 
    bcm_tunnel_terminator_t * tunnel_term)
{
    int rv;


    if ((tunnel_term->type == bcmTunnelTypeVxlanGpe) || (tunnel_term->type == bcmTunnelTypeVxlan6Gpe)){
        tunnel_term->default_vrf = cint_ip_tunnel_basic_info.default_vrf;
    }

    return rv;
}

int
tunnel_terminator_create(
    int unit)
{
    int rv;
    bcm_tunnel_terminator_t tunnel_term;

    /* Create IP tunnel terminator for SIP,DIP, VRF lookup */
    bcm_tunnel_terminator_t_init(&tunnel_term);
    tunnel_term.dip      = cint_ip_tunnel_basic_info.tunnel_dip;
    tunnel_term.dip_mask = cint_ip_tunnel_basic_info.tunnel_dip_mask;
    tunnel_term.sip      = cint_ip_tunnel_basic_info.tunnel_sip;
    tunnel_term.sip_mask = cint_ip_tunnel_basic_info.tunnel_sip_mask;
    tunnel_term.flags = BCM_TUNNEL_TERM_STAT_ENABLE;
    tunnel_term.vrf = cint_ip_tunnel_basic_info.provider_vrf;
    tunnel_term.type = cint_ip_tunnel_basic_info.tunnel_type;

    if (cint_ip_tunnel_basic_info.is_tunnel_contain_wide_data)
    {
        tunnel_term.flags = BCM_TUNNEL_TERM_WIDE;
    }

    rv = tunnel_terminator_create_set_default_vrf(unit, &tunnel_term);
    if (rv != BCM_E_NONE) {
        printf("Error, tunnel_terminator_create_set_default_vrf\n");
        return rv;
    }

    rv = bcm_tunnel_terminator_create(unit,&tunnel_term);
    if (rv != BCM_E_NONE) {
        printf("Error, bcm_tunnel_terminator_create\n");
        return rv;
    }
    cint_ip_tunnel_basic_info.tunnel_id = tunnel_term.tunnel_id;

    /** For VXLAN-GPE, no need to update the VRF.
     *  VRF is retrieved from VNI->VRF lookup  */
    if ((cint_ip_tunnel_basic_info.tunnel_type != bcmTunnelTypeVxlanGpe)
        && (cint_ip_tunnel_basic_info.tunnel_type != bcmTunnelTypeVxlan6Gpe)){
         /** In JR2 VRF is a tunnel-LIF property set by bcm_l3_ingress_create API */
         bcm_l3_ingress_t ing_intf;
         bcm_l3_ingress_t_init(&ing_intf);
         ing_intf.flags = BCM_L3_INGRESS_WITH_ID; /* must, as we update exist RIF */
         ing_intf.vrf   = cint_ip_tunnel_basic_info.access_vrf;

         /** Convert tunnel's GPort ID to intf ID */
          bcm_if_t intf_id;
          BCM_GPORT_TUNNEL_TO_L3_ITF_LIF(intf_id, tunnel_term.tunnel_id);

          rv = bcm_l3_ingress_create(unit, ing_intf, intf_id);
          if (rv != BCM_E_NONE) {
              printf("Error, bcm_l3_ingress_create\n");
              return rv;
          }
    }
    return rv;
}
