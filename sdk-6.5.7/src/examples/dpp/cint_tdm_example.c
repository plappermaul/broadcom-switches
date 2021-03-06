/*~~~~~~~~~~~~~~~~~~~~~~~~~~OTN/TDM Applications~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/* $Id: cint_tdm_example.c,v 1.7 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$ 
 *  
 * File: cint_tdm_example.c 
 * Purpose: Example shows setup configuration of TDM standard and optimize modes, with and without inner FTMH header. 
 *  
 * These modes are for devices that are dedicated to OTN/TDM traffic only.
 *
 * Application Sequence:
 *  1.  Set cell size constraints.
 *  2.  For a TDM port, configure the port editing settings to enable the following actions:
 *      o    At the ingress, the FTMH header is generated by the device itself. 
 *      o    At the egress, the FTMH header is removed.
 * 
 * For Arad the CINT consists of two applications: TDM standard and TDM optimize.
 *  o   TDM standard/optimize with inner FTMH header:
 *      o   Set the cell size range to min_size-max_size.
 *      o   Configure the port editing settings to enable the following actions:
 *          o    At the ingress: appending FTMH with a unicast/multicast destination 
 *               (user settings) and user defined fields.
 *          o    At the egress: removing FTMH.
 *      o   To run application:
 *          arad_tdm_example (unit,incoming_port_id, is_multicast, is_optimize_mode ,min_size, max_size)
 *      o   Run traffic from the incoming port. Note that the packet is considered to be payload only,
 *          since the forwarding decision is performed via the appended FTMH.
 *      o   You can configure different settings for the port editing, including a different 
 *          destination port, multicast group settings, FTMH actions, and user defined fields.
 *      Note: Before running the following TDM standard application, be sure the device is configured for
 *      TDM Standard/optimize. in add config-sand:
 *          o   fap_tdm_bypass.BCM88650_A0=TDM_OPTIMIZED/fap_tdm_bypass.BCM88650=TDM_STANDARD
 *          o   tm_port_header_type_<incoming_port_id>.BCM88650=TDM (for example tm_port_header_type_13.BCM88650=TDM)
 *  o   TDM standard/optimize with no inner FTMH header:
 *      o   Set the cell size range to min_size-max_size.
 *      o   Configure the port editing settings to disable the adding and removing FTMH headers at the ingress and egress.
 *      o   To run application:
 *          arad_tdm_outter_header_example (unit,incoming_port_id, is_multicast, in_size, max_size)
 *      o   Run traffic from the incoming port. Note that the packet is not considered to be payload only,
 *          since the forwarding decision is disabled via the appended FTMH.
 *      o   the packet header should contain the appropriate data:
 *              Uniticast Standard FTMH: DESTINATION_FAP_ID[3:13] OUT_FAP_PORT[33:40] Type(UC/MC)[48:48]
 *              Multicast Standard FTMH: DESTINATION_FAP_ID[3:13] OUT_FAP_PORT[49:56] Type(UC/MC)[48:48] Internaluse[41:49] must be define to 11111111
 *              Uniticast Optimize FTMH  DESTINATION_FAP_ID[13:23] OUT_FAP_PORT [24:31] Type(UC/MC)[8:8] 
 *              Multicast Optimize FTMH  MC-ID[13:31] Type(UC/MC)[8:8]
 *      o   You can configure different settings for the port editing, including a different 
 *          destination port, multicast group settings, FTMH actions, and user defined fields.
 *      Note: Before running the following TDM standard application, be sure the device is configured for
 *      TDM Standard/optimize. in add config-sand:
 *          o   fap_tdm_bypass.BCM88650_A0=TDM_OPTIMIZED/fap_tdm_bypass.BCM88650=TDM_STANDARD
 *          o   tm_port_header_type_<incoming_port_id>.BCM88650=TDM (for example tm_port_header_type_13.BCM88650=TDM) 
 * For Petra the CINT consists of two applications: TDM standard and TDM optimize. 
 *  o   TDM standard:
 *      o   Set the cell size range to 70-120.
 *      o   Configure the port editing settings to enable the following actions:
 *          o    At the ingress: appending FTMH with a unicast/multicast destination 
 *               (user settings) and user defined fields.
 *          o    At the egress: removing FTMH.
 *      o   To run application:
 *          petra_std_tdm_example (unit, incoming_port_id, is_multicast)
 *      o   Run traffic from the incoming port. Note that the packet is considered to be payload only,
 *          since the forwarding decision is performed via the appended FTMH.
 *      o   You can configure different settings for the port editing, including a different 
 *          destination port, multicast group settings, FTMH actions, and user defined fields.
 *      Note: Before running the following TDM standard application, be sure the device is configured for
 *      TDM Standard in dune.soc: Uncomment tdm=1
 * 
 *  o   TDM optimize:
 *      Note: Before running the following TDM optimize application, be sure the device is configured for
 *      TDM Optimize in dune.soc: Uncomment tdm=1 and add fap_device_mode=TDM_OPTIMIZED.
 *      o   Set the fixed cell size to 78.
 *      o   Configure the port editing settings to enable the following actions:
 *          o    At the ingress: appending FTMH with a unicast/multicast destination (user settings).
 *          o    At the egress: removing FTMH.
 *      o   To run application:
 *          petra_opt_tdm_example (unit, incoming_port_id, is_mc_settings)
 *      o   Run traffic from the incoming port. Note that the packet is considered payload only,
 *          since the forwarding decision is performed via the appended FTMH.
 *      o   You can configure different settings for the port editing, including a different incoming port,
 *          destination port, multicast group settings, and FTMH actions.
 * 
 * 
 *  BCM> examples/dpp/utility/cint_utils_multicast.c  
 */

