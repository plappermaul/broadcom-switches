/*! \file bcmtm_imm_wred_ucq.c
 *
 * In-memory call back function for unicast queue configurations for WRED.
 */
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

#include <bcmdrd/bcmdrd_dev.h>
#include <bcmlrd/bcmlrd_map.h>
#include <bcmltd/bcmltd_types.h>
#include <bcmltd/bcmltd_handler.h>
#include <bcmltd/chip/bcmltd_id.h>
#include <shr/shr_debug.h>
#include <bcmlrd/bcmlrd_table.h>
#include <bcmimm/bcmimm_int_comp.h>
#include <bcmtm/bcmtm_drv.h>
#include <bcmtm/bcmtm_utils_internal.h>
#include <bcmtm/bcmtm_utils.h>
#include <bcmtm/bcmtm_types.h>
#include <bcmtm/imm/bcmtm_imm_defines_internal.h>
#include <bcmtm/wred/bcmtm_wred_internal.h>
/*******************************************************************************
 * Local definitions
 */
#define BSL_LOG_MODULE  BSL_LS_BCMTM_IMM

/*******************************************************************************
 * Private functions
 */
/*!
 * \brief  Populates WRED unicast configuration using user defined values.
 *
 * \param [in] unit Unit number.
 * \param [in] data This is a linked list of the data fields in the
 * modified entry.
 * \param [out] wred_ucq_cfg  WRED Unicast queue configuration populated.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Error code for corresponding failures.
 */
static void
bcmtm_wred_ucq_cfg_parse(int unit,
                         const bcmltd_field_t *data,
                         bcmtm_wred_cfg_t *wred_cfg)
{
    bcmltd_field_t *cur_data = (bcmltd_field_t *)data;

    while(cur_data) {
        switch(cur_data->id) {
            case TM_WRED_UC_Qt_WEIGHTf:
                wred_cfg->weight = (uint8_t)cur_data->data;
                break;
            case TM_WRED_UC_Qt_CURRENT_Q_SIZEf:
                wred_cfg->curr_q_size = (uint8_t)cur_data->data;
                break;
            case TM_WRED_UC_Qt_WREDf:
                wred_cfg->wred = (uint8_t)cur_data->data;
                break;
            case TM_WRED_UC_Qt_ECNf:
                wred_cfg->ecn = (uint8_t)cur_data->data;
                break;
            case TM_WRED_UC_Qt_ECN_MODEf:
                wred_cfg->ecn_mode = cur_data->data;
                break;
            case TM_WRED_UC_Qt_TM_WRED_TIME_PROFILE_IDf:
                wred_cfg->time_profile_id = (uint8_t)cur_data->data;
                break;
            case TM_WRED_UC_Qt_MARK_TM_WRED_CNG_NOTIFICATION_PROFILE_IDf:
                wred_cfg->mark_cng_profile_id = (uint8_t)cur_data->data;
                break;
            case TM_WRED_UC_Qt_DROP_TM_WRED_CNG_NOTIFICATION_PROFILE_IDf:
                wred_cfg->drop_cng_profile_id = (uint8_t)cur_data->data;
                break;
            case TM_WRED_UC_Qt_TM_WRED_DROP_CURVE_SET_PROFILE_IDf:
                wred_cfg->wcs_profile_id = (uint8_t)cur_data->data;
                break;
            case TM_WRED_UC_Qt_OPERATIONAL_STATEf:
                wred_cfg->opcode = (uint8_t)cur_data->data;
                break;
            default:
                break;
        }
        cur_data = cur_data->next;
    }
}

/*! \brief TM_WRED_UC_Q imm lookup.
 *
 * Performs IMM lookup and populates wred cfg.
 *
 * \param [in] unit Logical unit number.
 * \param [in] ltid Logical table ID.
 * \param [in] lport Logical port ID.
 * \param [in] ucq_id Unicast queue ID.
 * \param [out] wred_cfg WRED Configuation from IMM.
 */
