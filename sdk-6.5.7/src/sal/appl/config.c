/*
 * $Id: config.c,v 1.28.6.1 Broadcom SDK $
 * $Copyright: (c) 2016 Broadcom.
 * Broadcom Proprietary and Confidential. All rights reserved.$
 *
 * SAL Configuration Variable Support
 *
 * The SAL layer maintains a set of configuration variables for use
 * by any upper layers (such as SOC or BCM).  It is maintained in the
 * SAL layer without using anything other than the standard lib calls
 * to avoid requiring anything other than just itself.
 *
 * These config variables can either be compiled into the image or read
 * from a file during initialization.
 *
 * Precompiled values are defined in the function sal_config_init_defaults()
 * which is automatically built from the uncommented values
 * in $SDK/rc/config.bcm using the bcm2c.pl script.  This generates a source
 * file called platform_defines.c
 *
 * Runtime values are loaded by reading the file config.bcm, typically
 * stored in the switch's flash file system.  The variables are stored one
 * per line in the file, in the form VARIABLE=VALUE.  Comment lines beginning
 * with a # character and empty lines consisting of only white space are
 * ignored.  The file $SDK/rc/config.bcm is an example that can be used
 * as a guide.
 *
 * If the image is compile with NO_FILEIO or SAL_CONFIG_FILE_DISABLE
 * defined, then the runtime values are not loaded from an external file.
 */

#ifdef __KERNEL__
#include <linux/string.h>
#include <linux/ctype.h>
#else
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#endif
#include <assert.h>

#include <sal/core/libc.h>
#include <sal/core/alloc.h>
#include <sal/core/spl.h>
#include <sal/types.h>
#include <sal/appl/config.h>
#include <sal/appl/io.h>
#include <sal/appl/sal.h>
#include <shared/util.h>

#ifdef  NO_FILEIO
#undef  SAL_CONFIG_FILE_DISABLE
#define SAL_CONFIG_FILE_DISABLE
#endif  /* NO_FILEIO */

typedef uint16  sc_hash_t;

/*
 * Speed up searches by first checking if a hash matches.
 * It encodes the first and last character, as well as string length.
 */

#define MAX_CONFIG_HASH_COUNT 1024
#define SC_HASH(_n, _h) do { \
    uint32 _hash = 5381; \
    uint32 _i = 0; \
    while (_n[_i] != 0) { \
        _hash = ((_hash << 5) + _hash) + _n[_i]; \
        _i++; \
    } \
    _hash = ((_hash << 5) + _hash) + _i; \
    _h = (sc_hash_t)(_hash % MAX_CONFIG_HASH_COUNT); \
    } while (0)
  
typedef struct sc_s {
    struct sc_s *sc_next;
    char    *sc_name;
    char    *sc_value;
    uint16  sc_flag;  /* Used for duplicate checking, flushing, and hits */
    uint16  sc_line;
    sc_hash_t   sc_hash;    
} sc_t;

/* Defines for sc_t->sc_flag */
#define SC_COMPILE_VALUE 0x0001
#define SC_FLUSHED       0x0002
#define SC_HIT           0x0004

static char *sal_config_file_name = NULL;
static char *sal_config_temp_name = NULL;

static sc_t *sal_config_list[MAX_CONFIG_HASH_COUNT];

/*
 *  sal_config_compiled_values is the count of compiled values
 *  sal_config_imported_values is the count of imported values
 *  from sal_config_file_name (ex. config.bcm)
 */

static uint32 sal_config_compiled_values = 0;
static uint32 sal_config_imported_values = 0;

#define FREE_SC(_s) if (_s) {                   \
                if ((_s)->sc_name) sal_free((_s)->sc_name); \
                if ((_s)->sc_value) sal_free((_s)->sc_value);\
                sal_free(_s);               \
            }

#ifndef SAL_CONFIG_FILE_DISABLE
#include <soc/property.h>
static char *sal_config_prop_names[] = BCM_SOC_PROP_NAMES_INITIALIZER;          
#endif

/*
 * Function: 
 *  wildcard_search
 * Purpose: 
 *  search for "*" in name and save prefix as wildcard 
 *  
 * Returns: 
 *  wildcard returns prefix of name
 *  length returns length of wildcard
 */
char*
wildcard_search(const char *name, char *wildcard, int *length)
{
    int len, index = 0;
    const char *m = name;

    len = strlen(name);
    wildcard = sal_alloc(len + 1, "wildcard");

    while (index < len) {
        if (*m == '*') {
            *(wildcard+index) = 0;
            *length = index;
            return wildcard;
        }
        else {
            *(wildcard+index) = *m;
            m++;
            index++;
        }
    }

    /* no '*' found */
    sal_free(wildcard);
    return NULL;
}


