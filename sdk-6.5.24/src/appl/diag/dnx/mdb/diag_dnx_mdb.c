/**
 * \file diag_dnx_mdb.c Contains all of the MDB diag commands
 */
/*
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2021 Broadcom Inc. All rights reserved.
 */

#ifdef BSL_LOG_MODULE
#error "BSL_LOG_MODULE redefined"
#endif
#define BSL_LOG_MODULE BSL_LS_SOCDNX_MDBDNX
#include <shared/bsl.h>

/*************
 * INCLUDES  *
 */
#include <soc/dnx/mdb.h>
#include <soc/dnx/mdb_internal_shared.h>
#include <soc/dnx/dbal/dbal_string_mgmt.h>
#include <soc/dnx/swstate/auto_generated/types/mdb_em_types.h>
#include <soc/dnx/dnx_data/auto_generated/dnx_data_l3.h>
#include <shared/dbx/dbx_xml.h>
#include <shared/dbx/dbx_file.h>
#include <sal/appl/sal.h>
#include <shared/bslnames.h>
#include <soc/dnx/kbp/kbp_common.h>
#include <soc/dnx/utils/dnx_sbusdma_desc.h>
/** allow drv.h include excplictly */
#ifdef _SOC_DRV_H
#undef _SOC_DRV_H
#endif
#include <soc/drv.h> /** needed for access */
#include <bcm/l3.h>
#include <bcm_int/dnx/cmn/dnxcmn.h>
#include <appl/diag/diag.h>

#include <src/soc/dnx/dbal/dbal_internal.h>

#include <appl/diag/sand/diag_sand_prt.h>
#include <appl/diag/sand/diag_sand_framework.h>
#include <shared/utilex/utilex_bitstream.h>

#include "diag_dnx_mdb.h"

/*************
 * DEFINES   *
 */

extern shr_error_e dbal_tables_table_by_mdb_phy_get(
    int unit,
    dbal_physical_tables_e phy_db_id,
    uint32 app_db_id,
    dbal_tables_e * table_id);

#define MDB_DB_TYPE_IS_VALID(_db_type) ((_db_type != DBAL_ENUM_FVAL_MDB_DB_TYPE_TCAM)     \
                                        && (_db_type != DBAL_ENUM_FVAL_MDB_DB_TYPE_NONE)  \
                                        && (_db_type != DBAL_NOF_ENUM_MDB_DB_TYPE_VALUES))


/*************
* FUNCTIONS *
 */

static sh_sand_man_t dnx_mdb_info_man = {
    .brief = "Dump all DBAL Physical tables names or print information for a specific table",
    .full = "Prints all of the DBAL physical table names if not given a table name, "
        "otherwise prints: table type [Direct, MDB, LPM, TCAM], Address resolution, capacity "
        "allocated clusters {mem, block, array_index, offset within bucket).",
    .synopsis = "[table=DBAL_PHYSICAL_TABLE_NAME]",
    .examples = "table=ISEM1\n" "table=EEDB1\n" "table=KAPS1\n" "table=MAP\n" "Clusters\n" "Capacity\n"
};

static sh_sand_option_t dnx_mdb_info_options[] = {
    {"Table", SAL_FIELD_TYPE_STR, "DBAL physical table name", ""},
    {"Clusters", SAL_FIELD_TYPE_BOOL, "Print cluster to table allocation", "no"},
    {"Capacity", SAL_FIELD_TYPE_BOOL, "Print a summary of the mdb capacity", "no"},
    {NULL}
};

/**
 * defualt unknown string
 */
static char *mdb_unknown_string = "Unknown ID";

/**
 * Strings mapping array for MDB access types:
 */
static char *mdb_db_types_strings[DBAL_NOF_ENUM_MDB_DB_TYPE_VALUES] = {
    "Exact Match",
    "Direct Access",
    "TCAM",
    "KAPS (LPM)",
    "EEDB"
};

char *
mdb_get_db_type_str(
    dbal_enum_value_field_mdb_db_type_e mdb_db_type)
{
    if (mdb_db_type >= DBAL_NOF_ENUM_MDB_DB_TYPE_VALUES)
    {
        return mdb_unknown_string;
    }
    else
    {
        return mdb_db_types_strings[mdb_db_type];
    }
}

static shr_error_e
mdb_diag_single_table_info_print(
    int unit,
    sh_sand_control_t * sand_control,
    dbal_physical_tables_e dbal_physical_table_id,
    char *table_name,
    dbal_enum_value_field_mdb_db_type_e mdb_db_type,
    uint32 max_capacity,
    mdb_cluster_alloc_info_t clusters[DNX_DATA_MAX_MDB_DH_MAX_NOF_DB_CLUSTERS],
    int nof_valid_clusters)
{
    int ii;

    PRT_INIT_VARS;
    SHR_FUNC_INIT_VARS(unit);

    /*
     * Print table header
     */
    PRT_TITLE_SET("DBAL PHYSICAL TABLE");


    PRT_COLUMN_ADD("Table Name");
    PRT_COLUMN_ADD("Type");
    PRT_COLUMN_ADD("Address capacity");
    PRT_COLUMN_ADD("Address Resolution[bits]");

    PRT_ROW_ADD(PRT_ROW_SEP_NONE);
    PRT_CELL_SET("%s", table_name);
    PRT_CELL_SET("%s", mdb_get_db_type_str(mdb_db_type));
    PRT_CELL_SET("%d", max_capacity);
    if (nof_valid_clusters != 0)
    {
        if (mdb_db_type == DBAL_ENUM_FVAL_MDB_DB_TYPE_DIRECT)
        {
            PRT_CELL_SET("%d", clusters[0].entry_size);
        }
        else if (mdb_db_type == DBAL_ENUM_FVAL_MDB_DB_TYPE_EM)
        {
            /*
             * EM is presented as maximal number of addresses
             * mdb info table=EM_TABLE should be used for specific entry encondings
             */
            PRT_CELL_SET("%d", MDB_DIRECT_BASIC_ENTRY_SIZE);
        }
    }

    PRT_COMMITX;

    /*
     * Print cluster info
     */
    PRT_TITLE_SET("DBAL PHYSICAL TABLE - CLUSTER INFO");


    PRT_COLUMN_ADD("MDB table");
    PRT_COLUMN_ADD("Macro type");
    PRT_COLUMN_ADD("Macro index");
    PRT_COLUMN_ADD("Cluster index");
    PRT_COLUMN_ADD("EEDB type");
    PRT_COLUMN_ADD("Logical start");


    for (ii = 0; ii < nof_valid_clusters; ii++)
    {
        mdb_physical_table_e mdb_physical_table_id;

        /*
         * This cluster is not currently allocated to this table
         */
        if (dbal_physical_table_id != clusters[ii].dbal_physical_table_id)
        {
            continue;
        }

        SHR_IF_ERR_EXIT(mdb_init_get_cluster_assoc
                        (unit, clusters[ii].macro_type, clusters[ii].macro_idx, clusters[ii].cluster_idx,
                         &mdb_physical_table_id));

        PRT_ROW_ADD(PRT_ROW_SEP_NONE);

        /*
         * Printing the MDB table is useful for LPM where a single DBAL table is associated with multiple MDB tables.
         * Note that EEDB tables may have EEDB banks that are not initialized and therefore not associated with an MDB table,
         * but can still be associated with the DBAL table.
         */
        if (mdb_physical_table_id != MDB_NOF_PHYSICAL_TABLES)
        {
            PRT_CELL_SET("%s", dnx_data_mdb.pdbs.mdb_pdb_info_get(unit, mdb_physical_table_id)->name);
        }
        else
        {
            PRT_CELL_SET("Uninitialized");
        }

        PRT_CELL_SET("%s", dnx_data_mdb.dh.macro_type_info_get(unit, clusters[ii].macro_type)->name);
        PRT_CELL_SET("%d", clusters[ii].macro_idx);
        PRT_CELL_SET("%d", clusters[ii].cluster_idx);
        if (mdb_db_type == DBAL_ENUM_FVAL_MDB_DB_TYPE_EEDB)
        {
            PRT_CELL_SET("%s", clusters[ii].mdb_eedb_type == MDB_EEDB_MEM_TYPE_LL ? "LL" : "Data");
        }
        else
        {
            PRT_CELL_SET("");
        }

        if (clusters[ii].logical_start_address == MDB_DIRECT_INVALID_START_ADDRESS)
        {
            PRT_CELL_SET("Uninitialized");
        }
        else
        {
            PRT_CELL_SET("0x%x", clusters[ii].logical_start_address);
        }

    }

    PRT_COMMITX;


exit:
    PRT_FREE;
    SHR_FUNC_EXIT;
}

/**
* \brief
* Print a summary of the different entry sizes and their capacity in each of the MDB physical DBs
 */
