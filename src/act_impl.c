/*
 * File:        act_impl.c
 * Author:      Karl-Mihkel Ott
 * Created      2021-05-20
 * Last edit:   2021-05-20
 * Description: Contains function definitions to user command 
 *              action implementations
 */


#define __ACT_IMPL_C
#include <act_impl.h>


/// Perform sorting on power plant data according to the list sorting mode
void __sortPowerPlantRefs(PowerPlantRefs *p_refs, ListSortMode smode) {
    // If no plants are found return
    if(!p_refs->n) return;

    // Check if sorting should be done for power plants
    switch(smode) {
    case LIST_SORT_MODE_POW_UTIL_DECR:
        mergesort(p_refs->p_plants, offsetof(PlantData, avg_utilisation), sizeof(PlantData*),
            true, SORT_VALUE_TYPE_FLOAT32, true, 0, p_refs->n - 1);
        break;

    case LIST_SORT_MODE_POW_UTIL_INCR:
        mergesort(p_refs->p_plants, offsetof(PlantData, avg_utilisation), sizeof(PlantData*),
            false, SORT_VALUE_TYPE_FLOAT32, true, 0, p_refs->n - 1);
        break;

    case LIST_SORT_MODE_POW_ID_INCR:
        mergesort(p_refs->p_plants, offsetof(PlantData, no), sizeof(PlantData*),
            false, SORT_VALUE_TYPE_UINT32, true, 0, p_refs->n - 1);
        break;

    case LIST_SORT_MODE_POW_ID_DECR:
        mergesort(p_refs->p_plants, offsetof(PlantData, no), sizeof(PlantData*), 
            true, SORT_VALUE_TYPE_UINT32, true, 0, p_refs->n - 1);
        break;

    case LIST_SORT_MODE_POW_CAP_INCR:
        mergesort(p_refs->p_plants, offsetof(PlantData, rated_cap), sizeof(PlantData*), 
            false, SORT_VALUE_TYPE_FLOAT32, true, 0, p_refs->n - 1);
        break;

    case LIST_SORT_MODE_POW_CAP_DECR:
        mergesort(p_refs->p_plants, offsetof(PlantData, rated_cap), sizeof(PlantData*), 
            true, SORT_VALUE_TYPE_FLOAT32, true, 0, p_refs->n - 1);
        break;

    case LIST_SORT_MODE_POW_COST_INCR:
        mergesort(p_refs->p_plants, offsetof(PlantData, avg_cost), sizeof(PlantData*),
            false, SORT_VALUE_TYPE_FLOAT32, true, 0, p_refs->n - 1);
        break;

    case LIST_SORT_MODE_POW_COST_DECR:
        mergesort(p_refs->p_plants, offsetof(PlantData, avg_cost), sizeof(PlantData*), 
            true, SORT_VALUE_TYPE_FLOAT32, true, 0, p_refs->n - 1);
        break;

    default: 
        break;
    }
}


