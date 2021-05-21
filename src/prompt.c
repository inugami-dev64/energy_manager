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


/// Convert string command into command enumeral
static UserInputAction __convertStrInputToEnum(Hashmap *tokens, char *in_str, bool is_sel, size_t len) {
    // Check if the command is mode independent
    if(!len) return USER_INPUT_ACTION_UNKNOWN;
    else if(!strncmp(in_str, SAVE, len))
        return USER_INPUT_ACTION_SAVE;
    else if(!strncmp(in_str, EXIT, len))
        return USER_INPUT_ACTION_EXIT;

    char buf[__DEFAULT_BUF_LEN] = { 0 };

    // Check if the energy_manager has "selected" mode
    if(is_sel) {
        // Create a copy of the key with "selected" specifier
        strncpy(buf, in_str, len);
        sprintf(buf + len, "%s", SEL_SPECIFIER);
        len += strlen(SEL_SPECIFIER);
    }

    else {
        // Create a copy of the key with "unselected" specifier
        strncpy(buf, in_str, len);
        sprintf(buf + len, "%s", UNSEL_SPECIFIER);
        len += strlen(UNSEL_SPECIFIER);
    }

    // Return the pointer value which should be castable to UserInputAction 
    // enumeral
    return (UserInputAction) ((uint64_t) findValue(tokens, buf, len));
}


/// Prompt the user about possible actions that can be taken, when
/// duplicate power plant entries were found
DuplicateEntryAction promptDuplicatePowerPlantEntries(PlantData *ent1, PlantData *ent2) {
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


/// Create a token hashmap for all possible user input data
Hashmap tokeniseUserInput() {
    Hashmap map = {};
    newHashmap(&map, __roundToBase2(2 * USER_INPUT_ACTION_ENUM_C));

    // Push unselected mode tokens into map
    // PS! Action enumeral value will be stored as a pointer that must 
    // never be dereferenced, instead the pointer's value should be used instead
    pushToHashmap(&map, UNSEL_HELP UNSEL_SPECIFIER, strlen(UNSEL_HELP
        UNSEL_SPECIFIER), (void*) USER_INPUT_ACTION_U_SHOW_HELP);
    pushToHashmap(&map, UNSEL_NEW SEL_SPECIFIER, strlen(UNSEL_NEW
        SEL_SPECIFIER), (void*) USER_INPUT_ACTION_U_NEW_POWER_PLANT);
    pushToHashmap(&map, UNSEL_LIST UNSEL_SPECIFIER, strlen(UNSEL_LIST
        UNSEL_SPECIFIER), (void*) USER_INPUT_ACTION_U_LIST_PLANTS);
    pushToHashmap(&map, UNSEL_LOG UNSEL_SPECIFIER, strlen(UNSEL_LOG
        UNSEL_SPECIFIER), (void*) USER_INPUT_ACTION_U_SHOW_LOGS);
    pushToHashmap(&map, UNSEL_EDIT UNSEL_SPECIFIER, strlen(UNSEL_EDIT
        UNSEL_SPECIFIER), (void*) USER_INPUT_ACTION_U_EDIT_POWER_PLANT);
    pushToHashmap(&map, UNSEL_DEL UNSEL_SPECIFIER, strlen(UNSEL_DEL
        UNSEL_SPECIFIER), (void*) USER_INPUT_ACTION_U_DELETE_POWER_PLANT);
    pushToHashmap(&map, UNSEL_SEL UNSEL_SPECIFIER, strlen(UNSEL_SEL
        UNSEL_SPECIFIER), (void*) USER_INPUT_ACTION_U_SELECT_POWER_PLANT);

    // Push selected mode tokens into the map
    pushToHashmap(&map, SEL_HELP SEL_SPECIFIER, strlen(SEL_HELP
        SEL_SPECIFIER), (void*) USER_INPUT_ACTION_S_SHOW_HELP);
    pushToHashmap(&map, SEL_NEW SEL_SPECIFIER, strlen(SEL_NEW
        SEL_SPECIFIER), (void*) USER_INPUT_ACTION_S_NEW_LOG);
    pushToHashmap(&map, SEL_LIST SEL_SPECIFIER, strlen(SEL_LIST
        SEL_SPECIFIER), (void*) USER_INPUT_ACTION_S_LIST_LOGS);
    pushToHashmap(&map, SEL_EDIT SEL_SPECIFIER, strlen(SEL_EDIT
        SEL_SPECIFIER), (void*) USER_INPUT_ACTION_S_EDIT_LOG);
    pushToHashmap(&map, SEL_DEL SEL_SPECIFIER, strlen(SEL_DEL
        SEL_SPECIFIER), (void*) USER_INPUT_ACTION_S_DELETE_LOG);

    // Push mode independent tokens to the hashmap
    pushToHashmap(&map, SAVE, strlen(SAVE), (void*) USER_INPUT_ACTION_SAVE);
    pushToHashmap(&map, EXIT, strlen(EXIT), (void*) USER_INPUT_ACTION_EXIT);

    return map;
}


/// Parse the user entry into enumeral
UserInputAction parseUserInputAction(Hashmap *tokens, char *in_str, bool is_sel, uint32_t *out_arg) {
    UserInputAction act;
    
    // Skip all separators at the beginning of the string
    char *ptr = in_str;
    char *end = in_str + strlen(in_str);
    while(ptr < end && (*ptr == 0x20 || *ptr == 0x09))
        ptr++;

    // Find the end of the first word
    char *word_end = strchr(ptr, 0x20);
    word_end = !word_end ? end : word_end;

    // Check if command argument value exists
    if(word_end != end) {
        // Skip all separators
        char *arg_beg = word_end;
        while(arg_beg < end && (*arg_beg == 0x20 || *arg_beg == 0x09))
            arg_beg++;

        // Set the argument value if possible
        if(arg_beg < end)
            *out_arg = atoi(arg_beg);
    }
    
    // Find the requested action
    return __convertStrInputToEnum(tokens, ptr, is_sel, word_end - ptr);
}
