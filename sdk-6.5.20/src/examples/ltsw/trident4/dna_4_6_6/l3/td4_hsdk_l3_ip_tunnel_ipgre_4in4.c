/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */
/*  Feature  : L3 Ipv4 in Ipv4 GRE Tunnel 
 *
 *  Usage    : BCM.0> cint td4_hsdk_l3_ip_tunnel_ipgre_4in4.c  
 *
 *  config   : bcm56880_a0-generic-32x400.config.yml   
 *
 *  Log file : td4_hsdk_l3_ip_tunnel_ipgre_4in4_log.txt
 *
 *  Test Topology :
 *
 *                   +------------------------------+
 *                   |                              |
 *                   |                              |
 *                   |                              |
 *   access_port     |                              |  network_port
 *  +----------------+          SWITCH-TD4          +-----------------+
 *                   |                              |
 *                   |                              |
 *                   |                              |
 *                   |                              |
 *                   |                              |
 *                   +------------------------------+
 *  Summary:
 *  ========
 *     This cint example demonstrates L3 Ipv4 in Ipv4 GRE tunnel initiation and termination
 *     using BCM APIs.
 *
 *  Detailed steps done in the CINT script:
 *  =======================================
 *    1) Step1 - Test Setup (Done in test_setup()):
 *    =============================================
 *     a) Selects two ports and configure them in Loopback mode. Out of these
 *        two ports, one port is used as ingress_port and the other as
 *        egress_port.
 *
 *     b) Install an IFP rule to copy incoming and outgoing packets to CPU and
 *        start packet watcher to display packet contents.
 *
 *     Note: IFP rule is meant for a testing purpose only (Internal) and it is
 *           nothing to do with an actual functional test.
 *
 *    2) Step2 - Configuration (Done in config_l3_tunnel_ipgre())
 *    ===========================================================
 *      a) Create an access_vlan(21) and add access_port as member.
 *
 *      b) Create network_vlan(22) and add network_port as member.
 *
 *      c) Create two L3 interfaces one each for access side and network side.
 *
 *      d) Create two L3 egress objects one each for access side and network side.
 *
 *      e) Configure tunnel initiator(type = bcmTunnelTypeGreAnyIn4)
 *         on network side L3 interface.
 *
 *      f) Configure tunnel terminator(type = bcmTunnelTypeGreAnyIn4)
 *         to match on packet's tunnel header contents.
 *
 *      g) For tunnel initiation, add a L3 route pointing to network side egress object.
 *
 *      h) For tunnel termination, add a L3 route pointing to access side egress object.
 *
 *    3) Step3 - Verification (Done in verify()):
 *    ===========================================
 *      a) For IP-GRE tunnel initiation, send the below packet on access_port.
 *
 *          Ethernet II, Src: 00:00:00_00:00:01 (00:00:00:00:00:01), Dst: 00:00:00_00:11:11 (00:00:00:00:11:11)
 *          802.1Q Virtual LAN, PRI: 0, CFI: 0, ID: 21
 *          Internet Protocol Version 4, Src: 2.2.2.2, Dst: 1.1.1.1
 *          User Datagram Protocol, Src Port: 63, Dst Port: 63
 *          Data (14 bytes)
 *          0000 00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d ..............
 *
 *          0000 0000 1111 0000 0000 0001 8100 0015
 *          0800 4500 002a 0000 0000 4011 74be 0202
 *          0202 0101 0101 003f 003f 0016 cf0d 0001
 *          0203 0405 0607 0809 0a0b 0c0d f971 7221
 *
 *      b)For IP-GRE Tunnel Termination, send the below packet on network_port
 *         Ethernet II, Src: 00:00:00_00:00:02 (00:00:00:00:00:02), Dst: 00:00:00_00:22:22 (00:00:00:00:22:22)
 *         802.1Q Virtual LAN, PRI: 0, CFI: 0, ID: 22
 *         Internet Protocol Version 4, Src: 9.9.9.9, Dst: 10.10.10.10
 *         Generic Routing Encapsulation (IP)
 *         Internet Protocol Version 4, Src: 1.1.1.1, Dst: 2.2.2.2
 *         User Datagram Protocol, Src Port: 63, Dst Port: 63
 *
 *         0000 0000 2222 0000 0000 0002 8100 0016
 *         0800 4500 0034 0000 0000 402f 5476 0909
 *         0909 0a0a 0a0a 0000 0800 4500 001c 0000
 *         0000 4011 74cc 0101 0101 0202 0202 003f
 *         003f 0008 f95a 0e73 0ded
 *
 *       c) Expected Result:
 *       ===================
 *        After step 3.a, verify that the below IP-GRE tunnel packet egresses out
 *        of network_port.
 *
 *         Ethernet II, Src: 00:00:00_00:22:22 (00:00:00:00:22:22), Dst: 00:00:00_00:00:02 (00:00:00:00:00:02)
 *         802.1Q Virtual LAN, PRI: 0, CFI: 0, ID: 22
 *         Internet Protocol Version 4, Src: 10.10.10.10, Dst: 9.9.9.9
 *         Generic Routing Encapsulation (IP)
 *         Flags and Version: 0x0000
 *         Protocol Type: IP (0x0800)
 *         Internet Protocol Version 4, Src: 2.2.2.2, Dst: 1.1.1.1
 *         User Datagram Protocol, Src Port: 63, Dst Port: 63
 *         Data (14 bytes)
 *         0000 00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d ..............
 *
 *         0000 0000 0002 0000 0000 2222 8100 0016
 *         0800 4500 0042 0000 0000 162f 7e68 0a0a
 *         0a0a 0909 0909 0000 0800 4500 002a 0000
 *         0000 3f11 75be 0202 0202 0101 0101 003f
 *         003f 0016 cf0d 0001 0203 0405 0607 0809
 *         0a0b 0c0d f971 7221
 *
 *        After step 3.b, verify that the below packet egresses out of access_port.
 *
 *         Ethernet II, Src: 00:00:00_00:11:11 (00:00:00:00:11:11), Dst: 00:00:00_00:00:01 (00:00:00:00:00:01)
 *         802.1Q Virtual LAN, PRI: 0, CFI: 0, ID: 21
 *         Internet Protocol Version 4, Src: 1.1.1.1, Dst: 2.2.2.2
 *         User Datagram Protocol, Src Port: 63, Dst Port: 63
 *
 *         0000 0000 0001 0000 0000 1111 8100 0015
 *         0800 4500 001c 0000 0000 3f11 75cc 0101
 *         0101 0202 0202 003f 003f 0008 f95a 0e73
 *         0ded
*/

