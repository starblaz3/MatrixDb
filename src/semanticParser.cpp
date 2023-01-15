#include"global.h"

bool semanticParse(){
    logger.log("semanticParse");
    switch(parsedQuery.queryType){
        case CLEAR: return semanticParseCLEAR();
        case CROSS: return semanticParseCROSS();
        case DISTINCT: return semanticParseDISTINCT();
        case EXPORT: return semanticParseEXPORT();
        case EXPORT_MATRIX: return semanticParseEXPORT();
        case INDEX: return semanticParseINDEX();
        case JOIN: return semanticParseJOIN();
        case LIST: return semanticParseLIST();
        case LOAD: return semanticParseLOAD();
        case PRINT: return semanticParsePRINT();
        case LOAD_MATRIX: return semanticParseLOAD();
        case PRINT_MATRIX: return semanticParsePRINT();
        case PROJECTION: return semanticParsePROJECTION();
        case RENAME: return semanticParseRENAME();
        case SELECTION: return semanticParseSELECTION();
        case SORT: return semanticParseSORT();
        case SOURCE: return semanticParseSOURCE();
        case CROSS_TRANSPOSE: return semanticParseCROSS_TRANSPOSE();
        case LOAD_SPARSE_MATRIX: return semanticParseLOAD();
        case PRINT_SPARSE_MATRIX: return semanticParsePRINT();
        case TRANSPOSE_SPARSE_MATRIX: return semanticParseTRANSPOSE_SPARSE_MATRIX();
        default: cout<<"SEMANTIC ERROR"<<endl;
    }

    return false;
}