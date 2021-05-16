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
    DUPLICATE_ENTRY_ACTION_USE_FIRST        = 0,
    DUPLICATE_ENTRY_ACTION_USE_SECOND       = 1,
    DUPLICATE_ENTRY_ACTION_APPEND_FIRST     = 2,
    DUPLICATE_ENTRY_ACTION_APPEND_SECOND    = 3
} DuplicateEntryAction;


/// User input action specifier 
typedef enum UserInputAction {
    USER_INPUT_ACTION_SHOW_HELP     = 0,
    USER_INPUT_ACTION_LIST_PLANTS   = 1,
    USER_INPUT_ACTION_SELECT_PLANT
};


#ifdef __PROMPT_C
    #include <stdlib.h>
    #include <stdbool.h>
    #include <stdio.h>
    #include <stdint.h>

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
    DuplicateEntryAction __promptDuplicateAction();


    /// Unselected mode help text
    static const char *__help_unselected = 
        "energy_manager(unselected mode) usage: \n"\
        "help -- show usage info\n"\
        "list -- show all available power plants\n"\
        "log -- show all available logs\n"\
        "edit [ID] -- edit power plant values\n"\
        "delete [ID] -- delete power plant from the list\n"\
        "select [ID] -- select a power plant for usage\n"\
        "exit -- exit the program\n";

    /// Selected mode help text
    static const char *__help_selected =
        "energy_manager(selected mode) usage: \n"\
        "help -- show usage info\n"\
        "list -- show all logs for that power plant\n"\
        "edit [ID] -- edit log values\n"\
        "delete [ID] -- delete log\n"\
        "exit -- exit selected mode\n";

#endif


/// Prompt the user about possible actions that can be taken, when
/// duplicate power plant entries were found
DuplicateEntryAction promptDuplicatePowerPlantEntries(PlantData *ent1, PlantData *ent2);


/// Prompt the user about possible actions that can be taken, when
/// duplicate log entries were found
DuplicateEntryAction promptDuplicateLogEntries(LogEntry *ent1, LogEntry *ent2);


/// Handle duplicate entry values according the specified duplicate handling action
void handleDuplicatesEntries(DuplicateEntryAction action, PowerPlants *p_plants, 
    PlantData *dup1, PlantData *dup2);


#endif
