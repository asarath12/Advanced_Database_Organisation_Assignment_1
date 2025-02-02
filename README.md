# Advanced_Database_Organisation_Assignment_1
Assignment 1 : Store Manager

Summary:  This is a simple storage manager.  It reads and writes single blocks of data from a file on disk into and out of memory.  Blocks are limited to PAGE_SIZE.  Besides reading and writing, the storage manager creates, opens, and closes files.  The manager tracks open file information: Total pages, current page, file name, and POSIX file descriptor (file pointer).

Functions available to user: 

*initialize*

initStorageManager(void):

/*page file manipulation*/

createPageFile (char *fileName):
openPageFile (char *fileName, SM_FileHandle *fHandle):
closePageFile (SM_FileHandle *fHandle): 
destroyPageFile (char *fileName):

/* reading blocks from disc */

readBlock (int pageNum , SM_FileHandle *fHandle , SM_PageHandle memPage):
int getBlockPos (SM_FileHandle *fHandle):
readFirstBlock (SM_FileHandle *fHandle, SM_PageHandle memPage):
readPreviousBlock (SM_FileHandle *fHandle, SM_PageHandle memPage):
readCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage):
readNextBlock (SM_FileHandle *fHandle, SM_PageHandle memPage):
readLastBlock (SM_FileHandle *fHandle, SM_PageHandle memPage):

/* writing blocks to a page file */

writeBlock (int pageNum , SM_FileHandle *fHandle , SM_PageHandle memPage): Write to file using given block position.  Single block only.

writeCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage): Ask writeBlock to write to block using current file position. Single block only.

appendEmptyBlock (SM_FileHandle *fHandle):Increase total Page number by one, fill new page with zero bytes.

ensureCapacity (int numberOfPages, SM_FileHandle *fHandle): If given numberOfPages is more than totalNumPages, increase totalNubPages with appendEmptyBlock until equal.

Basic code structure and architecture description:

The page files began at page 0.

It was the goal to reduce repeated actions in multiple functions.  As an example, writeCurrentBlock calls writeBlock using the current block location.  This way, only one function writes information to a block.

The writeCurrentBlock determines the current page location and asks writeBlock to write to the disk.  writeBlock error checks the file and the data to be written, and then writes it to disk.  appendEmptyBlock creates new empty last page in the file with zero bytes.  ensureCapacity checks that the existing totalNumPages is sufficient for a given numberOfPages.  It will use appendEmptyBlock if the size is insufficient.  


Project File List:
Please include all files for project performance.


Authors:
Rebecca Thomson
A20548618
rthomson@hawk.iit.edu
(rst)

Anish Roy
Aroy21@hawk.iit.edu


Anushka Sarath
asarath@hawk.iit.edu

