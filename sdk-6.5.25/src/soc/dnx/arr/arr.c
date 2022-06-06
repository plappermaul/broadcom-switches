/** \file arr.c
 * 
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2021 Broadcom Inc. All rights reserved.
 */

#ifdef BSL_LOG_MODULE
#error "BSL_LOG_MODULE redefined"
#endif
#define BSL_LOG_MODULE BSL_LS_SOCDNX_DIAG

#include <sal/appl/sal.h>
#include <soc/drv.h>
#include <shared/shrextend/shrextend_debug.h>
#include <shared/utilex/utilex_rhlist.h>
#include <shared/utilex/utilex_str.h>
#include <shared/dbx/dbx_xml.h>
#include <shared/dbx/dbx_file.h>
#include <soc/sand/sand_signals.h>
#include <soc/dnx/dbal/dbal.h>
#include "src/soc/dnx/dbal/dbal_internal.h"

#include <soc/dnx/arr/arr.h>
#include <soc/dnx/dnx_data/auto_generated/dnx_data_pp.h>
#include <soc/dnx/dnx_data/auto_generated/dnx_data_mpls.h>

#define ARR_EG_TYPE_FIELD_SIZE_BITS               (dnx_data_pp.ETPP.etps_type_size_bits_get(unit))

#define ARR_EG_TYPE_FIELD_OFFSET_MSB              0

typedef enum
{
    ARR_DECODE_NONE,
    ARR_DECODE_MPLS,
    ARR_DECODE_SRV6,
    ARR_DECODE_IPV6,
    ARR_DECODE_IPV6_o_SRV6
} arr_decode_data_type_e;

typedef struct
{
    arr_decode_data_type_e decode_type;
    int nof_data_entries;
    int opaque;
    char from_stage[RHNAME_MAX_SIZE];
    char to_stage[RHNAME_MAX_SIZE];
} arr_decode_hitory_t;

extern shr_error_e dbal_mdb_table_id_and_result_type_from_etps_format_get(
    int unit,
    uint32 etps_format,
    dbal_tables_e * table_id,
    int *result_type);

static shr_error_e
dnx_etps_arr_decode_field_value_get(
    int unit,
    uint32 *data,
    uint32 data_size,
    int field_offset_msb,
    int field_size,
    uint32 *val)
{
    SHR_FUNC_INIT_VARS(unit);

    if (field_offset_msb + field_size < data_size)
    {
        SHR_BITCOPY_RANGE(val, 0, data, data_size - field_offset_msb - field_size, field_size);
    }
    else
    {
        SHR_BITCOPY_RANGE(val, 0, data, 0, field_size);
    }
    SHR_EXIT();
exit:
    SHR_FUNC_EXIT;
}

static shr_error_e
dnx_etps_arr_update_history(
    int unit,
    uint32 data_type,
    dbal_tables_e table_id,
    dbal_fields_e field_id,
    uint32 field_value,
    arr_decode_hitory_t * arr_history)
{

    SHR_FUNC_INIT_VARS(unit);

    if ((table_id == DBAL_TABLE_EEDB_SRV6) && (field_id == DBAL_FIELD_NOF_SIDS) && (field_value > 0))
    {
        if (arr_history->decode_type == ARR_DECODE_IPV6)
        {
            arr_history->decode_type = ARR_DECODE_IPV6_o_SRV6;
            arr_history->nof_data_entries = arr_history->opaque + field_value;
        }
        else
        {
            arr_history->decode_type = ARR_DECODE_SRV6;
            arr_history->opaque = 0;
            arr_history->nof_data_entries = field_value;
        }
    }
    else if (table_id == DBAL_TABLE_EEDB_IPV6_TUNNEL)
    {
        arr_history->decode_type = ARR_DECODE_IPV6;
        if (field_id == DBAL_FIELD_HAS_DIP_LL)
        {
            arr_history->opaque = field_value;
        }
        arr_history->nof_data_entries = 1;
    }

    SHR_EXIT();

exit:
    SHR_FUNC_EXIT;
}

shr_error_e
dnx_etps_arr_table_id_and_data_type_get(
    int unit,
    arr_decode_hitory_t arr_history,
    dbal_tables_e * table_id,
    int *hw_result_type)
{

    SHR_FUNC_INIT_VARS(unit);

    switch (arr_history.decode_type)
    {
        case ARR_DECODE_MPLS:
            break;
        case ARR_DECODE_IPV6:

        {
            *table_id = DBAL_TABLE_EEDB_IPV6_DIP;
            *hw_result_type = DBAL_RESULT_TYPE_EEDB_IPV6_DIP_ETPS_DATA_128B;
        }
            break;
        case ARR_DECODE_SRV6:

        {
            *table_id = DBAL_TABLE_EEDB_SRV6_SID;
            *hw_result_type = DBAL_RESULT_TYPE_EEDB_SRV6_SID_ETPS_DATA_128B;
        }
            break;
        case ARR_DECODE_IPV6_o_SRV6:

        {
            if ((arr_history.nof_data_entries == 1) && (arr_history.opaque == 1))
            {
                *table_id = DBAL_TABLE_EEDB_IPV6_DIP;
                *hw_result_type = DBAL_RESULT_TYPE_EEDB_IPV6_DIP_ETPS_DATA_128B;
            }
            else
            {
                *table_id = DBAL_TABLE_EEDB_SRV6_SID;
                *hw_result_type = DBAL_RESULT_TYPE_EEDB_SRV6_SID_ETPS_DATA_128B;
            }
        }
            break;
        default:
            SHR_ERR_EXIT(_SHR_E_CONFIG, "Mssing data entry parsing configuration.\n");
    }

exit:
    SHR_FUNC_EXIT;
}

