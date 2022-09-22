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

#include "map.h"

void map_fprintf_IntString(FILE* stream, Map* map) {
      for (int i= 0; i < list_len(&map->keys); i++) {
            char** key= (void*) list_getPtr(&map->keys, i);
            int* value= (int*) list_getPtr(&map->values, i);
            fprintf(stream, "%s: %d\n", *key, *value);
      }
}

// --> Initializes and deletes the array and array information
Map* newMap(int keySize, int valueSize, int maxThings) {
      Map* map= malloc(sizeof(Map));
      initList(&map->keys, keySize, maxThings);
      initList(&map->values, valueSize, maxThings);
      return map;
}
void initMap(Map* map, int keySize, int valueSize, int maxThings) {
      initList(&map->keys, keySize, maxThings);
      initList(&map->values, valueSize, maxThings);
      return;
}
void deleteMap(Map* map) {
      closeList(&map->keys);
      closeList(&map->values);
      free(map);
      return;
}
void closeMap(Map* map) {
      closeList(&map->keys);
      closeList(&map->values);
      return;
}

// --> Adds and removes elements from the map
void map_add(Map* map, const void* key, void* value) {
      list_push(&map->keys, key);
      list_push(&map->values, value);
      return;
}
void map_remove(Map* map, const void* key, void* output) {
      for (int i= 0; i < list_len(&map->keys); i++) {
            if (memcmp(key, list_getPtr(&map->keys, i), list_elementSize(&map->keys)) == 0) {
                  list_remove(&map->keys, i, NULL);
                  list_remove(&map->values, i, output);
                  return;
            }
      }
      return;
}

// --> Reads values without removing them
void map_get(Map* map, const void* key, void* output) {
      for (int i= 0; i < list_len(&map->keys); i++) {
            if (memcmp(key, list_getPtr(&map->keys, i), list_elementSize(&map->keys)) == 0) {
                  list_get(&map->values, i, output);
                  return;
            }
      }
      return;
}
void* map_getPtr(Map* map, const void* key) {
      for (int i= 0; i < list_len(&map->keys); i++) {
            if (memcmp(key, list_getPtr(&map->keys, i), list_elementSize(&map->keys)) == 0) {
                  return list_getPtr(&map->values, i);
            }
      }
      return NULL;
}

// --> Returns attributes of the map
int map_keySize(Map* map) {
      return list_len(&map->keys);
}
int map_valueSize(Map* map) {
      return list_len(&map->values);
}
int map_maxLen(Map* map) {
      return list_maxLen(&map->keys);
}
int map_len(Map* map) {
      return list_len(&map->keys);
}
