/*
 * File:        mem_check.c
 * Author:      Karl-Mihkel Ott
 * Created      2021-05-11
 * Last edit:   2021-05-12
 * Description: Provide simple memory management function definitions that are used commonly 
 * in the program
 */


#define __MEM_CHECK_C
#include <mem_check.h>


/// Round the 64 bit integer to the nearest base2 exponant
size_t __roundToBase2(size_t k) {
    // Manipulate the integer bitwise so that all of the less significant bits will be one
    k |= k >> 1;
    k |= k >> 2;
    k |= k >> 4;
    k |= k >> 8;
    k |= k >> 16;
    k |= k >> 32;

    // Add one to the return value, since otherwise the value would be something like this: pow(2, n) - 1
    return k + 1;
}


/// Check if memory reallocation is necessary based on the required size and capacity
void reallocCheck(void **p_data, size_t size, size_t req_n, size_t *p_cap) {
    // Check if the required memory size is bigger than capacity
    if(size * req_n >= (*p_cap)) {
        // Assign new capacity value determined from the rounded required size or rounded previous cap * 2
        (*p_cap) = __roundToBase2(size * req_n) < __roundToBase2((*p_cap) << 1) ? 
                   __roundToBase2((*p_cap) << 1) : __roundToBase2((size * req_n) << 1);

        // Attempt reallocation
        void *tmp = realloc((*p_data), (*p_cap) * size);

        // If the previous reallocation was not successful, throw an error
        if(!tmp) {
            fprintf(stderr, "Failed reallocation");
            exit(EXIT_FAILURE);
        }
        (*p_data) = tmp;
    }
}


/// Check if the given memory area contains only ascii numbers
/// Returns 1 if the memory area contains only ascii numbers and 0 otherwise
bool numcheck(char *data, size_t len) {
    // For each byte in the memory area check its value
    for(size_t i = 0; i < len; i++) {
        if((data[i] < '0' || data[i] > '9') && data[i] != ' ')
            return false;
    }

    return true;
}


/// Check if the given memory area contains only ascii numbers and / or '.'
/// Returns 1 if the memory area is float compatible, 0 otherwise
bool floatcheck(char *data, size_t len) {
    int dot_c = 0;

    // For each byte in the memory area check its value
    for(size_t i = 0; i < len; i++) {
        if((data[i] < '0' || data[i] > '9') && data[i] != ' ' && 
           data[i] != '.' || dot_c > 1)
            return false;
        else if(data[i] == '.')
            dot_c++;
    }

    return true;
}


/// Convert FuelType enumeral into appropriate string
/// NOTE: Pointer to in-scope static char is returned
char *fuelTypeToStr(FuelType tp) {
    // Check the fueltype and return correct string
    switch(tp) {
        case FUEL_TYPE_COAL:        return "coal";
        case FUEL_TYPE_GAS:         return "gas";
        case FUEL_TYPE_GEOTHERMAL:  return "geothermal";
        case FUEL_TYPE_SHALE_OIL:   return "shaleoil";
        case FUEL_TYPE_URANIUM:     return "uranium";
        case FUEL_TYPE_WATER:       return "water";
        case FUEL_TYPE_WIND:        return "wind";
        default:                    return NULL;
    }
}


/// Convert string into appropriate FuelType instance
FuelType strToFuelType(char *str) {
    // Check for the string value and return correct FuelType instance
    if(!strcmp(str, "coal")) return FUEL_TYPE_COAL;
    else if(!strcmp(str, "gas")) return FUEL_TYPE_GAS;
    else if(!strcmp(str, "geothermal")) return FUEL_TYPE_GEOTHERMAL;
    else if(!strcmp(str, "shaleoil")) return FUEL_TYPE_SHALE_OIL;
    else if(!strcmp(str, "uranium")) return FUEL_TYPE_URANIUM;
    else if(!strcmp(str, "water")) return FUEL_TYPE_WATER;
    else if(!strcmp(str, "wind")) return FUEL_TYPE_WIND;

    return FUEL_TYPE_UNKNOWN;
}
