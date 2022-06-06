/* $Id: cint_utils_port.c,v 1.10 2013/02/03 10:59:10 	Mark Exp $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * This file provides port basic functionality
 */


/* Set VLAN domain */
int port__vlan_domain__set(int unit, bcm_port_t port, int vlan_domain){

    return bcm_port_class_set(unit, port, bcmPortClassId, vlan_domain);
}

int port__tpid__set(int unit, bcm_port_t port, uint16 outer_tpid, uint16 inner_tpid)
{
    int rv;

    /* Set outer tpid */
    rv = bcm_port_tpid_set(unit, port, outer_tpid);
    if (rv != BCM_E_NONE) {
        printf("Error, in bcm_port_tpid_set, port=%d, \n",  port);
        return rv;
    }

    if(inner_tpid != 0)
    {
        /* Set inner TPID on port */
        rv = bcm_port_inner_tpid_set(unit, port, inner_tpid);
        if (rv != BCM_E_NONE) {
            printf("Error, in bcm_port_inner_tpid_set, port=%d, \n",  port);
            return rv;
        }
    }

    return rv;
}

int port__basic_tpid_class__set(int unit, bcm_port_t port)
{
    bcm_port_tpid_class_t port_tpid_class;
    int rv;

    bcm_port_tpid_class_t_init(&port_tpid_class);
    sal_memset(&port_tpid_class, 0, sizeof (port_tpid_class));

    port_tpid_class.port  = port;
    port_tpid_class.tpid1 = BCM_PORT_TPID_CLASS_TPID_ANY;
    port_tpid_class.tpid2 = BCM_PORT_TPID_CLASS_TPID_ANY;
    port_tpid_class.tag_format_class_id = 2;  /* Set tag format to '2' for all TPID types  */
    rv = bcm_port_tpid_class_set(unit, &port_tpid_class);
    if (rv != BCM_E_NONE) {
        printf("Error, in bcm_port_tpid_class_set, port=%d, \n", port);
        return rv;
    }

    bcm_port_tpid_class_t_init(&port_tpid_class);
    sal_memset(&port_tpid_class, 0, sizeof (port_tpid_class));

    port_tpid_class.port  = port;
    port_tpid_class.tpid1 = BCM_PORT_TPID_CLASS_TPID_ANY;
    port_tpid_class.tpid2 = BCM_PORT_TPID_CLASS_TPID_INVALID;
    port_tpid_class.tag_format_class_id = 1;  /* Set tag format to '1' for all TPID types with one TPID  */
    rv = bcm_port_tpid_class_set(unit, &port_tpid_class);
    if (rv != BCM_E_NONE) {
        printf("Error, in bcm_port_tpid_class_set, port=%d, \n", port);
        return rv;
    }
	
    bcm_port_tpid_class_t_init(&port_tpid_class);
    sal_memset(&port_tpid_class, 0, sizeof (port_tpid_class));

    port_tpid_class.port  = port;
    port_tpid_class.tpid1 = BCM_PORT_TPID_CLASS_TPID_INVALID;
    port_tpid_class.tpid2 = BCM_PORT_TPID_CLASS_TPID_ANY;
    port_tpid_class.tag_format_class_id = 1;  /* Set tag format to '1' for all TPID types with one TPID  */
    rv = bcm_port_tpid_class_set(unit, &port_tpid_class);
    if (rv != BCM_E_NONE) {
        printf("Error, in bcm_port_tpid_class_set, port=%d, \n", port);
        return rv;
    }


    bcm_port_tpid_class_t_init(&port_tpid_class);
    sal_memset(&port_tpid_class, 0, sizeof (port_tpid_class));

    port_tpid_class.port  = port;
    port_tpid_class.tpid1 = BCM_PORT_TPID_CLASS_TPID_INVALID;
    port_tpid_class.tpid2 = BCM_PORT_TPID_CLASS_TPID_INVALID;
    port_tpid_class.tag_format_class_id = 0;
    rv = bcm_port_tpid_class_set(unit, &port_tpid_class);
    if (rv != BCM_E_NONE) {
        printf("Error, in bcm_port_tpid_class_set, port=%d, \n", port);
        return rv;
    }

    return rv;
}



/* Set port TPID */
int port__default_tpid__set(int unit, bcm_port_t port){
    int rv;

    port_tpid_init(port, 1, 1);
    rv = port_tpid_set(unit);
    if (rv != BCM_E_NONE) {
        printf("Error, port_tpid_set with port %d\n", port);
        print rv;
    }
    return rv;
}