cint_reset();
bcm_port_t      access_port;
bcm_port_t      network_port;

/* This function is written so that hardcoding of port
   numbers in Cint scripts is removed. This function gives
   required number of ports
*/
bcm_error_t portNumbersGet(int unit, int *port_list, int num_ports)
{

  int i=0,port=0,rv=0;
  bcm_port_config_t configP;
  bcm_pbmp_t ports_pbmp;

  rv = bcm_port_config_get(unit,&configP);
  if(BCM_FAILURE(rv)) {
    printf("\nError in retrieving port configuration: %s.\n",bcm_errmsg(rv));
    return rv;
  }

  ports_pbmp = configP.e;
  for (i= 1; i < BCM_PBMP_PORT_MAX; i++) {
    if (BCM_PBMP_MEMBER(&ports_pbmp,i)&& (port < num_ports)) {
      port_list[port]=i;
      port++;
    }
  }

  if (( port == 0 ) || ( port != num_ports )) {
      printf("portNumbersGet() failed \n");
      return -1;
  }

  return BCM_E_NONE;

}

/*
 * Configures the port in loopback mode and installs
 * an IFP rule. This IFP rule copies the packets ingressing
 * on the specified port to CPU.
 */
bcm_error_t ingress_port_setup(int unit, bcm_port_t port)
{
    bcm_field_entry_t        entry;
    bcm_field_group_config_t group_config; 

    BCM_IF_ERROR_RETURN(bcm_port_loopback_set(unit, port, BCM_PORT_LOOPBACK_MAC));

    bcm_field_group_config_t_init(&group_config);
    BCM_FIELD_QSET_INIT(group_config.qset);
    BCM_FIELD_QSET_ADD(group_config.qset, bcmFieldQualifyStageIngress);
    BCM_FIELD_QSET_ADD(group_config.qset, bcmFieldQualifyInPort);
    BCM_FIELD_ASET_ADD(group_config.aset, bcmFieldActionCopyToCpu);
    BCM_FIELD_ASET_ADD(group_config.aset, bcmFieldActionDrop);
    group_config.priority = 1;
    BCM_IF_ERROR_RETURN(bcm_field_group_config_create(unit, &group_config));

    BCM_IF_ERROR_RETURN(bcm_field_entry_create(unit, group_config.group, &entry));
    BCM_IF_ERROR_RETURN(bcm_field_qualify_InPort(unit, entry, port, BCM_FIELD_EXACT_MATCH_MASK));
    BCM_IF_ERROR_RETURN(bcm_field_action_add(unit, entry, bcmFieldActionCopyToCpu, 0, 0));

    BCM_IF_ERROR_RETURN(bcm_field_entry_install(unit, entry));

    printf("ingress_port_setup configured for ingress_port : %d\n", port);

    return BCM_E_NONE;
}

