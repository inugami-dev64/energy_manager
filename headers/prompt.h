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
    USER_INPUT_ACTION_U_SHOW_LOGS               = 4,
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


    /// Token definitions
    /// UNSEL means unselected mode token and SEL means selected mode token
    #define UNSEL_HELP          "help"
    #define UNSEL_LIST          "list"
    #define UNSEL_NEW           "new"
    #define UNSEL_LOG           "log"
    #define UNSEL_EDIT          "edit"
    #define UNSEL_DEL           "delete"
    #define UNSEL_SEL           "select"
    #define UNSEL_SPECIFIER     "_u"

    #define SEL_HELP            "help"
    #define SEL_NEW             "new"
    #define SEL_LIST            "list"
    #define SEL_EDIT            "edit"
    #define SEL_DEL             "delete"
    #define SEL_UNSEL           "unsel"
    #define SEL_SPECIFIER       "_s"

    /// Mode independent commands
    #define SAVE                "save"
    #define EXIT                "exit"

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
