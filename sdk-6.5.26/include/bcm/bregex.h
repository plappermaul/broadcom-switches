/*
 * 
 * 
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2022 Broadcom Inc. All rights reserved.
 *
 */

#ifndef __BCM_BREGEX_H__
#define __BCM_BREGEX_H__

#if defined(INCLUDE_REGEX)

#include <bcm/types.h>
#include <bcm/field.h>

#ifndef BCM_HIDE_DISPATCHABLE

/* Initialize BCM regex module. */
extern int bcm_regex_init(
    int unit);

#endif /* BCM_HIDE_DISPATCHABLE */

/* Regex Configuration structure. */
typedef struct bcm_regex_config_s {
    uint32 flags;                   /* Flags for Regex configuration. */
    int max_flows;                  /* maximum number of flows to be tracked. */
    int payload_depth;              /* Maximum payload length to be inspected. */
    int inspect_num_pkt;            /* maximum number of packets to be inspected
                                       per flow. */
    int inactivity_timeout_usec;    /* Inactivity timeout. */
    bcm_mac_t dst_mac;              /* Destination MAC address */
    bcm_mac_t src_mac;              /* Source MAC address */
    uint16 ethertype;               /* EtherType */
    uint32 report_flags;            /* Bitmap of report types enabled. */
    uint32 report_buffer_size;      /* The number of entries which the report
                                       buffer can hold. */
} bcm_regex_config_t;

#define BCM_REGEX_CONFIG_ENABLE             0x0001     /* Enable Regex feature */
#define BCM_REGEX_CONFIG_TCP_SESSION_DETECT 0x0002     /* Enable TCP session
                                                          termination */
#define BCM_REGEX_CONFIG_IP4                0x0004     /* Enable tracking on
                                                          IPv4 packets */
#define BCM_REGEX_CONFIG_IP6                0x0008     /* Enable tracking on
                                                          IPv6 packets */
#define BCM_REGEX_CONFIG_AGING              0x0010     /* Enable aging on
                                                          tracked flows */
#define BCM_REGEX_COUNTER_ENABLE            0x0020     /* Enable counter and
                                                          statistics */
#define BCM_REGEX_PER_DIRECTION_COUNTER_ENABLE 0x0040     /* Enable counter and
                                                          statistics per
                                                          direction */
#define BCM_REGEX_USE_POLICY_ID             0x0080     /* Use only actions in
                                                          the referenced policy */

#define BCM_REGEX_REPORT_NEW        0x0001     /* Enable report generation for
                                                  new flows */
#define BCM_REGEX_REPORT_MATCHED    0x0002     /* Enable report generation for
                                                  flows that matched pattern */
#define BCM_REGEX_REPORT_END        0x0004     /* Enable report generation when
                                                  flow is retired */
#define BCM_REGEX_REPORT_ALL        0x0008     /* Enable generation of all
                                                  reports */

/*  bcm_regex_config_t_init */
extern void bcm_regex_config_t_init(
    bcm_regex_config_t *config);

#ifndef BCM_HIDE_DISPATCHABLE

/*  Set configuration for Regex module. */
extern int bcm_regex_config_set(
    int unit, 
    bcm_regex_config_t *config);

/*  Get configuration for Regex module. */
extern int bcm_regex_config_get(
    int unit, 
    bcm_regex_config_t *config);

/* Add a range of L4 ports to be excluded from flow tracking. */
extern int bcm_regex_exclude_add(
    int unit, 
    uint8 protocol, 
    uint16 l4_start, 
    uint16 l4_end);

/* Delete L4 ports range excluded from flow tracking. */
extern int bcm_regex_exclude_delete(
    int unit, 
    uint8 protocol, 
    uint16 l4_start, 
    uint16 l4_end);

/* Delete all L4 ports excluded from flow tracking. */
extern int bcm_regex_exclude_delete_all(
    int unit);

/* Delete L4 ports range excluded from flow tracking. */
extern int bcm_regex_exclude_get(
    int unit, 
    int array_size, 
    uint8 *protocol, 
    uint16 *l4low, 
    uint16 *l4high, 
    int *array_count);

#endif /* BCM_HIDE_DISPATCHABLE */

/* bcm_regex_engine_t */
typedef int bcm_regex_engine_t;

