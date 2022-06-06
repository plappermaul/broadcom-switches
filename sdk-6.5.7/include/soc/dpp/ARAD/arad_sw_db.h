/* $Id: arad_sw_db.h,v 1.102 Broadcom SDK $
* $Copyright: (c) 2016 Broadcom.
* Broadcom Proprietary and Confidential. All rights reserved.$
 * $Copyright
 * $
*/


#ifndef _ARAD_SW_DB_H_
/* { */
#define _ARAD_SW_DB_H_

/*************
 * INCLUDES  *
 *************/
/* { */

#include <shared/swstate/sw_state.h>
#include <soc/dpp/SAND/Utils/sand_header.h>


#include <soc/dpp/multicast.h>
#include <soc/dpp/multicast_imp.h>
#include <soc/dpp/ARAD/arad_api_general.h>
#include <soc/dpp/ARAD/arad_api_end2end_scheduler.h>
#include <soc/dpp/ARAD/arad_api_ingress_packet_queuing.h>
#include <soc/dpp/ARAD/arad_ofp_rates.h>
#include <soc/dpp/ARAD/arad_api_mgmt.h>
#include <soc/dpp/ARAD/arad_api_end2end_scheduler.h>
#include <soc/dpp/ARAD/arad_egr_queuing.h>
#include <soc/dpp/ARAD/arad_egr_prog_editor.h>
#include <soc/dpp/ARAD/arad_ports.h>
#include <soc/dpp/ARAD/arad_pmf_low_level.h>
#include <soc/dpp/ARAD/arad_pmf_low_level_fem_tag.h>
#include <soc/dpp/ARAD/arad_pmf_prog_select.h>
#include <soc/dpp/ARAD/arad_tcam.h>
#include <soc/dpp/ARAD/arad_tdm.h>
#include <soc/dpp/ARAD/arad_cell.h>
#include <soc/dpp/ARAD/arad_dram.h>
#include <soc/dpp/ARAD/ARAD_PP/arad_pp_l3_src_bind.h>
#include <soc/dpp/SAND/Utils/sand_occupation_bitmap.h>
#include <soc/dpp/SAND/Utils/sand_hashtable.h>
#include <soc/dpp/SAND/Utils/sand_hashtable.h>
#include <soc/dpp/SAND/Utils/sand_sorted_list.h>
#include <soc/dpp/ARAD/arad_tcam_mgmt.h>
#include <soc/dpp/ARAD/ARAD_PP/arad_pp_fp_key.h>
#include <soc/dpp/ARAD/ARAD_PP/arad_pp_fp_fem.h>
#include <soc/dpp/ARAD/arad_ingress_traffic_mgmt.h>

#include <soc/dpp/SAND/Utils/sand_multi_set.h>

#include <soc/dpp/ARAD/arad_sim_em_block.h>
#include <soc/dpp/ARAD/arad_interrupts.h>
#include <soc/dpp/ARAD/arad_mgmt.h>
#include <soc/dpp/JER/jer_fabric.h>

#if defined(INCLUDE_KBP) && !defined(BCM_88030)
#include <soc/kbp/alg_kbp/include/db.h>
#include <soc/dpp/ARAD/arad_kbp.h>
#endif /* defined(INCLUDE_KBP) && !defined(BCM_88030) */

/* } */

/*************
 * DEFINES   *
 *************/
/* { */


typedef uint16 ARAD_SYSPORT; /* type for system physical port */
#define ARAD_NOF_MODPORT (ARAD_NOF_FAPS_IN_SYSTEM*ARAD_NOF_FAP_PORTS) /* number of devices x number of device ports 2048*256=512K */
#define ARAD_MODPORT2SYSPORT_SIZE (sizeof(ARAD_SYSPORT[ARAD_NOF_MODPORT]))
#define ARAD_SW_DB_MODPORT2SYSPORT_REVERSE_GET_NOT_FOUND ((uint32)(-1)) /* return value of arad_sw_db_modport2sysport_reverse_get when it fails the search */
#define ARAD_SW_DB_SYSPORT2MODPORT_INVALID_ID ((uint32)(-1)) /* return value of arad_sw_db_sysport2modport_get when it fails the search */
#define ARAD_SW_DB_MODPORT2SYSPORT_INVALID_SYSPORT 0xffff /*Mark invalid sysport in the SW DB by using all-ones for ARAD_SYSPORT types - uint16*/


/* number of keys in program in cycle */
#define ARAD_SW_DB_PMF_NOF_KEYS                 (ARAD_PMF_LOW_LEVEL_PMF_KEY_MAX_ALL_STAGES + 1)
#define SOC_DPP_DEFS_GET_TCAM_NOF_LINES_IN_BYTES(unit)     ((SOC_DPP_DEFS_GET(unit, nof_tcam_big_bank_lines)+7)/8)

/* Hash table properties */
#define ARAD_TCAM_DB_HASH_TBL_KEY_SIZE                           (sizeof(uint32) /* for entry */+ sizeof(uint16) /* for DB */)
#define ARAD_TCAM_DB_HASH_TBL_DATA_SIZE                          (sizeof(uint32) /* for hash-id */)

#define ARAD_SW_DB_NOF_QUEUE_TYPES(unit) SOC_DPP_DEFS_GET(unit, nof_credit_request_profiles)

#ifdef BCM_JERICHO_SUPPORT
#define ARAD_SW_DB_NOF_DYNAMIC_QUEUE_TYPES (SOC_DPP_DEFS_MAX(NOF_CREDIT_REQUEST_PROFILES) - SOC_TMC_ITM_NOF_QT_STATIC)
#define ARAD_SW_DB_NOF_LEGAL_DYNAMIC_QUEUE_TYPES(unit) (SOC_IS_JERICHO(unit) ? ARAD_SW_DB_NOF_DYNAMIC_QUEUE_TYPES : 13)
#else
#define ARAD_SW_DB_NOF_DYNAMIC_QUEUE_TYPES 13
#define ARAD_SW_DB_NOF_LEGAL_DYNAMIC_QUEUE_TYPES(unit) ARAD_SW_DB_NOF_DYNAMIC_QUEUE_TYPES
#endif

