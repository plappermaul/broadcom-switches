/*
 * $Id: intr.c,v 1.8 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * Interrupt controller task.
 * This task emulates an interrupt handler and callout mechanism.
 */

#include <shared/bsl.h>

#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdio.h>

#ifdef VXWORKS
#include <vxWorks.h>
#include <sockLib.h>
#endif

#include <sal/appl/sal.h>
#include <sal/appl/io.h>
#include <sal/core/thread.h>
#include <sal/core/sync.h>
#include <sal/core/spl.h>
#include <soc/debug.h>	

#include "verinet.h"

int
intr_int_context(void)
{
    int i;

    for (i = 0; i < N_VERINET_DEVICES; i++) {
	if (sal_thread_self() == verinet[i].intrDispatchThread) {
	    return 1;
	}
    }

    return 0;
}

static void
intr_dispatch(void *v_void)
{
    verinet_t *v = (verinet_t *)v_void;
    int s;

    LOG_INFO(BSL_LS_SYS_VERINET,
             (BSL_META("intr_dispatch: running, sema=%p\n"),
              v->intrDispatchSema));

    while (!v->intrFinished) {
	if (sal_sem_take(v->intrDispatchSema, sal_sem_FOREVER)) {
	    cli_out("intr_dispatch: sal_sem_take failed\n");
	    continue;
	}

	/*
	 * In real hardware, interrupts aren't taken while at splhi.  We
	 * simulate this in our thread-based simulation simply by taking
	 * the semaphore for the duration of the call to the interrupt
	 * vector.  To be correct, sal_splhi() would actually have to
	 * suspend all other threads and spl() resume them.
	 */

        LOG_INFO(BSL_LS_SYS_VERINET,
                 (BSL_META("intr_dispatch: ISR=%p ISR_data=%p\n"),
                  v->ISR, v->ISR_data));

	if (v->ISR) {
	    s = sal_splhi();
	    (*v->ISR)(v->ISR_data);
	    sal_spl(s);
	}
    }

    LOG_INFO(BSL_LS_SYS_VERINET,
             (BSL_META("intr_dispatch: thread exiting ....\n")));
    sal_sem_destroy(v->intrDispatchSema);
    cli_out("INTC dispatcher shutdown.\n");
    v->intrDispatchThread = 0;
    sal_thread_exit(0);
}

/*
 * Process an interrupt request dispatched on the socket.
 */

void
intr_handler(void *v_void)
{
    verinet_t *v = (verinet_t *)v_void;
    uint32 cause;
    rpc_cmd_t cmd;
    int sockfd = v->intrFd;

    v->intrFinished = 0;

    /* Start off dispatch thread */

    v->intrDispatchSema =
	sal_sem_create("Interrupt-dispatch", sal_sem_BINARY, 0);

    if (!v->intrDispatchSema) {
	cli_out("intr_handler: failed to create dispatch sem\n");
	v->intrFinished = 1;
    } else {
	v->intrDispatchThread = sal_thread_create("Interrupt-dispatch",
						  SAL_THREAD_STKSZ, 100,
						  intr_dispatch, v);
	if (SAL_THREAD_ERROR == v->intrDispatchThread) {
	    cli_out("intr_handler: failed to create dispatch thread\n");
	    sal_sem_destroy(v->intrDispatchSema);
	    v->intrDispatchSema = 0;
	    v->intrFinished = 1;
	}
    }

    while (!v->intrFinished) {
        LOG_INFO(BSL_LS_SYS_VERINET,
                 (BSL_META("intr_handler: wait...\n")));

	if (wait_command(sockfd, &cmd) < 0) {
	    break;	/* Error message already printed */
	}

        LOG_INFO(BSL_LS_SYS_VERINET,
                 (BSL_META("intr_handler: request opcode 0x%x\n"),
                  cmd.opcode));

	switch(cmd.opcode) {

	case RPC_SEND_INTR:
	    /* Read cause */
	    cause = cmd.args[0];
	    make_rpc_intr_resp(&cmd,cause);
	    write_command(sockfd, &cmd);

	    if (v->intrSkipTest) {
                LOG_INFO(BSL_LS_SYS_VERINET,
                         (BSL_META("Test interrupt received (ignoring)\n")));
		v->intrSkipTest = 0;
		break;
	    }

	    /* Wake up dispatch thread */

            LOG_INFO(BSL_LS_SYS_VERINET,
                     (BSL_META("Wake dispatch\n")));

	    sal_sem_give(v->intrDispatchSema);
	    break;

	case RPC_DISCONNECT:
	    v->intrFinished = 1;
	    v->intrWorkerExit++;
	    cli_out("intr handler received disconnect\n");
	    sal_sem_give(v->intrDispatchSema);
	    break;

	default:
	    /* Unknown opcode */
	    break;
	}
    }
    cli_out("Interrupt service shutdown.\n");
    close(sockfd);

    /* If dispatch thread around - wait for him to exit */

    if (v->intrDispatchSema) {
	sal_sem_give(v->intrDispatchSema);
    }

    sal_thread_exit(0);
}

