/**
 * \file bcm_int/dnx/field/field_entry.h
 *
 *
 * Field entry procedures for DNX.
 *
 * Management for pushing entries into TCAM/ MDB / State Table / Action payload of TCAM (DT)
 */
/*
 * 
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

#ifndef FIELD_FIELD_ENTRY_H_INCLUDED
/* { */
#define FIELD_FIELD_ENTRY_H_INCLUDED

#ifndef BCM_DNX_SUPPORT
#error "This file is for use by DNX (JR2) family only!"
#endif
/*
 * Include files
 * {
 */
#include <include/bcm_int/dnx/field/field.h>
#include <soc/dnx/dnx_data/auto_generated/dnx_data_device.h>
#include <soc/dnx/dnx_data/auto_generated/dnx_data_field.h>
#include <soc/dnx/dnx_data/auto_generated/dnx_data_max_field.h>
#include <soc/dnx/swstate/auto_generated/access/dnx_field_tcam_access_access.h>

/*
 * }
 */
/*
 * The same as DNX_DATA_MAX_FIELD_COMMON_MAX_VAL_NOF_BITS_IN_FES_ACTION but expressed in
 * units of 'uint32'. Rounded up.
 */
#define DNX_DATA_MAX_FIELD_PMF_NOF_UINT32S_MAX_IN_FES_ACTION                    \
        ((DNX_DATA_MAX_FIELD_COMMON_MAX_VAL_NOF_BITS_IN_FES_ACTION + SAL_UINT32_NOF_BITS - 1) / SAL_UINT32_NOF_BITS)

/* COMMON to INTERNAL TCAM and DT */
#define DNX_DATA_FIELD_ENTRY_TCAM_INTERNAL_DT_CORE_ID_SHIFT    20

/* INTERNAL */
#define DNX_DATA_FIELD_ENTRY_TCAM_INTERNAL_ACCESS_ID_MASK   0xFFFFF
#define DNX_DATA_FIELD_ENTRY_TCAM_INTERNAL_CORE_ID_SHIFT    DNX_DATA_FIELD_ENTRY_TCAM_INTERNAL_DT_CORE_ID_SHIFT

/* EXTERNAL */
#define DNX_DATA_FIELD_ENTRY_TCAM_EXTERNAL_ACCESS_ID_SIZE   26
#define DNX_DATA_FIELD_ENTRY_TCAM_EXTERNAL_ACCESS_ID_MASK   0x3FFFFFF

/* DT */
#define DNX_DATA_FIELD_ENTRY_TCAM_DT_ACCESS_ID_MASK             0xFFF
#define DNX_DATA_FIELD_ENTRY_TCAM_DT_FG_ID_SHIFT                12
#define DNX_DATA_FIELD_ENTRY_TCAM_DT_FG_ID_MASK                 0xFF
#define DNX_DATA_FIELD_ENTRY_TCAM_DT_CORE_ID_SHIFT              DNX_DATA_FIELD_ENTRY_TCAM_INTERNAL_DT_CORE_ID_SHIFT
#define DNX_DATA_FIELD_ENTRY_TCAM_DT_FG_TYPE_SHIFT              30
#define DNX_DATA_FIELD_ENTRY_TCAM_DT_FG_TYPE_VAL                0x1

/* STATE TABLE */
#define DNX_DATA_FIELD_ENTRY_STATE_TABLE_ADDRESS_MASK      0xFFFF
#define DNX_DATA_FIELD_ENTRY_STATE_TABLE_CORE_ID_SHIFT     20

/* COMMON */
#define DNX_DATA_FIELD_ENTRY_CORE_ID_MASK                  0xF

/** Get core from internal TCAM. */
#define DNX_FIELD_ENTRY_TCAM_INTERNAL_CORE_ID(entry_handle)        \
        (((entry_handle) >> DNX_DATA_FIELD_ENTRY_TCAM_INTERNAL_CORE_ID_SHIFT) & DNX_DATA_FIELD_ENTRY_CORE_ID_MASK)

/** Get core from TCAM DT. */
#define DNX_FIELD_ENTRY_TCAM_DT_CORE_ID(entry_handle)        \
        (((entry_handle) >> DNX_DATA_FIELD_ENTRY_TCAM_DT_CORE_ID_SHIFT) & DNX_DATA_FIELD_ENTRY_CORE_ID_MASK)

/** Get core from either internal TCAM or TCAM DT. */
#define DNX_FIELD_ENTRY_TCAM_INTERNAL_DT_CORE_ID(entry_handle)        \
        (((entry_handle) >> DNX_DATA_FIELD_ENTRY_TCAM_INTERNAL_DT_CORE_ID_SHIFT) & DNX_DATA_FIELD_ENTRY_CORE_ID_MASK)

