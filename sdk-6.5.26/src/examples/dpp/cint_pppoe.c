/* 
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2022 Broadcom Inc. All rights reserved.
 * 
 * **************************************************************************************************************************************************

 *  

 * SOC property
 * Enable PPPoE encap and anti-spoofing function 
 * PPPoE_mode=1
 * Enable SIP4/SIP6 anti-spoofing
 * l3_source_bind_mode=IP
 * l3_source_bind_subnet_mode=IP
 * 
 * run:
 * cd ../../../../src/examples/dpp/
 * cint utility/cint_utils_l3.c
 * cint utility/cint_utils_l2.c
 * cint ../sand/utility/cint_sand_utils_global.c
 * cint cint_pppoe.c
 * cint cint_field_pppoe.c
 * 
 * debug bcm rx
 * 
 * cint
 * print basic_example_pppoe(0, 201, 203);
 * print pppoe_protocol_fg(0, 201);
 * print pppoe_sipv4_anti_spoofing_fg(0, 201);
 * print pppoe_protocol_pppoe_anti_spoofing_fg(0, 201);
 * print pppoe_data_sipv6_anti_spoofing_fg(0, 201);
 * exit;
 * 
 * ##############   PPPoE data packet format    #######################
 *
 * Data path:
 *
 *(UNI-->NNI)
 * PPPoE subnet DIP --- packet will be routed to port 203, stripped from PPP header and new next hop header with VLAN 0x64
 * tx 1  PtchSRCport=201 DATA="00110000001100000700010081000011886411005555002000214508002e000040004006b8bd6fffff017fffff033344556600a1a2a300b1b2b3500f01f57eb00000f1f2f3f4f5f600000000"
 *                             DA          SA          Vlan0x11Eth PPP Sess    IPv4                        SIP     DIP
 *     																											   Type    ion
 *  
 * (NNI-->UNI) 
 * IPv4(NNI->UNI) --- Packet will be routed to port 201, encapsulated with PPP header and new next hop header with VLAN 0x11
 * tx 1  PtchSRCport=203 DATA="0011000000220000070001008100006408004500002E0000000040FF79D27fffff038fffff0208004508002E00004000000102030405060708090A0B0C0D0E0F00000000"
 *                             DA          SA          Vlan0x64Eth IPv4                    SIP     DIP
 *                                                             Type
 *
 * UNI->NNI traps:
 * 
 * Trap due to PPPoE discovery protocol 0x8863
 * PPPoE control(discovery) --- Packet will be trapped to port 15 (due to 0x8863)
 * tx 1  PtchSRCport=201 DATA="00110000001100000700010081000011886311005555002e00214508002e000040004006b8bd6fffff017fffff033344556600a1a2a300b1b2b3500f01f57eb00000f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f"
 *                             DA          SA          Vlan0x11Eth PPP Sess                                 
 *                                                             Type    ion      
 *
 * Trap due to 0x8864 + PPPoE protocols
 * PPPoE Session control (0x8864 + 0x8021/0x8057/0xc021) -- Packet will be trapped to port 15 (due to the LCP/IPCP protocols ID)
 * tx 1  PtchSRCport=201 DATA="00110000001100000700010081000011886411005555002e8021ff08002e000040004006b8bd6fffff017fffff023344556600a1a2a300b1b2b3500f01f57eb00000f5f5f5f5f5f5f5f5f5"
 * tx 1  PtchSRCport=201 DATA="00110000001100000700010081000011886411005555002e8057ff08002e000040004006b8bd6fffff017fffff023344556600a1a2a300b1b2b3500f01f57eb00000f5f5f5f5f5f5f5f5f5"
 * tx 1  PtchSRCport=201 DATA="00110000001100000700010081000011886411005555002eC021ff08002e000040004006b8bd6fffff017fffff023344556600a1a2a300b1b2b3500f01f57eb00000f5f5f5f5f5f5f5f5f5"
 *                             DA          SA          Vlan0x11Eth PPP Sess    Ctrl                             
 *                                                             Type    ion     Prot    
 *                                                 
 * UNI->NNI Anti-Spoofing:
 *
 * PPPoE host DIP with unknown PPPoE session ID --- Packet will be dropped (due to unknown session)
 * tx 1  PtchSRCport=201 DATA="00110000001100000700010081000011886411005554002e00214508002e000040004006b8bd6fffff017fffff023344556600a1a2a300b1b2b3500f01f57eb00000f5f5f5f5f5f5f5f5f5"
 *                             DA          SA          Vlan0x11Eth PPP Sess    IPv4                        SIP     DIP
 *                                                             Type    ion         
 *
 * PPPoE host DIP with unknown SIP --- Packet will be dropped (due to unknown SIP)
 * tx 1  PtchSRCport=201 DATA="00110000001100000700010081000011886411005555002e00214508002e000040004006b8bd8fffff017fffff023344556600a1a2a300b1b2b3500f01f57eb00000f5f5f5f5f5f5f5f5f5"
 *                             DA          SA          Vlan0x11Eth PPP Sess    IPv4                        SIP     DIP
 *                                                             Type    ion        
 *
 * PPPoE host DIP   --- packet will be routed to port 203, stripped from PPP header and added a new next hop header with VLAN 0x64
 * tx 1  PtchSRCport=201 DATA="00110000001100000700010081000011886411005555002000214508002e000040004006b8bd6fffff017fffff023344556600a1a2a300b1b2b3500f01f57eb00000f5f5f5f5f5f5f5f5f5"
 *                             DA          SA          Vlan0x11Eth PPP Sess    IPv4                        SIP     DIP
 *                                                             Type    ion   
 *
 * IPv4 MC(UNI==>NNI) 
 * PPPoE host DIP with unknown PPPoE session ID --- Packet will be dropped (due to unknown session)
 * tx 1  PtchSRCport=201 DATA="01005E60E00200000700010081000011886411005554002000214508002e000040004006b8bd6fffff01e0e0e0023344556600a1a2a300b1b2b3500f01f57eb00000f1f2f3f4f5f600000000"
 *                             DA          SA          Vlan0x11Eth PPP Sess    IPv4                        SIP     DIP
 *                                                             Type    ion         
 *
 * PPPoE host DIP with unknown SIP --- Packet will be dropped (due to unknown SIP)
 * tx 1  PtchSRCport=201 DATA="01005E60E00200000700010081000011886411005555002000214508002e000040004006b8bd6ffffe01e0e0e0023344556600a1a2a300b1b2b3500f01f57eb00000f1f2f3f4f5f600000000"
 *                             DA          SA          Vlan0x11Eth PPP Sess    IPv4                        SIP     DIP
 *                                                             Type    ion        
 *
 * PPPoE host DIP   --- packet will be routed to port 203, stripped from PPP header and added a new next hop header with VLAN 0x64
 * tx 1  PtchSRCport=201 DATA="01005E60E00200000700010081000011886411005555002000214508002e000040004006b8bd6fffff01e0e0e0023344556600a1a2a300b1b2b3500f01f57eb00000f1f2f3f4f5f600000000"
 *                             DA          SA          Vlan0x11Eth PPP Sess    IPv4                        SIP     DIP
 *                                                             Type    ion   
 * IPv6 MC(UNI==>NNI) 
 * PPPoE host DIP with unknown PPPoE session ID --- Packet will be dropped (due to unknown session)
 * tx 1  PtchSRCport=201 DATA="33330000121200020500000081000011886411005554003000576034000000063BFF0000000000000000123400000000EE10FFFF0000000000000000000000001212B86DD60000003C27B1CF"
 *                             DA          SA          Vlan0x11Eth PPP Sess    IPv6                        SIP                     DIP
 *                                                             Type    ion         
 *
 * PPPoE host DIP with unknown SIP --- Packet will be dropped (due to unknown SIP)
 * tx 1  PtchSRCport=201 DATA="33330000121200020500000081000011886411005555003000576034000000063BFF0000000000000000123400000000ff10FFFF0000000000000000000000001212B86DD60000003C27B1CF"
 *                             DA          SA          Vlan0x11Eth PPP Sess    IPv6                        SIP                     DIP
 *                                                             Type    ion        
 *
 * PPPoE host DIP   --- packet will be routed to port 203, stripped from PPP header and added a new next hop header with VLAN 0x64
 * tx 1  PtchSRCport=201 DATA="33330000121200020500000081000011886411005555003000576034000000063BFF0000000000000000123400000000EE10FFFF0000000000000000000000001212B86DD60000003C27B1CF"
 *                             DA          SA          Vlan0x11Eth PPP Sess    IPv6                        SIP                     DIP
 *                                                             Type    ion   
 * IPv4 MC(NNI==>UNI)
 * Packet will be routed to port 201, encapsulated with PPP header and new next hop header with VLAN 0x11
 * tx 1  PtchSRCport=203 DATA="01005E60E0030000070001008100006408004500002E0000000040FF79D27fffff03e0e0e00308004508002E00004000000102030405060708090A0B0C0D0E0F00000000"
 *                             DA          SA          Vlan0x64Eth IPv4                    SIP     DIP
 *                                                             Type
 * IPv6 MC(NNI==>UNI)
 * Packet will be routed to port 201, encapsulated with PPP header and new next hop header with VLAN 0x11 
 * tx 1  PtchSRCport=203 DATA="3333000012130002050000008100006486dd6034000000063BFF0000000000000000123400000000EE10FFFF0000000000000000000000001313B86DD60000003C27B1CF"
 *                             DA          SA          Vlan0x64Eth IPv6            SIP                             DIP
 *                                                             Type
 */
 
 
