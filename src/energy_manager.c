/*
 * File:        energy_manager.c
 * Author:      Karl-Mihkel Ott
 * Created      2021-05-16
 * Last edit:   2021-05-20
 * Description: Contains function definitions to edit, load and save power plant data
 */

#define __ENERGY_MANAGER_C
#include <energy_manager.h>

/// Create a new hashmap instance for power plants
Hashmap createPowerPlantMap(PowerPlants *p_power_plants) {
    Hashmap map = {};
    newHashmap(&map, p_power_plants->cap << 1);

    // Variable for containing PlantData pointers retrieved from hashmap
    PlantData *p_duplicate = NULL;

    // For each power plant add it to hashmap if possible
    for(size_t i = 0; i < p_power_plants->n; i++) {
        // Check if power plant with current key value already exists
        if((p_duplicate = (PlantData*) findValue(&map, &p_power_plants->plants[i].no, sizeof(uint32_t)))) {
            DuplicateEntryAction act = promptDuplicatePowerPlantEntries(p_power_plants->plants + i, p_duplicate);
            handleDuplicatePowerPlantEntries(act, &map, p_power_plants,
                p_power_plants->plants + i, p_duplicate, p_power_plants->plants[i].no);
        }

        // No duplicates were found
        else {
            pushToHashmap(&map, &p_power_plants->plants[i].no, sizeof(uint32_t), p_power_plants->plants + i);
        }
    }

    return map;
}


/// Create a new hashmap instance for daily logs
Hashmap createLogMap(PlantLogs *p_logs) {
    Hashmap map = {};
    newHashmap(&map, p_logs->cap << 1);

    // Variable for containing PlantData pointers retrieved from hashmap
    LogEntry *p_duplicate = NULL;

    // For each power plant add it to hashmap if possible
    for(size_t i = 0; i < p_logs->n; i++) {
        // Check if power plant with current key value already exists
        if((p_duplicate = (LogEntry*) findValue(&map, &p_logs->entries[i].log_id, sizeof(uint32_t)))) {
            DuplicateEntryAction act = promptDuplicateLogEntries(p_logs->entries + i, p_duplicate);
            handleDuplicateLogEntries(act, &map, p_logs, p_logs->entries + i, 
                p_duplicate, p_logs->entries[i].log_id);
        }

        // No duplicates were found
        else {
            pushToHashmap(&map, &p_logs->entries[i].log_id, sizeof(uint32_t), p_logs->entries + i);
        }
    }

    return map;
}


/// Associate file read log data with its power plant instances
void associateLogData(PowerPlants *p_power_plants, PlantLogs *p_logs, Hashmap *p_map) {
    // For each power plant instance allocate initial amount of memory for logs
    for(size_t i = 0; i < p_power_plants->n; i++) {
        p_power_plants->plants[i].logs.cap = __DEFAULT_POWER_PLANT_LOG_C;
        p_power_plants->plants[i].logs.n = 0;
        p_power_plants->plants[i].logs.p_entries = (LogEntry**) calloc(__DEFAULT_POWER_PLANT_LOG_C,
            sizeof(LogEntry*));
    }

    
    // For each log instance add it to correct power plant instance
    for(size_t i = 0; i < p_logs->n; i++) {
        PlantData *p_data = (PlantData*) findValue(p_map, &p_logs->entries[i].plant_no, sizeof(uint32_t));

        // Check if the found entry is NULL and if it is, throw an error
        if(!p_data) {
            fprintf(stderr, "associateLogData(): Invalid plant number %d in log with id %d\n",
                p_logs->entries[i].plant_no, p_logs->entries[i].log_id);
            exit(EXIT_FAILURE);
        }

        // Check if reallocation might be needed
        reallocCheck((void**) &p_data->logs.p_entries, sizeof(LogEntry*), p_data->logs.n + 1, 
            &p_data->logs.cap);

        // Set the log value to its power plant entry
        p_data->logs.p_entries[p_data->logs.n] = p_logs->entries + i;
        p_data->logs.n++;
    }
}


/// Overwrite existing power plant data instance value with given value
/// NOTE: The given plant id has to be a valid key to value in hashmap, otherwise 
/// an error will be thrown
void overwritePlantData(PlantData *p_data, Hashmap *p_map) {
    // Retrive a PlantData entry from hashmap
    PlantData *p_hdata = (PlantData*) findValue(p_map, &p_data->no, sizeof(uint32_t));

    // Check if no data was found and throw error if necessary
    if(!p_hdata) {
        fprintf(stderr, "overwritePlantData(): No power plant found with number %d\n", p_data->no);
        exit(EXIT_FAILURE);
    }

    // Replace the retrieved data with new data
    *p_hdata = *p_data;
}


/// Create a new power plant instance and push it to map
void newPowerPlant (
    PlantData *p_data, 
    PowerPlants *p_plants, 
    Hashmap *p_map
) {
    // Check if power plants array needs reallocation
    reallocCheck((void**) &p_plants->plants, sizeof(PlantData), p_plants->n + 1,
        &p_plants->cap);

    // Set the new PlantData instance in power plants array
    p_plants->plants[p_plants->n] = *p_data;

    // Push the new entry to hashmap
    pushToHashmap(p_map, &p_plants->plants[p_plants->n].no, sizeof(uint32_t), 
        p_plants->plants + p_plants->n);

    // Increment the power plant count
    p_plants->n++;
}


