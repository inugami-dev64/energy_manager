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


/// Enumeral values to specify data sorting
typedef enum ListSortMode {
    LIST_SORT_MODE_UNKNOWN                  = 0,
    LIST_SORT_MODE_POW_UTIL_DECR            = 1,
    LIST_SORT_MODE_POW_UTIL_INCR            = 2,
    LIST_SORT_MODE_POW_ID_INCR              = 3,
    LIST_SORT_MODE_POW_ID_DECR              = 4,
    LIST_SORT_MODE_POW_CAP_INCR             = 5,
    LIST_SORT_MODE_POW_CAP_DECR             = 6,
    LIST_SORT_MODE_POW_COST_INCR            = 7,
    LIST_SORT_MODE_POW_COST_DECR            = 8,
    LIST_SORT_MODE_LOG_ID_INCR              = 9,
    LIST_SORT_MODE_LOG_ID_DECR              = 10,
    LIST_SORT_MODE_LOG_PLANT_ID_INCR        = 11,
    LIST_SORT_MODE_LOG_PLANT_ID_DECR        = 12,
    LIST_SORT_MODE_LOG_PRODUCTION_INCR      = 13,
    LIST_SORT_MODE_LOG_PRODUCTION_DECR      = 14,
    LIST_SORT_MODE_LOG_SALE_PRICE_INCR      = 15,
    LIST_SORT_MODE_LOG_SALE_PRICE_DECR      = 16,
    LIST_SORT_MODE_LOG_DATE_INCR            = 17,
    LIST_SORT_MODE_LOG_DATE_DECR            = 18,
    LIST_SORT_MODE_FIRST_UNSEL              = LIST_SORT_MODE_POW_UTIL_DECR,
    LIST_SORT_MODE_LAST_UNSEL               = LIST_SORT_MODE_POW_COST_DECR,
    LIST_SORT_MODE_FIRST_SEL                = LIST_SORT_MODE_LOG_ID_INCR,
    LIST_SORT_MODE_LOG_LAST_SEL             = LIST_SORT_MODE_LOG_DATE_DECR,
    LIST_SORT_MODE_FIRST                    = LIST_SORT_MODE_UNKNOWN,
    LIST_SORT_MODE_LAST                     = LIST_SORT_MODE_LOG_DATE_DECR
} ListSortMode;


#ifdef __ACT_IMPL_C
    #include <stdio.h>
    #include <stdint.h>
    #include <string.h>
    #include <stdlib.h>
    #include <stddef.h>
    #include <stdbool.h>
    
    #include <hashmap.h>
    #include <err_def.h>
    #include <entity_data.h>
    #include <algo.h>
    #include <mem_check.h>
    #include <prompt.h>
    #include <energy_manager.h>


    /// Unselected mode help text
    static const char *__help_unselected = 
        "energy_manager(unselected mode) usage: \n"\
        "help -- show usage info\n"\
        "new -- create a new power plant entry\n"\
        "list -- show all available power plants\n"\
        "  [i|d] -- list by increasing or decreasing value (default: increasing)\n"\
        "    plant_id -- sort by plant id\n"\
        "    rated_cap -- sort by rated capacity\n"\
        "    avg_price -- sort by average price per MWh\n"\
        "    avg_util -- sort by average utilisation\n"\
        "log -- show all available logs\n"\
        "edit <ID> -- edit power plant values\n"\
        "delete <ID> -- delete power plant from the list\n"\
        "select <ID> -- select a power plant for usage\n"\
        "save -- save the data into correct files\n"\
        "exit -- exit the program\n";

    /// Selected mode help text
    static const char *__help_selected =
        "energy_manager(selected mode) usage: \n"\
        "help -- show usage info\n"\
        "new -- create a new log entry\n"\
        "list -- show all logs for that power plant\n"\
        "  [i|d] -- list by increasing or decreasing value (default: increasing)\n"\
        "    log_id -- sort by log id\n"\
        "    plant_id -- sort by plant id\n"\
        "    production -- sort by production output\n"\
        "    price -- sort by average price for that day\n"\
        "    date -- sort by date\n"\
        "edit <ID> -- edit log values\n"\
        "delete <ID> -- delete log\n"\
        "unsel -- unselect current power plant\n"\
        "save -- save the data into correct files\n"\
        "exit -- exit selected mode\n";


    #define __MAX_LOG_LINE                  256
    #define __MAX_PLANT_FILE_LINE(max_name) __roundToBase2(82 + max_name)
    #define __DEFAULT_BUF_SIZE              4096

    
    /// Perform sorting on power plant data according to the list sorting mode
    void __sortPowerPlantRefs(PowerPlantRefs *p_refs, ListSortMode smode);


    /// Perform required sorting on logs references according to the list 
    /// sorting mode
    void __sortLogRefs(PlantLogRefs *p_logs, ListSortMode smode);


    /// Perform required sorting on logs according to the list sorting mode
    void __sortLogs(PlantLogs *p_logs, ListSortMode smode);
#endif


/// Print out help text
void showHelp(bool is_sel);


/// Ask information about the new power plant instance from the user
/// and create a new instance
void createNewPowerPlant(PowerPlants *p_plants, Hashmap *p_map);


/// List all currently available power plants according to specified sort mode
void listPowerPlants(PowerPlants *p_plants, ListSortMode smode);


/// List all written logs according to specified sort mode
void listAllLogs(PlantLogs *p_logs, ListSortMode smode);


/// List all logs that belong to the power plant
void listPowerPlantLogs(PlantData *plant, ListSortMode smode);


/// Edit power plant properties
void editPowerPlant(Hashmap *plant_map, uint32_t index);


/// Create a new log for certain power plant instance
void newLog(Hashmap *pow_map, Hashmap *log_map, PowerPlants *p_plants, 
    PlantLogs *p_logs, uint32_t sel_id);


/// Edit the power plant log data
void editLog(Hashmap *plant_map, Hashmap *log_map, uint32_t sel_id, uint32_t index);


/// Delete a power plant entry
void deletePowerPlant(PowerPlants *p_plants, Hashmap *plant_map, uint32_t index);


/// Delete a log entry
void deleteLog(PlantLogs *p_logs, Hashmap *pow_map, Hashmap *log_map, uint32_t sel_id, uint32_t index);


/// Check if the user provided selection id is available for selection
void selectionCheck(uint32_t *p_sel_val, uint32_t arg, Hashmap *p_map);


/// Save all edited data into a file
void saveData(PowerPlants *p_plants, PlantLogs *p_logs, char *plants_file,
    char *logs_file);
#endif