/*
 * Function:
 *  sal_config_file_set
 * Purpose:
 *  Set the name of the active configuration file.
 * Parameters:
 *  fname - name of config file (if NULL, use default; if empty
 *      string, there is no config file)
 *  tname - name of temporary file that gets renamed to config file
 *      (must be given if fname is non-NULL and non-empty)
 * Returns:
 *  -1  Illegal parameter
 *  0   Success
 * Notes:
 *  Makes a copy of the input string.
 */

int
sal_config_file_set(const char *fname, const char *tname)
{
    if (sal_config_file_name != NULL) {
    sal_free(sal_config_file_name);
    sal_config_file_name = NULL;
    }

    if (sal_config_temp_name != NULL) {
    sal_free(sal_config_temp_name);
    sal_config_temp_name = NULL;
    }

    if (fname != NULL) {
    sal_config_file_name = sal_strdup(fname);

    if (fname[0] != 0) {
        if (tname == NULL || tname[0] == 0) {
        return -1;
        }

        sal_config_temp_name = sal_strdup(tname);
    }
    }

    return 0;
}

/*
 * Function:
 *  sal_config_file_get
 * Purpose:
 *  Return the config file names
 * Parameters:
 *  fname - (OUT) Name of config file, NULL if none
 *  tname - (OUT) Name of temporary file, NULL if none
 * Returns:
 *  0   Success
 */

int
sal_config_file_get(const char **fname, const char **tname)
{
    *fname = (sal_config_file_name != NULL ?
          sal_config_file_name : SAL_CONFIG_FILE);
    *tname = (sal_config_temp_name != NULL ?
          sal_config_temp_name : SAL_CONFIG_TEMP);

    return 0;
}

/*
 * Function:
 *  sal_config_find
 * Purpose:
 *  Find a config entry for the specified name.
 * Parameters:
 *  name - name of variable to recover
 * Returns:
 *  NULL - not found
 *  !NULL - pointer to value
 */

static sc_t *
sal_config_find(const char *name)
{
    sc_t  *sc;
    sc_hash_t hash;
    SC_HASH(name, hash);
    sc = sal_config_list[hash];
    while (sc != NULL) {
        if ((sc->sc_name != NULL) && (sc->sc_value != NULL) && (sal_strcmp(sc->sc_name, name) == 0)) {
            break;
        }
        sc = sc->sc_next;
    }
    return sc;
}

#ifndef SAL_CONFIG_FILE_DISABLE /* Only used by fileio functions */
static int
sal_config_get_lvalue(char *str, sc_t *sc)
{
    char *equals_loc;

    if ((equals_loc = strchr(str, '=')) == NULL) {
        if (sc != NULL) {
            FREE_SC(sc);
        }
        return FALSE;
    }

    while (isspace((unsigned)*str)) {
        str++;              /* skip leading whitespace */
    }

    if (str == equals_loc) {
        if (sc != NULL) {
            FREE_SC(sc);
        }
        return FALSE;           /* lvalue is empty or only whitespace */
    }

    while (isspace((unsigned)*(equals_loc - 1)) && equals_loc > str) {
        equals_loc--;       /* trim trailing whitespace */
    }

    sc->sc_name = sal_alloc(equals_loc - str + 1, "config name");
    sal_strncpy(sc->sc_name, str, equals_loc - str);

    sc->sc_name[equals_loc - str] = '\0';

    SC_HASH(sc->sc_name, sc->sc_hash);
    return TRUE;
}

static int
sal_config_get_rvalue(char *str, sc_t *sc)
{
    char *begin;
    char *end;

    if ((begin = strchr(str, '=')) == NULL) {
        if (sc != NULL) {
            FREE_SC(sc);
        }
        return FALSE;
    }

    begin++;             /* Move past '=' */
    while (isspace((unsigned)*begin)) {
        begin++;         /* Trim leading whitespace */
    }

    for (end = begin + strlen(begin) - 1; isspace((unsigned)*end); end--)
        *end = '\0';     /* Trim trailing whitespace */

    sc->sc_value = sal_alloc(end - begin + 2, "config value");
    if (sc->sc_value == NULL) {
        sal_printf("sal_config_parse: Memory allocation failed\n");
        FREE_SC(sc);
        return FALSE;
    }

    sal_strncpy(sc->sc_value, begin, end - begin + 1);
    sc->sc_value[end - begin + 1] = '\0';

    return TRUE;
}

