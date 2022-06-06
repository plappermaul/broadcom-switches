/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~OAM test~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*
 *
 * $Id: oam.c,v 1.140 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 * 
 * File: cint_oam_acceleration_advanced_features.c
 * Purpose: Example of using OAM APIs specific for arad +
 *
 * Usage:
 * 
   cd
   cd ../../../../src/examples/dpp
   cint cint_port_tpid.c
   cint cint_l2_mact.c
   cint cint_vswitch_metro_mp.c
   cint utility/cint_utils_oam.c
   cint cint_oam_acceleration_advanced_features.c
   cint
   int unit=0;
   int port1=13,port2=14,port3=15;
   print oam_lm_dm_run_with_defaults(unit,port1,port2,port3);
 
 * 
 * This cint uses cint_vswitch_metro_mp_single_vlan.c to build the following vswitch:
 *  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
 *  |                                                                       |
 *  |                   o  \                         o                      |
 *  |                      \  \<4>        <10,20>/  /                       |
 *  |                  /\ \  \   -----------    /  /                        |
 *  |                   \  \ \/ /  \   /\   \  /  / /\                      |
 *  |                <40>\  \  |    \  /     |\/ /  /                       |
 *  |                     \  \ |     \/      |  /  /<30>                    |
 *  |                       p3\|     /\      | /  /                         |
 *  |                          |    /  \     |/p1                           |             
 *  |                         /|   \/   \    |                              |
 *  |                        /  \  VSwitch  /                               | 
 *  |                   /\  /p2  -----------                                |
 *  |                <5>/  /  /                                             |
 *  |                  /  /  /<3>                                           |
 *  |                 /  /  /                                               |
 *  |                   /  \/                                               |
 *  |                  o                                                    |
 *  |                                                                       | 
 *  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * 
 * comments:
 * 1) In order to prevent from OAMP send packets do: BCM.0> m OAMP_MODE_REGISTER TRANSMIT_ENABLE=0
 * 
 */
/*
 * Creating vswitch and adding mac table entries
 */

/* Globals - MAC addresses , ports & gports*/
  bcm_multicast_t mc_group = 1234;
  bcm_mac_t mac_mep_1 = {0x00, 0x00, 0x00, 0x01, 0x02, 0x03};
  bcm_mac_t mac_mep_2 = {0x00, 0x00, 0x00, 0xff, 0xff, 0xff};
  bcm_mac_t mac_mep_3 = {0x00, 0x00, 0x00, 0x01, 0x02, 0xff};
  bcm_mac_t src_mac = { 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff };
  bcm_mac_t src_mac_1 = { 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xfe };
  bcm_mac_t mac_mip = {0x00, 0x00, 0x00, 0x01, 0x02, 0xfe};
  bcm_mac_t src_mac_mep_2 = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05};
  bcm_mac_t src_mac_mep_3 = {0x00, 0x01, 0x02, 0x03, 0x04, 0x01};
  bcm_mac_t mac_mep_2_mc = {0x01, 0x80, 0xc2, 0x00, 0x00, 0x35}; /* 01-80-c2-00-00-3 + md_level_2 */
  bcm_gport_t gport1, gport2, gport3; /* these are the ports created by the vswitch*/
  int port_1 = 13; /* physical port (signal generator)*/
  int port_2 = 14;
  int port_3 = 15;
  bcm_oam_group_info_t group_info_long_ma;
  bcm_oam_group_info_t group_info_short_ma;
  bcm_oam_endpoint_info_t mep_acc_info;
  bcm_oam_endpoint_info_t rmep_info;
  /*1-const; 32-long format; 13-length; all the rest - MA name*/
  uint8 long_name[BCM_OAM_GROUP_NAME_LENGTH] = {1, 32, 13, 01, 02, 03, 04, 05, 06, 07, 08, 09, 0xa, 0xb, 0xc, 0xd};
  /*1-const; 3-short format; 2-length; all the rest - MA name*/
  uint8 short_name[BCM_OAM_GROUP_NAME_LENGTH] = {1, 3, 2, 0xab, 0xcd};

  int timeout_events_count = 0;
  bcm_vlan_t  vsi;
  int md_level_mip = 7;
  bcm_oam_endpoint_info_t mip_info;

  int OpCode_LMM = 43;
  int OpCode_DMM = 47;
  int OpCode_LBM = 3;
  int OpCode_LTM = 5;
  int OpCode_SLM = 55;

  /* Jericho only */
  int oam_lm_dm_reports_enabled = 0; /* enable LM/DM reports */
  int oam_slm_use = 0;                       /* enable OAMP to use SLMs */

  /* Jericho only - sticky statistics buffers */
  uint32 delay_report_count = 0;
  uint32 last_delta_FCB = -1;
  uint32 last_delta_FCf = -1;

  /* Determines whether PCP based counting configuration uses SLM or not.
     In Jericho A0 this is ignored, PCP based counting is supported only for SLM*/
  int oam_pcp_example_use_slm =0;

  int counter =0;
  int mep_id = 4096;
  int global_gtf_id =0;

  int arad_plus_device, jericho_device; /* Should be initialized in "initialize_vswitch_and_oam"*/
  int lb_tst_enable = 0;
  int lb_pcp_dei_enable= 0;

  /* Created endpoints information */
  oam__ep_info_s server_upmep;
  oam__ep_info_s server_downmep;
  oam__ep_info_s client_upmep;
  oam__ep_info_s client_downmep;
  oam__ep_info_s ep2;
  oam__ep_info_s ep3;

/**
 * Triggered upon CCM timeout event: Transmit AIS frames to 
 * indicate LOC. 
 * 
 * @author sinai (17/09/2014)
 * 
 * @param unit 
 * @param flags 
 * @param event_type 
 * @param group 
 * @param endpoint 
 * @param user_data 
 * 
 * @return int 
 */
int cb_oam_ais(int unit, uint32 flags, bcm_oam_event_type_t event_type, bcm_oam_group_t group,
               bcm_oam_endpoint_t endpoint, void *user_data) {
    bcm_oam_ais_t ais;
    int rv; 

    print event_type;
    printf("Adding AIS to MEP: 0x%08x\n", endpoint);

    ais.id = endpoint & 0x3fff ; /* endpoint represents that of the RMEP. 
         14 LSBs of the endpoint represent the HW ID which is identical to that of the local MEP.*/
    ais.period =BCM_OAM_ENDPOINT_CCM_PERIOD_1S; /**/
    ais.flags |= BCM_OAM_AIS_MULTICAST;
    ais.level = 7;

    rv = bcm_oam_ais_add(unit,&ais);
    if (rv!=BCM_E_NONE ) {
      printf("(%s) \n",bcm_errmsg(rv));
      return rv;
    }
    return BCM_E_NONE;
}


/**
 * Triggered upon LM/DM report rx.
 */
int cb_stat(int unit,
            bcm_oam_event_type_t event_type,
            bcm_oam_group_t group,
            bcm_oam_endpoint_t endpoint,
            bcm_oam_performance_event_data_t *event_data,
            void *user_data) {

    print unit;
    print event_type;
    print group;
    print endpoint;
    print *event_data;

    /* Update globals */
    switch (event_type) {
    case bcmOAMEventEndpointDmStatistics:
        delay_report_count++;
        break;
    case bcmOAMEventEndpointLmStatistics:
        last_delta_FCB = event_data->delta_FCB;
        last_delta_FCf = event_data->delta_FCf;
        break;
    }
}


