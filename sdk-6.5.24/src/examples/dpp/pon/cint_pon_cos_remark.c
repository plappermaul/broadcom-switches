/*
 * 
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2021 Broadcom Inc. All rights reserved.
 *
 * File: cint_pon_cos_remark.c
 * Purpose: An example how to implement ctc cos remark test cases.
 *          An example how to create traffic class.
 *
 *
 * To Activate Above Settings Run:
 *      BCM> cint examples/dpp/cint_port_tpid.c
 *      BCM> cint examples/dpp/pon/cint_pon_utils.c
 *      BCM> cint examples/dpp/pon/cint_pon_cos_remark.c
 *      BCM> cint
 *      cint> 
 *      cint> print pon_cos_remark_app(unit, pon1, pon2, nni1, nni2);
 * To clean all above configuration:
 *      cint> print pon_cos_remark_app_clean(unit);
 *      cint> 
 *      cint> 
 */

struct pon_cos_remark_info_s {
  bcm_vlan_t  vsi;
  bcm_gport_t pon_gport[5];
  bcm_gport_t nni_gport;
  bcm_port_t pon_port;
  bcm_port_t nni_port;
  bcm_port_t pon_port2;
  bcm_port_t nni_port2;
  int tunnel_id[5];
  int qos_map_id[4];
  int pri[4];
  int nni_svlan;
  int nni_cvlan;
  int pon_cvlan;
  int lif_offset;
  int tc_qos_map_id;
};


pon_cos_remark_info_s pon_cos_remark_info;

/* Map ingress and egress as follows
 * Ingress      INTERNAL        
 * PKT_PRI/CFI  TC/DP        

 *       1/0    0/GREEN         
 *       3/0    0/YELLOW        
 *       5/0    1/GREEN         
 *       7/0    1/RED           
 */

int pon_qos_map_l2_internal_pri[16] = {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7};

bcm_color_t pon_qos_map_l2_internal_color[16] = {bcmColorGreen,
					     bcmColorYellow,
					     bcmColorGreen,
					     bcmColorYellow,
					     bcmColorGreen,
					     bcmColorYellow,
					     bcmColorGreen,
					     bcmColorRed,
					     bcmColorGreen,
					     bcmColorRed,
					     bcmColorGreen,
					     bcmColorRed,
					     bcmColorGreen,
					     bcmColorRed,
					     bcmColorGreen,
					     bcmColorRed};


int pon_cos_remark_init(int unit, bcm_port_t pon_port1, bcm_port_t pon_port2, bcm_port_t nni_port1, bcm_port_t nni_port2)
{
  sal_memset(&pon_cos_remark_info, 0, sizeof(pon_cos_remark_info));
  pon_cos_remark_info.vsi = 0;
  pon_cos_remark_info.pon_port = pon_port1;
  pon_cos_remark_info.nni_port = nni_port1;
  pon_cos_remark_info.tunnel_id[0] = 1000; /* ONU1, remark cos to 1 */
  pon_cos_remark_info.tunnel_id[1] = 1001; /* ONU2, remark cos to 3 */
  pon_cos_remark_info.tunnel_id[2] = 1002; /* ONU3, remark cos to 5*/
  pon_cos_remark_info.tunnel_id[3] = 1003; /* ONU4, remark cos to 7 */
  pon_cos_remark_info.tunnel_id[4] = 2000; /* ONU5, cos cpoy from outer vid to inner vid */
  pon_cos_remark_info.nni_svlan = 1000;
  pon_cos_remark_info.nni_cvlan = 100;
  pon_cos_remark_info.pon_cvlan = 100; 
  pon_cos_remark_info.lif_offset = 0;
  pon_cos_remark_info.pri[0] = 1; /* ONU1 */
  pon_cos_remark_info.pri[1] = 3; /* ONU2 */
  pon_cos_remark_info.pri[2] = 5; /* ONU3 */
  pon_cos_remark_info.pri[3] = 7; /* ONU4 */

  pon_app_init(unit, pon_port1, nni_port1, nni_port2, 0);

  return 0;

}

