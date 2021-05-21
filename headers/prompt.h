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
    USER_INPUT_ACTION_EXIT                      = 13,
    USER_INPUT_ACTION_SAVE                      = 14,
    USER_INPUT_ACTION_ENUM_C                    = 15
} UserInputAction;


#ifdef __PROMPT_C
    #include <stdlib.h>
    #include <stdbool.h>
    #include <stdio.h>
    #include <stdint.h>
    #include <string.h>

    #include <hashmap.h>
    #include <mem_check.h>
    #include <entity_data.h>

    /// Display the power plant entry data
    static void __displayPowerPlantEntry(PlantData *p_data);


    /// Display log entry data, along with the name and id of the power plant it belongs to
    static void __displayLogEntry(LogEntry *p_entry);


    /// Format date from Date structure into string yyyy-mm-dd format
    /// NOTE: This function returns a pointer to stack allocated memory area,
    /// which gets overwritten with every function call
    static char *__formatDate(Date *p_date);

    
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
    #define SEL_SPECIFIER       "_s"

    /// Mode independent commands
    #define SAVE                "save"
    #define EXIT                "exit"

    #define __DEFAULT_BUF_LEN       256
#endif


/// Prompt the user about possible actions that can be taken, when
/// duplicate power plant entries were found
DuplicateEntryAction promptDuplicatePowerPlantEntries(PlantData *ent1, PlantData *ent2);


/// Prompt the user about possible actions that can be taken, when
/// duplicate log entries were found
DuplicateEntryAction promptDuplicateLogEntries(LogEntry *ent1, LogEntry *ent2);


/// Create a token hashmap for all possible user input data
Hashmap tokeniseUserInput();


/// Parse the user entry into enumeral
UserInputAction parseUserInputAction(Hashmap *tokens, char *in_str, bool is_sel, uint32_t *out_arg);
#endif
