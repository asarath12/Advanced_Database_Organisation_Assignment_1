//
//
//


// Include return codes and methods for logging errors
#include "dberror.h"
// Include bool DT
#include "dt.h"
//include header file
#include "buffer_mgr.h"


// Buffer Manager Interface Pool Handling
/*This function creates the new buffer pool.  Within this instance, NumPages are tracked
 *using the page replacement strategy.  This pool caches pages from pageFileName.
 *Initially, this produces an empty Buffer Pool.  This does NOT generate a new pageFileName.
 *stratData is used to pass info for page replacement in ReplacementStrategy.
 *
 */
RC initBufferPool(BM_BufferPool *const bm, const char *const pageFileName,
        const int numPages, ReplacementStrategy strategy,
        void *stratData) {

    return RC_OK;
}


/*This function destroys the Buffer Pool.
 *todo Determine if we are to unpin pages if this function is called, or just return an error.
 *If pinned pages exist, return an error /or/ Unpin pages if required.
 *All dirty pages are written to disk.
 *All resources are freed.
 *Return RC_OK when process finished.
 *
 */
RC shutdownBufferPool(BM_BufferPool *const bm) {
    //Check for pinned pages//todo determine directive if pinned pages found.
    //if (pinned_pages exist){return error};//todo add new error code
    //else{
    //call forceFlushPool to write all dirty pages to disk
    if (forceFlushPool(bm)!=RC_OK) { //todo check attributes of call
        return RC_WRITE_FAILED;//todo add new error code
    }else {
        //Free all resources

        //Return RC_OK when finished.
        return RC_OK;
    }
    //}
    //if something else happens, let's return an error:
    //return RC_WRITE_FAILED;
}
/*This function causes all dirty pages with a pin count of 0 to be
 *written to disk.
 *If pin count !=0, it does not change the dirty marker, nor write the page, even if it
 *is dirty.
 *Iterates through page table, writing dirty pages to disk.
 *Set dirty flag to 0.
 *Return RC_OK when process finished.
 *
 */
RC forceFlushPool(BM_BufferPool *const bm) {
// Iterate through BufferPool, if pinned = 0 and dirty !=0, write page to disk.
    //for the length of the BufferPool{
        //if (pinned==0 && dirty!=0){
        //call writeBlock
        //set dirty=0}}
    return RC_OK;
}

// Buffer Manager Interface Access Pages
/*This function will mark a single page as dirty.
 *Used when page is modified for any reason.
 */
RC markDirty (BM_BufferPool *const bm, BM_PageHandle *const page){

    return RC_OK;
}
/*This function is called when the client is finished using a page.
 *A client is finished using a page when all modifications are complete,
 *and the client can not access any editing of the page in memory.
 *NOTE: Because more than one client may have access to the page at a time,
 *the fix count could be greater than one.  Therefore, this function
 * only reduces the fix count by 1.
 */
RC unpinPage (BM_BufferPool *const bm, BM_PageHandle *const page){
//Verify that the fix count is greater than 0.
    //Subtract -1 from fix count
    return RC_OK;
}

/*This function calls writeBlock to write the given page to disk.
 *Sets dirty to 0
 *
 */
RC forcePage (BM_BufferPool *const bm, BM_PageHandle *const page){
    //call writeBlock to write page to disk.
    // set dirty to 0

    //On success, return
    return RC_OK;
}

/*This function pins the page of page number pageNum.
 *
 *
 */
RC pinPage (BM_BufferPool *const bm, BM_PageHandle *const page,
const PageNumber pageNum){

    return RC_OK;
}


// Statistics Interface
PageNumber *getFrameContents (BM_BufferPool *const bm){

    return RC_OK;
}

bool *getDirtyFlags (BM_BufferPool *const bm){

    return RC_OK;
}

int *getFixCounts (BM_BufferPool *const bm){

    return RC_OK;
}

int getNumReadIO (BM_BufferPool *const bm){

    return RC_OK;
}

int getNumWriteIO (BM_BufferPool *const bm){

    return RC_OK;
}

