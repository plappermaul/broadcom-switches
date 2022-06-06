/*! \file bcmcth_ctr_eflex_range_check.c
 *
 * BCMCTH CTR_EFLEX_RANGE_CHK_PROFILEt IMM handler.
 */
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

#include <sal/sal_libc.h>
#include <bcmltd/chip/bcmltd_id.h>

#include <bcmptm/bcmptm.h>
#include <bcmcth/bcmcth_ctr_eflex_drv.h>
#include <bcmcth/bcmcth_state_eflex_drv.h>
#include <bcmcth/bcmcth_ctr_eflex_internal.h>

/* Debug log target definition */
#define BSL_LOG_MODULE BSL_LS_BCMCTH_CTREFLEX

/*! Generate CTR_xxx_EFLEX_RANGE_CHK_PROFILE LT field ids. */
#define CTR_ING_EGR_RANGE_CHK_PROFILE_FIDS(x) \
    CTR_ING_EFLEX_RANGE_CHK_PROFILEt_##x, \
    CTR_EGR_EFLEX_RANGE_CHK_PROFILEt_##x

/*! Generate FLEX_STATE_xxx_RANGE_CHK_PROFILE LT field ids. */
#define STATE_ING_EGR_RANGE_CHK_PROFILE_FIDS(x) \
    FLEX_STATE_ING_RANGE_CHK_PROFILEt_##x, \
    FLEX_STATE_EGR_RANGE_CHK_PROFILEt_##x

/*! Generate CTR_xxx_EFLEX_RANGE_CHK_PROFILE_INFO LT field ids. */
#define CTR_ING_EGR_RANGE_CHK_PROFILE_INFO_FIDS(x) \
    CTR_ING_EFLEX_RANGE_CHK_PROFILE_INFOt_##x, \
    CTR_EGR_EFLEX_RANGE_CHK_PROFILE_INFOt_##x

/*! Generate FLEX_STATE_xxx_RANGE_CHK_PROFILE_INFO LT field ids. */
#define STATE_ING_EGR_RANGE_CHK_PROFILE_INFO_FIDS(x) \
    FLEX_STATE_ING_RANGE_CHK_PROFILE_INFOt_##x, \
    FLEX_STATE_EGR_RANGE_CHK_PROFILE_INFOt_##x

/*******************************************************************************
 * Local definitions
 */

/* RANGE_CHK_PROFILE LT field info */
static const bcmlrd_sid_t range_chk_table_sid[][2] = {
        {CTR_ING_EGR_LT_IDS(EFLEX_RANGE_CHK_PROFILEt)},
        {STATE_ING_EGR_LT_IDS(RANGE_CHK_PROFILEt)},
};

static const bcmlrd_fid_t range_chk_id_fid[][2] = {
    {CTR_ING_EFLEX_RANGE_CHK_PROFILEt_CTR_ING_EFLEX_RANGE_CHK_PROFILE_IDf,
     CTR_EGR_EFLEX_RANGE_CHK_PROFILEt_CTR_EGR_EFLEX_RANGE_CHK_PROFILE_IDf},
    {FLEX_STATE_ING_RANGE_CHK_PROFILEt_FLEX_STATE_ING_RANGE_CHK_PROFILE_IDf,
     FLEX_STATE_EGR_RANGE_CHK_PROFILEt_FLEX_STATE_EGR_RANGE_CHK_PROFILE_IDf},
};

static const bcmlrd_fid_t obj_select_fid[][2] = {
        {CTR_ING_EGR_RANGE_CHK_PROFILE_FIDS(OBJ_SELECTf)},
        {STATE_ING_EGR_RANGE_CHK_PROFILE_FIDS(OBJ_SELECTf)},
};

static const bcmlrd_fid_t range_checker_fid[][2] = {
        {CTR_ING_EGR_RANGE_CHK_PROFILE_FIDS(RANGE_CHECKERf)},
        {STATE_ING_EGR_RANGE_CHK_PROFILE_FIDS(RANGE_CHECKERf)},
};

static const bcmlrd_fid_t pipe_idx_fid[][2] = {
        {CTR_ING_EGR_RANGE_CHK_PROFILE_FIDS(PIPEf)},
        {STATE_ING_EGR_RANGE_CHK_PROFILE_FIDS(PIPEf)},
};

static const bcmlrd_fid_t instance_fid[][2] = {
        {0, 0},
        {FLEX_STATE_ING_RANGE_CHK_PROFILEt_INSTANCEf, 0},
};

static const bcmlrd_fid_t min_fid[][2] = {
        {CTR_ING_EGR_RANGE_CHK_PROFILE_FIDS(MINf)},
        {STATE_ING_EGR_RANGE_CHK_PROFILE_FIDS(MINf)},
};

static const bcmlrd_fid_t egr_instance_fid[][2] = {
        {0, 0},
        {0, FLEX_STATE_EGR_RANGE_CHK_PROFILEt_INSTANCEf},
};

static const bcmlrd_fid_t max_fid[][2] = {
        {CTR_ING_EGR_RANGE_CHK_PROFILE_FIDS(MAXf)},
        {STATE_ING_EGR_RANGE_CHK_PROFILE_FIDS(MAXf)},
};

/* RANGE_CHK_PROFILE_INFO LT field info */
static const bcmlrd_sid_t range_chk_info_table_sid[][2] = {
        {CTR_ING_EGR_LT_IDS(EFLEX_RANGE_CHK_PROFILE_INFOt)},
        {STATE_ING_EGR_LT_IDS(RANGE_CHK_PROFILE_INFOt)},
};

static const bcmlrd_fid_t range_chk_info_id_fid[][2] = {
{CTR_ING_EFLEX_RANGE_CHK_PROFILE_INFOt_CTR_ING_EFLEX_RANGE_CHK_PROFILE_IDf,
 CTR_EGR_EFLEX_RANGE_CHK_PROFILE_INFOt_CTR_EGR_EFLEX_RANGE_CHK_PROFILE_IDf},
{FLEX_STATE_ING_RANGE_CHK_PROFILE_INFOt_FLEX_STATE_ING_RANGE_CHK_PROFILE_IDf,
 FLEX_STATE_EGR_RANGE_CHK_PROFILE_INFOt_FLEX_STATE_EGR_RANGE_CHK_PROFILE_IDf},
};

static const bcmlrd_fid_t info_obj_select_fid[][2] = {
        {CTR_ING_EGR_RANGE_CHK_PROFILE_INFO_FIDS(OBJ_SELECTf)},
        {STATE_ING_EGR_RANGE_CHK_PROFILE_INFO_FIDS(OBJ_SELECTf)},
};

static const bcmlrd_fid_t info_range_checker_fid[][2] = {
        {CTR_ING_EGR_RANGE_CHK_PROFILE_INFO_FIDS(RANGE_CHECKERf)},
        {STATE_ING_EGR_RANGE_CHK_PROFILE_INFO_FIDS(RANGE_CHECKERf)},
};

/**********************************************************************
* Private functions
 */

/* Function to initialize RANGE_CHK_PROFILE entry with invalid values */
static int
bcmcth_ctr_eflex_range_chk_entry_init(
    int unit,
    ctr_eflex_range_chk_data_t *entry)
{
    SHR_FUNC_ENTER(unit);

    entry->range_chk_id = CTR_EFLEX_INVALID_VAL;
    entry->pipe_idx = CTR_EFLEX_INVALID_VAL;
    entry->inst = CTR_EFLEX_INVALID_VAL;
    entry->obj_select = CTR_EFLEX_INVALID_VAL;
    entry->range_chk_idx = CTR_EFLEX_INVALID_VAL;
    entry->min = CTR_EFLEX_INVALID_VAL;
    entry->max = CTR_EFLEX_INVALID_VAL;

    SHR_FUNC_EXIT();
}

