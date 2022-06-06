/*
 * $Id: $
 * 
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 * 
 */

#ifdef SW_AUTONEG_SUPPORT

 #ifndef __BCM_SW_AN_H__
 #define __BCM_SW_AN_H__

#include <phymod/phymod.h>
#include <bcm/types.h>

 typedef enum bcm_port_sw_an_event_e {
 	BCM_PORT_SW_AN_EVENT_NONE = 0,
 	BCM_PORT_SW_AN_EVENT_AN_START,
 	BCM_PORT_SW_AN_EVENT_AN_IN_PROCESS,
 	BCM_PORT_SW_AN_EVENT_AN_RESTART,
 	BCM_PORT_SW_AN_EVENT_AN_DOWN,
 	BCM_PORT_SW_AN_EVENT_AN_STOP,
 	BCM_PORT_SW_AN_EVENT_AN_LINK_DOWN,
 	BCM_PORT_SW_AN_EVENT_AN_MAX
 } bcm_port_sw_an_event_t;

 typedef enum bcm_sw_an_fsm_event_e {
    BCM_FSM_SW_AN_EVT_NONE = 0,
    BCM_FSM_SW_AN_EVT_WAIT, /* wait for event */
    BCM_FSM_SW_AN_EVT_START, /* SW state to kick off the state machine */
    BCM_FSM_SW_AN_EVT_SEQ_START, /* HW event to restart AN from base page exchange */
    BCM_FSM_SW_AN_EVT_LD_PAGE,  /* HW req to load the new page */
    BCM_FSM_SW_AN_EVT_LP_PAGE_RDY, /* HW event indicating LP page rcvd */
    BCM_FSM_SW_AN_EVT_PAGE_EX_COMPLETE,
    BCM_FSM_SW_AN_EVT_SEND_ACK,
    BCM_FSM_SW_AN_EVT_SEND_RESTART, /* Restart request from external events like link down */
    BCM_FSM_SW_AN_EVT_MAX
} bcm_sw_an_fsm_event_t;

#define SW_AN_MODE_CL73_MSA     0x3
#define SW_AN_MODE_MSA_ONLY     0x4


/* initialize the SW AN module */
extern int bcm_sw_an_module_init(int unit);

/* deInit the SW AN module */
extern int bcm_sw_an_module_deinit(int unit);

/* Start/Stop the SW AN thread for the unit */
extern int bcm_sw_an_enable_set(int unit, int enable); 

extern int bcm_sw_an_enable_get(int unit, int *enable);

/* add the port to SW AN */
extern int bcm_sw_an_port_register(int unit, bcm_port_t port);

/* remove the port from SW AN */
extern int bcm_sw_an_port_unregister(int unit, bcm_port_t port);

/* notify the SW AN thread of any AN event */
extern int bcm_sw_an_post_event(int unit, bcm_port_t port, bcm_port_sw_an_event_t event);

/* sw an advert set */
extern int bcm_sw_an_advert_set(int unit, bcm_port_t port, phymod_autoneg_ability_t *ability);

/* debug function to dump an context for the port */
extern int bcm_sw_an_port_diag(int unit, int port);


#endif /* __BCM_SW_AN_H__ */
#endif /* SW_AUTONEG_SUPPORT */