#define ARAD_SW_DB_QUEUE_TYPE_NOT_AVAILABLE 255

/*ingress rate class mode representing undifed mode*/
#define ARAD_SW_DB_QUEUE_TO_RATE_CLASS_MAPPING_IS_ADVANCED 0x0
#define ARAD_SW_DB_QUEUE_TO_RATE_CLASS_MAPPING_IS_SIMPLE 0x1
#define ARAD_SW_DB_QUEUE_TO_RATE_CLASS_MAPPING_IS_UNDEFINED 0x2

#define ARAD_SW_DB_CORE_ANY (-1)
#define ARAD_SW_DB_CORE_0	(0)
#define ARAD_SW_DB_CORE_1	(1)
/* } */

/*************
 * GLOBALS   *
 *************/


/*************
 *  MACROS   *
 *************/
/* { */

#define ARAD_SW_DB_INIT_DEFS                    \
    uint32                                    \
    soc_sand_ret = SOC_SAND_OK

#define ARAD_SW_DB_FIELD_SET(res, unit, field_name, val)           \
    if (Arad_sw_db.arad_device_sw_db[unit] == NULL)                \
    {                                                                   \
        SOC_SAND_SET_ERROR_CODE(SOC_SAND_ERR, 1, exit);                         \
    }                                                                   \
    res = soc_sand_os_memcpy(&(Arad_sw_db.arad_device_sw_db[unit]->field_name), \
                         val,                                           \
                         sizeof(*val)                                   \
                         );                                             \
    SOC_SAND_CHECK_FUNC_RESULT(res, 2, exit);


#define ARAD_SW_DB_FIELD_GET(unit, field_name, val)         \
  if (Arad_sw_db.arad_device_sw_db[unit] == NULL)          \
  {                                                               \
    return SOC_SAND_ERR;                                              \
  }                                                               \
  soc_sand_ret = soc_sand_os_memcpy(                                      \
      val,                                                        \
      &(Arad_sw_db.arad_device_sw_db[unit]->field_name),   \
      sizeof(*val)                                                \
    );                                                            \
  return soc_sand_ret;

#define ARAD_SW_DB_GLOBAL_FIELD_SET(field_name, val)             \
  uint32                                                        \
    soc_sand_ret = SOC_SAND_OK;                                           \
  soc_sand_ret = soc_sand_os_memcpy(                                      \
      &(Arad_sw_db.field_name),                                  \
      val,                                                        \
      sizeof(*val)                                                \
    );                                                            \
                                                                  \
  return soc_sand_ret;

#define ARAD_SW_DB_GLOBAL_FIELD_GET(field_name, val)             \
  uint32                                                        \
    soc_sand_ret = SOC_SAND_OK;                                           \
  soc_sand_ret = soc_sand_os_memcpy(                                      \
      val,                                                        \
      &(Arad_sw_db.field_name),                                  \
      sizeof(*val)                                                \
    );                                                            \
                                                                  \
  return soc_sand_ret;

/*************
 * TYPE DEFS *
 *************/
/* { */
typedef struct
{
  uint8
    valid;
  /* EGQ rate shaper */
  uint32
    egq_rates;
  /* EGQ burst max */
  uint32
    egq_bursts;
} ARAD_SW_DB_DEV_RATE;

typedef struct
{
  /* 
   * EGQ rate for each TCG entity. Summerize by NOF_PS * NOF_TCGS_IN_PS
   */
  ARAD_SW_DB_DEV_RATE
    tcg_rate[ARAD_EGR_NOF_PS][ARAD_NOF_TCGS];

} ARAD_SW_DB_DEV_EGR_TCG;

typedef struct
{
  /* 
   * EGQ rate for each Q-Pair
   */
  ARAD_SW_DB_DEV_RATE
    queue_rate[ARAD_EGR_NOF_Q_PAIRS];

} ARAD_SW_DB_DEV_EGR_PORT_PRIORITY;

typedef struct
{
  uint8
    valid;
  uint32
    sch_rates;
  uint32
    egq_rates;
  uint32
    egq_bursts;
}  ARAD_SW_DB_DEV_EGR_RATE;


typedef struct
{
    int
        priority_shaper_rate;
    uint8
        valid;

} ARAD_SW_DB_DEV_EGR_SCH_PORT_PRIORITY_SHAPER;  

typedef struct
{
    int
        tcg_shaper_rate;
    uint8
        valid;
    

} ARAD_SW_DB_DEV_EGR_SCH_TCG_SHAPER;  

typedef struct 
{
  uint32
    nof_calcal_instances;

} ARAD_SW_DB_DEV_EGR_CHAN_ARB;

