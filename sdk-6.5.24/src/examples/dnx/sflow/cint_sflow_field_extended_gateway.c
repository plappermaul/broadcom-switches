 /*
 * Configuration:
 *
 * cint ../../../src/examples/dnx/sflow/cint_sflow_field_extended_gateway.c
 * cint
 * see cint_sflow_extended_gateway.c configuration sequence.
 *
 * For 1st pass SFLOW IPoETH:
 *    Set up a FG that qualify upon (FWD_Layer) and action (Stat_Sampling):
 *    For L3 forwarding: FWD_Layer[curr-1].Type is ETH and FWD_Layer[curr].Type is (IPv4 or IPv6)
 *    For L2 bridging: Bridge packet will be skipped since for SFLOW IPoETH, on the 2nd pass, 
 *    there is EXEM lookup based on IP FWD decision (DIP-destination and SIP-destination) 
 *
 * For 2nd pass SFLOW IPoETH:
 *   The EXEM lookup:
 *   Key = dst (DIP/SIP destination)
 *   Result = sflow_encap_extended_id
 *   (sflow_encap_extended_dst_id or sflow_encap_extended_src_id)
 */

/* Snoop FG and action */
bcm_field_group_t cint_sflow_snoop_field_fg_id = BCM_FIELD_INVALID;

/* TCAM Entry per core for snooping */
bcm_field_entry_t cint_cflow_core_id_entry_id[2];

/* TCAM Entry per eventor port - action = DO NOTHING - for skipp the snooping */
bcm_field_entry_t cint_cflow_eventor_port_entry_id = BCM_FIELD_INVALID;

/* Note: 2nd pass (recycle port) presel-ID must be lower than the 1st pass presel-IDs */
bcm_field_presel_t sflow_recycle_port_presel_id = 90;
bcm_field_presel_t route_ipv4_presel_id = 91;
bcm_field_presel_t route_ipv6_presel_id = 92;

/* 2nd pass ingress - disable ITPP Termination - can be done only by IPMF3 */
bcm_field_presel_t ipmf3_presel_id = 44;

/* 2nd pass trap - for dropping the packet in case EXEM lookup fails*/
int sflow_trap_id = -1;
bcm_field_group_t cint_sflow_trap_dip_field_fg_id = BCM_FIELD_INVALID;
bcm_field_group_t cint_sflow_trap_sip_field_fg_id = BCM_FIELD_INVALID;
bcm_field_action_t cint_sflow_trap_dip_action_id = BCM_FIELD_INVALID;
bcm_field_action_t cint_sflow_trap_sip_action_id = BCM_FIELD_INVALID;

/* 2nd pass - adding SFLOW EXTENDED OUT-LIF and setting destination to eventor port*/
bcm_field_group_t cint_sflow_extended_lif_field_fg_id = BCM_FIELD_INVALID;
bcm_field_action_t cint_sflow_extended_lif_action_id = BCM_FIELD_INVALID;
bcm_field_action_t cint_sflow_eventor_port_action_id = BCM_FIELD_INVALID;

/* 2nd pass ingress - set parsing start offset to 0, so ITPP termination will not terminate SFLOW Header Record and In/Out-Port values  */
bcm_field_group_t cint_sflow_parsing_start_offset_fg_id = BCM_FIELD_INVALID;
bcm_field_action_t cint_sflow_parsing_start_offset_action_id = BCM_FIELD_INVALID;

/* Setting Outlif to Extended DIP/SIP*/
bcm_field_group_t cint_sflow_extended_dip_field_fg_id = BCM_FIELD_INVALID;
bcm_field_group_t cint_sflow_extended_sip_field_fg_id = BCM_FIELD_INVALID;

/* In case EXEM lookup successed - need to cancel the drop - use priority */
bcm_field_action_priority_t cint_sflow_exem_dip_invalidate_next_priority =  BCM_FIELD_ACTION_POSITION(2, 2);
bcm_field_action_priority_t cint_sflow_exem_sip_invalidate_next_priority =  BCM_FIELD_ACTION_POSITION(2, 4);
bcm_field_action_priority_t cint_sflow_trap_dip_priority =  BCM_FIELD_ACTION_POSITION(2, 3);
bcm_field_action_priority_t cint_sflow_trap_sip_priority =  BCM_FIELD_ACTION_POSITION(2, 5);

/* 
* FG entry for preventing snooping the SFLOW DATAGRAM coming from eventor port
* It's priority must be higher then the snoop priority!
*/
uint32 cint_field_eventor_port_entry_priority = 0;
uint32 cint_field_snoop_entry_priority = 1;

/**
* \brief
*  Configure FG with relevant qualifier/action/context -
*  For 1st pass - configure "SNOOP"
* \param [in] unit - Device id 
* \param [in] gport_stat_samp -  the gport attached to the FG
*        action
* \param [in] context_id  -  Context to attach the FG
* \return
*   int - Error Type
* \remark
*
* \see
*   * None
*/
int cint_sflow_field_extended_gateway_first_pass_group_create_and_attach(int unit, int nof_cores, bcm_gport_t gport_stat_samp_core_0, bcm_gport_t gport_stat_samp_core_1, bcm_field_context_t context_id)
{
   int rv = BCM_E_NONE;
   bcm_field_stage_t bcm_stage = bcmFieldStageIngressPMF1;
   bcm_field_action_t bcm_action = bcmFieldActionStatSampling;
   void *dest_char;
   bcm_field_group_info_t field_group_info;
   bcm_field_group_attach_info_t attach_info;
   bcm_field_entry_info_t ent_info;
   bcm_gport_t gport_stat_samp[2];
   int core_id;
   char *proc_name;

   proc_name = "cint_sflow_field_extended_gateway_first_pass_group_create_and_attach";
    
   printf("%s: START\n", proc_name);
     
   bcm_field_group_info_t_init(&field_group_info);
   field_group_info.fg_type = bcmFieldGroupTypeTcam;
   field_group_info.stage = bcmFieldStageIngressPMF1;

   /* Set quals */
   field_group_info.nof_quals = 1;
   field_group_info.qual_types[0] = bcmFieldQualifyInPort; /* qualifier for preventing snooping of SFLOW DATAGRAM coming from Eventor port !*/


   /* Set actions */
   field_group_info.nof_actions = 1;
   field_group_info.action_types[0] = bcm_action;

   dest_char = &(field_group_info.name[0]);
   sal_strncpy_s(dest_char, "sflow_snoop_ip_fg", sizeof(field_group_info.name));

   rv = bcm_field_group_add(unit, 0, &field_group_info, &cint_sflow_snoop_field_fg_id);
   if (rv != BCM_E_NONE)
   {
      printf("%s: Error (%d), in bcm_field_group_add\n", proc_name, rv);
      return rv;
   }
   printf("%s: FG Id (%d) was add to iPMF1 Stage\n", proc_name, cint_sflow_snoop_field_fg_id);

   bcm_field_group_attach_info_t_init(&attach_info);
   attach_info.key_info.nof_quals = field_group_info.nof_quals;
   attach_info.payload_info.nof_actions = field_group_info.nof_actions;
   attach_info.payload_info.action_types[0] = field_group_info.action_types[0];
   attach_info.key_info.qual_types[0] = field_group_info.qual_types[0];
   attach_info.key_info.qual_info[0].input_type = bcmFieldInputTypeMetaData;

   rv = bcm_field_group_context_attach(unit, 0, cint_sflow_snoop_field_fg_id, context_id, &attach_info);
   if (rv != BCM_E_NONE)
   {
      printf("%s: Error (%d), in bcm_field_group_context_attach\n", proc_name, rv);
      return rv;
   }

   printf("%s: FG Id (%d) in iPMF1 attached to context (%d)\n", proc_name, cint_sflow_snoop_field_fg_id, context_id);
 
   gport_stat_samp[0] = gport_stat_samp_core_0;
   gport_stat_samp[1] = gport_stat_samp_core_1;

   for (core_id = 0; core_id < nof_cores; core_id ++) {

      bcm_field_entry_info_t_init(&ent_info);

      ent_info.priority = cint_field_snoop_entry_priority;
      ent_info.nof_entry_quals = 0;
      ent_info.core = core_id;

      ent_info.nof_entry_actions = 1;
      ent_info.entry_action[0].type = bcm_action;
      ent_info.entry_action[0].value[0] = gport_stat_samp[core_id];
      ent_info.entry_action[0].value[1] = 5;

      rv = bcm_field_entry_add(unit, 0, cint_sflow_snoop_field_fg_id, &ent_info, &(cint_cflow_core_id_entry_id[core_id]));
      if (rv != BCM_E_NONE)
      {
         printf("%s: Error (%d), in bcm_field_entry_add core_id = %d \n", proc_name, rv, core_id);
         return rv;
      }

      printf("%s: TCAM entry is added for core_id = %d \n", proc_name, core_id);
   }

   printf("%s: END\n", proc_name);
   return rv;
}

