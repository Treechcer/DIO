#ifndef PARSER__H
#define PARSER__H

#include"../Headers/ast.h"
#include"../Headers/macros.h"

typedef struct varStruct {
    int index;
    int intialised; //TODO: check and use this
    char* type;
    char* name;
    union {
        int intVal;
        double floatVal;
        char* stringVal;
    } data;
} varStruct;

typedef struct funcStruct {
    int index;
    int initialised;
    int isLowLevel;
    char* name;
    Node* codeBlock;
} funcStruct;

typedef struct gotoStruct {
    int index;
    int intialised;
    char* name;
} gotoStruct;

STRUCT_DYNAMIC_ARR_MACRO(varStruct, dynamicVar)
STRUCT_DYNAMIC_ARR_MACRO(funcStruct, dynamicFunc)
STRUCT_DYNAMIC_ARR_MACRO(gotoStruct, dynamicGoto)

void parse(Node* ast);
void createLowLevelFunc(char* name);

#endif