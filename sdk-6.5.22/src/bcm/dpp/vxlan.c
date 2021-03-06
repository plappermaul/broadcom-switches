/*
 * 
 *
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2021 Broadcom Inc. All rights reserved.
 *
 * DNX VXLAN implementation
 */
#include <soc/defs.h>
 
/* defined(BCM_ARAD_SUPPORT) && */

#if defined(INCLUDE_L3) 

#ifdef _ERR_MSG_MODULE_NAME
  #error "_ERR_MSG_MODULE_NAME redefined"
#endif

#define _ERR_MSG_MODULE_NAME BSL_BCM_VXLAN

#include <shared/bsl.h>

#include <bcm/vxlan.h>


#include <bcm_int/control.h>
#include <bcm_int/dpp/utils.h>
#include <bcm_int/dpp/error.h>

#include <bcm_int/dpp/alloc_mngr.h>
#include <bcm_int/dpp/alloc_mngr_glif.h>
#include <bcm_int/dpp/alloc_mngr_local_lif.h>
#include <bcm_int/dpp/gport_mgmt.h>
#include <bcm_int/dpp/switch.h>
#include <bcm_int/dpp/vswitch.h> 
#include <bcm_int/dpp/tunnel.h> 

#include <bcm/debug.h>
#include <bcm/vxlan.h>
#include <bcm/vlan.h>
#include <bcm_int/dpp/vswitch.h>
#include <bcm_int/common/debug.h>
#include <bcm_int/petra_dispatch.h>

#include <soc/dpp/mbcm_pp.h>

#include <soc/dpp/PPD/ppd_api_lif_table.h>
#include <soc/dpp/PPD/ppd_api_frwrd_fec.h>
#include <soc/dpp/PPD/ppd_api_eg_filter.h>

#include <soc/dpp/ARAD/ARAD_PP/arad_pp_occupation_mgmt.h>
#include <soc/dpp/ARAD/ARAD_PP/arad_pp_sw_db.h>

/***************************************************************/

/*
 * Local defines
 */




#define DPP_VXLAN_INIT_CHECK(unit) \
do { \
    BCM_DPP_UNIT_CHECK(unit); \
    if (!_dpp_vxlan_state[unit]) { \
        BCMDNX_ERR_EXIT_MSG(BCM_E_UNIT,  (_BSL_BCM_MSG("%s: VXLAN unitialized on unit:%d \n"), \
                           FUNCTION_NAME(), unit)); \
    } \
} while (0) 

#define DPP_VXLAN_UNIT_LOCK(unit) \
do { \
    if (sal_mutex_take(_dpp_vxlan_state[unit]->lock, sal_mutex_FOREVER)) { \
        BCMDNX_ERR_EXIT_MSG(BCM_E_INTERNAL, (_BSL_BCM_MSG("%s: sal_mutex_take failed for unit %d. \n"), \
                    FUNCTION_NAME(), unit)); \
    } \
    _lock_taken = 1;  \
} while (0)


#define DPP_VXLAN_UNIT_UNLOCK(unit) \
do { \
    if(1 == _lock_taken) { \
        _lock_taken = 0;  \
        if (sal_mutex_give(_dpp_vxlan_state[unit]->lock)) { \
            BCMDNX_ERR_EXIT_MSG(BCM_E_INTERNAL, (_BSL_BCM_MSG("%s: sal_mutex_give failed for unit %d. \n"), \
                        FUNCTION_NAME(), unit)); \
        } \
    } \
} while (0)


/* map l2vpn to VSI, some tables consider only 12 lsb bits of the VSI */
#define _BCM_VXLAN_VPN_TO_VSI(__unit, __l2vpn)  (__l2vpn & (SOC_DPP_DEFS_GET(__unit, nof_epni_isid)-1))

#define _BCM_VXLAN_VPN_REPLACE_FLAGS (BCM_VXLAN_VPN_REPLACE               | \
                                      BCM_VXLAN_VPN_UNKNOWN_UCAST_REPLACE | \
                                      BCM_VXLAN_VPN_UNKNOWN_MCAST_REPLACE | \
                                      BCM_VXLAN_VPN_BCAST_REPLACE)

/* 
* sw state for VXLAN
*/  

/*
 * if all devices in system synced. 
 */
static    int _Bcm_dpp_sync_all = 1;

/* dpp VXLANstatus */

typedef struct _dpp_vxlan_state_s {
    sal_mutex_t lock;
} _dpp_vxlan_state_t;

static _dpp_vxlan_state_t *_dpp_vxlan_state[BCM_MAX_NUM_UNITS] = {0}; 

/* 
 * map VXLAN-port-id to in-LIF
 */
STATIC
int
  _bcm_petra_vxlan_port_id_to_lif(
      int unit, 
      bcm_gport_t vxlan_port_id)          /* GPORT identifier. */
{
    return BCM_GPORT_VXLAN_PORT_ID_GET(vxlan_port_id);
}

/* 
 * map in-LIF to VXLAN-port-id 
 */
STATIC
int
  _bcm_petra_vxlan_port_id_from_lif(
      int unit, 
      int in_lif,
      bcm_gport_t *vxlan_port_id)          
{
    BCM_GPORT_VXLAN_PORT_ID_SET(*vxlan_port_id,in_lif);
    return BCM_E_NONE;
}

int
  _bcm_petra_vxlan_port_id_to_resources(
      int unit, 
      bcm_gport_t vxlan_port_id,
      int         *global_in_lif,
      int         *local_in_lif, 
      int         *global_out_lif,
      int         *local_out_lif,
      int         *eg_fec,
      int         *match_port, 
      int         *is_local
      )          
{
    _BCM_GPORT_PHY_PORT_INFO phy_port;
    int rv = BCM_E_NONE;

    BCMDNX_INIT_FUNC_DEFS;

    *eg_fec = -1;
    *local_out_lif = -1;
    *global_out_lif = -1;
    /* in_lif */
    *global_in_lif = _bcm_petra_vxlan_port_id_to_lif(unit,vxlan_port_id);

    if (*global_in_lif == -1) {
        BCMDNX_ERR_EXIT_MSG(BCM_E_PARAM, (_BSL_BCM_MSG("Illegal vxlan port id.")));
    }

    rv = _bcm_dpp_global_lif_mapping_global_to_local_get(unit, _BCM_DPP_GLOBAL_LIF_MAPPING_INGRESS, *global_in_lif, local_in_lif);
    BCMDNX_IF_ERR_EXIT(rv);

    /* from has get <FEC,port> or <outlif,Port> */
    rv = _bcm_dpp_sw_db_hash_vlan_find(unit,
                        (sw_state_htb_key_t) &(vxlan_port_id), 
                        (sw_state_htb_data_t) &phy_port,FALSE);        
    if (GPORT_HASH_VLAN_NOT_FOUND(phy_port,rv)) {
        BCMDNX_ERR_EXIT_MSG(BCM_E_NOT_FOUND, (_BSL_BCM_MSG("gport-id not found")));
    }

    /* according to type get <FEC,port> or <outlif,Port> */
    if(phy_port.type == _bcmDppGportResolveTypeVxlanEgFec) {
        *eg_fec = phy_port.phy_gport;  /* FEC */
        *match_port = phy_port.encap_id;  /* as encap-id has no specific use, store match-port instead of read from FEC */
    }
    else if (phy_port.type == _bcmDppGportResolveTypeVxlan){/* else store port + outlif*/
        *match_port = phy_port.phy_gport;  /* physical port*/
        *global_out_lif = phy_port.encap_id; /* encap_id = out-AC */

        rv = _bcm_dpp_global_lif_mapping_global_to_local_get(unit, _BCM_DPP_GLOBAL_LIF_MAPPING_EGRESS, *global_out_lif, local_out_lif);
        BCMDNX_IF_ERR_EXIT(rv);
    }
    else{
        BCMDNX_ERR_EXIT_MSG(BCM_E_INTERNAL, (_BSL_BCM_MSG("gport id of vxlan mistmatch internal info")));
    }

    /* check if this local */
    rv = _bcm_dpp_gport_is_local(unit,*match_port,is_local);
    BCMDNX_IF_ERR_EXIT(rv);

exit:
    BCMDNX_FUNC_RETURN;
}


/* 
 * given VXLAN port return used if the call to configure ingress/egress side 
 * according to match port 
 */ 
int
_bcm_petra_vxlan_side_config_get(
    int unit, 
    bcm_vxlan_port_t *vxlan_port,
    int *ingress, 
    int *egress
 ){
    
    BCMDNX_INIT_FUNC_DEFS;

    /* VXLAN port always config ingress + egress as it exist in same device */
    *ingress = 1;
    *egress = 1;
    
    BCMDNX_FUNC_RETURN;
}

/* 
 * given VXLAN port return used resources for this  gport. 
 * consider ingres/egress. 
 */ 