typedef struct 
{
 /* 
  * Indicates if each (egress) local port has a reassembly context reserved for it
  * for a non mirroring application. 1 means reserved.
  */
  uint32
    port_reserved_reassembly_context[SOC_DPP_DEFS_MAX(NOF_LOGICAL_PORTS) / SOC_SAND_NOF_BITS_IN_UINT32];

  /*  
   * Calendar information of Port-Priority
   */
  ARAD_SW_DB_DEV_EGR_PORT_PRIORITY
    port_priority_cal[SOC_DPP_DEFS_MAX(NOF_CORES)];

  /* 
   * Calendar information of TCG
   */
  ARAD_SW_DB_DEV_EGR_TCG
    tcg_cal[SOC_DPP_DEFS_MAX(NOF_CORES)];

  ARAD_SW_DB_DEV_EGR_RATE
    rates[SOC_DPP_DEFS_MAX(NOF_CORES)][ARAD_EGR_NOF_BASE_Q_PAIRS];

  ARAD_SW_DB_DEV_EGR_SCH_PORT_PRIORITY_SHAPER
    port_priority[SOC_DPP_DEFS_MAX(NOF_CORES)][ARAD_EGR_NOF_BASE_Q_PAIRS];

  ARAD_SW_DB_DEV_EGR_CHAN_ARB
    chan_arb[SOC_DPP_DEFS_MAX(NOF_CORES)][SOC_DPP_DEFS_MAX(NOF_CHANNELIZED_CALENDARS)];

  uint32
    calcal_length[SOC_DPP_DEFS_MAX(NOF_CORES)];

  /* 
   * Disable / Enable EGQ TCG and QPair shapers
   */
  uint8
    egq_tcg_qpair_shaper_enable;

  /* 
   * ERP interface ID (taken from NIF avaiable interface)
   */
  ARAD_INTERFACE_ID
    erp_interface_id[SOC_DPP_DEFS_MAX(NOF_CORES)];

  /* 
  * Temp. field: 
  * Save the information of programming profile per port.
  */
  ARAD_EGR_PROG_TM_PORT_PROFILE
    ports_prog_editor_profile[ARAD_NOF_FAP_PORTS];

  ARAD_SW_DB_DEV_EGR_SCH_TCG_SHAPER
    tcg_shaper[SOC_DPP_DEFS_MAX(NOF_CORES)][ARAD_EGR_NOF_BASE_Q_PAIRS];

  /* 
   * Bimtap occupation to allocate channelize arbiter for requested interface
   */
  SOC_SAND_OCC_BM_PTR
    chanif2chan_arb_occ;
  /* 
   * Bimtap occupation to allocate non channelize arbiter for requested scheduler interfaces
   */
  SOC_SAND_OCC_BM_PTR
    nonchanif2sch_offset_occ;

  /*
   * Bitmap occupation to allocate channelized calendars
   */
  SOC_SAND_OCC_BM_PTR
    channelized_cals_occ[SOC_DPP_DEFS_MAX(NOF_CORES)];

  /*
   * indicates which calendars were modified (which cals should be recalculated) 
   */
  SOC_SAND_OCC_BM_PTR
    modified_channelized_cals_occ[SOC_DPP_DEFS_MAX(NOF_CORES)];

  /*
   * Bitmap occupation to allocate e2e interfaces
   */
  SOC_SAND_OCC_BM_PTR
    e2e_interfaces_occ[SOC_DPP_DEFS_MAX(NOF_CORES)];

  /*
   * indicates which e2e interfaces were modified (which cals should be recalculated) 
   */
  SOC_SAND_OCC_BM_PTR
    modified_e2e_interfaces_occ[SOC_DPP_DEFS_MAX(NOF_CORES)];

} ARAD_DEV_EGR_PORTS;

/* } */

typedef struct
{
  /*
   *  If TRUE, soc_petrab devices exist in the system. This imposes
   *  certain limitations on Arad behavior (e.g. TDM optimize
   *  cells must be fixed size etc.).
   */
  uint8 is_petrab_in_system;
  /*
   * TDM traffic mode and FTMH format configuration
   */
  ARAD_MGMT_TDM_MODE tdm_mode;
  /*
   * ILKN TDM SP mode - does tdm get presedence over regular packets 
   */
  ARAD_MGMT_ILKN_TDM_DEDICATED_QUEUING_MODE ilkn_tdm_dedicated_queuing;

  /* 
   * Multicast table mode
   */

}  ARAD_SW_DB_OP_MODE;

typedef struct
{
 /*
  *  to identify whether the size of the LAG is zero or one.
  */
  uint8
    in_use[ARAD_NOF_LAG_GROUPS_MAX];

  /* 
   * Specify port mapping between ingress port index (0-252) to reassembly context (0-191)
   */
  uint32
    local_to_reassembly_context[ARAD_NOF_LOCAL_PORTS_MAX];

}  ARAD_LAGS_INFO;

typedef struct
{

 /* 
  * mapped fap id
  */
  uint32 fap_id;
 /*
  *  mapped fap port
  */
  uint32 fap_port_id;

}  ARAD_MODPORT_INFO;

typedef struct
{
  /*  Buffers and index for FIFO read results. */
  uint32 *host_buff[SOC_TMC_CNT_NOF_PROCESSOR_IDS_ARAD]; 
  uint32 buff_line_ndx[SOC_TMC_CNT_NOF_PROCESSOR_IDS_ARAD]; 
}  ARAD_SW_DB_CNT;
/*
 *  SW-DB
 */

typedef enum
{
  ARAD_NOF_SW_DB_MULTI_SETS
} ARAD_SW_DB_MULTI_SET;

typedef struct
{
  uint8             valid;
  uint32             min_banks;
  ARAD_TCAM_BANK_OWNER bank_owner;
  ARAD_TCAM_MGMT_SIGNAL   callback;
  uint32              user_data;
} ARAD_SW_DB_TCAM_ACCESS_PROFILE;

typedef struct
{
  uint32 prefix_db[ARAD_TCAM_NOF_PREFIXES];  
  uint32 nof_dbs;
} ARAD_SW_DB_TCAM_MANAGED_BANK;

typedef struct
{ 
  ARAD_SW_DB_TCAM_ACCESS_PROFILE profiles[ARAD_TCAM_NOF_ACCESS_PROFILE_IDS]; 
  ARAD_SW_DB_TCAM_MANAGED_BANK   banks[SOC_DPP_DEFS_MAX_TCAM_NOF_BANKS];
} ARAD_SW_DB_TCAM_MGMT;

typedef struct
{
  uint32 vsi_to_isid[32*1024];
} ARAD_SW_DB_VSI;

typedef struct
{
    uint8  is_simple_mode;
    uint32 ref_count[SOC_DPP_DEFS_MAX(NOF_CORES)][SOC_TMC_ITM_NOF_RATE_CLASSES]; /*Number of queues pointing to rate class at offset i*/
    uint32 ocb_only_ref_count[SOC_DPP_DEFS_MAX(NOF_CORES)][SOC_TMC_ITM_NOF_RATE_CLASSES]; /*Number of OCB-only queues pointing to rate class at offset i*/
} ARAD_SW_DB_QUEUE_TO_RATE_CLASS_MAPPING;

typedef struct 
{
    uint8  valid_flags;
    uint32 base_queue;
} ARAD_SW_DB_SYSPORT_TO_BASE_QUEUE;


/* supported values for the credit_watchdog_mode field, must be the same as in include/bcm/fabric.h */
#define CREDIT_WATCHDOG_UNINITIALIZED 0 /* BCM_FABRIC_WATCHDOG_QUEUE_DISABLE */
/* BCM_FABRIC_WATCHDOG_QUEUE_ENABLE_NORMAL the default mode supporting both fsm and aging (queue deletion) */
#define CREDIT_WATCHDOG_NORMAL_MODE 1
/* BCM_FABRIC_WATCHDOG_QUEUE_ENABLE_FAST_STATUS_MESSAGE support aggressive (small)
   watchdog status message thresholds and not support aging (queue deletion) */