/**
 * The OAMP may reply to LMMs, DMMs, LBMs, LTMs and EXMs, SLMs with 
 * their respective reply packets. 
 * This is an example of how to set the reply mechanism. 
 *  
 * The reply packet's DA will be taken from the incoming 
 * packet's SA. 
 * The reply packets SA will be as configured in the 
 * endpoint_create() API. 
 *  
 * Note that LMRs, DMRs, SLRs will be transmitted with the 
 * counters/ToD stamped on the packet. For other types of reply 
 * packets, only the OpCode will change.
 * 
 * @author sinai (12/03/2015)
 * 
 * @param unit 
 * @param mep_id 
 * @param opcode 
 * @param endpoint_type_is_ethernet - set to 1 for Ethernet 
 *                                  endpoints, 0 for MPLS/PWE.
 *  
 * @return int 
 */
int oam_configure_oamp_reply(int unit, int mep_id, int opcode) {
    bcm_gport_t oamp_gports[2];
    int count_oamp_port;
    int rv;
    int trap_code;
    bcm_rx_trap_t trap_type;
    bcm_oam_endpoint_info_t ep;
    bcm_oam_action_type_t action_type;
    bcm_oam_endpoint_action_t action;

    if (opcode != OpCode_LTM && opcode != OpCode_DMM && opcode != OpCode_LMM && opcode != OpCode_LBM && opcode != OpCode_SLM) {
        printf("Given opcode must be valid reply message. Given opcode: %d\n",opcode);
        return 44;
    }

    /* Determine endpoint type */
    rv = bcm_oam_endpoint_get(unit,mep_id,&ep);
    if (rv != BCM_E_NONE) {
        printf("Unit %d: Error, in bcm_oam_endpoint_get\n", unit);
        return rv;
    }

    if (ep.flags & BCM_OAM_ENDPOINT_UP_FACING) {
        /* Only ethernet type endpoint can be up */
        trap_type = bcmRxTrapOamUpMEP2;
        action_type = bcmOAMActionUcFwd;
    } else {
        /* Use trap by endpoint type */
        if (ep.type == bcmOAMEndpointTypeEthernet) {
            trap_type = bcmRxTrapOamEthAccelerated;
            action_type = bcmOAMActionUcFwd;
        } else if (ep.type == bcmOAMEndpointTypeBHHMPLS ||
                   ep.type == bcmOAMEndpointTypeBHHPweGAL) {
            trap_type = bcmRxTrapOamY1731MplsTp;
            action_type = bcmOAMActionMcFwd;
        } else if (ep.type == bcmOAMEndpointTypeBHHPwe ||
                   ep.type == bcmOAMEndpointTypeBhhSection) {
            trap_type = bcmRxTrapOamY1731Pwe;
            action_type = bcmOAMActionMcFwd;
        }
    }
    /* Configure trap */
    rv = bcm_rx_trap_type_get(unit, 0, trap_type, &trap_code);
    if (rv != BCM_E_NONE) {
        printf("(%s) \n", bcm_errmsg(rv));
    }

    bcm_oam_endpoint_action_t_init(&action);

    BCM_GPORT_TRAP_SET(action.destination, trap_code, 7, 0);

    BCM_OAM_OPCODE_SET(action, opcode);
    BCM_OAM_ACTION_SET(action, action_type);
    if (opcode == OpCode_SLM) {
        BCM_OAM_ACTION_SET(action,bcmOAMActionSLMEnable);
        BCM_OAM_ACTION_SET(action,bcmOAMActionCountEnable);
    }

    rv = bcm_oam_endpoint_action_set(unit, mep_id, &action);

    if (rv != BCM_E_NONE) {
        printf("bcm_oam_endpoint_action_set failed (%s) \n",bcm_errmsg(rv));
        return rv;
    }

    return 0;
}


/**
 * Create an accelerated MEP with ID 4096 and level 2 and a MEP
 * with ID 4100 and level 5. 
 * MEP 4096 will have a loss (with statistics extended), delay, 
 * and loopback-reply entry. 
 *  
 * MEP 4100 will have a piggy-back loss entry.
 * 
 * @author sinai (09/09/2013)
 * 
 * @param unit 
 * @param port1 physical ports to be used
 * @param port2 
 * @param port3 
 * 
 * @return int 
 */
