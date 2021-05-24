/*
 * File:        data_parser.c
 * Author:      Karl-Mihkel Ott
 * Created      2021-05-11
 * Last edit:   2021-05-13
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
                LINE_GREATER_VAL_C_ERR(file_name, line);

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


/// Check the current entry and return integer when possible
int64_t __csvEntryRetrieveInteger(CsvEntry *p_entry) {
    // Check the entry type
    switch(p_entry->entry_type) {
    case CSV_ENTRY_TYPE_STRING:
        fprintf(stderr, "Invalid entry type, got string but expected integer\n");
        exit(EXIT_FAILURE);
        break;

    case CSV_ENTRY_TYPE_FLOAT: 
        return (int64_t) p_entry->fdata;

    case CSV_ENTRY_TYPE_INTEGER:
        return p_entry->idata;
    }
}

/// Check the current entry and return floating point number, when possible
double __csvEntryRetrieveFloat(CsvEntry *p_entry) {
    // Check the entry type
    switch(p_entry->entry_type) {
    case CSV_ENTRY_TYPE_STRING:
        fprintf(stderr, "Invalid entry type, got string but expected float\n");
        exit(EXIT_FAILURE);
        break;

    case CSV_ENTRY_TYPE_FLOAT: 
        return p_entry->fdata;

    case CSV_ENTRY_TYPE_INTEGER:
        return (double) p_entry->idata;
    }
}

/// Check the current entry for string value and return it, when possible
char *__csvEntryRetrieveString(CsvEntry *p_entry) {
    // Check the entry type
    switch(p_entry->entry_type) {
    case CSV_ENTRY_TYPE_STRING: {
        // Allocate new memory and copy string value to it
        char *new_str = (char*) calloc(strlen(p_entry->str_data) + 1, sizeof(char));
        strcpy(new_str, p_entry->str_data);
        return new_str;
    }

    case CSV_ENTRY_TYPE_FLOAT: 
        fprintf(stderr, "Invalid entry type, got float but expected string\n");
        exit(EXIT_FAILURE);
        break;

    case CSV_ENTRY_TYPE_INTEGER:
        fprintf(stderr, "Invalid entry type, got integer but expected string\n");
        exit(EXIT_FAILURE);
        break;
    }
}

/// Check the entry for string and try to parse fuel type out of it
FuelType __csvCheckFuelType(CsvEntry *p_entry) {
    // Check the entry type
    switch(p_entry->entry_type) {
    case CSV_ENTRY_TYPE_STRING:
        // Check the keyword and return appropriate enumeral fuel type value
        if(!strcmp(p_entry->str_data, "coal"))
            return FUEL_TYPE_COAL;
        else if(!strcmp(p_entry->str_data, "shaleoil"))
            return FUEL_TYPE_SHALE_OIL;
        else if(!strcmp(p_entry->str_data, "gas"))
            return FUEL_TYPE_GAS;
        else if(!strcmp(p_entry->str_data, "uranium"))
            return FUEL_TYPE_URANIUM;
        else if(!strcmp(p_entry->str_data, "water"))
            return FUEL_TYPE_WATER;
        else if(!strcmp(p_entry->str_data, "wind"))
            return FUEL_TYPE_WIND;
        else if(!strcmp(p_entry->str_data, "geothermal"))
            return FUEL_TYPE_GEOTHERMAL;
        // If no appropriate keyword was found then throw error
        else {
            fprintf(stderr, "Invalid fuel type specifier %s\n", p_entry->str_data);
            exit(EXIT_FAILURE);
        }
        break;

    case CSV_ENTRY_TYPE_FLOAT: 
        fprintf(stderr, "Invalid entry type, got float but expected string\n");
        exit(EXIT_FAILURE);
        break;

    case CSV_ENTRY_TYPE_INTEGER:
        fprintf(stderr, "Invalid entry type, got integer but expected string\n");
        exit(EXIT_FAILURE);
        break;
    }
}


/// Check the entry for string date format and try to parse it into Date structure
Date __csvCheckDateType(char *file_name, CsvEntry *p_entry) {
    // Buffer variable declarations
    char yyyy[8] = { 0 }; 
    char mm[4] = { 0 };
    char dd[4] = { 0 };

    // Check the entry type
    switch(p_entry->entry_type) {
    case CSV_ENTRY_TYPE_STRING:
        // Check if string date has correct amount of characters (yyyy-mm-dd, so 10 characters) and format
        if(strlen(p_entry->str_data) != 10 || (p_entry->str_data[4] != '-' && p_entry->str_data[7]))
            INVALID_DATE_FORMAT(file_name, p_entry->str_data);

        // Copy year, month and day to their buffer
        strncpy(yyyy, p_entry->str_data, 4);
        strncpy(mm, p_entry->str_data + 5, 2);
        strncpy(dd, p_entry->str_data + 8, 2);

        Date date;
        date.year = (uint16_t) atoi(yyyy);
        date.month = (uint16_t) atoi(mm);
        date.day = (uint16_t) atoi(dd);

        return date;

    case CSV_ENTRY_TYPE_FLOAT: 
        fprintf(stderr, "Invalid entry type, got float but expected string\n");
        exit(EXIT_FAILURE);
        break;

    case CSV_ENTRY_TYPE_INTEGER:
        fprintf(stderr, "Invalid entry type, got integer but expected string\n");
        exit(EXIT_FAILURE);
        break;
    }
}


/// High level function to parse all data from csv power plant file
void parsePowerPlantFile(char *file_name, PowerPlants *p_plants) {
    char *buf = NULL;
    size_t len = 0;

    // First read the file data into char buffer   
    __readFileToBuffer(file_name, &buf, &len);

    CsvRow *rows = NULL;
    size_t row_c = 0;
    __parseCSVRows(buf, len, file_name, &rows, &row_c);

    // Allocate reserve memory for power plants since it is not known how many plants would be in the file 
    p_plants->cap = row_c < __DEFAULT_POWER_PLANT_CAP ? __DEFAULT_POWER_PLANT_CAP : __roundToBase2(row_c << 1);
    p_plants->n = 0;
    p_plants->plants = (PlantData*) malloc(p_plants->cap * sizeof(PlantData));
    p_plants->max_id = 0;
    
    // For each row verify the correct data type and and set the PlantData values
    for(size_t i = 0; i < row_c; i++) {
        // Check if the row contains correct amount of records
        if(rows[i].n != 4)
            INVALID_ARG_C(file_name);

        // Check the row CSV values and check if their types are correct to be assigned into PlantData structure
        // 0. Powerplant id (integer, can also be a float)
        // 1. Power plant name (string)
        // 2. Fuel type (string: "coal", "shaleoil", "gas", "uranium", "water", "wind", "geothermal")
        // 3. Rated capacity (integer / float)
        p_plants->plants[i].no = (uint32_t) __csvEntryRetrieveInteger(&rows[i].entries[0]);
        p_plants->plants[i].name = __csvEntryRetrieveString(&rows[i].entries[1]);
        p_plants->plants[i].fuel = __csvCheckFuelType(&rows[i].entries[2]);
        p_plants->plants[i].rated_cap = __csvEntryRetrieveFloat(&rows[i].entries[3]);
        p_plants->plants[i].avg_cost = 0.0;
        p_plants->n++;

        // Check if maximum value should be updated
        if(p_plants->plants[i].no > p_plants->max_id)
            p_plants->max_id = p_plants->plants[i].no;

        // Free the CSV row data
        for(size_t j = 0; j < rows[i].n; j++) {
            if(rows[i].entries[j].entry_type == CSV_ENTRY_TYPE_STRING)
                free(rows[i].entries[j].str_data);
        }
        free(rows[i].entries);
    }

    free(rows);

    // Free the allocated char buffer
    free(buf);
}


/// High level function to parse all logs from the csv logs file
void parseLogsFile(char *file_name, PlantLogs *p_logs) {
    char *buf = NULL;
    size_t len = 0;

    // Read file data into char buffer
    __readFileToBuffer(file_name, &buf, &len);

    CsvRow *rows = NULL;
    size_t row_c = 0;
    __parseCSVRows(buf, len, file_name, &rows, &row_c);

    // Allocate initial amount of memory for logs
    p_logs->cap = row_c < __DEFAULT_LOG_CAP ? __DEFAULT_LOG_CAP : __roundToBase2(row_c);
    p_logs->n = 0;
    p_logs->entries = (LogEntry*) malloc(p_logs->cap * sizeof(LogEntry));

    // Set the initial max id value
    p_logs->max_id = 0;
    
    // For each row verify the correct data type and set LogEntry value
    for(size_t i = 0; i < row_c; i++) {
        // Check if the current row contains correct amount of entries
        if(rows[i].n != 5)
            INVALID_ARG_C(file_name);

        // Verify that the data is correct and set it to LogEntry values
        // 0. Log id (integer, can be a float)
        // 1. Plant number (integer, can be a float)
        // 2. Production for that day (float, can be an integer)
        // 3. Average cost for of energy for that day (float, can be an integer)
        // 4. Date (string)
        p_logs->entries[i].log_id = (uint32_t) __csvEntryRetrieveInteger(&rows[i].entries[0]);
        p_logs->entries[i].plant_no = (uint32_t) __csvEntryRetrieveInteger(&rows[i].entries[1]);
        p_logs->entries[i].production = (float) __csvEntryRetrieveFloat(&rows[i].entries[2]);
        p_logs->entries[i].avg_sale_price = (float) __csvEntryRetrieveFloat(&rows[i].entries[3]);
        p_logs->entries[i].date = __csvCheckDateType(file_name, &rows[i].entries[4]);
        p_logs->n++;

        // Check if maximum value should be updated
        if(p_logs->entries[i].log_id > p_logs->max_id)
            p_logs->max_id = p_logs->entries[i].log_id;

        // Free csv row entry data
        for(size_t j = 0; j < rows[i].n; j++) {
            if(rows[i].entries[j].entry_type == CSV_ENTRY_TYPE_STRING)
                free(rows[i].entries[j].str_data);
        }

        free(rows[i].entries);
    }

    free(rows);

    // Free allocated buffer data
    free(buf);
}