/* Function to get default values for RANGE_CHK_PROFILE entry */
static int
bcmcth_ctr_eflex_range_chk_default_get(
    int unit,
    ctr_eflex_range_chk_data_t *entry)
{
    bcmlrd_client_field_info_t *f_info = NULL;
    const bcmlrd_table_rep_t *lt_info = NULL;
    size_t num_fid = 0, i;
    int select;
    uint8_t pipe_unique;
    eflex_comp_t comp;

    SHR_FUNC_ENTER(unit);

    select  = entry->ingress ? 0 : 1;
    comp = entry->comp;

    /* Get pipe operating mode */
    SHR_IF_ERR_VERBOSE_EXIT
        (bcmcth_ctr_eflex_pipe_opermode_get(unit,
                                            entry->ingress,
                                            entry->comp,
                                            &pipe_unique));

    SHR_IF_ERR_VERBOSE_EXIT
        (bcmlrd_field_count_get(unit,
                                range_chk_table_sid[comp][select],
                                &num_fid));

    lt_info = bcmlrd_table_get(range_chk_table_sid[comp][select]);
    SHR_NULL_CHECK(lt_info, SHR_E_BADID);

    SHR_ALLOC(f_info, sizeof(bcmlrd_client_field_info_t) * num_fid,
              "bcmcthCtrEflexRangeAllocFields");
    SHR_NULL_CHECK(f_info, SHR_E_MEMORY);

    SHR_IF_ERR_VERBOSE_EXIT
        (bcmlrd_table_fields_def_get(unit,
                                     lt_info->name,
                                     num_fid,
                                     f_info,
                                     &num_fid));

    for (i = 0; i < num_fid; i++) {
        if (f_info[i].id == obj_select_fid[comp][select]) {
            entry->obj_select = f_info[i].def.u8;
        } else if (pipe_unique &&
                   f_info[i].id == pipe_idx_fid[comp][select]) {
            entry->pipe_idx = f_info[i].def.u8;
        } else if (entry->comp == STATE_EFLEX &&
                   entry->ingress == INGRESS &&
                  f_info[i].id == instance_fid[comp][select]) {
             entry->inst = (state_eflex_inst_t)f_info[i].def.u32;
        } else if (entry->comp == STATE_EFLEX &&
                 entry->ingress == EGRESS &&
                   f_info[i].id == egr_instance_fid[comp][select]) {
            entry->egr_inst = (state_eflex_egr_inst_t)f_info[i].def.u32;
        } else if (f_info[i].id == range_checker_fid[comp][select]) {
            entry->range_chk_idx = f_info[i].def.u8;
        } else if (f_info[i].id == min_fid[comp][select]) {
            entry->min = f_info[i].def.u16;
        } else if (f_info[i].id == max_fid[comp][select]) {
            entry->max = f_info[i].def.u16;
        }
    }

exit:
    SHR_FREE(f_info);
    SHR_FUNC_EXIT();
}

/*
 * Function to replace RANGE_CHK_PROFILE entry with values from another
 * entry if is invalid
 */
static int
bcmcth_ctr_eflex_range_chk_fill(int unit,
                                ctr_eflex_range_chk_data_t *entry,
                                ctr_eflex_range_chk_data_t *cur_entry)
{
    uint8_t pipe_unique;

    SHR_FUNC_ENTER(unit);

    /* Get pipe operating mode */
    SHR_IF_ERR_VERBOSE_EXIT
        (bcmcth_ctr_eflex_pipe_opermode_get(unit,
                                            entry->ingress,
                                            entry->comp,
                                            &pipe_unique));

    if (entry->range_chk_idx == CTR_EFLEX_INVALID_VAL) {
        entry->range_chk_idx = cur_entry->range_chk_idx;
    }

    if (pipe_unique && entry->pipe_idx == CTR_EFLEX_INVALID_VAL) {
        entry->pipe_idx = cur_entry->pipe_idx;
    }

    if (entry->ingress == INGRESS && cur_entry->ingress == INGRESS &&
        entry->comp == STATE_EFLEX &&
        entry->inst == CTR_EFLEX_INVALID_VAL) {
        entry->inst = cur_entry->inst;
    }

    if (entry->obj_select == CTR_EFLEX_INVALID_VAL) {
        entry->obj_select = cur_entry->obj_select;
    }

    if (entry->min == CTR_EFLEX_INVALID_VAL) {
        entry->min = cur_entry->min;
    }

    if (entry->max == CTR_EFLEX_INVALID_VAL) {
        entry->max = cur_entry->max;
    }

exit:
    SHR_FUNC_EXIT();
}

/* Function to parse data fields in given RANGE_CHK_PROFILE entry */
static int
bcmcth_ctr_eflex_range_chk_lt_fields_check(
    int unit,
    ctr_eflex_range_chk_data_t *entry,
    const bcmltd_field_t *data_fields)
{
    int select;
    uint8_t pipe_unique;
    eflex_comp_t comp;

    SHR_FUNC_ENTER(unit);

    select = entry->ingress ? 0 : 1;
    comp = entry->comp;

    /* Get pipe operating mode */
    SHR_IF_ERR_VERBOSE_EXIT
        (bcmcth_ctr_eflex_pipe_opermode_get(unit,
                                            entry->ingress,
                                            entry->comp,
                                            &pipe_unique));

    /* data_fields contains the key field for get_first/next IMM API */
    if (data_fields->id == range_chk_id_fid[comp][select]) {
        entry->range_chk_id = data_fields->data;
    } else if (data_fields->id == obj_select_fid[comp][select]) {
        entry->obj_select = data_fields->data;
    } else if (pipe_unique &&
               data_fields->id == pipe_idx_fid[comp][select]) {
        entry->pipe_idx = data_fields->data;
    } else if (entry->comp == STATE_EFLEX &&
               entry->ingress == INGRESS &&
               data_fields->id == instance_fid[comp][select]) {
        entry->inst = data_fields->data;
    } else if (entry->comp == STATE_EFLEX &&
               entry->ingress == EGRESS &&
               data_fields->id == egr_instance_fid[comp][select]) {
        entry->egr_inst = data_fields->data;
    } else if (data_fields->id == range_checker_fid[comp][select]) {
        entry->range_chk_idx = data_fields->data;
    } else if (data_fields->id == min_fid[comp][select]) {
        entry->min = data_fields->data;
    } else if (data_fields->id == max_fid[comp][select]) {
        entry->max = data_fields->data;
    }

exit:
    SHR_FUNC_EXIT();
}

/* Function to parse given RANGE_CHK_PROFILE entry */
static int
bcmcth_ctr_eflex_range_chk_lt_fields_parse(
    int unit,
    const bcmltd_field_t *key_fields,
    const bcmltd_field_t *data_fields,
    ctr_eflex_range_chk_data_t *entry)
{
    int select;
    eflex_comp_t comp;

    SHR_FUNC_ENTER(unit);

    select = entry->ingress ? 0 : 1;
    comp = entry->comp;

    /* Parse key fields */
    while (key_fields) {
        if (key_fields->id == range_chk_id_fid[comp][select]) {
            entry->range_chk_id = key_fields->data;
        }

        key_fields = key_fields->next;
    }

    /* Parse data fields */
    while (data_fields) {
        SHR_IF_ERR_VERBOSE_EXIT
            (bcmcth_ctr_eflex_range_chk_lt_fields_check(unit,
                                                        entry,
                                                        data_fields));

        data_fields = data_fields->next;
    }

exit:
    SHR_FUNC_EXIT();
}

/* Function to parse OPERAND_PROFILE IMM entry */
static int
bcmcth_ctr_eflex_range_chk_entry_parse(
    int unit,
    ctr_eflex_range_chk_data_t *entry,
    bcmltd_fields_t *flist)
{
    uint32_t i;
    bcmltd_field_t *data_fields = NULL;

    SHR_FUNC_ENTER(unit);

    /* extract range_chk_id and other fields */
    for (i = 0; i < flist->count; i++) {
        data_fields = flist->field[i];

        SHR_IF_ERR_VERBOSE_EXIT
            (bcmcth_ctr_eflex_range_chk_lt_fields_check(unit,
                                                        entry,
                                                        data_fields));
    }

exit:
    SHR_FUNC_EXIT();
}

