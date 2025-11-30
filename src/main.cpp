#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "tree.h"
#include "treeDump.h"
#include "mathDump.h"
#include "parseFileDataBase.h"
#include "mathematicalCalculator.h"

int main(){

    tree_t tree = {};

    createTreeByRecursiveDescent( &tree );

    dumpTree( &tree );
    destroyTree( &tree );

    printArrayWithVariablesInFile();
    destroyArrayWithVariables();
    return 0;
}



