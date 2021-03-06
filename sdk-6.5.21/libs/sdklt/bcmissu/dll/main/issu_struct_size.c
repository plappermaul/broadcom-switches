/*! \file issu_struct_size.c
 *
 * ISSU Structure size upgrade (for HA memory).
 */
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

#include <sal/sal_types.h>
#include <sal/sal_libc.h>
#include <sal/sal_alloc.h>
#include <sal/sal_assert.h>
#include <bsl/bsl.h>
#include <shr/shr_error.h>
#include <shr/shr_ha.h>
#include <bcmha/bcmha_mem.h>
#include <bcmissu/issu_types.h>
#include <bcmissu/issu_internal.h>

#define BSL_LOG_MODULE BSL_LS_BCMISSU_CTRL

static size_t bcmissu_retrieve_struct_size(uint32_t ver_idx,
                                           uint32_t start_idx,
                                           uint8_t *ha_blk,
                                           const issu_struct_db_t *from,
                                           const issu_struct_t *s,
                                           bcmissu_comp_s_info_t *s_info);

/*******************************************************************************
 * Private functions
 */
/*!
 * \brief Retrieve field value from HA memory block.
 *
 * This function obtains the value of a field from an HA memory block by using
 * the structure and field offsets within the block as well as the field width.
 *
 * \param [in] blk The HA memory block.
 * \param [in] fld_offset The field offset within the structure.
 * \param [in] width The field width.
 * \param [in] s_info The structure control structure indicating the structure
 * location within the HA memory block.
 *
 * \retval The value of the desired field.
 */
static uint64_t retrieve_ha_field_val(uint8_t *blk,
                                      uint32_t fld_offset,
                                      uint32_t width,
                                      bcmissu_comp_s_info_t *s_info)
{
    blk += s_info->offset + fld_offset;
    return bcmissu_obtain_fld_val(blk, width);
}

/*!
 * \brief Calculate the size change for a sub-structure.
 *
 * This function calculates the change to the sub-structure size. A sub
 * structure is a structure that is a field within a larger structure. This
 * function also returns the original sub-structure size.
 *
 * \param [in] ha_blk The pointer to the structure within the HA memory. This is
 * a real pointer.
 * \param [in] ver_idx The base s/w version index where the super structure is
 * currently being evaluated.
 * \param [in] id The sub-structure ID.
 * \param [in] s_info The super structure control info. Note that sub-structures
 * do not have control structure since they are part of their parent control
 * structure.
 * \param [out] orig_size the original structure size prior to the change.
 *
 * \retval True if the output parameter \c calc_size has a new value. False
 * otherwise.
 */
static bool sub_struct_size_change(uint8_t *ha_blk,
                                   uint32_t ver_idx,
                                   uint32_t start_idx,
                                   bcmissu_struct_id_t id,
                                   bcmissu_comp_s_info_t *s_info,
                                   size_t *calc_size)
{
    const issu_struct_db_t *ver_db = NULL;
    const issu_struct_db_t *base_ver;
    bcmissu_comp_s_info_t tmp_s_info;
    uint32_t j;
    int rv = SHR_E_INTERNAL;

    /*
     * Find the version of this data structure. Don't care about versions
     * prior to this version. So start with the current version (given by
     * ver_idx) and continue up.
     */
    for (j = start_idx; j < ver_idx; j++) {
        rv = bcmissu_struct_def_find(id, j, &ver_db);
        if (SHR_SUCCESS(rv)) {
            break;
        }
    }
    if (SHR_FAILURE(rv)) {
        return false;
    }
    /* If we are here we found that the structure had changed */
    base_ver = ver_db;

    /* Now find the current structure definition */
    rv = bcmissu_struct_def_find(id, ver_idx, &ver_db);
    if (SHR_FAILURE(rv)) {
        return false;
    }

    /*
     * Since this data structure is a sub structure it doesn't have a component
     * structure info. So we need to create one for this occasion.
     */
    tmp_s_info.offset = 0; /* The data starts at the beginning of the memory */
    tmp_s_info.struct_id = s_info->struct_id;
    tmp_s_info.data_size = base_ver->struct_info->generic_size;

    *calc_size = bcmissu_retrieve_struct_size(ver_idx, j, ha_blk, base_ver,
                                              ver_db->struct_info, &tmp_s_info);
    return true;
}

