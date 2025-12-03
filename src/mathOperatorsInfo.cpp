#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "mathOperatorsInfo.h"
#include "mathematicalCalculator.h"

informationWithMathOperators arrayWithMathInfo[] = {
        { ADD   , "ADD"     , "+"       ,   doAdd       ,   TWO_ARG    },
        { SUB   , "SUB"     , "-"       ,   doSub       ,   TWO_ARG    },
        { MUL   , "MUL"     , "*"       ,   doMul       ,   TWO_ARG    },
        { DIV   , "DIV"     , "/"       ,   doDiv       ,   TWO_ARG    },
        { LN    , "LN"      , "ln"      ,   doLn        ,   ONE_ARG    },
        { POW   , "POW"     , "^"       ,   doPow       ,   TWO_ARG    },
        { SIN   , "SIN"     , "sin"     ,   doSin       ,   ONE_ARG    },
        { COS   , "COS"     , "cos"     ,   doCos       ,   ONE_ARG    },
        { TG    , "TG"      , "tg"      ,   doTg        ,   ONE_ARG    },
        { CTG   , "CTG"     , "ctg"     ,   doCtg       ,   ONE_ARG    },
        { ARCSIN, "ARCSIN"  , "arcsin"  ,   doArcsin    ,   ONE_ARG    },
        { ARCCOS, "ARCCOS"  , "arccos"  ,   doArccos    ,   ONE_ARG    },
        { ARCTG , "ARCTG"   , "arctg"   ,   doArctg     ,   ONE_ARG    },
        { ARCCTG, "ARCCTG"  , "arcctg"  ,   doArcctg    ,   ONE_ARG    },
        { SH    , "SH"      , "sh"      ,   doSh        ,   ONE_ARG    },
        { CH    , "CH"      , "ch"      ,   doCh        ,   ONE_ARG    },
        { TH    , "TH"      , "th"      ,   doTh        ,   ONE_ARG    },
        { CTH   , "CTH"     , "cth"     ,   doCth       ,   ONE_ARG    },
        { EXP   , "EXP"     , "exp"     ,   doExp       ,   ONE_ARG    },
        { SQRT  , "SQRT"    , "sqrt"    ,   doSqrt      ,   ONE_ARG    }
};
const size_t sizeOfMathArray = sizeof( arrayWithMathInfo ) / sizeof( arrayWithMathInfo[ 0 ] );

informationWithStatements arrayWithStatements[] = {
        { ASSIGNMENT    ,   "ASSIGNMENT"    ,   "pabgo_govnito"          },
        { OPERATOR_END  ,   "OP_END"        ,   "gusi"                   },
        { IF          ,     "IF"            ,   "wool"                   }
};
const size_t sizeOfStatementArray = sizeof( arrayWithStatements ) / sizeof( arrayWithStatements[0] );

size_t startIndexForArrayWithVar = 0;
informationWithVariables* arrayWithVariables = ( informationWithVariables* )calloc( startIndexForArrayWithVar + 1, sizeof( informationWithVariables ) );
informationWithVariablesArray infoForVarArray = { startIndexForArrayWithVar + 1, startIndexForArrayWithVar };

double* arrayWithVariableValue = (double*)calloc( startIndexForArrayWithVar + 1, sizeof( double ) );

informationWithValueType arrayWithValueType[] = {
        { NUMBER,       "NUMBER"        },
        { VARIABLE,     "VARIABLE"      },
        { OPERATOR,     "OPERATOR"      },
        { STATEMENT,    "STATEMENT"     }
};
const size_t sizeOfArrayWithValueType = sizeof( arrayWithValueType ) / sizeof( arrayWithValueType[ 0 ] );
