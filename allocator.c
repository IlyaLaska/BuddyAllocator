//
// Created by illya on 11/3/2019.
//

#include <stdio.h>
#include "allocator.h"
#include "math.h"

extern HashTable used;
Array unused;

int *head;//, *tail;
static const unsigned short coreBlock = USHRT_MAX;

void createArray(unsigned short size) {
    unused.size = size;
    unused.elems = malloc(sizeof(Node *) * size);
    for (int i = 0; i < size; ++i) {
        unused.elems[i] = NULL;
    }
};

void printArray() {
//    printf("TABLE:\n");
    short pos = 0;
    do {
        printf("%hd: ", pos);
        if (unused.elems[pos]) printList(unused.elems[pos]);
        else printf("Empty Node\n");
        pos++;
    } while (pos < unused.size);
}

void *getCore() {//create memory core
    head = malloc(coreBlock);
    if (head) {
        // tail = (struct ctrl *) ((void *) head + size);
        short maxPow = ceil(log(coreBlock) / log(2));
        createTable(maxPow - 3);//disallow nodes < 16 b??
        createArray(maxPow - 3);
        addNode(&unused.elems[maxPow - 3 - 1], 0, USHRT_MAX);//-1 since array starts at 0, -3 to disallow nodes < 16
        return head;
    } else {
        return NULL;
    }
};

void *alloc(unsigned short size) {
    unsigned short rightSize = ceil(log(size) / log(2));
    if (rightSize < 4) rightSize = 4;
//    printf("SIZE: %i  ", rightSize);
//    printf("pos: %i\n", rightSize - 1 - 3);

    if (unused.elems[rightSize - 1 - 3]) {//such free block exists
        int *addr = unused.elems[rightSize - 1 - 3]->address + head;
        insertIntoTable(unused.elems[rightSize - 1 - 3]->address, (int) pow(2, rightSize));
        printf("Inserted into used block address: %li, size: %i\n", unused.elems[rightSize - 1 - 3]->address,
               (int) pow(2, rightSize));
        printf("Allocated Block From %li to %i\n", unused.elems[rightSize - 1 - 3]->address,
               (int) pow(2, (rightSize + 1)));
        deleteNode(&unused.elems[rightSize - 1 - 3], unused.elems[rightSize - 1 - 3]->address);
        return addr;
    } else {//need to break up bigger block
        int biggerBlockPos;
        for (biggerBlockPos = rightSize - 1 - 3 + 1; biggerBlockPos < unused.size; ++biggerBlockPos) {
            if (unused.elems[biggerBlockPos]) break;
        }
        //now biggerBlockPos is an index of unused.items where needed block is, or == items.size
        if (biggerBlockPos == unused.size) {//if no block is available
            printf("Failed to allocate\n");
            return NULL;
        }
        //remove the first block of biggerSize
        long biggerBlockAddr = unused.elems[biggerBlockPos]->address;
//        printf("oldAddrL: %li\n", unused.elems[biggerBlockPos]->address);
//        printf("BiggerBlockPos: %i\n", biggerBlockPos);
//        printf("oldAddrR: %i\n", (int) pow(2, biggerBlockPos + 1 + 3));
        deleteNode(&unused.elems[biggerBlockPos], unused.elems[biggerBlockPos]->address);
        biggerBlockPos--;
//        printf("BiggerBlockPos: %i\n", biggerBlockPos);
        for (; biggerBlockPos >= rightSize - 1 - 3; biggerBlockPos--) {
            //size of biggerBlock = 2^(biggerBlockPos+1+3) is it so?
            long leftAddr = biggerBlockAddr;
//            printf("BiggerBlockPos: %i\n", biggerBlockPos);
            long rightAddr = biggerBlockAddr + (int) pow(2, biggerBlockPos + 1 + 3);//1 smaller than biggerBlock
//            printf("newAddrL: %li\n", leftAddr);
//            printf("newAddrR: %li\n", rightAddr);
            addNode(&unused.elems[biggerBlockPos], rightAddr, (int) pow(2, biggerBlockPos + 1 + 3));
//            addNode(&unused.elems[biggerBlockPos], leftAddr);
            biggerBlockAddr = leftAddr;
            //if add leftAddr, have to then remove it again
        }
        //now biggerBlockAddr has the correct address that can be unused
//        printf("BiggerBlockPos: %i\n", biggerBlockPos);
        insertIntoTable(biggerBlockAddr, (int) pow(2, (biggerBlockPos + 1 + 1 + 3)));
        printf("Inserted into used block address: %li, size: %i\n", biggerBlockAddr,
               (int) pow(2, (biggerBlockPos + 1 + 1 + 3)));
        printf("Allocated Block From %li to %i\n", biggerBlockAddr, (int) pow(2, (biggerBlockPos + 1 + 1 + 3)) - 1);
        int *addr = biggerBlockAddr + head;
        return addr;
    }
};