/** Get core from state table. */
#define DNX_FIELD_ENTRY_STATE_TABLE_CORE_ID(entry_handle)        \
        (((entry_handle) >> DNX_DATA_FIELD_ENTRY_STATE_TABLE_CORE_ID_SHIFT) & DNX_DATA_FIELD_ENTRY_CORE_ID_MASK)

/** Get address from state table. */
#define DNX_FIELD_ENTRY_STATE_TABLE_ADDRESS(entry_handle)        \
        ((entry_handle) & DNX_DATA_FIELD_ENTRY_STATE_TABLE_ADDRESS_MASK)

/** Get access ID from internal TCAM. */
#define DNX_FIELD_ENTRY_TCAM_INTERNAL_ACCESS_ID(entry_handle)         \
        ((entry_handle) & DNX_DATA_FIELD_ENTRY_TCAM_INTERNAL_ACCESS_ID_MASK)

/** Get access ID from external TCAM (KBP). */
#define DNX_FIELD_ENTRY_TCAM_EXTERNAL_ACCESS_ID(entry_handle)         \
        ((entry_handle) & DNX_DATA_FIELD_ENTRY_TCAM_EXTERNAL_ACCESS_ID_MASK)

/** Get access ID from DT. */
#define DNX_FIELD_ENTRY_TCAM_DT_ACCESS_ID(entry_handle)         \
        ((entry_handle) & DNX_DATA_FIELD_ENTRY_TCAM_DT_ACCESS_ID_MASK)

/** This macro encodes the entry handle that is passed to TCAM.
 *
 * TCAM Entry Id is encoded in the following way (from MSB to LSB):
 * NOT USED         (8 bits)
 * CORE ID          (4 bits)
 * ACCESS_ID        (20 bits)
 */
#define DNX_FIELD_ENTRY_TCAM_INTERNAL(access_id, core_id)  \
        ((access_id & DNX_DATA_FIELD_ENTRY_TCAM_INTERNAL_ACCESS_ID_MASK) | \
         ((core_id & DNX_DATA_FIELD_ENTRY_CORE_ID_MASK) << \
           DNX_DATA_FIELD_ENTRY_TCAM_INTERNAL_CORE_ID_SHIFT))

/** This macro encodes the entry handle that is passed to External TCAM.
 *
 * EXTERNAL TCAM Entry Id is encoded in the following way (from MSB to LSB):
 * NOT USED         (6 bits)
 * ACCESS_ID        (26 bits)
 */
#define DNX_FIELD_ENTRY_TCAM_EXTERNAL(access_id)  \
        ((access_id & DNX_DATA_FIELD_ENTRY_TCAM_EXTERNAL_ACCESS_ID_MASK))

/** This macro encodes the entry handle that is passed to direct TCAM handler.
 * Note that the MSB bit #31 is 1 for backwards compatibility.
 *
 * EXTERNAL TCAM Entry Id is encoded in the following way (from MSB to LSB):
 * FG TYPE          (2 bit, value=0b01)
 * NOT USED         (6 bits)
 * CORE ID          (4 bits)
 * FG ID            (8 bits)
 * ACCESS_ID        (12 bits)
 */
#define DNX_FIELD_ENTRY_TCAM_DT(access_id, fg_id, core_id)  \
        ((DNX_DATA_FIELD_ENTRY_TCAM_DT_FG_TYPE_VAL << DNX_DATA_FIELD_ENTRY_TCAM_DT_FG_TYPE_SHIFT) | \
         ((core_id & DNX_DATA_FIELD_ENTRY_CORE_ID_MASK) << DNX_DATA_FIELD_ENTRY_TCAM_DT_CORE_ID_SHIFT) | \
         ((fg_id & DNX_DATA_FIELD_ENTRY_TCAM_DT_FG_ID_MASK) << DNX_DATA_FIELD_ENTRY_TCAM_DT_FG_ID_SHIFT) | \
         (access_id & DNX_DATA_FIELD_ENTRY_TCAM_DT_ACCESS_ID_MASK))

/** This macro encodes the entry handle for state table field group. Includes the entry address and core.
 * 
 * NOT USED         (8 bits)
 * CORE_ID          (4 bits)
 * NOT USED         (10-4 bits)
 * ADDRESS          (10-16 bits)
 */
#define DNX_FIELD_ENTRY_STATE_TABLE(address, core_id)  \
        (((core_id & DNX_DATA_FIELD_ENTRY_CORE_ID_MASK) << DNX_DATA_FIELD_ENTRY_STATE_TABLE_CORE_ID_SHIFT) | \
         (address & DNX_DATA_FIELD_ENTRY_STATE_TABLE_ADDRESS_MASK))