/**
 * \brief
 *  Create a trap for dropping the mirrored packet in the 2nd
 *  pass in case the EXEM lookup fails.
 * \param [in] unit - Device ID
 * \return
 *   int - Error Type
 * \remark
 *   * None
 * \see
 *   * None
 */
int cint_sflow_field_extended_gateway_trap_create(int unit)
{
   int rv = BCM_E_NONE;
   bcm_rx_trap_config_t config;
   char *proc_name;

   proc_name = "sflow_field_extended_gateway_trap_create";

   /*
    * change dest port for trap
    */
   bcm_rx_trap_config_t_init(&config);
   config.flags |= (BCM_RX_TRAP_UPDATE_DEST);
   config.dest_port = BCM_GPORT_BLACK_HOLE;

   /*
    * set uc strict to drop packet
    */
   rv = bcm_rx_trap_type_create(unit, 0, bcmRxTrapUserDefine, &sflow_trap_id);
   if (rv != BCM_E_NONE)
   {
      printf("%s: Error (%d), in trap create, trap bcmRxTrapUcStrictRpfFail \n", proc_name, rv);
      return rv;
   }

   rv = bcm_rx_trap_set(unit, sflow_trap_id, &config);
   if (rv != BCM_E_NONE)
   {
      printf("%s: Error (%d), in trap set sflow_trap_id (%d)\n", proc_name, rv, sflow_trap_id);
      return rv;
   }

   printf ("%s: trap created - sflow_trap_id = %d\n", proc_name, sflow_trap_id);

   return rv;
}


/**
* \brief
*  Configuration FG with relevant qualifier/action/context.
*  For 2nd pass - set constant actions.
* \param [in] unit - Device id 
* \param [in] sflow_encap_id - the sflow encap-id attached to the FG action
* \param [in] eventor_port -  the eventor port attached to the FG action
* \param [in] context_id  -  Context to attach the FG
* \return
*   int - Error Type
* \remark
* This function creates one FG with two constant actions:
*   - sets outlif0 to the given SFLOW EXTENDED EEDB entry.
*   - sets the destination to the given eventor port.
*  
* \see
*   * None
*/
int sflow_field_extended_gateway_second_pass_const_create_and_attach(int unit, int sflow_encap_id, int eventor_port, bcm_field_context_t context_id)
{
   int rv = BCM_E_NONE;
   bcm_field_action_info_t action_info, action_info_get;
   bcm_field_stage_t bcm_stage = bcmFieldStageIngressPMF1;
   bcm_field_action_t bcm_action_lif = bcmFieldActionOutVport0Raw;
   bcm_field_action_t bcm_action_dest = bcmFieldActionRedirect;
   uint32 bcm_value[BCM_FIELD_ACTION_WIDTH_IN_WORDS] = {0};
   uint32 hw_value[BCM_FIELD_ACTION_WIDTH_IN_WORDS] = {0};
   void *dest_char;
   bcm_field_group_info_t field_group_info;
   bcm_field_group_attach_info_t attach_info;
   bcm_gport_t trap_gport;
   bcm_gport_t gport_eventor_port;
   char *proc_name;

   proc_name = "sflow_field_extended_gateway_second_pass_const_create_and_attach";

   /* Make eventor port Gport */
   BCM_GPORT_LOCAL_SET(gport_eventor_port, eventor_port);

   rv = bcm_field_action_info_get(unit, bcm_action_lif, bcm_stage, &action_info_get);
   if (rv != BCM_E_NONE)
   {
      printf("%s: Error (%d), in bcm_field_action_info_get\n", proc_name, rv);
      return rv;
   }

   bcm_field_action_info_t_init(&action_info);
   action_info.action_type = bcm_action_lif;
   action_info.stage = bcm_stage;
   action_info.prefix_size = action_info_get.size;
   action_info.prefix_value = sflow_encap_id;
   action_info.size = 0;
   dest_char = &(action_info.name[0]);
   sal_strncpy_s(dest_char, "sflow_extended_lif_a", sizeof(action_info.name));
   rv = bcm_field_action_create(unit, 0, &action_info, &cint_sflow_extended_lif_action_id);
   if (rv != BCM_E_NONE)
   {
      printf("%s: Error (%d), in bcm_field_action_create\n", proc_name, rv);
      return rv;
   }

   bcm_value[0] = gport_eventor_port;
   rv = bcm_field_action_value_map(unit, bcm_stage, bcm_action_dest, bcm_value, hw_value);
   if (rv != BCM_E_NONE)
   {
      printf("%s: Error (%d), in bcm_field_action_value_map\n", proc_name, rv);
      return rv;
   }
        
   rv = bcm_field_action_info_get(unit, bcm_action_dest, bcm_stage, &action_info_get);
   if (rv != BCM_E_NONE)
   {
      printf("%s: Error (%d), in bcm_field_action_info_get\n", proc_name, rv);
      return rv;
   }

   bcm_field_action_info_t_init(&action_info);
   action_info.action_type = bcm_action_dest;
   action_info.stage = bcm_stage;
   action_info.prefix_size = action_info_get.size;
   action_info.prefix_value = hw_value[0];
   action_info.size = 0;
   dest_char = &(action_info.name[0]);
   sal_strncpy_s(dest_char, "sflow_eventor_port_a", sizeof(action_info.name));
   rv = bcm_field_action_create(unit, 0, &action_info, &cint_sflow_eventor_port_action_id);
   if (rv != BCM_E_NONE)
   {
      printf("%s: Error (%d), in bcm_field_action_create\n", proc_name, rv);
      return rv;
   }

   bcm_field_group_info_t_init(&field_group_info);
   field_group_info.fg_type = bcmFieldGroupTypeConst;
   field_group_info.stage = bcm_stage;

   /* Set quals */
   field_group_info.nof_quals = 0;

   /* Set actions */
   field_group_info.nof_actions = 2;
   field_group_info.action_types[0] = cint_sflow_extended_lif_action_id;
   field_group_info.action_types[1] = cint_sflow_eventor_port_action_id;
   field_group_info.action_with_valid_bit[0] = FALSE;
   field_group_info.action_with_valid_bit[1] = FALSE;

   dest_char = &(field_group_info.name[0]);
   sal_strncpy_s(dest_char, "sflow_extended_lif_fg", sizeof(field_group_info.name));

   rv = bcm_field_group_add(unit, 0, &field_group_info, &cint_sflow_extended_lif_field_fg_id);
   if (rv != BCM_E_NONE)
   {
      printf("%s: Error (%d), in bcm_field_group_add\n", proc_name, rv);
      return rv;
   }
    
   printf("%s: FG Id (%d) was add to iPMF1 Stage\n", proc_name, cint_sflow_extended_lif_field_fg_id);

   bcm_field_group_attach_info_t_init(&attach_info);
   attach_info.key_info.nof_quals = field_group_info.nof_quals;
   attach_info.payload_info.nof_actions = field_group_info.nof_actions;
   attach_info.payload_info.action_types[0] = field_group_info.action_types[0];
   attach_info.payload_info.action_types[1] = field_group_info.action_types[1];

   rv = bcm_field_group_context_attach(unit, 0, cint_sflow_extended_lif_field_fg_id, context_id, &attach_info);
   if (rv != BCM_E_NONE)
   {
      printf("%s: Error (%d), in bcm_field_group_context_attach\n", proc_name, rv);
      return rv;
   }

   printf("%s: FG Id (%d) in iPMF1 attached to context (%d)\n", proc_name, cint_sflow_extended_lif_field_fg_id, context_id);

   return rv;
}

