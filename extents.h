/**************************************************************
* Class:  CSC-415-1 Summer 2023
* Names: Kaung Nay Htet, Himal Shrestha, Rory McGinnis,  James Donnelly
* Student IDs:922292784, 922399514, 921337245, 917703805
* GitHub Name: rorymcginnis1
* Group Name: Team Drivers
* Project: Basic File System
*
* File: extents.h
*
* Description: Header for the file where we initialize allocate and release
*              blocks for our system
*
**************************************************************/

#ifndef CSC415_FILESYSTEM_RORYMCGINNIS1_EXTENTS_H
#define CSC415_FILESYSTEM_RORYMCGINNIS1_EXTENTS_H

#endif //CSC415_FILESYSTEM_RORYMCGINNIS1_BITMAP_H

//definition of an extent
typedef struct extent {
    int start;
    int count;
} extent, * pextent;

//initFreeSpace is called when you initialize the volume
// it returns the block number where the freespace map starts
int initFreeSpace();

// if the volume is already initialized you need to call loadFreeSpace so the system has
// the freespace system ready to use.

int loadFreeSpace ();

// These are the key functions that callers need to use to allocate disk blocks and free them.

//allocateBlocks is how you obtain disk blocks. The first parameter is the number of
// blocks the caller requires. The minPerExtent (second) parameter is the minimum number of
// blocks in any one extent. If the two parameters are the same value, then the resulting
// allocation will be contiguous in one extent.
// The return value is an array of extents the last one has the start and count = 0
// which indicates the end of the array of extents.
// The caller is responsible for calling free on the return value.

extent * allocateBlocks (int required, int minPerExtent);

// This function returns blocks to the freespace system. If the caller wants to free all
// the blocks in a series of extents, they should loop through each extent calling
// releaseBlocks for each extent.
void releaseBlocks(int start, int count);
void serializeFreeSpaceMap();

