//
// Created by Rebecca Sue Thomson for first project in cs525
//

#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>
#include<string.h>
#include<math.h>
#include <stdint.h>
#include "dberror.h"
#include "storage_mgr.h"
FILE *pageFile;

extern void initStorageManager (void) {
	// Initialising file pointer i.e. storage manager.
	pageFile = NULL;
}

extern RC createPageFile (char *fileName) {
	// Opening file stream in read & write mode. 'w+' mode creates an empty file for both reading and writing.
	pageFile = fopen(fileName, "w+");

	// Checking if file was successfully opened.
	if(pageFile == NULL) {
		return RC_FILE_NOT_FOUND;
	} else {
		// Creating an empty page in memory.
		SM_PageHandle emptyPage = (SM_PageHandle)calloc(PAGE_SIZE, sizeof(char));

		// Writing empty page to file.
		if(fwrite(emptyPage, sizeof(char), PAGE_SIZE,pageFile) < PAGE_SIZE)
			printf("write failed \n");
		else
			printf("write succeeded \n");

		// Closing file stream so that all the buffers are flushed.
		fclose(pageFile);

		// De-allocating the memory previously allocated to 'emptyPage'.
		// This is optional but always better to do for proper memory management.
		free(emptyPage);

		return RC_OK;
	}
}

extern RC openPageFile (char *fileName, SM_FileHandle *fHandle) {
	// Opening file stream in read mode. 'r' mode creates an empty file for reading only.
	pageFile = fopen(fileName, "r");

	// Checking if file was successfully opened.
	if(pageFile == NULL) {
		return RC_FILE_NOT_FOUND;
	} else {
		// Updating file handle's filename and set the current position to the start of the page.
		fHandle->fileName = fileName;
		fHandle->curPagePos = 0;

		/* In order to calculate the total size, we perform following steps -
		   1. Move the position of the file stream to the end of file
		   2. Check the file end position
		   3. Move the position of the file stream to the beginning of file

		fseek(pageFile, 0L, SEEK_END);
		int totalSize = ftell(pageFile);
		fseek(pageFile, 0L, SEEK_SET);
		fHandle->totalNumPages = totalSize/ PAGE_SIZE;  */

		/* Using fstat() to get the file total size.
		   fstat() is a system call that is used to determine information about a file based on its file descriptor.
		   'st_size' member variable of the 'stat' structure gives the total size of the file in bytes.
		*/

		struct stat fileInfo;
		if(fstat(fileno(pageFile), &fileInfo) < 0)
			return RC_FILE_NOT_FOUND;
		fHandle->totalNumPages = fileInfo.st_size/ PAGE_SIZE;

		// Closing file stream so that all the buffers are flushed.
		fclose(pageFile);
		return RC_OK;
	}
}

extern RC closePageFile (SM_FileHandle *fHandle) {
	// Checking if file pointer or the storage manager is intialised. If initialised, then close.
	if(pageFile != NULL)
		pageFile = NULL;
	return RC_OK;
}


extern RC destroyPageFile (char *fileName) {
	// Opening file stream in read mode. 'r' mode creates an empty file for reading only.
	pageFile = fopen(fileName, "r");

	if(pageFile == NULL)
		return RC_FILE_NOT_FOUND;

	// Deleting the given filename so that it is no longer accessible.
	remove(fileName);
	return RC_OK;
}

extern RC readBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage) {
	// Checking if the pageNumber parameter is less than Total number of pages and less than 0, then return respective error code
	if (pageNum > fHandle->totalNumPages || pageNum < 0)
        	return RC_READ_NON_EXISTING_PAGE;

	// Opening file stream in read mode. 'r' mode creates an empty file for reading only.
	pageFile = fopen(fHandle->fileName, "r");

	// Checking if file was successfully opened.
	if(pageFile == NULL)
		return RC_FILE_NOT_FOUND;

	// Setting the cursor(pointer) position of the file stream. Position is calculated by Page Number x Page Size
	// And the seek is success if fseek() return 0
	int isSeekSuccess = fseek(pageFile, (pageNum * PAGE_SIZE), SEEK_SET);
	if(isSeekSuccess == 0) {
		// We're reading the content and storing it in the location pointed out by memPage.
		fread(memPage, sizeof(char), PAGE_SIZE, pageFile);
	} else {
		return RC_READ_NON_EXISTING_PAGE;
	}

	// Setting the current page position to the cursor(pointer) position of the file stream
	fHandle->curPagePos = ftell(pageFile);

	// Closing file stream so that all the buffers are flushed.
	fclose(pageFile);

    	return RC_OK;
}

extern int getBlockPos (SM_FileHandle *fHandle) {
	// Returning the current page position retrieved from the file handle
	return fHandle->curPagePos;
}

extern RC readFirstBlock (SM_FileHandle *fHandle, SM_PageHandle memPage) {
	// Re-directing (passing) to readBlock(...) function with pageNumber = 0 i.e. first block
	return readBlock(0, fHandle, memPage);
}