/*
 * Configures the port in loopback mode and installs
 * an IFP rule. This IFP rule copies the packets ingressing
 * on the specified port to CPU. Port is also configured
 * to discard all packets. This is to avoid continuous
 * loopback of the packet.
 */
bcm_error_t egress_port_setup(int unit, bcm_port_t port)
{
    bcm_field_entry_t        entry;
    bcm_field_group_config_t group_config; 

    BCM_IF_ERROR_RETURN(bcm_port_loopback_set(unit, port, BCM_PORT_LOOPBACK_MAC));

    bcm_field_group_config_t_init(&group_config);
    BCM_FIELD_QSET_INIT(group_config.qset);
    BCM_FIELD_QSET_ADD(group_config.qset, bcmFieldQualifyStageIngress);
    BCM_FIELD_QSET_ADD(group_config.qset, bcmFieldQualifyInPort);
    BCM_FIELD_ASET_ADD(group_config.aset, bcmFieldActionCopyToCpu);
    BCM_FIELD_ASET_ADD(group_config.aset, bcmFieldActionDrop);
    group_config.priority = port;
    BCM_IF_ERROR_RETURN(bcm_field_group_config_create(unit, &group_config));

    BCM_IF_ERROR_RETURN(bcm_field_entry_create(unit, group_config.group, &entry));

    BCM_IF_ERROR_RETURN(bcm_field_qualify_InPort(unit, entry, port, BCM_FIELD_EXACT_MATCH_MASK));
    BCM_IF_ERROR_RETURN(bcm_field_action_add(unit, entry, bcmFieldActionCopyToCpu, 0, 0));
    BCM_IF_ERROR_RETURN(bcm_field_action_add(unit, entry, bcmFieldActionDrop, 0, 0));

    BCM_IF_ERROR_RETURN(bcm_field_entry_install(unit, entry));

    printf("egress_port_setup configured for egress_port : %d\n", port);

    return BCM_E_NONE;
}

/*
 * This functions gets the port numbers and sets up ingress and
 * egress ports. Check ingress_port_setup() and egress_port_setup().
 */
bcm_error_t test_setup(int unit)
{
  int port_list[2];

  if (BCM_E_NONE != portNumbersGet(unit, port_list, 2)) {
    printf("portNumbersGet() failed\n");
    return -1;
  }

  access_port = port_list[0];
  network_port = port_list[1];

  if (BCM_E_NONE != ingress_port_setup(unit, access_port)) {
    printf("ingress_port_setup() failed for port %d\n", access_port);
    return -1;
  }

  if (BCM_E_NONE != egress_port_setup(unit, network_port)) {
      printf("egress_port_setup() failed for port %d\n", network_port);
      return -1;
  }

  bshell(unit, "pw start report +raw +decode");
  return BCM_E_NONE;
}

/*
 * Configure IP-GRE tunnel initiation and termination.
 */
