/*
 * $Id: cint_logger_cmd.c
 *
 * This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
 * 
 * Copyright 2007-2022 Broadcom Inc. All rights reserved.
 *
 * File:        cint_logger_cmd.c
 * Purpose:     CINT logger Commands.
 *
 */

#if CINT_CONFIG_INCLUDE_CINT_LOGGER == 1
/* { */

#include <sal/limits.h>
#include <shared/bsl.h>
#include <shared/bslnames.h>
#include <appl/diag/parse.h>
#include <appl/diag/cmdlist.h>
#include <sal/appl/sal.h>
#include <appl/diag/parse.h>
#include <appl/diag/cmdlist.h>

#include "cint_config.h"
#include "cint_porting.h"
#include "cint_variables.h"
#include "cint_internal.h"
#include "cint_logger.h"
#include "cint_logger_circular_buffer.h"

extern cmd_result_t cint_logger_test_api_using_cint(const char *api, int donotrun, int unit, int rv);

/*
 * Assume 100 characters will be sufficient for function names. Have a more
 * generous limit on file names to allow for path specifications, 1024.
 */
#define CINT_LOGGER_CMD_FUNC_MAXLEN     100
#define CINT_LOGGER_CMD_FPATH_MAXLEN   1024

/***********************************************************************************/
/*                                                                                 */
/* Handler for the LOGGER shell command                                            */
/*                                                                                 */
/***********************************************************************************/

/*
 * Help text for top level LOGGER shell command. Refered from cmdlist.c
 */
char cint_logger_usage[] = "\
LOGGER                                            \n\
    ConFiG             - all configuration change \n\
    SHow               - show logged entries      \n\
    DeCoDe             - similar to SHow; source  \n\
                         entries from a raw file  \n\
    CircularBufferTEST - tests for the circular   \n\
                         buffer infrastructure    \n\
    TEST               - automated tests for APIs \n\
                                                  \n\
When invoked without any options, equivalent to:  \n\
        \"LOGGER ConFiG\" followed by             \n\
        \"LOGGER SHow\"                           \n\
";

/*
 * Help text for "LOGGER ConFiG" command
 */
static char cint_logger_cfg_usage[] = "\
LOGGER ConFiG                                                  \n\
    ENable         - 1=enabled,0=disabled                      \n\
    Mode           - 1=log - can use user-defined CINT function\n\
                     2=cache - log entries in circular buffer  \n\
    FunctioN       - name of the filter function               \n\
    File           - file name                                 \n\
    FileOPTYPE     - output type for File                      \n\
                     0x0 - None/Disabled                       \n\
                     0x1 - Raw Output                          \n\
                     0x2 - CINT-like Output                    \n\
    RAWFile        - file name for raw inline outputs          \n\
    LOGFunctioN    - filter function when logging to File with \n\
                     FileOPTYPE set to CINT-link (0x2)         \n\
    REPLAYFunctioN - filter function when replaying            \n\
                     previously generated logs                 \n\
    BaseIndent     - number of space in one indent level       \n\
";

static char cint_logger_cfg_help[] = "\
This command is used for online configuration of the logger.        \n\
Logger reads the api_logger* SOC properties only the very first     \n\
time it is invoked, and then never again. This means that SOC       \n\
properties cannot be used to modify the logger behavior even        \n\
after a Init/Deinit cycle or tr 141. This command is the only way   \n\
to modify logger after initial configuration using  SOC properties. \n\
";

/*
 * Usage and help text for "LOGGER SHow" command
 */
static char cint_logger_show_usage[] = "\
LOGGER SHow                                                                   \n\
    FirstN      - first N entries of circular buffer                          \n\
                  If NFreeze is Zero, starting from the current head          \n\
    LastN       - last N entries of circular buffer                           \n\
    SkipN       - skip N entries before showing FirstN or LastN entries       \n\
    ForMaTter   - invoke user supplied CINT function for each entry           \n\
    FILE        - write output to supplied file name                          \n\
    FileOPTionS - 0=truncate, 1=append                                        \n\
";

static char cint_logger_show_help[] = "\
Show entries from cirular buffer in CINT-like output. Entries may \n\
be passed to a user supplied filter/formatter function, which     \n\
takes no arguments and returns a boolean, and automagically       \n\
receives  the call context and its inputs as named variables in   \n\
its global namespace. An entry is filtered out if function        \n\
returns TRUE, else it is logged in CINT-like output. All          \n\
generated Output, including output generated by the function if   \n\
any may be written to FILE or logged to console by default.       \n\
";

/*
 * Usage and help text for "LOGGER DeCoDe" command
 */