/* Setup the TDM configuration (Both Ingress and Egress) */
int setup_tdm(int unit, int local_port_id, int dest_id, int is_mc, int add_crc,  
                                            bcm_fabric_tdm_editing_type_t ingress_edit_type, 
                                            bcm_fabric_tdm_editing_type_t egress_edit_type,
                                            int user_field_count,
                                            int *user_field) {
    bcm_error_t rv = BCM_E_NONE;
    bcm_gport_t gport_local;
    bcm_fabric_tdm_editing_t ing_editing, eg_editing;
    int i;
    
    /* Set up the GPort to configure */
    BCM_GPORT_LOCAL_SET(gport_local,local_port_id);    
    
    /* Set the destination and editing flags (for the Ingress) */
    if(is_mc) {
        ing_editing.multicast_id = dest_id; /* Destination is a MC group ID */
        ing_editing.flags = BCM_FABRIC_TDM_EDITING_INGRESS | BCM_FABRIC_TDM_EDITING_MULTICAST ;
    } else {
        ing_editing.destination_port = dest_id; /* Destination is a GPort */
        ing_editing.flags = BCM_FABRIC_TDM_EDITING_INGRESS | BCM_FABRIC_TDM_EDITING_UNICAST ;
    }

    /* Set the User Field */
    if (user_field_count > 48) {
        printf("Cant set a user field of over 48 bits");
        ing_editing.user_field_count = 0;
    }
    else {
        int index =0;
        ing_editing.user_field_count = user_field_count;
        for (i = 0; i < user_field_count; i+=8) {
            ing_editing.user_field[index] = user_field[index];
            index++;
        }
    } 
  
    /* Set the editing type according to bcm_fabric_tdm_editing_type_t */
    ing_editing.type = ingress_edit_type;
    ing_editing.add_packet_crc = add_crc;
        
    /* Set the Ingress configuration */
    rv = bcm_fabric_tdm_editing_set(unit,gport_local,ing_editing);
    if (rv != BCM_E_NONE)
        return rv;
    
    /* Set the Egress configuration (only editing) */
    eg_editing.flags = BCM_FABRIC_TDM_EDITING_EGRESS;
    eg_editing.type = egress_edit_type;
    eg_editing.add_packet_crc = add_crc;

    rv = bcm_fabric_tdm_editing_set(unit,gport_local,eg_editing);
    return rv;
    
} 

/* In MESH MC TDM mode,
 * When custom_feature_use_ext_tdm_mesh_mc soc_property is off 
 * (It means to use IRE table). 
 * We can configure mc replications:
                   0: local to EGQ-0
                   1: local to EGQ-1
             gport=2: replication to dest 0
             gport=3: replication to dest 1
             gport=4: replication to dest 2
                   
 */