/*
 * enum
 * {
 */

/**
 * Holds possible flags for entry creation/updating.
 * Used in dnx_field_ace_entry_add()/dnx_field_entry_tcam_add()
 */
typedef enum
{
    /**
     * Creates entry with a user-specified ID.
     */
    DNX_FIELD_ENTRY_ADD_FLAG_WITH_ID = 0x1,
    /**
     * Entry is not added, but rather updated.
     * - For ACE entries, only payload can be updated
     * - For TCAM entries, key/payload/valid_bit can be updated.
     */
    DNX_FIELD_ENTRY_ADD_FLAG_UPDATE = 0x2,
} dnx_field_entry_add_flags_e;

/*
 * }
 */

/*
 * Struct
 * {
 */

/**
 * Actions data, as specified by the user. To be used to load action values to HW.
 * See dnx_field_entry_payload_t.
 */
typedef struct
{

    /*
     *  DNX Action applied, For iPMF1-2-3/EPMF. Selects the action for which the action value is to be applies.
     *  See field_map_data for details
     */
    dnx_field_action_t dnx_action;

    /*
     * The value to be filled inside the FES action result, for the corresponding dnx_action.
     * The number of bits to take from the uint32 (starting with the lsb)
     * will be determined by the field group (and ultimately, from the action type).
     */
    uint32 action_value[DNX_DATA_MAX_FIELD_ENTRY_NOF_ACTION_PARAMS_PER_ENTRY];

} dnx_field_entry_action_t;

/**
 * The content of the payload of an entry.
 * See dnx_field_entry_t.
 * See dnx_field_entry_tcam_add.
 * See dnx_field_entry_exem_add.
 */
typedef struct
{

    /*
     *  The array of actions from which the entry payload is composed.
     */
    dnx_field_entry_action_t action_info[DNX_DATA_MAX_FIELD_COMMON_MAX_VAL_NOF_ACTION_PER_GROUP];

} dnx_field_entry_payload_t;

/**
 * Qualifier data that is being used to build the key to be inserted into HW,
 * including masks (for databases such as TCAM).
 * EXEM DB will not use it
 *
 * See dnx_field_entry_key_t
 */
typedef struct
{

    /** Dnx qualifier */
    dnx_field_qual_t dnx_qual;

    /*
     * The value to be filled for the part of the key that would be compared against the bits obtained by
     * each qualifier type. For ranged input, serves as minimum value.
     */
    uint32 qual_value[DNX_DATA_MAX_FIELD_ENTRY_NOF_QUAL_PARAMS_PER_ENTRY];
    /*
     * The maximum value, for ranged input only.
     */
    uint32 qual_max_value[DNX_DATA_MAX_FIELD_ENTRY_NOF_QUAL_PARAMS_PER_ENTRY];
    /*
     * The mask to be filled for the part of the key that would be compared against the bits obtained by
     * each qualifier type.
     */
    uint32 qual_mask[DNX_DATA_MAX_FIELD_ENTRY_NOF_QUAL_PARAMS_PER_ENTRY];

} dnx_field_entry_qual_t;

/**
 * Structure of the key for an entry that require a mask (like TCAM).
 * Contains the qualifiers data that is used to be inserted into HW.
 * See dnx_field_entry_t.
 * See dnx_field_entry_tcam_add.
 */
typedef struct
{
    /*
     *  The array of qualifiers from which the entry key is composed.
     */
    dnx_field_entry_qual_t qual_info[DNX_DATA_MAX_FIELD_GROUP_NOF_QUALS_PER_FG];

} dnx_field_entry_key_t;

/**Hold the in params for dnx_field_entry_tcam_add API*/
typedef struct
{
    /**
    * Each entry has its priority, meaning where the entry should be located in the Field Group
    * TCAM. Lower priority value means higher precedence, which means the TCAM hit with that entry should occur first.
    */
    uint32 priority;

    /** Hold the qualifier values to be filled in TCAM key*/
    dnx_field_entry_key_t key_info;

    /** Hold the actions values to be filled in TCAM payload*/
    dnx_field_entry_payload_t payload_info;

    /** Core to add the entry on */
    int core;

    /** Valid bit, if set to false, entry will exist but won't be active */
    uint8 valid_bit;

} dnx_field_entry_t;

/*
 * }
 */

/**
* \brief
*  Init structure type of dnx_field_entry_t to Invalid values
* \param [in] unit            - Device ID
* \param [in] entry_add_in_p  - Pointer to structure type of dnx_field_entry_t
* \return
*   shr_error_e - Error Type
* \remark
*   * None
* \see
*   * None
*/
shr_error_e dnx_field_entry_t_init(
    int unit,
    dnx_field_entry_t * entry_add_in_p);

