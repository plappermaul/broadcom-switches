/*
 * $Id: reg.c,v 1.20 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * File:        reg.c
 * Purpose:     Diag shell register commands for DPP
 */

#include <shared/bsl.h>

#include <appl/diag/system.h>
#ifdef BCM_PETRA_SUPPORT
#include <soc/dpp/drv.h>
#include <soc/dpp/port_sw_db.h>
#include <appl/dpp/regs_filter.h>
#endif
#ifdef BCM_IPROC_SUPPORT
#include <soc/iproc.h>
#endif

#include <appl/diag/sand/diag_sand_utils.h>
#include <appl/diag/sand/diag_sand_reg.h>

#define PFLAG_ALIAS      0x01
#define PFLAG_SUMMARY    0x02

/*
 * Register Types - for getreg and dump commands
 */
static regtype_entry_t dpp_regtypes[] = {
    {"SOC", soc_genreg,
     "SOC registers"},
    {"CMIC_SBUS_RING_MAP_0_7", soc_cpureg,
     "CMIC_SBUS_RING_MAP_0_7 registers"},
    {"CMIC_SBUS_RING_MAP_8_15", soc_cpureg,
     "CMIC_SBUS_RING_MAP_8_15 registers"},
    {"CMIC_SBUS_RING_MAP_16_23", soc_cpureg,
     "CMIC_SBUS_RING_MAP_16_23 registers"},
    {"CMIC_SBUS_RING_MAP_24_31", soc_cpureg,
     "CMIC_SBUS_RING_MAP_24_31 registers"},
    {"CMIC_SBUS_RING_MAP_32_39", soc_cpureg,
     "CMIC_SBUS_RING_MAP_32_39 registers"},
    {"CMIC_SBUS_RING_MAP_40_47", soc_cpureg,
     "CMIC_SBUS_RING_MAP_40_47 registers"},
    {"CMIC_SBUS_RING_MAP_48_55", soc_cpureg,
     "CMIC_SBUS_RING_MAP_48_55 registers"},
    {"CMIC_SBUS_RING_MAP_56_63", soc_cpureg,
     "CMIC_SBUS_RING_MAP_56_63 registers"},
};

#define PRINT_COUNT(str, len, wrap, prefix) \
    if ((wrap > 0) && (len > wrap)) { \
        bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit,"\n%s", prefix); \
        len = sal_strlen(prefix); \
    } \
    bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, "%s", str); \
    len += strlen(str)

/*
 * Given an address, fill in regaddrinfo structure
 * This can only be called for SOC registers, not for CPU registers
 */
STATIC void
diag_sand_reg_addrinfo_get(
    int unit,
    soc_regaddrinfo_t * ainfo,
    uint32 addr,
    uint32 cmic_blk)
{
    int reg;
    uint32 lkup_addr;
    int idx = 0, schan_blk;

    if (ainfo == NULL)
    {
        return;
    }

    ainfo->valid = 0;
    ainfo->reg = INVALIDr;

    ainfo->block = SOC_BLK_NONE;
    lkup_addr = addr;
    schan_blk = cmic_blk;

    while (SOC_BLOCK_INFO(unit, idx).type != -1)
    {
        if (SOC_BLOCK_INFO(unit, idx).schan == schan_blk)
        {
            ainfo->block = SOC_BLOCK_INFO(unit, idx).type;
            ainfo->idx = SOC_BLOCK_INFO(unit, idx).number;
            break;
        }
        idx++;
    }

    if (SOC_BLK_NONE == ainfo->block)
    {
        ainfo->valid = 0;
        ainfo->reg = INVALIDr;
        return;
    }
    for (reg = 0; reg < NUM_SOC_REG; reg++)
    {
        if (!SOC_REG_IS_VALID(unit, reg))
        {
            continue;
        }
        if (lkup_addr == (SOC_REG_INFO(unit, reg).offset & (~0x80000)))
        {
            soc_block_types_t regblktype = SOC_REG_INFO(unit, reg).block;

            if (SOC_BLOCK_IN_LIST(regblktype, ainfo->block))
            {
                ainfo->valid = 1;
                ainfo->reg = reg;
                ainfo->field = INVALIDf;
                ainfo->cos = -1;
                ainfo->port = -1;
                break;
            }
        }
    }
}

/*
 * Lists registers containing a specific pattern
 *
 * If use_reset is true, ignores val and uses reset default value.
 */
STATIC void
diag_sand_reg_do_list(
    int unit,
    soc_regaddrinfo_t * ainfo,
    int use_reset,
    soc_reg_above_64_val_t regval)
{
    soc_reg_t reg = ainfo->reg;
    soc_reg_info_t *reginfo = &SOC_REG_INFO(unit, reg);
    soc_field_info_t *fld;
    int f;
    uint32 flags;
    soc_reg_above_64_val_t mask, rval, rmsk;
    char buf[256];
    char rval_str[256], rmsk_str[256], dval_str[256];
    int i, copies, disabled;
    soc_reg_above_64_val_t fldval;

    if (!SOC_REG_IS_VALID(unit, reg))
    {
        cli_out("Register %s is not valid for chip %s\n", SOC_REG_NAME(unit, reg), SOC_UNIT_NAME(unit));
        return;
    }
    flags = reginfo->flags;

    SOC_REG_ABOVE_64_ALLONES(mask);

    SOC_REG_ABOVE_64_RST_VAL_GET(unit, reg, rval);
    SOC_REG_ABOVE_64_RST_MSK_GET(unit, reg, rmsk);
    if (use_reset)
    {
        SOC_REG_ABOVE_64_COPY(regval, rval);
        SOC_REG_ABOVE_64_COPY(mask, rmsk);
    }

    soc_reg_sprint_addr(unit, buf, ainfo);

    bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, "Register: %s", buf);
#if !defined(SOC_NO_ALIAS)
    if (soc_reg_alias[reg] && *soc_reg_alias[reg])
    {
        bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, " alias %s", soc_reg_alias[reg]);
    }
#endif /* !defined(SOC_NO_ALIAS) */
    bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, " %s register",
                soc_regtypenames[reginfo->regtype]);
    bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, " address 0x%08x\n", ainfo->addr);

    bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, "Flags:");
    if (flags & SOC_REG_FLAG_ABOVE_64_BITS)
    {
        bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, " above 64-bits");
    }
    else if (flags & SOC_REG_FLAG_64_BITS)
    {
        bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, " 64-bits");
    }
    else
    {
        bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, " 32-bits");
    }
    if (flags & SOC_REG_FLAG_COUNTER)
    {
        bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, " counter");
    }
    if (flags & SOC_REG_FLAG_ARRAY)
    {
        bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, " array[%d-%d]", 0, reginfo->numels - 1);
    }
    if (flags & SOC_REG_FLAG_NO_DGNL)
    {
        bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, " no-diagonals");
    }
    if (flags & SOC_REG_FLAG_RO)
    {
        bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, " read-only");
    }
    if (flags & SOC_REG_FLAG_WO)
    {
        bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, " write-only");
    }
    if (flags & SOC_REG_FLAG_ED_CNTR)
    {
        bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, " error/discard-counter");
    }
    if (flags & SOC_REG_FLAG_SPECIAL)
    {
        bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, " special");
    }
    if (flags & SOC_REG_FLAG_EMULATION)
    {
        bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, " emulation");
    }
    if (flags & SOC_REG_FLAG_VARIANT1)
    {
        bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, " variant1");
    }
    if (flags & SOC_REG_FLAG_VARIANT2)
    {
        bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, " variant2");
    }
    if (flags & SOC_REG_FLAG_VARIANT3)
    {
        bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, " variant3");
    }
    if (flags & SOC_REG_FLAG_VARIANT4)
    {
        bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, " variant4");
    }
    bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, "\n");

    bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, "Blocks:");
    copies = disabled = 0;
    for (i = 0; SOC_BLOCK_INFO(unit, i).type >= 0; i++)
    {
        /*
         * if (SOC_BLOCK_INFO(unit, i).type & reginfo->block) {
         */
        if (SOC_BLOCK_IS_TYPE(unit, i, reginfo->block))
        {
            if (SOC_INFO(unit).block_valid[i])
            {
                bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, " %s", SOC_BLOCK_NAME(unit, i));
            }
            else
            {
                bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, " [%s]", SOC_BLOCK_NAME(unit, i));
                disabled += 1;
            }
            copies += 1;
        }
    }
    bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, " (%d cop%s", copies,
                copies == 1 ? "y" : "ies");
    if (disabled)
    {
        bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, ", %d disabled", disabled);
    }
    bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, ")\n");

#if !defined(SOC_NO_DESC)
    if (soc_reg_desc[reg] && *soc_reg_desc[reg])
    {
        bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, "Description: %s\n", soc_reg_desc[reg]);
    }
#endif /* !defined(SOC_NO_ALIAS) */
    bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, "Displaying:");
    if (use_reset)
    {
        bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, " reset defaults");
    }
    else
    {
        if (SOC_REG_IS_ABOVE_64(unit, reg))
        {
            format_long_integer(dval_str, regval, SOC_REG_ABOVE_64_INFO(unit, reg).size);
            bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, " value %s", dval_str);
        }
        else
        {
            format_long_integer(dval_str, regval, 2);
            bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, " value %s", dval_str);
        }
    }

    if (SOC_REG_IS_ABOVE_64(unit, reg))
    {
        format_long_integer(rval_str, rval, SOC_REG_ABOVE_64_INFO(unit, reg).size);
        bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, "\nreset value %s", rval_str);
        format_long_integer(rmsk_str, rmsk, SOC_REG_ABOVE_64_INFO(unit, reg).size);
        bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, "\nmask %s\n", rmsk_str);

    }
    else
    {

        uint64 rval64, rmsk64;
        SOC_REG_RST_VAL_GET(unit, reg, rval64);
        SOC_REG_RST_MSK_GET(unit, reg, rmsk64);
        format_uint64(rval_str, rval64);
        format_uint64(rmsk_str, rmsk64);
        bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, ", reset value %s mask %s\n", rval_str,
                    rmsk_str);

    }

    for (f = reginfo->nFields - 1; f >= 0; f--)
    {
        fld = &reginfo->fields[f];
        bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, "  %s<%d", SOC_FIELD_NAME(unit, fld->field),
                    fld->bp + fld->len - 1);
        if (fld->len > 1)
        {
            bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, ":%d", fld->bp);
        }

        soc_reg_above_64_field_get(unit, reg, mask, fld->field, fldval);
        if (use_reset && SOC_REG_ABOVE_64_IS_ZERO(fldval))
        {
            bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, "> = x");
        }
        else
        {
            soc_reg_above_64_field_get(unit, reg, regval, fld->field, fldval);
            format_long_integer(buf, fldval, ((fld->len + 31) / 32));
            bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, "> = %s", buf);
        }
        if (fld->flags & (SOCF_RO | SOCF_WO))
        {
            bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, " [");
            i = 0;
            if (fld->flags & SOCF_RO)
            {
                bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, "RO");
                i++;
            }
            if (fld->flags & SOCF_WO)
            {
                bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, "%sWO", i++ ? "," : "");
            }
            bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, "]");
        }
        bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, "\n");

    }
}

