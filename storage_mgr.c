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

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include "storage_mgr.h"
#include "dberror.h"
FILE *filePointer;


/*
 * Function: initStorageManager
 * ---------------------------
 * Initializes storageManager for further actions on it.
 * Input:
 *    no input parameter
 * Output:
 *    void
 */
extern void initStorageManager()
{
    printf("======================================\n");
    printf("extern void initStorageManager()\n");
    printf("Initializing Storage Manager !\n");
    filePointer = NULL;
}


/*
 * Function: createPageFile
 * ---------------------------
 * Initializes storageManager for further actions on it.
 * Input:
 *    fileName : pointer to the filename of the file to be created.
 * Output:
 *    RC: RC_OK if successful, error code otherwise.
 */
extern RC createPageFile(char *fileName)
{
    printf("log entry: extern RC createPageFile(char *fileName)\n");
    // Opening file stream in read & write mode.
    //'w+' mode creates an empty file for both reading and writing.
    filePointer = fopen(fileName, "w+");
    // Checking if the file pointer is Null
    if (filePointer == NULL)
    {
        RC_message = "Failure encountered in the file creation";
        return  RC_ERROR;
    }
    int zero=0;
    int one=1;
    int charSize = sizeof(char);
    // Setting memory for an empty page
    SM_PageHandle smPageHandlePtr = (SM_PageHandle)calloc(PAGE_SIZE, (charSize * one));

    // Returns error code if the fwrite does not write properly
    if (fwrite(smPageHandlePtr, charSize * one, PAGE_SIZE, filePointer) >= PAGE_SIZE)
    {
        // Seeking to the end of file using SEEK_END
        fseek(filePointer, zero, SEEK_END);

        // Closing the file
        //pointed to by filePointer
        fclose(filePointer);

        // Free or releasing the memory
        //held by smPageHandlePtr
        free(smPageHandlePtr);

        RC_message = "Creation of file is successful";
        return RC_OK;
    }
    else
    {
        RC_message = "Failure in writing the file";
        return RC_WRITE_FAILED;
    }
}




/*
 * Function: openPageFile
 * ---------------------------
 * Initializes storageManager for further actions on it.
 * Input:
 *    fileName : pointer to the filename of the file to be created.
 *    fHandle  : pointer to SM_FileHandle object
 * Output:
 *    RC: RC_OK if successful, error code otherwise.
 */
extern RC openPageFile(char *fileName, SM_FileHandle *fHandle)
{
    printf("log entry: extern RC openPageFile(char *fileName, SM_FileHandle *fHandle)\n");
    filePointer = fopen(fileName, "r+");
    int one=1;
    int zero=0;
    // Check and return file not found if the file is not present
    if (filePointer == NULL)
    {
        printf("file pointer is null \n");
        return RC_FILE_NOT_FOUND;
    }
    else
    {
        if (fseek(filePointer, zero, SEEK_END) != 0)
        {
            fclose(filePointer);
            return RC_ERROR;
        }

        // Stores the size of file in a variable
        int sizeOfFile = ftell(filePointer);
        printf("FileSize (sizeOfFile) = %d\n", sizeOfFile);

        // Checks if the ftell returns -1, if so return error
        if (sizeOfFile == -1)
        {
            fclose(filePointer);
            return RC_ERROR;
        }

        int numberOfPages = (sizeOfFile * one) / PAGE_SIZE;
        printf("Total Number of Pages (numberOfPages)= %d\n", numberOfPages);
        printf("The file name is (fileName) = %s\n", fileName);

        // Setting the fHandle->fileName
        fHandle->fileName = strdup(fileName);  // Deep copy to avoid pointer issues

        // Setting the fHandle->mgmtInfo
        fHandle->mgmtInfo = filePointer;

        // Setting the total number of pages(fHandle->totalNumPages)
        fHandle->totalNumPages = numberOfPages * one;

        // Setting the current page position(fHandle->curPagePos)
        fHandle->curPagePos = zero;

        // Rewinding the file pointer to beginning of the file
        rewind(filePointer);
        // return OK code
        return RC_OK;
    }
}