/// Perform required sorting on logs according to the list sorting mode
void __sortLogs(PlantLogs *p_logs, ListSortMode smode) {
    // If no logs are found return
    if(!p_logs->n) return;

    switch(smode) {
    case LIST_SORT_MODE_LOG_ID_INCR:
        mergesort(p_logs->entries, offsetof(LogEntry, log_id), sizeof(LogEntry),
            false, SORT_VALUE_TYPE_UINT32, false, 0, p_logs->n - 1);
        break;

    case LIST_SORT_MODE_LOG_ID_DECR:
        mergesort(p_logs->entries, offsetof(LogEntry, log_id), sizeof(LogEntry),
            true, SORT_VALUE_TYPE_UINT32, false, 0, p_logs->n - 1);
        break;

    case LIST_SORT_MODE_LOG_PLANT_ID_INCR:
        mergesort(p_logs->entries, offsetof(LogEntry, plant_no), sizeof(LogEntry),
            false, SORT_VALUE_TYPE_UINT32, false, 0, p_logs->n - 1);
        break;

    case LIST_SORT_MODE_LOG_PLANT_ID_DECR:
        mergesort(p_logs->entries, offsetof(LogEntry, plant_no), sizeof(LogEntry),
            true, SORT_VALUE_TYPE_UINT32, false, 0, p_logs->n - 1);
        break;

    case LIST_SORT_MODE_LOG_PRODUCTION_INCR:
        mergesort(p_logs->entries, offsetof(LogEntry, production), sizeof(LogEntry),
            false, SORT_VALUE_TYPE_FLOAT32, false, 0, p_logs->n - 1);
        break;

    case LIST_SORT_MODE_LOG_PRODUCTION_DECR:
        mergesort(p_logs->entries, offsetof(LogEntry, production), sizeof(LogEntry),
            true, SORT_VALUE_TYPE_FLOAT32, false, 0, p_logs->n - 1);
        break;

    case LIST_SORT_MODE_LOG_SALE_PRICE_INCR:
        mergesort(p_logs->entries, offsetof(LogEntry, avg_sale_price), sizeof(LogEntry),
            false, SORT_VALUE_TYPE_FLOAT32, false, 0, p_logs->n - 1);
        break;

    case LIST_SORT_MODE_LOG_SALE_PRICE_DECR:
        mergesort(p_logs->entries, offsetof(LogEntry, avg_sale_price), sizeof(LogEntry),
            true, SORT_VALUE_TYPE_FLOAT32, false, 0, p_logs->n - 1);
        break;

    case LIST_SORT_MODE_LOG_DATE_INCR:
        mergesort(p_logs->entries, offsetof(LogEntry, date), sizeof(LogEntry),
            false, SORT_VALUE_TYPE_DATE, false, 0, p_logs->n - 1);
        break;

    case LIST_SORT_MODE_LOG_DATE_DECR:
        mergesort(p_logs->entries, offsetof(LogEntry, date), sizeof(LogEntry),
            true, SORT_VALUE_TYPE_DATE, false, 0, p_logs->n - 1);
        break;

    default: 
        break;
    }
}


/// Perform required sorting on logs according to the list sorting mode
void __sortLogRefs(PlantLogRefs *p_logs, ListSortMode smode) {
    // If no logs are found return
    if(!p_logs->n) return;

    switch(smode) {
    case LIST_SORT_MODE_LOG_ID_INCR:
        mergesort(p_logs->p_entries, offsetof(LogEntry, log_id), sizeof(LogEntry*),
            false, SORT_VALUE_TYPE_UINT32, true, 0, p_logs->n - 1);
        break;

    case LIST_SORT_MODE_LOG_ID_DECR:
        mergesort(p_logs->p_entries, offsetof(LogEntry, log_id), sizeof(LogEntry*),
            true, SORT_VALUE_TYPE_UINT32, true, 0, p_logs->n - 1);
        break;

    case LIST_SORT_MODE_LOG_PLANT_ID_INCR:
        mergesort(p_logs->p_entries, offsetof(LogEntry, plant_no), sizeof(LogEntry*),
            false, SORT_VALUE_TYPE_UINT32, true, 0, p_logs->n - 1);
        break;

    case LIST_SORT_MODE_LOG_PLANT_ID_DECR:
        mergesort(p_logs->p_entries, offsetof(LogEntry, plant_no), sizeof(LogEntry*),
            true, SORT_VALUE_TYPE_UINT32, true, 0, p_logs->n - 1);
        break;

    case LIST_SORT_MODE_LOG_PRODUCTION_INCR:
        mergesort(p_logs->p_entries, offsetof(LogEntry, production), sizeof(LogEntry*),
            false, SORT_VALUE_TYPE_FLOAT32, true, 0, p_logs->n - 1);
        break;

    case LIST_SORT_MODE_LOG_PRODUCTION_DECR:
        mergesort(p_logs->p_entries, offsetof(LogEntry, production), sizeof(LogEntry*),
            true, SORT_VALUE_TYPE_FLOAT32, true, 0, p_logs->n - 1);
        break;

    case LIST_SORT_MODE_LOG_SALE_PRICE_INCR:
        mergesort(p_logs->p_entries, offsetof(LogEntry, avg_sale_price), sizeof(LogEntry*),
            false, SORT_VALUE_TYPE_FLOAT32, true, 0, p_logs->n - 1);
        break;

    case LIST_SORT_MODE_LOG_SALE_PRICE_DECR:
        mergesort(p_logs->p_entries, offsetof(LogEntry, avg_sale_price), sizeof(LogEntry*),
            true, SORT_VALUE_TYPE_FLOAT32, true, 0, p_logs->n - 1);
        break;

    case LIST_SORT_MODE_LOG_DATE_INCR:
        mergesort(p_logs->p_entries, offsetof(LogEntry, date), sizeof(LogEntry*),
            false, SORT_VALUE_TYPE_DATE, true, 0, p_logs->n - 1);
        break;

    case LIST_SORT_MODE_LOG_DATE_DECR:
        mergesort(p_logs->p_entries, offsetof(LogEntry, date), sizeof(LogEntry*),
            true, SORT_VALUE_TYPE_DATE, true, 0, p_logs->n - 1);
        break;

    default: 
        break;
    }
}


