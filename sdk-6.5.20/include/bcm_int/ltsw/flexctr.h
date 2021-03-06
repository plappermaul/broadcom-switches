/*
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated.
 * Edits to this file will be lost when it is regenerated.
 *
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 */

#ifndef BCMI_LTSW_FLEXCTR_H
#define BCMI_LTSW_FLEXCTR_H

#include <bcm/flexctr.h>

#include <sal/sal_types.h>

/*! Invalid flex counter action. */
#define BCMI_LTSW_FLEXCTR_ACTION_INVALID (0)

/*! Options internal. */
#define BCMI_LTSW_FLEXCTR_OPTIONS_INTERNAL (1 << 31)

/*! Ingress flex counter stage. */
#define BCMI_LTSW_FLEXCTR_STAGE_ING_CTR (0x0)

/*! Egress flex counter stage. */
#define BCMI_LTSW_FLEXCTR_STAGE_EGR_CTR (0x1)

/*! Flex counter stage count. */
#define BCMI_LTSW_FLEXCTR_STAGE_COUNT (0x2)

/*! Ingress flex counter. */
#define BCMI_LTSW_FLEXCTR_DIR_INGRESS (0x0)

/*! Egress flex counter. */
#define BCMI_LTSW_FLEXCTR_DIR_EGRESS (0x1)

/*! Flex counter direction count. */
#define BCMI_LTSW_FLEXCTR_DIR_COUNT (0x2)

/*! Maximum packet attribute selector number. */
#define BCMI_LTSW_FLEXCTR_PKT_ATTR_SELECTOR_NUM_MAX (8)

/*! Maximum packet attribute object instance number. */
#define BCMI_LTSW_FLEXCTR_PKT_ATTR_OBJECT_INSTANCE_NUM_MAX (256)

/*!
 * \brief Internal flex counter source.
 */
typedef enum bcmi_ltsw_flexctr_source_s {

    /*! First internal source. */
    bcmiFlexctrSourceStart = 512,

    /*! Dest L2 source. */
    bcmiFlexctrSourceL2Dst = 512,

    /*! Src L2 source. */
    bcmiFlexctrSourceL2Src = 513,

    /*! Exact match flow tracker. */
    bcmiFlexctrSourceEM_FT = 514,

    /*! Last internal source. */
    bcmiFlexctrSourceEnd = 515

} bcmi_ltsw_flexctr_source_t;

#define BCMI_LTSW_FLEXCTR_SOURCE_STR \
{ \
    "Start", \
    "L2Dst", \
    "L2Src", \
    "EM_FT", \
    "End" \
}

/*!
 * \brief Counter informaiton.
 */
typedef struct bcmi_ltsw_flexctr_counter_info_s {

    /*! Stage id. */
    int stage;

    /*! Counter direction. */
    int direction;

    /*! Pipeline id. */
    int pipe;

    /*! h/w action index. */
    uint32_t action_index;

    /*! action profile id. */
    uint64_t action_profile_id;

    /*! Source enumeration in HSDK API. */
    int source;

    /*! h/w source table name. */
    const char *table_name;

    /*! Total counter index number. */
    uint32_t index_num;

    /*! First allocated counter pool. */
    uint32_t start_pool;

    /*! Last allocated counter pool. */
    uint32_t end_pool;

    /*! Flex counter action. */
    bcm_flexctr_action_t act_cfg;

    /*! Indicate the specified counter id is a group action id. */
    bool group;

    /*! Indicate the specified counter id is a single pipe action. */
    bool single_pipe;

} bcmi_ltsw_flexctr_counter_info_t;

/*!
 * \brief Hit control.
 */
typedef struct bcmi_ltsw_flexctr_hit_ctrl_s {

    /*! Stage id. */
    int stage;

    /*! table name. */
    const char *tbl_name;

    /*! Dest group action. */
    uint32_t dst_grp_id;

    /*! Src group action. */
    uint32_t src_grp_id;

    /*! Indicate global action in pipe unique mode. */
    bool global;

    /*! Destination action profile ID. */
    uint32_t dst_act_profile_id;

    /*! Source action profile ID. */
    uint32_t src_act_profile_id;

} bcmi_ltsw_flexctr_hit_ctrl_t;