int arad_tdm_ext_mesh_mc_example (int unit, 
                      int local_port_id, 
                      int dest_port_id,
                      int is_mc, 
                      int is_opt ,
                      int min_size, 
                      int max_size, 
                      int mc_port1,
                      int mc_port2) 
{
    bcm_gport_t dest_gport_local;
    int destid_count;
    int flags =0;
    int rv;
    int core , tm_port;

    BCM_GPORT_LOCAL_SET(dest_gport_local,dest_port_id);
   
    rv = arad_tdm_example_multi_device (unit, unit, 
                      local_port_id, 
                      dest_gport_local,
                      is_mc, 
                      is_opt ,
                      min_size, 
                      max_size, mc_port1, mc_port2);

    if (rv != BCM_E_NONE){
        return rv;
    }

    /* Set replication */
    destid_count=3;
    bcm_module_t destid_array[3];

    /* We asume that all the ports on the local device */
    if (get_core_and_tm_port_from_port(unit, dest_port_id, &core, &tm_port) != BCM_E_NONE){
        return rv;
    }
    destid_array[0] = core;

    if (get_core_and_tm_port_from_port(unit, mc_port1, &core, &tm_port) != BCM_E_NONE){
        return rv;
    }
    destid_array[1] = core;

    if (get_core_and_tm_port_from_port(unit, mc_port2, &core, &tm_port) != BCM_E_NONE){
        return rv;
    }
    destid_array[2] = core;

    rv = bcm_fabric_static_replication_set( unit, local_port_id, flags, destid_count, &destid_array);

    return rv;
}


/*
 * See arad_tdm_example_multi_device description. 
 * dest_port_id is expected to be local port    .
 *                                              .
 */

int arad_tdm_example (int unit, 
                      int local_port_id, 
                      int dest_port_id,
                      int is_mc, 
                      int is_opt ,
                      int min_size, 
                      int max_size, 
                      int mc_port1,
                      int mc_port2) 
{
    bcm_gport_t dest_gport_local;

    BCM_GPORT_LOCAL_SET(dest_gport_local,dest_port_id);

    return arad_tdm_example_multi_device (unit, unit, 
                      local_port_id, 
                      dest_gport_local,
                      is_mc, 
                      is_opt ,
                      min_size, 
                      max_size, mc_port1, mc_port2);
}

/* ARAD Standard/Optimized TDM Example:
 * This example can configure a MC to ports 14,15 and incoming port id ,or UC to incoming incoming port id.
 * The FTMH will be appended in the ingress, and removed in the egress.
 * It also sets the Cells Size Range: min_size to max_size
 * local_port_id = Incoming BCM local port id. 
 * dest_gport = Outgoing BCM local port id or mod port id. 
 * is_mc = is it multicast configuration or unicast configuration 
 * min_size = min Cells Size Range: from 65
 * max_size = max Cells Size Range: up to 255  
 */
int arad_tdm_example_multi_device (int src_unit, int dst_unit,
                      int local_port_id, 
                      bcm_gport_t dest_gport,
                      int is_mc, 
                      int is_opt ,
                      int min_size, 
                      int max_size, int mc_port1, int mc_port2) {
    bcm_error_t rv = BCM_E_NONE;
    
    /* Set the MC Parameters */
    int mc_group_id = 5005;
    int mc_ports[3];
    int mc_cuds[3];
    mc_ports[0] = dest_gport;
    mc_ports[1] = mc_port1;
    mc_ports[2] = mc_port2;
    mc_cuds[0] = 1;
    mc_cuds[1] = 1;
    mc_cuds[2] = 1;
    /* Set the UC Parameters */ 
    /*in arad you must add crc to TDM packets*/
    int add_crc = 1;
    /* Set the user field parameter */
    int user_field_size;
    int user_field_data[2];
    if (is_opt) {
        user_field_size = 0; /* Size in bits (max is 32 bits) */
    } else {
        user_field_size = 16; /* Size in bits (max is 32 bits) */
        /* First 8 bits in user define will be 11 */
        /* Second 8 bits in user define will be 22 */
        user_field_data[0] = 0;
        user_field_data[1] = 0;
    }
    /* Setup the MC or UC configuration */
    if(is_mc){
        rv = multicast__open_egress_mc_group_with_local_ports(dst_unit, mc_group_id, mc_ports, mc_cuds, 3, 0);
        if (rv != BCM_E_NONE)
            return rv;
        rv = setup_tdm(src_unit, local_port_id, mc_group_id, 1, add_crc, bcmFabricTdmEditingAppend ,bcmFabricTdmEditingRemove, user_field_size, user_field_data);
        if (rv != BCM_E_NONE)
            return rv;
    } else {
        rv = setup_tdm(src_unit, local_port_id, dest_gport, 0, add_crc, bcmFabricTdmEditingAppend ,bcmFabricTdmEditingRemove, user_field_size, user_field_data);
        if (rv != BCM_E_NONE)
            return rv;
    }
    
    /* Set the Cell Size Range (Calling once for min, and once for max) */

    rv = bcm_fabric_control_set(src_unit, bcmFabricCellSizeMin, min_size);
    if (rv != BCM_E_NONE)
        return rv;
    rv = bcm_fabric_control_set(src_unit, bcmFabricCellSizeMax , max_size);
    
    return rv;
}

