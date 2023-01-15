#include "global.h"

/**
 * @brief 
 * SYNTAX: EXPORT <relation_name> 
 */

bool syntacticParseEXPORT()
{
    if (tokenizedQuery.size() == 3 && tokenizedQuery[1] == "MATRIX") {
        logger.log("syntacticParseEXPORT_MATRIX");
        parsedQuery.queryType = EXPORT_MATRIX;
        parsedQuery.exportRelationName = tokenizedQuery[2];
    }
    else if (tokenizedQuery.size() == 2) {
        logger.log("syntacticParseEXPORT");
        parsedQuery.queryType = EXPORT;
        parsedQuery.exportRelationName = tokenizedQuery[1];
    } else {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }
    return true;
}

bool semanticParseEXPORT()
{
    if (parsedQuery.queryType == EXPORT_MATRIX) {
        logger.log("semanticParseEXPORT_MATRIX");
        if (!matrixCatalogue.isMatrix(parsedQuery.exportRelationName))
        {
            cout << "SEMANTIC ERROR: Matrix does not exist" << endl;
            return false;
        }
    } else {
        logger.log("semanticParseEXPORT");
        if (!tableCatalogue.isTable(parsedQuery.exportRelationName))
        {
            cout << "SEMANTIC ERROR: Relation does not exist" << endl;
            return false;
        }
    }
}

void executeEXPORT()
{
    if (parsedQuery.queryType == EXPORT_MATRIX) {
        logger.log("executeEXPORT_MATRIX");
        Matrix* matrix = matrixCatalogue.getMatrix(parsedQuery.exportRelationName);
        matrix->makePermanent();
    } else {
        logger.log("executeEXPORT");
        Table* table = tableCatalogue.getTable(parsedQuery.exportRelationName);
        table->makePermanent();
    }
    return;
}