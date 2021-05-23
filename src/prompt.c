/*
 * File:        action_prompt.c
 * Author:      Karl-Mihkel Ott
 * Created      2021-05-13
 * Last edit:   2021-05-13
 * Description: Collection of user interaction functions for specific situations
 */


#define __PROMPT_C
#include <prompt.h>


/// Create a table separator, that is termwidth wide and the string 
/// consists of underscores 
char *__mkTableSeparator() {
    // Find the terminal size, which is needed for formating the data 
    struct winsize ws;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);

    // Allocate memory for table separator
    char *sep = (char*) calloc(ws.ws_col + 1, sizeof(char));

    // Create a table separator 
    memset(sep, '_', ws.ws_col);
    sep[ws.ws_col] = 0x00;

    return sep;
}


/******** Data display functions *********/

/// Display log data for each given log
void displayLogData(PlantLogRefs *p_refs) {
    char *sep = __mkTableSeparator();

    // Display heading
    printf("List of all power plant logs: \n"\
           " | Log ID | Plant ID | Production (MWh) | Average Sale Price (Euros) | Date | \n");

    // For each log instance print its data
    for(size_t i = 0; i < p_refs->n; i++) {
        char buf[__DEFAULT_BUF_SIZE] = { 0 };
        char *date = formatDate(&p_refs->p_entries[i]->date);
        sprintf(buf, "  %d.  |  %d.  |  %0.2fMWh  |  %0.4f€ |  %s  ", p_refs->p_entries[i]->log_id,
            p_refs->p_entries[i]->plant_no, p_refs->p_entries[i]->production, p_refs->p_entries[i]->avg_sale_price,
            date);

        // Print the output data to stdout
        printf("%s\n%s\n%s\n", sep, buf, sep);
    }

    // Free the separator buffer
    free(sep);
}


/// Display power plant data for each given power plant
void displayPowerPlants(PowerPlantRefs *p_refs) {
    char *sep = __mkTableSeparator();

    // Print the table heading information
    printf(" | ID | Name | Fuel | Rated capacity | Average cost | Average utilisation | \n");

    // For each power plant, show its information
    for(size_t i = 0; i < p_refs->n; i++) {
        // Create a buffer for string data
        char *fuel = fuelTypeToStr(p_refs->p_plants[i]->fuel);
        char buf[__DEFAULT_BUF_SIZE] = { 0 };
        sprintf(buf, "  %d.  |  %s  |  %s  |  %0.2fMW  |  %0.2f€  |  %.2f%%  ",
            p_refs->p_plants[i]->no, p_refs->p_plants[i]->name, fuel ,p_refs->p_plants[i]->rated_cap,
            p_refs->p_plants[i]->avg_cost, p_refs->p_plants[i]->avg_utilisation);  

        // Print the output data to stdout
        printf("%s\n%s\n%s\n", sep, buf, sep);
    }

    // Free memory allocated for table separator
    free(sep);
}


/// Format date from Date structure into string yyyy-mm-dd format
/// NOTE: This function returns a pointer to stack allocated memory area,
/// which gets overwritten with every function call
char *formatDate(Date *p_date) {
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

    char buf[__DEFAULT_BUF_SIZE] = { 0 };

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

    // Construct temporary array for containing plantdata on stack
    PlantData data[2] = { *ent1, *ent2 };
    PowerPlantRefs plants = { .cap = 2, .n = 2, .p_plants = (PlantData**) &data };

    // Print out the entities' information
    displayPowerPlants(&plants);

    // Return the user selected action value
    return __promptDuplicateAction();
}


/// Prompt the user about possible actions that can be taken, when
/// duplicate log entries were found
DuplicateEntryAction promptDuplicateLogEntries(LogEntry *ent1, LogEntry *ent2) {
    printf("The following log entries have the same id\n");

    // Create LogDataRefs instance with given pointers on stack
    LogEntry *ents[] = { ent1, ent2 };
    PlantLogRefs refs = { .cap = 2, .n = 2, .p_entries = ents};

    // Print out the entry values
    displayLogData(&refs);

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
    pushToHashmap(&map, UNSEL_NEW UNSEL_SPECIFIER, strlen(UNSEL_NEW
        UNSEL_SPECIFIER), (void*) USER_INPUT_ACTION_U_NEW_POWER_PLANT);
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
    pushToHashmap(&map, SEL_UNSEL SEL_SPECIFIER, strlen(SEL_UNSEL
        SEL_SPECIFIER), (void*) USER_INPUT_ACTION_S_UNSEL_POWER_PLANT);

    // Push mode independent tokens to the hashmap
    pushToHashmap(&map, SAVE, strlen(SAVE), (void*) USER_INPUT_ACTION_SAVE);
    pushToHashmap(&map, EXIT, strlen(EXIT), (void*) USER_INPUT_ACTION_EXIT);

    return map;
}