static char cint_logger_decode_usage[] = "\
LOGGER DeCoDe                                                                 \n\
    InputFILE   - read raw logs from tis file name                            \n\
    FirstN      - first N entries of circular buffer                          \n\
    SkipN       - skip N entries before showing FirstN or LastN entries       \n\
    ForMaTter   - invoke user supplied CINT function for each entry           \n\
    FILE        - write output to supplied file name                          \n\
    FileOPTionS - 0=truncate, 1=append                                        \n\
";

static char cint_logger_decode_help[] = "\
Command to decode a raw file nd inspect its entries. Has the same  \n\
behavior and takes the same options as the show command, except    \n\
that the entries are taken here from a previously generated raw    \n\
file.                                                              \n\
";

/*
 * Usage and help text for "LOGGER CircularBufferTEST" command. This
 * command is used for testing/regressing the circular buffer infrastructure.
 */
static char cint_logger_cbtest_usage[] = "\
LOGGER CircularBufferTEST                                         \n\
    TotalSZ     - total size of buffer                            \n\
    NFreeze     - first N entries which are not wrapped over      \n\
    Iterations  - number of requests/entries to allocate          \n\
    Alloc       - basic allocation unit. Test routine randomly    \n\
                  allocates various multiples and fractions of    \n\
                  this basic allocation size.                     \n\
    PrintEvery  - generate summary log every so many requests     \n\
    ConsumerThreadPrio - priority for the consumer thread.        \n\
                NOTE: This is ignored by SAL.                     \n\
    SeedRANDom - seed for the random number generator             \n\
";

static char cint_logger_cbtest_help[] = "\
Test for the circular buffer infrastructure. It allocates a      \n\
circular buffer of specified size and generates specified number \n\
of buffer requests from it with random sizes. Returns CMD_OK for \n\
pass and CMD_FAIL for failed test.                               \n\
";

/*
 * Usage and help text for "LOGGER Test" command
 */
static char cint_logger_test_usage[] = "\
LOGGER Test                                                       \n\
    API         - BCM API to test                                 \n\
    DoNotRun    - test script is not executed, only sourced       \n\
    Unit        - Unit# to be passed to the API                   \n\
    Rv          - value to be returned by the API                 \n\
";

static char cint_logger_test_help[] = "\
Test for the API logging infrastructure. It auto-generates a CINT \n\
script from a template, which invokes the supplied API with       \n\
randomized inputs and checks whether those random values were     \n\
correctly logged. This test command can be invoked to test        \n\
correctness of logging of any API. See cint_logger*tests*.[cht]   \n\
";

/*
 * Utility function to select an appropriate value for an option being
 * configured by the user. Setting the new value to empty string, or the case
 * insensitive string "null" has the effect of clearing it. If the provided
 * value is neither of those and different from the old value, the strdup of
 * the new value is returned. Else, the old value is returned.
 *
 * Parameters
 *   [in] new
 *     new string provided by the user for an option
 *   [in] old
 *     existing value of the option a user is trying to configure
 *   [out] res
 *     Returns pointer to the selected value in this parameter
 *
 */
static void
cint_logger_cmd_select_string_value (char *new,
                                     char *old,
                                     char **res,
                                     int maxlen)
{

    if (!new) {
        *res = old;
        return;
    }

    if (!CINT_STRCMP(new, "") ||
        !CINT_STRNCASECMP(new, "NULL", CINT_LOGGER_CMD_FUNC_MAXLEN)) {

        *res = NULL;
    } else if (!old || CINT_STRNCMP(new, old, maxlen)) {

        *res = CINT_STRDUP(new);
    } else {

        *res = old;
    }
}

