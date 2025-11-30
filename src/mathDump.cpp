#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "mathDump.h"
#include "treeDump.h"
#include "myStringFunction.h"
#include "paint.h"
#include "mathOperatorsInfo.h"

char* askInformationAboutMathDump(){
    colorPrintf( NOMODE, YELLOW, "Enter the name of file, where i will save latex dump: " );

    char* nameOfFileForLatexDump = NULL;
    size_t sizeOfAllocationMemory = 0;
    ssize_t sizeOfLine = getlineWrapper( &nameOfFileForLatexDump, &sizeOfAllocationMemory, stdin );

    if( sizeOfLine == -1 ){
        return NULL;
    }

    return nameOfFileForLatexDump;
}

FILE* beginMathDump( char* nameOfFileForLatexDump ){
    assert( nameOfFileForLatexDump );

    FILE* fileForLatex = fopen( nameOfFileForLatexDump, "w" );
    if( fileForLatex == NULL ){
        return NULL;
    }

    fprintf( fileForLatex, "\\documentclass[12pt, litterpaper]{article}\n"
                            "\\usepackage{graphicx}\n"
                            "\\usepackage{mathtext}\n"
                            "\\usepackage[utf8]{inputenc}\n"
                            "\\usepackage[english, russian]{babel}\n"
                            "\\usepackage{float}\n"
                            "\\usepackage{tabularx}\n"
                            "\\begin{document}\n"

    );

    fprintf( fileForLatex, "\\textb{Дифференцирование математический функций в LaTeX:}\n"
                            "\\newline\n\\newline\n"
    );

    return fileForLatex;
}

treeErrors dumpMathTree( tree_t* tree, FILE* fileForLatex ){
    assert( tree );
    assert( fileForLatex );

    fprintf( fileForLatex, "\n\\[ " );

    printMathematicalFormulas( fileForLatex, tree->rootTree );

    fprintf( fileForLatex, " \\]\n" );

    return CORRECT_TREE;
}

void endMathDump( FILE* fileForLatex, char* nameOfFileForLatexDump ){
    assert( fileForLatex );
    assert( nameOfFileForLatexDump );

    fprintf( fileForLatex, "\n\\end{document}" );

    fclose( fileForLatex );
    free( nameOfFileForLatexDump );

}

void printMathematicalFormulas( FILE* fileForLatex, node_t* node ){
    assert( node );
    assert( fileForLatex );

    if( node->nodeValueType == NUMBER ){
        printNumberInLatex( fileForLatex, node );
    }
    else if( node->nodeValueType == VARIABLE ){
        fprintf( fileForLatex, " %s ", getStringOfVariable( node ) );
    }
    else{
        printMathFunction( fileForLatex, node );
    }

}

operationComparison compareTwoMathOperator( node_t* currentNode, node_t* parentNode ){
    if( currentNode == NULL ){
        return ERROR_OF_COMPARE;
    }
    if( parentNode == NULL || currentNode->nodeValueType != OPERATOR ){
        return MORE_BY_PRIORITY;
    }

    if( ( currentNode->data.mathOperation == ADD || currentNode->data.mathOperation == SUB ) &&
        !( parentNode->data.mathOperation == ADD || parentNode->data.mathOperation == SUB ) &&
          parentNode->data.mathOperation != DIV ){

        return LOWER_IN_PRIORITY;
    }
    else if( (currentNode->data.mathOperation == MUL || currentNode->data.mathOperation == DIV ) &&
             ( parentNode->data.mathOperation == LOG || parentNode->data.mathOperation == LN ||
               parentNode->data.mathOperation == POW || parentNode->data.mathOperation == SIN ||
               parentNode->data.mathOperation == COS || parentNode->data.mathOperation == TG ||
               parentNode->data.mathOperation == CTG || parentNode->data.mathOperation == SH ||
               parentNode->data.mathOperation == CH   ) ){

               return LOWER_IN_PRIORITY;
    }
    else if( ( currentNode->data.mathOperation == SIN || currentNode->data.mathOperation == COS ||
               currentNode->data.mathOperation == TG ||  currentNode->data.mathOperation == CTG ||
               currentNode->data.mathOperation == ARCSIN || currentNode->data.mathOperation == ARCCOS ||
               currentNode->data.mathOperation == ARCTG || currentNode->data.mathOperation == ARCCTG ||
               currentNode->data.mathOperation == LN || currentNode->data.mathOperation == LOG ) &&
               parentNode->data.mathOperation == POW ){

                return LOWER_IN_PRIORITY;
    }
    else{
        return MORE_BY_PRIORITY;
    }
}

void printNumberInLatex( FILE* fileForLatex, node_t* node ){
    assert( fileForLatex );
    assert( node );

    if( node->data.number < 0 ){
        fprintf( fileForLatex, " (%lg) ", node->data.number );
    }
    else{
        fprintf( fileForLatex, " %lg ", node->data.number );
    }
}

