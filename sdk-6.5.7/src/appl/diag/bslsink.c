/*
 * $Id: $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * Broadcom System Log Sinks
 */

#include <sal/core/libc.h> 
#include <sal/core/time.h>
#include <sal/core/thread.h>
#include <sal/appl/io.h> 
#include <shared/bsl.h>
#include <shared/bslnames.h>
#include <appl/diag/bslenable.h>
#include <appl/diag/bslsink.h>

/* List of output sinks */
static bslsink_sink_t *bslsink_sinks = NULL;

/* System start time */
static sal_time_t init_time;

/* Append string value to prefix */
static int
add_string(char *buf, int max, const char *str)
{
    return sal_snprintf(buf, max, "%s", str);
}

/* Append decimal integer to prefix */
static int
add_dval(char *buf, int max, int dval)
{
    return sal_snprintf(buf, max, "%d", dval);
}

/* Append unsigned decimal integer to prefix */
static int
add_uval(char *buf, int max, uint32 uval)
{
    return sal_snprintf(buf, max, "%u", uval);
}

/* Append minutes/seconds to prefix */
static int
add_msval(char *buf, int max, int msval)
{
    return sal_snprintf(buf, max, ":%02d", msval);
}

/* Append time stamp to prefix */
static int
add_timetamp(char *buf, int max)
{
    int len = 0;
    sal_time_t now = sal_time() - init_time;
    unsigned long hrs, mins, secs;

    secs = now % 60;
    now = now / 60;
    mins = now % 60;
    hrs = now / 60;

    len += add_dval(&buf[len], max-len, hrs);
    len += add_msval(&buf[len], max-len, mins);
    len += add_msval(&buf[len], max-len, secs);

    return len;
}

/* Append micro second timestamp to prefix */
static int
add_hi_res_timestamp(char *buf, int max)
{
    int len = 0;
    unsigned long time = (unsigned long)(sal_time_usecs());

    len += add_uval(buf, max, time);

    return len;
}

/* Append thread name to prefix */
static int
add_thread_name(char *buf, int max, const char *str)
{
    char tname[SAL_THREAD_NAME_MAX_LEN], *name;

    name = sal_thread_name(sal_thread_self(), tname, SAL_THREAD_NAME_MAX_LEN);

    if (name == NULL) {
        name = "unknown thread";
    }

    return add_string(buf, max, name);
}

/*
 * Prefix formatting function
 *
 * Formats log prefix string based on meta data contents and
 * configuration string.
 *
 * The configuration string is similar to a printf format string,
 * except that is uses a different set of format specifiers.
 *
 *   %F - function name
 *   %f - file name
 *   %l - line number
 *   %u - unit number
 *   %p - port number
 *   %x - xtra parameter
 *   %L - layer of this log message source
 *   %S - source of this log message
 *   %s - severity level (string value) of this log message
 *   %t - time stamp (based seconds since system start)
 *   %T - high resolution timestamp (micro seconds)
 *   %P - process or thread name
 */
static int
format_prefix(char *buf, int max, const char *prefix_format,
              bsl_meta_t *meta, int path_offset)
{
    int len = 0;
    const char *str, *cp = prefix_format;

    while (len < max) {
        if (*cp != '%') {
            buf[len++] = *cp;
            if (*cp == 0) {
                return len;
            }
            cp++;
            continue;
        }
        cp++;
        switch (*cp) {
        case 'F':
            str = meta->func;
            if (str == NULL) {
                str = "<nofunc>";
            }
            len += add_string(&buf[len], max-len, str);
            cp++;
            break;
        case 'f':
            str = "<nofile>";
            if (meta->file != NULL) {
                str = &meta->file[path_offset];
            }
            len += add_string(&buf[len], max-len, str);
            cp++;
            break;
        case 'l':
            len += add_dval(&buf[len], max-len, meta->line);
            cp++;
            break;
        case 'u':
            if (meta->unit < 0) {
                len += add_string(&buf[len], max-len, "<nounit>");
            } else {
                len += add_dval(&buf[len], max-len, meta->unit);
            }
            cp++;
            break;
        case 'p':
            len += add_dval(&buf[len], max-len, meta->port);
            cp++;
            break;
        case 'x':
            len += add_dval(&buf[len], max-len, meta->xtra);
            cp++;
            break;
        case 'L':
            str = bsl_layer2str(meta->layer);
            len += add_string(&buf[len], max-len, str);
            cp++;
            break;
        case 'S':
            str = bsl_source2str(meta->source);
            len += add_string(&buf[len], max-len, str);
            cp++;
            break;
        case 's':
            str = bsl_severity2str(meta->severity);
            len += add_string(&buf[len], max-len, str);
            cp++;
            break;
        case 't':
            len += add_timetamp(&buf[len], max-len);
            cp++;
            break;
        case 'T':
            len += add_hi_res_timestamp(&buf[len], max-len);
            cp++;
            break;
        case 'P':
            len += add_thread_name(&buf[len], max-len, str);
            cp++;
            break;
        default:
            /* Bad format */
            return len;
        }
    }
    return len;
}

/*
 * Check that severity is within a specific range
 */