/*
 * Add an IPv4 Route entry <vrf, addr, mask> --> intf
 * - addr: IP address 32 bit value
 * - mask 1: to consider 0: to ingore, for example for /24 mask = 0xffffff00 will mask the 24 MSBs of the DIP
 * - vrf: VRF ID
 * - intf: egress object created using create_l3_egress (FEC)
 *
 */
int
add_route_ipv4(
    int unit,
    uint32 addr,
    uint32 mask,
    int vrf,
    int intf)
{
    int rc;
    bcm_l3_route_t l3rt;
    bcm_l3_route_t_init(l3rt);

    l3rt.l3a_subnet = addr;
    l3rt.l3a_ip_mask = mask;
    l3rt.l3a_vrf = vrf;
    l3rt.l3a_intf = intf;       /* FEC-ID */
    rc = bcm_l3_route_add(unit, l3rt);
    if (rc != BCM_E_NONE)
    {
        printf("bcm_l3_route_add failed: %x \n", rc);
    }
    return rc;
}

/*
 * Add an IPv6 Route entry using <vrf, addr, mask> that will result to an intf.
 * - addr: IPv6 address 16 8-bit values
 * - mask: 1: to consider, 0: to ingore, for example for /64 mask = ffff:ffff:ffff:ffff:0000:0000:0000:0000
 *              masks the 64 MSB of the IPv6 DIP which will be considered.
 * - vrf: VRF ID
 * - intf: egress object created using create_l3_egress (FEC)
 *
 */