/// Parse the user entry into enumeral
UserInputAction parseUserInputAction ( 
    Hashmap *tokens,
    char *in_str,
    bool is_sel,
    ListSortMode *p_sort,
    uint32_t *out_arg
) {
    UserInputAction act;
    *p_sort = LIST_SORT_MODE_UNKNOWN;

    // Allocate memory for command line arguments
    size_t cmd_arg_cap = 8;
    size_t cmd_arg_n = 0;
    char **cmd_args = (char**) calloc(cmd_arg_cap, sizeof(char*));
    
    // Skip all separators at the beginning of the string
    char *ptr = in_str;
    char *end = in_str + strlen(in_str);

    // Search for each command line argument separated by separator
    while(ptr < end) {
        // Skip all whitespaces and tabs from the beginning
        while(ptr < end && (*ptr == 0x20 || *ptr == 0x09))
            ptr++;

        // Find the end of the word
        char *word_end = strchr(ptr, 0x20);
        word_end = !word_end ? end : word_end;

        // Check if the word exists
        if(word_end - ptr) {
            // Allocate memory for the word
            cmd_args[cmd_arg_n] = (char*) calloc(word_end - ptr + 1, sizeof(char));
            
            // Copy word to args
            strncpy(cmd_args[cmd_arg_n], ptr, word_end - ptr);
            cmd_arg_n++;
        }

        ptr = word_end + 1;
    }

    // Find the requested action
    if(!cmd_args[0]) 
        return USER_INPUT_ACTION_UNKNOWN;
    else {
        size_t i = cmd_arg_n == 2 ? 1 : 2;
        act = __convertStrInputToEnum(tokens, cmd_args[0], is_sel, strlen(cmd_args[0]));

        // Check if the parsed action is power plant listing 
        if(act == USER_INPUT_ACTION_U_LIST_PLANTS) {
            if(cmd_arg_n == 1) 
                *p_sort = LIST_SORT_MODE_POW_ID_INCR;

            // Sorting is done in increasing order
            else if(cmd_arg_n == 2 || cmd_arg_n == 3 && !strcmp(cmd_args[1], "i")) {
                if(!strcmp(cmd_args[i], "plant_id")) 
                    *p_sort = LIST_SORT_MODE_POW_ID_INCR; 
                else if(!strcmp(cmd_args[i], "rated_cap"))
                    *p_sort = LIST_SORT_MODE_POW_CAP_INCR;
                else if(!strcmp(cmd_args[i], "avg_price"))
                    *p_sort = LIST_SORT_MODE_POW_COST_INCR;
                else if(!strcmp(cmd_args[i], "avg_util"))
                    *p_sort = LIST_SORT_MODE_POW_UTIL_INCR;
                else return USER_INPUT_ACTION_UNKNOWN;
            }

            // Sorting is done in decreasing order
            else if(cmd_arg_n == 3 && !strcmp(cmd_args[1], "d")) {
                if(!strcmp(cmd_args[i], "plant_id")) 
                    *p_sort = LIST_SORT_MODE_POW_ID_DECR; 
                else if(!strcmp(cmd_args[i], "rated_cap"))
                    *p_sort = LIST_SORT_MODE_POW_CAP_DECR;
                else if(!strcmp(cmd_args[i], "avg_price"))
                    *p_sort = LIST_SORT_MODE_POW_COST_DECR;
                else if(!strcmp(cmd_args[i], "avg_util"))
                    *p_sort = LIST_SORT_MODE_POW_UTIL_DECR;
                else return USER_INPUT_ACTION_UNKNOWN;
            }
        }

        // Check if the parsed action is log listing
        else if(act == USER_INPUT_ACTION_U_SHOW_LOGS || act == USER_INPUT_ACTION_S_LIST_LOGS) {
            if(cmd_arg_n == 1)
                *p_sort = LIST_SORT_MODE_LOG_ID_INCR;

            // Sorting is done in increasing order
            else if(cmd_arg_n == 2 || cmd_arg_n == 3 && !strcmp(cmd_args[1], "i")) {
                if(!strcmp(cmd_args[i], "log_id")) 
                    *p_sort = LIST_SORT_MODE_LOG_ID_INCR; 
                else if(!strcmp(cmd_args[i], "plant_id"))
                    *p_sort = LIST_SORT_MODE_LOG_PLANT_ID_INCR;
                else if(!strcmp(cmd_args[i], "production"))
                    *p_sort = LIST_SORT_MODE_LOG_PRODUCTION_INCR;
                else if(!strcmp(cmd_args[i], "price"))
                    *p_sort = LIST_SORT_MODE_LOG_SALE_PRICE_INCR;
                else return USER_INPUT_ACTION_UNKNOWN;
            }

            // Sorting is done in decreasing order
            else if(cmd_arg_n == 3 && !strcmp(cmd_args[1], "d")) {
                if(!strcmp(cmd_args[i], "log_id")) 
                    *p_sort = LIST_SORT_MODE_LOG_ID_DECR; 
                else if(!strcmp(cmd_args[i], "plant_id"))
                    *p_sort = LIST_SORT_MODE_LOG_PLANT_ID_DECR;
                else if(!strcmp(cmd_args[i], "production"))
                    *p_sort = LIST_SORT_MODE_LOG_PRODUCTION_DECR;
                else if(!strcmp(cmd_args[i], "price"))
                    *p_sort = LIST_SORT_MODE_LOG_SALE_PRICE_DECR;
                else return USER_INPUT_ACTION_UNKNOWN;
            }
        }
        
        // Check if argument is required
        else if(cmd_arg_n == 2)
            *out_arg = atoi(cmd_args[1]);
    }

    // Clean all allocated memory for command strings
    for(size_t i = 0; i < cmd_arg_n; i++)
        free(cmd_args[i]);
    free(cmd_args);

    return act;
}


