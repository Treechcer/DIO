#ifndef PARSER__H
#define PARSER__H

#include"..\Headers\ast.h"
#include"..\Headers\macros.h"

typedef struct varStruct {
    int index;
    char* type;
    char* name;
    union {
        int intVal;
        double floatVal;
        char* stringVal;
    } data;
} varStruct;

STRUCT_DYNAMIC_ARR_MACRO(varStruct, dynamicVar)

void parse(Node* ast);

#endif