/*!
 * \brief Flexctr mode.
 */
typedef enum bcmi_ltsw_flexctr_mode_s {

    /*! Global mode. */
    bcmiFlexctrModeGlobal = 0,

    /*! Pipe unique mode. */
    bcmiFlexctrModePipeUnique = 1,

    /*! Last one, not valid. */
    bcmiFlexctrModeCount = 2

} bcmi_ltsw_flexctr_mode_t;

#define BCMI_LTSW_FLEXCTR_MODE_STR \
{ \
    "Global", \
    "PipeUnique" \
}

/*!
 * \brief Flex counter stat info.
 */
typedef struct bcmi_ltsw_flexctr_stat_info_s {

    /*! The maximum num of entries in a flex counter stage. */
    uint32_t total_entries;

    /*! The sum of all free entries that can be allocated for a source. */
    uint32_t free_entries;

} bcmi_ltsw_flexctr_stat_info_t;

/*!
 * \brief Packet attribute object instance information.
 */
typedef struct bcmi_ltsw_flexctr_pkt_attr_obj_inst_s {

    /*! Packet attribute object value. */
    int object_value;

    /*! Number of packet attribute map entries. */
    uint32_t num_entries;

    /*! Packet attribute map entry selector type array. */
    int map_entry_type[BCMI_LTSW_FLEXCTR_PKT_ATTR_SELECTOR_NUM_MAX];

    /*! Packet attribute map entry selector value array. */
    uint32_t map_entry_value[BCMI_LTSW_FLEXCTR_PKT_ATTR_SELECTOR_NUM_MAX];

} bcmi_ltsw_flexctr_pkt_attr_obj_inst_t;

/*!
 * \brief Packet attribute informaiton.
 */
typedef struct bcmi_ltsw_flexctr_packet_attribute_info_s {

    /*! Stage id. */
    int stage;

    /*! Counter direction. */
    int direction;

    /*! Pipeline id. */
    int pipe;

    /*! H/W packet attribute index. */
    uint32_t pkt_attr_index;

    /*! Packet attribute profile id. */
    uint64_t pkt_attr_profile_id;

    /*! Maximum packet attribute object instance number. */
    uint32_t obj_inst_max;

    /*! Packet attribute object instance array. */
    bcmi_ltsw_flexctr_pkt_attr_obj_inst_t obj_inst[BCMI_LTSW_FLEXCTR_PKT_ATTR_OBJECT_INSTANCE_NUM_MAX];

} bcmi_ltsw_flexctr_packet_attribute_info_t;

