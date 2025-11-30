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

expertSystemErrors createTreeByRecursiveDescent( tree_t* tree ){
    colorPrintf( NOMODE, YELLOW, "Enter the name of file, where i will find mathematical statement: " );

    char* nameOfFileForMathStatement = NULL;
    size_t sizeOfAllocationMemory = 0;
    ssize_t sizeOfLine = getlineWrapper( &nameOfFileForMathStatement, &sizeOfAllocationMemory, stdin );

    if( sizeOfLine == -1 ){
        return ERROR_WITH_GETLINE;
    }

    FILE* fileForMathStatement = fopen( nameOfFileForMathStatement, "r" );
    if( fileForMathStatement == NULL ){
        colorPrintf( NOMODE, RED, "\ncan not open file:%s %s %d\n", __FILE__, __func__, __LINE__ );
        free( nameOfFileForMathStatement );
        return ERROR_WITH_FILE;
    }

    bufferInformation dataBaseFromFile = {};
    errorCode statusOfReadFromFile = initBufferInformation( &dataBaseFromFile, fileForMathStatement, nameOfFileForMathStatement);
    if( statusOfReadFromFile != correct ){
        return ERROR_WITH_FILE;
    }

    char* ptrOnSymbolInPosition = dataBaseFromFile.buffer;
    tree->rootTree = getGeneral( &ptrOnSymbolInPosition );

    free( nameOfFileForMathStatement );
    fclose( fileForMathStatement );
    destroyBufferInformation( &dataBaseFromFile );

    colorPrintf( NOMODE, GREEN, "\nSuccessfully reading an expression from a file\n");
    return CORRECT_WORK;
}

node_t* getGeneral( char** ptrOnSymbolInPosition ){
    assert( ptrOnSymbolInPosition );
    assert( *ptrOnSymbolInPosition );

    node_t* expression = getExpression( ptrOnSymbolInPosition );

    printf( "char gen = %c\n", **ptrOnSymbolInPosition );

    if( **ptrOnSymbolInPosition != '$' ){
        printf( "%c\n", **ptrOnSymbolInPosition );
        colorPrintf( NOMODE, RED, "\nError of getGeneral:%s %s %d\n", __FILE__, __func__, __LINE__ );
        exit( 0 );
    }

    ++(*ptrOnSymbolInPosition);
    return expression;
}

node_t* getExpression( char** ptrOnSymbolInPosition ){
    assert( ptrOnSymbolInPosition );
    assert( *ptrOnSymbolInPosition );

    node_t* left = getTerm( ptrOnSymbolInPosition );

    while( **ptrOnSymbolInPosition == '+' || **ptrOnSymbolInPosition == '-' ){
        char operation = **ptrOnSymbolInPosition;
        ++(*ptrOnSymbolInPosition);

        node_t* right = getTerm( ptrOnSymbolInPosition );

        if( operation == '+' ){
            left = newNode( OPERATOR, ADD, left, right );
        }
        else{
            left = newNode( OPERATOR, SUB, left, right );
        }
    }

    return left;
}

node_t* getTerm( char** ptrOnSymbolInPosition ){
    assert( ptrOnSymbolInPosition );
    assert( *ptrOnSymbolInPosition );

    node_t* left = getPrimaryExpression( ptrOnSymbolInPosition );


    while( **ptrOnSymbolInPosition == '*' || **ptrOnSymbolInPosition == '/' ){
        char operation = **ptrOnSymbolInPosition;
        ++(*ptrOnSymbolInPosition);

        node_t* right = getPrimaryExpression( ptrOnSymbolInPosition );

        if( operation == '*' ){
            left = newNode( OPERATOR, MUL, left, right );
        }
        else if( operation == '/' ){
            left = newNode( OPERATOR, DIV, left, right );
        }
    }

    return left;
}

/*node_t* getPow( char** ptrOnSymbolInPosition ){
    assert( ptrOnSymbolInPosition );
    assert( **ptrOnSymbolInPosition );

    node_t* left = getFunctionWithOneArgument( ptrOnSymbolInPosition );

    while( **ptrOnSymbolInPosition == '^' ){
        char operation = **ptrOnSymbolInPosition;
        ++(*ptrOnSymbolInPosition);

        node_t* right = getFunctionWithOneArgument( ptrOnSymbolInPosition );

        if( operation == '^' ){
            left = newNode( OPERATOR, POW, left, right );
        }
    }

    return left;

}*/

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
            return newNode( OPERATOR, arrayWithMathInfo[ indexOfMathFunc ].mathOperation, NULL, funcNode );
        }
    }

    free( lineForFunc );
    return NULL;
}

node_t* getPrimaryExpression( char** ptrOnSymbolInPosition ){
    assert( ptrOnSymbolInPosition );
    assert( *ptrOnSymbolInPosition );


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

node_t* getVariable( char** ptrOnSymbolInPosition ){
    assert( ptrOnSymbolInPosition );
    assert( *ptrOnSymbolInPosition );

    char* lineWithVar = NULL;
    size_t lineLen = readingWord( &lineWithVar, ptrOnSymbolInPosition );

    bool isFunction = checkingOnFunction( lineWithVar );
    printf( "\n bool = %d\n", isFunction );
    if( isFunction == true ){
        free( lineWithVar );
        return getFunctionWithOneArgument( ptrOnSymbolInPosition );
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
        }

    arrayWithVariables[ infoForVarArray.freeIndexNow ] = { lineWithVar , infoForVarArray.freeIndexNow  };
    treeElem_t data = {};
    data.variableIndexInArray = arrayWithVariables[ varIndex ].variableIndexInArray;
    node_t* nodeWithVar = NULL;
    initNode( &nodeWithVar, VARIABLE, data );
    ++(infoForVarArray.freeIndexNow);
    *ptrOnSymbolInPosition += lineLen;
    return nodeWithVar;

}

size_t readingWord( char** lineWithWord, char** ptrOnSymbolInPosition ){
    assert( lineWithWord );

    size_t lineIndex = 0;
    size_t sizeOfLine = startSizeForWord;
    *lineWithWord = (char*)calloc( sizeOfLine, sizeof( char ) );

    while( islower( (*ptrOnSymbolInPosition)[ lineIndex ] ) ){
            isEnoughSize( lineWithWord, &lineIndex, &sizeOfLine );
            (*lineWithWord)[ lineIndex ] = (*ptrOnSymbolInPosition)[ lineIndex ];
            ++lineIndex;
    }
    (*lineWithWord)[ lineIndex ] = '\0';

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

node_t* getNumber( char** ptrOnSymbolInPosition ){
    assert( ptrOnSymbolInPosition );
    assert( *ptrOnSymbolInPosition );

    double value = 0;
    bool statusOfReadNumber = false;

    while( '0' <= (**ptrOnSymbolInPosition) &&
                  (**ptrOnSymbolInPosition) <= '9' ){

        value = value * 10 + ( (**ptrOnSymbolInPosition) - '0' );
        ++(*ptrOnSymbolInPosition);
        statusOfReadNumber = true;
    }

    /*if( statusOfReadNumber == false ){
        colorPrintf( NOMODE, RED, "\nError of getNumber:%s %s %d\n", __FILE__, __func__, __LINE__ );
        exit( 0 );
    }*/

    return makeConstNode( value );
}