int
add_route_ipv6(
    int unit,
    bcm_ip6_t addr,
    bcm_ip6_t mask,
    int vrf,
    int intf)
{
    int rc;
    bcm_l3_route_t l3rt;
    bcm_l3_route_t_init(l3rt);

    l3rt.l3a_ip6_net = addr;
    l3rt.l3a_ip6_mask = mask;
    l3rt.l3a_vrf = vrf;
    l3rt.l3a_intf = intf;       /* FEC-ID */
    l3rt.l3a_flags = BCM_L3_IP6;
    rc = bcm_l3_route_add(unit, l3rt);
    if (rc != BCM_E_NONE)
    {
        printf("bcm_l3_route_add failed: %x \n", rc);
    }
    return rc;
}

/*
 * Add IPv4 Host entry <vrf, addr> --> intf
 * - addr: IP address 32 bit value
 * - vrf: VRF ID
 * - intf: egress object created using create_l3_egress (FEC)
 * - encap_id: arp_encap_id encapsulation ID of an l3 egress interface.
 * - destination: when host format is {System-Port, OutRIF, ARP} (No-FEC),
 *   this field should contain gport, and will be assigned to host.l3a_port_tgid,
 *   otherwise this field should contain 0.
 *
 * If encap_id = 0, the destination will be a FEC.
 * If encap_id != 0, the entry destination will be a FEC + out-LIF (ARP).
 *
 */
int
add_host_ipv4(
    int unit,
    uint32 addr,
    int vrf,
    int intf,
    int encap_id,
    int destination)
{
    int rc;
    bcm_l3_host_t host;
    bcm_l3_host_t_init(host);

    host.l3a_ip_addr = addr;
    host.l3a_vrf = vrf;
    host.l3a_intf = intf;       /* FEC-ID or OUTRIF*/
    host.encap_id = encap_id;
    if(destination)
    {
        /*
         * if destination is assigned then intf should be outrif and not FEC
         */
        if(BCM_L3_ITF_TYPE_IS_RIF(intf))
        {
            host.l3a_port_tgid = destination;
        }
        else
        {
            return BCM_E_PARAM;
        }

    }
    rc = bcm_l3_host_add(unit, host);
    if (rc != BCM_E_NONE)
    {
        printf("bcm_l3_host_add failed: %x \n", rc);
    }
    return rc;
}

