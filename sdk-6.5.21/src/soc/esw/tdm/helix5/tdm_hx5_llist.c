/*
 * 
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2020 Broadcom Inc. All rights reserved.
 * $All Rights Reserved.$
 *
 * TDM chip linked list methods
 */
#ifdef _TDM_STANDALONE
	#include <tdm_top.h>
#else
	#include <soc/tdm/core/tdm_top.h>
#endif


/**
@name: tdm_hx5_ll_print
@param:

Print elements from TDM linked list
 */
void
tdm_hx5_ll_print(struct hx5_ll_node *llist)
{
    int i=0;
    struct hx5_ll_node *list = llist;

    if (list!=NULL) {
        list=list->next;
        TDM_PRINT0("\n");
        TDM_PRINT0("\tTDM linked list content: \n\t\t");
        while(list!=NULL) {
            TDM_PRINT1("[%03d]",list->port);
            list=list->next;
            if ((++i)%10==0) {
                TDM_PRINT0("->\n\t\t");
            } else {
                TDM_PRINT0("->");
            }
        }
        TDM_PRINT0("[*]\n\n\n");
    } else {
        TDM_PRINT0("\n\t\tUnable to parse TDM linked list for print\n\n");
    }
}


/**
@name: tdm_hx5_ll_deref
@param:

Dereference entire TDM linked list into unlinked array
 */
void
tdm_hx5_ll_deref(struct hx5_ll_node *llist, int *tdm[HX5_LR_VBS_LEN], int lim)
{
    int i=0;
    struct hx5_ll_node *list = llist;

    if (list!=NULL) {
        list=list->next;
        while ( (list!=NULL) && (i<lim) ) {
            (*tdm)[i++]=list->port;
            list=list->next;
        }
    } else {
        TDM_PRINT0("\t\tUnable to parse TDM linked list for deref\n\n");
    }
}


/**
@name: tdm_hx5_ll_append
@param:

Append an element to TDM linked list
 */
void
tdm_hx5_ll_append(struct hx5_ll_node *llist, unsigned short port_append, int *pointer)
{
    struct hx5_ll_node *mbox,
                    *tail = ( (struct hx5_ll_node *) llist );

    if (llist) {
        while (tail->next!=NULL) {
            tail=tail->next;
        }
    }

    if (tail) {
        mbox = ( (struct hx5_ll_node *) TDM_ALLOC(sizeof(struct hx5_ll_node),"hx5_ll_node") );
        mbox->port=port_append;
        mbox->next=NULL;
        tail->next=mbox;
        tail=mbox;
        tail->next=NULL;
    } else {
        /* llist=mbox; */
    }

    (*pointer)++;
}


/**
@name: tdm_hx5_ll_insert
@param:

Insert an element at index to TDM linked list
 */
void
tdm_hx5_ll_insert(struct hx5_ll_node *llist, unsigned short port_insert, int idx)
{
    int i;
    struct hx5_ll_node *mbox, *right=llist, *left=NULL;

    /* Conditions: 1) list header cannot be NULL; 2) valid element index; */
    if ( (llist != NULL)                        &&
         (idx>=0 && idx<=tdm_hx5_ll_len(llist)) ){
        mbox = (struct hx5_ll_node *) TDM_ALLOC(sizeof(struct hx5_ll_node),"hx5_ll_node");
        mbox->port=port_insert;
        mbox->next=NULL;
        
        /* List header is always empty */
        left = right;
        right= right->next;		
        
        /* Insert the element at designated index */
        for (i=0; i<idx; i++){
            left = right;
            right= right->next;
        }
        left->next=mbox;
        left=mbox;
        left->next=right;
    }
}


/**
@name: tdm_hx5_ll_delete
@param:

Delete an element at index from TDM linked list
 */
int
tdm_hx5_ll_delete(struct hx5_ll_node *llist, int idx)
{
    int i;
    struct hx5_ll_node *mbox=llist, *last=NULL;

    /* Conditions: 1) non-empty list; 2) index range within [0,1,2,...,(len-1)] ; */
    if ((tdm_hx5_ll_len(llist)>0)             && 
        (idx>=0 && idx<tdm_hx5_ll_len(llist)) ){
        /* List header is always empty */
        last=mbox;
        mbox=mbox->next;
        /* Delete the element at designated index */
        for (i=0; i<idx; i++) {
            last=mbox;
            mbox=mbox->next;
        }
        last->next=mbox->next;
        TDM_FREE(mbox);
        
        return PASS;
    }

    return FAIL;
}


