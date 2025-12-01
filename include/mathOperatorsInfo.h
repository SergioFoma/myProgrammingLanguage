#ifndef H_GLOBALS
#define H_GLOBALS

#include <stdio.h>

#include "tree.h"

enum functionClasses {
    ONE_ARG   = 0,
    TWO_ARG   = 1,
    TRIG      = 2
};

struct informationWithValueType {
    typeOfDataInNode typeOfData;
    const char* nameOfType;
};

extern informationWithValueType arrayWithValueType[];
extern const size_t sizeOfArrayWithValueType;

struct informationWithMathOperators {
    typeOfMathOperation mathOperation;
    const char* nameOfMathOperation;
    const char* viewOfMathOperationInFile;
    double(*doMathOperation)( double firstNumber, double doubleSecondNumber );
    functionClasses functionClass;
};

extern informationWithMathOperators arrayWithMathInfo[];
extern const size_t sizeOfMathArray;

struct informationWithVariables {
    char* nameOfVariable;
    size_t variableIndexInArray;
};

struct informationWithVariablesArray {
    size_t capacity;
    size_t freeIndexNow;
};

extern informationWithVariables* arrayWithVariables;
extern informationWithVariablesArray infoForVarArray;

struct informationWithStatements {
    typeOfStatement statement;
    const char* nameOfStatement;
    const char* viewOfStatementInFile;
};

extern informationWithStatements arrayWithStatements[];
extern const size_t sizeOfStatementArray;

#endif
