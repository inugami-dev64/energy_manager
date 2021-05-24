/*
 * File:        algo.h
 * Author:      Karl-Mihkel Ott
 * Created      2021-05-21
 * Last edit:   2021-05-21
 * Description: Provide algoritm implementation function declarations
 */


#ifndef __ALGO_H


/// Specifies sort value type
typedef enum SortValueType {
    SORT_VALUE_TYPE_UINT32      = 0,
    SORT_VALUE_TYPE_FLOAT32     = 1,
    SORT_VALUE_TYPE_DATE        = 2
} SortValueType;


#ifdef __ALGO_C
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>
    #include <stdint.h>
    #include <string.h>

    #include <entity_data.h>

    /// Check if the data sorting should set for integer to destination buffe
    void iswap(bool is_decr, uint32_t lval, uint32_t rval, void *dst, void *la, 
        void *ra, size_t stride, size_t *p_i, size_t *p_j);


    /// Check if the data sorting should set for floating point integer to 
    /// destination buffe
    void fswap(bool is_decr, float lval, float rval, void *dst, void *la, 
        void *ra, size_t stride, size_t *p_i, size_t *p_j);


    /// Check how the subarray element sorting should be done for sorting
    /// float values
    void __sortCheckf(bool is_decr, bool is_ref, void *dst, void *la, void *ra, size_t stride, 
        size_t val_offset, size_t *p_i, size_t *p_j);


    /// Check how the subarray element sorting should be done for sorting
    /// integer values
    void __sortChecki(bool is_decr, bool is_ref, void *dst, void *la, void *ra, size_t stride, 
        size_t val_offset, size_t *p_i, size_t *p_j);

    
    /// Check how the subarray element sorting should be done for sorting
    /// by date
    void __sortCheckDate(bool is_decr, bool is_ref, void *dst, void *la, void *ra, size_t stride,
        size_t val_offset, size_t *p_i, size_t *p_j);


    /// Merge the subarrays in sorted order (using floating point values)
    void __merge(void *arr, size_t val_offset, size_t stride, bool is_decr,
        SortValueType use_float_cmp, bool is_ref, size_t end);


    #define __DEFAULT_BUF_SIZE          256
#endif


/// Perform merge sort on generic structure with specified stride and value
/// offset
void mergesort(void *arr, size_t val_offset, size_t stride, bool is_decr,
    SortValueType type, bool is_ref, size_t beg, size_t end);


/// Calculate the average utilisation for a power plant based on its daily logs
void calcAvgUtilisation(PlantData *p_plant);


/// Calculate the average cost for the power plant based on its daily logs
void calcAvgCost(PlantData *p_plant);

#endif