/*!
 * \brief Determine the size of a structure for a given version.
 *
 * This function determine the size of a structure for a given version and
 * storage of that structure.
 * The basic size of the structure is provided as part of the structure DB.
 * However, if the structure has variable size array or if it contain a field
 * that it is a structure itself, then it is possible for the basic size to
 * grow.
 *
 * \param [in] ver_idx The index of the structure version in the DB.
 * \param [in] ha_blk The HA memory block containing this instance of the
 * structure. For variable size structure the actual location may indicate
 * different size.
 * \param [in] base_db The structure base DB. This DB correspond with the actual
 * values that can be found in \c ha_blk.
 * \param [in] s The current structure DB version.
 * \param [in] s_info Is the structure information as reported by the component.
 *
 * \retval The calculated size of the structure.
 */
static size_t bcmissu_retrieve_struct_size(uint32_t ver_idx,
                                           uint32_t start_idx,
                                           uint8_t *ha_blk,
                                           const issu_struct_db_t *from,
                                           const issu_struct_t *s,
                                           bcmissu_comp_s_info_t *s_info)
{
    const issu_field_t *fld;
    uint32_t j;
    size_t rv = s->generic_size;
    size_t fld_size = 0;
    uint32_t offset;
    uint32_t width;
    uint64_t val;
    size_t struct_delta;

    /*
     * The only field that can impact the generic size is an unbounded array
     * field. However, we are also looking into structure fields since they
     * might contain unbounded array.
     */
    for (j = 0, fld = s->fields; j < s->field_count; j++, fld++) {
        struct_delta = 0;
        /* If the field is structure itself find if it changed */
        if ((fld->struct_id != ISSU_INVALID_STRUCT_ID) &&
            (!fld->is_local_enum)) {
            offset = from->struct_info->offset_get_func(fld->fid);
            /* New field has no offset in old structure */
            if (offset != ISSU_INVALID_FLD_OFFSET) {
                /*
                 * For structure size we need to know their new size and if it
                 * is different from the current field size.
                 */
                if (sub_struct_size_change(ha_blk + offset + s_info->offset,
                                           ver_idx,
                                           start_idx,
                                           fld->struct_id,
                                           s_info,
                                           &fld_size)) {
                    if (fld_size > fld->width) {
                        struct_delta = fld_size - fld->width;
                    }
                }
            } else {
                fld_size = fld->width;
            }
        } else {
            fld_size = fld->width;
        }
        if (fld->is_array) {
            if (fld->var_size_id != ISSU_INVALID_FIELD_ID) {
                /*
                 * Find the field offset in the original version since the value
                 * will be retrieved from the HA block (which contains the
                 * original data structure).
                 */
                offset = bcmissu_fld_offset(from, fld->var_size_id, &width);
                val = retrieve_ha_field_val(ha_blk, offset, width, s_info);
                rv += val * fld_size;
            } else {
                /*
                 * Fixed array, incure the size increment if type is struct
                 * and the struct size had changed.
                 */
                rv += fld->size * struct_delta;
            }
        } else {  /* If the type was struct and it changed */
            rv += struct_delta;
        }
    }

    return rv;
}

