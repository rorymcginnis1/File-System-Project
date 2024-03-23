/**************************************************************
* Class:  CSC-415-1 Summer 2023
* Names: Kaung Nay Htet, Himal Shrestha, Rory McGinnis,  James Donnelly
* Student IDs:922292784, 922399514, 921337245, 917703805
* GitHub Name: rorymcginnis1
* Group Name: Team Drivers
* Project: Basic File System
*
* File: fsInit.c
*
* Description: Main driver for file system assignment.
*
* This file is where you will start and initialize your system
*
**************************************************************/

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include "fsLow.h"
#include "mfs.h"
#include "extents.c"
#include "fsInit.h"

struct DirectoryEntry* globalDirEntries = NULL;

//Initalize the root directory
int initialize_root_directory(int minEntreis, struct DirectoryEntry * parent)
{
	//Set the total number of bytes and entries we need
	int bytesNeeded =(sizeof(DirectoryEntry)*NUM_DIRECT_ENTRIES);
	int numBlocks=(bytesNeeded +BLOCK_SIZE-1)/BLOCK_SIZE;	
	int bytesToAllocate =numBlocks*BLOCK_SIZE;
	int actualEnteries = bytesToAllocate/(sizeof(DirectoryEntry));
	
	//Validate in case of an error
	if (actualEnteries< minEntreis){
		return -1;
	}
	
	// Allocate memory for entries to be stored
	struct DirectoryEntry * newD =malloc(sizeof(DirectoryEntry)*actualEnteries);
	time_t currentTime = time(NULL);

	if(newD ==NULL)	{
		return -1;
	}

	globalDirEntries = newD;	
	
	//Initalize the NewDirectory created with default data
	for (int i=0; i<actualEnteries; i++){
		strcpy(newD[i].fileName, "");
		newD[i].fileSize= 0;
		newD[i].fileLocation=-1;
		newD[i].dateCreated=0;
		newD[i].dateAccessed=0;//changed nullls to 0^^vv
		newD[i].dateModified=0;
		newD[i].isaDirectory=0;
	}
	
	//Populate data for the directory "."
	strcpy(newD[0].fileName, ".");
	newD[0].fileSize= actualEnteries * sizeof(DirectoryEntry);
	extent * e = allocateBlocks(numBlocks, numBlocks);
	if(e ==NULL)
	{
		free(newD);
		return -1;
	}
	newD[0].fileLocation = e->start;
	newD[0].dateCreated=currentTime;
	newD[0].dateAccessed=currentTime;
	newD[0].dateModified=currentTime;
	newD[0].isaDirectory=1;
	free(e);
	
	//Populate data for the directory ".."
	strcpy(newD[1].fileName,"..");
	//if parent is null it is the root directory so parent will be itself
	if(parent == NULL)
	{
		parent = newD;
	
	}
	else
	{
		newD[1].fileSize = parent[0].fileSize;
		newD[1].fileLocation = parent[0].fileLocation;
		newD[1].dateCreated=currentTime;
		newD[1].dateAccessed=currentTime;
		newD[1].dateModified=currentTime;
		newD[1].isaDirectory=1;	
	}     

	//release the blocks     
	releaseBlocks(newD[1].fileLocation,numBlocks);
	
	//write to memory
	LBAwrite(newD, numBlocks, newD[0].fileLocation);
	
	//return the location of the file
	return (newD[0].fileLocation);

	}


//	Initalize the file system
int initFileSystem (uint64_t numberOfBlocks, uint64_t blockSize)
	{

	printf ("Initializing File System with %ld blocks with a block size of %ld\n", numberOfBlocks, blockSize);
	/* TODO: Add any code you need to initialize your file system. */

	// Create memory allocation for vcb
	struct VolumeControlBlock * vcbPointer;
	vcbPointer = malloc(blockSize);

	//Catch case in case pointer is invalid
	if (vcbPointer == NULL ){
		printf("NULL");
		return -1;
	}
	
	//Initalize the freespace
	int fs = initFreeSpace();
	if(fs != 6)
	{
		return -1;
	}
	
	int minEnt = 2;
	struct DirectoryEntry * root = malloc(sizeof(DirectoryEntry));
	
	//initalize the root directory
	int rd = initialize_root_directory(minEnt, root);
	
	// rd returns the root directory, and status check of whether
	// created or not
	if(rd < 0)
	{
		return -1;
	}
	serializeFreeSpaceMap();

	//Populate vcbPointer and write to the disk
	if (vcbPointer->signature != MAGICNUMBER)
	{
		vcbPointer->signature = MAGICNUMBER;
		vcbPointer->totalBlockSize = numberOfBlocks;
		vcbPointer->blockSize = blockSize;
		vcbPointer->startBlock = rd;
		LBAwrite (vcbPointer, 1, 0);
	}

	printf("\n");

	return 0;

	}
	
//exit the file system
void exitFileSystem ()
	{
	printf ("System exiting\n");
	}