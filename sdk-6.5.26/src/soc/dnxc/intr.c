
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2022 Broadcom Inc. All rights reserved.
 */

#ifdef BSL_LOG_MODULE
#error "BSL_LOG_MODULE redefined"
#endif
#define BSL_LOG_MODULE BSL_LS_SOC_INIT

#include <sal/core/dpc.h>
#include <soc/cm.h>
#include <shared/bsl.h>
#include <soc/intr.h>
#include <soc/ipoll.h>
#include <soc/iproc.h>
#include <soc/drv.h>
#include <soc/intr_cmicx.h>
#include <soc/sand/shrextend/shrextend_debug.h>
#include <soc/dnxc/intr.h>
#include <soc/dnxc/dnxc_intr_handler.h>
#ifdef BCM_DNX_SUPPORT
#include <soc/dnx/dnx_data/auto_generated/dnx_data_intr.h>
#endif
#ifdef BCM_DNXF_SUPPORT
#include <soc/dnxf/dnxf_data/auto_generated/dnxf_data_intr.h>
#endif
#ifdef BCM_CMICX_GEN2_SUPPORT
#include <soc/access/intr_cmicx_gen2.h>
#endif
#ifdef BCM_ACCESS_SUPPORT
#include <soc/access/cmicx_gen2.h>
#include <soc/access/access_intr.h>
#include <soc/dnxc/dnxc_access_intr.h>
#endif

int
soc_dnxc_access_available(
    int unit)
{
    if (SOC_IS_NEW_ACCESS_INITIALIZED(unit))
    {
#ifdef BCM_DNX_SUPPORT
        if (SOC_IS_DNX(unit) && !dnx_data_intr.general.feature_get(unit, dnx_data_intr_general_new_access_support))
        {
            return 0;
        }
#endif
#ifdef BCM_DNXF_SUPPORT
        if (SOC_IS_DNXF(unit) && !dnxf_data_intr.general.feature_get(unit, dnxf_data_intr_general_new_access_support))
        {
            return 0;
        }
#endif
        return 1;
    }

    return 0;
}

int
soc_dnxc_intr_init(
    int unit)
{
    int irq_polled = 1;
    SHR_FUNC_INIT_VARS(unit);

    if (soc_cmic_intr_init(unit) < 0)
    {
        SHR_ERR_EXIT(_SHR_E_INTERNAL, "error initializing cmic interrupt");
    }

    soc_cmic_intr_all_disable(unit);

    if (sal_dpc_enable(INT_TO_PTR(unit)) < 0)
    {
        SHR_ERR_EXIT(_SHR_E_INTERNAL, "error enable dpc");
    }
#ifdef BCM_DNX_SUPPORT
    if (SOC_IS_DNX(unit))
    {
        irq_polled = dnx_data_intr.general.polled_irq_mode_get(unit);
    }
    else
#endif
#ifdef BCM_DNXF_SUPPORT
    if (SOC_IS_DNXF(unit))
    {
        irq_polled = dnxf_data_intr.general.polled_irq_mode_get(unit);
    }
    else
#endif
    {
        SHR_ERR_EXIT(_SHR_E_INTERNAL, "Should be dnx or dnxf device");
    }

#ifdef BCM_CMICX_GEN2_SUPPORT
    if (soc_feature(unit, soc_feature_cmicx_gen2))
    {

        SHR_IF_ERR_EXIT(access_cmicx_gen2_intr_connect(unit, irq_polled));
        SHR_EXIT();
    }
#endif

    if (irq_polled)
    {
        if (soc_ipoll_connect(unit, soc_cmicx_intr, INT_TO_PTR(unit)) < 0)
        {
            SHR_ERR_EXIT(_SHR_E_INTERNAL, "error initializing polled interrupt mode");
        }
        SOC_FLAGS_SET(unit, SOC_FLAGS_GET(unit) | SOC_F_POLLED);
#ifdef SEPARATE_PKTDMA_INTR_HANDLER
        if (soc_ipoll_pktdma_connect(unit, soc_cmicx_pktdma_intr, INT_TO_PTR(unit)) < 0)
        {
            SHR_ERR_EXIT(_SHR_E_INTERNAL, "error initializing polled interrupt mode");
        }
#endif
    }
    else
    {
#ifdef SEPARATE_PKTDMA_INTR_HANDLER
        SHR_ERR_EXIT(_SHR_E_INTERNAL, "error separate pktdma thread cann't be created under interrupt mode");
#endif
        if (soc_cm_interrupt_connect(unit, soc_cmicx_intr, INT_TO_PTR(unit)) < 0)
        {
            SHR_ERR_EXIT(_SHR_E_INTERNAL, "could not connect interrupt line");
        }
    }

exit:
    SHR_FUNC_EXIT;
}