/* Reset the TPIDs for a port
 * Deletes any previously defined TPIDs for the port and sets the user supplied TPIDs.
 *
 * INPUT: 
 *   port: Physical port or a PWE gport
 *   outer_tpid: Outer TPID value
 *   inner_tpid: Inner TPID value
 */
int port__tpids__set(int unit,
                     bcm_port_t port,
                     uint32 outer_tpid,
                     uint32 inner_tpid)
{
    int rv;
     
    /* Remove possible old TPIDs from the port */
    rv = bcm_port_tpid_delete_all(unit, port);
    if (rv != BCM_E_NONE) {
        printf("Error, in bcm_port_tpid_delete_all, port - %d, rv - %d\n", port, rv);
        return rv;
    }
    
    /* Set the outer TPID of the port */
    rv = bcm_port_tpid_set(unit, port, outer_tpid);
    if (rv != BCM_E_NONE) {
        printf("Error, in bcm_port_tpid_set, port - %d, outer_tpid - %d, rv - %d\n", port, outer_tpid, rv);
        return rv;
    }
    
    /* Set the inner TPID of the port */
    rv = bcm_port_inner_tpid_set(unit, port, inner_tpid);
    if (rv != BCM_E_NONE) {
        printf("Error, in bcm_port_tpid_set, port - %d, inner_tpid - %d, rv - %d\n", port, inner_tpid, rv);
        return rv;
    }

    return BCM_E_NONE;
}


/* Classify a Ports combination of TPIDs to a specific tag format.
 * Applicable only in AVT mode. 
 *  
 * INPUT: 
 *   port: Physical port or a PWE gport
 *   tag_format: Tag format.
 *   outer_tpid: Outer TPID value
 *   inner_tpid: Inner TPID value
 */
int port__tag_classification__set(int unit,
                                  bcm_port_t port,
                                  bcm_port_tag_format_class_t tag_format,
                                  uint32 outer_tpid,
                                  uint32 inner_tpid)
{
    int rv;
    bcm_port_tpid_class_t tpid_class;

    bcm_port_tpid_class_t_init(&tpid_class);

    tpid_class.flags = 0;   /* Both for Ingress and Egress */
    tpid_class.port = port;
    tpid_class.tag_format_class_id = tag_format;
    tpid_class.tpid1 = outer_tpid;
    tpid_class.tpid2 = inner_tpid;
    rv = bcm_port_tpid_class_set(unit, &tpid_class);
    if (rv != BCM_E_NONE) {
        printf("Error, in bcm_port_tpid_class_set, port - %d, rv - %d\n", port, rv);
        return rv;
    }

    return BCM_E_NONE;
}


int cint_ire_nif_shaper_drops_set(int unit)
{
    int rv = BCM_E_NONE;
    int is_jer = 0, is_qmx = 0, is_qax = 0;
    bcm_info_t info;

    rv = bcm_info_get(unit, &info);
    if (rv != BCM_E_NONE) {
        printf("Failed(%d) bcm_info_get\n", rv);
        return rv;
    }

    if(info.device == 0x8675 || info.device == 0x8680) {
        is_jer = 1;
    }

    if(info.device == 0x8375) {
        is_qmx = 1;
    }

    if(info.device == 0x8470) {
        is_qax = 1;
    }

    /* artificially create pressure in the NIF buffers */
    if (is_jer || is_qmx) {
        bshell(unit, "m IRE_NETWORK_INTERFACE_SHAPER NIF_N_SHAPER_TIMER_CYCLES=1 NIF_N_SHAPER_GRANT_SIZE=2 NIF_N_SHAPER_MAX_BURST=0x101");
    }
    else {
        bshell(unit, "m IRE_NETWORK_INTERFACE_SHAPER NIF_SHAPER_TIMER_CYCLES=1 NIF_SHAPER_GRANT_SIZE=2 NIF_SHAPER_MAX_BURST=0x101");
    }

    return BCM_E_NONE;
}