/*!
 * \brief Validate the structure size report vs. actual HA block size.
 *
 * This function verifies that the old structure size can fit instances times
 * into the HA block. The purpose of this function is to identify faulty
 * reports of data size that was larger than it should. It also prevents from
 * the ISSU to attempt to upgrade structures that are outside the boundary of
 * the HA block.
 * If the function find discrepancy it will try to fix the reported structure
 * size to the generic size. This can happen only if there is no unbounded
 * array in the structure.
 *
 * \param [in] len The HA block length.
 * \param [in] from The structure DB description before the change (i.e. the
 * structure that is currently written into the HA block).
 * \param [in,out] s_info The report to validate.
 *
 * \return SHR_E_NONE Report is validated.
 * \return SHR_E_FAIL Report is not validated and can't be fixed.
 */
static int validate_report(uint32_t len,
                           const issu_struct_db_t *from,
                           bcmissu_comp_s_info_t *s_info)
{
    const issu_struct_t *s_def;
    const issu_field_t *fld;
    uint32_t j;

    if (s_info->data_size * s_info->instances <= len) {
        return SHR_E_NONE;
    }

    /*
     * Possible the data size is not properly set so 'fix' it. This can
     * be done only if there is no structure nesting or no unbounded array
     * field within the structure.
     */
    s_def = from->struct_info;
    for (j = 0, fld = s_def->fields; j < s_def->field_count; j++, fld++) {
        if (fld->struct_id != ISSU_INVALID_STRUCT_ID ||
            (fld->is_array && fld->var_size_id != ISSU_INVALID_FIELD_ID)) {
            return SHR_E_FAIL;
        }
    }
    /* Adjust the structure size if lead to success */
    if (s_def->generic_size * s_info->instances <= len) {
        s_info->data_size = s_def->generic_size;
        s_info->max_data_size = s_info->last_ver_data_size = s_def->generic_size;
        LOG_INFO(BSL_LOG_MODULE,
                 (BSL_META("Repaired invalid reported structure "
                         "comp=%d sub=%d struct ID=0x%" PRIx64"\n"),
                         s_info->comp_id, s_info->sub_id, s_info->struct_id));
        return SHR_E_NONE;
    } else {
        return SHR_E_FAIL;
    }
}

/*******************************************************************************
 * Public functions local to ISSU
 */
uint32_t bcmissu_fld_offset(const issu_struct_db_t *s,
                            bcmissu_field_id_t fid,
                            uint32_t *width)
{
    uint32_t j;

    *width = 0;
    for (j = 0; j < s->struct_info->field_count; j++) {
        if (s->struct_info->fields[j].fid == fid) {
            *width = s->struct_info->fields[j].width;
            break;
        }
    }
    assert(*width > 0);
    return s->struct_info->offset_get_func(fid);
}

uint64_t bcmissu_obtain_fld_val(uint8_t *src, uint32_t fld_len)
{
    uint64_t val = 0;

    switch (fld_len) {
    case 1:
        val = (uint64_t)*src;
        break;
    case 2:
        val = (uint64_t)(*(uint16_t *)src);
        break;
    case 4:
        val = (uint64_t)(*(uint32_t *)src);
        break;
    case 8:
        val = *(uint64_t *)src;
        break;
    default:
        assert(0);
    }
    return val;
}


