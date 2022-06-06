/*
 * $Id: port.c,v 1.36 Broadcom SDK $
 * 
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 */

#include <soc/defs.h>
#include <soc/debug.h>
#include <soc/drv.h>
#include <soc/mem.h>

#include <sal/core/time.h>
#include <shared/bsl.h>
#include <bcm/port.h>
#include <bcm/tx.h>
#include <bcm/error.h>
#include <bcm_int/esw/stack.h>
#include <bcm_int/esw/katana2.h>
#include <bcm_int/esw/saber2.h>
#include <bcm_int/esw/cosq.h>
#include <bcm_int/esw/triumph3.h>
#include <soc/katana2.h>
#include <soc/saber2.h>
#include <soc/phyreg.h>
#include <soc/ll.h>

#if defined(BCM_METROLITE_SUPPORT)
#include <soc/metrolite.h>
#endif

#if defined(BCM_SABER2_SUPPORT)
#include <bcm_int/esw_dispatch.h>
#include <bcm_int/esw/katana.h>
#include <bcm_int/esw/katana2.h>
#include <bcm_int/esw/virtual.h>
#include <bcm_int/common/sat.h>

static uint32   sb2_port_lanes_unchanged;
static uint32   new_tdm[SB2_TDM_MAX_SIZE] = {0};
extern int      block_max_startaddr[SB2_MAX_BLOCKS];
extern int      block_max_endaddr[SB2_MAX_BLOCKS];
extern sb2_port_to_block_subports_t *sb2_port_to_block_subports[SOC_MAX_NUM_DEVICES];
extern int _sb2_config_id[SOC_MAX_NUM_DEVICES];
extern sb2_speed_t *bcm56260_speed[SOC_MAX_NUM_DEVICES];
extern int eagle_n4s_mode;
extern int eagle_n2_mode;
extern int eagle_4ge_mode;

#if defined(BCM_METROLITE_SUPPORT)
extern ml_speed_t *bcm56270_speed[SOC_MAX_NUM_DEVICES];
#endif
extern int _bcm_sb2_ipmc_port_ext_queue_count(int unit, int ipmc_id, bcm_port_t port);
extern int _bcm_kt2_ipmc_subscriber_egress_queue_ext_set(int unit, int port, int ipmc_id, int ext_mem_enable);

#define SB2_SPEED_MODE_LINK_1G          2
#define SB2_SPEED_MODE_LINK_2G5         3
#define SB2_SPEED_MODE_LINK_10G_PLUS    4

int
_bcm_sb2_port_sw_info_display(int unit, bcm_port_t port) {

    int        rv = 0;
    soc_info_t *si = &SOC_INFO(unit);
    int        lanes;
    uint8      block;
    int        enable;
    int        speed;
    int        link;
    int        autoneg;
    int        discard_mode;
    int        loopback_mode;
    int        master_mode;
    int        encap_mode = BCM_PORT_ENCAP_COUNT;
    char       *discard[] = {"NONE", "ALL", "UNTAG", "TAG"};
    char       *loopback[] = {"NONE", "MAC", "PHY", "RMT", "C57"};
    char       *master[] = {"SLAVE", "MASTER", "AUTO", "NONE"};
    char       *encap[] = {"IEEE", "HIGIG", "B5632", "HIGIG2", "HIGIG-Lite"};

    rv += (_bcm_sb2_port_lanes_get(unit, port, &lanes));
    rv += (soc_saber2_get_port_block(unit,port,&block));
    rv += (bcm_esw_port_enable_get(unit, port, &enable));
    rv += (bcm_esw_port_autoneg_get(unit, port, &autoneg));
    rv += (bcm_esw_port_speed_get(unit, port, &speed));
    rv += (bcm_esw_port_link_status_get(unit, port, &link));
    rv += (bcm_esw_port_discard_get(unit, port, &discard_mode));
    rv += (bcm_esw_port_loopback_get(unit, port, &loopback_mode));
    rv += (bcm_esw_port_master_get(unit, port, &master_mode));
    rv += (bcm_esw_port_encap_get(unit, port, &encap_mode));

    if (rv < 0) {
        LOG_CLI((BSL_META_U(unit, "\n\n Some parameters not set properly !!")));
    }
    LOG_CLI((BSL_META_U(unit, "\n\n SOFTWARE PORT CONFIGURATION :\n")));
    LOG_CLI((BSL_META_U(unit, "------------------------------------")));
    LOG_CLI((BSL_META_U(unit, "\n Port Name\t\t:\t%s"),si->port_name[port]));
    LOG_CLI((BSL_META_U(unit, "\n Block ID\t\t:\t%d"), block));
    LOG_CLI((BSL_META_U(unit, "\n Lane Number\t\t:\t%d "),lanes));
    LOG_CLI((BSL_META_U(unit, "\n Current Speed\t\t:\t%d"), speed));
    LOG_CLI((BSL_META_U(unit, "\n Max speed\t\t:\t%d"),si->port_speed_max[port]));
    LOG_CLI((BSL_META_U(unit, "\n Speed\t\t\t:\t%d"),((* 
                        sb2_port_speeds[unit])[block][lanes-1])));
    LOG_CLI((BSL_META_U(unit, "\n Port status \t\t:\t%s"),(enable ? "ENABLED" :    "DISABLED")));
    LOG_CLI((BSL_META_U(unit, "\n Link status\t\t:\t%s"),(link ? "UP" : "DOWN")));
    LOG_CLI((BSL_META_U(unit, "\n Autoneg status\t\t:\t%s"),(autoneg ? "TRUE" :    "FALSE")));
    LOG_CLI((BSL_META_U(unit, "\n Discard Type\t\t:\t%s"),(discard_mode <= 3 ?     discard[discard_mode] : "INVALID")));
    LOG_CLI((BSL_META_U(unit, "\n Loopback Mode\t\t:\t%s"),(loopback_mode <= 4 ?   loopback[loopback_mode] : "INVALID")));
    LOG_CLI((BSL_META_U(unit, "\n Master Status\t\t:\t%s"),(master_mode <= 3 ?     master[master_mode] : "INVALID")));
    LOG_CLI((BSL_META_U(unit, "\n Encapsulation \t\t:\t%s"),(encap_mode <= 4 ?     encap[encap_mode] : "INVALID")));
    LOG_CLI((BSL_META_U(unit, "\n\n*********************************************** \n\n")));

    return BCM_E_NONE;

}

int 
_bcm_sb2_port_hw_info_display(int unit, bcm_port_t port) {

    uint32 rval = 0; 
    uint64 rval_64;
    int lanes = 0; 
    uint8 mxqblock;
    int port0, port1, port2, port3;
    int higig, higig2;
    int core_mode, phy_mode;
    int tx_en, rx_en, ipg_check_disable, lpbk_leak;
    int soft_reset, rs_soft_reset, sw_link, link_status_select; 
    int hdr_mode, sop_crc, speed_mode; 
    int port_enable;
    egr_enable_entry_t egr_en;

    char *phy[] = {"Single", "Dual", "Quad"};
    char *core[] = {"Single", "Dual", "Quad"};
    char *enable[] = {"DISABLED", "ENABLED"};
    char *active[] = {"INACTIVE", "ACTIVE"};
    char *link_sel[] = {"SOFTWARE", "STRAP-PIN"};
    char *hdr[] = {"IEEE", "HG+", "HG2", "CLH", "SCH", "SOP ONLY IEEE"};
    char *speed[] = {"10Mpbps", "100Mbps", "1Gbps", "2.5Gbps", ">=10Gbps"};

    BCM_IF_ERROR_RETURN(_bcm_sb2_port_lanes_get(unit, port, &lanes));
    BCM_IF_ERROR_RETURN(soc_saber2_get_port_block(unit, port, &mxqblock));
    
    LOG_CLI((BSL_META_U(unit, "\n\n HARDWARE PORT CONFIGURATION : \n")));
    LOG_CLI((BSL_META_U(unit, "--------------------------------------\n")));

    /* XPORT_MODE_REG */
    if (SOC_REG_PORT_VALID(unit, XPORT_MODE_REGr, port)) {
        SOC_IF_ERROR_RETURN(READ_XPORT_MODE_REGr(unit, port, &rval));
        phy_mode  = soc_reg_field_get(unit, XPORT_MODE_REGr, rval, PHY_PORT_MODEf);
        LOG_CLI((BSL_META_U(unit, "\n XPORT MODE ")));
        LOG_CLI((BSL_META_U(unit, "\n Phy Mode\t:\t%s"), phy[phy_mode]));
    }

    /* XLPORT_MODE_REG */
    if (SOC_REG_PORT_VALID(unit, XLPORT_MODE_REGr, port)) {
        SOC_IF_ERROR_RETURN(READ_XLPORT_MODE_REGr(unit, port, &rval));
        core_mode = soc_reg_field_get(unit, XLPORT_MODE_REGr, rval,
                XPORT0_CORE_PORT_MODEf);
        phy_mode  = soc_reg_field_get(unit, XLPORT_MODE_REGr, rval,
                XPORT0_PHY_PORT_MODEf);
        LOG_CLI((BSL_META_U(unit, "\n XPORT MODE ")));
        LOG_CLI((BSL_META_U(unit, "\n Core Mode\t:\t%s"), core[core_mode]));
        LOG_CLI((BSL_META_U(unit, "\n Phy Mode\t:\t%s"), phy[phy_mode]));
    }

    /* XPORT_PORT_ENABLE */
    if (SOC_REG_PORT_VALID(unit, XPORT_PORT_ENABLEr, port)) {
        SOC_IF_ERROR_RETURN(READ_XPORT_PORT_ENABLEr(unit, port, &rval));
        port0 = soc_reg_field_get(unit, XPORT_PORT_ENABLEr, rval, PORT0f);
        port1 = soc_reg_field_get(unit, XPORT_PORT_ENABLEr, rval, PORT1f);
        port2 = soc_reg_field_get(unit, XPORT_PORT_ENABLEr, rval, PORT2f);
        port3 = soc_reg_field_get(unit, XPORT_PORT_ENABLEr, rval, PORT3f);
        LOG_CLI((BSL_META_U(unit, "\n\n XPORT PORT ENABLE ")));
        LOG_CLI((BSL_META_U(unit, "\n MxQBlock\t:\t%d\n Port0\t\t:\t%s\n Port1\t\t:\t%s"
                     "\n Port2\t\t:\t%s\n Port3\t\t:\t%s"), mxqblock,   \
                     enable[port0], enable[port1], enable[port2], enable[port3]));
    }

    /* XLPORT_ENABLE_REG */
    if (SOC_REG_PORT_VALID(unit, XLPORT_ENABLE_REGr, port)) {
        SOC_IF_ERROR_RETURN(READ_XLPORT_ENABLE_REGr(unit, port, &rval));
        port0 = soc_reg_field_get(unit, XLPORT_ENABLE_REGr, rval, PORT0f);
        port1 = soc_reg_field_get(unit, XLPORT_ENABLE_REGr, rval, PORT1f);
        port2 = soc_reg_field_get(unit, XLPORT_ENABLE_REGr, rval, PORT2f);
        port3 = soc_reg_field_get(unit, XLPORT_ENABLE_REGr, rval, PORT3f);
        LOG_CLI((BSL_META_U(unit, "\n\n XLPORT PORT ENABLE ")));
        LOG_CLI((BSL_META_U(unit, "\n MxQBlock\t:\t%d\n Port0\t\t:\t%s\n Port1\t\t:\t%s"
                     "\n Port2\t\t:\t%s\n Port3\t\t:\t%s"), mxqblock,   \
                     enable[port0], enable[port1], enable[port2], enable[port3]));
    }

    /* XPORT_CONFIG */
    if (SOC_REG_PORT_VALID(unit, XPORT_CONFIGr, port)) {
        SOC_IF_ERROR_RETURN(READ_XPORT_CONFIGr(unit, port, &rval));
        higig = soc_reg_field_get(unit, XPORT_CONFIGr, rval, HIGIG_MODEf);
        higig2 = soc_reg_field_get(unit, XPORT_CONFIGr, rval, HIGIG2_MODEf);
        LOG_CLI((BSL_META_U(unit, "\n\n XPORT CONFIG ")));
        LOG_CLI((BSL_META_U(unit, "\n HiGig\t\t:\t%s"), enable[higig]));
        LOG_CLI((BSL_META_U(unit, "\n HiGig2\t\t:\t%s"), enable[higig2]));
    }

    /* XLPORT_CONFIG */
    if (SOC_REG_PORT_VALID(unit, XLPORT_CONFIGr, port)) {
        SOC_IF_ERROR_RETURN(READ_XLPORT_CONFIGr(unit, port, &rval));
        higig = soc_reg_field_get(unit, XLPORT_CONFIGr, rval, HIGIG_MODEf);
        higig2 = soc_reg_field_get(unit, XLPORT_CONFIGr, rval, HIGIG2_MODEf);
        LOG_CLI((BSL_META_U(unit, "\n\n XLPORT CONFIG ")));
        LOG_CLI((BSL_META_U(unit, "\n HiGig\t\t:\t%s"), enable[higig]));
        LOG_CLI((BSL_META_U(unit, "\n HiGig2\t\t:\t%s"), enable[higig2]));
    }

    /* XLMAC_CTRL */
    SOC_IF_ERROR_RETURN(READ_XLMAC_CTRLr(unit, port, &rval_64));
    tx_en = soc_reg64_field32_get(unit, XLMAC_CTRLr, rval_64, TX_ENf);
    rx_en = soc_reg64_field32_get(unit, XLMAC_CTRLr, rval_64, RX_ENf);
    soft_reset = soc_reg64_field32_get(unit, XLMAC_CTRLr, rval_64, SOFT_RESETf);
    rs_soft_reset = soc_reg64_field32_get(unit, XLMAC_CTRLr, rval_64, RS_SOFT_RESETf);
    lpbk_leak = soc_reg64_field32_get(unit, XLMAC_CTRLr, rval_64, LOCAL_LPBK_LEAK_ENBf);
    sw_link = soc_reg64_field32_get(unit, XLMAC_CTRLr, rval_64, SW_LINK_STATUSf);
    link_status_select = soc_reg64_field32_get(unit, XLMAC_CTRLr, rval_64, 
            LINK_STATUS_SELECTf);
    ipg_check_disable = soc_reg64_field32_get(unit, XLMAC_CTRLr, rval_64,
            XGMII_IPG_CHECK_DISABLEf);
    LOG_CLI((BSL_META_U(unit, "\n\n XMAC CTRL")));
    LOG_CLI((BSL_META_U(unit, "\n Tx\t \t\t:\t%s"), enable[tx_en]));
    LOG_CLI((BSL_META_U(unit, "\n Rx \t\t\t:\t%s"), enable[rx_en]));
    LOG_CLI((BSL_META_U(unit, "\n Soft Reset\t\t:\t%s"),enable[soft_reset]));
    LOG_CLI((BSL_META_U(unit, "\n RS Soft Reset\t\t:\t%s"), enable[rs_soft_reset]));
    LOG_CLI((BSL_META_U(unit, "\n RS Soft Reset\t\t:\t%s"), enable[rs_soft_reset]));
    LOG_CLI((BSL_META_U(unit, "\n Lpbk Leak(Local)\t:\t%s"), enable[lpbk_leak]));
    LOG_CLI((BSL_META_U(unit, "\n SW Link Status\t\t:\t%s"), active[sw_link]));
    LOG_CLI((BSL_META_U(unit, "\n Link Status Select\t:\t%s"), link_sel[link_status_select]));
    LOG_CLI((BSL_META_U(unit, "\n ipg check disable\t:\t%s"), enable[ipg_check_disable]));

    /* XLMAC_MODE*/
    SOC_IF_ERROR_RETURN(READ_XLMAC_MODEr(unit, port, &rval_64));
    hdr_mode = soc_reg64_field32_get(unit, XLMAC_MODEr, rval_64, HDR_MODEf);
    sop_crc  = soc_reg64_field32_get(unit, XLMAC_MODEr, rval_64, NO_SOP_FOR_CRC_HGf);
    speed_mode = soc_reg64_field32_get(unit, XLMAC_MODEr, rval_64, SPEED_MODEf);
    LOG_CLI((BSL_META_U(unit, "\n\n XMAC MODE")));
    LOG_CLI((BSL_META_U(unit, "\n Header Mode\t\t:\t%s"),hdr[hdr_mode]));
    LOG_CLI((BSL_META_U(unit, "\n SOP for CRC in HG\t:\t%s"),
                (sop_crc ? "EXCLUDE" : "INCLUDE")));
    LOG_CLI((BSL_META_U(unit, "\n Speed Mode\t\t:\t%s"), speed[speed_mode]));

    /* EGR_ENABLE */
    SOC_IF_ERROR_RETURN(READ_EGR_ENABLEm(unit, SOC_BLOCK_ANY, port, &egr_en));
    port_enable = soc_EGR_ENABLEm_field32_get(unit, &egr_en, PRT_ENABLEf);
    LOG_CLI((BSL_META_U(unit, "\n\n EGR ENABLE")));
    LOG_CLI((BSL_META_U(unit, "\n Port\t\t:\t%s\n\n"),enable[port_enable]));

    return BCM_E_NONE;

}

/*
 * Feature implementation is based on KT2 approach.
 * So changing the lanes convention as per the implementation.
 * lanes = 4 --> 4 active ports in the port block.(Eg : 4 ge ports)
 * lanes = 1 --> 1 active port in the port block.(Eg : 1 xe port)
 */
int
_bcm_kt2_lanes_convention(int *lanes) {

    switch(*lanes) {
        case 1 : *lanes = 4;
                 break;
        case 2 : *lanes = 2;
                 break;
        case 4 : *lanes = 1;
                 break;
        default : return BCM_E_PARAM;
    }
    return BCM_E_NONE;
}

int
_bcm_sb2_port_lanes_set_post_operation(int unit, bcm_port_t port)
{
    soc_error_t        rv = BCM_E_NONE;
    uint8              loop = 0;
    uint8              block = 0;
    uint8              block_port = 0;
    int                mode = 0;
    int                new_lanes = 0;
    int                port_enable = 0;
    int                speed=0, max_speed=0;

    if (sb2_port_lanes_unchanged) {
        return BCM_E_NONE;
    }

    SOC_IF_ERROR_RETURN(soc_saber2_get_port_block(
                unit, port, &block));
    SOC_IF_ERROR_RETURN(_bcm_sb2_port_lanes_get(
                unit, port,&new_lanes));
    BCM_IF_ERROR_RETURN(_bcm_kt2_lanes_convention(&new_lanes));

#ifdef BCM_PORT_DEFAULT_DISABLE
        port_enable = FALSE;
#else
        port_enable = TRUE;
#endif  /* BCM_PORT_DEFAULT_DISABLE */

    for (loop = 0;loop < SB2_MAX_PORTS_PER_BLOCK; loop++) {
        block_port=(*sb2_block_ports[unit])[block][loop];
        if (SOC_PBMP_MEMBER((PBMP_ALL(unit)), block_port)) {
            SOC_IF_ERROR_RETURN(bcm_esw_linkscan_mode_get(
                        unit, block_port, &mode));

            if (mode == BCM_LINKSCAN_MODE_NONE) {
                BCM_IF_ERROR_RETURN(bcm_esw_linkscan_mode_set(
                            unit, block_port, BCM_LINKSCAN_MODE_SW));
            }

            BCM_IF_ERROR_RETURN(bcm_esw_port_enable_set(unit,
                        block_port, port_enable));

            if (block < 6) {
                speed = (*sb2_port_speeds[unit])[block][new_lanes-1];
            } else {
                /* Add exception for eagle port due to mixed mode */
                soc_saber2_xl_port_speed_get(unit, block_port, &speed);
            }

            BCM_IF_ERROR_RETURN(bcm_esw_port_speed_max(unit,
                        block_port,  &max_speed));
            speed = speed > max_speed ? max_speed : speed;
            rv = bcm_esw_port_speed_set(unit, block_port, speed);
            SOC_IF_ERROR_RETURN(rv);

            BCM_IF_ERROR_RETURN(bcm_esw_port_learn_set(unit,
                        block_port,
                        BCM_PORT_LEARN_ARL | BCM_PORT_LEARN_FWD));
            BCM_IF_ERROR_RETURN(bcm_esw_port_stp_set(
                        unit, block_port, BCM_STG_STP_FORWARD));

            BCM_IF_ERROR_RETURN(bcm_esw_port_loopback_set(unit,
                        block_port, BCM_PORT_LOOPBACK_NONE));
        } /* if SOC_PBMP_MEMBER */
    } /* For loop */

    return BCM_E_NONE;
}