int cint_nif_priority_config(int unit, int in_port,
                             int lp_pcp, int low_priority,
                             int hp_pcp, int high_priority, uint32 tpid, uint32 header)
{

    int rv = BCM_E_NONE;
    uint32 flags = 0;
    int lp_key, hp_key;
    bcm_cosq_ingress_port_drop_map_t cosq_map;
    bcm_info_t info;

    rv = bcm_info_get(unit, &info);
    if (rv != BCM_E_NONE) {
        printf("Failed(%d) bcm_info_get\n", rv);
        return rv;
    }

    /* Configure the NIF ports priority drop*/

    /* Header type legend
    0 - VLAN
    1 - ITMH
    2 - HIGIG
    3 - IP
    4 - MPLS
    */

    /*Depending on the header we chose also and the map type
    bcmCosqIngressPortDropTmTcDpPriorityTable = 0,
    bcmCosqIngressPortDropIpDscpToPriorityTable = 1,
    bcmCosqIngressPortDropEthPcpDeiToPriorityTable = 2,
    bcmCosqIngressPortDropMplsExpToPriorityTable = 3
    }*/

    /* Choose the right key constructor macro for the specified header type and the right COSQ table*/
    if (header == 0) {
        lp_key = BCM_COSQ_INGRESS_PORT_DROP_ETH_MAP_KEY(lp_pcp,0);
        hp_key = BCM_COSQ_INGRESS_PORT_DROP_ETH_MAP_KEY(hp_pcp,0);
        cosq_map = bcmCosqIngressPortDropEthPcpDeiToPriorityTable;
    } else if (header == 1 || header == 2) {
        lp_key = BCM_COSQ_INGRESS_PORT_DROP_TM_MAP_KEY(lp_pcp,0);
        hp_key = BCM_COSQ_INGRESS_PORT_DROP_TM_MAP_KEY(hp_pcp,0);
        cosq_map = bcmCosqIngressPortDropTmTcDpPriorityTable;
    } else if (info.device == 0x8680 && header == 3) {
        /*Check if device is Jericho Plus and header type IP*/
        lp_key = BCM_COSQ_INGRESS_PORT_DROP_IP_MAP_KEY(lp_pcp);
        hp_key = BCM_COSQ_INGRESS_PORT_DROP_IP_MAP_KEY(hp_pcp);
        cosq_map = bcmCosqIngressPortDropIpDscpToPriorityTable;
    } else if (info.device == 0x8680 && header == 4) {
        /*Check if device is Jericho Plus and header type MPLS*/
        lp_key = BCM_COSQ_INGRESS_PORT_DROP_MPLS_MAP_KEY(lp_pcp);
        hp_key = BCM_COSQ_INGRESS_PORT_DROP_MPLS_MAP_KEY(hp_pcp);
        cosq_map = bcmCosqIngressPortDropMplsExpToPriorityTable;
    } else {
        printf("ERROR: Incorrect value for header type %d", header);
        return BCM_E_PARAM;
    }

    /*Map the created lp_key to low priority*/
    rv = bcm_cosq_ingress_port_drop_map_set(unit,in_port,flags,cosq_map,lp_key,low_priority);
    if (rv != BCM_E_NONE) {
        printf("Failed(%d) bcm_cosq_ingress_port_drop_map_set\n", rv);
        return rv;
    }

    /*Map the created hp_key to high priority */
    rv = bcm_cosq_ingress_port_drop_map_set(unit,in_port,flags,cosq_map,hp_key,high_priority);
    if (rv != BCM_E_NONE) {
        printf("Failed(%d) bcm_cosq_ingress_port_drop_map_set\n", rv);
        return rv;
    }

    /*Change the TPID value for the MPLS and IP tests*/
    if (header == 3 || header == 4){
        tpid=0x9100;
    }

    /*Set TPID for the port*/
    rv = bcm_cosq_control_set(unit,in_port,-1,bcmCosqControlIngressPortDropTpid1,tpid);
    if (rv != BCM_E_NONE) {
        printf("Failed(%d) bcm_cosq_control_set\n", rv);
        return rv;
    }

    /*Set 1 byte as a threshold level for the FIFO for packets mapped to low_priority*/
    rv = bcm_cosq_ingress_port_drop_threshold_set(unit,in_port,flags,low_priority,1);
    if (rv != BCM_E_NONE) {
        printf("Failed(%d) bcm_cosq_ingress_port_drop_threshold_set\n", rv);
        return rv;
    }

    /*Return traffic back to the IXIA to get stream statistics*/
    rv = bcm_port_force_forward_set(unit,in_port,in_port,1);
    if (rv != BCM_E_NONE) {
        printf("Failed(%d) bcm_port_force_forward_set\n", rv);
        return rv;
    }

    /*Enable the NIF PRD feature for the given port!
      If device is JER+ enable hard mode!*/
    rv = bcm_cosq_ingress_port_drop_enable_set(unit, in_port, flags, 1);  
    if (rv != BCM_E_NONE) {
        printf("Failed(%d) bcm_cosq_ingress_port_drop_enable_set\n", rv);
        return rv;
    }

    printf("cint_nif_priority_config: PASS\n\n");
    return BCM_E_NONE;

}