int bcmissu_size_update(int unit,
                        const issu_struct_db_t *from,
                        uint32_t ver_idx,
                        uint32_t last_idx,
                        bcmissu_ha_blk_inc_t **blk_list)
{
    size_t origin_s_size;
    size_t s_size;
    size_t new_s_size;
    const issu_struct_db_t *ver_db;
    bcmissu_comp_s_info_t *s_info;
    void *blk;
    uint8_t *ha_blk;
    uint32_t len;
    bcmissu_ha_blk_inc_t *tmp_ha_blk;
    int rv;

    s_info = bcmissu_find_first_s_info(unit, from->id,  &blk);
    /* Go through all instances of this structures in all HA blocks */
    while (s_info) {
        len = 4;    /* Minimal block length */
        if (unit == BCMISSU_NO_UNIT) {
            ha_blk = bcmha_mem_alloc(BCMHA_GEN_UNIT,
                                     s_info->comp_id, s_info->sub_id,
                                     NULL, &len);
        } else {
            ha_blk = shr_ha_mem_alloc(unit,
                                      s_info->comp_id, s_info->sub_id,
                                      NULL, &len);
        }
        if (!ha_blk) {
            return SHR_E_MEMORY;
        } else if (len < s_info->data_size) {
            bcmissu_comp_s_info_t *tmp = s_info;
            void *tmp_blk = blk;
            LOG_WARN(BSL_LOG_MODULE,
                     (BSL_META_U(unit,
                             "Stale reported structure comp=%d sub=%d struct ID=0x%"
                             PRIx64"\n"),
                             tmp->comp_id, tmp->sub_id, tmp->struct_id));
            /* Stale report that should be deleted */
            tmp = bcmissu_find_next_s_info(from->id, s_info, &blk);
            bcmissu_struct_info_clear(unit, s_info->struct_id,
                                      s_info->comp_id, s_info->sub_id);
            if (s_info->struct_id != from->id) {
                s_info = tmp;
                blk = tmp_blk;
            }
            /* Free the HA block as it was actually unused */
            if (unit == BCMISSU_NO_UNIT) {
                bcmha_mem_free(BCMHA_GEN_UNIT, ha_blk);
            } else {
                shr_ha_mem_free(unit, ha_blk);
            }
            continue;
        }
        /* Validate report with respect to HA block */
        if (SHR_FAILURE(validate_report(len, from, s_info))) {
            LOG_WARN(BSL_LOG_MODULE,
                     (BSL_META_U(unit,
                             "report validation failed comp=%d sub=%d struct ID=0x%"
                             PRIx64". Skipping\n"),
                             s_info->comp_id, s_info->sub_id, s_info->struct_id));
            s_info = bcmissu_find_next_s_info(from->id, s_info, &blk);
            continue;
        }

        origin_s_size = s_info->max_data_size;

        s_size = origin_s_size;
        /*
         * Check the new size in target version since we need to find the
         * size. Find the structure first in the current version DB
         */
        rv = bcmissu_struct_def_find(from->id, last_idx, &ver_db);
        if (SHR_FAILURE(rv)) {
            /* Structure had been deleted. Need to remove all the structure info */
            break;
        }
        new_s_size =
            bcmissu_retrieve_struct_size(last_idx,
                                         ver_idx,
                                         ha_blk,
                                         from,
                                         ver_db->struct_info,
                                         s_info);
        if (new_s_size > s_size) {
            s_size = new_s_size;
        }
        s_info->last_ver_data_size = new_s_size;

        if (s_size > origin_s_size) {
            /* Search for the block in the list, If not found allocate one */
            s_info->max_data_size = s_size;
            for (tmp_ha_blk = *blk_list; tmp_ha_blk;
                 tmp_ha_blk = tmp_ha_blk->next) {
                if (tmp_ha_blk->comp_id == s_info->comp_id &&
                    tmp_ha_blk->sub_id == s_info->sub_id) {
                    break;
                }
            }
            if (!tmp_ha_blk) {
                tmp_ha_blk = sal_alloc(sizeof(*tmp_ha_blk),  "bcmissuHaBlk");
                if (!tmp_ha_blk) {
                    return SHR_E_MEMORY;
                }
                tmp_ha_blk->comp_id = s_info->comp_id;
                tmp_ha_blk->sub_id = s_info->sub_id;
                tmp_ha_blk->orig_size = len;
                tmp_ha_blk->inc = 0;
                tmp_ha_blk->blk = ha_blk;
                tmp_ha_blk->next = (*blk_list);
                *blk_list = tmp_ha_blk;
            }
            tmp_ha_blk->inc += (s_size - origin_s_size) * s_info->instances;
        }

        s_info = bcmissu_find_next_s_info(from->id, s_info, &blk);
    }
    return SHR_E_NONE;
}

