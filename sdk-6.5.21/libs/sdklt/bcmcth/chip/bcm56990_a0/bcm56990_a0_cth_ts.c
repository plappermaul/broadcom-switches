/*! \file bcm56990_a0_cth_ts.c
 *
 * Chip stub for BCMCTH timesync control.
 */
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

#include <bcmcth/bcmcth_ts_drv.h>

#include <shr/shr_debug.h>
#include <bcmcth/bcmcth_ts_drv.h>
#include <bcmcth/bcmcth_util.h>
#include <bcmdrd/chip/bcm56990_a0_enum.h>
#include <bcmdrd/chip/bcm56990_a0_defs.h>
#include <bcmdrd/bcmdrd_pt.h>
#include <bcmpc/bcmpc_lport.h>
#include <bcmptm/bcmptm.h>

/*******************************************************************************
 * Defines
 */
#define BSL_LOG_MODULE BSL_LS_BCMCTH_TIMESYNC

/*******************************************************************************
 * Private Functions
 */
static void
bcm56990_a0_cth_pt_op_info_t_init(bcmcth_pt_op_info_t *pt_op_info)
{
    if (pt_op_info) {
        pt_op_info->drd_sid = BCM56990_A0_ENUM_COUNT;
        pt_op_info->trans_id = -1;
        pt_op_info->req_lt_sid = BCM56990_A0_ENUM_COUNT;
        pt_op_info->rsp_lt_sid = BCM56990_A0_ENUM_COUNT;
        pt_op_info->req_flags = BCMPTM_REQ_FLAGS_NO_FLAGS;
        pt_op_info->rsp_flags = BCMPTM_REQ_FLAGS_NO_FLAGS;
        pt_op_info->op = BCMPTM_OP_NOP;
        pt_op_info->dyn_info.index = -1;
        pt_op_info->dyn_info.tbl_inst = -1;
        pt_op_info->wsize = 0;
    }
    return;
}

static int
bcm56990_a0_cth_synce_ctrl_validate(int unit,
                                    ts_synce_ctrl_t *ctrl)
{
    int lport, pport;

    lport = ctrl->port;
    pport = bcmpc_lport_to_pport(unit, lport);
    if (pport == BCMPC_INVALID_PPORT) {
        ctrl->pm_port = SYNCE_INVALID_PM_PORT;
    } else {
        /*
         * [2:0] lanes per PM8x50 = (physical port -1) % 4 * 2
         * [5:3] PM8x50 per pm_grp = [(physical port -1) / 4] % 8
         * [8:6] pm_grp = (physical port -1) / 32
         */
        pport = pport -1;
        ctrl->pm_port = (pport % 4) * 2 |
                        ((pport / 4) % 8) << 3 |
                        (pport / 32) << 6;
    }
    return SHR_E_NONE;
}

static int
bcm56990_a0_cth_synce_ctrl_update(int unit,
                                  bcmltd_sid_t ltid,
                                  const bcmltd_op_arg_t *op_arg,
                                  int ctrl_num,
                                  ts_synce_ctrl_t *ctrl,
                                  bool override)
{
    bcmcth_pt_op_info_t pt_op_info;
    uint32_t *buf_ptr = NULL;
    TOP_SYNCE_CTRLr_t buf;
    ts_synce_ctrl_t *control;
    int pm_port, i;

    SHR_FUNC_ENTER(unit);
    sal_memset(&buf, 0, sizeof(buf));
    buf_ptr = (uint32_t *)&buf;

    bcm56990_a0_cth_pt_op_info_t_init(&pt_op_info);
    pt_op_info.drd_sid = TOP_SYNCE_CTRLr;
    pt_op_info.trans_id = op_arg->trans_id;
    pt_op_info.req_lt_sid = ltid;
    pt_op_info.req_flags =
        bcmptm_lt_entry_attrib_to_ptm_req_flags(op_arg->attrib);
    pt_op_info.op = BCMPTM_OP_READ;
    pt_op_info.wsize = bcmdrd_pt_entry_wsize(unit, TOP_SYNCE_CTRLr);
    pt_op_info.dyn_info.index = 0;
    pt_op_info.dyn_info.tbl_inst = 0;
    SHR_IF_ERR_EXIT
       (bcmcth_pt_read(unit, &pt_op_info, buf_ptr));

    for (i = 0; i < ctrl_num; i++) {
        control = &ctrl[i];
        if (!control->update) {
            continue;
        }
        if (control->pm_port == SYNCE_INVALID_PM_PORT) {
            pm_port = 0;
        } else {
            pm_port = control->pm_port;
        }
        if (control->instance == 0) {
            TOP_SYNCE_CTRLr_L1_RCVD_SW_OVWR_VALIDf_SET(buf,
                                                       control->ovr_valid);
            TOP_SYNCE_CTRLr_L1_RCVD_FREQ_SELf_SET(buf, control->clk_divisor);
            TOP_SYNCE_CTRLr_L1_RCVD_PORT_SELf_SET(buf, pm_port);
        }
        if (control->instance == 1) {
            TOP_SYNCE_CTRLr_L1_RCVD_SW_OVWR_BKUP_VALIDf_SET(buf,
                                                            control->ovr_valid);
            TOP_SYNCE_CTRLr_L1_RCVD_BKUP_FREQ_SELf_SET(buf,
                                                       control->clk_divisor);
            TOP_SYNCE_CTRLr_L1_RCVD_BKUP_PORT_SELf_SET(buf, pm_port);
        }
    }

    TOP_SYNCE_CTRLr_L1_RCVD_SW_OVWR_ENf_SET(buf, override);

    bcm56990_a0_cth_pt_op_info_t_init(&pt_op_info);
    pt_op_info.drd_sid = TOP_SYNCE_CTRLr;
    pt_op_info.trans_id = op_arg->trans_id;
    pt_op_info.req_lt_sid = ltid;
    pt_op_info.req_flags =
        bcmptm_lt_entry_attrib_to_ptm_req_flags(op_arg->attrib);
    pt_op_info.op = BCMPTM_OP_WRITE;
    pt_op_info.wsize = bcmdrd_pt_entry_wsize(unit, TOP_SYNCE_CTRLr);
    pt_op_info.dyn_info.index = 0;
    pt_op_info.dyn_info.tbl_inst = 0;

    LOG_VERBOSE(BSL_LOG_MODULE,
                (BSL_META_U(unit,"PT write 0x%x\n"),
                 TOP_SYNCE_CTRLr_GET(buf)));
    SHR_IF_ERR_EXIT
        (bcmcth_pt_write(unit, &pt_op_info, buf_ptr));
exit:
    SHR_FUNC_EXIT();
}