/*
 * Function: closePageFile
 * ---------------------------
 * Initializes storageManager for further actions on it.
 * Input:
 *    fHandle  : pointer to SM_FileHandle object
 * Output:
 *    RC: RC_OK if successful, error code otherwise.
 */
extern RC closePageFile(SM_FileHandle *fHandle)
{
    printf("log entry: extern RC closePageFile(SM_FileHandle *fHandle)\n");
    if (fHandle == NULL || fHandle->mgmtInfo == NULL)
    {
        printf("fHandle == NULL || fHandle->mgmtInfo == NULL \n");
        RC_message = "Invalid file handle. File handle not initialized";
        return RC_ERROR;
    }
    // Check and return error if the fclose returns EOF
    if (fclose((FILE *)fHandle->mgmtInfo) == EOF)
    {
        printf("fclose((FILE *)fHandle->mgmtInfo) == EOF \n");
        RC_message = "Error encountered in closing of the file";
        return RC_ERROR;
    }
    else
    {
        printf("The file was closed successfully \n");
        RC_message = "The file closed successfully";
        return RC_OK;
    }
}


/*
 * Function: destroyPageFile
 * ---------------------------
 * Initializes storageManager for further actions on it.
 * Input:
 *    fileName : pointer to the filename of the file to be created.
 * Output:
 *    RC: RC_OK if successful, error code otherwise.
 */

extern RC destroyPageFile(char *fileName)
{
    printf("log entry: extern RC destroyPageFile(char *fileName)\n");
    // Storing the result returned by remove function
    int removeFileResult = remove(fileName);
    int negOne = -1;
    // Return error if res is negOne
    if (removeFileResult == negOne)
    {
        printf("removeFileResult == Failure. The file could not be deleted\n");
        RC_message = "Error occurred while destroying file";
        return RC_ERROR;
    }
    RC_message = "File destroyed successfully";
    return RC_OK;
}



/*
 * Function: readBlock
 * -------------------
 * Reads a specific block (page) from the file into memory.
 *
 * blockNumber: The page number to read (starting from 0).
 * fileHandle: Pointer to the file handle structure.
 * memoryPage: Memory location where the data will be stored.
 *
 * returns: RC_OK if successful, error code otherwise.
 */
RC readBlock(int blockNumber, SM_FileHandle *fileHandle, SM_PageHandle memoryPage) {
    // Check if the block number is invalid (negative or beyond total number of pages)
    if (blockNumber < 0 || blockNumber >= fileHandle->totalNumPages) {
        return RC_READ_NON_EXISTING_PAGE; // Invalid page access
    } else {
        // Open the file in read-only mode
        int fileDescriptor = open(fileHandle->fileName, O_RDONLY);
        if (fileDescriptor < 0) // If file couldn't be opened
            return RC_FILE_NOT_FOUND;

        // Move the file pointer to the starting byte of the desired page
        int offset = lseek(fileDescriptor, (PAGE_SIZE * blockNumber), SEEK_SET);
        if (offset >= 0) {
            read(fileDescriptor, memoryPage, PAGE_SIZE); // Read PAGE_SIZE bytes into memoryPage
            fileHandle->curPagePos = blockNumber;        // Update the current page position
            close(fileDescriptor);                       // Close the file after reading
            return RC_OK;                                // Successful read
        } else {
            close(fileDescriptor);                       // Ensure the file is closed even on failure
            return RC_READ_NON_EXISTING_PAGE;            // Page offset could not be set
        }
    }
}

/*
 * Function: getBlockPos
 * ---------------------
 * Retrieves the current position of the file pointer (current page number).
 *
 * fileHandle: Pointer to the file handle structure.
 *
 * returns: The current page position in the file.
 */