void printMathFunction( FILE* fileForLatex, node_t* node ){
    assert( fileForLatex );
    assert( node );

    for( size_t functionIndex = 0; functionIndex < sizeOfMathArray; functionIndex++ ){
        if( node->data.mathOperation == arrayWithMathInfo[ functionIndex ].mathOperation ){
            arrayWithMathInfo[ functionIndex ].printInLatex( fileForLatex, node );
            break;
        }
    }
}

void printCenterFunctionInLatex( FILE* fileForLatex, node_t* node ){
    assert( fileForLatex );
    assert( node );

    operationComparison statusOfCompare = compareTwoMathOperator( node, node->parent );
    if( statusOfCompare == LOWER_IN_PRIORITY ){
        fprintf( fileForLatex, "\\left(" );
    }
    if( node->left ){
        printMathematicalFormulas( fileForLatex, node->left );
    }

    for( size_t centerFunctionInLatex = 0; centerFunctionInLatex < sizeOfMathArray; centerFunctionInLatex++ ){
        if( node->data.mathOperation == arrayWithMathInfo[ centerFunctionInLatex ].mathOperation ){
            fprintf( fileForLatex, "%s", arrayWithMathInfo[ centerFunctionInLatex ].viewInLatex );
            break;
        }
    }

    if( node->right ){
        printMathematicalFormulas( fileForLatex, node->right );
    }
    if( statusOfCompare == LOWER_IN_PRIORITY ){
        fprintf( fileForLatex, "\\right)" );
    }
}

void printFunctionOfTwoArguments( FILE* fileForLatex, node_t* node ){
    assert( fileForLatex );
    assert( node );

    operationComparison statusOfCompare = compareTwoMathOperator( node, node->parent );
    if( statusOfCompare == LOWER_IN_PRIORITY ){
        fprintf( fileForLatex, "\\left(" );
    }

    for( size_t functionOfTwoArguments = 0; functionOfTwoArguments < sizeOfMathArray; functionOfTwoArguments++ ){
        if( node->data.mathOperation == arrayWithMathInfo[ functionOfTwoArguments ].mathOperation ){
            fprintf( fileForLatex, "%s", arrayWithMathInfo[ functionOfTwoArguments ].viewInLatex );
            break;
        }
    }

    if( node->left ){
        printMathematicalFormulas( fileForLatex, node->left );
    }
    fprintf( fileForLatex, "}{" );
    if( node->right ){
        printMathematicalFormulas( fileForLatex, node->right );
    }
    fprintf( fileForLatex, "}" );
    if( statusOfCompare == LOWER_IN_PRIORITY ){
        fprintf( fileForLatex, "\\right)" );
    }
}

void printFunctionOfOneArguments( FILE* fileForLatex, node_t* node ){
    assert( fileForLatex );
    assert( node );

    operationComparison statusOfCompare = compareTwoMathOperator( node, node->parent );
    if( statusOfCompare == LOWER_IN_PRIORITY ){
        fprintf( fileForLatex, "\\left(" );
    }

    for( size_t functionOfOneArgument = 0; functionOfOneArgument < sizeOfMathArray; functionOfOneArgument++ ){
        if( node->data.mathOperation == arrayWithMathInfo[functionOfOneArgument  ].mathOperation ){
            fprintf( fileForLatex, "%s", arrayWithMathInfo[ functionOfOneArgument ].viewInLatex );
            break;
        }
    }

    if( node->right ){
        fprintf( fileForLatex, " \\left( " );
        printMathematicalFormulas( fileForLatex, node->right );
        fprintf( fileForLatex, " \\right) " );
    }
    if( statusOfCompare == LOWER_IN_PRIORITY ){
        fprintf( fileForLatex, "\\right)" );
    }
}

void printFunctionWithOneStaples( FILE* fileForLatex, node_t* node ){
    assert( fileForLatex );
    assert( node );

    operationComparison statusOfCompare = compareTwoMathOperator( node, node->parent );
    if( statusOfCompare == LOWER_IN_PRIORITY ){
        fprintf( fileForLatex, "\\left(" );
    }
    if( node->left ){
        printMathematicalFormulas( fileForLatex, node->left );
    }

    for( size_t functionWithOneStaples = 0; functionWithOneStaples < sizeOfMathArray; functionWithOneStaples++ ){
        if( node->data.mathOperation == arrayWithMathInfo[ functionWithOneStaples ].mathOperation ){
            fprintf( fileForLatex, "%s", arrayWithMathInfo[ functionWithOneStaples ].viewInLatex );
            break;
        }
    }

    if( node->right ){
        printMathematicalFormulas( fileForLatex, node->right );
        fprintf( fileForLatex, "}" );
    }
    if( statusOfCompare == LOWER_IN_PRIORITY ){
        fprintf( fileForLatex, "\\right)" );
    }

}

void printResultOfDifferentiation( FILE* fileForPrint, node_t* originalNode, node_t* diffNode ){
    assert( fileForPrint );
    assert( originalNode );
    assert( diffNode );

    fprintf( fileForPrint, "\\[ \\frac{ d }{ dx }( " );

    printMathematicalFormulas( fileForPrint, originalNode );

    fprintf( fileForPrint, ") = " );

    printMathematicalFormulas( fileForPrint, diffNode );

    fprintf( fileForPrint, " \\]\n " );
}

