/*
 * $Id: //depot/tomahawk2/dv/tdm/chip/src/tdm_th2_shim.c#4 $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 * $All Rights Reserved.$
 *
 * TDM chip to core API shim layer
 */
#ifdef _TDM_STANDALONE
	#include <tdm_top.h>
#else
	#include <soc/tdm/core/tdm_top.h>
#endif


/**
@name: tdm_chip_th2_shim__check_ethernet
@param:

Upward abstraction layer between TDM.4 and TDM.5 API
**/
tdm_mod_t*
tdm_chip_th2_shim__check_ethernet_d( int port, enum port_speed_e speed[TH2_NUM_EXT_PORTS], enum port_state_e state[TH2_NUM_EXT_PORTS], int **tsc, int traffic[TH2_NUM_PM_MOD] )
{
	int idx1, idx2;
	tdm_mod_t *_tdm_s;
	
	_tdm_s = TDM_ALLOC(sizeof(tdm_mod_t),"TDM shim allocation");
	if (!_tdm_s) {
		return NULL;
	}
	
	_tdm_s->_chip_data.soc_pkg.speed=TDM_ALLOC(TH2_NUM_EXT_PORTS*sizeof(int *), "port speed list");
	for (idx1=0; idx1<TH2_NUM_EXT_PORTS; idx1++) {
		_tdm_s->_chip_data.soc_pkg.speed[idx1] = speed[idx1];
	}
	_tdm_s->_chip_data.soc_pkg.state=TDM_ALLOC(TH2_NUM_EXT_PORTS*sizeof(int *), "port state list");
	for (idx1=0; idx1<TH2_NUM_EXT_PORTS; idx1++) {
		_tdm_s->_chip_data.soc_pkg.state[idx1] = state[idx1];
	}
	_tdm_s->_chip_data.soc_pkg.pmap_num_modules = TH2_NUM_PHY_PM;
	_tdm_s->_chip_data.soc_pkg.pmap_num_lanes = TH2_NUM_PM_LNS;
	_tdm_s->_chip_data.soc_pkg.pmap=(int **) TDM_ALLOC((_tdm_s->_chip_data.soc_pkg.pmap_num_modules)*sizeof(int *), "portmod_map_l1");
	for (idx1=0; idx1<(_tdm_s->_chip_data.soc_pkg.pmap_num_modules); idx1++) {
		_tdm_s->_chip_data.soc_pkg.pmap[idx1]=(int *) TDM_ALLOC((_tdm_s->_chip_data.soc_pkg.pmap_num_lanes)*sizeof(int), "portmod_map_l2");
		TDM_MSET(_tdm_s->_chip_data.soc_pkg.pmap[idx1],(_tdm_s->_chip_data.soc_pkg.num_ext_ports),_tdm_s->_chip_data.soc_pkg.pmap_num_lanes);
	}
	for (idx1=0; idx1<TH2_NUM_PHY_PM; idx1++) {
		for (idx2=0; idx2<TH2_NUM_PM_LNS; idx2++) {
			_tdm_s->_chip_data.soc_pkg.pmap[idx1][idx2] = tsc[idx1][idx2];
		}
	}
	/* _tdm_s->_chip_data.soc_pkg.soc_vars.th2.pm_encap_type=sal_alloc(TH2_NUM_PM_MOD*sizeof(int *), "traffic encap"); */	
	for (idx1=0; idx1<TH2_NUM_PM_MOD; idx1++) {
		_tdm_s->_chip_data.soc_pkg.soc_vars.th2.pm_encap_type[idx1] = traffic[idx1];
	}	
	
	_tdm_s->_core_data.vars_pkg.port = port;
	return _tdm_s;
	
}


/**
@name: tdm_chip_th2_shim__core_vbs_scheduler_ovs
@param:

Passthru function for oversub scheduling request from TD2/TD2+
**/
int
tdm_chip_th2_shim__core_vbs_scheduler_ovs( tdm_mod_t *_tdm )
{

	if (_tdm->_chip_data.soc_pkg.flex_port_en) {
		/* Used for FlexPort */
		/*return tdm_th2_vbs_scheduler_ovs_flex_port(_tdm);*/
		return tdm_th2_vbs_scheduler_ovs_flex_port_delta(_tdm);
	} else {
		/* Used for initial config */
		return tdm_th2_vbs_scheduler_ovs(_tdm);
	}
}