#define CREDIT_WATCHDOG_FAST_STATUS_MESSAGE_MODE 2
/* BCM_FABRIC_WATCHDOG_QUEUE_ENABLE_COMMON_STATUS_MESSAGE Arad+ mode whic common FSM time for the chip */
#define CREDIT_WATCHDOG_COMMON_STATUS_MESSAGE_MODE 3

typedef struct 
{
    /* 
     * supported values are: 
     *  CREDIT_WATCHDOG_AGGRESSIVE_STATUS_MSG_MODE,
     *  CREDIT_WATCHDOG_NORMAL, 
     *  CREDIT_WATCHDOG_COMMON_STATUS_MESSAGE_MODE, 
     *  CREDIT_WATCHDOG_COMMON_STATUS_MESSAGE_MODE+1, The last value means generate a message after two scans
     */ 
    int8 credit_watchdog_mode;
    /* exact credit watchdog scan time in nano seconds */
    uint32 exact_credit_watchdog_scan_time_nano;
} ARAD_SW_DB_CREDIT_WATCHDOG;

typedef struct
{
  uint8  is_power_saving_called;
  ARAD_SW_DB_QUEUE_TO_RATE_CLASS_MAPPING queue_to_rate_class_mapping;
  PARSER_HINT_ARR ARAD_SW_DB_SYSPORT_TO_BASE_QUEUE* sysport2basequeue;
  PARSER_HINT_ARR ARAD_SYSPORT*                     queuequartet2sysport;
  int groups_bw[SOC_DPP_DEFS_MAX(NOF_CORES)][ARAD_SCH_NOF_GROUPS];
  int hr_group_bw[SOC_DPP_DEFS_MAX(NOF_CORES)][ARAD_EGR_NOF_BASE_Q_PAIRS];
  int rcy_single_context_port[SOC_DPP_DEFS_MAX(NOF_CORES)][SOC_DPP_IMP_DEFS_MAX(NOF_CORE_INTERFACES)];
  uint32 rcy_reassembly_ctxt[SOC_DPP_DEFS_MAX(NOF_CORES)][SOC_DPP_IMP_DEFS_MAX(NOF_CORE_INTERFACES)];
  int rcy_channels_to_egr_nif_mapping[SOC_DPP_DEFS_MAX(NOF_CORES)][SOC_DPP_MAX_NOF_CHANNELS];
  uint8 pg_numq[SOC_DPP_DEFS_MAX(NOF_CORES)][SOC_DPP_DEFS_MAX(NOF_VSQ_F)];
  ARAD_SW_DB_CREDIT_WATCHDOG credit_watchdog;
  SOC_TMC_SCH_FLOW_IPF_CONFIG_MODE ipf_config_mode;
}ARAD_SW_DB_TM;


typedef struct
{
  ARAD_SW_DB_CNT
    cnt;
}  ARAD_SW_DB_DEVICE;

/* NEW SW STATE STRUCT */
typedef struct soc_arad_sw_state_tm_s{
    PARSER_HINT_PTR ARAD_SW_DB_OP_MODE         *op_mode;
    PARSER_HINT_PTR JER_MODID_GROUP_MAP        *jer_modid_group_map;
    PARSER_HINT_PTR soc_dpp_config_arad_plus_t *arad_plus;
    PARSER_HINT_PTR ARAD_SW_DB_TM              *tm_info;
    PARSER_HINT_PTR ARAD_SW_DB_TCAM_MGMT       *tcam_mgmt;
    PARSER_HINT_PTR     ARAD_TCAM              *tcam;
    PARSER_HINT_ARR ARAD_PMF                   *pmf;
    PARSER_HINT_PTR ARAD_DEV_EGR_PORTS         *arad_egr_ports;
#if defined(INCLUDE_KBP) && !defined(BCM_88030)
    PARSER_HINT_PTR PARSER_HINT_ALLOW_WB_ACCESS ARAD_KBP_INFO              *kbp_info;
#endif /* defined(INCLUDE_KBP) && !defined(BCM_88030) */
    PARSER_HINT_PTR ARAD_LAGS_INFO             *lag;
    PARSER_HINT_PTR ARAD_CELL                  *cell;
    PARSER_HINT_PTR ARAD_TDM                   *tdm;
    PARSER_HINT_PTR ARAD_DRAM                  *dram;
    PARSER_HINT_PTR ARAD_MULTICAST             *arad_multicast;
    PARSER_HINT_ARR uint32                     *q_type_ref_count;
    /* maps user defined queue types to (the smaller amount of) hardware types (credit request profiled) */
    PARSER_HINT_ARR uint8                      *q_type_map;
    PARSER_HINT_PTR ARAD_SW_DB_VSI             *vsi;
    /* Array representing the modport to sysport mapping */
    PARSER_HINT_ARR ARAD_SYSPORT               *modport2sysport;
    /* Array representing the sysport to modport mapping */
    PARSER_HINT_ARR ARAD_MODPORT_INFO          *sysport2modport;
    PARSER_HINT_PTR ARAD_INTERRUPTS            *interrupts;
    PARSER_HINT_PTR ARAD_CHIP_DEFINITIONS      *chip_definitions;
    PARSER_HINT_ARR PARSER_HINT_ALLOW_WB_ACCESS soc_phy_port_sw_db_t       *phy_ports_info;
    PARSER_HINT_ARR PARSER_HINT_ALLOW_WB_ACCESS soc_logical_port_sw_db_t   *logical_ports_info;
    PARSER_HINT_PTR ARAD_REASSBMEBLY_CTXT      *reassembly_ctxt;
    PARSER_HINT_ARR CHIP_SIM_EM_BLOCK          *arad_em_blocks;
    /* book keeping of the guaranteed resource for VOQs */
    PARSER_HINT_ARR soc_dpp_guaranteed_q_resource_t     *guaranteed_q_resource;
    PARSER_HINT_PTR ARAD_EGR_PROG_EDITOR_INFO  *egr_prog_editor;
} soc_arad_sw_state_tm_t;