/*
 * Function:
 *  sal_config_parse
 * Purpose:
 *  Parse a single input line.
 * Parameters:
 *  str - pointer to null terminated input line.
 * Returns:
 *  NULL - failed to parse
 *  !NULL - pointer to sc entry.
 * Notes:
 *  Does not modify input line.
 */
static  sc_t    *
sal_config_parse(char *str)
{
    sc_t    *sc;

    sc = (sc_t *)sal_alloc(sizeof(sc_t), "config parse");
    if (sc != NULL) {
        sal_memset(sc, 0, sizeof(sc_t));
    }
    
    if (!sc || !sal_config_get_lvalue(str, sc) || 
        !sal_config_get_rvalue(str, sc)) {
        return NULL;
    }

    sc->sc_next = NULL;
    return sc;
}
#endif  /* SAL_CONFIG_FILE_DISABLE */

/*
 * Function:
 *  sal_config_init
 * Purpose:
 *  If not already initialized, call refresh.
 * Parameters:
 *  None
 * Returns:
 *  0 - success
 *  -1 - failed.
 */

int
sal_config_init(void)
{
    static int initdone = FALSE;

    if (!initdone) {
        sal_memset(sal_config_list, 0, sizeof(sal_config_list));
        initdone = TRUE;
        return sal_config_refresh();
    }

    return 0;
}

#ifndef SAL_CONFIG_FILE_DISABLE
int
sal_config_search_valid_prop(char *name)
{
    int i;

    for (i = 0; sal_config_prop_names[i][0] != '\0'; i++) {
        if (sal_strcasecmp(sal_config_prop_names[i], name) == 0) {
            return TRUE;
        }
    }

    return FALSE;
}

/*
 * Function:
 *   sal_config_prop_is_known
 * Purpose:
 *   Determine if a given property is valid or not
 * Returns:
 *   TRUE if property exists in property.h, FALSE if it does not
 */
int
sal_config_prop_is_known(sc_t *sc)
{
    static char name[256];
    char *loc;

    sal_strncpy(name, sc->sc_name, sizeof(name) - 1);
    name[sizeof(name) - 1] = '\0';
    if (sal_config_search_valid_prop(name)) {
        return TRUE;
    }

    /* try removing the . if there is one, for "prop.0" style */
    loc = sal_strrchr(name, '.');
    if (loc != NULL) {
        *loc = '\0';
        if (sal_config_search_valid_prop(name)) {
            return TRUE;
        }
    }

    /* try removing the . again if there is another one, 
       for "prop.port.0" style */
    loc = sal_strrchr(name, '.');
    if (loc != NULL) {
        *loc = '\0';
        if (sal_config_search_valid_prop(name)) {
            return TRUE;
        }
    }

    /* try removing the . again if there is another one, 
       for "prop.prop1.port.0" style */
    loc = sal_strrchr(name, '.');
    if (loc != NULL) {
        *loc = '\0';
        if (sal_config_search_valid_prop(name)) {
            return TRUE;
        }
    }

    /* try removing the . again if there is another one, 
       for "prop.prop1.prop2.port.0" style */
    loc = sal_strrchr(name, '.');
    if (loc != NULL) {
        *loc = '\0';
        if (sal_config_search_valid_prop(name)) {
            return TRUE;
        }
    }

    /* try removing the . again if there is another one, 
       for "prop.prop1.prop2.prop3.port.0" style */
    loc = sal_strrchr(name, '.');
    if (loc != NULL) {
        *loc = '\0';
        if (sal_config_search_valid_prop(name)) {
            return TRUE;
        }
    }
  
    /* try removing the last brace if it exists, for "prop{0}" style */
    loc = sal_strrchr(name, '{');
    if (loc != NULL) {
        *loc = '\0';
        if (sal_config_search_valid_prop(name)) {
            return TRUE;
        }
    }

    /* try removing the last underscore if it exists, for "prop_xe.0" style */
    loc = sal_strrchr(name, '_');
    if (loc != NULL) {
        *loc = '\0';
        if (sal_config_search_valid_prop(name)) {
            return TRUE;
        }
    }

    /* 
     * try removing one more underscore if it exists, for "prop_in_203.bcm88650" 
     * style 
     */
    loc = sal_strrchr(name, '_');
    if (loc != NULL) {
        *loc = '\0';
        if (sal_config_search_valid_prop(name)) {
            return TRUE;
        }
    }

    /* 
     * try removing one more underscore if it exists, for "prop_dram0_byte0_bit0.BCM88675"
     * style 
     */

    loc = sal_strrchr(name, '_');
    if (loc != NULL) {
        *loc = '\0';
        if (sal_config_search_valid_prop(name)) {
            return TRUE;
        }
    }

    return FALSE;
}
#endif

