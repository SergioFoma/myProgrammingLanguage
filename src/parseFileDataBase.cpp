#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>

#include "parseFileDataBase.h"
#include "treeDump.h"
#include "myStringFunction.h"
#include "paint.h"
#include "mathOperatorsInfo.h"
#include "mathematicalCalculator.h"

const size_t startSizeForWord = 7;
const size_t startSizeForTokens = 20;

expertSystemErrors writeInformationInFile( tree_t* tree ){
    if( tree == NULL ){
        return TREE_NULL_PTR;
    }

    colorPrintf( NOMODE, YELLOW, "\nEnter the name of file were do you want save data base: " );

    char* fileNameForWrite = NULL;
    size_t sizeOfAllocationMemory = 0;
    ssize_t sizeOfLine = getlineWrapper( &fileNameForWrite, &sizeOfAllocationMemory, stdin );
    if( sizeOfLine == -1 ){
        return ERROR_WITH_GETLINE;
    }

    FILE* fileForTree = fopen( fileNameForWrite, "w" );
    if( fileForTree == NULL ){
        colorPrintf( NOMODE, RED, "\nError with write data base in file\n" );
        return ERROR_WITH_FILE;
    }

    dumpTreeInFile( tree, fileForTree );

    free( fileNameForWrite );

    fclose( fileForTree );
    return CORRECT_WORK;
}

errorCode initBufferInformation( bufferInformation *bufferFromFile, FILE* myFile, const char* nameOfFile ){
    assert( bufferFromFile );
    assert( nameOfFile );

    if( myFile == NULL ){
        return fileOpenErr;
    }

    getFileSize( bufferFromFile, nameOfFile );
    bufferFromFile->buffer = (char*)calloc( bufferFromFile->fileSize + 1, sizeof( char ) );
    if( bufferFromFile->buffer == NULL ){
        return memoryErr;
    }
    bufferFromFile->bufferSize = fread( bufferFromFile->buffer, sizeof( char ), bufferFromFile->fileSize, myFile );
    if( bufferFromFile->bufferSize == 0 ){
        return fileOpenErr;
    }

    (bufferFromFile->buffer)[ bufferFromFile->fileSize ]= '\0';
    (bufferFromFile->buffer)[ bufferFromFile->bufferSize ] = '\0';

    return correct;
}

void getFileSize( bufferInformation* bufferFromFile, const char* nameOfFile ){
    assert( bufferFromFile != NULL );
    assert( nameOfFile != NULL );

    struct stat fileText;
    int status = stat( nameOfFile, &fileText );
    if( status == -1 ){
        colorPrintf( NOMODE, RED, "\nError of get information from file\n" );
        return ;
    }

    bufferFromFile->fileSize = fileText.st_size;
}

void destroyBufferInformation( bufferInformation *bufferFromFile ){
    if( bufferFromFile == NULL ){
        return ;
    }

    free( bufferFromFile->buffer );
    bufferFromFile->bufferSize = 0;
    bufferFromFile->fileSize = 0;
    bufferFromFile = NULL;
}

bufferInformation getBufferFromFile( FILE** fileWithBuffer ){

    bufferInformation dataBaseFromFile = {};
    char* nameOfFile = NULL;
    size_t sizeOfAllocationMemory = 0;
    ssize_t sizeOfLine = getlineWrapper( &nameOfFile, &sizeOfAllocationMemory, stdin );

    if( sizeOfLine == -1 ){
        free( nameOfFile );
        return dataBaseFromFile;
    }

    FILE* fileForReading = fopen( nameOfFile, "r" );
    if( fileForReading == NULL ){
        colorPrintf( NOMODE, RED, "\ncan not open file:%s %s %d\n", __FILE__, __func__, __LINE__ );
        free( nameOfFile );
        return dataBaseFromFile;
    }

    errorCode statusOfReadFromFile = initBufferInformation( &dataBaseFromFile, fileForReading, nameOfFile );
    if( statusOfReadFromFile != correct ){
        free( nameOfFile );
        return dataBaseFromFile;
    }

    *fileWithBuffer = fileForReading;
    free( nameOfFile );
    return dataBaseFromFile;
}

