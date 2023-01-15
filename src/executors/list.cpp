#include "global.h"
/**
 * @brief 
 * SYNTAX: LIST TABLES
 */
bool syntacticParseLIST()
{
    logger.log("syntacticParseLIST");
    if (tokenizedQuery.size() != 2 || (tokenizedQuery[1] != "TABLES" && tokenizedQuery[1] != "MATRICES"))
    {
        cout << "SYNTAX ERROR" << endl;
        return false;
    }
    parsedQuery.queryType = LIST;
    parsedQuery.listEntityType = tokenizedQuery[1];
    return true;
}

bool semanticParseLIST()
{
    logger.log("semanticParseLIST");
    return true;
}

void executeLIST()
{
    logger.log("executeLIST");
    if(parsedQuery.listEntityType == "TABLES") tableCatalogue.print();
    if(parsedQuery.listEntityType == "MATRICES") matrixCatalogue.print();
}