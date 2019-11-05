//
// Created by illya on 11/3/2019.
//

#ifndef BUDDYALLOCATOR_HASHTABLE_H
#define BUDDYALLOCATOR_HASHTABLE_H

#include "SLL.h"
//#include "stdlib.h"

typedef struct HashTable {
    short size;
    Node** elems;
} HashTable;

void createTable(unsigned short size);

void insertIntoTable(unsigned long address, unsigned short size);

Node* getFromTable(unsigned long address);

void deleteFromTable(unsigned long address);

void printTable();

#endif //BUDDYALLOCATOR_HASHTABLE_H