/******************************************************************************* 
 * Create qos map id
 *     ONU1 -- pri 1
 *     ONU2 -- pri 3
 *     ONU3 -- pri 5
 *     ONU4 -- pri 7
 * or 
 *     pri 0, 1 -- 0
 *     pri 2, 3 -- 1
 *     pri 4, 5 -- 2
 *     pri 6, 7 -- 3
 */
int pon_cos_remark_qos_map_id_create(int unit, bcm_gport_t gport, int idx, int is_tc)
{
  int rv;
  bcm_qos_map_t l2_in_map;
  int flags = 0;
  int i = 0;

  if (is_tc)
  {
    rv = bcm_qos_map_create(unit, BCM_QOS_MAP_INGRESS, &(pon_cos_remark_info.tc_qos_map_id));

    if (rv != BCM_E_NONE) 
    {
        printf("error in ingress bcm_qos_map_create() %d %s \n", idx, bcm_errmsg(rv));
        return rv;
    }
  }
  else
  {
    rv = bcm_qos_map_create(unit, BCM_QOS_MAP_INGRESS|BCM_QOS_MAP_L2_VLAN_PCP, &(pon_cos_remark_info.qos_map_id[idx]));

    if (rv != BCM_E_NONE) 
    {
        printf("error in ingress PCP bcm_qos_map_create() %d %s \n", idx, bcm_errmsg(rv));
        return rv;
    }
  }

  for (i = 0; i < 16; i++)
  {
    bcm_qos_map_t_init(&l2_in_map);

    flags = BCM_QOS_MAP_L2|BCM_QOS_MAP_L2_OUTER_TAG;

    /* Set internal priority for this ingress pri */
    l2_in_map.int_pri = pon_qos_map_l2_internal_pri[i];
    l2_in_map.pkt_cfi = i&1;
    
    /* Set color for this ingress Priority  */
    l2_in_map.color = pon_qos_map_l2_internal_color[i];

    if(is_tc)
    {
      l2_in_map.pkt_pri = (l2_in_map.int_pri >> 1);
      rv = bcm_qos_map_add(unit, flags, &l2_in_map, pon_cos_remark_info.qos_map_id[idx]);
      if (rv != BCM_E_NONE) 
      {
          printf("error in ingress bcm_qos_map_create() %d %s \n", idx, bcm_errmsg(rv));
          return rv;
      }
    }
    else
    {
      l2_in_map.pkt_pri = pon_cos_remark_info.pri[idx];
      rv = bcm_qos_map_add(unit, flags | BCM_QOS_MAP_L2_VLAN_PCP, &l2_in_map, pon_cos_remark_info.qos_map_id[idx]);
      if (rv != BCM_E_NONE) 
      {
          printf("error in ingress PCP bcm_qos_map_create() %d %s \n", idx, bcm_errmsg(rv));
          return rv;
      }
    }
  }

  return rv;

}

/******************************************************************************* 
 * Destory qos map id
 *     ONU1 -- pri 1
 *     ONU2 -- pri 3
 *     ONU3 -- pri 5
 *     ONU4 -- pri 7
 */
int pon_cos_remark_qos_map_id_destroy(int unit, bcm_gport_t gport, int idx, int is_tc)
{
  int rv;
  bcm_qos_map_t l2_in_map;
  int flags = 0;
  int i = 0;


  for (i = 0; i < 16; i++)
  {
    bcm_qos_map_t_init(&l2_in_map);

    l2_in_map.pkt_pri = pon_cos_remark_info.pri[idx];
    /* Set internal priority for this ingress pri */
    l2_in_map.int_pri = pon_qos_map_l2_internal_pri[i];
    l2_in_map.pkt_cfi = i&1;
    /* Set color for this ingress Priority  */
    l2_in_map.color = pon_qos_map_l2_internal_color[i];
    flags = BCM_QOS_MAP_INGRESS | BCM_QOS_MAP_L2_OUTER_TAG | BCM_QOS_MAP_L2;
    if (0 == is_tc)
    {
        flags |= BCM_QOS_MAP_L2_VLAN_PCP;
    }

    rv = bcm_qos_map_delete(unit, flags, &l2_in_map, pon_cos_remark_info.qos_map_id[idx]);
    if (rv != BCM_E_NONE) 
    {
        printf("error in ingress bcm_qos_map_delete() %d %s \n", idx, bcm_errmsg(rv));
        return rv;
    }
  }

  rv = bcm_qos_map_destroy(unit, pon_cos_remark_info.qos_map_id[idx]);

  if (rv != BCM_E_NONE) 
  {
      printf("error in ingress PCP bcm_qos_map_destroy() %d %s \n", idx, bcm_errmsg(rv));
      return rv;
  }

  return rv;

}


