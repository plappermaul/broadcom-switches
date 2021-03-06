/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

/*
 * Arad+ based GPON16 Application supports 4 GPON PHY ports (0-3). Each GPON PHY port
 * supports 4 channels. The channels are signaled in the GPON TID:
 * |1|0|9|8|7|6|5|4|3|2|1|0|9|8|7|6|5|4|3|2|1|0|9|8|7|6|5|4|3|2|1|0|
 * |     reserved        | PCP |Chn| rsvd  |       GEM-ID          |
 * 
 * For compatibility with the existing xPON application, the PP/TM Ports will be offset from the
 * PHY port base by 8. Ports 0-127 are reserved for xPON. Ports 128+ support all other NIF types
 *
 * The following entity TM-Port/PP-Port assignements will result from the GPON configuration. Vlan-Domain must
 * be assigned in the range 0-15.
 *
 * Note: The physical port is not channelized, therefore the Ingress TM port is the TM port of channel 0.
 *
 *                         PP-Port/
 * Arad+:
 * In-PP-Port/Out-PP-Port/OTM-Port = GPON-Chan << 3 | PHY-Port
 * Vlan-Domain = Phy-Port << 2 | Phy-Chan
 *
 * Phy-port     GPON-Chan  TM-Port    Vlan-Domain
 * -----------  ---------  ---------  -----------
 * 0            0          0          0         
 * 0            1          8          1        
 * 0            2          16         2       
 * 0            3          24         3      
 * 1            0          1          4     
 * 1            1          9          5    
 * 1            2          17         6          
 * 1            3          25         7         
 * 2            0          2          8        
 * 2            1          10         9       
 * 2            2          18         10     
 * 2            3          26         11    
 * 3            0          3          12
 * 3            1          11         13
 * 3            2          19         14
 * 3            3          27         15
 * 
 * Jericho (PTC size: 4):
 * In-PP-Port/Out-PP-Port/OTM-Port = GPON-Chan << 4 | PHY-Port
 * Vlan-Domain = Phy-Port << 2 | Phy-Chan
 *
 * Phy-port     GPON-Chan  TM-Port    Vlan-Domain
 * -----------  ---------  ---------  -----------
 * 0            0          0          0
 * 0            1          16         1
 * 0            2          32         2
 * 0            3          48         3
 * 0            0          1          4
 * 0            1          17         5
 * 0            2          33         6
 * 0            3          49         7
 * 0            0          2          8
 * 0            1          18         9
 * 0            2          34         10
 * 0            3          50         11
 * 0            0          3          12
 * 0            1          19         13
 * 0            2          35         14
 * 0            3          51         15
 * 0            0          4          16
 * 0            1          20         17
 * 0            2          36         18
 * 0            3          52         19
 * 0            0          5          20
 * 0            1          21         21
 * 0            2          37         22
 * 0            3          53         23
 * 0            0          6          24
 * 0            1          22         25
 * 0            2          38         26
 * 0            3          54         27
 * 0            0          7          28
 * 0            1          23         29
 * 0            2          39         30
 * 0            3          55         31
 * 0            0          8          32
 * 0            1          24         33
 * 0            2          40         34
 * 0            3          56         35
 * 0            0          9          36
 * 0            1          25         37
 * 0            2          41         38
 * 0            3          57         39
 * 0            0          10         40
 * 0            1          26         41
 * 0            2          42         42
 * 0            3          58         43
 * 0            0          11         44
 * 0            1          27         45
 * 0            2          43         46
 * 0            3          59         47
 * 0            0          12         48
 * 0            1          28         49
 * 0            2          44         50
 * 0            3          60         51
 * 0            0          13         52
 * 0            1          29         53
 * 0            2          45         54
 * 0            3          61         55
 * 0            0          14         56
 * 0            1          30         57
 * 0            2          46         58
 * 0            3          62         59
 * 0            0          15         60
 * 0            1          31         61
 * 0            2          47         62
 * 0            3          63         63
 *
 * In this example, GPON Phy Port 0 is configured with 4 channels.
 *
 * ucode_port_0.BCM88650=10GBase-R13.0
 * ucode_port_8.BCM88650=10GBase-R13.1
 * ucode_port_16.BCM88650=10GBase-R13.2
 * ucode_port_24.BCM88650=10GBase-R13.3
 * pon_application_support_enabled_0.BCM88650=TRUE
 * custom_feature_gpon_application_enable=1
 *
 * NNI ports start at 128:
 * ucode_port_128.BCM88650=10GBase-R12
 *
 * NOTE: config-sand.bcm configures port 0 as a CPU port. When configuring GPON port 0, 
 * be sure to modify the config.bcm file to assign a differnt port to CPU.0
 * For example, instead configure port 220 for CPU.0:
 *    from:
 *        ucode_port_0.BCM88650=CPU.0
 *        tm_port_header_type_in_0.BCM88650=INJECTED_2
 *        tm_port_header_type_out_0.BCM88650=TM
 *    to:
 *        ucode_port_220.BCM88650=CPU.0
 *        tm_port_header_type_in_220.BCM88650=INJECTED_2
 *        tm_port_header_type_out_220.BCM88650=TM
 *
 * To run the example:
 * 
 * cint ../../../../src/examples/dpp/cint_port_tpid.c
 * cint ../../../../src/examples/dpp/pon/cint_pon_utils.c
 * cint ../../../../src/examples/dpp/pon/cint_gpon_channelized_port.c
 * cint 
 * int  unit = 0;
 * bcm_port_t port_gpon = 0;
 * bcm_port_t port_nni = 128;
 * bcm_port_t port_gpon_pp = 16;
 * uint8 num_channels = 4;	
 * uint32 tunnel = 4095;
 * uint32 qtag = 100;
 * uint32 stag = 999;
 * 
 * print port_gpon_example(unit, port_gpon, num_channels, port_nni);
 * print port_gpon_service_create(unit, port_gpon_pp, tunnel, qtag, port_nni, stag); 
 * exit;
 *
 * Jericho (PTC size: 5):
 * In-PP-Port/Out-PP-Port/OTM-Port = GPON-Chan << 5 | PHY-Port
 * Vlan-Domain = Phy-Port << 2 | Phy-Chan
 *
 * config add ucode_port_0.0=CPU.0:core_0.0
 * config add ucode_port_32.0=CPU.1:core_0.32
 * config add ucode_port_64.0=CPU.2:core_0.64
 * config add ucode_port_96.0=CPU.3:core_0.96 
 * config add ucode_port_202.0=CPU.24:core_1.202

 * config add tm_port_header_type_in_0.0=INJECTED_2_PP
 * config add tm_port_header_type_out_0.0=ETH 
 * config add tm_port_header_type_in_32.0=INJECTED_2_PP
 * config add tm_port_header_type_out_32.0=ETH 
 * config add tm_port_header_type_in_64.0=INJECTED_2_PP
 * config add tm_port_header_type_out_64.0=ETH 
 * config add tm_port_header_type_in_96.0=INJECTED_2_PP
 * config add tm_port_header_type_out_96.0=ETH 

 * config add ucode_port_1.0=10GBase-R13.0:core_0.1
 * config add ucode_port_33.0=10GBase-R13.1:core_0.33
 * config add ucode_port_65.0=10GBase-R13.2:core_0.65
 * config add ucode_port_97.0=10GBase-R13.3:core_0.97 
 
 * config add ucode_port_2.0=10GBase-R14.0:core_0.2
 * config add ucode_port_34.0=10GBase-R14.1:core_0.34
 * config add ucode_port_66.0=10GBase-R14.2:core_0.66
 * config add ucode_port_98.0=10GBase-R14.3:core_0.98 
 
 * config add ucode_port_3.0=10GBase-R15.0:core_0.3
 * config add ucode_port_35.0=10GBase-R15.1:core_0.35
 * config add ucode_port_67.0=10GBase-R15.2:core_0.67
 * config add ucode_port_99.0=10GBase-R15.3:core_0.99 
 
 * config add ucode_port_4.0=10GBase-R16.0:core_0.4 
 * config add ucode_port_5.0=10GBase-R17.0:core_0.5 
 * config add ucode_port_6.0=10GBase-R18.0:core_0.6 
 * config add ucode_port_7.0=10GBase-R19.0:core_0.7
 * config add ucode_port_8.0=10GBase-R20.0:core_0.8
 * config add ucode_port_9.0=10GBase-R21.0:core_0.9
 * config add ucode_port_10.0=10GBase-R22.0:core_0.10
 * config add ucode_port_11.0=10GBase-R23.0:core_0.11
 * config add ucode_port_12.0=10GBase-R24.0:core_0.12
 * config add ucode_port_13.0=10GBase-R25.0:core_0.13
 * config add ucode_port_14.0=10GBase-R26.0:core_0.14
 * config add ucode_port_15.0=10GBase-R27.0:core_0.15
 * config add ucode_port_16.0=10GBase-R28.0:core_0.16
 * config add ucode_port_17.0=10GBase-R29.0:core_0.17
 * config add ucode_port_18.0=10GBase-R30.0:core_0.18
 * config add ucode_port_19.0=10GBase-R31.0:core_0.19

 
 * config add pon_application_support_enabled_0.0=TRUE 
 * config add pon_application_support_enabled_32.0=TRUE 
 * config add pon_application_support_enabled_64.0=TRUE 
 * config add pon_application_support_enabled_96.0=TRUE 
 * config add pon_application_support_enabled_1.0=TRUE 
 * config add pon_application_support_enabled_33.0=TRUE 
 * config add pon_application_support_enabled_65.0=TRUE 
 * config add pon_application_support_enabled_97.0=TRUE 
 * config add pon_application_support_enabled_2.0=TRUE 
 * config add pon_application_support_enabled_34.0=TRUE 
 * config add pon_application_support_enabled_66.0=TRUE 
 * config add pon_application_support_enabled_98.0=TRUE 
 * config add pon_application_support_enabled_3.0=TRUE 
 * config add pon_application_support_enabled_35.0=TRUE 
 * config add pon_application_support_enabled_67.0=TRUE 
 * config add pon_application_support_enabled_99.0=TRUE 

 * config add pon_application_support_enabled_4.0=TRUE 
 * config add pon_application_support_enabled_5.0=TRUE 
 * config add pon_application_support_enabled_6.0=TRUE 
 * config add pon_application_support_enabled_7.0=TRUE 
 * config add pon_application_support_enabled_8.0=TRUE 
 * config add pon_application_support_enabled_9.0=TRUE 
 * config add pon_application_support_enabled_10.0=TRUE 
 * config add pon_application_support_enabled_11.0=TRUE 
 * config add pon_application_support_enabled_12.0=TRUE 
 * config add pon_application_support_enabled_13.0=TRUE 
 * config add pon_application_support_enabled_14.0=TRUE 
 * config add pon_application_support_enabled_15.0=TRUE 
 * config add pon_application_support_enabled_16.0=TRUE 
 * config add pon_application_support_enabled_17.0=TRUE 
 * config add pon_application_support_enabled_18.0=TRUE 
 * config add pon_application_support_enabled_18.0=TRUE 
 * config add pon_application_support_enabled_20.0=TRUE     
 
 * config add custom_feature_gpon_application_enable.0=1
 */

