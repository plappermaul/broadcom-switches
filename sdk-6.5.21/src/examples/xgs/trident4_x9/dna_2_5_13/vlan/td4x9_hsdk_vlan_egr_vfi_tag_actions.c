/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

/*
 * Feature  : Egress OTAG and ITAG actions based on egress VFI
 *
 * Usage    : BCM.0> cint td4x9_hsdk_vlan_egr_vfi_tag_actions.c
 *
 * config   : bcm56780_a0-generic-vlan.config.yml
 *
 * Log file : td4x9_hsdk_vlan_egr_vfi_tag_actions_log.txt
 *
 * Test Topology :
 *                      +-------------------+
 *                      |                   |
 *                      |        TD4.X9     |
 *       ingress_port   |                   |  egress_port
 *           ---------->+ vfi 20            +---------->
 *     (inner_vlan=10)  |                   |  otag 20 / itag 10
 *     (outer_vlan=20)  |                   |  dst_mac 00:00:00:00:00:11
 *                      |                   |
 *                      |                   |
 *                      +-------------------+
 *
 *
 * Summary:
 * ========
 * cint script to demonstrate egress OTAG/ITAG actions based on
 * egress VFI for untag/ single tag/ double tagged packets
 *
 * Detailed steps done in the CINT script:
 * =======================================
 *   1) Step1 - Test Setup (Done in test_setup()):
 *   =============================================
 *     a) Selects two ports and configure them in Loopback mode. Out of these
 *        two ports, one port is used as ingress_port and the other as
 *        egress_port.
 *
 *     b) Install an IFP rule to copy ingress & egress packets to CPU and start
 *        packet watcher.
 *
 *     Note: IFP rule is meant for a verification purpose and is not part of the feature configuration.
 *
 *   2) Step2 - Configuration (Done in setup_vlan_switching()):
 *   =========================================================
 *     a) Ports are configured in the specific vlan/vfi
 *     b) Assign VFI from port
 *     c) L2 static entry is setup for unicast forwarding
 *     d) L2 learn mode is setup on ingress and egress ports
 *     e) configure egress OTAG add / ITAG add actions
 *
 *   3) Step3 - Verification (Done in verify()):
 *   ===========================================
 *     a) Check the outputs of 'vlan show' 'l2 show' 'show c'
 *     b) Transmit the packet on the ingress port side
 *
 *     c) Expected Result:
 *     ===================
 *     The packet capture shows the egress packet transmitted on outgoing port as per the forwarding path setup
 *
 *   Ingress Packet: untag packet
 *   000000000011000000000002
 *   080600010800060400010000
 *   000000020A0A0A0200000000
 *   000014141402000102030405
 *   060708090a0b0c0dbc94977f

 *   Egress Packet:
 *   [bcmPWN.0]Packet[2]: data[0000]: {000000000011} {000000000002} 8100 2014
 *   [bcmPWN.0]Packet[2]: data[0010]: 9100 200a 0806 0001 0800 0604 0001 0000
 *   [bcmPWN.0]Packet[2]: data[0020]: 0000 0002 0a0a 0a02 0000 0000 0000 1414
 *   [bcmPWN.0]Packet[2]: data[0030]: 1402 0001 0203 0405 0607 0809 0a0b 0c0d
 *   [bcmPWN.0]Packet[2]: data[0040]: bc94 977f
 *
 *   Ingress Packet: single tag packet
 *   [bcmPWN.0]Packet[3]: data[0000]: {000000000011} {000000000002} 8100 0014
 *   [bcmPWN.0]Packet[3]: data[0010]: 0806 0001 0800 0604 0001 0000 0000 0002
 *   [bcmPWN.0]Packet[3]: data[0020]: 0a0a 0a02 0000 0000 0000 1414 1402 0001
 *   [bcmPWN.0]Packet[3]: data[0030]: 0203 0405 0607 0809 0a0b 0c0d bc94 977f
 *
 *   Egress Packet:
 *   [bcmPWN.0]Packet[4]: data[0000]: {000000000011} {000000000002} 8100 2014
 *   [bcmPWN.0]Packet[4]: data[0010]: 9100 200a 0806 0001 0800 0604 0001 0000
 *   [bcmPWN.0]Packet[4]: data[0020]: 0000 0002 0a0a 0a02 0000 0000 0000 1414
 *   [bcmPWN.0]Packet[4]: data[0030]: 1402 0001 0203 0405 0607 0809 0a0b 0c0d
 *   [bcmPWN.0]Packet[4]: data[0040]: bc94 977f
 *
 *   Ingress Packet: double tag packet
 *   [bcmPWN.0]Packet[5]: data[0000]: {000000000011} {000000000002} 8100 0014
 *   [bcmPWN.0]Packet[5]: data[0010]: 9100 000a 0806 0001 0800 0604 0001 0000
 *   [bcmPWN.0]Packet[5]: data[0020]: 0000 0002 0a0a 0a02 0000 0000 0000 1414
 *   [bcmPWN.0]Packet[5]: data[0030]: 1402 0001 0203 0405 0607 0809 0a0b 0c0d
 *   [bcmPWN.0]Packet[5]: data[0040]: bc94 977f
 *
 *   Egress Packet:
 *   [bcmPWN.0]Packet[6]: data[0000]: {000000000011} {000000000002} 8100 2014
 *   [bcmPWN.0]Packet[6]: data[0010]: 9100 200a 0806 0001 0800 0604 0001 0000
 *   [bcmPWN.0]Packet[6]: data[0020]: 0000 0002 0a0a 0a02 0000 0000 0000 1414
 *   [bcmPWN.0]Packet[6]: data[0030]: 1402 0001 0203 0405 0607 0809 0a0b 0c0d
 *   [bcmPWN.0]Packet[6]: data[0040]: bc94 977f
*/