static shr_error_e
mdb_diag_capacity_info_print(
    int unit,
    sh_sand_control_t * sand_control)
{
    dbal_physical_tables_e dbal_physical_table_id_iter;
    PRT_INIT_VARS;

    SHR_FUNC_INIT_VARS(unit);

    /*
     * Print table header
     */
    PRT_TITLE_SET("Capacity summary table");

    PRT_INFO_ADD("EM tables capacity is presented as estimates in Expected(Pessimistic) format.");
    PRT_INFO_ADD("LPM(KAPS) tables capacity is presented as estimates in IPv4(32bits) and IPv6(128bits) entry sizes.");
    PRT_INFO_ADD
        ("EEDB entry size is controlled by the outlif_physical_phase_data_granularity_XX soc property, capacities are for the current dynamic allocation.");
    PRT_INFO_ADD("MAP (PMF-Direct) entry size is controlled by the map_payload_size_enum soc property.");

    PRT_COLUMN_ADD("Table Name");
    PRT_COLUMN_ADD("Entry bits 1:Capacity");
    PRT_COLUMN_ADD("Entry bits 2:Capacity");
    PRT_COLUMN_ADD("Entry bits 3:Capacity");

    for (dbal_physical_table_id_iter = 0; dbal_physical_table_id_iter < DBAL_NOF_PHYSICAL_TABLES;
         dbal_physical_table_id_iter++)
    {
        dbal_enum_value_field_mdb_db_type_e db_type;
        int num_valid_entries = 0;
        /*
         * Up to 3 valid entry types are expected
         */
        int entry_sizes_arr[MDB_EM_ENTRY_ENCODING_EIGHTH] = { 0 };
        int entry_capacity_arr[MDB_EM_ENTRY_ENCODING_EIGHTH] = { 0 };
        /*
         * EM requires an additional array for the lower bound on the estimate
         */
        int entry_capacity_em_arr[MDB_EM_ENTRY_ENCODING_EIGHTH] = { 0 };
        int entry_size_iter;

        /*
         * Only print info for MDB tables
         */
        if (!MDB_DB_TYPE_IS_VALID(dnx_data_mdb.pdbs.dbal_pdb_info_get(unit, dbal_physical_table_id_iter)->db_type))
        {
            continue;
        }

        /*
         * Populate the entry size array based on the DB type
         */
        db_type = dnx_data_mdb.pdbs.dbal_pdb_info_get(unit, dbal_physical_table_id_iter)->db_type;
        if ((db_type == DBAL_ENUM_FVAL_MDB_DB_TYPE_DIRECT) || (db_type == DBAL_ENUM_FVAL_MDB_DB_TYPE_EEDB))
        {
            int entry_size;
            int capacity;
            mdb_physical_table_e mdb_physical_table_id =
                dnx_data_mdb.pdbs.dbal_pdb_info_get(unit, dbal_physical_table_id_iter)->logical_to_physical;

            SHR_IF_ERR_EXIT(mdb_direct_table_get_basic_size(unit, mdb_physical_table_id, &entry_size));

            SHR_IF_ERR_EXIT(mdb_db_infos.capacity.get(unit, dbal_physical_table_id_iter, &capacity));

            if (MDB_INIT_IS_FEC_TABLE(mdb_physical_table_id))
            {
                /*
                 * FEC usual format is SUPER-FEC, present both FEC and SUPER-FEC
                 */
                num_valid_entries = 2;
                entry_sizes_arr[0] = entry_size / 2;
                entry_capacity_arr[0] = capacity * 2;
                entry_sizes_arr[1] = entry_size;
                entry_capacity_arr[1] = capacity;
            }
            else if ((dnx_data_mdb.pdbs.dbal_pdb_info_get(unit, dbal_physical_table_id_iter)->db_type ==
                      DBAL_ENUM_FVAL_MDB_DB_TYPE_DIRECT)
                     && (dnx_data_mdb.pdbs.dbal_pdb_info_get(unit, dbal_physical_table_id_iter)->db_subtype ==
                         MDB_DIRECT_TYPE_INLIF))
            {
                /*
                 * INLIF supports 120 and 180 entries, therefore the addressing is in 60bits
                 */
                num_valid_entries = 2;
                entry_sizes_arr[0] = entry_size * 2;
                entry_capacity_arr[0] = capacity / 2;
                entry_sizes_arr[1] = entry_size * 3;
                entry_capacity_arr[1] = capacity / 3;
            }
            else
            {
                /*
                 * Other direct and EEDB tables simply support a single entry size
                 * EEDB entry size is set using a soc property
                 */
                num_valid_entries = 1;
                entry_sizes_arr[0] = entry_size;
                entry_capacity_arr[0] = capacity;
            }
        }
        else if (db_type == DBAL_ENUM_FVAL_MDB_DB_TYPE_EM)
        {
            uint8 vmv_size, vmv_value;
            mdb_em_entry_encoding_e entry_encoding_iter;
            mdb_physical_table_e mdb_physical_table_id =
                dnx_data_mdb.pdbs.dbal_pdb_info_get(unit, dbal_physical_table_id_iter)->logical_to_physical;
            int row_width = dnx_data_mdb.pdbs.mdb_pdb_info_get(unit, mdb_physical_table_id)->row_width;

            num_valid_entries = MDB_EM_ENTRY_ENCODING_EIGHTH;
            /*
             * Iterate on all the entry encoding
             * First print small entry capacity
             */
            for (entry_encoding_iter = MDB_EM_ENTRY_ENCODING_ONE; entry_encoding_iter < MDB_EM_ENTRY_ENCODING_EIGHTH;
                 entry_encoding_iter++)
            {
                int arr_index = MDB_EM_ENTRY_ENCODING_EIGHTH - entry_encoding_iter - 1;
                SHR_IF_ERR_EXIT(mdb_em_get_vmv_size_value
                                (unit, dbal_physical_table_id_iter, entry_encoding_iter, &vmv_size, &vmv_value));

                entry_sizes_arr[arr_index] = row_width / (1 << entry_encoding_iter);
                /*
                 * Only valid encodings have non-zero capacities
                 */
                if (vmv_size != 0)
                {
                    SHR_IF_ERR_EXIT(mdb_db_infos.em_entry_capacity_estimate.get(unit, dbal_physical_table_id_iter,
                                                                                entry_encoding_iter,
                                                                                &entry_capacity_em_arr[arr_index]));
                    SHR_IF_ERR_EXIT(mdb_db_infos.
                                    em_entry_capacity_estimate_average.get(unit, dbal_physical_table_id_iter,
                                                                           entry_encoding_iter,
                                                                           &entry_capacity_arr[arr_index]));

                }
                else
                {
                    entry_capacity_arr[arr_index] = 0;
                    entry_capacity_em_arr[arr_index] = 0;
                }
            }
        }
        else if (db_type == DBAL_ENUM_FVAL_MDB_DB_TYPE_KAPS)
        {
            int ipv4_capacity, ipv6_capacity;

            SHR_IF_ERR_EXIT(mdb_lpm_get_ip_capacity(unit, dbal_physical_table_id_iter, &ipv4_capacity, &ipv6_capacity));

            num_valid_entries = 2;
            entry_sizes_arr[0] = 32;
            entry_capacity_arr[0] = ipv4_capacity;
            entry_sizes_arr[1] = 128;
            entry_capacity_arr[1] = ipv6_capacity;
        }

        /*
         * Print the table and entry size contents
         */
        PRT_ROW_ADD(PRT_ROW_SEP_NONE);
        PRT_CELL_SET("%s", dbal_physical_table_to_string(unit, dbal_physical_table_id_iter));
        for (entry_size_iter = 0; entry_size_iter < num_valid_entries; entry_size_iter++)
        {
            if (db_type == DBAL_ENUM_FVAL_MDB_DB_TYPE_EM)
            {
                PRT_CELL_SET("%d:%d (%d)", entry_sizes_arr[entry_size_iter], entry_capacity_arr[entry_size_iter],
                             entry_capacity_em_arr[entry_size_iter]);
            }
            else
            {
                PRT_CELL_SET("%d:%d", entry_sizes_arr[entry_size_iter], entry_capacity_arr[entry_size_iter]);
            }
        }
    }
    PRT_COMMITX;

exit:
    PRT_FREE;
    SHR_FUNC_EXIT;
}

/**
* \brief
* dump physical table information.
* See dnx_mdb_info_man for the different options.
 */
shr_error_e
cmd_dnx_mdb_info_print(
    int unit,
    args_t * args,
    sh_sand_control_t * sand_control)
{
    dbal_physical_tables_e dbal_physical_table_id;
    char *table_name = NULL;
    int ii;
    int capacity = 0;
    int clusters_print, capacity_print;
    PRT_INIT_VARS;

    SHR_FUNC_INIT_VARS(unit);

    PRT_TITLE_SET("MDB Profile");

    PRT_INFO_ADD("MDB profile: %s", dnx_data_mdb.pdbs.mdb_profile_get(unit)->profile);

    PRT_INFO_ADD("MDB profile KAPS cfg: %d", dnx_data_mdb.pdbs.mdb_profile_kaps_cfg_get(unit)->val);

    PRT_COMMITX;

    SH_SAND_GET_STR("table", table_name);
    SH_SAND_GET_BOOL("clusters", clusters_print);
    SH_SAND_GET_BOOL("capacity", capacity_print);

    if (capacity_print == TRUE)
    {
        SHR_IF_ERR_EXIT(mdb_diag_capacity_info_print(unit, sand_control));
    }
    else if (ISEMPTY(table_name))
    {
        /*
         * Print table names
         */

        PRT_TITLE_SET("DBAL PHYSICAL TABLES");


        PRT_COLUMN_ADD("Table Name");
        PRT_COLUMN_ADD("Type");
        PRT_COLUMN_ADD("Address capacity");
        PRT_COLUMN_ADD("Address Resolution[bits]");

        for (ii = 1; ii < DBAL_NOF_PHYSICAL_TABLES; ii++)
        {
            mdb_cluster_alloc_info_t clusters[DNX_DATA_MAX_MDB_DH_MAX_NOF_DB_CLUSTERS];
            int nof_valid_clusters;

            /*
             * Only print info for MDB tables
             */
            if (!MDB_DB_TYPE_IS_VALID(dnx_data_mdb.pdbs.dbal_pdb_info_get(unit, ii)->db_type))
            {
                continue;
            }

            SHR_IF_ERR_EXIT(mdb_init_get_table_resources(unit, ii, clusters, &nof_valid_clusters));

            SHR_IF_ERR_EXIT(mdb_db_infos.capacity.get(unit, ii, &capacity));
            PRT_ROW_ADD(PRT_ROW_SEP_NONE);
            PRT_CELL_SET("%s", dbal_physical_table_to_string(unit, ii));
            PRT_CELL_SET("%s", mdb_get_db_type_str(dnx_data_mdb.pdbs.dbal_pdb_info_get(unit, ii)->db_type));
            PRT_CELL_SET("%d", capacity);
            if (nof_valid_clusters != 0)
            {
                if (dnx_data_mdb.pdbs.dbal_pdb_info_get(unit, ii)->db_type == DBAL_ENUM_FVAL_MDB_DB_TYPE_DIRECT)
                {
                    PRT_CELL_SET("%d", clusters[0].entry_size);
                }
                else if (dnx_data_mdb.pdbs.dbal_pdb_info_get(unit, ii)->db_type == DBAL_ENUM_FVAL_MDB_DB_TYPE_EM)
                {
                    /*
                     * EM is presented as maximal number of addresses
                     * mdb info table=EM_TABLE should be used for specific entry encondings
                     */
                    PRT_CELL_SET("%d", MDB_DIRECT_BASIC_ENTRY_SIZE);
                }
            }
        }

        PRT_COMMITX;

        if (clusters_print == TRUE)
        {
            mdb_macro_types_e macro_type_iter;
            int cluster_idx;
            /*
             * Print cluster association
             * Iterate on all the macros, clusters, retrieve allocation, convert from MDB table to DBAL table, print DBAL table name
             */

            for (macro_type_iter = 0; macro_type_iter < MDB_EEDB_BANK; macro_type_iter++)
            {
                uint32 nof_macros = dnx_data_mdb.dh.macro_type_info_get(unit, macro_type_iter)->nof_macros;
                uint32 macro_iter;

                if (nof_macros == 0)
                {
                    continue;
                }

                PRT_TITLE_SET("Cluster allocation, macro_type=%s",
                              dnx_data_mdb.dh.macro_type_info_get(unit, macro_type_iter)->name);

                PRT_COLUMN_ADD("Macro ID");
                for (cluster_idx = 0;
                     cluster_idx < dnx_data_mdb.dh.macro_type_info_get(unit, macro_type_iter)->nof_clusters;
                     cluster_idx++)
                {
                    PRT_COLUMN_ADD("Cluster %d", cluster_idx);
                }

                for (macro_iter = 0; macro_iter < nof_macros; macro_iter++)
                {
                    PRT_ROW_ADD(PRT_ROW_SEP_NONE);
                    PRT_CELL_SET("Macro %d", macro_iter);

                    for (cluster_idx = 0;
                         cluster_idx < dnx_data_mdb.dh.macro_type_info_get(unit, macro_type_iter)->nof_clusters;
                         cluster_idx++)
                    {
                        mdb_physical_table_e mdb_physical_table_id;
                        int serial_cluster_index;

                        serial_cluster_index =
                            MDB_DH_SERIAL_CLUSTER_NUMBER_FOR_MACRO_TYPE(unit, macro_type_iter, macro_iter, cluster_idx);

                        SHR_IF_ERR_EXIT(mdb_db_infos.mdb_cluster_infos.macro_cluster_assoc.
                                        pdb.get(unit, macro_type_iter, serial_cluster_index, &mdb_physical_table_id));

                        if (mdb_physical_table_id != MDB_NOF_PHYSICAL_TABLES)
                        {
                            {
                                PRT_CELL_SET("%s",
                                             dnx_data_mdb.pdbs.mdb_pdb_info_get(unit, mdb_physical_table_id)->name);
                            }
                        }
                        else
                        {
                            PRT_CELL_SET("Unallocated");
                        }
                    }
                }

                PRT_COMMITX;
            }
        }
    }
    else
    {
        mdb_cluster_alloc_info_t clusters[DNX_DATA_MAX_MDB_DH_MAX_NOF_DB_CLUSTERS];
        int nof_valid_clusters;
        dbal_enum_value_field_mdb_db_type_e db_type;

        SHR_IF_ERR_EXIT(dbal_physical_table_string_to_id(unit, table_name, &dbal_physical_table_id));

        db_type = dnx_data_mdb.pdbs.dbal_pdb_info_get(unit, dbal_physical_table_id)->db_type;

        if (!MDB_DB_TYPE_IS_VALID(db_type))
        {
            SHR_ERR_EXIT(_SHR_E_PARAM, "Unrecognized table name %s.\n", table_name);
        }

        SHR_IF_ERR_EXIT(mdb_init_get_table_resources(unit, dbal_physical_table_id, clusters, &nof_valid_clusters));

        SHR_IF_ERR_EXIT(mdb_db_infos.capacity.get(unit, dbal_physical_table_id, &capacity));

        SHR_IF_ERR_EXIT(mdb_diag_single_table_info_print
                        (unit, sand_control, dbal_physical_table_id,
                         dbal_physical_table_to_string(unit, dbal_physical_table_id),
                         db_type, capacity, clusters, nof_valid_clusters));

        /*
         * Print information related to APP ID for EM tables
         */
        if (dnx_data_mdb.pdbs.dbal_pdb_info_get(unit, dbal_physical_table_id)->db_type == DBAL_ENUM_FVAL_MDB_DB_TYPE_EM)
        {
            int nof_app_ids = 1 << dnx_data_mdb.em.em_info_get(unit, dbal_physical_table_id)->tid_size;
            int hw_entry_count;
            int app_id;

            PRT_TITLE_SET("Associated APP IDs");

            PRT_INFO_ADD("The key_size is presented as DBAL (MDB)");

            PRT_COLUMN_ADD("Logical table name");
            PRT_COLUMN_ADD("APP ID");
            PRT_COLUMN_ADD("APP ID size");
            PRT_COLUMN_ADD("Key size");
            PRT_COLUMN_ADD("Result type name");
            PRT_COLUMN_ADD("Payload size");
            PRT_COLUMN_ADD("VMV size");

            for (app_id = 0; app_id < nof_app_ids; app_id++)
            {
                soc_reg_above_64_val_t mdb_key_size;

                SHR_IF_ERR_EXIT(mdb_em_get_key_size(unit, dbal_physical_table_id, app_id, mdb_key_size));

                if (mdb_key_size[0] != 0)
                {
                    /** Only print valid APP IDs */
                    dbal_tables_e dbal_logical_table_id;
                    int dbal_key_size = 0, payload_size_int = 0;
                    uint32 app_db_id;
                    int app_db_size;
                    CONST dbal_logical_table_t *dbal_logical_table;
                    int result_type_iter;

                    SHR_IF_ERR_EXIT(dbal_tables_table_by_mdb_phy_get
                                    (unit, dbal_physical_table_id, app_id, &dbal_logical_table_id));
                    SHR_IF_ERR_EXIT(dbal_tables_sizes_get
                                    (unit, dbal_logical_table_id, &dbal_key_size, &payload_size_int));
                    SHR_IF_ERR_EXIT(dbal_tables_app_db_id_get(unit, dbal_logical_table_id, &app_db_id, &app_db_size));

                    SHR_IF_ERR_EXIT(dbal_tables_table_get(unit, dbal_logical_table_id, &dbal_logical_table));

                    for (result_type_iter = 0; result_type_iter < dbal_logical_table->nof_result_types;
                         result_type_iter++)
                    {
                        int payload_size;
                        mdb_em_entry_encoding_e entry_encoding = MDB_EM_ENTRY_ENCODING_ONE;
                        uint8 vmv_size;
                        uint8 vmv_value;

                        payload_size = dbal_logical_table->multi_res_info[result_type_iter].entry_payload_size;

                        /** Remove the SW core_id if necessary */
                        if (SHR_IS_BITSET(dbal_logical_table->indications_bm, DBAL_TABLE_IND_RESULT_TYPE_MAPPED_TO_SW))
                        {
                            payload_size -= dbal_logical_table->multi_res_info[0].results_info[0].field_nof_bits;
                        }

                        SHR_IF_ERR_EXIT(mdb_em_get_entry_encoding
                                        (unit, dbal_physical_table_id, mdb_key_size[0], payload_size,
                                         dbal_logical_table->app_id, dbal_logical_table->app_id_size, &entry_encoding));

                        SHR_IF_ERR_EXIT(mdb_em_get_vmv_size_value
                                        (unit, dbal_physical_table_id, entry_encoding, &vmv_size, &vmv_value));

                        if (dbal_physical_table_id == DBAL_PHYSICAL_TABLE_PPMC)
                        {

                            /*
                             * DBAL_PHYSICAL_TABLE_PPMC does not require vmv stamping
                             * But for the purposes of this diag reduce the vmv size from the payload size
                             */

                            payload_size -= vmv_size;
                        }

                        PRT_ROW_ADD(PRT_ROW_SEP_NONE);
                        PRT_CELL_SET("%s", dbal_logical_table_to_string(unit, dbal_logical_table_id));
                        PRT_CELL_SET("%d", app_id);
                        PRT_CELL_SET("%d", app_db_size);
                        PRT_CELL_SET("%d (%d)", dbal_key_size, mdb_key_size[0]);
                        PRT_CELL_SET("%s", dbal_logical_table->multi_res_info[result_type_iter].result_type_name);
                        PRT_CELL_SET("%d", payload_size);
                        PRT_CELL_SET("%d", vmv_size);
                    }
                }
            }

            PRT_COMMITX;

            PRT_TITLE_SET("Associated VMV");
            PRT_INFO_ADD("VMV value is in the full %d bit vmv representation",
                         dnx_data_mdb.em.max_nof_vmv_size_get(unit));
            PRT_INFO_ADD("VMV size is from the MSbit of the %d bit VMV value",
                         dnx_data_mdb.em.max_nof_vmv_size_get(unit));

            PRT_COLUMN_ADD("Entry encoding (0-full,1-half,2-quarter...)");
            PRT_COLUMN_ADD("VMV size");
            PRT_COLUMN_ADD("VMV value");
            PRT_COLUMN_ADD("Entry count");
            PRT_COLUMN_ADD("Entry capacity estimate");
            if (mdb_em_age_supported(unit, dbal_physical_table_id) == TRUE)
            {
                PRT_COLUMN_ADD("age bits");
            }

            for (ii = 0; ii < MDB_EM_ENTRY_ENCODING_EMPTY; ii++)
            {
                uint8 vmv_size, vmv_value;
                SHR_IF_ERR_EXIT(mdb_em_get_vmv_size_value(unit, dbal_physical_table_id, ii, &vmv_size, &vmv_value));
                /*
                 * Only print valid encodings
                 */
                if (vmv_size != 0)
                {
                    int entry_count, capacity_estimate;

                    SHR_IF_ERR_EXIT(mdb_db_infos.em_entry_count.get(unit, dbal_physical_table_id, ii, &entry_count));
                    SHR_IF_ERR_EXIT(mdb_db_infos.em_entry_capacity_estimate.get(unit, dbal_physical_table_id, ii,
                                                                                &capacity_estimate));

                    PRT_ROW_ADD(PRT_ROW_SEP_NONE);
                    PRT_CELL_SET("%d", ii);
                    PRT_CELL_SET("%d", vmv_size);
                    PRT_CELL_SET("%d", vmv_value);
                    PRT_CELL_SET("%d", entry_count);
                    PRT_CELL_SET("%d", capacity_estimate);
                    if (mdb_em_age_supported(unit, dbal_physical_table_id) == TRUE)
                    {
                        int nof_age_bits;

                        SHR_IF_ERR_EXIT(mdb_init_em_age_nof_bits_get(unit, dbal_physical_table_id, ii, &nof_age_bits));
                        PRT_CELL_SET("%d", nof_age_bits);
                    }
                }
            }

            SHR_IF_ERR_EXIT(mdb_em_hw_entry_count_get(unit, dbal_physical_table_id, &hw_entry_count));

            PRT_INFO_ADD("Total number of HW entries (including learned entries): %d", hw_entry_count);

            PRT_COMMITX;
        }
    }

exit:
    PRT_FREE;
    SHR_FUNC_EXIT;
}