struct gpon_service_info_s{
    bcm_port_t gpon_pp_port;
    bcm_port_t nni_port;
    int service_mode;   
    int up_lif_type;
    bcm_tunnel_id_t tunnel_id;
    bcm_vlan_t up_ovlan;
    bcm_vlan_t up_ivlan;
    int down_lif_type;
    bcm_vlan_t down_ovlan;
    bcm_vlan_t down_ivlan;
    bcm_gport_t pon_gport;
    bcm_gport_t nni_gport;
};

bcm_gport_t gpon_pp_port[4];
gpon_service_info_s gpon_1_1_service_info[3];

/* Set PON tunnel profile to ARAD */
int port_gpon_config(int unit, bcm_port_t port_gpon, uint8 num_gpon_channels)
{
    int rv = 0;
    uint8 channel;
    int j;
    uint32 flags;
    bcm_gport_t port_gpon_tm;
    uint32 port_gpon_vlan_domain;
    uint8 ptc_size;
    ptc_size = (is_device_or_above(unit, JERICHO) ? soc_property_get(unit,"custom_feature_pon_port_ptc_size", 4) : 3);

    printf("Entering port_gpon_config: port_gpon %d num_gpon_channels %d\n", port_gpon, num_gpon_channels);

    if (num_gpon_channels > 4) {
        printf("\nThe number of channels must be <= 4\n");
        return BCM_E_PARAM;
    }

    for (channel = 0; channel < num_gpon_channels; channel++) {
        /* Associate the PP port with the PON channel */
        flags = BCM_PORT_PON_TUNNEL_WITH_ID;
        gpon_pp_port[channel] = 0; /* Only the default profile is needed for each GPON port */
        port_gpon_tm =  channel << ptc_size | port_gpon;
	    printf("port_gpon_config: bcm_port_pon_tunnel_add port_gpon_tm %d\n", port_gpon_tm);
        rv = bcm_port_pon_tunnel_add(unit, port_gpon_tm, flags, &gpon_pp_port[channel]);
        if (rv != BCM_E_NONE)
        {    
            printf("Error, bcm_port_pon_tunnel_add!\n");
            print rv;
            return rv;
        }
        
        /* Map the {Phy-Port, TID:GPON-Port(2)} to the PP port */
        printf("port_gpon_config: bcm_port_pon_tunnel_map_set port_gpon %d channel %d gpon_pp_port %d\n", port_gpon,channel, gpon_pp_port[channel]);
        rv = bcm_port_pon_tunnel_map_set(unit, port_gpon, channel, gpon_pp_port[channel]);
        if (rv != BCM_E_NONE)
        {    
            printf("Error, bcm_port_pon_tunnel_map_set!\n");
            print rv;
            return rv;
        } 

        rv = pon_tpid_profile_init(unit, gpon_pp_port[channel]);
        if (rv != BCM_E_NONE)
        {
            printf("Error, pon_tpid_profile_init!\n");
            print rv;
            return rv;
        }
        
        if (soc_property_get(unit , "bcm886xx_vlan_translate_mode",0)) {
            /* In case of port tpid_profile != 0, need to init a VLAN action
             * to modify the tpid_profile of IVE to the correct vlaue
             */
            if ((outer_tpid != 0x8100) ||( (outer_tpid == 0x8100) && (inner_tpid != 0x9100))) {
                ing_vlan_action_only_modify_tpid_init(unit);
            }
            egr_vlan_action_no_op_init(unit);
        }

        /* Set PON PP PORT VLAN Domain */
        port_gpon_vlan_domain = gpon_pp_port[channel];
        bcm_port_class_set(unit, gpon_pp_port[channel], bcmPortClassId, port_gpon_vlan_domain);

        bcm_vlan_control_port_set(unit, gpon_pp_port[channel], bcmVlanPortLookupTunnelEnable, 1);

        printf("\nMapped GPON phy port %d, channel %d to TM port %d, PP port %d, vlan-domain %d\n",port_gpon, channel, port_gpon_tm, gpon_pp_port[channel], port_gpon_vlan_domain);

    }
    
    return rv;
}

