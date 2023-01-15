#include "global.h"
/**
 * @brief Construct a new MatrixPage object. Never used as part of the code
 *
 */
MatrixPage::MatrixPage()
{
    this->matrixName="";
    this->blockRowIdx=0;
    this->blockColIdx=0;
    this->columnCount=0;
    this->rowCount=0;
    this->matrix.clear();
    this->pageName = "";
}

// void print_block(vector<vector<int>> block, int blockr, int blockc) {
//     cout << "\nPrinting Block: " << blockr << " " << blockc << endl;
//     for (auto v : block) {
//         for (auto i : v) {
//             cout << i << " ";
//         }
//         cout << endl;
//     }
//     cout << endl;
// }

/**
 * @brief Construct a new MatrixPage:: MatrixPage object given the table name and page
 * index. When tables are loaded they are broken up into blocks of BLOCK_SIZE
 * and each block is stored in a different file named
 * "<tablename>_MatrixPage<pageindex>". For example, If the MatrixPage being loaded is of
 * table "R" and the pageIndex is 2 then the file name is "R_MatrixPage2". The page
 * loads the rows (or tuples) into a vector of rows (where each row is a vector
 * of integers).
 *
 * @param tableName 
 * @param pageIndex 
 */
MatrixPage::MatrixPage(string matrixName, int blockRowIdx, int blockColIdx)
{
    logger.log("MatrixPage::MatrixPage");

    this->blockRowIdx=blockRowIdx;
    this->blockColIdx=blockColIdx;
    this->matrixName = matrixName;
    this->pageName = "../data/temp/"+ matrixName + "_MatrixPage-" + to_string(blockRowIdx) + "-" + to_string(blockColIdx);

    string line, word;
    this->rowCount = 0;
    int columnCounter = 0;
    vector<int> row(MAT_PAGE_DIM,-1);
    this->matrix.assign(MAT_PAGE_DIM, row);

    ifstream fin(this->pageName, ios::in);
    while(getline(fin, line)) {
        stringstream s(line);
        while (getline(s, word, ','))
            this->matrix[this->rowCount][columnCounter++] = stoi(word);
        this->columnCount = columnCounter;
        columnCounter = 0;
        this->rowCount++;
    }

    this->matrix.resize(this->rowCount);
    for (auto iter = this->matrix.begin(); iter != this->matrix.end(); iter++) {
        iter->resize(this->columnCount);
    }
}

/**
 * @brief Get row from page indexed by rowIndex
 * 
 * @param rowIndex 
 * @return vector<int> 
 */
vector<int> MatrixPage::getRow(int rowIndex)
{
    logger.log("MatrixPage::getRow");
    vector<int> result;
    result.clear();
    if (rowIndex >= this->rowCount)
        return result;
    return this->matrix[rowIndex];
}
vector<vector<int>> MatrixPage::getMatrix()
{
    return this->matrix;
}
MatrixPage::MatrixPage(string matrixName, int blockRowIdx, int blockColIdx, const vector<vector<int>> matrix)
{
    logger.log("MatrixPage::MatrixPage");
    this->matrixName = matrixName;
    this->blockRowIdx = blockRowIdx;
    this->blockColIdx = blockColIdx;
    this->matrix = matrix;
    this->rowCount = matrix.size();
    this->columnCount = matrix[0].size();
    this->pageName = "../data/temp/"+ matrixName + "_MatrixPage-" + to_string(blockRowIdx) + "-" + to_string(blockColIdx);
}

/**
 * @brief writes current page contents to file.
 * 
 */
void MatrixPage::writePage()
{
    logger.log("MatrixPage::writeMatrixPage");
    ofstream fout(this->pageName, ios::trunc);
    for (int rowCounter = 0; rowCounter < this->rowCount; rowCounter++)
    {
        for (int columnCounter = 0; columnCounter < this->columnCount; columnCounter++)
        {
            if (columnCounter != 0)
                fout << ",";
            fout << this->matrix[rowCounter][columnCounter];
        }
        fout << endl;
    }
    fout.close();
}

void MatrixPage::transpose() {
    vector<vector<int>> transposedMatrix(this->columnCount, vector<int>(this->rowCount, 0));

    for (int r = 0; r < this->rowCount; r++) {
        for (int c = 0; c < this->columnCount; c++) {
            transposedMatrix[c][r] = this->matrix[r][c];
        }
    }

    this->matrix.clear();
    this->matrix = transposedMatrix;
    swap(this->rowCount, this->columnCount);
}
