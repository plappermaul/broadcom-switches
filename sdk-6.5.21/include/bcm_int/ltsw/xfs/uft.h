/*
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated.
 * Edits to this file will be lost when it is regenerated.
 *
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

#ifndef BCMINT_LTSW_XFS_UFT_H
#define BCMINT_LTSW_XFS_UFT_H

#include <bcm/switch.h>

#include <bcm_int/ltsw/uft.h>

/*!
 * \brief Initializes the UFT module.
 *
 * \param [in] unit Unit Number.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
xfs_ltsw_uft_init(int unit);

/*!
 * \brief De-initializes the UFT module.
 *
 * \param [in] unit Unit Number.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
xfs_ltsw_uft_detach(int unit);

/*!
 * \brief Get the current UFT mode.
 *
 * \param [in] unit Unit Number.
 * \param [out] uft_mode UFT mode.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
xfs_ltsw_uft_mode_get(
    int unit,
    bcmi_ltsw_uft_mode_t *uft_mode);

/*!
 * \brief Set the maximum number of entry moves for hash reordering.
 *
 * \param [in] unit Unit Number.
 * \param [in] type Move depth switch control type.
 * \param [in] arg Move depth value.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
xfs_ltsw_uft_multi_move_depth_set(
    int unit,
    bcm_switch_control_t type,
    int arg);

/*!
 * \brief Get the maximum number of entry moves for hash reordering.
 *
 * \param [in] unit Unit Number.
 * \param [in] type Move depth switch control type.
 * \param [out] arg Move depth value.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
xfs_ltsw_uft_multi_move_depth_get(
    int unit,
    bcm_switch_control_t type,
    int *arg);

/*!
 * \brief Get information about a hash bank.
 *
 * \param [in] unit Unit Number.
 * \param [in/out] bank_info Hash bank information structure.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
xfs_ltsw_uft_bank_info_get(
    int unit,
    bcm_switch_hash_bank_info_t *bank_info);

/*!
 * \brief Get the UFT banks assigned to a hash table.
 *
 * \param [in] unit Unit Number.
 * \param [in] hash_table Hash table.
 * \param [in] array_size Array size of bank_array.
 * \param [out] bank_array Hash bank array.
 * \param [out] num_banks Actual number of UFT banks returned in bank_array.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
xfs_ltsw_uft_bank_multi_get(
    int unit,
    bcm_switch_hash_table_t hash_table,
    int array_size,
    bcm_switch_hash_bank_t *bank_array,
    int *num_banks);

/*!
 * \brief Get the hash tables to which a given UFT bank is assigned.
 *
 * \param [in] unit Unit Number.
 * \param [in] bank_num UFT bank number.
 * \param [in] array_size Array size of table_array.
 * \param [out] table_array Hash table array.
 * \param [out] num_tables Actual number of hash tables returned in table_array.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
xfs_ltsw_uft_table_multi_get(
    int unit,
    bcm_switch_hash_bank_t bank_num,
    int array_size,
    bcm_switch_hash_table_t *table_array,
    int *num_tables);

/*!
 * \brief UFT bank control set function.
 *
 * \param [in] unit Unit Number.
 * \param [in] bank_control Hash bank control structure.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
xfs_ltsw_uft_bank_control_set(
    int unit,
    bcm_switch_hash_bank_control_t *bank_control);

/*!
 * \brief Hash bank control get function.
 *
 * \param [in] unit Unit Number.
 * \param [in/out] bank_control Hash bank control structure.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
xfs_ltsw_uft_bank_control_get(
    int unit,
    bcm_switch_hash_bank_control_t *bank_control);

#endif /* BCMINT_LTSW_XFS_UFT_H */
