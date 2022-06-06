/*
 * $Id: jer_cnt.c Exp $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 */
#ifdef _ERR_MSG_MODULE_NAME 
    #error "_ERR_MSG_MODULE_NAME redefined" 
#endif
#define _ERR_MSG_MODULE_NAME BSL_SOC_CNT

/* 
 * Includes
 */ 

#include <shared/bsl.h>
#include <bcm/stat.h>
#include <soc/dcmn/error.h>
#include <soc/dpp/JER/jer_cnt.h>
#include <soc/dpp/drv.h>
#include <soc/dpp/QAX/qax_cnt.h>
#include <soc/dpp/mbcm.h>

#define JER_PLUS_CNT_ALLOCATION_UNIT_RESOLUTION (4096)
#define JER_PLUS_CNT_NOF_ALLOCATION_UNITS (64)
#define JER_PLUS_CNT_OFFSET_RESOLUTION (2048)


/*
 * Functions
 */

/*
 * Function:
 *      soc_jer_filter_config_egress_receive_set_get
 * Purpose:
 *      set and get filters criteria configurations in egress receive,
 *      eccentialy decides which filters criteria the CRPS will filter in and out
 * Parameters:
 *      unit -  unit number
 *      relevant_bit - the bit to set/reset in the register
 *      command_id - the CRPS support few types of command: 0,1, and special cases which has a uniqe command id. In the egress receive 0,1 and BCM_STAT_COUNTER_TM_COMMAND command IDs are supported.
 *      get - decides if get or set value
 *      value - value to set or returned value from get
 * Returns:
 *      SOC_E_XXX
 */
int soc_jer_filter_config_egress_receive_set_get(int unit, int relevant_bit, int command_id, int get, int* value)
{
    uint32 reg32_val;
    uint32 mask;
    uint32 field32_val;

    SOCDNX_INIT_FUNC_DEFS;
    SOCDNX_NULL_CHECK(value);

    /* create a mask for relevant bit */
    mask = 1U << relevant_bit;

    /* handle each command ID */
    if (command_id == BCM_STAT_COUNTER_TM_COMMAND) {
        SOCDNX_IF_ERR_EXIT(READ_CGM_CRPS_TM_PQP_DROP_REASONS_MASKr(unit, SOC_CORE_ALL, &reg32_val));
        field32_val = soc_reg_field_get(unit, CGM_CRPS_TM_PQP_DROP_REASONS_MASKr, reg32_val, CRPS_TM_PQP_DROP_REASONS_MASKf);
        if (get) {
            *value = (field32_val & mask) ? 1 : 0;
        } else {
            field32_val = (*value) ? field32_val | mask : field32_val & (~mask) ;
            soc_reg_field_set(unit, CGM_CRPS_TM_PQP_DROP_REASONS_MASKr, &reg32_val, CRPS_TM_PQP_DROP_REASONS_MASKf, field32_val);
            SOCDNX_IF_ERR_EXIT(WRITE_CGM_CRPS_TM_PQP_DROP_REASONS_MASKr(unit, SOC_CORE_ALL, reg32_val));
        }        
    } else if (command_id == 0) {
        SOCDNX_IF_ERR_EXIT(READ_CGM_CRPS_PP_1_PQP_DROP_REASONS_MASKr(unit, SOC_CORE_ALL, &reg32_val));
        field32_val = soc_reg_field_get(unit, CGM_CRPS_PP_1_PQP_DROP_REASONS_MASKr, reg32_val, CRPS_PP_1_PQP_DROP_REASONS_MASKf);
        if (get) {
            *value = (field32_val & mask) ? 1 : 0;
        } else {
            field32_val = (*value) ? field32_val | mask : field32_val & (~mask) ;
            soc_reg_field_set(unit, CGM_CRPS_PP_1_PQP_DROP_REASONS_MASKr, &reg32_val, CRPS_PP_1_PQP_DROP_REASONS_MASKf, field32_val);
            SOCDNX_IF_ERR_EXIT(WRITE_CGM_CRPS_PP_1_PQP_DROP_REASONS_MASKr(unit, SOC_CORE_ALL, reg32_val));
        }
    } else if (command_id == 1) {
        SOCDNX_IF_ERR_EXIT(READ_CGM_CRPS_PP_2_PQP_DROP_REASONS_MASKr(unit, SOC_CORE_ALL, &reg32_val));
        field32_val = soc_reg_field_get(unit, CGM_CRPS_PP_2_PQP_DROP_REASONS_MASKr, reg32_val, CRPS_PP_2_PQP_DROP_REASONS_MASKf);
        if (get) {
            *value = (field32_val & mask) ? 1 : 0;
        } else {
            field32_val = (*value) ? field32_val | mask : field32_val & (~mask) ;
            soc_reg_field_set(unit, CGM_CRPS_PP_2_PQP_DROP_REASONS_MASKr, &reg32_val, CRPS_PP_2_PQP_DROP_REASONS_MASKf, field32_val);
            SOCDNX_IF_ERR_EXIT(WRITE_CGM_CRPS_PP_2_PQP_DROP_REASONS_MASKr(unit, SOC_CORE_ALL, reg32_val));
        }        
    } else {
        LOG_ERROR(BSL_LS_SOC_CNT, (BSL_META_U(unit, "egress receive counter doesn't support command id %d\n"), command_id));
        SOCDNX_IF_ERR_EXIT(SOC_E_PARAM);
    }

exit:
    SOCDNX_FUNC_RETURN;
}


/*
 * Function:
 *      soc_jer_filter_config_ingress_set_get
 * Purpose:
 *      set and get filters criteria configurations in ingress,
 *      eccentialy decides which filters criteria the CRPS will filter in or out
 * Parameters:
 *      unit -  unit number
 *      relevant_bit - the bit to set/reset in the register
 *      command_id - the CRPS support few types of command, 0,1, and special cases which has a uniqe command id, here only 0 and 1 are supported.
 *      get - decides if get or set
 *      value - value to set, or returned value from get
 * Returns:
 *      SOC_E_XXX
 */
int soc_jer_filter_config_ingress_set_get(int unit, int relevant_bit, int command_id, int get, int* value)
{
    uint64 reg64_val;
    uint32 mask;
    uint32 field32_val;

    SOCDNX_INIT_FUNC_DEFS;

    /* create a mask for relevant bit */
    mask = 1U << relevant_bit;
    
    /* handle each command ID */
    if (command_id == 0) {
        SOCDNX_IF_ERR_EXIT(READ_IQM_IQM_CNT_CMD_CONFIG_Ar(unit, SOC_CORE_ALL, &reg64_val));
        field32_val = soc_reg64_field32_get(unit, IQM_IQM_CNT_CMD_CONFIG_Ar, reg64_val, IQM_CNT_CMD_ERRORS_FILTER_Af);
        if (get) {
            *value = (field32_val & mask) ? 1 : 0;
        } else {
            field32_val = (*value) ? field32_val | mask : field32_val & (~mask) ;
            soc_reg64_field32_set(unit, IQM_IQM_CNT_CMD_CONFIG_Ar, &reg64_val, IQM_CNT_CMD_ERRORS_FILTER_Af, field32_val);
            SOCDNX_IF_ERR_EXIT(WRITE_IQM_IQM_CNT_CMD_CONFIG_Ar(unit, SOC_CORE_ALL, reg64_val));
        }
    } else if (command_id == 1) {
        SOCDNX_IF_ERR_EXIT(READ_IQM_IQM_CNT_CMD_CONFIG_Br(unit, SOC_CORE_ALL, &reg64_val));
        field32_val = soc_reg64_field32_get(unit, IQM_IQM_CNT_CMD_CONFIG_Br, reg64_val, IQM_CNT_CMD_ERRORS_FILTER_Bf);
        if (get) {
            *value = (field32_val & mask) ? 1 : 0;
        } else {
            field32_val = (*value) ? field32_val | mask : field32_val & (~mask) ;
            soc_reg64_field32_set(unit, IQM_IQM_CNT_CMD_CONFIG_Br, &reg64_val, IQM_CNT_CMD_ERRORS_FILTER_Bf, field32_val);
            SOCDNX_IF_ERR_EXIT(WRITE_IQM_IQM_CNT_CMD_CONFIG_Br(unit, SOC_CORE_ALL, reg64_val));
        }        
    } else {
        LOG_ERROR(BSL_LS_SOC_CNT, (BSL_META_U(unit, "ingress counter doesn't support command id %d\n"), command_id));
        SOCDNX_IF_ERR_EXIT(SOC_E_PARAM);
    }

exit:
    SOCDNX_FUNC_RETURN;
}


