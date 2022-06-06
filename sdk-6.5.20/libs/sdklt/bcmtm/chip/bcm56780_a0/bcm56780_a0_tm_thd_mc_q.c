/*! \file bcm56780_a0_tm_thd_mc_q.c
 *
 * IMM handlers for threshold LTs.
 *
 * This file contains callbcak handlers for LT TM_THD_MC_Q.
 */
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

#include <shr/shr_error.h>
#include <shr/shr_debug.h>
#include <bcmdrd/chip/bcm56780_a0_enum.h>
#include <bcmltd/chip/bcmltd_id.h>
#include <bcmlrd/bcmlrd_map.h>
#include <bcmimm/bcmimm_int_comp.h>
#include <bcmtm/bcmtm_utils_internal.h>
#include <bcmtm/bcmtm_utils.h>
#include <bcmtm/bcmtm_drv.h>
#include <bcmtm/bcmtm_pt_internal.h>
#include <bcmtm/chip/bcm56780_a0_tm_thd_internal.h>
#include <bcmtm/chip/bcm56780_a0_tm.h>

#include "bcm56780_a0_tm_imm_internal.h"

#define TD4_X9_MAX_NUM_MC_Q 6

/* TM_THD_MC_Q_ENTRY_STATE_T */
#define VALID                   0
#define PORT_INFO_UNAVAILABLE   1
#define MC_Q_INVALID            2
#define INCORRECT_MIN_GUARANTEE 3

#define NUM_KEY_FIELDS          2
#define NUM_OPER_FIELDS         5
#define BSL_LOG_MODULE  BSL_LS_BCMTM_IMM_CHIP
static int
tm_thd_mc_q_read_from_lt(int unit, int lport, int queue, bcmlrd_fid_t fid, uint32_t *fval)
{
    bcmltd_sid_t sid = TM_THD_MC_Qt;
    bcmltd_fields_t entry;
    bcmltd_fields_t keys;
    size_t num_fid;
    uint64_t field_data;

    SHR_FUNC_ENTER(unit);

    sal_memset(&entry, 0, sizeof(bcmltd_fields_t));
    sal_memset(&keys, 0, sizeof(bcmltd_fields_t));

    SHR_IF_ERR_EXIT
        (bcmtm_field_list_alloc(unit, NUM_KEY_FIELDS, &keys));
    keys.field[0]->id   = TM_THD_MC_Qt_PORT_IDf;
    keys.field[0]->data = lport;
    keys.field[1]->id   = TM_THD_MC_Qt_TM_MC_Q_IDf;
    keys.field[1]->data = queue;
    SHR_IF_ERR_EXIT
        (bcmlrd_field_count_get(unit, sid, &num_fid));
    SHR_IF_ERR_EXIT
        (bcmtm_field_list_alloc(unit, num_fid, &entry));

    SHR_IF_ERR_EXIT
        (bcmimm_entry_lookup(unit, sid, &keys, &entry));

    SHR_IF_ERR_EXIT
        (bcmtm_field_list_get(unit, &entry, fid, 0, &field_data));
    *fval = (uint32_t)field_data;

exit:
    bcmtm_field_list_free(&keys);
    bcmtm_field_list_free(&entry);
    SHR_FUNC_EXIT();
}

