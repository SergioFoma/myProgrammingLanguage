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

void initializationTokens( infoForCreateTree* infoForTree ){
    infoForTree->countOfTokens = startSizeForTokens;
    infoForTree->tokens= ( node_t** )calloc( infoForTree->countOfTokens, sizeof( node_t* ) );
    size_t startIndex = 0;
    initializationNodeInArray( infoForTree, startIndex );

}

void initializationNodeInArray( infoForCreateTree* infoForTree, size_t startIndex ){
    assert( infoForTree );

    treeElem_t data = {};
    data.statement = NO_TYPE;
    for( ; startIndex < infoForTree->countOfTokens; startIndex++ ){
        initNode( infoForTree->tokens + startIndex, STATEMENT, data );
    }
}

void lexAnalysis( char** symbol, infoForCreateTree* infoForTree ){
    assert( symbol );
    assert( *symbol );
    assert( infoForTree );

    while( **symbol != '\0' ){
        if( infoForTree->freeIndexNow == ( infoForTree->countOfTokens - 1) ){
            infoForTree->countOfTokens *= 2;
            infoForTree->tokens = ( node_t** )realloc( infoForTree->tokens, sizeof( node_t* ) * infoForTree->countOfTokens );
            initializationNodeInArray( infoForTree, infoForTree->freeIndexNow + 1 );
        }

        bool isSearchStatement = false;
        for( size_t statementIndex = 0; statementIndex < sizeOfStatementArray; statementIndex++ ){
            const char* nameOfStatement = arrayWithStatements[ statementIndex ].viewOfStatementInFile;
            size_t lenOfStatement = strlen( arrayWithStatements[ statementIndex ].viewOfStatementInFile );

            if( strncmp( *symbol, nameOfStatement, lenOfStatement ) == 0 ){

                (infoForTree->tokens)[ infoForTree->freeIndexNow ]->nodeValueType = STATEMENT;
                (infoForTree->tokens)[ infoForTree->freeIndexNow ]->data.statement = arrayWithStatements[ statementIndex ].statement;
                (*symbol) += lenOfStatement;
                ++( infoForTree->freeIndexNow );
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

                (infoForTree->tokens)[ infoForTree->freeIndexNow ]->nodeValueType = OPERATOR;
                (infoForTree->tokens)[ infoForTree->freeIndexNow ]->data.mathOperation = arrayWithMathInfo[ mathIndex ].mathOperation;
                (*symbol) += lenOfMath;
                ++( infoForTree->freeIndexNow );
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

            (infoForTree->tokens)[ infoForTree->freeIndexNow ]->nodeValueType = NUMBER;
            (infoForTree->tokens)[ infoForTree->freeIndexNow ]->data.number = value;
            ++( infoForTree->freeIndexNow );
            continue;
        }


        if( islower( **symbol ) || **symbol == '_' ){
            char* lineWithVar = NULL;
            size_t lineLen = readingVariable( &lineWithVar, symbol );
            bool statusOfSearching = changTypeOfNodeOnVariableNode( infoForTree, symbol, lineWithVar, lineLen );
            if( !statusOfSearching ){
                free( ( infoForTree->tokens )[ infoForTree->freeIndexNow ] );
                ( infoForTree->tokens )[ infoForTree->freeIndexNow ] = makeNodeWithNewVariable( lineWithVar, symbol, lineLen, infoForVarArray.freeIndexNow );
            }
            ++( infoForTree->freeIndexNow );
            continue;
        }

        if( isspace( **symbol ) ){
            cleanLineWithCode( symbol );
            continue;
        }

        colorPrintf( NOMODE, RED, "\n\nERROR OF LEX ANALYSIS\n\n" );
        exit( 0 );
    }
}

bool changTypeOfNodeOnVariableNode( infoForCreateTree* infoForTree, char** ptrOnSymbolInPosition, char* lineWithVar, size_t lineLen ){
    assert( infoForTree );
    assert( lineWithVar );

    size_t varIndex = 0;
    for( varIndex = 0 ;varIndex < infoForVarArray.freeIndexNow; varIndex++ ){
        if( arrayWithVariables[ varIndex ].nameOfVariable &&
            strcmp( lineWithVar, arrayWithVariables[ varIndex ].nameOfVariable ) == 0 ){
            treeElem_t data = {};
            data.variableIndexInArray = arrayWithVariables[ varIndex ].variableIndexInArray;
            ( infoForTree->tokens )[ infoForTree->freeIndexNow ]->nodeValueType = VARIABLE;
            ( infoForTree->tokens )[ infoForTree->freeIndexNow ]->data = data;
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

    infoForCreateTree infoForTree = {};
    initializationTokens( &infoForTree );

    lexAnalysis( &ptrOnSymbolInPosition, &infoForTree );
    infoForTree.currentIndex = 0;                               // start position - zero index
    dumpLexArrayInFile( &infoForTree );

    printf( "\n freeIndex = %lu\n, currentIndex = %lu\n", infoForTree.freeIndexNow, infoForTree.currentIndex  );
    tree->rootTree = getGeneral( &infoForTree );
    colorPrintf( NOMODE, GREEN, "\nSuccessfully reading an expression from a file\n");

    destroyLexArray( &infoForTree );
    destroyBufferInformation( &dataBaseFromFile );
    fclose( fileForMathStatement );
    return CORRECT_WORK;
}

node_t* getGeneral( infoForCreateTree* infoForTree ){
    assert( infoForTree );
    assert( infoForTree->tokens );

    node_t* nodeOperator = NULL;
    do{
        node_t* newOperator = getOperator( infoForTree );
        nodeOperator = newStatementNode( STATEMENT, OPERATOR_END, nodeOperator, newOperator );
        printf( "token if gen = %lu\nsize of array = %lu\n", infoForTree->currentIndex, infoForTree->countOfTokens );
    }while( infoForTree->currentIndex < ( infoForTree->freeIndexNow - 1 ) );

    printf( "token stoped = %lu\nsize of array = %lu", infoForTree->currentIndex, infoForTree->countOfTokens );
    return nodeOperator;
}

node_t* getOperator( infoForCreateTree* infoForTree ){
    assert( infoForTree );
    assert( infoForTree->tokens );

    node_t* nodeOperator = NULL;

    if( nodeOperator = getAssignment( infoForTree )  ){}
    else if( nodeOperator = getCondition( infoForTree ) ){}
    else{
        if( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == STATEMENT &&
            ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.statement == CURLY_PAR_OPEN ){
            ++( infoForTree->currentIndex );
        }

        do{
            node_t* newOperator = getOperator( infoForTree );
            nodeOperator = newStatementNode( STATEMENT, OPERATOR_END, nodeOperator, newOperator );
        }while( !( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == STATEMENT &&
                   ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.statement == CURLY_PAR_CLOSE ) );

        if( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == STATEMENT &&
            ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.statement == CURLY_PAR_CLOSE ){
            ++( infoForTree->currentIndex );
        }
    }

    if( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == STATEMENT &&
        ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.statement == OPERATOR_END ){
        ++( infoForTree->currentIndex );
    }

    return nodeOperator;
}

node_t* getCondition( infoForCreateTree* infoForTree ){
    assert( infoForTree );
    assert( infoForTree->tokens );

    for( size_t statementIndex = 0; statementIndex < sizeOfStatementArray; statementIndex++ ){
        if( arrayWithStatements[ statementIndex ].statement == IF &&
            ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == STATEMENT &&
            ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.statement == IF ){

            ++( infoForTree->currentIndex );
            if( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == STATEMENT &&
                ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.statement == PAR_OPEN ){
                ++( infoForTree->currentIndex );
            }
            node_t* left = getExpression( infoForTree );
            if( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == STATEMENT &&
                ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.statement == PAR_CLOSE ){
                ++( infoForTree->currentIndex );
            }
            node_t* right = getOperator( infoForTree );

            return newStatementNode( STATEMENT, IF, left, right );
          }
    }

    return NULL;
}

node_t* getAssignment( infoForCreateTree* infoForTree ){
    assert( infoForTree );
    assert( infoForTree->tokens );

    node_t* left = NULL;
    if( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == VARIABLE ){
        left = getVariable( infoForTree );
    }
    else{
        return NULL;
    }

    for( size_t statementIndex = 0; statementIndex < sizeOfStatementArray; statementIndex++ ){
        if( arrayWithStatements[ statementIndex ].statement == ASSIGNMENT &&
            ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == STATEMENT &&
            ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.statement == ASSIGNMENT ){

            ++( infoForTree->currentIndex );
            node_t* right = getExpression( infoForTree );
            arrayWithVariableValue[ left->data.variableIndexInArray ] = calculateValue( right );
            return newStatementNode( STATEMENT, ASSIGNMENT, left, right );
        }
    }
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

node_t* getExpression( infoForCreateTree* infoForTree ){
    assert( infoForTree );
    assert( infoForTree->tokens );

    node_t* left = getTerm( infoForTree );

    while( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == OPERATOR &&
           ( ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.mathOperation == ADD ||
             ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.mathOperation == SUB ) ){

        typeOfMathOperation currentMathOp = ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.mathOperation;
        ++( infoForTree->currentIndex );

        node_t* right = getTerm( infoForTree );

        if( currentMathOp == ADD ){
            left = newMathNode( OPERATOR, ADD, left, right );
        }
        else{
            left = newMathNode( OPERATOR, SUB, left, right );
        }
    }

    return left;
}

node_t* getTerm( infoForCreateTree* infoForTree ){
    assert( infoForTree );
    assert( infoForTree->tokens );

    node_t* left = getPrimaryExpression( infoForTree );


    while( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == OPERATOR &&
           ( ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.mathOperation == MUL ||
             ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.mathOperation == DIV ) ){

        typeOfMathOperation currentMathOp = ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.mathOperation;
        ++( infoForTree->currentIndex );

        node_t* right = getPrimaryExpression( infoForTree );

        if( currentMathOp == MUL ){
            left = newMathNode( OPERATOR, MUL, left, right );
        }
        else if( currentMathOp == DIV ){
            left = newMathNode( OPERATOR, DIV, left, right );
        }
    }

    return left;
}

node_t* getPrimaryExpression( infoForCreateTree* infoForTree ){
    assert( infoForTree );
    assert( infoForTree->tokens );

    if( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == STATEMENT &&
        ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.statement == PAR_OPEN ){
        ++( infoForTree->currentIndex );

        node_t* nodeFromExpression = getExpression( infoForTree );

        if( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == STATEMENT &&
            ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.statement == PAR_CLOSE ){
            ++( infoForTree->currentIndex );
        }
        return nodeFromExpression;
    }

    if( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType != NUMBER ){
        return getVariable( infoForTree );
    }
    else{
        return getNumber( infoForTree );
    }

}

node_t* getFunctionWithOneArgument( infoForCreateTree* infoForTree ){
    assert( infoForTree );
    assert( infoForTree->tokens );

    for( size_t indexOfMathFunc = 0; indexOfMathFunc < sizeOfMathArray; indexOfMathFunc++ ){
        if( arrayWithMathInfo[ indexOfMathFunc ].functionClass == ONE_ARG &&
            ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == OPERATOR &&
            ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.mathOperation == arrayWithMathInfo[ indexOfMathFunc ].mathOperation ){

            ++( infoForTree->currentIndex );
            if( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == STATEMENT &&
                ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.statement == PAR_OPEN ){
                ++( infoForTree->currentIndex );
            }
            node_t* funcNode = getExpression( infoForTree );

            if( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == STATEMENT &&
                ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.statement == PAR_CLOSE ){
                ++( infoForTree->currentIndex );
            }
            return newMathNode( OPERATOR, arrayWithMathInfo[ indexOfMathFunc ].mathOperation, NULL, funcNode );
        }
    }

    return NULL;
}


node_t* getVariable( infoForCreateTree* infoForTree ){
    assert( infoForTree );
    assert( infoForTree->tokens );

    bool isFunction = checkingOnFunction( infoForTree );
    if( isFunction ){
        return getFunctionWithOneArgument( infoForTree );
    }

    bool isStatement = checkingOnStatement( infoForTree );
    if( isStatement ){
        return NULL;
    }

    size_t treeIndex = ( infoForTree->currentIndex );
    ++( infoForTree->currentIndex );
    return copyNode( ( infoForTree->tokens )[ treeIndex ] );
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

bool checkingOnFunction( infoForCreateTree* infoForTree ){
    assert( infoForTree );

    for( size_t indexOfMathFunc = 0; indexOfMathFunc < sizeOfMathArray; indexOfMathFunc++ ){
        if( arrayWithMathInfo[ indexOfMathFunc ].functionClass == ONE_ARG &&
        ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == OPERATOR &&
        ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.mathOperation == arrayWithMathInfo[ indexOfMathFunc ].mathOperation ){
            return true;
        }
    }

    return false;
}

bool checkingOnStatement( infoForCreateTree* infoForTree ){
    assert( infoForTree );

    for( size_t statementIndex = 0; statementIndex < sizeOfStatementArray; statementIndex++ ){
        if( ( infoForTree->tokens )[ infoForTree->currentIndex ]->nodeValueType == STATEMENT &&
            ( infoForTree->tokens )[ infoForTree->currentIndex ]->data.statement == arrayWithStatements[ statementIndex ].statement ){

                return true;
            }
    }

    return false;
}

node_t* getNumber( infoForCreateTree* infoForTree ){
    assert( infoForTree );
    assert( infoForTree->tokens );

    if( (infoForTree->tokens)[ infoForTree->currentIndex ]->nodeValueType == NUMBER ){
        size_t treeIndex = infoForTree->currentIndex;
        ++( infoForTree->currentIndex );
        return copyNode( (infoForTree->tokens)[ treeIndex ] );
    }

    colorPrintf( NOMODE, RED, "ERROR:%s %s %d\n typeOfNode = %d, current index = %lu\n", __FILE__, __func__, __LINE__,
                (infoForTree->tokens)[ infoForTree->currentIndex ]->nodeValueType, infoForTree->currentIndex );
    return NULL;
}

void cleanLineWithCode( char** ptrOnSymbolInPostion ){
    while( **ptrOnSymbolInPostion == '\n' || **ptrOnSymbolInPostion == ' ' ){
        ++(*ptrOnSymbolInPostion);
    }
}


void dumpLexArrayInFile( infoForCreateTree* infoForTree ){
    assert( infoForTree );

    colorPrintf( NOMODE, YELLOW, "Enter the name of file, where i will save info about tokens: " );

    char* nameOfFileForTokens = NULL;
    size_t sizeOfAllocationMemory = 0;
    ssize_t sizeOfLine = getlineWrapper( &nameOfFileForTokens, &sizeOfAllocationMemory, stdin );

    if( sizeOfLine == -1 ){
        return ;
    }

    FILE* fileForTokens = fopen( nameOfFileForTokens, "w" );
    fprintf( fileForTokens, "array with tokens[ %lu ] = {\n", infoForTree->freeIndexNow );

    for( size_t tokenIndex = 0; tokenIndex < infoForTree->countOfTokens; tokenIndex++ ){
        if( ( infoForTree->tokens )[ tokenIndex ] == NULL ){
            continue;
        }

        if( ( infoForTree->tokens )[ tokenIndex ]->nodeValueType == NUMBER ){
            fprintf( fileForTokens, "\t[%lu] = { type = NUMBER, val = %lg}\n", tokenIndex, ( infoForTree->tokens )[ tokenIndex ]->data.number );
        }
        else if( ( infoForTree->tokens )[ tokenIndex ]->nodeValueType == VARIABLE ){
            fprintf( fileForTokens, "\t[%lu] = { type = VARIABLE, val = '%s'}\n", tokenIndex, getStringOfVariable( ( infoForTree->tokens )[ tokenIndex] ) );
        }
        else if( ( infoForTree->tokens )[ tokenIndex ]->nodeValueType == OPERATOR ){
            fprintf( fileForTokens, "\t[%lu] ={ type = OPERATOR, val = '%s'}\n", tokenIndex, getViewOfMathOperation( ( infoForTree->tokens )[ tokenIndex ] ) );
        }
        else if( ( infoForTree->tokens )[ tokenIndex ]->nodeValueType == STATEMENT ){
            fprintf( fileForTokens, "\t[%lu] = { type = STATEMENT, val = '%s'}\n", tokenIndex, getViewOfStatement( ( infoForTree->tokens )[ tokenIndex ] ) );
        }
    }
    fprintf( fileForTokens, "};\ncapacity = %lu\n\n", infoForTree->countOfTokens );


    free( nameOfFileForTokens );
    fclose( fileForTokens );
}

void destroyLexArray( infoForCreateTree* infoForTree ){
    assert( infoForTree );

    for( size_t tokenIndex = 0; tokenIndex < infoForTree->countOfTokens; tokenIndex++ ){

        free( ( infoForTree->tokens )[tokenIndex ] );
    }

    free( infoForTree->tokens );
}
