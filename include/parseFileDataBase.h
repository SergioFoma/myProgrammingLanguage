#ifndef H_PARSEFILEDATABASE
#define H_PARSEFILEDATABASE

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "tree.h"

struct bufferInformation{
    char* buffer;
    size_t fileSize;
    size_t bufferSize;
};

enum errorCode {
    correct        = 0,
    fileOpenErr    = 1,
    memoryErr      = 2
};

enum expertSystemErrors {
    CORRECT_WORK            = 0,
    CAN_NOT_DO_ALLOCATION   = 1,
    ERROR_WITH_GETLINE      = 2,
    NOT_CORRECT_WORK        = 3,
    ERROR_OF_CLEAN          = 4,
    TREE_NULL_PTR           = 5,
    TREE_ERRORS             = 6,
    NULL_PTR_IN_FUNC        = 7,
    ERROR_WITH_FILE         = 8,
    ROOT_NULL_PTR           = 9
};

expertSystemErrors writeInformationInFile( tree_t* tree );

void getFileSize( bufferInformation* bufferFromFile, const char* nameOfFile );

errorCode initBufferInformation( bufferInformation *bufferFromFile, FILE* myFile, const char* nameOfFile );

void destroyBufferInformation( bufferInformation *bufferFromFile );

expertSystemErrors createTreeFromFile( tree_t* tree );

node_t* createNodeFromFile( char** ptrOnSymbolInPosition );

char* readNodeNameFromFile( char** ptrOnSymbolInPosition );

expertSystemErrors buildNewNode( node_t** node, char* nodeName );

expertSystemErrors createTreeByRecursiveDescent( tree_t* tree );

node_t* getGeneral( char** ptrOnSymbolInPosition );

node_t* getExpression( char** ptrOnSymbolInPosition );

node_t* getTerm( char** ptrOnSymbolInPosition );

node_t* getPow( char** ptrOnSymbolInPosition );

node_t* getFunctionWithOneArgument( char** ptrOnSymbolInPosition );

node_t* getPrimaryExpression( char** ptrOnSymbolInPosition );

node_t* getVariable( char** ptrOnSymbolInPosition );

node_t* makeNodeWithNewVariable( char* lineWithVar, char** ptrOnSymbolInPosition, size_t lineLen, size_t varIndex );

size_t readingWord( char** lineWithWord, char** ptrOnSymbolInPosition );

void isEnoughSize( char** lineWithWord, size_t* lineIndex, size_t* sizeOfLine );

bool checkingOnFunction( char* lineWithWord );

node_t* getNumber( char** ptrOnSymbolInPosition );

#endif