/**
 * \brief
 *  Init Structure dnx_field_entry_payload_t to invalid params
 * \param [in] unit            - Device Id
 * \param [in] payload_info_p  - Structure pointer to init
 * \return
 *   shr_error_e - Error Type
 * \remark
 *   * None
 * \see
 *   * None
 */
shr_error_e dnx_field_entry_payload_t_init(
    int unit,
    dnx_field_entry_payload_t * payload_info_p);

/**
* \brief
*  Add a new entry to indicated TCAM database or TCAM cache list.
*  On input, specify all actions and qualifiers to go into that entry in hardware:
*    Actions will go into 'actions payload' and
*    qualifiers will go into 'entry key'. See 'remarks' below
*
* \param [in] unit            - Device ID
* \param [in] flags           - TCAM entry addition flags, currently supported flags can be (WITH_ID | UPDATE)
* \param [in] fg_id           - The ID of the field group to which we add the entry.
* \param [in] entry_info_p    - Entry info to configure into TCAM
* \param [out] entry_handle_p - Entry id handler to return to user
*
* \return
*   shr_error_e - Error Type
* \remark
*   * The action payload construction and key construction is done in dnx_field_group_context_attach()
*   * Field group  Id can be found in dnx_field_entry_t->fg_id.
*     It is used to extract the relevant data regarding the TCAM key and payload and which TCAM needs to be accessed.
*     User can fill a subset of qualifier and actions, the DBAL will complete the qualifier to dont_cares and actions to 0 (invalid)
* \see
*   * None
*/
shr_error_e dnx_field_entry_tcam_add(
    int unit,
    uint32 flags,
    dnx_field_group_t fg_id,
    dnx_field_entry_t * entry_info_p,
    uint32 *entry_handle_p);

/**
* \brief
*  Add a new entry to indicated TCAM database: calling TCAM manager to allocate a location for the entry.
*  On input, specify all actions and qualifiers to go into that entry in hardware:
*    Actions will go into 'actions payload' and
*    qualifiers will go into 'entry key'. See 'remarks' below
*
* \param [in] unit            - Device ID
* \param [in] flags           - TCAM entry addition flags, currently supported flags can be (WITH_ID | UPDATE)
* \param [in] fg_id           - The ID of the field group to which we add the entry.
* \param [in] entry_info_p    - Entry info to configure into TCAM
* \param [out] entry_handle_p - Entry id handler to return to user
*
* \return
*   shr_error_e - Error Type
* \remark
*   * The action payload construction and key construction is done in dnx_field_group_context_attach()
*   * Field group  Id can be found in dnx_field_entry_t->fg_id.
*     It is used to extract the relevant data regarding the TCAM key and payload and which TCAM needs to be accessed.
*     User can fill a subset of qualifier and actions, the DBAL will complete the qualifier to dont_cares and actions to 0 (invalid)
* \see
*   * None
*/
shr_error_e dnx_field_entry_tcam_install(
    int unit,
    uint32 flags,
    dnx_field_group_t fg_id,
    dnx_field_entry_t * entry_info_p,
    uint32 *entry_handle_p);

/**
* \brief
*  Add a new entry to indicated TCAM DT database.
*  On input, specify all actions and qualifiers to go into that entry in hardware:
*    Actions will go into 'actions payload' and
*    qualifiers will go into 'entry key'. See 'remarks' below
*
* \param [in] unit            - Device ID
* \param [in] flags           - TBD
* \param [in] fg_id           - The ID of the TCAM DT field group to which we add the entry.
* \param [in] entry_info_p    - Entry info to configure into TCAM
* \param [out] entry_handle_p - Entry id handler to return to user
*
* \return
*   shr_error_e - Error Type
* \remark
*   * The action payload construction and key construction is done in dnx_field_group_context_attach()
*   * Field group  Id can be found in dnx_field_entry_t->fg_id.
*     It is used to extract the relevant data regarding the TCAM key and payload and which TCAM needs to be accessed.
*     User can fill a subset of qualifier and actions, the DBAL will complete the qualifier to dont_cares and actions to 0 (invalid)
* \see
*   * None
*/
shr_error_e dnx_field_entry_tcam_dt_add(
    int unit,
    uint32 flags,
    dnx_field_group_t fg_id,
    dnx_field_entry_t * entry_info_p,
    uint32 *entry_handle_p);

