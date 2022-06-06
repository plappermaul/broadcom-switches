/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

/*
 * Feature  : VPWS QoS mapping from access to network
 *
 * Usage    : BCM.0> cint vpws_qos_1.c
 *
 * config   : mpls_config.bcm
 *
 * Log file : vpws_qos_1_log.txt
 *
 * Test Topology :
 *
 *                  +------------------------------+
 *                  |                              |
 *                  |                              |
 *                  |                              |
 *     access_port  |                              |  network_port
 * +----------------+          SWITCH              +-----------------+
 *                  |                              |
 *                  |                              |
 *                  |                              |
 *                  |                              |
 *                  |                              |
 *                  +------------------------------+
 * Summary:
 * ========
 *   This Cint example to show configuration of VPWS QoS mapping from access-side
 *   to network-side using BCM APIs.
 *
 * Detailed steps done in the CINT script:
 * =======================================
 *   1) Step1 - Test Setup (Done in test_setup()):
 *   =============================================
 *     a) Selects two ports and configure them in Loopback mode. Out of these
 *        two ports, one port is used as access_port and the other as
 *        network_port.
 *
 *     b) Install an IFP rule to copy incoming packets to CPU and start
 *        packet watcher.
 *
 *     Note: IFP rule is meant for a testing purpose only (Internal) and it is
 *           nothing to do with an actual functional test.
 *
 *   2) Step2 - Configuration (Done in config_vpws_qos_1()):
 *   =========================================================
 *     a) Configure a basic VPWS from access-side, packet .1Q PRI is mapped to
 *        internal priority. On egress to provider port, internal priority is
 *        remarked to EXP field functional scenario and does the necessary
 *        configurations of vlan, interface and next hop.
 *     b) Customer port -----  XGS ----- Provider port
 *        .1Q priority ---> Internal priority ---> Remark EXP field
 *
 *   3) Step3 - Verification (Done in verify()):
 *   ===========================================
 *     a) Check the configurations by 'vlan show', 'l3 intf show' and
 *        'l3 egress show'.
 *
 *     b) Transmit the customer packet with .1Q priority on access port and
 *        also send tunneled packet on network port.
 *        The contents of the packet are printed on screen.
 *
 *     c) Expected Result:
 *     ===================
 *       We can see that VPWS tunnel initiation with remarked EXP as 3 and flows
 *       out of network_port for incoming .1Q pri and . Example, we are sending
 *       with .1Q PRI as 3 on customer/access port. Also we can see that VPWS
 *       tunnel terminates on network/provider port and goes out of access port.
 *       Also run the 'show c' to check the Tx/Rx packet stats/counters.
 */

/* Reset C interpreter*/
cint_reset();

bcm_port_t access_port;
bcm_port_t network_port;

/*
 * This function is written so that hardcoding of port
 * numbers in Cint scripts is removed. This function gives
 * required number of ports
 */