#ifndef SAL_CONFIG_FILE_DISABLE
#define SAL_CONFIG_MAX_IMPORTED_CONFIG_FILES  1024

/**
 * 
 * 
 * 
 * @param import_fname 
 * @param nof_imported_config_files 
 * @param already_imported_tables 
 * check if file already 
 * imported 
 * 
 * @return int 
 */
static int
 sal_config_already_imported(char *import_fname,int *nof_imported_config_files, char *already_imported_tables[])
{
    int i;

    /*
     *  the function save the file names to keep we are not enter
     *  into infinite loop of imports
     *  the saved name is
     *  1.basename(config.bcm) - first time
     *  2. the argument to import command
     *  
     */
    if (SAL_CONFIG_MAX_IMPORTED_CONFIG_FILES<=*nof_imported_config_files) {
        sal_printf("nof imported files exceeded its limit: %d:\n ",SAL_CONFIG_MAX_IMPORTED_CONFIG_FILES);
        return 1;
    }
    for (i=0;i<*nof_imported_config_files;i++) {
        if (!sal_strcmp(import_fname, already_imported_tables[i])) {
#if defined(BROADCOM_DEBUG)
            sal_printf("sal_config_already_imported: file: %s, "
                   "is already imported\n",
                   import_fname);
#endif /* BROADCOM_DEBUG */
            return 1;
        }
    }
    already_imported_tables[i] = sal_alloc(sal_strlen(import_fname)+1," alloc bcm file name");
    sal_strcpy(already_imported_tables[i],import_fname);
    (*nof_imported_config_files)++;
    return 0;
}

/**
 * import config file 
 * to one config file 
 *  
 */

static int 
sal_config_line_import(char *str,char *import_fname) {
    char  line[SAL_CONFIG_STR_MAX];
    char *sep=" \t\n\r";
    char *token;
    /*
     * we check the case if the 
     * line of the form 
     * import <filename> 
    */


    if ((sal_strlen(str)) < SAL_CONFIG_STR_MAX) {
        sal_strcpy(line,str);
    } else {
        return 0;
    }
    /*
     * split the line into 2 tokens
     */
    token=sal_strtok(line,sep);
    if (token==NULL) {
        return 0;
    }

    /*
     * if the first token is import 
     * then we look for the second token to be the file name 
     */

    if (sal_strcmp(token,"import")) {
        return 0;
    }
    token = sal_strtok(NULL, sep);
    if (token==NULL) {
        return 0;
    }

    /*
     * we build the import file name as 
     * dirname/filename contained at the second token 
     *  
     */
    sal_strcpy(import_fname, token); 

    return 1;
}

/**
 * 
 * 
 * 
 * Function:
 *  sal_config_file_process
 * Purpose:
 *      parse single config-bcm file .
 *      if find import statement in the parsed file
 *      call recurssivley itself with the import file name
 * @param fname 
 * @param nof_imported_config_files 
 * @param already_imported_tables 
 * 
 * @return int 
 */
