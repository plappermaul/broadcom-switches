/*
 * $Id: cint_oam_cfm_o_eth_o_pwe_o_eth.c ,v 1.2 2013/09/03 11:28:44 Exp $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 * 
 * File: cint_oam_cfm_o_eth_o_pwe_o_eth.c
 * Purpose:  using CFMoEthoMplsoEth, 
 *
 * 
 */


/* 
 * In order to enable inititalization of the OAM TCAM to identify CFMoEthoMplsoEth, 
 * enable soc property, custom_feature_oam_downmep_pwe_classification=1
 * 
 * This feature supports CFM identification per-md level. 
 * This feature does not support identification per opcode. All CFM packets will be asociated with opcode=1 (CCM).
 * Inner Ethernet frames with 0 or 1 VLAN tags preceeding the CFM EtherType are supported.
 *
 * Supported packet terminations:
 *  SOC_PPC_PKT_TERM_TYPE_MPLS_ETH: Link-Layer and MPLS tunnel were terminated
 *  SOC_PPC_PKT_TERM_TYPE_CW_MPLS_ETH: Link-Layer and MPLS (with Control Word) tunnel were terminated
 *  SOC_PPC_PKT_TERM_TYPE_MPLS2_ETH: Link-Layer and MPLSx2 tunnel were terminated
 *  SOC_PPC_PKT_TERM_TYPE_CW_MPLS2_ETH: Link-Layer and MPLSx2 (with Control Word) tunnel were terminated
 *  SOC_PPC_PKT_TERM_TYPE_MPLS3_ETH: Link-Layer and MPLSx3 tunnel were terminated
 *  SOC_PPC_PKT_TERM_TYPE_CW_MPLS3_ETH: Link-Layer and MPLSx3 (with Control Word) tunnel were terminated
 */


/*
 * To run P2MP:
 * cint ../../../../src/examples/dpp/cint_port_tpid.c
 * cint ../../../../src/examples/dpp/cint_advanced_vlan_translation_mode.c
 * cint ../../../../src/examples/dpp/cint_qos.c
 * cint ../../../../src/examples/dpp/utility/cint_utils_l3.c
 * cint ../../../../src/examples/dpp/cint_mpls_lsr.c
 * cint ../../../../src/examples/dpp/cint_vswitch_metro_mp.c
 * cint ../../../../src/examples/dpp/cint_vswitch_vpls.c
 * cint ../../../../src/examples/dpp/cint_multi_device_utils.c
 * cint ../../../../src/examples/dpp/utility/cint_utils_oam.c
 * cint ../../../../src/examples/dpp/cint_queue_tests.c
 * cint ../../../../src/examples/dpp/cint_oam.c
 * cint ../../../../src/examples/dpp/cint_oam_cfm_o_eth_o_pwe_o_eth.c
 * cint
 * print oam_eth_o_mpls_o_eth_run_with_defaults(unit, 13, 14, 100, 200);
 */

/*
 * To run P2P:
 * cint ../../../../src/examples/dpp/cint_port_tpid.c
 * cint ../../../../src/examples/dpp/cint_advanced_vlan_translation_mode.c
 * cint ../../../../src/examples/dpp/cint_qos.c
 * cint ../../../../src/examples/dpp/cint_qos_vpls.c
 * cint  ../../../../src/examples/dpp/cint_vswitch_cross_connect_p2p.c
 * cint ../../../../src/examples/dpp/cint_oam_cfm_o_eth_o_pwe_o_eth.c
 * cint
 * print oam_eth_o_mpls_o_eth_run_with_defaults_p2p(unit, 13, 14, 100, 200);
 *  
 *  
 */




/* PIPE mode not used */
uint8 pipe_mode_exp_set = 0;

/**
 * Create P-2-MP  AC to VPLS settings and configure OAM endpoint
 * on the PWE LIF. 
 * 
 * @author sinai (16/04/2014)
 * 
 * @param unit 
 * @param ac_port 
 * @param pwe_port 
 * @param ac_inner_vlan 
 * @param ac_outer_vlan 
 * 
 * @return int 
 */