static cmd_result_t
cint_logger_cmd_cfg (int unit, args_t *a)
{
    cint_logger_global_cfg_data_t new_cfg;
    parse_table_t pt;
    cint_parameter_desc_t cfg_pds = { "cint_logger_global_cfg_data_t", "new_cfg", 0, 0 };
    char *func = NULL, *logfile = NULL, *logfunc = NULL, *replayfunc = NULL, *raw_logfile = NULL;
    cint_variable_t *vcint_logger_cfg = Cint_logger_cfg.cint_var;
    cmd_result_t ret = CMD_OK;

    if (!ARG_CNT(a)) {
        cli_out("\nCurrent configuration:\n\n");
        cint_variable_print(vcint_logger_cfg, 4, NULL);
        return CMD_OK;
    }

    new_cfg = Cint_logger_cfg;

    parse_table_init(unit, &pt);
    parse_table_add(&pt, "Enable"         , PQ_DFL | PQ_INT , NULL                            , &new_cfg.enabled        , NULL);
    parse_table_add(&pt, "Mode"           , PQ_DFL | PQ_INT , NULL                            , &new_cfg.mode           , NULL);
    parse_table_add(&pt, "FunctioN"       , PQ_STRING       , new_cfg.cint_filter_fn          , &func                   , NULL);
    parse_table_add(&pt, "File"           , PQ_STRING       , new_cfg.logfile                 , &logfile                , NULL);
    parse_table_add(&pt, "FileOPTYPE"     , PQ_DFL | PQ_INT , NULL                            , &new_cfg.log_to_file    , NULL);
    parse_table_add(&pt, "RAWFile"        , PQ_STRING       , new_cfg.raw_logfile             , &raw_logfile            , NULL);
    parse_table_add(&pt, "LOGFunctioN"    , PQ_STRING       , new_cfg.cint_logfile_filter_fn  , &logfunc                , NULL);
    parse_table_add(&pt, "REPLAYFunctioN" , PQ_STRING       , new_cfg.cint_replay_filter_fn   , &replayfunc             , NULL);
    parse_table_add(&pt, "BaseIndent"     , PQ_DFL | PQ_INT , NULL                            , &new_cfg.base_indent    , NULL);

    if (parse_arg_eq(a, &pt) <= 0) {
        cli_out("Error: invalid option \"%s\"\n", ARG_CUR(a));
        parse_arg_eq_done(&pt);
        return CMD_USAGE;
    }

    parse_eq_format(&pt);

    /*
     * validate all the options before applying them
     */
    if ((new_cfg.enabled != 0) && (new_cfg.enabled != 1)) {

        cli_out("Error: Enable %d is invalid\n", new_cfg.enabled);
        return CMD_USAGE;
    }

    if ((new_cfg.mode != cintLoggerModeLog) &&
        (new_cfg.mode != cintLoggerModeReplay) &&
        (new_cfg.mode != cintLoggerModeCache)) {

        cli_out("Error: Mode %d is invalid\n", new_cfg.mode);
        return CMD_USAGE;
    }

    if ((new_cfg.log_to_file != CINT_LOGGER_FILE_OP_NONE) &&
        (new_cfg.log_to_file != CINT_LOGGER_FILE_OP_CINT) &&
        (new_cfg.log_to_file != CINT_LOGGER_FILE_OP_RAW)) {

        cli_out("Error: File OPType %d is invalid\n", new_cfg.log_to_file);
        return CMD_USAGE;
    }

    /*
     * Curate the string type arguments passed by the user.
     *
     * Setting an option to the empty string, or to the case insensitive string
     * "null" has the effect of clearing it. If the provided value is neither
     * of those then it is accepted only if it is different from the current
     * value. This saves a memory leak since user input is strdup'd for
     * storage. The strdup'd pointer is freed only when user clears the option
     * using either the empty string or the string "null".
     */
    cint_logger_cmd_select_string_value(func,
                                        Cint_logger_cfg.cint_filter_fn,
                                        &new_cfg.cint_filter_fn,
                                        CINT_LOGGER_CMD_FUNC_MAXLEN);

    cint_logger_cmd_select_string_value(logfunc,
                                        Cint_logger_cfg.cint_logfile_filter_fn,
                                        &new_cfg.cint_logfile_filter_fn,
                                        CINT_LOGGER_CMD_FUNC_MAXLEN);

    cint_logger_cmd_select_string_value(replayfunc,
                                        Cint_logger_cfg.cint_replay_filter_fn,
                                        &new_cfg.cint_replay_filter_fn,
                                        CINT_LOGGER_CMD_FUNC_MAXLEN);

    cint_logger_cmd_select_string_value(logfile,
                                        Cint_logger_cfg.logfile,
                                        &new_cfg.logfile,
                                        CINT_LOGGER_CMD_FPATH_MAXLEN);

    cint_logger_cmd_select_string_value(raw_logfile,
                                        Cint_logger_cfg.raw_logfile,
                                        &new_cfg.raw_logfile,
                                        CINT_LOGGER_CMD_FPATH_MAXLEN);

    parse_arg_eq_done(&pt);

    cli_out("\nDesired configuration: (actual may differ)\n\n");
    cint_logger_log_variable(&cfg_pds, "", &new_cfg, 4);
    cli_out("\n\n");

    /*
     * pause the logger...
     */
    CINT_LOGGER_LOCK;

    /*
     * ...and pause all the logging to file
     */
    CINT_VARIABLES_LOCK;

    /*
     * if a file is open, close it if either there's no name assigned for
     * it, or a new file name has been provided by the user. If no file was
     * open and a new log file name has just been provided, open the new
     * log file.
     */
    if (Cint_logger_cfg.logfile_fp &&
        (!Cint_logger_cfg.logfile || !new_cfg.logfile ||
          CINT_STRNCMP(Cint_logger_cfg.logfile, new_cfg.logfile, CINT_LOGGER_CMD_FPATH_MAXLEN))) {

        cli_out("Closing existing FILE pointer: %p (\"%s\")\n", Cint_logger_cfg.logfile_fp,
                (Cint_logger_cfg.logfile ? Cint_logger_cfg.logfile : "??"));
        CINT_FCLOSE(Cint_logger_cfg.logfile_fp);
        Cint_logger_cfg.logfile_fp = new_cfg.logfile_fp = NULL;
    }

    if (!Cint_logger_cfg.logfile_fp && new_cfg.logfile) {

        cli_out("Opening file \"%s\" for writing\n", new_cfg.logfile);
        new_cfg.logfile_fp = CINT_FOPEN(new_cfg.logfile, "a");
        if (!new_cfg.logfile_fp) {
            cli_out("Error: failed to open \"%s\" for writing\n", new_cfg.logfile);
            ret = CMD_FAIL;
        }
    }

    /*
     * ... repeat aforementioned steps for the raw logfile also
     */
    if (Cint_logger_cfg.raw_logfile_fp &&
        (!Cint_logger_cfg.raw_logfile || !new_cfg.raw_logfile ||
          CINT_STRNCMP(Cint_logger_cfg.raw_logfile, new_cfg.raw_logfile, CINT_LOGGER_CMD_FPATH_MAXLEN))) {

        cli_out("Closing existing raw FILE pointer: %p (\"%s\")\n", Cint_logger_cfg.raw_logfile_fp,
                (Cint_logger_cfg.raw_logfile ? Cint_logger_cfg.raw_logfile : "??"));
        CINT_FCLOSE(Cint_logger_cfg.raw_logfile_fp);
        Cint_logger_cfg.raw_logfile_fp = new_cfg.raw_logfile_fp = NULL;
    }

    if (!Cint_logger_cfg.raw_logfile_fp && new_cfg.raw_logfile) {

        cli_out("Opening raw file \"%s\" for writing\n", new_cfg.raw_logfile);
        new_cfg.raw_logfile_fp = CINT_FOPEN(new_cfg.raw_logfile, "a");
        if (!new_cfg.raw_logfile_fp) {
            cli_out("Error: failed to open \"%s\" for writing\n", new_cfg.raw_logfile);
            ret = CMD_FAIL;
        }
    }

    Cint_logger_cfg.enabled                = new_cfg.enabled;
    if ((Cint_logger_cfg.mode != new_cfg.mode) &&
        ((Cint_logger_cfg.mode == cintLoggerModeReplay) ||
         (new_cfg.mode == cintLoggerModeReplay))) {

        cint_logger_cint_const_fields(new_cfg.mode != cintLoggerModeReplay);
        cli_out("call_ctxt attributes updated due to mode change from/to replay.\n");
    }
    Cint_logger_cfg.mode                   = new_cfg.mode;

    if (Cint_logger_cfg.cint_filter_fn &&
        (Cint_logger_cfg.cint_filter_fn != new_cfg.cint_filter_fn)) {
        CINT_FREE(Cint_logger_cfg.cint_filter_fn);
    }
    Cint_logger_cfg.cint_filter_fn         = new_cfg.cint_filter_fn;

    if (Cint_logger_cfg.logfile &&
        (Cint_logger_cfg.logfile != new_cfg.logfile)) {
        CINT_FREE(Cint_logger_cfg.logfile);
    }
    Cint_logger_cfg.logfile                = new_cfg.logfile;

    Cint_logger_cfg.logfile_fp             = new_cfg.logfile_fp;
    Cint_logger_cfg.log_to_file            = new_cfg.log_to_file;

    if (Cint_logger_cfg.cint_logfile_filter_fn &&
        (Cint_logger_cfg.cint_logfile_filter_fn != new_cfg.cint_logfile_filter_fn)) {
        CINT_FREE(Cint_logger_cfg.cint_logfile_filter_fn);
    }
    Cint_logger_cfg.cint_logfile_filter_fn = new_cfg.cint_logfile_filter_fn;

    if (Cint_logger_cfg.raw_logfile &&
        (Cint_logger_cfg.raw_logfile != new_cfg.raw_logfile)) {
        CINT_FREE(Cint_logger_cfg.raw_logfile);
    }
    Cint_logger_cfg.raw_logfile            = new_cfg.raw_logfile;

    Cint_logger_cfg.raw_logfile_fp         = new_cfg.raw_logfile_fp;

    /*
     * If a replay function was previously installed, and a new name was
     * provided by the user, then free the previously installed string since it
     * was created from a strdup.
     */
    if (Cint_logger_cfg.cint_replay_filter_fn &&
        (Cint_logger_cfg.cint_replay_filter_fn != new_cfg.cint_replay_filter_fn)) {
        CINT_FREE(Cint_logger_cfg.cint_replay_filter_fn);
    }
    Cint_logger_cfg.cint_replay_filter_fn = new_cfg.cint_replay_filter_fn;

    Cint_logger_cfg.base_indent            = new_cfg.base_indent;

    cli_out("\n");

    CINT_VARIABLES_UNLOCK;

    CINT_LOGGER_UNLOCK;

    return ret;
}