static int
bcmtm_wred_ucq_imm_lkup(int unit,
                        bcmltd_sid_t ltid,
                        bcmtm_lport_t lport,
                        uint32_t ucq_id,
                        bcmtm_wred_cfg_t *wred_cfg)
{
    bcmltd_fields_t out, in;
    bcmltd_fid_t fid;
    size_t num_fid;

    SHR_FUNC_ENTER(unit);
    sal_memset(&in, 0, sizeof(bcmltd_fields_t));
    sal_memset(&out, 0, sizeof(bcmltd_fields_t));

    SHR_IF_ERR_EXIT(bcmlrd_field_count_get(unit, ltid, &num_fid));
    SHR_IF_ERR_EXIT
        (bcmtm_field_list_alloc(unit, num_fid, &out));
    SHR_IF_ERR_EXIT
        (bcmtm_field_list_alloc(unit, 2, &in));
    in.count = 0;

    fid = TM_WRED_UC_Qt_PORT_IDf;
    SHR_IF_ERR_EXIT
        (bcmtm_field_list_set(unit, &in, fid, 0, lport));

    fid = TM_WRED_UC_Qt_TM_UC_Q_IDf;
    SHR_IF_ERR_EXIT
        (bcmtm_field_list_set(unit, &in, fid, 0, ucq_id));

    if (SHR_FAILURE(bcmimm_entry_lookup(unit, ltid, &in, &out))) {
        wred_cfg->opcode = ENTRY_INVALID;
    } else {
        bcmtm_wred_ucq_cfg_parse(unit, *(out.field), wred_cfg);
    }
exit:
    bcmtm_field_list_free(&in);
    bcmtm_field_list_free(&out);
    SHR_FUNC_EXIT();
}

/*!
 * \brief  WRED unicast queue physical table configuration based on device type.
 *
 * \param [in] unit  Unit number.
 * \param [in] lport Logical port number.
 * \param [in] ucq_id Unicast Queue ID.
 * \param [in] ltid Logical table ID.
 * \param [in] wred_cfg WRED unicast queue configuration.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Error code for corresponding failures.
 */
static int
bcmtm_wred_ucq_pt_set(int unit,
                      bcmtm_lport_t lport,
                      uint32_t ucq_id,
                      bcmltd_sid_t ltid,
                      bcmtm_wred_cfg_t *wred_cfg)
{
    bcmtm_drv_t *drv;
    bcmtm_wred_drv_t wred_drv;

    SHR_FUNC_ENTER(unit);
    sal_memset(&wred_drv, 0, sizeof(bcmtm_wred_drv_t));

    SHR_IF_ERR_EXIT(bcmtm_drv_get(unit, &drv));
    SHR_NULL_CHECK(drv->wred_drv_get, SHR_E_UNAVAIL);
    SHR_IF_ERR_EXIT(drv->wred_drv_get(unit, &wred_drv));

    /* update physical table. */
    if (wred_drv.ucq_set) {
        SHR_IF_ERR_EXIT
            (wred_drv.ucq_set(unit, lport, ucq_id, ltid, wred_cfg));
    }
exit:
    SHR_FUNC_EXIT();
}

/*!
 * \brief  Resets the physical table to default values.
 *
 * \param [in] unit Unit number.
 * \param [in] ltid Logical table ID.
 * \param [in] lport Logical port ID.
 * \param [in] ucq_id Unicast queueu ID.
 * \param [in] wred_cfg WRED Unicast queue configuration.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Error code for corresponding failures.
 */
static int
bcmtm_wred_ucq_reset(int unit,
                     bcmltd_sid_t ltid,
                     bcmtm_lport_t lport,
                     uint8_t ucq_id,
                     bcmtm_wred_cfg_t *wred_cfg)
{

    SHR_FUNC_ENTER(unit);

    SHR_IF_ERR_EXIT
        (bcmtm_wred_ucq_imm_lkup(unit, ltid, lport, ucq_id, wred_cfg));

    if (wred_cfg->opcode == VALID) {
        sal_memset(wred_cfg, 0, sizeof(bcmtm_wred_cfg_t));
        /* Physical table update */
        SHR_IF_ERR_EXIT
            (bcmtm_wred_ucq_pt_set(unit, lport, ucq_id, ltid, wred_cfg));
    }
exit:
    SHR_FUNC_EXIT();
}

/*!
 * \brief  WRED unicast queue logical table IMM update.
 *
 * \param [in] unit Unit number.
 * \param [in] ltid Logical table ID.
 * \param [in] lport Logical port ID.
 * \param [in] ucq_id Unicast queueu ID.
 * \param [in] wred_cfg WRED Unicast queue configuration.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Error code for corresponding failures.
 */
