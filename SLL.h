//
// Created by illya on 11/3/2019.
//

#ifndef BUDDYALLOCATOR_SLL_H
#define BUDDYALLOCATOR_SLL_H

#include <stdlib.h>

typedef struct Node {
    unsigned long address;
    unsigned short size;
    struct Node* next;
} Node;

void addNode(Node** head, unsigned long address, int size);

void deleteNode(Node** head, unsigned long address);

void printList(Node* head);

Node* getNode(Node* head, unsigned long address);

#endif //BUDDYALLOCATOR_SLL_H