STATIC void
diag_sand_print_regname(
    int unit,
    soc_reg_t reg,
    int *col,
    int pflags)
{
    int len;
    soc_reg_info_t *reginfo;

    reginfo = &SOC_REG_INFO(unit, reg);
    len = strlen(SOC_REG_NAME(unit, reg)) + 1;

    if (pflags & PFLAG_SUMMARY)
    {
        char tname, *dstr1, *dstr2, *bname;
        int dlen, copies, i;
        char nstr[128], bstr[64];

        switch (reginfo->regtype)
        {
            case soc_schan_reg:
                tname = 's';
                break;
            case soc_cpureg:
                tname = 'c';
                break;
            case soc_genreg:
                tname = 'g';
                break;
            case soc_portreg:
                tname = 'p';
                break;
            case soc_cosreg:
                tname = 'o';
                break;
            case soc_hostmem_w:
            case soc_hostmem_h:
            case soc_hostmem_b:
                tname = 'm';
                break;
            case soc_phy_reg:
                tname = 'f';
                break;
            case soc_pci_cfg_reg:
                tname = 'P';
                break;
            default:
                tname = '?';
                break;
        }
        dstr2 = strchr(soc_reg_desc[reg], '\n');
        if (dstr2 == NULL)
        {
            dlen = strlen(soc_reg_desc[reg]);
        }
        else
        {
            dlen = dstr2 - soc_reg_desc[reg];
        }
        if (dlen > 30)
        {
            dlen = 30;
            dstr2 = "...";
        }
        else
        {
            dstr2 = "";
        }
        dstr1 = soc_reg_desc[reg];
        if (reginfo->flags & SOC_REG_FLAG_ARRAY)
        {
            sal_sprintf(nstr, "%s[%d]", SOC_REG_NAME(unit, reg), reginfo->numels);
        }
        else
        {
            sal_sprintf(nstr, "%s", SOC_REG_NAME(unit, reg));
        }

        copies = 0;
        bname = NULL;
        for (i = 0; SOC_BLOCK_INFO(unit, i).type >= 0; i++)
        {
            /*
             * if (SOC_BLOCK_INFO(unit, i).type & reginfo->block) {
             */
            if (SOC_BLOCK_IS_TYPE(unit, i, reginfo->block))
            {
                if (bname == NULL)
                {
                    bname = SOC_BLOCK_NAME(unit, i);
                }
                copies += 1;
            }
        }
        if (copies > 1)
        {
            sal_sprintf(bstr, "%d/%s", copies, bname);
        }
        else if (copies == 1)
        {
            sal_sprintf(bstr, "%s", bname);
        }
        else
        {
            sal_sprintf(bstr, "none");
        }
        bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, " %c%c%c%c%c  %-26s %-8.8s  %*.*s%s\n",
                    tname,
                    (reginfo->flags & SOC_REG_FLAG_64_BITS) ? '6' : '3',
                    (reginfo->flags & SOC_REG_FLAG_COUNTER) ? 'c' : '-',
                    (reginfo->flags & SOC_REG_FLAG_ED_CNTR) ? 'e' : '-',
                    (reginfo->flags & SOC_REG_FLAG_RO) ? 'r' :
                    (reginfo->flags & SOC_REG_FLAG_WO) ? 'w' : '-', nstr, bstr, dlen, dlen, dstr1, dstr2);
        return;
    }
    if (*col < 0)
    {
        bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, "  ");
        *col = 2;
    }
    if (*col + len > ((pflags & PFLAG_ALIAS) ? 65 : 72))
    {
        bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, "\n  ");
        *col = 2;
    }
    bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, "%s%s ",
                SOC_REG_NAME(unit, reg), SOC_REG_ARRAY(unit, reg) ? "[]" : "");
    if ((pflags & PFLAG_ALIAS) && soc_reg_alias[reg])
    {
        len += strlen(soc_reg_alias[reg]) + 8;
        bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, "(aka %s) ", soc_reg_alias[reg]);
    }
    *col += len;
}

STATIC void
diag_sand_reg_list_by_type(
    int unit,
    soc_block_t blk,
    int *col,
    int pflag)
{
    soc_reg_t reg;

    *col = -1;
    for (reg = 0; reg < NUM_SOC_REG; reg++)
    {
        if (!SOC_REG_IS_VALID(unit, reg))
        {
            continue;
        }
        /*
         * if (SOC_REG_INFO(unit, reg).block & blk) {
         */
        if (SOC_BLOCK_IN_LIST(SOC_REG_INFO(unit, reg).block, blk))
        {
            diag_sand_print_regname(unit, reg, col, pflag);
        }
    }
    bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, "\n");
}

char cmd_sand_reg_list_usage[] =
    "Usage: listreg [options] regname [value]\n"
    "Options:\n"
    "    -alias/-a       display aliases\n"
    "    -summary/-s     display single line summary for each reg\n"
    "    -counters/-c    display counters\n"
    "    -ed/-e          display error/discard counters\n"
    "    -type/-t        display registers grouped by block type\n"
    "If regname is '*' or does not match a register name, a substring\n"
    "match is performed.  [] indicates register array.\n"
    "If regname is a numeric address, the register that resides at that\n" "address is displayed.\n";

cmd_result_t
cmd_sand_reg_list(
    int unit,
    args_t * a)
{
    char *str;
    char *val;
    soc_reg_above_64_val_t value;
    soc_regaddrinfo_t ainfo;
    int found;
    int rv = CMD_OK;
    int all_regs;
    soc_reg_t reg;
    int col;
    int pflag;

    if (!sh_check_attached(ARG_CMD(a), unit))
    {
        return CMD_FAIL;
    }

    if (!soc_property_get(unit, spn_REGLIST_ENABLE, 1))
    {
        return CMD_OK;
    }
    bsl_log_start(BSL_APPL_SHELL, bslSeverityNormal, unit, "");

    ainfo.reg = INVALIDr;
    pflag = 0;
    col = -1;

    /*
     * Parse options 
     */
    while (((str = ARG_GET(a)) != NULL) && (str[0] == '-'))
    {
        while (str[0] && str[0] == '-')
        {
            str += 1;
        }
        if (sal_strcasecmp(str, "alias") == 0 || sal_strcasecmp(str, "a") == 0)
        {       /* list w/ alias */
            pflag |= PFLAG_ALIAS;
            continue;
        }
        if (sal_strcasecmp(str, "summary") == 0 || sal_strcasecmp(str, "s") == 0)
        {       /* list w/ summary */
            pflag |= PFLAG_SUMMARY;
            continue;
        }
        if (sal_strcasecmp(str, "counters") == 0 || sal_strcasecmp(str, "c") == 0)
        {       /* list counters */
            bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, "unit %d counters\n", unit);
            for (reg = 0; reg < NUM_SOC_REG; reg++)
            {
                if (!SOC_REG_IS_VALID(unit, reg))
                    continue;
                if (!SOC_REG_IS_COUNTER(unit, reg))
                    continue;
                diag_sand_print_regname(unit, reg, &col, pflag);
            }
            bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, "\n\n");
            return CMD_OK;
        }
        if (sal_strcasecmp(str, "ed") == 0 || sal_strcasecmp(str, "e") == 0)
        {       /* error/discard */
            bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, "unit %d error/discard counters\n",
                        unit);
            for (reg = 0; reg < NUM_SOC_REG; reg++)
            {
                if (!SOC_REG_IS_VALID(unit, reg))
                {
                    continue;
                }
                if (!(SOC_REG_INFO(unit, reg).flags & SOC_REG_FLAG_ED_CNTR))
                {
                    continue;
                }
                diag_sand_print_regname(unit, reg, &col, pflag);
            }
            bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, "\n\n");
            return CMD_OK;
        }
        if (sal_strcasecmp(str, "type") == 0 || sal_strcasecmp(str, "t") == 0)
        {       /* list by type */
            int i;

            /*
             * soc_block_t blk;
             * 
             * for (i = 0; i < (sizeof(SOC_INFO(unit).has_block) * 8); i++) { blk = (1 << i); if ( !(blk &
             * SOC_INFO(unit).has_block)) { continue; }
             */

            for (i = 0; i < COUNTOF(SOC_INFO(unit).has_block); i++)
            {
                if (!(SOC_INFO(unit).has_block[i]))
                {
                    continue;
                }
                bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, "unit %d %s registers\n", unit,
                            /*
                             * soc_block_name_lookup_ext(blk, unit));
                             */
                            soc_block_name_lookup_ext(SOC_INFO(unit).has_block[i], unit));
                col = -1;
                /*_list_regs_by_type(unit, blk, &col, pflag);*/
                diag_sand_reg_list_by_type(unit, SOC_INFO(unit).has_block[i], &col, pflag);
            }
            bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, "\n");
            return CMD_OK;
        }
        cli_out("ERROR: unrecognized option: %s\n", str);
        return CMD_FAIL;
    }

    if (!str)
    {
        return CMD_USAGE;
    }

    if ((val = ARG_GET(a)) != NULL)
    {
        parse_long_integer(value, SOC_REG_ABOVE_64_MAX_SIZE_U32, val);

    }
    else
    {
        SOC_REG_ABOVE_64_CLEAR(value);

    }

    if (isint(str))
    {
        /*
         * Address given, look up SOC register.
         */
        char buf[80];
        uint32 addr;
        addr = parse_integer(str);
        diag_sand_reg_addrinfo_get(unit, &ainfo, addr, 0);
        if (!ainfo.valid || (int) ainfo.reg < 0)
        {
            cli_out("Unknown register address: 0x%x\n", addr);
            rv = CMD_FAIL;
        }
        else
        {
            soc_reg_sprint_addr(unit, buf, &ainfo);
            bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, "Address %s\n", buf);
        }
    }
    else
    {
        soc_regaddrlist_t alist;

        if (soc_regaddrlist_alloc(&alist) < 0)
        {
            cli_out("Could not allocate address list.  Memory error.\n");
            return CMD_FAIL;
        }

        /*
         * Symbolic name.
         * First check if the register is there as exact match.
         * If not, list all substring matches.
         */

        all_regs = 0;
        if (*str == '$')
        {
            str++;
        }
        else if (*str == '*')
        {
            str++;
            all_regs = 1;
        }

        if (all_regs || parse_symbolic_reference(unit, &alist, str) < 0)
        {
            found = 0;
            for (reg = 0; reg < NUM_SOC_REG; reg++)
            {
                if (!SOC_REG_IS_VALID(unit, reg))
                {
                    continue;
                }
                if (strcaseindex(SOC_REG_NAME(unit, reg), str) != 0)
                {
                    if (!found && !all_regs)
                    {
                        cli_out("Unknown register; possible matches are:\n");
                    }
                    diag_sand_print_regname(unit, reg, &col, pflag);
                    found = 1;
                }
            }
            if (!found)
            {
                cli_out("No matching register found");
            }
            bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, "\n");
            rv = CMD_FAIL;
        }
        else
        {
            ainfo = alist.ainfo[0];
        }

        soc_regaddrlist_free(&alist);
    }

    /*
     * Now have ainfo -- if reg is no longer INVALIDr
     */

    if (ainfo.reg != INVALIDr)
    {
        if (val)
        {
            diag_sand_reg_do_list(unit, &ainfo, 0, value);
        }
        else
        {
            SOC_REG_ABOVE_64_CLEAR(value);
            diag_sand_reg_do_list(unit, &ainfo, 1, value);
        }
    }
    bsl_log_end(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, "");

    return rv;
}

/*
 * Print a SOC internal register with fields broken out.
 */
STATIC void
diag_sand_reg_print(
    int unit,
    soc_regaddrinfo_t * ainfo,
    soc_reg_above_64_val_t val,
    uint32 flags,
    char *fld_sep,
    int wrap)
{
    soc_reg_info_t *reginfo = &SOC_REG_INFO(unit, ainfo->reg);
    int f;
    char buf[80];
    char line_buf[256];
    int linelen = 0;
    int nprint;
    int rlen;
    int skip;
    soc_reg_above_64_val_t resval, fieldval, resfld;

    if (flags & REG_PRINT_HEX)
    {
        format_long_integer(line_buf, val, SOC_REG_ABOVE_64_MAX_SIZE_U32);
        bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, "%s\n", line_buf);
        return;
    }

    if (flags & REG_PRINT_ADDR)
    {
        if (SOC_REG_IS_ABOVE_64(unit, ainfo->reg))
        {
            rlen = SOC_REG_ABOVE_64_INFO(unit, ainfo->reg).size;
        }
        else if (SOC_REG_IS_64(unit, ainfo->reg))
        {
            rlen = 2;
        }
        else
        {
            rlen = 1;
        }
        format_spec_long_integer(line_buf, val, rlen);

        skip = 0;
        if (SOC_REG_IS_ARRAY(unit, ainfo->reg))
        {
            skip = ainfo->idx * SOC_REG_ARRAY_INFO(unit, ainfo->reg).element_skip;
        }
        else if (SOC_REG_ARRAY(unit, ainfo->reg))
        {
            if (SOC_REG_ARRAY2(unit, ainfo->reg))
            {
                skip = ainfo->idx * 2;
            }
            else
            {
                skip = ainfo->idx;
            }
        }
        bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, "0x%02X 0x%04X: %s\n",
                    SOC_BLOCK_INFO(unit, ainfo->block).schan, SOC_REG_INFO(unit, ainfo->reg).offset + skip, line_buf);
        return;
    }

    if (flags & REG_PRINT_CHG)
    {
        SOC_REG_ABOVE_64_RST_VAL_GET(unit, ainfo->reg, resval);
        if (SOC_REG_ABOVE_64_IS_EQUAL(val, resval))
        {       /* no changed fields */
            return;
        }
    }
    else
    {
        SOC_REG_ABOVE_64_CLEAR(resval);
    }

    soc_reg_sprint_addr(unit, buf, ainfo);

    sal_sprintf(line_buf, "%s[0x%x]=", buf, ainfo->addr);
    PRINT_COUNT(line_buf, linelen, wrap, "   ");

    format_long_integer(line_buf, val, SOC_REG_ABOVE_64_MAX_SIZE_U32);
    PRINT_COUNT(line_buf, linelen, -1, "");

    if (flags & REG_PRINT_RAW)
    {
        bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, "\n");
        return;
    }

    PRINT_COUNT(": <", linelen, wrap, "   ");

    nprint = 0;
    for (f = reginfo->nFields - 1; f >= 0; f--)
    {
        soc_field_info_t *fld = &reginfo->fields[f];
        soc_reg_above_64_field_get(unit, ainfo->reg, val, fld->field, fieldval);
        if (flags & REG_PRINT_CHG)
        {
            soc_reg_above_64_field_get(unit, ainfo->reg, resval, fld->field, resfld);
            if (SOC_REG_ABOVE_64_IS_EQUAL(fieldval, resfld))
            {
                continue;
            }
        }
        {
            if (nprint > 0)
            {
                sal_sprintf(line_buf, "%s", fld_sep);
                PRINT_COUNT(line_buf, linelen, -1, "");
            }
            sal_sprintf(line_buf, "%s=", SOC_FIELD_NAME(unit, fld->field));

            PRINT_COUNT(line_buf, linelen, wrap, "   ");
            format_long_integer(line_buf, fieldval, (fld->len + 31) / 32);
            PRINT_COUNT(line_buf, linelen, -1, "");

        }

        nprint += 1;
        /*
         * bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, "\n");
         */

    }

    bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, ">\n");
}