int cint_nif_priority_config_soft_stage(int unit, int in_port,
                             int lp_pcp, int low_priority,
                             int hp_pcp, int high_priority, uint32 tpid,
                             uint32 header, uint32 ether_type_val,
                             uint32 lp_match_key, uint32 hp_match_key, uint32 offset)
{

    int rv = BCM_E_NONE;
    uint32 flags = 0;
    int lp_key, hp_key;
    bcm_cosq_ingress_port_drop_map_t cosq_map;
    bcm_info_t info;

    rv = bcm_info_get(unit, &info);
    if (rv != BCM_E_NONE) {
        printf("Failed(%d) bcm_info_get\n", rv);
        return rv;
    }

    if (!(info.device == 0x8680)) {
        printf("NIF PRD Soft Stage is supported only for Jericho plus! \n", rv);
        return BCM_E_UNIT;
    }

    /* Configure the NIF ports priority drop*/

    /* Header type legend
    0 - VLAN
    1 - ITMH
    2 - HIGIG
    3 - IP
    4 - MPLS
    */ 

    /*Depending on the header we chose also and the map type
    bcmCosqIngressPortDropTmTcDpPriorityTable = 0,
    bcmCosqIngressPortDropIpDscpToPriorityTable = 1,
    bcmCosqIngressPortDropEthPcpDeiToPriorityTable = 2,
    bcmCosqIngressPortDropMplsExpToPriorityTable = 3
    }*/

    /* Choose the right key constructor macro for the specified header type and the right COSQ table*/
    if (header == 0) {
        lp_key = BCM_COSQ_INGRESS_PORT_DROP_ETH_MAP_KEY(lp_pcp,0);
        hp_key = BCM_COSQ_INGRESS_PORT_DROP_ETH_MAP_KEY(hp_pcp,0);
        cosq_map = bcmCosqIngressPortDropEthPcpDeiToPriorityTable;
    } else if (header == 1 || header == 2) {
        lp_key = BCM_COSQ_INGRESS_PORT_DROP_TM_MAP_KEY(lp_pcp,0);
        hp_key = BCM_COSQ_INGRESS_PORT_DROP_TM_MAP_KEY(hp_pcp,0);
        cosq_map = bcmCosqIngressPortDropTmTcDpPriorityTable;
    } else if (info.device == 0x8680 && header == 3) {
        /*Check if device is Jericho Plus and header type IP*/
        lp_key = BCM_COSQ_INGRESS_PORT_DROP_IP_MAP_KEY(lp_pcp);
        hp_key = BCM_COSQ_INGRESS_PORT_DROP_IP_MAP_KEY(hp_pcp);
        cosq_map = bcmCosqIngressPortDropIpDscpToPriorityTable;
    } else if (info.device == 0x8680 && header == 4) {
        /*Check if device is Jericho Plus and header type MPLS*/
        lp_key = BCM_COSQ_INGRESS_PORT_DROP_MPLS_MAP_KEY(lp_pcp);
        hp_key = BCM_COSQ_INGRESS_PORT_DROP_MPLS_MAP_KEY(hp_pcp);
        cosq_map = bcmCosqIngressPortDropMplsExpToPriorityTable;
    } else {
        printf("ERROR: Incorrect value for header type %d", header);
        return BCM_E_PARAM;
    }

    /*Map the created lp_key to low priority*/
    rv = bcm_cosq_ingress_port_drop_map_set(unit,in_port,flags,cosq_map,lp_key,low_priority);
    if (rv != BCM_E_NONE) {
        printf("Failed(%d) bcm_cosq_ingress_port_drop_map_set\n", rv);
        return rv;
    }

    /*Map the created hp_key to high priority */
    rv = bcm_cosq_ingress_port_drop_map_set(unit,in_port,flags,cosq_map,hp_key,high_priority);
    if (rv != BCM_E_NONE) {
        printf("Failed(%d) bcm_cosq_ingress_port_drop_map_set\n", rv);
        return rv;
    }

    /*Set TPID for the port*/
    rv = bcm_cosq_control_set(unit,in_port,-1,bcmCosqControlIngressPortDropTpid1,tpid);
    if (rv != BCM_E_NONE) {
        printf("Failed(%d) bcm_cosq_control_set\n", rv);
        return rv;
    }

    /*Set 1 byte as a threshold level for the FIFO for packets mapped to low_priority*/
    rv = bcm_cosq_ingress_port_drop_threshold_set(unit,in_port,flags,low_priority,1);
    if (rv != BCM_E_NONE) {
        printf("Failed(%d) bcm_cosq_ingress_port_drop_threshold_set\n", rv);
        return rv;
    }

      /********************************/
     /**  Start Soft Stage override **/
    /********************************/

    /* Configure ConfigurableEthType1(code 2) for the given port with value 0x1234 */
    rv = bcm_cosq_ingress_port_drop_custom_ether_type_set ( unit, in_port, flags,  2, ether_type_val);
    if (rv != BCM_E_NONE) {
        printf("Failed(%d) bcm_cosq_ingress_port_drop_custom_ether_type_set \n", rv);
        return rv;
    }

    /* Set the rules for constructing a key on this port */
    bcm_cosq_ingress_drop_flex_key_construct_id_t key_id;
    key_id.port = in_port;

    /* Construct the key of from the given offset value*/
    bcm_cosq_ingress_drop_flex_key_construct_t flex_key_config;
    flex_key_config.array_size = 4;
    flex_key_config.offset_array[0] = offset;
    flex_key_config.offset_array[1] = offset-1;
    flex_key_config.offset_array[2] = offset+1;
    flex_key_config.offset_array[3] = offset+2;

    rv = bcm_cosq_ingress_port_drop_flex_key_construct_set ( unit, key_id, flags, flex_key_config);
    if (rv != BCM_E_NONE) {
        printf("Failed(%d) bcm_cosq_ingress_port_drop_flex_key_construct_set \n", rv);
        return rv;
    }

    /* Override the priority of the two streams*/
    bcm_cosq_ingress_drop_flex_key_t ether_code;
    ether_code.value = 2;
    ether_code.mask = 0x1;
    /* Check for match only the first part of the key - flex_key_config.offset_array[0].
       The other offset bytes are around the one that is checked so this
       reasuers us that if we have a match we checked the correct byte */
    int num_key_fields = 1;

    /* Add low prio key to the TCAM */
    /* If match - set to priority 2 */
    bcm_cosq_ingress_drop_flex_key_entry_t lp_flex_key_info;

    bcm_cosq_ingress_drop_flex_key_t lp_key_field_match;
    lp_key_field_match.value = lp_match_key;
    lp_key_field_match.mask = 0xFF;

    lp_flex_key_info.ether_code = ether_code;
    lp_flex_key_info.num_key_fields = num_key_fields;
    lp_flex_key_info.priority = high_priority;
    lp_flex_key_info.key_fields[0] = lp_key_field_match;

    uint32 lp_key_index = 10;

    rv = bcm_cosq_ingress_port_drop_flex_key_entry_set( unit, in_port, flags, lp_key_index, lp_flex_key_info);
    if (rv != BCM_E_NONE) {
        printf("Failed(%d) bcm_cosq_ingress_port_drop_flex_key_entry_set\n", rv);
        return rv;
    }

    /* Add high prio key to the TCAM */
    /* If match - set to priority 0 */
    bcm_cosq_ingress_drop_flex_key_entry_t hp_flex_key_info;

    bcm_cosq_ingress_drop_flex_key_t hp_key_field_match;
    hp_key_field_match.value = hp_match_key;
    hp_key_field_match.mask = 0xFF;

    hp_flex_key_info.ether_code = ether_code;
    hp_flex_key_info.num_key_fields = num_key_fields;
    hp_flex_key_info.priority = low_priority;

    hp_flex_key_info.key_fields[0] = hp_key_field_match;

    uint32 hp_key_index = 20;
    rv = bcm_cosq_ingress_port_drop_flex_key_entry_set( unit, in_port, flags, hp_key_index, hp_flex_key_info);
    if (rv != BCM_E_NONE) {
        printf("Failed(%d) bcm_cosq_ingress_port_drop_flex_key_entry_set\n", rv);
        return rv;
    }

    /*Return traffic back to the IXIA to get stream statistics*/
    rv = bcm_port_force_forward_set(unit,in_port,in_port,1);
    if (rv != BCM_E_NONE) {
        printf("Failed(%d) bcm_port_force_forward_set\n", rv);
        return rv;
    }

    /*Enable the NIF PRD soft stage feature for the given port!*/
    rv = bcm_cosq_ingress_port_drop_enable_set(unit, in_port, flags, 2);
    if (rv != BCM_E_NONE) {
        printf("Failed(%d) bcm_cosq_ingress_port_drop_enable_set\n", rv);
        return rv;
    }

    printf("cint_nif_priority_config_soft_stage: PASS\n\n");
    return BCM_E_NONE;

}