/* Regex Engine Configuration structure. */
typedef struct bcm_regex_engine_config_s {
    uint32 flags;   /* Flags for Regex configuration. */
} bcm_regex_engine_config_t;

#define BCM_REGEX_ENGINE_CONFIG_MULTI_PACKET 0x0001     /* Match pattern across
                                                          packet boundary */

/*  bcm_regex_engine_config_t_init */
extern void bcm_regex_engine_config_t_init(
    bcm_regex_engine_config_t *config);

#ifndef BCM_HIDE_DISPATCHABLE

/* Create regex engine. */
extern int bcm_regex_engine_create(
    int unit, 
    bcm_regex_engine_config_t *config, 
    bcm_regex_engine_t *engid);

/* destroy regex engine. */
extern int bcm_regex_engine_destroy(
    int unit, 
    bcm_regex_engine_t engid);

#endif /* BCM_HIDE_DISPATCHABLE */

/*  Prototype for regex engine traverse callback. */
typedef int (*bcm_regex_engine_traverse_cb)(
    int unit, 
    bcm_regex_engine_t engine, 
    bcm_regex_engine_config_t *config, 
    void *user_data);

#ifndef BCM_HIDE_DISPATCHABLE

/* Traverse regex engines. */
extern int bcm_regex_engine_traverse(
    int unit, 
    bcm_regex_engine_traverse_cb cb, 
    void *user_data);

/* Get regex engine configuration. */
extern int bcm_regex_engine_get(
    int unit, 
    bcm_regex_engine_t engid, 
    bcm_regex_engine_config_t *config);

#endif /* BCM_HIDE_DISPATCHABLE */

#define BCM_REGEX_MAX_PATTERN_SIZE  256        

/* Policy identifier */
typedef int bcm_regex_policy_t;

/* Regex Configuration structure. */
typedef struct bcm_regex_match_s {
    uint32 flags;                       /* Flags for Regex match. */
    int match_id;                       /* User specified match object
                                           identifier */
    char pattern[BCM_REGEX_MAX_PATTERN_SIZE]; /* Regex Pattern. */
    int sequence;                       /* first packet (0), any packet
                                           (non-zero). */
    int payload_length_max;             /* Check payload length of the match
                                           packet is less than this. */
    int payload_length_min;             /* Check payload length of the match
                                           packet is greater than this. */
    uint16 l4_src_port_low;             /* L4 port value to qualify source port
                                           range. */
    uint16 l4_src_port_high;            /* L4 port value to qualify source port
                                           range. */
    uint16 l4_dst_port_low;             /* L4 port value to qualify destination
                                           port range. */
    uint16 l4_dst_port_high;            /* L4 port value to qualify destination
                                           port range. */
    uint16 priority;                    /* priority for match. */
    int provides;                       /* Identifier of this match object in
                                           link list */
    int requires;                       /* Identifier of the next match object
                                           in chain */
    int inactivity_timeout_usec;        /* timeout value in microsec */
    uint32 action_flags;                /* action flags for Regex match. */
    int new_int_pri;                    /* Assign new internal priority to
                                           packets */
    int new_dscp;                       /* reassign DSCP value */
    int new_pkt_pri;                    /* reassign new .1x priority */
    int policer_id;                     /* meter using the specified policer id */
    int sig_id;                         /* The signature ID assigned by the SDK */
    bcm_regex_policy_t policy_id;       /* The ID of the policy to be used if
                                           BCM_REGEX_USE_POLICY_ID is
                                           configured. */
} bcm_regex_match_t;

/*  bcm_regex_engine_config_t_init */
extern void bcm_regex_match_t_init(
    bcm_regex_match_t *match);

#ifndef BCM_HIDE_DISPATCHABLE

/* check Regex match objects. */
extern int bcm_regex_match_check(
    int unit, 
    bcm_regex_match_t *matches, 
    int count, 
    int *metric);

/* Add Regex match objects to engine. */
extern int bcm_regex_match_set(
    int unit, 
    bcm_regex_engine_t engid, 
    bcm_regex_match_t *matches, 
    int count);

#endif /* BCM_HIDE_DISPATCHABLE */