int
_bcm_petra_vxlan_resources_get(
    int unit, 
    bcm_vxlan_port_t *vxlan_port,
    int ingress, 
    int egress, 
    int *global_in_lif,
    int *local_in_lif, 
    int *global_out_lif,
    int *local_out_lif, 
    SOC_PPC_FEC_ID *fec_index
 ){
    int rv = BCM_E_NONE;    
    _bcm_dpp_gport_hw_resources gport_hw_resources;


    BCMDNX_INIT_FUNC_DEFS;

    /* init to -1, for not exist */
    *local_in_lif = -1;
    *local_out_lif = -1;
    *fec_index = -1;

    if(ingress) {
        /* get match tunnel (terminator) */
        rv = _bcm_dpp_gport_to_hw_resources(unit, vxlan_port->match_tunnel_id, _BCM_DPP_GPORT_HW_RESOURCES_LOCAL_LIF_INGRESS | _BCM_DPP_GPORT_HW_RESOURCES_GLOBAL_LIF_INGRESS, 
                                                   &gport_hw_resources);
        BCMDNX_IF_ERR_EXIT(rv);
        *global_in_lif = gport_hw_resources.global_in_lif;
        *local_in_lif = gport_hw_resources.local_in_lif;
    }

    /* 
     * Always get egress information as well. 
     */

    /* use egress_tunnel if valid, points egress objects for protection/ECMP*/
    if ((vxlan_port->flags & BCM_VXLAN_PORT_EGRESS_TUNNEL)) {
        /* get FEC id */
        rv = _bcm_l3_intf_to_fec(unit, vxlan_port->egress_if, fec_index );
        BCMDNX_IF_ERR_EXIT(rv);
    }
    /* get outlif if valid */
    if (vxlan_port->egress_tunnel_id != 0) {
        /* get match tunnel (terminator) */
        rv = _bcm_dpp_gport_to_hw_resources(unit, vxlan_port->egress_tunnel_id, _BCM_DPP_GPORT_HW_RESOURCES_LOCAL_LIF_EGRESS | _BCM_DPP_GPORT_HW_RESOURCES_GLOBAL_LIF_EGRESS, 
                                                 &gport_hw_resources);
        BCMDNX_IF_ERR_EXIT(rv);
        *global_out_lif = gport_hw_resources.global_out_lif;
        *local_out_lif = gport_hw_resources.local_out_lif;
    }

exit:
    BCMDNX_FUNC_RETURN;
}


int
_bcm_petra_vxlan_check_lif(int unit, int local_lif, int ingress){
    int rv = BCM_E_NONE;
    bcm_tunnel_terminator_t info;
    SOC_PPC_EG_ENCAP_ENTRY_INFO *encap_entry_info = NULL;
    uint32 next_eep[SOC_PPC_NOF_EG_ENCAP_EEP_TYPES_MAX], nof_entries;
    uint32 soc_sand_rv;
    unsigned int soc_sand_dev_id;

    BCMDNX_INIT_FUNC_DEFS;

    if(ingress) {
        /* check if it's allocated from alloc manager */
        if (SOC_IS_ARADPLUS_AND_BELOW(unit)) {
            rv = bcm_dpp_am_l3_lif_is_allocated(unit, _BCM_DPP_AM_L3_LIF_IP_TERM, 0, local_lif);
            if(rv != BCM_E_EXISTS) {
                BCMDNX_IF_ERR_EXIT(rv);
            }
        } else if (SOC_IS_JERICHO(unit)) {
            rv = _bcm_dpp_am_local_inlif_is_alloc(unit, local_lif);
            if(rv != BCM_E_EXISTS) {
                BCMDNX_IF_ERR_EXIT(rv);
            }
        }

        /* check it's used for IPv4 [with GRE] from lif SW DB */
        rv = _bcm_dpp_in_lif_tunnel_term_get(unit, &info, local_lif);
        BCMDNX_IF_ERR_EXIT(rv);

        /* type has to be with VXLAN */
        if(info.type != bcmTunnelTypeVxlan && info.type != bcmTunnelTypeIpAnyIn4 &&
           info.type != bcmTunnelTypeVxlan6 && info.type != bcmTunnelTypeIpAnyIn6) {
            BCMDNX_ERR_EXIT_MSG(BCM_E_PARAM, (_BSL_BCM_MSG("%s: VXLAN cannot be build over this type tunnel has to be bcmTunnelTypeVxlan\n"), FUNCTION_NAME()));
        }
    }
    else{
        bcm_tunnel_type_t tunnel_type; 
        _bcm_tunnel_subtype_t tunnel_subtype; 

        /* egress info checked only if all devices in systems are synced
         * otherwsie check is not performed and input assumed to be valid
         */
        if(!_Bcm_dpp_sync_all) {
            BCM_EXIT;
        }

        /* check this entry is allocated */
        if (SOC_IS_ARADPLUS_AND_BELOW(unit)) {
            rv = bcm_dpp_am_ip_tunnel_eep_is_alloced(unit, 0, local_lif);
            if(rv != BCM_E_EXISTS) {
                BCM_IF_ERROR_RETURN(rv);
            }
        } else if (SOC_IS_JERICHO(unit)) {
            rv = _bcm_dpp_am_local_outlif_is_alloc(unit, local_lif);
            if(rv != BCM_E_EXISTS) {
                BCMDNX_IF_ERR_EXIT(rv);
            }
        }

        soc_sand_dev_id = (unit);

        BCMDNX_ALLOC(encap_entry_info, sizeof(*encap_entry_info) * SOC_PPC_NOF_EG_ENCAP_EEP_TYPES_MAX,
            "_bcm_petra_vxlan_check_lif.encap_entry_info");
        if(encap_entry_info == NULL) {
            BCMDNX_ERR_EXIT_MSG(BCM_E_MEMORY, (_BSL_BCM_MSG("Memory allocation failue\n")));
        }

        /* get egress info to check type is ipv4 gre */
        soc_sand_rv =
            soc_ppd_eg_encap_entry_get(soc_sand_dev_id,
                                   SOC_PPC_EG_ENCAP_EEP_TYPE_TUNNEL_EEP, local_lif, 1,
                                   encap_entry_info, next_eep, &nof_entries);
        BCM_SAND_IF_ERR_EXIT(soc_sand_rv);

        if ((encap_entry_info[0].entry_type != SOC_PPC_EG_ENCAP_ENTRY_TYPE_IPV4_ENCAP) && 
            (encap_entry_info[0].entry_type != SOC_PPC_EG_ENCAP_ENTRY_TYPE_IPV6_ENCAP)){
            BCMDNX_ERR_EXIT_MSG(BCM_E_PARAM, (_BSL_BCM_MSG("%s: egress tunnel has to be of IP type \n"), FUNCTION_NAME()));
        }

        /* below arad+, use gre encapsulation mode */

        /* get current tunnel type */
        rv = _bcm_petra_tunnel_ipv4_type_from_ppd(unit, &(encap_entry_info[0].entry_val.ipv4_encap_info), &tunnel_type, &tunnel_subtype); 
        BCMDNX_IF_ERR_EXIT(rv);

        if  (SOC_IS_ARAD_B1_AND_BELOW(unit)) {
            if ((tunnel_type != bcmTunnelTypeGreAnyIn4) && (tunnel_type != bcmTunnelTypeL2Gre)) {
                BCMDNX_ERR_EXIT_MSG(BCM_E_PARAM, (_BSL_BCM_MSG("%s: egress tunnel has to be of GRE type \n"), FUNCTION_NAME()));
            }

            /* fix type to be enhanced if's not */
            if (tunnel_type != bcmTunnelTypeL2Gre) {
                rv = _bcm_petra_tunnel_ipv4_type_to_ppd(unit, bcmTunnelTypeL2Gre, _bcm_tunnel_subtype_none, &(encap_entry_info[0].entry_val.ipv4_encap_info), TRUE); 
                BCMDNX_IF_ERR_EXIT(rv);

                soc_sand_rv =
                soc_ppd_eg_encap_ipv4_entry_add(soc_sand_dev_id, local_lif, 
                                &encap_entry_info[0].entry_val.ipv4_encap_info, next_eep[0]);
                SOC_SAND_IF_ERR_EXIT(soc_sand_rv);
            }
        }
        /* arad+ support vxlan encapsulation mode */
        #ifdef BCM_88660_A0
        else {
            if (tunnel_type != bcmTunnelTypeVxlan) {
                BCMDNX_ERR_EXIT_MSG(BCM_E_PARAM, (_BSL_BCM_MSG("%s: egress tunnel has to be of vxlan type \n"), FUNCTION_NAME()));
            }
        } 
        #endif

    }

exit:
    BCM_FREE(encap_entry_info);
    BCMDNX_FUNC_RETURN;
}


int
_bcm_petra_vxlan_learn_info_map_set(
    int unit, 
    int global_out_lif,
    bcm_gport_t match_port,
    SOC_PPC_FEC_ID fec_id,
    SOC_PPC_FRWRD_DECISION_INFO *learn_info
 ){
    unsigned int soc_sand_dev_id;
    uint32 soc_sand_rv;
    BCMDNX_INIT_FUNC_DEFS;

    soc_sand_dev_id = (unit);

    /* if FEC used, then learn FEC */
    if(fec_id != -1) {
        SOC_PPD_FRWRD_DECISION_FEC_SET(soc_sand_dev_id, learn_info, fec_id, soc_sand_rv);
        BCM_SAND_IF_ERR_EXIT(soc_sand_rv);
    }
    else{
        /* otherwise learn out-lif + port */
        soc_sand_rv = _bcm_dpp_gport_to_fwd_decision(soc_sand_dev_id,match_port,learn_info);
        BCM_SAND_IF_ERR_EXIT(soc_sand_rv);

        /* set outlif to learn */
        learn_info->additional_info.outlif.type = SOC_PPC_OUTLIF_ENCODE_TYPE_RAW;
        learn_info->additional_info.outlif.val = global_out_lif;
    }


exit:
    BCMDNX_FUNC_RETURN;
}


