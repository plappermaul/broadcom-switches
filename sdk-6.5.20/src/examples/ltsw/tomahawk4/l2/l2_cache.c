/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */
/*
 *
 * Purpose:  Test the l2 cache.
 *
 * Description: Select 3 ports, send unicast packets from one port to the others, make sure
 * l2 cache can be used to forward packets.
 *
 * Usage : BCM.0> cint l2_cache.c
 *
 * Config : bcm56990_a0-generic-64x400.config.yml + bcm56990_a0-generic-system_port.config.yml
 *
 * Verification method: Check port counters of 3 Ports.
 * Ingress port will send two packets, one packet will be forwarded to egress port1,
 * another packet will be forwarded to egress port2.
 */

cint_reset();

bcm_port_t ingress_port;
bcm_port_t egress_port1, egress_port2;
int index_used;
bcm_l2_cache_addr_t l2cache_addr;
bcm_vlan_t vid = 1;

/*
 * This function checked if given port/index is already present
 * in the list so that same ports number is not generated again
*/
int checkPortAssigned(int *port_index_list,int no_entries, int index)
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
bcm_error_t portNumbersGet(int unit, int *port_list_ptr, int num_ports)
{
  int i = 0, port = 0,rv = 0, index = 0;
  bcm_port_config_t configP;
  bcm_pbmp_t ports_pbmp;
  int tempports[BCM_PBMP_PORT_MAX];
  int port_index_list[num_ports];

  rv = bcm_port_config_get(unit,&configP);
  if(BCM_FAILURE(rv)) {
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
    if (checkPortAssigned(port_index_list, i, index) == 0)
    {
      port_list_ptr[i] = tempports[index];
      port_index_list[i] = index;
    } else {
      i = i - 1;
    }
  }

  return BCM_E_NONE;
}

/*
 * This function is pre-test setup.
 */
int testSetup(int unit)
{
  bcm_l2_addr_t addr;
  int lb_mode = BCM_PORT_LOOPBACK_MAC;
  int port_list[3];
  uint8 des_mac[6] = {0x00, 0xaa, 0xaa, 0xaa, 0xaa, 0x05};

  if (BCM_E_NONE != portNumbersGet(unit, port_list, 3)) {
    printf("portNumbersGet() failed\n");
    return -1;
  }

  ingress_port = port_list[0];
  egress_port1 = port_list[1];
  egress_port2 = port_list[2];
  printf("Ingress port: %d\n",ingress_port);
  printf("Egress port1: %d\n",egress_port1);
  printf("Egress port2: %d\n",egress_port2);

  /* Get the default vlan. */
  BCM_IF_ERROR_RETURN(bcm_vlan_default_get(unit, &vid));

  bcm_l2_cache_addr_t_init(&l2cache_addr);
  l2cache_addr.flags = BCM_L2_CACHE_DESTPORTS;
  sal_memcpy(l2cache_addr.mac, des_mac, sizeof(des_mac));
  l2cache_addr.mac_mask[0] = 0xff;
  l2cache_addr.mac_mask[1] = 0xff;
  l2cache_addr.mac_mask[2] = 0xff;
  l2cache_addr.mac_mask[3] = 0xff;
  l2cache_addr.mac_mask[4] = 0xff;
  l2cache_addr.mac_mask[5] = 0xff;

  l2cache_addr.vlan = vid;
  l2cache_addr.vlan_mask = 0xfff;

  /* Traffic test starts here - set port mac loopback mode. */
  BCM_IF_ERROR_RETURN(bcm_port_loopback_set(unit, ingress_port, lb_mode));
  BCM_IF_ERROR_RETURN(bcm_port_loopback_set(unit, egress_port1, lb_mode));
  BCM_IF_ERROR_RETURN(bcm_port_loopback_set(unit, egress_port2, lb_mode));

  return BCM_E_NONE;
}

/*
 * Clean up pre-test setup.
 */
int testCleanup(int unit)
{
  int lb_mode = BCM_PORT_LOOPBACK_NONE;
  /* Traffic test is completed - recover ports as none loopback mode. */
  BCM_IF_ERROR_RETURN(bcm_port_loopback_set(unit, ingress_port, lb_mode));
  BCM_IF_ERROR_RETURN(bcm_port_loopback_set(unit, egress_port1, lb_mode));
  BCM_IF_ERROR_RETURN(bcm_port_loopback_set(unit, egress_port2, lb_mode));

  return BCM_E_NONE;
}

/*
 * This function does the following.
 * Transmit the below packet on ingress_port and verify
 */
