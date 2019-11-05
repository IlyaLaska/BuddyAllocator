//
// Created by illya on 11/3/2019.
//

#include "SLL.h"
#include "stdio.h"

//Node* headP = NULL;

void addNode(Node** head, unsigned long address, int size) {
    if(size > USHRT_MAX) --size;
    Node* new = malloc(sizeof(Node));
    new->address = address;
    new->size = size;
    if(head) new->next = (*head);
    else new->next = NULL;
    (*head) = new;
};

void deleteNode(Node** head, unsigned long address) {
    Node* curr = (*head);
    Node* prev = NULL;
    do {
        if(curr->address == address) {
            if(!prev) (*head) = curr->next;
            else prev->next = curr->next;
            free(curr);
            break;
        } else {
            prev = curr;
            curr = curr->next;
        }
    } while (curr != NULL);
};
////TODO see if I need to replace it with Node* line 38 41
Node* getNode(Node* head, unsigned long address) {
    Node* curr = head;
    do {
        if(curr->address == address) return curr;
        curr = curr->next;
    } while (curr != NULL);
    Node* fail = malloc(sizeof(Node));
    fail->address = -1;
    return fail;
};

//Node getFirstNode(Node* head) {
//    return
//};

void printList(Node* head) {
    Node* curr = head;
//    printf("LIST:\n");
    short pos = 1;
    do {
        int size;
        if(curr->size != USHRT_MAX) size = curr->size;
        else size = curr->size + 1;
        printf("El %i: address: %li size: %i\n", pos, curr->address, size);
        curr = curr->next;
        pos++;
    } while (curr != NULL);
};