/*
 * $Id: diag.h,v 1.15 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * File:        diag.h
 * Purpose:     
 */

#ifndef   _DIAG_H_
#define   _DIAG_H_

#include <soc/defs.h>
#include <bcm/port.h>
#include <bcm/link.h>

#include <sal/types.h>
#include <appl/diag/shell.h>
#include <soc/portmode.h>
#include <soc/register.h>
#include <soc/types.h>

#define SOC_CLI_THREAD_PRI 100
#define SOC_BG_THREAD_PRI (SOC_CLI_THREAD_PRI+10)

extern cmd_t *cur_cmd_list[SOC_MAX_NUM_DEVICES];
extern int cur_cmd_cnt[SOC_MAX_NUM_DEVICES];

/* These commands are visible in all (both) modes */
extern cmd_t bcm_cmd_common[];
extern int bcm_cmd_common_cnt;
extern cmd_t* dyn_cmd_list; 
extern int dyn_cmd_cnt; 

extern int cur_mode[SOC_MAX_NUM_DEVICES];

extern char *discard_mode[];
extern char *linkscan_mode[];
extern char *interface_mode[];
extern char *phymaster_mode[];
extern char *loopback_mode[];
extern char *forward_mode[];
extern char *encap_mode[];
extern char *mdix_mode[];
extern char *mdix_status[];
extern char *medium_status[];




#define _E2S(x, max, string_array) \
    ((((size_t)(x) < (max))) ?  (string_array)[(x)] : "?")

#define DIAG_FUNC_STR_MATCH(_function, _string, _short_string) \
    !(sal_strncasecmp(function, _string, \
                SOC_SAND_MAX(sal_strlen(function), \
                    sal_strlen(_string)))) || \
    !(sal_strncasecmp(function, _short_string, \
            SOC_SAND_MAX(sal_strlen(function),\
                sal_strlen(_short_string))))

#define DISCARD_MODE(x)    _E2S(x, BCM_PORT_DISCARD_COUNT, discard_mode)
#define LINKSCAN_MODE(x)   _E2S(x, BCM_LINKSCAN_MODE_COUNT, linkscan_mode)
#define INTERFACE_MODE(x)  _E2S(x, BCM_PORT_IF_COUNT, interface_mode)
#define PHYMASTER_MODE(x)  _E2S(x, BCM_PORT_MS_COUNT, phymaster_mode)
#define LOOPBACK_MODE(x)   _E2S(x, BCM_PORT_LOOPBACK_COUNT, loopback_mode)
#define FORWARD_MODE(x)	   _E2S(x, BCM_STG_STP_COUNT, forward_mode)
#define ENCAP_MODE(x)      _E2S(x, COUNTOF(encap_mode), encap_mode)
#define MDIX_MODE(x)       _E2S(x, BCM_PORT_MDIX_COUNT, mdix_mode)
#define MDIX_STATUS(x)     _E2S(x, BCM_PORT_MDIX_STATUS_COUNT, mdix_status)
#define MEDIUM_STATUS(x)   _E2S(x, BCM_PORT_MEDIUM_COUNT, medium_status)
#define MCAST_FLOOD(x)     _E2S(x, BCM_PORT_MCAST_FLOOD_COUNT, mcast_flood)

#if defined(BCM_ROBO_SUPPORT)
extern char *robo_discard_mode[];
extern char *robo_linkscan_mode[];
extern char *robo_interface_mode[];
extern char *robo_phymaster_mode[];
extern char *robo_loopback_mode[];
extern char *robo_forward_mode[];
extern char *robo_encap_mode[];
extern char *robo_mdix_mode[];
extern char *robo_mdix_status[];
extern char *robo_medium_status[];
#define ROBO_DISCARD_MODE(x)    _E2S(x, BCM_PORT_DISCARD_COUNT, robo_discard_mode)
#define ROBO_LINKSCAN_MODE(x)   _E2S(x, BCM_LINKSCAN_MODE_COUNT, robo_linkscan_mode)
#define ROBO_INTERFACE_MODE(x)  _E2S(x, BCM_PORT_IF_COUNT, robo_interface_mode)
#define ROBO_PHYMASTER_MODE(x)  _E2S(x, BCM_PORT_MS_COUNT, robo_phymaster_mode)
#define ROBO_LOOPBACK_MODE(x)   _E2S(x, BCM_PORT_LOOPBACK_COUNT, robo_loopback_mode)
#define ROBO_FORWARD_MODE(x)	   _E2S(x, BCM_STG_STP_COUNT, robo_forward_mode)
#define ROBO_ENCAP_MODE(x)      _E2S(x, BCM_PORT_ENCAP_COUNT, robo_encap_mode)
#define ROBO_MDIX_MODE(x)       _E2S(x, BCM_PORT_MDIX_COUNT, robo_mdix_mode)
#define ROBO_MDIX_STATUS(x)     _E2S(x, BCM_PORT_MDIX_STATUS_COUNT, robo_mdix_status)
#define ROBO_MEDIUM_STATUS(x)   _E2S(x, BCM_PORT_MEDIUM_COUNT, robo_medium_status)
#define ROBO_MCAST_FLOOD(x)     _E2S(x, BCM_PORT_MCAST_FLOOD_COUNT,robo_ mcast_flood)
#endif 
extern void command_mode_set(int unit, int mode);
extern int command_mode_get(int unit);

