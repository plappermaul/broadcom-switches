/*
 * 
 * 
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 *
 */

#ifndef __BCM_GDPLL_H__
#define __BCM_GDPLL_H__

#if defined(INCLUDE_GDPLL)

#include <bcm/types.h>

/* GDPLL Macros */
#define BCM_GDPLL_NUM_CHANNELS      128        /* Supported number of GDPLL
                                                  channels */
#define BCM_GDPLL_NUM_PORTS         64         /* GDPLL supports 64 ports */
#define BCM_GDPLL_NUM_CPRI_PORTS    32         /* GDPLL supports 32 CPRI ports */
#define BCM_GDPLL_NUM_STATES        3          /* GDPLL number of states */

/* GDPLL Flags for channel create */
#define BCM_GDPLL_CONF_DPLL         0x1        /* Configure DPLL */
#define BCM_GDPLL_EVENT_CONFIG_REF  0x2        /* Configure channel reference */
#define BCM_GDPLL_EVENT_CONFIG_FB   0x4        /* Configure channel feedback */
#define BCM_GDPLL_CHAN_ALLOC        0x8        /* Channel alloc */
#define BCM_GDPLL_CHAN_SET_PRIORITY 0x10       /* Set channel priority */
#define BCM_GDPLL_CHAN_OUTPUT_CONF  0x20       /* Configure channel output */

/* GDPLL Flags to retrieve state in gdpll_chan_state_t */
#define BCM_GDPLL_CHAN_STATE_LOCK   0x1        /* Channel Lock flag */
#define BCM_GDPLL_CHAN_STATE_ENABLE 0x2        /* Channel enable flag */
#define BCM_GDPLL_CHAN_STATE_ERROR  0x4        /* Channel error flag */

/* GDPLL Input Events */
typedef enum bcm_gdpll_input_event_e {
    bcmGdpllInputEventCpu = 0,          /* CPU generated input event type */
    bcmGdpllInputEventBS0HB = 1,        /* BroadSync0 heartbeat input event type */
    bcmGdpllInputEventBS1HB = 2,        /* BroadSync1 heartbeat input event type */
    bcmGdpllInputEventGPIO0 = 3,        /* GPIO0 input event type */
    bcmGdpllInputEventGPIO1 = 4,        /* GPIO1 input event type */
    bcmGdpllInputEventGPIO2 = 5,        /* GPIO2 input event type */
    bcmGdpllInputEventGPIO3 = 6,        /* GPIO3 input event type */
    bcmGdpllInputEventGPIO4 = 7,        /* GPIO4 input event type */
    bcmGdpllInputEventGPIO5 = 8,        /* GPIO5 input event type */
    bcmGdpllInputEventIPDM0 = 9,        /* IPDM0 input event type */
    bcmGdpllInputEventIPDM1 = 10,       /* IPDM1 input event type */
    bcmGdpllInputEventTS1TS = 11,       /* TS1 counter timestamp event type */
    bcmGdpllInputEventBS0PLL = 12,      /* BroadSync0 PLL input event type */
    bcmGdpllInputEventBS1PLL = 13,      /* BroadSync1 PLL input event type */
    bcmGdpllInputEventRSVD1IF = 14,     /* RSVD1 interface input event type */
    bcmGdpllInputEventRSVD1RF = 15,     /* BCN counter input event type */
    bcmGdpllInputEventBS0ConvTC = 16,   /* BroadSync0 converted timecode input
                                           type */
    bcmGdpllInputEventBS1ConvTC = 17,   /* BroadSync1 converted timecode input
                                           type */
    bcmGdpllInputEventLCPLL0 = 18,      /* Serdes LCPLL0 input event type */
    bcmGdpllInputEventLCPLL1 = 19,      /* Serdes LCPLL1 input event type */
    bcmGdpllInputEventLCPLL2 = 20,      /* Serdes LCPLL2 input event type */
    bcmGdpllInputEventLCPLL3 = 21,      /* Serdes LCPLL3 input event type */
    bcmGdpllInputEventPORT = 22,        /* Input event from port type */
    bcmGdpllInputEventR5 = 23,          /* Input event from R5 core */
    bcmGdpllInputEventMax = 24          /* Input event max value */
} bcm_gdpll_input_event_t;