int oam_lm_dm_run_with_defaults(int unit, int port1, int port2, int port3) {
	bcm_error_t rv;
    bcm_oam_endpoint_info_t acc_endpoint; 
    bcm_oam_endpoint_info_t remote_endpoint;


    single_vlan_tag = 1;

    port_1 = port1;
    port_2 = port2;
    port_3 = port3;

    rv = initialize_vswitch_and_oam(unit);
    if (rv != BCM_E_NONE) {
        printf("(%s) \n", bcm_errmsg(rv));
        return rv;
    }
  
    if (device_type<device_type_arad_plus) {
        printf("Cint only works for Arad+.\n");
        return 21; 
    }


    mep_id = (device_type == device_type_qux) ? 250 : 4096;
    counter = 1;
    rv =   create_acc_endpoint(unit, &acc_endpoint, &remote_endpoint, 1, group_info_short_ma.id);
    if (rv != BCM_E_NONE) {
        printf("(%s) \n", bcm_errmsg(rv));
        return rv;
    }

    ep2.id = acc_endpoint.id;
    ep2.rmep_id = remote_endpoint.id;

    mep_id = (device_type == device_type_qux) ? 254 : 4100;
    counter = 4; /*This gives us a mep with the same LIF but a diffferent level.*/
    rv =   create_acc_endpoint(unit, &acc_endpoint, &remote_endpoint, 0, group_info_short_ma.id);
    if (rv != BCM_E_NONE) {
        printf("(%s) \n", bcm_errmsg(rv));
        return rv;
    }

    ep3.id = acc_endpoint.id;
    ep3.rmep_id = remote_endpoint.id;

    /* In order to send packets to endpoint 4096, send the packets to port1.*/
    bcm_l2_addr_t addr;
    bcm_mac_t mac; 
    mac[3] = mac[4] = mac[5] = 0xfe;

    bcm_l2_addr_t_init(&addr, mac, 4096);
    BCM_GPORT_LOCAL_SET(addr.port, port1); 
    rv =  bcm_l2_addr_add(unit, &addr); 
    if (rv != BCM_E_NONE) {
        printf("(%s) \n", bcm_errmsg(rv));
        return rv;
    }

/* Create LM/DM/LBR entries for endpoint 4096:*/


    bcm_oam_loss_t loss;
    bcm_oam_delay_t delay;
    bcm_oam_loopback_t lb;
    bcm_oam_loss_t_init(&loss);
    bcm_oam_delay_t_init(&delay);
    bcm_oam_loopback_t_init(&lb);
    lb.id = delay.id = loss.id = ((device_type == device_type_qux) ? 250 : 4096);
    loss.period = BCM_OAM_ENDPOINT_CCM_PERIOD_100MS;
    delay.period = BCM_OAM_ENDPOINT_CCM_PERIOD_10MS;
    delay.timestamp_format = bcmOAMTimestampFormatIEEE1588v1; /*The onlt supported format*/
    loss.peer_da_mac_address[3] = delay.peer_da_mac_address[3] = lb.peer_da_mac_address[3] = 0xef;
    loss.peer_da_mac_address[1] = delay.peer_da_mac_address[1] = lb.peer_da_mac_address[1] = 0xab;
    /* use extended statistics*/
    loss.flags = BCM_OAM_LOSS_SINGLE_ENDED; /* LMM based loss management*/

    if (oam_lm_dm_reports_enabled) {
        bcm_oam_event_types_t loss_event, delay_event;

        printf("Setting report mode for Loss/Delay measurements\nand increasing period to reduce CPU load\n");
        loss.flags |= BCM_OAM_LOSS_REPORT_MODE; /* LMR report mode */
        loss.period = BCM_OAM_ENDPOINT_CCM_PERIOD_10M;
        delay.flags |= BCM_OAM_DELAY_REPORT_MODE; /* DMR report mode */
        delay.period = BCM_OAM_ENDPOINT_CCM_PERIOD_10M;

        printf("Registering callbacks for statistics events\n");

        BCM_OAM_EVENT_TYPE_SET(loss_event, bcmOAMEventEndpointLmStatistics);
        rv = bcm_oam_performance_event_register(unit, loss_event, cb_stat, (void*)1);
        if (rv != BCM_E_NONE) {
            printf("(%s) \n", bcm_errmsg(rv));
            return rv;
        }

        BCM_OAM_EVENT_TYPE_SET(delay_event, bcmOAMEventEndpointDmStatistics);
        rv = bcm_oam_performance_event_register(unit, delay_event, cb_stat, (void*)2);
        if (rv != BCM_E_NONE) {
            printf("(%s) \n", bcm_errmsg(rv));
            return rv;
        }

    }
    else {
        loss.flags |= BCM_OAM_LOSS_STATISTICS_EXTENDED;
    }

/*
 * Count SLM packets for a given endpoint. 
 * For the LIF on which the given endpoint resides data 
 * packets will not be counted 
 */
    if (oam_slm_use && device_type == device_type_jericho) {
        loss.flags |=BCM_OAM_LOSS_SLM;
    }
    
    if (device_type >= device_type_qax) {
        loss.loss_id = (device_type == device_type_qux) ? 0x1 : 0x100;
        loss.flags |= BCM_OAM_LOSS_WITH_ID;
    }
    rv = bcm_oam_loss_add(unit,&loss);
    if (rv != BCM_E_NONE) {
        printf("(%s) \n", bcm_errmsg(rv));
        return rv;
    }

    rv = bcm_oam_delay_add(unit,&delay);
    if (rv != BCM_E_NONE) {
        printf("(%s) \n", bcm_errmsg(rv));
        return rv;
    }

    if (device_type == device_type_arad_plus) { 
        rv = bcm_oam_loopback_add(unit, &lb);
        if (rv != BCM_E_NONE) {
            printf("(%s) \n", bcm_errmsg(rv));
            return rv;
        }
    }

    /* Now a CCM based entry for endpoint 4100.*/
    bcm_oam_loss_t_init(&loss);
    loss.id = (device_type == device_type_qux) ? 254 : 4100;
    loss.flags = oam_lm_dm_reports_enabled ? BCM_OAM_LOSS_REPORT_MODE : 0;
    if (device_type >= device_type_qax) {
        loss.loss_id = (device_type == device_type_qux) ? 0x2 : 0x101;
        loss.flags |= BCM_OAM_LOSS_WITH_ID;
    }
    rv = bcm_oam_loss_add(unit,&loss);
    if (rv != BCM_E_NONE) {
        printf("(%s) \n", bcm_errmsg(rv));
        return rv;
    }

    if (device_type>=device_type_jericho) {
        if (!oam_lm_dm_reports_enabled) {
            printf("Register AIS callback: transmit AISframes upon loss of continuity interrupt\n");
            bcm_oam_event_types_t timeout_event, timein_event;
            BCM_OAM_EVENT_TYPE_SET(timeout_event, bcmOAMEventEndpointCCMTimeout);
            rv = bcm_oam_event_register(unit, timeout_event, cb_oam_ais, (void *)0  );
            if (rv != BCM_E_NONE) {
                printf("(%s) \n", bcm_errmsg(rv));
                return rv;
            }
        }

    }

    return 0;
}

 /*
 * configure SAT starting to send LBM/TST packet.
 */
