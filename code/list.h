/***********************************************************************/
/* Copyright (c) 2022 Clay Kress                                       */
/*                                                                     */
/* This file is part of VioletGE.                                      */
/* VioletGE is free software: you can redistribute it and/or modify it */
/* under the terms of the GNU General Public License as published by   */
/* the Free Software Foundation, either version 3 of the License, or   */
/* (at your option) any later version.                                 */
/*                                                                     */
/* VioletGE is distributed in the hope that it will be useful,         */
/* but WITHOUT ANY WARRANTY; without even the implied warranty         */
/* of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.             */
/*                                                                     */
/* See the GNU General Public License for more details.                */
/*                                                                     */
/* You should have received a copy of the GNU General Public License   */
/* along with VioletGE. If not, see <https://www.gnu.org/licenses/>.   */
/*                                                                     */
/***********************************************************************/

#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*** List type data structure in C
 * Use values of any bytelength
 * (DON'T FORGET THE TYPE OR THE LENGTH)
 */

typedef struct {
      char* array;
      int typeSize;
      int maxThings;
      int numThings;
} List;

void list_fprintf_Int(FILE* stream, List* list);
void list_fprintf_String(FILE* stream, List* list);

// --> Initializes and deletes the array and array information
List* newList(int typeSize, int maxThings);
void initList(List* list, int typeSize, int maxThings);
void deleteList(List* list);                                            // For Heap initialized Lists
void closeList(List* list);                                             // For Stack initialized Lists

// --> Appends and removes elements from the end of the array
void* list_push(List* list, const void* input);                         // *input is pushed to the array, returns a ptr to the element
void list_pop(List* list, void* output);                                // *output is filled with the popped element of the array
void* list_addElements(List* list, int numElements);                    // Appends elements onto the end of the List, returns a ptr to the first element

// --> Inserts and removes elements from the middle of the array
void* list_insert(List* list, int index, const void* input);            // *input is inserted at the specified index, returns a ptr to the element
void list_remove(List* list, int index, void* output);                  // *output is filled with the element at the specified index
void* list_insertElements(List* list, int index, int numElements);      // Inserts elemente into the List, returns a ptr to the first element

// --> Reads values without removing them
void list_get(List* list, int index, void* output);                     // *output is filled with the element of the array at the specified index
void* list_getPtr(List* list, int index);                               // Returns a pointer to the element

// --> Returns attributes of the list
int list_elementSize(List* list);                                       // Size of each element is returned
int list_maxLen(List* list);                                            // Maximum length of the array is returned
int list_len(List* list);                                               // Length of the array is returned