/**
* \brief
* Add a new entry to indicated EXEM data base.
* On input, specify all actions and qualifiers to go into that entry in hardware:
*    Actions will go into 'actions payload' and
*    qualifiers will go into 'entry key'. See 'remarks' below
* \param [in] unit           - Device ID
* \param [in] flags          - entry addition flags, currently only supported flag is
*                              DNX_FIELD_ENTRY_ADD_FLAG_UPDATE
* \param [in] fg_id          - The ID of the field group to which we add the entry.
* \param [in] entry_info_p   - Entry info to configure into the database.
* \return
*   shr_error_e - Error Type
* \remark
*   * The action payload construction and key construction is done in dnx_field_group_context_attach()
*   * Field group Id is used to extract the relevant data regarding the EXEM key and payload with which EXEM needs to 
*     be accessed. User can fill a subset of and actions, the DBAL will complete the actions to 0 (Including setting 
*     the valid bit to zero, thus invalidating the actions). 
*     However, unlike TCAM, all qualifiers must be given, since we don't have a mask in exact match and the value of 
*     every qualifier is compared in the lookup.
* \see
*   * None
*/
shr_error_e dnx_field_entry_exem_add(
    int unit,
    uint32 flags,
    dnx_field_group_t fg_id,
    dnx_field_entry_t * entry_info_p);

/**
* \brief
* Add a new entry to indicated MDB DT data base.
* On input, specify all actions and qualifiers to go into that entry in hardware:
*    Actions will go into 'actions payload' and
*    qualifiers will go into 'entry key'. See 'remarks' below
* \param [in] unit           - Device ID
* \param [in] flags          - TBD
* \param [in] fg_id          - The ID of the field group to which we add the entry.
* \param [in] entry_info_p   - Entry info to configure into the database.
* \return
*   shr_error_e - Error Type
* \remark
*   * The action payload construction and key construction is done in dnx_field_group_context_attach()
*   * Field group Id is used to extract the relevant data regarding the MDB DT key and payload with which MDB DT needs to 
*     be accessed. User can fill a subset of and actions, the DBAL will complete the actions to 0 (Including setting 
*     the valid bit to zero, thus invalidating the actions). 
*     However, unlike TCAM, all qualifiers must be given, since we don't have a mask in exact match and the value of 
*     every qualifier is compared in the lookup.
* \see
*   * None
*/
shr_error_e dnx_field_entry_mdb_dt_add(
    int unit,
    uint32 flags,
    dnx_field_group_t fg_id,
    dnx_field_entry_t * entry_info_p);

/**
* \brief
* Add a new entry to indicated External TCAM (KBP) data base.
* On input, specify all actions and qualifiers to go into that entry in hardware:
*    Actions will go into 'actions payload' and
*    qualifiers will go into 'entry key'.
* \param [in] unit            - Device ID
* \param [in] flags           - TCAM entry addition flags, currently supported flags can be (WITH_ID | UPDATE)
* \param [in] fg_id           - The ID of the field group to which we add the entry.
* \param [in] entry_info_p    - Entry info to configure into TCAM
* \param [out] entry_handle_p - Entry id handler to return to user
* \return
*   shr_error_e - Error Type
* \remark
* \see
*   * None
*/
shr_error_e dnx_field_entry_kbp_add(
    int unit,
    uint32 flags,
    dnx_field_group_t fg_id,
    dnx_field_entry_t * entry_info_p,
    uint32 *entry_handle_p);

/**
* \brief
*  Adds a new entry to State Table at the given address and sets its data as the given data
*
* \param [in] unit             - Device ID
* \param [in] stage            - Stage of the qualifiers
* \param [in] entry_key_info_p - Entry key info. Address and data are extracted from it.
* \param [out] core_id         - The core ID to write to.
* \param [out] entry_handle_p  - Entry id handler to return to user.
*
* \return
*   shr_error_e - Error Type
* \remark
*   * This function ignores the Do-Write bit.
* \see
*   * None
*/
shr_error_e dnx_field_entry_state_table_add(
    int unit,
    dnx_field_stage_e stage,
    dnx_field_entry_key_t * entry_key_info_p,
    int core_id,
    uint32 *entry_handle_p);

/**
* \brief
*  Deletes the State Table entry at the given address.
*
* \param [in] unit    - Device ID
* \param [in] stage            - Stage of the qualifiers
* \param [in] core_id          - Core to delete in.
* \param [in] address          - address of the entry within the core.
*
* \return
*   shr_error_e - Error Type
* \remark
*   * None
* \see
*   * None
*/
shr_error_e dnx_field_entry_state_table_delete(
    int unit,
    dnx_field_stage_e stage,
    int core_id,
    uint32 address);