int oam_lb_tst_sat_start(int unit, int gtf_id)
 {
     bcm_error_t rv;
     bcm_sat_gtf_bandwidth_t bw1,bw2;
     bcm_sat_gtf_rate_pattern_t rate_pattern1,rate_pattern2;
     
     bcm_sat_gtf_bandwidth_t_init(&bw1);
     bcm_sat_gtf_bandwidth_t_init(&bw2);
     bcm_sat_gtf_rate_pattern_t_init(&rate_pattern1);
     bcm_sat_gtf_rate_pattern_t_init(&rate_pattern2);
      
     /*1 Set Bandwidth */
     bw1.rate = 64; /*gtf_bw_cir_rate;*/
     bw1.max_burst = 100;/*gtf_bw_cir_max_burst;*/
     rv = bcm_sat_gtf_bandwidth_set(unit, gtf_id, 0, &bw1);
     if (rv != BCM_E_NONE) {
         printf("(%s) \n", bcm_errmsg(rv));
         return rv;
     }
     
     bw1.rate = 64;/*gtf_bw_cir_rate + gtf_bw_eir_rate;*/
     bw1.max_burst = 100;/*gtf_bw_cir_max_burst+ gtf_bw_eir_max_burst;*/
     rv = bcm_sat_gtf_bandwidth_set(unit, gtf_id, 1, &bw1);
     if (rv != BCM_E_NONE) {
         printf("(%s) \n", bcm_errmsg(rv));
         return rv; 
     }
     
     bw1.rate = 128;/*gtf_bw_cir_rate + gtf_bw_eir_rate;*/
     bw1.max_burst = 200;/*gtf_bw_cir_max_burst+ gtf_bw_eir_max_burst;*/
     rv = bcm_sat_gtf_bandwidth_set(unit, gtf_id, -1, &bw1);
     if (rv != BCM_E_NONE) {
         printf("(%s) \n", bcm_errmsg(rv));
         return rv;
     }
     
     /* 2. Set Rate Pattern */
     rate_pattern1.rate_pattern_mode = bcmSatGtfRatePatternContinues; /*gtf_rate_pattern_mode;*/
     rate_pattern1.high_threshold = 10; /*gtf_rate_pattern_high;*/
     rate_pattern1.low_threshold =10 ; /*gtf_rate_pattern_low;*/
     rate_pattern1.stop_iter_count = 1000; /*gtf_rate_pattern_stop_iter_count;*/
     rate_pattern1.stop_burst_count = 10000; /*gtf_rate_pattern_stop_burst_count;*/
     rate_pattern1.stop_interval_count = 10000; /*gtf_rate_pattern_stop_interval_count;*/
     rate_pattern1.burst_packet_weight = 1; /*gtf_rate_pattern_burst_packet_weight;*/
     rate_pattern1.interval_packet_weight = 1; /*gtf_rate_pattern_interval_packet_weight;*/
     rv = bcm_sat_gtf_rate_pattern_set(unit,gtf_id, 0, &rate_pattern1);
     if (rv != BCM_E_NONE) {
         printf("(%s) \n", bcm_errmsg(rv));
         return rv;
     }
	 /*CIR and EIR have the same sequence number, so we just need to start one of them*/
     rv = bcm_sat_gtf_packet_start(unit, gtf_id, bcmSatGtfPriCir);
     if (rv != BCM_E_NONE) {
         printf("(%s) \n", bcm_errmsg(rv));
         return rv;
     }
	 
     return 0;
  }

 /**
  *  To verify the max number of lb creating and deleting
  */
 int oam_max_lb_tst_run(int unit, int port1, int port2, int port3)
 {
       bcm_error_t rv;
       bcm_oam_endpoint_info_t acc_endpoint; 
       bcm_oam_endpoint_info_t remote_endpoint;
       int max_number = 8;
       bcm_oam_loopback_t lb[max_number];
       bcm_oam_tst_rx_t  tst_rx_ptr[max_number];
       bcm_oam_tst_tx_t  tst_tx_ptr[max_number];
       int index = 0;
       lb_tst_enable = 1;
       port_1 = port1;
       port_2 = port2;
       port_3 = port3;
       single_vlan_tag = 1;
       
       rv = initialize_vswitch_and_oam(unit);
       if (rv != BCM_E_NONE) {
           printf("(%s) \n", bcm_errmsg(rv));
           return rv;
       }
       counter = 1; 
       mep_id = (device_type == device_type_qux) ? 1003 : 4009;
       rv =   create_acc_endpoint(unit, &acc_endpoint, &remote_endpoint, 0, group_info_short_ma.id);
       if (rv != BCM_E_NONE) {
           printf("(%s) \n", bcm_errmsg(rv));
           return rv;
       }
       counter = counter+3; 
       
       for(index = 0; index <max_number; index++){
           bcm_oam_loopback_t_init(lb[index]);
           bcm_oam_tst_rx_t_init(tst_rx_ptr[index]);
           bcm_oam_tst_tx_t_init(tst_tx_ptr[index]);
           tst_tx_ptr[index].endpoint_id = tst_rx_ptr[index].endpoint_id = lb[index].id = mep_id;
           tst_tx_ptr[index].peer_da_mac_address = lb[index].peer_da_mac_address = "00:01:02:03:04:05";
           tst_tx_ptr[index].peer_da_mac_address[5] = lb[index].peer_da_mac_address[5] = index;
           lb[index].num_tlvs =1;
           lb[index].tlvs[0].four_byte_repeatable_pattern = 0xABCDABCD;
           lb[index].tlvs[0].tlv_type = bcmOamTlvTypeData;
           lb[index].tlvs[0].tlv_length = 46;
           rv =  bcm_oam_loopback_add(0, lb[index]);
           if (rv != BCM_E_NONE) {
               printf("(%s) \n", bcm_errmsg(rv));
               return rv;
           }
          rv = bcm_oam_loopback_delete(0, lb[index]);
          if (rv != BCM_E_NONE) {
              printf("(%s) \n", bcm_errmsg(rv));
              return rv;
          }
       }
       
       for(index = 0; index <max_number; index++){
           bcm_oam_loopback_t_init(lb[index]);
           bcm_oam_tst_rx_t_init(tst_rx_ptr[index]);
           bcm_oam_tst_tx_t_init(tst_tx_ptr[index]);
           
           mep_id = ((device_type == device_type_qux) ? 1025 : 4100) + index;
           rv =   create_acc_endpoint(unit, &acc_endpoint, &remote_endpoint, 0, group_info_short_ma.id);
           if (rv != BCM_E_NONE) {
               printf("(%s) \n", bcm_errmsg(rv));
               return rv;
           }
           
           tst_tx_ptr[index].endpoint_id = tst_rx_ptr[index].endpoint_id = lb[index].id = mep_id;
           tst_tx_ptr[index].peer_da_mac_address = lb[index].peer_da_mac_address = "00:01:02:03:04:05";
           tst_tx_ptr[index].peer_da_mac_address[5] = lb[index].peer_da_mac_address[5] = index;
           lb[index].num_tlvs =1;
           lb[index].tlvs[0].four_byte_repeatable_pattern = 0xABCDABCD;
           lb[index].tlvs[0].tlv_type = bcmOamTlvTypeData;
           lb[index].tlvs[0].tlv_length = 46;
           rv =  bcm_oam_loopback_add(0, lb[index]);
           if (rv != BCM_E_NONE) {
               printf("(%s) \n", bcm_errmsg(rv));
               return rv;
           }
           tst_tx_ptr[index].tlv.tlv_length = 36;
           tst_tx_ptr[index].tlv.tlv_type =  bcmOamTlvTypeTestPrbsWithCRC;
           tst_rx_ptr[index].expected_tlv.tlv_length = 36;
           tst_rx_ptr[index].expected_tlv.tlv_type =  bcmOamTlvTypeTestPrbsWithCRC;
 
           counter = counter+3; 
       }
  
       for(index = 0; index <max_number; index++){
          bcm_oam_loopback_t_init(lb[index]);
          mep_id = ((device_type == device_type_qux) ? 1025 : 4100) + index;
          lb[index].id = mep_id;
          rv = bcm_oam_loopback_delete(0, lb[index]);
          if (rv != BCM_E_NONE) {
              printf("(%s) \n", bcm_errmsg(rv));
              return rv;
          }
       }
       
       /*Before testing tst, must make sure the LB were deleted*/
       for(index = 0; index <max_number; index++){
           rv = bcm_oam_tst_rx_add(unit, tst_rx_ptr[index]);
           if (rv != BCM_E_NONE) {
               printf("(%s) \n", bcm_errmsg(rv));
               return rv;
           }
           rv = bcm_oam_tst_tx_add(unit, tst_tx_ptr[index]);
           if (rv != BCM_E_NONE) {
               printf("(%s) \n", bcm_errmsg(rv));
               return rv;
           }
         }
       
       for(index = 0; index <max_number; index++){
           bcm_oam_tst_rx_t_init(tst_rx_ptr[index]);
           bcm_oam_tst_tx_t_init(tst_tx_ptr[index]);
           mep_id = ((device_type == device_type_qux) ? 1025 : 4100) + index;
           tst_tx_ptr[index].endpoint_id = tst_rx_ptr[index].endpoint_id = mep_id;
          rv = bcm_oam_tst_rx_delete(unit, tst_rx_ptr[index]);
          if (rv != BCM_E_NONE) {
              printf("(%s) \n", bcm_errmsg(rv));
              return rv;
          }
          rv = bcm_oam_tst_tx_delete(unit, tst_tx_ptr[index]);
          if (rv != BCM_E_NONE) {
              printf("(%s) \n", bcm_errmsg(rv));
              return rv;
          }
       }
      
       return rv;
 }

 
 /**
   *  To verify the different TLV lb (short packet should longer than 64)
   */
