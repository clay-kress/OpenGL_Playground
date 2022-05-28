#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "list.h"

typedef struct {
      List keys;
      List values;
} Map;

void map_printIntString(Map* map);

// --> Initializes and deletes the array and array information
Map* newMap(int keySize, int valueSize, int maxThings);
void initMap(Map* map, int keySize, int valueSize, int maxThings);
void deleteMap(Map* map);                                               // For Heap initialized Maps
void closeMap(Map* map);                                                // For Stack initialized Maps

// --> Adds and removes elements from the map
void map_add(Map* map, const void* key, void* value);                   // Adds key value pair to the map
void map_remove(Map* map, const void* key, void* output);               // Output stores the value

// --> Reads values without removing them
void map_get(Map* map, const void* key, void* output);                  // *output is filled with the value at the specified key
void* map_getPtr(Map* map, const void* key);                            // Returns a pointer to the value of the key

// --> Returns attributes of the map
int map_keySize(Map* map);
int map_valueSize(Map* map);
int map_maxLen(Map* map);
int map_len(Map* map);