int cint_nif_priority_config_for_ilkn(int unit, int in_port, int ilkn_port,
                                      bcm_mac_t mac_address, int vid,
                                      int lp_pcp, int low_priority,
                                      int hp_pcp, int high_priority, uint32 tpid, uint32 header, int is_both_tpid_set_equal_pkt_tpid)
{

    int rv = BCM_E_NONE;
    uint32 flags = 0;
    int lp_key, hp_key, untagged_pcp = 6;
    bcm_cosq_ingress_port_drop_map_t cosq_map;
    bcm_info_t info;
    bcm_gport_t in_sys_port;
    bcm_gport_t ilkn_sys_port;
    bcm_port_nif_prio_t nif_priority;
    bcm_pbmp_t affected_ports;

    rv = bcm_info_get(unit, &info);
    if (rv != BCM_E_NONE) {
        printf("Failed(%d) bcm_info_get\n", rv);
        return rv;
    }

    if (!((info.device == 0x8680) || (info.device == 0x8470))) {
	printf("NIF PRD for ILKN is supported only for Jericho plus and QAX! \n", rv);
	return BCM_E_UNIT;
    }

    /* Configure the NIF ports priority drop*/

    /* Header type legend
    0 - VLAN
    1 - ITMH
    2 - HIGIG
    3 - IP
    4 - MPLS
    */

    /*Depending on the header we chose also and the map type
    bcmCosqIngressPortDropTmTcDpPriorityTable = 0,
    bcmCosqIngressPortDropIpDscpToPriorityTable = 1,
    bcmCosqIngressPortDropEthPcpDeiToPriorityTable = 2,
    bcmCosqIngressPortDropMplsExpToPriorityTable = 3
    }*/

    /* Choose the right key constructor macro for the specified header type and the right COSQ table*/
    if (header == 0) {
        lp_key = BCM_COSQ_INGRESS_PORT_DROP_ETH_MAP_KEY(lp_pcp,0);
	if (is_both_tpid_set_equal_pkt_tpid) {
	    hp_key = BCM_COSQ_INGRESS_PORT_DROP_ETH_MAP_KEY(hp_pcp,0);
	} else {
	    rv = bcm_cosq_control_set(unit,in_port,-1,bcmCosqControlIngressPortDropUntaggedPCP,untagged_pcp);
	    if (rv != BCM_E_NONE) {
                printf("Failed(%d) bcm_cosq_control_set\n", rv);
                return rv;
    	    }

	    rv = bcm_cosq_control_set(unit,ilkn_port,-1,bcmCosqControlIngressPortDropUntaggedPCP,untagged_pcp);
	    if (rv != BCM_E_NONE) {
                printf("Failed(%d) bcm_cosq_control_set\n", rv);
                return rv;
    	    }
	} 
        cosq_map = bcmCosqIngressPortDropEthPcpDeiToPriorityTable;
    } else if (header == 1 || header == 2) {
        lp_key = BCM_COSQ_INGRESS_PORT_DROP_TM_MAP_KEY(lp_pcp,0);
        hp_key = BCM_COSQ_INGRESS_PORT_DROP_TM_MAP_KEY(hp_pcp,0);
        cosq_map = bcmCosqIngressPortDropTmTcDpPriorityTable;
    } else if (info.device == 0x8680 && header == 3) {
        /*Check if device is Jericho Plus and header type IP*/
        lp_key = BCM_COSQ_INGRESS_PORT_DROP_IP_MAP_KEY(lp_pcp);
        hp_key = BCM_COSQ_INGRESS_PORT_DROP_IP_MAP_KEY(hp_pcp);
        cosq_map = bcmCosqIngressPortDropIpDscpToPriorityTable;
    } else if (info.device == 0x8680 && header == 4) {
        /*Check if device is Jericho Plus and header type MPLS*/
        lp_key = BCM_COSQ_INGRESS_PORT_DROP_MPLS_MAP_KEY(lp_pcp);
        hp_key = BCM_COSQ_INGRESS_PORT_DROP_MPLS_MAP_KEY(hp_pcp);
        cosq_map = bcmCosqIngressPortDropMplsExpToPriorityTable;
    } else {
        printf("ERROR: Incorrect value for header type %d", header);
        return BCM_E_PARAM;
    }

    /*Map the created lp_key to low priority*/
    rv = bcm_cosq_ingress_port_drop_map_set(unit,ilkn_port,flags,cosq_map,lp_key,low_priority);
    if (rv != BCM_E_NONE) {
        printf("Failed(%d) bcm_cosq_ingress_port_drop_map_set\n", rv);
        return rv;
    }

    /*Map the created hp_key to high priority*/
    if (is_both_tpid_set_equal_pkt_tpid == 0 && header == 0) {
        rv = bcm_cosq_ingress_port_drop_map_set(unit,ilkn_port,flags,cosq_map,untagged_pcp|0x1000000,high_priority);
        if (rv != BCM_E_NONE) {
            printf("Failed(%d) bcm_cosq_ingress_port_drop_map_set\n", rv);
            return rv;
        }
    } else {
        rv = bcm_cosq_ingress_port_drop_map_set(unit,ilkn_port,flags,cosq_map,hp_key,high_priority);
        if (rv != BCM_E_NONE) {
            printf("Failed(%d) bcm_cosq_ingress_port_drop_map_set\n", rv);
            return rv;
        }
    }
 
    /*Set TPID for the ports*/
    rv = bcm_cosq_control_set(unit,in_port,-1,bcmCosqControlIngressPortDropTpid1,tpid);
    if (rv != BCM_E_NONE) {
        printf("Failed(%d) bcm_cosq_control_set\n", rv);
        return rv;
    }

    rv = bcm_cosq_control_set(unit,ilkn_port,-1,bcmCosqControlIngressPortDropTpid1,tpid);
    if (rv != BCM_E_NONE) {
        printf("Failed(%d) bcm_cosq_control_set\n", rv);
        return rv;
    }

    /*Set 1 byte as a threshold level for the FIFO for packets mapped to low_priority*/
    rv = bcm_cosq_ingress_port_drop_threshold_set(unit,ilkn_port,flags,low_priority,1);
    if (rv != BCM_E_NONE) {
        printf("Failed(%d) bcm_cosq_ingress_port_drop_threshold_set\n", rv);
        return rv;
    }
    /* Set the NIF priority of the IN_PORT to be higher than that of the ILKN_PORT*/
    /* In order to not receive drops at least until we reach tx of the ILNK_PORT*/
    nif_priority.priority_level=1;
    rv =  bcm_port_nif_priority_set( unit, in_port,  flags, nif_priority, affected_ports);
    if (rv != BCM_E_NONE) {
        printf("Failed(%d) bcm_cosq_ingress_port_drop_threshold_set\n", rv);
        return rv;
    }
    nif_priority.priority_level=0;
    rv =  bcm_port_nif_priority_set( unit, ilkn_port,  flags, nif_priority, affected_ports);
    if (rv != BCM_E_NONE) {
        printf("Failed(%d) bcm_cosq_ingress_port_drop_threshold_set\n", rv);
        return rv;
    }

       /*****************************************/
      /**  Configure snake from the IXIA port **/
     /***   to the ILKN port and back again ***/
    /*****************************************/

    if (is_both_tpid_set_equal_pkt_tpid == 0 && header == 0) {
        rv = bcm_port_force_forward_set(unit,in_port,ilkn_port,1);
        if (rv != BCM_E_NONE) {
            printf("Failed(%d) bcm_port_force_forward_set\n", rv);
            return rv;
        }

        rv = bcm_port_loopback_set(unit,ilkn_port,BCM_PORT_LOOPBACK_PHY);
        if (rv != BCM_E_NONE) {
            printf("Failed(%d) bcm_port_loopback_set\n", rv);
            return rv;
        }

        rv = bcm_port_force_forward_set(unit,ilkn_port,in_port,1);
            if (rv != BCM_E_NONE) {
            printf("Failed(%d) bcm_port_force_forward_set\n", rv);
            return rv;
        }
    } else {
	rv = cint_vlan_nif_snake_test_on_two_ports( unit, mac_address, vid, in_port, ilkn_port);
        if (rv != BCM_E_NONE) {
            printf("Failed(%d) cint_vlan_nif_snake_test_on_two_ports\n", rv);
            return rv;
        }
    }
	
    /*Enable the NIF PRD hard mode for both ports!*/
    rv = bcm_cosq_ingress_port_drop_enable_set(unit, ilkn_port, flags, 1);
    if (rv != BCM_E_NONE) {
        printf("Failed(%d) bcm_cosq_ingress_port_drop_enable_set\n", rv);
        return rv;
    }

    printf("cint_nif_priority_config_for_ilkn: PASS\n\n");
    return BCM_E_NONE;

}