/*
 * Print a SOC internal register with fields broken out.
 */
STATIC int
diag_sand_reg_print_one_field(
    int unit,
    soc_regaddrinfo_t * ainfo,
    soc_reg_above_64_val_t val,
    uint32 flags,
    char *fld_sep,
    int wrap,
    char *field_name)
{
    soc_reg_info_t *reginfo = &SOC_REG_INFO(unit, ainfo->reg);
    int f;
    int rv = 0;
    char line_buf[256];
    int linelen = 0;
    int nprint;
    int field_found = 0;
    soc_reg_above_64_val_t resval, fieldval, resfld;

    if (flags & REG_PRINT_HEX)
    {
        format_long_integer(line_buf, val, SOC_REG_ABOVE_64_MAX_SIZE_U32);
        bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, "%s\n", line_buf);
        return rv;
    }
    if (flags & REG_PRINT_CHG)
    {
        SOC_REG_ABOVE_64_RST_VAL_GET(unit, ainfo->reg, resval);
        if (SOC_REG_ABOVE_64_IS_EQUAL(val, resval))
        {       /* no changed fields */
            return rv;
        }
    }
    else
    {
        SOC_REG_ABOVE_64_CLEAR(resval);
    }

    nprint = 0;
    for (f = reginfo->nFields - 1; f >= 0; f--)
    {
        soc_field_info_t *fld = &reginfo->fields[f];
        if (sal_strcasecmp(SOC_FIELD_NAME(unit, fld->field), field_name))
        {
            continue;
        }
        field_found = 1;
        soc_reg_above_64_field_get(unit, ainfo->reg, val, fld->field, fieldval);
        if (flags & REG_PRINT_CHG)
        {
            soc_reg_above_64_field_get(unit, ainfo->reg, resval, fld->field, resfld);
            if (SOC_REG_ABOVE_64_IS_EQUAL(fieldval, resfld))
            {
                continue;
            }
        }

        if (nprint > 0)
        {
            sal_sprintf(line_buf, "%s", fld_sep);
            PRINT_COUNT(line_buf, linelen, -1, "");
        }
        sal_sprintf(line_buf, "%s=", SOC_FIELD_NAME(unit, fld->field));

        PRINT_COUNT(line_buf, linelen, wrap, "   ");
        format_long_integer(line_buf, fieldval, (fld->len + 31) / 32);
        PRINT_COUNT(line_buf, linelen, -1, "");
        nprint += 1;
    }

    bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, "\n");
    if (field_found == 0)
    {
        rv = -1;
    }
    return rv;
}

/*
 * Reads and displays all SOC registers specified by alist structure.
 */
STATIC int
diag_sand_reg_print_all(
    int unit,
    soc_regaddrlist_t * alist,
    uint32 flags)
{
    int j;
    soc_reg_above_64_val_t value;
    int r, rv = 0, array_index, blk;
    uint32 val32 = 0, addr;
    uint8 acc_type;
    soc_regaddrinfo_t *ainfo;

    if (alist == NULL)
    {
        return BCM_E_INTERNAL;
    }
    for (j = 0; j < alist->count; j++)
    {
        ainfo = &alist->ainfo[j];
        array_index = SOC_REG_IS_ARRAY(unit, ainfo->reg) ? ainfo->idx : 0;

        if (SOC_REG_IS_WRITE_ONLY(unit, ainfo->reg))
        {
            continue;
        }

        if (soc_cpureg == SOC_REG_INFO(unit, ainfo->reg).regtype)
        {
            addr =
                soc_reg_addr_get(unit, ainfo->reg, ainfo->block, ainfo->idx, SOC_REG_ADDR_OPTION_NONE, &blk, &acc_type);
            val32 = soc_pci_read(unit, addr);
            SOC_REG_ABOVE_64_CLEAR(value);
            value[0] = val32;
            diag_sand_reg_print(unit, ainfo, value, flags, ",", 62);
        }
#ifdef BCM_IPROC_SUPPORT
        else if (soc_iprocreg == SOC_REG_INFO(unit, ainfo->reg).regtype)
        {
            addr =
                soc_reg_addr_get(unit, ainfo->reg, ainfo->block, ainfo->idx, SOC_REG_ADDR_OPTION_NONE, &blk, &acc_type);
            soc_iproc_getreg(unit, addr, &val32);
            SOC_REG_ABOVE_64_CLEAR(value);
            value[0] = val32;
            diag_sand_reg_print(unit, ainfo, value, flags, ",", 62);
        }
#endif /* BCM_IPROC_SUPPORT */
#ifdef BCM_CMICM_SUPPORT
        else if (soc_mcsreg == SOC_REG_INFO(unit, ainfo->reg).regtype)
        {
            addr =
                soc_reg_addr_get(unit, ainfo->reg, ainfo->block, ainfo->idx, SOC_REG_ADDR_OPTION_NONE, &blk, &acc_type);
            val32 = soc_pci_mcs_read(unit, addr);
            if (flags & REG_PRINT_ADDR)
            {
                bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, "%02X 0x%04X: 0x%08x\n",
                            SOC_BLOCK_INFO(unit, ainfo->block).schan, addr, val32);
            }
            else
            {
                bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, "%s[0x%x] = 0x%x\n",
                            SOC_REG_NAME(unit, ainfo->reg), addr, val32);
            }

        }
#endif
        else if (ainfo->is_custom)
        {
            r = soc_custom_reg_above_64_get(unit, ainfo->reg, ainfo->port, array_index, value);
            if (r < 0)
            {
                char buf[80];
                soc_reg_sprint_addr(unit, buf, ainfo);
                cli_out("ERROR: read from register %s failed: %s\n", buf, soc_errmsg(r));
                rv = -1;
            }
            diag_sand_reg_print(unit, ainfo, value, flags, ",", 62);
        }
        else if ((r = soc_reg_above_64_get(unit, ainfo->reg, ainfo->port, array_index, value)) < 0)
        {
            char buf[80];
            soc_reg_sprint_addr(unit, buf, ainfo);
            cli_out("ERROR: read from register %s failed: %s\n", buf, soc_errmsg(r));
            rv = -1;
        }
        else
        {
            diag_sand_reg_print(unit, ainfo, value, flags, ",", 62);
        }
        /*
         * bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, "\n");
         */

    }
    return rv;
}

/*
 * Reads and displays field of specific registers specified by alist structure.
 */
STATIC int
diag_sand_reg_print_field(
    int unit,
    soc_regaddrlist_t * alist,
    uint32 flags,
    char *field_name)
{
    int j;
    soc_reg_above_64_val_t value;
    int r, rv = 0, array_index, blk;
    uint32 val32 = 0, addr;
    uint8 acc_type;
    soc_regaddrinfo_t *ainfo;

    if (alist == NULL)
    {
        return BCM_E_INTERNAL;
    }

    for (j = 0; j < alist->count; j++)
    {
        ainfo = &alist->ainfo[j];
        array_index = SOC_REG_IS_ARRAY(unit, ainfo->reg) ? ainfo->idx : 0;

        if (SOC_REG_IS_WRITE_ONLY(unit, ainfo->reg))
        {
            continue;
        }

        if (soc_cpureg == SOC_REG_INFO(unit, ainfo->reg).regtype)
        {
            addr =
                soc_reg_addr_get(unit, ainfo->reg, ainfo->block, ainfo->idx, SOC_REG_ADDR_OPTION_NONE, &blk, &acc_type);
            val32 = soc_pci_read(unit, addr);
            SOC_REG_ABOVE_64_CLEAR(value);
            value[0] = val32;
            rv = diag_sand_reg_print_one_field(unit, ainfo, value, flags, ",", 62, field_name);
        }
#ifdef BCM_IPROC_SUPPORT
        else if (soc_iprocreg == SOC_REG_INFO(unit, ainfo->reg).regtype)
        {
            addr =
                soc_reg_addr_get(unit, ainfo->reg, ainfo->block, ainfo->idx, SOC_REG_ADDR_OPTION_NONE, &blk, &acc_type);
            soc_iproc_getreg(unit, addr, &val32);
            SOC_REG_ABOVE_64_CLEAR(value);
            value[0] = val32;
            rv = diag_sand_reg_print_one_field(unit, ainfo, value, flags, ",", 62, field_name);
        }
#endif /* BCM_IPROC_SUPPORT */
        else if ((r = soc_reg_above_64_get(unit, ainfo->reg, ainfo->port, array_index, value)) < 0)
        {
            char buf[80];
            soc_reg_sprint_addr(unit, buf, ainfo);
            cli_out("ERROR: read from register %s failed: %s\n", buf, soc_errmsg(r));
            rv = -1;
        }
        else
        {
            rv = diag_sand_reg_print_one_field(unit, ainfo, value, flags, ",", 62, field_name);
        }
    }

    return rv;
}

static regtype_entry_t *
diag_sand_regtype_lookup_name(
    char *str)
{
    int i;

    for (i = 0; i < COUNTOF(dpp_regtypes); i++)
    {
        if (!sal_strcasecmp(str, dpp_regtypes[i].name))
        {
            return &dpp_regtypes[i];
        }
    }

    return 0;
}

/*
 * Get a register by type.
 *
 * doprint:  Boolean.  If set, display data.
 *
 * array_index should be the array index for register arrays, or negative for non array registers
 */
STATIC int
diag_sand_reg_get_by_type(
    int unit,
    uint32 regaddr,
    uint32 cmic_blk,
    soc_regtype_t regtype,
    soc_reg_above_64_val_t outval,
    uint32 flags,
    int array_index)
{
    int rv = CMD_OK;
    int r, i;
    soc_regaddrinfo_t ainfo;

    switch (regtype)
    {
        case soc_genreg:
        case soc_portreg:
            diag_sand_reg_addrinfo_get(unit, &ainfo, regaddr, cmic_blk);
            if (!ainfo.valid)
            {
                cli_out("ERROR: internal error getting reginfo for reg at %ux \n", regaddr);
                rv = CMD_FAIL;
                break;
            }

            if (array_index >= 0)
            {
                /*
                 * non zero array index, implying this is a register array 
                 */
                if (!SOC_REG_IS_ARRAY(unit, ainfo.reg))
                {
                    cli_out("ERROR: can not access a non array register with an array index\n");
                    rv = CMD_FAIL;
                    break;
                }
                else if (array_index >= SOC_REG_NUMELS(unit, ainfo.reg))
                {
                    cli_out("ERROR: array index %d is out of range, should be under %d\n",
                            array_index, SOC_REG_NUMELS(unit, ainfo.reg));
                    rv = CMD_FAIL;
                    break;
                }
            }
            else
            {
                /*
                 * an array index was not specified 
                 */
                if (SOC_REG_IS_ARRAY(unit, ainfo.reg))
                {
                    cli_out("ERROR: can not access a register array without an array index\n");
                    rv = CMD_FAIL;
                    break;
                }
                array_index = 0;        /* for accessing non array registers */
            }

            r = soc_reg_above_64_get(unit, ainfo.reg, ainfo.idx, array_index, outval);
            if (SOC_FAILURE(r))
            {
                cli_out("ERROR: soc_reg_read failed: %s\n", soc_errmsg(r));
                rv = CMD_FAIL;
            }
            break;

        default:
            cli_out("ERROR: Invalid regtype for getreg specified.\n");
            rv = CMD_FAIL;
            break;
    }

    if ((rv == CMD_OK) && (flags & REG_PRINT_DO_PRINT))
    {
        if (flags & REG_PRINT_HEX)
        {
            if (SOC_REG_IS_ABOVE_64(unit, ainfo.reg))
            {
                for (i = 0; i < SOC_REG_ABOVE_64_INFO(unit, ainfo.reg).size; i++)
                {
                    bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, "%08x", outval[i]);
                }
                bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, "\n");
            }
            else if (SOC_REG_IS_64(unit, ainfo.reg))
            {
                bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, "%08x %08x\n", outval[0],
                            outval[1]);
            }
            else
            {
                bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, "%08x\n", outval[0]);
            }
        }
        else
        {
            char buf[256];
            bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, "%s[0x%x] = ",
                        soc_regtypenames[regtype], regaddr);
            if (SOC_REG_IS_ABOVE_64(unit, ainfo.reg))
            {
                format_long_integer(buf, outval, SOC_REG_ABOVE_64_INFO(unit, ainfo.reg).size);
            }
            else if (SOC_REG_IS_64(unit, ainfo.reg))
            {
                format_long_integer(buf, outval, 2);
            }
            else
            {
                format_long_integer(buf, outval, 1);
            }
            bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, "%s\n", buf);
        }
    }

    return rv;
}