/*
 * Dump all the entries in private or public DB associated with a specific APP ID
 */
static shr_error_e
mdb_diag_lpm_dump(
    int unit,
    dbal_physical_tables_e dbal_physical_table_id,
    uint8 hitbit,
    sh_sand_control_t * sand_control)
{
    dbal_physical_entry_iterator_t physical_entry_iterator;
    dbal_physical_entry_t entry;
    uint8 is_end = FALSE;
    uint32 app_id = MDB_APP_ID_ITER_ALL;
    uint32 iter_init = FALSE;
    PRT_INIT_VARS;

    SHR_FUNC_INIT_VARS(unit);

    /*
     * Print table header
     * kaps.db_info holds the first KAPS DBAL table
     */
    if (dnx_data_mdb.pdbs.dbal_pdb_info_get(unit, dbal_physical_table_id)->db_type == DBAL_ENUM_FVAL_MDB_DB_TYPE_KAPS)
    {
        PRT_TITLE_SET("MDB LPM %s",
                      dnx_data_mdb.kaps.db_info_get(unit,
                                                    (dnx_data_mdb.pdbs.dbal_pdb_info_get(unit,
                                                                                         dbal_physical_table_id)->db_subtype))->
                      name);
    }
    else
    {
        SHR_ERR_EXIT(_SHR_E_PARAM, "LPM dump operation must be done on an LPM table.\n");
    }

    PRT_COLUMN_ADD("Prefix Length");
    PRT_COLUMN_ADD("Prefix");
    PRT_COLUMN_ADD("Payload Size");
    PRT_COLUMN_ADD("Payload");
    if (dnx_data_mdb.kaps.feature_get(unit, dnx_data_mdb_kaps_hitbit_support))
    {
        PRT_COLUMN_ADD("HitBit");
    }

    sal_memset(&physical_entry_iterator, 0, sizeof(physical_entry_iterator));
    SHR_IF_ERR_EXIT(mdb_lpm_iterator_init(unit, dbal_physical_table_id, app_id, &physical_entry_iterator));
    iter_init = TRUE;

    if (dnx_data_mdb.kaps.feature_get(unit, dnx_data_mdb_kaps_hitbit_support))
    {
        physical_entry_iterator.hit_bit_flags = hitbit;
    }

    SHR_IF_ERR_EXIT(mdb_lpm_iterator_get_next
                    (unit, dbal_physical_table_id, app_id, &physical_entry_iterator, &entry, &is_end));
    while (is_end == FALSE)
    {
        PRT_ROW_ADD(PRT_ROW_SEP_NONE);
        PRT_CELL_SET("%d", entry.prefix_length);
        PRT_CELL_SET("0x%08x%08x%08x%08x%08x", entry.key[4], entry.key[3], entry.key[2], entry.key[1], entry.key[0]);
        PRT_CELL_SET("%d", entry.payload_size);
        {
            PRT_CELL_SET("0x%05x", entry.payload[0]);
        }

        if (dnx_data_mdb.kaps.feature_get(unit, dnx_data_mdb_kaps_hitbit_support))
        {
            PRT_CELL_SET("%s", ((entry.hitbit & DBAL_PHYSICAL_KEY_PRIMARY_HITBIT_ACCESSED) != 0) ? "Hit" : "No-Hit");
        }

        SHR_IF_ERR_EXIT(mdb_lpm_iterator_get_next
                        (unit, dbal_physical_table_id, app_id, &physical_entry_iterator, &entry, &is_end));
    }

    PRT_COMMITX;

exit:
    PRT_FREE;
    if (iter_init == TRUE)
    {
        SHR_IF_ERR_CONT(mdb_lpm_iterator_deinit(unit, dbal_physical_table_id, app_id, &physical_entry_iterator));
    }
    SHR_FUNC_EXIT;
}

static sh_sand_man_t dnx_mdb_lpm_man = {
    .brief = "perform different operations on the LPM (KAPS) table.",
    .full = "Directly perform an operation on the LPM private or public tables. "
        "Specify the 160bit key (including 6bit app_id prefix!) using key, the prefix_length and the payload in case of add. "
        "Note: the LPM is aligned to the MSB, for example if the key is only 16bits=0x1234, "
        "then the input is key=0x1234000000000000000000000000000000000000, note the app_id prefix in this case is 0x12 >> 2 = 4.\n"
        "If the LPM hitbit is relevant \"hit\" must be set for add and delete operations as well.",
    .synopsis =
        "add/get/search/delete/dump [table=TABLE_NAME] [key=0xVALUE] [prefix_length=0-160] [payload=0xVALUE] [app_id=VALUE]",
    .examples =
        "add table=KAPS1 key=0x1234567812345678123456781234560000000000 prefix_length=120 payload_size=20 payload=0xabcd app_id=0\n"
        "get table=KAPS1 key=0x1234567812345678123456781234560000000000 prefix_length=120 app_id=0\n"
#ifndef ADAPTER_SERVER_MODE
        "search table=KAPS1 key=0x1234567812345678123456781234560000000000 app_id=0\n"
        "verify table=KAPS1\n"
        "add table=KAPS1 key=0x1534567812345678123456781234560000000000 prefix_length=120 payload_size=20 payload=0xabcd app_id=0 hit\n"
        "get table=KAPS1 key=0x1534567812345678123456781234560000000000 prefix_length=120 app_id=0 hit\n"
        "get table=KAPS1 key=0x1534567812345678123456781234560000000000 prefix_length=120 app_id=0 hit_clr\n"
        "delete table=KAPS1 key=0x1534567812345678123456781234560000000000 prefix_length=120 app_id=0 hit\n"
#endif
        "stats table=KAPS1\n" "dump table=KAPS1\n" "dump hw table=KAPS1\n"
        "delete table=KAPS1 key=0x1234567812345678123456781234560000000000 prefix_length=120 app_id=0\n"
    "clear table=KAPS1 app_id=0xFFFFFFFF"
};

static sh_sand_option_t dnx_mdb_lpm_options[] = {
    {"ADD", SAL_FIELD_TYPE_BOOL, "Perform an add operation", "no"},
    {"app_id", SAL_FIELD_TYPE_UINT32, "The APP ID", "0"},
    {"CLEAR", SAL_FIELD_TYPE_BOOL, "Perform a table clear operation with the specified app_id (0xFFFFFFFF for all)",
     "no"},
    {"DeLeTe", SAL_FIELD_TYPE_BOOL, "Perform a delete operation", "no"},
    {"dump", SAL_FIELD_TYPE_BOOL, "Perform a dump operation", "no"},
    {"hw", SAL_FIELD_TYPE_BOOL, "Force HW interaction, skipping sw cache, relevant only for dump", "no"},
    {"Get", SAL_FIELD_TYPE_BOOL, "Perform a get operation", "no"},
    {"STATS", SAL_FIELD_TYPE_BOOL, "Print LPM statistics", "no"},
    {"payload_size", SAL_FIELD_TYPE_UINT32, "The KAPS payload size", "20"},
    {"payload", SAL_FIELD_TYPE_ARRAY32, "The KAPS payload", "0"},
    {"prefix_length", SAL_FIELD_TYPE_INT32, "The prefix length, 0-160", "0"},
    {"key", SAL_FIELD_TYPE_ARRAY32, "The 160bit key", "0x0"},
#ifndef ADAPTER_SERVER_MODE
    {"search", SAL_FIELD_TYPE_BOOL, "Perform a HW search operation", "no"},
    {"hit", SAL_FIELD_TYPE_BOOL, "Perform a hitbit operation", "no"},
    {"hit_clr", SAL_FIELD_TYPE_BOOL, "Perform a hitbit clear operation", "no"},
    {"VeRiFY", SAL_FIELD_TYPE_BOOL, "Verify the SW shadow contents match the HW search results", "no"},
#endif
    {"Table", SAL_FIELD_TYPE_STR, "DBAL physical table name", ""},
    {NULL}
};


