#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include <entity_data.h>
#include <data_parser.h>

int main(int argc, char *argv[]) {
     // Read the user input about file name
     char file_name[1024] = { 0 };
     scanf("%s", file_name);

     PowerPlants plants;
     parsePowerPlantFile(file_name, &plants);

     return EXIT_SUCCESS;
}