/* Function to get range check profile id resource list */
static int
bcmcth_ctr_eflex_range_chk_id_list_get(int unit,
                                       uint32_t pipe_idx,
                                       ctr_eflex_range_chk_data_t *entry,
                                       ctr_eflex_pool_list_t **list)
{
    ctr_eflex_control_t *ctrl = NULL;

    SHR_FUNC_ENTER(unit);

    /* Get flex control struct */
    SHR_IF_ERR_VERBOSE_EXIT
        (bcmcth_eflex_ctrl_get(unit, entry->comp, &ctrl));

    switch (entry->comp) {
        case CTR_EFLEX:
            *list = entry->ingress ? &ctrl->ing_range_chk_id_list[pipe_idx] :
                                     &ctrl->egr_range_chk_id_list[pipe_idx];
            break;
        case STATE_EFLEX:
            if (entry->ingress) {
                switch (entry->inst) {
                    case ING_POST_LKUP_INST:
                        *list = &ctrl->ing_range_chk_id_list[pipe_idx];
                        break;
                    case ING_POST_FWD_INST:
                        *list =
                        &ctrl->state_ext.ing_range_chk_id_list_b[pipe_idx];
                        break;
                    default:
                        SHR_ERR_EXIT(SHR_E_PARAM);
                } /* end inner SWITCH */
            } else {
                switch (entry->egr_inst) {
                    case EGR_POST_LKUP_INST:
                        *list = &ctrl->egr_range_chk_id_list[pipe_idx];
                        break;
                    case EGR_POST_FWD_INST:
                        *list =
                        &ctrl->state_ext.egr_range_chk_id_list_b[pipe_idx];
                        break;
                    default:
                        SHR_ERR_EXIT(SHR_E_PARAM);
                } /* end inner SWITCH */
            }
            break;
        default:
            SHR_ERR_EXIT(SHR_E_PARAM);
    } /* end outer SWITCH */

exit:
    SHR_FUNC_EXIT();
}

/* Function to validate range checker in RANGE_CHK_PROFILE entry */
static int
bcmcth_ctr_eflex_range_chk_validate(
    int unit,
    ctr_eflex_range_chk_data_t *entry)
{
    uint32_t pipe_idx;
    ctr_eflex_pool_list_t *list = NULL;

    SHR_FUNC_ENTER(unit);

    /* Check if given range checker is already in use in all applicable pipes */
    for (pipe_idx = entry->start_pipe_idx;
         pipe_idx < entry->end_pipe_idx;
         pipe_idx++) {

        /* Get range_check profile id resource list */
        SHR_IF_ERR_VERBOSE_EXIT
            (bcmcth_ctr_eflex_range_chk_id_list_get(unit,
                                                    pipe_idx,
                                                    entry,
                                                    &list));

        SHR_IF_ERR_VERBOSE_EXIT
            (bcmcth_ctr_eflex_id_validate(unit,
                                          entry->bank_num,
                                          list));
    }

exit:
    SHR_FUNC_EXIT();
}

/* Function to set/clear range checker in resource list */
static int
bcmcth_ctr_eflex_range_checker_set(
    int unit,
    bool val,
    ctr_eflex_range_chk_data_t *entry)
{
    uint32_t pipe_idx;
    ctr_eflex_pool_list_t *list = NULL;

    SHR_FUNC_ENTER(unit);

    /* Set/Clear given range checker in all applicable pipes */
    for (pipe_idx = entry->start_pipe_idx;
         pipe_idx < entry->end_pipe_idx;
         pipe_idx++) {

        /* Get range_check profile id resource list */
        SHR_IF_ERR_VERBOSE_EXIT
            (bcmcth_ctr_eflex_range_chk_id_list_get(unit,
                                                    pipe_idx,
                                                    entry,
                                                    &list));

        SHR_IF_ERR_VERBOSE_EXIT
            (bcmcth_ctr_eflex_id_set(unit,
                                     val,
                                     entry->bank_num,
                                     list));
    }

exit:
    SHR_FUNC_EXIT();
}

/* Function to insert/delete RANGE_CHK_PROFILE_INFO entry */
static int
bcmcth_ctr_eflex_range_chk_info_program(int unit,
                                         bool insert,
                                         ctr_eflex_range_chk_data_t *entry)
{
    int select;
    bcmltd_fields_t range_chk_info_lt_flds = {0};
    size_t num_fields, f_cnt, count;
    bcmlrd_fid_t   *fid_list = NULL;
    eflex_comp_t comp;

    SHR_FUNC_ENTER(unit);

    select = entry->ingress ? 0 : 1;
    comp = entry->comp;

    /* Get number of fields. */
    SHR_IF_ERR_VERBOSE_EXIT
        (bcmlrd_field_count_get(unit,
                                range_chk_info_table_sid[comp][select],
                                &num_fields));

    SHR_ALLOC(fid_list, sizeof(bcmlrd_fid_t) * num_fields,
        "bcmcthCtrEFlexRngInfoFields");
    SHR_NULL_CHECK(fid_list, SHR_E_MEMORY);
    sal_memset(fid_list, 0, sizeof(bcmlrd_fid_t) * num_fields);

    SHR_IF_ERR_VERBOSE_EXIT
        (bcmlrd_field_list_get(unit,
                               range_chk_info_table_sid[comp][select],
                               num_fields,
                               fid_list,
                               &count));

    range_chk_info_lt_flds.field = NULL;

    SHR_IF_ERR_VERBOSE_EXIT
        (bcmcth_ctr_eflex_fields_alloc(unit,
                                       &range_chk_info_lt_flds,
                                       num_fields));

    for (f_cnt = 0; f_cnt < count; f_cnt++) {
        range_chk_info_lt_flds.field[f_cnt]->id = fid_list[f_cnt];

        if (fid_list[f_cnt] == range_chk_info_id_fid[comp][select]) {
            range_chk_info_lt_flds.field[f_cnt]->data =
                                        entry->range_chk_id;
        } else if (fid_list[f_cnt] == info_obj_select_fid[comp][select]) {
            range_chk_info_lt_flds.field[f_cnt]->data =
                                        entry->obj_select;
        } else if (fid_list[f_cnt] == info_range_checker_fid[comp][select]) {
            range_chk_info_lt_flds.field[f_cnt]->data =
                                        entry->range_chk_idx;
        }
    }

    range_chk_info_lt_flds.count = count;

    if (insert) {
        SHR_IF_ERR_VERBOSE_EXIT
            (bcmimm_entry_insert(unit,
                                 range_chk_info_table_sid[comp][select],
                                 &range_chk_info_lt_flds));
    } else {
        SHR_IF_ERR_VERBOSE_EXIT
            (bcmimm_entry_delete(unit,
                                 range_chk_info_table_sid[comp][select],
                                 &range_chk_info_lt_flds));
    }

exit:
    /* Restore range_chk_info_lt_flds.count */
    range_chk_info_lt_flds.count = num_fields;
    bcmcth_ctr_eflex_fields_free(unit, &range_chk_info_lt_flds);
    SHR_FREE(fid_list);
    SHR_FUNC_EXIT();
}

/*
 * Function to lookup RANGE_CHK_PROFILE table
 * with given range check profile id.
 * It populates the LT entry with all fields except the key field
 * which is provided within the entry as input.
 */
