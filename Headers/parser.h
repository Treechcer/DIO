#ifndef PARSER__H
#define PARSER__H

#include"../Headers/ast.h"
#include"../Headers/macros.h"

typedef enum{
    SUM = 0,
    SUB,
    MULT,
    DIVI,
    CONVERT,
} actionTypes;

typedef struct {
    int index;
    int intialised; //TODO: check and use this
    char* type;
    char* name;
    union {
        int intVal;
        double floatVal;
        char* stringVal;
    } data;
    variableTypes typedVar;
} varStruct;

typedef struct {
    int index;
    int initialised;
    int isLowLevel;
    char* name;
    Node* codeBlock;
    dynamicNode inputs;
} funcStruct;

typedef struct {
    int index;
    int intialised;
    char* name;
} gotoStruct;

void parse(Node* ast);
void createLowLevelFunc(char* name);

STRUCT_DYNAMIC_ARR_MACRO(varStruct, dynamicVar)
STRUCT_DYNAMIC_ARR_MACRO(funcStruct, dynamicFunc)
STRUCT_DYNAMIC_ARR_MACRO(gotoStruct, dynamicGoto)

#endif