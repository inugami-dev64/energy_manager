/*
 * File:        mem_check.h
 * Author:      Karl-Mihkel Ott
 * Created      2021-05-11
 * Last edit:   2021-05-11
 * Description: Provide simple memory management function declarations that are used commonly 
 * in the program
 */


#ifndef __MEM_CHECK_H
#define __MEM_CHECK_H

#ifdef __MEM_CHECK_C
    #include <stdio.h>
    #include <stdint.h>
    #include <string.h>
    #include <stdbool.h>
    #include <stdlib.h>

    #include <entity_data.h>
#endif

/// Round the 64 bit integer to the nearest base2 exponant
size_t __roundToBase2(size_t k);


/// Check if memory reallocation is necessary based on the required size and capacity
void reallocCheck(void **p_data, size_t size, size_t req_n, size_t *p_cap);


/// Check if the given memory area contains only ascii numbers
/// Returns 1 if the memory area contains only ascii numbers and 0 otherwise
bool numcheck(char *data, size_t len);


/// Check if the given memory area contains only ascii numbers and / or '.'
/// Returns 1 if the memory area is float compatible, 0 otherwise
bool floatcheck(char *data, size_t len);


/// Convert FuelType enumeral into appropriate string
/// NOTE: Pointer to in-scope static char is returned
char *fuelTypeToStr(FuelType tp);


/// Convert string into appropriate FuelType instance
FuelType strToFuelType(char *str);

#endif