int oam_eth_o_mpls_o_eth_run_with_defaults(int unit, int ac_port, int pwe_port, int ac_inner_vlan, int ac_outer_vlan, int enable_trap) {
	int rv;
	bcm_oam_group_info_t group;
	bcm_gport_t gport;
	bcm_oam_endpoint_info_t ep;

	rv = bcm_port_class_set(unit, ac_port, bcmPortClassId, pwe_port);
	if (rv != BCM_E_NONE) {
		printf("Error in bcm_port_class_set\n");
		print rv;
		return rv;
	}

	/**
	 * Use default labels. See "Packets Received from PWE1"  figure
	 * in cint_vswitch_vpls.c 
	 *  
	 */
	vswitch_vlps_info_init(unit, ac_port, pwe_port, pwe_port, ac_outer_vlan, ac_inner_vlan, 0, 0, 4096 );

	rv = vswitch_vpls_run(unit, -1,0); 
	if (rv != BCM_E_NONE) {
		printf("Error in vswitch_vpls_run\n");
		print rv;
		return rv;
	}

	bcm_oam_group_info_t_init(&group);
	group.name[0] = 1;
	group.name[1] = 3;
	group.name[2] = 2;
	group.name[3] = 0xcd;
	group.name[4] = 0xab;
	rv = bcm_oam_group_create(unit, &group);
	if (rv != BCM_E_NONE) {
		printf("Error in bcm_oam_group_create\n");
		print rv;
		return rv;
	}

	bcm_oam_endpoint_info_t_init(& ep);
	ep.type = bcmOAMEndpointTypeEthernet;
	ep.group = group.id;
	ep.level  = 3;
	ep.dst_mac_address[0] = 0x01;
	ep.dst_mac_address[1] = 0x80;
	ep.dst_mac_address[2] = 0xc2;
	ep.dst_mac_address[3] = 0x00;
	ep.dst_mac_address[4] = 0x00;
	ep.dst_mac_address[5] = 0x30+ep.level;
	ep.gport = network_port_id; /* gport created by the VPN. Global variable defined in cint_vswitch_vpls.c*/
	ep.timestamp_format = soc_property_get(unit,"oam_dm_ntp_enable",1) ? bcmOAMTimestampFormatNTP : bcmOAMTimestampFormatIEEE1588v1;
	rv =  bcm_oam_endpoint_create(unit, &ep);
	if (rv != BCM_E_NONE) {
		printf("Error in bcm_oam_endpoint_create\n");
		print rv;
		return rv;
	}

    /* two calls, one for each action, will create two traps.
       Can be done using only one trap. In order to use the existing function, called twice */
	if(enable_trap){
		rv = oam_action_set(unit, ac_port, ep.id, bcmOAMActionMcFwd, 1 /* CCM */);
		if (rv != BCM_E_NONE) {
		  printf("Error in oam_action_set\n");
		  print rv;
		  return rv;
		}
           
		rv = oam_action_set(unit, ac_port, ep.id, bcmOAMActionUcFwd, 1 /* CCM */);
		if (rv != BCM_E_NONE) {
		  printf("Error in oam_action_set\n");
		  print rv;
		  return rv;
		}
	}
	return 0;
}


/**
 * Create P2P AC to VPLS setting and configure OAM endpoint on 
 * PWE LIF. 
 *  
 * Use configurations according to 
 * cint_vswtich_cross_connect_p2p.c 
 *  
 * @author sinai (17/04/2014)
 * 
 * @param unit 
 * @param ac_port 
 * @param pwe_port 
 * 
 * @return int 
 */
int oam_eth_o_mpls_o_eth_run_with_defaults_p2p(int unit, int ac_port , int pwe_port) {
	int rv;
	bcm_oam_group_info_t group;
	bcm_gport_t gport;
	bcm_oam_endpoint_info_t ep;

	rv = run(unit,pwe_port, 2 /* mpls */,0 /* protection */,0 /* protection */, ac_port, 1 /* vlan */ ,0,0,0);

	bcm_oam_group_info_t_init(&group);
	group.name[0] = 1;
	group.name[1] = 3;
	group.name[2] = 2;
	group.name[3] = 0xcd;
	group.name[4] = 0xab;
	rv = bcm_oam_group_create(unit, &group);
	if (rv != BCM_E_NONE) {
		printf("Error in bcm_oam_group_create\n");
		print rv;
		return rv;
	}

	bcm_oam_endpoint_info_t_init(&ep);
	ep.type = bcmOAMEndpointTypeEthernet;
	ep.group = group.id;
	ep.level  = 4;
	ep.dst_mac_address[0] = 0x01;
	ep.dst_mac_address[1] = 0x80;
	ep.dst_mac_address[2] = 0xc2;
	ep.dst_mac_address[3] = 0x00;
	ep.dst_mac_address[4] = 0x00;
	ep.dst_mac_address[5] = 0x30 + ep.level;
	ep.gport = cross_connect_info.gports.port1; /* gport created by cint_vswitch_cross_connect_p2p.c*/
	ep.timestamp_format = soc_property_get(unit,"oam_dm_ntp_enable",1) ? bcmOAMTimestampFormatNTP : bcmOAMTimestampFormatIEEE1588v1;
	rv =  bcm_oam_endpoint_create(unit, &ep);
	if (rv != BCM_E_NONE) {
		printf("Error in bcm_oam_endpoint_create\n");
		print rv;
		return rv;
	}

	return 0;
}
