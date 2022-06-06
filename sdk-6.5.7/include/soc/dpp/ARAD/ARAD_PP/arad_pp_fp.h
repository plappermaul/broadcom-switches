/* $Id: arad_pp_fp.h,v 1.24 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
*/

#ifndef __ARAD_PP_FP_INCLUDED__
/* { */
#define __ARAD_PP_FP_INCLUDED__

/*************
 * INCLUDES  *
 *************/
/* { */

#include <soc/dpp/SAND/Utils/sand_header.h>

#include <soc/dpp/SAND/Management/sand_general_macros.h>
#include <soc/dpp/SAND/Management/sand_error_code.h>

#include <soc/dpp/PPC/ppc_api_fp.h>
#include <soc/dpp/ARAD/ARAD_PP/arad_pp_framework.h>
#include <soc/dpp/ARAD/ARAD_PP/arad_pp_llp_parse.h>

#include <soc/dpp/ARAD/arad_pmf_low_level.h>
#include <soc/dpp/ARAD/arad_tcam_mgmt.h>

/* } */
/*************
 * DEFINES   *
 *************/
/* { */

/*     Number of Priority decoders                          */
#define  ARAD_PP_FP_NOF_PDS        (8)


/* } */
/*************
 * MACROS    *
 *************/
/* { */



/* } */
/*************
 * TYPE DEFS *
 *************/
/* { */


typedef enum
{
  /*
   * Auto generated. Do not edit following section {
   */
  SOC_PPC_FP_PACKET_FORMAT_GROUP_SET = ARAD_PP_PROC_DESC_BASE_FP_FIRST,
  SOC_PPC_FP_PACKET_FORMAT_GROUP_SET_PRINT,
  SOC_PPC_FP_PACKET_FORMAT_GROUP_SET_UNSAFE,
  SOC_PPC_FP_PACKET_FORMAT_GROUP_SET_VERIFY,
  SOC_PPC_FP_PACKET_FORMAT_GROUP_GET,
  SOC_PPC_FP_PACKET_FORMAT_GROUP_GET_PRINT,
  SOC_PPC_FP_PACKET_FORMAT_GROUP_GET_VERIFY,
  SOC_PPC_FP_PACKET_FORMAT_GROUP_GET_UNSAFE,
  ARAD_PP_FP_DATABASE_CREATE,
  ARAD_PP_FP_DATABASE_CREATE_PRINT,
  ARAD_PP_FP_DATABASE_CREATE_UNSAFE,
  ARAD_PP_FP_DATABASE_CREATE_VERIFY,
  ARAD_PP_FP_DATABASE_GET,
  ARAD_PP_FP_DATABASE_GET_PRINT,
  ARAD_PP_FP_DATABASE_GET_UNSAFE,
  ARAD_PP_FP_DATABASE_GET_VERIFY,
  ARAD_PP_FP_DATABASE_DESTROY,
  ARAD_PP_FP_DATABASE_DESTROY_PRINT,
  ARAD_PP_FP_DATABASE_DESTROY_UNSAFE,
  ARAD_PP_FP_DATABASE_DESTROY_VERIFY,
  ARAD_PP_FP_ENTRY_ADD,
  ARAD_PP_FP_ENTRY_ADD_PRINT,
  ARAD_PP_FP_ENTRY_ADD_UNSAFE,
  ARAD_PP_FP_ENTRY_ADD_VERIFY,
  ARAD_PP_FP_ENTRY_GET,
  ARAD_PP_FP_ENTRY_GET_PRINT,
  ARAD_PP_FP_ENTRY_GET_UNSAFE,
  ARAD_PP_FP_ENTRY_GET_VERIFY,
  ARAD_PP_FP_ENTRY_REMOVE,
  ARAD_PP_FP_ENTRY_REMOVE_PRINT,
  ARAD_PP_FP_ENTRY_REMOVE_UNSAFE,
  ARAD_PP_FP_ENTRY_REMOVE_VERIFY,
  ARAD_PP_FP_DATABASE_ENTRIES_GET_BLOCK,
  ARAD_PP_FP_DATABASE_ENTRIES_GET_BLOCK_PRINT,
  ARAD_PP_FP_DATABASE_ENTRIES_GET_BLOCK_UNSAFE,
  ARAD_PP_FP_DATABASE_ENTRIES_GET_BLOCK_VERIFY,
  ARAD_PP_FP_DIRECT_EXTRACTION_ENTRY_ADD,
  ARAD_PP_FP_DIRECT_EXTRACTION_ENTRY_ADD_PRINT,
  ARAD_PP_FP_DIRECT_EXTRACTION_ENTRY_ADD_UNSAFE,
  ARAD_PP_FP_DIRECT_EXTRACTION_ENTRY_ADD_VERIFY,
  ARAD_PP_FP_DIRECT_EXTRACTION_ENTRY_GET,
  ARAD_PP_FP_DIRECT_EXTRACTION_ENTRY_GET_PRINT,
  ARAD_PP_FP_DIRECT_EXTRACTION_ENTRY_GET_UNSAFE,
  ARAD_PP_FP_DIRECT_EXTRACTION_ENTRY_GET_VERIFY,
  ARAD_PP_FP_DIRECT_EXTRACTION_ENTRY_REMOVE,
  ARAD_PP_FP_DIRECT_EXTRACTION_ENTRY_REMOVE_PRINT,
  ARAD_PP_FP_DIRECT_EXTRACTION_ENTRY_REMOVE_UNSAFE,
  ARAD_PP_FP_DIRECT_EXTRACTION_ENTRY_REMOVE_VERIFY,
  ARAD_PP_FP_DIRECT_EXTRACTION_DB_ENTRIES_GET,
  ARAD_PP_FP_DIRECT_EXTRACTION_DB_ENTRIES_GET_PRINT,
  ARAD_PP_FP_DIRECT_EXTRACTION_DB_ENTRIES_GET_UNSAFE,
  ARAD_PP_FP_DIRECT_EXTRACTION_DB_ENTRIES_GET_VERIFY,
  ARAD_PP_FP_CONTROL_SET,
  ARAD_PP_FP_CONTROL_SET_PRINT,
  ARAD_PP_FP_CONTROL_SET_UNSAFE,
  ARAD_PP_FP_CONTROL_SET_VERIFY,
  ARAD_PP_FP_CONTROL_GET,
  ARAD_PP_FP_CONTROL_GET_PRINT,
  ARAD_PP_FP_CONTROL_GET_VERIFY,
  ARAD_PP_FP_CONTROL_GET_UNSAFE,
  ARAD_PP_FP_EGR_DB_MAP_SET,
  ARAD_PP_FP_EGR_DB_MAP_SET_PRINT,
  ARAD_PP_FP_EGR_DB_MAP_SET_UNSAFE,
  ARAD_PP_FP_EGR_DB_MAP_SET_VERIFY,
  ARAD_PP_FP_EGR_DB_MAP_GET,
  ARAD_PP_FP_EGR_DB_MAP_GET_PRINT,
  ARAD_PP_FP_EGR_DB_MAP_GET_VERIFY,
  ARAD_PP_FP_EGR_DB_MAP_GET_UNSAFE,
  SOC_PPC_FP_PACKET_DIAG_GET,
  SOC_PPC_FP_PACKET_DIAG_GET_PRINT,
  SOC_PPC_FP_PACKET_DIAG_GET_UNSAFE,
  SOC_PPC_FP_PACKET_DIAG_GET_VERIFY,
  ARAD_PP_FP_RESOURCE_DIAG_GET,
  ARAD_PP_FP_RESOURCE_DIAG_GET_PRINT,
  ARAD_PP_FP_RESOURCE_DIAG_GET_UNSAFE,
  ARAD_PP_FP_RESOURCE_DIAG_GET_VERIFY,
  ARAD_PP_FP_GET_PROCS_PTR,
  ARAD_PP_FP_GET_ERRS_PTR,
  /*
   * } Auto generated. Do not edit previous section.
   */
   ARAD_PP_FP_DB_TYPES_VERIFY_FIRST_STAGE,
   SOC_PPC_FP_QUAL_TYPES_VERIFY_FIRST_STAGE,
   SOC_PPC_FP_ACTION_TYPES_VERIFY_FIRST_STAGE,
   SOC_PPC_FP_ACTION_TYPE_TO_CE_COPY,
   ARAD_PP_FP_DB_STAGE_GET,
   ARAD_PP_FP_DB_CASCADED_CYCLE_GET,
   ARAD_PP_FP_DB_IS_EQUAL_PLACE_GET,
   ARAD_PP_FP_TCAM_CALLBACK,
   ARAD_PP_FP_INIT_UNSAFE,
   SOC_PPC_FP_ACTION_TYPE_MAX_SIZE_GET,
   SOC_PPC_FP_ACTION_TYPE_TO_PMF_CONVERT,
   ARAD_PP_FP_FEM_SET,
   ARAD_PP_FP_FEM_DB_FIND,
   SOC_PPC_FP_QUAL_TYPE_PREDEFINED_KEY_GET,
   ARAD_PP_FP_PREDEFINED_KEY_GET,
   ARAD_PP_FP_KEY_GET,
   SOC_PPC_FP_QUAL_TYPE_TO_TCAM_TYPE_CONVERT,
   SOC_PPC_FP_QUAL_TYPE_TO_KEY_FLD_CONVERT,
   ARAD_PP_FP_PREDEFINED_KEY_SIZE_AND_TYPE_GET,
   ARAD_PP_FP_ENTRY_VALIDITY_GET,
   SOC_PPC_FP_QUAL_TYPE_PRESET1,
   SOC_PPC_FP_QUAL_VAL_ENCODE,
   ARAD_PP_FP_ACTION_LSB_GET,
   ARAD_PP_FP_QUAL_LSB_AND_LENGTH_GET,
   ARAD_PP_FP_CE_KEY_LENGTH_MINIMAL_GET,
   ARAD_PP_FP_KEY_INPUT_GET,
   ARAD_PP_FP_ENTRY_NDX_DIRECT_TABLE_GET,
   SOC_PPC_FP_QUAL_TYPE_AND_LOCAL_LSB_GET,
   SOC_PPC_FP_ACTION_TYPE_FROM_PMF_CONVERT,
   ARAD_PP_FP_INGRESS_QUAL_VERIFY,
   ARAD_PP_FP_IPV4_HOST_EXTEND_ENABLE,
   ARAD_PP_FP_CYCLE_FOR_DB_INFO_VERIFY,
   ARAD_PP_FP_IRE_TRAFFIC_SEND_VERIFY,
   ARAD_PP_FP_IRE_TRAFFIC_SEND_UNSAFE,


  /*
   * Last element. Do no touch.
   */
  ARAD_PP_FP_PROCEDURE_DESC_LAST
} ARAD_PP_FP_PROCEDURE_DESC;