static int
bcmcth_ctr_eflex_range_chk_lookup(int unit,
                                  ctr_eflex_range_chk_data_t *entry)
{
    bcmltd_fields_t key_fields = {0};
    bcmltd_fields_t imm_fields = {0};
    size_t num_fields = 0, num_key_fields;
    int select;
    uint32_t t;
    ctr_eflex_control_t *ctrl = NULL;
    eflex_comp_t comp;

    SHR_FUNC_ENTER(unit);

    select = entry->ingress ? 0 : 1;
    comp = entry->comp;

    /* Get flex control struct */
    SHR_IF_ERR_VERBOSE_EXIT
        (bcmcth_eflex_ctrl_get(unit, comp, &ctrl));

    /* Assign default values */
    SHR_IF_ERR_VERBOSE_EXIT
        (bcmcth_ctr_eflex_range_chk_default_get(unit, entry));

    for (t = 0; t < ctrl->lt_info_num; t++) {
        if (ctrl->ctr_eflex_field_info[t].lt_sid ==
            range_chk_table_sid[comp][select]) {
            break;
        }
    }

    if (t == ctrl->lt_info_num) {
        LOG_ERROR(BSL_LOG_MODULE,
            (BSL_META_U(unit, "RANGE_CHK_PROFILE LT info not found.\n")));
        SHR_ERR_EXIT(SHR_E_INTERNAL);
    }

    num_key_fields = ctrl->ctr_eflex_field_info[t].num_key_fields;

    /* Allocate key field */
    SHR_IF_ERR_VERBOSE_EXIT
        (bcmcth_ctr_eflex_fields_alloc(unit,
                                       &key_fields,
                                       num_key_fields));

    key_fields.count = num_key_fields;
    key_fields.field[0]->id   = range_chk_id_fid[comp][select];
    key_fields.field[0]->data = entry->range_chk_id;

    /* Allocate imm fields */
    SHR_IF_ERR_VERBOSE_EXIT
        (bcmlrd_field_count_get(unit,
                                range_chk_table_sid[comp][select],
                                &num_fields));

    SHR_IF_ERR_VERBOSE_EXIT
        (bcmcth_ctr_eflex_fields_alloc(unit, &imm_fields, num_fields));

    /* lookup IMM table; error if entry not found */
    SHR_IF_ERR_VERBOSE_EXIT
        (bcmimm_entry_lookup(unit,
                             range_chk_table_sid[comp][select],
                             &key_fields,
                             &imm_fields));

    /* Parse IMM entry fields */
    SHR_IF_ERR_VERBOSE_EXIT
        (bcmcth_ctr_eflex_range_chk_entry_parse(unit, entry, &imm_fields));

exit:
    bcmcth_ctr_eflex_fields_free(unit, &key_fields);
    /* imm_fields.count may be modified by bcmimm_entry_lookup */
    imm_fields.count = num_fields;
    bcmcth_ctr_eflex_fields_free(unit, &imm_fields);

    SHR_FUNC_EXIT();
}

/* Function to help insert RANGE_CHK_PROFILE entry based on cold or warm boot */
static int
bcmcth_ctr_eflex_range_chk_insert_helper(int unit,
                                          bool warm,
                                          ctr_eflex_range_chk_data_t *entry)
{
    uint8_t pipe_unique;
    int num_range_checker_per_bank;
    ctr_eflex_control_t *ctrl = NULL;
    bcmcth_ctr_eflex_device_info_t *device_info;

    SHR_FUNC_ENTER(unit);

    /* Get flex control struct */
    SHR_IF_ERR_VERBOSE_EXIT
        (bcmcth_eflex_ctrl_get(unit, entry->comp, &ctrl));

    device_info = ctrl->ctr_eflex_device_info;
    num_range_checker_per_bank = device_info->num_range_checker_per_bank;

    /* Get pipe operating mode */
    SHR_IF_ERR_VERBOSE_EXIT
        (bcmcth_ctr_eflex_pipe_opermode_get(unit,
                                            entry->ingress,
                                            entry->comp,
                                            &pipe_unique));

    /* Determine first and last pipe number */
    SHR_IF_ERR_VERBOSE_EXIT
        (bcmcth_ctr_eflex_num_pipes_get(unit,
                                        entry->ingress,
                                        entry->comp,
                                        entry->pipe_idx,
                                        &entry->start_pipe_idx,
                                        &entry->end_pipe_idx));

    /* Determine range checker bank number */
    entry->bank_num = (entry->obj_select * num_range_checker_per_bank) +
                      entry->range_chk_idx;

    LOG_VERBOSE(BSL_LOG_MODULE,
        (BSL_META_U(unit,
            "Insert %s Entry:\n"
            "range_chk_id %d, pipe_unique %d, pipe_idx %d, inst %d\n"
            "obj_select %d, range_checker %d bank_num %d\n"
            "min %d, max %d egr_inst %d\n"),
            entry->ingress ? "Ingress" : "Egress",
            entry->range_chk_id, pipe_unique,
            entry->pipe_idx, entry->inst,
            entry->obj_select, entry->range_chk_idx, entry->bank_num,
            entry->min, entry->max, entry->egr_inst));

    /* Validate and write to PT only during coldboot */
    if (!warm) {
        /* Validate range checker on all applicable pipes */
        SHR_IF_ERR_VERBOSE_EXIT
            (bcmcth_ctr_eflex_range_chk_validate(unit, entry));

        /* Write to range check profile table */
        if (entry->comp == CTR_EFLEX) {
            SHR_IF_ERR_VERBOSE_EXIT
                (bcmcth_ctr_eflex_range_chk_write(unit, entry));
        } else if (entry->comp == STATE_EFLEX) {
            SHR_IF_ERR_VERBOSE_EXIT
                (bcmcth_state_eflex_range_chk_write(unit, entry));
        }

        /* Insert range check profile info LT entry */
        SHR_IF_ERR_VERBOSE_EXIT
            (bcmcth_ctr_eflex_range_chk_info_program(unit, true, entry));
    }

    /* Set range checker bit in resource list */
    SHR_IF_ERR_VERBOSE_EXIT
        (bcmcth_ctr_eflex_range_checker_set(unit, ENABLE, entry));

exit:
    SHR_FUNC_EXIT();
}

/*
 * Function to recover internal state from RANGE_CHK_PROFILE LT
 * during warmboot
 */
int
bcmcth_ctr_eflex_range_chk_state_recover(int unit,
                                         bool ingress,
                                         eflex_comp_t comp)
{
    bcmltd_fields_t imm_fields = {0};
    bcmltd_fields_t key_fields = {0};
    size_t num_fields = 0, num_key_fields;
    int rv, select;
    uint32_t t;
    ctr_eflex_range_chk_data_t entry;
    ctr_eflex_control_t *ctrl = NULL;

    SHR_FUNC_ENTER(unit);
    select  = ingress ? 0 : 1;

    /* Get flex control struct */
    SHR_IF_ERR_VERBOSE_EXIT
        (bcmcth_eflex_ctrl_get(unit, comp, &ctrl));

    for (t = 0; t < ctrl->lt_info_num; t++) {
        if (ctrl->ctr_eflex_field_info[t].lt_sid ==
            range_chk_table_sid[comp][select]) {
            break;
        }
    }

    if (t == ctrl->lt_info_num) {
        LOG_ERROR(BSL_LOG_MODULE,
            (BSL_META_U(unit, "RANGE_CHK_PROFILE LT info not found.\n")));
        SHR_ERR_EXIT(SHR_E_INTERNAL);
    }

    num_key_fields = ctrl->ctr_eflex_field_info[t].num_key_fields;

    /* Allocate key field */
    SHR_IF_ERR_VERBOSE_EXIT
        (bcmcth_ctr_eflex_fields_alloc(unit,
                                       &key_fields,
                                       num_key_fields));

    key_fields.count = num_key_fields;
    key_fields.field[0]->id = range_chk_id_fid[comp][select];

    /* Allocate imm fields */
    SHR_IF_ERR_VERBOSE_EXIT
        (bcmlrd_field_count_get(unit,
                                range_chk_table_sid[comp][select],
                                &num_fields));

    SHR_IF_ERR_VERBOSE_EXIT
        (bcmcth_ctr_eflex_fields_alloc(unit, &imm_fields, num_fields));

    /* Get first entry from RANGE_CHK_PROFILE IMM LT */
    rv = bcmimm_entry_get_first(unit,
                                range_chk_table_sid[comp][select],
                                &imm_fields);

    while (rv == SHR_E_NONE) {
        sal_memset(&entry, 0, sizeof(ctr_eflex_range_chk_data_t));
        entry.ingress = ingress;
        entry.comp = comp;

        /* Assign default values */
        SHR_IF_ERR_VERBOSE_EXIT
            (bcmcth_ctr_eflex_range_chk_default_get(unit, &entry));

        /* Parse IMM entry fields */
        SHR_IF_ERR_VERBOSE_EXIT
            (bcmcth_ctr_eflex_range_chk_entry_parse(unit, &entry,&imm_fields));

        /* Insert entry in PT */
        SHR_IF_ERR_VERBOSE_EXIT
            (bcmcth_ctr_eflex_range_chk_insert_helper(unit, true, &entry));

        /* Setup key for next entry */
        key_fields.field[0]->data = entry.range_chk_id;

        /* imm_fields.count may be modified. */
        imm_fields.count = num_fields;

        /* Get next entry from RANGE_CHK_PROFILE IMM LT */
        rv = bcmimm_entry_get_next(unit,
                                   range_chk_table_sid[comp][select],
                                   &key_fields,
                                   &imm_fields);
    } /* end WHILE */

    if (rv == SHR_E_NOT_FOUND) {
        rv = SHR_E_NONE;
    }

exit:
    bcmcth_ctr_eflex_fields_free(unit, &key_fields);
    /* imm_fields.count may be modified. */
    imm_fields.count = num_fields;
    bcmcth_ctr_eflex_fields_free(unit, &imm_fields);

    SHR_FUNC_EXIT();

}

