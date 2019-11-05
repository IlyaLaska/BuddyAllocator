//
// Created by illya on 11/3/2019.
//

#include "hashTable.h"

HashTable used;

void createTable(unsigned short size) {
    used.size = size;
    used.elems = malloc(sizeof(Node*) * size);
    for (int i = 0; i < size; ++i) {
        used.elems[i] = NULL;
    }
};

void insertIntoTable(unsigned long address, unsigned short size) {
    short pos = address % used.size;
    addNode(&used.elems[pos], address, size);
};

Node* getFromTable(unsigned long address) {
    short pos = address % used.size;
    Node* retNode = getNode(used.elems[pos], address);
    return retNode;
};

void deleteFromTable(unsigned long address) {
    short pos = address % used.size;
    deleteNode(&used.elems[pos], address);
};

void printTable() {
//    printf("TABLE:\n");
    short pos = 0;
    do {
        printf("%hd: ", pos);
        if(used.elems[pos]) printList(used.elems[pos]);
        else printf("Empty Node\n");
        pos++;
    } while (pos < used.size);
}