/**
@name: tdm_hx5_ll_get
@param:

Return content of hx5_ll_node at index
 */
int
tdm_hx5_ll_get(struct hx5_ll_node *llist, int idx)
{
    int i;
    struct hx5_ll_node *list = llist;

    if (list!=NULL) {
        list=list->next;
        for (i=0; ((i<idx) && (list!=NULL)); i++) {
            list=list->next;
        }
    }

    if (list!=NULL) {
        return (list->port);
    } else {
        return HX5_NUM_EXT_PORTS;
    }
}


/**
@name: tdm_hx5_ll_len
@param:

Return current length of linked list
 */
int
tdm_hx5_ll_len(struct hx5_ll_node *llist)
{
    int len=0;
    struct hx5_ll_node *list = llist;

    if (list!=NULL) {
        list=list->next;
        while ( (list!=NULL) ) {
            len++;
            list=list->next;
        }
    }

    return len;
}


/**
@name: tdm_hx5_ll_strip
@param:

Strip all of one tokenized value from TDM linked list and updates pool value for that token
 */
void
tdm_hx5_ll_strip(struct hx5_ll_node *llist, int *pool, int token)
{
    int i;
    struct hx5_ll_node *list = llist;

    if ( list!=NULL ) {
        for (i=0; i<tdm_hx5_ll_len(list); i++) {
            if ( tdm_hx5_ll_get(list,i)==token ) {
                tdm_hx5_ll_delete(list,i);
                i--; 
                (*pool)++;
            }
        }
    }
}


/**
@name: tdm_hx5_ll_count
@param:

Return count of tokenized port identity within TDM linked list
 */
int
tdm_hx5_ll_count(struct hx5_ll_node *llist, int token)
{
    int i, count=0;
    struct hx5_ll_node *list = llist;

    if (list!=NULL) {
        list=list->next;
        for (i=0; i<tdm_hx5_ll_len(list); i++) {
            HX5_TOKEN_CHECK(token) {
                if ( (tdm_hx5_ll_get(list,i)>0) && (tdm_hx5_ll_get(list,i)<129) ) {
                    count++;
                }				
            } else {
                if (tdm_hx5_ll_get(list,i)==token) {
                    count++;
                }
            }
        }
    }

    return count;
}


/**
@name: tdm_hx5_ll_weave
@param:

Based on pool, interspace pooled tokenized ll_nodes to space PGW TDM ports
 */