/* Reset C interpreter*/
cint_reset();

bcm_port_t egress_port;
bcm_port_t ingress_port;
bcm_gport_t egress_phy_gport;
bcm_gport_t ingress_phy_gport;

/*
 * This function is written so that hardcoding of port
 * numbers in Cint scripts is removed. This function gives
 * required number of ports
 */
bcm_error_t
portNumbersGet(int unit, int *port_list, int num_ports)
{
    int i = 0, port = 0;
    bcm_error_t rv = 0;
    bcm_port_config_t configP;
    bcm_pbmp_t ports_pbmp;

    rv = bcm_port_config_get(unit, &configP);
    if (BCM_FAILURE(rv)) {
        printf("\nError in bcm_port_config_get: %s.\n",bcm_errmsg(rv));
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
ingress_port_multi_setup(int unit, bcm_pbmp_t pbm)
{
    bcm_field_entry_t entry;
    bcm_port_t     port;
    bcm_field_group_config_t group_config;

    bcm_field_group_config_t_init(&group_config);
    BCM_FIELD_QSET_INIT(group_config.qset);
    BCM_FIELD_QSET_ADD(group_config.qset, bcmFieldQualifyInPort);
    BCM_FIELD_QSET_ADD(group_config.qset, bcmFieldQualifyStageIngress);
    BCM_FIELD_ASET_ADD(group_config.aset, bcmFieldActionCopyToCpu);
    group_config.priority = 1;

    BCM_IF_ERROR_RETURN(bcm_field_group_config_create(unit, &group_config));

    BCM_PBMP_ITER(pbm, port) {
        printf("ingress_port_multi_setup port: %d setup\n", port);
        BCM_IF_ERROR_RETURN(bcm_port_loopback_set(unit, port, BCM_PORT_LOOPBACK_MAC));

        BCM_IF_ERROR_RETURN(bcm_field_entry_create(unit, group_config.group, &entry));

        BCM_IF_ERROR_RETURN(bcm_field_qualify_InPort(unit, entry, port, BCM_FIELD_EXACT_MATCH_MASK));
        BCM_IF_ERROR_RETURN(bcm_field_action_add(unit, entry, bcmFieldActionCopyToCpu, 0, 0));

        BCM_IF_ERROR_RETURN(bcm_field_entry_install(unit, entry));
    }

    return BCM_E_NONE;
}

/*
 * Configures the port in loopback mode and installs
 * an IFP rule. This IFP rule copies the packets ingressing
 * on the specified port to CPU. Port is also configured
 * to discard all packets. This is to avoid continuous
 * loopback of the packet from the egress port.
 */
bcm_error_t
egress_port_multi_setup(int unit, bcm_pbmp_t pbm)
{
    bcm_field_group_t group;
    bcm_field_entry_t entry;
    bcm_port_t     port;

    bcm_field_group_config_t group_config;

    bcm_field_group_config_t_init(&group_config);
    BCM_FIELD_QSET_INIT(group_config.qset);
    BCM_FIELD_QSET_ADD(group_config.qset, bcmFieldQualifyInPort);
    BCM_FIELD_QSET_ADD(group_config.qset, bcmFieldQualifyStageIngress);
    BCM_FIELD_ASET_ADD(group_config.aset, bcmFieldActionCopyToCpu);
    BCM_FIELD_ASET_ADD(group_config.aset, bcmFieldActionDrop);
    group_config.priority = 2;

    BCM_IF_ERROR_RETURN(bcm_field_group_config_create(unit, &group_config));

    BCM_PBMP_ITER(pbm, port) {
        printf("egress_port_multi_setup port: %d setup\n", port);

        BCM_IF_ERROR_RETURN(bcm_port_loopback_set(unit, port, BCM_PORT_LOOPBACK_MAC));

        BCM_IF_ERROR_RETURN(bcm_field_entry_create(unit, group_config.group, &entry));

        BCM_IF_ERROR_RETURN(bcm_field_qualify_InPort(unit, entry, port, BCM_FIELD_EXACT_MATCH_MASK));
        BCM_IF_ERROR_RETURN(bcm_field_action_add(unit, entry, bcmFieldActionCopyToCpu, 0, 0));
        BCM_IF_ERROR_RETURN(bcm_field_action_add(unit, entry, bcmFieldActionDrop, 0, 0));

        BCM_IF_ERROR_RETURN(bcm_field_entry_install(unit, entry));
    }

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
    int num_ports = 2;
    int port_list[num_ports];

    if (BCM_E_NONE != portNumbersGet(unit, port_list, num_ports)) {
        printf("portNumbersGet() failed\n");
        return -1;
    }

    ingress_port = port_list[0];
    egress_port = port_list[1];

    if (BCM_E_NONE !=bcm_port_gport_get(unit, ingress_port, &(ingress_phy_gport))){
        printf("bcm_port_gport_get() failed to get gport for port %d\n", ingress_port);
        return -1;
    }

    if (BCM_E_NONE != bcm_port_gport_get(unit, egress_port, &(egress_phy_gport))){
        printf("bcm_port_gport_get() failed to get gport for port %d\n", egress_port);
        return -1;
    }

    bcm_pbmp_t  pbmp;
    BCM_PBMP_CLEAR(pbmp);
    BCM_PBMP_PORT_ADD(pbmp, ingress_port);
    if (BCM_E_NONE != ingress_port_multi_setup(unit, pbmp)) {
        printf("ingress_port_multi_setup() failed for port %d\n", ingress_port);
        return -1;
    }

    BCM_PBMP_CLEAR(pbmp);
    BCM_PBMP_PORT_ADD(pbmp, egress_port);
    if (BCM_E_NONE != egress_port_multi_setup(unit, pbmp)) {
        printf("egress_port_multi_setup() failed for port %d\n", egress_port);
        return -1;
    }

    bshell(unit, "pw start report +all");

    return BCM_E_NONE;
}

/*Verify function call*/
void
verify(int unit)
{
    char str[512];

    print "vlan show";
    bshell(unit, "vlan show");
    print "l2 show";
    bshell(unit, "l2 show");
    bshell(unit, "clear c");
    printf("\nSending untag packet to ingress port:%d\n",ingress_port);
    snprintf(str, 512, "tx 1 pbm=%d data=0x000000000011000000000002080600010800060400010000000000020A0A0A0200000000000014141402000102030405060708090a0b0c0dbc94977f; sleep quiet 2", ingress_port);
    bshell(unit, str);
    print "show c";
    bshell(unit, "show c");

    bshell(unit, "clear c");
    printf("\nSending single tag packet to ingress port:%d\n",ingress_port);
    snprintf(str, 512, "tx 1 pbm=%d data=0x00000000001100000000000291000004080600010800060400010000000000020A0A0A0200000000000014141402000102030405060708090a0b0c0dbc94977f; sleep quiet 2", ingress_port);
    bshell(unit, str);
    print "show c";
    bshell(unit, "show c");

    bshell(unit, "clear c");
    printf("\nSending double tag packet to ingress port:%d\n",ingress_port);
    snprintf(str, 512, "tx 1 pbm=%d data=0x000000000011000000000002810000149100000A080600010800060400010000000000020A0A0A0200000000000014141402000102030405060708090a0b0c0dbc94977f; sleep quiet 2", ingress_port);
    bshell(unit, str);
    print "show c";
    bshell(unit, "show c");

    printf("\nverify l2 learning:\n");
    print "l2 show";
    bshell(unit, "l2 show");

    return;
}

/* Configure port to vlan as tagged or untagged*/
bcm_error_t
vlan_add_port(int unit, int vid, bcm_port_t t_port, bcm_port_t ut_port)
{
    bcm_pbmp_t pbmp, upbmp;
    bcm_error_t rv = BCM_E_NONE;

    BCM_PBMP_CLEAR(pbmp);
    BCM_PBMP_CLEAR(upbmp);
    BCM_PBMP_PORT_ADD(pbmp, t_port);
    BCM_PBMP_PORT_ADD(upbmp, ut_port);
    rv = bcm_vlan_port_add(unit, vid, pbmp, upbmp);
    if (BCM_FAILURE(rv)) {
        printf("Error in bcm_vlan_port_add: %s\n", bcm_errmsg(rv));
        return rv;
    }
    return BCM_E_NONE;
}

/* Static L2 entry for forwarding */
bcm_error_t
add_l2_static_entry(int unit,uint32 flag,bcm_mac_t mac,bcm_vpn_t vpn_id,bcm_port_t port)
{
    bcm_l2_addr_t l2_addr;
    bcm_l2_addr_t_init(&l2_addr, mac, vpn_id);
    l2_addr.flags = BCM_L2_STATIC | flag;
    l2_addr.port = port;
    return bcm_l2_addr_add(unit, &l2_addr);
}

/*Setup learning on port*/
bcm_error_t
port_learn_mode_set(int unit, bcm_port_t port, uint32 flag)
{
    bcm_error_t rv = BCM_E_NONE;

    rv = bcm_port_learn_set(unit, port, flag);
    if (BCM_FAILURE(rv)) {
        printf("Error in bcm_port_learn_set: %s\n", bcm_errmsg(rv));
        return rv;
    }
    return BCM_E_NONE;
}

/*configure the per port tpid as required*/
bcm_error_t
port_tpid_setup(int unit,bcm_gport_t port,uint16 outer_tpid,uint16 inner_tpid)
{
    bcm_error_t rv = BCM_E_NONE;

    rv = bcm_port_tpid_set(unit,port,outer_tpid);
    if (BCM_FAILURE(rv)) {
        printf("Error in bcm_port_tpid_set: %s\n", bcm_errmsg(rv));
        return rv;
    }

    rv = bcm_port_inner_tpid_set(unit,port,inner_tpid);
    if (BCM_FAILURE(rv)) {
        printf("Error in bcm_port_inner_tpid_set: %s\n", bcm_errmsg(rv));
        return rv;
    }
    return BCM_E_NONE;
}

/*configure default egress vlan actions per forwarding domain*/
bcm_error_t
egr_vlan_actions_setup(int unit,bcm_vlan_t vlanId,bcm_vlan_t pkt_inner_vlan)
{
    bcm_error_t rv = BCM_E_NONE;

    /*configure mapped_vfi*/
    bcm_vlan_control_vlan_t vlan_control;
    bcm_vlan_control_vlan_t_init(&vlan_control);
    vlan_control.forwarding_vlan = vlanId;

    vlan_control.egress_action.outer_tag = bcmVlanActionAdd;
    vlan_control.egress_action.dt_outer_pkt_prio = bcmVlanActionAdd;
    vlan_control.egress_action.new_outer_vlan = vlanId;
    vlan_control.egress_action.new_outer_cfi = 0;
    vlan_control.egress_action.priority = 1;

    vlan_control.egress_action.inner_tag = bcmVlanActionAdd;
    vlan_control.egress_action.dt_inner_pkt_prio = bcmVlanActionAdd;
    vlan_control.egress_action.new_inner_vlan = pkt_inner_vlan;
    vlan_control.egress_action.new_inner_cfi = 0;
    vlan_control.egress_action.new_inner_pkt_prio = 1;

    rv = bcm_vlan_control_vlan_selective_set(unit,vlanId,
                    BCM_VLAN_CONTROL_VLAN_FORWARDING_VLAN_MASK |
                    BCM_VLAN_CONTROL_VLAN_EGRESS_ACTION_MASK,vlan_control);
    if (BCM_FAILURE(rv)) {
        printf("\nError in bcm_vlan_control_vlan_set: %s\n", bcm_errmsg(rv));
        return rv;
    }

    return BCM_E_NONE;
}

/* L2 switch configuration function */
bcm_error_t
setup_vlan_switching(int unit)
{
    bcm_vlan_t vlanId=20, default_vlan=20, pkt_inner_vlan=10;
    bcm_error_t rv = BCM_E_NONE;
    bcm_mac_t dst_mac = "00:00:00:00:00:11";

    /*Create the vlan with specific id*/
    printf("create vlan %d\n", vlanId);
    rv = bcm_vlan_create(unit, vlanId);
    if (BCM_FAILURE(rv)) {
        printf("Error in bcm_vlan_create: %s\n", bcm_errmsg(rv));
        return rv;
    }

    /*Add ports to vlan*/
    rv = vlan_add_port(unit, vlanId, ingress_port, 0);
    if (BCM_FAILURE(rv)) {
        printf("\nError in vlan_add_port: %s\n", bcm_errmsg(rv));
        return rv;
    }

    rv = vlan_add_port(unit, vlanId, egress_port, 0);
    if (BCM_FAILURE(rv)) {
        printf("\nError in vlan_add_port: %s\n", bcm_errmsg(rv));
        return rv;
    }

    /*setup per port tpid*/
    rv = port_tpid_setup(unit,ingress_port,0x8100,0x9100);
    if (BCM_FAILURE(rv)) {
        printf("\nError in port_tpid_setup: %s\n", bcm_errmsg(rv));
        return rv;
    }

    rv = port_tpid_setup(unit,egress_port,0x8100,0x9100);
    if (BCM_FAILURE(rv)) {
        printf("\nError in port_tpid_setup: %s\n", bcm_errmsg(rv));
        return rv;
    }

    /*Setup default vlan port action*/
    bcm_vlan_action_set_t default_action;
    bcm_vlan_action_set_t_init(default_action);
    default_action.forwarding_domain = default_vlan;
    /*do not preserve incoming tags*/
    default_action.outer_tag = bcmVlanActionDelete;
    default_action.inner_tag = bcmVlanActionDelete;
    rv = bcm_vlan_port_default_action_set(unit,ingress_port,&default_action);
    if (BCM_FAILURE(rv)) {
        printf("\nError in bcm_vlan_port_default_action_set: %s\n", bcm_errmsg(rv));
        return rv;
    }

    /*Setup identity mapping of VLAN_ID to VFI at ingress port*/
    /*it is enabled by default in the switch*/
    rv = bcm_port_control_set(unit, ingress_port, bcmPortControlTrustIncomingVlan,1);
    if (BCM_FAILURE(rv)) {
        printf("\nError in bcm_port_control_set (bcmPortControlTrustIncomingVlan): %s\n", bcm_errmsg(rv));
        return rv;
    }

    /*enable egress vlan priority from egr_vfi*/
    rv = bcm_port_control_set(unit, egress_port, bcmPortControlEgressVlanPriSrc, bcmPortEgressVlanPriSrcVpn);
    if (BCM_FAILURE(rv)) {
        printf("\nError in bcm_port_control_seti (bcmPortControlEgressVlanPriSrc): %s\n", bcm_errmsg(rv));
        return rv;
    }

    /*setup default egress vlan actions*/
    printf("default egress vlan actions setup\n");
    rv = egr_vlan_actions_setup(unit, default_vlan, pkt_inner_vlan);
    if (BCM_FAILURE(rv)) {
        printf("\nError in egr_vlan_actions_setup: %s\n", bcm_errmsg(rv));
        return rv;
    }

    /*Learn mode port config*/
    rv = port_learn_mode_set(unit, ingress_port, BCM_PORT_LEARN_ARL | BCM_PORT_LEARN_FWD);
    if (BCM_FAILURE(rv)) {
        printf("\nError in port_learn_mode_set: %s\n", bcm_errmsg(rv));
        return rv;
    }

    /*Drop egress_port received packet to avoid looping back*/
    rv = port_learn_mode_set(unit, egress_port, 0);
    if (BCM_FAILURE(rv)) {
        printf("\nError in port_learn_mode_set: %s\n", bcm_errmsg(rv));
        return rv;
    }

    /*Configure static l2 entry for forwarding*/
    rv = add_l2_static_entry(unit,0,dst_mac,vlanId,egress_port);
    if (BCM_FAILURE(rv)) {
        printf("\nError in add_l2_static_entry: %s\n", bcm_errmsg(rv));
        return rv;
    }

    return rv;
}

/*
 * execute:
 *  This function does the following
 *  a) test setup
 *  b) actual configuration (Done in setup_vlan_switching())
 *  c) demonstrates the functionality(done in verify()).
 */
bcm_error_t
execute(void)
{
    bcm_error_t rv;
    int unit = 0;
    print "config show; attach; version";
    bshell(unit, "config show; attach ; version");

    print "~~~ #1) test_setup(): ** start **";
    if (BCM_FAILURE((rv = test_setup(unit)))) {
        printf("test_setup() failed.\n");
        return -1;
    }
    print "~~~ #1) test_setup(): ** end **";

    print "~~~ #2) setup_vlan_switching(): ** start **";
    if (BCM_FAILURE((rv = setup_vlan_switching(unit)))) {
        printf("setup_vlan_switching() failed.\n");
        return -1;
    }
    print "~~~ #2) setup_vlan_switching(): ** end **";

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
