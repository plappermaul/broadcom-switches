/* $Id: cint_utils_global.c,v 1.10 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
*/


/* ********* 
  Globals/Aux Variables
 ********** */

/* debug prints */
int verbose = 1;

/* These defines used for in/out port definitions*/
int IN_PORT=0;
int OUT_PORT=1;
int NUMBER_OF_PORTS=2;

/* General port defines*/
int PORT_1 = 0;
int PORT_2 = 1;
int PORT_3 = 2;
int PORT_4 = 3;

/* General RIF defines */
int RIF_1 = 0;
int RIF_2 = 1;
int RIF_3 = 2;
int RIF_4 = 3;

/* General host defines*/
int HOST_1 = 0;
int HOST_2 = 1;
int HOST_3 = 2;
int HOST_4 = 3;
int HOST_5 = 4;

int PETRA         = 0x88640;
int ARAD          = 0x88650;
int ARAD_PLUS     = 0x88660;
int ARDON         = 0x88202;
int JERICHO       = 0x88675;
int JERICHO_B0    = 0x1188675; /*11 is the revision*/
int QUMRAN_MX     = 0x88375;
int QUMRAN_MX_B0  = 0x1188375; /*11 is the revision*/
int JERICHO_PLUS  = 0x88680;
int QUMRAN_AX     = 0x88470;
int QUMRAN_AX_B0  = 0x1188470;   /*11 is the revision*/
int JERICHO2      = 0x88690;
int QUMRAN_UX     = 0x88270;

/* General device list - devices must be chronologically ordered
   New devices should be added at the end */
int DEVICE_LIST_SIZE = 13;
int DEVICE_LIST [DEVICE_LIST_SIZE] = {0};
DEVICE_LIST[0] = PETRA;
DEVICE_LIST[1] = ARAD;
DEVICE_LIST[2] = ARAD_PLUS;
DEVICE_LIST[3] = ARDON;
DEVICE_LIST[4] = JERICHO;
DEVICE_LIST[5] = QUMRAN_MX;
DEVICE_LIST[6] = JERICHO_B0;
DEVICE_LIST[7] = QUMRAN_MX_B0;
DEVICE_LIST[8] = JERICHO_PLUS;
DEVICE_LIST[9] = QUMRAN_AX;
DEVICE_LIST[10] = QUMRAN_AX_B0;
DEVICE_LIST[11] = JERICHO2;
DEVICE_LIST[12] = QUMRAN_UX;


/* port_tpid */
struct port_tpid_info_s {
    int port;
    int outer_tpids[2];
    int nof_outers; /*0, 1, 2*/
    int inner_tpids[2];
    int nof_inners; /*0, 1 */
    int discard_frames; /* BCM_PORT_DISCARD_NONE/ALL/UNTAG/TAG */
    uint32 tpid_class_flags; /* flags for bcm_port_tpid_class_set */
    int vlan_transation_profile_id; /* Default Ingress action command in case of no match in VTT L2 LIF lookup */
};

port_tpid_info_s port_tpid_info1;

/* Global declaration, since many cints use it and some redeclare it, or don't declare it at all, which causes errors. */
int advanced_vlan_translation_mode = 0;

/* indicate if the current device is equal to or newer than the one provided
   Get unit - unit id
   Get device - first supported device
   returns 1 if current device is equal or newer than the one provided */
int is_device_or_above(int unit, int supported_device_id) {
    bcm_info_t info;
    int i;
    int current_device_id;
    int current_device_index;
    int supported_device_index = DEVICE_LIST_SIZE;

    int rv = bcm_info_get(unit, &info);
    if (rv != BCM_E_NONE) {
        printf("Error in bcm_info_get\n");
        print rv;
        return rv;
    }

    current_device_id = info.device;
    for (i=0;i<DEVICE_LIST_SIZE;i++) {
        if (((DEVICE_LIST[i] & 0xffff) == current_device_id) && (info.revision >= (DEVICE_LIST[i] >> 20))) {
            current_device_index = i;
        }
        if (DEVICE_LIST[i] == supported_device_id) {
            supported_device_index = i;
        }
    }

    return current_device_index >= supported_device_index;
}

/* indicate if the device is arad
   Get unit - unit id
   yesno  -  indicate if the device is arad */ 