/*
*  SW state store for VXLAN:
*   mapping from VXLAN-gport (in-lif) to <out-FEC> or <out-LIF, out-port>
*   stored in hash table.
*  
*   used for:
*    - forward to gport.
*    - get function
*  
*   this information always stored: for local and remote gports.
*  
*   optimization:
*     - for local VXLAN port, if egress points to FEC and not out-LIF
*       then no need for storage, FEC can be restored from in-LIF 
 */

int 
bcm_petra_vxlan_port_sw_state_add(
    int unit, 
    bcm_vpn_t l2vpn, 
    bcm_vxlan_port_t *vxlan_port,
    int is_local,
    int eg_fec,
    int local_in_lif,
    int global_out_lif,
    int local_out_lif,
    int update)
 {
     _BCM_GPORT_PHY_PORT_INFO phy_port;
     int             rv = BCM_E_NONE;

     BCMDNX_INIT_FUNC_DEFS

    if(update) { /* if update remove from hash, reusue same memory*/
        rv = _bcm_dpp_sw_db_hash_vlan_find(unit,
                            (sw_state_htb_key_t) &(vxlan_port->vxlan_port_id), 
                            (sw_state_htb_data_t) &phy_port,TRUE);        
        if (GPORT_HASH_VLAN_NOT_FOUND(phy_port,rv)) {
            BCMDNX_ERR_EXIT_MSG(BCM_E_INTERNAL, (_BSL_BCM_MSG("update un-exist entry")));
        }
    }

    sal_memset(&phy_port, 0, sizeof(_BCM_GPORT_PHY_PORT_INFO));
    
    /* if fec store only FEC */
    if(eg_fec != -1) {
        phy_port.type = _bcmDppGportResolveTypeVxlanEgFec;
        phy_port.phy_gport = eg_fec;  /* physical port*/
        phy_port.encap_id = vxlan_port->match_port; /* as encap-id has no specific use, store match-port instead of read from FEC */
    }
    else{/* else store port + outlif*/
        phy_port.type = _bcmDppGportResolveTypeVxlan;
        phy_port.phy_gport = vxlan_port->match_port;  /* physical port*/
        phy_port.encap_id = global_out_lif; /* encap_id = out-AC */
    }

    rv = _bcm_dpp_sw_db_hash_vlan_insert(unit,
                                         (sw_state_htb_key_t) &(vxlan_port->vxlan_port_id),
                                         (sw_state_htb_data_t) &phy_port);        
    if (BCM_FAILURE(rv)) {
        LOG_ERROR(BSL_LS_BCM_VXLAN,
                  (BSL_META_U(unit,
                              "error(%s) Updating vxlan Gport Dbase (0x%x)\n"),
                   bcm_errmsg(rv), vxlan_port->vxlan_port_id));
        BCMDNX_IF_ERR_EXIT(rv);
    }

    /* update lif usage */
    rv = _bcm_dpp_local_lif_sw_resources_lif_usage_set(unit, local_in_lif, local_out_lif, _bcmDppLifTypeVxlan, _bcmDppLifTypeVxlan);
    BCMDNX_IF_ERR_EXIT(rv);

    if (local_in_lif != -1) {
        rv = _bcm_dpp_local_lif_sw_resources_vsi_set(unit, local_in_lif, l2vpn);
        BCMDNX_IF_ERR_EXIT(rv);
    }   

    /* Update FEC->GPORT SW DB */
    if(eg_fec != -1) {
        rv = bcm_dpp_gport_mgmt_sw_resources_fec_to_gport_set(unit, eg_fec, vxlan_port->vxlan_port_id);
        BCMDNX_IF_ERR_EXIT(rv);
    }

exit:
    BCMDNX_FUNC_RETURN;
}


int 
bcm_petra_vxlan_port_sw_state_remove(
    int unit, 
    bcm_vpn_t l2vpn, 
    bcm_gport_t vxlan_port_id,
    int is_local,
    int eg_fec,
    int in_lif,
    int out_lif)
 {
     _BCM_GPORT_PHY_PORT_INFO phy_port;
     int             rv = BCM_E_NONE;

     BCMDNX_INIT_FUNC_DEFS

    /* remove from hash, reusue same memory*/
    rv = _bcm_dpp_sw_db_hash_vlan_find(unit,
                        (sw_state_htb_key_t) &(vxlan_port_id), 
                        (sw_state_htb_data_t) &phy_port,TRUE);        
    if (GPORT_HASH_VLAN_NOT_FOUND(phy_port,rv)) {
        BCMDNX_ERR_EXIT_MSG(BCM_E_INTERNAL, (_BSL_BCM_MSG("update un-exist entry")));
    }
    phy_port.type = _bcmDppGportResolveTypeInvalid;

    /* update lif usage back to tunnel of type VXLAN */
    rv = _bcm_dpp_local_lif_sw_resources_lif_usage_set(unit, in_lif, out_lif, _bcmDppLifTypeIpTunnel, _bcmDppLifTypeIpTunnel);
    BCMDNX_IF_ERR_EXIT(rv);

    if (in_lif != -1) {
        rv = _bcm_dpp_local_lif_sw_resources_vsi_set(unit, in_lif, 0);
        BCMDNX_IF_ERR_EXIT(rv);
    }

exit:
    BCMDNX_FUNC_RETURN;
}


int 
bcm_petra_vxlan_port_add_verify(
    int unit, 
    bcm_vpn_t l2vpn, 
    int update,
    bcm_vxlan_port_t *vxlan_port)
{
    int rv = BCM_E_NONE;

    BCMDNX_INIT_FUNC_DEFS;
    /* check paramters */
    if ( l2vpn != BCM_VXLAN_VPN_INVALID) {
        /* check vpn is already exist */
        rv = _bcm_dpp_vlan_check(unit,l2vpn);
        BCMDNX_IF_ERR_EXIT(rv);
     }

    if(vxlan_port->criteria != BCM_VXLAN_PORT_MATCH_VN_ID){
        BCMDNX_ERR_EXIT_MSG(BCM_E_PARAM, (_BSL_BCM_MSG("%s: only vxlan ports can be created by this API \n"), FUNCTION_NAME()));
    }

     if (vxlan_port->flags & BCM_VXLAN_PORT_WITH_ID ) {
        if (!BCM_GPORT_IS_VXLAN_PORT(vxlan_port->vxlan_port_id)) {
            BCMDNX_ERR_EXIT_MSG(BCM_E_PARAM, (_BSL_BCM_MSG("%s: given id is not vxlan-port  \n"), FUNCTION_NAME()));
        }
    }

    /* use egress_tunnel if valid, points egress objects for protection/ECMP*/
    if ((vxlan_port->flags & BCM_VXLAN_PORT_EGRESS_TUNNEL)) {
        if(!_BCM_PETRA_L3_ITF_IS_VALID_FEC(unit,vxlan_port->egress_if)){
            BCMDNX_ERR_EXIT_MSG(BCM_E_PARAM, (_BSL_BCM_MSG("%s: egress_if is not valid  \n"), FUNCTION_NAME()));
        }
    } 

    /* check given match tunnel */
    if (!BCM_GPORT_IS_TUNNEL(vxlan_port->match_tunnel_id)) {
        return BCM_E_PARAM;
    }
    /* if update then has to be with ID */
    if (update && !(vxlan_port->flags & BCM_VXLAN_PORT_WITH_ID)) {
        BCMDNX_ERR_EXIT_MSG(BCM_E_PARAM, (_BSL_BCM_MSG("%s: if update then has to be WITH_ID \n"), FUNCTION_NAME()));
    }

    BCM_EXIT;
exit:
    BCMDNX_FUNC_RETURN;
}


/*
 * Function
 *      bcm_petra_vxlan_port_add
 * Purpose
 *      add VXLAN port to VPN
 * Parameters
 *      (in) unit       = unit number
 *      (in) mode       = group mode
 * Returns
 *      bcm_error_t = BCM_E_NONE if no error
 *                    BCM_E_* appropriately if not 
 */