static int
diag_sand_filter_regs(
    int unit,
    soc_reg_t reg)
{

    /*
     * CMIC regs filter 
     */
    switch (reg)
    {
            /*
             * Sarath Kumar Immadisetty : These registers are absolete.
             */
        case CMIC_MMU_COSLC_COUNT_ADDRr:
        case CMIC_MMU_COSLC_COUNT_DATAr:

            /*
             * Nir Shaknay These registers do not ack the CPU until the FSCHAN read cycle is completed. Meaning you
             * must activate a proper FSCHAN read command in order to read these registers.
             */
        case CMIC_CMC0_FSCHAN_DATA32r:
        case CMIC_CMC0_FSCHAN_DATA64_LOr:
        case CMIC_FSCHAN_DATA32r:
        case CMIC_FSCHAN_DATA64_LOr:
        case CMIC_CMC1_FSCHAN_DATA32r:
        case CMIC_CMC1_FSCHAN_DATA64_LOr:
        case CMIC_CMC2_FSCHAN_DATA32r:
        case CMIC_CMC2_FSCHAN_DATA64_LOr:
        case CMIC_TS_FREQ_CTRL_LOWERr:
        case CMIC_TS_FREQ_CTRL_UPPERr:
        case CMIC_BROADSYNC_REF_CLK_GEN_CTRLr:
        case CMIC_TIMESYNC_BROADSYNC0_CLK_COUNT_CTRLr:
        case CMIC_TIMESYNC_BROADSYNC1_CLK_COUNT_CTRLr:
        case CMIC_TIMESYNC_CAPTURE_STATUS_1r:
        case CMIC_TIMESYNC_CAPTURE_STATUS_CLR_1r:
        case CMIC_TIMESYNC_INTERRUPT_ENABLEr:
        case CMIC_TIMESYNC_INTERRUPT_STATUSr:
        case CMIC_TIMESYNC_INTERRUPT_CLRr:
        case CMIC_TIMESYNC_TMr:
        case CMIC_TIMESYNC_TS0_FREQ_CTRL_LOWERr:
        case CMIC_TIMESYNC_TS1_FREQ_CTRL_LOWERr:
        case CMIC_TIMESYNC_COUNTER_CONFIG_SELECTr:
        case CMIC_TIMESYNC_TS0_COUNTER_ENABLEr:
        case CMIC_TIMESYNC_TS0_FREQ_CTRL_FRACr:
        case CMIC_TIMESYNC_TS0_FREQ_CTRL_UPPERr:
        case CMIC_TIMESYNC_TS1_COUNTER_ENABLEr:
        case CMIC_TIMESYNC_TS1_FREQ_CTRL_FRACr:
        case CMIC_TIMESYNC_TS1_FREQ_CTRL_UPPERr:
        case CMIC_TIMESYNC_TIME_CAPTURE_MODEr:
        case CMIC_TIMESYNC_INPUT_TIME_FIFO_TS_LOWERr:
        case CMIC_TIMESYNC_INPUT_TIME_FIFO_TS_UPPERr:
        case CMIC_TIMESYNC_FIFO_STATUSr:
        case CMIC_TIMESYNC_GPIO_0_CTRLr:
        case CMIC_TIMESYNC_GPIO_0_OUTPUT_ENABLEr:
        case CMIC_TIMESYNC_GPIO_0_UP_EVENT_CTRLr:
        case CMIC_TIMESYNC_GPIO_0_DOWN_EVENT_CTRLr:
        case CMIC_TIMESYNC_GPIO_0_PHASE_ADJUST_LOWERr:
        case CMIC_TIMESYNC_GPIO_0_PHASE_ADJUST_UPPERr:
        case CMIC_TIMESYNC_GPIO_0_INPUT_DIVISORr:
        case CMIC_TIMESYNC_GPIO_1_CTRLr:
        case CMIC_TIMESYNC_GPIO_1_OUTPUT_ENABLEr:
        case CMIC_TIMESYNC_GPIO_1_UP_EVENT_CTRLr:
        case CMIC_TIMESYNC_GPIO_1_DOWN_EVENT_CTRLr:
        case CMIC_TIMESYNC_GPIO_1_PHASE_ADJUST_LOWERr:
        case CMIC_TIMESYNC_GPIO_1_PHASE_ADJUST_UPPERr:
        case CMIC_TIMESYNC_GPIO_1_INPUT_DIVISORr:
        case CMIC_TIMESYNC_GPIO_2_CTRLr:
        case CMIC_TIMESYNC_GPIO_2_OUTPUT_ENABLEr:
        case CMIC_TIMESYNC_GPIO_2_UP_EVENT_CTRLr:
        case CMIC_TIMESYNC_GPIO_2_DOWN_EVENT_CTRLr:
        case CMIC_TIMESYNC_GPIO_2_PHASE_ADJUST_LOWERr:
        case CMIC_TIMESYNC_GPIO_2_PHASE_ADJUST_UPPERr:
        case CMIC_TIMESYNC_GPIO_2_INPUT_DIVISORr:
        case CMIC_TIMESYNC_GPIO_3_CTRLr:
        case CMIC_TIMESYNC_GPIO_3_OUTPUT_ENABLEr:
        case CMIC_TIMESYNC_GPIO_3_UP_EVENT_CTRLr:
        case CMIC_TIMESYNC_GPIO_3_DOWN_EVENT_CTRLr:
        case CMIC_TIMESYNC_GPIO_3_PHASE_ADJUST_LOWERr:
        case CMIC_TIMESYNC_GPIO_3_PHASE_ADJUST_UPPERr:
        case CMIC_TIMESYNC_GPIO_3_INPUT_DIVISORr:
        case CMIC_TIMESYNC_GPIO_4_CTRLr:
        case CMIC_TIMESYNC_GPIO_4_OUTPUT_ENABLEr:
        case CMIC_TIMESYNC_GPIO_4_UP_EVENT_CTRLr:
        case CMIC_TIMESYNC_GPIO_4_DOWN_EVENT_CTRLr:
        case CMIC_TIMESYNC_GPIO_4_PHASE_ADJUST_LOWERr:
        case CMIC_TIMESYNC_GPIO_4_PHASE_ADJUST_UPPERr:
        case CMIC_TIMESYNC_GPIO_4_INPUT_DIVISORr:
        case CMIC_TIMESYNC_GPIO_5_CTRLr:
        case CMIC_TIMESYNC_GPIO_5_OUTPUT_ENABLEr:
        case CMIC_TIMESYNC_GPIO_5_UP_EVENT_CTRLr:
        case CMIC_TIMESYNC_GPIO_5_DOWN_EVENT_CTRLr:
        case CMIC_TIMESYNC_GPIO_5_PHASE_ADJUST_LOWERr:
        case CMIC_TIMESYNC_GPIO_5_PHASE_ADJUST_UPPERr:
        case CMIC_TIMESYNC_GPIO_5_INPUT_DIVISORr:
        case CMIC_TIMESYNC_SYNCE_CLK1_COUNT_CTRLr:
        case CMIC_TIMESYNC_SYNCE_CLK2_COUNT_CTRLr:
        case CMIC_TIMESYNC_SYNCE_CLK3_COUNT_CTRLr:
        case CMIC_TIMESYNC_SYNCE_CLK4_COUNT_CTRLr:
        case CMIC_TIMESYNC_SYNCE_CLK5_COUNT_CTRLr:
        case CMIC_TIMESYNC_TIME_CAPTURE_CONTROLr:
        case CMICTXCOSMASKr:
        case CMIC_BS_CLK_CTRLr:
        case CMIC_BS_CONFIGr:
        case CMIC_BS_HEARTBEAT_CTRLr:
        case CMIC_BS_HEARTBEAT_DOWN_DURATIONr:
        case CMIC_BS_HEARTBEAT_UP_DURATIONr:
        case CMIC_BS_INITIAL_CRCr:
        case CMIC_BS_INPUT_TIME_0r:
        case CMIC_BS_INPUT_TIME_1r:
        case CMIC_BS_INPUT_TIME_2r:
        case CMIC_BS_OFFSET_ADJUST_0r:
        case CMIC_BS_OFFSET_ADJUST_1r:
        case CMIC_BS_OUTPUT_TIME_0r:
        case CMIC_BS_OUTPUT_TIME_1r:
        case CMIC_BS_OUTPUT_TIME_2r:
        case CMIC_BS_REF_CLK_GEN_CTRLr:
        case CMIC_CMC0_FIFO_CH2_RD_DMA_HOSTMEM_START_ADDRESSr:
        case CMIC_TS_CAPTURE_STATUSr:
        case CMIC_TS_CAPTURE_STATUS_CLRr:
        case CMIC_TS_FIFO_STATUSr:
        case CMIC_TS_INPUT_TIME_FIFO_IDr:
        case CMIC_TS_INPUT_TIME_FIFO_TSr:
        case CMIC_TS_L1_CLK_RECOVERED_CLK_BKUP_COUNT_CTRLr:
        case CMIC_TS_L1_CLK_RECOVERED_CLK_COUNT_CTRLr:
        case CMIC_TS_LCPLL_CLK_COUNT_CTRLr:
        case CMIC_TS_TIME_CAPTURE_CTRLr:
        case CMIC_TS_GPIO_1_CTRLr:
        case CMIC_TS_GPIO_1_DOWN_EVENT_CTRLr:
        case CMIC_TS_GPIO_1_OUTPUT_ENABLEr:
        case CMIC_TS_GPIO_1_PHASE_ADJUSTr:
        case CMIC_TS_GPIO_1_UP_EVENT_CTRLr:
        case CMIC_TS_GPIO_2_CTRLr:
        case CMIC_TS_GPIO_2_DOWN_EVENT_CTRLr:
        case CMIC_TS_GPIO_2_OUTPUT_ENABLEr:
        case CMIC_TS_GPIO_2_PHASE_ADJUSTr:
        case CMIC_TS_GPIO_2_UP_EVENT_CTRLr:
        case CMIC_TS_GPIO_3_CTRLr:
        case CMIC_TS_GPIO_3_DOWN_EVENT_CTRLr:
        case CMIC_TS_GPIO_3_OUTPUT_ENABLEr:
        case CMIC_TS_GPIO_3_PHASE_ADJUSTr:
        case CMIC_TS_GPIO_3_UP_EVENT_CTRLr:
        case CMIC_TS_GPIO_4_CTRLr:
        case CMIC_TS_GPIO_4_DOWN_EVENT_CTRLr:
        case CMIC_TS_GPIO_4_OUTPUT_ENABLEr:
        case CMIC_TS_GPIO_4_PHASE_ADJUSTr:
        case CMIC_TS_GPIO_4_UP_EVENT_CTRLr:
        case CMIC_TS_GPIO_5_CTRLr:
        case CMIC_TS_GPIO_5_DOWN_EVENT_CTRLr:
        case CMIC_TS_GPIO_5_OUTPUT_ENABLEr:
        case CMIC_TS_GPIO_5_PHASE_ADJUSTr:
        case CMIC_TS_GPIO_5_UP_EVENT_CTRLr:
        case CMIC_TS_GPIO_6_CTRLr:
        case CMIC_TS_GPIO_6_DOWN_EVENT_CTRLr:
        case CMIC_TS_GPIO_6_OUTPUT_ENABLEr:
        case CMIC_TS_GPIO_6_PHASE_ADJUSTr:
        case CMIC_TS_GPIO_6_UP_EVENT_CTRLr:

            return 1;
    }

    if (SOC_IS_JERICHO_AND_BELOW(unit))
    {
        /*
         * we skip on the following common regs after checking with noam halevi from the following reasons
         * 1. XXX_ECC_INTERRUPT_REGISTER_TEST because that the test mask is vary from block to block biut since the register
         * taken from common we cant know the actual size
         *
         * 2. XXX_INDIRECT_COMMAND   since INDIRECT_COMMAND_COUNT change by the HW we cant count on what we wrote
         *
         * 3. XXX_INDIRECT_COMMAND_WR_DATA since length vary from block to block and its width is like the widest mem in a block  
         *  we cant actually know how match bits is realy active for this register while its definition came from common
         *  and its 640 bits
         *
         *
         */
#ifdef BCM_PETRA_SUPPORT
        uint32 disallowed_fields[] = { INDIRECT_COMMAND_COUNTf, INDIRECT_COMMAND_WR_DATAf, INTERRUPT_REGISTER_TESTf,
            ECC_INTERRUPT_REGISTER_TESTf, NUM_SOC_FIELD
        };
        if (reg_contain_one_of_the_fields(unit, reg, disallowed_fields))
        {
            return 1;
        }
#endif
        switch (reg)
        {
                /*
                 * these global registers vary from block to block, making the expected value wrong 
                 */
                /*
                 * Following regs are not included in all blocks 
                 */
            case FDR_ENABLE_DYNAMIC_MEMORY_ACCESSr:
            case FMAC_ENABLE_DYNAMIC_MEMORY_ACCESSr:
            case NBIH_ENABLE_DYNAMIC_MEMORY_ACCESSr:
            case NBIL_ENABLE_DYNAMIC_MEMORY_ACCESSr:
            case FDR_INDIRECT_COMMAND_ADDRESSr:
            case FMAC_INDIRECT_COMMAND_ADDRESSr:
            case NBIH_INDIRECT_COMMAND_ADDRESSr:
            case NBIL_INDIRECT_COMMAND_ADDRESSr:
            case FDR_INDIRECT_COMMAND_DATA_INCREMENTr:
            case FMAC_INDIRECT_COMMAND_DATA_INCREMENTr:
            case NBIH_INDIRECT_COMMAND_DATA_INCREMENTr:
            case NBIL_INDIRECT_COMMAND_DATA_INCREMENTr:
            case FDR_INDIRECT_COMMAND_WIDE_MEMr:
            case FMAC_INDIRECT_COMMAND_WIDE_MEMr:
            case NBIH_INDIRECT_COMMAND_WIDE_MEMr:
            case NBIL_INDIRECT_COMMAND_WIDE_MEMr:
            case FDR_INDIRECT_FORCE_BUBBLEr:
            case FMAC_INDIRECT_FORCE_BUBBLEr:
            case NBIH_INDIRECT_FORCE_BUBBLEr:
            case NBIL_INDIRECT_FORCE_BUBBLEr:

                /*
                 * Unknown errors 
                 */
            case CLPORT_SGNDET_EARLYCRSr:
            case EDB_PAR_ERR_INITIATEr:
            case MACSEC_PROG_TX_CRCr:
            case MAC_PFC_CTRLr:
            case MAC_PFC_REFRESH_CTRLr:
            case SFD_OFFSETr:

                /*
                 * additional regs r/w test at tr 3 failed on emulation
                 */
            case IHB_INTERRUPT_MASK_REGISTERr:
            case IHP_VTT_PROGRAM_ENCOUNTEREDr:
            case ILKN_PMH_ECC_INTERRUPT_REGISTER_TESTr:
            case ILKN_PMH_INDIRECT_COMMAND_WR_DATAr:
            case ILKN_PMH_INDIRECT_COMMANDr:
            case ILKN_PMH_INTERRUPT_REGISTER_TESTr:
            case ILKN_PML_ECC_INTERRUPT_REGISTER_TESTr:
            case ILKN_PML_INDIRECT_COMMAND_WR_DATAr:
            case ILKN_PML_INDIRECT_COMMANDr:
            case ILKN_PML_INTERRUPT_REGISTER_TESTr:
            case ILKN_SLE_RX_CFGr:
            case ILKN_SLE_RX_DEBUG_0r:
            case ILKN_SLE_RX_ERRINS_0r:
            case ILKN_SLE_RX_LANEr:
            case ILKN_SLE_RX_STATS_WT_ERR_HIGHr:
            case ILKN_SLE_RX_STATS_WT_ERR_LOWr:
            case ILKN_SLE_RX_STATS_WT_PARITYr:
            case ILKN_SLE_RX_STATS_WT_PKT_LOWr:
            case ILKN_SLE_TX_CFGr:
            case ILKN_SLE_TX_DEBUG_0r:
            case ILKN_SLE_TX_ERRINS_0r:
            case ILKN_SLE_TX_LANEr:
            case ILKN_SLE_TX_STATS_WT_ERR_HIGHr:
            case ILKN_SLE_TX_STATS_WT_ERR_LOWr:
            case ILKN_SLE_TX_STATS_WT_PARITYr:
            case ILKN_SLE_TX_STATS_WT_PKT_LOWr:

            case ILKN_SLE_RX_AFIFO_WMr:
            case ILKN_SLE_RX_BURSTr:
            case ILKN_SLE_RX_CAL_ACCr:
            case ILKN_SLE_RX_CAL_INBAND_DYNr:
            case ILKN_SLE_RX_CAL_INBANDr:
            case ILKN_SLE_RX_CAL_OUTBAND_DYNr:
            case ILKN_SLE_RX_CAL_OUTBANDr:
            case ILKN_SLE_RX_CAL_WTr:
            case ILKN_SLE_RX_CTLr:
            case ILKN_SLE_RX_DEBUG_1r:
            case ILKN_SLE_RX_DEBUG_2r:
            case ILKN_SLE_RX_DEBUG_3r:
            case ILKN_SLE_RX_DEBUG_HOLD_0r:
            case ILKN_SLE_RX_DEBUG_HOLD_1r:
            case ILKN_SLE_RX_DEBUG_HOLD_2r:
            case ILKN_SLE_RX_DEBUG_HOLD_3r:
            case ILKN_SLE_RX_ERRINS_1r:
            case ILKN_SLE_RX_ERRINS_2r:
            case ILKN_SLE_RX_ERRINS_3r:
            case ILKN_SLE_RX_EXT_INT_2_ND_MASKr:
            case ILKN_SLE_RX_EXT_INT_FORCEr:
            case ILKN_SLE_RX_EXT_INT_MASKr:
            case ILKN_SLE_RX_FCOB_RETRANSMIT_SLOT_DYr:
            case ILKN_SLE_RX_FCOB_RETRANSMIT_SLOTr:
            case ILKN_SLE_RX_INT_2_ND_MASKr:
            case ILKN_SLE_RX_INT_FORCEr:
            case ILKN_SLE_RX_INT_MASKr:
            case ILKN_SLE_RX_LANE_2r:
            case ILKN_SLE_RX_METAFRAMEr:
            case ILKN_SLE_RX_REMAP_LANE_14_10r:
            case ILKN_SLE_RX_REMAP_LANE_19_15r:
            case ILKN_SLE_RX_REMAP_LANE_24_20r:
            case ILKN_SLE_RX_REMAP_LANE_29_25r:
            case ILKN_SLE_RX_REMAP_LANE_34_30r:
            case ILKN_SLE_RX_REMAP_LANE_39_35r:
            case ILKN_SLE_RX_REMAP_LANE_44_40r:
            case ILKN_SLE_RX_REMAP_LANE_47_45r:
            case ILKN_SLE_RX_REMAP_LANE_4_0r:
            case ILKN_SLE_RX_REMAP_LANE_9_5r:
            case ILKN_SLE_RX_RETRANSMIT_CONFIGr:
            case ILKN_SLE_RX_RETRANSMIT_TIME_CONFIG_2r:
            case ILKN_SLE_RX_RETRANSMIT_TIME_CONFIGr:
            case ILKN_SLE_RX_SEGMENT_ENABLEr:
            case ILKN_SLE_RX_SERDES_TEST_CNTLr:
            case ILKN_SLE_RX_SERDES_TEST_PATTERNAr:
            case ILKN_SLE_RX_SERDES_TEST_PATTERNBr:
            case ILKN_SLE_RX_SERDES_TEST_PATTERNCr:
            case ILKN_SLE_RX_STATS_ACCr:
            case ILKN_SLE_RX_STATS_WT_BYTE_LOWr:
            case ILKN_SLE_TX_AFIFO_WMr:
            case ILKN_SLE_TX_BURSTr:
            case ILKN_SLE_TX_CAL_ACCr:
            case ILKN_SLE_TX_CAL_INBAND_DYNr:
            case ILKN_SLE_TX_CAL_INBANDr:
            case ILKN_SLE_TX_CAL_OUTBAND_DYNr:
            case ILKN_SLE_TX_CAL_OUTBANDr:
            case ILKN_SLE_TX_CAL_WTr:
            case ILKN_SLE_TX_CTLr:
            case ILKN_SLE_TX_DEBUG_1r:
            case ILKN_SLE_TX_DEBUG_2r:
            case ILKN_SLE_TX_DEBUG_3r:
            case ILKN_SLE_TX_DEBUG_HOLD_0r:
            case ILKN_SLE_TX_DEBUG_HOLD_1r:
            case ILKN_SLE_TX_DEBUG_HOLD_2r:
            case ILKN_SLE_TX_DEBUG_HOLD_3r:
            case ILKN_SLE_TX_ERRINS_1r:
            case ILKN_SLE_TX_ERRINS_2r:
            case ILKN_SLE_TX_ERRINS_3r:
            case ILKN_SLE_TX_FCOB_RETRANSMIT_SLOT_DYr:
            case ILKN_SLE_TX_FCOB_RETRANSMIT_SLOTr:
            case ILKN_SLE_TX_FIFO_CFGr:
            case ILKN_SLE_TX_INT_2_ND_MASKr:
            case ILKN_SLE_TX_INT_FORCEr:
            case ILKN_SLE_TX_INT_MASKr:
            case ILKN_SLE_TX_LANE_2r:
            case ILKN_SLE_TX_METAFRAMEr:
            case ILKN_SLE_TX_RATE_1r:
            case ILKN_SLE_TX_REMAP_LANE_14_10r:
            case ILKN_SLE_TX_REMAP_LANE_19_15r:
            case ILKN_SLE_TX_REMAP_LANE_24_20r:
            case ILKN_SLE_TX_REMAP_LANE_29_25r:
            case ILKN_SLE_TX_REMAP_LANE_34_30r:
            case ILKN_SLE_TX_REMAP_LANE_39_35r:
            case ILKN_SLE_TX_REMAP_LANE_44_40r:
            case ILKN_SLE_TX_REMAP_LANE_47_45r:
            case ILKN_SLE_TX_REMAP_LANE_4_0r:
            case ILKN_SLE_TX_REMAP_LANE_9_5r:
            case ILKN_SLE_TX_RETRANSMIT_CONFIGr:
            case ILKN_SLE_TX_SEGMENT_ENABLEr:
            case ILKN_SLE_TX_SERDES_AFIFO_STALL_SELr:
            case ILKN_SLE_TX_SERDES_TEST_CNTLr:
            case ILKN_SLE_TX_SERDES_TEST_PATTERNAr:
            case ILKN_SLE_TX_SERDES_TEST_PATTERNBr:
            case ILKN_SLE_TX_SERDES_TEST_PATTERNCr:
            case ILKN_SLE_TX_STATS_ACCr:
            case ILKN_SLE_TX_STATS_WT_BYTE_LOWr:

            case CFC_INTERRUPT_MASK_REGISTERr: /* tr3 emulation * * *
                                                 * reg=CFC_INTERRUPT_MASK_REGISTERr,value=0x55501,expected0x55555 */

            case MESH_TOPOLOGY_ECC_ERR_1B_INITIATEr:
            case MESH_TOPOLOGY_ECC_ERR_1B_MONITOR_MEM_MASKr:
            case MESH_TOPOLOGY_ECC_ERR_2B_INITIATEr:
            case MESH_TOPOLOGY_ECC_ERR_2B_MONITOR_MEM_MASKr:
            case MESH_TOPOLOGY_GLOBAL_MEM_OPTIONSr:
            case MESH_TOPOLOGY_RESERVED_MTCPr:

                /*
                 * Dana Ziman (Block Owner) : "The NBIL/NBIH in Jericho has no indirect access." 
                 */
            case NBIH_INDIRECT_COMMAND_RD_DATAr:
            case NBIL_INDIRECT_COMMAND_RD_DATAr:

                /*
                 * Uri Amit (Block Owner) : "FDR, FMAC and MESH_TOPOLOGY in Jericho has no indirect access."
                 */
            case FDR_INDIRECT_COMMAND_RD_DATAr:
            case FMAC_INDIRECT_COMMAND_RD_DATAr:
            case MESH_TOPOLOGY_ECC_1B_ERR_CNTr:
            case MESH_TOPOLOGY_ECC_2B_ERR_CNTr:
            case MESH_TOPOLOGY_ERROR_INITIATION_DATAr:
            case MESH_TOPOLOGY_PARITY_ERR_CNTr:

                /*
                 * Keren Yaar(Block Owner) : "Should not be readable"
                 */

                return 1;
        }
    }
    return 0;
}

