/*  Feature  : Trunk failover configuration
 *
 *  Usage    : BCM.0> cint th4_hsdk_trunk_failover.c
 *
 *  config   : th4_hsdk_trunk_config.yml
 *
 *  Log file : th4_hsdk_trunk_failover_log.txt
 *
 *  Test Topology :
 *
 *                +---------------+
 *                |               |
 *                |               |        +--EgressPort1--->
 *                |               |        |
 *                |               |--trunk-+--EgressPort2--->
 *                |               |        |
 *                |               |        +--EgressPort3--->
 *  IngressPort-->|      TH4      |
 *                |               |
 *                |               |   +--Failover EgressPort1--->
 *                |               |---|
 *                |               |   +--Failover EgressPort2--->
 *                |               |
 *                +---------------+
 *
 * Summary:
 * ========
 * cint script to demonstrate unicast LAG failover scenario on trunk ports.
 *
 * Detailed steps done in the CINT script:
 * =======================================
 *   1) Step1 - Test Setup (Done in test_setup()):
 *   =============================================
 *     a) Selects six ports and configure them in PHY Loopback mode. Out of
 *        these, one port is ingress, three ports are trunk member ports and
 *        two ports are failover egress ports.
 *
 *   2) Step2 - Configuration (Done in trunk_failover_config()):
 *   =========================================================
 *     a) Ports are configured in the specific vlan
 *     c) L2 static entry is setup for unicast forwarding on trunk group.
 *
 *   3) Step3 - Verification (Done in test_verify()):
 *   ===========================================
 *     a) Check the outputs of 'vlan show' 'l2 show' 'show c' 'trunk show'
 *     b) shut or link down the failport and transmit the packets
 *        on ingress port
 *     d) Traffic is expected to load balance on failover ports
 *
 *    4) Expected Result:
 *     ===================
 *    a) The packet capture shows the egress packet transmitted and failover
 *       scenario is acheived.
 *
 *          Ingress Packets
 *          ===============
 *          DA 0x00000000AAAA
 *          SA 0x000000002222
 *          VLAN 12
 *          DIP 10.10.2.0 SIP 10.10.1.0
 *          Src Port 5000 Dst Port 80
 *          00 00 00 00 AA AA 00 00  00 00 22 22 81 00 00 0C
 *          08 00 45 00 00 51 00 01  00 00 40 11 63 88 0A 0A
 *          01 00 0A 0A 02 00 13 88  00 50 00 3D CE 40 28 20
 *          DB 20 00 20 00 20 45 20  00 20 00 20 54 20 00 20
 *          00 20 40 20 00 20 40 20  00 20 5E 20 A5 20 CA 20
 *          0B 20 28 20 9E 20 C0 20  A8 20 28 20 B2 20 08 20
 *          00 20 FF
 *
 *          DA 0x00000000AAAA
 *          SA 0x000000002222
 *          VLAN 12
 *          DIP 10.10.2.0 SIP 10.10.1.0
 *          Src Port 5001 Dst Port 81
 *          00 00 00 00 AA AA 00 00  00 00 22 22 81 00 00 0C
 *          08 00 45 00 00 51 00 01  00 00 40 11 63 86 0A 0A
 *          01 01 0A 0A 02 01 13 89  00 51 00 3D CE 3C 28 20
 *          DB 20 00 20 00 20 45 20  00 20 00 20 54 20 00 20
 *          00 20 40 20 00 20 40 20  00 20 5E 20 A5 20 CA 20
 *          0B 20 28 20 9E 20 C0 20  A8 20 28 20 B2 20 08 20
 *          00 20 FF
 *
 *          DA 0x00000000AAAA
 *          SA 0x000000002222
 *          VLAN 12
 *          DIP 10.10.2.0 SIP 10.10.1.0
 *          Src Port 5002 Dst Port 82
 *          00 00 00 00 AA AA 00 00  00 00 22 22 81 00 00 0C
 *          08 00 45 00 00 51 00 01  00 00 40 11 63 84 0A 0A
 *          01 02 0A 0A 02 02 13 8A  00 52 00 3D CE 38 28 20
 *          DB 20 00 20 00 20 45 20  00 20 00 20 54 20 00 20
 *          00 20 40 20 00 20 40 20  00 20 5E 20 A5 20 CA 20
 *          0B 20 28 20 9E 20 C0 20  A8 20 28 20 B2 20 08 20
 *          00 20 FF
 *
 *          DA 0x00000000AAAA
 *          SA 0x000000002222
 *          VLAN 12
 *          DIP 10.10.2.0 SIP 10.10.1.0
 *          Src Port 5003 Dst Port 83
 *          00 00 00 00 AA AA 00 00  00 00 22 22 81 00 00 0C
 *          08 00 45 00 00 51 00 01  00 00 40 11 63 82 0A 0A
 *          01 03 0A 0A 02 03 13 8B  00 53 00 3D CE 34 28 20
 *          DB 20 00 20 00 20 45 20  00 20 00 20 54 20 00 20
 *          00 20 40 20 00 20 40 20  00 20 5E 20 A5 20 CA 20
 *          0B 20 28 20 9E 20 C0 20  A8 20 28 20 B2 20 08 20
 *          00 20 FF
 *
 *       b) Expected Result
 *          ===============
 *          The packets should be load balanced across all the trunk member
 *          ports. This can be checked using "show c" and packet io callbacks.
 */

/* Reset C interpreter*/
cint_reset();