int 
bcm_petra_vxlan_port_add(
    int unit, 
    bcm_vpn_t l2vpn, 
    bcm_vxlan_port_t *vxlan_port)
{
    int rv = BCM_E_NONE;
    SOC_PPC_FEC_ID fec_index = -1; /* if egress is FEC and not tunnel */
    int local_in_lif = -1; /* where match tunnel is defined */
    int local_out_lif = -1; /* where egress tunnel is defined */
    int global_in_lif = -1;
    int global_out_lif = -1;
    int ingress = 1;
    int egress = 1;
    int is_local_config=0;
    int update = 0;

    unsigned int soc_sand_dev_id;
    uint32 soc_sand_rv;
    SOC_PPC_LIF_ENTRY_INFO *lif_info = NULL;
    SOC_PPC_EG_ENCAP_ENTRY_INFO *encap_entry_info = NULL;

    BCMDNX_INIT_FUNC_DEFS;

    DPP_VXLAN_INIT_CHECK(unit);
    BCMDNX_NULL_CHECK(vxlan_port);

    soc_sand_dev_id = (unit);
    update = (vxlan_port->flags & BCM_VXLAN_PORT_REPLACE)?1:0;
    rv = bcm_petra_vxlan_port_add_verify(unit, l2vpn, update, vxlan_port);
    BCMDNX_IF_ERR_EXIT(rv);


    /* take lock */ 
    DPP_VXLAN_UNIT_LOCK(unit);

    rv = _bcm_dpp_gport_is_local(unit,vxlan_port->match_port,&is_local_config);
    BCMDNX_IF_ERR_EXIT(rv);

    /* if not local has to be with ID */
    if (!is_local_config && !(vxlan_port->flags & BCM_VXLAN_PORT_WITH_ID)) {
        BCMDNX_ERR_EXIT_MSG(BCM_E_PARAM, (_BSL_BCM_MSG("%s: if not local vxlan port, then has to be WITH_ID \n"), FUNCTION_NAME()));
    }

    /* check if this configuration related to ingress or egress  */
    rv = _bcm_petra_vxlan_side_config_get(unit,vxlan_port,&ingress,&egress);
    BCMDNX_IF_ERR_EXIT(rv);


    /* map l2gport to used resource */
    rv = _bcm_petra_vxlan_resources_get(unit, vxlan_port, ingress, egress, &global_in_lif, &local_in_lif, 
                                        &global_out_lif, &local_out_lif, &fec_index);
    BCMDNX_IF_ERR_EXIT(rv);

    /* If with_id, check that the inlif encoded in the vxlan gport
       is the inlif we extract from vxlan_port->match_tunnel_id */
    if (vxlan_port->flags & BCM_VXLAN_PORT_WITH_ID) {
        int global_in_lif_from_with_id = BCM_GPORT_VXLAN_PORT_ID_GET(vxlan_port->vxlan_port_id); 
        if (global_in_lif != global_in_lif_from_with_id) {
            BCMDNX_ERR_EXIT_MSG(BCM_E_PARAM, (_BSL_BCM_MSG("%s: given vxlan port gport is not encoded with the tunnel in-lif configured in match_tunnel_id\n"), FUNCTION_NAME()));
        }
    }

    /* If out_lif used and no protection (fec_index == -1) then it has to be equal to in_lif */
    if(local_out_lif != - 1 && fec_index == -1 && global_out_lif != global_in_lif) {
        BCMDNX_ERR_EXIT_MSG(BCM_E_PARAM, (_BSL_BCM_MSG("%s: match_tunnel 0x%x and egress_tunnel 0x%x have to be equal \n"), FUNCTION_NAME(), global_in_lif, global_out_lif));
    }

    /* three things to update */
    /* 1. LIF, learn-info + VSI-assignment mode.
       2. map VXLAN-vpn to VSI and vice-versa. note this is done in vpn-create.
       3. store SW state
     */

    /* update ingress */
    if(global_in_lif != -1 && is_local_config) {
        /* check it's used for IPv4 [with VXLAN] from lif SW DB */
        rv = _bcm_petra_vxlan_check_lif(unit, local_in_lif, 1);
        BCMDNX_IF_ERR_EXIT(rv);

       /* update LIF info */
        BCMDNX_ALLOC(lif_info, sizeof(SOC_PPC_LIF_ENTRY_INFO), "bcm_petra_v xlan_port_add.lif_info");
        if (lif_info == NULL) {        
            BCMDNX_ERR_EXIT_MSG(BCM_E_MEMORY, (_BSL_BCM_MSG("failed to allocate memory")));
        }
        soc_sand_rv = soc_ppd_lif_table_entry_get(soc_sand_dev_id,local_in_lif,lif_info);
        BCM_SAND_IF_ERR_EXIT(soc_sand_rv);

        if(lif_info->type != SOC_PPC_LIF_ENTRY_TYPE_IP_TUNNEL_RIF) {
            BCMDNX_ERR_EXIT_MSG(BCM_E_PARAM, (_BSL_BCM_MSG("%s: inlif Type is not expected, expect for IP tunnel term \n"), FUNCTION_NAME()));
        }

        /* assume: MP so set learn info */
        lif_info->value.ip_term_info.learn_enable = 1;
        rv = _bcm_petra_vxlan_learn_info_map_set(unit,global_out_lif,vxlan_port->match_port,fec_index, &(lif_info->value.ip_term_info.learn_record));
        BCMDNX_IF_ERR_EXIT(rv);

        /* set VSI to zero as taken from VSI-assignment mode according to LIF-index*/
        /* note the assigment of VSI is taken from LIF only, the vsi here is used to save SW state */
        lif_info->value.ip_term_info.vsi = l2vpn;
        lif_info->value.ip_term_info.vsi_assignment_mode = SOC_PPC_VSI_EQ_IN_LIF;

        /* Update orientation in inLif structure */

        if (SOC_IS_JERICHO(unit) && soc_property_get(unit, spn_SPLIT_HORIZON_FORWARDING_GROUPS_MODE, 1)) {
            uint32 profile_split_horizon_val; 

            /* 2b orientation support: orientation msb in lif.outlif_profile,
                                       orientation lsb in lif.orientation */
            lif_info->value.ip_term_info.orientation = vxlan_port->network_group_id & 1;
            if (soc_property_get(unit, spn_SPLIT_HORIZON_FORWARDING_GROUPS_MODE, 1) != 3) {
                profile_split_horizon_val = (vxlan_port->network_group_id & 2) >> 1;
                rv = MBCM_PP_DRIVER_CALL(unit, mbcm_pp_occ_mgmt_app_set,
                                         (unit, SOC_OCC_MGMT_TYPE_INLIF, SOC_OCC_MGMT_INLIF_APP_ORIENTATION,
                                          profile_split_horizon_val,&(lif_info->value.ip_term_info.lif_profile)));
                BCMDNX_IF_ERR_EXIT(rv);
            }

        } else {
            lif_info->value.ip_term_info.orientation = 
                (vxlan_port->flags & BCM_VXLAN_PORT_NETWORK) ? SOC_SAND_PP_HUB_SPOKE_ORIENTATION_HUB : SOC_SAND_PP_HUB_SPOKE_ORIENTATION_SPOKE;
        }
        /* VXLAN tunnels learn FEC only which shouldn't set the global_lif */
        if (SOC_IS_JERICHO(unit)) {
            lif_info->value.ip_term_info.global_lif = 0;
        }
        soc_sand_rv = soc_ppd_lif_table_entry_update(soc_sand_dev_id,local_in_lif, lif_info);
        BCM_SAND_IF_ERR_EXIT(soc_sand_rv);

    }

    /* update egress */
    if(global_out_lif != -1 && is_local_config) {
        /* fix gre type from 4 to 8 */
        rv = _bcm_petra_vxlan_check_lif(unit, local_out_lif,0);
        BCMDNX_IF_ERR_EXIT(rv);

#ifdef BCM_ARAD_SUPPORT
        if (SOC_IS_JERICHO(unit)) {
            /* update IP tunnel outlif:
               if need to update orientation in outlif profile 
               if need to update is_l2_lif in outlif profile */
            if (soc_property_get(unit, spn_SPLIT_HORIZON_FORWARDING_GROUPS_MODE, 1) 
                || SOC_IS_JERICHO_PLUS(unit)) {
                /* In this mode the orientation should be saved to the outlif profile */
                uint32 
                    next_eep[SOC_PPC_NOF_EG_ENCAP_EEP_TYPES_MAX],
                    nof_entries;

                BCMDNX_ALLOC(encap_entry_info, sizeof(*encap_entry_info)*SOC_PPC_NOF_EG_ENCAP_EEP_TYPES_MAX, "bcm_petra_vxlan_port_add.encap_entry_info");
                if (encap_entry_info == NULL) {        
                    BCMDNX_ERR_EXIT_MSG(BCM_E_MEMORY, (_BSL_BCM_MSG("failed to allocate memory")));
                }

                soc_sand_rv = soc_ppd_eg_encap_entry_get(
                    soc_sand_dev_id,
                    SOC_PPC_EG_ENCAP_EEP_TYPE_TUNNEL_EEP, local_out_lif, 1,
                    encap_entry_info, next_eep, &nof_entries);
                SOC_SAND_IF_ERR_EXIT(soc_sand_rv);

                if (encap_entry_info[0].entry_type == SOC_PPC_EG_ENCAP_ENTRY_TYPE_IPV6_ENCAP) {
                    /* update orientation in outlif profile */
                    if (soc_property_get(unit, spn_SPLIT_HORIZON_FORWARDING_GROUPS_MODE, 1))  {
                        uint32 profile_split_horizon_val;
                        if (vxlan_port->flags & BCM_VXLAN_PORT_NETWORK) {
                            profile_split_horizon_val = vxlan_port->network_group_id;
                        } else {
                            profile_split_horizon_val = 0; 
                        }

                        soc_sand_rv = MBCM_PP_DRIVER_CALL(unit, mbcm_pp_occ_mgmt_app_set,
                          (unit, SOC_OCC_MGMT_TYPE_OUTLIF, SOC_OCC_MGMT_OUTLIF_APP_ORIENTATION, profile_split_horizon_val,
                              &(encap_entry_info[0].entry_val.data_info.outlif_profile)));
                        BCMDNX_IF_ERR_EXIT(soc_sand_rv);
                    }

                    /* update is_l2_lif in outlif profile.
                                Used in ROO application to build native LL in case a L2_LIF is in the encapsulation stack */
                    if (SOC_IS_JERICHO_PLUS(unit)) {
                        /* set entry to be L2LIF */
                        soc_sand_rv = MBCM_PP_DRIVER_CALL(unit, mbcm_pp_occ_mgmt_app_set,
                                          (unit, SOC_OCC_MGMT_TYPE_OUTLIF, SOC_OCC_MGMT_OUTLIF_APP_ROO_IS_L2_LIF, 1,
                                              &(encap_entry_info[0].entry_val.data_info.outlif_profile)));
                        BCMDNX_IF_ERR_EXIT(soc_sand_rv);
                    }

                    soc_sand_rv = soc_ppd_eg_encap_data_lif_entry_add(soc_sand_dev_id, local_out_lif, 
                                                                  &encap_entry_info[0].entry_val.data_info, TRUE, next_eep[0]);
                    SOC_SAND_IF_ERR_EXIT(soc_sand_rv);
                } else {
                    /* update orientation in outlif profile */
                    if (soc_property_get(unit, spn_SPLIT_HORIZON_FORWARDING_GROUPS_MODE, 1))  {
                        uint32 profile_split_horizon_val;
                        if (vxlan_port->flags & BCM_VXLAN_PORT_NETWORK) {
                            profile_split_horizon_val = vxlan_port->network_group_id;
                        } else {
                            profile_split_horizon_val = 0; 
                        }

                        soc_sand_rv = MBCM_PP_DRIVER_CALL(unit, mbcm_pp_occ_mgmt_app_set,
                          (unit, SOC_OCC_MGMT_TYPE_OUTLIF, SOC_OCC_MGMT_OUTLIF_APP_ORIENTATION, profile_split_horizon_val,
                              &(encap_entry_info[0].entry_val.ipv4_encap_info.outlif_profile)));
                        BCMDNX_IF_ERR_EXIT(soc_sand_rv);
                    }

                    /* update is_l2_lif in outlif profile.
                                Used in ROO application to build native LL in case a L2_LIF is in the encapsulation stack */
                    if (SOC_IS_JERICHO_PLUS(unit)) {
                        /* set entry to be L2LIF */
                        soc_sand_rv = MBCM_PP_DRIVER_CALL(unit, mbcm_pp_occ_mgmt_app_set,
                                          (unit, SOC_OCC_MGMT_TYPE_OUTLIF, SOC_OCC_MGMT_OUTLIF_APP_ROO_IS_L2_LIF, 1,
                                              &(encap_entry_info[0].entry_val.ipv4_encap_info.outlif_profile)));
                        BCMDNX_IF_ERR_EXIT(soc_sand_rv);
                    }

                    soc_sand_rv = soc_ppd_eg_encap_ipv4_entry_add(soc_sand_dev_id, local_out_lif, 
                                                                  &encap_entry_info[0].entry_val.ipv4_encap_info, next_eep[0]);
                    SOC_SAND_IF_ERR_EXIT(soc_sand_rv);
                }
            }
        }
        /* update orientation for outlif in auxiliary table if egress hairpin is enabled set egress state as well*/
        else if (SOC_DPP_CONFIG(unit)->arad->pp_op_mode.split_horizon_filter_enable) {
            SOC_SAND_PP_HUB_SPOKE_ORIENTATION orientation = 
                (vxlan_port->flags & BCM_VXLAN_PORT_NETWORK) ? SOC_SAND_PP_HUB_SPOKE_ORIENTATION_HUB : SOC_SAND_PP_HUB_SPOKE_ORIENTATION_SPOKE; 
            soc_sand_rv = soc_ppd_eg_filter_split_horizon_out_lif_orientation_set(
               soc_sand_dev_id,local_out_lif, orientation);
            BCM_SAND_IF_ERR_EXIT(soc_sand_rv);
        }


#endif /* BCM_ARAD_SUPPORT */
    }

    /* set VXLAN port id if needed*/
    if(!(vxlan_port->flags & BCM_VXLAN_PORT_WITH_ID)) {
        _bcm_petra_vxlan_port_id_from_lif(unit,global_in_lif,&(vxlan_port->vxlan_port_id));
    }

    /* store SW state */
    rv = _bcm_dpp_local_lif_sw_resources_learn_gport_set(unit, local_in_lif, vxlan_port->vxlan_port_id);
    BCMDNX_IF_ERR_EXIT(rv);
    rv = bcm_petra_vxlan_port_sw_state_add(unit, l2vpn, vxlan_port, is_local_config, fec_index, local_in_lif, 
                                           global_out_lif, local_out_lif, update);
    BCMDNX_IF_ERR_EXIT(rv);


exit:
    DPP_VXLAN_UNIT_UNLOCK(unit);
    BCM_FREE(lif_info);
    BCM_FREE(encap_entry_info);
    BCMDNX_FUNC_RETURN;
}

