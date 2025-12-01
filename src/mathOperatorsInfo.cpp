#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "mathOperatorsInfo.h"
#include "mathematicalCalculator.h"

informationWithMathOperators arrayWithMathInfo[] = {
        { ADD   , "ADD"     , "+"       ,   doAdd   ,   TWO_ARG    },
        { SUB   , "SUB"     , "-"       ,   doSub   ,   TWO_ARG    },
        { MUL   , "MUL"     , "*"       ,   doMul   ,   TWO_ARG    },
        { DIV   , "DIV"     , "/"       ,   doDiv   ,   TWO_ARG    },
        { LN    , "LN"      , "ln"      ,   doAdd   ,   ONE_ARG    },
        { LOG   , "LOG"     , "log"     ,   doAdd   ,   TWO_ARG    },
        { POW   , "POW"     , "^"       ,   doAdd   ,   TWO_ARG    },
        { SIN   , "SIN"     , "sin"     ,   doAdd   ,   ONE_ARG    },
        { COS   , "COS"     , "cos"     ,   doAdd   ,   ONE_ARG    },
        { TG    , "TG"      , "tg"      ,   doAdd   ,   ONE_ARG    },
        { CTG   , "CTG"     , "ctg"     ,   doAdd   ,   ONE_ARG    },
        { ARCSIN, "ARCSIN"  , "arcsin"  ,   doAdd   ,   ONE_ARG    },
        { ARCCOS, "ARCCOS"  , "arccos"  ,   doAdd   ,   ONE_ARG    },
        { ARCTG , "ARCTG"   , "arctg"   ,   doAdd   ,   ONE_ARG    },
        { ARCCTG, "ARCCTG"  , "arcctg"  ,   doAdd   ,   ONE_ARG    },
        { SH    , "SH"      , "sh"      ,   doAdd   ,   ONE_ARG    },
        { CH    , "CH"      , "ch"      ,   doAdd   ,   ONE_ARG    },
        { TH    , "TH"      , "th"      ,   doAdd   ,   ONE_ARG    },
        { CTH   , "CTH"     , "cth"     ,   doAdd   ,   ONE_ARG    },
        { EXP   , "EXP"     , "exp"     ,   doAdd   ,   ONE_ARG    },
        { SQRT  , "SQRT"    , "sqrt"    ,   doAdd   ,   ONE_ARG    }
};
const size_t sizeOfMathArray = sizeof( arrayWithMathInfo ) / sizeof( arrayWithMathInfo[ 0 ] );

informationWithStatements arrayWithStatements[] = {
        { ASSIGNMENT    ,   "ASSIGNMENT"    ,   "="          },
        { OPERATOR_END  ,   "OP_END"        ,   "gusi"       }
};
const size_t sizeOfStatementArray = sizeof( arrayWithStatements ) / sizeof( arrayWithStatements[0] );

size_t startIndexForArrayWithVar = 0;
informationWithVariables* arrayWithVariables = ( informationWithVariables* )calloc( startIndexForArrayWithVar + 1, sizeof( informationWithVariables ) );
informationWithVariablesArray infoForVarArray = { startIndexForArrayWithVar + 1, startIndexForArrayWithVar };

informationWithValueType arrayWithValueType[] = {
        { NUMBER, "NUMBER"},
        { VARIABLE, "VARIABLE"},
        { OPERATOR, "OPERATOR"}
};
const size_t sizeOfArrayWithValueType = sizeof( arrayWithValueType ) / sizeof( arrayWithValueType[ 0 ] );