bcm_error_t testVerify(int unit)
{
  char   str[512];
  uint64 in_pkt, out_pkt;

  l2cache_addr.dest_port = egress_port1;
  l2cache_addr.dest_modid = 0;
  BCM_IF_ERROR_RETURN(bcm_l2_cache_set(unit, 0, &l2cache_addr, &index_used));

  printf("Verify UC pkt from %d forwarding to port %d\n", ingress_port, egress_port1);
  snprintf(str, 512, "sleep 1;tx 1 pbm=%d data=0x00aaaaaaaa05002a1077750008004500003c67620000ff01bd760a3a40010a3a42730000550a000100516162636465666768696a6b6c6d6e6f7071727374757677616263646566676869;sleep 2", ingress_port);
  bshell(unit, str);

  printf("Executing 'show c'\n");
  bshell(unit, "show c");

  printf("\n******Port stats check******\n");
  printf("--------Ingress port (%d) stats--------\n", ingress_port);
  COMPILER_64_ZERO(in_pkt);
  BCM_IF_ERROR_RETURN(bcm_stat_get(unit, ingress_port, snmpDot1dTpPortInFrames, &in_pkt));
  printf("Rx : %d packets\n", COMPILER_64_LO(in_pkt));

  printf("--------Egress port1 (%d) stats--------\n", egress_port1);
  COMPILER_64_ZERO(out_pkt);
  BCM_IF_ERROR_RETURN(bcm_stat_get(unit, egress_port1, snmpDot1dTpPortOutFrames, &out_pkt));
  printf("Tx : %d packets\n", COMPILER_64_LO(out_pkt));
  if (COMPILER_64_LO(out_pkt) != 1) {
    printf("Packet verify failed. Expected out packet 1 but get %d\n", COMPILER_64_LO(out_pkt));
    return -1;
  }
  printf("--------Egress port2 (%d) stats--------\n", egress_port2);
  COMPILER_64_ZERO(out_pkt);
  BCM_IF_ERROR_RETURN(bcm_stat_get(unit, egress_port2, snmpDot1dTpPortOutFrames, &out_pkt));
  printf("Tx : %d packets\n", COMPILER_64_LO(out_pkt));
  if (COMPILER_64_LO(out_pkt) != 0) {
    printf("Packet verify failed. Expected out packet 0 but get %d\n", COMPILER_64_LO(out_pkt));
    return -1;
  }
  BCM_IF_ERROR_RETURN(bcm_stat_clear(unit, ingress_port));
  BCM_IF_ERROR_RETURN(bcm_stat_clear(unit, egress_port1));
  BCM_IF_ERROR_RETURN(bcm_stat_clear(unit, egress_port2));

  BCM_IF_ERROR_RETURN(bcm_l2_cache_delete(unit, index_used));

  l2cache_addr.dest_port = egress_port2;
  l2cache_addr.dest_modid = 0;
  BCM_IF_ERROR_RETURN(bcm_l2_cache_set(unit, 0, &l2cache_addr, &index_used));
  bshell(0, "l2 show");

  printf("Verify UC pkt from %d forwarding to port %d\n", ingress_port, egress_port2);
  snprintf(str, 512, "tx 1 pbm=%d data=0x00aaaaaaaa05002a1077750008004500003c67620000ff01bd760a3a40010a3a42730000550a000100516162636465666768696a6b6c6d6e6f7071727374757677616263646566676869;sleep 2", ingress_port);
  bshell(unit, str);

  printf("Executing 'show c'\n");
  bshell(unit, "show c");

  printf("\n******Port stats check******\n");
  printf("--------Ingress port (%d) stats--------\n", ingress_port);
  COMPILER_64_ZERO(in_pkt);
  BCM_IF_ERROR_RETURN(bcm_stat_get(unit, ingress_port, snmpDot1dTpPortInFrames, &in_pkt));
  printf("Rx : %d packets\n", COMPILER_64_LO(in_pkt));

  printf("--------Egress port1 (%d) stats--------\n", egress_port1);
  COMPILER_64_ZERO(out_pkt);
  BCM_IF_ERROR_RETURN(bcm_stat_get(unit, egress_port1, snmpDot1dTpPortOutFrames, &out_pkt));
  printf("Tx : %d packets\n", COMPILER_64_LO(out_pkt));
  if (COMPILER_64_LO(out_pkt) != 0) {
    printf("Packet verify failed. Expected out packet 0 but get %d\n", COMPILER_64_LO(out_pkt));
    return -1;
  }
  printf("--------Egress port2 (%d) stats--------\n", egress_port2);
  COMPILER_64_ZERO(out_pkt);
  BCM_IF_ERROR_RETURN(bcm_stat_get(unit, egress_port2, snmpDot1dTpPortOutFrames, &out_pkt));
  printf("Tx : %d packets\n", COMPILER_64_LO(out_pkt));
  if (COMPILER_64_LO(out_pkt) != 1) {
    printf("Packet verify failed. Expected out packet 1 but get %d\n", COMPILER_64_LO(out_pkt));
    return -1;
  }
  BCM_IF_ERROR_RETURN(bcm_stat_clear(unit, ingress_port));
  BCM_IF_ERROR_RETURN(bcm_stat_clear(unit, egress_port1));
  BCM_IF_ERROR_RETURN(bcm_stat_clear(unit, egress_port2));
  bshell(0, "l2 show");
  BCM_IF_ERROR_RETURN(bcm_l2_cache_delete(unit, index_used));

  return BCM_E_NONE;
}

/*
 * This functions does the following
 * a)test setup *
 * b)demonstrates the functionality(done in testVerify()).
 */
bcm_error_t testExecute()
{
  bcm_error_t rv;
  int unit =0;

  bshell(unit, "config show; a ; version");

  rv = testSetup(unit);
  if (BCM_FAILURE(rv)) {
    printf("testSetup() failed, %s.\n", bcm_errmsg(rv));
    return -1;
  }
  printf("Completed test setup successfully.\n");

  rv = testVerify(unit);
  if (BCM_FAILURE(rv)) {
    printf("testVerify() failed\n");
    return -1;
  }
  rv = testCleanup(unit);
  if (BCM_FAILURE(rv)) {
    printf("testCleanup() failed\n");
    return -1;
  }
  printf("Completed test verify successfully.\n");

  return BCM_E_NONE;
}

const char *auto_execute = (ARGC == 1) ? ARGV[0] : "YES";
if (!sal_strcmp(auto_execute, "YES")) {
  print testExecute();
}