void soc_saber2_pbmp_add(int unit,soc_port_t port,int block,
                         int speed, int new_lanes) {
    int         blk=0;
    soc_info_t  *si = &SOC_INFO(unit);
    soc_pbmp_t  my_pbmp_xport_xe;
    soc_pbmp_t  my_pbmp_xport_ge;

    blk = SOC_DRIVER(unit)->port_info[port].blk;
    SOC_PBMP_PORT_ADD(si->block_bitmap[blk],port);

    if (block < 6) {
         /* mxq ports */
         if (speed == 10000) {
             SOC_PBMP_PORT_ADD(si->xe.bitmap, port);
             SOC_PBMP_PORT_REMOVE(si->xe.disabled_bitmap, port);
         } else {
             SOC_PBMP_PORT_ADD(si->ge.bitmap, port);
             SOC_PBMP_PORT_REMOVE(si->ge.disabled_bitmap, port);
         }
         SOC_PBMP_PORT_ADD(si->ether.bitmap, port);
         SOC_PBMP_PORT_REMOVE(si->ether.disabled_bitmap, port);

         SOC_PBMP_PORT_ADD(si->mxq.bitmap, port);
         SOC_PBMP_PORT_REMOVE(si->mxq.disabled_bitmap, port);

         SOC_PBMP_PORT_ADD(si->port.bitmap, port);
         SOC_PBMP_PORT_REMOVE(si->port.disabled_bitmap, port);

         SOC_PBMP_PORT_ADD(si->all.bitmap, port);
         SOC_PBMP_PORT_REMOVE(si->all.disabled_bitmap, port);

         SOC_PORT_TYPE(unit,port)=SOC_BLK_MXQPORT;
         si->port_speed_max[port] = speed;
    } else if (block == 6) {
        /* xl ports */
        SOC_PBMP_CLEAR(my_pbmp_xport_xe);
        SOC_PBMP_CLEAR(my_pbmp_xport_ge);
        my_pbmp_xport_xe = soc_property_get_pbmp_default(unit, spn_PBMP_XPORT_XE,
                                                     my_pbmp_xport_xe);
        my_pbmp_xport_ge = soc_property_get_pbmp_default(unit, spn_PBMP_XPORT_GE,
                                                     my_pbmp_xport_ge);

        si->port_speed_max[port] = speed;
        SOC_PBMP_PORT_ADD(si->xl.bitmap, port);
        SOC_PBMP_PORT_REMOVE(si->xl.disabled_bitmap, port);

        /* On XL port speed 2.5G can mean pbmp_xport_ge config or HGL port */
        if (SOC_PBMP_MEMBER(my_pbmp_xport_ge, port) &&
                (new_lanes == 4)) {
            SOC_PBMP_PORT_ADD(si->ge.bitmap, port);
            SOC_PBMP_PORT_REMOVE(si->ge.disabled_bitmap, port);
            si->port_speed_max[port] = 2500;
        } else if (SOC_PBMP_MEMBER(my_pbmp_xport_xe, port) || 
            (speed < 2500) || (eagle_n4s_mode && (new_lanes == 4))) {
             if (speed == 10000) {
                 SOC_PBMP_PORT_ADD(si->xe.bitmap, port);
                 SOC_PBMP_PORT_REMOVE(si->xe.disabled_bitmap, port);
             } else if (SOC_PBMP_MEMBER(my_pbmp_xport_xe, port) &&
                     (new_lanes != 4)) {
                 if(speed <= 2500) {
                    si->port_speed_max[port] = 11000; 
                 }
                 SOC_PBMP_PORT_ADD(si->xe.bitmap, port);
                 SOC_PBMP_PORT_REMOVE(si->xe.disabled_bitmap, port);
             } else {
                 SOC_PBMP_PORT_ADD(si->ge.bitmap, port);
                 SOC_PBMP_PORT_REMOVE(si->ge.disabled_bitmap, port);
             }
        } else if (SOC_PBMP_MEMBER(my_pbmp_xport_ge, port) &&
                (new_lanes == 4)) {
            SOC_PBMP_PORT_ADD(si->ge.bitmap, port);
            SOC_PBMP_PORT_REMOVE(si->ge.disabled_bitmap, port);
            si->port_speed_max[port] = 2500;
        } else {
            if(speed <= 2500) {
                /* The port has been migrated from ge to hg.
                 * Change speed accordingly.
                 */
                si->port_speed_max[port] = 11000;
            }
            SOC_PBMP_PORT_ADD(si->hg.bitmap, port);
            SOC_PBMP_PORT_ADD(si->st.bitmap, port);
            SOC_PBMP_PORT_REMOVE(si->hg.disabled_bitmap, port);
            SOC_PBMP_PORT_REMOVE(si->st.disabled_bitmap, port);
        }

         SOC_PBMP_PORT_ADD(si->ether.bitmap, port);
         SOC_PBMP_PORT_REMOVE(si->ether.disabled_bitmap, port);

         SOC_PBMP_PORT_ADD(si->port.bitmap, port);
         SOC_PBMP_PORT_REMOVE(si->port.disabled_bitmap, port);

         SOC_PBMP_PORT_ADD(si->all.bitmap, port);
         SOC_PBMP_PORT_REMOVE(si->all.disabled_bitmap, port);
         
         SOC_PORT_TYPE(unit,port)=SOC_BLK_XLPORT;
    }
}

int _bcm_sb2_update_port_mode(int unit,uint8 port,int speed) {
    uint32      rval; 
    uint8       block;
    bcmMxqPhyPortMode_t phy_mode = -1;
    bcmXlCorePortMode_t core_mode_xl = -1;
    bcmXlPhyPortMode_t  phy_mode_xl = -1;
     
    SOC_IF_ERROR_RETURN(soc_saber2_get_port_block(
                                unit, port, &block));

    if(IS_MXQ_PORT(unit, port)) {
        SOC_IF_ERROR_RETURN(soc_saber2_get_mxq_phy_port_mode(
                    unit, port, speed, &phy_mode));
        if(phy_mode == -1) {
            return BCM_E_FAIL;
        }
    }
    if (SOC_REG_PORT_VALID(unit, XPORT_MODE_REGr, port)) {
        SOC_IF_ERROR_RETURN(READ_XPORT_MODE_REGr(unit, port, &rval));
        soc_reg_field_set(unit, XPORT_MODE_REGr, &rval, PHY_PORT_MODEf, phy_mode);
        soc_reg_field_set(unit, XPORT_MODE_REGr, &rval,
                PORT_GMII_MII_ENABLEf, (speed >= 10000) ? 0 : 1);
        SOC_IF_ERROR_RETURN(WRITE_XPORT_MODE_REGr(unit, port, rval));
    }

    if(IS_XL_PORT(unit, port)) {
        SOC_IF_ERROR_RETURN(soc_saber2_get_xl_phy_core_port_mode(unit, port,
                &phy_mode_xl, &core_mode_xl));
        if((phy_mode_xl == -1) || (core_mode_xl == -1)){
            return BCM_E_FAIL;
        }
    }
    if (SOC_REG_PORT_VALID(unit, XLPORT_MODE_REGr, port)) {
        SOC_IF_ERROR_RETURN(READ_XLPORT_MODE_REGr(unit, port, &rval));
        soc_reg_field_set(unit, XLPORT_MODE_REGr, &rval,
                EGR_1588_TIMESTAMPING_CMIC_48_ENf, 1);
        soc_reg_field_set(unit, XLPORT_MODE_REGr, &rval,
                XPORT0_CORE_PORT_MODEf,core_mode_xl);
        soc_reg_field_set(unit, XLPORT_MODE_REGr, &rval,
                XPORT0_PHY_PORT_MODEf, phy_mode_xl);
        SOC_IF_ERROR_RETURN(WRITE_XLPORT_MODE_REGr(unit, port, rval));
    }
    
    return BCM_E_NONE;
}

soc_error_t soc_saber2_port_detach(int unit,uint8 block_port) {

    soc_linkscan_pause(unit);
    
    SOC_IF_ERROR_CLEAN_RETURN((soc_phyctrl_enable_set(unit, block_port, 1)),
            soc_linkscan_continue(unit));

    SOC_IF_ERROR_CLEAN_RETURN((soc_phyctrl_detach(unit, block_port)),
            soc_linkscan_continue(unit));

    PHY_FLAGS_CLR_ALL(unit, block_port);

    soc_linkscan_continue(unit);
    return BCM_E_NONE;
}

soc_error_t soc_saber2_port_attach(
        int unit, uint8 block, uint8 block_port ) {
    uint16 phy_addr=0;
    uint16 phy_addr_int=0;
    int okay = 0;
    uint64 rval64;

    soc_linkscan_pause(unit);

#ifdef BCM_METROLITE_SUPPORT
    if (SOC_IS_METROLITE(unit)){
        _ml_phy_addr_default(unit, block_port,
                             &phy_addr, &phy_addr_int);
    }else
#endif
    {
        _saber2_phy_addr_default(unit, block_port,
                                 &phy_addr, &phy_addr_int);
    }
    SOC_IF_ERROR_CLEAN_RETURN
        (soc_phy_cfg_addr_set(unit,block_port,0, phy_addr),
            soc_linkscan_continue(unit));
    SOC_IF_ERROR_CLEAN_RETURN
        (soc_phy_cfg_addr_set(unit,block_port,SOC_PHY_INTERNAL,
                              phy_addr_int), soc_linkscan_continue(unit));
    PHY_ADDR(unit, block_port)     = phy_addr;
    PHY_ADDR_INT(unit, block_port) = phy_addr_int;
    SOC_IF_ERROR_CLEAN_RETURN(_bcm_port_probe(unit, block_port, &okay),
            soc_linkscan_continue(unit));
    if (!okay) {
        soc_linkscan_continue(unit);
        return BCM_E_INTERNAL;
    }

    SOC_IF_ERROR_CLEAN_RETURN(_bcm_port_mode_setup(unit, block_port, FALSE),
            soc_linkscan_continue(unit));

    /* _bcm_port_probe() sets the encap only on MAC and not on PHY.
     * Resetting the mac encap to allow
     * bcm_esw_port_encap() function to set encap on both MAC and PHY
     */
    if (IS_HG_PORT(unit, block_port)) {
        SOC_IF_ERROR_CLEAN_RETURN(READ_XLMAC_MODEr(unit,block_port, &rval64),
                soc_linkscan_continue(unit));
        soc_reg64_field32_set(unit, XLMAC_MODEr, &rval64, HDR_MODEf, 0);
        SOC_IF_ERROR_CLEAN_RETURN(WRITE_XLMAC_MODEr(unit,block_port, rval64),
                soc_linkscan_continue(unit));
        bcm_esw_port_encap_set(unit, block_port, BCM_PORT_ENCAP_HIGIG);
    } else {
        SOC_IF_ERROR_CLEAN_RETURN(READ_XLMAC_MODEr(unit,block_port, &rval64),
                soc_linkscan_continue(unit));
        soc_reg64_field32_set(unit, XLMAC_MODEr, &rval64, HDR_MODEf, 1);
        SOC_IF_ERROR_CLEAN_RETURN(WRITE_XLMAC_MODEr(unit,block_port, rval64),
                soc_linkscan_continue(unit));
        bcm_esw_port_encap_set(unit, block_port, BCM_PORT_ENCAP_IEEE);
    }

    if (BCM_FAILURE(_soc_linkscan_hw_port_init(unit, block_port))) {
        LOG_ERROR(BSL_LS_BCM_PORT,
                (BSL_META_U(unit,
                "HW linkscan port init failed for port[%d]\n"), block_port));
    }
    soc_linkscan_continue(unit);

    return BCM_E_NONE;
}

