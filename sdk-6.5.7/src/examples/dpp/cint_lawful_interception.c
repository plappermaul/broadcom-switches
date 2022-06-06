/* $Id: cint_vxlan.c,v 1.10 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
*/
/*
 * Test Scenario: flooding, UC packets
 *
 * soc properties:
 * lawful_interception_enable=1
 * field_presel_mgmt_advanced_mode=1
 *
 * RCY port:
 * ucode_port_166.BCM88650=RCY.0:core_0.166
 *
 * how to run the test: 
 * cd ../../../../src/examples/dpp
 * cint utility/cint_utils_global.c
 * cint utility/cint_utils_l2.c
 * cint utility/cint_utils_l3.c
 * cint cint_port_tpid.c
 * cint cint_ip_tunnel.c
 * cint cint_lawful_interception.c
 * cint pon/cint_pon_utils.c
 * cint pon/cint_pon_application.c
 * c
 * lawful_interception_example_dvapi(0, 2, 128, 166, 100, 0xAABBCCDD, 0x5555, 0x5000, 0x1010101, strip_l2);
 * pon_service_init(0, 2, 128);
 * pon_n_1_service(0);
 * 
 * Tests:
 * 1. Mirror full packet including ETH as payload:
 *     strip_l2 = 0
 *
 *     Packet sent:
 * 
 *     Expected RX:
 *
 * 2. Mirror IP payload only into lawful interception tunnel:
 *     strip_l2 = 1
 *
 *     Packet sent:
 * 
 *     Expected RX:
 *
 * Mirror destination ports can ONLY be NNI ports, PON ports could not be the mirror destination.
 */
struct lawful_interception_info_s{
    int pon_port;
    int nni_port;
    int rcy_port;
    int out_vlan;
    ip_tunnel_s ip_tunnel;
    uint32 content_id;
    uint16 udp_dst_port;
    uint16 udp_src_port;

    bcm_gport_t tunnel_id;
    bcm_gport_t mirror_dest_id;
    bcm_l3_intf_t vxlan_if;
    bcm_l3_intf_t lawful_interception_if;
    bcm_if_t next_hop_if;

    bcm_field_presel_t pon_presel_id;
    bcm_field_presel_t rcy_presel_id;

    bcm_field_group_t filter_group_id;
    bcm_field_presel_set_t filter_group_psset;
    int filter_group_prio;
    bcm_field_entry_t filter_group_entry;
    uint32 dip_to_capture;

    bcm_field_group_t rcy_ext_group_id;
    bcm_field_presel_set_t rcy_ext_group_psset;
    int rcy_ext_group_prio;
    bcm_field_entry_t rcy_ext_group_entry;

    int strip_l2;
};

lawful_interception_info_s lawful_interception_info;

int lawful_interception_info_init(int pon_port, int nni_port, int rcy_port, int out_vlan, uint32 content_id, uint16 udp_dst_port, uint16 udp_src_port, uint32 dip, int strip_l2) {
    bcm_mac_t next_hop_mac = {0x00, 0x00, 0x00, 0x00, 0xcd, 0x1d};
    bcm_mac_t my_mac = ip_tunnel_my_mac_get();
    
    lawful_interception_info.pon_port = pon_port;
    lawful_interception_info.nni_port = nni_port;
    lawful_interception_info.rcy_port = rcy_port;
    lawful_interception_info.out_vlan = out_vlan;

    lawful_interception_info.ip_tunnel.da = next_hop_mac;
    lawful_interception_info.ip_tunnel.sa = my_mac;
    lawful_interception_info.ip_tunnel.sip = 0xA0000011;
    lawful_interception_info.ip_tunnel.dip = 0xA1000011;
    lawful_interception_info.ip_tunnel.ttl = 60;
    lawful_interception_info.ip_tunnel.dscp = 10;
    lawful_interception_info.ip_tunnel.dscp_sel = bcmTunnelDscpAssign;    
        
    lawful_interception_info.content_id = content_id;
    lawful_interception_info.udp_dst_port = udp_dst_port;
    lawful_interception_info.udp_src_port = udp_src_port;

    lawful_interception_info.filter_group_prio = BCM_FIELD_GROUP_PRIO_ANY;
    lawful_interception_info.rcy_ext_group_prio = BCM_FIELD_GROUP_PRIO_ANY;
    
    lawful_interception_info.dip_to_capture = dip;
    lawful_interception_info.strip_l2 = strip_l2;
}