static int
sal_config_file_process(char *fname,int *nof_imported_config_files, char *already_imported_tables[], char *dirname)
{
    FILE  *fp;
    sc_t  *found_sc;
    char  str[SAL_CONFIG_STR_MAX], *c;
    int   line = 0;
    int   suppress_unknown_warnings = 0;
    char import_fname[SAL_NAME_MAX];
    char fname_full_path[SAL_NAME_MAX];
    sc_t  *sc;


    if (dirname!=NULL && sal_strcasecmp(dirname,".")) {
        sal_sprintf(fname_full_path, "%s/%s", dirname, fname); 
    } else {
        sal_strncpy(fname_full_path, fname,sizeof(fname_full_path)-1); 
    }
    if ((fp = sal_fopen(fname_full_path, "r")) == NULL) {
#if defined(BROADCOM_DEBUG)
        sal_printf("sal_config_refresh: cannot read file: %s, "
               "variables not loaded\n",
               fname_full_path);
#endif /* BROADCOM_DEBUG */
        return -1;
    }

    /* Read the entire file  - parsing as we go */

    while (sal_fgets(str, sizeof(str), fp)) {

        line++;

        /* Skip comment lines */

        if (str[0] == '#') {
            continue;
        }
        if (sal_config_line_import(str,import_fname)) {
            if (sal_config_already_imported(import_fname, nof_imported_config_files,already_imported_tables)) {
                sal_printf("%s already imported\n",import_fname);
                continue;
            }
            if (sal_config_file_process(import_fname,  nof_imported_config_files,already_imported_tables,dirname) == -1) {
                sal_fclose(fp);
                return -1;
            }
            continue;
        }

        /* Strip trailing newline/blanks */

        c = str + strlen(str);
        while (c > str && isspace((unsigned) c[-1])) {
            c--;
        }

        *c = '\0';

        /* Skip blank lines */

        if (str[0] == 0) {
            continue;
        }

        if ((sc = sal_config_parse(str)) == NULL) {
            sal_printf("sal_config_refresh: format error "
                 "in %s on line %d (ignored)\n",
                 fname, line);
            continue;
        }

        /* Check for pre-existing default or duplicates in the file */        
        found_sc = sal_config_list[sc->sc_hash];
        while (found_sc != NULL) {
            if (sal_strcmp(found_sc->sc_name, sc->sc_name) == 0) {
                break;
            }
            found_sc = found_sc->sc_next;
        }

        if (found_sc != NULL) {
            if (!(found_sc->sc_flag & SC_COMPILE_VALUE)) {
                sal_printf("sal_config_refresh: ignoring duplicate entry "
               "\"%s\"\n"
               "                    %s line %d "
               "(first defined on line %d)\n",
               sc->sc_name, fname,
               line, found_sc->sc_line);
            }
            else {
                /* Clobber the compiled-in default value */
                char *temp = sc->sc_value;         /* New value */
                sc->sc_value = found_sc->sc_value; /* Old, to be freed */
                found_sc->sc_value = temp;
                found_sc->sc_line = line; /* Line # of 1st definition */
            }
            FREE_SC(sc);
            continue;
        }

        /* 
         * Scan for "suppress_unknown_prop_warnings" directly as we go
         * instead of sal_config_find() because it's much faster
         */
        if (sal_strcasecmp("suppress_unknown_prop_warnings", 
                           sc->sc_name) == 0) {
            suppress_unknown_warnings = _shr_ctoi(sc->sc_value);
        }

        if (!suppress_unknown_warnings) {
            if (sal_config_prop_is_known(sc) == FALSE) {
                sal_printf("sal_config_refresh: unknown entry \"%s\""
                           " on %s line %d\n", sc->sc_name, fname, line);
            }
        }

        sc->sc_line = line; /* Line # of 1st definition */
        sc->sc_next = sal_config_list[sc->sc_hash];
        sal_config_list[sc->sc_hash] = sc;  
        sal_config_imported_values++;
    } /* parse config file */
    sal_fclose(fp);
    return 0;
}
#endif
/*
 * Function:
 *  sal_config_refresh
 * Purpose:
 *      Refresh default (compiled-in) configuration.
 *  Refresh the memory image from the configuration file,
 *      clobbering default values (if the config file exists).
 * Parameters:
 *  None
 * Returns:
 *  0 - success
 *  -1 - failed.
 */
int
sal_config_refresh(void)
{
    sc_t  *sc, *sc_tmp;
    int i;

#ifndef SAL_CONFIG_FILE_DISABLE
    char *fname;
    int  nof_imported_config_files=1;
    char  *already_imported_tables[SAL_CONFIG_MAX_IMPORTED_CONFIG_FILES]={NULL};
    int rv;
    char dirname[SAL_NAME_MAX];


    fname = (sal_config_file_name != NULL ?
    sal_config_file_name : SAL_CONFIG_FILE);

    if (fname[0] == 0) {
        return 0;   /* No config file */
    }

    /* Try to load config file ... */
#endif

    /* Clear all previous state */
    for (i = 0; i < MAX_CONFIG_HASH_COUNT; i++) {
        sc = sal_config_list[i];
        sal_config_list[i] = NULL;
        while (sc != NULL) {
            sc_tmp = sc->sc_next;
            FREE_SC(sc);
            sc = sc_tmp;
        }
    }
    /* load precompiled values from platform_defines.c */
    sal_config_init_defaults();

#ifndef SAL_CONFIG_FILE_DISABLE
    /*
     * Add new file-based variables, superseding any matching compiled
     * variables.  Find end-of-list, and initialize the default-or-file
     * flag.
     */
    sal_config_compiled_values = 0;
    sal_config_imported_values = 0;

    for (i = 0; i < MAX_CONFIG_HASH_COUNT; i++) {
        for (sc = sal_config_list[i]; sc != NULL; sc = sc->sc_next) {
            sc->sc_flag |= SC_COMPILE_VALUE;
            sal_config_compiled_values++;
        }
    }
    sal_dirname(fname,dirname);
    already_imported_tables[0] = sal_alloc(sal_strlen(fname)+1 ," alloc bcm file name");
    sal_basename(fname, already_imported_tables[0]);
    rv =  sal_config_file_process(already_imported_tables[0],&nof_imported_config_files,already_imported_tables, dirname);

    for (i=0;i<nof_imported_config_files;i++) {
        sal_free(already_imported_tables[i]);
    }

    return rv;

#endif /* SAL_CONFIG_FILE_DISABLE */

    return 0;
}