static cmd_result_t
cint_logger_cmd_cbtest (int unit, args_t *a)
{
    parse_table_t pt;
    int totalsz     = 100000;
    int nfreeze     = 10;
    int alloc       = 4000;
    int iter        = 100;
    int print_every = 1;
    int consumer_thread_prio = CINT_LOGGER_THREAD_PRIO_DEFAULT;
    int seed = 0;

    parse_table_init(unit, &pt);
    parse_table_add(&pt, "TotalSZ"            , PQ_DFL | PQ_INT , NULL       , &totalsz                , NULL);
    parse_table_add(&pt, "NFreeze"            , PQ_DFL | PQ_INT , NULL       , &nfreeze                , NULL);
    parse_table_add(&pt, "Iterations"         , PQ_DFL | PQ_INT , NULL       , &iter                   , NULL);
    parse_table_add(&pt, "Alloc"              , PQ_DFL | PQ_INT , NULL       , &alloc                  , NULL);
    parse_table_add(&pt, "PrintEvery"         , PQ_DFL | PQ_INT , NULL       , &print_every            , NULL);
    parse_table_add(&pt, "ConsumerThreadPrio" , PQ_DFL | PQ_INT , NULL       , &consumer_thread_prio   , NULL);
    parse_table_add(&pt, "SeedRANDom"         , PQ_DFL | PQ_INT , NULL       , &seed                   , NULL);

    if (parse_arg_eq(a, &pt) < 0) {
        cli_out("Error: invalid option \"%s\"\n", ARG_CUR(a));
        parse_arg_eq_done(&pt);
        return CMD_USAGE;
    }

    parse_eq_format(&pt);

    return cbuf_circular_buffer_test(totalsz, nfreeze, alloc, iter, print_every, consumer_thread_prio, seed);

}