int oam_diff_tlv_lb_with_short(int unit, int port1, int port2, int port3, int update)
{
  bcm_error_t rv;
  bcm_oam_endpoint_info_t acc_endpoint; 
  bcm_oam_endpoint_info_t remote_endpoint;
  bcm_oam_loopback_t lb;
  int gtf_id=0;
  single_vlan_tag = 1;
  port_1 = port1;
  port_2 = port2;
  port_3 = port3;
  lb_tst_enable = 1;
  bcm_oam_loopback_t_init(&lb);
  lb.peer_da_mac_address[0] = 0x56;
  lb.peer_da_mac_address[1] = 0x78;
  lb.peer_da_mac_address[2] = 0xef;
  lb.peer_da_mac_address[3] = 0xcd;
  lb.peer_da_mac_address[4] = 0x00;
  lb.peer_da_mac_address[5] = 0x00;
  if(!update){
      rv = initialize_vswitch_and_oam(unit);
      if (rv != BCM_E_NONE) {
          printf("(%s) \n", bcm_errmsg(rv));
          return rv;
      }
      
      if (device_type<device_type_jericho) {
          printf("Cint only works for Jericho.\n");
          return 21; 
      }

      mep_id = (device_type == device_type_qux) ? 1028 : 4100;
      lb.id = (device_type == device_type_qux) ? 1028 : 4100;

      counter = 1; 
      rv =   create_acc_endpoint(unit, &acc_endpoint, &remote_endpoint, 0, group_info_short_ma.id);
      if (rv != BCM_E_NONE) {
          printf("(%s) \n", bcm_errmsg(rv));
          return rv;
      }
      ep2.id = acc_endpoint.id;
      ep2.rmep_id = remote_endpoint.id;
      /*lb for config*/
      lb.num_tlvs =0;
      rv = bcm_oam_loopback_add(unit, &lb);
      if (rv != BCM_E_NONE) {
          printf("(%s) \n", bcm_errmsg(rv));
          return rv;
      }
      gtf_id=lb.gtf_id;
      printf("lb gtf %d \n\n",lb.gtf_id);
  }
  else{
    /*lb config*/
    lb.id = mep_id;
    lb.num_tlvs =1;   
    lb.tlvs[0].four_byte_repeatable_pattern = 0xABCDEEFF;
    lb.tlvs[0].tlv_type =bcmOamTlvTypeData;
    lb.tlvs[0].tlv_length =34;
    lb.flags = BCM_OAM_LOOPBACK_UPDATE;
    
    /* Update lb configure*/
     rv = bcm_oam_loopback_add(unit, &lb);
     if (rv != BCM_E_NONE) {
         printf("(%s) \n", bcm_errmsg(rv));
         return rv;
     }
     gtf_id=lb.gtf_id;
     printf("UPDATE_lb gtf %d \n\n",lb.gtf_id);
  }
  
  /*start to send lb/tst packet*/
  rv = oam_lb_tst_sat_start(unit, gtf_id);
  if (rv != BCM_E_NONE) {
        printf("(%s) \n", bcm_errmsg(rv));
        return rv;
  }
  return 0;
}

 /**
  *  An example of creating Loopback and TST function base on SAT
  * 
  *  1, create an MEP with ID 4100 and level 2 
  *  2, create a LoopBack or TST function with lb_tst_flag
  *  3, configure SAT to send lb or tst packet.
  * 
  * @author xiao (09/02/2015)
  * 
  * @param unit 
  * @param port1 physical ports to be used
  * @param port2 
  * @param port3 
  * @param lb_tst_flag: 1mens creating lb, 2 mens creating tst
  * @param mep_is_up: 0 - the created mep would be down facing, otherwise
  *                 - up facing.
  * @param lb_pcp_dei_flag: default 0, 1 means test lb with different pcp dei. 
  * @return int 
  */
 int oam_lb_tst_sat_run(int unit, int port1, int port2, int port3, int lb_tst_flag, int mep_is_up, int lb_pcp_dei_flag)
{
    bcm_error_t rv;
    bcm_oam_endpoint_info_t acc_endpoint; 
    bcm_oam_endpoint_info_t remote_endpoint;
    bcm_oam_loopback_t lb;
    bcm_oam_tst_rx_t  tst_rx_ptr;
    bcm_oam_tst_tx_t  tst_tx_ptr;
    int gtf_id=0;
    uint8 lb_pcp = 5;  /* different with ccm packet pri(2)*/
    uint8 lb_dei = 1;
    uint8 lb_inner_pcp = 4;/* different with ccm packet inner pri(7)*/
    uint8 lb_inner_dei = 1;
    uint8 lb_int_pri = 10;
    
    single_vlan_tag = 1;
    port_1 = port1;
    port_2 = port2;
    port_3 = port3;
    lb_tst_enable = 1;
    rv = initialize_vswitch_and_oam(unit);
    if (rv != BCM_E_NONE) {
        printf("(%s) \n", bcm_errmsg(rv));
        return rv;
    }
  
    if (device_type<device_type_jericho) {
        printf("Cint only works for Jericho.\n");
        return 21; 
    }
    bcm_oam_loopback_t_init(&lb);
    bcm_oam_tst_rx_t_init(&tst_rx_ptr);
    bcm_oam_tst_tx_t_init(&tst_tx_ptr);
    if(lb_pcp_dei_flag){
        lb_pcp_dei_enable = 1;
        lb.pkt_pri = (lb_pcp* 2) + lb_dei;
        lb.inner_pkt_pri =  (lb_inner_pcp * 2) + lb_inner_dei;
        lb.int_pri= lb_int_pri;
    }
    
    mep_id = (device_type == device_type_qux) ? 1028 : 4100;
    counter = 1; 
    rv =   create_acc_endpoint(unit, &acc_endpoint, &remote_endpoint, mep_is_up, group_info_short_ma.id);
    if (rv != BCM_E_NONE) {
        printf("(%s) \n", bcm_errmsg(rv));
        return rv;
    }
    ep2.id = acc_endpoint.id;
    ep2.rmep_id = remote_endpoint.id;

    /* Create lb/tst entries for endpoint 4100:*/
    tst_tx_ptr.endpoint_id=tst_rx_ptr.endpoint_id=lb.id = ((device_type == device_type_qux) ? 1028 : 4100);/*tst &lb test down mep*/
    tst_tx_ptr.peer_da_mac_address[3]= lb.peer_da_mac_address[3] = 0xef;
    tst_tx_ptr.peer_da_mac_address[1]= lb.peer_da_mac_address[1] = 0xab;
    
    /*lb for config*/
    lb.num_tlvs =1;
    lb.tlvs[0].four_byte_repeatable_pattern = lb_pcp_dei_flag? 0x0:0xABCDEEFF;
    lb.tlvs[0].tlv_type =bcmOamTlvTypeData;
    lb.tlvs[0].tlv_length =46;
    /*tst config*/
    tst_rx_ptr.expected_tlv.tlv_length = 34; /* tlv length should longer than 34 */
    tst_rx_ptr.expected_tlv.tlv_type =  bcmOamTlvTypeTestPrbsWithoutCRC;
    tst_tx_ptr.tlv.tlv_length = 34;
    tst_tx_ptr.tlv.tlv_type =  bcmOamTlvTypeTestPrbsWithoutCRC;
    
    if (1 == lb_tst_flag){ 
        rv = bcm_oam_loopback_add(unit, &lb);
        if (rv != BCM_E_NONE) {
            printf("(%s) \n", bcm_errmsg(rv));
            return rv;
        }
        gtf_id=lb.gtf_id;
        printf("lb gtf %d \n\n",lb.gtf_id);
    }
    else if (2 == lb_tst_flag){
        rv = bcm_oam_tst_rx_add(unit, &tst_rx_ptr);
        if (rv != BCM_E_NONE) {
            printf("(%s) \n", bcm_errmsg(rv));
            return rv;
        }
        rv = bcm_oam_tst_tx_add(unit, &tst_tx_ptr);
        if (rv != BCM_E_NONE) {
            printf("(%s) \n", bcm_errmsg(rv));
            return rv;
        }
        gtf_id=tst_tx_ptr.gtf_id;
        printf("lb_tst gtf %d ctf %d\n\n",tst_tx_ptr.gtf_id,tst_rx_ptr.ctf_id);
     }
    else{
        printf("lb_tst flag error \n");
        return -1;
    }
       
   /*lb update config*/
   lb.num_tlvs =1;   
   lb.tlvs[0].four_byte_repeatable_pattern = lb_pcp_dei_flag? 0x0:0xABCDEEFF;
   lb.tlvs[0].tlv_type =bcmOamTlvTypeData;
   lb.tlvs[0].tlv_length =36;
   lb.flags = BCM_OAM_LOOPBACK_UPDATE;
   tst_tx_ptr.peer_da_mac_address[0] = lb.peer_da_mac_address[0] = 0x56;
   tst_tx_ptr.peer_da_mac_address[1] = lb.peer_da_mac_address[1] = 0x78;
   tst_tx_ptr.peer_da_mac_address[2] = lb.peer_da_mac_address[2] = 0xef;
   tst_tx_ptr.peer_da_mac_address[3] = lb.peer_da_mac_address[3] = 0xcd;
   tst_tx_ptr.peer_da_mac_address[4] = lb.peer_da_mac_address[4] = 0x00;
   tst_tx_ptr.peer_da_mac_address[5] = lb.peer_da_mac_address[5] = 0x00;
   tst_tx_ptr.tlv.tlv_length = 36;
   tst_tx_ptr.tlv.tlv_type =  bcmOamTlvTypeTestPrbsWithCRC;
   tst_tx_ptr.flags = BCM_OAM_TST_TX_UPDATE;
   tst_rx_ptr.expected_tlv.tlv_length = 36;
   tst_rx_ptr.expected_tlv.tlv_type =  bcmOamTlvTypeTestPrbsWithCRC;
   tst_rx_ptr.flags = BCM_OAM_TST_RX_UPDATE;
   
   /* Update lb configure*/
   if (1 == lb_tst_flag){ 
       rv = bcm_oam_loopback_add(unit, &lb);
       if (rv != BCM_E_NONE) {
           printf("(%s) \n", bcm_errmsg(rv));
           return rv;
       }
       gtf_id=lb.gtf_id;
       printf("UPDATE_lb gtf %d \n\n",lb.gtf_id);

   }
   else if (2 == lb_tst_flag){
       rv = bcm_oam_tst_rx_add(unit, &tst_rx_ptr);
       if (rv != BCM_E_NONE) {
           printf("(%s) \n", bcm_errmsg(rv));
           return rv;
       }
       rv = bcm_oam_tst_tx_add(unit, &tst_tx_ptr);
       if (rv != BCM_E_NONE) {
           printf("(%s) \n", bcm_errmsg(rv));
           return rv;
       }
       gtf_id=tst_tx_ptr.gtf_id;
       printf("UPDATE_tst gtf %d ctf %d\n\n",tst_tx_ptr.gtf_id,tst_rx_ptr.ctf_id);
    }
   
    global_gtf_id = gtf_id;
    /*start to send lb/tst packet*/
    rv = oam_lb_tst_sat_start(unit, gtf_id);
    if (rv != BCM_E_NONE) {
          printf("(%s) \n", bcm_errmsg(rv));
          return rv;
    }
    
    return 0;
}