/* GDPLL chan error codes */
typedef enum bcm_gdpll_chan_error_e {
    bcmGdpllChanErrorNone = 0,      /* No error */
    bcmGdpllChanErrorOverload = 1,  /* M7 Overload */
    bcmGdpllChanErrorTsPpm = 2,     /* TS PPM Violation */
    bcmGdpllChanErrorTsTout = 3,    /* Late Timestamp. WD timer expired */
    bcmGdpllChanErrorDpllLError = 4 /* DPLL tracking large error */
} bcm_gdpll_chan_error_t;

/* GDPLL port events */
typedef enum bcm_gdpll_port_event_e {
    bcmGdpllPortEventRXSOF = 0, /* CPRI port recieve SOF input event */
    bcmGdpllPortEventTXSOF = 1, /* CPRI port transmit SOF input event */
    bcmGdpllPortEventROE = 2,   /* CPRI ROE timestamp event */
    bcmGdpllPortEventRXCDR = 3, /* Port recieve CDR event */
    bcmGdpllPortEventTXPI = 4   /* Port TXPI event */
} bcm_gdpll_port_event_t;

/* GDPLL event destination */
typedef enum bcm_gdpll_event_dest_e {
    bcmGdpllEventDestCPU = 0,   /* Event forward to CPU FIFO */
    bcmGdpllEventDestM7 = 1,    /* Event forward to GDPLL IA */
    bcmGdpllEventDestALL = 2    /* Event forward to both */
} bcm_gdpll_event_dest_t;

/* GDPLL channel output NCO control types */
typedef enum bcm_gdpll_output_event_e {
    bcmGdpllOutputEventBSPLL0 = 0,  /* NCO out for BroadSync0 PLL */
    bcmGdpllOutputEventBSPLL1 = 1,  /* NCO out for BroadSync1 PLL */
    bcmGdpllOutputEventLCPLL0 = 2,  /* NCO out for serdes LCPLL0 */
    bcmGdpllOutputEventLCPLL1 = 3,  /* NCO out for serdes LCPLL1 */
    bcmGdpllOutputEventLCPLL2 = 4,  /* NCO out for serdes LCPLL2 */
    bcmGdpllOutputEventLCPLL3 = 5,  /* NCO out for serdes LCPLL3 */
    bcmGdpllOutputEventTS0 = 6,     /* NCO out for TS0 counter */
    bcmGdpllOutputEventTS1 = 7,     /* NCO out for TS1 counter */
    bcmGdpllOutputEventTXPI = 8,    /* NCO out for TXPI on a given port */
    bcmGdpllOutputEventBCN = 9,     /* NCO out for BCN counter */
    bcmGdpllOutputEventNTPTOD = 10, /* NCO out for NTP TOD */
    bcmGdpllOutputEventPTPTOD = 11  /* NCO out for PTP TOD */
} bcm_gdpll_output_event_t;

/* GDPLL Channel prioritization */
typedef enum bcm_gdpll_chan_priority_e {
    bcmGdpllChanPriority0 = 0,  /* Highest priority */
    bcmGdpllChanPriority1 = 1, 
    bcmGdpllChanPriority2 = 2, 
    bcmGdpllChanPriority3 = 3   /* Lowest priority */
} bcm_gdpll_chan_priority_t;

/* GDPLL Debug modes */
typedef enum bcm_gdpll_debug_mode_e {
    bcmGdpllDebugMode1 = 0, /* To enable debug Mode-1 */
    bcmGdpllDebugMode2 = 1, /* To enable debug Mode-2 */
    bcmGdpllDebugCount = 2  /* Channel debug mode count */
} bcm_gdpll_debug_mode_t;

/* TS Pair forwarding */
typedef enum bcm_gdpll_debug_dest_e {
    bcmGdpllDebugDestCPU = 0,   /* TS-Pair forward to CPU-FIFO */
    bcmGdpllDebugDestM7 = 1,    /* TS-Pair forward to M7 */
    bcmGdpllDebugDestALL = 2    /* TS-Pair forward to both M7 and CPU */
} bcm_gdpll_debug_dest_t;