/**
* \brief
* perform different operations on the LPM (KAPS) table
* See dnx_mdb_lpm_man for the different options.
 */
static shr_error_e
cmd_dnx_mdb_lpm(
    int unit,
    args_t * args,
    sh_sand_control_t * sand_control)
{
    dbal_physical_tables_e physical_tables[DBAL_MAX_NUM_OF_PHY_DB_IN_TABLE] = { 0 };
    dbal_physical_tables_e dbal_physical_table_id;
    dbal_physical_entry_t entry;
    int add_cmd, get_cmd, delete_cmd, dump_cmd, stats_cmd, clear_cmd;
    int hw_cmd;
#ifndef ADAPTER_SERVER_MODE
    int search_cmd;
    int hit_cmd;
    int hit_clr_cmd;
    int verify_cmd;
#endif
    char *table_name = NULL;
    int prefix_length;
    uint32 app_id;
    uint32 *array_uint32;
    PRT_INIT_VARS;

    SHR_FUNC_INIT_VARS(unit);

    sal_memset(&entry, 0x0, sizeof(entry));

    SH_SAND_GET_STR("Table", table_name);
    SH_SAND_GET_BOOL("CLEAR", clear_cmd);
    SH_SAND_GET_BOOL("ADD", add_cmd);
    SH_SAND_GET_BOOL("Get", get_cmd);
    SH_SAND_GET_BOOL("DeLeTe", delete_cmd);
    SH_SAND_GET_BOOL("dump", dump_cmd);
#ifndef ADAPTER_SERVER_MODE
    SH_SAND_GET_BOOL("search", search_cmd);
    SH_SAND_GET_BOOL("hit", hit_cmd);
    SH_SAND_GET_BOOL("hit_clr", hit_clr_cmd);
    SH_SAND_GET_BOOL("VeRiFY", verify_cmd);
#endif
    SH_SAND_GET_BOOL("STATS", stats_cmd);
    SH_SAND_GET_BOOL("hw", hw_cmd);

    SH_SAND_GET_ARRAY32("key", array_uint32);
    sal_memcpy(entry.key, array_uint32, MDB_KAPS_KEY_WIDTH_IN_UINT32(unit) * sizeof(entry.key[0]));
    SH_SAND_GET_INT32("prefix_length", prefix_length);
    SH_SAND_GET_ARRAY32("payload", array_uint32);
    sal_memcpy(entry.payload, array_uint32,
               BITS2WORDS(DNX_DATA_MAX_MDB_PDBS_MAX_PAYLOAD_SIZE) * sizeof(entry.payload[0]));
    SH_SAND_GET_UINT32("app_id", app_id);
    SH_SAND_GET_UINT32("payload_size", entry.payload_size);

    if (prefix_length > dnx_data_mdb.kaps.key_width_in_bits_get(unit))
    {
        SHR_ERR_EXIT(_SHR_E_PARAM, "The prefix_length must be smaller than %d bits.\n",
                     dnx_data_mdb.kaps.key_width_in_bits_get(unit));
    }
    else
    {
        entry.prefix_length = prefix_length;
    }

    entry.key_size = dnx_data_mdb.kaps.key_width_in_bits_get(unit);

#ifndef ADAPTER_SERVER_MODE
    if (hit_cmd == TRUE)
    {
        entry.hitbit |= DBAL_PHYSICAL_KEY_HITBIT_GET | DBAL_PHYSICAL_KEY_HITBIT_PRIMARY;
    }
    if (hit_clr_cmd == TRUE)
    {
        entry.hitbit |= DBAL_PHYSICAL_KEY_HITBIT_CLEAR | DBAL_PHYSICAL_KEY_HITBIT_PRIMARY;
    }
#endif

    {
        if (ISEMPTY(table_name))
        {
            SHR_ERR_EXIT(_SHR_E_PARAM, "The Table parameter must be specified.\n");
        }
        else
        {
            shr_error_e rv;

            rv = dbal_physical_table_string_to_id(unit, table_name, &dbal_physical_table_id);
            if (rv == _SHR_E_NOT_FOUND)
            {
                /*
                 * Do not return an error
                 * Different devices may use different table names
                 * Diag examples are not device specific
                 */
                LOG_INFO(BSL_LOG_MODULE, (BSL_META_U(unit, "Table not found.\n")));
                SHR_EXIT();
            }
            else
            {
                if (rv != _SHR_E_NONE)
                {
                    SHR_ERR_EXIT(rv, "dbal_physical_table_string_to_id returned an error.\n");
                }
            }

            if (dnx_data_mdb.pdbs.dbal_pdb_info_get(unit, dbal_physical_table_id)->db_type !=
                DBAL_ENUM_FVAL_MDB_DB_TYPE_KAPS)
            {
                SHR_ERR_EXIT(_SHR_E_BADID, "The provided table (%s) is not an MDB LPM table.\n",
                             dbal_physical_table_to_string(unit, dbal_physical_table_id));
            }

            physical_tables[0] = dbal_physical_table_id;

            if (add_cmd != 0)
            {
                SHR_IF_ERR_EXIT(mdb_lpm_entry_add(unit, physical_tables, app_id, &entry, 0));
                LOG_INFO(BSL_LOG_MODULE, (BSL_META_U(unit, "Add operation completed successfully on LPM table.\n")));
            }
            else if (get_cmd != 0)
            {
                shr_error_e return_error;
                return_error = mdb_lpm_entry_get(unit, dbal_physical_table_id, app_id, &entry, 0);
                if (return_error == _SHR_E_NOT_FOUND)
                {
                    LOG_INFO(BSL_LOG_MODULE,
                             (BSL_META_U(unit, "Get operation failed, entry not found in LPM table.\n")));
                }
                else if (return_error != _SHR_E_NONE)
                {
                    SHR_ERR_EXIT(return_error, "Get operation failed with unexpected error in LPM table.\n");
                }
                else
                {
                    LOG_INFO(BSL_LOG_MODULE,
                             (BSL_META_U(unit, "Get operation completed successfully on LPM table.\n")));
                    {
                        LOG_INFO(BSL_LOG_MODULE,
                                 (BSL_META_U(unit, "The associated payload is: 0x%x\n"), entry.payload[0]));
                    }
#ifndef ADAPTER_SERVER_MODE
                    if ((hit_cmd != FALSE) || (hit_clr_cmd != FALSE))
                    {
                        LOG_INFO(BSL_LOG_MODULE, (BSL_META_U(unit, "Hitbit: %s\n"),
                                                  ((entry.hitbit & DBAL_PHYSICAL_KEY_PRIMARY_HITBIT_ACCESSED) !=
                                                   0) ? "Hit" : "No-Hit"));
                    }
#endif
                }
            }
            else if (delete_cmd != 0)
            {
                SHR_IF_ERR_EXIT(mdb_lpm_entry_delete(unit, physical_tables, app_id, &entry, 0));
                LOG_INFO(BSL_LOG_MODULE, (BSL_META_U(unit, "Delete operation completed successfully on LPM table.\n")));
            }
            else if (dump_cmd != 0)
            {
                if (hw_cmd == FALSE)
                {
                    SHR_IF_ERR_EXIT(mdb_diag_lpm_dump(unit, dbal_physical_table_id, entry.hitbit, sand_control));
                }
                else
                {
                    SHR_IF_ERR_EXIT(mdb_lpm_dump_hw(unit, dbal_physical_table_id));
                }
            }
            else if (clear_cmd != 0)
            {
                SHR_IF_ERR_EXIT(mdb_lpm_table_clear(unit, physical_tables, app_id));
            }
#ifndef ADAPTER_SERVER_MODE
            else if (search_cmd != 0)
            {
                int core;

                /*
                 * prevent desc dma from accessing the block simultaneously 
                 */
                SHR_IF_ERR_EXIT(dnx_sbusdma_desc_wait_done(unit));

                PRT_TITLE_SET("MDB LPM %s",
                              dnx_data_mdb.kaps.db_info_get(unit,
                                                            dnx_data_mdb.pdbs.dbal_pdb_info_get(unit,
                                                                                                dbal_physical_table_id)->db_subtype)->
                              name);

                PRT_COLUMN_ADD("Core");
                PRT_COLUMN_ADD("Prefix Length");
                PRT_COLUMN_ADD("Prefix");
                PRT_COLUMN_ADD("Payload");

                DNXCMN_CORES_ITER(unit, BCM_CORE_ALL, core)
                {
                    SHR_IF_ERR_EXIT(mdb_lpm_xpt_entry_search(unit, core, dbal_physical_table_id, app_id, &entry));

                    PRT_ROW_ADD(PRT_ROW_SEP_NONE);
                    PRT_CELL_SET("%d", core);
                    PRT_CELL_SET("%d", entry.prefix_length);
                    PRT_CELL_SET("0x%08x%08x%08x%08x%08x", entry.key[4], entry.key[3], entry.key[2], entry.key[1],
                                 entry.key[0]);
                    PRT_CELL_SET("0x%05x", entry.payload[0]);
                }
                PRT_COMMITX;
            }
            else if (verify_cmd != 0)
            {
                SHR_IF_ERR_EXIT(mdb_lpm_table_verify(unit, dbal_physical_table_id));
                LOG_INFO(BSL_LOG_MODULE, (BSL_META_U(unit, "Verify completed successfully.\n")));
            }
#endif
            else if (stats_cmd != 0)
            {
                int num_entries, num_entries_iter, capacity_estimate, ipv4_capacity, ipv6_capacity;

                PRT_TITLE_SET("MDB LPM Stats");
                PRT_COLUMN_ADD("Database");
                PRT_COLUMN_ADD("Num Entries");
                PRT_COLUMN_ADD("Num Entries Iterator");
                PRT_COLUMN_ADD("IPv4 Capacity estimate");
                PRT_COLUMN_ADD("IPv6 Capacity estimate");

                SHR_IF_ERR_EXIT(mdb_lpm_get_current_capacity
                                (unit, dbal_physical_table_id, &num_entries, &num_entries_iter, &capacity_estimate));
                SHR_IF_ERR_EXIT(mdb_lpm_get_ip_capacity(unit, dbal_physical_table_id, &ipv4_capacity, &ipv6_capacity));

                PRT_ROW_ADD(PRT_ROW_SEP_NONE);
                PRT_CELL_SET("%s", dbal_physical_table_to_string(unit, dbal_physical_table_id));

                PRT_CELL_SET("%d", num_entries);
                PRT_CELL_SET("%d", num_entries_iter);
                PRT_CELL_SET("%d", ipv4_capacity);
                PRT_CELL_SET("%d", ipv6_capacity);

                PRT_COMMITX;
            }
            else
            {
                LOG_INFO(BSL_LOG_MODULE, (BSL_META_U(unit, "An operation must be specified.\n")));
            }
        }
    }

exit:
    PRT_FREE;
    SHR_FUNC_EXIT;
}

static sh_sand_man_t dnx_mdb_direct_man = {
    .brief = "perform different operations on the direct tables.",
    .full = "Directly perform an operation on the direct tables. "
        "Specify the index (key) using key, the payload_size and the payload in case of add. "
        "If payload_size is not specified(=0) then it is assumed to be the table default. "
        "Payload mask is assumed to be full.",
    .synopsis =
        "add/get/delete/dump [table=DBAL_PHYSICAL_TABLE_NAME] [key=0xVALUE] [payload_size=0-180] [payload=0xVALUE]",
    .examples =
        "add table=IVSI key=100 payload_size=90 payload=0x12345678abcdef01030507\n"
        "get table=IVSI key=100 payload_size=90\n"
#ifndef ADAPTER_SERVER_MODE
        "get table=IVSI key=100 payload_size=90 hit\n" "get table=IVSI key=100 payload_size=90 hit_clr\n"
#endif
    "get hw table=IVSI key=100 payload_size=90\n"
        "dump table=IVSI\n" "dump hw table=IVSI\n"
        "delete table=IVSI key=100 payload_size=90 payload=0x12345678abcdef01030507\n" "clear table=IVSI"
};