/*
 * Function:
 *      soc_jer_map_filter_to_bit_and_source_type
 * Purpose:
 *      map a filter criterion to the relevant bit that controls it, and find the relevant source types for it
 * Parameters:
 *      unit -  unit number
 *      filter - filter criterion to map
 *      relevant_bit - the bit to set/reset in the register
 *      engine_source_max_count - size of array
 *      engine_source_array - array to store matching engine sources for drop reason
 *      engine_source_count - amount of found matches
 * Returns:
 *      SOC_E_XXX
 */
int soc_jer_map_filter_to_bit_and_source_type(int                               unit, 
                                              SOC_TMC_CNT_FILTER_TYPE           filter, 
                                              int*                              relevant_bit, 
                                              int                               engine_source_max_count, 
                                              SOC_TMC_CNT_SRC_TYPE*             engine_source_array, 
                                              int*                              engine_source_count)
{
    int is_egq = 0;
    int is_iqm = 0;

    SOCDNX_INIT_FUNC_DEFS;
    SOCDNX_NULL_CHECK(relevant_bit);
    SOCDNX_NULL_CHECK(engine_source_array);
    SOCDNX_NULL_CHECK(engine_source_count);

    /* set relevant info for given filter */
    switch (filter) {
    case SOC_TMC_CNT_TOTAL_PDS_THRESHOLD_VIOLATED:
        *relevant_bit = 0;
        is_egq = 1;
        break;
    case SOC_TMC_CNT_TOTAL_PDS_UC_POOL_SIZE_THRESHOLD_VIOLATED:
        *relevant_bit = 1;
        is_egq = 1;
        break;
    case SOC_TMC_CNT_PER_PORT_UC_PDS_THRESHOLD_VIOLATED:
        *relevant_bit = 2;
        is_egq = 1;
        break;
    case SOC_TMC_CNT_PER_QUEUE_UC_PDS_THRESHOLD_VIOLATED:
        *relevant_bit = 3;
        is_egq = 1;
        break;
    case SOC_TMC_CNT_PER_PORT_UC_DBS_THRESHOLD_VIOLATED:
        *relevant_bit = 4;
        is_egq = 1;
        break;
    case SOC_TMC_CNT_PER_QUEUE_UC_DBS_THRESHOLD_VIOLATED:
        *relevant_bit = 5;
        is_egq = 1;
        break;
    case SOC_TMC_CNT_PER_QUEUE_DISABLE_BIT:
        *relevant_bit = 6;
        is_egq = 1;
        break;
    case SOC_TMC_CNT_TOTAL_PDS_MC_POOL_SIZE_THRESHOLD_VIOLATED:
        *relevant_bit = 8;
        is_egq = 1;
        break;
    case SOC_TMC_CNT_PER_INTERFACE_PDS_THREHOLD_VIOLATED:
        *relevant_bit = 9;
        is_egq = 1;
        break;
    case SOC_TMC_CNT_MC_SP_THRESHOLD_VIOLATED:
        *relevant_bit = 10;
        is_egq = 1;
        break;
    case SOC_TMC_CNT_PER_MC_TC_THRESHOLD_VIOLATED:
        *relevant_bit = 11;
        is_egq = 1;
        break;
    case SOC_TMC_CNT_MC_PDS_PER_PORT_THRESHOLD_VIOLATED:
        *relevant_bit = 12;
        is_egq = 1;
        break;
    case SOC_TMC_CNT_MC_PDS_PER_QUEUE_THRESHOLD_VIOLATED:
        *relevant_bit = 13;
        is_egq = 1;
        break;
    case SOC_TMC_CNT_MC_PER_PORT_SIZE_THRESHOLD_VIOLATED:
        *relevant_bit = 14;
        is_egq = 1;
        break;
    case SOC_TMC_CNT_MC_PER_QUEUE_SIZE_THRESHOLD_VIOLATED:
        *relevant_bit = 15;
        is_egq = 1;
        break;
    case SOC_TMC_CNT_GLOBAL_REJECT_DISCARDS:
        *relevant_bit = 0;
        is_iqm = 1;
        break;
    case SOC_TMC_CNT_DRAM_REJECT_DISCARDS:
        *relevant_bit = 1;
        is_iqm = 1;
        break;
    case SOC_TMC_CNT_VOQ_TAIL_DROP_DISCARDS:
        *relevant_bit = 2;
        is_iqm = 1;
        break;
    case SOC_TMC_CNT_VOQ_STATISTICS_DISCARDS:
        *relevant_bit = 3;
        is_iqm = 1;
        break;
    case SOC_TMC_CNT_VSQ_TAIL_DROP_DISCARDS:
        *relevant_bit = 4;
        is_iqm = 1;
        break;
    case SOC_TMC_CNT_VSQ_STATISTICS_DISCARDS:
        *relevant_bit = 5;
        is_iqm = 1;
        break;
    case SOC_TMC_CNT_QUEUE_NOT_VALID_DISCARD:
        *relevant_bit = 6;
        is_iqm = 1;
        break;
    case SOC_TMC_CNT_OTHER_DISCARDS:
        *relevant_bit = 7;
        is_iqm = 1;
        break;
    default:
        LOG_ERROR(BSL_LS_BCM_CNT, (BSL_META_U(unit, "filter criterion %d is invalid\n"), filter));
        SOCDNX_IF_ERR_EXIT(SOC_E_PARAM);
    }

    /* set possible engine source types accoridng to info(egq/iqm...) */
    if (engine_source_max_count < 1){
        LOG_ERROR(BSL_LS_BCM_CNT, (BSL_META_U(unit, "insufficant size in engine_source_array\n")));
        SOCDNX_IF_ERR_EXIT(SOC_E_FULL);
    }

    if (is_egq) {
        engine_source_array[0] = SOC_TMC_CNT_SRC_TYPE_EGR_PP;
        *engine_source_count = 1;
    } else if (is_iqm) {
        engine_source_array[0] = SOC_TMC_CNT_SRC_TYPE_VOQ;
        *engine_source_count = 1;
    }

exit:
    SOCDNX_FUNC_RETURN;
}

/*
 * Function:
 *      soc_jer_stat_counter_filter_set
 * Purpose:
 *      set filters (values are filtered out or in to the count) for source (source_type and command_id).
 * Parameters:
 *      unit -  unit number
 *      source - engine source to set filter for (command_id and source type)
 *      filter_array - filters to filter
 *      filter_count - nof members in filter array
 *      is_active - if set activate filters (filter out), else deactivate filters (filter in)
 * Returns:
 *      SOC_E_XXX
 */
