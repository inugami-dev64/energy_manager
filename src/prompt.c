/*
 * File:        action_prompt.c
 * Author:      Karl-Mihkel Ott
 * Created      2021-05-13
 * Last edit:   2021-05-13
 * Description: Collection of user interaction functions for specific situations
 */


#define __PROMPT_C
#include <prompt.h>



/// Display the power plant entry data
void __displayPowerPlantEntry(PlantData *p_data) {
    printf("\n-- Power Plant --\n");
    printf("no: %d\n", p_data->no);
    printf("name: %s\n", p_data->name);

    // Check the fuel type to display
    switch(p_data->fuel) {
    case FUEL_TYPE_COAL:
        printf("fuel: coal\n");
        break;

    case FUEL_TYPE_SHALE_OIL:
        printf("fuel: shale oil\n");
        break;

    case FUEL_TYPE_GAS:
        printf("fuel: gas\n");
        break;

    case FUEL_TYPE_URANIUM:
        printf("fuel: uranium\n");
        break;

    case FUEL_TYPE_WATER:
        printf("fuel: water\n");
        break;

    case FUEL_TYPE_WIND:
        printf("fuel: wind\n");
        break;

    case FUEL_TYPE_GEOTHERMAL:
        printf("fuel: geothermal\n");
        break;
    
    default:
        break;
    }

    // Display rated capacity
    printf("rated capacity: %f\n", p_data->rated_cap);
}


/// Display log entry data, along with the id of the power plant it belongs to
void __displayLogEntry(LogEntry *p_entry) {
    // Format date into string
    char *date = __formatDate(&p_entry->date);

    // Print out all data about the log entry
    printf("--Log entry--\n"\
           "log id: %d\n"\
           "plant id: %d\n"\
           "production: %f\n"\
           "average sale price: %f\n"\
           "date: %s\n", 
           p_entry->log_id, p_entry->plant_no, p_entry->production, 
           p_entry->avg_sale_price, date);
}


/// Format date from Date structure into string yyyy-mm-dd format
/// NOTE: This function returns a pointer to stack allocated memory area,
/// which gets overwritten with every function call
char *__formatDate(Date *p_date) {
    // Char buffer for containing date information
    static char date[64] = {0};

    // Write year and hyphen to buffer
    sprintf(date, "%d-", p_date->year);
    
    // Check if zero should be added before month number
    if(p_date->month < 10)
        sprintf(date + 5, "0%d-", p_date->month);
    else sprintf(date + 5, "%d-", p_date->month);

    // Check if zero should be added before day number
    if(p_date->day < 10)
        sprintf(date + 8, "0%d", p_date->day);
    else sprintf(date + 8, "%d", p_date->day);

    return date;
}


/// Prompt user to solve duplicate entity instances
DuplicateEntryAction __promptDuplicateAction() {
    char sel = 0x00;

    while(true) {
        // Propose the user with options to choose from
        printf("Select any of the following options\n"\
               "a) Use only the first power plant\n"\
               "b) Use only the second power plant\n"\
               "c) Append the first power plant's id to the last id\n"\
               "d) Append the second power plant's id to the last id\n");

        // Read user input about the selection
        printf("Selection: ");
        fflush(stdout);
        scanf("%c", &sel);

        // Check if the read character is correctly between bounds
        if(sel >= 'a' && sel <= 'd')
            break;

        printf("Invalid option '%c', try again\n", sel);
    }

    return (DuplicateEntryAction) (sel - 'a');
}


/// Prompt the user about possible actions that can be taken, when
/// duplicate power plant entries were found
DuplicateEntryAction promptDuplicateEntries(PlantData *ent1, PlantData *ent2) {
    printf("The following power plant entities have the same id\n");

    // Print out the entities' information
    __displayPowerPlantEntry(ent1);
    __displayPowerPlantEntry(ent2);

    // Return the user selected action value
    return __promptDuplicateAction();
}


/// Prompt the user about possible actions that can be taken, when
/// duplicate log entries were found
DuplicateEntryAction promptDuplicateLogEntries(LogEntry *ent1, LogEntry *ent2) {
    printf("The following log entries have the same id\n");

    // Print out the entry values
    __displayLogEntry(ent1);
    __displayLogEntry(ent2);

    // Return the user selected action value
    return __promptDuplicateAction();
}