/**
* \brief
*  Gets the State Table entry data at the given address
*
* \param [in] unit    - Device ID
* \param [in] stage            - Stage of the qualifiers
* \param [in] fg_id            - The field group for qualifier parsing.
* \param [in] core_id          - The core to read from.
* \param [in] address          - The entry address.
* \param [out] entry_key_info_p - Entry key info. taken from address and data.
*
* \return
*   shr_error_e - Error Type
* \remark
*   * None
* \see
*   * None
*/
shr_error_e dnx_field_entry_state_table_get(
    int unit,
    dnx_field_stage_e stage,
    dnx_field_group_t fg_id,
    int core_id,
    uint32 address,
    dnx_field_entry_key_t * entry_key_info_p);

/**
* \brief
* Add a new entry to indicated ACE format.
* On input, specify all actions and their values to go into that entry in hardware.
* \param [in] unit                 - Device ID
* \param [in] flags                - Can be WITH_ID or UPDATE. If UPDATE flag is set, WITH_ID flag is ignored.
* \param [in] ace_id               - ACE format ID
* \param [in] payload_info_p       - The actions and their value to fill in the payload of the entry.
* \param [in, out] ace_entry_key_p - The entry of the ACE key, the value to be written in the CUD_OUTLIF_OR_MCDB_PTR
*                                    action of eMPF for the entry to be matched in the ACE table lookup.
*                                    Input if WITH_ID or UPDATE flags are set.
* \return
*   shr_error_e - Error Type
* \remark
*   * The action payload construction and key construction is done in dnx_field_ace_format_add()
*   * User can fill a subset of the actions, the DBAL will complete the actions to 0 
*     (Including setting the valid bit to zero, thus invalidating the actions).
* \see
*   * None
*/
shr_error_e dnx_field_ace_entry_add(
    int unit,
    uint32 flags,
    dnx_field_ace_id_t ace_id,
    dnx_field_entry_payload_t * payload_info_p,
    dnx_field_ace_key_t * ace_entry_key_p);

/**
* \brief
*  This function allocates Id that will later be used by TCAM to know where to put the entry inside TCAM banks of the given core
*  also configure the TCAM manager to find place in the TCAM banks of the given core to put the entry and connect it to the
*  allocated access id
*
* \param [in] unit                - Device Id
* \param [in] core                - Core ID
* \param [in] dbal_table_id       - static DBAL table that was create for the sake of the input entry,
*                                                 static since api used by non-PMF stages
* \param [in] priority            - Entry priority, see dnx_field_tcam_entry_t
* \param [out] entry_access_id_p  - Access_id that was allocated for this entry and
* \return
*   shr_error_e - Error Type
* \remark
*   * This function is being used by non PMF stages (VTT/FLP..) stages that use static tables for TCAM
* \see
*   * dnx_field_entry_access_id_alloc() to see how entry_access_id_p is encoded
*/
shr_error_e dnx_field_entry_access_id_create(
    int unit,
    int core,
    dbal_tables_e dbal_table_id,
    uint32 priority,
    uint32 *entry_access_id_p);

/**
* \brief
*  Destroy an entry_acces_id for specific DBAL table
* Deallocates the access id
* \param [in] unit             - Device ID
* \param [in] entry_access_id  - Access Id that should be destroyed
* \return
*   shr_error_e - Error Type
* \remark
*   * None
* \see
*   * None
*/
shr_error_e dnx_field_entry_access_id_deallocate(
    int unit,
    uint32 entry_access_id);

/**
* \brief
*  Destroy an entry_acces_id for specific DBAL table
* Deallocates the access id and delete information from TCAM manager to free the entry
* \param [in] unit             - Device ID
* \param [in] dbal_table_id    - DBAL table
* \param [in] entry_access_id  - Access Id that should be destroyed
* \return
*   shr_error_e - Error Type
* \remark
*   * None
* \see
*   * None
*/
shr_error_e dnx_field_entry_access_id_destroy(
    int unit,
    dbal_tables_e dbal_table_id,
    uint32 entry_access_id);

/**
* \brief
*  Destoy all entries that were allocated for specific Dbal table
*  Deallocates all access id's and deletes information from TCAM manager to free all entries
* \param [in] unit           - Device ID
* \param [in] dbal_table_id  - DBAL table to free
* \return
*   shr_error_e - Error Type
* \remark
*   * None
* \see
*   * None
*/
shr_error_e dnx_field_entry_access_id_destroy_all(
    int unit,
    dbal_tables_e dbal_table_id);

/**
* \brief
*  Delete all entries that were allocated for the given tcam handler id.
* \param [in] unit             - Device ID
* \param [in] fg_id            - ID of the field group to delete its entries
* \return
*   shr_error_e - Error Type
* \remark
*   * None
* \see
*   * None
*/
shr_error_e dnx_field_entry_tcam_delete_all(
    int unit,
    dnx_field_group_t fg_id);

