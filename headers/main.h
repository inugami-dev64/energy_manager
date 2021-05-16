/*
 * File:        main.h
 * Author:      Karl-Mihkel Ott
 * Created      2021-05-13
 * Last edit:   2021-05-13
 * Description: Contains main and input polling functions
 */


#ifndef __MAIN_H
#define __MAIN_H

#ifdef __MAIN_C
    #include <stdlib.h>
    #include <stdbool.h>
    #include <stdio.h>
    #include <stdint.h>
    
    #include <hashmap.h>
    #include <entity_data.h>
    #include <data_parser.h>
    #include <energy_manager.h>
    
    #define __DEFAULT_BUF_LEN   1024
#endif


/// Poll user input
void poll();


#endif