/**
* \brief
*  Configuration FG with relevant qualifier/action/context.
*  For 2nd pass - set constant actions.
* \param [in] unit -  Device id 
* \param [in] context_id  -  Context to attach the FG
* \return
*   int - Error Type
* \remark
* This function creates one FG with one constant actions:
*   - drop the packet.
* It will be used in case the EXEM lookup fails
*  
* \see
*   * None
*/
int sflow_field_extended_gateway_second_pass_dip_const_create_and_attach(int unit, bcm_field_context_t context_id)
{
   int rv = BCM_E_NONE;
   bcm_field_action_info_t action_info, action_info_get;
   bcm_field_stage_t bcm_stage = bcmFieldStageIngressPMF1;
   bcm_field_action_t bcm_action = bcmFieldActionTrap;
   uint32 bcm_value[BCM_FIELD_ACTION_WIDTH_IN_WORDS] = {0};
   uint32 hw_value[BCM_FIELD_ACTION_WIDTH_IN_WORDS] = {0};
   void *dest_char;
   bcm_field_group_info_t field_group_info;
   bcm_field_group_attach_info_t attach_info;
   bcm_gport_t trap_gport;
   char *proc_name;

   proc_name = "sflow_field_extended_gateway_second_pass_dip_const_create_and_attach";

   BCM_GPORT_TRAP_SET(trap_gport, sflow_trap_id, 7, 0);
   bcm_value[0] = trap_gport;
   rv = bcm_field_action_value_map(unit, bcm_stage, bcm_action, bcm_value, hw_value);
   if (rv != BCM_E_NONE)
   {
      printf("%s: Error (%d), in bcm_field_action_value_map\n", proc_name, rv);
      return rv;
   }

   rv = bcm_field_action_info_get(unit, bcm_action, bcm_stage, &action_info_get);
   if (rv != BCM_E_NONE)
   {
      printf("%s: Error (%d), in bcm_field_action_info_get\n", proc_name, rv);
      return rv;
   }

   bcm_field_action_info_t_init(&action_info);
   action_info.action_type = bcm_action;
   action_info.stage = bcm_stage;
   action_info.prefix_size = action_info_get.size;
   action_info.prefix_value = hw_value[0];
   action_info.size = 0;
   dest_char = &(action_info.name[0]);
   sal_strncpy_s(dest_char, "sflow_trap_dip_a", sizeof(action_info.name));
   rv = bcm_field_action_create(unit, 0, &action_info, &cint_sflow_trap_dip_action_id);
   if (rv != BCM_E_NONE)
   {
      printf("%s: Error (%d), in bcm_field_action_create\n", proc_name, rv);
      return rv;
   }
    
   bcm_field_group_info_t_init(&field_group_info);
   field_group_info.fg_type = bcmFieldGroupTypeConst;
   field_group_info.stage = bcm_stage;

   /* Set quals */
   field_group_info.nof_quals = 0;

   /* Set actions */
   field_group_info.nof_actions = 1;
   field_group_info.action_types[0] = cint_sflow_trap_dip_action_id;
   field_group_info.action_with_valid_bit[0] = FALSE;

   dest_char = &(field_group_info.name[0]);
   sal_strncpy_s(dest_char, "sflow_trap_ip_fg", sizeof(field_group_info.name));

   rv = bcm_field_group_add(unit, 0, &field_group_info, &cint_sflow_trap_dip_field_fg_id);
   if (rv != BCM_E_NONE)
   {
      printf("%s: Error (%d), in bcm_field_group_add\n", proc_name, rv);
      return rv;
   }
   printf("%s: FG Id (%d) was add to iPMF1 Stage\n", proc_name, cint_sflow_trap_dip_field_fg_id);

   bcm_field_group_attach_info_t_init(&attach_info);
   attach_info.key_info.nof_quals = field_group_info.nof_quals;
   attach_info.payload_info.nof_actions = field_group_info.nof_actions;
   attach_info.payload_info.action_types[0] = field_group_info.action_types[0];
   attach_info.payload_info.action_info[0].priority = cint_sflow_trap_dip_priority;
   rv = bcm_field_group_context_attach(unit, 0, cint_sflow_trap_dip_field_fg_id, context_id, &attach_info);
   if (rv != BCM_E_NONE)
   {
      printf("%s: Error (%d), in bcm_field_group_context_attach\n", proc_name, rv);
      return rv;
   }

   printf("%s: FG Id (%d) in iPMF1 attached to context (%d)\n", proc_name, cint_sflow_trap_dip_field_fg_id, context_id);

   return rv;
}

