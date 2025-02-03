#include "storage_mgr.h"
#include <sys/stat.h>
#include "dberror.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>


//global variable filePointer declaration
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

    int charSize = sizeof(char);

    // Setting memory for an empty page
    //SM_PageHandle smPageHandlePtr = (SM_PageHandle)calloc(PAGE_SIZE, sizeof(char));
    SM_PageHandle smPageHandlePtr = (SM_PageHandle)calloc(PAGE_SIZE, charSize);
    

    

    // Returns error code if the fwrite does not write properly
    //if (fwrite(smPageHandlePtr, sizeof(char), PAGE_SIZE, filePointer) >= PAGE_SIZE)
    if (fwrite(smPageHandlePtr, charSize, PAGE_SIZE, filePointer) >= PAGE_SIZE)
    {
        // Seeking to the end of file using SEEK_END
        fseek(filePointer, 0, SEEK_END);

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

    // Check and return file not found if the file is not present
    if (filePointer == NULL)
    {
        printf("file pointer is null \n");
        return RC_FILE_NOT_FOUND;
    }
    else
    {
        //fseek(filePointer, 0, SEEK_END);
        if (fseek(filePointer, 0, SEEK_END) != 0) 
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
        //fHandle->fileName = fileName;
        fHandle->fileName = strdup(fileName);  // Deep copy to avoid pointer issues

        // Setting the fHandle->mgmtInfo
        fHandle->mgmtInfo = filePointer;

        // Setting the total number of pages(fHandle->totalNumPages)
        fHandle->totalNumPages = numberOfPages * one;

        // Setting the current page position(fHandle->curPagePos)
        fHandle->curPagePos = 0;

        // Rewinding the file pointer to beginning of the file
        rewind(filePointer);

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
    //if (fclose(fHandle->mgmtInfo) == EOF)
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

    // Return error if res is -1
    if (removeFileResult == -1)
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
 * Input: 
 *    pageNum: The page number to read (starting from 0).
 *    fHandle: Pointer to the file handle structure.
 *    memPage: Memory location where the data will be stored.
 * 
 * Output: 
 *    RC_OK if successful, error code otherwise.
 */
extern RC readBlock(int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage)
{
    printf("log entry: extern RC readBlock(int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage)\n");


    int one=1;
    // If the page number (pageNum) is greater than total number of pages(totalNumPages) 
    //or page number (pageNum) is less than 0, 
    // then return error
    if ( pageNum >= (fHandle->totalNumPages * one) ||  pageNum < (0 * one) )
    {
        RC_message = "Reading a page which is non-existent.";
        return RC_READ_NON_EXISTING_PAGE;
    }
    
    int charSize = sizeof(char);
    //printf("sizeof(char)==%d \n",charSize);

    // Calculating the position in the file 
    //for the requested pageNum
    
    //int offset = pageNum * PAGE_SIZE * sizeof(char) * one;
    int offset = one * charSize * pageNum  *  PAGE_SIZE ;
    printf("offset==%d \n",offset);




    // Checking if using the fseek 
    // returns a value which is not 0. 
    //If this is the case, then, 
    //there is error encountered in 
    //seeking the correct position value.
    if (fseek(fHandle->mgmtInfo, offset, SEEK_SET) != 0)
    {
        RC_message = "An error was encountered in seeking of the correct offset position in the fHandle variable";
        return RC_ERROR;
    }

    // Read a block, whose size is PAGE_SIZE value, 
    //and stores its data in the vaiable memPage
    //if (fread(memPage, sizeof(char), PAGE_SIZE, fHandle->mgmtInfo) != PAGE_SIZE)
    if (fread( memPage, charSize * one, PAGE_SIZE, fHandle->mgmtInfo)  != PAGE_SIZE)
    {
        RC_message = "Failure encountered in reading of the  block of data !!";
        return RC_READ_FAILED;
    }

    // Set the value of current  page Position (fHandle->curPagePos) to value of the variable pageNum
    fHandle->curPagePos = pageNum * one;

    RC_message = "Successfully read the file data.";
    return RC_OK;

}

































/*
 * Function: getBlockPos
 * ---------------------
 * Retrieves the current position of the file pointer (current page number).
 * Input:
 *   fHandle: Pointer to the file handle structure.
 * Output:
 *   The current page position in the file.
 */
extern int getBlockPos(SM_FileHandle *fHandle)
{
    printf("log entry: extern int getBlockPos(SM_FileHandle *fHandle)\n");
    int one=1;

    // Returning the position of the currentPage 
    //inside the fileHandle pointer
    int currentPagePosition = (fHandle->curPagePos) * one;
    return currentPagePosition;
}










































/*
 * Function: readFirstBlock
 * ------------------------
 * Reads the first block (page) of the file into memory.
 * Input: 
 *   fHandle: Pointer to the file handle structure.
 *   memPage: Memory location where the data will be stored.
 * Output:
 *   RC_OK if successful, error code otherwise.
 */

extern RC readFirstBlock(SM_FileHandle *fHandle, SM_PageHandle memPage)
{
    printf("log entry: extern RC readFirstBlock(SM_FileHandle *fHandle, SM_PageHandle memPage) \n");
    int zeroOffset=0;
    // Returns the first block from the disk
    return readBlock(zeroOffset, fHandle, memPage);
}



































/*
 * Function: readPreviousBlock
 * ---------------------------
 * Reads the previous block relative to the current page position.
 * Input: 
 *   fHandle: Pointer to the file handle structure.
 *   memPage: Memory location where the data will be stored.
 * Output: 
 *   RC_OK if successful, error code otherwise.
 */

extern RC readPreviousBlock(SM_FileHandle *fHandle, SM_PageHandle memPage)
{
    printf("log entry: extern RC readPreviousBlock(SM_FileHandle *fHandle, SM_PageHandle memPage) \n");
    int one=1;
    // Returns the previous block from the disk
    return readBlock( ((fHandle->curPagePos - 1) * one), fHandle, memPage);
}







































/*
 * Function: readCurrentBlock
 * --------------------------
 * Reads the current block (page) based on the current page position.
 * Input:  
 *   fHandle: Pointer to the file handle structure.
 *   memPage: Memory location where the data will be stored.
 * Output: 
 *   RC_OK if successful, error code otherwise.
 */
extern RC readCurrentBlock(SM_FileHandle *fHandle, SM_PageHandle memPage)
{
    printf("log entry: extern RC readCurrentBlock(SM_FileHandle *fHandle, SM_PageHandle memPage) \n");
    int one=1;
    // Returns the current block from the disk
    return readBlock(((fHandle->curPagePos) * one), fHandle, memPage);
}









































/*
 * Function: readNextBlock
 * -----------------------
 * Reads the next block relative to the current page position.
 * Input:
 * fHandle: Pointer to the file handle structure.
 * memPage: Memory location where the data will be stored.
 * Output:
 *   RC_OK if successful, error code otherwise.
 */

extern RC readNextBlock(SM_FileHandle *fHandle, SM_PageHandle memPage)
{
    printf("log entry: extern RC readNextBlock(SM_FileHandle *fHandle, SM_PageHandle memPage) \n");
    int one=1;
    // Returns the next block of data from the file on the disk
    //return readBlock((fHandle->curPagePos) + 1, fHandle, memPage);
    return readBlock(((fHandle->curPagePos) + 1) * one, fHandle, memPage);
}




































/*
 * Function: readLastBlock
 * -----------------------
 * Reads the last block (page) of the file into memory.
 * Input: 
 *   fHandle: Pointer to the file handle structure.
 *   memPage: Memory location where the data will be stored.
 * Output:
 *   RC_OK if successful, error code otherwise.
 */

extern RC readLastBlock(SM_FileHandle *fHandle, SM_PageHandle memPage)
{
    printf("log entry: extern RC readLastBlock(SM_FileHandle *fHandle, SM_PageHandle memPage) \n");
    int one=1;
    // Returns the last block of data from the file on the disk
    //return readBlock(fHandle->totalNumPages - 1, fHandle, memPage);
    return readBlock(((fHandle->totalNumPages) - 1) * one, fHandle, memPage);
}



































/*
 * Function: writeBlock
 * -------------------
 * Reads a specific block (page) from the file into memory.
 * 
 * Input: 
 *    pageNum: The page number to write.
 *    fHandle: Pointer to the file handle structure.
 *    memPage: Memory location where the data will be stored.
 * 
 * Output: 
 *    RC: RC_OK if successful, error code otherwise.
 */
extern RC writeBlock(int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage)
{
    printf("log entry: extern RC writeBlock(int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage) \n");
    printf("pageNum=%d \n", pageNum);

    int one=1;
    // Set the value for the filePointer
    filePointer = (FILE *)fHandle->mgmtInfo;

    // If the pageNum is less than 0 
    // or greater than total number of pages return error
    if ( pageNum > ((fHandle->totalNumPages) * one) || pageNum < 0)
    {
        printf("Trying to read data from a page which is non-existent. \n");
        RC_message = "Trying to read data from a page which is non-existent.";
        return RC_WRITE_FAILED;
    }

    // Returns error when filePointer is null
    if (filePointer == NULL)
    {
        printf("filePointer value is NULL \n");
        RC_message = "filePointer value is NULL";
        return RC_FILE_NOT_FOUND;
    }

    // Getting the size of char data type
    int charSize = sizeof(char);

    // Calculating the position in the file for the requested pageNum
    int offsetValue = pageNum * PAGE_SIZE * charSize * one ;

    // Checking if the fseek returns a number other than 0, 
    // if so, there is error in seeking the correct position
    if (fseek(filePointer, offsetValue, SEEK_SET) != 0)
    {
        printf("Error encountered in seeking the correct offset position in the file ! \n");
        RC_message = "Error encountered in seeking the correct offset position in the file !";
        return RC_WRITE_FAILED;
    }

    // Writing the contents from memPage to the requested pageNum
    //fwrite(memPage, sizeof(char), PAGE_SIZE, filePointer);
    fwrite(memPage, (charSize * one), PAGE_SIZE, filePointer);
    printf("Writing of the data is successful. \n");

    RC_message = "Writing of the data is successful";
    return RC_OK;
}








































/*
 * Function: writeCurrentBlock
 * -------------------
 * Reads a specific block (page) from the file into memory.
 * 
 * Input: 
 *    fHandle: Pointer to the file handle structure.
 *    memPage: Memory location where the data will be stored.
 * 
 * Output: 
 *    RC: RC_OK if successful, error code otherwise.
 */
extern RC writeCurrentBlock(SM_FileHandle *fHandle, SM_PageHandle memPage)
{
    printf("log entry: extern RC writeCurrentBlock(SM_FileHandle *fHandle, SM_PageHandle memPage) \n");
    
    // get block position
    int one=1;
    int blockPosition = getBlockPos(fHandle); 
    blockPosition = blockPosition * one;

    // write the data contents from the memPage variable 
    // into the disk at the location of 
    //the current position of fHandle variable
    return writeBlock(blockPosition, fHandle, memPage);
}










































/*
 * Function: appendEmptyBlock
 * -------------------
 * Reads a specific block (page) from the file into memory.
 * 
 * Input: 
 *    fHandle: Pointer to the file handle structure.
 * 
 * Output: 
 *    RC: RC_OK if successful, error code otherwise.
 */
extern RC appendEmptyBlock(SM_FileHandle *fHandle)
{
    printf("log entry: extern RC appendEmptyBlock(SM_FileHandle *fHandle) \n");

    int one = 1;
    int zero = 0;

    // Check to see the file handle is null or not.
    // If file handle is null, return proper error message
    if(fHandle == NULL) 
    {
        RC_message = "File handle has not been correctly initialized, and it is null";

        return RC_FILE_HANDLE_NOT_INIT ;
    }

    // Setting the filePointer
    filePointer = (FILE *)fHandle->mgmtInfo;

    // If mgmtInfo inside the fHandle is null,
    // then return error
    if (filePointer == NULL)
    {
        RC_message = "The file not found error!";
        return RC_FILE_NOT_FOUND;
    }

    // If fseek  does not return 0
    // return proper error message
    
    if (fseek(filePointer, zero, SEEK_END) != 0)
    {
        RC_message = "fseek of the file has failed";
        return RC_WRITE_FAILED;
    }

    // Allocate a block of size equal to PAGE_SIZE
    // and initialize it to an empty pointer
    SM_PageHandle sm_PageHandle_ptr = (SM_PageHandle)calloc(PAGE_SIZE, one);

    // Initialize  an empty block using fwrite
    fwrite(sm_PageHandle_ptr, one, PAGE_SIZE, filePointer);

    // fSeek to the end of the file
    fseek(filePointer, zero, SEEK_END);

    // Increment the total number of pages
    // in fHandle by 1
    fHandle->totalNumPages = fHandle->totalNumPages + one;

    return RC_OK;
}




































/*
 * Function: ensureCapacity
 * -------------------
 * Reads a specific block (page) from the file into memory.
 * 
 * Input: 
 *    numberOfPages: number of pages.
 *    fHandle: Pointer to the file handle structure.
 * 
 * Output: 
 *    RC: RC_OK if successful, error code otherwise.
 */
extern RC ensureCapacity(int numberOfPages, SM_FileHandle *fHandle)
{
    printf("log entry: extern RC ensureCapacity(int numberOfPages, SM_FileHandle *fHandle) \n");

    int one=1;

    // Check to see if the capacity 
    // to accomodate for the  required number of
    //  pages is present  or not
    if (numberOfPages <= (fHandle->totalNumPages) * one)
    {
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

    return RC_OK;
}




