int _bcm_sb2_port_lanes_set(int unit, bcm_port_t port, int lanes)
{
    uint32              start_addr=0;
    uint32              end_addr=0;
    soc_pbmp_t          link_pbmp;
    uint32              time_multiplier=1;
    uint64              rval64 ;
    uint32              rval = 0, rval1 = 0;
    uint32              cell_cnt = 0;
    uint32              try = 0 , try_count = 0;
    soc_timeout_t       to = {0};
    sal_usecs_t         timeout_usec = 100000;
    int                 min_polls = 100;
    uint32             pfc_enable=0;
    uint8              old_ports[][SB2_MAX_PORTS_PER_BLOCK]=
                                  {{1,0,0,0},{1,0,3,0},{0,0,0,0},{1,2,3,4}};
    uint8              new_ports[][SB2_MAX_PORTS_PER_BLOCK]=
                                  {{1,0,0,0},{1,0,3,0},{0,0,0,0},{1,2,3,4}};
    uint8              new_port_mode[5]={0,1,2,0,3};
    soc_info_t         *si = &SOC_INFO(unit);
    uint8              block = 0;
    uint8              block_port = 0, first_port = 0;
    uint8              try_loop = 0;
    uint8              loop = 0;
    int                old_lanes = 0;
    int                new_lanes;
    int                config_lanes = lanes;
    soc_field_t        port_intf_reset_fld[] = {
                       XQ1_PORT_INTF_RESETf,
                       XQ2_PORT_INTF_RESETf, XQ3_PORT_INTF_RESETf,
                       XQ4_PORT_INTF_RESETf, XQ5_PORT_INTF_RESETf,
                       XQ6_PORT_INTF_RESETf, XQ7_PORT_INTF_RESETf,
                       XQ8_PORT_INTF_RESETf, XQ9_PORT_INTF_RESETf};
    soc_field_t        mmu_intf_reset_fld[] = {
                       XQ1_MMU_INTF_RESETf,
                       XQ2_MMU_INTF_RESETf, XQ3_MMU_INTF_RESETf,
                       XQ4_MMU_INTF_RESETf, XQ5_MMU_INTF_RESETf,
                       XQ6_MMU_INTF_RESETf, XQ7_MMU_INTF_RESETf,
                       XQ8_MMU_INTF_RESETf, XQ9_MMU_INTF_RESETf};
    soc_field_t        new_port_mode_fld[] = {
                       XQ1_NEW_PORT_MODEf,
                       XQ2_NEW_PORT_MODEf, XQ3_NEW_PORT_MODEf,
                       XQ4_NEW_PORT_MODEf, XQ5_NEW_PORT_MODEf,
                       XQ6_NEW_PORT_MODEf, XQ7_NEW_PORT_MODEf,
                       XQ8_NEW_PORT_MODEf, XQ9_NEW_PORT_MODEf};
    int                port_speed = 0;
    uint8              tdm_ports[4] = {0};
    uint8              index = 0;
    uint32             pos = 0;
    uint32             egr_fifo_depth = 0;
    int                threshold;
    int                port_bw, start_threshold = 0;
    int                block_max_nxtaddr;
#if defined(SABER2_DEBUG)
    char               config_str[80];
    uint8              lane_incr[SB2_MAX_PORTS_PER_BLOCK]={4,2,0,1};
    int                auto_portgroup=0;
    int                portgroup=0;
#endif
   soc_field_t          port_enable_field[SB2_MAX_PORTS_PER_BLOCK]=
                            {PORT0f, PORT1f, PORT2f , PORT3f};
    int                init_mode = 0;
    int                port_enable = 0;
    egr_enable_entry_t egr_enable_entry = {{0}}; 
    int                block_speed=0, max_speed=0, length_check;
    uint64             xlmac_ctrl;
    epc_link_bmap_entry_t epc_link_bmap_entry={{0}};
    txlp_port_addr_map_table_entry_t txlp_port_addr_map_table_entry={{0}};
    uint64             ctr_val;

    sb2_port_lanes_unchanged = FALSE;

    if (!SOC_PORT_VALID(unit, port)) {
        return BCM_E_PORT;
    }
    if (SAL_BOOT_QUICKTURN) {
        time_multiplier=1000;
    }

    BCM_IF_ERROR_RETURN(_bcm_kt2_lanes_convention(&lanes));
    new_lanes = lanes;
    if (IS_MXQ_PORT(unit, port)) {
        if ((new_lanes != 1) && (new_lanes != 4)) {
            return BCM_E_PARAM;
        }
    } else if(IS_XL_PORT(unit, port)) {
        if ((new_lanes != 1) && (new_lanes != 2) && (new_lanes != 4)) {
            return BCM_E_PARAM;
        }
        if (eagle_4ge_mode) {
            LOG_ERROR(BSL_LS_BCM_PORT,
                    (BSL_META_U(unit,
                                "Flex operation not supported port %d cfg %d\n"),
                     port, _sb2_config_id[unit]));
            return BCM_E_PARAM;
        }
        if ((new_lanes == 4) && (eagle_n2_mode)) {
            LOG_ERROR(BSL_LS_BCM_PORT,
                    (BSL_META_U(unit,
                                "Lane 4 not supported on N2 flex unit %d: port %d cfg %d\n"),
                     unit, port, _sb2_config_id[unit]));
            return BCM_E_PARAM;
        }    
    }

    SOC_IF_ERROR_RETURN(soc_saber2_get_port_block(
                unit,port,&block));
    SOC_IF_ERROR_RETURN(_bcm_sb2_port_lanes_get(
                unit,port,&old_lanes));
    BCM_IF_ERROR_RETURN(_bcm_kt2_lanes_convention(&old_lanes));

    if(old_lanes == new_lanes) {
        sb2_port_lanes_unchanged = TRUE;
        return BCM_E_NONE;
    }

    for (loop = 0; loop < SB2_MAX_PORTS_PER_BLOCK; loop++) {
        if ((IS_LP_PORT(unit, (*sb2_block_ports[unit])[block][loop])) ||
                (IS_SUBTAG_PORT(unit, (*sb2_block_ports[unit])[block][loop]))) {
            LOG_CLI((BSL_META_U(unit,
                            "HotSwap is not supported for \
Subport Coe/LinkPhy Ports\n Disable CoE and run the API again.\n")));
            return BCM_E_PARAM;
        }
    }
    
    block_speed = 0;
    for (loop = 0;loop < SB2_MAX_PORTS_PER_BLOCK; loop++) {
        block_port=(*sb2_block_ports[unit])[block][loop];
        max_speed = bcm56260_speed[unit][_sb2_config_id[unit]][block_port - 1];
        if((IS_HG_PORT(unit, block_port)) && (max_speed == 10000)) {
            max_speed = 11000;
        }
        block_speed += max_speed;
    }
    if (block_speed < (*sb2_port_speeds[unit])[block][new_lanes-1]) {
        LOG_CLI((BSL_META_U(unit,
                        "Total BW of block %d is less than %d\n"),block_speed,
                    (*sb2_port_speeds[unit])[block][new_lanes-1]));
        return BCM_E_PARAM;
    }

    if (new_lanes == 1) {
        if(!(IS_XL_PORT(unit, port))) {
            tdm_ports[0] = (*sb2_block_ports[unit])[block][0];
            tdm_ports[1] = (*sb2_block_ports[unit])[block][0];
            tdm_ports[2] = (*sb2_block_ports[unit])[block][0];
            tdm_ports[3] = (*sb2_block_ports[unit])[block][0];
        }
    }

    if (new_lanes == 2) {
        if(!(IS_XL_PORT(unit, port))) {
            tdm_ports[0] = (*sb2_block_ports[unit])[block][0];
            tdm_ports[1] = (*sb2_block_ports[unit])[block][2];
            tdm_ports[2] = (*sb2_block_ports[unit])[block][0];
            tdm_ports[3] = (*sb2_block_ports[unit])[block][2];
        }
    }

    if (new_lanes == 4) {
        tdm_ports[0] = (*sb2_block_ports[unit])[block][0];
        tdm_ports[1] = (*sb2_block_ports[unit])[block][1];
        tdm_ports[2] = (*sb2_block_ports[unit])[block][2];
        tdm_ports[3] = (*sb2_block_ports[unit])[block][3];
    }

    sal_memcpy(&new_tdm[0],&sb2_current_tdm[0],
            sb2_current_tdm_size *sizeof(sb2_current_tdm[0]));

    if(!(IS_XL_PORT(unit, port))) {
        for (index=0,loop=0; loop < sb2_tdm_pos_info[block].total_slots; loop++) {
            pos = sb2_tdm_pos_info[block].pos[loop];
            new_tdm[pos]=tdm_ports[index];
            index = (index + 1) % 4 ;
        }
    }

    BCM_IF_ERROR_RETURN(bcm_esw_linkscan_mode_get(unit, port, &init_mode));

    for (loop = 0; loop < SB2_MAX_PORTS_PER_BLOCK; loop++) {
        block_port=(*sb2_block_ports[unit])[block][loop];
        if (SOC_PBMP_MEMBER((PBMP_ALL(unit)), block_port)) {
            /* 1. Stop linkscan on the port */
            bcm_esw_kt2_port_unlock(unit);
            BCM_IF_ERROR_RETURN(bcm_esw_linkscan_mode_set(
                        unit, block_port, BCM_LINKSCAN_MODE_NONE));
            bcm_esw_kt2_port_lock(unit);

            /* 2. To stop the incoming traffic, disable XLMAC Rx */
             SOC_IF_ERROR_RETURN(READ_XLMAC_CTRLr(
                                 unit, block_port, &xlmac_ctrl));
             soc_reg64_field32_set(unit, XLMAC_CTRLr, &xlmac_ctrl, TX_ENf, 1);
             soc_reg64_field32_set(unit, XLMAC_CTRLr, &xlmac_ctrl, RX_ENf, 0);
             SOC_IF_ERROR_RETURN(WRITE_XLMAC_CTRLr(
                                 unit, block_port, xlmac_ctrl));
             sal_udelay((10*time_multiplier));

        }
    }

    sal_udelay((10 * time_multiplier));
    for (loop = 0; loop < SB2_MAX_PORTS_PER_BLOCK; loop++) {
        block_port=(*sb2_block_ports[unit])[block][loop];
        if (SOC_PBMP_MEMBER((PBMP_ALL(unit)), block_port)) {
            /* serdes requires autoneg to be False before flex operation */
            BCM_IF_ERROR_RETURN(bcm_esw_port_autoneg_set(unit, block_port,
                        FALSE));
            /* 3. Brings the port down. */
            BCM_IF_ERROR_RETURN(READ_EPC_LINK_BMAPm(
                        unit, MEM_BLOCK_ANY, 0, &epc_link_bmap_entry));
            soc_mem_pbmp_field_get(unit, EPC_LINK_BMAPm,
                    &epc_link_bmap_entry, PORT_BITMAPf,&link_pbmp);
            BCM_PBMP_PORT_REMOVE(link_pbmp,block_port);
            soc_mem_pbmp_field_set(unit, EPC_LINK_BMAPm,
                    &epc_link_bmap_entry, PORT_BITMAPf,&link_pbmp);
            SOC_IF_ERROR_RETURN(WRITE_EPC_LINK_BMAPm(
                        unit, MEM_BLOCK_ALL, 0, &epc_link_bmap_entry));
        }
    }

    for (loop = 0; loop < SB2_MAX_PORTS_PER_BLOCK; loop++) {
        block_port=(*sb2_block_ports[unit])[block][loop];
        if (SOC_PBMP_MEMBER((PBMP_ALL(unit)), block_port)) {

#ifdef PORT_FLUSH_DEBUG
            /* 4. Issues MMU port flush command */ 
            /* PortFlushing creating some issue with L0 node movement.
             * So moving to Q Flush.
             */
            BCM_IF_ERROR_RETURN(READ_TOQ_FLUSH0r(unit, &flush_reg));
            soc_reg_field_set(unit, TOQ_FLUSH0r, &flush_reg, 
                    FLUSH_ACTIVEf,1);
            /* PORT FLUSHING not QUEUE FLUSHING */
            soc_reg_field_set(unit, TOQ_FLUSH0r, &flush_reg, FLUSH_TYPEf,1);
            soc_reg_field_set(unit, TOQ_FLUSH0r, &flush_reg, FLUSH_NUMf,1);
            soc_reg_field_set(unit, TOQ_FLUSH0r, &flush_reg, 
                    FLUSH_ID0f, block_port);
            BCM_IF_ERROR_RETURN(WRITE_TOQ_FLUSH0r(unit, flush_reg));
#endif


            /* 5. Save the current configuration of PAUSE/PFC flow control */ 
            BCM_IF_ERROR_RETURN(bcm_esw_port_speed_get(
                        unit, block_port, &port_speed));
            if (!(SAL_BOOT_BCMSIM || SAL_BOOT_PLISIM)) { 
                if (port_speed == 10000) {
                    BCM_IF_ERROR_RETURN(READ_XPORT_TO_MMU_BKPr(
                                unit, block, &pfc_enable));
                    BCM_IF_ERROR_RETURN(WRITE_XPORT_TO_MMU_BKPr(
                                unit, block, 0));
                }
             }
           
            /* 6. flushes any pending egress packets in MXQPORT */
            if (SOC_REG_PORT_VALID(unit, IECELL_TXFIFO_PKT_DROP_CTLr, block_port)) {
                READ_IECELL_TXFIFO_PKT_DROP_CTLr(unit, block_port, &rval);
                soc_reg_field_set(unit, IECELL_TXFIFO_PKT_DROP_CTLr, &rval, 
                        DROP_ENf, 1);
                WRITE_IECELL_TXFIFO_PKT_DROP_CTLr(unit, block_port, rval);
            }

            if (SOC_REG_PORT_VALID(unit, XP_TXFIFO_PKT_DROP_CTLr, block_port)) {
                READ_XP_TXFIFO_PKT_DROP_CTLr(unit, block_port, &rval);
                soc_reg_field_set(unit, XP_TXFIFO_PKT_DROP_CTLr, &rval, 
                        DROP_ENf, 1);
                WRITE_XP_TXFIFO_PKT_DROP_CTLr(unit, block_port, rval);
            }

#ifndef PORT_FLUSH_DEBUG
            BCM_IF_ERROR_RETURN(bcm_kt2_cosq_port_flush(
                        unit, block_port, bcmCosqFlushTypeQueue));
#endif

#ifdef PORT_FLUSH_DEBUG
            /* 7. Waits until all egress port packets are drained. */
            try_count=0;
            if ((SAL_BOOT_BCMSIM || SAL_BOOT_PLISIM)) {
                BCM_IF_ERROR_RETURN(READ_TOQ_FLUSH0r(unit, &rval));
                soc_reg_field_set(unit, TOQ_FLUSH0r, &rval, FLUSH_ACTIVEf, 0);
                BCM_IF_ERROR_RETURN(WRITE_TOQ_FLUSH0r(unit, rval));
            }

            do {
                soc_timeout_init(&to, timeout_usec, min_polls);
                if(soc_timeout_check(&to)) {
                    LOG_ERROR(BSL_LS_BCM_PORT,
                            (BSL_META_U(unit,
                                        "%s:%d:TimeOut InternalError\n"),
                             FUNCTION_NAME(),__LINE__));
                    return BCM_E_INTERNAL;
                }
                try_count++;
                BCM_IF_ERROR_RETURN(READ_TOQ_FLUSH0r(unit, &rval));
                if (soc_reg_field_get(
                            unit, TOQ_FLUSH0r, rval, FLUSH_ACTIVEf) == 0) {
                    break;
                }
            } while (try_count != 3);

            if (try_count == 3) {
                LOG_ERROR(BSL_LS_BCM_PORT,
                        (BSL_META_U(unit,
                                    "%s:%d: TimeOut - TOQ Flush\n"),
                         FUNCTION_NAME(),__LINE__));
                return BCM_E_TIMEOUT;
            }
#endif

            try_count=0;
            if ((SAL_BOOT_BCMSIM || SAL_BOOT_PLISIM)) {
                if (SOC_REG_PORT_VALID(unit, XP_TXFIFO_CELL_CNTr, block_port)) {
                    BCM_IF_ERROR_RETURN(READ_XP_TXFIFO_CELL_CNTr(
                                unit, block_port, &rval));
                    soc_reg_field_set(unit, XP_TXFIFO_CELL_CNTr,
                            &rval, CELL_CNTf, 0);
                    BCM_IF_ERROR_RETURN(WRITE_XP_TXFIFO_CELL_CNTr(
                                unit, block_port, rval));
                }

                if (SOC_REG_PORT_VALID(unit, IECELL_TXFIFO_CELL_CNTr, block_port)) {
                    BCM_IF_ERROR_RETURN(READ_IECELL_TXFIFO_CELL_CNTr(
                                unit, block_port, &rval));
                    soc_reg_field_set(unit, IECELL_TXFIFO_CELL_CNTr,
                            &rval, CELL_CNTf, 0); 
                    BCM_IF_ERROR_RETURN(WRITE_IECELL_TXFIFO_CELL_CNTr(
                                unit, block_port, rval));
                }   
 
            }
            soc_timeout_init(&to, timeout_usec, min_polls);
            for (try=0; try<100 && try_count < 10 ; try++) {
                if(soc_timeout_check(&to)) {
                    LOG_ERROR(BSL_LS_BCM_PORT,
                            (BSL_META_U(unit,
                                        "%s:%d:TimeOut InternalError\n"),
                             FUNCTION_NAME(),__LINE__));
                    return BCM_E_INTERNAL;
                }

                BCM_IF_ERROR_RETURN(READ_XP_TXFIFO_CELL_CNTr (
                            unit,block_port, &rval));
                cell_cnt=soc_reg_field_get(unit, XP_TXFIFO_CELL_CNTr,     
                        rval, CELL_CNTf);
                if (cell_cnt == 0) {
                    try_count++;
                    break;     
                }
            }

            if (try == 100) {
                LOG_ERROR(BSL_LS_BCM_PORT,
                        (BSL_META_U(unit,
                                    "%s:%d: TimeOut - Drain Egress packets\n"),
                         FUNCTION_NAME(),__LINE__));
                return BCM_E_TIMEOUT;
            }
        }
    }

    /* Disable port before detaching phy */
    for (loop = 0; loop < SB2_MAX_PORTS_PER_BLOCK; loop++) {
        block_port=(*sb2_block_ports[unit])[block][loop];
        if (SOC_PBMP_MEMBER((PBMP_ALL(unit)), block_port)) {
            BCM_IF_ERROR_RETURN(
                    bcm_esw_port_enable_set(unit, block_port, 0));
        }
    }

    for (loop = 0; loop < SB2_MAX_PORTS_PER_BLOCK; loop++) {
        block_port=(*sb2_block_ports[unit])[block][loop];
        if (SOC_PBMP_MEMBER((PBMP_ALL(unit)), block_port)) {
        
            /* 8. Powerdown Vipercore/Eaglecore serdes */
            if (SOC_REG_PORT_VALID(unit, XPORT_XGXS_CTRLr, block_port)) {
                SOC_IF_ERROR_RETURN(READ_XPORT_XGXS_CTRLr(unit, block_port, &rval));
                soc_reg_field_set(unit, XPORT_XGXS_CTRLr, &rval,
                        IDDQf,1);
                soc_reg_field_set(unit, XPORT_XGXS_CTRLr, &rval,
                        PWRDWNf,1);
                WRITE_XPORT_XGXS_CTRLr(unit, block_port,rval);
            }


            if (SOC_REG_PORT_VALID(unit, XLPORT_XGXS0_CTRL_REGr, block_port)) {
                SOC_IF_ERROR_RETURN(READ_XLPORT_XGXS0_CTRL_REGr(unit, 
                            block_port, &rval));
                soc_reg_field_set(unit, XLPORT_XGXS0_CTRL_REGr, &rval, 
                        PWRDWNf, 1);
                SOC_IF_ERROR_RETURN(WRITE_XLPORT_XGXS0_CTRL_REGr(unit, 
                            block_port, rval));
            }

            /* 9. Disable egress cell request generation. */
            if(block > 0) {
                sal_memset(&egr_enable_entry, 0, sizeof(egr_enable_entry_t));
                soc_mem_field32_set(unit, EGR_ENABLEm, &egr_enable_entry, PRT_ENABLEf, 0);
                SOC_IF_ERROR_RETURN(WRITE_EGR_ENABLEm(unit, MEM_BLOCK_ALL, block_port,
                            &egr_enable_entry));
            } else {
                SOC_IF_ERROR_RETURN(READ_TXLP_PORT_ENABLEr(unit, port,
                            &rval));
                port_enable = soc_reg_field_get(unit, TXLP_PORT_ENABLEr, rval,
                        PORT_ENABLEf);
                port_enable &= ~(1 << (loop));
                soc_reg_field_set(unit, TXLP_PORT_ENABLEr, &rval,
                        PORT_ENABLEf, port_enable);
                BCM_IF_ERROR_RETURN(WRITE_TXLP_PORT_ENABLEr(unit, port, rval));
            }

            /* 10. disable the MXQPORT flush. */
            if (SOC_REG_PORT_VALID(unit, XP_TXFIFO_PKT_DROP_CTLr, block_port)) {
                READ_XP_TXFIFO_PKT_DROP_CTLr(unit, block_port,&rval);
                soc_reg_field_set(unit, XP_TXFIFO_PKT_DROP_CTLr, &rval, 
                        DROP_ENf,0);
                WRITE_XP_TXFIFO_PKT_DROP_CTLr(unit, block_port,rval);
            }
            
            if (SOC_REG_PORT_VALID(unit, IECELL_TXFIFO_PKT_DROP_CTLr, block_port)) {
                READ_IECELL_TXFIFO_PKT_DROP_CTLr(unit, block_port,&rval);
                soc_reg_field_set(unit, IECELL_TXFIFO_PKT_DROP_CTLr, &rval, 
                        DROP_ENf,0);
                WRITE_IECELL_TXFIFO_PKT_DROP_CTLr(unit, block_port,rval);
            }
            /* 11. Restore PFC */
            if (!(SAL_BOOT_BCMSIM || SAL_BOOT_PLISIM)) {
                if (port_speed >= 10000) {
                    BCM_IF_ERROR_RETURN(WRITE_XPORT_TO_MMU_BKPr(
                                unit, block, pfc_enable));
                }
            }
            /* PLEASE NOTE THAT PORT IS STILL DOWN */

        } /* If SOC_PBMP_MEMBER */
    } /* for loop */
    
    /* Update local pbmp's */
    soc_linkscan_pause(unit);
    for (loop = 0; loop < SB2_MAX_PORTS_PER_BLOCK; loop++) {
        block_port=(*sb2_block_ports[unit])[block][loop];
        if ((old_ports[old_lanes-1][loop] == 0) &&
                (new_ports[new_lanes-1][loop] == 0)) {
            /* Either not used  */
            continue;
        }

        if (block < 6) {
            si->port_speed_max[block_port]=(*sb2_port_speeds[unit])[block]
                [new_lanes-1];
        } else {
            /* Add exception for eagle port due to mixed mode */
            soc_saber2_xl_port_speed_get(unit, block_port,
                    &(si->port_speed_max[block_port]));
        }

        if (old_ports[old_lanes-1][loop] == new_ports[new_lanes-1][loop] ) {
            /* same port was used so only speed change */
            soc_katana2_pbmp_remove(unit,block_port);
            soc_saber2_pbmp_add(unit,block_port,block,
                    si->port_speed_max[block_port], new_lanes);
        }
        if (old_ports[old_lanes-1][loop]  > new_ports[new_lanes-1][loop]) {
            /* This port is not applicable so remove it from pbmp list */
            soc_katana2_pbmp_remove(unit,block_port);
#if defined(INCLUDE_L3)
            bcm_sb2_ipmc_repl_extq_port_free(unit, block_port);
#endif
            continue;
        }
        if (new_ports[new_lanes-1][loop]  > old_ports[old_lanes-1][loop]) {
            /* This port is new so add it in pbmp list first */
            soc_saber2_pbmp_add(unit,block_port, block,
                    si->port_speed_max[block_port], new_lanes);
#if defined(INCLUDE_L3)
            BCM_IF_ERROR_RETURN(bcm_sb2_ipmc_repl_extq_port_alloc(unit, block_port));
#endif
        }
        BCM_IF_ERROR_RETURN(bcm_katana2_cosq_reconfigure_flexio(unit, block_port, config_lanes));
    }

    soc_katana2_pbmp_all_resync(unit) ;
    soc_esw_dport_init(unit);
    soc_linkscan_continue(unit);
    SOC_IF_ERROR_RETURN(_soc_saber2_mmu_reconfigure(unit)); 

    /* ReConfigure H/W */
    /* 1. Soft-reset relevant MXQPORT(s) */
    if (block < 6) { 
        soc_saber2_block_reset(unit, block, 0);
    }

    /* ReConfigure TDM */
    soc_saber2_reconfigure_tdm(unit, sb2_current_tdm_size, new_tdm); 
    sal_memcpy(&sb2_current_tdm[0],&new_tdm[0],
            sb2_current_tdm_size *sizeof(sb2_current_tdm[0]));

    block_max_nxtaddr = block_max_startaddr[block];
    for (loop = 0; loop < SB2_MAX_PORTS_PER_BLOCK; loop++) {
        block_port=(*sb2_block_ports[unit])[block][loop];
        if (SOC_PBMP_MEMBER((PBMP_ALL(unit)), block_port)) {
            BCM_IF_ERROR_RETURN(READ_DEQ_EFIFO_CFGr(unit,block_port,&rval));

            if (si->port_speed_max[block_port] <= 1000) {
                egr_fifo_depth = (IS_EXT_MEM_PORT(unit, block_port)) ? 10 : 2;
                threshold = (IS_EXT_MEM_PORT(unit, block_port)) ? 8 : 1;
                port_bw = 25;
                start_threshold = 7;
            } else if (si->port_speed_max[block_port] <= 2500) {
                egr_fifo_depth = (IS_EXT_MEM_PORT(unit, block_port)) ? 19 : 2;
                threshold = (IS_EXT_MEM_PORT(unit, block_port)) ? 15 : 1;
                port_bw = 63;
                start_threshold = 14;
            } else if (si->port_speed_max[block_port] <= 13000) {
                egr_fifo_depth = (IS_EXT_MEM_PORT(unit, block_port)) ? 53 : 10;
                threshold = (IS_EXT_MEM_PORT(unit, block_port)) ? 49 : 1;
                port_bw = 325;
                start_threshold = 48;
            } else {
                LOG_ERROR(BSL_LS_BCM_PORT,
                        (BSL_META_U(unit, "Invalid Speed config\n")));
                return BCM_E_FAIL;
            }

            soc_reg_field_set(unit, DEQ_EFIFO_CFGr, &rval,
                    EGRESS_FIFO_START_ADDRESSf,
                    block_max_nxtaddr);

            if ((block_max_nxtaddr + egr_fifo_depth) > block_max_endaddr[block]) {
                egr_fifo_depth = (block_max_endaddr[block] -
                        block_max_nxtaddr)+1;
                block_max_nxtaddr = block_max_endaddr[block];
            } else {
                block_max_nxtaddr += egr_fifo_depth;
            }
            if (egr_fifo_depth == 1) {
                LOG_CLI((BSL_META_U(unit,
                                "WARN: EGR_FIFO_DEPTH IS ZERO for port=%d\n"),
                            block_port));
            }
            soc_reg_field_set(unit, DEQ_EFIFO_CFGr, &rval,
                    EGRESS_FIFO_DEPTHf, egr_fifo_depth);
            soc_reg_field_set(unit, DEQ_EFIFO_CFGr, &rval,
                    EGRESS_FIFO_XMIT_THRESHOLDf, threshold);
            BCM_IF_ERROR_RETURN(WRITE_DEQ_EFIFO_CFGr(
                        unit,block_port,rval));

            /* Port BW Ctrl */
            SOC_IF_ERROR_RETURN(READ_TOQ_PORT_BW_CTRLr(unit,
                        block_port, &rval));
            soc_reg_field_set(unit, TOQ_PORT_BW_CTRLr, &rval,
                    PORT_BWf, port_bw);
            soc_reg_field_set(unit, TOQ_PORT_BW_CTRLr, &rval,
                    START_THRESHOLDf, start_threshold);
            SOC_IF_ERROR_RETURN(WRITE_TOQ_PORT_BW_CTRLr(unit,
                        block_port, rval));

            LOG_VERBOSE(BSL_LS_SOC_COMMON, (BSL_META_U(unit,
                "DEQ_EFIFO_CFG values: "
                "port:%d depth:%d new_block_max_nxtaddr=%d\n"),
                port, egr_fifo_depth, block_max_nxtaddr));
        } /* if SOC_PBMP_MEMBER  ... block_port */
    } /* for loop */

    /* WORK-AROUND for port-flushing i.e. need to repeat below step twice */
    for (try_loop = 0; try_loop < 2; try_loop++) { 
        /* EP Reset */
        if ((SAL_BOOT_BCMSIM || SAL_BOOT_PLISIM)) {
            BCM_IF_ERROR_RETURN(WRITE_DYNAMIC_PORT_RECFG_VECTOR_CFG_29_00r(
                        unit, 0));
        }

        for (loop = 0; loop < SB2_MAX_PORTS_PER_BLOCK; loop++) {
            block_port=(*sb2_block_ports[unit])[block][loop];
            if (SOC_PBMP_MEMBER((PBMP_ALL(unit)), block_port)) {
                rval1 |= (1 << block_port);
            }
        }

        BCM_IF_ERROR_RETURN(WRITE_DYNAMIC_PORT_RECFG_VECTOR_CFG_29_00r(
                    unit, rval1));

        if ((SAL_BOOT_BCMSIM || SAL_BOOT_PLISIM)) {
            BCM_IF_ERROR_RETURN(WRITE_DYNAMIC_PORT_RECFG_VECTOR_CFG_29_00r(
                        unit, 0));
        }

        try_count = 0;
        do {
            soc_timeout_init(&to, timeout_usec, min_polls);
            if(soc_timeout_check(&to)) {
                LOG_ERROR(BSL_LS_BCM_PORT,
                        (BSL_META_U(unit,
                                    "%s:%d:TimeOut InternalError\n"),
                         FUNCTION_NAME(),__LINE__));
                return BCM_E_INTERNAL;
            }
            try_count++;

            BCM_IF_ERROR_RETURN(READ_DYNAMIC_PORT_RECFG_VECTOR_CFG_29_00r(
                        unit, &rval1));
            if (rval1 == 0 ) {
                break;
            }
        } while (try_count != 5);

        if (try_count == 5) {
                LOG_ERROR(BSL_LS_BCM_PORT,
                        (BSL_META_U(unit,
                                    "%s:%d: TimeOut - Port reconfig vector\n"),
                         FUNCTION_NAME(),__LINE__));
            return BCM_E_TIMEOUT;
        }

        sal_usleep(1000);
    } /* for try_loop */

    if ((block > 0) && (block < 6)) {
        READ_EDATABUF_DBG_PORT_INTF_RESETr(unit, &rval);
        soc_reg_field_set(unit, EDATABUF_DBG_PORT_INTF_RESETr,
                &rval,  port_intf_reset_fld[block - 1], 1);
        WRITE_EDATABUF_DBG_PORT_INTF_RESETr(unit, rval);
        READ_EDATABUF_DBG_MMU_INTF_RESETr(unit, &rval);
        soc_reg_field_set(unit, EDATABUF_DBG_MMU_INTF_RESETr,
                &rval,  mmu_intf_reset_fld[block - 1], 1);
        soc_reg_field_set(unit, EDATABUF_DBG_MMU_INTF_RESETr,
                &rval,  new_port_mode_fld[block - 1], 
                new_port_mode[new_lanes]);
        WRITE_EDATABUF_DBG_MMU_INTF_RESETr(unit, rval);
    } else if (block == 6) {
        for (loop = 0; loop < SB2_MAX_PORTS_PER_BLOCK; loop++) {
            READ_EDATABUF_DBG_PORT_INTF_RESETr(unit, &rval);
            soc_reg_field_set(unit, EDATABUF_DBG_PORT_INTF_RESETr,
                    &rval,  port_intf_reset_fld[block + loop - 1], 1);
            WRITE_EDATABUF_DBG_PORT_INTF_RESETr(unit, rval);
            READ_EDATABUF_DBG_MMU_INTF_RESETr(unit, &rval);
            soc_reg_field_set(unit, EDATABUF_DBG_MMU_INTF_RESETr,
                    &rval,  mmu_intf_reset_fld[block + loop - 1], 1);
            soc_reg_field_set(unit, EDATABUF_DBG_MMU_INTF_RESETr,
                    &rval,  new_port_mode_fld[block + loop - 1],
                    new_port_mode[new_lanes]);
            WRITE_EDATABUF_DBG_MMU_INTF_RESETr(unit, rval);
        }
    } else if (block == 0){
        WRITE_TXLP_PORT_CREDIT_RESETr(unit, port,0xf);
        for (loop = 0; loop < SB2_MAX_PORTS_PER_BLOCK; loop++) {
            sal_memset(&txlp_port_addr_map_table_entry,0,
                    sizeof(txlp_port_addr_map_table_entry_t));
            block_port=(*sb2_block_ports[unit])[block][loop];
            if (SOC_PBMP_MEMBER((PBMP_ALL(unit)), block_port)) {
                
                if (si->port_speed_max[port] <= 2500) {
                    end_addr = start_addr + (( 8 * 4) - 1); /* 8 cells */
                } else if(si->port_speed_max[port] <= 10000) {
                    end_addr = start_addr + (( 16 * 4) - 1);
                } else  {
                    return BCM_E_PARAM;
                }

                soc_TXLP_PORT_ADDR_MAP_TABLEm_field_set(unit,
                        &txlp_port_addr_map_table_entry,START_ADDRf,&start_addr);
                soc_TXLP_PORT_ADDR_MAP_TABLEm_field_set(unit,
                        &txlp_port_addr_map_table_entry,END_ADDRf,&end_addr);
                start_addr = end_addr+1;
            }

            BCM_IF_ERROR_RETURN(WRITE_TXLP_PORT_ADDR_MAP_TABLEm(
                        unit,SOC_INFO(unit).txlp_block[block],
                        loop, &txlp_port_addr_map_table_entry));
        } /* for loop */
    } /* if else block != 0 */


    /* Begin: Link up sequence */
    sal_udelay((10*time_multiplier));
    
    /* 1. Bring edatabuf port and mmu intf out of reset */
    if ((block > 0) && (block < 6)) {
        READ_EDATABUF_DBG_PORT_INTF_RESETr(unit, &rval);
        soc_reg_field_set(unit, EDATABUF_DBG_PORT_INTF_RESETr,
                &rval,  port_intf_reset_fld[block - 1], 0);
        WRITE_EDATABUF_DBG_PORT_INTF_RESETr(unit, rval);
        READ_EDATABUF_DBG_MMU_INTF_RESETr(unit, &rval);
        soc_reg_field_set(unit, EDATABUF_DBG_MMU_INTF_RESETr,
                &rval,  mmu_intf_reset_fld[block - 1], 0);
        WRITE_EDATABUF_DBG_MMU_INTF_RESETr(unit, rval);
    } else if(block == 6) {
        for (loop = 0; loop < SB2_MAX_PORTS_PER_BLOCK; loop++) {
            READ_EDATABUF_DBG_PORT_INTF_RESETr(unit, &rval);
            soc_reg_field_set(unit, EDATABUF_DBG_PORT_INTF_RESETr,
                    &rval,  port_intf_reset_fld[block + loop - 1], 0);
            WRITE_EDATABUF_DBG_PORT_INTF_RESETr(unit, rval);
            READ_EDATABUF_DBG_MMU_INTF_RESETr(unit, &rval);
            soc_reg_field_set(unit, EDATABUF_DBG_MMU_INTF_RESETr,
                    &rval,  mmu_intf_reset_fld[block + loop - 1], 0);
            WRITE_EDATABUF_DBG_MMU_INTF_RESETr(unit, rval);
        }
    }

    if (block == 0) {
        WRITE_TXLP_PORT_CREDIT_RESETr(unit, port,0);
    }

    /* 2. Enable port */
    BCM_IF_ERROR_RETURN(READ_EPC_LINK_BMAPm(
                unit, MEM_BLOCK_ANY, 0, &epc_link_bmap_entry));
    soc_mem_pbmp_field_get(unit, EPC_LINK_BMAPm,
            &epc_link_bmap_entry, PORT_BITMAPf,&link_pbmp);
    for (loop = 0; loop < SB2_MAX_PORTS_PER_BLOCK; loop++) {
        block_port=(*sb2_block_ports[unit])[block][loop];
        if (SOC_PBMP_MEMBER((PBMP_ALL(unit)), block_port)) {
            BCM_PBMP_PORT_ADD(link_pbmp,block_port);
        }
    } 
    soc_mem_pbmp_field_set(unit, EPC_LINK_BMAPm,
            &epc_link_bmap_entry, PORT_BITMAPf,&link_pbmp);
    SOC_IF_ERROR_RETURN(WRITE_EPC_LINK_BMAPm(
                unit, MEM_BLOCK_ALL, 0, &epc_link_bmap_entry));
    
    sal_udelay((10*time_multiplier));

    /* Bring out of reset */
    if (block < 6) {
        soc_saber2_block_reset(unit, block, 1);
    }

    first_port = (*sb2_block_ports[unit])[block][0];
    if (SOC_REG_PORT_VALID(unit, XLPORT_XGXS0_CTRL_REGr, first_port)) {
        SOC_IF_ERROR_RETURN(READ_XLPORT_XGXS0_CTRL_REGr(unit, first_port, &rval));

        soc_reg_field_set(unit, XLPORT_XGXS0_CTRL_REGr, &rval, PWRDWNf, 0);
        SOC_IF_ERROR_RETURN(WRITE_XLPORT_XGXS0_CTRL_REGr(unit, first_port, rval));
        
        soc_reg_field_set(unit, XLPORT_XGXS0_CTRL_REGr, &rval, IDDQf, 0);
        SOC_IF_ERROR_RETURN(WRITE_XLPORT_XGXS0_CTRL_REGr(unit, first_port, rval));

        soc_reg_field_set(unit, XLPORT_XGXS0_CTRL_REGr, &rval, RSTB_HWf, 1);
        SOC_IF_ERROR_RETURN(WRITE_XLPORT_XGXS0_CTRL_REGr(unit, first_port, rval));

    } else if (SOC_REG_PORT_VALID(unit, XPORT_XGXS_CTRLr, first_port)) {
        SOC_IF_ERROR_RETURN(READ_XPORT_XGXS_CTRLr(unit, first_port, &rval));
        
        soc_reg_field_set(unit, XPORT_XGXS_CTRLr, &rval, PWRDWNf, 0);
        SOC_IF_ERROR_RETURN(WRITE_XPORT_XGXS_CTRLr(unit, first_port, rval));

        soc_reg_field_set(unit, XPORT_XGXS_CTRLr, &rval, IDDQf, 0);
        SOC_IF_ERROR_RETURN(WRITE_XPORT_XGXS_CTRLr(unit, first_port, rval));
        
        soc_reg_field_set(unit, XPORT_XGXS_CTRLr, &rval, RSTB_HWf, 1);
        SOC_IF_ERROR_RETURN(WRITE_XPORT_XGXS_CTRLr(unit, first_port, rval));
  
        soc_reg_field_set(unit, XPORT_XGXS_CTRLr, &rval, RSTB_MDIOREGSf, 1);
        SOC_IF_ERROR_RETURN(WRITE_XPORT_XGXS_CTRLr(unit, first_port, rval));
        
        soc_reg_field_set(unit, XPORT_XGXS_CTRLr, &rval, RSTB_PLLf, 1);
        SOC_IF_ERROR_RETURN(WRITE_XPORT_XGXS_CTRLr(unit, first_port, rval));

        for (loop=0;loop<SB2_MAX_PORTS_PER_BLOCK;loop++) {
             block_port=(*sb2_block_ports[unit])[block][loop];
             if (SOC_PBMP_MEMBER((PBMP_ALL(unit)), block_port)) {
                 SOC_IF_ERROR_RETURN(READ_XPORT_XGXS_CTRLr(
                                     unit, block_port, &rval));
                 if (si->port_speed_max[block_port] > 2500) {
                     soc_reg_field_set(unit, XPORT_XGXS_CTRLr, &rval,
                                       TXD10G_FIFO_RSTBf, 1);
                 } else {
                     soc_reg_field_set(unit, XPORT_XGXS_CTRLr, &rval,
                                       TXD10G_FIFO_RSTBf,0);
                 }
                 soc_reg_field_set(unit,XPORT_XGXS_CTRLr, &rval,
                                   TXD1G_FIFO_RSTBf, 0xF);
                 SOC_IF_ERROR_RETURN(WRITE_XPORT_XGXS_CTRLr(
                                     unit,block_port, rval));
             }
        }
    }

    if (block == 6) {
        for (loop = 0; loop < SB2_MAX_PORTS_PER_BLOCK; loop++) {
            block_port=(*sb2_block_ports[unit])[block][loop];
            if (SOC_PBMP_MEMBER((PBMP_ALL(unit)), block_port)) {
                if(SOC_REG_PORT_VALID(unit, XLPORT_MAC_CONTROLr, block_port)) {
                    SOC_IF_ERROR_RETURN(READ_XLPORT_MAC_CONTROLr(unit, block_port, &rval));

                    soc_reg_field_set(unit, XLPORT_MAC_CONTROLr,  &rval, XMAC0_RESETf, 1);
                    SOC_IF_ERROR_RETURN(WRITE_XLPORT_MAC_CONTROLr(unit, block_port, rval));

                    sal_usleep(1000);
                    soc_reg_field_set(unit, XLPORT_MAC_CONTROLr,  &rval, XMAC0_RESETf, 0);
                    SOC_IF_ERROR_RETURN(WRITE_XLPORT_MAC_CONTROLr(unit, block_port, rval));
                }
            }
        }

        for (loop = 0; loop < SB2_MAX_PORTS_PER_BLOCK; loop++) {
            block_port=(*sb2_block_ports[unit])[block][loop];
            if (SOC_PBMP_MEMBER((PBMP_ALL(unit)), block_port)) {
                if (SOC_REG_PORT_VALID(unit, IECELL_CONFIGr, block_port)) {
                    SOC_IF_ERROR_RETURN(READ_IECELL_CONFIGr(unit, block_port, &rval));

                    soc_reg_field_set(unit, IECELL_CONFIGr, &rval, SOFT_RESETf, 1);
                    SOC_IF_ERROR_RETURN(WRITE_IECELL_CONFIGr(unit, block_port, rval));
 
                    sal_usleep(1000);
                    soc_reg_field_set(unit, IECELL_CONFIGr, &rval, SOFT_RESETf, 0);
                    SOC_IF_ERROR_RETURN(WRITE_IECELL_CONFIGr(unit, block_port, rval));
                }
            }
        }
    }

    for (loop = 0; loop < SB2_MAX_PORTS_PER_BLOCK; loop++) {
        block_port=(*sb2_block_ports[unit])[block][loop];
        if (SOC_PBMP_MEMBER((PBMP_ALL(unit)), block_port)) {
            BCM_IF_ERROR_RETURN(_bcm_sb2_update_port_mode(
                        unit,block_port,
                        si->port_speed_max[block_port]));
            break;
        }
    }

    for (loop = 0; loop < SB2_MAX_PORTS_PER_BLOCK; loop++) {
        block_port=(*sb2_block_ports[unit])[block][loop];
        if (SOC_PBMP_MEMBER((PBMP_ALL(unit)), block_port)) {
            if(SOC_REG_PORT_VALID(unit, XPORT_XMAC_CONTROLr, block_port)) {
                SOC_IF_ERROR_RETURN(READ_XPORT_XMAC_CONTROLr(unit, block_port, &rval));
                soc_reg_field_set(unit, XPORT_XMAC_CONTROLr, &rval, XMAC_RESETf, 1);
                SOC_IF_ERROR_RETURN(WRITE_XPORT_XMAC_CONTROLr(unit, block_port, rval));
            }
        }
    }
    sal_udelay(10);

    for (loop = 0; loop < SB2_MAX_PORTS_PER_BLOCK; loop++) {
        block_port=(*sb2_block_ports[unit])[block][loop];
        if (SOC_PBMP_MEMBER((PBMP_ALL(unit)), block_port)) {
            if(SOC_REG_PORT_VALID(unit, XPORT_XMAC_CONTROLr, block_port)) {
                SOC_IF_ERROR_RETURN(READ_XPORT_XMAC_CONTROLr(unit, block_port, &rval));
                soc_reg_field_set(unit, XPORT_XMAC_CONTROLr, &rval, XMAC_RESETf, 0);
                SOC_IF_ERROR_RETURN(WRITE_XPORT_XMAC_CONTROLr(unit, block_port, rval));
            }
        }
    }

    length_check = soc_property_get(unit, spn_MAC_LENGTH_CHECK_ENABLE, 0); 
    
    for (loop = 0; loop < SB2_MAX_PORTS_PER_BLOCK; loop++) {
        block_port=(*sb2_block_ports[unit])[block][loop];
        if (SOC_PBMP_MEMBER((PBMP_ALL(unit)), block_port)) {
            if(SOC_REG_PORT_VALID(unit, MAC_RSV_MASKr, block_port)) {
                SOC_IF_ERROR_RETURN(READ_MAC_RSV_MASKr(unit, block_port, &rval));
                
                if (length_check) {
                    rval |= 0x20;  /* set bit 5 to enable frame length check */
                } else {
                    rval &= ~0x20; /* clear bit 5 to disable frame length check */
                }
                SOC_IF_ERROR_RETURN(WRITE_MAC_RSV_MASKr(unit,block_port, rval));
            } /* SOC_REG_PORT_VALID MAC_RSV_MASK */

            if(SOC_REG_PORT_VALID(unit, XLPORT_MAC_RSV_MASKr, block_port)) {
                SOC_IF_ERROR_RETURN(READ_XLPORT_MAC_RSV_MASKr(unit,
                                     block_port, &rval));
                
                if (IS_ST_PORT(unit, block_port)) {
                    /* No frame length for stack */
                    soc_reg_field_set(unit, XLPORT_MAC_RSV_MASKr,
                            &rval, MASKf, 0x58);
                } else {
                    if (length_check) {
                        rval |= 0x20;  /* set bit 5 to enable frame length check */
                    } else {
                        rval &= ~0x20; /* clear bit 5 to disable frame length check */
                    }
                }
                SOC_IF_ERROR_RETURN(WRITE_XLPORT_MAC_RSV_MASKr(unit,
                            block_port, rval));
            } /* SOC_REG_PORT_VALID MAC_RSV_MASK */
        } /* if SOC_PBMP_MEMBER */
    } /* for loop */

    for (loop = 0; loop < SB2_MAX_PORTS_PER_BLOCK; loop++) {
        block_port=(*sb2_block_ports[unit])[block][loop];
        if (SOC_PBMP_MEMBER((PBMP_ALL(unit)), block_port)) {

            if (SOC_REG_PORT_VALID(unit, XPORT_MIB_RESETr, block_port)) {
                SOC_IF_ERROR_RETURN(WRITE_XPORT_MIB_RESETr(unit, block_port, 0xf));
                sal_usleep(1000);
                SOC_IF_ERROR_RETURN(WRITE_XPORT_MIB_RESETr(unit, block_port, 0x0));
            }

            if (SOC_REG_PORT_VALID(unit, XLPORT_MIB_RESETr, block_port)) {
                SOC_IF_ERROR_RETURN(WRITE_XLPORT_MIB_RESETr(unit, block_port, 0xf));
                sal_usleep(1000);
                SOC_IF_ERROR_RETURN(WRITE_XLPORT_MIB_RESETr(unit, block_port, 0x0));
            }
        }
    }


    for (loop = 0; loop < SB2_MAX_PORTS_PER_BLOCK; loop++) {
        block_port=(*sb2_block_ports[unit])[block][loop];
        if (SOC_PBMP_MEMBER((PBMP_ALL(unit)), block_port)) {
            if (SOC_REG_PORT_VALID(unit, XLMAC_RX_CTRLr, block_port)) {
                SOC_IF_ERROR_RETURN(READ_XLMAC_RX_CTRLr(
                            unit,block_port, &rval64));
                if (IS_ST_PORT(unit, block_port)) {
                    soc_reg64_field32_set(unit, XLMAC_RX_CTRLr, &rval64,
                            RUNT_THRESHOLDf, 76);
                } else {
                    soc_reg64_field32_set(unit, XLMAC_RX_CTRLr, &rval64,
                            RUNT_THRESHOLDf, 64);
                }
                soc_reg64_field32_set(unit, XLMAC_RX_CTRLr, &rval64,
                        STRIP_CRCf, 0);
                SOC_IF_ERROR_RETURN(WRITE_XLMAC_RX_CTRLr(
                            unit,block_port, rval64));
            } /* if SOC_REG_PORT_VALID XLMAC_RX_CTRL */
        } /* if SOC_PBMP_MEMBER block_port */
    } /* for loop */

    for (loop = 0; loop < SB2_MAX_PORTS_PER_BLOCK; loop++) {
        block_port=(*sb2_block_ports[unit])[block][loop];
        if (SOC_PBMP_MEMBER((PBMP_ALL(unit)), block_port)) {
            if (SOC_REG_PORT_VALID(unit, XLMAC_TX_CTRLr, block_port)) {
                SOC_IF_ERROR_RETURN(READ_XLMAC_TX_CTRLr(
                        unit, block_port, &rval64));
                soc_reg64_field32_set(unit, XLMAC_TX_CTRLr, &rval64,
                        CRC_MODEf,3);
                SOC_IF_ERROR_RETURN(WRITE_XLMAC_TX_CTRLr(
                        unit,block_port, rval64));
            } /* if SOC_REG_PORT_VALID XLMAC_TX_CTRL */
        } /* if SOC_PBMP_MEMBER block_port */
    } /* for loop */

    for (loop = 0; loop < SB2_MAX_PORTS_PER_BLOCK; loop++) {
        block_port=(*sb2_block_ports[unit])[block][loop];
        if (SOC_PBMP_MEMBER((PBMP_ALL(unit)), block_port)) {
            if (SOC_REG_PORT_VALID(unit, XLMAC_RX_MAX_SIZEr, block_port)) {
                SOC_IF_ERROR_RETURN(READ_XLMAC_RX_MAX_SIZEr(
                            unit,block_port, &rval64));
                soc_reg64_field32_set(unit, XLMAC_RX_MAX_SIZEr, &rval64,
                        RX_MAX_SIZEf,(12*1024));
                SOC_IF_ERROR_RETURN(WRITE_XLMAC_RX_MAX_SIZEr(
                            unit,block_port, rval64));
            } /* if SOC_REG_PORT_VALID XLMAC_RX_MAX_SIZE */
        } /* if SOC_PBMP_MEMBER block_port */
    } /* for loop */

    if (SOC_REG_PORT_VALID(unit, XLPORT_ECC_CONTROLr, port)) {
        SOC_IF_ERROR_RETURN(READ_XLPORT_ECC_CONTROLr(unit, port, &rval));
        soc_reg_field_set(unit, XLPORT_ECC_CONTROLr, &rval,
                MIB_TSC_MEM_ENf, 1);
        SOC_IF_ERROR_RETURN(WRITE_XLPORT_ECC_CONTROLr(unit, port, rval));
    }

    /* detach phy from old port */
    for (loop = 0; loop < SB2_MAX_PORTS_PER_BLOCK; loop++) {
        block_port=(*sb2_block_ports[unit])[block][loop];
        if ((old_ports[old_lanes-1][loop] == 0) &&
                (new_ports[new_lanes-1][loop] == 0)) {
            continue;
        }
        if (old_ports[old_lanes-1][loop] == new_ports[new_lanes-1][loop] ) {
            /* same port was used so only speed change */
            SOC_IF_ERROR_RETURN(soc_saber2_port_detach(unit,block_port));
        }
        if (old_ports[old_lanes-1][loop]  > new_ports[new_lanes-1][loop]) {
            /* This port is not applicable so remove it from pbmp list */
            SOC_IF_ERROR_RETURN(soc_saber2_port_detach(unit,block_port));
            continue;
        }
        if (new_ports[new_lanes-1][loop]  > old_ports[old_lanes-1][loop]) {
            /* This port is new continue */
            continue;
        }
    }

    for (loop = 0; loop < SB2_MAX_PORTS_PER_BLOCK; loop++) {
        block_port=(*sb2_block_ports[unit])[block][loop];
        if (SOC_PBMP_MEMBER((PBMP_ALL(unit)), block_port)) {
        
            if (SOC_REG_PORT_VALID(unit, XPORT_PORT_ENABLEr, block_port)) {
                SOC_IF_ERROR_RETURN(READ_XPORT_PORT_ENABLEr(unit, block_port,
                            &rval));
                soc_reg_field_set(unit, XPORT_PORT_ENABLEr, &rval,
                        port_enable_field[loop], 1);
                SOC_IF_ERROR_RETURN(WRITE_XPORT_PORT_ENABLEr(unit, block_port,
                            rval));
           }        
                            
           if (SOC_REG_PORT_VALID(unit, XLPORT_ENABLE_REGr, block_port)) {
                SOC_IF_ERROR_RETURN(READ_XLPORT_ENABLE_REGr(unit, block_port,
                            &rval));
                soc_reg_field_set(unit, XLPORT_ENABLE_REGr, &rval,
                        port_enable_field[loop], 1);
                SOC_IF_ERROR_RETURN(WRITE_XLPORT_ENABLE_REGr(unit, block_port,
                            rval));
           }        

        }/* if SOC_PBMP_MEMBER block_port */
    } /* for loop */

    sal_udelay((10*time_multiplier));
    for (loop = 0; loop < SB2_MAX_PORTS_PER_BLOCK; loop++) {
        block_port=(*sb2_block_ports[unit])[block][loop];
        if (SOC_PBMP_MEMBER((PBMP_ALL(unit)), block_port)) {
            BCM_IF_ERROR_RETURN(soc_mac_xl.md_enable_set(
                        unit, block_port, TRUE));
        } /* if SOC_PBMP_MEMBER block_port */
    } /* for loop */

    if (SOC_REG_PORT_VALID(unit, THDO_PORT_DISABLE_CFG1r, port)) {
        for (loop = 0; loop < SB2_MAX_PORTS_PER_BLOCK; loop++) {
            block_port=(*sb2_block_ports[unit])[block][loop];
            SOC_IF_ERROR_RETURN(READ_THDO_PORT_DISABLE_CFG1r(unit,&rval));

            soc_reg_field_set(unit, THDO_PORT_DISABLE_CFG1r, &rval,
                    PORT_WRf,1);
            soc_reg_field_set(unit, THDO_PORT_DISABLE_CFG1r, &rval,
                    PORT_WR_TYPEf,0);
            soc_reg_field_set(unit, THDO_PORT_DISABLE_CFG1r, &rval,
                    PORT_IDf,block_port);
            SOC_IF_ERROR_RETURN(WRITE_THDO_PORT_DISABLE_CFG1r(unit,rval));

            sal_udelay(10*time_multiplier);

            if ((SAL_BOOT_BCMSIM || SAL_BOOT_PLISIM)) {
                soc_reg_field_set(unit, THDO_PORT_DISABLE_CFG1r, &rval,
                        PORT_WRf,0);
                SOC_IF_ERROR_RETURN(WRITE_THDO_PORT_DISABLE_CFG1r(
                            unit,rval));
            }
            SOC_IF_ERROR_RETURN(READ_THDO_PORT_DISABLE_CFG1r(unit,&rval));

            if (soc_reg_field_get(unit, THDO_PORT_DISABLE_CFG1r, rval,
                    PORT_WRf) != 0) {
                LOG_ERROR(BSL_LS_BCM_PORT,
                        (BSL_META_U(unit,
                                  "%s:%d: TimeOut - THDO port disable cfg\n"),
                         FUNCTION_NAME(),__LINE__));
                return BCM_E_TIMEOUT;
            }
        } /* for loop */
    } /* if SOC_REG_PORT_VALID THDO_PORT_DISABLE_CFG1 */

    sal_udelay((10*time_multiplier));
    for (loop = 0; loop < SB2_MAX_PORTS_PER_BLOCK; loop++) {
        block_port=(*sb2_block_ports[unit])[block][loop];
        if (SOC_PBMP_MEMBER((PBMP_ALL(unit)), block_port)) {
            if(block > 0) {
                sal_memset(&egr_enable_entry, 0, sizeof(egr_enable_entry_t));
                soc_mem_field32_set(unit, EGR_ENABLEm, &egr_enable_entry, PRT_ENABLEf, 1);
                SOC_IF_ERROR_RETURN(WRITE_EGR_ENABLEm(unit, MEM_BLOCK_ALL, block_port,
                            &egr_enable_entry));
            } else {
                 SOC_IF_ERROR_RETURN(READ_TXLP_PORT_ENABLEr(
                                     unit, port, &rval));
                 port_enable = soc_reg_field_get(unit, TXLP_PORT_ENABLEr,
                                     rval, PORT_ENABLEf);
                 port_enable |= ( 1<< ((*sb2_port_to_block_subports[unit])
                                      [block_port-1]));
                 soc_reg_field_set(unit, TXLP_PORT_ENABLEr,
                                     &rval, PORT_ENABLEf, port_enable);
                 SOC_IF_ERROR_RETURN(WRITE_TXLP_PORT_ENABLEr(
                                     unit, port, rval));
            }
        }
    }

    /* Update port_num_lanes in soc_info_config */
    for (loop = 0; loop < SB2_MAX_PORTS_PER_BLOCK; loop++) {
        block_port=(*sb2_block_ports[unit])[block][loop];
        if (!SOC_PBMP_MEMBER((PBMP_ALL(unit)), block_port)) {
            /* Not a valid port. So set num_lanes to 0 */
            si->port_num_lanes[block_port] = 0;
            si->port_speed_max[block_port] = 0;
            sal_snprintf(si->port_name[block_port], sizeof(si->port_name[block_port]),
                         "?%d", block_port);
        }
    }
 
    for (loop = 0; loop < SB2_MAX_PORTS_PER_BLOCK; loop++) {
        block_port=(*sb2_block_ports[unit])[block][loop];
        if ((old_ports[old_lanes-1][loop] == 0) &&
                (new_ports[new_lanes-1][loop] == 0)) {
            continue;
        }
        if (old_ports[old_lanes-1][loop] == new_ports[new_lanes-1][loop] ) {
            /* same port was used so only speed change */
            SOC_IF_ERROR_RETURN(soc_saber2_port_attach(
                        unit,block ,block_port));
        }
        if (old_ports[old_lanes-1][loop]  > new_ports[new_lanes-1][loop]) {
            /* This port is not applicable so remove it from pbmp list */
            continue;
        }
        if (new_ports[new_lanes-1][loop]  > old_ports[old_lanes-1][loop]) {
            /* This port is new so add it in pbmp list first */
            SOC_IF_ERROR_RETURN(soc_saber2_port_attach(
                        unit,block ,block_port));
        }
    }

    /* Clear counters for the new ports */
    SOC_PBMP_CLEAR(link_pbmp);
    for (loop = 0; loop < SB2_MAX_PORTS_PER_BLOCK; loop++) {
        block_port=(*sb2_block_ports[unit])[block][loop];
        if (SOC_PBMP_MEMBER((PBMP_ALL(unit)), block_port)) {
            BCM_PBMP_PORT_ADD(link_pbmp,block_port);
        }
    }

    COMPILER_64_ZERO(ctr_val);
    BCM_IF_ERROR_RETURN(soc_counter_set_by_port(unit, link_pbmp, ctr_val));

    bcm_esw_kt2_port_unlock(unit);
    BCM_IF_ERROR_RETURN(bcm_esw_linkscan_mode_set(unit, port, init_mode));
    bcm_esw_kt2_port_lock(unit);
    
#ifdef BCM_WARM_BOOT_SUPPORT
    SOC_CONTROL_LOCK(unit);
    SOC_CONTROL(unit)->scache_dirty = 1;
    SOC_CONTROL_UNLOCK(unit);
#endif

    /* End: Link up sequence */
   
    return BCM_E_NONE;
}