/* Function to insert entry into RANGE_CHK_PROFILE IMM LT */
static int
bcmcth_ctr_eflex_range_chk_lt_entry_insert(
    int unit,
    const bcmltd_field_t *key_fields,
    const bcmltd_field_t *data_fields,
    ctr_eflex_range_chk_data_t *entry)
{
    SHR_FUNC_ENTER(unit);

    /* Get default values */
    SHR_IF_ERR_VERBOSE_EXIT
        (bcmcth_ctr_eflex_range_chk_default_get(unit, entry));

    /*
     * Parse user values
     * default values will be over-written by user values, if present
     */
    SHR_IF_ERR_VERBOSE_EXIT
        (bcmcth_ctr_eflex_range_chk_lt_fields_parse(unit,
                                                    key_fields,
                                                    data_fields,
                                                    entry));

    /* Insert entry in PT */
    SHR_IF_ERR_VERBOSE_EXIT
        (bcmcth_ctr_eflex_range_chk_insert_helper(unit, false, entry));

exit:
    SHR_FUNC_EXIT();
}

/* Function to delete entry from RANGE_CHK_PROFILE IMM LT */
static int
bcmcth_ctr_eflex_range_chk_lt_entry_delete(
    int unit,
    ctr_eflex_range_chk_data_t *entry)
{
    state_eflex_inst_t inst;
    state_eflex_inst_t egr_inst;
    uint32_t bank_num;
    int num_range_checker_per_bank;
    ctr_eflex_control_t *ctrl = NULL;
    bcmcth_ctr_eflex_device_info_t *device_info;

    SHR_FUNC_ENTER(unit);

    /* Get flex control struct */
    SHR_IF_ERR_VERBOSE_EXIT
        (bcmcth_eflex_ctrl_get(unit, entry->comp, &ctrl));

    device_info = ctrl->ctr_eflex_device_info;
    num_range_checker_per_bank = device_info->num_range_checker_per_bank;

    /* Lookup entry with given key fields */
     SHR_IF_ERR_VERBOSE_EXIT
        (bcmcth_ctr_eflex_range_chk_lookup(unit, entry));

    /* Get flex state instance for current entry */
    inst = entry->inst;
    egr_inst = entry->egr_inst;

    /* Determine first and last pipe number */
    SHR_IF_ERR_VERBOSE_EXIT
        (bcmcth_ctr_eflex_num_pipes_get(unit,
                                        entry->ingress,
                                        entry->comp,
                                        entry->pipe_idx,
                                        &entry->start_pipe_idx,
                                        &entry->end_pipe_idx));

    /* Determine range checker bank number */
    entry->bank_num = (entry->obj_select * num_range_checker_per_bank) +
                      entry->range_chk_idx;

    /* Save bank number to set default values to correct range checker */
    bank_num = entry->bank_num;

    /* Clear range checker bit in resource list */
    SHR_IF_ERR_VERBOSE_EXIT
        (bcmcth_ctr_eflex_range_checker_set(unit, DISABLE, entry));

    /* Delete range check profile info LT entry */
    SHR_IF_ERR_VERBOSE_EXIT
        (bcmcth_ctr_eflex_range_chk_info_program(unit, false, entry));

    /* Get default values */
    SHR_IF_ERR_VERBOSE_EXIT
        (bcmcth_ctr_eflex_range_chk_default_get(unit, entry));

    /* Restore flex state instance, range checker bank number for entry */
    entry->inst = inst;
    entry->egr_inst = egr_inst;
    entry->bank_num = bank_num;

    /* Write to range check profile table */
    if (entry->comp == CTR_EFLEX) {
        SHR_IF_ERR_VERBOSE_EXIT
            (bcmcth_ctr_eflex_range_chk_write(unit, entry));
    } else if (entry->comp == STATE_EFLEX) {
        SHR_IF_ERR_VERBOSE_EXIT
            (bcmcth_state_eflex_range_chk_write(unit, entry));
    }

exit:
    SHR_FUNC_EXIT();
}

/* Function to update entry into RANGE_CHK_PROFILE IMM LT */
static int
bcmcth_ctr_eflex_range_chk_lt_entry_update(
    int unit,
    const bcmltd_field_t *key_fields,
    const bcmltd_field_t *data_fields,
    ctr_eflex_range_chk_data_t *entry,
    ctr_eflex_range_chk_data_t *cur_entry)
{
    bool bank_num_valid = false;
    int num_range_checker_per_bank;
    uint8_t pipe_unique;
    ctr_eflex_control_t *ctrl = NULL;
    bcmcth_ctr_eflex_device_info_t *device_info;

    SHR_FUNC_ENTER(unit);

    /* Get flex control struct */
    SHR_IF_ERR_VERBOSE_EXIT
        (bcmcth_eflex_ctrl_get(unit, entry->comp, &ctrl));

    device_info = ctrl->ctr_eflex_device_info;
    num_range_checker_per_bank = device_info->num_range_checker_per_bank;

    /* Get pipe operating mode */
    SHR_IF_ERR_VERBOSE_EXIT
        (bcmcth_ctr_eflex_pipe_opermode_get(unit,
                                            entry->ingress,
                                            entry->comp,
                                            &pipe_unique));

    /* Parse user values */
    SHR_IF_ERR_VERBOSE_EXIT
        (bcmcth_ctr_eflex_range_chk_lt_fields_parse(unit,
                                                    key_fields,
                                                    data_fields,
                                                    entry));

    cur_entry->range_chk_id = entry->range_chk_id;

    /* Lookup entry with given key fields */
     SHR_IF_ERR_VERBOSE_EXIT
        (bcmcth_ctr_eflex_range_chk_lookup(unit, cur_entry));

    /* Calculate range checker bank num for current entry */
    cur_entry->bank_num = (cur_entry->obj_select * num_range_checker_per_bank) +
                          cur_entry->range_chk_idx;

    /* Check if object, range checker or instance is being updated */
    if (entry->obj_select != CTR_EFLEX_INVALID_VAL ||
        entry->range_chk_idx != CTR_EFLEX_INVALID_VAL ||
        entry->egr_inst != CTR_EFLEX_INVALID_VAL ||
        entry->inst != CTR_EFLEX_INVALID_VAL) {
        bank_num_valid = true;
    }

    /* Fill missing fields in new entry with current entry */
    SHR_IF_ERR_VERBOSE_EXIT
        (bcmcth_ctr_eflex_range_chk_fill(unit, entry, cur_entry));

    /* Calculate range checker bank num for new entry */
    if (bank_num_valid) {
        entry->bank_num = (entry->obj_select * num_range_checker_per_bank) +
                          entry->range_chk_idx;
    }

    /*
     * If range checker bank number is valid and updated,
     * validate it on all applicable pipes.
     */
    if (entry->bank_num != cur_entry->bank_num && bank_num_valid) {
        SHR_IF_ERR_VERBOSE_EXIT
            (bcmcth_ctr_eflex_range_chk_validate(unit, entry));
    }

    /* Determine first and last pipe number for new entry */
    SHR_IF_ERR_VERBOSE_EXIT
        (bcmcth_ctr_eflex_num_pipes_get(unit,
                                        entry->ingress,
                                        entry->comp,
                                        entry->pipe_idx,
                                        &entry->start_pipe_idx,
                                        &entry->end_pipe_idx));

    /* Determine first and last pipe number for current entry */
    SHR_IF_ERR_VERBOSE_EXIT
        (bcmcth_ctr_eflex_num_pipes_get(unit,
                                        cur_entry->ingress,
                                        cur_entry->comp,
                                        cur_entry->pipe_idx,
                                        &cur_entry->start_pipe_idx,
                                        &cur_entry->end_pipe_idx));

    LOG_VERBOSE(BSL_LOG_MODULE,
        (BSL_META_U(unit,
            "Update %s Entry:\n"
            "range_chk_id %d, pipe_unique %d, pipe_idx %d, inst %d\n"
            "obj_select %d, range_checker %d, bank_num %d\n"
            "min %d, max %d egr_inst %d\n"),
            entry->ingress ? "Ingress" : "Egress",
            entry->range_chk_id,
            pipe_unique,
            entry->pipe_idx, entry->inst,
            entry->obj_select, entry->range_chk_idx, entry->bank_num,
            entry->min, entry->max, entry->egr_inst));

    /* Write to operand profile table */
    if (entry->comp == CTR_EFLEX) {
        SHR_IF_ERR_VERBOSE_EXIT
            (bcmcth_ctr_eflex_range_chk_write(unit, entry));
    } else if (entry->comp == STATE_EFLEX) {
        SHR_IF_ERR_VERBOSE_EXIT
            (bcmcth_state_eflex_range_chk_write(unit, entry));
    }

    /*
     * Update internal resource list and delete current entry from HW table
     * only if bank number or instance or pipe index in pipe unique
     * mode has changed
     */
    if ((entry->bank_num != cur_entry->bank_num && bank_num_valid) ||
        (entry->inst != cur_entry->inst) ||
        (pipe_unique && entry->pipe_idx != cur_entry->pipe_idx)) {
        /* Delete current entry from HW table */
        SHR_IF_ERR_VERBOSE_EXIT
            (bcmcth_ctr_eflex_range_chk_lt_entry_delete(unit, cur_entry));

        /* Insert new range check profile info LT entry */
        SHR_IF_ERR_VERBOSE_EXIT
            (bcmcth_ctr_eflex_range_chk_info_program(unit, true, entry));

        /* Set new range checker bit in resource list */
        SHR_IF_ERR_VERBOSE_EXIT
            (bcmcth_ctr_eflex_range_checker_set(unit, ENABLE, entry));
    }

exit:
    SHR_FUNC_EXIT();
}