/// Print out help text
void showHelp(bool is_sel) {
    // Check if selected mode text or unselected mode text should be shown
    if(!is_sel) printf("\n%s\n", __help_unselected);
    else printf("\n%s\n", __help_selected);
}


/// Ask information about the new power plant instance from the user
/// and create a new instance
void createNewPowerPlant(PowerPlants *p_plants, Hashmap *p_map) {
    PlantData user_data = promptNewPowerPlant(&p_plants->max_id, p_map);
    newPowerPlant(&user_data, p_plants, p_map);
}


/// List all currently available power plants according to
/// the average utilisation percentage
void listPowerPlants(PowerPlants *p_plants, ListSortMode smode) {
    // Allocate memory for power plant references
    PowerPlantRefs refs = { .n = p_plants->n, .cap = p_plants->cap };
    refs.p_plants = (PlantData**) calloc(p_plants->cap, sizeof(PlantData*));

    // Copy all pointers to newly allocated power plant reference array
    for(size_t i = 0; i < refs.n; i++)
        refs.p_plants[i] = p_plants->plants + i;

    // Sort power plants if necessary
    __sortPowerPlantRefs(&refs, smode);
    displayPowerPlants(&refs);

    // Free allocated memory
    free(refs.p_plants);
}


/// List all written logs according to specified sort mode
void listAllLogs(PlantLogs *p_logs, ListSortMode smode) {
    // Allocate memory for log references
    PlantLogRefs refs = { 0 };
    refs.cap = p_logs->cap;
    refs.n = p_logs->n;
    refs.p_entries = (LogEntry**) calloc(p_logs->cap, sizeof(LogEntry*));

    // Populate references' array with new log data
    for(size_t i = 0; i < refs.n; i++)
        refs.p_entries[i] = p_logs->entries + i;

    // Sort all data according to the given sort mode
    __sortLogRefs(&refs, smode);
    displayLogData(&refs);

    // Free the reference buffer
    free(refs.p_entries);
}


/// List all logs that belong to the power plant
void listPowerPlantLogs(PlantData *plant, ListSortMode smode) {
    // Sort data and display it to stdout
    __sortLogRefs(&plant->logs, smode);
    displayLogData(&plant->logs);
}


/// Edit power plant properties
void editPowerPlant(Hashmap *plant_map, uint32_t index) {
    // Check if id parsing failed
    if(index == UINT32_MAX) {
        printf("Invalid index given for power plants\n");
        printf("Try again\n\n");
        return;
    }

    // Find the PlantData reference
    PlantData *data = (PlantData*) findValue(plant_map, &index, sizeof(uint32_t));
        
    // Check if data exists
    if(!data) {
        printf("Failed to edit power plant with id %u\n"
               "Power plant not available\n\n", index);
        return;
    }

    promptEditPowerPlant(data);
}


/// Create a new log for certain power plant instance
void newLog (
    Hashmap *pow_map, 
    Hashmap *log_map, 
    PowerPlants *p_plants, 
    PlantLogs *p_logs, 
    uint32_t sel_id
) {
    LogEntry log = promptNewLogEntry(log_map, &p_logs->max_id, sel_id);

    // Push the log entry to PlantLogs array
    LogEntry *prev_val = p_logs->entries;
    reallocCheck((void**) &p_logs->entries, sizeof(LogEntry), p_logs->n + 1,
        &p_logs->cap); 

    // Check if reallocation happended and memory addresses should be adjusted
    if(p_logs->entries - prev_val) {
        size_t delta = p_logs->entries - prev_val;
        // For each power plant entry adjust its log memory addresses
        for(size_t i = 0; i < p_plants->n; i++) {
            for(size_t j = 0; j < p_plants->plants[i].logs.n; j++)
                p_plants->plants[i].logs.p_entries[j] += delta;
        }

        // For each log replace its value in hashmap
        for(size_t i = 0; i < p_logs->n; i++) {
            popFromHashmap(log_map, &p_logs->entries[i].log_id, sizeof(uint32_t));
            pushToHashmap(log_map, &p_logs->entries[i].log_id, sizeof(uint32_t),
                p_logs->entries + i);
        }
    }
    
    p_logs->entries[p_logs->n] = log;
    p_logs->n++;

    // Push log entry to hashmap
    printf("log_id: %u\n", log.log_id);
    pushToHashmap(log_map, &p_logs->entries[p_logs->n - 1].log_id, sizeof(uint32_t), 
        p_logs->entries + p_logs->n - 1);

    // Retrieve associated power plant instance and push new LogEntry value
    // to power plant logs' data
    PlantData *p_pow_data = (PlantData*) findValue(pow_map, &sel_id, sizeof(uint32_t));

    reallocCheck((void**) &p_pow_data->logs.p_entries, sizeof(LogEntry*), p_pow_data->logs.n,
        &p_pow_data->logs.cap);
    p_pow_data->logs.p_entries[p_pow_data->logs.n] = p_logs->entries + p_logs->n - 1;
    p_pow_data->logs.n++;

    // Recalculate average cost and utilisation
    calcAvgCost(p_pow_data);
    calcAvgUtilisation(p_pow_data);
}


