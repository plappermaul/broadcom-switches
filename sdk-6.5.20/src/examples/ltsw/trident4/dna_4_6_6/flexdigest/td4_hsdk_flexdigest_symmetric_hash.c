/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

/*  Feature  : FlexDigest for LAG/Trunk
 *
 *  config   : td4_hsdk_flexdigest.config.yml
 *             (Usage: rack#./bcm.user -y ./td4_hsdk_flexdigest.config.yml)
 *
 *  Usage    : BCM.0> cint td4_hsdk_flexdigest_symmetric_hash.c
 *
 *  Log file : td4_hsdk_flexdigest_lag_log.txt
 *
 *  Test Topology :
 *      +-----------------+            +---------+
 *      |         ing-port|<-----------+         |
 *      |                 |            |         |
 *      |         lag-p1  +------------>         |
 *      |                 |            |         |
 *      |                 |            |Traffic  |
 *      |     SVK lag-p2  +------------>Generator|
 *      |                 |            |         |
 *      |         lag-p3  +------------>         |
 *      |                 |            |         |
 *      |         lag-p4  +------------>         |
 *      |                 |            |         |
 *      |                 |            |         |
 *      |                 |            |         |
 *      +-----------------+            +---------+
 *
 * Summary:
 * ========
 *   This Cint example to show configuration of the FlexDigest with symmetric hash
 *   scenario for LAG application using BCM APIs.
 *
 * Detailed steps done in the CINT script:
 * =======================================
 *   1) Step1 - Test Setup (Done in test_setup()):
 *   =============================================
 *     a) Selects five ports and configure them in Loopback mode. Out of these
 *        five ports, one port is used as ingress_port and the rest all as
 *        egress_ports.
 *
 *   2) Step2 - Configuration (Done in config_flexdigest_lag()):
 *   =========================================================
 *     a) Configure a basic port, vlan, flexdigest with symmetric hash settings
 *        and LAG/trunk functional scenario and does the necessary configurations.
 *
 *   3) Step3 - Verification (Done in verify()):
 *   ===========================================
 *     a) Check the configurations by 'vlan show', 'l2 show', and 'trunk show'.
 *
 *     b) Transmit the 2 UC packets with Incrementing SIP and DIP from ingress-port.
 *        And reverse the SIP and DIPs and resend the same 2 packet to verify the
 *        symmetric hash functionality. The contents of the packet
 *        are printed on screen.
 *
 *     c) Expected Result:
 *     ===================
 *       We can see that the packets/traffic is splits in a way that the
 *       traffic load is balanced across multiple egress-ports ( i.e LAG ports).
 *       Observer that, egress-ports should be same for 2nd set of traffic as well.
 *       Also see 'show c' to check the Tx/Rx packet stats/counters.
 */

cint_reset();

bcm_port_t ingress_port;
int i;
int port_count = 5;
int port_count_max = 7;
bcm_port_t port_arr[8] = {0};
bcm_vlan_t vid = 11;
uint8 L2IifOpaqueCtrlId = 1;

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
setup_port(int unit, int L2IifOpaqueCtrlId, int port_count, bcm_port_t *port_arr)
{
    int i;
    for (i = 0; i < port_count; i++) {
        BCM_IF_ERROR_RETURN(bcm_port_class_set(unit, port_arr[i], bcmPortClassOpaqueCtrlId, L2IifOpaqueCtrlId));
        BCM_IF_ERROR_RETURN(bcm_port_loopback_set(unit, port_arr[i], BCM_PORT_LOOPBACK_MAC));
    }
    return BCM_E_NONE;
}

bcm_error_t
configure_HashOutSelection(int unit, bcm_port_t port)
{
    /* Trunk UC and Non-UC hashout selection */
    BCM_IF_ERROR_RETURN(bcm_switch_control_set(unit, bcmSwitchHashUseFlowSelTrunkUc, 0));
    BCM_IF_ERROR_RETURN(bcm_switch_control_set(unit, bcmSwitchHashUseFlowSelTrunkNonUnicast, 0));
    /* Setting to test the symmetric hash functionality */
    BCM_IF_ERROR_RETURN(bcm_switch_control_port_set(unit, port, bcmSwitchTrunkUnicastHashOffset, 48));
    BCM_IF_ERROR_RETURN(bcm_switch_control_port_set(unit, port, bcmSwitchTrunkNonUnicastHashOffset, 48));
    return BCM_E_NONE;
}