int lawful_interception_port_preselector_create(int unit, int port, int presel_id)
{
    bcm_pbmp_t pbm;
    bcm_pbmp_t pbm_mask;
    int core;
    int index;
    int rv;
    int presel_flags = 0;
          
    /* Cretae a presel entity */
    if(soc_property_get(unit, spn_FIELD_PRESEL_MGMT_ADVANCED_MODE, FALSE)) {
        presel_flags = BCM_FIELD_QUALIFY_PRESEL_ADVANCED_MODE_STAGE_INGRESS;
        rv = bcm_field_presel_create_stage_id(unit, bcmFieldStageIngress, presel_id);
        if (BCM_E_NONE != rv) {
            printf("Error in bcm_field_presel_create_stage_id\n");
            return rv;
        }

    } else {
        rv = bcm_field_presel_create_id(unit, presel_id);
        if (BCM_E_NONE != rv) {
            printf("Error in bcm_field_presel_create_id\n");
            return rv;
        }
    }
    
    /* Define the set of in/out ports */
    BCM_PBMP_CLEAR(pbm);
    BCM_PBMP_PORT_ADD(pbm, port);
      
    for(index=0; index<512; index++) { 
        BCM_PBMP_PORT_ADD(pbm_mask, index); 
    }

    rv = bcm_field_qualify_Stage(unit, presel_id | BCM_FIELD_QUALIFY_PRESEL | presel_flags, bcmFieldStageIngress);
    if (BCM_E_NONE != rv) {
        printf("Error in bcm_field_qualify_Stage\n");
        return rv;
    }

    rv = bcm_field_qualify_InPorts(unit, presel_id | BCM_FIELD_QUALIFY_PRESEL | presel_flags, pbm, pbm_mask);
    if (BCM_E_NONE != rv) {
        printf("Error in bcm_field_qualify_InPorts\n");
        return rv;
    }

    return rv;
}

int lawful_interception_pmf_init(int unit, int presel_id_start)
{
    lawful_interception_info.pon_presel_id = presel_id_start++;
    BCM_IF_ERROR_RETURN(lawful_interception_port_preselector_create(unit, lawful_interception_info.pon_port, lawful_interception_info.pon_presel_id));
    lawful_interception_info.rcy_presel_id = presel_id_start++;
    BCM_IF_ERROR_RETURN(lawful_interception_port_preselector_create(unit, lawful_interception_info.rcy_port, lawful_interception_info.rcy_presel_id));

    BCM_FIELD_PRESEL_INIT(lawful_interception_info.filter_group_psset);
    BCM_FIELD_PRESEL_ADD(lawful_interception_info.filter_group_psset, lawful_interception_info.pon_presel_id);      
    BCM_FIELD_PRESEL_INIT(lawful_interception_info.rcy_ext_group_psset);
    BCM_FIELD_PRESEL_ADD(lawful_interception_info.rcy_ext_group_psset, lawful_interception_info.rcy_presel_id);      

    return 0;
}

int lawful_interception_tunnel_add(int unit) {
    create_l3_intf_s intf;
    bcm_l3_egress_t l3eg;
    bcm_if_t l3egid;
    bcm_tunnel_initiator_t vxlan;
    bcm_tunnel_initiator_t lawful_interception;
    bcm_l3_intf_t vxlan_intf;
    bcm_l3_intf_t lawful_interception_intf;
    ip_tunnel_s ip_tunnel = lawful_interception_info.ip_tunnel;
    int rv;    

    intf.vsi = lawful_interception_info.out_vlan;
    intf.my_global_mac = ip_tunnel.sa;
    intf.my_lsb_mac = ip_tunnel.sa;
    rv = l3__intf_rif__create(unit, &intf);
    if (rv != BCM_E_NONE) {
        printf("Error, l3__intf_rif__create\n");
        return rv;
    }

    bcm_l3_egress_t_init(&l3eg);
    l3eg.flags = BCM_L3_EGRESS_ONLY;
    l3eg.port = lawful_interception_info.nni_port;  
    l3eg.mac_addr = ip_tunnel.da;
    l3eg.vlan = lawful_interception_info.out_vlan;  
    rv = bcm_l3_egress_create(unit, 0, &l3eg, &l3egid);
    if (rv != BCM_E_NONE) {
        printf("Error, bcm_l3_egress_create\n");
    }

    bcm_tunnel_initiator_t_init(&vxlan);
    vxlan.dip = ip_tunnel.dip;
    vxlan.sip = ip_tunnel.sip;
    vxlan.dscp = ip_tunnel.dscp; 
    vxlan.flags = 0;
    vxlan.ttl = ip_tunnel.ttl;
    vxlan.type = bcmTunnelTypeVxlan;
    vxlan.vlan = lawful_interception_info.out_vlan;
    vxlan.dscp_sel = ip_tunnel.dscp_sel;
    vxlan.outlif_counting_profile = BCM_STAT_LIF_COUNTING_PROFILE_NONE;
    vxlan.l3_intf_id = l3eg.encap_id;
    bcm_l3_intf_t_init(&vxlan_intf);
    rv = bcm_tunnel_initiator_create(unit, &vxlan_intf, &vxlan);
    if (rv != BCM_E_NONE) {
        printf("Error, bcm_tunnel_initiator_create\n");
    }

    bcm_tunnel_initiator_t_init(&lawful_interception);
    lawful_interception.type = bcmTunnelTypeLawfulInterception;
    lawful_interception.aux_data = lawful_interception_info.content_id;
    lawful_interception.udp_dst_port = lawful_interception_info.udp_dst_port;
    lawful_interception.l3_intf_id = vxlan_intf.l3a_intf_id;
    bcm_l3_intf_t_init(&lawful_interception_intf);
    rv = bcm_tunnel_initiator_create(unit, &lawful_interception_intf, &lawful_interception);
    if (rv != BCM_E_NONE) {
        printf("Error, bcm_tunnel_initiator_create\n");
    }

    lawful_interception_info.tunnel_id = lawful_interception.tunnel_id;
    lawful_interception_info.vxlan_if = vxlan_intf;
    lawful_interception_info.lawful_interception_if = lawful_interception_intf;
    lawful_interception_info.next_hop_if = l3egid;

    return BCM_E_NONE;
}