int 
bcm_petra_vxlan_cleanup(
    int unit)
{
    BCMDNX_INIT_FUNC_DEFS;

    BCM_DPP_UNIT_CHECK(unit);
    if (!_dpp_vxlan_state[unit]) {
        /* not init, so detach done already */
        BCM_RETURN_VAL_EXIT(BCM_E_NONE);
    }
    if(_dpp_vxlan_state[unit]->lock) {
        sal_mutex_destroy(_dpp_vxlan_state[unit]->lock);
    }
    _dpp_vxlan_state[unit]->lock = NULL;

    /* get rid of allocated structure */
    BCM_FREE(_dpp_vxlan_state[unit]);

exit:
    BCMDNX_FUNC_RETURN;
}


int 
bcm_petra_vxlan_init(
    int unit)
{
    _dpp_vxlan_state_t *temp_state = NULL;

    BCMDNX_INIT_FUNC_DEFS;
    BCM_DPP_UNIT_CHECK(unit);

    /* create new unit state information */
    BCMDNX_ALLOC(temp_state, sizeof(_dpp_vxlan_state_t), "bcm_petra_vxlan_init");

    if (temp_state) {
        sal_memset(temp_state, 0, sizeof(_dpp_vxlan_state_t));
        temp_state->lock = sal_mutex_create("_dpp_vxlan_unit_lock");
        if (!temp_state->lock) {
           BCMDNX_ERR_EXIT_MSG(BCM_E_RESOURCE, (_BSL_BCM_MSG("%s: fail to allocate lock \n"), FUNCTION_NAME()));
        }
    } else {
      BCMDNX_ERR_EXIT_MSG(BCM_E_MEMORY, (_BSL_BCM_MSG("%s: fail to allocate memory \n"), FUNCTION_NAME()));
    }



    _dpp_vxlan_state[unit] = temp_state;
    BCMDNX_FUNC_RETURN; /* Don't go to exit because there is no need to free temp_state*/

    /* configure soc global parameters */
exit:
    BCM_FREE(temp_state);
    BCMDNX_FUNC_RETURN;  
}



int 
bcm_petra_vxlan_port_delete(
    int unit, 
    bcm_vpn_t l2vpn, 
    bcm_gport_t vxlan_port_id)
{
    int         global_in_lif;
    int         global_out_lif;
    int         local_in_lif;
    int         local_out_lif;
    int         eg_fec;
    int         phy_port;
    int       is_local;
    unsigned int soc_sand_dev_id;
    uint32 soc_sand_rv;
    SOC_PPC_LIF_ENTRY_INFO lif_info;
    int rv = BCM_E_NONE;

    BCMDNX_INIT_FUNC_DEFS;
    BCM_DPP_UNIT_CHECK(unit);
    soc_sand_dev_id = (unit);

    DPP_VXLAN_INIT_CHECK(unit);
    /* take lock */ 
    DPP_VXLAN_UNIT_LOCK(unit);

    /* get <port,in-lif, out-lif, FEC associated with this GPORT */
    rv = _bcm_petra_vxlan_port_id_to_resources(unit, vxlan_port_id, &global_in_lif, &local_in_lif,
                                               &global_out_lif, &local_out_lif, &eg_fec, &phy_port, &is_local);
    BCMDNX_IF_ERR_EXIT(rv);

    /* 1. remove: LIF learn-info + VSI-assignment mode.*/
    if(global_in_lif != -1 && is_local) {
        /* check it's used for IPv4 [with VXLAN] from lif SW DB */
        rv = _bcm_petra_vxlan_check_lif(unit, local_in_lif, 1);
        BCMDNX_IF_ERR_EXIT(rv);

       /* update LIF info */
        soc_sand_rv = soc_ppd_lif_table_entry_get(soc_sand_dev_id, local_in_lif, &lif_info);
        BCM_SAND_IF_ERR_EXIT(soc_sand_rv);

        if(lif_info.type != SOC_PPC_LIF_ENTRY_TYPE_IP_TUNNEL_RIF) {
            BCMDNX_ERR_EXIT_MSG(BCM_E_PARAM, (_BSL_BCM_MSG("%s: inlif Type is not expected, expect for IP tunnel term \n"), FUNCTION_NAME()));
        }

        /* disable learning  */
        lif_info.value.ip_term_info.learn_enable = 0;
        /* set VSI to zero as */
        lif_info.value.ip_term_info.vsi = 0;
        /* set back vsi assignment to normal, will be zero for this removed tunnel*/
        lif_info.value.ip_term_info.vsi_assignment_mode = SOC_PPC_VSI_EQ_VSI_BASE;

        soc_sand_rv = soc_ppd_lif_table_entry_update(soc_sand_dev_id, local_in_lif, &lif_info);
        BCM_SAND_IF_ERR_EXIT(soc_sand_rv);
    }

    /* remove store SW state */
    rv = bcm_petra_vxlan_port_sw_state_remove(unit, l2vpn, vxlan_port_id, is_local, eg_fec, local_in_lif, local_out_lif);
    BCMDNX_IF_ERR_EXIT(rv);


    exit:
if (unit >= 0 && unit < BCM_MAX_NUM_UNITS) {
    DPP_VXLAN_UNIT_UNLOCK(unit);     
}
    BCMDNX_FUNC_RETURN;
}