/*********************************************************************************
* Setup pon service
*     ONU1, ONu2, ONU3, ONU4 is cos remark, and ONU5 is cos copy
*
*     PON Port 1  <-------------------------------------->  VSI  <---> NNI Port 128
*     Pon 1 Tunnel-ID 1000 cvlan 100 --------------------|         |
*     Pon 1 Tunnel-ID 1001 cvlan 100 --------------------|---4096  |
*     Pon 1 Tunnel-ID 1002 cvlan 100 --------------------|         |----------- svlan 1000 cvlan 100
*     Pon 1 Tunnel-ID 1003 cvlan 100 --------------------|         |
*     Pon 1 Tunnel-ID 2000 cvlan 100 --------------------|         |
*
* Input parameters
*     int unit              - unit number
*     bcm_port_t pon_port_1 - First PON port  
*     bcm_port_t pon_port_1 - Second PON port
*     bcm_port_t nni_port_1 - First NNI port
*     bcm_port_t nni_port_2 - Second NNI port
* steps
 */
int pon_cos_remark_app(int unit, bcm_port_t pon_port_1, bcm_port_t pon_port_2,
                       bcm_port_t nni_port_1, bcm_port_t nni_port_2)
{
  int i = 0;
  int rv = 0;
  bcm_if_t encap_id = 0;
  int action_id = 0;
  /*bcm_gport_t trunk_gport;*/

  pon_cos_remark_init(unit, pon_port_1, pon_port_2, nni_port_1, nni_port_2);

  /*BCM_GPORT_TRUNK_SET(trunk_gport, nni_trunk_id);*/

  rv = vswitch_create(unit, &(pon_cos_remark_info.vsi)/*, pon_cos_remark_info.lif_offset*/);
  if (rv != BCM_E_NONE)
  {
    printf("Error, vswitch_create returned %s\n", bcm_errmsg(rv));
    return rv;
  }

  /* Create PON LIF for cos remark */
  for (i = 0; i < 4; i++)
  {
    rv = pon_lif_create(unit, 
                        pon_cos_remark_info.pon_port, 
                        match_otag,
                        0, 0,
                        pon_cos_remark_info.tunnel_id[i],
                        pon_cos_remark_info.tunnel_id[i],
                        pon_cos_remark_info.pon_cvlan,
                        0, 0, 0, 
                       &(pon_cos_remark_info.pon_gport[i]));

    if (rv != BCM_E_NONE)
    {
        printf("Error, pon_lif_create returned %s i:%d\n", bcm_errmsg(rv), i);
        return rv;
    }
    /* Add PON LIF to vswitch */
    rv = bcm_vswitch_port_add(unit, 
                              pon_cos_remark_info.vsi, 
                              pon_cos_remark_info.pon_gport[i]
                             );
    if (rv != BCM_E_NONE)
    {
        printf("Error, bcm_vswitch_port_add returned %s\n", bcm_errmsg(rv));
        return rv;
    }
	
    
    rv = multicast_vlan_port_add(unit, 
                                 pon_cos_remark_info.vsi+lif_offset, 
                                 pon_cos_remark_info.pon_port, 
                                 pon_cos_remark_info.pon_gport[i]);
    if (rv != BCM_E_NONE)
    {
        printf("Error, multicast_vlan_port_add gport %d\n", pon_cos_remark_info.pon_gport[2]);
        return rv;
    }


    rv = pon_cos_remark_qos_map_id_create(unit, pon_cos_remark_info.pon_gport[i], i, 0);
    if (rv != BCM_E_NONE)
    {
        printf("Error, pon_cos_remark_qos_map_id_create returned %s %d\n", bcm_errmsg(rv), i);
        return rv;
    }


    /* Set PON LIF ingress VLAN editor */
    rv = pon_port_ingress_vt_set(unit,
                                 otag_to_o_i_tag,
                                 pon_cos_remark_info.nni_svlan,
                                 pon_cos_remark_info.nni_cvlan,
                                 pon_cos_remark_info.pon_gport[i],
                                 pon_cos_remark_info.qos_map_id[i],
                                 &action_id);
    if (rv != BCM_E_NONE)
    {
        printf("Error, pon_port_ingress_vt_set returned 5s %d\n", bcm_errmsg(rv), i);
        return rv;
    }                                             
    /* Set PON LIF egress VLAN editor */
    rv = pon_port_egress_vt_set(unit,
                                otag_to_o_i_tag,
                                pon_cos_remark_info.tunnel_id[i],
                                0, 
                                0,
                                pon_cos_remark_info.pon_gport[i]
                               );
    if (rv != BCM_E_NONE)
    {
        printf("Error, pon_port_egress_vt_set returned %s\n", bcm_errmsg(rv));
        return rv;
    }
  }

  /* Create PON LIF for cos copy */
  i = 4; /* ONU5 */
  rv = pon_lif_create(unit, 
                      pon_cos_remark_info.pon_port, 
                      match_otag,
                      0, 0,
                      pon_cos_remark_info.tunnel_id[i],
                      pon_cos_remark_info.tunnel_id[i],
                      pon_cos_remark_info.pon_cvlan,
                      0, 0, 0, 
                     &(pon_cos_remark_info.pon_gport[i]));
  
  if (rv != BCM_E_NONE)
  {
      printf("Error, pon_lif_create returned %s i:%d\n", bcm_errmsg(rv), i);
      return rv;
  }
  /* Add PON LIF to vswitch */
  rv = bcm_vswitch_port_add(unit, 
                            pon_cos_remark_info.vsi, 
                            pon_cos_remark_info.pon_gport[i]
                           );
  if (rv != BCM_E_NONE)
  {
      printf("Error, bcm_vswitch_port_add returned %s\n", bcm_errmsg(rv));
      return rv;
  }
  
  /* Set PON LIF ingress VLAN editor */
  rv = pon_port_ingress_vt_set(unit,
                               otag_to_o_i_tag,
                               pon_cos_remark_info.nni_svlan,
                               pon_cos_remark_info.nni_cvlan,
                               pon_cos_remark_info.pon_gport[i],
                               0,
                               &action_id);
  if (rv != BCM_E_NONE)
  {
      printf("Error, pon_port_ingress_vt_set returned %s %d\n", bcm_errmsg(rv), i);
      return rv;
  }                                             
  /* Set PON LIF egress VLAN editor */
  rv = pon_port_egress_vt_set(unit,
                              otag_to_o_i_tag,
                              pon_cos_remark_info.tunnel_id[i],
                              0, 
                              0,
                              pon_cos_remark_info.pon_gport[i]
                             );
  if (rv != BCM_E_NONE)
  {
      printf("Error, pon_port_egress_vt_set returned %s\n", bcm_errmsg(rv));
      return rv;
  }

  /* Create NNI LIF */
  rv = nni_lif_create(unit,
                     /*trunk_gport,*/
                     nni_port_1,
                     match_otag,
                     0,
                     pon_cos_remark_info.nni_cvlan,
                     0, 
                     0,
                     &(pon_cos_remark_info.nni_gport),
                     &encap_id);
  if (rv != BCM_E_NONE) {
     printf("Error, nni_lif_create returned %s\n", bcm_errmsg(rv));
     return rv;
  }

  /*For upstream*/
  rv = multicast_vlan_port_add(unit, 
                              pon_cos_remark_info.vsi,
                              /*trunk_gport,*/
                              nni_port_1,
                              pon_cos_remark_info.nni_gport);
  if (rv != BCM_E_NONE) {
     printf("Error, multicast_vlan_port_add returned %s, nni_gport\n", bcm_errmsg(rv));
     return rv;
  }

  /* add to vswitch */
  rv = bcm_vswitch_port_add(unit,
                           pon_cos_remark_info.vsi,
                           pon_cos_remark_info.nni_gport);
  if (rv != BCM_E_NONE) {
     printf("Error, bcm_vswitch_port_add returned %s nni_gport\n", bcm_errmsg(rv));
     return rv;
  }
  
  /* Set the multicast group offset to flood downstream packets in multicast group(vsi+lif_offset) */
  bcm_port_control_set(unit, pon_cos_remark_info.nni_gport, bcmPortControlFloodUnknownUcastGroup, lif_offset);
  bcm_port_control_set(unit, pon_cos_remark_info.nni_gport, bcmPortControlFloodUnknownMcastGroup, lif_offset);
  bcm_port_control_set(unit, pon_cos_remark_info.nni_gport, bcmPortControlFloodBroadcastGroup, lif_offset);

  return rv;
}

