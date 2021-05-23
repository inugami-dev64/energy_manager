/*
 * File:        algo.c
 * Author:      Karl-Mihkel Ott
 * Created      2021-05-21
 * Last edit:   2021-05-21
 * Description: Provide algoritm implementation function definitions
 */


#define __ALGO_C
#include <algo.h>


/// Check if the data sorting should set data to destination buffer
void iswap (
    bool is_decr, 
    uint32_t lval, 
    uint32_t rval, 
    void *dst, 
    void *la, 
    void *ra, 
    size_t stride, 
    size_t *p_i, 
    size_t *p_j
) {
    // Sorting order is not decreasing and left subarray value is greater than
    // corresponding right subarray value
    if(!is_decr && lval <= rval) {
        memcpy(dst, la, stride);
        (*p_i)++;
    }

    // Sorting order is not decreasing and right subarray value is greater than
    // corresponding left subarray value
    else if(!is_decr && lval > rval) {
        memcpy(dst, ra, stride);
        (*p_j)++;
    }

    // Sorting order is decreasing and left subarray value is greater than
    // corresponding right subarray value
    else if(is_decr && lval >= rval) {
        memcpy(dst, la, stride);
        (*p_i)++;
    }

    // Sorting order is decreasing and right subarray value is greater than
    // corresponding left subarray value
    else if(is_decr && lval < rval) {
        memcpy(dst, ra, stride);
        (*p_j)++;
    }
}

/// Check how the subarray element sorting should be done for sorting
/// float values
void __sortCheckf (
    bool is_decr, 
    bool is_ref,
    void *dst, 
    void *la, 
    void *ra, 
    size_t stride, 
    size_t val_offset,
    size_t *p_i,
    size_t *p_j
) {
    // Find the compare values
    float lval = is_ref ? *(float*) (*((void**) la) + val_offset) :
        *(float*) (la + val_offset);

    float rval = is_ref ? *(float*) (*((void**) ra) + val_offset) :
        *(float*) (ra + val_offset);

    iswap(is_decr, lval, rval, dst, la, ra, stride, p_i, p_j);
}


/// Check how the subarray element sorting should be done for sorting
/// integer values
void __sortChecki (
    bool is_decr, 
    bool is_ref,
    void *dst, 
    void *la, 
    void *ra, 
    size_t stride, 
    size_t val_offset,
    size_t *p_i,
    size_t *p_j
) {
    // Find the compare values
    uint32_t lval = is_ref ? *(uint32_t*) (*((void**) la) + val_offset) :
        *(uint32_t*) (la + val_offset);

    uint32_t rval = is_ref ? *(uint32_t*) (*((void**) ra) + val_offset) :
        *(uint32_t*) (ra + val_offset);

    iswap(is_decr, lval, rval, dst, la, ra, stride, p_i, p_j);
}


/// Check how the subarray element sorting should be done for sorting
/// by date
void __sortCheckDate (
    bool is_decr, 
    bool is_ref, 
    void *dst, 
    void *la, 
    void *ra, 
    size_t stride,
    size_t val_offset, 
    size_t *p_i, 
    size_t *p_j
) {
    uint32_t lval = 0, rval = 0;
    char buf[__DEFAULT_BUF_SIZE] = { 0 };

    // Check if the date data is referenced
    if(!is_ref) {
        // Find the appropriate date data
        sprintf(buf, "%d%d%d", (*(Date*) (la + val_offset)).year, (*(Date*) (la + val_offset)).month,
            (*(Date*) (la + val_offset)).day);
        lval = atoi(buf);

        // Clear the buffer
        memset(buf, 0, __DEFAULT_BUF_SIZE);

        // Read right date data
        sprintf(buf, "%d%d%d", (*(Date*) (ra + val_offset)).year, (*(Date*) (ra + val_offset)).month,
            (*(Date*) (ra + val_offset)).day);

        rval = atoi(buf);
    }

    else {
        // Find the appropriate date data
        sprintf(buf, "%d%d%d", (*(Date*) ((*(void**) (la)) + val_offset)).year, (*(Date*) ((*(void**) (la)) + val_offset)).month,
            (*(Date*) ((*(void**) (la)) + val_offset)).day);
        lval = atoi(buf);

        // Clear the buffer
        memset(buf, 0, __DEFAULT_BUF_SIZE);

        // Read right date data
        sprintf(buf, "%d%d%d", (*(Date*) ((*(void**) (ra)) + val_offset)).year, (*(Date*) ((*(void**) (ra)) + val_offset)).month,
            (*(Date*) ((*(void**) (ra)) + val_offset)).day);

        rval = atoi(buf);
    }

    iswap(is_decr, lval, rval, dst, la, ra, stride, p_i, p_j);
}


