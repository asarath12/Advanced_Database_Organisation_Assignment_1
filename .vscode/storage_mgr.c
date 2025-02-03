#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include "storage_mgr.h"
#include "dberror.h"

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