static sh_sand_option_t dnx_mdb_direct_options[] = {
    {"Table", SAL_FIELD_TYPE_STR, "DBAL physical table name", NULL},
    {"ADD", SAL_FIELD_TYPE_BOOL, "Perform an add operation", "no"},
    {"CLEAR", SAL_FIELD_TYPE_BOOL, "Perform a table clear operation", "no"},
    {"Get", SAL_FIELD_TYPE_BOOL, "Perform a get operation", "no"},
    {"hw", SAL_FIELD_TYPE_BOOL, "Force HW interaction, skipping sw cache, relevant only for dump and get", "no"},
    {"DeLeTe", SAL_FIELD_TYPE_BOOL, "Perform a delete operation", "no"},
    {"Dump", SAL_FIELD_TYPE_BOOL, "Perform a dump operation", "no"},
    {"key", SAL_FIELD_TYPE_UINT32, "The direct entry index", "0"},
    {"payload_size", SAL_FIELD_TYPE_UINT32, "The payload size", "0"},
    {"payload", SAL_FIELD_TYPE_ARRAY32, "Up to 180bits of the payload", "0x0"},
#ifndef ADAPTER_SERVER_MODE
    {"hit", SAL_FIELD_TYPE_BOOL, "Perform a hitbit operation", "no"},
    {"hit_clr", SAL_FIELD_TYPE_BOOL, "Perform a hitbit clear operation", "no"},
#endif
    {NULL}
};

/*
 * Dump all the entries in a specific direct table
 */
static shr_error_e
mdb_diag_direct_dump(
    int unit,
    dbal_physical_tables_e dbal_physical_table_id,
    uint32 app_id,
    int hw_cmd,
    sh_sand_control_t * sand_control)
{
    dbal_physical_entry_iterator_t physical_entry_iterator;
    dbal_physical_entry_t entry;
    uint8 is_end = FALSE;

    uint8 has_two_hit_bits =
        MDB_INIT_IS_FEC_TABLE(dnx_data_mdb.pdbs.dbal_pdb_info_get(unit, dbal_physical_table_id)->logical_to_physical);

    uint8 hit_bit_enabled =
        (MDB_INIT_IS_FEC_TABLE(dnx_data_mdb.pdbs.dbal_pdb_info_get(unit, dbal_physical_table_id)->logical_to_physical))
        && (dnx_data_l3.feature.feature_get(unit, dnx_data_l3_feature_fec_hit_bit)) ? FALSE : TRUE;
    PRT_INIT_VARS;

    SHR_FUNC_INIT_VARS(unit);

    sal_memset(&physical_entry_iterator, 0, sizeof(physical_entry_iterator));
    SHR_IF_ERR_EXIT(mdb_direct_table_iterator_init(unit, dbal_physical_table_id, app_id, &physical_entry_iterator));

    /*
     * Print table header
     */
    PRT_TITLE_SET("Entries in: %s", dbal_physical_table_to_string(unit, dbal_physical_table_id));

    PRT_COLUMN_ADD("Key");
    PRT_COLUMN_ADD("Payload Size");
    PRT_COLUMN_ADD("Payload");
    if (has_two_hit_bits)
    {
        PRT_COLUMN_ADD("HitBit A (primary)");
        PRT_COLUMN_ADD("HitBit B (primary)");
        PRT_COLUMN_ADD("HitBit A (secondary)");
        PRT_COLUMN_ADD("HitBit B (secondary)");
        physical_entry_iterator.hit_bit_flags |= DBAL_PHYSICAL_KEY_HITBIT_SECONDARY;
    }
    else
    {
        PRT_COLUMN_ADD("HitBit A");
        PRT_COLUMN_ADD("HitBit B");
    }

    physical_entry_iterator.hit_bit_flags |= (DBAL_PHYSICAL_KEY_HITBIT_GET | DBAL_PHYSICAL_KEY_HITBIT_PRIMARY);
    if (hw_cmd == TRUE)
    {
        physical_entry_iterator.mdb_action_apply = DBAL_MDB_ACTION_APPLY_HW_ONLY;
    }

    entry.payload_size = 0;
    SHR_IF_ERR_EXIT(mdb_direct_table_iterator_get_next
                    (unit, dbal_physical_table_id, app_id, &physical_entry_iterator, &entry, &is_end));
    while (is_end == FALSE)
    {
        PRT_ROW_ADD(PRT_ROW_SEP_NONE);
        PRT_CELL_SET("%d", entry.key[0]);
        PRT_CELL_SET("%d", entry.payload_size);

        if (entry.payload_size <= SAL_UINT32_NOF_BITS)
        {
            PRT_CELL_SET("0x%08x", entry.payload[0]);
        }
        else if (entry.payload_size <= 2 * SAL_UINT32_NOF_BITS)
        {
            PRT_CELL_SET("0x%08x%08x", entry.payload[1], entry.payload[0]);
        }
        else if (entry.payload_size <= 3 * SAL_UINT32_NOF_BITS)
        {
            PRT_CELL_SET("0x%08x%08x%08x", entry.payload[2], entry.payload[1], entry.payload[0]);
        }
        else if (entry.payload_size <= 4 * SAL_UINT32_NOF_BITS)
        {
            PRT_CELL_SET("0x%08x%08x%08x%08x", entry.payload[3], entry.payload[2], entry.payload[1], entry.payload[0]);
        }
        else if (entry.payload_size <= 5 * SAL_UINT32_NOF_BITS)
        {
            PRT_CELL_SET("0x%08x%08x%08x%08x%08x", entry.payload[4], entry.payload[3], entry.payload[2],
                         entry.payload[1], entry.payload[0]);
        }
        else if (entry.payload_size <= 6 * SAL_UINT32_NOF_BITS)
        {
            PRT_CELL_SET("0x%08x%08x%08x%08x%08x%08x", entry.payload[5], entry.payload[4], entry.payload[3],
                         entry.payload[2], entry.payload[1], entry.payload[0]);
        }
        else
        {
            SHR_ERR_EXIT(_SHR_E_INTERNAL, "Unexpected payload size, larger than 192 bits.\n");
        }

        if (hit_bit_enabled)
        {
            PRT_CELL_SET("%s", ((entry.hitbit & DBAL_PHYSICAL_KEY_PRIMARY_HITBIT_ACCESSED_A) != 0) ? "Hit" : "No-Hit");
            PRT_CELL_SET("%s", ((entry.hitbit & DBAL_PHYSICAL_KEY_PRIMARY_HITBIT_ACCESSED_B) != 0) ? "Hit" : "No-Hit");
        }
        else
        {
            PRT_CELL_SET("%s", "N/A");
            PRT_CELL_SET("%s", "N/A");
        }

        if (has_two_hit_bits)
        {
            if (hit_bit_enabled)
            {
                PRT_CELL_SET("%s",
                             ((entry.hitbit & DBAL_PHYSICAL_KEY_SECONDARY_HITBIT_ACCESSED_A) != 0) ? "Hit" : "No-Hit");
                PRT_CELL_SET("%s",
                             ((entry.hitbit & DBAL_PHYSICAL_KEY_SECONDARY_HITBIT_ACCESSED_B) != 0) ? "Hit" : "No-Hit");
            }
            else
            {
                PRT_CELL_SET("%s", "N/A");
                PRT_CELL_SET("%s", "N/A");
            }
        }

        entry.payload_size = 0;
        SHR_IF_ERR_EXIT(mdb_direct_table_iterator_get_next
                        (unit, dbal_physical_table_id, app_id, &physical_entry_iterator, &entry, &is_end));
    }

    PRT_COMMITX;

exit:
    PRT_FREE;
    SHR_IF_ERR_CONT(mdb_direct_table_iterator_deinit(unit, dbal_physical_table_id, app_id, &physical_entry_iterator));
    SHR_FUNC_EXIT;
}

/**
* \brief
* perform different operations on the direct tables
* See dnx_mdb_direct_man for the different options.
 */
shr_error_e
cmd_dnx_mdb_direct(
    int unit,
    args_t * args,
    sh_sand_control_t * sand_control)
{
    dbal_physical_tables_e physical_tables[DBAL_MAX_NUM_OF_PHY_DB_IN_TABLE] = { 0 };
    dbal_physical_tables_e dbal_physical_table_id;
    dbal_physical_entry_t entry;
    int add_cmd, get_cmd, delete_cmd, dump_cmd, clear_cmd, hw_cmd;
    char *table_name = NULL;
    /*
     * app_id is unused in direct tables
     */
    uint32 app_id = 0;
    uint32 *array_uint32;
#ifndef ADAPTER_SERVER_MODE
    int hit_cmd;
    int hit_clr_cmd;
    uint8 has_two_hit_bits = 0;
#endif

    SHR_FUNC_INIT_VARS(unit);

    sal_memset(&entry, 0x0, sizeof(entry));

    SH_SAND_GET_STR("Table", table_name);
    SH_SAND_GET_BOOL("ADD", add_cmd);
    SH_SAND_GET_BOOL("CLEAR", clear_cmd);
    SH_SAND_GET_BOOL("Get", get_cmd);
    SH_SAND_GET_BOOL("Delete", delete_cmd);
    SH_SAND_GET_BOOL("dump", dump_cmd);
    SH_SAND_GET_BOOL("hw", hw_cmd);

    SH_SAND_GET_UINT32("key", entry.key[0]);

    SH_SAND_GET_UINT32("payload_size", entry.payload_size);
    SH_SAND_GET_ARRAY32("payload", array_uint32);
    sal_memcpy(entry.payload, array_uint32,
               BITS2WORDS(dnx_data_mdb.direct.max_payload_size_get(unit)) * sizeof(entry.payload[0]));

#ifndef ADAPTER_SERVER_MODE
    SH_SAND_GET_BOOL("hit", hit_cmd);
    SH_SAND_GET_BOOL("hit_clr", hit_clr_cmd);
#endif

    if (ISEMPTY(table_name))
    {
        SHR_ERR_EXIT(_SHR_E_PARAM, "The Table parameter must be specified.\n");
    }
    else
    {
        SHR_IF_ERR_EXIT(dbal_physical_table_string_to_id(unit, table_name, &dbal_physical_table_id));

        if (dnx_data_mdb.pdbs.dbal_pdb_info_get(unit, dbal_physical_table_id)->db_type !=
            DBAL_ENUM_FVAL_MDB_DB_TYPE_DIRECT)
        {
            SHR_ERR_EXIT(_SHR_E_BADID, "The provided table (%s) is not an MDB direct table.\n",
                         dbal_physical_table_to_string(unit, dbal_physical_table_id));
        }

        physical_tables[0] = dbal_physical_table_id;

        /*
         * Use the table default payload size if not specified
         */
        if (entry.payload_size == 0)
        {
            int basic_size;
            SHR_IF_ERR_EXIT(mdb_direct_table_get_basic_size_using_logical_mdb_table
                            (unit, dbal_physical_table_id, &basic_size));
            entry.payload_size = (uint8) basic_size;
        }

        SHR_IF_ERR_EXIT(utilex_bitstream_set_bit_range(entry.p_mask, 0, entry.payload_size));

        if (add_cmd != 0)
        {
            SHR_IF_ERR_EXIT(mdb_direct_table_entry_add(unit, physical_tables, app_id, &entry, 0));
            LOG_INFO(BSL_LOG_MODULE, (BSL_META_U(unit, "Add operation completed successfully on direct table.\n")));
        }
        else if (get_cmd != 0)
        {
            int ii;
            if (hw_cmd == TRUE)
            {
                entry.mdb_action_apply = DBAL_MDB_ACTION_APPLY_HW_ONLY;
            }
#ifndef ADAPTER_SERVER_MODE
            has_two_hit_bits =
                ((MDB_INIT_IS_FEC_TABLE
                  (dnx_data_mdb.pdbs.dbal_pdb_info_get(unit, dbal_physical_table_id)->logical_to_physical))
                 && (!dnx_data_l3.feature.feature_get(unit, dnx_data_l3_feature_fec_hit_bit)));
            if (hit_cmd == TRUE)
            {
                entry.hitbit |= (DBAL_PHYSICAL_KEY_HITBIT_GET | DBAL_PHYSICAL_KEY_HITBIT_PRIMARY);
                if (has_two_hit_bits)
                {
                    entry.hitbit |= DBAL_PHYSICAL_KEY_HITBIT_SECONDARY;
                }
            }
            if (hit_clr_cmd == TRUE)
            {
                entry.hitbit |= DBAL_PHYSICAL_KEY_HITBIT_CLEAR | DBAL_PHYSICAL_KEY_HITBIT_PRIMARY;
                if (has_two_hit_bits)
                {
                    entry.hitbit |= DBAL_PHYSICAL_KEY_HITBIT_SECONDARY;
                }
            }
#endif
            SHR_IF_ERR_EXIT(mdb_direct_table_entry_get(unit, dbal_physical_table_id, app_id, &entry, 0));
            LOG_INFO(BSL_LOG_MODULE, (BSL_META_U(unit, "Get operation completed successfully on direct table.\n")));
            for (ii = 0; ii < BITS2WORDS(entry.payload_size); ii++)
            {
                LOG_INFO(BSL_LOG_MODULE,
                         (BSL_META_U(unit, "entry.payload[%d]: 0x%08x.\n"), BITS2WORDS(entry.payload_size) - 1 - ii,
                          entry.payload[BITS2WORDS(entry.payload_size) - 1 - ii]));
            }
#ifndef ADAPTER_SERVER_MODE
            if ((hit_cmd != FALSE) || (hit_clr_cmd != FALSE))
            {
                LOG_INFO(BSL_LOG_MODULE, (BSL_META_U(unit, "Hitbit: %s\n"),
                                          ((entry.hitbit & DBAL_PHYSICAL_KEY_PRIMARY_HITBIT_ACCESSED) !=
                                           0) ? "Primary Hit" : "No Primary Hit"));
                if (has_two_hit_bits)
                {
                    LOG_INFO(BSL_LOG_MODULE, (BSL_META_U(unit, "Hitbit: %s\n"),
                                              ((entry.hitbit & DBAL_PHYSICAL_KEY_SECONDARY_HITBIT_ACCESSED) !=
                                               0) ? "Secondary Hit" : "No Secondary Hit"));
                }
            }
#endif

        }
        else if (delete_cmd != 0)
        {
            SHR_IF_ERR_EXIT(mdb_direct_table_entry_delete(unit, physical_tables, app_id, &entry, 0));
            LOG_INFO(BSL_LOG_MODULE, (BSL_META_U(unit, "Delete operation completed successfully on direct table.\n")));
        }
        else if (dump_cmd != 0)
        {
            SHR_IF_ERR_EXIT(mdb_diag_direct_dump(unit, dbal_physical_table_id, app_id, hw_cmd, sand_control));
        }
        else if (clear_cmd != 0)
        {
            SHR_IF_ERR_EXIT(mdb_direct_table_clear(unit, physical_tables, app_id));
        }
        else
        {
            LOG_INFO(BSL_LOG_MODULE, (BSL_META_U(unit, "An operation must be specified.\n")));
        }
    }

exit:
    SHR_FUNC_EXIT;
}