static int
bcmtm_wred_ucq_imm_update(int unit,
                          bcmltd_sid_t ltid,
                          bcmtm_lport_t lport,
                          uint32_t ucq_id,
                          bcmtm_wred_cfg_t *wred_cfg)
{
    bcmltd_fields_t in;
    bcmltd_fid_t fid;

    SHR_FUNC_ENTER(unit);

    sal_memset(&in, 0, sizeof(bcmltd_fields_t));
    SHR_IF_ERR_EXIT
        (bcmtm_field_list_alloc(unit, 3, &in));

    in.count = 0;

    /* TM_WRED_UC_Qt_PORT_IDf */
    fid =TM_WRED_UC_Qt_PORT_IDf;
    SHR_IF_ERR_EXIT
        (bcmtm_field_list_set(unit, &in, fid, 0, lport));

    /* TM_WRED_UC_Qt_TM_UC_Q_IDf */
    fid = TM_WRED_UC_Qt_TM_UC_Q_IDf;
    SHR_IF_ERR_EXIT
        (bcmtm_field_list_set(unit, &in, fid, 0, ucq_id));

    /* TM_WRED_UC_Qt_OPERATIONAL_STATEf */
    fid = TM_WRED_UC_Qt_OPERATIONAL_STATEf;
    SHR_IF_ERR_EXIT
        (bcmtm_field_list_set(unit, &in, fid, 0, wred_cfg->opcode));

    SHR_IF_ERR_EXIT
        (bcmimm_entry_update(unit, 0, ltid, &in));
exit:
    bcmtm_field_list_free(&in);
    SHR_FUNC_EXIT();
}

/*!
 * \brief Configures the physical table with user defined values.
 *
 * \param [in] unit Unit number.
 * \param [in] ltid Logical table ID.
 * \param [in] lport Logical port ID.
 * \param [in] ucq_id Unicast queue ID.
 * \param [in] wred_cfg WRED unicast queue configuration.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Error code for corresponding failures.
 */
static int
bcmtm_wred_ucq_set(int unit,
                   bcmltd_sid_t ltid,
                   bcmtm_lport_t lport,
                   uint32_t ucq_id,
                   bcmtm_wred_cfg_t *wred_cfg)
{
    int mport, num_ucq;
    uint8_t action;

    SHR_FUNC_ENTER(unit);

    action = wred_cfg->action;

    /* WRED is supported for front panel and management ports. */
    if (bcmtm_lport_is_cpu(unit, lport) ||
        bcmtm_lport_is_lb(unit, lport)) {
        SHR_ERR_EXIT(SHR_E_PARAM);
    }

    switch(action) {

        case ACTION_INSERT:
            /*
             * TM local port information.
             * Returns error if local port to physical port map is not available.
             * Updates opeartional state.
             */
            if (SHR_FAILURE(bcmtm_lport_mmu_local_port_get(unit, lport, &mport))) {
                wred_cfg->opcode = PORT_INFO_UNAVAIL;
                SHR_EXIT();
            }

            /* Fall through */
        case ACTION_PORT_UPDATE_INT:
            /* Unicast queue id is exceeding the max limit. */
            SHR_IF_ERR_EXIT
                (bcmtm_lport_num_ucq_get(unit, lport, &num_ucq));
            if (ucq_id >= (uint32_t)num_ucq) {
                wred_cfg->opcode = UC_Q_INVALID;
                SHR_EXIT();
            }
            /* PT update */
            SHR_IF_ERR_EXIT
                (bcmtm_wred_ucq_pt_set(unit, lport, ucq_id, ltid, wred_cfg));
            wred_cfg->opcode = VALID;
            break;

        case ACTION_PORT_DELETE_INT:
            /* PT update */
            SHR_IF_ERR_EXIT
                (bcmtm_wred_ucq_reset(unit, ltid, lport, ucq_id, wred_cfg));
            wred_cfg->opcode = PORT_INFO_UNAVAIL;
            break;
        default:
            break;

    }
exit:
    SHR_FUNC_EXIT();
}


/*!
 * \brief TM_WRED_UC_Q logical table callback function for staging.
 *
 * Handle TM_WRED_UC_Q IMM table change events.
 *
 * \param [in] unit Unit number.
 * \param [in] sid This is the logical table ID.
 * \param [in] trans_id LT Transaction Id
 * \param [in] event_reason Call back reason.
 * \param [in] key This is a linked list of the key fields identifying
 * the entry.
 * \param [in] data This is a linked list of the data fields in
 * modified entry.
 * \param [in] context Is a pointer that was given during registration.
 * \param [out] data. This is a linked list of extra added data fields (mostly
 * read only fields).
 * The callback can use this pointer to retrieve some context.
 *
 * \retval SHR_E_NONE No errors.
 * \retval SHR_E_FAIL LM fails to handle LT change events.
 */
