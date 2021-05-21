/*
 * File:        act_impl.h
 * Author:      Karl-Mihkel Ott
 * Created      2021-05-19
 * Last edit:   2021-05-19
 * Description: Contains function declarations to user command 
 *              action implementations
 */


#ifndef __ACT_IMPL_H
#define __ACT_IMPL_H


#ifdef __ACT_IMPL_C
    #include <stdio.h>
    #include <stdint.h>
    #include <string.h>
    #include <stdlib.h>
    #include <sys/ioctl.h>
    #include <unistd.h>
    #include <stdbool.h>
    
    #include <hashmap.h>
    #include <err_def.h>
    #include <mem_check.h>
    #include <ncurses.h>
    #include <entity_data.h>
    #include <prompt.h>
    #include <energy_manager.h>

    /// Unselected mode help text
    static const char *__help_unselected = 
        "energy_manager(unselected mode) usage: \n"\
        "help -- show usage info\n"\
        "new -- create a new power plant entry\n"\
        "list -- show all available power plants\n"\
        "log -- show all available logs\n"\
        "edit [ID] -- edit power plant values\n"\
        "delete [ID] -- delete power plant from the list\n"\
        "select [ID] -- select a power plant for usage\n"\
        "save -- save the data into correct files\n"\
        "exit -- exit the program\n";

    /// Selected mode help text
    static const char *__help_selected =
        "energy_manager(selected mode) usage: \n"\
        "help -- show usage info\n"\
        "new -- create a new log entry\n"\
        "list -- show all logs for that power plant\n"\
        "edit [ID] -- edit log values\n"\
        "delete [ID] -- delete log\n"\
        "save -- save the data into correct files\n"\
        "exit -- exit selected mode\n";


    #define __MAX_LOG_LINE                  256
    #define __MAX_PLANT_FILE_LINE(max_name) __roundToBase2(82 + max_name)
#endif

/// Print out help text
void showHelp(bool is_sel);


/// Ask information about the new power plant instance from the user
/// and create a new instance
void createNewPowerPlant(PowerPlants *p_plants, Hashmap *p_map);


/// List all currently available power plants according to
/// the average utilisation percentage
void listPowerPlants(PowerPlants *p_plants);


/// List all written logs in date order
void listAllLogs(PlantLogs *p_logs);


/// List all logs that belong to the power plant
void listPowePlantLogs(PlantData *plant);


/// Edit power plant properties
void editPowerPlant(Hashmap *plant_map, uint32_t index);


/// Edit the power plant log data
void editLog(Hashmap *log_map, uint32_t index);


/// Delete a power plant entry
void deletePowerPlant(PowerPlants *p_plants, Hashmap *plant_map, uint32_t index);


/// Delete a log entry
void deleteLog(PlantLogs *p_logs, Hashmap *log_map, uint32_t index);


/// Save all edited data into a file
void saveData(PowerPlants *p_plants, PlantLogs *p_logs, char *plants_file,
    char *logs_file);
#endif