static sh_sand_man_t dnx_mdb_eedb_man = {
    .brief = "perform different operations on the eedb tables.",
    .full = "Directly perform an operation on the eedb tables. "
        "Specify the index (key) using key, the payload_size and the payload in case of add. "
        "If payload_size is not specified(=0) then it is assumed to be the table default. "
        "Payload mask is assumed to be full.",
    .synopsis = "add/get/delete/dump [table=EEDBX] [key=0xVALUE] [payload_size=0-150] [payload=0xVALUE]",
    .examples = "add table=EEDB1 key=100 payload_size=120 payload=0x12345678abcdef01030507 eedb_ll=0\n"
        "get table=EEDB1 key=100 payload_size=120 eedb_ll=0\n"
#ifndef ADAPTER_SERVER_MODE
        "get table=EEDB1 key=100 payload_size=120 eedb_ll=0 hit\n"
        "get table=EEDB1 key=100 payload_size=120 eedb_ll=0 hit_clr\n"
#endif
    "get hw table=EEDB1 key=100 payload_size=120 eedb_ll=0\n" "dump table=EEDB1\n"
        "dump hw table=EEDB1\n"
        "delete table=EEDB1 key=100 payload_size=120 payload=0x12345678abcdef01030507 eedb_ll=0\n" "clear table=EEDB1"
};

static sh_sand_option_t dnx_mdb_eedb_options[] = {
    {"Table", SAL_FIELD_TYPE_STR, "DBAL physical table name", NULL},
    {"ADD", SAL_FIELD_TYPE_BOOL, "Perform an add operation", "no"},
    {"CLEAR", SAL_FIELD_TYPE_BOOL, "Perform a table clear operation", "no"},
    {"Get", SAL_FIELD_TYPE_BOOL, "Perform a get operation", "no"},
    {"hw", SAL_FIELD_TYPE_BOOL, "Force HW interaction, skipping sw cache, relevant only for dump and get", "no"},
    {"DeLeTe", SAL_FIELD_TYPE_BOOL, "Perform a delete operation", "no"},
    {"Dump", SAL_FIELD_TYPE_BOOL, "Perform a delete operation", "no"},
    {"eedb_ll", SAL_FIELD_TYPE_BOOL, "Does the entry include LL entry", "yes"},
    {"key", SAL_FIELD_TYPE_UINT32, "The eedb entry index", "0"},
    {"payload_size", SAL_FIELD_TYPE_UINT32, "The payload size", "0"},
    {"payload", SAL_FIELD_TYPE_ARRAY32, "Up to 150bits of the payload", "0x0"},
#ifndef ADAPTER_SERVER_MODE
    {"hit", SAL_FIELD_TYPE_BOOL, "Perform a hitbit operation", "no"},
    {"hit_clr", SAL_FIELD_TYPE_BOOL, "Perform a hitbit clear operation", "no"},
#endif
    {NULL}
};

/*
 * Dump all the entries in a specific direct table
 */
static shr_error_e
mdb_diag_eedb_dump(
    int unit,
    dbal_physical_tables_e dbal_physical_table_id,
    uint32 app_id,
    int hw_cmd,
    sh_sand_control_t * sand_control)
{
    dbal_physical_entry_iterator_t physical_entry_iterator;
    dbal_physical_entry_t entry;
    uint8 is_end = FALSE;
    PRT_INIT_VARS;

    SHR_FUNC_INIT_VARS(unit);

    /*
     * Print table header
     */
    PRT_TITLE_SET("Entries in: %s", dbal_physical_table_to_string(unit, dbal_physical_table_id));
    PRT_INFO_ADD("Shared entries are printed for both phases in the phase-pair");

    PRT_COLUMN_ADD("Key");
    PRT_COLUMN_ADD("Payload Size");
    PRT_COLUMN_ADD("Payload");
    PRT_COLUMN_ADD("HitBit");

    sal_memset(&physical_entry_iterator, 0, sizeof(physical_entry_iterator));
    SHR_IF_ERR_EXIT(mdb_eedb_table_iterator_init(unit, dbal_physical_table_id, app_id, &physical_entry_iterator));

    physical_entry_iterator.hit_bit_flags |= (DBAL_PHYSICAL_KEY_HITBIT_GET | DBAL_PHYSICAL_KEY_HITBIT_PRIMARY);
    if (hw_cmd == TRUE)
    {
        physical_entry_iterator.mdb_action_apply = DBAL_MDB_ACTION_APPLY_HW_ONLY;
    }

    SHR_IF_ERR_EXIT(mdb_eedb_table_iterator_get_next
                    (unit, dbal_physical_table_id, app_id, &physical_entry_iterator, &entry, &is_end));
    while (is_end == FALSE)
    {
        PRT_ROW_ADD(PRT_ROW_SEP_NONE);
        PRT_CELL_SET("%d", entry.key[0]);
        PRT_CELL_SET("%d", entry.payload_size);

        if (entry.payload_size <= SAL_UINT32_NOF_BITS)
        {
            PRT_CELL_SET("0x%08x", entry.payload[0]);
        }
        else if (entry.payload_size <= 2 * SAL_UINT32_NOF_BITS)
        {
            PRT_CELL_SET("0x%08x%08x", entry.payload[1], entry.payload[0]);
        }
        else if (entry.payload_size <= 3 * SAL_UINT32_NOF_BITS)
        {
            PRT_CELL_SET("0x%08x%08x%08x", entry.payload[2], entry.payload[1], entry.payload[0]);
        }
        else if (entry.payload_size <= 4 * SAL_UINT32_NOF_BITS)
        {
            PRT_CELL_SET("0x%08x%08x%08x%08x", entry.payload[3], entry.payload[2], entry.payload[1], entry.payload[0]);
        }
        else if (entry.payload_size <= 5 * SAL_UINT32_NOF_BITS)
        {
            PRT_CELL_SET("0x%08x%08x%08x%08x%08x", entry.payload[4], entry.payload[3], entry.payload[2],
                         entry.payload[1], entry.payload[0]);
        }
        else if (entry.payload_size <= 6 * SAL_UINT32_NOF_BITS)
        {
            PRT_CELL_SET("0x%08x%08x%08x%08x%08x%08x", entry.payload[5], entry.payload[4], entry.payload[3],
                         entry.payload[2], entry.payload[1], entry.payload[0]);
        }
        else
        {
            SHR_ERR_EXIT(_SHR_E_INTERNAL, "Unexpected payload size, larger than 192 bits.\n");
        }

        PRT_CELL_SET("%s", ((entry.hitbit & DBAL_PHYSICAL_KEY_PRIMARY_HITBIT_ACCESSED) != 0) ? "Hit" : "No-Hit");

        SHR_IF_ERR_EXIT(mdb_eedb_table_iterator_get_next
                        (unit, dbal_physical_table_id, app_id, &physical_entry_iterator, &entry, &is_end));
    }

    PRT_COMMITX;

exit:
    PRT_FREE;
    SHR_IF_ERR_CONT(mdb_eedb_table_iterator_deinit(unit, dbal_physical_table_id, app_id, &physical_entry_iterator));
    SHR_FUNC_EXIT;
}

/**
* \brief
* perform different operations on the eedb tables
* See dnx_mdb_direct_man for the different options.
 */
