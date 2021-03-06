/*
 * File:        prompt.h
 * Author:      Karl-Mihkel Ott
 * Created      2021-05-13
 * Last edit:   2021-05-16
 * Description: Collection of user interaction functions for specific situations
 */


#ifndef __PROMPT_H
#define __PROMPT_H


/// Duplicate power plant entry action specifier
typedef enum DuplicateEntryAction {
    DUPLICATE_ENTRY_ACTION_USE_MAPPED       = 0,
    DUPLICATE_ENTRY_ACTION_USE_UNMAPPED     = 1,
    DUPLICATE_ENTRY_ACTION_APPEND_MAPPED    = 2,
    DUPLICATE_ENTRY_ACTION_APPEND_UNMAPPED  = 3
} DuplicateEntryAction;


/// User input action specifier 
typedef enum UserInputAction {
    USER_INPUT_ACTION_UNKNOWN                   = 0,
    USER_INPUT_ACTION_U_SHOW_HELP               = 1,
    USER_INPUT_ACTION_U_NEW_POWER_PLANT         = 2,
    USER_INPUT_ACTION_U_LIST_PLANTS             = 3,
    USER_INPUT_ACTION_U_LIST_LOGS               = 4,
    USER_INPUT_ACTION_U_EDIT_POWER_PLANT        = 5,
    USER_INPUT_ACTION_U_DELETE_POWER_PLANT      = 6,
    USER_INPUT_ACTION_U_SELECT_POWER_PLANT      = 7,
    USER_INPUT_ACTION_S_SHOW_HELP               = 8,
    USER_INPUT_ACTION_S_NEW_LOG                 = 9,
    USER_INPUT_ACTION_S_LIST_LOGS               = 10,
    USER_INPUT_ACTION_S_EDIT_LOG                = 11,
    USER_INPUT_ACTION_S_DELETE_LOG              = 12,
    USER_INPUT_ACTION_S_UNSEL_POWER_PLANT       = 13,
    USER_INPUT_ACTION_EXIT                      = 14,
    USER_INPUT_ACTION_SAVE                      = 15,
    USER_INPUT_ACTION_ENUM_C                    = 16
} UserInputAction;


