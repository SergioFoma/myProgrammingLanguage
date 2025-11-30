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

mathErrors differentiationOfTheFunction( tree_t* tree, tree_t* differentiationTree, FILE* fileForDump );

node_t* differentiation( const node_t* node, size_t variable, FILE* fileForDump );

node_t* copyNode( node_t* node );

node_t* makeConstNode( double value );

node_t* newNode( typeOfDataInNode nodeType, typeOfMathOperation mathOperator, node_t* leftNode, node_t* rightNode );

void summaryOfConstants( tree_t* treeForOptimisation );

void optimisationConsts( node_t* node );

void removingNeutralElements( tree_t* treeForOptimisation );

void optimisationNeutralElem( node_t* node );

node_t* differentiationAdd( const node_t* node, size_t variable, FILE* fileForDump);
node_t* differentiationSub( const node_t* node, size_t variable, FILE* fileForDump);
node_t* differentiationMul( const node_t* node, size_t variable, FILE* fileForDump);
node_t* differentiationDiv( const node_t* node, size_t variable, FILE* fileForDump);
node_t* differentiationLn( const node_t* node, size_t variable, FILE* fileForDump);
node_t* differentiationLog( const node_t* node, size_t variable, FILE* fileForDump);
node_t* differentiationSin( const node_t* node, size_t variable, FILE* fileForDump);
node_t* differentiationCos( const node_t* node, size_t variable, FILE* fileForDump);
node_t* differentiationTg( const node_t* node, size_t variable, FILE* fileForDump);
node_t* differentiationCtg( const node_t* node, size_t variable, FILE* fileForDump);
node_t* differentiationArcsin( const node_t* node, size_t variable, FILE* fileForDump);
node_t* differentiationArccos( const node_t* node, size_t variable, FILE* fileForDump);
node_t* differentiationArctg( const node_t* node, size_t variable, FILE* fileForDump);
node_t* differentiationArcctg( const node_t* node, size_t variable, FILE* fileForDump);
node_t* differentiationSh( const node_t* node, size_t variable, FILE* fileForDump);
node_t* differentiationCh( const node_t* node, size_t variable, FILE* fileForDump);
node_t* differentiationTh( const node_t* node, size_t variable, FILE* fileForDump);
node_t* differentiationCth( const node_t* node, size_t variable, FILE* fileForDump);
node_t* differentiationExp( const node_t* node, size_t variable, FILE* fileForDump);
node_t* differentiationSqrt( const node_t* node, size_t variable, FILE* fileForDump);
node_t* differentiationPow( const node_t* node, size_t variable, FILE* fileForDump );

statusOfFind variableSearching( const node_t* node, size_t variable );

double doAdd( double firstNumber, double secondNumber );
double doSub( double firstNumber, double secondNumber );
double doMul( double firstNumber, double secondNumber );
double doDiv( double firstNumber, double secondNumber );

void destroyArrayWithVariables();

void printArrayWithVariablesInFile();

#endif
