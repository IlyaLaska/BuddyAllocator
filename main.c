#include <stdio.h>
#include "allocator.h"

int main() {
    int* coreHead = getCore();
    int* sixteen = alloc(4);
    int* a = alloc(16);
    int* q = alloc(33000);
    printFreeBlocks();
    printUsedBlocks();
    dealloc(sixteen);
    printFreeBlocks();
    printUsedBlocks();
    a = reealloc(a, 32);
    printFreeBlocks();
    printUsedBlocks();
    dealloc(a);
    printFreeBlocks();
    printUsedBlocks();

    free(coreHead);
    return 0;
}
