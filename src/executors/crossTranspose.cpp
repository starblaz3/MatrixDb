#include "global.h"
/**
 * @brief
 * SYNTAX: PRINT relation_name
 */
bool syntacticParseCROSS_TRANSPOSE()
{
    if (tokenizedQuery.size() == 3)
    {
        logger.log("syntacticParseCROSS_TRANSPOSE");
        parsedQuery.queryType = CROSS_TRANSPOSE;
        parsedQuery.crossTransposeRelationNameA = tokenizedQuery[1];
        parsedQuery.crossTransposeRelationNameB = tokenizedQuery[2];
    }
    else
    {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }
    return true;
}

bool semanticParseCROSS_TRANSPOSE()
{
    logger.log("semanticParseCROSS_TRANSPOSE");
    if (!matrixCatalogue.isMatrix(parsedQuery.crossTransposeRelationNameA))
    {
        cout << "SEMANTIC ERROR: Relation doesn't exist - " << parsedQuery.crossTransposeRelationNameA << endl;
        return false;
    }
    if (!matrixCatalogue.isMatrix(parsedQuery.crossTransposeRelationNameB))
    {
        cout << "SEMANTIC ERROR: Relation doesn't exist - " << parsedQuery.crossTransposeRelationNameB << endl;
        return false;
    }
    return true;
}

void executeCROSS_TRANSPOSE()
{
    logger.log("executeCROSS_TRANSPOSE");
    Matrix *matrixA = matrixCatalogue.getMatrix(parsedQuery.crossTransposeRelationNameA);
    Matrix *matrixB = matrixCatalogue.getMatrix(parsedQuery.crossTransposeRelationNameB);

    bool isSelfTranspose = (parsedQuery.crossTransposeRelationNameA.compare(parsedQuery.crossTransposeRelationNameB) == 0);

    if (matrixA->rowBlockCount != matrixB->rowBlockCount || matrixA->colBlockCount != matrixB->colBlockCount || matrixA->colBlockCount!=matrixA->rowBlockCount || matrixB->colBlockCount!=matrixB->rowBlockCount)
        return;

    MatrixPage *pageA, *pageB, *swapPage;

    for (int rowBlockIdx = 0; rowBlockIdx < matrixA->rowBlockCount; rowBlockIdx++)
    {
        for (int colBlockIdx = (isSelfTranspose ? rowBlockIdx + 1 : 0); colBlockIdx < matrixA->colBlockCount; colBlockIdx++)
        {
            if (colBlockIdx == rowBlockIdx) continue;

            // Load page A and transpose
            pageA = new MatrixPage(matrixA->matrixName, rowBlockIdx, colBlockIdx);
            pageA->transpose();

            // Push page A transposed into swapPage with altered path and delete page A
            swapPage = new MatrixPage(matrixB->matrixName, colBlockIdx, rowBlockIdx, pageA->getMatrix());
            delete pageA;

            // Load page B and transpose
            pageB = new MatrixPage(matrixB->matrixName, colBlockIdx, rowBlockIdx);
            pageB->transpose();

            // Write the swapPage that contains transposed page A to page B's location
            swapPage->writePage();

            // Push page B transposed into swapPage with altered path and write it
            swapPage = new MatrixPage(matrixA->matrixName, rowBlockIdx, colBlockIdx, pageB->getMatrix());
            swapPage->writePage();
        }
    }
    
    for (int blockIdx = 0; blockIdx < matrixA->rowBlockCount; blockIdx++) {
        // Load page A and transpose
        pageA = new MatrixPage(matrixA->matrixName, blockIdx, blockIdx);
        pageA->transpose();

        // Push page A transposed into swapPage with altered path and delete page A
        swapPage = new MatrixPage(matrixB->matrixName, blockIdx, blockIdx, pageA->getMatrix());
        delete pageA;

        // Load page B and transpose
        pageB = new MatrixPage(matrixB->matrixName, blockIdx, blockIdx);
        pageB->transpose();

        // Write the swapPage that contains transposed page A to page B's location
        swapPage->writePage();

        // Push page B transposed into swapPage with altered path and write it
        swapPage = new MatrixPage(matrixA->matrixName, blockIdx, blockIdx, pageB->getMatrix());
        swapPage->writePage();
    }

    return;
}