int getBlockPos(SM_FileHandle *fileHandle) {
    return fileHandle->curPagePos;
}

/*
 * Function: readFirstBlock
 * ------------------------
 * Reads the first block (page) of the file into memory.
 *
 * fileHandle: Pointer to the file handle structure.
 * memoryPage: Memory location where the data will be stored.
 *
 * returns: RC_OK if successful, error code otherwise.
 */
RC readFirstBlock(SM_FileHandle *fileHandle, SM_PageHandle memoryPage) {
    int fileDescriptor = open(fileHandle->fileName, O_RDONLY); // Open file for reading
    if (fileDescriptor < 0)
        return RC_FILE_NOT_FOUND; // File does not exist or cannot be opened

    int offset = lseek(fileDescriptor, 0, SEEK_SET); // Move to the start of the file (offset 0)
    if (offset >= 0) {
        read(fileDescriptor, memoryPage, PAGE_SIZE); // Read the first page
        fileHandle->curPagePos = 0;                  // Set current page position to 0
        close(fileDescriptor);                       // Close the file
        return RC_OK;
    } else {
        close(fileDescriptor);                       // Close file even if seeking fails
        return RC_READ_NON_EXISTING_PAGE;            // Error seeking to the first page
    }
    printf(" total pages %d./n",fileHandle->totalNumPages);
}

/*
 * Function: readPreviousBlock
 * ---------------------------
 * Reads the previous block relative to the current page position.
 *
 * fileHandle: Pointer to the file handle structure.
 * memoryPage: Memory location where the data will be stored.
 *
 * returns: RC_OK if successful, error code otherwise.
 */
RC readPreviousBlock(SM_FileHandle *fileHandle, SM_PageHandle memoryPage) {
    int fileDescriptor = open(fileHandle->fileName, O_RDONLY);
    if (fileDescriptor < 0)
        return RC_FILE_NOT_FOUND;

    // Check if current page is the first page (no previous block exists)
    if (fileHandle->curPagePos == 0) {
        close(fileDescriptor);
        return RC_READ_NON_EXISTING_PAGE;
    }

    // Move the file pointer to the previous block
    int offset = lseek(fileDescriptor, (fileHandle->curPagePos - 1) * PAGE_SIZE, SEEK_SET);
    if (offset >= 0) {
        read(fileDescriptor, memoryPage, PAGE_SIZE); // Read the previous block
        fileHandle->curPagePos--;                    // Update current page position
        close(fileDescriptor);
        return RC_OK;
    } else {
        close(fileDescriptor);
        return RC_READ_NON_EXISTING_PAGE;            // Error seeking to the previous page
    }
}

/*
 * Function: readCurrentBlock
 * --------------------------
 * Reads the current block (page) based on the current page position.
 *
 * fileHandle: Pointer to the file handle structure.
 * memoryPage: Memory location where the data will be stored.
 *
 * returns: RC_OK if successful, error code otherwise.
 */
RC readCurrentBlock(SM_FileHandle *fileHandle, SM_PageHandle memoryPage) {
    int fileDescriptor = open(fileHandle->fileName, O_RDONLY);
    if (fileDescriptor < 0)
        return RC_FILE_NOT_FOUND;

    // Move the file pointer to the current page's position
    int offset = lseek(fileDescriptor, fileHandle->curPagePos * PAGE_SIZE, SEEK_SET);
    if (offset >= 0) {
        read(fileDescriptor, memoryPage, PAGE_SIZE); // Read the current page
        close(fileDescriptor);
        return RC_OK;
    } else {
        close(fileDescriptor);
        return RC_READ_NON_EXISTING_PAGE;            // Error reading current page
    }
}

/*
 * Function: readNextBlock
 * -----------------------
 * Reads the next block relative to the current page position.
 *
 * fileHandle: Pointer to the file handle structure.
 * memoryPage: Memory location where the data will be stored.
 *
 * returns: RC_OK if successful, error code otherwise.
 */