/*
 * Add IPv6 Host entry <vrf, addr> --> intf
 * - addr: IPv6 address 16 8-bit values
 * - vrf: VRF ID
 * - intf: egress object created using create_l3_egress (FEC)
 *
 */
int
add_host_ipv6(
    int unit,
    bcm_ip6_t addr,
    int vrf,
    int intf)
{
    int rc;
    bcm_l3_host_t host;
    bcm_l3_host_t_init(host);

    host.l3a_ip6_addr = addr;
    host.l3a_vrf = vrf;
    host.l3a_intf = intf;       /* FEC-ID */
    host.l3a_flags = BCM_L3_IP6;
    rc = bcm_l3_host_add(unit, host);
    if (rc != BCM_E_NONE)
    {
        printf("bcm_l3_host_add failed: %x \n", rc);
    }
    return rc;
}

/*
 * Set In-Port default ETH-RIF:
 * - in_port: Incoming port ID
 * - eth_rif: ETH-RIF 
 */
int
vlan_port_intf_create_with_fec(
    int unit,
    int in_port,
    int eth_rif,
    int pppoe_session_id,
    int *uni_gport, 
    int *encap_id)
{
    bcm_vlan_port_t vlan_port;
    int rc;

    vlan_port.flags |= BCM_VLAN_PORT_FORWARD_GROUP;
    vlan_port.port = in_port;
    vlan_port.match_vlan = eth_rif;
    vlan_port.egress_vlan = eth_rif;
    vlan_port.egress_tunnel_value = pppoe_session_id;
    vlan_port.criteria = BCM_VLAN_PORT_MATCH_PORT_VLAN;

    rc = bcm_vlan_port_create(unit, vlan_port);
    if (rc != BCM_E_NONE)
    {
        printf("Error, bcm_vlan_port_create\n");
        return rc;
    }

    *uni_gport = vlan_port.vlan_port_id;
    *encap_id = vlan_port.encap_id;

    return rc;
}

int  bind_PPPoE_anti_spoofing(int unit,int uni_gport,int session_ID)
{
    int rv;
    bcm_l3_source_bind_t src_bind;

    bcm_l3_source_bind_t_init(&src_bind);
    src_bind.session_id = session_ID;
    src_bind.port = uni_gport;
    src_bind.flags = BCM_L3_SOURCE_BIND_PPPoE;
    rv = bcm_l3_source_bind_add(unit, &src_bind);
    if (rv != BCM_E_NONE)
    {
        printf("Error, bcm_l3_source_bind_add binding PPPoE anti-spoofing entry\n");
        return rv;
    }

    return rv;
}

int  bind_sipv4_anti_spoofing(int unit,int uni_gport, bcm_ip_t sipv4, bcm_ip_t mask )
{
    int rv;
    bcm_l3_source_bind_t src_bind;

    bcm_l3_source_bind_t_init(&src_bind);
    src_bind.ip = sipv4;
    src_bind.ip_mask = mask;/*255.255.255.0*/
    src_bind.port = uni_gport;
    src_bind.flags = BCM_L3_SOURCE_BIND_USE_MASK;
    rv = bcm_l3_source_bind_add(unit, &src_bind);
    if (rv != BCM_E_NONE)
    {
        printf("Error, bcm_l3_source_bind_add binding SIPv4 anti-spoofing entry\n");
        return rv;
    }

    return rv;
}

int  bind_sipv6_anti_spoofing(int unit,int uni_gport, bcm_ip6_t sip, bcm_ip6_t mask )
{
    int rv;
    bcm_l3_source_bind_t src_bind;

    bcm_l3_source_bind_t_init(&src_bind);
    sal_memcpy(src_bind.ip6, sip, 16);
    sal_memcpy(src_bind.ip6_mask, mask, 16);
    src_bind.port = uni_gport;
    src_bind.flags = (BCM_L3_SOURCE_BIND_USE_MASK|BCM_L3_SOURCE_BIND_IP6);
    rv = bcm_l3_source_bind_add(unit, &src_bind);
    if (rv != BCM_E_NONE)
    {
        printf("Error, bcm_l3_source_bind_add binding SIPv6 anti-spoofing entry\n");
        return rv;
    }

    return rv;
}