static int
tm_thd_mc_q_populate_entry(int unit, int lport, int queue, int up,
        soc_mmu_cfg_buf_t *buf)
{
    bcmltd_fields_t entry;
    bcmltd_sid_t sid = TM_THD_MC_Qt;
    size_t num_fid;
    int num_q;
    soc_mmu_cfg_buf_queue_t *buf_queue;
    int oper_state;

    SHR_FUNC_ENTER(unit);

    sal_memset(&entry, 0, sizeof(bcmltd_fields_t));
    buf_queue = &buf->ports[lport].queues[queue];

    /*
     * Populate default entry (value fields) with HW default values.
     * Key fields must be set.
     */
    SHR_IF_ERR_EXIT
        (bcmlrd_field_count_get(unit, sid, &num_fid));
    SHR_IF_ERR_EXIT
        (bcmtm_field_list_alloc(unit, num_fid, &entry));
    SHR_IF_ERR_EXIT
        (bcmtm_default_entry_array_construct(unit, sid, &entry));
    SHR_IF_ERR_EXIT
        (bcmtm_field_list_set(unit, &entry, TM_THD_MC_Qt_PORT_IDf, 0, lport));
    SHR_IF_ERR_EXIT
        (bcmtm_field_list_set(unit, &entry, TM_THD_MC_Qt_TM_MC_Q_IDf, 0, queue));

        oper_state = PORT_INFO_UNAVAILABLE;
        /* Check for invalid queue (number of MC queue for the port). */
        if (!bcmtm_lport_is_cpu(unit, lport)) {
            num_q = bcm56780_a0_tm_num_mc_q_non_cpu_port_get(unit);
        } else {
            num_q = SOC_TD4_X9_NUM_CPU_QUEUES;
        }
        if (queue >= num_q) {
            oper_state = MC_Q_INVALID;
        } else {
            /* Write to field list. */
            /* Populate user-configured fields during init. */
            SHR_IF_ERR_EXIT
                (bcmtm_field_list_set(unit, &entry,
                    TM_THD_MC_Qt_MIN_GUARANTEE_CELLSf, 0, buf_queue->guarantee));
            SHR_IF_ERR_EXIT
                (bcmtm_field_list_set(unit, &entry,
                    TM_THD_MC_Qt_SHARED_LIMITSf, 0, buf_queue->discard_enable));
            SHR_IF_ERR_EXIT
                (bcmtm_field_list_set(unit, &entry,
                                 TM_THD_MC_Qt_DYNAMIC_SHARED_LIMITSf, 0, 1));
            SHR_IF_ERR_EXIT
                (bcmtm_field_list_set(unit, &entry,
                    TM_THD_MC_Qt_RESUME_OFFSET_CELLSf, 0, (buf_queue->pool_resume
                    * TD4_X9_GRANULARITY)));
            SHR_IF_ERR_EXIT
                (bcmtm_field_list_set(unit, &entry,
                    TM_THD_MC_Qt_COLOR_SPECIFIC_LIMITSf, 0, buf_queue->color_discard_enable));
        }
    SHR_IF_ERR_EXIT
        (bcmtm_field_list_set(unit, &entry,
            TM_THD_MC_Qt_OPERATIONAL_STATEf, 0, oper_state));

    SHR_IF_ERR_EXIT
        (bcmimm_entry_insert(unit, sid, &entry));

exit:
    bcmtm_field_list_free(&entry);
    SHR_FUNC_EXIT();
}