int cint_nif_prd_port_set_all_to_default_priority(int unit,int port,uint32 def_prio,
                                                   uint32 ignore_headers, uint32 tpid_mismatch, uint32 tpid_value)
{

    int rv = BCM_E_NONE;
    uint32 flags = 0;
    bcm_info_t info;

    rv = bcm_info_get(unit, &info);
    if (rv != BCM_E_NONE) {
        printf("Failed(%d) bcm_info_get\n", rv);
        return rv;
    }

    if (!(info.device == 0x8680)) {
        printf("Default priority API is supported only for Jericho plus! \n", rv);
        return BCM_E_UNIT;
    }

    /* Set default priority*/
    rv = bcm_cosq_ingress_port_drop_default_priority_set (unit, port, flags, def_prio);
    if (rv != BCM_E_NONE) {
        printf("Failed(%d) bcm_cosq_ingress_port_drop_default_priority_set\n", rv);
        return rv;
    }

    /* If we use the cint for the TPID mismatch test*/
    /* We change the expected TPID value for the second port*/
    if (tpid_mismatch) {
        rv = bcm_cosq_control_set(unit,port,-1,bcmCosqControlIngressPortDropTpid1,tpid_value);
        if (rv != BCM_E_NONE) {
            printf("Failed(%d) bcm_cosq_control_set\n", rv);
            return rv;
        }
    }

    if (ignore_headers) {

          /**Ignore all header information **/
         /** in order to receive  default **/
        /**    priority in second_port   **/

        /* bcm_cosq_control_t{
        bcmCosqControlIngressPortDropIgnoreIpDscp = 143, cosq ingress port drop ignore IP DSCP.
        bcmCosqControlIngressPortDropIgnoreMplsExp = 144,  cosq ingress port drop ignore MPLS EXP. 
        bcmCosqControlIngressPortDropIgnoreInnerTag = 145, cosq ingress port drop ignore inner VLAN tag.
        bcmCosqControlIngressPortDropIgonreOuterTag = 146,  cosq ingress port drop ignore outer
            } */

        rv = bcm_cosq_control_set (unit, port, -1, bcmCosqControlIngressPortDropIgnoreIpDscp, 1);
        if (rv != BCM_E_NONE) {
            printf("Failed(%d) bcm_cosq_control_set\n", rv);
            return rv;
        }

        rv = bcm_cosq_control_set (unit, port, -1, bcmCosqControlIngressPortDropIgnoreMplsExp, 1);
        if (rv != BCM_E_NONE) {
            printf("Failed(%d) bcm_cosq_control_set\n", rv);
            return rv;
        }

        rv = bcm_cosq_control_set (unit, port, -1, bcmCosqControlIngressPortDropIgnoreInnerTag, 1);
        if (rv != BCM_E_NONE) {
            printf("Failed(%d) bcm_cosq_control_set\n", rv);
            return rv;
        }

        rv = bcm_cosq_control_set (unit, port, -1, bcmCosqControlIngressPortDropIgonreOuterTag, 1);
        if (rv != BCM_E_NONE) {
            printf("Failed(%d) bcm_cosq_control_set\n", rv);
            return rv;
        }
    }

    printf("cint_nif_prd_port_set_all_to_default_priority: PASS\n\n");
    return BCM_E_NONE;

}

uint64 cint_port_dropped_packets_get (int unit, int port) {
    int rv = BCM_E_NONE;
    uint64 dropped_cnt;

    rv = bcm_stat_get(unit,port,snmpEtherStatsDropEvents,&dropped_cnt);
    if (rv != BCM_E_NONE) {
        printf("Failed(%d) bcm_stat_get \n", rv);
        return rv;
    }

    return dropped_cnt;
}