bcm_error_t
configure_port_vlan(int unit, uint16 vid, int port_count, bcm_port_t *port_arr)
{
    int rv = 0;
    int i;
    bcm_pbmp_t pbmp, ubmp;

    /* Port VLAN configuration */
    BCM_PBMP_CLEAR(ubmp);
    BCM_PBMP_CLEAR(pbmp);
    for (i = 0; i < port_count; i++) {
        BCM_PBMP_PORT_ADD(pbmp, port_arr[i]);
    }
    BCM_IF_ERROR_RETURN(bcm_vlan_create(unit, vid));
    BCM_IF_ERROR_RETURN(bcm_vlan_port_add(unit, vid, pbmp, ubmp));

    return rv;
}

bcm_error_t
configure_FlexDigest_symmetrichash(int unit, uint8 L2IifOpaqueCtrlId)
{
    int seed_profile_id, norm_profile_id, hash_profile_id;
    int value;
    int debug = 0;
    uint16 salt_a[16] = {
        0xabcd,
        0xbbcb,
        0xbabe,
        0x0123,
        0x7654,
        0xabcd,
        0xaaaa,
        0x5555,
        0xffff,
        0x0000,
        0xa5a5,
        0xface,
        0xbbbb,
        0xcccc,
        0xdddd,
        0xeeee
    };
    bcm_flexdigest_qset_t qset;
    bcm_flexdigest_group_t group;
    bcm_flexdigest_entry_t entry;
    bcm_flexdigest_match_id_t match_id;
    int pri = 0;

    BCM_IF_ERROR_RETURN
        (bcm_flexdigest_init(unit));

    BCM_IF_ERROR_RETURN
        (bcm_flexdigest_norm_seed_profile_create(unit, 0, &seed_profile_id));
    value = 0xbabaface;
    BCM_IF_ERROR_RETURN
        (bcm_flexdigest_norm_seed_profile_set(unit, seed_profile_id,
                                              bcmFlexDigestNormSeedControlBinASeed,
                                              value));
    value = 0xa5a5a5a5;
    BCM_IF_ERROR_RETURN
        (bcm_flexdigest_norm_seed_profile_set(unit, seed_profile_id,
                                              bcmFlexDigestNormSeedControlBinBSeed,
                                              value));
    value = 0x5a5a5a5a;
    BCM_IF_ERROR_RETURN
        (bcm_flexdigest_norm_seed_profile_set(unit, seed_profile_id,
                                              bcmFlexDigestNormSeedControlBinCSeed,
                                              value));
    if (debug) {
        bshell(unit, "bsh -c 'lt FLEX_DIGEST_NORM_PROFILE_SEED traverse -l'");
    }

    BCM_IF_ERROR_RETURN
        (bcm_flexdigest_norm_profile_create(unit, 0, &norm_profile_id));

    value = 1;
    BCM_IF_ERROR_RETURN
        (bcm_flexdigest_norm_profile_set(unit, norm_profile_id,
                                         bcmFlexDigestNormProfileControlNorm, value));
    value = 0;
    BCM_IF_ERROR_RETURN
        (bcm_flexdigest_norm_profile_set(unit, norm_profile_id,
                                         bcmFlexDigestNormProfileControlSeedId, value));

    if (debug) {
        bshell(unit, "bsh -c 'lt FLEX_DIGEST_NORM_PROFILE traverse -l'");
    }

    BCM_IF_ERROR_RETURN
        (bcm_flexdigest_hash_profile_create(unit, 0, &hash_profile_id));

    if (debug) {
        bshell(unit, "bsh -c 'lt FLEX_DIGEST_HASH_PROFILE traverse -l'");
    }
    BCM_IF_ERROR_RETURN
        (bcm_flexdigest_hash_profile_set(unit, hash_profile_id,
                                         bcmFlexDigestHashProfileControlPreProcessBinA,
                                         1));
    BCM_IF_ERROR_RETURN
        (bcm_flexdigest_hash_profile_set(unit, hash_profile_id,
                                         bcmFlexDigestHashProfileControlPreProcessBinB,
                                         1));
    BCM_IF_ERROR_RETURN
        (bcm_flexdigest_hash_profile_set(unit, hash_profile_id,
                                         bcmFlexDigestHashProfileControlPreProcessBinC,
                                         0));

    BCM_IF_ERROR_RETURN
        (bcm_flexdigest_hash_profile_set(unit, hash_profile_id,
                                         bcmFlexDigestHashProfileControlXorSaltBinA,
                                         1));
    BCM_IF_ERROR_RETURN
        (bcm_flexdigest_hash_profile_set(unit, hash_profile_id,
                                         bcmFlexDigestHashProfileControlXorSaltBinB,
                                         1));
    BCM_IF_ERROR_RETURN
        (bcm_flexdigest_hash_profile_set(unit, hash_profile_id,
                                         bcmFlexDigestHashProfileControlXorSaltBinC,
                                         0));

    BCM_IF_ERROR_RETURN
        (bcm_flexdigest_hash_profile_set(unit, hash_profile_id,
                                         bcmFlexDigestHashProfileControlBinA0FunctionSelection,
                                         BCM_FLEXDIGEST_HASH_FUNCTION_CRC32LO));
    BCM_IF_ERROR_RETURN
        (bcm_flexdigest_hash_profile_set(unit, hash_profile_id,
                                         bcmFlexDigestHashProfileControlBinA1FunctionSelection,
                                         BCM_FLEXDIGEST_HASH_FUNCTION_CRC32HI));
    BCM_IF_ERROR_RETURN
        (bcm_flexdigest_hash_profile_set(unit, hash_profile_id,
                                         bcmFlexDigestHashProfileControlBinB0FunctionSelection,
                                         BCM_FLEXDIGEST_HASH_FUNCTION_CRC32_ETH_LO));
    BCM_IF_ERROR_RETURN
        (bcm_flexdigest_hash_profile_set(unit, hash_profile_id,
                                         bcmFlexDigestHashProfileControlBinB1FunctionSelection,
                                         BCM_FLEXDIGEST_HASH_FUNCTION_CRC32_ETH_HI));
    BCM_IF_ERROR_RETURN
        (bcm_flexdigest_hash_profile_set(unit, hash_profile_id,
                                         bcmFlexDigestHashProfileControlBinC0FunctionSelection,
                                         BCM_FLEXDIGEST_HASH_FUNCTION_CRC16));
    BCM_IF_ERROR_RETURN
        (bcm_flexdigest_hash_profile_set(unit, hash_profile_id,
                                         bcmFlexDigestHashProfileControlBinC1FunctionSelection,
                                         BCM_FLEXDIGEST_HASH_FUNCTION_CRC16CCITT));
    if (debug) {
        bshell(unit, "bsh -c 'lt FLEX_DIGEST_HASH_PROFILE traverse -l'");
    }


    if (debug) {
        bshell(unit, "bsh -c 'lt FLEX_DIGEST_HASH_SALT traverse -l'");
    }

    BCM_IF_ERROR_RETURN
        (bcm_flexdigest_hash_salt_set(unit, bcmFlexDigestHashBinA, 16, salt_a));
    BCM_IF_ERROR_RETURN
        (bcm_flexdigest_hash_salt_set(unit, bcmFlexDigestHashBinB, 16, salt_a));

    if (debug) {
        bshell(unit, "bsh -c 'lt FLEX_DIGEST_HASH_SALT traverse -l'");
    }


    if (debug) {
        bshell(unit, "bsh -c 'lt FLEX_DIGEST_LKUP_PRESEL traverse -l'");
    }
    BCM_FLEXDIGEST_QSET_INIT(qset);
    BCM_FLEXDIGEST_QSET_ADD(qset, bcmFlexDigestQualifyMatchId);
    BCM_FLEXDIGEST_QSET_ADD(qset, bcmFlexDigestQualifyL4Valid);
    BCM_FLEXDIGEST_QSET_ADD(qset, bcmFlexDigestQualifyMyStation1Hit);
    BCM_FLEXDIGEST_QSET_ADD(qset, bcmFlexDigestQualifyL2IifOpaqueCtrlId);
    BCM_IF_ERROR_RETURN
        (bcm_flexdigest_group_create(unit, qset, pri, &group));
    if (debug) {
        bshell(unit, "bsh -c 'lt FLEX_DIGEST_LKUP_PRESEL traverse -l'");
    }

    BCM_IF_ERROR_RETURN
        (bcm_flexdigest_entry_create(unit, group, &entry));
    BCM_IF_ERROR_RETURN
        (bcm_flexdigest_entry_priority_set(unit, entry, 2));

    BCM_IF_ERROR_RETURN
        (bcm_flexdigest_match_id_create(unit, &match_id));
    BCM_IF_ERROR_RETURN
        (bcm_flexdigest_match_add(unit, match_id, bcmFlexDigestMatchOuterL2HdrL2));
    BCM_IF_ERROR_RETURN
        (bcm_flexdigest_match_add(unit, match_id, bcmFlexDigestMatchOuterL2HdrOtag));
    BCM_IF_ERROR_RETURN
        (bcm_flexdigest_qualify_MatchId(unit, entry, match_id));

    BCM_IF_ERROR_RETURN
        (bcm_flexdigest_qualify_L4Valid(unit, entry, 1, 0x1));
    BCM_IF_ERROR_RETURN
        (bcm_flexdigest_qualify_MyStation1Hit(unit, entry, 1, 0x1));
    BCM_IF_ERROR_RETURN
        (bcm_flexdigest_qualify_L2IifOpaqueCtrlId(unit, entry, L2IifOpaqueCtrlId, 0xf));

    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetACmds0,  BCM_FLEXDIGEST_FIELD_OUTER_MACSA_0_15, 0xffff));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetACmds1,  BCM_FLEXDIGEST_FIELD_OUTER_MACSA_16_31, 0xffff));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetACmds2,  BCM_FLEXDIGEST_FIELD_OUTER_MACSA_32_47, 0xffff));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetACmds3,  BCM_FLEXDIGEST_FIELD_OUTER_MACDA_0_15, 0xffff));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetACmds4,  BCM_FLEXDIGEST_FIELD_OUTER_MACDA_16_31, 0xffff));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetACmds5,  BCM_FLEXDIGEST_FIELD_OUTER_MACDA_32_47, 0xffff));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetACmds6,  BCM_FLEXDIGEST_FIELD_OUTER_OTAG_0_15, 0xffff));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetACmds7,  BCM_FLEXDIGEST_FIELD_OUTER_OTAG_16_31, 0xffff));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetACmds8,  BCM_FLEXDIGEST_FIELD_OUTER_ETHERTYPE, 0xffff));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetACmds9,  BCM_FLEXDIGEST_FIELD_INGRESS_PP_PORT, 0xffff));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetACmds10, BCM_FLEXDIGEST_FIELD_L2_IIF, 0xffff));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetACmds11, BCM_FLEXDIGEST_FIELD_NORMALIZED_SEED_A_0_15, 0xffff));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetACmds12, BCM_FLEXDIGEST_FIELD_NORMALIZED_SEED_A_16_31, 0xffff));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetACmds13, BCM_FLEXDIGEST_FIELD_NORMALIZED_SEED_B_0_15, 0xffff));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetACmds14, BCM_FLEXDIGEST_FIELD_NORMALIZED_SEED_B_16_31, 0xffff));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetACmds15, BCM_FLEXDIGEST_FIELD_NORMALIZED_SEED_C_0_15, 0xffff));

    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetBCmds0,  BCM_FLEXDIGEST_FIELD_NORMALIZED_OUTER_IP_ADDR_LSB32_LO_0_15, 0xffff));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetBCmds1,  BCM_FLEXDIGEST_FIELD_NORMALIZED_OUTER_IP_ADDR_LSB32_LO_16_31, 0xffff));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetBCmds8,  BCM_FLEXDIGEST_FIELD_NORMALIZED_OUTER_IP_ADDR_LSB32_HI_0_15, 0xffff));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetBCmds9,  BCM_FLEXDIGEST_FIELD_NORMALIZED_OUTER_IP_ADDR_LSB32_HI_16_31, 0xffff));

    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetCCmds0, 1, 0x1));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetCCmds1, 1, 0x1));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetCCmds2, 1, 0x1));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetCCmds3, 1, 0x1));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetCCmds4, 1, 0x1));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetCCmds5, 1, 0x1));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetCCmds6, 0, 0x1));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetCCmds7, 0, 0x1));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetCCmds8, 0, 0x1));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetCCmds9, 0, 0x1));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetCCmds10, 0, 0x1));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetCCmds11, 0, 0x1));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetCCmds12, 0, 0x1));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetCCmds13, 0, 0x1));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetCCmds14, 0, 0x1));
    BCM_IF_ERROR_RETURN(bcm_flexdigest_action_add(unit, entry, bcmFlexDigestActionExtractBinSetCCmds15, 0, 0x1));

    if (debug) {
        bshell(unit, "bsh -c 'lt FLEX_DIGEST_LKUP_MASK_PROFILE traverse -l'");
        bshell(unit, "bsh -c 'lt FLEX_DIGEST_LKUP traverse -l'");
    }

    BCM_IF_ERROR_RETURN
        (bcm_flexdigest_entry_install(unit, entry));

    if (debug) {
        bshell(unit, "bsh -c 'lt FLEX_DIGEST_LKUP_MASK_PROFILE traverse -l'");
        bshell(unit, "bsh -c 'lt FLEX_DIGEST_LKUP traverse -l'");
    }

    BCM_IF_ERROR_RETURN
        (bcm_flexdigest_qualifier_delete(unit, entry, bcmFlexDigestQualifyL4Valid));
    BCM_IF_ERROR_RETURN
        (bcm_flexdigest_qualifier_delete(unit, entry, bcmFlexDigestQualifyMyStation1Hit));
    BCM_IF_ERROR_RETURN
        (bcm_flexdigest_action_delete(unit, entry, bcmFlexDigestActionExtractBinSetACmds15));

    BCM_IF_ERROR_RETURN
        (bcm_flexdigest_entry_reinstall(unit, entry));

    if (debug) {
        bshell(unit, "bsh -c 'lt FLEX_DIGEST_LKUP_MASK_PROFILE traverse -l'");
        bshell(unit, "bsh -c 'lt FLEX_DIGEST_LKUP traverse -l'");
    }
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
    trunk_info.flags = BCM_TRUNK_FLAG_FAILOVER_ALL_LOCAL;
    trunk_info.psc = BCM_TRUNK_PSC_PORTFLOW;
    trunk_info.dlf_index = BCM_TRUNK_UNSPEC_INDEX;
    trunk_info.mc_index = BCM_TRUNK_UNSPEC_INDEX;
    trunk_info.ipmc_index = BCM_TRUNK_UNSPEC_INDEX;

    for (i = 0; i < port_count; i++) {
        bcm_trunk_member_t_init(&member_array[i]);
        BCM_GPORT_MODPORT_SET(member_array[i].gport, 0, port_arr[i]);
        }

    BCM_IF_ERROR_RETURN(bcm_trunk_set(unit,tid,&trunk_info,port_count,member_array));

    for (i = 0; i < port_count; i++) {
        BCM_IF_ERROR_RETURN(bcm_port_learn_set(unit, port_arr[i], 0));
    }

    uint8 mac[6] = {0x00, 0x00, 0x00, 0xbb, 0xbb, 0xbb};
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
    BCM_IF_ERROR_RETURN(configure_FlexDigest_symmetrichash(unit, L2IifOpaqueCtrlId));
    BCM_IF_ERROR_RETURN(configure_HashOutSelection(unit, ingress_port));
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
    BCM_IF_ERROR_RETURN(setup_port(unit, L2IifOpaqueCtrlId, port_count + 1, port_arr));

    return BCM_E_NONE;
}