typedef struct
{
  ARAD_SW_DB_DEVICE*
    arad_device_sw_db[SOC_SAND_MAX_DEVICE];
}  ARAD_SW_DB;

extern uint8 Arad_sw_db_initialized;
extern ARAD_SW_DB_DEVICE **_arad_device_sw_db;

/*************
 * FUNCTIONS *
 *************/
/* { */

/********************************************************************************************
 * Initialization
 * {
 ********************************************************************************************/
uint32
  arad_sw_db_init(void);

void
  arad_sw_db_close(void);

/*
 *  Per-device software database initializations
 */
int
    arad_sw_db_sw_state_alloc(
        SOC_SAND_IN int     unit
  );

uint32
  arad_sw_db_device_init(
    SOC_SAND_IN int     unit
  );

/*
 *  Per-device software database closing
 */
uint32
  arad_sw_db_device_close(
    SOC_SAND_IN int unit
  );

/*********************************************************************************************
 * }
 * arad_egr_ports
 * {
 *********************************************************************************************/

uint32
  arad_sw_db_egr_ports_get(
    SOC_SAND_IN  int                     unit,
    SOC_SAND_IN  int                     core,
    SOC_SAND_IN  uint32                  base_q_pair,
    SOC_SAND_OUT ARAD_SW_DB_DEV_EGR_RATE *val
  );

/*********************************************************************************************
 * }
 * arad_scheduler
 * {
 *********************************************************************************************/

uint32
  arad_sw_db_sch_max_expected_port_rate_set(
    SOC_SAND_IN int unit,
    SOC_SAND_IN uint32 port_ndx,
    SOC_SAND_IN uint32  rate
  );

uint32
  arad_sw_db_sch_max_expected_port_rate_get(
    SOC_SAND_IN int unit,
    SOC_SAND_IN uint32 port_ndx
  );

uint32
  arad_sw_db_sch_accumulated_grp_port_rate_set(
    SOC_SAND_IN int unit,
    SOC_SAND_IN uint32 grp_ndx,
    SOC_SAND_IN uint32  rate
  );

uint32
  arad_sw_db_sch_accumulated_grp_port_rate_get(
    SOC_SAND_IN int unit,
    SOC_SAND_IN uint32 grp_ndx
  );

/*********************************************************************************************
 * }
 * arad_multicast
 * {
 *********************************************************************************************/

/* Mark the given egress multicast group as open or not in SWDB */
uint32 arad_sw_db_egress_group_open_set(
    SOC_SAND_IN  int     unit, /* device */
    SOC_SAND_IN  uint32  group_id,  /* multicast ID */
    SOC_SAND_IN  uint8   is_open    /* non zero value will mark the group as open */
);

/* Mark all egress multicast groups as open or not in SWDB */
uint32 arad_sw_db_egress_group_open_set_all(
    SOC_SAND_IN  int     unit, /* device */
    SOC_SAND_IN  uint8   is_open    /* non zero value will mark the group as open */
);

uint32
  arad_sw_db_op_mode_init(
    SOC_SAND_IN int unit
  );

void
  arad_sw_db_is_petrab_in_system_set(
    SOC_SAND_IN int unit,
    SOC_SAND_IN uint8 is_petrab_in_system
  );

uint8
  arad_sw_db_is_petrab_in_system_get(
    SOC_SAND_IN int unit
  );


void
  arad_sw_db_tdm_mode_set(
    SOC_SAND_IN int unit,
    SOC_SAND_IN ARAD_MGMT_TDM_MODE tdm_mode
  );

ARAD_MGMT_TDM_MODE
  arad_sw_db_tdm_mode_get(
    SOC_SAND_IN int unit
  );


void
  arad_sw_db_ilkn_tdm_dedicated_queuing_set(
     SOC_SAND_IN int unit,
     SOC_SAND_IN ARAD_MGMT_ILKN_TDM_DEDICATED_QUEUING_MODE ilkn_tdm_dedicated_queuing
  );

ARAD_MGMT_ILKN_TDM_DEDICATED_QUEUING_MODE
  arad_sw_db_ilkn_tdm_dedicated_queuing_get(
     SOC_SAND_IN int unit
  );

/*
 * Cnt
 */
uint32
  arad_sw_db_cnt_buff_and_index_set(
    SOC_SAND_IN int                     unit,
    SOC_SAND_IN uint16                     proc_id,
    SOC_SAND_IN uint32                     *buff,
    SOC_SAND_IN uint32                     index
  );
uint32
  arad_sw_db_cnt_buff_and_index_get(
    SOC_SAND_IN int                     unit,
    SOC_SAND_IN uint16                     proc_id,
    SOC_SAND_OUT uint32                     **buff,
    SOC_SAND_OUT uint32                    *index
   );
  
uint32
  arad_sw_db_dram_deleted_buff_list_add(
    SOC_SAND_IN int    unit,
    SOC_SAND_IN uint32     buff
  );

uint32
  arad_sw_db_dram_deleted_buff_list_remove(
    SOC_SAND_IN int    unit,
    SOC_SAND_IN uint32     buff
  );
 
int
  arad_sw_db_dram_deleted_buff_list_get(
    SOC_SAND_IN int    unit,
    SOC_SAND_IN uint32     buff,
    SOC_SAND_OUT uint32*     is_deleted
  ); 
 
int 
  arad_sw_db_dram_deleted_buff_list_get_all(
    SOC_SAND_IN int    unit,
    SOC_SAND_OUT uint32*    buff_list_arr,
    SOC_SAND_IN uint32      arr_size,
    SOC_SAND_OUT uint32*    buff_list_num);
  
/*
 * check/set if a (egress) local port has a reassembly context reserved for it
 * for a non mirroring application. 
 */
uint32
  arad_sw_db_is_port_reserved_for_reassembly_context(
    SOC_SAND_IN int  unit,
    SOC_SAND_IN uint32  local_port,
    SOC_SAND_OUT uint8  *is_reserved /* returns one of: 0 for not reserved, 1 for reserved */
  );

