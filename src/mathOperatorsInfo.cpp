#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "mathOperatorsInfo.h"
#include "mathDump.h"
#include "mathematicalCalculator.h"

informationWithMathOperators arrayWithMathInfo[] = {
{ ADD   , "ADD"     , "+"       , printCenterFunctionInLatex    , "+"           ,   doAdd   , differentiationAdd    ,   TWO_ARG    }, //complete
{ SUB   , "SUB"     , "-"       , printCenterFunctionInLatex    , "-"           ,   doSub   , differentiationSub    ,   TWO_ARG    }, //complete
{ MUL   , "MUL"     , "*"       , printCenterFunctionInLatex    , "\\cdot "     ,   doMul   , differentiationMul    ,   TWO_ARG    }, //complete
{ DIV   , "DIV"     , "/"       , printFunctionOfTwoArguments   , "\\frac{"     ,   doDiv   , differentiationDiv    ,   TWO_ARG    }, //complete
{ LN    , "LN"      , "ln"      , printFunctionOfOneArguments   , "ln"          ,   doAdd   , differentiationLn     ,   ONE_ARG    }, //complete
{ LOG   , "LOG"     , "log"     , printFunctionOfTwoArguments   , "\\log_{"     ,   doAdd   , differentiationLog    ,   TWO_ARG    },
{ POW   , "POW"     , "^"       , printFunctionWithOneStaples   , "^{"          ,   doAdd   , differentiationPow    ,   TWO_ARG    }, //complete
{ SIN   , "SIN"     , "sin"     , printFunctionOfOneArguments   , "sin "        ,   doAdd   , differentiationSin    ,   ONE_ARG    }, //complete
{ COS   , "COS"     , "cos"     , printFunctionOfOneArguments   , "cos "        ,   doAdd   , differentiationCos    ,   ONE_ARG    }, //complete
{ TG    , "TG"      , "tg"      , printFunctionOfOneArguments   , "tg "         ,   doAdd   , differentiationTg     ,   ONE_ARG    }, //complete
{ CTG   , "CTG"     , "ctg"     , printFunctionOfOneArguments   , "ctg "        ,   doAdd   , differentiationCtg    ,   ONE_ARG    }, //complete
{ ARCSIN, "ARCSIN"  , "arcsin"  , printFunctionOfOneArguments   , "arcsin "     ,   doAdd   , differentiationArcsin ,   ONE_ARG    }, //complete
{ ARCCOS, "ARCCOS"  , "arccos"  , printFunctionOfOneArguments   , "arccos "     ,   doAdd   , differentiationArccos ,   ONE_ARG    }, //complete
{ ARCTG , "ARCTG"   , "arctg"   , printFunctionOfOneArguments   , "arctg "      ,   doAdd   , differentiationArctg  ,   ONE_ARG    }, //complete
{ ARCCTG, "ARCCTG"  , "arcctg"  , printFunctionOfOneArguments   , "arcctg "     ,   doAdd   , differentiationArcctg ,   ONE_ARG    }, //complete
{ SH    , "SH"      , "sh"      , printFunctionOfOneArguments   , "sh "         ,   doAdd   , differentiationSh     ,   ONE_ARG    }, //complete
{ CH    , "CH"      , "ch"      , printFunctionOfOneArguments   , "ch "         ,   doAdd   , differentiationCh     ,   ONE_ARG    }, //complete
{ TH    , "TH"      , "th"      , printFunctionOfOneArguments   , "th "         ,   doAdd   , differentiationTh     ,   ONE_ARG    }, //complete
{ CTH   , "CTH"     , "cth"     , printFunctionOfOneArguments   , "cth "        ,   doAdd   , differentiationCth    ,   ONE_ARG    }, //complete
{ EXP   , "EXP"     , "exp"     , printFunctionWithOneStaples   , "e^{"         ,   doAdd   , differentiationExp    ,   ONE_ARG    }, //complete
{ SQRT  , "SQRT"    , "sqrt"    , printFunctionWithOneStaples   , "\\sqrt{"     ,   doAdd   , differentiationSqrt   ,   ONE_ARG    }  //complete
};
const size_t sizeOfMathArray = sizeof( arrayWithMathInfo ) / sizeof( arrayWithMathInfo[ 0 ] );

size_t startIndexForArrayWithVar = 0;
informationWithVariables* arrayWithVariables = ( informationWithVariables* )calloc( startIndexForArrayWithVar + 1, sizeof( informationWithVariables ) );
informationWithVariablesArray infoForVarArray = { startIndexForArrayWithVar + 1, startIndexForArrayWithVar };

informationWithValueType arrayWithValueType[] = {
        { NUMBER, "NUMBER"},
        { VARIABLE, "VARIABLE"},
        { OPERATOR, "OPERATOR"}
};
const size_t sizeOfArrayWithValueType = sizeof( arrayWithValueType ) / sizeof( arrayWithValueType[ 0 ] );