int soc_jer_stat_counter_filter_set(SOC_SAND_IN int                             unit, 
                                    SOC_SAND_IN SOC_TMC_CNT_SOURCE*             source, 
                                    SOC_SAND_IN SOC_TMC_CNT_FILTER_TYPE*        filter_array, 
                                    SOC_SAND_IN int                             filter_count,
                                    SOC_SAND_IN int                             is_active )
{

    int filter_iter = 0;
    int engine_source_iter = 0;
    int engine_source_count = 0;
    int relevant_bit = 0;
    int found_match = 0;
    int value = is_active ? 0 : 1;
    int command_id = source->command_id;
    SOC_TMC_CNT_SRC_TYPE engine_source = source->source_type;
    SOC_TMC_CNT_SRC_TYPE engine_source_array[SOC_TMC_CNT_NOF_SRC_TYPES_JERICHO];

    SOCDNX_INIT_FUNC_DEFS;

    /* iterate over all filter types */
    for (filter_iter = 0; filter_iter < filter_count; ++filter_iter) 
    {
        /* map filter to relevant engines and bits to change */
        SOCDNX_IF_ERR_EXIT(soc_jer_map_filter_to_bit_and_source_type(unit, filter_array[filter_iter], &relevant_bit, SOC_TMC_CNT_NOF_SRC_TYPES_JERICHO, engine_source_array, &engine_source_count));
        /* iterate over found engines, and try to match to input engine */
        for (engine_source_iter = 0, found_match = 0; engine_source_iter < engine_source_count; ++engine_source_iter) 
        {
            /* continue if not match */
            if (engine_source_array[engine_source_iter] != engine_source) 
            {
                continue;
            }

            found_match = 1;
            /* when finding a match, activate per engine type function */
            if (engine_source == SOC_TMC_CNT_SRC_TYPE_EGR_PP){
                SOCDNX_IF_ERR_EXIT(MBCM_DPP_DRIVER_CALL(unit, mbcm_dpp_cnt_filter_config_egress_receive_set_get,(unit, relevant_bit, command_id, 0, &value)));                
            } 
            else if (engine_source == SOC_TMC_CNT_SRC_TYPE_VOQ) 
            {
                SOCDNX_IF_ERR_EXIT(MBCM_DPP_DRIVER_CALL(unit, mbcm_dpp_cnt_filter_config_ingress_set_get,(unit, relevant_bit, command_id, 0, &value)));
            } else {
                LOG_ERROR(BSL_LS_BCM_CNT, (BSL_META_U(unit, "engine source %d doesn't support any filters\n"), engine_source));
                SOCDNX_IF_ERR_EXIT(SOC_E_PARAM);        
            }
            break;
        }

        if (found_match != 1) 
        {
            LOG_ERROR(BSL_LS_BCM_CNT, (BSL_META_U(unit, "engine type %d and filter criterion %d doesn't match invalid\n"), engine_source, filter_array[filter_iter]));
            SOCDNX_IF_ERR_EXIT(SOC_E_PARAM);
        }
    }
    
exit:
    SOCDNX_FUNC_RETURN;
}



/*
 * Function:
 *      soc_jer_stat_counter_filter_get
 * Purpose:
 *      get active filters (values are filtered out of the count) for source (source_type and command_id).
 * Parameters:
 *      unit -  unit number
 *      source - engine source to set filter for (command_id and source type)
 *      filter_max_count - size of filter array
 *      filter_array - returned found filters
 *      filter_count - nof members found
 * Returns:
 *      SOC_E_XXX
 */
int soc_jer_stat_counter_filter_get(SOC_SAND_IN  int                            unit, 
                                    SOC_SAND_IN  SOC_TMC_CNT_SOURCE*            source, 
                                    SOC_SAND_IN  int                            filter_max_count,
                                    SOC_SAND_OUT SOC_TMC_CNT_FILTER_TYPE*       filter_array, 
                                    SOC_SAND_OUT int*                           filter_count )
{
    int filter_iter = 0;
    int engine_source_iter = 0;
    int engine_source_count = 0;
    int relevant_bit = 0;
    int value;
    int found_filter_index = 0;
    int command_id = source->command_id;
    SOC_TMC_CNT_SRC_TYPE engine_source = source->source_type;
    SOC_TMC_CNT_SRC_TYPE engine_source_array[SOC_TMC_CNT_NOF_SRC_TYPES_JERICHO];

    SOCDNX_INIT_FUNC_DEFS;

    /* iterate over all filter types */
    for (filter_iter = 0; filter_iter < SOC_TMC_CNT_DROP_REASON_COUNT; ++filter_iter) 
    {
        /* map filter to relevant engines and bits to change */
        SOCDNX_IF_ERR_EXIT(soc_jer_map_filter_to_bit_and_source_type(unit, filter_iter, &relevant_bit, SOC_TMC_CNT_NOF_SRC_TYPES_JERICHO, engine_source_array, &engine_source_count));
        /* iterate over found engines, and try to match to input engine */
        for (engine_source_iter = 0; engine_source_iter < engine_source_count; ++engine_source_iter) 
        {
            /* continue if not match */
            if (engine_source_array[engine_source_iter] != engine_source) 
            {
                continue;
            }

            /* when finding a match, activate per engine type function */
            if (engine_source == SOC_TMC_CNT_SRC_TYPE_EGR_PP){
                SOCDNX_IF_ERR_EXIT(MBCM_DPP_DRIVER_CALL(unit, mbcm_dpp_cnt_filter_config_egress_receive_set_get,(unit, relevant_bit, command_id, 1, &value)));                                
            }
            else if (engine_source == SOC_TMC_CNT_SRC_TYPE_VOQ) 
            {
                SOCDNX_IF_ERR_EXIT(MBCM_DPP_DRIVER_CALL(unit, mbcm_dpp_cnt_filter_config_ingress_set_get,(unit, relevant_bit, command_id, 1, &value)));
            }
            else 
            {
                LOG_ERROR(BSL_LS_BCM_CNT, (BSL_META_U(unit, "engine source %d doesn't support any filters\n"), engine_source));
                SOCDNX_IF_ERR_EXIT(SOC_E_PARAM);
            }

            /* fill returned filter array with filter criterion which is filtered out */
            if (!value) 
            {
                if (found_filter_index < filter_max_count) 
                {
                    filter_array[found_filter_index] = filter_iter;
                    ++found_filter_index;
                } else {
                    LOG_ERROR(BSL_LS_BCM_CNT, (BSL_META_U(unit, "insufficant size of filter_array\n")));
                    SOCDNX_IF_ERR_EXIT(SOC_E_FULL);
                }
            }
            break;
        }
    }

    /* update amount of found members */
    *filter_count = found_filter_index;
    
exit:
    SOCDNX_FUNC_RETURN;
}

/*
 * Function:
 *      soc_jer_cnt_ingress_compensation_profile_delta_set
 * Purpose:
 *      set the memory IQM_CNTCMD_CPM / IQM_IRPP_CNTCMD_CPM
 *      part of packetSize compensation feature
 * Parameters:
 * SOC_SAND_IN int       unit,
 * SOC_SAND_IN int       core,
 * SOC_SAND_IN int       src_type
 * SOC_SAND_IN int       profileIndex,
 * SOC_SAND_IN int       delta
 * Returns:
 *      SOC_E_XXX
 */