int lawful_interception_mirror_dest_create(int unit, int strip_l2) {
    bcm_gport_t mirror_gport_id;
    bcm_mirror_destination_t mirror_dest;
    int rv = 0;

    bcm_mirror_destination_t_init(&mirror_dest);

    if (strip_l2) {
        mirror_dest.gport = lawful_interception_info.rcy_port;
        rv = bcm_mirror_destination_create(unit, &mirror_dest);
        if (rv != BCM_E_NONE) {
            printf("Error, bcm_mirror_destination_create\n");
            return rv;
        }    
    }
    else {
        mirror_dest.flags |= BCM_MIRROR_DEST_TUNNEL_WITH_ENCAP_ID;
        mirror_dest.gport = lawful_interception_info.nni_port;
        mirror_dest.encap_id = BCM_GPORT_TUNNEL_ID_GET(lawful_interception_info.tunnel_id);
        rv = bcm_mirror_destination_create(unit, &mirror_dest);
        if (rv != BCM_E_NONE) {
            printf("Error, bcm_mirror_destination_create\n");
            return rv;
        }    
    }

    lawful_interception_info.mirror_dest_id = mirror_dest.mirror_dest_id;

    return rv;
}

int lawful_interception_filter_set(int unit) {
    bcm_field_qset_t  qset;
    bcm_field_aset_t  aset;
    bcm_field_group_config_t grp;
    bcm_field_entry_t ent;
    bcm_gport_t mirror_gport_id;
    bcm_mirror_destination_t mirror_dest;
    int rv = 0;

    bcm_field_group_config_t_init(&grp);
    BCM_FIELD_QSET_INIT(grp.qset);
    BCM_FIELD_QSET_ADD(grp.qset, bcmFieldQualifyStageIngress);
    BCM_FIELD_QSET_ADD(grp.qset, bcmFieldQualifyDstIp);


    grp.priority = lawful_interception_info.filter_group_prio;
    grp.flags = BCM_FIELD_GROUP_CREATE_WITH_PRESELSET;
    grp.preselset = lawful_interception_info.filter_group_psset;

    BCM_IF_ERROR_RETURN(bcm_field_group_config_create(unit, &grp));

    BCM_FIELD_ASET_INIT(aset);
    BCM_FIELD_ASET_ADD(aset, bcmFieldActionMirrorIngress);
    
    BCM_IF_ERROR_RETURN(bcm_field_group_action_set(unit, grp.group, aset));
    
    BCM_IF_ERROR_RETURN(bcm_field_entry_create(unit, grp.group, &ent));
    BCM_IF_ERROR_RETURN(bcm_field_qualify_DstIp(unit, ent, lawful_interception_info.dip_to_capture, 0xFFFFFFFF));
    BCM_IF_ERROR_RETURN(bcm_field_action_add(unit, ent, bcmFieldActionMirrorIngress, 0, lawful_interception_info.mirror_dest_id));
    
    rv = bcm_field_group_install(unit, grp.group);
    if (rv != BCM_E_NONE) {
        printf("Error, bcm_field_group_install\n");
    }    

    lawful_interception_info.filter_group_id = grp.group;
    lawful_interception_info.filter_group_entry = ent;

    return rv;
}

