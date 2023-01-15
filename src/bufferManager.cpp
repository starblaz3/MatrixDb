#include "global.h"

BufferManager::BufferManager()
{
    logger.log("BufferManager::BufferManager");
}

/**
 * @brief Function called to read a page from the buffer manager. If the page is
 * not present in the pool, the page is read and then inserted into the pool.
 *
 * @param tableName 
 * @param pageIndex 
 * @return Page 
 */
Page BufferManager::getTablePage(string tableName, int pageIndex)
{
    logger.log("BufferManager::getTablePage");
    string pageName = "../data/temp/"+tableName + "_TablePage-" + to_string(pageIndex);
    if (this->inPool(pageName))
        return any_cast<Page>(this->getFromPool(pageName));
    else
        return this->insertTablePageIntoPool(tableName, pageIndex);
}

MatrixPage BufferManager::getMatrixPage(string matrixName, int blockRowIdx, int blockColIdx) {
    logger.log("BufferManager::getMatrixPage");

    string pageName = "../data/temp/"+ matrixName + "_MatrixPage-" + to_string(blockRowIdx) + "-" + to_string(blockColIdx);
    if (this->inPool(pageName)) {
        return any_cast<MatrixPage>(this->getFromPool(pageName));
    } else {
        return this->insertMatrixPageIntoPool(matrixName, blockRowIdx, blockColIdx);
    }
}

/**
 * @brief Checks to see if a page exists in the pool
 *
 * @param pageName 
 * @return true 
 * @return false 
 */
bool BufferManager::inPool(string pageName)
{
    logger.log("BufferManager::inPool");
    for (auto page : this->pages)
    {
        try {
            if (pageName == any_cast<Page>(page).pageName) return true;
        } catch (const std::bad_any_cast& e) {
            if (pageName == any_cast<MatrixPage>(page).pageName) return true;
        }
    }
    return false;
}

/**
 * @brief If the page is present in the pool, then this function returns the
 * page. Note that this function will fail if the page is not present in the
 * pool.
 *
 * @param pageName 
 * @return Page 
 */
any BufferManager::getFromPool(string pageName)
{
    logger.log("BufferManager::getFromPool");
    for (auto page : this->pages) {
        try {
            if (pageName == any_cast<Page>(page).pageName) return page;
        } catch (const std::bad_any_cast& e) {
            if (pageName == any_cast<MatrixPage>(page).pageName) return page;
        }
    }
}

/**
 * @brief Inserts page indicated by tableName and pageIndex into pool. If the
 * pool is full, the pool ejects the oldest inserted page from the pool and adds
 * the current page at the end. It naturally follows a queue data structure.
 *
 * @param tableName 
 * @param pageIndex 
 * @return Page 
 */
Page BufferManager::insertTablePageIntoPool(string tableName, int pageIndex)
{
    logger.log("BufferManager::insertTablePageIntoPool");
    Page page(tableName, pageIndex);
    if (this->pages.size() >= BLOCK_COUNT)
        pages.pop_front();
    pages.push_back(page);
    return any_cast<Page>(page);
}

MatrixPage BufferManager::insertMatrixPageIntoPool(string matrixName, int rowIndex, int colIndex) {
    logger.log("BufferManager::insertMatrixPageIntoPool");

    MatrixPage page(matrixName, rowIndex, colIndex);

    if (this->pages.size() >= BLOCK_COUNT)
        pages.pop_front();
    pages.push_back(page);
    return any_cast<MatrixPage>(page);
}

/**
 * @brief The buffer manager is also responsible for writing pages. This is
 * called when new tables are created using assignment statements.
 *
 * @param tableName 
 * @param pageIndex 
 * @param rows 
 * @param rowCount 
 */
void BufferManager::writeTablePage(string tableName, int pageIndex, vector<vector<int>> rows, int rowCount)
{
    logger.log("BufferManager::writeTablePage");
    Page page(tableName, pageIndex, rows, rowCount);
    page.writePage();
}
void BufferManager::writeMatrixPage(string matrixName, int blockRowIdx, int blockColIdx, vector<vector<int>> rows)
{
    logger.log("BufferManager::writeMatrixPage");
    MatrixPage page(matrixName, blockRowIdx, blockColIdx, rows);
    page.writePage();
}
/**
 * @brief Deletes file names fileName
 *
 * @param fileName 
 */
void BufferManager::deleteFile(string fileName)
{
    if (remove(fileName.c_str()))
        logger.log("BufferManager::deleteFile: Err");
        else logger.log("BufferManager::deleteFile: Success");
}

/**
 * @brief Overloaded function that calls deleteFile(fileName) by constructing
 * the fileName from the tableName and pageIndex.
 *
 * @param tableName 
 * @param pageIndex 
 */
void BufferManager::deleteTableFile(string tableName, int pageIndex)
{
    logger.log("BufferManager::deleteFile");    
    string fileName = "../data/temp/"+tableName + "_TablePage-" + to_string(pageIndex);
    this->deleteFile(fileName);
}

void BufferManager::deleteMatrixFile(string matrixName, int blockRowIdx, int blockColIdx)
{
    logger.log("BufferManager::deleteMatrixFile");
    string fileName = "../data/temp/"+ matrixName + "_MatrixPage-" + to_string(blockRowIdx) + "-" + to_string(blockColIdx);
    this->deleteFile(fileName);
}