int soc_jer_cnt_ingress_compensation_profile_delta_set(SOC_SAND_IN int unit, SOC_SAND_IN int core, SOC_SAND_IN int src_type, SOC_SAND_IN int profileIndex, SOC_SAND_IN int delta)
{
    uint32 data;
    uint32 ctr_profile_cmp;
    int rv;
    SOCDNX_INIT_FUNC_DEFS;

    if((delta < MIN_ING_COMP_DELTA_VALUE) || (delta > MAX_ING_COMP_DELTA_VALUE))
    {
        LOG_ERROR(BSL_LS_SOC_CNT, (BSL_META_U(unit, "delta=%d, out of the legal values\n"), delta));
        SOCDNX_IF_ERR_EXIT(SOC_E_INTERNAL);
    }
    if (profileIndex < 0)
    {
        LOG_ERROR(BSL_LS_BCM_CNT, (BSL_META_U(unit, "negative profile index\n")));
        SOCDNX_IF_ERR_EXIT(SOC_E_PARAM);
    }
    if (src_type == SOC_PKT_SIZE_ADJUST_SRC_CRPS_IQM)
    {
        rv = READ_IQM_CNTCMD_CPMm(unit, IQM_BLOCK(unit,core), profileIndex , &data);
        SOCDNX_IF_ERR_EXIT(rv);
        ctr_profile_cmp = CONVERT_SIGNED_NUM_TO_TWO_COMPLEMENT_METHOD(delta,EIGHT_BITS);
        soc_IQM_CNTCMD_CPMm_field32_set(unit, &data, CTR_PROFILE_CMPf, ctr_profile_cmp);
        rv = WRITE_IQM_CNTCMD_CPMm(unit, IQM_BLOCK(unit,core), profileIndex, &data);
        SOCDNX_IF_ERR_EXIT(rv);
    }
    else if (src_type == SOC_PKT_SIZE_ADJUST_SRC_CRPS_IQM_IRPP)
    {
        rv = READ_IQM_IRPP_CNTCMD_CPMm(unit, IQM_BLOCK(unit,core), profileIndex , &data);
        SOCDNX_IF_ERR_EXIT(rv);
        ctr_profile_cmp = CONVERT_SIGNED_NUM_TO_TWO_COMPLEMENT_METHOD(delta, EIGHT_BITS);
        soc_IQM_IRPP_CNTCMD_CPMm_field32_set(unit, &data, IRPP_CTR_PROFILE_CMPf, ctr_profile_cmp);
        rv = WRITE_IQM_IRPP_CNTCMD_CPMm(unit, IQM_BLOCK(unit,core), profileIndex, &data);
        SOCDNX_IF_ERR_EXIT(rv);
    } else if (src_type == SOC_PKT_SIZE_ADJUST_SRC_STAT_REPOR_IN) 
    {
        rv = READ_IQMT_ING_RPT_PCMm(unit, IQMT_BLOCK(unit), profileIndex , &data);
        SOCDNX_IF_ERR_EXIT(rv);
        ctr_profile_cmp = CONVERT_SIGNED_NUM_TO_TWO_COMPLEMENT_METHOD(delta,EIGHT_BITS);
        soc_IQMT_ING_RPT_CPMm_field32_set(unit, &data, IQM_ING_PROFILE_CMPf, ctr_profile_cmp); 
        rv = WRITE_IQMT_ING_RPT_CPMm(unit, IQMT_BLOCK(unit), profileIndex, &data);
        SOCDNX_IF_ERR_EXIT(rv);
    } 
    else if (src_type == SOC_PKT_SIZE_ADJUST_SRC_STAT_REPOR_OUT) 
    {
        rv = READ_IQMT_EGR_RPT_PCMm(unit, IQMT_BLOCK(unit), profileIndex , &data);
        SOCDNX_IF_ERR_EXIT(rv);
        ctr_profile_cmp = CONVERT_SIGNED_NUM_TO_TWO_COMPLEMENT_METHOD(delta,EIGHT_BITS);
        soc_IQMT_EGR_RPT_CPMm_field32_set(unit, &data, EGR_PROFILE_CMPf, ctr_profile_cmp); 
        rv = WRITE_IQMT_EGR_RPT_CPMm(unit, IQMT_BLOCK(unit), profileIndex, &data);
        SOCDNX_IF_ERR_EXIT(rv);
    }else 
    {
        LOG_ERROR(BSL_LS_BCM_CNT, (BSL_META_U(unit, "src_type=%d. allowed types: SOC_COMPENSATION_PKT_SIZE_SRC_IQM_IRPP|SOC_COMPENSATION_PKT_SIZE_SRC_IQM\n"), src_type));
        SOCDNX_IF_ERR_EXIT(SOC_E_PARAM);
    }


exit:
    SOCDNX_FUNC_RETURN;
}


/*
 * Function:
 *      soc_jer_cnt_ingress_compensation_port_profile_set
 * Purpose:
 *      set the memory IQM_CNTCMD_PCM / IQM_IRPP_CNTCMD_PCM
 *      part of packetSize compensation feature
 * Parameters:
 *  SOC_SAND_IN  int                   unit,
 *  SOC_SAND_IN  int                   core,
 *  SOC_SAND_IN  int                   commandId,
 *  SOC_SAND_IN  int                   src_type,
 *  SOC_SAND_IN  int                   port, (can be specific ports or MAX_PORTS_IN_CORE in order to configure all ports)
 *  SOC_SAND_IN  int                   profileIndex
 * Returns:
 *      SOC_E_XXX
 */
int soc_jer_cnt_ingress_compensation_port_profile_set(SOC_SAND_IN  int unit, SOC_SAND_IN  int core, SOC_SAND_IN  int commandId, SOC_SAND_IN  int src_type, SOC_SAND_IN  int port, SOC_SAND_IN  int profileIndex)
{
    uint32 data, dw_enable;
    int nof_loops, rv;
    uint32 port_index;

    SOCDNX_INIT_FUNC_DEFS;

    if (profileIndex < 0)
    {
        LOG_ERROR(BSL_LS_BCM_CNT, (BSL_META_U(unit, "negative profile index\n")));
        SOCDNX_IF_ERR_EXIT(SOC_E_PARAM);
    }
    if (port < 0)
    {
        LOG_ERROR(BSL_LS_BCM_CNT, (BSL_META_U(unit, "negative port index\n")));
        SOCDNX_IF_ERR_EXIT(SOC_E_PARAM);
    }
    /* update all ports */
    if(port == MAX_PORTS_IN_CORE)
    {
        nof_loops = MAX_PORTS_IN_CORE;
        port_index = 0;
    }
    else /* update specific port */
    {
        nof_loops = 1;
        port_index = (uint32)port;
    }

    if (src_type == SOC_PKT_SIZE_ADJUST_SRC_CRPS_IQM)
    {
        while (nof_loops > 0)
        {
            rv = READ_IQM_CNTCMD_PCMm(unit, IQM_BLOCK(unit,core), port_index , &data);
            SOCDNX_IF_ERR_EXIT(rv);
            if(commandId == 0)
            {
                soc_IQM_CNTCMD_PCMm_field32_set(unit, &data, IPP_CTR_A_CMP_PROFILEf, profileIndex);
            }
            else
            {
                soc_IQM_CNTCMD_PCMm_field32_set(unit, &data, IPP_CTR_B_CMP_PROFILEf, profileIndex);
            }

            rv = WRITE_IQM_CNTCMD_PCMm(unit, IQM_BLOCK(unit,core), port_index, &data);
            SOCDNX_IF_ERR_EXIT(rv);
            port_index++;
            nof_loops--;
        }
    }
    else if (src_type == SOC_PKT_SIZE_ADJUST_SRC_CRPS_IQM_IRPP)
    {
        while (nof_loops > 0)
        {
            rv = READ_IQM_IRPP_CNTCMD_PCMm(unit, IQM_BLOCK(unit,core), port_index , &data);
            SOCDNX_IF_ERR_EXIT(rv);
            if(commandId == 0)
            {
                soc_IQM_IRPP_CNTCMD_PCMm_field32_set(unit, &data, IPP_IRPP_CTR_A_CMP_PROFILEf, profileIndex);
            }
            else
            {
                soc_IQM_IRPP_CNTCMD_PCMm_field32_set(unit, &data, IPP_IRPP_CTR_B_CMP_PROFILEf, profileIndex);
            }

            rv = WRITE_IQM_IRPP_CNTCMD_PCMm(unit, IQM_BLOCK(unit,core), port_index, &data);
            SOCDNX_IF_ERR_EXIT(rv);
            port_index++;
            nof_loops--;
        }
    }
    else if (src_type == SOC_PKT_SIZE_ADJUST_SRC_STAT_REPOR_IN) 
    {
        while (nof_loops > 0)
        {
            rv = READ_IQMT_ING_RPT_PCMm(unit, IQMT_BLOCK(unit), port_index , &data);
            SOCDNX_IF_ERR_EXIT(rv);
            if(core == 0)
            {
                soc_IQMT_ING_RPT_PCMm_field32_set(unit, &data, IQM_0_ING_CMP_PROFILEf, profileIndex);
            }
            else
            {
                soc_IQMT_ING_RPT_PCMm_field32_set(unit, &data, IQM_1_ING_CMP_PROFILEf, profileIndex);
            }

            SOCDNX_IF_ERR_EXIT(READ_IQMT_ENABLE_DYNAMIC_MEMORY_ACCESSr(unit, &dw_enable));
            if (!dw_enable) {
                SOCDNX_IF_ERR_EXIT(WRITE_IQMT_ENABLE_DYNAMIC_MEMORY_ACCESSr(unit, 0x1)); 
            }
            rv = WRITE_IQMT_ING_RPT_PCMm(unit, IQMT_BLOCK(unit), port_index, &data);
            SOCDNX_IF_ERR_EXIT(rv);
            if (!dw_enable) {
                SOCDNX_IF_ERR_EXIT(WRITE_IQMT_ENABLE_DYNAMIC_MEMORY_ACCESSr(unit, 0x0));
            }
            port_index++;
            nof_loops--;
        }
    }
    else if (src_type == SOC_PKT_SIZE_ADJUST_SRC_STAT_REPOR_OUT) 
    {
        while (nof_loops > 0)
        {
            rv = READ_IQMT_EGR_RPT_PCMm(unit, IQMT_BLOCK(unit), port_index , &data);
            SOCDNX_IF_ERR_EXIT(rv);
            if(core == 0)
            {
                soc_IQMT_EGR_RPT_PCMm_field32_set(unit, &data, EGR_0_CMP_PROFILEf, profileIndex);
            }
            else
            {
                soc_IQMT_EGR_RPT_PCMm_field32_set(unit, &data, EGR_1_CMP_PROFILEf, profileIndex);
            }

            rv = WRITE_IQMT_EGR_RPT_PCMm(unit, IQMT_BLOCK(unit), port_index, &data);
            SOCDNX_IF_ERR_EXIT(rv);
            port_index++;
            nof_loops--;
        }
    }
    else 
    {
        LOG_ERROR(BSL_LS_BCM_CNT, (BSL_META_U(unit, "src_type=%d. allowed types: SOC_COMPENSATION_PKT_SIZE_SRC_IQM_IRPP|SOC_COMPENSATION_PKT_SIZE_SRC_IQM\n"), src_type));
        SOCDNX_IF_ERR_EXIT(SOC_E_PARAM);
    }


exit:
    SOCDNX_FUNC_RETURN;
}