/*! Function to insert/update/delete RANGE_CHK_PROFILE IMM LT entry. */
static int
bcmcth_ctr_eflex_range_chk_lt_stage(int unit,
                                    bool ingress,
                                    eflex_comp_t comp,
                                    uint64_t req_flags,
                                    bcmimm_entry_event_t event_reason,
                                    const bcmltd_field_t *key_fields,
                                    const bcmltd_field_t *data_fields,
                                    bcmltd_fields_t *output_fields)
{
    ctr_eflex_range_chk_data_t entry = {0};
    ctr_eflex_range_chk_data_t cur_entry = {0};

    SHR_FUNC_ENTER(unit);

    entry.ingress = ingress;
    entry.comp = comp;
    entry.req_flags = req_flags;

    if (output_fields) {
        output_fields->count = 0;
    }

    SHR_IF_ERR_VERBOSE_EXIT
        (bcmcth_ctr_eflex_range_chk_entry_init(unit, &entry));

    switch (event_reason) {
        case BCMIMM_ENTRY_INSERT:
            SHR_IF_ERR_VERBOSE_EXIT
                (bcmcth_ctr_eflex_range_chk_lt_entry_insert(unit,
                                                            key_fields,
                                                            data_fields,
                                                            &entry));
            break;

        case BCMIMM_ENTRY_UPDATE:
            cur_entry.ingress = ingress;
            cur_entry.comp = comp;
            cur_entry.req_flags = req_flags;

            SHR_IF_ERR_VERBOSE_EXIT
                (bcmcth_ctr_eflex_range_chk_entry_init(unit, &cur_entry));

            SHR_IF_ERR_VERBOSE_EXIT
                (bcmcth_ctr_eflex_range_chk_lt_entry_update(unit,
                                                            key_fields,
                                                            data_fields,
                                                            &entry,
                                                            &cur_entry));
            break;

        case BCMIMM_ENTRY_DELETE:
            /* Parse user values */
            SHR_IF_ERR_VERBOSE_EXIT
                (bcmcth_ctr_eflex_range_chk_lt_fields_parse(unit,
                                                            key_fields,
                                                            data_fields,
                                                            &entry));

            SHR_IF_ERR_VERBOSE_EXIT
                (bcmcth_ctr_eflex_range_chk_lt_entry_delete(unit, &entry));
            break;

        default:
            break;
    }

exit:
    SHR_FUNC_EXIT();

}

/*! Function to get RANGE_CHK_PROFILE IMM LT entry limit value. */
static int
bcmcth_ctr_eflex_range_chk_lt_ent_limit_get(int unit,
                                            bool ingress,
                                            eflex_comp_t comp,
                                            uint32_t *count)
{
    ctr_eflex_control_t *ctrl = NULL;
    bcmcth_ctr_eflex_device_info_t *device_info;
    uint8_t pipe_unique;
    uint32_t num_pipes, num_range_checker, num_instances = 1;
    SHR_FUNC_ENTER(unit);

    /* Get flex control struct */
    SHR_IF_ERR_VERBOSE_EXIT
        (bcmcth_eflex_ctrl_get(unit, comp, &ctrl));
    device_info = ctrl->ctr_eflex_device_info;

    /* Get pipe operating mode */
    SHR_IF_ERR_VERBOSE_EXIT
        (bcmcth_ctr_eflex_pipe_opermode_get(unit,
                                            ingress,
                                            comp,
                                            &pipe_unique));

    num_range_checker = device_info->num_range_checker;
    num_pipes = pipe_unique ? device_info->num_pipes : 1;

    if (comp == STATE_EFLEX && ingress == INGRESS) {
        num_instances = device_info->state_ext.num_ingress_instance;
    } else if (comp == STATE_EFLEX && ingress == EGRESS) {
        num_instances = device_info->state_ext.num_egress_instance;
    }

    *count = num_range_checker * num_pipes * num_instances;
    LOG_VERBOSE(BSL_LOG_MODULE,
        (BSL_META_U(unit,"comp %d ingress %d entry_limit %d\n"),
        comp, ingress, *count));

exit:
    SHR_FUNC_EXIT();
}

/*!
 * \brief Stage callback function of CTR_ING_EFLEX_RANGE_CHK_PROFILE to
 *        insert, update, delete IMM LT entry.
 *
 * \param [in] unit Unit number.
 * \param [in] sid  Logical table ID.
 * \param [in] op_arg The operation arguments.
 * \param [in] event_reason Reason for the entry event.
 * \param [in] key_fields Linked list of the key fields identifying
 *                        the entry.
 * \param [in] data_fields Linked list of the data fields in the
 *                         modified entry.
 * \param [in] context Pointer that was given during registration.
 *                     The callback can use this pointer to retrieve some
 *                     context.
 * \param [out] output_fields Linked list of the output fields in the
 *                            modified entry.
 * \return SHR_E_NONE on success and error code otherwise.
 */
static int
bcmcth_ctr_eflex_ing_range_chk_lt_stage_cb(int unit,
                                           bcmltd_sid_t sid,
                                           const bcmltd_op_arg_t *op_arg,
                                           bcmimm_entry_event_t event_reason,
                                           const bcmltd_field_t *key_fields,
                                           const bcmltd_field_t *data_fields,
                                           void *context,
                                           bcmltd_fields_t *output_fields)
{
    uint64_t req_flags;

    SHR_FUNC_ENTER(unit);
    SHR_NULL_CHECK(key_fields, SHR_E_PARAM);
    SHR_NULL_CHECK(op_arg, SHR_E_PARAM);
    if (event_reason != BCMIMM_ENTRY_DELETE) {
        SHR_NULL_CHECK(output_fields, SHR_E_PARAM);
    }

    req_flags = bcmptm_lt_entry_attrib_to_ptm_req_flags(op_arg->attrib);

    SHR_IF_ERR_VERBOSE_EXIT
        (bcmcth_ctr_eflex_range_chk_lt_stage(unit,
                                             INGRESS,
                                             CTR_EFLEX,
                                             req_flags,
                                             event_reason,
                                             key_fields,
                                             data_fields,
                                             output_fields));

exit:
    SHR_FUNC_EXIT();
}