int
_bcm_sb2_port_lanes_get(int unit, bcm_port_t port, int *active_lanes)
{
    uint8 loop=0, block=0;
    int lanes = 0;

    *active_lanes = lanes;

    SOC_IF_ERROR_RETURN(soc_saber2_get_port_block(
                unit, port, &block));

    for (loop = 0;loop < SB2_MAX_PORTS_PER_BLOCK; loop++) {
        if (SOC_PBMP_MEMBER((PBMP_ALL(unit)),
                    (*sb2_block_ports[unit])[block][loop])) {
            lanes++;
        }
    }

    /* active_lanes refer to the number of active lanes per port. */
    /* lanes refer to number of valid lanes in the port block. */
    switch (lanes) {
        case 1 : *active_lanes = 4;
                 break;
        case 2 : *active_lanes = 2;
                 break;
        case 4 :*active_lanes = 1;
                 break;
        default : return BCM_E_INTERNAL;
    }

    return BCM_E_NONE;
}

#if defined(BCM_WARM_BOOT_SUPPORT)
int
_bcm_sb2_wb_v1_flexio_pbmp_update(int unit, bcm_pbmp_t *pbmp)
{
    bcm_port_t blk_port;
    int        blk, blk_idx;
    uint16     dev_id = 0;
    uint8      rev_id = 0;

    soc_cm_get_id(unit, &dev_id, &rev_id);

    if (NULL == pbmp) {
        return BCM_E_PORT;
    }

    if(dev_id == BCM56233_DEVICE_ID) {
        /* Dagger2 does not support flex operation. So no need of pbmp update */
        return SOC_E_NONE;
    }

    for (blk = 0; blk < SB2_MAX_BLOCKS; blk++) {

        if (blk < 6) {
            /* Add the port at index 1, 2, & 3 to gven pbmp and update the
             * port_type so that it is treated as valid port
             */
            for (blk_idx = 1; 
                    blk_idx < SB2_MAX_PORTS_PER_BLOCK; blk_idx++) {

                blk_port = (*sb2_block_ports[unit])[blk][blk_idx];
                if (blk_port < SOC_INFO(unit).lb_port) {
                    BCM_PBMP_PORT_ADD(*pbmp, blk_port);
                    SOC_PORT_TYPE(unit, blk_port) = SOC_BLK_MXQPORT;
                }

            } /* for blk_idx */
        } else if (blk == 6) {

            for (blk_idx = 1; 
                    blk_idx < SB2_MAX_PORTS_PER_BLOCK; blk_idx++) {

                blk_port = (*sb2_block_ports[unit])[blk][blk_idx];
                if (blk_port < SOC_INFO(unit).lb_port) {
                    BCM_PBMP_PORT_ADD(*pbmp, blk_port);
                    SOC_PORT_TYPE(unit, blk_port) = SOC_BLK_XLPORT;
                }

            } /* for blk_idx */
        } /* if SOC_PORT_TYPE */
    } /* For blk */
    return BCM_E_NONE;
} /* _bcm_sb2_flexio_pbmp_update */
#endif

