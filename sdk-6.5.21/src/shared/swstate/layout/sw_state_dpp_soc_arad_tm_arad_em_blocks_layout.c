/* 
 * 
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 * 
 * DO NOT EDIT THIS FILE!
 * This file is auto-generated.
 * Edits to this file will be lost when it is regenerated.
*/
#include <sal/core/libc.h>
#include <shared/swstate/sw_state.h>
#include <shared/error.h>
#include <shared/swstate/sw_state_defs.h>
#include <shared/swstate/sw_state_utils.h>
#include <shared/bsl.h>
#include <shared/swstate/layout/sw_state_layout.h>
#include <shared/swstate/layout/sw_state_dpp_soc_arad_tm_arad_em_blocks_layout.h>

#ifdef _ERR_MSG_MODULE_NAME  
  #error "_ERR_MSG_MODULE_NAME redefined"  
#endif  
#define _ERR_MSG_MODULE_NAME BSL_LS_SHARED_SWSTATE 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
int sw_state_dpp_soc_arad_tm_arad_em_blocks_layout_node_create(int unit, int* root_node_id, uint32* next_free_node_id){  

   int younger_brother_node_id = 0; 
   int offset = 0; 
   CHIP_SIM_EM_BLOCK* dummy_struct; 
   SOC_INIT_FUNC_DEFS;
   /* alloc dummy struct */ 
   dummy_struct = sal_alloc(sizeof(CHIP_SIM_EM_BLOCK),"CHIP_SIM_EM_BLOCK"); 

   /* update current root node */  
   *root_node_id = *next_free_node_id; 
   /* add node */    
   _SOC_IF_ERR_EXIT(shr_sw_state_ds_layout_node_set(unit, 
                                   *root_node_id,                 /* node id var */      
                                   "arad_em_blocks",             /* name */         
                                   sizeof(CHIP_SIM_EM_BLOCK),    /* size of the element's type*/          
                                   1,                 /* nof pointers */ 
                                   "CHIP_SIM_EM_BLOCK",              /* param type */
                                   0,                 /* array size 0 */ 
                                   0));             /* array size 1 */ 
   /* update next free node id */ 
    SW_STATE_NODE_ID_CHECK(unit, ++(*next_free_node_id));  

   /* add node */ 
   _SOC_IF_ERR_EXIT(sw_state_dpp_soc_arad_tm_arad_em_blocks_read_result_address_layout_node_create(unit, &younger_brother_node_id, next_free_node_id));
   /* connect node (1st child) to its parent */ 
   _SOC_IF_ERR_EXIT(shr_sw_state_ds_layout_add_child(unit, *root_node_id, younger_brother_node_id)); 
   /* update offset */ 
   offset = ((uint8*) &(dummy_struct->read_result_address)) - (( uint8*) dummy_struct); 
   _SOC_IF_ERR_EXIT(shr_sw_state_ds_layout_update_offset(unit, younger_brother_node_id, offset)); 

   /* add node */ 
   _SOC_IF_ERR_EXIT(sw_state_dpp_soc_arad_tm_arad_em_blocks_offset_layout_node_create(unit, &younger_brother_node_id, next_free_node_id));
   /* connect node (1st child) to its parent */ 
   _SOC_IF_ERR_EXIT(shr_sw_state_ds_layout_add_child(unit, *root_node_id, younger_brother_node_id)); 
   /* update offset */ 
   offset = ((uint8*) &(dummy_struct->offset)) - (( uint8*) dummy_struct); 
   _SOC_IF_ERR_EXIT(shr_sw_state_ds_layout_update_offset(unit, younger_brother_node_id, offset)); 

   /* add node */ 
   _SOC_IF_ERR_EXIT(sw_state_dpp_soc_arad_tm_arad_em_blocks_table_size_layout_node_create(unit, &younger_brother_node_id, next_free_node_id));
   /* connect node (1st child) to its parent */ 
   _SOC_IF_ERR_EXIT(shr_sw_state_ds_layout_add_child(unit, *root_node_id, younger_brother_node_id)); 
   /* update offset */ 
   offset = ((uint8*) &(dummy_struct->table_size)) - (( uint8*) dummy_struct); 
   _SOC_IF_ERR_EXIT(shr_sw_state_ds_layout_update_offset(unit, younger_brother_node_id, offset)); 

   /* add node */ 
   _SOC_IF_ERR_EXIT(sw_state_dpp_soc_arad_tm_arad_em_blocks_key_size_layout_node_create(unit, &younger_brother_node_id, next_free_node_id));
   /* connect node (1st child) to its parent */ 
   _SOC_IF_ERR_EXIT(shr_sw_state_ds_layout_add_child(unit, *root_node_id, younger_brother_node_id)); 
   /* update offset */ 
   offset = ((uint8*) &(dummy_struct->key_size)) - (( uint8*) dummy_struct); 
   _SOC_IF_ERR_EXIT(shr_sw_state_ds_layout_update_offset(unit, younger_brother_node_id, offset)); 

   /* add node */ 
   _SOC_IF_ERR_EXIT(sw_state_dpp_soc_arad_tm_arad_em_blocks_data_nof_bytes_layout_node_create(unit, &younger_brother_node_id, next_free_node_id));
   /* connect node (1st child) to its parent */ 
   _SOC_IF_ERR_EXIT(shr_sw_state_ds_layout_add_child(unit, *root_node_id, younger_brother_node_id)); 
   /* update offset */ 
   offset = ((uint8*) &(dummy_struct->data_nof_bytes)) - (( uint8*) dummy_struct); 
   _SOC_IF_ERR_EXIT(shr_sw_state_ds_layout_update_offset(unit, younger_brother_node_id, offset)); 

   /* add node */ 
   _SOC_IF_ERR_EXIT(sw_state_dpp_soc_arad_tm_arad_em_blocks_start_address_layout_node_create(unit, &younger_brother_node_id, next_free_node_id));
   /* connect node (1st child) to its parent */ 
   _SOC_IF_ERR_EXIT(shr_sw_state_ds_layout_add_child(unit, *root_node_id, younger_brother_node_id)); 
   /* update offset */ 
   offset = ((uint8*) &(dummy_struct->start_address)) - (( uint8*) dummy_struct); 
   _SOC_IF_ERR_EXIT(shr_sw_state_ds_layout_update_offset(unit, younger_brother_node_id, offset)); 

   /* add node */ 
   _SOC_IF_ERR_EXIT(sw_state_dpp_soc_arad_tm_arad_em_blocks_end_address_layout_node_create(unit, &younger_brother_node_id, next_free_node_id));
   /* connect node (1st child) to its parent */ 
   _SOC_IF_ERR_EXIT(shr_sw_state_ds_layout_add_child(unit, *root_node_id, younger_brother_node_id)); 
   /* update offset */ 
   offset = ((uint8*) &(dummy_struct->end_address)) - (( uint8*) dummy_struct); 
   _SOC_IF_ERR_EXIT(shr_sw_state_ds_layout_update_offset(unit, younger_brother_node_id, offset)); 

   /* add node */ 
   _SOC_IF_ERR_EXIT(sw_state_dpp_soc_arad_tm_arad_em_blocks_multi_set_layout_node_create(unit, &younger_brother_node_id, next_free_node_id));
   /* connect node (1st child) to its parent */ 
   _SOC_IF_ERR_EXIT(shr_sw_state_ds_layout_add_child(unit, *root_node_id, younger_brother_node_id)); 
   /* update offset */ 
   offset = ((uint8*) &(dummy_struct->multi_set)) - (( uint8*) dummy_struct); 
   _SOC_IF_ERR_EXIT(shr_sw_state_ds_layout_update_offset(unit, younger_brother_node_id, offset)); 

   /* add node */ 
   _SOC_IF_ERR_EXIT(sw_state_dpp_soc_arad_tm_arad_em_blocks_base_layout_node_create(unit, &younger_brother_node_id, next_free_node_id));
   /* connect node (1st child) to its parent */ 
   _SOC_IF_ERR_EXIT(shr_sw_state_ds_layout_add_child(unit, *root_node_id, younger_brother_node_id)); 
   /* update offset */ 
   offset = ((uint8*) &(dummy_struct->base)) - (( uint8*) dummy_struct); 
   _SOC_IF_ERR_EXIT(shr_sw_state_ds_layout_update_offset(unit, younger_brother_node_id, offset)); 

   SOC_EXIT;
 exit:
   /* free dummy struct */ 
   sal_free(dummy_struct); 
   SOC_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
int sw_state_dpp_soc_arad_tm_arad_em_blocks_read_result_address_layout_node_create(int unit, int* root_node_id, uint32* next_free_node_id){  

   SOC_INIT_FUNC_DEFS;

   /* update current root node */  
   *root_node_id = *next_free_node_id; 
   /* add node */    
   _SOC_IF_ERR_EXIT(shr_sw_state_ds_layout_node_set(unit, 
                                   *root_node_id,                 /* node id var */      
                                   "read_result_address",             /* name */         
                                   sizeof(UINT32),    /* size of the element's type*/          
                                   0,                 /* nof pointers */ 
                                   "UINT32",              /* param type */
                                   0,                 /* array size 0 */ 
                                   0));             /* array size 1 */ 
   /* update next free node id */ 
    SW_STATE_NODE_ID_CHECK(unit, ++(*next_free_node_id));  

   SOC_EXIT;
 exit:
   SOC_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
int sw_state_dpp_soc_arad_tm_arad_em_blocks_offset_layout_node_create(int unit, int* root_node_id, uint32* next_free_node_id){  

   SOC_INIT_FUNC_DEFS;

   /* update current root node */  
   *root_node_id = *next_free_node_id; 
   /* add node */    
   _SOC_IF_ERR_EXIT(shr_sw_state_ds_layout_node_set(unit, 
                                   *root_node_id,                 /* node id var */      
                                   "offset",             /* name */         
                                   sizeof(UINT32),    /* size of the element's type*/          
                                   0,                 /* nof pointers */ 
                                   "UINT32",              /* param type */
                                   0,                 /* array size 0 */ 
                                   0));             /* array size 1 */ 
   /* update next free node id */ 
    SW_STATE_NODE_ID_CHECK(unit, ++(*next_free_node_id));  

   SOC_EXIT;
 exit:
   SOC_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
int sw_state_dpp_soc_arad_tm_arad_em_blocks_table_size_layout_node_create(int unit, int* root_node_id, uint32* next_free_node_id){  

   SOC_INIT_FUNC_DEFS;

   /* update current root node */  
   *root_node_id = *next_free_node_id; 
   /* add node */    
   _SOC_IF_ERR_EXIT(shr_sw_state_ds_layout_node_set(unit, 
                                   *root_node_id,                 /* node id var */      
                                   "table_size",             /* name */         
                                   sizeof(UINT32),    /* size of the element's type*/          
                                   0,                 /* nof pointers */ 
                                   "UINT32",              /* param type */
                                   0,                 /* array size 0 */ 
                                   0));             /* array size 1 */ 
   /* update next free node id */ 
    SW_STATE_NODE_ID_CHECK(unit, ++(*next_free_node_id));  

   SOC_EXIT;
 exit:
   SOC_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
int sw_state_dpp_soc_arad_tm_arad_em_blocks_key_size_layout_node_create(int unit, int* root_node_id, uint32* next_free_node_id){  

   SOC_INIT_FUNC_DEFS;

   /* update current root node */  
   *root_node_id = *next_free_node_id; 
   /* add node */    
   _SOC_IF_ERR_EXIT(shr_sw_state_ds_layout_node_set(unit, 
                                   *root_node_id,                 /* node id var */      
                                   "key_size",             /* name */         
                                   sizeof(UINT32),    /* size of the element's type*/          
                                   0,                 /* nof pointers */ 
                                   "UINT32",              /* param type */
                                   0,                 /* array size 0 */ 
                                   0));             /* array size 1 */ 
   /* update next free node id */ 
    SW_STATE_NODE_ID_CHECK(unit, ++(*next_free_node_id));  

   SOC_EXIT;
 exit:
   SOC_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
int sw_state_dpp_soc_arad_tm_arad_em_blocks_data_nof_bytes_layout_node_create(int unit, int* root_node_id, uint32* next_free_node_id){  

   SOC_INIT_FUNC_DEFS;

   /* update current root node */  
   *root_node_id = *next_free_node_id; 
   /* add node */    
   _SOC_IF_ERR_EXIT(shr_sw_state_ds_layout_node_set(unit, 
                                   *root_node_id,                 /* node id var */      
                                   "data_nof_bytes",             /* name */         
                                   sizeof(UINT32),    /* size of the element's type*/          
                                   0,                 /* nof pointers */ 
                                   "UINT32",              /* param type */
                                   0,                 /* array size 0 */ 
                                   0));             /* array size 1 */ 
   /* update next free node id */ 
    SW_STATE_NODE_ID_CHECK(unit, ++(*next_free_node_id));  

   SOC_EXIT;
 exit:
   SOC_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
int sw_state_dpp_soc_arad_tm_arad_em_blocks_start_address_layout_node_create(int unit, int* root_node_id, uint32* next_free_node_id){  

   SOC_INIT_FUNC_DEFS;

   /* update current root node */  
   *root_node_id = *next_free_node_id; 
   /* add node */    
   _SOC_IF_ERR_EXIT(shr_sw_state_ds_layout_node_set(unit, 
                                   *root_node_id,                 /* node id var */      
                                   "start_address",             /* name */         
                                   sizeof(UINT32),    /* size of the element's type*/          
                                   0,                 /* nof pointers */ 
                                   "UINT32",              /* param type */
                                   0,                 /* array size 0 */ 
                                   0));             /* array size 1 */ 
   /* update next free node id */ 
    SW_STATE_NODE_ID_CHECK(unit, ++(*next_free_node_id));  

   SOC_EXIT;
 exit:
   SOC_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
int sw_state_dpp_soc_arad_tm_arad_em_blocks_end_address_layout_node_create(int unit, int* root_node_id, uint32* next_free_node_id){  

   SOC_INIT_FUNC_DEFS;

   /* update current root node */  
   *root_node_id = *next_free_node_id; 
   /* add node */    
   _SOC_IF_ERR_EXIT(shr_sw_state_ds_layout_node_set(unit, 
                                   *root_node_id,                 /* node id var */      
                                   "end_address",             /* name */         
                                   sizeof(UINT32),    /* size of the element's type*/          
                                   0,                 /* nof pointers */ 
                                   "UINT32",              /* param type */
                                   0,                 /* array size 0 */ 
                                   0));             /* array size 1 */ 
   /* update next free node id */ 
    SW_STATE_NODE_ID_CHECK(unit, ++(*next_free_node_id));  

   SOC_EXIT;
 exit:
   SOC_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
int sw_state_dpp_soc_arad_tm_arad_em_blocks_multi_set_layout_node_create(int unit, int* root_node_id, uint32* next_free_node_id){  

   SOC_INIT_FUNC_DEFS;

   /* update current root node */  
   *root_node_id = *next_free_node_id; 
   /* add node */    
   _SOC_IF_ERR_EXIT(shr_sw_state_ds_layout_node_set(unit, 
                                   *root_node_id,                 /* node id var */      
                                   "multi_set",             /* name */         
                                   sizeof(SOC_SAND_MULTI_SET_PTR),    /* size of the element's type*/          
                                   0,                 /* nof pointers */ 
                                   "SOC_SAND_MULTI_SET_PTR",              /* param type */
                                   0,                 /* array size 0 */ 
                                   0));             /* array size 1 */ 
   /* update next free node id */ 
    SW_STATE_NODE_ID_CHECK(unit, ++(*next_free_node_id));  

   SOC_EXIT;
 exit:
   SOC_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
#ifdef BCM_PETRA_SUPPORT
#if defined(BCM_PETRA_SUPPORT)
int sw_state_dpp_soc_arad_tm_arad_em_blocks_base_layout_node_create(int unit, int* root_node_id, uint32* next_free_node_id){  

   SOC_INIT_FUNC_DEFS;

   /* update current root node */  
   *root_node_id = *next_free_node_id; 
   /* add node */    
   _SOC_IF_ERR_EXIT(shr_sw_state_ds_layout_node_set(unit, 
                                   *root_node_id,                 /* node id var */      
                                   "base",             /* name */         
                                   sizeof(uint8),    /* size of the element's type*/          
                                   1,                 /* nof pointers */ 
                                   "uint8",              /* param type */
                                   0,                 /* array size 0 */ 
                                   0));             /* array size 1 */ 
   /* update next free node id */ 
    SW_STATE_NODE_ID_CHECK(unit, ++(*next_free_node_id));  

   SOC_EXIT;
 exit:
   SOC_FUNC_RETURN;
}

#endif /* defined(BCM_PETRA_SUPPORT)*/ 
#endif /* BCM_PETRA_SUPPORT*/ 