static int
bcmtm_wred_ucq_stage(int unit,
                     bcmltd_sid_t ltid,
                     uint32_t trans_id,
                     bcmimm_entry_event_t event_reason,
                     const bcmltd_field_t *key,
                     const bcmltd_field_t *data,
                     void *context,
                     bcmltd_fields_t *output_fields)
{
    bcmtm_wred_cfg_t wred_cfg;
    bcmtm_lport_t lport;
    bcmltd_fid_t fid;
    uint32_t ucq_id;
    uint64_t fval;

    SHR_FUNC_ENTER(unit);

    sal_memset(&wred_cfg, 0, sizeof(bcmtm_wred_cfg_t));

    fid = TM_WRED_UC_Qt_PORT_IDf;
    SHR_IF_ERR_EXIT
        (bcmtm_fval_get_from_field_array(unit, fid, 0, key, &fval));
    lport = (bcmtm_lport_t)fval;

    fid = TM_WRED_UC_Qt_TM_UC_Q_IDf;
    SHR_IF_ERR_EXIT
        (bcmtm_fval_get_from_field_array(unit, fid, 0, key, &fval));
    ucq_id = (bcmtm_lport_t)fval;

    switch(event_reason) {
        case BCMIMM_ENTRY_UPDATE:
            SHR_IF_ERR_EXIT
                (bcmtm_wred_ucq_imm_lkup(unit, ltid, lport, ucq_id, &wred_cfg));

            /* Fall through */
        case BCMIMM_ENTRY_INSERT:
            SHR_NULL_CHECK(output_fields, SHR_E_PARAM);
            output_fields->count = 0;
            bcmtm_wred_ucq_cfg_parse(unit, data, &wred_cfg);
            wred_cfg.action = ACTION_INSERT;
            SHR_IF_ERR_EXIT
                (bcmtm_wred_ucq_set(unit, ltid, lport, ucq_id, &wred_cfg));

            /* Operation state update */
            fid = TM_WRED_UC_Qt_OPERATIONAL_STATEf;
            SHR_IF_ERR_EXIT
                (bcmtm_field_list_set(unit, output_fields, fid, 0,
                                      wred_cfg.opcode));
            break;
        case BCMIMM_ENTRY_DELETE:
            SHR_IF_ERR_EXIT
                (bcmtm_wred_ucq_reset(unit, ltid, lport, ucq_id, &wred_cfg));
            break;
        default:
            break;
    }
exit:
    SHR_FUNC_EXIT();
}

/*******************************************************************************
 * Public functions
 */
int
bcmtm_wred_ucq_internal_update(int unit,
                               bcmtm_lport_t lport,
                               uint8_t action)
{
    uint32_t ucq_id;
    int num_ucq;
    bcmltd_sid_t ltid = TM_WRED_UC_Qt;
    bcmtm_wred_cfg_t wred_cfg;

    SHR_FUNC_ENTER(unit);

    /* Get number of unicast queue for the port. */
    SHR_IF_ERR_EXIT
        (bcmtm_lport_num_ucq_get(unit, lport, &num_ucq));

    for (ucq_id = 0; ucq_id < (uint32_t)num_ucq; ucq_id++) {
        SHR_IF_ERR_EXIT
           (bcmtm_wred_ucq_imm_lkup(unit, ltid, lport, ucq_id, &wred_cfg));
        wred_cfg.action = action;
        if (wred_cfg.opcode != ENTRY_INVALID) {
            SHR_IF_ERR_EXIT
                (bcmtm_wred_ucq_set(unit, ltid, lport, ucq_id, &wred_cfg));
            SHR_IF_ERR_EXIT
                (bcmtm_wred_ucq_imm_update(unit, ltid, lport, ucq_id,
                                           &wred_cfg));
        }
    }
exit:
    SHR_FUNC_EXIT();
}

int
bcmtm_wred_ucq_imm_register(int unit)
{
    /*! Inmemory callback functions for TM_WRED_UC_Q logical table. */
    bcmimm_lt_cb_t bcmtm_wred_ucq_imm_cb = {
        /*! Staging function. */
        .stage = bcmtm_wred_ucq_stage,
    };
    const bcmlrd_map_t *map = NULL;
    int rv;

    SHR_FUNC_ENTER(unit);
    rv = bcmlrd_map_get(unit, TM_WRED_UC_Qt, &map);
    SHR_IF_ERR_EXIT_EXCEPT_IF(rv, SHR_E_UNAVAIL);

    if ((rv == SHR_E_UNAVAIL) || (!map)) {
        SHR_EXIT();
    }
    /*! Registers callback functions for WRED unicast queue. */
    SHR_IF_ERR_EXIT
        (bcmimm_lt_event_reg(unit, TM_WRED_UC_Qt, &bcmtm_wred_ucq_imm_cb, NULL));
exit:
    SHR_FUNC_EXIT();
}