void dealloc(int *realAddr) {
    unsigned long address = realAddr - head;
    short posInUsed = address % used.size;//this is right now?
    fflush(stdout);
    Node *thisNode = getNode(used.elems[posInUsed], address);
    if (thisNode->address == -1) {
        printf("This block does not exist\n");
        free(thisNode);
        return;
    } else {
//        int nodeSize = thisNode->size;
        int posInUnused = ((int) ceil(log(thisNode->size) / log(2))) - 1 - 3;
        unsigned long buddyNumber, buddyAddress;
        Node *toRem = getNode(used.elems[address % used.size], address);//node that should be removed
        buddyNumber = toRem->address / toRem->size;//find if this is the odd or even block
        if (buddyNumber % 2 == 0) {//even
            buddyAddress = toRem->address + toRem->size;
        } else {//odd
            buddyAddress = toRem->address - toRem->size;
        }
        deleteFromTable(toRem->address);
        //Node deleted from used, not yet added to unused

//        addNode(&unused.elems[posInUnused], toRem->address, toRem->size);//Sure that do need it???
//            short buddyPos = buddyAddress%unused.size;
//            Node* buddy = getNode(&unused.elems[], buddyAddress);
        //see if can join buddies
        unsigned long toJoinAddr = address;
//        Node* unjoinedNode = NULL;//this does not exist anywhere on 1st iter
        for (int i = posInUnused; i < unused.size; ++i) {//go up the hash unused joining up all the broken up blocks
            if(!unused.elems[i]) {//no buddy can be found
                printf("No buddy found\n");
                addNode(&unused.elems[i], toJoinAddr, pow(2, i+1+3));
                return;
            }
//            unjoinedNode = getNode(unused.elems[i], toJoinAddr);
            unsigned long unjoinedNodeAddr = toJoinAddr;
            int unjoinedNodeSize = pow(2, i+1+3);
            buddyNumber = unjoinedNodeAddr / unjoinedNodeSize;//find if this is the odd or even block
            if (buddyNumber % 2 == 0) {//even
                buddyAddress = unjoinedNodeAddr + unjoinedNodeSize;
            } else {//odd
                buddyAddress = unjoinedNodeAddr - unjoinedNodeSize;
            }
            Node *curr = unused.elems[i];
            short foundBuddy = 0;
            while (curr != NULL) {//go through blocks of the same size, looking for buddy
                if (curr->address == buddyAddress) {//it's a buddy block and it's free
                    foundBuddy = 1;
                    if (buddyNumber % 2 == 0) {
//                        addNode(&unused.elems[i + 1], address, unjoinedNodeSize * 2);//add a block twice as bif to next tier
                    } else {
//                        addNode(&unused.elems[i + 1], buddyAddress, unjoinedNodeSize * 2);//add a block twice as bif to next tier
                        toJoinAddr = buddyAddress;
                    }
                    deleteNode(&unused.elems[i], buddyAddress);
                    break;//gets out of while
                }
                curr = curr->next;
            };
            if(!foundBuddy) {//no buddy exists - have to add this block - further iterations useless;
                printf("This will work only if there is a block of same size, but not buddy\n");
                printf("Added to pos: %i addr: %lu size: %i\n", i, unjoinedNodeAddr, unjoinedNodeSize);
                addNode(&unused.elems[i], unjoinedNodeAddr, unjoinedNodeSize);
                printFreeBlocks();
                fflush(stdout);
                return;
            }
        }
    }
};

void *reealloc(int *realAddr, unsigned short newSize) {
    dealloc(realAddr);
    return alloc(newSize);
}

void printFreeBlocks() {
    printf("       Free Blocks:\n");
    printArray();
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
};

void printUsedBlocks() {
    printf("       Used Blocks:\n");
    printTable();
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

};