/// Merge the subarrays in sorted order
void __merge ( 
    void *arr,
    size_t val_offset,
    size_t stride,
    bool is_decr,
    SortValueType val_type,
    bool is_ref,
    size_t end
) {
    // Find the sizes of both subarrays
    size_t n1 = end / 2 + 1;
    size_t n2 = end - (end / 2);

    if(!n1 || !n2) return;
 
    // Create sub arrays
    char la[n1 * stride];
    char ra[n2 * stride];

    // Copy all given data over to the temporarely created arrays
    memcpy(la, arr, n1 * stride);
    memcpy(ra, arr + (n1 * stride), n2 * stride);

    // Index of the first subarray
    size_t i = 0;

    // Index of the second subarray
    size_t j = 0;

    // Index of the main array
    size_t k = 0;

    // For each element in the sub arrays check if they are sorted appropriately
    while(i < n1 && j < n2) {
        // Check the ValueType and sort the data correctly 
        switch(val_type) {
        case SORT_VALUE_TYPE_FLOAT32:
            __sortCheckf(is_decr, is_ref, arr + k * stride, la + i * stride, ra + j * stride,
                stride, val_offset, &i, &j);
            break;

        case SORT_VALUE_TYPE_UINT32:
            __sortChecki(is_decr, is_ref, arr + k * stride, la + i * stride, ra + j * stride,
                stride, val_offset, &i, &j);
            break;

        case SORT_VALUE_TYPE_DATE:
            __sortCheckDate(is_decr, is_ref, arr + k * stride, la + i * stride, ra + j * stride,
                stride, val_offset, &i, &j);
            break;
        }
        k++;
    }


    // Write any left over data from first subarray to the main array
    if(i < n1)
        memcpy(arr + k * stride, (la + i * stride), (n1 - i) * stride);

    // Write any left over data from second subarray to the main array
    if(j < n2)
        memcpy(arr + k * stride, (ra + j * stride), (n2 - j) * stride);
}


/// Perform merge sort on generic array with 32 bit floating point
/// values for comparison
void mergesort (
    void *arr, 
    size_t val_offset, 
    size_t stride, 
    bool is_decr,
    SortValueType val_type,
    bool is_ref,
    size_t beg,
    size_t end
) {
    if(beg < end) {
        size_t mid = beg + (end - beg) / 2;
        mergesort(arr, val_offset, stride, is_decr, val_type, is_ref, beg, mid);
        mergesort(arr, val_offset, stride, is_decr, val_type, is_ref, mid + 1, end);
        __merge(arr + beg * stride, val_offset, stride, is_decr, val_type, is_ref, 
            end - beg);
    }
}


/// Calculate the average utilisation for a power plant based on its daily logs
void calcAvgUtilisation(PlantData *p_plant) {
    const float max_day_produc = p_plant->rated_cap * 24;
    float total_production = 0.0f;

    // For each log in the plant instance add its production stat to
    // total production
    for(size_t i = 0; i < p_plant->logs.n; i++)
        total_production += p_plant->logs.p_entries[i]->production;

    if(p_plant->logs.n)
        p_plant->avg_utilisation = total_production / (p_plant->logs.n * max_day_produc) * 100;
    else p_plant->avg_cost = 0;
}


/// Calculate the average cost for the power plant based on its daily logs
void calcAvgCost(PlantData *p_plant) {
    float total_cost = 0.0f;

    // For each log in the plant instance add its cost stat to 
    // total cost
    for(size_t i = 0; i < p_plant->logs.n; i++)
        total_cost += p_plant->logs.p_entries[i]->avg_sale_price;

    if(p_plant->logs.n)
        p_plant->avg_cost = total_cost / p_plant->logs.n;
    else p_plant->avg_cost = 0;
}