/*
 * Function:
 *  sal_config_flush
 * Purpose:
 *  Flush the memory image to the configuration file.
 * Parameters:
 *  None
 * Returns:
 *  0 - success
 *  -1 - failed.
 */

int
sal_config_flush(void)
{
    int   rv = 0;
#ifndef SAL_CONFIG_FILE_DISABLE
    FILE  *old_fp, *new_fp;
    sc_t  *sc, *new_sc;
    char  str[SAL_CONFIG_STR_MAX], *c;
    int   line = 0;
    char  *fname, *tname;
    int i;

    fname = (sal_config_file_name != NULL ?
       sal_config_file_name : SAL_CONFIG_FILE);

    if (fname[0] == 0) {
        return 0; /* No config file */
    }

    tname = (sal_config_temp_name != NULL ?
       sal_config_temp_name : SAL_CONFIG_TEMP);

    assert(tname != NULL && tname[0] != 0);

    /* Attempt to create temp file */

    if ((new_fp = sal_fopen(tname, "w")) == NULL) {
        rv = -1;
        goto done;
    }

    /* old_fp can be NULL if creating config file for first time */

    old_fp = sal_fopen(fname, "r");

    /* Initialize the "flushed" flag */
    for (i = 0; i < MAX_CONFIG_HASH_COUNT; i++) {
        for (sc = sal_config_list[i]; sc != NULL; sc = sc->sc_next) {
            sc->sc_flag &= ~SC_FLUSHED;
        }
    }

    /* Read the entire file  - parsing as we go */

    while (old_fp != NULL && sal_fgets(str, sizeof(str), old_fp)) {
        char *s;
      
        line++;
      
        /* Strip trailing newline/blanks */
      
        c = str + strlen(str);
        while (c > str && isspace((unsigned) c[-1])) {
            c--;
        }
      
        *c = '\0';
      
        /*
         * Transfer blank lines and comment lines, but NOT commented-out
         * variable settings (yet)
         */
      
        if (str[0] == 0 || (str[0] == '#' && strchr(str, '=') == NULL)) {
            sal_fprintf(new_fp, "%s\n", str);
            continue;
        }
      
        s = str;
        if (*s == '#') {
            s++;
        }
      
        if ((sc = sal_config_parse(s)) == NULL) {
            sal_printf("sal_config_flush: format error "
                 "in %s on line %d (removed)\n",
                 fname, line);
            continue;
        }
      
        /* Write new value (or comment) for this entry */
      
        if ((new_sc = sal_config_find(sc->sc_name)) == NULL ||
            (new_sc->sc_flag & SC_FLUSHED)) {
            /* Not found or a dup, write out commented assignment */
            sal_fprintf(new_fp, "#%s=%s\n", sc->sc_name, sc->sc_value);
        } else {
            sal_fprintf(new_fp, "%s=%s\n", new_sc->sc_name, new_sc->sc_value);
            new_sc->sc_flag |= SC_FLUSHED;
        }
      
        FREE_SC(sc);
    }

    /* Write out the current values that were not in the old_fp file */
    for (i = 0; i < MAX_CONFIG_HASH_COUNT; i++) {
        for (sc = sal_config_list[i]; sc != NULL; sc = sc->sc_next) {
            if (!(sc->sc_flag & SC_FLUSHED)) {
                sal_fprintf(new_fp, "%s=%s\n", sc->sc_name, sc->sc_value);
            }
        }
    }

    sal_fclose(new_fp);

    if (old_fp != NULL) {
        sal_fclose(old_fp);
    }

    if (rv == 0) {      /* No error, rename file */
        rv = sal_rename(tname, fname);
    }

    if (rv != 0) {      /* Error, remove temp file */
        (void)sal_remove(tname);
    }

 done:
    if (rv < 0) {
        sal_printf("sal_config_flush: variables not saved\n");
    }
#endif /* SAL_CONFIG_FILE_DISABLE */

    return rv;
}