typedef enum
{
  /*
   * Auto generated. Do not edit following section {
   */
  ARAD_PP_FP_PFG_NDX_OUT_OF_RANGE_ERR = ARAD_PP_ERR_DESC_BASE_FP_FIRST,
  ARAD_PP_FP_DB_ID_NDX_OUT_OF_RANGE_ERR,
  ARAD_PP_FP_SUCCESS_OUT_OF_RANGE_ERR,
  ARAD_PP_FP_ENTRY_ID_NDX_OUT_OF_RANGE_ERR,
  ARAD_PP_FP_IS_FOUND_OUT_OF_RANGE_ERR,
  ARAD_PP_FP_NOF_ENTRIES_OUT_OF_RANGE_ERR,
  SOC_PPC_FP_FWD_TYPE_NDX_OUT_OF_RANGE_ERR,
  ARAD_PP_FP_PORT_PROFILE_NDX_OUT_OF_RANGE_ERR,
  ARAD_PP_FP_TYPE_OUT_OF_RANGE_ERR,
  ARAD_PP_FP_HDR_FORMAT_BMP_OUT_OF_RANGE_ERR,
  ARAD_PP_FP_VLAN_TAG_STRUCTURE_BMP_OUT_OF_RANGE_ERR,
  ARAD_PP_FP_DB_TYPE_OUT_OF_RANGE_ERR,
  ARAD_PP_FP_SUPPORTED_PFGS_OUT_OF_RANGE_ERR,
  SOC_PPC_FP_QUAL_TYPES_OUT_OF_RANGE_ERR,
  SOC_PPC_FP_DATABASE_TYPE_OUT_OF_RANGE_ERR,
  SOC_PPC_FP_ACTION_TYPES_OUT_OF_RANGE_ERR,
  ARAD_PP_FP_STRENGTH_OUT_OF_RANGE_ERR,
  ARAD_PP_FP_VAL_OUT_OF_RANGE_ERR,
  ARAD_PP_FP_PRIORITY_OUT_OF_RANGE_ERR,
  ARAD_PP_FP_FLD_LSB_OUT_OF_RANGE_ERR,
  ARAD_PP_FP_CST_VAL_OUT_OF_RANGE_ERR,
  ARAD_PP_FP_NOF_BITS_OUT_OF_RANGE_ERR,
  ARAD_PP_FP_NOF_FIELDS_OUT_OF_RANGE_ERR,
  ARAD_PP_FP_BASE_VAL_OUT_OF_RANGE_ERR,
  ARAD_PP_FP_DB_ID_OUT_OF_RANGE_ERR,
  ARAD_PP_FP_VAL_NDX_OUT_OF_RANGE_ERR,
  /*
   * } Auto generated. Do not edit previous section.
   */
   ARAD_PP_FP_DB_ID_ALREADY_EXIST_ERR,
   ARAD_PP_FP_DB_ID_NOT_DIRECT_EXTRACTION_ERR,
   ARAD_PP_FP_DB_ID_NOT_LOOKUP_ERR,
   ARAD_PP_FP_ENTRY_QUAL_TYPE_NOT_IN_DB_ERR,
   ARAD_PP_FP_ENTRY_ACTION_TYPE_NOT_IN_DB_ERR,
   ARAD_PP_FP_ENTRY_ALREADY_EXIST_ERR,
   SOC_PPC_FP_QUAL_TYPES_NOT_PREDEFINED_ACL_KEY_ERR,
   ARAD_PP_FP_DB_CREATION_FOR_IPV4_HOST_EXTEND_ERR,
   ARAD_PP_FP_EGR_DATABASE_NOT_ALREADY_ADDED_ERR,
   SOC_PPC_FP_QUAL_TYPES_END_OF_LIST_ERR,
   SOC_PPC_FP_ACTION_TYPES_END_OF_LIST_ERR,
   ARAD_PP_FP_DB_ID_DIRECT_TABLE_ALREADY_EXIST_ERR,
   ARAD_PP_FP_ACTION_LENGTHS_OUT_OF_RANGE_ERR,
   ARAD_PP_FP_QUALS_LENGTHS_OUT_OF_RANGE_ERR,
   ARAD_PP_FP_EGRESS_QUAL_USED_FOR_INGRESS_DB_ERR,
   ARAD_PP_FP_TAG_ACTION_ALREADY_EXIST_ERR,
   ARAD_PP_FP_QUAL_NOT_EGRESS_ERR,
   ARAD_PP_FP_ACTIONS_MIXED_WITH_TAG_ERR,
   SOC_PPC_FP_PACKET_SIZE_OUT_OF_RANGE_ERR,
   SOC_PPC_FP_NOF_DBS_PER_BANK_OUT_OF_RANGE_ERR,
   ARAD_PP_FP_NOF_QUALS_PER_DB_OUT_OF_RANGE_ERR,
   ARAD_PP_FP_NOF_ACTIONS_PER_DB_OUT_OF_RANGE_ERR, 
   ARAD_PP_FP_STAGE_OUT_OF_RANGE_ERR,
   ARAD_PP_FP_PORT_PROFILE_ALREADY_EXIST_ERR,

  /*
   * Last element. Do no touch.
   */
  ARAD_PP_FP_ERR_LAST
} ARAD_PP_FP_ERR;

