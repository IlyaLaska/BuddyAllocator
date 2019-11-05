//
// Created by illya on 11/3/2019.
//

#ifndef BUDDYALLOCATOR_ALLOCATOR_H
#define BUDDYALLOCATOR_ALLOCATOR_H

#include "hashTable.h"

typedef struct Array {
    short size;
    Node** elems;
} Array;

void createArray(unsigned short size);

void printArray();

void* getCore();

void* alloc(unsigned short size);

void dealloc(int* realAddr);

void* reealloc(int* realAddr, unsigned short newSize);

void printFreeBlocks();

void printUsedBlocks();

#endif //BUDDYALLOCATOR_ALLOCATOR_H
