/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */
/*
 * Feature  : Flex Digest configuration along with Trunk on TD4
 *
 * Usage    : BCM.0> cint td4_hsdk_port_trunk_flex_digest_src_port_knock.c
 *
 * config   : bcm56880_a0-generic.yml
 *
 * Log file : td4_hsdk_port_trunk_flex_digest_src_port_knock_log.txt
 *
 * Test Topology :
 *
 *                     +-------------------+
 *                     |                   |
 *                     |  L2 ENTRY         |
 *                     | +--------+  EgressHigig3Port1+----------->
 *                     |  mac=0x1          |
 *                     |  vlan=2           |
 *                     |  Trunk=1    EgressHigig3Port2+----------->
 *                     |                   |
 *                     |                   |
 *    +-----------Ingressport        EgressHigig3Port3+----------->
 *                     |                   |
 *    +---->           |                   |
 *  Ingress Pkts:      |             EgressHigig3Port4+----------->
 *                     |                   |
 *  Set-1:             |                   |
 *  DMAC=00::01        |             EgressHigig3Port5+----------->
 *  SMAC=00:XX         |                   |
 *  VLAN=2             |                   |         TRUNK
 *                     |                   |         ID = 0
 *                     |                   |
 *                     |   BCM56880(TD4)   |
 *                     +-------------------+
 *
 * Summary:
 * ========
 * cint script to demonstrate unicast LAG resolution forwarding on trunk ports and
 * source prot knock when the same port sent on one of the trunks port.
 *
 * Detailed steps done in the CINT script:
 * =======================================
 *   1) Step1 - Test Setup (Done in test_setup()):
 *   =============================================
 *     a) Selects Five ports and configure them in Loopback mode. Out of these
 *        three ports, one port is used as ingress_port and the other Four as
 *        egress_port.
 *
 *   2) Step2 - Configuration (Done in):
 *   =========================================================
 *     a) Ports are configured in the specific vlan
 *     b) L2 static entry is setup for unicast forwarding on trunk group.
 *     c) L2 learn mode is setup on ingress and egress ports
 *
 *   3) Step3 - Verification (Done in verify()):
 *   ===========================================
 *     a) Check the outputs of 'vlan show' 'l2 show' 'show c' 'trunk show'
 *
 *     c) Expected Result:
 *     ===================
 *     The packet capture shows the egress packet transmitted on outgoing ports as per the forwarding path setup
 *
 *   Ingress Packet:
 *  Incrementa source mac is sent on the ingress port and packet are forwarded based on LAG resolution.
 *  tx 20 pbm=%d DestMac=0x01  SourceMac=0x3214 SourceMacInc=1 vlan=2 len=80
 *
 *   Egress Packet:
 *   packets based on LAG resolution forwarding on trunk ports and srouce port knockout
 */

cint_reset();

bcm_port_t ingress_port,source_port_knock;
int i;
int port_count = 5;
int port_count_max = 7;
bcm_port_t port_arr[8] = {0};
bcm_gport_t gport_array[8]={0};
bcm_vlan_t vid = 2;