/**
* \brief
*  Delete all entries in the state table (for all state table field groups).
* \param [in] unit             - Device ID
* \return
*   shr_error_e - Error Type
* \remark
*   * None
* \see
*   * None
*/
shr_error_e dnx_field_entry_state_table_all_delete_all(
    int unit);

/**
* \brief
*  Delete TCAM Entry From HW and free all allocated resources
* \param [in] unit          - Device ID
* \param [in] fg_id         - Entry's FG ID
* \param [in] entry_handle  - unique identifier of entry
* \param [in] key_info_p    - Key information for DT entries
* \return
*   shr_error_e - Error Type
* \remark
*   * None
* \see
*   * dnx_field_group_entry_delete_all
*/
shr_error_e dnx_field_entry_tcam_delete(
    int unit,
    dnx_field_group_t fg_id,
    uint32 entry_handle,
    dnx_field_entry_key_t * key_info_p);

/**
* \brief
*  Delete External TCAM (KBP) Entry From HW and free all allocated resources
* \param [in] unit          - Device ID
* \param [in] fg_id         - Entry's FG ID
* \param [in] entry_handle  - unique identifier of entry
* \return
*   shr_error_e - Error Type
* \remark
*   * None
* \see
*   * dnx_field_group_entry_delete_all
*/
shr_error_e dnx_field_entry_kbp_delete(
    int unit,
    bcm_field_group_t fg_id,
    uint32 entry_handle);

/**
* \brief
*  Delete EXEM Entry From HW.
* \param [in] unit          - Device ID
* \param [in] fg_id         - The ID of the field group in which the entry we want to delete.
* \param [in] entry_key_p   - The key of the entry to delete, used as identifier.
* \return
*   shr_error_e - Error Type
* \remark
*   * None
* \see
*   * dnx_field_group_entry_delete_all
*/
shr_error_e dnx_field_entry_exem_delete(
    int unit,
    dnx_field_group_t fg_id,
    dnx_field_entry_key_t * entry_key_p);

/**
* \brief
*  Delete MDB DT Entry From HW.
* \param [in] unit          - Device ID
* \param [in] fg_id         - The ID of the field group in which the entry we want to delete.
* \param [in] entry_key_p   - The key of the entry to delete, used as identifier.
* \return
*   shr_error_e - Error Type
* \remark
*   * None
* \see
*   * dnx_field_group_entry_delete_all
*/
shr_error_e dnx_field_entry_mdb_dt_delete(
    int unit,
    dnx_field_group_t fg_id,
    dnx_field_entry_key_t * entry_key_p);


shr_error_e dnx_field_ace_entry_delete(
    int unit,
    dnx_field_ace_key_t ace_entry_key);

/**
* \brief
*  Get info for specific TCAM Field group by entry id
* \param [in] unit           - Device ID
* \param [in] fg_id          - Entry's FG ID
* \param [in] entry_handle   - unique entry handle
* \param [out] entry_info_p  - Pointer to dnx_field_entry_t to get info from TCAM HW
* \return
*   shr_error_e - Error Type
* \remark
*   * None
* \see
*   * None
*/
shr_error_e dnx_field_entry_tcam_get(
    int unit,
    dnx_field_group_t fg_id,
    uint32 entry_handle,
    dnx_field_entry_t * entry_info_p);

/**
* \brief
*  Get info from cache for specific TCAM Field group by entry id
* \param [in] unit           - Device ID
* \param [in] fg_id          - Entry's FG ID
* \param [in] entry_handle   - unique entry handle
* \param [out] entry_found_p - Pointer that specifies whether the entry was found on cache
* \param [out] entry_info_p  - Pointer to dnx_field_entry_t to get info from TCAM cache
* \return
*   shr_error_e - Error Type
* \remark
*   * None
* \see
*   * None
*/
shr_error_e dnx_field_entry_tcam_cache_get(
    int unit,
    dnx_field_group_t fg_id,
    uint32 entry_handle,
    uint8 *entry_found_p,
    dnx_field_entry_t * entry_info_p);
/**
* \brief
*  Get info for specific KBP Field group by entry id
* \param [in] unit                  - Device ID
* \param [in] fg_id                 - Entry's FG ID
* \param [in] entry_handle          - unique entry handle
* \param [in,out] cached_indication - Indication whether the entry is cached in SW or installed to HW.
*                                     When NULL is provided, the entry will be retrieved only if installed to HW.
*                                     Otherwise, the parameter will be set to TRUE if the entry is cached or FALSE if
*                                     the entry is installed. All relevant entry information will be retrieved if the
*                                     entry is found (only relevant for diagnostics).
* \param [out] entry_info_p         - Pointer to dnx_field_entry_t to get info from KBP HW
* \return
*   shr_error_e - Error Type
* \remark
*   * None
* \see
*   * None
*/
shr_error_e dnx_field_entry_kbp_get(
    int unit,
    bcm_field_group_t fg_id,
    uint32 entry_handle,
    uint32 *cached_indication,
    dnx_field_entry_t * entry_info_p);

