/*
 * File:        data_parser.c
 * Author:      Karl-Mihkel Ott
 * Created      2021-05-11
 * Last edit:   2021-05-11
 * Description: Function definitions for csv data parser
 */


#define __DATA_PARSER_C
#include <data_parser.h>

/// Read from file to char buffer
void __readFileToBuffer(char *file_name, char **p_buf, size_t *p_len) {
    // Open file for reading
    FILE *file = fopen(file_name, "rb");

    // Check if the file opening was unsuccessful
    if(!file) FOPEN_ERR(file_name);

    // Find the file size
    fseek(file, 0, SEEK_END);
    *p_len = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate memory for char buffer
    (*p_buf) = (char*) calloc((*p_len) + 1, sizeof(char));

    // Read file contents into 
    size_t res = fread(*p_buf, sizeof(char), *p_len, file);

    // Check if the file reading was successful
    if(!res) FREAD_ERR(file_name);

    // Close the file stream
    fclose(file);
}


/// Parse a single CSV file row (O(2n))
void __parseCSVRow(char *beg, char *end, char *file_name, uint32_t line, CsvRow *p_row) {
    // Check if the line length is appropriate
    if(end - beg >= __MAX_LINE_SIZE)
        LINE_LENGTH_ERR(file_name, line);

    // Array of all pointers to separators
    char *sep[__MAX_SEP_C];
    size_t sep_c = 0;

    // Array for all quotation mark pointer locations
    char *strs[__MAX_STR_VAL_C][2] = { { 0 } };
    size_t str_c = 0;

    // Find all separator instances
    char *cur = beg;
    bool close_str = false;
    while(cur < end) {
        // Found string identifier symbol
        if(*cur == '\"')
            close_str = !close_str;

        // Found comma separator that is not between string identifier
        else if((*cur == ',' || *cur == ';') && !close_str) {
            // Check if the comma separator is not out of bounds
            if(sep_c >= __MAX_SEP_C)
                LINE_VAL_C_ERR(file_name, line);

            // Add the separator to its array
            sep[sep_c] = cur;
            sep_c++;
        }

        cur++;
    }

    // Allocate initial amount of memory for row entries
    p_row->cap = __MAX_SEP_C + 1;
    p_row->n = 0;
    p_row->entries = (CsvEntry*) calloc(p_row->cap, sizeof(CsvEntry));

    // For each separator instance check the values inbetween
    for(size_t i = 0; i < sep_c + 1; i++) {
        char *br = end;
        char buf[__MAX_LINE_SIZE] = { 0 };
        size_t buf_len = 0;

        // Set the begin and end pointers
        if(i) beg = sep[i - 1] + 1;
        if(i < sep_c) br = sep[i];
        else br = end;

        // Copy entity data to buffer
        strncpy(buf, beg, br - beg);
        buf_len = strlen(buf);

        bool is_num = true;
        bool is_float = true;

        // Check if the entry is either a integer, a float or both
        for(size_t j = 0; j < buf_len; j++) { 
            if(buf[j] < '0' || buf[j] > '9') {
                is_num = false;
                if(buf[j] != '.') {
                    is_float = false;
                    break;
                }
            }
        }

        // Assign quoted string value
        if(buf_len > 2 && *buf == '\"' && buf[buf_len - 1] == '\"') {
            p_row->entries[p_row->n].entry_type = CSV_ENTRY_TYPE_STRING;
            p_row->entries[p_row->n].str_data = (char*) calloc(buf_len, sizeof(char));
            strncpy(p_row->entries[p_row->n].str_data, buf + 1, buf_len - 2);
            p_row->n++;
        }

        // Assign integer value
        else if(is_num) {
            p_row->entries[p_row->n].entry_type = CSV_ENTRY_TYPE_INTEGER;
            p_row->entries[p_row->n].idata = atoll(buf);
            p_row->n++;
        }

        // Assign floating point value
        else if(is_float) {
            printf("Assigning floating point value\n");
            p_row->entries[p_row->n].entry_type = CSV_ENTRY_TYPE_FLOAT;
            p_row->entries[p_row->n].fdata = atof(buf);
            p_row->n++;
        }

        // If any of the type conditions were not met, set the value as string terminated by nearest
        // whitespace or comma
        else if(buf_len) {
            char *buf_end = strchr(buf, 0x20);
            buf_end = !buf_end ? buf + buf_len : buf_end;

            // Allocate memory and copy the string
            p_row->entries[p_row->n].entry_type = CSV_ENTRY_TYPE_STRING;
            p_row->entries[p_row->n].str_data = (char*) calloc(buf_end - buf + 1, sizeof(char));
            strncpy(p_row->entries[p_row->n].str_data, buf, buf_end - buf);
            p_row->n++;
        }
    }
}


/// Parse all CSV rows and check if the csv table has constant amount of columns
void __parseCSVRows(char *buf, size_t buf_len, char *file_name, CsvRow **p_rows, size_t *p_row_c) {
    char *cur = buf;
    char *next = NULL;
    char *end = buf;

    // Allocate initial amount of memory for csv rows
    size_t row_cap = __DEFAULT_POWER_PLANT_CAP;
    *p_rows = (CsvRow*) malloc(row_cap * sizeof(CsvRow));

    // While the current reading pointer is not over the buffer, parse line
    uint32_t line = 1;
    while(cur < buf + buf_len) {
        end = strchr(cur, 0x0a);
        end = !end ? buf + buf_len : end;

        // Check if memory reallocation is needed
        reallocCheck((void**) p_rows, sizeof(CsvRow), (*p_row_c) + 1, &row_cap);

        // Parse the current line and set the value accordingly
        __parseCSVRow(cur, end, file_name, line, ((*p_rows) + (*p_row_c)));

        // Check if the previous row entity count does not match the current one and throw error
        if(line != 1 && (*p_rows)[(*p_row_c)].n != (*p_rows)[(*p_row_c) - 1].n)
            INCONSISTANT_FIELD_C_ERR(file_name, line);

        line++;
        (*p_row_c)++;
        cur = end + 1;
    }
}


/// High level function to parse all data from csv power plant file
void parsePowerPlantFile(char *file_name, PowerPlants *p_plants) {
    char *buf = NULL;
    size_t offset = 0;
    size_t len = 0;

    // First read the file data into char buffer   
    __readFileToBuffer(file_name, &buf, &len);

    char *cur = buf;
    char *end = buf;

    CsvRow *rows = NULL;
    size_t row_c = 0;
    __parseCSVRows(buf, len, file_name, &rows, &row_c);

    // Allocate reserve memory for power plants since it is not known how many plants would be in the file 
    p_plants->cap = row_c < __DEFAULT_POWER_PLANT_CAP ? __DEFAULT_POWER_PLANT_CAP : __roundToBase2(row_c << 1);
    p_plants->n = 0;
    p_plants->plants = (PlantData*) malloc(p_plants->cap * sizeof(PlantData));
    
    for(size_t i = 0; i < row_c; i++) {
        for(size_t j = 0; j < rows[i].n; j++) {
            
        }

        // Free the row data
        free(rows[i].entries);
    }

    free(rows);

    // Free the allocated char buffer
    free(buf);
}