/*
 * This function checked if given port/index is already present
 * in the list so that same ports number is not generated again
*/
int
checkPortAssigned(int *port_index_list,int no_entries, int index)
{
  int i=0;

  for (i= 0; i < no_entries; i++) {
    if (port_index_list[i] == index)
      return 1;
  }

  /* no entry found */
  return 0;
}
/*
 * This function is written so that hardcoding of port
 * numbers in Cint scripts is removed. This function gives
 * required number of ports
*/
bcm_error_t
portNumbersGet(int unit, int *port_list, int num_ports)
{
    int i = 0, port = 0,rv = 0, index = 0;
    bcm_port_config_t configP;
    bcm_pbmp_t ports_pbmp;
    int tempports[BCM_PBMP_PORT_MAX];
    int port_index_list[num_ports];

    rv = bcm_port_config_get(unit,&configP);
    if (BCM_FAILURE(rv)) {
        printf("\nError in retrieving port configuration: %s.\n",bcm_errmsg(rv));
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
        if (checkPortAssigned(port_index_list, i, index) == 0) {
            port_list[i] = tempports[index];
            port_index_list[i] = index;
        } else {
            i = i - 1;
        }
    }

    return BCM_E_NONE;
}
bcm_error_t
setup_port(int unit, int port_count, bcm_port_t *port_arr)
{
    int i;
    for (i = 0; i < port_count; i++) {
         
        BCM_IF_ERROR_RETURN(bcm_port_loopback_set(unit, port_arr[i], BCM_PORT_LOOPBACK_MAC));
    }
    return BCM_E_NONE;
}
bcm_error_t
configure_port_vlan(int unit, uint16 vid, int port_count, bcm_port_t *port_arr)
{
    int rv = 0;
    int i;
    bcm_pbmp_t pbmp, ubmp;
    bcm_multicast_t l2mc_group = vid;
    uint32_t flags = BCM_MULTICAST_TYPE_L2 | BCM_MULTICAST_WITH_ID;


    /* Port VLAN configuration */
    BCM_PBMP_CLEAR(ubmp);
    BCM_PBMP_CLEAR(pbmp);
    for (i = 0; i < port_count; i++) {
        BCM_PBMP_PORT_ADD(pbmp, port_arr[i]);
    }
    BCM_IF_ERROR_RETURN(bcm_vlan_create(unit, vid));
    BCM_IF_ERROR_RETURN(bcm_vlan_port_add(unit, vid, pbmp, ubmp));
    
     BCM_IF_ERROR_RETURN(bcm_multicast_create(unit, flags, &l2mc_group));

    for (i = 0; i < port_count; i++) {
     BCM_IF_ERROR_RETURN(bcm_port_gport_get(unit, port_arr[i], &gport_array[i])); 
      BCM_IF_ERROR_RETURN(bcm_multicast_egress_add(unit, l2mc_group, gport_array[i], -1));
    
    }

    return rv;
}

