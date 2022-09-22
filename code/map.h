/***********************************************************************/
/* Copyright (c) 2022 Clay Kress                                       */
/*                                                                     */
/* This file is part of VioletGE.                                      */
/* VioletGE is free software: you can redistribute it and/or modify it */
/* under the terms of the GNU General Public License as published by   */
/* the Free Software Foundation, either version 3 of the License, or   */
/* (at your option) any later version.                                 */
/*                                                                     */
/* Foobar is distributed in the hope that it will be useful,           */
/* but WITHOUT ANY WARRANTY; without even the implied warranty         */
/* of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.             */
/*                                                                     */
/* See the GNU General Public License for more details.                */
/*                                                                     */
/* You should have received a copy of the GNU General Public License   */
/* along with Foobar. If not, see <https://www.gnu.org/licenses/>.     */
/*                                                                     */
/***********************************************************************/

#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "list.h"

/*** Map type data structure in C
 * Use keys/values of any bytelength
 * (DON'T FORGET THE TYPE OR THE LENGTH)
 */

typedef struct {
      List keys;
      List values;
} Map;

void map_fprintf_IntString(FILE* stream, Map* map);

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
int map_keySize(Map* map);                                              // Size of each key is returned
int map_valueSize(Map* map);                                            // Size of each values is returned
int map_maxLen(Map* map);                                               // Actual size of the arrays is returned
int map_len(Map* map);                                                  // returns length of the map (amount of key value pairs)
