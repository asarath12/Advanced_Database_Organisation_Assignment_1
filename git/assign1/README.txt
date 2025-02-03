Team Members:
1. Anish Roy	    aroy21@hawk.iit.edu
2. Anushka Sarath   asarath@hawk.iit.edu
3. Rebecca Thomson	rthomson@hawk.iit.edu (rst)


Team Contribution:
Anish Roy:
    extern void initStorageManager();
    extern RC createPageFile(char *fileName);
    extern RC openPageFile(char *fileName, SM_FileHandle *fHandle);
    extern RC closePageFile(SM_FileHandle *fHandle);
    extern RC destroyPageFile(char *fileName);
	MakeFile creation,Merging files, running tests

Anushka Sarath:
    extern RC readBlock(int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage);
    extern int getBlockPos ( SM_FileHandle * fHandle );
    extern RC readFirstBlock ( SM_FileHandle * fHandle , SM_PageHandle memPage );
    extern RC readPreviousBlock ( SM_FileHandle * fHandle , SM_PageHandle memPage );
    extern RC readCurrentBlock ( SM_FileHandle * fHandle , SM_PageHandle memPage );
    extern RC readNextBlock ( SM_FileHandle * fHandle , SM_PageHandle memPage );
    extern RC readLastBlock ( SM_FileHandle * fHandle , SM_PageHandle memPage );

Rebecca Thomson:
extern RC writeBlock (int pageNum , SM_FileHandle * fHandle , SM_PageHandle memPage );
extern RC writeCurrentBlock ( SM_FileHandle * fHandle , SM_PageHandle memPage );
extern RC appendEmptyBlock ( SM_FileHandle * fHandle );
extern RC ensureCapacity (int numberOfPages , SM_FileHandle * fHandle );




Folder structure
The assign1_storage_manager folder contains the following files:
- dberror.h: This file contains the error constants defined with RC error codes. 2 macros THROW and CHECK are defined for throwing and checking error.
- dberror.c: This file gives the method definition of the printError and errorMessage methods declared in dberror.h.
- storage_mgr.h: This file defines a structure for SM_FileHandle with its fields and also defines SM_PageHandle. This file declares methods for file manipulation, read and writing files.
- storage_mgr.c: This file provides method definition to all the methods declared in initStorageManager.h for file manipulation, reading and writing in files.
- test_helper.h: This files defines set of macros used for testing purpose.
- test_assign1_1.c: This file has 2 methods testCreateOpenClose and testSinglePageContent which is used for testing the functionality of the program.
- Makefile: This file is used to create binary test_assign1 from test_assign1_1.c and it is executed when make run command is executed. 
            The file also declares the output fileName after compiling and executing the above c files, which are db_error.o, storage_mgr.o and test_assign1_1.o.


Functions:

extern void initStorageManager (void)
This method initializes the storage manager program.

extern RC createPageFile (char *fileName)
This method is used to create a new page file using the fileName specified as parameter.

extern RC openPageFile (char *fileName, SM_FileHandle *fHandle)
This method opens the existing page file, if the file not found returns RC_FILE_NOT_FOUND error. After opening the file, 
information about the file are initialized in fHandle fields. 

extern RC closePageFile (SM_FileHandle *fHandle)
This method is used to close the file, return error if there is any error in closing the file.

extern RC destroyPageFile (char *fileName)
This method is used to destroy or delete the file using remove method and if this method returns -1 ,
return error in destroying the file.

extern RC readBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage)
This method is used to read the block for the specified page number. Return RC_READ_NON_EXISTING_PAGE error 
if the pageNum given is less than 0 or greater than the total number of pages. This method calclates the offset
to access the required pageNum, seek to the required position and tries to read the contents and stores it 
in memPage. If there is error in seeking the required position or in reading, returns approriate error.

extern int getBlockPos (SM_FileHandle *fHandle)
This method returns the current position of the page in file handle.

extern RC readFirstBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
This method reads the contents of the first page and stores it in memPage.

extern RC readPreviousBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
This method reads the previous page relative to the current page of the file and stores it in memPage.

extern RC readCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
This methods reads the contents of the current page and stores it in memPage.

extern RC readNextBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
This method reads the next page relative to the current page of the file and stores it in memPage.

extern RC readLastBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
This method reads the contents of the last page and stores it in memPage.

extern RC writeBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage)
This method writes the contents from memPage to the requested pageNum. 
Return RC_WRITE_FAILED error if the pageNum given is less than 0 or greater than the total number of pages. 
This method calclates the offset to access the required pageNum, seek to the required position 
and tries to write the contents from the in memPage to the requested pageNum. 
If there is error in seeking the required position or in writing, returns approriate error.

extern RC writeCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
This method is used to write the contents of the memPage and store it in the current page. Returns approriate errors if 
there is any problem in seeking the required position or in writing the contents.

extern RC appendEmptyBlock (SM_FileHandle *fHandle)
This method is defined to increase the number of pages. Here, the file is seeked to the end and 
an empty block is allocated and written, also incrementing the number of pages by 1.

extern RC ensureCapacity (int numberOfPages, SM_FileHandle *fHandle)
This method makes sure if the number of pages in the file is less than the requested number of pages. 
If so, appends possible number of extra pages towards end of the file.



To run the file, navigate to the directory where the code files are present and then run the following commands:
make
make run

The output of running the file is shown in outputFile1.txt.