/* } */
/*************
 * GLOBALS   *
 *************/
/* { */

typedef struct
{
  SOC_SAND_MAGIC_NUM_VAR
  /*
   *  The key value of teh direct table entry.
   */
  uint32 value;
  /*
   *  The mask for direct table entry key.
   */
  uint32 mask;

} ARAD_PP_FP_DT_KEY;


/* 
 * This union is defined to allow usage of the TCAM location hash table
 * for field processor direct table keys.
 */
typedef union
{
  ARAD_TCAM_LOCATION location;
  ARAD_PP_FP_DT_KEY entry_dt_key;
}ARAD_PP_FP_LOCATION_HASH;


#define MAX_NOF_SHARED_QUALIFIERS_PER_PROGRAM   4

typedef enum 
{
    ARAD_PP_FP_DIP_SIP_CLUSTERS_IPV4,
    ARAD_PP_FP_DIP_SIP_CLUSTERS_IPV6,

    ARAD_PP_FP_NOF_DIP_SIP_CLUSTERS
} ARAD_PP_FP_DIP_SIP_CLUSTERS;



#define ARAD_PP_FP_MAX_NOF_PROGRAMS_IN_DIP_SIP_CLUSTERS   5

typedef struct
{

  char  name[20];/* name of the qualifier as in the ELK master key */

  uint32 size; /* full qualifier length */

  SOC_PPC_FP_QUAL_TYPE qual_type;  

} ARAD_PP_FP_SHARED_QUAL_INFO;


void ARAD_PP_FP_SHARED_QUAL_INFO_clear(ARAD_PP_FP_SHARED_QUAL_INFO* shard_qualifiers_info);

typedef struct
{

  uint32 prog_id; /* the program Id that can perform dip sip sharing */

  /* the qualefier shared information */
  ARAD_PP_FP_SHARED_QUAL_INFO shared_quals[MAX_NOF_SHARED_QUALIFIERS_PER_PROGRAM];  

  uint32 nof_qualifiers; /* nof qualifiers that can be shared */

} ARAD_PP_FP_KEY_DIP_SIP_SHARING;


typedef struct
{

  char cluster_name[20];

  ARAD_PP_FP_DIP_SIP_CLUSTERS cluster_id;

  uint32 prog_id[ARAD_PP_FP_MAX_NOF_PROGRAMS_IN_DIP_SIP_CLUSTERS]; /* the program Ids that related to the cluster */

  /* the qualefier shared information */
  ARAD_PP_FP_SHARED_QUAL_INFO shared_quals[MAX_NOF_SHARED_QUALIFIERS_PER_PROGRAM];  

  uint32 nof_qualifiers; /* nof qualifiers that can be shared */

} ARAD_PP_FP_DIP_SIP_CLUSTER_INFO;


void
  dip_sip_sharing_qual_info_get(int unit, int prog_id, SOC_PPC_FP_QUAL_TYPE qual_type, int* is_exists,  ARAD_PP_FP_SHARED_QUAL_INFO* shared_qual_info);

uint32
  arad_pp_fp_dip_sip_sharing_is_sharing_enabled_for_db(int unit, SOC_PPC_FP_DATABASE_STAGE stage, uint32 db_id_ndx, uint32* is_enabled, int* cluster_id);

void
  dip_sip_sharing_is_shared_qualifier_qset(int unit, int prog_id, SOC_PPC_FP_QUAL_TYPE qual_type, int* is_exists, ARAD_PP_FP_SHARED_QUAL_INFO* shared_qual_info);

/* } */
/*************
 * FUNCTIONS *
 *************/
/* { */

uint32
  arad_pp_fp_key_get(
    SOC_SAND_IN  int                     unit,
    SOC_SAND_IN  uint8                     is_for_egress,
    SOC_SAND_IN  SOC_PPC_FP_QUAL_TYPE           *qual_types,
    SOC_SAND_OUT SOC_PPC_FP_PREDEFINED_ACL_KEY  *predefined_acl_key
  );

uint32
  arad_pp_fp_init_unsafe(
    SOC_SAND_IN  int                                 unit
  );

uint32
    arad_pp_fp_db_stage_get(
      SOC_SAND_IN int unit,
      SOC_SAND_IN uint32  db_id_ndx,
      SOC_SAND_OUT SOC_PPC_FP_DATABASE_STAGE  *stage
    );

uint32
    arad_pp_fp_db_cascaded_cycle_get(
          SOC_SAND_IN int unit,
          SOC_SAND_IN uint32  db_id_ndx,
          SOC_SAND_OUT uint8  *is_cascaded,
          SOC_SAND_OUT uint8  *lookup_id /* 1st or 2nd cycle */
    );

uint32
    arad_pp_fp_db_is_equal_place_get(
          SOC_SAND_IN int unit,
          SOC_SAND_IN uint32  db_id_ndx,
          SOC_SAND_OUT uint8  *is_equal,
          SOC_SAND_OUT uint32 *place /* LSB or MSB */
    );

uint32
  arad_pp_fp_key_length_get_unsafe(
    SOC_SAND_IN    int              unit,
    SOC_SAND_IN     SOC_PPC_FP_DATABASE_STAGE    stage,
    SOC_SAND_IN    SOC_PPC_FP_QUAL_TYPE     qual_type,
    SOC_SAND_IN    uint8              with_padding,
    SOC_SAND_OUT   uint32              *length
  );


/*********************************************************************
* NAME:
 *   arad_pp_fp_packet_format_group_set_unsafe
 * TYPE:
 *   PROC
 * FUNCTION:
 *   Set a Packet Format Group (PFG). The packet format group
 *   defines the supported Packet formats. The user must
 *   indicate for each Database which Packet format(s) are
 *   associated with this Database. E.g.: A Packet Format
 *   Group including only IPv6 packets can be defined to use
 *   Databases with IPv6 Destination-IP qualifiers.
 * INPUT:
 *   SOC_SAND_IN  int                    unit -
 *     Identifier of the device to access.
 *   SOC_SAND_IN  uint32                    pfg_ndx -
 *     Packet Format Group index. Range: 0 - 4.
 *   SOC_SAND_IN  SOC_PPC_PMF_PFG_INFO            *info -
 *     Packet-Format-Group parameters.
 *   SOC_SAND_OUT SOC_SAND_SUCCESS_FAILURE         *success -
 *     Indicate if the database is created successfully.
 * REMARKS:
 *   The user should set a minimal number of Packet Format
 *   Groups since each one uses many Hardware resources.
 * RETURNS:
 *   OK or ERROR indication.
*********************************************************************/
uint32
  arad_pp_fp_packet_format_group_set_unsafe(
    SOC_SAND_IN  int                    unit,
    SOC_SAND_IN  uint32                    pfg_ndx,
    SOC_SAND_IN  SOC_PPC_PMF_PFG_INFO            *info,
    SOC_SAND_OUT SOC_SAND_SUCCESS_FAILURE         *success
  );

uint32
  arad_pp_fp_packet_format_group_set_verify(
    SOC_SAND_IN  int                    unit,
    SOC_SAND_IN  uint32                    pfg_ndx,
    SOC_SAND_IN  SOC_PPC_PMF_PFG_INFO            *info
  );

uint32
  arad_pp_fp_packet_format_group_get_verify(
    SOC_SAND_IN  int                    unit,
    SOC_SAND_IN  uint32                    pfg_ndx
  );

/*********************************************************************
*     Gets the configuration set by the
 *     "arad_pp_fp_packet_format_group_set_unsafe" API.
 *     Refer to "arad_pp_fp_packet_format_group_set_unsafe" API
 *     for details.
*********************************************************************/
uint32
  arad_pp_fp_packet_format_group_get_unsafe(
    SOC_SAND_IN  int                    unit,
    SOC_SAND_IN  uint32                    pfg_ndx,
    SOC_SAND_INOUT SOC_PPC_PMF_PFG_INFO            *info
  );
uint32
    arad_pp_fp_tcam_callback(
      SOC_SAND_IN int unit,
      SOC_SAND_IN uint32  user_data
    );
