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
        (*p_cap) = __roundToBase2(size * req_n) < __roundToBase2((*p_cap) >> 1) ? 
                   __roundToBase2((*p_cap) >> 1) : __roundToBase2((size * req_n) >> 1);

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