void
intr_set_vector(verinet_t *v, pli_isr_t isr, void *isr_data)
{
    /*
     * Set interrupt vector
     */

    LOG_INFO(BSL_LS_SYS_VERINET,
             (BSL_META("intr_set_vector: dev=%d isr=%p isr_data=%p\n"),
              (int)(v - verinet), isr, isr_data));

    v->ISR = isr;
    v->ISR_data = isr_data;
}

/*
 * start_isr_service: Sets up the socket and handles client requests
 * with a child thread per socket.
 */
void
intr_listener(void *v_void)
{
    verinet_t *v = (verinet_t *)v_void;
    int sockfd, newsockfd, one = 1;
    socklen_t clilen;
    struct sockaddr_in cli_addr, serv_addr;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
	perror("server: can't open stream socket");
	exit(1);
    }

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR,
		   (char *)&one, sizeof (one)) < 0) {
	perror("setsockopt");
    }

    /*
     * Setup server address...
     */
    memset((void *) &serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(0);	/* Pick any port */

    /*
     * Bind our local address
     */
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
	perror("dmac: unable to bind address");
	sal_thread_exit(0);
    }

    v->intPort = getsockport(sockfd);	/* Get port that was picked */

    /* listen for inbound homies ...*/
    listen(sockfd, 5);

    /* Notify intr_init that socket is listening */
    sal_sem_give(v->intrListening);

    cli_out("ISR dispatcher listening on port[%d]\n", v->intPort);

    while (!v->intrWorkerExit) {
	clilen = sizeof(cli_addr);

	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	if (newsockfd < 0 && errno == EINTR) {
	    continue;
	}

	if (newsockfd < 0) {
	    perror("server: accept error");
	} else {
	    v->intrSkipTest = 1;

	    v->intrFd = newsockfd;

	    v->intrHandlerThread = sal_thread_create("Interrupt-handler",
						     SAL_THREAD_STKSZ, 100,
						     intr_handler, v);

	    if (SAL_THREAD_ERROR == v->intrHandlerThread) {
		cli_out("start_isr_service: thread creation error\n");
	    } else{
                LOG_INFO(BSL_LS_SYS_VERINET,
                         (BSL_META("Interrupt request thread dispatched.\n")));
	    }
	}
    }

    LOG_INFO(BSL_LS_SYS_VERINET,
             (BSL_META("INTC listener shutdown.\n")));
    close(sockfd);
    sal_thread_exit(0);
}

int
intr_init(verinet_t *v)
{
    if (v->intrInited) {
	return 0;
    }

    v->intrListening = sal_sem_create("intr listener", sal_sem_BINARY, 0);

    /*
     * Create Interrupt task
     */
    cli_out("Starting Interrupt service...\n");

    v->intrThread = sal_thread_create("Interrupt-listener",
				      SAL_THREAD_STKSZ, 100,
				      intr_listener, v);

    if (SAL_THREAD_ERROR == v->intrThread) {
	cli_out("ERROR: could not create interrupt listener task!\n");
	return -2;
    }

    sal_sem_take(v->intrListening, sal_sem_FOREVER);   /* Wait for listen() */
    sal_sem_destroy(v->intrListening);

    v->intrInited = 1;

    return 0;
}