int is_arad_only(int unit, int *yesno) {
    bcm_info_t info;

    int rv = bcm_info_get(unit, &info);
    if (rv != BCM_E_NONE) {
  	  printf("Error in bcm_info_get\n");
  	  print rv;
  	  return rv;
    }

    *yesno = info.device == 0x8650 ? 1 : 0;

    return rv;
}

/* indicate if the device is arad a0
   Get unit - unit id
   yesno  -  indicate if the device is arad a0 */ 
int is_arad_a0_only(int unit, int *yesno) {
    bcm_info_t info;

    int rv = bcm_info_get(unit, &info);
    if (rv != BCM_E_NONE) {
  	  printf("Error in bcm_info_get\n");
  	  print rv;
  	  return rv;
    }

    *yesno = info.device == 0x8650 &
			 info.revision == 0x0 ? 1 : 0;

    return rv;
}

/* indicate if the device is arad b0
   Get unit - unit id
   yesno  -  indicate if the device is arad b0 */ 
int is_arad_b0_only(int unit, int *yesno) {
    bcm_info_t info;

    int rv = bcm_info_get(unit, &info);
    if (rv != BCM_E_NONE) {
  	  printf("Error in bcm_info_get\n");
  	  print rv;
  	  return rv;
    }

    *yesno = info.device == 0x8650 &
			 info.revision == 0x11 ? 1 : 0;

    return rv;
}

/* indicate if the device is arad b1
   Get unit - unit id
   yesno  -  indicate if the device is arad b1 */ 
int is_arad_b1_only(int unit, int *yesno) {
    bcm_info_t info;

    int rv = bcm_info_get(unit, &info);
    if (rv != BCM_E_NONE) {
  	  printf("Error in bcm_info_get\n");
  	  print rv;
  	  return rv;
    }

    *yesno = info.device == 0x8650 &
			 info.revision == 0x12 ? 1 : 0;

    return rv;
}

/* indicate if the device is arad plus
   Get unit - unit id
   yesno  -  indicate if the device is arad plus */ 
int is_arad_plus_only(int unit, int *yesno) {
    bcm_info_t info;

    int rv = bcm_info_get(unit, &info);
    if (rv != BCM_E_NONE) {
  	  printf("Error in bcm_info_get\n");
  	  print rv;
  	  return rv;
    }

    *yesno = info.device == 0x8660 ? 1 : 0;

    return rv;
}

/* indicate if the device is jericho
   Get unit - unit id
   yesno  -  indicate if the device is jericho */ 
int is_jericho_only(int unit, int *yesno) {
    bcm_info_t info;

    int rv = bcm_info_get(unit, &info);
    if (rv != BCM_E_NONE) {
  	  printf("Error in bcm_info_get\n");
  	  print rv;
  	  return rv;
    }

    *yesno = info.device == 0x8675 ? 1 : 0;

    return rv;
}

/* indicate if the device is jericho b0
   Get unit - unit id
   yesno  -  indicate if the device is jericho b0 */ 
int is_jericho_b0_only(int unit, int *yesno) {
    bcm_info_t info;

    int rv = bcm_info_get(unit, &info);
    if (rv != BCM_E_NONE) {
  	  printf("Error in bcm_info_get\n");
  	  print rv;
  	  return rv;
    }

    *yesno = info.device == 0x8675 &
             info.revision == 0x11 ? 1 : 0;

    return rv;
}

/* indicate if the device is qumran_mx
   Get unit - unit id
   yesno  -  indicate if the device is qumran_mx */ 
int is_qumran_mx_only(int unit, int *yesno) {
    bcm_info_t info;

 
    int rv = bcm_info_get(unit, &info);
    if (rv != BCM_E_NONE) {
  	  printf("Error in bcm_info_get\n");
  	  print rv;
  	  return rv;
    }

    *yesno = info.device == 0x8375 ? 1 : 0;

    return rv;
}

/* indicate if the device is qumran_mx b0
   Get unit - unit id
   yesno  -  indicate if the device is qumran_mx b0 */ 
int is_qumran_mx_b0_only(int unit, int *yesno) {
    bcm_info_t info;

 
    int rv = bcm_info_get(unit, &info);
    if (rv != BCM_E_NONE) {
  	  printf("Error in bcm_info_get\n");
  	  print rv;
  	  return rv;
    }

    *yesno = info.device == 0x8375 &
             info.revision == 0x11 ? 1 : 0;

    return rv;
}