/*
 * Function:
 *      soc_jer_cnt_ingress_compensation_outLif_delta_set
 * Purpose:
 *      set the memory IQM_CNTCMD_HAPM / IQM_IRPP_CNTCMD_HAPM .
 *      part of packetSize compensation feature
 * Parameters:
 *  SOC_SAND_IN  int                   unit,
 *  SOC_SAND_IN  int                   core,
 *  SOC_SAND_IN  int                   commandId,
 *  SOC_SAND_IN  int                   src_type,
 *  SOC_SAND_IN  int                   lifIndex,
 *  SOC_SAND_IN  int                   delta
 * Returns:
 *      SOC_E_XXX
 */
int soc_jer_cnt_ingress_compensation_outLif_delta_set(SOC_SAND_IN  int unit, SOC_SAND_IN  int core, SOC_SAND_IN  int commandId, SOC_SAND_IN  int src_type, SOC_SAND_IN  int lifIndex, SOC_SAND_IN  int delta)
{
    uint32 data;
    int rv;
    int coreIndex;
    int baseLifIndex;
    SOCDNX_INIT_FUNC_DEFS;

    if(lifIndex > MAX_ING_COMP_LIF_NUMBER || lifIndex < 0)
    {
        LOG_ERROR(BSL_LS_BCM_CNT, (BSL_META_U(unit, "lifIndex=%d. allowed values 0..31\n"), lifIndex));
        SOCDNX_IF_ERR_EXIT(SOC_E_PARAM);
    }

    baseLifIndex = (lifIndex & 0x1F);
    if(src_type == SOC_PKT_SIZE_ADJUST_SRC_CRPS_IQM)
    {
        SOC_DPP_CORES_ITER(core, coreIndex)
        {
                rv = READ_IQM_CNTCMD_HAPMm(unit, IQM_BLOCK(unit,coreIndex), baseLifIndex , &data);
                SOCDNX_IF_ERR_EXIT(rv);
                if(commandId == 0)
                {
                    soc_IQM_CNTCMD_HAPMm_field32_set(unit, &data, CTR_A_HEADER_APPEND_CMPf, CONVERT_SIGNED_NUM_TO_TWO_COMPLEMENT_METHOD(delta, EIGHT_BITS));
                }
                else
                {
                    soc_IQM_CNTCMD_HAPMm_field32_set(unit, &data, CTR_B_HEADER_APPEND_CMPf, CONVERT_SIGNED_NUM_TO_TWO_COMPLEMENT_METHOD(delta, EIGHT_BITS));
                }

                rv = WRITE_IQM_CNTCMD_HAPMm(unit, IQM_BLOCK(unit,coreIndex), baseLifIndex, &data);
                SOCDNX_IF_ERR_EXIT(rv);
        }
    }
    else if (src_type == SOC_PKT_SIZE_ADJUST_SRC_CRPS_IQM_IRPP)
    {
        SOC_DPP_CORES_ITER(core, coreIndex)
        {
                rv = READ_IQM_IRPP_CNTCMD_HAPMm(unit, IQM_BLOCK(unit,coreIndex), baseLifIndex , &data);
                SOCDNX_IF_ERR_EXIT(rv);
                if(commandId == 0)
                {
                    soc_IQM_IRPP_CNTCMD_HAPMm_field32_set(unit, &data, IRPP_CTR_A_HEADER_APPEND_CMPf, CONVERT_SIGNED_NUM_TO_TWO_COMPLEMENT_METHOD(delta, EIGHT_BITS));
                }
                else
                {
                    soc_IQM_IRPP_CNTCMD_HAPMm_field32_set(unit, &data, IRPP_CTR_B_HEADER_APPEND_CMPf, CONVERT_SIGNED_NUM_TO_TWO_COMPLEMENT_METHOD(delta, EIGHT_BITS));
                }

                rv = WRITE_IQM_IRPP_CNTCMD_HAPMm(unit, IQM_BLOCK(unit,coreIndex), baseLifIndex, &data);
                SOCDNX_IF_ERR_EXIT(rv);
        }
    } else if (src_type == SOC_PKT_SIZE_ADJUST_SRC_STAT_REPOR_IN) {
        SOC_DPP_CORES_ITER(core, coreIndex)
        {
                rv = READ_IQMT_ING_RPT_HAPMm(unit, IQMT_BLOCK(unit), baseLifIndex , &data);
                SOCDNX_IF_ERR_EXIT(rv);
                if(coreIndex == 0)
                {
                    soc_IQMT_ING_RPT_HAPMm_field32_set(unit, &data, IQM_0_ING_HEADER_APPEND_CMPf, CONVERT_SIGNED_NUM_TO_TWO_COMPLEMENT_METHOD(delta, EIGHT_BITS));
                }
                else
                {
                    soc_IQMT_ING_RPT_HAPMm_field32_set(unit, &data, IQM_1_ING_HEADER_APPEND_CMPf, CONVERT_SIGNED_NUM_TO_TWO_COMPLEMENT_METHOD(delta, EIGHT_BITS));
                }
                rv = WRITE_IQMT_ING_RPT_HAPMm(unit, IQMT_BLOCK(unit), baseLifIndex, &data);
                SOCDNX_IF_ERR_EXIT(rv);
        }
    } else if (src_type == SOC_PKT_SIZE_ADJUST_SRC_STAT_REPOR_OUT) {
        SOC_DPP_CORES_ITER(core, coreIndex)
        {
                rv = READ_IQMT_EGR_RPT_HAPMm(unit, IQMT_BLOCK(unit), baseLifIndex , &data);
                SOCDNX_IF_ERR_EXIT(rv);
                if(coreIndex == 0)
                {
                    soc_IQMT_EGR_RPT_HAPMm_field32_set(unit, &data, EGR_0_HEADER_APPEND_CMPf, CONVERT_SIGNED_NUM_TO_TWO_COMPLEMENT_METHOD(delta, EIGHT_BITS));
                }
                else
                {
                    soc_IQMT_EGR_RPT_HAPMm_field32_set(unit, &data, EGR_1_HEADER_APPEND_CMPf, CONVERT_SIGNED_NUM_TO_TWO_COMPLEMENT_METHOD(delta, EIGHT_BITS));
                }

                rv = WRITE_IQMT_EGR_RPT_HAPMm(unit, IQMT_BLOCK(unit), baseLifIndex, &data);
                SOCDNX_IF_ERR_EXIT(rv);
        }
    } else {
        LOG_ERROR(BSL_LS_BCM_CNT, (BSL_META_U(unit, "src_type=%d. allowed types: SOC_COMPENSATION_PKT_SIZE_SRC_IQM_IRPP|SOC_COMPENSATION_PKT_SIZE_SRC_IQM\n"), src_type));
        SOCDNX_IF_ERR_EXIT(SOC_E_PARAM);
    }
    

exit:
    SOCDNX_FUNC_RETURN;
}