int
bcm_sb2_flexio_pbmp_update(int unit, bcm_pbmp_t *pbmp)
{
    bcm_port_t blk_port;
    bcm_port_t first_port;
    int        blk, blk_idx;
    uint16     dev_id = 0;
    uint8      rev_id = 0;

    soc_cm_get_id(unit, &dev_id, &rev_id);

    if (NULL == pbmp) {
        return BCM_E_PORT;
    }

    if(dev_id == BCM56233_DEVICE_ID) {
        /* Dagger2 does not support flex operation. So no need of pbmp update */
        return SOC_E_NONE;
    }

    for (blk = 0; blk < SB2_MAX_BLOCKS; blk++) {

        /* If the first port is invalid, then skip the block */
        first_port = (*sb2_block_ports[unit])[blk][0];
        if (!SOC_PORT_VALID(unit, first_port)) {
            continue;
        }
        if (blk < 6) {
            /* Add the port at index 1, 2, & 3 to gven pbmp and update the
             * port_type so that it is treated as valid port
             */
            for (blk_idx = 1; 
                    blk_idx < SB2_MAX_PORTS_PER_BLOCK; blk_idx++) {

                blk_port = (*sb2_block_ports[unit])[blk][blk_idx];
                if (blk_port < SOC_INFO(unit).lb_port) {
                    BCM_PBMP_PORT_ADD(*pbmp, blk_port);
                    SOC_PORT_TYPE(unit, blk_port) = SOC_BLK_MXQPORT;
                }

            } /* for blk_idx */
        } else if (blk == 6) {

            for (blk_idx = 1; 
                    blk_idx < SB2_MAX_PORTS_PER_BLOCK; blk_idx++) {

                blk_port = (*sb2_block_ports[unit])[blk][blk_idx];
                if (blk_port < SOC_INFO(unit).lb_port) {
                    BCM_PBMP_PORT_ADD(*pbmp, blk_port);
                    SOC_PORT_TYPE(unit, blk_port) = SOC_BLK_XLPORT;
                }

            } /* for blk_idx */
        } /* if SOC_PORT_TYPE */
    } /* For blk */
    return BCM_E_NONE;
} /* _bcm_sb2_flexio_pbmp_update */