/**
 * The following demonstrates how to use the loss/delay_add() 
 * and related APIs over a server endpoint. 
 * 
 * @author sinai (05/01/2015)
 * 
 * @param server_unit 
 * @param client_unit 
 * @param server_endpoint 
 * @param client_endpoint 
 * 
 * @return int 
 */
int add_loss_delay_entries_to_server_endpoint(int server_unit, int client_unit, int server_endpoint, int client_endpoint) {
    bcm_oam_loss_t loss;
    bcm_oam_delay_t delay;
    int rv;

    bcm_oam_loss_t_init(&loss);
    bcm_oam_delay_t_init(&delay);

    /* First set up the server side: transmit LMMs/DMMs */
    delay.id = loss.id = server_endpoint;
    loss.period = BCM_OAM_ENDPOINT_CCM_PERIOD_100MS;
    delay.period = BCM_OAM_ENDPOINT_CCM_PERIOD_100MS;
    delay.timestamp_format = bcmOAMTimestampFormatIEEE1588v1; /*The only supported format*/
    loss.peer_da_mac_address[3] = delay.peer_da_mac_address[3] =  0xef;
    loss.peer_da_mac_address[1] = delay.peer_da_mac_address[1] = 0xab;
    /* use extended statistics*/
    loss.flags = BCM_OAM_LOSS_STATISTICS_EXTENDED;
    loss.flags = BCM_OAM_LOSS_SINGLE_ENDED; /* LMM based loss management*/

    rv = bcm_oam_loss_add(server_unit,&loss);
    if (rv != BCM_E_NONE) {
        printf("(%s) \n", bcm_errmsg(rv));
        return rv;
    }

    rv = bcm_oam_delay_add(server_unit,&delay);
    if (rv != BCM_E_NONE) {
        printf("(%s) \n", bcm_errmsg(rv));
        return rv;
    }

    /* Now the client side: redirect LMMs/DMMs/LMRs/DMRs to the remote OAMP*/
    bcm_oam_loss_t_init(&loss);
    bcm_oam_delay_t_init(&delay);
    delay.id = loss.id = client_endpoint; /* Only the endpoint is needed.*/
    loss.flags = BCM_OAM_LOSS_SINGLE_ENDED;
    rv = bcm_oam_loss_add(client_unit,&loss);
    if (rv != BCM_E_NONE) {
        printf("(%s) \n", bcm_errmsg(rv));
        return rv;
    }

    rv = bcm_oam_delay_add(client_unit,&delay);
    if (rv != BCM_E_NONE) {
        printf("(%s) \n", bcm_errmsg(rv));
        return rv;
    }
    return 0;
}