/********** New powerplant creation prompts *********/

/// Prompt the user about new power plant fuel type
FuelType __promptNewPowerPlantFuelType(FuelType *old) {
    // Prompt the user until he enters correct fuel type string
    while(true) {
        // Check if the keepable option should be presented
        if(old)
            printf("Enter new power plant's fuel type (!k: keep the current value): ");
        else printf("Enter new power plant's fuel type: ");
        fflush(stdout);
        
        // Read user input
        char in_buf[__DEFAULT_BUF_SIZE] = { 0 };
        fgets(in_buf, __DEFAULT_BUF_SIZE, stdin);
        in_buf[strlen(in_buf) - 1] = 0x00;

        // Check if the old value should be used
        if(old && !strcmp(in_buf, "!k"))
            return *old;
        
        // Convert the string to FuelType instance
        FuelType ft = strToFuelType(in_buf);
        if(ft != FUEL_TYPE_UNKNOWN)
            return ft;

        printf("Invalid fuel type '%s'\n", in_buf);
        printf("Try again\n\n");
    }
}


/// Prompt the user about new power plant name
char *__promptNewPowerPlantNameValue(char *old) {
    // Allocate memory for name buffer
    char *name = (char*) calloc(__DEFAULT_NAME_LEN, sizeof(char));

    // Check if keeping the old values should be considered
    if(old) printf("Enter new power plant name (!k: keep previous value): ");
    else printf("Enter new power plant name: ");
    fflush(stdout);

    // Retrieve user input
    fgets(name, __DEFAULT_NAME_LEN - 1, stdin);
    name[strlen(name) - 1] = 0x00;

    // Check if the name should be kept old one if specified
    if(old && !strcmp(name, "!k"))
        return old;

    return name;
}


/********** New log creation prompts **********/

/// Prompt the user about log's date info
Date __promptNewLogDate(Date *old) {
    // Prompt the user until he enters correct date
    while(true) {
        if(old)
            printf("Enter date in 'yyyy-mm-dd' format (!k: keep current, !t: use date today): ");
        else printf("Enter date in 'yyyy-mm-dd' format (!t: use date today): ");
        fflush(stdout);

        // Read user input
        char in_buf[__DEFAULT_BUF_SIZE] = { 0 };
        fgets(in_buf, __DEFAULT_BUF_SIZE, stdin);
        in_buf[strlen(in_buf) - 1] = 0x00;

        if(old && !strcmp(in_buf, "!k"))
            return *old;
        else if(!strcmp(in_buf, "!t")) {
            time_t ti = time(NULL);
            struct tm ts = *localtime(&ti);

            return (Date) { 
                .year = (uint16_t) ts.tm_year + 1900, 
                .month = (uint16_t) ts.tm_mon + 1,
                .day = (uint16_t) ts.tm_mday
            };
        }

        // Try to parse user input into Date format
        if(in_buf[4] == '-' && in_buf[7] == '-') {
            // Create string represantations of year, month and day
            char year[8] = { 0 };
            char month[4] = { 0 };
            char day[4] = { 0 };

            // Copy substrings to their appropriate string instances
            strncpy(year, in_buf, 4);
            strncpy(month, in_buf + 5, 2);
            strncpy(day, in_buf + 8, 2);

            // Check if the string year, month and day are parseable to int
            if(numcheck(year, 4) && numcheck(month, 2) && numcheck(day, 2)) {
                return (Date) {
                    .year = (uint16_t) atoi(year),
                    .month = (uint16_t) atoi(month),
                    .day = (uint16_t) atoi(day)
                };
            }
        }

        printf("Invalid date format\n"\
               "Try again\n\n");
    }
}