#ifdef __PROMPT_C
    #include <stdlib.h>
    #include <stdbool.h>
    #include <stdio.h>
    #include <stdint.h>
    #include <time.h>
    #include <unistd.h>
    #include <sys/ioctl.h>
    #include <string.h>

    #include <hashmap.h>
    #include <entity_data.h>
    #include <algo.h>
    #include <act_impl.h>
    #include <mem_check.h>

    /// Display the power plant entry data
    static void __displayPowerPlantEntry(PlantData *p_data);


    /// Display log entry data, along with the name and id of the power plant it belongs to
    static void __displayLogEntry(LogEntry *p_entry);

    
    /// Prompt user to solve duplicate instances
    static DuplicateEntryAction __promptDuplicateAction();


    /// Convert string command into command enumeral
    static UserInputAction __convertStrInputToEnum(Hashmap *tokens, char *in_str, bool is_sel, size_t len);


    /// Specifiers for tokens to identify their mode
    #define UNSEL_SPECIFIER     "_u"
    #define SEL_SPECIFIER       "_s"


    /// Token definitions
    typedef struct __TokenDef {
        char *token;
        UserInputAction act;
    } __TokenDef;


    /// All possible command tokens
    #define __MAX_TOKEN_LEN     16
    static const __TokenDef __cmd_tokens[] = {
        // Unselected mode tokens
        { "help"        UNSEL_SPECIFIER,    USER_INPUT_ACTION_U_SHOW_HELP },
        { "list"        UNSEL_SPECIFIER,    USER_INPUT_ACTION_U_LIST_PLANTS },
        { "new"         UNSEL_SPECIFIER,    USER_INPUT_ACTION_U_NEW_POWER_PLANT },
        { "log"         UNSEL_SPECIFIER,    USER_INPUT_ACTION_U_LIST_LOGS },
        { "edit"        UNSEL_SPECIFIER,    USER_INPUT_ACTION_U_EDIT_POWER_PLANT },
        { "delete"      UNSEL_SPECIFIER,    USER_INPUT_ACTION_U_DELETE_POWER_PLANT },
        { "select"      UNSEL_SPECIFIER,    USER_INPUT_ACTION_U_SELECT_POWER_PLANT },

        // Selected mode tokens
        { "help"        SEL_SPECIFIER,      USER_INPUT_ACTION_S_SHOW_HELP },
        { "list"        SEL_SPECIFIER,      USER_INPUT_ACTION_S_LIST_LOGS },
        { "new"         SEL_SPECIFIER,      USER_INPUT_ACTION_S_NEW_LOG },
        { "edit"        SEL_SPECIFIER,      USER_INPUT_ACTION_S_EDIT_LOG },
        { "delete"      SEL_SPECIFIER,      USER_INPUT_ACTION_S_DELETE_LOG },
        { "unsel"       SEL_SPECIFIER,      USER_INPUT_ACTION_S_UNSEL_POWER_PLANT },

        // General purpose commands
        { "save",              USER_INPUT_ACTION_SAVE },
        { "exit",              USER_INPUT_ACTION_EXIT }
    };


    /// Structure for specifying all differenct sorting modes
    typedef struct __SortDef {
        char *str_mode;
        ListSortMode sort_modes[2];
    } __SortDef;


    /// Unselected sort mode specifiers
    static const __SortDef __unsel_sort_modes[] = {
        /* string keyword   increasing ListSortMode specifier       decreasing ListSort mode specifier */
        { "plant_id",       { LIST_SORT_MODE_POW_ID_INCR,           LIST_SORT_MODE_POW_ID_DECR } },
        { "rated_cap",      { LIST_SORT_MODE_POW_CAP_INCR,          LIST_SORT_MODE_POW_CAP_DECR } },
        { "avg_price",      { LIST_SORT_MODE_POW_COST_INCR,         LIST_SORT_MODE_POW_COST_DECR } },
        { "avg_util",       { LIST_SORT_MODE_POW_UTIL_INCR,         LIST_SORT_MODE_POW_UTIL_DECR } },
    };
    
    /// Selected mode sort mode specifiers
    static const __SortDef __sel_sort_modes[] = {
        /* string keyword   increasing ListSortMode specifier       decreasing ListSort mode specifier */
        { "log_id",         { LIST_SORT_MODE_LOG_ID_INCR,           LIST_SORT_MODE_LOG_ID_DECR } },
        { "plant_id",       { LIST_SORT_MODE_LOG_PLANT_ID_INCR,     LIST_SORT_MODE_LOG_PLANT_ID_DECR } },
        { "production",     { LIST_SORT_MODE_LOG_PRODUCTION_INCR,   LIST_SORT_MODE_LOG_PRODUCTION_DECR } },
        { "price",          { LIST_SORT_MODE_LOG_SALE_PRICE_INCR,   LIST_SORT_MODE_LOG_SALE_PRICE_DECR } },
        { "date",           { LIST_SORT_MODE_LOG_DATE_INCR,         LIST_SORT_MODE_LOG_DATE_DECR } } 
    };

    #define __DEFAULT_BUF_SIZE          4096
    #define __DEFAULT_SMALL_BUF_SIZE    64
    #define __DEFAULT_NAME_LEN          1024

    /// Create a table separator, that is termwidth wide and the string 
    /// consists of underscores 
    char *__mkTableSeparator();


    /********** New powerplant creation prompts *********/

    /// Prompt the user about new power plant fuel type
    FuelType __promptNewPowerPlantFuelType(FuelType *old);


    /// Prompt the user about new power plant name
    char *__promptNewPowerPlantNameValue(char *old);


    /********** New log creation prompts **********/

    /// Prompt the user about log's date info
    Date __promptNewLogDate(Date *old);


    /********** Generic prompts ***********/

    /// Prompt the user until he enters correct floating point value
    float __promptFloatValue(char *msg, float *old);
    

    /// Prompt the user until he enters correct id
    uint32_t __promptIdValue(char *msg, size_t *p_max_id, Hashmap *p_map);
#endif


/******** Data display functions *********/

/// Display log data for each given log
void displayLogData(PlantLogRefs *p_refs);


/// Display power plant data for each given power plant
void displayPowerPlants(PowerPlantRefs *p_refs);


/// Format date from Date structure into string yyyy-mm-dd format
/// NOTE: This function returns a pointer to stack allocated memory area,
/// which gets overwritten with every function call
char *formatDate(Date *p_date);


/// Prompt the user about possible actions that can be taken, when
/// duplicate power plant entries were found
DuplicateEntryAction promptDuplicatePowerPlantEntries(PlantData *ent1, PlantData *ent2);


/// Prompt the user about possible actions that can be taken, when
/// duplicate log entries were found
DuplicateEntryAction promptDuplicateLogEntries(LogEntry *ent1, LogEntry *ent2);


/// Create a token hashmap for all possible user input data
Hashmap tokeniseUserInput();


/// Parse the user entry into enumeral
UserInputAction parseUserInputAction(Hashmap *tokens, char *in_str, bool is_sel, 
    ListSortMode *p_sort, uint32_t *out_arg);


/// Prompt the user for information about a new power plant instance
PlantData promptNewPowerPlant(size_t *p_max_id, Hashmap *p_map);


/// Prompt the user for information about editing a power plant instance
void promptEditPowerPlant(PlantData *data);


/// Prompt the user for information about editing a log instance
void promptEditLog(LogEntry *log);


/// Prompt the user to create a new log entry
LogEntry promptNewLogEntry(Hashmap *p_map, size_t *p_max_id, uint32_t sel_id);
#endif