/* indicate if the device is jericho plus
   Get unit - unit id
   yesno  -  indicate if the device is jericho plus */ 
int is_jericho_plus_only(int unit, int *yesno) {
    bcm_info_t info;

    int rv = bcm_info_get(unit, &info);
    if (rv != BCM_E_NONE) {
  	  printf("Error in bcm_info_get\n");
  	  print rv;
  	  return rv;
    }

    *yesno = info.device == 0x8680 ? 1 : 0;

    return rv;
}

/* indicate if the device is qumran_ax
   Get unit - unit id
   yesno  -  indicate if the device is qumran_ax */ 
int is_qumran_ax_only(int unit, int *yesno) {
    bcm_info_t info;

 
    int rv = bcm_info_get(unit, &info);
    if (rv != BCM_E_NONE) {
  	  printf("Error in bcm_info_get\n");
  	  print rv;
  	  return rv;
    }

    *yesno = info.device == 0x8470 ? 1 : 0;

    return rv;
}

/* indicate if the device is qumran_ax
   Get unit - unit id
   yesno  -  indicate if the device is qumran_ax */ 
int is_qumran_ux_only(int unit, int *yesno) {
    bcm_info_t info;

 
    int rv = bcm_info_get(unit, &info);
    if (rv != BCM_E_NONE) {
  	  printf("Error in bcm_info_get\n");
  	  print rv;
  	  return rv;
    }

    *yesno = info.device == 0x8270 ? 1 : 0;

    return rv;
}

/* indicate if the device is qumran_ax
   Get unit - unit id
   yesno  -  indicate if the device is qumran_ax */ 
int is_qumran_ax(int unit, int *yesno) {
    bcm_info_t info;

    int rv = bcm_info_get(unit, &info);
    if (rv != BCM_E_NONE) {
  	  printf("Error in bcm_info_get\n");
  	  print rv;
  	  return rv;
    }

    if (info.device == 0x8470 || info.device == 0x8270) {
        *yesno = 1;
    } else {
        *yesno = 0;
    }

    return rv;
}



/* port_tpid */

/* 
 *  nof_outers = 1,2
 *  nof_inners = 0,1
 *  nof_outers >= nof_inners
 *  nof_outers + nof_inners = 2
 */ 
void
port_tpid_init(int port, int nof_outers, int nof_inners) {
    port_tpid_info1.port = port;
    port_tpid_info1.outer_tpids[0] = 0x8100;
    port_tpid_info1.outer_tpids[1] = 0x88a0;
    port_tpid_info1.inner_tpids[0] = 0x9100;
    port_tpid_info1.nof_outers = nof_outers;
    port_tpid_info1.nof_inners = nof_inners;
    port_tpid_info1.discard_frames = BCM_PORT_DISCARD_NONE;
    port_tpid_info1.tpid_class_flags = 0;
    port_tpid_info1.vlan_transation_profile_id = 0;
}

int
port_tpid_set(int unit) {
    return _port_tpid_set(unit, 0);
}

int
port_lif_tpid_set(int unit) {
    return _port_tpid_set(unit, 1);
}