RC readNextBlock(SM_FileHandle *fileHandle, SM_PageHandle memoryPage) {
    int fileDescriptor = open(fileHandle->fileName, O_RDONLY);
    if (fileDescriptor < 0)
        return RC_FILE_NOT_FOUND;

    // Check if we are already at the last page
    if (fileHandle->curPagePos >= fileHandle->totalNumPages - 1) {
        close(fileDescriptor);
        return RC_READ_NON_EXISTING_PAGE;
    }

    // Move to the next page
    int offset = lseek(fileDescriptor, (fileHandle->curPagePos + 1) * PAGE_SIZE, SEEK_SET);
    if (offset >= 0) {
        read(fileDescriptor, memoryPage, PAGE_SIZE); // Read the next page
        fileHandle->curPagePos++;                    // Update the current page position
        close(fileDescriptor);
        return RC_OK;
    } else {
        close(fileDescriptor);
        return RC_READ_NON_EXISTING_PAGE;            // Error seeking to the next page
    }
}

/*
 * Function: readLastBlock
 * -----------------------
 * Reads the last block (page) of the file into memory.
 *
 * fileHandle: Pointer to the file handle structure.
 * memoryPage: Memory location where the data will be stored.
 *
 * returns: RC_OK if successful, error code otherwise.
 */
RC readLastBlock(SM_FileHandle *fileHandle, SM_PageHandle memoryPage) {
    int fileDescriptor = open(fileHandle->fileName, O_RDONLY);
    if (fileDescriptor < 0)
        return RC_FILE_NOT_FOUND;

    // Move the file pointer to the start of the last block
    int offset = lseek(fileDescriptor, (fileHandle->totalNumPages - 1) * PAGE_SIZE, SEEK_SET);
    if (offset >= 0) {
        read(fileDescriptor, memoryPage, PAGE_SIZE);         // Read the last block
        fileHandle->curPagePos = fileHandle->totalNumPages - 1; // Update position to last page
        close(fileDescriptor);
        return RC_OK;
    } else {
        close(fileDescriptor);
        return RC_READ_NON_EXISTING_PAGE;                    // Error reading the last page
    }
}
/* writing blocks to a page file
 *write a SINGLE page to disk of given position
 *pageNum: given page number, starts from zero
 *fHandle: Pointer to file handle structure
 * memPage Information to be added.  This is function is for a single page, it can't be over PAGE_SIZE
 */
extern RC writeBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage)

{//
    //First, check that pageNum is valid
    if (pageNum>fHandle->totalNumPages|| pageNum<0){return RC_WRITE_FAILED;}
    //Check that PageHandle is not larger than PAGE_SIZE
    if (sizeof(memPage)>PAGE_SIZE){return RC_IM_N_TO_LAGE;}
    //Checking that file exists.  Use read/write so file open for next step.
    filePointer = fopen(fHandle->fileName, "r+" );
    if (filePointer==NULL){return RC_FILE_NOT_FOUND;}
    //seek to position, check
	int seekCheck =fseek(filePointer, (pageNum * PAGE_SIZE), SEEK_SET);

	if (seekCheck == 0) {
		//now write
		fwrite(memPage, sizeof(char), strlen(memPage), filePointer);
		//reset current page position
		fHandle->curPagePos = pageNum;
		//close it down
		fclose(filePointer);
		//if operation successful, return RC_OK

		return RC_OK;
	} else {
		fclose(filePointer);
		return RC_WRITE_FAILED;
	}
  }
/*write the current block to disk
 *write a SINGLE page to disk
 *fHandle: Pointer to file handle structure
 * memPage Information to be added.  This is function is for a single page, it can't be over PAGE_SIZE
 *
 * This function will determine the current page and call WriteBlock
*/
extern RC writeCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)

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
/*
Increase the number of pages in the file by one.  The new last page of the file shall have zero bytes.

*/