int 
bcm_petra_vxlan_port_delete_all(
    int unit, 
    bcm_vpn_t l2vpn)
{
    BCMDNX_INIT_FUNC_DEFS;
    DPP_VXLAN_INIT_CHECK(unit);
    return bcm_petra_vswitch_port_delete_all(unit,l2vpn);

    exit:
    BCMDNX_FUNC_RETURN;
}



int 
bcm_petra_vxlan_port_get(
    int unit, 
    bcm_vpn_t l2vpn, 
    bcm_vxlan_port_t *vxlan_port)
{
    int         global_in_lif;
    int         global_out_lif;
    int         local_in_lif;
    int         local_out_lif;
    int         eg_fec;
    int         phy_port;
    int  gport_id;
    int       is_local;
    int rv = BCM_E_NONE;
    SOC_PPC_LIF_ENTRY_INFO *lif_info = NULL;
    uint32 soc_sand_rv;
    unsigned int soc_sand_dev_id;
    _bcm_lif_type_e lif_usage;

    BCMDNX_INIT_FUNC_DEFS;

    /* take lock */ 
    BCM_DPP_UNIT_CHECK(unit);
    DPP_VXLAN_INIT_CHECK(unit);
    DPP_VXLAN_UNIT_LOCK(unit);

    soc_sand_dev_id = (unit);

    global_in_lif = _bcm_petra_vxlan_port_id_to_lif(unit, vxlan_port->vxlan_port_id);
    if (global_in_lif == -1) {
        BCM_RETURN_VAL_EXIT(BCM_E_PARAM);
    }
    gport_id = vxlan_port->vxlan_port_id;

    /* get <port,in-lif, out-lif, FEC associated with this GPORT */
    rv = _bcm_petra_vxlan_port_id_to_resources(unit, vxlan_port->vxlan_port_id, &global_in_lif, &local_in_lif, 
                                               &global_out_lif, &local_out_lif, &eg_fec, &phy_port, &is_local);
    BCMDNX_IF_ERR_EXIT(rv);

    /* Check it is not the FEC */
    if (global_out_lif != -1) {
        /* Verify outLIF is really VXLAN. */
        BCMDNX_IF_ERR_EXIT(_bcm_dpp_local_lif_sw_resources_lif_usage_get(unit, -1, local_out_lif, NULL, &lif_usage));
        if (lif_usage != _bcmDppLifTypeVxlan) {
            BCMDNX_ERR_EXIT_MSG(BCM_E_PORT, (_BSL_BCM_MSG("OutLIF is not VXLAN")));
        }
    }

    /* fill vxlan_port*/
    bcm_vxlan_port_t_init(vxlan_port);

    /* ID */
    vxlan_port->vxlan_port_id = gport_id;

    /* const values */
    vxlan_port->criteria = BCM_VXLAN_PORT_MATCH_VN_ID;
    /* match port */
    vxlan_port->match_port = phy_port;
    /* FEC */
    if(eg_fec != -1) {
        _bcm_l3_fec_to_intf(unit, eg_fec, &vxlan_port->egress_if);
        vxlan_port->flags |= BCM_VXLAN_PORT_EGRESS_TUNNEL;
    }
    else{ /* point directly to tunnel */
        BCM_GPORT_TUNNEL_ID_SET(vxlan_port->egress_tunnel_id, global_out_lif);
    }

    /* match tunnel */
    BCM_GPORT_TUNNEL_ID_SET(vxlan_port->match_tunnel_id, global_in_lif);
    /* Get port orientation information from register */
    BCMDNX_ALLOC(lif_info, sizeof(SOC_PPC_LIF_ENTRY_INFO), "bcm_petra_v xlan_port_add.lif_info");
    if (lif_info == NULL) {        
        BCMDNX_ERR_EXIT_MSG(BCM_E_MEMORY, (_BSL_BCM_MSG("failed to allocate memory")));
    }
    SOC_PPC_LIF_ENTRY_INFO_clear(lif_info);
    soc_sand_rv = soc_ppd_lif_table_entry_get(soc_sand_dev_id, local_in_lif, lif_info);
    BCM_SAND_IF_ERR_EXIT(soc_sand_rv);
    
    if(lif_info->type != SOC_PPC_LIF_ENTRY_TYPE_IP_TUNNEL_RIF) {
        BCMDNX_ERR_EXIT_MSG(BCM_E_PARAM, (_BSL_BCM_MSG("%s: inlif Type is not expected, expect for IP tunnel term \n"), FUNCTION_NAME()));
    }

    /* 2b orientation support: orientation msb in lif.outlif_profile,
                               orientation lsb in lif.orientation */
    if (SOC_IS_JERICHO(unit) && soc_property_get(unit, spn_SPLIT_HORIZON_FORWARDING_GROUPS_MODE, 1)) {
        uint32 profile_orientation = 0;
        if (soc_property_get(unit, spn_SPLIT_HORIZON_FORWARDING_GROUPS_MODE, 1) != 3) {
            soc_sand_rv = MBCM_PP_DRIVER_CALL(unit, mbcm_pp_occ_mgmt_app_get,
                                 (unit, SOC_OCC_MGMT_TYPE_INLIF, SOC_OCC_MGMT_INLIF_APP_ORIENTATION,
                                  &(lif_info->value.ip_term_info.lif_profile), &profile_orientation));
            BCMDNX_IF_ERR_EXIT(soc_sand_rv);
        }
        vxlan_port->network_group_id = ((profile_orientation & 1) << 1) | (lif_info->value.ip_term_info.orientation & 1); 

        if (vxlan_port->network_group_id > 0) {
            vxlan_port->flags |= BCM_VXLAN_PORT_NETWORK;
        }
    }
    /* 1b orientation support */
    else {
        if (lif_info->value.ip_term_info.orientation == SOC_SAND_PP_HUB_SPOKE_ORIENTATION_HUB) {
            vxlan_port->flags |= BCM_VXLAN_PORT_NETWORK;
        }
    }
exit:

    if (unit >= 0 && unit < BCM_MAX_NUM_UNITS) {
        DPP_VXLAN_UNIT_UNLOCK(unit);     
    }

    BCM_FREE(lif_info);
    BCMDNX_FUNC_RETURN;
}

/* not supported */
int 
bcm_petra_vxlan_port_get_all(
    int unit, 
    bcm_vpn_t l2vpn, 
    int port_max, 
    bcm_vxlan_port_t *port_array, 
    int *port_count)
{
    int rv = BCM_E_NONE;
    int iter;
    int port_val;
    int ports_num = 0;
    int vsi;

    BCMDNX_INIT_FUNC_DEFS;

    BCMDNX_NULL_CHECK(port_array);
    BCMDNX_NULL_CHECK(port_count);

    /* check vsi exist */
    rv = _bcm_dpp_vlan_check(unit, l2vpn);
    BCMDNX_IF_ERR_EXIT(rv);

    for(iter = 0; iter != -1;) {
        /* get next port */
        rv = _bcm_dpp_vpn_get_next_gport(unit, l2vpn, &iter, &port_val, &vsi);
        BCMDNX_IF_ERR_EXIT(rv);

        /* no more ports in vpn */
        if(port_val == BCM_GPORT_INVALID) {
            break;
        }

        port_array[ports_num].vxlan_port_id = port_val;
        /* only for VXLAN ports full content is returned for othe port types, only id is retuend use vswitch_get for these ports */
        if(BCM_GPORT_IS_VXLAN_PORT(port_val)) {
            rv = bcm_petra_vxlan_port_get(unit,l2vpn,&port_array[ports_num]);
            BCMDNX_IF_ERR_EXIT(rv);
        }
        /* delete next port */
        ++ports_num;
        if (ports_num == port_max) {
            *port_count = ports_num;
            BCM_EXIT;
        }
    }
    *port_count = ports_num;

exit:
    BCMDNX_FUNC_RETURN;
}