int port_gpon_example(int unit, bcm_port_t port_gpon, uint8 num_channels, bcm_port_t port_nni)
{
    int rv;

    rv = pon_app_init(unit, port_gpon, port_nni, 0, 0);
    if (rv != BCM_E_NONE)
    {
        printf("Error, pon_app_init!\n");
        print rv;
        return rv;
    }

    rv = port_gpon_config(unit, port_gpon, num_channels);
    if (rv != BCM_E_NONE)
    {
        printf("Error, port_gpon_config!\n");
        print rv;
        return rv;
    }

    return rv;
}

int port_gpon_service_create(int unit, 
                 bcm_port_t port_gpon_pp, 
                 uint32 tunnel,
                 uint32 qtag,
                 bcm_port_t port_nni,
                 uint32 stag,
                 int first_time)
{

    bcm_gport_t gpon_vlan_port_id, gpon_tls_port_id, nni_vlan_port_id;
    bcm_if_t encap_id;
    bcm_vlan_t vsi = 5000;

    int rv = BCM_E_NONE;

    /* 
     * Create GPON AC-LIF
     * Service classification: {GPON-Port(4), GEM-ID(12), QTAG(12)}
     * GPON-Port is obtained from the Vlan Domain assigned to the GPON PP Port
     */
    gpon_vlan_port_id = 0;	/* Allow the SDK to allocate the LIF-Id */
    rv =  pon_lif_create(unit, port_gpon_pp, match_otag, 0, 0, tunnel, tunnel, qtag, 0, 0, 0, &gpon_vlan_port_id);
    if (rv != BCM_E_NONE) {
        printf("Error, pon_lif_create!\n");
        print rv;
        return rv;
    }
    else {
        printf("Created gpon vlan-port 0x%08x pp-port %d tunnel %d qtag %d\n", gpon_vlan_port_id, port_gpon_pp, tunnel, qtag);
    }

    gpon_tls_port_id = 0;
    rv = pon_lif_create(unit, port_gpon_pp, match_all_tags, 0, 0, tunnel, tunnel, 0, 0, 0, 0, &gpon_tls_port_id);
    if (rv != BCM_E_NONE) {
        printf("Error, pon_lif_create!\n");
        print rv;
        return rv;
    } else if (rv == BCM_E_EXISTS) {
        printf("TLS vlan-port 0x%08x already exists for gpon port %d tunnel %d\n", gpon_tls_port_id, port_gpon_pp, tunnel);
    }
    else {
        printf("Created gpon vlan-port 0x%08x pp-port %d tunnel %d all-tags\n", gpon_tls_port_id, port_gpon_pp, tunnel);
    }


    /*
     * Ingress Vlan Editing (IVE)
     * Replace outer tag
     */
    rv =  pon_port_ingress_vt_set(unit, otag_to_otag2, stag, 0, gpon_vlan_port_id, 0, NULL);
    if (rv != BCM_E_NONE) {
        printf("Error, pon_port_ingress_vt_set!\n");
        print rv;
        return rv;
    }
    else {
        printf("Configured IVE gpon vlan-port 0x%08x stag %d\n", gpon_vlan_port_id, stag);
    }

    rv = pon_port_egress_vt_set(unit, otag_to_otag2, tunnel, qtag, 0, gpon_vlan_port_id);
    if (rv != BCM_E_NONE) {
        printf("Error, pon_port_egress_vt_set!\n");
        print rv;
        return rv;
    }
    else {
        printf("Configured EVE gpon vlan-port 0x%08x tunnel %d qtag %d\n", gpon_vlan_port_id, tunnel, qtag);
    }

    rv =  pon_port_ingress_vt_set(unit, otag_to_otag2, stag, 0, gpon_tls_port_id, 0, NULL);
    if (rv != BCM_E_NONE) {
        printf("Error, pon_port_ingress_vt_set!\n");
        print rv;
        return rv;
    }
    else {
        printf("Configured IVE gpon vlan-port 0x%08x stag %d\n", gpon_tls_port_id, stag);
    }

    rv = pon_port_egress_vt_set(unit, otag_to_otag2, tunnel, qtag, 0, gpon_tls_port_id);
    if (rv != BCM_E_NONE) {
        printf("Error, pon_port_egress_vt_set!\n");
        print rv;
        return rv;
    }
    else {
        printf("Configured EVE gpon vlan-port 0x%08x tunnel %d qtag %d\n", gpon_tls_port_id, tunnel, qtag);
    }

    /*
     * Create NNI AC-LIF
     */
     nni_vlan_port_id = 0;	/* Allow the SDK to allocate the LIF-Id */
     rv = nni_lif_create(unit, port_nni, match_otag, 0, stag, 0, 0, &nni_vlan_port_id, &encap_id);
     if (rv != BCM_E_NONE) {
         printf("Error, pon_lif_create!\n");
         print rv;
         return rv;
     }
     else {
         printf("Created nni vlan-port 0x%08x phy-port %d stag %d\n", nni_vlan_port_id, port_nni, stag);
       }

    if (first_time) {
        rv = bcm_vswitch_create_with_id(0, vsi);
        if (rv != BCM_E_NONE) {
            printf("Error, bcm_vswitch_create_with_id!\n");
            print rv;
            return rv;
        } else {
            printf("Create vswitch %d\n", vsi);
        }
    }

    rv = bcm_vswitch_port_add(unit, vsi, gpon_vlan_port_id);
    if (rv != BCM_E_NONE) {
        printf("Error, bcm_vswitch_port_add!\n");
        print rv;
        return rv;
    } else {
        printf("Added gpon_vlan_port_id 0x%08x to vsi %d \n", gpon_vlan_port_id, vsi);
    }

    rv = bcm_vswitch_port_add(unit, vsi, gpon_tls_port_id);
    if (rv != BCM_E_NONE) {
        printf("Error, bcm_vswitch_port_add!\n");
        print rv;
        return rv;
    } else {
        printf("Added gpon_vlan_port_id 0x%08x to vsi %d \n", gpon_vlan_port_id, vsi);
    }

    rv = bcm_vswitch_port_add(unit, vsi, nni_vlan_port_id);
    if (rv != BCM_E_NONE) {
        printf("Error, bcm_vswitch_port_add!\n");
        print rv;
        return rv;
    } else {
        printf("Added nni_vlan_port_id 0x%08x to vsi %d \n", gpon_vlan_port_id, vsi);
    }

    return rv;
}