/**
* \brief
*  Configuration FG with relevant qualifier/action/context.
*  For 2nd pass - set constant actions.
* \param [in] unit - Device id 
* \param [in] context_id  -  Context to attach the FG
* \return
*   int - Error Type
* \remark
* This function creates one FG with one constant actions:
*   - drop the packet.
* It will be used in case the EXEM lookup fails
*  
* \see
*   * None
*/
int sflow_field_extended_gateway_second_pass_sip_const_create_and_attach(int unit, bcm_field_context_t context_id)
{
   int rv = BCM_E_NONE;
   bcm_field_action_info_t action_info, action_info_get;
   bcm_field_stage_t bcm_stage = bcmFieldStageIngressPMF2;
   bcm_field_action_t bcm_action = bcmFieldActionTrap;
   uint32 bcm_value[BCM_FIELD_ACTION_WIDTH_IN_WORDS] = {0};
   uint32 hw_value[BCM_FIELD_ACTION_WIDTH_IN_WORDS] = {0};
   void *dest_char;
   bcm_field_group_info_t field_group_info;
   bcm_field_group_attach_info_t attach_info;
   bcm_gport_t trap_gport;
   char *proc_name;

   proc_name = "sflow_field_extended_gateway_second_pass_sip_const_create_and_attach";

   BCM_GPORT_TRAP_SET(trap_gport, sflow_trap_id, 7, 0);
   bcm_value[0] = trap_gport;
   rv = bcm_field_action_value_map(unit, bcm_stage, bcm_action, bcm_value, hw_value);
   if (rv != BCM_E_NONE)
   {
      printf("%s: Error (%d), in bcm_field_action_value_map\n", proc_name, rv);
      return rv;
   }

   rv = bcm_field_action_info_get(unit, bcm_action, bcm_stage, &action_info_get);
   if (rv != BCM_E_NONE)
   {
      printf("%s: Error (%d), in bcm_field_action_info_get\n", proc_name, rv);
      return rv;
   }

   bcm_field_action_info_t_init(&action_info);
   action_info.action_type = bcm_action;
   action_info.stage = bcm_stage;
   action_info.prefix_size = action_info_get.size;
   action_info.prefix_value = hw_value[0];
   action_info.size = 0;
   dest_char = &(action_info.name[0]);
   sal_strncpy_s(dest_char, "sflow_trap_sip_a", sizeof(action_info.name));
   rv = bcm_field_action_create(unit, 0, &action_info, &cint_sflow_trap_sip_action_id);
   if (rv != BCM_E_NONE)
   {
      printf("%s: Error (%d), in bcm_field_action_create\n", proc_name, rv);
      return rv;
   }

   bcm_field_group_info_t_init(&field_group_info);
   field_group_info.fg_type = bcmFieldGroupTypeConst;
   field_group_info.stage = bcm_stage;

   /* Set quals */
   field_group_info.nof_quals = 0;

   /* Set actions */
   field_group_info.nof_actions = 1;
   field_group_info.action_types[0] = cint_sflow_trap_sip_action_id;
   field_group_info.action_with_valid_bit[0] = FALSE;

   dest_char = &(field_group_info.name[0]);
   sal_strncpy_s(dest_char, "sflow_trap_ip_fg", sizeof(field_group_info.name));

   rv = bcm_field_group_add(unit, 0, &field_group_info, &cint_sflow_trap_sip_field_fg_id);
   if (rv != BCM_E_NONE)
   {
      printf("%s: Error (%d), in bcm_field_group_add\n", proc_name, rv);
      return rv;
   }
   printf("%s: FG Id (%d) was add to iPMF2 Stage\n", proc_name, cint_sflow_trap_sip_field_fg_id);

   bcm_field_group_attach_info_t_init(&attach_info);
   attach_info.key_info.nof_quals = field_group_info.nof_quals;
   attach_info.payload_info.nof_actions = field_group_info.nof_actions;
   attach_info.payload_info.action_types[0] = field_group_info.action_types[0];
   attach_info.payload_info.action_info[0].priority = cint_sflow_trap_sip_priority;

   rv = bcm_field_group_context_attach(unit, 0, cint_sflow_trap_sip_field_fg_id, context_id, &attach_info);
   if (rv != BCM_E_NONE)
   {
      printf("%s: Error (%d), in bcm_field_group_context_attach\n", proc_name, rv);
      return rv;
   }

   printf("%s: FG Id (%d) in iPMF2 attached to context (%d)\n", proc_name, cint_sflow_trap_sip_field_fg_id, context_id);

   return rv;
}

/**
* \brief
* Configuration of FG with relevant qualifier/action/context.
* For 2nd pass - set EXEM lookup.
* \param [in] unit            -  Device id 
* \param [in] context_id  -  Context to attach the FG
* \return
*   int - Error Type
* \remark
*  Set EXEM lookup:
*   - Key = DIP Destination (Fwd_Action_Dst)
*   - Result = outlif1
*  
* \see
*   * None
*/
int sflow_field_extended_gateway_second_pass_exem_dip_create_and_attach(int unit, bcm_field_context_t context_id)
{
   int rv = BCM_E_NONE;
   bcm_field_action_info_t action_info, action_info_get;
   bcm_field_stage_t bcm_stage = bcmFieldStageIngressPMF1;
   void *dest_char;
   bcm_field_group_info_t field_group_info;
   bcm_field_group_attach_info_t attach_info;
   bcm_gport_t trap_gport;
   char *proc_name;

   proc_name = "sflow_field_extended_gateway_second_pass_exem_dip_create_and_attach";

   bcm_field_group_info_t_init(&field_group_info);
   field_group_info.fg_type = bcmFieldGroupTypeExactMatch;
   field_group_info.stage = bcm_stage;

   /* Set quals */
   field_group_info.nof_quals = 1;
   field_group_info.qual_types[0] = bcmFieldQualifyDstPort;

   /* Set actions
      0. Disable the drop (Invalidate next) in case the EXEM lookup successed.
      1. Set Exem lookup action - set outlif 1
      */
   field_group_info.nof_actions = 2;
   field_group_info.action_types[0] = bcmFieldActionInvalidNext;
   field_group_info.action_types[1] = bcmFieldActionOutVport1Raw;
   
   dest_char = &(field_group_info.name[0]);
   sal_strncpy_s(dest_char, "sflow_extended_dip_fg", sizeof(field_group_info.name));

   rv = bcm_field_group_add(unit, 0, &field_group_info, &cint_sflow_extended_dip_field_fg_id);
   if (rv != BCM_E_NONE)
   {
      printf("%s: Error (%d), in bcm_field_group_add\n", proc_name, rv);
      return rv;
   }
   printf("%s: FG Id (%d) was add to iPMF1 Stage\n", proc_name, cint_sflow_extended_dip_field_fg_id);

   bcm_field_group_attach_info_t_init(&attach_info);
   attach_info.key_info.nof_quals = field_group_info.nof_quals;
   attach_info.key_info.qual_types[0] = field_group_info.qual_types[0];
   attach_info.key_info.qual_info[0].input_type = bcmFieldInputTypeMetaData;
   attach_info.payload_info.nof_actions = field_group_info.nof_actions;
   attach_info.payload_info.action_types[0] = field_group_info.action_types[0];
   attach_info.payload_info.action_types[1] = field_group_info.action_types[1];
   attach_info.payload_info.action_info[0].priority = cint_sflow_exem_dip_invalidate_next_priority;

   rv = bcm_field_group_context_attach(unit, 0, cint_sflow_extended_dip_field_fg_id, context_id, &attach_info);
   if (rv != BCM_E_NONE)
   {
      printf("%s: Error (%d), in bcm_field_group_context_attach\n", proc_name, rv);
      return rv;
   }

   printf("%s: FG Id (%d) in iPMF1 attached to context (%d)\n", proc_name, cint_sflow_extended_dip_field_fg_id, context_id);

   return rv;
}