/// Create a new power plant log entry for specific power plant
/// NOTE: plant_no must be a correct key to the plant instance, otherwise
/// an error is thrown
void newPowerPlantLog(LogEntry *entry, PlantLogs *p_logs, Hashmap *p_map) {
    // Check if reallocation might be necessary
    reallocCheck((void**) &p_logs->entries, sizeof(LogEntry), p_logs->n + 1,
        &p_logs->cap);

    // Set new log entry instance 
    p_logs->entries[p_logs->n] = *entry;
    p_logs->n++;
}


/// Handle duplicate power plant values according to the specified duplicate handling action
/// mdup is the duplicate entry that exists in the map
/// udup is the duplicate value that would be pushed to the map
void handleDuplicatePowerPlantEntries (
    DuplicateEntryAction action, 
    Hashmap *p_map,
    PowerPlants *p_plants, 
    PlantData *mdup, 
    PlantData *udup, 
    uint32_t key
) {
    // Check the action type
    switch(action) {
    case DUPLICATE_ENTRY_ACTION_USE_UNMAPPED:
        *mdup = *udup;
        break;

    case DUPLICATE_ENTRY_ACTION_APPEND_MAPPED:
        // Pop the current mapped pointer from the map
        mdup = (PlantData*) popFromHashmap(p_map, &key, sizeof(uint32_t));
        p_plants->plants[p_plants->n - 1].no = mdup->no;
        mdup->no = p_plants->max_id + 1;
        p_plants->max_id++;

        // Push previously mapped instance back to the hashmap
        pushToHashmap(p_map, &mdup->no, sizeof(uint32_t), mdup);
        
        // Push the unmapped instance to hashmap
        pushToHashmap(p_map, &p_plants->plants[p_plants->n - 1].no, sizeof(uint32_t), udup);
        break;


    case DUPLICATE_ENTRY_ACTION_APPEND_UNMAPPED:
        // Pop the current mapped pointer from the map
        p_plants->plants[p_plants->n - 1].no = p_plants->max_id + 1;
        p_plants->max_id++;
        
        // Push the unmapped instance to hashmap
        pushToHashmap(p_map, &p_plants->plants[p_plants->n - 1].no, sizeof(uint32_t),
            p_plants->plants + p_plants->n - 1);
        break;

    default:
        break;
    }
}


/// Handle duplicate log values according to the specified duplicate handling action
/// mdup is the duplicate entry that exists in the map
/// udup is the duplicate value that would be pushed to the map
void handleDuplicateLogEntries (
    DuplicateEntryAction action, 
    Hashmap *p_map,
    PlantLogs *p_logs,
    LogEntry *mdup, 
    LogEntry *udup,
    uint32_t key
) {
    // Check the duplicate handling action
    switch(action) {
    case DUPLICATE_ENTRY_ACTION_USE_UNMAPPED:
        // Set the unmapped instance value as the mapped one
        *mdup = *udup;
        break;

    case DUPLICATE_ENTRY_ACTION_APPEND_MAPPED:
        // Pop the current mapped pointer from the map
        mdup = (LogEntry*) popFromHashmap(p_map, &key, sizeof(uint32_t));
        p_logs->entries[p_logs->n - 1].log_id = mdup->log_id;
        mdup->log_id = p_logs->max_id + 1;
        p_logs->max_id++;

        // Push previously mapped instance back to the hashmap
        pushToHashmap(p_map, &mdup->log_id, sizeof(uint32_t), mdup);
        
        // Push the unmapped instance to hashmap
        pushToHashmap(p_map, &p_logs->entries[p_logs->n - 1].log_id, sizeof(uint32_t), udup);
        break;


    case DUPLICATE_ENTRY_ACTION_APPEND_UNMAPPED:
        // Pop the current mapped pointer from the map
        p_logs->entries[p_logs->n - 1].log_id = p_logs->max_id + 1;
        p_logs->max_id++;
        
        // Push the unmapped instance to hashmap
        pushToHashmap(p_map, &p_logs->entries[p_logs->n - 1].log_id, sizeof(uint32_t), udup);
        break;

    default: 
        break;
    }
}


/// Convert FuelType enumeral into appropriate string
/// NOTE: Pointer to in-scope static char is returned
char *fuelTypeToStr(FuelType tp) {
    // Check the fueltype and return correct string
    switch(tp) {
        case FUEL_TYPE_COAL: return "coal";
        case FUEL_TYPE_GAS: return "gas";
        case FUEL_TYPE_GEOTHERMAL: return "geothermal";
        case FUEL_TYPE_SHALE_OIL: return "shaleoil";
        case FUEL_TYPE_URANIUM: return "uranium";
        case FUEL_TYPE_WATER: return "water";
        case FUEL_TYPE_WIND: return "wind";
        default: return NULL;
    }
}


/// Convert string into appropriate FuelType instance
FuelType strToFuelType(char *str) {
    // Check for the string value and return correct FuelType instance
    if(!strcmp(str, "coal")) return FUEL_TYPE_COAL;
    else if(!strcmp(str, "gas")) return FUEL_TYPE_GAS;
    else if(!strcmp(str, "geothermal")) return FUEL_TYPE_GEOTHERMAL;
    else if(!strcmp(str, "shaleoil")) return FUEL_TYPE_SHALE_OIL;
    else if(!strcmp(str, "uranium")) return FUEL_TYPE_URANIUM;
    else if(!strcmp(str, "water")) return FUEL_TYPE_WATER;
    else if(!strcmp(str, "wind")) return FUEL_TYPE_WIND;

    return FUEL_TYPE_UNKNOWN;
}