node_t** initializationTokens( size_t* countOfNodes ){
    *countOfNodes = startSizeForTokens;
    node_t** tokens = ( node_t** )calloc( *countOfNodes, sizeof( node_t* ) );
    size_t startIndex = 0;
    initializationNodeInArray( tokens, startIndex, countOfNodes );

    return tokens;
}

void initializationNodeInArray( node_t** tokens, size_t startIndex, size_t* countOfTokens ){
    assert( tokens );
    assert( countOfTokens );

    treeElem_t data = {};
    data.statement = NO_TYPE;
    for( ; startIndex < (*countOfTokens); startIndex++ ){
        initNode( tokens + startIndex, STATEMENT, data );
    }
}

node_t** lexAnalysis( char** symbol, size_t* countOfTokens ){
    assert( symbol );
    assert( *symbol );
    assert( countOfTokens );

    size_t tokensIndex = 0;
    node_t** tokens = initializationTokens( countOfTokens );

    while( **symbol != '\0' ){
        if( tokensIndex == ( *countOfTokens - 1) ){
            *countOfTokens *= 2;
            tokens = ( node_t** )realloc( tokens, sizeof( node_t* ) * (*countOfTokens) );
            initializationNodeInArray( tokens, tokensIndex + 1, countOfTokens );
        }

        bool isSearchStatement = false;
        for( size_t statementIndex = 0; statementIndex < sizeOfStatementArray; statementIndex++ ){
            const char* nameOfStatement = arrayWithStatements[ statementIndex ].viewOfStatementInFile;
            size_t lenOfStatement = strlen( arrayWithStatements[ statementIndex ].viewOfStatementInFile );

            if( strncmp( *symbol, nameOfStatement, lenOfStatement ) == 0 ){

                tokens[ tokensIndex ]->nodeValueType = STATEMENT;
                tokens[ tokensIndex ]->data.statement = arrayWithStatements[ statementIndex ].statement;
                (*symbol) += lenOfStatement;
                ++tokensIndex;
                isSearchStatement = true;
                break;
            }
        }
        if( isSearchStatement ){
            continue;
        }

        bool isSearchMath = false;
        for( size_t mathIndex = 0; mathIndex < sizeOfStatementArray; mathIndex++ ){
            const char* nameOfMath = arrayWithMathInfo[ mathIndex ].viewOfMathOperationInFile;
            size_t lenOfMath = strlen( arrayWithMathInfo[ mathIndex ].viewOfMathOperationInFile );

            if( strncmp( *symbol, nameOfMath, lenOfMath ) == 0 ){

                tokens[ tokensIndex ]->nodeValueType = OPERATOR;
                tokens[ tokensIndex ]->data.mathOperation = arrayWithMathInfo[ mathIndex ].mathOperation;
                (*symbol) += lenOfMath;
                ++tokensIndex;
                isSearchMath = true;
                break;
            }
        }
        if( isSearchMath ){
            continue;
        }


        if( '0' <= **symbol && **symbol <= '9' ){
            double value = 0;
            do{
                value = value * 10 + ( **symbol - '0' );
                ++(*symbol);
            }while( '0' <= **symbol && **symbol <= '9' );

            tokens[ tokensIndex ]->nodeValueType = NUMBER;
            tokens[ tokensIndex ]->data.number = value;
            ++tokensIndex;
            continue;
        }


        if( islower( **symbol ) || **symbol == '_' ){
            char* lineWithVar = NULL;
            size_t lineLen = readingVariable( &lineWithVar, symbol );
            bool statusOfSearching = changTypeOfNodeOnVariableNode( tokens, symbol, lineWithVar, tokensIndex, lineLen );
            if( !statusOfSearching ){
                free( tokens[ tokensIndex ] );
                tokens[ tokensIndex ] = makeNodeWithNewVariable( lineWithVar, symbol, lineLen, infoForVarArray.freeIndexNow );
            }
            ++tokensIndex;
            continue;
        }

        if( isspace( **symbol ) ){
            cleanLineWithCode( symbol );
            continue;
        }

        colorPrintf( NOMODE, RED, "\n\nERROR OF LEX ANALYSIS\n\n" );
        exit( 0 );
    }

    return tokens;
}