int
_bcm_sb2_port_oamp_enable(int unit, bcm_port_t port, int enable)
{
    uint32                  rval = 0;
    uint32                  time_multiplier=1;
    uint8                   block;
    uint8                   first_port = 0;
    uint8                   rev_id = 0;
#if defined(BCM_METROLITE_SUPPORT)
    uint8                   loop = 0;
    int                     cfg_num = 0;
#endif
    uint16                  dev_id = 0;
    int                     init_mode = 0, oamp_status = 0;
    uint32                  cell_cnt = 0;
    uint32                  try = 0 , try_count = 0;
    int                     min_polls = 100;
    uint32                  pfc_enable=0;
    int                     rv = 0;
    int                     port_enable = 0;
    int                     port_speed = 0, max_speed = 0;
    uint64                  xlmac_ctrl;
    soc_info_t              *si = &SOC_INFO(unit);
    epc_link_bmap_entry_t   epc_link_bmap_entry={{0}};
    soc_pbmp_t              link_pbmp;
    soc_timeout_t           to = {0};
    sal_usecs_t             timeout_usec = 100000;
    egr_enable_entry_t      egr_enable_entry = {{0}};

    soc_cm_get_id(unit, &dev_id, &rev_id);
    if ((rev_id == BCM56260_A0_REV_ID) &&
        (!(SOC_IS_METROLITE(unit)))){
        /* OAMP hotswap not supported in Saber2 A0 device */
        return BCM_E_UNAVAIL;
    }

    if(dev_id == BCM56233_DEVICE_ID) {
        /* Dagger2 does not support OAMP hostswap */
        return SOC_E_UNAVAIL;
    }

    if (!SOC_PORT_VALID(unit, port)) {
        return BCM_E_PORT;
    }

    /* Corner case : enable is neither TRUE nor FALSE */
    if(!((enable == TRUE) || (enable == FALSE))) {
        return BCM_E_PARAM;
    }

    if (SAL_BOOT_QUICKTURN) {
        time_multiplier=1000;
    }

#if defined(BCM_METROLITE_SUPPORT) 
    /* Retreive internal configuration no. */
    BCM_IF_ERROR_RETURN(_soc_metrolite_get_cfg_num(unit, 0, &cfg_num));
#endif

    /* In saber2 HW design supports OAMP only on port 5 */
    if (port != _BCM_SB2_SAT_OAMP_PHY_PORT_NUMBER) {
            LOG_CLI((BSL_META_U(unit,
                            "OAMP HotSwap is not supported for port %d.\n"), port));
        return BCM_E_PORT;
    }

#if defined(BCM_METROLITE_SUPPORT)
    if (SOC_IS_METROLITE(unit)){
        SOC_IF_ERROR_RETURN(soc_metrolite_get_port_block(
                    unit,port,&block));

        for (loop = 0; loop < ML_MAX_PORTS_PER_BLOCK; loop++) {
            if ((IS_LP_PORT(unit, (*ml_block_ports[unit])[block][loop])) ||
                    (IS_SUBTAG_PORT(unit, (*ml_block_ports[unit])[block][loop]))) {
                LOG_CLI((BSL_META_U(unit,
                                "HotSwap is not supported for \
    Subport Coe/LinkPhy Ports\n Disable CoE and run the API again.\n")));
	        return BCM_E_PARAM;
	    }
	}
    }else
#endif
    {
        SOC_IF_ERROR_RETURN(soc_saber2_get_port_block(
                    unit,port,&block));

    /* CoE ports cannot be hotswapped */
    if (IS_SUBTAG_PORT(unit, port)) {
            LOG_CLI((BSL_META_U(unit,
                            "HotSwap is not supported for CoE port\n")));
            return BCM_E_PARAM;
	}
    }

    /* Check whether SAT is initialized */
    BCM_IF_ERROR_RETURN(bcm_sb2_sat_init_check(unit));

    SOC_IF_ERROR_RETURN(READ_OAMP_ENABLEr(unit, &rval));
    oamp_status = soc_reg_field_get(unit, OAMP_ENABLEr, rval, ENABLEf);
    if(oamp_status == enable) {
        /* Status already met the requirement */
        return BCM_E_NONE;
    }
    
    rv = bcm_esw_linkscan_mode_get(unit, port, &init_mode);
    if(rv == BCM_E_PORT) {
        /* port not available. Maybe in oamp mode. */
        init_mode = BCM_LINKSCAN_MODE_NONE;
    }

    if (enable == TRUE) {
        /* Ethernet to OAMP sequence */

        /* 1. Stop linkscan */
        bcm_esw_kt2_port_unlock(unit);
        BCM_IF_ERROR_RETURN(bcm_esw_linkscan_mode_set(
                    unit, port, BCM_LINKSCAN_MODE_NONE));
        bcm_esw_kt2_port_lock(unit);

        /* 2. To stop the incoming traffic, disable XLMAC Rx */
         SOC_IF_ERROR_RETURN(READ_XLMAC_CTRLr(
                             unit, port, &xlmac_ctrl));
         soc_reg64_field32_set(unit, XLMAC_CTRLr, &xlmac_ctrl, TX_ENf, 1);
         soc_reg64_field32_set(unit, XLMAC_CTRLr, &xlmac_ctrl, RX_ENf, 0);
         SOC_IF_ERROR_RETURN(WRITE_XLMAC_CTRLr(
                             unit, port, xlmac_ctrl));
         sal_udelay((10*time_multiplier));

        /* 3. Bring the port down. */
        BCM_IF_ERROR_RETURN(READ_EPC_LINK_BMAPm(
                    unit, MEM_BLOCK_ANY, 0, &epc_link_bmap_entry));
        soc_mem_pbmp_field_get(unit, EPC_LINK_BMAPm,
                &epc_link_bmap_entry, PORT_BITMAPf,&link_pbmp);
        BCM_PBMP_PORT_REMOVE(link_pbmp, port);
        soc_mem_pbmp_field_set(unit, EPC_LINK_BMAPm,
                &epc_link_bmap_entry, PORT_BITMAPf,&link_pbmp);
        SOC_IF_ERROR_RETURN(WRITE_EPC_LINK_BMAPm(
                    unit, MEM_BLOCK_ALL, 0, &epc_link_bmap_entry));

        /* 4. Save the current configuration of PAUSE/PFC flow control */
        BCM_IF_ERROR_RETURN(bcm_esw_port_speed_get(
                    unit, port, &port_speed));
        if (!(SAL_BOOT_BCMSIM || SAL_BOOT_PLISIM)) {
            if (port_speed == 10000) {
                BCM_IF_ERROR_RETURN(READ_XPORT_TO_MMU_BKPr(
                            unit, block, &pfc_enable));
                BCM_IF_ERROR_RETURN(WRITE_XPORT_TO_MMU_BKPr(
                            unit, block, 0));
            }
         }

        /* 5. Flush any pending egress packets */
        READ_XP_TXFIFO_PKT_DROP_CTLr(unit, port, &rval);
        soc_reg_field_set(unit, XP_TXFIFO_PKT_DROP_CTLr, &rval,
                DROP_ENf, 1);
        WRITE_XP_TXFIFO_PKT_DROP_CTLr(unit, port, rval);

        BCM_IF_ERROR_RETURN(bcm_kt2_cosq_port_flush(
                    unit, port, bcmCosqFlushTypeQueue));

        /* 6. Wait until all egress port packets are drained. */
        try_count=0;
        if ((SAL_BOOT_BCMSIM || SAL_BOOT_PLISIM)) {
            BCM_IF_ERROR_RETURN(READ_XP_TXFIFO_CELL_CNTr(
                        unit, port, &rval));
            soc_reg_field_set(unit, XP_TXFIFO_CELL_CNTr,
                    &rval, CELL_CNTf, 0);
            BCM_IF_ERROR_RETURN(WRITE_XP_TXFIFO_CELL_CNTr(
                        unit, port, rval));
        }

        soc_timeout_init(&to, timeout_usec, min_polls);
        for (try=0; try<100 && try_count < 10 ; try++) {
            if(soc_timeout_check(&to)) {
                LOG_ERROR(BSL_LS_BCM_PORT,
                        (BSL_META_U(unit,
                                    "%s:%d:TimeOut InternalError\n"),
                         FUNCTION_NAME(),__LINE__));
                return BCM_E_INTERNAL;
            }

            BCM_IF_ERROR_RETURN(READ_XP_TXFIFO_CELL_CNTr (
                        unit,port, &rval));
            cell_cnt=soc_reg_field_get(unit, XP_TXFIFO_CELL_CNTr,
                    rval, CELL_CNTf);
            if (cell_cnt == 0) {
                try_count++;
                break;
            }
        }

        /* 6a. Disable Flush */
        READ_XP_TXFIFO_PKT_DROP_CTLr(unit, port, &rval);
        soc_reg_field_set(unit, XP_TXFIFO_PKT_DROP_CTLr, &rval,
                DROP_ENf, 0);
        WRITE_XP_TXFIFO_PKT_DROP_CTLr(unit, port, rval);


        if (try == 100) {
            LOG_ERROR(BSL_LS_BCM_PORT,
                    (BSL_META_U(unit,
                                "%s:%d: TimeOut - Drain Egress packets\n"),
                     FUNCTION_NAME(),__LINE__));
            return BCM_E_TIMEOUT;
        }

        /* 7. Disable XLMAC Tx and put XLMAC in reset */
         SOC_IF_ERROR_RETURN(READ_XLMAC_CTRLr(
                             unit, port, &xlmac_ctrl));
         soc_reg64_field32_set(unit, XLMAC_CTRLr, &xlmac_ctrl, TX_ENf, 0);
         SOC_IF_ERROR_RETURN(WRITE_XLMAC_CTRLr(
                             unit, port, xlmac_ctrl));
         sal_udelay((10*time_multiplier));
         soc_reg64_field32_set(unit, XLMAC_CTRLr, &xlmac_ctrl, SOFT_RESETf, 1);
         SOC_IF_ERROR_RETURN(WRITE_XLMAC_CTRLr(
                             unit, port, xlmac_ctrl));
         sal_udelay((10*time_multiplier));

        /* 8. Disable port 5 */
	 if (SOC_IS_METROLITE(unit)){
            SOC_IF_ERROR_RETURN(READ_XLPORT_ENABLE_REGr(unit, port, &rval));
            soc_reg_field_set(unit, XLPORT_ENABLE_REGr, &rval, PORT0f, 0);
	    SOC_IF_ERROR_RETURN(WRITE_XLPORT_ENABLE_REGr(unit, port, rval));
	 } else{
            SOC_IF_ERROR_RETURN(READ_XPORT_PORT_ENABLEr(unit, port, &rval));
	    soc_reg_field_set(unit, XPORT_PORT_ENABLEr, &rval, PORT0f, 0);
	    SOC_IF_ERROR_RETURN(WRITE_XPORT_PORT_ENABLEr(unit, port, rval));
	 }

        /* 8a. Detach PHY */
        BCM_IF_ERROR_RETURN(soc_saber2_port_detach(unit, port));

        /* 9. Enable OAMP */
        BCM_IF_ERROR_RETURN(bcm_sb2_sat_oamp_enable_set(unit, enable));
        BCM_IF_ERROR_RETURN(bcm_katana2_cosq_reconfigure_flexio(unit, port, 0));
#if defined(BCM_METROLITE_SUPPORT)
        if (SOC_IS_METROLITE(unit)){
            SOC_IF_ERROR_RETURN(_soc_ml_mmu_reconfigure(unit));
	}else
#endif
	{
            SOC_IF_ERROR_RETURN(_soc_saber2_mmu_reconfigure(unit));
	}

        /* 10. Bring the port back up. */
        BCM_IF_ERROR_RETURN(READ_EPC_LINK_BMAPm(
                    unit, MEM_BLOCK_ANY, 0, &epc_link_bmap_entry));
        soc_mem_pbmp_field_get(unit, EPC_LINK_BMAPm,
                &epc_link_bmap_entry, PORT_BITMAPf,&link_pbmp);
        BCM_PBMP_PORT_ADD(link_pbmp, port);
        soc_mem_pbmp_field_set(unit, EPC_LINK_BMAPm,
                &epc_link_bmap_entry, PORT_BITMAPf,&link_pbmp);
        SOC_IF_ERROR_RETURN(WRITE_EPC_LINK_BMAPm(
                    unit, MEM_BLOCK_ALL, 0, &epc_link_bmap_entry));

        /* Keep PORT in STP FWD state w.r.t OAMP engine */
        BCM_IF_ERROR_RETURN(bcm_esw_port_stp_set(unit, port, 
                    BCM_STG_STP_FORWARD));

        /* Enable Egress */
        sal_memset(&egr_enable_entry, 0, sizeof(egr_enable_entry_t));
        soc_mem_field32_set(unit, EGR_ENABLEm, &egr_enable_entry,
                PRT_ENABLEf, 1);
        SOC_IF_ERROR_RETURN(WRITE_EGR_ENABLEm(unit, MEM_BLOCK_ALL, port,
                &egr_enable_entry));

        /* Update port bitmaps */
#if defined(BCM_METROLITE_SUPPORT)
	if (SOC_IS_METROLITE(unit)){
            first_port = (*ml_block_ports[unit])[block][0];
            port_speed = bcm56270_speed[unit][cfg_num][first_port];
	} else
#endif
	{
            first_port = (*sb2_block_ports[unit])[block][0];
            port_speed = bcm56260_speed[unit][_sb2_config_id[unit]][first_port];
	}
        if (port_speed <= 2500) {
            SOC_PBMP_PORT_REMOVE(si->ge.bitmap, port);
            SOC_PBMP_PORT_ADD(si->ge.disabled_bitmap, port);
        } else {
            SOC_PBMP_PORT_REMOVE(si->xe.bitmap, port);
            SOC_PBMP_PORT_ADD(si->xe.disabled_bitmap, port);
        }
        SOC_PBMP_PORT_REMOVE(si->ether.bitmap, port);
        SOC_PBMP_PORT_ADD(si->ether.disabled_bitmap, port);

        SOC_PBMP_PORT_REMOVE(si->port.bitmap, port);
        SOC_PBMP_PORT_ADD(si->port.disabled_bitmap, port);

        SOC_PBMP_PORT_ADD(si->intp.bitmap, port);
        SOC_PBMP_PORT_REMOVE(si->intp.disabled_bitmap, port);

        soc_katana2_pbmp_all_resync(unit) ;
        soc_esw_dport_init(unit);

    } else {
        /* OAMP to Ethernet sequence */

        /* 1. Check whether the GTFs and CTFs are destroyed */
        if((bcm_common_sat_gtf_inuse(unit)) ||
                (bcm_common_sat_ctf_inuse(unit))) {
            LOG_ERROR(BSL_LS_BCM_PORT,
                    (BSL_META_U(unit,
                                "GTFs and CTFs not cleaned up\n")));
            return BCM_E_FAIL;
        }

        /* 2. Disable SAT redirection to OAMP from ingress and egress pipe */
        BCM_IF_ERROR_RETURN(
                bcm_sb2_sat_endpoint_destroy_all(unit, BCM_SAT_ENDPOINT_UPMEP));
        BCM_IF_ERROR_RETURN(
                bcm_sb2_sat_endpoint_destroy_all(unit, BCM_SAT_ENDPOINT_DOWNMEP));

        /* 3. Bring the port down */
        BCM_IF_ERROR_RETURN(READ_EPC_LINK_BMAPm(
                    unit, MEM_BLOCK_ANY, 0, &epc_link_bmap_entry));
        soc_mem_pbmp_field_get(unit, EPC_LINK_BMAPm,
                &epc_link_bmap_entry, PORT_BITMAPf,&link_pbmp);
        BCM_PBMP_PORT_REMOVE(link_pbmp, port);
        soc_mem_pbmp_field_set(unit, EPC_LINK_BMAPm,
                &epc_link_bmap_entry, PORT_BITMAPf,&link_pbmp);
        SOC_IF_ERROR_RETURN(WRITE_EPC_LINK_BMAPm(
                    unit, MEM_BLOCK_ALL, 0, &epc_link_bmap_entry));

        /* 4. Flush any pending egress packets */
        READ_XP_TXFIFO_PKT_DROP_CTLr(unit, port, &rval);
        soc_reg_field_set(unit, XP_TXFIFO_PKT_DROP_CTLr, &rval,
                DROP_ENf, 1);
        WRITE_XP_TXFIFO_PKT_DROP_CTLr(unit, port, rval);

        BCM_IF_ERROR_RETURN(bcm_kt2_cosq_port_flush(
                    unit, port, bcmCosqFlushTypeQueue));

        /* 5. Wait until all egress port packets are drained. */
        try_count=0;
        if ((SAL_BOOT_BCMSIM || SAL_BOOT_PLISIM)) {
            BCM_IF_ERROR_RETURN(READ_XP_TXFIFO_CELL_CNTr(
                        unit, port, &rval));
            soc_reg_field_set(unit, XP_TXFIFO_CELL_CNTr,
                    &rval, CELL_CNTf, 0);
            BCM_IF_ERROR_RETURN(WRITE_XP_TXFIFO_CELL_CNTr(
                        unit, port, rval));
        }

        soc_timeout_init(&to, timeout_usec, min_polls);
        for (try=0; try<100 && try_count < 10 ; try++) {
            if(soc_timeout_check(&to)) {
                LOG_ERROR(BSL_LS_BCM_PORT,
                        (BSL_META_U(unit,
                                    "%s:%d:TimeOut InternalError\n"),
                         FUNCTION_NAME(),__LINE__));
                return BCM_E_INTERNAL;
            }

            BCM_IF_ERROR_RETURN(READ_XP_TXFIFO_CELL_CNTr (
                        unit,port, &rval));
            cell_cnt=soc_reg_field_get(unit, XP_TXFIFO_CELL_CNTr,
                    rval, CELL_CNTf);
            if (cell_cnt == 0) {
                try_count++;
                break;
            }
        }

        /* 5a. Disable Flush */
        READ_XP_TXFIFO_PKT_DROP_CTLr(unit, port, &rval);
        soc_reg_field_set(unit, XP_TXFIFO_PKT_DROP_CTLr, &rval,
                DROP_ENf, 0);
        WRITE_XP_TXFIFO_PKT_DROP_CTLr(unit, port, rval);

        if (try == 100) {
            LOG_ERROR(BSL_LS_BCM_PORT,
                    (BSL_META_U(unit,
                                "%s:%d: TimeOut - Drain Egress packets\n"),
                     FUNCTION_NAME(),__LINE__));
            return BCM_E_TIMEOUT;
        }

        /* 6. Disable OAMP */
        BCM_IF_ERROR_RETURN(bcm_sb2_sat_oamp_enable_set(unit, enable));
#if defined(BCM_METROLITE_SUPPORT)
        if (SOC_IS_METROLITE(unit)){
            SOC_IF_ERROR_RETURN(_soc_ml_mmu_reconfigure(unit));
	}else
#endif
	{
            SOC_IF_ERROR_RETURN(_soc_saber2_mmu_reconfigure(unit)); 
	}
        
        /* 7. Enable port 5 */
        if (SOC_IS_METROLITE(unit)){
            SOC_IF_ERROR_RETURN(READ_XLPORT_ENABLE_REGr(unit, port, &rval));
	    soc_reg_field_set(unit, XLPORT_ENABLE_REGr, &rval, PORT0f, 1);
	    SOC_IF_ERROR_RETURN(WRITE_XLPORT_ENABLE_REGr(unit, port, rval));
	}else{
            SOC_IF_ERROR_RETURN(READ_XPORT_PORT_ENABLEr(unit, port, &rval));
	    soc_reg_field_set(unit, XPORT_PORT_ENABLEr, &rval, PORT0f, 1);
	    SOC_IF_ERROR_RETURN(WRITE_XPORT_PORT_ENABLEr(unit, port, rval));
	}

        /* 8. Enable XLMAC Tx and Rx and being XLMAC out of reset */
         SOC_IF_ERROR_RETURN(READ_XLMAC_CTRLr(
                             unit, port, &xlmac_ctrl));
         soc_reg64_field32_set(unit, XLMAC_CTRLr, &xlmac_ctrl, SOFT_RESETf, 0);
         SOC_IF_ERROR_RETURN(WRITE_XLMAC_CTRLr(
                             unit, port, xlmac_ctrl));
         sal_udelay((10*time_multiplier));
         soc_reg64_field32_set(unit, XLMAC_CTRLr, &xlmac_ctrl, TX_ENf, 1);
         soc_reg64_field32_set(unit, XLMAC_CTRLr, &xlmac_ctrl, RX_ENf, 1);
         SOC_IF_ERROR_RETURN(WRITE_XLMAC_CTRLr(
                             unit, port, xlmac_ctrl));
         sal_udelay((10*time_multiplier));

        /* 9. Bring the port back up. */
        BCM_IF_ERROR_RETURN(READ_EPC_LINK_BMAPm(
                    unit, MEM_BLOCK_ANY, 0, &epc_link_bmap_entry));
        soc_mem_pbmp_field_get(unit, EPC_LINK_BMAPm,
                &epc_link_bmap_entry, PORT_BITMAPf,&link_pbmp);
        BCM_PBMP_PORT_ADD(link_pbmp, port);
        soc_mem_pbmp_field_set(unit, EPC_LINK_BMAPm,
                &epc_link_bmap_entry, PORT_BITMAPf,&link_pbmp);
        SOC_IF_ERROR_RETURN(WRITE_EPC_LINK_BMAPm(
                    unit, MEM_BLOCK_ALL, 0, &epc_link_bmap_entry));

        /* Update port bitmaps */
#if defined(BCM_METROLITE_SUPPORT)
        if (SOC_IS_METROLITE(unit)){
            first_port = (*ml_block_ports[unit])[block][0];
            port_speed = bcm56270_speed[unit][cfg_num][first_port];
	} else
#endif
	{
            first_port = (*sb2_block_ports[unit])[block][0];
            port_speed = bcm56260_speed[unit][_sb2_config_id[unit]][first_port];
	}
        if (port_speed <= 2500) {
            SOC_PBMP_PORT_REMOVE(si->ge.disabled_bitmap, port);
            SOC_PBMP_PORT_ADD(si->ge.bitmap, port);
        } else {
            SOC_PBMP_PORT_REMOVE(si->xe.disabled_bitmap, port);
            SOC_PBMP_PORT_ADD(si->xe.bitmap, port);
        }
        SOC_PBMP_PORT_REMOVE(si->ether.disabled_bitmap, port);
        SOC_PBMP_PORT_ADD(si->ether.bitmap, port);

        SOC_PBMP_PORT_REMOVE(si->port.disabled_bitmap, port);
        SOC_PBMP_PORT_ADD(si->port.bitmap, port);

        SOC_PBMP_PORT_ADD(si->intp.disabled_bitmap, port);
        SOC_PBMP_PORT_REMOVE(si->intp.bitmap, port);

        soc_katana2_pbmp_all_resync(unit) ;
        soc_esw_dport_init(unit);

        SOC_IF_ERROR_RETURN(soc_saber2_port_attach(unit, block , port));
        
        /* 10. Enable linkscan */
        if (init_mode == BCM_LINKSCAN_MODE_NONE) {
            init_mode = BCM_LINKSCAN_MODE_SW;
        }
        bcm_esw_kt2_port_unlock(unit);

#ifdef BCM_PORT_DEFAULT_DISABLE
        port_enable = FALSE;
#else
        port_enable = TRUE;
#endif  /* BCM_PORT_DEFAULT_DISABLE */

        BCM_IF_ERROR_RETURN(bcm_esw_port_enable_set(unit, port, port_enable));
#if defined(BCM_METROLITE_SUPPORT)
        if (SOC_IS_METROLITE(unit)){
            port_speed = (*ml_port_speeds[unit])[block][0];
	}else
#endif
	{
            port_speed = (*sb2_port_speeds[unit])[block][0];
	}
        BCM_IF_ERROR_RETURN(bcm_esw_port_speed_max(unit, port, &max_speed));
        port_speed = port_speed > max_speed ? max_speed : port_speed;
        BCM_IF_ERROR_RETURN(bcm_esw_port_speed_set(unit, port, port_speed));

        BCM_IF_ERROR_RETURN(bcm_esw_port_learn_set(unit, port,
                    BCM_PORT_LEARN_ARL | BCM_PORT_LEARN_FWD));
        BCM_IF_ERROR_RETURN(bcm_esw_port_stp_set(unit, port, 
                    BCM_STG_STP_FORWARD));
        BCM_IF_ERROR_RETURN(bcm_esw_port_loopback_set(unit, port,
                    BCM_PORT_LOOPBACK_NONE));

        /* if sat is enabled with sat_enable config, then tpid would not set */
        rv = bcm_esw_port_tpid_set(unit, port,
                        _bcm_fb2_outer_tpid_default_get(unit));
        if (BCM_FAILURE(rv)) {
         LOG_WARN(BSL_LS_BCM_PORT,
                  (BSL_META_U(unit,
                              "Failed to set default TPID: %s\n"),
                  bcm_errmsg(rv)));
        }
        if (soc_feature(unit, soc_feature_inner_tpid_enable)) {
            /* Enable the default inner TPID. */
            rv = bcm_esw_port_inner_tpid_set(unit, port,
                            _bcm_fb2_outer_tpid_default_get(unit));
            if (BCM_FAILURE(rv)) {
             LOG_WARN(BSL_LS_BCM_PORT,
                  (BSL_META_U(unit,
                              "Failed to set default Inner TPID: %s\n"),
                  bcm_errmsg(rv)));
            }
        }

        BCM_IF_ERROR_RETURN(bcm_esw_linkscan_mode_set(unit, port, init_mode));
        bcm_esw_kt2_port_lock(unit);
    }

#ifdef BCM_WARM_BOOT_SUPPORT
    SOC_CONTROL_LOCK(unit);
    SOC_CONTROL(unit)->scache_dirty = 1;
    SOC_CONTROL_UNLOCK(unit);
#endif

    return BCM_E_NONE;
}

