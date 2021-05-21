/*
 * File:        energy_manager.h
 * Author:      Karl-Mihkel Ott
 * Created      2021-05-16
 * Last edit:   2021-05-16
 * Description: Contains function declarations to edit, load and save power plant data
 */


#ifndef __ENERGY_MANAGER_H
#define __ENERGY_MANAGER_H


#ifdef __ENERGY_MANAGER_C
    #include <stdio.h>
    #include <string.h>
    #include <stdbool.h>
    #include <stdlib.h>
    #include <stdint.h>

    #include <hashmap.h>
    #include <mem_check.h>
    #include <entity_data.h>
    #include <err_def.h>
    #include <prompt.h>

    #define __DEFAULT_POWER_PLANT_LOG_C     16
    #define __FUEL_TYPE_STR_MAX_LEN         32
#endif

/// Create a new hashmap instance for power plants
Hashmap createPowerPlantMap(PowerPlants *p_power_plants);


/// Create a new hashmap instance for daily logs
Hashmap createLogMap(PlantLogs *p_logs);


/// Associate file read log data with its power plant instances
void associateLogData(PowerPlants *p_power_plants, PlantLogs *p_logs, Hashmap *p_map);


/// Overwrite existing power plant data instance value with given value
/// NOTE: The given plant id has to be a valid key to value in hashmap, otherwise 
/// an error will be thrown
void overwritePlantData(PlantData *p_data, Hashmap *p_map);


/// Create a new power plant instance and push it to map
void newPowerPlant(PlantData *p_data, PowerPlants *p_plants, Hashmap *p_map);


/// Create a new power plant log entry for specific power plant
void newPowerPlantLog(LogEntry *entry, PlantLogs *p_logs, Hashmap *p_map);


/// Handle duplicate power plant values according to the specified duplicate handling action
/// mdup is the duplicate entry that exists in the map
/// udup is the duplicate value that would be pushed to the map
void handleDuplicatePowerPlantEntries(DuplicateEntryAction action, Hashmap *p_map,
    PowerPlants *p_plants, PlantData *mdup, PlantData *udup, uint32_t key);


/// Handle duplicate log values according to the specified duplicate handling action
/// mdup is the duplicate entry that exists in the map
/// udup is the duplicate value that would be pushed to the map
void handleDuplicateLogEntries(DuplicateEntryAction action, Hashmap *p_map,
    PlantLogs *p_logs, LogEntry *mdup, LogEntry *udup, uint32_t key);


/// Convert FuelType enumeral into appropriate string
/// NOTE: Pointer to in-scope static char is returned
char *fuelTypeToStr(FuelType tp);


/// Convert string into appropriate FuelType instance
FuelType strToFuelType(char *str);

#endif