/**
* \brief
* Configuration of FG with relevant qualifier/action/context.
* For 2nd pass - set EXEM lookup.
* \param [in] unit            -  Device id 
* \param [in] context_id  -  Context to attach the FG
* \return
*   int - Error Type
* \remark
*  Set EXEM lookup:
*   - Key = SIP Destination (RPF_Dst )
*   - Result = outlif2
*  
* \see
*   * None
*/
int sflow_field_extended_gateway_second_pass_exem_sip_create_and_attach(int unit, bcm_field_context_t context_id)
{
   int rv = BCM_E_NONE;
   bcm_field_action_info_t action_info, action_info_get;
   bcm_field_stage_t bcm_stage = bcmFieldStageIngressPMF2;
   void *dest_char;
   bcm_field_group_info_t field_group_info;
   bcm_field_group_attach_info_t attach_info;
   bcm_gport_t trap_gport;
   char *proc_name;

   proc_name = "sflow_field_extended_gateway_second_pass_exem_sip_create_and_attach";

   bcm_field_group_info_t_init(&field_group_info);
   field_group_info.fg_type = bcmFieldGroupTypeExactMatch;
   field_group_info.stage = bcm_stage;

   /* Set quals */
   field_group_info.nof_quals = 1;
   field_group_info.qual_types[0] = bcmFieldQualifyDstRpfGport;

   /* Set actions
      0. Disable the drop (Invalidate next) in case the EXEM lookup successed.
      1. Set Exem lookup action - set outlif 2
      */
   field_group_info.nof_actions = 2;
   field_group_info.action_types[0] = bcmFieldActionInvalidNext;
   field_group_info.action_types[1] = bcmFieldActionOutVport2Raw;
   
   dest_char = &(field_group_info.name[0]);
   sal_strncpy_s(dest_char, "sflow_extended_sip_fg", sizeof(field_group_info.name));

   rv = bcm_field_group_add(unit, 0, &field_group_info, &cint_sflow_extended_sip_field_fg_id);
   if (rv != BCM_E_NONE)
   {
      printf("%s: Error (%d), in bcm_field_group_add\n", proc_name, rv);
      return rv;
   }
   printf("%s: FG Id (%d) was add to iPMF2 Stage\n", proc_name, cint_sflow_extended_sip_field_fg_id);

   bcm_field_group_attach_info_t_init(&attach_info);
   attach_info.key_info.nof_quals = field_group_info.nof_quals;
   attach_info.key_info.qual_types[0] = field_group_info.qual_types[0];
   attach_info.key_info.qual_info[0].input_type = bcmFieldInputTypeMetaData;
   attach_info.payload_info.nof_actions = field_group_info.nof_actions;
   attach_info.payload_info.action_types[0] = field_group_info.action_types[0];
   attach_info.payload_info.action_types[1] = field_group_info.action_types[1];
   attach_info.payload_info.action_info[0].priority = cint_sflow_exem_sip_invalidate_next_priority;

   rv = bcm_field_group_context_attach(unit, 0, cint_sflow_extended_sip_field_fg_id, context_id, &attach_info);
   if (rv != BCM_E_NONE)
   {
      printf("%s: Error (%d), in bcm_field_group_context_attach\n", proc_name, rv);
      return rv;
   }

   printf("%s: FG Id (%d) in iPMF2 attached to context (%d)\n", proc_name, cint_sflow_extended_sip_field_fg_id, context_id);

   return rv;
}


/**
* \brief
*  Create PMF context
* \param [in] unit - Device id
* \param [in] context_id - Context ID
* \param [in] context_name - Context name
* \param [in] is_first_pass - is first pass?
* \return
*   int - Error Type
* \remark 
* None  
* \see
* None
*/
int cint_sflow_field_extended_gateway_group_context_create(int unit, bcm_field_context_t *context_id, char *context_name, int is_first_pass)
{
   int rv = BCM_E_NONE;
   bcm_field_context_info_t context_info;
   bcm_field_context_param_info_t context_param;
   void *dest_char;
   char *proc_name;

   proc_name = "cint_sflow_field_extended_gateway_group_context_create";
  
   bcm_field_context_info_t_init(&context_info);
   dest_char = &(context_info.name[0]);
   sal_strncpy_s(dest_char, context_name, sizeof(context_info.name));
   rv = bcm_field_context_create(unit, 0, bcmFieldStageIngressPMF1, &context_info, context_id);
   if (rv != BCM_E_NONE)
   {
      printf("%s: Error (%d), in bcm_field_context_create\n", proc_name, rv);
      return rv;
   }

   /* Defining System header profiles of the context - so the PTCH layer will be deleted */
   bcm_field_context_param_info_t_init(&context_param);
   context_param.param_type = bcmFieldContextParamTypeSystemHeaderProfile;
   context_param.param_val = bcmFieldSystemHeaderProfileFtmhTshPphUdh;
   rv = bcm_field_context_param_set(unit,0,bcmFieldStageIngressPMF1,*context_id,&context_param);
   if (rv != BCM_E_NONE)
   {
      printf("%s: Error (%d), in bcm_field_context_param_set system header info\n", proc_name, rv);
      return rv;
   }

   bcm_field_context_param_info_t_init(&context_param);
   context_param.param_type = bcmFieldContextParamTypeSystemHeaderStrip;
   context_param.param_val = BCM_FIELD_PACKET_STRIP(bcmFieldPacketRemoveLayerOffset1, 0);
   rv = bcm_field_context_param_set(unit,0,bcmFieldStageIngressPMF2,*context_id,&context_param);
   if (rv != BCM_E_NONE)
   {
      printf("%s: Error (%d), in bcm_field_context_param_set system header info\n", proc_name, rv);
      return rv;
   }

   if (!is_first_pass) {

      /* In 2nd pass, need to disable termination of SFLOW Header Record and the In/Out-Port Data
         and only terminate the PTCH1 header. Thus, setting "bytes to remove" to 3 (the size of the PTCH1 header) */
      int bytes_to_remove = 3;
      bcm_field_context_param_info_t_init(&context_param);
      context_param.param_type = bcmFieldContextParamTypeSystemHeaderStrip;
      context_param.param_val = BCM_FIELD_PACKET_STRIP(bcmFieldPacketRemoveLayerOffset0, bytes_to_remove);
      rv = bcm_field_context_param_set(unit,0,bcmFieldStageIngressPMF2,*context_id,&context_param);
      if (rv != BCM_E_NONE)
      {
          printf("Error (%d), in bcm_field_context_param_set system header info\n", rv);
          return rv;
      }

      printf("%s: Set bytes-to-remove to %d.\n", proc_name, bytes_to_remove);
   }
   
   return rv;
}