/* ARAD Standard/Optimized TDM Example:
 * This example can configure a MC to ports 14,15 and incoming port id ,or UC to incoming incoming port id.
 * The FTMH will not be appended in the ingress, and will nor be removed in the egress.
 * Set the Cells Size Range: min_size to max_size
 * local_port_id = Incoming BCM local port id. 
 * dest_port_id = Outgoing BCM local port id.  
 * is_mc = is it multicast configuration or unicast configuration 
 * min_size = min Cells Size Range: from 65
 * max_size = max Cells Size Range: up to 255  
 */
int arad_tdm_outter_header_example (int unit,
                                    int local_port_id, 
                                    int dest_port_id,
                                    int is_mc, 
                                    int min_size, 
                                    int max_size,
                                    int mc_port1,
                                    int mc_port2) {
    bcm_error_t rv = BCM_E_NONE;

    /* Set the MC Parameters */
    int mc_group_id = 5005;
    int mc_ports[3];
    int mc_cuds[3];
    mc_ports[0] = dest_port_id;
    mc_ports[1] = mc_port1;
    mc_ports[2] = mc_port2;
    mc_cuds[0] = 0x123;
    mc_cuds[1] = 0x789;
    mc_cuds[2] = 0x975;

    /* Set the UC Parameters */ 
    /*in arad you must add crc to TDM packets*/
    int add_crc = 1;
    /* Set the user field parameter:
       there is no inner header so there is no use for user fields */
    int user_field_size = 0;
    int user_field_data[2];
    /* Setup the MC or UC configuration */
    if(is_mc){
        int enable_outer_ftmh_cud_stamping = (soc_property_suffix_num_get(unit, -1, spn_CUSTOM_FEATURE, "tdm_eg_editing_is_cud_stamping", 0) == 1) || 
                                             (soc_property_suffix_num_get(unit, -1, spn_CUSTOM_FEATURE, "tdm_eg_editing_is_cud_stamping_type1", 0) == 1);
        bcm_fabric_tdm_editing_type_t egress_edit_type = (enable_outer_ftmh_cud_stamping ? bcmFabricTdmEditingCustomExternal : bcmFabricTdmEditingNoChange);
		rv = multicast__open_egress_mc_group_with_local_ports(unit, mc_group_id, mc_ports, mc_cuds, 3, 0);
		if (rv != BCM_E_NONE)
            return rv;
        rv = setup_tdm(unit, local_port_id, 0, 1, add_crc, bcmFabricTdmEditingNoChange, egress_edit_type, user_field_size, user_field_data);
        if (rv != BCM_E_NONE)
            return rv;
    } else {
        rv = setup_tdm(unit, local_port_id, 0, 0, add_crc, bcmFabricTdmEditingNoChange,bcmFabricTdmEditingNoChange, user_field_size, user_field_data);
        if (rv != BCM_E_NONE)
            return rv;
    }

    /* Set the Cell Size Range (Calling once for min, and once for max) */

    rv = bcm_fabric_control_set(unit, bcmFabricCellSizeMin, min_size);
    if (rv != BCM_E_NONE)
        return rv;
    rv = bcm_fabric_control_set(unit, bcmFabricCellSizeMax , max_size);

    return rv;
}

/* PETRA Standard TDM Example:
 * This example can configure a MC to ports 1,2,3 or UC to port 1.
 * The FTMH will be appended in the ingress, and removed in the egress.
 * It also sets the Cells Size Range: 70 to 120
 * local_port_id = Incoming BCM local port id.
 * is_mc = is it multicast configuration or unicast configuration
 */
