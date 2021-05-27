/*
 * File:        log.c
 * Author:      Karl-Mihkel Ott
 * Created      2021-05-27
 * Last edit:   2021-05-27
 * Description: Logging function definitions for energy_manager program
 */

#define __LOG_C
#include <log.h>


/// Create new formated runtime length specifying string
/// NOTE: The pointer returned is a pointer to a static variable
char *__mkRuntimeLengthStr(time_t ti) {
    static char buf[__MAX_BUF_SIZE] = { 0 };
    time_t delta = time(NULL) - ti;

    // Find hours, minutes and seconds passed since the start of the program
    time_t hours = (delta %= 3600) / 3600;
    time_t minutes = (delta %= 60) / 60;
    time_t seconds = delta;

    // Write the human readable timestamp string
    sprintf(buf, "[%02lu:%02lu:%02lu] ", hours, minutes, seconds);
    return buf;
}


/// Create a new file stream for logging the program
FILE *newLogger(time_t ti) {
    // Find the file name
    char name[__MAX_BUF_SIZE] = { 0 };
    sprintf(name, "%lu.log", ti);

    // Try opening the file for writing
    FILE *file = fopen(name, "wb");
    
    // Check for file opening errors
    if(!file) FOPEN_ERR(name);

    return file;
}


/// Log user command to file 
void logCommandAction (
    FILE *file, 
    UserInputAction act, 
    size_t id_arg, 
    time_t ti
) {
    char *msg = NULL;

    // Get and write the current runtime length string
    char *run = __mkRuntimeLengthStr(ti);
    bool req_clean = false;
    fwrite(run, sizeof(char), strlen(run), file);

    // Check the command type and write to the log
    switch(act) {
    case USER_INPUT_ACTION_U_SHOW_HELP:
        msg = "Showing help for unselected mode\n";
        break;

    case USER_INPUT_ACTION_U_NEW_POWER_PLANT:
        msg = (char*) calloc(__MAX_BUF_SIZE, sizeof(char));
        sprintf(msg, "Created new power plant instance with id %lu\n", id_arg);
        req_clean = true;
        break;

    case USER_INPUT_ACTION_U_LIST_PLANTS:
        msg = "Listing power plant instances\n";
        break;

    case USER_INPUT_ACTION_U_LIST_LOGS:
        msg = "Listing all logs from unselected mode\n";
        break;

    case USER_INPUT_ACTION_U_EDIT_POWER_PLANT:
        msg = (char*) calloc(__MAX_BUF_SIZE, sizeof(char));
        sprintf(msg, "Modified power plant instance nr %lu\n", id_arg);
        break;

    case USER_INPUT_ACTION_U_DELETE_POWER_PLANT:
        msg = (char*) calloc(__MAX_BUF_SIZE, sizeof(char));
        sprintf(msg, "Deleted power plant instance nr %lu\n", id_arg);
        req_clean = true;
        break;

    case USER_INPUT_ACTION_U_SELECT_POWER_PLANT:
        msg = (char*) calloc(__MAX_BUF_SIZE, sizeof(char));
        sprintf(msg, "Selected power plant instance nr %lu\n", id_arg);
        req_clean = true;
        break;

    case USER_INPUT_ACTION_S_SHOW_HELP:
        msg = "Showing help for selected mode";
        break;

    case USER_INPUT_ACTION_S_NEW_LOG:
        msg = (char*) calloc(__MAX_BUF_SIZE, sizeof(char));
        sprintf(msg, "Created a new log instance with id %lu\n", id_arg);
        req_clean = true;
        break;

    case USER_INPUT_ACTION_S_LIST_LOGS:
        msg = (char*) calloc(__MAX_BUF_SIZE, sizeof(char));
        sprintf(msg, "Listing all logs for power plant nr %lu\n", id_arg);
        req_clean = true;
        break;

    case USER_INPUT_ACTION_S_EDIT_LOG:
        msg = (char*) calloc(__MAX_BUF_SIZE, sizeof(char));
        sprintf(msg, "Edited a log with id %lu\n", id_arg);
        req_clean = true;
        break;

    case USER_INPUT_ACTION_S_DELETE_LOG:
        msg = (char*) calloc(__MAX_BUF_SIZE, sizeof(char));
        sprintf(msg, "Deleted a log with id %lu\n", id_arg);
        req_clean = true;
        break;

    case USER_INPUT_ACTION_S_UNSEL_POWER_PLANT:
        msg = (char*) calloc(__MAX_BUF_SIZE, sizeof(char));
        sprintf(msg, "Unselected power plant nr %lu\n", id_arg);
        req_clean = true;
        break;

    case USER_INPUT_ACTION_SAVE:
        msg = "Saved data to files\n";
        break;

    case USER_INPUT_ACTION_EXIT:
        msg = "Exiting the program\n";
        break;

    default: 
        return;

    }

    // Write the message to the log file
    fwrite(msg, sizeof(char), strlen(msg), file);
    
    // Check if cleanup is necessary
    if(req_clean) free(msg);
}


/// Log provided miscellaneous information
void logMiscInfo(FILE *file, char *info, time_t ti) {
    char *run = __mkRuntimeLengthStr(ti);
    fwrite(run, sizeof(char), strlen(run), file);
    fwrite(info, sizeof(char), strlen(info), file);
}


/// Destroy a current log file stream
void destroyLogger(FILE *file) {
    fclose(file);
}