int 
bcm_petra_vxlan_vpn_create_verify(
    int unit,
    int update,
    bcm_vxlan_vpn_config_t *info)
{
    BCMDNX_INIT_FUNC_DEFS;
    /* check parameters */
    /* p2p is not supported */
    if(info->flags & BCM_VXLAN_VPN_ELINE){
        BCMDNX_ERR_EXIT_MSG(BCM_E_UNAVAIL, (_BSL_BCM_MSG("%s: BCM_VXLAN_VPN_ELINE is not supported \n"), FUNCTION_NAME()));
    }

    if(!(info->flags & BCM_VXLAN_VPN_WITH_VPNID)){
        BCMDNX_ERR_EXIT_MSG(BCM_E_UNAVAIL, (_BSL_BCM_MSG("%s: BCM_VXLAN_VPN_WITH_VPNID, must present in vpn create \n"), FUNCTION_NAME()));
    }

    /* if update then has to be with ID */
    if (update && !(info->flags & BCM_VXLAN_VPN_WITH_ID)) {
        BCMDNX_ERR_EXIT_MSG(BCM_E_PARAM, (_BSL_BCM_MSG("%s: if update then has to be WITH_ID \n"), FUNCTION_NAME()));
    }

    /* if update then the VPN should be existed */
    if (update && (BCM_E_EXISTS != bcm_dpp_am_l2_vpn_vsi_is_alloced(unit, info->vpn))) {
        BCMDNX_ERR_EXIT_MSG(BCM_E_NOT_FOUND, (_BSL_BCM_MSG("VPN %d is not allocated"),info->vpn));
    }

    BCM_EXIT;
exit:
    BCMDNX_FUNC_RETURN;
}


int 
bcm_petra_vxlan_vpn_create_vlan_control_update(
    int unit,
    bcm_vlan_control_vlan_t *vlan_control,
    bcm_vxlan_vpn_config_t  *info)
{
    uint32 replace_flags = 0;
    int rv = BCM_E_NONE;

    BCMDNX_INIT_FUNC_DEFS;
    BCMDNX_NULL_CHECK(vlan_control);
    /* Update unknown unicast mc-group, unknown multicast  mc-group and broadcast mc-group
     * base replace flag.
     */
    rv = bcm_petra_vlan_control_vlan_get(unit,info->vpn,vlan_control);
    BCMDNX_IF_ERR_EXIT(rv);

    if (info->flags & BCM_VXLAN_VPN_REPLACE) {
        replace_flags = (BCM_VXLAN_VPN_UNKNOWN_UCAST_REPLACE | 
                         BCM_VXLAN_VPN_UNKNOWN_MCAST_REPLACE |
                         BCM_VXLAN_VPN_BCAST_REPLACE);
    } else {
        replace_flags = (info->flags & (BCM_VXLAN_VPN_UNKNOWN_UCAST_REPLACE | 
                         BCM_VXLAN_VPN_UNKNOWN_MCAST_REPLACE |
                         BCM_VXLAN_VPN_BCAST_REPLACE));
    }
    
    if (replace_flags & BCM_VXLAN_VPN_UNKNOWN_UCAST_REPLACE) {
        vlan_control->unknown_unicast_group = info->unknown_unicast_group;
    }
    
    if (replace_flags & BCM_VXLAN_VPN_UNKNOWN_MCAST_REPLACE) {
        vlan_control->unknown_multicast_group = info->unknown_multicast_group;
    }
    
    if (replace_flags & BCM_VXLAN_VPN_BCAST_REPLACE) {
        vlan_control->broadcast_group = info->broadcast_group;
    }

    BCM_EXIT;
exit:
    BCMDNX_FUNC_RETURN;
}


int 
bcm_petra_vxlan_vpn_create(
    int unit, 
    bcm_vxlan_vpn_config_t *info)
{
    int flags=0;
    bcm_vlan_control_vlan_t vlan_control;
    SOC_PPC_L2_LIF_VXLAN_KEY vxlan_key;
    SOC_PPC_L2_LIF_VXLAN_INFO vxlan_info;
    SOC_PPC_LIF_ID vxlan_lif_index;
    SOC_SAND_SUCCESS_FAILURE failure_indication;
    uint32 soc_sand_rv;
    unsigned int soc_sand_dev_id;
    int rv = BCM_E_NONE;
    int update = 0;
    int ignore_key;

    BCMDNX_INIT_FUNC_DEFS;

    DPP_VXLAN_INIT_CHECK(unit);
    BCMDNX_NULL_CHECK(info);

    update = (info->flags & _BCM_VXLAN_VPN_REPLACE_FLAGS)?1:0;
    rv = bcm_petra_vxlan_vpn_create_verify(unit, update, info);
    BCMDNX_IF_ERR_EXIT(rv);

    /* take lock */ 
    DPP_VXLAN_UNIT_LOCK(unit);

    soc_sand_dev_id = (unit);

    /* For create a new VPN, allocate a new VPN and add entry mapping VPN to VSI */
    if (!update) {
        /* with ID allocate */
        if(info->flags & BCM_VXLAN_VPN_WITH_ID) {
            flags=BCM_DPP_AM_FLAG_ALLOC_WITH_ID;
        }

        /* map l2vpn to Vpn */
        SOC_PPC_L2_LIF_VXLAN_KEY_clear(&vxlan_key);
        SOC_PPC_L2_LIF_VXLAN_INFO_clear(&vxlan_info);
        /* allocate VSI */
        rv = _bcm_dpp_vswitch_vsi_usage_alloc(unit,flags,_bcmDppVsiTypeVxlan,&info->vpn,NULL);
        BCMDNX_IF_ERR_EXIT(rv);

        /* packet's VXLAN key */
        vxlan_key.vpn_key = info->vnid;
        vxlan_key.match_port_class = info->match_port_class;
        /* VSI */
        vxlan_info.vsid = info->vpn;
        /* HW constraint LIF = VSI
           the check is done in the vpn-create
         */
        vxlan_lif_index = vxlan_info.vsid;
        ignore_key = 0; /* indicates that we add VNI->VSI at ISEM */

        /* add entry mapping VPN (VNI) to VSI, using SEM
           beside add mapping at egress from VSI to VPN (VNI)
         */
        soc_sand_rv = soc_ppd_l2_lif_vxlan_add(
                soc_sand_dev_id,
                &vxlan_key,
                vxlan_lif_index,
                &vxlan_info,
                ignore_key,
                &failure_indication);
        BCM_SAND_IF_ERR_EXIT(soc_sand_rv);
        SOC_SAND_IF_FAIL_RETURN(failure_indication);

        if (SOC_IS_VXLAN_VDC_ENABLE(unit)) {
            /* Store VSI->VDxVNI mapping to SW DB */
            rv = arad_pp_sw_db_vdc_vsi_mapping_set(unit, _BCM_VXLAN_VPN_TO_VSI(unit, info->vpn), vxlan_key.match_port_class, vxlan_key.vpn_key);
            BCMDNX_IF_ERR_EXIT(rv);
        }
    }


    bcm_vlan_control_vlan_t_init(&vlan_control);

    if (update) {
        rv = bcm_petra_vxlan_vpn_create_vlan_control_update(unit, &vlan_control, info);
        BCMDNX_IF_ERR_EXIT(rv);

        if(info->flags & BCM_VXLAN_VPN_WITH_VPNID) {
            /* Replace VNI */
            uint32 replaced_vni = 0;
            bcm_vpn_t vsi;

            SOC_PPC_L2_LIF_VXLAN_KEY_clear(&vxlan_key);
            SOC_PPC_L2_LIF_VXLAN_INFO_clear(&vxlan_info);

            replaced_vni = info->vnid;
            vsi = info->vpn;

            /*
             * get the entry that mapped VSI to VNI
             */
            soc_sand_rv = soc_ppd_l2_lif_vsi_to_isid(
                unit,
                _BCM_VXLAN_VPN_TO_VSI(unit, vsi),
                &vxlan_key.vpn_key);
            BCM_SAND_IF_ERR_EXIT(soc_sand_rv);

            /* VSI */
            vxlan_info.vsid = vsi;
            vxlan_key.match_port_class = info->match_port_class;
            /* HW constraint LIF = VSI
             * the check is done in the vpn-create
             */
            vxlan_lif_index = vxlan_info.vsid;

            /*
             * remove entry mapping VNI to VSI
             */
            soc_sand_rv = soc_ppd_l2_lif_vxlan_remove(
                 soc_sand_dev_id,
                 &vxlan_key,
                 &vxlan_lif_index);
            BCM_SAND_IF_ERR_EXIT(soc_sand_rv);

            if(replaced_vni == BCM_VXLAN_VNI_INVALID) {
                vxlan_key.vpn_key = 0;
                ignore_key = 1; /* Indicate that we don't add VNI->VSI mapping at ISEM */
            }
            else {
                ignore_key = 0; /* indicates that we add VNI->VSI at ISEM */
                vxlan_key.vpn_key = replaced_vni;
            }
            vxlan_lif_index = vxlan_info.vsid;

            /* add entry mapping VPN (VNI) to VSI, using SEM
             * beside add mapping at egress from VSI to VPN (VNI)
             */
            soc_sand_rv = soc_ppd_l2_lif_vxlan_add(
                soc_sand_dev_id,
                &vxlan_key,
                vxlan_lif_index,
                &vxlan_info,
                ignore_key,
                &failure_indication);
            BCM_SAND_IF_ERR_EXIT(soc_sand_rv);
            SOC_SAND_IF_FAIL_RETURN(failure_indication);

            if (SOC_IS_VXLAN_VDC_ENABLE(unit)) {
                /* Store VSI->VDxVNI mapping to SW DB */
                rv = arad_pp_sw_db_vdc_vsi_mapping_set(unit, _BCM_VXLAN_VPN_TO_VSI(unit, info->vpn), vxlan_key.match_port_class, vxlan_key.vpn_key);
                BCMDNX_IF_ERR_EXIT(rv);
            }
        }
    }else {
        vlan_control.broadcast_group = info->broadcast_group;
        vlan_control.unknown_multicast_group = info->unknown_multicast_group;
        vlan_control.unknown_unicast_group = info->unknown_unicast_group;
        /* forwarding vlan has to be equal to vsi */
        vlan_control.forwarding_vlan = info->vpn;
    }

    rv = bcm_petra_vlan_control_vlan_set(unit,info->vpn,vlan_control);
    BCMDNX_IF_ERR_EXIT(rv);

exit:
    if (unit >= 0 && unit < BCM_MAX_NUM_UNITS) {
        DPP_VXLAN_UNIT_UNLOCK(unit);     
    }
    BCMDNX_FUNC_RETURN;  
}


