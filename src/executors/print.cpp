#include "global.h"
/**
 * @brief 
 * SYNTAX: PRINT relation_name
 */
bool syntacticParsePRINT()
{
    if (tokenizedQuery.size() == 3 && tokenizedQuery[1] == "MATRIX") {
        logger.log("syntacticParsePRINT_MATRIX");
        parsedQuery.queryType = PRINT_MATRIX;
        parsedQuery.printRelationName = tokenizedQuery[2];
    } else if (tokenizedQuery.size() == 3 && tokenizedQuery[1] == "SPARSE_MATRIX") {
        logger.log("syntacticParsePRINT_SPARSE_MATRIX");
        parsedQuery.queryType = PRINT_SPARSE_MATRIX;
        parsedQuery.printRelationName = tokenizedQuery[2];
    } else if (tokenizedQuery.size() == 2) {
        logger.log("syntacticParsePRINT");
        parsedQuery.queryType = PRINT;
        parsedQuery.printRelationName = tokenizedQuery[1];
    } else {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }
    return true;
}

bool semanticParsePRINT()
{
    if (parsedQuery.queryType == PRINT_MATRIX) {
        logger.log("semanticParsePRINT_MATRIX");
        if (!matrixCatalogue.isMatrix(parsedQuery.printRelationName))
        {
            cout << "SEMANTIC ERROR: Matrix doesn't exist" << endl;
            return false;
        }
    } else {
        if (parsedQuery.queryType == PRINT_SPARSE_MATRIX) logger.log("semanticParsePRINT_SPARSE_MATRIX");
        else logger.log("semanticParsePRINT");
        if (!tableCatalogue.isTable(parsedQuery.printRelationName))
        {
            cout << "SEMANTIC ERROR: Relation doesn't exist" << endl;
            return false;
        }
    }

    return true;
}

void executePRINT()
{
    if (parsedQuery.queryType == PRINT_MATRIX) {
        logger.log("executePRINT_MATRIX");
        Matrix *matrix = matrixCatalogue.getMatrix(parsedQuery.printRelationName);
        matrix->print();
    } else {
        Table *table = tableCatalogue.getTable(parsedQuery.printRelationName);
        if (parsedQuery.queryType == PRINT_SPARSE_MATRIX) {
            logger.log("executePRINT_SPARSE_MATRIX");
            table->sparseMatrixPrint();
        } else {
            logger.log("executePRINT");
            table->print();
        }
    }
    return;
}
