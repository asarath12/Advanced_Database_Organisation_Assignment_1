//
// Created by Rebecca Sue Thomson on 1/28/25.
//
#include <stdint.h>
#include "dberror.h"
#include "rst_storage_mgr.h"

/************************************************************
 *                    handle data structures                *
 ************************************************************/
//removing data structures from c file to only be defined in h file
/*
typedef struct SM_FileHandle {
	char *fileName;
	int totalNumPages;
	int curPagePos;
	void *mgmtInfo; //create this function to store bookkeeping info.
} SM_FileHandle;

typedef char* SM_PageHandle;
*/
/************************************************************
 *                    interface                             *
 ************************************************************/
/* manipulating page files */
extern void initStorageManager (void);

extern RC createPageFile (char *fileName);
{
//creates a single new page file with fileName,
// fill single page with '\0' bytes, use new function fillZeros?
//fill fields of SM_FileHandle.
struct SM_fileHandle *fileName;
//strcpy(*fileName.fileName, *fileName); //todo check format
*fileName.totalNumPages = 0;//start from zero
*fileName.curPagePos = 0;


//todo ADD this return to ALL complete RC functions.
//if operation successful, return RC_OK
return RC_OK;
}

extern RC openPageFile (char *fileName, SM_FileHandle *fHandle);
{
    //open existing page file - try/except block, if it doesn't work "RC_FILE_NOT_FOUND"

    //if opening successfull, fill fields of SM_FileHandle
    struct SM_fileHandle *fileName;
    //strcpy(*fileName.fileName, *fileName); //todo check format
    //find size of file, divide by PAGE_SIZE
    //*fileName.totalNumPages = /PAGE_SIZE; //todo find current size
    *fileName.curPagePos = 0;
    //todo ADD this return to ALL complete RC functions.
    //if operation successful, return RC_OK
    return RC_OK;
}
extern RC closePageFile (SM_FileHandle *fHandle);
{
    //close the page file. //todo do we want error handling?
    //todo do we need to update SM_FileHandle here?

    //todo ADD this return to ALL complete RC functions.
    //if operation successful, return RC_OK
    return RC_OK;

}
extern RC destroyPageFile (char *fileName);
{//delete the page file.  //todo do we want error handling?  verification before delete?

    //todo ADD this return to ALL complete RC functions.
    //if operation successful, return RC_OK
    return RC_OK;

}
/* reading blocks from disc */
extern RC readBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage);
{//create error-check for if pages available<pageNum, if-then return error code in notes, otherwise:
//reads block at position pageNum from a file
//stores content in the memory pointed to by the memPage page handle
    //todo ADD this return to ALL complete RC functions.
    //if operation successful, return RC_OK
    return RC_OK;
    }


extern int getBlockPos (SM_FileHandle *fHandle);
{//Return the current page position in a file

  }
extern RC readFirstBlock (SM_FileHandle *fHandle, SM_PageHandle memPage);
{//read First Block in a file

    //todo ADD this return to ALL complete RC functions.
    //if operation successful, return RC_OK
    return RC_OK;
  }

extern RC readPreviousBlock (SM_FileHandle *fHandle, SM_PageHandle memPage);
{//read previous block in a file
//change curPagePos to the page read
    //todo ADD this return to ALL complete RC functions.
    //if operation successful, return RC_OK
    return RC_OK;
    }
extern RC readCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage);
{//read current block in a file
    //todo ADD this return to ALL complete RC functions.
    //if operation successful, return RC_OK
    return RC_OK;
    }
extern RC readNextBlock (SM_FileHandle *fHandle, SM_PageHandle memPage);
{//read next block in a file
//change curPagePos to the page read
    //todo ADD this return to ALL complete RC functions.
    //if operation successful, return RC_OK
    return RC_OK;
}
extern RC readLastBlock (SM_FileHandle *fHandle, SM_PageHandle memPage);
{//read First Block in a file

    //todo ADD this return to ALL complete RC functions.
    //if operation successful, return RC_OK
    return RC_OK;
}

/* writing blocks to a page file */
extern RC writeBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage);
{//write a page to disk of given position

  //todo ADD this return to ALL complete RC functions.
    //if operation successful, return RC_OK
    return RC_OK;
  }
extern RC writeCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage);
{//write a page to disk using the current position

    //todo ADD this return to ALL complete RC functions.
    //if operation successful, return RC_OK
    return RC_OK;}
extern RC appendEmptyBlock (SM_FileHandle *fHandle);
//Increase the number of pages in the file by one.  The new last page of the file shall have zero bytes.
{
  //update dberror.h(PAGE_SIZE)
  //update totalNumPages
  //Goto last page, create new page (with pointers?), fill with zeros.

}
extern RC ensureCapacity (int numberOfPages, SM_FileHandle *fHandle);
// if file  totalNumPages< number of Pages, then increase file size to number of pages
// will call function appendEmptyBlock
{
  while (totalNumPages<numberOfPages){appendEmptyBlock (SM_FileHandle *fHandle); }
//if operation successful, return RC_OK
  return RC_OK;
}