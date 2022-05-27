#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "list.h"

typedef struct {
      List keys;
      List values;
} Map;

// --> Initializes and deletes the array and array information
Map* newMap(int keySize, int valueSize, int maxThings);
void initMap(Map* map, int keySize, int valueSize, int maxThings);
void deleteMap(Map* map);                                               // For Heap initialized Maps
void closeMap(Map* map);                                                // For Stack initialized Maps