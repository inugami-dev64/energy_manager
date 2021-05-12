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
    #include <stdlib.h>
#endif

/// Round the 64 bit integer to the nearest base2 exponant
size_t __roundToBase2(size_t k);

/// Check if memory reallocation is necessary based on the required size and capacity
void reallocCheck(void **p_data, size_t size, size_t req_n, size_t *p_cap);


#endif
