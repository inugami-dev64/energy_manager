/*
 * File:        err_def.h
 * Author:      Karl-Mihkel Ott
 * Created      2021-05-11
 * Last edit:   2021-05-11
 * Description: Define error definition macros that print error messages to stderr and 
 *              exit the program with code 1
 */


#ifndef __ERR_DEF_H
#define __ERR_DEF_H

/// IO error handling macros
#define FOPEN_ERR(file_name)                        fprintf(stderr, "Failed to open file: %s\n", file_name), \
                                                    exit(EXIT_FAILURE)

#define FREAD_ERR(file_name)                        fprintf(stderr, "Failed to read from file: %s\n", file_name), \
                                                    exit(EXIT_FAILURE)

#define FWRITE_ERR(file_name)                       fprintf(stderr, "Failed to write into file: %s\n", file_name), \
                                                    exit(EXIT_FAILURE)


/// Line parsing error macros
#define LINE_LENGTH_ERR(file, lc)                   fprintf(stderr, "Error, too long line in file %s, line %d\n", file, lc), \
                                                    exit(EXIT_FAILURE)

#define LINE_GREATER_VAL_C_ERR(file, lc)            fprintf(stderr, "Error, too many values in file %s, line %d\n", file, lc), \
                                                    exit(EXIT_FAILURE)

#define INCONSISTANT_FIELD_C_ERR(file, lc)          fprintf(stderr, "Error, CSV field count is not constant in file %s, line: %d\n", file, lc), \
                                                    exit(EXIT_FAILURE)

/// Data entry parsing errors
#define INVALID_ARG_C(file)                         fprintf(stderr, "The record count in file '%s' does not match to the required record count for power plant information\n", file), \
                                                    exit(EXIT_FAILURE)

#define INVALID_DATE_FORMAT(file, date)             fprintf(stderr, "Invalid date format '%s' in file '%s', date must be presented in YYYY-MM-DD format", date, file), \
                                                    exit(EXIT_FAILURE)

#endif