/**
* \brief
*  Create PMF3 context
* \param [in] unit - Device id
* \param [in] context_id - Context ID
* \param [in] context_name - Context name
* \param [in] is_first_pass - is first pass?
* \return
*   int - Error Type
* \remark 
* IPMF3 is used for disabling ITPP termination on the 2nd pass 
* (so the SFLOW Header Record plus In-Port and Out-port values 
* are passed to the Egress) 
* \see
* None
*/
int cint_sflow_field_extended_gateway_group_context_ipmf3_create(int unit, bcm_field_context_t *context_id, char *context_name)
{
   int rv = BCM_E_NONE;
   bcm_field_context_info_t context_info;
   void *dest_char;
   char *proc_name;

   proc_name = "cint_sflow_field_extended_gateway_group_context_ipmf3_create";
  
   bcm_field_context_info_t_init(&context_info);
   dest_char = &(context_info.name[0]);
   sal_strncpy_s(dest_char, context_name, sizeof(context_info.name));
   rv = bcm_field_context_create(unit, 0, bcmFieldStageIngressPMF3, &context_info, context_id);
   if (rv != BCM_E_NONE)
   {
      printf("%s: Error (%d), in bcm_field_context_create\n", proc_name, rv);
      return rv;
   }

   return rv;
}

/**
* \brief
*  Set PMF3 context preselector information
* \param [in] unit            -  Device id
* \param [in] ipmf3_context_id      -  IPMF3 Context ID 
* \param [in] ipmf2_context_id      -  IPMF2 Context ID context 
*  
* \return 
*   int - Error Type
* \remark 
*  IPMF3 context is selected based on IPMF2 context
* \see
* None
*/
int cint_sflow_field_extended_gateway_group_context_ipmf3_presel_set(int unit, bcm_field_context_t ipmf3_context_id, bcm_field_context_t ipmf2_context_id)
{
   int rv = BCM_E_NONE;
   bcm_field_presel_entry_data_t presel_data;
   bcm_field_presel_entry_id_t presel_entry_id;
   uint32 layer_type;
   char *proc_name;

   proc_name = "cint_sflow_field_extended_gateway_group_context_ipmf3_presel_set";

   bcm_field_presel_entry_id_info_init(&presel_entry_id);
   bcm_field_presel_entry_data_info_init(&presel_data);
    
   /* Select IPMF3 context to be selected based on IPMF2 context */
   presel_entry_id.presel_id = ipmf3_presel_id;
   presel_entry_id.stage = bcmFieldStageIngressPMF3;

   presel_data.entry_valid = TRUE;
   presel_data.context_id = ipmf3_context_id;
   presel_data.nof_qualifiers = 1;

   presel_data.qual_data[0].qual_type = bcmFieldQualifyContextId; 
   presel_data.qual_data[0].qual_value = ipmf2_context_id;
   presel_data.qual_data[0].qual_mask = 0x3F;   /* 6 bits*/

   rv = bcm_field_presel_set(unit, 0, &presel_entry_id, &presel_data);
   if (rv != BCM_E_NONE)
   {
      printf("%s: Error (%d), in bcm_field_presel_set(sflow recycle port)\n", proc_name, rv);
      return rv;
   }

   return rv;
}

/**
* \brief
*  Configuration FG with relevant qualifier/action/context.
* \param [in] unit - Device id 
* \param [in] context_id  -  Context to attach the FG
* \return
*   int - Error Type
* \remark
* This function creates one FG with one constant action:
*   - sets Parsing start Offset to 0.
*  Thus, ITPP termination will not terminate SFLOW Header Record
*  + In/Out-Port value.
* \see
*   * None
*/
int sflow_field_extended_gateway_second_pass_ipmf3_const_create_and_attach(int unit, bcm_field_context_t context_id)
{
   int rv = BCM_E_NONE;
   bcm_field_action_info_t action_info, action_info_get;
   bcm_field_stage_t bcm_stage = bcmFieldStageIngressPMF3;
   bcm_field_action_t bcm_action = bcmFieldActionParsingStartOffset;
   uint32 bcm_value[BCM_FIELD_ACTION_WIDTH_IN_WORDS] = {0};
   uint32 hw_value[BCM_FIELD_ACTION_WIDTH_IN_WORDS] = {0};
   void *dest_char;
   bcm_field_group_info_t field_group_info;
   bcm_field_group_attach_info_t attach_info;
   char *proc_name;

   proc_name = "sflow_field_extended_gateway_second_pass_ipmf3_const_create_and_attach";


   rv = bcm_field_action_info_get(unit, bcm_action, bcm_stage, &action_info_get);
   if (rv != BCM_E_NONE)
   {
      printf("%s: Error (%d), in bcm_field_action_info_get\n", proc_name, rv);
      return rv;
   }

    /* Parsing Start Offset value = 0*/
   bcm_value[0] = 0;
   rv = bcm_field_action_value_map(unit, bcm_stage, bcm_action, bcm_value, hw_value);
   if (rv != BCM_E_NONE)
   {
      printf("%s: Error (%d), in bcm_field_action_value_map\n", proc_name, rv);
      return rv;
   }
        

   bcm_field_action_info_t_init(&action_info);
   action_info.action_type = bcm_action;
   action_info.stage = bcm_stage;
   action_info.prefix_size = action_info_get.size;
   action_info.prefix_value = hw_value[0];
   action_info.size = 0;
   dest_char = &(action_info.name[0]);
   sal_strncpy_s(dest_char, "sflow_parsing_start_offset_a", sizeof(action_info.name));
   rv = bcm_field_action_create(unit, 0, &action_info, &cint_sflow_parsing_start_offset_action_id);
   if (rv != BCM_E_NONE)
   {
      printf("%s: Error (%d), in bcm_field_action_create\n", proc_name, rv);
      return rv;
   }

   bcm_field_group_info_t_init(&field_group_info);
   field_group_info.fg_type = bcmFieldGroupTypeConst;
   field_group_info.stage = bcm_stage;

   /* Set quals */
   field_group_info.nof_quals = 0;

   /* Set actions */
   field_group_info.nof_actions = 1;
   field_group_info.action_types[0] = cint_sflow_parsing_start_offset_action_id;
   field_group_info.action_with_valid_bit[0] = FALSE;


   dest_char = &(field_group_info.name[0]);
   sal_strncpy_s(dest_char, "sflow_parsing_start_offset_fg", sizeof(field_group_info.name));

   rv = bcm_field_group_add(unit, 0, &field_group_info, &cint_sflow_parsing_start_offset_fg_id);
   if (rv != BCM_E_NONE)
   {
      printf("%s: Error (%d), in bcm_field_group_add\n", proc_name, rv);
      return rv;
   }
    
   printf("%s: FG Id (%d) was add to iPMF3 Stage\n", proc_name, cint_sflow_parsing_start_offset_fg_id);

   bcm_field_group_attach_info_t_init(&attach_info);
   attach_info.key_info.nof_quals = field_group_info.nof_quals;
   attach_info.payload_info.nof_actions = field_group_info.nof_actions;
   attach_info.payload_info.action_types[0] = field_group_info.action_types[0];


   rv = bcm_field_group_context_attach(unit, 0, cint_sflow_parsing_start_offset_fg_id, context_id, &attach_info);
   if (rv != BCM_E_NONE)
   {
      printf("%s: Error (%d), in bcm_field_group_context_attach\n", proc_name, rv);
      return rv;
   }

   printf("%s: FG Id (%d) in iPMF3 attached to context (%d)\n", proc_name, cint_sflow_parsing_start_offset_fg_id, context_id);

   return rv;
}