/**
 * Initialize vswitch and mac entries, oam group. After calling 
 * this function create_acc_endpoint() may be called. 
 * 
 * @author sinai (23/12/2013)
 * 
 * @param unit 
 * 
 * @return int - success or fail.
 */
int initialize_vswitch_and_oam(int unit) {
    bcm_error_t rv;


    rv = get_device_type(unit, &device_type);
    if (rv < 0) {
        printf("Error checking whether the device is arad+.\n");
        print rv;
        return rv;
    }

    rv = create_vswitch_and_mac_entries(unit);
    if (rv != BCM_E_NONE) {
        printf("(%s) \n",bcm_errmsg(rv));
        return rv;
    }

    bcm_oam_group_info_t_init(&group_info_short_ma);
    sal_memcpy(group_info_short_ma.name, short_name, BCM_OAM_GROUP_NAME_LENGTH);
    rv = bcm_oam_group_create(unit, &group_info_short_ma);
    if (rv != BCM_E_NONE) {
        printf("(%s) \n",bcm_errmsg(rv));
        return rv;
    }

    return 0;

}

int lm_counter_base_ids[3];
int first_call=1;

/**
 *  Function creates an accelerated endpoint with a matching 
 * remote endpoint. 
 * Function is limited to 6 meps ( 2 MEPs per LIF, each with 
 * different direction and level) 
 * 
 * @author sinai (09/09/2013)
 * 
 * @param unit 
 * @param local_ep 
 * @param remote_ep (optional - may be NULL)
 * @param is_up signifies whether an up-mep or down-mep is 
 *               requested.
 * @param group_id 
 * 
 * @return int 
 */
int create_acc_endpoint(int unit, bcm_oam_endpoint_info_t *local_ep, bcm_oam_endpoint_info_t *remote_ep, int is_up, int group_id) {
    bcm_error_t rv;
    bcm_gport_t gportss[3];
    int local_ports[3] = { port_1, port_2, port_3 };
    static int src_mac_lsb =0;
    ++src_mac_lsb;

    gportss[0] = gport1;
    gportss[1] = gport2;
    gportss[2] = gport3;

	if (first_call) {
          /*Endpoints will be created on 3 different LIFs. Allocate 3 different counters */
		rv = set_counter_source_and_engines(unit, &lm_counter_base_ids[0],port_1);
		BCM_IF_ERROR_RETURN(rv);
		rv = set_counter_source_and_engines(unit, &lm_counter_base_ids[1],port_2);
		BCM_IF_ERROR_RETURN(rv); 
        rv = set_counter_source_and_engines(unit, &lm_counter_base_ids[2],port_3);
        BCM_IF_ERROR_RETURN(rv);
        first_call=0; 
    }

    bcm_oam_endpoint_info_t_init(local_ep);
    /*TX*/
    local_ep->type = bcmOAMEndpointTypeEthernet;
    local_ep->group = group_id;
    local_ep->level = counter % 7 + 1;
    local_ep->name = 123 + counter;
    local_ep->ccm_period = BCM_OAM_ENDPOINT_CCM_PERIOD_10MS;
    local_ep->id = mep_id;
   /* Time stamp format 1588:    otherwise delay_add() cannot be called for this endpoint.*/
    local_ep->timestamp_format = bcmOAMTimestampFormatIEEE1588v1; 

    if (is_up) {
        local_ep->flags |= BCM_OAM_ENDPOINT_UP_FACING;
    } else {
        BCM_GPORT_SYSTEM_PORT_ID_SET(local_ep->tx_gport, local_ports[counter % 3]);
    }

    local_ep->flags |= BCM_OAM_ENDPOINT_WITH_ID;
    local_ep->opcode_flags |= BCM_OAM_OPCODE_CCM_IN_HW;
    local_ep->timestamp_format = bcmOAMTimestampFormatIEEE1588v1; 


    local_ep->vlan = 5;
    local_ep->pkt_pri = local_ep->pkt_pri = 0 +(2<<1);  /*dei(1bit) + (pcp(3bit) << 1) */
    local_ep->outer_tpid = 0x8100;
    
    if(lb_pcp_dei_enable){ /* use to test different pkt pri with LB*/
        local_ep->inner_vlan =20;
        local_ep->inner_pkt_pri =  (7<<1) + 0;
        local_ep->inner_tpid = 0x9100;
    }
    else{
        local_ep->inner_vlan = 0;
        local_ep->inner_pkt_pri = 0;
        local_ep->inner_tpid = 0;
    }
    local_ep->int_pri = 3 +(1<<2);


    /* The MAC address that the CCM packets are sent with. */
    /* src_mac_address constraints: Only apply to Arad+*/
    if (device_type<device_type_jericho) {
        if (is_up) {
            /* For Up MEPs the LSB of the src MAC address must be equal to the local port*/
            src_mac[5] =src_mac_lsb;
        } else {
            /* For Down MEPs the limitation is that MEPs using different system ports (encoded in tx_gport) must have distinct LSBs in the src MAC address.*/
            src_mac[5] = local_ports[counter % 3];
        }
    }
   sal_memcpy(local_ep->src_mac_address, src_mac, 6);
/*     RX*/
    local_ep->gport = gportss[counter % 3];
    if(lb_tst_enable == 1){
       sal_memcpy(local_ep->dst_mac_address, src_mac, 6);
    }
    else{
       sal_memcpy(local_ep->dst_mac_address, mac_mep_2, 6);
    }
	local_ep->lm_counter_base_id = lm_counter_base_ids[counter % 3]; /* Endpoints on the same LIF will use the same counter base id.*/

	if (is_up) {
		local_ep->src_mac_address[5] = src_mac_lsb;
	}


    printf("Creating accelerated EP.\n");
    rv = bcm_oam_endpoint_create(unit, local_ep);
    if (rv != BCM_E_NONE) {
        printf("Failed at the %d-th attempt: (%s) \n", counter, bcm_errmsg(rv));

        return rv;
    }
    printf("created MEP with id %d\n", local_ep->id);


    if (remote_ep) {
        /*
        * Adding Remote MEP
        */
        bcm_oam_endpoint_info_t_init(remote_ep);
        remote_ep->name = 0xff;
        remote_ep->local_id = local_ep->id;
        remote_ep->type = bcmOAMEndpointTypeEthernet;
        remote_ep->ccm_period = 0;
        remote_ep->flags |= BCM_OAM_ENDPOINT_REMOTE | BCM_OAM_ENDPOINT_WITH_ID;
        remote_ep->loc_clear_threshold = 1;
        remote_ep->id = local_ep->id;

        printf("bcm_oam_endpoint_create RMEP\n");
        rv = bcm_oam_endpoint_create(unit, remote_ep);
        if (rv != BCM_E_NONE) {
            printf("(%s) \n", bcm_errmsg(rv));
            return rv;
        }
        printf("created RMEP with id %d\n", remote_ep->id);

    }

    ++counter; 
    return 0; 
}

/**
 * Example of using PCP based loss measurement endpoint. At the 
 * egress the mapping must be configured manually, by 
 * outlif profile and the int_pri. At the ingress the PCP can 
 * either be taken from the VLAN PCP or the int_pri as well. 
 * 
 * @author sinai (12/10/2014)
 * 
 * @param unit 
 * @param port1 
 * @param port2 
 * @param port3 
 * 
 * @return int 
 */