static int
tm_thd_mc_q_update(int unit,
                   bcmltd_sid_t sid,
                   int lport,
                   int queue,
                   const bcmltd_field_t *data_fields,
                   bcmltd_fields_t *output_fields,
                   uint8_t action)
{
    const bcmltd_field_t *data_ptr;
    int pport, num_q, base;
    long int delta;
    bcmtm_pt_info_t pt_info = {0};
    bcmdrd_sid_t ptid;
    bcmdrd_fid_t fid;
    uint32_t q_min, new_q_min, use_dynamic_shared, use_dynamic_color, fval, pool;
    uint32_t ltmbuf[BCMTM_MAX_THD_ENTRY_WSIZE] = {0};
    uint32_t q_config_buf[BCMTM_MAX_THD_ENTRY_WSIZE] = {0};
    bool write_flag;
    bcmtm_drv_info_t *drv_info;
    soc_mmu_cfg_buf_t *buf;
    soc_mmu_device_info_t devcfg;
    soc_mmu_cfg_buf_queue_t *buf_queue;
    uint32_t limit;

    SHR_FUNC_ENTER(unit);
    buf = bcm56780_a0_mmu_buf_cfg_alloc(unit);
    if (!buf) {
        return SHR_E_MEMORY;
    }
    bcmtm_drv_info_get(unit, &drv_info);
    td4_x9_mmu_init_dev_config(unit, &devcfg, drv_info->lossless);
    td4_x9_mmu_config_thdo_set(unit, lport, buf, &devcfg, drv_info->lossless);
    buf_queue = &buf->ports[lport].queues[queue];

    SHR_IF_ERR_EXIT
        (bcmtm_field_list_size_check(unit, output_fields, NUM_OPER_FIELDS));
    output_fields->count = 0;

    /* Check for invalid port (port is not added). */
    if (SHR_FAILURE(bcmtm_lport_pport_get(unit, lport, &pport))) {
        SHR_IF_ERR_EXIT
            (bcmtm_field_list_set(unit, output_fields, TM_THD_MC_Qt_OPERATIONAL_STATEf,
                             0, PORT_INFO_UNAVAILABLE));
        SHR_EXIT();
    }
    /* Check for invalid queue (number of MC queue for the port). */
    if (!bcmtm_lport_is_cpu(unit, lport)) {
        num_q = bcm56780_a0_tm_num_mc_q_non_cpu_port_get(unit);
    } else {
        num_q = SOC_TD4_X9_NUM_CPU_QUEUES;
    }
    if (queue >= num_q) {
        SHR_IF_ERR_EXIT
            (bcmtm_field_list_set(unit, output_fields, TM_THD_MC_Qt_OPERATIONAL_STATEf,
                             0, MC_Q_INVALID));
        SHR_EXIT();
    }

    /* Read MMU_THDO_QUEUE_CONFIGm and store the result. */
    ptid = MMU_THDO_QUEUE_CONFIGm;
    SHR_IF_ERR_EXIT
        (bcmtm_lport_mcq_base_get(unit, lport, &base));
    BCMTM_PT_DYN_INFO(pt_info, base + queue, 0);
    sal_memset(q_config_buf, 0, sizeof(q_config_buf));
    SHR_IF_ERR_EXIT
        (bcmtm_pt_indexed_read(unit, ptid, sid, &pt_info, q_config_buf));
    SHR_IF_ERR_EXIT
        (bcmtm_field_get(unit, ptid, MIN_LIMITf, q_config_buf, &q_min));
    SHR_IF_ERR_EXIT
        (bcmtm_field_get(unit, ptid, LIMIT_DYNAMICf, q_config_buf, &use_dynamic_shared));
    SHR_IF_ERR_EXIT
        (bcmtm_field_get(unit, ptid, COLOR_LIMIT_MODEf, q_config_buf, &use_dynamic_color));
    SHR_IF_ERR_EXIT
        (bcmtm_field_get(unit, ptid, SPIDf, q_config_buf, &pool));

    if (action == PORT_ADD) {
        q_min = buf_queue->guarantee;
    }

    /* Min guarantee and control bool fields. */
    data_ptr = data_fields;
    while (data_ptr) {
        /* Default behavior for each loop is to write to q_config_buf. */
        write_flag = TRUE;
        ptid = MMU_THDO_QUEUE_CONFIGm;
        fval = data_ptr->data;

        switch (data_ptr->id) {
            case TM_THD_MC_Qt_MIN_GUARANTEE_CELLSf:
                fid = MIN_LIMITf;
                new_q_min = fval;
                if (action == PORT_DELETE) {
                    fval = new_q_min = buf_queue->guarantee;
                }
                /*
                 * Calculate delta of available shared size.
                 * delta of shared_size always equals to -(delta of reserved_size).
                 */
                delta = -((long int) new_q_min - q_min);
                /* Check if both ITM have enough buffer size to allocate the delta. */
                if (SHR_SUCCESS
                        (bcm56780_a0_tm_shared_buffer_update(unit, 0, pool, delta,
                                                             RESERVED_BUFFER_UPDATE,
                                                             TRUE))) {
                    SHR_IF_ERR_EXIT
                        (bcm56780_a0_tm_shared_buffer_update(unit, 0, pool, delta,
                                                             RESERVED_BUFFER_UPDATE,
                                                             FALSE));
                    /* Update operational fields to the new limit. */
                    SHR_IF_ERR_EXIT
                        (bcmtm_field_list_set(unit, output_fields,
                                         TM_THD_MC_Qt_MIN_GUARANTEE_CELLS_OPERf,
                                         0, fval));
                    SHR_IF_ERR_EXIT
                        (bcmtm_field_list_set(unit, output_fields,
                                         TM_THD_MC_Qt_OPERATIONAL_STATEf,
                                         0, VALID));
                } else {
                    write_flag = FALSE;
                    /* Update operational fields to reflect the error. */
                    SHR_IF_ERR_EXIT
                        (bcmtm_field_list_set(unit, output_fields,
                                         TM_THD_MC_Qt_OPERATIONAL_STATEf,
                                         0, INCORRECT_MIN_GUARANTEE));
                }
                break;
            case TM_THD_MC_Qt_SHARED_LIMITSf:
                fid = LIMIT_ENABLEf;
                break;
            case TM_THD_MC_Qt_DYNAMIC_SHARED_LIMITSf:
                fid = LIMIT_DYNAMICf;
                use_dynamic_shared = fval;
                if (use_dynamic_shared) {
                    /* New mode is dynamic. */
                    if (SHR_SUCCESS
                            (tm_thd_mc_q_read_from_lt(unit, lport, queue,
                                TM_THD_MC_Qt_SHARED_LIMIT_DYNAMICf, &limit))) {
                        SHR_IF_ERR_EXIT
                            (bcmtm_field_set(unit, ptid, SHARED_LIMITf,
                                             q_config_buf, &limit));
                    }
                } else {
                    /* New mode is static. */
                    if (SHR_SUCCESS
                            (tm_thd_mc_q_read_from_lt(unit, lport, queue,
                                TM_THD_MC_Qt_SHARED_LIMIT_CELLS_STATICf, &limit))) {
                        SHR_IF_ERR_EXIT
                            (bcmtm_field_set(unit, ptid, SHARED_LIMITf,
                                             q_config_buf, &limit));
                    }
                }
                SHR_IF_ERR_EXIT
                    (bcmtm_field_set(unit, ptid, LIMIT_DYNAMICf,
                                     q_config_buf, &use_dynamic_shared));
                break;
            case TM_THD_MC_Qt_COLOR_SPECIFIC_LIMITSf:
                fid = COLOR_ENABLEf;
                break;
            case TM_THD_MC_Qt_COLOR_SPECIFIC_DYNAMIC_LIMITSf:
                fid = COLOR_LIMIT_MODEf;
                use_dynamic_color = fval;
                if (use_dynamic_color) {
                    /* New mode is dynamic. */
                    if (SHR_SUCCESS
                            (tm_thd_mc_q_read_from_lt(unit, lport, queue,
                                TM_THD_MC_Qt_YELLOW_LIMIT_DYNAMICf, &limit))) {
                        SHR_IF_ERR_EXIT
                            (bcmtm_field_set(unit, ptid, LIMIT_YELLOWf,
                                             q_config_buf, &limit));
                    }
                    if (SHR_SUCCESS
                            (tm_thd_mc_q_read_from_lt(unit, lport, queue,
                                TM_THD_MC_Qt_RED_LIMIT_DYNAMICf, &limit))) {
                        SHR_IF_ERR_EXIT
                            (bcmtm_field_set(unit, ptid, LIMIT_REDf,
                                             q_config_buf, &limit));
                    }
                } else {
                    /* New mode is static. */
                    if (SHR_SUCCESS
                            (tm_thd_mc_q_read_from_lt(unit, lport, queue,
                                TM_THD_MC_Qt_YELLOW_LIMIT_CELLS_STATICf, &limit))) {
                        limit = CEILING(limit, TD4_X9_GRANULARITY);
                        SHR_IF_ERR_EXIT
                            (bcmtm_field_set(unit, ptid, LIMIT_YELLOWf,
                                             q_config_buf, &limit));
                    }
                    if (SHR_SUCCESS
                            (tm_thd_mc_q_read_from_lt(unit, lport, queue,
                                TM_THD_MC_Qt_RED_LIMIT_CELLS_STATICf, &limit))) {
                        limit = CEILING(limit, TD4_X9_GRANULARITY);
                        SHR_IF_ERR_EXIT
                            (bcmtm_field_set(unit, ptid, LIMIT_REDf,
                                             q_config_buf, &limit));
                    }
                }
                SHR_IF_ERR_EXIT
                    (bcmtm_field_set(unit, ptid, COLOR_LIMIT_MODEf,
                                     q_config_buf, &use_dynamic_color));
                break;
            default:
                write_flag = FALSE;
                break;
        }
        if (write_flag) {
            /* Update MMU_THDO_QUEUE_CONFIGm buffer. */
            SHR_IF_ERR_EXIT
                (bcmtm_field_set(unit, ptid, fid, q_config_buf, &fval));

        }
        data_ptr = data_ptr->next;
    }


    /* Other fields. */
    data_ptr = data_fields;
    while (data_ptr) {
        /* Default behavior for each loop is to write to q_config_buf. */
        write_flag = TRUE;
        ptid = MMU_THDO_QUEUE_CONFIGm;
        fval = data_ptr->data;

        switch (data_ptr->id) {
            case TM_THD_MC_Qt_SHARED_LIMIT_CELLS_STATICf:
                fid = SHARED_LIMITf;
                if (use_dynamic_shared) {
                    write_flag = FALSE;
                }
                break;
            case TM_THD_MC_Qt_SHARED_LIMIT_DYNAMICf:
                fid = SHARED_LIMITf;
                if (!use_dynamic_shared) {
                    write_flag = FALSE;
                }
                break;
            case TM_THD_MC_Qt_RESUME_OFFSET_CELLSf:
                write_flag = FALSE;
                ptid = MMU_THDO_QUEUE_RESUME_OFFSETm;
                fid = RESUME_OFFSETf;
                sal_memset(ltmbuf, 0, sizeof(ltmbuf));

                /* Apply granularity. */
                fval = CEILING(fval, TD4_X9_GRANULARITY);
                SHR_IF_ERR_EXIT
                    (bcmtm_field_list_set(unit, output_fields,
                                     TM_THD_MC_Qt_RESUME_OFFSET_CELLS_OPERf, 0,
                                     fval * TD4_X9_GRANULARITY));
                /* Read - modify - write. */
                SHR_IF_ERR_EXIT
                    (bcmtm_pt_indexed_read(unit, ptid, sid, &pt_info, ltmbuf));
                SHR_IF_ERR_EXIT
                    (bcmtm_field_set(unit, ptid, fid, ltmbuf, &fval));
                SHR_IF_ERR_EXIT
                    (bcmtm_pt_indexed_write(unit, ptid, sid, &pt_info, ltmbuf));
                break;
            case TM_THD_MC_Qt_YELLOW_LIMIT_CELLS_STATICf:
                fid = LIMIT_YELLOWf;
                if (use_dynamic_color) {
                    write_flag = FALSE;
                }
                /* Apply granularity. */
                fval = CEILING(fval, TD4_X9_GRANULARITY);
                SHR_IF_ERR_EXIT
                    (bcmtm_field_list_set(unit, output_fields,
                                     TM_THD_MC_Qt_YELLOW_LIMIT_CELLS_STATIC_OPERf,
                                     0, fval * TD4_X9_GRANULARITY));
                break;
            case TM_THD_MC_Qt_YELLOW_LIMIT_DYNAMICf:
                fid = LIMIT_YELLOWf;
                if (!use_dynamic_color) {
                    write_flag = FALSE;
                }
                break;
            case TM_THD_MC_Qt_RED_LIMIT_CELLS_STATICf:
                fid = LIMIT_REDf;
                if (use_dynamic_color) {
                    write_flag = FALSE;
                }
                /* Apply granularity. */
                fval = CEILING(fval, TD4_X9_GRANULARITY);
                SHR_IF_ERR_EXIT
                    (bcmtm_field_list_set(unit, output_fields,
                                     TM_THD_MC_Qt_RED_LIMIT_CELLS_STATIC_OPERf,
                                     0, fval * TD4_X9_GRANULARITY));
                break;
            case TM_THD_MC_Qt_RED_LIMIT_DYNAMICf:
                fid = LIMIT_REDf;
                if (!use_dynamic_color) {
                    write_flag = FALSE;
                }
                break;
            case TM_THD_MC_Qt_TM_EGR_BST_THD_Q_PROFILE_IDf:
                fid = BST_Q_WM_TOTAL_THRESHOLD_SELf;
                break;
            case TM_THD_MC_Qt_TM_EBST_PROFILE_IDf:
                fid = BST_Q_EBST_STARTSTOP_TOTAL_THRESHOLD_SELf;
                break;
            default:
                write_flag = FALSE;
                break;
        }
        if (write_flag) {
            /* Update MMU_THDO_QUEUE_CONFIGm buffer. */
            SHR_IF_ERR_EXIT
                (bcmtm_field_set(unit, ptid, fid, q_config_buf, &fval));

        }
        data_ptr = data_ptr->next;
    }

    /* Write to MMU_THDO_QUEUE_CONFIGm. */
    SHR_IF_ERR_EXIT
        (bcmtm_pt_indexed_write(unit, ptid, sid, &pt_info, q_config_buf));


exit:
    sal_free(buf);
    SHR_FUNC_EXIT();
}