/* GDPLL CB Type definitions */
typedef enum bcm_gdpll_cb_reg_e {
    bcmGdpllCbRsvd1 = 0,        /* CB for RSVD1 */
    bcmGdpllCbChanState = 1,    /* CB for gdpll chan state change */
    bcmGdpllCbMax = 2           /* CB max value */
} bcm_gdpll_cb_reg_t;

/* GDPLL Channel input structure. */
typedef struct bcm_gdpll_chan_input_s {
    bcm_gdpll_input_event_t input_event; /* Input event type for the channel */
    bcm_port_t port;                    /* Event input from the port */
    bcm_gdpll_port_event_t port_event_type; /* Event input type from the port */
    uint32 event_divisor;               /* Divisor value for an input event */
    bcm_gdpll_event_dest_t event_dest;  /* Input event forwarding destination
                                           type */
    int ts_counter;                     /* 0: TS0, 1:TS1 */
    int ppm_check_enable;               /* PPM check enable flag, boolean */
    uint16 ppm_value;                   /* PPM value */
    uint16 event_frequency;             /* Event frequency for PPM check
                                           configuration */
} bcm_gdpll_chan_input_t;

/* DPLL Config structure. */
typedef struct bcm_gdpll_chan_dpll_config_s {
    int32 k1[BCM_GDPLL_NUM_STATES];     /* DPLL config parameter */
    uint8 k1Shift[BCM_GDPLL_NUM_STATES]; /* left or right sight, 48 places max */
    int32 k1k2[BCM_GDPLL_NUM_STATES]; 
    uint8 k1k2Shift[BCM_GDPLL_NUM_STATES]; 
    uint32 lockDetThres[BCM_GDPLL_NUM_STATES]; 
    uint32 lockIndicatorThresholdLo;    /* Lock Indicator threshold */
    uint32 lockIndicatorThresholdLHi;   /* Lock Indicator threshold */
    uint16 dwell_count[BCM_GDPLL_NUM_STATES]; 
    uint64 phase_error_limiter_thres[BCM_GDPLL_NUM_STATES]; 
    uint8 nco_bits; 
    uint8 phase_counter_ref;            /* Boolean */
    uint8 dpll_num_states; 
    uint16 idump_mod[BCM_GDPLL_NUM_STATES]; 
    uint8 phase_error_shift; 
    uint64 nominal_loop_filter; 
    uint8 invert_phase_error; 
    uint8 norm_phase_error; 
    uint32 phase_error_thres0; 
    uint32 phase_error_thres1; 
    uint16 holdover_filter_coeff; 
} bcm_gdpll_chan_dpll_config_t;

/* Per DPLL state variables stored in state memory. */
typedef struct bcm_gdpll_chan_dpll_state_s {
    uint8 dpll_state;           /* Software DPLL state */
    int64 loop_filter;          /* filter parameter */
    uint32 dwell_counter;       /* filter dwell counter */
    uint32 lockDetFilter;       /* Filter parameter */
    int64 offset; 
    uint8 init_flag;            /* Initialize to TRUE */
    uint8 init_offset_flag;     /* Initialize to FALSE */
    uint64 phase_counter;       /* Phase counter value, unsigned 52 */
    uint64 phaseCounterDelta; 
    uint16 phaseCounterN; 
    uint16 phaseCounterM; 
    uint16 phaseCounterFrac; 
    uint16 idumpCounter; 
    int64 accumPhaseError; 
    uint8 lockIndicator; 
    uint8 lossOfLockIndicator; 
    uint8 largeErrorIndicator; 
} bcm_gdpll_chan_dpll_state_t;

/* GDPLL Channel state */
typedef struct bcm_gdpll_chan_state_s {
    int chan;                           /* Channel handle */
    uint32 state;                       /* Channel state info */
    bcm_gdpll_chan_error_t error_code;  /* Error code on the given channel */
} bcm_gdpll_chan_state_t;