/**
* \brief
*  Set PMF context preselector information
* \param [in] unit            -  Device id
* \param [in] context_id      -  Context ID 
* \param [in] is_first_pass   -  is it "Sflow IPoETH 1st pass" 
*        context
*  
* \return 
*   int - Error Type
* \remark 
* For 1st pass - context preselector: 
*  1. Route IPv4: (FWD layer -1 == ETH) && (FWD layer == IPv4)
*  2. Route IPv6: (FWD layer -1 == ETH) && (FWD layer == IPv6)
* For 2nd pass - context preselector: 
*  The packet is ingressed via the sflow recycle port
* \remark 
* Bridge packet will be skipped since for SFLOW IPoETH, on the 
* 2nd pass, there is EXEM lookup based on IP FWD decision 
* (DIP-destination and SIP-destination) 
*  
* \see
* None
*/
int cint_sflow_field_extended_gateway_group_context_presel_set(int unit, bcm_field_context_t context_id, int is_first_pass)
{
   int rv = BCM_E_NONE;
   bcm_field_presel_entry_data_t presel_data;
   bcm_field_presel_entry_id_t presel_entry_id;
   uint32 layer_type;
   char *proc_name;

   proc_name = "cint_sflow_field_extended_gateway_group_context_create";

   bcm_field_presel_entry_id_info_init(&presel_entry_id);
   bcm_field_presel_entry_data_info_init(&presel_data);
    
   if (is_first_pass == 1) {

      /* route IPv4:*/
      presel_entry_id.presel_id = route_ipv4_presel_id;
      presel_entry_id.stage = bcmFieldStageIngressPMF1;

      presel_data.entry_valid = TRUE;
      presel_data.context_id = context_id;
      presel_data.nof_qualifiers = 2;

      presel_data.qual_data[0].qual_type = bcmFieldQualifyForwardingType; 
      presel_data.qual_data[0].qual_value = bcmFieldLayerTypeEth;
      presel_data.qual_data[0].qual_mask = 0x1F;
      presel_data.qual_data[0].qual_arg = -1; /* FWD layer -1*/

      presel_data.qual_data[1].qual_type = bcmFieldQualifyForwardingType; 
      presel_data.qual_data[1].qual_value = bcmFieldLayerTypeIp4;
      presel_data.qual_data[1].qual_mask = 0x1F;
      presel_data.qual_data[1].qual_arg = 0; /* FWD layer */

      rv = bcm_field_presel_set(unit, 0, &presel_entry_id, &presel_data);
      if (rv != BCM_E_NONE)
      {
         printf("%s: Error (%d), in bcm_field_presel_set(route IPv4)\n", proc_name, rv);
         return rv;
      }

      /* route IPv6:*/
      presel_entry_id.presel_id = route_ipv6_presel_id;
      presel_entry_id.stage = bcmFieldStageIngressPMF1;

      presel_data.entry_valid = TRUE;
      presel_data.context_id = context_id;
      presel_data.nof_qualifiers = 2;

      presel_data.qual_data[0].qual_type = bcmFieldQualifyForwardingType; 
      presel_data.qual_data[0].qual_value = bcmFieldLayerTypeEth;
      presel_data.qual_data[0].qual_mask = 0x1F;
      presel_data.qual_data[0].qual_arg = -1; /* FWD layer -1*/

      presel_data.qual_data[1].qual_type = bcmFieldQualifyForwardingType; 
      presel_data.qual_data[1].qual_value = bcmFieldLayerTypeIp6;
      presel_data.qual_data[1].qual_mask = 0x1F;
      presel_data.qual_data[1].qual_arg = 0; /* FWD layer */

      rv = bcm_field_presel_set(unit, 0, &presel_entry_id, &presel_data);
      if (rv != BCM_E_NONE)
      {
         printf("%s: Error (%d), in bcm_field_presel_set(route IPv6)\n", proc_name, rv);
         return rv;
      }
   }
   else {
      /* 2nd pass: packet is coming from sflow-recycle-port (based on it's port profile) */

      presel_entry_id.presel_id = sflow_recycle_port_presel_id;
      presel_entry_id.stage = bcmFieldStageIngressPMF1;

      presel_data.entry_valid = TRUE;
      presel_data.context_id = context_id;
      presel_data.nof_qualifiers = 1;

      presel_data.qual_data[0].qual_type = bcmFieldQualifyPortClassTrafficManagement; 
      presel_data.qual_data[0].qual_value = cint_sflow_extended_gateway_info.sflow_recycle_port_profile;
      presel_data.qual_data[0].qual_mask = 0x7;   /* 3 bits*/

      rv = bcm_field_presel_set(unit, 0, &presel_entry_id, &presel_data);
      if (rv != BCM_E_NONE)
      {
         printf("%s: Error (%d), in bcm_field_presel_set(sflow recycle port)\n", proc_name, rv);
         return rv;
      }
   }

   return rv;
}