bcm_error_t
configure_FlexDigest(int unit)
{
   /*###########################*/
   /* FLEX DIGEST CONFIGURATION */
   /*###########################*/
   printf("Flex Digest Configuration:\n");
   int rv;
   int seed_profile_id, norm_profile_id, hash_profile_id;
   int norm_enable;
   int seed;
   int debug = 0;

   BCM_IF_ERROR_RETURN(bcm_flexdigest_init(unit));


    /**** NORMALIZATION and SEED SETUP ****/
    printf("Flex Digest Normalization setup:\n");
    seed_profile_id = 1;
    BCM_IF_ERROR_RETURN(bcm_flexdigest_norm_seed_profile_create(unit, BCM_FLEXDIGEST_PROFILE_CREATE_OPTIONS_WITH_ID, &seed_profile_id));

    seed = 0xbabaface;
    BCM_IF_ERROR_RETURN(bcm_flexdigest_norm_seed_profile_set(unit, seed_profile_id, bcmFlexDigestNormSeedControlBinASeed, seed));
    seed = 0xa5a5a5a5;
    BCM_IF_ERROR_RETURN(bcm_flexdigest_norm_seed_profile_set(unit, seed_profile_id, bcmFlexDigestNormSeedControlBinBSeed, seed));
    seed = 0x5a5a5a5a;
    BCM_IF_ERROR_RETURN(bcm_flexdigest_norm_seed_profile_set(unit, seed_profile_id, bcmFlexDigestNormSeedControlBinCSeed, seed));


    norm_profile_id = 0;
    BCM_IF_ERROR_RETURN(bcm_flexdigest_norm_profile_create(unit, BCM_FLEXDIGEST_PROFILE_CREATE_OPTIONS_WITH_ID, &norm_profile_id));

    norm_enable = TRUE;
    BCM_IF_ERROR_RETURN(bcm_flexdigest_norm_profile_set(unit, norm_profile_id, bcmFlexDigestNormProfileControlNorm, norm_enable));

    BCM_IF_ERROR_RETURN(bcm_flexdigest_norm_profile_set(unit, norm_profile_id, bcmFlexDigestNormProfileControlSeedId, seed_profile_id));

    /**** LOOKUP SETUP ****/
    printf("Flex Digest Lookup setup:\n");
    bcm_flexdigest_qset_t qset;

    BCM_FLEXDIGEST_QSET_INIT(qset);
    BCM_FLEXDIGEST_QSET_ADD(qset, bcmFlexDigestQualifyMatchId);

    bcm_flexdigest_group_t group;
    bcm_flexdigest_entry_t entry;
    bcm_flexdigest_match_id_t match_id;
    int pri = 0, mask_1, mask_2;

    mask_1 = 0xFFFF; /*General Mask for all full fields */
    mask_2 = 0x0FFF; /*For VLAN ID from VLAN tag */

    int use_a_bin = 1; /*C_BIN[x] = A_BIN[x]*/
    int not_use_a_bin = 0; /*C_BIN[x] = 16 bits of all zeros */
    int c_mask = 1;

    /*** GROUP 0 (Pri = 0) for L2 packets ***/
    printf("Flex Digest Lookup Group 0:\n");
    pri = 0;
    BCM_IF_ERROR_RETURN(bcm_flexdigest_group_create(unit, qset, pri, &group));

    BCM_IF_ERROR_RETURN(bcm_flexdigest_entry_create(unit, group, &entry));

    BCM_IF_ERROR_RETURN(bcm_flexdigest_match_id_create(unit, &match_id));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_match_add(unit, match_id, bcmFlexDigestMatchOuterL2HdrL2));

    /* Qualifiers */
    BCM_IF_ERROR_RETURN(bcm_flexdigest_qualify_MatchId(unit, entry, match_id));

    /* Actions : Bin field population */
    /* Set A */
    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetACmds0, BCM_FLEXDIGEST_FIELD_INGRESS_PP_PORT, mask_1));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetACmds1, BCM_FLEXDIGEST_FIELD_OUTER_OTAG_0_15, mask_2));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetACmds2, BCM_FLEXDIGEST_FIELD_OUTER_ETHERTYPE, mask_1));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetACmds3, BCM_FLEXDIGEST_FIELD_OUTER_MACDA_0_15, mask_1));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetACmds4, BCM_FLEXDIGEST_FIELD_OUTER_MACDA_16_31, mask_1));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetACmds5, BCM_FLEXDIGEST_FIELD_OUTER_MACDA_32_47, mask_1));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetACmds6, BCM_FLEXDIGEST_FIELD_OUTER_MACSA_0_15, mask_1));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetACmds7, BCM_FLEXDIGEST_FIELD_OUTER_MACSA_16_31, mask_1));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetACmds8, BCM_FLEXDIGEST_FIELD_OUTER_MACSA_32_47, mask_1));

    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetACmds14, BCM_FLEXDIGEST_FIELD_NORMALIZED_SEED_A_0_15, mask_1));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetACmds15, BCM_FLEXDIGEST_FIELD_NORMALIZED_SEED_C_0_15, mask_1));

    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetCCmds0, use_a_bin, c_mask));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetCCmds1, use_a_bin, c_mask));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetCCmds2, use_a_bin, c_mask));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetCCmds3, use_a_bin, c_mask));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetCCmds4, use_a_bin, c_mask));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetCCmds5, use_a_bin, c_mask));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetCCmds6, not_use_a_bin, c_mask));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetCCmds7, not_use_a_bin, c_mask));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetCCmds8, not_use_a_bin, c_mask));

    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetCCmds14, not_use_a_bin, c_mask));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetCCmds15, use_a_bin, c_mask));

    BCM_IF_ERROR_RETURN(bcm_flexdigest_entry_install(unit, entry));

    /*** GROUP 1 (Pri = 1) for IPV4 and IPv4 UDP packets ***/
    printf("Flex Digest Lookup Group 1 Entry 0:\n");
    pri = 1;
   BCM_IF_ERROR_RETURN(bcm_flexdigest_group_create(unit, qset, pri, &group));

    /** Entry 0 : IPv4 UDP **/
    BCM_IF_ERROR_RETURN(bcm_flexdigest_entry_create(unit, group, &entry));

    BCM_IF_ERROR_RETURN(bcm_flexdigest_match_id_create(unit, &match_id));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_match_add(unit, match_id, bcmFlexDigestMatchOuterL3L4HdrUdp));

    /* Qualifiers */
    BCM_IF_ERROR_RETURN(bcm_flexdigest_qualify_MatchId(unit, entry, match_id));

    /* Actions : Bin field population */
    /* Set A */
    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetACmds3, BCM_FLEXDIGEST_FIELD_OUTER_L4_DST_PORT, mask_1));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetACmds4, BCM_FLEXDIGEST_FIELD_OUTER_L4_SRC_PORT, mask_1));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetACmds5, BCM_FLEXDIGEST_FIELD_OUTER_IP_HDR_SIP_0_15, mask_1));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetACmds6, BCM_FLEXDIGEST_FIELD_OUTER_IP_HDR_SIP_16_31, mask_1));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetACmds7, BCM_FLEXDIGEST_FIELD_OUTER_IP_HDR_DIP_0_15, mask_1));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetACmds8, BCM_FLEXDIGEST_FIELD_OUTER_IP_HDR_DIP_16_31, mask_1));

    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetCCmds3, use_a_bin, c_mask));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetCCmds4, not_use_a_bin, c_mask));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetCCmds5, not_use_a_bin, c_mask));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetCCmds6, not_use_a_bin, c_mask));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetCCmds7, use_a_bin, c_mask));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetCCmds8, use_a_bin, c_mask));


    BCM_IF_ERROR_RETURN(bcm_flexdigest_entry_install(unit, entry));

    /** Entry 1 : IPv4 Unknown Protocol Type **/
    printf("Flex Digest Lookup Group 1 Entry 1:\n");
    BCM_IF_ERROR_RETURN(bcm_flexdigest_entry_create(unit, group, &entry));

    BCM_IF_ERROR_RETURN(bcm_flexdigest_match_id_create(unit, &match_id));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_match_add(unit, match_id, bcmFlexDigestMatchOuterL3L4HdrUnknownL4));

    /* Qualifiers */
    BCM_IF_ERROR_RETURN(bcm_flexdigest_qualify_MatchId(unit, entry, match_id));

    /* Actions : Bin field population */
    /* Set A */
    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetACmds3, BCM_FLEXDIGEST_FIELD_OUTER_IP_HDR_PROTOCOL, mask_1));
    BCM_IF_ERROR_RETURN( bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetACmds4, BCM_FLEXDIGEST_FIELD_OUTER_IP_HDR_TTL, mask_1));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetACmds5, BCM_FLEXDIGEST_FIELD_OUTER_IP_HDR_SIP_0_15, mask_1));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetACmds6, BCM_FLEXDIGEST_FIELD_OUTER_IP_HDR_SIP_16_31, mask_1));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetACmds7, BCM_FLEXDIGEST_FIELD_OUTER_IP_HDR_DIP_0_15, mask_1));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetACmds8, BCM_FLEXDIGEST_FIELD_OUTER_IP_HDR_DIP_16_31, mask_1));

    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetCCmds3, use_a_bin, c_mask));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetCCmds4, use_a_bin, c_mask));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetCCmds5, not_use_a_bin, c_mask));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetCCmds6, not_use_a_bin, c_mask));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetCCmds7, use_a_bin, c_mask));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetCCmds8, use_a_bin, c_mask));

   BCM_IF_ERROR_RETURN(bcm_flexdigest_entry_install(unit, entry));

    /*** HASH SETUP ***/
    printf("Flex Digest Hash setup:\n");
    hash_profile_id = 0;
    BCM_IF_ERROR_RETURN(bcm_flexdigest_hash_profile_create(unit,
    BCM_FLEXDIGEST_PROFILE_CREATE_OPTIONS_WITH_ID, &hash_profile_id));

    uint16 salt[16] = {
    0xabcd, 0xbbcb, 0xbabe, 0x0123, 0x7654, 0xabcd, 0xaaaa, 0x5555,
    0xffff, 0x0000, 0xa5a5, 0xface, 0xbbbb, 0xcccc, 0xdddd, 0xeeee
    };

    BCM_IF_ERROR_RETURN(bcm_flexdigest_hash_salt_set(unit, bcmFlexDigestHashBinA, sizeof(salt)/sizeof(salt[0]), salt));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_hash_salt_set(unit, bcmFlexDigestHashBinC, sizeof(salt)/sizeof(salt[0]), salt));

    int enable = 1;
    BCM_IF_ERROR_RETURN(bcm_flexdigest_hash_profile_set(unit, hash_profile_id, bcmFlexDigestHashProfileControlPreProcessBinA, enable));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_hash_profile_set(unit, hash_profile_id, bcmFlexDigestHashProfileControlPreProcessBinC, enable));

    BCM_IF_ERROR_RETURN( bcm_flexdigest_hash_profile_set(unit, hash_profile_id, bcmFlexDigestHashProfileControlXorSaltBinA, enable));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_hash_profile_set(unit, hash_profile_id, bcmFlexDigestHashProfileControlXorSaltBinC, enable));

    BCM_IF_ERROR_RETURN(bcm_flexdigest_hash_profile_set(unit, hash_profile_id, bcmFlexDigestHashProfileControlBinA0FunctionSelection, BCM_FLEXDIGEST_HASH_FUNCTION_CRC32HI));
    BCM_IF_ERROR_RETURN( bcm_flexdigest_hash_profile_set(unit, hash_profile_id, bcmFlexDigestHashProfileControlBinA1FunctionSelection, BCM_FLEXDIGEST_HASH_FUNCTION_CRC32LO));

    BCM_IF_ERROR_RETURN(bcm_flexdigest_hash_profile_set(unit, hash_profile_id, bcmFlexDigestHashProfileControlBinC0FunctionSelection, BCM_FLEXDIGEST_HASH_FUNCTION_CRC32HI));
    BCM_IF_ERROR_RETURN( bcm_flexdigest_hash_profile_set(unit, hash_profile_id, bcmFlexDigestHashProfileControlBinC1FunctionSelection, BCM_FLEXDIGEST_HASH_FUNCTION_CRC32LO));


    /**** HASH BITS SELECTION SETUP ****/
    printf("Trunk Unicast and Non-unicast Hash bits selection setup:\n");
    int trunk_uc_flow_select = 0;   /*Use Port based hash */
    int trunk_uc_offset = 1;        /*A0 block with Offset 1*/

    BCM_IF_ERROR_RETURN(bcm_switch_control_set(unit, bcmSwitchHashUseFlowSelTrunkUc, trunk_uc_flow_select));
    BCM_IF_ERROR_RETURN(bcm_switch_control_set(unit, bcmSwitchTrunkUnicastHashOffset, trunk_uc_offset));

    int trunk_nonuc_flow_select = 0;    /*Use Port based hash */
    int trunk_nonuc_offset = 65;        /*C1 block with Offset 1*/

    BCM_IF_ERROR_RETURN(bcm_switch_control_set(unit, bcmSwitchHashUseFlowSelTrunkNonUnicast, trunk_nonuc_flow_select));
    BCM_IF_ERROR_RETURN(bcm_switch_control_set(unit, bcmSwitchTrunkNonUnicastHashOffset, trunk_nonuc_offset));
    return BCM_E_NONE;
}
int
configure_trunk_setup(int unit, int port_count, bcm_port_t *port_arr, bcm_vlan_t vid)
{
    int i;
    int tid = 0;
    bcm_trunk_info_t trunk_info;
    bcm_trunk_member_t member_array[8];

    BCM_IF_ERROR_RETURN(bcm_trunk_create(unit,BCM_TRUNK_FLAG_WITH_ID,&tid));
    bcm_trunk_info_t_init(&trunk_info);
    /*trunk_info.flags = BCM_TRUNK_FLAG_FAILOVER_ALL_LOCAL;*/
    trunk_info.psc = BCM_TRUNK_PSC_PORTFLOW;
    trunk_info.dlf_index = BCM_TRUNK_UNSPEC_INDEX;
    trunk_info.mc_index = BCM_TRUNK_UNSPEC_INDEX;
    trunk_info.ipmc_index = BCM_TRUNK_UNSPEC_INDEX;

    for (i = 0; i < port_count; i++) {
        bcm_trunk_member_t_init(&member_array[i]);
        BCM_GPORT_MODPORT_SET(member_array[i].gport, 0, port_arr[i]);
        if (i==1){
         source_port_knock = port_arr[i];
        }  
     }
    
    BCM_IF_ERROR_RETURN(bcm_trunk_set(unit,tid,&trunk_info,port_count,member_array));

    for (i = 0; i < port_count; i++) {
        BCM_IF_ERROR_RETURN(bcm_port_learn_set(unit, port_arr[i], 0));
    }

    uint8 mac[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x01};
    bcm_l2_addr_t l2addr;
    bcm_l2_addr_t_init(&l2addr, mac, vid);
    l2addr.port = tid;
    l2addr.flags |= BCM_L2_STATIC;
    l2addr.flags |= BCM_L2_TRUNK_MEMBER;
    BCM_IF_ERROR_RETURN(bcm_l2_addr_add(unit, &l2addr));
    return BCM_E_NONE;
}
bcm_error_t
config_flexdigest_lag(int unit)
{
    BCM_IF_ERROR_RETURN(configure_port_vlan(unit, vid, port_count + 1, port_arr));
    BCM_IF_ERROR_RETURN(configure_FlexDigest(unit));
    /*BCM_IF_ERROR_RETURN(configure_HashOutSelection(unit, ingress_port));*/
    BCM_IF_ERROR_RETURN(configure_trunk_setup(unit, port_count, port_arr, vid));

    return BCM_E_NONE;
}
bcm_error_t
test_setup(int unit)
{
    if (port_count > port_count_max) {
        return BCM_E_PARAM;
    }
    BCM_IF_ERROR_RETURN(portNumbersGet(unit, port_arr, port_count + 1));
    ingress_port = port_arr[port_count];
    BCM_IF_ERROR_RETURN(setup_port(unit, port_count + 1, port_arr));

    return BCM_E_NONE;
}
void
verify(int unit)
{
    int i;
    int pkt_cnt = 16;
    char   str[512];
    uint64 in_pkt, out_pkt;

    printf("Verify %d UC pkts from port %d forwarding to tid 0\n", pkt_cnt, ingress_port);
    snprintf(str, 512, "tx 20 pbm=%d DestMac=0x01  SourceMac=0x3214 SourceMacInc=1 vlan=2 len=80; sleep quiet 1", ingress_port);
    printf("%s\n", str);
    bshell(unit, str);
    printf("Executing 'l2 show'\n");
    bshell(unit, "l2 show");
    bshell(unit, "sleep 2");
    printf("Executing 'show c'\n");
    bshell(unit, "show c");
    bshell(unit, "clear c");
    printf("Executing 'source knock out'\n");
    snprintf(str, 512, "tx 20 pbm=%d DestMac=0x01  SourceMac=0x3214 SourceMacInc=1 vlan=2 len=80; sleep quiet 1", source_port_knock);
    bshell(unit, str);
    printf("%s\n", str);
    printf("Executing 'show c'\n");
    bshell(unit, "sleep 2");
    bshell(unit, "show c");
    return BCM_E_NONE;
}
/*
 * execute:
 *  This function does the following
 *  a) test setup
 *  b) actual configuration (Done in config_flexdigest_lag())
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

    print "~~~ #2) configure_FlexDigest(): ** start **";
    if (BCM_FAILURE((rv = config_flexdigest_lag(unit)))) {
        printf("config_flexdigest_lag() failed.\n");
        return -1;
    }
    print "~~~ #2) config_flexdigest_lag(): ** end **";

    print "~~~ #3) verify(): ** start **";
    verify(unit);
    print "~~~ #3) verify(): ** end **";

    return BCM_E_NONE;
}
const char *auto_execute = (ARGC == 1) ? ARGV[0] : "YES";
if (!sal_strcmp(auto_execute, "YES")) {
    print execute();
}