/*!
 * \brief Get a counter id information.
 *
 * \param [in] unit Unit Number.
 * \param [in] stat_counter_id Flex counter action identifier.
 * \param [out] info Counter information.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
bcmi_ltsw_flexctr_counter_id_info_get(
    int unit,
    uint32_t stat_counter_id,
    bcmi_ltsw_flexctr_counter_info_t *info);

/*!
 * \brief Update an attached counter id status.
 *
 * \param [in] unit Unit Number.
 * \param [in] stat_counter_id Flex counter action identifier.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
bcmi_ltsw_flexctr_attach_counter_id_status_update(
    int unit,
    uint32_t stat_counter_id);

/*!
 * \brief Update a detached counter id status.
 *
 * \param [in] unit Unit Number.
 * \param [in] stat_counter_id Flex counter action identifier.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
bcmi_ltsw_flexctr_detach_counter_id_status_update(
    int unit,
    uint32_t stat_counter_id);

/*!
 * \brief Get a flex counter stat counter id.
 *
 * \param [in] unit Unit Number.
 * \param [in] info Counter information.
 * \param [out] stat_counter_id Flex counter action identifier.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
bcmi_ltsw_flexctr_counter_id_get(
    int unit,
    bcmi_ltsw_flexctr_counter_info_t *info,
    uint32_t *stat_counter_id);

/*!
 * \brief Hit control update.
 *
 * \param [in] unit Unit Number.
 * \param [in] ctrl Hit control.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
bcmi_ltsw_flexctr_hit_control_update(
    int unit,
    bcmi_ltsw_flexctr_hit_ctrl_t *ctrl);

/*!
 * \brief Flex counter action update.
 *
 * \param [in] unit Unit Number.
 * \param [in] stat_counter_id Flex counter action identifier.
 * \param [in] act_cfg Flex counter action.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
bcmi_ltsw_flexctr_action_update(
    int unit,
    uint32_t stat_counter_id,
    bcm_flexctr_action_t *act_cfg);

/*!
 * \brief Update flex counter info for field group.
 *
 * \param [in] unit Unit Number.
 * \param [in] source Flex counter source.
 * \param [in] group_id Field group id.
 * \param [in] old_pri Old priority.
 * \param [in] new_pri New priority.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
bcmi_ltsw_flexctr_field_group_update(
    int unit,
    bcm_flexctr_source_t source,
    int group_id,
    int old_pri,
    int new_pri);

/*!
 * \brief Get flexctr mode.
 *
 * \param [in] unit Unit Number.
 * \param [in] egr Egress or ingress pipeline, 1 for egress.
 * \param [out] mode Flexctr mode.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
bcmi_ltsw_flexctr_mode_get(
    int unit,
    bool egr,
    bcmi_ltsw_flexctr_mode_t *mode);

/*!
 * \brief Retrieve flex counter info like total entries num, free entries num for a source.
 *
 * \param [in] unit Unit Number.
 * \param [in] source Flexctr source.
 * \param [in] pipe Pipe id. -1 indicates global.
 * \param [out] info Flex counter stat info.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
bcmi_ltsw_flexctr_stat_info_get(
    int unit,
    bcm_flexctr_source_t source,
    int pipe,
    bcmi_ltsw_flexctr_stat_info_t *info);

/*!
 * \brief Get a packet attribute profile id information.
 *
 * \param [in] unit Unit Number.
 * \param [in] profile_id Flex counter action identifier.
 * \param [out] info Packet attribute information.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
bcmi_ltsw_flexctr_packet_attribute_id_info_get(
    int unit,
    int profile_id,
    bcmi_ltsw_flexctr_packet_attribute_info_t *info);

/*!
 * \brief Dump flexctr module sw information.
 *
 * \param [in] unit Unit Number.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
bcmi_ltsw_flexctr_sw_dump(int unit);

/*!
 * \brief Flex counter group action get function.
 *
 * \param [in] unit Unit Number.
 * \param [in] group_stat_counter_id Flex counter group ID.
 * \param [out] group_action Flex counter group action data structure.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
bcmi_ltsw_flexctr_group_action_get(
    int unit,
    uint32_t group_stat_counter_id,
    bcm_flexctr_group_action_t *group_action);

/*!
 * \brief Flex counter action is a single pipe action. Return value: 1 for yes, 0 for no, others indicate error.
 *
 * \param [in] unit Unit Number.
 * \param [in] stage Flex counter stage id.
 * \param [in] action_index Hardware action index.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
bcmi_ltsw_flexctr_action_is_single_pipe(
    int unit,
    int stage,
    uint32_t action_index);

/*!
 * \brief Get field opaque object value for specific flex counter action and counter index.
 *
 * \param [in] unit Unit Number.
 * \param [in] stat_counter_id Flex counter action identifier.
 * \param [in] counter_index Flex counter index.
 * \param [in] size Element number of object value array.
 * \param [out] obj_val Object value array.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
bcmi_ltsw_flexctr_field_opaque_object_value_get(
    int unit,
    uint32_t stat_counter_id,
    uint32_t counter_index,
    uint32_t size,
    uint32_t *obj_val);

/*!
 * \brief Egress flex counter pre-select set.
 *
 * \param [in] unit Unit Number.
 * \param [in] val Pre-select value.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
bcmi_ltsw_flexctr_egr_presel_set(
    int unit,
    int val);

/*!
 * \brief Egress flex counter pre-select get.
 *
 * \param [in] unit Unit Number.
 * \param [out] val Pre-select value.
 *
 * \retval SHR_E_NONE No errors.
 * \retval !SHR_E_NONE Failure.
 */
extern int
bcmi_ltsw_flexctr_egr_presel_get(
    int unit,
    int *val);

#endif /* BCMI_LTSW_FLEXCTR_H */