extern RC readPreviousBlock (SM_FileHandle *fHandle, SM_PageHandle memPage) {
	//printf("CURRENT PAGE POSITION = %d \n", fHandle->curPagePos);
	//printf("TOTAL PAGES = %d \n", fHandle->totalNumPages);

	// Calculating current page number by dividing page size by current page position
	int currentPageNumber = fHandle->curPagePos / PAGE_SIZE;

	// Re-directing (passing) to readBlock(...) function with pageNumber = currentPagePosition - 1 i.e. previous block
	return readBlock(currentPageNumber - 1, fHandle, memPage);
}

extern RC readCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage) {
	// Calculating current page number by dividing page size by current page position
	int currentPageNumber = fHandle->curPagePos / PAGE_SIZE;

	// Re-directing (passing) to readBlock(...) function with pageNumber = currentPagePosition i.e. current block
	return readBlock(currentPageNumber, fHandle, memPage);
}

extern RC readNextBlock (SM_FileHandle *fHandle, SM_PageHandle memPage){
	// Calculating current page number by dividing page size by current page position
	int currentPageNumber = fHandle->curPagePos / PAGE_SIZE;

	printf("CURRENT PAGE POSITION = %d \n", fHandle->curPagePos);
	// Re-directing (passing) to readBlock(...) function with pageNumber = currentPagePosition + 1 i.e. last block
	return readBlock(currentPageNumber + 1, fHandle, memPage);
}

extern RC readLastBlock (SM_FileHandle *fHandle, SM_PageHandle memPage){
	// Re-directing (passing) to readBlock(...) function with pageNumber = totalNumPages i.e. last block
	// printf("TOTAL PAGES = %d \n", fHandle->totalNumPages);
	return readBlock(fHandle->totalNumPages - 1, fHandle, memPage);
}

/* writing blocks to a page file */
extern RC writeBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage)
//Rebecca Thomson
{//write a SINGLE page to disk of given position
    //First, check that pageNum is valid
    if (pageNum>fHandle->totalNumPages|| pageNum<0){return RC_WRITE_FAILED;}
    //Check that PageHandle is not larger than PAGE_SIZE
    if (sizeof(memPage)>PAGE_SIZE){return RC_IM_N_TO_LAGE;}
    //Checking that file exists.  Use read/write so file open for next step.
    pageFile = fopen(fHandle->fileName, "r+" );
    if (pageFile==NULL){return RC_FILE_NOT_FOUND;}
    //seek to position, check
	int seekCheck =fseek(pageFile, (pageNum * PAGE_SIZE), SEEK_SET);
	if (seekCheck == 0) {
		//now write
		fwrite(memPage, sizeof(char), sizeof(memPage), pageFile);
		//reset current page position
		fHandle->curPagePos = pageNum;
		//close it down
		fclose(pageFile);
		//if operation successful, return RC_OK
		return RC_OK;
	} else {
		fclose(pageFile);
		return RC_WRITE_FAILED;
	}
  }
extern RC writeCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
//Rebecca Thomson
{//write a page to disk using the current position
	int tempPageNum = fHandle->curPagePos ;
    if(RC_OK == writeBlock(tempPageNum,fHandle,memPage))
        return RC_OK;
    else
        return RC_WRITE_FAILED;
    //
    //if operation successful, return RC_OK
    return RC_OK;}
extern RC appendEmptyBlock (SM_FileHandle *fHandle)
//Rebecca Thomson
//Increase the number of pages in the file by one.  The new last page of the file shall have zero bytes.
{
    //
    //create new empty page

	SM_PageHandle newPage = (SM_PageHandle)calloc(PAGE_SIZE,sizeof(char));

	//seek to the last page, check exists
	int tempCursorPoint = fHandle->totalNumPages*PAGE_SIZE;
	int checkseek = fseek(pageFile, tempCursorPoint , SEEK_SET);
	if(checkseek != 0) {
		free(newPage); //Don't lose memory
		return RC_WRITE_FAILED;
	}else {
		//At last page, write new page
		fwrite(newPage,sizeof(char),PAGE_SIZE,pageFile);
		//todo update the block lists in the memory data structure (mgmtInfo)

		//todo update header file.
	}
	//empty NewPage
	free(newPage);
	//update totalNumPages
	fHandle->totalNumPages++;

    return RC_OK;


}
extern RC ensureCapacity (int numberOfPages, SM_FileHandle *fHandle)
//Rebecca Thomson
// if file  totalNumPages< number of Pages, then increase file size to number of pages
// will call function appendEmptyBlock
{
		//check exists
	pageFile = fopen(fHandle->fileName, "r+" );
	if(pageFile == NULL){return RC_FILE_NOT_FOUND;}
	//loop for additions
	while ((fHandle->totalNumPages)<numberOfPages){appendEmptyBlock (fHandle); }
	//close open
	fclose(pageFile);
    //after inserting the correct number of pages,  return RC_OK
	return RC_OK;
}