int
_port_tpid_set(int unit, int is_lif) {
    int rv;
    int indx;
    int color_set=0;/* has to be zero */

    advanced_vlan_translation_mode = soc_property_get(unit, "bcm886xx_vlan_translate_mode",0);

    if (advanced_vlan_translation_mode) {
        if (port_tpid_info1.nof_outers == 1 && port_tpid_info1.nof_inners == 1) {
            return port_dt_tpid_set(unit, port_tpid_info1.port, port_tpid_info1.outer_tpids[0], port_tpid_info1.inner_tpids[0], is_lif, port_tpid_info1.vlan_transation_profile_id);
        } else if (port_tpid_info1.nof_outers == 1 && port_tpid_info1.nof_inners == 0) {
            return port_outer_tpid_set(unit, port_tpid_info1.port, port_tpid_info1.outer_tpids[0], is_lif, port_tpid_info1.vlan_transation_profile_id);
        }
    }
    
    /* assume port_tpid_init was called with legal params*/
    
    /* remove old tpids on port */
    rv = bcm_port_tpid_delete_all(unit,port_tpid_info1.port);
    if (rv != BCM_E_NONE) {
        printf("Error, in bcm_port_tpid_delete_all, port=%d, \n", port_tpid_info1.port);
        return rv;
    }
    
    /* set outer tpids of the port */
    for(indx = 0; indx < port_tpid_info1.nof_outers; ++indx){
        rv = bcm_port_tpid_add(unit, port_tpid_info1.port, port_tpid_info1.outer_tpids[indx], color_set);
        if (rv != BCM_E_NONE) {
            printf("Error, in bcm_port_tpid_add, tpid=%d, \n", port_tpid_info1.outer_tpids[indx]);
            return rv;
        }
    }
    
    /* set inner tpids of the port */
    for(indx = 0; indx < port_tpid_info1.nof_inners; ++indx){
        rv = bcm_port_inner_tpid_set(unit, port_tpid_info1.port, port_tpid_info1.inner_tpids[indx]);
        if (rv != BCM_E_NONE) {
            printf("Error, in bcm_port_inner_tpid_set, tpid=%d, \n", port_tpid_info1.inner_tpids[indx]);
            return rv;
        }
    }
    
    /* Only relevant for physical port */    
    if(!is_lif) {    
        /* set discard frames on port */
        rv = bcm_port_discard_set(unit, port_tpid_info1.port, port_tpid_info1.discard_frames);
        if (rv != BCM_E_NONE) {
            printf("Error, in bcm_port_discard_set, discard-type=%d, \n", port_tpid_info1.discard_frames);
            return rv;
        }
    }
    
    return rv;
}

/* Send a packet from string input */
int tx_packet_via_bcm(int unit, char *data, int *ptch, int ptch_version) {
    char tmp, data_iter;
    int data_len, i, pkt_len, ptch_len, ind, data_base;
    bcm_pkt_t *pkt_info;
    bcm_error_t rv;

    /* If string data starts with 0x or 0X, skip it */
    data_base = 0;
    if ((data[0] == '0') && ((data[1] == 'x') || (data[1] == 'X'))) {
        data_base = 2;
    }

    /* PTCH length by version */
    ptch_len = ptch_version == 1 ? 3 : 2;

    /* figure out packet data_length (no strdata_len in cint) */
    data_len=0;
    i=0;
    while (data[data_base+data_len] != 0) ++data_len;

    pkt_len = 64+ptch_len;
    pkt_len = (pkt_len < data_len+1 ? (data_len+ptch_len) : pkt_len);

    rv = bcm_pkt_alloc(unit, pkt_len, BCM_TX_CRC_ALLOC, &pkt_info);
    BCM_IF_ERROR_RETURN(rv);

    sal_memset(pkt_info->_pkt_data.data, 0, pkt_len);

    pkt_info->pkt_data = &pkt_info->_pkt_data;

    /* Determine which core should the packet be injected to */
    uint32 dummy_flags;
    bcm_port_interface_info_t interface_info;
    bcm_port_mapping_info_t mapping_info;

    rv = bcm_port_get(unit, ptch[ptch_len-1], &dummy_flags, &interface_info, &mapping_info);
    if (BCM_FAILURE(rv)) {
        LOG_ERROR(BSL_LS_APPL_COMMON,
            (BSL_META_U(unit,"bcm_port_get failed. Error:%d (%s)\n"),
                                                    rc, bcm_errmsg(rc)));
        return -1;
    }

    if(mapping_info.core==1) {
        pkt_info->_dpp_hdr[0] = mapping_info.channel;
        pkt_info->flags = pkt_info->flags | BCM_PKT_F_HGHDR;
    }

    printf("Packet will be injected to core %d, local port %d\n",mapping_info.core, mapping_info.tm_port);

    /* PTCH - 2B / 3B */
    pkt_info->_pkt_data.data[0] = ptch[0];
    pkt_info->_pkt_data.data[1] = 0;
    pkt_info->_pkt_data.data[ptch_len-1] = mapping_info.tm_port;

    /* Convert char to value */
    i = 0;
    while (i < data_len) {
        data_iter=data[data_base+i];
        if (('0' <= data_iter) && (data_iter <= '9')) {
            tmp = data_iter - '0';
        } else if (('a' <= data_iter) && (data_iter <= 'f')) {
            tmp = data_iter - 'a' + 10;
        } else if (('A' <= data_iter) && (data_iter <= 'F')) {
            tmp = data_iter - 'A' + 10;
        } else {
            printf("Unexpected char: %c\n", data_iter);
            return BCM_E_PARAM;
        }

        /* String input is in 4b unit. Below we're filling in 8b:
           offset is /2, and we shift by 4b if the input char is odd */
        pkt_info->_pkt_data.data[ptch_len+i/2] |= tmp << (((i+1)%2)*4);
        ++i;
    }

    rv = bcm_tx(unit, pkt_info, NULL);

    return rv;
}

