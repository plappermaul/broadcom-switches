/* $Id: ui_vx_shell.c,v 1.3 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
*/
/*
 * Basic_include_file.
 */

/*
 * General include file for reference design.
 */
#include <appl/diag/dpp/ref_sys.h>
/*
 * INCLUDE FILES:
 */
#ifdef SAND_LOW_LEVEL_SIMULATION
/* { */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
/*
 * Utilities include file.
 */
#include <appl/diag/dpp/utils_defx.h>
/*
 * User interface external include file.
 */
#include <appl/diag/dpp/ui_defx.h>
/*
 * User interface internal include file.
 */
#include <appl/dpp/UserInterface/ui_defi.h>
/*
 * Dune chips include file.
 */
#include <appl/diag/dpp/dune_chips.h>
/* } */
#else
/* { */
#include <vxWorks.h>
#include <netShow.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <taskLib.h>
#include <errnoLib.h>
#include <usrLib.h>
#include <tickLib.h>
#include <ioLib.h>
#include <iosLib.h>
#include <logLib.h>
#include <pingLib.h>
#include <pipeDrv.h>

#include <appl/diag/dpp/utils_defx.h>
#include <appl/diag/dpp/ui_defx.h>
#include <appl/diag/dpp/dune_chips.h>

#include <appl/dpp/UserInterface/ui_defi.h>

#include <appl/diag/dpp/utils_string.h>


/* } */
#endif

/*
 * BOOT function
 */
extern unsigned
  char
    disable_internal_watchdog(
      void
    );
extern int
  detach_irq(
    unsigned int irq_num,
    char         *err_msg
  );

