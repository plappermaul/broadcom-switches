/*
 * 
 * 
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 * 
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated.
 * Edits to this file will be lost when it is regenerated.
 */

#ifndef __BCMX_LPORT_H__
#define __BCMX_LPORT_H__

#include <bcm/types.h>
#include <bcm/error.h>
#include <bcmx/types.h>
#include <bcmx/lplist.h>
#include <bcmx_int/lport.h>

/* BCMX Application Logical Port (User Port) definitions. */
#define BCMX_UPORT_CAST(val)    INT_TO_PTR(val) 
#define BCMX_UPORT_EQ(a, b)     ((a) == (b)) 
#define BCMX_UPORT_SET(dst, src)  ((dst) = (src)) 

/* 
 * Special BCMX logical port definitions.
 * 
 *   INVALID:        Explicitly invalid logical port.
 *   NO_SUCH_LPORT:  Alias for INVALID.
 *   LOCAL_CPU:      Special marker requiring a unit context;
 *                   Given a unit, this is the CPU port associated
 *                   with that unit.
 *   ETHER_ALL:      Indicates all ethernet ports (FE, GE, XE, CE),
 *                   as defined in the soc driver file.
 */
#define BCMX_LPORT_INVALID      ((bcmx_lport_t)(BCM_GPORT_INVALID)) 
#define BCMX_NO_SUCH_LPORT      BCMX_LPORT_INVALID /* Compatibility */
#define BCMX_LPORT_LOCAL_CPU    ((bcmx_lport_t)(BCM_GPORT_LOCAL_CPU)) 
#define BCMX_LPORT_ETHER_ALL    \
    ((bcmx_lport_t)(BCM_GPORT_SPECIAL(BCMX_GPORT_SPECIAL_ETHER_ALL))) 

/* BCMX Port Flags. */
#define BCMX_PORT_F_VALID           0x1        /* This port is active. */
#define BCMX_PORT_F_STACK_INT       0x2        /* Internal stack port. */
#define BCMX_PORT_F_STACK_EXT       0x4        /* External stack port. */
#define BCMX_PORT_F_CPU             0x8        /* CPU port. */
#define BCMX_PORT_F_HG              0x10       /* HiGig port. */
#define BCMX_PORT_F_FE              0x20       /* 10/100 port. */
#define BCMX_PORT_F_GE              0x40       /* 1000 port. */
#define BCMX_PORT_F_XE              0x80       /* 10000 port. */
#define BCMX_PORT_F_CE              0x100      /* 100000 port. */
#define BCMX_PORT_F_IS_STACK_PORT(_flags)  \
    ((_flags) & (BCMX_PORT_F_STACK_INT | BCMX_PORT_F_STACK_EXT)) 

/* Mapping Macros lports -> unit, port, modid, modport, flags. */
#define BCMX_LPORT_BOUNDS(_lport)  (BCM_GPORT_IS_SET(_lport)) 
#define BCMX_LPORT_VALID(_lport)  (bcmx_lport_valid(_lport)) 
#define BCMX_LPORT_BCM_UNIT(_lport)  (bcmx_lport_bcm_unit(_lport)) 
#define BCMX_LPORT_BCM_PORT(_lport)  (bcmx_lport_bcm_port(_lport)) 
#define BCMX_LPORT_MODID(_lport)  (bcmx_lport_modid(_lport)) 
#define BCMX_LPORT_MODPORT(_lport)  (bcmx_lport_modport(_lport)) 
#define BCMX_LPORT_FLAGS(_lport)  (bcmx_lport_flags(_lport)) 

/* 
 * LPORT Iterator.
 * 
 * For running thru all lports.
 * 
 * NOTE: Order of ports is based on the order in which ports were added
 * in BCMX.
 */
#define BCMX_LPORT_FIRST                (bcmx_lport_first()) 
#define BCMX_LPORT_NEXT(_lport)         (bcmx_lport_next(_lport)) 
#define BCMX_FOREACH_LPORT(_lport)      \
    for (_lport = BCMX_LPORT_FIRST; _lport != BCMX_LPORT_INVALID;  \
        _lport = BCMX_LPORT_NEXT(_lport)) 
#define BCMX_FOREACH_QUALIFIED_LPORT(_lport, _flag)  \
    BCMX_FOREACH_LPORT(_lport) if (BCMX_LPORT_FLAGS(_lport) & (_flag)) 

/* Mapping Macros uports <--> lports. */
#define BCMX_UPORT_TO_LPORT(_uport)  bcmx_uport_to_lport(_uport) 
#define BCMX_LPORT_TO_UPORT(_lport)  bcmx_uport_get(_lport) 
#define BCMX_UPORT_GET(_lport)      BCMX_LPORT_TO_UPORT(_lport) 
#define BCMX_UPORT_INVALID          ((bcmx_uport_t) (-1)) 
#define BCMX_UPORT_INVALID_DEFAULT  BCMX_UPORT_INVALID /* Deprecated.  Use
                                                  BCMX_UPORT_INVALID_DEFAULT. */