bool changTypeOfNodeOnVariableNode( node_t** tokens, char** ptrOnSymbolInPosition, char* lineWithVar, size_t tokensIndex, size_t lineLen ){
    assert( tokens );
    assert( lineWithVar );

    size_t varIndex = 0;
    for( varIndex = 0 ;varIndex < infoForVarArray.freeIndexNow; varIndex++ ){
        if( arrayWithVariables[ varIndex ].nameOfVariable &&
            strcmp( lineWithVar, arrayWithVariables[ varIndex ].nameOfVariable ) == 0 ){
            treeElem_t data = {};
            data.variableIndexInArray = arrayWithVariables[ varIndex ].variableIndexInArray;
            tokens[ tokensIndex ]->nodeValueType = VARIABLE;
            tokens[ tokensIndex ]->data = data;
            *ptrOnSymbolInPosition += lineLen;
            free( lineWithVar );
            return true;
        }
    }
    return false;
}

expertSystemErrors createTreeByRecursiveDescent( tree_t* tree ){
    colorPrintf( NOMODE, YELLOW, "Enter the name of file, where i will find mathematical statement: " );

    FILE* fileForMathStatement = NULL;

    bufferInformation dataBaseFromFile = getBufferFromFile( &fileForMathStatement );
    char* ptrOnSymbolInPosition = dataBaseFromFile.buffer;

    size_t countOfTokens = 0;
    node_t** tokens = lexAnalysis( &ptrOnSymbolInPosition, &countOfTokens );

    dumpLexArrayInFile( tokens, countOfTokens );
    destroyLexArray( tokens, countOfTokens );
    destroyBufferInformation( &dataBaseFromFile );
    /*tree->rootTree = getGeneral( &ptrOnSymbolInPosition );

    fclose( fileForMathStatement );


    colorPrintf( NOMODE, GREEN, "\nSuccessfully reading an expression from a file\n");*/
    return CORRECT_WORK;
}

node_t* getGeneral( char** ptrOnSymbolInPosition ){
    assert( ptrOnSymbolInPosition );
    assert( *ptrOnSymbolInPosition );

    cleanLineWithCode( ptrOnSymbolInPosition );
    node_t* nodeOperator = NULL;
    do{
        node_t* newOperator = getOperator( ptrOnSymbolInPosition );
        nodeOperator = newStatementNode( STATEMENT, OPERATOR_END, nodeOperator, newOperator );
    }while( **ptrOnSymbolInPosition != '$' );

    cleanLineWithCode( ptrOnSymbolInPosition );

    printf( "char stoped = %c", **ptrOnSymbolInPosition );
    return nodeOperator;
}

node_t* getOperator( char** ptrOnSymbolInPosition ){
    assert( ptrOnSymbolInPosition );
    assert( *ptrOnSymbolInPosition );

    node_t* nodeOperator = NULL;

    cleanLineWithCode( ptrOnSymbolInPosition );
    if( nodeOperator = getAssignment( ptrOnSymbolInPosition )  ){
        cleanLineWithCode( ptrOnSymbolInPosition );
    }
    else if( nodeOperator = getCondition( ptrOnSymbolInPosition ) ){
        cleanLineWithCode( ptrOnSymbolInPosition );
    }
    else{
        cleanLineWithCode( ptrOnSymbolInPosition );
        if( **ptrOnSymbolInPosition == '{' ){
            ++(*ptrOnSymbolInPosition );
        }

        do{
            node_t* newOperator = getOperator( ptrOnSymbolInPosition );
            nodeOperator = newStatementNode( STATEMENT, OPERATOR_END, nodeOperator, newOperator );
        }while( **ptrOnSymbolInPosition != '}' );

        if( **ptrOnSymbolInPosition == '}' ){
            ++(*ptrOnSymbolInPosition );
        }
    }

    char* lineWithOpEnd = NULL;
    size_t lineLen = readingWord( &lineWithOpEnd, ptrOnSymbolInPosition );

    const char* endOfAssignment = getEndOfAssignment();

    if( strcmp( lineWithOpEnd, endOfAssignment ) == 0 ){
        *ptrOnSymbolInPosition += lineLen;
        cleanLineWithCode( ptrOnSymbolInPosition );
    }

    cleanLineWithCode( ptrOnSymbolInPosition );
    free( lineWithOpEnd );
    return nodeOperator;
}