shr_error_e
dnx_etps_arr_decode(
    int unit,
    int core,
    char *desc,
    uint32 *data,
    uint32 data_size_bits,
    rhlist_t ** parsed_info,
    char from_n[RHNAME_MAX_SIZE],
    char to_n[RHNAME_MAX_SIZE])
{
    uint32 data_type = 0;
    dbal_tables_e table_id;
    int result_type;
    CONST dbal_logical_table_t *table;
    int field_idx;
    int field_hw_offset = 0;

    static arr_decode_hitory_t arr_history = { 0 };

    SHR_FUNC_INIT_VARS(unit);

    SHR_NULL_CHECK(data, _SHR_E_PARAM, "data");
    SHR_NULL_CHECK(parsed_info, _SHR_E_PARAM, "parsed_info");
    SHR_RANGE_VERIFY(data_size_bits, ARR_EG_TYPE_FIELD_SIZE_BITS, DSIG_FIELD_MAX_SIZE_BITES, _SHR_E_PARAM,
                     "Data size must be at least %d bits long to parse the data type, and at most %d bits long to fit"
                     " in the output structure.", ARR_EG_TYPE_FIELD_SIZE_BITS, DSIG_FIELD_MAX_SIZE_BITES);

    if ((from_n == NULL) || (to_n == NULL) || (arr_history.nof_data_entries == 0))
    {
        if (from_n != NULL)
        {
            sal_strncpy(arr_history.from_stage, from_n, RHNAME_MAX_SIZE - 1);
        }
        if (to_n != NULL)
        {
            sal_strncpy(arr_history.to_stage, to_n, RHNAME_MAX_SIZE - 1);
        }
        arr_history.decode_type = ARR_DECODE_NONE;
        arr_history.nof_data_entries = 0;
        arr_history.opaque = 0;
    }

    SHR_IF_ERR_EXIT(dnx_etps_arr_decode_data_type_get(unit, data, data_size_bits, &data_type));

    SHR_IF_ERR_EXIT(dbal_mdb_table_id_and_result_type_from_etps_format_get(unit, data_type, &table_id, &result_type));

    if ((arr_history.decode_type != ARR_DECODE_NONE)
        && !((arr_history.decode_type == ARR_DECODE_IPV6) && (table_id == DBAL_TABLE_EEDB_SRV6)))
    {
        data_type = 0;
        SHR_IF_ERR_EXIT(dnx_etps_arr_table_id_and_data_type_get(unit, arr_history, &table_id, &result_type));
    }
    else
    {

        if (data_type == 0)
        {
            SHR_EXIT();
        }
    }

    if (NULL == *parsed_info)
    {
        if ((*parsed_info = utilex_rhlist_create("Arr information", sizeof(signal_output_t), 0)) == NULL)
        {
            SHR_ERR_EXIT(_SHR_E_RESOURCE, "Could not allocate parsed information list to parse the required buffer.\n");
        }
    }

    if (table_id == DBAL_NOF_TABLES)
    {

        uint32 raw_etps_format_min, raw_etps_format_max;
        raw_etps_format_min = 0x3 << (dnx_data_pp.ETPP.etps_type_size_bits_get(unit) - 2);
        raw_etps_format_max = (1 << dnx_data_pp.ETPP.etps_type_size_bits_get(unit)) - 1;
        if ((data_type < raw_etps_format_min) || (data_type > raw_etps_format_max))
        {
            SHR_ERR_EXIT(_SHR_E_PARAM,
                         "Undefined result type provided. HW value %d mapped to %d without corresponding table\n",
                         data_type, result_type);
        }
        else
        {
            rhhandle_t fld_handle;
            signal_output_t *field_sig;

            SHR_IF_ERR_EXIT(utilex_rhlist_entry_add_tail
                            (*parsed_info, "data_entry", RHID_TO_BE_GENERATED, &fld_handle));

            field_sig = fld_handle;
            sand_signal_value_to_str(unit, SAL_FIELD_TYPE_NONE, data, field_sig->print_value, data_size_bits,
                                     PRINT_BIG_ENDIAN);
            field_sig->size = data_size_bits;

            SHR_EXIT();
        }
    }

    SHR_IF_ERR_EXIT(dbal_tables_table_get(unit, table_id, &table));

    for (field_idx = 0; field_idx < table->multi_res_info[result_type].nof_result_fields; field_idx++)
    {
        dbal_fields_e field_id;
        int field_nof_bits;
        uint32 arr_prefix[1];
        uint8 arr_prefix_size;
        rhhandle_t fld_handle;
        char field_name_string[RHNAME_MAX_SIZE];
        signal_output_t *field_sig;

        field_id = table->multi_res_info[result_type].results_info[field_idx].field_id;
        field_nof_bits = table->multi_res_info[result_type].results_info[field_idx].field_nof_bits;
        arr_prefix[0] = table->multi_res_info[result_type].results_info[field_idx].arr_prefix;
        arr_prefix_size = table->multi_res_info[result_type].results_info[field_idx].arr_prefix_size;

        sal_snprintf(field_name_string, RHNAME_MAX_SIZE - 1, "%s", dbal_field_to_string(unit, field_id));

        SHR_IF_ERR_EXIT(utilex_rhlist_entry_add_tail
                        (*parsed_info, field_name_string, RHID_TO_BE_GENERATED, &fld_handle));
        field_sig = fld_handle;

        SHR_IF_ERR_EXIT(dnx_etps_arr_decode_field_value_get
                        (unit, data, data_size_bits, field_hw_offset, field_nof_bits, field_sig->value));

        {

            field_hw_offset += field_nof_bits;
        }

        SHR_IF_ERR_EXIT(dnx_etps_arr_update_history
                        (unit, data_type, table_id, field_id, field_sig->value[0], &arr_history));

        if (field_id == DBAL_FIELD_RESULT_TYPE)
        {
            sal_snprintf(field_sig->print_value, RHNAME_MAX_SIZE, "%s(0x%02x)",
                         table->multi_res_info[result_type].result_type_name, data_type);
            field_sig->size = field_nof_bits;
        }
        else if (arr_prefix[0] == 0)
        {
            sand_signal_value_to_str(unit, SAL_FIELD_TYPE_NONE, field_sig->value, field_sig->print_value,
                                     field_nof_bits, PRINT_BIG_ENDIAN);
            field_sig->size = field_nof_bits;
        }
        else
        {

            rhhandle_t fld_value_handle, fld_prefix_handle;
            signal_output_t *field_value_sig;
            signal_output_t *field_prefix_sig;
            uint32 field_full_value[1];

            field_full_value[0] = (arr_prefix[0] << field_nof_bits) | field_sig->value[0];
            field_sig->size = field_nof_bits + arr_prefix_size;
            sand_signal_value_to_str(unit, SAL_FIELD_TYPE_NONE, field_full_value, field_sig->print_value,
                                     field_sig->size, PRINT_BIG_ENDIAN);

            if (field_sig->field_list == NULL)
            {
                if ((field_sig->field_list =
                     utilex_rhlist_create("field information", sizeof(signal_output_t), 0)) == NULL)
                {
                    SHR_ERR_EXIT(_SHR_E_RESOURCE,
                                 "Could not allocate parsed information list to parse the required buffer.\n");
                }
            }

            SHR_IF_ERR_EXIT(utilex_rhlist_entry_add_tail
                            (field_sig->field_list, "Value", RHID_TO_BE_GENERATED, &fld_value_handle));
            field_value_sig = fld_value_handle;
            sand_signal_value_to_str(unit, SAL_FIELD_TYPE_NONE, field_sig->value, field_value_sig->print_value,
                                     field_nof_bits, PRINT_BIG_ENDIAN);
            field_value_sig->size = field_nof_bits;

            SHR_IF_ERR_EXIT(utilex_rhlist_entry_add_tail
                            (field_sig->field_list, "expected_prefix", RHID_TO_BE_GENERATED, &fld_prefix_handle));
            field_prefix_sig = fld_prefix_handle;
            sand_signal_value_to_str(unit, SAL_FIELD_TYPE_NONE, arr_prefix, field_prefix_sig->print_value,
                                     arr_prefix_size, PRINT_BIG_ENDIAN);
            field_prefix_sig->size = arr_prefix_size;
        }
    }

    if ((arr_history.decode_type != ARR_DECODE_NONE) && (data_type == 0))
    {
        arr_history.nof_data_entries--;
    }

exit:
    SHR_FUNC_EXIT;
}

shr_error_e
dnx_etps_arr_decode_data_type_get(
    int unit,
    uint32 *data,
    uint32 size,
    uint32 *data_type)
{
    SHR_FUNC_INIT_VARS(unit);
    SHR_NULL_CHECK(data, _SHR_E_PARAM, "data");
    SHR_NULL_CHECK(data_type, _SHR_E_PARAM, "data_type");
    SHR_RANGE_VERIFY(size, ARR_EG_TYPE_FIELD_SIZE_BITS, DSIG_FIELD_MAX_SIZE_BITES, _SHR_E_PARAM,
                     "Data size must be at least %d bits long to parse the data type, and at most %d bits long to fit"
                     " in the output structure.", ARR_EG_TYPE_FIELD_SIZE_BITS, DSIG_FIELD_MAX_SIZE_BITES);
    *data_type = 0;
    SHR_IF_ERR_EXIT(dnx_etps_arr_decode_field_value_get(unit, data, size, ARR_EG_TYPE_FIELD_OFFSET_MSB,
                                                        ARR_EG_TYPE_FIELD_SIZE_BITS, data_type));
exit:
    SHR_FUNC_EXIT;
}