/*********************************************************************************
* clean all configuration. 
*
* Steps
*     1. clean pon service configuration
 */
int pon_cos_remark_app_clean(int unit)
{
    int rv = 0;
    int i = 0;

    /* Delete pon gport */
    for (i = 0; i < 4; i++)
    {
      rv = pon_cos_remark_qos_map_id_destroy(unit, pon_cos_remark_info.pon_gport[i], i, 0);
      if (rv != BCM_E_NONE) 
      {
          printf("Error, pon_cos_remark_qos_map_id_destroy() returned %s remove pon gport[%d] %d failed\n", 
                 bcm_errmsg(rv), i, pon_cos_remark_info.pon_gport[i]);
          return rv;
      }

      rv = vswitch_delete_port(unit, pon_cos_remark_info.vsi, pon_cos_remark_info.pon_gport[i]);
      if (rv != BCM_E_NONE) 
      {
          printf("Error, vswitch_delete_port() returned %s remove pon gport[%d] %d failed\n",
                 bcm_errmsg(rv), i, pon_cos_remark_info.pon_gport[i]);
          return rv;
      }
    }

    /* ONU5 */
    i = 4; 
    rv = vswitch_delete_port(unit, pon_cos_remark_info.vsi, pon_cos_remark_info.pon_gport[i]);
    if (rv != BCM_E_NONE) 
    {
        printf("Error, vswitch_delete_port() returned %s remove pon gport[%d] %d failed\n",
               bcm_errmsg(rv), i, pon_cos_remark_info.pon_gport[i]);
        return rv;
    }

    /* Delete nni gport */
    rv = vswitch_delete_port(unit, pon_cos_remark_info.vsi, pon_cos_remark_info.nni_gport);
    if (rv != BCM_E_NONE)
    {
        printf("Error, vswitch_delete_port returned %s remove nni gport failed\n", bcm_errmsg(rv));
        return rv;
    }

    rv = vswitch_delete(unit, pon_cos_remark_info.vsi);
    if (rv != BCM_E_NONE)
    {
        printf("Error, vswitch_delete returned %s remove vsi failed\n", bcm_errmsg(rv));
        return rv;
    }

    return rv;
}