node_t* getCondition( char** ptrOnSymbolInPosition ){
    assert( ptrOnSymbolInPosition );
    assert( *ptrOnSymbolInPosition );

    char* lineForIf = NULL;
    size_t lineLen = readingWord( &lineForIf, ptrOnSymbolInPosition );

    for( size_t statementIndex = 0; statementIndex < sizeOfStatementArray; statementIndex++ ){
        if( arrayWithStatements[ statementIndex ].statement == IF &&
            strcmp( lineForIf, arrayWithStatements[ statementIndex ].viewOfStatementInFile ) == 0 ){

            *ptrOnSymbolInPosition += lineLen;
            if( **ptrOnSymbolInPosition == '(' ){
                ++(*ptrOnSymbolInPosition);
            }
            node_t* left = getExpression( ptrOnSymbolInPosition );
            if( **ptrOnSymbolInPosition == ')' ){
                ++(*ptrOnSymbolInPosition);
            }
            node_t* right = getOperator( ptrOnSymbolInPosition );

            free( lineForIf);
            return newStatementNode( STATEMENT, IF, left, right );
          }
    }

    free( lineForIf );
    cleanLineWithCode( ptrOnSymbolInPosition );
    return NULL;
}

node_t* getAssignment( char** ptrOnSymbolInPosition ){
    assert( ptrOnSymbolInPosition );
    assert( *ptrOnSymbolInPosition );

    cleanLineWithCode( ptrOnSymbolInPosition );
    node_t* left = NULL;
    if( islower( **ptrOnSymbolInPosition) || **ptrOnSymbolInPosition == '_' ){
        left = getVariable( ptrOnSymbolInPosition );
    }
    else{
        return NULL;
    }

    char* lineForAss = NULL;
    size_t lineLen = readingWord( &lineForAss, ptrOnSymbolInPosition );

    for( size_t statementIndex = 0; statementIndex < sizeOfStatementArray; statementIndex++ ){
        if( arrayWithStatements[ statementIndex ].statement == ASSIGNMENT &&
            strcmp( lineForAss, arrayWithStatements[ statementIndex ].viewOfStatementInFile ) == 0 ){

            (*ptrOnSymbolInPosition) += lineLen;
            node_t* right = getExpression( ptrOnSymbolInPosition );
            cleanLineWithCode( ptrOnSymbolInPosition );
            free( lineForAss );
            arrayWithVariableValue[ left->data.variableIndexInArray ] = calculateValue( right );
            return newStatementNode( STATEMENT, ASSIGNMENT, left, right );
        }
    }

    cleanLineWithCode( ptrOnSymbolInPosition );
    free( lineForAss );
    return NULL;
}

const char* getEndOfAssignment(){
    for( size_t statementIndex = 0; statementIndex < sizeOfStatementArray; statementIndex++ ){
        if( arrayWithStatements[ statementIndex ].statement == OPERATOR_END ){
            return arrayWithStatements[ statementIndex ].viewOfStatementInFile;
        }
    }

    return NULL;
}

node_t* getExpression( char** ptrOnSymbolInPosition ){
    assert( ptrOnSymbolInPosition );
    assert( *ptrOnSymbolInPosition );

    cleanLineWithCode( ptrOnSymbolInPosition );
    node_t* left = getTerm( ptrOnSymbolInPosition );

    while( **ptrOnSymbolInPosition == '+' || **ptrOnSymbolInPosition == '-' ){
        char operation = **ptrOnSymbolInPosition;
        ++(*ptrOnSymbolInPosition);

        node_t* right = getTerm( ptrOnSymbolInPosition );

        if( operation == '+' ){
            left = newMathNode( OPERATOR, ADD, left, right );
        }
        else{
            left = newMathNode( OPERATOR, SUB, left, right );
        }
    }

    cleanLineWithCode( ptrOnSymbolInPosition );
    return left;
}