#define BCM_REGEX_MATCH_TO_SERVER           0x00000001 
#define BCM_REGEX_MATCH_TO_CLIENT           0x00000002 
#define BCM_REGEX_MATCH_MULTI_FLOW          0x00000004 /* HTTP Persistent Flow */
#define BCM_REGEX_MATCH_CASE_INSENSITIVE    0x00000008 
#define BCM_REGEX_MATCH_L4_SRC_PORT         0x00000010 
#define BCM_REGEX_MATCH_L4_DST_PORT         0x00000020 
#define BCM_REGEX_MATCH_PKT_SEQUENCE        0x00000040 /* Qualify match only if
                                                          packets are received
                                                          without error in
                                                          sequence */
#define BCM_REGEX_MATCH_SIG_ID_RETURN       0x00000080 /* Set if the hardware
                                                          signature ID
                                                          corresponding to the
                                                          Match ID is to be
                                                          returned */

#define BCM_REGEX_MATCH_ACTION_IGNORE       0x00000001 /* Deprecated, use
                                                          separate policy API */
#define BCM_REGEX_MATCH_ACTION_DROP         0x00000002 /* Deprecated, use
                                                          separate policy API */
#define BCM_REGEX_MATCH_ACTION_COPY_TO_CPU  0x00000004 /* Deprecated, use
                                                          separate policy API */
#define BCM_REGEX_MATCH_ACTION_INT_PRI      0x00000008 /* Deprecated, use
                                                          separate policy API */
#define BCM_REGEX_MATCH_ACTION_DSCP         0x00000010 /* Deprecated, use
                                                          separate policy API */
#define BCM_REGEX_MATCH_ACTION_PKT_PRI      0x00000020 /* Deprecated, use
                                                          separate policy API */
#define BCM_REGEX_MATCH_ACTION_NON_POLICY_ACTIONS 0x0000003F /* Used to check for
                                                          deprecated actions */
#define BCM_REGEX_MATCH_ACTION_NOTIFY_HEADER_ONLY 0x00000040 
#define BCM_REGEX_MATCH_ACTION_NOTIFY_COPY_TO_CPU 0x00000080 

/* Flow Tracker's Flow End Reasons. */
typedef enum bcm_regex_flow_end_reason_e {
    bcmRegexFlowEndReasonFirst = 0,     /* Beginning of Flow End Reason */
    bcmRegexFlowEndReasonTcpAgeout = 0, /* Flow session ended because of age out */
    bcmRegexFlowEndReasonTcpFINr = 1,   /* Flow session ended because TCP FIN
                                           received in reverse direction */
    bcmRegexFlowEndReasonTcpFINf = 2,   /* Flow session ended because TCP FIN
                                           received in forward direction */
    bcmRegexFlowEndReasonTcpReset = 3,  /* Flow session ended because TCP RESET
                                           received in either direction */
    bcmRegexFlowEndReasonCount = 3      /* End of list */
} bcm_regex_flow_end_reason_t;

/* Regex report structure. */
typedef struct bcm_regex_report_s {
    int match_id;                       /* Match identifier. */
    uint32 flags;                       /* flags. */
    bcm_ip_t sip;                       /* Source IPV4 address. */
    bcm_ip_t dip;                       /* destination IPV4 address. */
    bcm_ip6_t sip6;                     /* Source IPV6 address. */
    uint8 protocol;                     /* IP protocol */
    bcm_regex_flow_end_reason_t flow_end_reason; /* FT flow end reason */
    bcm_ip6_t dip6;                     /* destination IPV6 address. */
    uint32 dst_port;                    /* L4 dst port. */
    uint32 src_port;                    /* L4 src port */
    uint32 match_flags;                 /* match flags. */
    uint64 packet_count;                /* Packet count. */
    uint64 byte_count;                  /* byte count. */
    uint64 packet_count_to_server;      /* Packet count to server. */
    uint64 byte_count_to_server;        /* byte count to server . */
    uint64 packet_count_to_client;      /* Packet count to client. */
    uint64 byte_count_to_client;        /* byte count to client . */
    uint32 start_timestamp;             /* Session start timestamp (10ms
                                           granularity) */
    uint32 last_timestamp;              /* Session last packet timestamp (10ms
                                           granularity). */
    uint32 refresh_timestamp;           /* Session refresh packet timestamp
                                           (10ms granularity). */
} bcm_regex_report_t;