/*
 * Function:
 *  sal_config_save
 * Purpose:
 *  Flush the config to the a given file.
 * Parameters:
 *  None
 * Returns:
 *  0 - success
 *  -1 - failed.
 */

int
sal_config_save(char *fname, char *pat, int append)
{
    int   rv = 0;
#ifndef SAL_CONFIG_FILE_DISABLE
    FILE  *new_fp;
    sc_t  *sc;
    int   pat_len = 0;
    int   i;

    if (fname == NULL) {
        return 0; /* No config file */
    }

    if(append) {
        new_fp = sal_fopen(fname, "a");
    } else {
        new_fp = sal_fopen(fname, "w");
    }
    
    if (new_fp == NULL) {
        rv = -1;
        goto done;
    }

    if (pat) {
        pat_len = strlen(pat);
    }
    /* Write out the current values */
    for (i = 0; i < MAX_CONFIG_HASH_COUNT; i++) {
        for (sc = sal_config_list[i]; sc != NULL; sc = sc->sc_next) {
            if ((pat == NULL) || (sal_strncasecmp(sc->sc_name, pat, pat_len) == 0)) {
                sal_fprintf(new_fp, "%s=%s\n", sc->sc_name, sc->sc_value);
            }
        }
    }

    sal_fclose(new_fp);

 done:
    if (rv < 0) {
        sal_printf("sal_config_flush: variables not saved\n");
    }
#endif /* SAL_CONFIG_FILE_DISABLE */

    return rv;
}

/*
 * Function:
 *  sal_config_get
 * Purpose:
 *  Recover a sal configuration variable
 * Parameters:
 *  name - name of variable to recover
 * Returns:
 *  NULL - not found
 *  !NULL - pointer to value
 */

char *
sal_config_get(const char *name)
{
    sc_t  *sc;

    sc = sal_config_find(name);
    if (sc != NULL) {
        sc->sc_flag |= SC_HIT;
        return sc->sc_value;
    } else {
        return NULL;
    }
}

/*
 * Function:
 *  sal_config_get_next
 * Purpose:
 *  Recover a sal configuration variables in order.
 * Parameters:
 *  name - (IN/OUT) name of last variable recovered.
 *  value- (OUT) value of variable recovered.
 * Returns:
 *  0 - variable recovered.
 *  -1 - All complete, not more variables.
 */

int
sal_config_get_next(char **name, char **value)
{
    sc_t  *sc;
    sc_hash_t hash;
    char * nm = *name;

    if (nm) {
        SC_HASH(nm, hash);
        sc = sal_config_list[hash];
        while (sc != NULL) {
            if (sal_strcmp(sc->sc_name, nm) == 0) {
                break;
            }
            sc = sc->sc_next;
        } 
        if (sc != NULL) {
            sc = sc->sc_next;
            if (sc == NULL) {
                hash++;
                for (;hash < MAX_CONFIG_HASH_COUNT; hash++) {
                    sc = sal_config_list[hash];
                    if (sc != NULL) {
                        break;
                    }
                }
            }
        }
        
    } else {
        for (hash = 0; hash < MAX_CONFIG_HASH_COUNT; hash++) {
            sc = sal_config_list[hash];
            if (sc != NULL) {
                break;  
            }
        }
    }
    
    if (sc) {     
        *name = sc->sc_name;
        *value = sc->sc_value;
        return 0;
    } else {
        return -1;
    }
}

/*
 * Function:
 *  sal_config_set
 * Purpose:
 *  Set a sal configuration variable
 * Parameters:
 *  name - name of variable to set
 *  value - name of value; can be NULL to delete a variable
 * Returns:
 *  0 - found and changed
 *  -1 - not found or out of memory
 */