node_t* getTerm( char** ptrOnSymbolInPosition ){
    assert( ptrOnSymbolInPosition );
    assert( *ptrOnSymbolInPosition );

    cleanLineWithCode( ptrOnSymbolInPosition );
    node_t* left = getPrimaryExpression( ptrOnSymbolInPosition );


    while( **ptrOnSymbolInPosition == '*' || **ptrOnSymbolInPosition == '/' ){
        char operation = **ptrOnSymbolInPosition;
        ++(*ptrOnSymbolInPosition);

        cleanLineWithCode( ptrOnSymbolInPosition );
        node_t* right = getPrimaryExpression( ptrOnSymbolInPosition );

        if( operation == '*' ){
            left = newMathNode( OPERATOR, MUL, left, right );
        }
        else if( operation == '/' ){
            left = newMathNode( OPERATOR, DIV, left, right );
        }
    }

    cleanLineWithCode( ptrOnSymbolInPosition );
    return left;
}

node_t* getPrimaryExpression( char** ptrOnSymbolInPosition ){
    assert( ptrOnSymbolInPosition );
    assert( *ptrOnSymbolInPosition );

    cleanLineWithCode( ptrOnSymbolInPosition );

    if( **ptrOnSymbolInPosition == '(' ){
        ++(*ptrOnSymbolInPosition);

        node_t* nodeFromExpression = getExpression( ptrOnSymbolInPosition );

        if( **ptrOnSymbolInPosition == ')' ){
            ++(*ptrOnSymbolInPosition);
        }
        return nodeFromExpression;
    }

    if( islower( **ptrOnSymbolInPosition) ){
        return getVariable( ptrOnSymbolInPosition );
    }
    else{
        return getNumber( ptrOnSymbolInPosition );
    }

}

node_t* getFunctionWithOneArgument( char** ptrOnSymbolInPosition ){
    assert( ptrOnSymbolInPosition );
    assert( *ptrOnSymbolInPosition );

    char* lineForFunc = NULL;
    size_t lineLen = readingWord( &lineForFunc, ptrOnSymbolInPosition );

    for( size_t indexOfMathFunc = 0; indexOfMathFunc < sizeOfMathArray; indexOfMathFunc++ ){
        if( arrayWithMathInfo[ indexOfMathFunc ].functionClass == ONE_ARG &&
            strcmp( lineForFunc, arrayWithMathInfo[ indexOfMathFunc ].viewOfMathOperationInFile ) == 0 ){
            *ptrOnSymbolInPosition += lineLen;
            if( **ptrOnSymbolInPosition == '(' ){
                ++(*ptrOnSymbolInPosition);
            }
            node_t* funcNode = getExpression( ptrOnSymbolInPosition );

            if( **ptrOnSymbolInPosition == ')' ){
                ++(*ptrOnSymbolInPosition);
            }
            free( lineForFunc );
            cleanLineWithCode( ptrOnSymbolInPosition );
            return newMathNode( OPERATOR, arrayWithMathInfo[ indexOfMathFunc ].mathOperation, NULL, funcNode );
        }
    }

    cleanLineWithCode( ptrOnSymbolInPosition );
    free( lineForFunc );
    return NULL;
}


node_t* getVariable( char** ptrOnSymbolInPosition ){
    assert( ptrOnSymbolInPosition );
    assert( *ptrOnSymbolInPosition );

    char* lineWithVar = NULL;
    size_t lineLen = readingVariable( &lineWithVar, ptrOnSymbolInPosition );

    bool isFunction = checkingOnFunction( lineWithVar );
    if( isFunction ){
        free( lineWithVar );
        return getFunctionWithOneArgument( ptrOnSymbolInPosition );
    }

    bool isStatement = checkingOnStatement( lineWithVar );
    if( isStatement ){
        free( lineWithVar );
        return NULL;
    }

    size_t varIndex = 0;
    for( varIndex = 0 ;varIndex < infoForVarArray.freeIndexNow; varIndex++ ){
        if( arrayWithVariables[ varIndex ].nameOfVariable &&
            strcmp( lineWithVar, arrayWithVariables[ varIndex ].nameOfVariable ) == 0 ){
            treeElem_t data = {};
            data.variableIndexInArray = arrayWithVariables[ varIndex ].variableIndexInArray;
            node_t* nodeWithVar = NULL;
            initNode( &nodeWithVar, VARIABLE, data );
            *ptrOnSymbolInPosition += lineLen;
            cleanLineWithCode( ptrOnSymbolInPosition );
            free( lineWithVar );
            return nodeWithVar;
        }
    }

    return makeNodeWithNewVariable( lineWithVar, ptrOnSymbolInPosition,  lineLen, varIndex);
}