/*  bcm_regex_engine_config_t_init */
extern void bcm_regex_report_t_init(
    bcm_regex_report_t *match);

/*  Prototype for user provided report callback. */
typedef void (*bcm_regex_report_cb)(
    int unit, 
    bcm_regex_report_t *report, 
    void *user_data);

#ifndef BCM_HIDE_DISPATCHABLE

/* register user provided callback for regex report. */
extern int bcm_regex_report_register(
    int unit, 
    uint32 reports, 
    bcm_regex_report_cb callback, 
    void *user_data);

/* Unregister user provided callback for regex report. */
extern int bcm_regex_report_unregister(
    int unit, 
    uint32 reports, 
    bcm_regex_report_cb callback, 
    void *user_data);

/* Return user-provided Match ID for a created signature ID. */
extern int bcm_regex_get_match_id(
    int unit, 
    int signature_id, 
    int *match_id);

/* Return hardware Signature ID for a user provided Match ID. */
extern int bcm_regex_get_sig_id(
    int unit, 
    int match_id, 
    int *signature_id);

#endif /* BCM_HIDE_DISPATCHABLE */

/* Regex engine info structure. */
typedef struct bcm_regex_engine_info_s {
    int size;       /* Static size of largest memory that a single engine (or
                       SME) can use. */
    int free_size;  /* Available free memory on the specified engine, or on all
                       engines. */
} bcm_regex_engine_info_t;

#define BCM_REGEX_ENGINE_ALL    -1         

#ifndef BCM_HIDE_DISPATCHABLE

/* Return information about the Signature Matching Engine. */
extern int bcm_regex_engine_info_get(
    int unit, 
    int engine_id, 
    bcm_regex_engine_info_t *regex_engine_info);

#endif /* BCM_HIDE_DISPATCHABLE */

/* Regex session info structure. */
typedef struct bcm_regex_info_s {
    int session_size;           /* Static size of entire Flow Tracker session
                                   table. */
    int session_free_size_ipv4; /* Total free memory for IPv4 sessions available
                                   in the FT. */
    int session_free_size_ipv6; /* Total free memory for IPv6 sessions available
                                   in the FT. */
    int policy_size;            /* Static size of entire Flow Tracker policy
                                   table. */
    int policy_free_size;       /* Total free size of Flow Tracker policy table. */
} bcm_regex_info_t;

#ifndef BCM_HIDE_DISPATCHABLE

/* Return various info about the Flow Tracker Session and Policy tables. */
extern int bcm_regex_info_get(
    int unit, 
    bcm_regex_info_t *regex_info);

#endif /* BCM_HIDE_DISPATCHABLE */