/********** Generic prompts ***********/

/// Prompt the user until he enters correct floating point value
float __promptFloatValue(char *msg, float *old) {
    // Prompt the user untile he enters correct plant rating 
    while(true) {
        // Check if keepable options should be presented
        if(old) printf("%s (!k: keep current value): ", msg);
        else printf("%s: ", msg);
        fflush(stdout);

        // Read user input
        char in_buf[__DEFAULT_BUF_SIZE] = { 0 };
        fgets(in_buf, __DEFAULT_BUF_SIZE, stdin);
        in_buf[strlen(in_buf) - 1] = 0x00;

        // Check if the user input is to keep current data
        if(old && !strcmp(in_buf, "!k"))
            return *old;

        // Check if the user entered string can be converted into float
        if(floatcheck(in_buf, strlen(in_buf)))
            return (float) atof(in_buf);

        printf("Invalid input, expected a floating point number\n");
        printf("Try again\n\n");
    }
}


/// Prompt the user until he enters correct id or lets it be autogenerated
uint32_t __promptIdValue(char *msg, size_t *p_max_id, Hashmap *p_map) {
    uint32_t id = UINT32_MAX;

    // Until no correct number or no auto gen flag is provided prompt the 
    // user about the plant numbering
    while(true) {
        // Recieve the user input
        printf("%s (a for autogen): ", msg);
        fflush(stdout);
        char str_no[__DEFAULT_BUF_SIZE] = { 0 };
        fgets(str_no, __DEFAULT_BUF_SIZE, stdin);
        str_no[strlen(str_no) - 1] = 0x00;

        // Check if id autogen should be used
        if(!strcmp(str_no, "a")) {
            (*p_max_id)++;
            id = (uint32_t) *p_max_id;
            break;
        }

        // Check if only ascii numbers are used in input
        if(numcheck(str_no, strlen(str_no))) {
            // Get the integer value of given number
            uint32_t no = atoi(str_no);

            // Check if the value does not collide with already existing one
            if(!findValue(p_map, &no, sizeof(uint32_t))) {
                id = no;
                
                // Check if the new id is bigger than the current maximum id
                if(id > *p_max_id) *p_max_id = id;
                break;
            }
        }

        else {
            printf("Please enter a valid number!\n");
            continue;
        }

        printf("ID %s is already used\n", str_no);
        printf("Try again\n\n");
    }

    return id;
}


/// Prompt the user for information about a new power plant instance
PlantData promptNewPowerPlant(size_t *p_max_id, Hashmap *p_map) {
    PlantData data = { 0 };
    data.no = __promptIdValue("Enter new power plant id value", p_max_id, p_map);
    data.name = __promptNewPowerPlantNameValue(NULL);
    data.fuel = __promptNewPowerPlantFuelType(NULL);
    data.rated_cap = __promptFloatValue("Enter new rated capacity", 
        false);

    printf("Successfully created new power plant '%s'\n", data.name);
    return data;
}


/// Prompt the user for information about editing a power plant instance
void promptEditPowerPlant(PlantData *data) {
    PlantData new_dat = {};
    new_dat.no = data->no;
    new_dat.name = __promptNewPowerPlantNameValue(data->name);
    new_dat.fuel = __promptNewPowerPlantFuelType(&data->fuel);
    new_dat.rated_cap = __promptFloatValue("Enter new rated capacity", 
        &data->rated_cap);
    new_dat.avg_cost = data->avg_cost;
    new_dat.logs = data->logs;

    // Check if the previous name instance memory must be freed
    if(new_dat.name != data->name)
        free(data->name);

    *data = new_dat;

    // Calculate new average utilisation and cost
    calcAvgUtilisation(data);
}


/// Prompt the user for information about editing a log instance
void promptEditLog(LogEntry *log) {
    log->log_id = log->log_id;
    log->plant_no = log->plant_no;
    log->production = __promptFloatValue("Enter energy production amount (MWh)", 
        &log->production);
    log->avg_sale_price = __promptFloatValue("Enter average sale price for that day", 
        &log->avg_sale_price);
    log->date = __promptNewLogDate(&log->date);
}


/// Prompt the user to create a new log entry
LogEntry promptNewLogEntry(Hashmap *p_map, size_t *p_max_id, uint32_t sel_id) {
    LogEntry entry = { 0 };
    entry.log_id = __promptIdValue("Enter new log id value", p_max_id, p_map);
    entry.plant_no = sel_id;

    // Temporary value
    entry.ref_ind = UINT32_MAX;
    entry.avg_sale_price = __promptFloatValue("Enter average price for the day", NULL);
    entry.production = __promptFloatValue("Enter energy production amount (MWh)", NULL); 
    entry.date = __promptNewLogDate(NULL);

    return entry;
}