uint32
  arad_sw_db_set_port_reserved_for_reassembly_context(
    SOC_SAND_IN int  unit,
    SOC_SAND_IN uint32  local_port,
    SOC_SAND_IN uint8   reserve /* 0 will cancel reservation, other values will reserve */
  );

SOC_SAND_OCC_BM_PTR
  arad_sw_db_egr_ports_nonchanif2sch_offset_occ_get(
    SOC_SAND_IN int unit
  );

SOC_SAND_OCC_BM_PTR
  arad_sw_db_egr_e2e_interfaces_occ_get(
    SOC_SAND_IN int unit,
    SOC_SAND_IN int core
  );

uint32
  arad_sw_db_sysport2queue_set(
    SOC_SAND_IN int             unit,
    SOC_SAND_IN uint32          core_id,
    SOC_SAND_IN ARAD_SYSPORT    sysport,
    SOC_SAND_IN uint8           valid,
    SOC_SAND_IN uint8           sw_only,
    SOC_SAND_IN uint32          base_queue
   );
uint32
  arad_sw_db_sysport2queue_get(
    SOC_SAND_IN  int             unit,
    SOC_SAND_IN  int             core_id,
    SOC_SAND_IN  ARAD_SYSPORT    sysport,
    SOC_SAND_OUT uint8          *valid,
    SOC_SAND_OUT uint8          *sw_only,
    SOC_SAND_OUT uint32         *base_queue
   );

uint32
  arad_sw_db_queuequartet2sysport_set(
    SOC_SAND_IN int             unit,
    SOC_SAND_IN uint32          core_id,
    SOC_SAND_IN uint32          queue_quartet,
    SOC_SAND_IN ARAD_SYSPORT    sysport
   );

uint32
  arad_sw_db_queuequartet2sysport_get(
    SOC_SAND_IN int             unit,
    SOC_SAND_IN uint32          core_id,
    SOC_SAND_IN uint32          queue_quartet,
    SOC_SAND_OUT ARAD_SYSPORT    *sysport
   );

uint32
  arad_sw_db_tcam_db_forbidden_dbs_set(
    SOC_SAND_IN int    unit,
    SOC_SAND_IN uint32 tcam_db_id,
    SOC_SAND_IN uint32 tcam_db_other,
    SOC_SAND_IN uint8  is_forbidden
  );

uint32
  arad_sw_db_tcam_db_forbidden_dbs_get(
    SOC_SAND_IN int    unit,
    SOC_SAND_IN uint32 tcam_db_id,
    SOC_SAND_IN uint32 tcam_db_other,
    SOC_SAND_OUT uint8 *is_forbidden
  );

SOC_SAND_SORTED_LIST_PTR
  arad_sw_db_tcam_db_priorities_get(
    SOC_SAND_IN int unit,
    SOC_SAND_IN uint32 tcam_db_id
  );

SOC_SAND_HASH_TABLE_PTR
  arad_sw_db_tcam_db_entry_id_to_location_get(
    SOC_SAND_IN int unit
  );

/*
 * PMF resource sattus API
 */
/*
 * PMF PSL info {
 */
uint32
  arad_sw_db_pmf_psl_pfg_dbs_set(
    SOC_SAND_IN  int                   unit,
    SOC_SAND_IN  SOC_PPC_FP_DATABASE_STAGE            stage,
    SOC_SAND_IN  uint32                   pfg_ndx,
    SOC_SAND_IN uint32                    pfgs_db_pmb[ARAD_BIT_TO_U32(ARAD_PMF_NOF_DBS)]
  );


uint32
  arad_sw_db_pmf_psl_pfg_dbs_get(
    SOC_SAND_IN  int                   unit,
    SOC_SAND_IN  SOC_PPC_FP_DATABASE_STAGE            stage,
    SOC_SAND_IN  uint32                   pfg_ndx,
    SOC_SAND_OUT uint32                   pfgs_db_pmb[ARAD_BIT_TO_U32(ARAD_PMF_NOF_DBS)]
  );

uint32
  arad_sw_db_pmf_psl_new_db_id_set(
    SOC_SAND_IN  int                   unit,
    SOC_SAND_IN  SOC_PPC_FP_DATABASE_STAGE            stage,
    SOC_SAND_IN  uint32                   new_db_id
  );

uint32
  arad_sw_db_pmf_psl_new_db_id_get(
    SOC_SAND_IN  int                   unit,
    SOC_SAND_IN  SOC_PPC_FP_DATABASE_STAGE            stage,
    SOC_SAND_OUT  uint32                   *new_db_id
  );


/*
 * PMF PSL info }
 */


uint32
  arad_sw_db_fp_db_entry_bitmap_set(
    SOC_SAND_IN  int                     unit,
    SOC_SAND_IN  SOC_PPC_FP_DATABASE_STAGE     stage,
    SOC_SAND_IN  uint32                     bank_index,
    SOC_SAND_IN  uint32                     entry_index,
    SOC_SAND_IN  uint8                      is_used
  );

uint32
  arad_sw_db_fp_db_entry_bitmap_get(
    SOC_SAND_IN  int                        unit,
    SOC_SAND_IN  SOC_PPC_FP_DATABASE_STAGE     stage,
    SOC_SAND_IN  uint32                     bank_index,
    SOC_SAND_IN  uint32                     entry_index,
    SOC_SAND_OUT uint8                      *is_used
  );
 
uint32
  arad_sw_db_fp_db_entry_bitmap_clear(
    SOC_SAND_IN  int                        unit,
    SOC_SAND_IN  SOC_PPC_FP_DATABASE_STAGE     stage
  );

uint32
  arad_sw_db_eg_encap_prge_tbl_nof_dynamic_entries_get(
    SOC_SAND_IN  int  unit,
    SOC_SAND_OUT uint32 *nof_dynamic_members
  );

uint32
  arad_sw_db_eg_encap_prge_tbl_overlay_arp_entries_base_index_get(
     SOC_SAND_IN  int  unit, 
     SOC_SAND_OUT uint32 *overlay_arp_entry_start_index
  );


/*
 * SW DB multi-sets {
 */