int lawful_interception_rcy_ext_set(int unit) {
    bcm_field_qset_t  qset;
    bcm_field_aset_t  aset;
    bcm_field_group_config_t grp;
    bcm_field_entry_t ent;
    bcm_field_action_core_config_t config;
    bcm_gport_t gport;
    int rv = 0;

    bcm_field_group_config_t_init(&grp);
    BCM_FIELD_QSET_INIT(grp.qset);
    BCM_FIELD_QSET_ADD(grp.qset, bcmFieldQualifyStageIngress);
    BCM_FIELD_QSET_ADD(grp.qset, bcmFieldQualifyDstIp);

    grp.priority = lawful_interception_info.rcy_ext_group_prio;
    grp.flags = BCM_FIELD_GROUP_CREATE_WITH_PRESELSET;
    grp.preselset = lawful_interception_info.rcy_ext_group_psset;

    BCM_IF_ERROR_RETURN(bcm_field_group_config_create(unit, &grp));

    BCM_FIELD_ASET_INIT(aset);
    BCM_FIELD_ASET_ADD(aset, bcmFieldActionForwardingTypeNew);
    BCM_FIELD_ASET_ADD(aset, bcmFieldActionRedirect);
    BCM_FIELD_ASET_ADD(aset, bcmFieldActionVportNew);
    
    BCM_IF_ERROR_RETURN(bcm_field_group_action_set(unit, grp.group, aset));

    BCM_IF_ERROR_RETURN(bcm_field_entry_create(unit, grp.group, &ent));
    BCM_IF_ERROR_RETURN(bcm_field_qualify_DstIp(unit, ent, lawful_interception_info.dip_to_capture, 0xFFFFFFFF));    
	BCM_GPORT_LOCAL_SET(gport, lawful_interception_info.nni_port);
    BCM_IF_ERROR_RETURN(bcm_field_action_add(unit, ent, bcmFieldActionRedirect, 0, gport));
    BCM_IF_ERROR_RETURN(bcm_field_action_add(unit, ent, bcmFieldActionVportNew, lawful_interception_info.tunnel_id, 0));

    config.param0 = bcmFieldForwardingTypeSnoop;
    config.param1 = 2; /* forwarding header index */
    config.param2 = 0; /* forwarding offset in the header */    
    rv = bcm_field_action_config_add(unit, ent, bcmFieldActionForwardingTypeNew, 1, &config);
    if (rv != BCM_E_NONE) {
        printf("Error, bcm_field_action_config_add\n");
    }    
    
    rv = bcm_field_group_install(unit, grp.group);
    if (rv != BCM_E_NONE) {
        printf("Error, bcm_field_group_install\n");
    }    

    lawful_interception_info.rcy_ext_group_id = grp.group;
    lawful_interception_info.rcy_ext_group_entry = ent;

    return rv;
}

int lawful_interception_example_dvapi(int unit, int pon_port, int nni_port, int rcy_port, int out_vlan, uint32 content_id, uint16 udp_dst_port, uint16 udp_src_port, uint32 dip, int strip_l2) {
    uint32 data;
    int rv = 0;
    
    lawful_interception_info_init(pon_port, nni_port, rcy_port, out_vlan, content_id, udp_dst_port, udp_src_port, dip, strip_l2);
    BCM_IF_ERROR_RETURN(lawful_interception_pmf_init(unit, 10));
    BCM_IF_ERROR_RETURN(lawful_interception_tunnel_add(unit));
    BCM_IF_ERROR_RETURN(lawful_interception_mirror_dest_create(unit, strip_l2));

    if (strip_l2) {        
        BCM_IF_ERROR_RETURN(lawful_interception_rcy_ext_set(unit));
    }
    BCM_IF_ERROR_RETURN(lawful_interception_filter_set(unit));
    
    BCM_IF_ERROR_RETURN(bcm_switch_control_set(unit, bcmSwitchLIUdpSrcPortSet, udp_src_port));

    return rv;
}

int lawful_interception_example_dvapi_clean_up(int unit) {
    if (lawful_interception_info.strip_l2) {
        bcm_field_entry_destroy(unit, lawful_interception_info.rcy_ext_group_entry);
        bcm_field_group_destroy(unit, lawful_interception_info.rcy_ext_group_id);
    }
    bcm_field_entry_destroy(unit, lawful_interception_info.filter_group_entry);
    bcm_field_group_destroy(unit, lawful_interception_info.filter_group_id);
    bcm_field_presel_destroy(unit, lawful_interception_info.pon_presel_id);
    bcm_field_presel_destroy(unit, lawful_interception_info.rcy_presel_id);
    bcm_mirror_destination_destroy(unit, lawful_interception_info.mirror_dest_id);
    bcm_tunnel_initiator_clear(unit,lawful_interception_info.lawful_interception_if);
    bcm_tunnel_initiator_clear(unit,lawful_interception_info.vxlan_if);
    bcm_l3_egress_destroy(unit, lawful_interception_info.next_hop_if);

    return 0;
}