shr_error_e
cmd_dnx_mdb_eedb(
    int unit,
    args_t * args,
    sh_sand_control_t * sand_control)
{
    dbal_physical_tables_e physical_tables[DBAL_MAX_NUM_OF_PHY_DB_IN_TABLE] = { 0 };
    dbal_physical_tables_e dbal_physical_table_id;
    dbal_physical_entry_t entry;
    int add_cmd, get_cmd, hw_cmd, delete_cmd, dump_cmd, clear_cmd;
    char *table_name = NULL;
    /*
     * app_id is unused in direct tables
     */
    uint32 app_id = 0;
    uint32 *array_uint32;
    uint8 eedb_ll;
#ifndef ADAPTER_SERVER_MODE
    int hit_cmd;
    int hit_clr_cmd;
#endif
    SHR_FUNC_INIT_VARS(unit);

    sal_memset(&entry, 0x0, sizeof(entry));

    SH_SAND_GET_STR("Table", table_name);
    SH_SAND_GET_BOOL("ADD", add_cmd);
    SH_SAND_GET_BOOL("CLEAR", clear_cmd);
    SH_SAND_GET_BOOL("Get", get_cmd);
    SH_SAND_GET_BOOL("hw", hw_cmd);
    SH_SAND_GET_BOOL("Delete", delete_cmd);
    SH_SAND_GET_BOOL("dump", dump_cmd);

    SH_SAND_GET_UINT32("key", entry.key[0]);

    SH_SAND_GET_UINT32("payload_size", entry.payload_size);
    SH_SAND_GET_ARRAY32("payload", array_uint32);
    sal_memcpy(entry.payload, array_uint32,
               BITS2WORDS(dnx_data_mdb.direct.max_payload_size_get(unit)) * sizeof(entry.payload[0]));
    SH_SAND_GET_BOOL("eedb_ll", eedb_ll);

#ifndef ADAPTER_SERVER_MODE
    SH_SAND_GET_BOOL("hit", hit_cmd);
    SH_SAND_GET_BOOL("hit_clr", hit_clr_cmd);
#endif

    if (ISEMPTY(table_name))
    {
        SHR_ERR_EXIT(_SHR_E_PARAM, "The Table parameter must be specified.\n");
    }
    else
    {
        SHR_IF_ERR_EXIT(dbal_physical_table_string_to_id(unit, table_name, &dbal_physical_table_id));

        if (dnx_data_mdb.pdbs.dbal_pdb_info_get(unit, dbal_physical_table_id)->db_type !=
            DBAL_ENUM_FVAL_MDB_DB_TYPE_EEDB)
        {
            SHR_ERR_EXIT(_SHR_E_BADID, "The provided table (%s) is not an MDB EEDB table.\n",
                         dbal_physical_table_to_string(unit, dbal_physical_table_id));
        }

        physical_tables[0] = dbal_physical_table_id;

        /*
         * Use the table default payload size if not specified
         */
        if (entry.payload_size == 0)
        {
            int basic_size;
            SHR_IF_ERR_EXIT(mdb_direct_table_get_basic_size
                            (unit,
                             dnx_data_mdb.pdbs.dbal_pdb_info_get(unit, dbal_physical_table_id)->logical_to_physical,
                             &basic_size));
            entry.payload_size = (uint8) basic_size;
        }

        SHR_IF_ERR_EXIT(utilex_bitstream_set_bit_range(entry.p_mask, 0, entry.payload_size));

        if (add_cmd != 0)
        {
            uint32 entry_flags[1] = { 0 };
            if (eedb_ll)
            {
                SHR_BITSET(entry_flags, DBAL_MDB_ENTRY_FLAGS_EEDB_LL_EXISTS);
            }
            SHR_IF_ERR_EXIT(mdb_eedb_table_entry_add(unit, physical_tables, app_id, &entry, entry_flags[0]));
            LOG_INFO(BSL_LOG_MODULE, (BSL_META_U(unit, "Add operation completed successfully on eedb table.\n")));
        }
        else if (get_cmd != 0)
        {
            int ii;
            if (hw_cmd == TRUE)
            {
                entry.mdb_action_apply = DBAL_MDB_ACTION_APPLY_HW_ONLY;
            }
#ifndef ADAPTER_SERVER_MODE
            if (hit_cmd == TRUE)
            {
                entry.hitbit |= DBAL_PHYSICAL_KEY_HITBIT_GET | DBAL_PHYSICAL_KEY_HITBIT_PRIMARY;
            }
            if (hit_clr_cmd == TRUE)
            {
                entry.hitbit |= DBAL_PHYSICAL_KEY_HITBIT_CLEAR | DBAL_PHYSICAL_KEY_HITBIT_PRIMARY;
            }
#endif
            SHR_IF_ERR_EXIT(mdb_eedb_table_entry_get(unit, dbal_physical_table_id, app_id, &entry, 0));
            LOG_INFO(BSL_LOG_MODULE, (BSL_META_U(unit, "Get operation completed successfully on eedb table.\n")));
            for (ii = 0; ii < BITS2WORDS(entry.payload_size); ii++)
            {
                LOG_INFO(BSL_LOG_MODULE,
                         (BSL_META_U(unit, "entry.payload[%d]: 0x%08x.\n"), BITS2WORDS(entry.payload_size) - 1 - ii,
                          entry.payload[BITS2WORDS(entry.payload_size) - 1 - ii]));
            }
#ifndef ADAPTER_SERVER_MODE
            if ((hit_cmd != FALSE) || (hit_clr_cmd != FALSE))
            {
                LOG_INFO(BSL_LOG_MODULE, (BSL_META_U(unit, "Hitbit: %s\n"),
                                          ((entry.hitbit & DBAL_PHYSICAL_KEY_PRIMARY_HITBIT_ACCESSED) !=
                                           0) ? "Hit" : "No-Hit"));
            }
#endif
        }
        else if (delete_cmd != 0)
        {
            SHR_IF_ERR_EXIT(mdb_eedb_table_entry_delete(unit, physical_tables, app_id, &entry, 0));
            LOG_INFO(BSL_LOG_MODULE, (BSL_META_U(unit, "Delete operation completed successfully on eedb table.\n")));
        }
        else if (dump_cmd != 0)
        {
            SHR_IF_ERR_EXIT(mdb_diag_eedb_dump(unit, dbal_physical_table_id, app_id, hw_cmd, sand_control));
        }
        else if (clear_cmd != 0)
        {
            SHR_IF_ERR_EXIT(mdb_eedb_table_clear(unit, physical_tables, app_id));
        }
        else
        {
            LOG_INFO(BSL_LOG_MODULE, (BSL_META_U(unit, "An operation must be specified.\n")));
        }
    }

exit:
    SHR_FUNC_EXIT;
}

/*
 * Dump all the entries in a specific EM table
 */
static shr_error_e
mdb_diag_em_dump(
    int unit,
    dbal_physical_tables_e dbal_physical_table_id,
    uint32 app_id,
    int hw_cmd,
    sh_sand_control_t * sand_control)
{
    dbal_physical_entry_iterator_t physical_entry_iterator;
    dbal_physical_entry_t entry;
    uint8 is_end = FALSE;

    /*
     * Used to de-allocate the iterator in case of a failure
     */
    int iterator_init = FALSE;

    PRT_INIT_VARS;

    SHR_FUNC_INIT_VARS(unit);

    /*
     * Print table header
     */
    PRT_TITLE_SET("Entries in: %s", dbal_physical_table_to_string(unit, dbal_physical_table_id));

    PRT_COLUMN_ADD("APP ID");
    PRT_COLUMN_ADD("Key Size");
    PRT_COLUMN_ADD("Key");
    PRT_COLUMN_ADD("Payload Size");
    PRT_COLUMN_ADD("Payload");
    if (mdb_em_hitbit_supported(unit, dbal_physical_table_id) == TRUE)
    {
        PRT_COLUMN_ADD("HitBit A");
        PRT_COLUMN_ADD("HitBit B");
    }
    if (mdb_em_age_supported(unit, dbal_physical_table_id) == TRUE)
    {
        PRT_COLUMN_ADD("Age");
    }


    sal_memset(&physical_entry_iterator, 0, sizeof(physical_entry_iterator));
    if (hw_cmd == TRUE)
    {
        physical_entry_iterator.mdb_action_apply = DBAL_MDB_ACTION_APPLY_HW_ONLY;
    }
    SHR_IF_ERR_EXIT(mdb_em_iterator_init(unit, dbal_physical_table_id, app_id, &physical_entry_iterator));

    iterator_init = TRUE;

    if (mdb_em_hitbit_supported(unit, dbal_physical_table_id) == TRUE)
    {
        physical_entry_iterator.hit_bit_flags |= (DBAL_PHYSICAL_KEY_HITBIT_GET | DBAL_PHYSICAL_KEY_HITBIT_PRIMARY);
    }

    if (mdb_em_age_supported(unit, dbal_physical_table_id) == TRUE)
    {
        physical_entry_iterator.age_flags |= DBAL_PHYSICAL_KEY_AGE_GET;
    }

    SHR_IF_ERR_EXIT(mdb_em_iterator_get_next
                    (unit, dbal_physical_table_id, app_id, &physical_entry_iterator, &entry, &is_end));
    while (is_end == FALSE)
    {
        PRT_ROW_ADD(PRT_ROW_SEP_NONE);
        PRT_CELL_SET("%d", physical_entry_iterator.app_id);
        PRT_CELL_SET("%d", entry.key_size);

        if (entry.key_size <= SAL_UINT32_NOF_BITS)
        {
            PRT_CELL_SET("0x%08x", entry.key[0]);
        }
        else if (entry.key_size <= 2 * SAL_UINT32_NOF_BITS)
        {
            PRT_CELL_SET("0x%08x%08x", entry.key[1], entry.key[0]);
        }
        else if (entry.key_size <= 3 * SAL_UINT32_NOF_BITS)
        {
            PRT_CELL_SET("0x%08x%08x%08x", entry.key[2], entry.key[1], entry.key[0]);
        }
        else if (entry.key_size <= 4 * SAL_UINT32_NOF_BITS)
        {
            PRT_CELL_SET("0x%08x%08x%08x%08x", entry.key[3], entry.key[2], entry.key[1], entry.key[0]);
        }
        else if (entry.key_size <= 5 * SAL_UINT32_NOF_BITS)
        {
            PRT_CELL_SET("0x%08x%08x%08x%08x%08x", entry.key[4], entry.key[3], entry.key[2],
                         entry.key[1], entry.key[0]);
        }
        else if (entry.key_size <= 6 * SAL_UINT32_NOF_BITS)
        {
            PRT_CELL_SET("0x%08x%08x%08x%08x%08x%08x", entry.key[5], entry.key[4], entry.key[3],
                         entry.key[2], entry.key[1], entry.key[0]);
        }
        else
        {
            SHR_ERR_EXIT(_SHR_E_INTERNAL, "Unexpected key size, larger than 192 bits.\n");
        }

        PRT_CELL_SET("%d", entry.payload_size);

        if (entry.payload_size <= SAL_UINT32_NOF_BITS)
        {
            PRT_CELL_SET("0x%08x", entry.payload[0]);
        }
        else if (entry.payload_size <= 2 * SAL_UINT32_NOF_BITS)
        {
            PRT_CELL_SET("0x%08x%08x", entry.payload[1], entry.payload[0]);
        }
        else if (entry.payload_size <= 3 * SAL_UINT32_NOF_BITS)
        {
            PRT_CELL_SET("0x%08x%08x%08x", entry.payload[2], entry.payload[1], entry.payload[0]);
        }
        else if (entry.payload_size <= 4 * SAL_UINT32_NOF_BITS)
        {
            PRT_CELL_SET("0x%08x%08x%08x%08x", entry.payload[3], entry.payload[2], entry.payload[1], entry.payload[0]);
        }
        else if (entry.payload_size <= 5 * SAL_UINT32_NOF_BITS)
        {
            PRT_CELL_SET("0x%08x%08x%08x%08x%08x", entry.payload[4], entry.payload[3], entry.payload[2],
                         entry.payload[1], entry.payload[0]);
        }
        else if (entry.payload_size <= 6 * SAL_UINT32_NOF_BITS)
        {
            PRT_CELL_SET("0x%08x%08x%08x%08x%08x%08x", entry.payload[5], entry.payload[4], entry.payload[3],
                         entry.payload[2], entry.payload[1], entry.payload[0]);
        }
        else
        {
            SHR_ERR_EXIT(_SHR_E_INTERNAL, "Unexpected payload size, larger than 192 bits.\n");
        }

        if (mdb_em_hitbit_supported(unit, dbal_physical_table_id) == TRUE)
        {
            PRT_CELL_SET("%s", ((entry.hitbit & DBAL_PHYSICAL_KEY_PRIMARY_HITBIT_ACCESSED_A) != 0) ? "Hit" : "No-Hit");
            PRT_CELL_SET("%s", ((entry.hitbit & DBAL_PHYSICAL_KEY_PRIMARY_HITBIT_ACCESSED_B) != 0) ? "Hit" : "No-Hit");
        }

        if (mdb_em_age_supported(unit, dbal_physical_table_id) == TRUE)
        {
            PRT_CELL_SET("0x%x", entry.age);
        }


        SHR_IF_ERR_EXIT(mdb_em_iterator_get_next
                        (unit, dbal_physical_table_id, app_id, &physical_entry_iterator, &entry, &is_end));
    }

    SHR_IF_ERR_EXIT(mdb_em_iterator_deinit(unit, dbal_physical_table_id, app_id, &physical_entry_iterator));
    iterator_init = FALSE;

    PRT_COMMITX;

exit:
    PRT_FREE;
    if (iterator_init == TRUE)
    {
        SHR_IF_ERR_CONT(mdb_em_iterator_deinit(unit, dbal_physical_table_id, app_id, &physical_entry_iterator));
    }
    SHR_FUNC_EXIT;
}

static sh_sand_man_t dnx_mdb_em_man = {
    .brief = "perform different operations on the EM table.",
    .full = "Directly perform an operation on the EM tables. "
        "Specify up to 160bits for the key using key, specify the key_size, the payload_size and the payload in case of add. "
        "If key_size is unspecified, the default is assumed. " "Payload mask is assumed to be full.",
    .synopsis =
        "mdb em add/get/delete [table=DBAL_PHYSICAL_TABLE_NAME] [key_size=0-160] [key=0xVALUE] [payload_size=1-180] [payload=0xVALUE] [app_id=VALUE]",
    .examples =
        "add table=ISEM1 key=0x12345678 payload_size=16 payload=0xabcd app_id=0\n"
        "get table=ISEM1 key=0x12345678 payload_size=16 app_id=0\n" "dump table=ISEM1\n"
#ifndef ADAPTER_SERVER_MODE
        "dump hw table=ISEM1\n"
#endif
        "delete table=ISEM1 key=0x12345678 app_id=0\n"
        "add table=LEM key=0x123456789abc payload_size=48 payload=0xabcdef0123 app_id=0\n"
        "get hw table=LEM key=0x123456789abc payload_size=48 app_id=0\n"
#ifndef ADAPTER_SERVER_MODE
        "get table=LEM key=0x123456789abc payload_size=48 hit app_id=0\n"
        "get table=LEM key=0x123456789abc payload_size=48 hit_clr app_id=0\n"
#endif
    "delete table=LEM key=0x123456789abc payload_size=48 app_id=0\n" "clear table=LEM app_id=0xFFFFFFFF"
};