/*
 *Initialize the service models and set up GPON application configurations.
 */
int gpon_service_init(int unit, bcm_port_t port_pon, int num_channels, bcm_port_t port_nni)
{
    int rv = 0;
    int index;
    int tunnel_base = 0;

    rv = port_gpon_example(unit, port_pon, num_channels, port_nni);
    if (rv != BCM_E_NONE) {
        printf("Error, port_gpon_example\n");
        print rv;
        return rv;
    }

    /*1:1 services*/
    /* PON Port 5 Tunnel-ID 1000 CVLAN 20 <-CrossConnect-> NNI Port 129 CVLAN 200*/
    index = 0;
    gpon_1_1_service_info[index].gpon_pp_port = gpon_pp_port[0];
    gpon_1_1_service_info[index].nni_port = port_nni;
    gpon_1_1_service_info[index].service_mode = otag_to_otag2;
    gpon_1_1_service_info[index].tunnel_id  = tunnel_base+1000;
    gpon_1_1_service_info[index].up_lif_type = match_otag;
    gpon_1_1_service_info[index].up_ovlan   = 20;
    gpon_1_1_service_info[index].down_lif_type = match_otag;
    gpon_1_1_service_info[index].down_ovlan = 200;

    /* PON Port 5 Tunnel-ID 1000 CVLAN 20 <-CrossConnect-> NNI Port 129 CVLAN 200*/
    index++;
    gpon_1_1_service_info[index].gpon_pp_port = gpon_pp_port[3];
    gpon_1_1_service_info[index].nni_port = port_nni;
    gpon_1_1_service_info[index].service_mode = otag_to_otag2;
    gpon_1_1_service_info[index].tunnel_id  = tunnel_base+1001;
    gpon_1_1_service_info[index].up_lif_type = match_otag;
    gpon_1_1_service_info[index].up_ovlan   = 30;
    gpon_1_1_service_info[index].down_lif_type = match_otag;
    gpon_1_1_service_info[index].down_ovlan = 300;

    /* PON Port 5 Tunnel-ID 1001 CVLAN 40 CVLAN 41 <-CrossConnect-> NNI Port 129 SVLAN 400 CVLAN 410*/
    index++;
    gpon_1_1_service_info[index].gpon_pp_port = gpon_pp_port[1];
    gpon_1_1_service_info[index].nni_port = port_nni;
    gpon_1_1_service_info[index].service_mode = o_i_tag_to_o2_i2_tag;
    gpon_1_1_service_info[index].tunnel_id  = tunnel_base+1002;
    gpon_1_1_service_info[index].up_lif_type = match_t_o_i_tag;
    gpon_1_1_service_info[index].up_ovlan   = 40;
    gpon_1_1_service_info[index].up_ivlan   = 41;
    gpon_1_1_service_info[index].down_lif_type = match_o_i_tag;
    gpon_1_1_service_info[index].down_ovlan = 400;
    gpon_1_1_service_info[index].down_ivlan = 410;

    return 0;
}

