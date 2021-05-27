/*
 * File:        log.c
 * Author:      Karl-Mihkel Ott
 * Created      2021-05-27
 * Last edit:   2021-05-27
 * Description: Function declarations for logging commands and program
 */


#ifndef __LOG_H
#define __LOG_H


#ifdef __LOG_C
    #define __MAX_BUF_SIZE      256     
    #include <stdio.h>
    #include <time.h>
    #include <string.h>
    #include <stdlib.h>
    #include <stdint.h>
    #include <stdbool.h>
    
    #include <err_def.h>
    #include <hashmap.h>
    #include <entity_data.h>
    #include <act_impl.h>
    #include <prompt.h>

    
    /// Create new formated runtime length specifying string
    /// NOTE: The pointer returned is a pointer to a static variable
    static char *__mkRuntimeLengthStr(time_t ti);
#endif


/// Create a new file stream for logging the program
FILE *newLogger(time_t ti);


/// Log user command to file 
void logCommandAction(FILE *file, UserInputAction act, size_t id_arg, time_t ti);


/// Log provided miscellaneous information
void logMiscInfo(FILE *file, char *info, time_t ti);


/// Destroy a current log file stream
void destroyLogger(FILE *file);

#endif