int pon_cos_remark_ingress_qos_profile_create(int unit, int *qos_map_id)
{
    int rv = BCM_E_NONE;
    int pcp_index = 0;
    int dei_index = 0;
    bcm_qos_map_t map_entry;

    rv = bcm_qos_map_create(unit, BCM_QOS_MAP_WITH_ID | BCM_QOS_MAP_INGRESS, qos_map_id);
    if (rv != BCM_E_NONE)
    {
        printf("error in bcm_qos_map_create() %s \n", bcm_errmsg(rv));
        return rv;
    }

    bcm_qos_map_t_init(&map_entry);
    for (pcp_index = 0; pcp_index < 8; pcp_index++)
    {
        for (dei_index = 0; dei_index < 2; dei_index++)
        {
            map_entry.pkt_pri = pcp_index;
            map_entry.pkt_cfi = dei_index;
            map_entry.int_pri = (pcp_index + 1) % 8;
            map_entry.color = dei_index;

            /* Map pcp/dei of outer tag */
            rv = bcm_qos_map_add(unit, BCM_QOS_MAP_L2, &map_entry, *qos_map_id);
            if (rv != BCM_E_NONE)
            {
                printf("error in bcm_qos_map_add() %s \n", bcm_errmsg(rv));
                return rv;
            }
        }
    }

    return rv;

}