/*********************************************************************
* NAME:
 *   arad_pp_fp_database_create_unsafe
 * TYPE:
 *   PROC
 * FUNCTION:
 *   Create a database. Each database specifies the action
 *   types to perform and the qualifier fields for this
 *   Database. Entries in the database specify the specific
 *   actions to be taken upon specific values of the
 *   packet. E.g.: Policy Based Routing database update the
 *   FEC value according to DSCP DIP and In-RIF. An entry in
 *   the database may set the FEC of a packet with DIP
 *   1.2.2.3, DSCP value 7 and In-RIF 3 to be 9.
 * INPUT:
 *   SOC_SAND_IN  int                                 unit -
 *     Identifier of the device to access.
 *   SOC_SAND_IN  uint32                                 db_id_ndx -
 *     Database-ID. Range: 0 - 127.
 *   SOC_SAND_IN  SOC_PPC_FP_DATABASE_INFO                    *info -
 *     Database parameters.
 *   SOC_SAND_OUT SOC_SAND_SUCCESS_FAILURE                      *success -
 *     Indicate if the database is created successfully.
 * REMARKS:
 *   None.
 * RETURNS:
 *   OK or ERROR indication.
*********************************************************************/
uint32
  arad_pp_fp_database_create_unsafe(
    SOC_SAND_IN  int                                 unit,
    SOC_SAND_IN  uint32                                 db_id_ndx,
    SOC_SAND_INOUT  SOC_PPC_FP_DATABASE_INFO                    *info,
    SOC_SAND_OUT SOC_SAND_SUCCESS_FAILURE                      *success
  );

uint32
  arad_pp_fp_database_create_verify(
    SOC_SAND_IN  int                                 unit,
    SOC_SAND_IN  uint32                                 db_id_ndx,
    SOC_SAND_IN  SOC_PPC_FP_DATABASE_INFO                    *info,
    SOC_SAND_OUT SOC_SAND_SUCCESS_FAILURE                      *success
  );

/*********************************************************************
* NAME:
 *   arad_pp_fp_database_get_unsafe
 * TYPE:
 *   PROC
 * FUNCTION:
 *   Get the database parameters.
 * INPUT:
 *   SOC_SAND_IN  int                                 unit -
 *     Identifier of the device to access.
 *   SOC_SAND_IN  uint32                                 db_id_ndx -
 *     Database-ID. Range: 0 - 127.
 *   SOC_SAND_OUT SOC_PPC_FP_DATABASE_INFO                    *info -
 *     Database parameters.
 * REMARKS:
 *   None.
 * RETURNS:
 *   OK or ERROR indication.
*********************************************************************/
uint32
  arad_pp_fp_database_get_unsafe(
    SOC_SAND_IN  int                                 unit,
    SOC_SAND_IN  uint32                                 db_id_ndx,
    SOC_SAND_OUT SOC_PPC_FP_DATABASE_INFO                    *info
  );

uint32
  arad_pp_fp_database_get_verify(
    SOC_SAND_IN  int                                 unit,
    SOC_SAND_IN  uint32                                 db_id_ndx
  );


uint32
  arad_pp_fp_database_is_tm_get(
    SOC_SAND_IN  int                                 unit,
    SOC_SAND_IN  SOC_PPC_FP_DATABASE_INFO               *info,
    SOC_SAND_OUT uint8                                  *is_for_tm,
    SOC_SAND_OUT uint8                                  *is_default_tm
  );

/*********************************************************************
* NAME:
 *   arad_pp_fp_database_destroy_unsafe
 * TYPE:
 *   PROC
 * FUNCTION:
 *   Destroy the database: all its entries are suppressed and
 *   the Database-ID is freed.
 * INPUT:
 *   SOC_SAND_IN  int                                 unit -
 *     Identifier of the device to access.
 *   SOC_SAND_IN  uint32                                 db_id_ndx -
 *     Database-ID. Range: 0 - 127.
 * REMARKS:
 *   None.
 * RETURNS:
 *   OK or ERROR indication.
*********************************************************************/
uint32
  arad_pp_fp_database_destroy_unsafe(
    SOC_SAND_IN  int                                 unit,
    SOC_SAND_IN  uint32                                 db_id_ndx
  );

uint32
  arad_pp_fp_database_destroy_verify(
    SOC_SAND_IN  int                                 unit,
    SOC_SAND_IN  uint32                                 db_id_ndx
  );

/*********************************************************************
* NAME:
 *   arad_pp_fp_entry_add_unsafe
 * TYPE:
 *   PROC
 * FUNCTION:
 *   Add an entry to the Database. The database entry is
 *   selected if the entire relevant packet field values are
 *   matched to the database entry qualifiers values. When
 *   the packet is qualified to several entries, the entry
 *   with the strongest priority is chosen.
 * INPUT:
 *   SOC_SAND_IN  int                                 unit -
 *     Identifier of the device to access.
 *   SOC_SAND_IN  uint32                                 db_id_ndx -
 *     Database-ID. Range: 0 - 127.
 *   SOC_SAND_IN  uint32                                 entry_id_ndx -
 *     Entry handle ID. The entry index is a SW handle, to
 *     enable retrieving the entry attributes by the
 *     soc_ppd_fp_entry_get() function, and remove it by the
 *     soc_ppd_fp_entry_remove() function. The actual location of
 *     the entry in the database is selected according to the
 *     entry's priority. Range: 0 - 4K-1.
 *   SOC_SAND_IN  SOC_PPC_FP_ENTRY_INFO                       *info -
 *     Entry parameters.
 *   SOC_SAND_OUT SOC_SAND_SUCCESS_FAILURE                      *success -
 *     Indicate if the database entry is created successfully.
 * REMARKS:
 *   1. The database must be created before the insertion of
 *   an entry. To create a Database, call the
 *   soc_ppd_fp_database_create API.2. The database must be NOT
 *   of type 'direct extraction'3. For a Database of type
 *   'Direct Table', the entry qualifier value must be not
 *   masked.
 * RETURNS:
 *   OK or ERROR indication.
*********************************************************************/
uint32
  arad_pp_fp_entry_add_unsafe(
    SOC_SAND_IN  int                                 unit,
    SOC_SAND_IN  uint32                                 db_id_ndx,
    SOC_SAND_IN  uint32                                 entry_id_ndx,
    SOC_SAND_IN  SOC_PPC_FP_ENTRY_INFO                       *info,
    SOC_SAND_OUT SOC_SAND_SUCCESS_FAILURE                      *success
  );

uint32
  arad_pp_fp_entry_add_verify(
    SOC_SAND_IN  int                                 unit,
    SOC_SAND_IN  uint32                                 db_id_ndx,
    SOC_SAND_IN  uint32                                 entry_id_ndx,
    SOC_SAND_IN  SOC_PPC_FP_ENTRY_INFO                       *info
  );

/*********************************************************************
* NAME:
 *   arad_pp_fp_entry_get_unsafe
 * TYPE:
 *   PROC
 * FUNCTION:
 *   Get an entry from the Database.
 * INPUT:
 *   SOC_SAND_IN  int                                 unit -
 *     Identifier of the device to access.
 *   SOC_SAND_IN  uint32                                 db_id_ndx -
 *     Database-ID. Range: 0 - 127.
 *   SOC_SAND_IN  uint32                                 entry_id_ndx -
 *     Entry-ID. Range: 0 - 4K-1.
 *   SOC_SAND_OUT uint8                                 *is_found -
 *     If True, then the entry is found and the entry
 *     parameters are returned in the 'info' structure.
 *     Otherwise, the entry is not present in the Database.
 *   SOC_SAND_OUT SOC_PPC_FP_ENTRY_INFO                       *info -
 *     Entry parameters. Meaningful only the entry is found.
 * REMARKS:
 *   None.
 * RETURNS:
 *   OK or ERROR indication.
*********************************************************************/
uint32
  arad_pp_fp_entry_get_unsafe(
    SOC_SAND_IN  int                                 unit,
    SOC_SAND_IN  uint32                                 db_id_ndx,
    SOC_SAND_IN  uint32                                 entry_id_ndx,
    SOC_SAND_OUT uint8                                  *is_found,
    SOC_SAND_INOUT SOC_PPC_FP_ENTRY_INFO                *info
  );