uint32
  arad_sw_db_buffer_set_entry(
    SOC_SAND_IN  int                             unit,
    SOC_SAND_IN  uint32                             sec_hanlde,
    SOC_SAND_INOUT  uint8                           *buffer,
    SOC_SAND_IN  uint32                             offset,
    SOC_SAND_IN  uint32                             len,
    SOC_SAND_IN  uint8                              *data
  );

uint32
  arad_sw_db_buffer_get_entry(
    SOC_SAND_IN  int                             unit,
    SOC_SAND_IN  uint32                             sec_hanlde,
    SOC_SAND_IN  uint8                              *buffer,
    SOC_SAND_IN  uint32                             offset,
    SOC_SAND_IN  uint32                             len,
    SOC_SAND_OUT uint8                              *data
  );

uint32
  arad_sw_db_multiset_add(
    SOC_SAND_IN  int                unit,
	SOC_SAND_IN	 int				      core_id,
	SOC_SAND_IN  uint32                multiset_type, /* ARAD_SW_DB_MULTI_SET */
    SOC_SAND_IN  uint32                 *val,
    SOC_SAND_OUT  uint32                *data_indx,
    SOC_SAND_OUT  uint8               *first_appear,
    SOC_SAND_OUT  SOC_SAND_SUCCESS_FAILURE    *success
  );

uint32
  arad_sw_db_multiset_remove(
    SOC_SAND_IN  int                unit,
	SOC_SAND_IN	 int	   core_id,
    SOC_SAND_IN  uint32                multiset_type, /* ARAD_SW_DB_MULTI_SET */
    SOC_SAND_IN  uint32                 *val,
    SOC_SAND_OUT  uint32                *data_indx,
    SOC_SAND_OUT  uint8               *last_appear
  );

uint32
  arad_sw_db_multiset_lookup(
    SOC_SAND_IN  int       unit,
	SOC_SAND_IN	 int	   core_id,
	SOC_SAND_IN  uint32       multiset_type, /* ARAD_SW_DB_MULTI_SET */
    SOC_SAND_IN  uint32        *val,
    SOC_SAND_OUT  uint32       *data_indx,
    SOC_SAND_OUT  uint32       *ref_count
  );

uint32
  arad_sw_db_multiset_add_by_index(
    SOC_SAND_IN  int                unit,
	SOC_SAND_IN	 int	   			core_id,
    SOC_SAND_IN  uint32                multiset_type, /* ARAD_SW_DB_MULTI_SET */
    SOC_SAND_IN  uint32                 *val,
    SOC_SAND_OUT  uint32                data_indx,
    SOC_SAND_OUT  uint8               *first_appear,
    SOC_SAND_OUT  SOC_SAND_SUCCESS_FAILURE    *success
  );

uint32
  arad_sw_db_multiset_remove_by_index(
    SOC_SAND_IN  int                unit,
	SOC_SAND_IN	 int	   core_id,
    SOC_SAND_IN  uint32                multiset_type, /* ARAD_SW_DB_MULTI_SET */
    SOC_SAND_IN  uint32                 data_indx,
    SOC_SAND_OUT  uint8               *last_appear
  );

uint32
  arad_sw_db_multiset_clear(
    SOC_SAND_IN  int                unit,
	SOC_SAND_IN	 int	   core_id,
    SOC_SAND_IN  uint32                multiset_type /* ARAD_SW_DB_MULTI_SET */
  );

uint32
  arad_sw_db_multiset_get_enable_bit(
    SOC_SAND_IN  int                	unit,
    SOC_SAND_IN  uint32                	core_id, /* ARAD_SW_DB_MULTI_SET */
	SOC_SAND_IN  uint32					tbl_offset,
	SOC_SAND_OUT uint8					*enable
  );

/*
 * } Configuration
 */

/*********************************************************************************************
* }
* arad interrupt
* {
*********************************************************************************************/

uint8
  arad_sw_db_interrupt_mask_on_get(
    SOC_SAND_IN  int                        unit
  );

void
  arad_sw_db_interrupt_mask_on_set(
    SOC_SAND_IN  int                        unit,
    SOC_SAND_IN  uint8                        val
  );

/* } */


int arad_sw_db_sw_dump(int unit);


/*
Set a device x port to system physical port mapping.
Performs allocation inside the data structure if needed.
*/
uint32 arad_sw_db_modport2sysport_set(
    SOC_SAND_IN int unit,
    SOC_SAND_IN uint32 fap_id,
    SOC_SAND_IN uint32 fap_port_id,
    SOC_SAND_IN ARAD_SYSPORT sysport
  );

/*
Set a system physical port to device x port mapping.
Performs allocation inside the data structure if needed.
*/
uint32 arad_sw_db_sysport2modport_set(
    SOC_SAND_IN int unit,
    SOC_SAND_IN ARAD_SYSPORT sysport,
    SOC_SAND_IN uint32 fap_id,
    SOC_SAND_IN uint32 fap_port_id
  );

/*
Get a device x port to system physical port mapping.
If the mapping does not exist, the value of ARAD_NOF_SYS_PHYS_PORTS is returned
*/
uint32 arad_sw_db_modport2sysport_get(
    SOC_SAND_IN int unit,
    SOC_SAND_IN uint32 fap_id,
    SOC_SAND_IN uint32 fap_port_id,
    SOC_SAND_OUT ARAD_SYSPORT *sysport
  );

/*
Get system physical port to device x port mapping.
*/
uint32 arad_sw_db_sysport2modport_get(
    SOC_SAND_IN int unit,
    SOC_SAND_IN ARAD_SYSPORT sysport,
    SOC_SAND_OUT uint32 *fap_id,
    SOC_SAND_OUT uint32 *fap_port_id
  );

/*
Get a reverse system physical port to device x port mapping.
Works by searching the mapping till finding the system physical port.
If the mapping does not exist, the value of ARAD_SW_DB_MODPORT2SYSPORT_REVERSE_GET_NOT_FOUND is returned
*/
uint32 arad_sw_db_modport2sysport_reverse_get(
    SOC_SAND_IN int unit,
    SOC_SAND_IN ARAD_SYSPORT sysport,
    SOC_SAND_OUT uint32 *fap_id,
    SOC_SAND_OUT uint32 *fap_port_id
  );