/* GDPLL Channel configuration */
typedef struct bcm_gdpll_chan_s {
    bcm_gdpll_chan_input_t event_ref;   /* Channel input reference */
    bcm_gdpll_chan_input_t event_fb;    /* Channel input feedback */
    bcm_gdpll_chan_priority_t chan_prio; /* Channel priority configurations */
    bcm_gdpll_chan_dpll_config_t chan_dpll_config; /* Channel DPLL configuration */
    bcm_gdpll_chan_dpll_state_t chan_dpll_state; /* Channel DPLL state */
    bcm_gdpll_output_event_t out_event; /* Channel output configuration for NCO
                                           control */
    bcm_port_t port;                    /* Output port for TXPI controls */
    bcm_gdpll_debug_dest_t ts_pair_dest; /* Debug Configurations to forward
                                           timestamp pairs */
} bcm_gdpll_chan_t;

#ifndef BCM_HIDE_DISPATCHABLE

/* Polling API for GDPLL input event */
extern int bcm_gdpll_input_event_poll(
    int unit, 
    bcm_gdpll_input_event_t input_event, 
    uint32 timeout);

/* Create the GDPLL channel. */
extern int bcm_gdpll_chan_create(
    int unit, 
    uint32 flags, 
    bcm_gdpll_chan_t *gdpll_chan, 
    int *pChan);

/* Destroy the GDPLL channel. */
extern int bcm_gdpll_chan_destroy(
    int unit, 
    int chan);

/* Set the GDPLL channel offset */
extern int bcm_gdpll_offset_set(
    int unit, 
    int chan, 
    uint64 offset);

/* Get the GDPLL channel offset */
extern int bcm_gdpll_offset_get(
    int unit, 
    int chan, 
    uint64 *dpllOffset);

/* Enable the GDPLL channel */
extern int bcm_gdpll_chan_enable(
    int unit, 
    int chan, 
    int enable);

/* Get the state of the GDPLL channel */
extern int bcm_gdpll_chan_state_get(
    int unit, 
    int chan, 
    uint32 *dpllState);

/* Enable debug on a given GDPLL channel */
extern int bcm_gdpll_chan_debug_enable(
    int unit, 
    int chan, 
    bcm_gdpll_debug_mode_t debug_mode, 
    int enable);

/* Enable global GDPLL debug feature */
extern int bcm_gdpll_debug(
    int unit, 
    int enable);

/* Flush the GDPLL pipeline */
extern int bcm_gdpll_flush(
    int unit);

#endif /* BCM_HIDE_DISPATCHABLE */

/* GDPLL debug buffer recieve callback */
typedef int (*bcm_gdpll_debug_cb)(
    int unit, 
    void *user_data, 
    uint32 debug_buff, 
    uint32 debug_buf_size);

#ifndef BCM_HIDE_DISPATCHABLE

/* Register callback function for debug buffer */
extern int bcm_gdpll_debug_cb_register(
    int unit, 
    bcm_gdpll_debug_cb cb, 
    void *user_data);

#endif /* BCM_HIDE_DISPATCHABLE */

/* GDPLL generic callback */
typedef int (*bcm_gdpll_cb)(
    int unit, 
    void *user_data, 
    uint32 *cb_data, 
    int cb_type);

#ifndef BCM_HIDE_DISPATCHABLE

/* Register callback function for a given type */
extern int bcm_gdpll_cb_register(
    int unit, 
    bcm_gdpll_cb_reg_t cb_type, 
    bcm_gdpll_cb cb, 
    void *user_data);

/* Un-register callback function for a given type */
extern int bcm_gdpll_cb_unregister(
    int unit, 
    bcm_gdpll_cb_reg_t cb_type);

/* Get the GDPLL channel. */
extern int bcm_gdpll_chan_get(
    int unit, 
    int chan, 
    bcm_gdpll_chan_t *gdpll_chan);

#endif /* defined(INCLUDE_GDPLL) */

#endif /* BCM_HIDE_DISPATCHABLE */

#endif /* __BCM_GDPLL_H__ */