/*
 * Function:
 *      soc_jer_cnt_ingress_compensation_outLif_delta_get
 * Purpose:
 *      get the memory IQM_CNTCMD_HAPM / IQM_IRPP_CNTCMD_HAPM .
 *      part of packetSize compensation feature
 * Parameters:
 *  SOC_SAND_IN  int                   unit,
 *  SOC_SAND_IN  int                   core,
 *  SOC_SAND_IN  int                   commandId,
 *  SOC_SAND_IN  int                   src_type,
 *  SOC_SAND_IN  int                   lifIndex,
 *  SOC_SAND_OUT int*                  delta
 * Returns:
 *      SOC_E_XXX
 */
int soc_jer_cnt_ingress_compensation_outLif_delta_get(SOC_SAND_IN  int unit, SOC_SAND_IN  int core, SOC_SAND_IN  int commandId, SOC_SAND_IN  int src_type, SOC_SAND_IN  int lifIndex, SOC_SAND_OUT  int * delta)
{
    uint32 memData, fieldData;
    int rv;
    SOCDNX_INIT_FUNC_DEFS;

    if(lifIndex >= MAX_ING_COMP_LIF_NUMBER || lifIndex < 0)
    {
        LOG_ERROR(BSL_LS_BCM_CNT, (BSL_META_U(unit, "lifIndex=%d. allowed values 0..31\n"), lifIndex));
        SOCDNX_IF_ERR_EXIT(SOC_E_PARAM);
    }

    if(src_type == SOC_PKT_SIZE_ADJUST_SRC_CRPS_IQM)
    {
        rv = READ_IQM_CNTCMD_HAPMm(unit, IQM_BLOCK(unit,core), (lifIndex & 0x1F) , &memData);
        SOCDNX_IF_ERR_EXIT(rv);
        if(commandId == 0)
        {
            fieldData = soc_IQM_CNTCMD_HAPMm_field32_get(unit, &memData, CTR_A_HEADER_APPEND_CMPf);
        }
        else
        {
            fieldData = soc_IQM_CNTCMD_HAPMm_field32_get(unit, &memData, CTR_B_HEADER_APPEND_CMPf);
        }
    }
    else if (src_type == SOC_PKT_SIZE_ADJUST_SRC_CRPS_IQM_IRPP)
    {
        rv = READ_IQM_IRPP_CNTCMD_HAPMm(unit, IQM_BLOCK(unit,core), (lifIndex & 0x1F) , &memData);
        SOCDNX_IF_ERR_EXIT(rv);
        if(commandId == 0)
        {
            fieldData = soc_IQM_IRPP_CNTCMD_HAPMm_field32_get(unit, &memData, IRPP_CTR_A_HEADER_APPEND_CMPf);
        }
        else
        {
            fieldData = soc_IQM_IRPP_CNTCMD_HAPMm_field32_get(unit, &memData, IRPP_CTR_B_HEADER_APPEND_CMPf);
        }
    }
    else if (src_type == SOC_PKT_SIZE_ADJUST_SRC_STAT_REPOR_IN) 
    {
        rv = READ_IQMT_ING_RPT_HAPMm(unit, IQMT_BLOCK(unit), (lifIndex & 0x1F) , &memData);
        SOCDNX_IF_ERR_EXIT(rv);
        if(core == 0)
        {
            fieldData = soc_IQMT_ING_RPT_HAPMm_field32_get(unit, &memData, IQM_0_ING_HEADER_APPEND_CMPf);
        }
        else
        {
            fieldData = soc_IQMT_ING_RPT_HAPMm_field32_get(unit, &memData, IQM_1_ING_HEADER_APPEND_CMPf);
        }
    }
    else if (src_type == SOC_PKT_SIZE_ADJUST_SRC_STAT_REPOR_OUT) 
    {
        rv = READ_IQMT_EGR_RPT_HAPMm(unit, IQMT_BLOCK(unit), (lifIndex & 0x1F) , &memData);
        SOCDNX_IF_ERR_EXIT(rv);
        if(core == 0)
        {
            fieldData = soc_IQMT_EGR_RPT_HAPMm_field32_get(unit, &memData, EGR_0_HEADER_APPEND_CMPf);
        }
        else
        {
            fieldData = soc_IQMT_EGR_RPT_HAPMm_field32_get(unit, &memData, EGR_1_HEADER_APPEND_CMPf);
        }
    }
    else
    {
        LOG_ERROR(BSL_LS_BCM_CNT, (BSL_META_U(unit, "src_type=%d. allowed types: SOC_COMPENSATION_PKT_SIZE_SRC_IQM_IRPP|SOC_COMPENSATION_PKT_SIZE_SRC_IQM\n"), src_type));
        SOCDNX_IF_ERR_EXIT(SOC_E_PARAM);
    }

    * delta = CONVERT_TWO_COMPLEMENT_INTO_SIGNED_NUM(fieldData, EIGHT_BITS);
exit:
    SOCDNX_FUNC_RETURN;
}


/*
 * Function:
 *      soc_jer_cnt_ingress_compensation_port_delta_and_profile_get
 * Purpose:
 *      get the memory IQM_CNTCMD_PCM / IQM_IRPP_CNTCMD_PCM / IQM_CNTCMD_CPM / IQM_IRPP_CNTCMD_CPM .
 *      part of packetSize compensation feature
 * Parameters:
 *  SOC_SAND_IN  int                   unit,
 *  SOC_SAND_IN  int                   core,
 *  SOC_SAND_IN  int                   commandId,
 *  SOC_SAND_IN  int                   src_type,
 *  SOC_SAND_IN  int                   port,
 *  SOC_SAND_OUT int*                  profileIndex,
 *  SOC_SAND_OUT int*                  delta
 * Returns:
 *      SOC_E_XXX
 */
