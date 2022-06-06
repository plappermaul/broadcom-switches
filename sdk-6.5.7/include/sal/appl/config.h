/*
 * $Id: config.h,v 1.11 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * File: 	config.h
 * Purpose: 	SAL Configuration file definitions
 */

#ifndef _SAL_CONFIG_H
#define _SAL_CONFIG_H

#if defined(UNIX) || defined(__ECOS)
# ifndef SAL_CONFIG_FILE
#  define SAL_CONFIG_FILE		"config.bcm"
# endif
# ifndef SAL_CONFIG_TEMP
#  define SAL_CONFIG_TEMP		"config.tmp"
# endif
#else
# ifndef SAL_CONFIG_FILE
#  define SAL_CONFIG_FILE               "config.bcm"
#       ifndef SAL_CONFIG_FILE_FLASH
#        define SAL_CONFIG_FILE_FLASH	"flash:config.bcm"
#       endif
# endif
# ifndef SAL_CONFIG_TEMP
#  define SAL_CONFIG_TEMP		"config.tmp"
#       ifndef SAL_CONFIG_TEMP_FLASH
#        define SAL_CONFIG_TEMP_FLASH	"flash:config.tmp"
#       endif
# endif
#endif

#define SAL_CONFIG_STR_MAX		128	/* Max len of "NAME=VALUE\0" */

/*
 * Defines:	SAL_CONFIG_XXX
 *
 *	SWITCH_MAC 	Mac address used for management interface
 *	SWITCH_IP	IP address used for management interface
 *	ASSIGNED_MAC_BASE Base of MAC address range assigned to switch
 *			(excluding SWITCH_MAC).
 *	ASSIGNED_MAC_COUNT # of MAC addresses assigned to switch
 *			(excluding SWITCH_MAC).
 */

#define	SAL_CONFIG_SWITCH_MAC		"station_mac_address"
#define	SAL_CONFIG_SWITCH_IP		"station_ip_address"
#define	SAL_CONFIG_SWITCH_IP_NETMASK	"station_ip_netmask"
#define	SAL_CONFIG_SWITCH_HOSTNAME	"station_hostname"
#define	SAL_CONFIG_ASSIGNED_MAC_BASE	"switch_mac_base"
#define	SAL_CONFIG_ASSIGNED_MAC_COUNT	"switch_mac_count"

#define SAL_CONFIG_RELOAD_BUFFER_SIZE	"reload_buffer_size"
#define SAL_CONFIG_RELOAD_FILE_NAME	"reload_file_name"

extern int 	sal_config_refresh(void);
extern int	sal_config_file_set(const char *fname, const char *tname);
extern int	sal_config_file_get(const char **fname, const char **tname);
extern int	sal_config_flush(void);
extern int	sal_config_save(char *file, char *pattern, int append);
extern char 	*sal_config_get(const char *name);
extern int	sal_config_get_next(char **name, char **value);
extern int	sal_config_set(char *name, char *value);
extern void	sal_config_show(void);
extern void    sal_config_dump(void);
extern int      sal_config_init(void);

/* Declaration for compiled-in config variables.  These are generated
 * automatically from config.bcm by $SDK/make/bcm2c.pl.
 */
extern void     sal_config_init_defaults(void);

#endif	/* !_SAL_CONFIG_H */