static int
tm_thd_mc_q_delete(int unit, bcmltd_sid_t sid, int lport, int queue)
{
    /* Should reset all fields to HW default when delete. */
    bcmltd_field_t *data_fields = NULL;
    bcmltd_fields_t dummy_fields;
    size_t num_fid;

    SHR_FUNC_ENTER(unit);

    sal_memset(&dummy_fields, 0, sizeof(bcmltd_fields_t));

    /* Create data_fields with default field values. */
    SHR_IF_ERR_EXIT
        (bcmtm_default_entry_ll_construct(unit, sid, &data_fields));

    /* Reuse update callback to write HW. */
    SHR_IF_ERR_EXIT
        (bcmlrd_field_count_get(unit, sid, &num_fid));
    SHR_IF_ERR_EXIT
        (bcmtm_field_list_alloc(unit, num_fid, &dummy_fields));

    SHR_IF_ERR_EXIT
        (tm_thd_mc_q_update(unit, sid, lport, queue, data_fields, &dummy_fields,
                            FALSE));

exit:
    SHR_FREE(data_fields);
    bcmtm_field_list_free(&dummy_fields);
    SHR_FUNC_EXIT();
}

static int
tm_thd_mc_q_stage(int unit,
                  bcmltd_sid_t sid,
                  uint32_t trans_id,
                  bcmimm_entry_event_t event_reason,
                  const bcmltd_field_t *key_fields,
                  const bcmltd_field_t *data_fields,
                  void *context,
                  bcmltd_fields_t *output_fields)
{
    const bcmltd_field_t *key_ptr;
    int lport = -1;
    int queue = -1;

    SHR_FUNC_ENTER(unit);

    key_ptr = key_fields;
    while (key_ptr) {
        switch (key_ptr->id) {
            case TM_THD_MC_Qt_PORT_IDf:
                lport = key_ptr->data;
                break;
            case TM_THD_MC_Qt_TM_MC_Q_IDf:
                queue = key_ptr->data;
                break;
            default:
                SHR_ERR_EXIT(SHR_E_PARAM);
        }
        key_ptr = key_ptr->next;
    }

    if (lport < 0 || queue < 0) {
        return SHR_E_PARAM;
    }

    switch (event_reason) {
        case BCMIMM_ENTRY_INSERT:
            /* Insert shares the same callback with update. */
        case BCMIMM_ENTRY_UPDATE:
            SHR_IF_ERR_EXIT
                (tm_thd_mc_q_update(unit, sid, lport, queue, data_fields,
                                    output_fields, UPDATE_ONLY));
            break;
        case BCMIMM_ENTRY_DELETE:
            SHR_IF_ERR_EXIT
                (tm_thd_mc_q_delete(unit, sid, lport, queue));
            break;
        default:
            SHR_ERR_EXIT(SHR_E_PARAM);
    }

exit:
    SHR_FUNC_EXIT();
}

