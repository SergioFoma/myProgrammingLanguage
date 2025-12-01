#ifndef H_MATHEMATICAL_CALCULATOR
#define H_MATHEMATICAL_CALCULATOR

#include "tree.h"

enum mathErrors {
    CORRECT_CALCULATE           = 0,
    CORRECT_DIFFERENTIATION     = 1,
    NULL_PTR                    = 2
};

enum statusOfFind {
    DETECTED_VAR        = 0,
    NOT_DETECTED_VAR    = 1,
    ERROR_OF_FIND_VAR   = 2
};

mathErrors calculateTheFunctionValue( tree_t* tree );

double calculateValue( node_t* node );

double doMathOperations( node_t* node, double firstNumber, double secondNumber );

node_t* copyNode( node_t* node );

node_t* makeConstNode( double value );

node_t* newMathNode( typeOfDataInNode nodeType, typeOfMathOperation mathOperator, node_t* leftNode, node_t* rightNode );

void summaryOfConstants( tree_t* treeForOptimisation );

void optimisationConsts( node_t* node );

void removingNeutralElements( tree_t* treeForOptimisation );

void optimisationNeutralElem( node_t* node );

statusOfFind variableSearching( const node_t* node, size_t variable );

double doAdd( double firstNumber, double secondNumber );
double doSub( double firstNumber, double secondNumber );
double doMul( double firstNumber, double secondNumber );
double doDiv( double firstNumber, double secondNumber );

void destroyArrayWithVariables();

void printArrayWithVariablesInFile();

#endif
