/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */
/*
 * Feature  : L2 MPLS LSR Label Swap functionality with EXP remarking
 *
 * Usage    : BCM.0> cint td4_hsdk_l2mpls_lsr_swap_qos.c
 *
 * config   : bcm56880_a0-generic-32x400.mpls-config.yml
 *
 * Log file : td4_hsdk_l2mpls_lsr_swap_qos_log.txt
 *
 * Test Topology :
 *
 *                  +-------------------+
 *                  |                   |
 *                  |                   |
 *     ingress_port |                   |  egress_port
 *    ------------->+        TD4        +--------------->
 *                  |                   |
 *                  |                   |
 *                  |                   |
 *                  +-------------------+
 * Summary:
 * ========
 *   This Cint example to show the configuration of L2 MPLS lsr label swap
 *   functionality along with label exp remarking
 *
 * Detailed steps done in the CINT script:
 * =======================================
 *   1) Step1 - Test Setup (Done in test_setup()):
 *   =============================================
 *     a) Selects two ports and configure them in Loopback mode. Out of these
 *        two ports, one port is used as ingress_port and the other as
 *        egress_port.
 *
 *     b) Install an IFP rule to copy incoming packets to CPU and start
 *        packet watcher.
 *
 *     Note: IFP rule is meant for a testing purpose only (Internal) and it is
 *           nothing to do with an actual functional test.
 *
 *   2) Step2 - Configuration (Done in config_l2_mpls_lsr()):
 *   =========================================================
 *     a) Configure a basic L2 MPLS functional scenario and
 *        does the necessary configurations of vlan, interface, next hop and
 *        label action.
 *
 *   3) Step3 - Verification (Done in verify()):
 *   ===========================================
 *     a) Check the configurations by 'vlan show', 'l3 intf show',
 *        'l3 egress show' and 'l3 defip show'
 *
 *     b) Transmit the MPLS packet with l2 unicast payload. The contents of the
 *        packet are printed on screen.
 *
 *     c) Expected Result:
 *     ===================
 *      We can see that L2 mpls packet out on egress port with configured
 *      LSR functionality.
 */

/* Reset C interpreter*/
cint_reset();

bcm_port_t ingress_port;
bcm_port_t egress_port;
bcm_gport_t ingress_gport;
bcm_gport_t egress_gport;

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
        printf("\nError in retrieving port configuration:%s\n",bcm_errmsg(rv));
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
    bcm_field_entry_t        entry;
    bcm_field_group_config_t group_config;

    BCM_IF_ERROR_RETURN(bcm_port_loopback_set(unit, port,
                                  BCM_PORT_LOOPBACK_MAC));

    bcm_field_group_config_t_init(&group_config);
    BCM_FIELD_QSET_INIT(group_config.qset);
    BCM_FIELD_QSET_ADD(group_config.qset, bcmFieldQualifyStageIngress);
    BCM_FIELD_QSET_ADD(group_config.qset, bcmFieldQualifyInPort);
    BCM_FIELD_ASET_ADD(group_config.aset, bcmFieldActionCopyToCpu);
    group_config.priority = 1;
    BCM_IF_ERROR_RETURN(bcm_field_group_config_create(unit, &group_config));

    BCM_IF_ERROR_RETURN(bcm_field_entry_create(unit, group_config.group,
                                               &entry));
    BCM_IF_ERROR_RETURN(bcm_field_qualify_InPort(unit, entry, port,
                                               BCM_FIELD_EXACT_MATCH_MASK));
    BCM_IF_ERROR_RETURN(bcm_field_action_add(unit, entry,
                                             bcmFieldActionCopyToCpu, 0, 0));

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
bcm_error_t
egress_port_setup(int unit, bcm_port_t port)
{
    bcm_field_entry_t        entry;
    bcm_field_group_config_t group_config;

    BCM_IF_ERROR_RETURN(bcm_port_loopback_set(unit, port,
                                 BCM_PORT_LOOPBACK_MAC));

    bcm_field_group_config_t_init(&group_config);
    BCM_FIELD_QSET_INIT(group_config.qset);
    BCM_FIELD_QSET_ADD(group_config.qset, bcmFieldQualifyStageIngress);
    BCM_FIELD_QSET_ADD(group_config.qset, bcmFieldQualifyInPort);
    BCM_FIELD_ASET_ADD(group_config.aset, bcmFieldActionCopyToCpu);
    BCM_FIELD_ASET_ADD(group_config.aset, bcmFieldActionDrop);
    group_config.priority = 2;
    BCM_IF_ERROR_RETURN(bcm_field_group_config_create(unit, &group_config));

    BCM_IF_ERROR_RETURN(bcm_field_entry_create(unit, group_config.group,
                                                               &entry));

    BCM_IF_ERROR_RETURN(bcm_field_qualify_InPort(unit, entry, port,
                                                 BCM_FIELD_EXACT_MATCH_MASK));
    BCM_IF_ERROR_RETURN(bcm_field_action_add(unit, entry,
                                             bcmFieldActionCopyToCpu, 0, 0));
    BCM_IF_ERROR_RETURN(bcm_field_action_add(unit, entry,
                                             bcmFieldActionDrop, 0, 0));

    BCM_IF_ERROR_RETURN(bcm_field_entry_install(unit, entry));

    printf("egress_port_setup configured for egress_port : %d\n", port);

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

    ingress_port = port_list[0];
    egress_port = port_list[1];

    if (BCM_E_NONE != ingress_port_setup(unit, ingress_port)) {
        printf("ingress_port_setup() failed for port %d\n", ingress_port);
        return -1;
    }

    if (BCM_E_NONE != egress_port_setup(unit, egress_port)) {
        printf("egress_port_setup() failed for port %d\n", egress_port);
        return -1;
    }

    bshell(unit, "pw start report +raw +decode +pmd");

    return BCM_E_NONE;
}

