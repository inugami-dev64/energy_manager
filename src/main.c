/*
 * File:        main.c
 * Author:      Karl-Mihkel Ott
 * Created      2021-05-13
 * Last edit:   2021-05-13
 * Description: Contains main and input polling functions
 */


#define __MAIN_C
#include <main.h>


/// Poll user input
void poll() {
    // Input data buffer
    char in_buf[__DEFAULT_BUF_LEN] = {};

    printf("Welcome to energy manager program!\n"
           "To view all available commands type 'help'\n");

    // Main program loop
    while(true) {
        printf("(energy_manager) ");
        fflush(stdout);
        fgets(in_buf, __DEFAULT_BUF_LEN, stdin);
    }
}



int main(int argc, char *argv[]) {
    // Read the user input about power plant file
    char pow_file[1024] = { 0 };
    printf("Enter power plant file name: ");
    fflush(stdout);
    scanf("%s", pow_file);

    // Read user input about log file
    char log_file[1024] = { 0 };
    printf("Enter power plant log file name: ");
    fflush(stdout);
    scanf("%s", log_file);


    // Parse and log power plants information
    PowerPlants plants;
    parsePowerPlantFile(pow_file, &plants);

    // Parse and log power plant logs
    PlantLogs logs;
    parseLogsFile(log_file, &logs);

    // Create hashmap instances for power plant data and daily log data
    Hashmap pow_map = createPowerPlantMap(&plants);
    Hashmap log_map = createLogMap(&logs);

    // Put log data into their corresponding PlantData instance
    associateLogData(&plants, &logs, &pow_map);

    return EXIT_SUCCESS;
}