/**
* \brief
*  Adds EXEM entry
* \param [in] unit - Device id
* \param [in] is_dip - is DIP?
* \param [in] dst - the DIP/SIP destination
* \param [in] sflow_encap_extended_id - the sflow encap-id
* \return
*   int - Error Type
* \remark 
*   The EXEM lookup:
*   Key = dst (DIP/SIP destination)
*   Result = sflow_encap_extended_id
*   (sflow_encap_extended_dst_id or sflow_encap_extended_src_id)
*  
* \see
*   * None
*/
int cint_sflow_field_extended_gateway_entry_add(int unit, int is_dip, int dst, int sflow_encap_extended_id)
{
   int rv = BCM_E_NONE;
   bcm_field_entry_info_t ent_info;
   bcm_field_group_t fg_id;
   char *proc_name;

   proc_name = "cint_sflow_field_extended_gateway_entry_add";

   bcm_field_entry_info_t_init(&ent_info);

   ent_info.nof_entry_quals = 1;

   ent_info.entry_qual[0].type = is_dip? bcmFieldQualifyDstPort:bcmFieldQualifyDstRpfGport;
   ent_info.entry_qual[0].value[0] = dst; /* need 21 bits or 20 bits ?*/


   ent_info.nof_entry_actions = 2;
   ent_info.entry_action[0].type = bcmFieldActionInvalidNext;
   ent_info.entry_action[1].type = is_dip? bcmFieldActionOutVport1Raw:bcmFieldActionOutVport2Raw;
   ent_info.entry_action[1].value[0] = sflow_encap_extended_id;

   fg_id = is_dip? cint_sflow_extended_dip_field_fg_id:cint_sflow_extended_sip_field_fg_id;

   rv = bcm_field_entry_add(unit, 0, fg_id, &ent_info, NULL);
   if (rv != BCM_E_NONE)
   {
      printf("%s: Error (%d), in bcm_field_entry_add\n", proc_name, rv);
      return rv;
   }

   printf("%s: EXEM entry is added: key = 0x%08X, result = 0x%08X \n", proc_name, dst, sflow_encap_extended_id);
   
   return rv;
}

/**
* \brief
*  Delete EXEM entry
* \param [in] unit - Device id
* \param [in] is_dip - is DIP?
* \param [in] dst - the DIP/SIP destination
* \return
*   int - Error Type
* \remark 
*   The EXEM lookup:
*   Key = dst (DIP/SIP destination)
*  
* \see
*   * None
*/
int cint_sflow_field_extended_gateway_entry_delete(int unit, int is_dip, int dst)
{
   int rv = BCM_E_NONE;
   bcm_field_group_t fg_id;
   bcm_field_entry_qual_t entry_qual_info[BCM_FIELD_NUMBER_OF_QUALS_PER_GROUP];

   char *proc_name;

   proc_name = "cint_sflow_field_extended_gateway_entry_delete";

   bcm_field_entry_qual_t_init(entry_qual_info);

   entry_qual_info[0].type = is_dip? bcmFieldQualifyDstPort:bcmFieldQualifyDstRpfGport;
   entry_qual_info[0].value[0] = dst; /* need 21 bits or 20 bits ?*/


   fg_id = is_dip? cint_sflow_extended_dip_field_fg_id:cint_sflow_extended_sip_field_fg_id;

   rv = bcm_field_entry_delete(unit, fg_id, entry_qual_info, 0);
   if (rv != BCM_E_NONE)
   {
      printf("Error (%d), in bcm_field_entry_delete\n", rv);
      return rv;
   }

   printf("%s: EXEM entry is deleted: key = 0x%08X\n", proc_name, dst);
   
   return rv;
}


/**
* \brief
*  Destroy configuration.
*   Detach context from FG
*   Clear context preselector
*   Delete FG
* \param [in] unit         -  Device id
* \param [in] context_id   -  Context ID  
* \return
*   int - Error Type
* \remark
*   * None
* \see
*   * None
*/
int cint_sflow_field_extended_gateway_destroy(int unit,bcm_field_context_t context_id, int nof_cores)
{
   int rv = BCM_E_NONE;
   bcm_field_presel_entry_data_t presel_data;
   bcm_field_presel_entry_id_t presel_entry_id;
   int core_id;
   char *proc_name;

   proc_name = "cint_sflow_field_extended_gateway_destroy";

   /* Detach the IPMF FG from its context */
   rv = bcm_field_group_context_detach(unit, cint_sflow_snoop_field_fg_id, context_id);
   if (rv != BCM_E_NONE)
   {
      printf("%s: Error (%d), bcm_field_group_context_detach(fg = %d, context_id = %d)\n", proc_name, rv, cint_sflow_snoop_field_fg_id, context_id);
      return rv;
   }

   /* Clear context preselector */
   bcm_field_presel_entry_id_info_init(&presel_entry_id);
   bcm_field_presel_entry_data_info_init(&presel_data);

   presel_entry_id.presel_id = route_ipv4_presel_id;
   presel_entry_id.stage = bcmFieldStageIngressPMF1;
   rv = bcm_field_presel_set(unit, 0, &presel_entry_id, &presel_data);
   if (rv != BCM_E_NONE)
   {
      printf("%s: Error (%d), in bcm_field_presel_set(route IPv4)\n", proc_name, rv);
      return rv;
   }

   presel_entry_id.presel_id = route_ipv6_presel_id;
   rv = bcm_field_presel_set(unit, 0, &presel_entry_id, &presel_data);
   if (rv != BCM_E_NONE)
   {
      printf("%s: Error (%d), in bcm_field_presel_set(bridge IPv6)\n", proc_name, rv);
      return rv;
   }

   rv = bcm_field_context_destroy(unit, bcmFieldStageIngressPMF1, context_id);
   if (rv != BCM_E_NONE)
   {
      printf("%s: Error (%d), in bcm_field_context_destroy(context_id = %d)\n", proc_name, rv, context_id);
      return rv;
   }

   for (core_id = 0; core_id < nof_cores; core_id ++) {

      rv = bcm_field_entry_delete(unit, cint_sflow_snoop_field_fg_id, NULL, cint_cflow_core_id_entry_id[core_id]);
      if (rv != BCM_E_NONE)
      {
         printf("%s: Error (%d), in bcm_field_entry_delete core_id = %d \n", proc_name, rv, core_id);
         return rv;
      }
   }

   if (cint_cflow_eventor_port_entry_id != BCM_FIELD_INVALID) {
      rv = bcm_field_entry_delete(unit, cint_sflow_snoop_field_fg_id, NULL, cint_cflow_eventor_port_entry_id);
      if (rv != BCM_E_NONE)
      {
         printf("%s: Error (%d), in bcm_field_entry_delete cint_cflow_eventor_port_entry_id = %d \n", proc_name, rv, cint_cflow_eventor_port_entry_id);
         return rv;
      }

   }

   rv = bcm_field_group_delete(unit, cint_sflow_snoop_field_fg_id);
   if (rv != BCM_E_NONE)
   {
      printf("%s: Error (%d), in bcm_field_group_delete(fg_id = %d)\n", proc_name, rv, cint_sflow_snoop_field_fg_id);
      return rv;
   }

   rv = cint_sflow_field_extended_gateway_entry_delete(unit, 1, cint_sflow_basic_info.kaps_result);
   if (rv != BCM_E_NONE)
   {
      printf("%s: Error (%d), in cint_sflow_field_extended_gateway_entry_delete(DIP)\n", proc_name, rv);
      return rv;
   }

   rv = cint_sflow_field_extended_gateway_entry_delete(unit, 0, cint_sflow_basic_info.kaps_result);
   if (rv != BCM_E_NONE)
   {
      printf("%s: Error (%d), in cint_sflow_field_extended_gateway_entry_delete(SIP)\n", proc_name, rv);
      return rv;
   }
   

    return rv;
}