static cmd_result_t
cint_logger_cmd_show (int unit, args_t *a)
{
    circular_buffer_t *cbuf = Cint_logger_cfg.cbuf;
    circular_buffer_t *show_cbuf;
    parse_table_t pt;
    int firstN = 0;
    int lastN = 0;
    int skipN = 0;
    el_t *ler;
    char *fn = NULL;
    char *file = NULL;
    int fileOpts = 0;
    cint_logger_thread_data_t *tdata = NULL;
    char *fopenOpts = NULL;
    int file_opened = 0;
    void *logfile_fp_save = NULL;
    int logger_is_active, logger_is_active_save;
    cint_logger_call_ctxt_t tcall_ctxt, *call_ctxt = NULL;
    cint_logger_api_params_t tctxt, *ctxt = NULL;


    if (!cbuf) {

        cli_out("No log buffer found. Exit\n");
        return CMD_OK;
    }

    if (!ARG_CNT(a)) {

        cli_out("\nCircular Buffer Info\n\n");

        cli_out("    Total Buffer Size   : %9d\n", cbuf->totalsz);
        cli_out("    Buffer Space Used   : %9d\n", cbuf->bufused);
        cli_out("    Total Entries Count : %9d\n", cbuf->bufcnt);
        cli_out("    NFreeze Entries     : %9d\n", cbuf->nfreeze);
        cli_out("    NFreeze Space Used  : %9d\n", ((int)((char*)cbuf->base - (char*)cbuf->buffer_start)));
        cli_out("    Successful Requests : %9llu\n", cbuf->nreqs);
        cli_out("    Failed     Requests : %9llu\n", cbuf->nfailed_reqs);


        return CMD_OK;
    }

    parse_table_init(unit, &pt);
    parse_table_add(&pt, "FirstN"      , PQ_DFL | PQ_INT , NULL                    , &firstN                 , NULL);
    parse_table_add(&pt, "LastN"       , PQ_DFL | PQ_INT , NULL                    , &lastN                  , NULL);
    parse_table_add(&pt, "SkipN"       , PQ_DFL | PQ_INT , NULL                    , &skipN                  , NULL);
    parse_table_add(&pt, "ForMaTter"   , PQ_STRING       , NULL                    , &fn                     , NULL);
    parse_table_add(&pt, "FILE"        , PQ_STRING       , NULL                    , &file                   , NULL);
    parse_table_add(&pt, "FileOPTionS" , PQ_DFL | PQ_HEX , NULL                    , &fileOpts               , NULL);

    if (parse_arg_eq(a, &pt) <= 0) {
        cli_out("Error: invalid option \"%s\"\n", ARG_CUR(a));
        parse_arg_eq_done(&pt);
        return CMD_USAGE;
    }

    parse_eq_format(&pt);

    tdata = cint_logger_thread_specific();

    logfile_fp_save = (*tdata).logfile_fp;

    logger_is_active_save = (*tdata).logger_is_active;

    logger_is_active = CINT_LOGGER_CINT_OP_BSL;

    if (file && CINT_STRNCMP(file, "", CINT_LOGGER_CMD_FPATH_MAXLEN)) {

        switch (fileOpts & 1) {
        case 0:
        default:
            fopenOpts = "w";
            break;
        case 1:
            fopenOpts = "a";
            break;
        }

        (*tdata).logfile_fp = CINT_FOPEN(file, fopenOpts);

        if (!(*tdata).logfile_fp) {

            cli_out("Unable to open %s with fileOpts %s - stopping\n", file, fopenOpts);

            (*tdata).logfile_fp = logfile_fp_save;

            return CMD_USAGE;
        }

        file_opened = 1;
        logger_is_active = CINT_LOGGER_CINT_OP_THREAD_FILE;
    }

    (*tdata).logger_is_active = logger_is_active;

    if (firstN) {

        show_cbuf = cbuf_splice_entries(cbuf, skipN, firstN,
                                        cint_logger_copy_arguments_buffer,
                                        "firstN show copy",
                                        0,
                                        0,
                                        NULL,
                                        NULL);

        if (!show_cbuf) {

            cli_out("Failed to create shadow buffer to show from\n");
            return CMD_FAIL;
        }

        for (ler = show_cbuf->head;
             ler && firstN;
             ler = ler->next) {

            firstN--;

            call_ctxt = NULL;
            ctxt = NULL;

            if (fn && CINT_STRNCMP(fn, "", CINT_LOGGER_CMD_FUNC_MAXLEN)) {

                CINT_MEMSET(&tcall_ctxt, 0, sizeof(tcall_ctxt));
                call_ctxt = &tcall_ctxt;
                CINT_MEMSET(&tctxt, 0, sizeof(tctxt));
                ctxt = &tctxt;

                if (cint_logger_run_filter_offline(fn, PLD(ler), ler->sz, 1, &call_ctxt, &ctxt)) {
                    continue;
                }
            }

            CINT_VARIABLES_LOCK;

            cint_logger_log_arguments_buffer(call_ctxt, ctxt, PLD(ler), ler->sz);

            CINT_VARIABLES_UNLOCK;
        }

        cbuf_circular_buffer_t_destroy(show_cbuf);

    }

    if (lastN) {

        show_cbuf = cbuf_splice_entries(cbuf, skipN, -lastN,
                                        cint_logger_copy_arguments_buffer,
                                        "lastN show copy",
                                        0,
                                        0,
                                        NULL,
                                        NULL);

        if (!show_cbuf) {

            cli_out("Failed to create shadow buffer to show from\n");
            return CMD_FAIL;
        }

        for (ler = show_cbuf->head;
             ler && lastN;
             ler = ler->next) {

            lastN--;

            call_ctxt = NULL;
            ctxt = NULL;

            if (fn && CINT_STRNCMP(fn, "", CINT_LOGGER_CMD_FUNC_MAXLEN)) {

                CINT_MEMSET(&tcall_ctxt, 0, sizeof(tcall_ctxt));
                call_ctxt = &tcall_ctxt;
                CINT_MEMSET(&tctxt, 0, sizeof(tctxt));
                ctxt = &tctxt;

                if (cint_logger_run_filter_offline(fn, PLD(ler), ler->sz, 1, &call_ctxt, &ctxt)) {
                    continue;
                }
            }

            CINT_VARIABLES_LOCK;

            cint_logger_log_arguments_buffer(call_ctxt, ctxt, PLD(ler), ler->sz);

            CINT_VARIABLES_UNLOCK;
        }

        cbuf_circular_buffer_t_destroy(show_cbuf);

    }

    if (file_opened && (*tdata).logfile_fp) {

        CINT_FCLOSE((*tdata).logfile_fp);
    }

    (*tdata).logger_is_active = logger_is_active_save;

    (*tdata).logfile_fp       = logfile_fp_save;

    return CMD_OK;
}