/* Flow Tracker global statistics. */
typedef enum bcm_regex_stat_e {
    bcmStatRegexSessionFirst = 0,       /* Beginning of Session part of list */
    bcmStatRegexSessionEntriesInUse = 0, /* 32-bit counter, Current number of
                                           session entries in use */
    bcmStatRegexSessionFlowsCreated = 1, /* 32-bit counter, Cumulative number of
                                           flows created in flow tracker session
                                           table */
    bcmStatRegexSessionMaxFlowsInUse = 2, /* 32-bit counter, Maximum number of
                                           flows in use, high watermark */
    bcmStatRegexSessionFlowsMissedTCP = 3, /* 32-bit counter, Cumulative number of
                                           TCP flows that could not be inserted
                                           because of bucket full */
    bcmStatRegexSessionFlowsMissedUDP = 4, /* 32-bit counter, Cumulative number of
                                           UDP flows that could not be inserted
                                           because of bucket full */
    bcmStatRegexSessionCmdWaitTimeouts = 5, /* 32-bit counter, Number of flows for
                                           which no results packet was received */
    bcmStatRegexSessionUnusedResults = 6, /* 32-bit counter, Number of results
                                           packets that were ignored */
    bcmStatRegexSessionSuppressedActions = 7, /* 32-bit counter, Number of flows
                                           created where actions inhibited by
                                           VFP */
    bcmStatRegexSessionTcpSynData = 8,  /* 32-bit counter, Number of flows
                                           created with TCP SYN data */
    bcmStatRegexSessionL4Invalid = 9,   /* 32-bit counter, Number of packets
                                           seen where L4 offset/length could not
                                           be determined */
    bcmStatRegexSessionL4PortsExcluded = 10, /* 32-bit counter, Number of flows not
                                           created because of L4Port Table
                                           exclusion */
    bcmStatRegexSessionLast = 10,       /* End of Session part of list */
    bcmStatRegexSigMatchFirst = 21,     /* Beginning of SigMatch part of list */
    bcmStatRegexSigMatchPacketsReceived = 21, /* 32-bit counter, Number of packets
                                           received by the Signature Match for
                                           inspection */
    bcmStatRegexSigMatchPacketsSent = 22, /* 32-bit counter, Number of packets
                                           sent by Signature Match for reporting
                                           to the Flow Tracker */
    bcmStatRegexSigMatchPacketsDropped = 23, /* 32-bit counter, Total number of
                                           packets dropped without match
                                           processing */
    bcmStatRegexSigMatchBytesMatched = 24, /* 32-bit counter, Number of L4 payload
                                           bytes processed by the regex engines */
    bcmStatRegexSigMatchMatchedFlows = 25, /* 32-bit counter, Number of flows that
                                           were successfully matched */
    bcmStatRegexSigMatchUnmatchedFlows = 26, /* 32-bit counter, Number of flows that
                                           were not successfully matches */
    bcmStatRegexSigMatchTotalMatch = 27, /* 32-bit counter, Number of matches
                                           successfully detected by the Regex
                                           engines (includes unreported matches) */
    bcmStatRegexSigMatchCrossSigFlags = 28, /* 32-bit counter, Number of updates to
                                           cross-signature flags */
    bcmStatRegexSigMatchFragmentsReceived = 29, /* 32-bit counter, Number of fragmented
                                           packets received */
    bcmStatRegexSigMatchInPacketError = 30, /* 32-bit counter, Number of packets
                                           dropped for incorrect framing signals
                                           (SOP, EOP) or error signal asserted
                                           anywhere in the packet */
    bcmStatRegexSigMatchFlowTrackerError = 31, /* 32-bit counter, Number of packets
                                           dropped because the FT indicated an
                                           error in a queued packet before EOP */
    bcmStatRegexSigMatchPacketLengthError = 32, /* 32-bit counter, Number of packets
                                           dropped because of a length error */
    bcmStatRegexSigMatchL4ChecksumError = 33, /* 32-bit counter, Number of packets
                                           dropped because of an invalid L4
                                           (TCP/UDP) checksum */
    bcmStatRegexSigMatchFlowDonePacketDrop = 34, /* 32-bit counter, Number of flows that
                                           encountered an invalid Flow Table
                                           entry error */
    bcmStatRegexSigMatchFlowTimestampError = 35, /* 32-bit counter, Number of packets
                                           with timestamp errors -- the first
                                           packet of the current flow was lost */
    bcmStatRegexSigMatchFlowPacketNumError = 36, /* 32-bit counter, Number of flows that
                                           encountered an FT packet number
                                           mismatch */
    bcmStatRegexSigMatchECCError = 37,  /* 32-bit counter, Number of
                                           uncorrectable ECC errors detected */
    bcmStatRegexSigMatchLast = 37,      /* End of SigMatch part of list */
    bcmStatRegexLast = 37,              /* End of list */
    bcmStatRegexCount = 27              /* Number of unique enums in the list */
} bcm_regex_stat_t;

#ifndef BCM_HIDE_DISPATCHABLE

/* Retrieve a specified regex Flow Tracker statistic. */
extern int bcm_regex_stat_get(
    int unit, 
    bcm_regex_stat_t type, 
    uint64 *val);

/* Set a specified regex Flow Tracker statistic. */
extern int bcm_regex_stat_set(
    int unit, 
    bcm_regex_stat_t type, 
    uint64 val);

#endif /* BCM_HIDE_DISPATCHABLE */

#define BCM_REGEX_POLICY_WITH_ID    0x0001     /* Create policy with supplied ID */

#ifndef BCM_HIDE_DISPATCHABLE