int _bcm_saber2_mmu_dest_port_cfg_init(int unit, int port)
{
    uint32 rval, rval1, rval2;
    int start_pp_port, end_pp_port, pp_port;
    soc_info_t    *si= &SOC_INFO(unit);

    if (IS_EXT_MEM_PORT(unit, port)) {

        SOC_IF_ERROR_RETURN(READ_DEST_PORT_CFG_0r(unit, &rval));
        rval |= (1 << port);
        SOC_IF_ERROR_RETURN(WRITE_DEST_PORT_CFG_0r(unit, rval));

        SOC_IF_ERROR_RETURN(READ_MMU_ENQ_DEST_PPP_CFG_0r(unit, &rval));
        rval |= (1 << port);
        SOC_IF_ERROR_RETURN(WRITE_MMU_ENQ_DEST_PPP_CFG_0r(unit, rval));

        if (si->port_num_subport[port] > 0) {
            start_pp_port = si->port_subport_base[port];
            end_pp_port = si->port_subport_base[port] +
                si->port_num_subport[port];

            SOC_IF_ERROR_RETURN(READ_MMU_ENQ_DEST_PPP_CFG_0r(unit, &rval));
            SOC_IF_ERROR_RETURN(READ_MMU_ENQ_DEST_PPP_CFG_1r(unit, &rval1));
            SOC_IF_ERROR_RETURN(READ_MMU_ENQ_DEST_PPP_CFG_2r(unit, &rval2));
            for (pp_port = start_pp_port; pp_port < end_pp_port; pp_port++) {
                if (( pp_port <= 31 )){
                    rval |= (1 << pp_port);
                } else if (( pp_port >31 ) && (pp_port <= 63) ){
                    rval1 |= (1 << (pp_port - 32));
                } else if (( pp_port > 63 ) && (pp_port <= 95) ){
                    rval2 |= (1 << (pp_port - 64));
                }
            }
            SOC_IF_ERROR_RETURN(WRITE_MMU_ENQ_DEST_PPP_CFG_0r(unit, rval));
            SOC_IF_ERROR_RETURN(WRITE_MMU_ENQ_DEST_PPP_CFG_1r(unit, rval1));
            SOC_IF_ERROR_RETURN(WRITE_MMU_ENQ_DEST_PPP_CFG_2r(unit, rval2));
        }
    } else {
        SOC_IF_ERROR_RETURN(READ_DEST_PORT_CFG_0r(unit, &rval));
        rval &= ~(1 << port);
        SOC_IF_ERROR_RETURN(WRITE_DEST_PORT_CFG_0r(unit, rval));

        SOC_IF_ERROR_RETURN(READ_MMU_ENQ_DEST_PPP_CFG_0r(unit, &rval));
        rval &= ~(1 << port);
        SOC_IF_ERROR_RETURN(WRITE_MMU_ENQ_DEST_PPP_CFG_0r(unit, rval));

        if (si->port_num_subport[port] > 0) {
            start_pp_port = si->port_subport_base[port];
            end_pp_port = si->port_subport_base[port] +
                si->port_num_subport[port];

            SOC_IF_ERROR_RETURN(READ_MMU_ENQ_DEST_PPP_CFG_0r(unit, &rval));
            SOC_IF_ERROR_RETURN(READ_MMU_ENQ_DEST_PPP_CFG_1r(unit, &rval1));
            SOC_IF_ERROR_RETURN(READ_MMU_ENQ_DEST_PPP_CFG_2r(unit, &rval2));
            for (pp_port = start_pp_port; pp_port < end_pp_port; pp_port++) {
                if (( pp_port <= 31 )){
                    rval &= ~(1 << pp_port);
                } else if (( pp_port >31 ) && (pp_port <= 63) ){
                    rval1 &= ~(1 << (pp_port - 32));
                } else if (( pp_port > 63 ) && (pp_port <= 95) ){
                    rval2 &= ~(1 << (pp_port - 64));
                }
            }
            SOC_IF_ERROR_RETURN(WRITE_MMU_ENQ_DEST_PPP_CFG_0r(unit, rval));
            SOC_IF_ERROR_RETURN(WRITE_MMU_ENQ_DEST_PPP_CFG_1r(unit, rval1));
            SOC_IF_ERROR_RETURN(WRITE_MMU_ENQ_DEST_PPP_CFG_2r(unit, rval2));
        }
    }
    return SOC_E_NONE;
}

int _bcm_saber2_mmu_credit_reset_ext_mem(int unit, int port)
{
    uint32      rval = 0;
    soc_info_t  *si= &SOC_INFO(unit);
    uint8       block=0;
    uint8       loop;
    uint8       try_loop;
    int         block_port;
    uint32      rval1 = 0;
    uint32      start_addr = 0, end_addr = 0;
    uint32      try_count = 0;
    soc_timeout_t to = {0};
    sal_usecs_t timeout_usec = 100000;
    int         min_polls = 100;
    uint32      egr_fifo_depth = 0;
    int         threshold;
    int         port_bw, start_threshold = 0;
    int         block_max_nxtaddr = 0;
    soc_field_t        mmu_intf_reset_fld[] = {
                       XQ1_MMU_INTF_RESETf,
                       XQ2_MMU_INTF_RESETf, XQ3_MMU_INTF_RESETf,
                       XQ4_MMU_INTF_RESETf, XQ5_MMU_INTF_RESETf,
                       XQ6_MMU_INTF_RESETf, XQ7_MMU_INTF_RESETf,
                       XQ8_MMU_INTF_RESETf, XQ9_MMU_INTF_RESETf};
    txlp_port_addr_map_table_entry_t txlp_port_addr_map_table_entry={{0}};

    BCM_IF_ERROR_RETURN(soc_saber2_get_port_block(unit,port,&block));

    block_max_nxtaddr = block_max_startaddr[block];
    for (loop = 0;loop < SB2_MAX_PORTS_PER_BLOCK; loop++) {
        block_port=(*sb2_block_ports[unit])[block][loop];
        if((block == 6) && (block_port != port)) {

            if ((block_max_nxtaddr + egr_fifo_depth) > block_max_endaddr[block]) {
                egr_fifo_depth = (block_max_endaddr[block] -
                        block_max_nxtaddr)+1;
                block_max_nxtaddr = block_max_endaddr[block];
            } else {
                block_max_nxtaddr += egr_fifo_depth;
            }
            if (egr_fifo_depth == 1) {
                LOG_CLI((BSL_META_U(unit,
                                "WARN: EGR_FIFO_DEPTH IS ZERO for port=%d\n"),
                            block_port));
            }

            continue;
        }
        if (SOC_PBMP_MEMBER((PBMP_ALL(unit)), block_port)) {

            /* Reconfigure DEQ FIFO depth based on ext mem configuration */
            BCM_IF_ERROR_RETURN(READ_DEQ_EFIFO_CFGr(unit,block_port,&rval));

            if (si->port_speed_max[block_port] <= 1000) {
                egr_fifo_depth = (IS_EXT_MEM_PORT(unit, block_port)) ? 10 : 2;
                threshold = (IS_EXT_MEM_PORT(unit, block_port)) ? 8 : 1;
                port_bw = 25;
                start_threshold = 7;
            } else if (si->port_speed_max[block_port] <= 2500) {
                egr_fifo_depth = (IS_EXT_MEM_PORT(unit, block_port)) ? 19 : 2;
                threshold = (IS_EXT_MEM_PORT(unit, block_port)) ? 15 : 1;
                port_bw = 63;
                start_threshold = 14;
            } else if (si->port_speed_max[block_port] <= 13000) {
                egr_fifo_depth = (IS_EXT_MEM_PORT(unit, block_port)) ? 53 : 10;
                threshold = (IS_EXT_MEM_PORT(unit, block_port)) ? 49 : 1;
                port_bw = 325;
                start_threshold = 48;
            } else {
                LOG_ERROR(BSL_LS_BCM_PORT,
                        (BSL_META_U(unit, "Invalid Speed config\n")));
                return BCM_E_FAIL;
            }

            soc_reg_field_set(unit, DEQ_EFIFO_CFGr, &rval,
                    EGRESS_FIFO_START_ADDRESSf, block_max_nxtaddr);

            if ((block_max_nxtaddr + egr_fifo_depth) > block_max_endaddr[block]) {
                egr_fifo_depth = (block_max_endaddr[block] -
                        block_max_nxtaddr)+1;
                block_max_nxtaddr = block_max_endaddr[block];
            } else {
                block_max_nxtaddr += egr_fifo_depth;
            }
            if (egr_fifo_depth == 1) {
                LOG_CLI((BSL_META_U(unit,
                                "WARN: EGR_FIFO_DEPTH IS ZERO for port=%d\n"),
                            block_port));
            }

             if ((IS_EXT_MEM_PORT(unit, block_port)) &&
                 !(PBMP_MEMBER(si->linkphy_pbm, block_port))) {
                 soc_reg_field_set(unit, DEQ_EFIFO_CFGr, &rval,
                                   EGRESS_FIFO_XMIT_OVERSPEED_RATE_LIMITERf, 1);
             } else {
                 soc_reg_field_set(unit, DEQ_EFIFO_CFGr, &rval,
                                   EGRESS_FIFO_XMIT_OVERSPEED_RATE_LIMITERf, 0);
             }

            soc_reg_field_set(unit, DEQ_EFIFO_CFGr, &rval,
                    EGRESS_FIFO_DEPTHf, egr_fifo_depth);
            soc_reg_field_set(unit, DEQ_EFIFO_CFGr, &rval,
                    EGRESS_FIFO_XMIT_THRESHOLDf, threshold);
            BCM_IF_ERROR_RETURN(WRITE_DEQ_EFIFO_CFGr(
                        unit,block_port,rval));

            BCM_IF_ERROR_RETURN(_bcm_saber2_mmu_dest_port_cfg_init(unit, port));

            /* Port BW Ctrl */
            SOC_IF_ERROR_RETURN(READ_TOQ_PORT_BW_CTRLr(unit,
                        block_port, &rval));
            soc_reg_field_set(unit, TOQ_PORT_BW_CTRLr, &rval,
                    PORT_BWf, port_bw);
            soc_reg_field_set(unit, TOQ_PORT_BW_CTRLr, &rval,
                    START_THRESHOLDf, start_threshold);
            SOC_IF_ERROR_RETURN(WRITE_TOQ_PORT_BW_CTRLr(unit,
                        block_port, rval));

        } /* If SOC_PBMP_MEMBER */
        if(block == 6) {
            break;
        }
    } /* for loop */


    /* WORK-AROUND for port-flushing i.e. need to repeat below step twice */
    /* Refer Flexio hotswap sequence */
    for (try_loop = 0; try_loop < 2; try_loop++) { 
        /* EP Reset */
        if ((SAL_BOOT_BCMSIM || SAL_BOOT_PLISIM)) {
            BCM_IF_ERROR_RETURN(WRITE_DYNAMIC_PORT_RECFG_VECTOR_CFG_29_00r(
                        unit, 0));
        }

        if(block == 6) {
            rval1 |= (1 << port);
        } else {
            for (loop = 0; loop < SB2_MAX_PORTS_PER_BLOCK; loop++) {
                block_port=(*sb2_block_ports[unit])[block][loop];
                if (SOC_PBMP_MEMBER((PBMP_ALL(unit)), block_port)) {
                    rval1 |= (1 << block_port);
                }
            }
        }

        BCM_IF_ERROR_RETURN(WRITE_DYNAMIC_PORT_RECFG_VECTOR_CFG_29_00r(
                    unit, rval1));

        if ((SAL_BOOT_BCMSIM || SAL_BOOT_PLISIM)) {
            BCM_IF_ERROR_RETURN(WRITE_DYNAMIC_PORT_RECFG_VECTOR_CFG_29_00r(
                        unit, 0));
        }

        try_count = 0;
        do {
            soc_timeout_init(&to, timeout_usec, min_polls);
            if(soc_timeout_check(&to)) {
                LOG_ERROR(BSL_LS_BCM_PORT,
                        (BSL_META_U(unit,
                                    "%s:%d:TimeOut InternalError\n"),
                         FUNCTION_NAME(),__LINE__));
                return BCM_E_INTERNAL;
            }
            try_count++;

            BCM_IF_ERROR_RETURN(READ_DYNAMIC_PORT_RECFG_VECTOR_CFG_29_00r(
                        unit, &rval1));
            if (rval1 == 0 ) {
                break;
            }
        } while (try_count != 5);

        if (try_count == 5) {
                LOG_ERROR(BSL_LS_BCM_PORT,
                        (BSL_META_U(unit,
                                    "%s:%d: TimeOut - Port reconfig vector\n"),
                         FUNCTION_NAME(),__LINE__));
            return BCM_E_TIMEOUT;
        }

        sal_usleep(1000);
    } /* for try_loop */

    /* Flush existing MMU credits */
    if ((block > 0) && (block < 6)) {
        READ_EDATABUF_DBG_MMU_INTF_RESETr(unit, &rval);
        soc_reg_field_set(unit, EDATABUF_DBG_MMU_INTF_RESETr,
                &rval,  mmu_intf_reset_fld[block - 1], 1);
        WRITE_EDATABUF_DBG_MMU_INTF_RESETr(unit, rval);
    } else if (block == 6) {
        loop = (port-1) % 4;
        READ_EDATABUF_DBG_MMU_INTF_RESETr(unit, &rval);
        soc_reg_field_set(unit, EDATABUF_DBG_MMU_INTF_RESETr,
                &rval,  mmu_intf_reset_fld[block + loop - 1], 1);
        WRITE_EDATABUF_DBG_MMU_INTF_RESETr(unit, rval);
    } else if (block == 0){
        WRITE_TXLP_PORT_CREDIT_RESETr(unit, port,0xf);
        for (loop = 0; loop < SB2_MAX_PORTS_PER_BLOCK; loop++) {
            sal_memset(&txlp_port_addr_map_table_entry,0,
                    sizeof(txlp_port_addr_map_table_entry_t));
            block_port=(*sb2_block_ports[unit])[block][loop];
            if (SOC_PBMP_MEMBER((PBMP_ALL(unit)), block_port)) {

                if (si->port_speed_max[port] <= 2500) {
                    end_addr = start_addr + (( 8 * 4) - 1); /* 8 cells */
                } else if(si->port_speed_max[port] <= 10000) {
                    end_addr = start_addr + (( 16 * 4) - 1);
                } else  {
                    return BCM_E_PARAM;
                }

                soc_TXLP_PORT_ADDR_MAP_TABLEm_field_set(unit,
                        &txlp_port_addr_map_table_entry,START_ADDRf,&start_addr);
                soc_TXLP_PORT_ADDR_MAP_TABLEm_field_set(unit,
                        &txlp_port_addr_map_table_entry,END_ADDRf,&end_addr);
                start_addr = end_addr+1;
            }

            BCM_IF_ERROR_RETURN(WRITE_TXLP_PORT_ADDR_MAP_TABLEm(
                        unit,SOC_INFO(unit).txlp_block[block],
                        loop, &txlp_port_addr_map_table_entry));
        } /* for loop */
    } /* if else block != 0 */

    /* Bring Edatabuf MMU intf out of reset to get fresh credits */
    if ((block > 0) && (block < 6)) {
        READ_EDATABUF_DBG_MMU_INTF_RESETr(unit, &rval);
        soc_reg_field_set(unit, EDATABUF_DBG_MMU_INTF_RESETr,
                &rval,  mmu_intf_reset_fld[block - 1], 0);
        WRITE_EDATABUF_DBG_MMU_INTF_RESETr(unit, rval);
    } else if(block == 6) {
        loop = (port-1) % 4;
        READ_EDATABUF_DBG_MMU_INTF_RESETr(unit, &rval);
        soc_reg_field_set(unit, EDATABUF_DBG_MMU_INTF_RESETr,
                &rval,  mmu_intf_reset_fld[block + loop - 1], 0);
        WRITE_EDATABUF_DBG_MMU_INTF_RESETr(unit, rval);
    } else if (block == 0) {
        WRITE_TXLP_PORT_CREDIT_RESETr(unit, port,0);
    }

    return BCM_E_NONE;
}

/* Dynamic Helper function for MMU settings based on
 * memory selection (internal/external) 
 * The memory selection is done using port config property bcmPortControlBufferExternalEnable 
 * For Threshold values refer _soc_saber2_mmu_init_helper()
 */