static sh_sand_option_t dnx_mdb_em_options[] = {
    {"table", SAL_FIELD_TYPE_STR, "DBAL physical table name", "\n"},
    {"ADD", SAL_FIELD_TYPE_BOOL, "Perform an add operation", "no"},
    {"CLEAR", SAL_FIELD_TYPE_BOOL,
     "Perform a static entries table clear operation with the specified app_id (0xFFFFFFFF for all)", "no"},
    {"Get", SAL_FIELD_TYPE_BOOL, "Perform a get operation", "no"},
    {"hw", SAL_FIELD_TYPE_BOOL, "Force HW interaction, skipping sw cache, relevant only for dump and get", "no"},
    {"Delete", SAL_FIELD_TYPE_BOOL, "Perform a delete operation", "no"},
    {"Dump", SAL_FIELD_TYPE_BOOL, "Perform a dump operation, only static entries are dumped", "no"},
    {"key", SAL_FIELD_TYPE_ARRAY32, "The 160bit key", "0x0"},
    {"payload_size", SAL_FIELD_TYPE_UINT32, "The payload size in bits", "0"},
    {"payload", SAL_FIELD_TYPE_ARRAY32, "Up to 180bits of the payload", "0x0"},
    {"app_id", SAL_FIELD_TYPE_UINT32, "The APP ID", "0xFFFFFFFF"},
#ifndef ADAPTER_SERVER_MODE
    {"hit", SAL_FIELD_TYPE_BOOL, "Perform a hitbit operation", "no"},
    {"hit_clr", SAL_FIELD_TYPE_BOOL, "Perform a hitbit clear operation", "no"},
#endif
    {NULL}
};

/**
* \brief
* perform different operations on EM table
* See dnx_mdb_em_man for the different options.
 */
shr_error_e
cmd_dnx_mdb_em(
    int unit,
    args_t * args,
    sh_sand_control_t * sand_control)
{
    dbal_physical_tables_e physical_tables[DBAL_MAX_NUM_OF_PHY_DB_IN_TABLE] = { 0 };
    dbal_physical_tables_e dbal_physical_table_id;
    dbal_physical_entry_t entry;
    int add_cmd, get_cmd, hw_cmd, delete_cmd, clear_cmd, dump_cmd;
    char *table_name = NULL;
    uint32 app_id;
    uint32 *array_uint32;
#ifndef ADAPTER_SERVER_MODE
    int hit_cmd;
    int hit_clr_cmd;
#endif

    SHR_FUNC_INIT_VARS(unit);

    sal_memset(&entry, 0x0, sizeof(entry));

    SH_SAND_GET_STR("Table", table_name);
    SH_SAND_GET_BOOL("ADD", add_cmd);
    SH_SAND_GET_BOOL("CLEAR", clear_cmd);
    SH_SAND_GET_BOOL("Get", get_cmd);
    SH_SAND_GET_BOOL("hw", hw_cmd);
    SH_SAND_GET_BOOL("Delete", delete_cmd);
    SH_SAND_GET_BOOL("Dump", dump_cmd);

    SH_SAND_GET_ARRAY32("key", array_uint32);
    sal_memcpy(entry.key, array_uint32, MDB_MAX_EM_KEY_SIZE_IN_UINT32 * sizeof(entry.key[0]));

    SH_SAND_GET_UINT32("payload_size", entry.payload_size);
    SH_SAND_GET_ARRAY32("payload", array_uint32);
    sal_memcpy(entry.payload, array_uint32,
               BITS2WORDS(DNX_DATA_MAX_MDB_PDBS_MAX_PAYLOAD_SIZE) * sizeof(entry.payload[0]));

    SH_SAND_GET_UINT32("app_id", app_id);
#ifndef ADAPTER_SERVER_MODE
    SH_SAND_GET_BOOL("hit", hit_cmd);
    SH_SAND_GET_BOOL("hit_clr", hit_clr_cmd);
#endif

    if (ISEMPTY(table_name))
    {
        SHR_ERR_EXIT(_SHR_E_PARAM, "The Table parameter must be specified.\n");
    }
    else
    {
        shr_error_e rv;

        rv = dbal_physical_table_string_to_id(unit, table_name, &dbal_physical_table_id);
        if (rv == _SHR_E_NOT_FOUND)
        {
            /*
             * Do not return an error
             * Different devices may use different table names
             * Diag examples are not device specific
             */
            LOG_INFO(BSL_LOG_MODULE, (BSL_META_U(unit, "Table not found.\n")));
            SHR_EXIT();
        }
        else
        {
            if (rv != _SHR_E_NONE)
            {
                SHR_ERR_EXIT(rv, "dbal_physical_table_string_to_id returned an error.\n");
            }
        }

        if (dnx_data_mdb.pdbs.dbal_pdb_info_get(unit, dbal_physical_table_id)->db_type != DBAL_ENUM_FVAL_MDB_DB_TYPE_EM)
        {
            SHR_ERR_EXIT(_SHR_E_BADID, "The provided table (%s) is not an MDB EM table.\n",
                         dbal_physical_table_to_string(unit, dbal_physical_table_id));
        }

#ifndef ADAPTER_SERVER_MODE
        if (((hit_cmd == TRUE) || (hit_clr_cmd == TRUE))
            && (mdb_em_hitbit_supported(unit, dbal_physical_table_id) == FALSE))
        {
            SHR_ERR_EXIT(_SHR_E_INTERNAL,
                         "%s does not support hitbits in current MDB profile.\n", dbal_physical_table_to_string(unit,
                                                                                                                dbal_physical_table_id));
        }
#endif

        physical_tables[0] = dbal_physical_table_id;

        if (dump_cmd != 0)
        {
            SHR_IF_ERR_EXIT(mdb_diag_em_dump(unit, dbal_physical_table_id, app_id, hw_cmd, sand_control));
        }
        else if (clear_cmd != 0)
        {
            SHR_IF_ERR_EXIT(mdb_em_table_clear(unit, physical_tables, app_id));
        }
        else
        {
            /*
             * Retrieve the key size
             */
            SHR_IF_ERR_EXIT(mdb_em_get_key_size(unit, dbal_physical_table_id, app_id, &entry.key_size));

            /*
             * If default key_size is 0
             */
            if (entry.key_size == 0)
            {
                SHR_ERR_EXIT(_SHR_E_PARAM, "This app_id is not associated with this table.\n");
            }

            SHR_IF_ERR_EXIT(utilex_bitstream_set_bit_range(entry.p_mask, 0, entry.payload_size));

            if (add_cmd != 0)
            {
                SHR_IF_ERR_EXIT(mdb_em_entry_add(unit, physical_tables, app_id, &entry, 0));
                LOG_INFO(BSL_LOG_MODULE, (BSL_META_U(unit, "Add operation completed successfully on EM table.\n")));
            }
            else if (get_cmd != 0)
            {
                int ii;

                if (hw_cmd == TRUE)
                {
                    entry.mdb_action_apply = DBAL_MDB_ACTION_APPLY_HW_ONLY;
                }

#ifndef ADAPTER_SERVER_MODE
                if (hit_cmd == TRUE)
                {
                    entry.hitbit |= DBAL_PHYSICAL_KEY_HITBIT_GET | DBAL_PHYSICAL_KEY_HITBIT_PRIMARY;
                }
                if (hit_clr_cmd == TRUE)
                {
                    entry.hitbit |= DBAL_PHYSICAL_KEY_HITBIT_CLEAR | DBAL_PHYSICAL_KEY_HITBIT_PRIMARY;
                }
#endif
                if (mdb_em_age_supported(unit, dbal_physical_table_id) == TRUE)
                {
                    entry.age = DBAL_PHYSICAL_KEY_AGE_GET;
                }
                SHR_IF_ERR_EXIT(mdb_em_entry_get(unit, dbal_physical_table_id, app_id, &entry, 0));
                LOG_INFO(BSL_LOG_MODULE, (BSL_META_U(unit, "Get operation completed successfully on EM table.\n")));
                for (ii = 0; ii < BITS2WORDS(entry.payload_size); ii++)
                {
                    LOG_INFO(BSL_LOG_MODULE,
                             (BSL_META_U(unit, "entry.payload[%d]: 0x%08x.\n"), BITS2WORDS(entry.payload_size) - 1 - ii,
                              entry.payload[BITS2WORDS(entry.payload_size) - 1 - ii]));
                }
#ifndef ADAPTER_SERVER_MODE
                if ((hit_cmd != FALSE) || (hit_clr_cmd != FALSE))
                {
                    LOG_INFO(BSL_LOG_MODULE, (BSL_META_U(unit, "Hitbit: %s\n"),
                                              ((entry.hitbit & DBAL_PHYSICAL_KEY_PRIMARY_HITBIT_ACCESSED) !=
                                               0) ? "Hit" : "No-Hit"));
                }

#endif
            }
            else if (delete_cmd != 0)
            {
                SHR_IF_ERR_EXIT(mdb_em_entry_delete(unit, physical_tables, app_id, &entry, 0));
                LOG_INFO(BSL_LOG_MODULE, (BSL_META_U(unit, "Delete operation completed successfully on EM table.\n")));
            }
            else
            {
                LOG_INFO(BSL_LOG_MODULE, (BSL_META_U(unit, "An operation must be specified.\n")));
            }
        }
    }

exit:
    SHR_FUNC_EXIT;
}

static sh_sand_option_t dnx_mdb_log_severity_options[] = {
    {"Severity", SAL_FIELD_TYPE_INT32, "MDB Log Severity", "-1"},
    {NULL}
};

static sh_sand_man_t dnx_mdb_log_severity_man = {
    .brief = "Change or print the severity threshold of the logger.",
    .full =
        "The different severity thresholds are - 0: Off, 1: Fatal, 2: Error, 3: Warn, 4: Info, 5: Verbose, 6:Debug. Pass Severity=-1 to print current severity.",
    .synopsis = "[Severity=<Level/-1>]",
    .examples = "Severity=3"
};

/**
* \brief
* Change the severity of the MDB module,
* if no parameter than dump the current severity
 */
static shr_error_e
cmd_mdb_print_mode_set(
    int unit,
    args_t * args,
    sh_sand_control_t * sand_control)
{
    int val;
    bsl_severity_t severity = bslSeverityOff;

    SHR_FUNC_INIT_VARS(unit);

    SH_SAND_GET_INT32("Severity", val);

    SHR_GET_SEVERITY_FOR_MODULE(severity);
    LOG_INFO(BSL_LOG_MODULE,
             (BSL_META_U(unit, "Current MDB log severity: %s (%d)\n"), bsl_severity2str(severity), severity));

    if (val != -1)
    {
        if ((val > bslSeverityDebug) || (val < bslSeverityOff))
        {
            SHR_ERR_EXIT(_SHR_E_INTERNAL, "Severity out of bounds %d\n", val);
        }
        else
        {
            SHR_SET_SEVERITY_FOR_MODULE(val);
            LOG_INFO(BSL_LOG_MODULE, (BSL_META_U(unit, "Set MDB log severity: %s (%d)\n"), bsl_severity2str(val), val));
            if (val > bslSeverityInfo)
            {
                LOG_INFO(BSL_LOG_MODULE,
                         (BSL_META_U
                          (unit, "MDB detailed prints enabled, every MDB action will print input/output.\n")));
            }
        }
    }

exit:
    SHR_FUNC_EXIT;
}

sh_sand_man_t sh_dnx_mdb_man = {
    .brief = "MDB diagnostic commands",
    .full =
        "The dispatcher for the different MDB diagnostics commands, which allow printing different information regarding the table, manipulating the LPM/Direct/EM tables and changing the MDB logging level.",
};

/**
 * \brief MDB diagnostic pack
 * List of the supported commands, pointer to command function and command usage function.
 * This is the entry point for MDB diagnostic commands
 */
sh_sand_cmd_t sh_dnx_mdb_cmds[] = {
   /*************************************************************************************************************************************
    *   CMD_NAME    *     CMD_ACTION            * Next                    *        Options                 *         MAN                *
    *               *                           * Level                   *                                *                            *
    *               *                           * CMD                     *                                *                            *
 */
    {"Info", cmd_dnx_mdb_info_print, NULL, dnx_mdb_info_options, &dnx_mdb_info_man, NULL, NULL, SH_CMD_NO_XML_VERIFY}
    ,
    {"LPM", cmd_dnx_mdb_lpm, NULL, dnx_mdb_lpm_options, &dnx_mdb_lpm_man, NULL, NULL, SH_CMD_NO_XML_VERIFY}
    ,
    {"Direct", cmd_dnx_mdb_direct, NULL, dnx_mdb_direct_options, &dnx_mdb_direct_man, NULL, NULL, SH_CMD_NO_XML_VERIFY}
    ,
    {"EEDB", cmd_dnx_mdb_eedb, NULL, dnx_mdb_eedb_options, &dnx_mdb_eedb_man, NULL, NULL, SH_CMD_NO_XML_VERIFY}
    ,
    {"EM", cmd_dnx_mdb_em, NULL, dnx_mdb_em_options, &dnx_mdb_em_man, NULL, NULL, SH_CMD_NO_XML_VERIFY}
    ,
    {"LoGger", cmd_mdb_print_mode_set, NULL, dnx_mdb_log_severity_options, &dnx_mdb_log_severity_man, NULL, NULL,
     SH_CMD_NO_XML_VERIFY}
    ,
    {NULL}
};