int pon_cos_remark_egress_qos_profile_create(int unit, int *qos_remark_id)
{
    int rv = BCM_E_NONE;
    int tc_index = 0;
    int dp_index = 0;
    int flags = 0;
    bcm_qos_map_t map_entry;

    rv = bcm_qos_map_create(unit, BCM_QOS_MAP_EGRESS | BCM_QOS_MAP_L2_VLAN_PCP, qos_remark_id);
    if (rv != BCM_E_NONE)
    {
        printf("error in bcm_qos_map_create() %s \n", bcm_errmsg(rv));
        return rv;
    }

    bcm_qos_map_t_init(&map_entry);
    flags = BCM_QOS_MAP_L2 | BCM_QOS_MAP_L2_VLAN_PCP | BCM_QOS_MAP_L2_UNTAGGED;

    for (tc_index = 0; tc_index < 8; tc_index++)
    {
        for (dp_index = 0; dp_index < 2; dp_index++)
        {
            map_entry.int_pri = tc_index;
            map_entry.color = dp_index;
            map_entry.pkt_pri = (tc_index + 1) % 8;
            map_entry.pkt_cfi = dp_index;

            /* Map pcp/dei of outer tag */
            rv = bcm_qos_map_add(unit, flags, &map_entry, *qos_remark_id);
            if (rv != BCM_E_NONE)
            {
                printf("error in bcm_qos_map_add() %s \n", bcm_errmsg(rv));
                return rv;
            }
        }
    }

    return rv;

}

void create_pon_vlan_lif(int unit, int port, int tunnel_id, int vlan ,bcm_gport_t *gport_ac_lif)
{
    int rv = BCM_E_NONE;
    bcm_vlan_port_t vlan_port;

    bcm_vlan_port_t_init(&vlan_port);
    vlan_port.criteria = BCM_VLAN_PORT_MATCH_PORT_TUNNEL_VLAN;
    vlan_port.flags =  BCM_VLAN_PORT_INNER_VLAN_PRESERVE | BCM_VLAN_PORT_OUTER_VLAN_PRESERVE;
    vlan_port.match_tunnel_value = tunnel_id;
    vlan_port.egress_tunnel_value = tunnel_id;
    vlan_port.match_vlan = vlan;
    vlan_port.port = port;
    vlan_port.ingress_network_group_id = 1;
    vlan_port.egress_network_group_id = 1;
    rv = bcm_vlan_port_create(unit, &vlan_port);
    if (rv != BCM_E_NONE) {
        printf("Error, in bcm_vlan_port_create, port=%d, \n", port);
        return rv;
    }
    *gport_ac_lif = vlan_port.vlan_port_id;

    return rv;
}

void create_vlan_lif(int unit, int port, int vlan, bcm_gport_t *gport_ac_lif)
{
    int rv = BCM_E_NONE;
    bcm_vlan_port_t vlan_port;

    bcm_vlan_port_t_init(&vlan_port);
    vlan_port.criteria = BCM_VLAN_PORT_MATCH_PORT_VLAN;
    vlan_port.flags =  BCM_VLAN_PORT_INNER_VLAN_PRESERVE | BCM_VLAN_PORT_OUTER_VLAN_PRESERVE;
    vlan_port.match_vlan = vlan;
    vlan_port.port = port;
    rv = bcm_vlan_port_create(unit, &vlan_port);
    if (rv != BCM_E_NONE) {
        printf("Error, in bcm_vlan_port_create, port=%d, \n", port);
        return rv;
    }
    *gport_ac_lif = vlan_port.vlan_port_id;

    return rv;
}