/// Edit the power plant log data
void editLog(Hashmap *plant_map, Hashmap *log_map, uint32_t sel_id, uint32_t index) {
    // Check if the id was given
    if(index == UINT32_MAX) {
        printf("Invalid index given for power plant logs\n");
        printf("Try again\n\n");
        return;
    }

    // Retrieve the LogEntry reference from the map 
    printf("%u\n", index);
    LogEntry *log = findValue(log_map, &index, sizeof(uint32_t));

    // Check if the retrieval was successful
    if(!log || log->plant_no != sel_id) {
        printf("%p\n", log);
        printf("Cannot edit log entry with ID %u\n"\
               "Log not available\n", index);
        return;
    }

    promptEditLog(log);

    // Find the associated plant data and update its average cost 
    // and utilisation
    PlantData *plant = (PlantData*) findValue(plant_map, &log->plant_no, sizeof(uint32_t));
    calcAvgUtilisation(plant);
    calcAvgCost(plant);
}


/// Delete a power plant entry
void deletePowerPlant(PowerPlants *p_plants, Hashmap *plant_map, uint32_t index) {
    // Check if the id was given
    if(index == UINT32_MAX) {
        printf("Invalid delete index given for power plants\n");
        printf("Try again\n\n");
        return;
    }

    PlantData *p_pop_plant = (PlantData*) popFromHashmap(plant_map, &index, sizeof(uint32_t));

    // Check if no elements were found in the map
    if(!p_pop_plant) {
        printf("Cannot delete power plant entry with id %u\n"\
               "Power plant not available\n\n", index);
        return;
    }

    // Free memory allocated for log instances
    free(p_pop_plant->logs.p_entries);
    // Free memory allocated for plant name
    free(p_pop_plant->name);

    // Find the memory area index in array
    size_t a_ind = p_pop_plant - p_plants->plants; 

    // For each element after the popped plant instance, shift the elements in array to left
    for(size_t i = a_ind + 1; i < p_plants->n; i++) {
        // Pop the previous reference from the map and push the new one
        PlantData *data = (PlantData*) popFromHashmap(plant_map, &p_plants->plants[i].no,
            sizeof(uint32_t));
        p_plants->plants[i - 1] = p_plants->plants[i];
        pushToHashmap(plant_map, &p_plants->plants[i - 1].no, sizeof(uint32_t),
            p_plants->plants + i - 1);
    }

    memset(p_plants->plants + p_plants->n - 1, 0, sizeof(PlantData));
    p_plants->n--;
}