int num_egress_ports = 5;
int num_ports = 6;
int member_ports = 3;
int failover_ports = 2;
int port_list[num_ports];
int trunk_member_ports[member_ports];
int trunk_failover_ports[failover_ports];
bcm_field_group_t group;
bcm_field_entry_t entry[num_egress_ports];
bcm_port_t        ingress_port;
bcm_vlan_t        vid = 12;
bcm_pbmp_t        ing_pbmp, egr_pbmp;
bcm_pbmp_t        pbmp, ubmp;
uint32_t          stat_counter_id;
const uint8       priority = 101;
unsigned char    *expected_packets[4];
bcm_pbmp_t        expected_pbmp;
uint32            packet_patterns = 4;
int               test_failed;
int               pktio_rx_count;      /* Global received packet count */
bcm_trunk_t       trunk_id = 1;

/*
 * This function checked if given port/index is already present
 * in the list so that same ports number is not generated again.
 */
int checkPortAssigned(int *port_index_list, int no_entries, int index)
{
    int i = 0;

    for (i = 0; i < no_entries; i++) {
        if (port_index_list[i] == index) {
            return 1;
        }
    }

    /* no entry found */
    return 0;
}

/*
 * This function is written so that hardcoding of port
 * numbers in Cint scripts is removed. This function gives
 * required number of ports.
 */
bcm_error_t portNumbersGet(int unit, int *port_list_ptr, int num_ports)
{
    bcm_port_config_t  configP;
    bcm_pbmp_t         ports_pbmp;
    int                tempports[BCM_PBMP_PORT_MAX];
    int                port_index_list[num_ports];
    int                i = 0, port = 0, rv = 0, index = 0;

    rv = bcm_port_config_get(unit, &configP);
    if(BCM_FAILURE(rv)) {
        printf("\nError in retrieving port configuration: %s.\n",
                bcm_errmsg(rv));
        return rv;
    }

    ports_pbmp = configP.e;
    for (i = 1; i < BCM_PBMP_PORT_MAX; i++) {
        if (BCM_PBMP_MEMBER(&ports_pbmp, i)) {
            tempports[port] = i;
            port++;
        }
    }

    if (( port == 0 ) || ( port < num_ports )) {
        printf("portNumbersGet() failed \n");
        return -1;
    }

    /* generating random ports */
    for (i= 0; i < num_ports; i++) {
        index = sal_rand()% port;
        if (checkPortAssigned(port_index_list, i, index) == 0)
        {
            port_list_ptr[i] = tempports[index];
            port_index_list[i] = index;
        } else {
            i = i - 1;
        }
    }

    return BCM_E_NONE;
}

/*
 * Configures the port in loopback mode.
 */
