/*
 * File:        energy_manager.c
 * Author:      Karl-Mihkel Ott
 * Created      2021-05-16
 * Last edit:   2021-05-16
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

            // Check what the following action should be
            switch(act) {
            case DUPLICATE_ENTRY_ACTION_USE_FIRST:
                // Pop the existing value from hashmap and add a new one instead
                p_duplicate = popFromHashmap(&map, &p_power_plants->plants[i].no, sizeof(uint32_t));
                pushToHashmap(&map, &p_power_plants->plants[i].no, sizeof(uint32_t), 
                    p_power_plants->plants + i);
                break;

            case DUPLICATE_ENTRY_ACTION_USE_SECOND:
                continue;

            case DUPLICATE_ENTRY_ACTION_APPEND_FIRST:
                // Increment maximum power plant number value
                p_power_plants->max_id++;
                p_power_plants->plants[i].no = p_power_plants->max_id;
                
                pushToHashmap(&map, &p_power_plants->plants[i].no, sizeof(uint32_t), p_power_plants + i);
                break;

            case DUPLICATE_ENTRY_ACTION_APPEND_SECOND:
                // Pop the pointer from hashmap
                p_duplicate = popFromHashmap(&map, &p_power_plants->plants[i].no, sizeof(uint32_t));

                // Increment the maximum power plant value and set it as new key
                p_power_plants->max_id++;
                p_duplicate->no = p_power_plants->max_id;

                pushToHashmap(&map, &p_duplicate->no, sizeof(uint32_t), p_duplicate);
                break;
            }
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

            // Check what the following action should be
            switch(act) {
            case DUPLICATE_ENTRY_ACTION_USE_FIRST:
                // Pop the existing value from hashmap and add a new one instead
                p_duplicate = popFromHashmap(&map, &p_logs->entries[i].log_id, sizeof(uint32_t));
                pushToHashmap(&map, &p_logs->entries[i].log_id, sizeof(uint32_t), 
                    p_logs->entries + i);
                break;

            case DUPLICATE_ENTRY_ACTION_USE_SECOND:
                continue;

            case DUPLICATE_ENTRY_ACTION_APPEND_FIRST:
                // Increment maximum power plant number value
                p_logs->max_id++;
                p_logs->entries[i].log_id = p_logs->max_id;
                
                pushToHashmap(&map, &p_logs->entries[i].log_id, sizeof(uint32_t), p_logs->entries + i);
                break;

            case DUPLICATE_ENTRY_ACTION_APPEND_SECOND:
                // Pop the pointer from hashmap
                p_duplicate = popFromHashmap(&map, &p_logs->entries[i].log_id, sizeof(uint32_t));

                // Increment the maximum power plant value and set it as new key
                p_logs->max_id++;
                p_duplicate->log_id = p_logs->max_id;

                pushToHashmap(&map, &p_duplicate->log_id, sizeof(uint32_t), p_duplicate);
                break;
            }
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
void newPowerPlant(PlantData *p_data, PowerPlants *p_plants, Hashmap *p_map) {
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
void newPowerPlantLog(LogEntry *entry, Hashmap *p_map) {
    // Retrieve a power plant instance from the map
    PlantData *p_data = (PlantData*) findValue(p_map, &entry->plant_no, sizeof(uint32_t));

    // Check if error should be thrown
    if(!p_data) {
        fprintf(stderr, "newPowerPlantLog(): Invalid plant number %d provided in LogEntry instance\n", 
            entry->plant_no);
        exit(EXIT_FAILURE);
    }

    // Check if reallocation might be necessary
    reallocCheck((void**) &p_data->logs.entries, sizeof(LogEntry), p_data->logs.n + 1,
        &p_data->logs.cap);

    // Set new log entry instance 
    p_data->logs.entries[p_data->logs.n] = *entry;
    p_data->logs.n++;
}


/// Write all data to their corresponding files
void saveData(PowerPlants *p_plants, char *plant_file_name, char *log_file_name) {
    // Open files for writing
    FILE *plant_file = fopen(plant_file_name, "wb");
    FILE *log_file = fopen(log_file_name, "wb");

    // Check if file opening was successful
    if(!plant_file) FOPEN_ERR(plant_file_name);
    if(!log_file) FOPEN_ERR(log_file_name);

    // Find power plant with longest name and the total count of log lines
    size_t max_len = 0;
    size_t log_len = 0;
    for(size_t i = 0; i < p_plants->n; i++) {
        size_t cur_len;
        if((cur_len = strlen(p_plants->plants[i].name)) > max_len) {
            max_len = cur_len;
            log_len += p_plants->plants[i].logs.n;
        }
    }

    // Find how much memory to allocate for buffers
    size_t plant_buf_len = __MAX_PLANT_FILE_LINE(max_len) * (!p_plants->n ? 1 : p_plants->n);
    size_t plant_buf_offset = 0;
    size_t log_buf_len = __MAX_LOG_LINE * (!log_len ? 1 : log_len);
    size_t log_buf_offset = 0;

    // Allocate memory for csv buffers
    char *plant_buf = (char*) calloc(plant_buf_len, sizeof(char));
    char *log_buf = (char*) calloc(log_buf_len, sizeof(char));

    // For each power plant instance write data and log data to their buffers
    for(size_t i = 0; i < p_plants->n; i++) {
        // Check the fuel type and write to buffer accordingly
        switch(p_plants->plants[i].fuel) {
        case FUEL_TYPE_COAL:
            sprintf(plant_buf + plant_buf_offset, "%d,%s,\"coal\",%f\n", p_plants->plants[i].no,
                p_plants->plants[i].name, p_plants->plants[i].rated_cap);
            break;

        case FUEL_TYPE_SHALE_OIL:
            sprintf(plant_buf + plant_buf_offset, "%d,%s,\"shaleoil\",%f\n", p_plants->plants[i].no,
                p_plants->plants[i].name, p_plants->plants[i].rated_cap);
            break;

        case FUEL_TYPE_GAS:
            sprintf(plant_buf + plant_buf_offset, "%d,%s,\"gas\",%f\n", p_plants->plants[i].no,
                p_plants->plants[i].name, p_plants->plants[i].rated_cap);
            break;

        case FUEL_TYPE_URANIUM:
            sprintf(plant_buf + plant_buf_offset, "%d,%s,\"uranium\",%f\n", p_plants->plants[i].no,
                p_plants->plants[i].name, p_plants->plants[i].rated_cap);
            break;

        case FUEL_TYPE_WATER:
            sprintf(plant_buf + plant_buf_offset, "%d,%s,\"water\",%f\n", p_plants->plants[i].no,
                p_plants->plants[i].name, p_plants->plants[i].rated_cap);
            break;

        case FUEL_TYPE_WIND:
            sprintf(plant_buf + plant_buf_offset, "%d,%s,\"water\",%f\n", p_plants->plants[i].no,
                p_plants->plants[i].name, p_plants->plants[i].rated_cap);
            break;

        case FUEL_TYPE_GEOTHERMAL:
            sprintf(plant_buf + plant_buf_offset, "%d,%s,\"geothermal\",%f\n", p_plants->plants[i].no,
                p_plants->plants[i].name, p_plants->plants[i].rated_cap);
            break;

        default: 
            break;
        }
        
        // Increase the offset according to the concatenated string length
        plant_buf_offset += strlen(plant_buf + plant_buf_offset);

        // For each log entry in power plant entry, write it to the buffer
        for(size_t j = 0; j < p_plants->plants[i].logs.n; j++) {
            LogEntry *p_ent = &p_plants->plants[i].logs.entries[j];

            // Write csv line data into buffer
            sprintf(log_buf + log_buf_offset, "%d,%d,%f,%f,\"%d-%d-%d\"\n",
                p_ent->log_id, p_plants->plants[i].no, p_ent->production, 
                p_ent->avg_sale_price, p_ent->date.year, p_ent->date.month,
                p_ent->date.day);
        }
    }

    // Free all buffers allocated
    free(plant_buf);
    free(log_buf);
}