{
    /*
    //This block of code was to originally create small individual page files.  We are appending blocks
    //into the existing file.The page file shall be created with a automatically generated name based on its page number.
    //The file needs to be linked other pages.
    //Testing for beginning Total Page Number
    char filename[256];//get a temp name for the new file.
    int newTotal= (fHandle->totalNumPages)+1;
    sprintf(filename, "file_%d.bin", newTotal);



    printf("New file name: %s\n", filename);
    //use createPageFile to add to overall File in next project itteration
    if (RC_OK == createPageFile(filename)) {
        //update totalNumPages
        //fHandle->totalNumPages=newTotal;
        return RC_OK;
    }else {
        return RC_APPEND_FAILED;
    }
*/

    printf("log entry: extern RC appendEmptyBlock(SM_FileHandle *fHandle) \n");
    //creating a blank page in memory
	SM_PageHandle newPage = (SM_PageHandle)calloc(PAGE_SIZE,1);
    //setting filePointer,use append!
    //Check file exists

    filePointer = fopen(fHandle->fileName, "a+" );
    // Stores the size of file in a variable, print size of existing file.
    //int sizeOfFile = ftell(fHandle->fileName);
    //printf("FileSize (sizeOfFile) = %d\n", sizeOfFile);
    if (filePointer==NULL) {
        fclose(filePointer);
        printf("File Not Found.  Append has stopped.\n");
        return RC_APPEND_FAILED;
    }
	//seek to the last page
    filePointer = fopen(fHandle->fileName, "a+" );
    //printf("total pages: %d.\n", fHandle->totalNumPages);

    //setting pointer to end of file.
    fseek(filePointer, 0 , SEEK_END);


    //At last page, write new page
	   
    fwrite(newPage,sizeof(char),PAGE_SIZE,filePointer);
		//todo update the block lists in the memory data structure (mgmtInfo)

		//todo update header file.

	//empty NewPage
	free(newPage);
    //printf("FileSize (sizeOfFile) = %d\n", sizeOfFile);
    fclose(filePointer);
	//update totalNumPages
    fHandle->totalNumPages++;
    //printf("Total Number of Pages Now: %d.\n",fHandle->totalNumPages);
    return RC_OK;

}
extern RC ensureCapacity (int numberOfPages, SM_FileHandle *fHandle)
//Rebecca Thomson
// if file  totalNumPages< number of Pages, then increase file size to number of pages
// will call function appendEmptyBlock
/*
{
		//check exists
	filePointer = fopen(fHandle->fileName, "a+" );
	if(filePointer == NULL) {
	    printf("File not found when calling ensureCapacity function\n");
	    return RC_FILE_NOT_FOUND;
	}



    int enumberOfPages = fHandle->totalNumPages*1;
	//loop for additions
	while ((enumberOfPages*1)<numberOfPages){appendEmptyBlock (fHandle); }
	//close open
	fclose(filePointer);
    //after inserting the correct number of pages,  return RC_OK
	return RC_OK;
}*/
{
    printf("log entry: ensureCapacity method \n");
    int one=1;
    // Check to see if the capacity
    // to accomodate for the  required number of
    //  pages is present  or not
    if (numberOfPages <= (fHandle->totalNumPages) * one)
    {
        printf("Capacity left is not enough. \n");
        RC_message = "Capacity left is not enough";
        return RC_ERROR;
    }
    int totalNoOfPages = (fHandle->totalNumPages) * one;
    int requiredNumberOfPages = (numberOfPages - totalNoOfPages) * one;

    // Append empty blocks for
    // the required extra number of the pages
    int k = 0;
    while (k < (requiredNumberOfPages * one))
    {
        appendEmptyBlock(fHandle);
        k = k + 1;
    }
    printf("log exit: ensureCapacity method \n");
    return RC_OK;
}