int soc_jer_cnt_ingress_compensation_port_delta_and_profile_get(SOC_SAND_IN  int unit, SOC_SAND_IN  int core, SOC_SAND_IN  int commandId, SOC_SAND_IN  int src_type, SOC_SAND_IN  int port, SOC_SAND_OUT  int * profileIndex,  SOC_SAND_OUT  int * delta)
{
    uint32 memData, fieldData;
    int rv;
    SOCDNX_INIT_FUNC_DEFS;


    if(src_type == SOC_PKT_SIZE_ADJUST_SRC_CRPS_IQM)
    {
        rv = READ_IQM_CNTCMD_PCMm(unit, IQM_BLOCK(unit,core), port , &memData);
        SOCDNX_IF_ERR_EXIT(rv);
        if(commandId == 0)
        {
            * profileIndex = (int)soc_IQM_CNTCMD_PCMm_field32_get(unit, &memData, IPP_CTR_A_CMP_PROFILEf);
        }
        else
        {
            * profileIndex = (int)soc_IQM_CNTCMD_PCMm_field32_get(unit, &memData, IPP_CTR_B_CMP_PROFILEf);
        }

        rv = READ_IQM_CNTCMD_CPMm(unit, IQM_BLOCK(unit,core), *profileIndex , &memData);
        SOCDNX_IF_ERR_EXIT(rv);
        fieldData = soc_IQM_CNTCMD_CPMm_field32_get(unit, &memData, CTR_PROFILE_CMPf);
        * delta = CONVERT_TWO_COMPLEMENT_INTO_SIGNED_NUM(fieldData, EIGHT_BITS);
    }
    else if (src_type == SOC_PKT_SIZE_ADJUST_SRC_CRPS_IQM_IRPP)
    {
        rv = READ_IQM_IRPP_CNTCMD_PCMm(unit, IQM_BLOCK(unit,core), port , &memData);
        SOCDNX_IF_ERR_EXIT(rv);
        if(commandId == 0)
        {
            * profileIndex = (int)soc_IQM_IRPP_CNTCMD_PCMm_field32_get(unit, &memData, IPP_IRPP_CTR_A_CMP_PROFILEf);
        }
        else
        {
            * profileIndex = (int)soc_IQM_IRPP_CNTCMD_PCMm_field32_get(unit, &memData, IPP_IRPP_CTR_B_CMP_PROFILEf);
        }

        rv = READ_IQM_IRPP_CNTCMD_CPMm(unit, IQM_BLOCK(unit,core), *profileIndex , &memData);
        SOCDNX_IF_ERR_EXIT(rv);
        fieldData = soc_IQM_IRPP_CNTCMD_CPMm_field32_get(unit, &memData, IRPP_CTR_PROFILE_CMPf);
        * delta = CONVERT_TWO_COMPLEMENT_INTO_SIGNED_NUM(fieldData, EIGHT_BITS);
    } else if (src_type == SOC_PKT_SIZE_ADJUST_SRC_STAT_REPOR_IN) 
    {
        rv = READ_IQMT_ING_RPT_PCMm(unit, IQMT_BLOCK(unit), port , &memData);
        SOCDNX_IF_ERR_EXIT(rv);
        if(core == 0)
        {
            * profileIndex = (int)soc_IQMT_ING_RPT_PCMm_field32_get(unit, &memData, IQM_0_ING_CMP_PROFILEf);
        }
        else
        {
            * profileIndex = (int)soc_IQMT_ING_RPT_PCMm_field32_get(unit, &memData, IQM_1_ING_CMP_PROFILEf);
        }

        rv = READ_IQMT_ING_RPT_CPMm(unit, IQMT_BLOCK(unit), *profileIndex , &memData);
        SOCDNX_IF_ERR_EXIT(rv);
        fieldData = soc_IQMT_ING_RPT_CPMm_field32_get(unit, &memData, IQM_ING_PROFILE_CMPf);
        * delta = CONVERT_TWO_COMPLEMENT_INTO_SIGNED_NUM(fieldData, EIGHT_BITS);
    } else if (src_type == SOC_PKT_SIZE_ADJUST_SRC_STAT_REPOR_OUT) 
    {
        rv = READ_IQMT_EGR_RPT_PCMm(unit, IQMT_BLOCK(unit), port , &memData);
        SOCDNX_IF_ERR_EXIT(rv);
        if(core == 0)
        {
            * profileIndex = (int)soc_IQMT_EGR_RPT_PCMm_field32_get(unit, &memData, EGR_0_CMP_PROFILEf);
        }
        else
        {
            * profileIndex = (int)soc_IQMT_EGR_RPT_PCMm_field32_get(unit, &memData, EGR_1_CMP_PROFILEf);
        }

        rv = READ_IQMT_EGR_RPT_CPMm(unit, IQMT_BLOCK(unit), *profileIndex , &memData);
        SOCDNX_IF_ERR_EXIT(rv);
        fieldData = soc_IQMT_EGR_RPT_CPMm_field32_get(unit, &memData, EGR_PROFILE_CMPf);
        * delta = CONVERT_TWO_COMPLEMENT_INTO_SIGNED_NUM(fieldData, EIGHT_BITS);
    } else 
    {
        LOG_ERROR(BSL_LS_BCM_CNT, (BSL_META_U(unit, "src_type=%d. allowed types: SOC_COMPENSATION_PKT_SIZE_SRC_IQM_IRPP|SOC_COMPENSATION_PKT_SIZE_SRC_IQM\n"), src_type));
        SOCDNX_IF_ERR_EXIT(SOC_E_PARAM);
    }

exit:
    SOCDNX_FUNC_RETURN;
}

/*
 * Function:
 *      soc_jer_stat_egress_receive_tm_pointer_format_set
 * Purpose:
 *      set the value of the mask and shift fields of Queue-Pair, TC, UC/MC that the user set to register EGQ_PQP_CRPS_CONF
 * Parameters:
 *  SOC_SAND_IN   int    unit,
 *  SOC_SAND_IN   uint32 queue_pair_mask,
 *  SOC_SAND_IN   uint32 traffic_class_mask,
 *  SOC_SAND_IN   uint32 cast_mask,
 *  SOC_SAND_OUT  uint32* queue_pair_shift,
 *  SOC_SAND_OUT  uint32* traffic_class_shift,
 *  SOC_SAND_OUT  uint32* cast_shift,
 * Returns:
 *      SOC_E_XXX
 */
int soc_jer_stat_egress_receive_tm_pointer_format_set(SOC_SAND_IN  int unit,
                                                      SOC_SAND_IN  uint32 queue_pair_mask,
                                                      SOC_SAND_IN  uint32 traffic_class_mask,
                                                      SOC_SAND_IN  uint32 cast_mask,
                                                      SOC_SAND_OUT uint32* queue_pair_shift,
                                                      SOC_SAND_OUT uint32* traffic_class_shift,
                                                      SOC_SAND_OUT uint32* cast_shift){

    uint32 reg32_val;
    uint32 cast_shift_local, traffic_class_shift_local, queue_pair_shift_local;
    int tc_bits,cast_bits,queue_pair_bits;
    SOCDNX_INIT_FUNC_DEFS;

    SHR_BITCOUNT_RANGE(&traffic_class_mask, tc_bits, 0, 3);
    SHR_BITCOUNT_RANGE(&cast_mask, cast_bits, 0, 1);
    SHR_BITCOUNT_RANGE(&queue_pair_mask, queue_pair_bits, 0, 8);

    if (SOC_IS_QUX(unit)) {
        if((queue_pair_bits + tc_bits + cast_bits) > 10) {
            LOG_ERROR(BSL_LS_SOC_CNT, (BSL_META_U(unit, "in QUX cannot fit more than 10bits for pointer - counters 1K /n")));
        }
    }
    /* calculate the shift for the 3 fields */
    cast_shift_local = 0; /* cast is not shifted */
    traffic_class_shift_local = cast_bits;
    queue_pair_shift_local = cast_bits + tc_bits;
    
    /* read  -> set all fields -> write */
    SOCDNX_IF_ERR_EXIT(READ_EGQ_PQP_CRPS_CONFr(unit, SOC_CORE_ALL, &reg32_val));

    soc_reg_field_set(unit, EGQ_PQP_CRPS_CONFr, &reg32_val, CRPS_TM_PTR_QNUM_MASKf, queue_pair_mask);
    soc_reg_field_set(unit, EGQ_PQP_CRPS_CONFr, &reg32_val, CRPS_TM_PTR_SYS_TC_MASKf, traffic_class_mask);
    soc_reg_field_set(unit, EGQ_PQP_CRPS_CONFr, &reg32_val, CRPS_TM_PTR_CAST_MASKf, cast_mask);
    soc_reg_field_set(unit, EGQ_PQP_CRPS_CONFr, &reg32_val, CRPS_TM_PTR_QNUM_SHIFTf, queue_pair_shift_local);
    soc_reg_field_set(unit, EGQ_PQP_CRPS_CONFr, &reg32_val, CRPS_TM_PTR_SYS_TC_SHIFTf, traffic_class_shift_local);
    soc_reg_field_set(unit, EGQ_PQP_CRPS_CONFr, &reg32_val, CRPS_TM_PTR_CAST_SHIFTf, cast_shift_local);
    /* write to the reg*/
    SOCDNX_IF_ERR_EXIT(WRITE_EGQ_PQP_CRPS_CONFr(unit, SOC_CORE_ALL, reg32_val));

    *cast_shift = cast_shift_local;
    *traffic_class_shift = traffic_class_shift_local;
    *queue_pair_shift = queue_pair_shift_local;

exit:
    SOCDNX_FUNC_RETURN;
}