static int
bcm56990_a0_cth_synce_ctrl_enable(int unit,
                                  bcmltd_sid_t ltid,
                                  const bcmltd_op_arg_t *op_arg,
                                  uint8_t inst_map,
                                  bool enable)
{
    bcmcth_pt_op_info_t pt_op_info;
    uint32_t *buf_ptr = NULL;
    TOP_SYNCE_CTRLr_t buf;
    int i;

    SHR_FUNC_ENTER(unit);
    sal_memset(&buf, 0, sizeof(buf));
    buf_ptr = (uint32_t *)&buf;

    bcm56990_a0_cth_pt_op_info_t_init(&pt_op_info);
    pt_op_info.drd_sid = TOP_SYNCE_CTRLr;
    pt_op_info.trans_id = op_arg->trans_id;
    pt_op_info.req_lt_sid = ltid;
    pt_op_info.req_flags =
        bcmptm_lt_entry_attrib_to_ptm_req_flags(op_arg->attrib);
    pt_op_info.op = BCMPTM_OP_READ;
    pt_op_info.wsize = bcmdrd_pt_entry_wsize(unit, TOP_SYNCE_CTRLr);
    pt_op_info.dyn_info.index = 0;
    pt_op_info.dyn_info.tbl_inst = 0;
    SHR_IF_ERR_EXIT
       (bcmcth_pt_read(unit, &pt_op_info, buf_ptr));

    for (i = 0; i < SYNCE_MAX_CLK_TYPE; i++) {
        if (inst_map & 1 << i) {
            if (i == 0) {
                TOP_SYNCE_CTRLr_L1_RCVD_CLK_RSTNf_SET(buf, enable);
            }
            if (i == 1) {
                TOP_SYNCE_CTRLr_L1_RCVD_CLK_BKUP_RSTNf_SET(buf, enable);
            }
        }
    }

    bcm56990_a0_cth_pt_op_info_t_init(&pt_op_info);
    pt_op_info.drd_sid = TOP_SYNCE_CTRLr;
    pt_op_info.trans_id = op_arg->trans_id;
    pt_op_info.req_lt_sid = ltid;
    pt_op_info.req_flags =
        bcmptm_lt_entry_attrib_to_ptm_req_flags(op_arg->attrib);
    pt_op_info.op = BCMPTM_OP_WRITE;
    pt_op_info.wsize = bcmdrd_pt_entry_wsize(unit, TOP_SYNCE_CTRLr);
    pt_op_info.dyn_info.index = 0;
    pt_op_info.dyn_info.tbl_inst = 0;

    LOG_VERBOSE(BSL_LOG_MODULE,
                (BSL_META_U(unit,"PT write 0x%x\n"),
                 TOP_SYNCE_CTRLr_GET(buf)));
    SHR_IF_ERR_EXIT
        (bcmcth_pt_write(unit, &pt_op_info, buf_ptr));
exit:
    SHR_FUNC_EXIT();
}


static bcmcth_ts_synce_drv_t bcm56990_a0_cth_synce_drv = {
    .synce_ctrl_enable = &bcm56990_a0_cth_synce_ctrl_enable,
    .synce_ctrl_update = &bcm56990_a0_cth_synce_ctrl_update,
    .synce_ctrl_validate = &bcm56990_a0_cth_synce_ctrl_validate
};

static bcmcth_ts_drv_t bcm56990_a0_cth_ts_drv = {
    .synce_drv = &bcm56990_a0_cth_synce_drv
};
/*******************************************************************************
 * Public functions
 */
bcmcth_ts_drv_t *
bcm56990_a0_cth_ts_drv_get(int unit)
{
    return &bcm56990_a0_cth_ts_drv;
}