static cmd_result_t
cint_logger_cmd_decode (int unit, args_t *a)
{
    parse_table_t pt;
    uint32 firstN = 0;
    uint64 count, firstN_64;
    uint32 skipN = 0;
    char *ifile = NULL;
    void *ifile_fp = NULL;
    int ifile_read_sz = 0;
    char *fn = NULL;
    char *file = NULL;
    int fileOpts = 1;
    cint_logger_thread_data_t *tdata = NULL;
    char *fopenOpts = NULL;
    int file_opened = 0;
    void *logfile_fp_save = NULL;
    int logger_is_active, logger_is_active_save;
    cint_logger_call_ctxt_t tcall_ctxt, *call_ctxt = NULL;
    cint_logger_api_params_t tctxt, *ctxt = NULL;
    int items, bufsz;
    void *buf;

    parse_table_init(unit, &pt);
    parse_table_add(&pt, "InputFILE"   , PQ_STRING       , NULL                    , &ifile                  , NULL);
    parse_table_add(&pt, "FirstN"      , PQ_DFL | PQ_INT , NULL                    , &firstN                 , NULL);
    parse_table_add(&pt, "SkipN"       , PQ_DFL | PQ_INT , NULL                    , &skipN                  , NULL);
    parse_table_add(&pt, "ForMaTter"   , PQ_STRING       , NULL                    , &fn                     , NULL);
    parse_table_add(&pt, "FILE"        , PQ_STRING       , NULL                    , &file                   , NULL);
    parse_table_add(&pt, "FileOPTionS" , PQ_DFL | PQ_HEX , NULL                    , &fileOpts               , NULL);

    if (parse_arg_eq(a, &pt) <= 0) {
        cli_out("Error: invalid option \"%s\"\n", ARG_CUR(a));
        parse_arg_eq_done(&pt);
        return CMD_USAGE;
    }

    parse_eq_format(&pt);

    if (ifile && CINT_STRNCMP(ifile, "", CINT_LOGGER_CMD_FPATH_MAXLEN)) {

        ifile_fp = CINT_FOPEN(ifile, "r");

        if (!ifile_fp) {

            cli_out("Unable to open \"%s\" for reading - stopping\n", ifile);

            return CMD_USAGE;
        }
    } else {

        cli_out("Must provide an InputFILE to decode\n");

        return CMD_USAGE;
    }

    tdata = cint_logger_thread_specific();

    logfile_fp_save = (*tdata).logfile_fp;

    logger_is_active_save = (*tdata).logger_is_active;

    logger_is_active = CINT_LOGGER_CINT_OP_BSL;

    COMPILER_64_SET(firstN_64, 0, firstN);

    if (file && CINT_STRNCMP(file, "", CINT_LOGGER_CMD_FPATH_MAXLEN)) {

        switch (fileOpts & 1) {
            case 0:
            default:
                fopenOpts = "w";
                break;
            case 1:
                fopenOpts = "a";
                break;
        }

        (*tdata).logfile_fp = CINT_FOPEN(file, fopenOpts);

        if (!(*tdata).logfile_fp) {

            cli_out("Unable to open %s with fileOpts %s - stopping\n", file, fopenOpts);

            (*tdata).logfile_fp = logfile_fp_save;

            CINT_FCLOSE(ifile_fp);

            return CMD_USAGE;
        }

        file_opened = 1;
        logger_is_active = CINT_LOGGER_CINT_OP_THREAD_FILE;
        /*
         * if decoding into a file, we process from start of file until EOF.
         * Set firstN to a large value if it was not set by user
         */
        if (COMPILER_64_IS_ZERO(firstN_64)) {
            COMPILER_64_SET(firstN_64, SAL_UINT32_MAX, SAL_UINT32_MAX);
        }
    }

    (*tdata).logger_is_active = logger_is_active;

    COMPILER_64_COPY(count, firstN_64);

    for (; !COMPILER_64_IS_ZERO(count); ) {

        items = CINT_FREAD(&bufsz, sizeof(int), 1, ifile_fp);
        if (items < 1) {
            if (CINT_FERROR(ifile_fp)) {
                cli_out("ERROR: error reading buffer size from file \"%s\"\n", ifile);
            }
            break;
        }
        ifile_read_sz += sizeof(int);

        /*
         * bufsz=0 should never really occur. However, if it does it means that
         * the following record has size=0 and therefore all the following
         * processing may be skipped and we can proceed to fetching size of the
         * next record. This check should also serve to remove taint on the
         * bufsz variable which is received from outside the program.
         */
        if (!bufsz) {
            continue;
        }

        /*
         * Range check to remove taint on bufsz. A negative next-buffer size is
         * meaningless, and adding +1 to SAL_INT32_MAX below at malloc will
         * result in overflow
         */
        if ((bufsz < 0) || (bufsz == SAL_INT32_MAX)) {
            cli_out("ERROR: bufsz=%d is illegal\n", bufsz);
            break;
        }

        buf = CINT_MALLOC(bufsz+1);
        if (!buf) {
            cli_out("ERROR: failed to allocate buffer of size %d\n", bufsz);
            break;
        }
        /*
         * Initialize allocated memory to be on the safe side for
         * string operations.
         * (appease coverity).
         */
        CINT_MEMSET(buf,0,bufsz+1);
        items = CINT_FREAD(buf, bufsz, 1, ifile_fp);
        if (items < 1) {
            if (CINT_FERROR(ifile_fp)) {
                cli_out("ERROR: error reading buffer of size %d from file \"%s\"\n", bufsz, ifile);
            } else {
                cli_out("ERROR: unexpected EOF \"%s\"\n", ifile);
            }
            CINT_FREE(buf);
            break;
        }
        ifile_read_sz += bufsz;

        /*
         * Ensure buf is NULL terminated for string operations downstream
         */
        ((char*)buf)[bufsz] = '\0';

        if (skipN) {
            skipN--;
            CINT_FREE(buf);
            continue;
        }

        call_ctxt = NULL;
        ctxt = NULL;

        if (fn && CINT_STRNCMP(fn, "", CINT_LOGGER_CMD_FUNC_MAXLEN)) {

            CINT_MEMSET(&tcall_ctxt, 0, sizeof(tcall_ctxt));
            call_ctxt = &tcall_ctxt;
            CINT_MEMSET(&tctxt, 0, sizeof(tctxt));
            ctxt = &tctxt;

            if (cint_logger_run_filter_offline(fn, buf, bufsz, 0, &call_ctxt, &ctxt)) {
                CINT_FREE(buf);
                continue;
            }
        }

        CINT_VARIABLES_LOCK;

        cint_logger_log_arguments_buffer(call_ctxt, ctxt, buf, bufsz);

        CINT_VARIABLES_UNLOCK;

        /*
         * coverity explanation:
         *
         * coverity had reported that buf is tainted by CINT_FREAD above, and
         * that is being passed to CINT_FREE...
         *
         * The pointer itself remains unchanged since it is passed by value and
         * not by reference. However, contents of memory pointed by buf should
         * be deemed to be tainted since those are read from an external
         * source - a file.
         *
         * fread writes to the memory starting from the pointer and beyond.
         * Irrespective of the data read from the file, fread does not write to
         * any address less than the pointer. Therefore, guard patterns just
         * before the pointed location which are read by sal_free cannot be
         * changed by fread.
         *
         * Content of the pointed memory was also sanitized first via NULL
         * termination above. This should have removed the taint on data also.
         *
         * Therefore, this warning is false-positive.
         *
         */
        /* coverity[tainted_data:FALSE] */
        CINT_FREE(buf);

        COMPILER_64_SUB_32(count, 1);

    }

    if (file_opened && (*tdata).logfile_fp) {

        CINT_FCLOSE((*tdata).logfile_fp);
    }

    (*tdata).logger_is_active = logger_is_active_save;

    (*tdata).logfile_fp       = logfile_fp_save;

    CINT_FCLOSE(ifile_fp);

    cli_out("Processed %d bytes from %s.\n", ifile_read_sz, ifile);

    return CMD_OK;
}