int pon_cos_remark_with_local_switch(int unit, bcm_port_t pon_port, bcm_port_t nni_port, int vsi, int tunnel_id)
{
    int rv = BCM_E_NONE;
    bcm_gport_t nni_gport;
    bcm_gport_t pon_gport;
    uint32 flags = 0;
    int vlan = vsi;
    /* custom_feature_ipmc_l2_ssm_mode_lem==1 && local_switching_enable==1 : ingress lif cos is [32: 63] */
    int qos_map_id = 32;
    int qos_remark_id = 0;

    rv = bcm_port_class_set(unit, pon_port, bcmPortClassId, pon_port);
    if (rv != BCM_E_NONE) {
        printf("Error, in bcm_port_class_set, port=%d, \n", pon_port);
        return rv;
    }

    rv = bcm_port_class_set(unit, nni_port, bcmPortClassId, nni_port);
    if (rv != BCM_E_NONE) {
        printf("Error, in bcm_port_class_set, port=%d, \n", nni_port);
        return rv;
    }

    rv = bcm_vlan_create(unit, vsi);
    if (rv != BCM_E_NONE)
    {
        printf("Error, bcm_vlan_create returned %s\n", bcm_errmsg(rv));
        return rv;
    }

    flags = BCM_MULTICAST_INGRESS_GROUP | BCM_MULTICAST_WITH_ID | BCM_MULTICAST_TYPE_L2;
    rv = bcm_multicast_create(unit, flags, &vlan);
    if (rv != BCM_E_NONE)
    {
        printf("Error, bcm_multicast_create returned %s \n", bcm_errmsg(rv));
        return rv;
    }

    /* Create PON LIF */
    rv = create_pon_vlan_lif(unit, pon_port, tunnel_id, vlan, &pon_gport);
    if (rv != BCM_E_NONE)
    {
        printf("Error, create_pon_vlan_lif returned %s \n", bcm_errmsg(rv));
        return rv;
    }
    /* Add PON LIF to vswitch */
    rv = bcm_vswitch_port_add(unit, vsi, pon_gport);
    if (rv != BCM_E_NONE)
    {
        printf("Error, bcm_vswitch_port_add returned %s\n", bcm_errmsg(rv));
        return rv;
    }

    rv = multicast_vlan_port_add(unit, vsi, pon_port, pon_gport);
    if (rv != BCM_E_NONE)
    {
        printf("Error, multicast_vlan_port_add gport %d\n", pon_gport);
        return rv;
    }

    /* Create NNI LIF */
    rv = create_vlan_lif(unit, nni_port, vlan, &nni_gport);
    if (rv != BCM_E_NONE) {
       printf("Error, create_vlan_lif returned %s\n", bcm_errmsg(rv));
       return rv;
    }

    /* For upstream */
    rv = multicast_vlan_port_add(unit, vsi, nni_port, nni_gport);
    if (rv != BCM_E_NONE) {
       printf("Error, multicast_vlan_port_add returned %s, nni_gport\n", bcm_errmsg(rv));
       return rv;
    }

    /* add to vswitch */
    rv = bcm_vswitch_port_add(unit, vsi, nni_gport);
    if (rv != BCM_E_NONE) {
       printf("Error, bcm_vswitch_port_add returned %s nni_gport\n", bcm_errmsg(rv));
       return rv;
    }

    rv = pon_cos_remark_ingress_qos_profile_create(unit, &qos_map_id);
    if (rv != BCM_E_NONE)
    {
        printf("Error, pon_cos_remark_ingress_qos_profile_create returned %s\n", bcm_errmsg(rv));
        return rv;
    }

    rv = pon_cos_remark_egress_qos_profile_create(unit, &qos_remark_id);
    if (rv != BCM_E_NONE)
    {
        printf("Error, pon_cos_remark_egress_qos_profile_create returned %s\n", bcm_errmsg(rv));
        return rv;
    }

    /* Associate the QoS map profile to the In-LIF */
    rv = bcm_qos_port_map_set(0, pon_gport, qos_map_id, qos_remark_id);
    if (rv != BCM_E_NONE)
    {
        printf("Error, bcm_qos_port_map_set returned %s\n", bcm_errmsg(rv));
        return rv;
    }

    /* Associate the QoS map profile to the In-LIF */
    rv = bcm_qos_port_map_set(0, nni_gport, qos_map_id, qos_remark_id);
    if (rv != BCM_E_NONE)
    {
        printf("Error, bcm_qos_port_map_set returned %s\n", bcm_errmsg(rv));
        return rv;
    }

    if (soc_property_get(unit , "local_switching_enable",0))
    {
        rv = bcm_port_control_set(unit, pon_gport, bcmPortControlLocalSwitching, 1);
        if (rv != BCM_E_NONE)
        {
            printf("Error, nni_lif_create returned %s\n", bcm_errmsg(rv));
            return rv;
        }

        rv = bcm_port_control_set(unit, nni_gport, bcmPortControlLocalSwitching, 1);
        if (rv != BCM_E_NONE)
        {
            printf("Error, nni_lif_create returned %s\n", bcm_errmsg(rv));
            return rv;
        }
    }

    return rv;
}