/* This function accept a unit, port, and pointer to int, and fills it with system port appropriate for the unit-port combination.
   Use it to find your system ports before using them in multi device cints.
   */
int port_to_system_port(int unit, int port, int *sysport) {
    int modid, gport, rv, tm_port, core;

    if (BCM_GPORT_IS_SYSTEM_PORT(port)) {
        *sysport = port;
        return BCM_E_NONE;
    }

    rv = bcm_stk_modid_get(unit, &modid);
    if (rv != BCM_E_NONE){
        printf("Error, in bcm_stk_modid_get\n");
        return rv;
    }
    if (is_device_or_above(unit,JERICHO)) {
        /* Core should also be considered */
        rv = get_core_and_tm_port_from_port(unit,port,&core,&tm_port);
        if (rv != BCM_E_NONE){
            printf("Error, in get_core_and_tm_port_from_port\n");
            return rv;
        }
    } else {
        core = 0;
        tm_port = port;
    }
    BCM_GPORT_MODPORT_SET(gport, modid + core, tm_port);

    rv = bcm_stk_gport_sysport_get(unit, gport, sysport);
    if (rv != BCM_E_NONE){
        printf("Error, in bcm_stk_gport_sysport_get\n");
        return rv;
    }

    return BCM_E_NONE;
}


/*
 * Extract core from input local port. On fail, return '-1'
 */
int get_core_and_tm_port_from_port(int unit, bcm_gport_t gport, int * core, int * tm_port)
{
    bcm_error_t rc;
    uint32 dummy_flags;
    bcm_port_interface_info_t interface_info;
    bcm_port_mapping_info_t mapping_info;

    rc = bcm_port_get(unit, gport, &dummy_flags, &interface_info, &mapping_info);
    if (BCM_FAILURE(rc)) {
        LOG_ERROR(BSL_LS_APPL_COMMON,
            (BSL_META_U(unit,"bcm_port_get failed. Error:%d (%s)\n"),  
                                                    rc, bcm_errmsg(rc)));
        return -1;
    } else {
        *core = mapping_info.core;
        *tm_port = mapping_info.tm_port;
    }

    return 0;
}

/*****************************************************************************
* Function:  check_eq_sig_val
* Purpose:   Check that the value in signal equals to the required one
* Params:
* unit      - 
* core      - 
* block     - 
* from      - 
* to        - 
* name      - 
* compareTo - 
* Return:    (int)
*******************************************************************************/
int check_eq_sig_val(int unit, int core,  char *block, char *from, char *to, char *name, uint32 compareTo)
{
    uint32 value;
    int size = 32;
    int rc = BCM_E_NONE;
    
    rc = dpp_dsig_read(unit,core,block,from,to,name,&value,size);
    if(rc != BCM_E_NONE)
    {
        printf("Error in dpp_dsig_read\n");
        return BCM_E_FAIL;
    }
    printf("Check Signal value: %s %s %s %s value: %d compareTo %d\n",block,from,to,name,value,compareTo);
    if(value == compareTo)
    {
        printf("Copmare Success Values are Equal\n");
        return BCM_E_NONE;
    }
    else
    {
        printf("Copmare Fail Values are NOT Equal\n");
        return BCM_E_PARAM;
    }
    
    return 0;
}

/* Get local port number of device
 *    unit   - unit id
 *    return - int, local port number
 */ 
int get_local_port_number(int unit)
{
    bcm_info_t info;
    int port_num = 256;

    int rv = bcm_info_get(unit, &info);
    if (rv != BCM_E_NONE) {
  	  printf("Error in bcm_info_get\n");
  	  print rv;
  	  return rv;
    }

    print info;
    if (info.device == 0x8675 ||
        info.device == 0x8375 ||
        info.device == 0x8680) 
    {
        port_num = 512;
    }

    return port_num;
}