static cmd_result_t
cint_logger_cmd_test (int u, args_t *a)
{
    parse_table_t pt;
    char *func = NULL;
    int donotrun = 0;
    int unit = 0;
    int rv = -16; /* BCM_E_UNAVAIL */

    parse_table_init(unit, &pt);
    parse_table_add(&pt, "API"      , PQ_STRING       , NULL          , &func        , NULL);
    parse_table_add(&pt, "DoNotRun" , PQ_DFL | PQ_INT , NULL          , &donotrun    , NULL);
    parse_table_add(&pt, "Unit"     , PQ_DFL | PQ_INT , NULL          , &unit        , NULL);
    parse_table_add(&pt, "Rv"       , PQ_DFL | PQ_INT , NULL          , &rv          , NULL);

    if (parse_arg_eq(a, &pt) <= 0) {
        cli_out("Error: invalid option \"%s\"\n", ARG_CUR(a));
        parse_arg_eq_done(&pt);
        return CMD_USAGE;
    }

    parse_eq_format(&pt);

    return cint_logger_test_api_using_cint(func, donotrun, unit, rv);
}


cmd_result_t
cint_logger_cmd (int unit, args_t *a)
{
    static cmd_t subcmd_list[] = {
        { "ConFiG"            , cint_logger_cmd_cfg    , cint_logger_cfg_usage    , cint_logger_cfg_help  },
        { "SHow"              , cint_logger_cmd_show   , cint_logger_show_usage   , cint_logger_show_help },
        { "DeCoDe"            , cint_logger_cmd_decode , cint_logger_decode_usage , cint_logger_decode_help },
        { "CircularBufferTEST", cint_logger_cmd_cbtest , cint_logger_cbtest_usage , cint_logger_cbtest_help },
        { "TEST"              , cint_logger_cmd_test   , cint_logger_test_usage   , cint_logger_test_help   },
    };
    static int subcmd_list_cnt = sizeof(subcmd_list)/sizeof(cmd_t);


    cmd_cint_initialize();

    if (!ARG_CNT(a)) {
        cint_logger_cmd_cfg(unit, a);
        cint_logger_cmd_show(unit, a);
        return CMD_OK;
    }

    return subcommand_execute(unit, a, subcmd_list, subcmd_list_cnt);
}

/* } */
#else
/* { */
/* Make ISO compilers happy. */
typedef int cint_logger_cmd_c_not_empty;
/* } */
#endif
