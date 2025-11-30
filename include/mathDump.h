#ifndef H_MATHDUMP
#define H_MATHDUMP

#include "tree.h"

enum operationComparison {
    MORE_BY_PRIORITY  = 0,
    LOWER_IN_PRIORITY = 1,
    ERROR_OF_COMPARE  = 2
};

char* askInformationAboutMathDump();

FILE* beginMathDump( char* nameOfFileForLatexDump );

treeErrors dumpMathTree( tree_t* tree, FILE* fileForLatex );

void endMathDump( FILE* fileForLatex, char* nameOfFileForLatexDump );

void printMathematicalFormulas( FILE* fileForPrint, node_t* node );

void printResultOfDifferentiation( FILE* fileForPrint, node_t* originalNode, node_t* diffNode );

void printNumberInLatex( FILE* fileForLatex, node_t* node );

operationComparison compareTwoMathOperator( node_t* currentNode, node_t* parentNode );

void printMathFunction( FILE* fileForLatex, node_t* node );

void printCenterFunctionInLatex( FILE* fileForLatex, node_t* node );

void printFunctionOfTwoArguments( FILE* fileForLatex, node_t* node );

void printFunctionOfOneArguments( FILE* fileForLatex, node_t* node );

void printFunctionWithOneStaples( FILE* fileForLatex, node_t* node );

#endif