bcm_error_t config_l3_tunnel_ipgre(int unit)
{
  bcm_error_t       rv;
  bcm_gport_t       access_gport;
  bcm_gport_t       network_gport;
  bcm_l3_intf_t     ing_l3_intf;
  bcm_l3_intf_t     egr_l3_intf;
  bcm_if_t          l3_egr_def_obj_id = 100000 + 1;
  bcm_if_t          l3_egr_obj_id_i = 100000 + 2;
  bcm_if_t          l3_egr_obj_id = 100000 + 3;
  bcm_if_t          ul_l3_egr_obj_id = 100000 + 32768 + 1; /* Underlay offset added */
  bcm_l3_egress_t   l3_egr_def;
  bcm_l3_egress_t   l3_egr_obj_i;
  bcm_l3_egress_t   l3_egr_obj;
  bcm_l3_ingress_t  l3_ingress;
  int               tnl_term_flags;

  bcm_vlan_control_vlan_t   vlan_ctrl;
  bcm_tunnel_initiator_t    tnl_init_info;
  bcm_tunnel_terminator_t   tnl_term_info;

  bcm_l3_route_t route_1;
  bcm_l3_route_t route_2;
  bcm_l3_route_t route_d;

  bcm_ip_t tnl_init_dip = 0x01010101; /* 1.1.1.1 */
  bcm_ip_t tnl_term_dip = 0x02020202; /* 2.2.2.2 */

  bcm_ip_t tnl_init_outer_sip = 0x0a0a0a0a; /* 10.10.10.10 */
  bcm_ip_t tnl_init_outer_dip = 0x09090909; /* 9.9.9.9 */
  bcm_ip_t tnl_term_outer_dip = 0x0a0a0a0a;
  bcm_ip_t tnl_term_outer_sip = 0x09090909;

  bcm_mac_t local_mac_1 = {0x00, 0x00, 0x00, 0x00, 0x11, 0x11};
  bcm_mac_t local_mac_2 = {0x00, 0x00, 0x00, 0x00, 0x22, 0x22};
  bcm_mac_t remote_mac_1 = {0x00, 0x00, 0x00, 0x00, 0x00, 0x01};
  bcm_mac_t remote_mac_2 = {0x00, 0x00, 0x00, 0x00, 0x00, 0x02};
  bcm_mac_t mac_mask = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

  bcm_vrf_t   vrf = 1;
  bcm_vlan_t  access_vlan = 21;
  bcm_vlan_t  network_vlan = 22;
  int         intf_id_1 = 1;
  int         intf_id_2 = 2;
  int         ul_intf_id_2 = 2 + 0x4000;  /* Underlay offset addded */

  int              station_id;
  bcm_l2_station_t station;

  rv = bcm_port_gport_get(unit, access_port, &access_gport);
  if (BCM_FAILURE(rv)) {
      printf("Error executing bcm_port_gport_get(): %s.\n", bcm_errmsg(rv));
      return rv;
  }

  rv = bcm_port_gport_get(unit, network_port, &network_gport);
  if (BCM_FAILURE(rv)) {
      printf("Error executing bcm_port_gport_get(): %s.\n", bcm_errmsg(rv));
      return rv;
  }

  /* Create access_vlan  */
  rv = bcm_vlan_create(unit, access_vlan);
  if (BCM_FAILURE(rv)) {
      printf("Error executing bcm_vlan_create(): %s.\n", bcm_errmsg(rv));
      return rv;
  }

  rv = bcm_vlan_gport_add(unit, access_vlan, access_gport, 0);
  if (BCM_FAILURE(rv)) {
      printf("Error executing bcm_vlan_gport_add(): %s.\n", bcm_errmsg(rv));
      return rv;
  }

  /* Create network vlan */
  rv = bcm_vlan_create(unit, network_vlan);
  if (BCM_FAILURE(rv)) {
      printf("Error executing bcm_vlan_create(): %s.\n", bcm_errmsg(rv));
      return rv;
  }

  rv = bcm_vlan_gport_add(unit, network_vlan, network_gport, 0);
  if (BCM_FAILURE(rv)) {
      printf("Error executing bcm_vlan_gport_add(): %s.\n", bcm_errmsg(rv));
      return rv;
  }

  /* Create L3 ingress interface for access */
  bcm_l3_ingress_t_init(&l3_ingress);
  l3_ingress.flags       = BCM_L3_INGRESS_WITH_ID;
  bcm_if_t ingress_if_ac = 0x1b2;
  l3_ingress.vrf         = vrf;
  rv = bcm_l3_ingress_create(unit, &l3_ingress, &ingress_if_ac);
  if (BCM_FAILURE(rv)) {
      printf("Error executing bcm_l3_ingress_create(): %s.\n", bcm_errmsg(rv));
      return rv;
  }

  /* Config vlan_id to l3_iif mapping for access */
  bcm_vlan_control_vlan_t_init(&vlan_ctrl);
  rv = bcm_vlan_control_vlan_get(unit, access_vlan, &vlan_ctrl);
  if (BCM_FAILURE(rv)) {
      printf("Error executing bcm_vlan_control_vlan_get(): %s.\n", bcm_errmsg(rv));
      return rv;
  }
  vlan_ctrl.ingress_if = ingress_if_ac;
  rv = bcm_vlan_control_vlan_set(unit, access_vlan, vlan_ctrl);
  if (BCM_FAILURE(rv)) {
      printf("Error executing bcm_vlan_control_vlan_set(): %s.\n", bcm_errmsg(rv));
      return rv;
  }

  /* Create L3 ingress interface for network */
  bcm_l3_ingress_t_init(&l3_ingress);
  l3_ingress.flags       = BCM_L3_INGRESS_WITH_ID;
  bcm_if_t ingress_if_nw = 0x1b3;
  l3_ingress.vrf         = vrf;
  rv = bcm_l3_ingress_create(unit, &l3_ingress, &ingress_if_nw);
  if (BCM_FAILURE(rv)) {
      printf("Error executing bcm_l3_ingress_create(): %s.\n", bcm_errmsg(rv));
      return rv;
  }

  /* Config vlan_id to l3_iif mapping for network */
  bcm_vlan_control_vlan_t_init(&vlan_ctrl);
  rv = bcm_vlan_control_vlan_get(unit, network_vlan, &vlan_ctrl);
  if (BCM_FAILURE(rv)) {
      printf("Error executing bcm_vlan_control_vlan_get(): %s.\n", bcm_errmsg(rv));
      return rv;
  }
  vlan_ctrl.ingress_if = ingress_if_nw;
  rv = bcm_vlan_control_vlan_set(unit, network_vlan, vlan_ctrl);
  if (BCM_FAILURE(rv)) {
      printf("Error executing bcm_vlan_control_vlan_set(): %s.\n", bcm_errmsg(rv));
      return rv;
  }

  /* Create access side L3 interface */
  bcm_l3_intf_t_init(&ing_l3_intf);
  ing_l3_intf.l3a_flags   = BCM_L3_WITH_ID;
  ing_l3_intf.l3a_intf_id = intf_id_1;
  ing_l3_intf.l3a_vid     = access_vlan;
  ing_l3_intf.l3a_vrf     = vrf;
  sal_memcpy(ing_l3_intf.l3a_mac_addr, local_mac_1, 6);
  rv = bcm_l3_intf_create(unit, &ing_l3_intf);
  if (BCM_FAILURE(rv)) {
      printf("Error executing bcm_l3_intf_create(): %s.\n", bcm_errmsg(rv));
      return rv;
  }

  /* Add tunnel initiation towards network */
  bcm_tunnel_initiator_t_init(&tnl_init_info);
  tnl_init_info.type = bcmTunnelTypeGreAnyIn4;
  tnl_init_info.ttl  = 0x16;
  tnl_init_info.dip = tnl_init_outer_dip;
  tnl_init_info.sip = tnl_init_outer_sip;
  sal_memcpy(tnl_init_info.dmac, remote_mac_2, 6);
  rv = bcm_tunnel_initiator_create(unit, NULL, &tnl_init_info);
  if (BCM_FAILURE(rv)) {
      printf("Error executing bcm_tunnel_initiator_create(): %s.\n", bcm_errmsg(rv));
      return rv;
  }

  /* Create network side L3 interface */
  bcm_l3_intf_t_init(&egr_l3_intf);
  egr_l3_intf.l3a_flags      = BCM_L3_WITH_ID;
  egr_l3_intf.l3a_intf_id    = intf_id_2;
  egr_l3_intf.l3a_vid        = network_vlan;
  egr_l3_intf.l3a_vrf        = vrf;
  egr_l3_intf.l3a_tunnel_idx = tnl_init_info.tunnel_id;
  sal_memcpy(egr_l3_intf.l3a_mac_addr, local_mac_2, 6);
  rv =  bcm_l3_intf_create(unit, &egr_l3_intf);
  if (BCM_FAILURE(rv)) {
      printf("Error executing bcm_l3_intf_create(): %s.\n", bcm_errmsg(rv));
      return rv;
  }

  /* Create Underlay network side L3 interface */
  bcm_l3_intf_t_init(&egr_l3_intf);
  egr_l3_intf.l3a_flags      = BCM_L3_WITH_ID;
  egr_l3_intf.l3a_intf_flags = BCM_L3_INTF_UNDERLAY;
  egr_l3_intf.l3a_intf_id    = ul_intf_id_2;
  egr_l3_intf.l3a_vid        = network_vlan;
  egr_l3_intf.l3a_vrf        = vrf;
  sal_memcpy(egr_l3_intf.l3a_mac_addr, local_mac_2, 6);
  rv =  bcm_l3_intf_create(unit, &egr_l3_intf);
  if (BCM_FAILURE(rv)) {
      printf("Error executing bcm_l3_intf_create(): %s.\n", bcm_errmsg(rv));
      return rv;
  }

  /* Create access side L3 egress object*/
  bcm_l3_egress_t_init(&l3_egr_obj_i);
  l3_egr_obj_i.flags   = 0;
  l3_egr_obj_i.intf    = intf_id_1;
  l3_egr_obj_i.vlan    = access_vlan;
  l3_egr_obj_i.port    = access_gport;
  sal_memcpy(l3_egr_obj_i.mac_addr, remote_mac_1, 6);
  rv = bcm_l3_egress_create(unit, BCM_L3_WITH_ID, &l3_egr_obj_i, &l3_egr_obj_id_i);
  if (BCM_FAILURE(rv)) {
      printf("Error executing bcm_l3_egress_create(): %s.\n", bcm_errmsg(rv));
      return rv;
  }

  /* Create network side L3 egress object */
  bcm_l3_egress_t_init(&l3_egr_obj);
  l3_egr_obj.flags = 0;
  l3_egr_obj.intf  = intf_id_2;
  l3_egr_obj.vlan  = network_vlan;
  l3_egr_obj.port  = network_gport;
  sal_memcpy(l3_egr_obj.mac_addr, remote_mac_2, 6);
  rv = bcm_l3_egress_create(unit, BCM_L3_WITH_ID, &l3_egr_obj, &l3_egr_obj_id);
  if (BCM_FAILURE(rv)) {
      printf("Error executing bcm_l3_egress_create(): %s.\n", bcm_errmsg(rv));
      return rv;
  }

  /* Create Underlay network side L3 egress object */
  bcm_l3_egress_t_init(&l3_egr_obj);
  l3_egr_obj.flags  = 0;
  l3_egr_obj.flags2 = BCM_L3_FLAGS2_UNDERLAY;
  l3_egr_obj.intf   = ul_intf_id_2;
  l3_egr_obj.vlan   = network_vlan;
  l3_egr_obj.port   = network_gport;
  sal_memcpy(l3_egr_obj.mac_addr, remote_mac_2, 6);
  rv = bcm_l3_egress_create(unit, BCM_L3_WITH_ID, &l3_egr_obj, &ul_l3_egr_obj_id);
  if (BCM_FAILURE(rv)) {
      printf("Error executing bcm_l3_egress_create(): %s.\n", bcm_errmsg(rv));
      return rv;
  }

  /* Enable Tunnel Termination on NW port by setting ING_L2_IIF_TABLE.IPV4_TUNNEL_ENABLE=1 */
  rv = bcm_port_control_set(unit, network_port, bcmPortControlIP4TunnelTermEnable, 1);
  if (BCM_FAILURE(rv)) {
      printf("Error executing bcmPortControlIP4TunnelTermEnable: %s.\n", bcm_errmsg(rv));
      return rv;
  }

  /* Add tunnel termination */
  bcm_tunnel_terminator_t_init(&tnl_term_info);
  tnl_term_info.type     = bcmTunnelTypeGreAnyIn4;
  tnl_term_info.vlan     = ingress_if_nw;
  tnl_term_info.sip      = tnl_term_outer_sip;
  tnl_term_info.dip      = tnl_term_outer_dip;
  tnl_term_info.sip_mask = bcm_ip_mask_create(32);
  tnl_term_info.dip_mask = bcm_ip_mask_create(32);
  BCM_PBMP_PORT_SET(tnl_term_info.pbmp, network_port);
  rv = bcm_tunnel_terminator_add(unit, &tnl_term_info);
  if (BCM_FAILURE(rv)) {
      printf("Error executing bcm_tunnel_terminator_add(): %s.\n", bcm_errmsg(rv));
      return rv;
  }

  /* Create default egress object */ 
  bcm_l3_egress_t_init(&l3_egr_def);
  l3_egr_def.intf  = intf_id_2;
  l3_egr_def.flags = BCM_L3_COPY_TO_CPU | BCM_L3_DST_DISCARD;
  rv = bcm_l3_egress_create(unit, BCM_L3_WITH_ID, &l3_egr_def, &l3_egr_def_obj_id);
  if (BCM_FAILURE(rv)) {
      printf("Error in configuring def L3 egress object : %s.\n", bcm_errmsg(rv));
      return rv;
  }

  /* Add L3 default Route in the system fisrt */
  bcm_l3_route_t_init(&route_d);
  route_d.l3a_subnet  = 0x00000000;  /* 0.0.0.0 */
  route_d.l3a_ip_mask = 0x00000000;  /* 0.0.0.0 */
  route_d.l3a_intf    = l3_egr_def_obj_id;
  route_d.l3a_vrf     = vrf;
  rv = bcm_l3_route_add(unit, &route_d);
  if (BCM_FAILURE(rv)) {
      printf("Error executing bcm_l3_route_add(): %s.\n", bcm_errmsg(rv));
      return rv;
  }

  /* Add L3 Route for tunnel initiation */
  bcm_l3_route_t_init(&route_1);
  route_1.l3a_subnet  = tnl_init_dip;
  route_1.l3a_ip_mask = bcm_ip_mask_create(32);
  route_1.l3a_intf    = l3_egr_obj_id;
  route_1.l3a_vrf     = vrf;
  route_1.l3a_ul_intf = ul_l3_egr_obj_id;
  rv = bcm_l3_route_add(unit, &route_1);
  if (BCM_FAILURE(rv)) {
      printf("Error executing bcm_l3_route_add(): %s.\n", bcm_errmsg(rv));
      return rv;
  }

  /* Add L3 Route for tunnel termination */
  bcm_l3_route_t_init(&route_2);
  route_2.l3a_subnet   = tnl_term_dip;
  route_2.l3a_ip_mask  = bcm_ip_mask_create(32);
  route_2.l3a_intf     = l3_egr_obj_id_i;
  route_2.l3a_vrf      = vrf;
  rv = bcm_l3_route_add(unit, &route_2);
  if (BCM_FAILURE(rv)) {
      printf("Error executing bcm_l3_route_add(): %s.\n", bcm_errmsg(rv));
      return rv;
  }

  /* Add L2 MAC into l2 station table to enable L3 lookup for MAC + VLAN */
  bcm_l2_station_t_init(&station);
  station.vlan      = access_vlan;
  station.vlan_mask = access_vlan ? 0xfff: 0x0;
  sal_memcpy(station.dst_mac, local_mac_1, sizeof(local_mac_1));
  sal_memcpy(station.dst_mac_mask, mac_mask, sizeof(mac_mask));
  rv = bcm_l2_station_add(unit, &station_id, &station);
  if (BCM_FAILURE(rv)) {
      printf("Error executing bcm_l2_station_add(): %s.\n", bcm_errmsg(rv));
      return rv;
  }

  bcm_l2_station_t_init(&station);
  station.vlan      = network_vlan;
  station.vlan_mask = network_vlan ? 0xfff: 0x0;
  sal_memcpy(station.dst_mac, local_mac_2, sizeof(local_mac_2));
  sal_memcpy(station.dst_mac_mask, mac_mask, sizeof(mac_mask));
  rv = bcm_l2_station_add(unit, &station_id, &station);
  if (BCM_FAILURE(rv)) {
      printf("Error executing bcm_l2_station_add(): %s.\n", bcm_errmsg(rv));
      return rv;
  }

  return BCM_E_NONE;
}

