#ifndef BUFFERMANAGER
#define BUFFERMANAGER
#include"page.h"

/**
 * @brief The BufferManager is responsible for reading pages to the main memory.
 * Recall that large files are broken and stored as blocks in the hard disk. The
 * minimum amount of memory that can be read from the disk is a block whose size
 * is indicated by BLOCK_SIZE. within this system we simulate blocks by
 * splitting and storing the file as multiple files each of one BLOCK_SIZE,
 * although this isn't traditionally how it's done. You can alternatively just
 * random access to the point where a block begins within the same
 * file. In this system we assume that the the sizes of blocks and pages are the
 * same. 
 * 
 * <p>
 * The buffer can hold multiple pages quantified by BLOCK_COUNT. The
 * buffer manager follows the FIFO replacement policy i.e. the first block to be
 * read in is replaced by the new incoming block. This replacement policy should
 * be transparent to the executors i.e. the executor should not know if a block
 * was previously present in the buffer or was read in from the disk. 
 * </p>
 *
 */
class BufferManager{
    // any is a std class that allows to store different classes as a vector, needed to maintain pool of matrixpage and tablepage type
    deque<any> pages;
    bool inPool(string pageName);
    any getFromPool(string pageName);  

    Page insertTablePageIntoPool(string tableName, int pageIndex);
    MatrixPage insertMatrixPageIntoPool(string matrixName, int blockRowIdx, int blockColIdx);

    public:
    
    BufferManager();
    // void writePage(string pageName, vector<vector<int>> rows);
    void deleteFile(string fileName);

    Page getTablePage(string tableName, int pageIndex);
    void deleteTableFile(string tableName, int pageIndex);
    void writeTablePage(string tableName, int pageIndex, vector<vector<int>> rows, int rowCount);

    MatrixPage getMatrixPage(string matrixName, int blockRowIdx, int blockColIdx);
    void deleteMatrixFile(string matrixName, int blockRowIdx, int blockColIdx);
    void writeMatrixPage(string matrixName, int blockRowIdx, int blockColIdx, vector<vector<int>> rows);
};
#endif