/*******************************************************************************
 * Public Functions
 */
int
bcm56780_a0_tm_thd_mc_q_register(int unit)
{
    bcmimm_lt_cb_t tm_thd_cb_handler;
    const bcmlrd_map_t *map = NULL;
    int rv;

    SHR_FUNC_ENTER(unit);

    /*
     * Assign callback functions to IMM event callback handler.
     * commit and abort functions can be added if necessary.
     */
    sal_memset(&tm_thd_cb_handler, 0, sizeof(tm_thd_cb_handler));
    tm_thd_cb_handler.stage    = tm_thd_mc_q_stage;

    rv = bcmlrd_map_get(unit, TM_THD_MC_Qt, &map);
    SHR_IF_ERR_EXIT_EXCEPT_IF(rv, SHR_E_UNAVAIL);
    if ((rv == SHR_E_UNAVAIL)|| (!map)) {
        SHR_EXIT();
    }
    /* Register callback handler for the LT. */
    SHR_IF_ERR_EXIT
        (bcmimm_lt_event_reg(unit, TM_THD_MC_Qt, &tm_thd_cb_handler, NULL));

exit:
    SHR_FUNC_EXIT();
}

int
bcm56780_a0_tm_thd_mc_q_populate(int unit)
{
    int lport, queue, max_num_mcq, rv;
    const bcmlrd_map_t *map = NULL;
    bcmtm_drv_info_t *drv_info;
    soc_mmu_cfg_buf_t *buf;
    soc_mmu_device_info_t devcfg;

    SHR_FUNC_ENTER(unit);

    buf = bcm56780_a0_mmu_buf_cfg_alloc(unit);
    if (!buf) {
        return SHR_E_MEMORY;
    }
    bcmtm_drv_info_get(unit, &drv_info);
    td4_x9_mmu_init_dev_config(unit, &devcfg, drv_info->lossless);
    rv = bcmlrd_map_get(unit, TM_THD_MC_Qt, &map);
    SHR_IF_ERR_EXIT_EXCEPT_IF(rv, SHR_E_UNAVAIL);
    if ((rv == SHR_E_UNAVAIL)|| (!map)) {
        SHR_EXIT();
    }
    for (lport = 0; lport < SOC_MMU_CFG_PORT_MAX; lport++) {
        td4_x9_mmu_config_thdo_set(unit, lport, buf, &devcfg, drv_info->lossless);
        if (bcmtm_lport_is_cpu(unit, lport)) {
            max_num_mcq = SOC_TD4_X9_NUM_CPU_QUEUES;
        } else {
            max_num_mcq = TD4_X9_MAX_NUM_MC_Q;
        }
        for (queue = 0; queue < max_num_mcq; queue++) {
            SHR_IF_ERR_EXIT
                (tm_thd_mc_q_populate_entry(unit, lport, queue, 1, buf));
        }
    }

exit:
    sal_free(buf);
    SHR_FUNC_EXIT();
}