/*!
 * \brief Entry limit get callback function of CTR_ING_EFLEX_RANGE_CHK_PROFILE
 *
 * \param [in] unit Unit number.
 * \param [in] sid  Logical table ID.
 * \param [in] count Entry limit value.
 * \return SHR_E_NONE on success and error code otherwise.
 */
static int
bcmcth_ctr_eflex_ing_range_chk_lt_ent_limit_get_cb(int unit,
                                                   uint32_t trans_id,
                                                   bcmltd_sid_t sid,
                                                   uint32_t *count)
{
    SHR_FUNC_ENTER(unit);
    SHR_NULL_CHECK(count, SHR_E_PARAM);

    SHR_IF_ERR_VERBOSE_EXIT
        (bcmcth_ctr_eflex_range_chk_lt_ent_limit_get(unit,
                                                     INGRESS,
                                                     CTR_EFLEX,
                                                     count));

exit:
    SHR_FUNC_EXIT();
}

/*!
 * \brief Stage callback function of CTR_EGR_EFLEX_RANGE_CHK_PROFILE to
 *        insert, update, delete IMM LT entry.
 *
 * \param [in] unit Unit number.
 * \param [in] sid  Logical table ID.
 * \param [in] op_arg The operation arguments.
 * \param [in] event_reason Reason for the entry event.
 * \param [in] key_fields Linked list of the key fields identifying
 *                        the entry.
 * \param [in] data_fields Linked list of the data fields in the
 *                         modified entry.
 * \param [in] context Pointer that was given during registration.
 *                     The callback can use this pointer to retrieve some
 *                     context.
 * \param [out] output_fields Linked list of the output fields in the
 *                            modified entry.
 * \return SHR_E_NONE on success and error code otherwise.
 */
static int
bcmcth_ctr_eflex_egr_range_chk_lt_stage_cb(int unit,
                                           bcmltd_sid_t sid,
                                           const bcmltd_op_arg_t *op_arg,
                                           bcmimm_entry_event_t event_reason,
                                           const bcmltd_field_t *key_fields,
                                           const bcmltd_field_t *data_fields,
                                           void *context,
                                           bcmltd_fields_t *output_fields)
{
    uint64_t req_flags;

    SHR_FUNC_ENTER(unit);
    SHR_NULL_CHECK(key_fields, SHR_E_PARAM);
    SHR_NULL_CHECK(op_arg, SHR_E_PARAM);
    if (event_reason != BCMIMM_ENTRY_DELETE) {
        SHR_NULL_CHECK(output_fields, SHR_E_PARAM);
    }

    req_flags = bcmptm_lt_entry_attrib_to_ptm_req_flags(op_arg->attrib);

    SHR_IF_ERR_VERBOSE_EXIT
        (bcmcth_ctr_eflex_range_chk_lt_stage(unit,
                                             EGRESS,
                                             CTR_EFLEX,
                                             req_flags,
                                             event_reason,
                                             key_fields,
                                             data_fields,
                                             output_fields));

exit:
    SHR_FUNC_EXIT();
}

/*!
 * \brief Entry limit get callback function of CTR_EGR_EFLEX_RANGE_CHK_PROFILE
 *
 * \param [in] unit Unit number.
 * \param [in] sid  Logical table ID.
 * \param [in] count Entry limit value.
 * \return SHR_E_NONE on success and error code otherwise.
 */
static int
bcmcth_ctr_eflex_egr_range_chk_lt_ent_limit_get_cb(int unit,
                                                   uint32_t trans_id,
                                                   bcmltd_sid_t sid,
                                                   uint32_t *count)
{
    SHR_FUNC_ENTER(unit);
    SHR_NULL_CHECK(count, SHR_E_PARAM);

    SHR_IF_ERR_VERBOSE_EXIT
        (bcmcth_ctr_eflex_range_chk_lt_ent_limit_get(unit,
                                                     EGRESS,
                                                     CTR_EFLEX,
                                                     count));

exit:
    SHR_FUNC_EXIT();
}

/*!
 * \brief Stage callback function of FLEX_STATE_ING_RANGE_CHK_PROFILE to
 *        insert, update, delete IMM LT entry.
 *
 * \param [in] unit Unit number.
 * \param [in] sid  Logical table ID.
 * \param [in] op_arg The operation arguments.
 * \param [in] event_reason Reason for the entry event.
 * \param [in] key_fields Linked list of the key fields identifying
 *                        the entry.
 * \param [in] data_fields Linked list of the data fields in the
 *                         modified entry.
 * \param [in] context Pointer that was given during registration.
 *                     The callback can use this pointer to retrieve some
 *                     context.
 * \param [out] output_fields Linked list of the output fields in the
 *                            modified entry.
 * \return SHR_E_NONE on success and error code otherwise.
 */
static int
bcmcth_state_eflex_ing_range_chk_lt_stage_cb(int unit,
                                             bcmltd_sid_t sid,
                                             const bcmltd_op_arg_t *op_arg,
                                             bcmimm_entry_event_t event_reason,
                                             const bcmltd_field_t *key_fields,
                                             const bcmltd_field_t *data_fields,
                                             void *context,
                                             bcmltd_fields_t *output_fields)
{
    uint64_t req_flags;

    SHR_FUNC_ENTER(unit);
    SHR_NULL_CHECK(key_fields, SHR_E_PARAM);
    SHR_NULL_CHECK(op_arg, SHR_E_PARAM);
    if (event_reason != BCMIMM_ENTRY_DELETE) {
        SHR_NULL_CHECK(output_fields, SHR_E_PARAM);
    }

    req_flags = bcmptm_lt_entry_attrib_to_ptm_req_flags(op_arg->attrib);

    SHR_IF_ERR_VERBOSE_EXIT
        (bcmcth_ctr_eflex_range_chk_lt_stage(unit,
                                             INGRESS,
                                             STATE_EFLEX,
                                             req_flags,
                                             event_reason,
                                             key_fields,
                                             data_fields,
                                             output_fields));

exit:
    SHR_FUNC_EXIT();
}

/*!
 * \brief Entry limit get callback function of FLEX_STATE_ING_RANGE_CHK_PROFILE
 *
 * \param [in] unit Unit number.
 * \param [in] sid  Logical table ID.
 * \param [in] count Entry limit value.
 * \return SHR_E_NONE on success and error code otherwise.
 */
static int
bcmcth_state_eflex_ing_range_chk_lt_ent_limit_get_cb(int unit,
                                                     uint32_t trans_id,
                                                     bcmltd_sid_t sid,
                                                     uint32_t *count)
{
    SHR_FUNC_ENTER(unit);
    SHR_NULL_CHECK(count, SHR_E_PARAM);

    SHR_IF_ERR_VERBOSE_EXIT
        (bcmcth_ctr_eflex_range_chk_lt_ent_limit_get(unit,
                                                     INGRESS,
                                                     STATE_EFLEX,
                                                      count));

exit:
    SHR_FUNC_EXIT();
}

/*!
 * \brief Stage callback function of FLEX_STATE_EGR_RANGE_CHK_PROFILE to
 *        insert, update, delete IMM LT entry.
 *
 * \param [in] unit Unit number.
 * \param [in] sid  Logical table ID.
 * \param [in] op_arg The operation arguments.
 * \param [in] event_reason Reason for the entry event.
 * \param [in] key_fields Linked list of the key fields identifying
 *                        the entry.
 * \param [in] data_fields Linked list of the data fields in the
 *                         modified entry.
 * \param [in] context Pointer that was given during registration.
 *                     The callback can use this pointer to retrieve some
 *                     context.
 * \param [out] output_fields Linked list of the output fields in the
 *                            modified entry.
 * \return SHR_E_NONE on success and error code otherwise.
 */