/* Qos configuration */
bcm_error_t
lsr_qos_config(int unit, int *ing_qos, int *egr_qos)
{
    int ing_qos_id;
    bcm_qos_map_t ing_qos_map;
    int i, j;
    bcm_error_t rv = BCM_E_NONE;

    /* Create QoS mapping profile */
    rv = bcm_qos_map_create(unit, BCM_QOS_MAP_L2 | BCM_QOS_MAP_INGRESS,
                            &ing_qos_id);
    if (BCM_FAILURE(rv)) {
        printf("\nError in bcm_qos_map_create: %s.\n", bcm_errmsg(rv));
        return rv;
    }
    *ing_qos = ing_qos_id;
    printf("Ingress QOS map = 0x%x\n", ing_qos_id);

    /* Set the packet PRI to internal priority mapping profile */
    int pck_pri[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    int int_pri[8] = {0, 1, 2, 3, 4, 5, 6, 7};

    for (i = 0; i < 8; i++) {
        bcm_qos_map_t_init(&ing_qos_map);
        ing_qos_map.pkt_pri = pck_pri[i];     /* packet priority */
        ing_qos_map.int_pri = int_pri[i];     /* internal priority */
        rv = bcm_qos_map_add(unit, BCM_QOS_MAP_L2 | BCM_QOS_MAP_INGRESS,
                             &ing_qos_map, ing_qos_id);
        if (BCM_FAILURE(rv)) {
            printf("\nError in ingress bcm_qos_map_add: %s.\n", bcm_errmsg(rv));
            return rv;
        }
    }
    rv = bcm_qos_port_map_set(unit, ingress_gport, ing_qos_id, -1);
    if (BCM_FAILURE(rv)) {
        printf("\nError in bcm_qos_port_map_set: %s.\n", bcm_errmsg(rv));
        return rv;
    }

    /* Create QoS mapping profile for remarking outgoing EXP field */
    int egr_qos_id;
    bcm_qos_map_t egr_qos_map;
    int in_pri[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    int lsr_exp[8] = {0, 1, 2, 3, 4, 5, 6, 7};

    rv = bcm_qos_map_create(unit, BCM_QOS_MAP_EGRESS | BCM_QOS_MAP_MPLS,
                            &egr_qos_id);
    if (BCM_FAILURE(rv)) {
        printf("\nError in bcm_qos_map_create: %s.\n", bcm_errmsg(rv));
        return rv;
    }
    *egr_qos = egr_qos_id;
    printf("Egress QOS map = 0x%x\n", egr_qos_id);

    for (i = 0; i < 8; i++) {
        bcm_qos_map_t_init(&egr_qos_map);
        egr_qos_map.int_pri = in_pri[i];    /* internal priority */
        egr_qos_map.exp = lsr_exp[i];       /* exp remarking */
        rv = bcm_qos_map_add(unit, BCM_QOS_MAP_EGRESS | BCM_QOS_MAP_MPLS,
                             &egr_qos_map, egr_qos_id);
        if (BCM_FAILURE(rv)) {
            printf("\nError in egress bcm_qos_map_add: %s.\n", bcm_errmsg(rv));
            return rv;
        }
    }

    return rv;
}


/*
 * Verification:
 *   In Packet:
 *   ----------
 *   0000 0000 1303 0000 0000 00aa 8100 800c
 *   8847 0030 1140 0000 0000 0011 0000 0000
 *   0022 8100 000b 0800 4500 0036 0000 0000
 *   40ff 65ac 0a00 0014 0a00 000a 0001 0203
 *   0405 0607 0809 0a0b 0c0d 0e0f 1011 1213
 *   1415 1617 1819 1a1b 1c1d 1e1f 2021 2394
 *   12f0
 *
 *   Out Packet:
 *   -----------
 *   0000 0000 2222 0000 0000 1111 8100 8016
 *   8847 0012 393f 0000 0000 0011 0000 0000
 *   0022 8100 000b 0800 4500 0036 0000 0000
 *   40ff 65ac 0a00 0014 0a00 000a 0001 0203
 *   0405 0607 0809 0a0b 0c0d 0e0f 1011 1213
 *   1415 1617 1819 1a1b 1c1d 1e1f 2021 2394
 *   12f0
 */

void
verify(int unit)
{
    char str[512];

    bshell(unit, "vlan show");
    bshell(unit, "l3 intf show");
    bshell(unit, "l3 egress show");
    bshell(unit, "clear c");
    /* mpls packet with l2 header payload */
    printf("\nSending MPLS packet to ingress_port:%d\n", ingress_port);
    snprintf(str, 512, "tx 1 pbm=%d data=0x0000000013030000000000AA8100800C8847003011400000000000110000000000228100000b0800450000360000000040ff65ac0a0000140a00000a000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f2021239412f0; sleep quiet 3", ingress_port);
    bshell(unit, str);
    bshell(unit, "show c");
    bshell(unit, "l2 show");

    return;
}

bcm_error_t
config_l2_mpls_lsr(int unit)
{
    bcm_error_t rv;
    bcm_vlan_t in_vlan = 12;
    bcm_vlan_t out_vlan = 22;
    bcm_pbmp_t pbmp, ubmp;
    bcm_mac_t router_mac = {0x00, 0x00, 0x00, 0x00, 0x13, 0x03};
    bcm_mac_t local_mac = {0x00, 0x00, 0x00, 0x00, 0x11, 0x11};
    bcm_mac_t nh_mac    = {0x00, 0x00, 0x00, 0x00, 0x22, 0x22};
    uint32 tunnel_label_term = 0x301;
    bcm_mpls_tunnel_switch_t tunnel_switch;
    bcm_l3_intf_t l3_intf;
    bcm_l3_egress_t l3_egress;
    bcm_if_t egr_obj;
    int ing_qos_id;
    int egr_qos_id;

    BCM_IF_ERROR_RETURN(bcm_port_gport_get(unit, ingress_port, &ingress_gport));
    BCM_IF_ERROR_RETURN(bcm_port_gport_get(unit, egress_port, &egress_gport));
    printf("ingress_gport=0x%x, egress_gport=0x%x\n",
                         ingress_gport, egress_gport);

    /* Create vlan and add port to vlan */
    BCM_IF_ERROR_RETURN(bcm_vlan_create(unit, in_vlan));
    BCM_PBMP_CLEAR(ubmp);
    BCM_PBMP_CLEAR(pbmp);
    BCM_PBMP_PORT_ADD(pbmp, ingress_port);
    BCM_IF_ERROR_RETURN(bcm_vlan_port_add(unit, in_vlan, pbmp, ubmp));

    /* Create vlan and add port to vlan */
    BCM_IF_ERROR_RETURN(bcm_vlan_create(unit, out_vlan));
    BCM_PBMP_CLEAR(ubmp);
    BCM_PBMP_CLEAR(pbmp);
    BCM_PBMP_PORT_ADD(pbmp, egress_port);
    BCM_IF_ERROR_RETURN(bcm_vlan_port_add(unit, out_vlan, pbmp, ubmp));

    /* QOS remarking for lsr swap label */
    rv = lsr_qos_config(unit, &ing_qos_id, &egr_qos_id);
    if (BCM_FAILURE(rv)) {
        printf("\nError in lsr_qos_config: %s.\n", bcm_errmsg(rv));
        return rv;
    }

    /* Create L3 interface  */
    bcm_l3_intf_t_init(&l3_intf);
    sal_memcpy(l3_intf.l3a_mac_addr, local_mac, 6);
    l3_intf.l3a_vid = out_vlan;
    BCM_IF_ERROR_RETURN(bcm_l3_intf_create(unit, &l3_intf));
    printf("l3 intf = %d\n", l3_intf.l3a_intf_id);

    /* Create Egress object for label swap */
    bcm_l3_egress_t_init(&l3_egress);
    sal_memcpy(l3_egress.mac_addr, nh_mac, 6);
    l3_egress.intf     = l3_intf.l3a_intf_id;
    l3_egress.vlan     = out_vlan;
    l3_egress.port     = egress_gport;
    l3_egress.mpls_label = 0x123;
    l3_egress.mpls_qos_map_id = egr_qos_id;
    BCM_IF_ERROR_RETURN(bcm_l3_egress_create(unit, 0, &l3_egress, &egr_obj));

    /* Add MPLS table entry for incoming label matching */
    bcm_mpls_tunnel_switch_t_init (&tunnel_switch);
    tunnel_switch.label   = tunnel_label_term;
    tunnel_switch.port    = BCM_GPORT_INVALID;
    tunnel_switch.action  = BCM_MPLS_SWITCH_ACTION_SWAP;
    tunnel_switch.egress_if = egr_obj;
    rv = bcm_mpls_tunnel_switch_add(unit, &tunnel_switch);
    if (BCM_FAILURE(rv)) {
        printf("\nError in bcm_mpls_tunnel_switch_add: %s.\n",bcm_errmsg(rv));
        return rv;
    }

    /* Enable MPLS Lookup for incoming packet */
    BCM_IF_ERROR_RETURN(bcm_l2_tunnel_add(unit, router_mac, in_vlan));

    return BCM_E_NONE;
}

/*
 * execute:
 *  This function does the following
 *  a) test setup
 *  b) actual configuration (Done in config_l2_mpls_lsr())
 *  c) demonstrates the functionality(done in verify()).
 */
bcm_error_t
execute(void)
{
    bcm_error_t rv;
    int unit = 0;
    print "config show; attach; version";
    bshell(unit, "config show; a ; version");

    print "~~~ #1) test_setup(): ** start **";
    if (BCM_FAILURE((rv = test_setup(unit)))) {
        printf("test_setup() failed.\n");
        return -1;
    }
    print "~~~ #1) test_setup(): ** end **";

    print "~~~ #2) config_l2_mpls_lsr(): ** start **";
    if (BCM_FAILURE((rv = config_l2_mpls_lsr(unit)))) {
        printf("config_l2_mpls_lsr() failed.\n");
        return -1;
    }
    print "~~~ #2) config_l2_mpls_lsr(): ** end **";

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
