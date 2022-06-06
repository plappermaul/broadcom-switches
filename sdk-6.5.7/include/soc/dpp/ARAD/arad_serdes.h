
/* $Id: arad_serdes.h,v 1.5 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
*/


#ifndef __ARAD_SERDES_INCLUDED__
#define __ARAD_SERDES_INCLUDED__

#define SOC_ARAD_SERDES_POLARITY_NO_CHANGE 0xFFFFFFFF
#define SOC_ARAD_SERDES_LANE_SWAP_NO_CHANGE 0xFFFFFFFF

soc_error_t 
soc_arad_serdes_polarity_set(int unit, soc_port_t phy_port, int tx_polarity, int rx_polarity);

soc_error_t 
soc_arad_serdes_lane_map_set(int unit, int wc_idx, int tx_map, int rx_map);

soc_error_t 
soc_arad_serdes_rx_seq_start(int unit, soc_port_t port);

soc_error_t 
soc_arad_serdes_rx_seq_stop(int unit, soc_port_t port);


#endif


