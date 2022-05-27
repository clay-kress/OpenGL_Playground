#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct {
      List keys;
      List values;
} Map;

// --> Initializes and deletes the array and array information
Map* newMap(int typeSize, int maxThings);
void initMap(Map* map, int typeSize, int maxThings);
void deleteMap(Map* map);