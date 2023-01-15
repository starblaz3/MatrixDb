#include "global.h"

/**
 * @brief Construct a new Matrix:: Matrix object
 *
 */
Matrix::Matrix()
{
    logger.log("Matrix::Matrix");
}

/**
 * @brief Construct a new Matrix:: Matrix object used in the case where the data
 * PAGE is available and LOAD command has been called. This command should be
 * followed by calling the load function;
 *
 * @param matrixName 
 */
Matrix::Matrix(string matrixName)
{
    logger.log("Matrix::Matrix");
    this->sourceFileName = "../data/" + matrixName + ".csv";
    this->matrixName = matrixName;
}

/**
 * @brief The load function is used when the LOAD MATRIX command is encountered. It
 * reads data from the source file, splits it into blocks following submatrix page layout.
 *
 * @return true if the matrix has been successfully loaded 
 * @return false if an error occurred 
 */
bool Matrix::load()
{
    logger.log("Matrix::load");
    fstream fin(this->sourceFileName, ios::in);
    string line;
    if (getline(fin, line))
    {
        fin.close();
        this->extractCounts(line);
        // cout << this->columnCount << " " << this->rowCount << endl;
        if (this->blockify())
            return true;
    }
    fin.close();
    return false;
}

/**
 * @brief Function extracts column counts from the first line of the .csv data
 * file. 
 *
 * @param line   
 */
void Matrix::extractCounts(string firstLine)
{
    logger.log("Matrix::extractCounts");
    string word;
    stringstream s(firstLine);
    while (getline(s, word, ',')) this->columnCount += 1;
    this->rowCount = this->columnCount;
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
 * @brief This function splits all the rows and columns of the matrix 
 * and stores them in multiple files across blocks using submatrix page layout
 * by first preprocessing offsets for each block in the input file and filling block by block row wise
 * and writing each filled block to the corresponding page.
 *
 * @return true if successfully blockified
 * @return false otherwise
 */
bool Matrix::blockify()
{
    ifstream fin(this->sourceFileName, ios::in);

    vector<uint> offsets;
    vector<int> columnsProcessed(this->rowCount, 0);
    string line, word;

    offsets.push_back(fin.tellg());
    while (getline(fin, line)) offsets.push_back(fin.tellg());
    offsets.pop_back();

    int colidx = 0;
    int rowidx = 0;
    int colblockidx = 0;

    for (int blockr = 0; blockr < offsets.size(); blockr += MAT_PAGE_DIM) {
        colblockidx = 0;
        while (columnsProcessed[blockr] != this->columnCount) {
            vector<int> tempRow(MAT_PAGE_DIM,0);
            vector<vector<int>> block(MAT_PAGE_DIM, tempRow);
            colidx = 0;

            for (rowidx = 0; rowidx < MAT_PAGE_DIM && blockr+rowidx < offsets.size(); rowidx++) {
                fin.clear();
                fin.seekg(offsets[rowidx + blockr], ios::beg);
                getline(fin, line);
                stringstream line_stream(line);

                while (getline(line_stream, word, ',')) {
                    block[rowidx][columnsProcessed[rowidx + blockr] % MAT_PAGE_DIM] = stoi(word);
                    columnsProcessed[rowidx + blockr]++;
                    if (columnsProcessed[rowidx + blockr] % MAT_PAGE_DIM == 0 || columnsProcessed[rowidx + blockr] == this->columnCount) break;
                    if (rowidx == 0) colidx++;
                }
                offsets[rowidx + blockr] += line_stream.tellg();
            }

                block.resize(rowidx);
            for (auto iter = block.begin(); iter != block.end(); iter++) {
                iter->resize(colidx + 1);
            }

            bufferManager.writeMatrixPage(this->matrixName, blockr / MAT_PAGE_DIM, colblockidx, block);
            colblockidx++;
            if (blockr == 0) this->colBlockCount++;
        }
        this->rowBlockCount++;
    }

    for (auto count : columnsProcessed) if (count != this->columnCount) return false;
    return true;
}

/**
 * @brief Function prints the first few rows of the matrix. If the matrix contains
 * more rows than PRINT_COUNT, exactly PRINT_COUNT rows are printed, else all
 * the rows are printed.
 *
 */
void Matrix::print()
{
    logger.log("Matrix::print");
    int count = min(PRINT_COUNT, this->rowCount);

    MatrixPage activePage;
    vector<int> activeRow;

    for (int browidx = 0; browidx < count; browidx += MAT_PAGE_DIM) {
        for (int ridx = 0; ridx < MAT_PAGE_DIM && ridx + browidx < count; ridx++) {
            for (int bcolidx = 0; bcolidx < count; bcolidx += MAT_PAGE_DIM) {
                activePage = bufferManager.getMatrixPage(this->matrixName, browidx / MAT_PAGE_DIM, bcolidx / MAT_PAGE_DIM);

                activeRow = activePage.getRow(ridx);
                // cout << browidx / MAT_PAGE_DIM << " " << bcolidx / MAT_PAGE_DIM << " " << ridx << endl;
                bool last_col_block = count - bcolidx <= MAT_PAGE_DIM;
                for (int cidx = 0; cidx < activeRow.size() - last_col_block; cidx++) cout << activeRow[cidx] << ", ";
                if (last_col_block) cout << activeRow.back();
            }
            cout << endl;
        }
    }
}


/**
 * @brief called when EXPORT command is invoked to move source file to "data"
 * folder.
 *
 */
void Matrix::makePermanent()
{
    logger.log("Matrix::makePermanent");
    if(!this->isPermanent())
        bufferManager.deleteFile(this->sourceFileName);
    string newSourceFile = "../data/" + this->matrixName + ".csv";
    ofstream fout(newSourceFile, ios::out);

    MatrixPage activePage;
    vector<int> activeRow;

    for (int browidx = 0; browidx < this->rowCount; browidx += MAT_PAGE_DIM) {
        for (int ridx = 0; ridx < MAT_PAGE_DIM && ridx + browidx < this->rowCount; ridx++) {
            for (int bcolidx = 0; bcolidx < this->columnCount; bcolidx += MAT_PAGE_DIM) {
                activePage = bufferManager.getMatrixPage(this->matrixName, browidx / MAT_PAGE_DIM, bcolidx / MAT_PAGE_DIM);

                activeRow = activePage.getRow(ridx);
                bool last_col_block = this->columnCount - bcolidx <= MAT_PAGE_DIM;
                for (int cidx = 0; cidx < activeRow.size() - last_col_block; cidx++) fout << activeRow[cidx] << ",";
                if (last_col_block) fout << activeRow.back();
            }
            fout << endl;
        }
    }

    fout.close();
}

/*
 * @brief Function to check if matrix is already exported
 *
 * @return true if exported
 * @return false otherwise
 */
bool Matrix::isPermanent()
{
    logger.log("Matrix::isPermanent");
    if (this->sourceFileName == "../data/" + this->matrixName + ".csv")
        return true;
    return false;
}

/**
 * @brief The unload function removes the matrix from the database by deleting
 * all temporary files created as part of this matrix
 *
 */
void Matrix::unload(){
    logger.log("Matrix::~unload");
        
    for (int rowCounter = 0; rowCounter < this->rowBlockCount; rowCounter++)
        for (int colCounter = 0; colCounter < this->colBlockCount; colCounter++) 
            bufferManager.deleteMatrixFile(this->matrixName, rowCounter, colCounter);
    if (!isPermanent())
        bufferManager.deleteFile(this->sourceFileName);
}