/*****************************************************
*NAME
*  subject_vx_shell
*TYPE: PROC
*DATE: 02/APR/2002
*FUNCTION:
*  Process input line which has a 'vx_shell' subject.
*  Input line is assumed to have been checked and
*  found to be of right format.
*CALLING SEQUENCE:
*  subject_vx_shell(current_line,current_line_ptr)
*INPUT:
*  SOC_SAND_DIRECT:
*    CURRENT_LINE *current_line -
*      Pointer to prompt line to process.
*    CURRENT_LINE **current_line_ptr -
*      Pointer to prompt line to be displayed after
*      this procedure finishes execution. Caller
*      points this variable to the pointer to
*      the next line to display. If called function wishes
*      to set the next line to display, it replaces
*      the pointer to the next line to display.
*  SOC_SAND_INDIRECT:
*    None.
*OUTPUT:
*  SOC_SAND_DIRECT:
*    int -
*      If non zero then some error has occurred.
*  SOC_SAND_INDIRECT:
*    Processing results. See 'current_line_ptr'.
*REMARKS:
*  This procedure should be carried out under 'task_safe'
*  state (i.e., task can not be deleted while this
*  procedure is being carried out).
*SEE ALSO:
*****************************************************/
int
  subject_vx_shell(
    CURRENT_LINE *current_line,
    CURRENT_LINE **current_line_ptr
  )
{
  int
    ret ;
  PARAM_VAL
    *param_val ;
  char
    *flags_show,
    err_msg[80*4] ;
  ret = FALSE ;
  /*
   * Detailed explanation on route flags field (applies to arp as well)
   */
  flags_show =
        "The \'flags\' field represents a decimal value of the flags specified for a\r\n"
        "given route. The following is a list of currently available flag values:\r\n"
        "==============================================================================\r\n"
        "0x1     - route is usable (that is, \'up\') | 0x2     - destination is a gateway\r\n"
        "0x4     - host specific routing entry     | 0x8     - host or net unreachable\r\n"
        "0x10    - created dynamically             | 0x20    - modified dynamically\r\n"
        "          (by redirect)                   |           (by redirect)\r\n"
        "0x40    - message confirmed               | 0x80    - subnet mask present\r\n"
        "0x100   - generate new routes on use      | 0x200   - external daemon resolves\r\n"
        "                                          |           name\r\n"
        "0x400   - generated by ARP                | 0x800   - manually added (static)\r\n"
        "0x1000  - just discard packets            | 0x2000  - modified by\r\n"
        "          (during updates)                |           management protocol\r\n"
        "0x4000  - protocol specific routing flag  | 0x8000  - protocol specific\r\n"
        "                                          |           routing flag\r\n" ;
  if (current_line->num_param_names == 0)
  {
    /*
     * Enter if there are no parameters on the line (just 'vx_shell').
     * This should be protected against by the calling environment.
     */
    send_string_to_screen(
      "\r\n\n"
      "*** There are no parameters on the line (just \'vx_shell\'). SW error.\r\n",TRUE) ;
    send_string_to_screen(err_msg,TRUE) ;
    ret = TRUE ;
    goto suvs_exit ;
  }
  else
  {
    /*
     * Enter if there are parameters on the line (not just 'mem').
     */
    unsigned
      int
        match_index ;
    if (!search_param_val_pairs(
                  current_line,&match_index,PARAM_VXSHELL_REBOOT_ID,1))
    {
      D_REBOOT_TYPE
        reboot_type;
      if (!search_param_val_pairs(
                  current_line,&match_index,PARAM_VXSHELL_REBOOT_SOFT_ID,1))
      {
        reboot_type = D_SOFT_REBOOT;
      }
      else
      {
        reboot_type = D_HARD_REBOOT;
      }
      /*
       * Enter if 'reboot' is on the line.
       * Reset the bloody system...
       */
      send_string_to_screen("\r\n\n"
                            "System restarting, may take a few seconds...\n", TRUE);
      if (reboot_type == D_HARD_REBOOT)
      {
        send_string_to_screen("Hard reboot (through hardware watchdog).\n\r", TRUE);
      }
      taskDelay(1);
      d_reboot(reboot_type, BOOT_CLEAR) ;
      if (reboot_type == D_SOFT_REBOOT)
      {
        disable_internal_watchdog() ;
        detach_irq(0,err_msg) ;
      }
    }
    else if (!search_param_val_pairs(
                  current_line,&match_index,PARAM_VXSHELL_I_ID,1))
    {
      /*
       * Enter if 'ii' is on the line.
       * Show summary of all tasks.
       */
      i(0) ;
    }
    else if (!search_param_val_pairs(
                  current_line,&match_index,PARAM_VXSHELL_LOGOUT_ID,1))
    {
      /*
       * Enter if 'logout' is on the line.
       * Show info on all system known devices.
       */
      send_array_to_screen("\r\n",2) ;
      logout() ;
    }
    else if (!search_param_val_pairs(
                  current_line,&match_index,PARAM_VXSHELL_DEVS_ID,1))
    {
      /*
       * Enter if 'devs' is on the line.
       * Show info on all system known devices.
       */
      send_array_to_screen("\r\n",2) ;
      devs() ;
    }
    else if (!search_param_val_pairs(
                  current_line,&match_index,PARAM_VXSHELL_CHECK_STACK_SHOW_ID,1))
    {
      /*
       * Enter if 'check_stack_show' is on the line.
       * Show info on stacks all active tasks.
       */
      send_array_to_screen("\r\n",2) ;
      checkStack(0) ;
    }
    else if (!search_param_val_pairs(
                  current_line,&match_index,PARAM_VXSHELL_IOSFDSHOW_ID,1))
    {
      /*
       * Enter if 'ios_fd_show' is on the line.
       * Display a list of file descriptor names in the system.
       */
      send_array_to_screen("\r\n",2) ;
      iosFdShow() ;
    }
    else if (!search_param_val_pairs(
                  current_line,&match_index,PARAM_VXSHELL_ROUTE_SHOW_ID,1))
    {
      /*
       * Enter if 'route_show' is on the line.
       * Display IP network details.
       */
      send_array_to_screen("\r\n",2) ;
      routeShow() ;
      send_array_to_screen("\r\n",2) ;
      send_string_to_screen(flags_show,TRUE) ;
    }
    else if (!search_param_val_pairs(
                  current_line,&match_index,PARAM_VXSHELL_IF_SHOW_ID,1))
    {
      /*
       * Enter if 'if_show' is on the line.
       * Display attached network interfaces details.
       */
      send_array_to_screen("\r\n\n",3) ;
      ifShow(NULL) ;
    }
    else if (!search_param_val_pairs(
                  current_line,&match_index,PARAM_VXSHELL_I_NET_STAT_SHOW_ID,1))
    {
      /*
       * Enter if 'i_net_stat_show' is on the line.
       * Display all active connections for Internet protocol sockets.
       */
      send_array_to_screen("\r\n\n",3) ;
      inetstatShow() ;
    }
    else if (!search_param_val_pairs(
                  current_line,&match_index,PARAM_VXSHELL_PRINT_ERRNO_ID,1))
    {
      /*
       * Enter if 'print_errno' is on the line.
       * Display corresponding error description.
       */
      int
        err_number = 0 ;
      /*
       *  Get error identifier from the user.
       */
      if (!get_val_of(
              current_line,(int *)&match_index,PARAM_VXSHELL_PRINT_ERRNO_ID,1,
              &param_val,VAL_NUMERIC,err_msg))
      {
        err_number = param_val->value.ulong_value ;
      }
      send_array_to_screen("\r\n\n",3) ;
      if (err_number)
      {
        sal_sprintf(err_msg,
                "Status value \'%lu\' relates to definition: ",(unsigned long)err_number) ;
      }
      else
      {
        sal_sprintf(err_msg,
                "Status of current task: %lu",(unsigned long)err_number) ;
      }
      send_string_to_screen(err_msg,FALSE) ;
      printErrno(err_number) ;
    }
    else if (!search_param_val_pairs(
                  current_line,&match_index,PARAM_VXSHELL_ICMP_STAT_SHOW_ID,1))
    {
      /*
       * Enter if 'icmp_stat_show' is on the line.
       * Display all active connections for Internet protocol sockets.
       */
      send_array_to_screen("\r\n\n",3) ;
      icmpstatShow() ;
    }
    else if (!search_param_val_pairs(
                  current_line,&match_index,PARAM_VXSHELL_MEM_SHOW_ID,1))
    {
      /*
       * Enter if 'mem_show' is on the line.
       * Display allocated and freed memory statistics.
       */
      send_array_to_screen("\r\n\n",3) ;
      if (search_param_val_pairs(
              current_line,&match_index,PARAM_VXSHELL_SHOW_FREE_LIST_ID,1) == TRUE)
      {
        /*
         * Parameter 'show_free_list' is not on this line. Do not show free list.
         */
        memShow(0) ;
      }
      else
      {
        /*
         * Parameter 'show_free_list' is not on this line. Show free list.
         */
        memShow(1) ;
      }
    }
    else if (!search_param_val_pairs(
                  current_line,&match_index,PARAM_VXSHELL_ARP_SHOW_ID,1))
    {
      /*
       * Enter if 'arp_show' is on the line.
       * Display entries in the system ARP table.
       */
      send_array_to_screen("\r\n\n",3) ;
      arpShow() ;
      send_array_to_screen("\r\n",2) ;
      send_string_to_screen(flags_show,TRUE) ;
    }
    else if (!search_param_val_pairs(
                  current_line,&match_index,PARAM_VXSHELL_PING_ID,1))
    {
      char ip_address[MAX_IP_STRING+2]="localhost";
      int  num_packets=1;
      ulong_t options =0;
      char
        err_msg[80*2] ={0};
      PARAM_VAL
        *param_val ;
      STATUS status;

      send_array_to_screen("\r\n",2) ;

      if (!search_param_val_pairs(current_line,&match_index,PARAM_VXSHELL_HOST_IP_ID,1))
      {
        /*
         *  get the host id from the user.
         */
        if (!get_val_of(
                current_line,(int *)&match_index,PARAM_VXSHELL_HOST_IP_ID,1,
                &param_val,VAL_IP,err_msg))
        {
          sprint_ip(ip_address, param_val->value.ip_value, TRUE);
        }
      }
      if (!search_param_val_pairs(current_line,&match_index,PARAM_VXSHELL_PING_NOF_PKTS_ID,1))
      {
        /*
         *  get number of packet to ping.
         */
        if (!get_val_of(
                current_line,(int *)&match_index,PARAM_VXSHELL_PING_NOF_PKTS_ID,1,
                &param_val,VAL_NUMERIC,err_msg))
        {
          num_packets = param_val->value.ulong_value;
        }
      }
      if (!search_param_val_pairs(current_line,&match_index,PARAM_VXSHELL_PING_OPT_NOHOST_ID,1))
      {
        /*
         *  get no_host option.
         */
        options|=PING_OPT_NOHOST;
      }
      if (!search_param_val_pairs(current_line,&match_index,PARAM_VXSHELL_PING_OPT_DONTROUTE_ID,1))
      {
        /*
         *  get dont_route option.
         */
        options|=PING_OPT_DONTROUTE;
      }
      if (!search_param_val_pairs(current_line,&match_index,PARAM_VXSHELL_PING_OPT_SILENT_ID,1))
      {
        /*
         *  get silent option.
         */
        options|=PING_OPT_SILENT;
      }
      if (!search_param_val_pairs(current_line,&match_index,PARAM_VXSHELL_PING_OPT_DEBUG_ID,1))
      {
        /*
         *  get debug option.
         */
        options|=PING_OPT_DEBUG;
      }

      status = ping(ip_address, num_packets, options);
      sal_sprintf(err_msg, "ping returned with status=%d", status);
      send_string_to_screen(err_msg, TRUE);
    }
    else if (!search_param_val_pairs(
                  current_line,&match_index,PARAM_VXSHELL_TASK_ID,1))
    {
      /*
       * Enter if 'task' is on the line.
       * Get the task ID
       */
      int tid = -1;

      /*
       *  Get Task identifier from the user.
       */
      if (!get_val_of(
              current_line,(int *)&match_index,PARAM_VXSHELL_TASK_ID,1,
              &param_val,VAL_NUMERIC,err_msg))
      {
        tid = param_val->value.ulong_value ;
      }
      if ((tid < 0) || !is_ptr_in_blocks((char *)tid) || (OK != d_taskIdVerify(tid))) {
        send_string_to_screen(
        "\r\n"
        "*** Illegal task ID.\r\n",
        TRUE) ;
        ret = TRUE;
      }
      else if (!search_param_val_pairs(
               current_line,&match_index,PARAM_VXSHELL_TASK_OPT_DELETE_ID,1)) {
        if (OK != d_taskDelete(tid)) {
          send_string_to_screen("\r\nFailed to delete task.\r\n", TRUE);
          ret = TRUE;
        }
      }
      else if (!search_param_val_pairs(
               current_line,&match_index,PARAM_VXSHELL_TASK_OPT_RESTART_ID,1)) {
        if (OK != d_taskRestart(tid)) {
          send_string_to_screen("\r\nFailed to restart task.\r\n", TRUE);
          ret = TRUE;
        }
      }
      else if (!search_param_val_pairs(
               current_line,&match_index,PARAM_VXSHELL_TASK_OPT_RESUME_ID,1)) {
        if (OK != d_taskResume(tid)) {
          send_string_to_screen("\r\nFailed to resume task.\r\n", TRUE);
          ret = TRUE;
        }
      }
      else if (!search_param_val_pairs(
               current_line,&match_index,PARAM_VXSHELL_TASK_OPT_SUSPEND_ID,1)) {
        if (OK != d_taskSuspend(tid)) {
          send_string_to_screen("\r\nFailed to suspend task.\r\n", TRUE);
          ret = TRUE;
        }
      }
    } /* PARAM_VXSHELL_TASK_ID */
    else
    {
      /*
       * Enter if an unknown request.
       */
      send_string_to_screen(
        "\r\n"
        "*** Unknown VxWorks shell command.\r\n"
        "    Syntax or SW error...\r\n",
        TRUE) ;
      ret = TRUE ;
      goto suvs_exit ;
    }
  }
suvs_exit:
  return (ret) ;
}