int
diag_sand_reg_get_all(
    int unit,
    int is_debug)
{
    int rc, blk, port, j;
    
#ifdef BCM_PETRA_SUPPORT
    int is_master, master_port;
#endif
    soc_reg_t reg;
    soc_regaddrlist_t alist;
    int count = 0;
    uint32 flags = REG_PRINT_RAW;       /* all regs should be printed in raw mode */
    uint32 phy_port;

    if (is_debug)
    {
        flags |= REG_PRINT_ADDR;
    }
    bsl_log_start(BSL_APPL_SHELL, bslSeverityNormal, unit, "");

    /*
     * count relevant regs
     */
    for (reg = 0; reg < NUM_SOC_REG; reg++)
    {
        if (!SOC_REG_IS_VALID(unit, reg))
        {
            continue;
        }

        if (diag_sand_filter_regs(unit, reg))
        {
            continue;
        }

        if (soc_genreg == SOC_REG_INFO(unit, reg).regtype)
        {
            SOC_BLOCKS_ITER(unit, blk, SOC_REG_INFO(unit, reg).block)
            {
                count += SOC_REG_INFO(unit, reg).numels;
            }
        }

        if (soc_portreg == SOC_REG_INFO(unit, reg).regtype)
        {
            PBMP_ITER(PBMP_PORT_ALL(unit), port)
            {
#ifdef BCM_PETRA_SUPPORT
                if (SOC_IS_ARAD(unit) && !IS_SFI_PORT(unit, port))
                {
                    rc = soc_port_sw_db_master_channel_get(unit, port, &master_port);
                    if (rc < 0)
                    {
                        return CMD_FAIL;
                    }
                    is_master = (port == master_port ? 1 : 0);
                    if (!is_master)
                    {
                        continue;
                    }
                }
#endif
                if (SOC_REG_BLOCK_IN_LIST(unit, reg, SOC_PORT_TYPE(unit, port)))
                {
                    count += SOC_REG_INFO(unit, reg).numels;
                }
            }
        }

        if (soc_cpureg == SOC_REG_INFO(unit, reg).regtype || soc_customreg == SOC_REG_INFO(unit, reg).regtype)
        {
            count += SOC_REG_INFO(unit, reg).numels;
        }
    }

    bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, "Collected %d registers\n", count);

    /*
     * allocate alist
     */
    alist.count = 0;
    alist.ainfo = sal_alloc(sizeof(soc_regaddrinfo_t) * count, "alist.ainfo");
    if (NULL == alist.ainfo)
    {
        cli_out("Failed to allocate alist.ainfo\n");
        return CMD_FAIL;
    }

    /*
     * collect data
     */
    for (reg = 0; reg < NUM_SOC_REG; reg++)
    {
        if (!SOC_REG_IS_VALID(unit, reg))
        {
            continue;
        }

        if (diag_sand_filter_regs(unit, reg))
        {
            continue;
        }

        if (soc_genreg == SOC_REG_INFO(unit, reg).regtype || soc_cpureg == SOC_REG_INFO(unit, reg).regtype)
        {
            SOC_BLOCKS_ITER(unit, blk, SOC_REG_INFO(unit, reg).block)
            {
                if (SOC_REG_INFO(unit, reg).numels > 0)
                {
                    for (j = 0; j < SOC_REG_INFO(unit, reg).numels; j++)
                    {
                        alist.ainfo[alist.count].addr = SOC_REG_INFO(unit, reg).offset;
                        alist.ainfo[alist.count].valid = 1;
                        alist.ainfo[alist.count].reg = reg;
                        alist.ainfo[alist.count].idx = j;
                        alist.ainfo[alist.count].block = blk;
                        alist.ainfo[alist.count].port = SOC_BLOCK_PORT(unit, blk);
                        alist.ainfo[alist.count].cos = -1;
                        alist.ainfo[alist.count].field = INVALIDf;
                        alist.ainfo[alist.count].is_custom = 0;
                        alist.count++;
                    }
                }
                else
                {
                    alist.ainfo[alist.count].addr = SOC_REG_INFO(unit, reg).offset;
                    alist.ainfo[alist.count].valid = 1;
                    alist.ainfo[alist.count].reg = reg;
                    alist.ainfo[alist.count].idx = 0;
                    alist.ainfo[alist.count].block = blk;
                    alist.ainfo[alist.count].port = SOC_BLOCK_PORT(unit, blk);
                    alist.ainfo[alist.count].cos = -1;
                    alist.ainfo[alist.count].field = INVALIDf;
                    alist.ainfo[alist.count].is_custom = 0;
                    alist.count++;
                }
            }
        }

        if (soc_portreg == SOC_REG_INFO(unit, reg).regtype)
        {
            PBMP_ITER(PBMP_PORT_ALL(unit), port)
            {
#ifdef BCM_PETRA_SUPPORT
                if (SOC_IS_ARAD(unit) && !IS_SFI_PORT(unit, port))
                {
                    rc = soc_port_sw_db_master_channel_get(unit, port, &master_port);
                    if (rc < 0)
                    {
                        return CMD_FAIL;
                    }
                    is_master = (port == master_port ? 1 : 0);
                    if (!is_master)
                    {
                        continue;
                    }
                }
#endif
                if (SOC_REG_BLOCK_IN_LIST(unit, reg, SOC_PORT_TYPE(unit, port)))
                {
                    alist.ainfo[alist.count].addr = SOC_REG_INFO(unit, reg).offset;
                    alist.ainfo[alist.count].valid = 1;
                    alist.ainfo[alist.count].reg = reg;
                    alist.ainfo[alist.count].idx = 0;
                    phy_port = SOC_INFO(unit).port_l2p_mapping[port];
                    alist.ainfo[alist.count].block = SOC_PORT_BLOCK(unit, phy_port);
                    alist.ainfo[alist.count].port = port;
                    alist.ainfo[alist.count].cos = -1;
                    alist.ainfo[alist.count].field = INVALIDf;
                    alist.ainfo[alist.count].is_custom = 0;
                    alist.count++;
                }
            }
        }

        if (soc_customreg == SOC_REG_INFO(unit, reg).regtype)
        {
            PBMP_ITER(PBMP_PORT_ALL(unit), port)
            {
#ifdef BCM_PETRA_SUPPORT
                if (SOC_IS_ARAD(unit) && !IS_SFI_PORT(unit, port))
                {
                    rc = soc_port_sw_db_master_channel_get(unit, port, &master_port);
                    if (rc < 0)
                    {
                        return CMD_FAIL;
                    }
                    is_master = (port == master_port);
                    if (!is_master)
                    {
                        continue;
                    }
                }
#endif
                if (SOC_REG_BLOCK_IS(unit, reg, SOC_PORT_TYPE(unit, port)))
                {
                    alist.ainfo[alist.count].addr = SOC_REG_INFO(unit, reg).offset;
                    alist.ainfo[alist.count].valid = 1;
                    alist.ainfo[alist.count].reg = reg;
                    alist.ainfo[alist.count].idx = 0;
                    phy_port = SOC_INFO(unit).port_l2p_mapping[port];
                    alist.ainfo[alist.count].block = SOC_PORT_BLOCK(unit, phy_port);
                    alist.ainfo[alist.count].port = port;
                    alist.ainfo[alist.count].cos = -1;
                    alist.ainfo[alist.count].field = INVALIDf;
                    alist.ainfo[alist.count].is_custom = 1;
                    alist.count++;
                }
            }
        }
    }

    /*
     * print
     */
    rc = diag_sand_reg_print_all(unit, &alist, flags);
    bsl_log_end(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, "");

    sal_free(alist.ainfo);
    if (rc < 0)
    {
        return CMD_FAIL;
    }

    return CMD_OK;
}