uint32
  arad_pp_fp_entry_get_verify(
    SOC_SAND_IN  int                                 unit,
    SOC_SAND_IN  uint32                                 db_id_ndx,
    SOC_SAND_IN  uint32                                 entry_id_ndx
  );

/*********************************************************************
* NAME:
 *   arad_pp_fp_entry_remove_unsafe
 * TYPE:
 *   PROC
 * FUNCTION:
 *   Remove an entry from the Database.
 * INPUT:
 *   SOC_SAND_IN  int                                 unit -
 *     Identifier of the device to access.
 *   SOC_SAND_IN  uint32                                 db_id_ndx -
 *     Database-ID. Range: 0 - 127.
 *   SOC_SAND_IN  uint32                                 entry_id_ndx -
 *     Entry-ID. Range: 0 - 4K-1.
 * REMARKS:
 *   None.
 * RETURNS:
 *   OK or ERROR indication.
*********************************************************************/
uint32
  arad_pp_fp_entry_remove_unsafe(
    SOC_SAND_IN  int                                 unit,
    SOC_SAND_IN  uint32                                 db_id_ndx,
    SOC_SAND_IN  uint32                                 entry_id_ndx,
    SOC_SAND_IN  uint32                                 is_sw_remove_only
  );

uint32
  arad_pp_fp_entry_remove_verify(
    SOC_SAND_IN  int                                 unit,
    SOC_SAND_IN  uint32                                 db_id_ndx,
    SOC_SAND_IN  uint32                                 entry_id_ndx
  );

uint32
  arad_pp_fp_entry_remove_by_key_unsafe(
    SOC_SAND_IN  int                                 unit,
    SOC_SAND_IN  uint32                                 db_id_ndx,
    SOC_SAND_INOUT SOC_PPC_FP_ENTRY_INFO                *info
  );


uint32
  arad_pp_fp_dip_sip_sharing_handle(SOC_SAND_IN int unit, 
                                    SOC_SAND_IN uint32 db_id_ndx, 
                                    SOC_SAND_IN SOC_PPC_FP_ENTRY_INFO* info,
                                    uint8*   elk_data, 
                                    uint8*   elk_mask);
uint32
  arad_pp_fp_entry_remove_by_key_verify(
    SOC_SAND_IN  int                                 unit,
    SOC_SAND_IN  uint32                                 db_id_ndx,
    SOC_SAND_INOUT SOC_PPC_FP_ENTRY_INFO                *info
  );

/*********************************************************************
* NAME:
 *   arad_pp_fp_database_entries_get_block_unsafe
 * TYPE:
 *   PROC
 * FUNCTION:
 *   Get the Database entries. The function returns list of
 *   entries that were added to a database with database ID
 *   'db_id_ndx'.
 * INPUT:
 *   SOC_SAND_IN  int                                 unit -
 *     Identifier of the device to access.
 *   SOC_SAND_IN  uint32                                 db_id_ndx -
 *     Database-ID. Range: 0 - 127.
 *   SOC_SAND_INOUT SOC_SAND_TABLE_BLOCK_RANGE                    *block_range -
 *     Block range to get entries in this Database.
 *   SOC_SAND_OUT SOC_PPC_FP_ENTRY_INFO                       *entries -
 *     Database entries.
 *   SOC_SAND_OUT uint32                                  *nof_entries -
 *     Number of valid entries.
 * REMARKS:
 *   This API can be called only if the Database is NOT of
 *   type 'direct extraction'.
 * RETURNS:
 *   OK or ERROR indication.
*********************************************************************/
uint32
  arad_pp_fp_database_entries_get_block_unsafe(
    SOC_SAND_IN  int                                 unit,
    SOC_SAND_IN  uint32                                 db_id_ndx,
    SOC_SAND_INOUT SOC_SAND_TABLE_BLOCK_RANGE                    *block_range,
    SOC_SAND_OUT SOC_PPC_FP_ENTRY_INFO                       *entries,
    SOC_SAND_OUT uint32                                  *nof_entries
  );

uint32
  arad_pp_fp_database_entries_get_block_verify(
    SOC_SAND_IN  int                                 unit,
    SOC_SAND_IN  uint32                                 db_id_ndx,
    SOC_SAND_INOUT SOC_SAND_TABLE_BLOCK_RANGE                    *block_range
  );

/*********************************************************************
* NAME:
 *   arad_pp_fp_direct_extraction_entry_add_unsafe
 * TYPE:
 *   PROC
 * FUNCTION:
 *   Add an entry to the Database. The database entry is
 *   selected if all the Packet Qualifier field values are in
 *   the Database entry range.
 * INPUT:
 *   SOC_SAND_IN  int                                 unit -
 *     Identifier of the device to access.
 *   SOC_SAND_IN  uint32                                 db_id_ndx -
 *     Database-ID. Range: 0 - 127.
 *   SOC_SAND_IN  uint32                                 entry_id_ndx -
 *     Entry-ID. Range: 0 - 15.
 *   SOC_SAND_IN  SOC_PPC_FP_DIR_EXTR_ENTRY_INFO              *info -
 *     Entry parameters.
 *   SOC_SAND_OUT SOC_SAND_SUCCESS_FAILURE                      *success -
 *     Indicate if the database entry is created successfully.
 * REMARKS:
 *   1. The database must be created before the insertion of
 *   an entry. To create a Database, call the
 *   soc_ppd_fp_database_create API.2. The database must be of
 *   type 'direct extraction'3. The priority enables
 *   selection between two database entries with a
 *   superposition in the Qualifier field ranges.
 * RETURNS:
 *   OK or ERROR indication.
*********************************************************************/
uint32
  arad_pp_fp_direct_extraction_entry_add_unsafe(
    SOC_SAND_IN  int                                 unit,
    SOC_SAND_IN  uint32                                 db_id_ndx,
    SOC_SAND_IN  uint32                                 entry_id_ndx,
    SOC_SAND_IN  SOC_PPC_FP_DIR_EXTR_ENTRY_INFO              *info,
    SOC_SAND_OUT SOC_SAND_SUCCESS_FAILURE                      *success
  );

uint32
  arad_pp_fp_direct_extraction_entry_add_verify(
    SOC_SAND_IN  int                                 unit,
    SOC_SAND_IN  uint32                                 db_id_ndx,
    SOC_SAND_IN  uint32                                 entry_id_ndx,
    SOC_SAND_IN  SOC_PPC_FP_DIR_EXTR_ENTRY_INFO              *info
  );

/*********************************************************************
* NAME:
 *   arad_pp_fp_direct_extraction_entry_get_unsafe
 * TYPE:
 *   PROC
 * FUNCTION:
 *   Get an entry from the Database.
 * INPUT:
 *   SOC_SAND_IN  int                                 unit -
 *     Identifier of the device to access.
 *   SOC_SAND_IN  uint32                                 db_id_ndx -
 *     Database-ID. Range: 0 - 127.
 *   SOC_SAND_IN  uint32                                 entry_id_ndx -
 *     Entry-ID. Range: 0 - 15.
 *   SOC_SAND_OUT uint8                                 *is_found -
 *     If True, then the entry is found and the entry
 *     parameters are returned in the 'info' structure.
 *     Otherwise, the entry is not present in the Database.
 *   SOC_SAND_OUT SOC_PPC_FP_DIR_EXTR_ENTRY_INFO              *info -
 *     Entry parameters. Meaningful only the entry is found.
 * REMARKS:
 *   None.
 * RETURNS:
 *   OK or ERROR indication.
*********************************************************************/
uint32
  arad_pp_fp_direct_extraction_entry_get_unsafe(
    SOC_SAND_IN  int                                 unit,
    SOC_SAND_IN  uint32                                 db_id_ndx,
    SOC_SAND_IN  uint32                                 entry_id_ndx,
    SOC_SAND_OUT uint8                                 *is_found,
    SOC_SAND_OUT uint8                                 *is_fes,
    SOC_SAND_OUT SOC_PPC_FP_DIR_EXTR_ENTRY_INFO              *info
  );