static int
bcmcth_state_eflex_egr_range_chk_lt_stage_cb(int unit,
                                             bcmltd_sid_t sid,
                                             const bcmltd_op_arg_t *op_arg,
                                             bcmimm_entry_event_t event_reason,
                                             const bcmltd_field_t *key_fields,
                                             const bcmltd_field_t *data_fields,
                                             void *context,
                                             bcmltd_fields_t *output_fields)
{
    uint64_t req_flags;

    SHR_FUNC_ENTER(unit);
    SHR_NULL_CHECK(key_fields, SHR_E_PARAM);
    SHR_NULL_CHECK(op_arg, SHR_E_PARAM);
    if (event_reason != BCMIMM_ENTRY_DELETE) {
        SHR_NULL_CHECK(output_fields, SHR_E_PARAM);
    }

    req_flags = bcmptm_lt_entry_attrib_to_ptm_req_flags(op_arg->attrib);

    SHR_IF_ERR_VERBOSE_EXIT
        (bcmcth_ctr_eflex_range_chk_lt_stage(unit,
                                             EGRESS,
                                             STATE_EFLEX,
                                             req_flags,
                                             event_reason,
                                             key_fields,
                                             data_fields,
                                             output_fields));

exit:
    SHR_FUNC_EXIT();
}

/*!
 * \brief Entry limit get callback function of FLEX_STATE_EGR_RANGE_CHK_PROFILE
 *
 * \param [in] unit Unit number.
 * \param [in] sid  Logical table ID.
 * \param [in] count Entry limit value.
 * \return SHR_E_NONE on success and error code otherwise.
 */
static int
bcmcth_state_eflex_egr_range_chk_lt_ent_limit_get_cb(int unit,
                                                     uint32_t trans_id,
                                                     bcmltd_sid_t sid,
                                                     uint32_t *count)
{
    SHR_FUNC_ENTER(unit);
    SHR_NULL_CHECK(count, SHR_E_PARAM);

    SHR_IF_ERR_VERBOSE_EXIT
        (bcmcth_ctr_eflex_range_chk_lt_ent_limit_get(unit,
                                                     EGRESS,
                                                     STATE_EFLEX,
                                                     count));

exit:
    SHR_FUNC_EXIT();
}

/**********************************************************************
* Public functions
 */

/*
 * Function to register callback handlers for
 * CTR_xxx_EFLEX_RANGE_CHK_PROFILE LTs
 */
int
bcmcth_ctr_eflex_imm_range_chk_register(int unit)
{
    bcmimm_lt_cb_t ctr_eflex_lt_cb;

    SHR_FUNC_ENTER(unit);

    /* Register CTR_ING_EFLEX_RANGE_CHK_PROFILE handler */
    sal_memset(&ctr_eflex_lt_cb, 0, sizeof(bcmimm_lt_cb_t));
    ctr_eflex_lt_cb.op_stage  = bcmcth_ctr_eflex_ing_range_chk_lt_stage_cb;
    ctr_eflex_lt_cb.ent_limit_get  =
        bcmcth_ctr_eflex_ing_range_chk_lt_ent_limit_get_cb;

    SHR_IF_ERR_VERBOSE_EXIT(
        bcmimm_lt_event_reg(unit,
                            CTR_ING_EFLEX_RANGE_CHK_PROFILEt,
                            &ctr_eflex_lt_cb,
                            NULL));

    /* Register CTR_EGR_EFLEX_RANGE_CHK_PROFILE handler */
    sal_memset(&ctr_eflex_lt_cb, 0, sizeof(bcmimm_lt_cb_t));
    ctr_eflex_lt_cb.op_stage  = bcmcth_ctr_eflex_egr_range_chk_lt_stage_cb;
    ctr_eflex_lt_cb.ent_limit_get  =
        bcmcth_ctr_eflex_egr_range_chk_lt_ent_limit_get_cb;

    SHR_IF_ERR_VERBOSE_EXIT(
        bcmimm_lt_event_reg(unit,
                            CTR_EGR_EFLEX_RANGE_CHK_PROFILEt,
                            &ctr_eflex_lt_cb,
                            NULL));

    /* Register CTR_ING_EFLEX_RANGE_CHK_PROFILE_INFO handler */
    sal_memset(&ctr_eflex_lt_cb, 0, sizeof(bcmimm_lt_cb_t));
    ctr_eflex_lt_cb.validate = bcmcth_ctr_eflex_read_only_lt_validate_cb;
    ctr_eflex_lt_cb.ent_limit_get  =
        bcmcth_ctr_eflex_ing_range_chk_lt_ent_limit_get_cb;

    SHR_IF_ERR_VERBOSE_EXIT(
        bcmimm_lt_event_reg(unit,
                            CTR_ING_EFLEX_RANGE_CHK_PROFILE_INFOt,
                            &ctr_eflex_lt_cb,
                            NULL));

    /* Register CTR_EGR_EFLEX_RANGE_CHK_PROFILE_INFO handler */
    sal_memset(&ctr_eflex_lt_cb, 0, sizeof(bcmimm_lt_cb_t));
    ctr_eflex_lt_cb.validate = bcmcth_ctr_eflex_read_only_lt_validate_cb;
    ctr_eflex_lt_cb.ent_limit_get  =
        bcmcth_ctr_eflex_egr_range_chk_lt_ent_limit_get_cb;

    SHR_IF_ERR_VERBOSE_EXIT(
        bcmimm_lt_event_reg(unit,
                            CTR_EGR_EFLEX_RANGE_CHK_PROFILE_INFOt,
                            &ctr_eflex_lt_cb,
                            NULL));

exit:
    SHR_FUNC_EXIT();
}

/*
 * Function to register callback handlers for
 * FLEX_STATE_xxx_RANGE_CHK_PROFILE LTs
 */
int
bcmcth_state_eflex_imm_range_chk_register(int unit)
{
    bcmimm_lt_cb_t state_eflex_lt_cb;

    SHR_FUNC_ENTER(unit);

    /* Register FLEX_STATE_ING_RANGE_CHK_PROFILE handler */
    sal_memset(&state_eflex_lt_cb, 0, sizeof(bcmimm_lt_cb_t));
    state_eflex_lt_cb.op_stage  = bcmcth_state_eflex_ing_range_chk_lt_stage_cb;
    state_eflex_lt_cb.ent_limit_get  =
        bcmcth_state_eflex_ing_range_chk_lt_ent_limit_get_cb;

    SHR_IF_ERR_VERBOSE_EXIT(
        bcmimm_lt_event_reg(unit,
                            FLEX_STATE_ING_RANGE_CHK_PROFILEt,
                            &state_eflex_lt_cb,
                            NULL));

    /* Register FLEX_STATE_EGR_RANGE_CHK_PROFILE handler */
    sal_memset(&state_eflex_lt_cb, 0, sizeof(bcmimm_lt_cb_t));
    state_eflex_lt_cb.op_stage  = bcmcth_state_eflex_egr_range_chk_lt_stage_cb;
    state_eflex_lt_cb.ent_limit_get  =
        bcmcth_state_eflex_egr_range_chk_lt_ent_limit_get_cb;

    SHR_IF_ERR_VERBOSE_EXIT(
        bcmimm_lt_event_reg(unit,
                            FLEX_STATE_EGR_RANGE_CHK_PROFILEt,
                            &state_eflex_lt_cb,
                            NULL));

    /* Register FLEX_STATE_ING_RANGE_CHK_PROFILE_INFO handler */
    sal_memset(&state_eflex_lt_cb, 0, sizeof(bcmimm_lt_cb_t));
    state_eflex_lt_cb.validate = bcmcth_ctr_eflex_read_only_lt_validate_cb;
    state_eflex_lt_cb.ent_limit_get  =
        bcmcth_state_eflex_ing_range_chk_lt_ent_limit_get_cb;

    SHR_IF_ERR_VERBOSE_EXIT(
        bcmimm_lt_event_reg(unit,
                            FLEX_STATE_ING_RANGE_CHK_PROFILE_INFOt,
                            &state_eflex_lt_cb,
                            NULL));

    /* Register FLEX_STATE_EGR_RANGE_CHK_PROFILE_INFO handler */
    sal_memset(&state_eflex_lt_cb, 0, sizeof(bcmimm_lt_cb_t));
    state_eflex_lt_cb.validate = bcmcth_ctr_eflex_read_only_lt_validate_cb;
    state_eflex_lt_cb.ent_limit_get  =
        bcmcth_state_eflex_egr_range_chk_lt_ent_limit_get_cb;

    SHR_IF_ERR_VERBOSE_EXIT(
        bcmimm_lt_event_reg(unit,
                            FLEX_STATE_EGR_RANGE_CHK_PROFILE_INFOt,
                            &state_eflex_lt_cb,
                            NULL));

exit:
    SHR_FUNC_EXIT();
}