node_t* makeNodeWithNewVariable( char* lineWithVar, char** ptrOnSymbolInPosition, size_t lineLen, size_t varIndex ){
    assert( lineWithVar );
    assert( ptrOnSymbolInPosition );
    assert( *ptrOnSymbolInPosition );

    if( infoForVarArray.freeIndexNow == infoForVarArray.capacity - 1){
            infoForVarArray.capacity *= 2;
            arrayWithVariables = (informationWithVariables*)realloc( arrayWithVariables, infoForVarArray.capacity * sizeof( informationWithVariables ) );
            arrayWithVariableValue = (double*)realloc( arrayWithVariableValue, infoForVarArray.capacity * sizeof( double ) );
        }

    arrayWithVariables[ infoForVarArray.freeIndexNow ] = { lineWithVar , infoForVarArray.freeIndexNow  };
    treeElem_t data = {};
    data.variableIndexInArray = arrayWithVariables[ varIndex ].variableIndexInArray;
    node_t* nodeWithVar = NULL;
    initNode( &nodeWithVar, VARIABLE, data );
    ++(infoForVarArray.freeIndexNow);
    *ptrOnSymbolInPosition += lineLen;

    cleanLineWithCode( ptrOnSymbolInPosition );
    return nodeWithVar;
}

size_t readingWord( char** lineWithWord, char** ptrOnSymbolInPosition ){
    assert( lineWithWord );
    assert( ptrOnSymbolInPosition );
    assert( *ptrOnSymbolInPosition );

    cleanLineWithCode( ptrOnSymbolInPosition );

    size_t lineIndex = 0;
    size_t sizeOfLine = startSizeForWord;
    *lineWithWord = (char*)calloc( sizeOfLine, sizeof( char ) );

    while( islower( (*ptrOnSymbolInPosition)[ lineIndex ] ) || (*ptrOnSymbolInPosition)[ lineIndex ] == '_' ){
            isEnoughSize( lineWithWord, &lineIndex, &sizeOfLine );
            (*lineWithWord)[ lineIndex ] = (*ptrOnSymbolInPosition)[ lineIndex ];
            ++lineIndex;
    }
    (*lineWithWord)[ lineIndex ] = '\0';

    cleanLineWithCode( ptrOnSymbolInPosition );
    return lineIndex;
}

size_t readingVariable( char** lineWithVariable, char** ptrOnSymbolInPosition ){
    assert( lineWithVariable );
    assert( ptrOnSymbolInPosition );
    assert( *ptrOnSymbolInPosition );

    cleanLineWithCode( ptrOnSymbolInPosition );

    size_t lineIndex = 0;
    size_t sizeOfLine = startSizeForWord;
    *lineWithVariable = (char*)calloc( sizeOfLine, sizeof( char ) );

    while( islower( (*ptrOnSymbolInPosition)[ lineIndex ] ) || (*ptrOnSymbolInPosition)[ lineIndex ] == '_' ||
          ( '0' <= (*ptrOnSymbolInPosition)[ lineIndex ]  && (*ptrOnSymbolInPosition)[ lineIndex ]  <= '9')  ){
            isEnoughSize( lineWithVariable, &lineIndex, &sizeOfLine );
            (*lineWithVariable)[ lineIndex ] = (*ptrOnSymbolInPosition)[ lineIndex ];
            ++lineIndex;
    }
    (*lineWithVariable)[ lineIndex ] = '\0';

    cleanLineWithCode( ptrOnSymbolInPosition );
    return lineIndex;

}

void isEnoughSize( char** lineWithWord, size_t* lineIndex, size_t* sizeOfLine ){
    assert( lineWithWord );
    assert( lineIndex );
    assert( sizeOfLine );

    if( *lineIndex == *sizeOfLine - 1 ){
        *sizeOfLine *= 2;
        *lineWithWord = (char*)realloc( *lineWithWord, *sizeOfLine * sizeof( char ) );
    }
};

