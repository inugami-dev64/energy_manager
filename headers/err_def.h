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

#define LINE_VAL_C_ERR(file, lc)                    fprintf(stderr, "Error, too many values in file %s, line %d\n", file, lc), \
                                                    exit(EXIT_FAILURE)

#define INCONSISTANT_FIELD_C_ERR(file, lc)          fprintf(stderr, "Error, CSV field count is not constant in file %s, line: %d\n", file, lc), \
                                                    exit(EXIT_FAILURE)

#endif
