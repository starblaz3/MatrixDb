#include "global.h"
/**
 * @brief 
 * SYNTAX: LOAD relation_name
 */
bool syntacticParseLOAD()
{
    if (tokenizedQuery.size() == 3 && tokenizedQuery[1] == "MATRIX") {
        logger.log("syntacticParseLOAD_MATRIX");
        parsedQuery.queryType = LOAD_MATRIX;
        parsedQuery.loadRelationName = tokenizedQuery[2];
    } else if (tokenizedQuery.size() == 3 && tokenizedQuery[1] == "SPARSE_MATRIX") {
        logger.log("syntacticParseLOAD_SPARSE_MATRIX");
        parsedQuery.queryType = LOAD_SPARSE_MATRIX;
        parsedQuery.loadRelationName = tokenizedQuery[2];
    } else if (tokenizedQuery.size() == 2) {
        logger.log("syntacticParseLOAD");
        parsedQuery.queryType = LOAD;
        parsedQuery.loadRelationName = tokenizedQuery[1];
    } else {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }
    return true;
}

bool semanticParseLOAD()
{
    if (parsedQuery.queryType == LOAD_MATRIX) {
        logger.log("semanticParseLOAD_MATRIX");
        if (matrixCatalogue.isMatrix(parsedQuery.loadRelationName))
        {
            cout << "SEMANTIC ERROR: Matrix already exists" << endl;
            return false;
        }
    } else {
        if (parsedQuery.queryType == LOAD_SPARSE_MATRIX) logger.log("semanticParseLOAD_SPARSE_MATRIX");
        else logger.log("semanticParseLOAD");
        if (tableCatalogue.isTable(parsedQuery.loadRelationName))
        {
            cout << "SEMANTIC ERROR: Relation already exists" << endl;
            return false;
        }
    }
    
    if (!isFileExists(parsedQuery.loadRelationName))
    {
        cout << "SEMANTIC ERROR: Data file doesn't exist" << endl;
        return false;
    }
    return true;
}

void executeLOAD()
{
    if (parsedQuery.queryType == LOAD_MATRIX) {
        logger.log("executeLOAD_MATRIX");
        Matrix *matrix = new Matrix(parsedQuery.loadRelationName);
        if (matrix->load()) {
            matrixCatalogue.insertMatrix(matrix);
            cout << "Loaded Matrix. Matrix Dimensions: (" << matrix->columnCount << " , " << matrix->rowCount << ")\n";
        }
    } else {
        if (parsedQuery.queryType == LOAD_SPARSE_MATRIX) logger.log("executeLOAD_SPARSE_MATRIX");
        else logger.log("executeLOAD");
        Table *table = new Table(parsedQuery.loadRelationName);
        if (table->load())
        {
            tableCatalogue.insertTable(table);
            cout << "Loaded Table. Column Count: " << table->columnCount << " Row Count: " << table->rowCount << endl;
        }
    }
    return;
}