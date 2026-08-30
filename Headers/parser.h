#ifndef PARSER__H
#define PARSER__H

#include"../Headers/ast.h"
#include"../Headers/macros.h"

typedef enum{
    INTSUM = 0,
    CONCATSUM,
    SUB,
    MULT,
    DIVI,
    CONVERT,
    FCALL,
} actionTypes;

typedef enum {
    STRINGARRAYTYPE = 0,
    NUMBERARRAYTYPE,
} arrayType;

typedef struct arrayVar{
    arrayType type; //maybe not needed? I can just put it as variableType?
    int length;
    union value {
        char* stringValue;
        double* numberValue;
    } value;
} arrayVar;

typedef struct {
    int index;
    int intialised; //TODO: check and use this
    char* type;
    char* name;
    union {
        int intVal;
        double floatVal;
        //char* stringVal;
        arrayVar arrayVar;
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

typedef struct {
    variableTypes varType;
    union {
        char* stringVal;
        int intVal;
        float floatVar;
    } value;
} binOpResult;

void parse(Node* ast);
void createLowLevelFunc(char* name, dynamicNode inputs);

STRUCT_DYNAMIC_ARR_MACRO(varStruct, dynamicVar)
STRUCT_DYNAMIC_ARR_MACRO(funcStruct, dynamicFunc)
STRUCT_DYNAMIC_ARR_MACRO(gotoStruct, dynamicGoto)

#endif