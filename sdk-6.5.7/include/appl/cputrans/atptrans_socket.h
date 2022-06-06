/*
 * $Id: atptrans_socket.h,v 1.1 Broadcom SDK $
 *
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * File:      atptrans_socket.h
 * Purpose:   Declaration for ATP transport over Sockets support routines.
 *
 */

#ifndef _ATPTRANS_SOCKET_H_
#define _ATPTRANS_SOCKET_H_

#ifdef INCLUDE_ATPTRANS_SOCKET

#include <appl/cputrans/atp.h>

extern int atptrans_socket_config_set(int priority, int listen_port);
extern int atptrans_socket_local_cpu_key_set(cpudb_key_t const cpu_key);

extern int atptrans_socket_server_start(void);
extern int atptrans_socket_server_stop(void);
extern int atptrans_socket_server_running(void);

extern int atptrans_socket_install(cpudb_key_t dest_cpu_key, 
                                   bcm_ip_t dest_ip, uint32 flags);
extern int atptrans_socket_uninstall(cpudb_key_t dest_cpu_key);

extern void atptrans_socket_show(void);

#endif /* INCLUDE_ATPTRANS_SOCKET */

#endif /* _ATPTRANS_SOCKET_H_ */