/* Create a blank regex policy. */
extern int bcm_regex_policy_create(
    int unit, 
    int flags, 
    bcm_regex_policy_t *policy);

/* Destroy a regex policy. This deallocates memory only. */
extern int bcm_regex_policy_destroy(
    int unit, 
    bcm_regex_policy_t policy);

/* Destroy all regex policy entries. */
extern int bcm_regex_policy_destroy_all(
    int unit);

/* Install a regex policy into the hardware tables. */
extern int bcm_regex_policy_install(
    int unit, 
    bcm_regex_policy_t policy);

/* Remove a regex policy from the hardware tables. */
extern int bcm_regex_policy_remove(
    int unit, 
    bcm_regex_policy_t policy);

/* Add an action to a regex policy. */
extern int bcm_regex_policy_action_add(
    int unit, 
    bcm_regex_policy_t policy, 
    bcm_field_action_t action, 
    uint32 param0, 
    uint32 param1);

/* Add an action to a policy. */
extern int bcm_regex_policy_action_mac_add(
    int unit, 
    bcm_regex_policy_t policy, 
    bcm_field_action_t action, 
    bcm_mac_t mac);

/* Add an action to a regex policy. */
extern int bcm_regex_policy_action_ports_add(
    int unit, 
    bcm_regex_policy_t policy, 
    bcm_field_action_t action, 
    bcm_pbmp_t pbmp);

/* Delete an action from a regex policy. */
extern int bcm_regex_policy_action_delete(
    int unit, 
    bcm_regex_policy_t policy, 
    bcm_field_action_t action, 
    uint32 param0, 
    uint32 param1);

/* 
 * Retrieve the parameters for an action previously added to a regex
 * policy.
 */
extern int bcm_regex_policy_action_get(
    int unit, 
    bcm_regex_policy_t policy, 
    bcm_field_action_t action, 
    uint32 *param0, 
    uint32 *param1);

/* 
 * Retrieve the parameters for an action previously added to a regex
 * policy.
 */
extern int bcm_regex_policy_action_mac_get(
    int unit, 
    bcm_regex_policy_t policy, 
    bcm_field_action_t action, 
    bcm_mac_t *mac);

/* 
 * Retrieve the parameters for an action previously added to a regex
 * policy.
 */
extern int bcm_regex_policy_action_ports_get(
    int unit, 
    bcm_regex_policy_t policy, 
    bcm_field_action_t action, 
    bcm_pbmp_t *pbmp);

/* 
 * Remove an action from a regex_policy.  Same as
 * bcm_regex_policy_action_delete for actions without parameters.
 */
extern int bcm_regex_policy_action_remove(
    int unit, 
    bcm_regex_policy_t policy, 
    bcm_field_action_t action);

/* Remove all actions from a regex_policy. */
extern int bcm_regex_policy_action_remove_all(
    int unit, 
    bcm_regex_policy_t policy);

/* Attach a policer to a regex policy. */
extern int bcm_regex_policy_policer_attach(
    int unit, 
    bcm_regex_policy_t policy, 
    int level, 
    bcm_policer_t policer_id);

/* Detach a policer from a regex policy. */
extern int bcm_regex_policy_policer_detach(
    int unit, 
    bcm_regex_policy_t policy, 
    int level);

/* Detach all policers from a regex policy. */
extern int bcm_regex_policy_policer_detach_all(
    int unit, 
    bcm_regex_policy_t policy);

/* Get the policer(s) attached to a regex policy. */
extern int bcm_regex_policy_policer_get(
    int unit, 
    bcm_regex_policy_t policy, 
    int level, 
    bcm_policer_t *policer_id);

#endif /* BCM_HIDE_DISPATCHABLE */

#define BCM_REGEX_STAT_WITH_ID  0x0001     /* Create stat with supplied ID */

#ifndef BCM_HIDE_DISPATCHABLE

/* Create stat collection entity. */
extern int bcm_regex_stat_create(
    int unit, 
    int flags, 
    int nstat, 
    bcm_field_stat_t *stat_arr, 
    int *stat_id);

/* Create stat collection entity. */
extern int bcm_regex_stat_destroy(
    int unit, 
    int stat_id);

/* Attach statistics entity to Regex policy. */
extern int bcm_regex_policy_stat_attach(
    int unit, 
    bcm_regex_policy_t policy, 
    int stat_id);