int
soc_dnxc_intr_deinit(
    int unit)
{
    SHR_FUNC_INIT_VARS(unit);

    soc_cmic_intr_all_disable(unit);

#ifdef BCM_CMICX_GEN2_SUPPORT
    if (soc_feature(unit, soc_feature_cmicx_gen2))
    {

        int irq_polled = SOC_FLAGS_GET(unit) & SOC_F_POLLED;

        if (access_cmicx_gen2_intr_disconnect(unit, irq_polled) < 0)
        {
            LOG_ERROR(BSL_LS_SOC_INIT, (BSL_META_U(unit, "could not disconnect interrupt\n")));
        }
    }
    else
#endif
    if (SOC_FLAGS_GET(unit) & SOC_F_POLLED)
    {
        if (soc_ipoll_disconnect(unit) < 0)
        {
            LOG_ERROR(BSL_LS_SOC_INIT, (BSL_META_U(unit, "could not disconnect interrupt polling\n")));
        }
#ifdef SEPARATE_PKTDMA_INTR_HANDLER
        if (soc_ipoll_pktdma_disconnect(unit) < 0)
        {
            LOG_ERROR(BSL_LS_SOC_INIT, (BSL_META_U(unit, "could not disconnect Packet DMA interrupt polling\n")));
        }
#endif
        SOC_FLAGS_SET(unit, SOC_FLAGS_GET(unit) & ~SOC_F_POLLED);
    }
    else
    {
        if (soc_cm_interrupt_disconnect(unit) < 0)
        {
            LOG_ERROR(BSL_LS_SOC_INIT, (BSL_META_U(unit, "could not disconnect interrupt line\n")));
        }
    }

    sal_dpc_disable_and_wait(INT_TO_PTR(unit));

    SHR_FUNC_EXIT;
}

void
soc_dnxc_ser_intr_handler(
    void *unit_vp,
    void *d1,
    void *d2,
    void *d3,
    void *d4)
{
    int rc = SOC_E_NONE, i;
    int unit = PTR_TO_INT(unit_vp);
    int flags = 0;
    int max_interrupts_size = PTR_TO_INT(d1);
    soc_interrupt_cause_t *interrupts = SOC_CONTROL(unit)->interrupt_event_control.interrupts;
    int interrupt_num = 0;
    int interrupt_bit;
#ifdef PLISIM
    soc_reg_t ser_status_reg[4] = { ICFG_CHIP_LP_INTR_RAW_STATUS_REG0r,
        ICFG_CHIP_LP_INTR_RAW_STATUS_REG1r,
        ICFG_CHIP_LP_INTR_RAW_STATUS_REG2r,
        ICFG_CHIP_LP_INTR_RAW_STATUS_REG3r
    };
#endif

    sal_memset(interrupts, 0x0, max_interrupts_size * sizeof(soc_interrupt_cause_t));

    flags = SOC_ACTIVE_INTERRUPTS_GET_UNMASKED_ONLY;
#ifdef BCM_ACCESS_SUPPORT
    if (soc_dnxc_access_available(unit))
    {
        rc = access_active_interrupts_get(unit, flags, max_interrupts_size, interrupts, &interrupt_num);
    }
    else
#endif
        rc = soc_active_interrupts_get(unit, flags, max_interrupts_size, interrupts, &interrupt_num);
    if (SOC_FAILURE(rc))
    {
        LOG_ERROR(BSL_LS_SOC_INTR, (BSL_META_U(unit, "%s\n"), soc_errmsg(rc)));
        return;
    }

    LOG_VERBOSE(BSL_LS_SOC_INTR,
                (BSL_META_U(unit, "interrupt_num=%d, max_interrupts_size=%d\n"), interrupt_num, max_interrupts_size));

    if (interrupt_num > 1)
    {
        rc = soc_sort_interrupts_according_to_priority(unit, interrupts, interrupt_num);
        if (SOC_FAILURE(rc))
        {
            LOG_ERROR(BSL_LS_SOC_INTR, (BSL_META_U(unit, "%s\n"), soc_errmsg(rc)));
            return;
        }
    }

    for (i = 0; i < interrupt_num; i++)
    {
        uint32 interrupt_flags;

        dnxc_intr_switch_event_cb(unit, SOC_SWITCH_EVENT_DEVICE_INTERRUPT, interrupts[i].id, interrupts[i].index, 0, 0);
        rc = soc_interrupt_flags_get(unit, interrupts[i].id, &interrupt_flags);
        if (SOC_FAILURE(rc))
        {
            LOG_ERROR(BSL_LS_SOC_INTR, (BSL_META_U(unit, "%s\n"), soc_errmsg(rc)));
            return;
        }
        if (((interrupt_flags & SOC_INTERRUPT_DB_FLAGS_BCM_AND_USR_CB) == 0)
            && (SHR_BITGET(&interrupt_flags, SOC_INTERRUPT_DB_FLAGS_CORR_ACT_OVERRIDE_ENABLE) == 0))
        {
            continue;
        }
        soc_event_generate(unit, SOC_SWITCH_EVENT_DEVICE_INTERRUPT, interrupts[i].id, interrupts[i].index, 0);
    }
#ifdef BCM_CMICX_GEN2_SUPPORT
    if (soc_feature(unit, soc_feature_iproc_20))
    {
        interrupt_bit = IPROC_IRQ_CHIP_INTR;
    }
    else
#endif
        interrupt_bit =
            soc_feature(unit, soc_feature_iproc_17) ? IPROC17_CHIP_INTR_LOW_PRIORITY : CHIP_INTR_LOW_PRIORITY;

    soc_cmic_intr_enable(unit, interrupt_bit);

#ifdef PLISIM

#ifdef BCM_ACCESS_SUPPORT
    if (soc_dnxc_access_available(unit) && soc_feature(unit, soc_feature_iproc_20))
    {
        int ii;
        uint32 addr = ICFG_CHIP_INTR_ENABLE_OFFSET_START;

        for (ii = 0; ii < 8; ii++)
        {
            soc_cm_iproc_write(unit, addr + ii * 4, 0);
        }
    }
    else
#endif

    {
        for (i = 0; i < 4; i++)
        {
            (void) soc_pci_write(unit, soc_reg_addr(unit, ser_status_reg[i], REG_PORT_ANY, 0), 0);
        }
    }
#endif

}

#undef BSL_LOG_MODULE