uint32
  arad_pp_fp_direct_extraction_entry_get_verify(
    SOC_SAND_IN  int                                 unit,
    SOC_SAND_IN  uint32                                 db_id_ndx,
    SOC_SAND_IN  uint32                                 entry_id_ndx
  );

/*********************************************************************
* NAME:
 *   arad_pp_fp_direct_extraction_entry_remove_unsafe
 * TYPE:
 *   PROC
 * FUNCTION:
 *   Remove an entry from the Database.
 * INPUT:
 *   SOC_SAND_IN  int                                 unit -
 *     Identifier of the device to access.
 *   SOC_SAND_IN  uint32                                 db_id_ndx -
 *     Database-ID. Range: 0 - 127.
 *   SOC_SAND_IN  uint32                                 entry_id_ndx -
 *     Entry-ID. Range: 0 - 15.
 * REMARKS:
 *   None.
 * RETURNS:
 *   OK or ERROR indication.
*********************************************************************/
uint32
  arad_pp_fp_direct_extraction_entry_remove_unsafe(
    SOC_SAND_IN  int                                 unit,
    SOC_SAND_IN  uint32                                 db_id_ndx,
    SOC_SAND_IN  uint32                                 entry_id_ndx
  );

uint32
  arad_pp_fp_direct_extraction_entry_remove_verify(
    SOC_SAND_IN  int                                 unit,
    SOC_SAND_IN  uint32                                 db_id_ndx,
    SOC_SAND_IN  uint32                                 entry_id_ndx
  );

/*********************************************************************
* NAME:
 *   arad_pp_fp_direct_extraction_db_entries_get_unsafe
 * TYPE:
 *   PROC
 * FUNCTION:
 *   Get the Database entries.
 * INPUT:
 *   SOC_SAND_IN  int                                 unit -
 *     Identifier of the device to access.
 *   SOC_SAND_IN  uint32                                 db_id_ndx -
 *     Database-ID. Range: 0 - 127.
 *   SOC_SAND_INOUT SOC_SAND_TABLE_BLOCK_RANGE                    *block_range -
 *     Block range to get entries in this Database.
 *   SOC_SAND_OUT SOC_PPC_FP_DIR_EXTR_ENTRY_INFO              *entries -
 *     Database entries.
 *   SOC_SAND_OUT uint32                                  *nof_entries -
 *     Number of valid entries.
 * REMARKS:
 *   This API can be called only if the Database is of type
 *   'direct extraction'.
 * RETURNS:
 *   OK or ERROR indication.
*********************************************************************/
uint32
  arad_pp_fp_direct_extraction_db_entries_get_unsafe(
    SOC_SAND_IN  int                                 unit,
    SOC_SAND_IN  uint32                                 db_id_ndx,
    SOC_SAND_INOUT SOC_SAND_TABLE_BLOCK_RANGE                    *block_range,
    SOC_SAND_OUT SOC_PPC_FP_DIR_EXTR_ENTRY_INFO              *entries,
    SOC_SAND_OUT uint32                                  *nof_entries
  );

uint32
  arad_pp_fp_direct_extraction_db_entries_get_verify(
    SOC_SAND_IN  int                                 unit,
    SOC_SAND_IN  uint32                                 db_id_ndx,
    SOC_SAND_INOUT SOC_SAND_TABLE_BLOCK_RANGE                    *block_range
  );

/*********************************************************************
* NAME:
 *   arad_pp_fp_control_set_unsafe
 * TYPE:
 *   PROC
 * FUNCTION:
 *   Set one of the control options.
 * INPUT:
 *   SOC_SAND_IN  int                    unit -
 *     Identifier of the device to access.
 *   SOC_SAND_IN  SOC_PPC_FP_CONTROL_INDEX       *control_ndx -
 *     Index for the control set API.
 *   SOC_SAND_IN  SOC_PPC_FP_CONTROL_INFO        *info -
 *     Type and Values of selected control option.
 *   SOC_SAND_OUT SOC_SAND_SUCCESS_FAILURE         *success -
 *     Indicate if the operation has succeeded.
 * REMARKS:
 *   The exact semantics of the info are determined by the
 *   control option specified by the type.
 * RETURNS:
 *   OK or ERROR indication.
*********************************************************************/
uint32
  arad_pp_fp_control_set_unsafe(
    SOC_SAND_IN  int                    unit,
    SOC_SAND_IN  int                    core_id,
    SOC_SAND_IN  SOC_PPC_FP_CONTROL_INDEX       *control_ndx,
    SOC_SAND_IN  SOC_PPC_FP_CONTROL_INFO        *info,
    SOC_SAND_OUT SOC_SAND_SUCCESS_FAILURE         *success
  );

uint32
  arad_pp_fp_control_set_verify(
    SOC_SAND_IN  int                    unit,
    SOC_SAND_IN  SOC_PPC_FP_CONTROL_INDEX      * control_ndx,
    SOC_SAND_IN  SOC_PPC_FP_CONTROL_INFO        *info
  );

uint32
  arad_pp_fp_control_get_verify(
    SOC_SAND_IN  int                    unit,
    SOC_SAND_IN  SOC_PPC_FP_CONTROL_INDEX      * control_ndx
  );

/*********************************************************************
*     Gets the configuration set by the
 *     "arad_pp_fp_control_set_unsafe" API.
 *     Refer to "arad_pp_fp_control_set_unsafe" API for details.
*********************************************************************/
uint32
  arad_pp_fp_control_get_unsafe(
    SOC_SAND_IN  int                    unit,
    SOC_SAND_IN  int                    core_id,
    SOC_SAND_IN  SOC_PPC_FP_CONTROL_INDEX      * control_ndx,
    SOC_SAND_OUT SOC_PPC_FP_CONTROL_INFO        *info
  );

/*********************************************************************
* NAME:
 *   arad_pp_fp_egr_db_map_set_unsafe
 * TYPE:
 *   PROC
 * FUNCTION:
 *   Set the mapping between the Packet forward type and the
 *   Port profile to the Database-ID.
 * INPUT:
 *   SOC_SAND_IN  int                    unit -
 *     Identifier of the device to access.
 *   SOC_SAND_IN  SOC_PPC_FP_FWD_TYPE            fwd_type_ndx -
 *     Packet forward type.
 *   SOC_SAND_IN  uint32                    port_profile_ndx -
 *     PP-Port profile. Range: 0 - 3.
 *   SOC_SAND_IN  uint32                     db_id -
 *     Database-Id to use for these packets. Range: 0 - 63.
 * REMARKS:
 *   1. The mapping between PP-Port and PP-Port profile is
 *   set via the soc_ppd_fp_control_set API with type
 *   SOC_PPC_FP_CONTROL_TYPE_PP_PORT_PROFILE.2. The Database with
 *   this Database-Id must exist and correspond to an Egress
 *   ACL Database
 * RETURNS:
 *   OK or ERROR indication.
*********************************************************************/
uint32
  arad_pp_fp_egr_db_map_set_unsafe(
    SOC_SAND_IN  int                    unit,
    SOC_SAND_IN  SOC_PPC_FP_FWD_TYPE            fwd_type_ndx,
    SOC_SAND_IN  uint32                    port_profile_ndx,
    SOC_SAND_IN  uint32                     db_id
  );

uint32
  arad_pp_fp_egr_db_map_set_verify(
    SOC_SAND_IN  int                    unit,
    SOC_SAND_IN  SOC_PPC_FP_FWD_TYPE            fwd_type_ndx,
    SOC_SAND_IN  uint32                    port_profile_ndx,
    SOC_SAND_IN  uint32                     db_id
  );

uint32
  arad_pp_fp_egr_db_map_get_verify(
    SOC_SAND_IN  int                    unit,
    SOC_SAND_IN  SOC_PPC_FP_FWD_TYPE            fwd_type_ndx,
    SOC_SAND_IN  uint32                    port_profile_ndx
  );