/*
 * Function:
 *      soc_jer_stat_egress_receive_tm_pointer_format_get
 * Purpose:
 *      get the value of the masks of Queue-Pair, TC, UC/MC from register EGQ_PQP_CRPS_CONF
 * Parameters:
 *  SOC_SAND_IN   int    unit,
 *  SOC_SAND_OUT  uint32 queue_pair_mask,
 *  SOC_SAND_OUT  uint32 traffic_class_mask,
 *  SOC_SAND_OUT  uint32 cast_mask,
 * Returns:
 *      SOC_E_XXX
 */
int soc_jer_stat_egress_receive_tm_pointer_format_get(SOC_SAND_IN  int unit, SOC_SAND_OUT uint32* queue_pair_mask, SOC_SAND_OUT uint32* traffic_class_mask, SOC_SAND_OUT uint32* cast_mask){

    uint32 reg32_val;
    SOCDNX_INIT_FUNC_DEFS;

    SOCDNX_IF_ERR_EXIT(READ_EGQ_PQP_CRPS_CONFr(unit, SOC_CORE_ALL, &reg32_val));
    *queue_pair_mask = soc_reg_field_get(unit, EGQ_PQP_CRPS_CONFr, reg32_val, CRPS_TM_PTR_QNUM_MASKf);
    *traffic_class_mask = soc_reg_field_get(unit, EGQ_PQP_CRPS_CONFr, reg32_val, CRPS_TM_PTR_SYS_TC_MASKf);
    *cast_mask = soc_reg_field_get(unit, EGQ_PQP_CRPS_CONFr, reg32_val, CRPS_TM_PTR_CAST_MASKf);
exit:
    SOCDNX_FUNC_RETURN;
}

/***********************************************************************************************************************
 * Function:
 *      jer_plus_cnt_out_lif_counting_range_set
 * Purpose:
 *     set the Allocation unit table according to the table itself and the new input.
 *     Each time this function called, the offset in the table will be calculated from scratch.
 *     First 8 AUs, are save for RIF
 * Parameters:
 *      unit -  unit number
 *      SOC_TMC_CNT_SRC_TYPE source
 *      crps_counter_id
 *      SOC_SAND_U32_RANGE*  profile_range - the range of the new profile to set.
 *      stif_counter_id
 *      is_double_entry
 * Returns:
 *      SOC_E_XXX
 * Notes:
 *      even thought allocation unit contain 4 counters, we set only one counter that belong to the CRPS and one pointer tha belong to the statistic interface.
 ***********************************************************************************************************************/
int jer_plus_cnt_out_lif_counting_range_set(
    SOC_SAND_IN int unit, 
    SOC_SAND_IN SOC_TMC_CNT_SRC_TYPE source,    
    SOC_SAND_IN int crps_counter_id, 
    SOC_SAND_IN SOC_SAND_U32_RANGE  *profile_range,    
    SOC_SAND_IN int stif_counter_id,    
    SOC_SAND_IN int is_double_entry
    )
{
    int i, base_aloocation_unit;
    uint32 reg_val;
    uint32 offset;
    
    SOCDNX_INIT_FUNC_DEFS;

    /* update the entire parameters in the table for the new profile, except the offsets */
    base_aloocation_unit = profile_range->start/JER_PLUS_CNT_ALLOCATION_UNIT_RESOLUTION;
    for(i = profile_range->start/JER_PLUS_CNT_ALLOCATION_UNIT_RESOLUTION; i < (profile_range->end/JER_PLUS_CNT_ALLOCATION_UNIT_RESOLUTION + 1); i++)
    {
        READ_EPNI_COUNTER_SOURCE_MAPm(unit, EPNI_BLOCK(unit,SOC_CORE_ALL), i, &reg_val);
        soc_EPNI_COUNTER_SOURCE_MAPm_field32_set(unit, &reg_val, COUNT_ON_ETPP_0f, (crps_counter_id == 0));
        soc_EPNI_COUNTER_SOURCE_MAPm_field32_set(unit, &reg_val, COUNT_ON_ETPP_1f, (crps_counter_id == 1));
        soc_EPNI_COUNTER_SOURCE_MAPm_field32_set(unit, &reg_val, COUNT_ON_ETPP_2f, (stif_counter_id == 0));
        soc_EPNI_COUNTER_SOURCE_MAPm_field32_set(unit, &reg_val, COUNT_ON_ETPP_3f, (stif_counter_id == 1));
        soc_EPNI_COUNTER_SOURCE_MAPm_field32_set(unit, &reg_val, DOUBLE_ENTRYf, (is_double_entry == TRUE));
        /* if double entry, we add offset to the start of the range */
        if(is_double_entry)
        {
            offset = (base_aloocation_unit/2)/JER_PLUS_CNT_OFFSET_RESOLUTION;
            if(offset >= 64)
            {
                LOG_ERROR(BSL_LS_SOC_CNT, (BSL_META_U(unit, "offset=%d, max allowed=63\n"), offset));
                SOCDNX_IF_ERR_EXIT(SOC_E_PARAM);            
            }
            soc_EPNI_COUNTER_SOURCE_MAPm_field32_set(unit, &reg_val,POINTER_BASEf, offset);                        
        }
        WRITE_EPNI_COUNTER_SOURCE_MAPm(unit, EPNI_BLOCK(unit,SOC_CORE_ALL), i, &reg_val);        
    }   

exit:
    SOCDNX_FUNC_RETURN;
}


/***********************************************************************************************************************
 * Function:
 *      jer_plus_cnt_out_lif_counting_range_get
 * Purpose:
 *     get the  parameters relevant for specific allocation unit
 * Parameters:
 *      unit      - (IN)  Unit number.
 *      range     - (IN/OUT) The range configuration.
 *      offset     - (OUT) relevant only for J+, return the offset configure for that range
 *     command_id   - (OUT) relevant only for J+, return the command_id configure for that range
 *     command_id   - (OUT) relevant only for J+, return the stif_id configure for that range
 * Returns:
 *      SOC_E_XXX
 * Notes:
 *      
 ***********************************************************************************************************************/
int jer_plus_cnt_out_lif_counting_range_get(
    SOC_SAND_IN int                       unit,
    SOC_SAND_INOUT SOC_SAND_U32_RANGE*    range,
    SOC_SAND_OUT uint32*                  offset,
    SOC_SAND_OUT int*                     command_id,
    SOC_SAND_OUT int*                     stif_id,
    SOC_SAND_OUT int*                     is_double_entry
    )

{  
    uint32 reg_val;

    SOCDNX_INIT_FUNC_DEFS;
        
    SOCDNX_IF_ERR_EXIT(READ_EPNI_COUNTER_SOURCE_MAPm(unit, EPNI_BLOCK(unit,SOC_CORE_ALL), range->start/JER_PLUS_CNT_ALLOCATION_UNIT_RESOLUTION, &reg_val));
    if(command_id != NULL)
    {
        if(soc_EPNI_COUNTER_SOURCE_MAPm_field32_get(unit, &reg_val, COUNT_ON_ETPP_0f))
        {
            *command_id = 0;
        }
        else if(soc_EPNI_COUNTER_SOURCE_MAPm_field32_get(unit, &reg_val, COUNT_ON_ETPP_1f))
        {
            *command_id = 1;
        } 
        else
        {
            *command_id = -1;            
        }
    }
    if(stif_id != NULL)
    {
        if(soc_EPNI_COUNTER_SOURCE_MAPm_field32_get(unit, &reg_val, COUNT_ON_ETPP_2f))
        {
            *stif_id = 0;
        }
        else if(soc_EPNI_COUNTER_SOURCE_MAPm_field32_get(unit, &reg_val, COUNT_ON_ETPP_3f))
        {
            *stif_id = 1;    
        }   
        else
        {
            *stif_id = -1;
        }
    }
    if(offset != NULL)
    {
        *offset = JER_PLUS_CNT_OFFSET_RESOLUTION*soc_EPNI_COUNTER_SOURCE_MAPm_field32_get(unit, &reg_val, POINTER_BASEf);        
    }
    if(is_double_entry != NULL)
    {
        *is_double_entry = soc_EPNI_COUNTER_SOURCE_MAPm_field32_get(unit, &reg_val, DOUBLE_ENTRYf);        
    }
    
exit:
    SOCDNX_FUNC_RETURN;

}