static int
severity_in_range(bslsink_severity_range_t *range, bsl_severity_t val)
{
    if (range == NULL) {
        return 0;
    }
    return (val >= range->min && val <= range->max);
}

/*
 * Convenience function for printing prefix string
 */
static int
sink_printf(bslsink_sink_t *sink, const char *format, ...)
{
    int rv;
    va_list args;

    va_start(args, format);  
    rv = sink->vfprintf(sink->file, format, args);
    va_end(args);

    return rv;
}

/*
 * Additional filtering based on meta data
 */
int
bslsink_out(bslsink_sink_t *sink, bsl_meta_t *meta,
            const char *format, va_list args)
{
    int unit;
    bsl_severity_t sev = meta->severity;
    char buf[BSLSINK_PREFIX_MAX];

    if (sink->check && sink->check(meta) == 0) {
        return 0;
    }
    if (!severity_in_range(&sink->enable_range, sev)) {
        return 0;
    }
    if (meta->source == bslSourceEcho &&
        (sink->options & BSLSINK_OPT_NO_ECHO)) {
        return 0;
    }
    unit = meta->unit;
    if (unit == BSL_UNIT_UNKNOWN) {
        /* Map to pseudo-unit to allow proper filtering */
        unit = BSLSINK_UNIT_UNKNOWN;
    }
    if (unit >= 0 && unit < BSLSINK_MAX_NUM_UNITS) {
        if (SHR_BITGET(sink->units, unit) == 0) {
            return 0;
        }
    }
    if (meta->port >= 0 && meta->port < BSLSINK_MAX_NUM_PORTS) {
        if (SHR_BITGET(sink->ports, meta->port) == 0) {
            return 0;
        }
    }
    if (meta->xtra >= 0 && sink->xtra >= 0) {
        if (meta->xtra != sink->xtra) {
            return 0;
        }
    }
    if ((meta->source == bslSourceShell || meta->source == bslSourceEcho) &&
        (sink->options & BSLSINK_OPT_SHELL_PREFIX) == 0) {
        /* Skip prefix for shell output */
    } else if (severity_in_range(&sink->prefix_range, sev)) {
        if (meta->options & BSL_META_OPT_START) {
            format_prefix(buf, sizeof(buf)-1, sink->prefix_format,
                          meta, sink->path_offset);
            buf[sizeof(buf)-1] = 0;
            sink_printf(sink, buf);
            if (meta->options & BSL_META_OPT_PFX_NL) {
                sink_printf(sink, "\n");
            }
        }
    }
    return sink->vfprintf(sink->file, format, args);
}

void
bslsink_sink_t_init(bslsink_sink_t *sink)
{
    sal_memset(sink, 0, sizeof(*sink));
    SHR_BITSET_RANGE(sink->units, 0, BSLSINK_MAX_NUM_UNITS);
    SHR_BITSET_RANGE(sink->ports, 0, BSLSINK_MAX_NUM_PORTS);
    sink->xtra = -1;
#ifndef BSLSINK_FULL_FILE_PATH
    {
        char *ptr, *file = BSL_FILE;

        /* Calculate position of SDK root in BSL_FILE (__FILE__) */
        ptr = sal_strstr(file, "src/appl/diag/bslsink.c");
        if (ptr != NULL) {
            sink->path_offset = PTR_TO_UINTPTR(ptr) - PTR_TO_UINTPTR(file);
        }
    }
#endif
}

int
bslsink_sink_cleanup(bslsink_sink_t *sink)
{
    if (sink->cleanup) {
        sink->cleanup(sink);
    }
    return 0;
}

int
bslsink_sink_add(bslsink_sink_t *new_sink)
{
    bslsink_sink_t *sink;
    int sink_id = 0;

    if (bslsink_sinks == NULL) {
        /* First sink */
        bslsink_sinks = new_sink;
        new_sink->sink_id = sink_id;
        return sink_id;
    }
    sink = bslsink_sinks;
    sink_id++;
    while (sink->next != NULL) {
        sink = sink->next;
        sink_id++;
    }
    sink->next = new_sink;
    new_sink->sink_id = sink_id;
    return sink_id;
}

bslsink_sink_t *
bslsink_sink_find(const char *name)
{
    bslsink_sink_t *sink = bslsink_sinks;

    if (name == NULL) {
        return NULL;
    }

    while (sink != NULL) {
        if (sal_strcmp(name, sink->name) == 0) {
            return sink;
        }
        sink = sink->next;
    }
    return NULL;
}

bslsink_sink_t *
bslsink_sink_find_by_id(int sink_id)
{
    bslsink_sink_t *sink = bslsink_sinks;

    while (sink != NULL) {
        if (sink->sink_id == sink_id) {
            return sink;
        }
        sink = sink->next;
    }
    return NULL;
}

int
bslsink_cleanup(void)
{
    bslsink_sink_t *sink = bslsink_sinks;

    while (sink != NULL) {
        bslsink_sink_cleanup(sink);
        sink = sink->next;
    }
    return 0;
}

int
bslsink_init(void)
{
    bslsink_sinks = NULL;
    init_time = sal_time();

    return 0;
}
