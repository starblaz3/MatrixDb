#include "global.h"
/**
 * @brief
 * SYNTAX: PRINT relation_name
 */
bool syntacticParseTRANSPOSE_SPARSE_MATRIX()
{
    if (tokenizedQuery.size() == 2)
    {
        logger.log("syntacticParseTRANSPOSE_SPARSE_MATRIX");
        parsedQuery.queryType = TRANSPOSE_SPARSE_MATRIX;
        parsedQuery.transposeSparseMatrixName = tokenizedQuery[1];
    }
    else
    {
        cout << "SYNTAX ERROR trans" << endl;
        return false;
    }
    return true;
}

bool semanticParseTRANSPOSE_SPARSE_MATRIX()
{
    logger.log("semanticParseTRANSPOSE_SPARSE_MATRIX");
    if (!tableCatalogue.isTable(parsedQuery.transposeSparseMatrixName))
    {
        cout << "SEMANTIC ERROR: Sparse Matrix doesn't exist - " << parsedQuery.transposeSparseMatrixName << endl;
        return false;
    }
    return true;
}

void executeTRANSPOSE_SPARSE_MATRIX()
{
    logger.log("executeTRANSPOSE_SPARSE_MATRIX");
    Table *table = tableCatalogue.getTable(parsedQuery.transposeSparseMatrixName);

    table->renameColumn("r", "t");
    table->renameColumn("c", "r");
    table->renameColumn("t", "c");

    return;
}