/*
 * Pedantic compiler allows only 509 chars
 */
char cmd_sand_reg_get_usage[] =
#ifdef COMPILER_STRING_CONST_LIMIT
    "1. Parameters: [hex|raw|chg] [<REGTYPE>] [idx <array_index>] <REGISTER>\n\t"
    "2. Parameters: [hex|raw|chg] [<REGTYPE>] cmic_blk <cmic_blk> "
    "<REGISTER_ADDRESS>\n\t" "Full documentation cannot be displayed with -pendantic compiler\n";
#else
    "1. Parameters: [hex|raw|chg] [<REGTYPE>] [idx <array_index>] <REGISTER>\n\t"
    "2. Parameters: [hex|raw|chg] [<REGTYPE>] cmic_blk <cmic_blk> "
    "<REGISTER_ADDRESS>\n\t"
    "If <REGTYPE> is not specified, it defaults to \"soc\".\n\t"
    "<REGISTER> designates offset, address, or symbolic name.\n\t"
    "If hex is specified, dumps only a hex value (for Expect parsing).\n\t"
    "If raw is specified, no field decoding is done.\n\t"
    "If chg is specified, show only fields/regs changed from defaults.\n\t"
    "In case of register arrays specified by name(symbolically), specify idx to select array index.\n\t"
    "To get a register using cmic block id and address use the second option."
    "\n\t"
    "For a list of register types, use \"dump\".\n\t"
    "For direct access use \"g direct dwc_read <nof words to read> cmic_blk <cmic_blk> <REGISTER_ADDRESS>\".\n";
#endif

/*
 * Gets a memory value or register from the SOC.
 * Syntax: getreg [<regtype>] <offset|symbol>
 */
