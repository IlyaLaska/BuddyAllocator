cmake_minimum_required(VERSION 3.15)
project(BuddyAllocator C)

set(CMAKE_C_STANDARD 11)

add_executable(BuddyAllocator main.c SLL.c SLL.h hashTable.c hashTable.h allocator.c allocator.h)