typedef enum sh_cmd_mode_e {
    BCMX_CMD_MODE,
    ESW_CMD_MODE,
    ROBO_CMD_MODE,
    SBX_CMD_MODE,
    DPP_CMD_MODE,
    DNX_CMD_MODE,
    DNXF_CMD_MODE,
    EA_CMD_MODE,
    ILLEGAL_CMD_MODE
} sh_cmd_mode_t;

/*
 * Switch-case enums for Custom handler
 */
#define CUSTOM_HANDLER_STAT_GET        1
#define CUSTOM_HANDLER_STAT_SYNC_GET   2
#define CUSTOM_HANDLER_VLAN_GET        3


#define DSC3B0_CDR_STATUS2_OSCDR_MODE_MASK                         0x01c0
#define DSC3B0_CDR_STATUS2_OSCDR_MODE_SHIFT                        6

/* System related diag functions */
extern void _diag_assert(const char *expr, const char *file, int line);


/* Port related diag functions */
extern void brief_port_info_header(int unit);
extern int brief_port_info(int unit, int port, bcm_port_info_t *info,
                           uint32 flags);
extern void disp_port_info(int unit, char *port_ref, bcm_port_t port, bcm_port_info_t *info, 
                           int st_port, uint32 flags);
extern void port_parse_setup(int unit, parse_table_t *pt,
                             bcm_port_info_t *info);
extern void port_parse_mask_get(parse_table_t *pt, uint32 *seen,
                                uint32 *parsed);
extern int port_parse_port_info_set(uint32 flags,
                 bcm_port_info_t *src, bcm_port_info_t *dest);

extern void port_info_init(int unit, int port, bcm_port_info_t *info,
                           uint32 actions);
extern void format_port_speed_ability(char *buf, int bufsize, soc_port_mode_t ability);
extern void format_port_intf_ability(char *buf, int bufsize, soc_port_mode_t ability);
extern void format_port_medium_ability(char *buf, int bufsize, soc_port_mode_t ability);
extern void format_port_pause_ability(char *buf, int bufsize, soc_port_mode_t ability);
extern void format_port_lb_ability(char *buf, int bufsize, soc_port_mode_t ability);
extern void format_port_flags_ability(char *buf, int bufsize, soc_port_mode_t ability);

extern cmd_result_t _phy_diag_eyescan(int unit, bcm_pbmp_t pbmp,args_t *args);  
extern cmd_result_t _phy_diag_pcs(int unit, bcm_pbmp_t pbmp,args_t *args);
extern int port_to_phyaddr(int unit, int port);
extern int port_to_phyaddr_int(int unit, int port);
extern int port_diag_ctrl(int unit, int port, uint32 inst, int op_type, int op_cmd, void *arg);
extern char *get_core_str(int unit, int core);

#if defined(BCM_TOMAHAWK_SUPPORT)
extern int esw_brief_port_info(int unit, int port, bcm_port_info_t *info,
                               uint32 flags);
#endif

#if defined(BCM_ROBO_SUPPORT)
extern void robo_brief_port_info_header(int unit);

/* Port related diag functions */
extern int robo_brief_port_info(char *port_ref, bcm_port_info_t *info,
                           uint32 flags);
extern void robo_disp_port_info(char *port_ref, bcm_port_info_t *info, int hg_port,
			   uint32 flags);
extern void robo_port_parse_setup(int unit, parse_table_t *pt,
                             bcm_port_info_t *info);
extern void robo_port_parse_mask_get(parse_table_t *pt, uint32 *seen,
                                uint32 *parsed);
extern int robo_port_parse_port_info_set(uint32 flags,
                 bcm_port_info_t *src, bcm_port_info_t *dest);

extern void robo_port_info_init(int unit, int port, bcm_port_info_t *info,
                                uint32 actions);
#endif 

/* Register/mem diagnostic functions */
typedef soc_reg_above_64_val_t reg_val;
typedef uint32 mem_val[SOC_MAX_MEM_WORDS];

extern int diag_reg_get(int unit, char *name, reg_val value);
extern int diag_reg_field_get(int unit, char *name, char* field_name, 
                              reg_val value);

extern int diag_reg_set(int unit, char *name, reg_val value);
extern int diag_reg_field_set(int unit, char *name, char* field_name,
                              reg_val value);

extern int diag_mem_get(int unit, char *name, int index, mem_val value);
extern int diag_mem_field_get(int unit, char *name, char* field_name, 
                              int index, mem_val value);
extern int diag_soc_mem_read_range(int unit, char *name, int index1, 
                                   int index2, void *buffer);

extern int diag_mem_set(int unit, char *name, int start, int count, 
                        mem_val value);
extern int diag_mem_field_set(int unit, char *name, char* field_name, 
                              int start, int count, mem_val value);
extern int diag_soc_mem_write_range(int unit, char *name, int index1, 
                                    int index2, void *buffer);

#endif /* _DIAG_H_ */