cmd_result_t
cmd_sand_reg_get(
    int unit,
    args_t * a)
{
    soc_reg_above_64_val_t regval;
    uint32 regaddr = 0, cmic_blk = 0, all_regs = 0, is_debug = 0;
    int rv = CMD_OK;
    regtype_entry_t *rt;
    soc_regaddrlist_t alist;
    char *name, *next_name;
    uint32 flags = REG_PRINT_DO_PRINT;
    int i, direct_mode = 0, dwc_read = 0, array_index = -1;
    int ram_mode = 0;

    if (0 == sh_check_attached(ARG_CMD(a), unit))
    {
        return CMD_FAIL;
    }
    bsl_log_start(BSL_APPL_SHELL, bslSeverityNormal, unit, "");
    /*
     * If first arg is a register type, take it and use the next argument
     * as the name or address, otherwise default to register type "soc."
     */
    name = ARG_GET(a);

    for (;;)
    {
        if (name != NULL && !sal_strcasecmp(name, "raw"))
        {
            flags |= REG_PRINT_RAW;
            name = ARG_GET(a);
        }
        else if (name != NULL && !sal_strcasecmp(name, "hex"))
        {
            flags |= REG_PRINT_HEX;
            name = ARG_GET(a);
        }
        else if (name != NULL && !sal_strcasecmp(name, "chg"))
        {
            flags |= REG_PRINT_CHG;
            name = ARG_GET(a);
        }
        else if (name != NULL && !sal_strcasecmp(name, "cmic_blk"))
        {
            name = ARG_GET(a);
            if (isint(name))
            {
                cmic_blk = parse_integer(name);
                name = ARG_GET(a);
            }
        }
        else if (name != NULL && !sal_strcasecmp(name, "idx"))
        {
            name = ARG_GET(a);
            if (isint(name))
            {
                array_index = parse_integer(name);
                name = ARG_GET(a);
            }
            else
            {
                cli_out("Bad format for array index\n");
                return CMD_FAIL;
            }
        }
        else if (name != NULL && !sal_strcasecmp(name, "direct"))
        {
            direct_mode = 1;
            name = ARG_GET(a);
        }
        else if (name != NULL && !sal_strcasecmp(name, "ram"))
        {
            ram_mode = 1;
            name = ARG_GET(a);

        }
        else if (name != NULL && !sal_strcasecmp(name, "dwc_read"))
        {
            name = ARG_GET(a);
            if (isint(name))
            {
                dwc_read = parse_integer(name);
                name = ARG_GET(a);
            }
        }
        else if (name != NULL && !sal_strcasecmp(name, "*"))
        {
            all_regs = 1;
            name = ARG_GET(a);
        }
        else if (name != NULL && !sal_strcasecmp(name, "debug"))
        {
            is_debug = 1;
            flags |= REG_PRINT_ADDR;
            name = ARG_GET(a);

        }
        else
        {
            break;
        }
    }

    if (1 == all_regs)
    {
        if (array_index >= 0)
        {
            cli_out("Can not access all registers with an array index\n");
            return CMD_FAIL;
        }
        return diag_sand_reg_get_all(unit, is_debug);
    }

    if (name == NULL)
    {
        return CMD_USAGE;
    }

    if ((rt = diag_sand_regtype_lookup_name(name)) != 0)
    {
        /*
         * if ((name = ARG_GET(a)) == 0) { return CMD_USAGE; }
         */
    }
    else
    {
        rt = diag_sand_regtype_lookup_name("soc");
    }
    if (0 == rt)
    {
        cli_out("Unknown register.\n");
        return (CMD_FAIL);
    }

    if (soc_regaddrlist_alloc(&alist) < 0)
    {
        cli_out("Could not allocate address list.  Memory error.\n");
        return CMD_FAIL;
    }

    if (isint(name))
    {   /* Numerical address given */
        regaddr = parse_integer(name);
        if (1 == ram_mode)
        {
            /*
             * ram_mode can be changed to 1
             */
            /*
             * coverity[dead_error_line:FALSE] 
             */
            if (array_index >= 0)
            {
                cli_out("ram mode with a specified address is not supported with an array index\n");
                rv = CMD_FAIL;
            }
            else
            {
                bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, " 0x%08x = 0x%08x\n", regaddr,
                            (*((uint32 *) (INT_TO_PTR(regaddr)))));
                rv = CMD_OK;
            }
        }
        else if (1 == direct_mode)
        {
            if (array_index >= 0)
            {
                cli_out("direct mode with a specified address is not supported with an array index\n");
                rv = CMD_FAIL;
            }
            else if (dwc_read > 20)
            {
                bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit,
                            "dwc_read max size is 20 , received %d\n", dwc_read);
                rv = CMD_FAIL;
            }
            else
            {
                uint32 get_data[20];
                rv = soc_direct_reg_get(unit, cmic_blk, regaddr, dwc_read, get_data);
                if (SOC_E_NONE != rv)
                {
                    cli_out("ERROR: read reg failed: %s\n", soc_errmsg(rv));
                    rv = CMD_FAIL;
                }
                else
                {
                    for (i = 0; i < dwc_read; i++)
                    {
                        /*
                         * bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, "0x%x[%d]=0x%x\n",
                         * regaddr+4*i, i, get_data[i]);
                         */
                        bsl_log_add(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, "0x%x[%d]=0x%x\n",
                                    regaddr + 4 * i, i, get_data[i]);
                    }
                    rv = CMD_OK;
                }
            }
        }
        else
        {
            rv = diag_sand_reg_get_by_type(unit, regaddr, cmic_blk, rt->type, regval, flags, array_index);

        }
    }
    else
    {
        if (array_index >= 0)
        {
            cli_out
                ("ind should not be used for registers specified symbolically, specify array index (or range) in the register description\n");
            rv = CMD_FAIL;
        }
        else
        {

            if (*name == '$')
            {
                name++;
            }

            /*
             * Symbolic name given, print all or some values ... 
             */
            if (parse_symbolic_reference(unit, &alist, name) < 0)
            {
                cli_out("Syntax error parsing \"%s\"\n", name);
                rv = CMD_FAIL;
            }
            else
            {
                next_name = ARG_GET(a);
                if (next_name != NULL)
                {
                    if (diag_sand_reg_print_field(unit, &alist, flags, next_name) < 0)
                    {
                        rv = CMD_FAIL;
                        cli_out("Syntax error parsing \"%s\" field\n", next_name);
                    }
                }
                else
                {
                    if (diag_sand_reg_print_all(unit, &alist, flags) < 0)
                    {
                        rv = CMD_FAIL;
                    }
                }
            }
        }
    }
    bsl_log_end(BSL_APPL_SHELL, bslSeverityNormal, bslSinkIgnore, unit, "");

    soc_regaddrlist_free(&alist);
    return rv;
}

/*
 * Set a register by type.  For SOC registers, is64 is used to
 * indicate if this is a 64 bit register.  Otherwise, is64 is
 * ignored.
 *
 * array_index should be the array index for register arrays, or negative for non array registers
 *
 */
STATIC int
diag_sand_reg_set_by_type(
    int unit,
    uint32 regaddr,
    uint32 cmic_blk,
    soc_regtype_t regtype,
    soc_reg_above_64_val_t regval,
    int array_index)
{
    int rv = CMD_OK, r;
    soc_regaddrinfo_t ainfo;

    switch (regtype)
    {
        case soc_genreg:
        case soc_portreg:
            diag_sand_reg_addrinfo_get(unit, &ainfo, regaddr, cmic_blk);

            if (!ainfo.valid)
            {
                cli_out("ERROR: internal error getting reginfo for reg at %ux \n", regaddr);
                rv = CMD_FAIL;
                break;
            }

            if (array_index >= 0)
            {
                /*
                 * non zero array index, implying this is a register array 
                 */
                if (!SOC_REG_IS_ARRAY(unit, ainfo.reg))
                {
                    cli_out("ERROR: can not access a non array register with an array index\n");
                    return CMD_FAIL;
                }
                else if (array_index >= SOC_REG_NUMELS(unit, ainfo.reg))
                {
                    cli_out("ERROR: array index %d is out of range, should be under %d\n",
                            array_index, SOC_REG_NUMELS(unit, ainfo.reg));
                    return CMD_FAIL;
                }
            }
            else
            {
                /*
                 * an array index was not specified 
                 */
                if (SOC_REG_IS_ARRAY(unit, ainfo.reg))
                {
                    cli_out("ERROR: can not access a register array without an array index\n");
                    return CMD_FAIL;
                }
                array_index = 0;        /* for accessing non array registers */
            }

            r = soc_reg_above_64_set(unit, ainfo.reg, ainfo.idx, array_index, regval);

            if (r < 0)
            {
                cli_out("ERROR: write reg failed: %s\n", soc_errmsg(r));
                rv = CMD_FAIL;
            }
            break;

        default:
            cli_out("ERROR: Writing to un-supported regtype(%d)\n", regtype);
            rv = CMD_FAIL;
            break;
    }

    return rv;
}

/*
 * diag_sand_modify_reg_fields
 *
 *   Takes a soc_reg_t 'regno', pointer to current value 'val',
 *   and a string 'mod' containing a field replacement spec of the form
 *   "FIELD=value".   The string may contain more than one spec separated
 *   by commas.  Updates the field in the register value accordingly,
 *   leaving all other unmodified fields intact.  If the string CLEAR is
 *   present in the list, the current register value is zeroed.
 *   If mask is non-NULL, it receives a mask of all fields modified.
 *
 *   Examples with modreg:
 *        modreg fe_mac1 lback=1        (modifies only lback field)
 *        modreg config ip_cfg=2        (modifies only ip_cfg field)
 *        modreg config clear,ip_cfg=2,cpu_pri=4  (zeroes all other fields)
 *
 *   Note that if "clear" appears in the middle of the list, the
 *   values in the list before "clear" are ignored.
 *
 *   Returns -1 on failure, 0 on success.
 */

STATIC int
diag_sand_modify_reg_fields(
    int unit,
    soc_reg_t regno,
    soc_reg_above_64_val_t val,
    soc_reg_above_64_val_t * mask /* out param */ ,
    char *mod)
{
    soc_field_info_t *fld;
    char *fmod, *fval;
    char *modstr;
    soc_reg_info_t *reg = &SOC_REG_INFO(unit, regno);
    soc_reg_above_64_val_t fvalue, tmask, fldmask;
    char *tokstr;

    if ((modstr = sal_alloc(ARGS_BUFFER, "modify_reg")) == NULL)
    {
        cli_out("diag_sand_modify_reg_fields: Out of memory\n");
        return CMD_FAIL;
    }

    strncpy(modstr, mod, ARGS_BUFFER);  /* Don't destroy input string */
    modstr[ARGS_BUFFER - 1] = 0;
    mod = modstr;

    if (mask)
    {
        SOC_REG_ABOVE_64_CLEAR(*mask);
    }

    while ((fmod = sal_strtok_r(mod, ",", &tokstr)) != 0)
    {
        mod = NULL;     /* Pass strtok NULL next time */
        fval = strchr(fmod, '=');
        if (fval)
        {       /* Point fval to arg, NULL if none */
            *fval++ = 0;        /* Now fmod holds only field name. */
        }
        if (fmod[0] == 0)
        {
            cli_out("Null field name\n");
            sal_free(modstr);
            return -1;
        }
        if (!sal_strcasecmp(fmod, "clear"))
        {
            SOC_REG_ABOVE_64_CLEAR(val);
            if (mask)
            {
                SOC_REG_ABOVE_64_ALLONES(*mask);
            }
            continue;
        }
        for (fld = &reg->fields[0]; fld < &reg->fields[reg->nFields]; fld++)
        {
            if (!sal_strcasecmp(fmod, SOC_FIELD_NAME(unit, fld->field)))
            {
                break;
            }
        }
        if (fld == &reg->fields[reg->nFields])
        {
            cli_out("No such field \"%s\" in register \"%s\".\n", fmod, SOC_REG_NAME(unit, regno));
            sal_free(modstr);
            return -1;
        }
        if (!fval)
        {
            cli_out("Missing %d-bit value to assign to \"%s\" field \"%s\".\n",
                    fld->len, SOC_REG_NAME(unit, regno), SOC_FIELD_NAME(unit, fld->field));
            sal_free(modstr);
            return -1;
        }
        parse_long_integer(fvalue, SOC_REG_ABOVE_64_MAX_SIZE_U32, fval);

        /*
         * Check that field value fits in field 
         */
        SOC_REG_ABOVE_64_CREATE_MASK(tmask, fld->len, 0)
            SOC_REG_ABOVE_64_NOT(tmask) SOC_REG_ABOVE_64_AND(tmask, fvalue) if (!SOC_REG_ABOVE_64_IS_ZERO(tmask))
        {
            cli_out("Value \"%s\" too large for %d-bit field \"%s\".\n",
                    fval, fld->len, SOC_FIELD_NAME(unit, fld->field));
            sal_free(modstr);
            return -1;
        }

        soc_reg_above_64_field_set(unit, regno, val, fld->field, fvalue);

        SOC_REG_ABOVE_64_CREATE_MASK(fldmask, fld->len, fld->bp);
        if (mask)
        {
            SOC_REG_ABOVE_64_OR(*mask, fldmask);
        }
    }

    sal_free(modstr);
    return 0;
}

/*
 * Auxilliary routine to handle setreg and modreg.
 *      mod should be 0 for setreg, which takes either a value or a
 *              list of <field>=<value>, and in the latter case, sets all
 *              non-specified fields to zero.
 *      mod should be 1 for modreg, which does a read-modify-write of
 *              the register and permits value to be specified by a list
 *              of <field>=<value>[,...] only.
 */
