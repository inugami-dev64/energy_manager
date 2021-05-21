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


/// Print out help text
void showHelp(bool is_sel) {
    // Check if selected mode text or unselected mode text should be shown
    if(!is_sel) printf("\n%s\n", __help_unselected);
    else printf("\n%s\n", __help_selected);
}


/// List all currently available power plants according to
/// the average utilisation percentage
void listPowerPlants(PowerPlants *p_plants) {
    // Find the terminal size, which is needed for formating the data 
    struct winsize ws;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);

    // Create a power plant separator 
    char sep[ws.ws_col + 1];
    memset(sep, '_', ws.ws_col);
    sep[ws.ws_col] = 0x00;

    // Print the table heading information
    printf(" | ID | Name | Fuel | Rated cap | Average cost | Average utilisation | \n");

    // For each power plant, show its information
    for(size_t i = 0; i < p_plants->n; i++) {
        // Create a buffer for string data
        char buf[4096] = { 0 };
        sprintf(buf, "  %d.  |  %s  |  uranium  |  %.2f  |  %.2f  |  %.2f  ",
            p_plants->plants[i].no, p_plants->plants[i].name, p_plants->plants[i].rated_cap,
            p_plants->plants[i].avg_cost, p_plants->plants[i].avg_utililisation);  

        // Print the output data to stdout
        printf("%s\n%s\n%s\n", sep, buf, sep);
    }
}


/// List all written logs in date order
void listAllLogs(PlantLogs *p_logs) {
}


/// List all logs that belong to the power plant
void listPowePlantLogs(PlantData *plant) {
}


/// Edit power plant properties
void editPowerPlant(Hashmap *plant_map, uint32_t index) {
}


/// Edit the power plant log data
void editLog(Hashmap *log_map, uint32_t index) {
}


/// Delete a power plant entry
void deletePowerPlant(PowerPlants *p_plants, Hashmap *plant_map, uint32_t index) {
}


/// Delete a log entry
void deleteLog(PlantLogs *p_logs, Hashmap *log_map, uint32_t index) {
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

    printf("Written buffer to files '%s' and '%s'!\n", plants_file, logs_file);
}