void verify(int unit)
{
  char        str[512];
  bcm_error_t rv = BCM_E_NONE;

  bshell(unit, "vlan show");
  bshell(unit, "l3 ing_intf show");
  bshell(unit, "l3 intf show");
  bshell(unit, "l3 egress show");
  bshell(unit, "l3 route show");
  bshell(unit, "l3 route show v6=1");

 /*
  *  For IP-GRE tunnel initiation, send the below packet on access_port.
  *
  *    Ethernet II, Src: 00:00:00_00:00:01 (00:00:00:00:00:01), Dst: 00:00:00_00:11:11 (00:00:00:00:11:11)
  *    802.1Q Virtual LAN, PRI: 0, CFI: 0, ID: 21
  *    Internet Protocol Version 4, Src: 2.2.2.2, Dst: 1.1.1.1
  *    User Datagram Protocol, Src Port: 63, Dst Port: 63
  *    Data (14 bytes)
  *    0000 00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d ..............
  *
  *    0000 0000 1111 0000 0000 0001 8100 0015
  *    0800 4500 002a 0000 0000 4011 74be 0202
  *    0202 0101 0101 003f 003f 0016 cf0d 0001
  *    0203 0405 0607 0809 0a0b 0c0d f971 7221
  *
  *    Below IP-GRE tunnel packet egresses out of network_port.
  *
  *    Ethernet II, Src: 00:00:00_00:22:22 (00:00:00:00:22:22), Dst: 00:00:00_00:00:02 (00:00:00:00:00:02)
  *    802.1Q Virtual LAN, PRI: 0, CFI: 0, ID: 22
  *    Internet Protocol Version 4, Src: 10.10.10.10, Dst: 9.9.9.9
  *    Generic Routing Encapsulation (IP)
  *    Flags and Version: 0x0000
  *    Protocol Type: IP (0x0800)
  *    Internet Protocol Version 4, Src: 2.2.2.2, Dst: 1.1.1.1
  *    User Datagram Protocol, Src Port: 63, Dst Port: 63
  *    Data (14 bytes)
  *    0000 00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d ..............
  *
  *    0000 0000 0002 0000 0000 2222 8100 0016
  *    0800 4500 0042 0000 0000 162f 7e68 0a0a
  *    0a0a 0909 0909 0000 0800 4500 002a 0000
  *    0000 3f11 75be 0202 0202 0101 0101 003f
  *    003f 0016 cf0d 0001 0203 0405 0607 0809
  *    0a0b 0c0d f971 7221
  */

  printf("\nSending packet to access_port:%d\n", access_port);
  snprintf(str, 512, "tx 1 pbm=%d data=0x0000000011110000000000018100001508004500002A00000000401174BE0202020201010101003F003F0016CF0D000102030405060708090A0B0C0DF9717221; sleep quiet 1", access_port);
  bshell(unit, str);
  bshell(unit, "show c");

  /* Remove Drop for network port ingress */
  rv = bcm_field_action_remove(unit, 2, bcmFieldActionDrop);
  if (BCM_FAILURE(rv)) {
      printf("Error executing bcm_field_action_remove : %s.\n", bcm_errmsg(rv));
      return rv;
  }
  rv = bcm_field_entry_install(unit, 2);
  if (BCM_FAILURE(rv)) {
      printf("Error executing bcm_field_entry_install : %s.\n", bcm_errmsg(rv));
      return rv;
  }

  /* Add Drop for access port ingress */
  rv = bcm_field_action_add(unit, 1, bcmFieldActionDrop, 0, 0);
  if (BCM_FAILURE(rv)) {
      printf("Error executing bcm_field_action_add : %s.\n", bcm_errmsg(rv));
      return rv;
  }
  rv = bcm_field_entry_install(unit, 1);
  if (BCM_FAILURE(rv)) {
      printf("Error executing bcm_field_entry_install : %s.\n", bcm_errmsg(rv));
      return rv;
  }

 /*
  *  For IP-GRE Tunnel Termination, send the below packet on network_port
  *
  *    Ethernet II, Src: 00:00:00_00:00:02 (00:00:00:00:00:02), Dst: 00:00:00_00:22:22 (00:00:00:00:22:22)
  *    802.1Q Virtual LAN, PRI: 0, CFI: 0, ID: 22
  *    Internet Protocol Version 4, Src: 9.9.9.9, Dst: 10.10.10.10
  *    Generic Routing Encapsulation (IP)
  *    Internet Protocol Version 4, Src: 1.1.1.1, Dst: 2.2.2.2
  *    User Datagram Protocol, Src Port: 63, Dst Port: 63
  *
  *    0000 0000 2222 0000 0000 0002 8100 0016
  *    0800 4500 0034 0000 0000 402f 5476 0909
  *    0909 0a0a 0a0a 0000 0800 4500 001c 0000
  *    0000 4011 74cc 0101 0101 0202 0202 003f
  *    003f 0008 f95a 0e73 0ded
  *
  *    Below Tunnel packet egresses out of network_port.
  *
  *    Ethernet II, Src: 00:00:00_00:11:11 (00:00:00:00:11:11), Dst: 00:00:00_00:00:01 (00:00:00:00:00:01)
  *    802.1Q Virtual LAN, PRI: 0, CFI: 0, ID: 21
  *    Internet Protocol Version 4, Src: 1.1.1.1, Dst: 2.2.2.2
  *    User Datagram Protocol, Src Port: 63, Dst Port: 63
  *
  *    0000 0000 0001 0000 0000 1111 8100 0015
  *    0800 4500 001c 0000 0000 3f11 75cc 0101
  *    0101 0202 0202 003f 003f 0008 f95a 0e73
  *    0ded
  */

  printf("\nSending packet to network_port:%d\n", network_port);
  snprintf(str, 512, "tx 1 pbm=%d data=0x0000000022220000000000028100001608004500003400000000402F5476090909090A0A0A0A000008004500001C00000000401174CC0101010102020202003F003F0008F95A0E730DED; sleep quiet 1", network_port);
  bshell(unit, str);
  bshell(unit, "show c");
}

/*
 * This functions does the following
 * a)test setup
 * b)actual configuration (Done in config_l3_tunnel_ipgre())
 * c)demonstrates the functionality (Done in verify()).
 */
bcm_error_t execute()
{
  bcm_error_t rv;
  int unit = 0;

  bshell(unit, "config show; a ; version");

  if (BCM_FAILURE((rv = test_setup(unit)))) {
    printf("test_setup() failed.\n");
    return -1;
  }
  printf("Completed test setup successfully.\n");

  if (BCM_FAILURE((rv = config_l3_tunnel_ipgre(unit)))) {
    printf("config_l3_tunnel_ipgre() failed.\n");
    return -1;
  }
  printf("Completed configuration(i.e executing config_l3_tunnel_ipgre()) successfully.\n");

  verify(unit);
  return BCM_E_NONE;
}

const char *auto_execute = (ARGC == 1) ? ARGV[0] : "YES";
if (!sal_strcmp(auto_execute, "YES")) {
  print execute();
}

