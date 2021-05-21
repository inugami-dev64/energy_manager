/* File:        entity_data.h
 * Author:      Karl-Mihkel Ott
 * Created      2021-05-11
 * Last edit:   2021-05-11
 * Description: Provide structures for power plant entities
 */


#ifndef __ENTITY_DATA_H
#define __ENTITY_DATA_H


/// Enumeral values of the type of fuel used by the power plant
typedef enum FuelType {
    FUEL_TYPE_UNKNOWN       = 0,
    FUEL_TYPE_COAL          = 1,
    FUEL_TYPE_SHALE_OIL     = 2,
    FUEL_TYPE_GAS           = 3,
    FUEL_TYPE_URANIUM       = 4,
    FUEL_TYPE_WATER         = 5,
    FUEL_TYPE_WIND          = 6,
    FUEL_TYPE_GEOTHERMAL    = 7
} FuelType;


/// Structure for date information
typedef struct Date {
    uint16_t year;
    uint16_t month;
    uint16_t day;
} Date;


/// Structure for containing log information about single day
typedef struct LogEntry {
    uint32_t log_id;
    uint32_t plant_no;
    float production;
    float avg_sale_price;
    Date date;
} LogEntry;


/// Structure for containing multiple daily log instances
typedef struct PlantLogs {
    LogEntry *entries;
    size_t max_id;
    size_t n;
    size_t cap;
} PlantLogs;


/// Structure for containing multiple log references accessed from
/// PlantData structure
typedef struct PlantLogRefs {
    LogEntry **p_entries;
    size_t n;
    size_t cap;
} PlantLogRefs;


/// Structure for containing all power plant related information 
typedef struct PlantData {
    uint32_t no;
    char *name;
    FuelType fuel;
    float rated_cap; // MW
    float avg_cost;
    float avg_utililisation;
    PlantLogRefs logs;
} PlantData;


/// Structure for containing all power plant instances
typedef struct PowerPlants {
    PlantData *plants;
    size_t max_id;
    size_t cap;
    size_t n;
} PowerPlants;

#endif