int
sal_config_set(char *name, char *value)
{
    sc_t  *sc, *psc;
    char  *newval;
    char    *wildcard = NULL;
    char    *sc_wildcard;
    int     length;
    sc_hash_t hash;

    if (name == NULL || *name == '\0') {
        return -1;
    }

    SC_HASH(name, hash);
    sc = sal_config_list[hash];
    psc = NULL;
    while (sc != NULL) {
        if (sal_strcmp(sc->sc_name, name) == 0) {
            break;
        }
        psc = sc;
        sc = sc->sc_next;
    }
    
    if (sc != NULL) {   /* found */
        if (value == NULL) {  /* delete */
            if (sal_config_list[hash] == sc)  {
                sal_config_list[hash] = sc->sc_next;
            } else {
                if (psc !=NULL) {
                    psc->sc_next = sc->sc_next;
                } 
            }
            FREE_SC(sc);
            return 0;
        } else {    /* replace */
            newval = sal_alloc(strlen(value) + 1, "config value");
            if (newval == NULL) {
                return -1;
            }
            sc->sc_flag = 0;
            sal_strncpy(newval, value, strlen(value));
            newval[strlen(value)] = '\0';
            sal_free(sc->sc_value);
            sc->sc_value = newval;
            return 0;
        }
    }

    /* not found, delete */
    if (value == NULL) {
        int i;
        wildcard = wildcard_search(name, wildcard, &length);
        if (wildcard != NULL) {
            sc_wildcard = sal_alloc((length + 1), "sc_wildcard");
            *(sc_wildcard+length) = 0;
            for (i = 0; i < MAX_CONFIG_HASH_COUNT; i++) {
                sc = sal_config_list[i];
                psc = NULL;
                while (sc != NULL){
                    sc_wildcard = sal_strncpy(sc_wildcard, sc->sc_name, length);
                    sc_wildcard[length] = '\0';
                    if (sal_strcmp(sc_wildcard, wildcard) == 0) {
                        if (sal_config_list[i] == sc) {
                            sal_config_list[i] = sc->sc_next;
                            FREE_SC(sc);
                            sc = sal_config_list[i]; 
                            psc = NULL;  
                        } else {
                            if (psc !=NULL) {
                                psc->sc_next = sc->sc_next;
                            } 
                            FREE_SC(sc);
                            if (psc !=NULL) {
                                sc = psc->sc_next;
                            }
                        }
                    } else {
                        psc = sc;
                        sc = sc->sc_next;
                    }
                }
            }
            sal_free(wildcard);
            sal_free(sc_wildcard);
            return 0;
        } else {
            return -1;
        }
    }

    

    /* not found, add */
    if ((sc = sal_alloc(sizeof(sc_t), "config set")) == NULL) {
        return -1;
    }
    sc->sc_flag = 0;
    sc->sc_line = 0;

    sc->sc_name = sal_alloc(strlen(name) + 1, "config name");
    sc->sc_value = sal_alloc(strlen(value) + 1, "config value");

    if (sc->sc_name == NULL || sc->sc_value == NULL) {
        FREE_SC(sc);
        return -1;
    }

    sal_strncpy(sc->sc_name, name, strlen(name));
    sc->sc_name[strlen(name)] = '\0';
    sal_strncpy(sc->sc_value, value, strlen(value));
    sc->sc_value[strlen(value)] = '\0';
    sc->sc_hash = hash;

    sc->sc_next = sal_config_list[hash];
    sal_config_list[hash] = sc;

    return 0;
}

/*
 * Function:
 *  sal_config_show
 * Purpose:
 *  Display current configuration variables.
 * Parameters:
 *  none
 * Returns:
 *  Nothing
 */

void
sal_config_show(void)
{
    sc_t  *sc;
    int i;

    for (i = 0; i < MAX_CONFIG_HASH_COUNT; i++) {
        for (sc = sal_config_list[i]; sc != NULL; sc = sc->sc_next) {
            sal_printf("\t%s=%s\n", sc->sc_name, sc->sc_value);
        }
    }
}

void
sal_config_dump(void)
{
    sc_t  *sc;
    int i;
    const char *fname, *tname;

    for (i = 0; i < MAX_CONFIG_HASH_COUNT; i++) {
        for (sc = sal_config_list[i]; sc != NULL; sc = sc->sc_next) {
            sal_printf("hash:%4d, line:%4d, flag: 0x%04x, %s=%s\n",
                       i,
                       sc->sc_line,
                       sc->sc_flag,
                       sc->sc_name,
                       sc->sc_value);
        }
    }

    sal_config_file_get(&fname, &tname);
    sal_printf("\nconfig file:\n\t%s\n", fname);

    sal_printf("\nNumber of compiled values:\n\t%4d\n",
                sal_config_compiled_values);

    sal_printf("\nNumber of imported values from %s:\n\t%4d\n",
                fname,
                sal_config_imported_values);

    sal_printf("\nflags:\n");
    sal_printf("\t#define SC_COMPILE_VALUE 0x%04x\n",SC_COMPILE_VALUE);
    sal_printf("\t#define SC_FLUSHED       0x%04x\n",SC_FLUSHED);
    sal_printf("\t#define SC_HIT           0x%04x\n",SC_HIT);


}