/******* Run example ******/
/*
 * example: 
 int unit = 0;
 int in_port = 201;
 int out_port = 203;
 */
int ing_mc_nni2uni = 0;
int  basic_example_pppoe(int unit,int in_port,int out_port)
{
    int rv;
    int intf_in = 0x11;           /* Incoming packet VLAN */
    int intf_out = 100;         /* Outgoing packet VLAN */
    int fec = 0;            /* 32769=32*1024+1 */
    int vrf = 0xf;
    bcm_mac_t intf_in_mac_address = { 0x00, 0x11, 0x00, 0x00, 0x00, 0x11 };     /* my-MAC - trerminating L2 UNI->NNI */
    bcm_mac_t intf_out_mac_address = { 0x00, 0x11, 0x00, 0x00, 0x00, 0x22 };    /* my-MAC - trerminating L2 NNI->UNI */
    bcm_mac_t arp_next_hop_mac = { 0x00, 0x00, 0x00, 0x00, 0xcd, 0x1d };        /* next_hop_mac - DA from UNI->NNI */
    bcm_mac_t arp_next_hop_mac_in = { 0x00, 0x00, 0x00, 0x00, 0xcd, 0x2d };     /* next_hop_mac - DA from NNI->UNI */
    bcm_gport_t gport;
    uint32 route = 0x7fffff00;         /* UNI->NNI subnet route */
    uint32 host = 0x7fffff02;          /* UNI->NNI host DIP */
    uint32 mask = 0xfffffff0;
    uint32 route_nni2uni = 0x8fffff00;
    uint32 host_nni2uni = 0x8fffff02;  /* NNI->UNI host DIP */
    uint32 mask_nni2uni = 0xfffffff0;
    uint32 session_id = 0x5555;
    int uni_gport = 0;
    create_l3_intf_s in_intf;
    create_l3_intf_s out_intf;
    bcm_ip_t sipv4 = 0x6fffff00;
    bcm_ip_t sipv4_mask = 0xffffff00;
    bcm_failover_t failover_id_fec;
    int pppoe_out_ac = 0;
   
    /* IPv6 */
    bcm_ip6_t route_ip6 =
        { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x12, 0x34, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x10 };
    bcm_ip6_t route_sip6 =
        { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x12, 0x34, 0x00, 0x00, 0x00, 0x00, 0xEE, 0x10 };
    bcm_ip6_t mask_sip6 = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00 };
    bcm_ip6_t mask_ip6 = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0 };
    bcm_ip6_t route_ip6_nni2uni =
        { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x12, 0x34, 0x00, 0x00, 0x00, 0x11, 0xFF, 0x10 };
    bcm_ip6_t mask_ip6_nni2uni = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0 };

    bcm_port_class_set(unit, in_port, bcmPortClassId, in_port);
    bcm_port_class_set(unit, out_port, bcmPortClassId, out_port);
    /*
     * Create vlan port(ingress: FEC-format B; egress: big AC(AC+PPPoE session ID))
     */
    rv = vlan_port_intf_create_with_fec(unit, in_port, intf_in, session_id, &uni_gport, &pppoe_out_ac); /* Create in/out ACs, out-ac includes pppoe session ID */ 
    if (rv != BCM_E_NONE)
    {
        printf("Error, vlan_port_intf_create_with_fec intf_in\n");
        return rv;
    }

    /*********************************************
     *  UNI ---> NNI
     ***************************************/
    rv =  bcm_vlan_create(unit, intf_in);
    if (rv != BCM_E_NONE)
    {
        printf("Error, bcm_vlan_create intf_in\n");
        return rv;
    }
    
    rv =  bcm_vswitch_port_add(unit, intf_in, uni_gport);
    if (rv != BCM_E_NONE)
    {
        printf("Error, bcm_vswitch_port_add intf_in\n");
        return rv;
    }

    /*
     * Create ETH-RIF and set its properties 
     */
    in_intf.vsi = intf_in;
    in_intf.my_global_mac = intf_in_mac_address;
    in_intf.my_lsb_mac = intf_in_mac_address;
    in_intf.urpf_mode = bcmL3IngressUrpfDisable;
    in_intf.vrf = vrf;
    in_intf.vrf_valid = 1;
    rv =  l3__intf_rif__create(unit, &in_intf);
    if (rv != BCM_E_NONE)
    {
        printf("Error, intf_eth_rif_create intf_in\n");
        return rv;
    }

    create_l3_egress_s l3_eg;
    sal_memcpy(l3_eg.next_hop_mac_addr, arp_next_hop_mac, 6);
    l3_eg.out_tunnel_or_rif = intf_out; 
    l3_eg.vlan   = intf_out;
    l3_eg.out_gport   = out_port;
    rv = l3__egress__create(unit,&l3_eg);
    if (rv != BCM_E_NONE) {
        printf ("l3__egress__create failed: %d \n", rc);
    }
    fec = l3_eg.fec_id;

    rv = add_route_ipv4(unit, route, mask, vrf, fec);
    if (rv != BCM_E_NONE)
    {
        printf("Error, in function add_route_ipv4, \n");
        return rv;
    }

    rv = add_host_ipv4(unit, host, vrf, fec, 0, 0);
    if (rv != BCM_E_NONE)
    {
        printf("Error, in function add_host_ipv4\n");
        return rv;
    }

    /* IPv6 */
    /* Add IPv6 route entry - UNI-->NNI */
    rv = add_route_ipv6(unit, route_ip6, mask_ip6, vrf, fec);
    if (rv != BCM_E_NONE)
    {
        printf("Error, in function add_route_ipv4, \n");
        return rv;
    }

    /* Binding anti-spoofing entry on uni_gport */
    rv = bind_PPPoE_anti_spoofing(unit, uni_gport, session_id);
    if (rv != BCM_E_NONE)
    {
        printf("Error, in function bind_PPPoE_anti_spoofing \n");
        return rv;
    }

    rv = bind_sipv4_anti_spoofing(unit, uni_gport, sipv4, sipv4_mask); /* add SIPv4 anti-spoofing entry to the KAPS. */
    if (rv != BCM_E_NONE)
    {
        printf("Error, in function bind_sipv4_anti_spoofing \n");
        return rv;
    }

    rv = bind_sipv6_anti_spoofing(unit, uni_gport, route_sip6, mask_sip6);
    if (rv != BCM_E_NONE)
    {
        printf("Error, in function bind_sipv6_anti_spoofing \n");
        return rv;
    }

    /*********************************************
     *  NNI ---> UNI
     ***************************************/
    out_intf.vsi = intf_out;
    out_intf.my_global_mac = intf_out_mac_address;
    out_intf.my_lsb_mac = intf_out_mac_address;
    out_intf.urpf_mode = bcmL3IngressUrpfDisable;
    out_intf.vrf = vrf;
    out_intf.vrf_valid = 1;
    rv =  l3__intf_rif__create(unit, &out_intf);
    if (rv != BCM_E_NONE)
    {
        printf("Error, intf_eth_rif_create intf_out\n");
        return rv;
    }


    if (!is_device_or_above(unit,JERICHO_B0)) {
        /* create failover id*/
        rv = bcm_failover_create(unit, BCM_FAILOVER_FEC, &failover_id_fec);
        if (rv != BCM_E_NONE) {
            printf("Error, bcm_failover_create 1\n");
            return rv;
        }
    }

    create_l3_egress_s l3_eg_in;
    sal_memcpy(l3_eg_in.next_hop_mac_addr, arp_next_hop_mac_in, 6);
    l3_eg_in.out_tunnel_or_rif = intf_in; 
    l3_eg_in.vlan   = intf_in;
    BCM_L3_ITF_FEC_TO_GPORT_FORWARD_GROUP(l3_eg_in.out_gport, (uni_gport & 0xFFFF | (1<<29)));
    l3_eg_in.failover_id = failover_id_fec;
    l3_eg_in.failover_if_id = 0;

    if (!is_device_or_above(unit,JERICHO_B0)) {
        /* Create protected FEC */
        rv = l3__egress__create(unit, &l3_eg_in);
        if (rv != BCM_E_NONE) {
            printf("Error, l3__egress__create\n");
          return rv;
        }
        l3_eg_in.failover_if_id = l3_eg_in.fec_id;
        l3_eg_in.arp_encap_id = 0;
    }
    
    /* primary FEC */
    rv = l3__egress__create(unit, &l3_eg_in);
    if (rv != BCM_E_NONE) {
        printf("Error, l3__egress__create\n");
        return rv;
    }

    fec = l3_eg_in.fec_id;

    rv = add_route_ipv4(unit, route_nni2uni, mask_nni2uni, vrf, fec);
    if (rv != BCM_E_NONE)
    {
        printf("Error, in function add_route_ipv4, \n");
        return rv;
    }

    rv = add_host_ipv4(unit, host_nni2uni, vrf, fec, 0, 0);
    if (rv != BCM_E_NONE)
    {
        printf("Error, in function add_host_ipv4, \n");
        return rv;
    }

    /* Add IPv6 route entry - NNI-->UNI */
    rv = add_route_ipv6(unit, route_ip6_nni2uni, mask_ip6_nni2uni, vrf, fec);
    if (rv != BCM_E_NONE)
    {
        printf("Error, in function add_route_ipv4\n");
        return rv;
    }

    /*UNI==>NNI Create egress MC */
    int mc_id = 6001;
    bcm_multicast_replication_t reps[2];
    int mc_flags = BCM_MULTICAST_EGRESS_GROUP|BCM_MULTICAST_TYPE_L3|BCM_MULTICAST_WITH_ID;
    rv = bcm_multicast_create(unit, mc_flags, &mc_id);
    if (rv != BCM_E_NONE)
    {
        printf("Error, in function bcm_multicast_create\n");
        return rv;
    }

    bcm_l3_egress_t l3eg2;
    int intf_out_2 = 200;         /* Outgoing packet VLAN */
    bcm_mac_t arp_next_hop_mac_2 = { 0x00, 0x00, 0x00, 0x00, 0xdd, 0x1d };        /* next_hop_mac - DA from UNI->NNI */
    bcm_if_t intf = 0;
    bcm_mac_t intf_out_mac_addr2 = { 0x00, 0x11, 0x00, 0x00, 0x00, 0x33 };    /* my-MAC - trerminating L2 NNI->UNI */

    create_l3_intf_s out_intf_2;
    out_intf_2.vsi = intf_out_2;
    out_intf_2.my_global_mac = intf_out_mac_addr2;
    out_intf_2.my_lsb_mac = intf_out_mac_addr2;
    out_intf_2.urpf_mode = bcmL3IngressUrpfDisable;
    out_intf_2.vrf = vrf;
    out_intf_2.vrf_valid = 1;
    rv =  l3__intf_rif__create(unit, &out_intf_2);
    if (rv != BCM_E_NONE)
    {
      printf("Error, intf_eth_rif_create out_intf_2\n");
      return rv;
    }

    bcm_l3_egress_t_init(&l3eg2);
    sal_memcpy(l3eg2.mac_addr, arp_next_hop_mac_2, 6);
    l3eg2.vlan   = intf_out_2;
    l3eg2.port   = out_port;
    l3eg2.flags  = BCM_L3_EGRESS_ONLY;
    rv = bcm_l3_egress_create(unit, 0, &l3eg2, &intf);
    if (rv != BCM_E_NONE)
    {
        printf("Error, in function bcm_l3_egress_create, create 2nd arp pointer\n");
        return rv;
    }

    BCM_GPORT_LOCAL_SET(reps[0].port, out_port);
    reps[0].encap1 = l3_eg.arp_encap_id & 0x3ffff; /* 1st ARP */
    BCM_GPORT_LOCAL_SET(reps[1].port, out_port);
    reps[1].encap1 = l3eg2.encap_id & 0x3ffff; /* 2nd ARP */
    rv = bcm_multicast_set(unit, mc_id, 0, 2, reps);
    if (rv != BCM_E_NONE)
    {
        printf("Error, in function bcm_multicast_set\n");
        return rv;
    }

    /*UNI==>NNI IPv4 MC*/
    bcm_ipmc_addr_t data;
    bcm_ip_t mc_ip_addr = 0xE0E0E002;
    bcm_ip_t mc_ip_mask = 0xffffffff;

    bcm_ipmc_addr_t_init(&data);
    data.mc_ip_addr = mc_ip_addr;
    data.mc_ip_mask = mc_ip_mask;
    data.vrf = vrf;
    data.group = mc_id;
    rv = bcm_ipmc_add(unit, &data);
    if (rv != BCM_E_NONE)
    {
        printf("Error, in function bcm_ipmc_add for IPv4 MC\n");
        return rv;
    }

    /*UNI==>NNI IPv6 MC*/
    bcm_ip6_t full_mask  = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
    bcm_ip6_t dip_mc_ip6  = {0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x12,0x12};
    bcm_ipmc_addr_t_init(&data);
    data.mc_ip6_addr = dip_mc_ip6;
    data.mc_ip6_mask = full_mask;
    data.flags = BCM_IPMC_IP6;
    data.group = mc_id;
    data.vid = intf_in;
    data.vrf = vrf;
    rv = bcm_ipmc_add(unit,&data);
    if (rv != BCM_E_NONE)
    {
        printf("Error, in function bcm_ipmc_add for IPv6 MC\n");
        return rv;
    }

    /*NNI==>UNI Create ingress MC */
    mc_id = 6002;
    if (ing_mc_nni2uni)
    {
        mc_flags = BCM_MULTICAST_INGRESS_GROUP|BCM_MULTICAST_TYPE_L3|BCM_MULTICAST_WITH_ID;
    }
    else
    {
        mc_flags = BCM_MULTICAST_EGRESS_GROUP|BCM_MULTICAST_TYPE_L3|BCM_MULTICAST_WITH_ID;
    }
    rv = bcm_multicast_create(unit, mc_flags, &mc_id);
    if (rv != BCM_E_NONE)
    {
        printf("Error, in function bcm_multicast_create for MC(NNI==>UNI), ing_mc_nni2uni:%d\n", ing_mc_nni2uni);
        return rv;
    }

    if (ing_mc_nni2uni)
    {
        rv = bcm_multicast_ingress_add(unit, mc_id, in_port, (l3_eg_in.arp_encap_id & 0x3ffff));
        if (rv != BCM_E_NONE) {
            printf("Error, bcm_multicast_ingress_add with l3_eg_in.arp_encap_id:%d\n", l3_eg_in.arp_encap_id);
            return rv;
        }
        /* Add esem entry to get out_ac */
        bcm_port_match_info_t match_info;
        bcm_port_match_info_t_init(&match_info);
        match_info.match = BCM_PORT_MATCH_PORT_VLAN;
        match_info.flags = BCM_PORT_MATCH_EGRESS_ONLY;
        match_info.port = in_port;
        match_info.match_vlan = intf_in;
        rv = bcm_port_match_add(unit, uni_gport, &match_info);
        if (rv != BCM_E_NONE)
        {
            printf("Error, in function bcm_port_match_add for MC(NNI==>UNI)\n");
            return rv;
        }
    }
    else
    {
        bcm_multicast_replication_t nni2uni_reps[1];

        BCM_GPORT_LOCAL_SET(nni2uni_reps[0].port, in_port);
        nni2uni_reps[0].encap1 = l3_eg_in.arp_encap_id & 0x3ffff; /* ARP */
        nni2uni_reps[0].encap2 = pppoe_out_ac; /* PPPoE out_ac */
        nni2uni_reps[0].flags = BCM_MUTICAST_REPLICATION_ENCAP2_VALID;
        rv = bcm_multicast_set(unit, mc_id, 0, 1, nni2uni_reps);
        if (rv != BCM_E_NONE)
        {
            printf("Error, in function bcm_multicast_set for MC(NNI==>UNI)\n");
            return rv;
        }
    }

    mc_ip_addr = 0xE0E0E003;
    bcm_ipmc_addr_t_init(&data);
    data.mc_ip_addr = mc_ip_addr;
    data.mc_ip_mask = mc_ip_mask;
    data.vrf = vrf;
    data.group = mc_id;
    rv = bcm_ipmc_add(unit, &data);
    if (rv != BCM_E_NONE)
    {
        printf("Error, in function bcm_ipmc_add for IPv4 MC(NNI==>UNI)\n");
        return rv;
    }

    bcm_ip6_t nni2uni_dip_mc_ip6  = {0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x13,0x13};
    bcm_ipmc_addr_t_init(&data);
    data.mc_ip6_addr = nni2uni_dip_mc_ip6;
    data.mc_ip6_mask = full_mask;
    data.flags = BCM_IPMC_IP6;
    data.group = mc_id;
    data.vid = intf_out;
    data.vrf = vrf;
    rv = bcm_ipmc_add(unit,&data);
    if (rv != BCM_E_NONE)
    {
        printf("Error, in function bcm_ipmc_add for IPv6 MC(NNI==>UNI)\n");
        return rv;
    }

    return rv;
}