bool checkingOnFunction( char* lineWithWord ){
    assert( lineWithWord );

    for( size_t indexOfMathFunc = 0; indexOfMathFunc < sizeOfMathArray; indexOfMathFunc++ ){
        if( arrayWithMathInfo[ indexOfMathFunc ].functionClass == ONE_ARG &&
            strcmp( lineWithWord, arrayWithMathInfo[ indexOfMathFunc ].viewOfMathOperationInFile ) == 0 ){
            return true;
        }
    }

    return false;
}

bool checkingOnStatement( char* lineWithWord ){
    assert( lineWithWord );

    for( size_t statementIndex = 0; statementIndex < sizeOfStatementArray; statementIndex++ ){
        if( strcmp( lineWithWord, arrayWithStatements[ statementIndex ].viewOfStatementInFile ) == 0 ){
            return true;
        }
    }

    return false;
}

node_t* getNumber( char** ptrOnSymbolInPosition ){
    assert( ptrOnSymbolInPosition );
    assert( *ptrOnSymbolInPosition );

    double value = 0;
    bool statusOfReadNumber = false;
    cleanLineWithCode( ptrOnSymbolInPosition );

    while( '0' <= (**ptrOnSymbolInPosition) &&
                  (**ptrOnSymbolInPosition) <= '9' ){

        value = value * 10 + ( (**ptrOnSymbolInPosition) - '0' );
        ++(*ptrOnSymbolInPosition);
        statusOfReadNumber = true;
    }

    /*if( statusOfReadNumber    == false ){
        colorPrintf( NOMODE, RED, "\nError of getNumber:%s %s %d\n", __FILE__, __func__, __LINE__ );
        exit( 0 );
    }*/

    cleanLineWithCode( ptrOnSymbolInPosition );
    return makeConstNode( value );
}

void cleanLineWithCode( char** ptrOnSymbolInPostion ){
    while( **ptrOnSymbolInPostion == '\n' || **ptrOnSymbolInPostion == ' ' ){
        ++(*ptrOnSymbolInPostion);
    }
}


void dumpLexArrayInFile( node_t** tokens, size_t countOfTokens ){
    colorPrintf( NOMODE, YELLOW, "Enter the name of file, where i will save info about tokens: " );

    char* nameOfFileForTokens = NULL;
    size_t sizeOfAllocationMemory = 0;
    ssize_t sizeOfLine = getlineWrapper( &nameOfFileForTokens, &sizeOfAllocationMemory, stdin );

    if( sizeOfLine == -1 ){
        return ;
    }

    FILE* fileForTokens = fopen( nameOfFileForTokens, "w" );
    fprintf( fileForTokens, "array with tokens[ ] = {\n" );

    for( size_t tokenIndex = 0; tokenIndex < countOfTokens; tokenIndex++ ){
        if( tokens[ tokenIndex ]->nodeValueType == NUMBER ){
            fprintf( fileForTokens, "\t{ type = NUMBER, val = %lg}\n", tokens[ tokenIndex ]->data.number );
        }
        else if( tokens[ tokenIndex ]->nodeValueType == VARIABLE ){
            fprintf( fileForTokens, "\t{ type = VARIABLE, val = '%s'}\n", getStringOfVariable( tokens[ tokenIndex ] ) );
        }
        else if( tokens[ tokenIndex ]->nodeValueType == OPERATOR ){
            fprintf( fileForTokens, "\t{ type = OPERATOR, val = '%s'}\n", getViewOfMathOperation( tokens[ tokenIndex ]) );
        }
        else if( tokens[ tokenIndex ]->nodeValueType == STATEMENT ){
            fprintf( fileForTokens, "\t{ type = STATEMENT, val = '%s'}\n", getViewOfStatement( tokens[ tokenIndex ] ) );
        }
    }
    fprintf( fileForTokens, "};\ncapacity = %lu\n\n", countOfTokens );


    free( nameOfFileForTokens );
    fclose( fileForTokens );
}

void destroyLexArray( node_t** tokens, size_t countOfTokens ){

    for( size_t tokenIndex = 0; tokenIndex < countOfTokens; tokenIndex++ ){

        free( tokens[ tokenIndex ] );
    }

    free( tokens );
}