int oam_pcp_example(int unit, int port1, int port2, int port3) {
    bcm_oam_endpoint_info_t ep;
    bcm_qos_map_t oam_pcp_qos_map;
    int     rv;
    int     oam_pcp_qos_map_id;
    int     egress_tc;
    int     up_mep_lm_counter_base , down_mep_lm_counter_base;
    uint64  arg;

    single_vlan_tag = 1;
    port_1 = port1;
    port_2 = port2;
    port_3 = port3;

    bcm_oam_endpoint_info_t_init(&ep);

/* When using PCP the 4 LSBs of the counter must be 0. The following cints return an appropriate counter base.
   16 counters are needed per MEP: 8 for each direction.*/
    rv = set_counter_source_and_engines(unit,&up_mep_lm_counter_base,port2);
    BCM_IF_ERROR_RETURN(rv);
    rv = set_counter_source_and_engines(unit,&down_mep_lm_counter_base,port1);
    BCM_IF_ERROR_RETURN(rv);

    /* Config Range to : Min=0, Max=0 . That allows the counters to be set */
    COMPILER_64_ZERO(arg);
    rv = bcm_oam_control_set(unit, bcmOamControlLmPcpCounterRangeMin, arg);
    BCM_IF_ERROR_RETURN(rv);
    rv = bcm_oam_control_set(unit, bcmOamControlLmPcpCounterRangeMax, arg);
    BCM_IF_ERROR_RETURN(rv);

    rv = initialize_vswitch_and_oam(unit);
    if (rv != BCM_E_NONE) {
        printf("(%s) \n", bcm_errmsg(rv));
        return rv;
    }

    if (device_type < device_type_jericho) {
        printf("Cint only works for Jericho and above.\n");
        return 21;
    }


    bcm_qos_map_t_init(&oam_pcp_qos_map);

    oam_pcp_qos_map_id = 0; /* Use OAM=outlif-profile 0 (Only available value in Jericho A0. In Jericho B and above this may be 1.)*/

    rv = bcm_qos_map_create(unit, BCM_QOS_MAP_EGRESS | BCM_QOS_MAP_OAM_PCP, &oam_pcp_qos_map_id);
    if (rv != BCM_E_NONE) {
        printf("(%s) \n", bcm_errmsg(rv));
        return rv;
    }

    for (egress_tc = 0; egress_tc < 8; egress_tc++) {

        oam_pcp_qos_map.int_pri = egress_tc;
        oam_pcp_qos_map.pkt_pri = egress_tc; /* OAM-PCP taken from this value. Use a 1:1 mapping in this case from the TC to the OAM-PCP.*/

        rv = bcm_qos_map_add(unit, 0, &oam_pcp_qos_map, oam_pcp_qos_map_id);
        if (rv != BCM_E_NONE) {
            printf("(%s) \n", bcm_errmsg(rv));
            return rv;
        }
    }

    /* Set the OAM-outlif-profile on the up MEP's gport.*/
    rv = bcm_qos_port_map_set(unit, gport2, -1 /* ingress map */, oam_pcp_qos_map_id);
    if (rv != BCM_E_NONE) {
        printf("(%s) \n", bcm_errmsg(rv));
        return rv;
    }


    /* Create up MEP.*/
    bcm_oam_endpoint_info_t_init(&ep);
    ep.gport = gport2;
    ep.type = bcmOAMEndpointTypeEthernet;
    ep.level = 3;
    ep.group = group_info_short_ma.id;
    sal_memcpy(ep.dst_mac_address, mac_mep_2, 6);
    ep.lm_counter_base_id = up_mep_lm_counter_base;

    ep.lm_flags = BCM_OAM_LM_PCP;
    ep.flags = BCM_OAM_ENDPOINT_UP_FACING;

    rv = bcm_oam_endpoint_create(unit, &ep);
    if (rv != BCM_E_NONE) {
        printf("(%s) \n", bcm_errmsg(rv));
        return rv;
    }


    if (device_type == device_type_jericho || oam_pcp_example_use_slm) {
        /* For Jericho A0 PCP based counting is only supported for SLMs*/
        rv = slm_set(unit, ep.id);
        BCM_IF_ERROR_RETURN(rv);
    }

    if (!soc_property_get(unit, "oam_pcp_value_extract_from_packet", 1)) {
        /* In this case the PCP is taken from the COS-profile*/
        int packet_pcp;
        int packet_dei;
        bcm_qos_map_t_init(&oam_pcp_qos_map);


        oam_pcp_qos_map_id = 1; /* use QoS map profile 1 in this LIF*/
        rv = bcm_qos_map_create(unit, BCM_QOS_MAP_INGRESS, &oam_pcp_qos_map_id);
        if (rv != BCM_E_NONE) {
            printf("(%s) \n", bcm_errmsg(rv));
            return rv;
        }
        for (packet_dei = 0; packet_dei < 2  /*1 bit value */; ++packet_dei) {
            for (packet_pcp = 0; packet_pcp < 0x8; ++packet_pcp) {
                /* Iterate through all possible PCP, DEI values on the vlan*/
                oam_pcp_qos_map.pkt_pri = packet_pcp;
                oam_pcp_qos_map.int_pri = packet_pcp  ; /* OAM-PCP taken from this value. Any mapping may be set here.*/
                oam_pcp_qos_map.pkt_cfi = packet_dei;

                rv = bcm_qos_map_add(unit, BCM_QOS_MAP_L2, &oam_pcp_qos_map, oam_pcp_qos_map_id);
                if (rv != BCM_E_NONE) {
                    printf("(%s) \n", bcm_errmsg(rv));
                    return rv;
                }
            }
        }
        /* Associate this Qos map profile with the  down MEP's gport.*/
        rv = bcm_qos_port_map_set(unit, gport1, oam_pcp_qos_map_id, -1 /* egress map*/);
        if (rv != BCM_E_NONE) {
            printf("(%s) \n", bcm_errmsg(rv));
            return rv;
        }
    }
    /* Added for SDK-103314 to get id of mep */
    ep2.id = ep.id;

    /* Create down Mep*/
    bcm_oam_endpoint_info_t_init(&ep);
    ep.type = bcmOAMEndpointTypeEthernet;
    ep.level = 2;
    ep.gport = gport1;
    sal_memcpy(ep.dst_mac_address, mac_mep_1, 6);
    ep.lm_counter_base_id = down_mep_lm_counter_base;
    ep.lm_flags = BCM_OAM_LM_PCP;

   
    rv = bcm_oam_endpoint_create(unit, &ep);
    if (rv != BCM_E_NONE) {
        printf("(%s) \n", bcm_errmsg(rv));
        return rv;
    }
    /* Added for SDK-103314 to get id of mep */
    ep3.id = ep.id;

    if (device_type == device_type_jericho || oam_pcp_example_use_slm) {
        /* For Jericho A0 PCP based counting is only supported for SLMs*/
        rv = slm_set(unit, ep.id);
        BCM_IF_ERROR_RETURN(rv);
    }

    return 0;
}