void
tdm_hx5_ll_weave(struct hx5_ll_node *llist, int token, tdm_mod_t *_tdm)
{
    int i, pool=0, skew=0, slices=0, split, count, set=BOOL_FALSE, div, timeout, original_len;
    struct hx5_ll_node *list = llist;

    if (list!=NULL && tdm_hx5_ll_len(list)>0) {
        original_len=tdm_hx5_ll_len(list);
        tdm_hx5_ll_strip(list,&pool,token);
        div=pool;
        if ( (pool>0) && ( tdm_hx5_ll_get(list,(tdm_hx5_ll_len(list)-1))==tdm_hx5_ll_get(list,0) ) ) {
            tdm_hx5_ll_insert(list,token,(tdm_hx5_ll_len(list))); set=BOOL_TRUE;
            pool--; slices++;
        }
        for (i=(tdm_hx5_ll_len(list)-1); i>0; i--) {			
            if ( (pool>0) && ( tdm_hx5_cmn_get_port_pm(tdm_hx5_ll_get(list,i),_tdm) == tdm_hx5_cmn_get_port_pm(tdm_hx5_ll_get(list,(i-1)),_tdm) ) ) {
                tdm_hx5_ll_insert(list,token,i); set=BOOL_TRUE;
                slices++;
                if ((--pool)<=0) {
                    break;
                }
            }
        }
        split=(pool>0)?(pool):(1);
        timeout=32000;
        while (pool>0 && (--timeout)>0) {
            count=0;
            if (set) {
                for (i=(1+skew); i<tdm_hx5_ll_len(list); i+=(((slices/split)>0)?(slices/split):(1))) {
                    if (tdm_hx5_ll_get(list,i)==token) {
                        if (++count==(slices/split)) {
                            tdm_hx5_ll_insert(list,token,i);
                            i++; count=0;
                            if ((--pool)<=0) {
                                break;
                            }
                        }
                    }
                }
                skew++;
            } else {
                for (i=((original_len/div)-1); i<original_len; i+=(original_len/div)) {
                    tdm_hx5_ll_insert(list,token,i);
                    if ((--pool)<=0) {
                        break;
                    }
                }
            }
        }
        if ( tdm_hx5_ll_len(list) < original_len ) {
            tdm_hx5_ll_insert(list,token,tdm_hx5_ll_len(list));
        }
        timeout=32000;
        while ( (tdm_hx5_ll_len(list) < original_len) && ((--timeout)>0) ) {
            for (i=(tdm_hx5_ll_len(list)-2); i>0; i--) {
                if ( (tdm_hx5_ll_get(list,(i-1))!=token) && (tdm_hx5_ll_get(list,(i+1))!=token) && (tdm_hx5_ll_get(list,i)!=token) ) {
                    tdm_hx5_ll_insert(list,token,i);
                    break;
                }
            }
        }
        timeout=32000;
        while ( (tdm_hx5_ll_len(list) < original_len) && ((--timeout)>0) ) {
            for (i=1; i<(tdm_hx5_ll_len(list)-1); i++) {
                if ( (tdm_hx5_ll_get(list,(i-1))!=token) && (tdm_hx5_ll_get(list,i)!=token) ) {
                    tdm_hx5_ll_insert(list,token,i);
                    break;
                }
            }
        }
        while ( tdm_hx5_ll_len(list) < original_len ) {
            tdm_hx5_ll_insert(list,token,tdm_hx5_ll_len(list));
        }
    }
}


/**
@name: tdm_hx5_ll_retrace
@param:

Repoint linked list ll_nodes if they violate PGW TDM min spacing
 */
void
tdm_hx5_ll_retrace(struct hx5_ll_node *llist, tdm_mod_t *_tdm)
{
    int i, j, port, done=BOOL_FALSE, timeout=32000;
    struct hx5_ll_node *list = llist;

    while((--timeout)>0) {
        for (i=1; i<tdm_hx5_ll_len(list); i++) {
            done=BOOL_FALSE;
            if (tdm_hx5_ll_get(list,i)!=HX5_OVSB_TOKEN && tdm_hx5_ll_get(list,(i-1))!=HX5_OVSB_TOKEN) {
                if ( tdm_hx5_cmn_get_port_pm(tdm_hx5_ll_get(list,i),_tdm) == tdm_hx5_cmn_get_port_pm(tdm_hx5_ll_get(list,(i-1)),_tdm) ) {
                    port=tdm_hx5_ll_get(list,i);
                    for (j=1; j<tdm_hx5_ll_len(list); j++) {
                        if ( ( tdm_hx5_cmn_get_port_pm(port,_tdm) != tdm_hx5_cmn_get_port_pm(tdm_hx5_ll_get(list, j   ),_tdm) ) &&
                             ( tdm_hx5_cmn_get_port_pm(port,_tdm) != tdm_hx5_cmn_get_port_pm(tdm_hx5_ll_get(list,(j-1)),_tdm) ) ){
                            if(i<j){
                                tdm_hx5_ll_insert(list,port,j);
                                tdm_hx5_ll_delete(list,i);
                            }
                            else{
                                tdm_hx5_ll_delete(list,i);
                                tdm_hx5_ll_insert(list,port,j);
                            }
                            done=BOOL_TRUE;
                            break;
                        }
                    }
                }
            }
            if (done) {
                break;
            }
        }
        if (!done) {
            break;
        }
    }
}


/**
@name: tdm_hx5_ll_free
@param:

Free memory space allocated to linked-list
 */
int
tdm_hx5_ll_free(struct hx5_ll_node *llist)
{
    struct hx5_ll_node *temp, *list = llist;
    if(list!=NULL){
        list = list->next;
        while(list!=NULL ){
            temp = list;
            list = list->next;
            TDM_FREE(temp);
        }
    }
    llist->next = NULL;

    return PASS;
}