int 
bcm_petra_vxlan_vpn_destroy(
    int unit, 
    bcm_vpn_t l2vpn)
{
    int rv = BCM_E_NONE;
    SOC_PPC_LIF_ID vxlan_lif_index;
    SOC_PPC_L2_LIF_VXLAN_KEY vxlan_key;
    unsigned int soc_sand_dev_id;
    uint32 soc_sand_rv;
    int is_detach_ports;

    BCMDNX_INIT_FUNC_DEFS;

    DPP_VXLAN_INIT_CHECK(unit);

    SOC_PPC_L2_LIF_VXLAN_KEY_clear(&vxlan_key);

    /* remove mapping from VXLAN-vpn to l2vpn (vsi)*/

        /* take lock */ 
    DPP_VXLAN_UNIT_LOCK(unit);

    /* check vpn is already exist */
    rv = _bcm_dpp_vlan_check(unit,l2vpn);
    BCMDNX_IF_ERR_EXIT(rv);

    /* as only vsi given, get VXLAN-VPN from HW mapping */
   /* map l2vpn to Vpn */
    soc_sand_dev_id = (unit);
    if (SOC_IS_VXLAN_VDC_ENABLE(unit)) {
        /* Get VSI->VDxVNI mapping from SW DB */
        rv = arad_pp_sw_db_vdc_vsi_mapping_get(unit, _BCM_VXLAN_VPN_TO_VSI(unit, l2vpn), &(vxlan_key.match_port_class), &(vxlan_key.vpn_key));
        BCMDNX_IF_ERR_EXIT(rv);
    } else{
        soc_sand_rv = soc_ppd_l2_lif_vsi_to_isid(soc_sand_dev_id,_BCM_VXLAN_VPN_TO_VSI(unit, l2vpn),&vxlan_key.vpn_key);
        BCM_SAND_IF_ERR_EXIT(soc_sand_rv);
    }
    /* 
     * remove entry mapping VPN to VSI
     */
    soc_sand_rv = soc_ppd_l2_lif_vxlan_remove(
            soc_sand_dev_id,
            &vxlan_key,
            &vxlan_lif_index);
    BCM_SAND_IF_ERR_EXIT(soc_sand_rv);

    /* no need to free usage of vxlan_lif_index as it was not allocated */

    /* Get the VSI detach mode to be applied in the destroy */
    rv =  _bcm_dpp_switch_control_get(unit, bcmSwitchVpnDetachPorts, &is_detach_ports);
    BCMDNX_IF_ERR_EXIT(rv);

    /* Destroy the VSI and optionally detach all associated ports */
    rv = bcm_petra_vswitch_destroy_internal(unit, l2vpn, _bcmDppVsiTypeVxlan, (is_detach_ports) ? TRUE : FALSE);
    BCMDNX_IF_ERR_EXIT(rv);
    
    if (SOC_IS_VXLAN_VDC_ENABLE(unit)) {
        /* Clear VSI->VDxVNI mapping */
        rv = arad_pp_sw_db_vdc_vsi_mapping_set(unit, _BCM_VXLAN_VPN_TO_VSI(unit, l2vpn), 0, 0);
        BCMDNX_IF_ERR_EXIT(rv);
    }

exit:
    if (unit >= 0 && unit < BCM_MAX_NUM_UNITS) {
        DPP_VXLAN_UNIT_UNLOCK(unit);     
    }
    BCMDNX_FUNC_RETURN;  
}


int 
bcm_petra_vxlan_vpn_destroy_all(
    int unit)
{
    return BCM_E_UNAVAIL;
    /* call vswitch_destroy_all */
    /* where vswitch_destroy_all: call vxlan_vpn_destroy if needed */
}


int 
bcm_petra_vxlan_vpn_get(
    int unit, 
    bcm_vpn_t l2vpn, 
    bcm_vxlan_vpn_config_t *info)
{

    int rv = BCM_E_NONE;
    bcm_vlan_control_vlan_t vlan_control;
    uint32 soc_sand_rv;
    unsigned int soc_sand_dev_id;
    uint8 found;
    SOC_PPC_L2_LIF_VXLAN_KEY vxlan_key;

    BCMDNX_INIT_FUNC_DEFS;

    DPP_VXLAN_INIT_CHECK(unit);
    BCMDNX_NULL_CHECK(info);

    SOC_PPC_L2_LIF_VXLAN_KEY_clear(&vxlan_key);

    vxlan_key.vpn_key = info->vnid;
    vxlan_key.match_port_class = info->match_port_class;

    /* check vpn is already exist */
    rv = _bcm_dpp_vlan_check(unit,l2vpn);
    BCMDNX_IF_ERR_EXIT(rv);

    /* take lock */ 
    DPP_VXLAN_UNIT_LOCK(unit);
    soc_sand_dev_id = (unit);


    rv = bcm_petra_vlan_control_vlan_get(unit,l2vpn,&vlan_control);
    BCMDNX_IF_ERR_EXIT(rv);

    bcm_vxlan_vpn_config_t_init(info);

    /* set back vpn id after clear */
    info->vpn = l2vpn;
    /* set flooding groups */
    info->broadcast_group = vlan_control.broadcast_group;
    info->unknown_multicast_group = vlan_control.unknown_multicast_group;
    info->unknown_unicast_group = vlan_control.unknown_unicast_group;

    /* only MP is supported */
    info->flags |= BCM_VXLAN_VPN_ELAN|BCM_VXLAN_VPN_WITH_VPNID;

   if (SOC_IS_VXLAN_VDC_ENABLE(unit)) {
       /* Get VSI->VDxVNI mapping from SW DB */
       rv = arad_pp_sw_db_vdc_vsi_mapping_get(unit, _BCM_VXLAN_VPN_TO_VSI(unit, l2vpn), &(info->match_port_class), &(info->vnid));
       BCMDNX_IF_ERR_EXIT(rv);
   } else{
       /* VXLAN-vpn-id, HW map according to 12 lsb of vpn */
        soc_sand_rv = soc_ppd_l2_lif_vsi_to_isid(soc_sand_dev_id,_BCM_VXLAN_VPN_TO_VSI(unit, l2vpn),&info->vnid);
        BCM_SAND_IF_ERR_EXIT(soc_sand_rv);
        if (info->vnid == 0) {

            SOC_PPC_L2_LIF_VXLAN_INFO vxlan_info;
            SOC_PPC_LIF_ID vxlan_lif_index;

            vxlan_info.vsid = info->vpn;
            vxlan_lif_index = vxlan_info.vsid;

            /* Check VNI->VSI to see if it's unmapped */
            soc_sand_rv = soc_ppd_l2_lif_vxlan_get(
                    soc_sand_dev_id,
                    &vxlan_key,
                    &vxlan_lif_index,
                    &vxlan_info,
                    &found);
            BCM_SAND_IF_ERR_EXIT(soc_sand_rv);
            if (!found) {
                info->vnid = BCM_VXLAN_VNI_INVALID; /* VNI -1 indicates invalid VNI number */
            }
        }
    }

exit:
    if (unit >= 0 && unit < BCM_MAX_NUM_UNITS) {
        DPP_VXLAN_UNIT_UNLOCK(unit);     
    }
    BCMDNX_FUNC_RETURN;  
}


/*  
* Function:
*      bcm_petra_vxlan_vpn_traverse
* Purpose:
*      Get information about a VPN instance
* Parameters:
*      unit  - (IN)  Device Number
*      cb    - (IN)  User-provided callback
*      info  - (IN/OUT) Cookie
* Returns:
*      BCM_E_XXX
*/

int bcm_petra_vxlan_vpn_traverse(int unit, bcm_vxlan_vpn_traverse_cb cb,
                                 void *user_data)
{
   int rv = BCM_E_UNAVAIL;

   return rv;
}

#endif /* INCLUDE_L3 */
