/*
 * File:        entity_data.h
 * Author:      Karl-Mihkel Ott
 * Created      2021-05-11
 * Last edit:   2021-05-11
 * Description: Provide structures for power plant entities
 */


#ifndef __ENTITY_DATA_H
#define __ENTITY_DATA_H


/// Enumeral values of the type of fuel used by the power plant
typedef enum FuelType {
    FUEL_TYPE_COAL          = 0,
    FUEL_TYPE_SHALE_OIL     = 1,
    FUEL_TYPE_GAS           = 2,
    FUEL_TYPE_URANIUM       = 3,
    FUEL_TYPE_WATER         = 4,
    FUEL_TYPE_WIND          = 5,
    FUEL_TYPE_GEOTHERMAL    = 6
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
    float daily_production;
    float avg_sale_price;
    Date date;
} LogEntry;


/// Structure for containing multiple daily log instances
typedef struct PlantLogs {
    LogEntry *entries;
    size_t cap;
    size_t n;
} PlantLogs;


/// Structure for containing all power plant related information 
typedef struct PlantData {
    uint32_t no;
    char *name;
    FuelType fuel;
    float rated_cap; // MW
    float avg_cost;
    PlantLogs logs;
} PlantData;


/// Structure for containing all power plant instances
typedef struct PowerPlants {
    PlantData *plants;
    size_t cap;
    size_t n;
} PowerPlants;

#endif