bcm_error_t
portNumbersGet(int unit, int *port_list, int num_ports)
{
    int i = 0, port = 0, rv = 0;
    bcm_port_config_t configP;
    bcm_pbmp_t ports_pbmp;

    rv = bcm_port_config_get(unit, &configP);
    if (BCM_FAILURE(rv)) {
        printf("\nError in retrieving port configuration: %s.\n",bcm_errmsg(rv));
        return rv;
    }

    ports_pbmp = configP.e;
    for (i= 1; i < BCM_PBMP_PORT_MAX; i++) {
        if (BCM_PBMP_MEMBER(&ports_pbmp, i) && (port < num_ports)) {
            port_list[port] = i;
            port++;
        }
    }

    if ((0 == port) || (port != num_ports)) {
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
bcm_error_t
ingress_port_setup(int unit, bcm_port_t port)
{
    bcm_field_qset_t qset;
    bcm_field_group_t group;
    bcm_field_entry_t entry;

    BCM_IF_ERROR_RETURN(bcm_port_loopback_set(unit, port, BCM_PORT_LOOPBACK_MAC));
    BCM_IF_ERROR_RETURN(bcm_port_discard_set(unit, port, BCM_PORT_DISCARD_NONE));

    BCM_FIELD_QSET_INIT(qset);
    BCM_FIELD_QSET_ADD(qset, bcmFieldQualifyInPort);

    BCM_IF_ERROR_RETURN(bcm_field_group_create(unit, qset, BCM_FIELD_GROUP_PRIO_ANY, &group));

    BCM_IF_ERROR_RETURN(bcm_field_entry_create(unit, group, &entry));

    BCM_IF_ERROR_RETURN(bcm_field_qualify_InPort(unit, entry, port, BCM_FIELD_EXACT_MATCH_MASK));
    BCM_IF_ERROR_RETURN(bcm_field_action_add(unit, entry, bcmFieldActionCopyToCpu, 0, 0));

    BCM_IF_ERROR_RETURN(bcm_field_entry_install(unit, entry));

    return BCM_E_NONE;
}

/*
 * Configures the port in loopback mode and installs
 * an IFP rule. This IFP rule copies the packets ingressing
 * on the specified port to CPU. Port is also configured
 * to discard all packets. This is to avoid continuous
 * loopback of the packet.
 */
bcm_error_t
egress_port_setup(int unit, bcm_port_t port)
{
    bcm_field_qset_t qset;
    bcm_field_group_t group;
    bcm_field_entry_t entry;

    BCM_IF_ERROR_RETURN(bcm_port_loopback_set(unit, port, BCM_PORT_LOOPBACK_MAC));
    BCM_IF_ERROR_RETURN(bcm_port_discard_set(unit, port, BCM_PORT_DISCARD_ALL));

    BCM_FIELD_QSET_INIT(qset);
    BCM_FIELD_QSET_ADD(qset, bcmFieldQualifyInPort);

    BCM_IF_ERROR_RETURN(bcm_field_group_create(unit, qset, BCM_FIELD_GROUP_PRIO_ANY, &group));

    BCM_IF_ERROR_RETURN(bcm_field_entry_create(unit, group, &entry));

    BCM_IF_ERROR_RETURN(bcm_field_qualify_InPort(unit, entry, port, BCM_FIELD_EXACT_MATCH_MASK));
    BCM_IF_ERROR_RETURN(bcm_field_action_add(unit, entry, bcmFieldActionCopyToCpu, 0, 0));

    BCM_IF_ERROR_RETURN(bcm_field_entry_install(unit, entry));

    return BCM_E_NONE;
}

/*
 * Test Setup:
 * This functions gets the port numbers and sets up ingress and
 * egress ports. Check ingress_port_setup() and egress_port_setup().
 */
bcm_error_t
test_setup(int unit)
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

void
verify(int unit)
{
    char str[512];

    bshell(unit, "hm ieee");
    bshell(unit, "vlan show");
    bshell(unit, "l3 intf show");
    bshell(unit, "l3 egress show");
    bshell(unit, "clear c");

    BCM_IF_ERROR_RETURN(bcm_port_discard_set(unit, access_port, BCM_PORT_DISCARD_NONE));
    BCM_IF_ERROR_RETURN(bcm_port_discard_set(unit, network_port, BCM_PORT_DISCARD_ALL));
    printf("\na) Sending a customer packet with .1Q PRI as 3 to access_port:%d\n", access_port);
    snprintf(str, 512, "tx 1 pbm=%d data=0x000000445566000000AABBCC8100600B000102030405060708090A0B0C0D0E0F101112131415161718191A1B1C1D1E1F202122232425262728292A2B2C2D2E2F303132337B9BCED; sleep quiet 1", access_port);
    bshell(unit, str);
    bshell(unit, "show c");
    bshell(unit, "sleep 1");
    bshell(unit, "clear c");

    BCM_IF_ERROR_RETURN(bcm_port_discard_set(unit, access_port, BCM_PORT_DISCARD_ALL));
    BCM_IF_ERROR_RETURN(bcm_port_discard_set(unit, network_port, BCM_PORT_DISCARD_NONE));
    printf("\nb) Sending a MPLS packet with TWO Labels to network_port:%d\n", network_port);
    snprintf(str, 512, "tx 1 pbm=%d data=0x0000000000BB0000000000AA8100001688470022204000333140000000AABBCC0000004455668100000B000102030405060708090A0B0C0D0E0F101112131415161718191A1B1C1D1E1F202122232425262728292A2B2C2D2E2F3031323300539BA; sleep quiet 1", network_port);
    bshell(unit, str);
    bshell(unit, "show c");
    bshell(unit, "sleep 1");
    bshell(unit, "clear c");

    return;
}

bcm_error_t
config_vpws_qos_1(int unit)
{
    bcm_port_t customer_port = access_port;
    bcm_port_t provider_port = network_port;
    bcm_gport_t customer_gport, provider_gport;
    bcm_vlan_t  customer_vid=11, provider_vid=22;
    bcm_pbmp_t  pbmp, ubmp;
    bcm_l3_intf_t       l3_intf;
    bcm_vrf_t   vrf = 0;
    bcm_mac_t   outer_da = {0x00, 0x00, 0x00, 0x00, 0x00, 0xaa};
    bcm_mac_t   outer_sa = {0x00, 0x00, 0x00, 0x00, 0x00, 0xbb};
    bcm_l3_egress_t     l3_egress;
    bcm_if_t    egr_obj;
    bcm_mpls_egress_label_t tun_label;
    uint8               ttl = 16;
    uint32      tunnel_label_init = 0x111;
    uint32      tunnel_label_term = 0x222;
    uint32      vc_label_term     = 0x333;
    uint32      vc_label_init     = 0x444;
    bcm_mpls_vpn_config_t vpn_info;
    bcm_mpls_port_t mpls_c_port;
    bcm_mpls_port_t mpls_p_port;
    bcm_mpls_tunnel_switch_t tunnel_switch;
    bcm_vpn_t   vpn;

    /* Enable egress mode */
    BCM_IF_ERROR_RETURN(bcm_switch_control_set(unit, bcmSwitchL3EgressMode, 1));

    /* Enable Ingress VLAN Translation on customer facing ports */
    BCM_IF_ERROR_RETURN(bcm_vlan_control_port_set(unit, customer_port, bcmVlanTranslateIngressEnable, 1));

    /* Disable VLAN checks on provider network facing port */
    BCM_IF_ERROR_RETURN(bcm_port_control_set(unit, provider_port, bcmPortControlDoNotCheckVlan, 1));

    BCM_IF_ERROR_RETURN(bcm_port_gport_get(unit, customer_port, &customer_gport));
    BCM_IF_ERROR_RETURN(bcm_port_gport_get(unit, provider_port, &provider_gport));
    printf("customer_gport=0x%x, provider_gport=0x%x\n", customer_gport, provider_gport);

    /* VLAN Configuration */
    /* vlan create $customer_vid pbm=$customer_port ubm=none */
    BCM_IF_ERROR_RETURN(bcm_vlan_create(unit, customer_vid));
    BCM_PBMP_CLEAR(ubmp);
    BCM_PBMP_CLEAR(pbmp);
    BCM_PBMP_PORT_ADD(pbmp, customer_port);
    BCM_IF_ERROR_RETURN(bcm_vlan_port_add(unit, customer_vid, pbmp, ubmp));

    /* vlan create $provider_vid pbm=$provider_port ubm=none */
    BCM_IF_ERROR_RETURN(bcm_vlan_create(unit, provider_vid));
    BCM_PBMP_CLEAR(ubmp);
    BCM_PBMP_CLEAR(pbmp);
    BCM_PBMP_PORT_ADD(pbmp, provider_port);
    BCM_IF_ERROR_RETURN(bcm_vlan_port_add(unit, provider_vid, pbmp, ubmp));

    /* Create an L3 interface to be used for the MPLS tunnel */
    bcm_l3_intf_t_init(&l3_intf);
    l3_intf.l3a_flags  = BCM_L3_ADD_TO_ARL;
    sal_memcpy(l3_intf.l3a_mac_addr, outer_sa, 6);
    l3_intf.l3a_vid = provider_vid;
    l3_intf.l3a_vrf = vrf;
    BCM_IF_ERROR_RETURN(bcm_l3_intf_create(unit, &l3_intf));

    /* Create L3 egress object for MPLS tunnel */
    bcm_l3_egress_t_init(&l3_egress);
    l3_egress.intf     = l3_intf.l3a_intf_id;
    sal_memcpy(l3_egress.mac_addr, outer_da, 6);
    l3_egress.vlan     =  provider_vid;
    l3_egress.port     =  provider_gport;
    BCM_IF_ERROR_RETURN(bcm_l3_egress_create(unit, 0, &l3_egress, &egr_obj));

    /* ==========QOS=========== */
    int ing_qos_id;
    bcm_qos_map_t ing_qos_map;
    int i;

    /* Create QoS mapping profile */
    BCM_IF_ERROR_RETURN(bcm_qos_map_create(unit, BCM_QOS_MAP_L2 | BCM_QOS_MAP_INGRESS, &ing_qos_id));
    printf("QOS map = 0x%x\n", ing_qos_id);

    /* Set the packet PRI to internal priority mapping profile */
    int pak_pri[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    int int_pri[8] = {0, 1, 2, 3, 4, 5, 6, 7};

    for (i = 0; i < 8; i++) {
        bcm_qos_map_t_init(&ing_qos_map);
        ing_qos_map.pkt_pri = pak_pri[i];
        ing_qos_map.pkt_cfi = 0;
        ing_qos_map.int_pri = int_pri[i];
        ing_qos_map.color = bcmColorGreen;
        BCM_IF_ERROR_RETURN(bcm_qos_map_add(unit, BCM_QOS_MAP_L2 | BCM_QOS_MAP_INGRESS,
                &ing_qos_map, ing_qos_id));
    }
    BCM_IF_ERROR_RETURN(bcm_qos_port_map_set(unit, customer_gport, ing_qos_id, -1));

    /* Create QoS mapping profile for remarking outgoing EXP field */
    int egr_qos_id;
    bcm_qos_map_t egr_qos_map;
    int in_pri[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    int tunnel_exp[8] = {0, 1, 2, 3, 4, 5, 6, 7};

    BCM_IF_ERROR_RETURN(bcm_qos_map_create(unit, BCM_QOS_MAP_EGRESS | BCM_QOS_MAP_MPLS, &egr_qos_id));
    printf("QOS map = %x\n", egr_qos_id);

    for (i = 0; i < 8; i++) {
        bcm_qos_map_t_init(&egr_qos_map);
        egr_qos_map.int_pri = in_pri[i];    /* internal priority */
        egr_qos_map.color = bcmColorGreen;
        egr_qos_map.exp = tunnel_exp[i];    /* new EXP priority */
        BCM_IF_ERROR_RETURN(bcm_qos_map_add(unit, BCM_QOS_MAP_EGRESS | BCM_QOS_MAP_MPLS,
                &egr_qos_map, egr_qos_id));
    }
    /* ==========END of QOS=========== */

    /* Set MPLS tunnel initiator */
    bcm_mpls_egress_label_t_init(&tun_label);
    tun_label.flags   =   BCM_MPLS_EGRESS_LABEL_TTL_SET;
    tun_label.label   =   tunnel_label_init;
    tun_label.ttl     =   ttl;
    /* ==========QOS========== */
    /* To use exp from mapping */
    tun_label.qos_map_id = egr_qos_id;  /* egr_qos_id & 0x3ff; */
    tun_label.flags |= BCM_MPLS_EGRESS_LABEL_EXP_REMARK;

    /* To use fixed exp
       tun_label.flags |= BCM_MPLS_EGRESS_LABEL_EXP_SET;
       tun_label.exp = 6;
       tun_label.pkt_pri = 3;
       tun_label.pkt_cfi = 1;
       */
    /* ==========END========== */
    BCM_IF_ERROR_RETURN(bcm_mpls_tunnel_initiator_set(unit, l3_intf.l3a_intf_id, 1, &tun_label));

    /* Create VPWS VPN */
    bcm_mpls_vpn_config_t_init(&vpn_info);
    vpn_info.flags = BCM_MPLS_VPN_VPWS;
    BCM_IF_ERROR_RETURN(bcm_mpls_vpn_id_create(unit, &vpn_info));
    vpn = vpn_info.vpn;

    /* Create and bind access port to VPN */
    bcm_mpls_port_t_init (&mpls_c_port);
    mpls_c_port.port        = customer_gport;
    mpls_c_port.criteria = BCM_MPLS_PORT_MATCH_PORT_VLAN;
    mpls_c_port.match_vlan = customer_vid;
    BCM_IF_ERROR_RETURN(bcm_mpls_port_add(unit, vpn, &mpls_c_port));

    BCM_IF_ERROR_RETURN(bcm_qos_port_map_set(unit, mpls_c_port.mpls_port_id, ing_qos_id, -1));

    /* Create and bind provider port to VPN */
    bcm_mpls_port_t_init (&mpls_p_port);
    mpls_p_port.flags = (BCM_MPLS_PORT_NETWORK | BCM_MPLS_PORT_EGRESS_TUNNEL
            | BCM_MPLS_PORT_COUNTED);
    mpls_p_port.port        = provider_gport;
    mpls_p_port.criteria    = BCM_MPLS_PORT_MATCH_LABEL;
    mpls_p_port.match_label = vc_label_term;
    mpls_p_port.egress_tunnel_if   = egr_obj;
    mpls_p_port.egress_label.flags = BCM_MPLS_EGRESS_LABEL_TTL_SET;
    mpls_p_port.egress_label.label = vc_label_init;
    mpls_p_port.egress_label.ttl   = ttl;
    BCM_IF_ERROR_RETURN(bcm_mpls_port_add (unit, vpn, &mpls_p_port));

    /* Add MPLS label lookup */
    bcm_mpls_tunnel_switch_t_init (&tunnel_switch);
    tunnel_switch.flags   = BCM_MPLS_SWITCH_COUNTED;
    tunnel_switch.label   = tunnel_label_term;
    tunnel_switch.port    = BCM_GPORT_INVALID;
    tunnel_switch.action  = BCM_MPLS_SWITCH_ACTION_POP;
    BCM_IF_ERROR_RETURN(bcm_mpls_tunnel_switch_add(unit, &tunnel_switch));

    /* mpls tunnel l2 add MAC=$outer_Da Vlan=$outer_vid */
    BCM_IF_ERROR_RETURN(bcm_l2_tunnel_add(unit, outer_sa, provider_vid));

    return BCM_E_NONE;
}


/*
 * execute:
 *  This function does the following
 *  a) test setup
 *  b) actual configuration (Done in config_vpws_qos_1())
 *  c) demonstrates the functionality(done in verify()).
 */
bcm_error_t
execute(void)
{
    bcm_error_t rv;
    int unit = 0;
    print "config show; attach; cancun stat; version";
    bshell(unit, "config show; a ; cancun stat; version");

    print "~~~ #1) test_setup(): ** start **";
    if (BCM_FAILURE((rv = test_setup(unit)))) {
        printf("test_setup() failed.\n");
        return -1;
    }
    print "~~~ #1) test_setup(): ** end **";

    print "~~~ #2) config_vpws_qos_1(): ** start **";
    if (BCM_FAILURE((rv = config_vpws_qos_1(unit)))) {
        printf("config_vpws_qos_1() failed.\n");
        return -1;
    }
    print "~~~ #2) config_vpws_qos_1(): ** end **";

    print "~~~ #3) verify(): ** start **";
    verify(unit);
    print "~~~ #3) verify(): ** end **";

    return BCM_E_NONE;
}

const char *auto_execute = (ARGC == 1) ? ARGV[0] : "YES";
if (!sal_strcmp(auto_execute, "YES")) {
    print "execute(): Start";
    print execute();
    print "execute(): End";
}