int _bcm_saber2_mmu_init_helper_dyn(int unit, int port, int ext_mem_enable)
{
    uint32        op_node=0, rval;
    uint32        port_max_opnodes = 0;
    uint8         port_index = 0;
    soc_pbmp_t    valid_port_cpu_pbmp;
    uint8 port_list[]={SB2_CMIC, SB2_LPBK ,
                       1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
                       11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
                       21, 22, 23, 24, 25, 26, 27, 28};
    soc_port_t    port_iter=0;
    soc_info_t    *si= &SOC_INFO(unit);
    uint32        op_node_offset=0;
    mmu_thdo_opnconfig_cell_entry_t     mmu_thdo_opnconfig_cell_entry={{0}};
    uint32        temp_val;
    uint32        queue;
    mmu_thdo_qconfig_cell_entry_t       mmu_thdo_qconfig_cell_entry={{0}};
    mmu_thdo_qoffset_cell_entry_t       mmu_thdo_qoffset_cell_entry={{0}};
    _sb2_output_port_threshold_t *output_port_threshold =
                       &_soc_sb2_mmu_intermediate_results.output_port_threshold;
    _sb2_general_info_t *general_info =
                    &_soc_sb2_mmu_intermediate_results.general_info;
#if defined (INCLUDE_L3)
    uint32 entry[SOC_MAX_MEM_FIELD_WORDS];
    int ipmc_id;
#endif

    SOC_PBMP_CLEAR (valid_port_cpu_pbmp);
    SOC_PBMP_ASSIGN(valid_port_cpu_pbmp, PBMP_ALL(unit));

    /* 2.1 Internal Buffer              */
    if (!ext_mem_enable) {
        if (SOC_PBMP_MEMBER(PBMP_EXT_MEM (unit), port)) {
            SOC_PBMP_PORT_REMOVE(PBMP_EXT_MEM (unit), port);
        } else {
            return SOC_E_NONE;
        }

        op_node = 0;
        port_max_opnodes = 0;
        for(port_index = 0, op_node = 0 ;
            port_index < COUNTOF(port_list);
            port_index++, op_node += port_max_opnodes ) {

            port_iter = port_list[port_index];
            port_max_opnodes = sal_ceil_func(si->port_num_uc_cosq[port_iter],8);

            if (!SOC_PBMP_MEMBER(valid_port_cpu_pbmp, port_iter)) {
                continue;
            }
            if (port != port_iter) {
                continue;
            }
            if (!SOC_PBMP_MEMBER(PBMP_EXT_MEM (unit), port_iter) &&
                (port_iter >= SB2_CMIC) && (port_iter <= SB2_LPBK)) {
                for (op_node_offset = 0;
                     op_node_offset < port_max_opnodes ;
                     op_node_offset++) {
                     SOC_IF_ERROR_RETURN(READ_MMU_THDO_OPNCONFIG_CELLm(unit,
                                         MEM_BLOCK_ANY,op_node + op_node_offset,
                                         &mmu_thdo_opnconfig_cell_entry));
                     /* THDO_OPNCONFIG_CELL.OPN_SHARED_LIMIT_CELL */
                     soc_mem_field32_set(unit, MMU_THDO_OPNCONFIG_CELLm,
                                         &mmu_thdo_opnconfig_cell_entry,
                                         OPN_SHARED_LIMIT_CELLf,
                                         output_port_threshold->
                                         min_grntd_tot_shr_queue_cells_int_buff);

                     /* THDO_OPNCONFIG_CELL.OP_SHARED_RESET_VALUE_CELL */
                     temp_val = output_port_threshold->
                                min_grntd_tot_shr_queue_cells_int_buff -
                                (2 * general_info->ether_mtu_cells_for_int_buff);
                     soc_mem_field32_set(unit, MMU_THDO_OPNCONFIG_CELLm,
                                         &mmu_thdo_opnconfig_cell_entry,
                                         OPN_SHARED_RESET_VALUE_CELLf, temp_val);
                     /* THDO_OPNCONFIG_CELL.PORT_LIMIT_ENABLE_CELL */
                     soc_mem_field32_set(unit, MMU_THDO_OPNCONFIG_CELLm,
                                         &mmu_thdo_opnconfig_cell_entry,
                                         PORT_LIMIT_ENABLE_CELLf, 0);
                     SOC_IF_ERROR_RETURN(WRITE_MMU_THDO_OPNCONFIG_CELLm(unit,
                                         MEM_BLOCK_ALL,op_node + op_node_offset,
                                         &mmu_thdo_opnconfig_cell_entry));
                }

                for (queue = si->port_uc_cosq_base[port_iter];
                     queue < si->port_uc_cosq_base[port_iter] +
                             si->port_num_uc_cosq[port_iter];
                     queue++) {
                     SOC_IF_ERROR_RETURN(READ_MMU_THDO_QCONFIG_CELLm(unit,
                                         MEM_BLOCK_ANY,queue,
                                         &mmu_thdo_qconfig_cell_entry));
                     /* THDO_QCONFIG_CELL.Q_MIN_CELL */
                     soc_mem_field32_set(unit, MMU_THDO_QCONFIG_CELLm,
                         &mmu_thdo_qconfig_cell_entry, Q_MIN_CELLf,
                         output_port_threshold->min_grntd_res_queue_cells_int_buff);

                     /* THDO_QCONFIG_CELL.Q_SHARED_LIMIT_CELL */
                     if (_soc_sb2_mmu_params.lossless_mode_d_c) {
                         temp_val = output_port_threshold->
                                           min_grntd_tot_shr_queue_cells_int_buff;
                     } else {
                         temp_val = _soc_sb2_mmu_params.
                             mmu_egr_queue_dyn_thd_param_baf_profile_lossy;
                     }
                     soc_mem_field32_set(unit, MMU_THDO_QCONFIG_CELLm,
                         &mmu_thdo_qconfig_cell_entry, Q_SHARED_LIMIT_CELLf,
                         temp_val);

                     /* THDO_QCONFIG_CELL.Q_LIMIT_DYNAMIC_CELL */
                     soc_mem_field32_set(unit, MMU_THDO_QCONFIG_CELLm,
                         &mmu_thdo_qconfig_cell_entry, Q_LIMIT_DYNAMIC_CELLf,
                         _soc_sb2_mmu_params.lossless_mode_d_c ? 0 : 1);

                     /* THDO_QCONFIG_CELL.Q_LIMIT_ENABLE_CELL */
                     soc_mem_field32_set(unit, MMU_THDO_QCONFIG_CELLm,
                         &mmu_thdo_qconfig_cell_entry, Q_LIMIT_ENABLE_CELLf,
                         _soc_sb2_mmu_params.lossless_mode_d_c ? 0 : 1);

                     /* THDO_QCONFIG_CELL.Q_COLOR_ENABLE_CELL = 0 */
                     soc_mem_field32_set(unit, MMU_THDO_QCONFIG_CELLm,
                         &mmu_thdo_qconfig_cell_entry, Q_COLOR_ENABLE_CELLf,0);

                     /* THDO_QCONFIG_CELL.Q_COLOR_LIMIT_DYNAMIC_CELL */
                     soc_mem_field32_set(unit, MMU_THDO_QCONFIG_CELLm,
                         &mmu_thdo_qconfig_cell_entry, Q_COLOR_LIMIT_DYNAMIC_CELLf,
                         _soc_sb2_mmu_params.lossless_mode_d_c ? 0 : 1);

                     /* THDO_QCONFIG_CELL.LIMIT_YELLOW_CELL */
                     temp_val = soc_mem_field32_get(unit, MMU_THDO_QCONFIG_CELLm,
                                &mmu_thdo_qconfig_cell_entry, Q_SHARED_LIMIT_CELLf);
                     soc_mem_field32_set(unit, MMU_THDO_QCONFIG_CELLm,
                         &mmu_thdo_qconfig_cell_entry, LIMIT_YELLOW_CELLf,
                         _soc_sb2_mmu_params.lossless_mode_d_c ?
                         sal_ceil_func(temp_val, 8) : temp_val);
                     SOC_IF_ERROR_RETURN(WRITE_MMU_THDO_QCONFIG_CELLm(unit,
                                         MEM_BLOCK_ALL,queue,
                                         &mmu_thdo_qconfig_cell_entry));

                     SOC_IF_ERROR_RETURN(READ_MMU_THDO_QOFFSET_CELLm(unit,
                                         MEM_BLOCK_ANY,queue,
                                         &mmu_thdo_qoffset_cell_entry));

                     /* THDO_QOFFSET_CELL.RESET_OFFSET_CELL */
                     soc_mem_field32_set(unit, MMU_THDO_QOFFSET_CELLm,
                         &mmu_thdo_qoffset_cell_entry, RESET_OFFSET_CELLf, 2);

                     /* THDO_QOFFSET_CELL.LIMIT_RED_CELL */
                     temp_val = soc_mem_field32_get(unit, MMU_THDO_QCONFIG_CELLm,
                                &mmu_thdo_qconfig_cell_entry, Q_SHARED_LIMIT_CELLf);
                     soc_mem_field32_set(unit, MMU_THDO_QOFFSET_CELLm,
                         &mmu_thdo_qoffset_cell_entry, LIMIT_RED_CELLf,
                         _soc_sb2_mmu_params.lossless_mode_d_c ?
                     sal_ceil_func(temp_val, 8) : temp_val);

                     /* THDO_QOFFSET_CELL.RESET_OFFSET_YELLOW_CELL */
                     soc_mem_field32_set(unit, MMU_THDO_QOFFSET_CELLm,
                         &mmu_thdo_qoffset_cell_entry, RESET_OFFSET_YELLOW_CELLf,2);
                     /* THDO_QOFFSET_CELL.RESET_OFFSET_RED_CELL */
                     soc_mem_field32_set(unit, MMU_THDO_QOFFSET_CELLm,
                         &mmu_thdo_qoffset_cell_entry, RESET_OFFSET_RED_CELLf,2);

                     SOC_IF_ERROR_RETURN(WRITE_MMU_THDO_QOFFSET_CELLm(unit,
                                         MEM_BLOCK_ALL,queue,
                                     &mmu_thdo_qoffset_cell_entry));
                }
            }
        }

        SOC_IF_ERROR_RETURN(_bcm_saber2_mmu_credit_reset_ext_mem(unit, port)) ;
        SOC_IF_ERROR_RETURN(READ_RQE_PP_PORT_CONFIGr(unit, port, &rval));
        soc_reg_field_set(unit, RQE_PP_PORT_CONFIGr, &rval, BUFF_TYPEf, 0);
        SOC_IF_ERROR_RETURN(WRITE_RQE_PP_PORT_CONFIGr(unit, port, rval));
    } else {
        /* 2.2 External Buffer */
        if (SOC_PBMP_MEMBER(PBMP_EXT_MEM (unit), port)) {
            return SOC_E_NONE;
        }
        SOC_PBMP_PORT_ADD(PBMP_EXT_MEM (unit), port);
        for(port_index = 0, op_node = 0 ;
            port_index < COUNTOF(port_list);
            port_index++, op_node += port_max_opnodes ) {

            port_iter = port_list[port_index];
            port_max_opnodes = sal_ceil_func(si->port_num_uc_cosq[port_iter],8);

            if (!SOC_PBMP_MEMBER(valid_port_cpu_pbmp, port_iter)) {
                continue;
            }
            if (port != port_iter) {
                continue;
            }

            if (!IS_CPU_PORT(unit, port_iter) &&
                (port_iter > SB2_CMIC) && (port_iter <= SB2_LPBK) &&
                SOC_PBMP_MEMBER(PBMP_EXT_MEM (unit), port_iter)) {

                SOC_IF_ERROR_RETURN(READ_MMU_THDO_OPNCONFIG_CELLm(unit,
                    MEM_BLOCK_ANY,op_node , &mmu_thdo_opnconfig_cell_entry));

                /* THDO_OPNCONFIG_CELL.OPN_SHARED_LIMIT_CELL */
                soc_mem_field32_set(unit, MMU_THDO_OPNCONFIG_CELLm,
                                    &mmu_thdo_opnconfig_cell_entry,
                                    OPN_SHARED_LIMIT_CELLf,
                                    output_port_threshold->
                                    min_grntd_tot_shr_queue_cells_ext_buff);

                 /* THDO_OPNCONFIG_CELL.OP_SHARED_RESET_VALUE_CELL */
                temp_val = output_port_threshold->
                    min_grntd_tot_shr_queue_cells_ext_buff -
                    (2 * general_info->ether_mtu_cells_for_ext_buff);
                soc_mem_field32_set(unit, MMU_THDO_OPNCONFIG_CELLm,
                                    &mmu_thdo_opnconfig_cell_entry,
                                    OPN_SHARED_RESET_VALUE_CELLf, temp_val);

                /* THDO_OPNCONFIG_CELL.PORT_LIMIT_ENABLE_CELL */
                soc_mem_field32_set(unit, MMU_THDO_OPNCONFIG_CELLm,
                                    &mmu_thdo_opnconfig_cell_entry,
                                    PORT_LIMIT_ENABLE_CELLf, 0);
                SOC_IF_ERROR_RETURN(WRITE_MMU_THDO_OPNCONFIG_CELLm(unit,
                                    MEM_BLOCK_ALL,op_node,
                                    &mmu_thdo_opnconfig_cell_entry));

                for (queue = si->port_uc_cosq_base[port_iter];
                     queue < si->port_uc_cosq_base[port_iter] +
                             si->port_num_uc_cosq[port_iter];
                     queue++) {
                     SOC_IF_ERROR_RETURN(READ_MMU_THDO_QCONFIG_CELLm(unit,
                                         MEM_BLOCK_ANY,queue,
                                         &mmu_thdo_qconfig_cell_entry));
                     /* THDO_QCONFIG_CELL.Q_MIN_CELL */
                     soc_mem_field32_set(unit, MMU_THDO_QCONFIG_CELLm,
                         &mmu_thdo_qconfig_cell_entry, Q_MIN_CELLf,
                         output_port_threshold->min_grntd_res_queue_cells_ext_buff);

                     /* THDO_QCONFIG_CELL.Q_SHARED_LIMIT_CELL */
                     if (_soc_sb2_mmu_params.lossless_mode_d_c) {
                         temp_val = output_port_threshold->
                                           min_grntd_tot_shr_queue_cells_ext_buff;
                     } else {
                         temp_val = _soc_sb2_mmu_params.
                             mmu_egr_queue_dyn_thd_param_baf_profile_lossy;
                     }
                     soc_mem_field32_set(unit, MMU_THDO_QCONFIG_CELLm,
                         &mmu_thdo_qconfig_cell_entry, Q_SHARED_LIMIT_CELLf,
                         temp_val);

                     /* THDO_QCONFIG_CELL.Q_LIMIT_DYNAMIC_CELL */
                     soc_mem_field32_set(unit, MMU_THDO_QCONFIG_CELLm,
                         &mmu_thdo_qconfig_cell_entry, Q_LIMIT_DYNAMIC_CELLf,
                         _soc_sb2_mmu_params.lossless_mode_d_c ? 0 : 1);

                     /* THDO_QCONFIG_CELL.Q_LIMIT_ENABLE_CELL */
                     soc_mem_field32_set(unit, MMU_THDO_QCONFIG_CELLm,
                         &mmu_thdo_qconfig_cell_entry, Q_LIMIT_ENABLE_CELLf,
                         _soc_sb2_mmu_params.lossless_mode_d_c ? 0 : 1);

                     /* THDO_QCONFIG_CELL.Q_COLOR_ENABLE_CELL */
                     soc_mem_field32_set(unit, MMU_THDO_QCONFIG_CELLm,
                         &mmu_thdo_qconfig_cell_entry, Q_COLOR_ENABLE_CELLf,0);

                     /* THDO_QCONFIG_CELL.Q_COLOR_LIMIT_DYNAMIC_CELL */
                     soc_mem_field32_set(unit, MMU_THDO_QCONFIG_CELLm,
                         &mmu_thdo_qconfig_cell_entry, Q_COLOR_LIMIT_DYNAMIC_CELLf,
                         _soc_sb2_mmu_params.lossless_mode_d_c ? 0 : 1);

                     /* THDO_QCONFIG_CELL.LIMIT_YELLOW_CELL */
                     temp_val = soc_mem_field32_get(unit, MMU_THDO_QCONFIG_CELLm,
                                &mmu_thdo_qconfig_cell_entry, Q_SHARED_LIMIT_CELLf);
                     soc_mem_field32_set(unit, MMU_THDO_QCONFIG_CELLm,
                         &mmu_thdo_qconfig_cell_entry, LIMIT_YELLOW_CELLf,
                         _soc_sb2_mmu_params.lossless_mode_d_c ?
                         sal_ceil_func(temp_val, 8) : temp_val);
                     SOC_IF_ERROR_RETURN(WRITE_MMU_THDO_QCONFIG_CELLm(unit,
                                         MEM_BLOCK_ALL,queue,
                                         &mmu_thdo_qconfig_cell_entry));

                     SOC_IF_ERROR_RETURN(READ_MMU_THDO_QOFFSET_CELLm(unit,
                                         MEM_BLOCK_ANY,queue,
                                         &mmu_thdo_qoffset_cell_entry));
                     /* THDO_QOFFSET_CELL.RESET_OFFSET_CELL */
                     soc_mem_field32_set(unit, MMU_THDO_QOFFSET_CELLm,
                         &mmu_thdo_qoffset_cell_entry, RESET_OFFSET_CELLf, 2);

                     /* THDO_QOFFSET_CELL.LIMIT_RED_CELL */ 
                     temp_val = soc_mem_field32_get(unit, MMU_THDO_QCONFIG_CELLm,
                                &mmu_thdo_qconfig_cell_entry, Q_SHARED_LIMIT_CELLf);
                     soc_mem_field32_set(unit, MMU_THDO_QOFFSET_CELLm,
                         &mmu_thdo_qoffset_cell_entry, LIMIT_RED_CELLf,
                         _soc_sb2_mmu_params.lossless_mode_d_c ?
                         sal_ceil_func(temp_val, 8) : temp_val);

                     /* THDO_QOFFSET_CELL.RESET_OFFSET_YELLOW_CELL */
                     soc_mem_field32_set(unit, MMU_THDO_QOFFSET_CELLm,
                         &mmu_thdo_qoffset_cell_entry, RESET_OFFSET_YELLOW_CELLf,2);
                     /* THDO_QOFFSET_CELL.RESET_OFFSET_RED_CELL */
                     soc_mem_field32_set(unit, MMU_THDO_QOFFSET_CELLm,
                         &mmu_thdo_qoffset_cell_entry, RESET_OFFSET_RED_CELLf,2);

                     SOC_IF_ERROR_RETURN(WRITE_MMU_THDO_QOFFSET_CELLm(unit,
                                         MEM_BLOCK_ALL,queue,
                                         &mmu_thdo_qoffset_cell_entry));
                }
            }
        }

        SOC_IF_ERROR_RETURN(_bcm_saber2_mmu_credit_reset_ext_mem(unit, port)) ;
        SOC_IF_ERROR_RETURN(READ_RQE_PP_PORT_CONFIGr(unit, port, &rval));
        soc_reg_field_set(unit, RQE_PP_PORT_CONFIGr, &rval, BUFF_TYPEf, 1);
        SOC_IF_ERROR_RETURN(WRITE_RQE_PP_PORT_CONFIGr(unit, port, rval));
    }

#if defined (INCLUDE_L3)
    /* IPMC configuration */
    soc_mem_lock(unit, L3_IPMCm);
    for (ipmc_id = soc_mem_index_min(unit, L3_IPMCm);
             ipmc_id <= soc_mem_index_max(unit, L3_IPMCm);
             ipmc_id++) {
        if (_bcm_sb2_ipmc_port_ext_queue_count(unit, ipmc_id, port)) {
            BCM_IF_ERROR_RETURN(soc_mem_read(unit, L3_IPMCm, MEM_BLOCK_ANY,
                                             ipmc_id, entry));
            soc_mem_field32_set(unit, L3_IPMCm, entry, EXT_Q_INTERNALf, 0);
            soc_mem_field32_set(unit, L3_IPMCm, entry, EXT_Q_EXTERNALf, 0);
            if (!ext_mem_enable) {
                SOC_PBMP_ITER (PBMP_EXT_MEM (unit), port_iter) {
                    if (port_iter < SB2_LPBK) {
                        if (_bcm_sb2_ipmc_port_ext_queue_count(
                                    unit, ipmc_id, port_iter))  {
                            soc_mem_field32_set(unit, L3_IPMCm, entry,
                                    EXT_Q_EXTERNALf, 1);
                        }
                    }
                }
                soc_mem_field32_set(unit, L3_IPMCm, entry, EXT_Q_INTERNALf, 1);
            } else {
                PBMP_ITER(PBMP_PORT_ALL(unit), port_iter){
                   if (port_iter < SB2_LPBK) {
                       if (!SOC_PBMP_MEMBER(PBMP_EXT_MEM (unit), port_iter) &&
                              _bcm_sb2_ipmc_port_ext_queue_count(
                                  unit, ipmc_id, port_iter)) {
                           soc_mem_field32_set(unit, L3_IPMCm, entry,
                                   EXT_Q_INTERNALf, 1);
                       }
                   }
                }
                soc_mem_field32_set(unit, L3_IPMCm, entry, EXT_Q_EXTERNALf, 1);
            }
            BCM_IF_ERROR_RETURN(soc_mem_write(unit, L3_IPMCm,
                                      MEM_BLOCK_ALL, ipmc_id, &entry));
            BCM_IF_ERROR_RETURN(_bcm_kt2_ipmc_subscriber_egress_queue_ext_set
                                (unit, port, ipmc_id, ext_mem_enable));
        }
    }
    soc_mem_unlock(unit, L3_IPMCm);
#endif
    return BCM_E_NONE;
}

#endif
