/* File:        data_parser.h
 * Author:      Karl-Mihkel Ott
 * Created      2021-05-11
 * Last edit:   2021-05-11
 * Description: Function declarations for csv data parser
 */


#ifndef __DATA_PARSER_H
#define __DATA_PARSER_H


#ifdef __DATA_PARSER_C
    #include <stdio.h>
    #include <string.h>
    #include <stdlib.h>
    #include <stdint.h>

    #include <entity_data.h>
    #include <mem_check.h>
    #include <err_def.h>

    #define __DEFAULT_POWER_PLANT_CAP       16
    #define __DEFAULT_LOG_CAP               32
    #define __MAX_SEP_C                     4
    #define __MAX_STR_VAL_C                 5

    // This is mainly needed for avoiding buffer overflows, when parsing a single line
    #define __MAX_LINE_SIZE                 4096

    /// Boolean type declaration
    typedef uint8_t bool;
    #define true 1
    #define false 0
#endif

/// Possible CSV data type specifiers
/// Keep in mind that integer and floating point values are 64 bits wide
typedef enum CsvEntryType {
    CSV_ENTRY_TYPE_STRING   = 0,
    CSV_ENTRY_TYPE_INTEGER  = 1,
    CSV_ENTRY_TYPE_FLOAT    = 2
} CsvEntryType;


/// CSV single entry type
typedef struct CsvEntry {
    char *str_data;
    int64_t idata;
    double fdata;
    CsvEntryType entry_type;   
} CsvEntry;


/// CSV row type
typedef struct CsvRow {
    CsvEntry *entries;
    size_t cap;
    size_t n;
} CsvRow;


#ifdef __DATA_PARSER_C
    /// Read all file data into char buffer
    static void __readFileToBuffer(char *file_name, char **p_buf, size_t *p_len);

    /// Parse a single CSV line
    /// Line size is given as parameters
    static void __parseCSVRow(char *beg, char *end, char *file_name, uint32_t line, CsvRow *p_row);


    /// Parse all CSV rows and check if the csv table has constant amount of columns
    static void __parseCSVRows(char *buf, size_t buf_len, char *file_name, CsvRow **p_rows, size_t *p_row_c);


    /// Check the current entry and return integer when possible
    static int64_t __csvEntryRetrieveInteger(CsvEntry *p_entry);


    /// Check the current entry and return floating point number, when possible
    static double __csvEntryRetrieveFloat(CsvEntry *p_entry);


    /// Check the current entry for string value and return it, when possible
    static char *__csvEntryRetrieveString(CsvEntry *p_entry);


    /// Check the entry for string and try to parse fuel type out of it
    static FuelType __csvCheckFuelType(CsvEntry *p_entry);


    /// Check the entry for string date format and try to parse it into Date structure
    static Date __csvCheckDateType(char *file_name, CsvEntry *p_entry);
#endif



/// High level function to parse all data from csv power plant file
void parsePowerPlantFile(char *file_name, PowerPlants *p_plants);

/// High level function to parse all logs from the csv logs file
void parseLogsFile(char *file_name, PlantLogs *p_logs);

#endif