/* 
 * bcmx_uport_create:
 * 
 * This is the callback made by BCMX when a logical port is added to the
 * configuration. BCMX uses this call to determine the application
 * logical port name for the port. The application may use this as an
 * indication that a port has been added to the system, or it may have
 * already been notified by the stack task.
 * 
 * The BCM Dispatch layer will already be set up at this point, so the
 * application can query it for additional information about the port or
 * system configuration if needed. However, the application should not
 * make any BCMX calls inside this callback.
 * 
 * The logical port reference is also provided in case the application
 * wants to record that.
 * 
 * The application is responsible for setting the flags variable properly
 * to indicate whether this is a stack port, etc.
 */
typedef bcmx_uport_t (*bcmx_uport_create_f)(
    bcmx_lport_t lport, 
    int unit, 
    bcm_port_t port, 
    uint32 flags);

typedef void (*bcmx_lport_remove_notify_f)(
    bcmx_lport_t lport, 
    bcmx_uport_t uport);

extern bcmx_uport_create_f bcmx_uport_create;

extern bcmx_lport_remove_notify_f bcmx_lport_remove_notify;

/* Initialize the BCMX Logical Port information. */
extern int bcmx_lport_init(void);

/* BCMX local CPU port reference. */
extern int bcmx_lport_local_cpu_get(
    int bcm_unit, 
    bcmx_lport_t *lport);

/* Get the uport associated with a BCMX lport. */
extern bcmx_uport_t bcmx_uport_get(
    bcmx_lport_t lport);

/* Set the uport associated with a BCMX lport. */
extern int bcmx_uport_set(
    bcmx_lport_t lport, 
    bcmx_uport_t uport);

/* Map a BCMX lport to a BCM dispatch unit and physical port. */
extern int bcmx_lport_to_unit_port(
    bcmx_lport_t lport, 
    int *unit, 
    bcm_port_t *port);

/* Map a BCMX lport to a BCM device module ID and port. */
extern int bcmx_lport_to_modid_port(
    bcmx_lport_t lport, 
    int *modid, 
    bcm_port_t *modport);

/* Map a BCMX lport to its BCM device module id, if appropriate. */
extern int bcmx_lport_to_modid(
    bcmx_lport_t lport, 
    int *modid);

/* Map a BCM unit and port number to a BCMX lport. */
extern bcmx_lport_t bcmx_unit_port_to_lport(
    int unit, 
    bcm_port_t port);

/* Map a BCM device module ID and port to a BCMX lport. */
extern bcmx_lport_t bcmx_modid_port_to_lport(
    int modid, 
    bcm_port_t port);

/* Map a user port to a BCMX lport. */
extern bcmx_lport_t bcmx_uport_to_lport(
    bcmx_uport_t uport);

/* Get the device unit number associated with given module id. */
extern int bcmx_modid_to_bcm_unit(
    int modid, 
    int *bcm_unit);

/* Map a user port to BCM unit and port pair. */
extern int bcmx_uport_to_unit_port(
    bcmx_uport_t uport, 
    int *bcm_unit, 
    bcm_port_t *bcm_port);

/* Deprecated.  Use bcmx_lport_to_unit_port. */
extern int bcmx_lport_bcm_unit(
    bcmx_lport_t lport);

/* Deprecated.  Use bcmx_lport_to_unit_port. */
extern bcm_port_t bcmx_lport_bcm_port(
    bcmx_lport_t lport);

/* Deprecated.  Use bcmx_lport_to_modid_port. */
extern int bcmx_lport_modid(
    bcmx_lport_t lport);

/* Deprecated.  Use bcmx_lport_to_modid_port. */
extern bcm_port_t bcmx_lport_modport(
    bcmx_lport_t lport);

/* Deprecated.  Use BCMX_LPORT_FLAGS */
extern uint32 bcmx_lport_flags(
    bcmx_lport_t lport);

/* Deprecated.  BCMX_LPORT_FIRST. */
extern bcmx_lport_t bcmx_lport_first(void);

/* Deprecated.  Use BCMX_LPORT_NEXT. */
extern bcmx_lport_t bcmx_lport_next(
    bcmx_lport_t lport);

/* Deprecated.  Use BCMX_LPORT_VALID. */
extern int bcmx_lport_valid(
    bcmx_lport_t lport);

/* 
 * Deprecated
 * Applications should no longer use the following definitions.
 */
#define BCMX_LPORT(_lport)      (NULL)     
#define BCMX_LPORT_MAX_DEFAULT  0          /* Was 1024 */
#define bcmx_lport_map_init(_lport_max)  bcmx_lport_init() 
#define _bcmx_lport_next        bcmx_lport_next 
#define _bcmx_uport_to_lport    bcmx_uport_to_lport 

#endif /* __BCMX_LPORT_H__ */