STATIC cmd_result_t
diag_sand_do_reg_set(
    int unit,
    args_t * a,
    int mod)
{
    soc_reg_above_64_val_t regval;
    uint32 regaddr = 0, cmic_blk = 0, addr;
    int rv = CMD_OK, i, blk;
    regtype_entry_t *rt;
    soc_regaddrlist_t alist = { 0, NULL };
    soc_regaddrinfo_t *ainfo;
    char *name;
    char *s, *valstr = NULL;
    uint8 acc_type;
    uint32 data[20];
    int dwc_write = 0, direct_mode = 0, array_index = -1;

    SOC_REG_ABOVE_64_ALLONES(regval);

    if (0 == sh_check_attached(ARG_CMD(a), unit))
    {
        return CMD_FAIL;
    }

    if ((name = ARG_GET(a)) == 0)
    {
        return CMD_USAGE;
    }

    for (;;)
    {
        if (name != NULL && !sal_strcasecmp(name, "cmic_blk"))
        {
            name = ARG_GET(a);
            if (isint(name))
            {
                cmic_blk = parse_integer(name);
                name = ARG_GET(a);
            }
        }
        else if (name != NULL && !sal_strcasecmp(name, "direct"))
        {
            direct_mode = 1;
            name = ARG_GET(a);
        }
        else if (name != NULL && !sal_strcasecmp(name, "data"))
        {
            while (0 != sal_strcasecmp(name, "dataend"))
            {
                if (isint(name))
                {
                    data[dwc_write] = parse_integer(name);
                    dwc_write++;
                    if (dwc_write >= 20)
                    {
                        cli_out("Error: Data is longer than 20\n");
                        return CMD_FAIL;
                    }
                }
                name = ARG_GET(a);
            }
            name = ARG_GET(a);
        }
        else if (name != NULL && !sal_strcasecmp(name, "idx"))
        {
            name = ARG_GET(a);
            if (isint(name))
            {
                array_index = parse_integer(name);
                name = ARG_GET(a);
            }
            else
            {
                cli_out("Bad format for array index\n");
                return CMD_FAIL;
            }
        }
        else
        {
            break;
        }
    }

    if (name == NULL)
    {
        return CMD_USAGE;
    }
    /*
     * The value of direct_mode may be changed from 0 to 1 
     */
    /*
     * coverity[dead_error_line:FALSE] 
     */
    if (1 == direct_mode && isint(name))
    {
        if (array_index >= 0)
        {
            cli_out("direct mode with a specified address is not supported with an array index\n");
            return CMD_FAIL;
        }
        regaddr = parse_integer(name);
        rv = soc_direct_reg_set(unit, cmic_blk, regaddr, dwc_write, data);
        if (SOC_E_NONE != rv)
        {
            cli_out("ERROR: write reg failed: %s\n", soc_errmsg(rv));
            return CMD_FAIL;
        }
        else
        {
            return CMD_OK;
        }
    }

    /*
     * If first arg is an access type, take it and use the next argument
     * as the name, otherwise use default access type.
     * modreg command does not allow this and assumes soc.
     */

    if ((0 == mod) && (rt = diag_sand_regtype_lookup_name(name)) != 0)
    {
        /*
         * if ((name = ARG_GET(a)) == 0) { return CMD_USAGE; }
         */
    }
    else
    {
        rt = diag_sand_regtype_lookup_name("soc");
        if (0 == rt)
        {
            return CMD_FAIL;
        }
    }

    /*
     * Parse the value field.  If there are more than one, string them
     * together with commas separating them (to make field-based setreg
     * inputs more convenient).
     */

    if ((s = ARG_GET(a)) == 0)
    {
        cli_out("Syntax error: missing value\n");
        return CMD_USAGE;
    }

    if ((valstr = sal_alloc(ARGS_BUFFER, "reg_set")) == NULL)
    {
        cli_out("diag_sand_do_reg_set: Out of memory\n");
        return CMD_FAIL;
    }

    diag_sand_utils_collect_comma_args(a, valstr, s);

    if (mod && isint(valstr))
    {
        sal_free(valstr);
        return CMD_USAGE;
    }

    if (soc_regaddrlist_alloc(&alist) < 0)
    {
        cli_out("Could not allocate address list.  Memory error.\n");
        sal_free(valstr);
        return CMD_FAIL;
    }

    if (!mod && isint(name))
    {
        /*
         * Numerical address given 
         */
        regaddr = parse_integer(name);
        parse_long_integer(regval, SOC_REG_ABOVE_64_MAX_SIZE_U32, valstr);
        rv = diag_sand_reg_set_by_type(unit, regaddr, cmic_blk, rt->type, regval, array_index);
    }
    else
    {   /* Symbolic name given, set all or some values ... */
        if (*name == '$')
        {
            name++;
        }

        if (array_index >= 0)
        {
            cli_out
                ("ind should not be used for registers specified symbolically, specify array index (or range) in the register description\n");
            return CMD_FAIL;
        }

        if (parse_symbolic_reference(unit, &alist, name) < 0)
        {

            cli_out("Syntax error parsing \"%s\"\n", name);
            sal_free(valstr);
            soc_regaddrlist_free(&alist);
            return CMD_FAIL;
        }
        else
        {
            if (isint(valstr))
            {   /* valstr is numeric */
                parse_long_integer(regval, SOC_REG_ABOVE_64_MAX_SIZE_U32, valstr);
            }

            for (i = 0; i < alist.count; i++)
            {
                ainfo = &alist.ainfo[i];

                /*
                 * alist now holds list of registers to change 
                 */
                if (!isint(valstr))
                {       /* Must modify registers */
                    /*
                     * valstr must be a field replacement spec.
                     * In modreg mode, read the current register value,
                     * and modify it.  In setreg mode,
                     * assume a starting value of zero and modify it.
                     */
                    if (mod)
                    {
                        rv = CMD_OK;
                        if (soc_cpureg == SOC_REG_INFO(unit, ainfo->reg).regtype)
                        {
                            addr =
                                soc_reg_addr_get(unit, ainfo->reg, ainfo->block, ainfo->idx, SOC_REG_ADDR_OPTION_WRITE,
                                                 &blk, &acc_type);
                            regval[0] = soc_pci_read(unit, addr);
                        }
#ifdef BCM_IPROC_SUPPORT
                        else if (soc_iprocreg == SOC_REG_INFO(unit, ainfo->reg).regtype)
                        {
                            addr =
                                soc_reg_addr_get(unit, ainfo->reg, ainfo->block, ainfo->idx, SOC_REG_ADDR_OPTION_WRITE,
                                                 &blk, &acc_type);
                            soc_iproc_getreg(unit, addr, regval);
                        }
#endif /* BCM_IPROC_SUPPORT */
#ifdef BCM_CMICM_SUPPORT
                        else if (soc_mcsreg == SOC_REG_INFO(unit, ainfo->reg).regtype)
                        {
                            addr =
                                soc_reg_addr_get(unit, ainfo->reg, ainfo->block, ainfo->idx, SOC_REG_ADDR_OPTION_WRITE,
                                                 &blk, &acc_type);
                            soc_pci_mcs_getreg(unit, addr, regval);
                            continue;
                        }
#endif
                        else if (ainfo->is_custom)
                        {
                            rv = soc_custom_reg_above_64_get(unit, ainfo->reg, ainfo->port,
                                                             SOC_REG_IS_ARRAY(unit, ainfo->reg) ? ainfo->idx : 0,
                                                             regval);
                        }
                        else
                        {
                            rv = soc_reg_above_64_get(unit, ainfo->reg, ainfo->port,
                                                      SOC_REG_IS_ARRAY(unit, ainfo->reg) ? ainfo->idx : 0, regval);
                        }
                    }
                    else
                    {
                        SOC_REG_ABOVE_64_CLEAR(regval);
                    }

                    if (rv == CMD_OK)
                    {
                        if ((rv = diag_sand_modify_reg_fields(unit, ainfo->reg, regval,
                                                              (soc_reg_above_64_val_t *) 0, valstr)) < 0)
                        {
                            cli_out("Syntax error, aborted\n");
                        }
                    }
                }

                if (soc_cpureg == SOC_REG_INFO(unit, ainfo->reg).regtype)
                {
                    addr =
                        soc_reg_addr_get(unit, ainfo->reg, ainfo->block, ainfo->idx, SOC_REG_ADDR_OPTION_WRITE, &blk,
                                         &acc_type);
                    soc_pci_write(unit, addr, regval[0]);       /* start dma transaction */
                    continue;
                }
#ifdef BCM_IPROC_SUPPORT
                else if (soc_iprocreg == SOC_REG_INFO(unit, ainfo->reg).regtype)
                {
                    addr =
                        soc_reg_addr_get(unit, ainfo->reg, ainfo->block, ainfo->idx, SOC_REG_ADDR_OPTION_WRITE, &blk,
                                         &acc_type);
                    soc_iproc_setreg(unit, addr, regval[0]);
                    continue;
                }
#endif /* BCM_IPROC_SUPPORT */
#ifdef BCM_CMICM_SUPPORT
                else if (soc_mcsreg == SOC_REG_INFO(unit, ainfo->reg).regtype)
                {
                    addr =
                        soc_reg_addr_get(unit, ainfo->reg, ainfo->block, ainfo->idx, SOC_REG_ADDR_OPTION_WRITE, &blk,
                                         &acc_type);
                    soc_pci_mcs_write(unit, addr, regval[0]);   /* start dma transaction */
                    continue;

                }
#endif
                else if (soc_customreg == SOC_REG_INFO(unit, ainfo->reg).regtype)
                {
                    rv = soc_custom_reg_above_64_set(unit, ainfo->reg, ainfo->port,
                                                     SOC_REG_IS_ARRAY(unit, ainfo->reg) ? ainfo->idx : 0, regval);
                }
                else if (rv == CMD_OK)
                {
                    rv = soc_reg_above_64_set(unit, ainfo->reg, ainfo->port,
                                              SOC_REG_IS_ARRAY(unit, ainfo->reg) ? ainfo->idx : 0, regval);
                }

            }
        }
    }

    sal_free(valstr);
    soc_regaddrlist_free(&alist);

    return rv;
}

char cmd_sand_reg_set_usage[] =
#ifdef COMPILER_STRING_CONST_LIMIT
    "1. Parameters: [<REGTYPE>] [idx <array_index>] <REGISTER> <VALUE>\n\t"
    "If <REGTYPE> is not specified, it defaults to \"soc\".\n\t"
    "<REGISTER> is offset, address, or symbolic name.\n"
    "2. Parameters: [idx <array_index>] <REGISTER> <FIELD>=<VALUE>[,...]\n\t"
    "<REGISTER> is SOC register offset or symbolic name.\n\t"
    "<FIELD>=<VALUE>[,...] is a list of fields to affect,\n\t"
    "3. Parameters: [<REGTYPE>] cmic_blk <cmic_blk> <REGISTER_ADDRESS> "
    "<VALUE>\n\t" "Full documentation cannot be displayed with -pendantic compiler\n";
#else
    "1. Parameters: [<REGTYPE>] [idx <array_index>] <REGISTER> <VALUE>\n\t"
    "If <REGTYPE> is not specified, it defaults to \"soc\".\n\t"
    "<REGISTER> is offset, address, or symbolic name.\n"
    "2. Parameters: [idx <array_index>] <REGISTER> <FIELD>=<VALUE>[,...]\n\t"
    "<REGISTER> is SOC register offset or symbolic name.\n\t"
    "<FIELD>=<VALUE>[,...] is a list of fields to affect,\n\t"
    "for example: L3_ENA=0,CPU_PRI=1.\n\t"
    "3. Parameters: [<REGTYPE>] cmic_blk <cmic_blk> <REGISTER_ADDRESS> "
    "<VALUE>\n\t"
    "Fields not specified in the list are set to zero.\n\t"
    "In case of register arrays specified by name(symbolically), specify idx to select array index.\n\t"
    "To get a register using cmic block id and address use the third option."
    "\n\t" "For a list of register types, use \"help dump\".\n";
#endif
/*
 * Sets a memory value or register on the SOC. Syntax 1: setreg [<regtype>] <offset|symbol> <value> Syntax 2: setreg
 * [<regtype>] <offset|symbol> <field>=<value>[,...] 
 */
cmd_result_t
cmd_sand_reg_set(
    int unit,
    args_t * a)
{
    return diag_sand_do_reg_set(unit, a, 0);
}

char cmd_sand_reg_modify_usage[] =
    "Parameters: <REGISTER> <FIELD>=<VALUE>[,...]\n\t"
    "<REGISTER> is SOC register symbolic name.\n\t"
    "<FIELD>=<VALUE>[,...] is a list of fields to affect,\n\t"
    "for example: L3_ENA=0,CPU_PRI=1.\n\t"
    "Fields not specified in the list are left unaffected.\n\t" "For a list of register types, use \"dump\".\n";

/*
 * Read/modify/write a memory value or register on the SOC.
 * Syntax: modreg [<regtype>] <offset|symbol> <field>=<value>[,...]
 */
cmd_result_t
cmd_sand_reg_modify(
    int unit,
    args_t * a)
{
    return diag_sand_do_reg_set(unit, a, 1);
}
