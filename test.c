#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <entity_data.h>
#include <algo.h>

#define LEN(arr) sizeof(arr) / sizeof(arr[0])

int main() {
    float cont[] = { 16.2, 1.1, 901.91, 23.13, 5.193, 6.135, 251.083, 12384, 600.32 };
    mergesort(cont, 0, sizeof(float), false, SORT_VALUE_TYPE_FLOAT32, false, 0, LEN(cont) - 1);

    // Print the array contents to stdout
    for(size_t i = 0; i < LEN(cont); i++)
        printf("%f\n", cont[i]);

	return EXIT_SUCCESS;
}
