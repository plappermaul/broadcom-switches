/*
 * $Id: timesync.h $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * Timesync Internal functions and definitions
 */

#ifndef __BCM_INT_TIMESYNC_H__
#define __BCM_INT_TIMESYNC_H__

#include <bcm/port.h>

extern int _bcm_esw_port_timesync_profile_init(int unit);

extern int _bcm_esw_port_timesync_profile_delete(int unit);

extern int
_bcm_esw_port_timesync_config_set(int unit, bcm_port_t port, int config_count,
                             bcm_port_timesync_config_t *config_array);

extern int
_bcm_esw_port_timesync_config_get(int unit, bcm_port_t port, int array_size,
                             bcm_port_timesync_config_t *config_array, 
                             int *array_count);
extern int
_bcm_esw_port_timesync_timestamping_mode_set(int unit, bcm_port_t port,
                    bcm_switch_timesync_timestamping_mode_t timestamping_mode);

extern int
_bcm_esw_port_timesync_timestamping_mode_get(int unit, bcm_port_t port,
                    bcm_switch_timesync_timestamping_mode_t *timestamping_mode);

#endif /* !__BCM_INT_TIMESYNC_H__ */