/*
Remove a device x port to system physical port mapping.
*/
uint32 arad_sw_db_modport2sysport_remove(
    SOC_SAND_IN int unit,
    SOC_SAND_IN uint32 fap_id,
    SOC_SAND_IN uint32 fap_port_id
  );

/* 
 *  Move a credit request profile-queue mapping from orig_q_type to new_q_type, 
 *  if core==SOC_CORE_ALL than we move nof_active_cores reference count.
 */ 
uint32
  arad_sw_db_queue_type_ref_count_exchange(
    SOC_SAND_IN  int            unit,
    SOC_SAND_IN  int            core,
    SOC_SAND_IN  uint8          orig_q_type,
    SOC_SAND_IN  uint8          new_q_type,
    SOC_SAND_IN  int            nof_additions);

/* Get the hardware queue type mapped to from the user queue type. Returns ARAD_SW_DB_QUEUE_TYPE_NOT_AVAILABLE in mapped_q_type if not found */
uint32
  arad_sw_db_queue_type_map_get(
    SOC_SAND_IN  int         unit,
    SOC_SAND_IN  uint8          user_q_type,  /* input user queue type (predefined type or user defined allocated type) */
    SOC_SAND_OUT uint8*         mapped_q_type /* output hardware queue type, 0 if not mapped */
  );

/*
 * Get the hardware queue type mapped to from the user queue type, allocating it if it was not allocated before.
 * Returns ARAD_SW_DB_QUEUE_TYPE_NOT_AVAILABLE in mapped_q_type if mapping is not possible since all hardware types (credit request profiles) are used.
 * If given a predefined queue type, will just return it as output as it does not use dynamic allocation.
 */
uint32
  arad_sw_db_queue_type_map_get_alloc(
    SOC_SAND_IN  int         unit,
    SOC_SAND_IN  uint8          user_q_type,  /* input user queue type (predefined type or user defined allocated type) */
    SOC_SAND_OUT uint8*         mapped_q_type /* output hardware queue type, 0 if not mapped */
  );

/* Get the user queue type mapped from the given hardware queue type. */
uint32
  arad_sw_db_queue_type_map_reverse_get(
    SOC_SAND_IN  int         unit,
    SOC_SAND_IN  uint8          mapped_q_type,  /* input hardware queue type, 0 if not mapped */
    SOC_SAND_OUT uint8*         user_q_type     /* output user queue type (predefined type or user defined allocated type) */
  );

uint32
  arad_sw_db_is_port_valid_get(
   SOC_SAND_IN  int               unit,
   SOC_SAND_IN  int               core,
   SOC_SAND_IN  uint32            base_q_pair,
   SOC_SAND_OUT uint8             *is_valid
   );
uint32
  arad_sw_db_is_port_valid_set(
   SOC_SAND_IN  int               unit,
   SOC_SAND_IN  int               core,
   SOC_SAND_IN  uint32            base_q_pair,
   SOC_SAND_IN  uint8             is_valid
   );
uint32
  arad_sw_db_egq_port_rate_set(
   SOC_SAND_IN  int               unit,
   SOC_SAND_IN  int               core,
   SOC_SAND_IN  uint32            base_q_pair,
   SOC_SAND_IN  uint32            rate
   );
uint32
  arad_sw_db_egq_port_rate_get(
   SOC_SAND_IN   int               unit,
   SOC_SAND_IN   int               core,
   SOC_SAND_IN   uint32            base_q_pair,
   SOC_SAND_OUT  uint32            *rate
   );
uint32
  arad_sw_db_sch_port_rate_set(
   SOC_SAND_IN  int               unit,
   SOC_SAND_IN  int               core,
   SOC_SAND_IN  uint32            base_q_pair,
   SOC_SAND_IN  uint32            rate
   );
uint32
  arad_sw_db_sch_port_rate_get(
   SOC_SAND_IN  int               unit,
   SOC_SAND_IN  int               core,
   SOC_SAND_IN  uint32            base_q_pair,
   SOC_SAND_OUT uint32            *rate
   );

uint32
  arad_sw_db_sch_priority_port_rate_set( 
   SOC_SAND_IN  int               unit,
   SOC_SAND_IN  int               core,
   SOC_SAND_IN  uint32            offset,
   SOC_SAND_IN  uint32            rate,
   SOC_SAND_IN  uint8             valid
   );
uint32
  arad_sw_db_sch_priority_port_rate_get( 
   SOC_SAND_IN  int               unit,
   SOC_SAND_IN  int               core,
   SOC_SAND_IN  uint32            offset,
   SOC_SAND_OUT int               *rate,
   SOC_SAND_OUT uint8             *valid
   );

uint32
  arad_sw_db_sch_port_tcg_rate_set( 
   SOC_SAND_IN  int               unit,
   SOC_SAND_IN  int               core,
   SOC_SAND_IN  uint32            offset,
   SOC_SAND_IN  uint32            rate,
   SOC_SAND_IN  uint8             valid
   );

uint32
  arad_sw_db_sch_port_tcg_rate_get( 
   SOC_SAND_IN  int               unit,
   SOC_SAND_IN  int               core,
   SOC_SAND_IN  uint32            offset,
   SOC_SAND_OUT int               *rate,
   SOC_SAND_OUT uint8             *valid
   );
  
uint32 
    arad_sw_db_rate_class_ref_count_get(
       SOC_SAND_IN  int                         unit, 
       SOC_SAND_IN  int                         core_id, 
       SOC_SAND_IN  uint32                      is_ocb_only, 
       SOC_SAND_IN  uint32                      rate_class, 
       SOC_SAND_OUT uint32*                     ref_count);
uint32 
    arad_sw_db_tm_queue_to_rate_class_mapping_ref_count_exchange(
       SOC_SAND_IN  int                         unit,
       SOC_SAND_IN  int                         core, 
       SOC_SAND_IN  uint32                      is_ocb_only,
       SOC_SAND_IN  uint32                      old_rate_class,
       SOC_SAND_IN  uint32                      new_rate_class,
       SOC_SAND_IN  int                         nof_additions);

#include <soc/dpp/SAND/Utils/sand_footer.h>


/* } __ARAD_SW_DB_INCLUDED__*/

#endif