/**
* \brief
*  Get info for specific EXEM Field group by entry id
* \param [in] unit             - Device ID
* \param [in] fg_id            - Field group ID.
* \param [in] entry_key_p      - The key of the entry we want to read.
* \param [out] entry_info_p    - Pointer to dnx_field_entry_t to get info from EXEM HW
* \return
*   shr_error_e - Error Type
* \remark
*   * None
* \see
*   * None
*/
shr_error_e dnx_field_entry_exem_get(
    int unit,
    dnx_field_group_t fg_id,
    dnx_field_entry_key_t * entry_key_p,
    dnx_field_entry_t * entry_info_p);

/**
* \brief
*  Get info for specific MDB DT Field group by entry id
* \param [in] unit             - Device ID
* \param [in] fg_id            - Field group ID.
* \param [in] entry_key_p      - The key of the entry we want to read.
* \param [out] entry_info_p    - Pointer to dnx_field_entry_t to get info from MDB DT HW
* \return
*   shr_error_e - Error Type
* \remark
*   * None
* \see
*   * None
*/
shr_error_e dnx_field_entry_mdb_dt_get(
    int unit,
    dnx_field_group_t fg_id,
    dnx_field_entry_key_t * entry_key_p,
    dnx_field_entry_t * entry_info_p);

/**
* \brief
*  Get info for specific ACE format by key.
* \param [in] unit             - Device ID
* \param [in] ace_entry_key    - The key of the entry we want to read.
* \param [out] payload_info_p  - The action values in the entry. If NULL not filled.
* \param [out] ace_id_p        - Pointer to the ACE format ID associated with the entry. If NULL not filled.
* \return
*   shr_error_e - Error Type
* \remark
*   * None
* \see
*   * None
*/
shr_error_e dnx_field_ace_entry_get(
    int unit,
    dnx_field_ace_key_t ace_entry_key,
    dnx_field_entry_payload_t * payload_info_p,
    dnx_field_ace_id_t * ace_id_p);

/**
* \brief
*  Get hit indication info for a specific TCAM entry per core.
* \param [in] unit           - Device ID
* \param [in] flags          - Flags for future use.
* \param [in] entry_handle   - Unique entry handle
* \param [out] entry_hit_core_bmp_p  - 2 bits bitmap, where every
*  bit indicates on which core the entry was hit:
*       - bit 0 = core 0
*       - bit 1 = core 1
*  There is possibility that entry was hit on both cores, then
*  both bits will be set.
* \return
*   shr_error_e - Error Type
* \remark
*   * None
* \see
*   * None
*/
shr_error_e dnx_field_entry_tcam_hit_get(
    int unit,
    uint32 flags,
    uint32 entry_handle,
    uint8 *entry_hit_core_bmp_p);

/**
* \brief
*  This function flushes hit information of an entry or all entries,
*  from hardware.
* \param [in] unit           - Device ID
* \param [in] core_id        - Entry core ID
* \param [in] flush_all      - Indicates if FLUSH_ALL flag was used. TRUE or FALSE.
* \param [in] entry_abs_location  - Absolute location of an unique
*  entry handle.
* \return
*   shr_error_e - Error Type
* \remark
*   * None
* \see
*   * None
*/
shr_error_e dnx_field_entry_tcam_hit_per_location_flush(
    int unit,
    dnx_field_tcam_core_e core_id,
    uint8 flush_all,
    uint32 entry_abs_location);

/**
* \brief
*  This function flushes hit information of an entry or all entries,
*      from hardware.
* \param [in] unit           - Device ID
* \param [in] flags - Flags to indicate what action should be performed
*  by the API, currently in use is:
*      - BCM_FIELD_ENTRY_HIT_FLUSH_ALL - used for flushing hit
*        indication info for all entries. No need to specify an entry ID.
*      - If no flag is specified, the API will clear information only for
*        the given entry_handle.
* \param [in] entry_handle   - Unique entry handle for which the hit
*  information will be flushed.
* \return
*   shr_error_e - Error Type
* \remark
*   * None
* \see
*   * None
*/
shr_error_e dnx_field_entry_tcam_hit_flush(
    int unit,
    uint32 flags,
    uint32 entry_handle);

#endif