int
bcm56780_a0_tm_thd_mc_q_update(int unit, int pport, int up)
{
    int lport, queue, max_num_mcq, rv;
    const bcmlrd_map_t *map = NULL;
    bcmltd_fields_t lkup_fields, keys, out_fields;
    size_t num_fid;
    bcmltd_sid_t sid = TM_THD_MC_Qt;
    bcmltd_field_t *data_fields = NULL;
    int j;

    SHR_FUNC_ENTER(unit);
    sal_memset(&lkup_fields, 0, sizeof(bcmltd_fields_t));
    sal_memset(&out_fields, 0, sizeof(bcmltd_fields_t));
    sal_memset(&keys, 0, sizeof(bcmltd_fields_t));

    rv = bcmlrd_map_get(unit, TM_THD_MC_Qt, &map);
    SHR_IF_ERR_EXIT_EXCEPT_IF(rv, SHR_E_UNAVAIL);
    if ((rv == SHR_E_UNAVAIL)|| (!map)) {
        SHR_EXIT();
    }
    SHR_IF_ERR_EXIT
        (bcmtm_field_list_alloc(unit, NUM_KEY_FIELDS, &keys));

    SHR_IF_ERR_EXIT
        (bcmlrd_field_count_get(unit, sid, &num_fid));
    SHR_IF_ERR_EXIT
        (bcmtm_field_list_alloc(unit, num_fid, &lkup_fields));
    /* Create data_fields with default field values. */
    SHR_IF_ERR_EXIT
        (bcmtm_default_entry_ll_construct(unit, sid, &data_fields));

    SHR_IF_ERR_EXIT
        (bcmtm_pport_lport_get(unit, pport, &lport));
    if (bcmtm_lport_is_cpu(unit, lport)) {
        max_num_mcq = SOC_TD4_X9_NUM_CPU_QUEUES;
    } else {
        max_num_mcq = bcm56780_a0_tm_num_mc_q_non_cpu_port_get(unit);
    }
    for (queue = 0; queue < max_num_mcq; queue++) {
        SHR_IF_ERR_EXIT
            (bcmtm_field_list_alloc(unit, num_fid, &out_fields));
        keys.field[0]->id   = TM_THD_MC_Qt_PORT_IDf;
        keys.field[0]->data = lport;
        keys.field[1]->id   = TM_THD_MC_Qt_TM_MC_Q_IDf;
        keys.field[1]->data = queue;
        if (SHR_SUCCESS(bcmimm_entry_lookup(unit, sid, &keys, &lkup_fields))) {
            for (j = 0; j < (int)lkup_fields.count; j++) {
               data_fields[j] = *(lkup_fields).field[j];
            }

            SHR_IF_ERR_EXIT
                (tm_thd_mc_q_update(unit, sid, lport, queue, data_fields,
                                    &out_fields, up));
            if (up == 0) {
                SHR_IF_ERR_EXIT
                    (bcmtm_field_list_set(unit, &out_fields,
                                     TM_THD_MC_Qt_OPERATIONAL_STATEf,
                                     0, PORT_INFO_UNAVAILABLE));
            }
            SHR_IF_ERR_EXIT
                (bcmtm_field_list_set(unit, &out_fields, TM_THD_MC_Qt_PORT_IDf,
                                      0, lport));
            SHR_IF_ERR_EXIT
                (bcmtm_field_list_set(unit, &out_fields, TM_THD_MC_Qt_TM_MC_Q_IDf,
                                      0, queue));
            SHR_IF_ERR_EXIT
                (bcmimm_entry_update(unit, FALSE, sid, &out_fields));
        }
        bcmtm_field_list_free(&out_fields);
    }

exit:
    SHR_FREE(data_fields);
    bcmtm_field_list_free(&out_fields);
    bcmtm_field_list_free(&lkup_fields);
    bcmtm_field_list_free(&keys);
    SHR_FUNC_EXIT();
}