/// Delete a log entry
void deleteLog(PlantLogs *p_logs, Hashmap *pow_map, Hashmap *log_map, uint32_t sel_id, uint32_t index) {
    // Check if delete index was correct
    if(index == UINT32_MAX) {
        printf("Invalid delete index given for power plants\n");
        printf("Try again\n\n");
        return;
    }

    LogEntry *del_entry = (LogEntry*) findValue(log_map, &index, sizeof(uint32_t));

    // Check if no entries were found in the map
    if(!del_entry || del_entry->plant_no != sel_id) {
        printf("Cannot delete log entry with id %u\n"\
               "Log entry not available\n\n", index);
        return;
    }
    
    // Find the memory area index in array
    size_t a_ind = del_entry - p_logs->entries;

    // Retrieve the associated plant data instance
    PlantData *p_data = (PlantData*) findValue(pow_map, &del_entry->plant_no, sizeof(uint32_t));

    // For each element after the popped value, shift elements to the left
    for(size_t i = a_ind + 1; i < p_logs->n; i++) {
        // Pop the previous reference from the hashmap and push it back with
        // correct pointer value
        LogEntry *p_entry = (LogEntry*) popFromHashmap(log_map, &p_logs->entries[i].log_id, sizeof(uint32_t));
        p_logs->entries[i - 1] = p_logs->entries[i];
        pushToHashmap(log_map, &p_logs->entries[i - 1].log_id, sizeof(uint32_t), 
            p_logs->entries + i - 1);
    }
       
    

    // For each log entry check and select new value for associated PowerPlant instance
    for(size_t i = 0, j = 0; i < p_logs->n; i++) {
        if(p_logs->entries[i].plant_no == p_data->no) {
            p_data->logs.p_entries[j] = p_logs->entries + i;
            j++;
        }
    }

    // Set the deprecated log data value to zero
    memset(p_logs->entries + p_logs->n - 1, 0, sizeof(LogEntry));
    p_logs->n--;
    memset(p_data->logs.p_entries + p_data->logs.n - 1, 0, sizeof(LogEntry*));
    p_data->logs.n--;
}


/// Check if the user provided selection id is available for selection
void selectionCheck(uint32_t *p_sel_val, uint32_t arg, Hashmap *p_map) {
    // Check if the id is valid
    if(arg == UINT32_MAX) {
        printf("Invalid selection id given\n");
        printf("Try again\n\n");
        return;
    }

    // Check if argument value is a valid key value in the given map
    if(!findValue(p_map, &arg, sizeof(uint32_t))) {
        printf("Cannot select power plant with id %u\n"
               "Power plant not available\n\n", arg);
    }

    else *p_sel_val = arg;
}


/// Save all edited data into a file
void saveData (
    PowerPlants *p_plants, 
    PlantLogs *p_logs, 
    char *plants_file,
    char *logs_file
) {
    // Open files for writing
    FILE *plant_file = fopen(plants_file, "wb");
    FILE *log_file = fopen(logs_file, "wb");

    // Check if file opening was successful
    if(!plant_file) FOPEN_ERR(plants_file);
    if(!log_file) FOPEN_ERR(logs_file);

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
    size_t log_buf_len = __MAX_LOG_LINE * (!log_len ? 1 : log_len);

    // Allocate memory for csv buffers
    char *plant_buf = (char*) calloc(plant_buf_len, sizeof(char));
    char *log_buf = (char*) calloc(log_buf_len, sizeof(char));

    // For each power plant instance write data and log data to their buffers
    for(size_t i = 0; i < p_plants->n; i++) {
        // Convert the FuelType into appropriate string instance
        char *fuel = fuelTypeToStr(p_plants->plants[i].fuel);
        sprintf(plant_buf, "%d,\"%s\",\"%s\",%f\n", p_plants->plants[i].no,
            p_plants->plants[i].name, fuel, p_plants->plants[i].rated_cap);

        // Write the power plant data instance into its file
        fwrite(plant_buf, sizeof(char), strlen(plant_buf), plant_file);

        // For each log entry in power plant entry, write it to the buffer
        for(size_t j = 0; j < p_plants->plants[i].logs.n; j++) {
            LogEntry *p_ent = p_plants->plants[i].logs.p_entries[j];

            char month[8] = { 0 };
            char day[8] = { 0 };

            // Check if additional zero paddings should be added when formating month
            if(p_ent->date.month < 10) sprintf(month, "0%d", p_ent->date.month);
            else sprintf(month, "%d", p_ent->date.month);

            // Check if additional zero paddings should be added when formating day
            if(p_ent->date.day < 10) sprintf(day, "0%d", p_ent->date.day);
            else sprintf(day, "%d", p_ent->date.day);
              
            // Write csv line data into buffer
            sprintf(log_buf, "%d,%d,%f,%f,\"%d-%s-%s\"\n",
                p_ent->log_id, p_plants->plants[i].no, p_ent->production, 
                p_ent->avg_sale_price, p_ent->date.year, month, day);

            // Write the log data to file
            fwrite(log_buf, sizeof(char), strlen(log_buf), log_file);
        }

        // Set the buffer datas to zero
        memset(plant_buf, 0x00, plant_buf_len);
        memset(log_buf, 0x00, log_buf_len);
    }

    // Free all buffers allocated
    free(plant_buf);
    free(log_buf);

    printf("Saved to files '%s' and '%s'!\n", plants_file, logs_file);
}