/* Detach statistics entity from Regex policy. */
extern int bcm_regex_policy_stat_detach(
    int unit, 
    bcm_regex_policy_t policy, 
    int stat_id);

/* Get 64 bit counter value for specific statistic type. */
extern int bcm_regex_policy_stat_get(
    int unit, 
    bcm_regex_policy_t policy, 
    int *stat_id);

#endif /* BCM_HIDE_DISPATCHABLE */

#define BCM_REGEX_SESSION_KEY_IPV6      0x0001     /* Key is IPv6.  If not set,
                                                      key is IPV4. */
#define BCM_REGEX_SESSION_KEY_REPLACE   0x0002     /* If key found in table when
                                                      adding, replace entry. */

/* Regex session key structure. */
typedef struct bcm_regex_session_key_s {
    int flags;          /* REGEX_SESSION_KEY_xxx flags */
    bcm_ip_t sip;       /* Source IPV4 address. */
    bcm_ip_t dip;       /* Destination IPV4 address. */
    bcm_ip6_t sip6;     /* Source IPV6 address. */
    bcm_ip6_t dip6;     /* Destination IPV6 address. */
    uint32 dst_port;    /* L4 destination port */
    uint32 src_port;    /* L4 source port */
    uint8 protocol;     /* IP protocol. */
} bcm_regex_session_key_t;

#define BCM_REGEX_SESSION_STATIC    0x0001     /* Will either convert an
                                                  existing dynamic session to
                                                  static or create a new static
                                                  session. If this flag is not
                                                  set, the session is assumed to
                                                  be dynamic and will only be
                                                  updated if it already exists. */

/* Regex Configuration structure. */
typedef struct bcm_regex_session_s {
    int flags;                  /* REGEX_SESSION_xxx flags */
    bcm_regex_policy_t policy;  /* The policy to apply to this flow. */
    uint32 inactivity_timeout;  /* Units are usec. */
    uint32 timestamp;           /* Output - session entry's timestamp. */
} bcm_regex_session_t;

/* Initialize regex session key structure. */
extern void bcm_regex_session_key_t_init(
    bcm_regex_session_key_t *session_key);

/* Initialize regex session structure. */
extern void bcm_regex_session_t_init(
    bcm_regex_session_t *session);

#ifndef BCM_HIDE_DISPATCHABLE

/* 
 * Adds or updates a session entry. Flags param for future use, values
 * 0x1-0x8 reserved.
 */
extern int bcm_regex_session_add(
    int unit, 
    int flags, 
    bcm_regex_session_key_t *key, 
    bcm_regex_session_t *session);

/* Updates the policy ID in a session entry. */
extern int bcm_regex_session_policy_update(
    int unit, 
    int flags, 
    int flow_index, 
    bcm_regex_policy_t policy);

/* Deletes an entry from the regex session table. */
extern int bcm_regex_session_delete(
    int unit, 
    bcm_regex_session_key_t *key);

/* Returns the session data (if any). Flags param for future use. */
extern int bcm_regex_session_get(
    int unit, 
    int flags, 
    bcm_regex_session_key_t *key, 
    bcm_regex_session_t *session);

#endif /* BCM_HIDE_DISPATCHABLE */

#define BCM_REGEX_SESSION_TRAVERSE_ALL  0x0001     /* If set, all sessions are
                                                      traversed.  Otherwise only
                                                      static sessions are
                                                      traversed. */

/*  Prototype for regex session traverse callback. */
typedef int (*bcm_regex_session_traverse_cb)(
    int unit, 
    bcm_regex_session_key_t *session_key, 
    bcm_regex_session_t *session_data, 
    void *user_data);

#ifndef BCM_HIDE_DISPATCHABLE

/* Traverse regex sessions. */
extern int bcm_regex_session_traverse(
    int unit, 
    int flags, 
    bcm_regex_session_traverse_cb cb, 
    void *user_data);

/* Deletes all the entries from the regex session table. */
extern int bcm_regex_session_delete_all(
    int unit);

#endif /* defined(INCLUDE_REGEX) */

#endif /* BCM_HIDE_DISPATCHABLE */

#endif /* __BCM_BREGEX_H__ */