int petra_std_tdm_example (int unit, int local_port_id, int is_mc) {
    bcm_error_t rv = BCM_E_NONE;
    
    /* Set the MC Parameters */
    int mc_group_id = 5005;
    int mc_ports[3];
    int mc_cuds[3];
    mc_ports[0] = 1;
    mc_ports[1] = 2;
    mc_ports[2] = 3;
    mc_cuds[0] = 1;
    mc_cuds[1] = 1;
    mc_cuds[2] = 1;
    
    /* Set the UC Parameters */
    int dest_port_id = 1;
    /*in Petra you do not have to add crc to TDM packets*/
    int add_crc = 0;
    /* Set the user field parameter */
    int user_field_size = 16; /* Size in bits (max is 32 bits) */
    int user_field_data[2];
    /* First 8 bits in user define will be 11 */
    /* Second 8 bits in user define will be 22 */
    user_field_data[0] = 11;
    user_field_data[1] = 22;
    
    
    /* Setup the MC or UC configuration */
    if(is_mc){
        rv = multicast__open_egress_mc_group_with_local_ports(unit, mc_group_id, mc_ports, mc_cuds, 3, 0);
        if (rv != BCM_E_NONE)
            return rv;
        rv = setup_tdm(unit, local_port_id, mc_group_id, 1, add_crc, bcmFabricTdmEditingAppend ,bcmFabricTdmEditingRemove, user_field_size, user_field_data);
        if (rv != BCM_E_NONE)
            return rv;
    } else {
        rv = setup_tdm(unit, local_port_id, dest_port_id, 0, add_crc, bcmFabricTdmEditingAppend ,bcmFabricTdmEditingRemove, user_field_size, user_field_data);
        if (rv != BCM_E_NONE)
            return rv;
    }
    
    /* Set the Cell Size Range (Calling once for min, and once for max) */
    int min_size = 70;
    int max_size = 120;
    rv = bcm_fabric_control_set(unit, bcmFabricCellSizeMin, min_size);
    if (rv != BCM_E_NONE)
        return rv;
    rv = bcm_fabric_control_set(unit, bcmFabricCellSizeMax , max_size);
    
    return rv;
}


/* PETRA Optimized TDM Example
 * This example can configure a MC to ports 1,2,3 or UC to port 1.
 * The FTMH will be appended in the ingress, and removed in the egress.
 * It also sets the Cells Size to 78. Running from a traffic generator one needs to set packet size to be 80.    
 * local_port_id = Incoming BCM local port id.
 * is_mc = is it multicast configuration or unicast configuration
 */
int petra_opt_tdm_example (int unit,int local_port_id, int is_mc) {
    bcm_error_t rv = BCM_E_NONE;
    /* Set the MC Parameters */
    int mc_group_id = 5005;
    int mc_ports[3];
    int mc_cuds[3];
    mc_ports[0] = 1;
    mc_ports[1] = 2;
    mc_ports[2] = 3;
    mc_cuds[0] = 1;
    mc_cuds[1] = 1;
    mc_cuds[2] = 1;
    int user_field;
    
    /* Set the UC Parameters */
    int dest_port_id = 1;
    /*in Petra you do not have to add crc to TDM packets*/
    int add_crc = 0;
    /* Setup the MC or UC configuration */
    if(is_mc){
        rv = multicast__open_egress_mc_group_with_local_ports(unit, mc_group_id, mc_ports, mc_cuds, 3, 0);
        if (rv != BCM_E_NONE)
            return rv;
        rv = setup_tdm(unit, local_port_id, mc_group_id, 1, add_crc, bcmFabricTdmEditingAppend ,bcmFabricTdmEditingRemove,0,&user_field);
        if (rv != BCM_E_NONE)
            return rv;
    } else {
        rv = setup_tdm(unit, local_port_id, dest_port_id, 0, add_crc, bcmFabricTdmEditingAppend ,bcmFabricTdmEditingRemove,0,&user_field);
        if (rv != BCM_E_NONE)
            return rv;
    }
    
    
    /* Set the Cell Size Fixed Value */
    int cell_size = 78;
    rv = bcm_fabric_control_set(unit, bcmFabricCellSizeFixed , cell_size);
    
    return rv;
}

    

/* Run automatic main */
/* Example when uint = 0, incoming port = 13, no multicast configuration. */    
/*std_tdm_example(0,13,0);*/
/*opt_tdm_example(0,13,0);*/

    
    
