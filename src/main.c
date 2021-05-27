/*
 * File:        main.c
 * Author:      Karl-Mihkel Ott
 * Created      2021-05-13
 * Last edit:   2021-05-23
 * Description: Contains main and input polling functions
 */


#define __MAIN_C
#include <main.h>


/// Poll user input
void poll(char *pow_file, char *log_file, time_t start) {
    // Create a new logger instance
    FILE *slog = newLogger(start);

    // Parse and log power plants information
    PowerPlants plants = { 0 };
    parsePowerPlantFile(pow_file, &plants);
    logMiscInfo(slog, "Parsed power plant file contents\n", start);

    // Parse and log power plant logs
    PlantLogs logs = { 0 };
    parseLogsFile(log_file, &logs);
    logMiscInfo(slog, "Parsed log file contents\n", start);

    // Create commandline token map
    Hashmap tokens = tokeniseUserInput();

    // Create hashmap instances for power plant data and daily log data
    Hashmap pow_map = createPowerPlantMap(&plants);
    Hashmap log_map = createLogMap(&logs);

    // Put log data into their corresponding PlantData instance
    associateLogData(&plants, &logs, &pow_map);

    // Input data buffer
    char in_buf[__DEFAULT_BUF_LEN] = { 0 };

    // Set the selected id as maximum possible
    uint32_t selected = UINT32_MAX;
    char *name_arg = NULL;
    ListSortMode sort_mode = LIST_SORT_MODE_UNKNOWN;

    // Allocate resources for ncurses
    printf("Welcome to energy manager program!\n"\
           "To view all available commands type 'help'\n\n");

    // Main program loop
    bool is_running = true;
    while(is_running) {
        // Print the energy manager input section prompt to line before end
        if(selected == UINT32_MAX)
            printf("(energy_manager) ");
        else printf("(energy_manager:%d) ", selected);

        // Get the user input into buffer
        fgets(in_buf, __DEFAULT_BUF_LEN, stdin);
        in_buf[strlen(in_buf) - 1] = 0x00;

        // Parse the input into enumeral value
        uint32_t arg = UINT32_MAX;
        sort_mode = LIST_SORT_MODE_UNKNOWN;
        UserInputAction act = parseUserInputAction(&tokens, in_buf, selected != UINT32_MAX,
            &sort_mode, &arg);

        // Check the parsed action value and call appropriate functions
        switch(act) {
        // Unselected mode actions
        case USER_INPUT_ACTION_U_SHOW_HELP:
            showHelp(false);
            break;

        case USER_INPUT_ACTION_U_NEW_POWER_PLANT:
            createNewPowerPlant(&plants, &arg, &pow_map);
            break;

        case USER_INPUT_ACTION_U_LIST_PLANTS:
            listPowerPlants(&plants, sort_mode);
            break;

        case USER_INPUT_ACTION_U_EDIT_POWER_PLANT:
            editPowerPlant(&pow_map, arg);
            break;

        case USER_INPUT_ACTION_U_LIST_LOGS:
            listAllLogs(&logs, sort_mode);
            break;

        case USER_INPUT_ACTION_U_DELETE_POWER_PLANT:
            deletePowerPlant(&plants, &pow_map, arg);
            break;

        case USER_INPUT_ACTION_U_SELECT_POWER_PLANT:
            selectionCheck(&selected, arg, &pow_map);
            break;

        case USER_INPUT_ACTION_S_SHOW_HELP:
            showHelp(true);
            break;

        case USER_INPUT_ACTION_S_LIST_LOGS: {
            PlantData *data = (PlantData*) findValue(&pow_map, &selected, sizeof(uint32_t));
            listPowerPlantLogs(data, sort_mode);
            break;
        }

        case USER_INPUT_ACTION_S_EDIT_LOG:
            editLog(&pow_map, &log_map, selected, arg);
            break;

        case USER_INPUT_ACTION_S_NEW_LOG: {
            newLog(&pow_map, &log_map, &plants, &logs, &arg, selected);
            break;
        }

        case USER_INPUT_ACTION_S_DELETE_LOG:
            deleteLog(&logs, &pow_map, &log_map, selected, arg);
            break;

        case USER_INPUT_ACTION_S_UNSEL_POWER_PLANT:
            selected = UINT32_MAX;
            name_arg = NULL;
            break;

        case USER_INPUT_ACTION_SAVE:
            saveData(&plants, &logs, pow_file, log_file);
            break;

        case USER_INPUT_ACTION_EXIT:
            // Clear hashmaps
            destroyHashmap(&pow_map);
            destroyHashmap(&log_map);
            destroyHashmap(&tokens);

            // For each power plant instance free the memory that was
            // allocated for their log pointers and name
            for(size_t i = 0; i < plants.n; i++) {
                free(plants.plants[i].name);
                free(plants.plants[i].logs.p_entries);
            }
            
            // Free all memory that was allocated for storing plant and log data
            free(plants.plants);
            free(logs.entries);

            is_running = false;
            break;

        default:
            printf("Unknown command '%s'\n", in_buf);
            break;
        }

        // Log the command into command log file
        logCommandAction(slog, act, arg != UINT32_MAX ? arg : selected, start);
    }

    // Close the log file stream
    destroyLogger(slog);
}


int main(int argc, char *argv[]) {
    // Start measuring program runtime duration
    time_t start = time(NULL);

    // Read the user input about power plant file
    char pow_file[1024] = { 0 };
    printf("Enter power plant file name: ");
    fflush(stdout);
    fgets(pow_file, 1024, stdin);
    pow_file[strlen(pow_file) - 1] = 0x00;

    // Read user input about log file
    char log_file[1024] = { 0 };
    printf("Enter power plant log file name: ");
    fflush(stdout);
    fgets(log_file, 1024, stdin);
    log_file[strlen(log_file) - 1] = 0x00;

    // Start input polling
    poll(pow_file, log_file, start);
    return EXIT_SUCCESS;
}