bcm_error_t
ingress_port_multi_setup(int unit, bcm_pbmp_t pbm)
{
    bcm_port_t         port;

    BCM_PBMP_ITER(pbm, port) {
        printf("ingress_port_multi_setup port: %d setup\n", port);
        BCM_IF_ERROR_RETURN(bcm_port_loopback_set(unit, port,
                                                  BCM_PORT_LOOPBACK_PHY));
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
    bcm_field_group_config_t    group_config;
    bcm_port_t                  port;
    int                         i = 0;

    bcm_field_group_config_t_init(&group_config);
    BCM_FIELD_QSET_INIT(group_config.qset);
    BCM_FIELD_QSET_ADD(group_config.qset, bcmFieldQualifyInPort);
    BCM_FIELD_QSET_ADD(group_config.qset, bcmFieldQualifyStageLookup);
    BCM_FIELD_ASET_ADD(group_config.aset, bcmFieldActionCopyToCpu);
    BCM_FIELD_ASET_ADD(group_config.aset, bcmFieldActionDrop);
    group_config.priority = 2;
    BCM_IF_ERROR_RETURN(bcm_field_group_config_create(unit, &group_config));

    group = group_config.group;

    BCM_PBMP_ITER(pbm, port) {
        printf("egress_port_multi_setup port: %d setup\n", port);

        BCM_IF_ERROR_RETURN(bcm_port_loopback_set(unit, port,
                                                  BCM_PORT_LOOPBACK_PHY));
        BCM_IF_ERROR_RETURN(bcm_field_entry_create(unit, group, &entry[i]));
        BCM_IF_ERROR_RETURN(bcm_field_qualify_InPort(unit, entry[i], port,
                                                  BCM_FIELD_EXACT_MATCH_MASK));
        BCM_IF_ERROR_RETURN(bcm_field_action_add(unit, entry[i],
                                                 bcmFieldActionCopyToCpu,
                                                 0, 0));
        BCM_IF_ERROR_RETURN(bcm_field_action_add(unit, entry[i],
                                                 bcmFieldActionDrop, 0, 0));

        BCM_IF_ERROR_RETURN(bcm_field_entry_install(unit, entry[i]));

        i++;
    }

    return BCM_E_NONE;
}

bcm_error_t
ingress_port_multi_cleanup(int unit, bcm_pbmp_t pbm)
{
    bcm_port_t         port;

    BCM_PBMP_ITER(pbm, port) {
        printf("ingress_port_multi_cleanup port: %d cleanup\n", port);
        BCM_IF_ERROR_RETURN(bcm_port_loopback_set(unit, port,
                                                  BCM_PORT_LOOPBACK_NONE));
    }

    return BCM_E_NONE;
}

bcm_error_t
egress_port_multi_cleanup(int unit, bcm_pbmp_t pbm)
{
    bcm_port_t         port;
    int                rv, i;

    rv = bcm_field_group_remove(unit, group);
    if (BCM_FAILURE(rv)) {
        printf("bcm_field_group_remove: %s\n", bcm_errmsg(rv));
        return rv;
    }

    for (i = 0; i < num_egress_ports; i++) {
        rv = bcm_field_entry_destroy(unit, entry[i]);
        if (BCM_FAILURE(rv)) {
            printf("bcm_field_entry_destroy: %s\n", bcm_errmsg(rv));
            return rv;
        }
    }

    rv = bcm_field_group_destroy(unit, group);
    if (BCM_FAILURE(rv)) {
        printf("bcm_field_group_destroy: %s\n", bcm_errmsg(rv));
        return rv;
    }

    BCM_PBMP_ITER(pbm, port) {
        printf("egress_port_multi_cleanup port: %d cleanup\n", port);
        BCM_IF_ERROR_RETURN(bcm_port_loopback_set(unit, port,
                                                  BCM_PORT_LOOPBACK_NONE));
    }

    return BCM_E_NONE;
}


/* Register callback function for received packets. */
bcm_error_t
registerPktioRxCallback(int unit)
{
    const uint32    flags = 0;    /* Flags only used on match (for now) */

    return bcm_pktio_rx_register
      (unit, "rx_cb", pktioRxCallback, priority, &pktio_rx_count, flags);
}

bcm_error_t
unregisterPktioRxCallback(int unit)
{
    return bcm_pktio_rx_unregister(unit, pktioRxCallback, priority);
}

bcm_error_t
setup_flex_ctr(int unit)
{
    bcm_flexctr_index_operation_t  *index_op = NULL;
    bcm_flexctr_value_operation_t  *value_a_op = NULL;
    bcm_flexctr_value_operation_t  *value_b_op = NULL;
    bcm_flexctr_action_t            action = {0};
    int                             options = 0, rv, i;

    /* Counter index is egress port. */
    index_op = &action.index_operation;
    index_op->object[0] = bcmFlexctrObjectStaticEgrEgressPort;
    index_op->mask_size[0] = 9;
    index_op->shift[0] = 0;
    index_op->object[1] = bcmFlexctrObjectConstZero;
    index_op->mask_size[1] = 1;
    index_op->shift[1] = 0;

    /* Increase counter per packet. */
    value_a_op = &action.operation_a;
    value_a_op->select = bcmFlexctrValueSelectCounterValue;
    value_a_op->object[0] = bcmFlexctrObjectConstOne;
    value_a_op->mask_size[0] = 16;
    value_a_op->shift[0] = 0;
    value_a_op->object[1] = bcmFlexctrObjectConstZero;
    value_a_op->mask_size[1] = 1;
    value_a_op->shift[1] = 0;
    value_a_op->type = bcmFlexctrValueOperationTypeInc;

    /* Increase counter per packet bytes. */
    value_b_op = &action.operation_b;
    value_b_op->select = bcmFlexctrValueSelectPacketLength;
    value_b_op->type = bcmFlexctrValueOperationTypeInc;

    action.source = bcmFlexctrSourceEgrPort;
    action.hint = 0;
    action.mode = bcmFlexctrCounterModeNormal;
    action.index_num = 512;
    /* Enable to count egress mirrored packets. */
    action.egress_mirror_count_enable=1;

    rv = bcm_flexctr_action_create(unit, options, &action, &stat_counter_id);
    if (BCM_FAILURE(rv)) {
        printf("bcm_flexctr_action_create %s\n", bcm_errmsg(rv));
        return rv;
    }

    printf("\nstat_counter_id = 0x%x\n", stat_counter_id);

    /* Attach counter action to egress ports. */
    for(i = 1; i < num_ports; i++){
        rv = bcm_port_stat_attach(unit, port_list[i], stat_counter_id);
        if (BCM_FAILURE(rv)) {
            printf("bcm_vlan_stat_attach %s\n", bcm_errmsg(rv));
            return rv;
        }
    }


    return BCM_E_NONE;
}


bcm_error_t
cleanup_flex_ctr(int unit)
{
    int i, rv;

    /* Detach counter action. */
    for(i = 1; i < num_ports; i++){
        rv = bcm_port_stat_detach_with_id(unit, port_list[i], stat_counter_id);
        if (BCM_FAILURE(rv)) {
            printf("bcm_port_stat_detach_with_id: %s\n", bcm_errmsg(rv));
            return rv;
        }
    }

    /* Destroy counter action. */
    rv = bcm_flexctr_action_destroy(unit, stat_counter_id);
    if (BCM_FAILURE(rv)) {
        printf("bcm_flexctr_action_destroy %s\n", bcm_errmsg(rv));
        return rv;
    }

    return BCM_E_NONE;
}

/*
 * This function creates a trunk and adds the member ports
 */
bcm_error_t trunk_create(int unit, bcm_trunk_t *tid, int count,
                         bcm_port_t *member_ports)
{
    bcm_error_t rv = BCM_E_NONE;
    int i = 0;
    bcm_trunk_info_t trunk_info;
    bcm_trunk_member_t *member_info = NULL;
    bcm_trunk_member_t info;

    bcm_trunk_info_t_init(&trunk_info);

    member_info = sal_alloc((sizeof(info) * count), "trunk members");

    for (i = 0 ; i < count ; i++) {
        bcm_trunk_member_t_init(&member_info[i]);
        BCM_GPORT_MODPORT_SET(member_info[i].gport, 0, member_ports[i]);
    }

    trunk_info.psc = BCM_TRUNK_PSC_PORTFLOW;
    trunk_info.dlf_index = BCM_TRUNK_UNSPEC_INDEX;
    trunk_info.mc_index = BCM_TRUNK_UNSPEC_INDEX;
    trunk_info.ipmc_index = BCM_TRUNK_UNSPEC_INDEX;

    rv = bcm_trunk_create(unit, BCM_TRUNK_FLAG_WITH_ID, tid);
    if (BCM_FAILURE (rv))
    {
        printf ("Error executing bcm_trunk_create %s.\n", bcm_errmsg (rv));
        return rv;
    }

    rv = bcm_trunk_set(unit, *tid, &trunk_info, count, member_info);
    if (BCM_FAILURE (rv))
    {
        printf ("Error executing bcm_trunk_set %s.\n", bcm_errmsg (rv));
        return rv;
    }

    return rv;
}


/* This function adds a member to the trunk group */
bcm_error_t trunk_add_member(int unit, bcm_trunk_t tid, int port)
{
    bcm_error_t rv = BCM_E_NONE;
    bcm_trunk_member_t member_info;

    BCM_GPORT_MODPORT_SET(member_info.gport, 0, port);
    rv =  bcm_trunk_member_add(unit, tid, &member_info);
    if (BCM_FAILURE(rv)) {
        printf ("Error executing bcm_trunk_member_add %s.\n", bcm_errmsg(rv));
        return rv;
    }
    return rv;
}

/* This function removes a member from the trunk group */
bcm_error_t trunk_delete_member(int unit, bcm_trunk_t tid, int port)
{
    bcm_error_t rv = BCM_E_NONE;
    bcm_trunk_member_t member_info;

    BCM_GPORT_MODPORT_SET(member_info.gport, 0, port);
    rv = bcm_trunk_member_delete(unit, tid, &member_info);
    if (BCM_FAILURE(rv)) {
        printf("Error executing bcm_trunk_member_delete %s.\n",bcm_errmsg(rv));
        return rv;
    }

    return rv;
}

/* Update PSC of a trunk group */
bcm_error_t trunk_update_psc(int unit, bcm_trunk_t tid, int psc)
{
    bcm_error_t rv = BCM_E_NONE;
    int current_psc;

    rv = bcm_trunk_psc_get(unit, tid, &current_psc);
    if (BCM_FAILURE(rv))
    {
        printf ("Error executing bcm_trunk_psc_get %s.\n", bcm_errmsg (rv));
        return rv;
    }
    printf ("\nTrunk id %d current PSC value %d\n", tid, current_psc);

    rv = bcm_trunk_psc_set(unit, tid, psc);
    if (BCM_FAILURE(rv))
    {
        printf ("Error executing bcm_trunk_psc_set %s.\n", bcm_errmsg (rv));
        return rv;
    }
    printf ("\nUpdate trunk id %d PSC setting to %d\n", tid, psc);

    return rv;
}

/*
 *   Configure RTAG7 as hashing based on L4 ports of IPv4 packet
 */
bcm_error_t trunk_config_rtag7_src_dst_l4port(int unit, bcm_trunk_t tid)
{
    bcm_error_t rv = BCM_E_NONE;
    int flags = 0;

    /* Enable RTAG7 hashing on non-uc traffic */
    rv = bcm_switch_control_get(unit, bcmSwitchHashControl, &flags);
    if (BCM_FAILURE(rv))
    {
        printf ("Error executing bcm_switch_control_get for HashControl %s.\n",
                bcm_errmsg (rv));
        return rv;
    }
    flags |= BCM_HASH_CONTROL_TRUNK_NUC_ENHANCE;
    rv = bcm_switch_control_set(unit, bcmSwitchHashControl, flags);
    if (BCM_FAILURE(rv))
    {
        printf ("Error executing bcm_switch_control_set for HashControl %s.\n",
                bcm_errmsg (rv));
        return rv;
    }

    /* Use CRC32LO on Rtag7 hashing */
    rv = bcm_switch_control_set(unit, bcmSwitchHashField0Config,
                                BCM_HASH_FIELD_CONFIG_CRC32LO);
    if (BCM_FAILURE(rv))
    {
        printf ("Error executing bcm_switch_control_set for
                HashField0Config %s.\n", bcm_errmsg (rv));
        return rv;
    }

    rv = bcm_switch_control_get(unit, bcmSwitchHashSelectControl, &flags);
    if (BCM_FAILURE(rv))
    {
        printf ("Error executing bcm_switch_control_get for
                HashSelectControl %s.\n", bcm_errmsg (rv));
        return rv;
    }
    rv = bcm_switch_control_set(unit, bcmSwitchHashSeed0, 0x33333333);
    if (BCM_FAILURE(rv))
    {
        printf ("Error executing bcm_switch_control_set for HashSeed0 %s.\n",
                bcm_errmsg (rv));
        return rv;
    }

    rv = bcm_switch_control_set(unit, bcmSwitchHashSeed1, 0x55555555);
    if (BCM_FAILURE(rv))
    {
        printf ("Error executing bcm_switch_control_set for HashSeed1 %s.\n",
                bcm_errmsg (rv));
        return rv;
    }

    flags &= ~BCM_HASH_FIELD0_DISABLE_IP4;
    rv = bcm_switch_control_set(unit, bcmSwitchHashSelectControl, flags);
    if (BCM_FAILURE(rv))
    {
        printf ("Error executing bcm_switch_control_set for
                HashSelectControl %s.\n", bcm_errmsg (rv));
        return rv;
    }

    /* Enable src/dest hashing */
    flags =  BCM_HASH_FIELD_DSTL4 | BCM_HASH_FIELD_SRCL4;
    rv = bcm_switch_control_set(unit, bcmSwitchHashIP4Field0,
                           BCM_HASH_FIELD_SRCL4 | BCM_HASH_FIELD_DSTL4);
    if (BCM_FAILURE(rv))
    {
        printf ("Error executing bcm_switch_control_set for
                HashIP4Field0 %s.\n", bcm_errmsg (rv));
        return rv;
    }

    rv = bcm_switch_control_set(unit, bcmSwitchHashIP4TcpUdpField0,
                           BCM_HASH_FIELD_SRCL4 | BCM_HASH_FIELD_DSTL4);

    if (BCM_FAILURE(rv))
    {
        printf ("Error executing bcm_switch_control_set for
                HashIP4TcpUdpField0 %s.\n", bcm_errmsg (rv));
        return rv;
    }

    rv = bcm_switch_control_set(unit, bcmSwitchHashIP4TcpUdpPortsEqualField0,
                           BCM_HASH_FIELD_SRCL4 | BCM_HASH_FIELD_DSTL4);
    if (BCM_FAILURE(rv))
    {
        printf ("Error executing bcm_switch_control_set for
                HashIP4TcpUdpPortsEqualField0 %s.\n", bcm_errmsg (rv));
        return rv;
    }

    return rv;
}



/* Trunk failover configuration */
bcm_error_t trunk_failover_config(int unit)
{
    bcm_gport_t trunk_gp_id;
    bcm_l2_addr_t l2addr;
    bcm_mac_t da = {0x00, 0x00, 0x00, 0x00, 0xAA, 0xAA};
    int i = 0;
    bcm_error_t rv = BCM_E_NONE;
    bcm_trunk_chip_info_t ta_info;

    rv = bcm_port_control_set (unit, ingress_port,
                      bcmPortControlTrunkLoadBalancingRandomizer, 3);
    if (BCM_FAILURE (rv))
    {
        printf ("Error setting TrunkLoadBalancingRandomizer %s.\n",
                bcm_errmsg (rv));
        return rv;
    }

    /* Create the vlan with specific id */
    rv = bcm_vlan_create (unit, vid);
    if (BCM_FAILURE (rv))
    {
        printf ("Error executing bcm_vlan_create(): %s.\n", bcm_errmsg (rv));
        return rv;
    }

    rv = bcm_vlan_port_add (unit, vid, pbmp, ubmp);
    if (BCM_FAILURE (rv))
    {
        printf ("Error executing bcm_vlan_port_add(): %s\n", bcm_errmsg (rv));
        return rv;
    }

    /* Learn mode port config. */
    rv = port_learn_mode_set(unit, ingress_port,
                             BCM_PORT_LEARN_ARL | BCM_PORT_LEARN_FWD);
    if (BCM_FAILURE(rv)) {
        printf("\nError in port_learn_mode_set: %s\n", bcm_errmsg(rv));
        return rv;
    }

    /* Drop egress_port's received packet to avoid looping back. */
    for(i = 1; i < num_ports; i++)
    {
        rv = port_learn_mode_set(unit, port_list[i], 0);
        if (BCM_FAILURE(rv)) {
            printf("\nError in port_learn_mode_set: %s\n", bcm_errmsg(rv));
            return rv;
        }
    }

    rv = trunk_create(unit, &trunk_id, member_ports, trunk_member_ports);
    if (BCM_FAILURE (rv))
    {
        printf ("Error executing create_trunk(): %s.\n", bcm_errmsg (rv));
        return rv;
    }
    BCM_GPORT_TRUNK_SET (trunk_gp_id, trunk_id);

    bcm_trunk_info_t trunk_info;
    int member_count;
    int member_max = 4;
    bcm_trunk_member_t member_array[member_max];

    rv = bcm_trunk_get(unit, trunk_id, &trunk_info, member_max, member_array,
                       &member_count);
    if (BCM_FAILURE(rv))
    {
        printf ("Error executing bcm_trunk_get %s.\n", bcm_errmsg (rv));
        return rv;
    }

    /* failover config */
    bcm_gport_t fail_gport;
    bcm_gport_t failover_gport_array[failover_ports];

    /* Using second member port as failover port */
    BCM_GPORT_MODPORT_SET(fail_gport, 0, trunk_member_ports[1]);

    for (i = 0; i < failover_ports; i++) {
      BCM_GPORT_MODPORT_SET(failover_gport_array[i],0,trunk_failover_ports[i]);
    }

    rv = bcm_trunk_failover_set(unit,trunk_id,fail_gport,BCM_TRUNK_PSC_PORTFLOW,
                                0, failover_ports, failover_gport_array);
    if (BCM_FAILURE (rv))
    {
        printf ("Error in bcm_trunk_failover_set(): %s.\n", bcm_errmsg (rv));
        return rv;
    }

    int psc, array_count;
    unsigned int flags;
    bcm_gport_t fail_to_array[4];
    rv = bcm_trunk_failover_get(unit, trunk_id, fail_gport, &psc, &flags, 4,
                                &fail_to_array, &array_count);
    if (BCM_FAILURE (rv))
    {
        printf ("Error in bcm_trunk_failover_get(): %s.\n", bcm_errmsg (rv));
        return rv;
    }
    printf("bcm_trunk_failover_get() info :\n");
    print psc;
    print flags;
    print fail_to_array;
    print array_count;

    rv = trunk_config_rtag7_src_dst_l4port(unit, trunk_id);
    if (BCM_FAILURE (rv))
    {
        printf ("Error executing trunk_config_rtag7_src_dst_l4port(): %s.\n",
                bcm_errmsg (rv));
        return rv;
    }

    bcm_l2_addr_t_init (&l2addr, da, vid);
    l2addr.port = trunk_gp_id;
    rv = bcm_l2_addr_add (unit, &l2addr);
    if (BCM_FAILURE (rv))
    {
        printf ("Error executing bcm_l2_addr_add(): %s.\n", bcm_errmsg (rv));
        return rv;
    }

    return rv;
}

/* cleanup trunk config */
bcm_error_t
cleanup_trunk_config(int unit)
{

    /* Delete all members from trunk group 1 */
    BCM_IF_ERROR_RETURN(bcm_trunk_member_delete_all(unit, trunk_id));

    BCM_IF_ERROR_RETURN(bcm_trunk_destroy(unit, trunk_id));
    BCM_IF_ERROR_RETURN(bcm_trunk_detach(unit));

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
    int rv, i;

    if (BCM_E_NONE != portNumbersGet(unit, port_list, num_ports)) {
        printf("portNumbersGet() failed\n");
        return -1;
    }

    ingress_port = port_list[0];
    for(i = 0; i < member_ports; i++){
        trunk_member_ports[i] = port_list[1+i];
    }
    for(i = 0; i < failover_ports; i++){
        trunk_failover_ports[i] = port_list[4+i];
    }

    printf("Ingress port : %d\n", ingress_port);
    printf("Trunk member ports: %d %d %d\n", trunk_member_ports[0],
                     trunk_member_ports[1], trunk_member_ports[2]);
    printf("Trunk failport: %d\n", trunk_member_ports[1]);
    printf("Failover ports: %d %d\n", trunk_failover_ports[0],
                                      trunk_failover_ports[1]);

    BCM_PBMP_CLEAR(pbmp);
    BCM_PBMP_CLEAR(ubmp);
    for(i = 0; i < num_ports; i++){
        BCM_PBMP_PORT_ADD(pbmp, port_list[i]);
    }

    BCM_PBMP_CLEAR(ing_pbmp);
    BCM_PBMP_PORT_ADD(ing_pbmp, port_list[0]);
    if (BCM_E_NONE != ingress_port_multi_setup(unit, ing_pbmp)) {
        printf("ingress_port_multi_setup() failed for port %d\n",ingress_port);
        return -1;
    }

    BCM_PBMP_CLEAR(egr_pbmp);
    for(i = 1; i < num_ports; i++){
        BCM_PBMP_PORT_ADD(egr_pbmp, port_list[i]);
    }
    if (BCM_E_NONE != egress_port_multi_setup(unit, egr_pbmp)) {
        printf("egress_port_multi_setup() failed for port %d\n", egress_port);
        return -1;
    }

    if (BCM_E_NONE != trunk_failover_config(unit)) {
        printf("trunk_failover_config() failed.\n");
        return -1;
    }

    if (BCM_E_NONE != registerPktioRxCallback(unit)) {;
        printf("registerPktioRxCallback() FAILED: %s\n", bcm_errmsg(rv));
        return rv;
    }

    if (BCM_E_NONE != setup_flex_ctr(unit)) {
        printf("setup_flex_ctr() failed.\n");
        return -1;
    }

    print bshell(unit, "pw start report +raw +decode");

    return BCM_E_NONE;
}


/*
 * Clean up pre-test setup.
 */
int test_cleanup(int unit)
{
    if (BCM_E_NONE != cleanup_flex_ctr(unit)) {
        printf("cleanup_flex_ctr() failed.\n");
        return -1;
    }

    if (BCM_E_NONE != cleanup_trunk_config(unit)) {
        printf("cleanup_trunk_config() failed.\n");
        return -1;
    }

    /* Clean up learned L2 entries. */
    BCM_IF_ERROR_RETURN(bcm_l2_addr_delete_by_vlan(unit, vid, 0));

    /* Delete vlan member ports. */
    BCM_IF_ERROR_RETURN(bcm_vlan_port_remove(unit, vid, pbmp));

    /* Delete a vlan. */
    BCM_IF_ERROR_RETURN(bcm_vlan_destroy(unit, vid));

    BCM_IF_ERROR_RETURN(ingress_port_multi_cleanup(unit, ing_pbmp));
    BCM_IF_ERROR_RETURN(egress_port_multi_cleanup(unit, egr_pbmp));

    bshell(unit, "pw stop");
    unregisterPktioRxCallback(unit);

    return BCM_E_NONE;
}


/* Setup learning on port. */
bcm_error_t
port_learn_mode_set(int unit, bcm_port_t port, uint32 flags)
{
    bcm_error_t rv = BCM_E_NONE;

    rv = bcm_port_control_set(unit, port, bcmPortControlL2Learn, flags);
    if (BCM_FAILURE(rv)) {
        printf("Error in bcm_port_control_set(L2Learn): %s\n",bcm_errmsg(rv));
        return rv;
    }

    rv = bcm_port_control_set(unit, port, bcmPortControlL2Move, flags);
    if (BCM_FAILURE(rv)) {
        printf("Error in bcm_port_control_set(L2Move): %s\n", bcm_errmsg(rv));
        return rv;
    }

    return BCM_E_NONE;
}


/* Rx callback function for applications using the HSDK PKTIO. */
bcm_pktio_rx_t
pktioRxCallback(int unit, bcm_pktio_pkt_t * packet, void *cookie)
{
    int                i = 0;
    int  *count = (auto) cookie;
    void              *buffer;
    uint32             length;
    bcm_port_t         src_port;
    uint32             port;

    /* Get basic packet info. */
    if (BCM_FAILURE(bcm_pktio_pkt_data_get(unit, packet, (void *) &buffer,
                                           &length))) {
        return BCM_PKTIO_RX_NOT_HANDLED;
    }

    /* Get source port metadata. */
    BCM_IF_ERROR_RETURN(bcm_pktio_pmd_field_get (unit, packet,
                                                 bcmPktioPmdTypeRx,
                                                 BCM_PKTIO_RXPMD_SRC_PORT_NUM,
                                                 &port));

    printf("pktioRxCallback: packet received from port %d with length %d bytes",
           port, length);
    src_port = port;

    for (i = 0; i < packet_patterns; i++) {
        if ((sal_memcmp(buffer, expected_packets[i], length) == 0) &&
            BCM_PBMP_MEMBER(expected_pbmp, src_port)) {

                (*count)++; /* Bump up the rx count */
                printf("\npktioRxCallback: pktio_rx_count = 0x%x\n", *count);
        }
    }

    if ((i == packet_patterns) && (count == 0)) {
        test_failed = 1;   /* An unexpected packet. */
    }

    return BCM_PKTIO_RX_NOT_HANDLED;
}


/*Expected egress packets */
unsigned char expected_egress_packet1[99] = {
    0x00, 0x00, 0x00, 0x00, 0xaa, 0xaa, 0x00, 0x00, 0x00, 0x00, 0x22, 0x22,
    0x81, 0x00, 0x00, 0x0c, 0x08, 0x00, 0x45, 0x00, 0x00, 0x51, 0x00, 0x01,
    0x00, 0x00, 0x40, 0x11, 0x63, 0x88, 0x0a, 0x0a, 0x01, 0x00, 0x0a, 0x0a,
    0x02, 0x00, 0x13, 0x88, 0x00, 0x50, 0x00, 0x3d, 0xce, 0x40, 0x28, 0x20,
    0xdb, 0x20, 0x00, 0x20, 0x00, 0x20, 0x45, 0x20, 0x00, 0x20, 0x00, 0x20,
    0x54, 0x20, 0x00, 0x20, 0x00, 0x20, 0x40, 0x20, 0x00, 0x20, 0x40, 0x20,
    0x00, 0x20, 0x5e, 0x20, 0xa5, 0x20, 0xca, 0x20, 0x0b, 0x20, 0x28, 0x20,
    0x9e, 0x20, 0xc0, 0x20, 0xa8, 0x20, 0x28, 0x20, 0xb2, 0x20, 0x08, 0x20,
    0x00, 0x20, 0xff, };

unsigned char expected_egress_packet2[99] = {
    0x00, 0x00, 0x00, 0x00, 0xaa, 0xaa, 0x00, 0x00, 0x00, 0x00, 0x22, 0x22,
    0x81, 0x00, 0x00, 0x0c, 0x08, 0x00, 0x45, 0x00, 0x00, 0x51, 0x00, 0x01,
    0x00, 0x00, 0x40, 0x11, 0x63, 0x86, 0x0a, 0x0a, 0x01, 0x01, 0x0a, 0x0a,
    0x02, 0x01, 0x13, 0x89, 0x00, 0x51, 0x00, 0x3d, 0xce, 0x3c, 0x28, 0x20,
    0xdb, 0x20, 0x00, 0x20, 0x00, 0x20, 0x45, 0x20, 0x00, 0x20, 0x00, 0x20,
    0x54, 0x20, 0x00, 0x20, 0x00, 0x20, 0x40, 0x20, 0x00, 0x20, 0x40, 0x20,
    0x00, 0x20, 0x5e, 0x20, 0xa5, 0x20, 0xca, 0x20, 0x0b, 0x20, 0x28, 0x20,
    0x9e, 0x20, 0xc0, 0x20, 0xa8, 0x20, 0x28, 0x20, 0xb2, 0x20, 0x08, 0x20,
    0x00, 0x20, 0xff, };

unsigned char expected_egress_packet3[99] = {
    0x00, 0x00, 0x00, 0x00, 0xaa, 0xaa, 0x00, 0x00, 0x00, 0x00, 0x22, 0x22,
    0x81, 0x00, 0x00, 0x0c, 0x08, 0x00, 0x45, 0x00, 0x00, 0x51, 0x00, 0x01,
    0x00, 0x00, 0x40, 0x11, 0x63, 0x84, 0x0a, 0x0a, 0x01, 0x02, 0x0a, 0x0a,
    0x02, 0x02, 0x13, 0x8a, 0x00, 0x52, 0x00, 0x3d, 0xce, 0x38, 0x28, 0x20,
    0xdb, 0x20, 0x00, 0x20, 0x00, 0x20, 0x45, 0x20, 0x00, 0x20, 0x00, 0x20,
    0x54, 0x20, 0x00, 0x20, 0x00, 0x20, 0x40, 0x20, 0x00, 0x20, 0x40, 0x20,
    0x00, 0x20, 0x5e, 0x20, 0xa5, 0x20, 0xca, 0x20, 0x0b, 0x20, 0x28, 0x20,
    0x9e, 0x20, 0xc0, 0x20, 0xa8, 0x20, 0x28, 0x20, 0xb2, 0x20, 0x08, 0x20,
    0x00, 0x20, 0xff, };

unsigned char expected_egress_packet4[99] = {
    0x00, 0x00, 0x00, 0x00, 0xaa, 0xaa, 0x00, 0x00, 0x00, 0x00, 0x22, 0x22,
    0x81, 0x00, 0x00, 0x0c, 0x08, 0x00, 0x45, 0x00, 0x00, 0x51, 0x00, 0x01,
    0x00, 0x00, 0x40, 0x11, 0x63, 0x82, 0x0a, 0x0a, 0x01, 0x03, 0x0a, 0x0a,
    0x02, 0x03, 0x13, 0x8b, 0x00, 0x53, 0x00, 0x3d, 0xce, 0x34, 0x28, 0x20,
    0xdb, 0x20, 0x00, 0x20, 0x00, 0x20, 0x45, 0x20, 0x00, 0x20, 0x00, 0x20,
    0x54, 0x20, 0x00, 0x20, 0x00, 0x20, 0x40, 0x20, 0x00, 0x20, 0x40, 0x20,
    0x00, 0x20, 0x5e, 0x20, 0xa5, 0x20, 0xca, 0x20, 0x0b, 0x20, 0x28, 0x20,
    0x9e, 0x20, 0xc0, 0x20, 0xa8, 0x20, 0x28, 0x20, 0xb2, 0x20, 0x08, 0x20,
    0x00, 0x20, 0xff };

/*
 * This function does the following.
 * Transmit the below packet on ingress_port and verify.
 */
bcm_error_t test_verify(int unit)
{
    bcm_flexctr_counter_value_t counter_value;
    char     str[512];
    uint32   num_entries = 1;
    uint32   counter_index;
    int      expected_rx_count, expected_rx_bytes, pass;
    int      received_rx_count = 0, received_rx_bytes = 0;
    int      expectedcount = 4;
    int      rv, i;
    int      number_of_ingress_packets = 4;
    int      ingress_packet_length = 99; /* each packet bytes */

    pktio_rx_count = 0;
    test_failed = 0;
    expected_rx_count = number_of_ingress_packets;
    /* Flex counter includes 4 bytes crc */
    expected_rx_bytes = number_of_ingress_packets*(ingress_packet_length + 4);

    expected_packets[0]  = expected_egress_packet1;
    expected_packets[1]  = expected_egress_packet2;
    expected_packets[2]  = expected_egress_packet3;
    expected_packets[3]  = expected_egress_packet4;

    BCM_PBMP_CLEAR(expected_pbmp);
    /* all member ports and failover ports */
    for(i = 1; i < num_ports; i++){
        BCM_PBMP_PORT_ADD(expected_pbmp, port_list[i]);
    }

    printf("Disabling failport %d\n", trunk_member_ports[1]);
    snprintf(str, 512, "port %d lb=none", trunk_member_ports[1]);
    bshell(unit, str);
    print bshell(unit, "sleep 1");

    printf("\nSending known unicast pkt on ingress port %d\n", ingress_port);
    snprintf(str, 512, "tx 1 pbm=%d data=0x00000000AAAA0000000022228100000C08004500005100010000401163880A0A01000A0A020013880050003DCE402820DB200020002045200020002054200020002040200020402000205E20A520CA200B2028209E20C020A8202820B22008200020FF", ingress_port);
    bshell(unit, str);
    print bshell(unit, "sleep 1");

    snprintf(str, 512, "tx 1 pbm=%d data=0x00000000AAAA0000000022228100000C08004500005100010000401163860A0A01010A0A020113890051003DCE3C2820DB200020002045200020002054200020002040200020402000205E20A520CA200B2028209E20C020A8202820B22008200020FF", ingress_port);
    bshell(unit, str);
    print bshell(unit, "sleep 1");

    snprintf(str, 512, "tx 1 pbm=%d data=0x00000000AAAA0000000022228100000C08004500005100010000401163840A0A01020A0A0202138A0052003DCE382820DB200020002045200020002054200020002040200020402000205E20A520CA200B2028209E20C020A8202820B22008200020FF", ingress_port);
    bshell(unit, str);
    print bshell(unit, "sleep 1");

    snprintf(str, 512, "tx 1 pbm=%d data=0x00000000AAAA0000000022228100000C08004500005100010000401163820A0A01030A0A0203138B0053003DCE342820DB200020002045200020002054200020002040200020402000205E20A520CA200B2028209E20C020A8202820B22008200020FF", ingress_port);
    bshell(unit, str);

    print bshell(unit, "sleep 3");
    print bshell(unit, "ps");
    print bshell(unit, "show c");
    print bshell(unit, "l2 show");

    /* Get counter value for egress ports */
    for(i = 1; i < num_ports; i++) {
        sal_memset(&counter_value, 0, sizeof(counter_value));
        counter_index = port_list[i];
        rv = bcm_flexctr_stat_sync_get(unit,
                                   stat_counter_id,
                                   num_entries,
                                   &counter_index,
                                   &counter_value);
        if (BCM_FAILURE(rv)) {
            printf("bcm_flexctr_stat_sync_get %s\n", bcm_errmsg(rv));
            return rv;
        }

        /* Print counter value. */
        printf("\nFlexCtr Get on egress port %d : %d packets / %d bytes\n",
                                         port_list[i],
               COMPILER_64_LO(counter_value.value[0]),
               COMPILER_64_LO(counter_value.value[1]));

        /* NOTE : As per device bahavior, Tx/Rx packets are expected to see on
         * failport in link down state. So, avoiding stat count increment on
         * failport for validation.
        */
        if(port_list[i] != trunk_member_ports[1])
        {
            received_rx_count += COMPILER_64_LO(counter_value.value[0]);
            received_rx_bytes += COMPILER_64_LO(counter_value.value[1]);
        } else {
            printf("Avoiding stat's collection on failport %d\n",
                                            trunk_member_ports[1]);
        }
    }

    printf("\nTotal pktio_rx_count : %d\n", pktio_rx_count);

    pass = ((received_rx_count == expected_rx_count) &&
           (received_rx_bytes == expected_rx_bytes) &&
           (pktio_rx_count == expected_rx_count) &&
           (test_failed == 0)) ;

    printf("\n----------------------------------------------------------- \n");
    printf("    RESULT OF TRUNK VERIFICATION  ");
    printf("\n----------------------------------------------------------- \n");
    printf("Test = [%s]", pass? "PASS":"FAIL");
    printf("\n------------------------------------- \n");

    if (pass)
        return BCM_E_NONE;
    else
        return BCM_E_FAIL;
}


/*
 * execute:
 */
bcm_error_t
execute(void)
{
    int  unit = 0;
    int  rv;

    bshell(unit, "config show; attach; version");

    print "\n~~~ #1) test_setup(): ** start **";
    if (BCM_FAILURE((rv = test_setup(unit)))) {
        printf("test_setup() failed.\n");
        return -1;
    }
    print "~~~ #1) test_setup(): ** end **";

    print "\n~~~ #2) test_verify(): ** start **";
    test_verify(unit);
    print "~~~ #2) test_verify(): ** end **";

    print "\n~~~ #3) test_cleanup(): ** start **";
    if (BCM_FAILURE((rv = test_cleanup(unit)))) {
        printf("test_cleanup() failed.\n");
        return -1;
    }
    print "~~~ #3) test_cleanup(): ** end **\n";

    return BCM_E_NONE;
}

const char *auto_execute = (ARGC == 1) ? ARGV[0] : "YES";
if (!sal_strcmp(auto_execute, "YES")) {
    print "execute(): Start";
    print execute();
    print "execute(): End";
}