/*********************************************************************
*     Gets the configuration set by the
 *     "arad_pp_fp_egr_db_map_set_unsafe" API.
 *     Refer to "arad_pp_fp_egr_db_map_set_unsafe" API for
 *     details.
*********************************************************************/
uint32
  arad_pp_fp_egr_db_map_get_unsafe(
    SOC_SAND_IN  int                    unit,
    SOC_SAND_IN  SOC_PPC_FP_FWD_TYPE            fwd_type_ndx,
    SOC_SAND_IN  uint32                    port_profile_ndx,
    SOC_SAND_OUT uint32                     *db_id
  );

/*********************************************************************
*     Compress a TCAM Database: compress the entries to minimum
*     number of banks.
*********************************************************************/
uint32
  arad_pp_fp_database_compress_unsafe(
    SOC_SAND_IN  int                  unit,
    SOC_SAND_IN  uint32                  db_id_ndx
  );

/* Get the program and program selection bitmap */
uint32
  arad_pp_fp_packet_diag_pgm_bmp_get_unsafe(
    SOC_SAND_IN  int                 unit,
    SOC_SAND_IN  SOC_PPC_FP_DATABASE_STAGE stage,
    SOC_SAND_IN  int                    core_id,
    SOC_SAND_OUT  uint32                *pmf_progs_bmp_final,
    SOC_SAND_OUT  uint32                pfg_bmp[ARAD_PMF_NOF_LINES_MAX_ALL_STAGES]
  );

/*********************************************************************
 * NAME:
 *   arad_pp_fp_packet_diag_get_unsafe
 * TYPE:
 *   PROC
 * FUNCTION:
 *   Get the Field Processing of the last packets.
 * INPUT:
 *   SOC_SAND_IN  int                 unit -
 *     Identifier of the device to access.
 *   SOC_SAND_OUT SOC_PPC_FP_PACKET_DIAG_INFO *info -
 *     Field Processor specifications for this packet.
 * REMARKS:
 *   This API must be called during a continuous stream of
 *   the identical packets coming from the same source.
 * RETURNS:
 *   OK or ERROR indication.
*********************************************************************/
uint32
  arad_pp_fp_packet_diag_get_unsafe(
    SOC_SAND_IN  int                 unit,
    SOC_SAND_IN  int                 core_id,
    SOC_SAND_OUT SOC_PPC_FP_PACKET_DIAG_INFO *info
  );

/*********************************************************************
 * NAME:
 *   arad_pp_fp_get_dbs_for_action
 * TYPE:
 *   PROC
 * FUNCTION:
 *   Get array of all DBs on which input 'in_action_type' is in use.
 * INPUT:
 *   SOC_SAND_IN  int                     unit -
 *     Identifier of the device to access.
 *   SOC_SAND_IN  uint32                  flags -
 *     Some extra controls for this procedure, Currently not in use.
 *   SOC_SAND_IN  SOC_PPC_FP_ACTION_TYPE  in_action_type -
 *     Action to search for on all valid DBs.
 *   SOC_SAND_IN  uint32                  db_identifier_size -
 *     Maximal number of array elements in '*db_identifier_p', as allowed
 *     by the caller.
 *   SOC_SAND_OUT uint32                  *db_identifier_p -
 *     Load *db_identifier_p by identifiers of containing DBs.
 *   SOC_SAND_OUT uint32                  *db_identifier_size_actual -
 *     To be loaded by this procedure by the actual number of
 *     valid elements loaded into '*db_identifier_p'. If 'db_identifier_size'
 *     is too small to load all relevant data bases, then
 *     '*data_base_size_actual' is loaded by '-1'.
 * REMARKS:
 *    To get 'stage' out of db_identifier:
 *      arad_pp_fp_db_stage_get(unit,db_identifier,&stage) ;
 *    To get priority out of db_identifier:
 *      sw_state_access[unit].dpp.soc.arad.tm.pmf.db_info.prio.get(unit,stage,db_idx,&db_priority) ;
 *    To get TCAM identifier out of db_identifier:
 *      tcam_db_idx = ARAD_PP_FP_DB_ID_TO_TCAM_DB(db_idx);
 * SEE:
 *   arad_pp_fp_is_action_on_any_db(), arad_pp_fp_action_info_show_unsafe(),
 *   arad_pp_fp_dbs_action_info_show_unsafe()
 * RETURNS:
 *   OK or ERROR indication.
 *********************************************************************/
uint32
  arad_pp_fp_get_dbs_for_action(
    SOC_SAND_IN  int                         unit,
    SOC_SAND_IN  uint32                      flags,
    SOC_SAND_IN  SOC_PPC_FP_ACTION_TYPE      in_action_type,
    SOC_SAND_IN  uint32                      db_identifier_size,
    SOC_SAND_OUT uint32                      *db_identifier_p,
    SOC_SAND_OUT uint32                      *db_identifier_size_actual
  ) ;

/*********************************************************************
 * NAME:
 *   arad_pp_fp_is_action_on_any_db
 * TYPE:
 *   PROC
 * FUNCTION:
 *   Find out whether input 'in_action_type' is in use on any DB and
 *   specify the first using DB.
 * INPUT:
 *   SOC_SAND_IN  int                     unit -
 *     Identifier of the device to access.
 *   SOC_SAND_IN  SOC_PPC_FP_ACTION_TYPE  in_action_type -
 *     Action to search for on all valid DBs.
 *   SOC_SAND_OUT uint32                  *db_identifier_p -
 *     If input action is NOT found then return *db_identifier_p set to -1.
 *     Otherwise, set *db_identifier_p to the index of containing DB.
 * REMARKS:
 *    To get 'stage' out of db_identifier:
 *      arad_pp_fp_db_stage_get(unit,db_identifier,&stage) ;
 *    To get priority out of db_identifier:
 *      sw_state_access[unit].dpp.soc.arad.tm.pmf.db_info.prio.get(unit,stage,db_idx,&db_priority) ;
 *    To get TCAM identifier out of db_identifier:
 *      tcam_db_idx = ARAD_PP_FP_DB_ID_TO_TCAM_DB(db_idx);
 * RETURNS:
 *   OK or ERROR indication.
 *********************************************************************/
uint32
  arad_pp_fp_is_action_on_any_db(
    SOC_SAND_IN  int                         unit,
    SOC_SAND_IN  SOC_PPC_FP_ACTION_TYPE      in_action_type,
    SOC_SAND_OUT uint32                      *db_identifier_p
  ) ;

/*********************************************************************
* NAME:
 *   arad_pp_fp_dbs_action_info_show_unsafe
 * TYPE:
 *   PROC
 * FUNCTION:
 *   Show data bases corresponding to each low level action (HW level).
 * INPUT:
 *   SOC_SAND_IN  int                 unit -
 *     Identifier of the device to access.
 * REMARKS:
 *   See SOC_PPC_FP_ACTION_TYPE,
 *   bcm_petra_field_internal_to_bcm_action_map(),
 *   arad_pp_fp_get_dbs_for_action
 *   sw_state_access[unit].dpp.soc.arad.tm.pmf.db_info.prio.get
 *   _bcm_arad_field_action_info[]
 * RETURNS:
 *   OK or ERROR indication.
*********************************************************************/
uint32
  arad_pp_fp_dbs_action_info_show_unsafe(
    SOC_SAND_IN  int unit
  ) ;
/*********************************************************************
* NAME:
 *   arad_pp_fp_action_info_show_unsafe
 * TYPE:
 *   PROC
 * FUNCTION:
 *   Show BCM identifiers corresponding to each low level action (HW level).
 * INPUT:
 *   SOC_SAND_IN  int                 unit -
 *     Identifier of the device to access.
 * REMARKS:
 *   See SOC_PPC_FP_ACTION_TYPE,bcm_field_internal_to_bcm_map_t,
 *   bcm_petra_field_internal_to_bcm_action_map(),
 *   _bcm_dpp_field_ppd_to_bcm_actions(),
 *   _bcm_dpp_field_internal_stage_to_bcm_stage(),
 *   _bcm_arad_field_action_info[]
 * RETURNS:
 *   OK or ERROR indication.
*********************************************************************/
uint32
  arad_pp_fp_action_info_show_unsafe(
    SOC_SAND_IN  int unit
  ) ;