int gpon_1_1_service(int unit, int index)
{
    int rv = BCM_E_NONE;
    bcm_port_t pon_port;
    bcm_port_t nni_port;
    int service_mode;
    bcm_tunnel_id_t tunnel_id;
    bcm_vlan_t up_ovlan;
    bcm_vlan_t up_ivlan;
    bcm_vlan_t down_ovlan;
    bcm_vlan_t down_ivlan;
    bcm_if_t encap_id;
    bcm_gport_t pon_gport, nni_gport;
    int pon_lif_type, nni_lif_type;
    bcm_vlan_action_set_t action;
    bcm_vswitch_cross_connect_t cross_connect_gport;
    
    pon_port     = gpon_1_1_service_info[index].gpon_pp_port;
    nni_port     = gpon_1_1_service_info[index].nni_port;
    service_mode = gpon_1_1_service_info[index].service_mode;
    tunnel_id    = gpon_1_1_service_info[index].tunnel_id;
    pon_lif_type = gpon_1_1_service_info[index].up_lif_type;
    up_ovlan     = gpon_1_1_service_info[index].up_ovlan;
    up_ivlan     = gpon_1_1_service_info[index].up_ivlan;
    nni_lif_type = gpon_1_1_service_info[index].down_lif_type;
    down_ovlan   = gpon_1_1_service_info[index].down_ovlan;
    down_ivlan   = gpon_1_1_service_info[index].down_ivlan;

    /* Create GPON AC-LIF  */
    rv =  pon_lif_create(unit, pon_port, pon_lif_type, 0, 0, tunnel_id, tunnel_id, up_ovlan, up_ivlan, 0, 0, &pon_gport);
    if (rv != BCM_E_NONE) {
        printf("Error, pon_lif_create!\n");
        print rv;
        return rv;
    }
    else {
        printf("Created gpon vlan-port:0x%08x, pp-port:%d, tunnel_id:%d, up_ovlan:%d, up_ivlan:%d\n", pon_gport, pon_port, tunnel_id, up_ovlan, up_ivlan);
    }


    /* Set PON LIF ingress VLAN editor */
    rv =  pon_port_ingress_vt_set(unit, service_mode, down_ovlan, down_ivlan, pon_gport, 0, NULL);
    if (rv != BCM_E_NONE) {
        printf("Error, pon_port_ingress_vt_set!\n");
        print rv;
        return rv;
    }
    else {
        printf("Configured IVE gpon vlan-port:0x%08x, down_ovlan:%d, down_ivlan:%d\n", pon_gport, down_ovlan, down_ivlan);
    }

    /* Set PON LIF egress VLAN editor */
    rv = pon_port_egress_vt_set(unit, service_mode, tunnel_id, up_ovlan, up_ivlan, pon_gport);
    if (rv != BCM_E_NONE) {
        printf("Error, pon_port_egress_vt_set!\n");
        print rv;
        return rv;
    }
    else {
        printf("Configured EVE gpon vlan-port:0x%08x, tunnel_id:%d, up_ovlan:%d, up_ivlan:%d\n", pon_gport, tunnel_id, up_ovlan, up_ivlan);
    }

    /* Create NNI AC-LIF  */
    rv = nni_lif_create(unit, nni_port, nni_lif_type, 0, down_ovlan, down_ivlan, 0, &nni_gport, &encap_id);
    if (rv != BCM_E_NONE) {
        printf("Error, pon_lif_create!\n");
        print rv;
        return rv;
    }
    else {
        printf("Created nni vlan-port:0x%08x, phy-port:%d, down_ovlan:%d, down_ivlan:%d\n", nni_gport, nni_port, down_ovlan, down_ivlan);
    }

    gpon_1_1_service_info[index].pon_gport = pon_gport;
    gpon_1_1_service_info[index].nni_gport = nni_gport;

    /* Cross connect the 2 LIFs */
    bcm_vswitch_cross_connect_t_init(&cross_connect_gport);
    cross_connect_gport.port1= pon_gport;
    cross_connect_gport.port2= nni_gport;
    rv = bcm_vswitch_cross_connect_add(unit, &cross_connect_gport);
    if (rv != BCM_E_NONE) {
        printf("Error, bcm_vswitch_cross_connect_add\n");
        print rv;
        return rv;
    }

    return rv;
}