void
verify(int unit)
{
    int i;
    char   str[512];
    uint64 in_pkt, out_pkt;


    printf("Verify pkts from port %d forwarding to tid 0; Increment SIP & DIP\n", ingress_port);
    snprintf(str, 512, "tx 1 pbm=%d data=0x000000BBBBBB0000010000008100000B08004500002E0000000040FF3D960A0A0A0A14141414000102030405060708090A0B0C0D0E0F10111213141516171819B71F5B42", ingress_port);
    printf("%s\n", str);
    bshell(unit, str);

    snprintf(str, 512, "tx 1 pbm=%d data=0x000000BBBBBB0000010000008100000B08004500002E0000000040FF3D940A0A0A0B14141415000102030405060708090A0B0C0D0E0F10111213141516171819F6C57D66", ingress_port);
    printf("%s\n", str);
    bshell(unit, str);

    printf("Executing 'l2 show'\n");
    bshell(unit, "sleep 2");
    bshell(unit, "l2 show");

    printf("Executing 'show c'\n");
    bshell(unit, "sleep 2");
    bshell(unit, "show c");

    bshell(unit, "sleep 2");
    bshell(unit, "clear c");
    printf("Verify pkts from port %d forwarding to tid 0; Just reverse the SIP and DIP to verify the symmetric hash scenario\n", ingress_port);
    snprintf(str, 512, "tx 1 pbm=%d data=0x000000BBBBBB0000010000008100000B08004500002E0000000040FF3D96141414140A0A0A0A000102030405060708090A0B0C0D0E0F101112131415161718198F9CDF03", ingress_port);
    printf("%s\n", str);
    bshell(unit, str);

    snprintf(str, 512, "tx 1 pbm=%d data=0x000000BBBBBB0000010000008100000B08004500002E0000000040FF3D94141414150A0A0A0B000102030405060708090A0B0C0D0E0F10111213141516171819CE46F927", ingress_port);
    printf("%s\n", str);
    bshell(unit, str);

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

    print "~~~ #2) config_flexdigest_lag(): ** start **";
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