/*********************************************************************
* NAME:
 *   arad_pp_fp_resource_diag_get_unsafe
 * TYPE:
 *   PROC
 * FUNCTION:
 *   Get the full resources usage.
 * INPUT:
 *   SOC_SAND_IN  int                 unit -
 *     Identifier of the device to access.
 *   SOC_SAND_OUT SOC_PPC_FP_RESOURCE_DIAG_INFO *info -
 *     Field Processor specifications for the resources.
 * REMARKS:
 *   This API must be called .....
 * RETURNS:
 *   OK or ERROR indication.
*********************************************************************/
uint32
  arad_pp_fp_resource_diag_get_verify(
    SOC_SAND_IN  int                 		    unit,
    SOC_SAND_IN  SOC_PPC_FP_RESOURCE_DIAG_MODE	mode,
    SOC_SAND_OUT SOC_PPC_FP_RESOURCE_DIAG_INFO	*info
  );

uint32
  arad_pp_fp_resource_diag_get_unsafe(
    SOC_SAND_IN  int                 		unit,
	SOC_SAND_IN  SOC_PPC_FP_RESOURCE_DIAG_MODE	mode,
    SOC_SAND_OUT SOC_PPC_FP_RESOURCE_DIAG_INFO 	*info
  );

/*********************************************************************
* NAME:
 *   arad_pp_fp_get_procs_ptr
 * TYPE:
 *   PROC
 * FUNCTION:
 *   Get the pointer to the list of procedures of the
 *   arad_pp_api_fp module.
 * INPUT:
 * REMARKS:
 *
 * RETURNS:
 *   OK or ERROR indication.
*********************************************************************/
CONST SOC_PROCEDURE_DESC_ELEMENT*
  arad_pp_fp_get_procs_ptr(void);

/*********************************************************************
* NAME:
 *   arad_pp_fp_get_errs_ptr
 * TYPE:
 *   PROC
 * FUNCTION:
 *   Get the pointer to the list of errors of the
 *   arad_pp_api_fp module.
 * INPUT:
 * REMARKS:
 *
 * RETURNS:
 *   OK or ERROR indication.
*********************************************************************/
CONST SOC_ERROR_DESC_ELEMENT*
  arad_pp_fp_get_errs_ptr(void);

uint32
  SOC_PPC_FP_QUAL_VAL_verify(
      SOC_SAND_IN  int                 unit,
      SOC_SAND_IN  SOC_PPC_FP_QUAL_VAL *info,
      SOC_SAND_IN     SOC_PPC_FP_DATABASE_STAGE    stage
  );

uint32
  SOC_PPC_PMF_PFG_INFO_verify(
      SOC_SAND_IN  int                    unit,
      SOC_SAND_IN  SOC_PPC_PMF_PFG_INFO *info
  );

uint32
  SOC_PPC_FP_DATABASE_INFO_verify(
    SOC_SAND_IN  SOC_PPC_FP_DATABASE_INFO *info
  );

uint32
  SOC_PPC_FP_ACTION_VAL_verify(
    SOC_SAND_IN  SOC_PPC_FP_ACTION_VAL *info
  );

uint32
  SOC_PPC_FP_ENTRY_INFO_verify(
      SOC_SAND_IN  int                 unit,
      SOC_SAND_IN  SOC_PPC_FP_ENTRY_INFO *info,
      SOC_SAND_IN     SOC_PPC_FP_DATABASE_STAGE    stage
  );

uint32
  SOC_PPC_FP_DIR_EXTR_ACTION_LOC_verify(
      SOC_SAND_IN  int                    unit,
      SOC_SAND_IN  SOC_PPC_FP_DIR_EXTR_ACTION_LOC *info,
      SOC_SAND_IN  SOC_PPC_FP_DATABASE_STAGE    stage,
      SOC_SAND_IN  uint8                           is_large_direct_extraction
  );

uint32
  SOC_PPC_FP_DIR_EXTR_ACTION_VAL_verify(
      SOC_SAND_IN  int                    unit,
      SOC_SAND_IN  SOC_PPC_FP_DIR_EXTR_ACTION_VAL *info,
      SOC_SAND_IN  SOC_PPC_FP_DATABASE_STAGE    stage,
      SOC_SAND_IN  uint8                           is_large_direct_extraction
  );

uint32
  SOC_PPC_FP_DIR_EXTR_ENTRY_INFO_verify(
      SOC_SAND_IN  int                    unit,
      SOC_SAND_IN  SOC_PPC_FP_DIR_EXTR_ENTRY_INFO *info,
      SOC_SAND_IN  SOC_PPC_FP_DATABASE_STAGE    stage,
      SOC_SAND_IN  uint8                           is_large_direct_extraction
  );

uint32
  SOC_PPC_FP_CONTROL_INDEX_verify(
    SOC_SAND_IN  SOC_PPC_FP_CONTROL_INDEX *info
  );

uint32
  SOC_PPC_FP_CONTROL_INFO_verify(
    SOC_SAND_IN  int                     unit,
    SOC_SAND_IN  SOC_PPC_FP_CONTROL_TYPE type,
    SOC_SAND_IN  SOC_PPC_FP_CONTROL_INFO *info
  );

/*********************************************************************
* NAME:
 *   arad_pp_fp_presel_max_id_get
 * TYPE:
 *   PROC
 * FUNCTION:
 *   Given a stage, returns the maximum preselector index for that stage. 
 * INPUT:
 *   int                        unit            - (IN) Device to be checked.
 *   SOC_PPC_FP_DATABASE_STAGE     stage           - (IN) stage.
 *   uint32                     presel_max_id   - (OUT) maximum presel id.
 * RETURNS:
 *   OK or ERROR indication.
*********************************************************************/
soc_error_t
    arad_pp_fp_presel_max_id_get(
      SOC_SAND_IN  int                      unit,
      SOC_SAND_IN  SOC_PPC_FP_DATABASE_STAGE   stage,
      SOC_SAND_OUT int                   *presel_max_id 
    );

uint32
arad_pp_fp_pp_test1(
    SOC_SAND_IN  int                           unit);

uint32
    arad_pp_fp_ire_traffic_send_verify(
        SOC_SAND_IN int          unit,
        SOC_SAND_IN SOC_PPC_FP_PACKET  *packet
    );

uint32
    arad_pp_fp_ire_traffic_send_unsafe(
        SOC_SAND_IN int          unit,
        SOC_SAND_IN SOC_PPC_FP_PACKET  *packet,
        SOC_SAND_IN int          core
    );

uint32
  arad_pp_fp_rewrite_entry(
     SOC_SAND_IN  int                        unit,
     SOC_SAND_IN  uint8                      entry_exists,
     SOC_SAND_IN  ARAD_TCAM_GLOBAL_LOCATION  *global_location,
     SOC_SAND_IN  ARAD_TCAM_LOCATION         *location
  );

uint32
  arad_pp_fp_qual_type_preset(
    SOC_SAND_IN  int                    unit,
    SOC_SAND_IN  SOC_PPC_FP_PREDEFINED_ACL_KEY  predefined_key,
    SOC_SAND_OUT SOC_PPC_FP_QUAL_TYPE           qual_types[SOC_PPC_FP_NOF_QUALS_PER_DB_MAX]
  );

uint32
  arad_pp_fp_qual_val_encode(
    SOC_SAND_IN  int                      unit,
    SOC_SAND_IN  SOC_PPC_FP_QUAL_VAL_ENCODE_INFO *qual_val_encode,
    SOC_SAND_OUT SOC_PPC_FP_QUAL_VAL             *qual_val
  );


/* } */

#include <soc/dpp/SAND/Utils/sand_footer.h>

/* } __ARAD_PP_FP_INCLUDED__*/
#endif


